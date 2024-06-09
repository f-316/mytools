[TOC]

# プロジェクト作成手順

## vite テンプレート作成

npm create vite@latest vanillats-vite -- --template vanilla-ts

## ワークスペース作成

.vscode/project.code-workspace
という名前で作る

### settings

ワークスペースごとの設定
.vscode/project.code-workspace の"settings"
フォルダに対しての設定
.vscode/settings.json

settingsに"files.exclude"と入力してCtrl+Spaceでテンプレート作成
vscodeのエクスプローラに表示されるファイルを除外できる。

### プロジェクトで推奨のextensionsを設定

https://future-architect.github.io/articles/20200828/

1. 推奨のextensionのページを開いてコマンドパレット(Ctrl+Shift+p)
1. add extension to Workspace Recommendations
1. フォルダかワークスペースかもしくは両方か選んだら.vscode 内に生成される

これも.vscode/extensions.jsonに設定するとフォルダに対しての設定になる。

## eslintを設定

### eslint

静的解析ツール
npm で落とす eslint
npx eslint
で設定した影響範囲のファイルの異常個所を出力してくれる

### dbaeumer.vscode-eslint

vscode extensionsのeslint
vscode上で波線で指摘してくれる

eslintはv9からFlatConfigがデフォルトとなった
[ESLintをeslintrcからFlat Configに移行する、ハマりポイントを添えて。](https://qiita.com/Shilaca/items/c494e4dc6b536a5231de)

### FlatConfig

FlatConfig移行ガイド
https://eslint.org/docs/latest/use/configure/migration-guide

### eslintの設定ファイルを生成

npx eslint --init  
v9で生成されるファイルが.eslintrc から eslint.config.js に変わった  

今回設定した内容  
この設定でもtsファイルのパーサが設定されていなかったりと生成直後では使えなかったため調査してカスタマイズした。
```
PS C:\Workspace\vanillats-vite> npx eslint --init
You can also run this command directly using 'npm init @eslint/config@latest'.
Need to install the following packages:
@eslint/create-config@1.1.3
Ok to proceed? (y) y
√ How would you like to use ESLint? · problems
√ What type of modules does your project use? · esm
√ Which framework does your project use? · none
√ Does your project use TypeScript? · typescript
√ Where does your code run? · browser
```
eslint.config.jsはFlatConfigの設定ファイルで
globalsはFlatConfigで使用されているため一緒にインストールされる

### plugins と extends の違い

FlatConfigになる前は分かれていた。
以前はpluginsはインポートしてrulesは自分で記載して有効になっていた。  
extendsはインポートして設定すれば反映された。  
https://www.sunapro.com/eslint-settings/

### eslint は個別設定しない

sharable configsを使う
https://www.sunapro.com/eslint-settings/

FlatConfigでは移行ガイドで
"@eslint/js"と検索するとわかりやすい

rulesで細かい設定はできるが膨大なので推奨のテンプレートを使うべき
https://qiita.com/khsk/items/0f200fc3a4a3542efa90

eslint ルール参考、困ったら見る程度、設定は極力しない
https://eslint.org/docs/latest/rules/

#### settingsに"eslint.experimental.useFlatConfig": trueを追加する

これを設定しないとvscodeでFlatConfigの設定eslint.config.jsが適用されない

### eslintではエラーになるのにdbaeumer.vscode-eslintではエラーにならない

原因1 dbaeumer.vscode-eslintの設定が必要だった
settingsに
"eslint.useESLintClass": true,
を追記
https://zenn.dev/suree/articles/994efc9206332d
https://github.com/microsoft/vscode-eslint/blob/main/CHANGELOG.md

原因2 tsファイルが解析できていなかった
eslint.config.jsに
languageOptions
parser: 'typescript-eslint/parser'
を追記
実際はimportしてオブジェクトを設定

## prettierを設定

.vscode/project.code-workspace

```json
{
  "settings": {
    "prettier.printWidth": 40
  }
}
```

.prettierrc

```json
{
  "printWidth": 200
}
```

project.code-workspace < .prettierrc
の順で優先される

### prettierの競合オフを推奨

eslint-config-prettier（ESLint のフォーマット関連のルールを全て無効にする、要は Prettier が整形した箇所に関してエラーを出さなくなる）

https://github.com/prettier/prettier/blob/554b15473dd4032a036d7db91a8f579e624c9822/docs/integrating-with-linters.md

eslint --fix 　 or Prettier
https://zenn.dev/kohski/articles/eslint-prettier-integration
eslint-config-prettier を使うのがよい？
