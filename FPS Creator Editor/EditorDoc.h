// EditorDoc.h : interface of the CEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITORDOC_H__0A28B884_5D87_456A_9D81_15433BF6E91C__INCLUDED_)
#define AFX_EDITORDOC_H__0A28B884_5D87_456A_9D81_15433BF6E91C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cNewProject.h"

class CEditorDoc : public COleDocument
{
	protected: // create from serialization only
		CEditorDoc();
		DECLARE_DYNCREATE(CEditorDoc)

	// Attributes
	public:
		bool	m_bQuit;
		void SetLevelName ( LPTSTR szText );
		BOOL SetQuitMessage ( bool bShow );

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEditorDoc)
	public:
		virtual BOOL OnNewDocument();
		virtual void Serialize(CArchive& ar);
		virtual void OnCloseDocument();
		virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
		virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
		virtual void OnEnterIdle( UINT, CWnd* );

	//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~CEditorDoc();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	protected:

	// Generated message map functions
	protected:
		cNewProject	m_NewProject;

		//{{AFX_MSG(CEditorDoc)
			// NOTE - the ClassWizard will add and remove member functions here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORDOC_H__0A28B884_5D87_456A_9D81_15433BF6E91C__INCLUDED_)
