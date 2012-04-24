



#if !defined(AFX_D3DTEXTUREMNG_H__B5840EDF_D75E_45F5_9D27_E8C7AC93F289__INCLUDED_)
#define AFX_D3DTEXTUREMNG_H__B5840EDF_D75E_45F5_9D27_E8C7AC93F289__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <d3dx8.h>
#include "core_def.h"
#include "ResManager.h"

 



#define MAX_SIZE_TEXTURE   2902


#define MAX_SIZE_FILENAME  128


class CTextureResource : public BaseResource
{
public :
	char		m_name[GTH_MAX_SIZE_FILENAME];
	int			m_owner;
	int			m_loaded;
	int			m_width;
    int			m_height;
    int			m_size;
    int			m_bits;
	int			m_mipLevel;
    int			m_hasAlpha;
	BYTE		*m_data;
    
	DWORD		m_minFilter;
	DWORD		m_magFilter;
	DWORD		m_mipFilter;
	DWORD		m_addrMode;
	
	LPDIRECT3DTEXTURE8		m_d3dTex;

	CTextureResource();
	virtual ~CTextureResource();

	bool			UploadTexture();

	
	
	void			Clear()
	{
		m_d3dTex		=	NULL;
		m_data			=	NULL;
	}
	
	bool			Create( char *fname, int iSize );
	bool			Register( char *fname, int iSize )
	{	
		m_size	=	iSize;	
		return true ;
	}

	virtual bool	Recreate();
	
	virtual void	Dispose()						
	{	
		GTH_SAFE_FREE( m_data );
		GTH_SAFE_RELEASE( m_d3dTex );
	}
	size_t			GetSize()						
	{	
		if( m_d3dTex ) return m_size;
		else return 0;
	}
	bool			IsDisposed()
	{
		if( m_d3dTex ) return false;
		else return true;
	}
};


#define  GTH_TEXTURE_QUALITY_LOW  1
#define  GTH_TEXTURE_QUALITY_HIGH 0

class CD3DTextureMng  
{
public:
	         CD3DTextureMng();
	virtual ~CD3DTextureMng();

	
	CTextureResource	m_textures[ MAX_SIZE_TEXTURE ];
	CResManager			m_resManager;

	int			m_numTextures;

	DWORD		m_whiteImage[ 16 * 16 * 4 ];
	int			m_whiteImageRef;

	int			m_texQuality;
	size_t		m_nTextureMaxMemory;

	
	unsigned long m_textureRscSize;

	void  SetTextureQuality( int quality ) { m_texQuality = quality; }

	LPDIRECT3DTEXTURE8 GetD3DTexture( int texRef );

	void Release();
	void Release( int owner );
	void Release( char *name , int owner );
	void Release( int idxTex , int owner );
	void Restore();

	int  GetWhiteImageRef() { return m_whiteImageRef; }
	void AddWhiteImage();
	int  Search( char *name , int owner );
	int  Add( char *name , int owner , int size = 0 , BYTE *data = NULL
						, int bits = 32 , int wrap = true , int mipLevel = 4 ,  int hasAlpha = false 
						, int width = 0 , int height = 0 );
};

extern CD3DTextureMng g_texMng;

#endif 
