#include "stdafx.h"
#include "ExternalLinkThread.h"
#include "MarketWindow.h"
#include "..\Editor.h"

ExternalLinkThread::ExternalLinkThread()
{
	sLink = "";
}

ExternalLinkThread::~ExternalLinkThread()
{
}

void ExternalLinkThread::SetLink( CMarketWindow *window, char *szLink )
{
	pWindow = window;
	
	if ( !szLink ) return;
	sLink = szLink;
}

unsigned ExternalLinkThread::Run( )
{
	char szBrowser [ MAX_PATH ];
	bool bResult = theApp.GetDefaultBrowserPath( szBrowser, MAX_PATH );
	if ( !bResult ) strcpy_s( szBrowser, MAX_PATH, "iexplore.exe" );

	if ( sLink.GetLength() > 0 ) ShellExecute( NULL, "open", szBrowser, sLink, NULL, SW_SHOWNORMAL );

	if ( pWindow ) pWindow->LinkLoaded( );

	delete this;
	return 0;
}
