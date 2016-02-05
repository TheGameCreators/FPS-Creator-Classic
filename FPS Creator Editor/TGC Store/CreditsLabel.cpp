// CreditsLabel.cpp : implementation file
//

#include "stdafx.h"
#include "CreditsLabel.h"


// CCreditsLabel

IMPLEMENT_DYNAMIC(CCreditsLabel, CStatic)

CCreditsLabel::CCreditsLabel()
{
	m_TextColor = RGB(0,0,0);
	m_Brush.CreateSolidBrush( RGB(220,220,220) );
}

CCreditsLabel::~CCreditsLabel()
{
}

void CCreditsLabel::SetTextColor( COLORREF color )
{
	m_TextColor = color;
}


BEGIN_MESSAGE_MAP(CCreditsLabel, CStatic)
	//ON_WM_CTLCOLOR_REFLECT( )
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CCreditsLabel message handlers

BOOL CCreditsLabel::OnEraseBkgnd( CDC *pDC )
{
	return TRUE;
}

void CCreditsLabel::OnPaint( )
{
	CPaintDC dc( this );
	
	CRect rectClient;
	GetClientRect( &rectClient );

    CBrush brush; brush.CreateSolidBrush( GetSysColor( COLOR_3DFACE ) );
	CPen pen; pen.CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DFACE ) );
	dc.SelectObject( &pen );
	dc.SelectObject( &brush );
	dc.Rectangle( &rectClient );
	
	rectClient.top++; rectClient.left++;
	rectClient.DeflateRect( 1,1 );
	brush.DeleteObject();
	brush.CreateSolidBrush( RGB(255,255,255) );
	dc.SelectObject( &brush );
	pen.DeleteObject();
	pen.CreatePen( PS_SOLID, 1, RGB(239,239,239) );
	dc.SelectObject( &pen );
	dc.Rectangle( &rectClient );

	dc.SelectStockObject( NULL_BRUSH );

	rectClient.InflateRect( 1,1 );
	pen.DeleteObject();
	pen.CreatePen( PS_SOLID, 1, RGB(190,190,190) );
	dc.SelectObject( &pen );
	dc.RoundRect( &rectClient, CPoint(2,2) );
	
	rectClient.top--; rectClient.left--;
	pen.DeleteObject();
	pen.CreatePen( PS_SOLID, 1, RGB(104,104,104) );
	dc.SelectObject( &pen );
	dc.RoundRect( &rectClient, CPoint(5,5) );	

	rectClient.bottom--;

	dc.SelectObject( GetFont( ) );
    dc.SetBkMode( TRANSPARENT );
	//dc.SetBkColor( GetSysColor( COLOR_3DFACE ) );
	dc.SetTextColor( m_TextColor );

	CString sText;
    GetWindowText( sText );

	dc.DrawTextEx( sText.GetBuffer(0), sText.GetLength(), &rectClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL );
    sText.ReleaseBuffer( );
}
