// StoreButton.cpp : implementation file
//

#include "stdafx.h"
#include "StoreButton.h"
#include "MarketWindow.h"
#include "..\Editor.h"


// CStoreButton

IMPLEMENT_DYNAMIC(CStoreButton, CButton)

CStoreButton::CStoreButton()
{
	m_bLButtonDown = false;
}

CStoreButton::~CStoreButton()
{
}


BEGIN_MESSAGE_MAP(CStoreButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CStoreButton message handlers

void CStoreButton::OnLButtonDown( UINT nFlags, CPoint point )
{
	CButton::OnLButtonDown( nFlags, point );

	m_bLButtonDown = true;
	SetCapture( );
}

void CStoreButton::OnLButtonUp( UINT nFlags, CPoint point )
{
	ReleaseCapture( );

	if ( m_bLButtonDown )
	{
		CRect rect;
		GetClientRect( rect );

		//char str [256];
		//sprintf_s( str, 256, "%d, %d", point.x, point.y );
		//MessageBox( str );

		if ( rect.PtInRect( point ) )
		{
			DWORD dwState = 0;
			BOOL bOnline = InternetGetConnectedState( &dwState, 0 );
			if ( !bOnline )
			{
				MessageBox( "Could not detect an internet connection", theApp.m_szErrorString );
			}
			else
			{
				SetWindowText( "Loading Store..." );
				//MessageBox( "Button Up" );
				
				char str [ 256 ];
				theApp.GetMainWnd()->GetWindowText( str, 256 );

				theApp.m_iAppX = 0;
				theApp.m_iAppY = 0;
				theApp.m_iAppWidth = 0;
				theApp.m_iAppHeight = 0;

				theApp.GetMainWnd()->SetWindowText( "FPS Creator - Game Creator Store" );
				CMarketWindow TGCStore;
				TGCStore.DoModal();

				theApp.GetMainWnd()->SetWindowPos( NULL, theApp.m_iAppX,theApp.m_iAppY, theApp.m_iAppWidth,theApp.m_iAppHeight, 0 );
				//theApp.GetMainWnd()->ShowWindow( SW_SHOW );

				//theApp.GetMainWnd()->SetWindowText( str );
				theApp.GetMainWnd()->SetWindowText( "FPS Creator" );
				
				SetWindowText( "Game Creator Store" );
			}
		}
	}

	m_bLButtonDown = false;
}
