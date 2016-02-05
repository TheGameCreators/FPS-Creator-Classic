#if !defined(AFX_CBUILDGAMEBUILD_H__DCF6D582_4D30_47D4_BAB0_7B6D20BD6823__INCLUDED_)
#define AFX_CBUILDGAMEBUILD_H__DCF6D582_4D30_47D4_BAB0_7B6D20BD6823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "cControls.h"

class cBuildGameBuild : public CTabPageSSL
{
	public:
		cBuildGameBuild ( CWnd* pParent = NULL );

		void Update     ( cIPC* pIPC );
		void SetControl ( cControls* pControl );

		void DisableAll ( void );
		void EnableAll ( void );

		//{{AFX_DATA(cBuildGameBuild)
		enum { IDD = IDD_BUILD_GAME_BUILD };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(cBuildGameBuild)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		//}}AFX_VIRTUAL

	protected:
		//{{AFX_MSG(cBuildGameBuild)
		afx_msg void OnSize(UINT nType, int cx, int cy);
		virtual BOOL OnInitDialog();
		afx_msg void OnButtonBuild();
		afx_msg void OnDestroy();
		afx_msg void OnButtonExecutablePath();
		afx_msg void OnRadioNoLightmaps();
		afx_msg void OnRadioSoftLightmaps();
		afx_msg void OnRadioFullLightmaps();
		afx_msg void OnRadioNoEffects();
		afx_msg void OnRadioEssentialEffects();
		afx_msg void OnRadioFullEffects();
		afx_msg void OnRadioHSR();
		afx_msg void OnRadioDynamicShadows();
		afx_msg void OnRadioDynamicLights();
	afx_msg void OnNoFloor();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

		CEdit			m_Path;
		CProgressCtrl	m_Progress;
		CEdit			m_Status;
		CButton			m_Build;
		CButton			m_ExePath;
		bool			m_BuildClicked;
		//BYTE*			m_pMessageData;
		cIPC*			m_pIPC;
		bool			m_bValid;
		cControls*		m_pControls;
		CButton			m_NoEffects;
		CButton			m_EssentialEffects;
		CButton			m_FullEffects;
		CButton			m_HSR;
		CButton			m_DynamicShadows;
		CButton			m_DynamicLights;
		CButton			m_NoFloor;

		CEdit			m_Name;

		void OnUpdateProgressBar ( void );
		void OnUpdateInformation ( void );

		CEdit			m_Lines   [ 13 ];
		CButton			m_Buttons;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUILDGAMEBUILD_H__DCF6D582_4D30_47D4_BAB0_7B6D20BD6823__INCLUDED_)
