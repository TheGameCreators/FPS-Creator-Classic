
#if !defined(AFX_CHILDFRM_H__928D5FA4_72DF_4EC4_A0EC_76DEFD0E3B7A__INCLUDED_)
#define AFX_CHILDFRM_H__928D5FA4_72DF_4EC4_A0EC_76DEFD0E3B7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#define CMDIChildWnd CBCGMDIChildWnd

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE ( CChildFrame )

	public:
		CChildFrame ( );

	public:

	public:
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CChildFrame)
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		virtual void ActivateFrame(int nCmdShow);
		//}}AFX_VIRTUAL

	public:
		virtual ~CChildFrame ( );
	
		#ifdef _DEBUG
			virtual void AssertValid ( ) const;
			virtual void Dump        ( CDumpContext& dc ) const;
		#endif

	protected:
		//{{AFX_MSG(CChildFrame)
		//}}AFX_MSG
		
		DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__928D5FA4_72DF_4EC4_A0EC_76DEFD0E3B7A__INCLUDED_)
