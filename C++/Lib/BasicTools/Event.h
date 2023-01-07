#pragma once
#include <windows.h>

class __declspec(dllexport) CEvent
{
public:
	CEvent(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE);
	~CEvent();

	const HANDLE& Handle(void) { return m_handle; }
	void Set(void);
	void Reset(void);

private:
	HANDLE m_handle;
};
