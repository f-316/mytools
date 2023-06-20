#pragma once
#ifndef __CBTool__
#define __CBTool__

#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <shellapi.h>

class __declspec(dllexport) FileNamesToCB
{
public:
	FileNamesToCB();
	~FileNamesToCB();
	BOOL Start();
	void SetNewLine(LPCWSTR newLine);
private:
	BOOL GetFileNamesFromClip(void);
	BOOL SetClipboardText(void) const;

	std::vector<std::wstring> m_fileNameList;
	WCHAR m_newLine[3];
};

#endif // !__CBTool__
