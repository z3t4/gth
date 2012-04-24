



#include "../global_def.h"
#include "IFCommon.h"





char *g_itemCursorDataFilename[] = 
{
	"itemIcon.dat",
	"itemIcon2.dat",
	"itemIcon3.dat",
	"itemIcon4.dat",	
	NULL
};

char *g_skillCursorDataFilename[] = 
{
	"skillIcon.dat",
	"skillIcon2.dat",
	NULL
};

CIFMouseCursor::CIFMouseCursor()
{
	memset ( m_mouseCursorBuffer, 0, sizeof(m_mouseCursorBuffer) );
	memset ( m_itemCursorBuffer, 0, sizeof(m_itemCursorBuffer) );
	memset ( m_skillCursorBuffer, 0, sizeof(m_skillCursorBuffer) );
}

CIFMouseCursor::~CIFMouseCursor()
{
	int i;

	for( i = 0; i < MAX_COUNT_CURSOR_BUFFER; i ++ )
	{
		SAFE_DELETE_ARRAY ( m_mouseCursorBuffer[i].buf );
		SAFE_DELETE_ARRAY ( m_itemCursorBuffer[i].buf );
		SAFE_DELETE_ARRAY ( m_skillCursorBuffer[i].buf );
	}
}

void CIFMouseCursor::CopyMouseCursor( IFCommonTexture_t *src, int x, int y, IFMouseCursorBuffer_t* dst )
{
	unsigned int sourad;

	BYTE* sour = src->buffer;
	BYTE* dest = dst->buf;

	sourad = y * src->width * 4 + x * 4;
	sour += sourad;

	for( int i = 0; i < 32; i ++ )
	{
		memcpy( dest, sour, 32 * 4 );

		dest += 32 * 4;
		sour += src->width * 4;
	}
}

void CIFMouseCursor::MakeCursorArrow( BYTE* src, DWORD color )
{
	int x, y;
	DWORD *pSrcWord = (DWORD *)src;
    for( y = 0; y < 32; y++ )
    {
        for( x = 0; x < 32; x++ )
        {
            if( x < 12 && y < 5 )
            {
                
                
                
                
                

                const WORD wMask[5] = { 0xf800, 0xf000, 0xe000, 0xd000, 0x8800 };
                if( wMask[y] & (1 << (15 - x)) )
                {
                    pSrcWord[32 * y + x] = color;
                }
            }
        }
	}
}

int CIFMouseCursor::MakeCursor()
{
	MakeMouseCursor();
	MakeItemCursor();
	MakeSkillCursor();

	return 1;
}

int CIFMouseCursor::MakeMouseCursor( int numOfCursor )
{
	IFCommonTexture_t *mouseCursor;
	int bufferSize, i, j, n;

	mouseCursor = LoadTARGA( "mouseicon.dat" );

	bufferSize = mouseCursor->width * mouseCursor->height * 4;

	n = 0;
	for( i = 0; i < mouseCursor->height / 32; i ++ )
	{
		for( j = 0; j < mouseCursor->width / 32; j ++ )
		{
			m_mouseCursorBuffer[n].width = 32;
			m_mouseCursorBuffer[n].height = 32;
			m_mouseCursorBuffer[n].buf = new BYTE[32 * 32 * 4];

			CopyMouseCursor( mouseCursor, j * 32, i * 32, &m_mouseCursorBuffer[n] );

			n ++;
			if( numOfCursor > 0 ) 
			{
				if( numOfCursor <= n ) break;
			}
		}
	}

	GTH_SAFE_FREE ( mouseCursor->buffer );
	GTH_SAFE_FREE ( mouseCursor );
    
	return 1;
}

int CIFMouseCursor::MakeItemCursor( int numOfCursor )
{
	IFCommonTexture_t *mouseCursor;
	int bufferSize, i, j, n, cursorDataIdx = 0;

	n = 0;

	
	while( g_itemCursorDataFilename[cursorDataIdx] != NULL )
	{
		mouseCursor = LoadTARGA( g_itemCursorDataFilename[cursorDataIdx] );

		bufferSize = mouseCursor->width * mouseCursor->height * 4;
		for( i = 0; i < mouseCursor->height / 32; i ++ )
		{
			for( j = 0; j < mouseCursor->width / 32; j ++ )
			{
				m_itemCursorBuffer[n].width = 32;
				m_itemCursorBuffer[n].height = 32;
				m_itemCursorBuffer[n].buf = new BYTE[32 * 32 * 4];
				
				CopyMouseCursor( mouseCursor, j * 32, i * 32, &m_itemCursorBuffer[n] );
				MakeCursorArrow( m_itemCursorBuffer[n].buf, 0xffffd700 );
				
				n ++;
				if( numOfCursor > 0 ) 
				{
					if( numOfCursor <= n ) break;
				}
			}
		}
		
		GTH_SAFE_FREE ( mouseCursor->buffer );
		GTH_SAFE_FREE ( mouseCursor );

		cursorDataIdx ++;
	}
    
	return 1;
}

int CIFMouseCursor::MakeSkillCursor( int numOfCursor )
{
	IFCommonTexture_t *mouseCursor;
	int bufferSize, i, j, n, cursorDataIdx = 0;

	n = 0;

	while( g_skillCursorDataFilename[cursorDataIdx] != NULL )
	{
		mouseCursor = LoadTARGA( g_skillCursorDataFilename[cursorDataIdx] );
		
		bufferSize = mouseCursor->width * mouseCursor->height * 4;
		
		for( i = 0; i < mouseCursor->height / 32; i ++ )
		{
			for( j = 0; j < mouseCursor->width / 32; j ++ )
			{
				m_skillCursorBuffer[n].width = 32;
				m_skillCursorBuffer[n].height = 32;
				m_skillCursorBuffer[n].buf = new BYTE[32 * 32 * 4];
				
				CopyMouseCursor( mouseCursor, j * 32, i * 32, &m_skillCursorBuffer[n] );
				MakeCursorArrow( m_skillCursorBuffer[n].buf, 0xffffd700 );
				
				n ++;
				if( numOfCursor > 0 ) 
				{
					if( numOfCursor <= n ) break;
				}
			}
		}
		
		GTH_SAFE_FREE ( mouseCursor->buffer );
		GTH_SAFE_FREE ( mouseCursor );	

		cursorDataIdx ++;
	}

	return 1;
}

int	CIFMouseCursor::ChangeCursor( int cursorID, int type )
{
	HRESULT hr;
    LPDIRECT3DSURFACE8 pCursorBitmap = NULL;
	BYTE* pCursorBuffer = NULL;
	BYTE* pMouseBuffer;

	DWORD dwSize;

	switch( type )
	{
	case MOUSE_CURSOR_TYPE :
		pMouseBuffer = m_mouseCursorBuffer[cursorID].buf;
		break;
	case ITEM_CURSOR_TYPE :
		pMouseBuffer = m_itemCursorBuffer[cursorID].buf;
		break;
	case SKILL_CURSOR_TYPE : case ACTION_CURSOR_TYPE :
		pMouseBuffer = m_skillCursorBuffer[cursorID].buf;
		break;
	}

	dwSize = 32 * 32 * 4;

    
    m_pd3dDevice->CreateImageSurface( 32, 32, D3DFMT_A8R8G8B8, &pCursorBitmap );

    D3DLOCKED_RECT lr;
    pCursorBitmap->LockRect( &lr, NULL, 0 );
    pCursorBuffer = (BYTE *)lr.pBits;
	memcpy( pCursorBuffer, pMouseBuffer, dwSize );
    pCursorBitmap->UnlockRect();

    if( FAILED( hr = m_pd3dDevice->SetCursorProperties( 0, 0, pCursorBitmap ) ) )
    {
        goto CHANGE_CURSOR_END ;
    }

    hr = S_OK;

CHANGE_CURSOR_END :

    SAFE_RELEASE( pCursorBitmap );

    if( FAILED( hr ) ) return 0;
	return 1;
}

IFCommonTexture_t* CIFMouseCursor::LoadTARGA( char *filename )
{
	unsigned char ucharBad, imageTypeCode, bitCount, colormapSize;
	short int sintBad, width, height;
	long imageSize;
	int colorMode;
	int tempColor;
	IFCommonTexture_t *thisTARGA;
	BYTE *originBuffer;
	BYTE *buffer;

	originBuffer = m_fileMng->ReadFileFromPack( filename );
	buffer = originBuffer;

	ucharBad = *buffer++;
	ucharBad = *buffer++;
	imageTypeCode = *buffer++;

	memcpy( &sintBad, buffer, sizeof( short int ) );
	buffer += sizeof( short int );
	memcpy( &sintBad, buffer, sizeof( short int ) );
	buffer += sizeof( short int );

	colormapSize = *buffer++;

	memcpy( &sintBad, buffer, sizeof( short int ) );
	buffer += sizeof( short int );
	memcpy( &sintBad, buffer, sizeof( short int ) );
	buffer += sizeof( short int );

	memcpy( &width, buffer, sizeof( short int ) );
	buffer += sizeof( short int );
	memcpy( &height, buffer, sizeof( short int ) );
	buffer += sizeof( short int );

	bitCount = *buffer++;
	ucharBad = *buffer++;

	colorMode = bitCount / 8;
	imageSize = width * height * 4;

	thisTARGA = ( IFCommonTexture_t* )malloc( sizeof( IFCommonTexture_t ) );

	thisTARGA->buffer = ( unsigned char* )malloc( imageSize );
	
	if( imageTypeCode == 2 )
	{
		unsigned char red, green, blue, alpha;
		long i;
		if( colormapSize == 0x18 )
		{
			for( i = 0; i < imageSize; i += 4 )
			{
				blue = *buffer++;
				green = *buffer++;
				red = *buffer++;
				if( colorMode == 4 ) alpha = *buffer++;
								
				thisTARGA->buffer[i] = blue;
				thisTARGA->buffer[i + 1]= green;
				thisTARGA->buffer[i + 2] = red;

				if( colorMode == 3 ) 
					if( red == 0 && green == 0 && blue == 0 ) thisTARGA->buffer[i + 3] = 0;
					else thisTARGA->buffer[i + 3] = 255;
					if( colorMode == 4 ) thisTARGA->buffer[i + 3] = alpha;
			}
		}
		else
		{
			long row, column;
			unsigned char *pixbuf;
			for( row = height - 1; row >= 0; row -- )
			{
				pixbuf = thisTARGA->buffer + row * width * 4;
				for( column = 0; column < width; column ++ )
				{
					blue = *buffer++;
					green = *buffer++;
					red = *buffer++;
					if( colorMode == 4 ) alpha = *buffer++;

					tempColor = blue;
					tempColor = (int)((float)tempColor * 1.5f);
					if( tempColor > 255 ) tempColor = 255;
					*pixbuf ++ = (unsigned char)tempColor;
					tempColor = green;
					tempColor = (int)((float)tempColor * 1.5f);
					if( tempColor > 255 ) tempColor = 255;
					*pixbuf ++ = (unsigned char)tempColor;
					tempColor = red;
					tempColor = (int)((float)tempColor * 1.5f);
					if( tempColor > 255 ) tempColor = 255;
					*pixbuf ++ = (unsigned char)tempColor;


					if( colorMode == 4 ) *pixbuf ++ = alpha;
					else *pixbuf ++ = 255;

				}
			}
		}
	}
	else
	if( imageTypeCode == 10 )
	{
		unsigned char red, green, blue, alpha, packetHeader, packetSize, j;
		unsigned char *pixbuf;
		int row, column;
		for( row = height - 1; row >= 0; row -- )
		{
			pixbuf = thisTARGA->buffer + row * width * 4;
			for( column = 0; column < width; )
			{
				packetHeader = *buffer++;
				packetSize = 1 + ( packetHeader & 0x7f );
				if( packetHeader & 0x80 )
				{
					blue = *buffer++;
					green = *buffer++;
					red = *buffer++;
					if( colorMode == 4 ) alpha = *buffer++;
					else alpha = 255;
					for( j = 0; j < packetSize; j ++ )
					{
						*pixbuf ++ = blue;
						*pixbuf ++ = green;
						*pixbuf ++ = red;
						*pixbuf ++ = alpha;


						column ++;
						if( column == width )
						{
							column = 0;
							if( row > 0 ) row --;
							else 
								goto breakOut;
							pixbuf = thisTARGA->buffer + row * width * 4;
						}
					}
				}
				else
				{
					for( j = 0; j < packetSize; j ++ )
					{
						blue = *buffer++;
						green = *buffer++;
						red = *buffer++;
						if( colorMode == 4 ) alpha = *buffer++;
						else alpha = 255;
						
						*pixbuf ++ = blue;
						*pixbuf ++ = green;
						*pixbuf ++ = red;
						*pixbuf ++ = alpha;

					}
					column ++;
					if( column == width )
					{
						column = 0;
						if( row > 0 ) row --;
						else goto breakOut;
						
						pixbuf = thisTARGA->buffer + row * width * 4;
					}
				}
			}
		}
	}
breakOut:;

	GTH_SAFE_FREE ( originBuffer );

	thisTARGA->colormode = 4;
	thisTARGA->width = width;
	thisTARGA->height = height;

	return thisTARGA;
}
