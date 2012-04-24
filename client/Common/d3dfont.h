




#ifndef D3DFONT_H
#define D3DFONT_H
#include <tchar.h>
#include <D3D8.h>



#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004


#define D3DFONT_CENTERED    0x0001
#define D3DFONT_TWOSIDED    0x0002
#define D3DFONT_FILTERED    0x0004








class CD3DFont
{
    TCHAR   m_strFontName[80];            
    DWORD   m_dwFontHeight;
    DWORD   m_dwFontFlags;

    LPDIRECT3DDEVICE8       m_pd3dDevice; 
    LPDIRECT3DTEXTURE8      m_pTexture;   
    LPDIRECT3DVERTEXBUFFER8 m_pVB;        
    DWORD   m_dwTexWidth;                 
    DWORD   m_dwTexHeight;
    FLOAT   m_fTextScale;
    FLOAT   m_fTexCoords[128-32][4];

    
    DWORD   m_dwSavedStateBlock;
    DWORD   m_dwDrawTextStateBlock;

public:

	HRESULT CD3DFont::DrawTexture( FLOAT sx, FLOAT sy, DWORD dwColor, DWORD dwFlags );
	void CD3DFont::SetRenderState();

    
    HRESULT DrawText( FLOAT x, FLOAT y, DWORD dwColor, 
                      TCHAR* strText, DWORD dwFlags=0L );
    HRESULT DrawTextScaled( FLOAT x, FLOAT y, FLOAT z, 
                            FLOAT fXScale, FLOAT fYScale, DWORD dwColor, 
                            TCHAR* strText, DWORD dwFlags=0L );
    HRESULT Render3DText( TCHAR* strText, DWORD dwFlags=0L );
    
    
    HRESULT GetTextExtent( TCHAR* strText, SIZE* pSize );

    
    HRESULT InitDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice );
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();

    
    CD3DFont( TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L );
    ~CD3DFont();
};




#endif


