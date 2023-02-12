#include "pch.h"
#include "CBTool.h"

FileNamesToCB::FileNamesToCB()
{
	m_fileNameList.empty();
	SetNewLine(L"\r\n");
}

FileNamesToCB::~FileNamesToCB()
{
}

BOOL FileNamesToCB::Start()
{
	if (!GetFileNamesFromClip())
	{
		return FALSE;
	}

	if (!SetClipboardText())
	{
		return FALSE;
	}

	return TRUE;
}

void FileNamesToCB::SetNewLine(LPCWSTR newLine)
{
	wcscpy_s(m_newLine, sizeof(m_newLine), newLine);
}

BOOL FileNamesToCB::GetFileNamesFromClip(void)
{
	// 指定した形式のデータがあるかを問い合わせる
	UINT format = CF_HDROP;
	BOOL ret = ::IsClipboardFormatAvailable(format);
	if (ret == 0) {
		wprintf(L"%d,Format is not file.,0x%08X\n", __LINE__, GetLastError());
		return FALSE;
	}

	// クリップボードのオープン
	ret = ::OpenClipboard(NULL);
	if (ret == 0) {
		wprintf(L"%d,OpenClipboard failed.,0x%08X\n", __LINE__, GetLastError());
		return FALSE;
	}

	m_fileNameList.empty();
	BOOL normalEnd = TRUE;
	do
	{
		// クリップボードのデータ取得
		HANDLE hCB = ::GetClipboardData(format);
		if (hCB == NULL)
		{
			wprintf(L"%d,GetClipboardData failed.,0x%08X\n", __LINE__, GetLastError());
			normalEnd = FALSE;
			break;
		}

		// ファイル数取得, UINT32_MAXを設定することでファイル数が取得できる。
		UINT fileCount = DragQueryFileW((HDROP)hCB, UINT32_MAX, NULL, 0);
		if (fileCount == 0)
		{
			wprintf(L"%d,File count is 0.,0x%08X\n", __LINE__, GetLastError());
			normalEnd = FALSE;
			break;
		}

		// ファイル名取得
		WCHAR lpszFile[MAX_PATH];
		for (int index = 0; index < fileCount; index++)
		{
			ret = DragQueryFileW((HDROP)hCB, index, lpszFile, MAX_PATH);
			if (ret == 0)
			{
				wprintf(L"%d,DragQueryFileW failed,0x%08X\n", __LINE__, GetLastError());
				normalEnd = FALSE;
				break;
			}
			std::wstring pBuf(lpszFile);
			m_fileNameList.push_back(pBuf);
		}

	} while (0);

	// クリップボードとじる
	if (::CloseClipboard() == 0)
	{
		wprintf(L"%d,CloseClipboard failed.,0x%08X\n", __LINE__, GetLastError());
		normalEnd = FALSE;
	}

	return normalEnd;
}

// クリップボードにテキストデータをコピーする関数
BOOL FileNamesToCB::SetClipboardText(void) const
{
	UINT32 wstrByteCount = 0;
	for (std::vector<std::wstring>::const_iterator it = m_fileNameList.begin(); it != m_fileNameList.end(); it++) {
		wstrByteCount += (it->size() * sizeof(WCHAR)) + (wcslen(m_newLine) * sizeof(WCHAR));
	}
	//wprintf(L"size:%d\n", wstrByteCount);

	// 移動可能な共有メモリを確保する
	HANDLE hMem = ::GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, wstrByteCount);
	if (!hMem)
	{
		return FALSE;
	}

	// 確保したメモリをロックし，アクセス可能にする
	BYTE* pMem = (BYTE*)GlobalLock(hMem);
	if (pMem == NULL)
	{
		return FALSE;
	}

	// 配列内のファイル名を書き出し。
	UINT32 bufSize = wstrByteCount;
	wstrByteCount = 0;
	int byteSize = 0;
	for (std::vector<std::wstring>::const_iterator it = m_fileNameList.begin(); it != m_fileNameList.end(); it++) {
		// 文字列を複写する
		byteSize = it->size() * sizeof(WCHAR);

		// ファイル名
		wprintf(L"%s\n", it->data());
		::memcpy_s(pMem + wstrByteCount, bufSize - wstrByteCount, it->data(), byteSize);
		wstrByteCount += byteSize;

		// 改行コード
		byteSize = wcslen(m_newLine) * sizeof(WCHAR);
		::memcpy_s(pMem + wstrByteCount, bufSize - wstrByteCount, m_newLine, byteSize);
		wstrByteCount += byteSize;
	}

	// メモリのロックを解除する
	::GlobalUnlock(hMem);
	if (::OpenClipboard(NULL))
	{
		// クリップボード内の古いデータを解放する
		::EmptyClipboard();
		::SetClipboardData(CF_UNICODETEXT, hMem); // クリップボードに新しいデータを入力する
		::CloseClipboard();
	}

	return TRUE;
}