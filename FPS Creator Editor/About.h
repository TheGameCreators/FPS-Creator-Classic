#if !defined(AFX_CNTRITEM_H__803A36AF_D62E_4771_83D8_0C467F28B2C9__INCLUDED_)
#define AFX_CNTRITEM_H__803A36AF_D62E_4771_83D8_0C467F28B2C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CBCGURLLinkButton	m_btnURL;
	CBCGURLLinkButton	m_btnURL1;
	CEdit				m_Version;

	CEdit				m_Line1;
	CEdit				m_Line2;
	CEdit				m_Line3;
	CEdit				m_Line4;
	CEdit				m_Line5;
	CButton				m_OK;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
		afx_msg void OnDestroy				 ( );
		afx_msg int  OnCreate				 ( LPCREATESTRUCT lpCreateStruct );
		afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		HDC		m_hDCSRC;
		HANDLE	m_hBMP;
		HANDLE	m_hOld;
public:
	afx_msg void OnStnClickedAboutLine5();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__803A36AF_D62E_4771_83D8_0C467F28B2C9__INCLUDED_)