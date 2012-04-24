#include <afxwin.h>
#include "resource.h"
#include "input.h"

extern HINSTANCE g_hInst;



void mouse_t::ChangeCursor( int type )
{

	
	oldCursorState = cursorState;

	return;

	switch( type )
	{
	case MOUSE_CURSOR_NULL :
		cursorState = MOUSE_CURSOR_NULL;
		SetCursor( NULL );
		break;
	case MOUSE_CURSOR_POINTER :
		cursorState = MOUSE_CURSOR_POINTER;
		hCur = LoadCursor( g_hInst, MAKEINTRESOURCE( IDC_POINTER ) );
		SetCursor( hCur );
		break;
	case MOUSE_CURSOR_ATTACK :
		cursorState = MOUSE_CURSOR_ATTACK;
		hCur = LoadCursor( g_hInst, MAKEINTRESOURCE( IDC_POINTER_ATTACK ) );
		SetCursor( hCur );
		break;
	case MOUSE_CURSOR_CHAT :
		cursorState = MOUSE_CURSOR_CHAT;
		hCur = LoadCursor( g_hInst, MAKEINTRESOURCE( IDC_POINTER_CHAT ) );
		SetCursor( hCur );
		break;
	case MOUSE_CURSOR_REPAIR :
		cursorState = MOUSE_CURSOR_REPAIR;
		hCur = LoadCursor( g_hInst, MAKEINTRESOURCE( IDC_POINTER_REPAIR ) );
		SetCursor( hCur );
		break;

	}











}

void mouse_t::Show()
{
	ChangeCursor( MOUSE_CURSOR_POINTER );
}

void mouse_t::Hide()
{
	ChangeCursor( MOUSE_CURSOR_NULL );
}

void mouse_t::Resize( int width , int height ) 
{
	mid.x = width >> 1;
	mid.y = height >> 1;
}

void mouse_t::GetCursorPos( POINT& p ) 
{
	if( middle_button || forcedMode )
	{
		if( currMode != SCROLL_MODE && !forcedMode )
		{
			oldMode = currMode;
			currMode = SCROLL_MODE;
			Center();
			p.x = 0;
			p.y = 0;
		}
		else
		{
			p.x = pos.x - mid.x;
			p.y = pos.y - mid.y;
			if( p.x || p.y )
				Center();
		}
	}
	else
	{
		if( currMode == SCROLL_MODE && !forcedMode )
		{
			oldMode = currMode;
			currMode = NORMAL_MODE;
			ClientToScreen( hWnd , &currPos );
			SetCursorPos( currPos.x , currPos.y );
		}
		else
		{
			prevPos = currPos;
			currPos = pos;
			p = currPos;
		}
	}
}

void mouse_t::GetDelta( POINT& p ) 
{
	p.x = currPos.x - prevPos.x;
	p.y = currPos.y - prevPos.y;
}
		
void mouse_t::Center() 
{ 



}

int mouse_t::GetWheelDelta()
{
	if( isWheeled && wheelDelta != 0 )
	{
		isWheeled = false;
		return wheelDelta;
	}
	
	return 0;
}

int mouse_t::GetButtonState( mouseButton_t state, int type )
{
	int curState = 0;
	curState = left_button | ( right_button << 1 ) | ( left_dbclick << 2 ) | ( right_dbclick << 3 );

	if( !type )
	{
		if( left_button && ( MOUSE_LEFT_BUTTON & state ) ) left_button = 0;
		if( right_button && ( MOUSE_RIGHT_BUTTON & state ) ) right_button = 0;
		if( left_dbclick && ( MOUSE_LEFT_DBCLICK & state ) ) left_dbclick = 0;
		if( right_dbclick && ( MOUSE_RIGHT_DBCLICK & state ) ) right_dbclick = 0;
	}

	return( curState & state );
}

int mouse_t::GetLeftClicked()
{
	if( !left_button_clicked ) return( false );
	else if( left_button == 0 )
	{
		left_button_clicked = 0;
		return( true );
	}
	return( false );
}


int mouse_t::GetRightClicked()
{
	if( !right_button_clicked ) return( false );
	else if( right_button == 0 )
	{
		right_button_clicked = 0;
		return( true );
	}
	return( false );
}
