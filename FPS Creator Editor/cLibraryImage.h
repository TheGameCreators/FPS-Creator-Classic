#if !defined(AFX_CLIBRARYIMAGE_H__8245A900_A2A3_4EFD_AE6C_EC017761672E__INCLUDED_)
#define AFX_CLIBRARYIMAGE_H__8245A900_A2A3_4EFD_AE6C_EC017761672E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef TGC_STORE
class cLibraryWindow;
#endif

class cLibraryImage : public CListCtrl
{
	public:
		cLibraryImage ( );

		int  Start      ( void );
		void LoadImages ( CString sPath );

		TCHAR	m_szImageFileNames [ 1000 ] [ 255 ];
		TCHAR	m_szImageExtra [ 1000 ] [ 255 ];
		int		m_iImageCount;

		//{{AFX_VIRTUAL(cLibraryImage)
		//}}AFX_VIRTUAL

	public:
		virtual ~cLibraryImage ( );
		void	OnClickList    ( NMHDR* pNMHDR, LRESULT* pResult );

		

	protected:

		//{{AFX_MSG(cLibraryImage)
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnVScrollClipboard(CWnd* pClipAppWnd, UINT nSBCode, UINT nPos);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

		CImageList	m_Images;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIBRARYIMAGE_H__8245A900_A2A3_4EFD_AE6C_EC017761672E__INCLUDED_)
