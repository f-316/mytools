#pragma once
#include "export.h"
//extern "C" DLLExport int __stdcall Multiply(int a, int b);
DLLExport int __stdcall Multiply(int a, int b);

class CHoge
{
public:
	CHoge();
	~CHoge();
	int Sum(int a, int b);
	__declspec(dllexport) int Double(int a);
	//DLLFuncExport int Double(int a);
	//__declspec(dllexport) int Double(int a);

private:

};
