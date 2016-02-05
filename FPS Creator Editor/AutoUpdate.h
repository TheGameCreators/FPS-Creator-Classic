#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "TGC Store\PreviewRect.h"

class UpdateCheckThread;

// CAutoUpdate dialog

class CAutoUpdate : public CDialog
{
	DECLARE_DYNAMIC(CAutoUpdate)

public:
	CAutoUpdate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoUpdate();

// Dialog Data
	enum { IDD = IDD_AUTOUPDATE };

	BOOL OnInitDialog( );
	void DownloadComplete( );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CBCGURLLinkButton	m_btnURL1;

	CEdit m_TxtCurrentVersion;
	CEdit m_TxtLatestVersion;
	CButton m_BtnDownloadUpdate;
	CProgressCtrl m_ProgressBar;
	CButton m_chkCheckAtStartup;
	CEdit m_MsgText;
	CStatic m_UpdateText;
	CPreviewRect m_UpdateImage;
	CButton m_BtnCheckForUpdates;
	CButton m_btnClose;
	CStatic m_staCurrentVersion;
	CStatic m_StaLatestVersion;

	UpdateCheckThread *m_pUpdateThread;
	UpdateCheckThread *m_pCheckThread;
	bool m_bUpdateAvailable;
	bool m_bDownloadComplete;

public:
	afx_msg void OnCheckForUpdates();
	afx_msg void OnDownloadUpdate();	
	afx_msg void OnCheckAtStartupClicked();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnStnClickedAupdateText();
};
