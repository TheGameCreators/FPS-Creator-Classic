#if !defined(AFX_CDIALOG1_H__BD2D578F_430D_49D4_99DB_E9DDA8B4D348__INCLUDED_)
#define AFX_CDIALOG1_H__BD2D578F_430D_49D4_99DB_E9DDA8B4D348__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDialog1.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// cDialog1 dialog

class cDialog1 : public CDialog
{
// Construction
public:
	cDialog1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cDialog1)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDialog1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cDialog1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDIALOG1_H__BD2D578F_430D_49D4_99DB_E9DDA8B4D348__INCLUDED_)
