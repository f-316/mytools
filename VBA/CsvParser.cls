VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "CsvParser"
Attribute VB_GlobalNameSpace = Falses
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_charSet As String
Private m_newLine As String
Private m_useTitle As Boolean
Private m_isParsed As Boolean
Private m_lineCount As Long
Private m_dataCount As Long
Private m_itemCount As Long
Private m_csvData() As Variant
Private m_titleData As Variant
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
  m_useTitle = True
  m_isParsed = False
  m_dataCount = 0
  m_itemCount = 0
End Sub

' 解析済みか確認します。
' @param[in] none
' @return 解析済みであればTrue,それ以外はFalse
Public Function IsParsed() As Boolean
    IsParsed = m_isParsed
End Function

' タイトル部を使用するか設定します。
' @param[in] タイトルを使用する場合はTRUE
' @return none
Public Sub SetUseTitle(ByVal useTitle As Boolean)
    m_useTitle = useTitle
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
    ' Shift-JIS
    Case "sjis", "shiftjis", "shift-jis", "shift_jis"
        m_charSet = "Shift-JIS"
        m_newLine = vbCrLf
    Case Else
        m_charSet = "Shift-JIS"
        m_newLine = vbCrLf
    End Select
    
End Sub

' CSVファイルを読み込みます。
' @param[in] ファイルフルパス
' @return 成功したらTRUE
Public Function Load(ByVal filePath As String) As Boolean

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
    Set fso = Nothing

End Function

' CSVデータの数を取得します。
' @param[in] none
' @return データ数, 失敗した場合は-1
Public Function GetDataCount() As Long
    GetDataCount = -1
    If Not m_isParsed Then Exit Function
    
    ' データ数を取得
    GetDataCount = m_dataCount

End Function

' CSVアイテム名を配列で取得します。
' @param[in] none
' @return アイテム名配列
Public Function GetItems() As Variant
    If Not m_isParsed Then Exit Function
    If Not m_useTitle Then Exit Function
    
    ' タイトルデータ取得
    GetItems = m_titleData

End Function

' CSVデータを取得します。
' @param[in] none
' @return CSVデータ
Public Function GetData() As Variant
    If Not m_isParsed Then Exit Function
    
    ' csvデータ取得
    GetData = m_csvData

End Function

' CSVアイテムのデータ値を取得します。
' @param[in] アイテム名
' @param[in] データインデックス
' @return アイテムの値, 見つからない場合はEmpty
Public Function GetItemVal(ByVal itemName As String, ByVal dataIndex As Long) As Variant
    GetItemVal = Empty
    If Not m_isParsed Then Exit Function
    
    ' アイテムインデックス取得
    Dim itemIndex As Long
    itemIndex = GetItemIndex(itemName)
    If itemIndex = -1 Then Exit Function
    
    ' 値取得
    GetItemVal = m_csvData(dataIndex, itemIndex)

End Function

' CSVテーブルのデータ値を取得します。
' @param[in] データ名(row側)
' @param[in] アイテム名(col側)
' @return アイテムの値, 見つからない場合はEmpty
Public Function GetTableVal(ByVal dataName As String, ByVal itemName As String) As Variant
    GetTableVal = Empty
    If Not m_isParsed Then Exit Function
    
    ' アイテムインデックス取得
    Dim itemIndex As Long
    itemIndex = GetItemIndex(itemName)
    If itemIndex = -1 Then Exit Function
    
    ' データインデックス取得
    Dim dataIndex As Long
    dataIndex = GetDataIndex(dataName)
    If dataIndex = -1 Then Exit Function
    
    ' 値取得
    GetTableVal = m_csvData(dataIndex, itemIndex)

End Function

' CSVファイルを解析します。
' @param[in] none
' @return 成功したらTRUE
Public Function Parse() As Boolean

    Parse = False
   
    ' 行データとしてセット
    Dim strLine As Variant
    strLine = Split(m_textBuf, m_newLine)
    
    ' アイテム数取得
    m_itemCount = StrCount(strLine(0), ",") + 1
            
    ' タイトル使用時は1行目がタイトル
    Dim startLine As Long: startLine = 0
    If m_useTitle Then
        m_titleData = Split(strLine(0), ",")
        startLine = 1
    End If
    
    Dim elems As Variant
    Dim dataIndex As Long
    Dim rowIndex As Long
    Dim colIndex As Long
    Dim dataCount As Long: dataCount = 0
    
    ' データ数確認
    For rowIndex = startLine To UBound(strLine)
        If InStr(strLine(rowIndex), ",") > 0 Then
            m_dataCount = m_dataCount + 1
        End If
    Next rowIndex
    
    '取得した行列で2次元配列の再定義
    ReDim m_csvData(0 To m_dataCount - 1, 0 To m_itemCount - 1) As Variant
    
    ' データ部取得
    For rowIndex = startLine To UBound(strLine)
        If InStr(strLine(rowIndex), ",") > 0 Then
            elems = Split(strLine(rowIndex), ",")
            For colIndex = 0 To m_itemCount - 1
                dataIndex = IIf(m_useTitle, rowIndex - 1, rowIndex)
                m_csvData(dataIndex, colIndex) = elems(colIndex)
            Next colIndex
            dataCount = dataCount + 1
        End If
    Next rowIndex
        
    Parse = True

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

' アイテムのインデックスを取得します。
' @param[in] アイテム名
' @return アイテムのインデックス,見つからない場合は-1
Private Function GetItemIndex(ByVal itemName As String) As Long
    GetItemIndex = -1
    If Not m_isParsed Then Exit Function
    If Not m_useTitle Then Exit Function

    ' タイトル配列から一致するものを検索
    Dim itemIndex As Long
    For itemIndex = 0 To UBound(m_titleData)
        If itemName = m_titleData(itemIndex) Then
            GetItemIndex = itemIndex
            Exit For
        End If
    Next itemIndex
        
End Function

' データのインデックスを取得します。
' @param[in] データ名
' @return データのインデックス,見つからない場合は-1
Private Function GetDataIndex(ByVal dataName As String) As Long
    GetDataIndex = -1
    If Not m_isParsed Then Exit Function
    If Not m_useTitle Then Exit Function

    ' 1列目から一致するものを検索
    Dim dataIndex As Long
    For dataIndex = 0 To m_dataCount - 1
        If dataName = m_csvData(dataIndex, 0) Then
            GetDataIndex = dataIndex
            Exit For
        End If
    Next dataIndex
        
End Function