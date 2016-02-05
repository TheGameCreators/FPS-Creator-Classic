// cBuildGameLevel.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cBuildGameLevel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cBuildGameLevel::cBuildGameLevel ( CWnd* pParent ) : CTabPageSSL ( cBuildGameLevel::IDD, pParent )
{
	//{{AFX_DATA_INIT(cBuildGameLevel)
	//}}AFX_DATA_INIT

	m_pData     = NULL;
	m_pControls = NULL;

	memset ( &m_bListClicked, 0, sizeof ( m_bListClicked ) );;
}

void cBuildGameLevel::SetControl ( cControls* pControl )
{
	m_pControls = pControl;
	
	m_pControls->AddList   ( &m_TitleScreen,			 5 );
	m_pControls->AddList   ( &m_GlobalScript,			 6 );
	m_pControls->AddList   ( &m_GameOver,				 7 );
	m_pControls->AddList   ( &m_GameComplete,			 8 );

	m_pControls->AddList   ( &m_MapNumbers,				 9 );
	m_pControls->AddList   ( &m_MapNames,				10 );
	m_pControls->AddList   ( &m_MapLoadingScreen,		11 );
	
	m_pControls->AddButton ( &m_ButtonTitleScreen,		2 );
	m_pControls->AddButton ( &m_ButtonGlobalScript,		3 );
	m_pControls->AddButton ( &m_ButtonGameOver,			4 );
	m_pControls->AddButton ( &m_ButtonGameComplete,		5 );

	m_pControls->AddButton ( &m_ButtonInsertLevel,		6 );
	m_pControls->AddButton ( &m_ButtonDeleteLevel,		7 );
	m_pControls->AddButton ( &m_ButtonNewLevelScript,	8 );
	m_pControls->AddButton ( &m_ButtonAddLevel,			17 );
}

void cBuildGameLevel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cBuildGameLevel)
		// NOTE: the ClassWizard will add DDX and DDV calls here

		DDX_Control ( pDX, IDC_LIST_TITLE_SCREEN,		m_TitleScreen );
		DDX_Control ( pDX, IDC_LIST_GLOBAL_SCRIPT,		m_GlobalScript );
		DDX_Control ( pDX, IDC_LIST_GAME_OVER,			m_GameOver );
		DDX_Control ( pDX, IDC_LIST_GAME_COMPLETE,		m_GameComplete );

		DDX_Control ( pDX, IDC_LIST_MAP_NUMBERS,		m_MapNumbers );
		DDX_Control ( pDX, IDC_LIST_MAP_NAMES,			m_MapNames );
		DDX_Control ( pDX, IDC_LIST_MAP_LOADING_SCREEN,	m_MapLoadingScreen );

		DDX_Control ( pDX, IDC_BUTTON_TITLE_SCREEN,		m_ButtonTitleScreen );
		DDX_Control ( pDX, IDC_BUTTON_GLOBAL_SCRIPT,	m_ButtonGlobalScript );
		DDX_Control ( pDX, IDC_BUTTON_GAME_OVER,		m_ButtonGameOver );
		DDX_Control ( pDX, IDC_BUTTON_GAME_COMPLETE,	m_ButtonGameComplete );

		DDX_Control ( pDX, IDC_BUTTON_ADD_LEVEL2,		m_ButtonAddLevel );
		DDX_Control ( pDX, IDC_BUTTON_INSERT_LEVEL,		m_ButtonInsertLevel );
		//DDX_Control ( pDX, IDC_BUTTON_PREVIEW_LEVEL,	m_ButtonPreviewLevel );
		DDX_Control ( pDX, IDC_BUTTON_DELETE_LEVEL,		m_ButtonDeleteLevel );
		DDX_Control ( pDX, IDC_BUTTON_NEW_LEVEL_SCRIPT,	m_ButtonNewLevelScript );
	//}}AFX_DATA_MAP

		DDX_Control ( pDX, IDC_STATIC_1,				m_Lines [ 0 ] );
		DDX_Control ( pDX, IDC_STATIC_2,				m_Lines [ 1 ] );
		DDX_Control ( pDX, IDC_STATIC_3,				m_Lines [ 2 ] );
		DDX_Control ( pDX, IDC_STATIC_4,				m_Lines [ 3 ] );
		DDX_Control ( pDX, IDC_STATIC_5,				m_Lines [ 4 ] );

		DDX_Control ( pDX, IDC_STATIC_6,				m_Lines [ 5 ] );
		DDX_Control ( pDX, IDC_STATIC_7,				m_Lines [ 6 ] );
		DDX_Control ( pDX, IDC_STATIC_8,				m_Lines [ 7 ] );
		DDX_Control ( pDX, IDC_STATIC_9,				m_Lines [ 8 ] );

		m_Lines [  0 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Box1" ) );
		m_Lines [  1 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Item1" ) );
		m_Lines [  2 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Item2" ) );
		m_Lines [  3 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Item3" ) );
		m_Lines [  4 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Item4" ) );

		m_Lines [  5 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Box2" ) );
		m_Lines [  6 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Item9" ) );
		m_Lines [  7 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Item10" ) );
		m_Lines [  8 ].SetWindowText ( GetLanguageData ( "Build Game Level", "Item11" ) );

		m_ButtonTitleScreen.SetWindowText ( GetLanguageData ( "Build Game Level", "Item5" ) );
		m_ButtonGlobalScript.SetWindowText ( GetLanguageData ( "Build Game Level", "Item6" ) );
		m_ButtonGameOver.SetWindowText ( GetLanguageData ( "Build Game Level", "Item7" ) );
		m_ButtonGameComplete.SetWindowText ( GetLanguageData ( "Build Game Level", "Item8" ) );

		m_ButtonAddLevel.SetWindowText ( GetLanguageData ( "Build Game Level", "Button1" ) );
		m_ButtonInsertLevel.SetWindowText ( GetLanguageData ( "Build Game Level", "Button2" ) );
		m_ButtonDeleteLevel.SetWindowText ( GetLanguageData ( "Build Game Level", "Button3" ) );
		m_ButtonNewLevelScript.SetWindowText ( GetLanguageData ( "Build Game Level", "Button4" ) );
}


BEGIN_MESSAGE_MAP(cBuildGameLevel, CDialog)
	//{{AFX_MSG_MAP(cBuildGameLevel)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED ( IDC_BUTTON_TITLE_SCREEN,	 OnButtonTitleScreen )
	ON_BN_CLICKED ( IDC_BUTTON_GLOBAL_SCRIPT,	 OnButtonGlobalScript )
	ON_BN_CLICKED ( IDC_BUTTON_GAME_OVER,		 OnButtonGameOver )
	ON_BN_CLICKED ( IDC_BUTTON_GAME_COMPLETE,	 OnButtonGameComplete )
	ON_BN_CLICKED ( IDC_BUTTON_INSERT_LEVEL,	 OnButtonInsertLevel )
	ON_BN_CLICKED ( IDC_BUTTON_DELETE_LEVEL,	 OnButtonDeleteLevel )
	ON_BN_CLICKED ( IDC_BUTTON_NEW_LEVEL_SCRIPT, OnButtonNewLevelScript )
	ON_BN_CLICKED ( IDC_BUTTON_ADD_LEVEL2, OnButtonAddLevel )
	ON_LBN_SELCHANGE( IDC_LIST_MAP_NUMBERS, OnClickList1 )
	ON_LBN_SELCHANGE( IDC_LIST_MAP_NAMES, OnClickList2 )
	ON_LBN_SELCHANGE( IDC_LIST_MAP_LOADING_SCREEN, OnClickList3 )
	ON_LBN_SELCHANGE( IDC_LIST_TITLE_SCREEN,  OnClickListTitleScreen )
	ON_LBN_SELCHANGE( IDC_LIST_GLOBAL_SCRIPT, OnClickListGlobalScript )
	ON_LBN_SELCHANGE( IDC_LIST_GAME_OVER,     OnClickListGameOver )
	ON_LBN_SELCHANGE( IDC_LIST_GAME_COMPLETE, OnClickListGameComplete )
	ON_WM_VSCROLL()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cBuildGameLevel message handlers

void cBuildGameLevel::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
}

void cBuildGameLevel::Update ( cIPC* pIPC )
{
	if ( !pIPC )
		return;

	m_pIPC = pIPC;
	if ( m_pControls )
		m_pControls->Update ( pIPC );

	// arena mode disables
	if ( g_bBuildGameInArenaMode==true )
	{
		m_ButtonAddLevel.EnableWindow ( FALSE );
		m_ButtonDeleteLevel.EnableWindow ( FALSE );
		m_ButtonInsertLevel.EnableWindow ( FALSE );

		// lee - 140307 - also need to disable FPI editing
		// as they create SINGLE PLAYER PAGES ONLY!
		m_ButtonTitleScreen.EnableWindow ( FALSE );
		m_ButtonGlobalScript.EnableWindow ( FALSE );
		m_ButtonGameOver.EnableWindow ( FALSE );
		m_ButtonGameComplete.EnableWindow ( FALSE );
		m_ButtonNewLevelScript.EnableWindow ( FALSE );
	}
	else
	{
		m_ButtonAddLevel.EnableWindow ( TRUE );
		m_ButtonDeleteLevel.EnableWindow ( TRUE );
		m_ButtonInsertLevel.EnableWindow ( TRUE );
		m_ButtonTitleScreen.EnableWindow ( TRUE );
		m_ButtonGlobalScript.EnableWindow ( TRUE );
		m_ButtonGameOver.EnableWindow ( TRUE );
		m_ButtonGameComplete.EnableWindow ( TRUE );
		m_ButtonNewLevelScript.EnableWindow ( TRUE );
	}

	// 
	for ( int i = 0; i < 20; i++ )
	{
		if ( m_bListClicked [ i ] )
		{
			DWORD dwListClicked = 1;
			DWORD dwListControl = i;
			pIPC->SendBuffer ( &dwListClicked, 140, sizeof ( DWORD ) );
			pIPC->SendBuffer ( &dwListControl, 144, sizeof ( DWORD ) );
			m_bListClicked [ i ] = false;
		}
	}
}

BOOL cBuildGameLevel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;
}

int cBuildGameLevel::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	if ( CDialog::OnCreate ( lpCreateStruct ) == -1 )
		return -1;
	
	return 0;
}

void cBuildGameLevel::OnClickList1 (  )
{
	m_bListClicked [ 9 ] = true;

	

	

	int iIndex = m_MapNumbers.SendMessage ( LB_GETCURSEL, 0, 0 );

	
	m_MapNames.SendMessage ( LB_SETCURSEL, iIndex, 0 );
	m_MapLoadingScreen.SendMessage ( LB_SETCURSEL, iIndex, 0 );

	//m_MapNumbers.ScrollWindow		( 0, 50000, NULL, NULL );
	//m_MapNames.ScrollWindow			( 0, 50000, NULL, NULL );
	//m_MapLoadingScreen.ScrollWindow ( 0, 50000, NULL, NULL );

	//m_MapNumbers.SetScrollPos( SB_VERT, 0, TRUE );
	//m_MapNames.SetScrollPos( SB_VERT, 0, TRUE );
	//m_MapLoadingScreen.SetScrollPos( SB_VERT, 0, TRUE );

	UpdateWindow ( );
}


void cBuildGameLevel::OnClickList2 (  )
{
	m_bListClicked [ 10 ] = true;

	int iIndex = m_MapNames.SendMessage ( LB_GETCURSEL, 0, 0 );

	m_MapNumbers.SendMessage ( LB_SETCURSEL, iIndex, 0 );
	m_MapLoadingScreen.SendMessage ( LB_SETCURSEL, iIndex, 0 );

	// click at bottom of list for levels and no selection
}

void cBuildGameLevel::OnClickList3 ( )
{
	m_bListClicked [ 11 ] = true;

	int iIndex = m_MapLoadingScreen.SendMessage ( LB_GETCURSEL, 0, 0 );

	m_MapNumbers.SendMessage ( LB_SETCURSEL, iIndex, 0 );
	m_MapNames.SendMessage ( LB_SETCURSEL, iIndex, 0 );
}

void cBuildGameLevel::OnClickListTitleScreen (  )
{
	m_bListClicked [ 0 ] = true;

	int iIndex = m_TitleScreen.SendMessage ( LB_GETCURSEL, 0, 0 );
	m_TitleScreen.SendMessage ( LB_SETCURSEL, -1, 0 );
}

void cBuildGameLevel::OnClickListGlobalScript ( )
{
	m_bListClicked [ 1 ] = true;

	int iIndex = m_GlobalScript.SendMessage ( LB_GETCURSEL, 0, 0 );
	m_GlobalScript.SendMessage ( LB_SETCURSEL, -1, 0 );
}

void cBuildGameLevel::OnClickListGameOver ( )
{
	m_bListClicked [ 2 ] = true;

	int iIndex = m_GameOver.SendMessage ( LB_GETCURSEL, 0, 0 );
	m_GameOver.SendMessage ( LB_SETCURSEL, -1, 0 );
}

void cBuildGameLevel::OnClickListGameComplete ( )
{
	m_bListClicked [ 3 ] = true;

	int iIndex = m_GameComplete.SendMessage ( LB_GETCURSEL, 0, 0 );
	m_GameComplete.SendMessage ( LB_SETCURSEL, -1, 0 );
}

void cBuildGameLevel::OnUpdateBrowse ( void )
{
	
}

void cBuildGameLevel::OnButtonTitleScreen ( )
{
	m_pControls->SetButton ( 2 );
}

void cBuildGameLevel::OnButtonGlobalScript ( )
{
	m_pControls->SetButton ( 3 );
}

void cBuildGameLevel::OnButtonGameOver ( )
{
	m_pControls->SetButton ( 4 );
}

void cBuildGameLevel::OnButtonGameComplete ( )
{
	m_pControls->SetButton ( 5 );
}

void cBuildGameLevel::OnButtonInsertLevel ( )
{
	m_pControls->SetButton ( 6 );
}

/*
void cBuildGameLevel::OnButtonPreviewLevel ( )
{
	m_pControls->SetButton ( 7 );
}
*/

void cBuildGameLevel::OnButtonDeleteLevel ( )
{
	m_pControls->SetButton ( 7 );
}

void cBuildGameLevel::OnButtonNewLevelScript ( )
{
	m_pControls->SetButton ( 8 );
}

void cBuildGameLevel::OnButtonAddLevel ( )
{
	m_pControls->SetButton ( 17 );
}

void cBuildGameLevel::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void cBuildGameLevel::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}
