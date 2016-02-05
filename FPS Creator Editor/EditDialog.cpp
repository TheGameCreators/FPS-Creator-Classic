// EditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EditDialog.h"
#include "TGC Store\MarketObject.h"
#include "TGC Store\HTTPComm.h"
#include "TGC Store\UploadAppDlg.h"
#include "Editor.h"
#include <atlimage.h>
#include "TGC Store\\unzip.h"


// CEditDialog dialog

IMPLEMENT_DYNAMIC(CEditDialog, CDialog)

CEditDialog::CEditDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditDialog::IDD, pParent)
{
	m_pObject = NULL;

	m_sName = "";
	m_sCost = "";
	m_sPolygons = "";
	m_sKeywords = "";
	m_sDescription = "";
	m_sModelFile = "";
	m_sThumbnail = "";
	m_iNumImages = 0;
	m_psPreviewImages = NULL;
	m_bPreviewImagesChanged = false;
}

CEditDialog::~CEditDialog()
{
}

bool CEditDialog::CheckStringAlNum( CString sField, CString sString )
{
	for ( int i = 0; i < sString.GetLength(); i++ )
	{
		if ( !isalnum(sString.GetAt( i )) )
		{
			CString sMsg;
			sMsg.Format( "\"%s\" %s", sField.GetString( ), GetLanguageData( _T("Uploader"), _T("AlphaNumInput") ) );
			MessageBox( sMsg, theApp.m_szErrorString );
			return false;
		}
	}

	return true;
}

bool CEditDialog::CheckStringDigit( CString sField, CString sString )
{
	for ( int i = 0; i < sString.GetLength(); i++ )
	{
		if ( !isdigit(sString.GetAt( i )) )
		{
			CString sMsg;
			sMsg.Format( "\"%s\" %s", sField.GetString( ), GetLanguageData( _T("Uploader"), _T("DigitInput") ) );
			MessageBox( sMsg, theApp.m_szErrorString );
			return false;
		}
	}

	return true;
}

bool CEditDialog::CheckStringAlNum2( CString sField, CString sString )
{
	if ( sString.FindOneOf( "|\\/?\"*><" ) > -1 )
	{
		CString sMsg;
		sMsg.Format( "\"%s\" %s:  | \\ / ? \" * > <", sField.GetString( ), GetLanguageData( _T("Uploader"), _T("GeneralInput") ) );
		MessageBox( sMsg, theApp.m_szErrorString );
		return false;
	}

	return true;
}

bool CEditDialog::CheckStringDescription( CString sField, CString sString )
{
	if ( sString.FindOneOf( "&+=;/?" ) > -1 )
	{
		CString sMsg;
		sMsg.Format( "\"%s\" %s:  : - _ ' A-Z 0-9 : . , ! ( ) ", sField.GetString( ), GetLanguageData( _T("Uploader"), _T("DescriptionInput") ) );
		MessageBox( sMsg, theApp.m_szErrorString );
		return false;
	}

	return true;
}

void CEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT6, m_editName);
	DDX_Control(pDX, IDC_CHECK3, m_chkX9);
	DDX_Control(pDX, IDC_CHECK4, m_chkX10);
	DDX_Control(pDX, IDC_EDIT4, m_editCost);
	DDX_Control(pDX, IDC_EDIT5, m_editPolygons);
	DDX_Control(pDX, IDC_EDIT8, m_editDescription);
	DDX_Control(pDX, IDC_CHECK1, m_chkResetRating);
	DDX_Control(pDX, IDC_CHECK2, m_chkResetDownloadCount);
	DDX_Control(pDX, IDC_STORE_BORDER, m_BoxStoreGroup);
	DDX_Control(pDX, IDC_EDIT1, m_editModelFile);
	DDX_Control(pDX, IDC_COMBO2, m_comboKeywords);
	DDX_Control(pDX, IDC_EDIT2, m_editThumbnail);
	DDX_Control(pDX, IDC_COMBO3, m_comboPreviewImages);
	DDX_Control(pDX, IDC_SAMPLE_IMAGE, m_imgPreview);
	DDX_Control(pDX, IDC_THUMBNAIL, m_imgThumbnail);
	DDX_Control(pDX, IDC_PROGRESS_SPINNER, m_ProgressSpinner);
	DDX_Control(pDX, IDC_UPLOAD_TEXT, m_textUploading);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Control(pDX, IDC_VIDEO_LINK, m_labelVideoLink);
	DDX_Control(pDX, IDC_EDIT10, m_editVideoLink);
	DDX_Control(pDX, IDC_MODEL_DESC, m_labelModelDesc);
	DDX_Control(pDX, IDC_MODEL_FILE, m_labelNewModel);
	DDX_Control(pDX, IDC_THUMB_LABEL, m_labelNewThumbnail);
	DDX_Control(pDX, IDC_LABEL_PREVIEW, m_labelNewImages);
	DDX_Control(pDX, IDC_THUMB_IMAGE_LABEL, m_labelThumbnail);
	DDX_Control(pDX, IDC_PREVIEW_IMAGE_LABEL, m_labelImages);
	DDX_Control(pDX, IDC_LABEL_NAME, m_labelName);
	DDX_Control(pDX, IDC_LABEL_COMPAT, m_labelCompatible);
	DDX_Control(pDX, IDC_LABEL_COST, m_labelCost);
	DDX_Control(pDX, IDC_LABEL_POLYGONS, m_labelPolygons);
	DDX_Control(pDX, IDC_LABEL_KEYWORDS, m_labelKeywords);
	DDX_Control(pDX, IDC_LABEL_DESC, m_labelDescription);
	DDX_Control(pDX, IDC_BUTTON3, m_btnAddImage);
	DDX_Control(pDX, IDC_BUTTON4, m_btnRemoveImage);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnApply);
	DDX_Control(pDX, IDC_THUMB_LABEL2, m_labelNewMusicSample);
	DDX_Control(pDX, IDC_EDIT3, m_editNewMusicSample);
	DDX_Control(pDX, IDC_BUTTON7, m_btnBrowseMusic);
}

BEGIN_MESSAGE_MAP(CEditDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CEditDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditDialog::OnNewModelFile)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditDialog::OnNewThumbnail)
	ON_BN_CLICKED(IDC_BUTTON3, &CEditDialog::OnAddPreviewImage)
	ON_BN_CLICKED(IDC_BUTTON4, &CEditDialog::OnRemovePreviewImage)
	ON_BN_CLICKED(IDC_BUTTON5, &CEditDialog::OnAddKeyword)
	ON_BN_CLICKED(IDC_BUTTON6, &CEditDialog::OnRemoveKeyword)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CEditDialog::OnChangedPreviewSelection)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CEditDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON7, &CEditDialog::OnBrowseMusic)
END_MESSAGE_MAP()

void CEditDialog::LoadThumbnail( )
{
	CString sFullPath = theApp.m_szDirectory;
	sFullPath += "\\Files\\TGCStore\\TEMP\\Thumbnails\\";
	sFullPath += m_pObject->GetThumbnail( );

	CImage image;
	HRESULT hr = image.Load( sFullPath );

	if ( FAILED(hr) )
	{
		CString sPath = "Files\\TGCStore\\TEMP\\Thumbnails\\";
		sPath += m_pObject->GetThumbnail( );

		CString sThumbnail = "Images\\Thumbnails\\";
		sThumbnail += m_pObject->GetThumbnail( );

		HTTPComm server;
		bool result = server.GetPreviewImage( sThumbnail, sPath );
		if ( !result ) 
		{
			MessageBox( server.GetLastError( ), "Server Error" );
			return;
		}
		
		hr = image.Load( sFullPath );
		if ( FAILED(hr) ) return;
	}

	CWindowDC wndDC( this );
	CDC dc;	dc.CreateCompatibleDC( &wndDC );

	CBitmap bmpThumb;
	bmpThumb.CreateBitmap( 64,64, 1, 32, NULL );
	CBitmap *oldBmp = dc.SelectObject( &bmpThumb );
	image.BitBlt( dc.GetSafeHdc( ), 0,0 );
	dc.SelectObject( oldBmp );
	m_imgThumbnail.CopyBitmap( &bmpThumb );
	image.Destroy();	
}

BOOL CEditDialog::OnInitDialog( )
{
	if ( !CDialog::OnInitDialog() ) return FALSE;

	m_labelModelDesc.SetWindowText		( GetLanguageData( _T("Uploader"), _T("ModelDesc") ) );
	m_labelNewModel.SetWindowText		( GetLanguageData( _T("Uploader"), _T("NewModel") ) );
	m_labelNewThumbnail.SetWindowText	( GetLanguageData( _T("Uploader"), _T("NewThumbnail") ) );
	m_labelNewImages.SetWindowText		( GetLanguageData( _T("Uploader"), _T("PreviewLabel") ) );
	m_labelThumbnail.SetWindowText		( GetLanguageData( _T("Uploader"), _T("ThumbnailLabel") ) );
	m_labelImages.SetWindowText			( GetLanguageData( _T("Uploader"), _T("PreviewLabel") ) );
	m_labelNewMusicSample.SetWindowText	( GetLanguageData( _T("Uploader"), _T("NewMusicSampleLabel") ) );
	m_labelName.SetWindowText			( GetLanguageData( _T("Uploader"), _T("NameLabel") ) );
	m_labelCompatible.SetWindowText		( GetLanguageData( _T("Uploader"), _T("CompatibleLabel") ) );
	m_labelCost;
	m_labelPolygons.SetWindowText		( GetLanguageData( _T("Uploader"), _T("PolygonsLabel") ) );
	m_labelKeywords.SetWindowText		( GetLanguageData( _T("Uploader"), _T("KeywordsLabel") ) );
	m_labelDescription.SetWindowText	( GetLanguageData( _T("Uploader"), _T("DescriptionLabel") ) );
	m_labelVideoLink.SetWindowText		( GetLanguageData( _T("Uploader"), _T("VideoLabel") ) );

	m_btnAddImage.SetWindowText			( GetLanguageData( _T("Uploader"), _T("Add") ) );
	m_btnRemoveImage.SetWindowText		( GetLanguageData( _T("Uploader"), _T("Remove") ) );
	m_btnCancel.SetWindowText			( GetLanguageData( _T("Uploader"), _T("Cancel") ) );
	m_btnApply.SetWindowText			( GetLanguageData( _T("Uploader"), _T("Apply") ) );
	
	if ( m_pObject )
	{
		m_editName.SetWindowText( m_pObject->GetName() );
		m_chkX9.SetCheck( m_pObject->IsX9Compatible() ? 1 : 0 );
		m_chkX10.SetCheck( m_pObject->IsX10Compatible() ? 1 : 0 );
		m_editDescription.SetWindowText( m_pObject->GetDescription() );
		m_editVideoLink.SetWindowText( m_pObject->GetVideoLink() );

		m_sName = m_pObject->GetName();

		char szNum [ 16 ];
		sprintf_s( szNum, 16, "%d", m_pObject->GetCost() );
		m_editCost.SetWindowText( szNum );
		
		sprintf_s( szNum, 16, "%d", m_pObject->GetPolygons() );
		m_editPolygons.SetWindowText( szNum );

		for ( DWORD i = 0; i < m_pObject->GetNumKeywords(); i++ )
		{
			m_comboKeywords.AddString( m_pObject->GetKeyword( i ) );
		}

		for ( DWORD i = 0; i < m_pObject->GetNumImages(); i++ )
		{
			m_comboPreviewImages.AddString( m_pObject->GetImage( i ) );
		}

		LoadThumbnail( );

		if ( !theApp.m_bAdmin )
		{
			m_chkResetRating.ShowWindow( SW_HIDE );
			m_chkResetDownloadCount.ShowWindow( SW_HIDE );
			m_BoxStoreGroup.ShowWindow( SW_HIDE );
		}

		if ( strcmp( m_pObject->GetType(), "Music" ) == 0 )
		{
			m_comboPreviewImages.EnableWindow( FALSE );
			m_btnAddImage.EnableWindow( FALSE );
			m_btnRemoveImage.EnableWindow( FALSE );

			m_editNewMusicSample.EnableWindow( FALSE );
			m_btnBrowseMusic.EnableWindow( TRUE );
		}
		else
		{
			m_comboPreviewImages.EnableWindow( TRUE );
			m_btnAddImage.EnableWindow( TRUE );
			m_btnRemoveImage.EnableWindow( TRUE );

			m_editNewMusicSample.EnableWindow( FALSE );
			m_btnBrowseMusic.EnableWindow( FALSE );
		}
	}
	else
	{
		MessageBox( "Failed to get current values, editing is likely to fail.", "Warning" );
	}

	return TRUE;
}

// CEditDialog message handlers

void CEditDialog::OnBnClickedOk()
{
	if ( !m_pObject ) 
	{
		MessageBox( "Could not get object details", "Error" );
		return;
	}

	if ( m_EditThread.IsUploading( ) )
	{
		MessageBox( "A previous edit is still being processed, please wait for it to finish" );
		return;
	}

	m_sModelFile = "";
	m_sThumbnail = "";
	m_iNumImages = -1;
	if ( m_psPreviewImages ) delete [] m_psPreviewImages;
	m_psPreviewImages = NULL;
	m_bPreviewImagesChanged = false;
	m_sMusicSample = "";

	m_editCost.GetWindowText( m_sCost );
	m_editPolygons.GetWindowText( m_sPolygons );
	m_editDescription.GetWindowText( m_sDescription );

	if ( m_chkX9.GetCheck() == 0 && m_chkX10.GetCheck() == 0 )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("Compatibility") ), theApp.m_szErrorString );
		return;
	}

	CString sMsg;

	if ( m_sCost.GetLength() < 1 ) m_sCost = "0"; //optional
	if ( m_sPolygons.GetLength() < 1 ) 
	{ 
		sMsg.Format( "'%s' %s",  GetLanguageData( _T("Uploader"), _T("Polygons") ),  GetLanguageData( _T("Uploader"), _T("MustExist") ) ); 
		MessageBox( sMsg, theApp.m_szErrorString ); 
		return; 
	}
	if ( m_sDescription.GetLength() < 5 ) 
	{ 
		sMsg.Format( "'%s' %s",  GetLanguageData( _T("Uploader"), _T("Description") ),  GetLanguageData( _T("Uploader"), _T("AtLeastFive") ) ); 
		MessageBox( sMsg, theApp.m_szErrorString ); 
		return; 
	}

	if ( !CheckStringDigit( GetLanguageData( _T("Uploader"), _T("Polygons") ), m_sPolygons ) ) return;
	if ( !CheckStringDigit( GetLanguageData( _T("Uploader"), _T("SuggestedCost") ), m_sCost ) ) return;
	if ( !CheckStringDescription( GetLanguageData( _T("Uploader"), _T("Description") ), m_sDescription ) ) return;

	m_sKeywords = "";
	if ( m_comboKeywords.GetCount() >= 1 )
	{
		CString sTemp;
		m_comboKeywords.GetLBText( 0, sTemp );
		if ( !CheckStringAlNum( GetLanguageData( _T("Uploader"), _T("Keywords") ), sTemp ) ) return;
		m_sKeywords = sTemp;
	}

	for ( int i = 1; i < m_comboKeywords.GetCount(); i++ )
	{
		CString sTemp;
		m_comboKeywords.GetLBText( i, sTemp );
		if ( !CheckStringAlNum( GetLanguageData( _T("Uploader"), _T("Keywords") ), sTemp ) ) return;
		m_sKeywords += ";";
		m_sKeywords += sTemp;
	}

	m_editModelFile.GetWindowText( m_sModelFile );
	m_editThumbnail.GetWindowText( m_sThumbnail );
	m_editNewMusicSample.GetWindowText( m_sMusicSample );

	/*
	if ( m_sModelFile.GetLength( ) < 1 ) 
	{
		MessageBox( GetLanguageData( _T("Uploader"), _T("InvalidModel") ), theApp.m_szErrorString );
		return;
	}
	*/

	if ( m_sModelFile.GetLength() > 0 )
	{
		int iType = 0;
		if ( strcmp( m_pObject->GetType(), "Entity" ) == 0 ) iType = 1;
		else if ( strcmp( m_pObject->GetType(), "Segment" ) == 0 ) iType = 2;

		CString sItemName = "";
		CUploadAppDlg::CheckModelFileFormat( m_sModelFile, iType, sItemName );

		if ( sItemName.GetLength() < 1 )
		{
			//LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			if ( iType == 1 ) MessageBox( GetLanguageData( "Uploader", "InvalidName" ), theApp.m_szErrorString );
			else if ( iType == 2 ) MessageBox( GetLanguageData( "Uploader", "InvalidSegmentName" ), theApp.m_szErrorString );
			return;
		}

		SetCurrentDirectory( theApp.m_szDirectory );

		//LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		/*
		//check fpe
		if ( bEntity )
		{
			CString sFPEPath = theApp.m_szDirectory;
			sFPEPath += "\\Files\\TGCStore\\Uploads\\";
			sFPEPath += szBaseName;
			sFPEPath += "\\entitybank\\";
			sFPEPath += sNewModelName;
			sFPEPath += ".fpe";

			FILE *fp = fopen( sFPEPath, "rb" );
			if ( !fp ) 
			{
				MessageBox( GetLanguageData( "Uploader", "InvalidFPE" ), theApp.m_szErrorString );
				return;
			}
			fclose( fp );
		}
		*/
	}

	//check thumbnail path
	if ( m_sThumbnail.GetLength() > 0 )
	{
		FILE *fp = fopen( m_sThumbnail, "rb" );
		if ( !fp ) 
		{
			MessageBox( GetLanguageData( "Uploader", "InvalidThumbnail" ), theApp.m_szErrorString );
			return;
		}
		fclose( fp );
	}
	
	if ( m_bPreviewImagesChanged && strcmp( m_pObject->GetType(), "Music" ) != 0 )
	{
		m_iNumImages = m_comboPreviewImages.GetCount( );
		if ( m_psPreviewImages ) delete [] m_psPreviewImages;
		m_psPreviewImages = NULL;
		
		if ( m_iNumImages > 0 )
		{
			m_psPreviewImages = new CString [ m_iNumImages ];
			for ( int i = 0 ; i < m_iNumImages; i++ )
			{
				m_comboPreviewImages.GetLBText( i, m_psPreviewImages [ i ] );

				//old image, make sure we have a local copy to re-upload
				if ( strncmp( m_psPreviewImages [ i ], "Images\\", strlen("Images\\") ) == 0 )
				{
					CString sFullPath = theApp.m_szDirectory;
					sFullPath += "\\Files\\TGCStore\\TEMP\\";
					sFullPath += m_psPreviewImages [ i ];

					CImage image;
					HRESULT hr = image.Load( sFullPath );

					if ( FAILED(hr) )
					{
						CString sPath = "Files\\TGCStore\\TEMP\\";
						sPath += m_psPreviewImages [ i ];

						HTTPComm server;
						bool result = server.GetPreviewImage( m_psPreviewImages [ i ], sPath );
						if ( !result ) 
						{
							MessageBox( server.GetLastError( ), "Server Error" );
							return;
						}
					}

					m_psPreviewImages [ i ] = sFullPath;
				}
			}
		}
	}
	else
	{
		m_iNumImages = -1;
		if ( m_psPreviewImages ) delete [] m_psPreviewImages;
		m_psPreviewImages = NULL;
	}

	if ( m_sMusicSample.GetLength() > 0 && strcmp( m_pObject->GetType(), "Music" ) == 0 )
	{
		FILE *fp = fopen( m_sMusicSample, "rb" );
		if ( !fp ) 
		{
			MessageBox( "Invalid music sample file", theApp.m_szErrorString );
			return;
		}
		fclose( fp );
	}
	else
	{
		m_sMusicSample = "";
	}

	if ( theApp.m_szSESID )
	{
		int iX9Compatible = m_chkX9.GetCheck() > 0 ? 1 : 0;
		int iX10Compatible = m_chkX10.GetCheck() > 0 ? 1 : 0;
		int iResetRating = m_chkResetRating.GetCheck() > 0 ? 1 : 0;
		int iResetDownloads = m_chkResetDownloadCount.GetCheck() > 0 ? 1 : 0;

		CString sCost;
		CString sPolygons;
		CString sDescription;

		m_editCost.GetWindowText( sCost );
		m_editPolygons.GetWindowText( sPolygons );
		m_editDescription.GetWindowText( sDescription );
		m_editVideoLink.GetWindowText( m_sVideo );

		if ( sCost.GetLength() < 1 ) sCost = "";
		if ( sPolygons.GetLength() < 1 ) sPolygons = "";

		HTTPComm server;
		bool bResult = server.EditObject( m_pObject->GetID(), iX9Compatible, iX10Compatible, sCost, sPolygons, sDescription, m_sVideo, m_sKeywords, iResetRating, iResetDownloads );
		if ( !bResult )
		{
			//MessageBox( "Error updating object data", "Server Error" );
			return;
		}
		
		//m_EditThread.SetValues( m_sName, m_sCost, m_sType, m_sCategory, m_sSubCategory, m_sPolygons, m_sKeywords, m_sDescription, m_dwGroupSelected, m_chkX9Compatible.GetCheck(), m_chkX10Compatible.GetCheck() );
		m_EditThread.SetFilenames( &m_ProgressBar, m_pObject->GetID(), m_sName, m_sModelFile, m_sThumbnail, m_iNumImages, m_psPreviewImages, m_sMusicSample );
		m_EditThread.Start( );
		
		m_ProgressSpinner.ShowWindow( SW_SHOW );
		m_ProgressSpinner.StartSpinning( );
		m_ProgressBar.SetPos( 0 );
		m_ProgressBar.ShowWindow( SW_SHOW );
		m_textUploading.SetWindowText( "Uploading:" );
		m_textUploading.ShowWindow( SW_SHOW );
		SetTimer( 1, 1000, NULL );
	}

	//OnOK();
}

void CEditDialog::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		if ( !m_EditThread.IsUploading( ) )
		{
			KillTimer( 1 );
			m_ProgressSpinner.StopSpinning();
			m_ProgressSpinner.ShowWindow( SW_HIDE );
			m_ProgressBar.ShowWindow( SW_HIDE );
			if ( m_EditThread.IsSuccessful() ) m_textUploading.SetWindowText( "Upload Finished" );
			else m_textUploading.SetWindowText( "Upload Failed" );

			EnterCriticalSection( &theApp.m_csDirectoryChanges );

			//clear upload folder
			if ( !SetCurrentDirectory( theApp.m_szDirectory ) ) 
			{
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );
				return;
			}
			if ( !SetCurrentDirectory( "Files\\TGCStore\\Uploads" ) ) 
			{
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );
				return;
			}

			WIN32_FIND_DATA FindFileData;
			HANDLE hFind = INVALID_HANDLE_VALUE;
				
			hFind = FindFirstFile("*.*", &FindFileData);

			if (hFind != INVALID_HANDLE_VALUE) 
			{
				do 
				{
					if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
					{
						if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
						{
							DeleteFile( FindFileData.cFileName );
						}
					}
				} while (FindNextFile(hFind, &FindFileData) != 0);

				FindClose(hFind);
			}

			//clear old preview images
			if ( m_bPreviewImagesChanged )
			{
				if ( !SetCurrentDirectory( theApp.m_szDirectory ) ) 
				{
					LeaveCriticalSection( &theApp.m_csDirectoryChanges );
					return;
				}
				if ( !SetCurrentDirectory( "Files\\TGCStore\\TEMP\\Images" ) ) 
				{
					LeaveCriticalSection( &theApp.m_csDirectoryChanges );
					return;
				}

				WIN32_FIND_DATA FindFileData;
				HANDLE hFind = INVALID_HANDLE_VALUE;
					
				hFind = FindFirstFile("*.*", &FindFileData);

				if (hFind != INVALID_HANDLE_VALUE) 
				{
					do 
					{
						if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
						{
							if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
							{
								if ( strncmp( FindFileData.cFileName, m_pObject->GetName(), strlen(m_pObject->GetName()) ) == 0 ) 
								{
									DeleteFile( FindFileData.cFileName );
								}
							}
						}
					} while (FindNextFile(hFind, &FindFileData) != 0);

					FindClose(hFind);
				}
			}

			LeaveCriticalSection( &theApp.m_csDirectoryChanges );

			HTTPComm server;
			if ( !server.GetModelDetails( m_pObject->GetID(), theApp.m_szSESID ) )
			{
				//MessageBox( server.GetLastError(), "Server Error" );
				return;
			}
			m_pObject->CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );

			CString sThumbnailFile = theApp.m_szDirectory;
			sThumbnailFile += "\\Files\\TGCStore\\TEMP\\Thumbnails\\";
			sThumbnailFile += m_pObject->GetThumbnail();

			FILE *pFile = fopen( sThumbnailFile, "r" );
			if ( pFile )
			{
				fclose( pFile );
				DeleteFile( sThumbnailFile );
			}

			//if ( m_pMarket ) m_pMarket->UploadComplete( );
		}
	}

	if ( nIDEvent == 99 )
	{
		if ( !m_EditThread.IsUploading() )
		{
			CDialog::OnCancel();
		}
	}
}

void CEditDialog::OnNewModelFile()
{
	CFileDialog fileSelect( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "Zip Files (*.zip)|*.zip||" );
	int result = (int) fileSelect.DoModal();
	if ( result != IDOK ) return;

	CString sFullPath = fileSelect.GetPathName( );
	m_editModelFile.SetWindowText( sFullPath.GetString() );

	m_editModelFile.EnableWindow( );
}

void CEditDialog::OnNewThumbnail()
{
	CFileDialog fileSelect( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "Image Files (*.bmp, *.jpg, *.png)|*.bmp;*.jpg;*.png||" );
	int result = (int) fileSelect.DoModal();
	if ( result != IDOK ) return;

	CString sFullPath = fileSelect.GetPathName( );
	m_editThumbnail.SetWindowText( sFullPath.GetString() );
	m_editThumbnail.EnableWindow( );

	CImage image;
	HRESULT hr = image.Load( sFullPath );

	if ( FAILED(hr) )
	{
		MessageBox( "Failed to load thumbnail" );
	}

	CWindowDC wndDC( this );
	CDC dc;	dc.CreateCompatibleDC( &wndDC );

	CBitmap bmpThumb;
	bmpThumb.CreateBitmap( 64,64, 1, 32, NULL );
	CBitmap *oldBmp = dc.SelectObject( &bmpThumb );
	image.BitBlt( dc.GetSafeHdc( ), 0,0 );
	dc.SelectObject( oldBmp );
	m_imgThumbnail.CopyBitmap( &bmpThumb );
	image.Destroy();	
}

void CEditDialog::OnAddPreviewImage()
{
	CFileDialog fileSelect( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "Image Files (*.bmp, *.jpg, *.png, *.gif)|*.bmp;*.jpg;*.png;*.gif||" );
	int result = (int) fileSelect.DoModal();
	if ( result != IDOK ) return;

	CString sFullPath = fileSelect.GetPathName( );
	m_comboPreviewImages.AddString( sFullPath.GetString() );
	m_comboPreviewImages.ShowDropDown();

	CImage image;
	HRESULT hr = image.Load( sFullPath );

	if ( FAILED(hr) )
	{
		MessageBox( "Failed to load image, make sure it is one of: .bmp .jpg .png .gif", theApp.m_szErrorString );
		return;
	}

	if ( image.GetWidth() != 368 || image.GetHeight() != 368 )
	{
		MessageBox( "Preview images must be 368x368 in size", theApp.m_szErrorString );
		return;
	}

	CWindowDC wndDC( this );
	CDC dc;	dc.CreateCompatibleDC( &wndDC );

	CBitmap bmpThumb;
	bmpThumb.CreateBitmap( image.GetWidth(),image.GetHeight(), 1, 32, NULL );
	CBitmap *oldBmp = dc.SelectObject( &bmpThumb );
	image.BitBlt( dc.GetSafeHdc( ), 0,0 );
	dc.SelectObject( oldBmp );
	m_imgPreview.CopyBitmap( &bmpThumb );
	image.Destroy();

	m_bPreviewImagesChanged = true;
}

void CEditDialog::OnRemovePreviewImage()
{
	int index = m_comboPreviewImages.GetCurSel();
	if ( index == CB_ERR )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("SelectPreview") ), theApp.m_szErrorString );
		return;
	}

	m_comboPreviewImages.DeleteString( index );

	m_imgPreview.Reset();

	m_bPreviewImagesChanged = true;
}

void CEditDialog::OnAddKeyword()
{
	CString sText;
	m_comboKeywords.GetWindowText( sText );
	if ( sText.GetLength() < 1 )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("BlankKeyword") ), theApp.m_szErrorString );
		return;
	}

	m_comboKeywords.AddString( sText.GetString() );
	m_comboKeywords.ShowDropDown();
	m_comboKeywords.SetWindowText( "" );
}

void CEditDialog::OnRemoveKeyword()
{
	int index = m_comboKeywords.GetCurSel();
	if ( index == CB_ERR )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("SelectKeyword") ), theApp.m_szErrorString );
		return;
	}

	m_comboKeywords.DeleteString( index );
}

void CEditDialog::OnChangedPreviewSelection()
{
	CString sImage;
	m_comboPreviewImages.GetWindowText( sImage );

	if ( strncmp( sImage.GetString(), "Images\\", strlen("Images\\") ) == 0 )
	{
		//previous image
		CString sFullPath = theApp.m_szDirectory;
		sFullPath += "\\Files\\TGCStore\\TEMP\\";
		sFullPath += sImage;

		CImage image;
		HRESULT hr = image.Load( sFullPath );

		if ( FAILED(hr) )
		{
			CString sPath = "Files\\TGCStore\\TEMP\\";
			sPath += sImage;

			HTTPComm server;
			bool result = server.GetPreviewImage( sImage, sPath );
			if ( !result ) 
			{
				MessageBox( server.GetLastError( ), "Server Error" );
				return;
			}
			
			hr = image.Load( sFullPath );
		}

		CWindowDC wndDC( this );
		CDC dc;	dc.CreateCompatibleDC( &wndDC );

		CBitmap bmpImage;
		bmpImage.CreateBitmap( image.GetWidth(),image.GetHeight(), 1, 32, NULL );
		CBitmap *oldBmp = dc.SelectObject( &bmpImage );
		image.BitBlt( dc.GetSafeHdc( ), 0,0 );
		dc.SelectObject( oldBmp );
		m_imgPreview.CopyBitmap( &bmpImage );
		image.Destroy();	
	}
	else
	{
		//new image
		CImage image;
		HRESULT hr = image.Load( sImage );

		if ( FAILED(hr) )
		{
			MessageBox( "Failed to load image" );
			return;
		}

		CWindowDC wndDC( this );
		CDC dc;	dc.CreateCompatibleDC( &wndDC );

		CBitmap bmpImage;
		bmpImage.CreateBitmap( image.GetWidth(),image.GetHeight(), 1, 32, NULL );
		CBitmap *oldBmp = dc.SelectObject( &bmpImage );
		image.BitBlt( dc.GetSafeHdc( ), 0,0 );
		dc.SelectObject( oldBmp );
		m_imgPreview.CopyBitmap( &bmpImage );
		image.Destroy();	
	}
}

void CEditDialog::OnBnClickedCancel()
{
	bool bWait = false;

	if ( m_EditThread.IsUploading() )
	{
		m_textUploading.SetWindowText( "Stopping..." );
		m_EditThread.Cancel();
		//m_EditThread.Join();
		bWait = true;
	}

	if ( bWait ) SetTimer( 99, 1000, NULL );
	else CDialog::OnCancel();
}

void CEditDialog::OnBrowseMusic()
{
	CFileDialog fileSelect( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "OGG Files (*.ogg)|*.ogg||" );
	int result = (int) fileSelect.DoModal();
	if ( result != IDOK ) return;

	CString sFullPath = fileSelect.GetPathName( );
	m_editNewMusicSample.SetWindowText( sFullPath.GetString() );
	m_editNewMusicSample.EnableWindow( );
}
