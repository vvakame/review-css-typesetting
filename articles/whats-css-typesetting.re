= CSS組版について知ろう！

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
