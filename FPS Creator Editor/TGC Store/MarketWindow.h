#if !defined(AFX_CMarketWindow_H__58FE114A_C1EA_4A01_AAA9_05B985F89A6D__INCLUDED_)
#define AFX_CMarketWindow_H__58FE114A_C1EA_4A01_AAA9_05B985F89A6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "MarketTree.h"
#include "..\ZSplitterDlg.h"
#include "..\resource.h"

#ifdef TGC_STORE
	#include "cMarketImage.h"
	#include "ServerDownloader.h"
	#include "afxcmn.h"
	#include "afxwin.h"
	#include "CreditsLabel.h"
	#include "..\explorer1.h"
	#include "AdBanner.h"
	#include "BlackBorder.h"
	#include "UploadAppDlg.h"
#endif

#define MAX_HISTORY 20
#define BUTTON_BORDER 3

class MarketObject;

class CMarketWindow : public CDialog
{
	public:
		CMarketWindow ( CWnd* pParent = NULL );
		~CMarketWindow ();

#ifdef TGC_STORE
		void ResizeAllPanes( int x, int y, int cx, int cy );
		void SwitchToMarketPlace( BOOL bMarket = TRUE, const char *szGroup = NULL );
		void LoadTGCStore( );
		void ClearObjectsDir( int group = 0, bool bClearPurchased = false );
		
		void DownloadComplete( int group, DWORD obj, bool bSuccess = true );
		void ListUpdated( );
		void PurchasedListUpdated( );
		void SearchListUpdated( );
		void ObjectsUpdated( );
		void ThumbnailsUpdated( );
		void LinkLoaded( );
		void UploadComplete( DWORD dwObjID );
		void LoggedIn( bool bSuccess, bool bSilent );

		void ShowPageControls( bool bShow = true );
		void SetPageNumbers( int current, int total );

		void UpdateThumbnails( CString sNeeded );
		void UpdateCartTotal( );
		void Logout( );
		void CancelRefresh() { KillTimer( 9 ); }
		void LoadObjectList( );

		int GetGroupSelected( ) { return iGroupSelected; }
		bool AreArtistIconsLoaded( ) { return m_bArtistIconsLoaded; }
		bool IsBuying() { return m_bBuyingCart; }

		void RecordHistory( int group );
		void ShowBrowser( BOOL bShow = TRUE );

		void BackSpace( );
		void ShowRegisterPage( );
		void ShowObjectDetails( DWORD dwID );
		void ShowNoItemsMsg( );

		MarketObject* GetSelectedObject( );

		CCreditsLabel	m_textNoItems;
#endif

		//{{AFX_DATA(CMarketWindow)
		enum { IDD = IDD_MARKET };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(CMarketWindow)
	protected:
			virtual void DoDataExchange ( CDataExchange* pDX );
		//}}AFX_VIRTUAL

	protected:
		//{{AFX_MSG(cLibraryWindow)
		afx_msg int OnCreate             ( LPCREATESTRUCT lpCreateStruct );
		virtual BOOL OnInitDialog        ( );
		afx_msg void OnSelchangedTree    ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnItemexpandingTree ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnClickList         ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnClickList1         ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
		afx_msg void OnActivate( UINT nState, CWnd *pWndOther, BOOL bMinimized );

#ifdef TGC_STORE
		afx_msg void OnClickList3         ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnClickList2         ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnRClickList		 ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnCustomdrawList    (NMHDR*, LRESULT*);
		afx_msg void OnCustomdrawTree    (NMHDR*, LRESULT*);
		afx_msg void OnBnClickedDetails();
		afx_msg void OnBnClickedLogin();
		afx_msg void OnBnClickedBuy();
		afx_msg void OnSearchClicked();

		afx_msg void OnShowPurchasedClicked();
		afx_msg void OnOrderByChanged();

		afx_msg void OnAddToCart();
		afx_msg void OnRemoveFromCart();
		afx_msg void OnTimer( UINT_PTR nIDEvent );
		afx_msg void OnSize( UINT nType, int cx, int cy );
		afx_msg void OnApproveItem();
		afx_msg void OnParentNotify( UINT message, LPARAM lParam );
#endif

	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

	private:
		void PositionStatusText( );

		CMarketTree	m_Tree;
		//cLibraryImage	m_Image;
		ZSplitter		m_spliter;

		CButton			m_Cancel;

		cMarketImage	m_MarketImage;

		int m_iSearchValid;
		bool m_bSearchListLoaded;
		bool m_bSearchApprovalObjectsOnly;

		ServerDownloader m_ServerDownloader;
		ServerDownloader m_ModelDownloader;

		CUploadAppDlg	m_Uploader;
		int				m_iCloseWaitTimer;

		CBitmap			m_bmpStar;
		CBitmap			m_bmpEmptyStar;
		CBitmap			m_bmpMaskStar;
		CBitmap			m_bmpInCart;

		CButton			m_btnDetails;
		CButton			m_btnLogin;
		CCreditsLabel	m_txtLoading;
		CCreditsLabel	m_txtCredits;
		CFont			m_fntCredits;
		CFont			m_fntLogin;
		CStatic			m_txtCreditsText;
		CStatic			m_txtLoggedInText;
		CButton			m_btnBuy;
		CButton			m_btnBack;
		CButton			m_btnHome;
		CButton			m_btnTopUp;
		CButton			m_btnUploadItem;
		CCreditsLabel	m_txtCartTotal;
		CStatic			m_txtCartTotalLabel;

		CString m_sLSName;
		CString m_sLSCategory;
		CString m_sLSCostLow;
		CString m_sLSCostHigh;
		CString m_sLSAuthor;
		CString m_sLSRatingLow;
		CString m_sLSRatingHigh;
		CString m_sLSKeyword;
		bool m_bExactMatch;

		int iGroupSelected;
		int iLastGroupUpdated;
		
		int iCurrentPage;
		int iMaxPage;
		int iItemsPerPage;

		bool m_bBuyingCart;
		bool m_bCancelCart;
		bool m_bLoggingIn;
		bool m_bLoggingOut;
		bool m_bAutoLogin;
		char szUsername [ 50 ];
		char szPassword [ 50 ];
		CString m_sDownloadingModelName;

		int iCartFlashCounter;
		int iCartFlashMode;
		//DWORD m_dwDownloadingObject;
		int iDownloadFlashMode;

		int iSearchFlashCounter;
		int iSearchFlashMode;
		
		CStatic m_txtStatus;
		CProgressCtrl m_ProgressBar;
		CStatic m_txtStatusLabel;

		CButton m_chkShowPurchased;
		CButton m_btnSearch;

		CComboBox m_comboOrderBy;
		CStatic m_txtOrderBy;
		
		CExplorer1 m_WebBrowser;
		bool m_bShowStoreFront;
		CString m_sURL;

		CAdBanner m_btnAdBanner;

		CBlackBorder m_boxStatusGroup;
		CBlackBorder m_boxLoginGroup;
		CBlackBorder m_boxBrowserGroup;
		CBlackBorder m_boxDetailsGroup;

		CButton m_btnPageFirst;
		CButton m_btnPageLeft;
		CButton m_btnPageRight;
		CButton m_btnPageLast;
		CStatic m_txtPageCounter;

		int iBrowseHistory [ MAX_HISTORY ];
		int iHistoryIndex;
		int iHistorySize;

		bool m_bArtistIconsLoaded;
		bool m_bThumbnailsLoaded;
		CString m_sThumbnailsNeeded;

		afx_msg void OnRemoveItem();
		afx_msg void OnDeleteItem();
		afx_msg void OnRestoreItem();
		afx_msg void OnEditItem();
		afx_msg void OnHomeClicked();
		afx_msg void OnBackClicked();
		afx_msg void OnTopUpClicked();
		afx_msg void OnBnUploadClicked();
		afx_msg void OnBnClickedAdBanner();
		afx_msg void OnBnClickedPageFirst();
		afx_msg void OnBnClickedPageLeft();
		afx_msg void OnBnClickedPageRight();
		afx_msg void OnBnClickedPageLast();
		CStatic m_txtItemsPerPage;
		CComboBox m_comboItemsPerPage;
		afx_msg void OnItemsPerPageChanged();
		DECLARE_EVENTSINK_MAP()
		void BeforeNavigate2Explorer2(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMarketWindow_H__58FE114A_C1EA_4A01_AAA9_05B985F89A6D__INCLUDED_)
