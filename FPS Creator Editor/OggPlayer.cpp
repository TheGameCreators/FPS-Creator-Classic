#include "stdafx.h"

#include "oggplayer.h"

OggPlayer::OggPlayer()
{
    bFileOpened     = false;
    bInitialized    = false;
    bReleaseDS      = false;
    pDS             = NULL;
    pDSB            = NULL;
    bLoop           = false;
    bDone           = false;
	dwLength		= 0;
}

OggPlayer::~OggPlayer()
{
    if (bFileOpened)
        Close();

    if (bReleaseDS && pDS)
        pDS->Release();
}

bool OggPlayer::InitDirectSound( HWND hWnd )
{
	HRESULT hr;
	
	if (FAILED(hr = DirectSoundCreate8(NULL, &pDS, NULL)))
        return bInitialized = false;

	pDS->Initialize(NULL);
	pDS->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );

    bReleaseDS = true;

    return bInitialized = true;
}

bool OggPlayer::OpenOgg( const char *filename )
{
    if (!bInitialized)
        return false;

    if (bFileOpened)
        Close();

    FILE    *f;

    f = fopen(filename, "rb");
    if (!f) return false;

    //ov_open(f, &vf, NULL, 0);
	int result = ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_DEFAULT);
	if ( result != 0 ) 
	{
		ov_clear( &vf );
		fclose( f );
		return false;
	}

    // ok now the tricky part

    // the vorbis_info struct keeps the most of the interesting format info
    vorbis_info *vi = ov_info(&vf,-1);

    // set the wave format
	WAVEFORMATEX	    wfm;

    memset(&wfm, 0, sizeof(wfm));

    wfm.cbSize          = sizeof(wfm);
    wfm.nChannels       = vi->channels;
    wfm.wBitsPerSample  = 16;                    // ogg vorbis is always 16 bit
    wfm.nSamplesPerSec  = vi->rate;
    wfm.nAvgBytesPerSec = wfm.nSamplesPerSec*wfm.nChannels*2;
    wfm.nBlockAlign     = 2*wfm.nChannels;
    wfm.wFormatTag      = 1;


	dwLength = (DWORD) ( vf.pcmlengths[1] * 2 * vi->channels );


    // set up the buffer
	DSBUFFERDESC desc;

	desc.dwSize         = sizeof(desc);
	desc.dwFlags        = DSBCAPS_GLOBALFOCUS;
	desc.lpwfxFormat    = &wfm;
	desc.dwReserved     = 0;
    desc.dwBufferBytes  = dwLength;//BUFSIZE*2;

    pDS->CreateSoundBuffer(&desc, &pDSB, NULL );

    // fill the buffer

    DWORD   pos = 0;
    int     sec = 0;
    int     ret = 1;
	DWORD   size = dwLength;//BUFSIZE*2;

    char    *buf;

    pDSB->Lock(0, dwLength, (LPVOID*)&buf, &size, NULL, NULL, 0);
    
    // now read in the bits
    while(ret && pos<size)
    {
        ret = ov_read(&vf, buf+pos, size-pos, 0, 2, 1, &sec);
        pos += ret;
    }

	pDSB->Unlock( buf, size, NULL, NULL );

	bFileOpened = true;
    return true;
}

void OggPlayer::Close()
{
	if ( bFileOpened )
	{
		ov_clear( &vf );
	}

    bFileOpened = false;

    if (pDSB) pDSB->Release();
	pDSB = NULL;
}


void OggPlayer::Play(bool loop)
{
    if ( !bInitialized ) return;
    if ( !bFileOpened ) return;
	if ( !pDSB ) return;

    // play looping because we will fill the buffer
    pDSB->Play(0,0,0/*DSBPLAY_LOOPING*/);    

    bLoop = loop;
    bDone = false;
}

void OggPlayer::Stop()
{
    if (!bInitialized)
        return;

    if (!bFileOpened)
        return;

    if ( pDSB ) pDSB->Stop();
}

bool OggPlayer::IsPlaying( )
{
	if ( !pDSB ) return false;

	DWORD dwStatus;
	pDSB->GetStatus( &dwStatus );

	if ( dwStatus & DSBSTATUS_PLAYING ) return true;
	else return false;
}

DWORD OggPlayer::GetPosition()
{
	DWORD pos = 0;
    if ( pDSB ) pDSB->GetCurrentPosition(&pos, NULL);
	return pos;
}

DWORD OggPlayer::GetLength()
{
	return dwLength;
}

void OggPlayer::Update()
{
	if ( !pDSB ) return;

    DWORD pos;
	pDSB->GetCurrentPosition(&pos, NULL);

    if ( 0 )
    {
        if (bDone && !bLoop)
            Stop();

        DWORD   size = dwLength;
        char    *buf;

        // fill the section we just left
        pDSB->Lock( 0, size, (LPVOID*)&buf, &size, NULL, NULL, 0 );

        DWORD   pos = 0;
        int     sec = 0;
        int     ret = 1;
                
        while(ret && pos<size)
        {
            ret = ov_read(&vf, buf+pos, size-pos, 0, 2, 1, &sec);
            pos += ret;
        }

        // reached the and?
        if (!ret && !(bLoop))
        {
            // not looping so fill the rest with 0
            while(pos<size)
			{
                *(buf+pos)=0; pos ++;
			}
        }
                
        pDSB->Unlock( buf, size, NULL, NULL );
    }
}