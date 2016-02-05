// cDialogEnd.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cDialogEnd.h"

#include <afxctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cDialogEnd dialog


cDialogEnd::cDialogEnd(CWnd* pParent /*=NULL*/)
	: CDialog(cDialogEnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(cDialogEnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iQuit = 0;
}


void cDialogEnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cDialogEnd)
		//DDX_Control ( pDX, IDC_STATIC_END, m_Bitmap );

			DDX_Control ( pDX, IDOK,		m_OK );
			DDX_Control ( pDX, IDCANCEL,	m_Cancel );
	//}}AFX_DATA_MAP

	m_OK.SetWindowText ( GetLanguageData ( "Demo", "Button2" ) );
	m_Cancel.SetWindowText ( GetLanguageData ( "Demo", "Button3" ) );

	
}


BEGIN_MESSAGE_MAP(cDialogEnd, CDialog)
	//{{AFX_MSG_MAP(cDialogEnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cDialogEnd message handlers

extern int g_iBoysLifeQuit;

void cDialogEnd::OnOK() 
{
	// TODO: Add extra validation here

	//SetCurrentDirectory ( theApp.m_szDirectory );

	char szDebug [ 256 ] = "";
	//GetPrivateProfileString ( _T ( "Demo" ), _T ( "Order" ), _T ( "" ), szDebug, MAX_PATH, _T ( "./files/editors/host/editor.ini" ) );
	GetPrivateProfileString ( _T ( "Demo" ), _T ( "Order" ), _T ( "" ), szDebug, MAX_PATH, theApp.m_szLanguage );
	::ShellExecute (NULL, NULL, szDebug, NULL, NULL, NULL);
	
	if ( g_iBoysLifeQuit != 1 )
		PostQuitMessage ( 0 );
	
	CDialog::OnOK();
}

void cDialogEnd::OnCancel() 
{
	// TODO: Add extra cleanup here

	//
		//SetCurrentDirectory ( theApp.m_szDirectory );

	char szDebug [ 256 ] = "";
	//GetPrivateProfileString ( _T ( "Demo" ), _T ( "Order" ), _T ( "" ), szDebug, MAX_PATH, _T ( "./files/editors/host/editor.ini" ) );
	GetPrivateProfileString ( _T ( "Help" ), _T ( "Website" ), _T ( "" ), szDebug, MAX_PATH, theApp.m_szLanguage );
	::ShellExecute (NULL, NULL, szDebug, NULL, NULL, NULL);
	//

	if ( g_iBoysLifeQuit != 1 )
		PostQuitMessage ( 0 );
	
	CDialog::OnCancel();
	
	
}

int cDialogEnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*
	CPictureHolder test;
	GetDlgItem ( IDC_STATIC_END, ( HWND* )  &test );

	GetDlgItem ( IDC_STATIC_END, ( HWND* ) &m_Bitmap );

	SetCurrentDirectory ( theApp.m_szDirectory );
	m_Bitmap.Attach ( ::LoadImage ( NULL, "demosplash.bmp", IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION ) );
	*/

		
	SetCurrentDirectory ( theApp.m_szDirectory );
	//SetCurrentDirectory ( "files\\languagebank\\english\\artwork\\" );

	SetCurrentDirectory ( theApp.m_szLanguagePath );
	SetCurrentDirectory ( "artwork" );
	
	m_hDCSRC = CreateCompatibleDC ( NULL ); 
	m_hBMP = LoadImage ( NULL, "demosplash.bmp", IMAGE_BITMAP, 640, 480, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION );

	SetCurrentDirectory ( theApp.m_szDirectory );

	return 0;
}

void cDialogEnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages

	m_hOld = SelectObject ( m_hDCSRC, m_hBMP );
	BitBlt ( dc, 2, 2, 640, 480, m_hDCSRC, 0, 0, SRCCOPY );
	SelectObject ( m_hDCSRC, m_hOld );
}
