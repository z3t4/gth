



#include <time.h>
#include <math.h>
#include <stdio.h>
#include <afxwin.h>
#include <d3dx8.h>
#include <mmsystem.h>
#include "../global_def.h"
#include "../game/GolryunBattle/GolryunBattle.h"

#ifdef _GTH_KOREAN_VERSION

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

void CChatManager::AddCombination()
{
	int len;
	char temp[4096];

	
	len = strlen( m_inputStr[m_curIdx] );
	if( len >= m_inputSize[m_curIdx] || len + strlen( m_combi ) > m_inputSize[m_curIdx] ) return;
	
	if( len == m_csPos[m_curIdx] )
	{
		
		strcat( m_inputStr[m_curIdx], m_combi );
		
		m_csPos[m_curIdx] += strlen( m_combi );
		return;
	}
	
	memset( temp, 0, sizeof( temp ) );
	memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] );

	
	strcat( temp, m_combi );
	m_csPos[m_curIdx] += strlen( m_combi );

	strcat( temp + m_csPos[m_curIdx], 
		m_inputStr[m_curIdx] + m_csPos[m_curIdx] - strlen( m_combi ) );
	strcpy( m_inputStr[m_curIdx], temp );
}

int CChatManager::GetString( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int len;

	HIMC m_hIMC = NULL;

	switch( message )
	{
	
	case WM_IME_COMPOSITION :
		
		m_csSaveTime = m_csCurTime = timeGetTime();

		m_hIMC = ImmGetContext( hwnd );
		
		len = strlen( m_inputStr[m_curIdx] );

		if( len >= m_inputSize[m_curIdx] ) return( CHAT_INPUT_RETURN_NONE );

		if( lParam & GCS_RESULTSTR )
		{
			len = ImmGetCompositionString( m_hIMC, GCS_RESULTSTR, NULL, 0 );
			if( len > 0 )
			{
				
				ImmGetCompositionString( m_hIMC, GCS_RESULTSTR, m_combi, len );
				
				AddCombination();
				
				memset( m_combi, 0, sizeof( m_combi ) );
			}
		}
		else 
		{
			if( lParam & GCS_COMPSTR )
			{
				len = ImmGetCompositionString( m_hIMC, GCS_COMPSTR, NULL, 0 );
				ImmGetCompositionString( m_hIMC, GCS_COMPSTR, m_combi, len );
				m_combi[len] = 0;
			}
		}
		ImmReleaseContext( hwnd, m_hIMC );
		return( CHAT_INPUT_RETURN_NONE );
	case WM_CHAR :
		
		m_csSaveTime = m_csCurTime = timeGetTime();
		if( wParam == VK_TAB ) return( CHAT_INPUT_RETURN_TAB );
		if( wParam == VK_ESCAPE ) return( CHAT_INPUT_RETURN_EXIT );
		if( wParam == 8 )			
		{
			BackSpace();
		}
		else
		{	
			
			if ( wParam == 1)
				return( CHAT_INPUT_RETURN_NONE );

			if ( wParam == 10)
				return( CHAT_INPUT_RETURN_NONE );

			if( !(GetKeyState( VK_CONTROL ) < 0 ) && wParam != 13 ) AddChar( wParam & 0xff );
			
		}
		return( CHAT_INPUT_RETURN_NONE );
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
		
		if( uniCode < 0xc800 && uniCode >= 0xac00 ) 
		{
			skipchar = true;
		} 
		else if( uniCode >= 0xc800 && uniCode < 0xd800 )
		{
			skipchar = true;
		} 
		else if( uniCode >= 0x0000 && uniCode <= 0x0100 )
		{
			return 1;
		} 
		else if( uniCode < 0x3400 && uniCode >= 0x3100 )
		{
			return 0;
		} 
		else
		{
			return 0;
		}
		str ++;
	}
	return 1;
}



int CChatManager::FindTexIdx( wchar_t uniCode )
{
	int texIdx;

	if( uniCode < 0xc800 && uniCode >= 0xac00 ) 
	{
		texIdx = ( uniCode - 0xac00 );
	} 
	else if( uniCode >= 0xc800 && uniCode < 0xd800 )
	{
		texIdx = ( 0xc800 - 0xac00 );
		texIdx += ( uniCode - 0xc800 );
	} 
	else if( uniCode >= 0x0000 && uniCode <= 0x0100 )
	{
		texIdx = ( 0xc800 - 0xac00 );
		texIdx += ( 0xd800 - 0xc800 );
		texIdx += ( uniCode );
	} 
	else if( uniCode < 0x3400 && uniCode >= 0x3100 )
	{
		texIdx = ( 0xc800 - 0xac00 );
		texIdx += ( 0xd800 - 0xc800 );
		texIdx += ( 0x0100 );
		texIdx += ( uniCode - 0x3100 );
	}
	else
		return -1;

	return texIdx;

}


#endif