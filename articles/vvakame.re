= Re:VEIWとCSS組版

嬉し恥ずかしCSS組版の話です。
残念ながら、CSS組版で本を作りきった話にはなりませんでした。

現在、TechBoosterではRe:VIEW@<fn>{review}+LaTeX@<fn>{latex}でPDFを生成し入稿するという書籍制作フローです。
これは非常に安定していて、Dockerのvvakame/reviewイメージ@<fn>{docker-review}を使い、Circle CIなどで継続的インテグレーションを行うワークフローが確立されています。
つまり、我々エンジニアが普段行っている開発のサイクルとまったく同じやりかたで執筆を行えるのです。
グッドですね。

//footnote[review][@<href>{https://github.com/kmuto/review/}]
//footnote[latex][@<href>{https://www.latex-project.org/}]
//footnote[docker-review][@<href>{https://hub.docker.com/r/vvakame/review/} Docker HubとGitHub両方にスターお願いします！]

=={why-css-typesettings} なぜCSS組版か？

ブラウザをPDF生成エンジンとし、CSSで見た目を調節する。
これはつまり、いつも使っているChrome DevToolsをも使って試行錯誤できるということです。
これは、LaTeXに比べるとより多くのエンジニア@<fn>{vvakame-oriented}が紙面の組み立てを自由に行えるということです。

//footnote[vvakame-oriented][筆者はWeb界隈を主戦場としているので視野が偏っている感は否めません]

実際に、Twitterでアンケートを取ってみました。
選択肢は次のとおり。

 1. LaTeXでならLaTeXでなら自由にレイアウトがいじれると思う
 2. CSSでなら自由にレイアウトがいじれると思う
 3. 両方イケるで！
 4. 両方だめです…

結果は、@<img>{questionnaire}のとおり、CSSでなら自由に組めるが37%で堂々の一位！
2位の両方イケるで！12%と合わせると49%となり、ほぼ半数がCSS組版を待ち望んでいるのです。

//image[questionnaire][アンケート結果 CSSは望まれている…！][scale=0.7]{
//}

LaTeXによるワークフローが使えなくなるわけでもないので、CSS組版の導入により64%のサークルが紙面を自分好みにカスタマイズできる計算になります。
勝ったなガハハ！

#@# prh:disable:わかる
この他に筆者が不満に思っていたのは、書籍の中に遊び心をトッピングし、紙面で遊んだりするのが難しいことです。
平たくいうと、Emojiとか使ったりしたいわけです。
『祝！リリース！🎉』@<fn>{tada}みたいな。
他にも、本の小口のあたりからSDキャラとかが チラッ ってしてたりとか。
したいですよね！！したい。わかる。

//footnote[tada][リリース！の後に @<code>{:tada:} が入ってます。LaTeXでは出力されていないはず]

また、多言語化の問題もあり、RTLな言語（アラビア語）などの記述にも難があります。
『اللغة العربية』@<fn>{alabic}みたいな。
プログラミングの話題でRTLに関する話題はちょいちょい俎上にのぼることがあります@<fn>{c87}。
ですが、LaTeXに不慣れな私では実現のための労力を想像しただけで萎えてしまいます。

//footnote[alabic][アラビア語が書いてあります。RTLが混在すると表示が壊れる…]
//footnote[c87][@<href>{https://techbooster.booth.pm/items/75028} 第8章 Issue tracker Watcher など]

さて、Prosはわかりましたが、Consも少し考えてみたいと思います。

まず、すでに固まっているワークフローを一度壊して組み直さなければなりません。
C92のときにSplatoon2ガイド@<fn>{splatoon2-guide}で実験したとおり、すでに印刷所に入稿可能なデータをCircle CI上で苦もなく出力できるようになっています。
しかし、このワークフローは残念ながらLaTeXを経由してPDFを出力する時の手順なのです。

//footnote[splatoon2-guide][@<href>{https://github.com/vvakame/C92-Splatoon2Guide}]

CSSで組版する場合、入稿データを作成するまでに次の課題があると想定できます。

 * 見栄えのよい出力が得られるCSSを書かなければいけない
 * コマンドラインだけで完結してPDFが出力できるのか？
 * 出力されたPDFは印刷所への入稿に適した状態か？
 ** フォントが埋込済か？
 ** PDF/Xフォーマットか？

これらの課題を解決できなければ、現在のワークフローと同等の快適さは得られないのです。
つらぽよですね。
果たして、この課題を解決し、本章の目的を達成することができるのか！？@<fn>{dekinai}

//footnote[dekinai][ネタバレ：できない。俺のやりこみが…たりなかったんやなって……]

さらに高品位な組版がしたければAdobe InDesignを使うという手もありますが、カロリーが高すぎるのでみなかったことにします。

== CSS組版について知ろう！

さて、ではまずはCSS組版そのものについて学んでいきます。
まず、Web関連技術で本を作りたい！という需要があります。
このためにすでにHTML+CSSなどをワンパックにしたepubという仕様があります。
epubファイルの実態はWebページ的なものをzipで圧縮しただけのものです。

次に、電書だけでなく紙の本もHTML+CSSでやりたい！という気持ちになりますね。
ハイなりました。
CSSで組版をしたら、ブラウザの印刷機能を使ってPDFを作成すれば、書籍データの完成です@<fn>{not-yet-completed}。

//footnote[not-yet-completed][少なくとも気持ち的には]

ところが、CSS単体で組版をするには、残念ながら時代が早すぎます。
そのため、vivliostyle.jsと呼ばれるライブラリを使います。
これの助けを借りて、まだブラウザに実装されていない組版用CSSの仕様を利用します。
JavaScriptでいうとBabelを使ってstage 1の仕様を試す感じですね。

詳細はpenntapodさんがC92で出した"CSSではじめる同人誌制作"@<fn>{pentapod-css-book}という本を参照してください。
こちらの本は実際にCSSで組版されていて、見た目もカラーでキレイに組まれて見応えがあります。
完全に神という気持ちになるので紙を作っていきましょう。

//footnote[pentapod-css-book][@<href>{https://pentapod.github.io/c92/}]

=== vivliostyle.jsについて

vivliostyle.jsについて、簡潔な説明を与えるのは難しい気もしますがやってみましょう。

Webサイト@<fn>{vivliostyle.org}によると、VivliostyleプロジェクトはCSSによる組版のシステムを提供します。
これは、電子出版をも志向したWebブラウザ技術をベースにしたものです。

 * 紙向けの組版、電子向けの組版の両方を行えるよう、ブラウザの文字組みとレイアウト機能の強化を行う。
 * CSSによる組版の実装をJavaScriptによって行う（Polyfillする）。
 * W3C標準として、組版関連のCSSの仕様の発展と策定に協力する。

//footnote[vivliostyle.org][@<href>{https://vivliostyle.org/}]

筆者が感じるvivliostyleのよい所は、オープンであり、開発者の方々が標準の仕様策定に対して熱心であることです。
つまり、彼らは将来的にvivliostyle.js無しの、ブラウザ単体でも実用に足る組版を目指しているということです。
これはOSSを愛する、標準を愛するエンジニアのマインドに合致しています。

//comment{
https://github.com/vivliostyle/vivliostyle.js/blob/master/doc/property-doc-generated.md
https://vivliostyle.github.io/vivliostyle.js/docs/supported-features.html
https://github.com/vivliostyle/vivliostyle_ja
//}

まずはこれを使ってCSS組版に挑んでいきます。
最終的には何にも依存することのないHTML+CSSで結果が得られるようになるといいですね。

== 実際にやってみる

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

=== TechBoosterのワークフロー with Re:VIEW

まずは我々の執筆フローをおさらいしていきます。

 1. GitHubで原稿管理
 2. Re:VIEW形式で原稿執筆
 3. Dockerのvvakame/reviewイメージ@<fn>{docker-review}でビルド（PDF生成）
 4. Circle CIで毎回ビルド&チェック
 5. 羊が睡眠時間を爆発四散させながら編集&入稿する

その他、詳しいことについては『技術書をかこう！〜はじめてのRe:VIEW〜』@<fn>{firststepreview}を参照してください。
たぶん書いてあったような気がします。
書いてなかったら許してください。

#@# prh:disable
//footnote[firststepreview][@<href>{https://github.com/TechBooster/C89-FirstStepReVIEW-v2}]

このフローを踏襲し、利用するDockerイメージを変更するだけでCSS組版による入稿データが得られる、というのが目指すべき到達点ですね。

=== ブラウザからPDF出力の下準備

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

=== 1枚のHTMLを出力する！気合で

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

=== 扉や奥付のページを表示したい！

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

=== 目次を表示したい！

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

=== ノンブルを打ちたい！

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

=== 柱に現在表示中の章タイトルを表示したい！

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

=== テキスト周りの見た目をなんとかしたい！

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

=== 脚注を表示したい！

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
      puts %Q(<p class="footnote" id="fn-#{normalize_id(id)}"><span>#{compile_inline(str)}</span></p>)
    end
    def inline_fn(id)
      %Q(<a id="fnb-#{normalize_id(id)}" href="#fn-#{normalize_id(id)}" class="noteref"></a>)
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

=== 画像をセンタリングしたい！

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

=== ボックス系の見た目をいい感じにしたい！

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

=== ページ上部に雑に線引きたい！

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

=== Vivliostyle Viewerを使ってデータを表示する

#@# prh:disable
Chrome拡張版のvivliostyle.jsを使うのもよいですが、レンダリングのマトモさという面ではVivliostyle Viewer@<fn>{vivliostyle-viewer}を使うのがよいです。
ダウンロードしてstart-webserverを叩きます。

//footnote[vivliostyle-viewer][@<href>{http://vivliostyle.com/ja/products/}]

自分の本を開く時は相対パスではなく、ローカルサーバを立ててから開くのがお勧めです。
@<code>{npx live-server --port=8989 --no-browser --cors --verbose}とかでOKです。

PDFを出力するときは、Viliostyle Viwerのドキュメントにかかれているとおり、MarginsはNone、Background graphicsのチェックはONでPDFとして保存します。

== 未解決の課題

はい。そして未解決の課題がいっぱい残っています。
これは読者への課題とします（PR待ってます）！

英語、技術書典運営のタスク、原稿などに挟まれてぎうぎうになっているため調査する時間とMPが足りなくなりました。
よって、ここに筆者の考える残タスクを開陳するものであります。
押忍。

=== 問題の上手な切り分け方

ある仕様が未サポートだったり上手く動かない場合、それはChromeの振る舞いなのか、それともvivliostyle.jsか、切り分けが難しいです。
vivliostyle.jsはブラウザ未サポートのCSSをJSで頑張ってDOMのstyle属性に突っ込んでくれるため、DOM自体の可読性も恐ろしく悪くなります。

誰かナレッジがあったら分けてください…。

=== PDFの生成とフォントの埋込

印刷所に入稿するデータはPDF形式であり、そこにはフォントデータを埋め込まなければなりません@<fn>{why-must-font-embed}。
しかし、現時点ではChromeの印刷機能でPDFを生成する時に、フォントデータを埋め込む方法が分かっていません。

//footnote[why-must-font-embed][印刷に使うフォントと同一のものを印刷所が持っているかわからないし同一性の検証もめんどくさいため]

pentapodさんの本によると、Adobe Acrobatに課金してフォントデータを後乗せで埋め込むという手法が選択肢としてあるそうです。
羊にこの手法を試してもらいましたが、上手くいきませんでした@<fn>{i-cant-understand-it}。

//footnote[i-cant-understand-it][羊が何か理由を説明してたけど筆者が理解できなかったパターン。なんて？？]

よって、今回の印刷ではフォントデータが欠損していても無視して無理やり印刷します。
現状の到達点だから…！仕方ないから…！

宿題に挑戦する読者のために、筆者が調べた範囲で参考になりそうなURLを掲載しておきます。

 * Vivliostyle.jsのフォーラムでの同様のやり取り
 ** @<href>{https://groups.google.com/forum/#!topic/vivliostyle-ja/HCoewxEmLfM}
 * StackOverflowでの同様のやり取り
 ** @<href>{https://stackoverflow.com/questions/11604726/}
 * Google ChromeのIssue
 ** @<href>{https://bugs.chromium.org/p/chromium/issues/detail?id=516317}
 * NotoフォントをWebフォントとして使う
 ** @<href>{https://fonts.google.com/earlyaccess#Noto+Sans+JP}

//comment{
 * kmutoさんはpdfToolbox（有償）を使っているとか…
//}

=== 凝ったデザインへの挑戦

やりたかったやつー！
ページ番号周りでわかめこちゃんがキャッキャしたりして余白が楽しそうなやつー！

やりたかったやつー！
小口のところまでスミ乗っけておいて辞書の検索用索引みたいなの出すやつー！

まぁ多分頑張ればできそうだけどCSS力が圧倒的に足りない。
EPUB Adaptive Layoutの仕様とかを使えばできるものもあるそうですが、標準ではないものはあまり使いたくないし…。

=== HTMLBuilderが吐き出すアンカーとリンクを修正する

Re:VIEWのHTMLBuilderが吐き出すHTMLは1ファイル==1章形式であり、すべてのファイルをconcatする前提ではありません。
なので、単純にconcatしただけだと章間のリンクが壊れたり、同名のfootnoteが複数章にまたがって存在すると不都合が生じます。

これを直すためには、review-ext.rbなどを経由してHTMLBuilderの挙動に手を入れる必要があります。
しかし、ここに手をいれるのはちょっとまとまった時間が必要そうなので、今回はいったん保留とします。

=== CSSの管理方法

CSSをよりよく上手に管理する方法が必要です。
今のところ生CSSを書いていて、たとえば"リスト表示に対するパラメータ設定"みたいなのがどこで意図されて設定されているのか判然としません。
これを解決するために、mixin的な機能などをもったAltCSS（SCSSとか）を導入したほうが健康によさそうだと考えています。
というか、生CSSだとコメントを書くのがめんどくさすぎて憤死しそうです。
CSSこんなめんどくさかったっけ？

最終的に、Re:VIEWユーザがCSS組版のために使える共有財産として設定を育てたいと考えた場合、どこをいじるとどこが変わるのか、パッと見て分かる必要があるでしょう。
自分で書いたCSSが何を意図しているか忘れないうちに…早く…！

=== ボックス系の見た目と脚注が被る話

リストなどのボックス系の見た目のものと、脚注が被ると見た目がおかしくなる場合があります（@<img>{bad-list-and-footnote}）。

//image[bad-list-and-footnote][ページまたぎのリストと脚注の相性の悪さ…]{
//}

これについて、今のところボックス系のものをページまたぎせずに配置するよう指定して迂回しました（@<list>{avoid-list-and-footnote-conflict}）。

//list[avoid-list-and-footnote-conflict][ページまたぎを許さない]{
pre.cmd {
  page-break-inside: avoid;
}
pre.list, pre.emlist {
  page-break-inside: avoid;
}
table {
  page-break-inside: avoid;
}
//}

この方法だと、ページ的に空白の部分が多く生まれてしまい、印刷費用（と頒布費用）が無駄になってしまいます。
ページまたぎが発生しても問題なく表示できる方法を見つけるか、空いた空間にリストなどの先にある文字を流し込めればよさそうです。
今のところ、組版用語としてこのような処理にどういう名前がついているのかわからず調べられずにいます…。
なきわかれ？

=== vivliostyle.jsのレンダリングがリロードで壊れる場合がある

図を見たほうがわかりやすいんですが、@<img>{bad-reset-hashbang}のようになります。
ボックスの改ページと脚注が被ったりします。

//image[bad-reset-hashbang][レンダリングが被る場合がある]{
//}

#@# prh:disable
これは、ページ途中でリロードした時に発生しがちな現象です。
Vivliostyle Viewer、Chrome拡張共に発生します。
これは解決可能で、vivliostyle.jsが生成するURLのhashbang部分を削って、本の先頭からレンダリングさせると改善します。
Vivliostyle Viewerを使っている時のURL末尾に付く@<code>{&f=epubcfi(/2!/4/12[chapter-vvakame]/139:0)}のような部分ですね。
おそらく、vivliostyle.jsのレンダリングが、本の先頭からのときと本の途中からのときで挙動が違うのだと考えられます。

この現象を完全に回避する方法は今のところ筆者は発見できていないので、PDF生成後に全部のページレイアウトを目で毎回確認する必要があります。

=== CI上でPDFを生成させ再現性を持たせる

これに至ってはまったく未着手です。

多人数で執筆を行う時、また、単に本を書きたいだけのときに他人が作ったテンプレートを流用できるのは嬉しいものです。
しかしながら、環境構築で躓くことも多く、可能であればできあがった環境をそのまま利用したいものですね。
Dockerのvvakame/reviewイメージはそういう点で、リポジトリとコレがあれば本ができる！CIで回せる！という意味で重宝されているのだと理解しています。

CSS組版の普及のためには最終的にdocker上でビルドし、PDFが得られるところまで行く必要があると考えています。

LaTeXはビルドが完了すれば、再現性があり問題のないPDFが得られるかわりに、ビルドエラーになった時にエラーの内容がわかりにくく血反吐を吐くことになります。

一方、CSS組版は基盤技術がWebページなだけあり、ぶっ壊れたデータを突っ込んでもなにがしかの結果を得ることができます。
逆にいうと、結果が壊れている（＝リンクが切れていたり図版が無かったり）しても、何らかの出力が得られてしまいます。
これは、入稿データを作る時には細心の注意が必要であるということになり、少々不都合です。

この問題を解消するためには、プリプロセッサ（＝Re:VIEW）レベルでエラーをより多く検出すること、ブラウザ上でレンダリングした時に信用できるエラーレポートが得られること、それをコマンドライン上に反映できること、が必要だと考えられます。
これはなかなか道のりが長そうです…。

これを実現するための要素技術としては、"Headless Chrome PDF"とかでググると結構色々と出て来るので、取り組めば割りとイケそうな感じだと思います。
vivliostyle.jsを使ってHTMLをレンダリングしないといけないのですが、vivliostyle.js自体もAPIのドキュメントがほぼないので調査がめんどくさい感じです。
適当にpuppeteerとかでやればええんちゃいますのん…？

Vivliostyle Viewerを立ち上げて、Chromeでレンダリングさせて、それの完了を待って、印刷するためにブラウザが色々と処理して、データを得る！
これは手元でやってみるとわかりますが、結構時間のかかる処理です。
現在TechBoosterのこの本は無編集で150Pを超えていますが、Chromeでレンダリングを待つ（目次のレンダリング完了が目安）のに50秒、印刷機能を読み出してLoading previewの表示が終わるのに20秒かかります。
うーん、気が重い…。

=== 他の組版エンジンを検討する

vivliostyle.jsを使っているが、他に選択肢があるのでは？という疑問があります。
米O'Reilly社はAtlasというシステムを作っていて、HTML+CSSでepubやPDFを生成できます。
これは裏でアンテナハウス社のAH Formatter@<fn>{antennahouse}を使っています。

//footnote[antennahouse][vivliostyle（現トリムマーク）の村上さんは2014年までアンテナハウス社にいました @<href>{http://blog.antenna.co.jp/CSSPage2/archives/165}]

他にも、Prince@<fn>{prince}などもあるようです。

//footnote[prince][@<href>{https://www.princexml.com/}]

が、全体的に商用ソフトウェアなんですよね。
ソフトウェアに金を落とさないエンジニアだ…と思うとちょっと自分のことを嫌な奴だなと思って凹みます。
まぁ同人誌制作の場合、利益を期待していないので制作費にお金を使わずに済ませたい…という事情があるわけです。
その費用が1万円とかそういうオーダーならば、割り切って払うという選択肢もあるのですが…。

===[column] トリムマーク社が爆誕した話

vivliostyle.jsを開発していたのは今までVivliostyle社でした。
Vivliostyle社はアンテナハウス社でXML組版やCSS組版エンジンの開発者としてブイブイいわせていた村上さんが、アンテナハウス社からの出資のもと設立された会社でした@<fn>{antennahouse-and-vivliostyle}。

//footnote[antennahouse-and-vivliostyle][@<href>{http://blog.antenna.co.jp/CSSPage2/archives/165}]

今回、この記事を書いている最中にVivliostyle社に変化がありました。
メインビジネスの変化に伴い、Vivliostyleという名前はオープンソースソフトウェアの名称として残り、会社自体はトリムマーク社に変更@<fn>{vivliostyle-and-trimmark}されました。

//footnote[vivliostyle-and-trimmark][@<href>{https://vivliostyle.org/ja/blog/2018/03/26/a-new-beginning/}]

これに伴い、vivliostyle.jsなどのOSSプロダクトは@<href>{https://vivliostyle.org/}に移行されました。

新しい組織に移行するに伴い、ユーザとしては変化に対して一抹の不安もあります。
旧Vivliostyle社の方々はCSS組版の分野について一流ですので、今後も変わらず辣腕を奮っていただけると大変うれしいですね。

===[/column]

== まとめ

ここまでで、筆者的には割りとそれっぽいPDFが得られるようになりました。
後は細部を詰めるのと、識者に赤を入れてもらい泣きながら修正する、というのが次の一手でしょうか。
フォント埋込…ｳｯ頭が！

この技法の研鑽が積まれ、紙、電子、Webという3つの出力結果が高品位になっていくと嬉しいです。
現状、電子といってもPDF以外の出力をTechBoosterやひかる黄金わかめ帝国では出していません。
リフロー可能な形式であるepubについては、見た目を整えるコストは高く、epubを求める需要は大変低いため、ないがしろにされています。
しかし、1つの出力先を改善すると全体が改善されるとなると、多少のコストを払う覚悟ができるでしょう。

また、我々は同人誌の組版をしたい、というのもCSS組版に対して有利に働きます。
プロユースの組版というのはナレッジの塊で、Re:VIEW開発者会議などで話を聞く心底関心してしまいます。
プロは細部のこだわりが凄まじく、なおかつそれを捨てられない場合が多くあるようです。
つまり、プロが既存技法を捨てCSS組版に移行する場合、大変な労力が要求されるのです。
一方、我々は素人で同人ですので、"それっぽさ"がある程度高まれば満足することができますし、読者もそれで満足できるでしょう。
プロの組版と素人の組版の差は、筆者は"ABテストをしたらプロが勝つが、素人の組版に明確な不備を指摘できる素人はいない"という感じだと筆者は思っています。

CSS組版自体はまだまだブラウザのサポートが十分ではないのが現状です。
つまり、筆者が意図したルックアンドフィールを少ない労力で得られる環境では（まだ）ない、ということです。
このあたりは、Dockerイメージの提供や本の素体を提供するなどで、CSS組版の高速道路が整備されればある程度無視することができます。

そして、ブラウザ側の思惑です。
ブラウザはインターネット上のWebページを閲覧するのが主目的です。
別に紙面をレンダリングしたりPDFを生成したりする組版エンジンになりたいわけではない、というのが大多数のコミッタの思いなのではないでしょうか。
つまり、人的資源が少ないため、Web関連技術にしては仕様の策定や実装の進み方がかなりゆっくりであると感じます。
ブラウザベンダのコミッタに話を聞いたわけではありませんので、実際にPaged Media周りの実装に絡んでいる人の話を聞いてみたいところです。

CSS組版が手法として確立したら、みんなが簡単にこのワークフローを使えるよう各所にコントリビュートしていきたいところですね。
まずは今回作ったCSSの完成度をあげていく必要があります。
改善すべき箇所や、取り込むべき技術があれば、ぜひGitHubのリポジトリまでIssueやPull Requestを送ってきてください。
次の技術書典でCSS組版で作られた本を増やすのは君だ！

//comment{
TODO 残タスク的な
 * 先達の確認と真似っこ
 * 既存の何かの本の組版を参考にする
 * ResetCSS系の何かっているんか？？
 * HTMLBuilderを改造してページ内リンクを修正する
 ** クリックするとdareka.htmlへのリンクになってたりする
 * https://www.npmjs.com/package/vivliostyle
 ** ライセンス問題
 ** 毎回AGPLってどこまでアレなのかとか理解できない話
 ** gccでコンパイルしたソフトウェアのこととかあんまし気にしないしさほど気にするユースケースじゃない気もする
//}

//comment{
参考資料的な
 * http://vivliostyle.github.io/vivliostyle.js/samples/css-secrets-long/doc/index.xhtml
 * ノンブル
 ** http://libroworks.co.jp/?p=646 ノンブルでページ内検索
 * https://twitter.com/vvakame/status/976395216872652801
 * https://www.antenna.co.jp/AHF/ahf5/CSSInfo/CSS-Page-Tutorial.pdf
 * 脚注の採番とかの良さそうなサンプル
 ** https://github.com/vivliostyle/vivliostyle.js/issues/406
 * https://libroworks.co.jp/?p=838
//}
