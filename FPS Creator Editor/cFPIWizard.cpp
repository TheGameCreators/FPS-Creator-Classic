// cFPIWizard.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cFPIWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cFPIWizard dialog


cFPIWizard::cFPIWizard(CWnd* pParent /*=NULL*/)
	: CDialog(cFPIWizard::IDD, pParent)
{
	//{{AFX_DATA_INIT(cFPIWizard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bOK     = false;
	m_bCancel = false;
	m_bPreview = false;
}


void cFPIWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cFPIWizard)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	

	DDX_Control ( pDX, IDCPREVIEW, m_Buttons [ 0 ] );
	DDX_Control ( pDX, IDOK, m_Buttons [ 1 ] );
	DDX_Control ( pDX, IDCANCEL, m_Buttons [ 2 ] );

	m_Buttons [  0 ].SetWindowText ( GetLanguageData ( "FPI Wizard", "Button1" ) );
	m_Buttons [  1 ].SetWindowText ( GetLanguageData ( "FPI Wizard", "Button2" ) );
	m_Buttons [  2 ].SetWindowText ( GetLanguageData ( "FPI Wizard", "Button3" ) );
}


BEGIN_MESSAGE_MAP(cFPIWizard, CDialog)
	//{{AFX_MSG_MAP(cFPIWizard)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCPREVIEW, OnPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cFPIWizard message handlers

int cFPIWizard::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	m_PropertyWindow.Create (dwViewStyle, rectDummy, this, 4);
	m_PropertyWindow.ShowApply ( false );

	m_Message.Start ( _T ( "FPSFPIWIZARD" ), 1024 * 4, 1, this );
	m_pIPC = m_Message.GetIPC();

	m_pProperty = new cProperty ( &m_PropertyWindow );
	
	return 0;
}

void cFPIWizard::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_PropertyWindow.SetWindowPos (NULL, -1, -1, cx, 300, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void cFPIWizard::HandleMessages ( void )
{
	// temp flag
	bool bTerminateDialog = false;

	// check if ptrs valid
	cIPC* pIPC = m_Message.GetIPC ( );
	if ( !pIPC ) return;

	// skip if mapeditor not waiting for interface
	if ( !pIPC->SkipIfEventNotWaiting () )
		return;

	DWORD dwReady	= 1;
	m_pIPC->ReceiveBuffer ( &dwReady, 0, sizeof ( DWORD ) );
	
	m_pProperty->HandleMessages ( m_pIPC, &m_Message );
	
	DWORD dwButton = 0;
	if ( m_bOK )
	{
		dwButton = 1;
		m_pIPC->SendBuffer ( &dwButton, 204, sizeof ( DWORD ) );
	}
	if ( m_bCancel )
	{
		dwButton = 1;
		m_pIPC->SendBuffer ( &dwButton, 208, sizeof ( DWORD ) );
	}
	if ( m_bPreview )
	{
		dwButton = 1;
		m_pIPC->SendBuffer ( &dwButton, 220, sizeof ( DWORD ) );
		m_bPreview = false;
	}
	
	DWORD dwClose = 0;
	m_pIPC->ReceiveBuffer ( &dwClose, 212, sizeof ( DWORD ) );
	if ( dwClose )
	{
		bTerminateDialog=true;
	}

	DWORD dwTitle  = 0;
	m_pIPC->ReceiveBuffer ( &dwTitle, 216, sizeof ( DWORD ) );
	if ( dwTitle )
	{
		// declare ansi and unicode strings
		char    szTextANSI [ 255 ] = "";
		CString szTextUNI;
		m_pIPC->ReceiveBuffer ( &szTextANSI [ 0 ], 1000, sizeof ( char ) * 255 );
		szTextUNI = szTextANSI;
		SetWindowText ( szTextUNI );

		DWORD dwValue = 0;
		m_pIPC->SendBuffer ( &dwValue, 216, sizeof ( DWORD ) );
	}

	// only when update completely done, signal that event complete
	pIPC->AllowSyncEventNow();

	// at very end
	if ( bTerminateDialog )
	{
		// Clear IPC and end dialog
		char pClear [ 1024 * 4 ] = { 0 };
		m_pIPC->SendBuffer ( pClear, 0, (1024 * 4) - 8 );
		EndDialog ( 1 );
	}
}

void cFPIWizard::OnOK() 
{
	// TODO: Add extra validation here

	m_bOK = true;
	
	//CDialog::OnOK();
}

void cFPIWizard::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	m_bCancel = true;
	
	//CDialog::OnClose();
}

void cFPIWizard::OnCancel() 
{
	// TODO: Add extra cleanup here

	m_bCancel = true;
	
	//CDialog::OnCancel();
}

void cFPIWizard::OnPreview() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = true;
}
