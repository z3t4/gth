



#include "../global_def.h"

int CMDLMng::LoadWorldClass( char *fname )
{
	char *token;
	mdlworldclass_t *mdlWorldClass;
	mdlworldclasspart_t *part;
	
	if( !m_script->Load( fname ) )
		return false;

	m_script->BeginParse();

	while( 1 )
	{
		token = m_script->GetNextToken( true );
		if( !token[0] ) break;

		if( token[0] != '{' )  continue;

		if( m_numMDLWorldClasses >= MAX_SIZE_MDL_WORLD_CLASS )
		{
			GTH_ERROR( GTH_ERROR_PRINT , "MDL World Class overflow.." );
			break;
		}

		mdlWorldClass = &m_MDLWorldClasses[ m_numMDLWorldClasses++ ];
	    mdlWorldClass->numParts = 0;
		memset( mdlWorldClass , 0 , sizeof( mdlworldclass_t ) );

		while( 1 )
		{
			token = m_script->GetNextToken( true );
			if( !token[0] ) break;

			if (token[0] == '{') 
			{
				part = &mdlWorldClass->parts[ mdlWorldClass->numParts++ ];
		        ParseWorldClassPart( part );
			}
	        else if (token[0] == '}')
			    break;
			else
			    ParseWorldClassToken( mdlWorldClass , token );
		}
    }

	return true;
}

void CMDLMng::ParseWorldClassPart( mdlworldclasspart_t *part )
{
	char *token , *param;

	part->mdlIdx = -1;
	part->msecPerFrame = 100.0f;
	part->scale = 1.0f;
	part->rotSpeed = 0.0f;

	while( 1 )
	{
		token = m_script->GetNextToken( true );
		if( !token[0] ) break;

	    if( token[0] == '}' )
		{
			break;
		}
		else if( !stricmp( token , "model" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			strcpy( part->mdlName , param );
		}
		else if( !stricmp( token , "scale" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->scale = (float) atof( param );
		}
		else if( !stricmp( token , "rotate_speed" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->rotSpeed = (float) atof( param );
		}
		else if( !stricmp( token , "tag" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			strcpy( part->tagName , param );
		}
		else if( !stricmp( token , "frame_per_second" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->msecPerFrame = (float) atof( param );
			if( part->msecPerFrame > 1.0f )
				part->msecPerFrame = 1000.0f / part->msecPerFrame;
		}

		m_script->SkipLine();
	}
}

void CMDLMng::ParseWorldClassToken( mdlworldclass_t *mdlWorldClass , char *token )
{
	char *param;

	if( !stricmp( token , "classname" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlWorldClass->classType = GetWorldClassType( param );
	}
	else if( !stricmp( token , "height" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlWorldClass->bobHeight = (float) atof( param );
	}
	else if( !stricmp( token , "speed" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlWorldClass->bobSpeed = (float) atof( param );
	}
	else if( !stricmp( token , "itemname" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		strcpy( mdlWorldClass->itemName , param );
	}
	 
	else if( !stricmp( token , "hideFlag" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlWorldClass->hideFlag = atoi( param );
	}
		

	m_script->SkipLine();
}

int CMDLMng::GetWorldClassType( char *token )
{
	if     ( !stricmp( token , "i3d_item_model" ) )
		return GTH_WORLD_ENTITY_TYPE_MODEL;
	else if( !stricmp( token , "i3d_info_player_start" ) )
		return GTH_WORLD_ENTITY_TYPE_PLAYER_POS;
	else if( !stricmp( token , "i3d_info_teleport" ) )
		return GTH_WORLD_ENTITY_TYPE_TELEPORT;
	else if( !stricmp( token , "i3d_info_activator" ) )
		return GTH_WORLD_ENTITY_TYPE_ACTIVATOR;
	else if( !stricmp( token , "i3d_info_deactivator" ) )
		return GTH_WORLD_ENTITY_TYPE_DEACTIVATOR;
	else if( !stricmp( token , "i3d_info_door" ) )
		return GTH_WORLD_ENTITY_TYPE_DOOR;
	else if( !stricmp( token , "i3d_info_path" ) )
		return GTH_WORLD_ENTITY_TYPE_PATH;
	else if( !stricmp( token , "i3d_info_camera_path" ) )
		return GTH_WORLD_ENTITY_TYPE_CAMERA_PATH;
	else if( !stricmp( token , "i3d_info_sound" ) )
		return GTH_WORLD_ENTITY_TYPE_SOUND;

	return GTH_WORLD_ENTITY_TYPE_NONE;
}

int CMDLMng::LinkMDLWorldClass( int idxMDLClass )
{
	int i;
	mdlworldclass_t *mdlWorldClass;
	mdlworldclasspart_t *part;
	int flag;

	mdlWorldClass = &m_MDLWorldClasses[idxMDLClass];
	if( mdlWorldClass->loaded )  return true;

	flag = true;
	part = &mdlWorldClass->parts[0];
	for( i = 0 ; i < mdlWorldClass->numParts ; i++ , part++ )
	{
		if( part->tagName[0] )
		{
			if( !stricmp( part->tagName , "tag_hand" ) )
				part->onlyTag = true;
		}
		part->mdlIdx = LoadMDL( part->mdlName );
		if( part->mdlIdx < 0 )
		{
			flag = false;
			break;
		}
	}
	if( flag )
		CalcWorldBoundBox( mdlWorldClass );
	mdlWorldClass->loaded = flag;

	return flag;
}

void CMDLMng::CalcWorldBoundBox( mdlworldclass_t *mdlWorldClass )
{
	mdlworldclasspart_t *part;
	vec3_t classMins , classMaxs;
	vec3_t partMins , partMaxs;
	CMDL *mdl;
	int  i;
	
	ClearBounds( classMins , classMaxs );
	part = &mdlWorldClass->parts[0];
	for( i = 0 ; i < mdlWorldClass->numParts ; i++ , part++ )
	{
		
		mdl	= GetMDL( part->mdlIdx );
		if( mdl == NULL ) continue;



		VectorCopy( partMins , mdl->m_mins );
		VectorCopy( partMaxs , mdl->m_maxs );

		if( part->rotSpeed != 0.0f )
		{
			CalcSphereBoundBox( partMins , partMaxs );
		}

		VectorScale( partMins , partMins , part->scale );
		VectorScale( partMaxs , partMaxs , part->scale );

		AddPointToBounds( partMins , classMins , classMaxs );
		AddPointToBounds( partMaxs , classMins , classMaxs );
	}

	if( mdlWorldClass->bobSpeed != 0.0f )
	{
		classMins[2] -= mdlWorldClass->bobHeight;
		classMaxs[2] += mdlWorldClass->bobHeight;
	}

	VectorCopy( mdlWorldClass->mins , classMins );
	VectorCopy( mdlWorldClass->maxs , classMaxs );
}

void CMDLMng::SetMDLWorldEntityOrigin( int idxMDLEntity , vec3_t origin )
{
	VectorCopy( m_MDLWorldEntities[ idxMDLEntity ].origin , origin );
}

void CMDLMng::SetMDLWorldEntityAngles( int idxMDLEntity , vec3_t angles )
{
	m_MDLWorldEntities[ idxMDLEntity ].rotate = angles[YAW];
}

mdlworldentity_t *CMDLMng::GetMDLWorldEntity( int idxMDLEntity )
{
	return &m_MDLWorldEntities[ idxMDLEntity ];
}

mdlworldentitylink_t *CMDLMng::GetLinkedMDLWorldEntity( int idxClass )
{
	return &m_linkedMDLWorldEntities[ idxClass ];
}

void CMDLMng::ClearWorldUseFlag()
{
	ClearRenderWorldLink();
	ClearBlendWorldLink();
	memset( m_linkedMDLWorldEntities , 0 , sizeof( mdlworldentitylink_t ) * m_numMDLWorldClasses );
	memset( m_worldInUse , 0 , sizeof( int ) * MAX_SIZE_MDL_WORLD_ENTITY );
	m_currMDLWorldEntity = 0;
}

int CMDLMng::AddWorldEntity( char *itemName , vec3_t origin , vec3_t angles )
{
	mdlworldclass_t       *mdlWorldClass;
	mdlworldentity_t      *mdlEntity;
	mdlworldentitylink_t  *linkedMDLEntity;
	int i;
	int idxClass , found;

	mdlWorldClass = &m_MDLWorldClasses[0];
	for( i = 0 ; i < m_numMDLWorldClasses ; i++ , mdlWorldClass++ )
	{
		if( mdlWorldClass->classType != GTH_WORLD_ENTITY_TYPE_MODEL 
			&& mdlWorldClass->classType != GTH_WORLD_ENTITY_TYPE_TELEPORT )  continue;

		if( !stricmp( mdlWorldClass->itemName , itemName ) )
		{
			idxClass = i;
			break;
		}
	}

	if( i == m_numMDLWorldClasses )  return -1;
	if( !LinkMDLWorldClass( i ) )  return -1;

	found = -1;
	for( i = m_currMDLWorldEntity ; i < MAX_SIZE_MDL_WORLD_ENTITY ; i++ )
	{
		if( !m_worldInUse[i] )
		{
			found = i;
			goto NEXT_FREE_MDL_WORLD_ENTITY;
		}
	}

	for( i = 0 ; i < m_currMDLWorldEntity ; i++ )
	{
		if( !m_worldInUse[i] )
		{
			found = i;
			goto NEXT_FREE_MDL_WORLD_ENTITY;
		}
	}

	if( found == -1 )
	{
		found = m_currMDLWorldEntity + 1;
		if( found >= MAX_SIZE_MDL_WORLD_ENTITY )
			found = 0;
		DeleteWorldEntity( found );
	}

NEXT_FREE_MDL_WORLD_ENTITY :
	m_currMDLWorldEntity = found;

	mdlEntity = &m_MDLWorldEntities[ m_currMDLWorldEntity ];
	linkedMDLEntity = &m_linkedMDLWorldEntities[ idxClass ];
	mdlEntity->idxClass = idxClass;

	mdlEntity->next = linkedMDLEntity->linked;
	mdlEntity->prev = NULL;
	if( linkedMDLEntity->linked )
		linkedMDLEntity->linked->prev = mdlEntity;
	linkedMDLEntity->linked = mdlEntity;

	VectorCopy( mdlEntity->origin , origin );
	mdlEntity->rotate = angles[ YAW ];

	
	

	mdlEntity->visible = false;
	m_worldInUse[ m_currMDLWorldEntity ] = true;

		
	MakeTransformedMDL( mdlEntity, idxClass );		
		
	
	return m_currMDLWorldEntity;
}

	
void  CMDLMng::MakeTransformedMDL( mdlworldentity_t *entity, int idxClass )
{
	mdlworldclass_t *worldClass;
	mdlworldclasspart_t *part;
	CMDL *mdl;
	int i, j;
	float cosval, sinval;
	float radval;
	mdlboundvertex_t  *source;

	worldClass = &m_MDLWorldClasses[idxClass];

	part = &worldClass->parts[0];
	for( i = 0 ; i < worldClass->numParts ; i++ , part++ )
	{
		
		mdl	= GetMDL( part->mdlIdx );
		if( mdl == NULL ) continue;


		entity->num_cvertex[i] = mdl->m_numBoundVertices;
		 
		if ( mdl->m_numBoundVertices > 0 )
			entity->collisionVertex[i] = new vec3_t [mdl->m_numBoundVertices];
		else 
			entity->collisionVertex[i] = NULL;

			
		radval = deg2rad( entity->rotate );
		cosval = cos( radval );
		 
		sinval = sin( radval );
		source = &mdl->m_boundVertices[0];
		for ( j = 0; j < mdl->m_numBoundVertices; j++, source++ )
		{
			entity->collisionVertex[i][j][0] = cosval*source->point[0] - sinval*source->point[1] + entity->origin[0];
			entity->collisionVertex[i][j][1] = sinval*source->point[0] + cosval*source->point[1] + entity->origin[1];
			entity->collisionVertex[i][j][2] = source->point[2] + entity->origin[2];
		}
	}
}
	

int CMDLMng::DeleteWorldEntity( int idxMDLEntity )
{
	mdlworldentity_t      *mdlEntity;
	
	mdlEntity = &m_MDLWorldEntities[ idxMDLEntity ];
	if( !m_worldInUse[ idxMDLEntity ] ) return true;

	if( mdlEntity->prev )
	{
		mdlEntity->prev->next = mdlEntity->next;
	}
	else  
	{
		m_linkedMDLWorldEntities[ mdlEntity->idxClass ].linked = mdlEntity->next;
	}

	if( mdlEntity->next )
		mdlEntity->next->prev = mdlEntity->prev;

	mdlEntity->next = NULL;
	mdlEntity->prev = NULL;
	m_worldInUse[ idxMDLEntity ] = false;

	return true;
}

int CMDLMng::GetMDLWorldClassBoundBox( int idxMDLEntity , vec3_t mins , vec3_t maxs )
{
	mdlworldentity_t      *mdlEntity;
	
	mdlEntity = &m_MDLWorldEntities[ idxMDLEntity ];
	if( !m_worldInUse[ idxMDLEntity ] ) return false;

	VectorCopy( mins , m_MDLWorldClasses[ mdlEntity->idxClass ].mins );
	VectorCopy( maxs , m_MDLWorldClasses[ mdlEntity->idxClass ].maxs );

	if( mdlEntity->rotate != 0.0f )
		CalcZAxisRotateBoundBox( mdlEntity->rotate , mins , maxs );

	return true;
}

void CMDLMng::RenderWorldEntity()
{
	int i , j , k;
	mdlworldentity_t     **linked;
	mdlworldclass_t       *mdlWorldClass;
	mdlworldclasspart_t   *mdlWorldClassPart;
	mdlworldentity_t      *mdlWorldEntity;
	CMDL             *mdl;
	mdlmesh_t        *mesh;
	float  bob;
	vec3_t scale , position;
	matrix4x4_t tmp;
	shader_t *shader;
	
	linked = &m_linkedRenderMDLWorldEntities[0];
	mdlWorldClass = &m_MDLWorldClasses[0];
	for( i = 0 ; i < m_numMDLWorldClasses ; i++ , linked++ , mdlWorldClass++ )
	{
		if( !*linked ) continue;

		m_waveFunc.amplitude = mdlWorldClass->bobHeight;
		m_waveFunc.frequency = mdlWorldClass->bobSpeed;
		bob = m_shaderMng->EvaluateFunc( &m_waveFunc ,  m_currTime );
		
		mdlWorldClassPart = &mdlWorldClass->parts[0];
		for( j = 0 ; j < mdlWorldClass->numParts ; j++ , mdlWorldClassPart++ )
		{
			if( mdlWorldClassPart->onlyTag ) continue;

			
			mdl	= GetMDL( mdlWorldClassPart->mdlIdx );
			if( mdl == NULL ) continue;



			VectorSet( scale , mdlWorldClassPart->scale , mdlWorldClassPart->scale , mdlWorldClassPart->scale );
		
			if( mdl->m_numBoneFrames > 1 )
			{
				m_isInterpolated = true;
				m_interPol = m_currMilliTime / mdlWorldClassPart->msecPerFrame;
				m_currFrame = ( (int) m_interPol ) % ( mdl->m_numBoneFrames - 1 );
				m_interPol = m_interPol - ( (int) m_interPol );
			}
			else
			{
				m_isInterpolated = false;
				m_interPol = 0.0f;
				m_currFrame = 0;
			}

			if( m_isInterpolated )
			{
				for( k = 0 ; k < mdl->m_numMeshes ; k++ )
					PrepareInterpolateMDL( mdl , &mdl->m_meshes[k] );

				m_D3DRender->SetStreamSource( 0 , m_dynamicVB->GetD3DVertexBuffer() , m_dynamicVB->GetStrideSize() );
				m_D3DRender->SetIndices( m_dynamicIB->GetD3DIndexBuffer() , 0 );
				m_currBank = -1;
			}	          
			else
			{
				if( mdl->m_needDynVB )
				{
					for( k = 0 ; k < mdl->m_numMeshes ; k++ )
						PrepareMDL( mdl , &mdl->m_meshes[k] );

					m_D3DRender->SetStreamSource( 0 , m_dynamicVB->GetD3DVertexBuffer() , m_dynamicVB->GetStrideSize() );
					m_D3DRender->SetIndices( m_dynamicIB->GetD3DIndexBuffer() , 0 );
					m_currBank = -1;
				}
				else
				{
					if( m_currBank != mdl->m_bank )
					{
						m_D3DRender->SetStreamSource( 0 , m_staticVB[ mdl->m_bank ]->GetD3DVertexBuffer() 
							, m_staticVB[ mdl->m_bank ]->GetStrideSize() );
						m_currBank = mdl->m_bank;
					}
					m_D3DRender->SetIndices( m_staticEB->GetD3DIndexBuffer() , 0 );
				}
			}

			mesh = &mdl->m_meshes[0];
			for( k = 0 ; k < mdl->m_numMeshes ; k++ , mesh++ )
			{
				if( mdl->m_shaderRefs[ mesh->skinInfo ].isShader )
					m_shaderMng->PrepareShader( mdl->m_shaderRefs[ mesh->skinInfo ].shader , m_frameFlag );
			}

			mdlWorldEntity = *linked;
			while( mdlWorldEntity )
			{
				if( !mdlWorldEntity->visible )
				{
					mdlWorldEntity = mdlWorldEntity->renderNext;
					continue;
				}
				
				m_transform.Identity();
				
				VectorSet( position , mdlWorldEntity->origin[0] , mdlWorldEntity->origin[1] , mdlWorldEntity->origin[2] + bob );
				m_transform.Translate( position );
			
				if( mdlWorldClassPart->rotSpeed == 0.0f )
					m_transform.RotateZ( mdlWorldEntity->rotate );
				else
					m_transform.RotateZ( mdlWorldClassPart->rotSpeed * m_currTime * 180.0f );

				
				
				tmp.Scale( scale );
				m_transform *= tmp;
				

				m_D3DRender->SetTransform( D3DTS_WORLD , &m_transform );

				mesh = &mdl->m_meshes[0];
				for( k = 0 ; k < mdl->m_numMeshes ; k++ , mesh++ )
				{
					if( mdl->m_shaderRefs[ mesh->skinInfo ].isShader )
					{
						shader = &m_shaderMng->m_shaders[ mdl->m_shaderRefs[ mesh->skinInfo ].shader ];
						if( shader->flags & SHADER_TCGEN )
							m_shaderMng->PrepareTCGen( shader , position );

						m_D3DRender->RenderMDLCustom( shader , mesh->offsetVB , mesh->numVertices , mesh->offsetEB , mesh->numElems );
					}
					else
					{
						m_D3DRender->SetTexture( 0 , m_texMng->GetD3DTexture( mdl->m_shaderRefs[ mesh->skinInfo ].shader ) );
						m_D3DRender->RenderDefault( mesh->offsetVB , mesh->numVertices , mesh->offsetEB , mesh->numElems );
					}

					m_totalRenderPolygon += mdl->m_meshes[k].numElems / 3;
				}

				

				
				

				mdlWorldEntity = mdlWorldEntity->renderNext;				
			}	
		}
	}	
}

void CMDLMng::RenderBlendWorldEntity()
{
	int i , j , k;
	mdlworldentity_t     **linked;
	mdlworldclass_t       *mdlWorldClass;
	mdlworldclasspart_t   *mdlWorldClassPart;
	mdlworldentity_t      *mdlWorldEntity;
	CMDL             *mdl;
	mdlmesh_t        *mesh;
	float  bob;
	vec3_t scale , position;
	matrix4x4_t tmp;
	shader_t *shader;

	 
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	
	linked = &m_linkedBlendMDLWorldEntities[0];
	mdlWorldClass = &m_MDLWorldClasses[0];
	for( i = 0 ; i < m_numMDLWorldClasses ; i++ , linked++ , mdlWorldClass++ )
	{
		if( !*linked ) continue;

		m_waveFunc.amplitude = mdlWorldClass->bobHeight;
		m_waveFunc.frequency = mdlWorldClass->bobSpeed;
		bob = m_shaderMng->EvaluateFunc( &m_waveFunc ,  m_currTime );
		
		mdlWorldClassPart = &mdlWorldClass->parts[0];
		for( j = 0 ; j < mdlWorldClass->numParts ; j++ , mdlWorldClassPart++ )
		{
			if( mdlWorldClassPart->onlyTag ) continue;

			
			mdl	= GetMDL( mdlWorldClassPart->mdlIdx );
			if( mdl == NULL ) continue;



			VectorSet( scale , mdlWorldClassPart->scale , mdlWorldClassPart->scale , mdlWorldClassPart->scale );
		
			if( mdl->m_numBoneFrames > 1 )
			{
				m_isInterpolated = true;
				m_interPol = m_currMilliTime / mdlWorldClassPart->msecPerFrame;
				m_currFrame = ( (int) m_interPol ) % ( mdl->m_numBoneFrames - 1 );
				m_interPol = m_interPol - ( (int) m_interPol );
			}
			else
			{
				m_isInterpolated = false;
				m_interPol = 0.0f;
				m_currFrame = 0;
			}

			if( m_isInterpolated )
			{
				for( k = 0 ; k < mdl->m_numMeshes ; k++ )
					PrepareInterpolateMDL( mdl , &mdl->m_meshes[k] );

				m_D3DRender->SetStreamSource( 0 , m_dynamicVB->GetD3DVertexBuffer() , m_dynamicVB->GetStrideSize() );
				m_D3DRender->SetIndices( m_dynamicIB->GetD3DIndexBuffer() , 0 );
				m_currBank = -1;
			}	          
			else
			{
				if( mdl->m_needDynVB )
				{
					for( k = 0 ; k < mdl->m_numMeshes ; k++ )
						PrepareMDL( mdl , &mdl->m_meshes[k] );

					m_D3DRender->SetStreamSource( 0 , m_dynamicVB->GetD3DVertexBuffer() , m_dynamicVB->GetStrideSize() );
					m_D3DRender->SetIndices( m_dynamicIB->GetD3DIndexBuffer() , 0 );
					m_currBank = -1;
				}
				else
				{
					if( m_currBank != mdl->m_bank )
					{
						m_D3DRender->SetStreamSource( 0 , m_staticVB[ mdl->m_bank ]->GetD3DVertexBuffer() 
							, m_staticVB[ mdl->m_bank ]->GetStrideSize() );
						m_currBank = mdl->m_bank;
					}
					m_D3DRender->SetIndices( m_staticEB->GetD3DIndexBuffer() , 0 );
				}
			}

			mesh = &mdl->m_meshes[0];
			for( k = 0 ; k < mdl->m_numMeshes ; k++ , mesh++ )
			{
				if( mdl->m_shaderRefs[ mesh->skinInfo ].isShader )
					m_shaderMng->PrepareShader( mdl->m_shaderRefs[ mesh->skinInfo ].shader , m_frameFlag );
			}

			mdlWorldEntity = *linked;
			while( mdlWorldEntity )
			{
				if( !mdlWorldEntity->visible )
				{
					mdlWorldEntity = mdlWorldEntity->blendNext;
					continue;
				}
				
				m_transform.Identity();
				
				VectorSet( position , mdlWorldEntity->origin[0] , mdlWorldEntity->origin[1] , mdlWorldEntity->origin[2] + bob );
				m_transform.Translate( position );
			
				if( mdlWorldClassPart->rotSpeed == 0.0f )
					m_transform.RotateZ( mdlWorldEntity->rotate );
				else
					m_transform.RotateZ( mdlWorldClassPart->rotSpeed * m_currTime * 180.0f );

				
				
				tmp.Scale( scale );
				m_transform *= tmp;
				

				m_D3DRender->SetTransform( D3DTS_WORLD , &m_transform );

				mesh = &mdl->m_meshes[0];
				for( k = 0 ; k < mdl->m_numMeshes ; k++ , mesh++ )
				{
					if( mdl->m_shaderRefs[ mesh->skinInfo ].isShader )
					{
						shader = &m_shaderMng->m_shaders[ mdl->m_shaderRefs[ mesh->skinInfo ].shader ];
						if( shader->flags & SHADER_TCGEN )
							m_shaderMng->PrepareTCGen( shader , position );

						m_D3DRender->RenderTransMDLCustom( shader , mesh->offsetVB , mesh->numVertices , mesh->offsetEB , mesh->numElems , 0xa0a0a0a0 );
					}
					else
					{
						m_D3DRender->SetTexture( 0 , m_texMng->GetD3DTexture( mdl->m_shaderRefs[ mesh->skinInfo ].shader ) );
						m_D3DRender->RenderTransDefault( mesh->offsetVB , mesh->numVertices , mesh->offsetEB , mesh->numElems , 0xa0a0a0a0 );
					}

					m_totalRenderPolygon += mdl->m_meshes[k].numElems / 3;
				}

				

				
				

				mdlWorldEntity = mdlWorldEntity->blendNext;
			}	
		}
	}	
}