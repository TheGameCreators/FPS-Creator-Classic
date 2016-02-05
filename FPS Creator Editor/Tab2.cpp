// Tab2.cpp : implementation file
//

#include "stdafx.h"
#include "Tab2.h"
#include "Editor.h"
#include "HelpWizard.h"


// CTab2 dialog

IMPLEMENT_DYNAMIC(CTab2, CDialog)

CTab2::CTab2(CWnd* pParent /*=NULL*/)
	: CDialog(CTab2::IDD, pParent)
{
	m_pCurrVideo = NULL;
	m_bPaused = false;
	m_bResizing = false;
	m_bInit = false;
}

CTab2::~CTab2()
{
	if ( m_pCurrVideo ) 
	{
		m_pCurrVideo->Stop();
		delete m_pCurrVideo;
		m_pCurrVideo = NULL;
	}
}

void CTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_VideosList);
	DDX_Control(pDX, IDC_BUTTON1, m_btnPlay);
	DDX_Control(pDX, IDC_BUTTON2, m_btnCloseVideo);
	DDX_Control(pDX, IDC_BLACK_BOX, m_BlackBox);
	DDX_Control(pDX, IDC_BUTTON3, m_btnPauseResume);
}

BOOL CTab2::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE) pMsg->wParam=NULL ;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CTab2, CDialog)
	ON_WM_SIZE()

	ON_NOTIFY( NM_DBLCLK, IDC_LIST1, OnDblClickList )
	ON_NOTIFY( NM_CLICK, IDC_LIST1, OnClickList )
	ON_BN_CLICKED(IDC_BUTTON1, &CTab2::OnPlayClicked)
	ON_BN_CLICKED(IDC_BUTTON2, &CTab2::OnCloseVideoClicked)
	ON_BN_CLICKED(IDC_BUTTON3, &CTab2::OnPauseResumeClicked)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTab2 message handlers

BOOL CTab2::OnInitDialog( )
{
	if ( !CDialog::OnInitDialog( ) ) return FALSE;

	m_VideosList.Init( );

	m_btnCloseVideo.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("CloseVideo") ) );
	m_btnPauseResume.SetWindowText( GetLanguageData( _T("Getting Started"), _T("Pause") ) );
	m_btnPlay.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("PlayVideo") ) );

	if ( m_pCurrVideo )
	{
		m_VideosList.ShowWindow( SW_HIDE );
		m_btnCloseVideo.ShowWindow( SW_SHOW );
		m_btnPauseResume.ShowWindow( SW_SHOW );
		m_BlackBox.ShowWindow( SW_SHOW );

		IBaseFilter *pVMR9 = NULL;
		m_pCurrVideo->m_hWnd = GetSafeHwnd();
		m_pCurrVideo->InitializeWindowlessVMR( &pVMR9 );

		m_bPaused = true;
		m_btnPauseResume.SetWindowText( GetLanguageData( _T("Getting Started"), _T("Resume") ) );
	}

	m_bInit = true;

	return TRUE;
}

void CTab2::OnClosing( )
{
	if ( m_pCurrVideo )
	{
		m_pCurrVideo->Stop();
		delete m_pCurrVideo;
		m_pCurrVideo = NULL;
	}
}

void CTab2::OnSize( UINT nType, int cx, int cy )
{
	if ( !m_bInit ) return;

	CDialog::OnSize( nType, cx, cy );

	CRect clientRect, btnRect, btn2Rect, btn3Rect;
	GetClientRect( &clientRect );
	m_btnPlay.GetClientRect( &btnRect );
	m_btnPauseResume.GetClientRect( &btn2Rect );
	m_btnCloseVideo.GetClientRect( &btn3Rect );


	m_VideosList.SetWindowPos( NULL, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height() - 27, NULL );
	m_VideosList.Arrange( LVA_ALIGNTOP );
	m_BlackBox.SetWindowPos( NULL, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height() - 27, NULL );
	
	int x = clientRect.right - btnRect.Width() - 1;
	m_btnPlay.SetWindowPos( NULL, x, clientRect.bottom - btnRect.Height() - 1, 0,0, SWP_NOSIZE );
	
	x = x - btn2Rect.Width() - 2;
	m_btnPauseResume.SetWindowPos( NULL, x, clientRect.bottom - btn2Rect.Height() - 1, 0,0, SWP_NOSIZE );

	x = x - btn3Rect.Width() - 2;
	m_btnCloseVideo.SetWindowPos( NULL, x, clientRect.bottom - btn3Rect.Height() - 1, 0,0, SWP_NOSIZE );

	if ( m_pCurrVideo ) 
	{
		pos = m_pCurrVideo->GetCurrentPosition( );
		m_pCurrVideo->Stop();
		//m_pCurrVideo->Resize( cx,cy-27 );
		//OnPlayClicked( );
		//m_pCurrVideo->SetPosition( pos );
		SetTimer( 1, 500, NULL );
	}
}

void CTab2::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		KillTimer( 1 );

		//m_bResizing = false;
		if ( m_pCurrVideo ) 
		{
			OnPlayClicked( );
			m_pCurrVideo->SetPosition( pos );
		}
	}

	if ( nIDEvent == 2 )
	{
		if ( !m_pCurrVideo ) KillTimer( 2 );
		else
		{
			if ( m_pCurrVideo->CheckStatus() )
			{
				KillTimer( 2 );
				OnCloseVideoClicked();
			}
		}
	}
}

void CTab2::OnClickList( NMHDR* pNMHDR, LRESULT* pResult )
{
	m_VideosList.OnClickList( pNMHDR, pResult );
}

void CTab2::OnDblClickList( NMHDR* pNMHDR, LRESULT* pResult )
{
	OnPlayClicked();
}
void CTab2::OnPlayClicked()
{
	//char szCurrDir [ MAX_PATH ];
	//GetCurrentDirectory( MAX_PATH, szCurrDir );
	
	//SetCurrentDirectory( theApp.m_szDirectory );
	//SetCurrentDirectory( "Files\\languagebank\\english\\helpwizard\\videos\\" );

	//MessageBox( "Play Video" );

	//theApp.m_pHelpWizardDlg->ShowWindow( SW_MINIMIZE );

	if ( m_pCurrVideo ) 
	{
		/*
		m_pCurrVideo->Stop();
		m_pCurrVideo->SetPosition( 0 );
		m_pCurrVideo->Play( );
		return;
		*/

		m_pCurrVideo->Stop();
		delete m_pCurrVideo;
		m_pCurrVideo = NULL;
	}

	TCHAR szFilename [ 256 ];
	if ( !m_VideosList.GetFilenameSelected( 256, szFilename ) )
	{
		return;
	}

	CString sFullPath = theApp.m_szDirectory;
	sFullPath += "\\";
	sFullPath += (theApp.m_szLanguagePath + 2);
	sFullPath += "\\helpwizard\\videos\\";
	sFullPath += szFilename;

	CRect clientRect;
	GetClientRect( &clientRect );

	//m_btnPlay.EnableWindow( FALSE );
	m_VideosList.ShowWindow( SW_HIDE );
	m_btnCloseVideo.ShowWindow( SW_SHOW );
	m_btnPauseResume.ShowWindow( SW_SHOW );
	m_BlackBox.ShowWindow( SW_SHOW );

	IBaseFilter *pVMR9 = NULL;
	m_pCurrVideo = new cVideo( false, "", sFullPath.GetString(), 0, 0, clientRect.Width(), clientRect.Height() - 27, this->GetSafeHwnd() );
	if ( !m_pCurrVideo->m_bSucceed )
	{
		OnCloseVideoClicked();
		return;
	}
	
	m_pCurrVideo->InitializeWindowlessVMR( &pVMR9 );
	m_pCurrVideo->Play();
	m_btnPauseResume.SetWindowText( GetLanguageData( _T("Getting Started"), _T("Pause") ) );
	m_bPaused = false;

	SetTimer( 2, 1000, NULL );

	/*
	//Play using Windows Media Player
	SHELLEXECUTEINFO execInfo;
	execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	execInfo.dwHotKey = 0;
	execInfo.fMask = NULL;
	execInfo.hIcon = NULL;
	execInfo.hInstApp = 0;
	execInfo.hkeyClass = NULL;
	execInfo.hMonitor = NULL;
	execInfo.hProcess = NULL;
	execInfo.hwnd = NULL;
	execInfo.lpClass = NULL;
	execInfo.lpDirectory = NULL;
	execInfo.lpFile = "Video.wmv";
	execInfo.lpIDList = NULL;
	execInfo.lpParameters = NULL;
	execInfo.lpVerb = "open";
	execInfo.nShow = SW_SHOW;

	ShellExecuteEx( &execInfo );
	*/

	//SetCurrentDirectory( szCurrDir );
}

void CTab2::OnCloseVideoClicked()
{
	m_btnCloseVideo.ShowWindow( SW_HIDE );
	m_btnPauseResume.ShowWindow( SW_HIDE );
	m_BlackBox.ShowWindow( SW_HIDE );

	if ( m_pCurrVideo ) 
	{
		m_pCurrVideo->Stop();
		delete m_pCurrVideo;
		m_pCurrVideo = NULL;
	}

	//m_btnPlay.EnableWindow( TRUE );
	m_VideosList.ShowWindow( SW_SHOW );

	Invalidate( );
}

void CTab2::OnPauseResumeClicked()
{
	if ( !m_pCurrVideo )
	{
		return;
	}

	if ( m_bPaused ) 
	{
		m_pCurrVideo->Play( );
		m_btnPauseResume.SetWindowText( GetLanguageData( _T("Getting Started"), _T("Pause") ) );
	}
	else 
	{
		m_pCurrVideo->Stop( );
		m_btnPauseResume.SetWindowText( GetLanguageData( _T("Getting Started"), _T("Resume") ) );
	}

	m_bPaused = !m_bPaused;
}

void CTab2::Pause()
{
	if ( !m_pCurrVideo )
	{
		return;
	}

	if ( m_bPaused ) return;
	m_bPaused = true;

	m_pCurrVideo->Stop( );
	m_btnPauseResume.SetWindowText( GetLanguageData( _T("Getting Started"), _T("Resume") ) );
}
