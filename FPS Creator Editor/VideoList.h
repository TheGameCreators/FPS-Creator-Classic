#pragma once

// CVideoList

class CVideoList : public CListCtrl
{
	DECLARE_DYNAMIC(CVideoList)

public:
	CVideoList();
	virtual ~CVideoList();

	void OnClickList ( NMHDR* pNMHDR, LRESULT* pResult );
	void Init( );
	void LoadVideoFiles( );
	bool GetFilenameSelected( int iBufferSize, TCHAR *szFilename );
	

protected:

	CImageList m_Images;
	int m_iItemClicked;

	DECLARE_MESSAGE_MAP()
};


