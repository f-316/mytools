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

' 参照設定
' Microsoft Visual Basic for Application Extensibilly 5.3
' Microsoft Scripting Runtime
' にチェックが必要

Private m_myName As String

' コンストラクタ
Private Sub Class_Initialize()
    m_myName = "ModuleManager"
End Sub

' デストラクタ
Private Sub Class_Terminate()
End Sub

' 名前
Public Property Get Name() As String
    Name = m_myName
End Property

' 使用方法
Public Sub Sample(Optional ByVal sampleType = 1)
    
    Dim parentDir As String
    Dim modulePathArr() As Variant

    If sampleType = 1 Then
        
        Debug.Print "== This class name =="
        Debug.Print Name
        
        ' ブックにあるすべてのモジュール、シート、クラス
        Debug.Print "== All modules and types =="
        With ThisWorkbook.VBProject
            Dim index As Long
            For index = 1 To .VBComponents.Count
                Debug.Print .VBComponents(index).Name; ":";
                Debug.Print .VBComponents(index).Type
            Next index
        
        End With
    
    ' モジュールインポート
    ElseIf sampleType = 2 Then
        
        ' このブックと同階層
        parentDir = ThisWorkbook.Path
        
        ' モジュール名を配列で設定
        modulePathArr = Array( _
            parentDir & "\Hoge.bas", _
            parentDir & "\Hage.bas", _
            parentDir & "\Huge.bas" _
        )
        
        ' インポート
        Call ImportModules(ThisWorkbook, modulePathArr)
    
    ' モジュールエクスポート
    Else
        
        ' このブックと同階層
        parentDir = ThisWorkbook.Path
        
        ' モジュール名を配列で設定
        modulePathArr = Array( _
            "Hoge", _
            "Hage", _
            "Huge" _
        )
        
        ' エクスポート
'        Call ExportModules(ThisWorkbook, modulePathArr, ThisWorkbook.Path)
        Call ExportModules(ThisWorkbook, modulePathArr, ThisWorkbook.Path & "\hoge\hoge\hoge")
    
    End If
    
End Sub

' 指定ワークブックに指定フォルダ配下のモジュールをインポートする
' @param[in] ワークブック
' @param[in] モジュールファイルのフルパス配列
Public Sub ImportModules(ByRef targetBook As Workbook, ByRef modulePathArr() As Variant)
    
    Dim fso As New FileSystemObject
    Dim modulePath As Variant
    Dim extStr As String
    
    '// 全モジュールをループ
    For Each modulePath In modulePathArr
        '// 拡張子がcls、frm、basのいずれかの場合
        extStr = LCase(fso.GetExtensionName(modulePath))
        If (extStr = "cls" Or extStr = "frm" Or extStr = "bas") Then
            '// 同名モジュールを削除
            On Error Resume Next
            Call targetBook.VBProject.VBComponents.Remove(targetBook.VBProject.VBComponents(fso.GetBaseName(modulePath)))
            On Error GoTo 0
            
            On Error GoTo ImportErr
            If fso.FileExists(modulePath) Then
                '// インポート
                Call targetBook.VBProject.VBComponents.Import(modulePath)
                Debug.Print "Import Success! "; fso.GetFileName(modulePath)
            Else
                Debug.Print "Import Failed! "; fso.GetFileName(modulePath)
            End If
        End If
    Next
    
    Exit Sub

ImportErr:
    MsgBox "エラー番号:" & Err.Number & vbCrLf & _
           "エラーの種類:" & Err.Description, vbExclamation

End Sub

' 指定ワークブックに指定フォルダ配下のモジュールをインポートする
' @param[in] ワークブック
' @param[in] モジュール名配列
' @param[in] モジュールファイルのフルパス配列
' @param[in] 出力ディレクトリ
Public Sub ExportModules(ByRef targetBook As Workbook, ByRef moduleNameArr() As Variant, Optional ByVal exportDir As String = "")
    
    ' 出力ディレクトリ補正
    exportDir = IIf(exportDir <> "", exportDir, ThisWorkbook.Path)
    exportDir = IIf(Right(exportDir, 1) <> "\", exportDir, Left(exportDir, Len(exportDir) - 1))
    
    Dim fso As New FileSystemObject
    Dim moduleName As Variant
    Dim modulePath As String
    Dim extStr As String
    
    '// 全モジュールをループ
    For Each moduleName In moduleNameArr
        '// 拡張子がcls、frm、basのいずれかの場合
        extStr = GetModuleExt(targetBook, moduleName)
        If (extStr = "cls" Or extStr = "frm" Or extStr = "bas") Then
            '// モジュールをエクスポート
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
    MsgBox "エラー番号:" & Err.Number & vbCrLf & _
           "エラーの種類:" & Err.Description, vbExclamation

End Sub

' 指定ワークブックに存在するモジュールのタイプから拡張子を取得します。
' @param[in] ワークブック
' @param[in] モジュール名
' @param[in] モジュールファイルの拡張子,モジュールが見つからない場合は""
Private Function GetModuleExt(ByRef targetBook As Workbook, ByVal moduleName As String) As String
    
    GetModuleExt = ""
    
    ' ブックにあるすべてのモジュール、シート、クラス
    With targetBook.VBProject
        
        Dim index As Long
        For index = 1 To .VBComponents.Count
            If moduleName = .VBComponents(index).Name Then
                Select Case .VBComponents(index).Type
                ' 標準モジュール
                Case 1
                    GetModuleExt = "bas"
                ' クラスモジュール
                Case 2
                    GetModuleExt = "cls"
                ' フォーム
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

' 指定したディレクトリを再帰的に生成します。
' @param[in] ディレクトリパス
' @param[none] 設定しないでください。子ディレクトリ
' @param[in] 成功した場合はTrue,それ以外はFalse
Private Function CreateDir(ByVal targetDir As String, Optional childDir As String = "") As Boolean

    CreateDir = False
    
    ' ドライブ存在確認
    Dim fso As New FileSystemObject
    If Not fso.DriveExists(fso.GetDriveName(targetDir)) Then
        Set fso = Nothing
        Exit Function
    End If
    
    ' 祖先まで再帰して順にフォルダを作成
    On Error GoTo Err
    If Not fso.FolderExists(targetDir) Then
        ' 引数に親ディレクトリ、子ディレクトリを設定して再帰する
        If Not CreateDir(fso.GetParentFolderName(targetDir), fso.GetBaseName(targetDir)) Then
            Exit Function
        End If
    End If
    
    ' 子ディレクトリを作成
    If targetDir <> "" And childDir <> "" Then
        fso.CreateFolder (targetDir & "\" & childDir)
    End If
    
    CreateDir = True
    Exit Function

Err:
    Debug.Print Err.Number
    Debug.Print Err.Description

End Function
