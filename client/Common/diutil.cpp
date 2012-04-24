





#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#include <basetsd.h>
#include <tchar.h>
#include <stdio.h>
#include "DIUtil.h"
#include "DXUtil.h"



 





CInputDeviceManager::CInputDeviceManager()
{
    HRESULT hr = CoInitialize(NULL);
    m_bCleanupCOM = SUCCEEDED(hr);

    m_dwNumDevices = 0;
    m_dwMaxDevices = 10;
    m_pDI          = NULL;

    
    m_pDevices = NULL;
    m_pDevices = (DeviceInfo*) realloc( m_pDevices, m_dwMaxDevices*sizeof(DeviceInfo) );
    ZeroMemory( m_pDevices, m_dwMaxDevices*sizeof(DeviceInfo) );
}








CInputDeviceManager::~CInputDeviceManager()
{
    if( m_pDevices )
    {
        
        for( DWORD i=0; i<m_dwNumDevices; i++ )
        {
            m_pDevices[i].pdidDevice->Unacquire();
            m_pDevices[i].pdidDevice->Release();
            m_pDevices[i].pdidDevice = NULL;
        }

		if(m_pDevices)
			free(m_pDevices);
    }

    
    SAFE_RELEASE( m_pDI );

    if( m_bCleanupCOM )
        CoUninitialize();
}








HRESULT CInputDeviceManager::GetDevices( DeviceInfo** ppDeviceInfo, 
                                         DWORD* pdwCount )
{
    if( NULL==ppDeviceInfo || NULL==pdwCount )
        return E_INVALIDARG;

    (*ppDeviceInfo) = m_pDevices;
    (*pdwCount) = m_dwNumDevices;

    return S_OK;
}








HRESULT CInputDeviceManager::AddDevice( const DIDEVICEINSTANCE* pdidi, 
                                        const LPDIRECTINPUTDEVICE8 pdidDevice )
{
    HRESULT hr;
    DWORD   dwDeviceType = pdidi->dwDevType;

    pdidDevice->Unacquire();

    
    hr = pdidDevice->SetCooperativeLevel( m_hWnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND|DISCL_NOWINKEY );
    if( FAILED(hr) )
        return hr;

    
    m_dwNumDevices++;
    if( m_dwNumDevices > m_dwMaxDevices )
    {
        m_dwMaxDevices += 10;
        m_pDevices = (DeviceInfo*) realloc( m_pDevices, m_dwMaxDevices*sizeof(DeviceInfo) );
        ZeroMemory( m_pDevices + m_dwMaxDevices - 10, 10*sizeof(DeviceInfo) );
    }

    DWORD dwCurrentDevice = m_dwNumDevices-1;
    m_pDevices[dwCurrentDevice].pdidDevice = pdidDevice;
    m_pDevices[dwCurrentDevice].pdidDevice->AddRef();

    
    
    if( m_AddDeviceCallback )
    {
        hr = m_AddDeviceCallback( &m_pDevices[dwCurrentDevice], pdidi, m_AddDeviceCallbackParam ); 
        if( FAILED(hr) )    
            return hr;
    }

    
    hr = m_pDevices[dwCurrentDevice].pdidDevice->BuildActionMap( &m_diaf, m_strUserName, 0 );
    if( FAILED(hr) )
        return hr;

    
    hr = m_pDevices[dwCurrentDevice].pdidDevice->SetActionMap( &m_diaf, m_strUserName, 0 );
    if( FAILED(hr) )
        return hr;

    
    return S_OK;
}









BOOL CALLBACK EnumSuitableDevicesCB( LPCDIDEVICEINSTANCE pdidi, 
                                     LPDIRECTINPUTDEVICE8 pdidDevice, 
                                     DWORD dwFlags, DWORD dwDeviceRemaining,
                                     VOID* pContext )
{
    
    ((CInputDeviceManager*)pContext)->AddDevice( pdidi, pdidDevice );

    
    return DIENUM_CONTINUE;
}









HRESULT CInputDeviceManager::SetActionFormat( DIACTIONFORMAT& diaf, BOOL bReenumerate )
{
    HRESULT hr = S_OK;

    
    m_diaf = diaf;

    
    
    
    if( bReenumerate )
    {
        
        for( DWORD i=0; i<m_dwNumDevices; i++ )
        {
            m_pDevices[i].pdidDevice->Unacquire();
            m_pDevices[i].pdidDevice->Release();
            m_pDevices[i].pdidDevice = NULL;
        }
        m_dwNumDevices = 0;

        
        hr = m_pDI->EnumDevicesBySemantics( m_strUserName, &m_diaf, 
                                            EnumSuitableDevicesCB, this, 0L );
        if( FAILED(hr) )
            return hr;
    }
    else 
    {
        
        UnacquireDevices();

        
        for( DWORD i=0; i<m_dwNumDevices; i++ )
        {
            hr = m_pDevices[i].pdidDevice->BuildActionMap( &m_diaf, m_strUserName, 0 );
            if( FAILED(hr) )
                return hr;

            hr = m_pDevices[i].pdidDevice->SetActionMap( &m_diaf, m_strUserName, 0 );
            if( FAILED(hr) )
                return hr;
        }
    }

    if( FAILED(hr) )
        return hr;

    return S_OK;
}








HRESULT CInputDeviceManager::Create( HWND hWnd, TCHAR* strUserName, 
                                     DIACTIONFORMAT& diaf,
                                     LPDIMANAGERCALLBACK AddDeviceCallback, 
                                     LPVOID pCallbackParam )
{
    HRESULT hr;

    
    m_hWnd        = hWnd;
    m_strUserName = strUserName;
    m_AddDeviceCallback = AddDeviceCallback;
    m_AddDeviceCallbackParam = pCallbackParam;
    
    
    hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                              IID_IDirectInput8, (VOID**)&m_pDI, NULL );
    if( FAILED(hr) )
        return hr;

    hr = SetActionFormat( diaf, TRUE );
    if( FAILED(hr) )
        return hr;

    return S_OK;
}








HRESULT CInputDeviceManager::ConfigureDevices( HWND hWnd, IUnknown* pSurface,
                                               VOID* ConfigureDevicesCB,
                                               DWORD dwFlags, LPVOID pvCBParam )
{
    HRESULT hr;

    
    DICOLORSET dics;
    ZeroMemory(&dics, sizeof(DICOLORSET));
    dics.dwSize = sizeof(DICOLORSET);

    
    DICONFIGUREDEVICESPARAMS dicdp;
    ZeroMemory(&dicdp, sizeof(dicdp));
    dicdp.dwSize = sizeof(dicdp);
    dicdp.dwcFormats     = 1;
    dicdp.lprgFormats    = &m_diaf;
    dicdp.hwnd           = hWnd;
    dicdp.lpUnkDDSTarget = pSurface;

    if( m_strUserName )
    {
        dicdp.dwcUsers       = 1;
        dicdp.lptszUserNames = m_strUserName;
    }

    
    UnacquireDevices();

    hr = m_pDI->ConfigureDevices( (LPDICONFIGUREDEVICESCALLBACK)ConfigureDevicesCB, 
                                  &dicdp, dwFlags, pvCBParam );
    if( FAILED(hr) )
        return hr;

    if( dwFlags & DICD_EDIT )
    {
        
        hr = SetActionFormat( m_diaf, TRUE );
        if( FAILED(hr) )
            return hr;
    }

    return S_OK;
}







VOID CInputDeviceManager::UnacquireDevices()
{
    for( DWORD i=0; i<m_dwNumDevices; i++ )
        m_pDevices[i].pdidDevice->Unacquire();
}








VOID CInputDeviceManager::SetFocus( HWND hWnd ) 
{
    m_hWnd = hWnd;

    UnacquireDevices();

    for( DWORD i=0; i<m_dwNumDevices; i++ )
    {
        
        m_pDevices[i].pdidDevice->SetCooperativeLevel( m_hWnd, 
                                        DISCL_NONEXCLUSIVE|DISCL_FOREGROUND|DISCL_NOWINKEY );
    }
}


