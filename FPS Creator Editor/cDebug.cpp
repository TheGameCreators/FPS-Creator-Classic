// cDebug.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cDebug.h"

#include "mmsystem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cDebug

cDebug::cDebug()
{
	

}

cDebug::~cDebug()
{
	if ( m_pFile )
		fclose ( m_pFile );
}


BEGIN_MESSAGE_MAP(cDebug, CWnd)
	//{{AFX_MSG_MAP(cDebug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cDebug message handlers

void cDebug::Start ( char* szFile, bool bINI )
{
	// 310105
	TCHAR szDebug [ MAX_PATH ];
	
	//GetPrivateProfileString ( _T ( "Debug" ), _T ( "Trace" ), _T ( "" ), szDebug, MAX_PATH, _T ( "./files/editors/host/editor.ini" ) );

	if ( bINI )
	{
		GetPrivateProfileString ( _T ( "Debug" ), _T ( "Trace" ), _T ( "" ), szDebug, MAX_PATH, theApp.m_szLanguage );
	}
	else
	{
		strcpy ( szDebug, "1" );
	}

	if ( strcmp ( szDebug, "1" ) == 0 )
	{
	
		m_pFile = fopen ( szFile, "wt" );
	}

	else
		m_pFile = NULL;
}

void cDebug::Write ( char* szMessage )
{
	

	if ( m_pFile )
	{
		char szTime [ 256 ] = "";
		DWORD dwTime = timeGetTime ( );

		sprintf ( szTime, "%i - %s", dwTime, szMessage );

		fwrite ( szTime, strlen ( szTime ) * sizeof ( char ), 1, m_pFile );

		//fwrite ( szMessage, strlen ( szMessage ) * sizeof ( char ), 1, m_pFile );
		fwrite ( "\n",      strlen ( "\n"      ) * sizeof ( char ), 1, m_pFile );

		fflush ( m_pFile );
	}
}