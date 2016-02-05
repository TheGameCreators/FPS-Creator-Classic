// StarRating.cpp : implementation file
//

#include "stdafx.h"
#include "StarRating.h"
#include <math.h>


// CStarRating

IMPLEMENT_DYNAMIC(CStarRating, CStatic)

CStarRating::CStarRating()
{
	m_pEmptyStar = NULL;
	m_pFilledStar = NULL;
	m_pMaskStar = NULL;
	m_bRated = false;
	m_dwRating = 0;
}

CStarRating::~CStarRating()
{
}

/*
void CStarRating::SetRating( DWORD rating )
{
	m_dwRating = rating;
}
*/

void CStarRating::SetRating( DWORD totalrating, DWORD ratingcount )
{
	if ( ratingcount == 0 ) 
	{
		m_dwRating = 0;
		m_fRating = 0;
		m_bRated = false;
	}
	else
	{
		float fRating = (totalrating / (float) ratingcount);
		if ( fRating < 0 ) fRating = 0;

		m_fRating = fRating;
		m_dwRating = (DWORD) floor( fRating );
		m_bRated = true;
	}
}

void CStarRating::SetBitmaps( CBitmap *pEmpty, CBitmap *pFilled, CBitmap *pMask )
{
	m_pEmptyStar = pEmpty;
	m_pFilledStar = pFilled;
	m_pMaskStar = pMask;
}

BEGIN_MESSAGE_MAP(CStarRating, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CStarRating message handlers

void CStarRating::OnPaint( )
{
	CPaintDC dc(this);

	CRect clientRect;
	GetClientRect( clientRect );

	//dc.SelectStockObject( WHITE_PEN );
	//dc.SelectStockObject( NULL_BRUSH );
	//dc.Rectangle( clientRect );

	if ( m_bRated )
	{
		CBitmap *oldBitmap;
		CDC bmpDC;
		bmpDC.CreateCompatibleDC( &dc );
		oldBitmap = bmpDC.SelectObject( m_pFilledStar );
		
		for ( DWORD i = 0; i < m_dwRating; i++ )
		{
			dc.MaskBlt( clientRect.left, clientRect.top, 15, 15, &bmpDC, 0,0, *m_pMaskStar, 0,0, MAKEROP4(SRCCOPY,MERGEPAINT) );
			clientRect.OffsetRect( 15,0 );
		}

		bmpDC.SelectObject( m_pEmptyStar );

		for ( DWORD i = m_dwRating; i < 5; i++ )
		{
			dc.MaskBlt( clientRect.left, clientRect.top, 15, 15, &bmpDC, 0,0, *m_pMaskStar, 0,0, MAKEROP4(SRCCOPY,MERGEPAINT) );
			clientRect.OffsetRect( 15,0 );
		}

		//half stars
		bmpDC.SelectObject( m_pFilledStar );

		GetClientRect( clientRect );
		for ( DWORD i = 0; i < m_dwRating; i++ ) { clientRect.OffsetRect( 15,0 ); }

		if ( m_dwRating < 5 )
		{
			float diff = m_fRating - (float) m_dwRating;
			int width = (int) (15.0f*diff);

			dc.MaskBlt( clientRect.left, clientRect.top, width, 15, &bmpDC, 0,0, *m_pMaskStar, 0,0, MAKEROP4(SRCCOPY,MERGEPAINT) );
		}

		bmpDC.SelectObject( oldBitmap );
	}
	else
	{
		dc.SelectObject( GetFont( ) );
		dc.SetBkMode( TRANSPARENT );
		dc.SetTextColor( RGB(155,0,0) );
		dc.DrawText( "Not Rated", -1, clientRect, DT_VCENTER | DT_SINGLELINE ) ;
		dc.SetTextColor( RGB(0,0,0) );
	}
}
