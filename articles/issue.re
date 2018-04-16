= 未解決の課題

はい。そして未解決の課題がいっぱい残っています。
これは読者への課題とします（PR待ってます）！

英語、技術書典運営のタスク、原稿などに挟まれてぎうぎうになっているため調査する時間とMPが足りなくなりました。
よって、ここに筆者の考える残タスクを開陳するものであります。
押忍。

== 問題の上手な切り分け方

ある仕様が未サポートだったり上手く動かない場合、それはChromeの振る舞いなのか、それともvivliostyle.jsか、切り分けが難しいです。
vivliostyle.jsはブラウザ未サポートのCSSをJSで頑張ってDOMのstyle属性に突っ込んでくれるため、DOM自体の可読性も恐ろしく悪くなります。

誰かナレッジがあったら分けてください…。

== PDFの生成とフォントの埋込

印刷所に入稿するデータはPDF形式であり、そこにはフォントデータを埋め込まなければなりません@<fn>{why-must-font-embed}。
しかし、現時点ではChromeの印刷機能でPDFを生成する時に、フォントデータを埋め込む方法が分かっていません。

//footnote[why-must-font-embed][印刷に使うフォントと同一のものを印刷所が持っているかわからないし同一性の検証もめんどくさいため]

pentapodさんの本によると、Adobe Acrobatに課金してフォントデータを後乗せで埋め込むという手法が選択肢としてあるそうです。
羊にこの手法を試してもらいましたが、上手くいきませんでした@<fn>{i-cant-understand-it}。

//footnote[i-cant-understand-it][羊が何か理由を説明してたけど筆者が理解できなかったパターン。なんて？？]

よって、今回の印刷ではフォントデータが欠損していても無視して無理やり印刷します。
現状の到達点だから…！仕方ないから…！

kmutoさんにTwitterで教えていただいた@<fn>{font-judge}ところによると、印刷所的にはフォントの埋込はCID、Type1はOK、Type3はアウト、という感じらしいです。
知らない用語が多かったので調べたところ、モリサワのフォント用語集@<fn>{morisawa-dictionary}を見るのがよさそうでした。

//footnote[font-judge][@<href>{https://twitter.com/kmuto/statuses/985670249289629696}]
//footnote[morisawa-dictionary][@<href>{http://www.morisawa.co.jp/culture/dictionary/}]

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

== 凝ったデザインへの挑戦

やりたかったやつー！
ページ番号周りでわかめこちゃんがキャッキャしたりして余白が楽しそうなやつー！

やりたかったやつー！
小口のところまでスミ乗っけておいて辞書の検索用索引みたいなの出すやつー！

まぁ多分頑張ればできそうだけどCSS力が圧倒的に足りない。
EPUB Adaptive Layoutの仕様とかを使えばできるものもあるそうですが、標準ではないものはあまり使いたくないし…。

== HTMLBuilderが吐き出すアンカーとリンクを修正する

Re:VIEWのHTMLBuilderが吐き出すHTMLは1ファイル==1章形式であり、すべてのファイルをconcatする前提ではありません。
なので、単純にconcatしただけだと章間のリンクが壊れたり、同名のfootnoteが複数章にまたがって存在すると不都合が生じます。

これを直すためには、review-ext.rbなどを経由してHTMLBuilderの挙動に手を入れる必要があります。
しかし、ここに手をいれるのはちょっとまとまった時間が必要そうなので、今回はいったん保留とします。

== CSSの管理方法

CSSをよりよく上手に管理する方法が必要です。
今のところ生CSSを書いていて、たとえば"リスト表示に対するパラメータ設定"みたいなのがどこで意図されて設定されているのか判然としません。
これを解決するために、mixin的な機能などをもったAltCSS（SCSSとか）を導入したほうが健康によさそうだと考えています。
というか、生CSSだとコメントを書くのがめんどくさすぎて憤死しそうです。
CSSこんなめんどくさかったっけ？

最終的に、Re:VIEWユーザがCSS組版のために使える共有財産として設定を育てたいと考えた場合、どこをいじるとどこが変わるのか、パッと見て分かる必要があるでしょう。
自分で書いたCSSが何を意図しているか忘れないうちに…早く…！

== ボックス系の見た目と脚注が被る話

リストなどのボックス系の見た目のものと、脚注が被ると見た目がおかしくなる場合があります（@<img>{bad-list-and-footnote}）。

//image[bad-list-and-footnote][ページまたぎのリストと脚注の相性の悪さ…][scale=0.7]{
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

== vivliostyle.jsのレンダリングがリロードで壊れる場合がある

図を見たほうがわかりやすいんですが、@<img>{bad-reset-hashbang}のようになります。
ボックスの改ページと脚注が被ったりします。

//image[bad-reset-hashbang][レンダリングが被る場合がある][scale=0.7]{
//}

#@# prh:disable
これは、ページ途中でリロードした時に発生しがちな現象です。
Vivliostyle Viewer、Chrome拡張共に発生します。
これは解決可能で、vivliostyle.jsが生成するURLのhashbang部分を削って、本の先頭からレンダリングさせると改善します。
Vivliostyle Viewerを使っている時のURL末尾に付く@<code>{&f=epubcfi(/2!/4/12[chapter-vvakame]/139:0)}のような部分ですね。
おそらく、vivliostyle.jsのレンダリングが、本の先頭からのときと本の途中からのときで挙動が違うのだと考えられます。

この現象を完全に回避する方法は今のところ筆者は発見できていないので、PDF生成後に全部のページレイアウトを目で毎回確認する必要があります。

== CI上でPDFを生成させ再現性を持たせる

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

== 他のブラウザを検討する

今回はGoogle Chromeのみに絞って調査・チューニングをしました。
代わりに、Mozilla Firefoxなどの別のブラウザを使うとどうなるでしょう…。

kmutoさんに教えていただいた@<fn>{ff-and-font}ところによると、PDFへのフォント埋め込み事情はFirefoxのほうが優れているそうです。
テキトーに試してみたところ、Firefoxが生成したPDFはフォントの埋込はバッチリで、紙面のレンダリングはさんざん…という結果でした。

//footnote[ff-and-font][@<href>{https://twitter.com/kmuto/statuses/985672642047819776}]

== 他の組版エンジンを検討する

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
