



#include "../global_def.h"

int CMDLMng::LoadItemClass( char *fname )
{
	char *token;
	mdlitemclass_t *mdlItemClass;
	mdlitemclasspart_t *part;
	
	if( !m_script->Load( fname ) )
		return false;

	m_script->BeginParse();

	while( 1 )
	{
		token = m_script->GetNextToken( true );
		if( !token[0] ) break;

		if( token[0] != '{' )  continue;

		if( m_numMDLItemClasses >= MAX_SIZE_MDL_WORLD_CLASS )
		{
			GTH_ERROR( GTH_ERROR_PRINT , "MDL Item Class overflow.." );
			break;
		}

		mdlItemClass = &m_MDLItemClasses[ m_numMDLItemClasses++ ];
	    mdlItemClass->numParts = 0;
		memset( mdlItemClass , 0 , sizeof( mdlitemclass_t ) );

		while( 1 )
		{
			token = m_script->GetNextToken( true );
			if( !token[0] ) break;

			if (token[0] == '{') 
			{
				part = &mdlItemClass->parts[ mdlItemClass->numParts++ ];
		        ParseItemClassPart( part );
			}
	        else if (token[0] == '}')
			    break;
			else
			    ParseItemClassToken( mdlItemClass , token );
		}
    }

	return true;
}

void CMDLMng::ParseItemClassPart( mdlitemclasspart_t *part )
{
	char *token , *param;

	part->mdlIdx = -1;
	VectorSet( part->color , 1.0f , 1.0f , 1.0f );
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
		else if( !stricmp( token , "color" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->color[0] = 255.0f / (float) atof( param );
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->color[1] = 255.0f / (float) atof( param );
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->color[2] = 255.0f / (float) atof( param );
		}

		m_script->SkipLine();
	}
}

void CMDLMng::ParseItemClassToken( mdlitemclass_t *mdlItemClass , char *token )
{
	char *param;

	if( !stricmp( token , "classtype" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlItemClass->classType = GetItemClassType( param );
	}
	else if( !stricmp( token , "height" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlItemClass->bobHeight = (float) atof( param );
	}
	else if( !stricmp( token , "speed" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlItemClass->bobSpeed = (float) atof( param );
	}
	else if( !stricmp( token , "itemname" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		strcpy( mdlItemClass->itemName , param );
	}
	else if( !stricmp( token , "itemid" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlItemClass->itemID = (int) atoi( param );
	}

	m_script->SkipLine();
}


int CMDLMng::GetItemClassType( char *token )
{
	if( !stricmp( token , "gth_item_weapon" ) )
		return GTH_ITEM_CLASS_WEAPON;
	else if( !stricmp( token , "gth_item_upper" ) )
		return GTH_ITEM_CLASS_UPPER;
	else if( !stricmp( token , "gth_item_lower" ) )
		return GTH_ITEM_CLASS_LOWER;
	else if( !stricmp( token , "gth_item_protector" ) )
		return GTH_ITEM_CLASS_PROTECTOR;
	else if( !stricmp( token , "gth_item_hand" ) )
		return GTH_ITEM_CLASS_HAND;
	else if( !stricmp( token , "gth_item_foot" ) )
		return GTH_ITEM_CLASS_FOOT;
	else if( !stricmp( token , "gth_item_air_protector" ) )
		return GTH_ITEM_CLASS_AIR_PROTECTOR;
	else if( !stricmp( token , "gth_item_accessory" ) )
		return GTH_ITEM_CLASS_ACCESSORY;
	else if( !stricmp( token , "gth_item_reserved" ) )
		return GTH_ITEM_CLASS_RESERVED;

	return GTH_ITEM_CLASS_NONE;
}

int CMDLMng::LinkMDLItemClass( int idxMDLClass )
{
	int i;
	mdlitemclass_t *mdlItemClass;
	mdlitemclasspart_t *part;
	int flag;

	mdlItemClass = &m_MDLItemClasses[idxMDLClass];
	if( mdlItemClass->loaded )  return true;

	flag = true;
	part = &mdlItemClass->parts[0];
	for( i = 0 ; i < mdlItemClass->numParts ; i++ , part++ )
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
		CalcItemBoundBox( mdlItemClass );
	mdlItemClass->loaded = flag;

	return flag;
}

void CMDLMng::CalcItemBoundBox( mdlitemclass_t *mdlItemClass )
{
	mdlitemclasspart_t *part;
	vec3_t classMins , classMaxs;
	vec3_t partMins , partMaxs;
	CMDL *mdl;
	int  i;
	
	ClearBounds( classMins , classMaxs );
	part = &mdlItemClass->parts[0];
	for( i = 0 ; i < mdlItemClass->numParts ; i++ , part++ )
	{
		
		mdl	=	GetMDL( part->mdlIdx );
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

	if( mdlItemClass->bobSpeed != 0.0f )
	{
		classMins[2] -= mdlItemClass->bobHeight;
		classMaxs[2] += mdlItemClass->bobHeight;
	}

	VectorCopy( mdlItemClass->mins , classMins );
	VectorCopy( mdlItemClass->maxs , classMaxs );
}

int CMDLMng::InitItemClass()
{
	for( int i = 0 ; i < m_numMDLItemClasses ; i++ )
	{	
		LinkMDLItemClass( i );
		
		
	}

	memset( m_linkedMDLItemEntities , 0 , sizeof( mdlitementitylink_t ) * MAX_SIZE_MDL_ITEM_CLASS );
	memset( m_MDLItemEntities , 0 , sizeof( mdlitementity_t ) * MAX_SIZE_MDL_ITEM_ENTITY );
	memset( m_itemInUse , 0 , sizeof( int ) * MAX_SIZE_MDL_ITEM_ENTITY );

	m_currMDLItemEntity = 0;

	return true;
}

void CMDLMng::ClearItemUseFlag()
{
	memset( m_linkedMDLItemEntities , 0 , sizeof( mdlitementitylink_t ) * m_numMDLItemClasses );
	memset( m_itemInUse , 0 , sizeof( int ) * MAX_SIZE_MDL_ITEM_ENTITY );
	m_currMDLItemEntity = 0;
}

int CMDLMng::AddItemEntity( GTH_ITEM_CLASS_TYPE classType , int itemID , vec3_t origin , vec3_t angles
						  , GTH_SYNC_TYPE syncType , GTH_ANIM_PLAY_TYPE animType , float animTime )
{
	mdlitemclass_t       *mdlItemClass;
	mdlitementity_t      *mdlEntity;
	mdlitementitylink_t  *linkedMDLEntity;
	int i;
	int idxClass , found;

	if( classType == GTH_ITEM_CLASS_NONE )  return -1;

	mdlItemClass = &m_MDLItemClasses[0];
	for( i = 0 ; i < m_numMDLItemClasses ; i++ , mdlItemClass++ )
	{
		if( mdlItemClass->classType == classType && mdlItemClass->itemID == itemID )
		{
			idxClass = i;
			break;
		}
	}

	if( i == m_numMDLItemClasses )  return -1;
	if( !mdlItemClass->loaded )     return -1;

	found = -1;
	for( i = m_currMDLItemEntity ; i < MAX_SIZE_MDL_ITEM_ENTITY ; i++ )
	{
		if( !m_itemInUse[i] )
		{
			found = i;
			goto NEXT_FREE_MDL_ITEM_ENTITY;
		}
	}

	for( i = 0 ; i < m_currMDLItemEntity ; i++ )
	{
		if( !m_itemInUse[i] )
		{
			found = i;
			goto NEXT_FREE_MDL_ITEM_ENTITY;
		}
	}

	if( found == -1 )
	{
		found = m_currMDLItemEntity + 1;
		if( found >= MAX_SIZE_MDL_ITEM_ENTITY )
			found = 0;
		DeleteItemEntity( found );
	}

NEXT_FREE_MDL_ITEM_ENTITY :
	m_currMDLItemEntity = found;

	mdlEntity = &m_MDLItemEntities[ m_currMDLItemEntity ];
	linkedMDLEntity = &m_linkedMDLItemEntities[ idxClass ];
	mdlEntity->idxClass = idxClass;

	mdlEntity->next = linkedMDLEntity->linked;
	mdlEntity->prev = NULL;
	if( linkedMDLEntity->linked )
		linkedMDLEntity->linked->prev = mdlEntity;
	linkedMDLEntity->linked = mdlEntity;

	VectorCopy( mdlEntity->origin , origin );
	mdlEntity->rotate = angles[ YAW ];

	
	
	
	mdlEntity->syncType = syncType;
	mdlEntity->animType = animType;
	mdlEntity->animTime = animTime;
	mdlEntity->visible = false;

	m_itemInUse[ m_currMDLItemEntity ] = true;

	return m_currMDLItemEntity;
}

int CMDLMng::DeleteItemEntity( int idxMDLEntity )
{
	mdlitementity_t      *mdlEntity;
	
	mdlEntity = &m_MDLItemEntities[ idxMDLEntity ];
	if( !m_itemInUse[ idxMDLEntity ] ) return true;

	if( mdlEntity->prev )
	{
		mdlEntity->prev->next = mdlEntity->next;
	}
	else  
	{
		m_linkedMDLItemEntities[ mdlEntity->idxClass ].linked = mdlEntity->next;
	}

	if( mdlEntity->next )
		mdlEntity->next->prev = mdlEntity->prev;

	mdlEntity->next = NULL;
	mdlEntity->prev = NULL;
	m_itemInUse[ idxMDLEntity ] = false;

	return true;
}

void CMDLMng::SetMDLItemEntityOrigin( int idxMDLEntity , vec3_t origin )
{
	VectorCopy( m_MDLItemEntities[ idxMDLEntity ].origin , origin );
}

void CMDLMng::SetMDLItemEntityAngles( int idxMDLEntity , vec3_t angles )
{
	m_MDLItemEntities[ idxMDLEntity ].rotate = angles[YAW];
}

mdlitementity_t *CMDLMng::GetMDLItemEntity( int idxMDLEntity )
{
	return &m_MDLItemEntities[ idxMDLEntity ];
}

mdlitementitylink_t *CMDLMng::GetLinkedMDLItemEntity( int idxClass )
{
	return &m_linkedMDLItemEntities[ idxClass ];
}

int CMDLMng::GetMDLItemClassBoundBox( int idxMDLEntity , vec3_t mins , vec3_t maxs )
{
	mdlitementity_t      *mdlEntity;
	
	mdlEntity = &m_MDLItemEntities[ idxMDLEntity ];
	if( !m_itemInUse[ idxMDLEntity ] ) return false;

	VectorCopy( mins , m_MDLItemClasses[ mdlEntity->idxClass ].mins );
	VectorCopy( maxs , m_MDLItemClasses[ mdlEntity->idxClass ].maxs );

	return true;
}

void CMDLMng::RenderItemEntity()
{
	int i , j , k;
	mdlitementitylink_t  *linked;
	mdlitemclass_t       *mdlItemClass;
	mdlitemclasspart_t   *mdlItemClassPart;
	mdlitementity_t      *mdlItemEntity;
	CMDL             *mdl;
	mdlmesh_t        *mesh;
	float  bob;
	vec3_t scale , position;
	matrix4x4_t tmp;
	shader_t *shader;
	int *linkedCount;
	
	linked = &m_linkedMDLItemEntities[0];
	mdlItemClass = &m_MDLItemClasses[0];
	linkedCount = &m_linkedVisibleCount.itemClasses[0];
	for( i = 0 ; i < m_numMDLItemClasses ; i++ , linked++ , linkedCount++ , mdlItemClass++ )
	{
		if( !linked->linked ) continue;
		if( *linkedCount < 1 ) continue;

		m_waveFunc.amplitude = mdlItemClass->bobHeight;
		m_waveFunc.frequency = mdlItemClass->bobSpeed;
		bob = m_shaderMng->EvaluateFunc( &m_waveFunc ,  m_currTime );
		
		mdlItemClassPart = &mdlItemClass->parts[0];
		for( j = 0 ; j < mdlItemClass->numParts ; j++ , mdlItemClassPart++ )
		{
			if( mdlItemClassPart->onlyTag ) continue;

			
			mdl	=	GetMDL( mdlItemClassPart->mdlIdx );
			if( mdl == NULL ) continue;



			VectorSet( scale , mdlItemClassPart->scale , mdlItemClassPart->scale , mdlItemClassPart->scale );
		
			if( mdl->m_numBoneFrames > 1 )
			{
				m_isInterpolated = true;
				m_interPol = m_currMilliTime / mdlItemClassPart->msecPerFrame;
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

			mdlItemEntity = linked->linked;
			while( mdlItemEntity )
			{
				if( !mdlItemEntity->visible )
				{
					mdlItemEntity = mdlItemEntity->next;
					continue;
				}

				m_transform.Identity();
				
				VectorSet( position , mdlItemEntity->origin[0] , mdlItemEntity->origin[1] , mdlItemEntity->origin[2] + bob );
				m_transform.Translate( position );
			
				if( mdlItemClassPart->rotSpeed == 0.0f )
					m_transform.RotateZ( mdlItemEntity->rotate );
				else
					m_transform.RotateZ( mdlItemClassPart->rotSpeed * m_currTime * 180.0f );

				
				
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

				

				
				

				mdlItemEntity = mdlItemEntity->next;
			}	
		}
	}

	if( m_focusedItem >= 0 && m_useFocusItem )
	{
		mdlItemEntity = &m_MDLItemEntities[ m_focusedItem ];
		if( !mdlItemEntity->visible )	return;

		mdlItemClass = &m_MDLItemClasses[ mdlItemEntity->idxClass ];

		m_waveFunc.amplitude = mdlItemClass->bobHeight;
		m_waveFunc.frequency = mdlItemClass->bobSpeed;
		bob = m_shaderMng->EvaluateFunc( &m_waveFunc ,  m_currTime );
		
		mdlItemClassPart = &mdlItemClass->parts[0];
		for( j = 0 ; j < mdlItemClass->numParts ; j++ , mdlItemClassPart++ )
		{
			if( mdlItemClassPart->onlyTag ) continue;

			
			mdl	=	GetMDL( mdlItemClassPart->mdlIdx );
			if( mdl == NULL ) continue;



			VectorSet( scale , mdlItemClassPart->scale , mdlItemClassPart->scale , mdlItemClassPart->scale );
			VectorScale( scale , scale , 1.1f );
		
			if( mdl->m_numBoneFrames > 1 )
			{
				m_isInterpolated = true;
				m_interPol = m_currMilliTime / mdlItemClassPart->msecPerFrame;
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
						
			m_transform.Identity();
				
			VectorSet( position , mdlItemEntity->origin[0] , mdlItemEntity->origin[1] , mdlItemEntity->origin[2] + bob );
			m_transform.Translate( position );
			
			if( mdlItemClassPart->rotSpeed == 0.0f )
				m_transform.RotateZ( mdlItemEntity->rotate );
			else
				m_transform.RotateZ( mdlItemClassPart->rotSpeed * m_currTime * 180.0f );

			
			
			tmp.Scale( scale );
			m_transform *= tmp;
			

			
			m_D3DRender->SetAttachShader( m_focusedItemColor );
			m_D3DRender->SetTransform( D3DTS_WORLD , &m_transform );

			mesh = &mdl->m_meshes[0];
			for( k = 0 ; k < mdl->m_numMeshes ; k++ , mesh++ )
			{
				m_D3DRender->RenderDefault( mesh->offsetVB , mesh->numVertices , mesh->offsetEB , mesh->numElems );
				m_totalRenderPolygon += mdl->m_meshes[k].numElems / 3;
			}

			m_D3DRender->EndAttachShader();
		}	
	}
}