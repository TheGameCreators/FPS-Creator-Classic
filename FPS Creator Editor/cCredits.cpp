// cCredits.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cCredits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cCredits dialog


cCredits::cCredits(CWnd* pParent /*=NULL*/)
	: CDialog(cCredits::IDD, pParent)
{
	//{{AFX_DATA_INIT(cCredits)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cCredits::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cCredits)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control ( pDX, IDC_STATIC_CREDITS_1, m_Lines [ 0 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_2, m_Lines [ 1 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_3, m_Lines [ 2 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_4, m_Lines [ 3 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_5, m_Lines [ 4 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_6, m_Lines [ 5 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_7, m_Lines [ 6 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_8, m_Lines [ 7 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_9, m_Lines [ 8 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_10, m_Lines [ 9 ] );
		DDX_Control ( pDX, IDC_STATIC_CREDITS_11, m_Lines [ 10 ] );
		DDX_Control ( pDX, IDOK, m_OK );
	//}}AFX_DATA_MAP

	

	SetWindowText ( GetLanguageData ( "Credits", "Title" ) );
	m_Lines [ 0 ].SetWindowText ( GetLanguageData ( "Credits", "Line1" ) );
	m_Lines [ 1 ].SetWindowText ( GetLanguageData ( "Credits", "Line2" ) );
	m_Lines [ 2 ].SetWindowText ( GetLanguageData ( "Credits", "Line3" ) );
	m_Lines [ 3 ].SetWindowText ( GetLanguageData ( "Credits", "Line4" ) );
	m_Lines [ 4 ].SetWindowText ( GetLanguageData ( "Credits", "Line5" ) );
	m_Lines [ 5 ].SetWindowText ( GetLanguageData ( "Credits", "Line6" ) );
	m_Lines [ 6 ].SetWindowText ( GetLanguageData ( "Credits", "Line7" ) );
	m_Lines [ 7 ].SetWindowText ( GetLanguageData ( "Credits", "Line8" ) );
	m_Lines [ 8 ].SetWindowText ( GetLanguageData ( "Credits", "Line9" ) );
	m_Lines [ 9 ].SetWindowText ( GetLanguageData ( "Credits", "Line10" ) );
	m_Lines [ 10 ].SetWindowText ( GetLanguageData ( "Credits", "Line11" ) );
	m_OK.SetWindowText ( GetLanguageData ( "Credits", "Button" ) );
}


BEGIN_MESSAGE_MAP(cCredits, CDialog)
	//{{AFX_MSG_MAP(cCredits)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_CREATE()
		ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cCredits message handlers

int cCredits::OnCreate ( LPCREATESTRUCT lpCreateStruct )
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
	m_hBMP = LoadImage ( NULL, "aboutsplash.bmp", IMAGE_BITMAP, 320, 160, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	
	SetCurrentDirectory ( theApp.m_szDirectory );

	// all done
	return 0;
}


void cCredits::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	m_hOld = SelectObject ( m_hDCSRC, m_hBMP );
	BitBlt ( dc, 25, 17, 320, 160, m_hDCSRC, 0, 0, SRCCOPY );
	SelectObject ( m_hDCSRC, m_hOld );
}
