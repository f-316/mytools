/** pre */
import path from 'path'
const NODE_EXE = process.argv[0];
const THIS_PATH = process.argv[1];
const THIS_FNAME = path.basename(THIS_PATH);
const THIS_DIR = path.dirname(THIS_PATH);
const WORKING_DIR = path.resolve('.');
main(process.argv.slice(2), process.argv.slice(2).length)

/** import */
import fs from 'fs';
import { exec } from 'child_process';
import { IniTool } from '../modules/ini-tool.mjs';

/**
 * main
 * @param { string[] } argv 
 * @param { number } argc 
 */
async function main(argv, argc) {
    let lSrcPath = './#src/a.ini';
    let rSrcPath = './#src/b.ini';
    let diffTool = 'C:/Program Files/WinMerge/WinMergeU.exe';
    let tempDir = './#tmp/';

    // arg check
    if (2 <= argc) {
        lSrcPath = argv[0];
        rSrcPath = argv[1];
    }
    if (3 <= argc) {
        diffTool = argv[2];
    }
    if (4 <= argc) {
        tempDir = argv[3];
    }

    // normalize dir path
    tempDir = tempDir.replaceAll('\\', '/')
    tempDir = tempDir.slice(-1) !== '/' ? `${tempDir}/` : tempDir;

    try {
        // read json files
        const lIni = new IniTool(lSrcPath);
        const rIni = new IniTool(rSrcPath);
        lIni.load();
        rIni.load();

        // get file names
        const lFname = path.basename(lSrcPath);
        const rFname = path.basename(rSrcPath);
        const dstLFPath = path.resolve(`${tempDir}left_${lFname}`);
        const dstRFPath = path.resolve(`${tempDir}right_${rFname}`);

        // mkdir if not exists
        if (!fs.existsSync(tempDir))
        {
            fs.mkdirSync(tempDir, {recursive: true})
        }

        // export to temp dir
        lIni.writeIni(`${dstLFPath}`)
        rIni.writeIni(`${dstRFPath}`)

        // diff
        exec(`"${diffTool}" "${dstLFPath}" "${dstRFPath}"`)

    } catch (error) {
        console.error(error);
        console.error(`lSrcPath=${lSrcPath}`);
        console.error(`rSrcPath=${rSrcPath}`);
        console.error(`tempDir=${tempDir}`);
        return false;
    }

    return true;
}
