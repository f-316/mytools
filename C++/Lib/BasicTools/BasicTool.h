#pragma once
#include <thread>
#include <chrono>

class __declspec(dllexport) CBasicTool
{
public:
    static void Sleep(unsigned long sleepInMs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepInMs));
    }
};
