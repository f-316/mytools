VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "JsonParser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_charSet As String
Private m_newLine As String
Private m_isParsed As Boolean
Private m_lineCount As Long
Private m_jsonData As Object
Private m_textBuf As String

Private Sub Class_Initialize()
    Call Reset
End Sub

Private Sub Class_Terminate()
    Call Reset
End Sub

' メンバをリセットします。
' @param[in] none
' @return none
Private Sub Reset()
    m_charSet = "UTF-8"
    m_newLine = vbCrLf
    m_isParsed = False
    m_lineCount = 0
    Set m_jsonData = Nothing
    m_textBuf = ""
End Sub

' 解析済みか確認します。
' @param[in] none
' @return 解析済みであればTrue,それ以外はFalse
Public Function IsParsed() As Boolean
    IsParsed = m_isParsed
End Function

' 文字セットを設定します。
' @param[in] 文字セット
' @param[in] 改行コード連動(推奨の改行コードをセット)
' @return none
Public Sub SetCharSet(ByVal charSet As String, Optional ByVal withNewLine As Boolean = False)
    charSet = LCase(charSet)
    Select Case charSet
    ' UTF-8
    Case "utf8", "utf-8", "utf_8"
        m_charSet = "UTF-8"
        If withNewLine Then m_newLine = vbLf
    ' Shift-JIS
    Case "sjis", "shiftjis", "shift-jis", "shift_jis"
        m_charSet = "Shift-JIS"
        If withNewLine Then m_newLine = vbCrLf
    Case Else
        m_charSet = "Shift-JIS"
        If withNewLine Then m_newLine = vbCrLf
    End Select
    
End Sub

' 改行コードを設定します。
' @param[in] 改行コード
' @return none
Public Sub SetNewLine(ByVal newLine As String)
    m_newLine = newLine
End Sub

' jsonファイルを読み込みます。
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

' jsonをテキストデータで取得します。
' @param[in] none
' @return jsonテキスト
Public Function GetJsonStr() As String
    
    GetJsonStr = ""
    If Not m_isParsed Then Exit Function

    ' 出力可能文字に変換
    Dim nestCnt As Long: nestCnt = 0
    Dim str As String
    Call GetPrintableStr(m_jsonData, nestCnt, str)

    ' return
    GetJsonStr = str

End Function

' jsonデータ内のrootオブジェクトが持つキーを配列で取得します。
' @param[in] none
' @return キー名配列
Public Function GetRootKeys() As Variant
    If Not m_isParsed Then Exit Function
    
    ' ルートキー取得
    GetRootKeys = m_jsonData.Keys()
    
End Function

' jsonデータを取得します。
' @param[in] none
' @return Dictionaryオブジェクト
Public Function GetData() As Scripting.Dictionary
    If Not m_isParsed Then Exit Function
    
    ' jsonデータ取得
    Set GetData = m_jsonData

End Function

' rootオブジェクトがキーを持つか判定します。
' @param[in] キー名
' @return キーを持っていればTrue,それ以外はFalse
Public Function RootHasKey(ByVal keyName As String) As Variant
    
    RootHasKey = False
    If Not m_isParsed Then Exit Function
    
    ' キー検索
    Dim key_ As Variant
    For Each key_ In m_jsonData.Keys()
        If key_ = keyName Then RootHasKey = True: Exit Function
    Next key_
    
End Function

' jsonファイルを解析します。
' @param[in] none
' @return 成功したらTRUE
' @note
' 1行に付き1つのkey: valueのみ対応 ×{"a": 1, "b": 2} 1行に複数は読めない
' すべて文字列としてDictionaryに保持 ×{"a": true, "b": 2} bool, intで格納しない
' オブジェクト構造のみ対応 ×{"a": [1, 2, 3]} 配列は読めない
Public Function Parse() As Boolean

    Parse = False

    ' 行データとしてセット
    Dim strLines As Variant
    strLines = Split(m_textBuf, m_newLine)
    m_textBuf = ""
    
    Dim curLine As Long: curLine = 0
    Dim nestCnt As Long: nestCnt = 0
    
    ' 解析したObjectをセット
    Dim topObjName As String: topObjName = "root"
    Set m_jsonData = ParseObject(strLines, curLine, nestCnt, topObjName)
    If m_jsonData Is Nothing Then
        MsgBox ("Parse failed.")
        Exit Function
    End If
    
    Parse = True

End Function

' jsonファイルを解析します。
' @param[in] 行配列
' @param[in, out] 現在処理行
' @param[in, out] ネスト数
' @param[out] オブジェクト名
' @return オブジェクトのディクショナリ構造
Private Function ParseObject(ByRef strLines As Variant, ByRef curLine As Long, ByRef nestCnt As Long, Optional ByRef objName As String) As Scripting.Dictionary

    Set ParseObject = Nothing

    If curLine = UBound(strLines) Then Exit Function
    Dim objBuf As Object
    
    ' objectの一時データ
    Dim parentObj As New Scripting.Dictionary

    Dim strLine As Variant
    Dim lineIndex As Long
    For lineIndex = curLine To UBound(strLines)
        strLine = Trim(strLines(lineIndex))
        
        ' オブジェクト開始
        If Left(strLine, 1) = "{" Then
            ' ネストをインクリメント
            nestCnt = nestCnt + 1
            ' 現在行に処理行の次をセット
            curLine = lineIndex + 1
            
            ' オブジェクト解析開始
            Set objBuf = ParseObject(strLines, curLine, nestCnt, objName)
            If objBuf Is Nothing Then
                Exit Function
            End If
            
            ' 親に子オブジェクトをセット
            Set parentObj(objName) = objBuf
            Set objBuf = Nothing
            
            lineIndex = curLine
        
        ' オブジェクト終了
        ElseIf Left(strLine, 1) = "}" Then

            ' ネストをデクリメント
            nestCnt = nestCnt - 1
            ' 現在行に処理行の次をセット
            curLine = lineIndex
            ' オブジェクトをセット
            Set ParseObject = parentObj
            Exit Function

        ' キー取得
        ElseIf 0 < nestCnt And Left(strLine, 1) = """" Then
            
            ' キーの""終わり
            Dim dquoteEndPos As Long
            dquoteEndPos = InStr(2, strLine, """")
            
            ' キー取得
            Dim keyName As String
            keyName = Mid(strLine, 2, dquoteEndPos - 2)
            
            ' コロン
            Dim colonPos As Long
            colonPos = InStr(dquoteEndPos, strLine, ":")
            If colonPos = 0 Then
                MsgBox ("Error: Colon is not found.")
                Exit Function
            End If
            
            ' 値取得
            Dim valStr As String
            valStr = Trim(Mid(strLine, colonPos + 1, Len(strLine) - colonPos))
            If valStr = "" Then
                MsgBox ("Error: Value is not found.")
                Exit Function
            End If
            
            ' 値がオブジェクトの場合
            If Left(valStr, 1) = "{" Then
                ' ネストをインクリメント
                nestCnt = nestCnt + 1
                ' 現在行に処理行の次をセット
                curLine = lineIndex + 1
                
                ' オブジェクト解析開始
                Set objBuf = ParseObject(strLines, curLine, nestCnt, objName)
                If objBuf Is Nothing Then
                    MsgBox ("Error: Object is nothing.")
                    Exit Function
                End If
                
                ' 親に子をセット
                Set parentObj(keyName) = objBuf
                Set objBuf = Nothing
                lineIndex = curLine
            
            ' 値がオブジェクト以外の場合
            Else
                ' 値を補正
                valStr = IIf(Right(valStr, 1) <> ",", valStr, Left(valStr, Len(valStr) - 1))
                If (Left(valStr, 1) = """" And Right(valStr, 1) = """") Then
                    valStr = Mid(valStr, 2, Len(valStr) - 2)
                End If
                
                ' 親にkey,valueをセット
                parentObj(keyName) = valStr
                
            End If
        
        ' 空行
        ElseIf strLine = "" Then
        Else
        End If

    Next lineIndex

    ' return
    Set ParseObject = parentObj
    
End Function

' Dictionaryオブジェクトをプリント出力します。
' @param[in] Dictionaryオブジェクト
' @param[in, out] ネスト数
' @param[in, out] 出力可能文字
' @return 見つかった文字数
Private Sub GetPrintableStr(ByRef dict As Scripting.Dictionary, ByRef nestCnt As Long, ByRef printableStr As String)

    Const tabStr = "    "
    Dim index As Long
    For index = 0 To UBound(dict.Keys())
        ' オブジェクト
        If LCase(typeName(dict(dict.Keys()(index)))) = "dictionary" Then
            
            printableStr = printableStr & IIf(index <> 0, "," & vbCrLf, "") & String(nestCnt, "  ") & """" & dict.Keys()(index) & """: {" & vbCrLf
            
            nestCnt = nestCnt + 1
            Call GetPrintableStr(dict(dict.Keys()(index)), nestCnt, printableStr)
            nestCnt = nestCnt - 1
            
            printableStr = printableStr & vbCrLf & String(nestCnt, tabStr) & "}"
        
        ElseIf LCase(typeName(dict(dict.Keys()(index)))) = "variant()" Then
        ' それ以外
        Else
            printableStr = printableStr & IIf(index <> 0, "," & vbCrLf, "") & String(nestCnt, tabStr) & """" & dict.Keys()(index) & """: """ & dict(dict.Keys()(index)) & """"
        End If
    Next index

End Sub