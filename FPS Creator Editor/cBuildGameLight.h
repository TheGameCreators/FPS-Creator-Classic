#if !defined(AFX_CBUILDGAMELIGHT_H__735503A1_B7EB_49E4_9A4F_EFC4A13C5E4A__INCLUDED_)
#define AFX_CBUILDGAMELIGHT_H__735503A1_B7EB_49E4_9A4F_EFC4A13C5E4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "cControls.h"

class cBuildGameLight : public CTabPageSSL
{
	public:
		cBuildGameLight ( CWnd* pParent = NULL );

		void Update     ( cIPC* pIPC );
		void SetControl ( cControls* pControl );

		//{{AFX_DATA(cBuildGameLight)
		enum { IDD = IDD_BUILD_GAME_LIGHT };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(cBuildGameLight)
		protected:
			virtual void DoDataExchange(CDataExchange* pDX);
		//}}AFX_VIRTUAL

	protected:
		//{{AFX_MSG(cBuildGameLight)
		afx_msg void OnRadioNoLightmaps();
		afx_msg void OnRadioSoftLightmaps();
		afx_msg void OnRadioFullShadowLightmaps();

		afx_msg void OnRadioLow();
		afx_msg void OnRadioMedium();
		afx_msg void OnRadioHigh();
		afx_msg int  OnCreate				 ( LPCREATESTRUCT lpCreateStruct );
		afx_msg void OnPaint();

		//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

		cControls*		m_pControls;
		CEdit			m_Info1;
		CEdit			m_Info2;

		CButton			m_NoLightMaps;
		CButton			m_SoftLightMaps;
		CButton			m_FullLightMaps;

		CButton			m_NoEffects;
		CButton			m_EssentialEffects;
		CButton			m_FullEffects;

		CButton			m_Low;
		CButton			m_Medium;
		CButton			m_High;

		void			CheckLightMapRadioButtons ( void );

		HDC		m_hDCSRC;
		HANDLE	m_hBMP;
		HANDLE	m_hOld;
		HDC		m_hDCSRC1;
		HANDLE	m_hBMP1;
		HANDLE	m_hOld1;

		CEdit			m_Lines   [ 13 ];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUILDGAMELIGHT_H__735503A1_B7EB_49E4_9A4F_EFC4A13C5E4A__INCLUDED_)
