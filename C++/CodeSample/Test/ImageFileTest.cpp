#include "ImageFileTest.h"
#include "windows.h"
#include <stdio.h>
#include <iostream>
//#include <string>
//#include <vector>
//#include <filesystem>

#include <tchar.h>

ImageFileTest::ImageFileTest()
{
}

ImageFileTest::~ImageFileTest()
{
}

/*
    指定したフォルダ内のファイルをサブフォルダを含めて巡回する
*/
void FindDirectory
(
    std::wstring oFolderPath    // フォルダパス
)
{
    WIN32_FIND_DATA tFindFileData;

    // 全てのファイル
    oFolderPath += L"\\*.*";

    // 最初に一致するファイルを取得
    HANDLE hFile = ::FindFirstFile(oFolderPath.c_str(), &tFindFileData);
    if (INVALID_HANDLE_VALUE == hFile) {
        return;
    }

    // L"¥¥*.*"を削除
    oFolderPath = oFolderPath.substr(0, oFolderPath.size() - 4);

    do {

        TCHAR* wpFileName = tFindFileData.cFileName;

        // フォルダかどうかを判定
        if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

            /*
                L"."とL".."はスキップ
            */
            if (L'.' == wpFileName[0]) {
                if ((L'¥0' == wpFileName[1])
                    || (L'.' == wpFileName[1] && L'¥0' == wpFileName[2])
                    ) {
                    continue;
                }
            }

            // フルパスの生成
            std::wstring oFullPath = oFolderPath + L"¥¥" + wpFileName;
            std::wcout << L"(dir )" << oFullPath << std::endl;

            // 再起してサブフォルダを巡回する
            FindDirectory(oFullPath);
        }
        else {

            // フルパスの生成
            std::wstring oFullPath = oFolderPath + L"¥¥" + wpFileName;
            std::wcout << L"(file)" << oFullPath << std::endl;
        }

        // 次に一致するファイルの検索
    } while (::FindNextFile(hFile, &tFindFileData));

    // 検索ハンドルを閉じる
    ::FindClose(hFile);
}

void ImageFileTest::Test()
{
	using namespace std;

	// ファイル一覧取得
	wstring dir = L"C:\\gitrepogitory\\mytools\\C++\\CodeSample\\data";
    FindDirectory(dir);


	//GetFiles();

	// ファイル名取得

	// ファイル開く

	// 必要なデータを追加

	// ファイル閉じる

	// データをファイル出力
}
