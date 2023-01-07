#pragma once
#ifndef __FileOpr__
#define __FileOpr__

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class __declspec(dllexport) CFile
{
public:
	CFile(void);
	virtual ~CFile(void);
	int Parse(const char* filePath);

private:
};

#endif // !__FileOpr__
