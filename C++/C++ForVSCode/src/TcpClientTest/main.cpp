#include "TcpClient.h"
#include "Log.h"
#include <string>
#include <iostream>

class App
{
public:
    void Start()
    {
        AppLog::Trace(FUNCTRACEARGS, "TcpClientTest,Start");
        client.AddListener(this, &App::OnRecv);
        client.Connect("127.0.0.1", 12345);
        AppLog::Trace(FUNCTRACEARGS, "TcpClientTest,Started");

        // ƒRƒ}ƒ“ƒh
        std::string line;
        while (true) {
            std::getline(std::cin, line);
            if (line == "close") break;
            client.Send((const BYTE*)line.c_str(), (DWORD)line.size());
        }
        client.Disconnect();
    }

private:
    static void OnRecv(void* pInstance, const SOCKET clientSocket, const BYTE* pData, DWORD dataSize)
    {
        App* pThis = (App*)pInstance;
        pThis->OnRecv(clientSocket, pData, dataSize);
    }

    void OnRecv(const SOCKET clientSocket, const BYTE* pData, DWORD dataSize)
    {
        AppLog::Trace(FUNCTRACEARGS, "dataSize=%lu", dataSize);
    }
    TcpClient client;
};

int main() {

    App app;
    app.Start();
    return 0;
}
