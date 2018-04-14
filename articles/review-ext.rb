# encoding: utf-8

module ReVIEW
    class HTMLBuilder
        # # footnoteの採番をブラウザでやらせようとして挫折
        # # vivliostyle.jsがIDを書き換えてしまい、参照できなくなってしまう
        # # target-counter を使ったやりかた
        # def footnote(id, str)
        #     puts %Q(<p class="footnote" id="fn-#{normalize_id(id)}"><span>#{compile_inline(str)}</span></p>)
        # end
        # def inline_fn(id)
        #     # 番号はCSSで出すので気にしない
        #     %Q(<a id="fnb-#{normalize_id(id)}" href="#fn-#{normalize_id(id)}" class="noteref"><!--#{@chapter.footnote(id).number}--></a>)
        # end
    end
end
