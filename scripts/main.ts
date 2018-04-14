import fs from "fs";
import path from "path";

import yaml from "js-yaml";
import { JSDOM } from "jsdom";

interface Catalog {
    PREDEF?: string[];
    CHAPS: string[];
    APPENDIX?: string[];
    POSTDEF?: string[];
}

interface Config {
    booktitle: string;
    aut: string[];
    edt: string[];
    history: string[][];
    date?: string;
    pbl?: string;
    rights?: string;
}

interface TOC {
    chapters: {
        id: string;
        title: string;
        headers: {
            isColumn: boolean;
            text: string;
        }[];
    }[];
}

const articleDir = path.join(__dirname, "../articles/");
const catalogPath = path.join(articleDir, "catalog.yml");
const configPath = path.join(articleDir, "config.yml");
const tocHtmlPath = path.join(articleDir, "toc.html");
const bookHtmlPath = path.join(articleDir, "book.html");

const catalog: Catalog = yaml.load(fs.readFileSync(catalogPath, { encoding: "utf8" })) as any;
const config: Config = yaml.load(fs.readFileSync(configPath, { encoding: "utf8" })) as any;

let result = "";

[
    catalog.PREDEF || [],
    catalog.CHAPS,
    catalog.APPENDIX || [],
    catalog.POSTDEF || [],
].forEach(chapFiles => {
    console.log(chapFiles);
    chapFiles.forEach(chapFile => {
        const chapPath = path.join(__dirname, "../articles/", chapFile);
        const htmlPath = chapPath.replace(/\.re$/g, ".html");
        const content = fs.readFileSync(htmlPath, { encoding: "utf8" });
        // TODO このIDは正式なIDではないけどめんどいので
        result += `<section id="chapter-${chapFile.replace(/\.re$/g, "")}">\n${content}\n</section>\n\n`;
        console.log(chapPath);
    })
});

const toc = generateTOC(getTOCdata(), catalog);

const generatedHtml = `
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>${config.booktitle}</title>
    <link rel="stylesheet" href="./book.css">
</head>
<body>

<aside>
    <h1>${config.booktitle}</h1>
    <div id="author">${config.aut.join(", ")} 著</div>
    <div id="date">${config.date} 版</div>
    <div id="pbl">${config.pbl} 発行</div>
</aside>

${toc}

${result}

<aside>
    <div>${config.booktitle}</div>
    <div>${config.history.map(s => s.join("<br>")).join("<br>")}</div>
    <div>著者 ${config.aut.join(", ")}</div>
    <div>編集 ${config.edt.join(", ")}</div>
    <div>発行所 ${config.pbl}</div>
    <div>${config.rights}</div>
</aside>

</body>
</html>
`;

fs.writeFileSync(bookHtmlPath, generatedHtml);
console.log("output", bookHtmlPath);

function getTOCdata(): TOC {
    const dom = new JSDOM(fs.readFileSync(tocHtmlPath, { encoding: "utf8" }));
    const { window: { document } } = dom;

    const toc: TOC = {
        chapters: [],
    };

    const el = document.querySelector(".book-toc") as HTMLElement;
    for (let i = 0; i < el.childElementCount; i++) {
        const chap = el.children[i];
        const children = el.children[i + 1];
        if (!isLiTag(chap)) {
            throw new Error(`unexpected tag: ${chap.tagName} ${chap.innerHTML}`);
        }
        if (isOlTag(children)) {
            // 内容が空の章があると死ぬので存在確認していく
            i++;
        }
        const chapTitle = chap.querySelector("a") as HTMLAnchorElement;
        const idMatched = chapTitle.name.match(/^\.\/(.*)\.html$/);
        const titleMatched = (chapTitle.textContent || "").match(/^[0-9]*\s*(.*)$/);

        const titleList = Array.from(children.querySelectorAll("li"));

        toc.chapters.push({
            id: idMatched![1],
            title: titleMatched![1],
            headers: titleList.map(el => {
                const title = el.textContent || "";
                return {
                    isColumn: title.startsWith("[column] "),
                    text: title,
                };
            }),
        });
    }

    return toc;

    function isLiTag(el: Element | null): el is HTMLLIElement {
        return !!el && el.tagName === "LI";
    }
    function isOlTag(el: Element | null): el is HTMLLIElement {
        return !!el && el.tagName === "OL";
    }
}

function generateTOC(toc: TOC, catalog: Catalog) {
    const dom = new JSDOM();
    const { window: { document } } = dom;

    const nav = document.createElement("nav");
    nav.className = "toc";

    const h2 = document.createElement("h2");
    h2.textContent = "目次";
    nav.appendChild(h2);

    const root = document.createElement("ul");
    root.className = "book-toc";
    nav.appendChild(root);

    toc.chapters.forEach((chap, chapIdx) => {
        const li = document.createElement("li");
        root.appendChild(li);

        const a = document.createElement("a");
        li.appendChild(a);
        a.href = `#chapter-${chap.id}`;
        a.textContent = chap.title;

        if (chap.headers.length === 0) {
            return;
        }

        const inCHAPS = catalog.CHAPS
            .map(chapPath => chapPath.replace(/\.re$/g, ""))
            .includes(chap.id)

        if (!inCHAPS) {
            return;
        }

        const ol = document.createElement("ol");
        root.appendChild(ol);

        let headerCount = 0;
        chap.headers.forEach(header => {
            if (header.isColumn) {
                return;
            }
            headerCount++;

            const li = document.createElement("li");
            ol.appendChild(li);

            const a = document.createElement("a");
            li.appendChild(a);
            a.href = `#h${chapIdx}-${headerCount}`;
            a.textContent = header.text;
        });
    });

    return nav.outerHTML;
}
