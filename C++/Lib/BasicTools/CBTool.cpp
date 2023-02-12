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
	// �w�肵���`���̃f�[�^�����邩��₢���킹��
	UINT format = CF_HDROP;
	BOOL ret = ::IsClipboardFormatAvailable(format);
	if (ret == 0) {
		wprintf(L"%d,Format is not file.,0x%08X\n", __LINE__, GetLastError());
		return FALSE;
	}

	// �N���b�v�{�[�h�̃I�[�v��
	ret = ::OpenClipboard(NULL);
	if (ret == 0) {
		wprintf(L"%d,OpenClipboard failed.,0x%08X\n", __LINE__, GetLastError());
		return FALSE;
	}

	m_fileNameList.empty();
	BOOL normalEnd = TRUE;
	do
	{
		// �N���b�v�{�[�h�̃f�[�^�擾
		HANDLE hCB = ::GetClipboardData(format);
		if (hCB == NULL)
		{
			wprintf(L"%d,GetClipboardData failed.,0x%08X\n", __LINE__, GetLastError());
			normalEnd = FALSE;
			break;
		}

		// �t�@�C�����擾, UINT32_MAX��ݒ肷�邱�ƂŃt�@�C�������擾�ł���B
		UINT fileCount = DragQueryFileW((HDROP)hCB, UINT32_MAX, NULL, 0);
		if (fileCount == 0)
		{
			wprintf(L"%d,File count is 0.,0x%08X\n", __LINE__, GetLastError());
			normalEnd = FALSE;
			break;
		}

		// �t�@�C�����擾
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

	// �N���b�v�{�[�h�Ƃ���
	if (::CloseClipboard() == 0)
	{
		wprintf(L"%d,CloseClipboard failed.,0x%08X\n", __LINE__, GetLastError());
		normalEnd = FALSE;
	}

	return normalEnd;
}

// �N���b�v�{�[�h�Ƀe�L�X�g�f�[�^���R�s�[����֐�
BOOL FileNamesToCB::SetClipboardText(void) const
{
	UINT32 wstrByteCount = 0;
	for (std::vector<std::wstring>::const_iterator it = m_fileNameList.begin(); it != m_fileNameList.end(); it++) {
		wstrByteCount += (it->size() * sizeof(WCHAR)) + (wcslen(m_newLine) * sizeof(WCHAR));
	}
	//wprintf(L"size:%d\n", wstrByteCount);

	// �ړ��\�ȋ��L���������m�ۂ���
	HANDLE hMem = ::GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, wstrByteCount);
	if (!hMem)
	{
		return FALSE;
	}

	// �m�ۂ��������������b�N���C�A�N�Z�X�\�ɂ���
	BYTE* pMem = (BYTE*)GlobalLock(hMem);
	if (pMem == NULL)
	{
		return FALSE;
	}

	// �z����̃t�@�C�����������o���B
	UINT32 bufSize = wstrByteCount;
	wstrByteCount = 0;
	int byteSize = 0;
	for (std::vector<std::wstring>::const_iterator it = m_fileNameList.begin(); it != m_fileNameList.end(); it++) {
		// ������𕡎ʂ���
		byteSize = it->size() * sizeof(WCHAR);

		// �t�@�C����
		wprintf(L"%s\n", it->data());
		::memcpy_s(pMem + wstrByteCount, bufSize - wstrByteCount, it->data(), byteSize);
		wstrByteCount += byteSize;

		// ���s�R�[�h
		byteSize = wcslen(m_newLine) * sizeof(WCHAR);
		::memcpy_s(pMem + wstrByteCount, bufSize - wstrByteCount, m_newLine, byteSize);
		wstrByteCount += byteSize;
	}

	// �������̃��b�N����������
	::GlobalUnlock(hMem);
	if (::OpenClipboard(NULL))
	{
		// �N���b�v�{�[�h���̌Â��f�[�^���������
		::EmptyClipboard();
		::SetClipboardData(CF_UNICODETEXT, hMem); // �N���b�v�{�[�h�ɐV�����f�[�^����͂���
		::CloseClipboard();
	}

	return TRUE;
}