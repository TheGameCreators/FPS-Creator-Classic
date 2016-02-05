#if !defined(AFX_CBUILDGAMELEVEL_H__6557E1AA_B17B_4B84_99EC_F58D162CF2E3__INCLUDED_)
#define AFX_CBUILDGAMELEVEL_H__6557E1AA_B17B_4B84_99EC_F58D162CF2E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "cControls.h"

extern bool g_bBuildGameInArenaMode;

class cBuildGameLevel : public CTabPageSSL
{
	public:
		cBuildGameLevel ( CWnd* pParent = NULL );
		
		void Update     ( cIPC* pIPC );
		void SetControl ( cControls* pControl );

		//{{AFX_DATA(cBuildGameLevel)
		enum { IDD = IDD_BUILD_GAME_LEVEL };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(cBuildGameLevel)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		//}}AFX_VIRTUAL

	protected:
		// Generated message map functions
		//{{AFX_MSG(cBuildGameLevel)
		afx_msg void OnSize(UINT nType, int cx, int cy);
		virtual BOOL OnInitDialog();
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnButtonTitleScreen    ( );
		afx_msg void OnButtonGlobalScript   ( );
		afx_msg void OnButtonGameOver       ( );
		afx_msg void OnButtonGameComplete   ( );
		afx_msg void OnButtonInsertLevel    ( );
		afx_msg void OnButtonDeleteLevel    ( );
		afx_msg void OnButtonNewLevelScript ( );
		afx_msg void OnButtonAddLevel ( );
		afx_msg void OnClickList1    ( );
		afx_msg void OnClickList2    (  );
		afx_msg void OnClickList3    (  );
		afx_msg void OnClickListTitleScreen (  );
		afx_msg void OnClickListGlobalScript (  );
		afx_msg void OnClickListGameOver (  );
		afx_msg void OnClickListGameComplete (  );
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

		CListBox		m_TitleScreen;
		CListBox		m_GlobalScript;
		CListBox		m_GameOver;
		CListBox		m_GameComplete;

		CListBox		m_MapNumbers;
		CListBox		m_MapNames;
		CListBox		m_MapLoadingScreen;

		CButton			m_ButtonTitleScreen;
		CButton			m_ButtonGlobalScript;
		CButton			m_ButtonGameOver;
		CButton			m_ButtonGameComplete;

		CButton			m_ButtonAddLevel;
		CButton			m_ButtonInsertLevel;
		CButton			m_ButtonPreviewLevel;
		CButton			m_ButtonDeleteLevel;
		CButton			m_ButtonNewLevelScript;

		bool			m_bListClicked [ 20 ];


		cControls*		m_pControls;

		BYTE*			m_pData;
		cIPC*			m_pIPC;

		void OnUpdateEditControls	( void );
		void OnUpdateButtons		( void );
		void OnUpdateBrowse			( void );

		CEdit			m_Lines   [ 13 ];
		CButton			m_Buttons [ 8 ];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUILDGAMELEVEL_H__6557E1AA_B17B_4B84_99EC_F58D162CF2E3__INCLUDED_)
