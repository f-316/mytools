<#
.SYNOPSIS
  ディレクトリ右クリックメニューにコマンドを追加します。
.DESCRIPTION
  管理者権限で実行する必要があります。
.EXAMPLE
  PS> ./Template.ps1 -arg1 hoge -cmd 1
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
    # Arg1
    [String]$arg1,
    # コマンド
    [Int]$arg2 = 0
)

function Show-MyHelp {
  param (
    $me
  )
  Get-Help $me -Detail
}

# 引数1確認
if ($arg1 -eq "") {
    Show-MyHelp -me $MyInvocation.MyCommand.Path
    return
}

# 引数2確認
if ($arg2 -eq 0) {
    Show-MyHelp -me $MyInvocation.MyCommand.Path
    return
}

# powershell -NoProfile -ExecutionPolicy RemoteSigned .\Template.ps1 -arg1 hello -arg2 10

#  --- 処理 ---
  Write-Host arg1=$arg1
  Write-Host arg2=$arg2
  Write-Host OK!

  # Write-Hostの.NET Framework版
  # コマンドレットより高速処理、大量に表示する場合に有効
  [System.Console]::WriteLine("[System.Console]::WriteLine")

  # Write-Hostの.NET Framework版
  # 改行しない
  [System.Console]::Write("[System.Console]::Write")
  Write-Host ""

  Write-Host -NoNewline
  "Write-Host -NoNewline"
  "|o|o|x|"
  "|x|o|x|"
  "|x|o| |"
  Write-Host -ForegroundColor Red red string
  Write-Host -BackgroundColor Red red background
  Write-Host -ForegroundColor Red -BackgroundColor Yellow red str yellow back
  Write-Host -ForegroundColor White -BackgroundColor Black red str yellow back

  # 以降の背景色を変更
  [Console]::BackgroundColor = [ConsoleColor]::Green
  Write-Host green background
  [Console]::BackgroundColor = [ConsoleColor]::Black

  # 以降の文字色を変更
  [Console]::ForegroundColor = [ConsoleColor]::Green
  Write-Host green string
  [Console]::ForegroundColor = [ConsoleColor]::White

  # 背景黒、文字黄色
  Write-Warning "Write-Warning"

  # 背景黒、文字赤色、エラー文
  Write-Error "Write-Error"

  # -Verbose オプションが指定された時だけ黄色文字で「詳細:」を先頭に付加して表示
  # -Verbose オプションが指定されない時は何も表示しない。
  Write-Verbose -Verbose "Write-Verbose"

  # -Debug オプションが指定された時だけ黄色文字で「デバッグ:」を先頭に付加表示し処理を中断。
  # Write-Debug -Debug "Write-Debug"

  Write-Host "How to return error to caller"
  throw "Bad thing happened"
  Write-Host "This message will not be printed."

#  --- 処理終わり ---
