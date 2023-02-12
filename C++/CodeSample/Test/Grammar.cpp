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
	//char* pStr = "test"; // �����񃊃e�����͐ݒ�ł��Ȃ�
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
	// �����������ӏ������l���������c���0
	// [0]0x00EFF5C4,2
	// [1]0x00EFF5C5,1
	// [2]0x00EFF5C6,0
	// [3]0x00EFF5C7,0

	char pStrArray[4];
	for (int i = 0; i < 4; i++)
	{
		printf("[%d]0x%08X,%d\n", i, &pStrArray[i], pStrArray[i]);
	}
	// ���������Ă��Ȃ��̂ŕs��
	// [0]0x00EFF5AC,-52
	// [1]0x00EFF5AD,-52
	// [2]0x00EFF5AE,-52
	// [3]0x00EFF5AF,-52
}

void Grammar::PointerRef(void)
{
	int a = 10;
	int d = 20;
	int* pB = &a; // �ϐ�a�̃A�h���X���|�C���^�ϐ��Ɋi�[
	//int*& pC = &a; // pC�̓|�C���^�ϐ��̎Q�ƂȂ̂Ń|�C���^�ϐ��łȂ��Ə������s�A�A�h���X�͓�����Ȃ��B
	int*& pC = pB; // pB�Ƃ����|�C���^�̎Q�Ƃ��Ƃ�
	pC = &d; // d�̃A�h���X���|�C���^�ϐ��Ɋi�[�ApC��pB�̎Q�ƂȂ̂�pB���ς��B
	printf("%p,%d\n", &a, a);    // -> 009BF9F4,10
	printf("%p,%d\n", pB, *pB);	 // -> 009BF9E8,20
	printf("%p,%d\n", pC, *pC);	 // -> 009BF9E8,20
	printf("%p,%d\n", &d, d);	 // -> 009BF9E8,20

	//const int*& pE = pB; // �r���h�G���[
	// ����
	// const int x = 1;
	// int& y = x; �����悤�Ƃ��Ă���̂ƈꏏ

	const int* pE = &a;
	const int* pF = &d;
	const int*& pG = pE;
	a++;
	printf("%p,%d\n", pG, *pG);
	pG = pF; // const�͒��g�̕ύX���Œ肷��̂Ń|�C���^�͕ύX�\
	//*pG = 1; // �l�̕ύX�͕s��
	printf("%p,%d\n", pG, *pG);

	Test cTest1;
	Test* p_cTest2 = &cTest1;
	Test*& p_cTest3 = p_cTest2;
	// const Test*& p_cTest4 = p_cTest2; // �r���h�G���[
	// ����
	// const int x = 1;
	// int& y = x; �����悤�Ƃ��Ă���̂ƈꏏ
	const Test cTest5;
	const Test* p_cTest6 = &cTest5;
	const Test*& p_cTest7 = p_cTest6;

	printf("%d\n", p_cTest3->GetWithChange()); // return �����o++;
	printf("%d\n", p_cTest3->GetWithoutChange()); // return �����o;
	// printf("%d\n", p_cTest7->GetWithChange()); // �r���h�G���[
	printf("%d\n", p_cTest7->GetWithoutChange());

}
