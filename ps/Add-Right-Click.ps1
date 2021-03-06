<#
.SYNOPSIS
  ディレクトリ右クリックメニューにコマンドを追加します。
.DESCRIPTION
  管理者権限で実行する必要があります。
.EXAMPLE
  PS> ./Add-Right-Click.ps1 -cmdAlias new-cmd -cmd notepad
.INPUTS
  None
.OUTPUTS
  None
.LINK
  None
.NOTES
  Auther: f-316
#>

Param (
    # 右クリックメニューに表示される名前
    [String]$cmdAlias,
    # コマンド
    [String]$cmd
)

# コマンド名確認
if ($cmdAlias -eq "") {
    $me = $MyInvocation.MyCommand.Path
    Get-Help $me -Detail
    pause
    return
}
# コマンド確認
if ($cmd -eq "") {
    $me = $MyInvocation.MyCommand.Path
    Get-Help $me -Detail
    pause
    return
}

# 実行ポリシー変更
$lastPolicy = Get-ExecutionPolicy
try {
    Set-ExecutionPolicy AllSigned -Force
}
catch {
    Write-Output ('Error message is ' + $_.Exception.Message)
    Write-Output "管理者権限で実行してください。"
    pause
    return
}

# HKEY_CLASSES_ROOTをHKCRとして登録
New-PSDrive -Name "HKCR" -PSProvider Registry -Root "HKEY_CLASSES_ROOT" > $null
# Remove-PSDrive -Name HKCR

$dirShellPath = "HKCR:\Directory\Background\shell\"
$keyCommand = "\command"

# 階層を再帰的に作成
New-Item ($dirShellPath + $cmdAlias + $keyCommand) -Force > $null

# 書き込み
Set-ItemProperty -LiteralPath ($dirShellPath + $cmdAlias + $keyCommand) -Name "(default)" -Value $cmd
# Get-ItemProperty -LiteralPath ($dirShellPath + $cmdAlias + $keyCommand)

# 実行ポリシーを戻す
Set-ExecutionPolicy $lastPolicy -Force

Write-Output "$cmdAlias コマンドが正常に追加されました。"
Write-Output "コマンド: $cmd"
pause
