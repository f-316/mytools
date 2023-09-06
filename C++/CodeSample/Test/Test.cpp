// Test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

//#include "MathEx.h"
//#include "NetTools.h"
//#include "CsvTest.h"
//#include "BasicTools.h"
//#include "Grammar.h"
//#include "CppStandard.h"
//#include "OfficerAdapter.h"
//#include "hoge.h"
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <locale.h>
#include <locale.h>
#include "WinEventTool.h"

int main()
{
    WinEventTool tool;
    //tool.Run();
    tool.WaitEventMode();
}

int _main()
{
    char pMulti[] = "私の名前は武です。";
    printf("%s\n", pMulti);
    //setlocale(LC_ALL, "ja-JP");
    _locale_t locale;
    locale = _create_locale(LC_ALL, "ja-JP");
    printf("%d\n", _mbstrlen(pMulti));
    printf("%d\n", _mbstrlen_l(pMulti, locale));

    int psz = 0;
    while (pMulti[psz] != '\0')
    {
        psz++;
    }
    printf("size=%d\n", psz);

    for (int i = 0; i < psz; i++)
    {
        printf(" 0x%02X", 0xFF & pMulti[i]);
    }

    //CppStandard cppStd;
    //cppStd.Test();
    //CHoge hoge;
    //int sum = hoge.Sum(3, 2);
    //printf("%d\n", hoge.Sum(3, 2));
    //printf("%d\n", Multiply(3, 2));

    //COfficerAdapter office;
    //office.Load();

    //CTcpClient tcpClient;
    //tcpClient.Test();

    //CTcpServer tcpServer;
    //tcpServer.Test();

    return 1;
}
//
//void TestGrammar()
//{
//    Grammar grammar;
//    grammar.Sample();
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
