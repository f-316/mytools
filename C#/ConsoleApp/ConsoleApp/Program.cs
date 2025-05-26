//using System;
using System;
using System.Runtime.CompilerServices;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

class Program
{
    static void Main2(string[] args)
    {
        Console.WriteLine("commandline: " + args[0]);
        string path = args[0]; // 指定のパス
        Console.WriteLine("Path.EndsInDirectorySeparator(path): " + Path.EndsInDirectorySeparator(path));

        if (Directory.Exists(path))
        {
            if (!Path.EndsInDirectorySeparator(path))
            {
                path += Path.DirectorySeparatorChar;
            }

            DateTime today = DateTime.Now;

            for (int i = 0; i <= 13; i++) // 0 から 13 なので、合計 14 日間
            {
                DateTime date = today.AddDays(i);
                Console.WriteLine(date.ToString("yyyy/MM/dd"));
            }

            static void SampleMethod(
            string str = "",
            [CallerFilePath] string file = "",
            [CallerMemberName] string member = "",
            [CallerLineNumber] int line = 0)
            {
                
                Console.WriteLine($"{Path.GetFileName(file)},{member}[{line}]{str}");
            }

            try
            {
                DateTime now = DateTime.Now;

                Console.WriteLine("path: " + path);
                string dir = path + "dir1";
                if (Directory.Exists(dir))
                { 
                    Directory.Delete(dir, true);
                }
                Directory.CreateDirectory(dir);
                Console.WriteLine("path: " + path);
            }
            catch (Exception ex)
            {
                SampleMethod("Error: " + ex.Message);
                //Console.WriteLine("Error: " + ex.Message);
            }   


            //string[] files = ;

            //Console.WriteLine("found " + files.Length + " files.");
            //foreach (string dir in Directory.EnumerateDirectories(path))
            //{
            //    Console.WriteLine(Path.GetFileName(dir));
            //}
        }
        else
        {
            Console.WriteLine("指定のディレクトリが存在しません。");
        }
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