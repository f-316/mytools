﻿// Test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "MathEx.h"
#include "NetTools.h"
#include "CsvTest.h"
#include "BasicTools.h"

int main()
{
    CStopwatch s;
    s.Sample();

    return 0;
    CsvTest test;
    test.Test();
    return 0;
    int num = 0;

    num = CMathEx::Align(1, 1);
    printf("%d\n", num);
    
    num = CMathEx::Align(1, -1);
    printf("%d\n", num);
    
    CTcpClient ca;
    ca.Test();
    return 0;
     
    for (int i = 0; i < 34; i++)
    {
        num = CMathEx::Align(i, 16);
        printf("[%d]%d\n", i, num);
    }
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
