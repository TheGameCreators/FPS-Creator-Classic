// cBuildGameLight.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cBuildGameLight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cBuildGameLight::cBuildGameLight ( CWnd* pParent ): CTabPageSSL ( cBuildGameLight::IDD, pParent )
{
	//{{AFX_DATA_INIT(cBuildGameLight)
	//}}AFX_DATA_INIT

	m_pControls = NULL;
}

void cBuildGameLight::DoDataExchange ( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cBuildGameLight)
		// NOTE: the ClassWizard will add DDX and DDV calls here

		DDX_Control ( pDX, IDC_EDIT_INFO_1,	m_Info1 );
		DDX_Control ( pDX, IDC_EDIT_INFO_2,	m_Info2 );

		DDX_Control ( pDX, IDC_RADIO_NO_LIGHTMAPS,			m_NoLightMaps   );
		DDX_Control ( pDX, IDC_RADIO_SOFT_LIGHTMAPS,		m_SoftLightMaps );
		DDX_Control ( pDX, IDC_RADIO_FULL_SHADOW_LIGHTMAPS, m_FullLightMaps );

		DDX_Control ( pDX, IDC_RADIO_LOW,    m_Low );
		DDX_Control ( pDX, IDC_RADIO_MEDIUM, m_Medium );
		DDX_Control ( pDX, IDC_RADIO_HIGH,   m_High );
		
		DDX_Control ( pDX, IDC_STATIC_1,				m_Lines [ 0 ] );
		DDX_Control ( pDX, IDC_STATIC_2,				m_Lines [ 1 ] );
		DDX_Control ( pDX, IDC_STATIC_3,				m_Lines [ 2 ] );

		m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Light", "Box1" ) );
		m_Lines [ 1 ].SetWindowText ( GetLanguageData ( "Build Game Light", "Box2" ) );
		m_Lines [ 2 ].SetWindowText ( GetLanguageData ( "Build Game Light", "Box3" ) );

		m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Light", "Box1" ) );
		m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Light", "Box1" ) );
		m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Build Game Light", "Box1" ) );

		m_NoLightMaps.SetWindowText ( GetLanguageData ( "Build Game Light", "Item1" ) );
		m_SoftLightMaps.SetWindowText ( GetLanguageData ( "Build Game Light", "Item2" ) );
		m_FullLightMaps.SetWindowText ( GetLanguageData ( "Build Game Light", "Item3" ) );
		m_Low.SetWindowText ( GetLanguageData ( "Build Game Light", "Item4" ) );
		m_Medium.SetWindowText ( GetLanguageData ( "Build Game Light", "Item5" ) );
		m_High.SetWindowText ( GetLanguageData ( "Build Game Light", "Item6" ) );

		//DDX_Control ( pDX, IDC_STATIC_1, m_Picture1 );
		//DDX_Control ( pDX, IDC_STATIC_2, m_Picture2 );

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cBuildGameLight, CDialog)
	//{{AFX_MSG_MAP(cBuildGameLight)
	ON_BN_CLICKED(IDC_RADIO_NO_LIGHTMAPS, OnRadioNoLightmaps)
	ON_BN_CLICKED(IDC_RADIO_SOFT_LIGHTMAPS, OnRadioSoftLightmaps)
	ON_BN_CLICKED(IDC_RADIO_FULL_SHADOW_LIGHTMAPS, OnRadioFullShadowLightmaps)

	ON_BN_CLICKED(IDC_RADIO_LOW, OnRadioLow)
	ON_BN_CLICKED(IDC_RADIO_MEDIUM, OnRadioMedium)
	ON_BN_CLICKED(IDC_RADIO_HIGH, OnRadioHigh)

	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cBuildGameLight message handlers

void cBuildGameLight::Update ( cIPC* pIPC )
{
	if ( !pIPC )
		return;
}

void cBuildGameLight::SetControl ( cControls* pControl )
{
	m_pControls = pControl;

	m_pControls->AddEdit ( &m_Info1,  9 );
	m_pControls->AddEdit ( &m_Info2, 10 );

	m_pControls->AddRadioButton ( &m_NoLightMaps,   1 );
	m_pControls->AddRadioButton ( &m_SoftLightMaps, 2 );
	m_pControls->AddRadioButton ( &m_FullLightMaps, 3 );

	m_pControls->AddRadioButton ( &m_Low,   4 );
	m_pControls->AddRadioButton ( &m_Medium, 5 );
	m_pControls->AddRadioButton ( &m_High, 6 );
}

void cBuildGameLight::OnRadioNoLightmaps() 
{
	m_NoLightMaps.SetCheck   ( TRUE );
	m_SoftLightMaps.SetCheck ( FALSE );
	m_FullLightMaps.SetCheck ( FALSE );
	
	m_pControls->SetRadio ( 1, true  );
	m_pControls->SetRadio ( 2, false );
	m_pControls->SetRadio ( 3, false );
}

void cBuildGameLight::OnRadioSoftLightmaps() 
{
	m_NoLightMaps.SetCheck   ( FALSE );
	m_SoftLightMaps.SetCheck ( TRUE );
	m_FullLightMaps.SetCheck ( FALSE );	

	m_pControls->SetRadio ( 1, false  );
	m_pControls->SetRadio ( 2, true );
	m_pControls->SetRadio ( 3, false );
}

void cBuildGameLight::OnRadioFullShadowLightmaps() 
{
	m_NoLightMaps.SetCheck   ( FALSE );
	m_SoftLightMaps.SetCheck ( FALSE );
	m_FullLightMaps.SetCheck ( TRUE );

	m_pControls->SetRadio ( 1, false );
	m_pControls->SetRadio ( 2, false );
	m_pControls->SetRadio ( 3, true );
}

void cBuildGameLight::OnRadioLow() 
{
	m_Low.SetCheck    ( TRUE );
	m_Medium.SetCheck ( FALSE );
	m_High.SetCheck   ( FALSE );

	m_pControls->SetRadio ( 4, true  );
	m_pControls->SetRadio ( 5, false );
	m_pControls->SetRadio ( 6, false );
}

void cBuildGameLight::OnRadioMedium() 
{
	m_Low.SetCheck    ( FALSE );
	m_Medium.SetCheck ( TRUE );
	m_High.SetCheck   ( FALSE );

	m_pControls->SetRadio ( 4, false  );
	m_pControls->SetRadio ( 5, true );
	m_pControls->SetRadio ( 6, false );
}

void cBuildGameLight::OnRadioHigh() 
{
	m_Low.SetCheck    ( FALSE );
	m_Medium.SetCheck ( FALSE );
	m_High.SetCheck   ( TRUE );

	m_pControls->SetRadio ( 4, false );
	m_pControls->SetRadio ( 5, false );
	m_pControls->SetRadio ( 6, true );
}

int cBuildGameLight::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	// create the dialog

	// call the implementation
	if ( CDialog::OnCreate ( lpCreateStruct ) == -1 )
		return -1;
	
	SetCurrentDirectory ( theApp.m_szDirectory );
	//SetCurrentDirectory ( "files\\languagebank\\english\\artwork\\" );
	SetCurrentDirectory ( theApp.m_szLanguagePath );
	SetCurrentDirectory ( "artwork" );
	
	m_hDCSRC = CreateCompatibleDC ( NULL ); 
	m_hBMP = LoadImage ( NULL, "lightmapoff.bmp", IMAGE_BITMAP, 201, 171, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION );

	m_hDCSRC1 = CreateCompatibleDC ( NULL ); 
	m_hBMP1 = LoadImage ( NULL, "lightmapon.bmp", IMAGE_BITMAP, 201, 171, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	
	SetCurrentDirectory ( theApp.m_szDirectory );

	// all done
	return 0;
}

void cBuildGameLight::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	m_hOld = SelectObject ( m_hDCSRC, m_hBMP );
	BitBlt ( dc, 54, 136, 210, 171, m_hDCSRC, 0, 0, SRCCOPY );
	SelectObject ( m_hDCSRC, m_hOld );

	m_hOld1 = SelectObject ( m_hDCSRC1, m_hBMP1 );
	BitBlt ( dc, 299, 136, 201, 171, m_hDCSRC1, 0, 0, SRCCOPY );
	SelectObject ( m_hDCSRC1, m_hOld1 );
}
