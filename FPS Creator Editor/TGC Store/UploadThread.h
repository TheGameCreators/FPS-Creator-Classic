#ifndef H_UPLOAD_THREAD
#define H_UPLOAD_THREAD

#include "stdafx.h"
#include "HTTPComm.h"
#include "Thread.h"

class CProgressCtrl;

class UploadThread : public Thread
{

public:

	UploadThread( );
	~UploadThread( );

	void SetValues( CProgressCtrl *progress, CString name, CString cost, CString type, CString category, CString subcategory, CString polygons, CString keywords, CString description, CString video, DWORD groupid, int x9compatible, int x10compatible );
	void SetFilenames( CString model, CString thumbnail, int numImages, CString *pImages, CString musicsample );

	bool IsUploading( ) { return m_bUploading; }
	bool IsSuccessful( ) { return m_bUploadSuccessful; }
	DWORD GetObjID( ) { return m_dwObjID; }

	void Cancel();

protected:

	virtual unsigned Run( );

private:

	CProgressCtrl *m_pProgressBar;

	HTTPComm m_Server;
	bool m_bUploading;
	bool m_bUploadSuccessful;
	
	CString m_sModelFilename;
	CString m_sThumbnail;
	int m_iNumImages;
	CString *m_psPreviewImages;
	CString m_sMusicSample;

	CString m_sName;
	CString m_sCost;
	CString m_sType;
	CString m_sCategory;
	CString m_sSubCategory;
	CString m_sAuthor;
	CString m_sPolygons;
	CString m_sKeywords;
	CString m_sDescription;
	CString m_sVideo;

	DWORD m_dwObjID;
	DWORD m_dwGroupID;

	int m_iX9Compatible;
	int m_iX10Compatible;
};

#endif