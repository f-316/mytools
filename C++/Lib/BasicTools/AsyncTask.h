#pragma once
#include <iostream>
#include <windows.h>
//#include <thread>
//#include <chrono>

//using namespace std::chrono;

class __declspec(dllexport) CAsyncTask
{
public:
    CAsyncTask();
    ~CAsyncTask();
    BOOL Start(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter);
    DWORD Wait(DWORD timeoutInMs = INFINITE);

protected:
    HANDLE m_hThread;
};
