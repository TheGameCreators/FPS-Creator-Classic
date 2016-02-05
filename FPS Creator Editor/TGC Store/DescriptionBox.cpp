// DescriptionBox.cpp : implementation file
//

#include "stdafx.h"
#include "DescriptionBox.h"


// CDescriptionBox

IMPLEMENT_DYNAMIC(CDescriptionBox, CStatic)

CDescriptionBox::CDescriptionBox()
{

}

CDescriptionBox::~CDescriptionBox()
{
}


BEGIN_MESSAGE_MAP(CDescriptionBox, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CDescriptionBox message handlers

BOOL CDescriptionBox::OnEraseBkgnd( CDC *pDC )
{
	return TRUE;
}

void CDescriptionBox::OnPaint( )
{
	CPaintDC dc( this );
	CRect clientRect, textRect;
	GetClientRect( clientRect );
	textRect = clientRect;
	textRect.DeflateRect( 2,2 );

	CString sText;
	GetWindowText( sText );
	
	dc.SetBkMode( TRANSPARENT );
	dc.SelectObject( GetFont( ) );

	dc.DrawText( sText, -1, textRect, DT_CALCRECT | DT_WORDBREAK );

	textRect.InflateRect( 2,2 );
	textRect.right = clientRect.right;
	if ( textRect.bottom > clientRect.bottom ) textRect.bottom = clientRect.bottom;

	dc.SelectStockObject( NULL_BRUSH );
	dc.SelectStockObject( BLACK_PEN );
	dc.Rectangle( clientRect );

	textRect.DeflateRect( 2,2 );
	dc.DrawText( sText, -1, textRect, DT_WORDBREAK );
}
