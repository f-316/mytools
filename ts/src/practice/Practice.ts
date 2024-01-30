export class Practice {
    start() {
        console.log(`${Practice.name} Enter`)
    
        // this.fizzBuzz();
        // this.indexSignatureTest();
        // this.readonlyTest();
        // this.typeTest();
        this.arrayTest();
        // this.restParameterTest();

        console.log(`${Practice.name} Leave`)
    }

    restParameterTest() {
        const sum = (...args: number[]) => {
            return args.reduce((accumlator, val) => {
                return accumlator + val
            }, 0)
        }
        const nums = [1,2,3,4,5,6,7]
        console.log('sum', sum(1, 2, 3, 4))
        console.log('sum(...nums)', sum(...nums))
    }

    arrayTest() {
        const arr = [1, 2, 3]
        const notTpl = [true, '2', 3]
        const tpl: [boolean, string, number] = [true, '2', 3]
        notTpl[0] = 1; // OK
        // tpl[0] = 1; // NG
        tpl.push(4); // OK
        console.log('arr', arr)
        console.log('notTpl', notTpl)
        console.log('tpl', tpl)

        const [first, second, ...rest] = tpl;
        console.log(first, second, rest);
    }

    typeTest() {
        const a = 1;
        type T = typeof a;
        // const b: T = 'b'; // stringなのでエラー
        const b: T = 1;

        const obj = {a: 1, b: 'b'}
        type U = typeof obj;
        // const obj2: U = {a: 1, b: 2}; // bはstring型なのでエラー
        const obj2: U = {a: 10, b: 'c'}; // bはstring型なのでエラー
        console.log(b)
        console.log(obj2)

        // 右のtypeofは演算子なので注意
        const res: typeof obj.b = typeof b;
        console.log('res:', res);
    }

    readonlyTest() {
        type ReadOnly = {
            readonly a: number
        };
        const obj: ReadOnly = {a: 1}; // 初期化はOK
        // obj.a = 2; // NG
        console.log(obj)
    }

    indexSignatureTest() {
        // インデックスシグネチャ
        type IndexSignature = {
            [key: string]: number;
        }

        const typedObj: IndexSignature = {
            a: 1,
            b: 2,
        }

        const obj = {
            a: 1,
            b: 2,
        }

        typedObj.c = 3;
        typedObj['d'] = 4;

        // obj.c = 3; // NG
        // obj['d'] = 4; // NG
        console.log('obj:%o', obj)
        console.log('typedObj:%o', typedObj)

        const num: number = typedObj.e;
        console.log(num) // -> undefined 型安全にならない
    }

    fizzBuzz() {
        for (let num = 1; num <= 100; num++) {
            if (num % 3 !== 0 && num % 5 !== 0) {
                console.log(num);
            } else if (num % 3 === 0 && num % 5 !== 0) {
                console.log('Fizz');
            } else if (num % 3 !== 0 && num % 5 === 0) {
                console.log('Buzz');
            } else {
                console.log('FizzBuzz');
            }
        }
    }
}