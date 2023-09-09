
// MinDDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MinD.h"
#include "MinDDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMinDDlg ダイアログ



CMinDDlg::CMinDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MIND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMinDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// コントロールと紐づけ
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
}

BEGIN_MESSAGE_MAP(CMinDDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMinDDlg::OnBnClickedMin)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CMinDDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMinDDlg::OnBnClickedCancel)
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CMinDDlg メッセージ ハンドラー

BOOL CMinDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	m_textColor = RGB(0, 0, 255);
	m_bgColor = RGB(0, 0, 0);
	m_Button1.SetWindowTextW(L"hoge");

	m_brush.CreateSolidBrush(RGB(255, 0, 0));

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CMinDDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CWnd::PostNcDestroy();
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMinDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMinDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMinDDlg::OnBnClickedMin()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}

HBRUSH CMinDDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH newBrushHdl = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。

	// スタティックコントロールの場合
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		newBrushHdl = m_brush;
		break;

	case CTLCOLOR_STATIC:
		if (pWnd->GetDlgCtrlID() == IDC_STATIC)
		{
			// 背景色を黒に設定します。
			pDC->SetTextColor(m_textColor);
			//pDC->SetBkColor(m_bgColor);
			pDC->SetBkMode(TRANSPARENT);

			// 黒のブラシを返却します。
			newBrushHdl = m_brush;
		}
		break;

	default:
		break;
	}

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return newBrushHdl;
}


void CMinDDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if (nType == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
		//ShowWindow(SW_MINIMIZE);
	}
}


void CMinDDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}


void CMinDDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


void CMinDDlg::OnNcPaint()
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CDialogEx::OnNcPaint() を呼び出さないでください。

	//CWnd* pWnd = GetDlgItem(IDC_BUTTON2); // IDC_CUSTOM_BUTTON はボタンのID

	//if (pWnd)
	//{
	//	CRect rect;
	//	pWnd->GetWindowRect(&rect);
	//	ScreenToClient(&rect);

	//	CDC* pDC = GetWindowDC();
	//	pDC->FillSolidRect(rect, RGB(0, 0, 255)); // 青い四角形を描画（ボタンの代わり）
	//	ReleaseDC(pDC);
	//}

	// 親クラスの NcPaint メッセージ ハンドラを呼び出します
	CDialogEx::OnNcPaint();
}


int CMinDDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。
	// タイトルバーにボタンを描画
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_SYSMENU;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);

	// タイトルバーにボタンを描画
	//ModifyStyleEx(0, WS_EX_TOOLWINDOW); // タイトルバーの最小化ボタンを非表示にする
	//ModifyStyle(0, WS_SYSMENU); // システムメニューボタンを表示

	return 0;
}


void CMinDDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CDialogEx::OnNcCalcSize(bCalcValidRects, lpncsp);
}


LRESULT CMinDDlg::OnNcHitTest(CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	LRESULT hitTest = CDialogEx::OnNcHitTest(point);

	if (hitTest == HTSYSMENU)
	{
		// カスタムボタンがクリックされた場合の処理を追加
		CRect customButtonRect;
		GetWindowRect(&customButtonRect);
		customButtonRect.right = customButtonRect.left + GetSystemMetrics(SM_CXSIZE); // カスタムボタンの幅

		if (customButtonRect.PtInRect(point))
		{
			// カスタムボタンがクリックされたことを示すメッセージを表示
			AfxMessageBox(_T("カスタムボタンがクリックされました。"));
		}
	}

	return hitTest;
}
