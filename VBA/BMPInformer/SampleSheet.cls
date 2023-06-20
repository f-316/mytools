VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "Sheet1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True
Option Explicit

Private Sub CB_GetInfo_Click()
    
    Call GetBmpInfo

End Sub

Sub GetBmpInfo()
    
    ' セル設定
    Dim rFilePath As Range
    Dim rStartInfo As Range
    Set rFilePath = Me.Range("B1")
    Set rStartInfo = Me.Range("A4")
    
    ' ファイル名取得
    Dim filePath As String
    filePath = rFilePath.Value

    ' 読み込み
    Dim bmp As New BMPInformer
    If Not bmp.ReadFile(filePath) Then
        Exit Sub
    End If
        
    ' セルに書き込み
    Call bmp.ToShtBmpHeader(rStartInfo)
    
    ' 解放
    Set rFilePath = Nothing
    Set rStartInfo = Nothing
    
End Sub

Private Sub CB_Read_Click()

    ' セル設定
    Dim rFilePath As Range
    Dim rStartData As Range
    Set rFilePath = Me.Range("B1")
    Set rStartData = Me.Range("J21")

    ' ファイル名取得
    Dim filePath As String
    filePath = rFilePath.Value

    ' 読み込み
    Dim bmp As New BMPInformer
    If Not bmp.ReadFile(filePath) Then
        Exit Sub
    End If

    ' セルに書き込み
    Call bmp.ToShtBmp8Data(rStartData)

    ' 描画状態を反映
    If MsgBox("描画状態を反映しますか？時間がかかります。", vbYesNo) = vbYes Then
        Call ColorFromVal(rStartData)
    End If
    
    ' 解放
    Set rFilePath = Nothing
    Set rStartData = Nothing
    
End Sub

Private Sub CB_Write_Click()

    ' セル設定
    Dim rSrcFilePath As Range
    Dim rDstFilePath As Range
    Dim rStartData As Range
    Set rSrcFilePath = Me.Range("B1")
    Set rDstFilePath = Me.Range("B2")
    Set rStartData = Me.Range("J21")

    ' ファイル名取得
    Dim srcFilePath As String
    srcFilePath = rSrcFilePath.Value

    Dim dstFilePath As String
    dstFilePath = rDstFilePath.Value

    ' 読み込み
    Dim bmp As New BMPInformer
    If Not bmp.ReadFile(srcFilePath) Then
        Exit Sub
    End If

    ' セルに書き込み
    Call bmp.WriteBmp8Data(rStartData, dstFilePath)

    ' 解放
    Set rSrcFilePath = Nothing
    Set rDstFilePath = Nothing
    Set rStartData = Nothing

End Sub

' エクセルの描画処理を止めます。
' 処理実行後は必ずResumeしてください。
Private Sub StopRendering()
    Application.ScreenUpdating = False '画面描画を停止
    Application.Cursor = xlWait 'ウエイトカーソル
    Application.EnableEvents = False 'イベントを抑止
    Application.DisplayAlerts = False '確認メッセージを抑止
    Application.Calculation = xlCalculationManual '計算を手動に
End Sub

' エクセルの描画処理を元に戻します。
Private Sub ResumeRendering()
    Application.StatusBar = False 'ステータスバーを消す
    Application.Calculation = xlCalculationAutomatic '計算を自動に
    Application.DisplayAlerts = True '確認メッセージを開始
    Application.EnableEvents = True 'イベントを開始
    Application.Cursor = xlDefault '標準カーソル
    Application.ScreenUpdating = True '画面描画を開始
End Sub

Private Sub ColorFromValInSel()

    Call ColorFromVal(Selection)

End Sub

' 選択中のセル内の値でグレースケールにする
Private Sub ColorFromVal(ByRef tarRng As Range)

    Call StopRendering
    
    ' セルの値でセルの色をグレースケール色に着色
    Dim tarCell As Range
    For Each tarCell In tarRng
        tarCell.Interior.Color = RGB(tarCell.Value, tarCell.Value, tarCell.Value)
    Next tarCell
    
    Call ResumeRendering
    
    Set tarRng = Nothing

End Sub
