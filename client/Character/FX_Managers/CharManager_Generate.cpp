 
 
 
 
 
 
 
 
 

 
 
 
#include "../../global_def.h"
#include "../Common.h"
#include <CASSERT>

float FX_CCharManager::GetInitialHeight( Fx_CHARACTER_t *in_char, int in_animIdx )
{	
	if( in_animIdx < 0 )
	{
		ErrorLog( "%s Character GetInitialHeight Error(AnimIndex : %d)", in_char->name, in_animIdx );
		return( 1.0f );			
	}

	if ( !m_pAnimManager->m_Anims[in_animIdx].inUse )		
	{
		return( 1.0f );			
	}

	return m_pAnimManager->m_Anims[in_animIdx].animation.GetInitialHeight ();	
}
float FX_CCharManager::GetInitialTall( Fx_CHARACTER_t *in_char, int in_animIdx )
{	
	if( in_animIdx < 0 )
	{
		ErrorLog( "%s Character GetInitialTall Error(AnimIndex : %d)", in_char->name, in_animIdx );
		return( 1.0f );			
	}

	if ( !m_pAnimManager->m_Anims[in_animIdx].inUse )		
	{
		return( 1.0f );			
	}

	return m_pAnimManager->m_Anims[in_animIdx].animation.GetInitialTall ();		
}
float FX_CCharManager::GetInitialRadius( Fx_CHARACTER_t *in_char, int in_animIdx )
{	
	if( in_animIdx < 0 )
	{
		ErrorLog( "%s Character GetInitialRadius Error(AnimIndex : %d)", in_char->name, in_animIdx );
		return( 1.0f );			
	}

	if ( !m_pAnimManager->m_Anims[in_animIdx].inUse )			
	{
		return( 1.0f );			
	}

	return m_pAnimManager->m_Anims[in_animIdx].animation.GetInitialRadius ();
}

 
 
 
void	FX_CCharManager::GenerateCharacter	(	syncCharacter_t	*data,
												int				numData		)
{
	
	

	Fx_CHARACTER_t	*pNewChar;
	int				backupIdx;
	float			distance;
	int				index,	subindex,	k;
	int				aniSetIndex = 0;

	bool	flagBackupChecked [MAX_CHAR_A_SCENE];
	memset ( flagBackupChecked, 0, sizeof(bool) * MAX_CHAR_A_SCENE );

	 
	memcpy( m_CharactersBackup, m_Characters, sizeof(Fx_CHARACTER_t) * m_iNumCharacters	);
	m_iNumBackupCharacters	=	m_iNumCharacters;	
	
	memset	(	m_Characters, 0, sizeof (Fx_CHARACTER_t) * MAX_CHAR_A_SCENE	);

	m_iNumCharacters	=	numData;

	for	(	index	=	0;	\
			index	<	numData;	\
			++index,	++data	)
	{
		pNewChar		=	&m_Characters[ index ];
		pNewChar->ID	=	index;

		 
		for	(	subindex	=	0;	\
				subindex	<	m_iNumBackupCharacters;	\
				++subindex	)
		{
			if	(	m_CharactersBackup[ subindex ].idxOnServer	==	data->idxOnServer	&&
					m_CharactersBackup[ subindex ].entityType	==	data->entityType	)
			{
				flagBackupChecked[ subindex ]	=	true;
				break;
			}
		}










		if	(	subindex	>=	m_iNumBackupCharacters	) 
			backupIdx	=	-1;
		else
			backupIdx	=	subindex;

		
		
		
		
		
		
		if	(	backupIdx	==	-1	)		 
		{
			SetInitialDataForNewChar( pNewChar, data );
			
			if	(	pNewChar->entityType	==	ENTITY_PC	)
			{
				pNewChar->isTransform			=	data->isTransform;
				pNewChar->transformEntityIdx	=	data->transformEntityIdx;
				pNewChar->transformType			=	data->transformType;

				
				if	(	! data->isTransform	)
				{
					SetPCSpecificDataNormal( pNewChar, data );
				}
				else	
				{
					SetPCSpecificDataTransform( pNewChar, data );
				}

				
				
				pNewChar->chaosPoint		=	data->chaosPoint;		
				pNewChar->pvpMode			=	data->pvpMode;			
				
				
				pNewChar->generalSpeed		=	data->generalSpeed;
			}
			else if	(	pNewChar->entityType	==	ENTITY_MONSTER	||
						pNewChar->entityType	==	ENTITY_NPC	)
			{
				SetNonPCSpecificData( pNewChar, data );
			}

			
			
			pNewChar->stat_height	=	GetInitialHeight( pNewChar, pNewChar->ani_curIdx ) * pNewChar->stat_scale;
			pNewChar->attr_tall		=	GetInitialTall	( pNewChar, pNewChar->ani_curIdx ) * pNewChar->stat_scale;
			pNewChar->stat_radius	=	GetInitialRadius( pNewChar, pNewChar->ani_curIdx ) * pNewChar->stat_scale;

			CheckNewAddedEffect( pNewChar, data );

			pNewChar->ren_generalSpeed	=	100.0f;
			SetCharacterSpeed( pNewChar, (float)data->generalSpeed );

			CheckNewCreature( pNewChar, data );
		}
		else	 
		{
			data->position[ 2 ]	=	g_move->GetHeight( data->position );

			memcpy( pNewChar, &m_CharactersBackup[ backupIdx ], sizeof(Fx_CHARACTER_t) );
			pNewChar->ID	=	index;

			SetInitialDataForExistChar( pNewChar, data );

			if (pNewChar->entityType == ENTITY_PC)
			{
				
				
				if( data->isTransform != pNewChar->isTransform )
				{	
					pNewChar->isTransform			=	data->isTransform;
					pNewChar->transformEntityIdx	=	data->transformEntityIdx;
					pNewChar->transformType			=	data->transformType;

					
					if	(	! data->isTransform		)
					{
						pNewChar->attr_skelID	=	m_pCharLookupMng->GetBoneIDFromTable( ENTITY_PC, pNewChar->tableIdx );

						SetPCSpecificDataNormal( pNewChar, data );
					}
					else
					{
						SetPCSpecificDataTransform( pNewChar, data );
					}

					
					
					pNewChar->stat_height	=	GetInitialHeight	( pNewChar,	pNewChar->ani_curIdx ) * pNewChar->stat_scale;
					pNewChar->attr_tall		=	GetInitialTall		( pNewChar,	pNewChar->ani_curIdx ) * pNewChar->stat_scale;
					pNewChar->stat_radius	=	GetInitialRadius	( pNewChar,	pNewChar->ani_curIdx ) * pNewChar->stat_scale;
				}
				else
				{
					if	(	! data->isTransform	)
					{	
						
						pNewChar->equipNumber	=	data->equipNumber;
						for	(	k	=	0;	\
								k	<	pNewChar->equipNumber;	\
								++k		)
							pNewChar->equipment[ k ]	=	data->equipment[ k ];

						


						



						
						SetDefaultCharacter( index );
						EquipAllItem( index );
					}
				}
				
				
				pNewChar->chaosPoint		=	data->chaosPoint;		
				pNewChar->pvpMode			=	data->pvpMode;			
				
				
				pNewChar->generalSpeed		=	data->generalSpeed;
			}

			pNewChar -> useInterpolation	=	false;
			
			
			if	(	pNewChar->idxOnServer	==	g_cgv.myPCIdxOnServer	&&
					pNewChar->entityType	==	ENTITY_PC	)
			{	  
				VectorCopy( pNewChar->syncPosition, data->position );
			}
			else
			{
				distance	=	GTH_GetDistance2D( data->position, pNewChar->position );
				if	(	distance	>	100		)
				{	
					VectorCopy( pNewChar->position, data->position	);
					VectorCopy( pNewChar->syncPosition, data->position	);
				}
				else
				{
					VectorCopy( pNewChar->syncPosition, data->position );
					pNewChar->useInterpolation	=	true;
				}

				
				pNewChar->event	=	data->event;
				VectorCopy( pNewChar->angles,	data->angles	);
				pNewChar->mouseAngle = data->angles[YAW];
			}

			
			
			pNewChar->boothState = data->boothState;
			if( data->boothState == 2 )
			{
				strcpy( pNewChar->boothName, data->boothName );
			}

			
			GenerateCharacterAffect( pNewChar, data->numStaticEffects, data->staticEffectIndex );
			SetCharacterSpeed ( pNewChar, (float)data ->generalSpeed );

			
			pNewChar->bHide = false;
			for( int p = 0; p < data->numStaticEffects; p++ )
			{
				
				if( data->staticEffectIndex[p] == 44 )
				{
					pNewChar->bHide = true;
				}
			}
		}


		if (pNewChar->entityType == ENTITY_PC)
		{
			pNewChar->skinPart[0] = data->skinPart[0]; 
			pNewChar->skinPart[1] = data->skinPart[1]; 

			pNewChar->m_bWaitingRoom = data->waitingRoom;
			pNewChar->m_JoinNum = data->joinNum;
			pNewChar->m_NameColorIdx = data->nameColorIdx;
		}

		if	(	pNewChar->curRA	>	0	)
			pNewChar->isAlive	=	true;
		else
			pNewChar->isAlive	=	false;

		CheckAuraExist( index );
		pNewChar->SetupVectors ();
	}	

	for	(	index	=	0;	\
			index	<	m_iNumBackupCharacters;	\
			++index		)
	{
		if	(	flagBackupChecked [index]	)	continue;
		 

		Fx_CHARACTER_t	*backup;
		backup	=	&m_CharactersBackup [index];

		for	(	int	subindex	=	0;	\
					subindex	<	backup ->ren_numStaticEffects;	\
					++subindex	)
		{
			int	effectEntityIndex	=	backup ->ren_staticEffectEntityIndex [subindex];
			if	(	effectEntityIndex	>=	0	)
				g_effectMng ->Delete ( effectEntityIndex );
		}
	}

	UpdateSysVBInfo ();
	UpdateCharacterState ();
	FixCharacterPosition ();

	SetTargetID ();
}

 
 
 
 
 
void	FX_CCharManager::GenerateMyCharacter ()
{
	Fx_CHARACTER_t	*pNewChar;
	int		itemTableIdx, itemIdx;

	pNewChar	=	&m_Characters [0];

	
	if( pNewChar->isTransform ) return;

	SetDefaultCharacter ( 0 );

	pNewChar->equipNumber	=	0;
	for	(	int	i	=	0;	\
				i	<	EQUIP_RING;	\
				i++		)
	{
		if	(	g_cgv.myCharacterInfo ->equipment [i]	==	-1	)		continue;
		if( g_cgv.myCharacterInfo->curWeapon == 0 && i == EQUIP_WEAPON_2 ) continue;
		if( g_cgv.myCharacterInfo->curWeapon == 1 && i == EQUIP_WEAPON_1 ) continue;
		itemIdx = g_cgv.myCharacterInfo->equipment[i];
		if( !GTH_CheckRequire( g_cgv.myCharacterInfo, 0, &g_cgv.myCharacterInfo->item[itemIdx] ) ) continue;

		itemTableIdx	=	g_cgv.myCharacterInfo ->item [g_cgv.myCharacterInfo ->equipment [i]].itemTableIdx;
		pNewChar ->equipment [pNewChar ->equipNumber]	=	itemTableIdx;
		pNewChar ->equipNumber++;
	}

	EquipAllItem (0);

	int tableIdx, mainClass, attackAni = 0;

	tableIdx = pNewChar->tableIdx;

	if ( pNewChar->equipNumber > 0 )
	{
		itemIdx = pNewChar->equipment[0];
		mainClass = g_itemTable[itemIdx].classIdx;
		if ( g_itemClassTable[mainClass].attackAtt )
		
		{
			attackAni = g_itemTable[itemIdx].attackAni[tableIdx];
		}
	}
	if( attackAni != pNewChar->ani_curItemSetIndex )
		GTH_ChangeAnimation( pNewChar, pNewChar->ani_curSetType,
		pNewChar->ani_curSetID, true, NULL, true );

	CheckAuraExist (0);

	UpdateSysVBInfo ();

	UpdateCharacterState ();

	SetTargetID ();
}

 
 
 
 
 
 
 
void FX_CCharManager::GenerateCharacterAffect	(	Fx_CHARACTER_t *pNewChar, int affectNum, int affectIdx[] )
{
	if	(	! pNewChar	)
	{
		ErrorLog( "NULL pointer inputed" );
		return;
	}
	if	(	affectNum	>	MAX_NUM_STATIC_EFFECT	)
	{
		ErrorLog( "affactNum<%d> exceeds MAX_NUM_STATIC_EFFECT. ignore all effect", affectNum );
		pNewChar->ren_numStaticEffects	=	0;
		memset( pNewChar->ren_staticEffectEntityIndex, 0, sizeof(pNewChar->ren_staticEffectEntityIndex) );
		memset( pNewChar->ren_staticEffectIndex, 0, sizeof(pNewChar->ren_staticEffectIndex) );
		memset( pNewChar->ren_staticEffectLevel, 0, sizeof(pNewChar->ren_staticEffectLevel) );
		return;
	}
	if	(	affectNum	<	0	)
	{
		ErrorLog( "affactNum<%d> has minus value", affectNum );
		pNewChar->ren_numStaticEffects	=	0;
		memset( pNewChar->ren_staticEffectEntityIndex, 0, sizeof(pNewChar->ren_staticEffectEntityIndex) );
		memset( pNewChar->ren_staticEffectIndex, 0, sizeof(pNewChar->ren_staticEffectIndex) );
		memset( pNewChar->ren_staticEffectLevel, 0, sizeof(pNewChar->ren_staticEffectLevel) );
		return;
	}

	 
	 
		 
	int		tempNumStaticEffects	=	0;
	int		tempStaticEffectsIndex [MAX_NUM_STATIC_EFFECT];
	int		tempStaticEffectsLevel [MAX_NUM_STATIC_EFFECT];
	int		tempStaticEffectsEntityIndex [MAX_NUM_STATIC_EFFECT];
	
	 
	 
	bool	flagProcessed [MAX_NUM_STATIC_EFFECT];
				 
	for	(	int	index	=	0;	\
			index	<	MAX_NUM_STATIC_EFFECT;	\
			++index		)
	{
		flagProcessed [index]	=	false;
	}
	
		 
	if	(	(	pNewChar->ren_numStaticEffects	<	0	)	||
			(	pNewChar->ren_numStaticEffects	>=	MAX_NUM_STATIC_EFFECT	)	)
	{
		ErrorLog( "entityType : %d, name : %s, ren_numStaticEffects : %d", pNewChar->entityType, pNewChar->name, pNewChar->ren_numStaticEffects );
		pNewChar->ren_numStaticEffects	=	0;
	}
	
	int	newEffectCount	=	affectNum;
	for	(	index	=	0;	\
			index	<	pNewChar ->ren_numStaticEffects;	\
			++index	)
	{
		int	effectIndex	=	pNewChar ->ren_staticEffectIndex [index];
		int	effectLevel	=	pNewChar ->ren_staticEffectLevel [index];
		int	effectEntityIndex	=	pNewChar ->ren_staticEffectEntityIndex [index];

		if	(	effectEntityIndex	<	0 || effectEntityIndex >= MAX_SIZE_EFFECT_ENTITY )	
		{
			ErrorLog( "effectEntityIndex is invalid(%d)", effectEntityIndex );
			continue;
		}

		if	(	effectLevel	<	0	)
			effectLevel	=	0;
		
		 
		bool	found	=	false;
		int		newEffectIndex;
		for	(	int	subindex	=	0;	\
					subindex	<	newEffectCount;	\
					++subindex	)
		{
			if	(	effectIndex	==	affectIdx [subindex]	)
			{
				newEffectIndex	=	subindex;
				found	=	true;
				break;
			}
		}

		if	(	found	)
		{
			 
			tempStaticEffectsIndex [tempNumStaticEffects]	=	effectIndex;
			tempStaticEffectsLevel [tempNumStaticEffects]	=	effectLevel;
			tempStaticEffectsEntityIndex [tempNumStaticEffects]	=	effectEntityIndex;
			++tempNumStaticEffects;
			
			 
			effectentity_t	*entity;
			entity	=	&g_effectMng ->m_entities [effectEntityIndex];
			mdleffectentity_t	*mdlEntity;
			mdlEntity	=	&g_effectMng ->m_MDLMng ->m_MDLEffectEntities [entity ->idxMDLEntity];
			mdlEntity ->attackerIdx	=	pNewChar ->idxOnServer;
			
			 
			flagProcessed [newEffectIndex]	=	true;
			
			continue;
		}
		
		 
		if	(	g_effectMng->m_entityInUse[ effectEntityIndex ]	)
		{
			 
			g_effectMng ->Delete ( effectEntityIndex );
		}
	}
	
				 
	if	(	tempNumStaticEffects	>	0	)
	{
		memcpy ( pNewChar ->ren_staticEffectIndex, tempStaticEffectsIndex, sizeof (int) * tempNumStaticEffects );
		memcpy ( pNewChar ->ren_staticEffectLevel, tempStaticEffectsLevel, sizeof (int) * tempNumStaticEffects );
		memcpy ( pNewChar ->ren_staticEffectEntityIndex, tempStaticEffectsEntityIndex, sizeof (int) * tempNumStaticEffects );
	}
	
		 
	
	
	CSkillEffect *pEffect;
	for	(	index	=	0;	\
			index	<	newEffectCount;	\
			++index		)
	{
		if	(	tempNumStaticEffects	>=	MAX_NUM_STATIC_EFFECT	)
		{
			ErrorLog( "entitytype : %d, name : %s, effectnum exceeded maxinum value : %d", pNewChar->entityType, pNewChar->name, tempNumStaticEffects );
			ErrorLog( "index : %d, newEffectCount : %d", index, newEffectCount );
			break;
		}

		 
		if	(	flagProcessed [index]	)	continue;

		 
		bool	found	=	false;
		for	(	int	subindex	=	0;	\
					subindex	<	tempNumStaticEffects;	\
					++subindex	)
		{
			if	(	affectIdx [index]	==	tempStaticEffectsIndex [subindex]	)
			{
				found	=	true;
				break;
			}
		}
		if	(	found	)	continue;
		
		 
		int		skillIndex	=	affectIdx [index];
		if	(	skillIndex	<	0	)
		{
			ErrorLog( "서버에서 minus 값을 가지는 skill index 날아왔음. %d", skillIndex );
			continue;
		}

		int		skillLevel	=	0;
		 
		pNewChar ->ren_staticEffectIndex [tempNumStaticEffects]	=	skillIndex;
		pNewChar ->ren_staticEffectLevel [tempNumStaticEffects]	=	skillLevel;
		
		
		CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillIndex);
		if(pSkillTable == NULL) continue;
		
		pEffect		= pSkillTable->GetSkillEffect();
		if(pEffect == NULL) continue;

		if	(	! pEffect->m_nIsSubEffect)	continue;
		if	(	pEffect->m_nSubEffectID	<	0	)
		{
			ErrorLog( "skillindex<%d> 의 subeffect index<%d> 가 잘못되었음", skillIndex, pEffect->m_nSubEffectID	);
			continue;
		}
		pNewChar ->ren_staticEffectEntityIndex [tempNumStaticEffects]
			=	g_effectMng ->AddSelfEffect (	GTH_EFFECT_CLASS_MODEL, 
												pEffect->m_nSubEffectID,
												pNewChar ->position,
												g_SkillLogic.GTH_GetSkillEffectLevel ( skillIndex, skillLevel ),
												true,
												pNewChar ->entityType,		
												pNewChar ->idxOnServer );	
		
		if	(	pNewChar ->ren_staticEffectEntityIndex [tempNumStaticEffects]	<	0	)
		{
			
			continue;
		}
		else
		{
			
			++tempNumStaticEffects;
		}
		

		
	}

	
	pNewChar ->ren_numStaticEffects	=	tempNumStaticEffects;

	return;
}

 
 
 
 
void	FX_CCharManager::GenerateOtherCharacter	(	int	iCharID	)
{
	Fx_CHARACTER_t	*pNewChar;

	pNewChar	=	&m_Characters [iCharID];
	SetDefaultCharacter ( iCharID );

	EquipAllItem (iCharID);
	CheckAuraExist (iCharID);

	UpdateSysVBInfo ();

	UpdateCharacterState ();

	SetTargetID ();
}

 
 
 
 
 
 
 
void	FX_CCharManager::SetPCSpecificDataNormal	(	Fx_CHARACTER_t	*in_char,
														syncCharacter_t	*in_data	)
{
	
	in_char->equipNumber	=	in_data->equipNumber;
	int		index;
	for	(	index	=	0;	\
			index	<	in_char->equipNumber;	\
			++index	)
		in_char->equipment[ index ]	=	in_data->equipment[ index ];
	
	SetDefaultCharacter( in_char->ID );
	EquipAllItem( in_char->ID );
	InitNewChar( in_char );
	
	in_char->ani_curSpeed	=	3.0f;
	in_char->ani_curRSpeed	=	1.0f / in_char ->ani_curSpeed;
	in_char->ani_option		=	ANIMOPTION_START_NEW_ANIM;

	int		itemIdx,	mainClass;
	int		aniSetIndex	=	0;
	if	(	in_char->equipNumber	>	0	)
	{
		itemIdx		=	in_char->equipment[ 0 ];
		mainClass	=	g_itemTable[ itemIdx ].classIdx;
		
		if	(	g_itemClassTable[ mainClass ].attackAtt	)
			aniSetIndex	=	g_itemTable[ itemIdx ].attackAni[ in_char->tableIdx ];
		else 
			aniSetIndex	=	0;
	}
	else 
		aniSetIndex	=	0;
	
	InitAnimation( in_char->ID, ANIMTYPE_BYITEM, aniSetIndex, ANIM_ITEM_IDLE );
}

 
 
 
 
 
 
 
void	FX_CCharManager::SetInitialDataForNewChar	(	Fx_CHARACTER_t	*in_char,
														syncCharacter_t	*in_data	)
{
	in_char->isNewChar		=	true;

	in_char->idxOnServer	=	in_data->idxOnServer;
	in_char->entityType		=	in_data->entityType;
	in_char->tableIdx		=	in_data->tableIdx;
	
	in_char->pcJob			=	in_data->pcJob;		

	in_char->stat_alive		=	in_data->aliveState;
	if	(	in_char->stat_alive	==	MON_ALIVE_STATE_DISAPPEAR	&& 
			in_char->entityType	!=	ENTITY_PC	)
		in_char->flag_disappear	=	true;
	else
	{
		in_char->flag_disappear			=	false;
		in_char->flag_appTimeStamped	=	false;
		in_char->flag_disappearSound	=	false;
	}

	in_char->skinPart[0]	=	in_data->skinPart[0];
	in_char->skinPart[1]	=	in_data->skinPart[1];
	in_char->skinPart[2]	=	in_data->skinPart[2];
	in_char->skinPart[3]	=	in_data->skinPart[3];

	in_char->worldIdx		=	in_data->worldIdx;

	in_data->position[2]	=	g_move->GetHeight( in_data->position );

	VectorCopy( in_char->position,		in_data->position	);
	VectorCopy( in_char->angles,		in_data->angles		);
	VectorCopy( in_char->syncPosition,	in_data->position	);
	in_char->useInterpolation	=	false;
	in_char->curRA		=	in_data->curRA;
	in_char->curSA		=	in_data->curSA;
	in_char->calMaxRA	=	in_data->calMaxRA;
	in_char->calMaxSA	=	in_data->calMaxSA;

	in_char->moveSpeed	=	(float)in_data->speed;
	in_char->event		=	in_data->event;

	
	if	(	in_char->event	==	GTH_EV_CHAR_MOUSEWALK	)
		in_char->event	=	GTH_EV_CHAR_WALK;
	else if	(	in_char->event	==	GTH_EV_CHAR_MOUSERUN	)
		in_char->event	=	GTH_EV_CHAR_RUN;

	strcpy( in_char->name, in_data->name );

	in_char->ani_elapsed	=	0.0f;
	in_char->ani_oldFrame	=	0.0f;
	in_char->ani_curIdx		=	m_pCharLookupMng->GetAnimIDFromTable( in_char->entityType, in_char->tableIdx,
								in_char->ani_curSetType, in_char->ani_curItemSetIndex, in_char->ani_curSetID, in_char->ID );
	in_char->ani_nextIdx	=	-1;
	in_char->ani_curState.type.changable	=	true;
	in_char->ani_curState.type.repeatable	=	true;
	in_char->ani_curState.type.movable		=	true;
	in_char->ani_curState.type.alive		=	true;
	in_char->ani_curGroup		=	FX_ANIM_GROUP_MOVE;
	in_char->ani_nextGroup		=	FX_ANIM_GROUP_NULL;
	in_char->oldEvent			=	GTH_EV_CHAR_IDLE;

	in_char->attr_skelID	=	m_pCharLookupMng->GetBoneIDFromTable( in_char->entityType, in_char->tableIdx );
	in_char->stat_scale		=	1.0f;
	in_char->tb_do	=	false;
	//lucky 2012 new GM system
	in_char->gameMaster	=	in_data->gameMaster;
	//end
}

 
 
 
 
 
 
 
void	FX_CCharManager::SetNonPCSpecificData	(	Fx_CHARACTER_t	*in_char,
													syncCharacter_t	*in_data	)
{
	if	(	in_char->entityType	==	ENTITY_NPC	)
		in_char->scriptFlag	=	in_data->scriptFlag;

	strcpy( in_char->name, g_monsterTable[ in_char->tableIdx ].name );

	int		index;
	for	(	index	=	0;	\
			index	<	MAX_COUNT_SKIN_PART;	\
			++index		)
		in_char->stat_skinID[ index ]	=	-1;
	for	(	index	=	0;	\
			index	<	MAX_COUNT_ITEM_PART;	\
			++index		) 
		in_char->stat_itemID[ index ]	=	g_monsterTable[ in_char->tableIdx ].tagModelID[ index ];

	in_char->stat_skinID[ SKIN_PART_MONSTER ]	=	GetSkinID( SKIN_PART_MONSTER, g_monsterTable[ in_char->tableIdx ].modelID );

	InitNewChar( in_char );

	in_char->ani_curSpeed	=	1.0f;
	in_char->ani_curRSpeed	=	1.0f / in_char->ani_curSpeed;

	in_char->ani_option		=	ANIMOPTION_START_NEW_ANIM;

	InitAnimation( in_char->ID, g_monsterTable[ in_char->tableIdx ].aniSetType, 0, ANIM_ITEM_IDLE );

	
	in_char->flag_drawSpawnEffect	=	true;
	
	
	in_char->summonValidTime	=	(float)in_data->equipNumber;

	
	if( in_data->specialMonster )
	{
		in_char->stat_scale = 2.0f; 
	}
	else
	{
		
		
		if( g_monsterTable[in_char->tableIdx].physicalSize < 0.1f ) in_char->stat_scale = 1.0f;
		else in_char->stat_scale = g_monsterTable[in_char->tableIdx].physicalSize;
	}
}

 
 
 
 
 
 
 
void	FX_CCharManager::SetPCSpecificDataTransform	(	Fx_CHARACTER_t	*in_char,
														syncCharacter_t	*in_data	)
{
	in_char->attr_skelID	=	m_pCharLookupMng->GetBoneIDFromTable( ENTITY_MONSTER, in_char->transformEntityIdx );

	int		index;
	for	(	index	=	0;	\
			index	<	MAX_COUNT_SKIN_PART;	\
			++index		)
		in_char->stat_skinID[ index ]	=	-1;

	for	(	index	=	0;	\
			index	<	MAX_COUNT_ITEM_PART;	\
			++index		) 
		in_char->stat_itemID[ index ]	=	g_monsterTable[ in_char->transformEntityIdx ].tagModelID[ index ];
	
	in_char->stat_skinID[ SKIN_PART_MONSTER ]	=	GetSkinID( SKIN_PART_MONSTER, g_monsterTable[ in_char->transformEntityIdx ].modelID );
	
	InitNewChar( in_char );

	in_char->ani_curSpeed	=	1.0f;
	in_char->ani_curRSpeed	=	1.0f / in_char->ani_curSpeed;
	
	in_char->ani_option		=	ANIMOPTION_START_NEW_ANIM;
	
	InitAnimation( in_char->ID, g_monsterTable[ in_char->transformEntityIdx ].aniSetType, 0, ANIM_ITEM_IDLE );
}

 
 
 
 
 
 
 
void	FX_CCharManager::CheckNewAddedEffect	(	Fx_CHARACTER_t	*in_char,
													syncCharacter_t	*in_data	)
{

	int		count	=	0;			 

	for	(	int	index	=	0;	\
				index	<	in_data->numStaticEffects;	\
				++index		)
	{
		
		
		if( index >= MAX_NUM_STATIC_EFFECT )
			continue;

		int		skillIndex	=	in_data ->staticEffectIndex[ index ];
		int		skillLevel	=	in_data ->staticEffectLevel[ index ];

		
		if ( skillIndex > MAX_SKILL_NUMBER || skillIndex < 0 ) continue;

		 
		in_char->ren_staticEffectIndex[ count ]	=	skillIndex;
		in_char->ren_staticEffectLevel[ count ]	=	skillLevel;

		
		
		

		CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillIndex);
		if(pSkillTable==NULL) continue;

		CSkillEffect *pEffect = pSkillTable->GetSkillEffect();
		if(pEffect == NULL) continue;

		if(!pEffect->m_nIsSubEffect) continue;

		in_char->ren_staticEffectEntityIndex[ count ]
			=	g_effectMng->AddSelfEffect(	GTH_EFFECT_CLASS_MODEL, 
											
											pEffect->m_nSubEffectID,
											in_char->position,
											g_SkillLogic.GTH_GetSkillEffectLevel( skillIndex, skillLevel ),
											true,
											in_char->entityType,
											in_char->idxOnServer );

		
		if	(	in_char->ren_staticEffectEntityIndex[ count ]	<	0	)
		{
			 
			continue;
		}
		else
		{
			 
			++count;
		}
	}
	in_char->ren_numStaticEffects	=	count;
}


 
 
 
 
 
 
 
void	FX_CCharManager::SetInitialDataForExistChar	(	Fx_CHARACTER_t	*in_char,
														syncCharacter_t	*in_data	)
{
	in_char->isNewChar	=	false;
	
	in_char->pcJob		=	in_data->pcJob;	
	//lucky 2012 new GM system	
	in_char->gameMaster	=	in_data->gameMaster;
	//end
	
	if	(	in_char->entityType	!=	ENTITY_PC	&&
			in_data->aliveState	==	MON_ALIVE_STATE_FADE	)
	{
		if	(	in_char->stat_alive	!=	MON_ALIVE_STATE_FADE	)
			in_char->flag_drawSpawnEffect	=	true;
	}

	in_char->stat_alive	=	in_data->aliveState;

	if	(	in_char->stat_alive	==	MON_ALIVE_STATE_DISAPPEAR	&&
			in_char->entityType	!=	ENTITY_PC	)
		in_char->flag_disappear	=	true;
	else
	{
		in_char->flag_disappear			=	false;
		in_char->flag_appTimeStamped	=	false;
		in_char->flag_disappearSound	=	false;
	}

	in_char->curRA		=	in_data->curRA;
	in_char->curSA		=	in_data->curSA;
	in_char->moveSpeed	=	(float)in_data->speed;
	in_char->calMaxRA	=	in_data->calMaxRA;
	in_char->calMaxSA	=	in_data->calMaxSA;

	
	if( in_char->entityType == ENTITY_MONSTER )
	{
		if( in_char->summonValidTime > 0 )
		{
			in_char->summonValidTime	=	(float)in_data->equipNumber;
		}
	}
}
