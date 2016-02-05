// CMarketTree.cpp : implementation file
//

#include "stdafx.h"
#include "..\editor.h"
#include "MarketTree.h"

#ifdef TGC_STORE
	//#include "cLibraryImage.h"
	#include "cMarketImage.h"
	#include "MarketWindow.h"
	#include "MarketObject.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct sTreeDataM
{
	HTREEITEM	item;
	CString		displayName;
	CString		realName;
};

vector < sTreeDataM > g_TreeData;

CMarketTree::CMarketTree ( )
{
	m_hCheck = 0;
	m_hLoading = 0;
	//m_hMarketPlace = 0;
	m_iStarted = 0;
	m_bUpdating = false;
}

CMarketTree::~CMarketTree ( )
{
}


BEGIN_MESSAGE_MAP(CMarketTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CMarketTree)
	//ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CMarketTree::Start ( CMarketWindow *pParent, /*cLibraryImage* pImage,*/ cMarketImage* pMarketImage, int iScroll )
{
	g_TreeData.clear ( );

	DeleteAllItems( );
	m_pParentWindow = pParent;
	m_pMarketImage = pMarketImage;

	/*
	m_hHome = InsertItem( "Home" );
	m_hSearchResults = InsertItem( "Search Results" );
	m_hShoppingCart = InsertItem( "Shopping Cart" );
	m_hFreeItems = InsertItem( "Free Items" );
	m_hOnlineItems = InsertItemEx( TVI_ROOT, NULL, "Online Items", -1 );
	*/

	m_hHome = InsertItem( GetLanguageData( _T("TGC Store"), _T("Home") ) );
	m_hSearchResults = InsertItem( GetLanguageData( _T("TGC Store"), _T("SearchResults") ) );
	m_hShoppingCart = InsertItem( GetLanguageData( _T("TGC Store"), _T("ShoppingCart") ) );
	m_hFreeItems = InsertItem( GetLanguageData( _T("TGC Store"), _T("FreeItems") ) );
	m_hOnlineItems = InsertItemEx( TVI_ROOT, NULL, GetLanguageData( _T("TGC Store"), _T("OnlineItems") ), -1 );

	m_iStarted = 1;

	m_hLoading = InsertItem( GetLanguageData( _T("TGC Store"), _T("Loading") ), m_hOnlineItems );
	Expand( m_hOnlineItems, TVE_EXPAND );
	//m_hLoading = InsertItemEx( TVI_ROOT, NULL, "Loading...", -1 );
	
	SetRedraw(FALSE);
	
	//ExpandItem();
	//Expand( m_hMarketPlace, TVE_COLLAPSE );
	m_iStarted = 2;
	
	SetRedraw(TRUE);

	//this->Select( m_hRoot, TVGN_CARET );

	if (  m_hCheck )
		this->Select( m_hCheck, TVGN_CARET );

	m_bCreating = false;

	return 0;
}


bool CMarketTree::ParsePath ( CString sPath, int nLevel, CString* const psDirName )
{
	ASSERT ( psDirName );
	
	int nStart = 0;

	while ( nLevel )
	{
		nStart = sPath.Find ( _T ( '\\' ), nStart );

		if ( nStart < 0 )
			return false;

		nLevel--;
		nStart++;
	}

	int nEnd = sPath.Find ( _T ( '\\' ), nStart );

	if ( nEnd < 0 )
		*psDirName = sPath.Mid ( nStart );
	else
		*psDirName = sPath.Mid ( nStart, nEnd - nStart );

	return true;
}

HTREEITEM CMarketTree::InsertItemEx ( HTREEITEM hParent, NETRESOURCE* const pNetResource, CString sText, int iImage, int iImageSelected, HTREEITEM hInsertAfter )
{
	TVINSERTSTRUCT InsertStruct;

	InsertStruct.hParent			= hParent;	
	InsertStruct.hInsertAfter		= hInsertAfter;
	InsertStruct.itemex.mask		= TVIF_IMAGE | TVIF_TEXT | TVIF_CHILDREN | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	InsertStruct.itemex.pszText		= sText.GetBuffer ( sText.GetLength ( ) );
	InsertStruct.itemex.iImage		= iImage;
	InsertStruct.itemex.cChildren	= 1;
	InsertStruct.itemex.lParam		= ( LPARAM ) pNetResource;

	if ( iImageSelected == -1 )
		InsertStruct.itemex.iSelectedImage = iImage;
	else
		InsertStruct.itemex.iSelectedImage = iImageSelected;

	sText.ReleaseBuffer ( );

	return this->InsertItem ( &InsertStruct );
}

CString CMarketTree::GetItemPath ( HTREEITEM hItem ) 
{
	CString sRet;

	do
	{
		NETRESOURCE* const pNetResource = ( NETRESOURCE* ) ( this->GetItemData ( hItem ) );
		
		if ( pNetResource )
		{
			sRet = CString ( pNetResource->lpRemoteName ) + _T( '\\' ) + sRet; 		
			break;
		}
		
		sRet  = this->GetItemText   ( hItem ) + _T ( '\\' ) + sRet; 
		hItem = this->GetParentItem ( hItem );
	}
	while ( hItem );

	return sRet;
}

void CMarketTree::OnSelchangedTree ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = ( NM_TREEVIEW* ) pNMHDR;

	ASSERT ( pNMTreeView );
	ASSERT ( pResult );

	SetRedraw( 0 );
	
	CString sPath;
	sPath = GetItemPath ( pNMTreeView->itemNew.hItem );
	
	m_LastPath = sPath;

	m_sSelectionPath = sPath;

	if ( !m_bCreating && !m_bUpdating )
	{
		char szPath [ 255 ] = "";
		ConvertWideToANSI ( NULL, &sPath, szPath );

		//strcpy ( m_szLastPath, szPath );

		//char *szItems = "Online Items\\";
		CString sItems = GetLanguageData( _T("TGC Store"), _T("OnlineItems") );
		sItems += "\\";

		//MessageBox( szPath, "path", 0 );

		if ( strcmp( szPath, sItems ) == 0 )
		{
			if ( m_iStarted == 2 )
			{
				m_iStarted = 3;
				m_pParentWindow->LoadTGCStore( );
				//Expand( m_hOnlineItems, TVE_EXPAND );
				//MessageBox( sPath );
			}
			else if ( m_iStarted == 4 )
			{
				Expand( m_hOnlineItems, TVE_EXPAND );
			}
		}
		
		//if ( strnicmp ( szPath, szMarketPlace, strlen ( szMarketPlace ) ) == 0 )
		{
			//char *szGroupName = strchr( szPath, '\\' );
			//if ( szGroupName ) szGroupName++;
			
			if ( szPath [ strlen(szPath) - 1 ] == '\\' )
			{
				szPath [ strlen(szPath) - 1 ] = '\0';
			}

			m_pParentWindow->SwitchToMarketPlace( TRUE, szPath );

			//MessageBox( szPath, "group", 0 );
		}	
	}
	
	SetRedraw( 1 );

	*pResult = 0;
}

void CMarketTree::OnItemexpandingTree ( NMHDR* pNMHDR, LRESULT* pResult )
{
	CWaitCursor CursorWaiting;
	NM_TREEVIEW* pNMTreeView = ( NM_TREEVIEW* ) pNMHDR;
	
	ASSERT ( pNMTreeView );
	ASSERT ( pResult );

	SetRedraw( 0 );

	if ( pNMTreeView->action == 2 )
	{
		CString sPath = GetItemPath ( pNMTreeView->itemNew.hItem );

		//MessageBox( sPath );

		//if ( strstr( sPath, "TGC Store" ) )
		{
			if ( m_iStarted == 2 )
			{
				m_iStarted = 3;
				
				m_pParentWindow->LoadTGCStore( );
				//MessageBox( sPath );
			}
			
			/*
			if ( m_iStarted == 3 )
			{
				char *szGroupName = strchr( sPath.GetBuffer(), '\\' );
				if ( szGroupName ) szGroupName++;
				
				if ( szGroupName [ strlen(szGroupName) - 1 ] == '\\' )
				{
					szGroupName [ strlen(szGroupName) - 1 ] = '\0';
				}

				m_pParentWindow->SwitchToMarketPlace( TRUE, szGroupName );
				sPath.ReleaseBuffer( );
			}
			*/
		}
		//else
		{
			/*
			if ( !this->GetChildItem ( pNMTreeView->itemNew.hItem ) )
			{
				PopulateTree ( sPath, pNMTreeView->itemNew.hItem );

				if ( this->GetSelectedItem ( ) != pNMTreeView->itemNew.hItem )
					this->SelectItem ( pNMTreeView->itemNew.hItem );
			}
			*/
		}
	}

	SetRedraw( 1 );
	

	*pResult = 0;
}

void CMarketTree::SetCartCount( int count )
{
	if ( !m_hShoppingCart ) return;
	CString sShoppingCart = GetLanguageData( "TGC Store", "ShoppingCart" );
	CString sCartLabel;
	sCartLabel.Format( "%s (%d)", sShoppingCart, count );
	SetItemText( m_hShoppingCart, sCartLabel );
}

void CMarketTree::SetSearchCount( int count )
{
	if ( !m_hSearchResults ) return;
	CString sSearchResults = GetLanguageData( "TGC Store", "SearchResults" );
	CString sSearchLabel;
	sSearchLabel.Format( "%s (%d)", sSearchResults, count );
	SetItemText( m_hSearchResults, sSearchLabel );
}

void CMarketTree::SetObjectCount( int count )
{
	if ( !m_hOnlineItems ) return;
	CString sOnlineItems = GetLanguageData( "TGC Store", "OnlineItems" );
	CString sObjectLabel;
	sObjectLabel.Format( "%s (%d)", sOnlineItems, count );
	SetItemText( m_hOnlineItems, sObjectLabel );
}

CString CMarketTree::GetSelectionPath ( void )
{
	return m_sSelectionPath;
}

void CMarketTree::ExpandStore( )
{
	Expand( m_hOnlineItems, TVE_EXPAND );
}

void CMarketTree::SelectSearchResults( )
{
	if ( m_hSearchResults ) Select( m_hSearchResults, TVGN_CARET );
}

void CMarketTree::SelectShoppingCart( )
{
	if ( m_hShoppingCart ) Select( m_hShoppingCart, TVGN_CARET );
}

void CMarketTree::SelectOnlineItems( )
{
	if ( m_hOnlineItems ) 
	{
		Select( m_hOnlineItems, TVGN_CARET );
		Expand( m_hOnlineItems, TVE_EXPAND );
	}
}

void CMarketTree::SelectFreeItems()
{
	if ( m_hFreeItems ) 
	{
		Select( m_hFreeItems, TVGN_CARET );
	}
}

void CMarketTree::SelectHome()
{
	if ( m_hHome ) 
	{
		Select( m_hHome, TVGN_CARET );
	}
}

bool CMarketTree::IsLoading( )
{
	return m_hLoading != NULL;
}

void CMarketTree::AddSubCategories( HTREEITEM hParent, MarketGroup *pGroupList, int iCurrentSelection )
{
	while ( pGroupList )
	{
		HTREEITEM hItem = InsertItem( pGroupList->szName, hParent );
		if ( pGroupList->dwGroupID == iCurrentSelection ) m_hNewSelection = hItem;

		if ( pGroupList->pChildGroup )
		{
			AddSubCategories( hItem, pGroupList->pChildGroup, iCurrentSelection );
		}

		pGroupList = pGroupList->pNextGroup;
	}
}

void CMarketTree::UpdateStoreCategories( int iCurrentSelection )
{
	MarketGroup *pGroup = MarketObject::GetGroupList( );

	m_bUpdating = true;

	if ( m_hLoading ) this->DeleteItem( m_hLoading );
	m_hLoading = 0;
	m_hNewSelection = 0;

	if ( m_iStarted == 2 ) m_iStarted = 3;

	DeleteAllItems();
	//this->DeleteItem( m_hMarketPlace );
	//m_hMarketPlace = this->InsertItem( TVI_ROOT, NULL, "TGC Store", 1 );
	
	/*
	m_hHome = InsertItem( "Home" );
	m_hSearchResults = InsertItem( "Search Results" );
	m_hShoppingCart = InsertItem( "Shopping Cart" );
	m_hFreeItems = InsertItem( "Free Items" );
	m_hOnlineItems = InsertItemEx( TVI_ROOT, NULL, "Online Items", -1 );
	*/

	m_hHome = InsertItem( GetLanguageData( _T("TGC Store"), _T("Home") ) );
	m_hSearchResults = InsertItem( GetLanguageData( _T("TGC Store"), _T("SearchResults") ) );
	m_hShoppingCart = InsertItem( GetLanguageData( _T("TGC Store"), _T("ShoppingCart") ) );
	m_hFreeItems = InsertItem( GetLanguageData( _T("TGC Store"), _T("FreeItems") ) );
	m_hOnlineItems = InsertItemEx( TVI_ROOT, NULL, GetLanguageData( _T("TGC Store"), _T("OnlineItems") ), -1 );

	SetCartCount( MarketObject::CountCartObjects() );
	SetSearchCount( MarketObject::CountSearchObjects() );
	SetObjectCount( MarketObject::CountObjects() );

	AddSubCategories( m_hOnlineItems, pGroup, iCurrentSelection );

	if ( iCurrentSelection == 0 ) m_hNewSelection = m_hHome;
	else if ( iCurrentSelection == -1 ) m_hNewSelection = m_hSearchResults;
	else if ( iCurrentSelection == -2 ) m_hNewSelection = m_hShoppingCart;
	else if ( iCurrentSelection == -3 ) m_hNewSelection = m_hOnlineItems;
	else if ( iCurrentSelection == -4 ) m_hNewSelection = m_hFreeItems;

	ExpandStore( );

	if ( m_hNewSelection ) Select( m_hNewSelection, TVGN_CARET );
	else
	{
		//Select( m_hHome, TVGN_CARET );
	}

	m_iStarted = 4;

	m_bUpdating = false;
}