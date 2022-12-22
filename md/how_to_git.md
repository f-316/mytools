# memo

## カレントディレクトリ移動後

```bat
git init
```

## 指定したディレクトリにローカルリポジトリを作成

```bat
git init ~/work/hoge
```

## すでにコミットされた状態のリモートリポジトリがある場合は

```bat
git clone
```

## ステータス確認

```bat
git status
```

## コミットする対象ファイルをステージングエリアに全部移動

```bat
git add .
```

## ステージングエリアのをコミット

```bat
git commit
git commit -m "コミットのコメント"
```

## リポジトリのリビジョン確認

```bat
git log
```

## ローカル->リモートに反映

origin(リモートリポジトリ)に「master」ブランチの情報をプッシュ  

```bat
git push origin main
rem ↓でも行けた  
git push origin
```

## ステージングエリアとワーキングエリアの差分

```bat
git diff
```
