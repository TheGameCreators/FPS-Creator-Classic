#if !defined(AFX_BROWSER_H__7F4887F9_68EB_4AAB_B0D8_B71789EB7360__INCLUDED_)
#define AFX_BROWSER_H__7F4887F9_68EB_4AAB_B0D8_B71789EB7360__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class CBrowserNew;

#include "resource.h"
#include "TabPageSSL.h"

class CBrowser : public CTabPageSSL
{
	public:
		CBrowser ( CWnd* pParent = NULL );

		void SetParentSize    ( int iWidth, int iHeight );
		void AddBitmap        ( LPTSTR szBitmap, LPTSTR szName );
		void RemoveBitmap     ( int iIndex );
		void RemoveAllBitmaps ( void );

		CImageList		m_imageList;
		CBrowserNew*	m_pTest;
		// 250105
		int				m_iArray [ 1000 ];
		int				m_iArrayCount;

		//{{AFX_DATA(CBrowser)
		enum { IDD = IDD_BROWSER };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(CBrowser)
		protected:
		virtual void DoDataExchange ( CDataExchange* pDX );
		//}}AFX_VIRTUAL

	protected:
		bool		bGo;
		int			m_iParentWidth;
		int			m_iParentHeight;
		int			m_iCount;
	
		//{{AFX_MSG(CBrowser)
		afx_msg int OnCreate        (LPCREATESTRUCT lpCreateStruct );
		afx_msg void OnShowWindow   ( BOOL bShow, UINT nStatus );
		afx_msg void OnSize			( UINT nType, int cx, int cy );
		afx_msg void OnLButtonUp	( UINT nFlags, CPoint point );
		afx_msg void OnMouseMove	( UINT nFlags, CPoint point );
		afx_msg void OnClickList    ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnClickListDbl ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg void OnKeyDown      ( NMHDR* pNMHDR, LRESULT* pResult );
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnTimer ( UINT nIDEvent );
		//}}AFX_MSG
	
		DECLARE_MESSAGE_MAP ( )
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSER_H__7F4887F9_68EB_4AAB_B0D8_B71789EB7360__INCLUDED_)
