// cBuildGameWeapons.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cBuildGameWeapons.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cBuildGameWeapons dialog


cBuildGameWeapons::cBuildGameWeapons(CWnd* pParent )
	: CDialog(cBuildGameWeapons::IDD, pParent)
{
	//{{AFX_DATA_INIT(cBuildGameWeapons)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void cBuildGameWeapons::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cBuildGameWeapons)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_COMBO_WEAPONS, m_WeaponType);
		DDX_Control(pDX, IDC_STATIC_WEAPON_1, m_1);
		DDX_Control(pDX, IDC_STATIC_WEAPON_2, m_2);

		DDX_Control(pDX, IDC_WEAPON_OK, m_OK);
		DDX_Control(pDX, IDCANCEL, m_Cancel);

	//}}AFX_DATA_MAP

	SetWindowText ( GetLanguageData ( "Weapons", "Title" ) );
	m_1.SetWindowText ( GetLanguageData ( "Weapons", "Text1" ) );
	m_2.SetWindowText ( GetLanguageData ( "Weapons", "Text2" ) );

	m_OK.SetWindowText ( GetLanguageData ( "Weapons", "Button1" ) );
	m_Cancel.SetWindowText ( GetLanguageData ( "Weapons", "Button2" ) );
}


BEGIN_MESSAGE_MAP(cBuildGameWeapons, CDialog)
	//{{AFX_MSG_MAP(cBuildGameWeapons)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_WEAPON_OK, OnWeaponOk)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cBuildGameWeapons message handlers

void cBuildGameWeapons::SetControl ( cControls* pControl )
{
	m_pControls = pControl;

	
}

int cBuildGameWeapons::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	m_pControls->AddCombo ( &m_WeaponType, 1 );

	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL cBuildGameWeapons::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
	m_bOk = false;
	
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cBuildGameWeapons::OnWeaponOk() 
{
	
	int iSel = m_WeaponType.GetCurSel ( );

	//if ( iSel )
	{
		m_WeaponType.GetLBText ( iSel, m_Selection );
		
	}

	m_pControls->RemoveCombo ( 1 );
	m_bOk = true;

	EndDialog ( 1 );
	
}

void cBuildGameWeapons::OnDestroy() 
{
	
	
	//
	//if ( m_bOk )
	{
	int iSel = m_WeaponType.GetCurSel ( );

	// 100105 - don't check selection
	//if ( iSel )
	{
	//	m_WeaponType.GetLBText ( iSel, m_Selection );
	}
	

	m_pControls->RemoveCombo ( 1 );
	}
	

	CDialog::OnDestroy();
	
}

void cBuildGameWeapons::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	//m_WeaponType.SetCurSel ( 5 );
}
