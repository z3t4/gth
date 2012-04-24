typedef enum
{
	NORMAL_MODE = 0,
	SCROLL_MODE ,
	WHEEL_MODE ,
	INTERFACE_MODE
} mousemode_t;


typedef enum
{
	MOUSE_LEFT_BUTTON	=	1 << 0,
	MOUSE_RIGHT_BUTTON	=	1 << 1,
	MOUSE_LEFT_DBCLICK	=	1 << 2,
	MOUSE_RIGHT_DBCLICK	=	1 << 3
} mouseButton_t;


typedef enum
{
	MOUSE_CURSOR_POINTER	=	0,
	MOUSE_CURSOR_ATTACK,
	MOUSE_CURSOR_CHAT,
	MOUSE_CURSOR_REPAIR,
	MOUSE_CURSOR_NULL,

	NUM_MOUSE_CURSOR_TYPE

} mouseCursorType_t;

typedef struct mouse_s
{
	mouse_s() : left_button(0), right_button(0) , middle_button(0)
	{ mid.x=mid.y=0; currPos=mid; currPos=mid; currMode = NORMAL_MODE; show = true; isWheeled = false; 
	left_dbclick = 0; right_dbclick = 0;
	left_button_clicked = 0; right_button_clicked = 0; check_clicked = false;
	forcedMode = false; cursorState = MOUSE_CURSOR_POINTER; oldCursorState = -1;
	}

	POINT currPos;
	POINT prevPos;
	POINT pos;
	POINT mid;

	int left_button;
	int right_button;

	int left_button_clicked;
	int right_button_clicked;
	int check_clicked;

	
	int left_dbclick;
	int right_dbclick;

	int middle_button;
	int wheelDelta;

	int isWheeled;
	int show;

	int forcedMode;

	mousemode_t currMode;
	mousemode_t oldMode;
	HWND hWnd;
	HCURSOR hCur;
	int cursorState;
	int oldCursorState;

	void Resize( int width , int height );
	void GetCursorPos( POINT& p );
	void GetDelta( POINT& p );

	void ChangeCursor( int type );

	int GetPosX()   { return currPos.x; }
	int GetPosY()   { return currPos.y; }
	int GetDeltaX() { return currPos.x - prevPos.x; }
	int GetDeltaY() { return currPos.y - prevPos.y; }

	int GetButtonState( mouseButton_t state, int type = 0 );

	int GetLeftClicked();
	int GetRightClicked();

	int  GetWheelDelta();
    void Center();
    void Show();
    void Hide();
} mouse_t;

typedef struct input_s 
{
	int		keys[256];

	char	hanKeys[3];
	int		sysKey;

	mouse_t mouse;

	input_s() { reset(); }
	void reset() 
	{
		ZeroMemory( &keys , sizeof( keys ) );
		ZeroMemory( &hanKeys, sizeof( hanKeys ) );
		sysKey = 0;
		mouse = mouse_t();
	}

	

	void SetHWND( HWND hWnd ) { mouse.hWnd = hWnd; }
	mousemode_t GetMouseMode() { return mouse.currMode;	}
	void SetMouseMode( mousemode_t mode ) { mouse.oldMode = mouse.currMode; mouse.currMode = mode; }

} input_t;