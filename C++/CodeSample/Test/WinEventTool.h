#pragma once
#include "windows.h"
#include "stdio.h"
#include <ctime>

class WinEventTool
{
public:
	WinEventTool();
	~WinEventTool();

	void Run(void);

protected:
	const char** GetArgs(const EVENTLOGRECORD* pBuf);
	BOOL GetModuleNameFromSourceName(const char* pSourceName, const char* pEntryName, char* pExpandedName);
	BOOL DispMessage(const char* pSourceName, const char* pEntryName, const char** pArgs, DWORD MessageId);
	void ReadLog(HANDLE hEventLog);
};
