#include "WinEventTool.h"

WinEventTool::WinEventTool()
{
}

WinEventTool::~WinEventTool()
{
}

void WinEventTool::Run(void)
{
	// イベントログハンドル取得
	HANDLE hEventLog;
	hEventLog = OpenEventLogA(NULL, "Application");
	if (hEventLog == NULL) {
		printf("event log can not open.\n");
		return;
		//goto Exit;
	}

	EVENTLOGRECORD evtLogRecord = { 0 };
	char pRetMsg[RetMsgBuffSize] = { 0 };
	char pSourceName[StrBuffSize] = { 0 };
	char pComputerName[StrBuffSize] = { 0 };
	char pEventType[8] = { 0 };
	char pDateStr[40] = { 0 };
	if (ReadLog(hEventLog, evtLogRecord, pRetMsg, pSourceName, pComputerName))
	{
		printf("[%d]EventType=%s\n", __LINE__, GetEventType(evtLogRecord.EventType, pEventType, sizeof(pEventType)));
		printf("[%d]生成時刻: %s", __LINE__, GetCTimeStr(evtLogRecord.TimeGenerated, pDateStr, sizeof(pDateStr)));
		printf("[%d]書き込み時刻: %s", __LINE__, GetCTimeStr(evtLogRecord.TimeWritten, pDateStr, sizeof(pDateStr)));
		printf("[%d]RecordNumber=%lu\n", __LINE__, evtLogRecord.RecordNumber);
		printf("[%d]EventID=%lu\n", __LINE__, evtLogRecord.EventID);
		printf("[%d]pSourceName=%s\n", __LINE__, pSourceName);
		printf("[%d]pComputerName=%s\n", __LINE__, pComputerName);
		printf("[%d]%d,%s\n", __LINE__, strlen(pRetMsg), pRetMsg);
	}

	// イベントログハンドル閉じる
	if (hEventLog != NULL)
	{
		CloseEventLog(hEventLog);
	}
}

void WinEventTool::WaitEventMode(void)
{
	// イベントログハンドル取得
	HANDLE hEventLog = NULL;
	HANDLE hEvent = NULL;

	printf("[%d]Wait event...\n", __LINE__);
	int ret = 0;

	while (1)
	{
		// イベントログオープン
		hEventLog = OpenEventLogA(NULL, "Application");
		if (hEventLog == NULL) {
			printf("event log can not open.\n");
			break;
		}

		// 監視イベント作成
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hEvent == INVALID_HANDLE_VALUE)
		{
			printf("[%d]GetLastError=%ld\n", __LINE__, GetLastError());
			break;
		}

		// イベント監視
		if (!NotifyChangeEventLog(hEventLog, hEvent))
		{
			printf("[%d]GetLastError=%ld\n", __LINE__, GetLastError());
			break;
		}

		// イベント待ち
		ret = WaitForSingleObject(hEvent, INFINITE);
		if (ret == WAIT_OBJECT_0)
		{
			EVENTLOGRECORD evtLogRecord = { 0 };
			char pRetMsg[RetMsgBuffSize] = { 0 };
			char pSourceName[StrBuffSize] = { 0 };
			char pComputerName[StrBuffSize] = { 0 };
			char pEventType[8] = { 0 };
			char pDateStr[40] = { 0 };
			if (ReadLog(hEventLog, evtLogRecord, pRetMsg, pSourceName, pComputerName))
			{
				printf("[%d]EventType=%s\n", __LINE__, GetEventType(evtLogRecord.EventType, pEventType, sizeof(pEventType)));
				printf("[%d]生成時刻: %s", __LINE__, GetCTimeStr(evtLogRecord.TimeGenerated, pDateStr, sizeof(pDateStr)));
				printf("[%d]書き込み時刻: %s", __LINE__, GetCTimeStr(evtLogRecord.TimeWritten, pDateStr, sizeof(pDateStr)));
				printf("[%d]RecordNumber=%lu\n", __LINE__, evtLogRecord.RecordNumber);
				printf("[%d]EventID=%lu\n", __LINE__, evtLogRecord.EventID);
				printf("[%d]pSourceName=%s\n", __LINE__, pSourceName);
				printf("[%d]pComputerName=%s\n", __LINE__, pComputerName);
				printf("[%d]%d,%s\n", __LINE__, strlen(pRetMsg), pRetMsg);
			}
		}
		else
		{
			printf("[%d]GetLastError=%ld\n", __LINE__, GetLastError());
		}

		// イベントハンドル閉じる
		if (hEvent != NULL)
		{
			CloseHandle(hEvent);
			hEvent = NULL;
		}
		// イベントログハンドル閉じる
		if (hEventLog != NULL)
		{
			CloseEventLog(hEventLog);
			hEventLog = NULL;
		}
	}

	// イベントハンドル閉じる
	if (hEvent != NULL)
	{
		CloseHandle(hEvent);
		hEvent = NULL;
	}
	// イベントログハンドル閉じる
	if (hEventLog != NULL)
	{
		CloseEventLog(hEventLog);
		hEventLog = NULL;
	}
}

/* 埋込み文字列の取得 */
const char** WinEventTool::GetArgs(const EVENTLOGRECORD* pBuf)
{
	char* cp;
	WORD ArgCount;
	const char** Args = NULL;

	if (pBuf->NumStrings == 0) return NULL;

	/* 引数リストを取得 */
	Args = (const char**)GlobalAlloc(GMEM_FIXED, sizeof(char*) * pBuf->NumStrings);
	cp = (char*)pBuf + (pBuf->StringOffset);

	for (ArgCount = 0; ArgCount < pBuf->NumStrings; ArgCount++) {
		Args[ArgCount] = cp;
		cp += strlen(cp) + 1;
	}
	return Args;
}

char* WinEventTool::GetEventType(WORD eventType, char* pEventType, int bufSize)
{
	switch (eventType) {
	case EVENTLOG_SUCCESS: ::strcpy_s(pEventType, bufSize, "成功"); break;
	case EVENTLOG_ERROR_TYPE: ::strcpy_s(pEventType, bufSize, "エラー"); break;
	case EVENTLOG_WARNING_TYPE: ::strcpy_s(pEventType, bufSize, "警告"); break;
	case EVENTLOG_INFORMATION_TYPE: ::strcpy_s(pEventType, bufSize, "情報"); break;
	case EVENTLOG_AUDIT_SUCCESS: ::strcpy_s(pEventType, bufSize, "監査成功"); break;
	case EVENTLOG_AUDIT_FAILURE: ::strcpy_s(pEventType, bufSize, "監査失敗"); break;
	default: ::strcpy_s(pEventType, bufSize, "不明"); break;
	}

	return pEventType;
}

char* WinEventTool::GetCTimeStr(time_t ctime, char* pDateStr, int bufSize)
{
	ctime_s(pDateStr, bufSize, &ctime);
	return pDateStr;
}

/* ソース名からモジュール名を取得 */
BOOL WinEventTool::GetModuleNameFromSourceName(const char* pSourceName, const char* pEntryName, char* pExpandedName /* 1000バイトのバッファ */)
{
	DWORD lResult;
	DWORD ModuleNameSize;
	char pModuleName[1000];
	HKEY hAppKey = NULL;
	HKEY hSourceKey = NULL;
	BOOL bReturn = FALSE;

	/* Applicationログ用のレジストリキーをオープン */
	lResult = RegOpenKeyExA(
		HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application",
		0,
		KEY_READ,
		&hAppKey);

	if (lResult != ERROR_SUCCESS) {
		printf("registry can not open.\n");
		goto Exit;
	}

	/* ソースの情報が格納されているレジストリをオープン */
	lResult = RegOpenKeyExA(
		hAppKey,
		pSourceName,
		0,
		KEY_READ,
		&hSourceKey);

	if (lResult != ERROR_SUCCESS) goto Exit;

	ModuleNameSize = 1000;

	/* ソースモジュール名を取得 */
	lResult = RegQueryValueExA(
		hSourceKey,
		"EventMessageFile",
		NULL,
		NULL,
		(BYTE*)pModuleName,
		&ModuleNameSize);

	if (lResult != ERROR_SUCCESS) goto Exit;

	/* 環境変数を展開 */
	ExpandEnvironmentStringsA(pModuleName, pExpandedName, 1000);

	/* 正常終了 */
	bReturn = TRUE;

Exit: /* 後処理 */
	if (hSourceKey != NULL) RegCloseKey(hSourceKey);
	if (hAppKey != NULL) RegCloseKey(hAppKey);

	return bReturn;
}

/* メッセージの抽出 */
BOOL WinEventTool::ExtractMessage(const char* pSourceName, const char* pEntryName, const char** pArgs, DWORD MessageId, char* pRetMsg)
{
	BOOL bResult;
	BOOL bReturn = FALSE;
	HMODULE hSourceModule = NULL;
	char pSourceModuleName[1000];
	char* pMessage = NULL;

	/* ソースモジュール名を取得 */
	bResult = GetModuleNameFromSourceName(pSourceName, pEntryName, pSourceModuleName);
	if (!bResult) goto Exit;

	/* ソースモジュールをロード */
	hSourceModule = LoadLibraryExA(
		pSourceModuleName,
		NULL,
		DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

	if (hSourceModule == NULL) goto Exit;

	/* メッセージを作成 */
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		hSourceModule,
		MessageId,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&pMessage,
		0,
		(va_list*)pArgs);

	/* 正常終了 */
	bReturn = TRUE;

Exit: /* 後処理 */
	if (pMessage != NULL)
	{
		::strcpy_s(pRetMsg, RetMsgBuffSize, pMessage);
	}
	//else
	//{
	//	printf("(%d)\n", MessageId);
	//}

	if (hSourceModule != NULL) FreeLibrary(hSourceModule);
	if (pMessage != NULL) LocalFree(pMessage);

	return bReturn;
}

/* イベントログの読み取り */
BOOL WinEventTool::ReadLog(HANDLE hEventLog, EVENTLOGRECORD& evtLogRecord, char* pRetMsg, char* pSourceName, char* pComputerName)
{
	DWORD bufSize;
	DWORD readBytes;
	DWORD nextSize;
	BOOL bResult;
	EVENTLOGRECORD* pBuf = NULL;

	/* イベントログのサイズ取得 */
	bufSize = 1;
	pBuf = (EVENTLOGRECORD*)GlobalAlloc(GMEM_FIXED, bufSize);

	DWORD dwReadFlags = EVENTLOG_SEEK_READ; // EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ
	dwReadFlags = EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ;

	bResult = ReadEventLogA(
		hEventLog,
		dwReadFlags,
		0,
		pBuf,
		bufSize,
		&readBytes,
		&nextSize);

	if (!bResult && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		return FALSE;
	}

	GlobalFree(pBuf);
	pBuf = NULL;

	/* バッファ割り当て */
	bufSize = nextSize;
	pBuf = (EVENTLOGRECORD*)GlobalAlloc(GMEM_FIXED, bufSize);

	/* イベントログの読み取り */
	bResult = ReadEventLogA(
		hEventLog,
		dwReadFlags,
		0,
		pBuf,
		bufSize,
		&readBytes,
		&nextSize);

	if (!bResult)
	{
		return FALSE;
	}

	::memcpy_s(&evtLogRecord, sizeof(evtLogRecord), pBuf, sizeof(evtLogRecord));

	// EVENTLOGRECORD以降のデータ解析
	DWORD i;
	char* cp;

	cp = (char*)pBuf;
	cp += sizeof(EVENTLOGRECORD);

	// ソース名
	::strcpy_s(pSourceName, 256, cp);
	cp += strlen(cp) + 1;

	// コンピュータ名
	::strcpy_s(pComputerName, 256, cp);
	cp += strlen(cp) + 1;

	/* メッセージの抽出 */
	const char** pArgs = NULL;
	pArgs = GetArgs(pBuf);
	ExtractMessage(pSourceName, "EventMessageFile", pArgs, pBuf->EventID, pRetMsg);
	if (pArgs != NULL) {
		GlobalFree(pArgs);
		pArgs = NULL;
	}

	/* バッファ解放 */
	GlobalFree(pBuf);
	pBuf = NULL;

Exit: /* 後処理 */
	if (pBuf != NULL) GlobalFree(pBuf);
	if (pArgs != NULL) GlobalFree(pArgs);

	return TRUE;
}