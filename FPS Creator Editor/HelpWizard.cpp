// HelpWizard.cpp : implementation file
//

#include "stdafx.h"
#include "HelpWizard.h"
#include "Editor.h"


// CHelpWizard dialog

IMPLEMENT_DYNAMIC(CHelpWizard, CDialog)

CHelpWizard::CHelpWizard(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpWizard::IDD, pParent)
{
	m_bInit = false;
}

CHelpWizard::~CHelpWizard()
{
	theApp.m_bHelpWizardVisible = false;
	theApp.m_pHelpWizardDlg = NULL;
}

BOOL CHelpWizard::OnInitDialog()
{
	if ( !CDialog::OnInitDialog() ) return FALSE;

	m_btnClose.SetWindowText( GetLanguageData( _T("Dialog Buttons"), _T("Close") ) );
	
	//OnSize( 0, rect.Width(), rect.Height() );

	m_Video.Create( IDD_DIALOG2, this );
	m_Video.ShowWindow( SW_SHOW );

	m_bInit = true;

	CRect rectClient;
	GetClientRect( &rectClient );

	rectClient.top += 5;
	rectClient.left = 5;
	rectClient.right -= 5;
	rectClient.bottom -= 35;
	
	m_Video.SetWindowPos( &wndTop, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_SHOWWINDOW );

	return TRUE;
}

void CHelpWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnClose);
}

unsigned CHelpWizard::Run( )
{
	theApp.m_bHelpWizardVisible = true;
	DoModal( );
	theApp.m_bHelpWizardVisible = false;

	DWORD dwValue = 0;
	if ( !theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "check_x9_updates_on_startup", &dwValue ) )
	{
		int result = MessageBox( GetLanguageData( _T("Getting Started"), _T("UpdateCheck") ), GetLanguageData( _T("Auto Update"), _T("AutoUpdate") ), MB_YESNO );
		if ( result == IDYES ) dwValue = 1;
		else dwValue = 0;

		theApp.SetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "check_x9_updates_on_startup", dwValue );
	}

	return 0;
}


BEGIN_MESSAGE_MAP(CHelpWizard, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHelpWizard message handlers

void CHelpWizard::OnOK( )
{
	m_Video.OnClosing();
	CDialog::OnOK( );
}

void CHelpWizard::OnCancel( )
{
	m_Video.OnClosing();
	CDialog::OnCancel( );
}

void CHelpWizard::OnSize( UINT nType, int cx, int cy )
{
	if ( !m_bInit ) return;

	CDialog::OnSize( nType, cx, cy );

	CRect btnRect;
	m_btnClose.GetClientRect( &btnRect );
	m_btnClose.SetWindowPos( NULL, cx - btnRect.Width() - 7, cy - btnRect.Height() - 3, 0,0, SWP_NOSIZE );

	CRect rectClient;
	GetClientRect( &rectClient );

	rectClient.top += 5;
	rectClient.left = 5;
	rectClient.right -= 5;
	rectClient.bottom -= 35;
	
	m_Video.SetWindowPos( &wndTop, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_SHOWWINDOW );
}
