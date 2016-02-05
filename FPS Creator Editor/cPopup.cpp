// cPopup.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cPopup dialog


cPopup::cPopup(CWnd* pParent /*=NULL*/ ) : CDialog(cPopup::IDD, pParent)
{
	//{{AFX_DATA_INIT(cPopup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}


void cPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cPopup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	//DDX_Control ( pDX, IDC_EDIT1, m_Text );
	DDX_Control ( pDX, IDC_STATIC_POPUP, m_Text1 );
}


BEGIN_MESSAGE_MAP(cPopup, CDialog)
	//{{AFX_MSG_MAP(cPopup)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cPopup message handlers

BOOL cPopup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//m_Text.SetWindowText ( m_String );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int cPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

//	m_Message.Start ( _T ( "FPSPOPUP" ), 1024 * 2, 10, this );
	m_Message.Start ( _T ( "FPSPOPUP" ), 1024 * 2, 1, this );
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void cPopup::HandleMessages ( void )
{
	// A bit of EditorApp to handle FPSEXCHANGE activity here
	if ( theApp.m_Message.GetIPC()->SkipIfEventNotWaiting () )
	{
		// regular editor app handling (do not call UpdateRestart as it calls Messages backwards!)
		theApp.UpdateToolBar         ( );
		theApp.UpdateStatusBar       ( );
		theApp.UpdateLibraryWindow   ( );
		theApp.UpdateMessageBoxes    ( );
		theApp.UpdateProject         ( );
		theApp.UpdateFileOpenAndSave ( );
		theApp.UpdateTermination	 ( );

		// set event for FPSEXCHANGE waits
		theApp.m_Message.GetIPC()->AllowSyncEventNow();
	}

	// temp flag
	bool bTerminateDialog = false;

	// check if ptrs valid
	cIPC* pIPC = m_Message.GetIPC ( );
	if ( !pIPC ) return;

	// skip if mapeditor not waiting for interface
	if ( !pIPC->SkipIfEventNotWaiting () )
		return;

	// active
	DWORD dwReady = 1;
	pIPC->SendBuffer ( &dwReady, 0, sizeof ( DWORD ) );

	DWORD dwTitle  = 0;
	pIPC->ReceiveBuffer ( &dwTitle, 4, sizeof ( DWORD ) );
	if ( dwTitle )
	{
		// declare ansi and unicode strings
		char    szTextANSI [ 255 ] = "";
		CString szTextUNI;
		pIPC->ReceiveBuffer ( &szTextANSI [ 0 ], 1000, sizeof ( char ) * 255  );
		szTextUNI = szTextANSI;
		m_Text1.SetWindowText ( szTextUNI );

		// reset flag
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 4, sizeof ( DWORD ) );
	}

	DWORD dwClose	= 0;
	pIPC->ReceiveBuffer ( &dwClose, 8, sizeof ( DWORD ) );
	if ( dwClose )
	{
		/////////////
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 8, sizeof ( DWORD ) );
		bTerminateDialog=true;		
	}

	// only when update completely done, signal that event complete
	pIPC->AllowSyncEventNow();

	// at very end
	if ( bTerminateDialog )
	{
		// 110105
		theApp.m_bTestGameCancel = false;
		EndDialog ( 1 );
	}
}
