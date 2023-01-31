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

' �����o�����Z�b�g���܂��B
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

' ��͍ς݂��m�F���܂��B
' @param[in] none
' @return ��͍ς݂ł����True,����ȊO��False
Public Function IsParsed() As Boolean
    IsParsed = m_isParsed
End Function

' �����Z�b�g��ݒ肵�܂��B
' @param[in] �����Z�b�g
' @param[in] ���s�R�[�h�A��(�����̉��s�R�[�h���Z�b�g)
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

' ���s�R�[�h��ݒ肵�܂��B
' @param[in] ���s�R�[�h
' @return none
Public Sub SetNewLine(ByVal newLine As String)
    m_newLine = newLine
End Sub

' json�t�@�C����ǂݍ��݂܂��B
' @param[in] �t�@�C���t���p�X
' @return ����������TRUE
Public Function Load(ByVal filePath) As Boolean

    ' ���΃p�X���΃p�X�ɕϊ�
    Dim fso As Object
    Set fso = CreateObject("Scripting.FileSystemObject")
    If Left(filePath, 1) = "." Then
        filePath = ThisWorkbook.Path & "\" & filePath
    End If
    filePath = fso.GetAbsolutePathName(filePath)
    
    ' �s���擾
    m_lineCount = CreateObject("Scripting.FileSystemObject").OpenTextFile(filePath, 8).Line '�t�@�C���̍s���擾

    ' �f�[�^�ǂݍ���
    With CreateObject("ADODB.Stream")
        .charSet = m_charSet
        .Open
        .LoadFromFile filePath
        m_textBuf = .ReadText
        .Close
    End With
    
    ' ���
    m_isParsed = Parse()
    Load = m_isParsed
    
End Function

' json���e�L�X�g�f�[�^�Ŏ擾���܂��B
' @param[in] none
' @return json�e�L�X�g
Public Function GetJsonStr() As String
    
    GetJsonStr = ""
    If Not m_isParsed Then Exit Function

    ' �o�͉\�����ɕϊ�
    Dim nestCnt As Long: nestCnt = 0
    Dim str As String
    Call GetPrintableStr(m_jsonData, nestCnt, str)

    ' return
    GetJsonStr = str

End Function

' json�f�[�^����root�I�u�W�F�N�g�����L�[��z��Ŏ擾���܂��B
' @param[in] none
' @return �L�[���z��
Public Function GetRootKeys() As Variant
    If Not m_isParsed Then Exit Function
    
    ' ���[�g�L�[�擾
    GetRootKeys = m_jsonData.Keys()
    
End Function

' json�f�[�^���擾���܂��B
' @param[in] none
' @return Dictionary�I�u�W�F�N�g
Public Function GetData() As Scripting.Dictionary
    If Not m_isParsed Then Exit Function
    
    ' json�f�[�^�擾
    Set GetData = m_jsonData

End Function

' root�I�u�W�F�N�g���L�[���������肵�܂��B
' @param[in] �L�[��
' @return �L�[�������Ă����True,����ȊO��False
Public Function RootHasKey(ByVal keyName As String) As Variant
    
    RootHasKey = False
    If Not m_isParsed Then Exit Function
    
    ' �L�[����
    Dim key_ As Variant
    For Each key_ In m_jsonData.Keys()
        If key_ = keyName Then RootHasKey = True: Exit Function
    Next key_
    
End Function

' json�t�@�C������͂��܂��B
' @param[in] none
' @return ����������TRUE
' @note
' 1�s�ɕt��1��key: value�̂ݑΉ� �~{"a": 1, "b": 2} 1�s�ɕ����͓ǂ߂Ȃ�
' ���ׂĕ�����Ƃ���Dictionary�ɕێ� �~{"a": true, "b": 2} bool, int�Ŋi�[���Ȃ�
' �I�u�W�F�N�g�\���̂ݑΉ� �~{"a": [1, 2, 3]} �z��͓ǂ߂Ȃ�
Public Function Parse() As Boolean

    Parse = False

    ' �s�f�[�^�Ƃ��ăZ�b�g
    Dim strLines As Variant
    strLines = Split(m_textBuf, m_newLine)
    m_textBuf = ""
    
    Dim curLine As Long: curLine = 0
    Dim nestCnt As Long: nestCnt = 0
    
    ' ��͂���Object���Z�b�g
    Dim topObjName As String: topObjName = "root"
    Set m_jsonData = ParseObject(strLines, curLine, nestCnt, topObjName)
    If m_jsonData Is Nothing Then
        MsgBox ("Parse failed.")
        Exit Function
    End If
    
    Parse = True

End Function

' json�t�@�C������͂��܂��B
' @param[in] �s�z��
' @param[in, out] ���ݏ����s
' @param[in, out] �l�X�g��
' @param[out] �I�u�W�F�N�g��
' @return �I�u�W�F�N�g�̃f�B�N�V���i���\��
Private Function ParseObject(ByRef strLines As Variant, ByRef curLine As Long, ByRef nestCnt As Long, Optional ByRef objName As String) As Scripting.Dictionary

    Set ParseObject = Nothing

    If curLine = UBound(strLines) Then Exit Function
    Dim objBuf As Object
    
    ' object�̈ꎞ�f�[�^
    Dim parentObj As New Scripting.Dictionary

    Dim strLine As Variant
    Dim lineIndex As Long
    For lineIndex = curLine To UBound(strLines)
        strLine = Trim(strLines(lineIndex))
        
        ' �I�u�W�F�N�g�J�n
        If Left(strLine, 1) = "{" Then
            ' �l�X�g���C���N�������g
            nestCnt = nestCnt + 1
            ' ���ݍs�ɏ����s�̎����Z�b�g
            curLine = lineIndex + 1
            
            ' �I�u�W�F�N�g��͊J�n
            Set objBuf = ParseObject(strLines, curLine, nestCnt, objName)
            If objBuf Is Nothing Then
                Exit Function
            End If
            
            ' �e�Ɏq�I�u�W�F�N�g���Z�b�g
            Set parentObj(objName) = objBuf
            Set objBuf = Nothing
            
            lineIndex = curLine
        
        ' �I�u�W�F�N�g�I��
        ElseIf Left(strLine, 1) = "}" Then

            ' �l�X�g���f�N�������g
            nestCnt = nestCnt - 1
            ' ���ݍs�ɏ����s�̎����Z�b�g
            curLine = lineIndex
            ' �I�u�W�F�N�g���Z�b�g
            Set ParseObject = parentObj
            Exit Function

        ' �L�[�擾
        ElseIf 0 < nestCnt And Left(strLine, 1) = """" Then
            
            ' �L�[��""�I���
            Dim dquoteEndPos As Long
            dquoteEndPos = InStr(2, strLine, """")
            
            ' �L�[�擾
            Dim keyName As String
            keyName = Mid(strLine, 2, dquoteEndPos - 2)
            
            ' �R����
            Dim colonPos As Long
            colonPos = InStr(dquoteEndPos, strLine, ":")
            If colonPos = 0 Then
                MsgBox ("Error: Colon is not found.")
                Exit Function
            End If
            
            ' �l�擾
            Dim valStr As String
            valStr = Trim(Mid(strLine, colonPos + 1, Len(strLine) - colonPos))
            If valStr = "" Then
                MsgBox ("Error: Value is not found.")
                Exit Function
            End If
            
            ' �l���I�u�W�F�N�g�̏ꍇ
            If Left(valStr, 1) = "{" Then
                ' �l�X�g���C���N�������g
                nestCnt = nestCnt + 1
                ' ���ݍs�ɏ����s�̎����Z�b�g
                curLine = lineIndex + 1
                
                ' �I�u�W�F�N�g��͊J�n
                Set objBuf = ParseObject(strLines, curLine, nestCnt, objName)
                If objBuf Is Nothing Then
                    MsgBox ("Error: Object is nothing.")
                    Exit Function
                End If
                
                ' �e�Ɏq���Z�b�g
                Set parentObj(keyName) = objBuf
                Set objBuf = Nothing
                lineIndex = curLine
            
            ' �l���I�u�W�F�N�g�ȊO�̏ꍇ
            Else
                ' �l��␳
                valStr = IIf(Right(valStr, 1) <> ",", valStr, Left(valStr, Len(valStr) - 1))
                If (Left(valStr, 1) = """" And Right(valStr, 1) = """") Then
                    valStr = Mid(valStr, 2, Len(valStr) - 2)
                End If
                
                ' �e��key,value���Z�b�g
                parentObj(keyName) = valStr
                
            End If
        
        ' ��s
        ElseIf strLine = "" Then
        Else
        End If

    Next lineIndex

    ' return
    Set ParseObject = parentObj
    
End Function

' Dictionary�I�u�W�F�N�g���v�����g�o�͂��܂��B
' @param[in] Dictionary�I�u�W�F�N�g
' @param[in, out] �l�X�g��
' @param[in, out] �o�͉\����
' @return ��������������
Private Sub GetPrintableStr(ByRef dict As Scripting.Dictionary, ByRef nestCnt As Long, ByRef printableStr As String)

    Const tabStr = "    "
    Dim index As Long
    For index = 0 To UBound(dict.Keys())
        ' �I�u�W�F�N�g
        If LCase(typeName(dict(dict.Keys()(index)))) = "dictionary" Then
            
            printableStr = printableStr & IIf(index <> 0, "," & vbCrLf, "") & String(nestCnt, "  ") & """" & dict.Keys()(index) & """: {" & vbCrLf
            
            nestCnt = nestCnt + 1
            Call GetPrintableStr(dict(dict.Keys()(index)), nestCnt, printableStr)
            nestCnt = nestCnt - 1
            
            printableStr = printableStr & vbCrLf & String(nestCnt, tabStr) & "}"
        
        ElseIf LCase(typeName(dict(dict.Keys()(index)))) = "variant()" Then
        ' ����ȊO
        Else
            printableStr = printableStr & IIf(index <> 0, "," & vbCrLf, "") & String(nestCnt, tabStr) & """" & dict.Keys()(index) & """: """ & dict(dict.Keys()(index)) & """"
        End If
    Next index

End Sub