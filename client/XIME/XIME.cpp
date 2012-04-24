



#include "../stdafx.h"

#include "XIME.h"





BOOL IsDBCS(LPCTSTR strBuf, int nPos)
{	
    BOOL bRet = FALSE;	
    for(int i = 0; i <= nPos; i++)
    {
        if(isascii(strBuf[i])==0)
        {
			
			{
				if(nPos == i+1)
					bRet = TRUE;
			}
			i++;
        }
    }
    return bRet;
}

_XIMEContainer::_XIMEContainer()
{
	ResetContainer();
}

_XIMEContainer::~_XIMEContainer()
{
}

void _XIMEContainer::ResetContainer( void )
{	
	m_length						= 0;
	m_MaxLength						= MAX_IMEBUFFERLENGTH;
	m_cursorX						= 0;
	m_active						= false; 
	m_comLength						= 0;
	m_candleNum						= -1;
	m_AlignType						= _XFONT_ALIGNTYPE_LEFT;
	m_ShowWindow					= FALSE;	
	m_NoFocusedDraw					= FALSE;
	m_ShowWindowRectangle			= FALSE;
	m_WindowPosition.x				= m_WindowPosition.y = 0;
	m_WindowSize.cx					= 256;
	m_WindowSize.cy					= 12;	
	m_TextColor						= _XSC_DEFAULT;
	m_pReturnKeyCallbackFunction	= NULL;
	m_pEscapeKeyCallbackFunction	= NULL;
	m_pTabKeyCallbackFunction		= NULL;
	m_pUpArrowKeyCallbackFunction	= NULL;
	m_pDownArrowKeyCallbackFunction = NULL;
	m_pLeftArrowKeyCallbackFunction = NULL;
	m_pRightArrowKeyCallbackFunction = NULL;
	m_pBackSpaceKeyCallbackFunction = NULL;
	m_pDataChangeCallbackFunction   = NULL;
	m_pNextIMEContainer				= NULL;
	m_PasswordEdit					= FALSE;
	m_bNewLine						= FALSE;
	m_HorizScrolling				= FALSE;	
	m_ScrollPosition				= 0;

	memset( m_text, 0, sizeof(TCHAR) * (MAX_IMEBUFFERLENGTH + 1) );
	memset( m_candleText, 0, sizeof(TCHAR) * MAX_CANDLE_LENGTH * MAX_CANDLE_COUNT );
	memset( m_comText, 0, sizeof(TCHAR) * (MAX_IMEBUFFERLENGTH + 1) );
}

void _XIMEContainer::ClearBuffer( void )
{
	memset( m_text, 0, sizeof(TCHAR) * (MAX_IMEBUFFERLENGTH + 1) );
	memset( m_candleText, 0, sizeof(TCHAR) * MAX_CANDLE_LENGTH * MAX_CANDLE_COUNT );
	memset( m_comText, 0, sizeof(TCHAR) * (MAX_IMEBUFFERLENGTH + 1) );

	m_length						= 0;
	m_cursorX						= 0;	
	m_comLength						= 0;
	m_candleNum						= -1;
	m_ScrollPosition				= 0;
}

void _XIMEContainer::SetFocus( void )
{
	_XIMEKERNEL.SetFocus( this );
}

BOOL _XIMEContainer::IsFocused( void )
{
	return (_XIMEKERNEL.GetIMEContainer() == this);
}


void _XIMEContainer::AddChar(TCHAR c)
{
	if( !m_HorizScrolling )
	{
		if( g_XBaseFont.GetWidth( m_text ) > m_WindowSize.cx ) return;
	}

	if(m_length == m_MaxLength) return;

	int rightLen = 0;
	TCHAR rightStr[MAX_IMEBUFFERLENGTH];
	memset( rightStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
	rightLen = GetRightString(rightStr);

	m_text[m_cursorX++] = c;
	m_length++;

	if(rightLen)
	{
		memset( m_text + m_cursorX, 0, sizeof(TCHAR) * (MAX_IMEBUFFERLENGTH - m_cursorX ) );
		strncpy(m_text + m_cursorX, rightStr ,rightLen);
	}

	m_text[m_length] = NULL;
	
	
	if( m_pDataChangeCallbackFunction )
	{
		m_pDataChangeCallbackFunction( this );
	}

	if( m_HorizScrolling )
	{
		int leftLen = 0;
		TCHAR leftStr[MAX_IMEBUFFERLENGTH];		
		memset( leftStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
		leftLen = GetLeftString(leftStr);

		if( leftLen > 0 )
		{
			int leftWidth = g_XBaseFont.GetWidth( leftStr );
			if( leftWidth > m_WindowSize.cx )
			m_ScrollPosition = leftWidth - m_WindowSize.cx;
		}
	}
}


void _XIMEContainer::AddWord(TCHAR *w)		
{
	if( !m_HorizScrolling )
	{
		if( g_XBaseFont.GetWidth( m_text ) > m_WindowSize.cx ) return;
	}
	
	if(m_length + 2 > m_MaxLength) return;

	TCHAR rightStr[MAX_IMEBUFFERLENGTH];
	memset( rightStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
	int rightLen = GetRightString(rightStr);

	m_text[m_cursorX++] = *w;
	m_text[m_cursorX++] = *(w+1);
	m_length+= 2;

	if(rightLen)
	{
		memset( m_text + m_cursorX, 0, sizeof(TCHAR) * (MAX_IMEBUFFERLENGTH - m_cursorX ) );
		strncpy(m_text + m_cursorX, rightStr ,rightLen);
	}

	m_text[m_length] = NULL;
	

	if( m_pDataChangeCallbackFunction )
	{
		m_pDataChangeCallbackFunction( this );
	}

	if( m_HorizScrolling )
	{
		int leftLen = 0;
		TCHAR leftStr[MAX_IMEBUFFERLENGTH];		
		memset( leftStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
		leftLen = GetLeftString(leftStr);
		
		if( leftLen > 0 )
		{
			int leftWidth = g_XBaseFont.GetWidth( leftStr );
			if( leftWidth > m_WindowSize.cx )
				m_ScrollPosition = leftWidth - m_WindowSize.cx;
		}
	}
}

void _XIMEContainer::AddString(TCHAR *string,int length)		
{
	TCHAR *tmp = string;
	
	for(int i = 0;i<length;i++)
	{
		if(IsDBCSLeadByte(*tmp))
		{
			AddWord(tmp++);
			i++; tmp++;

		}
		else
		{
			AddChar(*tmp);
			tmp++;
		}
	}
}

void _XIMEContainer::ResetCandleList( void )
{	
	memset( m_candleText, 0, sizeof(TCHAR) * MAX_CANDLE_LENGTH * MAX_CANDLE_COUNT );
	m_candleNum = 0;
	m_candleLength = 0;
}

void _XIMEContainer::SetCandleText(int candleindex, const TCHAR *candles)
{
	if( candleindex >= MAX_CANDLE_COUNT ) return;
	memset( m_candleText[candleindex], 0, sizeof(TCHAR) * MAX_CANDLE_LENGTH );
	strncpy( m_candleText[candleindex],candles,strlen(candles) );
}

void _XIMEContainer::SetCandleNum(int num)
{
	m_candleNum = num;
}

void _XIMEContainer::SetCandleLength(int len)
{
	m_candleLength = len;
}

void _XIMEContainer::ProcessDelKey( void )				
{
	if(m_cursorX == m_length)
		return;

	TCHAR rightStr[MAX_IMEBUFFERLENGTH];
	memset( rightStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
	int rightLen = GetRightString(rightStr);

	int check = 1;

	
	if(IsDBCSLeadByte(m_text[m_cursorX]))		
	{
		m_length--;
		check += 1;
	}
	m_length--;

	if( m_length < 0 )
	{
		m_length = 0;
	}
	
	if( m_cursorX < 0 )
	{ 
		m_cursorX = 0;
	}

	m_text[m_cursorX] = 0;
	
	if(rightLen > 0 && (rightLen - check) > 0 )
	   strncpy(m_text + m_cursorX, rightStr + check,rightLen - check);

	m_text[m_length] = 0;

	if( m_pDataChangeCallbackFunction )
	{
		m_pDataChangeCallbackFunction( this );
	}
}

void _XIMEContainer::ProcessHomeKey( void )
{
	m_cursorX = 0;

	if( m_HorizScrolling )
	{
		int leftLen = 0;
		TCHAR leftStr[MAX_IMEBUFFERLENGTH];		
		memset( leftStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
		leftLen = GetLeftString(leftStr);
		
		if( leftLen > 0 )
		{
			int leftWidth = g_XBaseFont.GetWidth( leftStr );
			if( leftWidth < m_ScrollPosition )
				m_ScrollPosition = leftWidth;
		}
		else
		{
			m_ScrollPosition = 0;
		}
	}
}

void _XIMEContainer::ProcessEndKey( void )
{
	m_cursorX = m_length;

	if( m_HorizScrolling )
	{
		int leftLen = 0;
		TCHAR leftStr[MAX_IMEBUFFERLENGTH];		
		memset( leftStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
		leftLen = GetLeftString(leftStr);
		
		if( leftLen > 0 )
		{
			int leftWidth = g_XBaseFont.GetWidth( leftStr );
			if( leftWidth > m_WindowSize.cx )
				m_ScrollPosition = leftWidth - m_WindowSize.cx;
		}
	}
}

void _XIMEContainer::ProcessBackSpaceKey( void )
{
	if(m_cursorX <= 0)
	{
		m_cursorX = 0;
		m_bNewLine = TRUE;
		return ;
	}

	TCHAR rightStr[MAX_IMEBUFFERLENGTH];
	memset( rightStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
	int rightLen = GetRightString(rightStr);

	if( IsDBCS( m_text, --m_cursorX ) )
	
	{
		--m_cursorX;
		--m_length;
	}
	--m_length;

	if( m_length < 0 )
	{
		m_length = 0;
		m_cursorX = 0;
		m_ScrollPosition = 0;
		memset( m_text, 0, sizeof(TCHAR) * MAX_IMEBUFFERLENGTH );
		if( m_pDataChangeCallbackFunction )
		{
			m_pDataChangeCallbackFunction( this );
		}

		return;
	}

	if( m_cursorX < 0 )
	{ 
		m_cursorX = 0;
	}

	m_text[m_cursorX] = 0;

	if(rightLen > 0)
	{
		memset( m_text + m_cursorX, 0, sizeof(TCHAR) * (MAX_IMEBUFFERLENGTH - m_cursorX ) );
		strncpy(m_text + m_cursorX, rightStr ,rightLen);
	}

	
	memset( m_text + m_length, 0, sizeof(TCHAR) * (MAX_IMEBUFFERLENGTH - m_length ) );

	if( m_pDataChangeCallbackFunction )
	{
		m_pDataChangeCallbackFunction( this );
	}

	if( m_HorizScrolling )
	{
		int leftLen = 0;
		TCHAR leftStr[MAX_IMEBUFFERLENGTH];		
		memset( leftStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
		leftLen = GetLeftString(leftStr);
		
		if( leftLen > 0 )
		{
			int leftWidth = g_XBaseFont.GetWidth( leftStr );
			if( leftWidth < m_ScrollPosition )
				m_ScrollPosition = leftWidth;
		}
		else
		{
			m_ScrollPosition = 0;
		}
	}
}

void _XIMEContainer::ProcessEscapeKey( void )
{
	
}

void _XIMEContainer::ProcessTabKey( void )
{
	if( m_pNextIMEContainer )
	{		
		_XIMEKERNEL.SetFocus( m_pNextIMEContainer );
	}
	else if( m_pTabKeyCallbackFunction )
	{
		m_pTabKeyCallbackFunction( this );
	}
}

void _XIMEContainer::ProcessLeftKey( void )
{
	if(m_cursorX == 0)
	{
		m_bNewLine = TRUE;
		return;
	}

	
	if(IsDBCSLeadByte(m_text[--m_cursorX]))
	{
		--m_cursorX;
	}
	

	if( m_HorizScrolling )
	{
		int leftLen = 0;
		TCHAR leftStr[MAX_IMEBUFFERLENGTH];		
		memset( leftStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
		leftLen = GetLeftString(leftStr);
		
		if( leftLen > 0 )
		{
			int leftWidth = g_XBaseFont.GetWidth( leftStr );
			if( leftWidth < m_ScrollPosition )
				m_ScrollPosition = leftWidth;
		}
		else
		{
			m_ScrollPosition = 0;
		}
	}
}

void _XIMEContainer::ProcessRightKey( void )
{
	if(m_cursorX == m_length)
	{
		m_bNewLine = TRUE;
		return;
	}

	
	if(IsDBCSLeadByte(m_text[m_cursorX++]))
	{
		m_cursorX++;
	}
	
	if( m_HorizScrolling )
	{
		int leftLen = 0;
		TCHAR leftStr[MAX_IMEBUFFERLENGTH];		
		memset( leftStr, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
		leftLen = GetLeftString(leftStr);
		
		if( leftLen > 0 )
		{
			int leftWidth = g_XBaseFont.GetWidth( leftStr );
			if( leftWidth > m_WindowSize.cx )
				m_ScrollPosition = leftWidth - m_WindowSize.cx;
		}
	}
}

void _XIMEContainer::CandleLeft( void )
{
	if(!IsCandle()) return;

	if(m_candleNum == 0) return;

	m_candleNum -= MAX_CANDLE_VIEWCOUNT;	
	UpdateCandleList();
}

void _XIMEContainer::CandleRight( void )
{
	if(!IsCandle())
		return;

	if(m_candleNum + MAX_CANDLE_VIEWCOUNT >= m_candleLength)
		return;

	m_candleNum += MAX_CANDLE_VIEWCOUNT;
	UpdateCandleList();
}

void _XIMEContainer::UpdateCandleList( void )
{
	if( g_IMEWindow.m_pListBox )
	{
		g_IMEWindow.m_pListBox->DeleteAllItem();
				
		if( m_candleNum > -1 )
		{
			TCHAR candlestring[MAX_CANDLE_LENGTH];
			memset( candlestring, 0, sizeof(TCHAR) * MAX_CANDLE_LENGTH );
			
			for( int i = m_candleNum; i < m_candleNum+MAX_CANDLE_VIEWCOUNT; i++ )
			{									
				if( !strlen(m_candleText[i]) ) break;				
				sprintf( candlestring, "%2d %s", i-m_candleNum+1, m_candleText[i] );
				g_IMEWindow.m_pListBox->InsertListItem( candlestring, _XSC_DEFAULT );
			}		
		}
	}
}


int _XIMEContainer::GetLeftString(TCHAR *target)			
{
	if(m_cursorX == 0)
		return 0;

	strncpy(target, m_text, m_cursorX);
	return m_cursorX;
}

int _XIMEContainer::GetRightString(TCHAR *target)		
{
	if(m_cursorX == m_length)
		return 0;

	strncpy(target,m_text + m_cursorX, m_length - m_cursorX);

	return m_length - m_cursorX;
}



void _XIMEContainer::SetCursorX( int cursorx )
{
	if( cursorx < m_length )
		m_cursorX = cursorx;
	else
		m_cursorX = m_length;
}

void _XIMEContainer::SetComText(TCHAR *text,int length)
{
	strncpy(m_comText,text, length);
	m_comText[length] = 0;
	m_comLength = length;
}

void _XIMEContainer::SetComTextBlank( void )
{
	m_comText[0] = 0; 
	m_comLength = 0;
}


int	_XIMEContainer::GetMaxLength( void )
{
	return m_MaxLength;
}

TCHAR* _XIMEContainer::GetText( void )
{ 
	return m_text;
}

TCHAR* _XIMEContainer::GetComText( void )
{
	return m_comText;
}

int	_XIMEContainer::GetComTextlength( void )	
{ 
	return m_comLength;
}

TCHAR* _XIMEContainer::GetCandle( int candleindex )
{
	if( candleindex >= m_candleNum ) return NULL;
	return m_candleText[candleindex];
}

int	_XIMEContainer::GetCandleTextlength( void )
{
	return m_candleLength;
}

int	_XIMEContainer::GetLength( void )	
{
	return m_length;
}

int	 _XIMEContainer::GetCursorX( void )
{
	return m_cursorX;
}

bool _XIMEContainer::GetAvtive( void )	
{
	return m_active;
}

bool _XIMEContainer::GetNewLine( void )	
{
	return m_bNewLine;
}

bool _XIMEContainer::GetFocus( void )
{
	return ( _XIMEKERNEL.GetIMEContainer() == this );
}

bool _XIMEContainer::IsCandle( void )
{
	if(m_candleNum == -1)
		return false;

	return true;
}


void _XIMEContainer::MoveWindow( int X, int Y )
{
	m_WindowPosition.x = X;
	m_WindowPosition.y = Y;
}

void _XIMEContainer::ResizeWindow( int width, int height )
{ 
	m_WindowSize.cx = width; 
	m_WindowSize.cy = height; 
}




void _XIMEContainer::Draw( _XGUIObject*& pfocusobject )
{	
	if( !m_ShowWindow && !m_NoFocusedDraw )	return;
		
	TCHAR leftstring[MAX_IMEBUFFERLENGTH];
	TCHAR rightstring[MAX_IMEBUFFERLENGTH];
	TCHAR textbuffer[MAX_IMEBUFFERLENGTH];

	memset( leftstring,  0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
	memset( rightstring, 0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
	memset( textbuffer,  0, sizeof(TCHAR)*MAX_IMEBUFFERLENGTH );
	
	GetLeftString(leftstring);
	GetRightString(rightstring);
	 
	sprintf( textbuffer, "%s%s%s", leftstring, m_comText, rightstring );

	if( m_ShowWindowRectangle )
	{		
		_XDrawRectAngle( m_WindowPosition.x - 2, m_WindowPosition.y-2, m_WindowPosition.x+m_WindowSize.cx+1, 
						 m_WindowPosition.y+m_WindowSize.cy+1, 0.0f, D3DCOLOR_ARGB(255,20,20,20) );
	}

	if( m_PasswordEdit )
	{
		TCHAR pwString[_XDEF_MAXPWSTRINGSIZE];
		memset( pwString, '*', _XDEF_MAXPWSTRINGSIZE-1 );
		pwString[_XDEF_MAXPWSTRINGSIZE-1] = '\0';
		pwString[ strlen( textbuffer ) ] = '\0';

		g_XBaseFont.SetColor(m_TextColor);

		int strwidth = g_XBaseFont.GetWidth( textbuffer );

		int alignpt;
		switch( m_AlignType ) 
		{
		default:
		case _XFONT_ALIGNTYPE_LEFT	:
			alignpt = 0;
			g_XBaseFont.Puts( m_WindowPosition.x, m_WindowPosition.y, pwString );
			break;
		case _XFONT_ALIGNTYPE_RIGHT :			
			alignpt = m_WindowSize.cx - strwidth;
			g_XBaseFont.Puts( m_WindowPosition.x+alignpt, m_WindowPosition.y, pwString );
			break;
		case _XFONT_ALIGNTYPE_CENTER :
			alignpt = (m_WindowSize.cx>>1) - (strwidth>>1);
			g_XBaseFont.Puts( m_WindowPosition.x+alignpt, m_WindowPosition.y, pwString );
			break;		
		}		
						
		if( m_ShowWindow || !m_NoFocusedDraw )
		{
			if( (gnFPS>>1) < gnFrameCounter && _XIMEKERNEL.GetIMEContainer() == this )
			{
				int length = strlen( leftstring );
				int asterisksize = g_XBaseFont.GetWidth( "*" );

				g_XBaseFont.SetColor(D3DCOLOR_ARGB( 255, 215, 235, 188 ));
				g_XBaseFont.Puts( m_WindowPosition.x+length*asterisksize+alignpt, m_WindowPosition.y, _XDEF_CARETTYPE );			
			}		
		}

		g_XBaseFont.Flush();
	}
	else
	{	
		g_XBaseFont.SetColor(m_TextColor);		
		int strwidth = g_XBaseFont.GetWidth( textbuffer );

		if(m_HorizScrolling)
		{
			int alignpt;
			int leftsize = g_XBaseFont.GetWidth( leftstring );

			g_XBaseFont.SetClippingArea( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1 );

			if( strwidth > m_WindowSize.cx )
			{
				alignpt = -m_ScrollPosition;
				g_XBaseFont.Puts( m_WindowPosition.x+alignpt, m_WindowPosition.y, textbuffer );
			}
			else
			{
				alignpt = 0;
				g_XBaseFont.Puts( m_WindowPosition.x, m_WindowPosition.y, textbuffer );
			}
			
			if( m_ShowWindow || !m_NoFocusedDraw )
			{
				if( (gnFPS>>1) < gnFrameCounter && _XIMEKERNEL.GetIMEContainer() == this )
				{
					g_XBaseFont.SetColor(D3DCOLOR_ARGB( 255, 215, 235, 188 ));
					
					{
						g_XBaseFont.Puts( m_WindowPosition.x+leftsize+alignpt, m_WindowPosition.y, _XDEF_CARETTYPE );
					}
					
				}
			}
	
			g_XBaseFont.Flush();
			g_XBaseFont.DisableClippingArea();
		}
		else
		{		
			int alignpt;
			switch( m_AlignType ) 
			{
			default:
			case _XFONT_ALIGNTYPE_LEFT	:
				alignpt = 0;
				g_XBaseFont.Puts( m_WindowPosition.x, m_WindowPosition.y, textbuffer );
				break;
			case _XFONT_ALIGNTYPE_RIGHT :			
				alignpt = m_WindowSize.cx - strwidth;
				g_XBaseFont.Puts( m_WindowPosition.x+alignpt, m_WindowPosition.y, textbuffer );			
				break;
			case _XFONT_ALIGNTYPE_CENTER :
				alignpt = (m_WindowSize.cx>>1) - (strwidth>>1);			
				g_XBaseFont.Puts( m_WindowPosition.x+alignpt, m_WindowPosition.y, textbuffer );
				break;
			}
			
			if( m_ShowWindow || !m_NoFocusedDraw )
			{
				if( (gnFPS>>1) < gnFrameCounter && _XIMEKERNEL.GetIMEContainer() == this )
				{	
					int leftsize = g_XBaseFont.GetWidth( leftstring );
					g_XBaseFont.SetColor(D3DCOLOR_ARGB( 255, 215, 235, 188 ));
					g_XBaseFont.Puts( m_WindowPosition.x+leftsize+alignpt, m_WindowPosition.y, _XDEF_CARETTYPE );
				}
			}

			g_XBaseFont.Flush();
		}
	}
}

BOOL _XIMEContainer::CheckMousePosition( void )
{
	if( !m_ShowWindow ) return FALSE;
	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if( scrnpos )
	{	
		return gpInput->CheckMousePos( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy );
	}
	return FALSE;
}





_XIMEKernel * _XIMEKernel::ms_pThis;

_XIMEKernel& _XIMEKernel::GetInstance( void )
{
	assert(ms_pThis!=NULL && "_XIMEKernel :: not initialized ime kernel");
	return *ms_pThis;
}

_XIMEKernel::_XIMEKernel()
{
	ms_pThis=this;
	m_pIMEContainer = NULL;
	ResetCombinationBuffer();
}

_XIMEKernel::~_XIMEKernel()
{
}

_XIMEContainer* _XIMEKernel::GetIMEContainer( void )
{
	return m_pIMEContainer;
}

bool _XIMEKernel::GetComposition(HWND hWnd, WPARAM wparam, LPARAM lparam)
{
	assert(m_pIMEContainer != NULL && "Invalid pointer : _XIMEContainer");

	HIMC	m_hIMC = ImmGetContext(hWnd);
		
	POINT pt;
	pt.x = gnWidth;
	pt.y = gnHeight;
	ImmSetStatusWindowPos(m_hIMC, &pt);
	
	COMPOSITIONFORM CompForm;
	CANDIDATEFORM	CandForm;
	CompForm.dwStyle = CFS_POINT;
	CompForm.ptCurrentPos.x = 3200;
	CompForm.ptCurrentPos.y = 3200;
	ImmSetCompositionWindow(m_hIMC, &CompForm);
	
	CandForm.dwIndex = 0;
	CandForm.dwStyle = CFS_CANDIDATEPOS;
	CandForm.ptCurrentPos.x = 3200;
	CandForm.ptCurrentPos.y = 2000;
	ImmSetCandidateWindow(m_hIMC, &CandForm);
		
	if(lparam & GCS_RESULTSTR)
	{		
		TCHAR szTmp[21];		
		memset( szTmp, 0, 21 );
		int len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0);

		if(len == 0) return false;

		ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, szTmp, len);

		if( m_pIMEContainer )
		{
			m_pIMEContainer->AddString(szTmp,len);
			m_pIMEContainer->SetComText("",0);											
		}
	}
	
	if( 		
		lparam & GCS_COMPSTR)
	{
		TCHAR szTmp[21];
		memset( szTmp, 0, 21 );

		if( m_pIMEContainer )
		{		
			int len = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);
			
			if(len + m_pIMEContainer->GetLength() > m_pIMEContainer->GetMaxLength())	
			{
				ImmNotifyIME(m_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);

				m_pIMEContainer->SetComText("",0);
				return true;
			}

			ImmGetCompositionString(m_hIMC, GCS_COMPSTR, szTmp, len);
			
			m_pIMEContainer->SetComText(szTmp, len);		
		}
	}

	if( !m_pIMEContainer )
	{
		ImmNotifyIME(m_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);		
	}

	ImmReleaseContext(hWnd, m_hIMC);

	return true;
}


bool _XIMEKernel::GetMsg(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
		case WM_CHAR:
			return GetChar(wparam,lparam);			
			break;
		case WM_KEYDOWN:
			return GetKey(wparam,lparam);
			break;
		case WM_KEYUP:
			return false;
			break;
		case WM_IME_NOTIFY:
			return GetCandle(hWnd,wparam,lparam);

		case WM_IME_COMPOSITION:							
			return GetComposition(hWnd,wparam, lparam);

	}
	return false;
}

bool _XIMEKernel::GetKey(WPARAM wparam, LPARAM lparam)
{
	if( !m_pIMEContainer ) return false;
	
	
	switch (wparam){
	case VK_LEFT:
		m_pIMEContainer->ProcessLeftKey();
		if( m_pIMEContainer->GetNewLine() )
		{
			if( m_pIMEContainer->GetLeftArrowKeyCallBack() )
			{
				m_pIMEContainer->GetLeftArrowKeyCallBack()(m_pIMEContainer);
			}
			m_pIMEContainer->SetNewLine( FALSE );
		}
		
		return true;
		
	case VK_RIGHT:
		m_pIMEContainer->ProcessRightKey();
		if( m_pIMEContainer->GetNewLine() )
		{
			if( m_pIMEContainer->GetRightArrowKeyCallBack() )
			{
				m_pIMEContainer->GetRightArrowKeyCallBack()(m_pIMEContainer);
			}
			m_pIMEContainer->SetNewLine( FALSE );
		}
		return true;
		
	case VK_DELETE:
		m_pIMEContainer->ProcessDelKey();
		return true;
		
	case VK_HOME:
		m_pIMEContainer->ProcessHomeKey();
		return true;
		
	case VK_END:
		m_pIMEContainer->ProcessEndKey();
		return true;
		
	case VK_RETURN:  
		if( m_pIMEContainer->GetReturnKeyCallBack() )
		{
			m_pIMEContainer->GetReturnKeyCallBack()(m_pIMEContainer);
		}
		return true;
		
	case VK_UP:  
		if( m_pIMEContainer->GetUpArrowKeyCallBack() )
		{
			m_pIMEContainer->GetUpArrowKeyCallBack()(m_pIMEContainer);
		}
		return true;
	case VK_DOWN:  
		if( m_pIMEContainer->GetDownArrowKeyCallBack() )
		{
			m_pIMEContainer->GetDownArrowKeyCallBack()(m_pIMEContainer);
		}
		return true;
		
	case VK_ESCAPE:
		if( m_pIMEContainer->GetEscapeKeyCallBack() )
		{
			m_pIMEContainer->GetEscapeKeyCallBack()(m_pIMEContainer);
		}
		return true;
	case VK_PROCESSKEY:			
		if( g_LanguageType == _XLANGUAGE_TYPE_CHINESE )
		{
			if(lparam == 0x01490001)				
			{
				m_pIMEContainer->CandleLeft();
			}
			else if(lparam == 0x01510001)		
			{
				m_pIMEContainer->CandleRight();
			}			
		}			
		else 
		{
			if(lparam == 0x14B0001)				
			{
				m_pIMEContainer->CandleLeft();
			}
			else if(lparam == 0x14D0001)		
			{
				m_pIMEContainer->CandleRight();
			}			
		}
		return true;		
	} 
	
	return false;
}



bool _XIMEKernel::GetCandle(HWND hWnd, WPARAM wparam, LPARAM lparam)
{
	HIMC m_hIMC = ImmGetContext(hWnd);

	DWORD dwBufLen;

	LPCANDIDATELIST m_CandList;
	LPCANDIDATELIST lpCandList;
	
	int	 CanNum;

	
	

	switch (wparam) 
	{
		case IMN_OPENCANDIDATE:											
			{
				if((dwBufLen = ImmGetCandidateList(m_hIMC, 0, NULL, 0)) == 0) break;
																		

				m_CandList = (LPCANDIDATELIST)new TCHAR[dwBufLen];		
				lpCandList = m_CandList;

				ImmGetCandidateList(m_hIMC, 0, lpCandList, dwBufLen);	

				CanNum = lpCandList->dwCount;							
				
				if(m_pIMEContainer)
				{
					m_pIMEContainer->ResetCandleList();
					m_pIMEContainer->SetCandleNum(0);
					m_pIMEContainer->SetCandleLength(CanNum);

					for( int i = 0; i < CanNum; i++ )
					{							
						m_pIMEContainer->SetCandleText( i, (LPSTR)(lpCandList) + lpCandList->dwOffset[i] );
					}				

					m_pIMEContainer->UpdateCandleList();

					POINT pt = m_pIMEContainer->GetWindowPosition();
					SIZE  sz = m_pIMEContainer->GetWindowSize();					
					g_IMEWindow.MoveWindow( pt.x + sz.cx, pt.y );
					g_IMEWindow.ShowWindow( TRUE );
															
					pt.x = gnWidth;
					pt.y = gnHeight;
					ImmSetStatusWindowPos(m_hIMC, &pt);

					COMPOSITIONFORM CompForm;
					CANDIDATEFORM	CandForm;
					CompForm.dwStyle = CFS_POINT;
					CompForm.ptCurrentPos.x = 3200;
					CompForm.ptCurrentPos.y = 3200;
					ImmSetCompositionWindow(m_hIMC, &CompForm);

					CandForm.dwIndex = 0;
					CandForm.dwStyle = CFS_CANDIDATEPOS;
					CandForm.ptCurrentPos.x = 3200;
					CandForm.ptCurrentPos.y = 2000;
					ImmSetCandidateWindow(m_hIMC, &CandForm);					
				}				
				
				delete m_CandList;				
			}
			break;
		case IMN_CHANGECANDIDATE:
			{ 
				if(m_pIMEContainer)
				{
					m_pIMEContainer->UpdateCandleList();				
					POINT pt = m_pIMEContainer->GetWindowPosition();
					SIZE  sz = m_pIMEContainer->GetWindowSize();					
					g_IMEWindow.MoveWindow( pt.x + sz.cx, pt.y );
					g_IMEWindow.ShowWindow( TRUE );
				}

				POINT pt;
				pt.x = gnWidth;
				pt.y = gnHeight;
				ImmSetStatusWindowPos(m_hIMC, &pt);
				
				COMPOSITIONFORM CompForm;
				CANDIDATEFORM	CandForm;
				CompForm.dwStyle = CFS_POINT;
				CompForm.ptCurrentPos.x = 3200;
				CompForm.ptCurrentPos.y = 3200;
				ImmSetCompositionWindow(m_hIMC, &CompForm);
				
				CandForm.dwIndex = 0;
				CandForm.dwStyle = CFS_CANDIDATEPOS;
				CandForm.ptCurrentPos.x = 3200;
				CandForm.ptCurrentPos.y = 3200;
				ImmSetCandidateWindow(m_hIMC, &CandForm);					
			}
			break;
		case IMN_CLOSECANDIDATE:
			
			if(m_pIMEContainer)
			{				
				m_pIMEContainer->SetCandleLength(0);
				m_pIMEContainer->SetCandleNum(-1);
			}

			g_IMEWindow.ShowWindow( FALSE );

			if( g_IMEWindow.m_pListBox )
				g_IMEWindow.m_pListBox->DeleteAllItem();
			break;
	}
	
	ImmReleaseContext(hWnd, m_hIMC);

	return true;
}



bool _XIMEKernel::GetChar(WPARAM wparam, LPARAM lparam)
{	
	if( gpInput->CheckKeyPress(DIK_LCONTROL) || gpInput->CheckKeyPress(DIK_LMENU) ) return false;
	if( !m_pIMEContainer ) return false;
	switch (wparam)
	{
	case VK_ESCAPE:
		if( m_pIMEContainer )
			m_pIMEContainer->ProcessEscapeKey();			
		return true;
		
	case VK_TAB:
		if( m_pIMEContainer )
			m_pIMEContainer->ProcessTabKey();
		return true;
		
	case VK_BACK:		
		if( m_pIMEContainer )
		{
			m_pIMEContainer->ProcessBackSpaceKey();
			
			if( m_pIMEContainer->GetNewLine() )
			{
				if( m_pIMEContainer->GetBackSpaceKeyCallBack() )
				{
					m_pIMEContainer->GetBackSpaceKeyCallBack()(m_pIMEContainer);
				}
				m_pIMEContainer->SetNewLine( FALSE );
			}
		}
		return true;
	case VK_RETURN: 
		return true;
		
	default:
		if(IsDBCSLeadByte((BYTE)wparam)) return true; 
		m_pIMEContainer->AddChar((TCHAR)wparam);		
		return true;
	}
	return false;
}


void _XIMEKernel::SetFocus(_XIMEContainer* imecontainer)
{
	m_pIMEContainer = imecontainer;
	if(m_pIMEContainer)
	{
		if( m_pIMEContainer->GetPasswordMode() )
		{
			HIMC hIMC = ImmGetContext(gHWnd);
			if( hIMC )
			{
				ImmSetConversionStatus( hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE );				
				ImmReleaseContext(gHWnd, hIMC);
			}
		}

		m_pIMEContainer->ShowWindow(TRUE);
	}
}

void _XIMEKernel::ResetCombinationBuffer( void )
{
	HIMC m_hIMC = ImmGetContext(gHWnd);
	if(m_hIMC) 
	{
		ImmNotifyIME(m_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(gHWnd, m_hIMC);		

		if( m_pIMEContainer )
			m_pIMEContainer->SetComText("",0);
	}
}

