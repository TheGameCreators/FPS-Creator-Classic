#pragma once
#include "afxwin.h"
#include "resource.h"
#include "TGC Store\PreviewRect.h"
#include "TGC Store\ProgressSpinner.h"
#include "TGC Store\EditThread.h"
#include "afxcmn.h"

class MarketObject;


// CEditDialog dialog

class CEditDialog : public CDialog
{
	DECLARE_DYNAMIC(CEditDialog)

public:
	CEditDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditDialog();

	void SetObject( MarketObject *pObject ) { m_pObject = pObject; }

	BOOL OnInitDialog( );

// Dialog Data
	enum { IDD = IDD_EDITDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();

	void LoadThumbnail( );

protected:

	MarketObject *m_pObject;
	EditThread m_EditThread;

	CString m_sName;
	CString m_sCost;
	CString m_sPolygons;
	CString m_sKeywords;
	CString m_sDescription;
	CString m_sVideo;
	CString m_sModelFile;
	CString m_sThumbnail;
	int m_iNumImages;
	CString* m_psPreviewImages;
	bool m_bPreviewImagesChanged;
	CString m_sMusicSample;
	
	CEdit m_editName;
	CButton m_chkX9;
	CButton m_chkX10;
	CEdit m_editCost;
	CEdit m_editPolygons;
	CEdit m_editDescription;
	CEdit m_editVideoLink;
	CButton m_chkResetRating;
	CButton m_chkResetDownloadCount;

	CStatic m_labelVideoLink;
	CStatic m_labelModelDesc;
	CStatic m_labelNewModel;
	CStatic m_labelNewThumbnail;
	CStatic m_labelNewImages;
	CStatic m_labelThumbnail;
	CStatic m_labelImages;
	CStatic m_labelName;
	CStatic m_labelCompatible;
	CStatic m_labelCost;
	CStatic m_labelPolygons;
	CStatic m_labelKeywords;
	CStatic m_labelDescription;

	CStatic m_BoxStoreGroup;
	CEdit m_editModelFile;
	CComboBox m_comboKeywords;
	CEdit m_editThumbnail;
	CComboBox m_comboPreviewImages;
	CPreviewRect m_imgPreview;
	CPreviewRect m_imgThumbnail;

	afx_msg void OnNewModelFile();
	afx_msg void OnNewThumbnail();
	afx_msg void OnAddPreviewImage();
	afx_msg void OnRemovePreviewImage();
	afx_msg void OnAddKeyword();
	afx_msg void OnRemoveKeyword();
	afx_msg void OnChangedPreviewSelection();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnBnClickedCancel();

	CProgressSpinner m_ProgressSpinner;
	CProgressCtrl m_ProgressBar;
	CStatic m_textUploading;

	bool CheckStringAlNum( CString sField, CString sString );
	bool CheckStringDigit( CString sField, CString sString );
	bool CheckStringAlNum2( CString sField, CString sString );
	bool CEditDialog::CheckStringDescription( CString sField, CString sString );

	CButton m_btnAddImage;
	CButton m_btnRemoveImage;
	CButton m_btnCancel;
	CButton m_btnApply;

	CStatic m_labelNewMusicSample;
	CEdit m_editNewMusicSample;
	CButton m_btnBrowseMusic;

	afx_msg void OnBrowseMusic();
};
