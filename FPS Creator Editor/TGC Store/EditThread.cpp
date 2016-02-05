#include "stdafx.h"

#include "EditThread.h"
#include "..\Editor.h"
#include <atlimage.h>

EditThread::EditThread( )
{
	m_sModelFilename = "";
	m_sThumbnail = "";
	m_iNumImages = 0;
	m_psPreviewImages = NULL;
	m_sMusicSample = "";

	m_sName = "";
	m_dwObjID = 0;

	m_bUploading = false;
	m_bUploadSuccessful = false;

	m_pProgressBar = NULL;

	/*
	m_sCost = "";
	m_sType = "";
	m_sCategory = "";
	m_sSubCategory = "";
	m_sAuthor = "";
	m_sPolygons = "";
	m_sKeywords = "";
	m_sDescription = "";

	m_iX9Compatible = 0;
	m_iX10Compatible = 0;
	*/
}

EditThread::~EditThread( )
{
	if ( m_psPreviewImages ) delete [] m_psPreviewImages;
	m_psPreviewImages = NULL;
}

/*
void UploadThread::SetValues( CString name, CString cost, CString type, CString category, CString subcategory, CString polygons, CString keywords, CString description, DWORD groupid, int x9compatible, int x10compatible )
{
	m_sName = name;
	m_sCost = cost;
	m_sType = type;
	m_sCategory = category;
	m_sSubCategory = subcategory;
	m_sPolygons = polygons;
	m_sKeywords = keywords;
	m_sDescription = description;

	m_dwGroupID = groupid;

	m_iX9Compatible = x9compatible;
	m_iX10Compatible = x10compatible;
}
*/

void EditThread::SetFilenames( CProgressCtrl *pProgress, DWORD dwObjID, CString sName, CString model, CString thumbnail, int numImages, CString *pImages, CString musicsample )
{
	m_pProgressBar = pProgress;
	m_sName = sName;
	m_dwObjID = dwObjID;

	m_sModelFilename = model;
	m_sThumbnail = thumbnail;
	m_iNumImages = numImages;
	m_sMusicSample = musicsample;

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

unsigned EditThread::Run( )
{
	m_bUploading = true;
	m_bUploadSuccessful = false;

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
	SetCurrentDirectory( "Files\\TGCStore\\" );

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
	SetCurrentDirectory( theApp.m_szDirectory );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
	
	CString sNewFilename; 

	m_Server.SetProgress( m_pProgressBar, NULL );

	if ( m_sModelFilename.GetLength() > 0 )
	{
		sNewFilename.Format( "%s_%06d.zip", m_sName, m_dwObjID );

		CString sNewFullPath = theApp.m_szDirectory;
		sNewFullPath += "\\Files\\TGCStore\\Temp\\";
		sNewFullPath += sNewFilename;
		CopyFile( m_sModelFilename, sNewFullPath, FALSE );

		bool bResult = m_Server.UploadModelFile( m_dwObjID, sNewFilename, true );
		if ( !bResult )
		{
			char str [ 256 ];
			sprintf_s( str, 256, "Failed to upload model file: %s", m_Server.GetLastError( ) );
			MessageBox( NULL, str, "Error", 0 );
			m_bUploading = false;
			return 0;
		}
	}

	CImage image;

	if ( m_sThumbnail.GetLength() > 0 )
	{
		EnterCriticalSection( &theApp.m_csDirectoryChanges );
		SetCurrentDirectory( theApp.m_szDirectory );
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
		SetCurrentDirectory( "Files\\TGCStore\\Temp\\" );
		sNewFilename.Format( "Thumbnails\\%s_%06d_thumb.jpg", m_sName, m_dwObjID );
		image.Save( sNewFilename );
		image.Destroy( );

		LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		bool bResult = m_Server.UploadThumbnail( m_dwObjID, sNewFilename, true );
		if ( !bResult )
		{
			MessageBox( NULL, "Failed to upload thumbnail image", "Error", 0 );
			m_bUploading = false;
			return 0;
		}
	}

	if ( m_iNumImages > 0 )
	{
		bool bResult = m_Server.ClearPreviewImages( m_dwObjID );
		if ( !bResult )
		{
			MessageBox( NULL, "Failed to clear preview images", "Server Error", 0 );
		}
	}

	for ( int i = 0; i < m_iNumImages; i++ )
	{
		//EnterCriticalSection( &theApp.m_csDirectoryChanges );
		//SetCurrentDirectory( theApp.m_szDirectory );
		HRESULT hr = image.Load( m_psPreviewImages [ i ] );
		//LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		if ( FAILED(hr) )
		{
			char str [ 256 ];
			sprintf_s( str, 256, "Failed to load preview image '%s'", m_psPreviewImages [ i ] );
			MessageBox( NULL, str, "Error", 0 );
			m_bUploading = false;
			return 0;
		}

		//EnterCriticalSection( &theApp.m_csDirectoryChanges );
		
		CString sFile;
		sFile.Format( "%s_%06d_%02d.jpg", m_sName, m_dwObjID, i+1 );

		sNewFilename = theApp.m_szDirectory;
		sNewFilename += "\\Files\\TGCStore\\Uploads\\";
		sNewFilename += sFile;

		const char *szExt = strrchr( m_psPreviewImages [ i ].GetString(), '.' );
		CString sExt = szExt;
		if ( sExt.CompareNoCase( ".gif" ) == 0 || sExt.CompareNoCase( ".jpg" ) == 0 ) CopyFile( m_psPreviewImages [ i ], sNewFilename, FALSE );
		else image.Save( sNewFilename );

		image.Destroy( );

		//LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		bool bResult = m_Server.UploadPreviewImage( m_dwObjID, sFile, true );
		if ( !bResult )
		{
			char str [ 256 ];
			sprintf_s( str, 256, "Failed to upload preview image '%s'", m_psPreviewImages [ i ] );
			MessageBox( NULL, str, "Error", 0 );
			m_bUploading = false;
			return 0;
		}
	}

	if ( m_sMusicSample.GetLength() > 0 )
	{
		sNewFilename.Format( "%s_%06d.ogg", m_sName, m_dwObjID );

		CString sNewFullPath = theApp.m_szDirectory;
		sNewFullPath += "\\Files\\TGCStore\\Temp\\";
		sNewFullPath += sNewFilename;
		CopyFile( m_sMusicSample, sNewFullPath, FALSE );

		bool bResult = m_Server.UploadMusicSample( m_dwObjID, sNewFilename, true );
		if ( !bResult )
		{
			char str [ 256 ];
			sprintf_s( str, 256, "Failed to upload music sample file: %s", m_Server.GetLastError( ) );
			MessageBox( NULL, str, "Error", 0 );
			m_bUploading = false;
			return 0;
		}
	}

	m_bUploadSuccessful = true;
	m_bUploading = false;
	return 0;
}