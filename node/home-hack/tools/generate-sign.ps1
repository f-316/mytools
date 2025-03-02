# トークンとシークレットキーを設定
$token = "<your_token>"
$secret = "<your_secret>"
$nonce = ""

# タイムスタンプを取得（ミリ秒単位）
$t = [int64](Get-Date -UFormat %s) * 1000

# 署名を生成
$stringToSign = "$token$t$nonce"
$secretKey = [System.Text.Encoding]::UTF8.GetBytes($secret)
$stringToSignBytes = [System.Text.Encoding]::UTF8.GetBytes($stringToSign)

$hmacsha256 = New-Object System.Security.Cryptography.HMACSHA256
$hmacsha256.Key = $secretKey
$hash = $hmacsha256.ComputeHash($stringToSignBytes)

$sign = [Convert]::ToBase64String($hash)

# 生成された署名を表示
Write-Output "Sign: $sign"
Write-Output "Timestamp: $t"
Write-Output "Nonce: $nonce"
