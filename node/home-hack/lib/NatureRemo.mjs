/** import */
import https from 'https';
import { AppLog } from './AppLog.mjs';

export class NatureRemo {
  m_token = ''
  m_log = new AppLog('', 'NR');
  constructor(token) {
    this.m_token = token;
  }

  /**
   * ヘッダを生成します。
   * @returns { Object } - header
   */
  _generateHeader() {
    return {
      'Authorization': `Bearer ${this.m_token}`,
      'Content-Type': 'application/json; charset=utf8',
    }
  }

  /**
   * https-request
   * @param { Object } options 
   * @param { String | undefined } chunk 
   * @returns { Promise<string> } - http-responce
   */
  async _httpsRequest(options, chunk = undefined) {
    // HTTPリクエストを送信
    const controller = new AbortController();
    const req = https.request(options, (res) => {
      let data = '';

      res.on('data', (_chunk) => {
        data += _chunk;
      });

      res.on('end', () => {
        controller.abort({result: true, data});
      });
    });

    req.on('error', (e) => {
      this.m_log.error(43, `Error: ${e.message}`);
      controller.abort({result: false, data: e.message});
    });
    if (chunk !== undefined) {
      req.write(chunk);
    }
    req.end();

    return new Promise(resolve => {
      controller.signal.onabort = () => {
        resolve(controller.signal.reason);
      }
    });
  }

  /**
   * デバイス情報を取得します。
   * @returns { Promise<string> } - http-responce
   */
  async getDeviceInfo() {
    // オプションの設定
    const options = {
      hostname: 'api.nature.global',
      path: '/1/devices',
      method: 'GET',
      headers: this._generateHeader(),
    };

    // HTTPリクエストを送信
    return this._httpsRequest(options);
  }

  /**
   * 温度などの情報を取得
   * @returns { Promise<{ te: '', hu: '', il: '' }> } - http-responce
   */
  async getTemps() {
    const resp = await this.getDeviceInfo();
    if (resp.result) {
      const data = JSON.parse(resp.data);
      const te = data[0]["newest_events"].te.val
      const hu = data[0]["newest_events"].hu.val
      const il = data[0]["newest_events"].il.val
      return { te, hu, il };
    }

    return { te: '', hu: '', il: '' };
  }

  /**
   * デバイス一覧を取得します。
   * @returns { Promise<string> } - http-responce
   */
  async getAppliances() {
    // オプションの設定
    const options = {
      hostname: 'api.nature.global',
      path: '/1/appliances',
      method: 'GET',
      headers: this._generateHeader(),
    };

    // HTTPリクエストを送信
    return this._httpsRequest(options);
  }

  /**
   * デバイスにコマンドを送信します。
   * @param { String } deviceId 
   * @param { String } command 
   * @returns { Promise<string> } - http-responce
   */
  async postDeviceCommand(deviceId, command) {  
    const headers = {...this._generateHeader(), 'Content-Length': command.length}

    // オプションの設定
    const options = {
      hostname: 'api.nature.global',
      path: `/1/appliances/${deviceId}/aircon_settings`,
      method: 'POST',
      headers,
    };

    // HTTPリクエストを送信
    return this._httpsRequest(options, command);
  }

}
