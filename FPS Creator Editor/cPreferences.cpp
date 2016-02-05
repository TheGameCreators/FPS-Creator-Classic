// cPreferences.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Externals
extern bool g_bFreeVersion;


/////////////////////////////////////////////////////////////////////////////
// cPreferences dialog

cPreferences::cPreferences(CWnd* pParent /*=NULL*/)
	: CDialog(cPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(cPreferences)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bOK     = false;
	m_bCancel = false;
	m_bStart  = false;
}


void cPreferences::DoDataExchange(CDataExchange* pDX)
{


	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cPreferences)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Control ( pDX, IDC_DYNAMIC_LIGHTING,		m_DynamicLighting );
	DDX_Control ( pDX, IDC_DYNAMIC_SHADOWS,			m_DynamicShadows );
	DDX_Control ( pDX, IDC_NO_LIGHT_MAPPING,		m_NoLightMapping );
	DDX_Control ( pDX, IDC_QUICK_LIGHT_MAPPING,		m_QuickLightMapping );
	DDX_Control ( pDX, IDC_FULL_LIGHT_MAPPING,		m_FullLightMapping );
	DDX_Control ( pDX, IDC_NO_EFFECTS,				m_NoEffects );
	DDX_Control ( pDX, IDC_ESSENTIAL_EFFECTS,		m_EssentialEffects );
	DDX_Control ( pDX, IDC_FULL_EFFECTS,			m_FullEffects );
	DDX_Control ( pDX, IDC_LOW,						m_Low );
	DDX_Control ( pDX, IDC_MEDIUM,					m_Medium );
	DDX_Control ( pDX, IDC_HIGH,					m_High );
	DDX_Control ( pDX, IDC_HSR,						m_HSR );
	DDX_Control ( pDX, IDC_NO_FLOOR,				m_NoFloor );
	DDX_Control ( pDX, IDC_SHOW_ENTITY,				m_ShowEntity );
	DDX_Control ( pDX, IDC_SHOW_DEBUG,				m_ShowDebug );

	DDX_Control ( pDX, IDC_TEST_BOX1, m_Lines [ 0 ] );
	DDX_Control ( pDX, IDC_TEST_BOX2, m_Lines [ 1 ] );
	DDX_Control ( pDX, IDC_TEST_BOX3, m_Lines [ 2 ] );
	DDX_Control ( pDX, IDC_TEST_BOX4, m_Lines [ 3 ] );
	DDX_Control ( pDX, IDC_TEST_BOX5, m_Lines [ 4 ] );

	DDX_Control ( pDX, IDOK,     m_Buttons [ 0 ] );
	DDX_Control ( pDX, IDCANCEL, m_Buttons [ 1 ] );

	DDX_Control ( pDX, IDC_TEST_BOX6, m_Lines [ 5 ] );
	DDX_Control ( pDX, IDC_SINGLE_PLAYER, m_SinglePlayer);
	DDX_Control ( pDX, IDC_MULTIPLAYER,	  m_MultiPlayer );

#ifdef TGC_STORE
	//	DDX_Control(pDX, IDC_ONE_CLICK, m_btnOneClick);
	DDX_Control(pDX, IDC_AUTO_LOGIN, m_chkSaveUsername);
	DDX_Control(pDX, IDC_CHECK4, m_chkSavePassword);
	DDX_Control(pDX, IDC_USERNAME, m_edtUsername);
	DDX_Control(pDX, IDC_PASSWORD, m_edtPassword);
	DDX_Control(pDX, IDC_CHECK3, m_chkShowPurchased);
	DDX_Control(pDX, IDC_TEXT_USERNAME, m_txtUsername);
	DDX_Control(pDX, IDC_TEXT_PASSWORD, m_txtPassword);
#endif



	SetWindowText ( GetLanguageData ( "Preferences", "Title" ) );
	m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Preferences", "Box1" ) );
	m_Lines [ 1 ].SetWindowText ( GetLanguageData ( "Preferences", "Box2" ) );
	m_Lines [ 2 ].SetWindowText ( GetLanguageData ( "Preferences", "Box3" ) );
	m_Lines [ 3 ].SetWindowText ( GetLanguageData ( "Preferences", "Box4" ) );
	m_Lines [ 4 ].SetWindowText ( GetLanguageData ( "Preferences", "Box5" ) );

	m_DynamicLighting.SetWindowText		( GetLanguageData ( "Preferences", "Item1" ) );
	m_NoLightMapping.SetWindowText		( GetLanguageData ( "Preferences", "Item2" ) );
	m_QuickLightMapping.SetWindowText	( GetLanguageData ( "Preferences", "Item3" ) );
	m_FullLightMapping.SetWindowText	( GetLanguageData ( "Preferences", "Item4" ) );
	m_NoEffects.SetWindowText			( GetLanguageData ( "Preferences", "Item5" ) );
	m_EssentialEffects.SetWindowText	( GetLanguageData ( "Preferences", "Item6" ) );
	m_Low.SetWindowText					( GetLanguageData ( "Preferences", "Item7" ) );
	m_Medium.SetWindowText				( GetLanguageData ( "Preferences", "Item8" ) );
	m_High.SetWindowText				( GetLanguageData ( "Preferences", "Item9" ) );
	m_NoFloor.SetWindowText				( GetLanguageData ( "Preferences", "Item10" ) );
	m_ShowEntity.SetWindowText			( GetLanguageData ( "Preferences", "Item11" ) );
	m_ShowDebug.SetWindowText			( GetLanguageData ( "Preferences", "Item12" ) );

	m_Lines [ 5 ].SetWindowText			( GetLanguageData ( "Preferences", "Box6" ) );
	m_SinglePlayer.SetWindowText		( GetLanguageData ( "Preferences", "Item13" ) );
	m_MultiPlayer.SetWindowText			( GetLanguageData ( "Preferences", "Item14" ) );
	m_Buttons [ 0 ].SetWindowText		( GetLanguageData ( "Preferences", "Button1" ) );
	m_Buttons [ 1 ].SetWindowText		( GetLanguageData ( "Preferences", "Button2" ) );

	// FREE VERSION
	if ( g_bFreeVersion )
	{
		m_Lines [ 5 ].ShowWindow ( SW_HIDE );
		m_SinglePlayer.ShowWindow ( SW_HIDE );
		m_MultiPlayer.ShowWindow ( SW_HIDE );
		m_Lines [ 1 ].ShowWindow ( SW_HIDE );
		m_NoEffects.ShowWindow ( SW_HIDE );
		m_EssentialEffects.ShowWindow ( SW_HIDE );
	}
	else
	{
		m_Lines [ 5 ].ShowWindow ( SW_SHOW );
		m_SinglePlayer.ShowWindow ( SW_SHOW );
		m_MultiPlayer.ShowWindow ( SW_SHOW );
		m_Lines [ 1 ].ShowWindow ( SW_SHOW );
		m_NoEffects.ShowWindow ( SW_SHOW );
		m_EssentialEffects.ShowWindow ( SW_SHOW );
	}
}


BEGIN_MESSAGE_MAP(cPreferences, CDialog)
	//{{AFX_MSG_MAP(cPreferences)
	ON_BN_CLICKED(IDC_DYNAMIC_LIGHTING, OnDynamicLighting)
	ON_BN_CLICKED(IDC_DYNAMIC_SHADOWS, OnDynamicShadows)
	ON_BN_CLICKED(IDC_NO_LIGHT_MAPPING, OnNoLightMapping)
	ON_BN_CLICKED(IDC_SHOW_DEBUG, OnShowDebug)
	ON_BN_CLICKED(IDC_FULL_LIGHT_MAPPING, OnFullLightMapping)
	ON_BN_CLICKED(IDC_NO_EFFECTS, OnNoEffects)
	ON_BN_CLICKED(IDC_ESSENTIAL_EFFECTS, OnEssentialEffects)
	ON_BN_CLICKED(IDC_FULL_EFFECTS, OnFullEffects)
	ON_BN_CLICKED(IDC_LOW, OnLow)
	ON_BN_CLICKED(IDC_MEDIUM, OnMedium)
	ON_BN_CLICKED(IDC_HIGH, OnHigh)
	ON_BN_CLICKED(IDC_HSR, OnHsr)
	ON_BN_CLICKED(IDC_SHOW_ENTITY, OnShowEntity)
	ON_BN_CLICKED(IDC_QUICK_LIGHT_MAPPING, OnQuickLightMapping)
	ON_BN_CLICKED(IDC_NO_FLOOR, OnNoFloor)
	ON_BN_CLICKED(IDC_SINGLE_PLAYER, OnSinglePlayer)
	ON_BN_CLICKED(IDC_MULTIPLAYER, OnMultiplayer)
	//}}AFX_MSG_MAP

#ifdef TGC_STORE
	ON_BN_CLICKED(IDC_AUTO_LOGIN, &cPreferences::OnSaveUsernameClicked)
	ON_BN_CLICKED(IDC_CHECK4, &cPreferences::OnSavePasswordClicked)
	ON_BN_CLICKED(IDC_CHECK3, &cPreferences::OnShowPurchasedClicked)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cPreferences message handlers

void cPreferences::HandleMessages ( void )
{
	// temp flag
	bool bTerminateDialog = false;

	// check if ptrs valid
	cIPC* pIPC = m_Message.GetIPC ( );
	if ( !pIPC ) return;

	// flag that prefs activity started
	DWORD dwReady = 1;
	pIPC->SendBuffer ( &dwReady, 0, sizeof ( DWORD ) );

	// skip if mapeditor not waiting for interface
	if ( !pIPC->SkipIfEventNotWaiting () )
		return;

	m_pIPC = pIPC;
	m_Controls.Update ( m_pIPC );

	DWORD dwButton = 0;
	if ( m_bOK )
	{
		dwButton = 1;
		pIPC->SendBuffer ( &dwButton, 4, sizeof ( DWORD ) );
	}
	if ( m_bCancel )
	{
		dwButton = 1;
		pIPC->SendBuffer ( &dwButton, 8, sizeof ( DWORD ) );
	}

	DWORD dwClose = 0;
	pIPC->ReceiveBuffer ( &dwClose, 12, sizeof ( DWORD ) );
	if ( dwClose )
	{
		dwClose = 0;
		pIPC->SendBuffer ( &dwClose, 12, sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwClose, 4, sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwClose, 8, sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwClose, 0, sizeof ( DWORD ) );
		bTerminateDialog=true;
	}

	// only when update completely done, signal that event complete
	pIPC->AllowSyncEventNow();

	// only at end
	if ( bTerminateDialog )
	{
		CDialog::OnCancel ( );
		EndDialog ( 1 );
	}
}

BOOL cPreferences::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Controls.AddRadioButton ( &m_DynamicLighting,		1  );
	m_Controls.AddRadioButton ( &m_DynamicShadows,		2  );
	m_Controls.AddRadioButton ( &m_NoLightMapping,		3  );
	m_Controls.AddRadioButton ( &m_QuickLightMapping,	4  );
	m_Controls.AddRadioButton ( &m_FullLightMapping,	5  );
	m_Controls.AddRadioButton ( &m_NoEffects,			6  );
	m_Controls.AddRadioButton ( &m_EssentialEffects,	7  );
	m_Controls.AddRadioButton ( &m_FullEffects,			8  );
	m_Controls.AddRadioButton ( &m_Low,					9  );
	m_Controls.AddRadioButton ( &m_Medium,				10 );
	m_Controls.AddRadioButton ( &m_High,				11 );
	m_Controls.AddRadioButton ( &m_HSR,					12 );
	m_Controls.AddRadioButton ( &m_NoFloor,				15 );
	m_Controls.AddRadioButton ( &m_ShowEntity,			13 );
	m_Controls.AddRadioButton ( &m_ShowDebug,			14 );

	m_Controls.AddRadioButton ( &m_SinglePlayer,		16 );
	m_Controls.AddRadioButton ( &m_MultiPlayer,			17 );

	// start IPC
	m_Message.Start ( _T ( "FPSPREFERENCES" ), 1024, 1, this );

#ifdef TGC_STORE
		
	m_txtUsername.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Email") ) );
	m_txtPassword.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Password") ) );
	m_chkShowPurchased.SetWindowText( GetLanguageData( _T("TGC Store"), _T("DisplayPurchased") ) );
	m_chkSavePassword.SetWindowText( GetLanguageData( _T("TGC Store"), _T("SavePassword") ) );
	m_chkSaveUsername.SetWindowText( GetLanguageData( _T("TGC Store"), _T("SaveUsername") ) );

	//m_edtUsername.SetWindowText( theApp.m_sUsername );
	//m_edtPassword.SetWindowText( theApp.m_sPassword );

	DWORD dwValue = 1;
	if ( !theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "always_display_purchased", &dwValue ) ) dwValue = 1;

	m_chkShowPurchased.SetCheck( dwValue ? 1 : 0 );

	//check saved username
	char szUsername [ 256 ] = "";
	if ( theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "username", szUsername, 256 ) )
	{
		m_edtUsername.SetWindowText( szUsername );
		m_edtUsername.EnableWindow( TRUE );
		m_chkSaveUsername.SetCheck( 1 );
	}
	else
	{
		m_edtUsername.SetWindowText( "" );
		m_edtUsername.EnableWindow( FALSE );
		m_chkSaveUsername.SetCheck( 0 );
	}


	char szPassword [ 256 ] = "";
	if ( theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "password", szPassword, 256 ) )
	{
		char *szDecrypt1 = theApp.Decrypt( szPassword );
		char *szDecrypt2 = theApp.Decrypt( szDecrypt1 );

		m_edtPassword.SetWindowText( szDecrypt2 );
		m_edtPassword.EnableWindow( TRUE );
		m_chkSavePassword.SetCheck( 1 );

		delete [] szDecrypt1;
		delete [] szDecrypt2;
	}
	else
	{
		m_edtPassword.SetWindowText( "" );
		m_edtPassword.EnableWindow( FALSE );
		m_chkSavePassword.SetCheck( 0 );
	}

	m_edtPassword.SetPasswordChar( '*' );
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cPreferences::OnDynamicLighting() 
{
	if ( m_Controls.m_bRadioClicked [ 1 ] )
	{
		m_Controls.SetRadio ( 1, false );
	}
	else
	{
		m_Controls.SetRadio ( 1, true );
	}
}

void cPreferences::OnDynamicShadows() 
{
	if ( m_Controls.m_bRadioClicked [ 2 ] )
	{
		m_Controls.SetRadio ( 2, false );
	}
	else
	{
		m_Controls.SetRadio ( 2, true );
	}
}

void cPreferences::OnNoLightMapping() 
{
	m_NoLightMapping.SetCheck    ( TRUE );
	m_QuickLightMapping.SetCheck ( FALSE );
	m_FullLightMapping.SetCheck  ( FALSE );

	m_Controls.SetRadio ( 3, true  );
	m_Controls.SetRadio ( 4, false );
	m_Controls.SetRadio ( 5, false );
}

void cPreferences::OnShowDebug() 
{
	if ( m_Controls.m_bRadioClicked [ 14 ] )
	{
		m_Controls.SetRadio ( 14, false );
	}
	else
	{
		m_Controls.SetRadio ( 14, true );
	}
	
}

void cPreferences::OnFullLightMapping() 
{
	m_NoLightMapping.SetCheck    ( FALSE );
	m_QuickLightMapping.SetCheck ( FALSE );
	m_FullLightMapping.SetCheck  ( TRUE );

	m_Controls.SetRadio ( 3, false  );
	m_Controls.SetRadio ( 4, false );
	m_Controls.SetRadio ( 5, true );
	
}

void cPreferences::OnNoEffects() 
{
	m_NoEffects.SetCheck       ( TRUE );
	m_EssentialEffects.SetCheck ( FALSE );
	m_FullEffects.SetCheck      ( FALSE );

	m_Controls.SetRadio ( 6, true  );
	m_Controls.SetRadio ( 7, false );
	m_Controls.SetRadio ( 8, false );
	
}

void cPreferences::OnEssentialEffects() 
{
	m_NoEffects.SetCheck       ( FALSE );
	m_EssentialEffects.SetCheck ( TRUE );
	m_FullEffects.SetCheck      ( FALSE );

	m_Controls.SetRadio ( 6, false  );
	m_Controls.SetRadio ( 7, true );
	m_Controls.SetRadio ( 8, false );
}

void cPreferences::OnFullEffects() 
{
	m_NoEffects.SetCheck       ( FALSE );
	m_EssentialEffects.SetCheck ( FALSE );
	m_FullEffects.SetCheck      ( TRUE );

	m_Controls.SetRadio ( 6, false  );
	m_Controls.SetRadio ( 7, false );
	m_Controls.SetRadio ( 8, true );
}

void cPreferences::OnLow() 
{
	m_Low.SetCheck       ( TRUE );
	m_Medium.SetCheck ( FALSE );
	m_High.SetCheck      ( FALSE );

	m_Controls.SetRadio ( 9, true  );
	m_Controls.SetRadio ( 10, false );
	m_Controls.SetRadio ( 11, false );
	
	
}

void cPreferences::OnMedium() 
{
	m_Low.SetCheck       ( FALSE );
	m_Medium.SetCheck ( TRUE );
	m_High.SetCheck      ( FALSE );

	m_Controls.SetRadio ( 9, false  );
	m_Controls.SetRadio ( 10, true );
	m_Controls.SetRadio ( 11, false );
}

void cPreferences::OnHigh() 
{
		m_Low.SetCheck       ( FALSE );
	m_Medium.SetCheck ( FALSE );
	m_High.SetCheck      ( TRUE );

	m_Controls.SetRadio ( 9, false  );
	m_Controls.SetRadio ( 10, false );
	m_Controls.SetRadio ( 11, true );
	
}

void cPreferences::OnHsr() 
{
	if ( m_Controls.m_bRadioClicked [ 12 ] )
	{
		m_Controls.SetRadio ( 12, false );
	}
	else
	{
		m_Controls.SetRadio ( 12, true );
	}
	
}

void cPreferences::OnShowEntity() 
{
	if ( m_Controls.m_bRadioClicked [ 13 ] )
	{
		m_Controls.SetRadio ( 13, false );
	}
	else
	{
		m_Controls.SetRadio ( 13, true );
	}
}

void cPreferences::OnQuickLightMapping() 
{
	m_NoLightMapping.SetCheck    ( FALSE );
	m_QuickLightMapping.SetCheck ( TRUE );
	m_FullLightMapping.SetCheck  ( FALSE );

	m_Controls.SetRadio ( 3, false  );
	m_Controls.SetRadio ( 4, true );
	m_Controls.SetRadio ( 5, false );
	
}

void cPreferences::OnOK() 
{
#ifdef TGC_STORE
	if ( m_chkSaveUsername.GetCheck() ) 
	{
		theApp.m_bSaveUsername = true;
		m_edtUsername.GetWindowText( theApp.m_sUsername );
	}
	else 
	{
		theApp.m_bSaveUsername = false;
		theApp.m_sUsername = "";
	}

	if ( m_chkSavePassword.GetCheck() ) 
	{
		theApp.m_bSavePassword = true;
		m_edtPassword.GetWindowText( theApp.m_sPassword );
	}
	else 
	{
		theApp.m_bSavePassword = false;
		theApp.m_sPassword = "";
	}

	//Show Purchased
	DWORD check = m_chkShowPurchased.GetCheck( ) > 0 ? 1 : 0;
	theApp.SetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "always_display_purchased", check );
	
	//Save Username
	if ( m_chkSaveUsername.GetCheck() )
	{
		char szUsername [ 256 ];
		m_edtUsername.GetWindowText( szUsername, 256 );
		theApp.SetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "username", szUsername );	
	}
	else
	{
		theApp.DeleteRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "username" );
	}

	//Save Password
	if ( m_chkSavePassword.GetCheck() )
	{
		char szPassword [ 50 ];
		m_edtPassword.GetWindowText( szPassword, 50 );
		char *szEnc1 = theApp.Encrypt( szPassword );
		char *szEnc2 = theApp.Encrypt( szEnc1 );
		theApp.SetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "password", szEnc2 );

		delete [] szEnc1;
		delete [] szEnc2;
	}
	else
	{
		theApp.DeleteRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "password" );
	}
#endif

	m_bOK = true;
}

void cPreferences::OnCancel() 
{
	m_bCancel = true;
}

void cPreferences::OnNoFloor() 
{
	if ( m_Controls.m_bRadioClicked [ 15 ] )
	{
		m_Controls.SetRadio ( 15, false );
	}
	else
	{
		m_Controls.SetRadio ( 15, true );
	}
	
}

void cPreferences::OnSinglePlayer() 
{
	m_SinglePlayer.SetCheck ( TRUE );
	m_MultiPlayer.SetCheck ( FALSE );
	m_Controls.SetRadio ( 16, true  );
	m_Controls.SetRadio ( 17, false );
}

void cPreferences::OnMultiplayer() 
{
	m_SinglePlayer.SetCheck ( FALSE );
	m_MultiPlayer.SetCheck ( TRUE );
	m_Controls.SetRadio ( 16, false  );
	m_Controls.SetRadio ( 17, true );
}

#ifdef TGC_STORE

void cPreferences::OnSaveUsernameClicked()
{
	int check = m_chkSaveUsername.GetCheck( );
	if ( check )
	{
		m_edtUsername.EnableWindow( TRUE );
	}
	else
	{
		m_edtUsername.SetWindowText( "" );
		m_edtUsername.EnableWindow( FALSE );
	}
}

void cPreferences::OnSavePasswordClicked()
{
	int check = m_chkSavePassword.GetCheck( );
	if ( check )
	{
		m_edtPassword.EnableWindow( TRUE );
	}
	else
	{
		m_edtPassword.SetWindowText( "" );
		m_edtPassword.EnableWindow( FALSE );
	}
}

void cPreferences::OnShowPurchasedClicked()
{
	
}
#endif
