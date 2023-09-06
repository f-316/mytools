#include "OfficerAdapter.h"

COfficerAdapter::COfficerAdapter()
    : m_hModule(NULL)
    , isLoaded(false)
{
}

COfficerAdapter::~COfficerAdapter()
{
    if (m_hModule != NULL)
    {
        ::FreeLibrary(m_hModule);
        m_hModule = NULL;
    }
}

bool COfficerAdapter::Load(void)
{
    if (isLoaded)
    {
        return true;
    }

    m_hModule = ::LoadLibraryA("OfficerDll.dll");
    if (m_hModule == NULL)
    {
        return false;
    }
    //printf("%d\n", hDll->unused);

    //typedef int __declspec(dllimport) Multiply(int a, int b);
    //typedef int (CALLBACK* Multiply)(int, int);
    typedef int(__stdcall* Multiply)(int, int);
    //typedef int (* Multiply)(int, int);
    //Multiply lpfnMultiply = (Multiply)::GetProcAddress(hDll, "Multiply");
    //if (lpfnMultiply == NULL)
    //{
    //    return false;
    //}
    //printf("%d\n", lpfnMultiply(3, 2));

	return false;
}
