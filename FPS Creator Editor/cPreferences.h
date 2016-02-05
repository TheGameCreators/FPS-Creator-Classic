#if !defined(AFX_CPREFERENCES_H__E6143A84_2CDF_4F9E_8451_10E702A3F70A__INCLUDED_)
#define AFX_CPREFERENCES_H__E6143A84_2CDF_4F9E_8451_10E702A3F70A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cPreferences.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cPreferences dialog

#include "cMessages.h"
#include "cControls.h"

#ifdef TGC_STORE
#include "afxwin.h"
#endif

class cPreferences : public CDialog, cBaseMessage
{
// Construction
public:
	cPreferences(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cPreferences)
	enum { IDD = IDD_PREFERENCES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	cMessages			m_Message;
	//BYTE*				m_pData;
	cIPC*				m_pIPC;
	cControls			m_Controls;

	bool				m_bOK;
	bool				m_bCancel;

	bool				m_bStart;
	DWORD				m_dwHackKeepPref;

	void HandleMessages ( void );

	CButton				m_DynamicLighting;
	CButton				m_DynamicShadows;
	CButton				m_NoLightMapping;
	CButton				m_QuickLightMapping;
	CButton				m_FullLightMapping;
	CButton				m_NoEffects;
	CButton				m_EssentialEffects;
	CButton				m_FullEffects;
	CButton				m_Low;
	CButton				m_Medium;
	CButton				m_High;
	CButton				m_HSR;
	CButton				m_NoFloor;
	CButton				m_ShowEntity;
	CButton				m_ShowDebug;

	// mike - 060505 - we need 2 buttons for the new multiplayer options
	CButton				m_SinglePlayer;
	CButton				m_MultiPlayer;
	

	// Generated message map functions
	//{{AFX_MSG(cPreferences)
	virtual BOOL OnInitDialog();
	afx_msg void OnDynamicLighting();
	afx_msg void OnDynamicShadows();
	afx_msg void OnNoLightMapping();
	afx_msg void OnShowDebug();
	afx_msg void OnFullLightMapping();
	afx_msg void OnNoEffects();
	afx_msg void OnEssentialEffects();
	afx_msg void OnFullEffects();
	afx_msg void OnLow();
	afx_msg void OnMedium();
	afx_msg void OnHigh();
	afx_msg void OnHsr();
	afx_msg void OnShowEntity();
	afx_msg void OnQuickLightMapping();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnNoFloor();
	afx_msg void OnSinglePlayer();
	afx_msg void OnMultiplayer();

#ifdef TGC_STORE
	afx_msg void OnSaveUsernameClicked();
	afx_msg void OnSavePasswordClicked();
	afx_msg void OnShowPurchasedClicked();

	CButton m_chkSaveUsername;
	CButton m_chkSavePassword;
	CButton m_chkShowPurchased;
	CEdit m_edtUsername;
	CEdit m_edtPassword;
	CStatic m_txtUsername;
	CStatic m_txtPassword;
#endif

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CEdit			m_Lines [ 12 ];
	CButton			m_Buttons [ 2 ];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPREFERENCES_H__E6143A84_2CDF_4F9E_8451_10E702A3F70A__INCLUDED_)
