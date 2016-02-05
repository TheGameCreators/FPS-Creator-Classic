// ObjectDetailsDialog.cpp : implementation file
//

#include "stdafx.h"

#ifdef TGC_STORE

//#include "resource.h"
#include "..\resource.h"
#include "ObjectDetailsDialog.h"
#include "HTTPComm.h"
#include "..\Editor.h"
#include "MarketObject.h"
#include <atlimage.h>
//#include <vfw.h>
//#include <ogg.h>


// CObjectDetailsDialog dialog

IMPLEMENT_DYNAMIC(CObjectDetailsDialog, CDialog)

CObjectDetailsDialog::CObjectDetailsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectDetailsDialog::IDD, pParent)
{
	m_pEmptyStar = NULL;
	m_pFilledStar = NULL;
	m_pMaskStar = NULL;
	m_bBuy = false;
	m_bAddToCart = false;
	m_bCartStateChanged = false;
	
	m_pMarket = NULL;
	m_pCurrObject = NULL;
	m_iItemSelected = 0;
}

CObjectDetailsDialog::~CObjectDetailsDialog()
{
	op.Close();
}

void CObjectDetailsDialog::SetBitmaps( CBitmap *pEmptyStar, CBitmap *pFilledStar, CBitmap *pMaskStar )
{
	m_pEmptyStar = pEmptyStar;
	m_pFilledStar = pFilledStar;
	m_pMaskStar = pMaskStar;
}

void CObjectDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_COUNTER, m_txtImageCounter);
	DDX_Control(pDX, IDC_IMAGES, m_ImageBox);
	DDX_Control(pDX, IDC_GIF_BOX, m_GifBox);
	DDX_Control(pDX, IDC_NAME, m_txtName);
	DDX_Control(pDX, IDC_COST, m_txtCost);
	DDX_Control(pDX, IDC_CATEGORY, m_txtCategory);
	DDX_Control(pDX, IDC_AUTHOR, m_txtAuthor);
	DDX_Control(pDX, IDC_RATING, m_txtRating);
	DDX_Control(pDX, IDC_THUMBNAIL, m_Thumbnail);
	DDX_Control(pDX, IDOK, m_btnBuy);
	DDX_Control(pDX, IDC_TEXT_REDOWNLOAD, m_txtDownload);
	DDX_Control(pDX, IDC_TEXT_NAME, m_labelName);
	DDX_Control(pDX, IDC_TEXT_CATEGORY, m_LabelCategory);
	DDX_Control(pDX, IDC_TEXT_COST, m_labelCost);
	DDX_Control(pDX, IDC_TEXT_AUTHOR, m_LabelAuthor);
	DDX_Control(pDX, IDC_TEXT_RATING, m_LabelRating);
	DDX_Control(pDX, IDC_TEXT_DESCRIPTION, m_LabelDescription);
	DDX_Control(pDX, IDPREVOBJECT, m_btnPrevObject);
	DDX_Control(pDX, IDNEXTOBJECT, m_btnNextObject);
	DDX_Control(pDX, IDC_BUTTON3, m_btnRate);
	DDX_Control(pDX, IDC_COMBO2, m_comboRating);
	DDX_Control(pDX, IDC_TEXT_RATE, m_txtYourRate);
	DDX_Control(pDX, IDC_TEXT_POLYGONS, m_labelPolygons);
	DDX_Control(pDX, IDC_POLYGONS, m_txtPolygons);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDADDTOCART, m_btnAddToCart);
	DDX_Control(pDX, IDC_TYPE_NAME, m_labelType);
	DDX_Control(pDX, IDC_TYPE, m_txtType);
	DDX_Control(pDX, IDC_TEXT_FILESIZE, m_labelFileSize);
	DDX_Control(pDX, IDC_FILESIZE, m_txtFileSize);
	DDX_Control(pDX, IDC_TEXT_DOWNLOADS, m_labelDownloads);
	DDX_Control(pDX, IDC_DOWNLOADS, m_txtDownloads);
	DDX_Control(pDX, IDC_VIDEO_LINK, m_linkVideo);
	DDX_Control(pDX, IDC_MEDIA_PROGRESS, m_MediaProgressBar);
	DDX_Control(pDX, IDC_BUTTON4, m_btnPlaySample);
	DDX_Control(pDX, IDC_IMAGE_RIGHT, m_btnNextImage);
	DDX_Control(pDX, IDC_IMAGE_LEFT, m_btnPrevImage);
	DDX_Control(pDX, IDC_MEDIA_TEXT, m_labelMedia);
}

void CObjectDetailsDialog::SetValues( MarketObject *pObject, cMarketImage *pMarket, CMarketWindow *pWindow, int iGroup, bool bFromHomePage )
									 /*const char* name, const char* category, const char* cost, const char* description, const char* author, const char* rating, char **pszImages, DWORD dwNumImages )*/
{
	/*
	m_name = name;
	m_category = category;
	m_cost = cost;
	m_description = description;
	m_author = author;
	m_rating = rating;

	m_pszImages = pszImages;
	m_dwNumImages = dwNumImages;
	*/
	m_pCurrObject = pObject;
	m_bFromHomePage = bFromHomePage;
	m_pWindow = pWindow;
	if ( bFromHomePage )
	{
		m_iGroup = 0;
		m_pMarket = NULL;
		m_iItemSelected = 0;
	}
	else
	{
		m_iGroup = iGroup;
		m_pMarket = pMarket;
		m_iItemSelected = m_pMarket->GetItemClicked( );
	}
}

BOOL CObjectDetailsDialog::OnInitDialog()
{
	m_bBuy = false;

	if ( !CDialog::OnInitDialog( ) ) return FALSE;

	if ( !m_pCurrObject ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return TRUE;
	}

	/*
	//if ( !m_pCurrObject->IsComplete( ) )
	{
		HTTPComm server;
		if ( !server.GetModelDetails( m_pCurrObject->GetID(), theApp.m_szSESID ) )
		{
			MessageBox( server.GetLastError(), "Server Error" );
		}
		m_pCurrObject->CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );
	}
	*/

	m_fontMain.DeleteObject( );
	if ( !m_fontMain.CreatePointFont( 80, "Calibri" ) ) m_fontMain.CreatePointFont( 80, "Arial" );

	m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Buy") ) );
	m_btnClose.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Close") ) );
	m_btnPrevObject.SetWindowText( GetLanguageData( _T("TGC Store"), _T("PrevObject") ) );
	m_btnNextObject.SetWindowText( GetLanguageData( _T("TGC Store"), _T("NextObject") ) );

	m_labelName.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Name") ) );
	m_labelType.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Type") ) );
	m_labelCost.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Cost") ) );
	m_LabelRating.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Rating") ) );
	m_LabelCategory.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Category") ) );
	m_LabelAuthor.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Author") ) );
	m_labelPolygons.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Polygons") ) );
	m_labelFileSize.SetWindowText( GetLanguageData( _T("TGC Store"), _T("FileSize") ) );
	m_labelDownloads.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloads") ) );
	m_linkVideo.SetWindowText( GetLanguageData( _T("TGC Store"), _T("VideoLink") ) );
	m_linkVideo.SetType( 1 );

	m_LabelDescription.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Description") ) );

	m_btnRate.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Rate") ) );
	m_txtYourRate.SetWindowText( GetLanguageData( _T("TGC Store"), _T("YourRating") ) );

	CString sAlreadyOwn; 
	sAlreadyOwn.Format( GetLanguageData( _T("TGC Store"), _T("AlreadyOwn1") ), m_pCurrObject->GetUserDownloads() );
	sAlreadyOwn += "\r\n";
	sAlreadyOwn += GetLanguageData( _T("TGC Store"), _T("AlreadyOwn2") );
	m_txtDownload.SetWindowText( sAlreadyOwn );

	if ( m_bFromHomePage )
	{
		m_btnPrevObject.EnableWindow( FALSE );
		m_btnNextObject.EnableWindow( FALSE );
	}
	else
	{
		m_btnPrevObject.EnableWindow( TRUE );
		m_btnNextObject.EnableWindow( TRUE );
	}

	//PopulateValues( );
	SetTimer( 1,500,NULL );

	m_GifBox.ShowWindow( SW_HIDE );

	op.InitDirectSound( this->GetSafeHwnd() );
	
	return TRUE;
}

void CObjectDetailsDialog::PopulateValues( )
{
	KillTimer( 2 );
	KillTimer( 3 );
	if ( m_ServerThread.IsDownloading() ) m_ServerThread.Cancel();
	op.Close();

	if ( !m_pCurrObject ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return;
	}

	HTTPComm server;
	if ( !server.GetModelDetails( m_pCurrObject->GetID(), theApp.m_szSESID ) )
	{
		//MessageBox( server.GetLastError(), "Server Error" );
		return;
	}
	m_pCurrObject->CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );

	if ( /*m_pCurrObject->IsPurchased() ||*/ MarketObject::IsObjectInCart( m_pCurrObject->GetID() ) ) 
	{
		//m_btnAddToCart.EnableWindow( FALSE );
		m_btnAddToCart.SetWindowTextA( GetLanguageData( _T("TGC Store"), _T("RemoveFromCart") ) );
		m_bAddToCart = true;
	}
	else
	{
		m_btnAddToCart.SetWindowTextA( GetLanguageData( _T("TGC Store"), _T("AddToCart") ) );
		m_bAddToCart = false;
	}

	m_editDescription.SetRedraw( 0 );
	m_dwCurrImage = m_pCurrObject->GetNumImages() > 0 ? 1 : 0;

	m_Thumbnail.SetWindowPos( NULL, 0,0, 64,64, SWP_NOMOVE );

	CString sTitle = GetLanguageData( "TGC STore", "PreviewItem" );
	sTitle += " - ";
	sTitle += m_pCurrObject->GetName( );
	SetWindowText( sTitle );

	CString sCost; 
	sCost.Format( "%d", m_pCurrObject->GetCost() );
	CString sRating; 
	sRating.Format( "%.2f", m_pCurrObject->GetTotalRating() / (float) m_pCurrObject->GetRatingCount() );
	CString sPolygons; 
	sPolygons.Format( "%d", m_pCurrObject->GetPolygons() );
	CString sFileSize; 
	sFileSize.Format( "%.2f MB", m_pCurrObject->GetFileSize() / 1048576.0f );
	CString sDownloads; 
	sDownloads.Format( "%d", m_pCurrObject->GetDownloads() );

	m_txtName.SetWindowText( m_pCurrObject->GetName() );
	m_txtType.SetWindowText( m_pCurrObject->GetType() );
	m_txtCategory.SetWindowText( m_pCurrObject->GetCategory() );
	m_txtCost.SetWindowText( sCost );
	m_editDescription.SetWindowText( m_pCurrObject->GetDescription() );
	m_txtAuthor.SetWindowText( m_pCurrObject->GetAuthor() );
	m_txtRating.SetBitmaps( m_pEmptyStar, m_pFilledStar, m_pMaskStar );
	m_txtRating.SetRating( m_pCurrObject->GetTotalRating(), m_pCurrObject->GetRatingCount() );
	m_txtPolygons.SetWindowText( sPolygons );
	m_txtFileSize.SetWindowText( sFileSize );
	m_txtDownloads.SetWindowText( sDownloads );

	CString sAlreadyOwn; 
	sAlreadyOwn.Format( GetLanguageData( _T("TGC Store"), _T("AlreadyOwn1") ), m_pCurrObject->GetUserDownloads() );
	sAlreadyOwn += "\r\n";
	sAlreadyOwn += GetLanguageData( _T("TGC Store"), _T("AlreadyOwn2") );
	m_txtDownload.SetWindowText( sAlreadyOwn );

	CString sLink = m_pCurrObject->GetVideoLink();
	if ( sLink.GetLength() < 1 ) m_linkVideo.ShowWindow( SW_HIDE );
	else
	{
		m_linkVideo.SetLink( m_pCurrObject->GetVideoLink() );
		m_linkVideo.ShowWindow( SW_SHOW );
	}

	m_txtName.SetFont( &m_fontMain, 1 );
	m_txtType.SetFont( &m_fontMain, 1 );
	m_txtCategory.SetFont( &m_fontMain, 1 );
	m_txtCost.SetFont( &m_fontMain, 1 );
	m_editDescription.SetFont( &m_fontMain, 1 );
	m_txtAuthor.SetFont( &m_fontMain, 1 );
	m_txtPolygons.SetFont( &m_fontMain, 1 );
	m_txtFileSize.SetFont( &m_fontMain, 1 );
	m_txtDownloads.SetFont( &m_fontMain, 1 );

	m_txtDownload.SetFont( &m_fontMain, 1 );
	m_labelName.SetFont( &m_fontMain, 1 );
	m_labelType.SetFont( &m_fontMain, 1 );
	m_LabelCategory.SetFont( &m_fontMain, 1 );
	m_labelCost.SetFont( &m_fontMain, 1 );
	m_LabelAuthor.SetFont( &m_fontMain, 1 );
	m_LabelRating.SetFont( &m_fontMain, 1 );
	m_LabelDescription.SetFont( &m_fontMain, 1 );
	m_labelPolygons.SetFont( &m_fontMain, 1 );
	m_labelFileSize.SetFont( &m_fontMain, 1 );
	m_labelDownloads.SetFont( &m_fontMain, 1 );

	if ( strcmp( m_pCurrObject->GetType(), "Entity" ) != 0 )
	{
		m_labelPolygons.ShowWindow( SW_HIDE );
		m_txtPolygons.ShowWindow( SW_HIDE );
	}
	else
	{
		m_labelPolygons.ShowWindow( SW_SHOW );
		m_txtPolygons.ShowWindow( SW_SHOW );
	}

	m_btnRate.ShowWindow( SW_HIDE );
	m_comboRating.ShowWindow( SW_HIDE );
	m_txtYourRate.ShowWindow( SW_HIDE );

	if ( m_pCurrObject->IsPurchased( ) )
	{
		m_txtDownload.ShowWindow( SW_SHOW );
		m_btnBuy.SetWindowText( "Download" );

		if ( !m_pCurrObject->IsRated( ) )
		{
			m_btnRate.ShowWindow( SW_SHOW );
			m_comboRating.ShowWindow( SW_SHOW );
			m_txtYourRate.ShowWindow( SW_SHOW );
		}
	}
	else
	{
		m_txtDownload.ShowWindow( SW_HIDE );
		m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Buy") ) );
	}

	if ( !m_pCurrObject->IsAvailable( ) )
	{
		m_btnBuy.EnableWindow( FALSE );
	}
	else
	{
		m_btnBuy.EnableWindow( TRUE );
	}

	CString sImageCounter;
	sImageCounter.Format( "%d/%d", m_dwCurrImage, m_pCurrObject->GetNumImages() );
	m_txtImageCounter.SetWindowText( sImageCounter );

	//get thumbnail
	CString sPath = "Files\\TGCStore\\TEMP\\Thumbnails\\";
	sPath += m_pCurrObject->GetThumbnail( );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	SetCurrentDirectory( theApp.m_szDirectory );
	CImage image;
	HRESULT hr = image.Load( sPath );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( FAILED(hr) )
	{
		CString sThumbnail = "Images\\Thumbnails\\";
		sThumbnail += m_pCurrObject->GetThumbnail( );

		HTTPComm server;
		bool result = server.GetPreviewImage( sThumbnail, sPath );
		if ( !result ) 
		{
			MessageBox( server.GetLastError( ), "Server Error" );
			return;
		}
		
		EnterCriticalSection( &theApp.m_csDirectoryChanges );
		SetCurrentDirectory( theApp.m_szDirectory );
		hr = image.Load( sPath );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
	}

	CWindowDC wndDC( this );
	CDC dc;	dc.CreateCompatibleDC( &wndDC );

	CBitmap bmpThumb;
	bmpThumb.CreateBitmap( 64,64, 1, 32, NULL );
	CBitmap *oldBmp = dc.SelectObject( &bmpThumb );
	image.BitBlt( dc.GetSafeHdc( ), 0,0 );
	dc.SelectObject( oldBmp );
	m_Thumbnail.CopyBitmap( &bmpThumb );
	image.Destroy();

	m_GifBox.ShowWindow( SW_HIDE );
	m_GifBox.UnLoad();

	m_btnPrevImage.EnableWindow( FALSE );
	m_btnNextImage.EnableWindow( FALSE );
	m_txtImageCounter.EnableWindow( FALSE );

	m_MediaProgressBar.ShowWindow( SW_HIDE );
	m_btnPlaySample.ShowWindow( SW_HIDE );
	m_labelMedia.ShowWindow( SW_HIDE );
	m_ImageBox.Reset();

	if ( strcmp( m_pCurrObject->GetType(), "Music" ) == 0
	  || strcmp( m_pCurrObject->GetType(), "Sound" ) == 0 )
	{
		m_ImageBox.EnableWindow( FALSE );
		m_MediaProgressBar.ShowWindow( SW_SHOW );
		m_btnPlaySample.ShowWindow( SW_SHOW );
		m_btnPlaySample.EnableWindow( FALSE );
		m_labelMedia.ShowWindow( SW_SHOW );
		m_labelMedia.Invalidate();

		m_ImageBox.LoadBitmap( IDB_MUSIC_PREVIEW );

		CString sFilename;
		sFilename.Format( "%s_%06d.ogg", m_pCurrObject->GetName(), m_pCurrObject->GetID() );

		m_sMusicSampleFile = "Files\\TGCStore\\TEMP\\Samples\\";
		m_sMusicSampleFile += sFilename;

		//m_ServerThread.Join();

		SetTimer( 4, 500, NULL );

		m_labelMedia.SetText( GetLanguageData( "TGC Store", "Buffering" ) );
		Invalidate();

		//SetTimer( 2, 500, NULL );
	}
	else
	{
		m_sMusicSampleFile = "";
		m_ImageBox.ShowWindow( SW_SHOW );
		m_ImageBox.EnableWindow( TRUE );

		if ( m_dwCurrImage > 0 )
		{
			m_btnPrevImage.EnableWindow( FALSE );
			m_txtImageCounter.EnableWindow( TRUE );
			if ( m_pCurrObject->GetNumImages() > 1 ) m_btnNextImage.EnableWindow( TRUE );

			EnterCriticalSection( &theApp.m_csDirectoryChanges );

			SetCurrentDirectory( theApp.m_szDirectory );

			sPath = "Files\\TGCStore\\TEMP\\";
			sPath += m_pCurrObject->GetImage( m_dwCurrImage-1 );

			hr = image.Load( sPath );
				
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );

			if ( FAILED(hr) )
			{
				HTTPComm server;
				bool result = server.GetPreviewImage( m_pCurrObject->GetImage( m_dwCurrImage-1 ), sPath );
				if ( !result ) 
				{
					MessageBox( server.GetLastError( ), "Server Error" );
					return;
				}
				
				EnterCriticalSection( &theApp.m_csDirectoryChanges );
				SetCurrentDirectory( theApp.m_szDirectory );
				hr = image.Load( sPath );
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			}

			if ( FAILED(hr) )
			{
				m_ImageBox.Reset( );
			}
			else
			{
				CString sFullPath = theApp.m_szDirectory;
				sFullPath += "\\";
				sFullPath += sPath;

				BOOL bIsGif = m_GifBox.Load( sFullPath );

				if ( bIsGif )
				{
					m_ImageBox.Reset( );
					CRect rectImage;
					m_ImageBox.GetClientRect( rectImage );

					SIZE size = m_GifBox.GetSize();
					if ( size.cx > rectImage.Width() || size.cy > rectImage.Height() ) 
					{
						m_GifBox.SetPaintRect( rectImage );
						m_GifBox.SetWindowPos( &wndTop, rectImage.left, rectImage.top, rectImage.Width(), rectImage.Height(), 0 );
					}
					else
					{
						int x = rectImage.left + ( rectImage.Width() - size.cx ) / 2;
						int y = rectImage.top + ( rectImage.Height() - size.cy ) / 2;
						m_GifBox.SetWindowPos( NULL, x, y, size.cx, size.cy, 0 );
					}

					m_GifBox.ShowWindow( SW_SHOW );
					m_GifBox.Draw();
				}
				else
				{
					m_GifBox.UnLoad();

					CWindowDC wndDC( this );
					CDC dc;
					dc.CreateCompatibleDC( &wndDC );

					bmpCurrImage.DeleteObject();
					bmpCurrImage.CreateBitmap( image.GetWidth(),image.GetHeight(), 1, 32, NULL );
					CBitmap *oldBmp = dc.SelectObject( &bmpCurrImage );

					::SetStretchBltMode( image.GetDC(), HALFTONE );
					::SetBrushOrgEx( image.GetDC(), 0,0, NULL );
					image.BitBlt( dc.GetSafeHdc( ), 0,0 );
					dc.SelectObject( oldBmp );
					m_ImageBox.CopyBitmap( &bmpCurrImage );

					image.ReleaseDC();
					image.ReleaseDC();
				}
			}
		}
	}

	m_editDescription.SetRedraw( 1 );
	Invalidate( );
	m_txtRating.Invalidate( );
	m_editDescription.Invalidate( );
}


BEGIN_MESSAGE_MAP(CObjectDetailsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CObjectDetailsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_IMAGE_LEFT, &CObjectDetailsDialog::OnBnClickedImageLeft)
	ON_BN_CLICKED(IDC_IMAGE_RIGHT, &CObjectDetailsDialog::OnBnClickedImageRight)
	ON_BN_CLICKED(IDPREVOBJECT, &CObjectDetailsDialog::OnPrevObjectClicked)
	ON_BN_CLICKED(IDNEXTOBJECT, &CObjectDetailsDialog::OnNextObjectClicked)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjectDetailsDialog::OnYourRatingClicked)
	ON_BN_CLICKED(IDADDTOCART, &CObjectDetailsDialog::OnAddToCart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CObjectDetailsDialog::OnPlaySample)
END_MESSAGE_MAP()


// CObjectDetailsDialog message handlers

void CObjectDetailsDialog::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		KillTimer( 1 );
		PopulateValues();
	}

	if ( nIDEvent == 2 )
	{
		if ( !m_ServerThread.IsDownloading() )
		{
			KillTimer( 2 );

			if ( m_ServerThread.WasSuccessful() )
			{
				m_btnPlaySample.EnableWindow( TRUE );
				CString sFile = theApp.m_szDirectory;
				sFile += "\\";
				sFile += m_sMusicSampleFile;

				bool bOpened = op.OpenOgg( sFile );
				if ( !bOpened )
				{
					MessageBox( "Failed to open sample music file" );
				}
				else
				{
					m_MediaProgressBar.SetPlayRange( (long) op.GetLength() );
					m_labelMedia.SetText( GetLanguageData( "TGC Store", "ClickToPlay" ) );
					Invalidate();
				}
			}
			else
			{
				MessageBox( "Failed to download sample music file" );
			}
		}
	}

	if ( nIDEvent == 3 )
	{
		m_MediaProgressBar.SetPlayPos( (long) op.GetPosition() );

		if ( !op.IsPlaying() ) 
		{
			KillTimer( 3 );
			m_btnPlaySample.SetWindowText( ">" );
		}
	}

	if ( nIDEvent == 4 )
	{
		if ( !m_ServerThread.IsDownloading() )
		{
			KillTimer( 4 );
			m_btnPlaySample.SetWindowText( ">" );
			m_MediaProgressBar.SetPlayPos( 0 );
			m_MediaProgressBar.SetPos( 0 );

			CString sFilename;
			sFilename.Format( "%s_%06d.ogg", m_pCurrObject->GetName(), m_pCurrObject->GetID() );

			CString sServerFile = "/Samples/Music/";
			sServerFile += sFilename;

			m_ServerThread.SetProgress( &m_MediaProgressBar, NULL );
			m_ServerThread.SetMusicSampleDownload( sServerFile, m_sMusicSampleFile );
			m_ServerThread.Start();

			SetTimer( 2, 500, NULL );
		}
	}
}

void CObjectDetailsDialog::OnBnClickedOk()
{
	m_bBuy = true;
	OnOK();
}

void CObjectDetailsDialog::OnBnClickedImageLeft()
{
	if ( !m_pCurrObject ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return;
	}

	m_btnNextImage.EnableWindow( TRUE );

	if ( m_dwCurrImage <= 1 ) return;
	m_dwCurrImage--;
	if ( m_dwCurrImage <= 1 ) m_btnPrevImage.EnableWindow( FALSE );

	m_GifBox.ShowWindow( SW_HIDE );
	m_GifBox.UnLoad();
	m_ImageBox.Reset( );

	CString sImageCounter;
	sImageCounter.Format( "%d/%d", m_dwCurrImage, m_pCurrObject->GetNumImages() );
	m_txtImageCounter.SetWindowText( sImageCounter );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	//char szOldDir [ 260 ];
	//GetCurrentDirectory( 260, szOldDir );
	SetCurrentDirectory( theApp.m_szDirectory );

	CString sPath = "Files\\TGCStore\\TEMP\\";
	sPath += m_pCurrObject->GetImage( m_dwCurrImage-1 );

	CImage image;
	HRESULT hr = image.Load( sPath );
	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( FAILED(hr) )
	{
		HTTPComm server;
		bool result = server.GetPreviewImage( m_pCurrObject->GetImage( m_dwCurrImage-1 ), sPath );
		if ( !result ) MessageBox( server.GetLastError( ), "Server Error" );

		EnterCriticalSection( &theApp.m_csDirectoryChanges );
		SetCurrentDirectory( theApp.m_szDirectory );
		hr = image.Load( sPath );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		if ( FAILED(hr) )
		{
			m_ImageBox.Reset( );
			//SetCurrentDirectory( szOldDir );
			return;
		}
	}

	CString sFullPath = theApp.m_szDirectory;
	sFullPath += "\\";
	sFullPath += sPath;

	BOOL bIsGif = m_GifBox.Load( sFullPath );

	if ( bIsGif )
	{
		m_ImageBox.Reset( );
		CRect rectImage;
		m_ImageBox.GetClientRect( rectImage );

		SIZE size = m_GifBox.GetSize();
		if ( size.cx > rectImage.Width() || size.cy > rectImage.Height() ) 
		{
			m_GifBox.SetPaintRect( rectImage );
			m_GifBox.SetWindowPos( NULL, rectImage.left, rectImage.top, rectImage.Width(), rectImage.Height(), 0 );
		}
		else
		{
			int x = rectImage.left + ( rectImage.Width() - size.cx ) / 2;
			int y = rectImage.top + ( rectImage.Height() - size.cy ) / 2;
			m_GifBox.SetWindowPos( NULL, x, y, size.cx, size.cy, 0 );
		}

		m_GifBox.ShowWindow( SW_SHOW );
		m_GifBox.Draw();
	}
	else
	{
		m_GifBox.UnLoad();

		CWindowDC wndDC( this );
		CDC dc;
		dc.CreateCompatibleDC( &wndDC );

		bmpCurrImage.DeleteObject( );
		bmpCurrImage.CreateBitmap( image.GetWidth(),image.GetHeight(), 1, 32, NULL );
		CBitmap *oldBmp = dc.SelectObject( &bmpCurrImage );

		::SetStretchBltMode( image.GetDC(), HALFTONE );
		::SetBrushOrgEx( image.GetDC(), 0,0, NULL );
		image.BitBlt( dc.GetSafeHdc( ), 0,0 );
		dc.SelectObject( oldBmp );
		m_ImageBox.CopyBitmap( &bmpCurrImage );

		image.ReleaseDC();
		image.ReleaseDC();
	}
}

void CObjectDetailsDialog::OnBnClickedImageRight()
{
	if ( !m_pCurrObject ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return;
	}

	m_btnPrevImage.EnableWindow( TRUE );

	if ( m_dwCurrImage >= m_pCurrObject->GetNumImages() ) return;
	m_dwCurrImage++;
	if ( m_dwCurrImage >= m_pCurrObject->GetNumImages() ) m_btnNextImage.EnableWindow( FALSE );

	m_GifBox.ShowWindow( SW_HIDE );
	m_GifBox.UnLoad();
	m_ImageBox.Reset( );

	CString sImageCounter;
	sImageCounter.Format( "%d/%d", m_dwCurrImage, m_pCurrObject->GetNumImages() );
	m_txtImageCounter.SetWindowText( sImageCounter );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	//char szOldDir [ 260 ];
	//GetCurrentDirectory( 260, szOldDir );
	SetCurrentDirectory( theApp.m_szDirectory );

	CString sPath = "Files\\TGCStore\\TEMP\\";
	sPath += m_pCurrObject->GetImage( m_dwCurrImage-1 );

	CImage image;
	HRESULT hr = image.Load( sPath );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( FAILED(hr) )
	{
		HTTPComm server;
		bool result = server.GetPreviewImage( m_pCurrObject->GetImage( m_dwCurrImage-1 ), sPath );
		if ( !result ) MessageBox( server.GetLastError( ), "Server Error" );

		EnterCriticalSection( &theApp.m_csDirectoryChanges );
		SetCurrentDirectory( theApp.m_szDirectory );
		hr = image.Load( sPath );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		if ( FAILED(hr) )
		{
			m_ImageBox.Reset( );
			//SetCurrentDirectory( szOldDir );
			return;
		}
	}

	CString sFullPath = theApp.m_szDirectory;
	sFullPath += "\\";
	sFullPath += sPath;

	BOOL bIsGif = m_GifBox.Load( sFullPath );

	if ( bIsGif )
	{
		m_ImageBox.Reset( );
		CRect rectImage;
		m_ImageBox.GetClientRect( rectImage );

		SIZE size = m_GifBox.GetSize();
		if ( size.cx > rectImage.Width() || size.cy > rectImage.Height() ) 
		{
			m_GifBox.SetPaintRect( rectImage );
			m_GifBox.SetWindowPos( NULL, rectImage.left, rectImage.top, rectImage.Width(), rectImage.Height(), 0 );
		}
		else
		{
			int x = rectImage.left + ( rectImage.Width() - size.cx ) / 2;
			int y = rectImage.top + ( rectImage.Height() - size.cy ) / 2;
			m_GifBox.SetWindowPos( NULL, x, y, size.cx, size.cy, 0 );
		}

		m_GifBox.ShowWindow( SW_SHOW );
		m_GifBox.Draw();
	}
	else
	{
		m_GifBox.UnLoad();

		CWindowDC wndDC( this );
		CDC dc;
		dc.CreateCompatibleDC( &wndDC );

		bmpCurrImage.DeleteObject( );
		bmpCurrImage.CreateBitmap( image.GetWidth(),image.GetHeight(), 1, 32, NULL );
		CBitmap *oldBmp = dc.SelectObject( &bmpCurrImage );

		::SetStretchBltMode( image.GetDC(), HALFTONE );
		::SetBrushOrgEx( image.GetDC(), 0,0, NULL );
		image.BitBlt( dc.GetSafeHdc( ), 0,0 );
		dc.SelectObject( oldBmp );
		m_ImageBox.CopyBitmap( &bmpCurrImage );

		image.ReleaseDC();
		image.ReleaseDC();
	}
}

void CObjectDetailsDialog::OnPrevObjectClicked()
{
	if ( m_bFromHomePage ) return;
	if ( !m_pMarket ) return;

	int iMax = m_pMarket->GetItemCount();
	//int iSelected = m_pMarket->GetItemClicked( );
	//if ( m_iItemSelected == 0 )
	{
	//	m_iItemSelected = iMax;
		//m_btnPrevObject.EnableWindow( FALSE );
		//MessageBox( "No More Objects", "Error" );
		//return;
	}

	m_btnNextObject.EnableWindow( TRUE );

	//m_pMarket->SetSelectionMark( iSelected - 1 );
	//m_pMarket->SetItemClicked( iSelected - 1 );

	int iOldItem = m_iItemSelected;

	char szText [ 64 ];
	do
	{
		m_iItemSelected--;
		if ( m_iItemSelected < 0 ) m_iItemSelected = iMax - 1;
		m_pMarket->GetItemText( m_iItemSelected, 0, szText, 64 );
	} while ( strchr( szText, '\\' ) && m_iItemSelected != iOldItem );

	if ( m_iItemSelected == iOldItem ) return;

	//m_pCurrObject = MarketObject::GetObject( m_iGroup, szText );
	m_pCurrObject = MarketObject::GetObjectByID( m_pMarket->GetObjByIndex( m_iItemSelected ), m_iGroup );
	if ( !m_pCurrObject )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return;
	}

	m_GifBox.ShowWindow( SW_HIDE );
	m_GifBox.UnLoad();
	m_ImageBox.Reset( );

	PopulateValues( );
}

void CObjectDetailsDialog::OnNextObjectClicked()
{
	if ( m_bFromHomePage ) return;
	if ( !m_pMarket ) return;

	int iMax = m_pMarket->GetItemCount();
	//int iSelected = m_pMarket->GetItemClicked( );
	//if ( m_iItemSelected + 1 >= iMax )
	{
	//	m_iItemSelected = -1;
		//m_btnNextObject.EnableWindow( FALSE );
		//MessageBox( "No More Objects", "Error" );
		//return;
	}

	m_btnPrevObject.EnableWindow( TRUE );

	//m_pMarket->SetSelectionMark( iSelected + 1 );
	//m_pMarket->SetItemClicked( iSelected + 1 );

	int iOldItem = m_iItemSelected;

	char szText [ 64 ];
	do
	{
		m_iItemSelected++;
		if ( m_iItemSelected >= iMax ) m_iItemSelected = 0;
		m_pMarket->GetItemText( m_iItemSelected, 0, szText, 64 );
	} while ( strchr( szText, '\\' ) && m_iItemSelected != iOldItem );

	if ( m_iItemSelected == iOldItem ) return;

	//m_pCurrObject = MarketObject::GetObject( m_iGroup, szText );
	m_pCurrObject = MarketObject::GetObjectByID( m_pMarket->GetObjByIndex( m_iItemSelected ), m_iGroup );
	if ( !m_pCurrObject )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return;
	}

	m_GifBox.ShowWindow( SW_HIDE );
	m_GifBox.UnLoad();
	m_ImageBox.Reset( );

	PopulateValues( );
}

void CObjectDetailsDialog::OnYourRatingClicked()
{
	if ( !m_pCurrObject ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return;
	}

	m_btnRate.ShowWindow( SW_HIDE );
	m_comboRating.ShowWindow( SW_HIDE );
	m_txtYourRate.ShowWindow( SW_HIDE );

	char szRating [ 16 ];
	m_comboRating.GetWindowText( szRating, 16 );
	char id [ 16 ];
	sprintf_s( id, 16, "%d", m_pCurrObject->GetID() );

	HTTPComm server;
	if ( server.Rate( id, szRating ) )
	{
		//MessageBox( GetLanguageData( _T("TGC Store"), _T("FinishRating") ) );

		m_pCurrObject->UpdateRating( (DWORD) atoi( szRating ) );
		m_txtRating.SetRating( m_pCurrObject->GetTotalRating(), m_pCurrObject->GetRatingCount() );
		m_txtRating.Invalidate();
		Invalidate( );
	}
	else
	{
		MessageBox( server.GetLastError(), "Rating Error" );
	}
}
#endif

void CObjectDetailsDialog::OnAddToCart()
{
	if ( !m_pCurrObject )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return;
	}

	/*
	if ( pObject->IsPurchased( ) )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("CartNoAdd") ) );
		return;
	}
	*/

	m_bCartStateChanged = true;

	if ( m_bAddToCart )
	{
		if ( m_pWindow->IsBuying() )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("CartNoRemoveBuying") ) );
			return;
		}

		bool bInCart = MarketObject::IsObjectInCart( m_pCurrObject->GetID() );

		m_btnAddToCart.SetWindowTextA( GetLanguageData( _T("TGC Store"), _T("AddToCart") ) );
		m_bAddToCart = false;

		if ( !bInCart ) return;

		MarketObject::RemoveFromCart( m_pCurrObject->GetID() );
		//m_btnAddToCart.EnableWindow( FALSE );
	}
	else
	{
		if ( m_pWindow->IsBuying() )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("CartNoAddBuying") ) );
			return;
		}

		bool bInCart = MarketObject::IsObjectInCart( m_pCurrObject->GetID() );

		m_btnAddToCart.SetWindowTextA( GetLanguageData( _T("TGC Store"), _T("RemoveFromCart") ) );
		m_bAddToCart = true;

		if ( bInCart )
		{
			//MessageBox( GetLanguageData( _T("TGC Store"), _T("CartAlreadyAdd") ) );
			return;
		}

		MarketObject::AddToCart( m_pCurrObject );
		//m_btnAddToCart.EnableWindow( FALSE );		
	}
}

void CObjectDetailsDialog::OnPlaySample()
{
	if ( !op.IsPlaying() )
	{
		op.Play();
		SetTimer( 3, 500, NULL );

		m_btnPlaySample.SetWindowText( "||" );
	}
	else
	{
		KillTimer( 3 );
		op.Stop();

		m_btnPlaySample.SetWindowText( ">" );
	}
}
