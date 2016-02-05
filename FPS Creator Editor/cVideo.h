
#ifndef _CVIDEO_H_
#define _CVIDEO_H_

//#include "cItem.h"

#include <atlbase.h>
#include <dshow.h>
//#include <streams.h>
#include <d3d9.h>
#include <vmr9.h>
#include <objbase.h>
#include <d3dx9.h>

void dbSetCustomPresentMode			( bool bMode );
void dbAddCustomPresentRectangle	( RECT rect );
void dbClearCustomPresentList		( void );

class cVideo
{
	public:
		IGraphBuilder*			m_pGraphBuilder;
		IMediaControl*			m_pMediaControl;
		IMediaEventEx*			m_pMediaEvent;
		IBasicAudio*			m_pBasicAudio;
		IMediaSeeking*			m_pMediaSeeking;
		IMediaPosition*			m_pMediaPosition;
		IVMRWindowlessControl9*	m_pWindowlessControl;
		IFileSourceFilter*		m_pFileSource;
		IBaseFilter*			m_pVMR9;

		WCHAR					m_wFile [ MAX_PATH ];
		int						m_iX;
		int						m_iY;
		int						m_iWidth;
		int						m_iHeight;
		HWND					m_hWnd;
		bool					m_bFocus;
		bool					m_bSucceed;
		
		bool	SetupGraphBuilder		( void );
		bool	RenderFile				( void );
		bool	QueryInterfaces			( void );
		bool	SetVideoProperties		( void );
		bool	CheckStatus				( void );

		int		GetTotal				( int iMode );
		int		GetCurrent				( int iMode );

		HRESULT RenderOutputPins		( IGraphBuilder *pGB, IBaseFilter *pFilter );
		HRESULT GetUnconnectedPin		( IBaseFilter* pFilter, PIN_DIRECTION PinDir, IPin** ppPin );
		HRESULT RenderFileToVMR9		( IGraphBuilder* pGB, WCHAR* wFileName, IBaseFilter* pRenderer, BOOL bRenderAudio = TRUE );
		HRESULT InitializeWindowlessVMR ( IBaseFilter** ppVmr9 );

	public:
		 cVideo ( );
		 cVideo ( bool bMaximised, char* szName, const char* szFile, int iX, int iY, int iWidth, int iHeight, HWND hWnd );
		~cVideo ( );

		void		Update				( void );

		void		Play				( void );
		void		Stop				( void );
		void		SetPosition			( __int64 position );

		__int64		GetCurrentPosition	( void );
		__int64		GetStopPosition		( void );

		int			GetTotalHours		( void );
		int			GetTotalMinutes		( void );
		int			GetTotalSeconds		( void );
		
		int			GetCurrentHours		( void );
		int			GetCurrentMinutes	( void );
		int			GetCurrentSeconds	( void );

		void		AddDrawRegion		( int iA, int iB, int iC, int iD );

		void		SetDimensions		( int iWidth, int iHeight );
		void		Resize				( int iWidth, int iHeight );

	protected:

		bool Error( char *szMsg );
};

#endif _CVIDEO_H_