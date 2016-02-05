// cDialogFront.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cDialogFront.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cDialogFront dialog


cDialogFront::cDialogFront(CWnd* pParent /*=NULL*/)
	: CDialog(cDialogFront::IDD, pParent)
{
	//{{AFX_DATA_INIT(cDialogFront)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cDialogFront::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cDialogFront)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control ( pDX, IDC_PROGRESS_FRONT_1,		m_Progress );
		DDX_Control ( pDX, IDC_STATIC_FRONT_TEXT,		m_Edit );
		DDX_Control ( pDX, IDC_STATIC_FRONT1,		m_Edit1 );
		DDX_Control ( pDX, IDOK,		m_Okay );
		DDX_Control ( pDX, IDCANCEL,		m_Cancel );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cDialogFront, CDialog)
	//{{AFX_MSG_MAP(cDialogFront)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cDialogFront message handlers

void CoreReadStringFromRegistry(char* PerfmonNamesKey, char* valuekey, char* string, DWORD* dwValue )
{
	HKEY hKeyNames = 0;
	DWORD Status;
	char ObjectType[256];
	DWORD Datavalue = 0;

	strcpy(string,"");
	strcpy(ObjectType,"Num");
	Status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, PerfmonNamesKey, 0L, KEY_READ, &hKeyNames);
    if(Status==ERROR_SUCCESS)
	{
		DWORD Type=REG_SZ;
		DWORD Size=256;
		Status = RegQueryValueEx(hKeyNames, valuekey, NULL, &Type, NULL, &Size);
		if(Size<255)
			RegQueryValueEx(hKeyNames, valuekey, NULL, &Type, (LPBYTE)dwValue, &Size);
			//RegQueryValueEx(hKeyNames, valuekey, NULL, &Type, (LPBYTE)string, &Size);

		RegCloseKey(hKeyNames);
	}
}


void cDialogFront::OnCancel() 
{
	// TODO: Add extra cleanup here

	char szDebug [ 256 ] = "";
	//GetPrivateProfileString ( _T ( "Demo" ), _T ( "Order" ), _T ( "" ), szDebug, MAX_PATH, _T ( "./files/editors/host/editor.ini" ) );
	GetPrivateProfileString ( _T ( "Demo" ), _T ( "Order" ), _T ( "" ), szDebug, MAX_PATH, theApp.m_szLanguage );
	::ShellExecute (NULL, NULL, szDebug, NULL, NULL, NULL);

	TerminateProcess ( theApp.m_DBProApplication.hProcess, 0 );

	//theApp.m_bEnd = TRUE;

	PostQuitMessage ( 0 );
	
	CDialog::OnCancel();
}

BOOL cDialogFront::OnInitDialog() 
{
	/*
		3. Change the progress bar code to read from Software\The Game Creators\FPSCreatorInfo\ for days
		The key field is called demodatepleasedonothack and holds a value of the first day FPSC was used
		To decrypt the field value, divide by 365 to get the year, divide the remainder by 31 for month
		and the rest of the remainder is the day of that month. Easy to hack but enough for now.

		// year * 365
		// month * 31
		// add current day

		// 5 * 365 = 1825
		// 2 * 31  = 62
		// 21

		// 1908

		// 1908 / 365 = 5.2
		// .22 = date
   */

	CDialog::OnInitDialog();

	char szDebug [ 256 ] = "";
	GetPrivateProfileString ( _T ( "Demo" ), _T ( "Text" ), _T ( "" ), szDebug, MAX_PATH, theApp.m_szLanguage );
	m_Edit1.SetWindowText ( szDebug );
	
	char szData [ 256 ] = "";
	DWORD dwValue = 0;
	DWORD dwReg     = 0;
	DWORD dwCurrent = 0;

	CTime time = CTime::GetCurrentTime();
	int iMonth = time.GetMonth ( );
	int iDay   = time.GetDay   ( );
	int iYear  = time.GetYear  ( ) - 2000;
	iYear *= 365;
	iMonth *= 31;
	dwCurrent = iYear + iMonth + iDay;

	// 30 in nov
	// 31 in dec
	// 31 in jan

	int iStartDay	= 31;
	int iStartMonth = 10;
	int iStartYear	= 2006;

	int iEndDay		= 31;
	int iEndMonth	= 1;
	int iEndYear	= 2007;


	CoreReadStringFromRegistry ( "SOFTWARE\\The Game Creators\\FPSCreatorInfo", "demodatepleasedonothackV1", szData, &dwValue );
	DWORD dwDayInDemo = 90;
	DWORD dwDaysLeft = dwDayInDemo;
	if ( dwValue==0 )
	{
		// new demo use - no registry entry
		dwReg = dwCurrent;
		m_Progress.SetRange ( 0, 31 );
		m_Progress.SetStep  ( 1 );
		m_Progress.SetPos ( 0 );
	}
	else
	{
		// get first use date from registry
		dwReg = dwValue;
		DWORD dwYear = dwValue / 365;
		dwValue = dwValue - ( dwYear * 365 );
		DWORD dwMonth = ( dwValue / 31 );
		dwValue = dwValue - ( dwMonth * 31 );
		DWORD dwDay = dwValue;

		m_Progress.SetRange ( 0, (short)(dwDayInDemo+1) );
		m_Progress.SetStep  ( 1 );
		m_Progress.SetPos ( dwCurrent - dwReg );
		dwDaysLeft = (DWORD) abs ( (int)dwCurrent - (int)dwReg - (int)dwDayInDemo );
	}
	if ( dwDaysLeft > dwDayInDemo || ( dwCurrent - dwReg ) > dwDayInDemo )
	{
		m_Okay.SendMessage (WM_COMMAND, BN_DISABLE, 0);
		m_bValid = false;
		dwDaysLeft = 0;
		m_Progress.SetPos ( dwDayInDemo+1 );
		theApp.m_bDoNotNeedToCheckForCrashesAnymore = true;
	}
	else
	{
		m_bValid = true;
	}

	char szInfo [ 256 ] = "";
	CString a = GetLanguageData ( "Demo", "Text1" );
	sprintf ( szInfo, "%i %s", dwDaysLeft, a );
	m_Edit.SetWindowText ( szInfo );

	m_Okay.SetWindowText ( GetLanguageData ( "Demo", "Button1" ) );
	m_Cancel.SetWindowText ( GetLanguageData ( "Demo", "Button2" ) );
	if ( m_bValid==false ) m_Okay.EnableWindow ( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int cDialogFront::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void cDialogFront::OnOK() 
{
	// TODO: Add extra validation here

	CTime time = CTime::GetCurrentTime();

	int iMonth = time.GetMonth ( );
	int iDay   = time.GetDay   ( );

	if ( !m_bValid )
	{
		//if ( iDay == 31 )
		{
			char szDebug [ 256 ] = "";
			//GetPrivateProfileString ( _T ( "Demo" ), _T ( "Order" ), _T ( "" ), szDebug, MAX_PATH, _T ( "./files/editors/host/editor.ini" ) );
			GetPrivateProfileString ( _T ( "Demo" ), _T ( "Order" ), _T ( "" ), szDebug, MAX_PATH, theApp.m_szLanguage );
			::ShellExecute (NULL, NULL, szDebug, NULL, NULL, NULL);

			TerminateProcess ( theApp.m_DBProApplication.hProcess, 0 );

			PostQuitMessage ( 0 );
			
			CDialog::OnCancel();
		}
	}
	else
	{

	}
	
	CDialog::OnOK();
}
