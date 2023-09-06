#pragma once
#include "export.h"
#include "TcpIpBase.h"

extern "C" class __declspec(dllexport) CTcpServer
	:public CTcpIpBase
{
public:
	CTcpServer();
	~CTcpServer();

	void Test(void);

private:

};

