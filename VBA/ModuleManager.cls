VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "ModuleManager"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

' �Q�Ɛݒ�
' Microsoft Visual Basic for Application Extensibilly 5.3
' Microsoft Scripting Runtime
' �Ƀ`�F�b�N���K�v

Private m_myName As String

' �R���X�g���N�^
Private Sub Class_Initialize()
    m_myName = "ModuleManager"
End Sub

' �f�X�g���N�^
Private Sub Class_Terminate()
End Sub

' ���O
Public Property Get Name() As String
    Name = m_myName
End Property

' �g�p���@
Public Sub Sample(Optional ByVal sampleType = 1)
    
    Dim parentDir As String
    Dim modulePathArr() As Variant

    If sampleType = 1 Then
        
        Debug.Print "== This class name =="
        Debug.Print Name
        
        ' �u�b�N�ɂ��邷�ׂẴ��W���[���A�V�[�g�A�N���X
        Debug.Print "== All modules and types =="
        With ThisWorkbook.VBProject
            Dim index As Long
            For index = 1 To .VBComponents.Count
                Debug.Print .VBComponents(index).Name; ":";
                Debug.Print .VBComponents(index).Type
            Next index
        
        End With
    
    ' ���W���[���C���|�[�g
    ElseIf sampleType = 2 Then
        
        ' ���̃u�b�N�Ɠ��K�w
        parentDir = ThisWorkbook.Path
        
        ' ���W���[������z��Őݒ�
        modulePathArr = Array( _
            parentDir & "\Hoge.bas", _
            parentDir & "\Hage.bas", _
            parentDir & "\Huge.bas" _
        )
        
        ' �C���|�[�g
        Call ImportModules(ThisWorkbook, modulePathArr)
    
    ' ���W���[���G�N�X�|�[�g
    Else
        
        ' ���̃u�b�N�Ɠ��K�w
        parentDir = ThisWorkbook.Path
        
        ' ���W���[������z��Őݒ�
        modulePathArr = Array( _
            "Hoge", _
            "Hage", _
            "Huge" _
        )
        
        ' �G�N�X�|�[�g
'        Call ExportModules(ThisWorkbook, modulePathArr, ThisWorkbook.Path)
        Call ExportModules(ThisWorkbook, modulePathArr, ThisWorkbook.Path & "\hoge\hoge\hoge")
    
    End If
    
End Sub

' �w�胏�[�N�u�b�N�Ɏw��t�H���_�z���̃��W���[�����C���|�[�g����
' @param[in] ���[�N�u�b�N
' @param[in] ���W���[���t�@�C���̃t���p�X�z��
Public Sub ImportModules(ByRef targetBook As Workbook, ByRef modulePathArr() As Variant)
    
    Dim fso As New FileSystemObject
    Dim modulePath As Variant
    Dim extStr As String
    
    '// �S���W���[�������[�v
    For Each modulePath In modulePathArr
        '// �g���q��cls�Afrm�Abas�̂����ꂩ�̏ꍇ
        extStr = LCase(fso.GetExtensionName(modulePath))
        If (extStr = "cls" Or extStr = "frm" Or extStr = "bas") Then
            '// �������W���[�����폜
            On Error Resume Next
            Call targetBook.VBProject.VBComponents.Remove(targetBook.VBProject.VBComponents(fso.GetBaseName(modulePath)))
            On Error GoTo 0
            
            On Error GoTo ImportErr
            If fso.FileExists(modulePath) Then
                '// �C���|�[�g
                Call targetBook.VBProject.VBComponents.Import(modulePath)
                Debug.Print "Import Success! "; fso.GetFileName(modulePath)
            Else
                Debug.Print "Import Failed! "; fso.GetFileName(modulePath)
            End If
        End If
    Next
    
    Exit Sub

ImportErr:
    MsgBox "�G���[�ԍ�:" & Err.Number & vbCrLf & _
           "�G���[�̎��:" & Err.Description, vbExclamation

End Sub

' �w�胏�[�N�u�b�N�Ɏw��t�H���_�z���̃��W���[�����C���|�[�g����
' @param[in] ���[�N�u�b�N
' @param[in] ���W���[�����z��
' @param[in] ���W���[���t�@�C���̃t���p�X�z��
' @param[in] �o�̓f�B���N�g��
Public Sub ExportModules(ByRef targetBook As Workbook, ByRef moduleNameArr() As Variant, Optional ByVal exportDir As String = "")
    
    ' �o�̓f�B���N�g���␳
    exportDir = IIf(exportDir <> "", exportDir, ThisWorkbook.Path)
    exportDir = IIf(Right(exportDir, 1) <> "\", exportDir, Left(exportDir, Len(exportDir) - 1))
    
    Dim fso As New FileSystemObject
    Dim moduleName As Variant
    Dim modulePath As String
    Dim extStr As String
    
    '// �S���W���[�������[�v
    For Each moduleName In moduleNameArr
        '// �g���q��cls�Afrm�Abas�̂����ꂩ�̏ꍇ
        extStr = GetModuleExt(targetBook, moduleName)
        If (extStr = "cls" Or extStr = "frm" Or extStr = "bas") Then
            '// ���W���[�����G�N�X�|�[�g
            On Error GoTo ExportErr
            If CreateDir(exportDir) Then
                modulePath = exportDir & "\" & moduleName & "." & extStr
                Call targetBook.VBProject.VBComponents(moduleName).Export(modulePath)
                Debug.Print "Export Success! "; exportDir & "\" & fso.GetFileName(modulePath)
            End If
        End If
    Next

    Exit Sub

ExportErr:
    MsgBox "�G���[�ԍ�:" & Err.Number & vbCrLf & _
           "�G���[�̎��:" & Err.Description, vbExclamation

End Sub

' �w�胏�[�N�u�b�N�ɑ��݂��郂�W���[���̃^�C�v����g���q���擾���܂��B
' @param[in] ���[�N�u�b�N
' @param[in] ���W���[����
' @param[in] ���W���[���t�@�C���̊g���q,���W���[����������Ȃ��ꍇ��""
Private Function GetModuleExt(ByRef targetBook As Workbook, ByVal moduleName As String) As String
    
    GetModuleExt = ""
    
    ' �u�b�N�ɂ��邷�ׂẴ��W���[���A�V�[�g�A�N���X
    With targetBook.VBProject
        
        Dim index As Long
        For index = 1 To .VBComponents.Count
            If moduleName = .VBComponents(index).Name Then
                Select Case .VBComponents(index).Type
                ' �W�����W���[��
                Case 1
                    GetModuleExt = "bas"
                ' �N���X���W���[��
                Case 2
                    GetModuleExt = "cls"
                ' �t�H�[��
                Case 3
                    GetModuleExt = "frm"
                ' Microsoft Excel Objects, Sheet and ThisWorkbook
                Case 100
                    GetModuleExt = "cls"
                End Select
                ' return
                Exit Function
            End If
        Next index
    
    End With

End Function

' �w�肵���f�B���N�g�����ċA�I�ɐ������܂��B
' @param[in] �f�B���N�g���p�X
' @param[none] �ݒ肵�Ȃ��ł��������B�q�f�B���N�g��
' @param[in] ���������ꍇ��True,����ȊO��False
Private Function CreateDir(ByVal targetDir As String, Optional childDir As String = "") As Boolean

    CreateDir = False
    
    ' �h���C�u���݊m�F
    Dim fso As New FileSystemObject
    If Not fso.DriveExists(fso.GetDriveName(targetDir)) Then
        Set fso = Nothing
        Exit Function
    End If
    
    ' �c��܂ōċA���ď��Ƀt�H���_���쐬
    On Error GoTo Err
    If Not fso.FolderExists(targetDir) Then
        ' �����ɐe�f�B���N�g���A�q�f�B���N�g����ݒ肵�čċA����
        If Not CreateDir(fso.GetParentFolderName(targetDir), fso.GetBaseName(targetDir)) Then
            Exit Function
        End If
    End If
    
    ' �q�f�B���N�g�����쐬
    If targetDir <> "" And childDir <> "" Then
        fso.CreateFolder (targetDir & "\" & childDir)
    End If
    
    CreateDir = True
    Exit Function

Err:
    Debug.Print Err.Number
    Debug.Print Err.Description

End Function