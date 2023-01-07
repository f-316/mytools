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
*	標準出力にログを書きだします。
*	@param[in] ファイル名 (FUNCTRACEARGS)
*	@param[in] 行数 (FUNCTRACEARGS)
*	@param[in] 関数名 (FUNCTRACEARGS)
*	@param[in] 出力文字列フォーマット
*	@param[in] 可変数引数...
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
*	標準出力にログを書きだします。
*	@param[in] ファイル名 (FUNCTRACEARGS)
*	@param[in] 行数 (FUNCTRACEARGS)
*	@param[in] 関数名 (FUNCTRACEARGS)
*	@param[in] 出力文字列フォーマット
*	@param[in] 可変数引数...
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
*	標準出力にログを書きだします。
*	@param[in] ファイル名 (FUNCTRACEARGS)
*	@param[in] 行数 (FUNCTRACEARGS)
*	@param[in] 出力文字列フォーマット
*	@param[in] 可変数引数...
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
*	標準出力にログを書きだします。
*	@param[in] ファイル名 (FUNCTRACEARGS)
*	@param[in] 行数 (FUNCTRACEARGS)
*	@param[in] 出力文字列フォーマット
*	@param[in] 可変数引数...
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
*	標準出力にフォーマット文を書きだします。
*	@param[in] 出力文字列フォーマット
*	@param[in] 可変数引数...
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
	// 可変数引数フォーマット
	va_list args;
	va_start(args, pStrFmt);
	char pFuncInfo[Max_FormatBufferSize];
	vsprintf_s(pFuncInfo, sizeof(pFuncInfo), pStrFmt, args);
	va_end(args);

	// 関数情報格納
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
*	@param[in] ファイル名 (FUNCTRACEARGS)
*	@param[in] 行数 (FUNCTRACEARGS)
*	@param[in] 出力文字列フォーマット
*	@param[in] 可変数引数...
*/
CFuncTrace::CFuncTrace(const char * pFileName, int line, const char * pStrFmt, ...)
{
	// 可変数引数フォーマット
	va_list args;
	va_start(args, pStrFmt);
	char pFuncInfo[Max_FormatBufferSize];
	vsprintf_s(pFuncInfo, sizeof(pFuncInfo), pStrFmt, args);
	va_end(args);

	// 関数情報格納
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
*	標準出力にログを書きだします。
*	@param[in] 行数
*	@param[in] 出力文字列フォーマット
*	@param[in] 可変数引数...
*	@return none
*/
void CFuncTrace::Trace(int line, const char * pStrFmt, ...)
{
	// 可変数引数フォーマット
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
*	標準出力にログを書きだします。
*	@param[in] 行数
*	@param[in] 出力文字列フォーマット
*	@param[in] 可変数引数...
*	@return none
*/
void CFuncTrace::Debug(int line, const char * pStrFmt, ...)
{
	// 可変数引数フォーマット
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
// シングルトンクラス
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
//*	ログファイルの場所を変更します。
//*	@param[in] ファイルパス
//*	@return 正常に終了した場合はTRUE,それ以外はFALSE
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
//*	ログファイルの場所を返します。
//*	@return ログファイルパス
//*/
//char* LogFile::Locate(void)
//{
//	return m_pLogFilePath;
//}
//
///*
//*	ログを書きだします。
//*	@param[in] ファイル名 (FUNCTRACEARGS)
//*	@param[in] 行数 (FUNCTRACEARGS)
//*	@param[in] 出力文字列フォーマット
//*	@param[in] 可変数引数...
//*	@return 正常に終了した場合はTRUE,それ以外はFALSE
//*/
//BOOL LogFile::Write(const char * pFileName, int line, const char* pStrFmt, ...)
//{
//	// 可変数引数フォーマット
//	va_list args;
//	va_start(args, pStrFmt);
//	char pString[Max_FormatBufferSize];
//	vsprintf_s(pString, sizeof(pString), pStrFmt, args);
//	va_end(args);
//
//	// 現在時刻取得
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
//	// 出力文字フォーマット
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
//*	文字列を整形せずに書きだします。
//*	@param[in] 出力文字列フォーマット
//*	@param[in] 可変数引数...
//*	@return 正常に終了した場合はTRUE,それ以外はFALSE
//*/
//BOOL LogFile::Put(const char* pStrFmt, ...)
//{
//	// 可変数引数フォーマット
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
//*	ファイルフルパス
//*	@return ファイルフルパス
//*/
//CStringA LogFile::GetFilePath(void)
//{
//	USES_CONVERSION;
//	return W2A(m_logFile.GetFilePath());
//}
//
///*
//*	ファイルサイズ
//*	@return ファイルサイズ, 見つからなかったとき-1
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
//*	最後に編集した日付
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
