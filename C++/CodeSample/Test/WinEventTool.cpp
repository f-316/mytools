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

	ReadLog(hEventLog);
	ReadLog(hEventLog);

	// イベントログハンドル閉じる
	if (hEventLog != NULL)
	{
		CloseEventLog(hEventLog);
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

/* メッセージの表示 */
BOOL WinEventTool::DispMessage(const char* pSourceName, const char* pEntryName, const char** pArgs, DWORD MessageId)
{
	BOOL bResult;
	BOOL bReturn = FALSE;
	HMODULE hSourceModule = NULL;
	char SourceModuleName[1000];
	char* pMessage = NULL;

	/* ソースモジュール名を取得 */
	bResult = GetModuleNameFromSourceName(pSourceName, pEntryName, SourceModuleName);
	if (!bResult) goto Exit;

	/* ソースモジュールをロード */
	hSourceModule = LoadLibraryExA(
		SourceModuleName,
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
	if (pMessage != NULL) printf("%s", pMessage);
	else printf("(%d)\n", MessageId);

	if (hSourceModule != NULL) FreeLibrary(hSourceModule);
	if (pMessage != NULL) LocalFree(pMessage);

	return bReturn;
}

/* イベントログの読み取り */
void WinEventTool::ReadLog(HANDLE hEventLog)
{
	DWORD BufSize;
	DWORD ReadBytes;
	DWORD NextSize;
	BOOL bResult;
	DWORD i;
	char* cp;
	char* pSourceName;
	char* pComputerName;
	EVENTLOGRECORD* pBuf = NULL;
	const char** Args = NULL;

	//for (;;) {
		/* イベントログのサイズ取得 */
		BufSize = 1;
		pBuf = (EVENTLOGRECORD*)GlobalAlloc(GMEM_FIXED, BufSize);

		bResult = ReadEventLogA(
			hEventLog,
			EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
			0,
			pBuf,
			BufSize,
			&ReadBytes,
			&NextSize);

		if (!bResult && GetLastError() != ERROR_INSUFFICIENT_BUFFER) return;

		GlobalFree(pBuf);
		pBuf = NULL;

		/* バッファ割り当て */
		BufSize = NextSize;
		pBuf = (EVENTLOGRECORD*)GlobalAlloc(GMEM_FIXED, BufSize);

		/* イベントログの読み取り */
		bResult = ReadEventLogA(
			hEventLog,
			EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
			0,
			pBuf,
			BufSize,
			&ReadBytes,
			&NextSize);

		if (!bResult) return;

		/* 読み取ったイベントの表示 */
		printf("レコード番号: %d\n", pBuf->RecordNumber);
		//printf("生成時刻: %s", ctime(&pBuf->TimeGenerated));
		//printf("書き込み時刻: %s", ctime(&pBuf->TimeWritten));
		printf("イベントID: %08x\n", pBuf->EventID);

		printf("イベントの種別: ");
		switch (pBuf->EventType) {
		case EVENTLOG_SUCCESS: printf("成功\n"); break;
		case EVENTLOG_ERROR_TYPE: printf("エラー\n"); break;
		case EVENTLOG_WARNING_TYPE: printf("警告\n"); break;
		case EVENTLOG_INFORMATION_TYPE: printf("情報\n"); break;
		case EVENTLOG_AUDIT_SUCCESS: printf("監査成功\n"); break;
		case EVENTLOG_AUDIT_FAILURE: printf("監査失敗\n"); break;
		default: printf("不明\n"); break;
		}

		cp = (char*)pBuf;
		cp += sizeof(EVENTLOGRECORD);

		pSourceName = cp;
		cp += strlen(cp) + 1;

		pComputerName = cp;
		cp += strlen(cp) + 1;

		printf("ソース名: %s\n", pSourceName);
		printf("コンピュータ名: %s\n", pComputerName);

		/* カテゴリの表示 */
		printf("二次カテゴリ: ", pBuf->EventCategory);
		DispMessage(pSourceName, "CategoryMessageFile", NULL, pBuf->EventCategory);

		/* メッセージの表示 */
		Args = GetArgs(pBuf);

		printf("メッセージ: ");
		DispMessage(pSourceName, "EventMessageFile", Args, pBuf->EventID);

		if (Args != NULL) {
			GlobalFree(Args);
			Args = NULL;
		}

		/* 固有データの表示 */
		if (pBuf->DataLength > 0) {
			printf("固有データ: ");
			for (i = 0; i < pBuf->DataLength; i++) printf("%02x ", *(((unsigned char*)pBuf) + (pBuf->DataOffset) + i));
			printf("\n");
		}

		printf("\n");

		/* バッファ解放 */
		GlobalFree(pBuf);
		pBuf = NULL;
	//}

Exit: /* 後処理 */
	if (pBuf != NULL) GlobalFree(pBuf);
	if (Args != NULL) GlobalFree(Args);
}