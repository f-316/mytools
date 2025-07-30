#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock;
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // 接続リトライループ
    while (true) {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == 0) {
            std::cout << "[接続] サーバーに接続成功" << std::endl;
            break;
        }
        closesocket(sock);
        std::cout << "[待機] 再接続を試みます..." << std::endl;
        Sleep(1000);
    }

    std::string line;
    while (true) {
        std::getline(std::cin, line);
        if (line == "close") break;
        send(sock, line.c_str(), (int)line.size(), 0);
    }

    closesocket(sock);
    std::cout << "[終了] 切断しました" << std::endl;
    WSACleanup();
    return 0;
}
