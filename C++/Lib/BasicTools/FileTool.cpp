#include "pch.h"
#include "FileTool.h"

CFileTool::CFileTool(std::string filePath)
	: m_hFile(INVALID_HANDLE_VALUE)
	, m_fileSize(ULONG_MAX)
	, m_rawData(NULL)
	, m_rawDataSize(ULONG_MAX)
	, m_lineParsedData(NULL)
	, m_lineParsedDataSize(ULONG_MAX)
	, m_pLines(NULL)
	, m_lineCount(ULONG_MAX)
{
	m_filePath = filePath;
}

CFileTool::CFileTool(void)
	: m_hFile(INVALID_HANDLE_VALUE)
	, m_fileSize(ULONG_MAX)
	, m_rawData(NULL)
	, m_rawDataSize(ULONG_MAX)
	, m_lineParsedData(NULL)
	, m_lineParsedDataSize(ULONG_MAX)
	, m_pLines(NULL)
	, m_lineCount(ULONG_MAX)
{
	m_filePath = "";
}

CFileTool::~CFileTool(void)
{
	Close();
}

void CFileTool::Sample(void)
{
	// ファイルを開く
	if (!Open())
	{
		return;
	}

	// ファイル読み出し
	if (!ParseLineText())
	{
		return;
	}
	for (int lineCount = 0; lineCount < m_lineCount; lineCount++)
	{
		std::cout << "[" << lineCount << "]" << m_pLines[lineCount] << std::endl;
	}

	// ファイル追記
	//const char* pAddText = "add\r\n";
	//DWORD nNumberOfBytesToWrite = strlen(pAddText);
	//DWORD numberOfBytesWritten = 0;
	//::WriteFile(
	//	m_hFile
	//	, pAddText
	//	, nNumberOfBytesToWrite
	//	, &numberOfBytesWritten
	//	, NULL
	//);

	// カレントディレクトリ出力
	char cdir[255];
	::GetCurrentDirectoryA(255, cdir);
	std::cout << "Current Directory : " << cdir << std::endl;

	// ファイルを閉じる
	Close();
}

DWORD CFileTool::GetSize(void)
{
	if (m_fileSize != ULONG_MAX)
	{
		return m_fileSize;
	}

	// ファイルを開く
	if (!Open())
	{
		return ULONG_MAX;
	}

	// ファイル読み出し
	m_fileSize = ::GetFileSize(m_hFile, NULL);

	// ファイルを閉じる
	Close();

	return m_fileSize;
}

BOOL CFileTool::Read(void)
{
	// データ確認
	if (m_rawData != NULL)
	{
		return TRUE;
	}
	// ファイルを開く
	else if (!Open())
	{
		return FALSE;
	}

	// ファイル読み出し
	m_fileSize = ::GetFileSize(m_hFile, NULL);

	// ファイル読み出しパラメータ
	DWORD dwFileSize = m_fileSize;
	DWORD nNumberOfBytesToRead = dwFileSize;
	m_rawDataSize = dwFileSize + 1;
	m_rawData = new BYTE[m_rawDataSize];
	::memset(m_rawData, 0, m_rawDataSize);
	DWORD numberOfBytesRead;

	// ファイル読み出し
	BOOL ret = ::ReadFile(
		m_hFile
		, m_rawData
		, nNumberOfBytesToRead
		, &numberOfBytesRead
		, NULL);
	if (!ret)
	{
		return FALSE;
	}
}

DWORD CFileTool::GetLineCount(void)
{
	if (m_lineCount != ULONG_MAX)
	{
		return m_lineCount;
	}
	else if (m_rawData == NULL)
	{
		return 0;
	}

	// 改行文字カウント
	DWORD lineCount = 0;
	for (int index = 0; index < m_fileSize; index++)
	{
		if (m_rawData[index] == '\n')
		{
			lineCount++;
		}
	}
	lineCount++;
	return lineCount;
}

BOOL CFileTool::ParseLineText(void)
{
	if (m_rawData == NULL || m_rawDataSize == 0)
	{
		if (!Read())
		{
			return FALSE;
		}
	}

	// 改行文字カウント
	m_lineCount = GetLineCount();
	if (m_lineCount == 0)
	{
		return FALSE;
	}

	// 行開始位置をポインタに格納
	m_pLines = new BYTE* [m_lineCount];
	::memset(m_pLines, 0, sizeof(BYTE*) * m_lineCount);

	// 行毎テキスト用バッファ
	m_lineParsedDataSize = m_rawDataSize;
	m_lineParsedData = new BYTE[m_lineParsedDataSize];
	::memcpy_s(m_lineParsedData, m_lineParsedDataSize, m_rawData, m_rawDataSize);

	// 行の先頭ポインタを格納
	DWORD lineCount = 0;
	m_pLines[lineCount++] = &m_lineParsedData[0];
	for (int index = 0; index < m_fileSize; index++)
	{
		if (m_lineParsedData[index] == '\n')
		{
			// 改行コードの次の文字を取得
			m_pLines[lineCount++] = &m_lineParsedData[index + 1];

			// CRLFの場合はCR削除
			if (0 <= index - 1 && m_lineParsedData[index - 1] == '\r')
			{
				m_lineParsedData[index - 1] = '\0';
			}

			// LF削除
			m_lineParsedData[index] = '\0';
		}
	}

	return TRUE;
}

BOOL CFileTool::Open(void)
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}
	//else if (m_filePath == "")
	//{
	//	return FALSE;
	//}

	// ファイルパラメータ
	DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
	DWORD dwShareMode = FILE_SHARE_READ; // | FILE_SHARE_DELETE;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
	DWORD dwCreationDisposition = OPEN_EXISTING; //  CREATE_ALWAYS;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE hTemplateFile = NULL;

	/*
	CreateFile
	ファイルまたはI/Oデバイスを作成または開きます。
	最も一般的に使用されるI/Oデバイスは、
	ファイル、ファイルストリーム、ディレクトリ、物理ディスク、ボリューム、コンソールバッファ、
	テープドライブ、通信リソース、メールスロット、およびパイプです。

	この関数は、ファイルまたはデバイス、および指定されたフラグと属性に応じて、
	さまざまなタイプのI/Oのファイルまたはデバイスにアクセスするために使用できるハンドルを返します。
	この操作をトランザクション操作として実行し、トランザクションI/Oに使用できるハンドルを生成するには、
	CreateFileTransacted関数を使用します。
	*/
	// ファイル作成or開く
	m_hFile = ::CreateFileA(
		m_filePath.c_str()
		, dwDesiredAccess
		, dwShareMode
		, lpSecurityAttributes
		, dwCreationDisposition
		, dwFlagsAndAttributes
		, hTemplateFile
	);
	if (m_hFile == INVALID_HANDLE_VALUE) {
		DWORD lastErr = ::GetLastError();
		LPVOID lpMsgBuf = NULL;
		DWORD result = ::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER  //      テキストのメモリ割り当てを要求する
			| FORMAT_MESSAGE_FROM_SYSTEM    //      エラーメッセージはWindowsが用意しているものを使用
			| FORMAT_MESSAGE_IGNORE_INSERTS,//      次の引数を無視してエラーコードに対するエラーメッセージを作成する
			NULL,
			lastErr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);
		if (0 < result)
		{
			::MessageBox(
				NULL,
				(LPTSTR)lpMsgBuf,
				TEXT("エラー"),
				MB_OK
			);
		}
		LocalFree(lpMsgBuf);
		return FALSE;
	}


	//switch (lastErr)
	//{
	//case ERROR_SUCCESS:
	//case ERROR_INVALID_FUNCTION:
	//case ERROR_FILE_NOT_FOUND:
	//case ERROR_PATH_NOT_FOUND:
	//default:
	//	break;
	//}

	return TRUE;
}

void CFileTool::Close(void)
{
	if (m_rawData != NULL)
	{
		delete m_rawData;
		m_rawData = NULL;
		m_rawDataSize = ULONG_MAX;
	}

	if (m_lineParsedData != NULL)
	{
		delete m_lineParsedData;
		m_lineParsedData = NULL;
		m_lineParsedDataSize = ULONG_MAX;
	}
	
	if (m_pLines != NULL)
	{
		delete[] m_pLines;
		m_pLines = NULL;
		m_lineCount = ULONG_MAX;
	}

	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}
