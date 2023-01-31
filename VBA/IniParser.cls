VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "IniParser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_charSet As String
Private m_newLine As String
Private m_isParsed As Boolean
Private m_lineCount As Long
Private m_dataCount As Long
Private m_itemCount As Long
Private m_iniData As Object
Private m_sectionData() As Variant
Private m_textBuf As String

Private Sub Class_Initialize()
  Call Reset
End Sub

Private Sub Class_Terminate()
End Sub

' メンバをリセットします。
' @param[in] none
' @return none
Private Sub Reset()
  m_newLine = vbCrLf
  m_charSet = "Shift-JIS"
  m_isParsed = False
  m_dataCount = 0
  m_itemCount = 0
End Sub

' 文字セットを設定します。
' @param[in] 文字セット
' @return none
Public Sub SetCharSet(ByVal charSet As String)
    charSet = LCase(charSet)
    Select Case charSet
    ' UTF-8
    Case "utf8", "utf-8", "utf_8"
        m_charSet = "UTF-8"
        m_newLine = vbLf
    ' UTF-16
    Case "utf16", "utf-16", "utf_16"
        m_charSet = "UTF-16"
        m_newLine = vbCrLf
    ' Shift-JIS
    Case "sjis", "shiftjis", "shift-jis", "shift_jis"
        m_charSet = "Shift-JIS"
        m_newLine = vbCrLf
    Case Else
        m_charSet = "Shift-JIS"
        m_newLine = vbCrLf
    End Select
    
End Sub

' iniファイルを読み込みます。
' @param[in] ファイルフルパス
' @return 成功したらTRUE
Public Function Load(ByVal filePath) As Boolean

    ' 相対パスを絶対パスに変換
    Dim fso As Object
    Set fso = CreateObject("Scripting.FileSystemObject")
    If Left(filePath, 1) = "." Then
        filePath = ThisWorkbook.Path & "\" & filePath
    End If
    filePath = fso.GetAbsolutePathName(filePath)
    
    ' 行数取得
    m_lineCount = CreateObject("Scripting.FileSystemObject").OpenTextFile(filePath, 8).Line 'ファイルの行数取得

    ' データ読み込み
    With CreateObject("ADODB.Stream")
        .charSet = m_charSet
        .Open
        .LoadFromFile filePath
        m_textBuf = .ReadText
        .Close
    End With
    
    ' 解析
    m_isParsed = Parse()
    Load = m_isParsed
    
End Function

' ファイルに書き出します。
' @param[in] Dictionaryデータ,1階層目をSection,2階層目をkey=valueとして扱います。
' @param[in] 出力先ファイルパス
' @return 成功したらTrue,それ以外False
Public Function WriteFile(ByRef iniData As Object, ByRef filePath As String) As Boolean
    
    WriteFile = False
    
    Dim writeStr As String
    writeStr = MakeIniText(iniData)
    
    ' 0:改行無し, 1:改行あり
    Dim writeMode As Long
    writeMode = 0
    
    ' 1:ファイルが存在する場合実行エラー, 2:上書き
    Dim saveMode As Long
    saveMode = 2
    
    ' ファイルを開く
    With CreateObject("ADODB.Stream")
        .charSet = m_charSet
        .Open
        .WriteText writeStr, writeMode
        .SaveToFile filePath, saveMode
       .Close
    End With
    
    ' return
    WriteFile = True

End Function

' iniデータ内のSectionを配列で取得します。
' @param[in] none
' @return Section名配列
Public Function GetSections() As Variant
    If Not m_isParsed Then Exit Function
    
    ' セクションデータ取得
    GetSections = m_iniData.Keys
    
End Function

' iniデータ内の指定したSectionのkeyを配列で取得します。
' @param[in] none
' @return key名配列
Public Function GetKeys(ByVal sectionName As String) As Variant
    If Not m_isParsed Then Exit Function
    
    ' keyデータ取得
    If Not HasSection(sectionName) Then Exit Function
    GetKeys = m_iniData(sectionName).Keys
    
End Function

' iniデータを取得します。
' @param[in] none
' @return iniデータ
Public Function GetData() As Object
    If Not m_isParsed Then Exit Function
    
    ' iniデータ取得
    Set GetData = m_iniData

End Function

' iniデータ値を取得します。
' @param[in] セクション名
' @param[in] キー名
' @return 値
Public Function GetVal(ByVal sectionName As String, ByVal keyName As String) As Variant
    
    GetVal = Empty
    If Not m_isParsed Then Exit Function
    If Not HasSection(sectionName) Then Exit Function
        
    ' iniデータ取得
    GetVal = m_iniData(sectionName)(keyName)

End Function

' セクションを持つか判定します。
' @param[in] セクション名
' @return セクションを持っていればTrue,それ以外はFalse
Public Function HasSection(ByVal sectionName As String) As Variant
    
    HasSection = False
    If Not m_isParsed Then Exit Function
    
    ' セクション検索
    Dim sections As Variant
    Dim section As Variant
    sections = m_iniData.Keys
    For Each section In sections
        If section = sectionName Then HasSection = True: Exit Function
    Next section
    
End Function

' キーを持つか判定します。
' @param[in] セクション名
' @param[in] キー名
' @return キーを持っていればTrue,それ以外はFalse
Public Function HasKey(ByVal sectionName As String, ByVal keyName As String) As Variant
    
    HasKey = False
    If Not m_isParsed Then Exit Function
    If Not HasSection(sectionName) Then Exit Function
        
    ' キー検索
    Dim keys_ As Variant
    Dim key_ As Variant
    keys_ = m_iniData(sectionName).Keys
    For Each key_ In keys_
        If key_ = keyName Then HasKey = True: Exit Function
    Next key_

End Function

' iniファイルを解析します。
' @param[in] none
' @return 成功したらTRUE
Public Function Parse() As Boolean

    Parse = False
   
    ' ini形式データ
    Set m_iniData = CreateObject("Scripting.Dictionary")
        
    ' sectionの一時データ
    Dim sectBuf As Object
    Set sectBuf = CreateObject("Scripting.Dictionary")

    ' 行データとしてセット
    Dim strLine As Variant
    strLine = Split(m_textBuf, m_newLine)
    
    ' データ部取得
    Dim curSectionName As String: curSectionName = ""
    Dim strBuf As String
    Dim lineIndex As Long
    For lineIndex = 0 To UBound(strLine)
        strBuf = Trim(strLine(lineIndex))
        
        ' 無視する行
        If Left(strBuf, 1) = "#" Then
        ElseIf Left(strBuf, 1) = ";" Then
        ElseIf Left(strBuf, 1) = "" Then
        
        ' セクション名セット
        ElseIf Left(strBuf, 1) = "[" Then
            
            ' 既にセクション名がある場合
            If curSectionName <> "" Then
                ' セクション内容セット
                Set m_iniData(curSectionName) = sectBuf
            End If
            
            ' セクション内容初期化
            Set sectBuf = CreateObject("Scripting.Dictionary")
            
            ' テキストからセクション名取得
            curSectionName = ToSectionName(strBuf)
            If curSectionName = "" Then MsgBox ("Section Name is invalid"): Exit Function
            
            ' セクション名をセット
            Set m_iniData(curSectionName) = Nothing
        
        ' 値セット
        Else
            If Not SetKeyValue(sectBuf, strBuf) Then MsgBox ("KeyValue is invalid"): Exit Function
        End If
    

    Next lineIndex

    ' 最後のセクション内容セット
    Set m_iniData(curSectionName) = sectBuf

    Set sectBuf = Nothing
    Parse = True

End Function

' テキストをSection名に変換します。
' @param[in] Seciton部テキスト
' @return Section名,取得できない場合は空白
Private Function ToSectionName(ByVal sectionStr As String) As String
    
    ToSectionName = ""
    
    ' "["と"]"が含まれるか確認
    If InStr(sectionStr, "[") <= 0 Then Exit Function
    If InStr(sectionStr, "]") <= 0 Then Exit Function
    
    ' section名をセット
    sectionStr = Replace(sectionStr, "[", "")
    sectionStr = Replace(sectionStr, "]", "")
    sectionStr = Trim(sectionStr)
    
    ToSectionName = sectionStr

End Function

' セクションDictionaryにKeyValueをセットします。
' @param[in, out] セクションDictionary
' @param[in] KeyValue部テキスト
' @return 成功したらTrue
Private Function SetKeyValue(ByRef sectDict As Object, ByVal keyValueStr As String)
    
    SetKeyValue = False
    
    ' "="が含まれるか確認
    If InStr(keyValueStr, "=") <= 0 Then Exit Function
    
    ' key,valueとしてセット
    Dim keyValueArr As Variant
    keyValueArr = Split(keyValueStr, "=")
    sectDict(Trim(keyValueArr(0))) = Trim(keyValueArr(1))

    SetKeyValue = True

End Function

' 文字数をカウントします。
' @param[in] 対象文字列
' @param[in] 検索文字列
' @return 見つかった文字数
Private Function StrCount(ByVal srcStr As String, ByVal tarStr As String) As Long
    
    Dim n As Long
    Dim cnt As Long
    Do
        n = InStr(n + 1, srcStr, tarStr)
        If n = 0 Then
            Exit Do
        Else
            cnt = cnt + 1
        End If
    Loop
    StrCount = cnt

End Function

' iniデータ(Dictionaryオブジェクト)からテキストを生成します。
' @param[in] Dictionaryデータ,1階層目をSection,2階層目をkey=valueとして扱います。
' @return 生成したテキストデータ
Private Function MakeIniText(ByRef iniData As Scripting.Dictionary) As String
    
    MakeIniText = ""
    
    Dim iniText As String

    Dim section_ As Variant
    For Each section_ In iniData.Keys()
        
        ' セクション名
        iniText = iniText & "[" & section_ & "]" & m_newLine
        
        Dim key_ As Variant
        For Each key_ In iniData(section_).Keys()
            ' key,value
            iniText = iniText & key_ & "=" & iniData(section_)(key_) & m_newLine
        Next key_
    
    Next section_

    ' return
    MakeIniText = iniText

End Function
