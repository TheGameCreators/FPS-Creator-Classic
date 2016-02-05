#if !defined(AFX_CFRONT_H__DE690BF7_7C4A_4496_A537_8FABCDAAD755__INCLUDED_)
#define AFX_CFRONT_H__DE690BF7_7C4A_4496_A537_8FABCDAAD755__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cFront.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cFront dialog

#include "cMessages.h"

class cFront : public CDialog, public cBaseMessage
{
// Construction
public:
	cFront(CWnd* pParent = NULL);   // standard constructor

	void	HandleMessages ( void );
	bool	SerialCodeCheck ( LPSTR pSerialCodeStringToCheck );
	bool	IsSerialCodeValid ( LPSTR pTryCodeOrNULL );
	LPSTR	CheckOSVersion ( void );
	DWORD	GetTodaysDays ( void );

	cMessages	m_Message;
	BYTE*		m_pMessageData;

	CEdit		m_Edit;

	DWORD		m_dwTime;

// Dialog Data
	//{{AFX_DATA(cFront)
	enum { IDD = IDD_FRONT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cFront)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cFront)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HDC		m_hDCSRC;
	HANDLE	m_hBMP;
	HANDLE	m_hOld;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFRONT_H__DE690BF7_7C4A_4496_A537_8FABCDAAD755__INCLUDED_)
