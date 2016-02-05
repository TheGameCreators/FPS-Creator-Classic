// cBuildGameProject.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cBuildGameProject.h"
#include "cBuildGameKeys.h"
#include "cBuildGameWeapons.h"

#include "cNewProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	BUILD_GAME_PROJECT_WEAPONS_DISPLAY			136

#define	BUILD_GAME_NEW_PROJECT						180

#define	STRING_A									1000
#define	STRING_B									1256
#define	STRING_C									1512
#define	STRING_D									1768


cBuildGameProject::cBuildGameProject ( CWnd* pParent ) : CTabPageSSL ( cBuildGameProject::IDD, pParent ) 
{

	//{{AFX_DATA_INIT(cBuildGameProject)
	//}}AFX_DATA_INIT

	//m_pData = NULL;
	m_bButtonGameProjectFileName = false;
	m_bBrowseFileSelected = false;

	m_iControlsCount = 0;
	m_iActionCount   = 0;

	m_bKeySet = false;
	m_iKeyIndex = 0;

	//m_pControls = NULL;

	m_bWeaponType = false;
	
	
}

void cBuildGameProject::SetControl ( cControls* pControl )
{

	m_pControls = pControl;

	m_pControls->AddEdit ( &m_Filename, 1 );
	//m_pControls->AddEdit ( &m_Info,     2 );
	m_pControls->AddEdit ( &m_Text,     3 );

	//m_pControls->AddButton ( 0 );

	m_pControls->AddList ( &m_Controls, 1 );
	m_pControls->AddList ( &m_Action,   2 );

	m_pControls->AddList ( &m_WeaponsNumber, 3 );
	m_pControls->AddList ( &m_WeaponsType, 4 );	
	
}

void cBuildGameProject::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cBuildGameProject)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_COMMANDS1, m_Controls);
	DDX_Control(pDX, IDC_LIST_KEY1, m_Action);
	DDX_Control(pDX, IDC_EDIT_PROJECT_FILENAME, m_Filename);

	//DDX_Control(pDX, IDC_EDIT_INFO, m_Info);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_Text);

	DDX_Control(pDX, IDC_LIST_WEAPONS_NUMBER, m_WeaponsNumber);
	DDX_Control(pDX, IDC_LIST_WEAPONS_TYPE, m_WeaponsType);

	//if ( &m_WeaponsType == NULL )
	//	MessageBox ( _T("error"),_T("error"), MB_OK );

	//m_WeaponsType.AddString ( _T ( "large" ) );
	//m_WeaponsType.AddString ( _T ( "large" ) );
	//m_WeaponsType.AddString ( _T ( "large" ) );

	//m_Controls.AddString ( _T ( "large" ) );

	//}}AFX_DATA_MAP

	DDX_Control ( pDX, IDC_STATIC_1, m_Lines [ 0 ] );
	DDX_Control ( pDX, IDC_STATIC_2, m_Lines [ 1 ] );
	DDX_Control ( pDX, IDC_BUTTON_PROJECT_NEW, m_Lines [ 2 ] );
	DDX_Control ( pDX, IDC_STATIC_4, m_Lines [ 3 ] );
	DDX_Control ( pDX, IDC_STATIC_5, m_Lines [ 4 ] );
	DDX_Control ( pDX, IDC_STATIC_6, m_Lines [ 5 ] );

	m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Project", "Box1" ) );
	m_Lines [ 1 ].SetWindowText ( GetLanguageData ( "Build Game Project", "Item1" ) );
	m_Lines [ 2 ].SetWindowText ( GetLanguageData ( "Build Game Project", "Item2" ) );
	m_Lines [ 3 ].SetWindowText ( GetLanguageData ( "Build Game Project", "Box2" ) );
	m_Lines [ 4 ].SetWindowText ( GetLanguageData ( "Build Game Project", "Box3" ) );
	m_Lines [ 5 ].SetWindowText ( GetLanguageData ( "Build Game Project", "box4" ) );
}


BEGIN_MESSAGE_MAP(cBuildGameProject, CDialog)
	//{{AFX_MSG_MAP(cBuildGameProject)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_PROJECT_FILENAME, OnButtonProjectFileName)
	
	//ON_LBN_SELCHANGE( IDC_LIST_KEY1, OnClickList )
	ON_LBN_DBLCLK( IDC_LIST_KEY1, OnClickList )

	//ON_CONTROL    ( NM_CLICK, IDC_LIST_KEY1, OnClickList )

	//ON_CONTROL    ( LVS_EX_ONECLICKACTIVATE, IDC_LIST_KEY1, OnClickList )

	ON_LBN_SELCHANGE( IDC_LIST_COMMANDS1, OnClickListCommands )
	ON_LBN_SELCHANGE( IDC_LIST_WEAPONS_NUMBER, OnClickListWeaponsNumber )
	ON_LBN_SELCHANGE( IDC_LIST_WEAPONS_TYPE, OnClickListWeaponsType )
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_PROJECT_NEW, OnButtonProjectNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cBuildGameProject message handlers

void cBuildGameProject::OnSize ( UINT nType, int cx, int cy ) 
{
	CDialog::OnSize ( nType, cx, cy );
	
	// TODO: Add your message handler code here
	
}

int cBuildGameProject::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	
	if ( CDialog::OnCreate ( lpCreateStruct ) == -1 )
		return -1;

	return 0;
}



void cBuildGameProject::OnButtonProjectFileName ( ) 
{
	m_pControls->SetButton ( 1 );

}

BOOL cBuildGameProject::OnInitDialog ( )
{

	CDialog::OnInitDialog ( );

	
	return TRUE;
}



void cBuildGameProject::SetKey ( CString string )
{

	m_Key     = string;
	m_bKeySet = true;
}


void cBuildGameProject::OnClickListWeaponsNumber ( void)
{

	int iIndex = m_WeaponsNumber.SendMessage ( LB_GETCURSEL, 0, 0 );
	m_WeaponsType.SendMessage ( LB_SETCURSEL, iIndex, 0 );
}


//void cBuildGameProject::OnClickListWeaponsType ( NMHDR* pNMHDR, LRESULT* pResult )
void cBuildGameProject::OnClickListWeaponsType ( void )
{
	
	int iIndex = m_WeaponsType.SendMessage ( LB_GETCURSEL, 0, 0 );



	m_WeaponsNumber.SendMessage ( LB_SETCURSEL, iIndex, 0 );

	m_bWeaponType = true;

	cBuildGameWeapons weapons ( this );

	
	weapons.SetControl ( m_pControls );
	weapons.DoModal ( );

	m_bWeaponType = false;

	
	if ( weapons.m_Selection.GetLength ( ) > 0 )
	{
		CString strItem;
		int nIndex = m_WeaponsType.GetCurSel ( );

		// 100105 - no need to check index
		//if ( nIndex )
		{
			m_WeaponsType.GetText ( nIndex, strItem );

			m_WeaponsType.DeleteString ( nIndex );

			//CString wp = _T( "aa" );
			//weapons.m_WeaponType.GetLBText ( 0, wp );

			m_WeaponsType.InsertString( nIndex, weapons.m_Selection );

			//m_WeaponType

			
			m_WeaponsType.SendMessage ( LB_SETCURSEL, iIndex, 0 );
			m_WeaponsNumber.SendMessage ( LB_SETCURSEL, iIndex, 0 );

		}
	}


	DWORD dwDisplay = 0;

	m_pIPC->SendBuffer ( &dwDisplay, BUILD_GAME_PROJECT_WEAPONS_DISPLAY, sizeof ( DWORD ) );

	//memcpy ( &m_pData [ BUILD_GAME_PROJECT_WEAPONS_DISPLAY ], &dwDisplay, sizeof ( DWORD ) );
}


void cBuildGameProject::OnClickListCommands ( void )
{
	

	
	int iIndex = m_Controls.SendMessage ( LB_GETCURSEL, 0, 0 );
	m_Action.SendMessage ( LB_SETCURSEL, iIndex, 0 );
	
}

void cBuildGameProject::OnClickList ( void )
//void cBuildGameProject::OnClickList ( NMHDR* pNMHDR )
{
	

	int iIndex = m_Action.SendMessage ( LB_GETCURSEL, 0, 0 );
	m_Controls.SendMessage ( LB_SETCURSEL, iIndex, 0 );

	cBuildGameKeys keys ( this );
	keys.DoModal ( );

	CString strItem;
    int nIndex = m_Action.GetCurSel ( );
    m_Action.GetText ( nIndex, strItem );

	m_Action.DeleteString ( nIndex );

	m_Action.InsertString( nIndex, keys.m_Key );

	m_Action.SetItemData( nIndex, keys.uiVirtKey );

	keys.m_Key = _T( "" );

	m_Controls.SendMessage ( LB_SETCURSEL, iIndex, 0 );
	m_Action.SendMessage ( LB_SETCURSEL, iIndex, 0 );

	DWORD dwListClicked = 10;
	DWORD dwListControl = 10;

	//memcpy ( &m_pData [ 140 ], &dwListClicked, sizeof ( DWORD ) );
	//memcpy ( &m_pData [ 144 ], &dwListControl, sizeof ( DWORD ) );

	m_pIPC->SendBuffer ( &dwListClicked, 140, sizeof ( DWORD ) );
	m_pIPC->SendBuffer ( &dwListControl, 144, sizeof ( DWORD ) );
}


void cBuildGameProject::Update ( cIPC* pIPC )
{
	
	if ( !pIPC )
		return;

	m_pIPC = pIPC;

	//m_pData = pData;

	if ( m_pControls )
		m_pControls->Update ( pIPC );

	
	if ( m_bWeaponType )
	{
		DWORD dwDisplay = 1;

		//memcpy ( &pData [ BUILD_GAME_PROJECT_WEAPONS_DISPLAY ], &dwDisplay, sizeof ( DWORD ) );

		pIPC->SendBuffer ( &dwDisplay, BUILD_GAME_PROJECT_WEAPONS_DISPLAY, sizeof ( DWORD ) );
	}
	else
	{
		DWORD dwDisplay = 0;

		//memcpy ( &pData [ BUILD_GAME_PROJECT_WEAPONS_DISPLAY ], &dwDisplay, sizeof ( DWORD ) );

		pIPC->SendBuffer ( &dwDisplay, BUILD_GAME_PROJECT_WEAPONS_DISPLAY, sizeof ( DWORD ) );
	}

	
	if ( m_Filename.GetModify ( ) )
	{
		int c = 0;
	}
}

void cBuildGameProject::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void cBuildGameProject::OnButtonProjectNew() 
{
	// TODO: Add your control notification handler code here

	cNewProject NewProject;

	NewProject.DoModal ( );

	char		szTextANSI [ 255 ]		= "";
	CString		szTextUNI				= "";

	//m_Edit.GetWindowText ( szTextUNI );

	char szString [ MAX_PATH ];
	m_pIPC->ReceiveBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );
	ConvertWideToANSI ( ( BYTE* ) &szString [ 0 ], &NewProject.m_UNI, szTextANSI );
	m_pIPC->SendBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );

	//ConvertWideToANSI ( &m_pData [ STRING_A ], &NewProject.m_UNI, szTextANSI );

	DWORD dwDisplay = 1;

		//memcpy ( &m_pData [ BUILD_GAME_NEW_PROJECT ], &dwDisplay, sizeof ( DWORD ) );

	m_pIPC->SendBuffer ( &dwDisplay, BUILD_GAME_NEW_PROJECT, sizeof ( DWORD ) );

	

}
