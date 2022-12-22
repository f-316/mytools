@echo off
powershell -NoProfile -ExecutionPolicy RemoteSigned -File Template.ps1

pause

powershell -NoProfile -ExecutionPolicy RemoteSigned -File Template.ps1 -arg1 hello -arg2 10