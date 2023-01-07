#include "pch.h"
#include "AsyncTask.h"

CAsyncTask::CAsyncTask()
{

}

CAsyncTask::~CAsyncTask()
{

}

BOOL CAsyncTask::Start(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter)
{
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    SIZE_T dwStackSize = 0; // 0で既定サイズを使用
    DWORD dwCreationFlags = 0; // 作成後即実行
    DWORD* lpThreadId = NULL; // NULLならスレッドIDは返されない
    m_hThread = CreateThread(
        lpThreadAttributes
        , dwStackSize
        , lpStartAddress
        , lpParameter
        , dwCreationFlags
        , lpThreadId);
	return 0;
}

DWORD CAsyncTask::Wait(DWORD timeoutInMs)
{
    DWORD ret;
    ret = ::WaitForSingleObject(m_hThread, timeoutInMs);
    if (ret == WAIT_OBJECT_0)
    {
        //std::cout << "End! ret:" << ret << std::endl;
    }
    else if (ret == WAIT_TIMEOUT)
    {
        std::cout << "ret:" << ret << std::endl;
    }
    else
    {
        std::cout << "ret:" << ret << std::endl;
    }

    return 0;
}
