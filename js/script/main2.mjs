import path from 'path'
const NODE_EXE = process.argv[0];
const THIS_PATH = process.argv[1];
const THIS_FNAME = path.basename(THIS_PATH);
const THIS_DIR = path.dirname(THIS_PATH);
const THIS_ARGV = process.argv.slice(2);
if (false) console.log(
    `nodeExe=${NODE_EXE}` + 
    `\nmyPath=${THIS_PATH}` + 
    `\nmyName=${THIS_FNAME}` + 
    `\nmyDir=${THIS_DIR}` +
    `\nmyArgv=${THIS_ARGV}`
);

import fs from 'fs'
class Log {
    constructor(filePath) {
        this.m_path = filePath;
        this.m_writeOption = {
            encoding: 'utf8',
            flag: 'a'
        }
        this.m_eol = '\n';
    }

    get now() {
        return new Date().toLocaleString(undefined,
            {
                year: 'numeric',
                month: '2-digit',
                day: '2-digit',
                hour: '2-digit',
                minute: '2-digit',
                second: '2-digit'
            })
    }

    log(text) {
        this.write(this.m_path, `${this.now} ${text}${this.m_eol}`, this.m_writeOption);
    }

    trace(text) {
        this.write(this.m_path, `${this.now} [TRACE]${text}${this.m_eol}`, this.m_writeOption);
    }

    write(targetPath, contents, option = { encoding: 'utf8', flag: 'a'}) {

        // なかったら作る
        const targetDir = path.dirname(targetPath)
        if (!fs.existsSync(targetPath)) { 
            fs.mkdirSync(targetDir, {recursive: true})
        }
    
        fs.writeFileSync(targetPath, contents, option)
    }
}

async function sleep(timeoutInMs) {
    return new Promise(resolve => {
        setTimeout(() => {
            resolve();
        }, timeoutInMs);
    })
}

(async function main(argv, argc) {
    const filePath = `/dir2/test.txt`
    const targetPath = `${THIS_DIR}${filePath}`

    const log = new Log(targetPath);
    log.log(`aiu`);
    await sleep(1000);
    log.log(`aiu`);
    await sleep(2000);
    log.log(`aiu`);
})(THIS_ARGV, THIS_ARGV.length)
