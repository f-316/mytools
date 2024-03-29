#pragma once
#ifndef __FileOpr__
#define __FileOpr__

#include <iostream>
#include <fstream>
#include <string>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

class __declspec(dllexport) CFileTool
{
public:
	CFileTool(std::string filePath);
	CFileTool(void);
	virtual ~CFileTool(void);

	void Sample(void);
	void SampleRead(void);
	void SampleWrite(void);
	BOOL Open(BOOL forceCreate = FALSE, BOOL addMode = FALSE);
	void Close(void);
	void SetFilePath(std::string filePath);

	DWORD GetSize(void);
	BOOL Read(void);
	DWORD GetLineCount(void);
	BOOL ParseLineText(void);

	BOOL Write(BYTE* pBuffer, DWORD bufferSize);

	// StringTools
	void UTF8toUTF16(const BYTE* pStr8, DWORD byteLen, char16_t* pStr16, DWORD pStr16BufSize, DWORD& str16Len);
	void UTF16toUTF8(const char16_t* pStr16, DWORD str16Len, BYTE* pStr8, DWORD pStr8BufSize, DWORD& byteLen);

protected:
	std::string m_filePath;
	HANDLE m_hFile;
	DWORD m_fileSize;
	BYTE* m_rawData;
	DWORD m_rawDataSize;
	BYTE* m_lineParsedData;
	DWORD m_lineParsedDataSize;
	BYTE** m_pLines;
	DWORD m_lineCount;

	BOOL IsOpen(void) const { return m_hFile != INVALID_HANDLE_VALUE; }
};

#endif // !__FileOpr__
