/** pre */
import path from 'path'
const NODE_EXE = process.argv[0];
const THIS_PATH = process.argv[1];
const THIS_FNAME = path.basename(THIS_PATH);
const THIS_DIR = path.dirname(THIS_PATH);
const WORKING_DIR = path.resolve('.');

/** import */
import { GoogleCloudAPI } from './lib/GoogleCloudAPI.mjs';

/** arg defaults */

/**
 * main
 * @param { string[] } argv 
 * @param { number } argc 
 */
const main = async (argv, argc) => {
  const CREDENTIALS_PATH = '#token/gc_credentials.json';
  const TOKEN_PATH = '#token/gc_token.json';

  const gcApi = new GoogleCloudAPI(CREDENTIALS_PATH, TOKEN_PATH);
  gcApi.saveFile('#test/src.txt', '1QE3YdDcQviPFzap0KU2dern-Bj-Ly0xs', 'sample.txt');
}

// main
main(process.argv.slice(2), process.argv.slice(2).length)
