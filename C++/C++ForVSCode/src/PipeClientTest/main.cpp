#include <stdio.h>
#include <windows.h>
#include <thread>
#include "PipeBase.h"

PipeBase pipe("ToClient", "ToServer");

void onRecv(const BYTE* pData, DWORD dataSize)
{
    AppLog::Trace(FUNCTRACEARGS, "dataSize=%lu", dataSize);
    Sleep(2 * 1000);
    pipe.Send((BYTE*)"aiu", 4);
}

int main(void) {

    AppLog::Trace(FUNCTRACEARGS, "PipeClientTest,Start");
    pipe.AddListener(onRecv);
    pipe.Start();
    pipe.Connect();
    
    AppLog::Trace(FUNCTRACEARGS, "Start");
    Sleep(2 * 1000);
    pipe.Send((BYTE*)"aiu", 4);

    pipe.Wait();

    return 0;
}
