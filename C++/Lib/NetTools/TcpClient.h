#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

class __declspec(dllexport)  CTcpClient
{
public:
	CTcpClient();
	virtual ~CTcpClient();
	
	void Test(void);
	void Test_ok(void);
	void Test2(void);

private:
	SOCKET m_clientSocket;

	BOOL Initialize(void);
	void End(void);
	BOOL CreateSocket(void);
	BOOL Connect(const char* pIpAddress, unsigned short port);
	BOOL ConnectOld(const char* pIpAddress, unsigned short port);
	BOOL SendData(const char* pData, int dataSize);
	BOOL ReceiveData(char* pData, int bufferSize);

};
