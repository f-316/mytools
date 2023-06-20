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

    // Winscok������
    if (!Initialize())
    {
        printf("[%d]return\n", __LINE__);
        return;
    }

    // �\�P�b�g�쐬
    if (!CreateSocket())
    {
        printf("[%d]return\n", __LINE__);
        return;
    }

    // �N���C�A���g���̃\�P�b�g�ݒ�
    struct sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // �ڑ��̑Ҏ���J�n����
    listen(m_serverSocket, 1);

    // ����M�Ɏg�p����o�b�t�@
    char recv_buf1[256] = { 0 };
    char send_buf[256] = { 0 };
    int result_or_bytes;
    printf("[%d]ok\n", __LINE__);

    // �N���C�A���g����̐ڑ��҂����[�v�֐�
    while (1) {

        printf("[%d]WaitClient..., port=%d\n", __LINE__, m_port);

        // �N���C�A���g����̐ڑ�����M����
        m_clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        printf("[%d]Client connecced.\n", __LINE__);

        // �ڑ���̏���
        while (1) {

            //�p�P�b�g�̎�M(recv�͐�������Ǝ�M�����f�[�^�̃o�C�g����ԋp�B�ؒf��0�A���s��-1���ԋp�����
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

            // ���ʂ��i�[�����p�P�b�g�̑��M
            send(m_clientSocket, send_buf, sizeof(send_buf), 0);
        }

        if (recv_buf1[0] == 'q')
        {
            break;
        }
    }

    // WinSock�̏I������
    WSACleanup();

    return;
}

BOOL CTcpServer::Initialize(void)
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

void CTcpServer::End(void)
{
    closesocket(m_serverSocket);
    closesocket(m_clientSocket);
    WSACleanup();
    printf("[%d]WSACleanup\n", __LINE__);
}

BOOL CTcpServer::CreateSocket(void)
{
    // sockaddr_in�\���̂̍쐬�ƃ|�[�g�ԍ��AIP�^�C�v�̓���
    struct sockaddr_in src_addr;
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin_port = htons(m_port);
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // AF_INET��ipv4��IP�v���g�R�� & SOCK_STREAM��TCP�v���g�R��
    // IPPROTO_HOPOPTS
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == INVALID_SOCKET)
    {
        return FALSE;
    }

    // �T�[�o���̃\�P�b�g������IP�A�h���X�ƃ|�[�g�ɕR�t����
    bind(m_serverSocket, (struct sockaddr*)&src_addr, sizeof(src_addr));

    return TRUE;
}
