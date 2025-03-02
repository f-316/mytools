/** import */
import https from 'https';
import crypto from 'crypto';

/**
 * 署名付きヘッダを生成します。
 * @param { String } token 
 * @param { String } secret 
 * @returns { Object } - header
 */
function generateSignedHeader(token, secret) {
  // トークンとシークレットキーを設定
  const nonce = "";

  // タイムスタンプを取得（ミリ秒単位）
  const t = Date.now();

  // 署名を生成
  const stringToSign = `${token}${t}${nonce}`;
  const hash = crypto.createHmac('sha256', secret).update(stringToSign).digest('base64');
  
  return {
    'Authorization': token,
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
async function httpsRequest(options, chunk = undefined) {
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
    console.error(`Error: ${e.message}`);
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
 * swith-botのデバイスリストを取得します。
 * @param { String } token 
 * @param { String } secret 
 * @returns { Promise<string> } - http-responce
 */
export async function getDevices(token, secret) {
  // オプションの設定
  const options = {
    hostname: 'api.switch-bot.com',
    path: '/v1.1/devices',
    method: 'GET',
    headers: generateSignedHeader(token, secret),
  };

  // HTTPリクエストを送信
  return httpsRequest(options);
}

/**
 * swith-botのデバイスのステータスを取得します。
 * @param { String } token 
 * @param { String } secret 
 * @param { String } deviceId 
 * @returns { Promise<string> } - http-responce
 */
export async function getDeviceStatus(token, secret, deviceId) {
  // オプションの設定
  const options = {
    hostname: 'api.switch-bot.com',
    path: `/v1.1/devices/${deviceId}/status`,
    method: 'GET',
    headers: generateSignedHeader(token, secret),
  };

  // HTTPリクエストを送信
  return httpsRequest(options);
}

/**
 * swith-botのデバイスにコマンドを送信します。
 * @param { String } token 
 * @param { String } secret 
 * @param { String } deviceId 
 * @param { String } command 
 * @returns { Promise<string> } - http-responce
 */
export async function postDeviceCommand(token, secret, deviceId, command) {  
  const headers = {...generateSignedHeader(token, secret), 'Content-Length': command.length}

  // オプションの設定
  const options = {
    hostname: 'api.switch-bot.com',
    path: `/v1.1/devices/${deviceId}/commands`,
    method: 'POST',
    headers,
  };

  // HTTPリクエストを送信
  return httpsRequest(options, command);
}
