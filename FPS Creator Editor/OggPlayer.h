#pragma once

#include    <windows.h>                 
#include    <mmreg.h>

#include    <dsound.h>                  // from the directx 8 sdk

#include    <vorbis/codec.h>            // from the vorbis sdk
#include    <vorbis/vorbisfile.h>       

class OggPlayer
{
protected:

    bool        bInitialized;           // initialized?
    bool        bFileOpened;            // have we opened an ogg yet?

    bool        bReleaseDS;             // release ds by ourselves?

    LPDIRECTSOUND8 pDS;                 // the directsound 8 object
    LPDIRECTSOUNDBUFFER pDSB;           // the buffer

    OggVorbis_File vf;                     // for the vorbisfile interface

    DWORD		dwLength;

    bool        bDone;                  // done playing
    bool        bLoop;                  

public:

    OggPlayer();
    ~OggPlayer();

    bool InitDirectSound( HWND hWnd );
 
    inline void UseDirectSound( LPDIRECTSOUND8 _pDS )
    {
        pDS = _pDS;
    }

    bool OpenOgg( const char *filename );
    void Close();
    void Play(bool loop = false);
    void Stop();
    void Update();               
	bool IsPlaying( );
	DWORD GetPosition();
	DWORD GetLength();
};