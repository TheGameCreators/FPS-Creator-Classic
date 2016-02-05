// UploadAppDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ProgressSpinner.h"
#include "UploadThread.h"
#include "Thread.h"
#include "PreviewRect.h"
//#include "cLibraryTree.h"

class CMarketWindow;

// CUploadAppDlg dialog
class CUploadAppDlg : public CDialog, public Thread
{
// Construction
public:
	CUploadAppDlg(CWnd* pParent = NULL);	// standard constructor
	~CUploadAppDlg();

// Dialog Data
	enum { IDD = IDD_UPLOADAPP_DIALOG };

	static void ClearUploadFolder( const char *szFolderName );
	static bool VerifyModelFolders( int iLevel, int iFolderType );
	static bool CheckModelFileFormat( const char *szModelFile, int iType, CString &sNewName );
	static bool SearchSegmentDirectory( const char *szFolder, CString &sNewName );
	static bool FindSegmentThumbnail( const char *szFolder, const char *szThumbnail, CString &sFolderPath );
	static void StrKeepSimilar( char *szStr1, const char *szStr2 );

	void SetProgress( int iProgress );

	bool IsRunning( ) { return m_bRunning; }
	bool IsUploading() { return m_UploadThread.IsUploading(); }
	void SetMarketWindow( CMarketWindow *window ) { m_pMarket = window; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	unsigned Run( );
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnActivate( UINT nState, CWnd *pWndOther, BOOL bMinimized );

	//void OnSelchangedTree ( NMHDR* pNMHDR, LRESULT* pResult );
	//void OnItemexpandingTree ( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

protected:
	UploadThread m_UploadThread;
	
	CString m_sName;
	CString m_sCost;
	CString m_sType;
	CString m_sCategory;
	CString m_sAuthor;
	CString m_sPolygons;
	CString m_sKeywords;
	CString m_sDescription;
	CString m_sVideoLink;
	CString m_sSubCategory;

	CString m_sModelFile;
	CString m_sThumbnail;
	int m_iNumImages;
	CString* m_psPreviewImages;

	bool m_bRunning;
	CMarketWindow *m_pMarket;

protected:
	CEdit m_editModelFile;
	CComboBox m_comboPreviewImages;
	CEdit m_editName;
	CEdit m_editCost;
	CComboBox m_comboType;
	CEdit m_editPolygons;
	CComboBox m_comboKeywords;
	CEdit m_editDescription;
	CEdit m_editSubCategory;
	CEdit m_editVideoLink;
	CEdit m_editMusicSample;

	CButton m_chkAgreement;
	CButton m_chkX9Compatible;
	CButton m_chkX10Compatible;
	CComboBox m_comboCategory;
	CButton m_btnCostHelp;
	CButton m_btnUpload;
	CButton m_btnCancel;
	CButton m_btnClearFields;
	CButton m_btnAddImage;
	CButton m_btnRemoveImage;
	CButton m_btnBrowseMusic;
	CButton m_btnHelpMusic;

	CStatic m_textUploading;
	CProgressSpinner m_ProgressSpinner;

	CButton m_btnCalculate;
	CProgressCtrl m_ProgressBar;
	CPreviewRect m_ImageThumbnail;
	CButton m_btnCopyFromObject;

	DWORD m_dwGroupSelected;

	CStatic m_labelModelDesc;
	CStatic m_labelPreviewDesc;
	CStatic m_labelModel;
	CStatic m_labelPreview;
	CStatic m_labelThumbnail;
	CStatic m_labelName;
	CStatic m_labelCompatible;
	CStatic m_labelType;
	CStatic m_labelCategory;
	CStatic m_labelSubCategory;
	CStatic m_labelCost;
	CStatic m_labelPolygons;
	CStatic m_labelKeywords;
	CStatic m_labelDescription;
	CStatic m_labelVideoLink;
	CStatic m_labelMusicSample;

public:
	afx_msg void OnBrowseModelFile();
	afx_msg void OnBrowseThumbnail();
	afx_msg void OnAddPreviewImage();
	afx_msg void OnRemovePreviewImage();
	afx_msg void OnAddKeyword();
	afx_msg void OnRemoveKeyword();
	//afx_msg void OnInsertGroup();
	afx_msg void OnUpload();
	afx_msg void OnTimer( UINT_PTR nIDEvent );

	afx_msg void OnRememberAuthorClicked();
	afx_msg void OnModelHelpClicked();
	afx_msg void OnTypeChanged();
	afx_msg void OnCostHelpClicked();
	afx_msg void OnBnClickedCancel();

private:

	bool CheckStringAlNum( CString sField, CString sString );
	bool CheckStringDigit( CString sField, CString sString );
	bool CheckStringAlNum2( CString sField, CString sString );
	bool CheckStringDescription( CString sField, CString sString );
	
	afx_msg void OnCalculatePolygons();
	afx_msg void OnClearFields();
	afx_msg void OnBnClickedCopyObject();
	afx_msg void OnBrowseMusicSample();
	afx_msg void OnMusicSampleHelp();
};
