#pragma once
#ifndef __AppLog__
#define __AppLog__

#include <string>
#include <Shlwapi.h>
#include "stdarg.h"

//ファイル名,行数マクロ
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define TRACEARGS __FILENAME__, __LINE__
#define FUNCTRACEARGS __FILENAME__, __LINE__, __func__

enum {
	Max_FormatBufferSize = 1024,
};

class __declspec(dllexport) CAppLog
{
public:
	CAppLog();
	~CAppLog();
	static void Trace(const char* pFileName, int line, const char* pFuncName, const char* pStrFmt, ...);
	static void Debug(const char* pFileName, int line, const char* pFuncName, const char* pStrFmt, ...);
	static void Trace(const char* pFileName, int line, const char* pStrFmt, ...);
	static void Debug(const char* pFileName, int line, const char* pStrFmt, ...);

private:
	static void Print(const char* pStrFmt, va_list args);
};

class __declspec(dllexport) CFuncTrace
{
public:
	CFuncTrace(const char * pFileName, int line, const char* pFuncName, const char * pStrFmt, ...);
	CFuncTrace(const char * pFileName, int line, const char * pStrFmt, ...);
	~CFuncTrace();
	void Trace(int line, const char* pStrFmt, ...);
	void Debug(int line, const char* pStrFmt, ...);

protected:
	char m_pFileName[MAX_PATH];
	int m_startLine;
	char m_pFuncName[MAX_PATH];
	char m_pFuncInfo[Max_FormatBufferSize];
};

//class LogFile
//{
//public:
//	LogFile(void);
//	virtual ~LogFile(void);
//	static LogFile* GetInstance(void);
//
//	BOOL Locate(const char* pFilePath);
//	char* Locate(void);
//
//	BOOL Write(const char* pFileName, int line, const char* pStrFmt, ...);
//	BOOL Put(const char* pStrFmt, ...);
//
//	//CStringA GetFilePath(void);
//	int GetLength(void);
//	//CTime LastModified(void);
//
//protected:
//
//	//LogFile* g_pLogFile;
//	//CStdioFile m_logFile;
//	char m_pLogFilePath[MAX_PATH];
//
//private:
//};

#endif // !__AppLog__
