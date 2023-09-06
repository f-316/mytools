#pragma once
#include "afxsock.h"

class CTcpIpBase
{
public:
	CTcpIpBase();
	~CTcpIpBase();

protected:
	sockaddr_in m_clientAddr;
	sockaddr_in m_serverAddr;

private:

};
