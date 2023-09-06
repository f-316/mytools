
// MFCToolDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MFCTool.h"
#include "MFCToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCToolDlg ダイアログ
CMFCToolDlg::CMFCToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCTOOL_DIALOG, pParent)
	, m_csMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_csMessage);
	DDV_MaxChars(pDX, m_csMessage, 6);
}

BEGIN_MESSAGE_MAP(CMFCToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCToolDlg::OnClickedShow)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMFCToolDlg メッセージ ハンドラー

BOOL CMFCToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	CSampleClass sampleClass;
	sampleClass.SampleMethod(1, 2);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CMFCToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMFCToolDlg::OnPaint()
{
	// 最小化になっているとき
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
		// CWndオブジェクトへのポインタを渡す。
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		// ペン作成
		CPen pen;
		pen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));

		// ペンやブラシに持ち替える
		// 戻り値には持ち替える前の描画オブジェクトのポインタが返る
		CPen* pOldPen = dc.SelectObject(&pen);
		dc.MoveTo(10, 10);
		dc.LineTo(100, 300);

		// 前のペンに持ち替える。
		dc.SelectObject(pOldPen);
		dc.LineTo(300, 300);
		POINT p[2];
		p[0].x = 100;
		p[0].y = 100;
		p[1].x = 200;
		p[1].y = 200;
		dc.Polyline(p, 2);
		RECT rect;
		rect.left = p[0].x;
		rect.top = p[0].y;
		rect.right = p[1].x;
		rect.bottom = p[1].y;
		dc.Arc(&rect, p[0], p[1]);

		pOldPen = dc.SelectObject(&pen);
		dc.MoveTo(500, 5000);
		dc.AngleArc(500, 5000, 4900, 75, 30);
		dc.LineTo(500, 5000);

		CImage img;
		RECT imgRect;
		imgRect.left = 100;
		imgRect.top = 100;
		imgRect.right = 1000;
		imgRect.bottom = 1000;
		CWnd::GetClientRect(&imgRect);
		CString imgFile(_T("C:\\Users\\f_uen\\Desktop\\neko.jpg"));
		//CFileDialog fileDlg(TRUE, 0, imgFile.GetString(), 6UL,_T("じぇーぺぐ|*.jpg|びっとま|*.bmp||"));
		//int ret = fileDlg.DoModal();
		//if (ret == IDOK)
		//{
		//	imgFile = fileDlg.GetPathName();
		//}
		HRESULT hImg = img.Load(imgFile.GetString());
		//HRESULT hImg = img.Load(_T("red.bmp"));
		//HRESULT hImg = img.Load(L"red.bmp");
		if (hImg != E_FAIL)
		{
			img.StretchBlt(dc, imgRect);
			img.Draw(dc, 100, 100);
			TRACE("\n%d,%d\n", img.GetHeight(), img.GetWidth());
			//dc.StretchBlt(img, imgRect);
		}
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMFCToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCToolDlg::OnClickedShow()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (CWnd::UpdateData(TRUE))
	{
		AfxMessageBox(m_csMessage);
	}

	m_csMessage += _T("a");
	if (CWnd::UpdateData(FALSE))
	{
		AfxMessageBox(m_csMessage);
	}
}


void CMFCToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CWnd::Invalidate();
	CWnd::UpdateWindow();
}
