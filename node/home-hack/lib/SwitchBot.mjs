/** import */
import https from 'https';
import crypto from 'crypto';
import { AppLog } from './AppLog.mjs';

export class SwitchBot {
  m_token = '';
  m_secret = '';
  m_log = new AppLog('', 'SWB');
  constructor(token, secret) {
    this.m_token = token;
    this.m_secret = secret;
  }

  /**
   * 署名付きヘッダを生成します。
   * @returns { Object } - header
   */
  _generateSignedHeader() {
    // トークンとシークレットキーを設定
    const nonce = "";

    // タイムスタンプを取得（ミリ秒単位）
    const t = Date.now();

    // 署名を生成
    const stringToSign = `${this.m_token}${t}${nonce}`;
    const hash = crypto.createHmac('sha256', this.m_secret).update(stringToSign).digest('base64');
    
    return {
      'Authorization': this.m_token,
      'sign': hash,
      't': t,
      'nonce': nonce,
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
      this.m_log.error(61, `Error: ${e.message}`);
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
   * デバイスリストを取得します。
   * @returns { Promise<string> } - http-responce
   */
  async getDevices() {
    // オプションの設定
    const options = {
      hostname: 'api.switch-bot.com',
      path: '/v1.1/devices',
      method: 'GET',
      headers: this._generateSignedHeader(),
    };

    // HTTPリクエストを送信
    return this._httpsRequest(options);
  }

  /**
   * 温度などの情報を取得
   * @returns { Promise<{ te: '', hu: '', il: '' }> } - http-responce
   */
  async getTemps() {
    const resp = await this.getDeviceStatus('EF29FB69D240');
    if (resp.result) {
      const data = JSON.parse(resp.data);
      const te = data?.body?.temperature ?? '';
      const hu = data?.body?.humidity ?? '';
      const il = data?.body?.lightLevel ?? '';
      // if (te === '') {
      //   this.m_log.error(105, `data:${resp.data}`);
      // }
      return { te, hu, il };
    }

    return { te: '', hu: '', il: '' };
  }

  /**
   * デバイスのステータスを取得します。
   * @param { String } deviceId 
   * @returns { Promise<string> } - http-responce
   */
  async getDeviceStatus(deviceId) {
    // オプションの設定
    const options = {
      hostname: 'api.switch-bot.com',
      path: `/v1.1/devices/${deviceId}/status`,
      method: 'GET',
      headers: this._generateSignedHeader(),
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
    // e.g.
    // const command = JSON.stringify({'command': 'turnOn'});
    // const command = JSON.stringify({"command": "setAll","parameter": "24,5,1,on","commandType": "command"});
    // const command = JSON.stringify({"command": "SetChannel","parameter": "9","commandType": "command"});

    const headers = {...this._generateSignedHeader(), 'Content-Length': command.length}

    // オプションの設定
    const options = {
      hostname: 'api.switch-bot.com',
      path: `/v1.1/devices/${deviceId}/commands`,
      method: 'POST',
      headers,
    };

    // HTTPリクエストを送信
    return this._httpsRequest(options, command);
  }

}
