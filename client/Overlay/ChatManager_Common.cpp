
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <afxwin.h>
#include <d3dx8.h>
#include <mmsystem.h>
#include "../global_def.h"

char* g_chatFontFilename[] = 
{
	"han",			
	"han",			
	"jpn",			
	"chn",			
	"twn",			
};


int g_nCharSet[5] = {
	HANGUL_CHARSET,			
	DEFAULT_CHARSET,		
	SHIFTJIS_CHARSET,		
	GB2312_CHARSET,			
	CHINESEBIG5_CHARSET,	
};


TCHAR g_szFontName[5][11] = {
	_T("Gulim"),	_T("Arial"),	_T("MS PGothic"),
	_T("NSimSun"),	_T("MingLiU"),	 
};

#define		CHAT_REVERSE_COLOR		0xd0000000
#define		CHAT_REVERSE_HANCOLOR	0xff000000

CChatManager::CChatManager()
{

	bIsActiveCompWnd = false; 

	
	m_chatColor[0] = D3DCOLOR_ARGB( 255, 255, 255, 255 );	
	m_chatColor[1] = D3DCOLOR_ARGB( 255, 0, 0, 0 );			

	m_chatColor[2] = D3DCOLOR_ARGB( 255, 170, 170, 255 );	


	m_chatColor[3] = D3DCOLOR_ARGB( 255, 0, 255, 0 );		
	m_chatColor[4] = D3DCOLOR_ARGB( 255, 0, 0, 255 );		
	m_chatColor[5] = 0xff7cfc00;							
	m_chatColor[6] = 0xfff5f5dc;							
	m_chatColor[7] = 0xffcd5c5c;							
	m_chatColor[8] = 0xffffd700;							
	m_chatColor[9] = D3DCOLOR_ARGB( 255, 150, 150, 150 );	

	
	m_hanFontBuffer = NULL;
	m_engFontBuffer = NULL;

	
	m_hanVex = NULL;
	m_engVex = NULL;
	m_reverseVex = NULL;

	m_numReverse = 0;

	
	m_hanTex = NULL;
	m_engTex = NULL;

	m_nEngStr = 0;
	m_nEngVexCount = 0;
	m_pEngVertices = NULL;
	m_engSize = 12.0f;

	m_nHanVexCount = 0;
	m_hanSize = 12.0f;
	m_pHanVertices = NULL;

	m_pReverse = NULL;

	m_zPos = 0.0f;
	m_wPos = 1.0f;

	
	m_nCopyIndexBuffer = 0;
	m_nStr = 0;
	for( int i = 0; i < 32; i ++ )
		for( int j = 0; j < 32; j ++ )
			m_texInfo[i][j] = 0;

	
	for( i = 0; i < CHAT_MAXSTRING; i ++ )
		memset( &m_str[i], 0, sizeof( chatString_t ) );

	m_engColor = m_hanColor = 0xffffffff;
	m_reverseColor = CHAT_REVERSE_COLOR;

	m_outputType = CHAT_OUTPUT_NORMAL;

	m_renderFlag = 0;

	m_debugOutputFlag = false;
	m_frameFlag = CHAT_GLOBAL_FRAME_FLAG;
	m_saveFrameFlag = 0;
	m_inputType = 0;

	InitInput();
}

CChatManager::~CChatManager()
{
	GTH_SAFE_FREE ( m_engFontBuffer );
	GTH_SAFE_FREE ( m_hanFontBuffer );

}

void CChatManager::SetChat( int languageType, int inputType )
{
	m_inputType = inputType;
	LoadFont( languageType );
	LoadSize( languageType );
}



void CChatManager::LoadFont( int languageType )
{
	
	char filename[128];
	BYTE *buf, *buf2;

	strcpy( filename, g_chatFontFilename[languageType] );
	strcat( filename, ".fnt" );

	buf2 = m_fileMng->ReadFileFromPack( filename );
	buf = buf2;


#if defined (_GTH_CHINESE_VERSION) || (_GTH_MALAYSIA_CHN_VERSION)		
	m_hanFontBuffer = (BYTE *)malloc( 4096 * 1024 * 2 );
	memcpy( m_hanFontBuffer, buf, 4096 * 1024 * 2 );
#endif
#ifdef _GTH_TAIWAN_VERSION
	m_hanFontBuffer = (BYTE *)malloc( 3072 * 1024 * 2 );
	memcpy( m_hanFontBuffer, buf, 3072 * 1024 * 2 );
#endif
#if defined _GTH_TERRA_VERSION || _GTH_KOREAN_VERSION || _GTH_ENGLISH_VERSION 
	m_hanFontBuffer = (BYTE *)malloc( 2048 * 1024 * 2 );
	memcpy( m_hanFontBuffer, buf, 2048 * 1024 * 2 );
#endif

	GTH_SAFE_FREE ( buf2 );

	buf2 = m_fileMng->ReadFileFromPack( "eng.fnt" );
	buf = buf2;

	m_engFontBuffer = (BYTE *)malloc( 256 * 256 * 2 );
	memcpy( m_engFontBuffer, buf, 256 * 256 * 2 );

	GTH_SAFE_FREE ( buf2 );

	
	m_logFont.lfHeight			= m_hanSize;
	m_logFont.lfWidth			= 0;
	m_logFont.lfEscapement		= 0;
	m_logFont.lfOrientation		= 0;
	m_logFont.lfWeight			= FW_NORMAL;
	m_logFont.lfItalic			= 0;
	m_logFont.lfUnderline		= false;
	m_logFont.lfStrikeOut		= false;
	m_logFont.lfCharSet			= g_nCharSet[languageType];
	m_logFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	m_logFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	m_logFont.lfQuality			= ANTIALIASED_QUALITY;
	m_logFont.lfPitchAndFamily	= VARIABLE_PITCH;
	strcpy( m_logFont.lfFaceName, g_szFontName[languageType] );

}


void CChatManager::LoadSize( int languageType )
{
	
	char filename[128];
	BYTE *buf, *buf2;

	strcpy( filename, g_chatFontFilename[languageType] );
	strcat( filename, ".dat" );

	buf2 = m_fileMng->ReadFileFromPack( filename );
	buf = buf2;

	memcpy( &m_nHanTexCoord, buf, sizeof( int ) );
	buf += sizeof( int );

	for( DWORD i = 0; i < m_nHanTexCoord; i ++ )
	{
		memcpy( &m_hanTexCoord[i], buf, sizeof( GTH_FONTTEXCOORD ) );
		buf += sizeof( GTH_FONTTEXCOORD );
	}

	GTH_SAFE_FREE ( buf2 );

	buf2 = m_fileMng->ReadFileFromPack( "eng.dat" );
	buf = buf2;

	memcpy( &m_nEngTexCoord, buf, sizeof( int ) );
	buf += sizeof( int );

	for( i = 0; i < m_nEngTexCoord; i ++ )
	{
		memcpy( &m_engTexCoord[i], buf, sizeof( GTH_FONTTEXCOORD ) );
		buf += sizeof( GTH_FONTTEXCOORD );
	}

	GTH_SAFE_FREE ( buf2 );
}

void CChatManager::CreateFont()
{
	
	
	
	D3DLOCKED_RECT d3dlr; 

	
	D3DXCreateTexture( m_pd3dDevice, 256, 256, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED,
	&m_engTex );

	m_engTex->LockRect( 0, &d3dlr, 0, 0 );
    
	BYTE* pDst = static_cast<byte *>(d3dlr.pBits); 
	memcpy( pDst, m_engFontBuffer, 256 * 256 * 2 );

	m_engTex->UnlockRect(0);

	
	
	
	
	
	m_engVex = new CD3DBuffer();
	m_engVex->Create( CHAT_MAXENGVERTEX, GTH_FORMAT_XYZRHW |  GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
		GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY	);

	
	
	
	D3DXCreateTexture( m_pd3dDevice, 512, 512, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED,
	&m_hanTex );

	m_hanTex->LockRect( 0, &d3dlr, 0, 0 );
    
	pDst = static_cast<byte *>(d3dlr.pBits); 

	memset( pDst, 0, 512 * 512 * 2 );

	m_hanTex->UnlockRect(0);

	



	m_hanVex = new CD3DBuffer();
	m_hanVex->Create( CHAT_MAXHANVERTEX, GTH_FORMAT_XYZRHW | GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
		GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY	);




	m_reverseVex = new CD3DBuffer();
	m_reverseVex->Create( CHAT_MAXREVERSEVERTEX, GTH_FORMAT_XYZRHW | GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
		GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY	);

	
	for( int i = 0; i < CHAT_MAXSTRING; i ++ )
		memset( &m_str[i], 0, sizeof( chatString_t ) );

	for( i = 0; i < 32; i ++ )
		for( int j = 0; j < 32; j ++ )
			m_texInfo[i][j] = 0;
}

void CChatManager::SetRenderState()
{
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING         , FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_NONE ); 
	m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE     , TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ZBIAS            , 0 );
	m_pd3dDevice->SetRenderState( D3DRS_ZFUNC            , D3DCMP_LESSEQUAL );
	m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE     , TRUE );
	
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE  , TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF         , 0x00 );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC        , D3DCMP_GREATER );

	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , D3DBLEND_INVSRCALPHA );

	m_pd3dDevice ->SetRenderState ( D3DRS_ZENABLE,	TRUE );

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP );

	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP   , D3DTOP_SELECTARG2 );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );
	
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP   , D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );

	m_pd3dDevice->SetTextureStageState( 1 , D3DTSS_COLOROP   , D3DTOP_DISABLE );
	m_pd3dDevice->SetTextureStageState( 1 , D3DTSS_ALPHAOP   , D3DTOP_DISABLE );

	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_MAGFILTER , D3DTEXF_LINEAR );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_MIPFILTER , D3DTEXF_NONE );
}

void CChatManager::Restore()
{
	CreateFont();
}




void CChatManager::InvalidateFont()
{
	
	GTH_SAFE_RELEASE( m_engTex );
	GTH_SAFE_RELEASE( m_hanTex );

	if	(	m_engVex	)	m_engVex ->Release ();
	GTH_SAFE_DELETE ( m_engVex );
	if	(	m_hanVex	)	m_hanVex ->Release ();
	GTH_SAFE_DELETE ( m_hanVex );
	if	(	m_reverseVex	)	m_reverseVex ->Release ();
	GTH_SAFE_DELETE ( m_reverseVex );
}




void CChatManager::Release()
{
	GTH_SAFE_FREE ( m_engFontBuffer );
	GTH_SAFE_FREE ( m_hanFontBuffer );
	
}


void CChatManager::BeginFrame()
{
	m_saveFrameFlag ++;
	m_frameFlag = m_saveFrameFlag;
}

void CChatManager::EndFrame()
{
	int i;

	SetRenderState();

	MakeChatTexture();

	
	
	m_pd3dDevice->SetTexture( 0, NULL );
	m_pd3dDevice->SetStreamSource( 0, this->m_reverseVex->GetD3DVertexBuffer(), sizeof(HANTEXTUREVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_HANVERTEX );

	for( i = 0; i < m_numReverse / 4; i ++ )
	{
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, i * 4, 2 );
	}
	m_numReverse = 0;

	
	m_pd3dDevice->SetTexture( 0, m_hanTex );
	m_pd3dDevice->SetStreamSource( 0, this->m_hanVex->GetD3DVertexBuffer(), sizeof(HANTEXTUREVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_HANVERTEX );

	for( i = 0; i < m_nHanVexCount / 4; i ++ )
	{
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, i * 4, 2 );
	}
	m_nHanVexCount = 0;



	
	m_frameFlag = CHAT_GLOBAL_FRAME_FLAG;
}

void CChatManager::Update()
{
	m_frameFlag = CHAT_GLOBAL_FRAME_FLAG;
	m_saveFrameFlag = 0;
}





void CChatManager::UpdateEmptyTexture()
{
	int i, j;
	int tx, ty, ts, k;
	for( i = 0; i < m_nStr; i ++ )
	{
		
		if( m_str[i].nPos == 0 )
		{
			for( j = 0; j < m_str[i].nTPos; j ++ )
			{
				tx = m_str[i].tx[j] / 16;
				ty = m_str[i].ty[j] / 16;
				ts = tx + m_str[i].size[j] / 16 + 1;
				
				for( k = tx; k < ts; k ++ ) m_texInfo[ty][k] = 0;
			}
			memset( &m_str[i], 0, sizeof( chatString_t ) );
		}
		m_str[i].nPos = 0;
	}
}

void CChatManager::SetChatTexture( int row, int column )
{
	
	
	int ci = m_nCopyIndexBuffer;
	m_copyIndexBuffer[ci].x = column * 16;
	m_copyIndexBuffer[ci].y = row * 16;
	m_copyIndexBuffer[ci].dx = 0;
	m_copyIndexBuffer[ci].dy = 0;
	m_copyIndexBuffer[ci].size = 16;
	m_copyIndexBuffer[ci].type = 2;
	m_nCopyIndexBuffer ++;
}

int CChatManager::FindEmptyColumn( int &row, int &column )
{
	int i, j;
	for( i = 0; i < 32; i ++ )
	{
		for( j = 0; j < 32; j ++ )
		{
			if( m_texInfo[i][j] == 0 )
			{
				row = i;
				column = j;
				SetChatTexture( row, column );
				return( 1 );
			}
		}
	}
	
	return( 0 );
}


int CChatManager::CopyFont( BYTE *src, int x, int y, BYTE* dst, int dx, int dy, int xs, int ys )
{
	unsigned int sourad, destad;
	BYTE* sour = src;
	BYTE* dest = dst;

	
	destad = y * 512 * 2 + x * 2;
	sourad = dy * 1024 * 2 + dx * 2;

	sour += sourad;
	dest += destad;

	
	for( int i = 0; i < ys; i ++ )
	{
		if( src != NULL )
		{
			memcpy( dest, sour, xs * 2 );
			sour += 1024 * 2;
		}
		else memset( dest, 0, xs * 2 );

		dest += 512 * 2;
	}
	return( 1 );
}


void CChatManager::AddStr( int x, int y, int idx, int reverseFlag )
{
	
	int n = this->m_str[idx].nPos;
	this->m_str[idx].x[n] = x;
	this->m_str[idx].y[n] = y;
	this->m_str[idx].fontSize[n] = m_hanSize;
	this->m_str[idx].fontColor[n] = m_hanColor;
	this->m_str[idx].reverseFlag[n] = reverseFlag;

	this->m_str[idx].zPos[n] =	m_zPos;
	this->m_str[idx].wPos[n] =	m_wPos;

	this->m_str[idx].renderFlag[n] = m_frameFlag;

	this->m_str[idx].nPos ++;
}


void CChatManager::MakeChatTexture()
{
	int i, j, k, reverserflag;
	float px, py;
	float tx, ty, ts, reverseSizeFlag = 0;
	float xsize, ysize;
	BYTE *src, *dst;
	D3DLOCKED_RECT d3dlr;
	DWORD fontColor;

	
	m_pHanVertices = (HANTEXTUREVERTEX *)m_hanVex->Lock();
	m_pReverse = (HANTEXTUREVERTEX *)m_reverseVex->Lock();

	for( i = 0; i < m_nStr; i ++ )
	{
		for( j = 0; j < m_str[i].nPos; j ++ )
		{
			if( m_frameFlag != m_str[i].renderFlag[j] ) continue;

			px = (float)m_str[i].x[j];
			py = (float)m_str[i].y[j];

			fontColor = m_str[i].fontColor[j];
			reverserflag = m_str[i].reverseFlag[j];

			for( k = 0; k < m_str[i].nTPos; k ++ )
			{
				tx = (float)m_str[i].tx[k];
				ty = (float)m_str[i].ty[k];
				ts = (float)m_str[i].size[k];
				
				xsize = m_str[i].fontSize[j] * (float)ts / 12.0f;
				ysize = m_str[i].fontSize[j];

				

				
				if( m_str[i].line[k] == 1 )
				{
					px = (float)m_str[i].x[j];
					py += m_str[i].fontSize[j];
				}

				if( reverserflag )
				{
					if( m_reverseSize > 0 )
					{
						if( k == 0 && m_str[i].nTPos == 1 ) reverseSizeFlag = 3;
						else if( k == 0 ) reverseSizeFlag = 0;
						else if( k == m_str[i].nTPos - 1 ) reverseSizeFlag = 2;
						else reverseSizeFlag = 1;
						MakeReverse( px, py, xsize, ysize, m_str[i].zPos[j], m_str[i].wPos[j], reverseSizeFlag );
					}
					else
					{
						MakeReverse( px, py, xsize, ysize, m_str[i].zPos[j], m_str[i].wPos[j], -1 );
					}
				}

				float tempzpos = m_str[i].zPos[j];
				float tempwpos = m_str[i].wPos[j];


				m_pHanVertices[m_nHanVexCount].position = D3DXVECTOR4( px - 0.5f, py - 0.5f, tempzpos, tempwpos );

				m_pHanVertices[m_nHanVexCount].color = fontColor;
				m_pHanVertices[m_nHanVexCount].tu = tx / 512.0f;
				m_pHanVertices[m_nHanVexCount].tv = ty / 512.0f;
				
				m_nHanVexCount ++;
				

				m_pHanVertices[m_nHanVexCount].position = D3DXVECTOR4( px + xsize - 0.5f, py - 0.5f, tempzpos, tempwpos );
				
				m_pHanVertices[m_nHanVexCount].color = fontColor;
				m_pHanVertices[m_nHanVexCount].tu = ( tx + ts ) / 512.0f;
				m_pHanVertices[m_nHanVexCount].tv = ty / 512.0f;
				
				m_nHanVexCount ++;
				

				m_pHanVertices[m_nHanVexCount].position = D3DXVECTOR4( px + xsize - 0.5f, py + ysize - 0.5f, tempzpos, tempwpos );

				m_pHanVertices[m_nHanVexCount].color = fontColor;
				m_pHanVertices[m_nHanVexCount].tu = ( tx + ts ) / 512.0f;
				m_pHanVertices[m_nHanVexCount].tv = ( ty + 12.0f ) / 512.0f;
				
				m_nHanVexCount ++;
				

				m_pHanVertices[m_nHanVexCount].position = D3DXVECTOR4( px - 0.5f, py + ysize - 0.5f, tempzpos, tempwpos );

				m_pHanVertices[m_nHanVexCount].color = fontColor;
				m_pHanVertices[m_nHanVexCount].tu = tx / 512.0f;
				m_pHanVertices[m_nHanVexCount].tv = ( ty + 12.0f ) / 512.0f;
				
				m_nHanVexCount ++;
				
				
				px += xsize;
			}
		}
	}

	this->m_hanVex->Unlock();
	m_reverseVex->Unlock();


	
	if( m_nCopyIndexBuffer == 0 ) return;

	this->m_hanTex->LockRect( 0, &d3dlr, 0, 0 );
	dst = static_cast<byte *>(d3dlr.pBits); 
	
	for( i = 0; i < m_nCopyIndexBuffer; i ++ )
	{
		switch( m_copyIndexBuffer[i].type )
		{
		
		case 0 :
			src = m_hanFontBuffer;
			break;
		case 1 :
			src = m_hanFontBuffer;
			break;

		case 2 :
			src = NULL;
			break;
		}
		CopyFont( src, m_copyIndexBuffer[i].x, m_copyIndexBuffer[i].y,
			dst, m_copyIndexBuffer[i].dx, m_copyIndexBuffer[i].dy,
			m_copyIndexBuffer[i].size, 12 );
	}
	
	m_nCopyIndexBuffer = 0;

	this->m_hanTex->UnlockRect(0);
	
}


void CChatManager::MakeReverse( float x, float y, float xs, float ys, float zpos, float wpos, int reverseSizeFlag )
{
	if( reverseSizeFlag >= 0 )
	{
		if( reverseSizeFlag == 0 ) 
		{
			x -= (float)m_reverseSize;
			xs += (float)m_reverseSize;
		}
		else if( reverseSizeFlag == 2 ) 
		{
			xs += (float)m_reverseSize;
		} 
		else if( reverseSizeFlag == 3 ) 
		{
			x -= (float)m_reverseSize;
			xs += (float)m_reverseSize * 2.0f;
		} 
		
		y -= (float)m_reverseSize;
		ys += (float)m_reverseSize * 2.0f;
	}
	else
	{
		xs += (float)1.0f;
	}
	
	float tempzpos = zpos; 
	float tempwpos = wpos;


	m_pReverse[m_numReverse].position = D3DXVECTOR4( x - 0.5f, y - 0.5f, tempzpos, tempwpos );
	
	m_pReverse[m_numReverse].color = m_reverseColor;
	m_pReverse[m_numReverse].tu = 0.0f;
	m_pReverse[m_numReverse].tu = 0.0f;
				
	m_numReverse ++;
		

	m_pReverse[m_numReverse].position = D3DXVECTOR4( x + xs - 0.5f, y - 0.5f, tempzpos, tempwpos );
		
	m_pReverse[m_numReverse].color = m_reverseColor;
	m_pReverse[m_numReverse].tu = 0.0f;
	m_pReverse[m_numReverse].tu = 0.0f;
				
	m_numReverse ++;
				

	m_pReverse[m_numReverse].position = D3DXVECTOR4( x + xs - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
	
	m_pReverse[m_numReverse].color = m_reverseColor;
	m_pReverse[m_numReverse].tu = 0.0f;
	m_pReverse[m_numReverse].tu = 0.0f;
				
	m_numReverse ++;
				

	m_pReverse[m_numReverse].position = D3DXVECTOR4( x - 0.5f, y + ys - 0.5f, tempzpos, tempwpos );
				
	m_pReverse[m_numReverse].color = m_reverseColor;
	m_pReverse[m_numReverse].tu = 0.0f;
	m_pReverse[m_numReverse].tu = 0.0f;
			
	m_numReverse ++;

}


int CChatManager::MakeStr( int idx )
{
	int tx1, ty1, tx2, ty2;
	int tx, ty, ts;		
	int rx, ry;			
	int type;
	int tn = 0;
	int row, col;
	int ci;
	char *str;

	m_str[idx].nPos = 0;

	rx = this->m_str[idx].x[0];
	ry = this->m_str[idx].y[0];
	str = this->m_str[idx].str;

	if( str[0] == NULL ) return( 0 );

	
	this->m_str[idx].nTPos = 0;
	
	if( !FindEmptyColumn( row, col ) ) return( 0 );

	tx = this->m_str[idx].tx[0] = col * 16;
	ty = this->m_str[idx].ty[0] = row * 16;
	this->m_str[idx].size[0] = 0;
	this->m_str[idx].fontSize[0] = m_hanSize;
	this->m_str[idx].nTPos ++;

	while( *str )
	{
		type = BuildStr( tx1, ty1, tx2, ty2, str );
	
		
		
		if( type == 1 ) 
		{
			ts = tx2 - tx1;
			str ++;
		} else 
		
		if( type == 0 )
		{
			ts = tx2 - tx1;
			str += 2;
		} else 
		
		if( type == 2 )
		{
			ts = 6;
			str ++;
		} else
		
		if( type == 3 )
		{
			m_texInfo[row][col] = 1;
			tn = this->m_str[idx].nTPos;
			if( !FindEmptyColumn( row, col ) ) return( 0 );

			tx = this->m_str[idx].tx[tn] = col * 16;
			ty = this->m_str[idx].ty[tn] = row * 16;
			this->m_str[idx].size[tn] = 0;
			this->m_str[idx].line[tn] = 1;
			this->m_str[idx].nTPos ++;

			str ++;
			continue;
		} else 
		{
			str ++;
			continue;
		}

		int checkEmpty;
		checkEmpty = ( tx + ts ) / 16;
		
		if( checkEmpty > col )
		{
			if( m_texInfo[row][checkEmpty] == 1 || checkEmpty >= 32 )
			{
				m_texInfo[row][col] = 1;
				
				tn = this->m_str[idx].nTPos;
				if( !FindEmptyColumn( row, col ) ) return( 0 );
				
				tx = this->m_str[idx].tx[tn] = col * 16;
				ty = this->m_str[idx].ty[tn] = row * 16;
				this->m_str[idx].size[tn] = 0;
				this->m_str[idx].nTPos ++;
			}
			else
			{
				m_texInfo[row][col] = 1;
				col = checkEmpty;
				SetChatTexture( row, col );
			}
		}
		
		ci = m_nCopyIndexBuffer;
		m_copyIndexBuffer[ci].x = tx;
		m_copyIndexBuffer[ci].y = ty;
		m_copyIndexBuffer[ci].dx = tx1;
		m_copyIndexBuffer[ci].dy = ty1;
		m_copyIndexBuffer[ci].size = tx2 - tx1;
		m_copyIndexBuffer[ci].type = type;
		m_nCopyIndexBuffer ++;

		this->m_str[idx].size[tn] += ts;
		tx += ts;
		rx += ts;
	}
	m_texInfo[row][col] = 1;
	return( 1 );
}






int CChatManager::GetChatStrLine( int size, char *str )
{
	int len, line = 0;
	int sPos = 0, cPos = 0, templen;

	char temp[4096];

	len = GetChatStrSizeChat( str );
	while( len > size )
	{
		if( str[sPos] == ' ' && cPos == 0 ) 
		{
			sPos ++;
			continue;
		}
		if( str[cPos + sPos] == 0 )
		{
			break;
		}
		
		
		if( IsDBCSLeadByte( (BYTE)str[cPos + sPos] ) )
		{
			memset( temp, 0, sizeof(temp) );
			memcpy( temp, str + sPos, cPos );
			templen = GetChatStrSizeChat( temp );

			if( templen + 12 >= size )
			{
				line ++;
				sPos += cPos;
				len -= templen;
				cPos = 0;
			}
			else			
			{
				cPos ++;							
				if (str[cPos + sPos] != 0) cPos++;	
			}
		}
		else 
		{
			memset( temp, 0, sizeof(temp) );
			memcpy( temp, str + sPos, cPos );

			templen = GetChatStrSizeChat( temp );

			if( templen + 8 >= size )
			{
				line ++;
				sPos += cPos;
				len -= templen;
				cPos = 0;
			}
			else cPos ++;
		}
	}
	return( line + 1 );
}





DWORD g_chatManagerFontColor[] =
{
	0xffffffff,		
	0xfff0ffff,		
	0xfff0fff0,		
	0xff6495ed,		
	0xffd2b48c,		
	0xff7cfc00,		
	0xffbc8f8f,		
	0xffcd5c5c,		
	0xffe6e6fa,		
	0xff000000		
};


void CChatManager::DebugOut( int x, int y, char* str, ... )
{
}


char* CChatManager::NextToken()
{
	char *token = m_pChatManagerTokenBuffer;

	while( !( *m_pChatManagerTokenBuffer == '&' || *m_pChatManagerTokenBuffer == NULL ) )
	{
		m_pChatManagerTokenBuffer ++;
	}
	*m_pChatManagerTokenBuffer++ = '\0';
	return( token );
}

char* CChatManager::NextTokenMail()
{
	char *token = m_pChatManagerTokenBuffer;

	while( !( *m_pChatManagerTokenBuffer == '\r' || *m_pChatManagerTokenBuffer == NULL ) )
	{
		m_pChatManagerTokenBuffer ++;
	}
	*m_pChatManagerTokenBuffer++ = '\0';
	return( token );
}

int CChatManager::GetTextRectPage( int xs, int ys, char* str, int page, char *pageStr )
{
	int len, line = 0;
	int sPos = 0, cPos = 0;
	int curpage = 0;
	char temp[4096];

	len = strlen( str );
	if( len <= 0 ) return -1;

	memset( pageStr, 0, sizeof( pageStr ) );

	while( 1 )
	{
		if( str[cPos + sPos] == 0 )
		{
			break;
		}
		
		
		if( IsDBCSLeadByte( (BYTE)str[cPos + sPos] ) )
		{
			memset( temp, 0, sizeof( temp ) );
			memcpy( temp, str + sPos, cPos );
			
			if( !GetChatStrSize2( xs, ys, temp ) )
			{
				if( curpage == page ) 
				{
					strcpy( pageStr, temp );
				}
				sPos += cPos;
				cPos = 0;
				curpage ++;
			}
			else 
			{
				cPos ++;								
				if ( str[cPos + sPos] != 0 ) cPos ++;	
			}
		}
		else 
		{
			memset( temp, 0, sizeof( temp ) );
			memcpy( temp, str + sPos, cPos );
			
			if( !GetChatStrSize2( xs, ys, temp ) )
			{
				if( curpage == page ) 
				{
					strcpy( pageStr, temp );
				}
				sPos += cPos;
				cPos = 0;
				curpage ++;
			}
			else 
			{
				cPos += 1;
			}
		}
	}
	memcpy( temp, str + sPos, len - sPos );
	if( curpage == page ) strcpy( pageStr, temp );

	return( curpage + 1 );
}



void CChatManager::Output( int x, int y, char *str, int reverseFlag )
{
	int i, idx;

	if( str[0] == NULL ) return;
	if( strlen( str ) >= 2048 ) return;
	for( i = 0; i < this->m_nStr; i ++ )
	{
		
		if( strcmp( this->m_str[i].str, str ) == 0 ) 
		{
			AddStr( x, y, i, reverseFlag );
			return;
		}
	}
	for( i = 0; i < CHAT_MAXSTRING; i ++ )
	{
		if( this->m_str[i].str[0] == NULL ) break;
	}
	idx = i;
	if( idx >= CHAT_MAXSTRING ) return;

	
	this->m_str[idx].x[0] = x;
	this->m_str[idx].y[0] = y;
	this->m_str[idx].fontSize[0] = m_hanSize;
	this->m_str[idx].fontColor[0] = m_hanColor;
	this->m_str[idx].reverseFlag[0] = reverseFlag;

	this->m_str[idx].zPos[0] =	m_zPos;
	this->m_str[idx].wPos[0] =	m_wPos;

	
	m_str[idx].renderFlag[0] = m_frameFlag;

	strcpy( m_str[idx].str, str );
	MakeStr( idx );
	m_str[idx].nPos ++;

	if( idx == m_nStr ) m_nStr ++;
}


void CChatManager::TextOut( int x, int y, char *str, int arrangeType, int xsize )
{
	int xs;
	DWORD saveColor = m_hanColor;

	switch( arrangeType )
	{
	case CHAT_ARRANGE_NONE :
		break;
	case CHAT_ARRANGE_CENTER :
		xs = GetChatStrSize( str );
		x += xsize / 2 - xs / 2;
		break;
	case CHAT_ARRANGE_RIGHT :
		xs = GetChatStrSize( str );
		x += xsize - xs;
		break;
	case CHAT_ARRANGE_OVERSIZECUT :
		break;
	}

	switch( m_outputType )
	{
	case CHAT_OUTPUT_NORMAL :
		break;
	case CHAT_OUTPUT_SHADOW :
		SetColor( 0xff000000 );
		Output( x - 1, y + 1, str );
		break;
	case CHAT_OUTPUT_BORDER :
		SetColor( 0xff000000 );
		Output( x - 1, y - 1, str );
		Output( x - 1, y + 1, str );
		Output( x + 1, y + 1, str );
		Output( x + 1, y - 1, str );
		break;
	case CHAT_OUTPUT_BOLD :
		Output( x + 1, y, str );
		Output( x, y + 1, str );
		break;
	case CHAT_OUTPUT_REVERSE :
		Output( x, y, str, true );
		return;
		break;
	}
	SetColor( saveColor );
	if( m_outputType != CHAT_OUTPUT_REVERSE ) Output( x, y, str );
}



int CChatManager::TextOut( int x, int y, char *str, int xsize, int lineFeedSize, int &cx, int &cy, int sizeFlag, int arrangeType )
{
	int len, line = 0;
	int sPos = 0, cPos = 0, templen;
	int xs, maxlen, onelineFlag = true;
	char temp[4096];

	if( strlen( str ) <= 0 ) return 0;

	maxlen = GetChatStrSize( str );
	len = maxlen;

	xs = xsize - ( cx - x );

	while( len > xs && xsize >= 0 )
	{
		if( str[sPos] == ' ' && cPos == 0 ) 
		{
			sPos ++;
			continue;
		}
		if( str[cPos + sPos] == 0 )
		{
			break;
		}
		
		
		if( IsDBCSLeadByte( (BYTE)str[cPos + sPos] ) )		
		{
			memset( temp, 0, sizeof(temp) );
			memcpy( temp, str + sPos, cPos );
			templen = GetChatStrSize( temp );

			if( templen + 12 >= xs )
			{
				if( !sizeFlag ) 
				{
					TextOut( cx, cy + (int)lineFeedSize * line, temp );
					onelineFlag = false;
				}
				line ++;
				sPos += cPos;
				len -= templen;
				cPos = 0;

				cx = x;
				cy = y;
				xs = xsize;
			}
			else
			{
				cPos ++;								
				if (str[cPos + sPos] != 0) cPos ++;		
			}
		}
		else 
		{
			memset( temp, 0, sizeof(temp) );
			memcpy( temp, str + sPos, cPos );

			templen = GetChatStrSize( temp );

			if( templen + 6 >= xsize )
			{
				if( !sizeFlag ) 
				{
					TextOut( cx, cy + (int)lineFeedSize * line, temp );
					onelineFlag = false;
				}
				line ++;
				sPos += cPos;
				len -= templen;
				cPos = 0;

				cx = x;
				cy = y;
				xs = xsize;
			}
			else cPos ++;
		}
	}
	while( str[sPos] == ' ' )
	{
		sPos ++;
	}

	len = strlen( str );
	memset( temp, 0, sizeof(temp) );
	memcpy( temp, str + sPos, len - sPos );

	if( !sizeFlag ) 
	{
		if( onelineFlag )
		{
			TextOut( cx, cy + (int)lineFeedSize * line, temp, arrangeType, xsize );
		}
		else
		{
			if( arrangeType != CHAT_ARRANGE_OVERSIZECUT )
				TextOut( cx, cy + (int)lineFeedSize * line, temp );
		}
	}

	cx += GetChatStrSize( temp );
	cy += lineFeedSize * line;

	if( xsize >= 0 ) return( xsize );
	else return( maxlen );
}

bool CChatManager::IsVisibleLine( int inputIdx, int line )
{

	if( inputIdx != -1 )
	{
		if( m_lineLimit[inputIdx][0] != -1 && m_lineLimit[inputIdx][1] != -1 )
		{
			if( line >= m_lineLimit[inputIdx][0] && line <= m_lineLimit[inputIdx][1] )
				return true;
			else
				return false;
		}
		else
			return true;
	}
	else
		return true;

}

int CChatManager::TextOutChat( int x, int y, char *str, int xsize, int lineFeedSize, int &cx, int &cy, int sizeFlag, int arrangeType, int inputIdx, int baseLine )
{
	int len, line = 0;
	int sPos = 0, cPos = 0, templen;
	int xs, maxlen, onelineFlag = true;
	char temp[4096];
	char tempStr[3];

	if( strlen( str ) <= 0 ) return 0;

	maxlen = GetChatStrSizeChat( str );
	len = maxlen;

	xs = xsize - ( cx - x );

	while( len > xs && xsize >= 0 )
	{
		if( str[cPos + sPos] == 0 )
		{
			break;
		}
		
		
		if( IsDBCSLeadByte( (BYTE)str[cPos + sPos] ) )		
		{
			tempStr[0] = str[cPos + sPos];
			tempStr[1] = str[cPos + sPos + 1];
			tempStr[2] = 0;

			memset( temp, 0, sizeof(temp) );
			memcpy( temp, str + sPos, cPos );
			templen = GetChatStrSizeChat( temp );

			if( templen + GetChatStrSizeChat( tempStr ) >= xs )
			{
				if( !sizeFlag ) 
				{
					if( IsVisibleLine( inputIdx, baseLine + line ) )
						TextOut( cx, cy + (int)lineFeedSize * line, temp );
					onelineFlag = false;
				}
				line ++;
				sPos += cPos;
				len -= templen;
				cPos = 0;

				cx = x;
				cy = y;
				xs = xsize;
			}
			else
			{
				cPos ++;								
				if (str[cPos + sPos] != 0) cPos ++;		
			}
		}
		else 
		{
			tempStr[0] = str[cPos + sPos];
			tempStr[1] = 0;

			memset( temp, 0, sizeof(temp) );
			memcpy( temp, str + sPos, cPos );

			templen = GetChatStrSizeChat( temp );

			if( templen + GetChatStrSizeChat( tempStr ) >= xsize )
			{
				if( !sizeFlag ) 
				{
					if( IsVisibleLine( inputIdx, baseLine + line ) )
						TextOut( cx, cy + (int)lineFeedSize * line, temp );
					onelineFlag = false;
				}
				line ++;
				sPos += cPos;
				len -= templen;
				cPos = 0;

				cx = x;
				cy = y;
				xs = xsize;
			}
			else cPos ++;
		}
	}

	len = strlen( str );
	memset( temp, 0, sizeof(temp) );
	memcpy( temp, str + sPos, len - sPos );

	if( !sizeFlag ) 
	{
		if( onelineFlag )
		{
			if( IsVisibleLine( inputIdx, baseLine + line ) )
				TextOut( cx, cy + (int)lineFeedSize * line, temp, arrangeType, xsize );
		}
		else
		{
			if( arrangeType != CHAT_ARRANGE_OVERSIZECUT )
			{
				if( IsVisibleLine( inputIdx, baseLine + line ) )
					TextOut( cx, cy + (int)lineFeedSize * line, temp );
			}
		}
	}

	cx += GetChatStrSizeChat( temp );
	cy += lineFeedSize * line;

	return line;
}

void CChatManager::DrawText( int x, int y, char *str, int xsize, int lineFeedSize, int arrangeType )
{
	char buffer[4096];
	char *token;
	int colorIndex, typeIndex, len;
	int sx, sy, tx, ty;

	tx = sx = x;
	ty = sy = y;
	len = strlen( str );

	if( len <= 0 ) return;

	strcpy( buffer, str );
	m_pChatManagerTokenBuffer = buffer;
	m_pChatManagerTokenBufferEnd = buffer + len;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;

		TextOut( x, y, token, xsize, lineFeedSize, sx, sy, false, arrangeType );
		if( sy > ty )
		{

			ty = sy;
		}

		if( m_pChatManagerTokenBuffer >= m_pChatManagerTokenBufferEnd ) 
		{
			y = sy;
			break;
		}

		switch( *m_pChatManagerTokenBuffer )
		{
		case CHAT_TOKEN_COLOR :
			colorIndex = *(++m_pChatManagerTokenBuffer) - '0';
			SetColor( m_chatColor[colorIndex] );
			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_LINE :
			sx = tx;
			sy += lineFeedSize;
			ty = sy;
			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_OUTTYPE :
			typeIndex = *(++m_pChatManagerTokenBuffer) - '0';
			SetOutputType( typeIndex );
			m_pChatManagerTokenBuffer ++;
			break;
			
		case CHAT_TOKEN_ARRANGETYPE :
			typeIndex = *(++m_pChatManagerTokenBuffer) - '0';
			arrangeType = typeIndex;
			m_pChatManagerTokenBuffer ++;
			break;
		}
		y = sy;

	}
}

void CChatManager::DrawTextMail( int x, int y, char *str, int xsize, int lineFeedSize, int arrangeType, int inputIdx )
{
	char buffer[4096];
	char *token;
	int len;
	int sx, sy, tx, ty;
	int line = 0;

	tx = sx = x;
	ty = sy = y;
	len = strlen( str );

	if( len <= 0 ) return;

	strcpy( buffer, str );
	m_pChatManagerTokenBuffer = buffer;
	m_pChatManagerTokenBufferEnd = buffer + len;

	while( 1 )
	{
		token = NextTokenMail();
		if( token == NULL ) break;

		line += TextOutChat( x, y, token, xsize, lineFeedSize, sx, sy, false, arrangeType, inputIdx, line );
		if( sy > ty )
		{
			ty = sy;
		}

		if( m_pChatManagerTokenBuffer >= m_pChatManagerTokenBufferEnd ) 
		{
			y = sy;
			break;
		}

		switch( *m_pChatManagerTokenBuffer )
		{
		case '\n' :
			sx = tx;
			sy += lineFeedSize;
			ty = sy;
			m_pChatManagerTokenBuffer ++;
			line ++;
			break;
		}
		y = sy;
	}
}

int	CChatManager::GetTextRect( int xsize, int lineFeedSize, char *str, int& mx, int& my )
{
	char buffer[4096];
	char *token;
	int colorIndex, typeIndex, len, tempmx;
	int x, y, sx, sy;

	mx = 0;
	my = 0;

	x = 0;
	y = 0;
	sx = 0;
	sy = 0;

	len = strlen( str );

	if( len <= 0 ) return 0;

	memset(buffer, 0, sizeof(buffer));		

	strcpy( buffer, str );
	m_pChatManagerTokenBuffer = buffer;
	m_pChatManagerTokenBufferEnd = buffer + len;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;

		tempmx = TextOut( x, y, token, xsize, lineFeedSize, sx, sy, true );
		
		if( m_pChatManagerTokenBuffer >= m_pChatManagerTokenBufferEnd ) 
		{
			y = sy;
			break;
		}

		switch( *m_pChatManagerTokenBuffer )
		{
		case CHAT_TOKEN_COLOR :
			colorIndex = *(++m_pChatManagerTokenBuffer) - '0';
			if( colorIndex >= 0 && colorIndex <= 9 ) SetColor( m_chatColor[colorIndex] );
			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_LINE :
			if( sx > mx ) mx = sx;
			sx = 0;
			sy += lineFeedSize;
			m_pChatManagerTokenBuffer ++;
			y = sy;
			break;
		case CHAT_TOKEN_OUTTYPE :
			typeIndex = *(++m_pChatManagerTokenBuffer) - '0';
			m_pChatManagerTokenBuffer ++;
			break;
		default :
			sx += tempmx;
			break;
		}
		y = sy;
	}
	my = y + lineFeedSize;
	if( sx > mx ) mx = sx;

	return 1;
}

int CChatManager::GetTextRectLine( int xsize, int lineFeedSize, char *str )
{
	char buffer[4096];
	char *token;
	int colorIndex, typeIndex, len, tempmx;
	int x, y, sx, sy;
	
	int mx = 0;
	int my = 0;
	
	x = 0;
	y = 0;
	sx = 0;
	sy = 0;
	
	len = strlen( str );
	
	if( len <= 0 ) return 0;

	memset( buffer, 0, sizeof(buffer) );		
	
	strcpy( buffer, str );
	m_pChatManagerTokenBuffer = buffer;
	m_pChatManagerTokenBufferEnd = buffer + len;
	
	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		
		tempmx = TextOut( x, y, token, xsize, lineFeedSize, sx, sy, true );
		if( tempmx > mx ) mx = tempmx;
		
		if( m_pChatManagerTokenBuffer >= m_pChatManagerTokenBufferEnd ) 
		{
			y = sy;
			break;
		}
		
		switch( *m_pChatManagerTokenBuffer )
		{
		case CHAT_TOKEN_COLOR :
			colorIndex = *(++m_pChatManagerTokenBuffer) - '0';
			if( colorIndex >= 0 && colorIndex <= 9 ) SetColor( m_chatColor[colorIndex] );
			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_LINE :
			sx = 0;
			sy += lineFeedSize;
			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_OUTTYPE :
			typeIndex = *(++m_pChatManagerTokenBuffer) - '0';
			m_pChatManagerTokenBuffer ++;
			break;
		}
		y = sy;
	}
	my = y + lineFeedSize;
	
	return my;
}



int CChatManager::DrawTextRect( int x, int y, char *str, int xsize, int ysize, int numPage )
{
	char buffer[4096];
	int maxpage;

	memset(buffer, 0, sizeof(buffer));		

	maxpage = GetTextRectPage( xsize, ysize, str, numPage, buffer );
	DrawText( x, y, buffer, xsize, 13 );

	return( maxpage );
}



int CChatManager::DrawTextRectLineScroll( int x, int y, char *str, int xsize, int ysize, int skipLine, int pointLinkIndex )
{
	char buffer[4096];
	int maxpage = 0, my;

	memset(buffer, 0, sizeof(buffer));		

	
	my = GetTextRectLineScroll( xsize, ysize, str, skipLine, buffer ); 
	DrawTextLineScroll( x, y, buffer, xsize, 13, pointLinkIndex ); 
	
	return( maxpage );
}


void CChatManager::DrawTextLineScroll( int x, int y, char *str, int xsize, int lineFeedSize, int pointLinkIndex, int arrangeType )
{
	char buffer[4096];
	char *token;
	int colorIndex, typeIndex, len;
	int sx, sy, tx, ty;

	
	int linkIndex;

	tx = sx = x;
	ty = sy = y;
	len = strlen( str );

	if( len <= 0 ) return;

	strcpy( buffer, str );
	m_pChatManagerTokenBuffer = buffer;
	m_pChatManagerTokenBufferEnd = buffer + len;

	linkIndex = 0;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;

		TextOut( x, y, token, xsize, lineFeedSize, sx, sy, false, arrangeType );
		if( sy > ty )
		{

			ty = sy;
		}

		if( m_pChatManagerTokenBuffer >= m_pChatManagerTokenBufferEnd ) 
		{
			y = sy;
			break;
		}

		switch( *m_pChatManagerTokenBuffer )
		{
		case CHAT_TOKEN_COLOR :
			colorIndex = *(++m_pChatManagerTokenBuffer) - '0';
			SetColor( m_chatColor[colorIndex] );
			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_LINE :
			sx = tx;
			sy += lineFeedSize;
			ty = sy;
			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_OUTTYPE :
			typeIndex = *(++m_pChatManagerTokenBuffer) - '0';
			SetOutputType( typeIndex );
			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_LINKSTART :
			
			if( pointLinkIndex == linkIndex )
				SetColor( m_chatColor[2] );
			else
				SetColor( m_chatColor[3] );

			m_pChatManagerTokenBuffer ++;
			break;
		case CHAT_TOKEN_LINKEND :
			SetColor( m_chatColor[0] );
			m_pChatManagerTokenBuffer ++;
			linkIndex++;
			break;
		case CHAT_TOKEN_LINKNUMBER :
			m_pChatManagerTokenBuffer ++;
			m_pChatManagerTokenBuffer ++;
			m_pChatManagerTokenBuffer ++;
			m_pChatManagerTokenBuffer ++;
			break;
		}
		y = sy;

	}
}





int CChatManager::GetTextRectLineScroll( int xs, int ys, char* str, int line, char *lineStr )
{
	int len;
	int sPos = 0, cPos = 0;
	int curLine = 0;
	char temp[4096];
	bool bDoubleByte = false;

	len = strlen( str );
	if( len <= 0 ) return -1;

	memset( lineStr, 0, sizeof( lineStr ) );

	while( 1 )
	{
		if( str[cPos + sPos] == 0 )
		{
			break;
		}

		memset( temp, 0, sizeof( temp ) );
		memcpy( temp, str + sPos, cPos );

		

		
		if( !GetChatStrSize2( xs, 12, temp ) )
		{
			
			if ( (BYTE)str[cPos + sPos - 2] != '&' && (BYTE)str[cPos + sPos - 2] != 'n' )
			{
				if (bDoubleByte)
					cPos -= 2;
				else
					cPos -= 1;
			}
			memset( temp, 0, sizeof( temp ) );
			memcpy( temp, str + sPos, cPos );

			
			
			if( line <= 0 )
			{
				
				if( ys - curLine*13 < 0 )
				{
					return( curLine + 1 );
				}
				
				strcat( lineStr, temp );
				
				curLine ++;
			}
			sPos += cPos;
			cPos = 0;

			line --;
		}
		else 
		{
			bDoubleByte = false;

			
			if( IsDBCSLeadByte( (BYTE)str[cPos + sPos] ) )
			{
				cPos ++;								
				if ( str[cPos + sPos] != 0 )
				{
					cPos ++;	
					bDoubleByte = true;
				}
			}
			else
			{
				cPos += 1;
			}
		}
	}
	memcpy( temp, str + sPos, len - sPos );
	

	
	if( ys - curLine*13 < 0 )
		return( curLine + 1 );

	if( line <= 0 )
		strcat( lineStr, temp );

	return( curLine + 1 );
}





void CChatManager::DPrint( int x, int y, char* str, ... )
{
	
	if ( !g_cgv.displayDebugMessage ) return;

		if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;


	int len = strlen( str );		
	int fontSize = 12;				
	int px, py;						

	float tx1, ty1, tx2, ty2;
	float w, h;

	char buf[4096];
	va_list marker;

	memset(buf, 0, sizeof(buf));
	va_start( marker, str );
	vsprintf( buf, str, marker);

	str = buf;
	
	if( m_pEngVertices == NULL ) m_pEngVertices = (HANTEXTUREVERTEX *)m_engVex->Lock();

	px = x;
	py = y;

	wchar_t uniCode;
	unsigned char aniCode[3];

	
	while( *str )
	{	
		memset( aniCode, 0, sizeof( aniCode ) );
		aniCode[0] = *str;

		
		if( aniCode[0] == '\n' )
		{
			px = x;
			py += (int)m_engSize;
			str ++;
			continue;
		}

		
		if( aniCode[0] == ' ' )
		{
			px += fontSize / 2;
			str ++;
			continue;
		}
		
		
		if( IsDBCSLeadByte( (BYTE)aniCode[0] ) )
		{
			aniCode[1] = * ++str;
			aniCode[2] = 0;

			continue;
		}
		else
		{
			aniCode[1] = 0;
			MultiByteToWideChar( CP_ACP, MB_COMPOSITE, (char*)aniCode, -1, &uniCode, 1 );
		}
		if( uniCode >= 0x0000 && uniCode <= 0x0100 )
		{
			tx1 = (float)m_engTexCoord[uniCode].tx1 / 256.0f;
			ty1 = (float)m_engTexCoord[uniCode].ty1 / 256.0f;
			tx2 = (float)m_engTexCoord[uniCode].tx2 / 256.0f;
			ty2 = (float)m_engTexCoord[uniCode].ty2 / 256.0f;
		}
		
		else
		{
			str ++;
			continue;
		}

		
		w = (tx2 - tx1) * 256.0f;
		w *= m_engSize / 12.0f;
		h = (ty2 - ty1) * 256.0f;
		h *= m_engSize / 12.0f;

		
		m_pEngVertices[m_nEngVexCount].position = D3DXVECTOR4( (float)px - 0.5f, (float)py - 0.5f + h, 0.0f, 1.0f );
		m_pEngVertices[m_nEngVexCount].color = m_engColor;
		m_pEngVertices[m_nEngVexCount].tu = tx1;
		m_pEngVertices[m_nEngVexCount].tv = ty2;
		m_nEngVexCount ++;

		
		m_pEngVertices[m_nEngVexCount].position = D3DXVECTOR4( (float)px - 0.5f, (float)py - 0.5f, 0.0f, 1.0f );
		m_pEngVertices[m_nEngVexCount].color = m_engColor;
		m_pEngVertices[m_nEngVexCount].tu = tx1;
		m_pEngVertices[m_nEngVexCount].tv = ty1;
		m_nEngVexCount ++;

		
		m_pEngVertices[m_nEngVexCount].position = D3DXVECTOR4( (float)px - 0.5f + w, (float)py - 0.5f + h, 0.0f, 1.0f );
		m_pEngVertices[m_nEngVexCount].color = m_engColor;
		m_pEngVertices[m_nEngVexCount].tu = (float)tx2;
		m_pEngVertices[m_nEngVexCount].tv = (float)ty2;
		m_nEngVexCount ++;
		
		m_pEngVertices[m_nEngVexCount].position = D3DXVECTOR4( (float)px - 0.5f + w, (float)py - 0.5f, 0.0f, 1.0f );
		m_pEngVertices[m_nEngVexCount].color = m_engColor;
		m_pEngVertices[m_nEngVexCount].tu = (float)tx2;
		m_pEngVertices[m_nEngVexCount].tv = (float)ty1;
		m_nEngVexCount ++;

		
		m_pEngVertices[m_nEngVexCount].position = D3DXVECTOR4( (float)px - 0.5f + w, (float)py - 0.5f + h, 0.0f, 1.0f );
		m_pEngVertices[m_nEngVexCount].color = m_engColor;
		m_pEngVertices[m_nEngVexCount].tu = (float)tx2;
		m_pEngVertices[m_nEngVexCount].tv = (float)ty2;
		m_nEngVexCount ++;

		
		m_pEngVertices[m_nEngVexCount].position = D3DXVECTOR4( (float)px - 0.5f, (float)py - 0.5f, 0.0f, 1.0f );
		m_pEngVertices[m_nEngVexCount].color = m_engColor;
		m_pEngVertices[m_nEngVexCount].tu = (float)tx1;
		m_pEngVertices[m_nEngVexCount].tv = (float)ty1;
		m_nEngVexCount ++;

		px += (int)w;

		m_nEngStr += 2;

		str ++;
	}
	
	if( m_pEngVertices != NULL ) 
	{
		this->m_engVex->Unlock();
		m_pEngVertices = NULL;
	}

}


void CChatManager::HPrint( int x, int y, char* str, ... )
{
	char buf[4096];
	DWORD tempColor;
	va_list marker;

	memset( buf, 0, sizeof( buf ) );
	va_start( marker, str );
	vsprintf( buf, str, marker);

	str = buf;

	tempColor = m_hanColor;
	Output( x, y, str );	
}





void CChatManager::InitInput()
{
	for( int i = 0; i < CHAT_MAXINPUT; i ++ )
	{
		InitInputStr( i );
		m_inputState[i] = -1;
	}
	memset( m_clipBoard, 0, sizeof( chatMessage ) );
	memset( m_returnStr, 0 , sizeof( chatMessage ) );
	m_clipStartPos = -1;

	m_curIdx = -1;
}

void CChatManager::InitInputStr( int idx )
{
	memset( m_inputStr[idx], 0, sizeof( chatMessage ) );
	memset( m_combi, 0, sizeof( m_combi ) );

	m_csPos[idx] = 0;
	m_inputOutPos[idx] = 0;

	
	m_inputLastPositionByLine[idx] = 0;
}

void CChatManager::AddInputStr( int idx, char* str )
{
	if( str[0] == 0 ) return;
	strcat( m_inputStr[idx], str );
	int len = strlen( m_inputStr[idx] );
	m_csPos[idx] = len;
}











void CChatManager::SetInput( int x, int y, int bufferSize, int outputSize, int idx, int state, int outputLength, int lineFeed )
{
	InitInputStr( idx );
	
	m_ix[idx] = x;
	m_iy[idx] = y;
	m_inputSize[idx] = bufferSize;
	m_inputState[idx] = state;
	m_inputOutSize[idx] = outputSize;
	m_inputLineFeed[idx] = lineFeed;
	
	
	m_inputLastPositionByLine[idx] = 0;
	
	m_lineLimit[idx][0] = -1;
	m_lineLimit[idx][0] = -1;

	if ( outputLength == 0 )
		m_inputOutLength[idx] = outputSize * 5;
	else
		m_inputOutLength[idx] = outputLength;

	m_csCurTime = timeGetTime();
	m_csSaveTime = timeGetTime();
}


void CChatManager::ChangeInputMessage( int idx )
{
	m_curIdx = idx;
}

int CChatManager::GetCharState( char *str, int pos )
{
	int n = 0;
	while( n < pos )
	{
		
		if( ( str[n] == '\r' && str[n+1] == '\n' ) )
		{
			n += 2;
			if( n == pos ) return( 2 );
		}
		
		else if( IsDBCSLeadByte( (BYTE)str[n] ) ) 
		{
			n += 2;
			if( n == pos ) return( 1 );
		}
		
		else 
		{
			n ++;
			if( n == pos ) return( 0 );
		}
	}
	return( 1 );
}

void CChatManager::SetInputPosition( int idx, int x, int y )
{
	m_ix[idx] = x;
	m_iy[idx] = y;
}

void CChatManager::SetInputLine( int idx, int minLine, int maxLine )
{
	m_lineLimit[idx][0] = minLine;
	m_lineLimit[idx][1] = maxLine;
}

int CChatManager::GetInputLine( int idx )
{
	char *str = m_inputStr[m_curIdx];

	return GetViewLine( str, m_inputOutLength[idx] );
}

int CChatManager::GetCursorLine( int inputIdx )
{
	char charStr[3];

	char *str = m_inputStr[inputIdx];
	int searchEnd = m_csPos[inputIdx];

	int cx = 0, cy = 0;
	int idx = 0;
	int charLen;
	int line = 0;

	while( true )
	{
		if( idx >= searchEnd )
			break;

		if( str[idx] == '\r' )
		{
			cx = 0;
			cy += 13;
			idx+=2;
			line ++;
			
			continue;
		}

		if( IsDBCSLeadByte( (BYTE)str[idx] ) )
		{
			charStr[0] = str[idx];
			charStr[1] = str[idx+1];
			charStr[2] = 0;

			charLen = GetChatStrSizeChat( charStr );

			if( cx + charLen >= m_inputOutLength[inputIdx] )
			{
				cx = charLen;
				cy += 13;
				line ++;
			}
			else
				cx += charLen;

			idx += 2;
		}
		else
		{
			charStr[0] = str[idx];
			charStr[1] = 0;

			charLen = GetChatStrSizeChat( charStr );

			if( cx + charLen >= m_inputOutLength[inputIdx] )
			{
				cx = charLen;
				cy += 13;
				line ++;
			}
			else
				cx += charLen;

			idx ++;
		}
	}

	if( !m_inputType )
	{
		int combiLen = GetChatStrSizeChat( m_combi );

		if( cx + combiLen >= m_inputOutLength[inputIdx] )
		{
			cx = combiLen;
			cy += 13;
			line ++;
		}
		else
			cx += combiLen;
	}

	return line;
}

int CChatManager::GetViewLine( char *str, int length )
{
	char charStr[3];

	int cx = 0, cy = 0;
	int idx = 0;
	int charLen;
	int line = 1;

	while( str[idx] != '\0' )
	{
		if( str[idx] == '\r' )
		{
			cx = 0;
			cy += 13;
			line ++;
			idx+=2;
			
			continue;
		}

		if( IsDBCSLeadByte( (BYTE)str[idx] ) )
		{
			charStr[0] = str[idx];
			charStr[1] = str[idx+1];
			charStr[2] = 0;

			charLen = GetChatStrSizeChat( charStr );

			if( cx + charLen >= length )
			{
				cx = charLen;
				cy += 13;
				line ++;
			}
			else
				cx += charLen;

			idx += 2;
		}
		else
		{
			charStr[0] = str[idx];
			charStr[1] = 0;

			charLen = GetChatStrSizeChat( charStr );

			if( cx + charLen >= length )
			{
				cx = charLen;
				cy += 13;
				line ++;
			}
			else
				cx += charLen;

			idx ++;
		}
	}

	if( !m_inputType )
	{
		int combiLen = GetChatStrSizeChat( m_combi );

		if( cx + combiLen >= length )
		{
			cx = combiLen;
			cy += 13;
			line ++;
		}
		else
			cx += combiLen;
	}

	return line;

}

void CChatManager::AddChar( char ch )
{
	int len;
	char temp[4096];

	
	char str[2];
	str[0] = ch;
	str[1] = 0;

	
	len = strlen( m_inputStr[m_curIdx] );

	if( len >= m_inputSize[m_curIdx] || len + 3 >= CHAT_BUFFER_SIZE ) return;

	
	if( len == m_csPos[m_curIdx] )
	{
		m_inputStr[m_curIdx][len] = ch;
		m_inputStr[m_curIdx][len + 1] = 0;
		m_csPos[m_curIdx] ++;
		return;
	}
	
	memset( temp, 0, sizeof( temp ) );
	memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] );
	
	strcat( temp, str );		
	m_csPos[m_curIdx] ++;

	
	strcat( temp , m_inputStr[m_curIdx] + m_csPos[m_curIdx] - 1 );

	strcpy( m_inputStr[m_curIdx], temp );
}


void CChatManager::BackSpace()
{
	int len;
	char temp[4096];

	len = strlen( m_inputStr[m_curIdx] );

	memset( temp, 0, sizeof( temp ) );

	
	if( m_clipStartPos >= 0 )
	{
		
		if( m_clipStartPos < m_csPos[m_curIdx] )
		{
			memcpy( temp, m_inputStr[m_curIdx], m_clipStartPos );
			strcat( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] );
			m_csPos[m_curIdx] = m_clipStartPos;
			m_clipStartPos = -1;
			strcpy( m_inputStr[m_curIdx], temp );
			return;
		}
		else
		{
			memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] );
			strcat( temp, m_inputStr[m_curIdx] + m_clipStartPos );
			m_clipStartPos = -1;
			strcpy( m_inputStr[m_curIdx], temp );
			return;
		}
	}
	
	if( m_csPos[m_curIdx] > 0 )
	{
		
		
		if( GetCharState( m_inputStr[m_curIdx], m_csPos[m_curIdx] ) != 0 )
		{
			if( m_csPos[m_curIdx] > 2 )
			{
				memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] - 2 );
				strcat( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] );
			}
			else
			{
				strcpy( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] );
			}

			strcpy( m_inputStr[m_curIdx], temp );
			m_csPos[m_curIdx] -= 2;

			if( m_csPos[m_curIdx] < 0 )
				m_csPos[m_curIdx] = 0;

			return;
		}
		else
		{
			if( m_csPos[m_curIdx] > 1 )
			{
				memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] - 1 );
				strcat( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] );
			}
			else
			{
				strcpy( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] );
			}

			strcpy( m_inputStr[m_curIdx], temp );
			m_csPos[m_curIdx] --;
			return;
		}
	}
}


void CChatManager::Delete()
{
	int len;
	char temp[4096];
	len = strlen( m_inputStr[m_curIdx] );

	memset( temp, 0, sizeof( temp ) );

	if( m_clipStartPos >= 0 )
	{
		if( m_clipStartPos < m_csPos[m_curIdx] )
		{
			memcpy( temp, m_inputStr[m_curIdx], m_clipStartPos );
			strcat( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] );
			m_csPos[m_curIdx] = m_clipStartPos;
			m_clipStartPos = -1;
			strcpy( m_inputStr[m_curIdx], temp );
			return;
		}
		else
		{
			memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] );
			strcat( temp, m_inputStr[m_curIdx] + m_clipStartPos );
			m_clipStartPos = -1;
			strcpy( m_inputStr[m_curIdx], temp );
			return;
		}
	}

	if( m_csPos[m_curIdx] < len )
	{
		
		if( IsDBCSLeadByte( (BYTE)m_inputStr[m_curIdx][m_csPos[m_curIdx]] ) || 
			( m_inputStr[m_curIdx][m_csPos[m_curIdx]] == '\r' && m_inputStr[m_curIdx][m_csPos[m_curIdx]+1] == '\n' ) )
		{
			memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] );

			if( m_csPos[m_curIdx] < len - 2 )
				strcat( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] + 2 );

			strcpy( m_inputStr[m_curIdx], temp );
			return;
		}
		else
		{
			memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] );

			if( m_csPos[m_curIdx] < len - 1 )
				strcat( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] + 1);

			strcpy( m_inputStr[m_curIdx], temp );
			return;
		}
	}
}

void CChatManager::Copy()
{
	
	if( m_clipStartPos < 0 ) return;

	int m1, m2;
	memset( m_clipBoard, 0, sizeof( m_clipBoard ) );
	if( m_clipStartPos < m_csPos[m_curIdx] )
	{
		m1 = m_clipStartPos;
		m2 = m_csPos[m_curIdx];
	}
	else
	{
		m2 = m_clipStartPos;
		m1 = m_csPos[m_curIdx];
	}
	memcpy( m_clipBoard, m_inputStr[m_curIdx] + m1, m2 - m1 );
}

void CChatManager::Paste()
{
	char temp[4096];
	int len = strlen( m_inputStr[m_curIdx] );
	memset( temp, 0, sizeof( temp ) );

	int size = strlen( m_clipBoard );
	if( len + size >= m_inputSize[m_curIdx] ) size = m_inputSize[m_curIdx] - len;

	memcpy( temp, m_inputStr[m_curIdx], m_csPos[m_curIdx] );
	memcpy( temp + strlen( temp ), m_clipBoard, size );
	strcat( temp, m_inputStr[m_curIdx] + m_csPos[m_curIdx] );

	strcpy( m_inputStr[m_curIdx], temp );
	m_csPos[m_curIdx] += size;
}

void CChatManager::Left()
{
	
	if ( bIsActiveCompWnd )
		return;

	if( m_curIdx == INPUT_INDEX_DIALOGBOX )
	{
		if( ( m_inputStr[m_curIdx][0] == 0    || m_inputStr[m_curIdx][0] == '/'  || m_inputStr[m_curIdx][0] == '\"' ||
			  m_inputStr[m_curIdx][0] == '#'  || m_inputStr[m_curIdx][0] == '&'  || m_inputStr[m_curIdx][0] == '@' ) 
			  && m_inputStr[m_curIdx][1] == 0 ) return;

		if( m_inputStr[m_curIdx][0] == '\'' )
		{
			char szTemp[CHATSTRING];

			if( g_ifMng->m_dlgWin->m_whisperName[0] == 0 ) 
				sprintf( szTemp, "'" );
			else 
				sprintf( szTemp, "'%s ", g_ifMng->m_dlgWin->m_whisperName );

			if( !stricmp( szTemp, m_inputStr[m_curIdx] ) )
				return;
		}
	}
	
	int len;
	len = strlen( m_inputStr[m_curIdx] );
	if( m_csPos[m_curIdx] == 0 ) return;

	if( GetCharState( m_inputStr[m_curIdx], m_csPos[m_curIdx] ) != 0 )
		m_csPos[m_curIdx] -= 2;
	else m_csPos[m_curIdx] --;

	UpdateLastPositionByLine();
}

void CChatManager::Right()
{
	
	if ( bIsActiveCompWnd )
		return;

	int len;
	len = strlen( m_inputStr[m_curIdx] );
	if( m_csPos[m_curIdx] == len ) return;
	
	if( IsDBCSLeadByte( (BYTE)m_inputStr[m_curIdx][m_csPos[m_curIdx]] ) || 
		( m_inputStr[m_curIdx][m_csPos[m_curIdx]] == '\r' && m_inputStr[m_curIdx][m_csPos[m_curIdx]+1] == '\n' ) )

		m_csPos[m_curIdx] += 2;

	else m_csPos[m_curIdx] ++;

	UpdateLastPositionByLine();
}


void CChatManager::UpdateLastPositionByLine()
{
	
	int		i, charState;
	
	i = m_csPos[m_curIdx];
	if( i == 0 ) m_inputLastPositionByLine[m_curIdx] = 0;

	while( i > 0 )
	{
		charState = GetCharState( m_inputStr[m_curIdx], i );
		if( charState == 2 ) 
		{
			break;		
		}
		else if( charState == 1 ) i -= 2;
		else if( charState == 0 ) i --;
	}
	m_inputLastPositionByLine[m_curIdx]	=	m_csPos[m_curIdx] - i;
}






void CChatManager::Up()
{

	int x, y;
	GetCursorPosition( x, y );

	y -= 13/2;

	SetCursorFromMousePos( x, y );


}


void CChatManager::Down()
{
	int x, y;
	GetCursorPosition( x, y );

	y += 13 + 13/2;

	SetCursorFromMousePos( x, y );


}

void CChatManager::GetInputRenderPosition()
{
	int sPos, cLen, len;
	cLen = strlen( m_combi );
	char temp[4096], temp2[4096];

	if( m_csPos[m_curIdx] < m_inputOutPos[m_curIdx] )
	{
		m_inputOutPos[m_curIdx] = m_csPos[m_curIdx];
		return;
	}

	memset( temp, 0, sizeof(temp) );
	memset( temp2, 0, sizeof(temp2) );
	strcpy( temp2, m_inputStr[m_curIdx] );
	memcpy(temp, temp2 + m_inputOutPos[m_curIdx], m_csPos[m_curIdx] - m_inputOutPos[m_curIdx]);



	len = GetChatStrSizeChat( temp );



	while( len > m_inputOutLength[m_curIdx] )		
	{
		sPos = m_inputOutPos[m_curIdx];
		
		if( IsDBCSLeadByte( (BYTE)temp2[sPos] ) )
			m_inputOutPos[m_curIdx] += 2;
		else m_inputOutPos[m_curIdx] ++;

		if (m_inputOutPos[m_curIdx] > m_csPos[m_curIdx])
			m_inputOutPos[m_curIdx] = m_csPos[m_curIdx];

		memset( temp, 0, sizeof(temp) );
		memcpy(temp, m_inputStr[m_curIdx] + m_inputOutPos[m_curIdx], m_csPos[m_curIdx] - m_inputOutPos[m_curIdx]);

		if( !m_inputType ) strcat( temp, m_combi );

		len = GetChatStrSizeChat( temp );
	}
}

void CChatManager::SetInputOutSize( int idx )
{
	char temp[4096], temp2[4096];
	int i = 1, len;

	memset( temp2, 0, sizeof(temp2) );
	strcpy( temp2, m_inputStr[idx] );

	if( !m_inputType )
	{
		strcat( temp2, m_combi );
	}
	
	while(1)
	{
		memset( temp, 0, sizeof(temp) );
		memcpy( temp, temp2 + m_inputOutPos[idx], i );

		len = GetChatStrSizeChat( temp );
		
		if ( len > m_inputOutLength[idx] )
		{
			m_inputOutSize[idx] = i - 1;
			break;
		}
		else if (temp2[m_inputOutPos[idx] + i] == 0)
		{
			m_inputOutSize[idx] = i;
			break;
		}

		if (IsDBCSLeadByte((BYTE)temp2[m_inputOutPos[idx] + i]))
			i+= 2;
		else
			i++;
	}



}

void CChatManager::GetCursorPosition( int &x, int &y )
{
	char charStr[3];

	char *str = m_inputStr[m_curIdx];
	int searchEnd = m_csPos[m_curIdx];

	int cx = 0, cy = 0;
	int idx = 0;
	int charLen;

	while( true )
	{
		if( idx >= searchEnd )
			break;

		if( str[idx] == '\r' )
		{
			cx = 0;
			cy += 13;
			idx+=2;
			
			continue;
		}

		if( IsDBCSLeadByte( (BYTE)str[idx] ) )
		{
			charStr[0] = str[idx];
			charStr[1] = str[idx+1];
			charStr[2] = 0;

			charLen = GetChatStrSizeChat( charStr );

			if( cx + charLen >= m_inputOutLength[m_curIdx] )
			{
				cx = charLen;
				cy += 13;
			}
			else
				cx += charLen;

			idx += 2;
		}
		else
		{
			charStr[0] = str[idx];
			charStr[1] = 0;

			charLen = GetChatStrSizeChat( charStr );

			if( cx + charLen >= m_inputOutLength[m_curIdx] )
			{
				cx = charLen;
				cy += 13;
			}
			else
				cx += charLen;

			idx ++;
		}
	}

	if( !m_inputType )
	{
		int combiLen = GetChatStrSizeChat( m_combi );

		if( cx + combiLen >= m_inputOutLength[m_curIdx] )
		{
			cx = combiLen;
			cy += 13;
		}
		else
			cx += combiLen;
	}

	x = m_ix[m_curIdx] + cx;
	y = m_iy[m_curIdx] + cy;


}

void CChatManager::SetCursorFromMousePos( int mx, int my )
{
	char charStr[3];

	char *str = m_inputStr[m_curIdx];
	int searchEnd = strlen( m_inputStr[m_curIdx] );

	int cx = 0, cy = 0;
	int prevCx, prevCy;
	int prevIdx, idx = 0;
	int charLen;

	mx -= m_ix[m_curIdx];
	my -= m_iy[m_curIdx];
	
	while( true )
	{
		if( idx >= searchEnd )
			break;

		prevCx = cx;
		prevCy = cy;
		prevIdx = idx;

		if( str[idx] == '\r' )
		{
			cx = 0;
			cy += 13;
			idx += 2;

			if( my >= prevCy && my < prevCy + 13 )
			{
				if( mx >= prevCx && mx < m_inputOutLength[m_curIdx] )
				{
					m_csPos[m_curIdx] = prevIdx;
				}
			}
	
			continue;
		}

		if( IsDBCSLeadByte( (BYTE)str[idx] ) )
		{
			charStr[0] = str[idx];
			charStr[1] = str[idx+1];
			charStr[2] = 0;

			charLen = GetChatStrSizeChat( charStr );

			if( cx + charLen >= m_inputOutLength[m_curIdx] )
			{
				cx = charLen;
				cy += 13;
			}
			else
				cx += charLen;

			idx += 2;
		}
		else
		{
			charStr[0] = str[idx];
			charStr[1] = 0;

			charLen = GetChatStrSizeChat( charStr );

			if( cx + charLen >= m_inputOutLength[m_curIdx] )
			{
				cx = charLen;
				cy += 13;
			}
			else
				cx += charLen;

			idx ++;
		}

		if( my >= prevCy && my < prevCy + 13 )
		{
			if( mx >= prevCx && mx < prevCx + charLen/2 )
			{
				m_csPos[m_curIdx] = prevIdx;
			}
			else if( mx >= prevCx + charLen/2 && mx < prevCx + charLen )
			{
				m_csPos[m_curIdx] = idx;
			}
		}
	}

	if( my >= cy && my < cy + 13 )
	{
		if( mx >= cx && mx < m_inputOutLength[m_curIdx] )
		{
			m_csPos[m_curIdx] = idx;
		}
	}

}

void CChatManager::RenderInputFrame( int inputIdx, bool displayCursor )
{
	char csTemp[4096];
	char temp[4096], temp2[4096];
	int x, y, outputLen;

	SetOutputType( CHAT_OUTPUT_NORMAL );
	m_csCurTime = timeGetTime();
	SetFontColor( 0xffffffff );

	int i = inputIdx;

	
	if( m_inputState[i] == 1 )
	{
		if( i != m_curIdx )
		{
			
			if( !m_inputLineFeed[i] )
			{
				SetInputOutSize(i);
				
				memset( temp, 0, sizeof( temp ) );
				memcpy( temp, m_inputStr[i], m_csPos[i] );
				strcat( temp, m_inputStr[i] + m_csPos[i] );
				
				memset( temp2, 0, sizeof( temp2 ) );
				memcpy( temp2, temp + m_inputOutPos[i], m_inputOutSize[i] );
				Output( m_ix[i], m_iy[i], temp2 );

			}
			else
			{
				int cx = m_ix[i], cy = m_iy[i];

				DrawTextMail( m_ix[i], m_iy[i], m_inputStr[i], m_inputOutLength[i], 13, CHAT_ARRANGE_NONE, i );
			}
		}
		else
		
		{
			if( !m_inputLineFeed[i] )
			{
				GetInputRenderPosition();

				SetInputOutSize(i);
					
				memset( temp, 0, sizeof( temp ) );
				memcpy( temp, m_inputStr[i], m_csPos[i] );
				
				outputLen = m_inputOutSize[i];
				
				if( !m_inputType )
				{
					strcat( temp, m_combi );
					outputLen += strlen( m_combi );
				}
				
				strcat( temp, m_inputStr[i] + m_csPos[i] );
				memset( temp2, 0, sizeof( temp2 ) );
				memcpy( temp2, temp + m_inputOutPos[i], outputLen );
				Output( m_ix[i], m_iy[i], temp2 );
				
				if( m_csCurTime - m_csSaveTime < 750 )
				{
					memset( csTemp, 0, sizeof( csTemp ) );
					memcpy( csTemp, temp2, m_csPos[i] - m_inputOutPos[i] );
					
					if( !m_inputType )
					{
						strcat( csTemp, m_combi );
					}
					
					x = GetChatStrSizeChat( csTemp ) + m_ix[i];

					if( displayCursor )
						Output( x-3 , m_iy[i], "|" );
				}
			}
			else
			{
				memset( temp, 0, sizeof( temp ) );
				memcpy( temp, m_inputStr[i], m_csPos[i] );
				
				outputLen = m_inputOutSize[i];
				
				if( !m_inputType )
				{
					strcat( temp, m_combi );
					outputLen += strlen( m_combi );
				}
				
				strcat( temp, m_inputStr[i] + m_csPos[i] );
				int hak = strlen( temp );
				memset( temp2, 0, sizeof( temp2 ) );
				memcpy( temp2, temp + m_inputOutPos[i], outputLen );

				DrawTextMail( m_ix[i], m_iy[i], temp2, m_inputOutLength[i], 13, CHAT_ARRANGE_NONE, i );
				
				if( m_csCurTime - m_csSaveTime < 750 )
				{
					GetCursorPosition( x, y );
					
					if( displayCursor )
						Output( x-3 , y, "|" );
				}
			}
			
			if(  m_csCurTime - m_csSaveTime > 1500 ) m_csSaveTime = m_csCurTime;
		}
	}
	else if( m_inputState[i] == 2 )
	{
		if( i != m_curIdx )
		{
			SetInputOutSize(i);
			memset( temp, 0, sizeof( temp ) );
			memcpy( temp, m_inputStr[i], m_csPos[i] );
			strcat( temp, m_inputStr[i] + m_csPos[i] );
			memset( temp2, 0, sizeof( temp2 ) );
			memcpy( temp2, temp + m_inputOutPos[i], m_inputOutSize[i] );
			memset( temp2, '*', strlen( temp2 ) );
			Output( m_ix[i], m_iy[i], temp2 );
		}
		else
		
		{
			GetInputRenderPosition();
			SetInputOutSize(i);
			memset( temp, 0, sizeof( temp ) );
			memcpy( temp, m_inputStr[i], m_csPos[i] );
			outputLen = m_inputOutSize[i];
			if( !m_inputType )
			{
				strcat( temp, m_combi );
				outputLen += strlen( m_combi );
			}

			strcat( temp, m_inputStr[i] + m_csPos[i] );

			memset( temp2, 0, sizeof( temp2 ) );
			memcpy( temp2, temp + m_inputOutPos[i], outputLen );
			memset( temp2, '*', strlen( temp2 ) );
			Output( m_ix[i], m_iy[i], temp2 );
		
			if( m_csCurTime - m_csSaveTime < 750 )
			{
				memset( csTemp, 0, sizeof( csTemp ) );
				memcpy( csTemp, temp2, m_csPos[i] - m_inputOutPos[i] );
				
				if( !m_inputType )
				{
					strcat( csTemp, m_combi );
				}
			
				x = GetChatStrSizeChat( csTemp ) + m_ix[i];
				Output( x-3 , m_iy[i], "|" );
			}
			if(  m_csCurTime - m_csSaveTime > 1500 ) m_csSaveTime = m_csCurTime;
		}
	}
}

void CChatManager::RenderInput()
{
	char csTemp[4096];
	char temp[4096], temp2[4096];
	int x, y, outputLen;

	SetOutputType( CHAT_OUTPUT_NORMAL );
	m_csCurTime = timeGetTime();
	SetFontColor( 0xffffffff );

	
	for( int i = 0; i < CHAT_MAXINPUT; i ++ )
	{
		if( m_inputState[i] == 1 )
		{
			if( i != m_curIdx )
			{
				
				if( !m_inputLineFeed[i] )
				{
					SetInputOutSize(i);
					
					memset( temp, 0, sizeof( temp ) );
					memcpy( temp, m_inputStr[i], m_csPos[i] );
					strcat( temp, m_inputStr[i] + m_csPos[i] );
					
					memset( temp2, 0, sizeof( temp2 ) );
					memcpy( temp2, temp + m_inputOutPos[i], m_inputOutSize[i] );
					Output( m_ix[i], m_iy[i], temp2 );
				}
				else
				{
					int cx = m_ix[i], cy = m_iy[i];
					TextOut( m_ix[i], m_iy[i], m_inputStr[i], m_inputOutLength[i], 13, cx, cy );
				}
			}
			else
			
			{
				if( !m_inputLineFeed[i] )
				{
					GetInputRenderPosition();
					SetInputOutSize(i);
					
					memset( temp, 0, sizeof( temp ) );
					memcpy( temp, m_inputStr[i], m_csPos[i] );
					
					outputLen = m_inputOutSize[i];
					
					if( !m_inputType )
					{
						strcat( temp, m_combi );
						outputLen += strlen( m_combi );
					}
					
					strcat( temp, m_inputStr[i] + m_csPos[i] );
					memset( temp2, 0, sizeof( temp2 ) );
					memcpy( temp2, temp + m_inputOutPos[i], outputLen );
					Output( m_ix[i], m_iy[i], temp2 );
					
					if( m_csCurTime - m_csSaveTime < 750 )
					{
						memset( csTemp, 0, sizeof( csTemp ) );
						memcpy( csTemp, temp2, m_csPos[i] - m_inputOutPos[i] );
						
						if( !m_inputType )
						{
							strcat( csTemp, m_combi );
						}
						
						x = GetChatStrSizeChat( csTemp ) + m_ix[i];
						Output( x-3 , m_iy[i], "|" );
					}
				}
				else
				{
					memset( temp, 0, sizeof( temp ) );
					memcpy( temp, m_inputStr[i], m_csPos[i] );
					
					outputLen = m_inputOutSize[i];
					
					if( !m_inputType )
					{
						strcat( temp, m_combi );
						outputLen += strlen( m_combi );
					}
					
					strcat( temp, m_inputStr[i] + m_csPos[i] );
					memset( temp2, 0, sizeof( temp2 ) );
					memcpy( temp2, temp + m_inputOutPos[i], outputLen );

					int cx = m_ix[i], cy = m_iy[i];
					TextOut( m_ix[i], m_iy[i], temp2, m_inputOutLength[i], 13, cx, cy );
					
					if( m_csCurTime - m_csSaveTime < 750 )
					{
						memset( csTemp, 0, sizeof( csTemp ) );
						memcpy( csTemp, temp2, m_csPos[i] - m_inputOutPos[i] );
						
						if( !m_inputType )
						{
							strcat( csTemp, m_combi );
						}
						
						cx = m_ix[i];
						cy = m_iy[i];
						TextOut( m_ix[i], m_iy[i], csTemp, m_inputOutLength[i], 13, cx, cy, true );
						
						x = cx;
						y = cy;
						
						Output( x-3 , y, "|" );
					}
				}
				

				if(  m_csCurTime - m_csSaveTime > 1500 ) m_csSaveTime = m_csCurTime;
			}
		}
		else if( m_inputState[i] == 2 )
		{
			if( i != m_curIdx )
			{
				SetInputOutSize(i);

				memset( temp, 0, sizeof( temp ) );
				memcpy( temp, m_inputStr[i], m_csPos[i] );

				strcat( temp, m_inputStr[i] + m_csPos[i] );
				memset( temp2, 0, sizeof( temp2 ) );
				memcpy( temp2, temp + m_inputOutPos[i], m_inputOutSize[i] );

				memset( temp2, '*', strlen( temp2 ) );

				Output( m_ix[i], m_iy[i], temp2 );
			}
			else
			
			{
				GetInputRenderPosition();
				SetInputOutSize(i);

				memset( temp, 0, sizeof( temp ) );
				memcpy( temp, m_inputStr[i], m_csPos[i] );

				outputLen = m_inputOutSize[i];

				if( !m_inputType )
				{
					strcat( temp, m_combi );
					outputLen += strlen( m_combi );
				}

				strcat( temp, m_inputStr[i] + m_csPos[i] );

				memset( temp2, 0, sizeof( temp2 ) );
				memcpy( temp2, temp + m_inputOutPos[i], outputLen );

				memset( temp2, '*', strlen( temp2 ) );
				Output( m_ix[i], m_iy[i], temp2 );

				
				if( m_csCurTime - m_csSaveTime < 750 )
				{
					memset( csTemp, 0, sizeof( csTemp ) );
					memcpy( csTemp, temp2, m_csPos[i] - m_inputOutPos[i] );
					
					if( !m_inputType )
					{
						strcat( csTemp, m_combi );
					}
				
					x = GetChatStrSizeChat( csTemp ) + m_ix[i];
					Output( x-3 , m_iy[i], "|" );
				}

				if(  m_csCurTime - m_csSaveTime > 1500 ) m_csSaveTime = m_csCurTime;
			}
		}
	}
}

void CChatManager::Render()
{
	 
	if	(	! g_pApp ->m_renderEnabled	)
		return;
	
	int i;
	int zInfo = false;

	if( m_renderFlag == 2 ) return;
	


	SetRenderState();

	m_pd3dDevice->SetTexture( 0, m_engTex );

	m_pd3dDevice->SetStreamSource( 0, this->m_engVex->GetD3DVertexBuffer(), sizeof(HANTEXTUREVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_HANVERTEX );

	 
	if	(	m_nEngStr	>	0	)
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_nEngStr );

	m_nEngStr = 0;
	m_nEngVexCount = 0;

	MakeChatTexture();

	
	m_pd3dDevice->SetTexture( 0, NULL );
	m_pd3dDevice->SetStreamSource( 0, this->m_reverseVex->GetD3DVertexBuffer(), sizeof(HANTEXTUREVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_HANVERTEX );

	for( i = 0; i < m_numReverse / 4; i ++ )
	{
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, i * 4, 2 );
	}
	m_numReverse = 0;

	
	m_pd3dDevice->SetTexture( 0, m_hanTex );
	
	m_pd3dDevice->SetStreamSource( 0, this->m_hanVex->GetD3DVertexBuffer(), sizeof(HANTEXTUREVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_HANVERTEX );

	for( i = 0; i < m_nHanVexCount / 4; i ++ )
	{
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, i * 4, 2 );
	}
	m_nHanVexCount = 0;

	UpdateEmptyTexture();



}




int CChatManager::GetChatStrSize( char *str )
{
	int texIdx;
	int tx1, tx2, ty1, ty2;
	int rx = 0, maxRX = 0;
	bool en;
	
	wchar_t uniCode;
	unsigned char aniCode[3];
	
	while( *str )
	{		
		memset( aniCode, 0, sizeof( aniCode ) );
		
		
		aniCode[0] = *str;
		
		tx1 = ty1 = tx2 = ty2 = 0;
		
		if( aniCode[0] == '\n' )
		{
			if( rx > maxRX ) 
			{
				maxRX = rx;
				rx = 0;
			}
			str ++;
			continue;
		}
		else if( aniCode[0] == '&' )
		{
			
			if ( aniCode[1] == 'l' ) 
			{
				
				str ++;
				str ++;
				str ++;
			}

			str ++;
			str ++;
			continue;
		}
		
		
		if( aniCode[0] == ' ' )
		{
			tx1 = 0;
			tx2 = 6;
			rx += tx2 - tx1;
			str ++;
			continue;
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
		{
			str ++;
			continue;
		}
		
		tx1 = m_hanTexCoord[texIdx].tx1;
		ty1 = m_hanTexCoord[texIdx].ty1;
		tx2 = m_hanTexCoord[texIdx].tx2;
		ty2 = m_hanTexCoord[texIdx].ty2;

		rx += tx2 - tx1;
		str ++;
	}
	if( rx > maxRX ) maxRX = rx;

	return( (int)((float)maxRX * m_hanSize / 12.0f) );
}



int CChatManager::GetChatStrSize2( int xs, int ys, char *str )
{
	int texIdx;
	int tx1, tx2, ty1, ty2;
	int rx = 0, ry = 0;
	bool en;
	
	wchar_t uniCode;
	unsigned char aniCode[3];
	
	while( *str )
	{		
		memset( aniCode, 0, sizeof( aniCode ) );
		
		aniCode[0] = *str;
		
		tx1 = ty1 = tx2 = ty2 = 0;
		texIdx = -1;
		
		if( aniCode[0] == '&' )
		{
			aniCode[1] = * ++str;
			if( aniCode[1] == 'n' )
			{
				ry += 13;
				rx = 0;
			}
			else if ( aniCode[1] == 'l' ) 
			{
				
				str ++;
				str ++;
				str ++;
			}
		}
		
		else if( aniCode[0] == ' ' )
		{
			tx1 = 0;
			tx2 = 6;
		}
		
		else 
		{
			
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
		}
		if( texIdx >= 0 )
		{
			tx1 = m_hanTexCoord[texIdx].tx1;
			ty1 = m_hanTexCoord[texIdx].ty1;
			tx2 = m_hanTexCoord[texIdx].tx2;
			ty2 = m_hanTexCoord[texIdx].ty2;
		}

		str ++;
		
		rx += tx2 - tx1;

		if( rx > xs )
		{
			rx = 0;
			ry += 13;
		}
		
		if( ry > ys ) 
			return 0;
	}

	return 1;
}



int CChatManager::GetChatStrSizeChat( char *str )
{
	int texIdx;
	int tx1, tx2, ty1, ty2;
	int rx = 0, maxRX = 0;
	bool en;
	
	wchar_t uniCode;
	unsigned char aniCode[3];
	
	while( *str )
	{		
		memset( aniCode, 0, sizeof( aniCode ) );
		
		aniCode[0] = *str;
		
		tx1 = ty1 = tx2 = ty2 = 0;
		
		if( aniCode[0] == '\n' || aniCode[0] == '\r' )
		{
			if( rx > maxRX ) 
			{
				maxRX = rx;
				rx = 0;
			}
			str ++;
			continue;
		}

		
		if( aniCode[0] == ' ' )
		{
			tx1 = 0;
			tx2 = 6;
			rx += tx2 - tx1;
			str ++;
			continue;
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
		{
			str ++;
			continue;
		}
		
		tx1 = m_hanTexCoord[texIdx].tx1;
		ty1 = m_hanTexCoord[texIdx].ty1;
		tx2 = m_hanTexCoord[texIdx].tx2;
		ty2 = m_hanTexCoord[texIdx].ty2;

		rx += tx2 - tx1;
		str ++;
	}
	if( rx > maxRX ) maxRX = rx;

	return( (int)((float)maxRX * m_hanSize / 12.0f) );
}

