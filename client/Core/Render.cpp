#include "bsp.h"
#include "camera.h"
#include "d3drender.h"
#include "d3dtexturemng.h"
#include "timer.h"
#include "d3ddevice.h"

void CBSP::RenderBefore()
{
	shadermap_t *shaderMap;
	shader_t *shader;
	int i , j , size;

	m_totalRenderPolygon = 0;
	m_currBank = -1;
	g_d3dRender.SetVertexShader( m_format );
	g_d3dRender.SetIndices( m_dynamicIB->GetD3DIndexBuffer() , 0 );
	m_dynamicIB->Discard();
	m_dynamicVB->Discard();

	g_d3dRender.SetDefaultRenderState();

	size = m_shaderType->sizeDefault;
	shaderMap = &m_shaderMaps[ m_shaderType->baseDefault ];
	for( i = 0 ; i < size ; i++ , shaderMap++ )
	{
		if( shaderMap->frameFlag != m_frameFlag )  continue;

		g_d3dRender.SetTexture( 0 , g_texMng.GetD3DTexture( shaderMap->shader ) );

		for( j = 0 ; j < m_numLightmaps ; j++ )
		{
			if( shaderMap->LMFrameFlags[ j ] != m_frameFlag )  continue;

			g_d3dRender.SetTexture( 1 , g_texMng.GetD3DTexture( m_lmapTexRefs[ j ] ) );
			RenderDefault( &shaderMap->LMMaps[ j ] );
		}

		
		if( shaderMap->LMFrameFlags[ m_numLightmaps ]  == m_frameFlag )
		{
			g_d3dRender.ApplyDiffuseRenderState();
			RenderDefault( &shaderMap->LMMaps[ m_numLightmaps ] );
			g_d3dRender.ReleaseDiffuseRenderState();
		}
	}

	g_d3dRender.SetCustomRenderState();

	size = m_shaderType->sizeCustom;
	shaderMap = &m_shaderMaps[ m_shaderType->baseCustom ];
	for( i = 0 ; i < size ; i++ , shaderMap++ )
	{
		if( shaderMap->frameFlag != m_frameFlag )  continue;

		shader = &g_shaderMng.m_shaders[ shaderMap->shader ];

		for( j = 0 ; j <= m_numLightmaps ; j++ )
		{
			if( shaderMap->LMFrameFlags[ j ] != m_frameFlag )  continue;

			RenderCustom( shader , &shaderMap->LMMaps[ j ] , j );
		}
	}

	g_d3dRender.SetStreamSource(0, m_dynamicVB->GetD3DVertexBuffer(), m_dynamicVB->GetStrideSize());

	size = m_shaderType->sizeDeform;
	shaderMap = &m_shaderMaps[ m_shaderType->baseDeform ];
	for( i = 0 ; i < size ; i++ , shaderMap++ )
	{
		if( shaderMap->frameFlag != m_frameFlag )  continue;

		shader = &g_shaderMng.m_shaders[ shaderMap->shader ];

		for( j = 0 ; j <= m_numLightmaps ; j++ )
		{
			if( shaderMap->LMFrameFlags[ j ] != m_frameFlag )  continue;			

			RenderDeform( shader , &shaderMap->LMMaps[ j ] , j );
		}
	}

	if(g_d3dRender.m_bFog)
	{
		
		D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)&g_camera.m_projMat,
									deg2rad(g_camera.m_projectParm.fov / g_camera.m_projectParm.aspect),
									g_camera.m_projectParm.aspect,
									g_camera.m_projectParm.znear,
									100000.0f );

		g_d3dDevice.GetD3DDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_camera.m_projMat );

		

		 
		D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)&g_camera.m_projMat,
								   deg2rad( g_camera.m_projectParm.fov / g_camera.m_projectParm.aspect ),
								   g_camera.m_projectParm.aspect,
								   g_camera.m_projectParm.znear,
								   g_camera.m_projectParm.zfar );

		g_d3dDevice.GetD3DDevice()->SetTransform( D3DTS_PROJECTION, (D3DXMATRIX *)&g_camera.m_projMat );
	}
}


void CBSP::RenderAfter()
{
	shadermap_t *shaderMap;
	shader_t *shader;
	int i , j , size;

	m_currBank = -1;
	g_d3dRender.SetVertexShader( m_format );
	g_d3dRender.SetIndices( m_dynamicIB->GetD3DIndexBuffer() , 0 );
	
	

	g_d3dRender.SetCustomRenderState();

	

	size = m_shaderType->sizeTrans;
	shaderMap = &m_shaderMaps[ m_shaderType->baseTrans ];
	for( i = 0 ; i < size ; i++ , shaderMap++ )
	{
		if( shaderMap->frameFlag != m_frameFlag )  continue;

		shader = &g_shaderMng.m_shaders[ shaderMap->shader ];

		for( j = 0 ; j <= m_numLightmaps ; j++ )
		{
			if( shaderMap->LMFrameFlags[ j ] != m_frameFlag )  continue;			

			RenderCustom( shader , &shaderMap->LMMaps[ j ] , j );
		}
	}

	g_d3dRender.SetStreamSource( 0 , m_dynamicVB->GetD3DVertexBuffer() , m_dynamicVB->GetStrideSize() );

	size = m_shaderType->sizeDeformTrans;
	shaderMap = &m_shaderMaps[ m_shaderType->baseDefromTrans ];
	for( i = 0 ; i < size ; i++ , shaderMap++ )
	{
		if( shaderMap->frameFlag != m_frameFlag )  continue;

		shader = &g_shaderMng.m_shaders[ shaderMap->shader ];
		
		for( j = 0 ; j <= m_numLightmaps ; j++ )
		{
			if( shaderMap->LMFrameFlags[ j ] != m_frameFlag )  continue;			

			RenderDeform( shader , &shaderMap->LMMaps[ j ] , j );
		}
	}
}

#define GTH_SIZE_RENDER_FACE_PER_CALL 1000
inline void CBSP::RenderDefault( const shaderLMmap_t *LMMap )
{
	int sizeRenderElems;
	leafbaseface_t *face , *faceLoop;
	face_t *renderFace;
	WORD *ptrIB;

	if( m_currBank != LMMap->bank )
	{
		g_d3dRender.SetStreamSource( 0 , m_staticVB[ LMMap->bank ]->GetD3DVertexBuffer() 
		                            , m_staticVB[ LMMap->bank ]->GetStrideSize() );
		m_currBank = LMMap->bank;
	}

	faceLoop = LMMap->linked;
	while( faceLoop )
	{
		sizeRenderElems = 0;
		face = faceLoop;
		while( face && sizeRenderElems <= GTH_SIZE_RENDER_FACE_PER_CALL )
		{
			sizeRenderElems += m_faces[ face->idxOriginFace ].sizeEB;
			face = face->next;
		}

		if( sizeRenderElems <= 3 )
		{
			faceLoop = face;
			continue;
		}

		ptrIB = (WORD *)m_dynamicIB->Lock( sizeRenderElems );
		sizeRenderElems = 0;
		face = faceLoop;
		while( face && sizeRenderElems <= GTH_SIZE_RENDER_FACE_PER_CALL )
		{
			renderFace = &m_faces[ face->idxOriginFace ];
			memcpy( ptrIB , &m_staticEB[ renderFace->offsetEB ] , renderFace->sizeEB * sizeof( WORD ) );
			ptrIB += renderFace->sizeEB;
			sizeRenderElems += renderFace->sizeEB;
			face = face->next;
		}
		m_dynamicIB->Unlock();

		m_totalRenderPolygon += sizeRenderElems / 3;
		g_d3dRender.RenderDefault( LMMap->baseVB , LMMap->sizeVB , m_dynamicIB->GetBase() , sizeRenderElems );

		faceLoop = face;
	}
}

inline void CBSP::RenderCustom( shader_t *shader , const shaderLMmap_t *LMMap , const int idxLM )
{
	int sizeRenderElems;
	leafbaseface_t *face , *faceLoop;
	face_t *renderFace;
	WORD* ptrIB;
	DWORD flags;
	DWORD genRGBColor = 0x00000000;
	DWORD genAlphaColor;
	float color;
	int i;

	if( m_currBank != LMMap->bank )
	{
		g_d3dRender.SetStreamSource( 0 , m_staticVB[ LMMap->bank ]->GetD3DVertexBuffer() 
		                            , m_staticVB[ LMMap->bank ]->GetStrideSize() );
		m_currBank = LMMap->bank;
	}

	faceLoop = LMMap->linked;
	while( faceLoop )
	{
		sizeRenderElems = 0;
		face = faceLoop;
		while( face && sizeRenderElems <= GTH_SIZE_RENDER_FACE_PER_CALL )
		{
			sizeRenderElems += m_faces[ face->idxOriginFace ].sizeEB;
			face = face->next;
		}

		if( sizeRenderElems <= 3 )
		{
			faceLoop = face;
			continue;
		}

		ptrIB = (WORD *)m_dynamicIB->Lock( sizeRenderElems );
		sizeRenderElems = 0;
		face = faceLoop;
		while( face && sizeRenderElems <= GTH_SIZE_RENDER_FACE_PER_CALL )
		{
			renderFace = &m_faces[ face->idxOriginFace ];
			memcpy( ptrIB , &m_staticEB[ renderFace->offsetEB ] , renderFace->sizeEB * sizeof( WORD ) );
			ptrIB += renderFace->sizeEB;
			sizeRenderElems += renderFace->sizeEB;
			face = face->next;
		}
		m_dynamicIB->Unlock();

		shaderpass_t *pass = &shader->passes[0];
		for( i = 0 ; i < shader->numPasses ; i++ , pass++ )
		{
			flags = pass->flags;

			if( flags & SHADERPASS_MAP ){;}
			else if( flags & SHADERPASS_MAP_LIGHTMAP )
			{
				if( idxLM != m_numLightmaps )
					pass->texRef = m_lmapTexRefs[ idxLM ];
				else
					pass->texRef = g_texMng.GetWhiteImageRef();
			}
			else if( flags & SHADERPASS_MAP_ANIMMAP )
			{
				pass->texRef = pass->animMap->GetFrame( g_timer.GetAppTime() );
			}
			else if( flags & SHADERPASS_MAP_WHITEIMAGE )
			{
				pass->texRef = g_texMng.GetWhiteImageRef();
			}

			if( flags & SHADERPASS_RGBGEN )
			{
				if( pass->rgbGen.type == SHADERPASS_RGBGEN_WAVE )
				{
					color = g_shaderMng.EvaluateFunc( &pass->rgbGen.func ,  g_timer.GetAppTime() );
					color *= 255.0f;
					if( color < 0.0f ) color = 0.0f;
					else if( color > 255.0f ) color = 255.0f;

					genRGBColor = (DWORD) color;
					pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , genRGBColor , genRGBColor , genRGBColor );
				}
				else if( pass->rgbGen.type == SHADERPASS_RGBGEN_IDENTITY )
					pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
			}

			if( flags & SHADERPASS_ALPHAGEN )
			{
				if( pass->alphaGen.type == SHADERPASS_RGBGEN_WAVE )
				{
					color = g_shaderMng.EvaluateFunc( &pass->alphaGen.func ,  g_timer.GetAppTime() );
					color *= 255.0f;
					if( color < 0.0f ) color = 0.0f;
					else if( color > 255.0f ) color = 255.0f;

					genAlphaColor = (DWORD) color;
					pass->alphaGen.color = MAKE_COLOR_ARGB( genAlphaColor , genRGBColor , genRGBColor , genRGBColor );
				}
				else if( pass->alphaGen.type == SHADERPASS_RGBGEN_IDENTITY )
					pass->alphaGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
			}

			if( flags & SHADERPASS_TCMOD )
				g_shaderMng.EvaluateTcMod( pass , g_timer.GetAppTime() );
		}

		if( shader->flags & SHADER_TCGEN )
		{
			shader->tcGenMatrix.Identity();
			shader->tcGenMatrix.zx -= g_camera.m_angles[YAW] / 360.0f;
			shader->tcGenMatrix.zy -= g_camera.m_angles[PITCH] / 180.0f;
		}

		m_totalRenderPolygon += sizeRenderElems / 3;
		g_d3dRender.RenderCustom(shader, LMMap->baseVB, LMMap->sizeVB, m_dynamicIB->GetBase(), sizeRenderElems);

		faceLoop = face;
	}
}


inline void CBSP::RenderSky( shader_t *shader , const shaderLMmap_t *LMMap , const int idxLM )
{
	int sizeRenderElems;
	leafbaseface_t *face , *faceLoop;
	face_t *renderFace;
	WORD* ptrIB;
	DWORD flags;
	DWORD genRGBColor = 0x00000000;
	DWORD genAlphaColor;
	float color;
	int i;

	if( m_currBank != LMMap->bank )
	{
		g_d3dRender.SetStreamSource( 0 , m_staticVB[ LMMap->bank ]->GetD3DVertexBuffer() 
		                            , m_staticVB[ LMMap->bank ]->GetStrideSize() );
		m_currBank = LMMap->bank;
	}

	faceLoop = LMMap->linked;
	while( faceLoop )
	{
		sizeRenderElems = 0;
		face = faceLoop;
		while( face && sizeRenderElems <= GTH_SIZE_RENDER_FACE_PER_CALL )
		{
			sizeRenderElems += m_faces[ face->idxOriginFace ].sizeEB;
			face = face->next;
		}

		if( sizeRenderElems <= 3 )
		{
			faceLoop = face;
			continue;
		}

		ptrIB = (WORD *)m_dynamicIB->Lock( sizeRenderElems );
		sizeRenderElems = 0;
		face = faceLoop;
		while( face && sizeRenderElems <= GTH_SIZE_RENDER_FACE_PER_CALL )
		{
			renderFace = &m_faces[ face->idxOriginFace ];
			memcpy( ptrIB , &m_staticEB[ renderFace->offsetEB ] , renderFace->sizeEB * sizeof( WORD ) );
			ptrIB += renderFace->sizeEB;
			sizeRenderElems += renderFace->sizeEB;
			face = face->next;
		}
		m_dynamicIB->Unlock();

		shaderpass_t *pass = &shader->passes[0];
		for( i = 0 ; i < shader->numPasses ; i++ , pass++ )
		{
			flags = pass->flags;

			if( flags & SHADERPASS_MAP ){;}
			else if( flags & SHADERPASS_MAP_LIGHTMAP )
			{
				if( idxLM != m_numLightmaps )
					pass->texRef = m_lmapTexRefs[ idxLM ];
				else
					pass->texRef = g_texMng.GetWhiteImageRef();
			}
			else if( flags & SHADERPASS_MAP_ANIMMAP )
			{
				pass->texRef = pass->animMap->GetFrame( g_timer.GetAppTime() );
			}
			else if( flags & SHADERPASS_MAP_WHITEIMAGE )
			{
				pass->texRef = g_texMng.GetWhiteImageRef();
			}

			if( flags & SHADERPASS_RGBGEN )
			{
				if( pass->rgbGen.type == SHADERPASS_RGBGEN_WAVE )
				{
					color = g_shaderMng.EvaluateFunc( &pass->rgbGen.func ,  g_timer.GetAppTime() );
					color *= 255.0f;
					if( color < 0.0f ) color = 0.0f;
					else if( color > 255.0f ) color = 255.0f;

					genRGBColor = (DWORD) color;
					pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , genRGBColor , genRGBColor , genRGBColor );
				}
				else if( pass->rgbGen.type == SHADERPASS_RGBGEN_IDENTITY )
					pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
			}

			if( flags & SHADERPASS_ALPHAGEN )
			{
				if( pass->alphaGen.type == SHADERPASS_RGBGEN_WAVE )
				{
					color = g_shaderMng.EvaluateFunc( &pass->alphaGen.func ,  g_timer.GetAppTime() );
					color *= 255.0f;
					if( color < 0.0f ) color = 0.0f;
					else if( color > 255.0f ) color = 255.0f;

					genAlphaColor = (DWORD) color;
					pass->alphaGen.color = MAKE_COLOR_ARGB( genAlphaColor , genRGBColor , genRGBColor , genRGBColor );
				}
				else if( pass->alphaGen.type == SHADERPASS_RGBGEN_IDENTITY )
					pass->alphaGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
			}

			if( flags & SHADERPASS_TCMOD )
				g_shaderMng.EvaluateTcMod( pass , g_timer.GetAppTime() );
		}

		if( shader->flags & SHADER_TCGEN )
		{
			shader->tcGenMatrix.Identity();
			shader->tcGenMatrix.zx -= g_camera.m_angles[YAW] / 360.0f;
			shader->tcGenMatrix.zy -= g_camera.m_angles[PITCH] / 180.0f;
		}

		m_totalRenderPolygon += sizeRenderElems / 3;
		g_d3dRender.RenderSky(shader, LMMap->baseVB, LMMap->sizeVB, m_dynamicIB->GetBase(), sizeRenderElems);

		faceLoop = face;
	}
}


inline void CBSP::RenderDeform( shader_t *shader , const shaderLMmap_t *LMMap , const int idxLM )
{
	int sizeRenderElems;
	int sizeRenderVerts;
	leafbaseface_t *face , *faceLoop;
	face_t *renderFace;
	WORD     *ptrIB , *elem;
	vertex_t *ptrVB;
	WORD  baseIdxVB;
	DWORD flags;
	DWORD genRGBColor = 0x00000000;
	DWORD genAlphaColor;
	vec3_t tmpVect;
	vertex_t *vert;
	float color , offset , deflect;
	int i , size;

	faceLoop = LMMap->linked;
	while( faceLoop )
	{
		sizeRenderVerts = 0;
		sizeRenderElems = 0;
		face = faceLoop;
		while( face && sizeRenderElems <= GTH_SIZE_RENDER_FACE_PER_CALL )
		{
			sizeRenderVerts += m_faces[ face->idxOriginFace ].sizeVB;
			sizeRenderElems += m_faces[ face->idxOriginFace ].sizeEB;
			face = face->next;
		}

		if( sizeRenderElems <= 3 )
		{
			faceLoop = face;
			continue;
		}

		ptrVB = (vertex_t *) m_dynamicVB->Lock( sizeRenderVerts );
		ptrIB = (WORD *)     m_dynamicIB->Lock( sizeRenderElems );
		baseIdxVB = m_dynamicVB->GetBase();

		sizeRenderVerts = 0;
		sizeRenderElems = 0;
		face = faceLoop;
		while( face && sizeRenderElems <= GTH_SIZE_RENDER_FACE_PER_CALL )
		{
			renderFace = &m_faces[ face->idxOriginFace ];
			vert = &m_deformVB[ renderFace->offsetVB ];
			size = renderFace->sizeVB;	
			for( i = 0 ; i < size ; i++ , ptrVB++ , vert++ )
			{
				offset = ( vert->point[0] + vert->point[1] + vert->point[2] ) * shader->deform.div;
				
				deflect = g_shaderMng.EvaluateFunc( &shader->deform.func , offset , g_timer.GetAppTime() );
				
				VectorCopy ( tmpVect , vert->normal );
				VectorScale( tmpVect , tmpVect , deflect );
				VectorAdd  ( tmpVect , tmpVect , vert->point );

				VectorCopy ( ptrVB->point  , tmpVect );
				VectorCopy ( ptrVB->normal , vert->normal );
				Vector2Copy( ptrVB->texUV  , vert->texUV );
				Vector2Copy( ptrVB->lmapUV , vert->lmapUV );
				ColorCopy  ( ptrVB->diffuse , vert->diffuse );
			}

			size = renderFace->sizeEB;
			elem = &m_staticEB[ renderFace->offsetEB ];
			for( i = 0 ; i < size ; i++ , ptrIB++ , elem++ )
				*ptrIB = *elem + baseIdxVB;

			baseIdxVB += renderFace->sizeVB;

			sizeRenderVerts += renderFace->sizeVB;
			sizeRenderElems += renderFace->sizeEB;
			face = face->next;
		}

		m_dynamicVB->Unlock();
		m_dynamicIB->Unlock();

		shaderpass_t *pass = &shader->passes[0];
		for( i = 0 ; i < shader->numPasses ; i++ , pass++ )
		{
			flags = pass->flags;

			if( flags & SHADERPASS_MAP ){;}
			else if( flags & SHADERPASS_MAP_LIGHTMAP )
			{
				if( idxLM != m_numLightmaps )
					pass->texRef = m_lmapTexRefs[ idxLM ];
				else
					pass->texRef = g_texMng.GetWhiteImageRef();
			}
			else if( flags & SHADERPASS_MAP_ANIMMAP )
			{
				pass->texRef = pass->animMap->GetFrame( g_timer.GetAppTime() );
			}
			else if( flags & SHADERPASS_MAP_WHITEIMAGE )
			{
				pass->texRef = g_texMng.GetWhiteImageRef();
			}

			if( flags & SHADERPASS_RGBGEN )
			{
				if( pass->rgbGen.type == SHADERPASS_RGBGEN_WAVE )
				{
					color = g_shaderMng.EvaluateFunc( &pass->rgbGen.func ,  g_timer.GetAppTime() );
					color *= 255.0f;
					if( color < 0.0f ) color = 0.0f;
					else if( color > 255.0f ) color = 255.0f;

					genRGBColor = (DWORD) color;
					pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , genRGBColor , genRGBColor , genRGBColor );
				}
				else if( pass->rgbGen.type == SHADERPASS_RGBGEN_IDENTITY )
					pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
			}

			if( flags & SHADERPASS_ALPHAGEN )
			{
				if( pass->alphaGen.type == SHADERPASS_RGBGEN_WAVE )
				{
					color = g_shaderMng.EvaluateFunc( &pass->alphaGen.func ,  g_timer.GetAppTime() );
					color *= 255.0f;
					if( color < 0.0f ) color = 0.0f;
					else if( color > 255.0f ) color = 255.0f;

					genAlphaColor = (DWORD) color;
					pass->alphaGen.color = MAKE_COLOR_ARGB( genAlphaColor , genRGBColor , genRGBColor , genRGBColor );
				}
				else if( pass->alphaGen.type == SHADERPASS_RGBGEN_IDENTITY )
					pass->alphaGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
			}

			if( flags & SHADERPASS_TCMOD )
				g_shaderMng.EvaluateTcMod( pass , g_timer.GetAppTime() );
		}

		if( shader->flags & SHADER_TCGEN )
		{
			shader->tcGenMatrix.Identity();
			shader->tcGenMatrix.zx -= g_camera.m_angles[ YAW ] / 360.0f;
			shader->tcGenMatrix.zy -= g_camera.m_angles[ PITCH ] / 180.0f;
		}

		m_totalRenderPolygon += sizeRenderElems / 3;
		g_d3dRender.RenderCustom( shader , m_dynamicVB->GetBase() , sizeRenderVerts , m_dynamicIB->GetBase() , sizeRenderElems );

		faceLoop = face;
	}
}


