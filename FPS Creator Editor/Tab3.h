#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "SampleList.h"
#include "afxwin.h"

// CTab3 dialog

class CTab3 : public CDialog
{
	DECLARE_DYNAMIC(CTab3)

public:
	CTab3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab3();

// Dialog Data
	enum { IDD = IDD_TAB3 };

	BOOL OnInitDialog( );

protected:

	bool m_bInit;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnClickList( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnDblClickList( NMHDR* pNMHDR, LRESULT* pResult );

public:
	CSampleList m_SamplesList;
	CButton m_btnLoad;
public:
	afx_msg void OnLoadClicked();
};
