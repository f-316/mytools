/** pre */
import path from 'path'
const NODE_EXE = process.argv[0];
const THIS_PATH = process.argv[1];
const THIS_FNAME = path.basename(THIS_PATH);
const THIS_DIR = path.dirname(THIS_PATH);
const WORKING_DIR = path.resolve('.');

/** import */
import fs from 'fs';
import { AppLog } from './lib/AppLog.mjs';
const appLog = new AppLog(`${THIS_FNAME}.log`, 'main');

/** arg defaults */

async function sleep(ms) {
  await new Promise(resolve => setTimeout(() => resolve(), ms));
}

/**
 * main
 * @param { string[] } argv 
 * @param { number } argc 
 */
const main = async (argv, argc) => {
  appLog.trace(26, 'hello!');

  const filePath = '#test.txt';
  
  const stream = fs.createWriteStream(filePath, { flags: 'a' });
  // ストリームのクローズ判定 -> false
  console.log('Stream closed:', stream.closed);

  // ストリームのエラーハンドリング
  stream.on('error', (err) => {
    console.error('Error occurred:', err);
  });
  // endイベントをリッスン
  stream.on('finish', () => {
    console.log('Stream has ended (finish event).');
    // ストリームのクローズ判定 -> false endしたがcloseまでできていない。
    console.log('[finish]Stream closed:', stream.closed);
  });
  stream.on('close', () => {
    // ストリームのクローズ判定 -> true
    console.log('[close]Stream closed:', stream.closed);
  })

  stream.write('Hello ');
  stream.write('World!\n');
  
  await sleep(3000);
  stream.end(() => console.log('Stream writing completed!'));
  console.log('Stream closed:', stream.closed);

  const stat = fs.statSync(filePath)
  // console.log(stat);
  const birth = new Date(stat.birthtime);
  console.log(`${birth.toLocaleDateString()} ${birth.toLocaleTimeString()}`);

  // 指定バイト位置を読む場合
  // const fd = fs.createReadStream(filePath, { start: 1, end: 2 });
  // let data = '';
  // fd.on('data', chunk => {
  //   data+=chunk;
  //   console.log(chunk);
  // })
  // fd.on('end', () => {
  //   console.log(data);
  // })
  return true;
}

// main
main(process.argv.slice(2), process.argv.slice(2).length)
