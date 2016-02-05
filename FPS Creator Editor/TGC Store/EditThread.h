#ifndef H_EDIT_THREAD
#define H_EDIT_THREAD

#include "stdafx.h"
#include "HTTPComm.h"
#include "Thread.h"

class EditThread : public Thread
{

public:

	EditThread( );
	~EditThread( );

	//void SetValues( CString name, CString cost, CString type, CString category, CString subcategory, CString polygons, CString keywords, CString description, DWORD groupid, int x9compatible, int x10compatible );
	void SetFilenames( CProgressCtrl *pProgress, DWORD dwObjID, CString sName, CString model, CString thumbnail, int numImages, CString *pImages, CString musicsample );

	bool IsUploading( ) { return m_bUploading; }
	bool IsSuccessful( ) { return m_bUploadSuccessful; }

	void Cancel() { m_Server.CancelTransfer( ); }

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

	DWORD m_dwObjID;
	CString m_sName;

	/*
	CString m_sCost;
	CString m_sType;
	CString m_sCategory;
	CString m_sSubCategory;
	CString m_sAuthor;
	CString m_sPolygons;
	CString m_sKeywords;
	CString m_sDescription;

	DWORD m_dwGroupID;

	int m_iX9Compatible;
	int m_iX10Compatible;
	*/
};

#endif