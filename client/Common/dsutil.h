






#ifndef DSUTIL_H
#define DSUTIL_H

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>







class CSoundManager;
class CSound;
class CStreamingSound;
class CWaveFile;







#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2

#define DSUtil_StopSound(s)         { if(s) s->Stop(); }
#define DSUtil_PlaySound(s)         { if(s) s->Play( 0, 0 ); }
#define DSUtil_PlaySoundLooping(s)  { if(s) s->Play( 0, DSBPLAY_LOOPING ); }








class CSoundManager
{
public:
    LPDIRECTSOUND8 m_pDS;

public:
    CSoundManager();
    ~CSoundManager();

    HRESULT Initialize( HWND hWnd, DWORD dwCoopLevel, DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate );
    inline  LPDIRECTSOUND8 GetDirectSound() { return m_pDS; }
    HRESULT SetPrimaryBufferFormat( DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate );
    HRESULT Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener );

    HRESULT Create( CSound* pSound, LPTSTR strWaveFileName, DWORD dwCreationFlags = 0, GUID guid3DAlgorithm = GUID_NULL );
};








class CSound
{
public:
	LPDIRECTSOUNDBUFFER    m_apDSBuffer;
	DWORD                  m_dwDSBufferSize;
	LPDIRECTSOUND3DBUFFER  m_pDS3DBuffer;
    CWaveFile             *m_pWaveFile;

protected:

    HRESULT RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored );

public:
	CSound();
    virtual ~CSound();

	void Release();
	HRESULT Create( DWORD dwDSBufferSize, CWaveFile* pWaveFile );
    HRESULT Get3DBufferInterface( LPDIRECTSOUND3DBUFFER* ppDS3DBuffer );
    HRESULT FillBufferWithSound( LPDIRECTSOUNDBUFFER pDSB, int bRepeatWavIfBufferLarger , DWORD sizeBuffer );
	LPDIRECTSOUND3DBUFFER Get3DBuffer() { return m_pDS3DBuffer; }
    
    HRESULT Play( LPDIRECTSOUNDBUFFER pDSB, DWORD sizeBuffer, DWORD dwPriority = 0, DWORD dwFlags = 0 );
    HRESULT Stop( LPDIRECTSOUNDBUFFER pDSB );
    HRESULT Reset( LPDIRECTSOUNDBUFFER pDSB );
    int     IsSoundPlaying( LPDIRECTSOUNDBUFFER pDSB );
};









class CWaveFile
{
public:
    WAVEFORMATEX  m_pwfx;        
    HMMIO         m_hmmio;       
    MMCKINFO      m_ck;          
    MMCKINFO      m_ckRiff;      
    DWORD         m_dwSize;      
    MMIOINFO      m_mmioinfoOut;
    DWORD         m_dwFlags;
    int           m_bIsReadingFromMemory;
    BYTE*         m_pbData;
    BYTE*         m_pbDataCur;
    ULONG         m_ulDataSize;
    CHAR*         m_pResourceBuffer;

protected:
    HRESULT ReadMMIO();
    HRESULT WriteMMIO( WAVEFORMATEX *pwfxDest );

public:
    CWaveFile();
    ~CWaveFile();

    HRESULT Open( LPTSTR strFileName, DWORD dwFlags );
    HRESULT OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags );
    HRESULT Close();

    HRESULT Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );
    HRESULT Write( UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote );

    DWORD   GetSize();
    HRESULT ResetFile();
    WAVEFORMATEX* GetFormat() { return &m_pwfx; };
};




#endif 
