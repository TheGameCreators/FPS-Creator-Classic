#if !defined(AFX_CBUILDGAMEWEAPONS_H__029DD29B_AE46_4952_80B1_0CDFE680B708__INCLUDED_)
#define AFX_CBUILDGAMEWEAPONS_H__029DD29B_AE46_4952_80B1_0CDFE680B708__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cBuildGameWeapons.h : header file
//

#include "cControls.h"

/////////////////////////////////////////////////////////////////////////////
// cBuildGameWeapons dialog

class cBuildGameWeapons : public CDialog
{
// Construction
public:
	cBuildGameWeapons(CWnd* pParent = NULL);   // standard constructor

	cControls*		m_pControls;

	void SetControl ( cControls* pControl );

	CComboBoxEx	m_WeaponType;

	CString m_Selection;

	bool	m_bOk;

// Dialog Data
	//{{AFX_DATA(cBuildGameWeapons)
	enum { IDD = IDD_BUILD_GAME_WEAPONS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cBuildGameWeapons)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cBuildGameWeapons)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnWeaponOk();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		CEdit m_1;
	CEdit m_2;
	CButton m_OK;
	CButton m_Cancel;

	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUILDGAMEWEAPONS_H__029DD29B_AE46_4952_80B1_0CDFE680B708__INCLUDED_)
