// ����������
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <chrono>
#include <iostream>

#include "TcpClient.h"
#include "TcpAttrib.h"
#include "Log.h"

TcpClient::TcpClient()
    : m_sock(NULL)
    , m_recvBufferSize(4096)
    , m_hRecvThrExit(INVALID_HANDLE_VALUE)
{
    WSADATA wsaData;
    ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    m_hRecvThrExit = ::CreateEvent(
        NULL,   // default security attributes
        TRUE,   // manual-reset event
        FALSE,  // initial state is nonsignaled
        NULL    // object name
    );
}

TcpClient::~TcpClient() {
    ::WSACleanup();
    ::CloseHandle(m_hRecvThrExit);
}

/**
 * �T�[�o�ɐڑ����܂��B
 * @param[in] address
 * @param[in] portNo
 * @return TRUE/����
 */
BOOL TcpClient::Connect(std::string address, u_short portNo) {
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(portNo);
    ::inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr);

    // �ڑ����g���C���[�v
    while (true) {
        m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (::connect(m_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == 0) {
            std::cout << "[�ڑ�] �T�[�o�[�ɐڑ�����" << std::endl;
            break;
        }
        ::closesocket(m_sock);
        std::cout << "[�ҋ@] �Đڑ������݂܂�..." << std::endl;
        Sleep(1000);
    }

    AppLog::Trace(FUNCTRACEARGS, "Server,Connected");
    TcpAttrib::ShowSocketInfo(m_sock);
    m_recvThread = std::thread(&TcpClient::RecvThread, this);

    return TRUE;
}

/**
 * �T�[�o����ؒf
 */
void TcpClient::Disconnect() {
    if (m_hRecvThrExit != INVALID_HANDLE_VALUE) {
        ::SetEvent(m_hRecvThrExit);
        m_hRecvThrExit = INVALID_HANDLE_VALUE;
    }
    if (m_recvThread.joinable()) {
        m_recvThread.join();
    }
}

/**
 * �N���C�A���g�̎�M�n���h����ǉ�
 * @param[in] �N���X�̃C���X�^���X
 * @param[in] �n���h��
 */
void TcpClient::AddListener(void* pInstance, ClientHander handler)
{
    m_handlerPairs.push_back({pInstance, handler});
}

void TcpClient::RecvThread() {
    // �N���C�A���g�\�P�b�g��FD_READ���o�C���h
    WSAEVENT readEvent = ::WSACreateEvent();
    ::WSAEventSelect(m_sock, readEvent, FD_READ | FD_CLOSE);
    char* pBuffer = new char[m_recvBufferSize];
    HANDLE handles[2] = { m_hRecvThrExit, readEvent };
    while (true) {
        DWORD result = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);

        if (result == WAIT_OBJECT_0) {
            AppLog::Trace(FUNCTRACEARGS, "Exit");
            break;
        } else if (result == WAIT_OBJECT_0 + 1) {
            // �\�P�b�g�C�x���g
            WSANETWORKEVENTS netEvents;
            ::WSAEnumNetworkEvents(m_sock, readEvent, &netEvents);

            // �f�[�^��M
            if (netEvents.lNetworkEvents & FD_READ) {
                int recvSize = ::recv(m_sock, pBuffer, m_recvBufferSize, 0);
                if (recvSize <= 0) {
                    AppLog::Trace(FUNCTRACEARGS, "Client,Close,recvSize=%d", recvSize);
                    TcpAttrib::ShowSocketInfo(m_sock);
                    break;
                }
                AppLog::Trace(FUNCTRACEARGS, "Client,recvSize=%d", recvSize);

                // �n���h���֒ʒm
                for (auto& handlerPair: m_handlerPairs)
                {
                    handlerPair.second(handlerPair.first, m_sock, (BYTE*)pBuffer, recvSize);
                }
            }

            // Close��M
            if (netEvents.lNetworkEvents & FD_CLOSE) {
                AppLog::Trace(FUNCTRACEARGS, "Client,Close");
                TcpAttrib::ShowSocketInfo(m_sock);
                break;
            }
        }
    }
    delete[] pBuffer;

    ::closesocket(m_sock);
    ::CloseHandle(readEvent);
}

/**
 * �N���C�A���g�Ƀf�[�^�𑗐M
 * @param[in] �N���C�A���g�\�P�b�g
 * @param[in] ���M�f�[�^
 * @param[in] ���M�T�C�Y
 * @return TRUE/����
 */
BOOL TcpClient::Send(const BYTE* pData, DWORD size) {
    int sent = ::send(m_sock, (const char*)pData, size, 0);
    if (sent == SOCKET_ERROR) {
        AppLog::Error(FUNCTRACEARGS, ::WSAGetLastError());
        return FALSE;
    }

    return TRUE;
}
