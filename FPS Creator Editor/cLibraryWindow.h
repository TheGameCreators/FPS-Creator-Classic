#if !defined(AFX_CLIBRARYWINDOW_H__58FE114A_C1EA_4A01_AAA9_05B985F89A6D__INCLUDED_)
#define AFX_CLIBRARYWINDOW_H__58FE114A_C1EA_4A01_AAA9_05B985F89A6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "cLibraryTree.h"
#include "cLibraryImage.h"
#include "ZSplitterDlg.h"

class cLibraryWindow : public CDialog
{
	public:
		cLibraryWindow ( CWnd* pParent = NULL );
		~cLibraryWindow ();

		//{{AFX_DATA(cLibraryWindow)
		enum { IDD = IDD_LIBRARY };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(cLibraryWindow)
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

	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

	private:
		void UpdateImageList ( CString sPath );
		void PositionStatusText( );

		cLibraryTree	m_Tree;
		cLibraryImage	m_Image;
		ZSplitter		m_spliter;

		CButton			m_Ok;
		CButton			m_Cancel;
		CButton			m_btnSearch;
		CStatic			m_txtSearch;
		CStatic			m_ProgressSpinner;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIBRARYWINDOW_H__58FE114A_C1EA_4A01_AAA9_05B985F89A6D__INCLUDED_)
#include "explorer1.h"
#include "afxwin.h"
