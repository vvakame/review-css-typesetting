"use strict";

let fs = require("fs");
let yaml = require("js-yaml");

const articles = "articles";
const bookConfig = yaml.safeLoad(fs.readFileSync(`${articles}/config.yml`, "utf8"));

const reviewPrefix = process.env["REVIEW_PREFIX"] || "bundle exec ";
const reviewPostfix = process.env["REVIEW_POSTFIX"] || "";             // REVIEW_POSTFIX="-peg" npm run pdf とかするとPEGでビルドできるよ
const reviewPreproc = `${reviewPrefix}review-preproc${reviewPostfix}`;
const reviewCompile = `${reviewPrefix}review-compile${reviewPostfix}`;
const reviewPdfMaker = `${reviewPrefix}review-pdfmaker${reviewPostfix}`;
const reviewEpubMaker = `${reviewPrefix}review-epubmaker${reviewPostfix}`;

module.exports = grunt => {
  grunt.initConfig({
    clean: {
      review: {
        src: [
          `${articles}/${bookConfig.bookname}-*/`, // pdf, epub temp dir
          `${articles}/*.pdf`,
          `${articles}/*.epub`,
          `${articles}/*.html`,
          `${articles}/*.md`,
          `${articles}/*.xml`,
          `${articles}/*.txt`
        ]
      }
    },
    shell: {
      preprocess: {
        options: {
          execOptions: {
            cwd: articles,
          }
        },
        command: `${reviewPreproc} -r --tabwidth=2 *.re`
      },
      compile2text: {
        options: {
          execOptions: {
            cwd: articles,
          }
        },
        command: `${reviewCompile} --target=text`
      },
      compile2markdown: {
        options: {
          execOptions: {
            cwd: articles,
          }
        },
        command: `${reviewCompile} --target=markdown`
      },
      compile2html: {
        options: {
          execOptions: {
            cwd: articles,
          }
        },
        command: `${reviewCompile} --target=html --stylesheet=style.css --chapterlink`
      },
      compile2latex: {
        options: {
          execOptions: {
            cwd: articles,
          }
        },
        command: `${reviewCompile} --target=latex --footnotetext`
      },
      compile2idgxml: {
        options: {
          execOptions: {
            cwd: articles,
          }
        },
        command: `${reviewCompile} --target=idgxml`
      },
      compile2pdf: {
        options: {
          execOptions: {
            cwd: articles,
          }
        },
        command: `${reviewPdfMaker} config.yml`
      },
      compile2epub: {
        options: {
          execOptions: {
            cwd: articles,
          }
        },
        command: `${reviewEpubMaker} config.yml`
      },
    }
  });

  function generateTask(target) {
    return ["clean", "shell:preprocess", `shell:compile2${target}`];
  }

  grunt.registerTask(
    "default",
    "原稿をコンパイルしてPDFファイルにする",
    "pdf");

  grunt.registerTask(
    "text",
    "原稿をコンパイルしてTextファイルにする",
    generateTask("text"));

  grunt.registerTask(
    "markdown",
    "原稿をコンパイルしてMarkdownファイルにする",
    generateTask("markdown"));

  grunt.registerTask(
    "html",
    "原稿をコンパイルしてHTMLファイルにする",
    generateTask("html"));

  grunt.registerTask(
    "idgxml",
    "原稿をコンパイルしてInDesign用XMLファイルにする",
    generateTask("idgxml"));

  grunt.registerTask(
    "pdf",
    "原稿をコンパイルしてpdfファイルにする",
    generateTask("pdf"));

  grunt.registerTask(
    "epub",
    "原稿をコンパイルしてepubファイルにする",
    generateTask("epub"));

  // ==================================
  // helper
  // ==================================
  function execCommandForegroundPromise(command) {
    return new Promise(function (resolve, reject) {
      const childProcess = require('child_process');
      childProcess.exec(command, (error, stdout, stderr) => {
        if (error) {
          console.log(stderr);
          process.exit(1);
        } else {
          console.log(stdout);
        }
      });
    });
  }

  function execCommandBackgroundPromise(command) {
    return new Promise(function (resolve, reject) {
      const spawn = require('child_process').spawn;
      const child = spawn(command, [], {
        detached: true,
        stdio: 'ignore'
      });
      child.unref();
      resolve('commnad exec.');
    });
  }

  function isServed(url) {
    var request = require('request');
    return new Promise(function (resolve, reject) {
      setTimeout(() => {
        request(url, function (error, res, body) {
          if (!error && res.statusCode == 200) {
            resolve(true);
          } else {
            resolve(false);
          }
        });
      }, 1000);
    });
  }

  async function wait(url, times) {
    for (let i = 0; i < times; i += 1) {
      const result = await isServed(url);
      if (result == true) {
        console.log(url, 'is served.');
        return;
      }
      console.log(url, 'is not served.');
    }
    console.log('Time out.');
  }

  async function startServer(command, url, times) {
    var isExeced = false;
    for (let i = 0; i < times; i += 1) {
      const result = await isServed(url);
      if (result == true) {
        console.log(url, 'is served.');
        return;
      }
      console.log(url, 'is not served.');
      if (!isExeced) {
        console.log('Start web server: ', url);
        var stdout = await execCommandBackground(command);
        isExeced = true;
      }
    }
    console.log('Time out.');
    process.exit(1);
  }

  async function execCommandForeground(command) {
    var stdout = await execCommandForegroundPromise(command);
  }

  async function execCommandBackground(command) {
    var stdout = await execCommandBackgroundPromise(command);
  }

  // ==================================
  // vivliostyle
  // ==================================
  grunt.registerTask("vivliostyle/setup", "vivlipstyle環境のセットアップ", function () {
    var done = this.async();
    execCommandForeground('././docker_env/review/vivliostyle_setup.sh');
  });

  grunt.registerTask("vivliostyle/up", "vivliostyleサーバーを起動", function () {
    var done = this.async();
    startServer(
      './docker_env/review/vivliostyle_server_up.sh',
      'http://127.0.0.1:8000/docker_env/review/vivliostyle/vivliostyle-js-2017.6/viewer/vivliostyle-viewer.html',
      30
    );
  });

  grunt.registerTask("vivliostyle/status", "vivliostyleサーバーのステータスチェック", function () {
    var done = this.async();
    const func = async () => {
      var res = await isServed('http://127.0.0.1:8000/docker_env/review/vivliostyle/vivliostyle-js-2017.6/viewer/vivliostyle-viewer.html', done)
      if (res) {
        console.log('served!!');
      } else {
        console.log('unserved!!');
      }
    };
    func();
  });

  grunt.registerTask("vivliostyle/kill", "vivliostyleサーバーの強制停止", function () {
    var done = this.async();
    console.log('Killing vivliostyle server ...');
    execCommandForeground('./docker_env/review/vivliostyle_server_kill.sh');
    console.log('Killing vivliostyle server ... done');
  });

  grunt.registerTask("vivliostyle/wait", "vivliostyleサーバーの待機", function () {
    var done = this.async();
    wait('http://127.0.0.1:8000/docker_env/review/vivliostyle/vivliostyle-js-2017.6/viewer/vivliostyle-viewer.html', 30);
  });

  // ==================================
  // review
  // ==================================
  grunt.registerTask("review/setup", "review環境のセットアップ", function () {
    var done = this.async();
    execCommandForeground('./setup.sh');
  });

  grunt.registerTask("review/up", "reviewサーバーを起動", function () {
    var done = this.async();
    startServer(
      './docker_env/review/server.sh',
      'http://127.0.0.1:8989/book.html',
      30
    );
  });

  grunt.registerTask("review/status", "reviewサーバーのステータスチェック", function () {
    var done = this.async();
    const func = async () => {
      var res = await isServed('http://127.0.0.1:8989/book.html', done)
      if (res) {
        console.log('served!!');
      } else {
        console.log('unserved!!');
      }
    };
    func();
  });

  grunt.registerTask("review/kill", "reviewサーバーの強制停止", function () {
    var done = this.async();
    console.log('Killing review server ...');
    execCommandForeground('./docker_env/review/server_kill.sh');
    console.log('Killing review server ... done');
  });

  grunt.registerTask("review/wait", "reviewサーバーの待機", function () {
    var done = this.async();
    wait('http://127.0.0.1:8989/book.html', 30);
  });

  require('load-grunt-tasks')(grunt);
};
