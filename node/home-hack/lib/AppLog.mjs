/** import */
import fs from 'fs';

export class AppLog {
  m_logPath = '#AppLog.log';
  m_logLinePrefix = '';
  constructor(logPath = '', logLinePrefix = '') {
    if (logPath) {
      this.m_logPath = logPath;
    }
    if (logLinePrefix) {
      this.m_logLinePrefix = logLinePrefix;
    }
  }

  /**
   * TRACEログを書き出します。
   * @param { number } line 
   * @param { string } text 
   */
  trace(line, text) {
    fs.writeFile(this.m_logPath, `${new Date().toLocaleString()} ${this.m_logLinePrefix}(${line}):[TRACE]${text}\n`, {flag:'a'}, (err) => {
      if (err) {
        console.error(`${new Date().toLocaleString()} ${err}`)
      }
    });
  }

  /**
   * ERRORログを書き出します。
   * @param { number } line 
   * @param { string } text 
   */
  error(line, text) {
    fs.writeFile(this.m_logPath, `${new Date().toLocaleString()} ${this.m_logLinePrefix}(${line}):[ERROR]${text}\n`, {flag:'a'}, (err) => {
      if (err) {
        console.error(`${new Date().toLocaleString()} ${err}`)
      }
    });
  }
}
