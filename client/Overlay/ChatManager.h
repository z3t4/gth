void CHAT_SaveFont( char* filename, int xs, int ys, unsigned char* buf );

#ifndef _INC_STDARG

typedef char * va_list;
#define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap) ( ap = (va_list)0 )

#endif


#define	INTERFACE_ZPOSITION						0.5f
#define	INTERFACE_CHAT_ZPOSITION				0.501f
#define	INTERFACE_ROLLOVER_ZPOSITION			0.502f
#define	INTERFACE_ROLLOVERCHAT_ZPOSITION		0.503f
#define	INTERFACE_MESSAGE_ZPOSITION				0.504f
#define INTERFACE_MESSAGE_BUTTON_ZPOSITION		0.505f
#define INTERFACE_MESSAGECHAT_ZPOSITION			0.506f

#define CHAT_BUFFER_SIZE				2048


#define CHAT_MAXPOSITION				256
#define CHAT_MAXTEXPOSITION				64
#define CHAT_MAXSTRING					1024
#define CHAT_MAXFONTNUMBER				30000


#define CHAT_MAXENGVERTEX				30000
#define	CHAT_MAXHANVERTEX				30000
#define	CHAT_MAXREVERSEVERTEX			1000

#define CHAT_MAXCOPYINDEXBUFFER			10000


#define CHAT_MAXINPUT					20

typedef char chatMessage[CHAT_BUFFER_SIZE];

typedef struct
{
	D3DXVECTOR4 position;
	DWORD color;
	FLOAT tu, tv;
} HANTEXTUREVERTEX;

#define D3DFVF_HANVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


typedef struct
{
	int x;
	int y;
	int dx;
	int dy;
	int type;
	int size;
} chatCopyIndexBuffer_t;


typedef struct
{
	char	str[CHAT_BUFFER_SIZE];			
	int		x[CHAT_MAXPOSITION];		
	int		y[CHAT_MAXPOSITION];
	float	fontSize[CHAT_MAXPOSITION];
	DWORD	fontColor[CHAT_MAXPOSITION];
	int		reverseFlag[CHAT_MAXPOSITION];

	float	zPos[CHAT_MAXPOSITION];
	float	wPos[CHAT_MAXPOSITION];

	
	
	int		renderFlag[CHAT_MAXPOSITION];

	int		nPos;						


	int		tx[CHAT_MAXTEXPOSITION];	
	int		ty[CHAT_MAXTEXPOSITION];
	int		size[CHAT_MAXTEXPOSITION];	
	int		line[CHAT_MAXTEXPOSITION];	

	int		nTPos;						

} chatString_t;


typedef enum
{
	CHAT_HANFONT1,
	CHAT_HANFONT2,
	CHAT_ENGFONT
} CHAT_FONTTYPE;


typedef enum
{
	CHAT_OUTPUT_NORMAL,
	CHAT_OUTPUT_SHADOW,
	CHAT_OUTPUT_BORDER,
	CHAT_OUTPUT_BOLD,
	CHAT_OUTPUT_REVERSE
};

enum
{
	CHAT_INPUT_RETURN_NONE		=	0,
	CHAT_INPUT_RETURN_SKIP,
	CHAT_INPUT_RETURN_OK,
	CHAT_INPUT_RETURN_TAB,
	CHAT_INPUT_RETURN_EXIT
};

enum
{
	CHAT_ARRANGE_NONE		=	0,
	CHAT_ARRANGE_CENTER,
	CHAT_ARRANGE_RIGHT,
	CHAT_ARRANGE_LINEFEED,
	CHAT_ARRANGE_OVERSIZECUT,
};


typedef struct
{
	int tx1;
	int ty1;
	int tx2;
	int ty2;
} GTH_FONTTEXCOORD;

#define CHAT_TOKEN_COLOR		'c'		
#define CHAT_TOKEN_LINE			'n'		
#define CHAT_TOKEN_OUTTYPE		'o'		
#define CHAT_TOKEN_LINKSTART	's'		
#define CHAT_TOKEN_LINKEND		'e'		
#define CHAT_TOKEN_LINKNUMBER	'l'		

#define CHAT_TOKEN_ARRANGETYPE	'a'		

#define CHAT_GLOBAL_FRAME_FLAG		1000

class CChatManager
{
public :
	CChatManager();
	~CChatManager();

public :

	int		m_frameFlag;
	int		m_saveFrameFlag;
	
	DWORD	m_chatColor[10];

	void BeginFrame();			
	void EndFrame();			

	void Update();

	
	void SetChat( int languageType = 0, int inputType = 0 );
	void LoadFont( int languageType );
	void LoadSize( int languageType );
	void CreateFont();
	void InvalidateFont();
	void Release();
	void DPrint( int x, int y, char* str, ... );

	
	char* NextToken();
	char* NextTokenMail();
	int GetTextRectPage( int xs, int ys, char* str, int page, char *pageStr );

	
	void DebugOut( int x, int y, char* str, ... );

	
	void Output( int x, int y, char *str, int reverseFlag = false );

	
	
	void TextOut( int x, int y, char *str, int arrangeType = CHAT_ARRANGE_NONE, int xsize = -1 );

	void DrawText( int x, int y, char *str, int xsize = -1, int lineFeedSize = 13, int arrangeType = CHAT_ARRANGE_NONE );
	void DrawTextMail( int x, int y, char *str, int xsize = -1, int lineFeedSize = 13, int arrangeType = CHAT_ARRANGE_NONE, int inputIdx = -1 );
	int DrawTextRect( int x, int y, char *str, int xsize, int ysize, int numPage );
	int	GetTextRect( int xsize, int lineFeedSize, char *str, int& mx, int& my );
	int	GetTextRectLine( int xsize, int lineFeedSize, char *str );
	void HPrint( int x, int y, char* str, ... );

	
	
	int DrawTextRectLineScroll( int x, int y, char *str, int xsize, int ysize, int numLine, int pointLinkIndex = -1 );
	
	void DrawTextLineScroll( int x, int y, char *str, int xsize = -1, int lineFeedSize = 13, int pointLinkIndex = -1, int arrangeType = CHAT_ARRANGE_NONE );
	int GetTextRectLineScroll( int xs, int ys, char* str, int line, char *lineStr );

	char		*m_pChatManagerTokenBuffer;
	char		*m_pChatManagerTokenBufferEnd;

	
	void SetDebugColor( DWORD color )						{ m_engColor = color; }
	void SetColor( DWORD color )							{ m_hanColor = color; }
	void SetIndexedColor ( int idx )						{ m_hanColor = m_chatColor[idx]; }
	void SetDebugSize( float size )							{ m_engSize = size; }
	void SetSize( float size )								{ m_hanSize = size; }
	void SetZPos( float zpos = 0.0f )	{ m_zPos = zpos; }
	void SetWPos( float wpos = 1.0f )	{ m_wPos = wpos; }

	void SetOutputType( int type )							{ m_outputType = type; }


	int GetChatStrLine( int size, char *str );
	void Render();
	void SetDFontSize( float size ) { m_engSize = size; }
	void SetFontSize( float size ) { m_hanSize = size; }
	void SetDevice( LPDIRECT3DDEVICE8 pD3DDevice ) { m_pd3dDevice = pD3DDevice; }
	void SetFileMng( CFileMng *fileMng )           { m_fileMng = fileMng; }
	
	void SetDFontColor( DWORD color ) { m_engColor = color; }
	void SetFontColor( DWORD color ) { m_hanColor = color; }
	void SetReverseColor( DWORD color ) { m_reverseColor = color; }

	void Restore();
	
	LPDIRECT3DDEVICE8 m_pd3dDevice;

	
	CFileMng       *m_fileMng;     

	int				m_nEngVexCount;
	int				m_texInfo[32][32];

	
	int				m_curIdx;					
	chatMessage		m_inputStr[CHAT_MAXINPUT];
	int				m_csPos[CHAT_MAXINPUT];
	int				m_inputState[CHAT_MAXINPUT];
	int				m_ix[CHAT_MAXINPUT];
	int				m_iy[CHAT_MAXINPUT];
	int				m_inputSize[CHAT_MAXINPUT];
	int				m_inputOutPos[CHAT_MAXINPUT];
	int				m_inputOutSize[CHAT_MAXINPUT];
	int				m_inputOutLength[CHAT_MAXINPUT];		
	int				m_inputLineFeed[CHAT_MAXINPUT];
	int				m_lineLimit[CHAT_MAXINPUT][2];
	
	int				m_inputLastPositionByLine[CHAT_MAXINPUT];

	int				m_inputRenderState;
	int				m_inputType;							

	unsigned int	m_csSaveTime;
	unsigned int	m_csCurTime;
	
	char			m_combi[CHAT_MAXSTRING];

	chatMessage		m_returnStr;

	int				m_clipStartPos;
	chatMessage		m_clipBoard;

	float			m_zPos;
	float			m_wPos;

	bool			bIsActiveCompWnd;						
	LOGFONT			m_logFont;								

	void InitInput();
	void InitInputStr( int idx );
	void AddInputStr( int idx, char* str );

	void SetInput( int x, int y, int bufferSize, int outputSize, int idx, int state = 1, int outputLength = 0, int lineFeed = false );
	void SetInputState( int idx, int state ) { m_inputState[idx] = state; }
	void SetInputOutSize( int idx );

	void SetInputPosition( int idx, int x, int y );
	void SetInputLine( int idx, int minLine, int maxLine );
	int GetInputLine( int idx );
	int GetViewLine( char *str, int length );
	int GetCursorLine( int inputIdx );

	int GetCharState( char *str, int pos );
	void ChangeInputMessage( int idx );
	
	int GetCurrentInputIndex()		{ return m_curIdx; }
	
	char* GetInputStr( int idx ) { return( m_inputStr[idx] ); }

#if defined _GTH_KOREAN_VERSION || _GTH_ENGLISH_VERSION		
	void AddCombination();
#endif
	void AddChar( char ch );

#ifdef _GTH_TERRA_VERSION
	void DeleteCombination();			
	void InsertString( char *str );		
	void SetCombi( HWND hwnd );			
#endif


#if defined (_GTH_CHINESE_VERSION) || (_GTH_TAIWAN_VERSION)	|| (_GTH_MALAYSIA_CHN_VERSION)
	void InsertString( char *str );
	void SetCombi( HWND hwnd, HIMC hIMC );
	bool IsInnerStatusWind( );
#endif

	void BackSpace();
	void Delete();

	void Right();
	void Left();
	void Up();
	void Down();
	void Paste();
	void Copy();
	
	
	void UpdateLastPositionByLine();

	void SetRenderFlag( int flag )	{ m_renderFlag = flag; }

	void RenderInput();
	void RenderInputFrame( int inputIdx, bool displayCursor = true );
	void GetInputRenderPosition();
	void DrawInputWindow();
	void SetReverseSize( int size ) { m_reverseSize = size; }

	void DisableDebugOutput()	{ m_debugOutputFlag = 0; }
	void EnableDebugOutput()	{ m_debugOutputFlag = 1; }
	int	 DebugOutputState()		{ return( m_debugOutputFlag ); }
	
	int GetString( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
	int GetChatStrSize( char *str );
	int GetChatStrSize2( int xs, int ys, char *str );
	
	int GetChatStrSizeChat( char *str );

	int	IsVaildString( char *str );

	int FindTexIdx( wchar_t uniCode );		
	void SetCursorFromMousePos( int mx, int my );

	
	inline GTH_FONTTEXCOORD*	GetHanTexCoord(int index)	{	return &m_hanTexCoord[index];}

private:

	void SetRenderState();

	void MakeChatTexture();
	int CopyFont( BYTE *src, int x, int y, BYTE* dst, int dx, int dy, int xs, int ys );
	void AddStr( int x, int y, int idx, int reverseFlag = false );
	int MakeStr( int idx );
	int BuildStr( int &tx1, int &ty1, int &tx2, int &ty2, char *str );
	int FindEmptyColumn( int &row, int &column );
	void UpdateEmptyTexture();

	void MakeReverse( float x, float y, float xs, float ys, float zpos, float wpos, int reverseSizeFlag );
	
	void SetChatTexture( int row, int column );

	
	int TextOut( int x, int y, char *str, int xsize, int lineFeedSize, int &cx, int &cy, int sizeFlag = false, int arrangeType = CHAT_ARRANGE_NONE );
	int TextOutChat( int x, int y, char *str, int xsize, int lineFeedSize, int &cx, int &cy, int sizeFlag = false, int arrangeType = CHAT_ARRANGE_NONE, int inputIdx = -1, int baseLine = 0 );

	void GetCursorPosition( int &x, int &y );
	bool IsVisibleLine( int inputIdx, int line );

	chatCopyIndexBuffer_t m_copyIndexBuffer[CHAT_MAXCOPYINDEXBUFFER];
	int				m_nCopyIndexBuffer;

	chatString_t	m_str[CHAT_MAXSTRING];
	int				m_nStr;

	int				m_renderFlag;

	int				m_nHanVexCount;
	float			m_hanSize;
	int				m_outputType;

	int				m_nEngStr;
	float			m_engSize;

	DWORD			m_hanColor;
	DWORD			m_engColor;
	DWORD			m_reverseColor;

	GTH_FONTTEXCOORD	m_engTexCoord[CHAT_MAXFONTNUMBER];
	GTH_FONTTEXCOORD	m_hanTexCoord[CHAT_MAXFONTNUMBER];
	DWORD				m_nEngTexCoord;
	DWORD				m_nHanTexCoord;

	BYTE				*m_engFontBuffer;
	BYTE				*m_hanFontBuffer;


	HANTEXTUREVERTEX *m_pEngVertices;
	HANTEXTUREVERTEX *m_pHanVertices;
	HANTEXTUREVERTEX *m_pReverse;

	CD3DBuffer		*m_hanVex;
	CD3DBuffer		*m_engVex;
	CD3DBuffer		*m_reverseVex;

	int				m_numReverse;
	int				m_reverseSize;

	int				m_debugOutputFlag;

	int				m_inputReturnFlag;

	LPDIRECT3DTEXTURE8	m_hanTex;
	LPDIRECT3DTEXTURE8	m_engTex;

};
