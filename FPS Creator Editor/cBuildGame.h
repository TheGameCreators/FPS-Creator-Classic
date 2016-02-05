#if !defined(AFX_CBUILDGAME_H__C0F83DAC_908F_440A_B385_9D7DEE2D0C2A__INCLUDED_)
#define AFX_CBUILDGAME_H__C0F83DAC_908F_440A_B385_9D7DEE2D0C2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "TabCtrlSSL.h"
#include "cBuildGameProject.h"
#include "cBuildGameLevel.h"
#include "cBuildGameBuild.h"
#include "cBuildGameLight.h"

#include "cBuildGameMultiplayer.h"
#include "cMessages.h"
#include "cControls.h"

class cBuildGame : public CDialog, cBaseMessage
{
	public:
		cBuildGame ( CWnd* pParent = NULL );

		//{{AFX_DATA(cBuildGame)
		enum { IDD = IDD_BUILD_GAME };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(cBuildGame)
	public:
	virtual void OnCancel();
	virtual BOOL DestroyWindow();
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	protected:
		CTabCtrlSSL				m_tabCtrl;
		cBuildGameProject		m_Project;
		cBuildGameLevel			m_Level;
		cBuildGameLight			m_Light;
		cBuildGameMultiplayer	m_Multiplayer;
		cBuildGameBuild			m_Build;

		cMessages				m_Message;
		BYTE*					m_pData;
		cControls				m_Controls;

		bool					m_bOk;
		bool					m_bCancel;
		bool					m_bHelp;
		bool					m_bClose;

		bool					m_bClean;

		void HandleMessages ( void );
		bool OnUpdateDialog ( void );

		//{{AFX_MSG(cBuildGame)
		virtual BOOL OnInitDialog			 ( );
		afx_msg void OnSize					 ( UINT nType, int cx, int cy );
		afx_msg void OnDestroy				 ( );
		afx_msg int  OnCreate				 ( LPCREATESTRUCT lpCreateStruct );
		afx_msg void OnClose				 ( );
		afx_msg void OnBuildGameButtonOk	 ( );
		afx_msg void OnBuildGameButtonCancel ( );
		afx_msg void OnBuildGameButtonHelp   ( );

		afx_msg void OnOK();

		//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

		//CEdit			m_Lines [ 6 ];
		CButton			m_Buttons [ 2 ];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUILDGAME_H__C0F83DAC_908F_440A_B385_9D7DEE2D0C2A__INCLUDED_)
