#pragma comment ( lib, "imm32.lib" )
#include <windows.h>
#include <imm.h>

class CIme{
	bool g_bIme;
	char g_szCompStr[ MAX_PATH ];
	char g_szCompReadStr[ MAX_PATH ];
	char g_szCandList[ MAX_PATH ];
	int g_nImeCursor;
	CANDIDATELIST *g_lpCandList;
	char g_szImeName[ 64 ];
	bool g_bImeSharp;
	bool g_bImeSymbol;
	void ConvertCandList( CANDIDATELIST *pCandList, char *pszCandList );
public:
	CIme() : g_lpCandList( NULL ){ DisableIme(); }
	~CIme(){
		DisableIme();
		if( g_lpCandList ){
			GlobalFree( (HANDLE)g_lpCandList );
			g_lpCandList = NULL;
		}
	}
	
	void DisableIme();
	void EnableIme();
	void NextIme();
	void SharpIme( HWND hWnd );
	void SymbolIme( HWND hWnd );

	
	char* GetImeName();
	bool IfImeSharp();
	bool IfImeSymbol();
	void GetImeInput( char **pszCompStr, char **pszCompReadStr, int *pnImeCursor, char **pszCandList );
		
		
		
		
		

	
	bool OnWM_INPUTLANGCHANGEREQUEST();
	bool OnWM_INPUTLANGCHANGE( HWND hWnd );
	bool OnWM_IME_SETCONTEXT(){ return true; }
	bool OnWM_IME_STARTCOMPOSITION(){ return true; }
	bool OnWM_IME_ENDCOMPOSITION(){ return true; }
	bool OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam );
	bool OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam );
};

void CIme::DisableIme(){
	while( ImmIsIME( GetKeyboardLayout( 0 )))
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
	g_bIme = false;
	g_szImeName[ 0 ] = 0;
	g_szCompStr[ 0 ] = 0;
	g_szCompReadStr[ 0 ] = 0;
	g_szCandList[ 0 ] = 0;
	g_nImeCursor = 0;
}

void CIme::EnableIme(){
	g_bIme = true;
}

void CIme::NextIme(){
	if( g_bIme )ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
}

void CIme::SharpIme( HWND hWnd ){
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SHAPE_TOGGLE );
}

void CIme::SymbolIme( HWND hWnd ){
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SYMBOL_TOGGLE );
}

void CIme::ConvertCandList( CANDIDATELIST *pCandList, char *pszCandList ){
	unsigned int i;
	if( pCandList->dwCount < pCandList->dwSelection ){
		pszCandList[ 0 ] = 0;
		return;
	}
		
		
		
		
	for( i = 0; ( i < pCandList->dwCount - pCandList->dwSelection )&&( i < pCandList->dwPageSize ); i++ ){
		*pszCandList++ = ( i % 10 != 9 )? i % 10 + '1' : '0';
		*pszCandList++ = '.';
		strcpy( pszCandList, (char*)pCandList
			+ pCandList->dwOffset[ pCandList->dwSelection + i ] );
		pszCandList += strlen( pszCandList );
		*pszCandList++ = '\t';
	}
	*( pszCandList - 1 )= 0;
}

bool CIme::OnWM_INPUTLANGCHANGEREQUEST(){
	return !g_bIme;
}

bool CIme::OnWM_INPUTLANGCHANGE( HWND hWnd ){
	
	HKL hKL = GetKeyboardLayout( 0 );
	if( ImmIsIME( hKL )){
		HIMC hIMC = ImmGetContext( hWnd );
		ImmEscape( hKL, hIMC, IME_ESC_IME_NAME, g_szImeName );
		DWORD dwConversion, dwSentence;
		ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
		g_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;
		g_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;
		ImmReleaseContext( hWnd, hIMC );
	}
	else
		g_szImeName[ 0 ] = 0;
	return false;
}

bool CIme::OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam ){
	HIMC hIMC;
	DWORD dwSize;
	DWORD dwConversion, dwSentence;
	switch( wParam ){
		case IMN_SETCONVERSIONMODE:
			hIMC = ImmGetContext( hWnd );
			ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
			g_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;
			g_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;
			ImmReleaseContext( hWnd, hIMC );
			break;
		case IMN_OPENCANDIDATE:
		case IMN_CHANGECANDIDATE:
			hIMC = ImmGetContext( hWnd );
			if( g_lpCandList ){
				GlobalFree( (HANDLE)g_lpCandList );
				g_lpCandList = NULL;
			}
			if( dwSize = ImmGetCandidateList( hIMC, 0, NULL, 0 )){
				g_lpCandList = (LPCANDIDATELIST)GlobalAlloc( GPTR, dwSize );
				if( g_lpCandList )
					ImmGetCandidateList( hIMC, 0, g_lpCandList, dwSize );
			}
			ImmReleaseContext( hWnd, hIMC );
			if( g_lpCandList )ConvertCandList( g_lpCandList, g_szCandList );
			break;
		case IMN_CLOSECANDIDATE:
			if( g_lpCandList ){
				GlobalFree( (HANDLE)g_lpCandList );
				g_lpCandList = NULL;
			}
			g_szCandList[ 0 ] = 0;
			break;
	}
	return true;
}
bool CIme::OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam ){
	HIMC hIMC;
	DWORD dwSize;
	hIMC = ImmGetContext( hWnd );
	if( lParam & GCS_COMPSTR ){
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPSTR, (void*)g_szCompStr, sizeof( g_szCompStr ));
		g_szCompStr[ dwSize ] = 0;
	}
	if( lParam & GCS_COMPREADSTR ){
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPREADSTR, (void*)g_szCompReadStr, sizeof( g_szCompReadStr ));
		g_szCompReadStr[ dwSize ] = 0;
	}
	if( lParam & GCS_CURSORPOS ){
		g_nImeCursor = 0xffff & ImmGetCompositionString( hIMC, GCS_CURSORPOS, NULL, 0 );
	}
	if( lParam & GCS_RESULTSTR ){
		unsigned char str[ MAX_PATH ];
		dwSize = ImmGetCompositionString( hIMC, GCS_RESULTSTR, (void*)str, sizeof( str ));
		str[ dwSize ] = 0;
		unsigned char *p = str;
		while( *p )PostMessage( hWnd, WM_CHAR, (WPARAM)(*p++), 1 );
	}
	ImmReleaseContext( hWnd, hIMC );
	return true;
}

char* CIme::GetImeName(){
	return g_szImeName[ 0 ]? g_szImeName : NULL;
}

bool CIme::IfImeSharp(){
	return g_bImeSharp;
}

bool CIme::IfImeSymbol(){
	return g_bImeSymbol;
}

void CIme::GetImeInput( char **pszCompStr, char **pszCompReadStr, int *pnImeCursor, char **pszCandList ){
	if( pszCompStr )*pszCompStr = g_szCompStr;
	if( pszCompReadStr )*pszCompReadStr = g_szCompReadStr;
	if( pnImeCursor )*pnImeCursor = g_nImeCursor;
	if( pszCandList )*pszCandList = g_szCandList;
}






