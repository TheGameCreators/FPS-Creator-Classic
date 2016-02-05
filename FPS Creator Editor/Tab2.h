#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "VideoList.h"
#include "afxwin.h"
#include "cVideo.h"
#include "BlackBox.h"

// CTab2 dialog

class CTab2 : public CDialog
{
	DECLARE_DYNAMIC(CTab2)

public:
	CTab2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab2();

// Dialog Data
	enum { IDD = IDD_TAB2 };

	BOOL OnInitDialog( );
	void OnClosing( );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnClickList( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnDblClickList( NMHDR* pNMHDR, LRESULT* pResult );

public:
	CVideoList m_VideosList;
	CButton m_btnPlay;

protected:

	cVideo *m_pCurrVideo;
	bool m_bPaused;
	bool m_bResizing;
	__int64 pos;
	bool m_bInit;

public:
	afx_msg void OnPlayClicked();
	afx_msg void OnCloseVideoClicked();
	afx_msg void OnPauseResumeClicked();
	afx_msg void OnTimer( UINT_PTR nIDEvent );

	void Pause();

	CButton m_btnCloseVideo;
	CBlackBox m_BlackBox;
	CButton m_btnPauseResume;
};
