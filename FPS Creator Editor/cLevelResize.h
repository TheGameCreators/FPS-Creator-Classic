#if !defined(AFX_CLEVELRESIZE_H__14491ABF_5045_414E_8D79_2D36AA7B1CCD__INCLUDED_)
#define AFX_CLEVELRESIZE_H__14491ABF_5045_414E_8D79_2D36AA7B1CCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cLevelResize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cLevelResize dialog

class cLevelResize : public CDialog
{
// Construction
public:
	cLevelResize(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cLevelResize)
	enum { IDD = IDD_LEVELRESIZE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cLevelResize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cLevelResize)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLEVELRESIZE_H__14491ABF_5045_414E_8D79_2D36AA7B1CCD__INCLUDED_)
