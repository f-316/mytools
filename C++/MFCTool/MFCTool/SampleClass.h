#pragma once
#include <afx.h>

class CSampleClass:
	public CObject
{
public:
	CSampleClass();
	~CSampleClass();

private:

public:
	// サンプルです
	void SampleMethod(int a, int b);
	void SampleMethod2(int a, int b);
};
