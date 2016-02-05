#pragma once


// CStoreButton

class CStoreButton : public CButton
{
	DECLARE_DYNAMIC(CStoreButton)

public:
	CStoreButton();
	virtual ~CStoreButton();

protected:

	bool m_bLButtonDown;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
};


