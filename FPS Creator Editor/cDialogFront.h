#if !defined(AFX_CDIALOGFRONT_H__F590ADFE_D7FB_44C5_A720_FC2526AAC2A8__INCLUDED_)
#define AFX_CDIALOGFRONT_H__F590ADFE_D7FB_44C5_A720_FC2526AAC2A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDialogFront.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cDialogFront dialog

class cDialogFront : public CDialog
{
// Construction
public:
	cDialogFront(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cDialogFront)
	enum { IDD = IDD_DIALOG_FRONT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDialogFront)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CProgressCtrl m_Progress;
	CEdit			m_Edit;
	CEdit			m_Edit1;

	CButton			m_Okay;
	CButton			m_Cancel;
	bool			m_bValid;

	// Generated message map functions
	//{{AFX_MSG(cDialogFront)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDIALOGFRONT_H__F590ADFE_D7FB_44C5_A720_FC2526AAC2A8__INCLUDED_)
