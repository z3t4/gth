#include "bsp.h"
#include "d3drender.h"













void  CBSP::SetShadowRenderState()
{
	g_d3dRender.SetVertexShader( m_shadowFormat );
	g_d3dRender.SetStreamSource( 0 , m_shadowVB->GetD3DVertexBuffer() , m_shadowVB->GetStrideSize() );
	g_d3dRender.SetIndices( m_dynamicIB->GetD3DIndexBuffer() , 0 );
	
	m_shadowVB->Discard();

	g_d3dRender.SetShadowRenderState();
}

void CBSP::RenderProjectiveShadow( vec3_t p1 , vec3_t p2 , vec3_t p3 , vec3_t p4 , LPDIRECT3DTEXTURE8 d3dTex )
{
	vec3_t origin , sub;
	float  dist , size , gridSize;
	int rowStart , rowEnd , colStart , colEnd;
	gridfacelump_t *faceLump;
	gridfacelist_t *faceList;
	face_t *face;
	int  i , j , k , l;
	vertex_t *vert;
	vec3_t pointOnPlane , diff;
	shadowvertex_t *ptrVB;
	WORD  *ptrIB , *elem;
	WORD   baseIdxVB , offsetVB;
	int    sizeLoop;
	int sizeRenderElems;
	int sizeRenderVerts;

	VectorAdd( origin , p1 , p3 );
	VectorScale( origin , origin , 0.5f );

	VectorSubtract( sub , p3 , origin );
	size = (float) sqrt( VectorDotProduct( sub , sub ) );

	gridSize = m_gridFaceLumpHead->gridSize;

	colStart = (int) ( ( origin[0] - size - m_gridFaceLumpHead->mins[0] ) / gridSize );
	colEnd   = (int) ( ( origin[0] + size - m_gridFaceLumpHead->mins[0] + gridSize - 1.0f ) / gridSize );
	
	rowStart = (int) ( ( origin[1] - size - m_gridFaceLumpHead->mins[1] ) / gridSize );
	rowEnd   = (int) ( ( origin[1] + size - m_gridFaceLumpHead->mins[1] + gridSize - 1.0f ) / gridSize );

	g_d3dRender.SetTexture( 0 , d3dTex );

	m_shadowFrameFlag++;

	sizeRenderVerts = 0;
	sizeRenderElems = 0;
	for( i = rowStart ; i <= rowEnd ; i++ )
	{
		for( j = colStart ; j <= colEnd ; j++ )
		{
			faceLump = &m_gridFaceLumps[ i * m_gridFaceLumpHead->width + j ];
			faceList = &m_gridFaceLists[ faceLump->base ];

			for( k = 0 ; k < faceLump->sizeLumps ; faceList++ , k++ )
			{
				face = &m_faces[ *faceList ];

				if( face->type == FACETYPE_PATCH ) continue;
				if( face->frameFlag == m_shadowFrameFlag ) continue;
				else
					face->frameFlag = m_shadowFrameFlag;

				sizeRenderVerts += face->sizeVB;
				sizeRenderElems += face->sizeEB;
			}
		}
	}

	m_shadowFrameFlag++;

	ptrVB = (shadowvertex_t *) m_shadowVB->Lock( sizeRenderVerts );
	ptrIB = (WORD *) m_dynamicIB->Lock( sizeRenderElems );
	baseIdxVB  = m_shadowVB->GetBase();

	for( i = rowStart ; i <= rowEnd ; i++ )
	{
		for( j = colStart ; j <= colEnd ; j++ )
		{
			faceLump = &m_gridFaceLumps[ i * m_gridFaceLumpHead->width + j ];
			faceList = &m_gridFaceLists[ faceLump->base ];

			for( k = 0 ; k < faceLump->sizeLumps ; faceList++ , k++ )
			{
				face = &m_faces[ *faceList ];

				if( face->type == FACETYPE_PATCH ) continue;
				if( face->frameFlag == m_shadowFrameFlag ) continue;
				else
					face->frameFlag = m_shadowFrameFlag;

				dist = VectorDotProduct( origin , face->planeNormal ) - face->planeDist;

				

				VectorProjectOnPlane( pointOnPlane , origin , face->planeRight , face->planeUp );
				
				if( m_shaderMaps[ face->shader ].flags & SHADER_DEFORMVERTEXES )
					offsetVB = 0;
				else
					offsetVB = (WORD) face->offsetVB;

				sizeLoop = face->sizeEB;
				elem = &m_staticEB[ face->offsetEB ];
				for( l = 0 ; l < sizeLoop ; l++ , ptrIB++ , elem++ )
					*ptrIB = *elem - offsetVB + baseIdxVB;

				sizeLoop = face->sizeVB;
				vert = &m_vertices[ face->vertexOffset ];
				for( l = 0 ; l < sizeLoop ; vert++ , ptrVB++ , l++ )
				{
					VectorCopy( ptrVB->point , vert->point );
					
					VectorCopy( ptrVB->normal , vert->normal );

					VectorSubtract( diff , vert->point , pointOnPlane );

					ptrVB->texUV[0] = ( ( VectorDotProduct( diff , face->planeRight ) ) / ( ( size + dist ) * 2 ) ) + 0.5f;
					ptrVB->texUV[1] = ( ( VectorDotProduct( diff , face->planeUp    ) ) / ( ( size + dist ) * 2 ) ) + 0.5f;
				}

				baseIdxVB += face->sizeVB;
			}
		}
	}

	m_shadowVB->Unlock();
	m_dynamicIB->Unlock();

	m_totalRenderPolygon += sizeRenderElems / 3;
	g_d3dRender.RenderDefault( m_shadowVB->GetBase() , sizeRenderVerts , m_dynamicIB->GetBase() , sizeRenderElems );
}

 
 
 
 
 
bool CBSP::RenderProjectiveShadowNeo( float in_size, vec3_t p1, CD3DBuffer* pBuffer, int& vertexSize )
{
	

	
	float  size , gridSize;
	int rowStart , rowEnd , colStart , colEnd;
	gridfacelump_t *faceLump;
	gridfacelist_t *faceList;
	face_t *face;
	int  i;
	int  j;
	int  k;
	






	




	size = in_size;

	gridSize = m_gridFaceLumpHead->gridSize;

	colStart = (int) ( ( p1[0] - size - m_gridFaceLumpHead->mins[0] ) / gridSize );
	colEnd   = (int) ( ( p1[0] + size - m_gridFaceLumpHead->mins[0] + gridSize - 1.0f ) / gridSize );
	
	rowStart = (int) ( ( p1[1] - size - m_gridFaceLumpHead->mins[1] ) / gridSize );
	rowEnd   = (int) ( ( p1[1] + size - m_gridFaceLumpHead->mins[1] + gridSize - 1.0f ) / gridSize );

	if	(	(	colStart	<	0	)	||
			(	colEnd		<	0	)	||
			(	rowStart	<	0	)	||
			(	rowEnd		<	0	)	)	return	false;

	m_shadowFrameFlag++;

	for( i = rowStart ; i <= rowEnd ; i++ )
	{
		for( j = colStart ; j <= colEnd ; j++ )
		{
			faceLump = &m_gridFaceLumps[ i * m_gridFaceLumpHead->width + j ];
			faceList = &m_gridFaceLists[ faceLump->base ];

			for( k = 0 ; k < faceLump->sizeLumps ; faceList++ , k++ )
			{
				face = &m_faces[ *faceList ];

				if( face->type == FACETYPE_PATCH ) continue;
				if( face->frameFlag == m_shadowFrameFlag ) continue;
				else
					face->frameFlag = m_shadowFrameFlag;

				
				
				if	(!m_decal.Process( *face ))
				{
					vertexSize = m_decal.GetVertexSize ();
					if(vertexSize > 0 )
					{
							 
						pBuffer ->Lock ( m_decal .GetVertexSize () );
						m_decal.CopyVertex ( (void *)pBuffer ->GetBuffer() );
						pBuffer->Unlock();
						return	true;
					}
					else
					{
						vertexSize = 0;
						return false;
					}
				}
			}
		}
	}

	vertexSize = m_decal.GetVertexSize ();
	if(vertexSize > 0 )
	{
		pBuffer ->Lock ( m_decal .GetVertexSize () );
		m_decal.CopyVertex ( (void *)pBuffer ->GetBuffer() );
		pBuffer->Unlock();
		return true;
	}

    vertexSize = 0;
	return false;
}

void CBSP::SetDecalBuffer()
{
	m_decal .SetVertices ( m_vertices );
	m_decal .SetIndices ( m_elems );
}

void CBSP::SetDecalInit(matrix4x4_t* matShadow, vec3_t& vLight, vec3_t& sunAngle, float dispPS, float min, float max)
{
	m_decal .SetInvTransformMatrix ( matShadow );
	m_decal .SetLight ( vLight );
	m_decal .SetSunLight ( sunAngle );
	m_decal .SetSize ( dispPS );
	m_decal .Initialize ();
	m_decal .SetMinMaxHeight ( min, max );
}

	