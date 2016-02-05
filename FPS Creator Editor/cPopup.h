#if !defined(AFX_CPOPUP_H__FA3349AD_661C_483F_8821_88C8D5DF8B71__INCLUDED_)
#define AFX_CPOPUP_H__FA3349AD_661C_483F_8821_88C8D5DF8B71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cPopup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cPopup dialog

#include "cMessages.h"

class cPopup : public CDialog, public cBaseMessage
{
// Construction
public:
	cPopup(CWnd* pParent = NULL );   // standard constructor

	CEdit m_Text;
	CEdit m_Text1;
	CString m_String;

	BYTE*		m_pMessageData;
	cMessages	m_Message;
	void		HandleMessages ( void );

// Dialog Data
	//{{AFX_DATA(cPopup)
	enum { IDD = IDD_POPUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cPopup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cPopup)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPOPUP_H__FA3349AD_661C_483F_8821_88C8D5DF8B71__INCLUDED_)
