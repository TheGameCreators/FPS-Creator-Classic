#if !defined(AFX_CCREDITS_H__00832B33_E76E_4F49_B8E1_BFD3ED4B2B8E__INCLUDED_)
#define AFX_CCREDITS_H__00832B33_E76E_4F49_B8E1_BFD3ED4B2B8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cCredits.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cCredits dialog

class cCredits : public CDialog
{
// Construction
public:
	cCredits(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cCredits)
	enum { IDD = IDD_CREDITS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cCredits)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cCredits)
		// NOTE: the ClassWizard will add member functions here
		afx_msg int  OnCreate				 ( LPCREATESTRUCT lpCreateStruct );
		afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		HDC		m_hDCSRC;
	HANDLE	m_hBMP;
	HANDLE	m_hOld;

	CEdit				m_Lines [ 12 ];
	CButton				m_OK;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCREDITS_H__00832B33_E76E_4F49_B8E1_BFD3ED4B2B8E__INCLUDED_)
