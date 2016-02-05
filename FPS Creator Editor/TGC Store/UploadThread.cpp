#include "stdafx.h"

#include "UploadThread.h"
#include "..\Editor.h"
#include "UploadAppDlg.h"
#include <atlimage.h>

UploadThread::UploadThread( )
{
	m_pProgressBar = NULL;

	m_sModelFilename = "";
	m_sThumbnail = "";
	m_iNumImages = 0;
	m_psPreviewImages = NULL;

	m_sName = "";
	m_sCost = "";
	m_sType = "";
	m_sCategory = "";
	m_sSubCategory = "";
	m_sAuthor = "";
	m_sPolygons = "";
	m_sKeywords = "";
	m_sDescription = "";

	m_dwObjID = 0;
	m_bUploading = false;
	m_bUploadSuccessful = false;

	m_iX9Compatible = 0;
	m_iX10Compatible = 0;
}

UploadThread::~UploadThread( )
{
	m_Server.CancelTransfer( );
	Join();

	if ( m_psPreviewImages ) delete [] m_psPreviewImages;
	m_psPreviewImages = NULL;
}

void UploadThread::SetValues( CProgressCtrl *progress, CString name, CString cost, CString type, CString category, CString subcategory, CString polygons, CString keywords, CString description, CString video, DWORD groupid, int x9compatible, int x10compatible )
{
	m_pProgressBar = progress;

	m_sName = name;
	m_sCost = cost;
	m_sType = type;
	m_sCategory = category;
	m_sSubCategory = subcategory;
	m_sPolygons = polygons;
	m_sKeywords = keywords;
	m_sDescription = description;
	m_sVideo = video;

	m_dwGroupID = groupid;

	m_iX9Compatible = x9compatible;
	m_iX10Compatible = x10compatible;
}

void UploadThread::SetFilenames( CString model, CString thumbnail, int numImages, CString *pImages, CString musicsample )
{
	m_sModelFilename = model;
	m_sThumbnail = thumbnail;
	m_sMusicSample = musicsample;
	m_iNumImages = numImages;

	if ( m_psPreviewImages ) delete [] m_psPreviewImages;
	m_psPreviewImages = NULL;

	if ( m_iNumImages > 0 )
	{
		m_psPreviewImages = new CString [ m_iNumImages ];

		for ( int i = 0 ; i < m_iNumImages; i++ )
		{
			m_psPreviewImages [ i ] = pImages [ i ];
		}
	}
}

void UploadThread::Cancel()
{
	m_Server.CancelTransfer( );
}

unsigned UploadThread::Run( )
{
	m_bUploading = true;
	m_bUploadSuccessful = false;
	m_dwObjID = 0;

	m_Server.SetProgress( NULL, NULL );

	char szNumImages [ 11 ];
	char szGroupID [ 11 ];
	
	sprintf_s( szNumImages, 11, "%d", m_iNumImages );
	sprintf_s( szGroupID, 11, "%d", m_dwGroupID );

	DWORD dwNewID = m_Server.CreateObject( m_sName, m_sCost, m_sType, m_sCategory, m_sSubCategory, m_sPolygons, m_sKeywords, m_sDescription, m_sVideo, szNumImages, szGroupID, m_iX9Compatible, m_iX10Compatible );
	if ( !dwNewID ) 
	{
		m_bUploading = false;
		return 0;
	}

	m_dwObjID = dwNewID;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	if ( !SetCurrentDirectory( theApp.m_szDirectory ) )
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		MessageBox( NULL, GetLanguageData( "Uploader", "InvalidMainPath" ), theApp.m_szErrorString, 0 );
		m_bUploading = false;
		return 0;
	}
	if ( !SetCurrentDirectory( "Files\\TGCStore\\" ) )
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		MessageBox( NULL, GetLanguageData( "Uploader", "InvalidStorePath" ), theApp.m_szErrorString, 0 );
		m_bUploading = false;
		return 0;
	}

	if ( !SetCurrentDirectory( "Temp" ) ) 
	{
		CreateDirectory( "Temp", NULL );
		if ( !SetCurrentDirectory( "Temp" ) )
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			MessageBox( NULL, GetLanguageData( "Uploader", "InvalidUploadPath" ), theApp.m_szErrorString, 0 );
			m_bUploading = false;
			return 0;
		}
	}
	SetCurrentDirectory( theApp.m_szDirectory );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
	
	CString sNewFilename; 
	sNewFilename.Format( "%s_%06d.zip", m_sName, dwNewID );

	CString sNewFullPath = theApp.m_szDirectory;
	sNewFullPath += "\\Files\\TGCStore\\Temp\\";
	sNewFullPath += sNewFilename;
	CopyFile( m_sModelFilename, sNewFullPath, FALSE );

	m_Server.SetProgress( m_pProgressBar, NULL );

	bool bResult = m_Server.UploadModelFile( dwNewID, sNewFilename );
	if ( !bResult )
	{
		char str [ 256 ];
		sprintf_s( str, 256, "Failed to upload model file: %s", m_Server.GetLastError( ) );
		//MessageBox( NULL, str, "Error", 0 );
		m_bUploading = false;
		return 0;
	}

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	SetCurrentDirectory( theApp.m_szDirectory );

	CImage image;
	HRESULT hr = image.Load( m_sThumbnail );
	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( FAILED(hr) )
	{
		MessageBox( NULL, "Unable to open thumbnail image", "Error", 0 );
		m_bUploading = false;
		return 0;
	}

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	SetCurrentDirectory( theApp.m_szDirectory );
	SetCurrentDirectory( "Files\\TGCStore\\Temp" );
	sNewFilename.Format( "%s_%06d_thumb.jpg", m_sName, dwNewID );
	image.Save( sNewFilename );
	image.Destroy( );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	bResult = m_Server.UploadThumbnail( dwNewID, sNewFilename );
	if ( !bResult )
	{
		MessageBox( NULL, "Failed to upload thumbnail image", "Error", 0 );
		m_bUploading = false;
		return 0;
	}

	for ( int i = 0; i < m_iNumImages; i++ )
	{
		EnterCriticalSection( &theApp.m_csDirectoryChanges );
		SetCurrentDirectory( theApp.m_szDirectory );
		hr = image.Load( m_psPreviewImages [ i ] );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		if ( FAILED(hr) )
		{
			char str [ 256 ];
			sprintf_s( str, 256, "Failed to load preview image '%s'", m_psPreviewImages [ i ] );
			MessageBox( NULL, str, "Error", 0 );
			m_bUploading = false;
			return 0;
		}

		EnterCriticalSection( &theApp.m_csDirectoryChanges );
		
		SetCurrentDirectory( theApp.m_szDirectory );
		SetCurrentDirectory( "Files\\TGCStore\\Uploads" );
		sNewFilename.Format( "%s_%06d_%02d.jpg", m_sName, dwNewID, i+1 );
		
		const char *szExt = strrchr( m_psPreviewImages [ i ].GetString(), '.' );
		CString sExt = szExt;
		if ( sExt.CompareNoCase( ".gif" ) == 0 ) CopyFile( m_psPreviewImages [ i ], sNewFilename, FALSE );
		else image.Save( sNewFilename );
		
		image.Destroy( );

		LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		bResult = m_Server.UploadPreviewImage( dwNewID, sNewFilename );
		if ( !bResult )
		{
			char str [ 256 ];
			sprintf_s( str, 256, "Failed to upload preview image '%s'", m_psPreviewImages [ i ] );
			MessageBox( NULL, str, "Error", 0 );
			m_bUploading = false;
			return 0;
		}
	}

	if ( strcmp( m_sType, "Music" ) == 0 )
	{
		m_Server.UploadMusicSample( dwNewID, m_sMusicSample );
		if ( !bResult )
		{
			MessageBox( NULL, "Failed to upload music sample file", "Error", 0 );
			m_bUploading = false;
			return 0;
		}
	}

	m_bUploadSuccessful = true;
	m_bUploading = false;
	return 0;
}