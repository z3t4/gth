enum
{
	INPUT_KEY_NONE		= 0,
	INPUT_KEY_DOWN,
	INPUT_KEY_UP,
	INPUT_KEY_KEEP,
	INPUT_KEY_EVENT,

};

enum
{
	INPUT_MOUSE_NONE	= 0,
	INPUT_MOUSE_DOWN,
	INPUT_MOUSE_UP,
	INPUT_MOUSE_DBCLK,
	INPUT_MOUSE_KEEP,
};

enum
{
	INPUT_MOUSE_NORMAL_MODE = 0,
	INPUT_MOUSE_SCROLL_MODE,
	INPUT_MOUSE_WHEEL_MODE,
	INPUT_MOUSE_INTERFACE_MODE
};

enum
{
	CURSOR_POINTER	=	0,
	CURSOR_ATTACK,
	CURSOR_CHAT,
	CURSOR_REPAIR,
	CURSOR_NULL,
};


enum
{
	MOUSE_CURSOR_TYPE		=	0,
	ITEM_CURSOR_TYPE,
	SKILL_CURSOR_TYPE,
	ACTION_CURSOR_TYPE,
};


#define NUM_MOUSE_BUTTON	5

class CInputMng
{
public :
	
	CInputMng();
	~CInputMng();

	

	
	LPDIRECTINPUT8				m_pDI;
	LPDIRECTINPUTDEVICE8		m_pKeyboard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

	BYTE						m_diks[256];
	BYTE						m_oldDiks[256];

	
	HWND						m_hwnd;
	POINT						m_mousePos;
	POINT						m_mouseCurrPos;
	POINT						m_mousePrevPos;
	POINT						m_mouseMidPos;

	DIMOUSESTATE2				m_dims;
	DIMOUSESTATE2				m_oldDims;

	int							m_mouseDBClk[NUM_MOUSE_BUTTON];
	int							m_isWheeled;
	int							m_wheelDelta;

	int							m_mouseMode;

	int							m_mouseShowFlag;

	int							m_mouseCursorType;
	int							m_mouseCursorID;
	int							m_mouseCursorData;

	
	
	bool						m_mouseEnable;
	bool						m_keyEnable;
	void						MouseEnable( int state = true )				{ m_mouseEnable = state; }
	void						KeyEnable( int state = true )				{ m_keyEnable = state; }
	
	
	void FreeDevice();
	int CreateDevice();

	
	void FreeKeyDevice();
	int	CreateKeyDevice( int dwCoopFlags = DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY );
	int ReadKeyImmediateData();
	int UpdateKeyData();

	int KeyState( DWORD element, int type );
	void SetKeyState( DWORD element, int newType = 0, int oldType = 0 );

	int KeyNone( DWORD element );
	int KeyDown( DWORD element );
	int KeyUp( DWORD element );
	int KeyKeep( DWORD element );
	int KeyEvent( DWORD element );

	
	void FreeMouseDevice();
	int	CreateMouseDevice( int dwCoopFlags = DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY );
	int ReadMouseImmediateData();
	int UpdateMouseData();

	void SetHWND( HWND hwnd ) { m_hwnd = hwnd; }

	bool Handle_messages( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp );

	void ChangeCursor( int cursorID = 0, int type = MOUSE_CURSOR_TYPE, int data = -1 );
	int GetCursorState( int cursorID, int type = MOUSE_CURSOR_TYPE );
	int GetCursorData() { return( m_mouseCursorData ); }

	void Show();
	void Hide();
	void Resize( int width , int height );

	int GetPosX()   { return m_mouseCurrPos.x; }
	int GetPosY()   { return m_mouseCurrPos.y; }
	int GetDeltaX() { return m_mouseCurrPos.x - m_mousePrevPos.x; }
	int GetDeltaY() { return m_mouseCurrPos.y - m_mousePrevPos.y; }

	int GetMouseMode() { return m_mouseMode; }
	void SetMouseMode( int mode ) { m_mouseMode = mode; }

	void GetCursorPos( POINT& p );
	void GetDelta( POINT& p );
	void Center();

	int GetWheelDelta();
	int GetButtonState( int btnType, int type );

	int GetLBDown();
	int GetRBDown();
	int GetMBDown();

	int GetLBKeep();
	int GetRBKeep();
	int GetMBKeep();

	int GetLBUp();
	int GetRBUp();
	int GetMBUp();
	
	int GetLBDBClk();
	int GetRBDBClk();
	int GetMBDBClk();
	
	int GetLBNone();
	int GetRBNone();
	int GetMBNone();

	 
	void InvalidateDeviceObject();
};



