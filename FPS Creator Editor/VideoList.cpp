// VideoList.cpp : implementation file
//

#include "stdafx.h"
#include "VideoList.h"
#include "resource.h"
#include "Editor.h"
#include "StringFunc.h"
#include <atlimage.h>

// CVideoList

IMPLEMENT_DYNAMIC(CVideoList, CListCtrl)

CVideoList::CVideoList()
{
	m_iItemClicked = -1;
}

CVideoList::~CVideoList()
{
}

void CVideoList::Init( )
{
	this->DeleteAllItems ( );

	m_Images.DeleteImageList ( );
	m_Images.Create     ( 64, 64, ILC_COLOR32 | ILC_MASK, 1, 16 );
	//m_Images.SetBkColor ( RGB ( 255, 255, 255 ) );

	this->SetImageList   ( &m_Images, LVSIL_NORMAL );
	this->SetItemCount   ( 64 );

	CBitmap bmp;
	bmp.LoadBitmap( IDB_SAMPLE_IMAGE );
	m_Images.Add( &bmp, (CBitmap*)NULL );

	//InsertItem ( 0, "Sample Item", 0 );
	LoadVideoFiles( );
}

void CVideoList::LoadVideoFiles( )
{
	TCHAR szCurrDir [ MAX_PATH ];
	GetCurrentDirectory ( MAX_PATH, szCurrDir );
	SetCurrentDirectory( theApp.m_szDirectory );
	
	SetCurrentDirectory( theApp.m_szLanguagePath );
	if ( !SetCurrentDirectory( _T("helpwizard\\videos") ) )
	{
		TCHAR szString [ 256 ];
		GetPrivateProfileString ( _T ( "Getting Started" ), _T ( "NoVideoFolder" ), _T ( "" ), szString, 256, theApp.m_szLanguage );
		MessageBox( szString );
		return;
	}
	
	CString	strPath, strPattern;
	CFile	ImgFile;	
		
	HANDLE					hFind = NULL;
	WIN32_FIND_DATA			FindFileData;
	CString					FileNames [ 256 ];
	int iIndex = 0;
	BOOL bRC = TRUE;

	//strPath = "Files\\languagebank\\english\\helpwizard\\videos\\";

	
	hFind = ::FindFirstFile( /*strPath + */_T("*.*"), &FindFileData );
	if ( hFind == INVALID_HANDLE_VALUE ) 
	{
		TCHAR szString [ 256 ];
		GetPrivateProfileString ( _T ( "Getting Started" ), _T ( "NoVideoFolder" ), _T ( "" ), szString, 256, theApp.m_szLanguage );
		MessageBox( szString );
		return;
	}

	/////////////////
	this->DeleteAllItems ( );

	//m_Images.DeleteImageList ( );
	//m_Images.Create     ( 64, 64, ILC_COLOR32 | ILC_MASK, 1, 16 );

	m_Images.DeleteImageList ( );
	m_Images.Create     ( 64, 64, ILC_COLOR32 | ILC_MASK, 1, 16 );
	//m_Images.SetBkColor ( RGB ( 255, 255, 255 ) );

	this->SetImageList   ( &m_Images, LVSIL_NORMAL );
	this->SetItemCount   ( 64 );

	CBitmap bmp;
	bmp.LoadBitmap( IDB_SAMPLE_IMAGE );
	m_Images.Add( &bmp, (CBitmap*)NULL );
	/////////////////

	int iItemCount = 0;
	int iImageCount = 1;

	do
	{
		TCHAR lwFilename [ 260 ];
		_tcscpy_s( lwFilename, 260, FindFileData.cFileName );
		_tcstolower( lwFilename );

		if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) 
				&& !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			TCHAR* szExt = _tcsrchr( lwFilename, _T('.') );
			if ( !szExt ) continue;

			if ( _tcscmp( szExt, _T(".wmv") ) == 0 )
			{
				TCHAR szFile [ 260 ];
				_tcsncpy_s( szFile, 260, lwFilename, (DWORD)( szExt - lwFilename ) );
				_tcscat_s( szFile, 260, _T(".bmp") );

				CImage image;
				if ( FAILED( image.Load( szFile ) ) )
				{
					//MessageBox( "Could not find video image" );
					InsertItem( iItemCount++, FindFileData.cFileName, 0 );
				}
				else
				{
					CWindowDC wndDC( this );
					CDC dc;
					dc.CreateCompatibleDC( &wndDC );

					CBitmap bmpCurrImage;
					bmpCurrImage.CreateBitmap( 64,64, 1, 32, NULL );
					CBitmap *oldBmp = dc.SelectObject( &bmpCurrImage );

					::SetStretchBltMode( image.GetDC(), HALFTONE );
					::SetBrushOrgEx( image.GetDC(), 0,0, NULL );
					image.StretchBlt( dc.GetSafeHdc( ), 0,0, 64,64 );
					dc.SelectObject( oldBmp );
					m_Images.Add( &bmpCurrImage, (CBitmap*)NULL );

					bmpCurrImage.DeleteObject();

					image.ReleaseDC( );
					image.ReleaseDC( );

					InsertItem( iItemCount++, FindFileData.cFileName, iImageCount++ );
				}
			}
		}
	} while ( ::FindNextFile(hFind, &FindFileData) );

	::FindClose ( hFind );

	if ( iItemCount == 0 )
	{
		InsertItem( 0, GetLanguageData( _T ( "Getting Started" ), _T ( "NoVideo" ) ), -1 );
	}

	SetCurrentDirectory( szCurrDir );
}

bool CVideoList::GetFilenameSelected( int iBufferSize, TCHAR *szFilename )
{
	if ( m_iItemClicked < 0 )
	{
		TCHAR szString [ 256 ];
		GetPrivateProfileString ( _T ( "Getting Started" ), _T ( "NoSelect" ), _T ( "" ), szString, 256, theApp.m_szLanguage );
		MessageBox( szString, theApp.m_szErrorString );
		return false;
	}

	TCHAR szNoVideo [ 256 ];
	GetPrivateProfileString ( _T ( "Getting Started" ), _T ( "NoVideo" ), _T ( "" ), szNoVideo, 256, theApp.m_szLanguage );

	CString sName = GetItemText( m_iItemClicked, 0 );
	if ( sName.Compare( szNoVideo ) == 0 )
	{
		TCHAR szInvalidVideo [ 256 ];
		GetPrivateProfileString ( _T ( "Getting Started" ), _T ( "InvalidVideo" ), _T ( "" ), szInvalidVideo, 256, theApp.m_szLanguage );
		MessageBox( szInvalidVideo, theApp.m_szErrorString );
		return false;
	}

	_tcscpy_s( szFilename, iBufferSize, sName );

	return true;
}

BEGIN_MESSAGE_MAP(CVideoList, CListCtrl)
END_MESSAGE_MAP()



// CVideoList message handlers

void CVideoList::OnClickList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	// retrieve message info.
	LPNMITEMACTIVATE pItemAct = ( LPNMITEMACTIVATE ) pNMHDR;

	// determine which item receives the click
	LVHITTESTINFO  hitTest;

	// determine item clicked on
	memset ( &hitTest, '\0', sizeof ( LVHITTESTINFO ) );
	hitTest.pt = pItemAct->ptAction;
	this->SendMessage ( LVM_SUBITEMHITTEST, 0, ( LPARAM ) &hitTest );

	m_iItemClicked = hitTest.iItem;
}
