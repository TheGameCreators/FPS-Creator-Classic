#pragma once


// CBlackBorder

class CBlackBorder : public CStatic
{
	DECLARE_DYNAMIC(CBlackBorder)

public:
	CBlackBorder();
	virtual ~CBlackBorder();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};


