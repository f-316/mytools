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
    [String]$cmdAlias
)

# コマンド名確認
if ($cmdAlias -eq "") {
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

# コマンド削除
Remove-Item -LiteralPath ($dirShellPath + $cmdAlias) -Recurse

# 実行ポリシーを戻す
Set-ExecutionPolicy $lastPolicy -Force

Write-Output "$cmdAlias コマンドの削除が正常に完了しました。"
pause
