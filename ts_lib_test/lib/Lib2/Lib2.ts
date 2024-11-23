import { DEFAULT_OBJ } from "./const";
import { Nums } from "./types";

export class Lib2 {
    constructor() {
        
    }

    sum (nums: Nums | undefined = undefined) {
        if (!nums) {
            return DEFAULT_OBJ.a + DEFAULT_OBJ.b;
        }
        
        return nums.a + nums.b;
    }
}
