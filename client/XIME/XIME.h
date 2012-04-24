



#if !defined(_XIME_H_)
#define _XIME_H_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "IMM.h"




class _XIMEContainer;
typedef	void (__stdcall *_XIMESpecialKeyCallback)( _XIMEContainer* pIMEContainer );
typedef	BOOL (__stdcall *_XIMEStatusCallback)( _XIMEContainer* pIMEContainer );

#define MAX_IMEBUFFERLENGTH			256
#define MAX_MULTILINE_COUNT			256
#define MAX_MULTILINE_LENGTH		256
#define MAX_CANDLE_COUNT			256
#define MAX_CANDLE_LENGTH			32
#define MAX_CANDLE_VIEWCOUNT		9
#define _XDEF_CARETTYPE				"_"
#define _XDEF_MAXPWSTRINGSIZE		32

class _XIMEKernel
{	
public:
	static _XIMEKernel& GetInstance( void );	
	
	_XIMEKernel();
	~_XIMEKernel();
	
	_XIMEContainer*		GetIMEContainer( void );
	void				SetFocus(_XIMEContainer* imecontainer);	
	
	void				Clear( void );
	void				ResetCombinationBuffer( void );
	
	bool				GetMsg(HWND hWnd, UINT msg,WPARAM wparam, LPARAM lparam);
	

	bool				GetChar(WPARAM wparam, LPARAM lparam);
	bool				GetKey(WPARAM wparam, LPARAM lparam);
	bool				GetCandle(HWND hWnd, WPARAM wparam, LPARAM lparam);	
	bool				GetComposition(HWND hWnd, WPARAM wparam, LPARAM lparam);
	
private:
	_XIMEContainer*		m_pIMEContainer;
	
	static _XIMEKernel* ms_pThis;
};

#define _XIMEKERNEL		_XIMEKernel::GetInstance()


class _XIMEContainer
{
private:	
	
public:
	
	_XIMEContainer();
	~_XIMEContainer();
	
	void		ResetContainer( void );
	void		ClearBuffer( void );
	void		SetFocus( void );
	BOOL		IsFocused( void );
		
	
	void		AddChar(TCHAR c);		
	void		AddWord(TCHAR *w);
	void		AddString(TCHAR *string,int length);
	
	
	void		ProcessDelKey( void );
	void		ProcessHomeKey( void );
	void		ProcessEndKey( void );
	void		ProcessBackSpaceKey( void );
	void		ProcessLeftKey( void );
	void		ProcessRightKey( void );
	void		ProcessEscapeKey( void );
	void		ProcessTabKey( void );
	void		CandleLeft( void );
	void		CandleRight( void );
	void		UpdateCandleList( void );
	void		ResetCandleList( void );
	
	
	int			GetMaxLength( void );
	TCHAR*		GetText( void );
	TCHAR*		GetComText( void );
	int			GetComTextlength( void );
	TCHAR*		GetCandle( int candleindex );	
	int			GetCandleTextlength( void );
	int			GetCandleScrollIndex( void ){ return m_candleNum;	}
	int			GetLength( void );
	int			GetCursorX( void );
	bool		GetAvtive( void );
	bool		GetFocus( void );
	bool		IsCandle( void );
	bool		GetNewLine( void );
	
	
	void		SetComText(TCHAR *text,int length);
	void		SetComTextBlank( void );
	
	void		SetCandleText(int candleindex, const TCHAR *candles);
	void		SetCandleNum(int num);
	void		SetCandleLength(int len);
	
	void		SetCursorX( int cursorx );

	void		SetMaxLength( int length )
	{
				if( length > MAX_IMEBUFFERLENGTH ) m_MaxLength = MAX_IMEBUFFERLENGTH;				
				else m_MaxLength = length;
	}
	void		SetNewLine( bool newline ){ m_bNewLine = newline; }

	void		SetHorizScrollMode( BOOL scroll )
	{
				m_HorizScrolling = scroll;
	}
	
	BOOL		CheckMousePosition( void );

	
private:
	BOOL					m_ShowWindow;
	BOOL					m_NoFocusedDraw;
	BOOL					m_ShowWindowRectangle;
	BOOL					m_PasswordEdit;
	POINT					m_WindowPosition;
	SIZE					m_WindowSize;

	D3DCOLOR				m_TextColor;
	DWORD					m_CommandID;
	BOOL					m_HorizScrolling;
	int						m_ScrollPosition;
	_XIMEContainer*			m_pNextIMEContainer;
	_XIMESpecialKeyCallback	m_pReturnKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pEscapeKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pTabKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pUpArrowKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pDownArrowKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pLeftArrowKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pRightArrowKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pBackSpaceKeyCallbackFunction;
	
	_XIMEStatusCallback		m_pDataChangeCallbackFunction;
	
public:	
	void		ShowWindow( BOOL show )
	{ 				
				m_ShowWindow = show; 
				if( !m_ShowWindow && (_XIMEKERNEL.GetIMEContainer() == this) )
				{
					_XIMEKERNEL.SetFocus( NULL );
				}
	}
	BOOL		GetShowStatus( void ){ return m_ShowWindow; }
	void		SetNoFocuseDraw( BOOL draw ){ m_NoFocusedDraw = draw; }
	void		ShowWindowRectAngle( BOOL show ){ m_ShowWindowRectangle = show;	}
	void		MoveWindow( int X, int Y );
	void		ResizeWindow( int width, int height );
	void		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	POINT		GetWindowPosition( void ){ return m_WindowPosition;	}
	SIZE		GetWindowSize( void ){ return m_WindowSize; }
	void		SetAlignType( _XFONT_ALIGNTYPE	aligntype ){ m_AlignType = aligntype; }
	void		SetTextColor( D3DCOLOR textcolor ){ m_TextColor = textcolor; }
	void		SetCommandID( DWORD cmdid ){ m_CommandID = cmdid; }
	DWORD		GetCommandID( void ){ return m_CommandID; }	
	
	_XIMEStatusCallback GetDataChangeCallBack( void )
	{
				return m_pDataChangeCallbackFunction;
	}
	void		SetDataChangeCallBack( _XIMEStatusCallback pDataChangeCallback )
	{	
				m_pDataChangeCallbackFunction = pDataChangeCallback; 
	}

	_XIMESpecialKeyCallback GetReturnKeyCallBack( void )
	{
				return m_pReturnKeyCallbackFunction;
	}
	void		SetReturnKeyCallback( _XIMESpecialKeyCallback pRetCallback )
	{	
				m_pReturnKeyCallbackFunction = pRetCallback; 
	}
	
	_XIMESpecialKeyCallback GetEscapeKeyCallBack( void )
	{
				return m_pEscapeKeyCallbackFunction;
	}
	void		SetEscapeKeyCallback( _XIMESpecialKeyCallback pESCCallback )
	{	
				m_pEscapeKeyCallbackFunction = pESCCallback; 
	}

	_XIMESpecialKeyCallback GetTabKeyCallBack( void )
	{
				return m_pTabKeyCallbackFunction;
	}
	void		SetTabKeyCallback( _XIMESpecialKeyCallback pTabCallback )
	{	
				m_pTabKeyCallbackFunction = pTabCallback; 
	}
	_XIMESpecialKeyCallback GetUpArrowKeyCallBack( void )
	{
		return m_pUpArrowKeyCallbackFunction;
	}
	void		SetUpArrowKeyCallback( _XIMESpecialKeyCallback pUpArrowCallback )
	{	
		m_pUpArrowKeyCallbackFunction = pUpArrowCallback; 
	}
	
	_XIMESpecialKeyCallback GetDownArrowKeyCallBack( void )
	{
		return m_pDownArrowKeyCallbackFunction;
	}
	void		SetDownArrowKeyCallback( _XIMESpecialKeyCallback pDownArrowCallback )
	{	
		m_pDownArrowKeyCallbackFunction = pDownArrowCallback; 
	}
	_XIMESpecialKeyCallback GetLeftArrowKeyCallBack( void )
	{
		return m_pLeftArrowKeyCallbackFunction;
	}
	void		SetLeftArrowKeyCallback( _XIMESpecialKeyCallback pLeftArrowCallback )
	{	
		m_pLeftArrowKeyCallbackFunction = pLeftArrowCallback; 
	}
	
	_XIMESpecialKeyCallback GetRightArrowKeyCallBack( void )
	{
		return m_pRightArrowKeyCallbackFunction;
	}
	void		SetRightArrowKeyCallback( _XIMESpecialKeyCallback pRightArrowCallback )
	{	
		m_pRightArrowKeyCallbackFunction = pRightArrowCallback; 
	}
	_XIMESpecialKeyCallback GetBackSpaceKeyCallBack( void )
	{
		return m_pBackSpaceKeyCallbackFunction;
	}
	void		SetBackSpaceKeyCallback( _XIMESpecialKeyCallback pBackSpaceCallback )
	{	
		m_pBackSpaceKeyCallbackFunction = pBackSpaceCallback; 
	}
	
	void		SetNextIMEContainerHandle( _XIMEContainer* nextimecontainer )
	{
				m_pNextIMEContainer = nextimecontainer;
	}
	void		SetPasswordMode( BOOL mode )
	{
				m_PasswordEdit = mode;
	}
	BOOL		GetPasswordMode( void )
	{
				return m_PasswordEdit;
	}
	

private:
	
	int			GetLeftString(TCHAR *target);		
	int			GetRightString(TCHAR *target);		
	
	
private:
	TCHAR		m_text[MAX_IMEBUFFERLENGTH + 1];				
	int			m_length;
	
	TCHAR		m_candleText[MAX_CANDLE_COUNT][MAX_CANDLE_LENGTH];
	int			m_candleLength;
	int			m_candleNum;
	
	TCHAR		m_comText[MAX_IMEBUFFERLENGTH + 1];			
	int			m_comLength;
	
	int			m_cursorX;

	
	bool		m_bNewLine;
	bool		m_active;

	int			m_MaxLength;
};

#endif 
