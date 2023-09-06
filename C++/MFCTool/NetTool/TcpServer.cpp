#include "pch.h"
#include "TcpServer.h"

CTcpServer::CTcpServer()
{
}

CTcpServer::~CTcpServer()
{
}

void CTcpServer::Test(void)
{
	MessageBox(NULL, _T("cap"), _T("title"), MB_OK);
	//printf("I am server!\n");
}
