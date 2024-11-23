import fs from 'fs';

const targetLibDirs = getDirNames('./dist')
resolveImportPath('./dist', targetLibDirs);

/**
 * 配下のディレクトリを取得します。
 * @param { String } directory 
 * @param { Boolean } recurse 
 * @param { String[] } dirNames 
 * @return String[] - dirNames
 */
function getDirNames(directory, recurse = true, dirNames = null) {
    if (dirNames === null) {
        dirNames = [];
    }
    const dirsAndFiles = fs.readdirSync(directory);
    dirsAndFiles.forEach(dirOrFile => {
        const dirPathOrFilePath = `${directory}/${dirOrFile}`;
        // ディレクトリ
        if (fs.lstatSync(dirPathOrFilePath).isDirectory()) {
            const dirPath = dirPathOrFilePath;
            dirNames.push(dirOrFile);
            if (recurse) {
                dirNames = getDirNames(dirPath, recurse, dirNames);
            }
        }
    });
    return dirNames;
}

/**
 * ディレクトリインポートを解決します。
 * ESModuleはディレクトリインポートをサポートしないため
 * 各ファイルのimport文をファイルインポートに書き換えます。
 * @param {*} directory 
 * @param {*} targetLibDirs 
 */
function resolveImportPath(directory, targetLibDirs) {
    // * ディレクト構成が以下の場合
    // ** lib/Lib1/Lib1.js
    // ** lib/Lib1/index.js
    // * ディレクトリインポート
    // ** impport { Lib1 } from 'lib/Lib1'の記法でLib1をimportする手法
    // * ファイルインポート
    // ** impport { Lib1 } from 'lib/Lib1/index.js'のように明示的にファイルを指定する手法

    // 処理
    // directory配下のtargetLibDirsに対して
    // * index.jsの場合
    // ** impport { Lib1 } from './Lib1'
    // ** ↓
    // ** impport { Lib1 } from './Lib1.js'
    // * それ以外の場合
    // ** impport { Lib1 } from '~/Lib1'
    // ** ↓
    // ** impport { Lib1 } from '~/Lib1/index.js'
   
    const dirsAndFiles = fs.readdirSync(directory);
    dirsAndFiles.forEach(dirOrFile => {
        const dirPathOrFilePath = `${directory}/${dirOrFile}`;
        // ディレクトリ
        if (fs.lstatSync(dirPathOrFilePath).isDirectory()) {
            const dirPath = dirPathOrFilePath;
            resolveImportPath(dirPath, targetLibDirs);
        // ファイル
        } else if (dirOrFile.slice(-3) === '.js') {
            const filePath = dirPathOrFilePath;
            let content = fs.readFileSync(filePath, 'utf8');

            targetLibDirs.forEach(targetDir => {
                if (!filePath.includes('index.js')) {
                    const dirRegex = new RegExp(`from "(.*/)${targetDir}";`, 'g');
                    content = content.replace(dirRegex, `from "$1${targetDir}/index.js";`);
                }

                const fileRegex = new RegExp(`from "\\.\\/${targetDir}";`, 'g');
                content = content.replace(fileRegex, `from "./${targetDir}.js";`);
            });

            fs.writeFileSync(filePath, content, 'utf8');
        }
    });
}
