// TrainingApp.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "TrainingApp.h"

CTrainingApp::CTrainingApp()
    : m_count(0)
    , m_cmd("")
    , m_cmdEvt()
    , m_exitEvt(TRUE)
{
}

CTrainingApp::~CTrainingApp()
{
}

BOOL CTrainingApp::Start(void)
{
    // ToDo: ファイル入出力
    std::string filePath = "C:\\tmp\\test.txt";
    CFileTool settingFile(filePath);
    if (!settingFile.ParseLineText())
    {
        CAppLog::Trace(FUNCTRACEARGS, "ParseLineText failed.");
        return FALSE;
    }

    // ToDo: マルチスレッド
    // ToDo: Inputスレッド
    m_inputTask.Start(InputThread, this);
    m_procTask.Start(ProcThread, this);

    // ToDo: TCP通信、Client、Server

    return TRUE;
}

BOOL CTrainingApp::WaitEnd(void)
{
    m_procTask.Wait();
    m_inputTask.Wait();
    CAppLog::Trace(FUNCTRACEARGS, "App End.");

    return TRUE;
}

DWORD CTrainingApp::InputThread(LPVOID pThis)
{
    reinterpret_cast <CTrainingApp*>(pThis)->InputTask();
    return 0;
}

void CTrainingApp::InputTask(void)
{
    CAppLog::Trace(FUNCTRACEARGS, "command wait...");
    while (1)
    {
        std::cin >> m_cmd;
        CAppLog::Trace(FUNCTRACEARGS, "command wait...");
        m_cmdEvt.Set();
        if (m_cmd.compare("exit") == 0)
        {
            m_exitEvt.Set();
        }
        if (::WaitForSingleObject(m_exitEvt.Handle(), 0) == WAIT_OBJECT_0)
        {
            break;
        }
    };

}

DWORD CTrainingApp::ProcThread(LPVOID pThis)
{
    reinterpret_cast <CTrainingApp*>(pThis)->ProcTask();
    return 0;
}

void CTrainingApp::ProcTask(void)
{
    CFuncTrace ftrace(FUNCTRACEARGS, "");
    const HANDLE hWait[/*MAXIMUM_WAIT_OBJECTS*/] = {m_exitEvt.Handle() , m_cmdEvt.Handle()};
    DWORD waitRet;
    while (1)
    {
        waitRet = ::WaitForMultipleObjects(_countof(hWait), hWait, FALSE, INFINITE);
        if (waitRet == WAIT_OBJECT_0 + 1)
        {
            ftrace.Trace(__LINE__, m_cmd.c_str());
        }
        else if (waitRet == WAIT_OBJECT_0 + 0)
        {
            ftrace.Trace(__LINE__, "Exit");
            break;
        }
    }
}
