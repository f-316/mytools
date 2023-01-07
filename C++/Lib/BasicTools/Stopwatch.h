#pragma once
//#include <iostream>
#include <stdio.h>
#include <thread>
#include <chrono>

using namespace std::chrono;

class __declspec(dllexport) CStopwatch
{
public:
    CStopwatch();
    ~CStopwatch();
    void Sample(void);
    void Start(void);
    void Stop(void);
    unsigned long ElapsedTimeInMs(void);

protected:
    system_clock::time_point m_startCounter;
    bool isStarted;

};
