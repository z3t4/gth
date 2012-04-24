






#define STRICT
#include <dmusicc.h>
#include <dmusici.h>
#include <dsound.h>
#include <dxerr8.h>
#include "DMUtil.h"
#include "DXUtil.h"








CMusicManager::CMusicManager()
{
    m_pLoader       = NULL;
    m_pPerformance  = NULL;
    
    
    HRESULT hr = CoInitialize(NULL);
    m_bCleanupCOM = SUCCEEDED(hr);
}








CMusicManager::~CMusicManager()
{
    SAFE_RELEASE( m_pLoader ); 

    if( m_pPerformance )
    {
        
        m_pPerformance->Stop( NULL, NULL, 0, 0 );
        m_pPerformance->CloseDown();

        SAFE_RELEASE( m_pPerformance );
    }

    if( m_bCleanupCOM )
        CoUninitialize();
}








HRESULT CMusicManager::Initialize( HWND hWnd, DWORD dwPChannels, DWORD dwDefaultPathType )
{
    HRESULT hr;

    
    if( FAILED( hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
                                       IID_IDirectMusicLoader8, (void**)&m_pLoader ) ) )
        return DXTRACE_ERR( TEXT("CoCreateInstance"), hr );

    
    if( FAILED( hr = CoCreateInstance( CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
                                       IID_IDirectMusicPerformance8, (void**)&m_pPerformance ) ) )
        return DXTRACE_ERR( TEXT("CoCreateInstance"), hr );

    
    
    
    
    if( FAILED( hr = m_pPerformance->InitAudio( NULL, NULL, hWnd, dwDefaultPathType, 
                                                dwPChannels, DMUS_AUDIOF_ALL, NULL ) ) )
    {
        if( hr == DSERR_NODRIVER )
        {
            DXTRACE( "Warning: No sound card found\n" );
            return hr;
        }

        return DXTRACE_ERR( TEXT("InitAudio"), hr );
    }

    return S_OK;
}









HRESULT CMusicManager::SetSearchDirectory( const TCHAR* strMediaPath )
{
    if( NULL == m_pLoader )
        return E_UNEXPECTED;

    
    WCHAR wstrMediaPath[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wstrMediaPath, strMediaPath );

    return m_pLoader->SetSearchDirectory( GUID_DirectMusicAllTypes, 
                                          wstrMediaPath, FALSE );
}








IDirectMusicAudioPath8* CMusicManager::GetDefaultAudioPath()
{
    IDirectMusicAudioPath8* pAudioPath = NULL;
    if( NULL == m_pPerformance )
        return NULL;

    m_pPerformance->GetDefaultAudioPath( &pAudioPath );
    return pAudioPath;
}









VOID CMusicManager::CollectGarbage()
{
    if( m_pLoader )
        m_pLoader->CollectGarbage();
}








HRESULT CMusicManager::CreateSegmentFromFile( CMusicSegment** ppSegment, 
                                              TCHAR* strFileName, 
                                              BOOL bDownloadNow,
                                              BOOL bIsMidiFile )
{
    HRESULT               hr;
    IDirectMusicSegment8* pSegment = NULL;

    
    WCHAR wstrFileName[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wstrFileName, strFileName );

    if ( FAILED( hr = m_pLoader->LoadObjectFromFile( CLSID_DirectMusicSegment,
                                                     IID_IDirectMusicSegment8,
                                                     wstrFileName,
                                                     (LPVOID*) &pSegment ) ) )
    {
        if( hr == DMUS_E_LOADER_FAILEDOPEN )
            return hr;
        return DXTRACE_ERR( TEXT("LoadObjectFromFile"), hr );
    }

    *ppSegment = new CMusicSegment( m_pPerformance, m_pLoader, pSegment );
    if (!*ppSegment)
        return E_OUTOFMEMORY;

    if( bIsMidiFile )
    {
        if( FAILED( hr = pSegment->SetParam( GUID_StandardMIDIFile, 
                                             0xFFFFFFFF, 0, 0, NULL ) ) )
            return DXTRACE_ERR( TEXT("SetParam"), hr );
    }

    if( bDownloadNow )
    {
        if( FAILED( hr = (*ppSegment)->Download() ) )
            return DXTRACE_ERR( TEXT("Download"), hr );
    }

    return S_OK;
}








HRESULT CMusicManager::CreateSegmentFromResource( CMusicSegment** ppSegment, 
                                                  TCHAR* strResource,
                                                  TCHAR* strResourceType,
                                                  BOOL bDownloadNow,
                                                  BOOL bIsMidiFile )
{
    HRESULT               hr;
    IDirectMusicSegment8* pSegment      = NULL;
    HRSRC                 hres          = NULL;
    void*                 pMem          = NULL;
    DWORD                 dwSize        = 0;
    DMUS_OBJECTDESC       objdesc;

    
    hres = FindResource( NULL,strResource,strResourceType );
    if( NULL == hres ) 
        return E_FAIL;

    
    pMem = (void*)LoadResource( NULL, hres );
    if( NULL == pMem ) 
        return E_FAIL;

    
    dwSize = SizeofResource( NULL, hres ); 
    
    
    ZeroMemory(&objdesc,sizeof(DMUS_OBJECTDESC));
    objdesc.dwSize = sizeof(DMUS_OBJECTDESC);
    objdesc.dwValidData = DMUS_OBJ_MEMORY | DMUS_OBJ_CLASS;
    objdesc.guidClass = CLSID_DirectMusicSegment;
    objdesc.llMemLength =(LONGLONG)dwSize;
    objdesc.pbMemData = (BYTE*)pMem;
    
    if (FAILED ( hr = m_pLoader->GetObject( &objdesc,
                                            IID_IDirectMusicSegment8,
                                            (void**)&pSegment ) ) )
    {
        if( hr == DMUS_E_LOADER_FAILEDOPEN )
            return hr;
        return DXTRACE_ERR( TEXT("LoadObjectFromFile"), hr );
    }

    *ppSegment = new CMusicSegment( m_pPerformance, m_pLoader, pSegment );
    if( NULL == *ppSegment )
        return E_OUTOFMEMORY;

    if( bIsMidiFile )
    {
        
        
        
        if( FAILED( hr = pSegment->SetParam( GUID_StandardMIDIFile, 
                                             0xFFFFFFFF, 0, 0, NULL ) ) )
            return DXTRACE_ERR( TEXT("SetParam"), hr );
    }

    if( bDownloadNow )
    {
        
        
        
        
        
        if( FAILED( hr = (*ppSegment)->Download() ) )
            return DXTRACE_ERR( TEXT("Download"), hr );
    }

    return S_OK;
}








HRESULT CMusicManager::CreateScriptFromFile( CMusicScript** ppScript, 
                                             TCHAR* strFileName )
{
    HRESULT               hr;
    IDirectMusicScript* pScript = NULL;

    
    WCHAR wstrFileName[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wstrFileName, strFileName );
    
    if ( FAILED( hr = m_pLoader->LoadObjectFromFile( CLSID_DirectMusicScript,
                                                     IID_IDirectMusicScript8,
                                                     wstrFileName,
                                                     (LPVOID*) &pScript ) ) )
        return DXTRACE_ERR_NOMSGBOX( TEXT("LoadObjectFromFile"), hr );

    if ( FAILED( hr = pScript->Init( m_pPerformance, NULL ) ) )
        return DXTRACE_ERR( TEXT("Init"), hr );

    *ppScript = new CMusicScript( m_pPerformance, m_pLoader, pScript );
    if (!*ppScript)
        return E_OUTOFMEMORY;

    return hr;
}








HRESULT CMusicManager::CreateChordMapFromFile( IDirectMusicChordMap8** ppChordMap, 
                                               TCHAR* strFileName )
{
    
    WCHAR wstrFileName[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wstrFileName, strFileName );

    return m_pLoader->LoadObjectFromFile( CLSID_DirectMusicChordMap,
                                          IID_IDirectMusicChordMap8,
                                          wstrFileName, (LPVOID*) ppChordMap );
}








HRESULT CMusicManager::CreateStyleFromFile( IDirectMusicStyle8** ppStyle, 
                                            TCHAR* strFileName )
{
    
    WCHAR wstrFileName[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wstrFileName, strFileName );

    return m_pLoader->LoadObjectFromFile( CLSID_DirectMusicStyle,
                                          IID_IDirectMusicStyle8,
                                          wstrFileName, (LPVOID*) ppStyle );
}








HRESULT CMusicManager::GetMotifFromStyle( IDirectMusicSegment8** ppMotif8, 
                                          TCHAR* strStyle, TCHAR* strMotif )
{       
    HRESULT              hr;
    IDirectMusicStyle8*  pStyle = NULL;
    IDirectMusicSegment* pMotif = NULL;

    if( FAILED( hr = CreateStyleFromFile( &pStyle, strStyle ) ) )
        return DXTRACE_ERR( TEXT("CreateStyleFromFile"), hr );

    if( pStyle )
    {
        
        WCHAR wstrMotif[MAX_PATH];
        DXUtil_ConvertGenericStringToWide( wstrMotif, strMotif );

        hr = pStyle->GetMotif( wstrMotif, &pMotif );
        SAFE_RELEASE( pStyle );

        if( FAILED( hr ) )
            return DXTRACE_ERR( TEXT("GetMotif"), hr );

        pMotif->QueryInterface( IID_IDirectMusicSegment8, (LPVOID*) ppMotif8 );
    }

    return S_OK;
}








CMusicSegment::CMusicSegment( IDirectMusicPerformance8* pPerformance, 
                              IDirectMusicLoader8*      pLoader,
                              IDirectMusicSegment8*     pSegment )
{
    m_pPerformance          = pPerformance;
    m_pLoader               = pLoader;
    m_pSegment              = pSegment;
    m_pEmbeddedAudioPath    = NULL;
    m_bDownloaded           = FALSE;
    
    
    
    
    IUnknown* pConfig = NULL;
    if( SUCCEEDED( m_pSegment->GetAudioPathConfig( &pConfig ) ) )
    {
        m_pPerformance->CreateAudioPath( pConfig, TRUE, &m_pEmbeddedAudioPath );
        SAFE_RELEASE( pConfig );
    } 

}








CMusicSegment::~CMusicSegment()
{
    if( m_pSegment )
    {
        
        if( m_pLoader )
            m_pLoader->ReleaseObjectByUnknown( m_pSegment );

        if( m_bDownloaded )
        {
            if( m_pEmbeddedAudioPath )
                m_pSegment->Unload( m_pEmbeddedAudioPath );
            else
                m_pSegment->Unload( m_pPerformance );
        }

        SAFE_RELEASE( m_pEmbeddedAudioPath ); 
        SAFE_RELEASE( m_pSegment ); 
    }

    m_pPerformance = NULL;
}









HRESULT CMusicSegment::Play( DWORD dwFlags, IDirectMusicAudioPath8* pAudioPath )
{
    if( m_pSegment == NULL || m_pPerformance == NULL )
        return CO_E_NOTINITIALIZED;

    if( !m_bDownloaded )
        return E_FAIL;

    
    
    if( pAudioPath == NULL && m_pEmbeddedAudioPath != NULL )
        pAudioPath = m_pEmbeddedAudioPath;
        
    
    return m_pPerformance->PlaySegmentEx( m_pSegment, 0, NULL, dwFlags, 
                                          0, 0, NULL, pAudioPath );
}









HRESULT CMusicSegment::Download( IDirectMusicAudioPath8* pAudioPath )
{
    HRESULT hr;
    
    if( m_pSegment == NULL )
        return CO_E_NOTINITIALIZED;

    
    
    
    if( pAudioPath == NULL )
    {
        if( m_pEmbeddedAudioPath )
            hr = m_pSegment->Download( m_pEmbeddedAudioPath );
        else    
            hr = m_pSegment->Download( m_pPerformance );
    }
    else
    {
        hr = m_pSegment->Download( pAudioPath );
    }
    
    if ( SUCCEEDED( hr ) )
        m_bDownloaded = TRUE;
        
    return hr;
}








HRESULT CMusicSegment::Unload( IDirectMusicAudioPath8* pAudioPath )
{
    HRESULT hr;
    
    if( m_pSegment == NULL )
        return CO_E_NOTINITIALIZED;

    
    
    
    if( pAudioPath == NULL )
    {
        if( m_pEmbeddedAudioPath )
            hr = m_pSegment->Unload( m_pEmbeddedAudioPath );
        else    
            hr = m_pSegment->Unload( m_pPerformance );
    }
    else
    {
        hr = m_pSegment->Unload( pAudioPath );
    }
        
    if ( SUCCEEDED( hr ) )
        m_bDownloaded = FALSE;

    return hr;
}








BOOL CMusicSegment::IsPlaying()
{
    if( m_pSegment == NULL || m_pPerformance == NULL )
        return CO_E_NOTINITIALIZED;

    return ( m_pPerformance->IsPlaying( m_pSegment, NULL ) == S_OK );
}








HRESULT CMusicSegment::Stop( DWORD dwFlags )
{
    if( m_pSegment == NULL || m_pPerformance == NULL )
        return CO_E_NOTINITIALIZED;

    return m_pPerformance->Stop( m_pSegment, NULL, 0, dwFlags );;
}








HRESULT CMusicSegment::SetRepeats( DWORD dwRepeats )
{
    if( m_pSegment == NULL )
        return CO_E_NOTINITIALIZED;

    return m_pSegment->SetRepeats( dwRepeats );
}








HRESULT CMusicSegment::GetStyle( IDirectMusicStyle8** ppStyle, DWORD dwStyleIndex )
{
    
    
    
    
    
    
    
    return m_pSegment->GetParam( GUID_IDirectMusicStyle, 0xffffffff, dwStyleIndex, 
                                 0, NULL, (VOID*)ppStyle );
}







CMusicScript::CMusicScript( IDirectMusicPerformance8* pPerformance, 
                            IDirectMusicLoader8* pLoader,                   
                            IDirectMusicScript8* pScript )
{
    m_pPerformance = pPerformance;
    m_pLoader      = pLoader;
    m_pScript      = pScript;
}








CMusicScript::~CMusicScript()
{
    if( m_pLoader )
    {
        
        m_pLoader->ReleaseObjectByUnknown( m_pScript );
        m_pLoader = NULL;
    }

    SAFE_RELEASE( m_pScript ); 
    m_pPerformance = NULL;
}








HRESULT CMusicScript::CallRoutine( TCHAR* strRoutine )
{
    
    WCHAR wstrRoutine[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wstrRoutine, strRoutine );

    return m_pScript->CallRoutine( wstrRoutine, NULL );
}








HRESULT CMusicScript::SetVariableNumber( TCHAR* strVariable, LONG lValue )
{
    
    WCHAR wstrVariable[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wstrVariable, strVariable );

    return m_pScript->SetVariableNumber( wstrVariable, lValue, NULL );
}








HRESULT CMusicScript::GetVariableNumber( TCHAR* strVariable, LONG* plValue )
{
    
    WCHAR wstrVariable[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wstrVariable, strVariable );

    return m_pScript->GetVariableNumber( wstrVariable, plValue, NULL );
}



