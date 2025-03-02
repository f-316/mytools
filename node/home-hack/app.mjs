/** pre */
import path, { resolve } from 'path'
const NODE_EXE = process.argv[0];
const THIS_PATH = process.argv[1];
const THIS_FNAME = path.basename(THIS_PATH);
const THIS_DIR = path.dirname(THIS_PATH);
const WORKING_DIR = path.resolve('.');

/** import */
import fs from 'fs';
// import { getDevices, getDeviceStatus, postDeviceCommand } from './lib/swtich-bot.mjs';
import { getDeviceInfo, getAppliances } from './lib/nature-remo.mjs';
import { NatureRemo } from './lib/NatureRemo.mjs';
import { SwitchBot } from './lib/SwitchBot.mjs';

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

  // トークン取得
  const nrJsonStr = fs.readFileSync('#token/nature-remo.json')
  const sbJsonStr = fs.readFileSync('#token/switch-bot.json')
  const nrJson = JSON.parse(nrJsonStr);
  const sbJson = JSON.parse(sbJsonStr);

  const natureRemo = new NatureRemo(nrJson.token);
  const switchBot = new SwitchBot(sbJson.token, sbJson.secret);

  let start = performance.now()
  let elapsedMs = 0
  const intervalMs = 5 * 60 * 1000;
  const logFile = './#dist/test.log';

  while (1) {
    // 処理
    const dateStr = new Date().toLocaleString();
    const nrTemps = await natureRemo.getTemps();
    const sbTemps = await switchBot.getTemps();
    const line = `${dateStr},${sbTemps.te},${sbTemps.hu},${nrTemps.te},${nrTemps.hu}\n`

    fs.writeFile(logFile, line, {flag:'a'}, err => !err ?? console.error(err))
    
    // 待ち
    elapsedMs = performance.now() - start;
    // console.log(elapsedMs);
    if (elapsedMs < intervalMs) {
        await sleep(intervalMs - elapsedMs);
    }
    start = performance.now()
  }

  return true;
}

// main
main(process.argv.slice(2), process.argv.slice(2).length)
