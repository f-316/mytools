#include "TcpServer.h"
#include "Log.h"

class App
{
public:
    void Start()
    {
        AppLog::Trace(FUNCTRACEARGS, "TcpServerTest,Start");
        server.AddListener(this, &App::OnRecv);
        server.Start(12345);
        AppLog::Trace(FUNCTRACEARGS, "TcpServerTest,Started");

        server.Wait();
        AppLog::Trace(FUNCTRACEARGS, "TcpServerTest,End");
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
        server.Send(clientSocket, (BYTE*)"aiu", 3);
    }
    TcpServer server;
};

int main()
{
    App app;
    app.Start();
    return 0;
}
