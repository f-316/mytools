#pragma once

#define DLLExport __declspec(dllexport)

// 関数Export構文
// extern "C" __declspec(dllexport) RETURN_TYPE FUNC_NAME(TYPE1 arg1, TYPE2 arg2, ...)
// __declspec(dllexport) // これだけだとGetProcAddress()が失敗する。
// Import側
// typedef RETURN_TYPE (FUNC_NAME)(TYPE1 arg1, TYPE2 arg2, ...);
// FUNC_NAME* func = (FUNC_NAME*)::GetProcAddress(hModule, "FUNC_NAME")
#define DLLFuncExport extern "C" __declspec(dllexport)

// クラスExport構文
//extern "C" class __declspec(dllexport) CLASS_NAME
#define DLLClassExport extern "C" class __declspec(dllexport)

// クラスメソッドExport構文
// class CLASS_NAME
// {
//	__declspec(dllexport) RETURN_TYPE FUNC_NAME(TYPE1 arg1, TYPE2 arg2, ...);
// }
// Import側
// typedef RETURN_TYPE (CALLBACK FUNC_NAME)(TYPE1 arg1, TYPE2 arg2, ...);
// .dllの中からメソッド名のテキストをコピーする
// FUNC_NAME* func = (FUNC_NAME*)::GetProcAddress(hModule, "?FUNC_NAME@CLASS_NAME@@QAEHH@Z")
