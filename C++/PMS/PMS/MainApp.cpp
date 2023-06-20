#include "MainApp.h"

MainApp::MainApp()
    : m_count(0)
    , m_cmd("")
    , m_cmdEvt()
    , m_serverEvt()
    , m_exitEvt(TRUE)
{
}

MainApp::~MainApp()
{
}

BOOL MainApp::Start(void)
{
    CFuncTrace ftrace(FUNCTRACEARGS, "Main");

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
    Sleep(100);
    m_procTask.Start(ProcThread, this);
    Sleep(100);
    m_serverTask.Start(ServerThread, this);

    // ToDo: TCP通信、Client、Server

    return TRUE;
}

BOOL MainApp::WaitEnd(void)
{
    m_procTask.Wait();
    m_inputTask.Wait();
    m_serverTask.Wait();

    return TRUE;
}

DWORD MainApp::InputThread(LPVOID pThis)
{
    reinterpret_cast <MainApp*>(pThis)->InputTask();
    return 0;
}

void MainApp::InputTask(void)
{
    while (1)
    {
        std::cin >> m_cmd;
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
    CAppLog::Trace(FUNCTRACEARGS, "End.");
}

DWORD MainApp::ProcThread(LPVOID pThis)
{
    reinterpret_cast <MainApp*>(pThis)->ProcTask();
    return 0;
}

void MainApp::ProcTask(void)
{
    const HANDLE hWait[/*MAXIMUM_WAIT_OBJECTS*/] = { m_exitEvt.Handle() , m_cmdEvt.Handle() };
    DWORD waitRet;
    while (1)
    {
        waitRet = ::WaitForMultipleObjects(_countof(hWait), hWait, FALSE, INFINITE);
        // 終了待ち
        if (waitRet == WAIT_OBJECT_0 + 0)
        {
            break;
        }
        // コマンド待ち
        else if (waitRet == WAIT_OBJECT_0 + 1)
        {
            // コマンド処理
            CAppLog::Trace(FUNCTRACEARGS, m_cmd.c_str());
        }
    }
    CAppLog::Trace(FUNCTRACEARGS, "End.");
}

DWORD MainApp::ServerThread(LPVOID pThis)
{
    reinterpret_cast <MainApp*>(pThis)->ServerTask();
    return 0;
}

void MainApp::ServerTask(void)
{
    // ToDo: サーバ起動
    const HANDLE hWait[/*MAXIMUM_WAIT_OBJECTS*/] = { m_exitEvt.Handle() , m_serverEvt.Handle() };
    DWORD waitRet;
    while (1)
    {
        waitRet = ::WaitForMultipleObjects(_countof(hWait), hWait, FALSE, INFINITE);
        // 終了待ち
        if (waitRet == WAIT_OBJECT_0 + 0)
        {
            break;
        }
        // コマンド待ち
        else if (waitRet == WAIT_OBJECT_0 + 1)
        {
            // コマンド処理
            CAppLog::Trace(FUNCTRACEARGS, m_cmd.c_str());
        }
    }
    CAppLog::Trace(FUNCTRACEARGS, "End.");
}