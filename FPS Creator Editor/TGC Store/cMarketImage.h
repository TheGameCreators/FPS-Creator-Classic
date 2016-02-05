#ifndef _H_MARKETIMAGE
#define _H_MARKETIMAGE

#if _MSC_VER > 1000
#pragma once
#endif

class CMarketWindow;
#include "HTTPComm.h"

#define MAX_ITEMS_PER_PAGE 100

class cMarketImage : public CListCtrl
{
	public:
		cMarketImage ( );

	public:

	public:
		int  Start      ( CMarketWindow *parent );
		void LoadRemoteImages ( int iGroup, bool bShowPurchased, LPCSTR szOrderBy, int pages, int itemsperpage = 40, bool bThumbnailsUpdate = false );	//items for the market place
		//void AddGroupsToTree( cLibraryTree *pTree, HTREEITEM hRoot );
		int GetItemClicked( ) { return m_iItemClicked; }
		DWORD GetObjectRClicked() { return m_dwObjectRClicked; }
		//void SetItemClicked( int item ) { m_iItemClicked = item; }

		void SetScrollRange( int nBar, int nMinPos, int nMaxPos, BOOL bRedraw = TRUE )
		{
			//MessageBox( "Scroll Range" );
			CWnd::SetScrollRange( nBar, nMinPos, nMaxPos, bRedraw );
		}

		DWORD GetObjSelected( );
		DWORD GetObjByIndex( DWORD dwIndex );


		//TCHAR	m_szImageFileNames [ 100 ] [ 255 ];
		//TCHAR	m_szImageExtra [ 100 ] [ 255 ];
		//int		m_iImageCount;

		//{{AFX_VIRTUAL(cLibraryImage)
		//}}AFX_VIRTUAL

	public:
		virtual ~cMarketImage ( );
		void	OnClickList    ( NMHDR* pNMHDR, LRESULT* pResult );
		void	OnRClickList   ( NMHDR* pNMHDR, LRESULT* pResult, int iGroup, CWnd *pParent );

		

	protected:
		//{{AFX_MSG(cLibraryImage)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScrollClipboard(CWnd* pClipAppWnd, UINT nSBCode, UINT nPos);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCount, UINT nFlags );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

		CImageList	m_Images;
		int m_iItemClicked;
		DWORD m_dwObjectRClicked;

		CMarketWindow *m_pParent;

		DWORD m_dwObjID [ MAX_ITEMS_PER_PAGE + 1 ];
};

#endif 
