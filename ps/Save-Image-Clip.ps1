<#
.SYNOPSIS
  クリップボードにある画像を作業ディレクトリに保存します。
.DESCRIPTION
  ファイル名: YYYYMMDD_hhmmss.jpg
.EXAMPLE
  PS> ./Save-Image-Clip.ps1
.INPUTS
  None
.OUTPUTS
  None
.LINK
  None
.NOTES
  Auther: f-316
#>

# クリップボードの画像取得
Add-Type -AssemblyName System.Windows.Forms
$clipboardImage = [Windows.Forms.Clipboard]::GetImage()

# 取得したクリップボードが画像か判定
if ($clipboardImage -eq $null)
{
  echo "No images in clipboard."
  pause
}

# 実行ディレクトリ取得
$workDir = Convert-Path .

# 画像ファイル名を現在日時に設定
$filePath = $workDir + "\" + (Get-Date -Format "yyyyMMdd_HHmmss") + ".jpg"

# 保存
$clipboardImage.Save($filePath)
echo ("Save at " + $filePath)
