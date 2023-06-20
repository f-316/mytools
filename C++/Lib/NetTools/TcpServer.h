#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "8080"
#define DEFAULT_BACKLOG 5

class __declspec(dllexport)  CTcpServer
{
public:
	CTcpServer();
	virtual ~CTcpServer();
	
	void Test(void);
	void Test_OK(void);
	void Test2(void);

private:
	SOCKET m_serverSocket;
	SOCKET m_clientSocket;
	unsigned short m_port;

	BOOL Initialize(void);
	void End(void);
	BOOL CreateSocket(void);
	BOOL Connect(const char* pIpAddress, unsigned short port);
	BOOL SendData(const char* pData, int dataSize);
	BOOL ReceiveData(char* pData, int bufferSize);

};
