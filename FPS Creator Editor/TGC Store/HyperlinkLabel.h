#pragma once


// CCreditsLabel

class CHyperlinkLabel : public CStatic
{
	DECLARE_DYNAMIC(CHyperlinkLabel)

public:
	CHyperlinkLabel();
	virtual ~CHyperlinkLabel();

	void SetType( int type ) { iType = type; }
	void SetLink( const char *szLink ) { sLink = szLink; }

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );

	bool m_bIsHovering;
	bool m_bIsClicking;
	int iType;
	CString sLink;
};


