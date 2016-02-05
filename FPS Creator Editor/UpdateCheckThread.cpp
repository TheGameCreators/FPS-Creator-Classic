#include "stdafx.h"
#include "UpdateCheckThread.h"
#include "AutoUpdate.h"
#include <afxinet.h>
#include "Editor.h"
#include "TGC Store\HTTPComm.h"

UpdateCheckThread::UpdateCheckThread( )
{
	m_iMode = -1;
	m_pStatusBox = NULL;
	m_pProgress = NULL;
	m_pOwner = NULL;
	m_bCancel = false;
}

UpdateCheckThread::~UpdateCheckThread( )
{
	
}

void UpdateCheckThread::SetDownloadFile( const TCHAR* szFilename, CProgressCtrl *pProgress, CEdit *pStatusBox, CAutoUpdate *pOwner )
{
	m_iMode = 1;
	_tcscpy_s( m_szFilename, 256, szFilename );
	m_pProgress = pProgress;
	m_pStatusBox = pStatusBox;
	m_pOwner = pOwner;
	m_bCancel = false;
}

void UpdateCheckThread::SetStartUpCheck( )
{
	m_iMode = 0;
	m_bCheckCompleted = false;
}

int UpdateCheckThread::GetMode( )
{
	return m_iMode;
}

void UpdateCheckThread::CancelDownload( )
{
	m_bCancel = true;
}

unsigned int UpdateCheckThread::Run( )
{
	if ( m_iMode == 0 )
	{
		_tcscpy_s( m_szLatestVersion, 64, _T("n/a") );

		HTTPComm server;
		server.GetLatestVersion( m_szLatestVersion, 64 );

		m_bCheckCompleted = true;
	}
	else if ( m_iMode == 1 )
	{
		FILE *pLocalFile = NULL;
		HINTERNET hInet, hInetConnect, hFtpFile;

		try
		{
			hInet = InternetOpen( _T("InternetConnection"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
			if ( !hInet )
			{
				m_pStatusBox->SetWindowText( GetLanguageData( _T("Auto Update"), _T("FailedInit") ) );
				throw 1;
			}

			hInetConnect = InternetConnect( hInet, _T("ftp.fpscreator.com"), INTERNET_DEFAULT_FTP_PORT, _T("anonymous"), NULL, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0 );
			if ( !hInetConnect )
			{
				m_pStatusBox->SetWindowText( GetLanguageData( _T("Auto Update"), _T("FailedConnect") ) );
				throw 2;
			}

			if ( !FtpSetCurrentDirectory( hInetConnect, _T("FPSCX9") ) )
			{
				m_pStatusBox->SetWindowText( GetLanguageData( _T("Auto Update"), _T("FailedDirectory") ) );
				throw 21;
			}

			hFtpFile = FtpOpenFile( hInetConnect, _T("FPSCX9Update.exe"), GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, 0 );
			if ( !hFtpFile )
			{
				TCHAR str [ 256 ];
				_stprintf_s( str, 256, _T("%s: %d\r\n\r\n"), GetLanguageData( _T("Auto Update"), _T("FailedGet") ), GetLastError( ) );

				TCHAR error [ 256 ];
				DWORD dwError, dwLen;
				InternetGetLastResponseInfo( &dwError, error, &dwLen );

				_tcscat_s( str, 256, error );
				m_pStatusBox->SetWindowText( str );
				throw 3;
			}

			DWORD dwSizeHigh = 0;
			DWORD dwSizeLow = FtpGetFileSize( hFtpFile, &dwSizeHigh );
			float fSize = (float) dwSizeLow;

			EnterCriticalSection( &theApp.m_csDirectoryChanges );

			SetCurrentDirectory( theApp.m_szDirectory );
			if ( !SetCurrentDirectory( _T("Updates") ) ) 
			{
				CreateDirectory( _T("Updates"), NULL );
				SetCurrentDirectory( _T("Updates") );
			}

			_tfopen_s( &pLocalFile, _T("FPSCX9Update.exe"), _T("wb") );
			
			LeaveCriticalSection( &theApp.m_csDirectoryChanges );
			
			if ( !pLocalFile )
			{
				//char str [ 256 ];
				//_stprintf_s( str, 256, _T("Could not create local file"), GetLastError( ) );
				m_pStatusBox->SetWindowText( GetLanguageData( _T("Auto Update"), _T("LocalFile") ) );
				throw 3;
			}

			bool bFinished = false;

			TCHAR pBuffer [ 20480 ];
			DWORD dwReceived = 0;
			DWORD dwTotalReceived = 0;
			TCHAR *szBytes = _T("");
			TCHAR *szKiloBytes = _T("KB");
			TCHAR *szMegaBytes = _T("MB");
			TCHAR *szUnits = szBytes;
			int iUnitMode = 0;

			if ( fSize > 2097152 )
			{
				szUnits = szMegaBytes;
				iUnitMode = 2;
				fSize /= 1048576;
			}
			else if ( fSize > 10240 )
			{
				szUnits = szKiloBytes;
				iUnitMode = 1;
				fSize /= 1024;
			}
			

			while ( !bFinished && !m_bCancel )
			{
				BOOL bResult = InternetReadFile( hFtpFile, pBuffer, 20480, &dwReceived );
				if ( !bResult )
				{
					TCHAR str [ 256 ];
					_stprintf_s( str, 256, _T("%s: %d\r\n\r\n"), GetLanguageData( _T("Auto Update"), _T("FailedDownload") ), GetLastError( ) );

					TCHAR error [ 256 ];
					DWORD dwError, dwLen;
					InternetGetLastResponseInfo( &dwError, error, &dwLen );

					_tcscat_s( str, 256, error );
					m_pStatusBox->SetWindowText( str );
					throw 4;
				}

				if ( dwReceived == 0 ) bFinished = true;
				dwTotalReceived += dwReceived;

				if ( dwReceived > 0 )
				{
					fwrite( pBuffer, 1, dwReceived, pLocalFile );
					m_pProgress->SetPos( (dwTotalReceived*100) / dwSizeLow );
					TCHAR str [ 256 ];
					float fTotalReceived = (float)dwTotalReceived;
					if ( iUnitMode == 1 ) fTotalReceived /= 1024;
					else if ( iUnitMode == 2 ) fTotalReceived /= 1048576;
					_stprintf_s( str, 256, _T("%s: %.1f/%.1f%s"), GetLanguageData( _T("Auto Update"), _T("Downloading") ), fTotalReceived, fSize, szUnits );
					m_pStatusBox->SetWindowText( str );
				}
			}

			if ( m_bCancel ) throw 5;

			if ( pLocalFile ) fclose( pLocalFile );
			pLocalFile = NULL;

			InternetCloseHandle( hFtpFile );
			InternetCloseHandle( hInetConnect );
			InternetCloseHandle( hInet );

			m_pStatusBox->SetWindowText( GetLanguageData( _T("Auto Update"), _T("Complete") ) );

			m_iMode = -1;
			m_pOwner->DownloadComplete( );
		}
		catch(...)
		{
			if ( pLocalFile ) fclose( pLocalFile );

			if ( hFtpFile ) InternetCloseHandle( hFtpFile );
			if ( hInetConnect ) InternetCloseHandle( hInetConnect );
			if ( hInet ) InternetCloseHandle( hInet );
		}
	}
	
	return 0;
}