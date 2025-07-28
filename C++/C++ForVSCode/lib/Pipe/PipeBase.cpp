// ‚ ‚¢‚¤‚¦‚¨
#include "PipeBase.h"
#include <chrono>

PipeBase::PipeBase(const char* pRecvLocalPipeName, const char* pSendLocalPipeName)
    : m_maxInstance(1)
    , m_recvBufferSize(1024 * 1024)
    , m_sendBufferSize(1024 * 1024)
    , m_hPipeRecv(INVALID_HANDLE_VALUE)
    , m_hPipeSend(INVALID_HANDLE_VALUE)
    , m_hRecvThread(INVALID_HANDLE_VALUE)
    , m_hRecvThrEnd(INVALID_HANDLE_VALUE)
    , m_hExitRecvThr(INVALID_HANDLE_VALUE)
    , m_callback(NULL)
{
    ::memset(m_recvPipeName, 0, sizeof(m_recvPipeName));
    ::memset(m_sendPipeName, 0, sizeof(m_sendPipeName));
    m_pRecvBuffer = new BYTE[m_recvBufferSize];
    m_pSendBuffer = new BYTE[m_sendBufferSize];

    m_hRecvThrEnd = ::CreateEvent( 
        NULL,         // default security attributes
        TRUE,        // manual-reset event
        FALSE,        // initial state is nonsignaled
        NULL  // object name
    );
    m_hExitRecvThr = ::CreateEvent( 
        NULL,         // default security attributes
        TRUE,        // manual-reset event
        FALSE,        // initial state is nonsignaled
        NULL  // object name
    );

    SetRecvLocalPipeName(pRecvLocalPipeName);
    SetSendLocalPipeName(pSendLocalPipeName);
}

PipeBase::~PipeBase()
{
    if (m_pRecvBuffer != NULL)
    {
        delete[] m_pRecvBuffer;
        m_pRecvBuffer = NULL;
    }

    if (m_pSendBuffer != NULL)
    {
        delete[] m_pSendBuffer;
        m_pSendBuffer = NULL;
    }

    if (m_hRecvThread != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_hRecvThread);
        m_hRecvThread = INVALID_HANDLE_VALUE;
    }

    if (m_hRecvThrEnd != INVALID_HANDLE_VALUE)
    {
        ::SetEvent(m_hRecvThrEnd);
        ::CloseHandle(m_hRecvThrEnd);
        m_hRecvThrEnd = INVALID_HANDLE_VALUE;
    }
    if (m_hExitRecvThr != INVALID_HANDLE_VALUE)
    {
        ::SetEvent(m_hExitRecvThr);
        ::CloseHandle(m_hExitRecvThr);
        m_hExitRecvThr = INVALID_HANDLE_VALUE;
    }

    m_recvThread.join();
}

BOOL PipeBase::Start()
{
    m_recvThread = std::thread(&PipeBase::RecvThreadProc, this);
    return TRUE;
}

void PipeBase::SetRecvLocalPipeName(const char* pPipeName)
{
    ::sprintf_s(m_recvPipeName, sizeof(m_recvPipeName), "%s%s", LOCALPIPE_PREFIX, pPipeName);
}
void PipeBase::SetSendLocalPipeName(const char* pPipeName)
{
    ::sprintf_s(m_sendPipeName, sizeof(m_sendPipeName), "%s%s", LOCALPIPE_PREFIX, pPipeName);
}

BOOL PipeBase::Connect()
{
    if (m_sendPipeName[0] == '\0')
    {
        return FALSE;
    }
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    long long elapsedInMs = 0;

    AppLog::Trace(FUNCTRACEARGS, "Start, Connect to: %s", m_sendPipeName);
    while (1)
    { 
        m_hPipeSend = ::CreateFileA( 
            m_sendPipeName,   // pipe name 
            GENERIC_WRITE, 
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 
    
        // Break if the pipe handle is valid. 
        if (m_hPipeSend != INVALID_HANDLE_VALUE)
        {
            // success
            break;
        }
    
        // Exit if an error other than ERROR_PIPE_BUSY occurs. 
        switch (::GetLastError())
        {
        case ERROR_PIPE_BUSY:
            // All pipe instances are busy
            if (!::WaitNamedPipeA(m_sendPipeName, 5 * 1000))
            { 
                AppLog::Trace(FUNCTRACEARGS, "Could not open pipe: 5 second wait timed out.");
            }
            continue;
            break;
        
        case ERROR_FILE_NOT_FOUND:
            break;

        default:
            printf("[%d]Could not open pipe. GLE=%d\n", __LINE__, ::GetLastError() );
            AppLog::Error(FUNCTRACEARGS, ::GetLastError());
            return FALSE;
        }

        end = std::chrono::steady_clock::now();
        elapsedInMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if (5 * 1000 < elapsedInMs)
        {
            start = std::chrono::steady_clock::now();
            AppLog::Trace(FUNCTRACEARGS, "Connecting...,to: %s\n", m_sendPipeName);
        }

        Sleep(1 * 1000);
    }

    return TRUE;
}

void PipeBase::AddListener(void (*callback)(const BYTE* pData, DWORD dataSize))
{
    m_callback = callback;
}

BOOL PipeBase::Send(const BYTE* pData, DWORD size)
{
    DWORD bytesWritten;
    if (!::WriteFile(m_hPipeSend, pData, size, &bytesWritten, NULL))
    {
        AppLog::Error(FUNCTRACEARGS, ::GetLastError());
        return FALSE;
    }
    else
    {
        // printf("[%d]WriteFile Success,bytesWritten=%lu\n", __LINE__, bytesWritten);
    }

    return TRUE;
}

void PipeBase::Close()
{
    if (m_hExitRecvThr != INVALID_HANDLE_VALUE)
    {
        ::SetEvent(m_hExitRecvThr);
    }
    if (m_hPipeSend != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_hPipeSend);
        m_hPipeSend = INVALID_HANDLE_VALUE;
    }
}

void PipeBase::Wait()
{
    ::WaitForSingleObject(m_hRecvThrEnd, INFINITE);
}

BOOL PipeBase::Open()
{
    if (m_recvPipeName[0] == '\0')
    {
        return FALSE;
    }

    const char* PIPE_TO_SERVER = R"(\\.\pipe\ToServer)";

    m_hPipeRecv = ::CreateNamedPipeA(
        m_recvPipeName,
        PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        m_maxInstance,               // max instances = 1
        m_recvBufferSize,
        m_recvBufferSize,
        0,
        nullptr
    );

    if (m_hPipeRecv == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    if (!::ConnectNamedPipe(m_hPipeRecv, nullptr)) {
        if (::GetLastError() != ERROR_PIPE_CONNECTED) {
            AppLog::Error(FUNCTRACEARGS, ::GetLastError());
            return FALSE;
        } else {
            // printf("%s[%d][Server] Client already connected.\n", __FILE__, __LINE__);
        }
    }

    return TRUE;
}

void PipeBase::RecvThreadProc()
{
    if (!Open())
    {
        return;
    }

    OVERLAPPED ov = {};
    ov.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

    HANDLE events[] = { m_hExitRecvThr, ov.hEvent };
    DWORD dwWaitResult;
    BOOL readResult = false;
    BOOL writeResult = false;
    DWORD bytesRead = 0;
    double elapsedUSec = 0;
    ::ResetEvent(m_hExitRecvThr);
    while (true) {
        ::ResetEvent(ov.hEvent);

        readResult = ::ReadFile(m_hPipeRecv, m_pRecvBuffer, m_recvBufferSize, &bytesRead, &ov);
        if (!readResult && ::GetLastError() != ERROR_IO_PENDING) {
            AppLog::Error(FUNCTRACEARGS, ::GetLastError());
            break;
        }

        dwWaitResult = ::WaitForMultipleObjects(_countof(events), events, FALSE, INFINITE);
        if (dwWaitResult == WAIT_OBJECT_0) {
            printf("%s[%d]Exit\n", __FILE__, __LINE__);
            ::CancelIo(m_hPipeRecv);
            break;
        }
        else if (dwWaitResult == WAIT_OBJECT_0 + 1) {
            if (GetOverlappedResult(m_hPipeRecv, &ov, &bytesRead, FALSE))
            {
                if (m_callback)
                {
                    m_callback(m_pRecvBuffer, bytesRead);
                }
            }
            else if (::GetLastError() == ERROR_BROKEN_PIPE) {
                AppLog::Error(FUNCTRACEARGS, ::GetLastError());
                printf("%s[%d]Pipe closed.\n", __FILE__, __LINE__);
                break;
            }
            else {
                AppLog::Error(FUNCTRACEARGS, ::GetLastError());
                break;
            }
        }
    }

    ::FlushFileBuffers(m_hPipeRecv);
    ::DisconnectNamedPipe(m_hPipeRecv);
    ::CloseHandle(ov.hEvent);
    ::CloseHandle(m_hPipeRecv);
    AppLog::Trace(FUNCTRACEARGS, "RecvThread, End");
    ::SetEvent(m_hRecvThrEnd);
}
