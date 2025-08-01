#pragma once
#include <stdio.h>
#include <windows.h>
#include "stdarg.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define FUNCTRACEARGS __FILENAME__, __LINE__, __func__

class AppLog
{
public:
	static void Trace(const char* pFileName, int line, const char* pFuncName, const char* pStrFmt, ...)
    {
        char str[1024];
        va_list args;
        va_start(args, pStrFmt);
        ::vsprintf_s(str, sizeof(str), pStrFmt, args);
        va_end(args);

        WriteLine(pFileName, line, pFuncName, "Trace", "", str);
    }

	static void Error(const char* pFileName, int line, const char* pFuncName, const char* pStrFmt, ...)
    {
        char str[1024];
        va_list args;
        va_start(args, pStrFmt);
        ::vsprintf_s(str, sizeof(str), pStrFmt, args);
        va_end(args);

        WriteLine(pFileName, line, pFuncName, "Error", "", str);
    }

	static void Error(const char* pFileName, int line, const char* pFuncName, DWORD errorCode)
    {
        LPVOID lpMsgBuf;
        DWORD bufLen = ::FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&lpMsgBuf,
            0, NULL);

        char errorStr[20];
        ::sprintf_s(errorStr, sizeof(errorStr), "Err:%lu,", errorCode);
        Write(pFileName, line, pFuncName, "Error", errorStr, (const char*)lpMsgBuf);

        ::LocalFree(lpMsgBuf);
    }

    static void WriteLine(const char* pFileName, int line, const char* pFuncName, const char* pType, const char* pPrefix, const char* pStr)
    {
        ::printf("%s(%d)%s[%s]%s%s\n", pFileName, line, pFuncName, pType, pPrefix, pStr);
    }

    static void Write(const char* pFileName, int line, const char* pFuncName, const char* pType, const char* pPrefix, const char* pStr)
    {
        ::printf("%s(%d)%s[%s]%s%s", pFileName, line, pFuncName, pType, pPrefix, pStr);
    }
};
