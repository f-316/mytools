#include "pch.h"
#include "FileOpr.h"

CFile::CFile(void)
{
}
CFile::~CFile(void)
{
}

int CFile::Parse(const char* filePath)
{
	HANDLE hFile;

	//hFile = CreateFile(
	//	TEXT("test.txt"), GENERIC_READ, 0, NULL,
	//	CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL
	//);
	hFile = CreateFile(
		TEXT("C:\\C++\console\\ParseImage\\ParseImage\\test.txt"), GENERIC_READ, 0, NULL,
		CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(
			NULL, TEXT("ファイルを作成できませんでした。"),
			TEXT("エラー"), MB_OK
		);
		return 1;
	}

	CloseHandle(hFile);

	//CFile file(_T("test.txt"), CFile::modeRead);
	//intf("file:%d\n", file.GetFilePath());
	//CFile file(_T("..\\ParseImage\\resouce\\dot_sample_red.bmp"), CFile::modeRead);
	//char filepath[] = "dot_sample_red.bmp";
	//ImageParser imgParser;
	//imgParser.Parse();
}
