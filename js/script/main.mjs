/** pre */
import path from 'path'
const NODE_EXE = process.argv[0];
const THIS_PATH = process.argv[1];
const THIS_FNAME = path.basename(THIS_PATH);
const THIS_DIR = path.dirname(THIS_PATH);
const WORKING_DIR = path.resolve('.');
main(process.argv.slice(2), process.argv.slice(2).length)

/** import */

/**
 * main
 * @param { string[] } argv 
 * @param { number } argc 
 */
async function main(argv, argc) {
    // arg pair check
    if (argc % 2 !== 0) {
        console.error(`argv is not pair,%o`, argv);
        return false;
    }
    let argObj = {};
    for (let i = 0; i < argc; i += 2) {
        argObj[argv[i]] = argv[i + 1];
    }

    console.log(argObj);
    return true;
}


