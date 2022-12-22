#pragma once

#include <stdio.h>

class CTcpClient
{
public:
	__declspec(dllexport) CTcpClient();
	__declspec(dllexport) ~CTcpClient();
	
	__declspec(dllexport) void Test(void);

private:

};
