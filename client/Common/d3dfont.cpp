




#define STRICT
#include <stdio.h>
#include <tchar.h>
#include <D3DX8.h>
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"







#define MAX_NUM_VERTICES 50*6

struct FONT2DVERTEX { D3DXVECTOR4 p;   DWORD color;     FLOAT tu, tv; };
struct FONT3DVERTEX { D3DXVECTOR3 p;   D3DXVECTOR3 n;   FLOAT tu, tv; };

#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FONT3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

inline FONT2DVERTEX InitFont2DVertex( const D3DXVECTOR4& p, D3DCOLOR color,
                                      FLOAT tu, FLOAT tv )
{
    FONT2DVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
    return v;
}

inline FONT3DVERTEX InitFont3DVertex( const D3DXVECTOR3& p, const D3DXVECTOR3& n,
                                      FLOAT tu, FLOAT tv )
{
    FONT3DVERTEX v;   v.p = p;   v.n = n;   v.tu = tu;   v.tv = tv;
    return v;
}








CD3DFont::CD3DFont( TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags )
{
    _tcscpy( m_strFontName, strFontName );
    m_dwFontHeight         = dwHeight;
    m_dwFontFlags          = dwFlags;

    m_pd3dDevice           = NULL;
    m_pTexture             = NULL;
    m_pVB                  = NULL;

    m_dwSavedStateBlock    = 0L;
    m_dwDrawTextStateBlock = 0L;
}








CD3DFont::~CD3DFont()
{
    InvalidateDeviceObjects();
    DeleteDeviceObjects();
}









HRESULT CD3DFont::InitDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice )
{
    HRESULT hr;

    
    m_pd3dDevice = pd3dDevice;

    
    m_fTextScale  = 1.0f; 

    
    if( m_dwFontHeight > 40 )
        m_dwTexWidth = m_dwTexHeight = 1024;
    else if( m_dwFontHeight > 20 )
        m_dwTexWidth = m_dwTexHeight = 512;
    else
        m_dwTexWidth  = m_dwTexHeight = 256;

    
    
    D3DCAPS8 d3dCaps;
    m_pd3dDevice->GetDeviceCaps( &d3dCaps );

    if( m_dwTexWidth > d3dCaps.MaxTextureWidth )
    {
        m_fTextScale = (FLOAT)d3dCaps.MaxTextureWidth / (FLOAT)m_dwTexWidth;
        m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
    }

    
    hr = m_pd3dDevice->CreateTexture( m_dwTexWidth, m_dwTexHeight, 1,
                                      0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &m_pTexture );
    if( FAILED(hr) )
        return hr;

    
    DWORD*      pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       =  (int)m_dwTexWidth;
    bmi.bmiHeader.biHeight      = -(int)m_dwTexHeight;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    
    HDC     hDC       = CreateCompatibleDC( NULL );
    HBITMAP hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
                                          (VOID**)&pBitmapBits, NULL, 0 );
    SetMapMode( hDC, MM_TEXT );

    
    
    INT nHeight    = -MulDiv( m_dwFontHeight, 
        (INT)(GetDeviceCaps(hDC, LOGPIXELSY) * m_fTextScale), 72 );
    DWORD dwBold   = (m_dwFontFlags&D3DFONT_BOLD)   ? FW_BOLD : FW_NORMAL;
    DWORD dwItalic = (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE    : FALSE;
    HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, m_strFontName );
    if( NULL==hFont )
        return E_FAIL;

    SelectObject( hDC, hbmBitmap );
    SelectObject( hDC, hFont );

    
    SetTextColor( hDC, RGB(255,255,255) );
    SetBkColor(   hDC, 0x00000000 );
    SetTextAlign( hDC, TA_TOP );

    
    
    DWORD x = 0;
    DWORD y = 0;
    TCHAR str[2] = _T("x");
    SIZE size;

    for( TCHAR c=32; c<127; c++ )
    {
        str[0] = c;
        GetTextExtentPoint32( hDC, str, 1, &size );

        if( (DWORD)(x+size.cx+1) > m_dwTexWidth )
        {
            x  = 0;
            y += size.cy+1;
        }

        ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL );

        m_fTexCoords[c-32][0] = ((FLOAT)(x+0))/m_dwTexWidth;
        m_fTexCoords[c-32][1] = ((FLOAT)(y+0))/m_dwTexHeight;
        m_fTexCoords[c-32][2] = ((FLOAT)(x+0+size.cx))/m_dwTexWidth;
        m_fTexCoords[c-32][3] = ((FLOAT)(y+0+size.cy))/m_dwTexHeight;

        x += size.cx+1;
    }

    
    D3DLOCKED_RECT d3dlr;
    m_pTexture->LockRect( 0, &d3dlr, 0, 0 );
    BYTE* pDstRow = (BYTE*)d3dlr.pBits;
    WORD* pDst16;
    BYTE bAlpha; 

    for( y=0; y < m_dwTexHeight; y++ )
    {
        pDst16 = (WORD*)pDstRow;
        for( x=0; x < m_dwTexWidth; x++ )
        {
            bAlpha = (BYTE)((pBitmapBits[m_dwTexWidth*y + x] & 0xff) >> 4);
            if (bAlpha > 0)
            {
                *pDst16++ = (bAlpha << 12) | 0x0fff;
            }
            else
            {
                *pDst16++ = 0x00000000;
            }
        }
        pDstRow += d3dlr.Pitch;
    }

    
    m_pTexture->UnlockRect(0);
    DeleteObject( hbmBitmap );
    DeleteDC( hDC );
    DeleteObject( hFont );

    return S_OK;
}








HRESULT CD3DFont::RestoreDeviceObjects()
{
    HRESULT hr;

    
    if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer( MAX_NUM_VERTICES*sizeof(FONT2DVERTEX),
                                                       D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                       D3DPOOL_DEFAULT, &m_pVB ) ) )
    {
        return hr;
    }

    
    for( UINT which=0; which<2; which++ )
    {
        
    }

    return S_OK;
}

void CD3DFont::SetRenderState()
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
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE          , FALSE );

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






HRESULT CD3DFont::InvalidateDeviceObjects()
{
    SAFE_RELEASE( m_pVB );

    
    if( m_pd3dDevice )
    {
        if( m_dwSavedStateBlock )
            m_pd3dDevice->DeleteStateBlock( m_dwSavedStateBlock );
        if( m_dwDrawTextStateBlock )
            m_pd3dDevice->DeleteStateBlock( m_dwDrawTextStateBlock );
    }

    m_dwSavedStateBlock    = 0L;
    m_dwDrawTextStateBlock = 0L;

    return S_OK;
}








HRESULT CD3DFont::DeleteDeviceObjects()
{
    SAFE_RELEASE( m_pTexture );
    m_pd3dDevice = NULL;

    return S_OK;
}








HRESULT CD3DFont::GetTextExtent( TCHAR* strText, SIZE* pSize )
{
    if( NULL==strText || NULL==pSize )
        return E_FAIL;

    FLOAT fRowWidth  = 0.0f;
    FLOAT fRowHeight = (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwTexHeight;
    FLOAT fWidth     = 0.0f;
    FLOAT fHeight    = fRowHeight;

    while( *strText )
    {
        TCHAR c = *strText++;

        if( c == _T('\n') )
        {
            fRowWidth = 0.0f;
            fHeight  += fRowHeight;
        }
        if( c < _T(' ') )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT tx2 = m_fTexCoords[c-32][2];

        fRowWidth += (tx2-tx1)*m_dwTexWidth;

        if( fRowWidth > fWidth )
            fWidth = fRowWidth;
    }

    pSize->cx = (int)fWidth;
    pSize->cy = (int)fHeight;

    return S_OK;
}












HRESULT CD3DFont::DrawTextScaled( FLOAT x, FLOAT y, FLOAT z,
                                  FLOAT fXScale, FLOAT fYScale, DWORD dwColor,
                                  TCHAR* strText, DWORD dwFlags )
{
    if( m_pd3dDevice == NULL )
        return E_FAIL;

	m_pd3dDevice->SetTexture( 0, m_pTexture );
	SetRenderState();

    
    
    
    m_pd3dDevice->SetVertexShader( D3DFVF_FONT2DVERTEX );
    m_pd3dDevice->SetPixelShader( NULL );
    m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT2DVERTEX) );

    
    

    D3DVIEWPORT8 vp;
    m_pd3dDevice->GetViewport( &vp );
    FLOAT sx  = x;
    FLOAT sy  = y;
    FLOAT sz  = z;
    FLOAT rhw = 1.0f;
    FLOAT fStartX = sx;

    FLOAT fLineHeight = ( m_fTexCoords[0][3] - m_fTexCoords[0][1] ) * m_dwTexHeight;

    
    FONT2DVERTEX* pVertices;
    DWORD         dwNumTriangles = 0L;
    m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

    while( *strText )
    {
        TCHAR c = *strText++;

        if( c == _T('\n') )
        {
            sx  = fStartX;
            sy += fYScale*vp.Height;
        }
        if( c < _T(' ') )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT ty1 = m_fTexCoords[c-32][1];
        FLOAT tx2 = m_fTexCoords[c-32][2];
        FLOAT ty2 = m_fTexCoords[c-32][3];

        FLOAT w = (tx2-tx1)*m_dwTexWidth;
        FLOAT h = (ty2-ty1)*m_dwTexHeight;

        w *= (fXScale*vp.Height)/fLineHeight;
        h *= (fYScale*vp.Height)/fLineHeight;

        if( c != _T(' ') )
        {
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+h-0.5f,sz,rhw), dwColor, tx1, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,sz,rhw), dwColor, tx1, ty1 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,sz,rhw), dwColor, tx2, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+0-0.5f,sz,rhw), dwColor, tx2, ty1 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,sz,rhw), dwColor, tx2, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,sz,rhw), dwColor, tx1, ty1 );
            dwNumTriangles += 2;

            if( dwNumTriangles*3 > (MAX_NUM_VERTICES-6) )
            {
                
                m_pVB->Unlock();
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
                m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );
                dwNumTriangles = 0L;
            }
        }

        sx += w;
    }

    
    m_pVB->Unlock();
    if( dwNumTriangles > 0 )
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

    
    

    return S_OK;
}








HRESULT CD3DFont::DrawText( FLOAT sx, FLOAT sy, DWORD dwColor,
                            TCHAR* strText, DWORD dwFlags )
{
    if( m_pd3dDevice == NULL )
        return E_FAIL;

	m_pd3dDevice->SetTexture( 0, m_pTexture );
	SetRenderState();

    
    
    
    m_pd3dDevice->SetVertexShader( D3DFVF_FONT2DVERTEX );
    m_pd3dDevice->SetPixelShader( NULL );
    m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT2DVERTEX) );

    
    
    {
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    }

    FLOAT fStartX = sx;

    
    FONT2DVERTEX* pVertices = NULL;
    DWORD         dwNumTriangles = 0;
    m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

    while( *strText )
    {
        TCHAR c = *strText++;

        if( c == _T('\n') )
        {
            sx = fStartX;
            sy += (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwTexHeight;
        }
        if( c < _T(' ') )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT ty1 = m_fTexCoords[c-32][1];
        FLOAT tx2 = m_fTexCoords[c-32][2];
        FLOAT ty2 = m_fTexCoords[c-32][3];

        FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
        FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

        if( c != _T(' ') )
        {
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx1, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx2, ty1 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
            dwNumTriangles += 2;

            if( dwNumTriangles*3 > (MAX_NUM_VERTICES-6) )
            {
                
                m_pVB->Unlock();
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
                pVertices = NULL;
                m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );
                dwNumTriangles = 0L;
            }
        }

        sx += w;
    }

    
    m_pVB->Unlock();
    if( dwNumTriangles > 0 )
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

    
    

    return S_OK;
}

HRESULT CD3DFont::DrawTexture( FLOAT sx, FLOAT sy, DWORD dwColor, DWORD dwFlags )
{
    if( m_pd3dDevice == NULL )
        return E_FAIL;

	m_pd3dDevice->SetTexture( 0, m_pTexture );
	SetRenderState();

    
    
    
    m_pd3dDevice->SetVertexShader( D3DFVF_FONT2DVERTEX );
    m_pd3dDevice->SetPixelShader( NULL );
    m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT2DVERTEX) );

    
    

    
    FONT2DVERTEX* pVertices = NULL;
    DWORD         dwNumTriangles = 2;
    m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

	FLOAT w = (float)m_dwTexWidth;
    FLOAT h = (float)m_dwTexHeight;

	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, 0.0f, 1.0f );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, 0.0f, 0.0f );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, 1.0f, 1.0f );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, 1.0f, 0.0f );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, 1.0f, 1.0f );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, 0.0f, 0.0f );
	
	
	m_pVB->Unlock();
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
	pVertices = NULL;
	
    
    

    return S_OK;
}







HRESULT CD3DFont::Render3DText( TCHAR* strText, DWORD dwFlags )
{
    if( m_pd3dDevice == NULL )
        return E_FAIL;

    
    m_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );
    m_pd3dDevice->ApplyStateBlock( m_dwDrawTextStateBlock );
    m_pd3dDevice->SetVertexShader( D3DFVF_FONT3DVERTEX );
    m_pd3dDevice->SetPixelShader( NULL );
    m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT3DVERTEX) );

    
    if( dwFlags & D3DFONT_FILTERED )
    {
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    }

    
    FLOAT x = 0.0f;
    FLOAT y = 0.0f;

    
    if( dwFlags & D3DFONT_CENTERED )
    {
        SIZE sz;
        GetTextExtent( strText, &sz );
        x = -(((FLOAT)sz.cx)/10.0f)/2.0f;
        y = -(((FLOAT)sz.cy)/10.0f)/2.0f;
    }

    
    if( dwFlags & D3DFONT_TWOSIDED )
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    FLOAT fStartX = x;
    TCHAR c;

    
    FONT3DVERTEX* pVertices;
    DWORD         dwVertex       = 0L;
    DWORD         dwNumTriangles = 0L;
    m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

    while( c = *strText++ )
    {
        if( c == '\n' )
        {
            x = fStartX;
            y -= (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwTexHeight/10.0f;
        }
        if( c < 32 )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT ty1 = m_fTexCoords[c-32][1];
        FLOAT tx2 = m_fTexCoords[c-32][2];
        FLOAT ty2 = m_fTexCoords[c-32][3];

        FLOAT w = (tx2-tx1) * m_dwTexWidth  / ( 10.0f * m_fTextScale );
        FLOAT h = (ty2-ty1) * m_dwTexHeight / ( 10.0f * m_fTextScale );

        if( c != _T(' ') )
        {
            *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+0,0), D3DXVECTOR3(0,0,-1), tx1, ty2 );
            *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+h,0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
            *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+0,0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
            *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+h,0), D3DXVECTOR3(0,0,-1), tx2, ty1 );
            *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+0,0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
            *pVertices++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+h,0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
            dwNumTriangles += 2;

            if( dwNumTriangles*3 > (MAX_NUM_VERTICES-6) )
            {
                
                m_pVB->Unlock();
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
                m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );
                dwNumTriangles = 0L;
            }
        }

        x += w;
    }

    
    m_pVB->Unlock();
    if( dwNumTriangles > 0 )
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

    
    m_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );

    return S_OK;
}




