#include <stdio.h>
#include <windows.h>
#include <thread>
#include "PipeBase.h"

PipeBase pipe("ToServer", "ToClient");

void onRecv(const BYTE* pData, DWORD dataSize)
{
    AppLog::Trace(FUNCTRACEARGS, "dataSize=%lu", dataSize);
    pipe.Send((BYTE*)"aiu", 4);
}

int main(void) {

    AppLog::Trace(FUNCTRACEARGS, "PipeServerTest,Start");
    pipe.AddListener(onRecv);
    pipe.Start();
    pipe.Connect();

    pipe.Wait();

    return 0;
}
