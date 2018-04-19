= 実際にやってみる

粛々とやれるところをやっていきましょう。
今回の本の内容は@<href>{https://github.com/vvakame/review-css-typesetting}に置いておく予定です。
4/23以降になってもこのURLのリポジトリがpublicになっていなかったり、内容が空だったりした場合、運営のタスクで忙殺されたんだな…と察してください。
そして、Twitterで『まだリポジトリ公開されないんですか？』と煽ってください。

また、現在の筆者によるCSS組版のフローは試行錯誤の道中であり、苦悶の欠片であり、鵜呑みにすると痛い目を見ること間違いなしです。

CSSについての細かい内容については前述したpentapodさんの本を参照してください。
僕はこの本で説明されている内容を我々の流儀っぽく適用できないか試したり調べたりしただけです。
みんな、買おう！

また、筆者が試して書いた内容について、実は書き方が悪いだけでやり方次第ではちゃんと動く、ということもあるかもしれません。
その場合、前述のリポジトリにIssueとして報告してもらえると大変うれしいです。
PRをいただけると神になります。

== TechBoosterのワークフロー with Re:VIEW

まずは我々の執筆フローをおさらいしていきます。

 1. GitHubで原稿管理
 2. Re:VIEW形式で原稿執筆
 3. Dockerのvvakame/reviewイメージ@<fn>{docker-review}でビルド（PDF生成）
 4. Circle CIで毎回ビルド&チェック
 5. 羊が睡眠時間を爆発四散させながら編集&入稿する

//footnote[docker-review][@<href>{https://hub.docker.com/r/vvakame/review/} Docker HubとGitHub両方にスターお願いします！]

その他、詳しいことについては『技術書をかこう！〜はじめてのRe:VIEW〜』@<fn>{firststepreview}を参照してください。
たぶん書いてあったような気がします。
書いてなかったら許してください。

#@# prh:disable
//footnote[firststepreview][@<href>{https://github.com/TechBooster/C89-FirstStepReVIEW-v2}]

このフローを踏襲し、利用するDockerイメージを変更するだけでCSS組版による入稿データが得られる、というのが目指すべき到達点ですね。

== ブラウザからPDF出力の下準備

#@# prh:disable:言う
何はなくとも同人誌を刷らないといけないので入稿可能なデータ、もっと言うとPDF形式での出力を行う必要があります。
本文の入稿データを作るためには単一のPDFにする必要があります。
ブラウザの印刷機能でPDFを生成するので、本全体を単一のHTMLにしておく必要があります。

Re:VIEWでは各章をHTMLに変換することができますが、すべての章をconcatして出力することはできません。
そこで、すべての章をなるべく余計な出力なしにHTMLで出力し、後から別処理でconcatしてやる必要があります。
具体的に、layouts/layout.html.erbを作成し、中身は@<code>{<%= @body %>}のみとしました。

今回、Vivliostyleの利用にはChrome拡張版@<fn>{viv-chrome-ext}を利用します。
真面目にやるなら再現性を考えてnpm経由でvivliostyleを利用するべきでしょう。

#@# prh:disable
//footnote[viv-chrome-ext][@<href>{https://chrome.google.com/webstore/detail/vivliostyle/ffeiildjegeigkbobbakjjmfeacadbne?hl=ja}]

== 1枚のHTMLを出力する！気合で

Chromeで単一のPDFを生成したいので、すべての章を1つのHTMLデータに結合する必要があります。
Re:VIEWは1章を1つの.reファイルにし、これを1つの.htmlファイルに変換します。
このままでは都合が悪いため、なんとかしてconcatする必要があります。

Re:VIEWがHTMLファイルを出力するとき、<html>タグや<body>タグなどの今回のユースケースでは邪魔なタグが出力されます。
これを排除するために、Re:VIEWが使うHTMLファイル出力時に使うテンプレートに手を加えます。
@<code>{layouts/layout.html.erb}を置くと、これがテンプレートとして使われるようになります。
中身は@<list>{layouts/layout.html.erb}のように大変簡素なものにしました。

#@# prh:disable
//list[layouts/layout.html.erb][<body>の中身が出ればよい！]{
#@mapfile(../articles/layouts/layout.html.erb)
<%= @body %>
#@end
//}

これで章毎のパーツが手に入るので、htmlやbodyタグを補ってやります。
この処理はTypeScriptで書き、catalog.ymlやconfig.ymlを読み込みつつ気合でconcatしました。

最終的には次のような処理を行うことになりました。

 1. 扉を出力
 1. 目次を出力
 1. 各章をsectionタグで包みidを振って本文を出力
 1. 奥付を出力

== 扉や奥付のページを表示したい！

扉というのは、表紙をめくった普通の紙の1枚目、奥付というのは裏表紙をめくった普通の紙の1枚目のことを指します。
扉には書名、著者名が。
奥付には書名、著者名、発行者、印刷者、出版年月日などの情報が記してある場合が多いです。

今回はconfig.ymlから情報を取ってきて扉と奥付を生成します。
出力する項目はlatex版で出力しているものを踏襲します。
扉には@<list>{title-page}のように、書名、著者名、刊行日、発行所を出力します。
奥付には@<list>{colophon}のように、書名、発行年月、著者名、編集者名、発行所、権利表記を出力します。

#@# prh:disable
//list[title-page][扉の例]{
<aside>
  <h1>${config.booktitle}</h1>
  <div id="author">${config.aut.join(", ")} 著</div>
  <div id="date">${config.date} 版</div>
  <div id="pbl">${config.pbl} 発行</div>
</aside>
//}

#@# prh:disable
//list[colophon][奥付の例]{
<aside>
  <div>${config.booktitle}</div>
  <div>${config.history.map(s => s.join("<br>")).join("<br>")}</div>
  <div>著者 ${config.aut.join(", ")}</div>
  <div>編集 ${config.edt.join(", ")}</div>
  <div>発行所 ${config.pbl}</div>
  <div>${config.rights}</div>
</aside>
//}

あとは、これらの要素をCSSで装飾して見た目をlatexに近づけてやればOKです。
僕はCSSが嫌いなので@<fn>{why-write-this-contents}、まだ頑張ってません。

//footnote[why-write-this-contents][なぜこの記事を書こうと思ったんだ！？ と思ったアナタ、気にしてはいけません]

HTMLのセマンティクス的にはここに<aside>を使うのは正しいんですかね…？（わからぬ）

== 目次を表示したい！

目次を出力します。
目次の出し方はpentapodさんの本を見てください。

1点注意点があって、vivliostyle.jsのChrome拡張を使って作業している場合、hashbangがある状態でリロードしてもう一回拡張を使うと、target-counterの解決が行われないようです。
目次の採番の計算が始まらない場合、URLの#以降を削ってリトライするとうまくいくようです。

さて、当然のことですが、Re:VIEWを使っているので目次を手書きしたくはないわけです。
そのため、目次を動的に生成する必要があります。
アプローチとしては2種類考えられ、ひとつはHTML生成時に目次も生成して一緒くたに出力する（静的処理）。
もうひとつはHTMLを生成した後に、JavaScriptを使ってDOMを走査し目次を後付で生成する（動的処理）。
今回はvivliostyle.jsとの噛み合わせやタイミングについて悩むのが嫌だったので前者にしました。

さて、Re:VIEWからなんとかして目次に関する情報を取得したいわけですが、Re:VIEWをRubyで無理やり改造したりするのはやりたくありません@<fn>{authors-favorite-lang}。

//footnote[authors-favorite-lang][筆者の使い慣れた言語はTypeScriptとGoです]

#@# prh:disable
Re:VIEWの外から目次情報を得る手段を考えます。
review-indexコマンドというものがあり、ここからデータを得ることができます。
そのままでは機械処理しにくいデータしか得られないので、あるオプションを使い機械可読性のあるHTML形式の結果を得ます。
具体的に、@<code>{review-index -a --html}とすると目次のhtmlが得られます。
このオプションは@<code>{(deprecated)}と書かれているのですが今は気にしないことにします。
これをjsdomに突っ込んでNode.js上で気合でポンすると目次データが得られます。

くぅー疲れました！これにて目次終了です！
なんだかんだ言ってPREDEFとかPOSTDEFとか深さ1でいいしコラムは避けたいしとかがめんどくさいので素直にRuby書いてJSONとかでダンプしたほうが楽だと思います。
catalog.ymlやconfig.ymlやその他一切合切を読み込んでnormalizeしてしてJSONでダンプしてくれる機能が欲しいですね…@<fn>{review.js}。

//footnote[review.js][筆者はreview.jsという実装を作っていたことがあるのですが、これがとても欲しい気持ちに… 再点火するべきかしら？]

== ノンブルを打ちたい！

ノンブル（ページ番号）を打っていきます。
詳細はpentapodさんの本を読め！終了！

一応コード例を出しておくとこういう感じです（@<list>{nombre}）。
ブラウザ上で見た目の確認をしていると、今見ているのが左ページか右ページかわからなくなるのでノンブルと共に矢印を出しています。

#@# prh:disable
//list[nombre][ページ下部にノンブルを出す]{
@page :left {
  @bottom-center {
    content: "←" counter(page);
  }
}
@page :right {
  @bottom-center {
    content: counter(page) "→";
  }
}
//}

カウンタの仕様はCSS2で定義されています@<fn>{counter-spec}。
counterに与えるpageという識別子は、CSS Paged Media Module Level 3という仕様にプリセットで定義されている@<fn>{page-counter}現在のページ数を表すカウンタです。

//footnote[counter-spec][カウンタ周りの仕様 @<href>{https://www.w3.org/TR/CSS2/generate.html#counters}]
//footnote[page-counter][@<href>{https://drafts.csswg.org/css-page-3/#page-based-counters}]

//comment{
 * https://caniuse.com/#feat=css-counters
 * https://caniuse.com/#feat=css-paged-media
//}

== 柱に現在表示中の章タイトルを表示したい！

柱というのは、本のページの上の部分のことです（@<img>{header}）。
ここには、既存のLaTeXのスタイルでは、左側のページには現在表示している章のタイトル、右側のページには現在表示している節のタイトルを表示しています。
これをCSS組版で再現できないか試してみました。

//image[header][柱][scale=0.5]{
//}

まずは結論から。
現時点では大変めんどくさい。
Running headers and footersの仕様と、名前付き@pageルールがサポートされていないのが敗因。

CSS Generated Content for Paged Media Module@<fn>{css-gcpm}という仕様があり、ざっくりいうとある要素の内容をコピーし、柱などに転記する仕様、脚注について処理する仕様を含みます。
脚注については後ほど別途言及します。

//footnote[css-gcpm][@<href>{https://drafts.csswg.org/css-gcpm/}]

まずは、@<code>{element}と@<code>{running}を使った作戦です（@<list>{running-element}）。

#@# prh:disable
//list[running-element][イケると思った作戦1]{
@page :right {
  @top-right {
    content: element(chaptitle);
  }
}
section h1 {
  position: running(chaptitle);
}
//}

Re:VIEWで出力したsectionとh1タグという構造があれば、そのテキストは章タイトルです。
これを柱にコピーしてやれば話は簡単。
節タイトルも同様にsectionとh2タグという構造を元にクリアできるでしょう。

しかし、残念ながら筆者が試した範囲ではこの仕様はまだサポートされていない@<fn>{chaptitle-forum}ようでした。
そのため、この案はボツです。

//comment{
Vivliostyle Viewerでも試したけどダメそうだった
//}

//footnote[chaptitle-forum][2016年のフォーラムの投稿を見ると別の（W3C仕様ではない）仕様でやれと書かれている @<href>{https://groups.google.com/forum/?hl=ja#!topic/vivliostyle-ja/ejSavhBOZ5g}]

さて、次にCSSに章タイトルを埋め込み、名前付き@pageを使って出力する作戦です。
節タイトルをこれで処理するのは難しいですが、章レベルだったらイケるのでは…！？
つまりはRe:VIEWから得られるデータをフル活用してビルド時に柱表示用のCSSを組み立てるのです。

と思って試したCSSが@<list>{named-page-rule}です。
これもサポートされていないのか、なんの出力も得られませんでした！
この手法だと章レベルはなんとかなりますが節レベルの表示のサポートは厳しそうです。

#@# prh:disable
//list[named-page-rule][イケると思った作戦2]{
@page foo:right {
  @top-right {
    content: "fooの章タイトル";
  }
}
section.foo {
  page: foo;
}
//}

悲しい。
将来的に@<list>{running-element}がうまく動くようになるのを祈っています。

== テキスト周りの見た目をなんとかしたい！

テキスト周りの見た目をなんとかしたい！
ので、なんとかしましょう。

デフォルトの状態だと、いわゆる紙の本的な見た目とは程遠いので修正していきます。

まずは、段落頭の字下げです。
小学生の頃、作文用紙に文を書く時に最初は1マスあけたアレです。
CSS的にはtext-indentでできるので、1文字分下げます（@<list>{text-indent}）。

//list[text-indent][1文字分字下げする]{
p {
  text-indent: 1rem;
}
//}

次は両端揃えです。
文章の右端がギザギザしていると恥ずかしいですからね。
CSS的にはtext-alignでできるので、両端揃えにします（@<list>{text-align}）。

//list[text-align][両端揃えにしましょう]{
p {
  text-align: justify;
}
//}

あとは、フォントとフォントサイズと文字間、行間などの調整です。
@mediaルールを使って、screenに出す時、pageで出す時、printに出す時で設定を細かく調整するとよいでしょう。
ついでに、リンクの装飾を無くして見た目的にリンクと分かるように斜体にしたりもします（@<list>{text-misc}）。
だいたいpentapodさんの設定例を踏襲しています。

#@# prh:disable
//list[text-misc][screenとprintでフォント周りを個別にカスタマイズしたりする]{
body {
  font-family: 'Noto Sans JP', sans-serif;
}
@media screen {
  :root {
    line-height: 1.3;
  }
}
@media print {
  :root {
    font-weight: 500;
    line-height: 1.3;
  }
  a {
    text-decoration: none;
    font-style: italic;
  }
  a:link,
  a:visited {
    color: black;
  }   
}
//}

Webフォントも普通に使えます。
Notoフォントなどを使うと見栄えがよりソレっぽくなりますが、フォントデータのサイズがでかいのでmediaがprintのときだけ使えばいいのではないでしょうか。
Webフォントを使っておけばPDFへのフォント埋め込みも問題ない！とかだとすごく楽なんですけどね…。

== 脚注を表示したい！

はい。脚注@<fn>{remember-css-gcpm}です。同人誌を作ると脚注芸をしたくなってしまう確率は100%@<fn>{we-want-footnote}といわれています。
この欠かすことの出来ない要素をやっていきます。

//footnote[remember-css-gcpm][脚注の仕様はCSS Generated Content for Paged Media Module@<fn>{css-gcpm}に含まれます]
//footnote[we-want-footnote][サンプル数は1です]

単に脚注にするだけなら、@<list>{simple-footnote}のようにするだけでOKです。

//list[simple-footnote][極単純な脚注の実現]{
p.footnote {
  float: footnote;
}
a.noteref {
  vertical-align: super;
}
//}

こうするだけで、Re:VIEWのfootnoteブロック構文で書いた箇所がページ下部に移動され、線で区切られます。

ここから、一歩進んで脚注の番号の採番をRe:VIEWで行うのではなく、ブラウザの機能で行ってみます。
Re:VIEW記法で書いている最中に脚注や図版の採番を気にしないのと同様に、出力されたHTML上でも採番を気にしないで済むならそれにこしたことはありません。

さて、pentapodさんの本で紹介されているように、1要素で参照元・参照先の両方を記述するなら特に問題はありません。
しかし、Re:VIEWの場合はインラインのfn記法と、ブロックのfootnote記法を組合せて脚注を実現します。
このため、参照元・参照先の採番ルールをなんとか上手く関連付けてやる必要があります。

そこで筆者が試したのが@<list>{autoincrement-footnotoe}、@<list>{autoincrement-footnote-reviewext}です。
Re:VIEWのHTMLBuilderで採番を出力させないようにするため、review-ext.rbを使って細工をします。

#@# prh:disable
//list[autoincrement-footnotoe][自動採番される脚注]{
section {
  counter-reset: footnote;
}
p.footnote {
  counter-increment: footnote;
}
p.footnote::footnote-marker {
  content: '* ' counter(footnote);
}
a.noteref::before {
  content: '*' target-counter(attr(href), footnote);
  vertical-align: super;
}
//}

#@# prh:disable
//list[autoincrement-footnote-reviewext][review-ext.rb]{
module ReVIEW
  class HTMLBuilder
    def footnote(id, str)
      puts %Q(<p class="footnote" id="fn-#{normalize_id(id)}">
              <span>#{compile_inline(str)}</span></p>)
    end
    def inline_fn(id)
      %Q(<a id="fnb-#{normalize_id(id)}" 
          href="#fn-#{normalize_id(id)}" class="noteref"></a>)
    end
  end
end
//}

書き方的には正しいのですが、vivliostyle.jsのChrome拡張では上手くいきません@<fn>{target-count-with-viv-viewer}でした。
脚注に変換されるpタグのidが書き換えられてしまい、aタグのhrefの参照が壊れ番号が参照できなくなります。

//footnote[target-count-with-viv-viewer][なお、Vivliostyle Viewer上だと正しくレンダリングされます。このtipsもpentapodさんの本に書いてあります。]

また、この脚注の見た目や採番が行われるのは、mediaがprintのときだけです。
mediaがscreenのときは特になんの変更も行われないため、無関係な文章が無造作に間に挟まれているかのような見た目になります。
つまり、Webページとして表示することを考えると、脚注部分の表示が意図しない、壊れた見た目になってしまうわけです。

これらの都合を勘案すると、今のところRe:VIEWに採番をやらせたままの状況のほうが、マルチユースという観点では都合がよいと考えています。

== 画像をセンタリングしたい！

そのまんまですね。
やっていった結果が@<list>{image-center}です。

//list[image-center][センタリングする]{
div.image {
  text-align: center;
}
p.caption {
  text-align: center;
  text-indent: 0;
}
p.caption::before {
  color: gray;
  content: "▲";
}
//}

pにはすでに字下げがついているので、打ち消す設定を付与しておきます。
また、キャプションにはlatex版を踏襲し約物を追加しました。
コードやテーブル系のキャプションにも同様の設定を追加するとよいでしょう。

== ボックス系の見た目をいい感じにしたい！

これもやっていきましょう（@<list>{box-style}、@<img>{box-example}）。
見た目はlatex版の出力をなるべく踏襲していきます。
ゼロから自分でひねり出すのが辛かったのでRe:VIEWのepub用cssをかなり流用しています。
特筆すべきことはあまりなさそう。

//list[box-style][ボックス系の見た目をそれっぽくする]{
pre.cmd {
  background-color: #444;
  color: white;
  padding: 1rem 2rem;
}
pre.list, pre.emlist {
  padding: 1rem 2rem;
  background-color: #eee;
  border: solid 3px gray;
  border-radius: 0.3rem;
}
div.column {
  padding: 0.8rem 1.5rem;
  border: solid 3px gray;
  border-radius: 0.3rem;
}
div.column h4 {
  margin-top: 0;
}
table {
  margin: 0 auto 2em auto;
  border-collapse: collapse;
}
table tr th {
  border:1px black solid;
  font-size: 0.9rem;
  padding: 0.3rem;
}
table tr td {
  border:1px black solid;
  font-size: 0.9rem;
  padding: 0.3rem;
}
p.tablecaption, table caption {
  color: #666;
  font-weight: bold;
  text-indent: 0;
}
//}

//image[box-example][描画結果][scale=0.5]{
//}

//comment{
 * テーブルで ----- 的なやつとかが表示されるテーブルとちゃんと消えるテーブルがあるような…？
//}

== ページ上部に雑に線引きたい！

気合でシュッとやるだけです（@<list>{page-top-boader}）。
章タイトルとかを@pageルールを使って表示している場合、いい感じにmergeしてやる必要があります。

#@# prh:disable
//list[page-top-boader][@pageルールで頑張って線引くだけ]{
@page {
  padding: 1rem;
  @top-left {
    content: "　"; /* 内容ないと枠でない */
    border-bottom: solid 1px black; 
  }
  @top-right {
    content: "　"; /* 内容ないと枠でない */
    border-bottom: solid 1px black; 
  }
  @top-center {
    content: "　"; /* 内容ないと枠でない */
    border-bottom: solid 1px black; 
  }
}
//}

パワーを感じますね。
引いた線とページ本体部分のテキストがくっつきすぎるので:rootに対してpadding-topの設定があったほうがよいでしょう。

== Vivliostyle Viewerを使ってデータを表示する

#@# prh:disable
Chrome拡張版のvivliostyle.jsを使うのもよいですが、レンダリングのマトモさという面ではVivliostyle Viewer@<fn>{vivliostyle-viewer}を使うのがよいです。
ダウンロードしてstart-webserverを叩きます。

//footnote[vivliostyle-viewer][@<href>{http://vivliostyle.com/ja/products/}]

自分の本を開く時は相対パスではなく、ローカルサーバを立ててから開くのがお勧めです。
@<code>{npx live-server --port=8989 --no-browser --cors --verbose}とかでOKです。

PDFを出力するときは、Viliostyle Viwerのドキュメントにかかれているとおり、MarginsはNone、Background graphicsのチェックはONでPDFとして保存します。
