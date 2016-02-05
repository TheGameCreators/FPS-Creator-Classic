#if !defined(AFX_CDIALOGEND_H__BA2A7E92_A642_4C33_9032_3E7145D2255D__INCLUDED_)
#define AFX_CDIALOGEND_H__BA2A7E92_A642_4C33_9032_3E7145D2255D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDialogEnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cDialogEnd dialog

class cDialogEnd : public CDialog
{
// Construction
public:
	cDialogEnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cDialogEnd)
	enum { IDD = IDD_DIALOG_END };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDialogEnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
		int m_iQuit;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cDialogEnd)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBitmap 	m_Bitmap;

	HDC		m_hDCSRC;
	HANDLE	m_hBMP;
	HANDLE	m_hOld;

	CButton	m_OK;
	CButton	m_Cancel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDIALOGEND_H__BA2A7E92_A642_4C33_9032_3E7145D2255D__INCLUDED_)
