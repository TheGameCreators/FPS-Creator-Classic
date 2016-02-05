#if !defined(AFX_CNEWPROJECT_H__F5571F7D_E0DC_4DD5_9349_ADBEE9F7F68D__INCLUDED_)
#define AFX_CNEWPROJECT_H__F5571F7D_E0DC_4DD5_9349_ADBEE9F7F68D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cNewProject.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cNewProject dialog

class cNewProject : public CDialog
{
// Construction
public:
	cNewProject(CWnd* pParent = NULL);   // standard constructor

	CEdit	m_Edit;
	CString m_UNI;

// Dialog Data
	//{{AFX_DATA(cNewProject)
	enum { IDD = IDD_NEWPROJECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cNewProject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(cNewProject)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CEdit	m_Line1;
	CButton	m_OK;
	CButton	m_Cancel;
		
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNEWPROJECT_H__F5571F7D_E0DC_4DD5_9349_ADBEE9F7F68D__INCLUDED_)
