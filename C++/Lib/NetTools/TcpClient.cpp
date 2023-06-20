#include "pch.h"
#include "TcpClient.h"

CTcpClient::CTcpClient()
    : m_clientSocket(INVALID_SOCKET)
{
}

CTcpClient::~CTcpClient()
{
    End();
}

void CTcpClient::Test(void)
{
    // Winscok初期化
    if (!Initialize())
    {
        printf("m_clientSocket=%ld\n", m_clientSocket);
        printf("[%d]return\n", __LINE__);
        return;
    }

    // ソケット作成
    if (!CreateSocket())
    {
        printf("[%d]return\n", __LINE__);
        return;
    }

    // サーバーに接続
    char server_ip_addr[32];
    int port_number;

    std::cout << "接続先IPアドレスを入力してください(xxx.xxx.xxx.xxx)" << std::endl;
    std::cin >> server_ip_addr;
    std::cout << "ポート番号を入力してください" << std::endl;
    std::cin >> port_number;

    if (!Connect(server_ip_addr, port_number))
    {
        printf("[%d]return\n", __LINE__);
        return;
    }

    // データ送信
    const char* pData1 = "Hello!!";
    int dataSize1 = strlen(pData1);

    std::string sendMsg;
    char recv_buf[256];
    while (1) {

        std::cout << "入力" << std::endl;
        std::cin >> sendMsg;

        // Packetの送信(SOCKET, Buffer, Datasize, 送信方法)
        if (!SendData(sendMsg.c_str(), sendMsg.size()))
        {
            printf("[%d]return\n", __LINE__);
            return;
        }

        // 終了コマンド
        if (sendMsg.compare("exit") == 0)
        {
            printf("[%d]return\n", __LINE__);
            return;
        }

        // Packetの受信
        if (!ReceiveData(recv_buf, sizeof(recv_buf)))
        {
            printf("[%d]return\n", __LINE__);
            return;
        }

        // 受信結果の表示
        // std::cout << "合計は" << atoi(recv_buf) << std::endl << std::endl;
        std::cout << "サーバからの応答" << recv_buf << std::endl;
    }

    return;
}

BOOL CTcpClient::Initialize(void)
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

void CTcpClient::End(void)
{
    closesocket(m_clientSocket);
    WSACleanup();
    printf("[%d]WSACleanup\n", __LINE__);
}

BOOL CTcpClient::CreateSocket(void)
{
    // ソケット作成
    m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_clientSocket == INVALID_SOCKET)
    {
        return FALSE;
    }
    printf("m_clientSocket=%ld\n", m_clientSocket);

    return TRUE;
}

BOOL CTcpClient::Connect(const char* pIpAddress, unsigned short port)
{
    // sockaddr_in構造体の作成とポート番号、IPタイプの入力
    struct sockaddr_in dst_addr;
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_port = htons(port);		// ポート番号
    dst_addr.sin_family = AF_INET;				// AF_INETはipv4を示す

    // 引数は (1) Type(ipv4 or v6) (2) IPアドレスのテキスト形式 (3) IPアドレスのバイナリ形式【(2)→(3)に変換】
    inet_pton(dst_addr.sin_family, pIpAddress, &dst_addr.sin_addr.s_addr);

    // 接続処理
    if (connect(m_clientSocket, (struct sockaddr*)&dst_addr, sizeof(dst_addr))) {
        std::cerr << "接続失敗(サーバIPアドレス" << pIpAddress << "/接続先ポート番号" << port << std::endl;
        //exit(0);
        return FALSE;
    }

    return TRUE;
}

BOOL CTcpClient::ConnectOld(const char* pIpAddress, unsigned short port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    if (serverAddr.sin_family == AF_INET)
    {
        IN_ADDR ipv4 = { 0 };
        serverAddr.sin_addr.s_addr = inet_pton(serverAddr.sin_family, pIpAddress, &ipv4);
        printf("Connect.as ipv4,%s:%d\n", pIpAddress, port);
    }
    else if (serverAddr.sin_family == AF_INET6)
    {
        IN6_ADDR ipv6 = { 0 };
        serverAddr.sin_addr.s_addr = inet_pton(serverAddr.sin_family, pIpAddress, &ipv6);
        printf("Connect.as ipv6,%s:%d\n", pIpAddress, port);
    }
    serverAddr.sin_port = htons(port);

    // 接続
    // ToDo: ノンブロッキング
    printf("[%d]connect start.\n", __LINE__);
    if (connect(m_clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("WSAGetLastError: %d\n", WSAGetLastError());
        return FALSE;
    }
    
    return TRUE;
}

BOOL CTcpClient::SendData(const char* pData, int dataSize)
{
    // 送信
    if (send(m_clientSocket, pData, dataSize, 0) == SOCKET_ERROR)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CTcpClient::ReceiveData(char* pData, int bufferSize)
{
    int recvLen = recv(m_clientSocket, pData, bufferSize, 0);
    if (recvLen == SOCKET_ERROR)
    {
        return FALSE;
    }
    // received data is in recvBuf up to recvLen bytes
    return TRUE;
}
