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
    // Winscok������
    if (!Initialize())
    {
        printf("m_clientSocket=%ld\n", m_clientSocket);
        printf("[%d]return\n", __LINE__);
        return;
    }

    // �\�P�b�g�쐬
    if (!CreateSocket())
    {
        printf("[%d]return\n", __LINE__);
        return;
    }

    // �T�[�o�[�ɐڑ�
    char server_ip_addr[32];
    int port_number;

    std::cout << "�ڑ���IP�A�h���X����͂��Ă�������(xxx.xxx.xxx.xxx)" << std::endl;
    std::cin >> server_ip_addr;
    std::cout << "�|�[�g�ԍ�����͂��Ă�������" << std::endl;
    std::cin >> port_number;

    if (!Connect(server_ip_addr, port_number))
    {
        printf("[%d]return\n", __LINE__);
        return;
    }

    // �f�[�^���M
    const char* pData1 = "Hello!!";
    int dataSize1 = strlen(pData1);

    std::string sendMsg;
    char recv_buf[256];
    while (1) {

        std::cout << "����" << std::endl;
        std::cin >> sendMsg;

        // Packet�̑��M(SOCKET, Buffer, Datasize, ���M���@)
        if (!SendData(sendMsg.c_str(), sendMsg.size()))
        {
            printf("[%d]return\n", __LINE__);
            return;
        }

        // �I���R�}���h
        if (sendMsg.compare("exit") == 0)
        {
            printf("[%d]return\n", __LINE__);
            return;
        }

        // Packet�̎�M
        if (!ReceiveData(recv_buf, sizeof(recv_buf)))
        {
            printf("[%d]return\n", __LINE__);
            return;
        }

        // ��M���ʂ̕\��
        // std::cout << "���v��" << atoi(recv_buf) << std::endl << std::endl;
        std::cout << "�T�[�o����̉���" << recv_buf << std::endl;
    }

    return;
}

BOOL CTcpClient::Initialize(void)
{
    // Winscok������
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
    // �\�P�b�g�쐬
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
    // sockaddr_in�\���̂̍쐬�ƃ|�[�g�ԍ��AIP�^�C�v�̓���
    struct sockaddr_in dst_addr;
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_port = htons(port);		// �|�[�g�ԍ�
    dst_addr.sin_family = AF_INET;				// AF_INET��ipv4������

    // ������ (1) Type(ipv4 or v6) (2) IP�A�h���X�̃e�L�X�g�`�� (3) IP�A�h���X�̃o�C�i���`���y(2)��(3)�ɕϊ��z
    inet_pton(dst_addr.sin_family, pIpAddress, &dst_addr.sin_addr.s_addr);

    // �ڑ�����
    if (connect(m_clientSocket, (struct sockaddr*)&dst_addr, sizeof(dst_addr))) {
        std::cerr << "�ڑ����s(�T�[�oIP�A�h���X" << pIpAddress << "/�ڑ���|�[�g�ԍ�" << port << std::endl;
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

    // �ڑ�
    // ToDo: �m���u���b�L���O
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
    // ���M
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
