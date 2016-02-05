#ifndef _H_UPDATE_CHECK_THREAD
#define _H_UPDATE_CHECK_THREAD

#include "Thread.h"

class CAutoUpdate;

class UpdateCheckThread : public Thread
{

public:

	UpdateCheckThread( );
	~UpdateCheckThread( );

	void SetDownloadFile( const TCHAR* szFilename, CProgressCtrl *pProgress, CEdit *pStatusBox, CAutoUpdate *pOwner );
	void SetStartUpCheck( );

	bool CheckFinished( ) { return m_bCheckCompleted; }
	const TCHAR* GetLatestVersion( ) { return m_szLatestVersion; }

	int GetMode( );
	void CancelDownload( );

protected:

	unsigned int Run( );

private:

	int m_iMode;
	CProgressCtrl *m_pProgress;
	CEdit *m_pStatusBox;
	CAutoUpdate *m_pOwner;
	TCHAR m_szFilename [ 256 ];
	bool m_bCancel;
	bool m_bCheckCompleted;
	TCHAR m_szLatestVersion [ 64 ];
	
};

#endif