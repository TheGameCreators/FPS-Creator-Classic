// MyTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyTabCtrl.h"
#include "Tab1.h"
#include "Tab2.h"
#include "Tab3.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{
	m_iNumberOfPages = 3;

	m_tabPages[ 0 ] = new CTab1;
	m_tabPages[ 1 ] = new CTab2;
	m_tabPages[ 2 ] = new CTab3;

	m_tabCurrent = 0;
}

CMyTabCtrl::~CMyTabCtrl()
{
	for( int nCount = 0; nCount < m_iNumberOfPages; nCount++)
	{
		if ( m_tabPages[ nCount ] ) delete m_tabPages[ nCount ];
	}
}

void CMyTabCtrl::Init( )
{
	if ( m_tabPages[ 0 ] ) 
	{
		m_tabPages[ 0 ]->Create( IDD_DIALOG2, this );
		m_tabPages[ 0 ]->ShowWindow( SW_SHOW );
	}
	if ( m_tabPages[ 1 ] ) 
	{
		m_tabPages[ 1 ]->Create( IDD_TAB2, this );
		m_tabPages[ 1 ]->ShowWindow( SW_SHOW );
	}
	if ( m_tabPages[ 2 ] ) 
	{
		m_tabPages[ 2 ]->Create( IDD_TAB3, this );
		m_tabPages[ 2 ]->ShowWindow( SW_SHOW );
	}

	( (CTab1*)m_tabPages[ 0 ] )->SetTabOwner( this );
	SetCurFocus( m_tabCurrent );

	SetRectangle();
}

void CMyTabCtrl::OnClosing( )
{
	((CTab1*) m_tabPages[ 0 ])->OnClosing( );
	((CTab2*) m_tabPages[ 1 ])->OnClosing( );
}

void CMyTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect( &tabRect );
	GetItemRect( 0, &itemRect );

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	if ( m_tabPages[ m_tabCurrent ] ) m_tabPages[ m_tabCurrent ]->SetWindowPos( &wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW );
	for( int nCount = 0; nCount < m_iNumberOfPages; nCount++ )
	{
		if ( nCount != m_tabCurrent && m_tabPages[ nCount ] ) m_tabPages[ nCount ]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}

	if ( m_tabCurrent != 0 ) ((CTab1*) m_tabPages[ 0 ])->OnClosing( );
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CMyTabCtrl message handlers

void CMyTabCtrl::OnSize( UINT nType, int cx, int cy )
{
	CTabCtrl::OnSize( nType, cx, cy );

	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect( &tabRect );
	GetItemRect( 0, &itemRect );

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	for( int nCount = 0; nCount < m_iNumberOfPages; nCount++ )
	{
		if ( m_tabPages[ nCount ] ) m_tabPages[ nCount ]->SetWindowPos( NULL, nX, nY, nXc, nYc, SWP_NOZORDER );
	}
}

void CMyTabCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CTabCtrl::OnLButtonDown( nFlags, point );

	ChangeTab( GetCurFocus() );
}

void CMyTabCtrl::ChangeTab( int iNewTab )
{
	if( m_tabCurrent != iNewTab )
	{
		if ( m_tabPages[ m_tabCurrent ] ) 
		{
			m_tabPages[ m_tabCurrent ]->ShowWindow( SW_HIDE );
			if ( m_tabCurrent == 0 ) ((CTab1*) m_tabPages[ 0 ])->OnClosing( );
			if ( m_tabCurrent == 1 ) ((CTab2*) m_tabPages[ 1 ])->Pause( );
		}
		
		m_tabCurrent = iNewTab;
		
		if ( m_tabPages[ m_tabCurrent ] )
		{
			m_tabPages[ m_tabCurrent ]->ShowWindow( SW_SHOW );
			m_tabPages[ m_tabCurrent ]->SetFocus();
			if ( m_tabCurrent == 0 ) ((CTab1*) m_tabPages[ 0 ])->OnRestart( );
		}
	}
}

