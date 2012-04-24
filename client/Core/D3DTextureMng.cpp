#include <stdio.h>
#include "core_def.h"
#include "d3dtexturemng.h"
#include "d3ddevice.h"
#include "filemng.h"
#include "ResourceCounter.h"
#include "../DebuggingLog/VUtil.h"
#include <assert.h>

CD3DTextureMng g_texMng;

CTextureResource::CTextureResource()
{
	Clear();
	m_owner			=	0;
	m_loaded		=	0;
	m_width			=	0;
    m_height		=	0;
    m_size			=	0;
    m_bits			=	0;
	m_mipLevel		=	0;
    m_hasAlpha		=	0;
	m_minFilter		=	0;
	m_magFilter		=	0;
	m_mipFilter		=	0;
	m_addrMode		=	0;
}

CTextureResource::~CTextureResource()
{
	Dispose();
}

bool CTextureResource::UploadTexture()
{
	CTextureResource	*texture	=	this;
	
	HRESULT hr;
    
	LPDIRECT3DDEVICE8	pd3dDevice	=	g_d3dDevice.GetD3DDevice();
	D3DFORMAT			format		=	( texture->m_bits == 32 ) ? D3DFMT_A8R8G8B8 : D3DFMT_A4R4G4B4;
	D3DXIMAGE_INFO		info;
	D3DSURFACE_DESC		desc;
	D3DLOCKED_RECT		rect;
	D3DXIMAGE_INFO		imageinfo;
	
	hr	=	D3DXGetImageInfoFromFileInMemory( texture->m_data, texture->m_size, &imageinfo );
	if( FAILED( hr ) )
	{ 
		texture->Dispose();
		return false;
	}
	
	if( imageinfo.ImageFileFormat == D3DXIFF_DDS )
	{
		
		hr = D3DXCreateTextureFromFileInMemoryEx(
			pd3dDevice ,
			texture->m_data ,
			texture->m_size ,
			imageinfo.Width ,
			imageinfo.Height ,
			imageinfo.MipLevels ,
			0 , 
			imageinfo.Format , 
			D3DPOOL_MANAGED ,
			D3DX_FILTER_TRIANGLE ,
			D3DX_FILTER_NONE ,
			0 ,
			&info ,
			NULL ,
			&texture->m_d3dTex );
	}
	else
	{
		hr = D3DXCreateTextureFromFileInMemoryEx(
			pd3dDevice ,
			texture->m_data,
			texture->m_size,
			( texture->m_width ) ? texture->m_width : D3DX_DEFAULT ,
			( texture->m_height ) ? texture->m_height : D3DX_DEFAULT ,
			( texture->m_mipLevel > 1 ) ? D3DX_DEFAULT : 1 ,
			0 , 
			format , 
			D3DPOOL_MANAGED ,
			D3DX_FILTER_TRIANGLE ,
			D3DX_FILTER_BOX ,
			0 ,
			&info ,
			NULL ,
			&texture->m_d3dTex );
	}
	
	if( FAILED( hr ) )
	{ 
		texture->Dispose();
		return false;
	}
    
	if( texture->m_hasAlpha && info.Format != D3DFMT_A8R8G8B8 ) 
	{
		if( SUCCEEDED( texture->m_d3dTex->GetLevelDesc( 0 , &desc ) ) ) 
		{
			if( SUCCEEDED( texture->m_d3dTex->LockRect( 0 , &rect , 0 , 0 ) ) ) 
			{
				BYTE* d = (BYTE*) rect.pBits;
				for( DWORD y = 0 ; y < desc.Height ; y++ ) 
				{
					DWORD *dst = (DWORD*) d;
					BYTE  *s = d;
					for( DWORD x = 0 ; x < desc.Width ; x++ ) {
						DWORD r , g , b , a;
						
						r =(DWORD) s[2];
						g =(DWORD) s[1];
						b =(DWORD) s[0];
						a =(r+g+b) / 3 ;
						dst[x] = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
						s += 4;
					}
					d += rect.Pitch;
				}
				texture->m_d3dTex->UnlockRect(0);
			}
		}
	}
	
	GTH_SAFE_FREE( texture->m_data );

	return true;
}

bool CTextureResource::Create( char* fname, int iSize )
{
	m_data		=	g_fileMng.ReadFileFromPack( m_name );
	m_size		=	iSize;

	if( !m_data ) return false;

	UploadTexture();

    return true;
}

bool CTextureResource::Recreate()
{
	m_data		=	g_fileMng.ReadFileFromPack( m_name );

	if( !m_data ) return false;

	UploadTexture();

    return true;
}

CD3DTextureMng::CD3DTextureMng()
{
	m_texQuality = GTH_TEXTURE_QUALITY_HIGH;
	m_numTextures = 0;

	
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus( &memStatus );
	
	if( memStatus.dwTotalPhys < 267870208 )
	{
		m_textureRscSize = 31457280;
	}
	else
	{
		m_textureRscSize = 31457280 * 2;
	}

	m_resManager.Create( m_textureRscSize );
}

CD3DTextureMng::~CD3DTextureMng()
{
}

void CD3DTextureMng::Release()
{
	int i;

	for( i = 0 ; i < m_numTextures ; i++ )
	{
		m_textures[i].Dispose();
		m_resManager.RemoveResource( (RHANDLE)&m_textures[i] );
	}
	m_numTextures = 0;

	
	m_resManager.Create( m_textureRscSize );
}


void CD3DTextureMng::Release( int owner )
{
	int i;

	for( i = 0 ; i < m_numTextures ; i++ )
	{
		if( m_textures[ i ].m_owner & owner )
		{
			m_textures[ i ].m_owner &= ~owner;
			if( !m_textures[ i ].m_owner )
			{
				m_textures[i].Dispose();
				m_resManager.RemoveResource( (RHANDLE)&m_textures[i] );
			}
		}
	}
}


void CD3DTextureMng::Release( char *name , int owner )
{
	int i;

	for( i = 0 ; i < m_numTextures ; i++ )
	{
		if( !stricmp( m_textures[ i ].m_name , name ) )
		{
			m_textures[ i ].m_owner &= ~owner;
			if( !m_textures[ i ].m_owner )
			{
				m_textures[i].Dispose();
				m_resManager.RemoveResource( (RHANDLE)&m_textures[i] );
			}
			return;
		}
	}
}


void CD3DTextureMng::Release( int idxTex , int owner )
{
	m_textures[idxTex].m_owner &= ~owner;
	if( !m_textures[idxTex].m_owner )
	{
		m_textures[idxTex].Dispose();
		m_resManager.RemoveResource( (RHANDLE)&m_textures[idxTex] );
	}
}


void CD3DTextureMng::Restore()
{
	CTextureResource	*texture;
	char		texName[256];
	char		fName[256];
	char		*ext;

	g_fileMng.SetResourceManager( &m_resManager );

	for( int i = 0 ; i < m_numTextures ; i++ )
	{
		texture = &m_textures[ i ];

		
		strcpy( texName , texture->m_name );

		strcpy( fName, texName );
		
		if( ext = CFileMng::GetFileExt( fName ) )
			strcpy( ext , "dds" );
		else
			strcat( fName , ".dds" );
		
		strcpy( texture->m_name, fName );
		
		if( !g_fileMng.RegisterFileFromPack( texture->m_name, (RHANDLE)texture, texture, RES_INIT_NOTCREATE ) )
		{
			strcpy( fName , texName );
			if( ext = CFileMng::GetFileExt( fName ) )
				strcpy( ext , "jpg" );
			else
				strcat( fName , ".jpg" );
			
			strcpy( texture->m_name, fName );
			
			if( !g_fileMng.RegisterFileFromPack( texture->m_name, (RHANDLE)texture, texture, RES_INIT_NOTCREATE ) )
			{
				strcpy( fName , texName );
				if( ext = CFileMng::GetFileExt( fName ) )
					strcpy( ext , "tga" );
				else
					strcat( fName , ".tga" );
				
				strcpy( texture->m_name, fName );
				
				if( !g_fileMng.RegisterFileFromPack( texture->m_name, (RHANDLE)texture, texture, RES_INIT_NOTCREATE ) ) 
				{

					continue;
				}
			}
		}
	}
}


int CD3DTextureMng::Search( char *name , int owner )
{
	int i;
	char	*fn1, *fn2;
	char	comparen1[256], comparen2[256];

	for( i = 0 ; i < m_numTextures ; i++ )
	{
		strcpy( comparen1, m_textures[ i ].m_name );

		if( m_texQuality == GTH_TEXTURE_QUALITY_LOW && 
			( owner != OWNER_TYPE_INTERFACE && owner != OWNER_TYPE_BSP_LIGHTMAP ) )
		{
			sprintf( comparen2 , "low_" );
			strcat( comparen2 , name );
		}
		else
		{
			strcpy( comparen2, name );
		}

		fn1 = CFileMng::GetFileNameOnly( comparen1 );
		fn2 = CFileMng::GetFileNameOnly( comparen2 );

		if( !stricmp( fn1, fn2 ) )
		{
			m_textures[ i ].m_owner |= owner;
			return i;
		}
	}

	return -1;
}

LPDIRECT3DTEXTURE8 CD3DTextureMng::GetD3DTexture( int texRef )
{
	CTextureResource	*texture	=	&m_textures[texRef];
	bool				bIsDisposed	=	texture->IsDisposed();

	if( m_resManager.GetResource( (RHANDLE)texture ) == NULL ) 
	{

		return NULL;	
	}

	if( bIsDisposed )
	{

	}

	return m_textures[texRef].m_d3dTex;
}

void CD3DTextureMng::AddWhiteImage()
{
	memset( m_whiteImage , 0xFF , sizeof( DWORD ) * 16 * 16 );
	m_whiteImageRef = Add( "whiteimage" , OWNER_TYPE_BSP | OWNER_TYPE_MODEL | OWNER_TYPE_CHARACTER 
		                 , 16 * 16 * 4 , (BYTE *) m_whiteImage , 32 , false , 1 , true , 16 , 16 );
}




int CD3DTextureMng::Add( char *name , int owner , int size , BYTE *data 
						, int bits , int wrap , int mipLevel ,  int hasAlpha
						, int width , int height )
{
	assert(m_numTextures <= MAX_SIZE_TEXTURE);

	int		i;
	char	*fn1, *fn2;
	char	comparen1[256], comparen2[256];
	CTextureResource *texture;

	if(m_numTextures >= MAX_SIZE_TEXTURE)
		int count = 0;

	for( i = 0 ; i < m_numTextures ; i++ )
	{
		strcpy( comparen1, m_textures[ i ].m_name );

		if( m_texQuality == GTH_TEXTURE_QUALITY_LOW && 
			( owner != OWNER_TYPE_INTERFACE && owner != OWNER_TYPE_BSP_LIGHTMAP ) )
		{
			sprintf( comparen2 , "low_" );
			strcat( comparen2 , name );
		}
		else
		{
			strcpy( comparen2, name );
		}

		fn1 = CFileMng::GetFileNameOnly( comparen1 );
		fn2 = CFileMng::GetFileNameOnly( comparen2 );

		if( !stricmp( fn1, fn2 ) )
		{
			m_textures[ i ].m_owner |= owner;
			return i;
		}
	}

	texture = &m_textures[ m_numTextures ];
	texture->m_owner = owner;
	texture->m_data = data;
	texture->m_size = size;
	texture->m_width = width;
	texture->m_height = height;
	texture->m_hasAlpha = hasAlpha;
	texture->m_mipLevel = mipLevel;
	texture->m_bits = bits;
	texture->m_minFilter = D3DTEXF_LINEAR;
	texture->m_magFilter = D3DTEXF_LINEAR;
	texture->m_mipFilter = ( mipLevel > 1 ) ? D3DTEXF_POINT : D3DTEXF_NONE;
	texture->m_addrMode = ( wrap ) ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP;


	if( m_texQuality == GTH_TEXTURE_QUALITY_LOW && 
		( owner != OWNER_TYPE_INTERFACE && owner != OWNER_TYPE_BSP_LIGHTMAP ) )
	{
		sprintf( texture->m_name, "low_" );
		strcat( texture->m_name, name );
	}
	else
	{
		strcpy( texture->m_name, name );
	}

	texture->m_d3dTex = NULL;

	m_numTextures++;
	
	
	return m_numTextures - 1;
}
