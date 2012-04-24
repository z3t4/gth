



#if !defined(AFX_D3DRENDER_H__7CEAC62E_8AEE_48E9_8275_B0ECA7D1E8EB__INCLUDED_)
#define AFX_D3DRENDER_H__7CEAC62E_8AEE_48E9_8275_B0ECA7D1E8EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <d3dx8.h>
#include "shader.h"

class CD3DRender  
{
public:
	         CD3DRender();
	virtual ~CD3DRender();

	DWORD m_format;
	DWORD m_FogColor;
	float m_FogStart;
	float m_FogEnd;
	bool  m_bFog;

	void InitDevice();


	void SetTransform( D3DTRANSFORMSTATETYPE transType , matrix4x4_t *matrix );
	void SetStreamSource( int numStream , LPDIRECT3DVERTEXBUFFER8 ptrVB , int sizeStride );
	void SetVertexShader( DWORD format );
	void SetIndices( LPDIRECT3DINDEXBUFFER8 ptrIB , int baseVertexIdx );
	void SetTexture( int stage , LPDIRECT3DTEXTURE8 d3dTex );
	void SetLight( int idxLight , light_t *light );
	void SetMaterial( material_t *material );
	void SetupVertexFog(DWORD color, float start, float end, DWORD mode = D3DFOG_LINEAR , int useRange = false , float density = 0.66f );
	void SetupPixelFog(DWORD color, float start, float end, DWORD mode = D3DFOG_LINEAR , float density = 0.66f );
	void SetFogEnable();
	void SetFogDisable();
	void SetSkyFogParameter(DWORD color , float start , float end);
	void SetFogParameter(DWORD color, float start, float end);
	void SetDefaultRenderState();
	void ApplyDiffuseRenderState();
	void ReleaseDiffuseRenderState();
	void SetCustomRenderState();
	void SetMDLRenderState();
	void SetCharacterRenderState();
	void SetShadowRenderState();
	void SetDefaultRenderstates();
	void RenderDefault( int baseVB , int sizeVB , int baseIB , int sizeRenderElems );
	void RenderCustom( shader_t *shader , int baseVB , int sizeVB , int baseIB , int sizeRenderElems );
	void RenderSky( shader_t *shader , int baseVB , int sizeVB , int baseIB , int sizeRenderElems );
	void RenderMDLCustom( shader_t *shader , int baseVB , int sizeVB , int baseIB , int sizeRenderElems );
	void SetAttachShader( DWORD color );
	void EndAttachShader();
	void RenderTransDefault( int baseVB , int sizeVB , int baseIB , int sizeRenderElems , DWORD color );
	void RenderTransMDLCustom( shader_t *shader , int baseVB , int sizeVB , int baseIB , int sizeRenderElems , DWORD color );
};


#define SetD3DRGBA(a,b,c,d,e) { a.r=b; a.g=c; a.b=d; a.a=e; }
#define SetD3DVector(a,b,c,d) { a.x=b; a.y=c; a.z=d; }


extern CD3DRender g_d3dRender;

#endif 
