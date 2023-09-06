#include "WinEventTool.h"

WinEventTool::WinEventTool()
{
}

WinEventTool::~WinEventTool()
{
}

void WinEventTool::Run(void)
{
	// �C�x���g���O�n���h���擾
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
		printf("[%d]��������: %s", __LINE__, GetCTimeStr(evtLogRecord.TimeGenerated, pDateStr, sizeof(pDateStr)));
		printf("[%d]�������ݎ���: %s", __LINE__, GetCTimeStr(evtLogRecord.TimeWritten, pDateStr, sizeof(pDateStr)));
		printf("[%d]RecordNumber=%lu\n", __LINE__, evtLogRecord.RecordNumber);
		printf("[%d]EventID=%lu\n", __LINE__, evtLogRecord.EventID);
		printf("[%d]pSourceName=%s\n", __LINE__, pSourceName);
		printf("[%d]pComputerName=%s\n", __LINE__, pComputerName);
		printf("[%d]%d,%s\n", __LINE__, strlen(pRetMsg), pRetMsg);
	}

	// �C�x���g���O�n���h������
	if (hEventLog != NULL)
	{
		CloseEventLog(hEventLog);
	}
}

void WinEventTool::WaitEventMode(void)
{
	// �C�x���g���O�n���h���擾
	HANDLE hEventLog = NULL;
	HANDLE hEvent = NULL;

	printf("[%d]Wait event...\n", __LINE__);
	int ret = 0;

	while (1)
	{
		// �C�x���g���O�I�[�v��
		hEventLog = OpenEventLogA(NULL, "Application");
		if (hEventLog == NULL) {
			printf("event log can not open.\n");
			break;
		}

		// �Ď��C�x���g�쐬
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hEvent == INVALID_HANDLE_VALUE)
		{
			printf("[%d]GetLastError=%ld\n", __LINE__, GetLastError());
			break;
		}

		// �C�x���g�Ď�
		if (!NotifyChangeEventLog(hEventLog, hEvent))
		{
			printf("[%d]GetLastError=%ld\n", __LINE__, GetLastError());
			break;
		}

		// �C�x���g�҂�
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
				printf("[%d]��������: %s", __LINE__, GetCTimeStr(evtLogRecord.TimeGenerated, pDateStr, sizeof(pDateStr)));
				printf("[%d]�������ݎ���: %s", __LINE__, GetCTimeStr(evtLogRecord.TimeWritten, pDateStr, sizeof(pDateStr)));
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

		// �C�x���g�n���h������
		if (hEvent != NULL)
		{
			CloseHandle(hEvent);
			hEvent = NULL;
		}
		// �C�x���g���O�n���h������
		if (hEventLog != NULL)
		{
			CloseEventLog(hEventLog);
			hEventLog = NULL;
		}
	}

	// �C�x���g�n���h������
	if (hEvent != NULL)
	{
		CloseHandle(hEvent);
		hEvent = NULL;
	}
	// �C�x���g���O�n���h������
	if (hEventLog != NULL)
	{
		CloseEventLog(hEventLog);
		hEventLog = NULL;
	}
}

/* �����ݕ�����̎擾 */
const char** WinEventTool::GetArgs(const EVENTLOGRECORD* pBuf)
{
	char* cp;
	WORD ArgCount;
	const char** Args = NULL;

	if (pBuf->NumStrings == 0) return NULL;

	/* �������X�g���擾 */
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
	case EVENTLOG_SUCCESS: ::strcpy_s(pEventType, bufSize, "����"); break;
	case EVENTLOG_ERROR_TYPE: ::strcpy_s(pEventType, bufSize, "�G���["); break;
	case EVENTLOG_WARNING_TYPE: ::strcpy_s(pEventType, bufSize, "�x��"); break;
	case EVENTLOG_INFORMATION_TYPE: ::strcpy_s(pEventType, bufSize, "���"); break;
	case EVENTLOG_AUDIT_SUCCESS: ::strcpy_s(pEventType, bufSize, "�č�����"); break;
	case EVENTLOG_AUDIT_FAILURE: ::strcpy_s(pEventType, bufSize, "�č����s"); break;
	default: ::strcpy_s(pEventType, bufSize, "�s��"); break;
	}

	return pEventType;
}

char* WinEventTool::GetCTimeStr(time_t ctime, char* pDateStr, int bufSize)
{
	ctime_s(pDateStr, bufSize, &ctime);
	return pDateStr;
}

/* �\�[�X�����烂�W���[�������擾 */
BOOL WinEventTool::GetModuleNameFromSourceName(const char* pSourceName, const char* pEntryName, char* pExpandedName /* 1000�o�C�g�̃o�b�t�@ */)
{
	DWORD lResult;
	DWORD ModuleNameSize;
	char pModuleName[1000];
	HKEY hAppKey = NULL;
	HKEY hSourceKey = NULL;
	BOOL bReturn = FALSE;

	/* Application���O�p�̃��W�X�g���L�[���I�[�v�� */
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

	/* �\�[�X�̏�񂪊i�[����Ă��郌�W�X�g�����I�[�v�� */
	lResult = RegOpenKeyExA(
		hAppKey,
		pSourceName,
		0,
		KEY_READ,
		&hSourceKey);

	if (lResult != ERROR_SUCCESS) goto Exit;

	ModuleNameSize = 1000;

	/* �\�[�X���W���[�������擾 */
	lResult = RegQueryValueExA(
		hSourceKey,
		"EventMessageFile",
		NULL,
		NULL,
		(BYTE*)pModuleName,
		&ModuleNameSize);

	if (lResult != ERROR_SUCCESS) goto Exit;

	/* ���ϐ���W�J */
	ExpandEnvironmentStringsA(pModuleName, pExpandedName, 1000);

	/* ����I�� */
	bReturn = TRUE;

Exit: /* �㏈�� */
	if (hSourceKey != NULL) RegCloseKey(hSourceKey);
	if (hAppKey != NULL) RegCloseKey(hAppKey);

	return bReturn;
}

/* ���b�Z�[�W�̒��o */
BOOL WinEventTool::ExtractMessage(const char* pSourceName, const char* pEntryName, const char** pArgs, DWORD MessageId, char* pRetMsg)
{
	BOOL bResult;
	BOOL bReturn = FALSE;
	HMODULE hSourceModule = NULL;
	char pSourceModuleName[1000];
	char* pMessage = NULL;

	/* �\�[�X���W���[�������擾 */
	bResult = GetModuleNameFromSourceName(pSourceName, pEntryName, pSourceModuleName);
	if (!bResult) goto Exit;

	/* �\�[�X���W���[�������[�h */
	hSourceModule = LoadLibraryExA(
		pSourceModuleName,
		NULL,
		DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

	if (hSourceModule == NULL) goto Exit;

	/* ���b�Z�[�W���쐬 */
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		hSourceModule,
		MessageId,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&pMessage,
		0,
		(va_list*)pArgs);

	/* ����I�� */
	bReturn = TRUE;

Exit: /* �㏈�� */
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

/* �C�x���g���O�̓ǂݎ�� */
BOOL WinEventTool::ReadLog(HANDLE hEventLog, EVENTLOGRECORD& evtLogRecord, char* pRetMsg, char* pSourceName, char* pComputerName)
{
	DWORD bufSize;
	DWORD readBytes;
	DWORD nextSize;
	BOOL bResult;
	EVENTLOGRECORD* pBuf = NULL;

	/* �C�x���g���O�̃T�C�Y�擾 */
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

	/* �o�b�t�@���蓖�� */
	bufSize = nextSize;
	pBuf = (EVENTLOGRECORD*)GlobalAlloc(GMEM_FIXED, bufSize);

	/* �C�x���g���O�̓ǂݎ�� */
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

	// EVENTLOGRECORD�ȍ~�̃f�[�^���
	DWORD i;
	char* cp;

	cp = (char*)pBuf;
	cp += sizeof(EVENTLOGRECORD);

	// �\�[�X��
	::strcpy_s(pSourceName, 256, cp);
	cp += strlen(cp) + 1;

	// �R���s���[�^��
	::strcpy_s(pComputerName, 256, cp);
	cp += strlen(cp) + 1;

	/* ���b�Z�[�W�̒��o */
	const char** pArgs = NULL;
	pArgs = GetArgs(pBuf);
	ExtractMessage(pSourceName, "EventMessageFile", pArgs, pBuf->EventID, pRetMsg);
	if (pArgs != NULL) {
		GlobalFree(pArgs);
		pArgs = NULL;
	}

	/* �o�b�t�@��� */
	GlobalFree(pBuf);
	pBuf = NULL;

Exit: /* �㏈�� */
	if (pBuf != NULL) GlobalFree(pBuf);
	if (pArgs != NULL) GlobalFree(pArgs);

	return TRUE;
}