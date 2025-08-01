#pragma once
#include <winsock2.h>
#include <vector>
#include <list>
#include <mutex>
#include <thread>

class TcpClient {
public:
    using ClientHander = void (*)(void* pInstance, const SOCKET clientSock, const BYTE* pData, DWORD dataSize);
    TcpClient();
    ~TcpClient();

    BOOL Connect(std::string address, u_short portNo);
    void Disconnect();
    void AddListener(void* pInstance, ClientHander handler);
    void RecvThread();
    BOOL Send(const BYTE* pData, DWORD size);

private:
    SOCKET m_sock;
    DWORD m_recvBufferSize;
    std::thread m_recvThread;
    HANDLE m_hRecvThrExit;
    std::vector<std::pair<void*, ClientHander>> m_handlerPairs;
};
