#if !defined(AFX_CMARKETTREE_H__2B9C19C1_A888_4E42_BEEE_BEC32071952B__INCLUDED_)
#define AFX_CMARKETTREE_H__2B9C19C1_A888_4E42_BEEE_BEC32071952B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMarketTree.h : header file
//


#include "..\TCExpand.h"
#include "..\TCPersist.h"
#include "..\TCIterator.h"


//class cLibraryImage;
class cMarketImage;
class CMarketWindow;
struct MarketGroup;

/////////////////////////////////////////////////////////////////////////////
// CMarketTree window



class CMarketTree : public TTCIterator<TTCPersist<TTCExpand<CTreeCtrl> > >
{
	public:
		CMarketTree ( );

		

	public:
		//cLibraryImage* m_pImage;
		cMarketImage* m_pMarketImage;
		CMarketWindow *m_pParentWindow;

	public:

		int		Start				( CMarketWindow *pParent, cMarketImage* pMarketImage, int iScroll );
		void UpdateStoreCategories( int iCurrentSelection );
		//void UpdateLocalCategories( );
		void AddSubCategories( HTREEITEM hParent, MarketGroup *pGroupList, int iCurrentSelection );
		HTREEITEM	InsertItemEx ( HTREEITEM hParent, NETRESOURCE* const pNetResource, CString sText, int iImage, int iImageSelected = -1, HTREEITEM hInsertAfter = TVI_LAST );
		void ExpandStore( );
		void SelectSearchResults( );
		void SelectStoreRoot();
		void SelectShoppingCart();
		void SelectOnlineItems();
		void SelectFreeItems();
		void SelectHome();
		bool IsLoading( );

		void	OnSelchangedTree	( NMHDR* pNMHDR, LRESULT* pResult );
		void	OnItemexpandingTree	( NMHDR* pNMHDR, LRESULT* pResult );
		CString GetSelectionPath	( void );

		void SetCartCount( int count );
		void SetSearchCount( int count );
		void SetObjectCount( int count );

		int GetStageValue( ) { return m_iStarted; }
		void ReloadTree( ) { m_iStarted = 3; }
	
		//{{AFX_VIRTUAL(CMarketTree)
		public:
		//}}AFX_VIRTUAL

	public:
		virtual ~CMarketTree ( );

	protected:
		//{{AFX_MSG(CMarketTree)
		afx_msg void OnSelChanged ( NMHDR* pNMHDR, LRESULT* pResult );
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

	private:
		bool		ParsePath    ( CString sPath, int nLevel, CString* const psDirName );

		CString		GetItemPath	 ( HTREEITEM hItem );

		CImageList	m_ImageListTree;
		CString		m_sPath;
		CString		m_sSelectionPath;
		CString		m_Path;
		CString		m_Name;

		HTREEITEM	m_hCheck;

		CString		m_LastPath;

		bool		m_bCreating;

		//HTREEITEM	m_hMarketPlace;
		HTREEITEM	m_hHome;
		HTREEITEM	m_hSearchResults;
		HTREEITEM	m_hShoppingCart;
		HTREEITEM	m_hOnlineItems;
		HTREEITEM   m_hFreeItems;

		//HTREEITEM	m_hPurchased;
		HTREEITEM	m_hLoading;
		HTREEITEM   m_hNewSelection;

		int			m_iStarted;
		bool		m_bUpdating;

		//void ExpandItem(HTREEITEM hti, UINT nCode);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMARKETTREE_H__2B9C19C1_A888_4E42_BEEE_BEC32071952B__INCLUDED_)
