#include <afxwin.h>
#include "global_def.h"

CInputMng::CInputMng()
{
    m_pKeyboard = NULL;
	m_pMouse = NULL;
    m_pDI = NULL;

	ZeroMemory( &m_diks, sizeof(m_diks) );
	ZeroMemory( &m_oldDiks, sizeof(m_oldDiks) );
	
	ZeroMemory( &m_dims, sizeof(m_dims) );
	ZeroMemory( &m_oldDims, sizeof(m_dims) );

	ZeroMemory( &m_mouseDBClk, sizeof(m_mouseDBClk) );

	m_mouseMode = INPUT_MOUSE_NORMAL_MODE;

	m_mouseShowFlag = true;

	m_mouseCursorType = -1;
	m_mouseCursorID = -1;

	
	m_isWheeled = false;
	m_wheelDelta = 0;
	
	
	m_mouseEnable = true;
	m_keyEnable = true;

}

CInputMng::~CInputMng()
{
}

void CInputMng::FreeDevice()
{
   SAFE_RELEASE( m_pDI );
}

int CInputMng::CreateDevice()
{
	HRESULT hr;

	
	if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
										 IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
		return( 0 );

	return( 1 );
}

void CInputMng::FreeKeyDevice()
{
    
    
    if( m_pKeyboard ) 
        m_pKeyboard->Unacquire();
    
    
    SAFE_RELEASE( m_pKeyboard );
}

int	CInputMng::CreateKeyDevice( int dwCoopFlags )
{
	HRESULT hr;
	
    FreeKeyDevice();

    if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
        return( 0 );

    
    
    
    
    
    
    
    if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
        return( 0 );
    
    
    
    
    hr = m_pKeyboard->SetCooperativeLevel( g_pApp->m_hWnd, dwCoopFlags );
    if( FAILED(hr) ) return( 0 );

	
























    
    m_pKeyboard->Acquire();
	
	return( 1 );
}

int CInputMng::ReadKeyImmediateData()
{
    HRESULT hr;

    if( NULL == m_pKeyboard ) return( 1 );
	
    
    ZeroMemory( &m_diks, sizeof(m_diks) );
    hr = m_pKeyboard->GetDeviceState( sizeof(m_diks), &m_diks );
    
	if( FAILED(hr) ) 
    {
        
        
        
        
        
        
        hr = m_pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = m_pKeyboard->Acquire();

        
        if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) 
		{
			ZeroMemory( &m_diks, sizeof(m_diks) );
			ZeroMemory( &m_oldDiks, sizeof(m_oldDiks) );
		}

        
        
        
        return( 1 );
    }

	return( 1 );
}

int CInputMng::UpdateKeyData()
{
	memcpy( &m_oldDiks, &m_diks, sizeof( m_diks ) );
	return( 1 );
}

int CInputMng::KeyState( DWORD element, int type )
{
	
	if(!m_keyEnable) return( 0 );
	
	switch( type )
	{
	case INPUT_KEY_NONE :
		if( !( m_diks[element] & 0x80 ) && !( m_oldDiks[element] & 0x80 ) ) 
		{
			return( 1 );
		}
		else return( 0 );
		break;

	case INPUT_KEY_DOWN :
		if( ( m_diks[element] & 0x80 ) && !( m_oldDiks[element] & 0x80 ) ) 
		{
			return( 1 );
		}
		else return( 0 );
		break;

	case INPUT_KEY_UP :
		if( !( m_diks[element] & 0x80 ) && ( m_oldDiks[element] & 0x80 ) ) 
		{
			return( 1 );
		}
		else return( 0 );
		break;

	case INPUT_KEY_KEEP :
		if( ( m_diks[element] & 0x80 ) && ( m_oldDiks[element] & 0x80 ) ) return( 1 );
		else return( 0 );
		break;

	case INPUT_KEY_EVENT :
		if( ( m_diks[element] & 0x80 ) || ( m_oldDiks[element] & 0x80 ) ) return( 1 );
		else return( 0 );
		break;
	}
	return( 0 );
}

void CInputMng::SetKeyState( DWORD element, int newFlag, int oldFlag )
{
	m_diks[element] = newFlag;
	m_oldDiks[element] = oldFlag;
}

int CInputMng::KeyNone( DWORD element )
{
	return( KeyState( element, INPUT_KEY_NONE ) );
}

int CInputMng::KeyDown( DWORD element )
{
	return( KeyState( element, INPUT_KEY_DOWN ) );
}

int CInputMng::KeyUp( DWORD element )
{
	return( KeyState( element, INPUT_KEY_UP ) );
}

int CInputMng::KeyKeep( DWORD element )
{
	return( KeyState( element, INPUT_KEY_KEEP ) );
}

int CInputMng::KeyEvent( DWORD element )
{
	return( KeyState( element, INPUT_KEY_EVENT ) );
}

void CInputMng::FreeMouseDevice()
{	
	
	
	if( m_pMouse ) 
		m_pMouse->Unacquire();
    
	
	SAFE_RELEASE( m_pMouse );
}

int CInputMng::CreateMouseDevice( int dwCoopFlags )
{
	HRESULT hr;

    FreeMouseDevice();
    
    
    if( FAILED( hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL ) ) )
        return( 0 );
    
    
    
    
    
    
    
    
    if( FAILED( hr = m_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
        return( 0 );
    
    
    
    
    hr = m_pMouse->SetCooperativeLevel( g_pApp->m_hWnd, dwCoopFlags );

    if( FAILED(hr) )
        return( 0 );

    
	m_pMouse->Acquire();

    return( 1 );
}






int CInputMng::ReadMouseImmediateData()
{
    HRESULT       hr;
    
    
    ZeroMemory( &m_dims, sizeof( m_dims ) );

    hr = m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_dims );

    if( FAILED(hr) ) 
    {
        
        
        
        
        
        
        hr = m_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = m_pMouse->Acquire();

		
        if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) 
		{
			ZeroMemory( &m_dims, sizeof(m_dims) );
			ZeroMemory( &m_oldDims, sizeof(m_dims) );
			ZeroMemory( &m_mouseDBClk, sizeof(m_mouseDBClk) );
		}
		
        
        
        
        return( 1 ); 
    }
        
    return( 1 );
}

int CInputMng::UpdateMouseData()
{
	memcpy( &m_oldDims, &m_dims, sizeof( m_dims ) );
	return( 1 );
}

bool CInputMng::Handle_messages( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
	if( msg == WM_MOUSEMOVE ) 
	{
        ::GetCursorPos( &m_mousePos );
		g_ifMng->m_pd3dDevice->SetCursorPosition( m_mousePos.x, m_mousePos.y, 0L );
        ::ScreenToClient( hwnd, &m_mousePos );
	}
    else
	{
		switch( msg ) 
		{
		case WM_LBUTTONDBLCLK :
			m_mouseDBClk[0] = true;
			ShowLogInfo( "WM_LBUTTONDBLCLK " );
			break;
		case WM_RBUTTONDBLCLK :
			m_mouseDBClk[1] = true;
			ShowLogInfo( "WM_RBUTTONDBLCLK " );
			break;

		case WM_MOUSEWHEEL:
			m_isWheeled = true;
            m_wheelDelta = (short)HIWORD(wp);
            ShowLogInfo( "WM_MOUSEWHEEL " );
            break;

       case WM_SETCURSOR:
		    SetCursor( NULL );
			g_ifMng->m_pd3dDevice->ShowCursor( m_mouseShowFlag );
			return true; 
			break;

        default:
            return false;
		}
    }

    return true;
}

void CInputMng::ChangeCursor( int cursorID, int type, int data )
{
	m_mouseCursorData = data;
	if( m_mouseCursorID == cursorID && m_mouseCursorType == type ) return;

	g_ifMng->m_cursor->ChangeCursor( cursorID, type );

	m_mouseCursorID = cursorID;
	m_mouseCursorType = type;
}

int CInputMng::GetCursorState( int cursorID, int type )
{
	if( cursorID < 0 )
	{
		if( type == m_mouseCursorType ) return( true );
		else return( false );
	}
	if( m_mouseCursorID == cursorID && type == m_mouseCursorType ) return( true );
	return( false );
}

void CInputMng::Show()
{
	m_mouseShowFlag = true;
	g_ifMng->m_pd3dDevice->ShowCursor( m_mouseShowFlag );
}

void CInputMng::Hide()
{
	m_mouseShowFlag = false;
	g_ifMng->m_pd3dDevice->ShowCursor( m_mouseShowFlag );
}

void CInputMng::Resize( int width , int height ) 
{
	m_mouseMidPos.x = width >> 1;
	m_mouseMidPos.y = height >> 1;
}

void CInputMng::GetCursorPos( POINT& p ) 
{
	if( ( GetRBDown() || GetRBKeep() ) && m_mouseMode != INPUT_MOUSE_INTERFACE_MODE)
	{
		if( m_mouseMode != INPUT_MOUSE_SCROLL_MODE )
		{
			m_mouseMode = INPUT_MOUSE_SCROLL_MODE;
			Center();
			p.x = 0;
			p.y = 0;
			Hide();
		}
		else
		{
			p.x = m_mousePos.x - m_mouseMidPos.x;
			p.y = m_mousePos.y - m_mouseMidPos.y;
			if( p.x || p.y ) Center();
		}
	}
	else
	{
		if( m_mouseMode == INPUT_MOUSE_SCROLL_MODE )
		{
			m_mouseMode = INPUT_MOUSE_NORMAL_MODE;
			Show();
			ClientToScreen( m_hwnd , &m_mouseCurrPos );
			g_ifMng->m_pd3dDevice->SetCursorPosition( m_mouseCurrPos.x, m_mouseCurrPos.y, 0L );
			SetCursorPos( m_mouseCurrPos.x, m_mouseCurrPos.y );
		}
		else
		{
			m_mousePrevPos = m_mouseCurrPos;
			m_mouseCurrPos = m_mousePos;
			p = m_mouseCurrPos;
		}
	}
}

void CInputMng::GetDelta( POINT& p ) 
{
	p.x = m_mouseCurrPos.x - m_mousePrevPos.x;
	p.y = m_mouseCurrPos.y - m_mousePrevPos.y;
}

void CInputMng::Center() 
{ 
	POINT p = m_mouseMidPos;
	ClientToScreen( m_hwnd , &p );
	g_ifMng->m_pd3dDevice->SetCursorPosition( p.x, p.y, 0L );
	SetCursorPos( p.x, p.y );
}

int CInputMng::GetWheelDelta()
{
	if( m_isWheeled && m_wheelDelta != 0 )
	{
		m_isWheeled = false;
		return m_wheelDelta;
	}
	return 0;
}

int CInputMng::GetButtonState( int btnType, int type )
{
	
	if(!m_mouseEnable)	return( 0 );

	switch( type )
	{
	case INPUT_MOUSE_NONE :
		if( !( m_dims.rgbButtons[btnType] & 0x80 ) && !( m_oldDims.rgbButtons[btnType] & 0x80 ) ) return( 1 );
		else return( 0 );
		break;
	case INPUT_MOUSE_DOWN :
		if( ( m_dims.rgbButtons[btnType] & 0x80 ) && !( m_oldDims.rgbButtons[btnType] & 0x80 ) ) 
			return( 1 );
		else return( 0 ); 
		break;
	case INPUT_MOUSE_UP :
		if( !( m_dims.rgbButtons[btnType] & 0x80 ) && ( m_oldDims.rgbButtons[btnType] & 0x80 ) ) return( 1 );
		else return( 0 );
		break;
	case INPUT_MOUSE_KEEP :
		if( ( m_dims.rgbButtons[btnType] & 0x80 ) && ( m_oldDims.rgbButtons[btnType] & 0x80 ) ) return( 1 );
		else return( 0 );
		break;
	case INPUT_MOUSE_DBCLK :
		if( m_mouseDBClk[btnType] ) 
		{
			m_mouseDBClk[btnType] = false;
			return( 1 );
		}
		else return( 0 );
		break;
	}
	return( 0 );
}

int CInputMng::GetLBNone()
{
	return( GetButtonState( 0, INPUT_MOUSE_NONE ) );
}

int CInputMng::GetRBNone()
{
	return( GetButtonState( 1, INPUT_MOUSE_NONE ) );
}

int CInputMng::GetMBNone()
{
	return( GetButtonState( 2, INPUT_MOUSE_NONE ) );
}

int CInputMng::GetLBDown()
{
	return( GetButtonState( 0, INPUT_MOUSE_DOWN ) );
}

int CInputMng::GetRBDown()
{
	return( GetButtonState( 1, INPUT_MOUSE_DOWN ) );
}

int CInputMng::GetMBDown()
{
	return( GetButtonState( 2, INPUT_MOUSE_DOWN ) );
}

int CInputMng::GetLBKeep()
{
	return( GetButtonState( 0, INPUT_MOUSE_KEEP ) );
}

int CInputMng::GetRBKeep()
{
	return( GetButtonState( 1, INPUT_MOUSE_KEEP ) );
}

int CInputMng::GetMBKeep()
{
	return( GetButtonState( 2, INPUT_MOUSE_KEEP ) );
}


int CInputMng::GetLBUp()
{
	return( GetButtonState( 0, INPUT_MOUSE_UP ) );
}

int CInputMng::GetRBUp()
{
	return( GetButtonState( 1, INPUT_MOUSE_UP ) );
}

int CInputMng::GetMBUp()
{
	return( GetButtonState( 2, INPUT_MOUSE_UP ) );
}

int CInputMng::GetLBDBClk()
{
	return( GetButtonState( 0, INPUT_MOUSE_DBCLK ) );
}

int CInputMng::GetRBDBClk()
{
	return( GetButtonState( 1, INPUT_MOUSE_DBCLK ) );
}

int CInputMng::GetMBDBClk()
{
	return( GetButtonState( 2, INPUT_MOUSE_DBCLK ) );
}

 
 
 
 
 
void CInputMng::InvalidateDeviceObject()
{
	m_mouseCursorID = -1;
}