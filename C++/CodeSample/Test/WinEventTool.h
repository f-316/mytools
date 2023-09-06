#pragma once
#include "windows.h"
#include "stdio.h"
#include <ctime>

class WinEventTool
{
public:
	WinEventTool();
	~WinEventTool();

	void WaitEventMode(void);
	void Run(void);

protected:
	enum Const
	{
		RetMsgBuffSize = 0x7FFF,
		StrBuffSize = 256,
	};
	const char** GetArgs(const EVENTLOGRECORD* pBuf);
	char* GetEventType(WORD eventType, char* pEventType, int bufSize);
	char* GetCTimeStr(time_t ctime, char* pDateStr, int bufSize);
	BOOL GetModuleNameFromSourceName(const char* pSourceName, const char* pEntryName, char* pExpandedName);
	BOOL ExtractMessage(const char* pSourceName, const char* pEntryName, const char** pArgs, DWORD MessageId, char* pRetMsg);
	BOOL ReadLog(HANDLE hEventLog, EVENTLOGRECORD& evtLogRecord, char* pRetMsg, char* pSourceName, char* pComputerName);
};
