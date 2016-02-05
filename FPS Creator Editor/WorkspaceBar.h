// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__6189B231_C6E6_4CC6_9FF6_D104AC5F67B4__INCLUDED_)
#define AFX_WORKSPACEBAR_H__6189B231_C6E6_4CC6_9FF6_D104AC5F67B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjectProperty.h"
#include "cMessages.h"

#include "cProperty.h"

#include "cDialog1.h"

class CWorkspaceBar : public CBCGSizingControlBar, public cBaseMessage
{
public:
	
	CWorkspaceBar();

	//BYTE*	m_pMessageData;

	cIPC*	m_pIPC;

	cMessages			m_Message;
	void	HandleMessages ( void );

	

	void SetVisible ( BOOL bState );

	CObjectProperty m_Property;
	cProperty*		m_pProperty;

// Attributes
protected:

	bool			m_bClose;

	bool			m_bValid;


	void UpdateWindow    ( void );

	CButton				m_ApplyButton;
	bool				m_bApplyButton;
	CButton				m_CancelButton;
	bool				m_bCancelButton;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	afx_msg void OnButtonApply    ( );
		afx_msg void OnButtonCancel    ( );
	//}}AFX_VIRTUAL

	

// Implementation
public:
	virtual ~CWorkspaceBar();

// Generated message map functions
protected:

	virtual BOOL QueryClose (void);

	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__6189B231_C6E6_4CC6_9FF6_D104AC5F67B4__INCLUDED_)
