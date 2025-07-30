#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <list>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

std::list<SOCKET> g_clients;
std::mutex g_mutex;

void HandleClient(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        int ret = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (ret <= 0) break;

        std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "[��M] �T�C�Y: " << ret << " bytes" << std::endl;
    }

    std::lock_guard<std::mutex> lock(g_mutex);
    closesocket(clientSocket);
    g_clients.remove(clientSocket);
    std::cout << "[�ؒf] �N���C�A���g�\�P�b�g�폜" << std::endl;
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(listenSock, SOMAXCONN);

    std::cout << "[�N��] �T�[�o�[�ҋ@��..." << std::endl;

    while (true) {
        SOCKET clientSock = accept(listenSock, nullptr, nullptr);
        if (clientSock == INVALID_SOCKET) continue;

        std::lock_guard<std::mutex> lock(g_mutex);
        g_clients.push_back(clientSock);
        std::thread(HandleClient, clientSock).detach();
        std::cout << "[�ڑ�] �N���C�A���g�ǉ�" << std::endl;
    }

    closesocket(listenSock);
    WSACleanup();
    return 0;
}
