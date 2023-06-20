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

' ClassではType定義できないため
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
    
    ' 0:アドレス, 1:サイズ, 2: アイテム名, 3: 説明
    m_itemInfo(1, 0) = "0x0000": m_itemInfo(1, 1) = "2":: m_itemInfo(1, 2) = "bfType": m_itemInfo(1, 3) = "ファイルタイプ　通常は'BM'"
    m_itemInfo(2, 0) = "0x0002": m_itemInfo(2, 1) = "4":: m_itemInfo(2, 2) = "bfSize": m_itemInfo(2, 3) = "ファイルサイズ (byte)"
    m_itemInfo(3, 0) = "0x0006": m_itemInfo(3, 1) = "2":: m_itemInfo(3, 2) = "bfReserved1": m_itemInfo(3, 3) = "予約領域　常に 0"
    m_itemInfo(4, 0) = "0x0008": m_itemInfo(4, 1) = "2":: m_itemInfo(4, 2) = "bfReserved2": m_itemInfo(4, 3) = "予約領域　常に 0"
    m_itemInfo(5, 0) = "0x000A": m_itemInfo(5, 1) = "4":: m_itemInfo(5, 2) = "bfOffBits": m_itemInfo(5, 3) = "ファイル先頭から画像データまでのオフセット (byte)"
    m_itemInfo(6, 0) = "0x000E": m_itemInfo(6, 1) = "4":: m_itemInfo(6, 2) = "bcSize": m_itemInfo(6, 3) = "ヘッダサイズ,40 - Windows Bitmap,12 - OS/2 Bitmap"
    m_itemInfo(7, 0) = "0x0012": m_itemInfo(7, 1) = "4":: m_itemInfo(7, 2) = "bcWidth": m_itemInfo(7, 3) = "画像の幅 (ピクセル)"
    m_itemInfo(8, 0) = "0x0016": m_itemInfo(8, 1) = "4":: m_itemInfo(8, 2) = "bcHeight": m_itemInfo(8, 3) = "画像の高さ (ピクセル),bcHeight の値が正数なら，画像データは下から上へ,bcHeight の値が負数なら，画像データは上から下へ"
    m_itemInfo(9, 0) = "0x001A": m_itemInfo(9, 1) = "2":: m_itemInfo(9, 2) = "bcPlanes": m_itemInfo(9, 3) = "プレーン数　常に 1"
    m_itemInfo(10, 0) = "0x001C": m_itemInfo(10, 1) = "2":: m_itemInfo(10, 2) = "bcBitCount": m_itemInfo(10, 3) = "1画素あたりのデータサイズ (bit),例）256 色ビットマップ ＝ 8"
    m_itemInfo(11, 0) = "0x001E": m_itemInfo(11, 1) = "4":: m_itemInfo(11, 2) = "biCompression": m_itemInfo(11, 3) = "圧縮形式,0 - BI_RGB (無圧縮),1 - BI_RLE8 (RunLength 8 bits/pixel),2 - BI_RLE4 (RunLength 4 bits/pixel),3 - BI_BITFIELDS (Bitfields),(4 - BI_JPEG),(5 - BI_PNG)"
    m_itemInfo(12, 0) = "0x0022": m_itemInfo(12, 1) = "4":: m_itemInfo(12, 2) = "biSizeImage": m_itemInfo(12, 3) = "画像データ部のサイズ (byte),96dpi ならば3780,0 の場合もある,※biCompression が 0 以外の場合、 ここの値を 0 にすることは出来ない"
    m_itemInfo(13, 0) = "0x0026": m_itemInfo(13, 1) = "4":: m_itemInfo(13, 2) = "biXPixPerMeter": m_itemInfo(13, 3) = "横方向解像度 (1mあたりの画素数),96dpi ならば37800 の場合もある"
    m_itemInfo(14, 0) = "0x002A": m_itemInfo(14, 1) = "4":: m_itemInfo(14, 2) = "biYPixPerMeter": m_itemInfo(14, 3) = "縦方向解像度 (1mあたりの画素数),96dpi ならば3780,0 の場合もある"
    m_itemInfo(15, 0) = "0x002E": m_itemInfo(15, 1) = "4":: m_itemInfo(15, 2) = "biClrUsed": m_itemInfo(15, 3) = "格納されているパレット数 (使用色数),0 の場合もある"
    m_itemInfo(16, 0) = "0x0032": m_itemInfo(16, 1) = "4":: m_itemInfo(16, 2) = "biCirImportant": m_itemInfo(16, 3) = "重要なパレットのインデックス,0 の場合もある"

End Sub

Private Sub Class_Terminate()
    Call Reset
End Sub

' メンバをリセットします。
' @param[in] none
' @return none
Private Sub Reset()
    m_filePath = ""
    Erase m_bufferArray
    
End Sub

' ファイルを読み込んでバッファに保持します。
' @param[in] ファイル名
' @return 正常終了したらTrue,それ以外はFalse
Public Function ReadFile(Optional ByVal filePath As String = "") As Boolean
    
    If filePath <> "" Then
        m_filePath = filePath
    End If
    
    ReadFile = False
            
    On Error GoTo MyError
    
    '// ファイル番号取得
    Dim fn As Integer
    fn = FreeFile
    
    '// ファイルオープン
    Open m_filePath For Binary Access Read As #fn
        
    '// ファイルサイズ取得
    Dim fileSize As Long
    fileSize = LOF(fn)
'    fileSize = FileLen(m_filePath)
    
    '// 配列の領域確保
    ReDim m_bufferArray(0 To fileSize - 1)
    
    '// バイナリデータ読み込み
    m_bufferArray = InputB(fileSize, fn)
    
    Close #fn
    
    ' メンバに情報格納
    Call Import
    
    ReadFile = True
    Exit Function

MyError:
    Debug.Print Err.Number; ":"; Err.Description
    Debug.Print m_filePath
    MsgBox (Err.Number & ":" & Err.Description)

End Function

' 情報をセルに書き込みます。
' @param[in] 開始セル位置
' @return 正常終了したらTrue,それ以外はFalse
Public Function ToShtBmpHeader(ByRef startRng As Range) As Boolean

    ToShtBmpHeader = False

    ' アドレス,サイズ,名称,説明,値0,値1,値2,値3
    Dim celldata(1 To 16, 1 To 9) As Variant
        
    Dim iRow As Long
    For iRow = 1 To 16
        celldata(iRow, 1) = m_itemInfo(iRow, 0) ' アドレス
        celldata(iRow, 2) = m_itemInfo(iRow, 1) ' サイズ
        celldata(iRow, 3) = m_itemInfo(iRow, 2) ' 名称
        celldata(iRow, 4) = m_itemInfo(iRow, 3) ' 説明
        
        If 1 = iRow Then
            celldata(iRow, 9) = "=CHAR(INDIRECT(""RC[-4]"", FALSE))&CHAR(INDIRECT(""RC[-3]"", FALSE))"
        Else
            celldata(iRow, 9) = "=INDIRECT(""RC[-4]"", FALSE) + BITLSHIFT(INDIRECT(""RC[-3]"", FALSE), 8) + BITLSHIFT(INDIRECT(""RC[-2]"", FALSE), 16) + BITLSHIFT(INDIRECT(""RC[-1]"", FALSE), 24)"
        End If
    Next iRow

    ' バッファから情報を取り込み
    Dim readIndex As Long: readIndex = 0
    Dim dataSize As Integer
    Dim dataIndex As Integer
    For iRow = 1 To 16
        
        ' データサイズ取得
        dataSize = Int(m_itemInfo(iRow, 1))
        If 4 < dataSize Then
            MsgBox ("Not support dataSize=" & dataSize)
            Exit Function
        End If
        
        ' バッファから1バイトずつ取り出し
        For dataIndex = 0 To dataSize - 1
            celldata(iRow, dataIndex + 5) = m_bufferArray(readIndex)
            readIndex = readIndex + 1
        Next dataIndex

    Next iRow
        
    ' リサイズ
    Set startRng = startRng.Resize(16, 9)
    Debug.Print startRng.Rows.Count
    Debug.Print startRng.Columns.Count
    
    ' セット
    startRng = celldata

    ToShtBmpHeader = True

End Function

' Bmp8を書き込みます。
' @param[in] 開始セル位置
' @return 正常終了したらTrue,それ以外はFalse
Public Function ToShtBmp8Data(ByRef startRng As Range) As Boolean
    
    ToShtBmp8Data = False
    
    ' 496×480固定
    Dim celldata(1 To m_imgHeight, 1 To m_imgWidth) As Variant

    ' 画像データ開始位置
    Dim readIndex As Long
    readIndex = m_bfOffBits(0) + m_bfOffBits(1) * (2 ^ 8) + m_bfOffBits(2) * (2 ^ 16) + m_bfOffBits(3) * (2 ^ 24)
    
    ' バッファから情報を取り込み
    Dim iRow As Integer
    Dim iColumn As Integer
    For iRow = m_imgHeight To 1 Step -1
        For iColumn = 1 To m_imgWidth
            ' バッファから1バイトずつ取り出し
            celldata(iRow, iColumn) = m_bufferArray(readIndex)
            readIndex = readIndex + 1
        Next iColumn
    Next iRow
        
    ' リサイズ
    Set startRng = startRng.Resize(m_imgHeight, m_imgWidth)
    
    ' セット
    startRng = celldata
    
    ToShtBmp8Data = True

End Function

' セルから取り込んだデータをファイルに書き込みます。
' @param[in] 開始セル位置
' @param[opt] ファイルパス,取り込み時と違うファイルに書き込む場合は値をセット
' @return 正常終了したらTrue,それ以外はFalse
Public Function WriteBmp8Data(ByRef startRng As Range, Optional ByVal filePath As String) As Boolean
    
    WriteBmp8Data = False

    ' 画像データ開始位置
    Dim readIndex As Long
    readIndex = m_bfOffBits(0) + m_bfOffBits(1) * (2 ^ 8) + m_bfOffBits(2) * (2 ^ 16) + m_bfOffBits(3) * (2 ^ 24)

    ' リサイズ
    Set startRng = startRng.Resize(m_imgHeight, m_imgWidth)
    Dim celldata As Variant
    celldata = startRng.Value

    ' バッファから情報を取り込み
    Dim iRow As Integer
    Dim iColumn As Integer
    For iRow = m_imgHeight To 1 Step -1
        For iColumn = 1 To m_imgWidth
            ' バッファから1バイトずつ取り出し
            m_bufferArray(readIndex) = celldata(iRow, iColumn)
            readIndex = readIndex + 1
        Next iColumn
    Next iRow

    On Error GoTo MyError
    
    '// ファイル番号取得
    Dim fn As Integer
    fn = FreeFile
    
    ' ファイル名
    If filePath = "" Then
        filePath = m_filePath
    End If
    
    '// ファイルオープン
    Open filePath For Binary Access Write As #fn
        
    '// ファイルサイズ取得
    Dim fileSize As Long
    fileSize = LOF(fn)
'    fileSize = FileLen(m_filePath)
    
    ' 書き込み
    Put #fn, , m_bufferArray
    
    Close #fn

    WriteBmp8Data = True
    Exit Function

MyError:
    Debug.Print Err.Number; ":"; Err.Description
    Debug.Print m_filePath
    MsgBox (Err.Number & ":" & Err.Description)

End Function

' バッファからメンバにインポートします。
' @param[in] none
' @return 正常終了したらTrue,それ以外はFalse
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

    '// １バイトずつ読み込んで出力
'    Dim byData As Byte
'    Dim i As Long
'    For i = 0 To fileSize - 1
'        Debug.Print "No." & CStr(i) & " - 0x" & byData & " - Char[" & Chr(byData) & "]"
'    Next
    
End Function