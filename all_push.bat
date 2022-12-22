@echo off

rem すべてステージングエリアに追加
rem git add -all 
git add .

rem コミット
git commit -m "commit by script"

rem プッシュ
git push origin master

rem hoge