#pragma once

#include "resource.h"
#include "afxwin.h"
#include "TGC Store\PreviewRect.h"
#include "cVideo.h"

class CMyTabCtrl;

// CTab1 dialog

class CTab1 : public CDialog
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab1();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

	BOOL OnInitDialog( );
	void OnClosing( );
	void OnRestart( );
	void SetTabOwner( CMyTabCtrl *pOwner );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	__int64 pos;
	bool m_bResizing;
	CMyTabCtrl *m_pOwnerTabCtrl;
	bool m_bInit;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnTimer( UINT_PTR nIDEvent );

public:

	CPreviewRect m_IntroImg;
	cVideo *m_pIntroVideo;
};
