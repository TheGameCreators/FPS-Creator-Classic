#pragma once


// CDescriptionBox

class CDescriptionBox : public CStatic
{
	DECLARE_DYNAMIC(CDescriptionBox)

public:
	CDescriptionBox();
	virtual ~CDescriptionBox();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint( );
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
};


