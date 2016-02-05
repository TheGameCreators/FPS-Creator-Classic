// UploadAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Editor.h"
#include "UploadAppDlg.h"
#include "LoginDialog.h"
#include "HTTPComm.h"
#include "MarketObject.h"
#include "MarketWindow.h"
#include "unzip.h"
#include <atlimage.h>
//#include "NewGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CUploadAppDlg::CheckStringAlNum( CString sField, CString sString )
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

bool CUploadAppDlg::CheckStringDigit( CString sField, CString sString )
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

bool CUploadAppDlg::CheckStringAlNum2( CString sField, CString sString )
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

bool CUploadAppDlg::CheckStringDescription( CString sField, CString sString )
{
	//not a full check, just check characters that cause PHP POST problems
	if ( sString.FindOneOf( "&+=;/?" ) > -1 )
	{
		CString sMsg;
		sMsg.Format( "\"%s\" %s:  : - _ ' A-Z 0-9 : . , ! ( ) ", sField.GetString( ), GetLanguageData( _T("Uploader"), _T("DescriptionInput") ) );
		MessageBox( sMsg, theApp.m_szErrorString );
		return false;
	}

	return true;
}


CUploadAppDlg::CUploadAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwGroupSelected = 0;
	m_psPreviewImages = NULL;

	m_bRunning = false;
	m_pMarket = NULL;
}

CUploadAppDlg::~CUploadAppDlg( )
{
	Terminate();
}

void CUploadAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TREE1, m_GroupList);
	DDX_Control(pDX, IDC_EDIT1, m_editModelFile);
	DDX_Control(pDX, IDC_COMBO3, m_comboPreviewImages);
	//DDX_Control(pDX, IDC_EDIT3, m_editName);
	DDX_Control(pDX, IDC_EDIT4, m_editCost);
	DDX_Control(pDX, IDC_COMBO1, m_comboType);
	DDX_Control(pDX, IDC_EDIT5, m_editPolygons);
	DDX_Control(pDX, IDC_COMBO2, m_comboKeywords);
	DDX_Control(pDX, IDC_UPLOAD_TEXT, m_textUploading);
	DDX_Control(pDX, IDC_PROGRESS_SPINNER, m_ProgressSpinner);
	DDX_Control(pDX, IDC_EDIT8, m_editDescription);
	DDX_Control(pDX, IDC_CHECK2, m_chkAgreement);
	DDX_Control(pDX, IDC_CHECK3, m_chkX9Compatible);
	DDX_Control(pDX, IDC_CHECK4, m_chkX10Compatible);
	DDX_Control(pDX, IDC_COMBO4, m_comboCategory);
	DDX_Control(pDX, IDC_BUTTON9, m_btnCostHelp);
	DDX_Control(pDX, IDC_EDIT6, m_editSubCategory);
	DDX_Control(pDX, IDC_BUTTON2, m_btnCalculate);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Control(pDX, IDC_THUMBNAIL, m_ImageThumbnail);
	DDX_Control(pDX, IDC_EDIT7, m_editName);
	DDX_Control(pDX, IDC_BUTTON7, m_btnCopyFromObject);
	DDX_Control(pDX, IDC_MODEL_DESC, m_labelModelDesc);
	DDX_Control(pDX, IDC_PREVIEW_DESC, m_labelPreviewDesc);
	DDX_Control(pDX, IDC_MODEL_LABEL, m_labelModel);
	DDX_Control(pDX, IDC_PREVIEW_LABEL, m_labelPreview);
	DDX_Control(pDX, IDC_THUMBNAIL_LABEL, m_labelThumbnail);
	DDX_Control(pDX, IDC_NAME_LABEL, m_labelName);
	DDX_Control(pDX, IDC_COMPATIBLE_LABEL, m_labelCompatible);
	DDX_Control(pDX, IDC_TYPE, m_labelType);
	DDX_Control(pDX, IDC_CATEGORY, m_labelCategory);
	DDX_Control(pDX, IDC_SUBCATEGORY, m_labelSubCategory);
	DDX_Control(pDX, IDC_COST, m_labelCost);
	DDX_Control(pDX, IDC_POLYGONS, m_labelPolygons);
	DDX_Control(pDX, IDC_KEYWORDS, m_labelKeywords);
	DDX_Control(pDX, IDC_DESCRIPTION, m_labelDescription);
	DDX_Control(pDX, IDOK, m_btnUpload);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON10, m_btnClearFields);
	DDX_Control(pDX, IDC_ADD, m_btnAddImage);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemoveImage);
	DDX_Control(pDX, IDC_VIDEO_LINK, m_labelVideoLink);
	DDX_Control(pDX, IDC_EDIT10, m_editVideoLink);
	DDX_Control(pDX, IDC_EDIT2, m_editMusicSample);
	DDX_Control(pDX, IDC_BUTTON3, m_btnBrowseMusic);
	DDX_Control(pDX, IDC_BUTTON11, m_btnHelpMusic);
	DDX_Control(pDX, IDC_MODEL_LABEL2, m_labelMusicSample);
}

BEGIN_MESSAGE_MAP(CUploadAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CUploadAppDlg::OnBrowseModelFile)
	ON_BN_CLICKED(IDC_ADD, &CUploadAppDlg::OnAddPreviewImage)
	ON_BN_CLICKED(IDC_REMOVE, &CUploadAppDlg::OnRemovePreviewImage)
	ON_BN_CLICKED(IDC_BUTTON5, &CUploadAppDlg::OnAddKeyword)
	ON_BN_CLICKED(IDC_BUTTON6, &CUploadAppDlg::OnRemoveKeyword)
	ON_BN_CLICKED(IDOK, &CUploadAppDlg::OnUpload)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, &CUploadAppDlg::OnModelHelpClicked)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CUploadAppDlg::OnTypeChanged)
	ON_BN_CLICKED(IDC_BUTTON9, &CUploadAppDlg::OnCostHelpClicked)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON2, &CUploadAppDlg::OnCalculatePolygons)
	ON_BN_CLICKED(IDC_BUTTON10, &CUploadAppDlg::OnClearFields)
	ON_BN_CLICKED(IDCANCEL, &CUploadAppDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON7, &CUploadAppDlg::OnBnClickedCopyObject)
	ON_BN_CLICKED(IDC_BUTTON3, &CUploadAppDlg::OnBrowseMusicSample)
	ON_BN_CLICKED(IDC_BUTTON11, &CUploadAppDlg::OnMusicSampleHelp)
END_MESSAGE_MAP()


// CUploadAppDlg message handlers

unsigned CUploadAppDlg::Run( )
{
	m_bRunning = true;
	DoModal( );
	m_bRunning = false;
	return 0;
}

void CUploadAppDlg::OnActivate( UINT nState, CWnd *pWndOther, BOOL bMinimized )
{
	if ( !m_hWnd ) return;

	if ( nState != WA_INACTIVE )
	{
		SetWindowPos( &wndBottom, 0,0, 0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
		SetWindowPos( &wndTop, 0,0, 0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
	}

	CDialog::OnActivate( nState, pWndOther, bMinimized );
}

BOOL CUploadAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if ( !theApp.m_szSESID )
	{
		MessageBox( GetLanguageData( _T("Uploader"), _T("MustLogin") ), theApp.m_szErrorString );
		OnCancel();
	}
	else 
	{
		if ( !theApp.m_bUploader ) 
		{
			MessageBox( GetLanguageData( _T("Uploader"), _T("Forbidden") ), theApp.m_szErrorString );
			OnCancel();
		}
	}

	m_labelModelDesc.SetWindowText		( GetLanguageData( _T("Uploader"), _T("ModelDesc") ) );
	m_labelPreviewDesc.SetWindowText	( GetLanguageData( _T("Uploader"), _T("PreviewDesc") ) );
	m_labelModel.SetWindowText			( GetLanguageData( _T("Uploader"), _T("ModelLabel") ) );
	m_labelPreview.SetWindowText		( GetLanguageData( _T("Uploader"), _T("PreviewLabel") ) );
	m_labelThumbnail.SetWindowText		( GetLanguageData( _T("Uploader"), _T("ThumbnailLabel") ) );
	m_labelName.SetWindowText			( GetLanguageData( _T("Uploader"), _T("NameLabel") ) );
	m_labelCompatible.SetWindowText		( GetLanguageData( _T("Uploader"), _T("CompatibleLabel") ) );
	m_labelType.SetWindowText			( GetLanguageData( _T("Uploader"), _T("TypeLabel") ) );
	m_labelCategory.SetWindowText		( GetLanguageData( _T("Uploader"), _T("CategoryLabel") ) );
	m_labelSubCategory.SetWindowText	( GetLanguageData( _T("Uploader"), _T("SubCategoryLabel") ) );
	m_labelCost.SetWindowText			( GetLanguageData( _T("Uploader"), _T("CostLabel") ) );
	m_labelPolygons.SetWindowText		( GetLanguageData( _T("Uploader"), _T("PolygonsLabel") ) );
	m_labelKeywords.SetWindowText		( GetLanguageData( _T("Uploader"), _T("KeywordsLabel") ) );
	m_labelDescription.SetWindowText	( GetLanguageData( _T("Uploader"), _T("DescriptionLabel") ) );
	m_labelVideoLink.SetWindowText		( GetLanguageData( _T("Uploader"), _T("VideoLabel") ) );
	m_btnCalculate.SetWindowText		( GetLanguageData( _T("Uploader"), _T("Calculate") ) );
	m_btnCopyFromObject.SetWindowText	( GetLanguageData( _T("Uploader"), _T("CopyObject") ) );
	m_btnUpload.SetWindowText			( GetLanguageData( _T("Uploader"), _T("Upload") ) );
	m_btnCancel.SetWindowText			( GetLanguageData( _T("Uploader"), _T("Cancel") ) );
	m_btnClearFields.SetWindowText		( GetLanguageData( _T("Uploader"), _T("ClearFields") ) );
	m_btnAddImage.SetWindowText			( GetLanguageData( _T("Uploader"), _T("Add") ) );
	m_btnRemoveImage.SetWindowText		( GetLanguageData( _T("Uploader"), _T("Remove") ) );
	m_labelMusicSample.SetWindowText	( GetLanguageData( _T("Uploader"), _T("MusicSampleLabel") ) );
	m_chkAgreement.SetWindowText		( GetLanguageData( _T("Uploader"), _T("UploadAgreement") ) );

	/*
	SetCurrentDirectory( theApp.m_szDirectory );
	FILE *pFile = NULL;
	int error = fopen_s( &pFile, "Author.ini", "rt" );
	if ( !error )
	{
		char str [ 32 ];
		fgets( str, 32, pFile );
		m_chkRememberAuthor.SetCheck( 1 );
		m_editAuthor.SetWindowText( str );
	}
	if ( pFile ) fclose( pFile );
	*/

	m_comboType.SetCurSel( 0 );
	
	//m_GroupList.Start( );

	SetTimer( 2, 1000, NULL );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUploadAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUploadAppDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUploadAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUploadAppDlg::OnBrowseModelFile()
{
	CFileDialog fileSelect( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "Zip Files (*.zip)|*.zip||" );
	int result = (int) fileSelect.DoModal();
	if ( result != IDOK ) return;

	CString sFullPath = fileSelect.GetPathName( );
	m_editModelFile.SetWindowText( sFullPath.GetString() );

	CString sItemName = "";
	if ( !CheckModelFileFormat( sFullPath, 0, sItemName ) ) 
	{
		m_editName.SetWindowText( "" );	
		return;
	}

	m_editName.SetWindowText( sItemName );
	
	CString sUploadFolder = theApp.m_szDirectory;
	sUploadFolder += "\\Files\\TGCStore\\Uploads\\";

	const char *szBaseName = strrchr( sFullPath.GetString(), '\\' );
	if ( !szBaseName ) szBaseName = sFullPath.GetString();
	else szBaseName++;

	//check thumbnail path
	m_sThumbnail = theApp.m_szDirectory;
	m_sThumbnail += "\\Files\\TGCStore\\Uploads\\";
	m_sThumbnail += szBaseName;
	m_sThumbnail += "\\entitybank\\";
	m_sThumbnail += sItemName;
	m_sThumbnail += ".bmp";

	FILE *fp = fopen( m_sThumbnail, "rb" );
	if ( fp ) 
	{
		fclose( fp );
		
		CImage image;
		HRESULT hr = image.Load( m_sThumbnail );

		if ( FAILED(hr) ) { MessageBox( "Failed to load thumbnail" ); return; }

		CWindowDC wndDC( this );
		CDC dc;	dc.CreateCompatibleDC( &wndDC );

		CBitmap bmpThumb;
		bmpThumb.CreateBitmap( 64,64, 1, 32, NULL );
		CBitmap *oldBmp = dc.SelectObject( &bmpThumb );
		image.BitBlt( dc.GetSafeHdc( ), 0,0 );
		dc.SelectObject( oldBmp );
		m_ImageThumbnail.CopyBitmap( &bmpThumb );
		image.Destroy();

		return;
	}

	//no entity thumbnail, try segment
	//MessageBox( GetLanguageData( "Uploader", "InvalidThumbnail" ), theApp.m_szErrorString );

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	SetCurrentDirectory( sUploadFolder );
	SetCurrentDirectory( szBaseName );

	m_sThumbnail = sItemName;
	m_sThumbnail += ".bmp";

	CString sFolderPath = "";
	if ( !FindSegmentThumbnail( "segments", m_sThumbnail, sFolderPath ) )
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		MessageBox( GetLanguageData( "Uploader", "InvalidThumbnail" ), theApp.m_szErrorString );
		return;
	}

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	sFolderPath += "\\";
	sFolderPath += m_sThumbnail;
	m_sThumbnail = sFolderPath;

	fp = NULL;
	fp = fopen( m_sThumbnail, "rb" );
	if ( !fp ) 
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		MessageBox( GetLanguageData( "Uploader", "InvalidThumbnail" ), theApp.m_szErrorString );
		return;
	}
	fclose( fp );

	CImage image;
	HRESULT hr = image.Load( m_sThumbnail );

	if ( FAILED(hr) ) { MessageBox( "Failed to load thumbnail" ); return; }

	CWindowDC wndDC( this );
	CDC dc;	dc.CreateCompatibleDC( &wndDC );

	CBitmap bmpThumb;
	bmpThumb.CreateBitmap( 64,64, 1, 32, NULL );
	CBitmap *oldBmp = dc.SelectObject( &bmpThumb );
	image.BitBlt( dc.GetSafeHdc( ), 0,0 );
	dc.SelectObject( oldBmp );
	m_ImageThumbnail.CopyBitmap( &bmpThumb );
	image.Destroy();
}

void CUploadAppDlg::OnBrowseThumbnail()
{
	CFileDialog fileSelect( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "Image Files (*.bmp, *.jpg, *.png, *.gif)|*.bmp;*.jpg;*.png;*.gif||" );
	int result = (int) fileSelect.DoModal();
	if ( result != IDOK ) return;

	CString sFullPath = fileSelect.GetPathName( );
//	m_editThumbnail.SetWindowText( sFullPath.GetString() );
}

void CUploadAppDlg::OnAddPreviewImage()
{
	CFileDialog fileSelect( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "Image Files (*.bmp, *.jpg, *.png, *.gif)|*.bmp;*.jpg;*.png;*.gif||" );
	int result = (int) fileSelect.DoModal();
	if ( result != IDOK ) return;

	CString sFullPath = fileSelect.GetPathName( );

	CImage image;
	HRESULT hr = image.Load( sFullPath );
	if ( FAILED( hr ) )
	{
		MessageBox( "Could not load image file, make sure it is one of: .bmp .jpg. png. gif", theApp.m_szErrorString );
		return;
	}

	if ( image.GetWidth() != 368 || image.GetHeight() != 368 )
	{
		MessageBox( "Preview image must be 368x368 in size", theApp.m_szErrorString );
		return;
	}

	m_comboPreviewImages.AddString( sFullPath.GetString() );
	m_comboPreviewImages.ShowDropDown();
}

void CUploadAppDlg::OnRemovePreviewImage()
{
	int index = m_comboPreviewImages.GetCurSel();
	if ( index == CB_ERR )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("SelectPreview") ), theApp.m_szErrorString );
		return;
	}

	m_comboPreviewImages.DeleteString( index );
}

void CUploadAppDlg::OnAddKeyword()
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

void CUploadAppDlg::OnRemoveKeyword()
{
	int index = m_comboKeywords.GetCurSel();
	if ( index == CB_ERR )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("SelectKeyword") ), theApp.m_szErrorString );
		return;
	}

	m_comboKeywords.DeleteString( index );
}

void CUploadAppDlg::ClearUploadFolder( const char *szFolderName )
{
	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char oldDir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, oldDir );

	if ( !szFolderName || !SetCurrentDirectory( szFolderName ) ) 
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
				//if ( (FindFileData.dwFileAttributes & (FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN) ) ) continue;
				if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					ClearUploadFolder( FindFileData.cFileName );
					RemoveDirectory( FindFileData.cFileName );
				}
				else
				{
					DeleteFile( FindFileData.cFileName );
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);

		FindClose(hFind);
	}

	SetCurrentDirectory( oldDir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
}

void CUploadAppDlg::StrKeepSimilar( char *szStr1, const char *szStr2 )
{
	if ( !szStr1 || !szStr2 ) return;

	int i = 0;
	while ( *(szStr1+i) && *(szStr1+i) == *(szStr2+i) ) i++;
	*(szStr1+i) = '\0';
}

bool CUploadAppDlg::FindSegmentThumbnail( const char *szFolder, const char *szThumbnail, CString &sFolderPath )
{
	char oldDir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, oldDir );

	if ( !SetCurrentDirectory( szFolder ) ) return false;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
		
	hFind = FindFirstFile("*.*", &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
			{
				if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					if ( FindSegmentThumbnail( FindFileData.cFileName, szThumbnail, sFolderPath ) )
					{
						return true;
					}
				}
				else
				{
					if ( strcmp( FindFileData.cFileName, szThumbnail ) == 0 )
					{
						sFolderPath = oldDir;
						if ( oldDir [ strlen(oldDir)-1 ] != '\\' ) sFolderPath += "\\";
						sFolderPath += szFolder;
						return true;
					}
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);

		FindClose(hFind);
	}

	SetCurrentDirectory( oldDir );

	return false;
}

bool CUploadAppDlg::SearchSegmentDirectory( const char *szFolder, CString &sNewName )
{
	char oldDir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, oldDir );

	if ( !SetCurrentDirectory( szFolder ) ) return false;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	char szNewModelName [ 256 ];
	strcpy_s( szNewModelName, 256, "" );

	bool bFound = false;
		
	hFind = FindFirstFile("*.*", &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
			{
				if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					CString sFoundName = "";
					if ( SearchSegmentDirectory( FindFileData.cFileName, sFoundName ) )
					{
						if ( strlen( szNewModelName ) > 0 ) StrKeepSimilar( szNewModelName, sFoundName );
						else strcpy_s( szNewModelName, 256, sFoundName );

						bFound = true;
					}
				}
				else
				{
					const char *szExt = strrchr( FindFileData.cFileName, '.' );
					if ( strcmp( szExt, ".fps" ) == 0 )
					{
						if ( strlen( szNewModelName ) > 0 ) StrKeepSimilar( szNewModelName, FindFileData.cFileName );
						else strncpy_s( szNewModelName, 256, FindFileData.cFileName, (DWORD) (szExt - FindFileData.cFileName) );
						//::MessageBox( NULL, szNewModelName, "Info", 0 );

						bFound = true;
					}
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);

		FindClose(hFind);
	}

	sNewName = szNewModelName;

	SetCurrentDirectory( oldDir );

	return bFound;
}

bool CUploadAppDlg::VerifyModelFolders( int iLevel, int iFolderType )
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	char oldDir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, oldDir );

	hFind = FindFirstFile("*.*", &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
			{
				if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					if ( iLevel == 0 )
					{
						//check root folders are valid
						if ( strcmp( FindFileData.cFileName, "audiobank" ) 
						  || strcmp( FindFileData.cFileName, "effectbank" ) 
						  || strcmp( FindFileData.cFileName, "meshbank" ) 
						  || strcmp( FindFileData.cFileName, "scriptbank" ) 
						  || strcmp( FindFileData.cFileName, "texturebank" )
						  || strcmp( FindFileData.cFileName, "videobank" ) )
						{
							SetCurrentDirectory( FindFileData.cFileName );
							if ( !VerifyModelFolders( iLevel + 1, 1 ) ) 
							{
								FindClose(hFind);
								return false;
							}
							SetCurrentDirectory( oldDir );
						}
						else if ( strcmp( FindFileData.cFileName, "entitybank" )
						       || strcmp( FindFileData.cFileName, "segments" ) )
						{
							SetCurrentDirectory( FindFileData.cFileName );
							if ( !VerifyModelFolders( iLevel + 1, 2 ) ) 
							{
								FindClose(hFind);
								return false;
							}
							SetCurrentDirectory( oldDir );
						} 
						else if ( strcmp( FindFileData.cFileName, "gamecore" ) )
						{
							SetCurrentDirectory( FindFileData.cFileName );
							if ( !VerifyModelFolders( iLevel + 1, 3 ) ) 
							{
								FindClose(hFind);
								return false;
							}
							SetCurrentDirectory( oldDir );
						}
						else
						{
							SetCurrentDirectory( FindFileData.cFileName );
							if ( !VerifyModelFolders( iLevel + 1, 4 ) ) 
							{
								FindClose(hFind);
								return false;
							}
							SetCurrentDirectory( oldDir );
						}
					}
					else if ( iLevel == 1 )
					{
						//check for artist folder?
						SetCurrentDirectory( FindFileData.cFileName );
						if ( !VerifyModelFolders( iLevel + 1, 5 ) ) 
						{
							FindClose(hFind);
							return false;
						}
						SetCurrentDirectory( oldDir );
					}
					else
					{
						//sub sub folders, anything goes
						SetCurrentDirectory( FindFileData.cFileName );
						if ( !VerifyModelFolders( iLevel + 1, 5 ) ) 
						{
							FindClose(hFind);
							return false;
						}
						SetCurrentDirectory( oldDir );
					}
				}
				else
				{

					const char *szExt = strrchr( FindFileData.cFileName, '.' );
					char lszExt [ 10 ];
					strcpy_s( lszExt, 10, szExt );
					strlower( lszExt );

					//restricted extensions
					if ( strcmp( lszExt, ".ade" ) == 0 || strcmp( lszExt, ".adp" ) == 0 || strcmp( lszExt, ".bas" ) == 0 
					  || strcmp( lszExt, ".bat" ) == 0 || strcmp( lszExt, ".chm" ) == 0 || strcmp( lszExt, ".cmd" ) == 0 
					  || strcmp( lszExt, ".com" ) == 0 || strcmp( lszExt, ".cpl" ) == 0 || strcmp( lszExt, ".crt" ) == 0 
					  || strcmp( lszExt, ".exe" ) == 0 || strcmp( lszExt, ".hlp" ) == 0 || strcmp( lszExt, ".hta" ) == 0 
					  || strcmp( lszExt, ".inf" ) == 0 || strcmp( lszExt, ".ins" ) == 0 || strcmp( lszExt, ".isp" ) == 0 
					  || strcmp( lszExt, ".jar" ) == 0 || strcmp( lszExt, ".js" ) == 0 || strcmp( lszExt, ".jse" ) == 0 
					  || strcmp( lszExt, ".lnk" ) == 0 || strcmp( lszExt, ".mdb" ) == 0 || strcmp( lszExt, ".mde" ) == 0 
					  || strcmp( lszExt, ".msc" ) == 0 || strcmp( lszExt, ".msi" ) == 0 || strcmp( lszExt, ".msp" ) == 0 
					  || strcmp( lszExt, ".mst" ) == 0 || strcmp( lszExt, ".pcd" ) == 0 || strcmp( lszExt, ".pif" ) == 0 
					  || strcmp( lszExt, ".reg" ) == 0 || strcmp( lszExt, ".scr" ) == 0 || strcmp( lszExt, ".sct" ) == 0 
					  || strcmp( lszExt, ".shs" ) == 0 || strcmp( lszExt, ".url" ) == 0 || strcmp( lszExt, ".vb" ) == 0 
					  || strcmp( lszExt, ".vbe" ) == 0 || strcmp( lszExt, ".vbs" ) == 0 || strcmp( lszExt, ".ws" ) == 0 
					  || strcmp( lszExt, ".wsc" ) == 0 || strcmp( lszExt, ".wsf" ) == 0 || strcmp( lszExt, ".wsh" ) == 0 )
					{
						char str [ 256 ];
						sprintf_s( str, 256, "The model zip file contains a restricted filetype: \"%s\" is not allowed", szExt );
						::MessageBox( NULL, str, theApp.m_szErrorString, 0 );
						FindClose(hFind);
						return false;
					}
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);

		FindClose(hFind);
	}

	return true;
}

bool CUploadAppDlg::CheckModelFileFormat( const char *szModelFile, int iType, CString &sNewName )
{
	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !SetCurrentDirectory( theApp.m_szDirectory ) ) ::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidMainPath" ), theApp.m_szErrorString, 0 );
	if ( !SetCurrentDirectory( "Files\\TGCStore" ) ) ::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidStorePath" ), theApp.m_szErrorString, 0 );
	if ( !SetCurrentDirectory( "Uploads" ) )
	{
		CreateDirectory( "Uploads", NULL );
		if ( !SetCurrentDirectory( "Uploads" ) ) 
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidUploadPath" ), theApp.m_szErrorString, 0 );
			return false;
		}
	}

	CString sUploadFolder = theApp.m_szDirectory;
	sUploadFolder += "\\Files\\TGCStore\\Uploads\\";

	const char *szBaseName = strrchr( szModelFile, '\\' );
	if ( !szBaseName ) szBaseName = szModelFile;
	else szBaseName++;

	ClearUploadFolder( szBaseName );

	if ( !SetCurrentDirectory( szBaseName ) )
	{
		CreateDirectory( szBaseName, NULL );
		if ( !SetCurrentDirectory( szBaseName ) ) 
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidUploadPath" ), theApp.m_szErrorString, 0 );
			return false;
		}
	}

	ZIPENTRY details;
	HZIP zpModel = OpenZip( szModelFile, 0 );
	
	GetZipItem( zpModel, -1, &details );
	for ( int i = 0; i < details.index; i++ )
	{
		ZIPENTRY ze;
		GetZipItem( zpModel, i, &ze );

		const char *szFile = strrchr( ze.name, '/' );	//must contain a slash
		if ( !szFile ) szFile = strrchr( ze.name, '\\' );
		if ( !szFile ) 
		{
			CloseZip( zpModel );
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			::MessageBox( NULL, GetLanguageData( "Uploader", "NoRootFiles" ), theApp.m_szErrorString, 0 );
			return false;
		}

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
			CloseZip( zpModel );
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidZipDirectory" ), theApp.m_szErrorString, 0 );
			return false;
		}

		UnzipItem( zpModel, i, ze.name );
	}

	CloseZip( zpModel );

	bool bEntity = false;
	CString sNewModelName = "";

	if ( !VerifyModelFolders( 0, 0 ) ) 
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return false;
	}

	if ( SetCurrentDirectory( "segments" ) )
	{
		if ( iType == 1 )
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			::MessageBox( NULL, GetLanguageData( "Uploader", "TypeMismatch" ), theApp.m_szErrorString, 0 );
			return false;
		}

		//find segment name
		SetCurrentDirectory( sUploadFolder );
		SetCurrentDirectory( szBaseName );
		SearchSegmentDirectory( "segments", sNewModelName );
	}
	else
	{
		if ( iType == 2 )
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			::MessageBox( NULL, GetLanguageData( "Uploader", "TypeMismatch" ), theApp.m_szErrorString, 0 );
			return false;
		}

		bEntity = true;
		if ( !SetCurrentDirectory( "entitybank" ) )
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidModelFile" ), theApp.m_szErrorString, 0 );
			return false;
		}

		//find entity name
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
			
		hFind = FindFirstFile("*.*", &FindFileData);

		char szNewModelName [ MAX_PATH ];
		strcpy_s( szNewModelName, MAX_PATH, "" );

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
				{
					if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
					{
						if ( bEntity )
						{
							LeaveCriticalSection( &theApp.m_csDirectoryChanges );
							::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidStructure" ), theApp.m_szErrorString, 0 );
							return false;
						}
					}
					else
					{
						const char *szExt = strrchr( FindFileData.cFileName, '.' );
						if ( strcmp( szExt, ".fpe" ) == 0 )
						{
							//if ( sNewModelName.GetLength() > 0 )
							if ( strlen(szNewModelName) > 0 ) StrKeepSimilar( szNewModelName, FindFileData.cFileName );
							else strncpy_s( szNewModelName, MAX_PATH, FindFileData.cFileName, (DWORD) (szExt - FindFileData.cFileName) );
							
							/*
							{
								LeaveCriticalSection( &theApp.m_csDirectoryChanges );
								::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidName" ), theApp.m_szErrorString, 0 );
								return false;
							}
							*/
							
							//MessageBox( sNewModelName );
						}
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);

			FindClose(hFind);
		}

		sNewModelName = szNewModelName;
	}

	sNewModelName.Trim( "-_ " );

	if ( sNewModelName.GetLength() < 1 )
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		if ( bEntity ) ::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidName" ), theApp.m_szErrorString, 0 );
		else ::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidSegmentName" ), theApp.m_szErrorString, 0 );
		return false;
	}

	if ( sNewModelName.GetLength( ) < 2 )
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		::MessageBox( NULL, GetLanguageData( "Uploader", "InvalidFileName" ), theApp.m_szErrorString, 0 );
		return false;
	}

	sNewName = sNewModelName;

	SetCurrentDirectory( theApp.m_szDirectory );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	return true;
}

void CUploadAppDlg::OnUpload()
{
	if ( m_UploadThread.IsUploading( ) )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("StillUploading") ) );
		return;
	}

	if ( !theApp.m_szSESID )
	{
		MessageBox( GetLanguageData( _T("Uploader"), _T("MustLogin") ), theApp.m_szErrorString );
		OnCancel();
	}
	else
	{
		if ( !theApp.m_bUploader ) 
		{
			MessageBox( GetLanguageData( _T("Uploader"), _T("Forbidden") ), theApp.m_szErrorString );
			OnCancel();
		}
	}

	if ( !m_chkAgreement.GetCheck() )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("AgreeTerms") ), theApp.m_szErrorString );
		return;
	}

//	m_editName.GetWindowText( m_sName );
	m_editCost.GetWindowText( m_sCost );
	m_comboType.GetWindowText( m_sType );
	m_comboCategory.GetWindowText( m_sCategory );
	m_editSubCategory.GetWindowText( m_sSubCategory );
	m_editPolygons.GetWindowText( m_sPolygons );
	m_editDescription.GetWindowText( m_sDescription );
	m_editVideoLink.GetWindowText( m_sVideoLink );

	if ( m_chkX9Compatible.GetCheck() == 0 && m_chkX10Compatible.GetCheck() == 0 )
	{
		MessageBox(  GetLanguageData( _T("Uploader"), _T("Compatibility") ), theApp.m_szErrorString );
		return;
	}

	CString sMsg;

	if ( m_sCost.GetLength() < 1 ) m_sCost = "0"; //optional
	if ( m_sPolygons.GetLength() < 1 ) m_sPolygons = "0"; //optional
	if ( m_sCategory.GetLength() < 1 ) 
	{ 
		sMsg.Format( "'%s' %s",  GetLanguageData( _T("Uploader"), _T("Category") ),  GetLanguageData( _T("Uploader"), _T("MustExist") ) ); 
		MessageBox( sMsg, theApp.m_szErrorString ); 
		return; 
	}
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

	if ( m_sType.Compare( "Entity" ) != 0 && m_sType.Compare( "Segment" ) != 0 && m_sType.Compare( "Music" ) != 0 ) 
	{
		MessageBox( GetLanguageData( _T("Uploader"), _T("InvalidType") ), theApp.m_szErrorString );
		return;
	}
	
	//if ( !CheckStringAlNum2( GetLanguageData( _T("Uploader"), _T("Category") ), m_sCategory ) ) return;
	//if ( !CheckStringAlNum2( GetLanguageData( _T("Uploader"), _T("SubCategory") ), m_sSubCategory ) ) return;
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
	m_sThumbnail = "";

	if ( m_sModelFile.GetLength( ) < 1 ) 
	{
		MessageBox( GetLanguageData( _T("Uploader"), _T("InvalidModel") ), theApp.m_szErrorString );
		return;
	}

	int iType = 0;
	if ( m_sType.Compare( "Entity" ) == 0 ) iType = 1;
	else if ( m_sType.Compare( "Segment" ) == 0 ) iType = 2;
	else if ( m_sType.Compare( "Music" ) == 0 ) iType = 3;

	CString sNewModelName = "";

	if ( !CheckModelFileFormat( m_sModelFile, iType, sNewModelName ) ) return;

	CString sUploadFolder = theApp.m_szDirectory;
	sUploadFolder += "\\Files\\TGCStore\\Uploads\\";

	const char *szBaseName = strrchr( m_sModelFile.GetString(), '\\' );
	if ( !szBaseName ) szBaseName = m_sModelFile.GetString();
	else szBaseName++;

	CString sMusicSampleFile = "";

	//check entity
	if ( iType == 1 || iType == 3 )
	{
		//check thumbnail path
		m_sThumbnail = theApp.m_szDirectory;
		m_sThumbnail += "\\Files\\TGCStore\\Uploads\\";
		m_sThumbnail += szBaseName;
		m_sThumbnail += "\\entitybank\\";
		m_sThumbnail += sNewModelName;
		m_sThumbnail += ".bmp";

		FILE *fp = fopen( m_sThumbnail, "rb" );
		if ( !fp ) 
		{
			MessageBox( GetLanguageData( "Uploader", "InvalidThumbnail" ), theApp.m_szErrorString );
			return;
		}
		fclose( fp );

		if ( iType == 3 )
		{
			EnterCriticalSection( &theApp.m_csDirectoryChanges );

			SetCurrentDirectory( sUploadFolder );
			SetCurrentDirectory( szBaseName );
			if ( !SetCurrentDirectory( "audiobank" ) )
			{
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );
				MessageBox( GetLanguageData( "Uploader", "NoAudiobank" ), theApp.m_szErrorString );
				return;
			}

			LeaveCriticalSection( &theApp.m_csDirectoryChanges );

			m_editMusicSample.GetWindowText( sMusicSampleFile );
			const char *szExt = strrchr( sMusicSampleFile, '.' );
			if ( !szExt || strcmp( szExt, ".ogg" ) != 0 )
			{
				MessageBox( GetLanguageData( "Uploader", "IncorrectSampleFormat" ), theApp.m_szErrorString );
				return;
			}
		}
	}
	
	if ( iType == 2 )
	{
		EnterCriticalSection( &theApp.m_csDirectoryChanges );

		SetCurrentDirectory( sUploadFolder );
		SetCurrentDirectory( szBaseName );

		m_sThumbnail = sNewModelName;
		m_sThumbnail += ".bmp";

		CString sFolderPath = "";
		if ( !FindSegmentThumbnail( "segments", m_sThumbnail, sFolderPath ) )
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			MessageBox( GetLanguageData( "Uploader", "InvalidThumbnail" ), theApp.m_szErrorString );
			return;
		}

		LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		sFolderPath += "\\";
		sFolderPath += m_sThumbnail;
		m_sThumbnail = sFolderPath;

		FILE *fp = fopen( m_sThumbnail, "rb" );
		if ( !fp ) 
		{
			//LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			MessageBox( GetLanguageData( "Uploader", "InvalidThumbnail" ), theApp.m_szErrorString );
			return;
		}
		fclose( fp );
	}
	
	m_iNumImages = m_comboPreviewImages.GetCount( );
	if ( m_psPreviewImages ) delete [] m_psPreviewImages;
	m_psPreviewImages = NULL;
	
	if ( iType == 1 || iType == 2 )
	{
		if ( m_iNumImages > 0 )
		{
			m_psPreviewImages = new CString [ m_iNumImages ];
			for ( int i = 0 ; i < m_iNumImages; i++ )
			{
				m_comboPreviewImages.GetLBText( i, m_psPreviewImages [ i ] );
			}
		}
		else
		{
			int result = MessageBox( GetLanguageData( _T("Uploader"), _T("ConfirmPreview") ), GetLanguageData( _T("Uploader"), _T("Confirm") ), MB_YESNO );
			if ( result != IDYES ) return;
		}
	}

	if ( theApp.m_szSESID )
	{
		if ( m_UploadThread.IsUploading( ) ) return;
		m_UploadThread.SetValues( &m_ProgressBar, sNewModelName, m_sCost, m_sType, m_sCategory, m_sSubCategory, m_sPolygons, m_sKeywords, m_sDescription, m_sVideoLink, m_dwGroupSelected, m_chkX9Compatible.GetCheck(), m_chkX10Compatible.GetCheck() );
		m_UploadThread.SetFilenames( m_sModelFile, m_sThumbnail, m_iNumImages, m_psPreviewImages, sMusicSampleFile );
		m_UploadThread.Start( );
		
		m_ProgressSpinner.StartSpinning( );
		m_ProgressSpinner.ShowWindow( SW_SHOW );
		m_ProgressBar.SetPos( 0 );
		m_ProgressBar.ShowWindow( SW_SHOW );
		m_textUploading.SetWindowText( "Uploading:" );
		m_textUploading.ShowWindow( SW_SHOW );

		SetTimer( 1, 1000, NULL );
	}
}

void CUploadAppDlg::SetProgress( int iProgress )
{
	if ( !m_ProgressBar.GetSafeHwnd() ) return;

	m_ProgressBar.SetPos( iProgress );
}

void CUploadAppDlg::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		if ( !m_UploadThread.IsUploading( ) )
		{
			KillTimer( 1 );
			m_ProgressSpinner.StopSpinning();
			m_ProgressSpinner.ShowWindow( SW_HIDE );
			m_ProgressBar.ShowWindow( SW_HIDE );
			if ( m_UploadThread.IsSuccessful() ) m_textUploading.SetWindowText( "Upload Finished" );
			else m_textUploading.SetWindowText( "Upload Failed" );

			EnterCriticalSection( &theApp.m_csDirectoryChanges );

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

			LeaveCriticalSection( &theApp.m_csDirectoryChanges );

			if ( m_pMarket && m_UploadThread.IsSuccessful() ) m_pMarket->UploadComplete( m_UploadThread.GetObjID() );
		}
	}

	if ( nIDEvent == 2 )
	{
		KillTimer( 2 );
		/*
		HTTPComm server;
		server.GetGroupList( "Temp\\GroupList.xml" );
		MarketGroup::LoadObjects( "Temp\\GroupList.xml" );
		m_GroupList.UpdateStoreCategories( );
		*/
	}

	if ( nIDEvent == 99 )
	{
		if ( !m_UploadThread.IsUploading() )
		{
			CDialog::OnCancel();
		}
	}
}

void CUploadAppDlg::OnRememberAuthorClicked()
{
	/*
	SetCurrentDirectory( theApp.m_szDirectory );
	DeleteFile( "Author.ini" );
	
	if ( m_chkRememberAuthor.GetCheck() )
	{
		CString sAuthor;
		m_editAuthor.GetWindowText( sAuthor );

		HANDLE hFile = CreateFile( "Author.ini", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		
		DWORD dwNumWritten = 0;
		WriteFile( hFile, sAuthor, sAuthor.GetLength(), &dwNumWritten, NULL );
		CloseHandle( hFile );
		/ *
		FILE *pFile = NULL;
		int error = fopen_s( &pFile, "Author.ini", "at+" );
		if ( error )
		{
			char str [ 64 ];
			sprintf_s( str, 64, "Failed to create file, error: %d", error );
			MessageBox( str, "Error" );
			m_chkRememberAuthor.SetCheck( 0 );
			if ( pFile ) fclose( pFile );
			return;
		}

				fputs( sAuthor, pFile );
		fclose( pFile ); pFile = NULL;
		* /
	}
*/
}

void CUploadAppDlg::OnModelHelpClicked()
{
	CString sMsg = GetLanguageData( _T("Uploader"), _T("ModelHelp1") );
	sMsg += "\r\n\r\n-";
	sMsg += GetLanguageData( _T("Uploader"), _T("ModelHelp2") );
	sMsg += "\r\n-";
	sMsg += GetLanguageData( _T("Uploader"), _T("ModelHelp3") );
	sMsg += "\r\n-";
	sMsg += GetLanguageData( _T("Uploader"), _T("ModelHelp4") );
	sMsg += "\r\n\r\n";
	sMsg += GetLanguageData( _T("Uploader"), _T("ModelHelp5") );
	sMsg += "\r\n\r\n";
	sMsg += GetLanguageData( _T("Uploader"), _T("ModelHelp6") );

	MessageBox( sMsg );
}

void CUploadAppDlg::OnTypeChanged()
{
	CString sType;
	m_comboType.GetWindowText( sType );
	
	if ( sType.Compare( "Entity" ) == 0 )
	{
		while ( m_comboCategory.GetCount() )
		{
			m_comboCategory.DeleteString( 0 );
		}

		m_comboCategory.AddString( "Characters" );
        m_comboCategory.AddString( "Weapons" );
        m_comboCategory.AddString( "Items" );
		m_comboCategory.AddString( "Indoor" );
        m_comboCategory.AddString( "Outdoor" );
        m_comboCategory.AddString( "Overlay" );
        m_comboCategory.AddString( "Poster" );

		m_comboCategory.SetCurSel( 0 );

		m_editPolygons.EnableWindow( TRUE );
	}
	else
	{
		if ( sType.Compare( "Segment" ) == 0 )
		{
			while ( m_comboCategory.GetCount() )
			{
				m_comboCategory.DeleteString( 0 );
			}

			m_comboCategory.AddString( "Room" );
			m_comboCategory.AddString( "Corridors" );
			m_comboCategory.AddString( "Platforms" );
			m_comboCategory.AddString( "Scenery" );

			m_comboCategory.SetCurSel( 0 );
		}
		else if ( sType.Compare( "Music" ) == 0 )
		{
			while ( m_comboCategory.GetCount() )
			{
				m_comboCategory.DeleteString( 0 );
			}

			m_comboCategory.AddString( "Music" );
			m_comboCategory.SetCurSel( 0 );
		}


		m_editPolygons.EnableWindow( FALSE );
	}

	if ( sType.Compare( "Music" ) == 0 )
	{
		m_editMusicSample.EnableWindow( TRUE );
		m_btnBrowseMusic.EnableWindow( TRUE );

		m_comboPreviewImages.EnableWindow( FALSE );
		m_btnAddImage.EnableWindow( FALSE );
		m_btnRemoveImage.EnableWindow( FALSE );
	}
	else
	{
		m_editMusicSample.EnableWindow( FALSE );
		m_btnBrowseMusic.EnableWindow( FALSE );

		m_comboPreviewImages.EnableWindow( TRUE );
		m_btnAddImage.EnableWindow( TRUE );
		m_btnRemoveImage.EnableWindow( TRUE );
	}	
}

void CUploadAppDlg::OnCostHelpClicked()
{
	CString sMsg = GetLanguageData( _T("Uploader"), _T("CostHelp1") );
	sMsg += "\r\n";
	sMsg += GetLanguageData( _T("Uploader"), _T("CostHelp2") );
	sMsg += "\r\n\r\n-";
	sMsg += GetLanguageData( _T("Uploader"), _T("CostHelp3") );
	sMsg += "\r\n-";
	sMsg += GetLanguageData( _T("Uploader"), _T("CostHelp4") );
	sMsg += "\r\n-";
	sMsg += GetLanguageData( _T("Uploader"), _T("CostHelp5") );
	sMsg += "\r\n-";
	sMsg += GetLanguageData( _T("Uploader"), _T("CostHelp6") );

	MessageBox( sMsg );
}

void CUploadAppDlg::OnCalculatePolygons()
{
	CString sType = "";
	m_comboType.GetWindowText( sType );
	
	if ( strcmp( sType, "Music" ) == 0 )
	{
		
	}
	else
	{
		m_editModelFile.GetWindowText( m_sModelFile );
		if ( m_sModelFile.GetLength( ) < 1 ) 
		{
			MessageBox( GetLanguageData( _T("Uploader"), _T("InvalidModel") ), theApp.m_szErrorString );
			return;
		}

		EnterCriticalSection( &theApp.m_csDirectoryChanges );

		if ( !SetCurrentDirectory( theApp.m_szDirectory ) ) MessageBox( GetLanguageData( "TGC Store", "InvalidMainPath" ) );
		if ( !SetCurrentDirectory( "Files\\TGCStore" ) ) MessageBox( GetLanguageData( "TGC Store", "InvalidStorePath" ) );
		if ( !SetCurrentDirectory( "Uploads" ) )
		{
			CreateDirectory( "Uploads", NULL );
			if ( !SetCurrentDirectory( "Uploads" ) ) 
			{
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );
				MessageBox( GetLanguageData( "TGC Store", "InvalidUploadPath" ) );
				return;
			}
		}

		const char *szBaseName = strrchr( m_sModelFile.GetString(), '\\' );
		if ( !szBaseName ) szBaseName = m_sModelFile.GetString();
		else szBaseName++;

		ClearUploadFolder( szBaseName );

		if ( !SetCurrentDirectory( szBaseName ) )
		{
			CreateDirectory( szBaseName, NULL );
			if ( !SetCurrentDirectory( szBaseName ) ) 
			{
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );
				MessageBox( GetLanguageData( "TGC Store", "InvalidUploadPath" ) );
				return;
			}
		}

		ZIPENTRY details;
		HZIP zpModel = OpenZip( m_sModelFile, 0 );
		
		GetZipItem( zpModel, -1, &details );
		for ( int i = 0; i < details.index; i++ )
		{
			ZIPENTRY ze;
			GetZipItem( zpModel, i, &ze );
			UnzipItem( zpModel, i, ze.name );
		}

		CloseZip( zpModel );

		bool bEntity = true;

		if ( !SetCurrentDirectory( "entitybank" ) )
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			MessageBox( GetLanguageData( "Uploader", "InvalidModelFile" ), theApp.m_szErrorString );
			return;
		}

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;

		CString sNewModelName = "";
			
		hFind = FindFirstFile("*.*", &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				if ( _tcscmp( FindFileData.cFileName, _T(".") ) && _tcscmp( FindFileData.cFileName, _T("..") ) )
				{
					if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
					{
						const char *szExt = strrchr( FindFileData.cFileName, '.' );
						if ( strcmp( szExt, ".x" ) == 0 || strcmp( szExt, ".X" ) == 0 )
						{
							if ( sNewModelName.GetLength() > 0 )
							{
								LeaveCriticalSection( &theApp.m_csDirectoryChanges );
								MessageBox( GetLanguageData( "Uploader", "InvalidName" ), theApp.m_szErrorString );
								return;
							}
							sNewModelName = FindFileData.cFileName;
							//MessageBox( sNewModelName );
						}
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);

			FindClose(hFind);
		}

		if ( sNewModelName.GetLength() < 1 )
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			MessageBox( GetLanguageData( "Uploader", "InvalidName" ), theApp.m_szErrorString );
			return;
		}

		CString sPolyCountFile = theApp.m_szDirectory;
		sPolyCountFile += "\\Files\\TGCStore\\TEMP\\PolyCount.txt";

		CString sFullModelPath = theApp.m_szDirectory;
		sFullModelPath += "\\Files\\TGCStore\\Uploads\\";
		sFullModelPath += szBaseName;
		sFullModelPath += "\\entitybank\\";
		sFullModelPath += sNewModelName;

		CString sArg = "";
		sArg += sPolyCountFile;
		sArg += "=";
		sArg += sFullModelPath;

		CString sAppPath = theApp.m_szDirectory;
		sAppPath += "\\CountPolygons.exe";

		SHELLEXECUTEINFO info = {0};
		info.cbSize = sizeof(SHELLEXECUTEINFO);
		info.lpVerb = "open";
		info.lpFile = sAppPath;
		info.lpParameters = sArg;
		info.fMask = SEE_MASK_NOCLOSEPROCESS;

		//ShellExecute( NULL, "open", sAppPath, sArg, NULL, 0 );
		ShellExecuteEx( &info );
		if ( info.hProcess )
		{
			WaitForSingleObject( info.hProcess, 10000 );
			CloseHandle( info.hProcess );
		}

		FILE *pFile = NULL;
		fopen_s( &pFile, sPolyCountFile, "r" );
		if ( !pFile )
		{
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			MessageBox( GetLanguageData( _T("Uploader"), _T("CountFailed") ), theApp.m_szErrorString );
			return;
		}

		LeaveCriticalSection( &theApp.m_csDirectoryChanges );

		char szPolys [ 32 ];
		fgets( szPolys, 32, pFile );
		DWORD dwPolys = atoi( szPolys );
		sprintf_s( szPolys, 32, "%d", dwPolys );
		m_editPolygons.SetWindowText( szPolys );
		fclose( pFile );
	}
}

void CUploadAppDlg::OnClearFields()
{
	m_editModelFile.SetWindowText( "" );
//	m_editThumbnail.SetWindowText( "" );
	//m_editName.SetWindowText( "" );
	m_editCost.SetWindowText( "" );
	m_editPolygons.SetWindowText( "" );
	m_editDescription.SetWindowText( "" );
	m_editSubCategory.SetWindowText( "" );
	while( m_comboPreviewImages.GetCount() > 0 ) m_comboPreviewImages.DeleteString( 0 );
	while( m_comboKeywords.GetCount() > 0 ) m_comboKeywords.DeleteString( 0 );
	m_comboType.SetCurSel( 0 );
	m_comboKeywords.SetWindowText( "" );
	m_chkX9Compatible.SetCheck( 0 );
	m_chkX10Compatible.SetCheck( 0 );
	m_chkAgreement.SetCheck( 0 );
	OnTypeChanged();
}

void CUploadAppDlg::OnBnClickedCancel()
{
	bool bWait = false;

	if ( m_UploadThread.IsUploading() )
	{
		m_textUploading.SetWindowText( "Stopping..." );
		m_UploadThread.Cancel();
		//m_UploadThread.Join();
		bWait = true;
	}

	if ( bWait ) SetTimer( 99, 1000, NULL );
	else CDialog::OnCancel();
}

void CUploadAppDlg::OnBnClickedCopyObject()
{
	if ( !m_pMarket ) return;

	MarketObject *pObject = m_pMarket->GetSelectedObject();
	if ( !pObject ) return;

	MarketObject *pLocalObject = new MarketObject;
	pLocalObject->Copy( pObject );
	pObject = NULL;

	HTTPComm server;
	if ( !server.GetModelDetails( pLocalObject->GetID(), theApp.m_szSESID ) )
	{
		//MessageBox( server.GetLastError(), "Server Error" );
		return;
	}
	pLocalObject->CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );

	m_chkX9Compatible.SetCheck( pLocalObject->IsX9Compatible() ? 1 : 0 );
	m_chkX10Compatible.SetCheck( pLocalObject->IsX10Compatible() ? 1 : 0 );
	m_editSubCategory.SetWindowText( pLocalObject->GetSubCategory() );
	m_editDescription.SetWindowText( pLocalObject->GetDescription() );

	CString sText;
	for ( int i = 0; i < m_comboType.GetCount(); i++ )
	{
		m_comboType.GetLBText( i, sText );
		if ( sText.CompareNoCase( pLocalObject->GetType() ) == 0 )
		{
			m_comboType.SetCurSel( i );
			OnTypeChanged();
			break;
		}
	}

	for ( int i = 0; i < m_comboCategory.GetCount(); i++ )
	{
		m_comboCategory.GetLBText( i, sText );
		if ( sText.CompareNoCase( pLocalObject->GetCategory() ) == 0 )
		{
			m_comboCategory.SetCurSel( i );
			break;
		}
	}

	char str [ 12 ];
	sprintf_s( str, 12, "%d", pLocalObject->GetCost( ) );
	m_editCost.SetWindowText( str );

	while ( m_comboKeywords.GetCount() ) m_comboKeywords.DeleteString( 0 );
	
	for ( DWORD i = 0; i < pLocalObject->GetNumKeywords(); i++ )
	{
		m_comboKeywords.AddString( pLocalObject->GetKeyword( i ) );
	}

	delete pLocalObject;
}

void CUploadAppDlg::OnBrowseMusicSample()
{
	CFileDialog fileSelect( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "OGG Files (*.ogg)|*.ogg||" );
	int result = (int) fileSelect.DoModal();
	if ( result != IDOK ) return;

	CString sFullPath = fileSelect.GetPathName( );
	m_editMusicSample.SetWindowText( sFullPath.GetString() );
}

void CUploadAppDlg::OnMusicSampleHelp()
{
	MessageBox( GetLanguageData( _T("Uploader"), _T("MusicHelp") ) );
}
