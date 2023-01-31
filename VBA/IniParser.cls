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

' �����o�����Z�b�g���܂��B
' @param[in] none
' @return none
Private Sub Reset()
  m_newLine = vbCrLf
  m_charSet = "Shift-JIS"
  m_isParsed = False
  m_dataCount = 0
  m_itemCount = 0
End Sub

' �����Z�b�g��ݒ肵�܂��B
' @param[in] �����Z�b�g
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

' ini�t�@�C����ǂݍ��݂܂��B
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

' �t�@�C���ɏ����o���܂��B
' @param[in] Dictionary�f�[�^,1�K�w�ڂ�Section,2�K�w�ڂ�key=value�Ƃ��Ĉ����܂��B
' @param[in] �o�͐�t�@�C���p�X
' @return ����������True,����ȊOFalse
Public Function WriteFile(ByRef iniData As Object, ByRef filePath As String) As Boolean
    
    WriteFile = False
    
    Dim writeStr As String
    writeStr = MakeIniText(iniData)
    
    ' 0:���s����, 1:���s����
    Dim writeMode As Long
    writeMode = 0
    
    ' 1:�t�@�C�������݂���ꍇ���s�G���[, 2:�㏑��
    Dim saveMode As Long
    saveMode = 2
    
    ' �t�@�C�����J��
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

' ini�f�[�^����Section��z��Ŏ擾���܂��B
' @param[in] none
' @return Section���z��
Public Function GetSections() As Variant
    If Not m_isParsed Then Exit Function
    
    ' �Z�N�V�����f�[�^�擾
    GetSections = m_iniData.Keys
    
End Function

' ini�f�[�^���̎w�肵��Section��key��z��Ŏ擾���܂��B
' @param[in] none
' @return key���z��
Public Function GetKeys(ByVal sectionName As String) As Variant
    If Not m_isParsed Then Exit Function
    
    ' key�f�[�^�擾
    If Not HasSection(sectionName) Then Exit Function
    GetKeys = m_iniData(sectionName).Keys
    
End Function

' ini�f�[�^���擾���܂��B
' @param[in] none
' @return ini�f�[�^
Public Function GetData() As Object
    If Not m_isParsed Then Exit Function
    
    ' ini�f�[�^�擾
    Set GetData = m_iniData

End Function

' ini�f�[�^�l���擾���܂��B
' @param[in] �Z�N�V������
' @param[in] �L�[��
' @return �l
Public Function GetVal(ByVal sectionName As String, ByVal keyName As String) As Variant
    
    GetVal = Empty
    If Not m_isParsed Then Exit Function
    If Not HasSection(sectionName) Then Exit Function
        
    ' ini�f�[�^�擾
    GetVal = m_iniData(sectionName)(keyName)

End Function

' �Z�N�V�������������肵�܂��B
' @param[in] �Z�N�V������
' @return �Z�N�V�����������Ă����True,����ȊO��False
Public Function HasSection(ByVal sectionName As String) As Variant
    
    HasSection = False
    If Not m_isParsed Then Exit Function
    
    ' �Z�N�V��������
    Dim sections As Variant
    Dim section As Variant
    sections = m_iniData.Keys
    For Each section In sections
        If section = sectionName Then HasSection = True: Exit Function
    Next section
    
End Function

' �L�[���������肵�܂��B
' @param[in] �Z�N�V������
' @param[in] �L�[��
' @return �L�[�������Ă����True,����ȊO��False
Public Function HasKey(ByVal sectionName As String, ByVal keyName As String) As Variant
    
    HasKey = False
    If Not m_isParsed Then Exit Function
    If Not HasSection(sectionName) Then Exit Function
        
    ' �L�[����
    Dim keys_ As Variant
    Dim key_ As Variant
    keys_ = m_iniData(sectionName).Keys
    For Each key_ In keys_
        If key_ = keyName Then HasKey = True: Exit Function
    Next key_

End Function

' ini�t�@�C������͂��܂��B
' @param[in] none
' @return ����������TRUE
Public Function Parse() As Boolean

    Parse = False
   
    ' ini�`���f�[�^
    Set m_iniData = CreateObject("Scripting.Dictionary")
        
    ' section�̈ꎞ�f�[�^
    Dim sectBuf As Object
    Set sectBuf = CreateObject("Scripting.Dictionary")

    ' �s�f�[�^�Ƃ��ăZ�b�g
    Dim strLine As Variant
    strLine = Split(m_textBuf, m_newLine)
    
    ' �f�[�^���擾
    Dim curSectionName As String: curSectionName = ""
    Dim strBuf As String
    Dim lineIndex As Long
    For lineIndex = 0 To UBound(strLine)
        strBuf = Trim(strLine(lineIndex))
        
        ' ��������s
        If Left(strBuf, 1) = "#" Then
        ElseIf Left(strBuf, 1) = ";" Then
        ElseIf Left(strBuf, 1) = "" Then
        
        ' �Z�N�V�������Z�b�g
        ElseIf Left(strBuf, 1) = "[" Then
            
            ' ���ɃZ�N�V������������ꍇ
            If curSectionName <> "" Then
                ' �Z�N�V�������e�Z�b�g
                Set m_iniData(curSectionName) = sectBuf
            End If
            
            ' �Z�N�V�������e������
            Set sectBuf = CreateObject("Scripting.Dictionary")
            
            ' �e�L�X�g����Z�N�V�������擾
            curSectionName = ToSectionName(strBuf)
            If curSectionName = "" Then MsgBox ("Section Name is invalid"): Exit Function
            
            ' �Z�N�V���������Z�b�g
            Set m_iniData(curSectionName) = Nothing
        
        ' �l�Z�b�g
        Else
            If Not SetKeyValue(sectBuf, strBuf) Then MsgBox ("KeyValue is invalid"): Exit Function
        End If
    

    Next lineIndex

    ' �Ō�̃Z�N�V�������e�Z�b�g
    Set m_iniData(curSectionName) = sectBuf

    Set sectBuf = Nothing
    Parse = True

End Function

' �e�L�X�g��Section���ɕϊ����܂��B
' @param[in] Seciton���e�L�X�g
' @return Section��,�擾�ł��Ȃ��ꍇ�͋�
Private Function ToSectionName(ByVal sectionStr As String) As String
    
    ToSectionName = ""
    
    ' "["��"]"���܂܂�邩�m�F
    If InStr(sectionStr, "[") <= 0 Then Exit Function
    If InStr(sectionStr, "]") <= 0 Then Exit Function
    
    ' section�����Z�b�g
    sectionStr = Replace(sectionStr, "[", "")
    sectionStr = Replace(sectionStr, "]", "")
    sectionStr = Trim(sectionStr)
    
    ToSectionName = sectionStr

End Function

' �Z�N�V����Dictionary��KeyValue���Z�b�g���܂��B
' @param[in, out] �Z�N�V����Dictionary
' @param[in] KeyValue���e�L�X�g
' @return ����������True
Private Function SetKeyValue(ByRef sectDict As Object, ByVal keyValueStr As String)
    
    SetKeyValue = False
    
    ' "="���܂܂�邩�m�F
    If InStr(keyValueStr, "=") <= 0 Then Exit Function
    
    ' key,value�Ƃ��ăZ�b�g
    Dim keyValueArr As Variant
    keyValueArr = Split(keyValueStr, "=")
    sectDict(Trim(keyValueArr(0))) = Trim(keyValueArr(1))

    SetKeyValue = True

End Function

' ���������J�E���g���܂��B
' @param[in] �Ώە�����
' @param[in] ����������
' @return ��������������
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

' ini�f�[�^(Dictionary�I�u�W�F�N�g)����e�L�X�g�𐶐����܂��B
' @param[in] Dictionary�f�[�^,1�K�w�ڂ�Section,2�K�w�ڂ�key=value�Ƃ��Ĉ����܂��B
' @return ���������e�L�X�g�f�[�^
Private Function MakeIniText(ByRef iniData As Scripting.Dictionary) As String
    
    MakeIniText = ""
    
    Dim iniText As String

    Dim section_ As Variant
    For Each section_ In iniData.Keys()
        
        ' �Z�N�V������
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