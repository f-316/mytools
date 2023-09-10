<#
.SYNOPSIS
  ini�t�@�C���̃Z�N�V�����A�L�[���w�肵�Ēl���Z�b�g���܂��B
.DESCRIPTION
  dstIni���w�肵�Ȃ��ꍇ��srcIni�ɂȂ�܂��B
  srcIni�����݂��Ȃ��ꍇ�V�����쐬���܂��B
  ini�t�@�C���ɃZ�N�V���������݂��Ȃ��ꍇ�A�Z�N�V�������쐬���ăL�[�l��ݒ肵�܂��B
  �Z�N�V�������ɃL�[�����݂��Ȃ��ꍇ�A�L�[�l���쐬���܂��B
.EXAMPLE
  PS> .\Set-IniContents.ps1 -section sectName -key keyName -value value -srcIni src.ini -dstIni .\newDir1\newDir2\dst.ini
  PS> .\Set-IniContents.ps1 sectName keyName value src.ini .\newDir1\newDir2\dst.ini
  PS> .\Set-IniContents.ps1 sectName keyName value src.ini
.NOTES
  Auther: f-316
#>

# INI�t�@�C���̃p�X�ƕύX����l���w��
Param(
    [string] $section = "",
    [string] $key = "",
    [string] $value = "",
    [string] $srcIni = "",
    [string] $dstIni = $srcIni
)

# �p�����[�^�`�F�b�N
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

# ���ݎ��A�����t�@�C���ǂݍ���
if (Test-Path($srcIni))
{
    # INI�t�@�C����ǂݍ���
    $iniContent = Get-Content -Path $srcIni
    $inSection = $false

    # �s���Ƃɏ���
    foreach ($line in $iniContent) {
        # �Z�N�V�������������ꍇ
        if ($line -match "^\[$section\]") {
            $inSection = $true
            $foundSection = $true
        }
        # �Z�N�V�������ɃL�[���������ꍇ
        elseif ($inSection -and $line -match "^$key=") {
            # �Z�N�V�������̐��m�ȃL�[���������Ēl��ύX
            $line = "$key=$value"
            # �L�[�̕ύX������������Z�N�V�����𔲂���
            $inSection = $false
            $keySet = $true
        }
        # �L�[�������炸�ʃZ�N�V�������������ꍇ
        elseif ($inSection -and $line -match "^\[") {
            $inSection = $false
            $sectionEnd = $lineCnt
        }
        $lineCnt++
        $outputContent += $line
    }

    # �L�[�������炸�Z�N�V�����𔲂����ꍇ
    if ($sectionEnd -ne 0)
    {
        $outputContent = @()
        $lineCnt = 0
        # �s���Ƃɏ���
        foreach ($line in $iniContent) {
            # �Z�N�V���������ɃL�[�l�ǉ�
            if ($sectionEnd -eq $lineCnt) {
                $outputContent += "$key=$value"
            }
            $lineCnt++
            $outputContent += $line
        }
    }
    # �Z�N�V������������Ȃ������ꍇ
    elseif (-not $foundSection)
    {
        # �t�@�C�������ɃZ�N�V�����A�L�[�l��ǉ�
        $outputContent += "[$section]"
        $outputContent += "$key=$value"
    }
    # �Z�N�V���������������L�[�������炸ini�t�@�C���𔲂����ꍇ
    elseif (-not $keySet)
    {
        # �t�@�C�������ɃL�[�l��ǉ�
        $outputContent += "$key=$value"
    }
}
# �V�K�쐬
else
{
    $outputContent += "[$section]"
    $outputContent += "$key=$value"
}

# �o�̓f�B���N�g������
$parentFolder = Split-Path -Path $dstIni -Parent
# $fileName = Split-Path -Path $dstIni -Leaf
if (($parentFolder -ne "") -and (-not (Test-Path($parentFolder))))
{
    New-Item -Path $parentFolder -ItemType Directory | Out-Null
}

# �ύX���e���t�@�C���ɕۑ�
$outputContent | Set-Content -Path $dstIni
