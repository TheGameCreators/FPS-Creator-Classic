#if !defined(AFX_CBUILDGAMEPROJECT_H__86AC97DC_65D1_40CA_B121_7CC063F27526__INCLUDED_)
#define AFX_CBUILDGAMEPROJECT_H__86AC97DC_65D1_40CA_B121_7CC063F27526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "cControls.h"

class cBuildGameProject : public CTabPageSSL
{
	public:
		cBuildGameProject(CWnd* pParent = NULL);   // standard constructor

		void SetGameProjectFilename ( CString filename );
		void AddToCommandList ( CString item );

		void Update ( cIPC* pIPC );

		void SetKey ( CString string );
		void SetControl ( cControls* pControl );

		//{{AFX_DATA(cBuildGameProject)
		enum { IDD = IDD_BUILD_GAME_PROJECT };
		//}}AFX_DATA

	//{{AFX_VIRTUAL(cBuildGameProject)
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	protected:

		// Generated message map functions
		//{{AFX_MSG(cBuildGameProject)
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnButtonProjectFileName();
		afx_msg void OnClickList(void);
		//afx_msg void OnClickList(NMHDR* pNMHDR);
		afx_msg void OnClickListCommands(void);
		afx_msg void OnClickListWeaponsNumber(void);
afx_msg void OnClickListWeaponsType(void);
		virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonProjectNew();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

		//CListCtrl	m_Controls;
		CListBox	m_Controls;
		int			m_iControlsCount;
		//CListCtrl	m_Action;
		CListBox	m_Action;
		int			m_iActionCount;
		CEdit		m_Filename;
		CEdit		m_Info;
		CEdit		m_Text;

		CListBox	m_WeaponsNumber;
		CListBox	m_WeaponsType;

		CString		szFileUNI;

		CString		m_Key;
		bool		m_bKeySet;
		int			m_iKeyIndex;

		int			m_iCommandCount;

		cControls*		m_pControls;

		bool		m_bWeaponType;

		bool		m_bButtonGameProjectFileName;
		bool		m_bBrowseFileSelected;

		//void OnUpdateGameProjectFileName ( void );
		void OnUpdateGameProjectBrowse   ( void );
		void OnUpdateKeyControls		 ( void );
		void OnUpdateEditControls		 ( void );

		//BYTE*	m_pData;
		cIPC* m_pIPC;

		CEdit			m_Lines [ 6 ];
		
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUILDGAMEPROJECT_H__86AC97DC_65D1_40CA_B121_7CC063F27526__INCLUDED_)
