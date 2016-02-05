#if !defined(AFX_CPROPERTY_H__9DB38FDA_198B_4736_8F2C_8C1DAE6A970D__INCLUDED_)
#define AFX_CPROPERTY_H__9DB38FDA_198B_4736_8F2C_8C1DAE6A970D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cProperty window

#include "ObjectProperty.h"

#include "cMessages.h"

class cProperty : public CWnd
{
// Construction
public:
	cProperty(CObjectProperty* pProperty);

// Attributes
public:
	CObjectProperty*	m_pPropertyWindow;

	

	//BYTE*				m_pMessageData;
	cIPC*				m_pIPC;
	
	DWORD				m_dwGroupIndex;
	DWORD				m_dwControlIndex;

	cMessages*			m_pMessages;

	void				ResetAll ( void );
	void				HandleMessages ( cIPC* pIPC, cMessages* pMessage );

	void UpdateSelection		( void );
	void UpdateControls			( void );
	void UpdateGroups			( void );
	void UpdateWindow			( void );

	void CheckForNewControls	( void );

	void UpdateEditBoxes		( void );
	void UpdateListBoxes		( void );
	void UpdateColorPickers		( void );
	void UpdateFilePickers		( void );
	void UpdateFontPickers		( void );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cProperty)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cProperty();

	// Generated message map functions
protected:
	//{{AFX_MSG(cProperty)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPROPERTY_H__9DB38FDA_198B_4736_8F2C_8C1DAE6A970D__INCLUDED_)
