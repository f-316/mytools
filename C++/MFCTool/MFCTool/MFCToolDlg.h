
// MFCToolDlg.h : ヘッダー ファイル
//

#pragma once
#include "SampleClass.h"

// CMFCToolDlg ダイアログ
class CMFCToolDlg : public CDialogEx
{
// コンストラクション
public:
	CMFCToolDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedShow();
	CString m_csMessage;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
