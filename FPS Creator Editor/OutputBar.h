// OutputBar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTBAR_H__DA8E8AA5_1FD3_495E_B5B7_806265546E7B__INCLUDED_)
#define AFX_OUTPUTBAR_H__DA8E8AA5_1FD3_495E_B5B7_806265546E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "browser.h"
#include "browsernew.h"

class COutputBar : public CBCGSizingControlBar
{
	public:
		COutputBar ( );

		CBrowserNew m_NewBrowser;

	protected:
		CBCGTabWnd	m_wndTabs;
		CWnd		m_wnd;

		CListCtrl	m_wndList1;
		CListCtrl	m_wndList2;
		CListCtrl	m_wndList3;

		CBrowser	m_BrowserSegments;
		CBrowser	m_BrowserPrefabs;
		CBrowser	m_BrowserEntities;

	public:
		void SetBrowserSize ( int cx, int cy );
		void AddBitmap      ( int iTab, LPTSTR szBitmap, LPTSTR szName );
		void RemoveBitmap   ( int iTab, int iIndex );

		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(COutputBar)
		//}}AFX_VIRTUAL

	public:
		virtual ~COutputBar ( );

	protected:
		//{{AFX_MSG(COutputBar)
		afx_msg int OnCreate     ( LPCREATESTRUCT lpCreateStruct );
		afx_msg void OnSize      ( UINT nType, int cx, int cy );
		afx_msg void OnLButtonUp ( UINT nFlags, CPoint point );
		afx_msg void OnLButtonDown ( UINT nFlags, CPoint point );
		
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBAR_H__DA8E8AA5_1FD3_495E_B5B7_806265546E7B__INCLUDED_)
