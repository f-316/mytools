#pragma once
#include <stdio.h>
#include <windows.h>
#include <thread>
#include "Log.h"

class PipeBase
{
private:
    int m_maxInstance;
    DWORD m_recvBufferSize;
    DWORD m_sendBufferSize;
    BYTE* m_pRecvBuffer;
    BYTE* m_pSendBuffer;
    HANDLE m_hPipeRecv;
    HANDLE m_hPipeSend;
    HANDLE m_hRecvThread;
    HANDLE m_hRecvThrEnd;
    HANDLE m_hExitRecvThr;
    void (*m_callback)(const BYTE* pData, DWORD dataSize);
    char m_recvPipeName[MAX_PATH];
    char m_sendPipeName[MAX_PATH];
    std::thread m_recvThread;
    static constexpr char* LOCALPIPE_PREFIX = R"(\\.\pipe\)";

public:
    PipeBase(const char* pRecvLocalPipeName, const char* pSendLocalPipeName);
    ~PipeBase();

    BOOL Start();
    void SetRecvLocalPipeName(const char* pPipeName);
    void SetSendLocalPipeName(const char* pPipeName);
    BOOL Connect();
    void AddListener(void (*callback)(const BYTE* pData, DWORD dataSize));
    BOOL Send(const BYTE* pData, DWORD size);
    void Close();
    void Wait();
    BOOL Open();
    void RecvThreadProc();
};
