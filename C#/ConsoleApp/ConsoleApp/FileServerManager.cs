//using System;
using System;
using System.Runtime.CompilerServices;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using static System.Collections.Specialized.BitVector32;
using System.IO.Compression;

public class Test
{
    static void Main(string[] args)
    {
        try
        {
            string path = @"\\localhost\C\Users\Fumihiro\Desktop\test\01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\a";
            //string path = @"C:\Users\Fumihiro\Desktop\test\01234567890123456789012345678901234567890123456789\b";
            foreach(var file in Directory.GetFiles(path, "*.*", SearchOption.AllDirectories))
            {
                Console.WriteLine(file);
                Console.WriteLine(file.Length);
                File.Delete(file);
            }
            Console.WriteLine(path.Length);
            //Directory.Delete(path, true);
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
        }
    }
}

class FileServerManager
{
    static void Main2(string[] args)
    {
        FileServerManager manager = new FileServerManager();
        manager.Execute(args);
    }

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
    private static extern uint GetPrivateProfileString(string section, string key, string defaultValue, StringBuilder retVal, int size, string filePath);
    void WriteLog(string message)
    {
        string timestamp = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"); // 現在の日時を取得
        string logEntry = $"[{timestamp}] {message}"; // ログ形式を作成
        using (StreamWriter writer = new StreamWriter(m_logFilePath, true)) // 追記モードで開く
        {
            writer.WriteLine(logEntry);
        }
    }

    void PutLog(string message)
    {
        string logEntry = $"{message}"; // ログ形式を作成
        using (StreamWriter writer = new StreamWriter(m_logFilePath, true)) // 追記モードで開く
        {
            writer.WriteLine(logEntry);
        }
    }

    private const string m_defailtLogFileName = "#output.log";
    private const string m_defailtConfigIniFileName = "#config.ini";
    private string m_targetDir = "."; // デフォルトのターゲットディレクトリは現在のディレクトリ
    private string m_logFilePath = m_defailtLogFileName;
    private string m_configIniFilePath = m_defailtConfigIniFileName;

    private int m_maxFileSizeInkB = 100; // デフォルトの最大ファイルサイズ（KB）
    private int m_daysToKeep = 14; // デフォルトの保持日数
    private List<string> m_ignoreDirs = new List<string>(); // 無視するディレクトリのリスト
    private List<string> m_initSubDirs = new List<string>(); // 無視するディレクトリのリスト

    bool ImportConfig(string iniFilePath)
    {
        StringBuilder buf = new StringBuilder(255);

        try
        {
            GetPrivateProfileString("Settings", "maxFileSizeInkB", "100", buf, buf.Capacity, m_configIniFilePath);
            m_maxFileSizeInkB = int.Parse(buf.ToString());
            GetPrivateProfileString("Settings", "daysToKeep", "14", buf, buf.Capacity, m_configIniFilePath);
            m_daysToKeep = int.Parse(buf.ToString());
            GetPrivateProfileString("Settings", "ignoreDirs", "", buf, buf.Capacity, m_configIniFilePath);
            string[] bufs = buf.ToString().Split(',');
            foreach (var item in bufs)
            {
                m_ignoreDirs.Add(item);
            }
            GetPrivateProfileString("Settings", "initSubDirs", "", buf, buf.Capacity, m_configIniFilePath);
            bufs = buf.ToString().Split(',');
            foreach (var item in bufs)
            {
                m_initSubDirs.Add(item);
            }
        }
        catch (Exception e)
        {
            Console.WriteLine($"error: {e}");
            return false;
        }
        return true;
    }
    static void CompressFile(string filePath, string zipPath)
    {
        using (FileStream zipStream = new FileStream(zipPath, FileMode.Create))
        using (ZipArchive archive = new ZipArchive(zipStream, ZipArchiveMode.Create))
        {
            archive.CreateEntryFromFile(filePath, Path.GetFileName(filePath));
        }
    }


    bool Prapare(string[] args)
    {
        switch (args.Length)
        {
            case 3:
                m_configIniFilePath = args[2];
                goto case 2;
            case 2:
                m_logFilePath = args[1];
                goto case 1;
            case 1:
                m_targetDir = args[0];
                break;
            default:
                Console.WriteLine($"Inivalid arguments provided.{args.Length}");
                return false;
        }

        // 対象ディレクトリ存在チェック
        if (!Directory.Exists(m_targetDir))
        {
            Console.WriteLine("Directory does not exist.");
            return false;
        }

        // ログディレクトリ存在チェック
        string? logParentDir = Path.GetDirectoryName(m_logFilePath); // 親ディレクトリを取得
        string curDir = Directory.GetCurrentDirectory();
        if (!string.IsNullOrEmpty(logParentDir) && !Directory.Exists(logParentDir))
        {
            string buf = m_defailtLogFileName;
            m_logFilePath = Path.Combine(curDir, m_defailtLogFileName);
            WriteLog($"logParentDir does not exist. change. {buf}->{m_logFilePath}");
        }

        // INIファイルからログ設定を取得
        if (!File.Exists(m_configIniFilePath))
        {
            string buf = m_configIniFilePath;
            m_configIniFilePath = Path.Combine(curDir, m_defailtConfigIniFileName);
            WriteLog($"configIniFilePath does not exist. change. {buf}->{m_configIniFilePath}");
        }

        // コンフィグ読み込み
        if (!ImportConfig(m_configIniFilePath))
        {
            WriteLog($"m_configIniFilePath does not exists. use default,{m_configIniFilePath}");
        }

        return true;
    }

    private void Execute(string[] args) {

        if (!Prapare(args))
        {
            return;
        }

        // 開始ログ
        WriteLog($"Start");
        PutLog($" targetDir={m_targetDir}");
        PutLog($" maxFileSizeInkB={m_maxFileSizeInkB} kB");
        PutLog($" daysToKeep={m_daysToKeep}");
        PutLog($" ignoreDirs={m_ignoreDirs.ToString()}");
        PutLog($" initSubDirs={m_initSubDirs.ToString()}");

        List<string> dayDirNames = new List<string>();
        DateTime today = DateTime.Now;
        for (int i = 0; i <= 13; i++) // 0 から 13 なので、合計 14 日間
        {
            DateTime date = today.AddDays(i);
            string dayDirName = date.ToString("yyyyMMdd");
            dayDirNames.Add(dayDirName);
            string dayDirPath = Path.Combine(m_targetDir, dayDirName);
            if (!Directory.Exists(dayDirPath))
            {
                Directory.CreateDirectory(dayDirPath);
            }
        }

        // ディレクトリの一覧を取得
        foreach (var dirPath in Directory.GetDirectories(m_targetDir))
        {
            if (!dayDirNames.Contains(Path.GetFileName(dirPath)))
            {
                // ToDo: 権限確認
                Directory.Delete(dirPath, true);
            }
        }

        string? logParentDir = Path.GetDirectoryName(m_logFilePath); // 親ディレクトリを取得
        if (!string.IsNullOrEmpty(logParentDir) && Directory.Exists(logParentDir))
        {
            string zipPath = Path.Combine(logParentDir, "compressed.zip");
            CompressFile(m_logFilePath, zipPath);
        }
    }
}
