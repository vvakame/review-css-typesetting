# Re:VIEW+CSS組版やっていき [![CircleCI](https://circleci.com/gh/vvakame/review-css-typesetting.svg?style=svg)](https://circleci.com/gh/vvakame/review-css-typesetting)

このリポジトリは技術書典4にて[ひかる黄金わかめ帝国](https://techbookfest.org/event/tbf04/circle/11680001)が頒布した『Re:VIEW+CSS組版やっていき』の全てです。
[Re:VIEW](https://github.com/kmuto/review/)と[Vivliostyle](https://vivliostyle.org/)を使って、同人誌をこしらえます。

## この本のビルドの仕方

### 環境のセットアップ

必要なものは `docker` と `make` です。

```
# このリポジトリのディレクトリで
$ make setup
```

### LaTeX版PDF

```
# `make setup` のあとで
$ make npm/run/pdf
$ open ./articles/review-css-typesetting.pdf
```

### CSS版PDF

```
# `make setup` のあとで
$ make css/pdf

# PDF=<path> で出力先を変更できます（デフォルト：./articles/book.pdf）
# PAGE_FORMAT=<format> でフォーマットを変更できます（デフォルト：A5）
$ make css/pdf PDF=./articles/my_book.pdf PAGE_FORMAT=A4
```

* 処理の最後に `open {出力先のpath}` を叩いているので、不要だったら消してください
* `PAGE_FORMAT` は [puppeteer/docs/api.md#pagepdfoptions](https://github.com/GoogleChrome/puppeteer/blob/master/docs/api.md#pagepdfoptions) を参考にしてください

### CSS版ブラウザ確認

```
# `make setup` のあとで
$ make css/browser
```

* 処理の最後に `open {URL}` を叩いているので、OSX環境だとデフォルトブラウザで即座に確認できます
  * 不要だったら消してください

## CSS版でCSSをいじりたい

### 編集するCSSファイル

* 編集するCSSファイルは `articles/book.css` です
* PDF、ブラウザ確認、共に 上記cssを `articles/book.html` に適用して出力します
  * DOMの構造などは `articles/book.html` を見てください

### book.html に独自のHTMLタグを追加したい

* `articles/review-ext.rb` が使えます
* 使用方法などは以下を参考にしてください
  * [Re:VIEW ナレッジベース - review-ext.rb による拡張 -](https://review-knowledge-ja.readthedocs.io/ja/latest/index.html#review-ext-rb)
  * その他、`review-ext.rb`で検索すると情報が出てきます

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
