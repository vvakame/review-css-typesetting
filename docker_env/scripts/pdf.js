const puppeteer = require('puppeteer');
var outputPath = process.argv[2];
var url = process.argv[3];
var pageFormat = process.argv[4];

if (process.argv.length != 5) {
  console.log('Please specified command options. `node pdf.js <output-path> <vivliostyle-url>`');
  process.exit(1);
}

(async () => {
  const browser = await puppeteer.launch({ args: ['--no-sandbox'] });
  const page = await browser.newPage();
  await page.goto(url, { waitUntil: 'networkidle0' });
  await page.pdf({
    path: outputPath,
    format: pageFormat,
    printBackground: true
  });

  await browser.close();

  console.log('output:', outputPath);

})();
