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

	// �t�@�C�����J��
	if (!Open())
	{
		return;
	}

	// �t�@�C���ǂݏo��
	if (!ParseLineText())
	{
		return;
	}
	for (int lineCount = 0; lineCount < m_lineCount; lineCount++)
	{
		std::cout << "[" << lineCount << "]" << m_pLines[lineCount] << std::endl;
	}

	// �J�����g�f�B���N�g���o��
	char cdir[255];
	::GetCurrentDirectoryA(255, cdir);
	std::cout << "Current Directory : " << cdir << std::endl;

	// �t�@�C�������
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
		const char* pStr = "����������" "\r\n";
		Write((BYTE*)pStr, strlen(pStr));
		//std::string pStr = "����������\r\n";
		//Write((BYTE*)pStr.c_str(), strlen(pStr.c_str()));
	}
	break;

	case 1:
	{
		// UTF-16,�e�L�X�g�t�@�C���ɏo�͎���BOM���K�v
		// \xFEFF��0xFF,0xFE���擪�ɕt��UTF-16LE�ƂȂ�BBE��0xFE,0xFF
		SetFilePath("sample_utf-16.txt");
		Open(TRUE, TRUE);
		const char16_t* pStr = /*BOM*/u"\xFEFF" "����������" "\r\n";
		Write((BYTE*)pStr, std::char_traits<char16_t>::length(pStr) * sizeof(char16_t));
	}
	break;

	case 2:
	{
		// WCHAR(UTF-16�Ɠ��l)
		SetFilePath("sample_wchar.txt");
		Open(TRUE, TRUE);
		const wchar_t* pStr = /*BOM*/L"\xFEFF" "����������" "\r\n";
		Write((BYTE*)pStr, wcslen(pStr) * sizeof(wchar_t));
	}
	break;

	case 3:
	{
		// UTF-16
		// \x�̌��16bit���l��16�i���ŕ\�L�ł���
		SetFilePath("sample_utf-16_2.txt");
		Open(TRUE, TRUE);
		const char16_t* pStr = /*BOM*/u"\xFEFF" /*��*/u"\x3042" /*��*/u"\x3044" "\r\n";
		//const char16_t* pStr = /*BOM*/u"\xFEFF\x3042\x3044\r\n";
		Write((BYTE*)pStr, std::char_traits<char16_t>::length(pStr) * sizeof(char16_t));
	}
	break;

	case 4:
	{
		// UTF-8,�e�L�X�g�t�@�C���ɏo�͎���BOM���K�v,�s�v�Ȃ��̂�����B
		SetFilePath("sample_utf-8_with-bom.txt");
		Open(TRUE, TRUE);
		const char* pStr = /*BOM*/"\xEF\xBB\xBF" /*��*/"\xe3\x81\x82" /*��*/"\xe3\x81\x84" "\r\n";
		Write((BYTE*)pStr, strlen(pStr));
	}
	break;
	
	case 5:
	{
		// UTF-8,BOM�Ȃ���
		SetFilePath("sample_utf-8_without-bom.txt");
		Open(TRUE, TRUE);
		const char* pStr = /*��*/"\xe3\x81\x82" /*��*/"\xe3\x81\x84" "\r\n";
		Write((BYTE*)pStr, strlen(pStr));
	}
	break;

	default:
		break;
	}

	// �t�@�C�������
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

	// �t�@�C���p�����[�^
	DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
	DWORD dwShareMode = FILE_SHARE_READ; // | FILE_SHARE_DELETE;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
	DWORD dwCreationDisposition = OPEN_EXISTING; //  CREATE_ALWAYS;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE hTemplateFile = NULL;

	//if (!::PathFileExistsA(m_filePath.c_str()) && forceCreate)

	// �㏑�����[�h
	if (forceCreate)
	{
		dwCreationDisposition = CREATE_ALWAYS;
	}
	// �ǋL���[�h
	else if (addMode)
	{
		dwDesiredAccess = FILE_APPEND_DATA;
	}

	/*
	CreateFile
	�t�@�C���܂���I/O�f�o�C�X���쐬�܂��͊J���܂��B
	�ł���ʓI�Ɏg�p�����I/O�f�o�C�X�́A
	�t�@�C���A�t�@�C���X�g���[���A�f�B���N�g���A�����f�B�X�N�A�{�����[���A�R���\�[���o�b�t�@�A
	�e�[�v�h���C�u�A�ʐM���\�[�X�A���[���X���b�g�A����уp�C�v�ł��B

	���̊֐��́A�t�@�C���܂��̓f�o�C�X�A����юw�肳�ꂽ�t���O�Ƒ����ɉ����āA
	���܂��܂ȃ^�C�v��I/O�̃t�@�C���܂��̓f�o�C�X�ɃA�N�Z�X���邽�߂Ɏg�p�ł���n���h����Ԃ��܂��B
	���̑�����g�����U�N�V��������Ƃ��Ď��s���A�g�����U�N�V����I/O�Ɏg�p�ł���n���h���𐶐�����ɂ́A
	CreateFileTransacted�֐����g�p���܂��B
	*/
	// �t�@�C���쐬or�J��
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
			FORMAT_MESSAGE_ALLOCATE_BUFFER  //      �e�L�X�g�̃��������蓖�Ă�v������
			| FORMAT_MESSAGE_FROM_SYSTEM    //      �G���[���b�Z�[�W��Windows���p�ӂ��Ă�����̂��g�p
			| FORMAT_MESSAGE_IGNORE_INSERTS,//      ���̈����𖳎����ăG���[�R�[�h�ɑ΂���G���[���b�Z�[�W���쐬����
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
				TEXT("�G���["),
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

	// �t�@�C�����J��
	if (!Open())
	{
		return ULONG_MAX;
	}

	// �t�@�C���ǂݏo��
	m_fileSize = ::GetFileSize(m_hFile, NULL);

	// �t�@�C�������
	Close();

	return m_fileSize;
}

BOOL CFileTool::Read(void)
{
	// �f�[�^�m�F
	if (m_rawData != NULL)
	{
		return TRUE;
	}
	// �t�@�C�����J��
	else if (!Open())
	{
		return FALSE;
	}

	// �t�@�C���ǂݏo��
	m_fileSize = ::GetFileSize(m_hFile, NULL);

	// �t�@�C���ǂݏo���p�����[�^
	DWORD dwFileSize = m_fileSize;
	DWORD nNumberOfBytesToRead = dwFileSize;
	m_rawDataSize = dwFileSize + 1;
	m_rawData = new BYTE[m_rawDataSize];
	::memset(m_rawData, 0, m_rawDataSize);
	DWORD numberOfBytesRead;

	// �t�@�C���ǂݏo��
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

	// ���s�����J�E���g
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

	// ���s�����J�E���g
	m_lineCount = GetLineCount();
	if (m_lineCount == 0)
	{
		return FALSE;
	}

	// �s�J�n�ʒu���|�C���^�Ɋi�[
	m_pLines = new BYTE* [m_lineCount];
	::memset(m_pLines, 0, sizeof(BYTE*) * m_lineCount);

	// �s���e�L�X�g�p�o�b�t�@
	m_lineParsedDataSize = m_rawDataSize;
	m_lineParsedData = new BYTE[m_lineParsedDataSize];
	::memcpy_s(m_lineParsedData, m_lineParsedDataSize, m_rawData, m_rawDataSize);

	// �s�̐擪�|�C���^���i�[
	DWORD lineCount = 0;
	m_pLines[lineCount++] = &m_lineParsedData[0];
	for (int index = 0; index < m_fileSize; index++)
	{
		if (m_lineParsedData[index] == '\n')
		{
			// ���s�R�[�h�̎��̕������擾
			m_pLines[lineCount++] = &m_lineParsedData[index + 1];

			// CRLF�̏ꍇ��CR�폜
			if (0 <= index - 1 && m_lineParsedData[index - 1] == '\r')
			{
				m_lineParsedData[index - 1] = '\0';
			}

			// LF�폜
			m_lineParsedData[index] = '\0';
		}
	}

	return TRUE;
}

BOOL CFileTool::Write(BYTE* pBuffer, DWORD bufferSize)
{
	// ReadFile���Ă���Ɩ����������݂ɂȂ�B

	// �t�@�C����������
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
