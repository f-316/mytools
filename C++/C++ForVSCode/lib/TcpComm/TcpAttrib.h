#pragma once
#include <winsock2.h>

class TcpAttrib {
public:
    static void ShowSocketInfo(const SOCKET sock);
};
