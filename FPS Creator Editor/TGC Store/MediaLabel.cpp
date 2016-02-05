// CMediaLabel.cpp : implementation file
//

#include "stdafx.h"
#include "MediaLabel.h"


// CMediaLabel

IMPLEMENT_DYNAMIC(CMediaLabel, CStatic)

CMediaLabel::CMediaLabel()
{
	m_TextColor = RGB(0,0,0);
	m_Brush.CreateSolidBrush( RGB(220,220,220) );
	m_sText = "No File";
}

CMediaLabel::~CMediaLabel()
{
}

void CMediaLabel::SetTextColor( COLORREF color )
{
	m_TextColor = color;
}


BEGIN_MESSAGE_MAP(CMediaLabel, CStatic)
	//ON_WM_CTLCOLOR_REFLECT( )
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CMediaLabel::SetText( CString text )
{
	m_sText = text;
	//Invalidate();
}

// CMediaLabel message handlers

BOOL CMediaLabel::OnEraseBkgnd( CDC *pDC )
{
	return TRUE;
}

void CMediaLabel::OnPaint( )
{
	CPaintDC dc( this );
	
	CRect rectClient;
	GetClientRect( &rectClient );

    dc.SelectObject( GetFont( ) );
    dc.SetBkMode( TRANSPARENT );
	
	//CString sText;
    //GetWindowText( sText );

	dc.DrawText( m_sText, -1, &rectClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
}
