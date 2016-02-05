// cLibraryImage.cpp : implementation file
//

#include "stdafx.h"

#ifdef TGC_STORE

#include "..\editor.h"
#include "cMarketImage.h"
#include "MarketTree.h"
#include "MarketWindow.h"
#include "MarketObject.h"
#include "unzip.h"

#include <atlimage.h>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cMarketImage::cMarketImage ( )
{
	m_pParent = NULL;
	m_dwObjectRClicked = 0;
}

cMarketImage::~cMarketImage ( )
{
}


BEGIN_MESSAGE_MAP ( cMarketImage, CListCtrl )
	//{{AFX_MSG_MAP(cLibraryImage)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLLCLIPBOARD()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

int cMarketImage::Start ( CMarketWindow *parent )
{
	m_pParent = parent;
	if ( m_pParent ) m_pParent->m_textNoItems.ShowWindow( SW_HIDE );

	m_iItemClicked = -1;

	m_Images.Create     ( 64, 64, ILC_COLORDDB, 1, 16 );
	m_Images.SetBkColor ( RGB ( 255, 255, 255 ) );

	MarketObject::ClearObjects( );
		
	this->DeleteAllItems ( );
	this->SetImageList   ( &m_Images, LVSIL_NORMAL );
	this->SetItemCount   ( 64 );

	for ( int i = 0; i <= MAX_ITEMS_PER_PAGE; i++ )
	{
		m_dwObjID [ i ] = 0;
	}

	//char dir [ 256 ];
	//GetCurrentDirectory( 256, dir );
	//MessageBox( dir );

	//m_Server.GetMainModelList( "AllObjects.xml" );

	//bool bSucceed = MarketObject::LoadObjects( "AllObjects.xml" );
	//if ( !bSucceed ) MessageBox( MarketObject::GetLastError( ) );

	//m_Server.GetImages( "AllImages.zip" );	//should be done on a thread

	//InsertItem ( 0, "Loading...", -1 );

	return 1;
}

#include "..\cDialogEnd.h"
extern int g_iBoysLifeQuit;

void cMarketImage::OnClickList ( NMHDR* pNMHDR, LRESULT* pResult )
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

DWORD cMarketImage::GetObjSelected( )
{
	if ( m_iItemClicked < 0 ) m_iItemClicked = 0;
	if ( m_iItemClicked > 100 )
	{
		MessageBox( "Selected ID Out of range", "Warning" );
		m_iItemClicked = 100;
	}

	return m_dwObjID [ m_iItemClicked ];
}

DWORD cMarketImage::GetObjByIndex( DWORD dwIndex )
{
	if ( dwIndex < 0 ) dwIndex = 0;
	if ( dwIndex > 100 )
	{
		MessageBox( "Specified ID Out of range", "Warning" );
		dwIndex = 100;
	}

	return m_dwObjID [ dwIndex ];
}

void cMarketImage::OnRClickList( NMHDR* pNMHDR, LRESULT* pResult, int iGroup, CWnd *pParent )
{
	LPNMITEMACTIVATE pItemAct = ( LPNMITEMACTIVATE ) pNMHDR;

	// determine which item receives the click
	LVHITTESTINFO  hitTest;

	// determine item clicked on
	memset ( &hitTest, '\0', sizeof ( LVHITTESTINFO ) );
	hitTest.pt = pItemAct->ptAction;
	this->SendMessage ( LVM_SUBITEMHITTEST, 0, ( LPARAM ) &hitTest );

	ClientToScreen( &hitTest.pt );

	if ( hitTest.iItem > -1 )
	{
		if ( strcmp( GetItemText( hitTest.iItem, 0 ), "<No Items>" ) == 0 ) return;
		if ( strchr( GetItemText( hitTest.iItem, 0 ), '\\' ) ) return;

		if ( m_iItemClicked < 0 ) m_iItemClicked = 0;
		if ( m_iItemClicked > 100 )
		{
			MessageBox( "Selected ID Out of range", theApp.m_szErrorString );
			m_iItemClicked = 100;
		}

		m_iItemClicked = hitTest.iItem;
		m_dwObjectRClicked = m_dwObjID [ m_iItemClicked ];

		MarketObject *pObject = MarketObject::GetObjectByID( m_dwObjectRClicked, m_pParent->GetGroupSelected() );
		if ( !pObject ) m_dwObjectRClicked = 0;

		//theApp.ShowPopupMenu (IDR_POSTER_MENU, hitTest.pt, this);
		CMenu contextMenu;
		CMenu *pPopupMenu;
		
		if ( iGroup == -2 ) contextMenu.LoadMenu( IDR_CART_MENU );
		else if ( iGroup > 0 || iGroup == -1 || iGroup == -4 ) 
		{
			if ( MarketObject::GetCartObject( m_dwObjectRClicked ) ) contextMenu.LoadMenu( IDR_CART_MENU );
			else 
			{
				if ( !pObject )
				{
					MessageBox( GetLanguageData( _T("TGC Store"), _T("ItemError") ), theApp.m_szErrorString );
					return;
				}

				//char str [ 256 ];
				//sprintf_s( str, 256, "Choosing Menu: Admin: %d, Approved: %d", theApp.m_bAdmin, pObject->IsApproved() );
				//MessageBox( str );

				if ( theApp.m_bAdmin || ( strlen(pObject->GetAuthor()) > 0 && strcmp( theApp.m_szAuthorName, pObject->GetAuthor() ) == 0 ) )
				{
					if ( !pObject->IsAvailable() ) contextMenu.LoadMenu( IDR_ADMIN_DELETE_MENU );
					else 
					{
						if ( !pObject->IsApproved() ) contextMenu.LoadMenu( IDR_ADMIN_ADD_MENU );
						else contextMenu.LoadMenu( IDR_ADMIN_NORMAL_MENU );
					}
				}
				else contextMenu.LoadMenu( IDR_CART_ADD_MENU );
			}

			//MarketObject *pObject = MarketObject::GetObject( iGroup, GetItemText( hitTest.iItem, 0 ) );
			//if ( pObject && pObject->IsPurchased() ) contextMenu.EnableMenuItem( 0, 0 );
		}
		else return;
		
		pPopupMenu = contextMenu.GetSubMenu( 0 );
		pPopupMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, hitTest.pt.x , hitTest.pt.y , pParent ); 
	}
}

extern int m_iScroll;

struct sFileList
{
	char	szFullPath	  [ MAX_PATH ];
	char	szDisplayName [ MAX_PATH ];
	char	szFileName    [ MAX_PATH ];
	char	szImageName   [ MAX_PATH ];
	char	szExtension   [ 4 ];
};

bool cmpO( const MarketObject* a, const MarketObject* b )
{
	return a->comp( b );
}

bool cmpG( const MarketGroup* a, const MarketGroup* b )
{
	return a->comp( b );
}

void cMarketImage::LoadRemoteImages( int iGroup, bool bShowPurchased, LPCSTR szOrderBy, int page, int itemsperpage, bool bThumbnailsUpdate )
{
	if ( m_pParent ) 
	{ 
		//m_pParent->m_textNoItems.ShowWindow( SW_HIDE );
		m_pParent->CancelRefresh();
	}

	for ( int i = 0; i <= MAX_ITEMS_PER_PAGE; i++ ) m_dwObjID [ i ] = 0;

	m_iItemClicked = -1;

	m_Images.DeleteImageList ( );
	m_Images.Create     ( 64, 64, ILC_COLOR32 | ILC_MASK, 1, 16 );
	//m_Images.SetBkColor ( RGB ( 255, 255, 255 ) );
	
	DeleteAllItems ( );
	SetImageList ( &m_Images, LVSIL_NORMAL );
	SetIconSpacing( 0, 130 );

	SetRedraw( FALSE );

	int iGroupID = 0;
	MarketGroup *pGroup = NULL;
	MarketObject *pObject = NULL;

	bool bIsSearch = false;
	bool bIsCart = false;
	bool bIsRoot = false;
	bool bIsItems = false;

	bool bNeedThumbnailUpdate = false;
	CString sThumbnailsNeeded = "";
	
	if ( iGroup == 0 ) bIsRoot = true;
	else if ( iGroup == -1 ) bIsSearch = true;
	else if ( iGroup == -2 ) bIsCart = true;
	else if ( iGroup == -3 ) bIsItems = true;
	else if ( iGroup == -4 ) iGroupID = -4;
	else
	{
		pGroup = MarketObject::GetGroup( iGroup );
		if ( pGroup ) iGroupID = pGroup->dwGroupID;
	}

	int imageCount = 0;
	int count = 0;

	if ( bIsRoot || bIsItems || (pGroup && pGroup->pChildGroup) )
	{
		bool bIsArtistList = false;

		MarketGroup *pChild;
		if ( pGroup ) pChild = pGroup->pChildGroup;
		else if ( bIsItems ) { pChild = MarketObject::GetGroupList(); bIsArtistList = true; }
		else if ( bIsRoot )
		{
			//main menu
			CBitmap bmpFolder;
			bmpFolder.LoadBitmap( IDB_FOLDER_ICON );

			m_Images.Add( &bmpFolder, (CBitmap*)NULL );
			imageCount++;

			CString sSearchResults = GetLanguageData( "TGC Store", "SearchResults" );
			CString sShoppingCart = GetLanguageData( "TGC Store", "ShoppingCart" );
			CString sFreeItems = GetLanguageData( "TGC Store", "FreeItems" );
			CString sOnlineItems = GetLanguageData( "TGC Store", "OnlineItems" );

			sSearchResults += "\\";
			sShoppingCart += "\\";
			sFreeItems += "\\";
			sOnlineItems += "\\";

			InsertItem ( 0, sSearchResults, 0 );
			InsertItem ( 1, sShoppingCart, 0 );
			InsertItem ( 2, sOnlineItems, 0 );
			
			SetRedraw( TRUE );
			Invalidate( );
			return;
		}

		int iGroupCount = 0;

		while ( pChild )
		{
			iGroupCount++;
			pChild = pChild->pNextGroup;
		}

		MarketGroup** pSortedGroupList = new MarketGroup* [ iGroupCount ];

		if ( pGroup ) pChild = pGroup->pChildGroup;
		else pChild = MarketObject::GetGroupList();
		iGroupCount = 0;

		while ( pChild )
		{
			pSortedGroupList [ iGroupCount ] = pChild;
			iGroupCount++;
			pChild = pChild->pNextGroup;
		}

		::sort( pSortedGroupList, pSortedGroupList + iGroupCount, cmpG );

		CBitmap bmpFolder;
		bmpFolder.LoadBitmap( IDB_FOLDER_ICON );

		m_Images.Add( &bmpFolder, (CBitmap*)NULL );
		imageCount++;

		if ( bIsArtistList )
		{
			bool bNeedImages = false;

			for( int i=0; i < iGroupCount; i++ )
			{
				CString sName = pSortedGroupList [ i ]->szName;

				EnterCriticalSection( &theApp.m_csDirectoryChanges );
				SetCurrentDirectory( theApp.m_szDirectory );
				CString path = "Files\\TGCStore\\TEMP\\Artists\\";
				path += sName;
				path += ".bmp";
				CImage image;
				HRESULT hr = image.Load( path );
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );

				sName += "\\";

				if ( FAILED(hr) )
				{
					InsertItem ( count++, sName, 0 );
					bNeedImages = true;
				}
				else
				{
					CWindowDC wndDC( this );
					CDC dc;
					dc.CreateCompatibleDC( &wndDC );

					CBitmap	bitmap; bitmap.CreateBitmap( 64,64, 1, 32, NULL );
					CBitmap *oldBmp = dc.SelectObject( &bitmap );

					image.StretchBlt( dc.GetSafeHdc( ), 0,0, 64,64 );
					dc.SelectObject( oldBmp );

					m_Images.Add( &bitmap, (CBitmap*)NULL );
					InsertItem ( count++, sName, imageCount++ );
				}
			}

			if ( bNeedImages && m_pParent && !m_pParent->AreArtistIconsLoaded() ) m_pParent->SetTimer( 5, 1000, NULL );
		}
		else
		{
			for( int i=0; i < iGroupCount; i++ )
			{
				CString sName = pSortedGroupList [ i ]->szName;
				sName += "\\";
				InsertItem ( count++, sName, 0 );
			}
		}

		delete [] pSortedGroupList;
	}

	CBitmap bmpNoThumbnail;
	bmpNoThumbnail.LoadBitmap( IDB_NO_THUMBNAIL );
	m_Images.Add( &bmpNoThumbnail, (CBitmap*)NULL );
	int iNoThumbnailIcon = imageCount;
	imageCount++;

	if ( bIsSearch ) pObject = MarketObject::GetSearchList( );
	else if ( bIsCart ) pObject = MarketObject::GetCartList( );
	else pObject = MarketObject::GetObjectList( );

	int iObjCount = 0;

	while ( pObject )
	{
		if ( ( bShowPurchased || !pObject->IsPurchased() ) && ( bIsSearch || bIsCart || iGroupID == -4 || pObject->GetGroupID( ) == iGroupID ) )
		{
			iObjCount++;
		}

		pObject = pObject->pNextObject;
	}

	if ( iObjCount == 0 && count == 0 )
	{
		//InsertItem ( 0, "<No Items>", -1 );
		if ( m_pParent ) m_pParent->ShowNoItemsMsg();
		if ( m_pParent ) m_pParent->SetPageNumbers( 1,1 );

		SetRedraw( TRUE );
		Invalidate();
		m_pParent->m_textNoItems.Invalidate();
		return;
	}

	if ( iObjCount == 0 )
	{
		if ( m_pParent ) m_pParent->SetPageNumbers( 1,1 );
	}
	else
	{
		if ( bIsSearch ) pObject = MarketObject::GetSearchList( );
		else if ( bIsCart ) pObject = MarketObject::GetCartList( );
		else pObject = MarketObject::GetObjectList( );

		MarketObject **pSortedList = new MarketObject* [ iObjCount ];
		iObjCount = 0;

		while ( pObject )
		{
			if ( ( bShowPurchased || !pObject->IsPurchased() ) && ( bIsSearch || bIsCart || iGroupID == -4 || pObject->GetGroupID( ) == iGroupID ) )
			{
				pSortedList [ iObjCount ] = pObject;
				iObjCount++;
			}

			pObject = pObject->pNextObject;
		}

		if ( szOrderBy && strlen( szOrderBy ) > 0 )
		{
			MarketObject::SetSortIndex( szOrderBy );

			::sort( pSortedList, pSortedList + iObjCount, cmpO );
		}

		//HZIP zpImages = OpenZip( "Files\\TGCStore\\TEMP\\Thumbnails.zip", NULL );
		//if ( !zpImages ) MessageBox( "Failed to open image zip" );
		//int count = 0;
		//int imageCount = 0;

		int start = 0;
		int end = iObjCount;
		if ( itemsperpage > 0 ) 
		{
			int iMaxPage = (iObjCount-1) / itemsperpage + 1;
			if ( page > iMaxPage ) page = iMaxPage;
			if ( page < 1 ) page = 1;

			if ( m_pParent ) m_pParent->SetPageNumbers( page, iMaxPage );

			if ( iObjCount > itemsperpage )
			{
				start = (page-1) * itemsperpage; 
				end = (page * itemsperpage);
				if ( end > iObjCount ) end = iObjCount;
			}
		}
		
		for ( int i = start; i < end; i++ )
		{
			pObject = pSortedList [ i ];

			try
			{
				if ( !pObject->GetThumbnail( ) ) throw 1;
			
				/*
				int iIndex = -1;
				ZRESULT result = FindZipItem( zpImages, pObject->GetThumbnail( ), true, &iIndex, NULL );
				if ( result != ZR_OK )
				{
					//char str [ 32 ];
					//sprintf_s( str, 32, "Error: %d", result );
					//MessageBox( str );
					//MessageBox( pObject->GetThumbnail( ) );
				}

				if ( iIndex < 0 ) throw 2;
				*/

				EnterCriticalSection( &theApp.m_csDirectoryChanges );

				SetCurrentDirectory( theApp.m_szDirectory );

				CString path = "Files\\TGCStore\\TEMP\\Thumbnails\\";
				path += pObject->GetThumbnail( );
				
				CImage image;
				HRESULT hr = image.Load( path );
				
				LeaveCriticalSection( &theApp.m_csDirectoryChanges );

				if ( FAILED(hr) )
				{
					if ( bNeedThumbnailUpdate ) sThumbnailsNeeded += ";";
					char szID [ 12 ];
					sprintf_s( szID, 12, "%d", pObject->GetID() );
					sThumbnailsNeeded += szID;
					bNeedThumbnailUpdate = true;
					throw 3;
				}

				CWindowDC wndDC( this );
				CDC dc;
				dc.CreateCompatibleDC( &wndDC );

				CBitmap	bitmap; bitmap.CreateBitmap( 64,64, 1, 32, NULL );
				CBitmap *oldBmp = dc.SelectObject( &bitmap );

				image.StretchBlt( dc.GetSafeHdc( ), 0,0, 64,64 );
				dc.SelectObject( oldBmp );

				m_Images.Add( &bitmap, (CBitmap*)NULL );
				//bitmap.DeleteObject( );

				InsertItem ( count, pObject->GetName( ), imageCount );
				if ( count <= MAX_ITEMS_PER_PAGE ) m_dwObjID [ count ] = pObject->GetID();
				count++;
				imageCount++;
			}
			catch ( int )
			{
				InsertItem ( count, pObject->GetName( ), iNoThumbnailIcon );
				if ( count <= MAX_ITEMS_PER_PAGE ) m_dwObjID [ count ] = pObject->GetID();
				count++;
			}
		}

		delete [] pSortedList;

//		CloseZip( zpImages );

		//SetCurrentDirectory( szOldDir );
	}

	if ( bNeedThumbnailUpdate && !bThumbnailsUpdate )
	{
		if ( m_pParent ) m_pParent->UpdateThumbnails( sThumbnailsNeeded );
	}

	SetRedraw( TRUE );
	Invalidate( );
	m_pParent->m_textNoItems.Invalidate();
}
/*
void cMarketImage::AddGroupsToTree( cLibraryTree *pTree, HTREEITEM hRoot )
{
	MarketGroup *pGroup = MarketObject::GetGroupList( );

	while ( pGroup )
	{
		pTree->InsertItemEx( hRoot, NULL, pGroup->szName, 1 );

		pGroup = pGroup->pNextGroup;
	}
}
*/

void cMarketImage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl ::OnHScroll(nSBCode, nPos, pScrollBar);
}

void cMarketImage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl ::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL cMarketImage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CListCtrl ::OnMouseWheel(nFlags, zDelta, pt);
}

void cMarketImage::OnVScrollClipboard(CWnd* pClipAppWnd, UINT nSBCode, UINT nPos) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl ::OnVScrollClipboard(pClipAppWnd, nSBCode, nPos);
}

void cMarketImage::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl ::OnMouseMove(nFlags, point);
}

void cMarketImage::OnKeyDown( UINT nChar, UINT nRepCount, UINT nFlags )
{
	CListCtrl::OnKeyDown( nChar, nRepCount, nFlags );

	if ( nChar == 8 ) m_pParent->BackSpace();

	//char str [ 256 ];
	//sprintf_s( str, 256, "KeyDown: %d", nChar );
	//MessageBox( str );
}

void cMarketImage::OnSize( UINT nType, int cx, int cy )
{
	CListCtrl::OnSize( nType, cx, cy );

	if ( m_pParent ) 
	{
		CRect rect;
		GetClientRect( rect );
		ClientToScreen( rect );
		m_pParent->ResizeAllPanes( rect.left,rect.top, cx,cy );
	}
}

#endif