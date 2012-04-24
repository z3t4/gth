


#include "core_def.h"
#include "d3drender.h"
#include "d3dbuffer.h"
#include "d3ddevice.h"
#include "d3dtexturemng.h"

CD3DRender g_d3dRender;





CD3DRender::CD3DRender()
{
	m_bFog = false;
}

CD3DRender::~CD3DRender()
{
}

void CD3DRender::InitDevice()   
{
	SetDefaultRenderstates();
}

void CD3DRender::SetTransform( D3DTRANSFORMSTATETYPE transType , matrix4x4_t *matrix )
{
	g_d3dDevice.GetD3DDevice()->SetTransform( transType, (D3DMATRIX *) matrix );
}

void CD3DRender::SetStreamSource( int numStream , LPDIRECT3DVERTEXBUFFER8 ptrVB , int sizeStride )
{
	g_d3dDevice.GetD3DDevice()->SetStreamSource( numStream , ptrVB , sizeStride );
}

void CD3DRender::SetIndices( LPDIRECT3DINDEXBUFFER8 ptrIB , int baseVertexIdx )
{
	g_d3dDevice.GetD3DDevice()->SetIndices( ptrIB , baseVertexIdx );
}

void CD3DRender::SetTexture( int stage , LPDIRECT3DTEXTURE8 d3dTex )
{
	g_d3dDevice.GetD3DDevice()->SetTexture( stage , d3dTex );
}

void CD3DRender::SetLight( int idxLight , light_t *light )
{
	
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();

	pd3dDevice->SetLight ( idxLight , (D3DLIGHT8*) light );
	pd3dDevice->LightEnable ( idxLight , TRUE );
	pd3dDevice->SetRenderState ( D3DRS_AMBIENT, 0xFF808080 );
}

void CD3DRender::SetMaterial( material_t *material )
{	
	g_d3dDevice.GetD3DDevice()->SetMaterial ( (D3DMATERIAL8*) material );
}

void CD3DRender::SetupVertexFog(DWORD color, float start, float end, DWORD mode, int useRange, float density)
{
	
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();


    pd3dDevice->SetRenderState( D3DRS_FOGCOLOR , m_FogColor );
	pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE , D3DFOG_NONE );



    if( D3DFOG_LINEAR == mode )
    {
        pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE , mode );
        pd3dDevice->SetRenderState( D3DRS_FOGSTART      , *(DWORD *)(&start) );
        pd3dDevice->SetRenderState( D3DRS_FOGEND        , *(DWORD *)(&end) );
    }
    else
    {
        pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE , mode );
        pd3dDevice->SetRenderState( D3DRS_FOGDENSITY    , *(DWORD *)(&density) );
    }


    if( useRange )
        pd3dDevice->SetRenderState( D3DRS_RANGEFOGENABLE , TRUE );
}

void CD3DRender::SetupPixelFog(DWORD color, float start, float end, DWORD mode, float density)
{
	
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();


    pd3dDevice->SetRenderState( D3DRS_FOGCOLOR , color );
	pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE , D3DFOG_NONE );
	


    if( D3DFOG_LINEAR == mode )
    {
        pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE , mode );
        pd3dDevice->SetRenderState( D3DRS_FOGSTART     , *(DWORD *)(&start) );
        pd3dDevice->SetRenderState( D3DRS_FOGEND       , *(DWORD *)(&end) );
    }
    else
    {
        pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE , mode );
        pd3dDevice->SetRenderState( D3DRS_FOGDENSITY   , *(DWORD *)(&density) );
    }

}

void CD3DRender::SetSkyFogParameter(DWORD color , float start , float end)
{
	if(g_d3dDevice.GetFogType() == GTH_PIXEL_FOG_TYPE)
		SetupPixelFog(color, start, end);
	else if(g_d3dDevice.GetFogType() == GTH_VERTEX_FOG_TYPE)
		SetupVertexFog(color, start, end);
}

void CD3DRender::SetFogParameter(DWORD color , float start , float end)
{
	m_FogColor = color;
	m_FogStart = start;
	m_FogEnd   = end;
	if(g_d3dDevice.GetFogType() == GTH_PIXEL_FOG_TYPE)
		SetupPixelFog(color, start, end);
	else if(g_d3dDevice.GetFogType() == GTH_VERTEX_FOG_TYPE)
		SetupVertexFog(color, start, end);
}

void CD3DRender::SetFogEnable()
{
	if(g_d3dDevice.GetFogType())
	{
		m_bFog = true;
		g_d3dDevice.GetD3DDevice()->SetRenderState( D3DRS_FOGENABLE , true );
	}
	else
	{
		m_bFog = false;
		g_d3dDevice.GetD3DDevice()->SetRenderState( D3DRS_FOGENABLE, false );
	}
}

void CD3DRender::SetFogDisable()
{
	m_bFog = false;
	g_d3dDevice.GetD3DDevice()->SetRenderState( D3DRS_FOGENABLE, false );
}

void CD3DRender::SetVertexShader( DWORD format )
{
	if( format & GTH_FORMAT_XYZ )
        m_format = D3DFVF_XYZ;
    else if( format & GTH_FORMAT_XYZRHW )
        m_format = D3DFVF_XYZRHW;
	
    if( format & GTH_FORMAT_NORMAL )
        m_format |= D3DFVF_NORMAL;
    
    if( format & GTH_FORMAT_DIFFUSE )
        m_format |= D3DFVF_DIFFUSE;
    
    if( format & GTH_FORMAT_SPECULAR )
        m_format |= D3DFVF_SPECULAR;
    
    if( format & GTH_FORMAT_TEXVERTEXFLAG )
    {
        switch( (format & ~GTH_FORMAT_TEXVERTEXFLAG ) >> 8 )
        {
        case 0:
            m_format |= D3DFVF_TEX0;
            break;
        case 1:
            m_format |= D3DFVF_TEX1;            
            break;
        case 2:
            m_format |= D3DFVF_TEX2;            
            break;
        case 3:
            m_format |= D3DFVF_TEX3;            
            break;
        case 4:
            m_format |= D3DFVF_TEX4;            
            break;
        case 5:
            m_format |= D3DFVF_TEX5;            
            break;
        case 6:
            m_format |= D3DFVF_TEX6;            
            break;
        case 7:
            m_format |= D3DFVF_TEX7;            
            break;
        case 8:
            m_format |= D3DFVF_TEX8;            
            break;
        default:
            GTH_ERROR( GTH_ERROR_PRINT , "vertex buffer couldn't have more than 8 texture coord" );
            break;
        }
    }
	g_d3dDevice.GetD3DDevice()->SetVertexShader( m_format );
}

void CD3DRender::SetDefaultRenderState()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();

	pd3dDevice->SetRenderState( D3DRS_LIGHTING         , FALSE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW ); 
	pd3dDevice->SetRenderState( D3DRS_DITHERENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE   , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE          , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZBIAS            , 0 );
	pd3dDevice->SetRenderState( D3DRS_ZFUNC            , D3DCMP_LESSEQUAL );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE  , FALSE );

	pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_WRAP );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_WRAP );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP   , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2 , D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP   , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP   , D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2 , D3DTA_CURRENT ); 
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP   , D3DTOP_DISABLE );












	pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );

}

void CD3DRender::ApplyDiffuseRenderState()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP   , D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP   , D3DTOP_DISABLE );
}

void CD3DRender::ReleaseDiffuseRenderState()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP   , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP   , D3DTOP_MODULATE );
}

void CD3DRender::SetCustomRenderState()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetRenderState( D3DRS_LIGHTING         , FALSE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW ); 
	pd3dDevice->SetRenderState( D3DRS_DITHERENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE   , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE          , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZBIAS            , 0 );
	pd3dDevice->SetRenderState( D3DRS_ZFUNC            , D3DCMP_LESSEQUAL );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE  , FALSE );

	pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_WRAP );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_WRAP );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP   , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2 , D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP   , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP   , D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP   , D3DTOP_DISABLE );











	pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );


}

void CD3DRender::SetMDLRenderState()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetRenderState( D3DRS_LIGHTING         , TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW ); 
	pd3dDevice->SetRenderState( D3DRS_DITHERENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE   , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE          , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZBIAS            , 0 );
	pd3dDevice->SetRenderState( D3DRS_ZFUNC            , D3DCMP_LESSEQUAL );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE  , FALSE );

	pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_WRAP );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_WRAP );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP   , D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2 , D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP   , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP   , D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP   , D3DTOP_DISABLE );











	pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );


}

void CD3DRender::SetCharacterRenderState()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetRenderState( D3DRS_LIGHTING         , TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW ); 
	pd3dDevice->SetRenderState( D3DRS_DITHERENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE   , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE          , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZBIAS            , 0 );
	pd3dDevice->SetRenderState( D3DRS_ZFUNC            , D3DCMP_LESSEQUAL );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE  , FALSE );

	pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_WRAP );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_WRAP );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP   , D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2 , D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP   , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP   , D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP   , D3DTOP_DISABLE );











	pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );


}

void CD3DRender::SetShadowRenderState()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetRenderState( D3DRS_LIGHTING         , TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW ); 
	pd3dDevice->SetRenderState( D3DRS_DITHERENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE   , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE          , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZBIAS            , 0 );
	pd3dDevice->SetRenderState( D3DRS_ZFUNC            , D3DCMP_LESSEQUAL );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE     , TRUE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE  , FALSE );

	pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP   , D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2 , D3DTA_CURRENT );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP   , D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2 , D3DTA_CURRENT );

	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP   , D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP   , D3DTOP_DISABLE );











	pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER , D3DTEXF_LINEAR );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );


}

void CD3DRender::RenderDefault( int baseVB , int sizeVB , int baseIB , int sizeRenderElems )
{
	g_d3dDevice.GetD3DDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , baseVB , sizeVB , baseIB , sizeRenderElems / 3 );
}

void CD3DRender::RenderTransDefault( int baseVB , int sizeVB , int baseIB , int sizeRenderElems , DWORD color )
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR , color );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_TFACTOR );

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , D3DBLEND_SRCCOLOR );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , D3DBLEND_INVSRCCOLOR );

	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , baseVB , sizeVB , baseIB , sizeRenderElems / 3 );

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );
}

void CD3DRender::RenderCustom( shader_t *shader , int baseVB , int sizeVB , int baseIB , int sizeRenderElems )
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	shaderpass_t *pass;
	DWORD flags;

	if( shader->flags & SHADER_CULL )
	{
		if( shader->flags & SHADER_CULL_NONE )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_NONE ); 
		else if( shader->flags & SHADER_CULL_FRONT )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CW ); 
	}

	pass = &shader->passes[0];
	for( int i = 0 ; i < shader->numPasses ; i++ , pass++ )
	{
		flags = pass->flags;
		
		if( flags & SHADERPASS_MAP_LIGHTMAP )
		{
			pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 1 );



		}
		else
		{
			pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );



		}

		pd3dDevice->SetTexture( 0 , g_texMng.GetD3DTexture( pass->texRef ) );

		if( flags & SHADERPASS_CLAMPMAP )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP );
		}

		if( flags & SHADERPASS_BLEND )
		{
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
			pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , pass->blend.src );
			pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , pass->blend.dest );
		}

		if( flags & SHADERPASS_RGBGEN )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP , D3DTOP_MODULATE );

			if( !( pass->rgbGen.type & SHADERPASS_RGBGEN_VERTEX ) )
			{
				pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR , pass->rgbGen.color );
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_TFACTOR );
			}
		}
		
		if( flags & SHADERPASS_ALPHAGEN )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP , D3DTOP_SELECTARG2 );

			if( !( pass->alphaGen.type & SHADERPASS_RGBGEN_VERTEX ) )
			{
				pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR , pass->alphaGen.color );
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_TFACTOR );
			}
		}
		

		if( flags & SHADERPASS_DEPTHFUNC )
			pd3dDevice->SetRenderState( D3DRS_ZFUNC , pass->depthFunc );

		if( flags & SHADERPASS_ALPHAFUNC )
		{
			pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );
			pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC       , pass->alphaFunc.func );
			pd3dDevice->SetRenderState( D3DRS_ALPHAREF        , pass->alphaFunc.ref );
		}

		if( flags & SHADERPASS_NONDEPTHWRITE )
			pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE , FALSE );

		if( flags & SHADERPASS_TCMOD )
		{
			pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX *)&pass->tcMod.tcMatrix );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		}

		if( flags & SHADERPASS_TCGEN )
		{
			pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX *)&shader->tcGenMatrix );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		}

		

		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , baseVB , sizeVB , baseIB , sizeRenderElems / 3 );

		

		if( flags & SHADERPASS_CLAMPMAP )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_WRAP );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_WRAP );
		}

		if( flags & SHADERPASS_BLEND )
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
		
		if( flags & SHADERPASS_RGBGEN )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP , D3DTOP_SELECTARG1 );

			if( !( pass->rgbGen.type & SHADERPASS_RGBGEN_VERTEX ) )
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );
		}

		if( flags & SHADERPASS_ALPHAGEN )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP , D3DTOP_SELECTARG1 );

			if( !( pass->alphaGen.type & SHADERPASS_RGBGEN_VERTEX ) )
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );
		}

		if( flags & SHADERPASS_DEPTHFUNC )
			pd3dDevice->SetRenderState( D3DRS_ZFUNC , D3DCMP_LESSEQUAL );

		if( flags & SHADERPASS_ALPHAFUNC )
			pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );
	
		if( flags & SHADERPASS_NONDEPTHWRITE )
			pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

		if( flags & SHADERPASS_TCMOD || flags & SHADERPASS_TCGEN )
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	}

	if( shader->flags & SHADER_CULL )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW ); 
}


void CD3DRender::RenderSky( shader_t *shader , int baseVB , int sizeVB , int baseIB , int sizeRenderElems )
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	shaderpass_t *pass;
	DWORD flags;

	if( shader->flags & SHADER_CULL )
	{
		if( shader->flags & SHADER_CULL_NONE )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_NONE ); 
		else if( shader->flags & SHADER_CULL_FRONT )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CW ); 
	}

	pass = &shader->passes[0];
	for( int i = 0 ; i < shader->numPasses ; i++ , pass++ )
	{
		flags = pass->flags;
		
		pd3dDevice->SetTexture( 0 , g_texMng.GetD3DTexture( pass->texRef ) );
		if( flags & SHADERPASS_CLAMPMAP )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP );
		}

		switch( i )
		{
		case 0:
			{
				pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , false);
				pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP ,  D3DTOP_SELECTARG1);
				pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			}
			break;
		case 1:
			{
				if(shader->numPasses == 2)
				{
					pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
					pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , D3DBLEND_SRCCOLOR );
					pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , D3DBLEND_INVSRCCOLOR );
					pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_SELECTARG1);
					pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				}
				else
				{
					pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
					pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , D3DBLEND_SRCCOLOR );
					pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , D3DBLEND_ONE );
					pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_SELECTARG1);
					pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				}
			}
			break;
		case 2:
			{
				pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
				pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , D3DBLEND_SRCCOLOR );
				pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , D3DBLEND_INVSRCCOLOR );
				pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_SELECTARG1);
				pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			}
			break;
		}

		if( flags & SHADERPASS_TCMOD )
		{
			pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX *)&pass->tcMod.tcMatrix );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		}

		if( flags & SHADERPASS_TCGEN )
		{
			pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX *)&shader->tcGenMatrix );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		}

		

		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , baseVB , sizeVB , baseIB , sizeRenderElems / 3 );

		

		if( flags & SHADERPASS_CLAMPMAP )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_WRAP );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_WRAP );
		}

		if( flags & SHADERPASS_BLEND )
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
		
		if( flags & SHADERPASS_RGBGEN )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP , D3DTOP_SELECTARG1 );

			if( !( pass->rgbGen.type & SHADERPASS_RGBGEN_VERTEX ) )
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );
		}

		if( flags & SHADERPASS_ALPHAGEN )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP , D3DTOP_SELECTARG1 );

			if( !( pass->alphaGen.type & SHADERPASS_RGBGEN_VERTEX ) )
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );
		}

		if( flags & SHADERPASS_DEPTHFUNC )
			pd3dDevice->SetRenderState( D3DRS_ZFUNC , D3DCMP_LESSEQUAL );

		if( flags & SHADERPASS_ALPHAFUNC )
			pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );
	
		if( flags & SHADERPASS_NONDEPTHWRITE )
			pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

		if( flags & SHADERPASS_TCMOD || flags & SHADERPASS_TCGEN )
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	}

	if( shader->flags & SHADER_CULL )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW ); 
}


void CD3DRender::RenderMDLCustom( shader_t *shader , int baseVB , int sizeVB , int baseIB , int sizeRenderElems )
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	shaderpass_t *pass;
	DWORD flags;

	if( shader->flags & SHADER_CULL )
	{
		if( shader->flags & SHADER_CULL_NONE )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_NONE ); 
		else if( shader->flags & SHADER_CULL_FRONT )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CW ); 
	}

	pass = &shader->passes[0];
	for( int i = 0 ; i < shader->numPasses ; i++ , pass++ )
	{
		flags = pass->flags;
		
		if( flags & SHADERPASS_MAP_LIGHTMAP )
		{
			pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 1 );



		}
		else
		{
			pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );



		}

		pd3dDevice->SetTexture( 0 , g_texMng.GetD3DTexture( pass->texRef ) );

		if( flags & SHADERPASS_CLAMPMAP )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP );
		}

		if( flags & SHADERPASS_BLEND )
		{
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
			pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , pass->blend.src );
			pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , pass->blend.dest );
		}

		if( flags & SHADERPASS_RGBGEN )
		{
			if( !( pass->rgbGen.type & SHADERPASS_RGBGEN_VERTEX ) )
			{
				pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR , pass->rgbGen.color );
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_TFACTOR );
			}
		}
		
		if( flags & SHADERPASS_ALPHAGEN )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP , D3DTOP_SELECTARG2 );

			if( !( pass->alphaGen.type & SHADERPASS_RGBGEN_VERTEX ) )
			{
				pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR , pass->alphaGen.color );
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_TFACTOR );
			}
		}
		

		if( flags & SHADERPASS_DEPTHFUNC )
			pd3dDevice->SetRenderState( D3DRS_ZFUNC , pass->depthFunc );

		if( flags & SHADERPASS_ALPHAFUNC )
		{
			pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );
			pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC       , pass->alphaFunc.func );
			pd3dDevice->SetRenderState( D3DRS_ALPHAREF        , pass->alphaFunc.ref );
		}

		if( flags & SHADERPASS_NONDEPTHWRITE )
			pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE , FALSE );

		if( flags & SHADERPASS_TCMOD )
		{
			pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX *)&pass->tcMod.tcMatrix );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		}

		if( flags & SHADERPASS_TCGEN )
		{
			pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX *)&shader->tcGenMatrix );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		}

		

		
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , baseVB , sizeVB , baseIB , sizeRenderElems / 3 );

		

		if( flags & SHADERPASS_CLAMPMAP )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_WRAP );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_WRAP );
		}

		if( flags & SHADERPASS_BLEND )
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
		
		if( flags & SHADERPASS_RGBGEN )
		{
			if( !( pass->rgbGen.type & SHADERPASS_RGBGEN_VERTEX ) )
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );
		}

		if( flags & SHADERPASS_ALPHAGEN )
		{
			

			if( !( pass->alphaGen.type & SHADERPASS_RGBGEN_VERTEX ) )
				pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );
		}

		if( flags & SHADERPASS_DEPTHFUNC )
			pd3dDevice->SetRenderState( D3DRS_ZFUNC , D3DCMP_LESSEQUAL );

		if( flags & SHADERPASS_ALPHAFUNC )
			pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );
	
		if( flags & SHADERPASS_NONDEPTHWRITE )
			pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

		if( flags & SHADERPASS_TCMOD || flags & SHADERPASS_TCGEN )
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	}

	if( shader->flags & SHADER_CULL )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW );
}

void CD3DRender::RenderTransMDLCustom( shader_t *shader , int baseVB , int sizeVB , int baseIB , int sizeRenderElems , DWORD color )
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	shaderpass_t *pass;
	DWORD flags;

	if( shader->flags & SHADER_CULL )
	{
		if( shader->flags & SHADER_CULL_NONE )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_NONE ); 
		else if( shader->flags & SHADER_CULL_FRONT )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CW ); 
	}

	pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR , color );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_TFACTOR );

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , D3DBLEND_SRCCOLOR );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , D3DBLEND_INVSRCCOLOR );

	pass = &shader->passes[0];
	for( int i = 0 ; i < shader->numPasses ; i++ , pass++ )
	{
		flags = pass->flags;
		
		if( flags & SHADERPASS_MAP_LIGHTMAP )
		{
			pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 1 );



		}
		else
		{
			pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );



		}

		pd3dDevice->SetTexture( 0 , g_texMng.GetD3DTexture( pass->texRef ) );

		if( flags & SHADERPASS_CLAMPMAP )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP );
		}

		if( flags & SHADERPASS_DEPTHFUNC )
			pd3dDevice->SetRenderState( D3DRS_ZFUNC , pass->depthFunc );

		if( flags & SHADERPASS_ALPHAFUNC )
		{
			pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );
			pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC       , pass->alphaFunc.func );
			pd3dDevice->SetRenderState( D3DRS_ALPHAREF        , pass->alphaFunc.ref );
		}

		if( flags & SHADERPASS_NONDEPTHWRITE )
			pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE , FALSE );

		if( flags & SHADERPASS_TCMOD )
		{
			pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX *)&pass->tcMod.tcMatrix );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		}

		if( flags & SHADERPASS_TCGEN )
		{
			pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX *)&shader->tcGenMatrix );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		}

		

		
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , baseVB , sizeVB , baseIB , sizeRenderElems / 3 );

		

		if( flags & SHADERPASS_CLAMPMAP )
		{
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_WRAP );
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_WRAP );
		}

		if( flags & SHADERPASS_DEPTHFUNC )
			pd3dDevice->SetRenderState( D3DRS_ZFUNC , D3DCMP_LESSEQUAL );

		if( flags & SHADERPASS_ALPHAFUNC )
			pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );
	
		if( flags & SHADERPASS_NONDEPTHWRITE )
			pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

		if( flags & SHADERPASS_TCMOD || flags & SHADERPASS_TCGEN )
			pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	}

	if( shader->flags & SHADER_CULL )
			pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_CCW );

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );
}


void CD3DRender::SetAttachShader( DWORD color )
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXCOORDINDEX, 0 );



	
	pd3dDevice->SetTexture( 0 , g_texMng.GetD3DTexture( g_texMng.GetWhiteImageRef() ) );

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND         , D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND        , D3DBLEND_INVSRCALPHA );
	
	pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR , color );

	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP   , D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_TFACTOR );
		
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP   , D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_TFACTOR );
	
}

void CD3DRender::EndAttachShader()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , FALSE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP , D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );



}

void CD3DRender::SetDefaultRenderstates()
{
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();
	
	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, 0xFF0000);
	pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
	pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
	pd3dDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE );
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pd3dDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	pd3dDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);

	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pd3dDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pd3dDevice->SetTextureStageState(3, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(3, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pd3dDevice->SetTextureStageState(4, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(4, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(4, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(4, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(4, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(4, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pd3dDevice->SetTextureStageState(5, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(5, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(5, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(5, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(5, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(5, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pd3dDevice->SetTextureStageState(6, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(6, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(6, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(6, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(6, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(6, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pd3dDevice->SetTextureStageState(7, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(7, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(7, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(7, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pd3dDevice->SetTextureStageState(7, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(7, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	pd3dDevice->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 2);
	pd3dDevice->SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 3);
	pd3dDevice->SetTextureStageState(4, D3DTSS_TEXCOORDINDEX, 4);
	pd3dDevice->SetTextureStageState(5, D3DTSS_TEXCOORDINDEX, 5);
	pd3dDevice->SetTextureStageState(6, D3DTSS_TEXCOORDINDEX, 6);
	pd3dDevice->SetTextureStageState(7, D3DTSS_TEXCOORDINDEX, 7);
	
	pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pd3dDevice->SetTextureStageState(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pd3dDevice->SetTextureStageState(2, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(2, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pd3dDevice->SetTextureStageState(3, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(3, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(3, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pd3dDevice->SetTextureStageState(4, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(4, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(4, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pd3dDevice->SetTextureStageState(5, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(5, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(5, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pd3dDevice->SetTextureStageState(6, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(6, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(6, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pd3dDevice->SetTextureStageState(7, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(7, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetTextureStageState(7, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(2, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(2, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(3, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(3, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(4, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(4, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(5, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(5, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(6, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(6, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(7, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetTextureStageState(7, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);

	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
	pd3dDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
	pd3dDevice->SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
	pd3dDevice->SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
	pd3dDevice->SetTextureStageState(4, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
	pd3dDevice->SetTextureStageState(5, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
	pd3dDevice->SetTextureStageState(6, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
	pd3dDevice->SetTextureStageState(7, D3DTSS_TEXTURETRANSFORMFLAGS, 0);

	pd3dDevice->SetRenderState( D3DRS_WRAP0, 0);
	pd3dDevice->SetRenderState( D3DRS_WRAP1, 0);
	pd3dDevice->SetRenderState( D3DRS_WRAP2, 0);
	pd3dDevice->SetRenderState( D3DRS_WRAP3, 0);
	pd3dDevice->SetRenderState( D3DRS_WRAP4, 0);
	pd3dDevice->SetRenderState( D3DRS_WRAP5, 0);
	pd3dDevice->SetRenderState( D3DRS_WRAP6, 0);
	pd3dDevice->SetRenderState( D3DRS_WRAP7, 0);

	pd3dDevice->SetTexture(0, NULL);
	pd3dDevice->SetTexture(1, NULL);
	pd3dDevice->SetTexture(2, NULL);
	pd3dDevice->SetTexture(3, NULL);
	pd3dDevice->SetTexture(4, NULL);
	pd3dDevice->SetTexture(5, NULL);
	pd3dDevice->SetTexture(6, NULL);
	pd3dDevice->SetTexture(7, NULL);
	
	pd3dDevice->SetPixelShader(0);
}
