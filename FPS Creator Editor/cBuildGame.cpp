// cBuildGame.cpp : implementation file
//


#include "stdafx.h"
#include "editor.h"
#include "cBuildGame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Global 
bool g_bBuildGameInArenaMode = true;
bool g_bBuildGameInProgress = false;

/////////////////////////////////////////////////////////////////////////////
// cBuildGame dialog


#define BUILD_GAME_SETUP                    0

#define BUILD_GAME_SET_TITLE                4
#define BUILD_GAME_BUTTON_OK                8
#define BUILD_GAME_BUTTON_CANCEL            12
#define BUILD_GAME_BUTTON_HELP              16
#define BUILD_GAME_BUTTON_CLOSE             20
#define BUILD_GAME_CLOSE		            24

#define BUILD_GAME_TIMER		            28

#define BUILD_GAME_BROWSE_DISPLAY			32
#define BUILD_GAME_BROWSE_FILE_SELECTED		36

#define	STRING_A							1000
#define	STRING_B							1256
#define	STRING_C							1512
#define	STRING_D							1768

cBuildGame::cBuildGame (CWnd* pParent ) : CDialog ( cBuildGame::IDD, pParent )
{
	m_pData		= NULL;
	m_bOk		= false;
	m_bCancel	= false;
	m_bCancel	= false;
	m_bHelp		= false;
	m_bClose	= false;
	m_bClean    = true;
}

void cBuildGame::DoDataExchange ( CDataExchange* pDX )
{
	CDialog::DoDataExchange ( pDX );
	//{{AFX_DATA_MAP(cBuildGame)
		DDX_Control ( pDX, IDC_TAB1, m_tabCtrl );

		DDX_Control ( pDX, IDC_BUILD_GAME_BUTTON_OK, m_Buttons [ 0 ] );
		DDX_Control ( pDX, IDC_BUILD_GAME_BUTTON_CANCEL, m_Buttons [ 1 ] );
	//}}AFX_DATA_MAP

	m_Buttons [ 0 ].SetWindowText		( GetLanguageData ( "Build Game", "Button1" ) );
	m_Buttons [ 1 ].SetWindowText		( GetLanguageData ( "Build Game", "Button2" ) );
}

BEGIN_MESSAGE_MAP ( cBuildGame, CDialog )
	//{{AFX_MSG_MAP(cBuildGame)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUILD_GAME_BUTTON_OK, OnBuildGameButtonOk)
	ON_BN_CLICKED(IDC_BUILD_GAME_BUTTON_CANCEL, OnBuildGameButtonCancel)
	ON_BN_CLICKED(IDC_BUILD_GAME_BUTTON_HELP, OnBuildGameButtonHelp)

	ON_BN_CLICKED(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

/////////////////////////////////////////////////////////////////////////////
// cBuildGame message handlers

BOOL cBuildGame::OnInitDialog ( ) 
{
	CDialog::OnInitDialog ( );

	SetWindowText ( GetLanguageData ( "Build Game", "Title" ) );

	m_Project.Create		( IDD_BUILD_GAME_PROJECT,		this );
	m_Level.Create			( IDD_BUILD_GAME_LEVEL,			this );
	m_Light.Create			( IDD_BUILD_GAME_LIGHT,			this );
	m_Multiplayer.Create   ( IDD_BUILD_GAME_MULTIPLAYER,   this );

	m_Build.Create			( IDD_BUILD_GAME_BUILD,			this );

	m_Project.SetControl		( &m_Controls );
	m_Level.SetControl			( &m_Controls );
	m_Light.SetControl			( &m_Controls );
	m_Multiplayer.SetControl	( &m_Controls );

	m_Build.SetControl			( &m_Controls );

	m_tabCtrl.AddSSLPage ( GetLanguageData ( "Build Game", "Tab1" ), 0, &m_Project );
	m_tabCtrl.AddSSLPage ( GetLanguageData ( "Build Game", "Tab2" ), 0, &m_Level   );
	m_tabCtrl.AddSSLPage ( GetLanguageData ( "Build Game", "Tab3" ), 0, &m_Light   );
    // mike - 080806 - disable this tab for boys life
	#ifdef FPSC_DEMO
	 m_tabCtrl.AddSSLPage ( GetLanguageData ( "Build Game", "Tab4" ), 0, &m_Build   );
    #else
	 m_tabCtrl.AddSSLPage ( GetLanguageData ( "Build Game", "Tab5" ), 0, &m_Multiplayer   );
	 m_tabCtrl.AddSSLPage ( GetLanguageData ( "Build Game", "Tab4" ), 0, &m_Build   );
	#endif

	m_Level.ShowWindow ( FALSE );
	m_Light.ShowWindow ( FALSE );
	m_Build.ShowWindow ( FALSE );
	m_Multiplayer.ShowWindow ( FALSE );

	g_bBuildGameInArenaMode = false;

	m_Message.Start ( _T ( "FPSBUILDGAME" ), 1024 * 6, 1, this );

	cIPC* pIPC = m_Message.GetIPC ( );
	DWORD dwReady = 1;
	pIPC->SendBuffer ( &dwReady, BUILD_GAME_SETUP, sizeof ( dwReady ) );

	m_bOk		= false;
	m_bCancel	= false;
	m_bCancel	= false;
	m_bHelp		= false;
	m_bClose	= false;
	m_bClean    = true;

	// restore buildgame from mass-disable of buildgameexecutable button
	m_Build.EnableAll ( );
	g_bBuildGameInProgress=false;
	m_tabCtrl.EnableWindow ( TRUE );
	m_Buttons [ 0 ].EnableWindow ( TRUE );

	return TRUE;
}

void cBuildGame::OnOK()
{

}

void cBuildGame::OnSize ( UINT nType, int cx, int cy ) 
{
	// for when dialog is resized

	// default function
	CDialog::OnSize ( nType, cx, cy );

	// check if tab control is valid
	if ( m_tabCtrl )
	{
		// set the window position and resize the dialog
		m_tabCtrl.SetWindowPos ( NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		m_tabCtrl.ResizeDialog ( 0, cx, cy );
	}
}

void cBuildGame::OnDestroy ( )
{
	// dialog is about to be destroyed

	// call default destruction
	CDialog::OnDestroy ( );
}
	
void cBuildGame::HandleMessages ( void )
{
	// temp flag
	bool bTerminateDialog = false;

	// check if ptrs valid
	cIPC* pIPC = m_Message.GetIPC ( );
	if ( !pIPC ) return;

	// A bit of EditorApp to handle FPSEXCHANGE activity here
	if ( theApp.m_Message.GetIPC()->SkipIfEventNotWaiting () )
	{
		theApp.UpdateFPIWizard();
		theApp.UpdateTermination();
		theApp.m_Message.GetIPC()->AllowSyncEventNow();
	}

	// skip if mapeditor not waiting for interface
	if ( !pIPC->SkipIfEventNotWaiting () )
		return;

	if ( m_bClean )
	{
		DWORD dwReady = 1;
		pIPC->SendBuffer ( &dwReady, BUILD_GAME_SETUP, sizeof ( dwReady ) );
		m_bClean = false;
	}

	// update the main dialog
	if ( OnUpdateDialog ( ) )
	{
		m_Project.Update ( pIPC );		// project
		m_Level.Update   ( pIPC );		// level
		m_Light.Update   ( pIPC );		// light
		
		m_Multiplayer.Update ( pIPC );	// multiplayer
		m_Build.Update   ( pIPC );		// build
	}
	
	DWORD dwClose = 0;
	pIPC->ReceiveBuffer ( &dwClose, BUILD_GAME_CLOSE, sizeof ( dwClose ) );
	if ( !dwClose ) m_Message.Restart ( );
	if ( dwClose )
	{
		DWORD dwZero = 0;
		pIPC->SendBuffer ( &dwZero, BUILD_GAME_SETUP, sizeof ( dwZero ) );
		pIPC->SendBuffer ( &dwZero, BUILD_GAME_CLOSE, sizeof ( dwZero ) );
		pIPC->SendBuffer ( &dwZero, BUILD_GAME_BUTTON_OK, sizeof ( dwZero ) );
		pIPC->SendBuffer ( &dwZero, BUILD_GAME_BUTTON_CANCEL, sizeof ( dwZero ) );
		pIPC->SendBuffer ( &dwZero, BUILD_GAME_BUTTON_HELP, sizeof ( dwZero ) );
		pIPC->SendBuffer ( &dwZero, BUILD_GAME_BUTTON_CLOSE, sizeof ( dwZero ) );

		m_Message.End ( );
		bTerminateDialog=true;
	}

	// handle enabling and disabling of TABcontrol/OK
	if ( g_bBuildGameInProgress==true )
	{
		m_tabCtrl.EnableWindow ( FALSE );
		m_Buttons [ 0 ].EnableWindow ( FALSE );
	}
	else
	{
		m_tabCtrl.EnableWindow ( TRUE );
		m_Buttons [ 0 ].EnableWindow ( TRUE );
	}

	// only when update completely done, signal that event complete
	pIPC->AllowSyncEventNow();

	// terminate at very end
	if ( bTerminateDialog )
	{
		CDialog::OnCancel ( );
		EndDialog ( 1 );
	}
}

int cBuildGame::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	// create the dialog

	// call the implementation
	if ( CDialog::OnCreate ( lpCreateStruct ) == -1 )
		return -1;
	
	// start the message functions
	//m_Message.Start ( _T ( "FPSBUILDGAME" ), 1024 * 6, 10, this );

	// all done
	return 0;
}

void cBuildGame::OnClose ( )
{
	// set to true to indicate the user has selected close button
	m_bClose = true;
}

bool cBuildGame::OnUpdateDialog ( void )
{
	// update the dialog

	cIPC* pIPC = m_Message.GetIPC ( );

	// local variables
	DWORD dwReady  = 1;												// ready status
	DWORD dwButton = 1;												// button value
	DWORD dwDisplayBrowse      = 0;
	DWORD dwBrowseFileSelected = 1;
	char			szTitleANSI     [ 255 ]	= "";
	char			szDirectoryANSI [ 255 ]	= "";
	char			szFilterANSI    [ 255 ]	= "";
	char			szFileANSI      [ 255 ]	= "";
	CString			szTitleUNI				= "";
	CString			szDirectoryUNI			= "";
	CString			szFilterUNI				= "";
	CString			szFileUNI				= "";
//	TCHAR			szFile [ 260 ];
//	OPENFILENAME	ofn;

	DWORD dwTitle = 0;
	DWORD dwClose = 0;
	DWORD dwTimer = 0;

	pIPC->ReceiveBuffer ( &dwTitle,			BUILD_GAME_SET_TITLE,	   sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwClose,			BUILD_GAME_CLOSE,		   sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwTimer,			BUILD_GAME_TIMER,		   sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwDisplayBrowse, BUILD_GAME_BROWSE_DISPLAY, sizeof ( DWORD ) );
	
	// see if we need to update the title
	if ( dwTitle )
	{
		// declare ansi and unicode strings
		char    szTextANSI [ 255 ] = "";
		CString szTextUNI;

		// copy string from file map into ansi
		//memcpy ( &szTextANSI [ 0 ], &m_pData [ STRING_A ], sizeof ( char ) * 255 );

		pIPC->ReceiveBuffer ( &szTextANSI [ 0 ], STRING_A, sizeof ( szTextANSI ) );
		
		// conversion from ansi to unicode
		szTextUNI = szTextANSI;

		// now set the window text
		SetWindowText ( szTextUNI );

		

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, BUILD_GAME_SET_TITLE, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ BUILD_GAME_SET_TITLE ], &dwValue, sizeof ( DWORD ) );
		/////////////
	}

	// set memory when ok button is pressed
	if ( m_bOk )
	{
		pIPC->SendBuffer ( &dwButton, BUILD_GAME_BUTTON_OK, sizeof ( dwButton ) );
	}

	// set memory when cancel button is pressed
	if ( m_bCancel )
	{
		pIPC->SendBuffer ( &dwButton, BUILD_GAME_BUTTON_CANCEL, sizeof ( dwButton ) );
	}

	// set memory when help button is pressed
	if ( m_bHelp )
	{
		pIPC->SendBuffer ( &dwButton, BUILD_GAME_BUTTON_HELP, sizeof ( dwButton ) );
	}

	// set memory when close button is pressed
	if ( m_bClose )
	{
		pIPC->SendBuffer ( &dwButton, BUILD_GAME_BUTTON_CLOSE, sizeof ( dwButton ) );
	}

	if ( m_bOk || m_bCancel || m_bClose )
	{
		DWORD dwReady = 0;
		pIPC->SendBuffer ( &dwReady, BUILD_GAME_SETUP, sizeof ( dwReady ) );
	}

	// update the timer speed
	if ( dwTimer )
	{
		// set the new speed
		m_Message.SetSpeed ( dwTimer );

		/////////////
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, BUILD_GAME_TIMER, sizeof ( dwValue ) );
	}

	return true;
}

void cBuildGame::OnBuildGameButtonOk ( ) 
{
	// okay button clicked
	m_bOk = true;
}

void cBuildGame::OnBuildGameButtonCancel ( )
{
	// cancel button clicked
	theApp.SetFileMapData ( 994, 1 );
	m_bCancel = true;
}

void cBuildGame::OnCancel()
{
	// dialog terminated by iother cancel methods
	theApp.SetFileMapData ( 994, 1 );
	m_bCancel = true;
}

void cBuildGame::OnBuildGameButtonHelp ( )
{
	OnCancel ( );
}

BOOL cBuildGame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog ::DestroyWindow();
}
