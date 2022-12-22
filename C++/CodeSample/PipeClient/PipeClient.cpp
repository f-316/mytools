// PipeClient.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdio.h>
#include <windows.h>
#include "PipeCommDef.h"

int main()
{
    printf("Client!\n");

    // クライアント送信イベント
    HANDLE hCltSendEvt = ::CreateEvent(
        NULL
        , TRUE
        , FALSE
        , CLIENT_SEND_EVT
    );
    // サーバ受信イベント
    HANDLE hSvrRecvEvt = ::CreateEvent(
        NULL
        , TRUE
        , TRUE
        , SERVER_RECV_EVT
    );

    HANDLE hToSvrPipe = CreateFile(
        PIPE_NAME_TOSVR
        , GENERIC_WRITE
        , 0
        , NULL
        , OPEN_EXISTING
        , FILE_ATTRIBUTE_NORMAL
        , NULL);
    if (hToSvrPipe == INVALID_HANDLE_VALUE) {
        printf("INVALID_HANDLE_VALUE\n");
        return 1;
    }

    char cmd[256];
    //char pSendBuff[255];
    DWORD bytesWritten;
    DWORD sigPos;
    do
    {
        ::gets_s(cmd, _countof(cmd));
        sigPos = ::WaitForSingleObject(hSvrRecvEvt, INFINITE);
        ::ResetEvent(hSvrRecvEvt);
        if (!WriteFile(hToSvrPipe, cmd, strlen(cmd), &bytesWritten, NULL)) {
            printf("WriteFile fail.\n");
        }
        // 送信完了
        ::SetEvent(hCltSendEvt);
    } while (::_stricmp(cmd, "end") != 0);
    //WAIT_OBJECT_0
    ::CloseHandle(hCltSendEvt);
    ::CloseHandle(hSvrRecvEvt);
    ::CloseHandle(hToSvrPipe);
    printf("End\n");
}
