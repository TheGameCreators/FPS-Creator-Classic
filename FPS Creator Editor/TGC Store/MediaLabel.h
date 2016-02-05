#pragma once


// CMediaLabel

class CMediaLabel : public CStatic
{
	DECLARE_DYNAMIC(CMediaLabel)

public:
	CMediaLabel();
	virtual ~CMediaLabel();

	void SetTextColor( COLORREF color );
	void SetText( CString text );

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );

	CString m_sText;

	CBrush m_Brush;
	COLORREF m_TextColor;
};


