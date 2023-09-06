#pragma once
#include "export.h"
#include "TcpIpBase.h"

extern "C" class __declspec(dllexport) CTcpClient
	:public CTcpIpBase
{
public:
	CTcpClient();
	~CTcpClient();

private:

};
