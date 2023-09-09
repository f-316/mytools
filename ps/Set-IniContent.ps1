<#
.SYNOPSIS
  ini�t�@�C���̃Z�N�V�����A�L�[���w�肵�Ēl�����������܂��B
.DESCRIPTION
  dstIni���w�肵�Ȃ��ꍇ��srcIni�ɂȂ�܂��B
.EXAMPLE
  PS> .\Set-IniContents.ps1 -srcIni src.ini -section sectName -key keyName -value value -dstIni .\newDir1\newDir2\dst.ini
  PS> .\Set-IniContents.ps1 src.ini sectName keyName value .\newDir1\newDir2\dst.ini
  PS> .\Set-IniContents.ps1 src.ini sectName keyName value
.NOTES
  Auther: f-316
#>

# INI�t�@�C���̃p�X�ƕύX����l���w��
Param(
    [string] $srcIni = "",
    [string] $section = "",
    [string] $key = "",
    [string] $value = "",
    [string] $dstIni = $srcIni
)

# �p�����[�^�`�F�b�N
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

# �p�X�m�F
if (-not(Test-Path($srcIni)))
{
    Write-Host $srcIni is not found.
    return
}

# INI�t�@�C����ǂݍ���
$iniContent = Get-Content -Path $srcIni
$outputContent = @()
$inSection = $false

# �s���Ƃɏ���
foreach ($line in $iniContent) {
    # �Z�N�V�������m�F
    if ($line -match "^\[$section\]") {
        $inSection = $true
    # �L�[���m�F
    } elseif ($inSection -and $line -match "^$key=") {
        # �Z�N�V�������̐��m�ȃL�[���������Ēl��ύX
        $line = "$key=$value"
        # �L�[�̕ύX������������Z�N�V�����𔲂���
        $inSection = $false
    }
    
    $outputContent += $line
}

# �o�̓f�B���N�g������
$parentFolder = Split-Path -Path $dstIni -Parent
# $fileName = Split-Path -Path $dstIni -Leaf
if (-not (Test-Path($parentFolder)))
{
    New-Item -Path $parentFolder -ItemType Directory | Out-Null
}

# �ύX���e���t�@�C���ɕۑ�
$outputContent | Set-Content -Path $dstIni
