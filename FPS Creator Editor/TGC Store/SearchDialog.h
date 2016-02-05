#pragma once
#include "afxwin.h"
#include "../resource.h"


// CSearchDialog dialog

class CSearchDialog : public CDialog
{
	DECLARE_DYNAMIC(CSearchDialog)

public:
	CSearchDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchDialog();

	BOOL OnInitDialog( );

// Dialog Data
	enum { IDD = IDD_SEARCHDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CEdit m_editKeyword;
	CStatic m_txtKeyword;

	CButton m_btnSearch;
	CButton m_btnCancel;
	CButton m_chkExact;
	CButton m_chkApprovalItems;

public:

	CString m_sKeyword;
	bool m_bExactMatch;
	bool m_bApprovalItems;
	bool m_bAdmin;

public:

	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedApprovalItems();
};
