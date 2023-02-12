#include "Grammar.h"

Grammar::Grammar()
{
}

Grammar::~Grammar()
{
}

void Grammar::Sample(void)
{
	Pointer();
	PointerRef();
}

void Grammar::Pointer(void)
{
	//char* pStr = "test"; // 文字列リテラルは設定できない
	char pStrArr[4] = "abc";
	char* pStr = pStrArr;
	printf("0x%08X,%s\n", pStr, pStr); // -> 0x00BBF61C,abc
	printf("0x%08X,%s\n", pStrArr, pStrArr); // -> 0x00BBF61C, abc
	pStr[1] = 'o';
	printf("0x%08X,%s\n", pStr, pStr); // -> 0x00BBF61C,aoc
	printf("0x%08X,%s\n", pStrArr, pStrArr); // -> 0x00BBF61C,aoc

	const char* c_pStr = "test";
	for (int i = 0; i < 4; i++)
	{
		printf("[%d]0x%08X,%d\n", i, &c_pStr[i], c_pStr[i]);
	}
	// [0]0x00F37E50,116
	// [1]0x00F37E51,101
	// [2]0x00F37E52,115
	// [3]0x00F37E53,116

	const char c_pStrArray[4] = { 2, 1 };
	for (int i = 0; i < 4; i++)
	{
		printf("[%d]0x%08X,%d\n", i, &c_pStrArray[i], c_pStrArray[i]);
	}
	// 初期化した箇所だけ値が代入され残りは0
	// [0]0x00EFF5C4,2
	// [1]0x00EFF5C5,1
	// [2]0x00EFF5C6,0
	// [3]0x00EFF5C7,0

	char pStrArray[4];
	for (int i = 0; i < 4; i++)
	{
		printf("[%d]0x%08X,%d\n", i, &pStrArray[i], pStrArray[i]);
	}
	// 初期化していないので不定
	// [0]0x00EFF5AC,-52
	// [1]0x00EFF5AD,-52
	// [2]0x00EFF5AE,-52
	// [3]0x00EFF5AF,-52
}

void Grammar::PointerRef(void)
{
	int a = 10;
	int d = 20;
	int* pB = &a; // 変数aのアドレスをポインタ変数に格納
	//int*& pC = &a; // pCはポインタ変数の参照なのでポインタ変数でないと初期化不可、アドレスは入れられない。
	int*& pC = pB; // pBというポインタの参照をとる
	pC = &d; // dのアドレスをポインタ変数に格納、pCはpBの参照なのでpBも変わる。
	printf("%p,%d\n", &a, a);    // -> 009BF9F4,10
	printf("%p,%d\n", pB, *pB);	 // -> 009BF9E8,20
	printf("%p,%d\n", pC, *pC);	 // -> 009BF9E8,20
	printf("%p,%d\n", &d, d);	 // -> 009BF9E8,20

	//const int*& pE = pB; // ビルドエラー
	// ↑は
	// const int x = 1;
	// int& y = x; をしようとしているのと一緒

	const int* pE = &a;
	const int* pF = &d;
	const int*& pG = pE;
	a++;
	printf("%p,%d\n", pG, *pG);
	pG = pF; // constは中身の変更を固定するのでポインタは変更可能
	//*pG = 1; // 値の変更は不可
	printf("%p,%d\n", pG, *pG);

	Test cTest1;
	Test* p_cTest2 = &cTest1;
	Test*& p_cTest3 = p_cTest2;
	// const Test*& p_cTest4 = p_cTest2; // ビルドエラー
	// ↑は
	// const int x = 1;
	// int& y = x; をしようとしているのと一緒
	const Test cTest5;
	const Test* p_cTest6 = &cTest5;
	const Test*& p_cTest7 = p_cTest6;

	printf("%d\n", p_cTest3->GetWithChange()); // return メンバ++;
	printf("%d\n", p_cTest3->GetWithoutChange()); // return メンバ;
	// printf("%d\n", p_cTest7->GetWithChange()); // ビルドエラー
	printf("%d\n", p_cTest7->GetWithoutChange());

}
