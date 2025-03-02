/** import */
import fs from 'fs';
import { google } from 'googleapis';
import { authenticate } from '@google-cloud/local-auth';

export class GoogleCloudAPI {
  SCOPES = ['https://www.googleapis.com/auth/drive.file'];
  m_credentialsPath = '#token/gc_credentials.json';
  m_tokenPath = '#token/gc_token.json';
  m_logPath = 'GoogleCloudAPI.log';
  constructor(credentialsPath, tokenPath) {
    // 認証情報の設定
    if (credentialsPath) {
      this.m_credentialsPath = credentialsPath;
    }
    if (tokenPath) {
      this.m_tokenPath = tokenPath;
    }
  }
  
  /**
   * 認証を行う関数
   * @returns { Promise<OAuth2Client> } auth
   */
  async authorize() {
    // トークンを読み込む
    try {
      const content = fs.readFileSync(this.m_tokenPath);
      const credentials = JSON.parse(content);
      const auth = new google.auth.OAuth2();
      auth.setCredentials(credentials);

      // アクセストークンが期限切れの場合、リフレッシュトークンを使用して新しいトークンを取得
      if (auth.credentials.expiry_date && auth.credentials.expiry_date <= Date.now()) {
        console.log(`refreshAccessToken, ${auth.credentials.expiry_date},${Date.now()}`);
        await refreshAccessToken(auth);
      }

      return auth;
    } catch (err) {
      console.log(err);
      fs.writeFile(this.m_logPath, `${new Date().toLocaleString()} [ERROR]${err}\n`, {flag:'a'}, err => !err ?? console.error(err));
      
      const auth = await authenticate({
        keyfilePath: this.m_credentialsPath,
        scopes: this.SCOPES,
      });
      fs.writeFileSync(this.m_tokenPath, JSON.stringify(auth.credentials));
      return auth;
    }
  }

  /**
   * リフレッシュトークンを使用して新しいアクセストークンを取得
   * @param { OAuth2Client } auth
   * @returns { Promise<OAuth2Client> } auth
   */
  async refreshAccessToken(auth) {
    return new Promise((resolve, reject) => {
      auth.refreshAccessToken((err, token) => {
        if (err) {
          reject('Error refreshing access token: ' + err);
        }
        auth.setCredentials(token);
        fs.writeFileSync(this.m_tokenPath, JSON.stringify(token));
        resolve(auth);
      });
    });
  }

  /**
   * ファイルを検索
   * folderIdがnullならすべてから検索して最初にヒットしたfileIdを返す。
   * @param { OAuth2Client } auth 
   * @param { String } remoteFileName 
   * @param { String } folderId
   * @param { String } fileId 
   */
  async searchFile(auth, remoteFileName, folderId = null) {
    const drive = google.drive({ version: 'v3', auth });
    const query = folderId !== null ? `name='${remoteFileName}' and '${folderId}' in parents and trashed=false` : `name='${remoteFileName}' and trashed=false`
    const res = await drive.files.list({
        q: query,
        fields: 'files(id, name)'
    });
    const fileId = res.data.files.length ? res.data.files[0].id : null;
    return fileId;
  }

  /**
   * ファイルをアップデートする関数
   * @param { OAuth2Client } auth 
   * @param { String } localFilePath 
   * @param { String } fileId 
   * @returns { String } fileId
   */
  async updateFile(auth, localFilePath, fileId) {
    const drive = google.drive({ version: 'v3', auth });
    const media = {
        mimeType: 'text/plain',
        body: fs.createReadStream(localFilePath)
    };
    const res = await drive.files.update({
        fileId: fileId,
        media: media,
        fields: 'id'
    });

    const _fileId = res.data.id;
    return _fileId;
  }
  
  /**
   * ファイルをアップロード
   * @param { OAuth2Client } auth 
   * @param { String } localFilePath 
   * @param { String } folderId - ドライブでフォルダのリンクをコピーしたときの`https://drive.google.com/file/d/${folderId}/view?usp=drive_link`
   * @param { String } remoteFileName 
   * @returns { String } fileId
   */
  async uploadFile(auth, localFilePath, folderId, remoteFileName) {
    const drive = google.drive({ version: 'v3', auth });
    const fileMetadata = {
      'name': remoteFileName,
      'parents': [folderId] // フォルダIDを指定
    };
    const media = {
      mimeType: 'text/plain',
      body: fs.createReadStream(localFilePath)
    };
    const res = await drive.files.create({
      resource: fileMetadata,
      media: media,
      fields: 'id'
    });

    const fileId = res.data.id;
    return fileId;
  }

  /**
   * 
   * @param { String } localFilePath 
   * @param { String } folderId 
   * @param { String } remoteFileName 
   * @returns { String } fileId
   */
  async saveFile(localFilePath, folderId, remoteFileName) {
    const auth = await this.authorize();

    const fileId = await this.searchFile(auth, remoteFileName, folderId);
    if (fileId) {
      return await this.updateFile(auth, localFilePath, fileId);
    } else {
      return await this.uploadFile(auth, localFilePath, folderId, remoteFileName);
    }
  }
}
