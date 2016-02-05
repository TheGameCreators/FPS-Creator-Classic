// WorkspaceBar.cpp : implementation of the CWorkspaceBar class
//

#include "stdafx.h"
#include "Editor.h"
#include "WorkspaceBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// offset positions in file map for messages
#define ENTITY_SETUP							 0
#define ENTITY_WINDOW_TITLE						 4


#define	STRING_A								1000
#define	STRING_B								1256
#define	STRING_C								1512
#define	STRING_D								1768


/////////////////////////////////////////////////////////////////////////////

#define ID_BUTTON_APPLY1		1000
#define ID_BUTTON_CANCEL1		1001

// CWorkspaceBar

BEGIN_MESSAGE_MAP(CWorkspaceBar, CBCGSizingControlBar)
	//{{AFX_MSG_MAP(CWorkspaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED ( IDOK,	 OnButtonApply )
	ON_BN_CLICKED ( IDCANCEL,	 OnButtonCancel )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar construction/destruction

CWorkspaceBar::CWorkspaceBar()
{
	// TODO: add one-time construction code here

	//m_pMessageData = NULL;

	m_bClose = false;
}

CWorkspaceBar::~CWorkspaceBar()
{
	//End ( );

	//theApp.m_bFocus = false;
}


/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

void CWorkspaceBar::HandleMessages ( void )
{
	m_pIPC = m_Message.GetIPC ( );

	m_Message.Stop ( );
	
	DWORD dwReady	= 1;
	m_pIPC->SendBuffer ( &dwReady, ENTITY_SETUP, sizeof ( DWORD ) );
	
	UpdateWindow		( );

	m_pProperty->HandleMessages ( m_pIPC, &m_Message );

	if ( m_bClose )
	{
		DWORD dwClose = 1;
		m_bClose = false;
		m_pIPC->SendBuffer ( &dwClose, 108, sizeof ( DWORD ) );
	}

	m_Message.Restart ( );
}

BOOL CWorkspaceBar::QueryClose (void)
{
	m_bClose = true;

	

	return FALSE;

	return CWorkspaceBar::QueryClose();
}


void CWorkspaceBar::UpdateWindow ( void )
{
	DWORD		dwSetWindowTitle	      = 999;
	char		szWindowTitleANSI [ 255 ] = "";
	CString		szWindowTitleUNI;

	m_pIPC->ReceiveBuffer ( &dwSetWindowTitle, ENTITY_WINDOW_TITLE, sizeof ( DWORD ) );

	//memcpy ( &dwSetWindowTitle,	&m_pMessageData [ ENTITY_WINDOW_TITLE ], sizeof ( DWORD ) );
	
	if ( dwSetWindowTitle == 1 )
	{
		m_pIPC->ReceiveBuffer ( &szWindowTitleANSI [ 0 ], STRING_A, sizeof ( char ) * 255 );

		//memcpy ( &szWindowTitleANSI [ 0 ], &m_pMessageData [ STRING_A ], sizeof ( char ) * 255 );
		szWindowTitleUNI = szWindowTitleANSI;

		SetWindowText ( szWindowTitleUNI );

		memset ( &szWindowTitleANSI [        0 ], 0,                        sizeof ( szWindowTitleANSI ) );

		m_pIPC->SendBuffer ( &szWindowTitleANSI [ 0 ], STRING_A, sizeof ( char ) * 255 );
		//memcpy ( &m_pMessageData    [ STRING_A ], &szWindowTitleANSI [ 0 ], sizeof ( szWindowTitleANSI ) );

		/////////////
		DWORD dwValue = 0;
		m_pIPC->SendBuffer ( &dwValue, ENTITY_WINDOW_TITLE, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_WINDOW_TITLE ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pMessageData [ ENTITY_WINDOW_TITLE ] = 0;
	}
}
int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Start ( _T ( "FPSENTITY" ), 1024 * 2, 10 );

	//m_Message.Start ( _T ( "FPSENTITY" ), 1024 * 6, 10, this );
	m_Message.Start ( _T ( "FPSENTITY" ), 1024 * 6, 1, this );
	

	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	SetWindowText ( _T("") );

	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	m_Property.Create (dwViewStyle, rectDummy, this, 4);

	//theApp.m_bFocus = true;

	m_pProperty = new cProperty ( &m_Property );

	m_bValid = true;

	return 0;
}


void CWorkspaceBar::OnButtonApply ( )
{
	m_bApplyButton = true;
}

void CWorkspaceBar::OnButtonCancel ( )
{
	m_bCancelButton = true;
}

void CWorkspaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGSizingControlBar::OnSize(nType, cx, cy);

	m_Property.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}


void CWorkspaceBar::OnKillFocus(CWnd* pNewWnd) 
{
	CBCGSizingControlBar::OnKillFocus(pNewWnd);
}

void CWorkspaceBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGSizingControlBar::OnSetFocus(pOldWnd);
}

void CWorkspaceBar::OnClose() 
{
	CBCGSizingControlBar::OnClose();
}

void CWorkspaceBar::OnDestroy() 
{
	CBCGSizingControlBar::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CWorkspaceBar::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBCGSizingControlBar::OnShowWindow(bShow, nStatus);

}

void CWorkspaceBar::SetVisible ( BOOL bState )
{
	if ( bState )
	{
		theApp.m_bEntityFocus = true;
		ShowWindow ( 1 );
	}
	else
	{
		ShowWindow ( 0 );
		theApp.m_bEntityFocus = false;

// moved further up in the trigger to close entity properties panel
//		// the proper way - let it cancel itself
//		m_Property.m_bCancelButton = true;
//		m_Property.Clear ( );
		if ( m_pProperty ) m_pProperty->ResetAll ();
	}
}
