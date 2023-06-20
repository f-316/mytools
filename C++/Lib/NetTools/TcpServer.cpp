#include "pch.h"
#include "TcpServer.h"

CTcpServer::CTcpServer()
    : m_serverSocket(INVALID_SOCKET)
    , m_clientSocket(INVALID_SOCKET)
    , m_port(0)
{
}

CTcpServer::~CTcpServer()
{
    End();
}

void CTcpServer::Test(void)
{
    m_port = 1234;

    // Winscok初期化
    if (!Initialize())
    {
        printf("[%d]return\n", __LINE__);
        return;
    }

    // ソケット作成
    if (!CreateSocket())
    {
        printf("[%d]return\n", __LINE__);
        return;
    }

    // クライアント側のソケット設定
    struct sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // 接続の待受を開始する
    listen(m_serverSocket, 1);

    // 送受信に使用するバッファ
    char recv_buf1[256] = { 0 };
    char send_buf[256] = { 0 };
    int result_or_bytes;
    printf("[%d]ok\n", __LINE__);

    // クライアントからの接続待ちループ関数
    while (1) {

        printf("[%d]WaitClient..., port=%d\n", __LINE__, m_port);

        // クライアントからの接続を受信する
        m_clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        printf("[%d]Client connecced.\n", __LINE__);

        // 接続後の処理
        while (1) {

            //パケットの受信(recvは成功すると受信したデータのバイト数を返却。切断で0、失敗で-1が返却される
            result_or_bytes = recv(m_clientSocket, recv_buf1, sizeof(char) * 256, 0);
            if (result_or_bytes == 0 || result_or_bytes == SOCKET_ERROR) {
                break;
            }
            else if (recv_buf1[0] == 'q')
            {
                break;
            }
            printf("[%d]recv_buf1=%s\n", __LINE__, recv_buf1);
            snprintf(send_buf, 256, "echo=%s", recv_buf1);

            // 結果を格納したパケットの送信
            send(m_clientSocket, send_buf, sizeof(send_buf), 0);
        }

        if (recv_buf1[0] == 'q')
        {
            break;
        }
    }

    // WinSockの終了処理
    WSACleanup();

    return;
}

BOOL CTcpServer::Initialize(void)
{
    // Winscok初期化
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return FALSE;
    }

    // Winsock is now initialized
    // ...
    printf("winsock ver.%d.%d\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

    return TRUE;
}

void CTcpServer::End(void)
{
    closesocket(m_serverSocket);
    closesocket(m_clientSocket);
    WSACleanup();
    printf("[%d]WSACleanup\n", __LINE__);
}

BOOL CTcpServer::CreateSocket(void)
{
    // sockaddr_in構造体の作成とポート番号、IPタイプの入力
    struct sockaddr_in src_addr;
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin_port = htons(m_port);
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // AF_INETはipv4のIPプロトコル & SOCK_STREAMはTCPプロトコル
    // IPPROTO_HOPOPTS
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == INVALID_SOCKET)
    {
        return FALSE;
    }

    // サーバ側のソケットを特定のIPアドレスとポートに紐付ける
    bind(m_serverSocket, (struct sockaddr*)&src_addr, sizeof(src_addr));

    return TRUE;
}
