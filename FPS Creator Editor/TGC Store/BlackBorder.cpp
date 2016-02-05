// BlackBorder.cpp : implementation file
//

#include "stdafx.h"
#include "BlackBorder.h"


// CBlackBorder

IMPLEMENT_DYNAMIC(CBlackBorder, CStatic)

CBlackBorder::CBlackBorder()
{

}

CBlackBorder::~CBlackBorder()
{
}


BEGIN_MESSAGE_MAP(CBlackBorder, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CBlackBorder message handlers


void CBlackBorder::OnPaint()
{
	CPaintDC dc( this );

	CRect rectClient;
	GetClientRect( &rectClient );

    //CBrush brush; brush.CreateSolidBrush( GetSysColor( COLOR_3DFACE ) );
	CPen pen; pen.CreatePen( PS_SOLID, 1, RGB(0,0,0) );
	dc.SelectObject( &pen );
	//dc.SelectObject( &brush );
	dc.SelectStockObject( NULL_BRUSH );
	dc.Rectangle( &rectClient );
}