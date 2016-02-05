
#include "stdafx.h"
#include "cVideo.h"
#include "objbase.h"
#include <windows.h>
#include <winuser.h>
//#include "globstruct.h"
//#include "cMouse.h"
#include <Vfw.h>
#include "Editor.h"
#include "HelpWizard.h"

//extern GlobStruct* g_pGlob;
#define SAFE_RELEASE( p ) { if ( p ) { ( p )->Release ( ); ( p ) = NULL; } }

cVideo::cVideo ( )
{

}

cVideo::cVideo ( bool bMaximised, char* szName, const char* szFile, int iX, int iY, int iWidth, int iHeight, HWND hWnd )
{
	m_bSucceed				= false;

	m_pGraphBuilder			= NULL;
	m_pMediaControl			= NULL;
	m_pMediaEvent			= NULL;
	m_pBasicAudio			= NULL;
	m_pMediaSeeking			= NULL;
	m_pMediaPosition		= NULL;
	m_pWindowlessControl	= NULL;
	m_pFileSource			= NULL;
	m_pVMR9					= NULL;

	m_iX					= iX;
	m_iY					= iY;
	m_iWidth				= iWidth;
	m_iHeight				= iHeight;
	m_hWnd					= hWnd;
	m_bFocus				= true;
	
//	strcpy_s ( m_szName, szName );

	
	MultiByteToWideChar ( CP_ACP, 0, ( const char* ) szFile, -1, m_wFile, strlen ( ( const char* ) szFile ) + 1 );

	if ( !SetupGraphBuilder ( ) )
		Error ( "SetupGraphBuilder failed" );

	if ( !QueryInterfaces ( ) )
		Error ( "QueryInterfaces failed" );

	if ( !RenderFile ( ) )
		Error ( "RenderFile failed" );

	if ( !SetVideoProperties ( ) )
		Error ( "SetVideoProperties failed" );

	//dbSetCustomPresentMode ( true );

	m_bSucceed = true;
}

cVideo::~cVideo ( )
{
	if ( m_pMediaControl )
		m_pMediaControl->Stop ( );

	if ( m_pMediaEvent )
        m_pMediaEvent->SetNotifyWindow ( ( OAHWND ) NULL, 0, 0 );

	SAFE_RELEASE ( m_pMediaEvent    );
    SAFE_RELEASE ( m_pMediaSeeking  );
    SAFE_RELEASE ( m_pMediaPosition );
    SAFE_RELEASE ( m_pMediaControl  );
    SAFE_RELEASE ( m_pBasicAudio    );
    SAFE_RELEASE ( m_pWindowlessControl  );
	SAFE_RELEASE ( m_pFileSource );
	SAFE_RELEASE ( m_pVMR9 );
    SAFE_RELEASE ( m_pGraphBuilder  );

	CoUninitialize ( );

	//dbSetCustomPresentMode ( false );
	//dbClearCustomPresentList ( );
}

bool cVideo::Error( char *szMsg )
{
	//theApp.m_pHelpWizardDlg->ShowWindow( SW_MINIMIZE );
	//MessageBox( NULL, szMsg, "Video Error", 0 );
	return false;
}

void cVideo::AddDrawRegion ( int iA, int iB, int iC, int iD )
{
//	if ( m_bVista == false )
	{
		RECT rect = { iA, iB, iC, iD };
		//dbAddCustomPresentRectangle ( rect );
	}
}

bool cVideo::SetupGraphBuilder ( void )
{
	if ( FAILED ( CoInitialize ( NULL ) ) )
		return false;
	
    if ( FAILED ( CoCreateInstance ( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, ( void** ) &m_pGraphBuilder ) ) )
		return false;
	
	return true;
}

bool cVideo::RenderFile ( void )
{
	if ( m_pMediaControl )
		m_pMediaControl->Run ( );

	return true;
}

bool cVideo::QueryInterfaces ( void )
{
	HRESULT hr;

	InitializeWindowlessVMR ( &m_pVMR9 );

	if ( FAILED ( hr = RenderFileToVMR9 ( m_pGraphBuilder, m_wFile, m_pVMR9, TRUE ) ) )
	{
		Error ( "unable to render using vmr9" );
		return false;
	}

    if ( FAILED ( m_pGraphBuilder->QueryInterface ( IID_IMediaControl, ( void** ) &m_pMediaControl ) ) )
		return Error ( "Failed to access media control interface" );

    if ( FAILED ( m_pGraphBuilder->QueryInterface ( IID_IMediaEventEx, ( void** ) &m_pMediaEvent ) ) )
		return Error ( "Failed to access media event interface" );

    if ( FAILED ( m_pGraphBuilder->QueryInterface ( IID_IMediaSeeking, ( void** ) &m_pMediaSeeking ) ) )
		return Error ( "Failed to access media seeking interface" );

    if ( FAILED ( m_pGraphBuilder->QueryInterface ( IID_IMediaPosition, ( void** ) &m_pMediaPosition ) ) )
		return Error ( "Failed to access media position interface" );

    if ( FAILED ( m_pGraphBuilder->QueryInterface ( IID_IBasicAudio, ( void** ) &m_pBasicAudio ) ) )
		return Error ( "Failed to access basic audio interface" );

	return true;
}

bool cVideo::SetVideoProperties ( void )
{
	long lWidth  = 0;
	long lHeight = 0;

	if ( m_pWindowlessControl )
		m_pWindowlessControl->GetNativeVideoSize ( &lWidth, &lHeight, NULL, NULL ); 

	RECT rcSrc;
	SetRect(&rcSrc, 0, 0, lWidth, lHeight); 

	RECT clientrc = { 0,0,0,0 };

//	if ( g_pGlob )
//		GetClientRect ( g_pGlob->hWnd, &clientrc );

	//float iScreenWidth = 1, iScreenHeight = 1;

	float fX = 1;//( float ) clientrc.right  / ( float ) iScreenWidth;
	float fY = 1;//( float ) clientrc.bottom / ( float ) iScreenHeight;

	RECT src  = { m_iX, m_iY, m_iX + m_iWidth, m_iY + m_iHeight };
	RECT dest = { ( int ) ( src.left * fX ), ( int ) ( src.top * fY ), ( int ) ( src.right * fX ), ( int ) ( src.bottom * fY ) };

	if ( m_pWindowlessControl ) m_pWindowlessControl->SetVideoPosition ( &rcSrc, &dest ); 

	return true;
}

void cVideo::Resize ( int iWidth, int iHeight )
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	RECT dest  = { m_iX, m_iY, m_iX + m_iWidth, m_iY + m_iHeight };

	if ( m_pWindowlessControl ) 
	{
		if ( FAILED( m_pWindowlessControl->SetVideoPosition ( NULL, &dest ) ) )
		{
			//MessageBox( NULL, "Resize Failed", "Video Error", 0 );
		}
	}
}

void cVideo::SetDimensions ( int iWidth, int iHeight )
{
	m_iWidth  = iWidth;
	m_iHeight = iHeight;
}

void cVideo::Update ( void )
{
	if ( m_bFocus )
	{
		m_bFocus = false;
	}

//	if ( dbMouseClick ( ) )
	{
//		m_bFocus = true;
	}
}

__int64 cVideo::GetCurrentPosition ( void )
{
	__int64 currentPosition = 0;
	
	if ( m_pMediaSeeking )
		m_pMediaSeeking->GetCurrentPosition ( &currentPosition );

	return currentPosition;
}

__int64 cVideo::GetStopPosition ( void )
{
	__int64 stopPosition = 0;

	if ( m_pMediaSeeking )
		m_pMediaSeeking->GetStopPosition ( &stopPosition );

	return stopPosition;
}

void cVideo::Play ( void )
{
	if ( m_pMediaControl )
	{
		if ( FAILED ( m_pMediaControl->Run ( ) ) )
		{
			Error ( "Failed to play the media" );
		}
	}
}

void cVideo::Stop ( void )
{
	if ( m_pMediaControl )
	{
		m_pMediaControl->Pause ( );
	}
}

void cVideo::SetPosition ( __int64 position )
{
	if ( m_pMediaSeeking )
	{
//		dbText ( 460, 560, "Please wait seeking..." );
//		dbSync ( );

		m_pMediaSeeking->SetPositions ( &position, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning );
	}
}

bool cVideo::CheckStatus ( void )
{

	LONG evCode, evParam1, evParam2;
	HRESULT hr=S_OK;

	if ( m_pMediaEvent )
	{
		// Process all queued events
		while(SUCCEEDED(m_pMediaEvent->GetEvent(&evCode, (LONG_PTR *) &evParam1, (LONG_PTR *) &evParam2, 0)))
		{
			// Free memory associated with callback, since we're not using it
			hr = m_pMediaEvent->FreeEventParams(evCode, evParam1, evParam2);

			// If this is the end of the clip, reset to beginning
			if(EC_COMPLETE == evCode)
			{
				return true;
			}
		}
	}

	return false;
}

int cVideo::GetTotal ( int iMode )
{
	if ( m_pMediaSeeking )
	{
		HRESULT hr;
		LONGLONG Duration;
		double FullDuration;
		GUID Format = TIME_FORMAT_MEDIA_TIME;
		m_pMediaSeeking->SetTimeFormat(&Format);
		hr = m_pMediaSeeking->GetDuration(&Duration);
		//FullDuration = double(Duration) / UNITS;
		FullDuration = double(Duration) / 10000;

		int hrs, mins, secs;

		FullDuration += 0.49;

		hrs  =  (int)FullDuration / 3600;
		mins = ((int)FullDuration % 3600) / 60;
		secs = ((int)FullDuration % 3600) % 60;

		if ( iMode == 0 ) return hrs;
		if ( iMode == 1 ) return mins;
		if ( iMode == 2 ) return secs;
	}

    return 0;
}

int cVideo::GetCurrent ( int iMode )
{
	if ( m_pMediaSeeking )
	{
		HRESULT hr;
		REFTIME rt = (REFTIME)0;
		LONGLONG Position;
		double FullPosition;
		hr = m_pMediaSeeking->GetPositions(&Position, NULL);
		//FullPosition = double(Position) / UNITS;
		FullPosition = double(Position) / 10000;

		int hrs, mins, secs;

		FullPosition += 0.49;

		hrs  =  (int)FullPosition / 3600;
		mins = ((int)FullPosition % 3600) / 60;
		secs = ((int)FullPosition % 3600) % 60;

		if ( iMode == 0 ) return hrs;
		if ( iMode == 1 ) return mins;
		if ( iMode == 2 ) return secs;
	}

    return 0;
}

int cVideo::GetTotalHours ( void )
{
	return GetTotal ( 0 );
}

int cVideo::GetTotalMinutes ( void )
{
	return GetTotal ( 1 );
}

int cVideo::GetTotalSeconds ( void )
{
	return GetTotal ( 2 );
}

int cVideo::GetCurrentHours ( void )
{
	return GetCurrent ( 0 );
}

int cVideo::GetCurrentMinutes ( void )
{
	return GetCurrent ( 1 );
}

int cVideo::GetCurrentSeconds ( void )
{
	return GetCurrent ( 2 );
}

HRESULT cVideo::InitializeWindowlessVMR ( IBaseFilter** ppVmr9 )
{
    IBaseFilter* pVmr = NULL;

    if (!ppVmr9)
        return E_POINTER;
    *ppVmr9 = NULL;

    // Create the VMR and add it to the filter graph.
    HRESULT hr = CoCreateInstance ( CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&pVmr );
    
	if (SUCCEEDED(hr)) 
    {
		
        hr = m_pGraphBuilder->AddFilter(pVmr, L"Video Mixing Renderer 9");
    
		
		if (SUCCEEDED(hr)) 
        {
            // Set the rendering mode and number of streams
            IVMRFilterConfig9* pConfig;

            pVmr->QueryInterface(IID_IVMRFilterConfig9, (void**)&pConfig);
            pConfig->SetRenderingMode(VMR9Mode_Windowless);

			pConfig->Release ( );

			//pConfig->SetRenderingMode(VMR9Mode_Windowless);
			//pConfig->SetRenderingPrefs(RenderPrefs_AllowOffscreen);

            hr = pVmr->QueryInterface(IID_IVMRWindowlessControl9, (void**)&m_pWindowlessControl);
            if( SUCCEEDED(hr)) 
            {
                //(m_pWindowlessControl->SetVideoClippingWindow(g_pGlob->hWnd));
				m_pWindowlessControl->SetVideoClippingWindow( m_hWnd );
				m_pWindowlessControl->SetAspectRatioMode( VMR_ARMODE_LETTER_BOX );
                (m_pWindowlessControl->SetBorderColor(RGB(0,0,0)));
            }
		}
		

        // Don't release the pVmr interface because we are copying it into
        // the caller's ppVmr9 pointer
        *ppVmr9 = pVmr;
    }

    return hr;
}

HRESULT cVideo::RenderFileToVMR9 ( IGraphBuilder* pGB, WCHAR* wFileName, IBaseFilter* pRenderer, BOOL bRenderAudio )
{
    HRESULT hr=S_OK;
    IPin* pOutputPin;
    IBaseFilter* pSource;
    IBaseFilter* pAudioRenderer;
    IFilterGraph2* pFG;

	hr = CoCreateInstance ( CLSID_WMAsfReader, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, ( void** ) &pSource );

	hr = pGB->AddFilter ( pSource, L"ASF Reader" );

	pSource->QueryInterface ( IID_IFileSourceFilter, ( void** ) &m_pFileSource );

	hr = m_pFileSource->Load ( wFileName, NULL );

	if ( FAILED ( hr ) )
	{
	
	}

	( GetUnconnectedPin ( pSource, PINDIR_OUTPUT, &pOutputPin ) );

	RenderOutputPins(pGB, pSource);

    // Render audio if requested (defaults to TRUE)
    if (bRenderAudio)
    {
        // Because we will be rendering with the RENDERTOEXISTINGRENDERERS flag,
        // we need to create an audio renderer and add it to the graph.  
        // Create an instance of the DirectSound renderer (for each media file).
        //
        // Note that if the system has no sound card (or if the card is disabled),
        // then creating the DirectShow renderer will fail.  In that case,
        // handle the failure quietly.
        if (SUCCEEDED(CoCreateInstance(CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pAudioRenderer)))
        {
            // The audio renderer was successfully created, so add it to the graph
            (pGB->AddFilter(pAudioRenderer, L"Audio Renderer"));
        }
    }

    // Get an IFilterGraph2 interface to assist in building the
    // multifile graph with the non-default VMR9 renderer
    (pGB->QueryInterface(IID_IFilterGraph2, (void **)&pFG));

    // Render the output pin, using the VMR9 as the specified renderer.  This is 
    // necessary in case the GraphBuilder needs to insert a Color Space convertor,
    // or if multiple filters insist on using multiple allocators.
    // The audio renderer will also be used, if the media file contains audio.
    (pFG->RenderEx(pOutputPin, AM_RENDEREX_RENDERTOEXISTINGRENDERERS, NULL));

    // If this media file does not contain an audio stream, then the 
    // audio renderer that we created will be unconnected.  If left in the 
    // graph, it could interfere with rate changes and timing.
    // Therefore, if the audio renderer is unconnected, remove it from the graph.
    if (pAudioRenderer != NULL)
    {
        IPin *pUnconnectedPin=0;

        // Is the audio renderer's input pin connected?
        HRESULT hrPin = GetUnconnectedPin(pAudioRenderer, PINDIR_INPUT, &pUnconnectedPin);

        // If there is an unconnected pin, then remove the unused filter
        if (SUCCEEDED(hrPin) && (pUnconnectedPin != NULL))
        {
            // Release the returned IPin interface
            pUnconnectedPin->Release();

            // Remove the audio renderer from the graph
            hrPin = pGB->RemoveFilter(pAudioRenderer);
        }
    }

	
	
	SAFE_RELEASE ( pAudioRenderer );
	SAFE_RELEASE ( pFG );
	SAFE_RELEASE ( pOutputPin );
	SAFE_RELEASE ( pSource );
	
    return hr;
}


HRESULT cVideo::RenderOutputPins ( IGraphBuilder *pGB, IBaseFilter *pFilter )
{
    HRESULT         hr = S_OK;
    IEnumPins*		pEnumPin = NULL;
    IPin*			pConnectedPin = NULL;
	IPin*			pPin = NULL;
    PIN_DIRECTION   PinDirection;
    ULONG           ulFetched;

    // enumerate all pins on the filter
    hr = pFilter->EnumPins ( &pEnumPin );

    if ( SUCCEEDED ( hr ) )
    {
        // step through every pin, looking for the output pins
        while ( S_OK == ( hr = pEnumPin->Next ( 1L, &pPin, &ulFetched ) ) )
        {
            // is this pin connected?  We're not interested in connected pins
            hr = pPin->ConnectedTo ( &pConnectedPin );

            if ( pConnectedPin )
            {
                pConnectedPin->Release ( );
                pConnectedPin = NULL;
            }

            // if this pin is not connected, render it.
            if ( VFW_E_NOT_CONNECTED == hr )
            {
                hr = pPin->QueryDirection ( &PinDirection );

                if ( ( S_OK == hr ) && ( PinDirection == PINDIR_OUTPUT ) )
                {
                    hr = pGB->Render ( pPin );
                }
            }

            pPin->Release ( );

            // If there was an error, stop enumerating
            if ( FAILED ( hr ) )
                break;
        }
    }

    // Release pin enumerator
    pEnumPin->Release ( );
    return hr;
}


HRESULT cVideo::GetUnconnectedPin ( IBaseFilter* pFilter, PIN_DIRECTION PinDir, IPin** ppPin )
{
    IEnumPins *pEnum = 0;
    IPin *pPin = 0;

    if (!ppPin)
        return E_POINTER;
    *ppPin = 0;

    // Get a pin enumerator
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
        return hr;

    // Look for the first unconnected pin
    while (pEnum->Next(1, &pPin, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;

        pPin->QueryDirection(&ThisPinDir);
        if (ThisPinDir == PinDir)
        {
            IPin *pTmp = 0;

            hr = pPin->ConnectedTo(&pTmp);
            if (SUCCEEDED(hr))  // Already connected, not the pin we want.
            {
                pTmp->Release();
            }
            else  // Unconnected, this is the pin we want.
            {
                pEnum->Release();
                *ppPin = pPin;
                return S_OK;
            }
        }
        pPin->Release();
    }

    // Release the enumerator
    pEnum->Release();

    // Did not find a matching pin
    return E_FAIL;
}
