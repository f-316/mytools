import { Lib2 } from "../Lib2";
import { Lib3 } from "../Lib3";
import { Lib1_1 } from "./Lib1_1";

export class Lib1 {
    constructor() {
        
    }

    test () {
        const calc = new Lib2();
        const sum1 = calc.sum({a: 2, b: 4});
        const sum2 = calc.sum();
        const sort = new Lib3();
        const arr = sort.addNumber1([sum1, sum2]);

        const lib1_1 = new Lib1_1();
        const html = lib1_1.makeHtml(['Hello!World!', ...arr]);
        const temp = document.createElement('div');
        temp.innerHTML = html;
        [...temp.children].forEach(element => {
            document.body.append(element)
        });
    }
}
