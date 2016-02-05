// cTestGame.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cTestGame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cTestGame::cTestGame ( CWnd* pParent ) : CDialog ( cTestGame::IDD, pParent )
{
	//{{AFX_DATA_INIT(cTestGame)
	//}}AFX_DATA_INIT

	m_pMessageData = NULL;
}

void cTestGame::DoDataExchange ( CDataExchange* pDX )
{
	CDialog::DoDataExchange ( pDX );
	//{{AFX_DATA_MAP(cTestGame)
	//}}AFX_DATA_MAP

	DDX_Control ( pDX, IDC_EDIT_INFORMATION, m_Information	);
	DDX_Control ( pDX, IDC_EDIT_STATUS,	 	 m_Status		);
	DDX_Control ( pDX, IDC_PROGRESS,		 m_Progress		);
	DDX_Control ( pDX, IDOK,				 m_Okay			);
	DDX_Control ( pDX, IDCANCEL,			 m_Cancel		);

	// FPGC - 090909 - hide OKAY button altogether, and center CANCEL button
	//m_Okay.EnableWindow ( FALSE );
	m_Okay.ShowWindow ( FALSE );
	m_Cancel.SetWindowPos(NULL, 185, 140, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING );

	//CString a = GetLanguageData ( "Test Game", "Title" );
	//CString b = GetLanguageData ( "Test Game", "Button1" );
	//CString c = GetLanguageData ( "Test Game", "Button2" );

	SetWindowText ( GetLanguageData ( "Test Game", "Title" ) );
	m_Okay.SetWindowText ( GetLanguageData ( "Test Game", "Button1" ) );
	m_Cancel.SetWindowText ( GetLanguageData ( "Test Game", "Button2" ) );
}

BEGIN_MESSAGE_MAP ( cTestGame, CDialog )
	//{{AFX_MSG_MAP(cTestGame)
	ON_WM_CREATE ( )
	ON_WM_MOVE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL cTestGame::OnInitDialog ( )
{
	CDialog::OnInitDialog ( );
	
	m_Progress.SetRange ( 0, 100 );
	m_Progress.SetStep  ( 1 );
	m_Progress.SetPos   ( 0 );
	
	m_bOkay = false;

	m_bClean = true;

	m_TestGameDebug.Start ( "testgame.log" );
	
	return TRUE;
}

void cTestGame::HandleMessages ( void )
{
	// A bit of EditorApp to handle FPSEXCHANGE activity here
	if ( theApp.m_Message.GetIPC()->SkipIfEventNotWaiting () )
	{
		// regular editor app handling (do not call UpdateRestart as it calls Messages backwards!)
		theApp.UpdateTermination();

		// if cancel pressed, terminate dialog
		DWORD dwCheck = 0;
		theApp.m_Message.GetIPC()->ReceiveBuffer ( &dwCheck, 994, sizeof ( dwCheck ) );
		if ( dwCheck == 2 )
		{
			// this kills the dialog (done after testgame dialog CANCELLED)
			CDialog ::OnCancel();
			EndDialog ( 1 );
			SetFocus ( );
		}

		// set event for FPSEXCHANGE waits
		theApp.m_Message.GetIPC()->AllowSyncEventNow();
	}

	// check if ptrs valid
	cIPC* pIPC = m_Message.GetIPC ( );
	if ( !pIPC ) return;

	// skip if mapeditor not waiting for interface
	if ( !pIPC->SkipIfEventNotWaiting () )
		return;

	char  szDebug [ MAX_PATH ] = "";

	// clean
	if ( m_bClean )
	{
		BYTE data [ 1024 * 2 ] = { 0 };
		pIPC->SendBuffer ( &data, 0, sizeof ( data ) );
		m_bClean = false;
	}

	// testgame dialog active
	DWORD dwReady = 1;
	pIPC->SendBuffer ( &dwReady, 0, sizeof ( dwReady ) );

	// set title
	DWORD dwTitle  = 0;
	pIPC->ReceiveBuffer ( &dwTitle, 20, sizeof ( dwTitle ) );
	if ( dwTitle )
	{
		// declare ansi and unicode strings
		char    szTextANSI [ 255 ] = "";
		CString szTextUNI;

		pIPC->ReceiveBuffer ( &szTextANSI [ 0 ], 1000, sizeof ( szTextANSI ) );
		szTextUNI = szTextANSI;
		sprintf ( szDebug, "cTestGame::HandleMessages - SetWindowTitle - %s", szTextANSI );
		m_TestGameDebug.Write ( szDebug );

		// now set the window text
		SetWindowText ( szTextUNI );

		// reset flag
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 20, sizeof ( dwValue ) );
	}

	// set text
	pIPC->ReceiveBuffer ( &dwTitle, 24, sizeof ( dwTitle ) );
	if ( dwTitle )
	{
		// declare ansi and unicode strings
		char    szTextANSI [ 255 ] = "";
		CString szTextUNI;

		pIPC->ReceiveBuffer ( &szTextANSI [ 0 ], 1000, sizeof ( szTextANSI ) );
		szTextUNI = szTextANSI;
		sprintf ( szDebug, "cTestGame::HandleMessages - SetCancelButton- %s", szTextANSI );
		m_TestGameDebug.Write ( szDebug );

		// now set the window text
		m_Cancel.SetWindowText ( szTextUNI );

		// reset flag
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 24, sizeof ( dwValue ) );
	}	

	// set text
	DWORD dwText = 0;
	pIPC->ReceiveBuffer ( &dwText, 4, sizeof ( dwText ) );
	if ( dwText )
	{
		char    szTextANSI [ 255 ] = "";
		CString szTextUNI;

		pIPC->ReceiveBuffer ( &szTextANSI [ 0 ], 1000, sizeof ( szTextANSI ) );
		szTextUNI = szTextANSI;
		sprintf ( szDebug, "cTestGame::HandleMessages - SetWindowText - %s", szTextANSI );
		m_TestGameDebug.Write ( szDebug );
		m_Information.SetWindowText ( szTextUNI );

		// reset flag
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 4, sizeof ( dwValue ) );
	}

	// get status
	DWORD dwStatus = 0;
	pIPC->ReceiveBuffer ( &dwStatus, 12, sizeof ( dwStatus ) );
	if ( dwStatus )
	{
		char    szTextANSI [ 255 ] = "";
		CString szTextUNI;

		pIPC->ReceiveBuffer ( &szTextANSI [ 0 ], 1000, sizeof ( szTextANSI ) );
		szTextUNI = szTextANSI;
		sprintf ( szDebug, "cTestGame::HandleMessages - SetStatusText - %s", szTextANSI );
		m_TestGameDebug.Write ( szDebug );
		m_Status.SetWindowText ( szTextUNI );

		// reset flag
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 12, sizeof ( dwValue ) );
	}
	
	// get progress position
	DWORD dwPosition = 1;
	pIPC->ReceiveBuffer ( &dwPosition, 8, sizeof ( dwPosition ) );
	if ( dwPosition < 100 )
	{
		sprintf ( szDebug, "cTestGame::HandleMessages - StatusBarPosition - %i", dwPosition );
		m_TestGameDebug.Write ( szDebug );
		m_Progress.SetPos ( dwPosition );
	}
	if ( dwPosition >= 100 )
	{
		// enable OK button when test game building complete
		if ( 1 )
		{
			// FPGC - 090909 - new way auto-runs issuing OK code manually (and keeping game focus)
			theApp.SetFileMapData ( 974, 1 );
			EndDialog ( 1 );
		}
		else
		{
			// Old way enabled OK button to user can click (but it steals focus)
			m_Okay.EnableWindow ( TRUE );
		}
	}

	// clicked okey button
	if ( m_bOkay )
	{
		// ??
		DWORD dwPosition = 1;
		pIPC->SendBuffer ( &dwPosition, 16, sizeof ( dwPosition ) );
	}

	// only when update completely done, signal that event complete
	pIPC->AllowSyncEventNow();
}

int cTestGame::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	if ( CDialog::OnCreate ( lpCreateStruct ) == -1 )
		return -1;

//	m_Message.Start ( _T ( "FPSTESTGAMEDIALOG" ), 1024 * 2, 10, this );
	m_Message.Start ( _T ( "FPSTESTGAMEDIALOG" ), 1024 * 2, 1, this );
	
	return 0;
}

void cTestGame::OnOK ( )
{
	// TEST GAME Starts Here
	theApp.SetFileMapData ( 974, 1 );

	// One more time to set the event (for above setting)
	// otherwise the testgame will not by allowed to leave WIAT command)
	HandleMessages();

	// Leave testgame dialog
	SetFocus ( );
	EndDialog ( 1 );
}

void cTestGame::OnCancel() 
{
	// TEST GAME Cancel (the testgame sets 994 to 2, which triggers enddialog later)
	theApp.SetFileMapData ( 994, 1 );
}


void cTestGame::OnMove(int x, int y) 
{
	//CDialog ::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}

BOOL cTestGame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog ::OnMouseWheel(nFlags, zDelta, pt);
}
