// OutputBar.cpp : implementation of the COutputBar class
//

#include "stdafx.h"
#include "Editor.h"
#include "OutputBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP ( COutputBar, CBCGSizingControlBar )
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COutputBar::COutputBar()
{
	
}

COutputBar::~COutputBar ( )
{

}

int COutputBar::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	lpCreateStruct->cx = 300;
	lpCreateStruct->cy = 50;

	if ( CBCGSizingControlBar::OnCreate ( lpCreateStruct ) == -1 )
		return -1;

	CRect rectClient;
	GetClientRect( rectClient );
	m_NewBrowser.Create ( NULL, _T ( "" ), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rectClient, this, 1 );
	
	return 0;
}

void COutputBar::AddBitmap ( int iTab, LPTSTR szBitmap, LPTSTR szName )
{
	m_NewBrowser.AddBitmap ( iTab, szBitmap, szName );
}

void COutputBar::RemoveBitmap ( int iTab, int iIndex)
{
	m_NewBrowser.RemoveBitmap ( iTab, iIndex );
}

void COutputBar::OnSize ( UINT nType, int cx, int cy )
{
	CBCGSizingControlBar::OnSize ( nType, cx, cy );

	m_NewBrowser.SetWindowPos ( NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
}

void COutputBar::SetBrowserSize ( int cx, int cy )
{

}

void COutputBar::OnLButtonUp ( UINT nFlags, CPoint point )
{
	CBCGSizingControlBar::OnLButtonUp ( nFlags, point );
}