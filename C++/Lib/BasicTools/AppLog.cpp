#include "pch.h"
#include "AppLog.h"

/******************
* AppLog
******************/
CAppLog::CAppLog()
{
}
CAppLog::~CAppLog()
{
}

/*
*	�W���o�͂Ƀ��O�����������܂��B
*	@param[in] �t�@�C���� (FUNCTRACEARGS)
*	@param[in] �s�� (FUNCTRACEARGS)
*	@param[in] �֐��� (FUNCTRACEARGS)
*	@param[in] �o�͕�����t�H�[�}�b�g
*	@param[in] �ϐ�����...
*	@return none
*/
void CAppLog::Trace(const char* pFileName, int line, const char* pFuncName, const char* pStrFmt, ...)
{
	printf("Trace,%s(%d)%s,", pFileName, line, pFuncName);
	va_list args;
	va_start(args, pStrFmt);
	Print(pStrFmt, args);
	va_end(args);
	printf("\n");
}

/*
*	�W���o�͂Ƀ��O�����������܂��B
*	@param[in] �t�@�C���� (FUNCTRACEARGS)
*	@param[in] �s�� (FUNCTRACEARGS)
*	@param[in] �֐��� (FUNCTRACEARGS)
*	@param[in] �o�͕�����t�H�[�}�b�g
*	@param[in] �ϐ�����...
*	@return none
*/
void CAppLog::Debug(const char* pFileName, int line, const char* pFuncName, const char* pStrFmt, ...)
{
	printf("Debug,%s(%d)%s,", pFileName, line, pFuncName);

	va_list args;
	va_start(args, pStrFmt);
	Print(pStrFmt, args);
	va_end(args);
	printf("\n");
}

/*
*	�W���o�͂Ƀ��O�����������܂��B
*	@param[in] �t�@�C���� (FUNCTRACEARGS)
*	@param[in] �s�� (FUNCTRACEARGS)
*	@param[in] �o�͕�����t�H�[�}�b�g
*	@param[in] �ϐ�����...
*	@return none
*/
void CAppLog::Trace(const char * pFileName, int line, const char * pStrFmt, ...)
{
	printf("Trace,%s(%d)", pFileName, line);
	va_list args;
	va_start(args, pStrFmt);
	Print(pStrFmt, args);
	va_end(args);
	printf("\n");
}

/*
*	�W���o�͂Ƀ��O�����������܂��B
*	@param[in] �t�@�C���� (FUNCTRACEARGS)
*	@param[in] �s�� (FUNCTRACEARGS)
*	@param[in] �o�͕�����t�H�[�}�b�g
*	@param[in] �ϐ�����...
*	@return none
*/
void CAppLog::Debug(const char * pFileName, int line, const char * pStrFmt, ...)
{
	printf("Debug,%s(%d)", pFileName, line);

	va_list args;
	va_start(args, pStrFmt);
	Print(pStrFmt, args);
	va_end(args);
	printf("\n");
}

/*
*	�W���o�͂Ƀt�H�[�}�b�g�������������܂��B
*	@param[in] �o�͕�����t�H�[�}�b�g
*	@param[in] �ϐ�����...
*	@return none
*/
void CAppLog::Print(const char* pStrFmt, va_list args)
{
	for (const char* p = pStrFmt; *p != '\0'; ++p) {
		if (p[0] != '%')
		{
			printf("%c", p[0]);
		}
		else
		{
			switch (p[1])
			{
			case 'd':
				printf("%d", va_arg(args, int));
				p++;
				break;
			case 'f':
				printf("%f", va_arg(args, float));
				p++;
				break;
			case 'c':
				printf("%c", va_arg(args, char));
				p++;
				break;
			case 's':
				printf("%s", va_arg(args, const char*));
				p++;
				break;
			case 'l':
				switch (p[2])
				{
				case 'd':
					printf("%d", va_arg(args, long));
					p++;
					p++;
					break;
				case 'f':
					printf("%f", va_arg(args, double));
					p++;
					p++;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}
}

CFuncTrace::CFuncTrace(const char* pFileName, int line, const char* pFuncName, const char* pStrFmt, ...)
{
	// �ϐ������t�H�[�}�b�g
	va_list args;
	va_start(args, pStrFmt);
	char pFuncInfo[Max_FormatBufferSize];
	vsprintf_s(pFuncInfo, sizeof(pFuncInfo), pStrFmt, args);
	va_end(args);

	// �֐����i�[
	strcpy_s(m_pFileName, sizeof(m_pFileName), pFileName);
	m_startLine = line;
	strcpy_s(m_pFuncName, sizeof(m_pFuncName), pFuncName);
	strcpy_s(m_pFuncInfo, sizeof(m_pFuncInfo), pFuncInfo);

	// Start
	char funcInfo[Max_FormatBufferSize];
	strcpy_s(funcInfo, sizeof(funcInfo), m_pFuncInfo);
	const char* startSuffix = funcInfo[0] == '\0' ? "Start" : ",Start";
	strcpy_s(&funcInfo[strlen(funcInfo)], sizeof(funcInfo) - strlen(funcInfo), startSuffix);
	printf("%s(%d)%s,%s\n", m_pFileName, m_startLine, m_pFuncName, funcInfo);
}

/******************
* FuncTrace
******************/
/*
*	@param[in] �t�@�C���� (FUNCTRACEARGS)
*	@param[in] �s�� (FUNCTRACEARGS)
*	@param[in] �o�͕�����t�H�[�}�b�g
*	@param[in] �ϐ�����...
*/
CFuncTrace::CFuncTrace(const char * pFileName, int line, const char * pStrFmt, ...)
{
	// �ϐ������t�H�[�}�b�g
	va_list args;
	va_start(args, pStrFmt);
	char pFuncInfo[Max_FormatBufferSize];
	vsprintf_s(pFuncInfo, sizeof(pFuncInfo), pStrFmt, args);
	va_end(args);

	// �֐����i�[
	strcpy_s(m_pFileName, sizeof(m_pFileName), pFileName);
	m_startLine = line;
	::memset(m_pFuncName, 0, sizeof(m_pFuncName));
	strcpy_s(m_pFuncInfo, sizeof(m_pFuncInfo), pFuncInfo);

	// Start
	char funcInfo[Max_FormatBufferSize];
	strcpy_s(funcInfo, sizeof(funcInfo), m_pFuncInfo);
	const char* startSuffix = funcInfo[0] == '\0' ? "Start" : ",Start";
	strcpy_s(&funcInfo[strlen(funcInfo)], sizeof(funcInfo) - strlen(funcInfo), startSuffix);
	printf("%s(%d)%s\n", m_pFileName, m_startLine, funcInfo);
}

CFuncTrace::~CFuncTrace()
{
	// End
	char funcInfo[Max_FormatBufferSize];
	strcpy_s(funcInfo, sizeof(funcInfo), m_pFuncInfo);
	const char* endSuffix = funcInfo[0] == '\0' ? "End" : ",End";
	strcpy_s(&funcInfo[strlen(funcInfo)], sizeof(funcInfo) - strlen(funcInfo), endSuffix);
	if (m_pFuncName[0] == '\0')
	{
		printf("%s(%d)%s\n", m_pFileName, m_startLine, funcInfo);
	}
	else
	{
		printf("%s(%d)%s,%s\n", m_pFileName, m_startLine, m_pFuncName, funcInfo);
	}
}

/*
*	�W���o�͂Ƀ��O�����������܂��B
*	@param[in] �s��
*	@param[in] �o�͕�����t�H�[�}�b�g
*	@param[in] �ϐ�����...
*	@return none
*/
void CFuncTrace::Trace(int line, const char * pStrFmt, ...)
{
	// �ϐ������t�H�[�}�b�g
	va_list args;
	va_start(args, pStrFmt);
	char pString[Max_FormatBufferSize];
	vsprintf_s(pString, sizeof(pString), pStrFmt, args);
	va_end(args);

	if (m_pFuncName[0] == '\0')
	{
		CAppLog::Trace(m_pFileName, line, "%s, %s", m_pFuncInfo, pString);
	}
	else
	{
		CAppLog::Trace(m_pFileName, line, m_pFuncName, "%s, %s", m_pFuncInfo, pString);
	}
}

/*
*	�W���o�͂Ƀ��O�����������܂��B
*	@param[in] �s��
*	@param[in] �o�͕�����t�H�[�}�b�g
*	@param[in] �ϐ�����...
*	@return none
*/
void CFuncTrace::Debug(int line, const char * pStrFmt, ...)
{
	// �ϐ������t�H�[�}�b�g
	va_list args;
	va_start(args, pStrFmt);
	char pString[Max_FormatBufferSize];
	vsprintf_s(pString, sizeof(pString), pStrFmt, args);
	va_end(args);

	if (m_pFuncName[0] == '\0')
	{
		CAppLog::Debug(m_pFileName, line, "%s, %s", m_pFuncInfo, pString);
	}
	else
	{
		CAppLog::Debug(m_pFileName, line, m_pFuncName, "%s, %s", m_pFuncInfo, pString);
	}
}

/******************
* LogFile
******************/
// �V���O���g���N���X
//LogFile g_logFile;
//
//LogFile::LogFile(void)
//{
//	strcpy_s(m_pLogFilePath, sizeof(m_pLogFilePath), "log.txt");
//}
//LogFile::~LogFile(void)
//{
//}
//LogFile* LogFile::GetInstance(void)
//{
//	return &g_logFile;
//}
//
///*
//*	���O�t�@�C���̏ꏊ��ύX���܂��B
//*	@param[in] �t�@�C���p�X
//*	@return ����ɏI�������ꍇ��TRUE,����ȊO��FALSE
//*/
//BOOL LogFile::Locate(const char * pFilePath)
//{
//	strcpy_s(m_pLogFilePath, sizeof(m_pLogFilePath), pFilePath);
//	USES_CONVERSION;
//	if (!PathFileExists(A2W(pFilePath)))
//	{
//		printf("file:%s is not exist\n", pFilePath);
//	}
//	return TRUE;
//}
//
///*
//*	���O�t�@�C���̏ꏊ��Ԃ��܂��B
//*	@return ���O�t�@�C���p�X
//*/
//char* LogFile::Locate(void)
//{
//	return m_pLogFilePath;
//}
//
///*
//*	���O�����������܂��B
//*	@param[in] �t�@�C���� (FUNCTRACEARGS)
//*	@param[in] �s�� (FUNCTRACEARGS)
//*	@param[in] �o�͕�����t�H�[�}�b�g
//*	@param[in] �ϐ�����...
//*	@return ����ɏI�������ꍇ��TRUE,����ȊO��FALSE
//*/
//BOOL LogFile::Write(const char * pFileName, int line, const char* pStrFmt, ...)
//{
//	// �ϐ������t�H�[�}�b�g
//	va_list args;
//	va_start(args, pStrFmt);
//	char pString[Max_FormatBufferSize];
//	vsprintf_s(pString, sizeof(pString), pStrFmt, args);
//	va_end(args);
//
//	// ���ݎ����擾
//	SYSTEMTIME st;
//	GetSystemTime(&st);
//	CStringA nowStr;
//	nowStr.Format("%04u/%02u/%02u %02u:%02u:%02u:%03u",
//		st.wYear,
//		st.wMonth,
//		st.wDay,
//		st.wHour,
//		st.wMinute,
//		st.wSecond,
//		st.wMilliseconds
//		);
//
//	// �o�͕����t�H�[�}�b�g
//	CStringA outputStr;
//	outputStr.Format("%s %s(%d):%s\n", nowStr, pFileName, line, pString);
//
//	USES_CONVERSION;
//	if (!m_logFile.Open(A2W(m_pLogFilePath), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
//	{
//		printf("file:%s is not exist\n", m_pLogFilePath);
//		return FALSE;
//	}
//	m_logFile.SeekToEnd();
//	m_logFile.Write(outputStr, outputStr.GetLength());
//	m_logFile.Close();
//	return TRUE;
//}
//
///*
//*	������𐮌`�����ɏ��������܂��B
//*	@param[in] �o�͕�����t�H�[�}�b�g
//*	@param[in] �ϐ�����...
//*	@return ����ɏI�������ꍇ��TRUE,����ȊO��FALSE
//*/
//BOOL LogFile::Put(const char* pStrFmt, ...)
//{
//	// �ϐ������t�H�[�}�b�g
//	va_list args;
//	va_start(args, pStrFmt);
//	char pString[Max_FormatBufferSize];
//	vsprintf_s(pString, sizeof(pString), pStrFmt, args);
//	va_end(args);
//
//	USES_CONVERSION;
//	if (!m_logFile.Open(A2W(m_pLogFilePath), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
//	{
//		return FALSE;
//	}
//	m_logFile.SeekToEnd();
//	m_logFile.Write(pString, strlen(pString));
//	m_logFile.Close();
//
//	return TRUE;
//}
//
///*
//*	�t�@�C���t���p�X
//*	@return �t�@�C���t���p�X
//*/
//CStringA LogFile::GetFilePath(void)
//{
//	USES_CONVERSION;
//	return W2A(m_logFile.GetFilePath());
//}
//
///*
//*	�t�@�C���T�C�Y
//*	@return �t�@�C���T�C�Y, ������Ȃ������Ƃ�-1
//*/
//int LogFile::GetLength(void)
//{
//	USES_CONVERSION;
//	if (!m_logFile.Open(A2W(m_pLogFilePath), CFile::modeRead))
//	{
//		return -1;
//	}
//	int fileSize = m_logFile.GetLength();
//	m_logFile.Close();
//
//	return fileSize;
//}
//
///*
//*	�Ō�ɕҏW�������t
//*	@return CTime
//*/
//CTime LogFile::LastModified(void)
//{
//	USES_CONVERSION;
//	if (!m_logFile.Open(A2W(m_pLogFilePath), CFile::modeRead))
//	{
//		return NULL;
//	}
//	CFileStatus fileSts;
//	m_logFile.GetStatus(fileSts);
//	m_logFile.Close();
//	return fileSts.m_mtime;
//}
