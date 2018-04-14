= はじめに

本書を手に取っていただき、ありがとうございます。

TechBoosterの新刊にこの本も収録される予定だったのですが、羊が爆発四散したため個別の本になりました。
そんなわけで現在4月16日深夜2時、まだ表紙すらない段階から本をでっちあげていきます。
オラ体の震えが止まらねぇぞ…！
落ち着け本の内容はすでに30ページ以上あるんだ…きっと体裁を整えるだけで勝てるに決まってる…。
まずは書名を決めるところからやっていかないといけません。
何にしよう。

さて、この本は嬉し恥ずかしCSS組版の話です。
残念ながら、CSS組版で本を作りきった話にはなりませんでした。

現在、TechBoosterではRe:VIEW@<fn>{review}+LaTeX@<fn>{latex}でPDFを生成し入稿するという書籍制作フローです。
これは非常に安定していて、Dockerのvvakame/reviewイメージ@<fn>{docker-review}を使い、Circle CIなどで継続的インテグレーションを行うワークフローが確立されています。
つまり、我々エンジニアが普段行っている開発のサイクルとまったく同じやりかたで執筆を行えるのです。
グッドですね。

//footnote[review][@<href>{https://github.com/kmuto/review/}]
//footnote[latex][@<href>{https://www.latex-project.org/}]
//footnote[docker-review][@<href>{https://hub.docker.com/r/vvakame/review/} Docker HubとGitHub両方にスターお願いします！]

しかしながら、LaTeXを用いた組版は現代っ子なエンジニアが簡単に使いこなせる技術ではありません。
本を作るのに新しく古い技術を覚えるのもめんどくさいので、Web開発に近いやり方で本が作れたらいいのにな、という願望からこの本は生まれました。

この本の内容は@<href>{https://github.com/vvakame/review-css-typesetting}にて公開されます。
本の出来栄えを見て、俺でも直せらぁ！と思ったらぜひPull Requestを送ってください。
