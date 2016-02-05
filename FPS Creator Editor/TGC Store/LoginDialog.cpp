// LoginDialog.cpp : implementation file
//

#include "stdafx.h"

#ifdef TGC_STORE

#include "LoginDialog.h"
#include "..\Editor.h"
#include "MarketWindow.h"

// CLoginDialog dialog

IMPLEMENT_DYNAMIC(CLoginDialog, CDialog)

CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDialog::IDD, pParent)
{
	szUsername = 0;
	szPassword = 0;
}

CLoginDialog::~CLoginDialog()
{
	if ( szUsername ) delete [] szUsername;
	if ( szPassword ) delete [] szPassword;
}

void CLoginDialog::SetUsername( const char* name )
{
	DWORD dwLength = strlen( name );
	
	if ( szUsername ) delete [] szUsername;
	szUsername = new char [ dwLength + 1 ];
	strcpy_s( szUsername, dwLength + 1, name );
	szUsername [ dwLength ] = '\0';
}

void CLoginDialog::SetPassword( const char* name )
{
	DWORD dwLength = strlen( name );
	
	if ( szPassword ) delete [] szPassword;
	szPassword = new char [ dwLength + 1 ];
	strcpy_s( szPassword, dwLength + 1, name );
	szPassword [ dwLength ] = '\0';
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERNAME, m_edtUsername);
	DDX_Control(pDX, IDC_PASSWORD, m_edtPassword);
	DDX_Control(pDX, IDC_LOGIN_REGISTER, m_txtRegister);
	DDX_Control(pDX, IDC_LOGIN_USERNAME, m_txtUsername);
	DDX_Control(pDX, IDC_LOGIN_PASSWORD, m_txtPassword);
	DDX_Control(pDX, IDC_LOGIN_STATEMENT, m_txtStatement);
	DDX_Control(pDX, IDC_LOGIN_STATEMENT2, m_txtSaveDetails);
}

BOOL CLoginDialog::OnInitDialog( )
{
	if ( !CDialog::OnInitDialog( ) ) return FALSE;

	SetWindowText( GetLanguageData( "TGC Store", "LoginDetails" ) );

	SetUsername( theApp.m_sUsername );
	m_edtUsername.SetWindowText( szUsername );

	SetPassword( theApp.m_sPassword );
	m_edtPassword.SetWindowText( szPassword );

	if ( strlen( szUsername ) > 0 ) SetTimer( 2, 500, NULL );
	else SetTimer( 1, 500, NULL );

	m_txtRegister.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Register") ) );
	m_txtUsername.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Email") ) );
	m_txtPassword.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Password") ) );
	m_txtStatement.SetWindowText( GetLanguageData( _T("TGC Store"), _T("LoginPrompt") ) );
	m_txtSaveDetails.SetWindowText( GetLanguageData( _T("TGC Store"), _T("SaveLogin") ) );

	m_edtPassword.SetPasswordChar( '*' );

	return TRUE;
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDialog::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED( IDC_LOGIN_REGISTER, OnRegister )
END_MESSAGE_MAP()


// CLoginDialog message handlers

void CLoginDialog::OnRegister( )
{
	//MessageBox( "Clicked" );
	m_pMarket->ShowRegisterPage();
	OnCancel();
}

void CLoginDialog::OnBnClickedOk()
{
	char str [ 256 ];

	m_edtUsername.GetWindowText( str, 256 );
	DWORD dwLength = (DWORD) strlen(str);
	szUsername = new char [ dwLength + 1 ];
	strcpy_s( szUsername, dwLength + 1, str );
	szUsername [ dwLength ] = '\0';

	m_edtPassword.GetWindowText( str, 256 );
	dwLength = (DWORD) strlen(str);
	szPassword = new char [ dwLength + 1 ];
	strcpy_s( szPassword, dwLength + 1, str );
	szPassword [ dwLength ] = '\0';

	OnOK();
}

void CLoginDialog::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		KillTimer( 1 );
		m_edtUsername.SetFocus( );
	}

	if ( nIDEvent == 2 )
	{
		KillTimer( 2 );
		m_edtPassword.SetFocus( );
	}
}

#endif