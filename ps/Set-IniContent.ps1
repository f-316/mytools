<#
.SYNOPSIS
  iniファイルのセクション、キーを指定して値を書き換えます。
.DESCRIPTION
  dstIniを指定しない場合はsrcIniになります。
.EXAMPLE
  PS> .\Set-IniContents.ps1 -srcIni src.ini -section sectName -key keyName -value value -dstIni .\newDir1\newDir2\dst.ini
  PS> .\Set-IniContents.ps1 src.ini sectName keyName value .\newDir1\newDir2\dst.ini
  PS> .\Set-IniContents.ps1 src.ini sectName keyName value
.NOTES
  Auther: f-316
#>

# INIファイルのパスと変更する値を指定
Param(
    [string] $srcIni = "",
    [string] $section = "",
    [string] $key = "",
    [string] $value = "",
    [string] $dstIni = $srcIni
)

# パラメータチェック
$check = $true
$check = $check -and ($srcIni -ne "")
$check = $check -and ($section -ne "")
$check = $check -and ($key -ne "")
if (-not $check)
{
    $Me = $MyInvocation.MyCommand.Path
    # Get-Help $Me
    Get-Help $Me -Detailed
    return
}

# パス確認
if (-not(Test-Path($srcIni)))
{
    Write-Host $srcIni is not found.
    return
}

# INIファイルを読み込み
$iniContent = Get-Content -Path $srcIni
$outputContent = @()
$inSection = $false

# 行ごとに処理
foreach ($line in $iniContent) {
    # セクション名確認
    if ($line -match "^\[$section\]") {
        $inSection = $true
    # キー名確認
    } elseif ($inSection -and $line -match "^$key=") {
        # セクション内の正確なキーを検索して値を変更
        $line = "$key=$value"
        # キーの変更が完了したらセクションを抜ける
        $inSection = $false
    }
    
    $outputContent += $line
}

# 出力ディレクトリ準備
$parentFolder = Split-Path -Path $dstIni -Parent
# $fileName = Split-Path -Path $dstIni -Leaf
if (-not (Test-Path($parentFolder)))
{
    New-Item -Path $parentFolder -ItemType Directory | Out-Null
}

# 変更内容をファイルに保存
$outputContent | Set-Content -Path $dstIni
