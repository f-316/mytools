#pragma once

#include "NetTools.h"
#include "BasicTools.h"
#include <iostream>

class MainApp
{
public:
	MainApp();
	~MainApp();
	BOOL Start(void);
	BOOL WaitEnd(void);

protected:
	CAsyncTask m_inputTask;
	CAsyncTask m_procTask;
	CAsyncTask m_serverTask;
	CEvent m_cmdEvt;
	CEvent m_serverEvt;
	CEvent m_exitEvt;

	int m_count;
	std::string m_cmd;

	static DWORD WINAPI InputThread(LPVOID pThis);
	void InputTask(void);

	static DWORD WINAPI ProcThread(LPVOID pThis);
	void ProcTask(void);

	static DWORD WINAPI ServerThread(LPVOID pThis);
	void ServerTask(void);

};

