<#
.SYNOPSIS
  iniファイルのセクション、キーを指定して値をセットします。
.DESCRIPTION
  dstIniを指定しない場合はsrcIniになります。
  srcIniが存在しない場合新しく作成します。
  iniファイルにセクションが存在しない場合、セクションを作成してキー値を設定します。
  セクション内にキーが存在しない場合、キー値を作成します。
.EXAMPLE
  PS> .\Set-IniContents.ps1 -section sectName -key keyName -value value -srcIni src.ini -dstIni .\newDir1\newDir2\dst.ini
  PS> .\Set-IniContents.ps1 sectName keyName value src.ini .\newDir1\newDir2\dst.ini
  PS> .\Set-IniContents.ps1 sectName keyName value src.ini
.NOTES
  Auther: f-316
#>

# INIファイルのパスと変更する値を指定
Param(
    [string] $section = "",
    [string] $key = "",
    [string] $value = "",
    [string] $srcIni = "",
    [string] $dstIni = $srcIni
)

# パラメータチェック
$check = $true
$check = $check -and ($section -ne "")
$check = $check -and ($key -ne "")
$check = $check -and ($srcIni -ne "")
if (-not $check)
{
    $Me = $MyInvocation.MyCommand.Path
    # Get-Help $Me
    Get-Help $Me -Detailed
    return
}

$outputContent = @()
$lineCnt = 0
$sectionEnd = 0
$foundSection = $false
$keySet = $false

# 存在時、既存ファイル読み込み
if (Test-Path($srcIni))
{
    # INIファイルを読み込み
    $iniContent = Get-Content -Path $srcIni
    $inSection = $false

    # 行ごとに処理
    foreach ($line in $iniContent) {
        # セクションを見つけた場合
        if ($line -match "^\[$section\]") {
            $inSection = $true
            $foundSection = $true
        }
        # セクション内にキーを見つけた場合
        elseif ($inSection -and $line -match "^$key=") {
            # セクション内の正確なキーを検索して値を変更
            $line = "$key=$value"
            # キーの変更が完了したらセクションを抜ける
            $inSection = $false
            $keySet = $true
        }
        # キーが見つからず別セクションを見つけた場合
        elseif ($inSection -and $line -match "^\[") {
            $inSection = $false
            $sectionEnd = $lineCnt
        }
        $lineCnt++
        $outputContent += $line
    }

    # キーが見つからずセクションを抜けた場合
    if ($sectionEnd -ne 0)
    {
        $outputContent = @()
        $lineCnt = 0
        # 行ごとに処理
        foreach ($line in $iniContent) {
            # セクション末尾にキー値追加
            if ($sectionEnd -eq $lineCnt) {
                $outputContent += "$key=$value"
            }
            $lineCnt++
            $outputContent += $line
        }
    }
    # セクションが見つからなかった場合
    elseif (-not $foundSection)
    {
        # ファイル末尾にセクション、キー値を追加
        $outputContent += "[$section]"
        $outputContent += "$key=$value"
    }
    # セクションを見つけたがキーが見つからずiniファイルを抜けた場合
    elseif (-not $keySet)
    {
        # ファイル末尾にキー値を追加
        $outputContent += "$key=$value"
    }
}
# 新規作成
else
{
    $outputContent += "[$section]"
    $outputContent += "$key=$value"
}

# 出力ディレクトリ準備
$parentFolder = Split-Path -Path $dstIni -Parent
# $fileName = Split-Path -Path $dstIni -Leaf
if (($parentFolder -ne "") -and (-not (Test-Path($parentFolder))))
{
    New-Item -Path $parentFolder -ItemType Directory | Out-Null
}

# 変更内容をファイルに保存
$outputContent | Set-Content -Path $dstIni
