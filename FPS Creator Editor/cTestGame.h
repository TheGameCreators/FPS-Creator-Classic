#if !defined(AFX_CTESTGAME_H__9F65E4E4_41C2_402B_B22A_5419A9CF064D__INCLUDED_)
#define AFX_CTESTGAME_H__9F65E4E4_41C2_402B_B22A_5419A9CF064D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "cMessages.h"


class cTestGame : public CDialog, public cBaseMessage
{
	public:
		cTestGame ( CWnd* pParent = NULL );

		void	HandleMessages ( void );

		//{{AFX_DATA(cTestGame)
		enum { IDD = IDD_TEST_GAME };
		//}}AFX_DATA

		//{{AFX_VIRTUAL(cTestGame)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		//}}AFX_VIRTUAL

	protected:
		//{{AFX_MSG(cTestGame)
		virtual BOOL OnInitDialog ( );
		afx_msg int  OnCreate     ( LPCREATESTRUCT lpCreateStruct );
		virtual void OnOK         ( );
	virtual void OnCancel();
	afx_msg void OnMove(int x, int y);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP ( )

		CEdit			m_Information;
		CEdit			m_Status;
		CProgressCtrl	m_Progress;
		CButton			m_Okay;
		CButton			m_Cancel;
		bool			m_bOkay;
		cMessages		m_Message;
		BYTE*			m_pMessageData;

		bool			m_bClean;

		cDebug			m_TestGameDebug;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTESTGAME_H__9F65E4E4_41C2_402B_B22A_5419A9CF064D__INCLUDED_)
