



#include "../global_def.h"
 
#include <assert.h>





CEffectMng::CEffectMng()
{
	m_currEntity = 0;
	m_frameFlag   = 0;
	m_visibleDist = 10000.0f;
	m_linked = NULL;

}


CEffectMng::~CEffectMng()
{
}

void CEffectMng::Initialize( float visibleDist )
{
	int i;

	m_frameFlag = 0;
	m_currEntity = 0;
	m_visibleDist = visibleDist;
	m_MDLMng->ClearEffectUseFlag();

	m_linked = NULL;
	memset( m_entities , 0 , sizeof( effectentity_t ) * MAX_SIZE_EFFECT_ENTITY );
	memset( m_entityInUse , 0 , sizeof( int ) * MAX_SIZE_EFFECT_ENTITY );

	for( i = 0 ; i < MAX_SIZE_EFFECT_ENTITY ; i++ )
		m_entities[i].idxSelf = i;
}

void CEffectMng::ClearUseFlag()
{
	memset( m_entityInUse , 0 , sizeof( int ) * MAX_SIZE_EFFECT_ENTITY );
	m_currEntity = 0;
	m_linked = NULL;
}


 
	 




	 

 
 
 
 
 
int	CEffectMng::AddSelfEffect	(	GTH_EFFECT_CLASS_TYPE	classType,
									int		effectID, 
									vec3_t	origin, 
									int		effect_level,
									int		follow_attacker,
									int		attackerType,	 
									int		attackerIdx,
									float	offset_x,
									float	offset_y,
									float	offset_z,
									float	in_lifeTime,	 
									int		in_follow_repetition,
									
									
									int		effect_type)	 
{
	 
	 
	return	Add ( classType, 
					effectID, 
					origin, 
					NULL, 
					NULL, 
					0.0f, 
					attackerType, 
					attackerIdx, 
					-1, 
					-1, 
					effect_level, 
					follow_attacker, 
					offset_x, 
					offset_y, 
					offset_z, 
					in_lifeTime, 
					in_follow_repetition,

					
					effect_type);
}

 
 
 
 
int	CEffectMng::Add	(	GTH_EFFECT_CLASS_TYPE	classType,
						int		effectID,
						vec3_t	origin,
						vec3_t	target,				
						vec3_t	angles,				
						float	hitDiff,			
						int		attackerType,		
						int		attackerID,			
						int		defenderType,		
						int		defenderID,			
						int		effect_level,		
						int		follow_attacker,	
						float	offset_x,			
						float	offset_y,			
						float	offset_z,			
						 
						float	in_lifeTime,		
						 
						int		in_follow_repetition,

						
						int		effect_type)	
{
	effectentity_t	*entity;
	int				index , found;

	if	(	classType	==	GTH_EFFECT_CLASS_NONE	)
		return	-1;
	found	=	-1;
	for	(	index	=	( m_currEntity + 1 );	\
			index	<	MAX_SIZE_EFFECT_ENTITY;	\
			++index	)
	{
		if	(	! m_entityInUse [index]	)
		{
			found	=	index;
			goto	NEXT_FREE_EFFECT_ENTITY;
		}
	}

	for	(	index	=	0;	\
			index	<=	m_currEntity;	\
			++index	)
	{
		if	(	! m_entityInUse [index]	)
		{
			found	=	index;
			goto	NEXT_FREE_EFFECT_ENTITY;
		}
	}

	if	(	found	==	-1	)
	{
		found	=	m_currEntity + 1;
		if	(	found	>=	MAX_SIZE_EFFECT_ENTITY	)
			found	=	0;
		Delete ( found );
	}

NEXT_FREE_EFFECT_ENTITY :
	m_currEntity = found;

	entity = &m_entities[ m_currEntity ];

	entity->next = m_linked;
	entity->prev = NULL;
	if( m_linked )
		m_linked->prev = entity;
	m_linked = entity;

	entity->classType = classType;
	entity->effectID = effectID;
	entity->isHitted = false;

	entity->attackerType = attackerType;
	entity->attackerID   = attackerID;
	entity->defenderType = defenderType;
	entity->defenderID   = defenderID;

	 
	entity->sendAttackMessage	=	false;
	entity->processDamage		=	false;

	VectorCopy( entity->origin , origin );

	if( angles )
		VectorCopy( entity->angles , angles );
	if( target )
		VectorCopy( entity->target , target );

	
	entity->EffectType	= effect_type;

	 
	entity->idxMDLEntity = m_MDLMng->AddEffectEntity(	m_currEntity, classType, effectID, origin, target, angles, hitDiff, effect_level,
														follow_attacker, attackerID, attackerType, defenderID, defenderType, offset_x, offset_y, offset_z, in_lifeTime, in_follow_repetition );

	if( entity->idxMDLEntity >= 0 )
	{
		if( !m_MDLMng->GetMDLEffectClassBoundBox( entity->idxMDLEntity , GTH_EFFECT_STATE_NORMAL , entity->mins , entity->maxs ) )
		{
			VectorClear( entity->mins );
			VectorClear( entity->maxs );
		}
	}
	else
	{
		m_entityInUse[ m_currEntity ] = false;
		return -1;
	}

	m_entityInUse[ m_currEntity ] = true;
	
	return m_currEntity;
}

int	CEffectMng::Delete	(	int	idxEntity	)
{
	effectentity_t	*entity;

	entity	=	&m_entities [idxEntity];
	if	(	entity ->prev	)
	{
		entity ->prev ->next	=	entity ->next;
	}
	else
	{
		m_linked	=	entity ->next;
	}

	if	(	entity ->next	)
		entity ->next ->prev	=	entity ->prev;

	entity ->next	=	NULL;
	entity ->prev	=	NULL;

	m_entityInUse [idxEntity]	=	false;

	if	(	entity ->idxMDLEntity	>=	0	)
		m_MDLMng ->DeleteEffectEntity ( entity ->idxMDLEntity );

	return	true;
}

inline int CEffectMng::ChechVisibleDistance( vec3_t origin )
{
	vec3_t dist;

	VectorSubtract( dist , origin , m_camPos );

	if( VectorDotProduct( dist , dist ) > m_visibleDist * m_visibleDist )
		return false;

	return true;
}

void CEffectMng::Process( vec3_t pos , DWORD frameFlag )
{
	m_frameFlag = frameFlag;
	
	VectorCopy( m_camPos , pos );
	m_visData = m_world->GetVisData();
	if( !m_visData ) return;

	ProcessEntityFrame();
}

inline int CEffectMng::Cull( vec3_t mins , vec3_t maxs , vec3_t position , int linkedCluster )
{
	vec3_t trans_mins , trans_maxs;

	if( !ChechVisibleDistance( position ) )
		return true;

	 



	VectorAdd( trans_mins , position , mins );
	VectorAdd( trans_maxs , position , maxs );

	

	if( m_frustum->Cull_Exact( trans_mins , trans_maxs ) )
		return true;

	return false;
}

void CEffectMng::ProcessEntityFrame()
{
	effectentity_t *entity;
	int *linkedCount;
	int overType;

	m_MDLMng->ClearEffectLinkedCount();
	linkedCount = m_MDLMng->GetEffectVisibleLinkedCount();

	Fx_CHARACTER_t	*thisChar;

	bool	processDamage	=	false;
	 
	bool	sendAttackMessage	=	false;

	entity = m_linked;
	while	(	entity	)
	{
		 

		overType = m_MDLMng->IsOverEffectCompleteTime_Neo( entity->idxMDLEntity, processDamage, sendAttackMessage );
		 
		 


		if	(	(	processDamage	)	&&
				(	entity->attackerID	>=	0	)	&&
				! entity->processDamage	)
		{
			thisChar	=	g_charManager ->FindCharacterByServerIndex ( entity ->attackerID, entity ->attackerType );
			if	(	thisChar	!=	NULL	)
			{
				 
				thisChar->atk_sendRangeAttackResend	=	false;

				if	(	g_charManager ->CalcAttackLength ( thisChar )	>=	500.0f	)
				{
					 






					thisChar->atk_processDamage	=	true;
					entity->processDamage	=	true;
				}
			}
			else
			{
				 
				entity	=	DeleteSelf ( entity );
				continue;
			}
		}
		 
		if	(	(	sendAttackMessage	)	&&
				(	entity->attackerID	>=	0	)	&&
				! entity->sendAttackMessage	)
		{
			thisChar	=	g_charManager->FindCharacterByServerIndex( entity->attackerID, entity->attackerType );
			if	(	thisChar	!=	NULL	)
			{
				 
				if	(	g_charManager ->CalcAttackLength ( thisChar )	>=	500.0f	)
				{
					
					
					if( entity->effectID == 0 ) thisChar->atk_sendRangeAttackResend	=	true;
					entity->sendAttackMessage	=	true;
				}
			}
			else
			{
				entity	=	DeleteSelf( entity );
				continue;
			}
		}

		
		if ( entity->effectID == 154 || entity->effectID == 153)
		{
			Fx_CHARACTER_t* pFxChar	 =NULL;
			pFxChar = g_charManager->FindCharacterByServerIndex( entity->attackerID, entity->attackerType );
			
			if	(	thisChar	!=	NULL	)
			{
				if ( (g_cgv.syncCharacter[0].nDanBattleIdx < 0 )||
					( g_cgv.syncCharacter[0].worldIdx != g_NewDanBattleSystem.GetDanBattleInfo()->nBattleMapIdx))
				{					
					entity	=	DeleteSelf( entity );
					continue;
				}
				
			}					
		}
		
		


		switch( overType )
		{
		case GTH_EFFECT_NOT_COMPLETED :
			break;
		case GTH_EFFECT_ALL_STATE_COMPLETED :
			
			
			if (entity->attackerID == g_pApp->m_myCharacter->idxOnServer &&
				entity->effectID == WorldChangeSystem::WORLD_CHANGE_EFFECT_IDX)
			{
				
				if(effectentity_t::enumEffectType::EFFECT_TYPE_PC_SUMMON == entity->EffectType)
				{
					
					g_ifMng->m_Item_ScrollWin->Send_Reply_PC_Summon(true);
				}
				else if(effectentity_t::enumEffectType::EFFECT_TYPE_PC_MOVE_SUMMON == entity->EffectType)
				{
					
					g_ifMng->m_Item_ScrollWin->Send_Reday_PC_Move_Summon(true);
				}
				
				
				
					
				
				
				
				
				
			}
			
			
			 
			entity	=	DeleteSelf ( entity );
			continue;

		case GTH_EFFECT_ARMED_STATE_COMPLETED :
			if( m_MDLMng->m_MDLEffectClasses[m_MDLMng->m_MDLEffectEntities[entity->idxMDLEntity].idxClass].states[GTH_EFFECT_STATE_FIRED].numParts < 1 )
			{
				m_MDLMng->m_MDLEffectEntities[entity->idxMDLEntity].state	=	GTH_EFFECT_STATE_FIRED;
				goto	FIRE_STATE_COMPLETED;
			}

#ifdef	_GTH_ONLINE_VERSION
			if( entity->defenderID < 0 )
			{
				 
				entity	=	DeleteSelf ( entity );
				continue;
			}

			if( g_GameTarget.GTH_GetTargetPosition( entity->defenderType , entity->defenderID , entity->target ) )
			{
				if	(	! m_MDLMng->SetFiredState_Neo ( entity->idxMDLEntity , entity->target , entity->mins , entity->maxs )	)
				{
					 
					entity	=	DeleteSelf ( entity );
					continue;
				}
			}
			else
			{
				 
				entity	=	DeleteSelf ( entity );
				continue;
			}
#else
			m_MDLMng->SetFiredState( entity->idxMDLEntity , entity->target , entity->mins , entity->maxs );
#endif

			break;

		case GTH_EFFECT_FIRED_STATE_COMPLETED :
FIRE_STATE_COMPLETED:
#ifdef	_GTH_ONLINE_VERSION
			entity->isHitted = m_numberCtl->CheckHitTarget( entity->attackerType , entity->attackerID , entity->defenderType , entity->defenderID );
#else
			entity->isHitted	= true;
#endif

			if( entity->isHitted )
			{
				if( g_GameTarget.GTH_GetTargetPosition( entity->defenderType , entity->defenderID , entity->target ) )
				{
					if	(	! m_MDLMng ->SetHittedState_Neo ( entity->idxMDLEntity , entity->target , entity->mins , entity->maxs )	)
					{
						 
						entity	=	DeleteSelf ( entity );
						continue;
					}
				}
				else
				{
					 
					entity	=	DeleteSelf ( entity );
					continue;
				}
			}
			else
			{
				if( g_GameTarget.GTH_GetTargetPosition( entity->defenderType , entity->defenderID , entity->target ) )
				{
					if(	!m_MDLMng->SetMissedState_Neo ( entity->idxMDLEntity , entity->target , entity->mins , entity->maxs ) )
					{
						 
						entity	=	DeleteSelf ( entity );
						continue;
					}
				}
				else
				{
					 
					entity	=	DeleteSelf ( entity );
					continue;
				}
			}

			VectorCopy( entity->origin , entity->target );
			break; 

		case	GTH_EFFECT_ERROR:
			 
			entity	=	DeleteSelf ( entity );
			continue;
		}

		 
		 
		mdleffectentity_t	*effectEntity;
		effectEntity	=	&m_MDLMng ->m_MDLEffectEntities [entity ->idxMDLEntity];
		vec3_t	origin;
		if	(	effectEntity ->follow_attacker	)
		{
			
			
			Fx_CHARACTER_t * ret = g_charManager ->FindCharacterByServerIndex ( effectEntity ->attackerIdx, effectEntity ->attackerType );

			if( ret == NULL )
			{
				ErrorLog( "FindCharacterByServerIndex() return NULL!!!" );
				entity = DeleteSelf( entity );
				continue;
			}

			VectorCopy ( origin, ret->position );
		}
		else
		{
			VectorCopy ( origin, entity ->origin );
		}
		VectorAdd ( origin, origin, effectEntity ->offset );

		if	(	Cull ( entity ->mins, entity ->maxs, origin, entity ->linkedCluster )	)
		{
			m_MDLMng->m_MDLEffectEntities[ entity->idxMDLEntity ].visible = false;
		}
		else
		{
			linkedCount[ m_MDLMng->m_MDLEffectEntities[ entity->idxMDLEntity ].idxClass ]++;
			m_MDLMng->m_MDLEffectEntities[ entity->idxMDLEntity ].visible = true;
		}
		
		entity	=	entity ->next;
	}
}

 
effectentity_t*		CEffectMng::DeleteSelf	(	effectentity_t	*in_entity	)
{
	effectentity_t	*temp;
	temp	=	in_entity ->next;
	Delete ( in_entity ->idxSelf );

	return	temp;
}