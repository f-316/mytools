#pragma once
#include <winsock2.h>
#include <vector>
#include <list>
#include <mutex>
#include <thread>

class TcpServer {
public:
    using ClientHander = void (*)(void* pInstance, const SOCKET clientSock, const BYTE* pData, DWORD dataSize);
    TcpServer();
    ~TcpServer();

    BOOL Start(u_short portNo);
    void ListenThread();
    void ClientHandleThread(SOCKET clientSock);
    void AddListener(void* pInstance, ClientHander handler);
    BOOL Send(const SOCKET clientSock, const BYTE* pData, DWORD size);
    void Stop();
    void Wait();

private:
    u_short m_portNo;
    DWORD m_recvBufferSize;
    SOCKET m_listenSock;
    std::list<SOCKET> m_clients;
    std::mutex m_clientsMutex;
    std::list<std::pair<SOCKET, std::unique_ptr<std::thread>>> m_clientHandleThreadPairs;
    std::thread m_listenThread;
    HANDLE m_hServiceExit;
    std::vector<std::pair<void*, ClientHander>> m_handlerPairs;
};
