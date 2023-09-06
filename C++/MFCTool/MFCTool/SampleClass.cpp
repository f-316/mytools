#include "pch.h"
#include "SampleClass.h"


CSampleClass::CSampleClass()
{
}

CSampleClass::~CSampleClass()
{
}

void CSampleClass::SampleMethod(int a, int b)
{
	CDC cdc;
}

/**
* 
*/
void CSampleClass::SampleMethod2(int a, int b)
{
	// TODO: ここに実装コードを追加します.
	CString str;
	str.Format(_T("a=%d,b=%d"), a, b);
	OutputDebugString(_T("SampleMethod\n"));
	TRACE("\n == %S ==\n", str.GetString());
	str.Delete(1, 5);
	TRACE("\n == %S ==\n", str.GetString());

	CFile file;
	CString dirPath(_T("C:\\gitrepogitory\\mytools\\C++\\"));
	CString fileName(_T("hoge.txt"));
	CString filePath;
	filePath = dirPath + fileName;
	char pBuf[100] = { 0 };
	if (file.Open(filePath.GetString(),
		CFile::modeCreate |
		CFile::modeNoTruncate | // ファイルがない場合作成はmodeCreateが必須
		CFile::shareDenyWrite |
		CFile::modeReadWrite
	))
	//if (file.Open(filePath.GetString(), CFile::modeNoTruncate))
	{
		file.Write(fileName.GetString(), fileName.GetLength() * sizeof(TCHAR));
		file.Read(pBuf, fileName.GetLength() * sizeof(TCHAR));
		int a = file.GetLength();
		file.Close();
	}
	CFileStatus fileSts;
	file.GetStatus(fileSts);
	CFile::GetStatus(filePath.GetString(), fileSts);
	//COleDateTime curTime = COleDateTime::GetCurrentTime();
	//CFile::Rename(filePath.GetString(), filePath + _T("_") + curTime.Format(_T("%Y%m%d_%H%M%S")));

	COleDateTime time;
	if (time.ParseDateTime(_T("2023/2/28")))
	{
		TRACE("\n%S", time.Format(_T("%Y%m%d_%H%M%S")));
		COleDateTimeSpan span;
		span.SetDateTimeSpan(1, 0, 0, 0);
		time += span;
		TRACE("\n%S", time.Format(_T("%Y%m%d_%H%M%S")));
		time += span;
		TRACE("\n%S", time.Format(_T("%Y%m%d_%H%M%S")));
	}
}
