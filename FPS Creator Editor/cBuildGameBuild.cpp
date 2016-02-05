// cBuildGameBuild.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cBuildGameBuild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool g_bBuildGameInProgress;

cBuildGameBuild::cBuildGameBuild ( CWnd* pParent ): CTabPageSSL ( cBuildGameBuild::IDD, pParent )
{
	//{{AFX_DATA_INIT(cBuildGameBuild)
	//}}AFX_DATA_INIT

	//m_pMessageData  = NULL;
	m_bValid	    = false;
	m_pControls		= NULL;
}

void cBuildGameBuild::SetControl ( cControls* pControl )
{
	m_pControls = pControl;

	
	m_pControls->AddEdit        ( &m_Path,             11 );
	m_pControls->AddEdit        ( &m_Status,           12 );
	m_pControls->AddRadioButton ( &m_NoEffects,        10 );
	m_pControls->AddRadioButton ( &m_EssentialEffects, 11 );
	m_pControls->AddRadioButton ( &m_FullEffects,      12 );

	m_pControls->AddRadioButton ( &m_HSR,				13 );
	m_pControls->AddRadioButton ( &m_DynamicShadows,    14 );
	m_pControls->AddRadioButton ( &m_DynamicLights,     15 );

	m_pControls->AddRadioButton ( &m_NoFloor,     16 );
	
	m_pControls->AddProgressBar ( &m_Progress,          1 );

	m_pControls->AddEdit        ( &m_Name,             13 );

	//m_pControls->AddButton ( &m_Build, 10 );
	//m_pControls->AddButton ( &m_ExePath, 10 );
	
}

void cBuildGameBuild::DoDataExchange ( CDataExchange* pDX )
{
	CDialog::DoDataExchange ( pDX );

	//{{AFX_DATA_MAP(cBuildGameBuild)
	//}}AFX_DATA_MAP

	
	DDX_Control ( pDX, IDC_PROGRESS_PROJECT,		m_Progress );
	
	DDX_Control ( pDX, IDC_EDIT_PROJECT_STATUS,		m_Status   );
	
	DDX_Control ( pDX, IDC_EDIT_PATH,				m_Path     );

	DDX_Control ( pDX, IDC_EDIT_PATH2,				m_Name     );

	DDX_Control ( pDX, IDC_RADIO_NO_EFFECTS,		m_NoEffects );
	DDX_Control ( pDX, IDC_RADIO_ESSENTIAL_EFFECTS, m_EssentialEffects );
	DDX_Control ( pDX, IDC_RADIO_FULL_EFFECTS,		m_FullEffects );

	DDX_Control ( pDX, IDC_CHECK_HSR,		m_HSR );
	DDX_Control ( pDX, IDC_CHECK_DYNAMIC_SHADOWS,		m_DynamicShadows );
	DDX_Control ( pDX, IDC_CHECK_DYNAMIC_LIGHTS,		m_DynamicLights );

	DDX_Control ( pDX, IDC_NO_FLOOR,		m_NoFloor );

	// DDX_Control ( pDX, IDC_BUTTON_BUILD,			m_Build    );
	// DDX_Control ( pDX, IDC_BUTTON_EXECUTABLE_PATH,			m_ExePath    );

	DDX_Control ( pDX, IDC_STATIC_1, m_Lines [ 0 ] );
	DDX_Control ( pDX, IDC_STATIC_2, m_Lines [ 1 ] );
	DDX_Control ( pDX, IDC_STATIC_3, m_Lines [ 2 ] );
	DDX_Control ( pDX, IDC_STATIC_5, m_Lines [ 3 ] );
	DDX_Control ( pDX, IDC_STATIC_8, m_Lines [ 4 ] );

	DDX_Control ( pDX, IDC_BUTTON_BUILD, m_Buttons );

	

	m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Build", "Box1" ) );
	m_Lines [ 1 ].SetWindowText ( GetLanguageData ( "Build Game Build", "Item1" ) );
	m_Lines [ 2 ].SetWindowText ( GetLanguageData ( "Build Game Build", "Item2" ) );
	m_Lines [ 3 ].SetWindowText ( GetLanguageData ( "Build Game Build", "Box2" ) );
	m_Lines [ 4 ].SetWindowText ( GetLanguageData ( "Build Game Build", "Box3" ) );

	m_NoEffects.SetWindowText			( GetLanguageData ( "Build Game Build", "Item3" ) );
	m_EssentialEffects.SetWindowText	( GetLanguageData ( "Build Game Build", "Item4" ) );
	
	m_NoFloor.SetWindowText				( GetLanguageData ( "Build Game Build", "Item5" ) );
	m_DynamicLights.SetWindowText		( GetLanguageData ( "Build Game Build", "Item6" ) );

	m_Buttons.SetWindowText		( GetLanguageData ( "Build Game Build", "Item7" ) );
}

BEGIN_MESSAGE_MAP ( cBuildGameBuild, CDialog )
	//{{AFX_MSG_MAP(cBuildGameBuild)
	ON_WM_SIZE()
	
	ON_WM_DESTROY()
	
	ON_BN_CLICKED(IDC_RADIO_NO_EFFECTS, OnRadioNoEffects)
	ON_BN_CLICKED(IDC_RADIO_ESSENTIAL_EFFECTS, OnRadioEssentialEffects)
	ON_BN_CLICKED(IDC_RADIO_FULL_EFFECTS, OnRadioFullEffects)
	ON_BN_CLICKED(IDC_CHECK_HSR, OnRadioHSR)
	//ON_BN_CLICKED(IDC_CHECK_DYNAMIC_SHADOWS, OnRadioDynamicShadows)
	ON_BN_CLICKED(IDC_CHECK_DYNAMIC_LIGHTS, OnRadioDynamicLights)
	ON_BN_CLICKED(IDC_NO_FLOOR, OnNoFloor)

	ON_BN_CLICKED(IDC_BUTTON_EXECUTABLE_PATH, OnButtonExecutablePath)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnButtonBuild)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cBuildGameBuild message handlers

void cBuildGameBuild::OnSize ( UINT nType, int cx, int cy )
{
	CDialog::OnSize ( nType, cx, cy );
}

BOOL cBuildGameBuild::OnInitDialog ( ) 
{
	CDialog::OnInitDialog ( );
	
	m_Progress.SetRange ( 0, 100 );
	m_Progress.SetStep  ( 1 );
	m_Progress.SetPos   ( 0 );

	m_Buttons.EnableWindow ( FALSE );

	m_bValid	   = true;
	m_BuildClicked = false;
	
	return TRUE;
}

void cBuildGameBuild::Update ( cIPC* pIPC )
{
	if ( !pIPC )
		return;

	if ( !m_bValid )
		return;

	// special built in rules
	// if BUILDEXE edit box empty, BUILDEXECUTABLE button disabled
	CString szString;
	m_Name.GetWindowText ( szString );
	if ( strcmp ( szString, "" )==NULL )
		m_Buttons.EnableWindow ( FALSE );
	else
		m_Buttons.EnableWindow ( TRUE );

	m_pIPC = pIPC;
}

void cBuildGameBuild::OnDestroy ( )
{
	CDialog::OnDestroy ( );
}

void cBuildGameBuild::DisableAll ( void )
{
	m_Path.EnableWindow ( FALSE );
	m_Status.EnableWindow ( FALSE );
	m_NoEffects.EnableWindow ( FALSE );
	m_EssentialEffects.EnableWindow ( FALSE );
	m_FullEffects.EnableWindow ( FALSE );
	m_DynamicShadows.EnableWindow ( FALSE );
	m_DynamicLights.EnableWindow ( FALSE );
	m_NoFloor.EnableWindow ( FALSE );
	m_Progress.EnableWindow ( FALSE );
	m_Name.EnableWindow ( FALSE );
	m_Buttons.EnableWindow ( FALSE );
}

void cBuildGameBuild::EnableAll ( void )
{
	m_Path.EnableWindow ( TRUE );
	m_Status.EnableWindow ( TRUE );
	m_NoEffects.EnableWindow ( TRUE );
	m_EssentialEffects.EnableWindow ( TRUE );
	m_FullEffects.EnableWindow ( TRUE );
	m_DynamicShadows.EnableWindow ( TRUE );
	m_DynamicLights.EnableWindow ( TRUE );
	m_NoFloor.EnableWindow ( TRUE );
	m_Progress.EnableWindow ( TRUE );
	m_Name.EnableWindow ( TRUE );
	m_Buttons.EnableWindow ( TRUE );
}

void cBuildGameBuild::OnButtonBuild ( ) 
{
	m_pControls->SetButton ( 10 );
	g_bBuildGameInProgress = true;
	DisableAll();
}

void cBuildGameBuild::OnButtonExecutablePath ( )
{
	m_pControls->SetButton ( 9 );
}

void cBuildGameBuild::OnRadioNoEffects ( ) 
{
	m_NoEffects.SetCheck        ( TRUE );
	m_EssentialEffects.SetCheck ( FALSE );
	m_FullEffects.SetCheck      ( FALSE );

	m_pControls->SetRadio ( 10, true  );
	m_pControls->SetRadio ( 11, false );
	m_pControls->SetRadio ( 12, false );
}

void cBuildGameBuild::OnRadioEssentialEffects ( ) 
{
	m_NoEffects.SetCheck        ( FALSE );
	m_EssentialEffects.SetCheck ( TRUE );
	m_FullEffects.SetCheck     ( FALSE );	

	m_pControls->SetRadio ( 10, false  );
	m_pControls->SetRadio ( 11, true );
	m_pControls->SetRadio ( 12, false );
}

void cBuildGameBuild::OnRadioFullEffects ( ) 
{
	m_NoEffects.SetCheck        ( FALSE );
	m_EssentialEffects.SetCheck ( FALSE );
	m_FullEffects.SetCheck      ( TRUE );

	m_pControls->SetRadio ( 10, false );
	m_pControls->SetRadio ( 11, false );
	m_pControls->SetRadio ( 12, true  );
}

void cBuildGameBuild::OnRadioHSR ( ) 
{
	if ( m_pControls->m_bRadioClicked [ 13 ] )
	{
		m_pControls->SetRadio ( 13, false );
	}
	else
	{
		m_pControls->SetRadio ( 13, true );
	}
}

/*
void cBuildGameBuild::OnRadioDynamicShadows ( ) 
{
	if ( m_pControls->m_bRadioClicked [ 14 ] )
	{
		m_pControls->SetRadio ( 14, false );
	}
	else
	{
		m_pControls->SetRadio ( 14, true );
	}
}
*/

void cBuildGameBuild::OnRadioDynamicLights ( ) 
{
	if ( m_pControls->m_bRadioClicked [ 15 ] )
	{
		m_pControls->SetRadio ( 15, false );
	}
	else
	{
		m_pControls->SetRadio ( 15, true );
	}
}
void cBuildGameBuild::OnNoFloor() 
{
		if ( m_pControls->m_bRadioClicked [ 16 ] )
	{
		m_pControls->SetRadio ( 16, false );
	}
	else
	{
		m_pControls->SetRadio ( 16, true );
	}
	
}
