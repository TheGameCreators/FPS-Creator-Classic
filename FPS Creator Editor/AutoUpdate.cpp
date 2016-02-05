// AutoUpdate.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include <afxinet.h>
#include "Editor.h"
#include "UpdateCheckThread.h"
#include "TGC Store\HTTPComm.h"

// CAutoUpdate dialog

IMPLEMENT_DYNAMIC(CAutoUpdate, CDialog)

CAutoUpdate::CAutoUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoUpdate::IDD, pParent)
{
	m_bUpdateAvailable = false;
	m_pUpdateThread = NULL;
	m_bDownloadComplete = false;
	m_pCheckThread = NULL;
}

CAutoUpdate::~CAutoUpdate()
{
	if ( m_pUpdateThread ) 
	{
		//m_pUpdateThread->Terminate( );
		m_pUpdateThread->CancelDownload( );
		m_pUpdateThread->Join( );
		delete m_pUpdateThread;
	}
	m_pUpdateThread = NULL;

	if ( m_pCheckThread ) 
	{
		//m_pUpdateThread->Terminate( );
		m_pCheckThread->CancelDownload( );
		m_pCheckThread->Join( );
		delete m_pCheckThread;
	}
	m_pCheckThread = NULL;
}

void CAutoUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_TxtCurrentVersion);
	DDX_Control(pDX, IDC_EDIT2, m_TxtLatestVersion);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnDownloadUpdate);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Control(pDX, IDC_CHECK1, m_chkCheckAtStartup);
	DDX_Control(pDX, IDC_EDIT4, m_MsgText);
	DDX_Control(pDX, IDC_AUPDATE_TEXT, m_UpdateText);
	DDX_Control(pDX, IDC_UPDATE_IMAGE, m_UpdateImage);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnCheckForUpdates);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
	DDX_Control(pDX, IDC_CURR_VERSION, m_staCurrentVersion);
	DDX_Control(pDX, IDC_LATEST_VERSION, m_StaLatestVersion);
}

BOOL CAutoUpdate::OnInitDialog( )
{
	if ( !CDialog::OnInitDialog( ) ) return FALSE;

	// image for dialog
	m_UpdateImage.LoadBitmap( IDB_UPDATE_LOGO );

	m_btnClose.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("Close") ) );
	m_BtnDownloadUpdate.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("Download") ) );
	m_BtnCheckForUpdates.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("Check") ) );
	m_chkCheckAtStartup.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("CheckBox2") ) );
	m_staCurrentVersion.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("Version1") ) );
	m_StaLatestVersion.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("Version2") ) );

	DWORD dwValue = 1;
	if ( !theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "check_x9_updates_on_startup", &dwValue ) ) dwValue = 0;

	if ( dwValue ) m_chkCheckAtStartup.SetCheck( 1 );
	else m_chkCheckAtStartup.SetCheck( 0 );

	////////////////////////
	//check current version
	TCHAR szVersion [ 64 ];
	_tcscpy_s( szVersion, 64, _T("n/a") );

	FILE *pFile = NULL;
	int error = _tfopen_s( &pFile, _T("version.ini"), _T("rb") );
	if ( !error )
	{
		_fgetts( szVersion, 64, pFile );
	}

	m_TxtCurrentVersion.SetWindowText( szVersion );

	if ( pFile ) fclose( pFile );

	TCHAR szMsg [ 512 ];
	GetPrivateProfileString( _T("Auto Update"), _T("UpdateMessage1"), _T(""), szMsg, 512, theApp.m_szLanguage );
	_tcscat_s( szMsg, 512, _T("\r\n\r\n") );

	TCHAR szMsg2 [ 256 ];
	GetPrivateProfileString( _T("Auto Update"), _T("UpdateMessage2"), _T(""), szMsg2, 256, theApp.m_szLanguage );
	_tcscat_s( szMsg, 512, szMsg2 );

	m_UpdateText.SetWindowText( szMsg );

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAutoUpdate, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoUpdate::OnCheckForUpdates)
	ON_BN_CLICKED(IDC_BUTTON2, &CAutoUpdate::OnDownloadUpdate)
	ON_BN_CLICKED(IDC_CHECK1, &CAutoUpdate::OnCheckAtStartupClicked)
	ON_WM_TIMER( )
	ON_BN_CLICKED(IDCANCEL, &CAutoUpdate::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_AUPDATE_TEXT, &CAutoUpdate::OnStnClickedAupdateText)
END_MESSAGE_MAP()


// CAutoUpdate message handlers

void CAutoUpdate::OnCheckForUpdates()
{
	m_BtnDownloadUpdate.EnableWindow( FALSE );
	m_BtnCheckForUpdates.EnableWindow( FALSE );
	m_MsgText.SetWindowText( GetLanguageData( _T("Auto Update"), _T("Checking") ) );

	if ( m_pCheckThread ) 
	{
		m_pCheckThread->CancelDownload( );
		m_pCheckThread->Join( );
		delete m_pCheckThread;
		m_pCheckThread = NULL;
	}

	m_pCheckThread = new UpdateCheckThread;
	m_pCheckThread->SetStartUpCheck( );
	m_pCheckThread->Start( );

	SetTimer( 2, 1000, NULL );
}

void CAutoUpdate::OnDownloadUpdate()
{
	int result = MessageBox( GetLanguageData( _T("Auto Update"), _T("DownloadConfirm") ), GetLanguageData( _T("Auto Update"), _T("AutoUpdate") ), MB_YESNO );
	if ( result == IDYES )
	{
		m_ProgressBar.SetRange32( 0, 100 );
		m_ProgressBar.SetPos( 0 );
		m_ProgressBar.ShowWindow( SW_SHOW );
		m_MsgText.SetWindowText( GetLanguageData( _T("Auto Update"), _T("Downloading") ) );

		m_BtnDownloadUpdate.EnableWindow( FALSE );
		m_BtnCheckForUpdates.EnableWindow( FALSE );

		if ( m_pUpdateThread )
		{
			m_pUpdateThread->CancelDownload( );
			m_pUpdateThread->Join( );
			//m_pUpdateThread->Terminate( );
			delete m_pUpdateThread;
		}

		m_bDownloadComplete = false;

		m_pUpdateThread = new UpdateCheckThread;
		m_pUpdateThread->SetDownloadFile( _T("FPSCX9Update.exe"), &m_ProgressBar, &m_MsgText, this );
		m_pUpdateThread->Start( );

		SetTimer( 1, 1000, NULL );
	}
}

void CAutoUpdate::DownloadComplete( )
{
	m_bDownloadComplete = true;
}

void CAutoUpdate::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		if ( m_bDownloadComplete )
		{
			KillTimer( 1 );
			m_bDownloadComplete = false;

			HTTPComm server;
			server.PostCounter( );

			CString sFullPath = theApp.m_szDirectory;
			sFullPath += _T("\\Updates\\FPSCX9Update.exe");
			HINSTANCE hInst = ShellExecute( NULL, _T("open"), sFullPath, NULL, NULL, SW_SHOW );

			if ( (int)hInst > 32 )
			{
				theApp.OnAppExit1();
				EndDialog( 0 );		//exit doesn't take hold until dialog closed
			}
			else
			{
				m_MsgText.SetWindowText( GetLanguageData( _T("Auto Update"), _T("FailedRun") ) );
			}
		}
	}

	if ( nIDEvent == 2 )
	{
		if ( !m_pCheckThread )
		{
			KillTimer( 2 );
			m_BtnDownloadUpdate.EnableWindow( TRUE );
			m_BtnCheckForUpdates.EnableWindow( TRUE );
		}
		else if ( m_pCheckThread->CheckFinished( ) )
		{
			KillTimer( 2 );

			m_TxtLatestVersion.SetWindowText( m_pCheckThread->GetLatestVersion() );

			TCHAR szCurrVersion [ 64 ];
			m_TxtCurrentVersion.GetWindowText( szCurrVersion, 64 );

			if ( _tcscmp( m_pCheckThread->GetLatestVersion(), _T("n/a") ) == 0 )
			{
				m_MsgText.SetWindowText( GetLanguageData( _T("Auto Update"), _T("FailedVersion") ) );
				m_BtnDownloadUpdate.ShowWindow( SW_HIDE );
			}
			else 
			{
				int iRel = _tcscmp( szCurrVersion, m_pCheckThread->GetLatestVersion() );
				if ( iRel != 0 )
				{
					//float fCurrentVersion = (float) _tstof( szCurrVersion );
					//float fLatestVersion = (float) _tstof( m_pCheckThread->GetLatestVersion() );
					//if ( fLatestVersion > fCurrentVersion )
					if ( iRel < 0 )
					{
						m_MsgText.SetWindowText( GetLanguageData( _T("Auto Update"), _T("VersionFound") ) );
						m_BtnDownloadUpdate.ShowWindow( SW_SHOW );
					}
					else
					{
						m_MsgText.SetWindowText( GetLanguageData( _T("Auto Update"), _T("VersionGreater") ) );
						m_BtnDownloadUpdate.ShowWindow( SW_HIDE );
					}
				}
				else
				{
					m_MsgText.SetWindowText( GetLanguageData( _T("Auto Update"), _T("UpToDate") ) );
					m_BtnDownloadUpdate.ShowWindow( SW_HIDE );
				}
			}

			m_BtnDownloadUpdate.EnableWindow( TRUE );
			m_BtnCheckForUpdates.EnableWindow( TRUE );

			m_pCheckThread->Join( );
			delete m_pCheckThread;
			m_pCheckThread = NULL;
		}
	}
}

void CAutoUpdate::OnCheckAtStartupClicked()
{
	theApp.SetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "check_x9_updates_on_startup", m_chkCheckAtStartup.GetCheck() );
}

void CAutoUpdate::OnBnClickedCancel()
{
	if ( m_pUpdateThread )
	{
		int result = MessageBox( GetLanguageData( _T("Auto Update"), _T("Cancel") ), _T(""), MB_YESNO );
		if ( result == IDYES ) OnCancel();
	}
	else
	{
		OnCancel();
	}
}

void CAutoUpdate::OnStnClickedAupdateText()
{
	// TODO: Add your control notification handler code here
}
