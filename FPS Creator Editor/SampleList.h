#pragma once

// CSampleList

class CSampleList : public CListCtrl
{
	DECLARE_DYNAMIC(CSampleList)

public:
	CSampleList();
	virtual ~CSampleList();

	void OnClickList ( NMHDR* pNMHDR, LRESULT* pResult );
	void Init( );
	void LoadSampleFiles( );
	bool GetFilenameSelected( int iBufferSize, TCHAR *szFilename );

protected:

	CImageList m_Images;
	int m_iItemClicked;

	DECLARE_MESSAGE_MAP()
};


