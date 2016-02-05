#include "stdafx.h"

#ifdef TGC_STORE

#include "ServerDownloader.h"
#include "cMarketImage.h"
#include "MarketTree.h"
#include "..\Editor.h"
#include "MarketWindow.h"
#include "unzip.h"

extern bool g_bFreeVersion;

ServerDownloader::ServerDownloader( )
{
	//m_pTree = NULL;
	//m_pList = NULL;
	m_pWindow = NULL;
	m_bDownloading = false;
	m_iDownloadType = -1;
	m_bExtendedList = false;
	m_bSuccess = false;

	m_bTerminate = false;

	m_szModelFilename = 0;
	m_dwObjID = 0;
	m_iGroupID = 0;
	m_dwProjectedCost = 0;
	m_iGroup = 0;
	m_sThumbnailsNeeded = "";

	m_pRequestQueue = NULL;
	m_pRequestTail = NULL;
	m_hMutex = CreateMutex( NULL, FALSE, NULL );
}

ServerDownloader::~ServerDownloader( )
{
	Cancel( );
	Terminate();
	//Join();
	if ( m_szModelFilename ) delete [] m_szModelFilename;

	CloseHandle( m_hMutex );
}

void ServerDownloader::SetLoginProcess( CMarketWindow *pParent, const char *szUser, const char *szPass, bool bSilent )
{
	m_pWindow = pParent;
	m_iDownloadType = 6;
	m_sUser = szUser;
	m_sPass = szPass;
	m_bSilentLogin = bSilent;

	m_Server.BeginTransfer( );
	m_bTerminate = false;
}

void ServerDownloader::SetGroupListDownload( CMarketWindow *pParent )
{
	m_pWindow = pParent;
	m_iDownloadType = 3;

	m_Server.BeginTransfer( );
	m_bTerminate = false;
}

void ServerDownloader::SetObjectListDownload( CMarketWindow *pParent, int group )
{
	m_pWindow = pParent;
	m_iDownloadType = 0;
	m_iGroup = group;

	m_Server.BeginTransfer( );
	m_bTerminate = false;
}

void ServerDownloader::SetSearchListDownload( CMarketWindow *pParent )
{
	m_pWindow = pParent;
	m_iDownloadType = 4;

	m_Server.BeginTransfer( );
	m_bTerminate = false;
}

void ServerDownloader::SetPurchasedListDownload( CMarketWindow *pParent )
{
	m_pWindow = pParent;
	m_iDownloadType = 5;

	m_Server.BeginTransfer( );
	m_bTerminate = false;
}

/*
	Request *pNewRequest = new Request();
	pNewRequest->pParent = pParent;
	pNewRequest->dwType = 0;

	DWORD result = WaitForSingleObject( m_hMutex, INFINITE );
	if ( result = WAIT_OBJECT_0 )
	{
		if ( m_pRequestTail ) m_pRequestTail->pNextRequest = pNewRequest;
		else
		{
			m_pRequestQueue = pNewRequest;
			m_pRequestTail = pNewRequest;
		}
	}
	else
	{
		MessageBox( NULL, "Unexpected wait result", "Warning", 0 );
	}

	ReleaseMutex( m_hMutex );
*/

void ServerDownloader::SetThumbnailDownload( int group, CMarketWindow *pParent, CString sNeeded )
{
	m_pWindow = pParent;
	m_iGroupID = group;
	m_iDownloadType = 2;

	m_Server.BeginTransfer( );
	m_bTerminate = false;
	m_sThumbnailsNeeded = sNeeded;

/*
	Request *pNewRequest = new Request();
	pNewRequest->pParent = pParent;
	pNewRequest->dwGroup = group;
	pNewRequest->dwType = 2;

	DWORD result = WaitForSingleObject( m_hMutex, INFINITE );
	if ( result = WAIT_OBJECT_0 )
	{
		if ( m_pRequestTail ) m_pRequestTail->pNextRequest = pNewRequest;
		else
		{
			m_pRequestQueue = pNewRequest;
			m_pRequestTail = pNewRequest;
		}
	}
	else
	{
		MessageBox( NULL, "Unexpected wait result", "Warning", 0 );
	}

	ReleaseMutex( m_hMutex );
*/
}

void ServerDownloader::SetMusicSampleDownload( const char *szServerFilename, const char *szSaveFilename )
{
	m_pWindow = NULL;
	m_iGroupID = 0;
	m_iDownloadType = 7;

	m_Server.BeginTransfer( );
	m_bTerminate = false;
	m_bSuccess = false;
	
	sString1 = szServerFilename;
	sString2 = szSaveFilename;
}

void ServerDownloader::SetModelDownload( LPCSTR szFilename, const MarketObject* pObject, DWORD dwProjectedCost, CMarketWindow *pParent )
{
	if ( !pObject )
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("ItemError") ), theApp.m_szErrorString, 0 );
		m_iDownloadType = -1;
		return;
	}

	m_pWindow = pParent;
	m_dwObjID = pObject->GetID();
	m_dwProjectedCost = dwProjectedCost;
	m_iDownloadType = 1;

	m_Server.BeginTransfer( );
	m_bTerminate = false;

	/*
	MarketObject *pObject = MarketObject::GetObject( m_dwObjID );
	if ( !pObject ) 
	{
		MessageBox( NULL, GetLanguageData( _T("TGC Store"), _T("ItemError") ), theApp.m_szErrorString, 0 );
		m_iDownloadType = -1;
		return;
	}
	*/

	cLocalObject.Copy( pObject );
	m_iGroup = cLocalObject.GetGroupID( );

	DWORD dwLength = strlen( szFilename );

	if ( m_szModelFilename ) delete [] m_szModelFilename;
	m_szModelFilename = new char [ dwLength + 1 ];
	strcpy_s( m_szModelFilename, dwLength+1, szFilename );

	const char* szRemaining = m_szModelFilename;
	const char* szSlash = strchr( m_szModelFilename, '\\' );

/*
	Request *pNewRequest = new Request();
	pNewRequest->pParent = pParent;
	pNewRequest->dwObjectID = dwObjectID;
	pNewRequest->dwType = 1;
	pNewRequest->szFilename = new char [ dwLength + 1 ];
	strcpy_s( pNewRequest->szFilename, dwLength + 1, szFilename );

	const char* szRemaining = pNewRequest->szFilename;
	const char* szSlash = strchr( pNewRequest->szFilename, '\\' );
*/

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


/*
	DWORD result = WaitForSingleObject( m_hMutex, INFINITE );
	if ( result = WAIT_OBJECT_0 )
	{
		if ( m_pRequestTail ) m_pRequestTail->pNextRequest = pNewRequest;
		else
		{
			m_pRequestQueue = pNewRequest;
			m_pRequestTail = pNewRequest;
		}
	}
	else
	{
		MessageBox( NULL, "Unexpected wait result", "Warning", 0 );
	}

	ReleaseMutex( m_hMutex );
	*/
}

void ServerDownloader::Cancel( )
{
	//Thread::Terminate( );
	m_bTerminate = true;
	m_Server.CancelTransfer( );
	//Join();

	//m_pTree = 0;
	//m_pList = 0;
	//m_pWindow = NULL;
}

unsigned ServerDownloader::Run( )
{
	m_bDownloading = true;

	switch( m_iDownloadType )
	{
		case 0 :
		{
			m_bDownloading = true;
			CString sFilename;
			bool bResult = false;

			if ( m_iGroup > 0 )
			{
				sFilename.Format( "Files\\TGCStore\\TEMP\\ObjectList%d.xml", m_iGroup );
				bResult = m_Server.GetGroupObjects( sFilename, m_iGroup );
			}
			else if ( m_iGroup == -4 )
			{
				sFilename = "Files\\TGCStore\\TEMP\\FreeList.xml";
				bResult = m_Server.GetFreeObjects( sFilename );
			}
			else MessageBox( NULL, "Unrecognised group number", theApp.m_szErrorString, 0 );

			if ( !bResult ) 
			{
				m_bDownloading = false;
				return 1;
			}

			if ( m_pWindow && !m_bTerminate )
			{
				m_pWindow->ObjectsUpdated( );
			}
			
			m_bDownloading = false;
		} break;
		case 1:
		{
			m_bDownloading = true;

			char szSESID [ 34 ];
			if ( theApp.m_szSESID ) strcpy_s( szSESID, 34, theApp.m_szSESID );
			else strcpy_s( szSESID, 34, "" );

			bool bGotDetails = m_Server.GetModelDetails( m_dwObjID, szSESID );
			if ( !bGotDetails )
			{
				m_iDownloadType = -1;
				m_bDownloading = false;
				return 0;
				//MessageBox( NULL, m_Server.GetLastError(), "Server Error", 0 );
			}

			CString sName;
			int iType = -1;	//0=entity, 1=segment, 2=add-on

			cLocalObject.CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );
			
			sName = cLocalObject.GetName();
			m_iGroup = cLocalObject.GetGroupID( );

			if ( strcmp( cLocalObject.GetType(), "Entity" ) == 0 ) iType = 0;
			else if ( strcmp( cLocalObject.GetType(), "Segment" ) == 0 ) iType = 1;
			else if ( strcmp( cLocalObject.GetType(), "Add-on" ) == 0 ) iType = 2;
			else if ( strcmp( cLocalObject.GetType(), "Music" ) == 0 ) iType = 3;

			if ( iType < 0 ) 
			{
				MessageBox( NULL, GetLanguageData( "TGC Store", "UnrecognisedType" ), theApp.m_szErrorString, 0 );
				m_iDownloadType = -1;
				m_bDownloading = false;
				return 0;
			}

			if ( iType == 2 && !g_bFreeVersion )
			{
				if ( cLocalObject.IsPurchased() )
				{
					MessageBox( NULL, GetLanguageData( "TGC Store", "UpgradePurchased" ), "Info", 0 );
				}
				else
				{
					MessageBox( NULL, GetLanguageData( "TGC Store", "AlreadyUpgraded" ), "Info", 0 );
				}
				m_iDownloadType = -1;
				m_bDownloading = false;
				return 0;
			}

			bool bResult = m_Server.GetModel( m_dwObjID, m_dwProjectedCost, m_szModelFilename );
			if ( !bResult ) 
			{
				if ( m_bTerminate ) 
				{
					m_iDownloadType = -1;
					m_bDownloading = false;
					return 0;
				}
				else
				{
					MessageBox( NULL, m_Server.GetLastError( ), "Download Failed", 0 );
					m_iDownloadType = -1;
					m_bDownloading = false;
					if ( m_pWindow ) m_pWindow->DownloadComplete( m_iGroup, m_dwObjID, false );
					return 1;
				}
			}

			if ( iType == 2 )
			{
				char szSerial [ 40 ];
				bool bResult = m_Server.GetX9Serial( m_dwObjID, theApp.m_szSESID, szSerial, 40 );
				if ( !bResult ) 
				{
					m_iDownloadType = -1;
					m_bDownloading = false;
					if ( !m_bTerminate ) MessageBox( NULL, m_Server.GetLastError( ), "Serial Download Failed", 0 );
					return 0;
				}

				char pUserFile [ 512 ];
				wsprintf ( pUserFile, "%s\\%s", theApp.m_szDirectory, "userdetails.ini" );
				char pUserSerialCode [ 512 ];
				wsprintf ( pUserSerialCode, "" );
				BOOL bSuccess = WritePrivateProfileString( _T("PERSONAL DATA"), _T("YourSerialCode"), szSerial, pUserFile );
				if ( !bSuccess )
				{
					char str [ 256 ];
					sprintf_s( str, 256, "%s: %s", GetLanguageData( "TGC Store", "WriteSerialFailed" ), szSerial );
					MessageBox( NULL, str, theApp.m_szDirectory, 0 );
				}
				
				MarketObject *pObject = MarketObject::GetObjectByID( m_dwObjID, 0 );
				if ( pObject ) pObject->SetValue( "purchased", "1" );

				pObject = MarketObject::GetCompleteListObject( m_dwObjID );
				if ( pObject ) pObject->SetValue( "purchased", "1" );

				EnterCriticalSection( &theApp.m_csDirectoryChanges );

				CString sLicensePath = "Files/TGCStore/licenses/";
				SetCurrentDirectory( theApp.m_szDirectory );

				ZIPENTRY details;
				HZIP zpModel = OpenZip( m_szModelFilename, 0 );

				GetZipItem( zpModel, -1, &details );
				for ( int i = 0; i < details.index; i++ )
				{
					ZIPENTRY ze;
					GetZipItem( zpModel, i, &ze );

					if ( strcmp( ze.name, "Build Game Upgrade License.html" ) != 0 ) continue;
					
					UnzipItem( zpModel, i, sLicensePath + ze.name );
				}

				CloseZip( zpModel );
				DeleteFile( m_szModelFilename );

				// V111 - 110608 - use serial code to remove free version flag
				theApp.CheckSerialCodeValidity();

				LeaveCriticalSection( &theApp.m_csDirectoryChanges );

				if ( m_pWindow && !m_bTerminate ) m_pWindow->DownloadComplete( m_iGroup, m_dwObjID );

				m_iDownloadType = -1;
				m_bDownloading = false;
				return 0;
			}
			
			EnterCriticalSection( &theApp.m_csDirectoryChanges );
			
			try
			{
				if ( !SetCurrentDirectory( theApp.m_szDirectory ) ) throw 1;
				if ( !SetCurrentDirectory( "Files\\TGCStore" ) ) throw 2;
					
				if ( !SetCurrentDirectory( "licenses" ) )
				{
					CreateDirectory( "licenses", NULL );
					if ( !SetCurrentDirectory( "licenses" ) ) throw 3;
				}

				SetCurrentDirectory( theApp.m_szDirectory );

				LeaveCriticalSection( &theApp.m_csDirectoryChanges );

				CString sLicenseFile = "Files\\TGCStore\\licenses\\";
				sLicenseFile += sName;
				sLicenseFile += ".html";

				bool bResult = m_Server.GetLicense( m_dwObjID, sLicenseFile, theApp.m_szSESID );
				if ( !bResult ) 
				{
					if ( m_bTerminate ) 
					{
						m_iDownloadType = -1;
						m_bDownloading = false;
						return 0;
					}
					MessageBox( NULL, m_Server.GetLastError( ), "License Download Failed", 0 );
				}
			}
			catch( int )
			{
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			}

			EnterCriticalSection( &theApp.m_csDirectoryChanges );

			char dir [ MAX_PATH ];
			GetCurrentDirectory( MAX_PATH, dir );
			SetCurrentDirectory( theApp.m_szDirectory );
			SetCurrentDirectory( "Files\\entitybank" );

			if ( !SetCurrentDirectory( "purchased" ) )
			{
				CreateDirectory( "purchased", NULL );
			}

			SetCurrentDirectory( theApp.m_szDirectory );
			SetCurrentDirectory( "Files\\segments" );

			if ( !SetCurrentDirectory( "purchased" ) )
			{
				CreateDirectory( "purchased", NULL );
			}

			SetCurrentDirectory( theApp.m_szDirectory );

			char szGroupPath [ 256 ];
			if ( !MarketObject::ConstructGroupPath( m_iGroup, szGroupPath, 256 ) )
			{
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );
				MessageBox( NULL, "Failed to get group path", theApp.m_szErrorString, 0 );
				m_iDownloadType = -1;
				m_bDownloading = false;
				if ( m_pWindow && !m_bTerminate ) m_pWindow->DownloadComplete( m_iGroup, m_dwObjID, false );
				return 1;
			}

			SetCurrentDirectory( theApp.m_szDirectory );

			CString sBasePath = "Files/";
			CString sEntityBank = "Files/entitybank/purchased/";
			sEntityBank += szGroupPath;
			sEntityBank += "/";

			CString sSegmentBank = "Files/segments/purchased/";
			sSegmentBank += szGroupPath;
			sSegmentBank += "/";

			//delete bin and dbo
			BOOL bSucceed = FALSE;
			if ( strcmp( cLocalObject.GetType(), "Segment" ) == 0 ) bSucceed = SetCurrentDirectory( sSegmentBank );
			else bSucceed = SetCurrentDirectory( sEntityBank );
			if ( bSucceed )
			{
				CString sBinFile = sName; sBinFile += ".bin";
				CString sDBOFile = sName; sDBOFile += ".dbo";

				DeleteFile( sBinFile );
				DeleteFile( sDBOFile );
			}

			SetCurrentDirectory( theApp.m_szDirectory );

			ZIPENTRY details;
			HZIP zpModel = OpenZip( m_szModelFilename, 0 );

			GetZipItem( zpModel, -1, &details );
			for ( int i = 0; i < details.index; i++ )
			{
				ZIPENTRY ze;
				GetZipItem( zpModel, i, &ze );

				const char *szFile = strrchr( ze.name, '/' );	//must contain a slash
				if ( !szFile ) szFile = strrchr( ze.name, '\\' );
				if ( !szFile ) continue;

				if ( strncmp( ze.name, "audiobank", strlen("audiobank") ) != 0
				  && strncmp( ze.name, "effectbank", strlen("effectbank") ) != 0
				  && strncmp( ze.name, "entitybank", strlen("entitybank") ) != 0
				  && strncmp( ze.name, "gamecore", strlen("gamecore") ) != 0
				  && strncmp( ze.name, "levelbank", strlen("levelbank") ) != 0
				  && strncmp( ze.name, "mapbank", strlen("mapbank") ) != 0 
				  && strncmp( ze.name, "meshbank", strlen("meshbank") ) != 0
				  && strncmp( ze.name, "scriptbank", strlen("scriptbank") ) != 0
				  && strncmp( ze.name, "segments", strlen("segments") ) != 0
				  && strncmp( ze.name, "skybank", strlen("skybank") ) != 0
				  && strncmp( ze.name, "texturebank", strlen("texturebank") ) != 0
				  && strncmp( ze.name, "videobank", strlen("videobank") ) != 0 )
				{
					MessageBox( NULL, "Detected an error in the model file directory structure, please inform support of the error in this item", "Warning", 0 );
					continue;
				}

				if ( (iType == 0 || iType == 3) && (strncmp( ze.name, "entitybank\\", strlen("entitybank\\") ) == 0 || strncmp( ze.name, "entitybank/", strlen("entitybank/") ) == 0) )
				{
					szFile++;
					
					//MessageBox( NULL, ze.name, "Zip name", 0 );
					//MessageBox( NULL, sEntityBank + szFile, "Full name", 0 );

					UnzipItem( zpModel, i, sEntityBank + szFile );
				}
				else if ( iType == 1 && (strncmp( ze.name, "segments\\", strlen("segments\\") ) == 0 || strncmp( ze.name, "segments/", strlen("segments/") ) == 0) )
				{
					szFile++;
					UnzipItem( zpModel, i, sSegmentBank + szFile );
				}
				else UnzipItem( zpModel, i, sBasePath + ze.name );
			}

			CloseZip( zpModel );
			DeleteFile( m_szModelFilename );

			if ( cLocalObject.GetNumKeywords() > 0 )
			{
				SetCurrentDirectory( theApp.m_szDirectory );
				bSucceed = FALSE;
				if ( strcmp( cLocalObject.GetType(), "Segment" ) == 0 ) bSucceed = SetCurrentDirectory( sSegmentBank );
				else bSucceed = SetCurrentDirectory( sEntityBank );

				if ( !bSucceed )
				{
					//char str [ 256 ];
					//if ( strcmp( cLocalObject.GetType(), "Segment" ) == 0 ) sprintf_s( str, 256, "Error opening directory: %s", sSegmentBank );
					//else sprintf_s( str, 256, "Error opening directory: %s", sEntityBank );
					//MessageBox( NULL, str, "Error", 0 );
				}
				else
				{
					CString sKeywordFilename = sName;
					sKeywordFilename += ".fpk";
					FILE *pKeywordFile;
					int error = fopen_s( &pKeywordFile, sKeywordFilename, "wb" );
					if ( error )
					{
						char str [ 64 ];
						sprintf_s( str, 64, "Error writing keyword file: %d", error );
						MessageBox( NULL, str, "Error", 0 );
						pKeywordFile = 0;
					}
					else
					{
						for ( DWORD i = 0; i < cLocalObject.GetNumKeywords(); i++ )
						{
							fputs( cLocalObject.GetKeyword( i ), pKeywordFile );
							fputs( "\r\n", pKeywordFile );
						}
						
						fclose( pKeywordFile );
						pKeywordFile = 0;
					}
				}
			}

			SetCurrentDirectory( dir );

			LeaveCriticalSection( &theApp.m_csDirectoryChanges );

			MarketObject *pObject = MarketObject::GetObjectByID( m_dwObjID, 0 );
			if ( pObject ) pObject->SetValue( "purchased", "1" );

			pObject = MarketObject::GetCompleteListObject( m_dwObjID );
			if ( pObject ) pObject->SetValue( "purchased", "1" );
			
			if ( m_pWindow && !m_bTerminate ) m_pWindow->DownloadComplete( m_iGroup, m_dwObjID );

			m_iDownloadType = -1;
			m_bDownloading = false;
		} break;
		case 2 :
		{
			m_bDownloading = true;
			bool bResult = m_Server.GetImages( "Files\\TGCStore\\TEMP\\Thumbnails\\", m_iGroupID, m_sThumbnailsNeeded );
			if ( !bResult )
			{
				m_bDownloading = false;
				return 1;
			}

			m_sThumbnailsNeeded = "";

			if ( m_pWindow && !m_bTerminate )
			{
				m_pWindow->ThumbnailsUpdated( );
			}
			/*
			if ( m_pList )
			{
				m_pList->DeleteAllItems( );
				m_pList->InsertItem( 0, "<Select a Category>", -1 );
			}
			if ( m_pTree ) 
			{
				m_pTree->UpdateStoreCategories( );
			}
			*/

			m_bDownloading = false;
		} break;

		case 3:
		{
			//group list download
			m_bDownloading = true;
			
			m_bDownloading = true;
			bool bResult = m_Server.GetGroupList( "Files\\TGCStore\\TEMP\\GroupList.xml" );
			if ( bResult && !m_bTerminate )
			{
				if ( m_pWindow ) m_pWindow->ListUpdated( );
				//bool bSucceed = MarketObject::LoadGroups( sFilename );
			}

			m_bDownloading = false;
		} break;

		case 4:
		{
			//search list download
			m_bDownloading = true;
			bool bResult = m_Server.GetSearchList( "Files\\TGCStore\\TEMP\\SearchList.xml" );
			if ( !bResult ) 
			{
				m_bDownloading = false;
				return 1;
			}

			bResult = m_Server.GetKeywordList( "Files\\TGCStore\\TEMP\\KeywordList.xml" );
			if ( !bResult )
			{
				m_bDownloading = false;
				return 1;
			}

			if ( m_pWindow && !m_bTerminate ) m_pWindow->SearchListUpdated( );

			m_bDownloading = false;
		} break;

		case 5:
		{
			//purchased list download
			m_bDownloading = true;

			if ( theApp.m_szSESID )
			{
				bool bResult = m_Server.GetPurchasedList( "Files\\TGCStore\\TEMP\\PurchasedObjects.xml" );
				if ( !bResult ) 
				{
					m_bDownloading = false;
					return 1;
				}

				if ( m_pWindow && !m_bTerminate ) m_pWindow->PurchasedListUpdated( );
			}

			m_bDownloading = false;			
		} break;

		case 6:
		{
			//login
			m_bDownloading = true;

			bool bResult = m_Server.Login( m_sUser, m_sPass );
			if ( m_pWindow ) m_pWindow->LoggedIn( bResult, m_bSilentLogin );
			
			m_bDownloading = false;			
		} break;

		case 7:
		{
			//music sample
			m_bDownloading = true;

			m_bSuccess = false;
			m_bSuccess = m_Server.GetPreviewImage( sString1, sString2 );
			
			m_bDownloading = false;			
		} break;

	}

	m_bDownloading = false;
	return 0;
}

#endif