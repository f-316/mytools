/**
 * sort object by key
 * @param { object } jsonObj
 * @returns { object }
 */
export function sortJsonObj(jsonObj) {
    if (typeof jsonObj !== 'object' ) {
        return jsonObj;
    }

    let obj = {};
    Object.keys(jsonObj).sort().forEach(key => {
        obj[key] = sortJsonObj(jsonObj[key]);
    })
    return obj;
}