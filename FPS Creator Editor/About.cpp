#include "stdafx.h"
#include "Editor.h"

#include "about.h"


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
		ON_WM_DESTROY()
		ON_WM_CREATE()
		ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_ABOUT_LINE5, &CAboutDlg::OnStnClickedAboutLine5)
END_MESSAGE_MAP()


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
	DDX_Control(pDX, IDC_COMPANY_URL2, m_btnURL1);

	DDX_Control ( pDX, IDC_ABOUT_LINE1, m_Line1 );
	DDX_Control ( pDX, IDC_ABOUT_LINE2, m_Line2 );
	DDX_Control ( pDX, IDC_ABOUT_LINE3, m_Line3 );
	DDX_Control ( pDX, IDC_ABOUT_LINE4, m_Line4 );
	DDX_Control ( pDX, IDC_ABOUT_LINE5, m_Line5 );

	DDX_Control ( pDX, IDOK, m_OK );
	//}}AFX_DATA_MAP

	TCHAR szLink [ MAX_PATH ];
	TCHAR szVer [ MAX_PATH ];

	GetPrivateProfileString ( _T ( "Help" ), _T ( "About" ), _T ( "" ), szLink, MAX_PATH, theApp.m_szLanguage );
	GetPrivateProfileString ( _T ( "Application" ), _T ( "Version" ), _T ( "" ), szVer, MAX_PATH, theApp.m_szLanguage );

	m_btnURL.SetURL ( szLink );

	

	//m_btnURL1.SetURL ( _T ( "http://www.fpscreator.com" ) );
	

	SetWindowText ( GetLanguageData ( "About", "Title" ) );
	m_Line1.SetWindowText ( GetLanguageData ( "About", "Line1" ) );
	m_Line2.SetWindowText ( GetLanguageData ( "About", "Line2" ) );
	m_Line3.SetWindowText ( GetLanguageData ( "About", "Line3" ) );
	m_Line4.SetWindowText ( GetLanguageData ( "About", "Line4" ) );
	m_Line5.SetWindowText ( GetLanguageData ( "About", "Line5" ) );
	m_OK.SetWindowText ( GetLanguageData ( "About", "Button" ) );

	//m_Version.SetWindowText ( szVer );

	m_btnURL1.SetWindowText ( GetLanguageData ( "Help", "Website" ) );
	m_btnURL.SetWindowText ( GetLanguageData ( "Help", "About" ) );

	m_btnURL1.SetURL ( GetLanguageData ( "Help", "Website" ) );
	m_btnURL.SetURL ( GetLanguageData ( "Help", "About" ) );
}

int CAboutDlg::OnCreate ( LPCREATESTRUCT lpCreateStruct )
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

	/*
	SetCurrentDirectory ( theApp.m_szDirectory );
	TCHAR szTitle [ MAX_PATH ];
	GetPrivateProfileString ( _T ( "About" ), _T ( "Title" ), _T ( "" ), szTitle, MAX_PATH, theApp.m_szLanguage );
	SetWindowText ( szTitle );
	*/



	// all done
	return 0;
}

void CAboutDlg::OnDestroy ( )
{
	// dialog is about to be destroyed

	

	// call default destruction
	CDialog::OnDestroy ( );
}

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	m_hOld = SelectObject ( m_hDCSRC, m_hBMP );
	BitBlt ( dc, 25, 17, 320, 160, m_hDCSRC, 0, 0, SRCCOPY );
	SelectObject ( m_hDCSRC, m_hOld );
}

void CAboutDlg::OnStnClickedAboutLine5()
{
	// TODO: Add your control notification handler code here
}
