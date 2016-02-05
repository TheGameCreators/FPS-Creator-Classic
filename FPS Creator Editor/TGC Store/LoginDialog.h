#pragma once
#include "afxwin.h"
#include "..\resource.h"
#include "HyperlinkLabel.h"

// CLoginDialog dialog

class CMarketWindow;

class CLoginDialog : public CDialog
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDialog();

	void SetUsername( const char* name );
	void SetPassword( const char* name );
	const char* GetUsername( ) { return szUsername; }
	const char* GetPassword( ) { return szPassword; }

	void SetMarketWindow( CMarketWindow *window ) { m_pMarket = window; }

	virtual BOOL OnInitDialog( );

// Dialog Data
	enum { IDD = IDD_LOGINDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	char *szUsername;
	char *szPassword;
	CEdit m_edtUsername;
	CEdit m_edtPassword;
	
	CHyperlinkLabel m_txtRegister;
	CStatic m_txtUsername;
	CStatic m_txtPassword;
	CStatic m_txtStatement;
	CMarketWindow *m_pMarket;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnRegister( );
public:
	CStatic m_txtSaveDetails;
};
