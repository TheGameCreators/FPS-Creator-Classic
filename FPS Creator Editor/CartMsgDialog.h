#pragma once
#include "afxwin.h"
#include "resource.h"


// CartMsgDialog dialog

class CartMsgDialog : public CDialog
{
	DECLARE_DYNAMIC(CartMsgDialog)

public:
	CartMsgDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CartMsgDialog();

	void SetValues( const char* szMsg, const char* szTitle, const char *szRegID );

	BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_ARTMSGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CButton m_chkNotShowAgain;
	CStatic m_txtMessage;

	CString m_sMessage;
	CString m_sTitle;
	CString m_sRegID;

	afx_msg void OnDontShowAgainClicked();
	afx_msg void OnBnClickedOk();
};
