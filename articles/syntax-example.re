= Re:VIEW記法出力見本

この節はPDFで出した時の出力例です。
さまざまなRe:VIEW的表現を書くだけ書いておいて、LaTeX版とCSS版でPDFにどのような違いが出るかを確認します。

== 出力見本

#@# なるべく https://github.com/kmuto/review/blob/master/doc/format.ja.md の登場順にしてある 

 * 箇条書き
 ** 箇条書きです

 1. 番号付きです
 1. 番号付きです

: 用語リスト1
  用語の意味です。
: 用語リスト2
  用語の意味です。

リストと参照です（@<list>{sample-list}、@<list>{sample-listnum}）。

//list[sample-list][リストです]{
リストの中身です。
//}

//listnum[sample-listnum][行数付きリストです]{
行数付きリストの中身です。
//}

//emlist[連番無しリストです]{
連番無しリストの中身です。
//}

//emlistnum[連番無し行数付きリストです]{
連番無し行数付きリストです。
//}

//comment{
sourceブロック記法がPDFで出力できなかった
//}

ソースコードを@<code>{console.log("Test");}文中に。

//cmd{
$ docker run -t --rm -v $(pwd):/book vvakame/review:2.4 \
  /bin/bash -ci "cd /book && ./setup.sh && npm run pdf"
//}

図とか（@<img>{sample-image1}）。

//image[sample-image1][適当な画像][scale=0.3]{
//}

色々ありますね。

//indepimage[sample-image2][採番無しの画像1][scale=0.3]

//comment{
紙面幅節約のため割愛 indepimageとあんまし変わらない
//numberlessimage[sample-numberlessimage][採番無しの画像2][scale=0.3]
//}

//graph[sample-graph][graphviz][グラフ]{
strict graph {
  a -- b
  b -- a
}
//}

テーブル（@<table>{sample-table}）。

//table[sample-table][テーブル]{
語句	意味
-------------------------------------------------------------
よさそう	肯定的な返事 生返事の場合も多い
すごそう	期待のもてる情報に対する返事
つよそう	勝てそうだったり便利そうだったりする情報に対する返事
よわそう	負けそうだったら既存のものよりよくない情報に対する返事
//}

//comment{
  なんかcaptionが崩れるのでみなかったことにする たぶんRe:VIEW本体の変更とこっちが使ってるテンプレが乖離している
  //emtable[テーブル]{
  語句	意味
  -------------------------------------------------------------
  よさそう	肯定的な返事 生返事の場合も多い
  すごそう	期待のもてる情報に対する返事
  //}
//}

//quote{
引用です。
//}

脚注です@<fn>{sample-footnote}。

//footnote[sample-footnote][ここが下に表示されています]

//lead{
リードです。
//}

//comment{
chap, title, chapref のテスト
//}

#@# prh:disable
リンクの様子@<href>{https://github.com/vvakame/review-css-typesetting}こんな感じ。

@<kw>{CSS組版,CSS typesetting}。
@<strong>{強調}。
@<tt>{等幅}。
@<tti>{等幅イタリック}。
@<ttb>{等幅ボールド}。
@<code>{コード}。
//comment{
LaTeXでルビをサポートしていない https://github.com/kmuto/review/issues/655
@<ruby>{振り仮名,ルビ}。
//}

===[column] コラム

コラムだよ〜〜〜〜。

===[/column]

== 節です

節ですね。

=== 項です

項ですね。
