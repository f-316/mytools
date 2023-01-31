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

' �����o�����Z�b�g���܂��B
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

' ��͍ς݂��m�F���܂��B
' @param[in] none
' @return ��͍ς݂ł����True,����ȊO��False
Public Function IsParsed() As Boolean
    IsParsed = m_isParsed
End Function

' �^�C�g�������g�p���邩�ݒ肵�܂��B
' @param[in] �^�C�g�����g�p����ꍇ��TRUE
' @return none
Public Sub SetUseTitle(ByVal useTitle As Boolean)
    m_useTitle = useTitle
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
    ' Shift-JIS
    Case "sjis", "shiftjis", "shift-jis", "shift_jis"
        m_charSet = "Shift-JIS"
        m_newLine = vbCrLf
    Case Else
        m_charSet = "Shift-JIS"
        m_newLine = vbCrLf
    End Select
    
End Sub

' CSV�t�@�C����ǂݍ��݂܂��B
' @param[in] �t�@�C���t���p�X
' @return ����������TRUE
Public Function Load(ByVal filePath As String) As Boolean

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
    Set fso = Nothing

End Function

' CSV�f�[�^�̐����擾���܂��B
' @param[in] none
' @return �f�[�^��, ���s�����ꍇ��-1
Public Function GetDataCount() As Long
    GetDataCount = -1
    If Not m_isParsed Then Exit Function
    
    ' �f�[�^�����擾
    GetDataCount = m_dataCount

End Function

' CSV�A�C�e������z��Ŏ擾���܂��B
' @param[in] none
' @return �A�C�e�����z��
Public Function GetItems() As Variant
    If Not m_isParsed Then Exit Function
    If Not m_useTitle Then Exit Function
    
    ' �^�C�g���f�[�^�擾
    GetItems = m_titleData

End Function

' CSV�f�[�^���擾���܂��B
' @param[in] none
' @return CSV�f�[�^
Public Function GetData() As Variant
    If Not m_isParsed Then Exit Function
    
    ' csv�f�[�^�擾
    GetData = m_csvData

End Function

' CSV�A�C�e���̃f�[�^�l���擾���܂��B
' @param[in] �A�C�e����
' @param[in] �f�[�^�C���f�b�N�X
' @return �A�C�e���̒l, ������Ȃ��ꍇ��Empty
Public Function GetItemVal(ByVal itemName As String, ByVal dataIndex As Long) As Variant
    GetItemVal = Empty
    If Not m_isParsed Then Exit Function
    
    ' �A�C�e���C���f�b�N�X�擾
    Dim itemIndex As Long
    itemIndex = GetItemIndex(itemName)
    If itemIndex = -1 Then Exit Function
    
    ' �l�擾
    GetItemVal = m_csvData(dataIndex, itemIndex)

End Function

' CSV�e�[�u���̃f�[�^�l���擾���܂��B
' @param[in] �f�[�^��(row��)
' @param[in] �A�C�e����(col��)
' @return �A�C�e���̒l, ������Ȃ��ꍇ��Empty
Public Function GetTableVal(ByVal dataName As String, ByVal itemName As String) As Variant
    GetTableVal = Empty
    If Not m_isParsed Then Exit Function
    
    ' �A�C�e���C���f�b�N�X�擾
    Dim itemIndex As Long
    itemIndex = GetItemIndex(itemName)
    If itemIndex = -1 Then Exit Function
    
    ' �f�[�^�C���f�b�N�X�擾
    Dim dataIndex As Long
    dataIndex = GetDataIndex(dataName)
    If dataIndex = -1 Then Exit Function
    
    ' �l�擾
    GetTableVal = m_csvData(dataIndex, itemIndex)

End Function

' CSV�t�@�C������͂��܂��B
' @param[in] none
' @return ����������TRUE
Public Function Parse() As Boolean

    Parse = False
   
    ' �s�f�[�^�Ƃ��ăZ�b�g
    Dim strLine As Variant
    strLine = Split(m_textBuf, m_newLine)
    
    ' �A�C�e�����擾
    m_itemCount = StrCount(strLine(0), ",") + 1
            
    ' �^�C�g���g�p����1�s�ڂ��^�C�g��
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
    
    ' �f�[�^���m�F
    For rowIndex = startLine To UBound(strLine)
        If InStr(strLine(rowIndex), ",") > 0 Then
            m_dataCount = m_dataCount + 1
        End If
    Next rowIndex
    
    '�擾�����s���2�����z��̍Ē�`
    ReDim m_csvData(0 To m_dataCount - 1, 0 To m_itemCount - 1) As Variant
    
    ' �f�[�^���擾
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

' �A�C�e���̃C���f�b�N�X���擾���܂��B
' @param[in] �A�C�e����
' @return �A�C�e���̃C���f�b�N�X,������Ȃ��ꍇ��-1
Private Function GetItemIndex(ByVal itemName As String) As Long
    GetItemIndex = -1
    If Not m_isParsed Then Exit Function
    If Not m_useTitle Then Exit Function

    ' �^�C�g���z�񂩂��v������̂�����
    Dim itemIndex As Long
    For itemIndex = 0 To UBound(m_titleData)
        If itemName = m_titleData(itemIndex) Then
            GetItemIndex = itemIndex
            Exit For
        End If
    Next itemIndex
        
End Function

' �f�[�^�̃C���f�b�N�X���擾���܂��B
' @param[in] �f�[�^��
' @return �f�[�^�̃C���f�b�N�X,������Ȃ��ꍇ��-1
Private Function GetDataIndex(ByVal dataName As String) As Long
    GetDataIndex = -1
    If Not m_isParsed Then Exit Function
    If Not m_useTitle Then Exit Function

    ' 1��ڂ����v������̂�����
    Dim dataIndex As Long
    For dataIndex = 0 To m_dataCount - 1
        If dataName = m_csvData(dataIndex, 0) Then
            GetDataIndex = dataIndex
            Exit For
        End If
    Next dataIndex
        
End Function