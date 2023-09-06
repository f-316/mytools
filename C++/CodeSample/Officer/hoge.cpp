#include "pch.h"
#include "hoge.h"

int __stdcall Multiply(int a, int b)
{
	return a * b;
}

CHoge::CHoge()
{
}

CHoge::~CHoge()
{
}

int CHoge::Sum(int a, int b)
{
	return a + b;
}

int CHoge::Double(int a)
{
	return a * 2;
}