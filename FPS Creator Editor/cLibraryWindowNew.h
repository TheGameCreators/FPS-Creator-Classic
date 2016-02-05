#if !defined(AFX_CLIBRARYWINDOWNEW_H__F8256B06_459B_4093_9AB1_F295F9B1BC6E__INCLUDED_)
#define AFX_CLIBRARYWINDOWNEW_H__F8256B06_459B_4093_9AB1_F295F9B1BC6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cLibraryWindowNew.h : header file
//

#include "ZSplitterDlg.h"

/////////////////////////////////////////////////////////////////////////////
// cLibraryWindowNew dialog


class cLibraryWindowNew : public CPropertyPage
{
// Construction
public:
	cLibraryWindowNew();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg3)
	enum { IDD = IDD_NEW_LIBRARY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cLibraryWindowNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cLibraryWindowNew)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  ZSplitter m_spliter;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIBRARYWINDOWNEW_H__F8256B06_459B_4093_9AB1_F295F9B1BC6E__INCLUDED_)
