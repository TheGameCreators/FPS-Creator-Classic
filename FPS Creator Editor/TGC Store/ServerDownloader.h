#ifndef H_SERVER_DOWNLOADER
#define H_SERVER_DOWNLOADER

#include "stdafx.h"
#include "HTTPComm.h"
#include "Thread.h"

#include "MarketObject.h"

class cMarketImage;
class CMarketTree;
class CMarketWindow;

struct Request
{
	DWORD dwType;
	CMarketWindow *pParent;
	int dwGroup;
	LPCSTR szFilename;
	DWORD dwObjectID;

	Request *pNextRequest;

	Request() { dwType = -1; pParent = NULL; dwGroup = 0; szFilename = NULL; dwObjectID = 0; pNextRequest = NULL; }
	~Request() { if ( szFilename ) delete [] szFilename; szFilename = NULL; }
};

class ServerDownloader : public Thread
{

public:

	ServerDownloader( );
	~ServerDownloader( );

	void SetLoginProcess( CMarketWindow *pParent, const char *szUser, const char *szPass, bool bSilent );
	void SetGroupListDownload( CMarketWindow *pParent );
	void SetObjectListDownload( CMarketWindow *pParent, int group );
	void SetSearchListDownload( CMarketWindow *pParent );
	void SetPurchasedListDownload( CMarketWindow *pParent );
	void SetThumbnailDownload( int group, CMarketWindow *pParent, CString sNeeded );
	void SetMusicSampleDownload( const char *szServerFilename, const char *szSaveFilename );
	void SetModelDownload( LPCSTR szFilename, const MarketObject *pObject, DWORD dwProjectedCost, CMarketWindow *pParent );
	void SetProgress( CProgressCtrl *ProgressBar, CStatic *Status ) { m_Server.SetProgress( ProgressBar, Status ); }

	bool IsDownloading( ) { return m_bDownloading; }
	bool IsModelListDownload( ) { return m_iDownloadType == 0; }
	bool IsThumbnailDownload( ) { return m_iDownloadType == 2; }
	void Cancel( );
	bool WasSuccessful() { return m_bSuccess; };

	DWORD GetObjectID() { return m_dwObjID; }

protected:

	virtual unsigned Run( );

private:

	//cMarketImage *m_pList;
	//cLibraryTree *m_pTree;
	CMarketWindow *m_pWindow;
	HTTPComm m_Server;
	bool m_bDownloading;
	int m_iDownloadType;
	bool m_bExtendedList;
	bool m_bTerminate;
	int m_iGroup;
	CString m_sThumbnailsNeeded;
	MarketObject cLocalObject;
	CString m_sUser;
	CString m_sPass;
	bool m_bSilentLogin;

	bool m_bSuccess;

	CString sString1, sString2;

	char* m_szModelFilename;
	DWORD m_dwObjID;
	int m_iGroupID;
	DWORD m_dwProjectedCost;

	Request *m_pRequestQueue, *m_pRequestTail;
	HANDLE m_hMutex; 
};

#endif