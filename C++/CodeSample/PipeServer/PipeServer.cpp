// PipeServer.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdio.h>
#include <windows.h>
#include "PipeCommDef.h"

int main()
{
    printf("Server!\n");

    // サーバ作成

    // サーバ作成


    BYTE buf[256];

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

    // Clt->Svrパイプ作成
    HANDLE hToSvrPipe = ::CreateNamedPipe(
        PIPE_NAME_TOSVR
        , PIPE_ACCESS_INBOUND // ToServer
        //, PIPE_NOWAIT | PIPE_TYPE_BYTE
        , PIPE_WAIT | PIPE_TYPE_BYTE
        , 1
        , sizeof(buf)
        , sizeof(buf)
        , 0
        , NULL
    );

    // Svr->Cltパイプ作成
    HANDLE hToCltPipe = ::CreateNamedPipe(
        PIPE_NAME_TOCLT
        , PIPE_ACCESS_OUTBOUND // ToClient
        , PIPE_NOWAIT | PIPE_TYPE_BYTE
        , 1
        , sizeof(buf)
        , sizeof(buf)
        , 0
        , NULL
    );

    printf("Wait Client...\n");
    if (!::ConnectNamedPipe(hToSvrPipe, NULL)) {
        CloseHandle(hToSvrPipe);
        return 1;
    }
    printf("Connected!\n");

    //const HANDLE handles[] = { hCltSendEvt, hSvrRecvEvt };
    const HANDLE handles[] = { hCltSendEvt };
    DWORD sigPos;
    char cmd[256] = { 0 };
    DWORD bytesRead;
    do
    {
        sigPos = ::WaitForMultipleObjects(_countof(handles), handles, FALSE, INFINITE);
        ::ResetEvent(hCltSendEvt);
        if (sigPos == 0)
        {
            if (!::ReadFile(hToSvrPipe, cmd, sizeof(cmd), &bytesRead, NULL)) {
                printf("ReadFile failed.\n");
                break;
            }
            printf("cmd=%s\n", cmd);
            // 受信完了
            ::SetEvent(hSvrRecvEvt);
        }

    } while (::_stricmp(cmd, "end") != 0);

    ::CloseHandle(hCltSendEvt);
    ::CloseHandle(hSvrRecvEvt);
    ::CloseHandle(hToSvrPipe);
    ::CloseHandle(hToCltPipe);
    printf("End\n");
}
