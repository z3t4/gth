



#include "../global_def.h"






CEntityMng::CEntityMng()
{
	m_numEntities = 0;
	m_frameFlag   = 0;
	m_visibleDist = 10000.0f;
	m_removeDist  = 10000.0f;
	m_numStrings = 0;
	m_spawnID = 0;
	m_numActivator = 0;
	m_viewSightMinHeight = 80.0f;

	m_visibleQuality = GTH_WORLD_VISIBLE_QUALITY_HIGH;
}

CEntityMng::~CEntityMng()
{
}

int CEntityMng::LoadEntity( char *fname )
{
	char *token;
	worldentity_t  *entity;
	
	if( !m_script->Load( fname ) )
		return false;

	m_script->BeginParse();

	m_numEntities = 0;
	m_frameFlag   = 0;
	m_numStrings = 0;

	while( 1 )
	{

NEXT_BRACE :

		token = m_script->GetNextToken( true );
		if( !token[0] ) break;

		if( token[0] != '{' )  continue;

		if( m_numEntities >= MAX_SIZE_WORLD_ENTITY )
		{
			GTH_ERROR( GTH_ERROR_PRINT , "Entity overflow.." );
			break;
		}

		entity = &m_entities[ m_numEntities++ ];
	    memset( entity , 0 , sizeof( worldentity_t ) );
		entity->effectiveDist = 30.0f;
		entity->speed = 1.0f;

		while( 1 )
		{
			token = m_script->GetNextToken( true );
			if( !token[0] ) break;

			if( token[0] == '}') 
			    break;
			else
			{
			    if( !ParseEntityToken( entity , token ) )
				{
					while( 1 )
					{
						token = m_script->GetNextToken( true );
						if( !token[0] ) break;

						if( token[0] == '}') 
							break;
					}
					m_numEntities--;
					goto NEXT_BRACE;
				}
			}
		}
    }

	return true;
}

int CEntityMng::ParseEntityToken( worldentity_t *entity , char *token )
{
	char parameter[10][32];
	char param[32];

	strcpy( param , token );

	int numParms = 0;
	while( 1 )
	{
		token = m_script->GetNextToken( false );
		if( !token[ 0 ] ) break;
		strcpy( parameter[ numParms ] , token );
		numParms++;
	}

	if     ( !stricmp( param , "classname" ) )
	{
		if( numParms < 1 ) return false;
		entity->classType = GetClassType( parameter[0] );
		if( entity->classType == GTH_WORLD_ENTITY_TYPE_NONE )
			return false;
	}
	else if( !stricmp( param , "itemname" ) )
	{
		if( numParms < 1 ) return false;
		strcpy( entity->itemName , parameter[0] );
	}
	else if( !stricmp( param , "origin" ) )
	{
		if( numParms < 3 ) return false;
		entity->origin[0] = (float) atof( parameter[0] );
		entity->origin[1] = (float) atof( parameter[1] );
		entity->origin[2] = (float) atof( parameter[2] );
	}
	else if( !stricmp( param , "angle" ) )
	{
		if( numParms < 1 ) return false;
		entity->angles[YAW] = (float) atof( parameter[0] );
	}
	else if( !stricmp( param , "angles" ) )
	{
		if( numParms < 3 ) return false;
		entity->angles[PITCH] = (float) atof( parameter[0] );
		entity->angles[YAW] = (float) atof( parameter[1] );
		entity->angles[ROLL] = (float) atof( parameter[2] );
	}
	else if( !stricmp( param , "spawnflags" ) )
	{
		if( numParms < 1 ) return false;
		entity->spawnflags = atoi( parameter[0] );
	}
	else if( !stricmp( param , "effectivedist" ) )
	{
		if( numParms < 1 ) return false;
		entity->effectiveDist = (float) atof( parameter[0] );
	}
	else if( !stricmp( param , "target" ) )
	{
		if( numParms < 1 ) return false;
		entity->target = GetNewString( parameter[0] );
	}
	else if( !stricmp( param , "targetname" ) )
	{
		if( numParms < 1 ) return false;
		entity->targetName = GetNewString( parameter[0] );
	}
	else if( !stricmp( param , "message" ) )
	{
		if( numParms < 1 ) return false;
		entity->message = GetNewString( parameter[0] );
	}
	else if( !stricmp( param , "sound" ) )
	{
		if( numParms < 1 ) return false;
		entity->sound = GetNewString( parameter[0] );
			
		entity->soundIdx = GetGameSampleID ( entity->sound );
			
	}
		
	else if( !stricmp( param , "sound_range_x" ) )
	{
		if( numParms < 1 ) return false;
		entity->soundRangeX = (float) atof( parameter[0] );
	}
	else if( !stricmp( param , "sound_range_y" ) )
	{
		if( numParms < 1 ) return false;
		entity->soundRangeY = (float) atof( parameter[0] );
	}
		
	else if( !stricmp( param , "speed" ) )
	{
		if( numParms < 1 ) return false;
		entity->speed = (float) atof( parameter[0] );
	}
	else if( !stricmp( param , "wait" ) )
	{
		if( numParms < 1 ) return false;
		entity->wait = (float) atof( parameter[0] );
	}
	else if( !stricmp( param , "spawnid" ) )
	{
		if( numParms < 1 ) return false;
		entity->spawnid =  atoi( parameter[0] );
	}
	
	

	return true;
}

char *CEntityMng::GetNewString( char *str )
{
	if( m_numStrings >= MAX_SIZE_ENTITY_STRING )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "Entity String overflow.." );
		return NULL;
	}

	strcpy( m_strings[ m_numStrings++ ] , str );
	return m_strings[ m_numStrings - 1 ];
}


int CEntityMng::GetClassType( char *token )
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

void CEntityMng::AddLink( worldentity_t  *entity )
{
	entity->next = m_linked;
	entity->prev = NULL;
	if( m_linked )
		m_linked->prev = entity;
	m_linked = entity;
}

worldentity_t *CEntityMng::SearchTarget( char *target )
{
	int i;
	worldentity_t  *entity;

	if( !target )  return NULL;

	entity = &m_entities[0];
	for( i = 0 ; i < m_numEntities ; i++ , entity++ )
	{
		if( !entity->targetName ) continue;
		if( !stricmp( entity->targetName , target ) )
			return entity;
	}

	return NULL;
}

worldentity_t *CEntityMng::SearchTarget( char *target , int classType )
{
	int i;
	worldentity_t  *entity;

	if( !target )  return NULL;

	entity = &m_entities[0];
	for( i = 0 ; i < m_numEntities ; i++ , entity++ )
	{
		if( entity->classType != classType ) continue;
		if( !entity->targetName ) continue;
		if( !stricmp( entity->targetName , target ) )
			return entity;
	}

	return NULL;
}


float CEntityMng::CalculateRadius( vec3_t mins , vec3_t maxs )
{
	float distMins , distMaxs , radius;

	if( maxs[2] - mins[2] < m_viewSightMinHeight )
		return -1.0f;

	distMins = VectorDotProduct( mins , mins );
	distMaxs = VectorDotProduct( maxs , maxs );

	if( distMins > distMaxs )
		radius = (float) sqrt( distMins );
	else
		radius = (float) sqrt( distMaxs );

	return radius;
}

void CEntityMng::Initialize( float visibleDist )
{
	worldentity_t  *entity;
	int i;

	m_frameFlag = 0;
	m_visibleDist = visibleDist;
	m_MDLMng->ClearWorldUseFlag();
	m_linked = NULL;
	m_currCameraPathEntity = NULL;
	m_currPathEntity = NULL;
	m_numActivator = 0;

	entity = &m_entities[0];
	for( i = 0 ; i < m_numEntities ; i++ , entity++ )
	{
		entity->linkedCluster = m_world->FindCluster( entity->origin );
		if( entity->linkedCluster < 0 )
		{
			entity->inuse = false;
			continue;
		}

		entity->targetEntity = SearchTarget( entity->target );
		if( entity->targetEntity )
			entity->targetEntity->prevEntity = entity;

		entity->inuse = false;
		switch( entity->classType )
		{
		case GTH_WORLD_ENTITY_TYPE_PLAYER_POS :
			        break;
		
		case GTH_WORLD_ENTITY_TYPE_MODEL :
			        entity->idxMDLEntity = m_MDLMng->AddWorldEntity( entity->itemName , entity->origin 
						                                           , entity->angles );
					if( entity->idxMDLEntity < 0 )	break;
					if( !m_MDLMng->GetMDLWorldClassBoundBox( entity->idxMDLEntity , entity->mins , entity->maxs ) )
					{
						VectorClear( entity->mins );
						VectorClear( entity->maxs );
					}
					entity->radius = CalculateRadius( entity->mins , entity->maxs );
					AddLink( entity );
					entity->inuse = true;
					break;

		case GTH_WORLD_ENTITY_TYPE_TELEPORT :
			    if( !entity->target ) break;
				entity->idxMDLEntity = m_MDLMng->AddWorldEntity( entity->itemName , entity->origin 
						                                           , entity->angles );
				if( entity->idxMDLEntity < 0 )	break;
				if( !m_MDLMng->GetMDLWorldClassBoundBox( entity->idxMDLEntity , entity->mins , entity->maxs ) )
				{
					VectorClear( entity->mins );
					VectorClear( entity->maxs );
				}
			    AddLink( entity );
				entity->inuse = true;
			        break;

		case GTH_WORLD_ENTITY_TYPE_ACTIVATOR :
			    if( !entity->targetEntity ) break;
				AddLink( entity );
				entity->inuse = true;
			        break;

		case GTH_WORLD_ENTITY_TYPE_DEACTIVATOR :
			    if( !entity->targetEntity ) break;
				AddLink( entity );
				entity->inuse = true;
			        break;

		case GTH_WORLD_ENTITY_TYPE_DOOR :
				AddLink( entity );
				entity->activator = GetNewActivator();
				entity->inuse = true;
			        break;

		case GTH_WORLD_ENTITY_TYPE_PATH :
				AddLink( entity );
				entity->activator = GetNewActivator();
				entity->inuse = false;
			        break;

		case GTH_WORLD_ENTITY_TYPE_CAMERA_PATH :
				AddLink( entity );
				entity->activator = GetNewActivator();
				entity->inuse = false;
			        break;

		case GTH_WORLD_ENTITY_TYPE_SOUND :
				AddLink( entity );
				entity->activator = GetNewActivator();
				entity->inuse = true;
			        break;

		default:
			        break;
		}
	}
}

	
int CEntityMng::CheckEntityCharCollision( worldentity_t *entity, bboxf_t bound )
{
	if( ((entity->origin[0] + entity->mins[0]) < bound[0]) &&
		((entity->origin[1] + entity->mins[1]) < bound[1]) &&
		((entity->origin[2] + entity->mins[2]) < bound[2]) )
		return false;

	if( ((entity->origin[0] + entity->maxs[0]) < bound[3]) &&
		((entity->origin[1] + entity->maxs[1]) < bound[4]) &&
		((entity->origin[2] + entity->maxs[2]) < bound[5]) )
		return false;

	int i, j;

	mdlworldentity_t *worldentity = &m_MDLMng->m_MDLWorldEntities[entity->idxMDLEntity];
	mdlworldclass_t *worldClass;
	worldClass = &m_MDLMng->m_MDLWorldClasses[worldentity->idxClass];
	
	int numparts = worldClass->numParts;
	for( i = 0; i < numparts; i++ )
	{
		for( j = 0; j < worldentity->num_cvertex[i]; j++ )
		{
			if( (worldentity->collisionVertex[i][j][0] > bound[0]) &&
				(worldentity->collisionVertex[i][j][0] < bound[3]) &&
				(worldentity->collisionVertex[i][j][1] > bound[1]) &&
				(worldentity->collisionVertex[i][j][1] < bound[4]) &&
				(worldentity->collisionVertex[i][j][2] > bound[2]) &&
				(worldentity->collisionVertex[i][j][2] < bound[5]) )
				return true;
		}
	}

	return false;
}
	

inline int CEntityMng::ChechVisibleDistance( vec3_t origin , vec3_t mins , vec3_t maxs , float radius )
{
	vec3_t diff;
	float  dist;

	VectorSubtract( diff , origin , m_camPos );

	dist = VectorDotProduct( diff , diff );
	if( dist > m_removeDist * m_removeDist )
		return ENTITY_OVER_THAN_VISIBLE;

	if( radius <= 0.0f )
		return ENTITY_OVER_THAN_CHARACTER;

	if( dist < m_camDist * m_camDist )
		return ENTITY_CLOSED_THAN_CHARACTER;

	if( m_palyerPos[0] > ( origin[0] + mins[0] ) && m_palyerPos[0] < ( origin[0] + maxs[0] )
     && m_palyerPos[1] > ( origin[1] + mins[1] ) && m_palyerPos[1] < ( origin[1] + maxs[1] )
	 && m_palyerPos[2] > ( origin[2] + mins[2] ) && m_palyerPos[2] < ( origin[2] + maxs[2] ) )
	    return ENTITY_CLOSED_THAN_CHARACTER;

	return ENTITY_OVER_THAN_CHARACTER;
}

inline int CEntityMng::ChechEffectiveDistance( vec3_t origin , float effectiveDist )
{
	vec3_t dist;

	VectorSubtract( dist , origin , m_palyerPos );

	if( VectorDotProduct( dist , dist ) > effectiveDist * effectiveDist )
		return false;

	return true;
}

	
inline int CEntityMng::CheckEffectiveSoundDistance( vec3_t origin , float range_x, float range_y )
{
	vec3_t dist;

	VectorSubtract( dist , origin , m_palyerPos );

	if( (fabs(dist[0]) < range_x) && (fabs(dist[1]) < range_y) )
		return true;

	return false;
}
	

inline int CEntityMng::ChechEffectiveCamDistance( vec3_t origin , float effectiveDist )
{
	vec3_t dist;

	VectorSubtract( dist , origin , m_camPos );

	if( VectorDotProduct( dist , dist ) > effectiveDist * effectiveDist )
		return false;

	return true;
}

void CEntityMng::Process( vec3_t palyerPos , vec3_t camPos , DWORD frameFlag , float camDist )
{
	m_frameFlag = frameFlag;
	m_MDLMng->SetFrameFlag( m_frameFlag );

	VectorCopy( m_palyerPos , palyerPos );
	VectorCopy( m_camPos , camPos );

	m_camDist = camDist;

	if( m_visibleQuality == GTH_ENTITY_VISIBLE_QUALITY_HIGH )
		m_removeDist = m_visibleDist;
	else
		m_removeDist = m_visibleDist * 0.7f;

	m_visData = m_world->GetVisData();
	if( !m_visData ) return;

	ProcessEntity();
}

inline int CEntityMng::Cull( vec3_t mins , vec3_t maxs , vec3_t position , int linkedCluster )
{
	vec3_t trans_mins , trans_maxs;

	if( !( m_visData[ linkedCluster >> 3 ] & ( 1 << ( linkedCluster & 7 ) ) ) )
		return true;

	VectorAdd( trans_mins , position , mins );
	VectorAdd( trans_maxs , position , maxs );

	

	if( m_frustum->Cull_Exact( trans_mins , trans_maxs ) )
		return true;

	return false;
}


void CEntityMng::ProcessEntity()
{
	worldentity_t         *entity;
	int  worldIdx;
	int  visType;
	int  idxClass;

	m_MDLMng->ClearRenderWorldLink();
	m_MDLMng->ClearBlendWorldLink();
	
	entity = m_linked;
	while( entity )
	{
		if( !entity->inuse )  
		{
			entity = entity->next;
			continue;
		}

		switch( entity->classType )
		{
		case GTH_WORLD_ENTITY_TYPE_PLAYER_POS :
			        break;
		
		case GTH_WORLD_ENTITY_TYPE_MODEL :
			visType = ChechVisibleDistance( entity->origin , entity->mins , entity->maxs , entity->radius );
			if( visType == ENTITY_OVER_THAN_VISIBLE )
			{
				m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].visible = false;
				break;
			}

			if( Cull( entity->mins , entity->maxs , entity->origin , entity->linkedCluster ) )
				m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].visible = false;
			else
			{
				idxClass = m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].idxClass;
				 


				if( ( visType == ENTITY_CLOSED_THAN_CHARACTER ) && m_MDLMng->m_MDLWorldClasses[ idxClass ].hideFlag )
				{
					
					m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].blendNext = m_MDLMng->m_linkedBlendMDLWorldEntities[ idxClass ];
					m_MDLMng->m_linkedBlendMDLWorldEntities[ idxClass ] = &m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ];
					m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].visible = visType;
				}
				else
				{
					m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].renderNext = m_MDLMng->m_linkedRenderMDLWorldEntities[ idxClass ];
					m_MDLMng->m_linkedRenderMDLWorldEntities[ idxClass ] = &m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ];
					m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].visible = visType;
				}
			}

			break;

		case GTH_WORLD_ENTITY_TYPE_TELEPORT :

			if ( g_cgv.flagChangeWorld ) break;

			visType = ChechVisibleDistance( entity->origin , entity->mins , entity->maxs , entity->radius );
			if( visType == ENTITY_OVER_THAN_VISIBLE )
			{
				m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].visible = false;
				break;
			}

			if( Cull( entity->mins , entity->maxs , entity->origin , entity->linkedCluster ) )
				m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].visible = false;
			else
			{
				idxClass = m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].idxClass;
				if( visType == ENTITY_CLOSED_THAN_CHARACTER )
				{
					m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].blendNext = m_MDLMng->m_linkedBlendMDLWorldEntities[ idxClass ];
					m_MDLMng->m_linkedBlendMDLWorldEntities[ idxClass ] = &m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ];
					m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].visible = visType;
				}
				else
				{
					m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].renderNext = m_MDLMng->m_linkedRenderMDLWorldEntities[ idxClass ];
					m_MDLMng->m_linkedRenderMDLWorldEntities[ idxClass ] = &m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ];
					m_MDLMng->m_MDLWorldEntities[ entity->idxMDLEntity ].visible = visType;
				}
			}

			if( !ChechEffectiveDistance( entity->origin , entity->effectiveDist ) )  break;

			
			if(g_SkillLogic.CancelTransform(true)) return ;
			
			
			
			if(true == g_ifMng->m_Item_ScrollWin->IfWorldChangeing())
			{	
				g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 588 ));
				return;
			}

			
			if( g_pApp->m_myCharacter->isTransform ) return ;

			worldIdx = GTH_WorldToIndex( entity->target );
			if( worldIdx < 0 ) break;
			m_spawnID = entity->spawnid;

			
			GTH_RequestChangeWorld( worldIdx, m_spawnID ); 
			break;

		case GTH_WORLD_ENTITY_TYPE_ACTIVATOR :
			if( !ChechEffectiveDistance( entity->origin , entity->effectiveDist ) )  break;
			break;

		case GTH_WORLD_ENTITY_TYPE_DEACTIVATOR :
			if( !ChechEffectiveDistance( entity->origin , entity->effectiveDist ) )  break;
			break;

		case GTH_WORLD_ENTITY_TYPE_DOOR :
			break;

		case GTH_WORLD_ENTITY_TYPE_PATH :
			ProcessPath( entity );
			break;

		case GTH_WORLD_ENTITY_TYPE_CAMERA_PATH :
			ProcessCameraPath( entity );
			break;

		case GTH_WORLD_ENTITY_TYPE_SOUND :
				
			if( !CheckEffectiveSoundDistance( entity->origin , entity->soundRangeX , entity->soundRangeY ) )
			{
				if( entity->soundInPlaying )
				{
					 
					g_musicMng->Stop ( entity->soundIdx );
					entity->soundInPlaying = false;
				}
				break;
			}
			if( entity->soundInPlaying )  break;

			g_musicMng->PlaySample ( entity->soundIdx, GTH_SOUND_PLAY_LOOP );
			entity->soundInPlaying = true;
				
				    break;

		default:
			        break;
		}

		entity = entity->next;
	}
}


activator_t *CEntityMng::GetNewActivator()
{
	if( m_numActivator >= MAX_SIZE_ACTIVATOR )
		return NULL;

	m_activator[ m_numActivator ].playing = false;

	m_numActivator++;
	return &m_activator[ m_numActivator - 1 ];
}


#define GTH_ENTITY_CAMERA_MOVE_SPEED 300.0f
int CEntityMng::ActivateCameraPath( char *start , char *dest , int playType )
{
	activator_t *activator;
	worldentity_t *startEnt , *destEnt , *nextEnt;
	vec3_t diff;
	float  dist;

	if( playType & GTH_ENTITY_PLAY_FORWARD )
	{
		startEnt = SearchTarget( start , GTH_WORLD_ENTITY_TYPE_CAMERA_PATH );
		if( !startEnt )  return false;		
		destEnt = SearchTarget( dest , GTH_WORLD_ENTITY_TYPE_CAMERA_PATH );
		nextEnt = startEnt->targetEntity;
	}
	else
	{
		startEnt = SearchTarget( dest , GTH_WORLD_ENTITY_TYPE_CAMERA_PATH );
		if( !startEnt )  return false;		
		destEnt = SearchTarget( start , GTH_WORLD_ENTITY_TYPE_CAMERA_PATH );
		nextEnt = startEnt->prevEntity;
	}
	
	if( nextEnt )
	{
		VectorSubtract( diff , nextEnt->origin , startEnt->origin );
		dist = (float) sqrt( VectorDotProduct( diff , diff ) );
	}
	else
		dist = 0.0f;

	activator = startEnt->activator;

	activator->startEntity = startEnt;
	activator->destEntity = destEnt;
	activator->currEntity = startEnt;
	
	activator->playType = playType;
	activator->startTime = m_timer->GetAppTime();
	activator->currChainTime = activator->startTime;
	activator->moveSpeed = GTH_ENTITY_CAMERA_MOVE_SPEED * startEnt->speed;
	activator->currChainCompleteTime = dist / activator->moveSpeed;

	if( m_currCameraPathEntity )
		Deactivate( m_currCameraPathEntity );

	activator->playing = true;
	startEnt->inuse = true;
	m_currCameraPathEntity = startEnt;

	return true;
}


#define GTH_ENTITY_PLAYER_MOVE_SPEED 150.0f
int CEntityMng::ActivatePath( int characterID , char *start , char *dest , int playType )
{
	activator_t *activator;
	worldentity_t *startEnt , *destEnt , *nextEnt;
	vec3_t diff;
	float  dist;

	if( playType & GTH_ENTITY_PLAY_FORWARD )
	{
		startEnt = SearchTarget( start , GTH_WORLD_ENTITY_TYPE_PATH );
		if( !startEnt )  return false;
		destEnt = SearchTarget( dest , GTH_WORLD_ENTITY_TYPE_PATH );
		nextEnt = startEnt->targetEntity;
	}
	else
	{
		startEnt = SearchTarget( dest , GTH_WORLD_ENTITY_TYPE_PATH );
		if( !startEnt )  return false;		
		destEnt = SearchTarget( start , GTH_WORLD_ENTITY_TYPE_PATH );
		nextEnt = startEnt->prevEntity;
	}
	
	if( nextEnt )
	{
		VectorSubtract( diff , nextEnt->origin , startEnt->origin );
		dist = (float) sqrt( VectorDotProduct( diff , diff ) );
	}
	else
		dist = 0.0f;

	activator = startEnt->activator;

	activator->startEntity = startEnt;
	activator->destEntity = destEnt;
	activator->currEntity = startEnt;

	activator->playType = playType;
	activator->startTime = m_timer->GetAppTime();
	activator->currChainTime = activator->startTime;
	activator->moveSpeed = GTH_ENTITY_PLAYER_MOVE_SPEED * startEnt->speed;
	activator->currChainCompleteTime = dist / activator->moveSpeed;
	activator->characterID = characterID;

	if( m_currPathEntity )
		Deactivate( m_currPathEntity );

	activator->playing = true;
	startEnt->inuse = true;
	m_currPathEntity = startEnt;

	return true;
}

void CEntityMng::Deactivate( worldentity_t *entity )
{
	entity->activator->playing = false;
	entity->inuse = false;
}

void CEntityMng::ProcessCameraPath( worldentity_t *entity )
{
	activator_t *activator;
	worldentity_t *startEnt , *nextEnt;
	float currRate , diffTime;

	if( entity->classType != GTH_WORLD_ENTITY_TYPE_CAMERA_PATH ) return;

	activator = entity->activator;

	startEnt = (worldentity_t *) activator->currEntity;
	if( activator->playType & GTH_ENTITY_PLAY_FORWARD )
		nextEnt = startEnt->targetEntity;
	else
		nextEnt = startEnt->prevEntity;
	
	if( !nextEnt || ( startEnt == activator->destEntity ) )
	{
		SetCameraState( activator , startEnt , NULL , 0.0f );
		Deactivate( entity );
		m_currCameraPathEntity = NULL;
		return;
	}

	diffTime = m_timer->GetAppTime() - activator->currChainTime;

	if( diffTime < activator->currChainCompleteTime )
	{
		if( activator->currChainCompleteTime > 0.0f )
			currRate = diffTime / activator->currChainCompleteTime;
		else
			currRate = 0.0f;
	}
	else
	{

GTH_LINK_NEXT_CAMERA_PATH_CHAIN :

		diffTime -= activator->currChainCompleteTime;

		startEnt = nextEnt;
		if( !LinkCameraPathChain( startEnt , activator ) )
		{
			SetCameraState( activator , startEnt , NULL , 0.0f );
			Deactivate( entity );
			m_currCameraPathEntity = NULL;
			return;
		}

		if( diffTime >= activator->currChainCompleteTime )
			goto GTH_LINK_NEXT_CAMERA_PATH_CHAIN;

		if( activator->playType & GTH_ENTITY_PLAY_FORWARD )
			nextEnt = startEnt->targetEntity;
		else
			nextEnt = startEnt->prevEntity;

		if( activator->currChainCompleteTime > 0.0f )
			currRate = diffTime / activator->currChainCompleteTime;
		else
			currRate = 0.0f;
	}

	SetCameraState( activator , startEnt , nextEnt , currRate );
}

int CEntityMng::LinkCameraPathChain( worldentity_t *entity , activator_t *activator )
{
	worldentity_t *nextEnt;
	vec3_t diff;
	float  dist;

	if( activator->playType & GTH_ENTITY_PLAY_FORWARD )
		nextEnt = entity->targetEntity;
	else
		nextEnt = entity->prevEntity;
	if( !nextEnt || ( entity == activator->destEntity ) )
		return false;
	
	VectorSubtract( diff , nextEnt->origin , entity->origin );
	dist = (float) sqrt( VectorDotProduct( diff , diff ) );

	activator->currEntity = entity;
	activator->currChainTime += activator->currChainCompleteTime;
	activator->moveSpeed = GTH_ENTITY_CAMERA_MOVE_SPEED * entity->speed;
	activator->currChainCompleteTime = dist / activator->moveSpeed;

	return true;
}

void CEntityMng::SetCameraState( activator_t *activator , worldentity_t *startEnt , worldentity_t *destEnt , float currRate )
{
	vec3_t diff , origin;
	vec3_t diffAngles , angles;

	if( destEnt )
	{
		VectorSubtract( diff , destEnt->origin , startEnt->origin );
		VectorSubtract( diffAngles , destEnt->angles , startEnt->angles );
	}
	else
	{
		VectorClear( diff );
		VectorClear( diffAngles );
	}

	VectorMA( origin , startEnt->origin , diff , currRate );
	VectorMA( angles , startEnt->angles , diffAngles , currRate );
	
	if( activator->playType & GTH_ENTITY_PLAY_REVERSE )
	{
		angles[ PITCH ] = -angles[ PITCH ];
		angles[ YAW ] += 180.0f;
	}

	m_camera->SetPosition( origin );
	m_camera->SetAngles( angles );
}


void CEntityMng::ProcessPath( worldentity_t *entity )
{
	activator_t *activator;
	worldentity_t *startEnt , *nextEnt;
	float currRate , diffTime;

	if( entity->classType != GTH_WORLD_ENTITY_TYPE_PATH ) return;

	activator = entity->activator;

	startEnt = (worldentity_t *) activator->currEntity;
	if( activator->playType & GTH_ENTITY_PLAY_FORWARD )
		nextEnt = startEnt->targetEntity;
	else
		nextEnt = startEnt->prevEntity;
	
	if( !nextEnt || ( startEnt == activator->destEntity ) )
	{
		SetPathState( activator , startEnt , NULL , 0.0f );
		Deactivate( entity );
		m_currPathEntity = NULL;
		return;
	}

	diffTime = m_timer->GetAppTime() - activator->currChainTime;

	if( diffTime < activator->currChainCompleteTime )
	{
		if( activator->currChainCompleteTime > 0.0f )
			currRate = diffTime / activator->currChainCompleteTime;
		else
			currRate = 0.0f;
	}
	else
	{

GTH_LINK_NEXT_PATH_CHAIN :

		diffTime -= activator->currChainCompleteTime;

		startEnt = nextEnt;
		if( !LinkPathChain( startEnt , activator ) )
		{
			SetPathState( activator , startEnt , NULL , 0.0f );
			Deactivate( entity );
			m_currPathEntity = NULL;
			return;
		}

		if( diffTime >= activator->currChainCompleteTime )
			goto GTH_LINK_NEXT_PATH_CHAIN;

		if( activator->playType & GTH_ENTITY_PLAY_FORWARD )
			nextEnt = startEnt->targetEntity;
		else
			nextEnt = startEnt->prevEntity;

		if( activator->currChainCompleteTime > 0.0f )
			currRate = diffTime / activator->currChainCompleteTime;
		else
			currRate = 0.0f;
	}

	SetPathState( activator , startEnt , nextEnt , currRate );
}

int CEntityMng::LinkPathChain( worldentity_t *entity , activator_t *activator )
{
	worldentity_t *nextEnt;
	vec3_t diff;
	float  dist;

	if( activator->playType & GTH_ENTITY_PLAY_FORWARD )
		nextEnt = entity->targetEntity;
	else
		nextEnt = entity->prevEntity;
	if( !nextEnt || ( entity == activator->destEntity ) )
		return false;
	
	VectorSubtract( diff , nextEnt->origin , entity->origin );
	dist = (float) sqrt( VectorDotProduct( diff , diff ) );

	activator->currEntity = entity;
	activator->currChainTime += activator->currChainCompleteTime;
	activator->moveSpeed = GTH_ENTITY_PLAYER_MOVE_SPEED * entity->speed;
	activator->currChainCompleteTime = dist / activator->moveSpeed;

	return true;
}

void CEntityMng::SetPathState( activator_t *activator , worldentity_t *startEnt , worldentity_t *destEnt , float currRate )
{
	vec3_t diff , origin , zero;
	vec3_t diffAngles , angles;
	Fx_CHARACTER_t *character;
	float squareDist;

	if( destEnt )
	{
		VectorSubtract( diff , destEnt->origin , startEnt->origin );
		VectorSubtract( diffAngles , destEnt->angles , startEnt->angles );
	}
	else
	{
		VectorClear( diff );
		VectorClear( diffAngles );
	}

	VectorMA( origin , startEnt->origin , diff , currRate );
	VectorMA( angles , startEnt->angles , diffAngles , currRate );
	if( activator->playType & GTH_ENTITY_PLAY_REVERSE )
	{
		angles[ PITCH ] = -angles[ PITCH ];
		angles[ YAW ] += 180.0f;
	}
	
	character = &m_characterMng->m_Characters[ activator->characterID ];

	VectorClear( zero );
	m_move->Pmove( origin , zero , angles );
	m_characterMng->UpdatePosition( activator->characterID , origin , angles );

	squareDist = VectorDotProduct( diff , diff );
	if( currRate == 0.0f || squareDist == 0.0f )
	{
		character->event = GTH_EV_CHAR_IDLE;
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE);
	}
	else if( startEnt->speed >= 1.5f )
	{
		character->event = GTH_EV_CHAR_RUN;
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_RUN);
	}
	else
	{
		character->event = GTH_EV_CHAR_WALK;
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_WALK);
	}
}