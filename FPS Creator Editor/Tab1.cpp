// Tab1.cpp : implementation file
//

#include "stdafx.h"
#include "Tab1.h"
#include <atlimage.h>
#include "Editor.h"
#include "MyTabCtrl.h"


// CTab1 dialog

IMPLEMENT_DYNAMIC(CTab1, CDialog)

CTab1::CTab1(CWnd* pParent /*=NULL*/)
	: CDialog(CTab1::IDD, pParent)
{
	m_pIntroVideo = NULL;
	m_bResizing = false;
	m_pOwnerTabCtrl = NULL;
	m_bInit = false;
}

CTab1::~CTab1()
{
	if ( m_pIntroVideo )
	{
		m_pIntroVideo->Stop();
		delete m_pIntroVideo;
		m_pIntroVideo = NULL;
	}
}

void CTab1::SetTabOwner( CMyTabCtrl *pOwner )
{
	m_pOwnerTabCtrl = pOwner;
}

void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INTRO_IMAGE, m_IntroImg);
}

BOOL CTab1::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE) pMsg->wParam=NULL ;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CTab1, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTab1 message handlers

BOOL CTab1::OnInitDialog( )
{
	if ( !CDialog::OnInitDialog( ) ) return FALSE;

	SetTimer( 1, 4000, NULL );	//don't call if intro video is not desired

	CRect clientRect;
	GetClientRect( &clientRect );

	m_IntroImg.SetWindowPos( NULL, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), NULL );
	//m_IntroImg.LoadBitmap( IDB_HELP_INTRO );

	//char szCurrDir [ MAX_PATH ];
	//GetCurrentDirectory( MAX_PATH, szCurrDir );
	//SetCurrentDirectory( theApp.m_szDirectory );
	//SetCurrentDirectory( "Files\\languagebank\\english\\helpwizard\\artwork" );

	CString sFullPath = theApp.m_szDirectory;
	sFullPath += _T("\\");
	sFullPath += (theApp.m_szLanguagePath + 2);
	sFullPath += _T("\\TGCStore\\");
	sFullPath += _T("MainSplash.bmp");

	CImage image;
	if ( FAILED( image.Load( sFullPath.GetString() ) ) )
	{
		//MessageBox( GetLanguageData( _T("Getting Started"), _T("NoIntro") ) );
	}
	else
	{
		CWindowDC wndDC( this );
		CDC dc;
		dc.CreateCompatibleDC( &wndDC );

		CBitmap bmpCurrImage;
		bmpCurrImage.CreateBitmap( image.GetWidth(),image.GetHeight(), 1, 32, NULL );
		CBitmap *oldBmp = dc.SelectObject( &bmpCurrImage );

		::SetStretchBltMode( image.GetDC(), HALFTONE );
		::SetBrushOrgEx( image.GetDC(), 0,0, NULL );
		image.StretchBlt( dc.GetSafeHdc( ), 0,0, image.GetWidth(),image.GetHeight() );
		dc.SelectObject( oldBmp );
		m_IntroImg.CopyBitmap( &bmpCurrImage );

		image.ReleaseDC();
		image.ReleaseDC();

		bmpCurrImage.DeleteObject();
	}

	//SetCurrentDirectory( szCurrDir );

	m_bInit = true;

	return TRUE;
}

void CTab1::OnClosing( )
{
	KillTimer( 1 );

	if ( m_pIntroVideo )
	{
		m_pIntroVideo->Stop();
		delete m_pIntroVideo;
		m_pIntroVideo = NULL;
	}

	m_IntroImg.ShowWindow( SW_SHOW );
}

void CTab1::OnSize( UINT nType, int cx, int cy )
{
	if ( !m_bInit ) return;

	CDialog::OnSize( nType, cx, cy );

	CRect clientRect;
	GetClientRect( &clientRect );

	int width = m_IntroImg.GetWidth( );
	int height = m_IntroImg.GetHeight( );

	int midX = ( clientRect.Width() - width ) / 2;
	int midY = ( clientRect.Height() - height ) / 2;

	m_IntroImg.SetWindowPos( NULL, clientRect.left + midX, clientRect.top + midY, width, height, NULL );
	if ( m_pIntroVideo ) 
	{
		//m_pIntroVideo->Resize( cx,cy );
		pos = m_pIntroVideo->GetCurrentPosition( );
		m_pIntroVideo->Stop( );
		m_bResizing = true;
		SetTimer( 2, 500, NULL );
	}
}

void CTab1::OnRestart( )
{
	m_IntroImg.ShowWindow( SW_SHOW );
	SetTimer( 1, 4000, NULL );
}

void CTab1::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		KillTimer( 1 );

		CString sFullPath = theApp.m_szDirectory;
		sFullPath += "\\";
		sFullPath += (theApp.m_szLanguagePath + 2);
		sFullPath += "\\TGCStore\\";
		sFullPath += "intro.wmv";

		CRect clientRect;
		GetClientRect( &clientRect );

		m_IntroImg.ShowWindow( SW_HIDE );
		//m_BlackBox.ShowWindow( SW_SHOW );

		IBaseFilter *pVMR9 = NULL;
		m_pIntroVideo = new cVideo( false, "", sFullPath.GetString(), 0, 0, clientRect.Width(), clientRect.Height(), this->GetSafeHwnd() );
		if ( !m_pIntroVideo->m_bSucceed )
		{
			m_IntroImg.ShowWindow( SW_SHOW );

			m_pIntroVideo->Stop();
			delete m_pIntroVideo;
			m_pIntroVideo = NULL;

			return;
		}
		
		m_pIntroVideo->InitializeWindowlessVMR( &pVMR9 );
		m_pIntroVideo->Play();

		SetTimer( 3, 1000, NULL );
	}

	if ( nIDEvent == 2 )
	{
		KillTimer( 2 );

		CString sFullPath = theApp.m_szDirectory;
		sFullPath += "\\";
		sFullPath += (theApp.m_szLanguagePath + 2);
		sFullPath += "\\TGCStore\\";
		sFullPath += "intro.wmv";

		CRect clientRect;
		GetClientRect( &clientRect );

		m_IntroImg.ShowWindow( SW_HIDE );

		IBaseFilter *pVMR9 = NULL;
		m_pIntroVideo = new cVideo( false, "", sFullPath.GetString(), 0, 0, clientRect.Width(), clientRect.Height(), this->GetSafeHwnd() );
		if ( !m_pIntroVideo->m_bSucceed )
		{
			m_IntroImg.ShowWindow( SW_SHOW );

			m_pIntroVideo->Stop();
			delete m_pIntroVideo;
			m_pIntroVideo = NULL;

			return;
		}
		
		m_pIntroVideo->InitializeWindowlessVMR( &pVMR9 );
		m_pIntroVideo->Play();
		m_pIntroVideo->SetPosition( pos );
	}

	if ( nIDEvent == 3 )
	{
		if ( !m_pIntroVideo ) KillTimer( 3 );
		else
		{
			if ( m_pIntroVideo->CheckStatus( ) )
			{
				//MessageBox( "Video Finished" );
				//m_pOwnerTabCtrl->SetCurFocus( 1 );
				//m_pOwnerTabCtrl->ChangeTab( 1 );
				KillTimer( 3 );
			}
		}
	}
}