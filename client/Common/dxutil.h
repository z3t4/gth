




#ifndef DXUTIL_H
#define DXUTIL_H





#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }









const TCHAR* DXUtil_GetDXSDKMediaPath();
HRESULT      DXUtil_FindMediaFile( TCHAR* strPath, TCHAR* strFilename );








HRESULT DXUtil_WriteStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue );
HRESULT DXUtil_WriteIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD dwValue );
HRESULT DXUtil_WriteGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID guidValue );
HRESULT DXUtil_WriteBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL bValue );

HRESULT DXUtil_ReadStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue, DWORD dwLength, TCHAR* strDefault );
HRESULT DXUtil_ReadIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD* pdwValue, DWORD dwDefault );
HRESULT DXUtil_ReadGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID* pGuidValue, GUID& guidDefault );
HRESULT DXUtil_ReadBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL* pbValue, BOOL bDefault );
















enum TIMER_COMMAND { TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE,
                     TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME };
FLOAT __stdcall DXUtil_Timer( TIMER_COMMAND command );







VOID DXUtil_ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar = -1 );
VOID DXUtil_ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar = -1 );
VOID DXUtil_ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
VOID DXUtil_ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
VOID DXUtil_ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, int cchDestChar = -1 );
VOID DXUtil_ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, int cchDestChar = -1 );







VOID DXUtil_ConvertGUIDToString( const GUID* pGuidIn, TCHAR* strOut );
BOOL DXUtil_ConvertStringToGUID( const TCHAR* strIn, GUID* pGuidOut );







VOID    DXUtil_Trace( TCHAR* strMsg, ... );
HRESULT _DbgOut( TCHAR*, DWORD, HRESULT, TCHAR* );

#if defined(DEBUG) | defined(_DEBUG)
    #define DXTRACE           DXUtil_Trace
#else
    #define DXTRACE           sizeof
#endif

#if defined(DEBUG) | defined(_DEBUG)
    #define DEBUG_MSG(str)    _DbgOut( __FILE__, (DWORD)__LINE__, 0, str )
#else
    #define DEBUG_MSG(str)    (0L)
#endif




#endif 
