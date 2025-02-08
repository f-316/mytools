import fs from 'fs'

export class IniTool {
    m_filePath = '';
    m_encoding = '';
    m_eol = '';
    m_withBom = true;
    m_iniObj = {};

    /**
     * 
     * @param { string } filePath 
     * @param { string } encofing - e.g.) 'utf16le', 'utf8'
     * @param { string } eol - e.g.) '\r\n', '\n'
     */
    constructor(filePath, encofing = 'utf16le', eol = '\r\n') {
        this.m_filePath = filePath;
        this.m_encoding = encofing;
        this.m_eol = eol;
    }

    /**
     * 
     * @returns { object }
     */
    async load() {
        try {
            const iniText = fs.readFileSync(this.m_filePath, {encoding: this.m_encoding});
            this.m_iniObj = this.toObj(iniText);
        } catch (error) {
            console.error(error);
            return undefined;
        }

        return this.m_iniObj;
    }

    /**
     * 読み込んだデータをini形式で出力
     * @param { string } filePath 
     * @param { string } encoding 
     */
    writeIni(filePath, encoding = undefined, withBom = undefined) {
        encoding = encoding === undefined ? this.m_encoding : encoding;
        withBom = withBom === undefined ? this.m_withBom : withBom;
    
        let bom = '';
        switch (encoding) {
            case 'utf16le':
                bom = '\ufeff';
                break;
        
            default:
                break;
        }

        // ファイル出力
        fs.writeFileSync(filePath, bom + this.toIniText(this.m_iniObj), { encoding });
    }

    /**
     * 読み込んだデータをjson形式で出力
     * @param { string } filePath 
     * @param { string } encoding 
     */
    writeJson(filePath, encoding = undefined) {
        encoding = encoding === undefined ? 'utf8' : encoding;

        // ファイル出力
        fs.writeFileSync(filePath, JSON.stringify(this.m_iniObj, null, 2), { encoding });
    }

   /**
    * ini形式のテキストを解析してobjectに変換
    * @param { string } iniText 
    * @param { string } eol 
    * @returns { object }
    */
    toObj(iniText, eol = undefined) {
        eol = eol === undefined ? this.m_eol : eol;
        
        // object生成
        const commentChars = ['#', ';']
        const lines = iniText.split(eol);
        let section = '';
        let foundPos = -1;
        return lines.reduce((accum, line) => {
            line = line.trim();

            // 空行除外
            if (line === '') return accum;
            // コメント除外
            if (commentChars.includes(line[0])) return accum;

            // セクション
            if (line[0] === '[') {
                section = [...line.matchAll(/\[(.*)\]/g)][0][1]
                accum[section] = {};
            // キー値
            } else {
                foundPos = line.indexOf('=');
                if (foundPos !== -1) {
                    accum[section][line.slice(0, foundPos)] = line.slice(foundPos + 1);
                }
            }

            return accum;
        }, {})
    }

   /**
    * objectを解析してini形式のテキストに変換
    * @param { object } obj 
    * @param { string } eol 
    * @returns { string }
    */
    toIniText(obj, eol = undefined) {
        eol = eol === undefined ? this.m_eol : eol;

        // iniText生成
        return Object.entries(this.m_iniObj).reduce((acuum, sectionKeyVals, index) => {
            const [section, keyVals] = sectionKeyVals;
            if (index !== 0) {
                acuum += `${eol}${eol}`;
            }

            // セクション
            acuum += `[${section}]`;

            // キー値
            return acuum += Object.entries(keyVals).reduce((_accum, keyVal, index) => {
            const [key, val] = keyVal;
                return _accum += `${eol}${key}=${val}`;
            }, '');
        }, '');
    }
}