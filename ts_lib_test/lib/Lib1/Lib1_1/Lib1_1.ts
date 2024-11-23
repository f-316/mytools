export class Lib1_1 {
    constructor() {
        
    }

    makeHtml (arr: any[]) {
        return arr.reduce((accum, elem) => {
            return accum + `<div>${elem}</div>`
        }, '')
    }
}
