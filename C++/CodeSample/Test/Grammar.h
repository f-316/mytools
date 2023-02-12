#pragma once
#include <stdio.h>
class Grammar
{
public:
	Grammar();
	~Grammar();
	void Sample(void);

protected:
	void Pointer(void);
	void PointerRef(void);

private:
	class Test
	{
	public:
		Test():m_iPrivateVar(0)
		{
		}
		~Test(){}
		int GetWithChange(void)
		{
			return m_iPrivateVar++;
		}
		int GetWithoutChange(void) const
		{
			return m_iPrivateVar;
		}
	private:
		int m_iPrivateVar;
	};
};
