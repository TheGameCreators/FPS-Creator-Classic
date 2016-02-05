// cBuildGameMultiplayer.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cBuildGameMultiplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cBuildGameMultiplayer dialog

// External global 
extern bool g_bBuildGameInArenaMode;

//cBuildGameMultiplayer::cBuildGameMultiplayer(CWnd* pParent /*=NULL*/) : CDialog(cBuildGameMultiplayer::IDD, pParent)
cBuildGameMultiplayer::cBuildGameMultiplayer ( CWnd* pParent ): CTabPageSSL ( cBuildGameMultiplayer::IDD, pParent )
{
	//{{AFX_DATA_INIT(cBuildGameMultiplayer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cBuildGameMultiplayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cBuildGameMultiplayer)
		// NOTE: the ClassWizard will add DDX and DDV calls here

		DDX_Control ( pDX, IDC_MP_EDIT1,	m_Edit [ 0 ] );
		DDX_Control ( pDX, IDC_MP_EDIT2,	m_Edit [ 1 ] );
		DDX_Control ( pDX, IDC_MP_EDIT3,	m_Edit [ 2 ] );

		DDX_Control ( pDX, IDC_MP_OBJECTIVE,	m_Buttons [ 0 ] );
		DDX_Control ( pDX, IDC_MP_FIRSTTO,		m_Buttons [ 1 ] );
		DDX_Control ( pDX, IDC_MP_HIGHESTFRAG,	m_Buttons [ 2 ] );
		DDX_Control ( pDX, IDC_MP_ONESHOT,		m_Buttons [ 3 ] );
		DDX_Control ( pDX, IDC_MP_DEFAULT,		m_Buttons [ 4 ] );
		DDX_Control ( pDX, IDC_MP_RANDOM,		m_Buttons [ 5 ] );

		// lee - need to add external text
		/*
		m_Buttons [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Box1" ) );
		m_Buttons [ 1 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Box1" ) );
		m_Buttons [ 2 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Box1" ) );
		m_Buttons [ 3 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Box1" ) );
		m_Buttons [ 4 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Box1" ) );
		m_Buttons [ 5 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Box1" ) );
		*/

	//}}AFX_DATA_MAP


	DDX_Control ( pDX, IDC_MP_EDIT1,	m_Edit [ 0 ] );
	DDX_Control ( pDX, IDC_MP_EDIT2,	m_Edit [ 1 ] );
	DDX_Control ( pDX, IDC_MP_EDIT3,	m_Edit [ 2 ] );

	DDX_Control ( pDX, IDC_MP_OBJECTIVE,	m_Buttons [ 0 ] );
	DDX_Control ( pDX, IDC_MP_FIRSTTO,		m_Buttons [ 1 ] );
	DDX_Control ( pDX, IDC_MP_HIGHESTFRAG,	m_Buttons [ 2 ] );
	DDX_Control ( pDX, IDC_MP_ONESHOT,		m_Buttons [ 3 ] );
	DDX_Control ( pDX, IDC_MP_DEFAULT,		m_Buttons [ 4 ] );
	DDX_Control ( pDX, IDC_MP_RANDOM,		m_Buttons [ 5 ] );

	DDX_Control ( pDX, IDC_STATIC_1,		m_Lines [ 0 ] );
	DDX_Control ( pDX, IDC_STATIC_10,		m_Lines [ 1 ] );
	DDX_Control ( pDX, IDC_STATIC_11,		m_Lines [ 2 ] );
	DDX_Control ( pDX, IDC_STATIC_12,		m_Lines [ 3 ] );
	DDX_Control ( pDX, IDC_STATIC_HOW,		m_Lines [ 4 ] );
	DDX_Control ( pDX, IDC_STATIC_FRAGS,	m_Lines [ 5 ] );
	DDX_Control ( pDX, IDC_STATIC_SECONDS,	m_Lines [ 6 ] );
	DDX_Control ( pDX, IDC_STATIC_NUMPLR,	m_Lines [ 7 ] );

	m_Buttons [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item2" ) );
	m_Buttons [ 1 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item3" ) );
	m_Buttons [ 2 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item5" ) );
	m_Buttons [ 3 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item8" ) );
	m_Buttons [ 4 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item11" ) );
	m_Buttons [ 5 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item12" ) );

	m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item1" ) );
	m_Lines [ 1 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item7" ) );
	m_Lines [ 2 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item10" ) );
	m_Lines [ 3 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item13" ) );
	m_Lines [ 4 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item14" ) );
	m_Lines [ 5 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item4" ) );
	m_Lines [ 6 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item6" ) );
	m_Lines [ 7 ].SetWindowText ( GetLanguageData ( "Build Game Multiplayer", "Item9" ) );
}


BEGIN_MESSAGE_MAP(cBuildGameMultiplayer, CDialog)
	//{{AFX_MSG_MAP(cBuildGameMultiplayer)
		// NOTE: the ClassWizard will add message map macros here
		ON_BN_CLICKED(IDC_MP_OBJECTIVE,		OnRadioObjective)
		ON_BN_CLICKED(IDC_MP_FIRSTTO,		OnRadioFirstTo)
		ON_BN_CLICKED(IDC_MP_HIGHESTFRAG,	OnRadioHighestFrag)
		ON_BN_CLICKED(IDC_MP_ONESHOT,		OnRadioOneShot)
		ON_BN_CLICKED(IDC_MP_DEFAULT,		OnRadioDefault)
		ON_BN_CLICKED(IDC_MP_RANDOM,		OnRadioRandom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cBuildGameMultiplayer message handlers

void cBuildGameMultiplayer::Update ( cIPC* pIPC )
{

	if ( m_Buttons[0].GetCheck()==FALSE && m_Buttons[1].GetCheck()==FALSE && m_Buttons[2].GetCheck()==FALSE )
	{
		// Disable arena tab if all not set
		if ( bArenaTabDisabled==false )
		{
			bArenaTabDisabled=true;
			m_Edit[0].EnableWindow ( FALSE );
			m_Edit[1].EnableWindow ( FALSE );
			m_Edit[2].EnableWindow ( FALSE );
			m_Buttons[0].EnableWindow ( FALSE );
			m_Buttons[1].EnableWindow ( FALSE );
			m_Buttons[2].EnableWindow ( FALSE );
			m_Buttons[3].EnableWindow ( FALSE );
			m_Buttons[4].EnableWindow ( FALSE );
			m_Buttons[5].EnableWindow ( FALSE );
			g_bBuildGameInArenaMode = false;
		}
	}
	else
	{
		// Enable arena tab if any one is set
		if ( bArenaTabDisabled==true )
		{
			bArenaTabDisabled=false;
			m_Edit[0].EnableWindow ( TRUE );
			m_Edit[1].EnableWindow ( TRUE );
			m_Edit[2].EnableWindow ( TRUE );
			m_Buttons[0].EnableWindow ( TRUE );
			m_Buttons[1].EnableWindow ( TRUE );
			m_Buttons[2].EnableWindow ( TRUE );
			m_Buttons[3].EnableWindow ( TRUE );
			m_Buttons[4].EnableWindow ( TRUE );
			m_Buttons[5].EnableWindow ( TRUE );
			g_bBuildGameInArenaMode = true;
		}
	}

	if ( !pIPC )
		return;
}

void cBuildGameMultiplayer::SetControl ( cControls* pControl )
{

	m_pControls = pControl;

	m_pControls->AddEdit ( &m_Edit [ 0 ], 18 );
	m_pControls->AddEdit ( &m_Edit [ 1 ], 19 );
	m_pControls->AddEdit ( &m_Edit [ 2 ], 20 );

	m_pControls->AddRadioButton ( &m_Buttons [ 0 ], 21 );
	m_pControls->AddRadioButton ( &m_Buttons [ 1 ], 22 );
	m_pControls->AddRadioButton ( &m_Buttons [ 2 ], 23 );
	m_pControls->AddRadioButton ( &m_Buttons [ 3 ], 24 );
	m_pControls->AddRadioButton ( &m_Buttons [ 4 ], 25 );
	m_pControls->AddRadioButton ( &m_Buttons [ 5 ], 26 );

	// default is disabled
	bArenaTabDisabled = false;
	g_bBuildGameInArenaMode = true;
}

void cBuildGameMultiplayer::OnRadioObjective()
{
	m_Buttons[0].SetCheck ( TRUE );
	m_Buttons[1].SetCheck ( FALSE );
	m_Buttons[2].SetCheck ( FALSE );
	m_pControls->SetRadio ( 21, true  );
	m_pControls->SetRadio ( 22, false );
	m_pControls->SetRadio ( 23, false );
}

void cBuildGameMultiplayer::OnRadioFirstTo()
{
	m_Buttons[0].SetCheck ( FALSE );
	m_Buttons[1].SetCheck ( TRUE );
	m_Buttons[2].SetCheck ( FALSE );
	m_pControls->SetRadio ( 21, false  );
	m_pControls->SetRadio ( 22, true );
	m_pControls->SetRadio ( 23, false );
}

void cBuildGameMultiplayer::OnRadioHighestFrag()
{
	m_Buttons[0].SetCheck ( FALSE );
	m_Buttons[1].SetCheck ( FALSE );
	m_Buttons[2].SetCheck ( TRUE );
	m_pControls->SetRadio ( 21, false  );
	m_pControls->SetRadio ( 22, false );
	m_pControls->SetRadio ( 23, true );
}

void cBuildGameMultiplayer::OnRadioOneShot()
{
	if ( m_Buttons[3].GetCheck()==TRUE )
		m_pControls->SetRadio ( 24, true );
	else
		m_pControls->SetRadio ( 24, false );
}

void cBuildGameMultiplayer::OnRadioDefault()
{
	m_Buttons[4].SetCheck ( TRUE );
	m_Buttons[5].SetCheck ( FALSE );
	m_pControls->SetRadio ( 25, true  );
	m_pControls->SetRadio ( 26, false );
}

void cBuildGameMultiplayer::OnRadioRandom()
{
	m_Buttons[4].SetCheck ( FALSE );
	m_Buttons[5].SetCheck ( TRUE );
	m_pControls->SetRadio ( 25, false );
	m_pControls->SetRadio ( 26, true );
}
