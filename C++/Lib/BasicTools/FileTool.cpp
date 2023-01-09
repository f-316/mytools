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
	//SampleRead();
	SampleWrite();
}

void CFileTool::SampleRead(void)
{
	//SetFilePath("sample_shift-jis.txt");
	SetFilePath("sample_utf-8.txt");
	//SetFilePath("sample_utf-16.txt");
	//SetFilePath("sample_wchar.txt");

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

	// カレントディレクトリ出力
	char cdir[255];
	::GetCurrentDirectoryA(255, cdir);
	std::cout << "Current Directory : " << cdir << std::endl;

	// ファイルを閉じる
	Close();
}

void CFileTool::SampleWrite(void)
{
	int nCase = 3;
	switch (nCase)
	{
	case 0:
	{
		// Shift-JIS
		SetFilePath("sample_shift-jis.txt");
		Open(TRUE, TRUE);
		const char* pStr = "あいうえお" "\r\n";
		Write((BYTE*)pStr, strlen(pStr));
		//std::string pStr = "あいうえお\r\n";
		//Write((BYTE*)pStr.c_str(), strlen(pStr.c_str()));
	}
	break;

	case 1:
	{
		// UTF-16,テキストファイルに出力時はBOMが必要
		// \xFEFFで0xFF,0xFEが先頭に付きUTF-16LEとなる。BEは0xFE,0xFF
		SetFilePath("sample_utf-16.txt");
		Open(TRUE, TRUE);
		const char16_t* pStr = /*BOM*/u"\xFEFF" "あいうえお" "\r\n";
		Write((BYTE*)pStr, std::char_traits<char16_t>::length(pStr) * sizeof(char16_t));
	}
	break;

	case 2:
	{
		// WCHAR(UTF-16と同様)
		SetFilePath("sample_wchar.txt");
		Open(TRUE, TRUE);
		const wchar_t* pStr = /*BOM*/L"\xFEFF" "あいうえお" "\r\n";
		Write((BYTE*)pStr, wcslen(pStr) * sizeof(wchar_t));
	}
	break;

	case 3:
	{
		// UTF-16
		// \xの後に16bit数値を16進数で表記できる
		SetFilePath("sample_utf-16_2.txt");
		Open(TRUE, TRUE);
		const char16_t* pStr = /*BOM*/u"\xFEFF" /*あ*/u"\x3042" /*い*/u"\x3044" "\r\n";
		//const char16_t* pStr = /*BOM*/u"\xFEFF\x3042\x3044\r\n";
		Write((BYTE*)pStr, std::char_traits<char16_t>::length(pStr) * sizeof(char16_t));
	}
	break;

	case 4:
	{
		// UTF-8,テキストファイルに出力時はBOMが必要,不要なものもある。
		SetFilePath("sample_utf-8_with-bom.txt");
		Open(TRUE, TRUE);
		const char* pStr = /*BOM*/"\xEF\xBB\xBF" /*あ*/"\xe3\x81\x82" /*い*/"\xe3\x81\x84" "\r\n";
		Write((BYTE*)pStr, strlen(pStr));
	}
	break;
	
	case 5:
	{
		// UTF-8,BOMなし版
		SetFilePath("sample_utf-8_without-bom.txt");
		Open(TRUE, TRUE);
		const char* pStr = /*あ*/"\xe3\x81\x82" /*い*/"\xe3\x81\x84" "\r\n";
		Write((BYTE*)pStr, strlen(pStr));
	}
	break;

	default:
		break;
	}

	// ファイルを閉じる
	Close();
}

/*
* 
*/
BOOL CFileTool::Open(BOOL forceCreate, BOOL addMode)
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

	//if (!::PathFileExistsA(m_filePath.c_str()) && forceCreate)

	// 上書きモード
	if (forceCreate)
	{
		dwCreationDisposition = CREATE_ALWAYS;
	}
	// 追記モード
	else if (addMode)
	{
		dwDesiredAccess = FILE_APPEND_DATA;
	}

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

void CFileTool::SetFilePath(std::string filePath)
{
	m_filePath = filePath;
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

BOOL CFileTool::Write(BYTE* pBuffer, DWORD bufferSize)
{
	// ReadFileしていると末尾書き込みになる。

	// ファイル書き込み
	DWORD nNumberOfBytesToWrite = bufferSize;
	DWORD numberOfBytesWritten = 0;
	return ::WriteFile(
		m_hFile
		, pBuffer
		, nNumberOfBytesToWrite
		, &numberOfBytesWritten
		, NULL
	);
}
