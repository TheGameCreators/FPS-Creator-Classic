#if !defined(AFX_CFPIWIZARD_H__DD16E744_CF25_4C5E_9043_9FFD6E69E2BA__INCLUDED_)
#define AFX_CFPIWIZARD_H__DD16E744_CF25_4C5E_9043_9FFD6E69E2BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cFPIWizard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cFPIWizard dialog

#include "ObjectProperty.h"
#include "cMessages.h"
#include "cProperty.h"

class cFPIWizard : public CDialog, public cBaseMessage
{
// Construction
public:
	cFPIWizard(CWnd* pParent = NULL);   // standard constructor

	//BYTE*		m_pMessageData;

	cIPC* m_pIPC;

	cMessages	m_Message;



	void		HandleMessages ( void );

	cProperty*		m_pProperty;

	bool m_bOK;
	bool m_bCancel;
	bool m_bPreview;

// Dialog Data
	//{{AFX_DATA(cFPIWizard)
	enum { IDD = IDD_DIALOG_FPI };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cFPIWizard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CObjectProperty m_PropertyWindow;

	// Generated message map functions
	//{{AFX_MSG(cFPIWizard)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnPreview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		CButton m_Buttons [ 3 ];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFPIWIZARD_H__DD16E744_CF25_4C5E_9043_9FFD6E69E2BA__INCLUDED_)
