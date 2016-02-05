#pragma once
#include "thread.h"

class CMarketWindow;

class ExternalLinkThread : public Thread
{

public:
	
	ExternalLinkThread();
	~ExternalLinkThread();

	void SetLink( CMarketWindow *window, char *szLink );

protected:

	unsigned Run( );

	CString sLink;
	CMarketWindow *pWindow;
};
