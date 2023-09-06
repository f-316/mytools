VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "BMPInformer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Dim m_filePath As String
Dim m_bufferArray() As Byte

Dim m_itemInfo(1 To 16, 0 To 3) As String

' Class�ł�Type��`�ł��Ȃ�����
Dim m_bfType(1) As Byte ' 0x0000(2)
Dim m_bfSize(3) As Byte ' 0x0002(4)
Dim m_bfReserved1(1) As Byte    ' 0x0006(2)
Dim m_bfReserved2(1) As Byte    ' 0x0008(2)
Dim m_bfOffBits(3) As Byte  ' 0x000A(4)
Dim m_bcSize(3) As Byte ' 0x000E(4)
Dim m_bcWidth(3) As Byte    ' 0x0012(4)
Dim m_bcHeight(3) As Byte   ' 0x0016(4)
Dim m_bcPlanes(1) As Byte   ' 0x001A(2)
Dim m_bcBitCount(1) As Byte ' 0x001C(2)
Dim m_biCompression(3) As Byte  ' 0x001E(4)
Dim m_biSizeImage(3) As Byte    ' 0x0022(4)
Dim m_biXPixPerMeter(3) As Byte ' 0x0026(4)
Dim m_biYPixPerMeter(3) As Byte ' 0x002A(4)
Dim m_biClrUsed(3) As Byte  ' 0x002E(4)
Dim m_biCirImportant(3) As Byte ' 0x0032(4)

Const m_imgHeight As Long = 480
Const m_imgWidth As Long = 496

Private Sub Class_Initialize()
    Call Reset
    
    ' 0:�A�h���X, 1:�T�C�Y, 2: �A�C�e����, 3: ����
    m_itemInfo(1, 0) = "0x0000": m_itemInfo(1, 1) = "2":: m_itemInfo(1, 2) = "bfType": m_itemInfo(1, 3) = "�t�@�C���^�C�v�@�ʏ��'BM'"
    m_itemInfo(2, 0) = "0x0002": m_itemInfo(2, 1) = "4":: m_itemInfo(2, 2) = "bfSize": m_itemInfo(2, 3) = "�t�@�C���T�C�Y (byte)"
    m_itemInfo(3, 0) = "0x0006": m_itemInfo(3, 1) = "2":: m_itemInfo(3, 2) = "bfReserved1": m_itemInfo(3, 3) = "�\��̈�@��� 0"
    m_itemInfo(4, 0) = "0x0008": m_itemInfo(4, 1) = "2":: m_itemInfo(4, 2) = "bfReserved2": m_itemInfo(4, 3) = "�\��̈�@��� 0"
    m_itemInfo(5, 0) = "0x000A": m_itemInfo(5, 1) = "4":: m_itemInfo(5, 2) = "bfOffBits": m_itemInfo(5, 3) = "�t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g (byte)"
    m_itemInfo(6, 0) = "0x000E": m_itemInfo(6, 1) = "4":: m_itemInfo(6, 2) = "bcSize": m_itemInfo(6, 3) = "�w�b�_�T�C�Y,40 - Windows Bitmap,12 - OS/2 Bitmap"
    m_itemInfo(7, 0) = "0x0012": m_itemInfo(7, 1) = "4":: m_itemInfo(7, 2) = "bcWidth": m_itemInfo(7, 3) = "�摜�̕� (�s�N�Z��)"
    m_itemInfo(8, 0) = "0x0016": m_itemInfo(8, 1) = "4":: m_itemInfo(8, 2) = "bcHeight": m_itemInfo(8, 3) = "�摜�̍��� (�s�N�Z��),bcHeight �̒l�������Ȃ�C�摜�f�[�^�͉�������,bcHeight �̒l�������Ȃ�C�摜�f�[�^�͏ォ�牺��"
    m_itemInfo(9, 0) = "0x001A": m_itemInfo(9, 1) = "2":: m_itemInfo(9, 2) = "bcPlanes": m_itemInfo(9, 3) = "�v���[�����@��� 1"
    m_itemInfo(10, 0) = "0x001C": m_itemInfo(10, 1) = "2":: m_itemInfo(10, 2) = "bcBitCount": m_itemInfo(10, 3) = "1��f������̃f�[�^�T�C�Y (bit),��j256 �F�r�b�g�}�b�v �� 8"
    m_itemInfo(11, 0) = "0x001E": m_itemInfo(11, 1) = "4":: m_itemInfo(11, 2) = "biCompression": m_itemInfo(11, 3) = "���k�`��,0 - BI_RGB (�����k),1 - BI_RLE8 (RunLength 8 bits/pixel),2 - BI_RLE4 (RunLength 4 bits/pixel),3 - BI_BITFIELDS (Bitfields),(4 - BI_JPEG),(5 - BI_PNG)"
    m_itemInfo(12, 0) = "0x0022": m_itemInfo(12, 1) = "4":: m_itemInfo(12, 2) = "biSizeImage": m_itemInfo(12, 3) = "�摜�f�[�^���̃T�C�Y (byte),96dpi �Ȃ��3780,0 �̏ꍇ������,��biCompression �� 0 �ȊO�̏ꍇ�A �����̒l�� 0 �ɂ��邱�Ƃ͏o���Ȃ�"
    m_itemInfo(13, 0) = "0x0026": m_itemInfo(13, 1) = "4":: m_itemInfo(13, 2) = "biXPixPerMeter": m_itemInfo(13, 3) = "�������𑜓x (1m������̉�f��),96dpi �Ȃ��37800 �̏ꍇ������"
    m_itemInfo(14, 0) = "0x002A": m_itemInfo(14, 1) = "4":: m_itemInfo(14, 2) = "biYPixPerMeter": m_itemInfo(14, 3) = "�c�����𑜓x (1m������̉�f��),96dpi �Ȃ��3780,0 �̏ꍇ������"
    m_itemInfo(15, 0) = "0x002E": m_itemInfo(15, 1) = "4":: m_itemInfo(15, 2) = "biClrUsed": m_itemInfo(15, 3) = "�i�[����Ă���p���b�g�� (�g�p�F��),0 �̏ꍇ������"
    m_itemInfo(16, 0) = "0x0032": m_itemInfo(16, 1) = "4":: m_itemInfo(16, 2) = "biCirImportant": m_itemInfo(16, 3) = "�d�v�ȃp���b�g�̃C���f�b�N�X,0 �̏ꍇ������"

End Sub

Private Sub Class_Terminate()
    Call Reset
End Sub

' �����o�����Z�b�g���܂��B
' @param[in] none
' @return none
Private Sub Reset()
    m_filePath = ""
    Erase m_bufferArray
    
End Sub

' �t�@�C����ǂݍ���Ńo�b�t�@�ɕێ����܂��B
' @param[in] �t�@�C����
' @return ����I��������True,����ȊO��False
Public Function ReadFile(Optional ByVal filePath As String = "") As Boolean
    
    If filePath <> "" Then
        m_filePath = filePath
    End If
    
    ReadFile = False
            
    On Error GoTo MyError
    
    '// �t�@�C���ԍ��擾
    Dim fn As Integer
    fn = FreeFile
    
    '// �t�@�C���I�[�v��
    Open m_filePath For Binary Access Read As #fn
        
    '// �t�@�C���T�C�Y�擾
    Dim fileSize As Long
    fileSize = LOF(fn)
'    fileSize = FileLen(m_filePath)
    
    '// �z��̗̈�m��
    ReDim m_bufferArray(0 To fileSize - 1)
    
    '// �o�C�i���f�[�^�ǂݍ���
    m_bufferArray = InputB(fileSize, fn)
    
    Close #fn
    
    ' �����o�ɏ��i�[
    Call Import
    
    ReadFile = True
    Exit Function

MyError:
    Debug.Print Err.Number; ":"; Err.Description
    Debug.Print m_filePath
    MsgBox (Err.Number & ":" & Err.Description)

End Function

' �����Z���ɏ������݂܂��B
' @param[in] �J�n�Z���ʒu
' @return ����I��������True,����ȊO��False
Public Function ToShtBmpHeader(ByRef startRng As Range) As Boolean

    ToShtBmpHeader = False

    ' �A�h���X,�T�C�Y,����,����,�l0,�l1,�l2,�l3
    Dim celldata(1 To 16, 1 To 9) As Variant
        
    Dim iRow As Long
    For iRow = 1 To 16
        celldata(iRow, 1) = m_itemInfo(iRow, 0) ' �A�h���X
        celldata(iRow, 2) = m_itemInfo(iRow, 1) ' �T�C�Y
        celldata(iRow, 3) = m_itemInfo(iRow, 2) ' ����
        celldata(iRow, 4) = m_itemInfo(iRow, 3) ' ����
        
        If 1 = iRow Then
            celldata(iRow, 9) = "=CHAR(INDIRECT(""RC[-4]"", FALSE))&CHAR(INDIRECT(""RC[-3]"", FALSE))"
        Else
            celldata(iRow, 9) = "=INDIRECT(""RC[-4]"", FALSE) + BITLSHIFT(INDIRECT(""RC[-3]"", FALSE), 8) + BITLSHIFT(INDIRECT(""RC[-2]"", FALSE), 16) + BITLSHIFT(INDIRECT(""RC[-1]"", FALSE), 24)"
        End If
    Next iRow

    ' �o�b�t�@���������荞��
    Dim readIndex As Long: readIndex = 0
    Dim dataSize As Integer
    Dim dataIndex As Integer
    For iRow = 1 To 16
        
        ' �f�[�^�T�C�Y�擾
        dataSize = Int(m_itemInfo(iRow, 1))
        If 4 < dataSize Then
            MsgBox ("Not support dataSize=" & dataSize)
            Exit Function
        End If
        
        ' �o�b�t�@����1�o�C�g�����o��
        For dataIndex = 0 To dataSize - 1
            celldata(iRow, dataIndex + 5) = m_bufferArray(readIndex)
            readIndex = readIndex + 1
        Next dataIndex

    Next iRow
        
    ' ���T�C�Y
    Set startRng = startRng.Resize(16, 9)
    Debug.Print startRng.Rows.Count
    Debug.Print startRng.Columns.Count
    
    ' �Z�b�g
    startRng = celldata

    ToShtBmpHeader = True

End Function

' Bmp8���������݂܂��B
' @param[in] �J�n�Z���ʒu
' @return ����I��������True,����ȊO��False
Public Function ToShtBmp8Data(ByRef startRng As Range) As Boolean
    
    ToShtBmp8Data = False
    
    ' 496�~480�Œ�
    Dim celldata(1 To m_imgHeight, 1 To m_imgWidth) As Variant

    ' �摜�f�[�^�J�n�ʒu
    Dim readIndex As Long
    readIndex = m_bfOffBits(0) + m_bfOffBits(1) * (2 ^ 8) + m_bfOffBits(2) * (2 ^ 16) + m_bfOffBits(3) * (2 ^ 24)
    
    ' �o�b�t�@���������荞��
    Dim iRow As Integer
    Dim iColumn As Integer
    For iRow = m_imgHeight To 1 Step -1
        For iColumn = 1 To m_imgWidth
            ' �o�b�t�@����1�o�C�g�����o��
            celldata(iRow, iColumn) = m_bufferArray(readIndex)
            readIndex = readIndex + 1
        Next iColumn
    Next iRow
        
    ' ���T�C�Y
    Set startRng = startRng.Resize(m_imgHeight, m_imgWidth)
    
    ' �Z�b�g
    startRng = celldata
    
    ToShtBmp8Data = True

End Function

' �Z�������荞�񂾃f�[�^���t�@�C���ɏ������݂܂��B
' @param[in] �J�n�Z���ʒu
' @param[opt] �t�@�C���p�X,��荞�ݎ��ƈႤ�t�@�C���ɏ������ޏꍇ�͒l���Z�b�g
' @return ����I��������True,����ȊO��False
Public Function WriteBmp8Data(ByRef startRng As Range, Optional ByVal filePath As String) As Boolean
    
    WriteBmp8Data = False

    ' �摜�f�[�^�J�n�ʒu
    Dim readIndex As Long
    readIndex = m_bfOffBits(0) + m_bfOffBits(1) * (2 ^ 8) + m_bfOffBits(2) * (2 ^ 16) + m_bfOffBits(3) * (2 ^ 24)

    ' ���T�C�Y
    Set startRng = startRng.Resize(m_imgHeight, m_imgWidth)
    Dim celldata As Variant
    celldata = startRng.Value

    ' �o�b�t�@���������荞��
    Dim iRow As Integer
    Dim iColumn As Integer
    For iRow = m_imgHeight To 1 Step -1
        For iColumn = 1 To m_imgWidth
            ' �o�b�t�@����1�o�C�g�����o��
            m_bufferArray(readIndex) = celldata(iRow, iColumn)
            readIndex = readIndex + 1
        Next iColumn
    Next iRow

    On Error GoTo MyError
    
    '// �t�@�C���ԍ��擾
    Dim fn As Integer
    fn = FreeFile
    
    ' �t�@�C����
    If filePath = "" Then
        filePath = m_filePath
    End If
    
    '// �t�@�C���I�[�v��
    Open filePath For Binary Access Write As #fn
        
    '// �t�@�C���T�C�Y�擾
    Dim fileSize As Long
    fileSize = LOF(fn)
'    fileSize = FileLen(m_filePath)
    
    ' ��������
    Put #fn, , m_bufferArray
    
    Close #fn

    WriteBmp8Data = True
    Exit Function

MyError:
    Debug.Print Err.Number; ":"; Err.Description
    Debug.Print m_filePath
    MsgBox (Err.Number & ":" & Err.Description)

End Function

' �o�b�t�@���烁���o�ɃC���|�[�g���܂��B
' @param[in] none
' @return ����I��������True,����ȊO��False
Private Function Import() As Boolean

    Import = False

    Dim readIndex As Long: readIndex = 0

    ' m_bfType(1) As Byte ' 0x0000(2)
    m_bfType(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfType(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    
    ' m_bfSize(3) As Byte ' 0x0002(4)
    m_bfSize(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfSize(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfSize(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfSize(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_bfReserved1(1) As Byte    ' 0x0006(2)
    m_bfReserved1(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfReserved1(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
        
    ' m_bfReserved2(1) As Byte    ' 0x0008(2)
    m_bfReserved2(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfReserved2(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    
    ' m_bfOffBits(3) As Byte  ' 0x000A(4)
    m_bfOffBits(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfOffBits(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfOffBits(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bfOffBits(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_bcSize(3) As Byte ' 0x000E(4)
    m_bcSize(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcSize(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcSize(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcSize(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_bcWidth(3) As Byte    ' 0x0012(4)
    m_bcWidth(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcWidth(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcWidth(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcWidth(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_bcHeight(3) As Byte   ' 0x0016(4)
    m_bcHeight(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcHeight(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcHeight(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcHeight(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_bcPlanes(1) As Byte   ' 0x001A(2)
    m_bcPlanes(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcPlanes(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_bcBitCount(1) As Byte ' 0x001C(2)
    m_bcBitCount(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_bcBitCount(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_biCompression(3) As Byte  ' 0x001E(4)
    m_biCompression(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biCompression(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biCompression(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biCompression(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_biSizeImage(3) As Byte    ' 0x0022(4)
    m_biSizeImage(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biSizeImage(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biSizeImage(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biSizeImage(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_biXPixPerMeter(3) As Byte ' 0x0026(4)
    m_biXPixPerMeter(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biXPixPerMeter(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biXPixPerMeter(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biXPixPerMeter(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_biYPixPerMeter(3) As Byte ' 0x002A(4)
    m_biYPixPerMeter(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biYPixPerMeter(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biYPixPerMeter(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biYPixPerMeter(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    ' m_biClrUsed(3) As Byte  ' 0x002E(4)
    m_biClrUsed(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biClrUsed(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biClrUsed(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biClrUsed(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    
    ' m_biCirImportant(3) As Byte ' 0x0032(4)
    m_biCirImportant(0) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biCirImportant(1) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biCirImportant(2) = m_bufferArray(readIndex)
    readIndex = readIndex + 1
    m_biCirImportant(3) = m_bufferArray(readIndex)
    readIndex = readIndex + 1

    Import = True

    '// �P�o�C�g���ǂݍ���ŏo��
'    Dim byData As Byte
'    Dim i As Long
'    For i = 0 To fileSize - 1
'        Debug.Print "No." & CStr(i) & " - 0x" & byData & " - Char[" & Chr(byData) & "]"
'    Next
    
End Function