# これでサブフォルダも
# $files = (Get-ChildItem *.csv -Recurse).FullName

# これはフルパス取得
# $files = (Get-ChildItem *.csv).FullName
# $files = (Get-ChildItem *.csv).Name

# これはファイル名のみ取得
$files = Get-ChildItem *.csv -Name
$files | write-host
return
