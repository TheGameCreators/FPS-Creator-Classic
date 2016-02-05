// CMediaProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\Editor.h"
#include "MediaProgressBar.h"

#include <math.h>


// CMediaProgressBar

IMPLEMENT_DYNAMIC(CMediaProgressBar, CProgressCtrl)

CMediaProgressBar::CMediaProgressBar()
{
	m_iDownloadPos = 0;
	m_iPlayPos = 0;
	m_iMaxPlayPos = 100;

	//SetRange( 0, 100 );
}

CMediaProgressBar::~CMediaProgressBar()
{
}


BEGIN_MESSAGE_MAP(CMediaProgressBar, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_MESSAGE( PBM_SETPOS, &SetPosMsg )
END_MESSAGE_MAP()

/*
int CMediaProgressBar::SetPos( int nPos )
{
	if ( nPos <= 0 ) nPos = 100;
	if ( nPos > 100 ) nPos = 100;

	m_iDownloadPos = nPos;
	Invalidate();

	return 0;
}
*/

LRESULT CMediaProgressBar::SetPosMsg( WPARAM wParam, LPARAM lParam )
{
	int nPos = wParam;

	if ( nPos <= 0 ) nPos = 100;
	if ( nPos > 100 ) nPos = 100;

	m_iDownloadPos = nPos;
	Invalidate();

	return 0;
}
	
void CMediaProgressBar::SetPlayPos( long pos  )
{
	if ( pos < 0 ) pos = 0;
	if ( pos > m_iMaxPlayPos ) pos = m_iMaxPlayPos;
	m_iPlayPos = pos;
	Invalidate();
}

void CMediaProgressBar::SetPlayRange( long max )
{
	if ( m_iMaxPlayPos < 1 ) m_iMaxPlayPos = 1;
	m_iMaxPlayPos = max;
	Invalidate();
}


// CMediaProgressBar message handlers


void CMediaProgressBar::OnPaint()
{
	CPaintDC dc( this );
	CRect clientRect;
	GetClientRect( &clientRect );

	dc.SetBkMode( TRANSPARENT );
	dc.SelectStockObject( BLACK_PEN );
	dc.SelectStockObject( LTGRAY_BRUSH );

	clientRect.DeflateRect( 0,1 );

	dc.RoundRect( &clientRect, CPoint(2,2) );

	CBrush greenBrush, green2Brush, *oldBrush;
	greenBrush.CreateSolidBrush( RGB(0,135,0) );
	green2Brush.CreateSolidBrush( RGB(0,220,0) );

	CRect drawRect = clientRect;
	drawRect.right = (int) ( (clientRect.Width()*(float)m_iDownloadPos) / 100.0f );

	oldBrush = dc.SelectObject( &greenBrush );
	dc.RoundRect( &drawRect, CPoint(2,2) );

	drawRect.right = (int) ( (clientRect.Width() * (float)m_iPlayPos) / (float)m_iMaxPlayPos );

	dc.SelectObject( &green2Brush );
	dc.RoundRect( &drawRect, CPoint(2,2) );
}

BOOL CMediaProgressBar::OnEraseBkgnd( CDC *pDC )
{
	return TRUE;
}

void CMediaProgressBar::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		
	}
}