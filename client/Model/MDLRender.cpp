#include "../global_def.h"

void CMDLMng::SetCamera( vec3_t pos , vec3_t dir )
{
	VectorCopy( m_camPos , pos );
	VectorCopy( m_camDir , dir );
}

void CMDLMng::Render()
{
	m_totalRenderPolygon = 0;
	m_currBank = -1;
	m_D3DRender->SetVertexShader( m_format );
	m_dynamicIB->Discard();
	m_dynamicVB->Discard();

	m_currTime += m_timer->GetElapsedTime();
	m_currMilliTime += m_timer->GetElapsedMilliTime();

	m_D3DRender->SetMDLRenderState();

	m_waveFunc.type = WAVE_SIN;
	m_waveFunc.base = 0.0f;
	m_waveFunc.phase = 0.0f;

	RenderWorldEntity();
	RenderItemEntity();
	 

	RenderEffectEntity_Neo ();
	RenderBlendWorldEntity();
}


void CMDLMng::RenderBound( CMDL *mdl )
{
	mdlboundvertex_t *vertex;
	int *elem , size;
	mdlvertex_t *ptrVB;
	WORD        *ptrIB;
	int i , baseIdxVB;
	mdlboundmesh_t *mesh;

	mesh = mdl->m_boundMeshes;
	
	ptrVB = (mdlvertex_t *) m_dynamicVB->Lock( mesh->numVertices );
	ptrIB = (WORD *)        m_dynamicIB->Lock( mesh->numElems );
	baseIdxVB = m_dynamicVB->GetBase();

	vertex = mdl->m_boundVertices;
	
	
	size = mesh->numVertices;
	for( i = 0 ; i < size ; i++ , ptrVB++ , vertex++ )
	{	
		VectorCopy( ptrVB->point  , vertex->point );
		VectorCopy( ptrVB->normal , vertex->normal );
		
		Vector2Set( ptrVB->texUV  , 0.0f , 0.0f );
	}

	elem = mdl->m_boundElems;
	size = mesh->numElems;
	for( i = 0 ; i < size ; i++ , ptrIB++ , elem++ )
		*ptrIB = (WORD) ( *elem + baseIdxVB );

	m_dynamicVB->Unlock();
	m_dynamicIB->Unlock();

	m_D3DRender->SetTexture( 0 , m_texMng->GetD3DTexture( 0 ) );
	m_D3DRender->RenderDefault( m_dynamicVB->GetBase() , mesh->numVertices , m_dynamicIB->GetBase() , mesh->numElems );
}

void CMDLMng::PrepareMDL( CMDL *mdl , mdlmesh_t *mesh )
{
	
		PrepareMDLMesh( mdl , mesh );
}

void CMDLMng::PrepareTCGenMDLMesh( CMDL *mdl , mdlmesh_t *mesh )
{
	mdlvertex_t *vertex;
	int *elem , numVertices , numElems;
	mdlvertex_t *ptrVB , *ptrTCGenVB;
	WORD        *ptrIB , *ptrTCGenIB;
	int i , baseIdxVB;
	float m00, m10, m20;
	float m01, m11, m21;
	float m02, m12, m22;
	
	m00 = m_envMat->xx,   m10 = m_envMat->yx,   m20 = m_envMat->zx;
	m01 = m_envMat->xy,   m11 = m_envMat->yy,   m21 = m_envMat->zy;
	m02 = m_envMat->xz,   m12 = m_envMat->yz,   m22 = m_envMat->zz;

	ptrVB = (mdlvertex_t *) m_dynamicVB->Lock( mesh->numVertices * 2 );
	ptrIB = (WORD *)        m_dynamicIB->Lock( mesh->numElems * 2 );
	baseIdxVB = m_dynamicVB->GetBase();
	ptrTCGenVB = ptrVB + mesh->numVertices;
	ptrTCGenIB = ptrIB + mesh->numElems;

	vertex = &mdl->m_vertices[ mesh->vertexOffset ];
	
	
	numVertices = mesh->numVertices;
	for( i = 0 ; i < numVertices ; i++ , ptrVB++ , ptrTCGenVB++ , vertex++ )
	{	
		
            

		VectorCopy( ptrVB->point  , vertex->point );
		VectorCopy( ptrVB->normal , vertex->normal );
		Vector2Copy( ptrVB->texUV  , vertex->texUV );

		VectorCopy( ptrTCGenVB->point  , vertex->point );
		VectorCopy( ptrTCGenVB->normal , vertex->normal );
		ptrTCGenVB->texUV[0] = 0.5f * ( 1.0f + ( vertex->normal[0] * m00 + vertex->normal[1] * m10 + vertex->normal[2] * m20 ) );
		ptrTCGenVB->texUV[1] = 0.5f * ( 1.0f - ( vertex->normal[0] * m02 + vertex->normal[1] * m12 + vertex->normal[2] * m22 ) );
	}

	elem = &mdl->m_elems[ mesh->elemOffset ];
	numElems = mesh->numElems;
	for( i = 0 ; i < numElems ; i++ , ptrIB++ , ptrTCGenIB++ , elem++ )
	{
		*ptrIB      = (WORD) ( *elem + baseIdxVB );
		*ptrTCGenIB = (WORD) ( *elem + baseIdxVB + numVertices );
	}

	m_dynamicVB->Unlock();
	m_dynamicIB->Unlock();

	mesh->offsetVB = m_dynamicVB->GetBase();
	mesh->offsetEB = m_dynamicIB->GetBase();
	mesh->offsetTCGenVB = mesh->offsetVB + mesh->numVertices;
	mesh->offsetTCGenEB = mesh->offsetEB + mesh->numElems;
}

void CMDLMng::PrepareMDLMesh( CMDL *mdl , mdlmesh_t *mesh )
{
	mdlvertex_t *vertex;
	int *elem , size;
	mdlvertex_t *ptrVB;
	WORD        *ptrIB;
	int i , baseIdxVB;
	
	ptrVB = (mdlvertex_t *) m_dynamicVB->Lock( mesh->numVertices );
	ptrIB = (WORD *)        m_dynamicIB->Lock( mesh->numElems );
	baseIdxVB = m_dynamicVB->GetBase();

	vertex = &mdl->m_vertices[ mesh->vertexOffset ];
	
	memcpy( ptrVB , vertex , sizeof( mdlvertex_t ) * mesh->numVertices );
	

	elem = &mdl->m_elems[ mesh->elemOffset ];
	size = mesh->numElems;
	for( i = 0 ; i < size ; i++ , ptrIB++ , elem++ )
		*ptrIB = (WORD) ( *elem + baseIdxVB );

	m_dynamicVB->Unlock();
	m_dynamicIB->Unlock();

	mesh->offsetVB = m_dynamicVB->GetBase();
	mesh->offsetEB = m_dynamicIB->GetBase();
	mesh->offsetTCGenVB = mesh->offsetVB;
	mesh->offsetTCGenEB = mesh->offsetEB;
}

void CMDLMng::PrepareInterpolateMDL( CMDL *mdl , mdlmesh_t *mesh )
{
	
		PrepareInterpolateMDLMesh( mdl , mesh );
}

void CMDLMng::PrepareInterpolateTCGenMDLMesh( CMDL *mdl , mdlmesh_t *mesh )
{
	int endFrame;
	mdlvertex_t *vertexA , *vertexB;
	int *elem , numVertices , numElems;
	mdlvertex_t *ptrVB , *ptrTCGenVB;
	WORD        *ptrIB , *ptrTCGenIB;
	int i , baseIdxVB;
	float m00, m10, m20;
	
	float m02, m12, m22;
	vec3_t  point , normal;
	
	m00 = m_envMat->xx,   m10 = m_envMat->yx,   m20 = m_envMat->zx;
	
	m02 = m_envMat->xz,   m12 = m_envMat->yz,   m22 = m_envMat->zz;
	
	
	

	endFrame = m_currFrame + 1;
	if( endFrame >= mesh->numMeshFrames )
		endFrame -= mesh->numMeshFrames;
	
	ptrVB = (mdlvertex_t *) m_dynamicVB->Lock( mesh->numVertices * 2 );
	ptrIB = (WORD *)        m_dynamicIB->Lock( mesh->numElems * 2 );
	baseIdxVB = m_dynamicVB->GetBase();
	ptrTCGenVB = ptrVB + mesh->numVertices;
	ptrTCGenIB = ptrIB + mesh->numElems;

	vertexA = &mdl->m_vertices[ mesh->vertexOffset + mesh->numVertices * m_currFrame ];
	vertexB = &mdl->m_vertices[ mesh->vertexOffset + mesh->numVertices * endFrame ];

	numVertices = mesh->numVertices;
	for( i = 0 ; i < numVertices ; i++ , ptrVB++ , ptrTCGenVB++ , vertexA++ , vertexB++ )
	{	
		VectorInterpolate( point  , vertexA->point  , vertexB->point  , m_interPol );
		VectorInterpolate( normal , vertexA->normal , vertexB->normal , m_interPol );

		VectorCopy( ptrVB->point  , point );
		VectorCopy( ptrVB->normal , normal );
		Vector2Copy( ptrVB->texUV  , vertexA->texUV );

		VectorCopy( ptrTCGenVB->point  , point );
		VectorCopy( ptrTCGenVB->normal , normal );
		ptrTCGenVB->texUV[0] = 0.5f * ( 1.0f + ( normal[0] * m00 + normal[1] * m10 + normal[2] * m20 ) );
		ptrTCGenVB->texUV[1] = 0.5f * ( 1.0f - ( normal[0] * m02 + normal[1] * m12 + normal[2] * m22 ) );
	}

	elem = &mdl->m_elems[ mesh->elemOffset ];
	numElems = mesh->numElems;
	for( i = 0 ; i < numElems ; i++ , ptrIB++ , ptrTCGenIB++ , elem++ )
	{
		*ptrIB      = (WORD) ( *elem + baseIdxVB );
		*ptrTCGenIB = (WORD) ( *elem + baseIdxVB + numVertices );
	}
	
	m_dynamicVB->Unlock();
	m_dynamicIB->Unlock();

	mesh->offsetVB = m_dynamicVB->GetBase();
	mesh->offsetEB = m_dynamicIB->GetBase();
	mesh->offsetTCGenVB = mesh->offsetVB + mesh->numVertices;
	mesh->offsetTCGenEB = mesh->offsetEB + mesh->numElems;
}


void CMDLMng::PrepareInterpolateMDLMesh( CMDL *mdl , mdlmesh_t *mesh )
{
	int endFrame;
	mdlvertex_t *vertexA , *vertexB;
	int *elem , size;
	mdlvertex_t *ptrVB;
	WORD        *ptrIB;
	int i , baseIdxVB;
	
	
	

	ptrVB = (mdlvertex_t *) m_dynamicVB->Lock( mesh->numVertices );
	ptrIB = (WORD *)        m_dynamicIB->Lock( mesh->numElems );
	baseIdxVB = m_dynamicVB->GetBase();

	endFrame = m_currFrame + 1;
	if( endFrame >= mesh->numMeshFrames )
		endFrame -= mesh->numMeshFrames;
	
	{
		vertexA = &mdl->m_vertices[ mesh->vertexOffset + mesh->numVertices * m_currFrame ];
		vertexB = &mdl->m_vertices[ mesh->vertexOffset + mesh->numVertices * endFrame ];

		size = mesh->numVertices;
		for( i = 0 ; i < size ; i++ , ptrVB++ , vertexA++ , vertexB++ )
		{	
			VectorInterpolate( ptrVB->point  , vertexA->point  , vertexB->point  , m_interPol );
			VectorInterpolate( ptrVB->normal , vertexA->normal , vertexB->normal , m_interPol );
			Vector2Copy( ptrVB->texUV  , vertexA->texUV );
		}
	}

	elem = &mdl->m_elems[ mesh->elemOffset ];
	size = mesh->numElems;
	for( i = 0 ; i < size ; i++ , ptrIB++ , elem++ )
		*ptrIB = (WORD) ( *elem + baseIdxVB );

	m_dynamicVB->Unlock();
	m_dynamicIB->Unlock();

	mesh->offsetVB = m_dynamicVB->GetBase();
	mesh->offsetEB = m_dynamicIB->GetBase();
	mesh->offsetTCGenVB = mesh->offsetVB;
	mesh->offsetTCGenEB = mesh->offsetEB;
}

void CMDLMng::RenderBBox( vec3_t mins , vec3_t maxs )
{
	mdlvertex_t *ptrVB;
	WORD    *ptrIB;
	int  i , baseIdxVB;
	vec3_t normal;
	matrix4x4_t worldMat;
	WORD bboxIdx[36] = { 0, 3, 2, 0, 2, 1,
		                 1, 5, 0, 0, 5, 4,
						 4, 7, 0, 0, 7, 3,
						 3, 7, 2, 2, 7, 6,
						 6, 5, 2, 2, 5, 1,
						 5, 6, 4, 4, 6, 7 };
	
	ptrVB = (mdlvertex_t *) m_dynamicVB->Lock( 8 );
	ptrIB = (WORD *)        m_dynamicIB->Lock( 36 );
	baseIdxVB = m_dynamicVB->GetBase();

	VectorSet( normal , 1.0f , 1.0f , 1.0f );

	
	VectorSet ( ptrVB->point  , mins[0] , mins[1] , mins[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	ptrVB++;

	
	VectorSet ( ptrVB->point  , mins[0] , maxs[1] , mins[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	ptrVB++;

	
	VectorSet ( ptrVB->point  , maxs[0] , maxs[1] , mins[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	ptrVB++;

	
	VectorSet ( ptrVB->point  , maxs[0] , mins[1] , mins[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	ptrVB++;

	
	VectorSet ( ptrVB->point  , mins[0] , mins[1] , maxs[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	ptrVB++;

	
	VectorSet ( ptrVB->point  , mins[0] , maxs[1] , maxs[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	ptrVB++;

	
	VectorSet ( ptrVB->point  , maxs[0] , maxs[1] , maxs[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	ptrVB++;

	
	VectorSet ( ptrVB->point  , maxs[0] , mins[1] , maxs[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	
	for( i = 0 ; i < 36 ; i++ , ptrIB++ )
		*ptrIB = (WORD) ( bboxIdx[i] + baseIdxVB );
	
	m_dynamicVB->Unlock();
	m_dynamicIB->Unlock();

	
	LPDIRECT3DDEVICE8 pd3dDevice = g_d3dDevice.GetD3DDevice();

	worldMat.Identity();
	m_D3DRender->SetTransform( D3DTS_WORLD , &worldMat );

	m_D3DRender->SetVertexShader( m_format );
	m_D3DRender->SetMDLRenderState();

	m_D3DRender->SetStreamSource( 0 , m_dynamicVB->GetD3DVertexBuffer() , m_dynamicVB->GetStrideSize() );
	m_D3DRender->SetIndices( m_dynamicIB->GetD3DIndexBuffer() , 0 );

	
	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	m_D3DRender->SetTexture( 0 , m_texMng->GetD3DTexture( 0 ) );
	m_D3DRender->RenderDefault( m_dynamicVB->GetBase() , 8 , m_dynamicIB->GetBase() , 36 );

	ptrVB = (mdlvertex_t *) m_dynamicVB->Lock( 2 );

	
	VectorSet ( ptrVB->point  , mins[0] , mins[1] , mins[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	ptrVB++;
	
	VectorSet ( ptrVB->point  , maxs[0] , maxs[1] , maxs[2] );
	VectorCopy( ptrVB->normal , normal );
	Vector2Set( ptrVB->texUV  , 0.5f , 0.5f );
	
	m_dynamicVB->Unlock();

	float PointSize = 10.0f;

	
	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
	pd3dDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&PointSize));
	pd3dDevice->DrawPrimitive( D3DPT_POINTLIST , m_dynamicVB->GetBase() , 2 );
	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}


void CMDLMng::ClearItemLinkedCount()
{
	memset( m_linkedVisibleCount.itemClasses , 0 , sizeof( int ) * m_numMDLItemClasses );
}

void CMDLMng::ClearEffectLinkedCount()
{
	memset( m_linkedVisibleCount.effectClasses , 0 , sizeof( int ) * m_numMDLEffectClasses );
}

void CMDLMng::ClearRenderWorldLink()
{
	memset( m_linkedRenderMDLWorldEntities , 0 , sizeof( mdlworldentity_t * ) * m_numMDLWorldClasses );
}

void CMDLMng::ClearBlendWorldLink()
{
	memset( m_linkedBlendMDLWorldEntities , 0 , sizeof( mdlworldentity_t * ) * m_numMDLWorldClasses );
}