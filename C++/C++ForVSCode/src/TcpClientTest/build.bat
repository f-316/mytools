:: ����������
@echo off
setlocal

set SCRIPT_DIR=%~dp0
set WORK_DIR=%cd%
set DIST_DIR=../../dist

cd %SCRIPT_DIR%

set envBat="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
call %envBat%

REM �r���h�f�B���N�g�����쐬
if not exist "%DIST_DIR%" mkdir "%DIST_DIR%"

REM CMake���g����Visual Studio�p�v���W�F�N�g�𐶐����ADebug�r���h
if exist build rmdir /s /q build
cmake -S . -B build
if %errorlevel% neq 0 (
    echo CMake generation failed.
    exit /b %errorlevel%
)

cmake --build build
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b %errorlevel%
)

echo Build completed.

xcopy build\Debug\*.exe ..\..\dist /E /Y /I
xcopy build\Debug\*.pdb ..\..\dist /E /Y /I

cd %WORK_DIR%
endlocal
