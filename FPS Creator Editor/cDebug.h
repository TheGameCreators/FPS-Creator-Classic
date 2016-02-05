#if !defined(AFX_CDEBUG_H__5B6C5622_442D_464C_A8A5_A7F7805A2F3B__INCLUDED_)
#define AFX_CDEBUG_H__5B6C5622_442D_464C_A8A5_A7F7805A2F3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDebug.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cDebug window

class cDebug : public CWnd
{
// Construction
public:
	cDebug();

// Attributes
public:

// Operations
public:
	void Start ( char* szFile, bool bINI = true );
	void Write ( char* szMessage );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDebug)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cDebug();

	// Generated message map functions
protected:
	//{{AFX_MSG(cDebug)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	FILE*	m_pFile;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDEBUG_H__5B6C5622_442D_464C_A8A5_A7F7805A2F3B__INCLUDED_)
