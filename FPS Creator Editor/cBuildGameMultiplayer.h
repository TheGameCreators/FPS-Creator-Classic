#if !defined(AFX_CBUILDGAMEMULTIPLAYER_H__FB4D2095_647E_4EBE_B8BB_5E2C972F110C__INCLUDED_)
#define AFX_CBUILDGAMEMULTIPLAYER_H__FB4D2095_647E_4EBE_B8BB_5E2C972F110C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cBuildGameMultiplayer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cBuildGameMultiplayer dialog

#include "cControls.h"

class cBuildGameMultiplayer : public CTabPageSSL
{
// Construction
public:
	cBuildGameMultiplayer(CWnd* pParent = NULL);   // standard constructor

	void Update     ( cIPC* pIPC );
	void SetControl ( cControls* pControl );

// Dialog Data
	//{{AFX_DATA(cBuildGameMultiplayer)
	enum { IDD = IDD_BUILD_GAME_MULTIPLAYER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cBuildGameMultiplayer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	cControls*		m_pControls;

	CButton			m_Buttons [ 6 ];
	CEdit			m_Lines   [ 8 ];
	CEdit 			m_Edit    [ 3 ];
	bool			bArenaTabDisabled;

	// Generated message map functions
	//{{AFX_MSG(cBuildGameMultiplayer)
		// NOTE: the ClassWizard will add member functions here
		afx_msg void OnRadioObjective();
		afx_msg void OnRadioFirstTo();
		afx_msg void OnRadioHighestFrag();
		afx_msg void OnRadioOneShot();
		afx_msg void OnRadioDefault();
		afx_msg void OnRadioRandom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUILDGAMEMULTIPLAYER_H__FB4D2095_647E_4EBE_B8BB_5E2C972F110C__INCLUDED_)
