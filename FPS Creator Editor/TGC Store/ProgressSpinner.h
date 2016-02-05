#pragma once


// CProgressSpinner

class CProgressSpinner : public CStatic
{
	DECLARE_DYNAMIC(CProgressSpinner)

public:
	CProgressSpinner();
	virtual ~CProgressSpinner();

	void StartSpinning( );
	void StopSpinning( );

protected:

	bool m_bSpinning;
	int m_iAngle;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnTimer( UINT_PTR nIDEvent );
};


