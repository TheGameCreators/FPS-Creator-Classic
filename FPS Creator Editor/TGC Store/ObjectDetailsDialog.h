#pragma once
//#include "afxwin.h"
#include "PreviewRect.h"
#include "..\resource.h"
#include "MarketObject.h"
#include "afxwin.h"
#include "StarRating.h"
#include "DescriptionBox.h"
#include "cMarketImage.h"
#include "MarketWindow.h"
#include "PictureEx.h"
#include "HyperlinkLabel.h"
#include "..\OggPlayer.h"
#include "ServerDownloader.h"
#include "MediaProgressBar.h"
#include "MediaLabel.h"

// CObjectDetailsDialog dialog

class CObjectDetailsDialog : public CDialog
{
	DECLARE_DYNAMIC(CObjectDetailsDialog)

public:
	CObjectDetailsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CObjectDetailsDialog();

	bool IsBought( ) { return m_bBuy; }
	bool IsAddedToCart( ) { return m_bAddToCart; }
	bool IsCartChanged() { return m_bCartStateChanged; }
	int GetItemSelected() { return m_iItemSelected; }

// Dialog Data
	enum { IDD = IDD_OBJECTDETAILSDIALOG };

	void SetValues( MarketObject *pObject, cMarketImage *pMarket, CMarketWindow *pWindow, int iGroup, bool bFromHomePage = false );
	void SetBitmaps( CBitmap *pEmptyStar, CBitmap *pFilledStar, CBitmap *pMaskStar );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	void PopulateValues( );

	DECLARE_MESSAGE_MAP()

	MarketObject *m_pCurrObject;
	cMarketImage *m_pMarket;
	CMarketWindow *m_pWindow;
	int m_iGroup;
	int m_iItemSelected;
	bool m_bFromHomePage;

	bool m_bBuy;
	bool m_bAddToCart;
	bool m_bCartStateChanged;

	CString m_sMusicSampleFile;
	ServerDownloader m_ServerThread;

	/*
	CString m_name;
	CString m_category;
	CString m_cost;
	CString m_description;
	CString m_author;
	CString m_rating;
	char **m_pszImages;
	DWORD m_dwNumImages;
	*/
	DWORD m_dwCurrImage;

	CBitmap bmpCurrImage;
	CBitmap *m_pEmptyStar;
	CBitmap *m_pFilledStar;
	CBitmap *m_pMaskStar;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedImageLeft();
	afx_msg void OnBnClickedImageRight();
	afx_msg void OnPrevObjectClicked();
	afx_msg void OnNextObjectClicked();
	afx_msg void OnYourRatingClicked();
	afx_msg void OnAddToCart();
	afx_msg void OnTimer( UINT_PTR nIDEvent );

	CFont m_fontMain;

	CStatic m_txtImageCounter;
	CPreviewRect m_ImageBox;
	CPictureEx m_GifBox;
	CPreviewRect m_Thumbnail;

	OggPlayer op;

	CStatic m_txtName;
	CStatic m_txtType;
	CStatic m_txtCost;
	CStarRating m_txtRating;
	CStatic m_txtCategory;
	CStatic m_txtAuthor;
	CStatic m_txtPolygons;
	CStatic m_txtFileSize;	
	CStatic m_txtDownloads;
	CEdit m_editDescription;
	
	CStatic m_txtDownload;

	CButton m_btnBuy;
	CButton m_btnClose;
	CButton m_btnPrevObject;
	CButton m_btnNextObject;

	CStatic m_labelName;
	CStatic m_labelType;
	CStatic m_labelCost;
	CStatic m_LabelRating;
	CStatic m_LabelCategory;
	CStatic m_LabelAuthor;
	CStatic m_labelPolygons;
	CStatic m_LabelDescription;
	CStatic m_labelFileSize;
	CStatic m_labelDownloads;

	CButton m_btnRate;
	CComboBox m_comboRating;
	CStatic m_txtYourRate;

	CButton m_btnAddToCart;
	CHyperlinkLabel m_linkVideo;

	CMediaProgressBar m_MediaProgressBar;
	CButton m_btnPlaySample;

	CButton m_btnNextImage;
	CButton m_btnPrevImage;
public:
	CMediaLabel m_labelMedia;
public:
	afx_msg void OnPlaySample();
};
