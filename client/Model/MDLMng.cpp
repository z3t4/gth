



#include "../global_def.h"





CMDLMng::CMDLMng()
{
	m_numMDLs   = 0;
	m_frameFlag = 0;

	m_numMDLWorldClasses = 0;
	m_numMDLItemClasses = 0;
	m_numMDLEffectClasses = 0;
	
	m_currMDLWorldEntity = 0;
	m_currMDLItemEntity = 0;
	m_currMDLEffectEntity = 0;

	m_format = GTH_MODEL_FORMAT;
	
	m_pD3DDevice = NULL;
	
	
	m_numBank				=	0;
	m_numStaticElems		=	0;
	m_prevNumStaticVerts	=	0;
	m_currNumStaticVerts	=	0;

	
	
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus( &memStatus );
	
	if( memStatus.dwTotalPhys < 535740416 )
	{
		m_mdlRscSize = 31457280;
	}
	else
	{
		m_mdlRscSize = 31457280 * 2;
	}

	m_resManager.Create( m_mdlRscSize );

	m_dynamicVB = NULL;
	m_dynamicIB = NULL;
	m_staticEB  = NULL;

	m_currTime = 0.0f;
	m_currMilliTime = 0.0f;

	m_useFocusItem = true;
	m_useFocusWorld = true;

	m_focusedItemColor = MAKE_COLOR_ARGB( 0x40 , 0xFF , 0xFF , 0x00 );

}

CMDLMng::~CMDLMng()
{
	int i;
		
	int j;
		

	GTH_SAFE_DELETE( m_dynamicVB );
	GTH_SAFE_DELETE( m_dynamicIB );
	GTH_SAFE_DELETE( m_staticEB );

	for( i = 0 ; i < m_numBank ; i++ )
		GTH_SAFE_DELETE( m_staticVB[i] );

		
	mdlworldentity_t *entity = &m_MDLWorldEntities[0];
	for( i = 0; i < m_currMDLWorldEntity; i++, entity++ )
	{
		for( j = 0; j < WORLD_ENTITY_MAX_PARTS; j++ )
			if( entity->num_cvertex[j] > 0 )
			{
				 

				GTH_SAFE_DELETE_ARRAY ( entity->collisionVertex[j] );
			}
			 
			entity->collisionVertex[j]	=	NULL;
	}
		
}

void CMDLMng::Release()
{
	int i;

	for( i = 0 ; i < m_numMDLs ; i++ )
		 m_MDLs[i].Release();

	m_dynamicVB->Release();
	m_dynamicIB->Release();
	
	if( m_staticEB )
		m_staticEB->Release();
	
	for( i = 0 ; i < m_numBank ; i++ )
		m_staticVB[i]->Release();
	
	m_shaderMng->Release();

	m_texMng->Release( OWNER_TYPE_MODEL );

	
	m_resManager.Create( m_mdlRscSize );
}



CMDL* CMDLMng::GetMDL( int mdlIdx )
{
	bool bIsDisposed	=	m_MDLs[mdlIdx].IsDisposed();

	if( m_resManager.GetResource( (RHANDLE)&m_MDLs[mdlIdx] ) == NULL ) 
	{

		return NULL;
	}
	
	if( bIsDisposed ) 
	{

		PrepareMDLShader( mdlIdx );
		RestoreVertexInfo( mdlIdx );
	}

	return( &m_MDLs[mdlIdx] );
}

int CMDLMng::PrepareMDLShader( int mdlIdx )
{
	int				idxShader;
	shader_t		*shader;
	mdlshaderref_t	*shaderRef;
	int				numShaderRefs;

	CMDL			*mdl			= &m_MDLs[mdlIdx];
	
	mdl->m_needDynVB	=	false;
	numShaderRefs		=	mdl->GetShaderRef( &shaderRef );

	for( int i = 0 ; i < numShaderRefs ; i++ )
	{
		idxShader = m_shaderMng->SearchShader( shaderRef[i].name );
		if( idxShader < 0 ) 
		{
			shaderRef[i].shader			=	m_texMng->Add( shaderRef[i].name , OWNER_TYPE_MODEL );
			shaderRef[i].surfaceFlags	=	0;
			shaderRef[i].isShader		=	false;
			continue;
		}
		m_shaderMng->InitShader( idxShader );
		
		shader						=	m_shaderMng->GetShader( idxShader );
		
		shaderRef[i].shader			=	idxShader;
		shaderRef[i].surfaceFlags	=	shader->flags;
		shaderRef[i].isShader		=	true;
		
		if( shader->flags & SHADER_TCGEN )
		{
			mdl->m_needDynVB		=	true;
		}
	}
	
	for( i = 0 ; i < mdl->m_numMeshes ; i++ )
	{
		mdl->m_meshes[i].needTCGen		=	false;
		mdl->m_meshes[i].offsetTCGenVB	=	0;
		mdl->m_meshes[i].offsetTCGenEB	=	0;
		if( shaderRef[ mdl->m_meshes[i].skinInfo ].surfaceFlags & SHADER_TCGEN )
			mdl->m_meshes[i].needTCGen	=	true;
	}

	return true;
}

int CMDLMng::PrepareMDLShader()
{
	
	return true;

	
	

	
	for( int i = 0; i < m_numMDLs; i ++ )
	{
		if( m_MDLs[i].IsDisposed() ) continue;



		PrepareMDLShader( i );
	}
	return true;

}

int CMDLMng::Initialize()
{
	m_currTime = 0.0f;
	m_currMilliTime = 0.0f;

	InitItemClass();
	InitEffectClass();

	
	
	EndOfCalcVertexBuffer();





	if(	!GenDynamicMem() )
		return false;

	return true;
}

int CMDLMng::Restore( LPDIRECT3DDEVICE8 pD3DDevice )
{
	m_pD3DDevice = pD3DDevice;

	
	
	



	for( int i = 0 ; i < m_numBank ; i++ )
	{
		if( m_bankSize[i] <= 0 ) continue;

		
		m_staticVB[i]->Create( m_bankSize[ i ] , m_format , GTH_VERTEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY );
	}


	m_dynamicVB->Create( MAX_SIZE_MDL_DYNAMIC_VERTEX_BUFFER , m_format 
		              , GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY);

	m_dynamicIB->Create( MAX_SIZE_MDL_DYNAMIC_INDEX_BUFFER , GTH_FORMAT_INDEX16 
		              , GTH_INDEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );

	if( m_numStaticElems > 0 )
	{

		m_staticEB->Create( m_numStaticElems , GTH_FORMAT_INDEX16 
			, GTH_INDEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY );
	}

	return UploadVertexBuffer();
}

int CMDLMng::GenDynamicMem()
{
	int i;
	
	for( i = 0 ; i < m_numBank ; i++ )
	{
		if( m_bankSize[i] <= 0 ) 
		{
			m_staticVB[i] = NULL;
			continue;
		}

		m_staticVB[i] = new CD3DBuffer();
		if( !m_staticVB[i] )
		{
			GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CMDLMng::GenDynamicMem::m_staticVB" );
			return false;
		}
		m_staticVB[i]->Create( m_bankSize[ i ] , m_format , GTH_VERTEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY );
	}

	m_dynamicVB = new CD3DBuffer();
	if( !m_dynamicVB )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CMDLMng::GenDynamicMem::m_dynamicVB" );
		return false;
	}
	m_dynamicVB->Create( MAX_SIZE_MDL_DYNAMIC_VERTEX_BUFFER , m_format 
		              , GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY);

	m_dynamicIB = new CD3DBuffer();
	if( !m_dynamicIB )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CMDLMng::GenDynamicMem::m_dynamicIB" );
		return false;
	}
	m_dynamicIB->Create( MAX_SIZE_MDL_DYNAMIC_INDEX_BUFFER , GTH_FORMAT_INDEX16 
		              , GTH_INDEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );

	if( m_numStaticElems > 0 )
	{
		m_staticEB = new CD3DBuffer();
		if( !m_staticEB )
		{
			GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CMDLMng::GenDynamicMem::m_staticEB" );
			return false;
		}
		m_staticEB->Create( m_numStaticElems , GTH_FORMAT_INDEX16 
		              , GTH_INDEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY );
	}
	else
		m_staticEB = NULL;

	return true;
}


void CMDLMng::CalcVertexBuffer( CMDL *mdl )
{
	int			i;	
	mdlmesh_t	*mesh;

	if( mdl->m_numBoneFrames > 1 || mdl->m_needDynVB )	return;
	
	mesh = &mdl->m_meshes[0];

	for( i = 0; i < mdl->m_numMeshes; i++ , mesh++ )
	{
		m_currNumStaticVerts		+=	mesh->vertexSize;
		m_numStaticElems			+=	mesh->elemSize;
	}

	if( m_currNumStaticVerts >= MAX_BANKSIZE_VERTEXBUFFER )
	{
		m_bankSize[ m_numBank ]		=	m_prevNumStaticVerts;
		m_currNumStaticVerts		-=	m_prevNumStaticVerts;
		
		m_numBank++;
	}

	mdl->m_bank				=	m_numBank;
	m_prevNumStaticVerts	=	m_currNumStaticVerts;
}

void CMDLMng::EndOfCalcVertexBuffer()
{
	if( m_currNumStaticVerts > 0 )
		m_bankSize[ m_numBank++ ]	=	m_currNumStaticVerts;
}




void CMDLMng::RestoreVertexInfo( int mdlIdx )
{
	CMDL *mdl	=	&m_MDLs[mdlIdx];

	if( mdl->m_numBoneFrames > 1 ) return;
	if( mdl->m_needDynVB ) return;
	
	if( mdl->m_storedMeshes == NULL ) return;

	memcpy( mdl->m_meshes, mdl->m_storedMeshes, sizeof( mdlmesh_t ) * mdl->m_iStoredMeshesSize );
}

int CMDLMng::UploadVertexBuffer()
{
	int i , j , k;
	mdlvertex_t *vert;
	element_t   *elem;
	int currBank;
	int currNumStaticVerts , currNumStaticElems;
	int elemSize;
	WORD *ptrEB , *ptrStaticEB;
	mdlvertex_t *ptrStaticVB;

	CMDL *mdl;
	mdlmesh_t *mesh;

	if( m_bankSize[0] <= 0 )   return true;

	ptrStaticVB  = (mdlvertex_t *) m_staticVB[0]->Lock();
	ptrStaticEB  = (WORD *) m_staticEB->Lock();
	
	currBank = 0;
	currNumStaticVerts = 0;
	currNumStaticElems = 0;
	for( i = 0 ; i < m_numMDLs ; i++ )
	{
		

		mdl	=	GetMDL( i );
		if( mdl == NULL ) continue;

		if( mdl->m_numBoneFrames > 1 ) continue;
		if( mdl->m_needDynVB ) continue;

		
		mdl->m_storedMeshes		=	new	mdlmesh_t[mdl->m_iStoredMeshesSize];

		mdl->m_baseVB = currNumStaticVerts;
		mdl->m_baseEB = currNumStaticElems;
		mdl->m_sizeVB = 0;
		mdl->m_sizeEB = 0;
		if( currBank != mdl->m_bank )
		{
			m_staticVB[ currBank ]->Unlock();
			ptrStaticVB  = (mdlvertex_t *) m_staticVB[ mdl->m_bank ]->Lock();
			currBank = mdl->m_bank;
			currNumStaticVerts = 0;
		}

		mesh = &mdl->m_meshes[0];
		for( j = 0 ; j < mdl->m_numMeshes ; j++ , mesh++ )
		{
			mesh->offsetVB = currNumStaticVerts;
			mesh->offsetEB = currNumStaticElems;

			vert = &mdl->m_vertices[ mesh->vertexOffset ];
			memcpy( ptrStaticVB , vert , sizeof( mdlvertex_t ) * mesh->numVertices );

			elem = &mdl->m_elems[ mesh->elemOffset ];
			elemSize = mesh->numElems;
			ptrEB = ptrStaticEB;
			for( k = 0 ; k < elemSize ; k++ , elem++ , ptrEB++ )
				*ptrEB = (WORD) ( currNumStaticVerts + *elem );

			mesh->sizeVB = mesh->numVertices;
			mesh->sizeEB = mesh->numElems;

			currNumStaticVerts += mesh->sizeVB;
			currNumStaticElems += mesh->sizeEB;
			
			ptrStaticVB += mesh->sizeVB;
			ptrStaticEB += mesh->sizeEB;

			mdl->m_sizeVB += mesh->sizeVB;
			mdl->m_sizeEB += mesh->sizeEB;
		}
		
		
		memcpy( mdl->m_storedMeshes, mdl->m_meshes, sizeof( mdlmesh_t ) * mdl->m_iStoredMeshesSize );
	}

	m_staticVB[ currBank ]->Unlock();
	m_staticEB->Unlock();

	return true;
}

int CMDLMng::LoadMDL( char *MDLName )
{
	int i;
	CMDL *mdl;
	int ret;

	for( i = 0 ; i < m_numMDLs ; i++ )
	{
		if( !stricmp( MDLName , m_MDLs[i].m_name ) )
			break;
	}

	if( i < m_numMDLs )
		return i;

	if( m_numMDLs >= MAX_SIZE_MDL )
	{
		assert( m_numMDLs < MAX_SIZE_MDL );
		GTH_ERROR( GTH_ERROR_PRINT , "MDL overflow.." );
		return -1;
	}

	mdl = &m_MDLs[ m_numMDLs ];

	mdl->SetFileMng( m_fileMng );

	
	g_fileMng.SetResourceManager( &m_resManager );
	mdl->SetMDLManager( this );

	ret = mdl->Load( MDLName );
	
	
	PrepareMDLShader( m_numMDLs );
	CalcVertexBuffer( mdl );

	if( !ret ) 
	{

		return -1;
	}

	m_numMDLs++;

	return m_numMDLs - 1;
}

 

































 
 
 
 
 
void	CMDLMng::BindSoundSample	()
{
	int		index,	subindex;

	for	(	index	=	0;	\
			index	<	g_soundMng ->m_nummdlSoundID;	\
			++index		)
	{
		for	(	subindex	=	0;	\
				subindex	<	m_numMDLs;	\
				++subindex	)
		{
			if	(	! stricmp ( g_soundMng ->m_mdlSoundID [index].mdlname, m_MDLs [subindex].m_name )	)
			{
				 

				m_MDLs [subindex].m_soundIdx	=	g_soundMng ->m_mdlSoundID [index].idx;
				break;
			}
		}
	}
}
	