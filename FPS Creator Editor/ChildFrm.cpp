
#include "stdafx.h"
#include "Editor.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE ( CChildFrame, CMDIChildWnd )

BEGIN_MESSAGE_MAP ( CChildFrame, CMDIChildWnd )
	//{{AFX_MSG_MAP(CChildFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

CChildFrame::CChildFrame ( )
{

}

CChildFrame::~CChildFrame ( )
{

}

BOOL CChildFrame::PreCreateWindow ( CREATESTRUCT& cs )
{
	if ( !CMDIChildWnd::PreCreateWindow ( cs ) )
		return FALSE;

	return TRUE;
}

void CChildFrame::ActivateFrame ( int nCmdShow )
{
	nCmdShow = SW_SHOWMAXIMIZED;

	CMDIChildWnd::ActivateFrame ( nCmdShow );
}


#ifdef _DEBUG

	void CChildFrame::AssertValid ( ) const
	{
		CMDIChildWnd::AssertValid ( );
	}

	void CChildFrame::Dump ( CDumpContext& dc ) const
	{
		CMDIChildWnd::Dump ( dc );
	}

#endif
