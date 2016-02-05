/
// HTTP Secure Server Communucation
//

//
// Includes
//

#include "stdafx.h"

#define TGC_HTTP
#ifdef TGC_HTTP

#include "HTTPComm.h"
#include <stdio.h>
#include <iostream>
#include "..\Editor.h"

// Externals
LPSTR g_pFeedbackAreaString;
bool g_bExtraReporting;

//
// Internal Functions
//

HTTPComm::HTTPComm( )
{
	//hInet = NULL;
	hInetConnect = NULL;
	m_pProgressBar = NULL;
	m_pStatusText = NULL;

	bCancelTransfer = false;

	strcpy_s( szLastError, 256, "" );
}

HTTPComm::~HTTPComm( )
{
	Disconnect( );
}

const char* HTTPComm::GetLastError( )
{
	return szLastError;
}

bool HTTPComm::Connect ( char* url, bool secure )
{
	bSecure = secure;

	// Access type SECURE
	WORD wHTTPType = bSecure ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;

	// Ensure we are disconnected first
	if( !hInetConnect )
	{
		// Attempt to connect...
		DWORD dwResult = InternetAttemptConnect(0);
		if( dwResult != ERROR_SUCCESS )
		{
			strcpy_s( szLastError, 256, GetLanguageData( _T("TGC Store"), _T("NoInternet") ) );
			return false;
		}
		
		// Create Internet-Open
		if( !theApp.m_hInet )
		{
			theApp.m_hInet = InternetOpen(	"InternetConnection",
									INTERNET_OPEN_TYPE_PRECONFIG,
									NULL, NULL, 0 );
			if( !theApp.m_hInet )
			{
				strcpy_s( szLastError, 256, GetLanguageData( _T("TGC Store"), _T("FailedOpen") ) );
				return false;
			}
		}

		// Internet Connect
		hInetConnect = InternetConnect(	theApp.m_hInet,
										url,
										wHTTPType,
										"",
										NULL,
										INTERNET_SERVICE_HTTP,
										0, 0);
		if( !hInetConnect )
		{
			strcpy_s( szLastError, 256, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ) );
			//InternetCloseHandle( hInet );
			//hInet = NULL;
			return false;
		}
	}

	//Successful
	return true;
}

LPSTR HTTPComm::RequestData ( LPCSTR pObjectName, LPSTR pPostData, DWORD dwPostDataSize, DWORD *dwWritten, LPCSTR szSaveToFile,  bool bAllowCache )
{
	char szHeaderString [ 256 ];
	strcpy_s( szHeaderString, 256, "" );

	char *pFinalPostData = NULL;
	DWORD dwFinalPostSize = 0;

	if ( pPostData )
	{
		strcpy_s( szHeaderString, 256, "Content-Type: application/x-www-form-urlencoded" );

		pFinalPostData = new char [dwPostDataSize + 45];
		strcpy_s( pFinalPostData, dwPostDataSize + 45, pPostData );
		if ( dwPostDataSize > 0 ) strcat_s( pFinalPostData, dwPostDataSize + 45, "&lang=" );
		else strcat_s( pFinalPostData, dwPostDataSize + 45, "lang=" );
		strcat_s( pFinalPostData, dwPostDataSize + 45, theApp.m_szLanguageName );

		dwFinalPostSize = strlen(pFinalPostData);
	}
	else if ( !bAllowCache )
	{
		strcpy_s( szHeaderString, 256, "Cache-Control: max-age=0" );
		//MessageBox( NULL, szHeaderString, "Info", 0 );
	}

	DWORD dwHeaderSize = (DWORD) strlen( szHeaderString );

	if ( dwWritten ) *dwWritten = 0;

	// return var
	DWORD dwDataReturned = 0;
	DWORD dwArraySize = 0;
	LPSTR pDataReturned = NULL;

	// access type SECURE
	DWORD dwFlag = INTERNET_FLAG_IGNORE_CERT_CN_INVALID;
	if ( bSecure ) dwFlag |= INTERNET_FLAG_SECURE;

	char *szType = pFinalPostData ? "POST" : "GET";

	// prepare request
	HINTERNET hHttpRequest = HttpOpenRequest( hInetConnect, szType, pObjectName, "HTTP/1.1", "The Agent", NULL, dwFlag, 0 );
	if ( !hHttpRequest )
	{
		strcpy_s( szLastError, 256, "Failed to open request" );
		return NULL;
	}

	// send request with header
	BOOL bSendResult = HttpSendRequest( hHttpRequest, (dwHeaderSize > 0 ? szHeaderString : NULL), dwHeaderSize, pFinalPostData, dwFinalPostSize );
	if ( !bSendResult )
	{
		sprintf_s( szLastError, 256, "Failed to send request: %d", ::GetLastError( ) );
		//strcpy_s( szLastError, 256, "Failed to send request" );
		return NULL;
	}

	DWORD dwContentLength = 0;
	DWORD dwBufferSize = sizeof(DWORD);
	DWORD dwHeaderIndex = 0;
	BOOL bReturnHeader = HttpQueryInfo( hHttpRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwContentLength, &dwBufferSize, &dwHeaderIndex );

	if ( dwContentLength > 0 && !szSaveToFile )
	{
		pDataReturned = new char [ dwContentLength + 1 ];
		dwArraySize = dwContentLength + 1;
	}

	bool bFileCreated = false;
	FILE *pFile = NULL;

	//if ( m_pStatusText ) m_pStatusText->SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloading") ) );
	if ( m_pProgressBar ) m_pProgressBar->SetPos( 0 );

	// reading all data
	do
	{
		//std::cout << "Getting Data..." << std::endl;

		// data comes in chunks
		DWORD dwSize = 0;
		if ( !InternetQueryDataAvailable( hHttpRequest, &dwSize, 0, 0 ) )
		{
			// error getting chunk
			strcpy_s( szLastError, 256, GetLanguageData( _T("TGC Store"), _T("ConnectionLost") ) );
			return NULL;
		}

		// chunk ready - allocate a buffer
		LPSTR lpszData = new TCHAR [ dwSize + 1 ];

		// read the data from the HINTERNET handle.
		DWORD dwDownloaded = 0;
		if( !InternetReadFile( hHttpRequest, (LPVOID) lpszData, dwSize, &dwDownloaded ) )
		{
			// failed to read the data
			strcpy_s( szLastError, 256, GetLanguageData( _T("TGC Store"), _T("ConnectionLost") ) );
			delete[] lpszData;
			return NULL;
		}

		// add a null terminator to the end of the data buffer.
		//lpszData[dwDownloaded]='\0';

		// Check the size of the remaining data.  If it is zero, break.
		if ( dwDownloaded == 0 ) break;

		if ( szSaveToFile )
		{
			if ( !bFileCreated )
			{
				if ( strncmp( lpszData, "<failed>", strlen("<failed>") ) == 0 )
				{
					szSaveToFile = NULL;
				}
				else
				{
					int error = fopen_s( &pFile, szSaveToFile, "wb" );
					if ( error ) szSaveToFile = NULL;
					else bFileCreated = true;
				}
			}

			if ( bFileCreated )
			{
				fwrite( lpszData, 1, dwDownloaded, pFile );
				dwDataReturned += dwDownloaded;
			}
		}
		
		if ( !szSaveToFile )
		{
			// Add part-data to overall data returned
			DWORD dwNewDataSize = dwDataReturned + dwDownloaded;

			if ( dwArraySize < dwNewDataSize )
			{
				// recreate a bigger array in 1MB chunks
				LPSTR pNewData = new char [ dwNewDataSize + 1000000 ];
				
				if ( pDataReturned ) 
				{
					memcpy( pNewData, pDataReturned, dwDataReturned );
					delete [] pDataReturned;
				}
				pDataReturned = pNewData;

				dwArraySize = dwNewDataSize + 1000000;
			}

			memcpy( pDataReturned + dwDataReturned, lpszData, dwDownloaded );
			dwDataReturned = dwNewDataSize;
		}
	
		//set progress bar, if any
		if ( m_pProgressBar && !bCancelTransfer && dwContentLength > 0 ) 
		{
			m_pProgressBar->SetPos( (int) (100.0f*dwDataReturned / dwContentLength) );
		}

		// free usages
		if ( lpszData )
		{
			delete [] lpszData;
			lpszData = NULL;
		}
	}
	while( TRUE && !bCancelTransfer );

	if ( bFileCreated ) fclose( pFile );

	if ( bCancelTransfer )
	{
		//if ( m_pProgressBar ) m_pProgressBar->SetPos( 0 );
		if ( pDataReturned ) delete [] pDataReturned;
		return NULL;
	}

	//char num [ 12 ];
	//sprintf_s( num, 12, "%p", m_pProgressBar );
	//MessageBox( NULL, num, "Info", 0 );

	if ( dwWritten ) *dwWritten = dwDataReturned;
	if ( pDataReturned ) pDataReturned [ dwDataReturned ] = '\0';

	//if ( m_pStatusText ) m_pStatusText->SetWindowText( GetLanguageData( _T("TGC Store"), _T("Idle") ) );
	if ( m_pProgressBar ) m_pProgressBar->SetPos( 0 );

	if ( bFileCreated ) 
	{
		if ( pDataReturned ) delete [] pDataReturned;
		pDataReturned = new char [ 2 ];
		pDataReturned [ 0 ] = '1';
		pDataReturned [ 1 ] = '\0';
	}

	// return data in ptr
	return pDataReturned;
}

LPSTR HTTPComm::SendData ( LPCSTR pObjectName, LPCSTR szFilename, DWORD dwObjID, bool bEditing, LPCSTR szFileType, LPSTR pFileData, DWORD dwFileSize )
{
	bCancelTransfer = false; 

	char pHeaderString [ 256 ];
	DWORD dwHeaderSize = 0;

	// return var
	DWORD dwDataReturned = 0;
	LPSTR pDataReturned = NULL;

	// access type SECURE
	DWORD dwFlag = INTERNET_FLAG_IGNORE_CERT_CN_INVALID;
	if ( bSecure ) dwFlag |= INTERNET_FLAG_SECURE;

	char szID [ 12 ];
	sprintf_s( szID, 12, "%d", dwObjID );

	char *szType = "POST";

	//sesid=
	CString sData = "--------------------AaB03x\r\nContent-Disposition: form-data; name=\"sesid\"\r\n\r\n";
	sData += theApp.m_szSESID;

	//lang=
	sData += "\r\n--------------------AaB03x\r\nContent-Disposition: form-data; name=\"lang\"\r\n\r\n";
	sData += theApp.m_szLanguageName;
	
	//objid=
	sData += "\r\n--------------------AaB03x\r\nContent-Disposition: form-data; name=\"objid\"\r\n\r\n";
	sData += szID;

	//editing=
	sData += "\r\n--------------------AaB03x\r\nContent-Disposition: form-data; name=\"editing\"\r\n\r\n";
	sData += bEditing ? "1" : "0";

	sData += "\r\n--------------------AaB03x\r\nContent-Disposition: form-data; name=\"myfile\"; filename=\"";
	sData += szFilename;
	sData += "\"\r\nContent-Type: ";
	if ( szFileType && strlen( szFileType ) > 0 ) sData += szFileType;
	else sData += "application/x-zip-compressed";
	sData += "\r\n\r\n";

	CString sEnd = "\r\n--------------------AaB03x--\r\n";

	DWORD dwLength = sData.GetLength() + sEnd.GetLength() + dwFileSize;
	
	char *pData = new char [ dwLength ];
	memcpy( pData, sData.GetBuffer(), sData.GetLength() );
	char *pOffset = pData + sData.GetLength();
	memcpy( pOffset, pFileData, dwFileSize );
	pOffset += dwFileSize;
	memcpy( pOffset, sEnd.GetBuffer(), sEnd.GetLength() );

	sprintf_s( pHeaderString, 256, "Content-Type: multipart/form-data; boundary=------------------AaB03x\r\nContent-Length: %d", dwLength );

	//unsigned long ulTime = 1000000;
	//InternetSetOption( hInet, INTERNET_OPTION_SEND_TIMEOUT, (void*) &ulTime, sizeof(unsigned long) );

	// prepare request
	HINTERNET hHttpRequest = HttpOpenRequest( hInetConnect, szType, pObjectName, "HTTP/1.1", "The Agent", NULL, dwFlag, 0 );
	if ( !hHttpRequest )
	{
		strcpy_s( szLastError, 256, "Failed to open request" );
		return NULL;
	}

	HttpAddRequestHeaders( hHttpRequest, pHeaderString, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE );
	/*
	// send request with header
	BOOL bSendResult = HttpSendRequest( hHttpRequest, NULL, -1, (void*) pData, dwLength );
	if ( !bSendResult )
	{
		sprintf_s( szLastError, 256, "Failed to send request: %d", ::GetLastError( ) );
		return NULL;
	}
	*/

	INTERNET_BUFFERS internetBuffer = { 0 };
	internetBuffer.dwStructSize = sizeof(INTERNET_BUFFERS);
	internetBuffer.lpvBuffer = 0;
	internetBuffer.dwBufferLength = 0;

	BOOL bSendResult = HttpSendRequestEx( hHttpRequest, NULL, NULL, NULL, NULL );
	if ( !bSendResult )
	{
		sprintf_s( szLastError, 256, "Failed to send request: %d", ::GetLastError( ) );
		return NULL;
	}

	DWORD dwWritten = 0;
	DWORD dwRemaining = dwLength;
	char *pPtr = pData;

	while ( dwRemaining > 50000 && !bCancelTransfer )
	{
		bSendResult = InternetWriteFile( hHttpRequest, pPtr, 50000, &dwWritten );
		if ( !bSendResult )
		{
			sprintf_s( szLastError, 256, "Failed to send data: %d", ::GetLastError( ) );
			return NULL;
		}

		dwRemaining -= 50000;
		pPtr += 50000;

		dwWritten = dwLength - dwRemaining;
		if ( m_pProgressBar ) m_pProgressBar->SetPos( (int) (dwWritten*100.0f / dwLength) );
	}

	if ( bCancelTransfer ) 
	{
		strcpy_s( szLastError, 256, GetLanguageData( _T("TGC Store"), _T("UploadCancelled") ) );
		return NULL;
	}

	//final chunk
	bSendResult = InternetWriteFile( hHttpRequest, pPtr, dwRemaining, &dwWritten );
	if ( !bSendResult )
	{
		sprintf_s( szLastError, 256, "Failed to send data: %d", ::GetLastError( ) );
		return NULL;
	}

	bSendResult = HttpEndRequest( hHttpRequest, NULL, NULL, NULL );
	if ( !bSendResult )
	{
		sprintf_s( szLastError, 256, "Failed to end data: %d", ::GetLastError( ) );
		return NULL;
	}

	DWORD dwContentLength = 0;
	DWORD dwBufferSize = sizeof(DWORD);
	DWORD dwHeaderIndex = 0;
	BOOL bReturnHeader = HttpQueryInfo( hHttpRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwContentLength, &dwBufferSize, &dwHeaderIndex );

	//char str [ 256 ];
	//sprintf_s( str, 256, "Content Length: %d", dwContentLength );
	//MessageBox( NULL, str, "info", 0 );

	// reading all data
	do
	{
		//std::cout << "Getting Data..." << std::endl;

		// data comes in chunks
		DWORD dwSize = 0;
		if ( !InternetQueryDataAvailable( hHttpRequest, &dwSize, 0, 0 ) )
		{
			// error getting chunk
			strcpy_s( szLastError, 256, "Failed to get chunk" );
			return NULL;
		}

		// chunk ready - allocate a buffer
		LPSTR lpszData = new TCHAR [ dwSize + 1 ];

		// read the data from the HINTERNET handle.
		DWORD dwDownloaded = 0;
		if( !InternetReadFile( hHttpRequest, (LPVOID) lpszData, dwSize, &dwDownloaded ) )
		{
			// failed to read the data
			strcpy_s( szLastError, "InternetReadFile failed" );
			delete[] lpszData;
			return NULL;
		}
		
		// add a null terminator to the end of the data buffer.
		//lpszData[dwDownloaded]='\0';

		// Check the size of the remaining data.  If it is zero, break.
		if ( dwDownloaded == 0 ) break;

		// Add part-data to overall data returned
		if ( !pDataReturned )
		{
			// create a new one
			dwDataReturned = dwDownloaded;
			pDataReturned = new char [ dwDataReturned ];
			memcpy( pDataReturned, lpszData, dwDataReturned );
		}
		else
		{
			// recreate a bigger array and add datas to it
			DWORD dwNewDataSize = dwDataReturned + dwDownloaded;
			LPSTR pNewData = new char [ dwNewDataSize ];
			
			memcpy( pNewData, pDataReturned, dwDataReturned );
			memcpy( pNewData + dwDataReturned, lpszData, dwDownloaded );

			// delete old and replace varvalues
			delete [] pDataReturned;
			pDataReturned = pNewData;
			dwDataReturned = dwNewDataSize;
		}

		//if ( m_pProgressBar && dwContentLength > 0 ) 
		//{
			//m_pProgressBar->SetPos( 100*dwDataReturned / dwContentLength );
		//}

		// free usages
		if ( lpszData )
		{
			delete [] lpszData;
			lpszData = NULL;
		}
	}
	while( TRUE && !bCancelTransfer );

	//if ( m_pStatusText ) m_pStatusText->SetWindowText( "Complete" );
	//if ( m_pProgressBar ) m_pProgressBar->SetPos( 0 );

	// return data in ptr
	return pDataReturned;
}

void HTTPComm::PostCounter ( )
{
	if (!Connect( "www.tgcstore.net", false ) ) return;

	char *pHeaderString = "Content-Type: application/x-www-form-urlencoded";
	DWORD dwHeaderSize = (DWORD) strlen( pHeaderString );

	char post [ 128 ] = "check=573286";

	// return var
	DWORD dwDataReturned = 0;
	LPSTR pDataReturned = NULL;

	DWORD dwFlag = INTERNET_FLAG_IGNORE_CERT_CN_INVALID;

	// prepare request
	HINTERNET hHttpRequest = HttpOpenRequest( hInetConnect, "POST", "countupdate.php", "HTTP/1.1", "The Agent", NULL, dwFlag, 0 );
	if ( !hHttpRequest )
	{
		strcpy_s( szLastError, 256, "Failed to open request" );
		return;
	}

	// send request with header
	BOOL bSendResult = HttpSendRequest( hHttpRequest, pHeaderString, dwHeaderSize, post, (DWORD) strlen(post) );
	if ( !bSendResult )
	{
		sprintf_s( szLastError, 256, "Failed to send request: %d", ::GetLastError( ) );
		return;
	}

	Disconnect( );
}

bool HTTPComm::GetLatestVersion( char *szVersion, DWORD dwSize )
{
	if ( !szVersion ) return false;
	if ( dwSize < 1 ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	//MessageBox( NULL, request, "Request", 0 );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "getversion.php", NULL, 0, &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Response", 0 );

	const char* szLatestVersion = strstr( reply, "<latestversion>" );
	if ( szLatestVersion )
	{
		szLatestVersion += strlen( "<latestversion>" );
		const char* szEndLatestVersion = strstr( szLatestVersion, "</latestversion>" );
		if ( szEndLatestVersion )
		{
			DWORD dwLength = (szEndLatestVersion - szLatestVersion);
			if ( dwLength >= dwSize ) dwLength = dwSize - 1;

			strncpy_s( szVersion, dwSize, szLatestVersion, dwLength );
			szVersion [ dwLength ] = '\0';
		}
		
		delete [] reply;
		return szEndLatestVersion != NULL;
	}
	else
	{
		delete [] reply;
		return false;
	}
}

bool HTTPComm::HandleReply( const char *szReply )
{
	char str [ 256 ];

	if ( bCancelTransfer ) return false;

	if ( !szReply ) 
	{
		strcpy_s( str, 256, szLastError );
		strcat_s( str, 256, "\r\n\r\n" );
		strcat_s( str, 256, GetLanguageData( "TGC Store", "NoReply" ) );
		MessageBox( NULL, str, theApp.m_szErrorString, 0 );
		return false;
	}

	if ( *szReply == '\0' )
	{
		strcpy_s( str, 256, szLastError );
		strcat_s( str, 256, "\r\n\r\n" );
		strcat_s( str, 256, GetLanguageData( "TGC Store", "NoReply" ) );
		MessageBox( NULL, str, theApp.m_szErrorString, 0 );
		delete [] szReply;
		return false;
	}

	if ( strncmp( szReply, "<success>", strlen("<success>") ) != 0
		&& strncmp( szReply, "<failed>", strlen("<failed>") ) != 0 )
	{
		CString sError = "Unexpected Error:\r\n\r\n";
		sError += szReply;
		MessageBox( NULL, sError, theApp.m_szErrorString, 0 );
		delete [] szReply;
		return false;
	}

	return true;
}

//reduced popups
bool HTTPComm::HandleReply2( const char *szReply )
{
	char str [ 256 ];

	if ( bCancelTransfer ) return false;

	if ( !szReply ) 
	{
		strcpy_s( str, 256, szLastError );
		strcat_s( str, 256, "\r\n\r\n" );
		strcat_s( str, 256, GetLanguageData( "TGC Store", "NoReply" ) );
		//MessageBox( NULL, str, theApp.m_szErrorString, 0 );
		strcpy_s( szLastError, 256, str );
		return false;
	}

	if ( *szReply == '\0' )
	{
		strcpy_s( str, 256, szLastError );
		strcat_s( str, 256, "\r\n\r\n" );
		strcat_s( str, 256, GetLanguageData( "TGC Store", "NoReply" ) );
		//MessageBox( NULL, str, theApp.m_szErrorString, 0 );
		strcpy_s( szLastError, 256, str );
		delete [] szReply;
		return false;
	}

	if ( strncmp( szReply, "<success>", strlen("<success>") ) != 0
		&& strncmp( szReply, "<failed>", strlen("<failed>") ) != 0 )
	{
		CString sError = "Unexpected Error:\r\n\r\n";
		sError += szReply;
		MessageBox( NULL, sError, theApp.m_szErrorString, 0 );
		//strcpy_s( szLastError, 256, sError );
		delete [] szReply;
		return false;
	}

	return true;
}

bool HTTPComm::UpdateCredits( LPCSTR szSessionID )
{
	if ( !szSessionID ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	char request [ 64 ];
	strcpy_s( request, 64, "sesid=" );
	strcat_s( request, 64, szSessionID );

	//MessageBox( NULL, request, "Request", 0 );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "getpoints.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Response", 0 );

	const char* szCredits = strstr( reply, "<points>" );
	if ( szCredits )
	{
		szCredits += strlen( "<points>" );
		const char* szEndCredits = strstr( szCredits, "</points>" );
		if ( szEndCredits )
		{
			DWORD dwLength = (szEndCredits - szCredits);

			//if ( dwLength != 32 ) MessageBox( NULL, "Invalid string returned from server", "Error", 0 );
			//else
			{
				#ifdef TGC_STORE
				if ( theApp.m_szCredits ) delete [] theApp.m_szCredits;

				theApp.m_szCredits = new char [ dwLength + 1 ];
				strncpy_s( theApp.m_szCredits, dwLength + 1, szCredits, dwLength );
				theApp.m_szCredits [ dwLength ] = '\0';
				#endif
			}
		}
		
		delete [] reply;
		return true;
	}
	else
	{
		delete [] reply;
		return false;
	}
}

bool HTTPComm::Rate( LPCSTR szObjID, LPCSTR szRating )
{
	strcpy_s( szLastError, 256, "Unknown Error" );

	if ( !szObjID || !szRating ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	char request [ 128 ];
	strcpy_s( request, 128, "sesid=" );
	strcat_s( request, 128, theApp.m_szSESID );
	strcat_s( request, 128, "&objid=" );
	strcat_s( request, 128, szObjID );
	strcat_s( request, 128, "&rating=" );
	strcat_s( request, 128, szRating );

	//MessageBox( NULL, request, "Request", 0 );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "rate.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Response", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (szEndError - szError);
			//char* szErrorMessage = new char [ dwLength + 1 ];
			//strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			//szErrorMessage [ dwLength ] = '\0';

			//MessageBox( NULL, szErrorMessage, "Rating Error", 0 );

			//delete [] szErrorMessage;

			strncpy_s( szLastError, 256, szError, dwLength );
			szLastError [ dwLength ] = '\0';
		}

		delete [] reply;
		return false;
	}
	else
	{
		delete [] reply;
		return true;
	}
}

bool HTTPComm::Approve( LPCSTR szObjID )
{
	strcpy_s( szLastError, 256, "Unknown Error" );

	if ( !szObjID ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	char request [ 128 ];
	strcpy_s( request, 128, "sesid=" );
	strcat_s( request, 128, theApp.m_szSESID );
	strcat_s( request, 128, "&objid=" );
	strcat_s( request, 128, szObjID );
	strcat_s( request, 128, "&action=approve" );
	
	//MessageBox( NULL, request, "Request", 0 );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "manage.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Response", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			strcpy_s( szLastError, 256, szErrorMessage );
			MessageBox( NULL, szErrorMessage, "Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}
	else
	{
		delete [] reply;
		return true;
	}
}

bool HTTPComm::Remove( LPCSTR szObjID )
{
	strcpy_s( szLastError, 256, "Unknown Error" );

	if ( !szObjID ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	char request [ 128 ];
	strcpy_s( request, 128, "sesid=" );
	strcat_s( request, 128, theApp.m_szSESID );
	strcat_s( request, 128, "&objid=" );
	strcat_s( request, 128, szObjID );
	strcat_s( request, 128, "&action=remove" );
	
	//MessageBox( NULL, request, "Request", 0 );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "manage.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Response", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			strcpy_s( szLastError, 256, szErrorMessage );
			MessageBox( NULL, szErrorMessage, "Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}
	else
	{
		delete [] reply;
		return true;
	}
}

bool HTTPComm::Restore( LPCSTR szObjID )
{
	strcpy_s( szLastError, 256, "Unknown Error" );

	if ( !szObjID ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	char request [ 128 ];
	strcpy_s( request, 128, "sesid=" );
	strcat_s( request, 128, theApp.m_szSESID );
	strcat_s( request, 128, "&objid=" );
	strcat_s( request, 128, szObjID );
	strcat_s( request, 128, "&action=restore" );
	
	//MessageBox( NULL, request, "Request", 0 );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "manage.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Response", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			strcpy_s( szLastError, 256, szErrorMessage );
			MessageBox( NULL, szErrorMessage, "Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}
	else
	{
		delete [] reply;
		return true;
	}
}

bool HTTPComm::Delete( LPCSTR szObjID )
{
	strcpy_s( szLastError, 256, "Unknown Error" );

	if ( !szObjID ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	char request [ 128 ];
	strcpy_s( request, 128, "sesid=" );
	strcat_s( request, 128, theApp.m_szSESID );
	strcat_s( request, 128, "&objid=" );
	strcat_s( request, 128, szObjID );
	strcat_s( request, 128, "&action=delete" );
	
	//MessageBox( NULL, request, "Request", 0 );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "manage.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Response", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			strcpy_s( szLastError, 256, szErrorMessage );
			MessageBox( NULL, szErrorMessage, "Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}
	else
	{
		delete [] reply;
		return true;
	}
}

bool HTTPComm::Login( LPCSTR szUsername, LPCSTR szPassword )
{
	if ( !szUsername ) return false;
	if ( !szPassword ) return false;

	//if ( strlen(szUsername) > 50 ) return false;
	//if ( strlen(szPassword) > 50 ) return false;

	/*
	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );
	
	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}
	*/

	if ( !Connect( "www.tgcstore.net", true ) )
	{
		MessageBox( NULL, szLastError, theApp.m_szErrorString, 0 );
		return false;
	}

	char request [ 128 ];
	strcpy_s( request, 128, "user=" );
	strcat_s( request, 128, szUsername );
	strcat_s( request, 128, "&pass=" );
	strcat_s( request, 128, szPassword );

	/*
	Connect( "www.darkbasic.com", false );
	char request [ 128 ];
	strcpy_s( request, 128, "email_address=" );
	strcat_s( request, 128, szUsername );
	strcat_s( request, 128, "&password=" );
	strcat_s( request, 128, szPassword );
	*/

	//MessageBox( NULL, request, "Request", 0 );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "login.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Response", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += strlen( "<errormessage>" );
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Login Error", 0 );

			if ( theApp.m_szSESID ) delete [] theApp.m_szSESID;
			theApp.m_szSESID = NULL;

			delete [] szErrorMessage;
		}
	}
	else
	{
		theApp.m_bAgreedTerms = false;

		const char* szTerms = strstr( reply, "<terms>" );
		if ( szTerms )
		{
			szTerms += strlen("<terms>");
			const char* szEndTerms = strstr( szTerms, "</terms>" );
			if ( szEndTerms )
			{
				DWORD dwLength = (szEndTerms - szTerms);
				if ( dwLength > 9 ) dwLength = 9;

				//if ( dwLength != 32 ) MessageBox( NULL, "Invalid string returned from server", "Error", 0 );
				//else
				{
					char str [ 10 ];
					strncpy_s( str, 10, szTerms, dwLength );
					str [ dwLength ] = '\0';
					if ( strcmp( str, "y" ) == 0 ) theApp.m_bAgreedTerms = true;
					else theApp.m_bAgreedTerms = false;

					//MessageBox( NULL, theApp.m_szSESID, "SESID", 0 );
				}
			}
		}

		if ( !theApp.m_bAgreedTerms )
		{
			delete [] reply;
			return true;
		}

		const char* szSESID = strstr( reply, "<sesid>" );
		if ( szSESID )
		{
			szSESID += strlen("<sesid>");
			const char* szEndSESID = strstr( szSESID, "</sesid>" );
			if ( szEndSESID )
			{
				DWORD dwLength = (szEndSESID - szSESID);

				//if ( dwLength != 32 ) MessageBox( NULL, "Invalid string returned from server", "Error", 0 );
				//else
				{
					#ifdef TGC_STORE
					if ( theApp.m_szSESID ) delete [] theApp.m_szSESID;

					theApp.m_szSESID = new char [ dwLength + 1 ];
					strncpy_s( theApp.m_szSESID, dwLength + 1, szSESID, dwLength );
					theApp.m_szSESID [ dwLength ] = '\0';

					//MessageBox( NULL, theApp.m_szSESID, "SESID", 0 );
					#endif
				}
			}
		}
		else
		{
			MessageBox( NULL, "Server did not return enough data", "Server Error", 0 );
			delete [] reply;
			return false;
		}

		const char* szAdmin = strstr( reply, "<admin>" );
		if ( szAdmin )
		{
			szAdmin += strlen( "<admin>" );
			const char* szEndAdmin = strstr( szAdmin, "</admin>" );
			if ( szEndAdmin )
			{
				DWORD dwLength = (szEndAdmin - szAdmin);

				//if ( dwLength != 32 ) MessageBox( NULL, "Invalid string returned from server", "Error", 0 );
				//else
				{
					#ifdef TGC_STORE
					if ( strncmp( szAdmin, "y", dwLength ) == 0 ) theApp.m_bAdmin = true;
					
					//MessageBox( NULL, theApp.m_szSESID, "SESID", 0 );
					#endif
				}
			}
		}

		const char* szUploader = strstr( reply, "<uploader>" );
		if ( szUploader )
		{
			szUploader += strlen( "<uploader>" );
			const char* szEnduploader = strstr( szUploader, "</uploader>" );
			if ( szEnduploader )
			{
				DWORD dwLength = (szEnduploader - szUploader);

				//if ( dwLength != 32 ) MessageBox( NULL, "Invalid string returned from server", "Error", 0 );
				//else
				{
					#ifdef TGC_STORE
					if ( strncmp( szUploader, "y", dwLength ) == 0 ) theApp.m_bUploader = true;
					#endif
				}
			}
		}

		const char* szAuthor = strstr( reply, "<authorname>" );
		if ( szAuthor )
		{
			szAuthor += strlen("<authorname>");
			const char* szEndAuthor = strstr( szAuthor, "</authorname>" );
			if ( szEndAuthor )
			{
				DWORD dwLength = (szEndAuthor - szAuthor);
				if ( dwLength > 29 ) dwLength = 29;

				strncpy_s( theApp.m_szAuthorName, 30, szAuthor, dwLength );
				theApp.m_szAuthorName [ dwLength ] = '\0';
			}
		}

		const char* szCredits = strstr( reply, "<points>" );
		if ( szCredits )
		{
			szCredits += strlen( "<points>" );
			const char* szEndCredits = strstr( szCredits, "</points>" );
			if ( szEndCredits )
			{
				DWORD dwLength = (szEndCredits - szCredits);

				//if ( dwLength != 32 ) MessageBox( NULL, "Invalid string returned from server", "Error", 0 );
				//else
				{
					#ifdef TGC_STORE
					if ( theApp.m_szCredits ) delete [] theApp.m_szCredits;

					theApp.m_szCredits = new char [ dwLength + 1 ];
					strncpy_s( theApp.m_szCredits, dwLength + 1, szCredits, dwLength );
					theApp.m_szCredits [ dwLength ] = '\0';
					#endif
				}
			}
		}
	}

	/*
	FILE *pLocalFile;
	fopen_s( &pLocalFile, szFilename, "wb" );
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;
	*/

	delete [] reply;

	return !szError;
}

void HTTPComm::Logout( )
{
	if ( !Connect( "www.tgcstore.net", false ) ) return;

	DWORD dwWritten = 0;
	const char* reply = RequestData( "logout.php", NULL, 0, &dwWritten );

	Disconnect( );

	if ( reply ) delete [] reply;
}

/*
bool HTTPComm::Search( LPCSTR szFilename, LPCSTR szName, LPCSTR szCategory, LPCSTR szCostLow, LPCSTR szCostHigh, LPCSTR szAuthor, LPCSTR szRatingLow, LPCSTR szRatingHigh, LPCSTR szKeyword )
{
	if ( !szFilename ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );
	
	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	char request [ 256 ]; 
	strcpy_s( request, 256, "" );
	if ( szName && strlen(szName) > 0 ) { strcat_s( request, 256, "name=" ); strcat_s( request, 256, szName ); strcat_s( request, 256, "&" ); }
	if ( szCategory && strlen(szCategory) > 0 ) { strcat_s( request, 256, "category=" ); strcat_s( request, 256, szCategory ); strcat_s( request, 256, "&" ); }
	if ( szCostLow && strlen(szCostLow) > 0 ) { strcat_s( request, 256, "costlow=" ); strcat_s( request, 256, szCostLow ); strcat_s( request, 256, "&" ); }
	if ( szCostHigh && strlen(szCostHigh) > 0 ) { strcat_s( request, 256, "costhigh=" ); strcat_s( request, 256, szCostHigh ); strcat_s( request, 256, "&" ); }
	if ( szAuthor && strlen(szAuthor) > 0 ) { strcat_s( request, 256, "author=" ); strcat_s( request, 256, szAuthor ); strcat_s( request, 256, "&" ); }
	if ( szRatingLow && strlen(szRatingLow) > 0 ) { strcat_s( request, 256, "ratinglow=" ); strcat_s( request, 256, szRatingLow ); strcat_s( request, 256, "&" ); }
	if ( szRatingHigh && strlen(szRatingHigh) > 0 ) { strcat_s( request, 256, "ratinghigh=" ); strcat_s( request, 256, szRatingHigh ); strcat_s( request, 256, "&" ); }
	if ( szKeyword && strlen(szKeyword) > 0 ) { strcat_s( request, 256, "keyword=" ); strcat_s( request, 256, szKeyword ); strcat_s( request, 256, "&" ); }
	if ( theApp.m_szSESID ) { strcat_s( request, 256, "sesid=" ); strcat_s( request, 256, theApp.m_szSESID ); }

	DWORD dwWritten = 0;
	const char* reply = RequestData( "search.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing model list: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}
*/

bool HTTPComm::GetMainModelList( LPCSTR szFilename, bool bExtended )
{
	if ( !szFilename ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	char request [ 64 ];

	if ( bExtended )
	{
		strcpy_s( request, 64, "extended=1" );

		if ( theApp.m_szSESID )
		{
			strcat_s( request, 64, "&sesid=" );
			strcat_s( request, 64, theApp.m_szSESID );
		}
	}
	else
	{
		if ( theApp.m_szSESID )
		{
			strcpy_s( request, 64, "sesid=" );
			strcat_s( request, 64, theApp.m_szSESID );
		}
		else strcpy_s( request, 64, "" );
	}

	DWORD dwWritten = 0;
	const char* reply = RequestData( "objectlist.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing model list: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetGroupList( LPCSTR szFilename )
{
	if ( !szFilename ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	char request [ 64 ];
	
	if ( theApp.m_szSESID )
	{
		strcpy_s( request, 64, "sesid=" );
		strcat_s( request, 64, theApp.m_szSESID );
	}
	else
	{
		strcpy_s( request, 64, "" );
	}

	DWORD dwWritten = 0;
	const char* reply = RequestData( "grouplist.php", request, (DWORD) strlen( request ), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing group list: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetGroupObjects( LPCSTR szFilename, int group )
{
	if ( !szFilename ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	char szGroup [ 12 ];
	sprintf_s( szGroup, 12, "%d", group );

	char request [ 64 ];
	strcpy_s( request, 64, "group=" );
	strcat_s( request, 64, szGroup );

	if ( theApp.m_szSESID )
	{
		strcat_s( request, 64, "&sesid=" );
		strcat_s( request, 64, theApp.m_szSESID );
	}
	
	DWORD dwWritten = 0;
	const char* reply = RequestData( "objectlist2.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing model list: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetFreeObjects( LPCSTR szFilename )
{
	if ( !szFilename ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	char request [ 64 ];
	strcpy_s( request, 64, "free=1" );

	if ( theApp.m_szSESID )
	{
		strcat_s( request, 64, "&sesid=" );
		strcat_s( request, 64, theApp.m_szSESID );
	}
	
	DWORD dwWritten = 0;
	const char* reply = RequestData( "objectlist2.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing model list: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetPurchasedList( LPCSTR szFilename )
{
	if ( !szFilename ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	char request [ 64 ];
	strcpy_s( request, 64, "" );

	if ( theApp.m_szSESID )
	{
		strcpy_s( request, 64, "sesid=" );
		strcat_s( request, 64, theApp.m_szSESID );

		//MessageBox( NULL, request, "Info", 0 );
	}

	DWORD dwWritten = 0;
	const char* reply = RequestData( "getpurchasedlist.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	//if ( !HandleReply( reply ) ) return false;
	if ( !reply ) return false;
	if ( strlen(reply) == 0 ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing purchased list: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetSearchList( LPCSTR szFilename )
{
	if ( !szFilename ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	char request [ 64 ];
	strcpy_s( request, 64, "" );

	if ( theApp.m_szSESID )
	{
		strcpy_s( request, 64, "sesid=" );
		strcat_s( request, 64, theApp.m_szSESID );
	}

	DWORD dwWritten = 0;
	const char* reply = RequestData( "getcompletelist2.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	//if ( !HandleReply( reply ) ) return false;
	if ( !reply ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing search list: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetKeywordList( LPCSTR szFilename )
{
	if ( !szFilename ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "keywordlist2.php", NULL, 0, &dwWritten );

	Disconnect( );

	//if ( !HandleReply( reply ) ) return false;
	if ( !reply ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing keyword list: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetModelDetails( DWORD dwModelID, LPCSTR szSESID )
{
	char szFilename [ MAX_PATH ] = "Files\\TGCStore\\TEMP\\Object.xml";

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !Connect( "www.tgcstore.net", false ) ) return false;
	//Connect( "127.0.0.1", false );

	char request [ 128 ];
	sprintf_s( request, 128, "id=%d", dwModelID );
	if ( szSESID )
	{
		strcat_s( request, 128, "&sesid=" );
		strcat_s( request, 128, szSESID );
	}

	DWORD dwWritten = 0;
	const char* reply = RequestData( "objectdetails.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		strcpy_s( szLastError, "Unknown Error" );

		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Details Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		char str [ 64 ];
		sprintf_s( str, 64, "Error writing model file: %d", error );
		MessageBox( NULL, str, "Error", 0 );
		delete [] reply;
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetImages( LPCSTR szFolder, int iGroup, CString sNeeded )
{
	if ( !szFolder ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	DWORD dwLength = sNeeded.GetLength() + 64;
	char *request = new char [ dwLength ];
	sprintf_s( request, 64, "group=%d", iGroup );
	if ( sNeeded.GetLength() > 0 )
	{
		strcat_s( request, dwLength, "&objectlist=" );
		strcat_s( request, dwLength, sNeeded );
	}

	DWORD dwWritten = 0;
	const char* reply = RequestData( "getthumbnails.php", request, strlen( request ), &dwWritten );
	delete [] request;

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	//char dir [ MAX_PATH ];
	//GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );
	
	const char* szRemaining = szFolder;
	const char* szSlash = strchr( szFolder, '\\' );
	
	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( strncmp( reply, "<failed>", strlen("<failed>") ) == 0 )
	{
		const char* szError = strstr( reply, "<errormessage>" );
		if ( szError )
		{
			szError += 14;
			const char* szEndError = strstr( szError, "</errormessage>" );
			if ( szEndError )
			{
				DWORD dwLength = (szEndError - szError);
				char* szErrorMessage = new char [ dwLength + 1 ];
				strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
				szErrorMessage [ dwLength ] = '\0';

				strcpy_s( szLastError, 256, szErrorMessage );
				MessageBox( NULL, szErrorMessage, "Thumbnail Error", 0 );

				delete [] szErrorMessage;
			}
		}

		delete [] reply;
		return false;
	}
	else
	{
		if ( strncmp( reply, "<success>", strlen("<success>") ) != 0 )
		{
			delete [] reply;
			return false;
		}

		const char *pPtr = reply + strlen("<success>");
		
		while ( strncmp( pPtr, "<file>", strlen("<file>") ) == 0 )
		{
			pPtr += strlen( "<file>" );
			if ( strncmp( pPtr, "<size>", strlen("<size>") ) != 0 ) 
			{
				MessageBox( NULL, "Size missing", "Thumbnail Error" , 0 );
				continue;
			}
			
			pPtr += strlen( "<size>" );
			const char* szEnd = strstr( pPtr, "</size>" );
			if ( !szEnd ) 
			{
				MessageBox( NULL, "Size missing: 2", "Thumbnail Error" , 0 );
				delete [] reply;
				return true;
			}

			DWORD dwLength = (szEnd - pPtr);
			char* szSize = new char [ dwLength + 1 ];
			strncpy_s( szSize, dwLength + 1, pPtr, dwLength );
			szSize [ dwLength ] = '\0';
			int iSize = atoi( szSize );
			delete [] szSize; szSize = NULL;

			pPtr = szEnd;
			pPtr += strlen( "</size>" );

			if ( iSize <= 0 ) continue;
			if ( strncmp( pPtr, "<name>", strlen("<name>") ) != 0 ) 
			{
				MessageBox( NULL, "Name missing", "Thumbnail Error" , 0 );
				continue;
			}
			
			pPtr += strlen( "<name>" );
			szEnd = strstr( pPtr, "</name>" );
			if ( !szEnd ) 
			{
				MessageBox( NULL, "Name missing: 2", "Thumbnail Error" , 0 );
				delete [] reply;
				return true;
			}

			dwLength = (szEnd - pPtr);
			char* szName = new char [ dwLength + 1 ];
			strncpy_s( szName, dwLength + 1, pPtr, dwLength );
			szName [ dwLength ] = '\0';

			pPtr = szEnd;
			pPtr += strlen( "</name>" );

			if ( strncmp( pPtr, "<data>", strlen("<data>") ) != 0 ) 
			{
				MessageBox( NULL, "Data missing", "Thumbnail Error" , 0 );
				continue;
			}

			pPtr += strlen( "<data>" );
			char* pData = new char [ iSize ];
			memcpy_s( pData, iSize, pPtr, iSize );

			pPtr += iSize;
			if ( strncmp( pPtr, "</data>", strlen("</data>") ) != 0 ) 
			{
				MessageBox( NULL, "Thumbnail Data Error", "Thumbnail Error", 0 );
				continue;
			}

			EnterCriticalSection( &theApp.m_csDirectoryChanges );

			SetCurrentDirectory( theApp.m_szDirectory );
			SetCurrentDirectory( szFolder );

			FILE *pLocalFile;
			fopen_s( &pLocalFile, szName, "wb" );
			fwrite( pData, 1, iSize, pLocalFile );
			fclose( pLocalFile );
			pLocalFile = 0;

			LeaveCriticalSection( &theApp.m_csDirectoryChanges );

			delete [] szName; szName = NULL;
			delete [] pData; pData = NULL;
			
			pPtr += strlen( "</data>" );
			pPtr += strlen( "</file>" );
		}

		delete [] reply;
		return true;
	}

	//SetCurrentDirectory( dir );

	delete [] reply;

	return true;
}

bool HTTPComm::GetModel( DWORD dwModelID, DWORD dwProjectedCost, LPCSTR szFilename )
{
	if ( !szFilename ) return false;

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		//MessageBox( NULL, szLastError, theApp.m_szErrorString, 0 );
		return false;
	}

	char szModelID [ 20 ];
	sprintf_s( szModelID, 20, "%d", dwModelID );
	char szProjectedCost [ 20 ];
	sprintf_s( szProjectedCost, 20, "%d", dwProjectedCost );
	
	char request [ 128 ];
	strcpy_s( request, 128, "objid=" );
	strcat_s( request, 128, szModelID );
	strcat_s( request, 128, "&sesid=" );
	strcat_s( request, 128, theApp.m_szSESID );
	strcat_s( request, 128, "&projectedcost=" );
	strcat_s( request, 128, szProjectedCost );

	CString sFullPath = theApp.m_szDirectory;
	sFullPath += "\\";
	sFullPath += szFilename;

	DWORD dwWritten = 0;
	const char* reply = RequestData( "getmodel.php", request, (DWORD) strlen(request), &dwWritten, sFullPath );

	Disconnect( );

	//if ( !HandleReply( reply ) ) return false;
	if ( !reply )
	{
		//MessageBox( NULL, "No response from server", "Server Error", 0 );
		return false;
	}
	
	if ( strncmp( reply, "<failed>", strlen("<failed>") ) == 0 )
	{
		strcpy_s( szLastError, 256, "Unknown Error" );

		const char* szError = strstr( reply, "<errormessage>" );
		if ( szError )
		{
			szError += 14;
			const char* szEndError = strstr( szError, "</errormessage>" );
			if ( szEndError )
			{
				DWORD dwLength = (szEndError - szError);
				if ( dwLength > 255 ) dwLength = 255;
				char* szErrorMessage = new char [ dwLength + 1 ];
				strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
				szErrorMessage [ dwLength ] = '\0';

				strcpy_s( szLastError, 256, szErrorMessage );

				delete [] szErrorMessage;
			}
		}

		delete [] reply;
		return false;
	}

	/*
	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	fopen_s( &pLocalFile, szFilename, "wb" );
	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
	*/

	delete [] reply;

	return true;
}

bool HTTPComm::GetLicense( DWORD dwObjID, LPCSTR szFilename, LPCSTR szSESID )
{
	if ( !szFilename ) return false;
	if ( !szSESID ) return false;

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		//MessageBox( NULL, szLastError, theApp.m_szErrorString, 0 );
		return false;
	}

	char szModelID [ 20 ];
	sprintf_s( szModelID, 20, "%d", dwObjID );
	
	char request [ 128 ];
	strcpy_s( request, 128, "objid=" );
	strcat_s( request, 128, szModelID );
	strcat_s( request, 128, "&sesid=" );
	strcat_s( request, 128, szSESID );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "getlicense.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply2( reply ) ) return false;
		
	if ( strncmp( reply, "<failed>", strlen("<failed>") ) == 0 )
	{
		strcpy_s( szLastError, 256, "Unknown Error" );

		const char* szError = strstr( reply, "<errormessage>" );
		if ( szError )
		{
			szError += strlen( "<errormessage>" );
			const char* szEndError = strstr( szError, "</errormessage>" );
			if ( szEndError )
			{
				DWORD dwLength = (szEndError - szError);
				if ( dwLength > 255 ) dwLength = 255;
				char* szErrorMessage = new char [ dwLength + 1 ];
				strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
				szErrorMessage [ dwLength ] = '\0';

				strcpy_s( szLastError, 256, szErrorMessage );

				delete [] szErrorMessage;
			}
		}

		delete [] reply;
		return false;
	}

	char* szLicenseText = NULL;

	const char* szLicense = strstr( reply, "<license>" );
	if ( szLicense )
	{
		szLicense += strlen( "<license>" );
		const char* szEndLicense = strstr( szLicense, "</license>" );
		if ( szEndLicense )
		{
			DWORD dwLength = (szEndLicense - szLicense);
			if ( dwLength > 0 )
			{
				dwWritten = dwLength;
				
				szLicenseText = new char [ dwLength + 1 ];
				strncpy_s( szLicenseText, dwLength + 1, szLicense, dwLength );
				szLicenseText [ dwLength ] = '\0';
			}
			else
			{
				strcpy_s( szLastError, 256, "Failed to get license data" );
				return false;
			}
		}
	}

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile;
	fopen_s( &pLocalFile, szFilename, "wb" );
	if ( !pLocalFile )
	{
		strcpy_s( szLastError, 256, GetLanguageData( _T("TGC Store"), _T("LicenseFile") ) );
		return false;
	}

	if ( szLicenseText ) 
	{
		fwrite( szLicenseText, 1, dwWritten, pLocalFile );
		delete [] szLicenseText;
	}
	else fwrite( reply, 1, dwWritten, pLocalFile );
	
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;

	return true;
}

bool HTTPComm::GetX9Serial( DWORD dwObjID, LPCSTR szSESID, LPSTR szSerialOut, DWORD dwSize )
{
	strcpy_s( szLastError, 256, "Unknown Error" );

	if ( !szSESID ) return false;
	if ( !szSerialOut ) return false;

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		//MessageBox( NULL, szLastError, theApp.m_szErrorString, 0 );
		return false;
	}

	char szModelID [ 20 ];
	sprintf_s( szModelID, 20, "%d", dwObjID );
	
	char request [ 128 ];
	strcpy_s( request, 128, "objid=" );
	strcat_s( request, 128, szModelID );
	strcat_s( request, 128, "&sesid=" );
	strcat_s( request, 128, szSESID );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "getX9serial.php", request, (DWORD) strlen(request), &dwWritten );

	Disconnect( );

	if ( !HandleReply2( reply ) ) return false;

	if ( strncmp( reply, "<failed>", strlen("<failed>") ) == 0 )
	{
		const char* szError = strstr( reply, "<errormessage>" );
		if ( szError )
		{
			szError += strlen( "<errormessage>" );
			const char* szEndError = strstr( szError, "</errormessage>" );
			if ( szEndError )
			{
				DWORD dwLength = (szEndError - szError);
				if ( dwLength > 255 ) dwLength = 255;
				char* szErrorMessage = new char [ dwLength + 1 ];
				strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
				szErrorMessage [ dwLength ] = '\0';

				strcpy_s( szLastError, 256, szErrorMessage );

				delete [] szErrorMessage;
			}
		}

		delete [] reply;
		return false;
	}

	bool bResult = false;

	const char* szSerial = strstr( reply, "<serial>" );
	if ( szSerial )
	{
		szSerial += strlen( "<serial>" );
		const char* szEndSerial = strstr( szSerial, "</serial>" );
		if ( szEndSerial )
		{
			DWORD dwLength = (szEndSerial - szSerial);
			if ( dwLength > 0 )
			{
				if ( dwLength < dwSize )
				{				
					strncpy_s( szSerialOut, dwSize, szSerial, dwLength );
					szSerialOut [ dwLength ] = '\0';
					bResult = true;
				}
				else
				{
					strcpy_s( szLastError, 256, "Serial number too long, please contact support" );
				}
			}
			else 
			{
				strcpy_s( szLastError, 256, "Failed to get serial number, please contact support" );
			}
		}
	}
	else
	{
		strcpy_s( szLastError, 256, "Failed to get serial number, please contact support" );
	}

	delete [] reply;
	return bResult;
}

bool HTTPComm::GetPreviewImage( LPCSTR szImage, LPCSTR szFilename )
{
	if ( !szFilename ) return false;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;

	//MessageBox( NULL, szImage, "Info", 0 );

	//bool bNotRight = !strstr( szFilename, "\\Images\\" );


	DWORD dwWritten = 0;
	const char* reply = RequestData( szImage, NULL, 0, &dwWritten, NULL, false );

	//MessageBox( NULL, reply, "info", 0 );

	Disconnect( );

	if ( !reply ) return false;
	//if ( !HandleReply( reply ) ) return false;

	const char* szRemaining = szFilename;
	const char* szSlash = strchr( szFilename, '\\' );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	char origdir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, origdir );
	SetCurrentDirectory( theApp.m_szDirectory );

	while ( szSlash )
	{
		char dir [ 260 ];
		strncpy_s( dir, 260, szRemaining, (szSlash - szRemaining) );

		if ( !SetCurrentDirectory( dir ) )
		{
			CreateDirectory( dir, NULL );
			SetCurrentDirectory( dir );
		}

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	SetCurrentDirectory( theApp.m_szDirectory );

	FILE *pLocalFile = 0;
	int error = fopen_s( &pLocalFile, szFilename, "wb" );
	if ( error )
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		strcpy_s( szLastError, 256, "Unable to save downloaded file" );
		if ( pLocalFile ) fclose( pLocalFile );
		return false;
	}

	fwrite( reply, 1, dwWritten, pLocalFile );
	fclose( pLocalFile );
	pLocalFile = 0;

	SetCurrentDirectory( origdir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	delete [] reply;
	return true;
}

DWORD HTTPComm::CreateObject( const char *szName, const char *szCost, const char *szType, const char *szCategory, const char *szSubCategory,
					   const char *szPolygons, const char *szKeywords, const char *szDescription, const char *szVideo, 
					   const char *szNumImages, const char *szGroupID, int iX9Compatible, int iX10Compatible )
{
	DWORD dwNewObjID = 0;

	if ( !theApp.m_szSESID ) return 0;

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return 0;
	}
	//Connect( "127.0.0.1", false );

	char request [ 2048 ];
	strcpy_s( request, 2048, "sesid=" );
	strcat_s( request, 2048, theApp.m_szSESID );
	strcat_s( request, 2048, "&name=" );
	strcat_s( request, 2048, szName );
	strcat_s( request, 2048, "&cost=" );
	strcat_s( request, 2048, szCost );
	strcat_s( request, 2048, "&type=" );
	strcat_s( request, 2048, szType );
	strcat_s( request, 2048, "&category=" );
	strcat_s( request, 2048, szCategory );
	strcat_s( request, 2048, "&subcategory=" );
	strcat_s( request, 2048, szSubCategory );
	strcat_s( request, 2048, "&polygons=" );
	strcat_s( request, 2048, szPolygons );
	strcat_s( request, 2048, "&keywords=" );
	strcat_s( request, 2048, szKeywords );
	strcat_s( request, 2048, "&description=" );
	strcat_s( request, 2048, szDescription );
	strcat_s( request, 2048, "&video=" );
	strcat_s( request, 2048, szVideo );
	strcat_s( request, 2048, "&numimages=" );
	strcat_s( request, 2048, szNumImages );
	strcat_s( request, 2048, "&group=" );
	strcat_s( request, 2048, szGroupID );

	char str [ 64 ];
	sprintf_s( str, 64, "&x9compatible=%d&x10compatible=%d", iX9Compatible, iX10Compatible );

	strcat_s( request, 2048, str );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "createobject.php", request, (DWORD) strlen( request ), &dwWritten );

	Disconnect( );

	//if ( !reply ) return 0;
	if ( !HandleReply( reply ) ) return 0;

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (DWORD) (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Upload Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return 0;
	}
	else
	{
		const char* szID = strstr( reply, "<objectid>" );
		if ( szID )
		{
			szID += 10;
			const char* szEndID = strstr( szID, "</objectid>" );
			if ( szEndID )
			{
				DWORD dwLength = (DWORD) (szEndID - szID);

				CString sTemp;
				sTemp.SetString( szID, dwLength );
				//MessageBox( NULL, sTemp, "Done ", 0 );
				dwNewObjID = atoi( sTemp );
			}
		}
	}

	delete [] reply;
	return dwNewObjID;
}

bool HTTPComm::EditObject( DWORD dwObjID, int iX9Compatible, int iX10Compatible, const char *szCost, const char *szPolygons, 
						    const char *szDescription, const char *szVideo, const char *szKeywords, int iResetRating, int iResetDownloads )
{
	if ( !theApp.m_szSESID ) return false;

	if ( !Connect( "www.tgcstore.net", false ) )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	char request [ 1500 ];
	strcpy_s( request, 1500, "sesid=" );
	strcat_s( request, 1500, theApp.m_szSESID );
	strcat_s( request, 1500, "&cost=" );
	strcat_s( request, 1500, szCost );
	strcat_s( request, 1500, "&polygons=" );
	strcat_s( request, 1500, szPolygons );
	strcat_s( request, 1500, "&description=" );
	strcat_s( request, 1500, szDescription );
	strcat_s( request, 1500, "&video=" );
	strcat_s( request, 1500, szVideo );
	strcat_s( request, 1500, "&keywords=" );
	strcat_s( request, 1500, szKeywords );
	
	char str [ 256 ];
	sprintf_s( str, 256, "&x9compatible=%d&x10compatible=%d&resetrating=%d&resetdownloadcount=%d&objid=%d", iX9Compatible, iX10Compatible, iResetRating, iResetDownloads, dwObjID );

	strcat_s( request, 1500, str );

	DWORD dwWritten = 0;
	const char* reply = RequestData( "editobject.php", request, (DWORD) strlen( request ), &dwWritten );

	Disconnect( );

	//if ( !reply ) return 0;
	if ( !HandleReply( reply ) ) return false;

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += strlen("<errormessage>");
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (DWORD) (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Edit Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}

	delete [] reply;
	return true;
}

bool HTTPComm::UploadModelFile( DWORD dwObjID, const char *szFilename, bool bEditing )
{
	//return true;
	if ( !Connect( "www.tgcstore.net", false ) ) 
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("FailedConnect") ), theApp.m_szErrorString, 0 );
		return false;
	}
	//Connect( "127.0.0.1", false );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	SetCurrentDirectory( theApp.m_szDirectory );
	SetCurrentDirectory( "Files\\TGCStore\\Temp" );

	FILE *pFile = NULL;
	int error = fopen_s( &pFile, szFilename, "rb" );
	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
	if ( error )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("ModelFileFailed") ), "Upload Error", 0 );
		return false;
	}

	fseek( pFile, 0, SEEK_END );
	long lSize = ftell( pFile );
	rewind( pFile );

	char *pFileData = new char [ lSize ];
	fread( pFileData, 1, lSize, pFile );
	fclose( pFile );
	pFile = NULL;

	DWORD dwWritten = 0;
	const char* reply = SendData( "UploadModel.php", szFilename, dwObjID, bEditing, "application/x-zip-compressed", pFileData, lSize );

	Disconnect( );

	delete [] pFileData;

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Info", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (DWORD) (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Upload Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}

	delete [] reply;
	return true;
}

bool HTTPComm::UploadThumbnail( DWORD dwObjID, const char *szFilename, bool bEditing )
{
	if ( !Connect( "www.tgcstore.net", false ) ) return false;
	//Connect( "127.0.0.1", false );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	SetCurrentDirectory( theApp.m_szDirectory );
	SetCurrentDirectory( "Files\\TGCStore\\Temp" );

	FILE *pFile = NULL;
	int error = fopen_s( &pFile, szFilename, "rb" );
	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
	if ( error )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("ThumbnailFailed") ), "Upload Error", 0 );
		return false;
	}

	fseek( pFile, 0, SEEK_END );
	long lSize = ftell( pFile );
	rewind( pFile );

	char *pFileData = new char [ lSize ];
	fread( pFileData, 1, lSize, pFile );
	fclose( pFile );
	pFile = NULL;

	DWORD dwWritten = 0;
	const char* reply = SendData( "UploadThumbnail.php", szFilename, dwObjID, bEditing, "image/bmp", pFileData, lSize );

	Disconnect( );

	delete [] pFileData;

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Info", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (DWORD) (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Upload Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}

	delete [] reply;
	return true;
}

bool HTTPComm::UploadPreviewImage( DWORD dwObjID, const char *szFilename, bool bEditing )
{
	//return true;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;
	//Connect( "127.0.0.1", false );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	SetCurrentDirectory( theApp.m_szDirectory );
	//SetCurrentDirectory( "Files\\TGCStore\\Uploads" );

	SetCurrentDirectory( "Files\\TGCStore" );
	if ( !SetCurrentDirectory( "Uploads" ) )
	{
		CreateDirectory( "Uploads", NULL );
		if ( !SetCurrentDirectory( "Uploads" ) )
		{
			  LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			  MessageBox( NULL, GetLanguageData( "TGC Store", "InvalidUploadPath" ), theApp.m_szErrorString, 0 );
			  return false;
		}
	}

	FILE *pFile = NULL;
	int error = fopen_s( &pFile, szFilename, "rb" );
	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
	if ( error )
	{
		char str [ MAX_PATH + 32 ];
		sprintf_s( str, MAX_PATH + 32, "%s: %s", GetLanguageData( _T("TGC Store"), _T("PreviewImageFailed") ), szFilename );
		MessageBox( NULL, str, "Upload Error", 0 );
		return false;
	}

	fseek( pFile, 0, SEEK_END );
	long lSize = ftell( pFile );
	rewind( pFile );

	char *pFileData = new char [ lSize ];
	fread( pFileData, 1, lSize, pFile );
	fclose( pFile );
	pFile = NULL;

	DWORD dwWritten = 0;
	const char* reply = SendData( "UploadPreview.php", szFilename, dwObjID, bEditing, "image/jpeg", pFileData, lSize );

	Disconnect( );

	delete [] pFileData;

	if ( !HandleReply( reply ) ) return false;

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (DWORD) (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Upload Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}

	delete [] reply;
	return true;
}

bool HTTPComm::ClearPreviewImages( DWORD dwObjID )
{
	//return true;

	if ( !Connect( "www.tgcstore.net", false ) ) return false;
	//Connect( "127.0.0.1", false );

	char szID [ 12 ];
	sprintf_s( szID, 12, "%d", dwObjID );

	char request [ 100 ];
	strcpy_s( request, 100, "sesid=" );
	strcat_s( request, 100, theApp.m_szSESID );
	strcat_s( request, 100, "&objid=" );
	strcat_s( request, 100, szID );
	
	DWORD dwWritten = 0;
	const char* reply = RequestData( "ClearPreviews.php", request, (DWORD) strlen( request ), &dwWritten );

	Disconnect( );

	if ( !HandleReply( reply ) ) return false;

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (DWORD) (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Server Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}

	delete [] reply;
	return true;
}

bool HTTPComm::UploadMusicSample( DWORD dwObjID, const char *szFilename, bool bEditing )
{
	if ( !Connect( "www.tgcstore.net", false ) ) return false;
	//Connect( "127.0.0.1", false );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	SetCurrentDirectory( theApp.m_szDirectory );
	SetCurrentDirectory( "Files\\TGCStore\\Temp" );

	FILE *pFile = NULL;
	int error = fopen_s( &pFile, szFilename, "rb" );
	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
	if ( error )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("OpenMusicFailed") ), "Upload Error", 0 );
		return false;
	}

	fseek( pFile, 0, SEEK_END );
	long lSize = ftell( pFile );
	rewind( pFile );

	char *pFileData = new char [ lSize ];
	fread( pFileData, 1, lSize, pFile );
	fclose( pFile );
	pFile = NULL;

	DWORD dwWritten = 0;
	const char* reply = SendData( "UploadMusicSample.php", szFilename, dwObjID, bEditing, "audio/ogg", pFileData, lSize );

	Disconnect( );

	delete [] pFileData;

	if ( !HandleReply( reply ) ) return false;

	//MessageBox( NULL, reply, "Info", 0 );

	const char* szError = strstr( reply, "<errormessage>" );
	if ( szError )
	{
		szError += 14;
		const char* szEndError = strstr( szError, "</errormessage>" );
		if ( szEndError )
		{
			DWORD dwLength = (DWORD) (szEndError - szError);
			char* szErrorMessage = new char [ dwLength + 1 ];
			strncpy_s( szErrorMessage, dwLength + 1, szError, dwLength );
			szErrorMessage [ dwLength ] = '\0';

			MessageBox( NULL, szErrorMessage, "Upload Error", 0 );

			delete [] szErrorMessage;
		}

		delete [] reply;
		return false;
	}

	delete [] reply;
	return true;
}

bool HTTPComm::Disconnect ( void )
{
	// Close Internet-Connect
	if( hInetConnect )
	{
		InternetCloseHandle( hInetConnect );
		hInetConnect = NULL;
	}

	/*
	// Close Internet-Open
	if( hInet )
	{
		InternetCloseHandle( hInet );
		hInet = NULL;
	}
	*/

	// complete
	return true;
}

#endif
