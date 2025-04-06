// Test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <locale.h>
#include "WinEventTool.h"
//
//#include <windows.h>
//#include <iostream>
//
//void MonitorDirectory(const std::wstring& directory) {
//    HANDLE hDirectory = CreateFile(
//        directory.c_str(),
//        FILE_LIST_DIRECTORY,
//        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
//        NULL,
//        OPEN_EXISTING,
//        FILE_FLAG_BACKUP_SEMANTICS,
//        NULL
//    );
//
//    if (hDirectory == INVALID_HANDLE_VALUE) {
//        std::cerr << "Failed to open directory handle." << std::endl;
//        return;
//    }
//
//    BYTE buffer[1024];
//    DWORD bytesReturned;
//
//    while (true) {
//        if (ReadDirectoryChangesW(
//            hDirectory,
//            buffer,
//            sizeof(buffer),
//            TRUE, // サブディレクトリを監視する
//            FILE_NOTIFY_CHANGE_FILE_NAME |
//            FILE_NOTIFY_CHANGE_DIR_NAME |
//            FILE_NOTIFY_CHANGE_ATTRIBUTES |
//            FILE_NOTIFY_CHANGE_SIZE |
//            FILE_NOTIFY_CHANGE_LAST_WRITE,
//            &bytesReturned,
//            NULL,
//            NULL
//        )) {
//            FILE_NOTIFY_INFORMATION* pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
//            std::wcout << L"Change detected: " << pNotify->FileName << std::endl;
//        }
//        else {
//            std::cerr << "Failed to read directory changes." << std::endl;
//            break;
//        }
//    }
//
//    CloseHandle(hDirectory);
//}

#include <windows.h>
#include <iostream>

void WatchDirectory(const std::wstring& directoryPath) {

    // コンソールのコードページをShift-JISに変更
    SetConsoleOutputCP(932);

    // ロケール設定（Shift-JIS対応）
    std::wcout.imbue(std::locale(""));

    HANDLE hDirectory = CreateFileW(
        directoryPath.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL);

    if (hDirectory == INVALID_HANDLE_VALUE) {
        std::wcerr << L"ディレクトリを開けません: " << GetLastError() << std::endl;
        return;
    }

    BYTE buffer[1024];
    DWORD bytesReturned;

    while (true) {
        if (ReadDirectoryChangesW(
            hDirectory,
            &buffer,
            sizeof(buffer),
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE |
            FILE_NOTIFY_CHANGE_CREATION,
            &bytesReturned,
            NULL,
            NULL)) {

            FILE_NOTIFY_INFORMATION* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);

            do {
                std::wstring fileName(info->FileName, info->FileNameLength / sizeof(WCHAR)); // UTF-16の長さで分割
                std::wcout << L"変更検知: " << fileName << std::endl;

                info = info->NextEntryOffset
                    ? reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                        reinterpret_cast<BYTE*>(info) + info->NextEntryOffset)
                    : nullptr;
            } while (info);

        }
        else {
            std::wcerr << L"監視エラー: " << GetLastError() << std::endl;
            break;
        }
    }

    CloseHandle(hDirectory);
}

int main() {
    std::wstring directoryPath = L"C:\\temp";
    WatchDirectory(directoryPath);
    return 0;
}
//
//int main()
//{
//    //printf("hello");
//    std::wstring directory = L"C:\\temp";
//    //MonitorDirectory(directory);
//
//    HANDLE hChangeHandle = FindFirstChangeNotification(
//        directory.c_str(), // 監視対象のディレクトリ
//        TRUE,                       // サブディレクトリも監視するか
//        FILE_NOTIFY_CHANGE_FILE_NAME // 監視する変更の種類
//    );
//
//    if (hChangeHandle == INVALID_HANDLE_VALUE) {
//        // エラー処理
//    }
//    BYTE buffer[1024];
//
//    // イベントが発生するまで待機
//    DWORD dwWaitStatus;
//    while (1)
//    {
//        dwWaitStatus = WaitForSingleObject(hChangeHandle, INFINITE);
//
//        FILE_NOTIFY_INFORMATION* pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
//        std::wcout << L"Change detected: " << pNotify->FileName << std::endl;
//
//        if (dwWaitStatus == WAIT_OBJECT_0) {
//            // 変更が検知された場合の処理
//            FindNextChangeNotification(hChangeHandle);
//        }
//    }
//
//
//    FindCloseChangeNotification(hChangeHandle);
//
//    return 1;
//}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
