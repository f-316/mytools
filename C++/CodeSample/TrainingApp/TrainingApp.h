#include <iostream>
#include <string>
#include <algorithm>
#include <windows.h>
#include "BasicTools.h"

class CTrainingApp
{
public:
    CTrainingApp(void);
    ~CTrainingApp(void);
    BOOL Start(void);
    BOOL WaitEnd(void);

private:
    CAsyncTask m_inputTask;
    CAsyncTask m_procTask;
    CEvent m_cmdEvt;
    CEvent m_exitEvt;

    std::string m_cmd;
    int m_count;

    static DWORD WINAPI InputThread(LPVOID pThis);
    void InputTask(void);

    static DWORD WINAPI ProcThread(LPVOID pThis);
    void ProcTask(void);
};
