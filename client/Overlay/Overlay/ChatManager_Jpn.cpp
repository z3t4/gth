



#include <time.h>
#include <math.h>
#include <stdio.h>
#include <afxwin.h>
#include <d3dx8.h>
#include <mmsystem.h>
#include "../global_def.h"

#ifdef _GTH_TERRA_VERSION

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

void CChatManager::DeleteCombination()
{

	int len, movelen;
	len = strlen( m_combi );
	movelen = strlen( m_inputStr[m_curIdx] + m_csPos[m_curIdx] + len ) + 1;
	memmove( m_inputStr[m_curIdx] + m_csPos[m_curIdx], m_inputStr[m_curIdx] + m_csPos[m_curIdx] + len, movelen );
}

void CChatManager::SetCombi( HWND hwnd )
{
	char temp[4096], csTemp[4096];
	HIMC m_hIMC;
	COMPOSITIONFORM cf;

	m_hIMC = ImmGetContext( hwnd );

	memset( temp, 0, sizeof( temp ) );
	memcpy( temp, m_inputStr[m_curIdx] + m_inputOutPos[m_curIdx], m_inputOutSize[m_curIdx] );

	memset( csTemp, 0, sizeof( csTemp ) );
	memcpy( csTemp, temp, m_csPos[m_curIdx] - m_inputOutPos[m_curIdx] );

	cf.dwStyle = CFS_FORCE_POSITION;
	cf.ptCurrentPos.x = m_ix[m_curIdx] + GetChatStrSizeChat( csTemp );
	cf.ptCurrentPos.y = m_iy[m_curIdx];

	ImmSetCompositionWindow(m_hIMC, &cf);

	ImmReleaseContext( hwnd, m_hIMC );
}

void CChatManager::AddCombination()
{
	int len, newlen;
	char temp[CHAT_BUFFER_SIZE];

	
	len = strlen( m_inputStr[m_curIdx] );
	if( len >= m_inputSize[m_curIdx] || len + strlen(m_combi) > m_inputSize[m_curIdx] ) return;

	
	if( len == m_csPos[m_curIdx] )
	{
		
		strcat( m_inputStr[m_curIdx], m_combi );
		

		if ( strlen(m_inputStr[m_curIdx]) > m_inputSize[m_curIdx] )
			m_inputStr[m_curIdx][m_inputSize[m_curIdx]] = 0;

		return;
	}
	
	memset( temp, 0, sizeof( temp ) );
	memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] );

	newlen = len + strlen(m_combi);
	if ( newlen > m_inputSize[m_curIdx] )
	{
		int t1;

		t1 = newlen - m_inputSize[m_curIdx];
		m_combi[newlen - len - t1] = 0;
	}
	
	strcat( temp, m_combi );
	m_csPos[m_curIdx] += strlen( m_combi );

	strcat( temp + m_csPos[m_curIdx], 
		m_inputStr[m_curIdx] + m_csPos[m_curIdx] - strlen( m_combi ) );
	strcpy( m_inputStr[m_curIdx], temp );

	m_csPos[m_curIdx] -= strlen( m_combi );

	return;
}

int CChatManager::GetString( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int len;
	HIMC m_hIMC = NULL;

	switch( message )
	{

	
	case WM_IME_STARTCOMPOSITION:

		if( m_inputState[m_curIdx] == 2 )
			return( CHAT_INPUT_RETURN_NONE );

		
		m_csSaveTime = m_csCurTime = timeGetTime();

		m_hIMC = ImmGetContext( hwnd );
		ImmSetCompositionFont(m_hIMC, &m_logFont);
		ImmReleaseContext( hwnd, m_hIMC );

		SetCombi( hwnd );

		memset( m_combi, 0, sizeof(m_combi) );

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

		if( m_inputState[m_curIdx] == 2 )
			return( CHAT_INPUT_RETURN_NONE );
		
		int complen;
		char temp[4096], tCombi[4096];

		m_hIMC = ImmGetContext( hwnd );

		len = strlen( m_inputStr[m_curIdx] ) ;
		complen = ImmGetCompositionString( m_hIMC, GCS_COMPSTR, NULL, 0 );

		if ( complen > m_inputSize[m_curIdx] - ( len - strlen(m_combi) ) ) 
		{
			ImmReleaseContext( hwnd, m_hIMC );
			return ( CHAT_INPUT_RETURN_NONE );
		}

		memset( tCombi, 0, sizeof(tCombi) );
		ImmGetCompositionString( m_hIMC, GCS_COMPSTR, tCombi, complen );
		tCombi[complen] = 0;

		if ( m_inputOutPos[m_curIdx] != m_csPos[m_curIdx] )
		{

			memset( temp, 0, sizeof(temp) );
			memcpy( temp, m_inputStr[m_curIdx] + m_inputOutPos[m_curIdx], m_csPos[m_curIdx] - m_inputOutPos[m_curIdx] );
			strcat( temp, tCombi );

			while ( GetChatStrSizeChat( temp ) > m_inputOutLength[m_curIdx] )
			{
				if ( IsDBCSLeadByte( (BYTE)m_inputStr[m_curIdx][m_inputOutPos[m_curIdx]] ) )
					m_inputOutPos[m_curIdx] += 2;
				else
					m_inputOutPos[m_curIdx] ++;

				if ( m_inputOutPos[m_curIdx] > m_csPos[m_curIdx] )
				{
					m_inputOutPos[m_curIdx] = m_csPos[m_curIdx];
	
					break;
				}

				memset( temp, 0, sizeof(temp) );
				memcpy( temp, m_inputStr[m_curIdx] + m_inputOutPos[m_curIdx], m_csPos[m_curIdx] - m_inputOutPos[m_curIdx] );
				strcat( temp, tCombi );
			}
		}
		else if ( GetChatStrSizeChat( tCombi ) > m_inputOutLength[m_curIdx] )
		{
			ImmReleaseContext( hwnd, m_hIMC );
			return ( CHAT_INPUT_RETURN_NONE );
		}

		if ( (lParam & GCS_RESULTSTR) && (lParam & GCS_COMPSTR) )
		{
			if ( complen > 2 )
			{
				DeleteCombination();
				strcpy (m_combi, tCombi);
				AddCombination();

				ImmReleaseContext( hwnd, m_hIMC );
				break;
			}

			m_csPos[m_curIdx] += strlen (m_combi);

			memset( m_combi, 0, sizeof( m_combi ) );
			ImmGetCompositionString( m_hIMC, GCS_COMPSTR, m_combi, complen );
			m_combi[complen] = 0;

			AddCombination();

			SetCombi( hwnd );

			ImmReleaseContext( hwnd, m_hIMC );
			break;
		}

		if (lParam & GCS_RESULTSTR)
		{
			m_csPos[m_curIdx] += strlen (m_combi);
			strcpy (m_combi, tCombi);
			AddCombination();

			ImmReleaseContext( hwnd, m_hIMC );
			break;
		}

		DeleteCombination();
		strcpy (m_combi, tCombi);
		AddCombination();

		SetCombi( hwnd );

		ImmReleaseContext( hwnd, m_hIMC );
		break;

	case WM_IME_CHAR:

		CHAR szChar[3];

		if (IsDBCSLeadByte((BYTE)(wParam >> 8)))
		{
			szChar[0] = HIBYTE(LOWORD(wParam));
			szChar[1] = LOBYTE(LOWORD(wParam));
			szChar[2] = 0;

			if ( (BYTE)szChar[0] == 0x81 && (BYTE)szChar[1] == 0x40 )
			{
				InsertString( szChar );
			}
		}

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
		else if ( uniCode >= 0xff00 && uniCode <= 0xffff )
		{
			skipchar = true;
		} 
		else if ( uniCode >= 0x0300 && uniCode <= 0x04ff )
		{
			skipchar = true;
		}
		else if ( uniCode >= 0x2000 && uniCode <= 0x26ff )
		{
			skipchar = true;
		}
		else if ( uniCode >= 0x3200 && uniCode <= 0x33ff )
		{
			skipchar = true;
		}
		else if ( uniCode >= 0x3000 && uniCode <= 0x30ff )
		{
			skipchar = true;
		} 
		else if ( uniCode >= 0x4e00 && uniCode <= 0x9fff )
		{
			skipchar = true;
		}
		else if ( uniCode >= 0xf900 && uniCode <= 0xfaff )
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


int CChatManager::FindTexIdx( wchar_t uniCode )
{
	int texIdx;

	if( uniCode >= 0x0000 && uniCode <= 0x00ff ) 
	{
		texIdx = ( uniCode );
	} 
	else if( uniCode >= 0xff00 && uniCode <= 0xffff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( uniCode - 0xff00 );
	} 
	else if( uniCode >= 0x0300 && uniCode <= 0x04ff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0100 );
		texIdx += ( uniCode - 0x0300 );
	}
	else if( uniCode >= 0x2000 && uniCode <= 0x26ff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( uniCode - 0x2000 );
	}
	else if( uniCode >= 0x3200 && uniCode <= 0x33ff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0700 );
		texIdx += ( uniCode - 0x3200 );
	}
	else if( uniCode >= 0x3000 && uniCode <= 0x30ff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0700 );
		texIdx += ( 0x0200 );
		texIdx += ( uniCode - 0x3000);
	} 
	else if( uniCode >= 0x4e00 && uniCode <= 0x9fff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0700 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0100);
		texIdx += ( uniCode - 0x4e00 );
	}
	else if ( uniCode >= 0xf900 && uniCode <=0xfaff )
	{
		texIdx = ( 0x0100 );
		texIdx += ( 0x0100 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0700 );
		texIdx += ( 0x0200 );
		texIdx += ( 0x0100);
		texIdx += ( 0x5200 );
		texIdx += ( uniCode - 0xf900 );
	}
	else
		return -1;

	return texIdx;
}

#endif

