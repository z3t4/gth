



#include <time.h>
#include <math.h>
#include <stdio.h>
#include <afxwin.h>
#include <d3dx8.h>
#include <mmsystem.h>
#include "../global_def.h"

#ifdef _GTH_TAIWAN_VERSION

int CChatManager::BuildStr( int &tx1, int &ty1, int &tx2, int &ty2, char *str )
{
	int texIdx;
	bool en;
	
	wchar_t uniCode;
	unsigned char aniCode[3];
	
	memset( aniCode, 0, sizeof( aniCode ) );
	
	aniCode[0] = *str;

	tx1 = ty1 = tx1 = ty1 = 0;

	if( aniCode[0] == '\n' || aniCode[0] == '\r' )
	{
		return( 3 );
	}
	
	
	if( aniCode[0] == ' ' )
	{
		tx1 = 0;
		tx2 = 6;
		return( 2 );
	}
	
	
	if( IsDBCSLeadByte( (BYTE)aniCode[0] ) )
	{
		aniCode[1] = * ++str;
		aniCode[2] = 0;
		MultiByteToWideChar( CP_ACP, MB_COMPOSITE, (char*)aniCode, -1, &uniCode, 1 );
		en = false;
	}
	else
	{
		aniCode[1] = 0;
		MultiByteToWideChar( CP_ACP, MB_COMPOSITE, (char*)aniCode, -1, &uniCode, 1 );
		en = true;
	}


	
	texIdx = FindTexIdx( uniCode );
	if ( texIdx == -1 )
		return( 4 );
	
	tx1 = m_hanTexCoord[texIdx].tx1;
	ty1 = m_hanTexCoord[texIdx].ty1;
	tx2 = m_hanTexCoord[texIdx].tx2;
	ty2 = m_hanTexCoord[texIdx].ty2;

	
	if( en ) return( 1 );
	
	else return( 0 );
}


void CChatManager::InsertString( char *str )
{
	int len1, len2;
	int movelen;

	
	len1 = strlen( str );
	len2 = strlen( m_inputStr[m_curIdx] );

	if ( len1 == 0 || len2 >= m_inputSize[m_curIdx] || len1 + len2 > m_inputSize[m_curIdx] ) return;

	movelen = strlen( m_inputStr[m_curIdx] + m_csPos[m_curIdx] ) + 1;
	memmove( m_inputStr[m_curIdx] + m_csPos[m_curIdx] + len1, m_inputStr[m_curIdx] + m_csPos[m_curIdx], movelen );
	memcpy( m_inputStr[m_curIdx] + m_csPos[m_curIdx], str, len1 );

	m_csPos[m_curIdx] += len1;
}

void CChatManager::SetCombi( HWND hwnd )
{
	char temp[4096], csTemp[4096];
	HIMC m_hIMC;
	COMPOSITIONFORM cf;

	m_hIMC = ImmGetContext( hwnd );

	SetInputOutSize( m_curIdx );

	memset( temp, 0, sizeof( temp ) );
	memcpy( temp, m_inputStr[m_curIdx] + m_inputOutPos[m_curIdx], m_inputOutSize[m_curIdx] );

	memset( csTemp, 0, sizeof( csTemp ) );
	memcpy( csTemp, temp, m_csPos[m_curIdx] - m_inputOutPos[m_curIdx] );

	cf.dwStyle = CFS_POINT;
	cf.ptCurrentPos.x = m_ix[m_curIdx] + GetChatStrSizeChat( csTemp );
	cf.ptCurrentPos.y = m_iy[m_curIdx];

	ImmSetCompositionWindow(m_hIMC, &cf);

	ImmReleaseContext( hwnd, m_hIMC );
}

int CChatManager::GetString( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int len;
	HIMC m_hIMC = NULL;

	switch( message )
	{

	
	case WM_IME_STARTCOMPOSITION:

		m_hIMC = ImmGetContext( hwnd );
		ImmSetCompositionFont(m_hIMC, &m_logFont);
		ImmReleaseContext( hwnd, m_hIMC );

		bIsActiveCompWnd = TRUE;

		SetCombi( hwnd );

		break;

	case WM_IME_ENDCOMPOSITION:

		bIsActiveCompWnd = FALSE;

		break;

	case WM_CHAR :
		
		m_csSaveTime = m_csCurTime = timeGetTime();

		if( wParam == VK_TAB ) return( CHAT_INPUT_RETURN_TAB );
		if( wParam == VK_ESCAPE ) return( CHAT_INPUT_RETURN_EXIT );
		if( wParam == VK_BACK )			
		{
			BackSpace();
		}
		else
		{
			if( !(GetKeyState( VK_CONTROL ) < 0 ) && wParam != 13 ) AddChar( (BYTE)wParam );
		}

		return( CHAT_INPUT_RETURN_NONE );

	case WM_IME_COMPOSITION :

		SetCombi( hwnd );

		break;

	case WM_IME_CHAR:

		CHAR szChar[3];

		if (IsDBCSLeadByte((BYTE)(wParam >> 8)))
		{
			szChar[0] = HIBYTE(LOWORD(wParam));
			szChar[1] = LOBYTE(LOWORD(wParam));
			szChar[2] = 0;
		}
		else
		{
			szChar[0] = (BYTE)wParam;
			szChar[1] = 0;
		}

		InsertString( szChar );

		SetCombi( hwnd );

		return ( CHAT_INPUT_RETURN_NONE );

	case WM_KEYDOWN :
		
		m_csSaveTime = m_csCurTime = timeGetTime();
		
		
		if( wParam == VK_RIGHT && ( GetKeyState( VK_SHIFT ) < 0 ) )
		{
			len = strlen( m_inputStr[m_curIdx] );
			
			if( m_clipStartPos < 0 && m_csPos[m_curIdx] != len ) 
				m_clipStartPos = m_csPos[m_curIdx];
			
			Right();
		}
		if( wParam == VK_LEFT && ( GetKeyState( VK_SHIFT ) < 0 ) )
		{
			if( m_clipStartPos < 0 && m_csPos[m_curIdx] > 0 ) 
				m_clipStartPos = m_csPos[m_curIdx];
			Left();
		}
		
		if( wParam == VK_RIGHT && !( GetKeyState( VK_SHIFT ) < 0 ) ) 
		{
			if( m_clipStartPos >= 0 ) 
			{
				if( m_csPos[m_curIdx] < m_clipStartPos ) 
					m_csPos[m_curIdx] = m_clipStartPos;
				m_clipStartPos = -1;
			}
			else Right();
		}
		if( wParam == VK_LEFT && !( GetKeyState( VK_SHIFT ) < 0 ) ) 
		{
			if( m_clipStartPos >= 0 ) 
			{
				if( m_csPos[m_curIdx] > m_clipStartPos ) 
					m_csPos[m_curIdx] = m_clipStartPos;
				m_clipStartPos = -1;
			}
			else Left();
		}

		if( wParam == VK_UP )
		{
			if( m_inputLineFeed[m_curIdx] )
				Up();
		}

		if( wParam == VK_DOWN )
		{
			if( m_inputLineFeed[m_curIdx] )
				Down();
		}

		if( wParam == VK_DELETE ) Delete();
		
		if( wParam == VK_HOME && ( GetKeyState( VK_SHIFT ) < 0 ) )
		{
			if( m_clipStartPos < 0 && m_csPos[m_curIdx] > 0 ) 
				m_clipStartPos = m_csPos[m_curIdx];
			m_csPos[m_curIdx] = 0;
		}
		if( wParam == VK_END && ( GetKeyState( VK_SHIFT ) < 0 ) )
		{
			len = strlen( m_inputStr[m_curIdx] );
			
			if( m_clipStartPos < 0 && m_csPos[m_curIdx] != len ) 
				m_clipStartPos = m_csPos[m_curIdx];
			m_csPos[m_curIdx] = strlen( m_inputStr[m_curIdx] );
		}
		
		if( wParam == VK_HOME && !( GetKeyState( VK_SHIFT ) < 0 ) ) 
		{
			if( m_clipStartPos >= 0 ) 
			{
				if( m_csPos[m_curIdx] > m_clipStartPos ) m_csPos[m_curIdx] = m_clipStartPos;
				m_clipStartPos = -1;
			}
			m_csPos[m_curIdx] = 0;
		}
		if( wParam == VK_END && !( GetKeyState( VK_SHIFT ) < 0 ) )
		{
			if( m_clipStartPos >= 0 ) 
			{
				if( m_csPos[m_curIdx] < m_clipStartPos ) m_csPos[m_curIdx] = m_clipStartPos;
				m_clipStartPos = -1;
			}
			m_csPos[m_curIdx] = strlen( m_inputStr[m_curIdx] );
		}
		
		if( wParam == 'c' || wParam == 'C' )
		{
			if( ( GetKeyState( VK_CONTROL ) < 0 ) )
			{
				Copy();
			}
		}
		
		if( wParam == 'x' || wParam == 'X' )
		{
			if( ( GetKeyState( VK_CONTROL ) < 0 ) )
			{
				Copy();
				if( m_clipStartPos >= 0 ) BackSpace();
			}
		}
		
		if( wParam == 'v' || wParam == 'V' )
		{
			if( ( GetKeyState( VK_CONTROL ) < 0 ) )
			{
				if( m_clipStartPos >= 0 ) BackSpace();
				Paste();
			}
		}
		if( wParam == VK_RETURN )
		{
			if( m_curIdx == INPUT_INDEX_MAIL_CONTENT )
			{
				AddChar( '\r' );
				AddChar( '\n' );

				return CHAT_INPUT_RETURN_OK;
			}
			
			if( m_inputStr[m_curIdx][0] == 0 ) return( CHAT_INPUT_RETURN_OK );
			else 
			{
				strcpy( m_returnStr, m_inputStr[m_curIdx] );
				return( CHAT_INPUT_RETURN_OK );
			}
		}
		return( CHAT_INPUT_RETURN_NONE );
	}
	return( CHAT_INPUT_RETURN_SKIP );
}



int	CChatManager::IsVaildString( char *str )
{
	bool en, skipchar;
	
	wchar_t uniCode;
	unsigned char aniCode[3];
	
	while( *str )
	{		
		memset( aniCode, 0, sizeof( aniCode ) );
		
		aniCode[0] = *str;
		
		if( aniCode[0] == '\n' )
		{
			str ++;
			continue;
		}
		
		
		if( aniCode[0] == ' ' )
		{
			str ++; 
			
			
			return 0;
		}
		
		
		if( IsDBCSLeadByte( (BYTE)aniCode[0] ) )
		{
			aniCode[1] = * ++str;
			aniCode[2] = 0;
			MultiByteToWideChar( CP_ACP, MB_COMPOSITE, (char*)aniCode, -1, &uniCode, 1 );
			en = false;
		}
		else
		{
			aniCode[1] = 0;
			MultiByteToWideChar( CP_ACP, MB_COMPOSITE, (char*)aniCode, -1, &uniCode, 1 );
			en = true;
		}

		
		if ( uniCode >= 0x0000 && uniCode <= 0x00ff ) 
		{
			return 1;
		} 
		else if ( uniCode >= 0xfe00 && uniCode <= 0xffff )
		{
			skipchar = true;
		} 
		else if ( uniCode >= 0x0200 && uniCode <= 0x03ff )
		{
			skipchar = true;
		}
		else if ( uniCode >= 0x2000 && uniCode <= 0x26ff )
		{
			skipchar = true;
		}
		else if ( uniCode >= 0x3000 && uniCode <= 0x33ff )
		{
			skipchar = true;
		}
		else if ( uniCode >= 0x4e00 && uniCode <= 0x9fff )
		{
			skipchar = true;
		}
		else
		{
			return 0;
		}
		str ++;
	}
	return 1;
}

bool CChatManager::IsInnerStatusWind( )
{
	HIMC hImc;
	POINT pt, pos;

	GetCursorPos( &pt );

	hImc = ImmGetContext( g_pApp->m_hWnd );

	if ( !ImmGetOpenStatus( hImc ) ) return false;

	ImmGetStatusWindowPos( hImc, &pos );

	if ( pt.x >= pos.x && pt.x <= pos.x + 155 && pt.y >= pos.y && pt.y <= pos.y + 25 )
		return true;

	return false;
}


int CChatManager::FindTexIdx( wchar_t uniCode )
{
	int texIdx;

	if( uniCode >= 0x0000 && uniCode <= 0x00ff ) 
	{
		texIdx = ( uniCode );
	} 
	else if( uniCode >= 0xfe00 && uniCode <= 0xffff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( uniCode - 0xfe00 );
	} 
	else if( uniCode >= 0x0200 && uniCode <= 0x03ff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( uniCode - 0x0200 );
	}
	else if( uniCode >= 0x2000 && uniCode <= 0x26ff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0200 );
		texIdx += ( uniCode - 0x2000 );
	}
	else if( uniCode >= 0x3000 && uniCode <= 0x33ff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0700 );
		texIdx += ( uniCode - 0x3000 );
	}
	else if( uniCode >= 0x4e00 && uniCode <= 0x9fff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0400 );
		texIdx += ( 0x0700 );
		texIdx += ( 0x0400 );
		texIdx += ( uniCode - 0x4e00 );
	}
	else 
		return -1;

	return texIdx;
}


#endif 