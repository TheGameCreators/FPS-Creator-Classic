#if !defined(AFX_CLIBRARYTREE_H__2B9C19C1_A888_4E42_BEEE_BEC32071952B__INCLUDED_)
#define AFX_CLIBRARYTREE_H__2B9C19C1_A888_4E42_BEEE_BEC32071952B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cLibraryTree.h : header file
//

#include "TCExpand.h"
#include "TCPersist.h"
#include "TCIterator.h"

#include "cLibraryImage.h"

/////////////////////////////////////////////////////////////////////////////
// cLibraryTree window



class cLibraryTree : public TTCIterator<TTCPersist<TTCExpand<CTreeCtrl> > >
{
	public:
		cLibraryTree ( );

		

	public:
		cLibraryImage* m_pImage;

		int		Start				( cLibraryImage* pImage, int iScroll );

		void	OnSelchangedTree	( NMHDR* pNMHDR, LRESULT* pResult );
		void	OnItemexpandingTree	( NMHDR* pNMHDR, LRESULT* pResult );
		CString GetSelectionPath	( void );
		void	SaveTreeState( );
	
		//{{AFX_VIRTUAL(cLibraryTree)
		public:
		//}}AFX_VIRTUAL

	public:
		virtual ~cLibraryTree ( );

	protected:
		//{{AFX_MSG(cLibraryTree)
		afx_msg void OnSelChanged ( NMHDR* pNMHDR, LRESULT* pResult );
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

	private:
		bool		ParsePath    ( CString sPath, int nLevel, CString* const psDirName );

		HTREEITEM	InsertItemEx ( HTREEITEM hParent, NETRESOURCE* const pNetResource, CString sText, int iImage, int iImageSelected = -1, HTREEITEM hInsertAfter = TVI_LAST );
		bool		PopulateTree ( CString sPath, HTREEITEM hParent );
		CString		GetItemPath	 ( HTREEITEM hItem );

		CImageList	m_ImageListTree;
		CString		m_sPath;
		HTREEITEM	m_hRoot;
		CString		m_sSelectionPath;
		CString		m_Path;
		CString		m_Name;

		HTREEITEM	m_hCheck;

		CString		m_LastPath;

		bool		m_bCreating;	

		//void ExpandItem(HTREEITEM hti, UINT nCode);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIBRARYTREE_H__2B9C19C1_A888_4E42_BEEE_BEC32071952B__INCLUDED_)
