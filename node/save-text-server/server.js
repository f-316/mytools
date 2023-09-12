const http = require('http')
const fs = require('fs')
const port = 3000

/**
 * POST処理
 * @param {Object} req - 要求
 * @param {Object} rsp - 応答
 */
const methodGet = (req, rsp) => {
    if (req.url[0] === '/') {
        const serveFile = req.url === '/' ? './index.html' : `.${req.url}`
        // GETリクエストかつルートURL('/')に対してのみ処理
        fs.readFile(serveFile, 'utf8', (err, data) => {
            if (err) {
                rsp.writeHead(500, { 'Content-Type': 'text/plain;charset=utf-8' })
                rsp.end('Internal Server Error')
                console.error('ファイルの読み取りエラー:', err)
            } else {
                rsp.writeHead(200, { 'Content-Type': 'text/html;charset=utf-8' })
                rsp.end(data)
            }
        })
    } else {
        // それ以外のリクエストに対するレスポンス
        rsp.writeHead(404, { 'Content-Type': 'text/plain;charset=utf-8' })
        rsp.end('Not Found')
    }
}

/**
 * POSTのJSONパラメータを処理します
 * @param {Object} jsonParam - JSONパラメータ
 * @param {Object} rsp - 応答
 */
const postJsonParamProc = (jsonParam, rsp) => {
    // レスポンスを設定
    rsp.writeHead(200, { 'Content-Type': 'application/json;charset=utf-8' })
    rsp.end(JSON.stringify({ message: 'POSTリクエストを受け取りました', ...jsonParam}))
    return true
}

/**
 * リクエストからIPアドレスを取得
 * @param {Object} req - http-request
 * @return {String} ip-address
 */
const toIPAddr = function (req) {
    if (req.headers['x-forwarded-for']) {
        return req.headers['x-forwarded-for']
    } else if (req.connection && req.connection.remoteAddress) {
        return req.connection.remoteAddress
    } else if (req.connection.socket && req.connection.socket.remoteAddress) {
        return req.connection.socket.remoteAddress
    } else if (req.socket && req.socket.remoteAddress) {
        return req.socket.remoteAddress
    }
    return '0.0.0.0'
}

/**
 * saveTextを実行します
 * @param {Object} text - テキスト
 * @param {Object} rsp - 応答
 */
const saveText = (text, rsp) => {
    const fname = './save-text.txt'
    fs.writeFile(fname, text, err => {
        // 書き出しに失敗した場合
        if(err){
            // レスポンスを設定
            rsp.writeHead(500, { 'Content-Type': 'text/plain;charset=utf-8' })
            rsp.end('File save error')
            console.error('File save error:', err)
            return false
        }
        // 書き出しに成功した場合
        else{
            // レスポンスを設定
            rsp.writeHead(200, { 'Content-Type': 'text/plain;charset=utf-8' })
            rsp.end(`ファイルが正常に書き出しされました\n${text.slice(0, 20)}...`)
            console.log("ファイルが正常に書き出しされました")
        }
    })
    return true
}

/**
 * POST処理
 * @param {Object} req - 要求
 * @param {Object} rsp - 応答
 */
const methodPost = (req, rsp) => {
    let body = ''
    // リクエストボディを受け取る
    req.on('data', data => {
        body += data.toString()
        // +をスペースに変換
        body = body.replace(/\+/g," ")
        // デコード
        body = decodeURIComponent(body)
    })

    // リクエストボディの受信が完了したら処理を行う
    req.on('end', () => {
        let submitType
        // saveText確認
        submitType = 'save-text'
        if (body.slice(0, submitType.length) === submitType) {
            // `${submitType}=`を削除
            body = body.slice(`${submitType}=`.length)
            // saveText実行
            console.log(`${toIPAddr(req)} requires save-text`)
            saveText(body, rsp)
        } else {
            try {
                // リクエストボディをJSONにパース（例：JSONデータが送信された場合）
                const jsonParam = JSON.parse(body)
                postJsonParamProc(jsonParam, rsp)
            } catch (error) {
                // レスポンスを設定
                rsp.writeHead(500, { 'Content-Type': 'text/plain;charset=utf-8' })
                rsp.end('Json parse error')
                console.error('Json parse error:', error)
            }
        }
    })
}

// サーバ
const server = http.createServer((req, rsp) => {
    switch (req.method) {
    case 'GET':
        methodGet(req, rsp)
        break

    case 'POST':
        methodPost(req, rsp)            
        break
    }
})

server.listen(port)
console.log(`Server: ${port}`)
