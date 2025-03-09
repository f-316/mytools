/** pre */
import path from 'path'
const NODE_EXE = process.argv[0];
const THIS_PATH = process.argv[1];
const THIS_FNAME = path.basename(THIS_PATH);
const THIS_DIR = path.dirname(THIS_PATH);
const WORKING_DIR = path.resolve('.');

/** import */
import fs from 'fs';
import { NatureRemo } from './lib/NatureRemo.mjs';
import { SwitchBot } from './lib/SwitchBot.mjs';
import { GoogleCloudAPI } from './lib/GoogleCloudAPI.mjs';
import { Stopwatch } from './lib/Stopwatch.mjs';
import { AppLog } from './lib/AppLog.mjs';
const appLog = new AppLog('', 'main');

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
  const logFile = './#dist/test.log';

  // トークン取得
  const nrJsonStr = fs.readFileSync('#token/nature-remo.json')
  const sbJsonStr = fs.readFileSync('#token/switch-bot.json')
  const nrJson = JSON.parse(nrJsonStr);
  const sbJson = JSON.parse(sbJsonStr);
  const CREDENTIALS_PATH = '#token/gc_credentials.json';
  const TOKEN_PATH = '#token/gc_token.json';

  const natureRemo = new NatureRemo(nrJson.token);
  const switchBot = new SwitchBot(sbJson.token, sbJson.secret);
  const gcApi = new GoogleCloudAPI(CREDENTIALS_PATH, TOKEN_PATH);
  
  const loopAdjTimer = new Stopwatch(true);
  let loopAdjLapMs = 0
  const loopIntervalMs = 2000;
  const loggingMin = 5;
  let initExe = true;
  let exeLog = false;
  let exeUpload = true;
  let curHour = new Date().getHours();
  let lastHour = curHour;
  let curMin = -1;
  let lastMin = -1;
  appLog.trace(55, `App start!loopIntervalMs=${loopIntervalMs},loggingMin=${loggingMin}`);
  while (1) {
    const date = new Date();
    curMin = date.getMinutes();
    curHour = date.getHours();

    // ログ
    if (initExe || (!exeLog && curMin % loggingMin === 0)) {
      initExe = false;
      exeLog = true;
      lastMin = curMin;

      // 処理
      const dateStr = date.toLocaleString();
      const nrTemps = await natureRemo.getTemps();
      const sbTemps = await switchBot.getTemps();
      const line = `${dateStr},${sbTemps.te},${sbTemps.hu},${nrTemps.te},${nrTemps.hu}\n`
  
      // ToDo: Streamの方がいい
      fs.writeFileSync(logFile, line, {flag:'a'})
      // fs.writeFile(logFile, line, {flag:'a'}, (err) => {
      //   !err ?? console.error(err);
      //   // gcApi.saveFile(logFile, '1QE3YdDcQviPFzap0KU2dern-Bj-Ly0xs', 'sample.txt');
      // })
    } else if (lastMin !== curMin) {
      lastMin = curMin;
      exeLog = false;
    }

    // アップロード
    // if (!exeUpload) {
    //   exeUpload = true;
    //   await gcApi.updateFile(logFile, '1yl7zEA1_pKi35aC6OrmKuPKylSn4HhuB');
    //   appLog.trace(94, `Updated!`);
    //   log(94, `Updated!`);
    //   lastHour = curHour;
    // } else if (lastHour !== curHour) {
    //   lastHour = curHour;
    //   exeUpload = false;
    // }
      
    // 待ち時間調整
    loopAdjLapMs = loopAdjTimer.lapMs();
    if (loopAdjLapMs < loopIntervalMs) {
      await sleep(loopIntervalMs - loopAdjLapMs);
    }
    loopAdjTimer.lap();
  }

  return true;
}

// main
main(process.argv.slice(2), process.argv.slice(2).length)
