# Re:VIEW+CSS組版やっていき [![CircleCI](https://circleci.com/gh/vvakame/review-css-typesetting.svg?style=svg)](https://circleci.com/gh/vvakame/review-css-typesetting)

このリポジトリは技術書典4にて[ひかる黄金わかめ帝国](https://techbookfest.org/event/tbf04/circle/11680001)が頒布した『Re:VIEW+CSS組版やっていき』の全てです。
[Re:VIEW](https://github.com/kmuto/review/)と[Vivliostyle](https://vivliostyle.org/)を使って、同人誌をこしらえます。

## この本のビルドの仕方

### LaTeX版

Dockerのセットアップが必要です。

```
$ ./build-in-docker.sh
$ open ./articles/review-css-typesetting.pdf
```

### CSS版

頒布時は Vivliostyle 2017.6 + Google Chrome Version 65.0.3325.181 を利用しました。

https://vivliostyle.org/download/ からVivliostyle.jsをダウンロード。

```
# Vivliostyleを解凍したディレクトリで
$ ./start-webserver
```
```
# このリポジトリのディレクトリで
$ ./setup.sh
$ npm run css
$ npm run serve
$ open http://127.0.0.1:8000/viewer/vivliostyle-viewer.html#x=http://127.0.0.1:8989/book.html
```

あとはChromeの印刷機能で出力します。
MarginsはNone、Background graphicsのチェックはONでPDFとして保存します。

## ライセンス

本リポジトリは画像以外はMITライセンス、画像はライセンスなしとします。
画像を別用途に勝手に使ったりしなければ、forkなどについては難しく考えなくて大丈夫です。

## このリポジトリの目標

1. LaTeX版の出力に近づける
2. LaTeX版同様のワークフローを実現する
3. CSS組版のナレッジを集積する

### コントリビュートについて

上記目標に沿う内容であれば随時受け付けています。
また、Issue化されていなくても書籍中で言及されている課題についての改善は大歓迎です。
リポジトリ内をTODOで検索して、これを消化してくれるのも助かります。
