#ifndef _H_HTTPCOMM
#define _H_HTTPCOMM

#include <windows.h>
#include <wininet.h>

class HTTPComm
{

public:

	HTTPComm( );
	~HTTPComm( );

	bool	UpdateCredits( LPCSTR szSessionID );
	bool	Rate( LPCSTR szObjID, LPCSTR szRating );
	bool	Approve( LPCSTR szObjID );
	bool	Remove( LPCSTR szObjID );
	bool	Restore( LPCSTR szObjID );
	bool	Delete( LPCSTR szObjID );

	bool	Login( LPCSTR szUsername, LPCSTR szPassword );
	void	Logout( );
	bool	GetMainModelList( LPCSTR szFilename, bool bExtended = false );
	bool	GetGroupList( LPCSTR szFilename );
	bool	GetGroupObjects( LPCSTR szFilename, int group );
	bool	GetFreeObjects( LPCSTR szFilename );
	bool	GetPurchasedList( LPCSTR szFilename );
	bool	GetSearchList( LPCSTR szFilename );
	bool	GetKeywordList( LPCSTR szFilename );
	//bool	Search( LPCSTR szFilename, LPCSTR szName, LPCSTR szCategory, LPCSTR szCostLow, LPCSTR szCostHigh, LPCSTR szAuthor, LPCSTR szRatingLow, LPCSTR szRatingHigh, LPCSTR szKeyword );
	bool	GetImages( LPCSTR szFolder, int iGroup, CString sNeeded );
	bool	GetModel( DWORD dwModelID, DWORD dwProjectedCost, LPCSTR szFilename );
	bool	GetLicense( DWORD dwObjID, LPCSTR szFilename, LPCSTR szSESID );
	bool	GetX9Serial( DWORD dwObjID, LPCSTR szSESID, LPSTR szSerialOut, DWORD dwSize );
	bool	GetModelDetails( DWORD dwModelID, LPCSTR szSESID );
	bool	GetPreviewImage( LPCSTR szImage, LPCSTR szFilename );
	void	SetProgress( CProgressCtrl *ProgressBar, CStatic *Status ) { m_pProgressBar = ProgressBar; m_pStatusText = Status; }
	void	PostCounter( );
	bool	GetLatestVersion( char *szVersion, DWORD dwSize );

	DWORD	CreateObject( const char *szName, const char *szCost, const char *szType, const char *szCategory, const char *szSubCategory,
					const char *szPolygons, const char *szKeywords, const char *szDescription, const char *szVideo, 
					const char *szNumImages, const char *szGroupID, int iX9Compatible, int iX10Compatible );
	bool	EditObject( DWORD dwObjID, int iX9Compatible, int iX10Compatible, const char *szCost, const char *szPolygons, const char *szDescription, 
		                const char *szVideo, const char *szKeywords, int iResetRating, int iResetDownloads );
	bool	UploadModelFile( DWORD dwObjID, const char *szFilename, bool bEditing = false );
	bool	UploadThumbnail( DWORD dwObjID, const char *szFilename, bool bEditing = false );
	bool	UploadPreviewImage( DWORD dwObjID, const char *szFilename, bool bEditing = false );
	bool	ClearPreviewImages( DWORD dwObjID );
	bool	UploadMusicSample( DWORD dwObjID, const char *szFilename, bool bEditing = false );

	void	CancelTransfer() { bCancelTransfer = true; }
	void	BeginTransfer() { bCancelTransfer = false; }
	bool	Disconnect		( void );

	const char* GetLastError( );

private:

	bool HandleReply( const char* szReply );
	bool HandleReply2( const char* szReply );

	//HINTERNET hInet;
	HINTERNET hInetConnect;
	char szLastError [ 256 ];
	bool bSecure;
	bool bCancelTransfer;

	char *szSESID;

	CStatic *m_pStatusText;
	CProgressCtrl *m_pProgressBar;

	bool	Connect		( char* url, bool secure );
	LPSTR	RequestData	( LPCSTR pObjectName, LPSTR pPostData, DWORD dwPostDataSize, DWORD *dwWritten, LPCSTR szSaveToFile = NULL, bool bAllowCache = true );
	LPSTR	SendData	( LPCSTR pObjectName, LPCSTR szFilename, DWORD dwObjID, bool bEditing, LPCSTR szFileType, LPSTR pFileData, DWORD dwFileSize );
};

#endif
