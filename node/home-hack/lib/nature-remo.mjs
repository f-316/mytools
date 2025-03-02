/** import */
import https from 'https';

/**
 * ヘッダを生成します。
 * @param { String } token 
 * @returns { Object } - header
 */
function generateHeader(token) {
  return {
    'Authorization': `Bearer ${token}`,
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
 * nature-remoのデバイス情報を取得します。
 * @param { String } token 
 * @param { String } secret 
 * @returns { Promise<string> } - http-responce
 */
export async function getDeviceInfo(token, secret) {
  // オプションの設定
  const options = {
    hostname: 'api.nature.global',
    path: '/1/devices',
    method: 'GET',
    headers: generateHeader(token),
  };

  // HTTPリクエストを送信
  return httpsRequest(options);
}

/**
 * nature-remoのデバイス一覧を取得します。
 * @param { String } token 
 * @returns { Promise<string> } - http-responce
 */
export async function getAppliances(token) {
  // オプションの設定
  const options = {
    hostname: 'api.nature.global',
    path: '/1/appliances',
    method: 'GET',
    headers: generateHeader(token),
  };

  // HTTPリクエストを送信
  return httpsRequest(options);
}

/**
 * nature-remoのデバイスにコマンドを送信します。
 * @param { String } token 
 * @param { String } secret 
 * @param { String } deviceId 
 * @param { String } command 
 * @returns { Promise<string> } - http-responce
 */
export async function postDeviceCommand(token, secret, deviceId, command) {  
  const headers = {...generateHeader(token), 'Content-Length': command.length}

  // オプションの設定
  const options = {
    hostname: 'api.nature.global',
    path: `/1/appliances/${deviceId}/aircon_settings`,
    method: 'POST',
    headers,
  };

  // HTTPリクエストを送信
  return httpsRequest(options, command);
}
