// CreditsLabel.cpp : implementation file
//

#include "stdafx.h"
#include "HyperlinkLabel.h"
#include "..\resource.h"
#include "..\Editor.h"


// CCreditsLabel

IMPLEMENT_DYNAMIC(CHyperlinkLabel, CStatic)

CHyperlinkLabel::CHyperlinkLabel()
{
	m_bIsHovering = false;
	m_bIsClicking = false;
	iType = 0;
	sLink = "";
}

CHyperlinkLabel::~CHyperlinkLabel()
{
}

BEGIN_MESSAGE_MAP(CHyperlinkLabel, CStatic)
	//ON_WM_CTLCOLOR_REFLECT( )
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CCreditsLabel message handlers

void CHyperlinkLabel::OnLButtonDown( UINT nFlags, CPoint point )
{
	m_bIsClicking = true;
	SetCapture();
}

void CHyperlinkLabel::OnLButtonUp( UINT nFlags, CPoint point )
{
	//char str [256];
	//sprintf_s( str, 256, "x: %d, y: %d", point.x, point.y );
	//MessageBox( str );

	if ( m_bIsClicking )
	{
		CRect rect;
		GetClientRect( rect );
		if ( rect.PtInRect( point ) )
		{
			//CWaitCursor wCur;
			//ShellExecute( NULL, "open", "http://www.thegamecreators.com/?m=registration", NULL, NULL, SW_SHOW );
			if ( iType == 0 ) GetParent()->SendMessage( WM_COMMAND, MAKELONG(IDC_LOGIN_REGISTER,BN_CLICKED) );
			else if ( iType == 1 ) 
			{
				char szBrowser [ MAX_PATH ];
				theApp.GetDefaultBrowserPath( szBrowser, MAX_PATH );
				ShellExecute( NULL, "open", szBrowser, sLink, NULL, SW_SHOW );
			}
		}
	}

	ReleaseCapture();
	m_bIsClicking = false;
}

void CHyperlinkLabel::OnMouseMove( UINT nFlags, CPoint point )
{
	CRect rect;
	GetClientRect( rect );
	if ( rect.PtInRect( point ) )
	{
		if ( !m_bIsHovering ) 
		{
			m_bIsHovering = true;
			SetCapture();
			Invalidate();
		}
	}
	else
	{
		ReleaseCapture();
		m_bIsHovering = false;
		Invalidate();
	}
}

BOOL CHyperlinkLabel::OnEraseBkgnd( CDC *pDC )
{
	return TRUE;
}

void CHyperlinkLabel::OnPaint( )
{
	CPaintDC dc( this );
	
	CRect rectClient, rectText;
	GetClientRect( &rectClient );
	rectText = rectClient;

    CBrush brush; brush.CreateSolidBrush( GetSysColor( COLOR_3DFACE ) );
	CPen pen; pen.CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DFACE ) );
	dc.SelectObject( &pen );
	dc.SelectObject( &brush );
	dc.Rectangle( &rectClient );
    
	dc.SelectObject( GetFont( ) );
    dc.SetBkMode( OPAQUE );
	dc.SetBkColor( GetSysColor( COLOR_3DFACE ) );
	dc.SetTextColor( RGB(0,0,255) );

	CString sText;
    GetWindowText( sText );

	dc.DrawTextEx( sText.GetBuffer(0), sText.GetLength(), &rectText, DT_CENTER | DT_CALCRECT, NULL );

	if ( rectText.Width() < rectClient.Width() )
	{
		int diff = rectClient.Width() - rectText.Width();
		rectText.left = rectText.left + diff/2;
		rectText.right = rectText.right + diff/2;
	}

	dc.DrawTextEx( sText.GetBuffer(0), sText.GetLength(), &rectText, DT_CENTER, NULL );
	if ( m_bIsHovering ) 
	{
		dc.SetBkMode( TRANSPARENT );
		dc.DrawTextEx( "_______________________", -1, &rectText, DT_CENTER, NULL );
	}
    sText.ReleaseBuffer( );
}
