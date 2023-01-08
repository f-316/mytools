#include "pch.h"
#include "Stopwatch.h"

using namespace std::chrono;

CStopwatch::CStopwatch()
    : isStarted(false)
{
    //time_point<system_clock>
    //m_startCounter = system_clock::time_point<>;
}

CStopwatch::~CStopwatch()
{
}

inline void CStopwatch::Sample(void)
{
    // éÛêMë“Çø
    unsigned long recvTOInMs = 1000;
    unsigned long elapsedTimeInMs = 0;
    printf("CStopwatch::Sample,Start!\n");
    Start();
    while (1)
    {
        elapsedTimeInMs = ElapsedTimeInMs();
        if (recvTOInMs < elapsedTimeInMs)
        {
            break;
        }
        Sleep(100);
        //printf("elapsedTimeInMs=%lu\n", elapsedTimeInMs);

    }
    elapsedTimeInMs = ElapsedTimeInMs();
    printf("CStopwatch::Sample,elapsedTimeInMs=%lu\n", elapsedTimeInMs);

    printf("CStopwatch::Sample,End!\n");
}

void CStopwatch::Start(void)
{
    m_startCounter = system_clock::now();
    isStarted = true;
}

void CStopwatch::Stop(void)
{
    isStarted = false;
}

unsigned long CStopwatch::ElapsedTimeInMs(void)
{
    if (!isStarted)
    {
        return 0;
    }

    return (unsigned long)duration_cast<milliseconds>(system_clock::now() - m_startCounter).count();
}
