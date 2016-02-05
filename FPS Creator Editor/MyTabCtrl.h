#pragma once


// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

	void Init( );
	void SetRectangle( );
	void OnClosing( );
	void ChangeTab( int iNewTab );

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnSize( UINT nType, int cx, int cy );

	int m_iNumberOfPages;
	int m_tabCurrent;
	CDialog *m_tabPages[3];
};


