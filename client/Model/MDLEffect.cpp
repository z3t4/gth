



#include "../global_def.h"

#define		MDL_PROJECTILE_SPEED		600		
 
#define		MDL_PROJECTILE_HIT_MARGIN	70
#define		MDL_PROJECTILE_SEND_ATTACK_MARGIN	300

 
#define		MDL_PROJECTILE_INITIAL_VELOCITY		400.0f
#define		MDL_PROJECTILE_ACCELERATION			1500.0f

int CMDLMng::LoadEffectClass( char *fname )
{
	char *token;
	mdleffectclass_t *mdlEffectClass;
	
	if( !m_script->Load( fname ) )
		return false;

	m_script->BeginParse();

	while( 1 )
	{
		token = m_script->GetNextToken( true );
		if( !token[0] ) break;

		if( token[0] != '{' )  continue;

		if( m_numMDLEffectClasses >= MAX_SIZE_MDL_WORLD_CLASS )
		{
			GTH_ERROR( GTH_ERROR_PRINT , "MDL Effect Class overflow.." );
			ShowLogInfo("MDL Effect Class overflow..!!!!!!!!!!!!!" );
			break;
		}

		mdlEffectClass = &m_MDLEffectClasses[ m_numMDLEffectClasses++ ];
	    memset( mdlEffectClass , 0 , sizeof( mdleffectclass_t ) );
		 
		mdlEffectClass->inuse	=	1;

		while( 1 )
		{
			token = m_script->GetNextToken( true );
			if( !token[0] ) break;

			if (token[0] == '{') 
			{
				ParseEffectClassState( mdlEffectClass->states );
			}
	        else if (token[0] == '}')
			    break;
			else
			    ParseEffectClassToken( mdlEffectClass , token );
		}
    }

	return true;
}

int CMDLMng::GetEffectClassStateType( char *token )
{
	if     ( !stricmp( token , "normal" ) )
		return GTH_EFFECT_STATE_NORMAL;
	else if( !stricmp( token , "armed" ) )
		return GTH_EFFECT_STATE_ARMED;
	else if( !stricmp( token , "fired" ) )
		return GTH_EFFECT_STATE_FIRED;
	else if( !stricmp( token , "hitted" ) )
		return GTH_EFFECT_STATE_HITTED;
	else if( !stricmp( token , "missed" ) )
		return GTH_EFFECT_STATE_MISSED;

	return GTH_EFFECT_STATE_NORMAL;
}

float CMDLMng::GetEffectClassAnimTime( char *token )
{
	if     ( !stricmp( token , "full" ) )
		return GTH_EFFECT_ANIMTIME_FULL;
	
	return (float) atof( token );
}

void CMDLMng::ParseEffectClassState( mdleffectclassstate_t *destState )
{
	char *token , *param;
	mdleffectclassstate_t state;
	mdleffectclasspart_t *part;

	memset( &state , 0 , sizeof( mdleffectclassstate_t ) );
	state.normalSize = 100.0f;
	state.soundID = -1;
	
	while( 1 )
	{
		token = m_script->GetNextToken( true );
		if( !token[0] ) break;

	    if (token[0] == '{') 
		{
			part = &state.parts[ state.numParts++ ];
		    ParseEffectClassPart( part );
		}
		else if( !stricmp( token , "height" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  return;
			state.bobHeight = (float) atof( param );
		}
		else if( !stricmp( token , "speed" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  return;
			state.bobSpeed = (float) atof( param );
		}
		else if( !stricmp( token , "state" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  return;
			state.state = GetEffectClassStateType( param );
		}
		else if( !stricmp( token , "total" ) )
		{
		}
		else if( !stricmp( token , "normalsize" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  return;
			state.normalSize = (float) atof( param );
		}
		else if( !stricmp( token , "sound" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  return;
			state.soundID = GetGameSampleID( param );
		}
		else if (token[0] == '}') 
		{
			break;
		}
		else if (	! stricmp ( token, "set_ground" )	)
		{
			param	=	g_script.GetNextToken ( false );
			if	(	! param [0]	)	return;
			if	(	atoi ( param )	==	1	)
			{
				state.setground	=	true;
			}
			else
			{
				state.setground	=	false;
			}
		}
		 
		else if	(	! stricmp( token, "projectile" )	)
		{
			param	=	g_script.GetNextToken ( false );
			if	(	! param [0]	)	continue;
			if	(	atoi ( param )	==	1	)
			{
				state.projectile	=	true;
			}
			else
			{
				state.projectile	=	false;
			}
		}
		 

		else if	(	! stricmp( token, "projectile_height" )	)
		{
			param	=	g_script.GetNextToken ( false );
			if	(	! param[ 0 ]	)	continue;
			state.projectileHeight	=	(float)atof ( param );
		}
		else if	(	! stricmp( token, "start_time" )	)
		{
			param	=	g_script.GetNextToken ( false );
			if	(	! param[ 0 ]	)	continue;
			state.startTime	=	(float)atof ( param );
		}

		m_script->SkipLine();
	}

	memcpy( &destState[ state.state ] , &state , sizeof( mdleffectclassstate_t ) );
}

void CMDLMng::ParseEffectClassPart( mdleffectclasspart_t *part )
{
	char *token , *param;

	part->mdlIdx = -1;
	
	part->msecPerFrame = 100.0f;
	part->scale = 1.0f;
	part->rotSpeed = 0.0f;
	part->startTime = 0.0f;
	part->endTime = GTH_EFFECT_ANIMTIME_FULL;

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
		else if( !stricmp( token , "start" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->startTime = GetEffectClassAnimTime( param );
		}
		else if( !stricmp( token , "end" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->endTime = GetEffectClassAnimTime( param );
		}
		else if( !stricmp( token , "self_rotate_speed" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->selfRotSpeed = (float) atof( param );
		}
		else if( !stricmp ( token , "offset" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] ) continue;
			part->offset[0] = (float) atof( param );
			param = g_script.GetNextToken( false );
			if( !param[0] ) continue;
			part->offset[1] = (float) atof( param );
			param = g_script.GetNextToken( false );
			if( !param[0] ) continue;
			part->offset[2] = (float) atof( param );
		}
		else if( !stricmp( token , "req_level" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			part->req_level = (int) atoi( param );
		}
		else if( !stricmp( token , "billboard" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] )  continue;
			if( ! stricmp( param, "true" ) )
				part->billboard = true;
			else if( ! stricmp( param, "false" ) )
				part->billboard = false;
		}
		else if( !stricmp ( token , "angle_offset" ) )
		{
			param = g_script.GetNextToken( false );
			if( !param[0] ) continue;
			part->angleOffset[0] = (int) atoi( param );
			param = g_script.GetNextToken( false );
			if( !param[0] ) continue;
			part->angleOffset[1] = (int) atoi( param );
			param = g_script.GetNextToken( false );
			if( !param[0] ) continue;
			part->angleOffset[2] = (int) atoi( param );
		}

		m_script->SkipLine();
	}
}

void CMDLMng::ParseEffectClassToken( mdleffectclass_t *mdlEffectClass , char *token )
{
	char *param;

	if( !stricmp( token , "classname" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlEffectClass->classType = GetEffectClassType( param );
	}
	else if( !stricmp( token , "effectname" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		strcpy( mdlEffectClass->effectName , param );
	}
	else if( !stricmp( token , "effectid" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlEffectClass->effectID = (int) atoi( param );
	}
	else if( !stricmp( token , "description" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		strcpy( mdlEffectClass->effectDesc , param );
	}
	else if( !stricmp( token , "maxlevel" ) )
	{
		param = g_script.GetNextToken( false );
		if( !param[0] )  return;
		mdlEffectClass->maxLevel = (int) atoi( param );
	}
	 
	else if	(	! stricmp( token, "inuse" )	)
	{
		param	=	g_script.GetNextToken( false );
		if	(	! param[ 0 ]	)
			return;
		mdlEffectClass->inuse	=	(int)atoi( param );
	}

	m_script->SkipLine();
}

int CMDLMng::GetEffectClassType( char *token )
{
	if     ( !stricmp( token , "gth_effect_model" ) )
		return GTH_EFFECT_CLASS_MODEL;
	else if( !stricmp( token , "gth_effect_multi_model" ) )
		return GTH_EFFECT_CLASS_MULTI_MODEL;
	else if( !stricmp( token , "gth_effect_pointer" ) )
		return GTH_EFFECT_CLASS_POINTER;

	return GTH_EFFECT_CLASS_NONE;
}

int CMDLMng::LinkMDLEffectClass( int idxMDLClass )
{
	int i , j;
	int k;

	mdleffectclass_t *mdlEffectClass;
	mdleffectclassstate_t *state;
	mdleffectclasspart_t *part;
	CMDL *mdl;
	int flag;
	float completeTime , partCompleteTime;

	mdlEffectClass = &m_MDLEffectClasses[idxMDLClass];
	if( mdlEffectClass->loaded )  return true;

	state = &mdlEffectClass->states[0];
	for( i = 0 ; i < MAX_SIZE_GTH_EFFECT_CLASS_STATE ; i++ , state++ )
	{
		if( state->numParts <= 0 ) continue;

		flag = true;
		partCompleteTime = -999999.0f;
		part = &state->parts[0];
		for( j = 0 ; j < state->numParts ; j++ , part++ )
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
			else
			{
				if ( part->endTime < 0 )
				{
					 
					for ( k = part->req_level ; k < MAX_SIZE_EFFECT_LEVEL ; k++ )
					{
						
						mdl = GetMDL( part->mdlIdx );
						if( mdl == NULL ) continue;


						part->numFrames = mdl->m_numBoneFrames;
						completeTime = ( mdl->m_numBoneFrames - 1 ) * part->msecPerFrame;
						if ( ( completeTime + part->startTime ) > state->completeTime[ k ] )
							state->completeTime[ k ] = ( completeTime + part->startTime );
					}
				}
				else
				{
					 
					for ( k = part->req_level ; k < MAX_SIZE_EFFECT_LEVEL ; k++ )
					{
						if ( part->endTime > state->completeTime[k] )
							state->completeTime[k] = part->endTime;
					}
				}
			}
		}
		if( flag )
		{
			CalcEffectBoundBox( state );
		}
		state->loaded = flag;
	}

	if( mdlEffectClass->classType != GTH_EFFECT_CLASS_MULTI_MODEL )
	{
		if( mdlEffectClass->states[ GTH_EFFECT_STATE_NORMAL ].loaded )
			mdlEffectClass->loaded = true;
		else
			mdlEffectClass->loaded = false;
	}
	else 
	{
		if(    mdlEffectClass->states[ GTH_EFFECT_STATE_ARMED ].loaded 
			&& mdlEffectClass->states[ GTH_EFFECT_STATE_HITTED ].loaded )
			mdlEffectClass->loaded = true;
		else
			mdlEffectClass->loaded = false;
	}

	if( mdlEffectClass->loaded )
	{
		if     ( mdlEffectClass->classType == GTH_EFFECT_CLASS_POINTER )
		{
			for ( i = 0 ; i < MAX_SIZE_EFFECT_LEVEL ; i++ )
				mdlEffectClass->states[ GTH_EFFECT_STATE_NORMAL ].completeTime[ i ] = -99999.0f;
			mdlEffectClass->completeTime = -99999.0f;
		}
		else if( mdlEffectClass->classType == GTH_EFFECT_CLASS_MODEL )
			mdlEffectClass->completeTime = mdlEffectClass->states[ GTH_EFFECT_STATE_NORMAL ].completeTime[ 0 ];
		else 
			mdlEffectClass->completeTime = mdlEffectClass->states[ GTH_EFFECT_STATE_NORMAL ].completeTime[ 0 ];
	}

	return mdlEffectClass->loaded;
}

void CMDLMng::CalcEffectBoundBox( mdleffectclassstate_t *state )
{
	mdleffectclasspart_t *part;
	vec3_t stateMins , stateMaxs;
	vec3_t partMins , partMaxs;
	CMDL *mdl;
	int  i;
	
	ClearBounds( stateMins , stateMaxs );
	part = &state->parts[0];
	for( i = 0 ; i < state->numParts ; i++ , part++ )
	{
		
		mdl = GetMDL( part->mdlIdx );
		if( mdl == NULL ) continue;



		VectorCopy( partMins , mdl->m_mins );
		VectorCopy( partMaxs , mdl->m_maxs );

		if( part->rotSpeed != 0.0f )
		{
			CalcSphereBoundBox( partMins , partMaxs );
		}

		VectorScale( partMins , partMins , part->scale );
		VectorScale( partMaxs , partMaxs , part->scale );

		AddPointToBounds( partMins , stateMins , stateMaxs );
		AddPointToBounds( partMaxs , stateMins , stateMaxs );
	}

	if( state->bobSpeed != 0.0f )
	{
		stateMins[2] -= state->bobHeight;
		stateMaxs[2] += state->bobHeight;
	}

	VectorCopy( state->mins , stateMins );
	VectorCopy( state->maxs , stateMaxs );
}

int CMDLMng::InitEffectClass()
{
	for( int i = 0 ; i < m_numMDLEffectClasses ; i++ )
	{	
		 
		if	(	! m_MDLEffectClasses[ i ].inuse	)
			continue;
		LinkMDLEffectClass( i );
	}

	memset( m_linkedMDLEffectEntities , 0 , sizeof( mdleffectentitylink_t ) * MAX_SIZE_MDL_EFFECT_CLASS );
	memset( m_MDLEffectEntities , 0 , sizeof( mdleffectentity_t ) * MAX_SIZE_MDL_EFFECT_ENTITY );

	return true;
}

void CMDLMng::ClearEffectUseFlag()
{
	memset( m_linkedMDLEffectEntities , 0 , sizeof( mdleffectentitylink_t ) * m_numMDLEffectClasses );

	m_currMDLEffectEntity = 0;
}

 
int	CMDLMng::AddEffectEntity	(	int		entityIndex,
									GTH_EFFECT_CLASS_TYPE	classType,
									int		effectID,
									vec3_t	origin,
									vec3_t	target,
									vec3_t	angles,
									float	hitDiff,
									int		effect_level,		
									int		follow_attacker,	
									int		attackerIdx,		
									int		attackerType,		
									int		defenderIdx,		
									int		defenderType,		
									float	offset_x,			
									float	offset_y,			
									float	offset_z,			
									 
									float	in_lifeTime,		
									int		in_follow_repetition	)	 
{
	mdleffectclass_t       *mdlEffectClass;
	mdleffectentity_t      *mdlEntity;
	mdleffectentitylink_t  *linkedMDLEntity;
	int i;
	int idxClass;

	mdlEffectClass = &m_MDLEffectClasses[0];
	for( i = 0 ; i < m_numMDLEffectClasses ; i++ , mdlEffectClass++ )
	{
		if( mdlEffectClass->classType == GTH_EFFECT_CLASS_NONE )  continue;

		if( mdlEffectClass->classType == classType && mdlEffectClass->effectID == effectID )
		{
			idxClass = i;
			break;
		}
	}

	if( i == m_numMDLEffectClasses )
		return -1;
	if( !mdlEffectClass->loaded )
		return -1;



	 
	chASSERT( m_MDLEffectClasses[ idxClass ].inuse );
	if	(	! m_MDLEffectClasses[ idxClass ].inuse	)
	{

		return	-1;
	}

	m_currMDLEffectEntity	=	entityIndex;

	mdlEntity = &m_MDLEffectEntities[ m_currMDLEffectEntity ];

	memset ( mdlEntity, 0, sizeof (mdleffectentity_t) );

	linkedMDLEntity = &m_linkedMDLEffectEntities[ idxClass ];
	mdlEntity->idxClass = idxClass;
	 
	mdlEntity ->idxEntity	=	m_currMDLEffectEntity;

	mdlEntity->next = linkedMDLEntity->linked;
	mdlEntity->prev = NULL;
	if( linkedMDLEntity->linked )
		linkedMDLEntity->linked->prev = mdlEntity;
	linkedMDLEntity->linked = mdlEntity;

	VectorCopy( mdlEntity->origin , origin );
	SetEffectTarget( mdlEntity , target , angles );

	
	mdlEntity->angles[ YAW ] += 90.0f;
	
	mdlEntity->state = GTH_EFFECT_STATE_NORMAL;
	mdlEntity->visible = false;
	mdlEntity->firedStep = 0;
	mdlEntity->hitDiff = hitDiff;

	
	mdlEntity->m_SendAttackMessage=false;

	
	mdlEntity->effectID = effectID;

	 
	

	if	(	follow_attacker	&&	! in_follow_repetition	)
		mdlEntity ->follow_attacker	=	-follow_attacker;
	else
		mdlEntity ->follow_attacker	=	follow_attacker;
	mdlEntity ->attackerIdx		=	attackerIdx;
	mdlEntity ->defenderIdx		=	defenderIdx;
	mdlEntity ->attackerType	=	attackerType;
	mdlEntity ->defenderType	=	defenderType;
	 
	mdlEntity->lifeTime			=	in_lifeTime;

	mdlEntity ->offset [0]		=	offset_x;
	mdlEntity ->offset [1]		=	offset_y;
	mdlEntity ->offset [2]		=	offset_z;

	 
	mdlEntity ->velocity	=	MDL_PROJECTILE_INITIAL_VELOCITY;

	mdlEntity->startTime = m_currMilliTime;
	mdlEntity->effect_level = effect_level;
	
	mdlEntity->completeTime = GetEffectStateCompleteTime( idxClass , GTH_EFFECT_STATE_NORMAL , m_currMilliTime , mdlEntity->effect_level );

	if( m_MDLEffectClasses[ idxClass ].states[ GTH_EFFECT_STATE_NORMAL ].soundID >= 0 )
		m_musicMng->PlaySample3D( m_MDLEffectClasses[ idxClass ].states[ GTH_EFFECT_STATE_NORMAL ].soundID
		                        , GTH_SOUND_PLAY_ONCE , origin );

	return m_currMDLEffectEntity;
}

void CMDLMng::SetEffectTarget( mdleffectentity_t *mdlEntity , vec3_t target , vec3_t angles )
{
	vec3_t diff;

	if( !target ) 
	{
		if( angles )
			VectorCopy( mdlEntity->angles , angles );
		else
			VectorClear( mdlEntity->angles );

		mdlEntity->firedScale = 0.0f;
		return;
	}

	VectorCopy( mdlEntity->target , target );
	VectorSubtract( diff , target , mdlEntity->origin );

	VectorToAngles( diff , mdlEntity->angles );
}

int	CMDLMng::DeleteEffectEntity	(	int	idxMDLEntity	)
{
	mdleffectentity_t	*mdlEntity;

	mdlEntity	=	&m_MDLEffectEntities [idxMDLEntity];
	if	(	mdlEntity ->prev	)
	{
		mdlEntity ->prev ->next	=	mdlEntity ->next;
	}
	else
	{
		m_linkedMDLEffectEntities [mdlEntity ->idxClass].linked	=	mdlEntity ->next;
	}

	if	(	mdlEntity ->next	)
		mdlEntity ->next ->prev	=	mdlEntity ->prev;

	mdlEntity ->next	=	NULL;
	mdlEntity ->prev	=	NULL;

	return true;
}

void CMDLMng::SetMDLEffectEntityOrigin( int idxMDLEntity , vec3_t origin )
{
	VectorCopy( m_MDLEffectEntities[ idxMDLEntity ].origin , origin );
}

void CMDLMng::SetMDLEffectEntityAngles( int idxMDLEntity , vec3_t angles )
{
	VectorCopy( m_MDLEffectEntities[ idxMDLEntity ].angles , angles );
}

mdleffectentity_t *CMDLMng::GetMDLEffectEntity( int idxMDLEntity )
{
	return &m_MDLEffectEntities[ idxMDLEntity ];
}

mdleffectentitylink_t *CMDLMng::GetLinkedMDLEffectEntity( int idxClass )
{
	return &m_linkedMDLEffectEntities[ idxClass ];
}

int CMDLMng::GetMDLEffectClassBoundBox( int idxMDLEntity , int state , vec3_t mins , vec3_t maxs )
{
	mdleffectentity_t      *mdlEntity;
	
	mdlEntity = &m_MDLEffectEntities[ idxMDLEntity ];

	if( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ state ].loaded )
	{
		VectorCopy( mins , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ state ].mins );
		VectorCopy( maxs , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ state ].maxs );
	}
	else
	{
		VectorClear( mins );
		VectorClear( maxs );
	}

	return true;
}

	
float CMDLMng::GetEffectStateCompleteTime( int idxClass , int state , float startTime,
										    int effect_level )
{
	 




	if( m_MDLEffectClasses[ idxClass ].completeTime > 0.0f )
		return m_MDLEffectClasses[ idxClass ].states[ state ].completeTime[ effect_level ] + startTime;
	else 
		return m_MDLEffectClasses[ idxClass ].states[ state ].completeTime[ effect_level ];
}
	

int CMDLMng::IsOverEffectCompleteTime( int idxMDLEntity )
{
	mdleffectentity_t      *mdlEntity;
	
	mdlEntity = &m_MDLEffectEntities[ idxMDLEntity ];

	mdlEntity->animTime = m_currTime;
	if( mdlEntity->completeTime < 0.0f )
		return GTH_EFFECT_NOT_COMPLETED;

	 
	 

	if	(	mdlEntity ->follow_attacker	>	0	)
		return	GTH_EFFECT_NOT_COMPLETED;
		

		
	if( mdlEntity->completeTime > m_currMilliTime )
		return GTH_EFFECT_NOT_COMPLETED;
		

	if( m_MDLEffectClasses[ mdlEntity->idxClass ].classType == GTH_EFFECT_CLASS_MODEL )
		return GTH_EFFECT_ALL_STATE_COMPLETED;
		
	else if( m_MDLEffectClasses[ mdlEntity->idxClass ].classType == GTH_EFFECT_CLASS_POINTER )
		return GTH_EFFECT_ALL_STATE_COMPLETED;
		
	else 
	{
		if     ( mdlEntity->state == GTH_EFFECT_STATE_ARMED )
			return GTH_EFFECT_ARMED_STATE_COMPLETED;	
		else if( mdlEntity->state == GTH_EFFECT_STATE_FIRED )
		{
			if( mdlEntity->firedStep < GTH_EFFECT_FIRED_STEP_COMPLETE )
				return GTH_EFFECT_ARMED_STATE_COMPLETED;
			else
				return GTH_EFFECT_FIRED_STATE_COMPLETED;
		}
			
		 


		else if( mdlEntity->state == GTH_EFFECT_STATE_HITTED )
		{
			if( mdlEntity->firedStep < GTH_EFFECT_HIT_STEP_COMPLETE )
				return GTH_EFFECT_FIRED_STATE_COMPLETED;
			else
				return GTH_EFFECT_ALL_STATE_COMPLETED;
		}
		else if( mdlEntity->state == GTH_EFFECT_STATE_MISSED )
		{
			if( mdlEntity->firedStep < GTH_EFFECT_HIT_STEP_COMPLETE )
				return GTH_EFFECT_FIRED_STATE_COMPLETED;
			else
				return GTH_EFFECT_ALL_STATE_COMPLETED;
		}
			
	}

	return GTH_EFFECT_ALL_STATE_COMPLETED;
}


int CMDLMng::SetFiredState( int idxMDLEntity , vec3_t target , vec3_t mins , vec3_t maxs )
{
	mdleffectentity_t *mdlEntity;
	vec3_t diff;
	float  dist;

	mdlEntity = &m_MDLEffectEntities[ idxMDLEntity ];
	if( !target ) return false;

	
	VectorSubtract( diff , target , mdlEntity->origin );
	VectorNormalize( diff );
	VectorScale( diff , diff , mdlEntity->hitDiff );
	VectorAdd( target , target , diff );

	VectorSubtract( diff , target , mdlEntity->origin );
	dist = (float) sqrt( VectorDotProduct( diff , diff ) );
	VectorToAngles( diff , mdlEntity->angles );
	
	mdlEntity->angles[ YAW ] += 90.0f;

	mdlEntity->firedScale = dist / m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_FIRED ].normalSize;

	VectorCopy( mdlEntity->target , target );
	if( mdlEntity->state == GTH_EFFECT_STATE_ARMED )
	{
		mdlEntity->state = GTH_EFFECT_STATE_FIRED;
		mdlEntity->firedStep = 1;
		mdlEntity->startTime = mdlEntity->completeTime;
			
		 



		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_FIRED_STEP_DIVINE * mdlEntity->firedStep 
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_FIRED , 0 , mdlEntity->effect_level ) );
			

		if( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_FIRED ].soundID >= 0 )
			m_musicMng->PlaySample3D( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_FIRED ].soundID
		                            , GTH_SOUND_PLAY_ONCE , mdlEntity->origin );
	}
	else 
	{
		mdlEntity->firedStep++;
			
		 



		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_FIRED_STEP_DIVINE * mdlEntity->firedStep 
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_FIRED , 0 , mdlEntity->effect_level ) );
			
	}
	
	VectorCopy( mins , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_FIRED ].mins );
	VectorCopy( maxs , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_FIRED ].maxs );

	
	mins[1] *= mdlEntity->firedScale;
	maxs[1] *= mdlEntity->firedScale;

	return true;
}

 


 
int CMDLMng::SetHittedState( int idxMDLEntity , vec3_t target , vec3_t mins , vec3_t maxs )
{
	mdleffectentity_t *mdlEntity;
	vec3_t diff;

	mdlEntity = &m_MDLEffectEntities[ idxMDLEntity ];
		

		
	if( !target ) return false;

	
	VectorSubtract( diff , target , mdlEntity->origin );
	VectorNormalize( diff );
	VectorScale( diff , diff , mdlEntity->hitDiff );
	VectorAdd( target , target , diff );

		 
	 

	if ( VectorCompare ( target , mdlEntity->origin ) )
		VectorCopy( diff , target );
	else
		VectorSubtract( diff , target , mdlEntity->origin );
		
	VectorToAngles( diff , mdlEntity->angles );
	
	mdlEntity->angles[ YAW ] += 90.0f;

	VectorCopy( mdlEntity->target , target );

	if( mdlEntity->state == GTH_EFFECT_STATE_FIRED )
	{
		mdlEntity->state = GTH_EFFECT_STATE_HITTED;
		mdlEntity->firedStep = 1;
		mdlEntity->startTime = mdlEntity->completeTime;

		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_HIT_STEP_DIVINE * mdlEntity->firedStep
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_HITTED , 0 , mdlEntity->effect_level ) );

		if( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_HITTED ].soundID >= 0 )
			m_musicMng->PlaySample3D( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_HITTED ].soundID
									, GTH_SOUND_PLAY_ONCE , mdlEntity->target );
	}
	else
	{
		mdlEntity->firedStep++;

		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_HIT_STEP_DIVINE * mdlEntity->firedStep
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_HITTED , 0 , mdlEntity->effect_level ) );
	}
	
	VectorCopy( mdlEntity->origin , mdlEntity->target );
	
	VectorCopy( mins , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_HITTED ].mins );
	VectorCopy( maxs , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_HITTED ].maxs );

	return true;
}

 


 
int CMDLMng::SetMissedState( int idxMDLEntity , vec3_t target , vec3_t mins , vec3_t maxs )
{
	mdleffectentity_t *mdlEntity;
	vec3_t diff;

	mdlEntity = &m_MDLEffectEntities[ idxMDLEntity ];
		

		

	if( !m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].loaded )
		return false;

	if( !target ) return false;

	
	VectorSubtract( diff , target , mdlEntity->origin );
	VectorNormalize( diff );
	VectorScale( diff , diff , mdlEntity->hitDiff );
	VectorAdd( target , target , diff );

		 
	 

	if ( VectorCompare ( target , mdlEntity->origin ) )
		VectorCopy( diff , target );
	else
		VectorSubtract( diff , target , mdlEntity->origin );
		
	VectorToAngles( diff , mdlEntity->angles );
	
	mdlEntity->angles[ YAW ] += 90.0f;

	VectorCopy( mdlEntity->target , target );

	if( mdlEntity->state == GTH_EFFECT_STATE_FIRED )
	{
		mdlEntity->state = GTH_EFFECT_STATE_MISSED;
		mdlEntity->firedStep = 1;
		mdlEntity->startTime = mdlEntity->completeTime;

		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_HIT_STEP_DIVINE * mdlEntity->firedStep
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_MISSED , 0 , mdlEntity->effect_level ) );

		if( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].soundID >= 0 )
			m_musicMng->PlaySample3D( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].soundID
									, GTH_SOUND_PLAY_ONCE , mdlEntity->target );
	}
	else
	{
		mdlEntity->firedStep++;

		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_HIT_STEP_DIVINE * mdlEntity->firedStep
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_HITTED , 0 , mdlEntity->effect_level ) );
	}

	VectorCopy( mdlEntity->origin , mdlEntity->target );
	
	VectorCopy( mins , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].mins );
	VectorCopy( maxs , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].maxs );

	return true;
}






































































































































































































































































































 


int CMDLMng::IsOverEffectCompleteTime_Neo	(	int		idxMDLEntity,	
												bool	&processDamage,
												bool	&sendAttackMessage	)
{
	mdleffectentity_t      *mdlEntity;
	 
	mdleffectclass_t		*effectclass;
	
	mdlEntity = &m_MDLEffectEntities[ idxMDLEntity ];
	 
	effectclass	=	&m_MDLEffectClasses[ mdlEntity->idxClass ];

	mdlEntity->animTime = m_currTime;

	 
	if	(	mdlEntity->lifeTime	>	0.0f	)
	{
		float	elapsedTime	=	m_currMilliTime - mdlEntity->startTime;
		if	(	elapsedTime	>	mdlEntity->lifeTime	)
		{
			return	GTH_EFFECT_ALL_STATE_COMPLETED;
		}
	}

	if( mdlEntity->completeTime < 0.0f )
		return GTH_EFFECT_NOT_COMPLETED;

	 

	if	(	mdlEntity ->follow_attacker	>	0	)
		return	GTH_EFFECT_NOT_COMPLETED;
	
	 


	if	(	effectclass->states[ GTH_EFFECT_STATE_FIRED ].startTime	<	0	)
	{
		if	(	mdlEntity->state	==	GTH_EFFECT_STATE_ARMED	)
		{
			if	(	( mdlEntity->completeTime + effectclass->states[ GTH_EFFECT_STATE_FIRED ].startTime )	>	m_currMilliTime 	)
				return	GTH_EFFECT_NOT_COMPLETED;
			mdlEntity->backup_startTime	=	mdlEntity->startTime;
			VectorCopy( mdlEntity->backup_origin, mdlEntity->origin );
			VectorCopy( mdlEntity->backup_angles, mdlEntity->angles );
		}
	}
	else if	(	effectclass->states[ GTH_EFFECT_STATE_HITTED ].startTime	<	0	)
	{
		if	(	(	effectclass->states[ GTH_EFFECT_STATE_FIRED ].numParts	<=	0	)	&&
				(	mdlEntity->state	==	GTH_EFFECT_STATE_ARMED	)	)
		{
			if	(	( mdlEntity->completeTime + effectclass->states[ GTH_EFFECT_STATE_HITTED ].startTime )	>	m_currMilliTime 	)
				return	GTH_EFFECT_NOT_COMPLETED;
			 
			mdlEntity->backup_startTime	=	mdlEntity->startTime;
			VectorCopy( mdlEntity->backup_origin, mdlEntity->origin );
			VectorCopy( mdlEntity->backup_angles, mdlEntity->angles );
		}
	}
	else if	(	mdlEntity->completeTime	>	m_currMilliTime	)
	{
		return GTH_EFFECT_NOT_COMPLETED;
	}

	if( m_MDLEffectClasses[ mdlEntity->idxClass ].classType == GTH_EFFECT_CLASS_MODEL )
		return GTH_EFFECT_ALL_STATE_COMPLETED;
	else if( m_MDLEffectClasses[ mdlEntity->idxClass ].classType == GTH_EFFECT_CLASS_POINTER )
		return GTH_EFFECT_ALL_STATE_COMPLETED;
	else 
	{
		 
		 



		 
		switch	(	mdlEntity->state	)
		{
		case	GTH_EFFECT_STATE_ARMED:
			{
				 
				if	(	effectclass->states[ GTH_EFFECT_STATE_FIRED ].projectile	)
					mdlEntity->origin[ 2 ]	+=	effectclass->states[ GTH_EFFECT_STATE_FIRED ].projectileHeight;

				return GTH_EFFECT_ARMED_STATE_COMPLETED;
			}
			break;
		case	GTH_EFFECT_STATE_FIRED:
			{
				vec3_t	diff;
				VectorSubtract ( diff, mdlEntity->target, mdlEntity->origin );
				 
				float	length	=	VectorLength( diff );
				
				 


				
				
				if	(mdlEntity->m_SendAttackMessage && length	<	MDL_PROJECTILE_HIT_MARGIN	)
				{
					processDamage	=	true;
					return GTH_EFFECT_FIRED_STATE_COMPLETED;
				}
				 
				else if	(	length	<	MDL_PROJECTILE_SEND_ATTACK_MARGIN	)
				{
					sendAttackMessage	=	true;
					
					
					mdlEntity->m_SendAttackMessage = true;

					return	GTH_EFFECT_ARMED_STATE_COMPLETED;
				}
				else
					return GTH_EFFECT_ARMED_STATE_COMPLETED;
			}
			break;
		case	GTH_EFFECT_STATE_HITTED:
			{
				if( mdlEntity->firedStep < GTH_EFFECT_HIT_STEP_COMPLETE )
					return GTH_EFFECT_FIRED_STATE_COMPLETED;
				else
					return GTH_EFFECT_ALL_STATE_COMPLETED;
			}
			break;
		case	GTH_EFFECT_STATE_MISSED:
			{
				if( mdlEntity->firedStep < GTH_EFFECT_HIT_STEP_COMPLETE )
					return GTH_EFFECT_FIRED_STATE_COMPLETED;
				else
					return GTH_EFFECT_ALL_STATE_COMPLETED;
			}
			break;
		}
	}

	return GTH_EFFECT_ALL_STATE_COMPLETED;
}

int	CMDLMng::SetFiredState_Neo	(	int		idxMDLEntity,
									vec3_t	target,
									vec3_t	mins,
									vec3_t	maxs	)
{
	mdleffectentity_t	*mdlEntity;
	vec3_t	diff;

	Fx_CHARACTER_t *character;

	mdlEntity	= &m_MDLEffectEntities [idxMDLEntity];
	if	(	! target	)
		return	false;

	 
	VectorSubtract ( diff, target, mdlEntity->origin );
	VectorNormalize ( diff );

	target [0]	= target [0] + ( diff [0] * mdlEntity->hitDiff );
	target [1]	= target [1] + ( diff [1] * mdlEntity->hitDiff );
	target [2]	= target [2] + ( diff [2] * mdlEntity->hitDiff );

	float	scaleFactor;
		 
	 
	 
	 
	 
	character	=	g_charManager ->FindCharacterByServerIndex (mdlEntity ->attackerIdx, mdlEntity ->attackerType);
	if	(	character	==	NULL	)
		return	false;

	 
	 

	 
	 

	scaleFactor	=	character->stat_attackSpeed * 0.001f
						* ( mdlEntity ->velocity * g_timer.GetElapsedTime () + 0.5f * MDL_PROJECTILE_ACCELERATION * g_timer.GetElapsedTime () * g_timer.GetElapsedTime () );
	mdlEntity ->velocity	=	mdlEntity ->velocity + MDL_PROJECTILE_ACCELERATION * g_timer.GetElapsedTime ();

	mdlEntity->origin [0]	= mdlEntity->origin [0] + ( diff [0] * scaleFactor );
	mdlEntity->origin [1]	= mdlEntity->origin [1] + ( diff [1] * scaleFactor );
	mdlEntity->origin [2]	= mdlEntity->origin [2] + ( diff [2] * scaleFactor );

	 
	vec3_t	check;
	VectorSubtract ( check, target, mdlEntity->origin );
	VectorNormalize ( check );
	float	ACCEPTRANGE	=	0.999f;
	if	(	VectorDotProduct ( check, diff )	<	ACCEPTRANGE	)
	{
		 
		VectorCopy ( mdlEntity->origin, target );
	}

	 
	VectorToAngles ( diff, mdlEntity->angles );
		 
	mdlEntity->angles [YAW]	+= 90.0f;

	 
	VectorCopy ( mdlEntity->target, target );

















	if	(	mdlEntity->state	==	GTH_EFFECT_STATE_ARMED	)
	{
		mdlEntity->state		= GTH_EFFECT_STATE_FIRED;
		 
		mdlEntity->startTime	= m_currMilliTime;





		if	(	m_MDLEffectClasses [mdlEntity->idxClass].states [GTH_EFFECT_STATE_FIRED].soundID	>=	0	)
			m_musicMng->PlaySample3D ( m_MDLEffectClasses [mdlEntity->idxClass].states [GTH_EFFECT_STATE_FIRED].soundID, GTH_SOUND_PLAY_ONCE, mdlEntity->origin );
	}








	VectorCopy ( mins, m_MDLEffectClasses [mdlEntity->idxClass].states [GTH_EFFECT_STATE_FIRED].mins );
	VectorCopy ( maxs, m_MDLEffectClasses [mdlEntity->idxClass].states [GTH_EFFECT_STATE_FIRED].maxs );





	return	true;
}


int CMDLMng::SetHittedState_Neo( int idxMDLEntity , vec3_t target , vec3_t mins , vec3_t maxs )
{
	mdleffectentity_t *mdlEntity;
	vec3_t diff;
	Fx_CHARACTER_t *character;

	mdlEntity = &m_MDLEffectEntities[ idxMDLEntity ];
		

		
	if( !target ) return false;

	
	VectorSubtract( diff , target , mdlEntity->origin );
	VectorNormalize( diff );
	VectorScale( diff , diff , mdlEntity->hitDiff );
	VectorAdd( target , target , diff );

		 
	 

	if ( VectorCompare ( target , mdlEntity->origin ) )
		VectorCopy( diff , target );
	else
	{
		 
		 
		 
		vec3_t	char_pos;
		 
		character	=	g_charManager ->FindCharacterByServerIndex ( mdlEntity ->attackerIdx, mdlEntity ->attackerType );
		if	(	character	!=	NULL	)
		{
			VectorCopy ( char_pos,  character->position );
			char_pos [2]	=	target [2];
			VectorSubtract( diff , target , char_pos );
		}
		else
		{
			return	false;
		}
		 

	}
		
	VectorToAngles( diff , mdlEntity->angles );
	
	mdlEntity->angles[ YAW ] += 90.0f;

	VectorCopy( mdlEntity->target , target );

	if( mdlEntity->state == GTH_EFFECT_STATE_FIRED )
	{
		mdlEntity->state = GTH_EFFECT_STATE_HITTED;
		mdlEntity->firedStep = 1;
		 

		mdlEntity->startTime	= m_currMilliTime;

		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_HIT_STEP_DIVINE * mdlEntity->firedStep
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_HITTED , 0 , mdlEntity->effect_level ) );

		if( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_HITTED ].soundID >= 0 )
			m_musicMng->PlaySample3D( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_HITTED ].soundID
									, GTH_SOUND_PLAY_ONCE , mdlEntity->target );
	}
	else
	{
		mdlEntity->firedStep++;

		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_HIT_STEP_DIVINE * mdlEntity->firedStep
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_HITTED , 0 , mdlEntity->effect_level ) );
	}
	
	VectorCopy( mdlEntity->origin , mdlEntity->target );
	
	VectorCopy( mins , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_HITTED ].mins );
	VectorCopy( maxs , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_HITTED ].maxs );

	return true;
}

int CMDLMng::SetMissedState_Neo( int idxMDLEntity , vec3_t target , vec3_t mins , vec3_t maxs )
{
	mdleffectentity_t *mdlEntity;
	vec3_t diff;

	mdlEntity = &m_MDLEffectEntities[ idxMDLEntity ];
		

		

	if( !m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].loaded )
		return false;

	if( !target ) return false;

	
	VectorSubtract( diff , target , mdlEntity->origin );
	VectorNormalize( diff );
	VectorScale( diff , diff , mdlEntity->hitDiff );
	VectorAdd( target , target , diff );

		 
	 

	if ( VectorCompare ( target , mdlEntity->origin ) )
		VectorCopy( diff , target );
	else
		VectorSubtract( diff , target , mdlEntity->origin );
	

	


	


	
	if(mdlEntity->effectID == 6 || mdlEntity->effectID == 31)
	{
		Fx_CHARACTER_t *character	=	g_charManager ->FindCharacterByServerIndex (mdlEntity ->attackerIdx, mdlEntity ->attackerType);
		if	(	character	!=	NULL	)
		{
			VectorToAngles( diff , character->angles );
		}
		else
		{
			VectorToAngles( diff , mdlEntity->angles );
		}
	}
	else
	{
		VectorToAngles( diff , mdlEntity->angles );
		
		mdlEntity->angles[ YAW ] += 90.0f;
	}

	


	VectorCopy( mdlEntity->target , target );

	if( mdlEntity->state == GTH_EFFECT_STATE_FIRED )
	{
		mdlEntity->state = GTH_EFFECT_STATE_MISSED;
		mdlEntity->firedStep = 1;
		 

		mdlEntity->startTime	= m_currMilliTime;

		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_HIT_STEP_DIVINE * mdlEntity->firedStep
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_MISSED , 0 , mdlEntity->effect_level ) );

		if( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].soundID >= 0 )
			m_musicMng->PlaySample3D( m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].soundID
									, GTH_SOUND_PLAY_ONCE , mdlEntity->target );
	}
	else
	{
		mdlEntity->firedStep++;

		mdlEntity->completeTime = mdlEntity->startTime 
			    + ( GTH_EFFECT_HIT_STEP_DIVINE * mdlEntity->firedStep
				  * GetEffectStateCompleteTime( mdlEntity->idxClass , GTH_EFFECT_STATE_HITTED , 0 , mdlEntity->effect_level ) );
	}

	VectorCopy( mdlEntity->origin , mdlEntity->target );
	
	VectorCopy( mins , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].mins );
	VectorCopy( maxs , m_MDLEffectClasses[ mdlEntity->idxClass ].states[ GTH_EFFECT_STATE_MISSED ].maxs );

	return true;
}

 































































































































































































































































































































































 
 
 
 
mdleffectentity_t*	CMDLMng::DeleteEffectManagerEntity	(	mdleffectentity_t	*in_entity	)
{
	mdleffectentity_t	*temp;
	temp	=	in_entity ->next;
	g_effectMng ->Delete ( in_entity ->idxEntity );

	return	temp;
}
