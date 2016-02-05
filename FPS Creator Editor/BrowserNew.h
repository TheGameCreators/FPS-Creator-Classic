#if !defined(AFX_BROWSERNEW_H__D3A9178D_37E2_4274_90F1_3B9DA90272D5__INCLUDED_)
#define AFX_BROWSERNEW_H__D3A9178D_37E2_4274_90F1_3B9DA90272D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "TabCtrlSSL.h"
#include "afxwin.h"
#include "TGC Store\StoreButton.h"

class CBrowserNew : public CDialog
{
	public:
		CBrowserNew ( CWnd* pParent = NULL );

		//{{AFX_DATA(CBrowserNew)
		enum { IDD = IDD_BROWSER_NEW };
		//}}AFX_DATA

		CTabCtrlSSL			m_tabCtrl;
		CBrowser			m_BrowserSegments;
		CBrowser			m_BrowserPrefabs;
		CBrowser			m_BrowserEntities;
		CBrowser			m_BrowserMarkers;

		void AddBitmap        ( int iTab, LPTSTR szBitmap, LPTSTR szName );
		void RemoveBitmap     ( int iTab, int iIndex);
		void RemoveAllBitmaps ( int iTab );

		//{{AFX_VIRTUAL(CBrowserNew)
		public:
			virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);
		//}}AFX_VIRTUAL

	protected:
		//{{AFX_MSG(CBrowserNew)
		afx_msg void OnPaint      ( );
		virtual BOOL OnInitDialog ( );
		afx_msg void OnSize       ( UINT nType, int cx, int cy );
		afx_msg void OnLButtonUp  ( UINT nFlags, CPoint point );
		afx_msg void OnClickList  ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnMouseMove  ( UINT nFlags, CPoint point );
		afx_msg void OnKeyDown    ( UINT nChar, UINT nRepCnt, UINT nFlags );
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP ()
public:
	CStoreButton m_btnStore;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERNEW_H__D3A9178D_37E2_4274_90F1_3B9DA90272D5__INCLUDED_)
