# テレビ
$ curl -X POST -H 'Content-Type: application/json; charset=utf8' \
  -H 'Authorization: [開発者用トークン]' \
  -d '{"command": "SetChannel","parameter": "9","commandType": "command"}' \
  https://api.switch-bot.com/v1.0/devices/[テレビのデバイスID]/commands


# エアコン
$ curl -X POST -H 'Content-Type: application/json; charset=utf8' \
  -H 'Authorization: [開発者トークン]' \
  -d '{"command": "setAll","parameter": "26,2,1,on","commandType": "command"}' \
  https://api.switch-bot.com/v1.0/devices/[エアコンのデバイスID]/commands

上記リクエストでは、温度が26℃の冷房を風速:自動にてエアコンを起動しています。
setAll コマンドと、parameter にて設定を行います。
4つの引数がありますが、1つ目は温度です。
2つ目はモードです。2が冷房、3が除湿（ドライ）、5が暖房になります。
3つ目は風速です。1が自動、2が弱、3が中、4が強になります。
4つ目は電源です。電源ONにしたい時は[on]を、OFFにした時は[off]を指定します。
APIのレスポンスの内容はその1と同じです。