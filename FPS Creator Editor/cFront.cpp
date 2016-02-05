// cFront.cpp : implementation file

// standard
#include "stdafx.h"
#include "editor.h"
#include "cFront.h"
#include "cdialogfront.h"

// Vista - detect UAC
#define NTDDI_VERSION NTDDI_LONGHORN
#define IMPLEMENT_VISTA_TOOLS
#include "VistaTools.cxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// externals
extern bool g_bFreeVersion;
extern int g_iUpgradeOffer;
extern int g_iDaysHavePassed;

/////////////////////////////////////////////////////////////////////////////
// cFront dialog

cFront::cFront(CWnd* pParent /*=NULL*/)
	: CDialog(cFront::IDD, pParent)
{
	//{{AFX_DATA_INIT(cFront)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMessageData = NULL;

	m_dwTime = 0;
}


void cFront::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cFront)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Control ( pDX, IDC_STATIC_1, m_Edit );
}


BEGIN_MESSAGE_MAP(cFront, CDialog)
	//{{AFX_MSG_MAP(cFront)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cFront message handlers

int cFront::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Message.Start ( _T ( "FPSSPLASH" ), 1024 * 2, 1, this );

	return 0;
}

void cFront::HandleMessages ( void )
{
	bool bTerminateModal=false;

	// check if ptrs valid
	cIPC* pIPC = m_Message.GetIPC ( );
	if ( !pIPC ) return;

	// skip if mapeditor not waiting for interface
	if ( !pIPC->SkipIfEventNotWaiting () )
		return;

	// do the usual code	
	DWORD dwReady = 1;
	pIPC->SendBuffer ( &dwReady, 0, sizeof ( dwReady ) );

	DWORD dwTitle = 0;
	pIPC->ReceiveBuffer ( &dwTitle, 4, sizeof ( dwTitle ) );
	if ( dwTitle )
	{
		// declare ansi and unicode strings
		char    szTextANSI [ 255 ] = "";
		CString szTextUNI;

		pIPC->ReceiveBuffer ( &szTextANSI, 1000, sizeof ( szTextANSI ) );
		
		// conversion from ansi to unicode
		szTextUNI = szTextANSI;

		// V111 - 140608 - 
		strcpy_s( theApp.m_szVersion, 10, "" );
		FILE *pFile = NULL;
		int error = _tfopen_s( &pFile, _T("version.ini"), _T("rb") );
		if ( !error )
		{
			fgets( theApp.m_szVersion, 10, pFile );
			fclose( pFile );
			pFile = NULL;
		}
		szTextUNI += " ";
		szTextUNI += theApp.m_szVersion;

		// now set the window text
		m_Edit.SetWindowText ( szTextUNI );

		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 20, sizeof ( dwValue ) );

		// ESRB silliness
		//Sleep ( 4500 );
		Sleep ( 500 );
	}

	DWORD dwClose = 0;
	pIPC->ReceiveBuffer ( &dwClose, 8, sizeof ( dwClose ) );
	if ( dwClose )
	{
		bTerminateModal=true;
	}

	// only when update completely done, signal that event complete
	pIPC->AllowSyncEventNow();

	// termination of modal here
	if ( bTerminateModal )
	{
		EndDialog ( 1 );
	}
}

LPSTR cFront::CheckOSVersion ( void )
{
	OSVERSIONINFO versionInfo;
	versionInfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	if (!::GetVersionEx (&versionInfo))
	{
		return _T ("Not able to get OS information");
	}
	if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (versionInfo.dwMajorVersion == 6)
		{
			return _T ("Windows Vista");
		}
		else
		{
			if (versionInfo.dwMajorVersion == 5)
			{
				return _T ("Windows XP");
			}
			else
			{
				if (versionInfo.dwMajorVersion == 3 || versionInfo.dwMinorVersion == 51)
					return _T ("Windows NT 3.51");
				else
					return _T ("Windows NT 4.0");
			}
		}
	}
	else if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		if ((versionInfo.dwMajorVersion > 4) || ((versionInfo.dwMajorVersion == 4) && (versionInfo.dwMinorVersion > 0)))
		{
			return _T ("Windows 98");
		}
		else
		{
			return _T ("Windows 95");
		}
	}
	else
	{
		return _T ("Windows 3.1");
	}
}

BOOL cFront::OnInitDialog() 
{
	CDialog::OnInitDialog();

	TCHAR szString [ MAX_PATH ];
	GetPrivateProfileString ( _T ( "Splash" ), _T ( "Start" ), _T ( "" ), szString, MAX_PATH, theApp.m_szLanguage );
	m_Edit.SetWindowText ( szString );

	// V110 - 300508 - if VISTA, detect if not administrator and UAC enabled
	if ( stricmp ( CheckOSVersion(), "Windows Vista" )==NULL )
	{
		TOKEN_ELEVATION_TYPE tet = TokenElevationTypeDefault;
		::GetElevationType( &tet );
		BOOL bElevated = FALSE;
		::IsElevated( &bElevated );
		if ( bElevated==FALSE )
		{
			// warn user UAC must be switched off
			//LPSTR pWarning = "In Windows Vista, ensure you are logged in as Administrator and UAC is switched off. By default Windows Vista starts with an Administrator account with UAC enabled. To disable UAC go to START\\Control Panel\\User Accounts and Family Safety\\User Accounts and find the 'Turn User Account Control off' item. Be aware that you must disable UAC at your own risk!";
			MessageBox ( GetLanguageData( "TGC Store", "VistaUACWarning" ), "Windows Vista UAC Detected", MB_OK );

			// immediately exit software
			EndDialog ( 1 );
			TerminateProcess ( theApp.m_DBProApplication.hProcess, 0 );
			PostQuitMessage ( 0 );
		}
	}

	SetCurrentDirectory ( theApp.m_szDirectory );
	SetCurrentDirectory ( theApp.m_szLanguagePath );
	SetCurrentDirectory ( "artwork" );
	

	m_hDCSRC = CreateCompatibleDC ( NULL ); 
	m_hBMP = LoadImage ( NULL, "splsh16.bmp", IMAGE_BITMAP, 512, 256, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	SetCurrentDirectory ( theApp.m_szDirectory );

	// FREE VERSION - serial code read from USERDETAILS.INI (else free version)
	// V111 - 140608 - moved from here to MainFrm (as we need it BEFORE we set the title!)
	// theApp.CheckSerialCodeValidity();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cFront::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	m_hOld = SelectObject ( m_hDCSRC, m_hBMP );
	BitBlt ( dc, 3, 1, 512, 256, m_hDCSRC, 0, 0, SRCCOPY );
	SelectObject ( m_hDCSRC, m_hOld );
}

