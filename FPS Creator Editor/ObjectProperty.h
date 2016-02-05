#if !defined(AFX_OBJECTPROPERTY_H__7B88BA06_94E7_46A1_8E51_4A7946B2DA0E__INCLUDED_)
#define AFX_OBJECTPROPERTY_H__7B88BA06_94E7_46A1_8E51_4A7946B2DA0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectProperty.h : header file
//

#include "cMessages.h"

#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty


class CObjectProperty : public CBCGPropList
{
	

	// Construction
	public:

	//	CEntityProperty* m_property;
	CButton				m_ApplyButton;
	bool				m_bApplyButton;

	CButton				m_CancelButton;
	bool				m_bCancelButton;

	// Attributes
	public:
		struct sItem
		{
			int			iGroup;
			int			iControl;

			CString		name;
			CString		contents;
			CString		description;

			CString		item;

			CString		extra;

			CString dir;
			CString title;
			CString filter;
		};

		CBCGProp*		m_pEditItem;

		bool	m_bModified;
		int		m_iGroup;
		int		m_iIndex;
		
		int		m_iGroupCount;
		
		
		void	AddGroup         ( CString name );
		void    DeleteGroup      ( int iIndex );
		void	DeleteControl    ( int iGroup, int iControl );

		void	AddControl		 ( int iType, int iGroup, int iControl, CString name, CString contents, CString description, CString extra, CString dir, CString title, CString filter );
		void    SetControl       ( int iGroup, int iControl, int iType, CString string );
		CString GetControl       ( int iGroup, int iControl, int iType );
		void	SetControlState  ( int iGroup, int iControl, int iState );

		
		void	AddItemToListBox ( int iGroup, int iControl, CString item );
		void	ClearListBox	 ( int iGroup, int iControl );

		void	Clear ( void );

		void	ShowApply ( bool bState );

		int		m_iX;
		int		m_iY;

	protected:

		BOOL m_bRedraw;
		virtual void OnDrawList (CDC* pDC);

		//bool	m_bClose;

		//void	ForceClose ( void );

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CObjectProperty)
		virtual BOOL EndEditItem (BOOL bUpdateData = TRUE);
		virtual BOOL EditItem (CBCGProp* pProp, LPPOINT lptClick = NULL);
		virtual void OnDrawDesciption (CDC* pDC, CRect rect);

		afx_msg void OnButtonApply    ( );
		afx_msg void OnButtonCancel    ( );
		//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~CObjectProperty();

		void TestUpdate ( );

		

		// Generated message map functions
	protected:
		//{{AFX_MSG(CObjectProperty)
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg LRESULT OnAddGroup (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnDeleteGroup (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnAddEditBox (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnDeleteControl (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnAddListBox (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnAddColorPicker (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnAddFilePicker (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnAddFontPicker (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnAddItemToListBox (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnClearListBox (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnSetControl (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnGetControl (WPARAM wParam, LPARAM lParam );
		afx_msg LRESULT OnSetControlState (WPARAM wParam, LPARAM lParam );
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

		//CBCGProp*		m_pGroups [ 24 ];

		struct sPropertyItem
		{
			CBCGProp* pProperty;
			int		  iType;
		};

		vector < sPropertyItem >   m_pGroups;


		char szLocation [ 256 ];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTPROPERTY_H__7B88BA06_94E7_46A1_8E51_4A7946B2DA0E__INCLUDED_)
