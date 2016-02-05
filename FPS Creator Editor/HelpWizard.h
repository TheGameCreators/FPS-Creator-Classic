#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "TGC Store\Thread.h"
#include "Tab1.h"
#include "afxwin.h"


// CHelpWizard dialog

class CHelpWizard : public CDialog, public Thread
{
	DECLARE_DYNAMIC(CHelpWizard)

public:
	CHelpWizard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHelpWizard();

// Dialog Data
	enum { IDD = IDD_HELPWIZARD };

	virtual BOOL OnInitDialog();
	void OnOK( );
	void OnCancel( );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual unsigned Run( );

	DECLARE_MESSAGE_MAP()

	bool m_bInit;

public:
	CTab1 m_Video;
	//CButton m_btnShowOnStartUp;
	CButton m_btnClose;

	afx_msg void OnSize( UINT nType, int cx, int cy );
};
