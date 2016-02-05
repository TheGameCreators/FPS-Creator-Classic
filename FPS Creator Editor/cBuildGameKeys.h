#if !defined(AFX_CBUILDGAMEKEYS_H__C88FDF6A_528F_40C4_9626_5F619E59D7D8__INCLUDED_)
#define AFX_CBUILDGAMEKEYS_H__C88FDF6A_528F_40C4_9626_5F619E59D7D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "cBuildGameProject.h"

class cBuildGameKeys : public CDialog
{
	public:
		cBuildGameKeys ( cBuildGameProject* pParent = NULL );

				CString				m_Key;
						int uiVirtKey;

		//{{AFX_DATA(cBuildGameKeys)
		enum { IDD = IDD_BUILD_GAME_KEYS };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(cBuildGameKeys)
		protected:
			virtual void DoDataExchange(CDataExchange* pDX);
		//}}AFX_VIRTUAL

	protected:
		//{{AFX_MSG(cBuildGameKeys)
		afx_msg void OnOk();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

			CButton			m_Button;
			CEdit			m_Static;

		void GetKey ( void );


		//CEdit				m_Key;
		cBuildGameProject*	m_pParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUILDGAMEKEYS_H__C88FDF6A_528F_40C4_9626_5F619E59D7D8__INCLUDED_)
