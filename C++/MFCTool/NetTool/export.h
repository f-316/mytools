#pragma once

#define DLLExport __declspec(dllexport)

// �֐�Export�\��
// extern "C" __declspec(dllexport) RETURN_TYPE FUNC_NAME(TYPE1 arg1, TYPE2 arg2, ...)
// __declspec(dllexport) // ���ꂾ������GetProcAddress()�����s����B
// Import��
// typedef RETURN_TYPE (FUNC_NAME)(TYPE1 arg1, TYPE2 arg2, ...);
// FUNC_NAME* func = (FUNC_NAME*)::GetProcAddress(hModule, "FUNC_NAME")
#define DLLFuncExport extern "C" __declspec(dllexport)

// �N���XExport�\��
//extern "C" class __declspec(dllexport) CLASS_NAME
#define DLLClassExport extern "C" class __declspec(dllexport)

// �N���X���\�b�hExport�\��
// class CLASS_NAME
// {
//	__declspec(dllexport) RETURN_TYPE FUNC_NAME(TYPE1 arg1, TYPE2 arg2, ...);
// }
// Import��
// typedef RETURN_TYPE (CALLBACK FUNC_NAME)(TYPE1 arg1, TYPE2 arg2, ...);
// .dll�̒����烁�\�b�h���̃e�L�X�g���R�s�[����
// FUNC_NAME* func = (FUNC_NAME*)::GetProcAddress(hModule, "?FUNC_NAME@CLASS_NAME@@QAEHH@Z")
