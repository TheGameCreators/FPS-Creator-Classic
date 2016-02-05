#pragma once


// CBlackBox

class CBlackBox : public CStatic
{
	DECLARE_DYNAMIC(CBlackBox)

public:
	CBlackBox();
	virtual ~CBlackBox();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};


