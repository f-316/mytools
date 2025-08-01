// ����������
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <chrono>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#include "TcpServer.h"
#include "TcpAttrib.h"
#include "Log.h"

TcpServer::TcpServer()
    : m_listenSock(NULL)
    , m_portNo(0)
    , m_recvBufferSize(4096)
    , m_hServiceExit(INVALID_HANDLE_VALUE)
{
    WSADATA wsaData;
    ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    m_hServiceExit = ::CreateEvent(
        NULL,   // default security attributes
        TRUE,   // manual-reset event
        FALSE,  // initial state is nonsignaled
        NULL    // object name
    );
}

TcpServer::~TcpServer() {
    ::WSACleanup();
    ::CloseHandle(m_hServiceExit);
}

/**
 * �T�[�o���J�n���܂��B
 * @param[in] portNo
 * @return TRUE/����
 */
BOOL TcpServer::Start(u_short portNo) {
    m_portNo = portNo;
    m_listenThread = std::thread(&TcpServer::ListenThread, this);
    return TRUE;
}

/**
 * Listen�X���b�h
 */
void TcpServer::ListenThread() {
    // TCP�T�[�o�N��
    m_listenSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(m_portNo);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    ::bind(m_listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    ::listen(m_listenSock, SOMAXCONN);

    // accept���u���b�L���O�ɂ��邽�߂�ACCEPT�C�x���g�ݒ�
    WSAEVENT sockEvent = ::WSACreateEvent();
    ::WSAEventSelect(m_listenSock, sockEvent, FD_ACCEPT);
    HANDLE events[2] = { m_hServiceExit, sockEvent };
    
    // �ڑ��҂�
    while (true) {
        AppLog::Trace(FUNCTRACEARGS, "port:%u,Listening...", m_portNo);
        DWORD waitResult = ::WaitForMultipleObjects(_countof(events), events, FALSE, INFINITE);
        if (waitResult == WAIT_OBJECT_0) {
            AppLog::Trace(FUNCTRACEARGS, "Exit");
            break;
        } else if (waitResult == WAIT_OBJECT_0 + 1) {
            // �\�P�b�g�C�x���g����
            WSANETWORKEVENTS netEvents;
            ::WSAEnumNetworkEvents(m_listenSock, sockEvent, &netEvents);

            if (netEvents.lNetworkEvents & FD_ACCEPT) {
                SOCKET clientSock = ::accept(m_listenSock, nullptr, nullptr);
                if (clientSock == INVALID_SOCKET) continue;

                // �N���C�A���g�Z�b�V�����쐬
                std::lock_guard<std::mutex> lock(m_clientsMutex);
                m_clients.push_back(clientSock);
                m_clientHandleThreadPairs.push_back({clientSock, std::make_unique<std::thread>(&TcpServer::ClientHandleThread, this, clientSock)});
                AppLog::Trace(FUNCTRACEARGS, "Client,ACCEPT");
                TcpAttrib::ShowSocketInfo(clientSock);
            }
        }
    }

    ::CloseHandle(sockEvent);
    AppLog::Trace(FUNCTRACEARGS, "End");
}

/**
 * �ڑ����ꂽ�N���C�A���g�C�x���g���n���h�����O���܂��B
 * @param[in] clientSock
 * @return none
 */
void TcpServer::ClientHandleThread(SOCKET clientSock) {
    
    // �N���C�A���g�\�P�b�g��FD_READ���o�C���h
    WSAEVENT readEvent = ::WSACreateEvent();
    ::WSAEventSelect(clientSock, readEvent, FD_READ | FD_CLOSE);
    char* pBuffer = new char[m_recvBufferSize];
    HANDLE handles[2] = { m_hServiceExit, readEvent };
    while (true) {
        DWORD result = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);

        if (result == WAIT_OBJECT_0) {
            AppLog::Trace(FUNCTRACEARGS, "Exit");
            break;
        } else if (result == WAIT_OBJECT_0 + 1) {
            // �\�P�b�g�C�x���g
            WSANETWORKEVENTS netEvents;
            ::WSAEnumNetworkEvents(clientSock, readEvent, &netEvents);

            // �f�[�^��M
            if (netEvents.lNetworkEvents & FD_READ) {
                int recvSize = ::recv(clientSock, pBuffer, m_recvBufferSize, 0);
                if (recvSize <= 0) {
                    AppLog::Trace(FUNCTRACEARGS, "Client,Close,recvSize=%d", recvSize);
                    TcpAttrib::ShowSocketInfo(clientSock);
                    break;
                }

                // �n���h���֒ʒm
                for (auto& handlerPair: m_handlerPairs)
                {
                    handlerPair.second(handlerPair.first, clientSock, (BYTE*)pBuffer, recvSize);
                }
            }

            // Close��M
            if (netEvents.lNetworkEvents & FD_CLOSE) {
                AppLog::Trace(FUNCTRACEARGS, "Client,Close");
                TcpAttrib::ShowSocketInfo(clientSock);
                break;
            }
        }
    }
    delete[] pBuffer;

    std::lock_guard<std::mutex> lock(m_clientsMutex);
    for (auto& clientHandleThreadPair : m_clientHandleThreadPairs)
    {
        if (clientHandleThreadPair.first == clientSock)
        {
            clientHandleThreadPair.second->detach();
            m_clientHandleThreadPairs.remove(clientHandleThreadPair);
            break;
        }
    }
    ::closesocket(clientSock);
    m_clients.remove(clientSock);

    ::CloseHandle(readEvent);
}

/**
 * �N���C�A���g�̎�M�n���h����ǉ�
 * @param[in] �N���X�̃C���X�^���X
 * @param[in] �n���h��
 */
void TcpServer::AddListener(void* pInstance, ClientHander handler)
{
    m_handlerPairs.push_back({pInstance, handler});
}

/**
 * �N���C�A���g�Ƀf�[�^�𑗐M
 * @param[in] �N���C�A���g�\�P�b�g
 * @param[in] ���M�f�[�^
 * @param[in] ���M�T�C�Y
 * @return TRUE/����
 */
BOOL TcpServer::Send(const SOCKET clientSock, const BYTE* pData, DWORD size) {
    int sent = ::send(clientSock, (const char*)pData, size, 0);
    if (sent == SOCKET_ERROR) {
        AppLog::Error(FUNCTRACEARGS, ::WSAGetLastError());
        return FALSE;
    }

    return TRUE;
}

/**
 * �T�[�o���~
 */
void TcpServer::Stop() {
    if (m_hServiceExit != INVALID_HANDLE_VALUE) {
        ::SetEvent(m_hServiceExit);
        m_hServiceExit = INVALID_HANDLE_VALUE;
    }
}

/**
 * �T�[�o�I����҂��܂��B
 */
void TcpServer::Wait() {
    if (m_listenThread.joinable()) {
        m_listenThread.join();
    }
    for (auto& clientHandleThreadPair : m_clientHandleThreadPairs)
    {
        if (clientHandleThreadPair.second->joinable())
        {
            clientHandleThreadPair.second->join();
        }
    }
    ::closesocket(m_listenSock);
    m_listenSock = NULL;
}
