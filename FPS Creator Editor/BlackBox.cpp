// BlackBox.cpp : implementation file
//

#include "stdafx.h"
#include "BlackBox.h"


// CBlackBox

IMPLEMENT_DYNAMIC(CBlackBox, CStatic)

CBlackBox::CBlackBox()
{

}

CBlackBox::~CBlackBox()
{
}


BEGIN_MESSAGE_MAP(CBlackBox, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CBlackBox message handlers

void CBlackBox::OnPaint( )
{
	CPaintDC paintDC( this );

	CRect clientRect;
	GetClientRect( &clientRect );

	CBrush brush;
	brush.CreateSolidBrush( RGB(0,0,0) );

	CBrush *oldBrush = paintDC.SelectObject( &brush );
	paintDC.Rectangle( &clientRect );

	paintDC.SelectObject( oldBrush );
	brush.DeleteObject();
}
