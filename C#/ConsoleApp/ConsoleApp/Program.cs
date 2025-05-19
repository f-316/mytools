//using System;
using System;

public class Cls1
{
    protected string name = "Bob";

    virtual public void Greet()
    {
        Console.WriteLine("Hello, {0}", name);
    }

    override public string ToString()
    {
        return name;
    }
}

public class Cls2 : Cls1
{
    public Cls2() {
        this.name = "Alice";
    }

    override public void Greet()
    {
        Console.WriteLine("Cls2,Hello, {0}", name);
    }
}

class Hoge
{
    static string name = "あああ";
    static void Main()
    {
        var a = new List<int> { 1, 2, 3 };

        try
        {
            Cls1 cls1 = new Cls1();
            Cls2 cls2 = new Cls2();
            cls1.Greet();
            cls2.Greet();
            Console.WriteLine("Hoge.name={0}", cls1);
            Console.WriteLine("Hoge.name={0}", cls2);
            for (int i = 0; i < a.Count(); i++)
            {
                Console.WriteLine("a[{0}]={1}", i, a[i]);
            }
            //for (int i = 0; i < 10; i++)
            //{
            //    if (i == 5)
            //    {
            //        throw new Exception("あああ");
            //    }
            //    Console.WriteLine("Hello World! [{0}]={1}", i, a[i]);
            //}
        }
        //catch (IndexOutOfRangeException e)
        //{
        //    Console.WriteLine("IndexOutOfRangeException: {0}", e.Message);
        //}
        catch (Exception e)
        {
            Console.WriteLine("Exception: {0}", e.Message);
        }
        finally
        {
            Console.WriteLine("finally");
        }

        //string? s = Console.ReadLine();
        //if (s == null || s == "")
        //{
        //    Console.WriteLine("NULL!");
        //    return;
        //}

        //a = Int32.Parse(s);
        //for (int i = 0; i < a.GetLength(0); i++)
        //{
        //    if (a[i] != null)
        //    {
        //        Console.WriteLine("a[{0}]={1}", i, a[i][0]);
        //    }
        //}
    }
    static int Avg(int a, int b)
    {
        return (a + b) / 2;
    }
}
//using System;
//using System.Collections.Generic;
//using System.IO;
//using System.Linq;

//class Program
//{
//    static void Main(string[] args)
//    {
//        string inputFilePath = "input.csv"; // 入力CSVファイルのパス
//        string outputFilePath = "output.csv"; // 出力CSVファイルのパス
//        var columnsToRemove = new[] { "ColumnName1", "ColumnName2" }; // 削除したい列名

//        // CSVファイルを読み込み
//        var lines = File.ReadAllLines(inputFilePath);
//        if (lines.Length == 0)
//        {
//            Console.WriteLine("入力ファイルが空です。");
//            return;
//        }

//        // ヘッダー行を解析
//        var headers = lines[0].Split(',');
//        var indicesToKeep = headers
//            .Select((header, index) => new { header, index })
//            .Where(x => !columnsToRemove.Contains(x.header))
//            .Select(x => x.index)
//            .ToArray();

//        if (indicesToKeep.Length == 0)
//        {
//            Console.WriteLine("すべての列が削除されました。");
//            return;
//        }

//        // フィルタリングされたデータを作成
//        var filteredLines = new List<string>
//        {
//            string.Join(",", indicesToKeep.Select(i => headers[i])) // 新しいヘッダー行
//        };

//        foreach (var line in lines.Skip(1)) // データ行を処理
//        {
//            var columns = line.Split(',');
//            var filteredColumns = indicesToKeep.Select(i => columns[i]);
//            filteredLines.Add(string.Join(",", filteredColumns));
//        }

//        // 結果を出力ファイルに書き出し
//        File.WriteAllLines(outputFilePath, filteredLines);

//        Console.WriteLine("処理が完了しました。");
//    }
//}