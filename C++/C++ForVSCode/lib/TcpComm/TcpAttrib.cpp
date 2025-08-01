// あいうえお
#include "TcpAttrib.h"
#include <ws2tcpip.h>
#include <iostream>

void TcpAttrib::ShowSocketInfo(const SOCKET sock) {
    sockaddr_storage addr;
    int addrlen = sizeof(addr);
    if (::getpeername(sock, (sockaddr*)&addr, &addrlen) != 0) {
        std::cerr << "[Error] getpeername failed: " << WSAGetLastError() << std::endl;
        return;
    }

    char ipStr[NI_MAXHOST] = {};
    char portStr[NI_MAXSERV] = {};
    char hostStr[NI_MAXHOST] = {};

    // IPアドレス & ポート番号を文字列に
    int ret = ::getnameinfo(
        (sockaddr*)&addr, addrlen,
        ipStr, sizeof(ipStr),
        portStr, sizeof(portStr),
        NI_NUMERICHOST | NI_NUMERICSERV);
    
    if (ret != 0) {
        std::cerr << "[Error] getnameinfo failed: " << WSAGetLastError() << std::endl;
        return;
    }

    // ホスト名（DNS逆引き）も取得（失敗しても無視）
    int hostRet = ::getnameinfo(
        (sockaddr*)&addr, addrlen,
        hostStr, sizeof(hostStr),
        nullptr, 0,
        NI_NAMEREQD);

    std::cout << "[SOCKINFO] IP: " << ipStr
              << ", Port: " << portStr;
    
    if (hostRet == 0) {
        std::cout << ", Hostname: " << hostStr;
    } else {
        std::cout << ", Hostname: (not resolved)";
    }

    std::cout << std::endl;
}
