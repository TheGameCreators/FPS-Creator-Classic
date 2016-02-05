// CMarketWindow.cpp : implementation file
//

#include "stdafx.h"
#include "..\editor.h"
#include "MarketWindow.h"
#include <math.h>

#ifdef TGC_STORE
	#include "MarketObject.h"
	#include "ObjectDetailsDialog.h"
	#include "LoginDialog.h"
	#include "SearchDialog.h"
	#include "UploadAppDlg.h"
	#include "ExternalLinkThread.h"
	#include "..\CartMsgDialog.h"
	#include "..\EditDialog.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char		m_szLastPathName [ 255 ];
extern bool g_bFreeVersion;

CMarketWindow::CMarketWindow ( CWnd* pParent ) : CDialog ( CMarketWindow::IDD, pParent )
{
	//{{AFX_DATA_INIT(CMarketWindow)
	//}}AFX_DATA_INIT

#ifdef TGC_STORE
	m_bmpStar.LoadBitmap( IDB_STAR );
	m_bmpEmptyStar.LoadBitmap( IDB_EMPTY_STAR );
	m_bmpMaskStar.LoadBitmap( IDB_MASK_STAR );
	m_bmpInCart.LoadBitmap( IDB_IN_CART );

	m_iSearchValid = 0;
	m_bSearchApprovalObjectsOnly = false;
	iGroupSelected = -10;
	iLastGroupUpdated = -20;
	m_bBuyingCart = false;
	m_bCancelCart = false;
	m_bShowStoreFront = false;
	
	iCartFlashCounter = 0;
	iCartFlashMode = 0;
	iDownloadFlashMode = 0;

	iSearchFlashCounter = 0;
	iSearchFlashMode = 0;

	iItemsPerPage = 40;

	m_bExactMatch = false;

	for ( int i = 0; i < MAX_HISTORY; i++ ) iBrowseHistory [ i ] = 0;
	iHistoryIndex = 0;
	iHistorySize = 1;

	m_bArtistIconsLoaded = false;
	m_bThumbnailsLoaded = false;
	m_sThumbnailsNeeded = "";
	m_bLoggingIn = false;
	m_bLoggingOut = false;
	m_bAutoLogin = false;
	strcpy_s( szUsername, 50, "" );
	strcpy_s( szPassword, 50, "" );
	m_bSearchListLoaded = false;

	m_iCloseWaitTimer = 0;
#endif
}

CMarketWindow::~CMarketWindow ()
{
	if ( m_Uploader.IsRunning() )
	{
		m_Uploader.OnBnClickedCancel( );
		//m_Uploader.Join();
	}

	if ( m_ServerDownloader.IsDownloading() )
	{
		m_ServerDownloader.SetProgress( NULL, NULL );
		m_ServerDownloader.Cancel();
		//m_ServerDownloader.Join();
	}

	if ( m_ModelDownloader.IsDownloading() )
	{
		m_ModelDownloader.SetProgress( NULL, NULL );
		m_ModelDownloader.Cancel();
		//m_ModelDownloader.Join();
	}
}

void CMarketWindow::DoDataExchange ( CDataExchange* pDX )
{


	CDialog::DoDataExchange ( pDX );

	//{{AFX_DATA_MAP(CMarketWindow)
	DDX_Control ( pDX, IDC_TREE1,     m_Tree    );
	//	DDX_Control ( pDX, IDC_LIST2,     m_Image   );
	DDX_Control ( pDX, IDC_SPLIT_MID, m_spliter );

#ifdef TGC_STORE
	DDX_Control ( pDX, IDC_LIST6,	  m_MarketImage	);
#endif

	DDX_Control ( pDX, IDCANCEL, m_Cancel );
	//}}AFX_DATA_MAP

#ifdef TGC_STORE
	DDX_Control(pDX, IDDETAILS, m_btnDetails);
	DDX_Control(pDX, IDLOGIN, m_btnLogin);
	DDX_Control(pDX, IDC_LOADING, m_txtLoading);
	DDX_Control(pDX, IDC_CREDITS, m_txtCredits);
	DDX_Control(pDX, IDC_CREDITSTEXT, m_txtCreditsText);
	DDX_Control(pDX, IDC_LOGGEDINTEXT, m_txtLoggedInText);
	DDX_Control(pDX, IDBUY, m_btnBuy);
	DDX_Control(pDX, IDC_STATUS_TEXT, m_txtStatus);
	DDX_Control(pDX, IDC_STATUS_PROGRESS, m_ProgressBar);
	DDX_Control(pDX, IDC_STATIC_LABEL, m_txtStatusLabel);
	DDX_Control(pDX, IDSEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_CHECK1, m_chkShowPurchased);
	DDX_Control(pDX, IDC_COMBO1, m_comboOrderBy);
	DDX_Control(pDX, IDC_TEXT_ORDER_BY, m_txtOrderBy);
	DDX_Control(pDX, IDC_TEXT_NO_ITEMS, m_textNoItems);
	DDX_Control(pDX, IDC_EXPLORER2, m_WebBrowser);
	DDX_Control(pDX, IDC_AD_BUTTON, m_btnAdBanner);
	DDX_Control(pDX, IDC_BUTTON2, m_btnHome);
	DDX_Control(pDX, IDC_BUTTON3, m_btnBack);
#endif
	DDX_Control(pDX, IDC_BUTTON4, m_btnTopUp);
	DDX_Control(pDX, IDC_BUTTON5, m_btnUploadItem);
	DDX_Control(pDX, IDC_BOX1, m_boxStatusGroup);
	DDX_Control(pDX, IDC_BOX2, m_boxLoginGroup);
	DDX_Control(pDX, IDC_BOX3, m_boxBrowserGroup);
	DDX_Control(pDX, IDC_BOX4, m_boxDetailsGroup);
	DDX_Control(pDX, IDC_CART_TOTAL, m_txtCartTotal);
	DDX_Control(pDX, IDC_CART_TOTAL_LABEL, m_txtCartTotalLabel);
	DDX_Control(pDX, IDC_PAGE_FIRST, m_btnPageFirst);
	DDX_Control(pDX, IDC_PAGE_LEFT, m_btnPageLeft);
	DDX_Control(pDX, IDC_PAGE_RIGHT, m_btnPageRight);
	DDX_Control(pDX, IDC_PAGE_LAST, m_btnPageLast);
	DDX_Control(pDX, IDC_PAGE_COUNTER, m_txtPageCounter);
	DDX_Control(pDX, IDC_ITEMS_PER_PAGE, m_txtItemsPerPage);
	DDX_Control(pDX, IDC_PAGE_COMBO, m_comboItemsPerPage);
}

BEGIN_MESSAGE_MAP ( CMarketWindow, CDialog )
	//{{AFX_MSG_MAP(CMarketWindow)
	ON_WM_CREATE ( )
	ON_NOTIFY    ( TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree )
	ON_NOTIFY    ( TVN_ITEMEXPANDING, IDC_TREE1, OnItemexpandingTree )

#ifdef TGC_STORE
	ON_NOTIFY    ( NM_DBLCLK, IDC_LIST6, OnClickList3 )
	ON_NOTIFY    ( NM_CLICK, IDC_LIST6, OnClickList2 )
	ON_NOTIFY	 ( NM_RCLICK, IDC_LIST6, OnRClickList )
	ON_NOTIFY    ( NM_CUSTOMDRAW, IDC_LIST6, OnCustomdrawList )
	ON_NOTIFY    ( NM_CUSTOMDRAW, IDC_TREE1, OnCustomdrawTree )
	ON_BN_CLICKED(IDDETAILS, &CMarketWindow::OnBnClickedDetails)
	ON_BN_CLICKED(IDLOGIN, &CMarketWindow::OnBnClickedLogin)
	ON_BN_CLICKED(IDBUY, &CMarketWindow::OnBnClickedBuy)
	ON_BN_CLICKED(IDSEARCH, &CMarketWindow::OnSearchClicked)
	ON_BN_CLICKED(IDC_CHECK1, &CMarketWindow::OnShowPurchasedClicked)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMarketWindow::OnOrderByChanged)
	ON_COMMAND(ID__ADDITEMTOCART, &CMarketWindow::OnAddToCart)
	ON_COMMAND(ID__REMOVEITEMFROMCART, &CMarketWindow::OnRemoveFromCart)
	ON_COMMAND(ID__APPROVEITEM, &CMarketWindow::OnApproveItem)
	ON_WM_TIMER()
	ON_WM_SIZE()

	ON_WM_PARENTNOTIFY()
#endif

	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID__REMOVEITEM, &CMarketWindow::OnRemoveItem)
	ON_COMMAND(ID__DELETEITEM, &CMarketWindow::OnDeleteItem)
	ON_COMMAND(ID__RESTOREITEM, &CMarketWindow::OnRestoreItem)
	ON_COMMAND(ID__EDITITEM, &CMarketWindow::OnEditItem)
	ON_BN_CLICKED(IDC_BUTTON2, &CMarketWindow::OnHomeClicked)
	ON_BN_CLICKED(IDC_BUTTON3, &CMarketWindow::OnBackClicked)
	ON_BN_CLICKED(IDC_BUTTON4, &CMarketWindow::OnTopUpClicked)
	ON_BN_CLICKED(IDC_BUTTON5, &CMarketWindow::OnBnUploadClicked)
	ON_BN_CLICKED(IDC_AD_BUTTON, &CMarketWindow::OnBnClickedAdBanner)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_PAGE_FIRST, &CMarketWindow::OnBnClickedPageFirst)
	ON_BN_CLICKED(IDC_PAGE_LEFT, &CMarketWindow::OnBnClickedPageLeft)
	ON_BN_CLICKED(IDC_PAGE_RIGHT, &CMarketWindow::OnBnClickedPageRight)
	ON_BN_CLICKED(IDC_PAGE_LAST, &CMarketWindow::OnBnClickedPageLast)
	ON_CBN_SELCHANGE(IDC_PAGE_COMBO, &CMarketWindow::OnItemsPerPageChanged)
END_MESSAGE_MAP ( )

int CMarketWindow::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	if ( CDialog::OnCreate ( lpCreateStruct ) == -1 )
		return -1;

	

	return 0;
}

void CMarketWindow::OnActivate( UINT nState, CWnd *pWndOther, BOOL bMinimized )
{
	if ( !m_hWnd ) return;

	if ( nState != WA_INACTIVE && m_Uploader.IsRunning( ) )
	{
		m_Uploader.SetWindowPos( &wndTopMost, 0,0, 0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
	}

	CDialog::OnActivate( nState, pWndOther, bMinimized );
}

#ifdef TGC_STORE
void CMarketWindow::PositionStatusText( )
{
	CRect marketRect, noItemRect;
	m_MarketImage.GetWindowRect( &marketRect );
	m_textNoItems.GetClientRect( &noItemRect );
	ScreenToClient( &marketRect );

	int x = marketRect.left + ( marketRect.Width() - noItemRect.Width() ) / 2;
	int y = marketRect.top + ( marketRect.Height() - noItemRect.Height() ) / 2;

	m_textNoItems.SetWindowPos( &m_MarketImage, x,y, 0,0, SWP_NOSIZE );
	m_MarketImage.Invalidate( );
	m_textNoItems.Invalidate( );
}

void CMarketWindow::ResizeAllPanes( int x, int y, int cx, int cy )
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	this->ScreenToClient( &pt );

	CRect rect, treeRect, mainRect, rectSize;
	//theApp.m_pEditorView->GetWindowRect ( &rect );
	//theApp.GetMainWnd()->GetWindowRect( &mainRect );
	//rect.top = mainRect.top + 30;
	//rect.left = mainRect.left + 1;
	//rect.right--;
	GetClientRect( &rect );
	//ClientToScreen( &rect );
	m_Tree.GetWindowRect( &treeRect );
	ScreenToClient( treeRect );

	DWORD dwWidth = rect.right - rect.left;
	DWORD dwHeight = rect.bottom - rect.top;

	int iTreePercent     = 25;
	int iSplitterPercent = 2;
	int iImagePercent    = 70;

	int iMins = 50;
	//int iAdBannerWidth = 120;
	int iAdBannerWidth = 1;
			
	int iImageX         = ( dwWidth  / 100 ) + treeRect.Width() + 5;
	int iImageY         = ( dwHeight / 100  );
	int iImageWidth     = dwWidth - treeRect.Width() - 20;
	int iImageHeight    = ( rect.bottom - rect.top ) - iMins;

	//m_WebBrowser.SetWindowPos( &wndBottom, iImageX,iImageY, iImageWidth - (iAdBannerWidth + 7),iImageHeight - 190, 0 );
	if ( m_bShowStoreFront ) m_WebBrowser.SetWindowPos( &wndBottom, iImageX,iImageY, iImageWidth - (iAdBannerWidth + 7),iImageHeight - 41, 0 );

	//if ( theApp.m_szSESID )
	{
		m_btnLogin.GetClientRect( rectSize ); 
		int iLoginSizeX = treeRect.Width() < 160 + (BUTTON_BORDER*2 + 4) ? (treeRect.Width()-(BUTTON_BORDER*2 + 4))/2 : 80;
		int iLoginX = treeRect.left + treeRect.Width()/2 - iLoginSizeX - 2;
		m_btnLogin.SetWindowPos ( NULL, iLoginX, dwHeight - (81 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );
		//m_boxLoginGroup.SetWindowPos( NULL, iLoginX - BUTTON_BORDER, dwHeight - 81, iLoginSizeX*2 + (BUTTON_BORDER*2 + 4), rectSize.Height() + BUTTON_BORDER*2, 0 );
		m_boxLoginGroup.SetWindowPos( NULL, treeRect.left, dwHeight - 81, treeRect.Width(), rectSize.Height() + BUTTON_BORDER*2, 0 );

		int iTopUpX = treeRect.left + treeRect.Width()/2 + 2;
		m_btnTopUp.SetWindowPos ( NULL, iTopUpX, dwHeight - (81 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );

		m_btnLogin.Invalidate();
		m_btnTopUp.Invalidate();
	}
	/*
	else
	{
		m_btnLogin.GetClientRect( rectSize ); 
		int iLoginSizeX = treeRect.Width() < 80 ? treeRect.Width() : 80;
		int iLoginX = treeRect.left + treeRect.Width()/2 - iLoginSizeX/2;
		m_btnLogin.SetWindowPos ( NULL, iLoginX, dwHeight - (78 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );
	}
	*/

	
	m_btnHome.GetClientRect( rectSize );
	int iHomeGroupWidth = rectSize.Width();
	m_btnBack.GetClientRect( rectSize );
	iHomeGroupWidth += rectSize.Width();

	m_btnHome.SetWindowPos( NULL, iImageX+BUTTON_BORDER+1, dwHeight - (81 - BUTTON_BORDER), 0,0, SWP_NOSIZE );
	m_btnHome.GetClientRect( rectSize ); int iBackX = iImageX + rectSize.Width() + 5 + BUTTON_BORDER;
	m_btnBack.SetWindowPos( NULL, iBackX, dwHeight - (81 - BUTTON_BORDER), 0,0, SWP_NOSIZE );

	int iHomeHeight = rectSize.Height();
	m_btnUploadItem.GetClientRect( rectSize );
	m_btnUploadItem.SetWindowPos ( NULL, iImageX+BUTTON_BORDER+1, dwHeight - (81-BUTTON_BORDER-iHomeHeight-1), iHomeGroupWidth+4, rectSize.Height(), 0 );
	
	int iHomeGroupHeight = theApp.m_bUploader ? rectSize.Height() + iHomeHeight + BUTTON_BORDER*2 + 1 : iHomeHeight + BUTTON_BORDER*2;
	m_boxBrowserGroup.SetWindowPos( NULL, iImageX+1, dwHeight - 81, iHomeGroupWidth + BUTTON_BORDER*2 + 4, iHomeGroupHeight, 0 );
	
	CRect rectDraw;
	rectDraw.SetRect( treeRect.left, dwHeight - 81, treeRect.right, (dwHeight - 81) + rectSize.Height() + BUTTON_BORDER*2 + 1 );
	this->InvalidateRect( rectDraw );
	rectDraw.SetRect( iImageX+1, dwHeight - 81, iImageX + iHomeGroupWidth + BUTTON_BORDER*2 + 4, (dwHeight - 81) + rectSize.Height() + iHomeHeight + BUTTON_BORDER*2 + 2 );
	this->InvalidateRect( rectDraw );

	m_boxLoginGroup.Invalidate();
	m_boxBrowserGroup.Invalidate();
	m_btnHome.Invalidate();
	m_btnBack.Invalidate();
	m_btnUploadItem.Invalidate();

	PositionStatusText( );
}

void CMarketWindow::ShowBrowser( BOOL bShow )
{
	CRect rect, treeRect, mainRect;
	/*
	theApp.m_pEditorView->GetWindowRect ( &rect );
	theApp.GetMainWnd()->GetWindowRect( &mainRect );
	rect.top = mainRect.top + 20;
	rect.left = mainRect.left + 1;
	rect.right--;
	*/
	GetClientRect( &rect );
	ClientToScreen( &rect );
	m_Tree.GetWindowRect( &treeRect );
	ScreenToClient( treeRect );

	DWORD dwWidth = rect.right - rect.left;
	DWORD dwHeight = rect.bottom - rect.top;

	int iTreePercent     = 25;
	int iSplitterPercent = 2;
	int iImagePercent    = 70;

	int iMins = 50;
	//int iAdBannerWidth = 120;
	int iAdBannerWidth = 1;
			
	int iImageX         = ( dwWidth  / 100 ) + treeRect.Width() + 5;
	int iImageY         = ( dwHeight / 100  );
	int iImageWidth     = dwWidth - (treeRect.Width() + 20);
	int iImageHeight    = ( rect.bottom - rect.top ) - iMins;

	if ( bShow )
	{
		CString sStoreFront = "http://www.tgcstore.net/StoreFront.php?version=";
		sStoreFront += theApp.m_szVersion;
		if ( theApp.m_szSESID ) sStoreFront += "&loggedin=1";
		sStoreFront += "&lang=";
		sStoreFront += theApp.m_szLanguageName;

		char szWidth [ 20 ];
		sprintf_s( szWidth, 20, "&width=%d", iImageWidth - (iAdBannerWidth + 7) );
		sStoreFront += szWidth;

		m_WebBrowser.Navigate( sStoreFront ,NULL, NULL, NULL, NULL );
		//m_WebBrowser.SetWindowPos( NULL, iImageX, iImageY, iImageWidth - (iAdBannerWidth + 7), iImageHeight - 190, 0 );
		m_WebBrowser.SetWindowPos( NULL, iImageX, iImageY, iImageWidth - (iAdBannerWidth + 7), iImageHeight - 41, 0 );
		m_WebBrowser.ShowWindow( SW_SHOW );
		m_WebBrowser.EnableWindow( TRUE );
		m_bShowStoreFront = true;

		//m_WebBrowser.Invalidate( );
		//m_MarketImage.SetWindowPos( NULL, iImageX, iImageY + (iImageHeight - 150 - 20 - 18), iImageWidth - (iAdBannerWidth + 7), 150, 0 );
		m_MarketImage.ShowWindow( SW_HIDE );
		ShowPageControls( false );

		iGroupSelected = 0;
		m_btnBack.EnableWindow( TRUE );
	}
	else
	{
		//m_WebBrowser.ShowWindow( SW_HIDE );
		m_WebBrowser.SetWindowPos( NULL, 0,0,0,0, 0 );
		m_WebBrowser.EnableWindow( FALSE );
		m_WebBrowser.Invalidate();
		m_bShowStoreFront = false;

		m_MarketImage.SetWindowPos( NULL, iImageX, iImageY, iImageWidth - (iAdBannerWidth + 7), iImageHeight - 61, 0 );
		m_MarketImage.ShowWindow( SW_SHOW );
		ShowPageControls( true );
	}
}

void CMarketWindow::ShowPageControls( bool bShow )
{
	if ( bShow )
	{
		m_txtItemsPerPage.ShowWindow( SW_SHOW );
		m_comboItemsPerPage.ShowWindow( SW_SHOW );
		m_btnPageFirst.ShowWindow( SW_SHOW );
		m_btnPageLeft.ShowWindow( SW_SHOW );
		m_txtPageCounter.ShowWindow( SW_SHOW );
		m_btnPageRight.ShowWindow( SW_SHOW );
		m_btnPageLast.ShowWindow( SW_SHOW );
	}
	else
	{
		m_txtItemsPerPage.ShowWindow( SW_HIDE );
		m_comboItemsPerPage.ShowWindow( SW_HIDE );
		m_btnPageFirst.ShowWindow( SW_HIDE );
		m_btnPageLeft.ShowWindow( SW_HIDE );
		m_txtPageCounter.ShowWindow( SW_HIDE );
		m_btnPageRight.ShowWindow( SW_HIDE );
		m_btnPageLast.ShowWindow( SW_HIDE );
	}
}

void CMarketWindow::SetPageNumbers( int current, int total )
{
	iCurrentPage = current;
	iMaxPage = total;

	CString sPage;
	sPage.Format( "Page: %d/%d", current, total );
	
	m_txtPageCounter.SetWindowText( sPage );
	m_txtPageCounter.Invalidate( );

	if ( iCurrentPage == 1 )
	{
		m_btnPageFirst.EnableWindow( FALSE );
		m_btnPageLeft.EnableWindow( FALSE );
	}
	else
	{
		m_btnPageFirst.EnableWindow( TRUE );
		m_btnPageLeft.EnableWindow( TRUE );
	}

	if ( iCurrentPage == iMaxPage )
	{
		m_btnPageLast.EnableWindow( FALSE );
		m_btnPageRight.EnableWindow( FALSE );
	}
	else
	{
		m_btnPageLast.EnableWindow( TRUE );
		m_btnPageRight.EnableWindow( TRUE );
	}
}

void CMarketWindow::SwitchToMarketPlace( BOOL bMarket, const char *szGroup )
{
	if ( theApp.m_iAppWidth == 0 ) return;

	m_textNoItems.ShowWindow( SW_HIDE );

	KillTimer( 9 );
	KillTimer( 4 );
	if ( m_ServerDownloader.IsDownloading() ) m_ServerDownloader.Cancel();

	CRect rect, treeRect, mainRect;
	/*
	theApp.m_pEditorView->GetWindowRect ( &rect );
	theApp.GetMainWnd()->GetWindowRect( &mainRect );
	rect.top = mainRect.top + 20;
	rect.left = mainRect.left + 1;
	rect.right--;
	*/
	GetClientRect( &rect );
	//ClientToScreen( &rect );
	m_Tree.GetWindowRect( &treeRect );
	ScreenToClient( treeRect );

	DWORD dwWidth = rect.right - rect.left;
	//DWORD dwWidth = theApp.m_iAppWidth;
	DWORD dwHeight = rect.bottom - rect.top;

	int iTreePercent     = 25;
	int iSplitterPercent = 2;
	int iImagePercent    = 70;

	int iMins = 50;
	//int iAdBannerWidth = 120;
	int iAdBannerWidth = 1;
			
	int iImageX         = ( dwWidth  / 100 ) + treeRect.Width() + 5;
	int iImageY         = ( dwHeight / 100  );
	int iImageWidth     = dwWidth - (treeRect.Width() + 20);
	int iImageHeight    = ( rect.bottom - rect.top ) - iMins;
	
	if ( bMarket )
	{
		CRect rect;
		GetClientRect( rect );

		int iMins = 50;
		int iTreeHeight     = ( rect.Height() ) - iMins;
		//m_Tree.SetWindowPos( NULL, 0,0, treeRect.Width(), iTreeHeight - 13 - 20, SWP_NOMOVE );

//		m_Image.ShowWindow( SW_HIDE );
		//m_MarketImage.ShowWindow( SW_SHOW );
		m_btnDetails.ShowWindow( SW_SHOW );
		m_btnLogin.ShowWindow( SW_SHOW );
		m_btnSearch.ShowWindow( SW_SHOW );
		m_txtCredits.ShowWindow( SW_SHOW );
		m_txtLoading.ShowWindow( SW_SHOW );
		m_txtCreditsText.ShowWindow( SW_SHOW );
		m_txtLoggedInText.ShowWindow( SW_SHOW );
		m_btnBuy.ShowWindow( SW_SHOW );

		m_btnDetails.EnableWindow( FALSE );
		m_btnBuy.EnableWindow( FALSE );

		m_txtStatusLabel.ShowWindow( SW_SHOW );
		m_txtStatus.ShowWindow( SW_SHOW );
		m_ProgressBar.ShowWindow( SW_SHOW );

		if ( theApp.m_szSESID ) m_chkShowPurchased.ShowWindow( SW_SHOW );
		else m_chkShowPurchased.ShowWindow( SW_HIDE );
		m_comboOrderBy.ShowWindow( SW_SHOW );
		m_txtOrderBy.ShowWindow( SW_SHOW );

		CString sHome = GetLanguageData( "TGC Store", "Home" );

		if ( !szGroup || strlen( szGroup ) < 1 || strcmp( szGroup, sHome ) == 0 )
		{
			CString sStoreFront = "http://www.tgcstore.net/StoreFront.php?version=";
			sStoreFront += theApp.m_szVersion;
			if ( theApp.m_szSESID ) sStoreFront += "&loggedin=1";
			sStoreFront += "&lang=";
			sStoreFront += theApp.m_szLanguageName;

			char szWidth [ 20 ];
			sprintf_s( szWidth, 20, "&width=%d", iImageWidth - (iAdBannerWidth + 7) );
			sStoreFront += szWidth;

			m_MarketImage.ShowWindow( SW_HIDE );
			m_WebBrowser.EnableWindow( TRUE );
			m_WebBrowser.SetWindowPos( NULL, iImageX, iImageY, iImageWidth - (iAdBannerWidth + 7), iImageHeight - 41, 0 );
			m_bShowStoreFront = true;

			if ( theApp.m_bFirstRun && !theApp.m_szSESID ) 
			{
				CString sURL = "http://www.thegamecreators.com/index_plain.php?m=store_registration";
				sURL += "&lang=";
				sURL += theApp.m_szLanguageName;
				m_WebBrowser.Navigate( sURL, NULL, NULL, NULL, NULL );
			}
			else m_WebBrowser.Navigate( sStoreFront ,NULL, NULL, NULL, NULL );

			
			//m_WebBrowser.ShowWindow( SW_SHOW );
			//m_WebBrowser.Invalidate( );
			//m_MarketImage.SetWindowPos( NULL, iImageX, iImageY + (iImageHeight - 147 - 38), iImageWidth - (iAdBannerWidth + 7), 147, 0 );

			m_btnBack.EnableWindow( TRUE );

			ShowPageControls( false );
		}
		else
		{
			//m_WebBrowser.ShowWindow( SW_HIDE );
			m_WebBrowser.SetWindowPos( NULL, 0,0,0,0, 0 );
			m_WebBrowser.EnableWindow( FALSE );
			m_WebBrowser.Invalidate();
			m_bShowStoreFront = false;

			m_MarketImage.SetWindowPos( NULL, iImageX, iImageY, iImageWidth - (iAdBannerWidth + 7), iImageHeight - 61, 0 );
			m_MarketImage.ShowWindow( SW_SHOW );

			if ( iHistorySize <= 1 ) m_btnBack.EnableWindow( FALSE ); 
			else m_btnBack.EnableWindow( TRUE );

			ShowPageControls( true );
		}

		//m_btnAdBanner.SetWindowPos( NULL, (iImageX + iImageWidth) - iAdBannerWidth - 5, iImageY, iAdBannerWidth, iImageHeight - 1, 0 );

		PositionStatusText( );

		/*
		int timeout = 300;
		while ( m_ServerDownloader.IsDownloading() && timeout > 0 )
		{
			Sleep(3);
			timeout--;
		}
		*/

		iGroupSelected = 0;
		m_bThumbnailsLoaded = false;

		if ( szGroup && strlen( szGroup ) > 0 )
		{
			char szLocation [ 256 ];

			/*
			if ( strncmp( szGroup, "Home", strlen("Home") ) == 0 ) { iGroupSelected = 0; strcpy_s( szLocation, 256, "Home" ); }
			else if ( strncmp( szGroup, "Search Results", strlen("Search Results") ) == 0 ) { iGroupSelected = -1; strcpy_s( szLocation, 256, "Search Results" ); }
			else if ( strncmp( szGroup, "Shopping Cart", strlen("Shopping Cart") ) == 0 ) { iGroupSelected = -2; strcpy_s( szLocation, 256, "Shopping Cart" ); }
			else if ( strncmp( szGroup, "Free Items", strlen("Free Items") ) == 0 ) { iGroupSelected = -4; strcpy_s( szLocation, 256, "Free Items" ); }
			else if ( strncmp( szGroup, "Online Items", strlen("Online Items") ) == 0 )
			*/
			
			CString sSearchResults = GetLanguageData( "TGC Store", "SearchResults" );
			CString sShoppingCart = GetLanguageData( "TGC Store", "ShoppingCart" );
			CString sFreeItems = GetLanguageData( "TGC Store", "FreeItems" );
			CString sOnlineItems = GetLanguageData( "TGC Store", "OnlineItems" );

			if ( strncmp( szGroup, sHome, sHome.GetLength() ) == 0 ) { iGroupSelected = 0; strcpy_s( szLocation, 256, sHome ); }
			else if ( strncmp( szGroup, sSearchResults, sSearchResults.GetLength() ) == 0 ) { iGroupSelected = -1; strcpy_s( szLocation, 256, sSearchResults ); }
			else if ( strncmp( szGroup, sShoppingCart, sShoppingCart.GetLength() ) == 0 ) { iGroupSelected = -2; strcpy_s( szLocation, 256, sShoppingCart ); }
			else if ( strncmp( szGroup, sFreeItems, sFreeItems.GetLength() ) == 0 ) { iGroupSelected = -4; strcpy_s( szLocation, 256, sFreeItems ); }
			else if ( strncmp( szGroup, sOnlineItems, sOnlineItems.GetLength() ) == 0 )
			{
				if ( strchr( szGroup, '\\' ) )
				{
					const char *szGroupName = strchr( szGroup, '\\' );
					if ( szGroupName ) szGroupName++;
					else szGroupName = szGroup;

					MarketGroup *pGroup = MarketObject::GetGroup( szGroupName );
					if ( pGroup ) iGroupSelected = pGroup->dwGroupID;

					strcpy_s( szLocation, 256, szGroupName );
				}
				else 
				{
					iGroupSelected = -3;
					strcpy_s( szLocation, 256, sOnlineItems );
				}
			}

			char str [ 256 ];
			sprintf_s( str, 256, "Game Creator Store - [%s]", szLocation );
			SetWindowText( str );
		}
		else
		{
			//SetWindowText( "Game Creator Store - [Home]" );
			char str [ 256 ];
			sprintf_s( str, 256, "Game Creator Store - [%s]", sHome );
			SetWindowText( str );
		}

		if ( iGroupSelected != 0 /*&& !m_ServerDownloader.IsDownloading()*/ )
		{
			iCurrentPage = 1;
			LoadObjectList();
			
			/*
			if ( m_ServerDownloader.IsDownloading() )
			{
				SetTimer( 9, 1000, NULL );
			}
			else
			{
				char order [ 32 ];
				m_comboOrderBy.GetWindowText( order, 32 );
				m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
			}
			*/
		}

		RecordHistory( iGroupSelected );

		if ( iGroupSelected == -2 )
		{
			if ( m_bBuyingCart ) m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("CancelBuy") ) );
			else m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("BuyCart") ) );
			m_btnBuy.EnableWindow( TRUE );
		}
		else
		{
			m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Buy") ) );
		}
		//else m_MarketImage.DeleteAllItems( );
	}
}

void CMarketWindow::LoadObjectList( )
{
	if ( iGroupSelected > 0 )
	{
		CString sObjectList;
		sObjectList.Format( "%s\\Files\\TGCStore\\TEMP\\ObjectList%d.xml", theApp.m_szDirectory, iGroupSelected );

		FILE *pFile = fopen( sObjectList, "rb" );
		if ( !pFile ) 
		{
			m_textNoItems.SetWindowText( "Contacting Store..." );
			m_textNoItems.ShowWindow( SW_SHOW );
			m_textNoItems.Invalidate();
			SetTimer( 98, 5000, NULL );

			SetTimer( 11, 200, NULL );
			m_MarketImage.DeleteAllItems();
		}
		else 
		{
			fclose( pFile );
			bool bSucceed = MarketObject::LoadObjects( sObjectList );
			if ( !bSucceed ) MessageBox( MarketObject::GetLastError( ), "List Error" );
			
			if ( m_ServerDownloader.IsDownloading() )
			{
				SetTimer( 9, 500, NULL );
			}
			else
			{
				char order [ 32 ];
				m_comboOrderBy.GetWindowText( order, 32 );
				m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
			}
		}
	}
	else if ( iGroupSelected == -4 )
	{
		CString sObjectList;
		sObjectList.Format( "%s\\Files\\TGCStore\\TEMP\\FreeList.xml", theApp.m_szDirectory );

		FILE *pFile = fopen( sObjectList, "rb" );
		if ( !pFile ) 
		{
			m_textNoItems.SetWindowText( "Contacting Store..." );
			m_textNoItems.ShowWindow( SW_SHOW );
			m_textNoItems.Invalidate();
			SetTimer( 98, 5000, NULL );

			SetTimer( 11, 200, NULL );
			m_MarketImage.DeleteAllItems();
		}
		else 
		{
			fclose( pFile );
			bool bSucceed = MarketObject::LoadObjects( sObjectList );
			if ( !bSucceed ) MessageBox( MarketObject::GetLastError( ), "List Error" );
			
			if ( m_ServerDownloader.IsDownloading() )
			{
				SetTimer( 9, 500, NULL );
			}
			else
			{
				char order [ 32 ];
				m_comboOrderBy.GetWindowText( order, 32 );
				m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
			}
		}
	}
	else
	{
		if ( m_ServerDownloader.IsDownloading() )
		{
			SetTimer( 9, 500, NULL );
		}
		else
		{
			char order [ 32 ];
			m_comboOrderBy.GetWindowText( order, 32 );
			m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
		}
	}
}

void CMarketWindow::BackSpace( )
{
	m_Tree.SendMessage( WM_KEYDOWN, 8, 0 );
}

void CMarketWindow::LoadTGCStore( )
{
	//iGroupSelected = 0;
	//if ( m_ServerDownloader.IsDownloading() ) 
	{
		SetTimer( 8, 1000, NULL );
		return;
	}

	/*
	m_ServerDownloader.SetProgress( &m_ProgressBar, NULL );
	m_ServerDownloader.SetCategoryDownload( this &m_MarketImage, &m_Tree );
	m_ServerDownloader.Start( );

	m_textNoItems.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Loading") ) );
	m_textNoItems.ShowWindow( SW_SHOW );
	*/
}

void CMarketWindow::LinkLoaded( )
{
	m_btnAdBanner.SetLoading( false );
}

void CMarketWindow::UploadComplete( DWORD dwObjID )
{
	HTTPComm server;
	if ( !server.GetModelDetails( dwObjID, theApp.m_szSESID ) )
	{
		return;
		//MessageBox( server.GetLastError(), "Server Error" );
	}
	
	MarketObject cLocalObject;
	cLocalObject.CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );

	ClearObjectsDir( cLocalObject.GetGroupID() );

	if ( iGroupSelected == cLocalObject.GetGroupID() ) SetTimer( 11, 1000, NULL );

	m_Tree.ReloadTree( );
	SetTimer( 8, 500, NULL );
	//LoadTGCStore( );
}

void CMarketWindow::DownloadComplete( int group, DWORD obj, bool bSuccess )
{
	KillTimer( 7 );
	m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Idle") ) );
	m_Cancel.SetWindowText( GetLanguageData( "TGC Store", "Close" ) );

	if ( bSuccess )
	{
		MarketObject::RemoveFromCart( obj );
		m_Tree.SetCartCount( MarketObject::CountCartObjects() );
	}
	else
	{
		if ( m_bBuyingCart )
		{
			m_bBuyingCart = false;
			SetTimer( 1, 300, NULL );
			return;
		}
	}

	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );

	//if ( m_iSearchValid == 2 )
	{
	//	MarketObject::CopyToSearchObjects();
	}
	if ( m_iSearchValid == 1 )
	{
		MarketObject::Search( m_sLSKeyword, m_bSearchApprovalObjectsOnly, m_bExactMatch );
		/*
		HTTPComm server;
		if ( server.Search( "Files\\TGCStore\\TEMP\\SearchResults.xml", m_sLSName, m_sLSCategory, m_sLSCostLow, m_sLSCostHigh, 
						m_sLSAuthor, m_sLSRatingLow, m_sLSRatingHigh, m_sLSKeyword ) )
		{
			MarketObject::LoadSearchObjects( "Files\\TGCStore\\TEMP\\SearchResults.xml" );
		}
		*/
	}

	if ( bSuccess ) 
	{
		//ClearObjectsDir( group );
	}

	//MarketObject::UpdateCartObjects();

	//iCurrentPage = 1;
	//m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, 1 );
		
	HTTPComm update;
	if ( update.UpdateCredits( theApp.m_szSESID ) )
	{
		m_txtCredits.SetWindowText( theApp.m_szCredits );
		m_txtCredits.Invalidate( );
	}

	if ( m_bBuyingCart )
	{
		/*
		MarketObject *pHeadObject = MarketObject::GetCartList();
		if ( pHeadObject )
		{
			MarketObject::RemoveFromCart( pHeadObject->GetID() );
			m_Tree.SetCartCount( MarketObject::CountCartObjects() );
		}
		*/

		SetTimer( 1, 300, NULL );
	}
	else
	{
		if ( bSuccess )
		{
			/*
			CString sFilename = theApp.m_szDirectory;
			sFilename += "\\Files\\TGCStore\\TEMP\\PurchasedObjects.xml";
			DeleteFile( sFilename );
			*/

			DWORD dwValue = 1;
			theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "display_cart_buy_message", &dwValue );
			if ( dwValue == 1 )
			{
				CartMsgDialog optionalMsg;
				optionalMsg.SetValues( GetLanguageData( "TGC Store", "CartFinished" ), GetLanguageData( "TGC Store", "Complete" ), "cart_buy" );
				optionalMsg.DoModal();
			}

			m_textNoItems.SetWindowText( GetLanguageData( "TGC Store", "DownloadSuccess" ) );
			m_textNoItems.ShowWindow( SW_SHOW );
			m_textNoItems.Invalidate();
			SetTimer( 98, 5000, 0 );

			SetTimer( 12, 200, NULL );
		}
	}

	SetTimer( 9,500,NULL );
	//m_Tree.ExpandStore( );

	UpdateCartTotal( );
}

void CMarketWindow::ListUpdated( )
{
	CString sGroupList = theApp.m_szDirectory;
	sGroupList += "\\";
	sGroupList += "Files\\TGCStore\\TEMP\\GroupList.xml";

	bool bResult = MarketObject::LoadGroups( sGroupList );
	if ( !bResult )
	{
		MessageBox( GetLanguageData( "TGC Store", "GroupListFailed" ) );
		return;
	}

	if ( !m_ModelDownloader.IsDownloading() ) m_txtStatus.SetWindowText( GetLanguageData( "TGC Store", "Idle" ) );
	m_textNoItems.ShowWindow( SW_HIDE );

	m_Tree.UpdateStoreCategories( iGroupSelected );
	
	m_btnDetails.EnableWindow( FALSE );
	m_btnBuy.EnableWindow( FALSE );

	m_Tree.SetSearchCount( MarketObject::CountSearchObjects() );
	m_Tree.SetCartCount( MarketObject::CountCartObjects() );
	m_Tree.SetObjectCount( MarketObject::CountObjects() );
}

void CMarketWindow::PurchasedListUpdated( )
{
	CString sPurchasedList = theApp.m_szDirectory;
	sPurchasedList += "\\Files\\TGCStore\\TEMP\\PurchasedObjects.xml";
	MarketObject::SetPurchasedObjects( sPurchasedList );

	if ( !m_ModelDownloader.IsDownloading() ) m_txtStatus.SetWindowText( GetLanguageData( "TGC Store", "Idle" ) );
	//m_textNoItems.ShowWindow( SW_HIDE );

	if ( iGroupSelected > -10 )
	{
		SetTimer( 9,400,0 );
	}
}

void CMarketWindow::SearchListUpdated( )
{
	CString sSearchList = theApp.m_szDirectory;
	sSearchList += "\\Files\\TGCStore\\TEMP\\SearchList.xml";
	MarketObject::LoadCompleteList( sSearchList );

	CString sKeywordList = theApp.m_szDirectory;
	sKeywordList += "\\Files\\TGCStore\\TEMP\\KeywordList.xml";
	MarketObject::SetKeywords( sKeywordList );

	CString sPurchasedList = theApp.m_szDirectory;
	sPurchasedList += "\\Files\\TGCStore\\TEMP\\PurchasedObjects.xml";
	MarketObject::SetPurchasedObjects( sPurchasedList );

	m_bSearchListLoaded = true;

	if ( !m_ModelDownloader.IsDownloading() ) m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Idle") ) );
	m_textNoItems.ShowWindow( SW_HIDE );

	if ( m_iSearchValid > 0 )
	{
		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );

		{
			MarketObject::Search( m_sLSKeyword, m_bSearchApprovalObjectsOnly, m_bExactMatch );
			
			m_iSearchValid = 1;
			iCurrentPage = 1;
			
			if ( iGroupSelected == -1 ) SetTimer( 9,1000,NULL );//m_MarketImage.LoadRemoteImages( -1, m_chkShowPurchased.GetCheck() > 0, order );
			else m_Tree.SelectSearchResults( );
			RecordHistory( -1 );
		}

		int count = MarketObject::CountSearchObjects();
		m_Tree.SetSearchCount( count );

		if ( count > 0 )
		{
			iSearchFlashCounter = 5;
			SetTimer( 3, 500, NULL );
		}
	}
}

void CMarketWindow::ObjectsUpdated( )
{
	CString sObjectList;
	if ( iGroupSelected > 0 ) sObjectList.Format( "%s\\Files\\TGCStore\\TEMP\\ObjectList%d.xml", theApp.m_szDirectory, iGroupSelected );
	else if ( iGroupSelected == -4 ) sObjectList.Format( "%s\\Files\\TGCStore\\TEMP\\FreeList.xml", theApp.m_szDirectory );
	else return;

	bool bSucceed = MarketObject::LoadObjects( sObjectList );
	if ( !bSucceed ) MessageBox( MarketObject::GetLastError( ), "List Error" );

	if ( !m_ModelDownloader.IsDownloading() ) m_txtStatus.SetWindowText( GetLanguageData( "TGC Store", "Idle" ) );

	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );

	m_textNoItems.ShowWindow( SW_HIDE );

	m_btnDetails.EnableWindow( FALSE );
	m_btnBuy.EnableWindow( FALSE );

	if ( iGroupSelected > -10 )
	{
		//m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
		SetTimer( 9,200,0 );
	}
}

void CMarketWindow::ThumbnailsUpdated( )
{
	m_bThumbnailsLoaded = true;

	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );

	m_textNoItems.ShowWindow( SW_HIDE );

	if ( iGroupSelected >= -10 )
	{
		m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage, true );
	}

	m_ServerDownloader.SetProgress( NULL, NULL );

	if ( !m_ModelDownloader.IsDownloading() ) m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Idle") ) );
}

void CMarketWindow::UpdateThumbnails( CString sNeeded )
{
	/*
	int timeout = 20;
	while ( m_ServerDownloader.IsDownloading() && timeout > 0 )
	{
		Sleep(10);
		timeout--;
	}
	*/

	/*
	if ( !m_ServerDownloader.IsDownloading() )
	{
		//if ( iLastGroupUpdated == iGroupSelected ) return;
		iLastGroupUpdated = iGroupSelected;

		m_textNoItems.SetWindowText( GetLanguageData( _T("TGC Store"), _T("LoadThumbnails") ) );
		m_textNoItems.ShowWindow( SW_SHOW );

		m_ServerDownloader.SetThumbnailDownload( iGroupSelected, this );
		m_ServerDownloader.Start();
	}
	else*/
	if ( !m_bThumbnailsLoaded )
	{
		m_sThumbnailsNeeded = sNeeded;
		SetTimer( 4, 500, NULL );
	}
}
#endif

BOOL CMarketWindow::OnInitDialog ( )
{
	CDialog::OnInitDialog ( );
	
#ifdef TGC_STORE
	//if ( theApp.m_szSESID ) delete [] theApp.m_szSESID;
	//theApp.m_szSESID = NULL;

	//m_Image.Start ( this );
	m_MarketImage.Start ( this );
	m_MarketImage.ShowWindow( SW_HIDE );
	ShowPageControls( false );

	char szIEVersion [ 30 ];
	if ( !theApp.GetRegistryValue( "Software\\Microsoft\\Internet Explorer", "Version", szIEVersion, 30, HKEY_LOCAL_MACHINE ) )
	{
		MessageBox( GetLanguageData( "TGC Store", "NoIE" ), "Warning" );
	}
	else
	{
		char *szDot = strchr( szIEVersion, '.' );
		if ( szDot ) *szDot = '\0';
		DWORD dwIEVersion = atoi( szIEVersion );
		if ( dwIEVersion < 7 ) MessageBox( GetLanguageData( "TGC Store", "NoIE" ), "Warning" );
	}

	m_Tree.Start  ( this, /*&m_Image,*/ &m_MarketImage, 0 );
#endif
	
	char szBlankImage [ 255 ];
	CString string = "FPSEXCHANGE";

	{
		{
			//HANDLE m_hFileMap;
			//LPVOID m_lpVoid;
			//BYTE*  m_pData;

			//m_hFileMap = OpenFileMapping ( FILE_MAP_ALL_ACCESS, TRUE, string );
			//m_lpVoid   = MapViewOfFile ( m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0 );


			//m_pData    = ( BYTE* ) m_lpVoid;

			//if ( m_lpVoid )
			{
				//if ( m_pData )
				{
					memset ( szBlankImage, 0, sizeof ( szBlankImage ) );

					cIPC* pIPC	= theApp.m_Message.GetIPC ( );
					
					pIPC->ReceiveBuffer ( &szBlankImage [ 0 ], 2280, sizeof ( szBlankImage ) );

					//memcpy ( &szBlankImage [ 0 ], &theApp.m_pData [ 2280 ], sizeof ( char ) * 255 );
				}
			}

			//UnmapViewOfFile ( m_lpVoid );
			//CloseHandle     ( m_hFileMap );
		}
	}

	CString title = szBlankImage;

	//SetWindowText ( title );

	m_spliter.addWindow ( IDC_TREE1,sp_StyleRight      );
	m_spliter.addWindow ( IDC_LIST6,sp_StyleLeft       );
	m_spliter.setStyle  ( sp_StyleLeft | sp_StyleRight );

#ifdef TGC_STORE
	if ( !m_fntCredits.CreatePointFont( 100, "Calibri" ) ) m_fntCredits.CreatePointFont( 100, "Arial" );
	if ( !m_fntLogin.CreatePointFont( 100, "Calibri" ) ) m_fntLogin.CreatePointFont( 100, "Arial" );

	m_txtCreditsText.SetFont( &m_fntCredits, 1 );
	m_txtCredits.SetFont( &m_fntCredits, 1 );

	m_txtLoggedInText.SetFont( &m_fntLogin, 1 );
	m_txtLoading.SetFont( &m_fntLogin, 1 );

	m_txtStatus.SetFont( &m_fntLogin, 1 );
	m_txtStatusLabel.SetFont( &m_fntLogin, 1 );

	m_txtCredits.SetTextColor( RGB(180,0,0) );
	m_txtLoading.SetTextColor( RGB(180,0,0) );

	m_txtCartTotalLabel.SetFont( &m_fntCredits );
	m_txtCartTotal.SetFont( &m_fntCredits );
	m_txtCartTotal.SetTextColor( RGB(180,0,0) );

	DWORD dwValue = 1;
	if ( !theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "always_display_purchased", &dwValue ) ) dwValue = 1;

	m_chkShowPurchased.SetCheck( dwValue ? 1 : 0 );
	m_btnBack.EnableWindow( FALSE );

	//m_ServerDownloader.SetProgress( &m_ProgressBar, &m_txtStatus );
	m_ServerDownloader.SetProgress( NULL, NULL );
	m_ModelDownloader.SetProgress( &m_ProgressBar, NULL );

	m_Cancel.SetWindowText (			GetLanguageData( _T("TGC Store"), _T("Close") ) );
	m_btnTopUp.SetWindowText (			GetLanguageData( _T("TGC Store"), _T("Register") ) );
	m_btnHome.SetWindowText (			GetLanguageData( _T("TGC Store"), _T("Home") ) );
	m_btnBack.SetWindowText (			GetLanguageData( _T("TGC Store"), _T("Back") ) );
	m_btnDetails.SetWindowText(			GetLanguageData( _T("TGC Store"), _T("Details") ) );
	m_btnLogin.SetWindowText(			GetLanguageData( _T("TGC Store"), _T("Login") ) );
	m_btnBuy.SetWindowText(				GetLanguageData( _T("TGC Store"), _T("Buy") ) );
	m_btnSearch.SetWindowText(			GetLanguageData( _T("TGC Store"), _T("Search") ) );
	m_txtCreditsText.SetWindowText(		GetLanguageData( _T("TGC Store"), _T("Points") ) );
	m_txtLoggedInText.SetWindowText(	GetLanguageData( _T("TGC Store"), _T("Email") ) );
	m_txtCartTotalLabel.SetWindowText(	GetLanguageData( _T("TGC Store"), _T("CartLabel") ) );
	m_txtStatusLabel.SetWindowText(		GetLanguageData( _T("TGC Store"), _T("Status") ) );
	m_chkShowPurchased.SetWindowText(	GetLanguageData( _T("TGC Store"), _T("ShowPurchased") ) );
	m_txtOrderBy.SetWindowText(			GetLanguageData( _T("TGC Store"), _T("OrderBy") ) );
	m_txtItemsPerPage.SetWindowText(	GetLanguageData( _T("TGC Store"), _T("ItemsPerPage") ) );
	m_btnUploadItem.SetWindowText(		GetLanguageData( _T("TGC Store"), _T("UploadItem") ) );
	m_btnPageLeft.SetWindowText(		GetLanguageData( _T("TGC Store"), _T("Prev") ) );
	m_btnPageRight.SetWindowText(		GetLanguageData( _T("TGC Store"), _T("Next") ) );

	while ( m_comboOrderBy.GetCount() )	m_comboOrderBy.DeleteString( 0 );

	m_comboOrderBy.AddString( GetLanguageData( "TGC Store", "SortName" ) );
	m_comboOrderBy.AddString( GetLanguageData( "TGC Store", "SortCostL" ) );
	m_comboOrderBy.AddString( GetLanguageData( "TGC Store", "SortCostH" ) );
	m_comboOrderBy.AddString( GetLanguageData( "TGC Store", "SortRatingL" ) );
	m_comboOrderBy.AddString( GetLanguageData( "TGC Store", "SortRatingH" ) );

	m_comboItemsPerPage.SetCurSel( 2 );

	CFont *pAdFont = new CFont();
	if ( !pAdFont->CreatePointFont( 100, "Calibri" ) ) pAdFont->CreatePointFont( 100, "Arial" );
	m_btnAdBanner.SetFont( pAdFont );
	m_btnAdBanner.Start( );
	delete pAdFont;

	m_Uploader.SetMarketWindow( this );

	SetTimer( 98, 5000, NULL );

	if ( theApp.m_szSESID )
	{
		SetTimer( 6, 1000, NULL );
		SetTimer( 8, 2000, NULL );
	}
	else
	{
		SetTimer( 8, 1000, NULL );
	}

	UpdateCartTotal();

	if ( theApp.m_bGotoAddons )
	{
		SetTimer( 16, 500, 0 );
		
		theApp.m_bGotoAddons = false;
	}

#endif

	//LoadTGCStore();

	return TRUE;
}

void CMarketWindow::OnSelchangedTree ( NMHDR* pNMHDR, LRESULT* pResult )
{
	m_Tree.OnSelchangedTree ( pNMHDR, pResult );

	//int i = m_Tree.GetCurSl

	CString sPath = m_Tree.GetSelectionPath ( );
}

void CMarketWindow::OnItemexpandingTree ( NMHDR* pNMHDR, LRESULT* pResult )
{
	m_Tree.OnItemexpandingTree ( pNMHDR, pResult );
}

#ifdef TGC_STORE
void CMarketWindow::OnClickList2 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	m_MarketImage.OnClickList ( pNMHDR, pResult );

	if ( m_MarketImage.GetItemClicked( ) >= 0 )
	{
		if ( m_MarketImage.GetSelectedCount() > 1 )
		{
			m_btnDetails.EnableWindow( FALSE );
			m_btnBuy.EnableWindow( TRUE );
			m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("AddToCart") ) );
		}
		else
		{
			m_btnDetails.EnableWindow( TRUE );
			m_btnBuy.EnableWindow( TRUE );

			char szText [ 64 ];
			m_MarketImage.GetItemText( m_MarketImage.GetItemClicked( ), 0, szText, 64 );

			//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );
			MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected(), iGroupSelected );
			if ( pObject ) 
			{
				if ( pObject->IsPurchased( ) ) m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Download") ) );
				else m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Buy") ) );
			}
		}
	}
	else
	{
		m_btnDetails.EnableWindow( FALSE );
		m_btnBuy.EnableWindow( FALSE );
		m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Buy") ) );
	}

	if ( iGroupSelected == -2 )
	{
		if ( m_bBuyingCart ) m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("CancelBuy") ) );
		else m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("BuyCart") ) );
		m_btnBuy.EnableWindow( TRUE );
	}
}

void CMarketWindow::OnClickList3 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	if ( m_MarketImage.GetItemClicked( ) >= 0 ) 
	{
		char szText [ 64 ];
		m_MarketImage.GetItemText( m_MarketImage.GetItemClicked( ), 0, szText, 64 );
		char *szSlash = strchr( szText, '\\' );

		if ( szSlash )
		{
			char order [ 32 ];
			m_comboOrderBy.GetWindowText( order, 32 );

			MarketGroup *pGroup = NULL, *pChild = NULL;
			*szSlash = '\0';

			//char str [ 32 ];
			//sprintf_s( str, "%d", iGroupSelected );
			//MessageBox( str );

			m_bThumbnailsLoaded = false;
			
			if ( iGroupSelected > 0 )
			{
				pGroup = MarketObject::GetGroup( iGroupSelected );
				if ( !pGroup || !pGroup->pChildGroup ) return;

				pChild = pGroup->pChildGroup;
			}
			else
			{
				if ( iGroupSelected == -3 ) pChild = MarketObject::GetGroupList( );
				else if ( iGroupSelected == 0 )
				{
					//root menu
					//if ( strcmp( szText, "Search Results" ) == 0 )
					if ( strcmp( szText, GetLanguageData( _T("TGC Store"), _T("SearchResults") ) ) == 0 )
					{
						//m_MarketImage.LoadRemoteImages( -1, m_chkShowPurchased.GetCheck() > 0, order );
						m_Tree.SelectSearchResults( );
						RecordHistory( -1 );
					}
					//else if ( strcmp( szText, "Shopping Cart" ) == 0 )
					else if ( strcmp( szText, GetLanguageData( _T("TGC Store"), _T("ShoppingCart") ) ) == 0 )
					{
						//m_MarketImage.LoadRemoteImages( -2, m_chkShowPurchased.GetCheck() > 0, order );
						m_Tree.SelectShoppingCart( );
						RecordHistory( -2 );
					}
					//else if ( strcmp( szText, "Online Items" ) == 0 )
					else if ( strcmp( szText, GetLanguageData( _T("TGC Store"), _T("OnlineItems") ) ) == 0 )
					{
						//m_MarketImage.LoadRemoteImages( -3, m_chkShowPurchased.GetCheck() > 0, order );
						m_Tree.SelectOnlineItems( );
						RecordHistory( -3 );
					}
					//else if ( strcmp( szText, "Free Items" ) == 0 )
					else if ( strcmp( szText, GetLanguageData( _T("TGC Store"), _T("FreeItems") ) ) == 0 )
					{
						//m_MarketImage.LoadRemoteImages( -4, m_chkShowPurchased.GetCheck() > 0, order );
						m_Tree.SelectFreeItems( );
						RecordHistory( -4 );
					}

					return;
				}
			}

			while ( pChild )
			{
				if ( strcmp( pChild->szName, szText ) == 0 )
				{
					iGroupSelected = pChild->dwGroupID;

					iCurrentPage = 1;
					m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
					RecordHistory( iGroupSelected );
					break;
				}
				
				pChild = pChild->pNextGroup;
			}

			if ( !pChild ) MessageBox( GetLanguageData( _T("TGC Store"), _T("GroupNotFound") ) );
			if ( !pChild ) return;

			HTREEITEM hSelected = m_Tree.GetSelectedItem();
			HTREEITEM hChildItem = m_Tree.GetChildItem( hSelected );
			CString sText;

			while ( hChildItem )
			{
				sText = m_Tree.GetItemText( hChildItem );

				if ( sText.Compare( pChild->szName ) == 0 )
				{
					m_Tree.SelectItem( hChildItem );
					m_Tree.Expand( hChildItem, TVE_EXPAND );
					break;
				}

				hChildItem = m_Tree.GetNextItem( hChildItem, TVGN_NEXT );
			}
		}
		else
		{
			OnBnClickedDetails( );
		}
	}
}

void CMarketWindow::OnRClickList( NMHDR* pNMHDR, LRESULT* pResult )
{
	m_MarketImage.OnRClickList( pNMHDR, pResult, iGroupSelected, this );
}
#endif

void CMarketWindow::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	DWORD dwWidth = 0;
	DWORD dwHeight = 0;

	CRect rectSize;
	CRect rect, mainRect;

	theApp.GetMainWnd()->GetClientRect( &rect );
	//theApp.GetMainWnd()->ClientToScreen( &rect );

	theApp.GetMainWnd()->GetWindowRect( &mainRect );
	theApp.m_iAppX = mainRect.left;
	theApp.m_iAppY = mainRect.top;
	theApp.m_iAppWidth = mainRect.Width();
	theApp.m_iAppHeight = mainRect.Height();

	if ( mainRect.left < 0 ) mainRect.left = 0;
	if ( mainRect.top < 0 ) mainRect.top = 0;
	mainRect.left++;
	mainRect.top++;
	mainRect.right-=2;
	mainRect.bottom-=2;

	cIPC* pIPC			= theApp.m_Message.GetIPC ( );
	
	//pIPC->ReceiveBuffer ( &dwWidth,  704, sizeof ( dwWidth  ) );
	//pIPC->ReceiveBuffer ( &dwHeight, 708, sizeof ( dwHeight ) );

	SetWindowPos ( NULL, mainRect.left, mainRect.top, mainRect.Width(), mainRect.Height(), 0 );
	GetClientRect( &rect );
	
	dwWidth = rect.right - rect.left;
	dwHeight = rect.bottom - rect.top;

	int iTreePercent     = 25;
	int iSplitterPercent = 2;
	int iImagePercent    = 70;

	int iMins = 50;

	int iTreeX          = ( dwWidth  / 100 );
	int iTreeY          = ( dwHeight / 100 );
	int iTreeWidth      = ( dwWidth  / 100 ) * iTreePercent;
	if ( iTreeWidth > 200 ) iTreeWidth = 200;
	int iTreeHeight     = ( rect.bottom - rect.top ) - iMins;
	
	//int iAdBannerWidth = 120;
	int iAdBannerWidth = 1;

	int iSplitterX      = iTreeX + iTreeWidth;
	int iSplitterY      = ( dwHeight / 100  );
	int iSplitterWidth  = 2;
	int iSplitterHeight = ( rect.bottom - rect.top ) - iMins;

	//int iImageX         = iSplitterX + iSplitterWidth;
	int iImageX         = iTreeX + iTreeWidth + 5;
	int iImageY         = ( dwHeight / 100  );
	//int iImageWidth     = ( dwWidth  / 100 * iImagePercent );
	//int iImageWidth     = rect.right - 15;
	//int iImageWidth     = dwWidth - iTreeWidth - 10;
	int iImageWidth     = dwWidth - (iTreeWidth + 20);
	int iImageHeight    = ( rect.bottom - rect.top ) - iMins;

	m_Tree.SetWindowPos     ( NULL, iTreeX, iTreeY, iTreeWidth, iTreeHeight - 41, 0 );
	m_spliter.SetWindowPos  ( NULL, iSplitterX, iSplitterY, iSplitterWidth, iSplitterHeight - 41, 0 );
	//m_Image.SetWindowPos    ( NULL, iImageX, iImageY, iImageWidth, iImageHeight, 0 );

	//m_Ok.SetWindowPos ( NULL, dwWidth - 180, dwHeight - 35, 0, 0, SWP_NOSIZE );
	//m_Cancel.SetWindowPos ( NULL, dwWidth - 90, dwHeight - 35, 0, 0, SWP_NOSIZE );

#ifdef TGC_STORE
	m_WebBrowser.SetWindowPos( NULL, iImageX, iImageY, iImageWidth - (iAdBannerWidth + 7), iImageHeight - 61, 0 );
	m_MarketImage.SetWindowPos( NULL, iImageX, iImageY, iImageWidth - (iAdBannerWidth + 7), iImageHeight - 61, 0 );

	int iPageX = iImageX + (iImageWidth - (iAdBannerWidth + 7) - 360) / 2;
	m_txtItemsPerPage.SetWindowPos( NULL, iPageX, iImageY + (iImageHeight - 57), 0,0, SWP_NOSIZE );

	m_txtItemsPerPage.GetClientRect( rectSize ); iPageX += rectSize.Width() + 1;
	m_comboItemsPerPage.SetWindowPos( NULL, iPageX, iImageY + (iImageHeight - 60), 0,0, SWP_NOSIZE );

	m_comboItemsPerPage.GetClientRect( rectSize ); iPageX += rectSize.Width() + 10;
	m_btnPageFirst.SetWindowPos( NULL, iPageX, iImageY + (iImageHeight - 60), 0,0, SWP_NOSIZE );
	
	m_btnPageFirst.GetClientRect( rectSize ); iPageX += rectSize.Width() + 1;
	m_btnPageLeft.SetWindowPos( NULL, iPageX, iImageY + (iImageHeight - 60), 0,0, SWP_NOSIZE );

	m_btnPageLeft.GetClientRect( rectSize ); iPageX += rectSize.Width() + 1;
	m_txtPageCounter.SetWindowPos( NULL, iPageX, iImageY + (iImageHeight - 57), 0,0, SWP_NOSIZE );

	m_txtPageCounter.GetClientRect( rectSize ); iPageX += rectSize.Width() + 1;
	m_btnPageRight.SetWindowPos( NULL, iPageX, iImageY + (iImageHeight - 60), 0,0, SWP_NOSIZE );

	m_btnPageRight.GetClientRect( rectSize ); iPageX += rectSize.Width() + 1;
	m_btnPageLast.SetWindowPos( NULL, iPageX, iImageY + (iImageHeight - 60), 0,0, SWP_NOSIZE );

	//m_btnAdBanner.SetWindowPos( NULL, (iImageX + iImageWidth) - iAdBannerWidth - 5, iImageY, iAdBannerWidth, iImageHeight + 20, 0 );
	if ( dwWidth < 900 ) m_btnAdBanner.SetWindowPos( NULL, dwWidth - (iAdBannerWidth+7), iImageY, iAdBannerWidth, iImageHeight - 41, 0 );
	else m_btnAdBanner.SetWindowPos( NULL, dwWidth - (iAdBannerWidth+7), iImageY, iAdBannerWidth, iImageHeight + 20, 0 );

	PositionStatusText( );

	m_btnHome.GetClientRect( rectSize );
	int iHomeGroupWidth = rectSize.Width();
	m_btnBack.GetClientRect( rectSize );
	iHomeGroupWidth += rectSize.Width();

	m_btnHome.SetWindowPos( NULL, iImageX+BUTTON_BORDER+1, dwHeight - (81 - BUTTON_BORDER), 0,0, SWP_NOSIZE );
	m_btnHome.GetClientRect( rectSize ); int iBackX = iImageX + rectSize.Width() + 5 + BUTTON_BORDER;
	m_btnBack.SetWindowPos( NULL, iBackX, dwHeight - (81-BUTTON_BORDER), 0,0, SWP_NOSIZE );

	int iHomeHeight = rectSize.Height();
	m_btnUploadItem.GetClientRect( rectSize );
	m_btnUploadItem.SetWindowPos ( NULL, iImageX+BUTTON_BORDER+1, dwHeight - (81-BUTTON_BORDER-iHomeHeight-1), iHomeGroupWidth+4, rectSize.Height(), 0 );
	m_btnUploadItem.ShowWindow( SW_HIDE );

	int iHomeGroupHeight = theApp.m_bUploader ? rectSize.Height() + iHomeHeight + BUTTON_BORDER*2 + 1 : iHomeHeight + BUTTON_BORDER*2;
	m_boxBrowserGroup.SetWindowPos( NULL, iImageX+1, dwHeight - 81, iHomeGroupWidth + BUTTON_BORDER*2 + 4, iHomeGroupHeight, 0 );

	m_Cancel.GetClientRect( rectSize ); int iCloseX = 0;
	if ( dwWidth < 900 ) iCloseX = dwWidth - 14 - rectSize.Width();
	else				 iCloseX = dwWidth - (iAdBannerWidth+14) - rectSize.Width();
	m_Cancel.SetWindowPos ( NULL, iCloseX, dwHeight - (59-BUTTON_BORDER), 0, 0, SWP_NOSIZE );

	m_btnBuy.GetClientRect( rectSize ); int iBuyX = iCloseX - (rectSize.Width() + 5);
	m_btnBuy.SetWindowPos ( NULL, iBuyX, dwHeight - (59-BUTTON_BORDER), 0, 0, SWP_NOSIZE );
	
	m_btnDetails.GetClientRect( rectSize ); int iDetailsX = iBuyX - (rectSize.Width() + 5);
	m_btnDetails.SetWindowPos ( NULL, iDetailsX, dwHeight - (59-BUTTON_BORDER), 0, 0, SWP_NOSIZE );
	
	m_btnSearch.GetClientRect( rectSize ); int iSearchX = iDetailsX - (rectSize.Width() + 5);
	m_btnSearch.SetWindowPos ( NULL, iSearchX, dwHeight - (59-BUTTON_BORDER), 0, 0, SWP_NOSIZE );
	
	if ( dwWidth < 900 ) m_boxDetailsGroup.SetWindowPos( NULL, iSearchX - BUTTON_BORDER, dwHeight - 81, dwWidth - 9 - (iSearchX - BUTTON_BORDER), BUTTON_BORDER + rectSize.Height() + 24, 0 );
	else				 m_boxDetailsGroup.SetWindowPos( NULL, iSearchX - BUTTON_BORDER, dwHeight - 81, dwWidth - (iAdBannerWidth+9) - (iSearchX - BUTTON_BORDER), BUTTON_BORDER + rectSize.Height() + 24, 0 );


	//m_chkShowPurchased.GetClientRect( rectSize ); int iPurchasedX = dwWidth - (iAdBannerWidth+12) - rectSize.Width();
	m_chkShowPurchased.SetWindowPos( NULL, iSearchX+2, dwHeight - (79 - BUTTON_BORDER), 0,0, SWP_NOSIZE );

	m_comboOrderBy.GetClientRect( rectSize ); int iOrderX = 0;
	if ( dwWidth < 900 ) iOrderX = dwWidth - 14 - rectSize.Width();
	else				 iOrderX = dwWidth - (iAdBannerWidth+14) - rectSize.Width();
	m_comboOrderBy.SetWindowPos( NULL, iOrderX, dwHeight - (81 - BUTTON_BORDER), 0,0, SWP_NOSIZE );

	m_txtOrderBy.GetClientRect( rectSize ); int iOrderTextX = iOrderX - (rectSize.Width() + 6);
	m_txtOrderBy.SetWindowPos( NULL, iOrderTextX, dwHeight - (77 - BUTTON_BORDER), 0,0, SWP_NOSIZE );

	if ( dwWidth < 900 ) m_boxStatusGroup.SetWindowPos( NULL, iTreeX, dwHeight - 29, dwWidth - 9 - iTreeX, 22, 0 ); 
	else				 m_boxStatusGroup.SetWindowPos( NULL, iTreeX, dwHeight - 29, dwWidth - (iAdBannerWidth+9) - iTreeX, 22, 0 ); 
	
	int iLoggedInTextX = 10;
	m_txtLoggedInText.SetWindowPos ( NULL, iLoggedInTextX, dwHeight - 25, 0, 0, SWP_NOSIZE );
	m_txtLoggedInText.GetClientRect( rectSize ); int iTextX = iLoggedInTextX + rectSize.Width() + 2;
	m_txtLoading.SetWindowPos ( NULL, iTextX, dwHeight - 27, 0, 0, SWP_NOSIZE );

	m_txtLoading.GetClientRect( rectSize ); int iCreditsTextX = iTextX + rectSize.Width() + 2;
	m_txtCreditsText.SetWindowPos ( NULL, iCreditsTextX, dwHeight - 25, 0, 0, SWP_NOSIZE );
	
	m_txtCreditsText.GetClientRect( rectSize ); int iCreditsText2X = iCreditsTextX + rectSize.Width() + 2;
	m_txtCredits.SetWindowPos ( NULL, iCreditsText2X, dwHeight - 27, 0, 0, SWP_NOSIZE );

	m_txtCredits.GetClientRect( rectSize ); int iCartTextX = iCreditsText2X + rectSize.Width() + 2;
	m_txtCartTotalLabel.SetWindowPos( NULL, iCartTextX, dwHeight - 25, 0, 0, SWP_NOSIZE );

	m_txtCartTotalLabel.GetClientRect( rectSize ); int iCartX = iCartTextX + rectSize.Width() + 2;
	m_txtCartTotal.SetWindowPos( NULL, iCartX, dwHeight - 27, 0, 0, SWP_NOSIZE );

	if ( theApp.m_szCredits ) 
	{
		m_txtCredits.SetTextColor( RGB(0,180,0) );
		m_txtCredits.SetWindowText( theApp.m_szCredits );
	}
	else 
	{
		m_txtCredits.SetTextColor( RGB(180,0,0) );
		m_txtCredits.SetWindowText( "0" );
	}

	m_txtCredits.Invalidate();

	m_btnLogin.GetClientRect( rectSize ); 
	int iLoginSizeX = iTreeWidth < 160 + (BUTTON_BORDER*2 + 4) ? (iTreeWidth-(BUTTON_BORDER*2 + 4))/2 : 80;
	int iLoginX = iTreeX + iTreeWidth/2 - iLoginSizeX - 2;
	m_btnLogin.SetWindowPos ( NULL, iLoginX, dwHeight - (81 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );
	//m_boxLoginGroup.SetWindowPos( NULL, iLoginX - BUTTON_BORDER, dwHeight - 81, iLoginSizeX*2 + BUTTON_BORDER*2 + 4, rectSize.Height() + BUTTON_BORDER*2, 0 );
	m_boxLoginGroup.SetWindowPos( NULL, iTreeX, dwHeight - 81, iTreeWidth, rectSize.Height() + BUTTON_BORDER*2, 0 );

	int iTopUpX = iTreeX + iTreeWidth/2 + 2;
	m_btnTopUp.SetWindowPos ( NULL, iTopUpX, dwHeight - (81 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );
	m_btnTopUp.ShowWindow( SW_SHOW );

	if ( theApp.m_szSESID ) 
	{
		m_txtLoading.SetTextColor( RGB(0,180,0) );
		m_txtLoading.SetWindowText( theApp.m_sUsername ); 
		m_btnLogin.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Logout") ) );

		if ( theApp.m_bUploader ) m_btnUploadItem.ShowWindow( SW_SHOW );
		else m_btnUploadItem.ShowWindow( SW_HIDE );

		m_btnTopUp.SetWindowText( GetLanguageData( _T("TGC Store"), _T("TopUp") ) );
	}
	else 
	{
		m_txtLoading.SetTextColor( RGB(180,0,0) );
		m_txtLoading.SetWindowText( GetLanguageData( _T("TGC Store"), _T("NotLoggedIn") ) );
		m_btnLogin.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Login") ) );

		/*
		m_btnLogin.GetClientRect( rectSize ); 
		int iLoginSizeX = iTreeWidth < 80 ? iTreeWidth : 80;
		int iLoginX = iTreeX + iTreeWidth/2 - iLoginSizeX/2;
		m_btnLogin.SetWindowPos ( NULL, iLoginX, dwHeight - (78 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );
		*/

		m_btnTopUp.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Register") ) );
		//m_btnTopUp.ShowWindow( SW_HIDE );
		m_btnUploadItem.ShowWindow( SW_HIDE );
	}

	m_txtLoading.Invalidate( );

	m_ProgressBar.GetClientRect( rectSize ); int iProgressX = 0;
	if ( dwWidth < 900 ) 
	{
		iProgressX = dwWidth - 14 - 100;
		m_ProgressBar.SetWindowPos( NULL, iProgressX, dwHeight - 27, 100,rectSize.Height(), 0 );
		rectSize.right = rectSize.left + 100;
	}
	else
	{
		iProgressX = dwWidth - (iAdBannerWidth+14) - rectSize.Width();
		m_ProgressBar.SetWindowPos( NULL, iProgressX, dwHeight - 27, 0,0, SWP_NOSIZE );
	}

	m_txtStatus.GetClientRect( rectSize ); int iStatusX = iProgressX - (rectSize.Width() + 3);
	m_txtStatus.SetWindowPos( NULL, iStatusX, dwHeight - 25, 0,0, SWP_NOSIZE );

	m_txtStatusLabel.GetClientRect( rectSize ); int iStatusLabelX = iStatusX - (rectSize.Width() + 6);
	m_txtStatusLabel.SetWindowPos( NULL, iStatusLabelX, dwHeight - 25, 0,0, SWP_NOSIZE );
	
	/*
	m_chkShowPurchased.SetWindowPos( NULL, dwWidth - 145, 4, 0,0, SWP_NOSIZE );
	m_comboOrderBy.SetWindowPos( NULL, dwWidth - 265, 1, 0,0, SWP_NOSIZE );
	m_txtOrderBy.SetWindowPos( NULL, dwWidth - 330, 5, 0,0, SWP_NOSIZE );
	*/

	m_comboOrderBy.SetCurSel( 0 );
	m_comboOrderBy.SetExtendedUI( TRUE );

	CString sText;

	for ( int i = 0 ; i < m_comboOrderBy.GetCount(); i++ )
	{
		m_comboOrderBy.GetLBText( i, sText );
		if ( sText.Compare( theApp.m_szOrderBy ) == 0 ) 
		{
			m_comboOrderBy.SetCurSel( i );
			break;
		}
	}

	SwitchToMarketPlace( );
	//SetTimer( 14, 1000, NULL );

#endif

	//m_Image.UpdateWindow ( );

	theApp.GetMainWnd()->SetWindowPos( NULL, 0,rect.bottom, 0,0, 0 );
}

#ifdef TGC_STORE
void CMarketWindow::OnCustomdrawList( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pCustom = (NMLVCUSTOMDRAW*) pNMHDR;
    
    *pResult = CDRF_DODEFAULT;

	if ( pCustom->nmcd.dwDrawStage == CDDS_PREPAINT )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( pCustom->nmcd.dwDrawStage == CDDS_ITEMPREPAINT )
	{
		*pResult = CDRF_NOTIFYPOSTPAINT;

		int nItem = (int) pCustom->nmcd.dwItemSpec;
		//char szText [ 64 ];
		//m_MarketImage.GetItemText( nItem, 0, szText, 64 );
		//if ( strchr( szText, '<' ) ) return;
		//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );
		MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjByIndex( nItem ), iGroupSelected );

		if ( pObject && ( !pObject->IsApproved() || !pObject->IsAvailable() ) )
		{
			CDC* pDC = CDC::FromHandle ( pCustom->nmcd.hdc );
			
			CRect rcItem;
			m_MarketImage.GetItemRect ( nItem, &rcItem, LVIR_ICON );
			rcItem.InflateRect( -5,2 );

			COLORREF color = pObject->IsAvailable() ? RGB(240,206,14) : RGB(226,61,53);

			CBrush brush; brush.CreateSolidBrush( color );
			CPen pen; pen.CreatePen( PS_SOLID, 1, color );

			CPen *oldPen = pDC->SelectObject( &pen );
			CBrush *oldBrush = pDC->SelectObject( &brush );

			pDC->Rectangle( rcItem );

			pDC->SelectObject( oldBrush );
			pDC->SelectObject( oldPen );
		}

		if ( pObject )
		{
			MarketGroup *pGroup = MarketObject::GetGroup( pObject->GetGroupID() );
			if ( pGroup )
			{
				if ( strcmp( pGroup->szName, "Add-ons" ) == 0 )
				{
					if ( pObject->IsPurchased() || !g_bFreeVersion )
					{
						pCustom->clrText = RGB(150,150,150);
									
						*pResult = CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT;
					}
				}
				else
				{
					if ( pObject->IsPurchased() )
					{
						pCustom->clrText = RGB(150,150,150);
									
						*pResult = CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT;
					}
				}
			}
		}
	}
	else if ( pCustom->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT )
	{
		if ( m_ServerDownloader.IsDownloading( ) && m_ServerDownloader.IsModelListDownload( ) )
		{
			//*pResult = CDRF_SKIPDEFAULT;
			SetTimer( 9, 200, NULL );
			return;
		}

		//item paint
		LVITEM   rItem;
		int      nItem = (int) pCustom->nmcd.dwItemSpec;
		CDC*     pDC   = CDC::FromHandle ( pCustom->nmcd.hdc );
		//COLORREF crBkgnd;
		//BOOL     bListHasFocus;
		CRect    rcItem;
		CRect    rcText;
		CRect    rcBounds;
		CString  sText;
		//UINT     uFormat;
		                
		//bListHasFocus = ( m_Image.GetSafeHwnd() == ::GetFocus() );

		ZeroMemory ( &rItem, sizeof(LVITEM) );
		rItem.mask  = LVIF_IMAGE | LVIF_STATE;
		rItem.iItem = nItem;
		rItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		m_MarketImage.GetItem ( &rItem );

		//char szText [ 64 ];
		//m_MarketImage.GetItemText( nItem, 0, szText, 64 );
		//if ( strchr( szText, '<' ) ) return;
		
		m_MarketImage.GetItemRect ( nItem, &rcBounds, LVIR_BOUNDS );
		m_MarketImage.GetItemRect ( nItem, &rcItem, LVIR_ICON );
		m_MarketImage.GetItemRect ( nItem, &rcText, LVIR_LABEL );

//		rcText.InflateRect( 2,2 );
//		pDC->Rectangle( rcText );
//		*pResult = 0;
//		return;
		
		CRect rcLabel( rcItem );
		rcLabel.top = rcLabel.bottom + 1;
		rcLabel.bottom = rcLabel.top + 15;
		rcLabel.left = rcBounds.left;
		rcLabel.right = rcBounds.right;

		//CRect rcCost( rcLabel );
		//rcCost.OffsetRect( 0,15 );
		CRect rcCost( rcText );
		rcCost.OffsetRect( 0,rcText.Height( ) );
		rcCost.bottom = rcCost.top + 15;
		rcCost.left = rcBounds.left;
		rcCost.right = rcBounds.right;

		CRect rcRating( rcCost );
		rcRating.OffsetRect( 0,15 );

		int diff = rcRating.Width( ) - 5*15;
		rcRating.DeflateRect( diff/2, 0, diff/2, 0 );
		rcRating.right = rcRating.left + 15;

		//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );
		MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjByIndex( nItem ), iGroupSelected );
		if ( !pObject ) 
		{
			//*pResult = CDRF_SKIPDEFAULT;
			return;
		}

		CBitmap *oldBitmap;
		CDC bmpDC;
		bmpDC.CreateCompatibleDC( pDC );
		oldBitmap = bmpDC.SelectObject( &m_bmpInCart );

		if ( MarketObject::IsObjectInCart( pObject->GetID() ) )
		{
			CRect rcCartIcon;
			rcCartIcon = rcItem;
			int mid = rcItem.left + rcItem.Width()/2;
			rcCartIcon.right = mid + 32;
			rcCartIcon.top = rcCartIcon.bottom - 17;
			rcCartIcon.left = rcCartIcon.right - 17;

			pDC->BitBlt( rcCartIcon.left, rcCartIcon.top, 17, 17, &bmpDC, 0,0, SRCCOPY );
		}

		bmpDC.SelectObject( &m_bmpEmptyStar );

		if ( pObject->GetRatingCount( ) > 0 )
		{
			float fRating = pObject->GetTotalRating() / (float) pObject->GetRatingCount( );
			int rating = (int) floor( fRating );
			for ( int i = 0; i < rating; i++ )
			{
				pDC->BitBlt( rcRating.left, rcRating.top, 15, 15, &bmpDC, 0,0, SRCCOPY );
				rcRating.OffsetRect( 15,0 );
			}

			bmpDC.SelectObject( &m_bmpStar );

			for ( int i = rating; i < 5; i++ )
			{
				pDC->BitBlt( rcRating.left, rcRating.top, 15, 15, &bmpDC, 0,0, SRCCOPY );
				rcRating.OffsetRect( 15,0 );
			}

			//half stars
			bmpDC.SelectObject( m_bmpEmptyStar );

			CRect rcRating( rcCost );
			rcRating.OffsetRect( 0,15 );

			diff = rcRating.Width( ) - 5*15;
			rcRating.DeflateRect( diff/2, 0, diff/2, 0 );
			rcRating.right = rcRating.left + 15;
			for ( int i = 0; i < rating; i++ ) { rcRating.OffsetRect( 15,0 ); }

			if ( rating < 5 )
			{
				float fDiff = fRating - (float) rating;
				int width = (int) (15.0f*fDiff);

				pDC->BitBlt( rcRating.left, rcRating.top, width, 15, &bmpDC, 0,0, SRCCOPY );
			}
		}
		else
		{
			rcRating.CopyRect( &rcCost );
			rcRating.OffsetRect( 0,15 );

			if ( pObject->IsPurchased() ) pDC->SetTextColor( RGB(205,128,128) );
			else pDC->SetTextColor( RGB(155,0,0) );
			pDC->DrawText ( GetLanguageData( _T("TGC Store"), _T("NotRated") ), rcRating, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
			pDC->SetTextColor( RGB(0,0,0) );
		}

		bmpDC.SelectObject( oldBitmap );

		// Draw the background of the list item.  Colors are selected 
		// according to the item's state.
/*
		if ( rItem.state & LVIS_SELECTED )
		{
			if ( bListHasFocus )
			{
				crBkgnd = GetSysColor ( COLOR_HIGHLIGHT );
				pDC->SetTextColor ( GetSysColor ( COLOR_HIGHLIGHTTEXT ));
			}
			else
			{
				crBkgnd = GetSysColor ( COLOR_BTNFACE );
				pDC->SetTextColor ( GetSysColor ( COLOR_BTNTEXT ));
			}
		}
		else
		{
			crBkgnd = GetSysColor ( COLOR_WINDOW );
			pDC->SetTextColor ( GetSysColor ( COLOR_BTNTEXT ));
		}

		// Draw the background & prep the DC for the text drawing.  Note
		// that the entire item RECT is filled in, so this emulates the full-
		// row selection style of normal lists.
		pDC->FillSolidRect ( rcItem, crBkgnd );
		pDC->SetBkMode ( TRANSPARENT );
*/
		pDC->SetTextColor( RGB(0,0,0) );

		MarketGroup *pGroup = MarketObject::GetGroup( pObject->GetGroupID() );
		if ( pGroup && strcmp( pGroup->szName, "Add-ons" ) == 0 )
		{
			if ( g_bFreeVersion )
			{
				if ( pObject->IsPurchased() )
				{
					// purchased but not installed
					pDC->SetTextColor( RGB(128,128,205) );
					sText =  GetLanguageData( _T("TGC Store"), _T("Purchased") );
				}
				else
				{
					//  not purchased yet
					pDC->SetTextColor( RGB(0,155,0) );
					if ( pObject->GetCost() == 0 ) sText = GetLanguageData( _T("TGC Store"), _T("Free") );
					else sText.Format( "%s %d", GetLanguageData( _T("TGC Store"), _T("Cost") ), pObject->GetCost( ) );
				}
			}
			else
			{
				// installed, all is fine - does not matter if purchased or not
				pDC->SetTextColor( RGB(128,128,205) );
				sText =  GetLanguageData( _T("TGC Store"), _T("Installed") );
			}
		}
		else
		{
			if ( pObject->IsPurchased() )
			{
				pDC->SetTextColor( RGB(128,128,205) );
				sText =  GetLanguageData( _T("TGC Store"), _T("Purchased") );
			}
			else
			{
				pDC->SetTextColor( RGB(0,155,0) );
				if ( pObject->GetCost() == 0 ) sText = GetLanguageData( _T("TGC Store"), _T("Free") );
				else sText.Format( "%s %d", GetLanguageData( _T("TGC Store"), _T("Cost") ), pObject->GetCost( ) );
			}
		}

/*
		if ( ( rItem.state & LVIS_SELECTED ) && bListHasFocus ) uFormat |= ILD_FOCUS;
		uFormat = ILD_TRANSPARENT;

		IMAGEINFO desc;
		POINT ptTopLeft;
		
		m_Image.m_Images.GetImageInfo( rItem.iImage, &desc );
		int diff = rcItem.Width( ) - ( desc.rcImage.right - desc.rcImage.left );
		if ( diff < 0 ) diff = 0;
		ptTopLeft.x = rcItem.left + diff/2;

		diff = rcItem.Height( ) - ( desc.rcImage.bottom - desc.rcImage.top );
		if ( diff < 0 ) diff = 0;
		ptTopLeft.y = rcItem.top + diff/2;

		m_Image.m_Images.Draw( pDC, rItem.iImage, ptTopLeft, uFormat );

		pDC->DrawText ( m_Image.GetItemText ( nItem, 0 ), rcLabel, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		*/
		//pDC->SetTextColor( RGB(0,155,0) );
		pDC->DrawText ( sText, rcCost, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		pDC->SetTextColor( RGB(0,0,0) );

		*pResult = CDRF_SKIPDEFAULT;
	}
}

void CMarketWindow::OnCustomdrawTree( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMTVCUSTOMDRAW* pCustom = (NMTVCUSTOMDRAW*) pNMHDR;
    
    *pResult = CDRF_DODEFAULT;

	if ( pCustom->nmcd.dwDrawStage == CDDS_PREPAINT )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( pCustom->nmcd.dwDrawStage == CDDS_ITEMPREPAINT )
	{
		/*
		if ( strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "TGC Store" ) == 0
		  || strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Search Results" ) == 0
		  || strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Shopping Cart" ) == 0
		  || strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Online Items" ) == 0 )
		{
			*pResult = CDRF_NOTIFYPOSTPAINT;
		}
		*/

		static CString sHome = GetLanguageData( "TGC Store", "Home" );
		static CString sSearchResults = GetLanguageData( "TGC Store", "SearchResults" );
		static CString sShoppingCart = GetLanguageData( "TGC Store", "ShoppingCart" );
		static CString sFreeItems = GetLanguageData( "TGC Store", "FreeItems" );
		static CString sOnlineItems = GetLanguageData( "TGC Store", "OnlineItems" );

		bool bNewFont = false;

		// leefix - 210508 - prevents artist name that starts with HOME being falsely recoloured
		//if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Home", strlen("Home") ) == 0 )
		//if ( strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Home" ) == 0 )
		if ( strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), sHome ) == 0 )
		{
			pCustom->clrText = RGB(69,70,70);
			bNewFont = true;
		}

		//if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Search Results", strlen("Search Results") ) == 0 )
		if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), sSearchResults, sSearchResults.GetLength() ) == 0 )
		{
			if ( iSearchFlashMode == 0 )
			{
				pCustom->clrText = RGB(0,82,165);
			}
			else
			{
				pCustom->clrText = RGB(100,182,255);
			}
			bNewFont = true;
		}

		//if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Shopping Cart", strlen("Shopping Cart") ) == 0 )
		if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), sShoppingCart, sShoppingCart.GetLength() ) == 0 )
		{
			if ( iCartFlashMode == 0 ) 
			{
				pCustom->clrText = RGB(0,165,82);
			}
			else 
			{
				pCustom->clrText = RGB(100,255,182);
			}
			bNewFont = true;
		}

		//if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Free Items", strlen("Free Items") ) == 0 )
		if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), sFreeItems, sFreeItems.GetLength() ) == 0 )
		{
			pCustom->clrText = RGB(198,159,26);
			bNewFont = true;
		}

		//if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Online Items", strlen("Online Items") ) == 0 )
		if ( strncmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), sOnlineItems, sOnlineItems.GetLength() ) == 0 )
		{
			pCustom->clrText = RGB(247,99,34);
			bNewFont = true;
		}

		if ( bNewFont )
		{
			*pResult = CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT;
		}
	}
	else if ( pCustom->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT )
	{
		/*
		CDC* pDC = CDC::FromHandle ( pCustom->nmcd.hdc );
		CRect rect = pCustom->nmcd.rc;

		CBitmap bmp;
		int iOffset = 38;
		if ( strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "TGC Store" ) == 0 ) { bmp.LoadBitmap( IDB_STORE_ICON ); iOffset = 19; }
		else if ( strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Search Results" ) == 0 ) bmp.LoadBitmap( IDB_SEARCH_RESULTS );
		else if ( strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Shopping Cart" ) == 0 ) bmp.LoadBitmap( IDB_SHOPPING_CART );
		else if ( strcmp( m_Tree.GetItemText( (HTREEITEM)pCustom->nmcd.dwItemSpec ), "Online Items" ) == 0 ) bmp.LoadBitmap( IDB_ONLINE_ITEMS );
		else return;

		if ( m_Tree.GetItemState( (HTREEITEM)pCustom->nmcd.dwItemSpec, TVIS_SELECTED ) ) iOffset += 2;
		*/

		/*
		CDC dc;
		dc.CreateCompatibleDC( pDC );
		CBitmap *oldBmp = dc.SelectObject( &bmp );

		pDC->BitBlt( rect.left + iOffset, rect.top, rect.Width(), rect.Height(), &dc, 0,0, SRCCOPY );

		dc.SelectObject( oldBmp );
		bmp.DeleteObject();
		*/
		
		//pDC->SetTextColor( RGB(0,155,0) );
		//pDC->DrawText ( "test", &(pCustom->nmcd.rc), DT_SINGLELINE );
		//pDC->SetTextColor( RGB(0,0,0) );

		CDC* pDC = CDC::FromHandle ( pCustom->nmcd.hdc );
		int y = pCustom->nmcd.rc.bottom - 1;
		CPen pen;
		pen.CreatePen( PS_SOLID, 1, RGB(199,204,204) );
		CPen* oldPen = pDC->SelectObject( &pen );
		pDC->MoveTo( 0,y );
		pDC->LineTo( 1000,y );
		pDC->SelectObject( oldPen );
	}
}
#endif

void CMarketWindow::OnOK() 
{
	// TODO: Add extra validation here

#ifdef TGC_STORE
	if ( theApp.m_szSESID )
	{
		/*
		delete [] theApp.m_szSESID; 
		theApp.m_szSESID = NULL;
		
		if ( theApp.m_szCredits )
		{
			delete [] theApp.m_szCredits;
			theApp.m_szCredits = NULL;
		}
		*/
	}
#endif
	
	CDialog ::OnOK();
}

void CMarketWindow::OnCancel() 
{
	if ( m_ModelDownloader.IsDownloading() )
	{
		m_txtStatus.SetWindowText( "Stopping..." );

		//m_ModelDownloader.SetProgress( NULL, NULL );
		m_ModelDownloader.Cancel();
		//m_ModelDownloader.Join();
		//m_ModelDownloader.SetProgress( &m_ProgressBar, NULL );
		
		m_Cancel.SetWindowText( GetLanguageData( "TGC Store", "Close" ) );
		m_ProgressBar.SetPos( 0 );
	}
	else
	{
		m_txtStatus.SetWindowText( "Closing..." );

		bool bWait = false;

		if ( m_Uploader.IsRunning() )
		{
			m_Uploader.OnBnClickedCancel( );
			//m_Uploader.Join();
			bWait = true;
		}

		if ( m_ServerDownloader.IsDownloading() )
		{
			//m_ServerDownloader.SetProgress( NULL, NULL );
			m_ServerDownloader.Cancel();
			//m_ServerDownloader.Join();
			bWait = true;
		}
		
		if ( bWait ) 
		{
			m_iCloseWaitTimer = 0;
			SetTimer( 99, 500, NULL );
		}
		else CDialog::OnCancel();
	}
}

#ifdef TGC_STORE
MarketObject* CMarketWindow::GetSelectedObject( )
{
	if ( m_MarketImage.GetSelectedCount( ) > 1 )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("TooManySelect") ) );
		return NULL;
	}

	int item = m_MarketImage.GetItemClicked( );
	if ( item < 0 )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("SelectError") ) );
		return NULL;
	}

	//char szText [ 64 ];
	//m_MarketImage.GetItemText( item, 0, szText, 64 );

	//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );
	MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected(), iGroupSelected );
	if ( !pObject )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return NULL;
	}

	return pObject;
}

void CMarketWindow::OnBnClickedDetails()
{
	CObjectDetailsDialog *pDetailsWindow = new CObjectDetailsDialog( );

	if ( m_MarketImage.GetSelectedCount( ) > 1 )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("TooManySelect") ) );
		return;
	}

	int item = m_MarketImage.GetItemClicked( );
	if ( item < 0 )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("SelectError") ) );
		return;
	}

	//char szText [ 64 ];
	//m_MarketImage.GetItemText( item, 0, szText, 64 );
	//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

	MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected(), iGroupSelected );
	if ( !pObject )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		return;
	}

	/*
	if ( !pObject->IsComplete( ) )
	{
		HTTPComm server;
		if ( !server.GetModelDetails( pObject->GetID(), theApp.m_szSESID ) )
		{
			MessageBox( server.GetLastError(), "Server Error" );
		}
		pObject->CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );
	}
	*/

	//MessageBox( pObject->GetName( ) );

	pDetailsWindow->SetBitmaps( &m_bmpStar, &m_bmpEmptyStar, &m_bmpMaskStar );	//reversed
	pDetailsWindow->SetValues( pObject, &m_MarketImage, this, iGroupSelected );
	int result = pDetailsWindow->DoModal( );

	if ( result == IDOK && pDetailsWindow->IsBought( ) )
	{
		//m_MarketImage.GetItemText( pDetailsWindow->GetItemSelected(), 0, szText, 64 );
		//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

		MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjByIndex( pDetailsWindow->GetItemSelected() ), iGroupSelected );
		if ( !pObject )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
			return;
		}

		if ( !theApp.m_szSESID ) MessageBox( GetLanguageData( _T("TGC Store"), _T("MustLogin") ), theApp.m_szErrorString ); 
		else
		{
			//reduce credits
			if ( m_ModelDownloader.IsDownloading( ) ) MessageBox( GetLanguageData( _T("TGC Store"), _T("OnlyOne") ) );
			else
			{
				if ( !pObject->IsPurchased( ) && pObject->GetCost() > 0 )
				{
					char format [ 256 ];
					strcpy_s( format, 256, GetLanguageData( _T("TGC Store"), _T("ConfirmPurchase") ) );

					char str [ 256 ];
					sprintf_s( str, 256, format, pObject->GetName(), pObject->GetCost() );
					int result = MessageBox( str, GetLanguageData( _T("TGC Store"), _T("Confirm") ), MB_YESNO | MB_ICONQUESTION );
					if ( result != IDYES ) return;
				}

				m_sDownloadingModelName = pObject->GetName();

				m_ModelDownloader.SetModelDownload( "Files\\TGCStore\\TEMP\\Model.zip", pObject, pObject->GetCost( ), this );
				m_ModelDownloader.Start( );

				m_Cancel.SetWindowText( GetLanguageData( "TGC Store", "Cancel" ) );

				m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloading") ) );
				SetTimer( 7, 2000, NULL );

				//MessageBox( "Download started" );
			}
		}
	}

	m_MarketImage.Invalidate();

	if ( pDetailsWindow->IsCartChanged() )
	{
		int count = MarketObject::CountCartObjects();
		m_Tree.SetCartCount( count );
		UpdateCartTotal( );

		if ( count > 0 )
		{
			iCartFlashCounter = 5;
			SetTimer( 2, 500, NULL );
		}
	}

	delete pDetailsWindow;
}

void CMarketWindow::ShowObjectDetails( DWORD dwID )
{
	CObjectDetailsDialog *pDetailsWindow = new CObjectDetailsDialog( );

	bool bCreated = false;
	MarketObject *pObject = MarketObject::GetObjectByID( dwID, 0 );
	if ( !pObject )
	{
		//MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
		char str [ 12 ];
		sprintf_s( str, 12, "%d", dwID );
		
		pObject = new MarketObject();
		pObject->SetValue( "object_id", str );
		bCreated = true;
		//return;
	}
	

	/*
	if ( !pObject->IsComplete( ) )
	{
		HTTPComm server;
		if ( !server.GetModelDetails( pObject->GetID(), theApp.m_szSESID ) )
		{
			MessageBox( server.GetLastError(), "Server Error" );
		}
		pObject->CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );
	}
	*/

	//MessageBox( pObject->GetName( ) );

	pDetailsWindow->SetBitmaps( &m_bmpStar, &m_bmpEmptyStar, &m_bmpMaskStar );	//reversed
	pDetailsWindow->SetValues( pObject, &m_MarketImage, this, iGroupSelected, true );
	int result = pDetailsWindow->DoModal( );

	if ( result == IDOK && pDetailsWindow->IsBought( ) )
	{
		//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );
		if ( !theApp.m_szSESID ) MessageBox( GetLanguageData( _T("TGC Store"), _T("MustLogin") ), theApp.m_szErrorString ); 
		else
		{
			if ( m_ModelDownloader.IsDownloading( ) ) MessageBox( GetLanguageData( _T("TGC Store"), _T("OnlyOne") ) );
			else
			{
				if ( !pObject->IsPurchased( ) && pObject->GetCost() > 0 )
				{
					char format [ 256 ];
					strcpy_s( format, 256, GetLanguageData( _T("TGC Store"), _T("ConfirmPurchase") ) );

					char str [ 256 ];
					sprintf_s( str, 256, format, pObject->GetName(), pObject->GetCost() );
					int result = MessageBox( str, GetLanguageData( _T("TGC Store"), _T("Confirm") ), MB_YESNO | MB_ICONQUESTION );
					if ( result != IDYES ) return;
				}

				m_sDownloadingModelName = pObject->GetName( );

				m_ModelDownloader.SetModelDownload( "Files\\TGCStore\\TEMP\\Model.zip", pObject, pObject->GetCost( ), this );
				m_ModelDownloader.Start( );

				m_Cancel.SetWindowText( GetLanguageData( "TGC Store", "Cancel" ) );

				m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloading") ) );
				SetTimer( 7, 2000, NULL );

				//MessageBox( "Download started" );
			}
		}
	}

	m_MarketImage.Invalidate();

	if ( pDetailsWindow->IsCartChanged() )
	{
		int count = MarketObject::CountCartObjects();
		m_Tree.SetCartCount( count );
		UpdateCartTotal( );

		if ( count > 0 )
		{
			iCartFlashCounter = 5;
			SetTimer( 2, 500, NULL );
		}
	}

	delete pDetailsWindow;

	if ( bCreated ) delete pObject;
}

void CMarketWindow::ClearObjectsDir( int group, bool bClearPurchased )
{
	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !SetCurrentDirectory( theApp.m_szDirectory ) ) 
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return;
	}
	if ( !SetCurrentDirectory( "Files\\TGCStore\\TEMP" ) ) 
	{
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
		return;
	}

	if ( group >= 0 )
	{
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
						if ( strncmp( FindFileData.cFileName, "ObjectList", strlen("ObjectList") ) == 0 )
						{ 
							if ( group <= 0 ) DeleteFile( FindFileData.cFileName );
							else
							{
								//only delete specified group
								const char *szObjectGroup = FindFileData.cFileName + strlen("ObjectList");
								char szID [ 12 ];
								sprintf_s( szID, 12, "%d.", group );

								if ( strncmp( szObjectGroup, szID, strlen(szID) ) == 0 ) 
								{
									DeleteFile( FindFileData.cFileName );
									break;
								}
							}
						}
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);

			FindClose(hFind);
		}
	}

	if ( group <= 0 || bClearPurchased )
	{
		CString sFilename = theApp.m_szDirectory;
		sFilename += "\\Files\\TGCStore\\TEMP\\PurchasedObjects.xml";
		DeleteFile( sFilename );
	}

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );
}

void CMarketWindow::Logout( )
{
	if ( m_bLoggingOut || m_bLoggingIn ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("LogoutBusy") ), theApp.m_szErrorString );
		return;
	}

	if ( m_Uploader.IsUploading() ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("LogoutUploading") ), theApp.m_szErrorString );
		return;
	}

	if ( m_bBuyingCart || m_ModelDownloader.IsDownloading() ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("LogoutDownloading") ), theApp.m_szErrorString );
		return;
	}

	m_bLoggingOut = true;
	KillTimer( 6 );
	strcpy_s( szUsername, 50, "" );
	for ( int i = 0; i < 50; i++ ) szPassword [ i ] = '\0';

	HTTPComm server;
	server.Logout();

	strcpy_s( theApp.m_szAuthorName, 30, "" );
	if ( !theApp.m_bSavePassword ) theApp.m_sPassword = "";

	m_txtLoading.SetTextColor( RGB(180,0,0) );
	m_txtLoading.SetWindowText( GetLanguageData( _T("TGC Store"), _T("NotLoggedIn") ) );
	m_txtLoading.Invalidate( );

	m_txtCredits.SetTextColor( RGB(180,0,0) );
	m_txtCredits.SetWindowText( "0" );
	m_txtCredits.Invalidate( );

	m_btnLogin.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Login") ) );
	
	CRect rect, treeRect, rectSize;
	//theApp.m_pEditorView->GetWindowRect ( &rect );
	GetClientRect( &rect );
	ClientToScreen( &rect );
	m_Tree.GetWindowRect( &treeRect );
	ScreenToClient( treeRect );
	DWORD dwHeight = rect.bottom - rect.top;

	/*
	m_btnLogin.GetClientRect( rectSize ); 
	int iLoginSizeX = treeRect.Width() < 80 ? treeRect.Width() : 80;
	int iLoginX = treeRect.left + treeRect.Width()/2 - iLoginSizeX/2;
	m_btnLogin.SetWindowPos ( NULL, iLoginX, dwHeight - (78 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );

	m_btnUploadItem.ShowWindow( SW_HIDE );
	m_chkShowPurchased.ShowWindow( SW_HIDE );
	m_btnTopUp.ShowWindow( SW_HIDE );
	*/

	int iHomeGroupHeight = 0;
	
	m_btnHome.GetClientRect( rectSize );
	iHomeGroupHeight = rectSize.Height() + BUTTON_BORDER*2;
	m_btnUploadItem.ShowWindow( SW_HIDE );
	
	m_boxBrowserGroup.GetClientRect( rectSize );
	m_boxBrowserGroup.SetWindowPos( NULL, 0, 0, rectSize.Width(), iHomeGroupHeight, SWP_NOMOVE );
	m_boxBrowserGroup.GetWindowRect( rectSize );
	this->ScreenToClient( rectSize );
	this->InvalidateRect( rectSize );

	m_btnTopUp.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Register") ) );

	MarketObject::ResetObjectCompleteness( );

	if ( theApp.m_szSESID )
	{
		delete [] theApp.m_szSESID; 
		theApp.m_szSESID = NULL;
		
		if ( theApp.m_szCredits )
		{
			delete [] theApp.m_szCredits;
			theApp.m_szCredits = NULL;
		}

		if ( theApp.m_bAdmin || theApp.m_bUploader ) 
		{
			ClearObjectsDir( );
			SetTimer( 8, 500, NULL );
			//SetTimer( 11, 1000, NULL );
		}

		CString sFilename = theApp.m_szDirectory;
		sFilename += "\\Files\\TGCStore\\TEMP\\PurchasedObjects.xml";
		DeleteFile( sFilename );
	
		/*
		int timeout = 300;
		while ( m_ServerDownloader.IsDownloading() && timeout > 0 )
		{
			Sleep(10);
			timeout--;
		}

		if( !m_ServerDownloader.IsDownloading() )
		{
			if ( !m_ModelDownloader.IsDownloading() ) m_ServerDownloader.SetProgress( &m_ProgressBar, NULL );
			else m_ServerDownloader.SetProgress( NULL, NULL );

			m_ServerDownloader.SetCategoryDownload( this );
			m_ServerDownloader.Start( );
		}
		*/
	}

	theApp.m_bAdmin = false;
	theApp.m_bUploader = false;
	m_bSearchListLoaded = false;
	theApp.m_bAgreedTerms = false;

	SetTimer( 9, 500, NULL );

	if ( iGroupSelected != 0 && iGroupSelected != -10 )
	{
		//m_Tree.SelectHome();
	}
	else
	{
		//SwitchToMarketPlace( );
	}

	m_bLoggingOut = false;
}

void CMarketWindow::OnBnClickedLogin()
{
	if ( m_bLoggingOut || m_bLoggingIn ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("LogoutBusy") ), theApp.m_szErrorString );
		return;
	}

	if ( theApp.m_szSESID )
	{
		Logout( );
		return;
	}

	if ( m_Uploader.IsUploading() ) 
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("NoLoginUpload") ), theApp.m_szErrorString );
		return;
	}
	
	theApp.m_bAdmin = false;

	CLoginDialog login;
	login.SetMarketWindow( this );

	m_bAutoLogin = false;

	if ( theApp.m_bSaveUsername && theApp.m_bSavePassword )
	{
		strcpy_s( szUsername, 50, theApp.m_sUsername );
		strcpy_s( szPassword, 50, theApp.m_sPassword );
		m_bAutoLogin = true;
	}
	else
	{
		int result = login.DoModal( );
		if ( result != IDOK ) return;
		
		strcpy_s( szUsername, 50, login.GetUsername( ) );
		strcpy_s( szPassword, 50, login.GetPassword( ) );
	}

	m_bLoggingIn = true;
	
	SetTimer( 6, 500, NULL );
	return;
	
	/*
	m_textNoItems.SetWindowText( "Please Wait..." );
	m_textNoItems.ShowWindow( SW_SHOW );
	m_textNoItems.Invalidate();
	SetTimer( 98, 5000, NULL );
	*/

	//HTTPComm m_Server;
	//bool result = m_Server.Login( szUsername, szPassword );

	//m_textNoItems.ShowWindow( SW_HIDE );

	//m_bLoggingIn = false;


	if ( !theApp.m_szSESID ) 
	{
		if ( m_bAutoLogin )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("CheckLoginPref") ) );
		}

		return;
	}
	
	if ( !theApp.m_bAgreedTerms )
	{
		CString sURL = "http://www.thegamecreators.com/index_plain.php?m=store_existing&email=";
		sURL += szUsername;
		sURL += "&lang=";
		sURL += theApp.m_szLanguageName;

		ShowBrowser( );
		m_WebBrowser.Navigate( sURL, NULL, NULL, NULL, NULL );
		
		return;
	}

	SetTimer( 6, 900000, NULL ); //15 minutes

	theApp.m_sUsername = szUsername;
	theApp.m_sPassword = szPassword;
	theApp.m_bFirstRun = false;

	m_txtLoading.SetTextColor( RGB(0,180,0) );
	m_txtLoading.SetWindowText( szUsername );
	m_txtLoading.Invalidate( );

	m_txtCredits.SetTextColor( RGB(0,180,0) );
	m_txtCredits.SetWindowText( theApp.m_szCredits ? theApp.m_szCredits : "0" );
	m_txtCredits.Invalidate( );

	int iHomeGroupHeight = 0;
	CRect rectSize;

	if ( theApp.m_bUploader ) 
	{
		m_btnHome.GetClientRect( rectSize );
		iHomeGroupHeight = rectSize.Height() + BUTTON_BORDER*2 + 1;
		m_btnUploadItem.GetClientRect( rectSize );
		iHomeGroupHeight += rectSize.Height();
		
		m_btnUploadItem.ShowWindow( SW_SHOW );
	}
	else 
	{
		m_btnHome.GetClientRect( rectSize );
		iHomeGroupHeight = rectSize.Height() + BUTTON_BORDER*2;
		m_btnUploadItem.ShowWindow( SW_HIDE );
	}

	m_boxBrowserGroup.GetClientRect( rectSize );
	m_boxBrowserGroup.SetWindowPos( NULL, 0, 0, rectSize.Width(), iHomeGroupHeight, SWP_NOMOVE );
	m_boxBrowserGroup.GetWindowRect( rectSize );
	this->ScreenToClient( rectSize );
	this->InvalidateRect( rectSize );

	m_btnLogin.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Logout") ) );

	CRect rect, treeRect;
	//theApp.m_pEditorView->GetWindowRect ( &rect );
	GetClientRect( &rect );
	ClientToScreen( &rect );
	m_Tree.GetWindowRect( &treeRect );
	ScreenToClient( treeRect );

	DWORD dwHeight = rect.bottom - rect.top;

	/*
	m_btnLogin.GetClientRect( rectSize ); 
	int iLoginSizeX = treeRect.Width() < 168 ? (treeRect.Width()-8)/2 : 80;
	int iLoginX = treeRect.left + treeRect.Width()/2 - iLoginSizeX - 2;
	m_btnLogin.SetWindowPos ( NULL, iLoginX, dwHeight - (78 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );

	int iTopUpX = treeRect.left + treeRect.Width()/2 + 2;
	m_btnTopUp.SetWindowPos ( NULL, iTopUpX, dwHeight - (78 - BUTTON_BORDER), iLoginSizeX, rectSize.Height(), 0 );

	m_btnTopUp.ShowWindow( SW_SHOW );
	*/
	
	m_btnTopUp.SetWindowText( GetLanguageData( _T("TGC Store"), _T("TopUp") ) );
	m_chkShowPurchased.ShowWindow( SW_SHOW );

	//m_Tree.SelectStoreRoot();
	//client needs to update purchased information about cached objects, flag for complete update
	MarketObject::ResetObjectCompleteness( );

	//m_MarketImage.DeleteAllItems();
	//m_MarketImage.InsertItem( 0, "<Loading...>" );

	m_bSearchListLoaded = false;

	if ( theApp.m_bAdmin || theApp.m_bUploader )
	{
		ClearObjectsDir( );
		SetTimer( 8, 500, NULL );
		//SetTimer( 11, 1000, NULL );
	}
	else
	{
		SetTimer( 9, 2000, NULL );
	}

	SetTimer( 12, 1000, NULL );
	
	//if ( m_iSearchValid == 1 )
	{
		//MarketObject::Search( m_sLSName, m_sLSCategory, m_sLSCostLow, m_sLSCostHigh, 
		//		m_sLSAuthor, m_sLSRatingLow, m_sLSRatingHigh, m_sLSKeyword );
		/*
		HTTPComm server;
		if ( server.Search( "Files\\TGCStore\\TEMP\\SearchResults.xml", m_sLSName, m_sLSCategory, m_sLSCostLow, m_sLSCostHigh, 
						m_sLSAuthor, m_sLSRatingLow, m_sLSRatingHigh, m_sLSKeyword ) )
		{
			//MessageBox( "Search Failed", "Server Error" );
			//m_txtStatus.SetWindowText( "Idle" );
			MarketObject::LoadSearchObjects( "Files\\TGCStore\\TEMP\\SearchResults.xml" );
			//return;
		}
		*/
	}

	//MarketObject::UpdateCartObjects();

	if ( iGroupSelected != 0 && iGroupSelected != -10 )
	{
		//m_Tree.SelectHome();
	}
	else
	{
		SwitchToMarketPlace( );
	}

	//MessageBox( "Success", "Login" );
}

void CMarketWindow::LoggedIn( bool bSuccess, bool bSilent )
{
	m_bLoggingIn = false;
	m_textNoItems.ShowWindow( SW_HIDE );

	if ( !bSuccess ) 
	{
		if ( bSilent )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("LoginRefreshFailed") ), theApp.m_szErrorString );
			Logout();
			return;
		}

		if ( m_bAutoLogin )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("CheckLoginPref") ) );
		}

		return;
	}

	if ( !theApp.m_bAgreedTerms )
	{
		SetTimer( 15, 500, 0 );
		return;
	}

	SetTimer( 6, 900000, NULL ); //15 minutes
	m_txtCredits.SetWindowText( theApp.m_szCredits ? theApp.m_szCredits : "0" );
	m_txtCredits.Invalidate( );

	if ( bSilent ) return;

	theApp.m_sUsername = szUsername;
	theApp.m_sPassword = szPassword;
	theApp.m_bFirstRun = false;

	m_txtLoading.SetTextColor( RGB(0,180,0) );
	m_txtLoading.SetWindowText( szUsername );
	m_txtLoading.Invalidate( );

	m_txtCredits.SetTextColor( RGB(0,180,0) );
	m_txtCredits.SetWindowText( theApp.m_szCredits ? theApp.m_szCredits : "0" );
	m_txtCredits.Invalidate( );

	int iHomeGroupHeight = 0;
	CRect rectSize;

	if ( theApp.m_bUploader ) 
	{
		m_btnHome.GetClientRect( rectSize );
		iHomeGroupHeight = rectSize.Height() + BUTTON_BORDER*2 + 1;
		m_btnUploadItem.GetClientRect( rectSize );
		iHomeGroupHeight += rectSize.Height();
		
		m_btnUploadItem.ShowWindow( SW_SHOW );
	}
	else 
	{
		m_btnHome.GetClientRect( rectSize );
		iHomeGroupHeight = rectSize.Height() + BUTTON_BORDER*2;
		m_btnUploadItem.ShowWindow( SW_HIDE );
	}

	m_boxBrowserGroup.GetClientRect( rectSize );
	m_boxBrowserGroup.SetWindowPos( NULL, 0, 0, rectSize.Width(), iHomeGroupHeight, SWP_NOMOVE );
	m_boxBrowserGroup.GetWindowRect( rectSize );
	this->ScreenToClient( rectSize );
	this->InvalidateRect( rectSize );

	m_btnLogin.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Logout") ) );

	CRect rect, treeRect;
	//theApp.m_pEditorView->GetWindowRect ( &rect );
	GetClientRect( &rect );
	ClientToScreen( &rect );
	m_Tree.GetWindowRect( &treeRect );
	ScreenToClient( treeRect );

	DWORD dwHeight = rect.bottom - rect.top;

	m_btnTopUp.SetWindowText( GetLanguageData( _T("TGC Store"), _T("TopUp") ) );
	m_chkShowPurchased.ShowWindow( SW_SHOW );

	MarketObject::ResetObjectCompleteness( );

	m_bSearchListLoaded = false;

	if ( theApp.m_bAdmin || theApp.m_bUploader )
	{
		ClearObjectsDir( );
		SetTimer( 8, 500, NULL );
		//SetTimer( 11, 1000, NULL );
	}

	SetTimer( 12, 1000, NULL );

	if ( iGroupSelected == 0 || iGroupSelected == -10 )
	{
		SetTimer( 14, 500, NULL );
	}
	else
	{
		SetTimer( 9, 2000, NULL );
	}
	
	//if ( m_iSearchValid == 1 )
	{
		//MarketObject::Search( m_sLSName, m_sLSCategory, m_sLSCostLow, m_sLSCostHigh, 
		//		m_sLSAuthor, m_sLSRatingLow, m_sLSRatingHigh, m_sLSKeyword );
		/*
		HTTPComm server;
		if ( server.Search( "Files\\TGCStore\\TEMP\\SearchResults.xml", m_sLSName, m_sLSCategory, m_sLSCostLow, m_sLSCostHigh, 
						m_sLSAuthor, m_sLSRatingLow, m_sLSRatingHigh, m_sLSKeyword ) )
		{
			//MessageBox( "Search Failed", "Server Error" );
			//m_txtStatus.SetWindowText( "Idle" );
			MarketObject::LoadSearchObjects( "Files\\TGCStore\\TEMP\\SearchResults.xml" );
			//return;
		}
		*/
	}
}

void CMarketWindow::OnBnClickedBuy()
{
	if ( iGroupSelected == -2 )
	{
		if ( m_bBuyingCart )
		{
			//m_bBuyingCart = false;
			m_bCancelCart = true;
			m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("BuyCart") ) );
			m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("FinishingObject") ) );
			KillTimer( 1 );

			//m_ModelDownloader.SetProgress( NULL, NULL );
			m_ModelDownloader.Cancel();
			//m_ModelDownloader.Join();
			//m_ModelDownloader.SetProgress( &m_ProgressBar, NULL );

			//if ( !m_ModelDownloader.IsDownloading( ) )
			{
				m_bBuyingCart = false;
				m_bCancelCart = false;

				m_Cancel.SetWindowText( GetLanguageData( "TGC Store", "Close" ) );
				m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Idle") ) );
			}

			/*
			MarketObject *pObject = MarketObject::GetCartList( );
			MarketObject::RemoveFromCart( pObject->GetID( ) );
			m_Tree.SetCartCount( MarketObject::CountCartObjects() );
			*/
			return;
		}

		m_bCancelCart = false;

		//shopping cart buy
		MarketObject *pObject = MarketObject::GetCartList( );
		if ( !pObject )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("NoItemsCart") ) );
			return;
		}

		if ( !theApp.m_szSESID ) 
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("MustLogin") ), theApp.m_szErrorString ); 
			return;
		}

		DWORD dwHeadObjID = pObject->GetID( );

		int iTotalCost = 0;
		while ( pObject )
		{
			if ( !pObject->IsPurchased() ) iTotalCost += pObject->GetCost( );
			pObject = pObject->pNextObject;
		}

		if ( m_ModelDownloader.IsDownloading( ) ) MessageBox( GetLanguageData( _T("TGC Store"), _T("CartBusy") ) );
		else
		{
			char format [ 256 ];
			char str [ 256 ];
			strcpy_s( format, 256, GetLanguageData( _T("TGC Store"), _T("ConfirmCart") ) );
			sprintf_s( str, 256, format, MarketObject::CountCartObjects(), iTotalCost );

			char str2 [ 256 ];
			sprintf_s( str2, 256, "%s\n\n%s", str, GetLanguageData( _T("TGC Store"), _T("ConfirmCart2") ) );
			int result = MessageBox( str2, GetLanguageData( _T("TGC Store"), _T("Confirm") ), MB_YESNO | MB_ICONQUESTION );
			if ( result != IDYES ) return;

			m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("CancelBuy") ) );
			m_bBuyingCart = true;

			m_sDownloadingModelName = MarketObject::GetCartList( )->GetName();

			m_ModelDownloader.SetModelDownload( "Files\\TGCStore\\TEMP\\Model.zip", MarketObject::GetCartList( ), MarketObject::GetCartList( )->GetCost( ), this );
			m_ModelDownloader.Start( );

			//MarketObject::RemoveFromCart( dwHeadObjID );
			//m_Tree.SetCartCount( MarketObject::CountCartObjects() );

			m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloading") ) );
			SetTimer( 7, 2000, NULL );
		}
	}
	else
	{
		if ( m_MarketImage.GetSelectedCount() > 1 )
		{
			if ( m_bBuyingCart )
			{
				MessageBox( GetLanguageData( _T("TGC Store"), _T("CartNoAddBuying") ) );
				return;
			}

			POSITION pos = m_MarketImage.GetFirstSelectedItemPosition();
			
			while ( pos )
			{
				int nItem = m_MarketImage.GetNextSelectedItem( pos );

				//char szText [ 64 ];
				//m_MarketImage.GetItemText( nItem, 0, szText, 64 );
				//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

				MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjByIndex( nItem ), iGroupSelected );
				if ( !pObject ) continue;

				MarketObject *pCartObject = MarketObject::GetCartObject( m_MarketImage.GetObjByIndex( nItem ) );

				if ( pCartObject && pCartObject->GetID() == pObject->GetID() ) continue;

				MarketObject::AddToCart( pObject );
			}

			m_Tree.SetCartCount( MarketObject::CountCartObjects() );

			m_MarketImage.Invalidate();
		}
		else
		{
			//individual buy
			int item = m_MarketImage.GetItemClicked( );
			if ( item < 0 )
			{
				MessageBox( GetLanguageData( _T("TGC Store"), _T("SelectError") ) );
				return;
			}

			//char szText [ 64 ];
			//m_MarketImage.GetItemText( item, 0, szText, 64 );
			//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

			MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected(), iGroupSelected );
			if ( !pObject )
			{
				MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
				return;
			}

			if ( !theApp.m_szSESID ) MessageBox( GetLanguageData( _T("TGC Store"), _T("MustLogin") ), theApp.m_szErrorString ); 
			else
			{
				//reduce credits
				if ( m_ModelDownloader.IsDownloading( ) || m_bBuyingCart ) MessageBox( GetLanguageData( _T("TGC Store"), _T("OnlyOne") ) );
				else
				{
					if ( !pObject->IsPurchased( ) && pObject->GetCost() > 0 )
					{
						char format [ 256 ];
						strcpy_s( format, 256, GetLanguageData( _T("TGC Store"), _T("ConfirmPurchase") ) );

						char str [ 256 ];
						sprintf_s( str, 256, format, pObject->GetName(), pObject->GetCost() );
						int result = MessageBox( str, GetLanguageData( _T("TGC Store"), _T("Confirm") ), MB_YESNO | MB_ICONQUESTION );
						if ( result != IDYES ) return;
					}

					m_bBuyingCart = false;

					m_sDownloadingModelName = pObject->GetName();

					m_ModelDownloader.SetModelDownload( "Files\\TGCStore\\TEMP\\Model.zip", pObject, pObject->GetCost( ), this );
					m_ModelDownloader.Start( );

					m_Cancel.SetWindowText( GetLanguageData( "TGC Store", "Cancel" ) );

					m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloading") ) );
					SetTimer( 7, 2000, NULL );

					//MessageBox( "Download started" );
				}
			}
		}
	}
}

void CMarketWindow::OnSize( UINT nType, int cx, int cy )
{
	CDialog::OnSize( nType, cx, cy );
	/*
	if ( nType == SIZE_MINIMIZED ) 
	{
		theApp.GetMainWnd()->ShowWindow( SW_MINIMIZE );
	}

	if ( nType == SIZE_RESTORED ) 
	{
		theApp.GetMainWnd()->ShowWindow( SW_RESTORE );
	}

	*/
}

void CMarketWindow::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		KillTimer(1);
		if ( !m_bBuyingCart || m_bCancelCart ) 
		{
			m_bBuyingCart = false;
			m_bCancelCart = false;
			if ( iGroupSelected == -2 ) m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("BuyCart") ) );
			SetTimer( 12, 500, NULL );
			return;
		}

		MarketObject *pObject = MarketObject::GetCartList( );
		if ( !pObject )
		{
			m_bBuyingCart = false;
			if ( iGroupSelected == -2 ) m_btnBuy.SetWindowText( GetLanguageData( _T("TGC Store"), _T("BuyCart") ) );

			DWORD dwValue = 1;
			theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "display_cart_buy_message", &dwValue );
			if ( dwValue == 1 )
			{
				CartMsgDialog optionalMsg;
				optionalMsg.SetValues( GetLanguageData( "TGC Store", "CartFinished" ), GetLanguageData( "TGC Store", "Complete" ), "cart_buy" );
				optionalMsg.DoModal();
			}

			m_textNoItems.SetWindowText( GetLanguageData( "TGC Store", "DownloadSuccess" ) );
			m_textNoItems.ShowWindow( SW_SHOW );
			m_textNoItems.Invalidate();
			SetTimer( 98, 5000, 0 );

			SetTimer( 12, 500, NULL );
			return;
		}
		else
		{
			static int iLoopCount = 0;
			if ( m_ModelDownloader.IsDownloading() ) 
			{
				iLoopCount++;
				if ( iLoopCount < 30 ) SetTimer( 1, 500, NULL );
				else iLoopCount = 0;
			}
			else
			{
				iLoopCount = 0;

				DWORD dwObjID = pObject->GetID( );

				//MarketObject::RemoveFromCart( dwObjID );
				//m_Tree.SetCartCount( MarketObject::CountCartObjects() );

				m_sDownloadingModelName = pObject->GetName();
				
				m_ModelDownloader.SetModelDownload( "Files\\TGCStore\\TEMP\\Model.zip", pObject, pObject->GetCost( ), this );
				m_ModelDownloader.Start( );

				m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloading") ) );
				SetTimer( 7, 2000, NULL );
			}
		}
	}

	if ( nIDEvent == 2 )
	{
		iCartFlashCounter--;
		if ( iCartFlashCounter <= 0 ) 
		{
			KillTimer( 2 );
			iCartFlashCounter = 0;
			iCartFlashMode = 0;
			m_Tree.Invalidate( );
			return;
		}

		iCartFlashMode = 1 - iCartFlashMode;
		m_Tree.Invalidate( );
	}

	if ( nIDEvent == 3 )
	{
		iSearchFlashCounter--;
		if ( iSearchFlashCounter <= 0 ) 
		{
			KillTimer( 3 );
			iSearchFlashCounter = 0;
			iSearchFlashMode = 0;
			m_Tree.Invalidate( );
			return;
		}

		iSearchFlashMode = 1 - iSearchFlashMode;
		m_Tree.Invalidate( );
	}

	if ( nIDEvent == 4 )
	{
		if ( !m_ServerDownloader.IsDownloading() )
		{
			KillTimer( 4 );
			
			//if ( iLastGroupUpdated == iGroupSelected ) return;
			iLastGroupUpdated = iGroupSelected;

			m_textNoItems.SetWindowText( GetLanguageData( _T("TGC Store"), _T("LoadThumbnails") ) );
			m_textNoItems.ShowWindow( SW_SHOW );
			m_textNoItems.Invalidate();
			SetTimer( 98, 5000, NULL );

			if ( !m_ModelDownloader.IsDownloading() ) 
			{
				m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloading") ) );
				m_ServerDownloader.SetProgress( &m_ProgressBar, NULL );
			}
			else m_ServerDownloader.SetProgress( NULL, NULL );
			m_ServerDownloader.SetThumbnailDownload( iGroupSelected, this, m_sThumbnailsNeeded );
			m_ServerDownloader.Start();
		}
	}

	if ( nIDEvent == 5 )
	{
		KillTimer( 5 );
		if ( m_bArtistIconsLoaded ) return;
		m_bArtistIconsLoaded = true;
		MarketGroup *pGroup = MarketObject::GetGroupList( );

		HTTPComm server;

		while ( pGroup )
		{
			CString sFilename = "Files\\TGCStore\\TEMP\\Artists\\";
			sFilename += pGroup->szName;
			sFilename += ".bmp";

			CString sImage = "Images\\Artists\\";
			sImage += pGroup->szName;
			sImage += ".bmp";

			bool result = server.GetPreviewImage( sImage, sFilename );
			if ( !result ) MessageBox( server.GetLastError( ), "Server Error" );

			pGroup = pGroup->pNextGroup;
		}

		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );
		m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
	}

	if ( nIDEvent == 6 )
	{
		KillTimer( 6 );

		if ( !theApp.m_szSESID && ( strlen(szUsername) == 0 || strlen(szPassword) == 0 ) )
		{			
			return;
		}
		
		if ( m_Uploader.IsUploading() ) 
		{
			SetTimer( 6, 300000, NULL );
			return;
		}
		if ( m_ServerDownloader.IsDownloading() || m_ModelDownloader.IsDownloading() ) 
		{
			SetTimer( 6, 5000, NULL );
			return;
		}
		

		m_bLoggingIn = true;

		m_textNoItems.SetWindowText( "Please Wait..." );
		m_textNoItems.ShowWindow( SW_SHOW );
		m_textNoItems.Invalidate();
		SetTimer( 98, 5000, NULL );

		if ( theApp.m_szSESID ) m_ServerDownloader.SetLoginProcess( this, theApp.m_sUsername, theApp.m_sPassword, true );
		else m_ServerDownloader.SetLoginProcess( this, szUsername, szPassword, false );
		m_ServerDownloader.Start();

		//HTTPComm m_Server;
		//bool result = m_Server.Login( theApp.m_sUsername, theApp.m_sPassword );

		//m_textNoItems.ShowWindow( SW_SHOW );

		//m_bLoggingIn = false;
		
		/*
		if ( !result ) 
		{
			Logout( );
			MessageBox( GetLanguageData( _T("TGC Store"), _T("LoginRefreshFailed") ), theApp.m_szErrorString );
		}

		SetTimer( 6, 900000, NULL );
		*/
	}

	if ( nIDEvent == 7 )
	{
		if ( !m_ModelDownloader.IsDownloading() ) 
		{
			KillTimer( 7 );
			m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Idle") ) );
			m_Cancel.SetWindowText( GetLanguageData( "TGC Store", "Close" ) );
			return;
		}

		iDownloadFlashMode = 1 - iDownloadFlashMode;
		if ( iDownloadFlashMode == 0 )
		{
			m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Downloading") ) );
		}
		else
		{
			//MarketObject *pObject = MarketObject::GetObject( m_ModelDownloader.GetObjectID() );
			//if ( pObject )
			{
				m_txtStatus.SetWindowText( m_sDownloadingModelName );
			}
		}
	}

	if ( nIDEvent == 8 )
	{
		if( !m_ServerDownloader.IsDownloading() && !m_bLoggingIn )
		{
			KillTimer( 8 );

			if ( !m_ModelDownloader.IsDownloading() ) 
			{
				m_txtStatus.SetWindowText( "Refreshing List" );
				m_ServerDownloader.SetProgress( &m_ProgressBar, NULL );
			}
			else m_ServerDownloader.SetProgress( NULL, NULL );

			//m_textNoItems.SetWindowText( GetLanguageData( _T("TGC Store"), _T("UpdatingList") ) );
			//m_textNoItems.ShowWindow( SW_SHOW );
			//m_textNoItems.Invalidate();

			m_ServerDownloader.SetGroupListDownload( this );
			m_ServerDownloader.Start( );
		}
	}

	if ( nIDEvent == 9 )
	{
		if ( !m_ServerDownloader.IsDownloading() )
		{
			KillTimer( 9 );

			//iCurrentPage = 1;

			if ( iGroupSelected == -2 ) m_btnBuy.EnableWindow( TRUE );
			else m_btnBuy.EnableWindow( FALSE );

			m_btnDetails.EnableWindow( FALSE );

			char order [ 32 ];
			m_comboOrderBy.GetWindowText( order, 32 );
			m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
		}
	}

	if ( nIDEvent == 10 )
	{
		KillTimer( 10 );
		
		int iIndex = m_sURL.Find( "view_object=" );
		if ( iIndex >= 0 ) 
		{
			const char *szURL = m_sURL.GetString();
			szURL = strstr( szURL, "view_object=" );
			if ( !szURL ) return;
			szURL += strlen( "view_object=" );
			
			char szID [ 11 ];
			const char *szAmp = strchr( szURL, '&' );
			if ( !szAmp )
			{
				strcpy_s( szID, 11, szURL );
			}
			else
			{
				DWORD dwLength = (DWORD) (szAmp - szURL);
				if ( dwLength > 10 ) dwLength = 10;
				strncpy_s( szID, 11, szURL, dwLength );
				szID [ dwLength ] = '\0';
			}

			DWORD dwID = atoi( szID );
			ShowObjectDetails( dwID );

			//CString sStoreFront = "http://www.tgcstore.net/StoreFront.php?version=";
			//sStoreFront += theApp.m_szVersion;
			//if ( theApp.m_szSESID ) sStoreFront += "&loggedin=1";
			//m_WebBrowser.Navigate( sStoreFront ,NULL, NULL, NULL, NULL );

			//SwitchToMarketPlace( 1, "Online Items" );
			
		}
		m_sURL = "";
	}

	if ( nIDEvent == 11 )
	{
		if( !m_ServerDownloader.IsDownloading() )
		{
			KillTimer( 11 );

			if ( !m_ModelDownloader.IsDownloading() ) 
			{
				m_txtStatus.SetWindowText( "Getting Objects" );
				m_ServerDownloader.SetProgress( &m_ProgressBar, NULL );
			}
			else m_ServerDownloader.SetProgress( NULL, NULL );

			//m_textNoItems.SetWindowText( GetLanguageData( _T("TGC Store"), _T("UpdatingList") ) );
			//m_textNoItems.ShowWindow( SW_SHOW );
			//m_textNoItems.Invalidate();

			m_ServerDownloader.SetObjectListDownload( this, iGroupSelected );
			m_ServerDownloader.Start( );
		}
	}

	if ( nIDEvent == 12 )
	{
		if( !m_ServerDownloader.IsDownloading() )
		{
			KillTimer( 12 );

			m_ServerDownloader.SetProgress( NULL, NULL );

			m_ServerDownloader.SetPurchasedListDownload( this );
			m_ServerDownloader.Start( );
		}
	}

	if ( nIDEvent == 13 )
	{
		if( !m_ServerDownloader.IsDownloading() )
		{
			KillTimer( 13 );

			if ( !m_bSearchListLoaded )
			{
				if ( !m_ModelDownloader.IsDownloading() ) 
				{
					m_txtStatus.SetWindowText( "Getting Data" );
					m_ServerDownloader.SetProgress( &m_ProgressBar, NULL );
				}
				else m_ServerDownloader.SetProgress( NULL, NULL );

				m_ServerDownloader.SetSearchListDownload( this );
				m_ServerDownloader.Start( );
			}
		}
	}

	if ( nIDEvent == 14 )
	{
		KillTimer( 14 );

		SwitchToMarketPlace( );
	}

	if ( nIDEvent == 15 )
	{
		KillTimer( 15 );

		CString sURL = "http://www.thegamecreators.com/index_plain.php?m=store_existing&email=";
		sURL += szUsername;
		sURL += "&lang=";
		sURL += theApp.m_szLanguageName;

		ShowBrowser( );
		m_WebBrowser.Navigate( sURL, NULL, NULL, NULL, NULL );
	}

	if ( nIDEvent == 16 )
	{
		if( !m_ServerDownloader.IsDownloading() )
		{
			KillTimer( 16 );

			m_ServerDownloader.SetProgress( NULL, NULL );

			m_textNoItems.SetWindowText( GetLanguageData( _T("TGC Store"), _T("PleaseWait") ) );
			m_textNoItems.ShowWindow( SW_SHOW );
			m_textNoItems.Invalidate();

			iGroupSelected = 310;
			
			m_ServerDownloader.SetObjectListDownload( this, iGroupSelected );
			m_ServerDownloader.Start( );

			ShowBrowser( FALSE );
		}
	}

	if ( nIDEvent == 98 )
	{
		if ( !m_ServerDownloader.IsDownloading() && !m_ModelDownloader.IsDownloading() ) 
		{
			m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Idle") ) );
			m_textNoItems.ShowWindow( SW_HIDE );
		}
	}

	if ( nIDEvent == 99 )
	{
		m_iCloseWaitTimer++;

		if ( (!m_ServerDownloader.IsDownloading() && !m_ModelDownloader.IsDownloading() && !m_Uploader.IsRunning()) || m_iCloseWaitTimer > 5 )
		{
			KillTimer( 99 );
			CDialog::OnCancel();
		}
	}
}

void CMarketWindow::ShowNoItemsMsg( )
{
	if ( !m_textNoItems.IsWindowVisible() )
	{
		m_textNoItems.SetWindowText( GetLanguageData( _T("TGC Store"), _T("NoItems") ) );
		m_textNoItems.ShowWindow( SW_SHOW );
		m_textNoItems.Invalidate();
		SetTimer( 98, 5000, NULL );
	}
}

void CMarketWindow::OnSearchClicked()
{
	m_iSearchValid = 0;

	CSearchDialog dlgSearch;

	dlgSearch.m_sKeyword = m_sLSKeyword;
	dlgSearch.m_bExactMatch = m_bExactMatch;
	dlgSearch.m_bApprovalItems = m_bSearchApprovalObjectsOnly;
	dlgSearch.m_bAdmin = theApp.m_bAdmin;

	int result = dlgSearch.DoModal( );
	if ( result != IDOK ) return;

	m_sLSKeyword = dlgSearch.m_sKeyword;
	m_bExactMatch = dlgSearch.m_bExactMatch;
	m_bSearchApprovalObjectsOnly = dlgSearch.m_bApprovalItems;

	m_iSearchValid = 1;

	if ( !m_bSearchListLoaded )
	{
		m_txtStatus.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Searching") ) );
		m_textNoItems.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Collecting") ) );
		m_textNoItems.ShowWindow( SW_SHOW );
		m_textNoItems.Invalidate();
		SetTimer( 98, 5000, NULL );

		SetTimer( 13, 1000, NULL );
	}
	else
	{
		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );

		{
			MarketObject::Search( m_sLSKeyword, m_bSearchApprovalObjectsOnly, m_bExactMatch );
			
			iCurrentPage = 1;
			m_bThumbnailsLoaded = false;
			if ( iGroupSelected == -1 ) SetTimer( 9,1000,NULL );//m_MarketImage.LoadRemoteImages( -1, m_chkShowPurchased.GetCheck() > 0, order );
			else m_Tree.SelectSearchResults( );
			RecordHistory( -1 );
		}

		int count = MarketObject::CountSearchObjects();
		m_Tree.SetSearchCount( count );

		if ( count > 0 )
		{
			iSearchFlashCounter = 5;
			SetTimer( 3, 500, NULL );
		}
	}
}

void CMarketWindow::OnShowPurchasedClicked()
{
	if ( iGroupSelected == -10 ) return;

	m_bThumbnailsLoaded = false;
	SetTimer( 9,1000,NULL );
	/*
	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );
	m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
	*/
}

void CMarketWindow::OnOrderByChanged()
{
	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );
	strcpy_s( theApp.m_szOrderBy, 32, order );

	//m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
	m_bThumbnailsLoaded = false;
	SetTimer( 9, 1000, NULL );
}

void CMarketWindow::UpdateCartTotal( )
{
	MarketObject *pObject = MarketObject::GetCartList();
	if ( !pObject ) 
	{
		m_txtCartTotal.SetWindowText( "0" );
		m_txtCartTotal.Invalidate( );
		return;
	}

	DWORD dwHeadObjID = pObject->GetID( );

	int iTotalCost = 0;
	while ( pObject )
	{
		if ( !pObject->IsPurchased() ) iTotalCost += pObject->GetCost( );
		pObject = pObject->pNextObject;
	}

	char szCartTotal [ 16 ];
	sprintf_s( szCartTotal, 16, "%d", iTotalCost );

	m_txtCartTotal.SetWindowText( szCartTotal );
	m_txtCartTotal.Invalidate( );
}

void CMarketWindow::OnAddToCart()
{
	//MessageBox( "Add Item" );

	if ( m_bBuyingCart )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("CartNoAddBuying") ) );
		return;
	}

	if ( m_MarketImage.GetSelectedCount() > 1 )
	{
		POSITION pos = m_MarketImage.GetFirstSelectedItemPosition();
		
		while ( pos )
		{
			int nItem = m_MarketImage.GetNextSelectedItem( pos );

			//char szText [ 64 ];
			//m_MarketImage.GetItemText( nItem, 0, szText, 64 );
			//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

			MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjByIndex( nItem ), iGroupSelected );
			if ( !pObject ) continue;

			MarketObject *pCartObject = MarketObject::GetCartObject( m_MarketImage.GetObjByIndex( nItem ) );

			if ( pCartObject && pCartObject->GetID() == pObject->GetID() ) continue;

			MarketObject::AddToCart( pObject );
		}
	}
	else
	{
		//char szText [ 64 ];
		//m_MarketImage.GetItemText( m_MarketImage.GetItemClicked(), 0, szText, 64 );
		//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

		MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected(), iGroupSelected );
		if ( !pObject )
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

		MarketObject *pCartObject = MarketObject::GetCartObject( m_MarketImage.GetObjSelected() );

		if ( pCartObject && pCartObject->GetID() == pObject->GetID() )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("CartAlreadyAdd") ) );
			return;
		}

		MarketObject::AddToCart( pObject );
	}

	int count = MarketObject::CountCartObjects();
	m_Tree.SetCartCount( count );
	m_MarketImage.Invalidate();
	UpdateCartTotal( );

	if ( count > 0 )
	{
		iCartFlashCounter = 5;
		SetTimer( 2, 500, NULL );
	}
}

void CMarketWindow::OnRemoveFromCart()
{
	//MessageBox( "Remove Item" );

	if ( m_bBuyingCart )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("CartNoRemoveBuying") ) );
		return;
	}

	if ( m_MarketImage.GetSelectedCount() > 1 )
	{
		POSITION pos = m_MarketImage.GetFirstSelectedItemPosition();
		
		while ( pos )
		{
			int nItem = m_MarketImage.GetNextSelectedItem( pos );

			//char szText [ 64 ];
			//m_MarketImage.GetItemText( nItem, 0, szText, 64 );
			//MarketObject *pObject = MarketObject::GetCartObject( szText );

			MarketObject *pObject = MarketObject::GetCartObject( m_MarketImage.GetObjByIndex( nItem ) );
			if ( !pObject ) continue;

			MarketObject::RemoveFromCart( pObject->GetID() );
		}
	}
	else
	{
		//char szText [ 64 ];
		//m_MarketImage.GetItemText( m_MarketImage.GetItemClicked(), 0, szText, 64 );
		//MarketObject *pObject = MarketObject::GetCartObject( szText );

		MarketObject *pObject = MarketObject::GetCartObject( m_MarketImage.GetObjSelected() );
		if ( !pObject )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
			return;
		}

		MarketObject::RemoveFromCart( pObject->GetID( ) );
	}

	SetTimer( 9, 500, NULL );
	/*
	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );
	m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
	*/

	m_Tree.SetCartCount( MarketObject::CountCartObjects() );
	m_MarketImage.Invalidate();
	UpdateCartTotal( );
}

void CMarketWindow::OnApproveItem()
{
	if ( m_MarketImage.GetSelectedCount() > 1 )
	{
		MessageBox( "Cannot approve more than one item at a time" );
		return;
	}

	//char szText [ 64 ];
	//m_MarketImage.GetItemText( m_MarketImage.GetItemClicked(), 0, szText, 64 );
	//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

	MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected(), iGroupSelected );
	if ( !pObject )
	{
		pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjectRClicked(), iGroupSelected );
		if ( !pObject )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
			return;
		}
	}

	char szObjectID [ 16 ];
	sprintf_s( szObjectID, "%d", pObject->GetID() );

	HTTPComm server;
	bool bResult = server.Approve( szObjectID );
	if ( bResult )
	{
		pObject->SetValue( "approved", "y" );

		SetTimer( 9, 500, NULL );
		/*
		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );
		m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
		*/
	}

	ClearObjectsDir( iGroupSelected );
}

void CMarketWindow::OnRemoveItem()
{
	if ( m_MarketImage.GetSelectedCount() > 1 )
	{
		POSITION pos = m_MarketImage.GetFirstSelectedItemPosition();
		
//		char szText [ 64 ];
		char szObjectID [ 16 ];

		while ( pos )
		{
			int nItem = m_MarketImage.GetNextSelectedItem( pos );

			//m_MarketImage.GetItemText( nItem, 0, szText, 64 );
			//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

			MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjByIndex( nItem ), iGroupSelected );
			if ( !pObject ) 
			{
				//pObject = MarketObject::GetObject( m_MarketImage.GetObjectRClicked() );
				if ( !pObject ) continue;
			}

			sprintf_s( szObjectID, "%d", pObject->GetID() );

			HTTPComm server;
			bool bResult = server.Remove( szObjectID );
			if ( bResult )
			{
				pObject->SetValue( "available", "n" );
			}
		}
	}
	else
	{
		//char szText [ 64 ];
		//m_MarketImage.GetItemText( m_MarketImage.GetItemClicked(), 0, szText, 64 );
		//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

		MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected( ), iGroupSelected );
		if ( !pObject )
		{
			pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjectRClicked(), iGroupSelected );
			if ( !pObject )
			{
				MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
				return;
			}
		}

		char szObjectID [ 16 ];
		sprintf_s( szObjectID, "%d", pObject->GetID() );

		HTTPComm server;
		bool bResult = server.Remove( szObjectID );
		if ( bResult )
		{
			pObject->SetValue( "available", "n" );
		}
	}

	ClearObjectsDir( iGroupSelected );
	m_bThumbnailsLoaded = false;

	/*
	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );
	m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
	*/

	SetTimer( 9, 500, NULL );
}

void CMarketWindow::OnDeleteItem()
{
	if ( !theApp.m_bAdmin ) 
	{
		MessageBox( "Only admin users can permanently remove an item from the database" );
		return;
	}

	int result = MessageBox( "This will permanently remove the selected items from the database, continue?", "Confirm", MB_YESNO );
	if ( result != IDYES ) return;

	if ( m_MarketImage.GetSelectedCount() > 1 )
	{
		POSITION pos = m_MarketImage.GetFirstSelectedItemPosition();
		
		//char szText [ 64 ];
		char szObjectID [ 16 ];

		while ( pos )
		{
			int nItem = m_MarketImage.GetNextSelectedItem( pos );

			//m_MarketImage.GetItemText( nItem, 0, szText, 64 );
			//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

			MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjByIndex( nItem ), iGroupSelected );
			if ( !pObject ) 
			{
				//pObject = MarketObject::GetObject( m_MarketImage.GetObjectRClicked() );
				if ( !pObject ) continue;
			}

			sprintf_s( szObjectID, "%d", pObject->GetID() );

			HTTPComm server;
			bool bResult = server.Delete( szObjectID );
			if ( bResult )
			{
				MarketObject::RemoveObject( pObject->GetID( ) );
			}
		}
	}
	else
	{
		//char szText [ 64 ];
		//m_MarketImage.GetItemText( m_MarketImage.GetItemClicked(), 0, szText, 64 );
		//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

		MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected( ), iGroupSelected );
		if ( !pObject )
		{
			pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjectRClicked(), iGroupSelected );
			if ( !pObject )
			{
				MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
				return;
			}
		}

		char szObjectID [ 16 ];
		sprintf_s( szObjectID, "%d", pObject->GetID() );

		HTTPComm server;
		bool bResult = server.Delete( szObjectID );
		if ( bResult )
		{
			MarketObject::RemoveObject( pObject->GetID( ) );
		}
	}

	/*
	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );
	m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
	*/

	ClearObjectsDir( iGroupSelected );
	m_bThumbnailsLoaded = false;

	SetTimer( 9, 500, NULL );
}

void CMarketWindow::OnRestoreItem()
{
	if ( m_MarketImage.GetSelectedCount() > 1 )
	{
		POSITION pos = m_MarketImage.GetFirstSelectedItemPosition();
		
		//char szText [ 64 ];
		char szObjectID [ 16 ];

		while ( pos )
		{
			int nItem = m_MarketImage.GetNextSelectedItem( pos );

			//m_MarketImage.GetItemText( nItem, 0, szText, 64 );
			//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

			MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjByIndex( nItem ), iGroupSelected );
			if ( !pObject )
			{
				//pObject = MarketObject::GetObject( m_MarketImage.GetObjectRClicked() );
				if ( !pObject ) continue;
			}

			sprintf_s( szObjectID, "%d", pObject->GetID() );

			HTTPComm server;
			bool bResult = server.Restore( szObjectID );
			if ( bResult )
			{
				pObject->SetValue( "available", "y" );
			}
		}
	}
	else
	{
		//char szText [ 64 ];
		//m_MarketImage.GetItemText( m_MarketImage.GetItemClicked(), 0, szText, 64 );
		//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

		MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected(), iGroupSelected );
		if ( !pObject )
		{
			pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjectRClicked(), iGroupSelected );
			if ( !pObject )
			{
				MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
				return;
			}
		}

		char szObjectID [ 16 ];
		sprintf_s( szObjectID, "%d", pObject->GetID() );

		HTTPComm server;
		bool bResult = server.Restore( szObjectID );
		if ( bResult )
		{
			pObject->SetValue( "available", "y" );
		}
	}

	/*
	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );
	m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
	*/

	ClearObjectsDir( iGroupSelected );
	SetTimer( 9, 500, NULL );
}

void CMarketWindow::OnEditItem()
{
	if ( !theApp.m_szSESID )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("EditMustLogin") ) );
	}

	if ( !theApp.m_bAdmin )
	{
		if ( theApp.m_bUploader )
		{
			int result = MessageBox( GetLanguageData( _T("TGC Store"), _T("EditConfirm") ), "Confirm", MB_YESNO );
			if ( result != IDYES ) return;
		}
		else
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("EditNoPermission") ) );
		}
	}

	if ( m_MarketImage.GetSelectedCount() > 1 )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("EditOneItem") ) );
		return;
	}
	
	//char szText [ 64 ];
	//m_MarketImage.GetItemText( m_MarketImage.GetItemClicked(), 0, szText, 64 );
	//MarketObject *pObject = MarketObject::GetObject( iGroupSelected, szText );

	MarketObject *pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjSelected(), iGroupSelected );
	if ( !pObject )
	{
		pObject = MarketObject::GetObjectByID( m_MarketImage.GetObjectRClicked(), iGroupSelected );
		if ( !pObject )
		{
			MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ) );
			return;
		}
	}

	HTTPComm server;
	if ( !server.GetModelDetails( pObject->GetID(), theApp.m_szSESID ) )
	{
		//MessageBox( server.GetLastError(), "Server Error" );
		return;
	}
	pObject->CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );

	CEditDialog edit;
	edit.SetObject( pObject );
	edit.DoModal( );

	if ( !server.GetModelDetails( pObject->GetID(), theApp.m_szSESID ) )
	{
		return;
		//MessageBox( server.GetLastError(), "Server Error" );
	}
	pObject->CompleteObject( "Files\\TGCStore\\TEMP\\Object.xml" );

	/*
	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );
	m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order );
	*/

	ClearObjectsDir( iGroupSelected );
	m_bThumbnailsLoaded = false;

	SetTimer( 9, 500, NULL );
}
#endif

void CMarketWindow::OnHomeClicked()
{
	//char order [ 32 ];
	//m_comboOrderBy.GetWindowText( order, 32 );
	//m_MarketImage.LoadRemoteImages( 0, m_chkShowPurchased.GetCheck() > 0, order );
	//SwitchToMarketPlace( );
	if ( iGroupSelected == 0 ) SwitchToMarketPlace( );
	else m_Tree.SelectHome( );
}

void CMarketWindow::OnBackClicked()
{
	if ( iGroupSelected == 0 || iGroupSelected == -10 )
	{
		try
		{
			m_WebBrowser.GoBack( );
		}
		catch(...) {}
		return;
	}

	if ( iHistorySize == 1 )
	{
		m_btnBack.EnableWindow( FALSE );
		return;
	}

	if ( iHistorySize <= 0 )
	{
		iHistorySize = 0;
		m_btnBack.EnableWindow( FALSE );
		return;
	}

	iHistoryIndex = (iHistoryIndex - 1) % 20;
	int group = iBrowseHistory [ iHistoryIndex ];

	iHistorySize--;
	if ( iHistorySize <= 1 ) 
	{
		iHistorySize = 1;
		m_btnBack.EnableWindow( FALSE ); 
	}

	iCurrentPage = 1;
	char order [ 32 ];
	m_comboOrderBy.GetWindowText( order, 32 );
	m_MarketImage.LoadRemoteImages( group, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );

	if ( group == 0 ) 
	{
		ShowBrowser();
		m_btnBack.EnableWindow( TRUE ); 
	}
	else ShowBrowser( FALSE );
}

void CMarketWindow::RecordHistory( int group )
{
	if ( iHistorySize > 0 )
	{
		if ( iBrowseHistory [ iHistoryIndex ] == group ) return;
	}

	iHistoryIndex = (iHistoryIndex + 1) % 20;
	iBrowseHistory [ iHistoryIndex ] = group;
	iHistorySize++;
	if ( iHistorySize > 20 ) iHistorySize = 20;

	if ( iHistorySize > 1 ) m_btnBack.EnableWindow( TRUE );
}

void CMarketWindow::OnTopUpClicked()
{
	if ( theApp.m_szSESID )
	{
		/*
		ExternalLinkThread *pLink = new ExternalLinkThread();
		pLink->SetLink( this, "http://www.thegamecreators.com/?f=store_credits" );
		pLink->Start();

		DWORD dwValue = 1;
		theApp.GetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "display_topup_bought_message", &dwValue );
		if ( dwValue == 1 )
		{
			CartMsgDialog optionalMsg;
			optionalMsg.SetValues( GetLanguageData( "TGC Store", "TopUpProcess" ), GetLanguageData( "TGC Store", "TopUp" ), "topup_bought" );
			optionalMsg.DoModal();
		}
		*/

		CString sURL = "http://www.thegamecreators.com/index_plain.php?m=store_buy_points&email=";
		sURL += theApp.m_sUsername;
		sURL += "&lang=";
		sURL += theApp.m_szLanguageName;

		ShowBrowser( );
		m_WebBrowser.Navigate( sURL, NULL, NULL, NULL, NULL );
	}
	else
	{
		ShowRegisterPage();
	}
}

void CMarketWindow::ShowRegisterPage( )
{
	ShowBrowser( );

	CString sURL = "http://www.thegamecreators.com/index_plain.php?m=store_registration";
	sURL += "&lang=";
	sURL += theApp.m_szLanguageName;

	m_WebBrowser.Navigate( sURL, NULL, NULL, NULL, NULL );
}

void CMarketWindow::OnBnUploadClicked()
{
	if ( m_Uploader.IsRunning() ) return;
	m_Uploader.Start( );
	/*
	CUploadAppDlg uploader;
	uploader.DoModal();

	m_Tree.ReloadTree( );
	LoadTGCStore( );
	*/
}

void CMarketWindow::OnBnClickedAdBanner()
{
	ExternalLinkThread *pLink = new ExternalLinkThread();
	pLink->SetLink( this, "www.fpscreatorx10.com" );
	pLink->Start();

	m_btnAdBanner.SetLoading( true );
}

void CMarketWindow::OnParentNotify( UINT message, LPARAM lParam )
{
	if ( !m_WebBrowser.GetControlSite() ) return;
	
	//m_sURL = m_WebBrowser.get_LocationName();
	//SetTimer( 10, 200, NULL );

	CWnd::OnParentNotify( message, lParam );
}

void CMarketWindow::OnBnClickedPageFirst()
{
	if ( iCurrentPage == 1 ) return;

	iCurrentPage -= 10;
	if ( iCurrentPage < 1 ) iCurrentPage = 1;

	KillTimer( 4 );
	m_bThumbnailsLoaded = false;

	if ( m_ServerDownloader.IsDownloading() )
	{
		if ( m_ServerDownloader.IsThumbnailDownload() ) m_ServerDownloader.Cancel();

		SetTimer( 9, 200, NULL );
	}
	else
	{
		if ( iGroupSelected == -2 ) m_btnBuy.EnableWindow( TRUE );
		else m_btnBuy.EnableWindow( FALSE );

		m_btnDetails.EnableWindow( FALSE );

		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );
		m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
	}
}

void CMarketWindow::OnBnClickedPageLeft()
{
	if ( iCurrentPage == 1 ) return;

	iCurrentPage--;
	if ( iCurrentPage < 1 ) iCurrentPage = 1;

	KillTimer( 4 );
	m_bThumbnailsLoaded = false;
	
	if ( m_ServerDownloader.IsDownloading() )
	{
		if ( m_ServerDownloader.IsThumbnailDownload() ) m_ServerDownloader.Cancel();

		SetTimer( 9, 200, NULL );
	}
	else
	{
		if ( iGroupSelected == -2 ) m_btnBuy.EnableWindow( TRUE );
		else m_btnBuy.EnableWindow( FALSE );

		m_btnDetails.EnableWindow( FALSE );

		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );
		m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
	}
}

void CMarketWindow::OnBnClickedPageRight()
{
	if ( iCurrentPage == iMaxPage ) return; 

	iCurrentPage++;
	if ( iCurrentPage > iMaxPage ) iCurrentPage = iMaxPage;

	KillTimer( 4 );
	m_bThumbnailsLoaded = false;
	
	if ( m_ServerDownloader.IsDownloading() )
	{
		if ( m_ServerDownloader.IsThumbnailDownload() ) m_ServerDownloader.Cancel();

		SetTimer( 9, 200, NULL );
	}
	else
	{
		if ( iGroupSelected == -2 ) m_btnBuy.EnableWindow( TRUE );
		else m_btnBuy.EnableWindow( FALSE );

		m_btnDetails.EnableWindow( FALSE );

		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );
		m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
	}
}

void CMarketWindow::OnBnClickedPageLast()
{
	if ( iCurrentPage == iMaxPage ) return; 

	iCurrentPage += 10;
	if ( iCurrentPage > iMaxPage ) iCurrentPage = iMaxPage;

	KillTimer( 4 );
	m_bThumbnailsLoaded = false;
	
	if ( m_ServerDownloader.IsDownloading() )
	{
		if ( m_ServerDownloader.IsThumbnailDownload() ) m_ServerDownloader.Cancel();

		SetTimer( 9, 200, NULL );
	}
	else
	{
		if ( iGroupSelected == -2 ) m_btnBuy.EnableWindow( TRUE );
		else m_btnBuy.EnableWindow( FALSE );

		m_btnDetails.EnableWindow( FALSE );

		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );
		m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
	}
}

void CMarketWindow::OnItemsPerPageChanged()
{
	CString sItemsPerPage;
	m_comboItemsPerPage.GetLBText( m_comboItemsPerPage.GetCurSel(), sItemsPerPage );
	
	int items = atoi( sItemsPerPage );
	if ( items > 0 ) iItemsPerPage = items;

	KillTimer( 4 );
	m_bThumbnailsLoaded = false;

	if ( m_ServerDownloader.IsDownloading() )
	{
		if ( m_ServerDownloader.IsThumbnailDownload() ) m_ServerDownloader.Cancel();

		SetTimer( 9, 200, NULL );
	}
	else
	{
		if ( iGroupSelected == -2 ) m_btnBuy.EnableWindow( TRUE );
		else m_btnBuy.EnableWindow( FALSE );

		m_btnDetails.EnableWindow( FALSE );

		char order [ 32 ];
		m_comboOrderBy.GetWindowText( order, 32 );
		m_MarketImage.LoadRemoteImages( iGroupSelected, m_chkShowPurchased.GetCheck() > 0, order, iCurrentPage, iItemsPerPage );
	}
}

BEGIN_EVENTSINK_MAP(CMarketWindow, CDialog)
	ON_EVENT(CMarketWindow, IDC_EXPLORER2, 250, CMarketWindow::BeforeNavigate2Explorer2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

void CMarketWindow::BeforeNavigate2Explorer2(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	m_sURL = URL->bstrVal;
	//MessageBox( m_sURL );

	if ( m_sURL.Find( "view_object=" ) >= 0 )
	{
		*Cancel = TRUE;
		SetTimer( 10, 100, NULL );
	}

	if ( m_sURL.CompareNoCase( "http://UpdatePoints/" ) == 0 )
	{
		*Cancel = TRUE;
		SetTimer( 6, 1000, NULL );
	}
}
