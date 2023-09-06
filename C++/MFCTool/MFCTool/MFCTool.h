
// MFCTool.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMFCToolApp:
// このクラスの実装については、MFCTool.cpp を参照してください
//

class CMFCToolApp : public CWinApp
{
public:
	CMFCToolApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
//	afx_msg void OnAfxIdPreviewClose();
};

extern CMFCToolApp theApp;
