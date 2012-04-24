








#include "im.h"





CIM::CIM() 
{ 
m_lpszImeInput = new char[_CIM_MAXINPUTCHARNUMBER]; 
ZeroMemory( m_lpszImeInput,_CIM_MAXINPUTCHARNUMBER ); 
*m_lpszImeInput = '\0'; 
*m_cInput = '\0'; 



} 

CIM::~CIM() 
{ 
ZeroMemory( m_lpszImeInput,_CIM_MAXINPUTCHARNUMBER ); 




} 

void CIM::OnImeComposition(HWND hWnd, LPARAM lParam) 
{ 
if (lParam & GCS_RESULTSTR) 
{ 
DWORD dwLen; 
LPSTR lpResultStr; 

HIMC hIMC = ImmGetContext(hWnd); 

if (!hIMC) 
return; 

dwLen = ImmGetCompositionString(hIMC,GCS_RESULTSTR,NULL,0L); 
dwLen+=1; 

if(dwLen) 
{ 
lpResultStr = new char[ dwLen ]; 


if( strlen( m_lpszImeInput ) + dwLen > _CIM_MAXINPUTCHARNUMBER - 2 ) 
{ 
MessageBeep( 0 ); 
return; 
} 
ZeroMemory( lpResultStr ,dwLen ); 
if (lpResultStr) 
{ 
ImmGetCompositionString(hIMC,GCS_RESULTSTR, lpResultStr,dwLen); 
strcat( m_lpszImeInput,lpResultStr ); 
} 
delete lpResultStr; 
} 

ImmReleaseContext(hWnd,hIMC); 
} 

} 

void CIM::OnImeNotify(HWND hWnd, WPARAM wParam) 
{ 
DWORD dwCommand = (DWORD) wParam; 
switch( dwCommand ) 
{ 
case IMN_CHANGECANDIDATE: 
GetCandidateList( hWnd ); 
break; 

case IMN_CLOSECANDIDATE: 

break; 

case IMN_OPENCANDIDATE: 
GetCandidateList( hWnd ); 
break; 
} 
} 

void CIM::OnChar( TCHAR ch ) 
{ 
int len = strlen( m_lpszImeInput ); 

if( ImeIsOpen() ) 
{ 

} 
else 
{ 
if( ch >= 32 && ch < 128 && len < _CIM_MAXINPUTCHARNUMBER - 1 ) 
{ 
*( m_lpszImeInput + len ) = ch; 
*( m_lpszImeInput + len + 1) = '\0'; 
} 
} 

if( ch == 8 ) 
{ 
if( len == 0 ) 
return; 

if( len == 1 ) 
{ 
*m_lpszImeInput = '\0'; 
return; 
} 

BYTE cc1,cc2; 
cc1 = *(m_lpszImeInput + len -1); 
cc2 = *(m_lpszImeInput + len -2); 

if( cc1 > 0xA0 && cc2 > 0xA0 ) 
*( m_lpszImeInput + len -2 ) = '\0'; 
else 
*( m_lpszImeInput + len -1 ) = '\0'; 
} 
} 

BOOL CIM::GetCandidateList(HWND hWnd) 
{ 
DWORD dwSize; 
HIMC hIMC; 

ZeroMemory( m_cCandidateList,sizeof(m_cCandidateList) ); 

if( m_lpCandList ) 
{ 
delete m_lpCandList; 
m_lpCandList = NULL; 
} 

if( GetKeyboardLayout(0)==0 ) 
{ 
return FALSE; 
} 

hIMC = ImmGetContext(hWnd); 
if(hIMC == NULL) 
{ 
return FALSE; 
} 

ZeroMemory( m_cCandidateList,sizeof(m_cCandidateList) ); 
if(dwSize = ImmGetCandidateList(hIMC,0x0,NULL,0)) 
{ 
m_lpCandList = (LPCANDIDATELIST)new char[dwSize]; 
if(m_lpCandList) 
{ 
ImmGetCandidateList(hIMC,0x0,m_lpCandList,dwSize); 
CandidateToString(m_lpCandList); 
} 
} 

ImmReleaseContext(hWnd,hIMC); 

return TRUE; 
} 

void CIM::ClearCandidateList() 
{ 
if(m_lpCandList) 
{ 
delete m_lpCandList; 
m_lpCandList = NULL; 
} 
ZeroMemory( m_cCandidateList,sizeof( m_cCandidateList ) ); 
} 


LPSTR CIM::GetCurInputLanguageDesc() 
{ 
HKL hKL = GetKeyboardLayout(0); 




int lengh = ImmGetDescription(hKL,NULL,0); 







return m_lpszCurInputLanguageDesc; 
} 

void CIM::UpdateShow() 
{ 
POINT pt; 
pt.y = 450; 
pt.x = 400; 



pt.y = 420; 
pt.x = 20; 


pt.y = 450; 
pt.x = 20; 
if( *m_lpszImeInput == '\0' ) 
return; 


} 

LPSTR CIM::GetResultString() 
{ 
return m_lpszImeInput; 
} 

BOOL CIM::CandidateToString(LPCANDIDATELIST lpCandidateList) 
{ 
if( !m_lpCandList ) 
return FALSE; 

if( m_lpCandList->dwCount>0 ) 
{ 
LPDWORD lpdwOffset; 

lpdwOffset = &m_lpCandList->dwOffset[0]; 

lpdwOffset += m_lpCandList->dwPageStart; 

ZeroMemory( m_lpCandList,sizeof( m_lpCandList ) ); 

DWORD z=1; 
for (DWORD i = m_lpCandList->dwPageStart; (i < lpCandidateList->dwCount) && (i < m_lpCandList->dwPageStart + m_lpCandList->dwPageSize); i++) 
{ 
LPSTR lpstr = (LPSTR)m_lpCandList + *lpdwOffset++; 
char buf[255]; 


z++; 
} 

return TRUE; 
} 

return FALSE; 
} 

BOOL CIM::ImeIsOpen() 
{ 
return ImmIsIME( GetKeyboardLayout(0) ); 
}
