#pragma once
#include "pch.h"

class COfficerAdapter
{
public:
	COfficerAdapter();
	~COfficerAdapter();
	bool Load(void);

protected:
	HMODULE m_hModule;
	bool isLoaded;

};

