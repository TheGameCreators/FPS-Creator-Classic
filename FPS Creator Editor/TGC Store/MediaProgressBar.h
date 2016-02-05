#pragma once


// CMediaProgressBar

class CMediaProgressBar : public CProgressCtrl
{
	DECLARE_DYNAMIC(CMediaProgressBar)

public:
	CMediaProgressBar();
	virtual ~CMediaProgressBar();

	//int SetPos( int nPos );
	
	void SetPlayPos( long pos  );
	void SetPlayRange( long max );

protected:

	long m_iDownloadPos;
	long m_iPlayPos;
	long m_iMaxPlayPos;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg LRESULT SetPosMsg( WPARAM wParam, LPARAM lParam );
};


