// Editor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Editor.h"
#include "Splash.h"
#include "cFront.h"
#include "mmsystem.h"

#include "cLibraryWindow.h"
#include "cLibraryWindowNew.h"
#include "cBuildGame.h"
#include "cTestGame.h"
#include "cFPIWizard.h"
#include "cPopup.h"

#include "FileDialog.h"
#include <tlhelp32.h>
#include "enumprocess.h"
#include "cdialogend.h"

#include "HelpWizard.h"		//HELPW - help wizard class

#include <afxinet.h>		//AutoUpdate
#include "UpdateCheckThread.h"
#include "TGC Store\UploadAppDlg.h"
#include "TGC Store\MarketWindow.h"

// serial code 
#include "SerialCode\md5.h"
#include <math.h>
char* pKeyCode = "00V1";
int iKeyA = 4;
int iKeyB = 9;
int iKeyC = 0;
int iKeyD = 1;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Global to control interface use
bool		g_bStopInterfaceOperations		= false;
bool		g_bWebsiteLink					= false;
bool		g_bFreeVersion					= true;	//default to free version if the check is some how skipped
int			g_iUpgradeOffer					= 0;
int			g_iDaysHavePassed				= 0;

/////////////////////////////////////////////////////////////////////////////
// CEditorApp

BEGIN_MESSAGE_MAP(CEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit1)
	ON_COMMAND(ID_EDIT_COPY_1, OnEditCopy1)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_CUT_1, OnEditCut1)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_PASTE_1, OnEditPaste1)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_REDO_1, OnEditRedo1)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_UNDO_1, OnEditUndo1)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)

	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT_1, OnUpdateEditCut1)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_1, OnUpdateEditCopy1)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_1, OnUpdateEditPaste1)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO_1, OnUpdateEditUndo1)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO_1, OnUpdateEditRedo1)
	ON_UPDATE_COMMAND_UI(ID_FILE_BUILDGAME, OnUpdateBuildGame)
//	ON_COMMAND(ID_HELP_WEBSITE, OnHelpWebsite)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEWARENA, OnUpdateFileNewArena)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_EX, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_EX, OnUpdateFileSaveAsEx)
	ON_UPDATE_COMMAND_UI(ID_FILE_PREFERENCES, OnUpdateFilePreferences)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateFileMruFile1)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE2, OnUpdateFileMruFile2)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE3, OnUpdateFileMruFile3)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE4, OnUpdateFileMruFile4)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE5, OnUpdateFileMruFile5)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE6, OnUpdateFileMruFile6)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE7, OnUpdateFileMruFile7)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE8, OnUpdateFileMruFile8)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE9, OnUpdateFileMruFile9)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE10, OnUpdateFileMruFile10)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE11, OnUpdateFileMruFile11)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE12, OnUpdateFileMruFile12)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE13, OnUpdateFileMruFile13)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE14, OnUpdateFileMruFile14)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE15, OnUpdateFileMruFile15)
	/*
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEW, OnUpdateViewView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREFAB, OnUpdateViewPrefab)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAW, OnUpdateViewDraw)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WAYPOINT, OnUpdateViewWaypoint)

	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TEST_GAME, OnUpdateViewTestGame)
	*/
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_NEWARENA, OnFileNewArena)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	
	
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// CEditorApp construction

CString GetLanguageData ( CString group, CString key )
{
	//SetCurrentDirectory ( theApp.m_szDirectory );
	char szText [ MAX_PATH ] = "";
	GetPrivateProfileString ( group, key, _T ( "" ), szText, MAX_PATH, theApp.m_szLanguage );
	CString string = szText;
	if ( string.GetLength() < 1 ) string = key;
	return string;
}


void ConvertWideToANSI ( BYTE* pData, CString* pUNI, char* pANSI )
{
	strcpy ( pANSI, *pUNI );

	if ( pData )
		memcpy ( pData, &pANSI [ 0 ], sizeof ( char ) * 255 );
}

void ConvertANSIToWide ( BYTE* pData, char* pANSI, CString* pUNI )
{
	memcpy ( &pANSI [ 0 ], pData, sizeof ( char ) * 255 );
	*pUNI = pANSI;
}

void ConvertANSIToWide ( char* pANSI, CString* pUNI )
{
	*pUNI = pANSI;
}

CEditorApp::CEditorApp() : CBCGWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
	m_pHelpWizardDlg	= NULL;		//HELPW - initial value
	m_bHelpWizardVisible = true;

	m_pUpdateThread		= NULL;		//AutoUpdate

	m_bDisplayMouse		= TRUE;
	m_lastPoint.x		= 0;
	m_lastPoint.y		= 0;
	m_bEnd				= FALSE;
	m_bDisable			= true;
	m_bFocus			= false;
	m_bEntityFocus		= false;
	m_iRecover			= 0;
	m_dwRecoverTimer	= 0;
	m_bAppHasStartedRunning = false;
	m_bDoNotNeedToCheckForCrashesAnymore = false;
	m_bGameOrBuildInProgress = false;
	m_bRestart			= false;
	m_bSplashScreen		= true;
	m_bBuildGameDisplay = false;
	m_bTestGameCancel	= false;

	memset ( m_bTest, 1, sizeof ( m_bTest ) );

#ifdef TGC_STORE
	m_szSESID = NULL;
	m_szCredits = NULL;
	strcpy_s( m_szOrderBy, 32, "Name" );
	strcpy_s( m_szAuthorName, 32, "" );

	strcpy_s( m_szLanguageName, 40, "english" );

	m_bAdmin = false;
	m_bUploader = false;
	m_bAgreedTerms = false;
	m_sUsername = "";
	m_sPassword = "";
	m_hInet = NULL;
	m_bGotoAddons = false;

	m_bFirstRun = false;
	InitializeCriticalSectionAndSpinCount( &m_csDirectoryChanges, 500 );
#endif
	
}

CEditorApp::~CEditorApp( )
{
#ifdef TGC_STORE
	ClearTGCStoreCache();

	if ( m_szSESID ) 
	{
		delete [] m_szSESID;
		m_szSESID = NULL;
	}
	if ( m_szCredits ) 
	{
		delete [] m_szCredits;
		m_szCredits = NULL;
	}

	if ( m_hInet ) 
	{
		InternetCloseHandle( m_hInet );
		m_hInet = NULL;
	}

	DeleteCriticalSection( &m_csDirectoryChanges );
#endif

	//HELPW - clear any help dialog running
	if ( m_pHelpWizardDlg ) 
	{
		if ( m_bHelpWizardVisible ) 
		{
			m_pHelpWizardDlg->EndDialog( 0 );
			m_pHelpWizardDlg->Join();
		}
		delete m_pHelpWizardDlg;
		m_pHelpWizardDlg = NULL;
	}
	//HELPW - end

	//AutoUpdate
	if ( m_pUpdateThread )
	{
		m_pUpdateThread->CancelDownload( );
		m_pUpdateThread->Join( );
		delete m_pUpdateThread;
		m_pUpdateThread = NULL;
	}
}

void CEditorApp::ClearTGCStoreCache( )
{
	EnterCriticalSection( &m_csDirectoryChanges );

	if ( !SetCurrentDirectory( m_szDirectory ) ) 
	{
		LeaveCriticalSection( &m_csDirectoryChanges );
		return;
	}
	if ( !SetCurrentDirectory( "Files\\TGCStore\\TEMP" ) ) 
	{
		LeaveCriticalSection( &m_csDirectoryChanges );
		return;
	}

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
		
	hFind = FindFirstFile("*.*", &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
			{
				if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					DeleteFile( FindFileData.cFileName );
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);

		FindClose(hFind);
	}

	if ( SetCurrentDirectory( "Thumbnails" ) )
	{
		hFind = FindFirstFile("*.*", &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
				{
					if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
					{
						DeleteFile( FindFileData.cFileName );
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);

			FindClose(hFind);
		}
	}

	if ( SetCurrentDirectory( "..\\Images" ) )
	{
		hFind = FindFirstFile("*.*", &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
				{
					if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
					{
						DeleteFile( FindFileData.cFileName );
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);

			FindClose(hFind);
		}
	}

	if ( SetCurrentDirectory( "..\\Samples" ) )
	{
		hFind = FindFirstFile("*.*", &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
				{
					if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
					{
						DeleteFile( FindFileData.cFileName );
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);

			FindClose(hFind);
		}
	}

	if ( !SetCurrentDirectory( m_szDirectory ) ) 
	{
		LeaveCriticalSection( &m_csDirectoryChanges );
		return;
	}
	if ( !SetCurrentDirectory( "Files\\TGCStore" ) ) 
	{
		LeaveCriticalSection( &m_csDirectoryChanges );
		return;
	}

	CUploadAppDlg uploader;
	uploader.ClearUploadFolder( "Uploads" );

	LeaveCriticalSection( &m_csDirectoryChanges );
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEditorApp object

CEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEditorApp initialization


BOOL CALLBACK EnumWindowsProcEx(HWND hwnd,LPARAM lParam );

BOOL CALLBACK EnumWindowsProcLee( HWND hWnd, LPARAM lParam )
{
	// see the class name
	char pClass [ 256 ];
	GetClassName ( hWnd, pClass, 256 );
	if ( strnicmp ( pClass, "FPSC", 4 )==NULL )
	{
		// list any with FPSC prefix
		strcat ( (char*)lParam, pClass );
	}
	return TRUE;//continue
}

void CEditorApp::SetRegistryValue( const char *szLocation, const char *szKey, const char *szValue )
{
	if ( !szLocation || strlen(szLocation) < 1 ) return;
	if ( !szKey || strlen(szKey) < 1 ) return;
	if ( !szValue || strlen(szValue) < 1 ) return;
	
	HKEY hKey = 0;
	
	try 
	{
		LSTATUS ls = RegCreateKeyEx( HKEY_CURRENT_USER, szLocation, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL );
		if ( ls != ERROR_SUCCESS ) throw 0;

		ls = RegSetValueEx( hKey, szKey, 0, REG_SZ, (BYTE*)szValue, strlen(szValue)+1 );
		if ( ls != ERROR_SUCCESS ) throw 1;

		RegCloseKey( hKey );
	}
	catch ( int )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("SetRegFailed") ), m_szErrorString, 0 );
		RegCloseKey( hKey );
	}
}

void CEditorApp::SetRegistryValue( const char *szLocation, const char *szKey, DWORD dwValue )
{
	if ( !szLocation || strlen(szLocation) < 1 ) return;
	if ( !szKey || strlen(szKey) < 1 ) return;
	
	HKEY hKey = 0;
	
	try 
	{
		LSTATUS ls = RegCreateKeyEx( HKEY_CURRENT_USER, szLocation, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL );
		if ( ls != ERROR_SUCCESS ) throw 0;

		ls = RegSetValueEx( hKey, szKey, 0, REG_DWORD, (BYTE*)&dwValue, 4 );
		if ( ls != ERROR_SUCCESS ) throw 1;

		RegCloseKey( hKey );
	}
	catch ( int )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("SetRegFailed") ), m_szErrorString, 0 );
		RegCloseKey( hKey );
	}
}

bool CEditorApp::GetRegistryValue( const char *szLocation, const char *szKey, char *szValue, DWORD dwSize, HKEY hRoot )
{
	if ( !szLocation || strlen(szLocation) < 1 ) return false;
	if ( !szKey || strlen(szKey) < 1 ) return false;
	if ( !szValue ) return false;

	HKEY hKey = 0;
	DWORD dwDataSize = dwSize;
	DWORD dwType = REG_SZ;

	try
	{
		LSTATUS ls = RegOpenKeyEx( hRoot, szLocation, NULL, KEY_QUERY_VALUE, &hKey );
		if ( ls != ERROR_SUCCESS ) throw 0;

		ls = RegQueryValueEx( hKey, szKey, NULL, &dwType, (BYTE*)szValue, &dwDataSize );
		if ( ls != ERROR_SUCCESS ) throw 1;

		RegCloseKey( hKey );

		return true;
	}
	catch (int)
	{
		RegCloseKey( hKey );

		return false;
	}
}

bool CEditorApp::GetRegistryValue( const char *szLocation, const char *szKey, DWORD *dwValue )
{
	if ( !szLocation || strlen(szLocation) < 1 ) return false;
	if ( !szKey || strlen(szKey) < 1 ) return false;
	if ( !dwValue ) return false;

	HKEY hKey = 0;
	DWORD dwDataSize = 4;
	DWORD dwType = REG_DWORD;

	try
	{
		LSTATUS ls = RegOpenKeyEx( HKEY_CURRENT_USER, szLocation, NULL, KEY_QUERY_VALUE, &hKey );
		if ( ls != ERROR_SUCCESS ) throw 0;

		ls = RegQueryValueEx( hKey, szKey, NULL, &dwType, (BYTE*)dwValue, &dwDataSize );
		if ( ls != ERROR_SUCCESS ) throw 1;

		RegCloseKey( hKey );

		return true;
	}
	catch (int)
	{
		RegCloseKey( hKey );

		return false;
	}
}

bool CEditorApp::DeleteRegistryValue( const char *szLocation, const char *szKey )
{
	if ( !szLocation || strlen(szLocation) < 1 ) return false;
	if ( !szKey || strlen(szKey) < 1 ) return false;

	HKEY hKey = 0;

	try 
	{
		LSTATUS ls = RegOpenKeyEx( HKEY_CURRENT_USER, szLocation, 0, KEY_SET_VALUE, &hKey );
		if ( ls != ERROR_SUCCESS ) throw 0;

		ls = RegDeleteValue( hKey, szKey );
		if ( ls != ERROR_SUCCESS ) throw 1;

		RegCloseKey( hKey );
		return true;
	}
	catch ( int )
	{
		//MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("DelRegFailed") ), theApp.m_szErrorString, 0 );
		RegCloseKey( hKey );
		return false;
	}
}

char* CEditorApp::Encrypt( const char *szString )
{
	const unsigned char *szPtr = (const unsigned char*) szString;

	DWORD dwLength = strlen( szString );
	DWORD dwNewLength = dwLength * 2 + 1;
	char *szNewString = new char [ dwNewLength + 1 ];

	int iCypher [ 10 ];
	iCypher [ 0 ] = -24;
	iCypher [ 1 ] = 144;
	iCypher [ 2 ] = 108;
	iCypher [ 3 ] = -62;
	iCypher [ 4 ] = 88;
	iCypher [ 5 ] = -194;
	iCypher [ 6 ] = -12;
	iCypher [ 7 ] = 111;
	iCypher [ 8 ] = -47;
	iCypher [ 9 ] = 65;
	
	for ( DWORD i = 0; i < dwNewLength; i++ )
	{
		if ( i % 2 == 0 )
		{
			//even
			int value = 0;
			DWORD index = i/2;
			if ( index > 0 ) value += szPtr [ index - 1 ];
			if ( index < dwLength ) value += szPtr [ index ];

			value *= 11;
			value += iCypher [ index % 10 ];
			if ( value < 0 ) value *= -1;
			value %= 224;
			value += 32;

			szNewString [ i ] = value;
		}
		else
		{
			//odd
			DWORD index = (i - 1) / 2;
			
			int value = 0;
			value = szPtr [ index ];
			value -= 32;
			value += iCypher [ index % 10 ];
			value %= 224;
			if ( value < 0 ) value += 224;
			value += 32;

			szNewString [ i ] = value;
		}
	}

	szNewString [ dwNewLength ] = '\0';

	return szNewString;
}

char* CEditorApp::Decrypt( const char *szString )
{
	const unsigned char *szPtr = (const unsigned char*) szString;

	int iCypher [ 10 ];
	iCypher [ 0 ] = -24;
	iCypher [ 1 ] = 144;
	iCypher [ 2 ] = 108;
	iCypher [ 3 ] = -62;
	iCypher [ 4 ] = 88;
	iCypher [ 5 ] = -194;
	iCypher [ 6 ] = -12;
	iCypher [ 7 ] = 111;
	iCypher [ 8 ] = -47;
	iCypher [ 9 ] = 65;

	DWORD dwLength = strlen( szString );
	DWORD dwNewLength = (dwLength-1) / 2;
	char *szNewString = new char [ dwNewLength + 1 ];
	
	for ( DWORD i = 0; i < dwNewLength; i++ )
	{
		DWORD index = (i*2) + 1;
		
		int value = 0;
		value = szPtr [ index ];
		value -= 32;
		value -= iCypher [ i % 10 ];
		if ( value < 0 ) value += 224;
		value = (value % 224);
		value += 32;

		szNewString [ i ] = value;
	}

	szNewString [ dwNewLength ] = '\0';

	return szNewString;
}

bool CEditorApp::GetDefaultBrowserPath( char *szValue, DWORD dwSize )
{
	if ( !szValue ) return false;

	HKEY hKey = 0;
	DWORD dwDataSize = dwSize;
	DWORD dwType = REG_SZ;

	try
	{
		LSTATUS ls = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet", NULL, KEY_QUERY_VALUE, &hKey );
		if ( ls != ERROR_SUCCESS ) throw 0;

		ls = RegQueryValueEx( hKey, NULL, NULL, &dwType, (BYTE*)szValue, &dwDataSize );
		if ( ls != ERROR_SUCCESS ) throw 1;

		RegCloseKey( hKey );
	}
	catch (int)
	{
		RegCloseKey( hKey );

		return false;
	}

	hKey = 0;
	dwDataSize = dwSize;
	dwType = REG_SZ;
	CString sPath = "SOFTWARE\\Clients\\StartMenuInternet\\";
	sPath += szValue;
	sPath += "\\shell\\open\\command";

	try
	{
		LSTATUS ls = RegOpenKeyEx( HKEY_LOCAL_MACHINE, sPath, NULL, KEY_QUERY_VALUE, &hKey );
		if ( ls != ERROR_SUCCESS ) throw 0;

		ls = RegQueryValueEx( hKey, NULL, NULL, &dwType, (BYTE*)szValue, &dwDataSize );
		if ( ls != ERROR_SUCCESS ) throw 1;

		RegCloseKey( hKey );
	}
	catch (int)
	{
		RegCloseKey( hKey );

		return false;
	}

	return true;
}

bool CEditorApp::IsProcessRunning ( char* szProcess )
{
	/* for whatever reason this can return FALSE, even when app is resident
	CEnumProcess				tmp;
	CEnumProcess::CProcessEntry entry;
	bool						bLoop	 = false;
	bool						bProcess = false;
	
	tmp.SetMethod ( 0x1 );

	// run through all processes
	for ( bLoop = tmp.GetProcessFirst ( &entry ); bLoop; bLoop = tmp.GetProcessNext ( &entry ) )
	{
		// convert the string to lowercase
		_strlwr ( entry.lpFilename );

		// set up our strings
		CString		process				= entry.lpFilename;
		char		szTextANSI [ 255 ]	= "";
		char		seps[ ]				= ".\\//;";
		char*		token;

		// convert the process name from wide to ansi
		ConvertWideToANSI ( NULL, &process, szTextANSI );

		m_Debug.Write ( szTextANSI );

		// get the first token
		token = strtok ( szTextANSI, seps );

		// extract each token
		while ( token != NULL )
		{
			// see if the map editor is in the list
			if ( _strnicmp ( token, szProcess, strlen ( token ) ) == 0 )
			{
				bProcess = true;
			}

			// get the next token
			token = strtok ( NULL, seps );
		}

		if ( bProcess )
			break;
	}
	LPARAM lParam = 0;
	EnumWindows(EnumWindowsProcLee, lParam);
	*/

	// Simply look for class name of desired applications
	bool bProcess = false;
	HWND hWndFind = FindWindow ( szProcess, NULL );
	if ( hWndFind ) bProcess = true;

	return bProcess;
}

void CEditorApp::CheckForDBProApplications ( void )
{
	// check the running processes and determine if either of the
	// db pro applications are not running

	// only fo check once a second
	if ( timeGetTime()-m_dwCheckProcessesTimer > 1000 )
	{
		// check processes 
		m_dwCheckProcessesTimer = timeGetTime();

		// local variables	
		bool bMapEditor = false;	// state of map editor
		m_bGameOrBuildInProgress = false;	// state of game
		
		// check to see if any of these processes are running
		bMapEditor = IsProcessRunning ( "FPSC Map Editor12345" );
		if ( bMapEditor==false ) bMapEditor = IsProcessRunning ( "FPSC-MapEditor" );
		m_bGameOrBuildInProgress = IsProcessRunning ( "FPSC Game12345" );
		if ( m_bGameOrBuildInProgress==false ) m_bGameOrBuildInProgress = IsProcessRunning ( "FPSC-Game" );

		// if neither are running then sleep for a while
		if ( m_bDoNotNeedToCheckForCrashesAnymore==false )
		{
			if ( !bMapEditor && !m_bGameOrBuildInProgress && m_bAppHasStartedRunning==true )
			{
				if ( m_iRecover==0 )
				{
					m_dwRecoverTimer = timeGetTime();
					m_iRecover=1;
				}
				else
				{
					// if no map or game after X seconds, message..
					if ( timeGetTime() - m_dwRecoverTimer > 20000 )
						m_iRecover=2;
				}
			}
			else
			{
				// one app must have started running before drop-out recover code can kick in
				m_bAppHasStartedRunning = true;
				m_iRecover=0;
			}
		}

		// we have waited long enough but something is wrong
		if ( m_iRecover==2 )
		{
			// extra demo information
			char szDebugInfo[1024];
			HWND hWndGame = FindWindow ( "FPSC Game12345", NULL );
			if ( hWndGame==NULL) hWndGame = FindWindow ( "FPSC-Game", NULL );
			HWND hWndMap = FindWindow ( "FPSC Map Editor12345", NULL );
			if ( hWndMap==NULL) hWndMap = FindWindow ( "FPSC-MapEditor", NULL );
			sprintf ( szDebugInfo, " Extra Debug Info: %i %i", hWndGame, hWndMap );
			EnumWindows ( EnumWindowsProcLee, (LPARAM)szDebugInfo );
			m_bEntityFocus = true;
			m_Message.Stop ( );
			char szDebugMessage[2048];
			strcpy ( szDebugMessage, "The application has encountered a problem and needs to close." );
			strcat ( szDebugMessage, szDebugInfo);
			MessageBox ( NULL, _T ( szDebugMessage ), _T ( "Error" ), MB_ICONERROR );
			PostQuitMessage ( 0 );
		}
	}
}

void CEditorApp::UpdatePopup ( void )
{
	// the popup dialog for displaying text information

	// local variables
	DWORD dwPopup = 0;						// popup
	DWORD dwValue = 0;						// reset value
	cIPC* pIPC	  = m_Message.GetIPC ( );	// message pointer

	// check message pointer is valid
	if ( !pIPC )
		return;

	// extract the popup value from offset 750
	pIPC->ReceiveBuffer ( &dwPopup, 750, sizeof ( dwPopup ) );

	// see if we need to display the popup
	if ( dwPopup )
	{
		// set the popup display value to true
		m_bPopupDisplay = true;

		// reset the popup display value
		pIPC->SendBuffer ( &dwValue, 750, sizeof ( dwValue ) );

		// now display the popup dialog
		cPopup pop;
		pIPC->AllowSyncEventNow();
		pop.DoModal ( );

		// finally reset the display value
		m_bPopupDisplay = false;
	}
}

void CEditorApp::UpdateTermination ( void )
{
	// see if we need to terminate the db pro app ( the map editor )
	// and then launch the game executable

	// local variables
	cIPC*	pIPC							= m_Message.GetIPC ( );	// message pointer
	DWORD	dwTerminateDBProApplication		= 0;					// terminate flag
	DWORD	dwValue							= 0;					// reset value
	BYTE	byClearData [ 1024 * 6 ]		= { 0 };
	char	szExeANSI	[ 255 ]				= "";
	char	szParamANSI [ 255 ]				= "";
	char	dataA		[ 255 ]				= "";
	char	dataB		[ 255 ]				= "";
	char	szDebug		[ 255 ]				= "";
	CString szParamUNI						= "";
	CString szExeUNI						= "";

	// check message pointer
	if ( !pIPC )
		return;

	// extract the terminate value from the message data
	pIPC->ReceiveBuffer ( &dwTerminateDBProApplication, 920, sizeof ( dwTerminateDBProApplication ) );

	// now see if we need to terminate the db pro app
	if ( dwTerminateDBProApplication )
	{
		// stop the messages from processing while we're going through
		// this section of code
		m_Message.Stop ( );

		// reset this message to 0
		pIPC->SendBuffer ( &dwValue, 920, sizeof ( dwValue ) );

		// set the focus flag to true
 		m_bFocus = true;

		// output debug information
		theApp.m_Debug.Write ( "\nCALLING GAME" );

		// terminate the db pro app using the handle
		if ( TerminateProcess ( m_DBProApplication.hProcess, 0 ) )
		{
			theApp.m_Debug.Write ( "\nPROCESS TERMINATED" );
		}
		else
		{
			theApp.m_Debug.Write ( "\nFAILED TO TERMINATE PROCESS" );
		}

		// turn off the quit message
		m_pDocumentA->SetQuitMessage ( false );

		// set the current directory of the app
		EnterCriticalSection( &theApp.m_csDirectoryChanges );
		SetCurrentDirectory ( theApp.m_szDirectory );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		// wait till the map editor has completely shut down before
		// attempting to launch the game, this code has helped to resolve
		// errors where the test game would fail
		while ( 1 )
		{
			bool bMapEditor = false;

			bMapEditor = IsProcessRunning ( "FPSC Map Editor12345" );

			if ( !bMapEditor )
				break;
		}

		theApp.m_Debug.Write ( "*** FPSC-mapeditor.exe HAS ENDED" );
		theApp.m_Debug.Write ( "*** NOW GOING TO CALL FPSC-GAME" );

		// get the name of the exe we need to run and it's parameters
		pIPC->ReceiveBuffer ( &dataA [ 0 ], 1000, sizeof ( dataA ) );
		pIPC->ReceiveBuffer ( &dataB [ 0 ], 1256, sizeof ( dataB ) );

		// convert into wide character set
		ConvertANSIToWide ( ( BYTE* ) &dataA [ 0 ], szExeANSI,   &szExeUNI );
		ConvertANSIToWide ( ( BYTE* ) &dataB [ 0 ], szParamANSI, &szParamUNI );

		// print name of executable
		sprintf ( szDebug, "Shell execute - executable - %s", szExeANSI   );
		theApp.m_Debug.Write ( szDebug );

		// print parameters
		sprintf ( szDebug, "Shell execute - parameters - %s", szParamANSI );
		theApp.m_Debug.Write ( szDebug );
		
		// reset recover counter
		theApp.m_iRecover = 0;
		theApp.m_dwRecoverTimer = 0;
		theApp.m_bGameOrBuildInProgress = false;

		// clear the message buffer
		pIPC->SendBuffer ( &byClearData [ 0 ], 0, sizeof ( byClearData ) );

		// reset file map states
		theApp.SetFileMapData (  44, 1 );
		theApp.SetFileMapData ( 486, 1 );
		
		// now attempt to launch the executable
		theApp.m_hInstance = ShellExecute ( NULL, _T ( "open" ), szExeUNI, szParamUNI, NULL, SW_SHOWNORMAL );
		
		// check everything went ok
		if ( ( int ) theApp.m_hInstance <= 32 )
		{
			theApp.m_Debug.Write ( "Shell execute - failed" );
		}
		else
		{
			theApp.m_Debug.Write ( "Shell execute - success" );
		}
		
		// reset member variables
		m_bAppRunning = false;
		m_bInvalidate = true;

		// restart the message checker
		m_Message.Restart ( );
	}
}

void CEditorApp::UpdateQuit	( void )
{
	// see if we need to quit

	// local variables
	cIPC* pIPC	  = theApp.m_Message.GetIPC ( );	// get the message pointer
	DWORD dwCheck = 0;								// check value
	DWORD dwValue = 0;								// reset value

	// return if the pointer is invalid
	if ( !pIPC )
		return;

	// get the value from the buffer
	pIPC->ReceiveBuffer ( &dwCheck, 912, sizeof ( dwCheck ) );

	// if this value is 1 we need to quit
	if ( dwCheck == 1 )
	{
		// output debug information
		m_Debug.Write ( "Message 912 - UpdateQuit" );

		// set the end flag to true so the app knows it
		// can quit in it's loops
		m_bEnd = true;

		// reset the quit value
		pIPC->SendBuffer ( &dwValue, 912, sizeof ( dwValue ) );
	}
}

void CEditorApp::UpdateRestart ( void )
{
	// see if we need to restart the editor
	bool bRepeatAndSleep = true;
	while ( bRepeatAndSleep )
	{
		// only do once by default
		bRepeatAndSleep = false;

		// local variables
		cIPC* pIPC = theApp.m_Message.GetIPC ( );		// get the message handler
		if ( !pIPC ) return;

		// get the value
		DWORD dwCheck	= 0;
		pIPC->ReceiveBuffer ( &dwCheck, 924, sizeof ( dwCheck ) );
		if ( dwCheck == 1 )
		{
			// Set event before possible reload of mapeditor
			pIPC->AllowSyncEventNow();

			// A small pause before launch restart - time for old app(s) to end
			Sleep(1000);

			// TEST GAME Ends Here, so reactivate the stopinterface flag
			g_bStopInterfaceOperations = false;

			// Signal that no apps are supposed to be running, so no drop-out recover error!
			m_bAppHasStartedRunning = false;

			// output debug information
			m_Debug.Write ( "Message 924 - UpdateRestart" );

			// set the restart flag to true
			m_bRestart = true;

			// stop the messages
			m_Message.Stop ( );
		
			// turn off the quit message (so the NewDocument function can reload mapeditor exe)
			m_pDocumentA->SetQuitMessage ( false );
			
			// send a message to the main frame to quit
			if ( pMainFrame )
				SendMessage ( pMainFrame->m_hWnd, WM_CLOSE_LEVEL_WINDOW, 0, 0 );
			
			// reset the check value
			dwCheck = 0;
			pIPC->SendBuffer ( &dwCheck, 924, sizeof ( dwCheck ) );

			// finally restart the message pump
			m_Message.Restart ( );
		}

		// if playing test game, can sleep and stay in here to save CPU cycles from interface activity
		if ( g_bStopInterfaceOperations ) 
		{
			Sleep ( 500 );
			bRepeatAndSleep = true;
			CheckForDBProApplications ( );
		}
	}
}

void CEditorApp::UpdateEntityWindow ( void )
{
	// update the entity window

	// local variables
	cIPC* pIPC		= theApp.m_Message.GetIPC ( );	// get the message pointer
	DWORD dwCheck	= 0;							// check value

	// get the value from the offset
	pIPC->ReceiveBuffer ( &dwCheck, 978, sizeof ( dwCheck ) );

	// act when the value is true
	if ( dwCheck )
	{
		// send a message to the frame to display or hide the entity window
		if ( pMainFrame )
			SendMessage ( pMainFrame->m_hWnd, WM_ENTITY_WINDOW, dwCheck, 0 );
		
		// reset the check value to 0
		dwCheck = 0;

		// update the buffer with the new value
		pIPC->SendBuffer ( &dwCheck, 978, sizeof ( dwCheck ) );
	}
}

void CEditorApp::UpdateBuildGame ( void )
{
	// ipc ptr
	cIPC* pIPC = theApp.m_Message.GetIPC ( );
	if ( !pIPC ) return;

	// if build game selected, go modal!
	DWORD dwCheck = 0;
	pIPC->ReceiveBuffer ( &dwCheck, 758, sizeof ( dwCheck ) );
	if ( dwCheck==2 )
	{
		// reset trigger
		DWORD dwClear = 0;
		pIPC->SendBuffer ( &dwClear, 758, sizeof ( dwClear ) );

		// Launch BUILDGAME dialog
		theApp.m_bBuildGameDisplay = true;
		cBuildGame game;
		pIPC->AllowSyncEventNow();
		game.DoModal ( );
		theApp.m_bBuildGameDisplay = false;
	}
}

void CEditorApp::UpdatePreferences ( void )
{
	// ipc ptr
	cIPC* pIPC = theApp.m_Message.GetIPC ( );
	if ( !pIPC ) return;

	// if prefs selected
	DWORD dwCheck = 0;
	pIPC->ReceiveBuffer ( &dwCheck, 754, sizeof ( dwCheck ) );
	if ( dwCheck==2 )
	{
		// clear flag
		DWORD dwClear = 0;
		pIPC->SendBuffer ( &dwClear, 754, sizeof ( dwCheck ) );

		// launch prefs
		theApp.m_bBuildGameDisplay = true;
		cPreferences pref;
		pIPC->AllowSyncEventNow();
		pref.DoModal ( );
		theApp.m_bBuildGameDisplay = false;
		theApp.SetFileMapData ( 754, 0 );
	}
}

void CEditorApp::UpdateTestGame ( void )
{
	cIPC* pIPC = theApp.m_Message.GetIPC ( );
	if ( !pIPC ) return;

	DWORD dwCheck = 0;
	pIPC->ReceiveBuffer ( &dwCheck, 970, sizeof ( dwCheck ) );
	if ( dwCheck )
	{
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 970, sizeof ( dwValue ) );

		cTestGame testGame;
		pIPC->AllowSyncEventNow();
		testGame.DoModal ( );
	}
}

void CEditorApp::UpdateFPIWizard ( void )
{
	cIPC* pIPC = theApp.m_Message.GetIPC ( );

	DWORD dwCheck = 0;
	pIPC->ReceiveBuffer ( &dwCheck, 990, sizeof ( dwCheck ) );
	if ( dwCheck )
	{
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 990, sizeof ( dwValue ) );
		
		theApp.m_bBuildGameDisplay = true;
		cFPIWizard	wizard;
		pIPC->AllowSyncEventNow();
		wizard.DoModal ( );
		theApp.m_bBuildGameDisplay = false;
	}
}

void CEditorApp::UpdateProject ( void )
{
	cIPC* pIPC = theApp.m_Message.GetIPC ( );

	DWORD dwCheck = 0;

	pIPC->ReceiveBuffer ( &dwCheck, 416, sizeof ( dwCheck ) );

	//memcpy ( &dwCheck, &m_pData [ 416 ], sizeof ( DWORD ) );

	if ( dwCheck == 1 )
	{
		::PostMessage (NULL, WM_MYMESSAGE, 0, 0);

		char szTitle [ 255 ] = "";

		pIPC->ReceiveBuffer ( &szTitle [ 0 ], 1000, sizeof ( szTitle ) );

		//memcpy ( &szTitle [ 0 ], &m_pData [ 1000 ], sizeof ( char ) * 255 );
		CString title = szTitle;

		m_szProjectName = title;

		SendMessage ( pMainFrame->m_hWnd, WM_MYMESSAGE, 0, 0 );

		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 416, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ 416 ], &dwValue, sizeof ( DWORD ) );
	}
}

void CEditorApp::HandleMessages ( void )
{
	// replace this with findwindow (it might be better!)
	CheckForDBProApplications ( );
}


void CEditorApp::OnFileOpen ( )
{
	// 110105
	if ( theApp.m_bTestGameCancel )
		return;

	SetFileMapData ( 400, 1 );
	SetFileMapData ( 912, 0 );
}

void CEditorApp::OnFileNewType ( int iNewType )
{
	// 110105
	//if ( theApp.m_bTestGameCancel )
	//	return;

	// called when a new file is created

	//theApp.m_Debug.Write ( "OnFileNew" );

	//SetCurrentDirectory ( theApp.m_szDirectory );

	// local variables
	CMainFrame* pFrame;							// frame pointer
	TCHAR		szString [ 7 ] [ MAX_PATH ];	// used for storing strings

	{
		// 120105
		//theApp.SetFileMapData ( 416, 1 );
		//theApp.SetFileMapDataString ( 1000, _T ( "Editor" ) );
	}
	

	//theApp.m_Debug.Write ( "OnFileNew - check OnFileNew valid" );

	// check that we're ready to go for creating a new file
	if ( m_bFileNewValid )
	{
		{
			CDocManager*    pDoc		= theApp.m_pDocManager;
			POSITION		pos			= pDoc->GetFirstDocTemplatePosition ( );
			CDocTemplate*	pTemplate	= pDoc->GetNextDocTemplate ( pos );

			SetFileMapData ( 408, iNewType );



			//theApp.m_Debug.Write ( "OnFileNew - check setup frame valid" );

			if ( !m_bSetupFrame )
			{
				
				//theApp.m_Debug.Write ( "OnFileNew - call OpenDocumentFile" );
				
				

				pTemplate->OpenDocumentFile ( NULL );
				

				// update file mappingu
				SetFileMapData ( 200, 1 );
				SetFileMapData ( 204, 0 );

				// get a pointer to the main window
				pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

				// load menu strings for - file, edit, level, view, tools, window, help
				GetPrivateProfileString ( _T ( "Menu" ), _T ( "A" ), _T ( "" ), szString [ 0 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Menu" ), _T ( "B" ), _T ( "" ), szString [ 1 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Menu" ), _T ( "C" ), _T ( "" ), szString [ 2 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Menu" ), _T ( "D" ), _T ( "" ), szString [ 3 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Menu" ), _T ( "E" ), _T ( "" ), szString [ 4 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Menu" ), _T ( "F" ), _T ( "" ), szString [ 5 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Menu" ), _T ( "G" ), _T ( "" ), szString [ 6 ], MAX_PATH, theApp.m_szLanguage );

				// update the buttons with the new text
				for ( int i = 0; i < 7; i++ )
				{
					pFrame->m_wndMenuBar.SetButtonText ( i + 1, szString [ i ] );
				}
				

				// update the menu bar
				pFrame->m_wndMenuBar.AdjustLayout ( );
				

				m_bSetupFrame = true;

				
			}
		}
	}

	//theApp.m_Debug.Write ( "OnFileNew - return" );
}

void CEditorApp::OnFileNew ( )
{
	OnFileNewType ( 1 );
}

int g_iBoysLifeQuit = 0;

void CEditorApp::OnFileNewArena ( )
{
	/*
	// mike - 080806 - for boys life
	char szString [ MAX_PATH ] = "";
	GetPrivateProfileString ( _T ( "Trial" ), _T ( "Message" ), _T ( "" ), szString, MAX_PATH, theApp.m_szLanguage );

	MessageBox ( NULL, szString, "Information", MB_OK );
	*/

	// demo version end splash
		#ifdef FPSC_DEMO
 		cDialogEnd end;
		m_bDoNotNeedToCheckForCrashesAnymore = true;
		g_iBoysLifeQuit = 1;
 		end.DoModal ( );
		g_iBoysLifeQuit = 0;
		#endif

	// identical to FILENEW (mapeditor detect difference from menu ID returned from selection)
	OnFileNewType ( 2 );
}

void CEditorApp::ClearFileMap ( void )
{
	cIPC* pIPC			= m_Message.GetIPC ( );
	BYTE  data [ 6144 ] = "0";

	pIPC->SendBuffer ( &data, 0, sizeof ( data ) );

	//pIPC->ReceiveBuffer ( &dwTitle, 4, sizeof ( dwTitle ) );


	/*
	HANDLE	hFileMap = NULL;	// handle to file map
	LPVOID	lpVoid   = NULL;	// pointer to data
	BYTE*	pData    = NULL;	// byte pointer to data

	CString string = "FPSEXCHANGE";

	// attempt to open the file map
	hFileMap = OpenFileMapping ( FILE_MAP_ALL_ACCESS, TRUE, string );

	// silent failure
	if ( !hFileMap )
		return;

	// get the data contained within the file map
	lpVoid = MapViewOfFile ( hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0 );

	//lpVoid = theApp.m_pfnGetFileMapData ( "FPSEXCHANGE" );

	// fail if invalid data
	if ( !lpVoid )
		return;

	// cast void pointer to byte pointer
	pData = ( BYTE* ) lpVoid;

	memset ( &pData [ 0 ], 0, 1024 );

	// now unmap the file and close the handle
	UnmapViewOfFile ( lpVoid );
	CloseHandle     ( hFileMap );
	*/
}

DWORD CEditorApp::GetFileMapData ( int iOffset )
{
	cIPC* pIPC	  = m_Message.GetIPC ( );
	DWORD dwValue = 0;
	pIPC->ReceiveBuffer ( &dwValue, iOffset, sizeof ( dwValue ) );
	return dwValue;
}

void CEditorApp::SetFileMapData ( int iOffset, DWORD dwData )
{
	cIPC* pIPC	  = m_Message.GetIPC ( );
	pIPC->SendBuffer ( &dwData, iOffset, sizeof ( dwData ) );
}

void CEditorApp::SetFileMapDataString ( int iOffset, LPTSTR pString )
{
	cIPC* pIPC	  = m_Message.GetIPC ( );

	char szDestination [ 255 ] = "";
	
	pIPC->SendBuffer ( &szDestination, iOffset, sizeof ( szDestination ) );
		
	strcpy ( szDestination, pString );
		
	pIPC->SendBuffer ( &szDestination, iOffset, sizeof ( szDestination ) );

	/*
	// set the file map data at the specified offset with the given data value

	// local variables
	HANDLE	hFileMap = NULL;	// handle to file map	
	LPVOID	lpVoid   = NULL;	// pointer to data
	BYTE*	pData    = NULL;	// byte pointer to data

	CString string = "FPSEXCHANGE";

	// attempt to open the file map
	hFileMap = OpenFileMapping ( FILE_MAP_ALL_ACCESS, TRUE, string );
	
	// silent failure
	if ( !hFileMap )
		return;

	// get the data contained within the file map
	lpVoid = MapViewOfFile ( hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0 );

	//lpVoid = theApp.m_pfnGetFileMapData ( "FPSEXCHANGE" );

	// fail if invalid data
	if ( !lpVoid )
		return;

	// cast void pointer to byte pointer
	pData = ( BYTE* ) lpVoid;

	

	{
		char szDestination [ MAX_PATH ];
		//int  iLen     = wcslen ( pString );
		//int  nResult = WideCharToMultiByte ( CP_ACP, 0, pString, -1, szDestination, iLen * sizeof ( CHAR ), NULL, NULL );

		strcpy ( szDestination, pString );
		
		//szDestination [ iLen ] = 0;
	
		memset ( &pData [ iOffset ], 0, sizeof ( char ) * 255 );

		memcpy ( &pData [ iOffset ], szDestination, sizeof ( char ) * strlen ( szDestination ) );
		
		int c = 0;
	}

	// now unmap the file and close the handle
	UnmapViewOfFile ( lpVoid );
	CloseHandle     ( hFileMap );
	*/
}

bool g_bMouseChange = false;

void CEditorApp::UpdateMouse ( void )
{
	// get mouse visibility flag
	cIPC* pIPC	  = m_Message.GetIPC ( );

	DWORD dwValue = 0;
	pIPC->ReceiveBuffer ( &dwValue, 44, sizeof ( dwValue ) );

	if ( dwValue == 1 )
		m_bDisplayMouse = TRUE;
	else
		m_bDisplayMouse = FALSE;

	DWORD dwControlMouse = 0;
	DWORD dwNewX		 = 0;
	DWORD dwNewY		 = 0;

	pIPC->ReceiveBuffer ( &dwControlMouse, 48, sizeof ( dwControlMouse ) );
	pIPC->ReceiveBuffer ( &dwNewX,         52, sizeof ( dwNewX         ) );
	pIPC->ReceiveBuffer ( &dwNewY,         56, sizeof ( dwNewY         ) );

	DWORD dwWidth  = 0;
	DWORD dwHeight = 0;

	pIPC->ReceiveBuffer ( &dwWidth,  704, sizeof ( dwWidth  ) );
	pIPC->ReceiveBuffer ( &dwHeight, 708, sizeof ( dwHeight ) );

	if ( dwControlMouse == 1 )
	{
		RECT rect;
		
		m_pEditorView->GetClientRect  ( &rect );
		m_pEditorView->ClientToScreen ( &rect );

		POINT point;
		GetCursorPos ( &point );

		m_lastPoint.x = rect.left + ( dwWidth / 2 );
		m_lastPoint.y = rect.top + ( dwHeight / 2 );

		POINT newPoint;

		newPoint.x = point.x - m_lastPoint.x;
		newPoint.y = point.y - m_lastPoint.y;
		
		theApp.SetFileMapData (  8, newPoint.x );
		theApp.SetFileMapData ( 12, newPoint.y );
		
		SetCursorPos ( rect.left + ( dwWidth / 2 ), rect.top + ( dwHeight / 2 ) );

		// 120105
		if ( g_bMouseChange == false )
		{
			ShowCursor ( FALSE );
			g_bMouseChange = true;
		}

		m_bDisplayMouse = FALSE;
		m_bDisplayMouseInQuickPreview = true;
	}
	else
	{
		if ( g_bMouseChange )
		{
			ShowCursor ( TRUE );
			g_bMouseChange = false;
		}
		m_bDisplayMouseInQuickPreview = FALSE;
	}
}

void CEditorApp::UpdateToolBar ( void )
{
	/*
	DWORD		dwUndo	= ( DWORD ) m_pData [ 466 ];
	DWORD		dwRedo	= ( DWORD ) m_pData [ 470 ];
	DWORD		dwCut	= ( DWORD ) m_pData [ 474 ];
	DWORD		dwCopy	= ( DWORD ) m_pData [ 478 ];
	DWORD		dwClear	= ( DWORD ) m_pData [ 482 ];
	DWORD		dwBuildGame	= ( DWORD ) m_pData [ 486 ];
	*/

	cIPC* pIPC = m_Message.GetIPC ( );

	DWORD dwUndo		= 0;
	DWORD dwRedo		= 0;
	DWORD dwCut			= 0;
	DWORD dwCopy		= 0;
	DWORD dwClear		= 0;
	DWORD dwBuildGame	= 0;

	pIPC->ReceiveBuffer ( &dwUndo,      466, sizeof ( dwUndo      ) );
	pIPC->ReceiveBuffer ( &dwRedo,      470, sizeof ( dwRedo      ) );
	pIPC->ReceiveBuffer ( &dwCut,       474, sizeof ( dwCut       ) );
	pIPC->ReceiveBuffer ( &dwCopy,		478, sizeof ( dwCopy      ) );
	pIPC->ReceiveBuffer ( &dwClear,     482, sizeof ( dwClear     ) );
	pIPC->ReceiveBuffer ( &dwBuildGame, 486, sizeof ( dwBuildGame ) );

	if ( dwUndo )
		m_bTest [ 0 ] = true;
	else
		m_bTest [ 0 ] = false;
	
	if ( dwRedo )
		m_bTest [ 1 ] = true;
	else
		m_bTest [ 1 ] = false;

	if ( dwCut )
		m_bTest [ 2 ] = true;
	else
		m_bTest [ 2 ] = false;
	
	if ( dwCopy )
		m_bTest [ 3 ] = true;
	else
		m_bTest [ 3 ] = false;

	if ( dwClear )
		m_bTest [ 4 ] = true;
	else
		m_bTest [ 4 ] = false;

	if ( dwBuildGame )
		m_bTest [ 5 ] = true;
	else
		m_bTest [ 5 ] = false;


	// update the toolbar

	//DWORD		dwToolbar	= ( DWORD ) m_pData [ 300 ];
	//DWORD		dwButton	= ( DWORD ) m_pData [ 304 ];
	//DWORD		dwState		= ( DWORD ) m_pData [ 308 ];

	DWORD		dwToolbar	= 0;
	DWORD		dwButton	= 0;
	DWORD		dwState		= 0;

	pIPC->ReceiveBuffer ( &dwToolbar, 300, sizeof ( dwToolbar ) );
	pIPC->ReceiveBuffer ( &dwButton,  304, sizeof ( dwButton  ) );
	pIPC->ReceiveBuffer ( &dwState,   308, sizeof ( dwState   ) );

	CMainFrame* pFrame		= NULL;

	if ( dwCut || dwCopy || dwClear )
	{
		
	}

	if ( dwToolbar > 0 && dwToolbar < 10 )
	{
		pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

		if ( dwToolbar == 1 )
		{
		
		}

		if ( pFrame )
		{
			pFrame->SetToolbarButtonState ( dwToolbar, dwButton, dwState );

			/////////////
			DWORD dwValue = 0;

			pIPC->SendBuffer ( &dwValue, 300, sizeof ( dwValue ) );

			//memcpy ( &m_pData [ 300 ], &dwValue, sizeof ( DWORD ) );
			/////////////

			//m_pData [ 300 ] = 0;
		}
	}

	if ( dwToolbar == 10 )
	{
		pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

		//m_pData [ 300 ] = 0;

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 300, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ 300 ], &dwValue, sizeof ( DWORD ) );
		/////////////
	}

	
	{
		DWORD dwDisable = 0;

		//memcpy ( &dwDisable, &m_pData [ 850 ], sizeof ( DWORD ) );

		pIPC->ReceiveBuffer ( &dwDisable, 850, sizeof ( dwDisable ) );

		if ( dwDisable )
		{
			//theApp.m_Debug.Write ( "HandleMessages - message 850" );
			//theApp.m_Debug.Write ( "HandleMessages - disable icons" );

			m_bDisable = false;

			m_bTest [ 4 ] = false;
			m_bTest [ 3 ] = false;
			m_bTest [ 0 ] = false;
			m_bTest [ 2 ] = false;

			//theApp.m_Debug.Write ( "HandleMessages - message 850 - end" );

		}
		else
			m_bDisable = true;
	}
}

void CEditorApp::UpdateStatusBar ( void )
{
	// status bar
	cIPC* pIPC = m_Message.GetIPC ( );

	//SetCurrentDirectory ( theApp.m_szDirectory );

	DWORD			dwLayer = 0;
	DWORD			dwBrush = 0;
	pIPC->ReceiveBuffer ( &dwLayer, 600, sizeof ( dwLayer ) );
	pIPC->ReceiveBuffer ( &dwBrush, 612, sizeof ( dwBrush ) );

	CMainFrame*		pFrame  = ( CMainFrame* ) theApp.m_pMainWnd;

	//SetCurrentDirectory ( theApp.m_szDirectory );

	theApp.pMainFrame->OnMouseMoveXX ( 0, CPoint ( 0, 0 ) );


}

void CEditorApp::UpdateLibraryWindow ( void )
{
	// library window
	cIPC* pIPC = m_Message.GetIPC ( );

	DWORD dwAddImage			= 0;
	DWORD dwDeleteImage			= 0;
	DWORD dwLibraryTab			= 0;
	DWORD dwDeleteImageIndex	= 0;
	DWORD dwActiveTab        	= 0;
	DWORD dwScroll           	= 0;
	DWORD dwDeleteAllImages		= 0;

	pIPC->ReceiveBuffer ( &dwAddImage,			500, sizeof ( dwAddImage			) );
	pIPC->ReceiveBuffer ( &dwDeleteImage,		504, sizeof ( dwDeleteImage			) );
	pIPC->ReceiveBuffer ( &dwLibraryTab,		508, sizeof ( dwLibraryTab			) );
	pIPC->ReceiveBuffer ( &dwDeleteImageIndex,	512, sizeof ( dwDeleteImageIndex	) );
	pIPC->ReceiveBuffer ( &dwActiveTab,			534, sizeof ( dwActiveTab			) );
	pIPC->ReceiveBuffer ( &dwScroll,			538, sizeof ( dwScroll				) );
	pIPC->ReceiveBuffer ( &dwDeleteAllImages,	542, sizeof ( dwDeleteAllImages		) );

	TCHAR szImageFileName [ 255 ];
	TCHAR szImageName     [ 255 ];

	EnterCriticalSection( &m_csDirectoryChanges );

	SetCurrentDirectory ( theApp.m_szDirectory );

	// add an image to library
	if ( dwAddImage == 1 )
	{
		char szA [ 255 ];
		char szB [ 255 ];

		pIPC->ReceiveBuffer ( &szA [ 0 ], 1000, sizeof ( szA ) );
		pIPC->ReceiveBuffer ( &szB [ 0 ], 1256, sizeof ( szB ) );

		CString a = szA;
		CString b;

		if ( strlen(szB) > 0 ) b = szB;
		else
		{
			strcpy_s( szB, 255, szA );

			char *szSlash = strrchr( szB, '\\' );
			if ( szSlash ) szSlash++;
			else szSlash = szB;

			char *szDot = strrchr( szSlash, '.' );
			if ( szDot ) *szDot = '\0';

			b = szSlash;
		}
		
		CMainFrame* pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

		strcpy ( szImageFileName, a );
		strcpy ( szImageName, b );
		pFrame->AddBitmapToWorkspace ( dwLibraryTab, &szImageFileName [ 0 ], &szImageName [ 0 ] );

		// reset string
		strcpy ( szImageName, _T ( "" ) );
		SetFileMapDataString ( 1000, szImageName );
		SetFileMapDataString ( 1256, szImageName );

		// reset
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 500, sizeof ( dwValue ) );
	}

	LeaveCriticalSection( &m_csDirectoryChanges );

	// remove image from library
	if ( dwDeleteImage == 1 )
	{
		CMainFrame* pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

		pFrame->RemoveBitmapFromWorkspace ( dwLibraryTab, dwDeleteImageIndex );

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 504, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ 504 ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pData [ 504 ] = 0;
	}

	if ( dwActiveTab )
	{
		CMainFrame* pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

		pFrame->m_wndOutput.m_NewBrowser.m_tabCtrl.ActivateSSLPage ( dwActiveTab - 1 );

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 534, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ 534 ], &dwValue, sizeof ( DWORD ) );
		/////////////
		
		//m_pData [ 534 ] = 0;
	}

	if ( dwScroll )
	{
		CMainFrame* pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 538, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ 538 ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pData [ 538 ] = 0;
	}

	if ( dwDeleteAllImages )
	{
		CMainFrame* pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

		pFrame->m_wndOutput.m_NewBrowser.RemoveAllBitmaps ( dwActiveTab );

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 542, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ 542 ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pData [ 542 ] = 0;
	}
}

void CEditorApp::UpdateBrowserWindow ( void )
{
	cIPC* pIPC = m_Message.GetIPC ( );

	DWORD dwRun = 0;

	pIPC->ReceiveBuffer ( &dwRun, 800, sizeof ( dwRun ) );

	//DWORD dwRun = ( DWORD ) m_pData [ 800 ];

	if ( dwRun == 1 )
	{
		 m_Message.Stop ( );

		 {
			 // 250105
			
				CMainFrame* pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

				pFrame->m_wndMenuBar.ResetAll ( );

				//pFrame->m_wndMenuBar.SetMaximizeMode (FALSE);

				//pFrame->m_wndMenuBar.Invalidate ();
				//pFrame->m_wndMenuBar.UpdateWindow ();

				//pFrame->m_wndMenuBar.RestoreMaximizeMode ( );

				//CBCGToolbarButton* pButton = pFrame->m_wndMenuBar.GetMenuItem ( 0 );
				//pButton->m_strText = "mike";
		 }

		cLibraryWindow	library;
		pIPC->AllowSyncEventNow();
		library.DoModal ( );
		m_Message.Restart ( );

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 800, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ 800 ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pData [ 800 ] = 0;
	}
}


extern WPARAM  wParam1;
BOOL CALLBACK EnumWindowsProc1(HWND hwnd,LPARAM lParam );


void CEditorApp::UpdateFileOpenAndSave ( void )
{
	cIPC* pIPC = m_Message.GetIPC ( );

	DWORD dwOpen	 = 0;
	DWORD dwSave	 = 0;
	DWORD dwFileList = 0;

	pIPC->ReceiveBuffer ( &dwOpen,	   424, sizeof ( dwOpen	    ) );
	pIPC->ReceiveBuffer ( &dwSave,	   428, sizeof ( dwSave	    ) );
	pIPC->ReceiveBuffer ( &dwFileList, 438, sizeof ( dwFileList ) );

	//DWORD dwOpen = ( DWORD ) m_pData [ 424 ];
	//DWORD dwSave = ( DWORD ) m_pData [ 428 ];
	//DWORD dwFileList = ( DWORD ) m_pData [ 438 ];

	char szA [ 255 ];
	char szB [ 255 ];
	char szC [ 255 ];
	
	pIPC->ReceiveBuffer ( &szA [ 0 ], 1000, sizeof ( szA ) );
	pIPC->ReceiveBuffer ( &szB [ 0 ], 1256, sizeof ( szB ) );
	pIPC->ReceiveBuffer ( &szC [ 0 ], 1512, sizeof ( szC ) );

	//memcpy ( &szA [ 0 ], &m_pData [ 1000 ], sizeof ( char ) * 255 );
	//memcpy ( &szB [ 0 ], &m_pData [ 1256 ], sizeof ( char ) * 255 );
	//memcpy ( &szC [ 0 ], &m_pData [ 1512 ], sizeof ( char ) * 255 );
	
	CString szDirectory = szA;
	CString szFilter    = szB;
	CString szTitle     = szC;

	if ( dwFileList == 1 )
	{
		AddToRecentFileList ( szDirectory );

		BYTE data [ 1000 ] = { 0 };

		pIPC->SendBuffer ( &data, 1000, sizeof ( data ) );

		//memset ( &m_pData [ 1000 ], 0, sizeof ( char ) * 255 );

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 438, sizeof ( dwValue ) );

		//memcpy ( &m_pData [ 438 ], &dwValue, sizeof ( DWORD ) );
		/////////////

	}

	if ( dwOpen == 1 || dwSave == 1 )
	{
		//theApp.m_Debug.Write ( "UpdateFileOpenAndSave" );

		bool bType = true;

		if ( dwSave )
			bType = false;

		m_bFocus = true;
		
		m_Message.Stop ( );

		TCHAR				szFile [ 260 ];
		CString				newName;
		cNewCustomFileDialog	dialog ( bType, szDirectory, szTitle, szFilter );

	

		pIPC->AllowSyncEventNow();
		if ( dialog.DoModal ( ) == IDOK )
		{
			//theApp.m_Debug.Write ( "UpdateFileOpenAndSave - okay button clicked" );

			newName = dialog.GetPathName ( );
		}
		else
		{
			//theApp.m_Debug.Write ( "UpdateFileOpenAndSave - cancel button clicked" );
		}
		
		// 170105
		//wcscpy ( szFile, newName );
		strcpy ( szFile, newName );

		//theApp.m_Debug.Write ( "UpdateFileOpenAndSave - set file map string" );
		
		EnterCriticalSection( &m_csDirectoryChanges );

		SetCurrentDirectory  ( theApp.m_szDirectory );
		SetFileMapDataString ( 1000, szFile );

		//theApp.m_Debug.Write ( "UpdateFileOpenAndSave - reset file map message" );

		if ( dwOpen )
		{
			DWORD dwValue = 0;
			//memcpy ( &m_pData [ 424 ], &dwValue, sizeof ( DWORD ) );
			
			pIPC->SendBuffer ( &dwValue, 424, sizeof ( dwValue ) );

		}

		if ( dwSave )
		{
			/////////////
			DWORD dwValue = 0;
			//memcpy ( &m_pData [ 428 ], &dwValue, sizeof ( DWORD ) );

			pIPC->SendBuffer ( &dwValue, 428, sizeof ( dwValue ) );

			/////////////

			
		}

		LeaveCriticalSection( &m_csDirectoryChanges );

		//theApp.m_Debug.Write ( "UpdateFileOpenAndSave - restart messages" );

		m_Message.Restart ( );

		m_bFocus = false;

	}

}

void CEditorApp::UpdateMessageBoxes ( void )
{
	cIPC* pIPC = m_Message.GetIPC ( );

	char szMessage [ 255 ];
	char szTitle   [ 255 ];
	pIPC->ReceiveBuffer ( &szMessage [ 0 ], 1000, sizeof ( szMessage ) );
	pIPC->ReceiveBuffer ( &szTitle   [ 0 ], 1256, sizeof ( szTitle   ) );

	DWORD dwDisplay = 0;
	pIPC->ReceiveBuffer ( &dwDisplay,        900, sizeof ( dwDisplay ) );
	if ( dwDisplay != 0 )
	{
		// yes no dialog
		m_Message.Stop ( );
		CString message = szMessage;
		CString title   = szTitle;
		m_bFocus = true;
		int iID = 0;
		if ( dwDisplay == 1 ) iID = ::MessageBox ( NULL, message, title, MB_YESNO | MB_ICONQUESTION );
		if ( dwDisplay == 2 ) iID = ::MessageBox ( NULL, message, title, MB_YESNOCANCEL | MB_ICONQUESTION );
		m_bFocus = false;
		m_Message.Restart ( );

		// yes no
		if ( iID == IDYES )
		{
			DWORD dwValue = 1;
			pIPC->SendBuffer ( &dwValue, 904, sizeof ( dwValue ) );
		}
		if ( iID == IDNO )
		{
			DWORD dwValue = 0;
			pIPC->SendBuffer ( &dwValue, 904, sizeof ( dwValue ) );
		}
		// cancel
		if ( dwDisplay == 2 )
		{
			if ( iID == IDCANCEL )
			{
				DWORD dwValue = 2;
				pIPC->SendBuffer ( &dwValue, 904, sizeof ( dwValue ) );
			}
		}

		// reset flag
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, 900, sizeof ( dwValue ) );
	}
}

void CEditorApp::LoadSettings ( void )
{
	TCHAR szTrace      [ MAX_PATH ];
	TCHAR szProcess    [ MAX_PATH ];
	TCHAR szActivation [ MAX_PATH ];

	m_bDebugTrace      = false;
	m_bDebugProcess    = false;
	m_bDebugActivation = false;

	GetPrivateProfileString ( _T ( "Debug" ), _T ( "Trace"      ), _T ( "" ), szTrace,      MAX_PATH, theApp.m_szLanguage );
	GetPrivateProfileString ( _T ( "Debug" ), _T ( "Process"    ), _T ( "" ), szProcess,    MAX_PATH, theApp.m_szLanguage );
	GetPrivateProfileString ( _T ( "Debug" ), _T ( "Activation" ), _T ( "" ), szActivation, MAX_PATH, theApp.m_szLanguage );

	if ( strcmp ( szTrace, "1" ) == 0 )
		m_bDebugTrace = true;

	if ( strcmp ( szProcess, "1" ) == 0 )
		m_bDebugProcess = true;

	if ( strcmp ( szActivation, "1" ) == 0 )
		m_bDebugActivation = true;
}

void CEditorApp::LoadLanguage ( void )
{	
	TCHAR szLanguage [ MAX_PATH ];
	TCHAR szPath     [ MAX_PATH ];

	// 220508 - needs a language, english is default
	GetPrivateProfileString ( _T ( "LOCALIZATION" ), _T ( "language" ), _T ( "" ), szLanguage, MAX_PATH, _T ( "./userdetails.ini" ) );
	if ( strlen ( szLanguage ) < 1 ) strcpy ( szLanguage, "english" );

	//store the language name to pass to PHP
	strcpy_s( theApp.m_szLanguageName, 40, szLanguage );

	strcpy ( szPath, _T ( "./files/languagebank/" ) );	//Tab3.cpp::OnLoadClicked() relies on the exact format of this string
	strcat ( szPath, szLanguage );
	strcpy ( theApp.m_szLanguagePath, szPath );
	strcat ( szPath, "/textfiles" );
	strcat ( szPath, "/fpsci-050.ini" );

	strcpy ( theApp.m_szLanguage, theApp.m_szDirectory );		// LANGCHANGE - change m_szLanguage to an absolute path
	strcat ( theApp.m_szLanguage, "\\" );
	strcat ( theApp.m_szLanguage, (szPath+2) );

	GetPrivateProfileString( _T( "Getting Started" ), _T( "Error" ), _T(""), m_szErrorString, 64, theApp.m_szLanguage );	// LANGCHANGE - pre-load frequently used string
}

bool CEditorApp::CheckForMultipleInstances ( void )
{
	if ( m_bDebugProcess == false )
		return false;

	theApp.m_Debug.Write ( "Testing for multiple versions" );

	HANDLE hSnapShot = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 );

	PROCESSENTRY32 processInfo;
	processInfo.dwSize=sizeof(PROCESSENTRY32);
	int index=0;

	int iCount = 0;

	while(Process32Next(hSnapShot,&processInfo)!=FALSE)
	{
		// 170105
		//if ( wcscmp ( processInfo.szExeFile, _T ( "FPSCreator.exe" ) ) == 0 )
		if ( strcmp ( processInfo.szExeFile, _T ( "FPSCreator.exe" ) ) == 0 )
		{
			iCount++;
			//CloseHandle(hSnapShot);
			//return FALSE;
		}
	}

	CloseHandle(hSnapShot);

	if ( iCount > 1 )
	{
		PostQuitMessage ( 0 );
		return TRUE;

		//Sleep ( 1000 );
	}

	return FALSE;


theApp.m_Debug.Write ( "None found - okay to proceed" );
}

BOOL CEditorApp::InitInstance ( )
{
	// V105 - 220107 - Detect DirectX OCTOBER 2006 or later, or else editor does not work
	bool bQuitEarly = false;
	char pWinDir [ _MAX_PATH ];
	GetWindowsDirectory ( pWinDir, _MAX_PATH );
	if ( strlen ( pWinDir ) > 0 )
	{
		strcat ( pWinDir, "\\system32\\d3dx9_31.dll" );
		OFSTRUCT os;
		if ( OpenFile ( pWinDir, &os, OF_EXIST )==HFILE_ERROR )
		{
			MessageBox ( NULL, GetLanguageData( _T("TGC Store"), _T("DXVersionError") ), "Latest DirectX Not Installed", MB_OK );
			bQuitEarly = true;
		}
	}
	if ( bQuitEarly==true )
		return false;

	// application start up

	// only create if no mutex exists for the FPSC INTERFACE
	m_hOnlyOneEditorMutex = OpenMutex ( MUTEX_ALL_ACCESS, FALSE, "THERECANBEONLYONEFPSCREATOR" );
	if ( m_hOnlyOneEditorMutex == NULL )
	{
		m_hOnlyOneEditorMutex = CreateMutex (NULL, FALSE, "THERECANBEONLYONEFPSCREATOR" );
		if ( m_hOnlyOneEditorMutex == NULL )
			return FALSE; // failed to create mutex for editor
	}
	else
		return FALSE; // editor already present in OS

	// debug info
	theApp.m_Debug.Write ( "InitInstance..." );

	// local variables
	CCommandLineInfo	cmdInfo;	// command line infor

	// LANGCHANGE - moved directory grab to ahead of language loading, allowing the absolute language path to be generated
	{
		theApp.m_Debug.Write ( "Getting current directory..." );

		char szFile [ 256 ] = "";
		char szEXE  [ 256 ] = "";
		char szDir  [ 256 ] = "";

		HMODULE mod = GetModuleHandle ( "fpscreator.exe" );
		GetModuleFileName ( mod, szFile, 256 );

		int c = 0;

		for ( int i = strlen ( szFile ); i > 0; i--, c++ )
		{
			if ( szFile [ i ] == '\\' || szFile [ i ] == '/' )
			{
				//memcpy ( szEXE, &szFile [ i + 1 ], ( strlen ( szFile ) - i ) * sizeof ( char ) );

				memcpy ( szDir, &szFile, ( strlen ( szFile ) - c ) * sizeof ( char ) );

				break;
			}
		}

		theApp.m_Debug.Write ( szDir );

		SetCurrentDirectory ( szDir );

		
		strcpy ( m_szDirectory, szDir );

		i = 0;
	}

	theApp.m_Debug.Write ( "Loading language data..." );

	// load the language file
	LoadLanguage ( );

	LoadSettings ( );

	m_Debug.Start ( "fpscreator.log" );

	if ( CheckForMultipleInstances ( ) )
		return TRUE;


	m_bAppRunning = false;
	m_bSetupFrame = false;

	{
		
		//GetCurrentDirectory ( MAX_PATH, m_szDirectory );

		/*
		// 170105
		{
			char szDestination [ MAX_PATH ];
			int  iLen     = wcslen ( m_szDirectory );
			int  nResult = WideCharToMultiByte ( CP_ACP, 0, m_szDirectory, -1, szDestination, iLen * sizeof ( CHAR ), NULL, NULL );
			szDestination [ iLen ] = 0;
			int c = 0;
		}
		*/
	}

	theApp.m_Debug.Write ( "Starting file map..." );

//	m_Message.Start ( _T ( "FPSEXCHANGE" ), 6144, 10, this );
	m_Message.Start ( _T ( "FPSEXCHANGE" ), 6144, 1, this );

	
	SetFileMapData ( 44, 1 );

	SetFileMapData ( 486, 1 );

	SetFileMapData ( 850, 0 );

	// set the visual look of the application
	CBCGVisualManager::SetDefaultManager ( RUNTIME_CLASS ( CBCGVisualManagerXP ) );

	// set file valid flag to false
	m_bFileNewValid = false;

	theApp.m_Debug.Write ( "Parse command line..." );

	// parse the command line
	ParseCommandLine ( cmdInfo );

	// show the splash screen
	//CSplashWnd::EnableSplashScreen ( cmdInfo.m_bShowSplash );

	theApp.m_Debug.Write ( "AfxOleInit..." );

	// initialize OLE libraries
	if ( !AfxOleInit ( ) )
	{
		AfxMessageBox ( IDP_OLE_INIT_FAILED );
		return FALSE;
	}

	theApp.m_Debug.Write ( "AfxEnableControlContainer..." );

	// set up container and enable 3D controls
	AfxEnableControlContainer ( );
	//Enable3dControls          ( ); .NET depreciated

	theApp.m_Debug.Write ( "SetRegistryKey..." );

	// registry settings
	// 140105
	SetRegistryKey         ( _T ( "FPSCreator" ) );
	LoadStdProfileSettings ( );
	SetRegistryBase        ( _T ( "Settings" ) );

	theApp.m_Debug.Write ( "EnableUserTools..." );

	// enable user defined tools
	EnableUserTools ( ID_TOOLS_ENTRY, ID_USER_TOOL1, ID_USER_TOOL10 );

	theApp.m_Debug.Write ( "Main Init..." );

	// set up managers
	InitMouseManager       ( );
	InitContextMenuManager ( );
	InitKeyboardManager    ( );
//	InitSkinManager        ( );
//	// set up skin manager
//	GetSkinManager ( )->EnableSkinsDownload ( _T ( "http://www.bcgsoft.com/Skins" ) );

	// register the application's document templates, document templates
	// serve as the connection between documents, frame windows and views

	theApp.m_Debug.Write ( "CMultiDocTemplate..." );

	CMultiDocTemplate* pDocTemplate;

	pDocTemplate = new CMultiDocTemplate (
											IDR_EDITORTYPE,
											RUNTIME_CLASS ( CEditorDoc  ),
											RUNTIME_CLASS ( CChildFrame ),
											RUNTIME_CLASS ( CEditorView )
										 );

	pDocTemplate->SetContainerInfo ( IDR_EDITORTYPE_CNTR_IP );

	AddDocTemplate ( pDocTemplate );

	// create main MDI Frame window
	pMainFrame = new CMainFrame;

	theApp.m_Debug.Write ( "LoadFrame..." );
	
	if ( !pMainFrame->LoadFrame ( IDR_MAINFRAME ) )
		return FALSE;

	m_pMainWnd = pMainFrame;

	// enable drag / drop open
	m_pMainWnd->DragAcceptFiles ( );

	// enable DDE execute open
	// 140105
	//EnableShellOpen        ( );
	//RegisterShellFileTypes ( TRUE );

	theApp.m_Debug.Write ( "ProcessShellCommand..." );

	// dispatch commands specified on the command line
	if ( !ProcessShellCommand ( cmdInfo ) )
		return FALSE;

	m_nCmdShow = SW_SHOWMAXIMIZED;

	// rhe main window has been initialized, so show and update it
	pMainFrame->ShowWindow   ( m_nCmdShow );
	pMainFrame->UpdateWindow ( );

	m_bFileNewValid = true;

	theApp.m_Debug.Write ( "OnFileNew..." );

	// automatically start a new project
	OnFileNew ( );

	theApp.m_Debug.Write ( "DoModal..." );

	cFront front;
	theApp.m_Message.GetIPC()->AllowSyncEventNow();
	front.DoModal ( );	

	theApp.m_Debug.Write ( "App Started..." );

//#ifdef AUTOUPDATE
	//AutoUpdate
	DWORD dwCheck = 0;
	
	if ( !GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "check_x9_updates_on_startup", &dwCheck ) ) dwCheck = 0;

	if ( dwCheck )
	{
		if ( m_pUpdateThread )	
		{
			m_pUpdateThread->CancelDownload( );
			m_pUpdateThread->Join( );
			delete m_pUpdateThread;
		}

		m_pUpdateThread = new UpdateCheckThread();
		m_pUpdateThread->SetStartUpCheck( );
		m_pUpdateThread->Start( );

		GetMainWnd( )->SetTimer( 1, 1000, NULL );
	}
	//AutoUpdate - end
//#endif

	strcpy_s( m_szVersion, 10, "" );

	FILE *pFile = NULL;
	int error = _tfopen_s( &pFile, _T("version.ini"), _T("rb") );
	if ( !error )
	{
		fgets( m_szVersion, 10, pFile );
		fclose( pFile );
		pFile = NULL;
	}

#ifdef TGC_STORE
	char szUsername [ 256 ] = "";
	if ( theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "username", szUsername, 256 ) )
	{
		m_bSaveUsername = true;
		m_sUsername = szUsername;
	}
	else
	{
		m_bSaveUsername = false;
		m_sUsername = "";
	}


	char szPassword [ 256 ] = "";
	if ( theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "password", szPassword, 256 ) )
	{
		char *szDecrypt1 = theApp.Decrypt( szPassword );
		char *szDecrypt2 = theApp.Decrypt( szDecrypt1 );

		m_bSavePassword = true;
		m_sPassword = szDecrypt2;

		delete [] szDecrypt1;
		delete [] szDecrypt2;
	}
	else
	{
		m_bSavePassword = false;
		m_sPassword = "";
	}

	DWORD dwFirstRun = 1;
	if ( !theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "first_run", &dwFirstRun ) )
	{
		m_bFirstRun = true;

		theApp.SetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "first_run", (DWORD) 0 );
	}
	else 
	{
		m_bFirstRun = false;
	}
#endif

#ifdef HELPWIZARD
	//HELPW - create and start the help wizard
	if ( m_pHelpWizardDlg )
	{
		m_pHelpWizardDlg->Terminate();
		delete m_pHelpWizardDlg;
		m_pHelpWizardDlg = NULL;
	}

	/*
	if ( m_bFirstRun )
	{
		theApp.m_bHelpWizardVisible = true;
		theApp.m_pHelpWizardDlg = new CHelpWizard();
		theApp.m_pHelpWizardDlg->Start( );
		
		//theApp.m_pHelpWizardDlg->DoModal();
		//theApp.m_bHelpWizardVisible = false;
	}
	*/
	//HELPW - end
#endif 

#ifdef TGC_STORE
	DWORD dwState = 0;
	BOOL bOnline = InternetGetConnectedState( &dwState, 0 );

	//open the store in the free version if online
	/*
	if ( g_bFreeVersion && bOnline )
	{
		char str [ 256 ];
		theApp.GetMainWnd()->GetWindowText( str, 256 );

		theApp.m_iAppX = 0;
		theApp.m_iAppY = 0;
		theApp.m_iAppWidth = 0;
		theApp.m_iAppHeight = 0;

		theApp.GetMainWnd()->SetWindowText( "FPS Creator - Game Creator Store" );
		CMarketWindow TGCStore;
		TGCStore.DoModal();

		theApp.GetMainWnd()->SetWindowPos( NULL, theApp.m_iAppX,theApp.m_iAppY, theApp.m_iAppWidth,theApp.m_iAppHeight, 0 );
		//theApp.GetMainWnd()->ShowWindow( SW_SHOW );

		//theApp.GetMainWnd()->SetWindowText( str );
		theApp.GetMainWnd()->SetWindowText( "FPS Creator" );
	}
	*/
#endif

	// finished
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorApp message handlers

int CEditorApp::ExitInstance ( ) 
{
	// called when the application ends

	// end mutex so editor can be used again
	if ( m_hOnlyOneEditorMutex )
	{
		CloseHandle ( m_hOnlyOneEditorMutex );
		m_hOnlyOneEditorMutex = NULL;
	}

	// clean up the interface library
	BCGCBCleanUp ( );

	// signal the end of threads
	m_bEnd = TRUE;
	
	// call the base class function
	return CWinApp::ExitInstance ( );

	
}

void CEditorApp::OnAppAbout ( )
{
	// 110105
	if ( theApp.m_bTestGameCancel )
		return;

	// 270105 - force redraw
	theApp.m_bBuildGameDisplay = true;

	CAboutDlg aboutDlg;
	theApp.m_Message.GetIPC()->AllowSyncEventNow();
	aboutDlg.DoModal ( );

	// 270105 - force redraw
	theApp.m_bBuildGameDisplay = false;
}

void CEditorApp::PreLoadState ( )
{
	GetMouseManager       ( )->AddView ( IDR_EDITORTYPE, _T ( "My view" ), IDR_EDITORTYPE );
	GetContextMenuManager ( )->AddMenu ( _T ( "My menu" ), IDR_CONTEXT_MENU );
}

BOOL CEditorApp::PreTranslateMessage ( MSG* pMsg )
{
	if ( CSplashWnd::PreTranslateAppMessage ( pMsg ) )
		return TRUE;

	return CWinApp::PreTranslateMessage ( pMsg );
}

BOOL CEditorApp::OnOpenRecentFile(UINT nID)
{
	// 110105
	//if ( theApp.m_bTestGameCancel )
	//	return FALSE;

	SetFileMapData ( 442, 1 );

	//return CWinApp::OnOpenRecentFile ( nID );

	int nIndex = nID - ID_FILE_MRU_FILE1;

	CString file = ( *m_pRecentFileList ) [ nIndex ];

	TCHAR szFileName [ MAX_PATH ];

	// 170105
	//wcscpy ( szFileName, file );
	strcpy ( szFileName, file );

	SetFileMapDataString ( 1000, szFileName );

	//m_pRecentFileList [ 0 ] = _T ( "" );


	//return CWinApp::OnOpenRecentFile ( nID );
	return TRUE;
}

/*
BOOL CWinApp::OnOpenRecentFile(UINT nID)
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList != NULL);

	ASSERT(nID >= ID_FILE_MRU_FILE1);
	ASSERT(nID < ID_FILE_MRU_FILE1 + (UINT)m_pRecentFileList->GetSize());
	int nIndex = nID - ID_FILE_MRU_FILE1;
	ASSERT((*m_pRecentFileList)[nIndex].GetLength() != 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*m_pRecentFileList)[nIndex]);

	if (OpenDocumentFile((*m_pRecentFileList)[nIndex]) == NULL)
		m_pRecentFileList->Remove(nIndex);

	return TRUE;
}
*/

CDocument* CEditorApp::OpenDocumentFile ( LPCTSTR lpszFileName ) 
{
	//theApp.m_Debug.Write ( "OpenDocumentFile" );
	
	
	SetFileMapDataString ( 1000, ( LPTSTR ) lpszFileName );

	CDocument* pDocument = NULL;

	//theApp.m_Debug.Write ( "OpenDocumentFile - check OpenDocumentFile valid" );

	if ( !m_bSetupFrame )
	{
		

		CDocManager*    pDoc		= theApp.m_pDocManager;
		POSITION		pos			= pDoc->GetFirstDocTemplatePosition ( );
		CDocTemplate*	pTemplate	= pDoc->GetNextDocTemplate ( pos );

		//theApp.m_Debug.Write ( "OpenDocumentFile - Call base function for OpenDocumentFile" );

		pDocument = pTemplate->OpenDocumentFile ( NULL );
	}

	SetFileMapData ( 400, 1 );

	TCHAR szFileName [ MAX_PATH ];

	//170105
	//wcscpy ( szFileName, lpszFileName );
	strcpy ( szFileName, lpszFileName );

	SetFileMapDataString ( 1000, szFileName );

	m_bSetupFrame = true;

	//theApp.m_Debug.Write ( "OpenDocumentFile - return" );

	return pDocument;
	
	return CWinApp::OpenDocumentFile ( lpszFileName );
}

void CEditorApp::OnAppExit1() 
{
	// TODO: Add your command handler code here
	
	SetFileMapData ( 908, 1 );
	return;

		// clean up the interface library
	BCGCBCleanUp ( );

	// signal the end of threads
	m_bEnd = TRUE;

	// call the base class function
	CWinApp::ExitInstance ( );
}

int CEditorApp::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWinApp::Run();
}

BOOL CEditorApp::OnIdle(LONG lCount) 
{
	// FPSEXCHANGE Event Monitor
	cIPC* pIPC = m_Message.GetIPC ( );
	if ( pIPC )
	{
		// non-critical filemap activity
		// lee, what process for mapeditor GETTING values from interface
		UpdateMouse ( );

		// perform if mapeditor waiting for interface
		if ( pIPC->SkipIfEventNotWaiting () )
		{
			// Update from filemap
			if ( m_bGameOrBuildInProgress==true )
			{
				UpdateBuildGame       ( );
				UpdateTestGame        ( );
				UpdateRestart         ( );
			}
			else
			{
				UpdateToolBar         ( );
				UpdateStatusBar       ( );
				UpdateLibraryWindow   ( );
				UpdateBrowserWindow   ( );
				UpdateMessageBoxes    ( );
				UpdateFileOpenAndSave ( );
				UpdatePopup           ( );
				UpdateEntityWindow    ( );
				UpdateBuildGame       ( );
				UpdatePreferences     ( );
				UpdateTestGame        ( );
				UpdateFPIWizard       ( );
				UpdateProject         ( );
				UpdateRestart         ( );
				UpdateTermination	  ( );
			}

			// Exit message
			if ( theApp.GetFileMapData ( 908 ) == 1 )
			{
				// perform quit code
				UpdateQuit ( );
			}

			// Set event
			pIPC->AllowSyncEventNow();
		}
	}

	// flag to trigger window quit
	if ( m_bEnd )
	{
		// demo version end splash
		#ifdef FPSC_DEMO
 		cDialogEnd end;
		m_bDoNotNeedToCheckForCrashesAnymore = true;
		pIPC->AllowSyncEventNow();
 		end.DoModal ( );
		#endif

		// Actual exit here
		m_bEnd = false;
		this->pMainFrame->Quit ( );
	}

	// when idle, and weblink requested, do so
	if ( g_bWebsiteLink )
	{
		// why is this so slow? 20/30 seconds..
		g_bWebsiteLink=false;
		TCHAR szLink [ MAX_PATH ];
		GetPrivateProfileString ( _T ( "Help" ), _T ( "Website" ), _T ( "" ), szLink, MAX_PATH, theApp.m_szLanguage );
		ShellExecute ( NULL, "open", szLink, NULL, NULL, SW_SHOWNORMAL );
	}

	return CWinApp::OnIdle(lCount);
}

void CEditorApp::OnEditCopy1() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 458, 1 );
	
}

void CEditorApp::OnEditCopy() 
{
	theApp.SetFileMapData ( 458, 1 );
}

void CEditorApp::OnEditCut() 
{
	theApp.SetFileMapData ( 454, 1 );
}

void CEditorApp::OnEditClear() 
{
	theApp.SetFileMapData ( 462, 1 );
}

void CEditorApp::OnEditCut1() 
{
	theApp.SetFileMapData ( 454, 1 );
}

void CEditorApp::OnEditPaste() 
{

}

void CEditorApp::OnEditPaste1() 
{
	theApp.SetFileMapData ( 462, 1 );
}

void CEditorApp::OnEditRedo() 
{
	
}

void CEditorApp::OnEditRedo1() 
{
	theApp.SetFileMapData ( 450, 1 );
}

void CEditorApp::OnEditUndo() 
{
	
}

void CEditorApp::OnEditUndo1() 
{
	theApp.SetFileMapData ( 446, 1 );
}

void CEditorApp::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bTest [ 4 ] );
}

void CEditorApp::OnUpdateBuildGame(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bDisable );
}


void CEditorApp::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bTest [ 3 ] );
}

void CEditorApp::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bTest [ 0 ] );
}

void CEditorApp::OnUpdateEditCut1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bTest [ 2 ] );
}

void CEditorApp::OnUpdateEditCopy1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bTest [ 3 ] );
}

void CEditorApp::OnUpdateEditPaste1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bTest [ 4 ] );
}

void CEditorApp::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	
}

void CEditorApp::OnUpdateEditUndo1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bTest [ 0 ] );
}

void CEditorApp::OnUpdateEditRedo1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bTest [ 1 ] );
}

void CEditorApp::OnHelpWebsite() 
{
	g_bWebsiteLink=true;
}

void CEditorApp::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileNewArena(CCmdUI* pCmdUI) 
{
	// FREE VERSION
	if ( g_bFreeVersion )
		pCmdUI->Enable ( false );
	else
		pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileSaveAsEx(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFilePreferences(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile5(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile6(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile7(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile8(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile9(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile10(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile11(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile12(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile13(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile14(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );
}

void CEditorApp::OnUpdateFileMruFile15(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_bDisable );

}

DWORD CEditorApp::GetTodaysDays ( void )
{
	DWORD dwCurrent = 0;
	CTime time = CTime::GetCurrentTime();
	int iMonth = time.GetMonth ( );
	int iDay   = time.GetDay   ( );
	int iYear  = time.GetYear  ( ) - 2000;
	iYear *= 365;
	iMonth *= 31;
	return dwCurrent = iYear + iMonth + iDay;
}

bool CEditorApp::SerialCodeCheck ( LPSTR pSerialCodeStringToCheck )
{
	// if no string instant failure
	if ( !pSerialCodeStringToCheck )
		return false;

	// Copied from the SERIAL DLL source code (amended for function call use here)
	int iTestStructureA = 0;

	// serial to check
	char szSerial [ 600 ] = "";
	strcpy ( szSerial, pSerialCodeStringToCheck );

	// check serial code
	if ( szSerial [ 0 ] == pKeyCode[0] && szSerial [ 1 ] == pKeyCode[1] && szSerial [ 2 ] == pKeyCode[2] && szSerial [ 3 ] == pKeyCode[3] )
	{
		char szNumberCheck [ 9 ] = "";
		sprintf ( 
					szNumberCheck,
					"%c%c%c%c%c%c%c%c",
					szSerial [  4 ],
					szSerial [  5 ],
					szSerial [  6 ],
					szSerial [  7 ],
					szSerial [  8 ],
					szSerial [  9 ],
					szSerial [ 10 ],
					szSerial [ 11 ]
				);

		MD5 numberCheck;
		unsigned int len = strlen ( ( char* ) szNumberCheck );
		numberCheck.update   ( ( unsigned char* ) szNumberCheck, len );
		numberCheck.finalize ( );

		char szVerifyCheck [ 600 ] = "";
		sprintf ( szVerifyCheck, "%s", numberCheck.hex_digest ( ) );
		if ( szVerifyCheck [ iKeyA ] == szSerial [ 12 ] )
			if ( szVerifyCheck [ iKeyB ] == szSerial [ 13 ] )
				if ( szVerifyCheck [ iKeyC ] == szSerial [ 14 ] )
					if ( szVerifyCheck [ iKeyD ] == szSerial [ 15 ] )
						iTestStructureA++;
	}

	// check code for validity
	if ( iTestStructureA > 0 )
		return true;
	else
		return false;
}

bool CEditorApp::IsSerialCodeValid ( LPSTR pTryCodeOrNULL )

{
      // if NULL, simply checking for a valid code in the registry,
      // if not NULL, then we are going to write it to the registry if it is valid
      bool bValid = false;

      // check existing
      if ( pTryCodeOrNULL==NULL )
      {
            /* V111 - 140608 - no registry based serial code check (interferes with X10)
            // used to hold the temp serial code taken from registry
            char pMySerialCodeFromRegistry [ 512 ];
            strcpy ( pMySerialCodeFromRegistry, "" );
 
            // take existing code from registry and see if valid
            HKEY keyCode = 0;
            LONG ls = RegOpenKeyEx( HKEY_CURRENT_USER, _T("Software\\FPSCreator\\Editor\\Settings"), NULL, KEY_QUERY_VALUE, &keyCode );
            if ( ls == ERROR_SUCCESS )
            {
                  DWORD dwType=REG_SZ;
                  DWORD dwSize=512;
                  ls = RegQueryValueEx ( keyCode, _T("my_serial_code"), NULL, &dwType, (LPBYTE)pMySerialCodeFromRegistry, &dwSize );
                  if ( strlen(pMySerialCodeFromRegistry) > 0 ) pTryCodeOrNULL = pMySerialCodeFromRegistry;
            }
            if ( keyCode!=0 ) RegCloseKey( keyCode );
 
            // is it valid
            if ( SerialCodeCheck ( pTryCodeOrNULL )==true )
                  bValid = true;
            */
      }
      else
      {
            // check newly entered code for validity
            if ( SerialCodeCheck ( pTryCodeOrNULL )==true )
            {
                  /* V111 - 140608 - no registry based serial code check (interferes with X10)
                  // valid code, write into registry
                  HKEY keyCode = 0;
                  LONG ls = RegCreateKeyEx( HKEY_CURRENT_USER, _T("Software\\FPSCreator\\Editor\\Settings"), 0, NULL, 0, KEY_SET_VALUE, NULL, &keyCode, NULL );
                  if ( ls == ERROR_SUCCESS )
                  {
                        ls = RegSetValueEx( keyCode, _T("my_serial_code"), 0, REG_SZ, (BYTE*)pTryCodeOrNULL, strlen(pTryCodeOrNULL)+1 );
                  }
                  if ( keyCode!=0 ) RegCloseKey( keyCode );
                  */
 
                  // and flag as valid to pass this check
                  bValid = true;
            }
      }

      // if valid, but a blacklisted code, reject now
      if ( bValid==true && pTryCodeOrNULL )
      {
            // black listed codes 220508
            if ( stricmp ( pTryCodeOrNULL, "00V100030031232e" )==NULL ) bValid = false;
            if ( stricmp ( pTryCodeOrNULL, "00V1004004337a76" )==NULL ) bValid = false;
            if ( stricmp ( pTryCodeOrNULL, "00V117850951d643" )==NULL ) bValid = false;
            if ( stricmp ( pTryCodeOrNULL, "00V1042002338900" )==NULL ) bValid = false;
            if ( stricmp ( pTryCodeOrNULL, "0X100000140429b3" )==NULL ) bValid = false;
      }
      // return success if code is valid
      return bValid;
}

void CEditorApp::CheckSerialCodeValidity ( void ) 
{
	// V111 - 110609 - moved from cFront (where it is also called from)
	// FREE VERSION - serial code read from USERDETAILS.INI (else free version)

	// Read pUserSerialCode
	char pUserFile [ 512 ];
	wsprintf ( pUserFile, "%s\\%s", theApp.m_szDirectory, "userdetails.ini" );
	char pUserSerialCode [ 512 ];
	wsprintf ( pUserSerialCode, "" );
	GetPrivateProfileString( _T("PERSONAL DATA"), _T("YourSerialCode"), _T(""), pUserSerialCode, 512, pUserFile );

	// V111 - 140608 - ensure anyone entering lower case 00v1 manually is auto-corrected (reduce support issues)
	if ( strlen ( pUserSerialCode ) > 5 )
		if ( strncmp ( pUserSerialCode, "00v1", 4 )==NULL )
			pUserSerialCode [ 2 ] = 'V';

	// Check pUserSerialCode
	if ( IsSerialCodeValid ( pUserSerialCode )==true )
	{
		// Only COMPLETE VERSION has a valid serial code, else it is a free
		// version that does not have a serial code (or is an invalid code).
		g_bFreeVersion = false;
	}
	else
	{
		// INVALID or no serial code means FREE VERSION
		g_bFreeVersion = true;

		// V110 BETA3 - 060608 - time stamp first use of free version for upgrade calculation
		HKEY keyCode = 0;
		char pMyFreeVersionStartDate [ 512 ];
		strcpy ( pMyFreeVersionStartDate, "" );
		LONG ls = RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("Software\\FPSCreator\\Editor\\Settings"), NULL, KEY_QUERY_VALUE, &keyCode );
		if ( ls == ERROR_SUCCESS )
		{
			DWORD dwType=REG_SZ;
			DWORD dwSize=512;
			ls = RegQueryValueEx ( keyCode, _T("uniquevalue"), NULL, &dwType, (LPBYTE)pMyFreeVersionStartDate, &dwSize );
		}
		if ( keyCode!=0 ) RegCloseKey( keyCode );
		if ( strcmp ( pMyFreeVersionStartDate, "" )==NULL )
		{
			// work out start date
			int iDaysSince2008 = (int)GetTodaysDays();
			wsprintf ( pMyFreeVersionStartDate, "%d", iDaysSince2008 );

			// write start date for new free version usage
			keyCode = 0;
			ls = RegCreateKeyEx( HKEY_LOCAL_MACHINE, _T("Software\\FPSCreator\\Editor\\Settings"), 0, NULL, 0, KEY_SET_VALUE, NULL, &keyCode, NULL );
			if ( ls == ERROR_SUCCESS )
			{
				ls = RegSetValueEx( keyCode, _T("uniquevalue"), 0, REG_SZ, (LPBYTE)pMyFreeVersionStartDate, strlen(pMyFreeVersionStartDate)+1 );
			}
			if ( keyCode!=0 ) RegCloseKey( keyCode );
		}
		else
		{
			// work out how many days have passed since start date
			int iDaysSince2008Start = atoi((LPSTR)pMyFreeVersionStartDate);
			int iDaysNow = (int)GetTodaysDays();
			g_iDaysHavePassed = iDaysNow - iDaysSince2008Start;
			if ( g_iDaysHavePassed < 60 )
			{
				if ( g_iDaysHavePassed < 45 )
					g_iUpgradeOffer = 1;
				else
					g_iUpgradeOffer = 2;
			}
			else
				g_iUpgradeOffer = 0;
		}
	}
}
