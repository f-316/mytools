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
    
    ' �Z���ݒ�
    Dim rFilePath As Range
    Dim rStartInfo As Range
    Set rFilePath = Me.Range("B1")
    Set rStartInfo = Me.Range("A4")
    
    ' �t�@�C�����擾
    Dim filePath As String
    filePath = rFilePath.Value

    ' �ǂݍ���
    Dim bmp As New BMPInformer
    If Not bmp.ReadFile(filePath) Then
        Exit Sub
    End If
        
    ' �Z���ɏ�������
    Call bmp.ToShtBmpHeader(rStartInfo)
    
    ' ���
    Set rFilePath = Nothing
    Set rStartInfo = Nothing
    
End Sub

Private Sub CB_Read_Click()

    ' �Z���ݒ�
    Dim rFilePath As Range
    Dim rStartData As Range
    Set rFilePath = Me.Range("B1")
    Set rStartData = Me.Range("J21")

    ' �t�@�C�����擾
    Dim filePath As String
    filePath = rFilePath.Value

    ' �ǂݍ���
    Dim bmp As New BMPInformer
    If Not bmp.ReadFile(filePath) Then
        Exit Sub
    End If

    ' �Z���ɏ�������
    Call bmp.ToShtBmp8Data(rStartData)

    ' �`���Ԃ𔽉f
    If MsgBox("�`���Ԃ𔽉f���܂����H���Ԃ�������܂��B", vbYesNo) = vbYes Then
        Call ColorFromVal(rStartData)
    End If
    
    ' ���
    Set rFilePath = Nothing
    Set rStartData = Nothing
    
End Sub

Private Sub CB_Write_Click()

    ' �Z���ݒ�
    Dim rSrcFilePath As Range
    Dim rDstFilePath As Range
    Dim rStartData As Range
    Set rSrcFilePath = Me.Range("B1")
    Set rDstFilePath = Me.Range("B2")
    Set rStartData = Me.Range("J21")

    ' �t�@�C�����擾
    Dim srcFilePath As String
    srcFilePath = rSrcFilePath.Value

    Dim dstFilePath As String
    dstFilePath = rDstFilePath.Value

    ' �ǂݍ���
    Dim bmp As New BMPInformer
    If Not bmp.ReadFile(srcFilePath) Then
        Exit Sub
    End If

    ' �Z���ɏ�������
    Call bmp.WriteBmp8Data(rStartData, dstFilePath)

    ' ���
    Set rSrcFilePath = Nothing
    Set rDstFilePath = Nothing
    Set rStartData = Nothing

End Sub

' �G�N�Z���̕`�揈�����~�߂܂��B
' �������s��͕K��Resume���Ă��������B
Private Sub StopRendering()
    Application.ScreenUpdating = False '��ʕ`����~
    Application.Cursor = xlWait '�E�G�C�g�J�[�\��
    Application.EnableEvents = False '�C�x���g��}�~
    Application.DisplayAlerts = False '�m�F���b�Z�[�W��}�~
    Application.Calculation = xlCalculationManual '�v�Z���蓮��
End Sub

' �G�N�Z���̕`�揈�������ɖ߂��܂��B
Private Sub ResumeRendering()
    Application.StatusBar = False '�X�e�[�^�X�o�[������
    Application.Calculation = xlCalculationAutomatic '�v�Z��������
    Application.DisplayAlerts = True '�m�F���b�Z�[�W���J�n
    Application.EnableEvents = True '�C�x���g���J�n
    Application.Cursor = xlDefault '�W���J�[�\��
    Application.ScreenUpdating = True '��ʕ`����J�n
End Sub

Private Sub ColorFromValInSel()

    Call ColorFromVal(Selection)

End Sub

' �I�𒆂̃Z�����̒l�ŃO���[�X�P�[���ɂ���
Private Sub ColorFromVal(ByRef tarRng As Range)

    Call StopRendering
    
    ' �Z���̒l�ŃZ���̐F���O���[�X�P�[���F�ɒ��F
    Dim tarCell As Range
    For Each tarCell In tarRng
        tarCell.Interior.Color = RGB(tarCell.Value, tarCell.Value, tarCell.Value)
    Next tarCell
    
    Call ResumeRendering
    
    Set tarRng = Nothing

End Sub