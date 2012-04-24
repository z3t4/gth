




#define STRICT
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <stdio.h> 
#include <stdarg.h>
#include "DXUtil.h"








const TCHAR* DXUtil_GetDXSDKMediaPath()
{
    static TCHAR strNull[2] = _T("");
    static TCHAR strPath[MAX_PATH];
    DWORD dwType;
    DWORD dwSize = MAX_PATH;
    HKEY  hKey;

    
    LONG lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                _T("Software\\Microsoft\\DirectX SDK"),
                                0, KEY_READ, &hKey );
    if( ERROR_SUCCESS != lResult )
        return strNull;

    lResult = RegQueryValueEx( hKey, _T("DX81SDK Samples Path"), NULL,
                              &dwType, (BYTE*)strPath, &dwSize );
    RegCloseKey( hKey );

    if( ERROR_SUCCESS != lResult )
        return strNull;

    _tcscat( strPath, _T("\\Media\\") );

    return strPath;
}








HRESULT DXUtil_FindMediaFile( TCHAR* strPath, TCHAR* strFilename )
{
    HANDLE file;
    TCHAR strFullPath[1024];
    TCHAR *strShortName;
    DWORD cchPath;

    if( NULL==strFilename || NULL==strPath )
        return E_INVALIDARG;

    
    cchPath = GetFullPathName(strFilename, sizeof(strFullPath)/sizeof(TCHAR), strFullPath, &strShortName);
    if ((cchPath == 0) || (sizeof(strFullPath)/sizeof(TCHAR) <= cchPath))
        return E_FAIL;

    
    file = CreateFile( strFullPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, 0, NULL );
    if( INVALID_HANDLE_VALUE != file )
    {
        _tcscpy( strPath, strFullPath );
        CloseHandle( file );
        return S_OK;
    }
    
    
    file = CreateFile( strShortName, GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, 0, NULL );
    if( INVALID_HANDLE_VALUE != file )
    {
        _tcscpy( strPath, strShortName );
        CloseHandle( file );
        return S_OK;
    }
    
    
    _stprintf( strPath, _T("%s%s"), DXUtil_GetDXSDKMediaPath(), strShortName );

    file = CreateFile( strPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, 0, NULL );
    if( INVALID_HANDLE_VALUE != file )
    {
        CloseHandle( file );
        return S_OK;
    }

    
    _tcscpy( strPath, strFilename );
    return E_FAIL;
}








HRESULT DXUtil_ReadStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue, 
                                 DWORD dwLength, TCHAR* strDefault )
{
    DWORD dwType;

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)strValue, &dwLength ) )
    {
        _tcscpy( strValue, strDefault );
    }

    return S_OK;
}








HRESULT DXUtil_WriteStringRegKey( HKEY hKey, TCHAR* strRegName,
                                  TCHAR* strValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_SZ, 
                                        (BYTE*)strValue, 
                                        (_tcslen(strValue)+1)*sizeof(TCHAR) ) )
        return E_FAIL;

    return S_OK;
}








HRESULT DXUtil_ReadIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD* pdwValue, 
                              DWORD dwDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(DWORD);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)pdwValue, &dwLength ) )
    {
        *pdwValue = dwDefault;
    }

    return S_OK;
}








HRESULT DXUtil_WriteIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD dwValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
                                        (BYTE*)&dwValue, sizeof(DWORD) ) )
        return E_FAIL;

    return S_OK;
}








HRESULT DXUtil_ReadBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL* pbValue, 
                              BOOL bDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(BOOL);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)pbValue, &dwLength ) )
    {
        *pbValue = bDefault;
    }

    return S_OK;
}








HRESULT DXUtil_WriteBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL bValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
                                        (BYTE*)&bValue, sizeof(BOOL) ) )
        return E_FAIL;

    return S_OK;
}








HRESULT DXUtil_ReadGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID* pGuidValue, 
                               GUID& guidDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(GUID);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (LPBYTE) pGuidValue, &dwLength ) )
    {
        *pGuidValue = guidDefault;
    }

    return S_OK;
}








HRESULT DXUtil_WriteGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID guidValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_BINARY, 
                                        (BYTE*)&guidValue, sizeof(GUID) ) )
        return E_FAIL;

    return S_OK;
}
















FLOAT __stdcall DXUtil_Timer( TIMER_COMMAND command )
{
    static BOOL     m_bTimerInitialized = FALSE;
    static BOOL     m_bUsingQPF         = FALSE;
    static BOOL     m_bTimerStopped     = TRUE;
    static LONGLONG m_llQPFTicksPerSec  = 0;

    
    if( FALSE == m_bTimerInitialized )
    {
        m_bTimerInitialized = TRUE;

        
        
        LARGE_INTEGER qwTicksPerSec;
        m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
        if( m_bUsingQPF )
            m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

    if( m_bUsingQPF )
    {
        static LONGLONG m_llStopTime        = 0;
        static LONGLONG m_llLastElapsedTime = 0;
        static LONGLONG m_llBaseTime        = 0;
        double fTime;
        double fElapsedTime;
        LARGE_INTEGER qwTime;
        
        
        
        if( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            qwTime.QuadPart = m_llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        
        if( command == TIMER_GETELAPSEDTIME )
        {
            fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
            m_llLastElapsedTime = qwTime.QuadPart;
            return (FLOAT) fElapsedTime;
        }
    
        
        if( command == TIMER_GETAPPTIME )
        {
            double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
            return (FLOAT) fAppTime;
        }
    
        
        if( command == TIMER_RESET )
        {
            m_llBaseTime        = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_llStopTime        = 0;
            m_bTimerStopped     = FALSE;
            return 0.0f;
        }
    
        
        if( command == TIMER_START )
        {
            if( m_bTimerStopped )
                m_llBaseTime += qwTime.QuadPart - m_llStopTime;
            m_llStopTime = 0;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_bTimerStopped = FALSE;
            return 0.0f;
        }
    
        
        if( command == TIMER_STOP )
        {
            m_llStopTime = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_bTimerStopped = TRUE;
            return 0.0f;
        }
    
        
        if( command == TIMER_ADVANCE )
        {
            m_llStopTime += m_llQPFTicksPerSec/10;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
            return (FLOAT) fTime;
        }

        return -1.0f; 
    }
    else
    {
        
        static double m_fLastElapsedTime  = 0.0;
        static double m_fBaseTime         = 0.0;
        static double m_fStopTime         = 0.0;
        double fTime;
        double fElapsedTime;
        
        
        
        if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            fTime = m_fStopTime;
        else
            fTime = timeGetTime() * 0.001;
    
        
        if( command == TIMER_GETELAPSEDTIME )
        {   
            fElapsedTime = (double) (fTime - m_fLastElapsedTime);
            m_fLastElapsedTime = fTime;
            return (FLOAT) fElapsedTime;
        }
    
        
        if( command == TIMER_GETAPPTIME )
        {
            return (FLOAT) (fTime - m_fBaseTime);
        }
    
        
        if( command == TIMER_RESET )
        {
            m_fBaseTime         = fTime;
            m_fLastElapsedTime  = fTime;
            m_fStopTime         = 0;
            m_bTimerStopped     = FALSE;
            return 0.0f;
        }
    
        
        if( command == TIMER_START )
        {
            if( m_bTimerStopped )
                m_fBaseTime += fTime - m_fStopTime;
            m_fStopTime = 0.0f;
            m_fLastElapsedTime  = fTime;
            m_bTimerStopped = FALSE;
            return 0.0f;
        }
    
        
        if( command == TIMER_STOP )
        {
            m_fStopTime = fTime;
            m_fLastElapsedTime  = fTime;
            m_bTimerStopped = TRUE;
            return 0.0f;
        }
    
        
        if( command == TIMER_ADVANCE )
        {
            m_fStopTime += 0.1f;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            return (FLOAT) fTime;
        }

        return -1.0f; 
    }
}










VOID DXUtil_ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, 
                                     int cchDestChar )
{
    if( wstrDestination==NULL || strSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = strlen(strSource)+1;

    MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
                         wstrDestination, cchDestChar-1 );

    wstrDestination[cchDestChar-1] = 0;
}










VOID DXUtil_ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                     int cchDestChar )
{
    if( strDestination==NULL || wstrSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = wcslen(wstrSource)+1;

    WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
                         cchDestChar-1, NULL, NULL );

    strDestination[cchDestChar-1] = 0;
}










VOID DXUtil_ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
    if( strDestination==NULL || tstrSource==NULL || cchDestChar == 0 )
        return;

#ifdef _UNICODE
    DXUtil_ConvertWideStringToAnsi( strDestination, tstrSource, cchDestChar );
#else
    if( cchDestChar == -1 )
    {
        strcpy( strDestination, tstrSource );
    }
    else
    {
        strncpy( strDestination, tstrSource, cchDestChar );
        strDestination[cchDestChar-1] = '\0';
    }
#endif
}










VOID DXUtil_ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
    if( wstrDestination==NULL || tstrSource==NULL || cchDestChar == 0 )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
    {
        wcscpy( wstrDestination, tstrSource );
    }
    else
    {
        wcsncpy( wstrDestination, tstrSource, cchDestChar );
        wstrDestination[cchDestChar-1] = L'\0';
    }
#else
    DXUtil_ConvertAnsiStringToWide( wstrDestination, tstrSource, cchDestChar );
#endif
}










VOID DXUtil_ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, 
                                        int cchDestChar )
{
    if( tstrDestination==NULL || strSource==NULL || cchDestChar == 0 )
        return;
        
#ifdef _UNICODE
    DXUtil_ConvertAnsiStringToWide( tstrDestination, strSource, cchDestChar );
#else
    if( cchDestChar == -1 )
    {
        strcpy( tstrDestination, strSource );
    }
    else
    {
        strncpy( tstrDestination, strSource, cchDestChar );
        tstrDestination[cchDestChar-1] = '\0';
    }
#endif
}










VOID DXUtil_ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, 
                                        int cchDestChar )
{
    if( tstrDestination==NULL || wstrSource==NULL || cchDestChar == 0 )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
    {
        wcscpy( tstrDestination, wstrSource );
    }
    else
    {
        wcsncpy( tstrDestination, wstrSource, cchDestChar );
        tstrDestination[cchDestChar-1] = L'\0';
    }
#else
    DXUtil_ConvertWideStringToAnsi( tstrDestination, wstrSource, cchDestChar );
#endif
}








HRESULT _DbgOut( TCHAR* strFile, DWORD dwLine, HRESULT hr, TCHAR* strMsg )
{
    TCHAR buffer[256];
    wsprintf( buffer, _T("%s(%ld): "), strFile, dwLine );
    OutputDebugString( buffer );
    OutputDebugString( strMsg );

    if( hr )
    { 
        wsprintf( buffer, _T("(hr=%08lx)\n"), hr );
        OutputDebugString( buffer );
    }

    OutputDebugString( _T("\n") );

    return hr;
}









VOID DXUtil_Trace( TCHAR* strMsg, ... )
{
#if defined(DEBUG) | defined(_DEBUG)
    TCHAR strBuffer[512];
    
    va_list args;
    va_start(args, strMsg);
    _vsntprintf( strBuffer, 512, strMsg, args );
    va_end(args);

    OutputDebugString( strBuffer );
#else
    UNREFERENCED_PARAMETER(strMsg);
#endif
}








BOOL DXUtil_ConvertStringToGUID( const TCHAR* strIn, GUID* pGuidOut )
{
    UINT aiTmp[10];

    if( _stscanf( strIn, TEXT("{%8X-%4X-%4X-%2X%2X-%2X%2X%2X%2X%2X%2X}"),
                    &pGuidOut->Data1, 
                    &aiTmp[0], &aiTmp[1], 
                    &aiTmp[2], &aiTmp[3],
                    &aiTmp[4], &aiTmp[5],
                    &aiTmp[6], &aiTmp[7],
                    &aiTmp[8], &aiTmp[9] ) != 11 )
    {
        ZeroMemory( pGuidOut, sizeof(GUID) );
        return FALSE;
    }
    else
    {
        pGuidOut->Data2       = (USHORT) aiTmp[0];
        pGuidOut->Data3       = (USHORT) aiTmp[1];
        pGuidOut->Data4[0]    = (BYTE) aiTmp[2];
        pGuidOut->Data4[1]    = (BYTE) aiTmp[3];
        pGuidOut->Data4[2]    = (BYTE) aiTmp[4];
        pGuidOut->Data4[3]    = (BYTE) aiTmp[5];
        pGuidOut->Data4[4]    = (BYTE) aiTmp[6];
        pGuidOut->Data4[5]    = (BYTE) aiTmp[7];
        pGuidOut->Data4[6]    = (BYTE) aiTmp[8];
        pGuidOut->Data4[7]    = (BYTE) aiTmp[9];
        return TRUE;
    }
}








VOID DXUtil_ConvertGUIDToString( const GUID* pGuidIn, TCHAR* strOut )
{
    _stprintf( strOut, TEXT("{%0.8X-%0.4X-%0.4X-%0.2X%0.2X-%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X}"),
               pGuidIn->Data1, pGuidIn->Data2, pGuidIn->Data3,
               pGuidIn->Data4[0], pGuidIn->Data4[1],
               pGuidIn->Data4[2], pGuidIn->Data4[3],
               pGuidIn->Data4[4], pGuidIn->Data4[5],
               pGuidIn->Data4[6], pGuidIn->Data4[7] );
}
