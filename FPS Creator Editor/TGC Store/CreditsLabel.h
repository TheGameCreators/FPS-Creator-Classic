#pragma once


// CCreditsLabel

class CCreditsLabel : public CStatic
{
	DECLARE_DYNAMIC(CCreditsLabel)

public:
	CCreditsLabel();
	virtual ~CCreditsLabel();

	void SetTextColor( COLORREF color );

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );

	CBrush m_Brush;
	COLORREF m_TextColor;
};


