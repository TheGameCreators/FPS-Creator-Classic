#pragma once


// CStarRating

class CStarRating : public CStatic
{
	DECLARE_DYNAMIC(CStarRating)

public:
	CStarRating();
	virtual ~CStarRating();

	//void SetRating( DWORD rating );
	void SetRating( DWORD totalrating, DWORD ratingcount );
	void SetBitmaps( CBitmap *pEmpty, CBitmap *pFilled, CBitmap *pMask );

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint( );

	DWORD m_dwRating;
	float m_fRating;
	bool m_bRated;

	CBitmap *m_pEmptyStar;
	CBitmap *m_pFilledStar;
	CBitmap *m_pMaskStar;
};


