 #include "../global.h"

#include "../Tools/CTools.h"
extern CTools* gcpTools;

#include "..\GolryunBattle\tagGolryunBattle.h"

#include "..\GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;










void GTH_InitGameServer()
{
	srand( (unsigned int)time( NULL ) );

	
	GTH_InitGlobalTime();
	g_timer->Start();

	
	GTH_Zone_Init(); 

	g_pcNumber = 0;
	
	g_maxPCNumber = 0;
	memset(&g_pc[0], 0, MAX_PCS * sizeof(playerCharacter_t));
}

float GTH_GetDistance(vec3_t s, vec3_t t)
{
	return sqrt( (t[0]-s[0])*(t[0]-s[0]) + (t[1]-s[1])*(t[1]-s[1]) + (t[2]-s[2])*(t[2]-s[2]) );
}

float GTH_GetDistance2D(vec3_t s, vec3_t t)
{
	return sqrt( (t[0]-s[0])*(t[0]-s[0]) + (t[1]-s[1])*(t[1]-s[1]) );
}

void GTH_SaveData_AlivedClient()
{
	int i;
	int count;
	playerCharacter_t *pc;

	count = 0;
	pc = &g_pc[0];
	for (i=0; i < MAX_PCS; i++, pc++)
	{
		if ( pc->active ) 
		{
			
			
			GTH_ForcedQuit( pc );
			count ++;
			DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);
		}

		if ( count >= 50 )
		{
			DB_Waiting_ProcessRemainderQuery();
			count = 0;
		}
	}

	
	DB_Waiting_ProcessRemainderQuery();
}

int GTH_GetHeightFromCollisionMap( int worldIdx, vec3_t position )
{
	int x, y;
	float height;
	collisionMap_t *map;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	map = &g_cmap[localWorldIdx];

	x = (int)position[0] / (int)map->scale;
	y = (int)position[1] / (int)map->scale;

	
	if( x < 0 || x >= map->xsize ) return( 0 );
	if( y < 0 || y >= map->ysize ) return( 0 );

	height = (int) map->heightMap[ y * map->xsize + x ];

	return height;
}

int GTH_GetLocalWorldIdx( int worldIdx )
{
	int i;

	for (i=0; i<g_config.serviceWorldNumber; i++)
	{
		if ( g_config.serviceWorldList[i] == worldIdx )
			return i;
	}

	return -1;
}





int GTH_GetEmptyAffectIdx( playerCharacter_t *pc )
{
	int i;
	for( i = 0; i < MAX_AFFECT; i ++ )
	{
		if( pc->affect[i].type == AFFECT_NONE ) break;
	}
	if( i == MAX_AFFECT ) return( -1 );
	return( i );
}

int GTH_GetWeaponClass( playerCharacter_t *pc )
{
	int itemIdx = pc->equipment[pc->curWeapon + EQUIP_WEAPON_1];
	int tableIdx, classIdx;
	
	if( itemIdx < 0 ) return( -1 );

	tableIdx = pc->item[itemIdx].itemTableIdx;
	classIdx = g_itemTable[tableIdx].classIdx;

	return( classIdx );
}






int GTH_OpenStore( int idx )
{
	storeStruct_t	*store;
	store = &g_store[idx];
	
				

	int i, j, nItem = 0, itemTableIdx;

	for( i = 0; i < MAX_NUMBER_OF_STORE_ITEM_PAGE; i ++ )
	{
		
		for( j = 0; j < store->defaultItem[i].defaultNumber; j ++ )
		{
			InitItem( &store->item[nItem].item );

			if( store->defaultItem[i].itemIndex[j] == STORE_ITEM_INDEX_EPACK_RA )
			{
				if( store->chargeRACost <= 0 ) continue;
				store->item[nItem].item.itemTableIdx = STORE_ITEM_INDEX_EPACK_RA;
			}
			else if( store->defaultItem[i].itemIndex[j] == STORE_ITEM_INDEX_EPACK_SA )
			{
				if( store->chargeSACost <= 0 ) continue;
				store->item[nItem].item.itemTableIdx = STORE_ITEM_INDEX_EPACK_SA;
			}
			
			else 
			{
				itemTableIdx = store->defaultItem[i].itemIndex[j];
				
				if( !SpawnItem( &store->item[nItem].item, itemTableIdx ) ) continue;
				
				store->item[nItem].item.durability = g_itemTable[itemTableIdx].durability;
				store->item[nItem].price = GTH_GetBuyPrice( &store->item[nItem].item );
			}
			store->item[nItem].page = i;
			store->item[nItem].maxNumber = store->item[nItem].curNumber = store->defaultItem[i].itemNumber[j];

			nItem ++;
		}
		


		
		for( j = 0; j < store->skill[i].skillIdxNumber; j ++ )
		{
			InitItem( &store->item[nItem].item );
			 

			
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(store->skill[i].skillIndex[j]);
			if(pSkillTable==NULL)
			{
				TRACE("Error! pSkillTable==NULL   \n");
				return 0;
			}

			store->item[nItem].price = pSkillTable->m_nSphereCost;

			
			
			store->item[nItem].item.itemTableIdx = store->skill[i].skillIndex[j] + STORE_SKILL_MASK;

			
			
			store->item[nItem].page = i;
			store->item[nItem].maxNumber = store->item[nItem].curNumber = -1;				

			nItem ++;
		}
	}
	store->itemNumber = nItem;
	

	return 1;
}

int GTH_GetWeaponItemIdx( playerCharacter_t *pc )
{
	int itemIdx = pc->equipment[pc->curWeapon + EQUIP_WEAPON_1];
	int tableIdx;
	
	if( itemIdx < 0 ) return( -1 );

	tableIdx = pc->item[itemIdx].itemTableIdx;
	return( tableIdx );
}



float GTH_GetHitRate( void *attacker, int attackerType, void *defender, int defenderType, int addHitPower )
{
	playerCharacter_t *attackerPC, *defenderPC;
	monsterCharacter_t *attackerMon, *defenderMon;

	float	hitPower, avoidPower, hitRate;
	int		atkLevel, defLevel;

	if( attackerType == ENTITY_PC ) 
	{
		attackerPC	=	static_cast<playerCharacter_t*>(attacker);
		atkLevel	=	attackerPC->level;
		hitPower	=	(float)attackerPC->calHitPower;
	}
	else
	{
		attackerMon	=	static_cast<monsterCharacter_t*>(attacker);
		atkLevel	=	attackerMon->basic.level;
		hitPower	=	(float)attackerMon->calHitPower;
	}

	if( defenderType == ENTITY_PC )
	{
		defenderPC	=	static_cast<playerCharacter_t*>(defender);
		defLevel	=	defenderPC->level;
		avoidPower	=	(float)defenderPC->calAvoidPower;
	}
	else
	{
		defenderMon	=	static_cast<monsterCharacter_t*>(defender);
		defLevel	=	defenderMon->basic.level;
		avoidPower	=	(float)defenderMon->calAvoidPower;
	}
	
	hitPower	+=	addHitPower;
	hitRate		=	hitPower	-	avoidPower;		
	
	
	
	if( attackerType == ENTITY_PC && defenderType != ENTITY_PC )
	{
		if( defLevel - atkLevel == 5 ) hitRate	-=	30;
		else if( defLevel - atkLevel == 6 ) hitRate	-=	35;
		else if( defLevel - atkLevel == 7 ) hitRate	-=	40;
		else if( defLevel - atkLevel == 8 ) hitRate	-=	45;
		else if( defLevel - atkLevel >= 9 ) hitRate	-=	50;
	}

	
	if( attackerType == ENTITY_PC )	hitRate	+=	attackerPC->optValue[OV_HIT_RATE];
	
	if( defenderType == ENTITY_PC ) hitRate	-=	defenderPC->optValue[OV_AVOID_RATE];



	
	if( attackerType == ENTITY_PC )
	{
		if( hitRate < 20.0f ) hitRate = 30.0f;
		if( hitRate > 95.0f ) hitRate = 95.0f;
	}
	else
	{
		if( hitRate < 20.0f ) hitRate = 30.0f;
		if( hitRate > 95.0f ) hitRate = 95.0f;
	}
	
	return( hitRate );
}

float GTH_GetAffectHitRate( void *attacker, int attackerType, void *defender, int defenderType, int type )
{
	
	
	return( 0.0f );


}

float GTH_GetMagicHitRate( void *attacker, int attackerType, void *defender, int defenderType )
{
	
	
	return( 100.0f );


}

float GTH_GetCriticalRate( void *attacker, int attackerType, void *defender, int defenderType )
{
	playerCharacter_t *attackerPC, *defenderPC;
	monsterCharacter_t *attackerMon, *defenderMon;

	float atkLevel, defLevel;
	float hitRate;
	float deCriticalRate;

	if( attackerType == ENTITY_PC ) 
	{
		attackerPC = static_cast<playerCharacter_t*>(attacker);
		atkLevel = (float)attackerPC->level;
		hitRate = (float)attackerPC->calCriticalRate;
	}
	else
	{
		attackerMon = static_cast<monsterCharacter_t*>(attacker);
		atkLevel = (float)attackerMon->basic.level;
		hitRate = (float)attackerMon->calCriticalRate;
	}

	if( defenderType == ENTITY_PC )
	{
		defenderPC = static_cast<playerCharacter_t*>(defender);
		defLevel = (float)defenderPC->level;
		deCriticalRate = (float)defenderPC->optValue[OV_DECRITICAL];
	}
	else
	{
		defenderMon = static_cast<monsterCharacter_t*>(defender);
		defLevel = (float)defenderMon->basic.level;
		deCriticalRate = 0.0f;
	}

	
	hitRate -= deCriticalRate;

	if( (int)hitRate <= 0 ) hitRate = 0.0f;

	return( hitRate );
}



void GTH_GetAttackSkillDamage(const int TargetEntityIdx,const int TargetEntityType,const message_t* message,int &SkillPhyDmg,int &SkillMysteryPosDmg,int &SkillMysteryNegDmg,int &SkillDefence)
{
	playerCharacter_t* pPlayer=NULL;
	monsterCharacter_t* pMonster=NULL;

	
	
	
	if( TargetEntityType == ENTITY_PC)
	{
		pPlayer =  gcpTools->GetPlayerRecordPointer(TargetEntityIdx);
		if(NULL == pPlayer) return;
	}
	else 
	{
		pMonster = gcpTools->GetMonsterRecordPointer(TargetEntityIdx);
		if(NULL == pMonster) return;
	}


	
	
	
	

	if(message->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_RA_RATIO_DAMAGE)
	{
		if( TargetEntityType == ENTITY_PC )
		{
			SkillPhyDmg				=pPlayer->calMaxRA * ( (float) message->data[MSG_ATKDATA_ETC_DATA_1] * 0.01f);
			SkillMysteryPosDmg		=pPlayer->calMaxRA * ( (float) message->data[MSG_ATKDATA_ETC_DATA_0] * 0.01f);
		}
		else
		{
			SkillPhyDmg				=pMonster->calMaxRA * ( (float) message->data[MSG_ATKDATA_ETC_DATA_1] * 0.01f);
			SkillMysteryPosDmg		=pMonster->calMaxRA * ( (float) message->data[MSG_ATKDATA_ETC_DATA_0] * 0.01f);
		}
	}
}


void GTH_GeSkillDamage(const int entityIdx,const int entityType,const message_t* message,int _Type,float &Val)
{
	playerCharacter_t* pPlayer=NULL;
	monsterCharacter_t* pMonster=NULL;

	
	
	
	if( entityType == ENTITY_PC)
	{
		pPlayer =  gcpTools->GetPlayerRecordPointer(entityIdx);
		if(NULL == pPlayer) return;
	}
	else 
	{
		pMonster = gcpTools->GetMonsterRecordPointer(entityIdx);
		if(NULL == pMonster) return;
	}

	
	
	
	float *pOptValue=NULL;

	if( entityType == ENTITY_PC )
	{
		pOptValue= &(*(pPlayer->optValue));
	}
	else
	{
		pOptValue= &(*(pMonster->optValue));
	}	


	switch(_Type) {
	case Skill_Enum_Data_Type::SKILL_DMG_PHYDMG:
		Val	*= ( 1 + (float) pOptValue[OV_SKILL_FINAL_ADD_PHY_DMG_RATE] * 0.01f);					
		break;
	case Skill_Enum_Data_Type::SKILL_DMG_MYSTERY_POS_DMG:
		Val	*= ( 1 + (float) pOptValue[OV_SKILL_FINAL_ADD_MYSTERY_POS_DMG_RATE] * 0.01f);			
		break;
	case Skill_Enum_Data_Type::SKILL_DMG_MYSTERY_NEG_DMG:
		Val	*= ( 1 + (float) pOptValue[OV_SKILL_FINAL_ADD_MYSTERY_NEG_DMG_RATE] * 0.01f);			
		break;
	case Skill_Enum_Data_Type::SKILL_DMG_DEFENCE:
		Val	*= ( 1 + (float) pOptValue[OV_SKILL_FINAL_ADD_DEFENCE_RATE] * 0.01f);					
		break;
	}
	

	
	
	

	return ;
}








void GTH_AnalyzeAttackMessageEtcInfo( message_t *message, int eachDmg[] )
{
	switch( message->data[MSG_ATKDATA_ETC_INFO] )
	{
	
	case MSGINFO_ATK_ETC_INFO_RASTEAL :
		{
			int raStealPoint;
			raStealPoint = (int)( (float)eachDmg[0] * (float)message->data[MSG_ATKDATA_ETC_DATA_0] * 0.01f );
			if( message->fromType == ENTITY_PC )
			{
				if( g_pc[message->fromIdx].alive )
				{
					g_pc[message->fromIdx].curRA += (float)raStealPoint;
					if( g_pc[message->fromIdx].curRA > g_pc[message->fromIdx].calMaxRA ) 
						g_pc[message->fromIdx].curRA = g_pc[message->fromIdx].calMaxRA;
				}
			}
			else
			{
				if( g_monster[message->fromIdx].aliveState == MON_ALIVE_STATE_NORMAL )
				{
					g_monster[message->fromIdx].curRA += (float)raStealPoint;
					if( g_monster[message->fromIdx].curRA > g_monster[message->fromIdx].calMaxRA ) 
						g_monster[message->fromIdx].curRA = g_monster[message->fromIdx].calMaxRA;
				}
			}
		}
	case MSGINFO_ATK_ETC_INFO_STUN :
		{
			
			
			float fRandom = GTH_Random();

		
			if( fRandom < (float)message->data[MSG_ATKDATA_ETC_DATA_0] * 0.01f )
			{
				if( message->toType == ENTITY_PC )
				{
					
					g_pc[message->toIdx].recoveryTime		=	(float)message->data[MSG_ATKDATA_ETC_DATA_1] * 1000.0f;
					g_pc[message->toIdx].event				=	GTH_EV_CHAR_STUN;
					g_pc[message->toIdx].saveSkillIdx		=	-1;
					g_pc[message->toIdx].skillCastingtime	=	0.0f;
					g_pc[message->toIdx].skillCastingState	=	true;
				}
				else
				{
					g_monster[message->toIdx].recoveryTime	=	(float)message->data[MSG_ATKDATA_ETC_DATA_1] * 1000.0f;
					g_monster[message->toIdx].event			=	GTH_EV_CHAR_STUN;
					
				}
				
				message->data[MSG_ATKDATA_STUN_FLAG]	=	MSG_ATKDATA_STUN_TYPE_STUN;
			}
		}
		break;
		
	case MSGINFO_ATK_ETC_INFO_SHOCK :
		{
			
			if( GTH_Random() < (float)message->data[MSG_ATKDATA_ETC_DATA_0] * 0.01f )
			{
				if( message->toType == ENTITY_PC )
				{
					g_pc[message->toIdx].recoveryTime		=	(float)message->data[MSG_ATKDATA_ETC_DATA_1];
					g_pc[message->toIdx].event				=	GTH_EV_CHAR_STUN;
					g_pc[message->toIdx].saveSkillIdx		=	-1;
					g_pc[message->toIdx].skillCastingtime	=	0.0f;
					g_pc[message->toIdx].skillCastingState	=	MSG_ATKDATA_STUN_TYPE_SHOCK;
				}
				else
				{
					g_monster[message->toIdx].recoveryTime	=	(float)message->data[MSG_ATKDATA_ETC_DATA_1] * 1000.0f;
					g_monster[message->toIdx].event			=	GTH_EV_CHAR_STUN;
				}
				
				message->data[MSG_ATKDATA_STUN_FLAG]	=	MSG_ATKDATA_STUN_TYPE_SHOCK;
			}
		}
		break;

	case MSGINFO_ATK_ECT_INFO_PUSHED:
		{
			
			if( message->toType == ENTITY_PC )
			{
				g_pc[message->toIdx].recoveryTime		=	(float)message->data[MSG_ATKDATA_ETC_DATA_0];
				g_pc[message->toIdx].event				=	GTH_EV_CHAR_PUSHED;
				g_pc[message->toIdx].saveSkillIdx		=	-1;
				g_pc[message->toIdx].skillCastingtime	=	0.0f;
				g_pc[message->toIdx].skillCastingState	=	MSG_ATKDATA_STUN_TYPE_PUSHED;

			}
			else
			{
				g_monster[message->toIdx].recoveryTime	=	(float)message->data[MSG_ATKDATA_ETC_DATA_0] * 1000.0f;
				g_monster[message->toIdx].event			=	GTH_EV_CHAR_PUSHED;
			}
			
			message->data[MSG_ATKDATA_STUN_FLAG]	=	MSG_ATKDATA_STUN_TYPE_PUSHED;
		}
		break;

	
	case MSGINFO_ATK_ETC_INFO_HIDE:
		{
			if( message->toType == ENTITY_PC )
			{
			}
			else
			{
			}
		}
		break;
	}

}


void GTH_GetExpFromLevelDiffer( int levelDiffer, int &exp, int &genExp, int &genCapability )
{
	float a, b;
	if( levelDiffer <= 1 ) return;
	if( levelDiffer >= 2 && levelDiffer <= 8  ) 
	{ 
		a = (float)levelDiffer-1;
		b = ( a * a + 15.0f * a ) * 0.5f * 0.01f;

		exp = (int)( (float)exp * ( 1.0f - b )  );
		genExp = (int)( (float)genExp * ( 1.0f - b )  );
		genCapability = (int)( (float)genCapability * ( 1.0f - b )  );
	}
	else
	{
		exp = (int)( (float)exp * 0.1f );
		genExp = (int)( (float)genExp * 0.1f );
		genCapability = (int)( (float)genCapability * 0.1f );
	}
}

int GTH_IsSharingParty( monsterCharacter_t *m, playerCharacter_t *pc )
{
	float dist;

	if( m->worldIdx != pc->worldIdx ) return 0;
	dist = GTH_GetDistance2D( pc->position, m->position );
	if( dist > 2400.0f ) return 0;

	return 1;
}





















































































int GTH_MakePortal( int makerType, int makerIdx, int keepTime, int numPerson, int worldIdx, vec3_t position, int targetWorldIdx, vec3_t targetPosition )
{
	int i;

	
	itemObject_t *itemObject;

	if ( g_objectNumber >= MAX_OBJECTS ) return -1;	
	
	itemObject = &g_object[0];

	for (i=0; i < MAX_OBJECTS; i++, itemObject++)
	{
		if ( !itemObject->active ) break;
	}

	if ( i >= MAX_OBJECTS ) return -1;

	itemObject->idx = i;
	itemObject->active = true;
	itemObject->objectType = OBJECT_TYPE_PORTAL;
	itemObject->worldIdx = worldIdx;
	vec_copy(position, itemObject->position);

	
	int usePortalNum = 0;

	itemObject->portal.portalType = 0;

	itemObject->portal.worldIdx[usePortalNum] = targetWorldIdx;
	itemObject->portal.numPerson = numPerson;


	vec_copy( targetPosition, itemObject->portal.position[usePortalNum] );
	itemObject->entityType = ENTITY_OBJECT;

	vec_set(itemObject->angles, 0, 0, 0);

	itemObject->zoneIdx = GTH_Zone_GetZoneIdx(itemObject->position);

	if ( !GTH_Zone_AddEntityList(itemObject->worldIdx, itemObject->zoneIdx, ENTITY_OBJECT, itemObject->idx) )
	{
		return -1;
	}

	if( makerType == ENTITY_PC )
	{
		if( g_pc[makerIdx].organizeServer < 0 )
		{
			itemObject->lootPCIdx = makerIdx;
			itemObject->lootType = LOOTTYPE_INDIVIDUAL;
			itemObject->lootTime = keepTime;
			itemObject->destroyTime = g_globalTime + keepTime;
		}
		else
		{
			itemObject->lootPCIdx = makerIdx;
			itemObject->lootType = LOOTTYPE_PARTY;
			itemObject->lootTime = keepTime;
			itemObject->destroyTime = g_globalTime + keepTime;

			itemObject->organizeServer = g_pc[makerIdx].organizeServer;
			itemObject->organizeTime = g_pc[makerIdx].organizeTime;
			strcpy( itemObject->organizerName, g_pc[makerIdx].organizerName );
		}
	}
	else
	{
		itemObject->lootPCIdx = -1;
		itemObject->lootType = LOOTTYPE_EVERYONE;
		itemObject->lootTime = 0;
		itemObject->destroyTime = g_globalTime + keepTime;
	}
	g_objectNumber++;

	
	GTH_SendMessage_AddItemObject( itemObject );

	return( 1 );
}









































































































void GTH_TransportWorldWithPortal( playerCharacter_t *pc, itemObject_t *itemObject )
{
	int zoneIdx;
	vec3_t pos;
	int worldIdx;
	int memberIdx;
	vec3_t savePosition;
	int saveWorldIdx;

	AI_InitMessage( pc );
	GTH_PC_Calculate( pc );

	pc->event = GTH_EV_CHAR_IDLE;		
	int usePortalNum = 0;
	
	
	
	if ( itemObject->lootType == LOOTTYPE_DANBATTLE_TEAMS )
	{
		LPREQUEST_RECORD	requester = static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetBattleRequestDan());
		LPREQUEST_RECORD	suggester = static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetBattleSuggestDan());
		
		if ( requester != NULL && suggester != NULL )			
		{
			if ( requester->DanIdx == pc->guildIdx )
				usePortalNum = 0;
			else 
				usePortalNum = 1;
		}		
	}	

		
		worldIdx = itemObject->portal.worldIdx[usePortalNum];		
		vec_copy( itemObject->portal.position[usePortalNum], pos );
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	if ( GTH_IsLocalServiceWorld( worldIdx ) )
	{
		if( worldIdx == pc->worldIdx ) 
		{
			vec_copy( pos, pc->position );
			pc->reliableWorldIdx = pc->worldIdx;
			vec_copy( pos, pc->reliablePosition );
			
			zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
			pc->zoneIdx = zoneIdx;
			
			GTH_SendPCEventMessage_Respawn( pc );
			GTH_SendMessage_SyncItemObject( pc );
		}
		else 
		{
			
			GTH_Zone_DeleteEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);
			
			pc->worldIdx = worldIdx;
			vec_copy( pos, pc->reliablePosition );
			pc->reliableWorldIdx = worldIdx;
			
			vec_copy( pos, pc->position );
			
			pc->zoneIdx = GTH_Zone_GetZoneIdx( pc->position );
			GTH_Zone_AddEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);
			GTH_SendPCEventMessage_Respawn( pc );
			pc->ready = false;
			
			
			g_logSystem->Write("User [%s] Move to Other World By Portal. Pos(%.2f, %.2f, %.2f), World : %d", pc->userID, pc->position[0], pc->position[1], pc->position[2], pc->worldIdx);
		}
	}
	else 
	{
		
		saveWorldIdx = pc->worldIdx;
		vec_copy(pc->position, savePosition);
		
		
		pc->worldIdx = worldIdx;
		
		vec_copy( pos, pc->position );
		
		memberIdx = GTH_FindServerWithinGroup( worldIdx );
		if (memberIdx < 0) 
		{
			
			pc->flagChangeServer = -1;
			DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);
			return;
		}
		else
		{
			
			pc->flagChangeServer = 1;
			pc->changeServerWorldIdx = worldIdx;
			DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);
		}
		
		
		pc->worldIdx = saveWorldIdx;
		vec_copy(savePosition, pc->position);
	}
	
}






void GTH_TransportWorldWithNPC( playerCharacter_t *pc, int worldIdx, vec3_t position )
{
	int zoneIdx;
	int memberIdx;
	vec3_t savePosition;
	int saveWorldIdx;

	AI_InitMessage( pc );
	GTH_PC_Calculate( pc );

	pc->event = GTH_EV_CHAR_IDLE;
	
	
	if ( GTH_IsLocalServiceWorld( worldIdx ) )
	{
		if( worldIdx == pc->worldIdx ) 
		{
			vec_copy( position, pc->position );
			pc->reliableWorldIdx = pc->worldIdx;
			vec_copy( position, pc->reliablePosition );

			zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
			pc->zoneIdx = zoneIdx;

			GTH_SendPCEventMessage_Respawn( pc );
			GTH_SendMessage_SyncItemObject( pc );
		}
		else 
		{
			
			GTH_Zone_DeleteEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);

			pc->worldIdx = worldIdx;
			vec_copy( position, pc->reliablePosition );
			pc->reliableWorldIdx = worldIdx;

			vec_copy( position, pc->position );

			pc->zoneIdx = GTH_Zone_GetZoneIdx( pc->position );
			GTH_Zone_AddEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);
			GTH_SendPCEventMessage_Respawn( pc );
			pc->ready = false;

			
			g_logSystem->Write("User [%s] Move to Other World By Portal. Pos(%.2f, %.2f, %.2f), World :%d", pc->userID, pc->position[0], pc->position[1], pc->position[2], pc->worldIdx);
		}
	}
	else 
	{
		
		saveWorldIdx = pc->worldIdx;
		vec_copy(pc->position, savePosition);

		
		pc->worldIdx = worldIdx;

		vec_copy( position, pc->position );

		memberIdx = GTH_FindServerWithinGroup( worldIdx );
		if (memberIdx < 0) 
		{
			
			pc->flagChangeServer = -1;
			DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);
			return;
		}
		else
		{
			
			pc->flagChangeServer = 1;
			pc->changeServerWorldIdx = worldIdx;
			DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);
		}

		
		pc->worldIdx = saveWorldIdx;
		vec_copy(savePosition, pc->position);
	}
	
	return;
}

int GTH_IsEtherealForm( playerCharacter_t *pc )
{
	if( pc->isTransform && pc->transformType == TRANSFORM_ETHEREAL ) return( true );
	return( false );
}

















int GTH_IsValidContributionPoint( monsterCharacter_t *m, playerCharacter_t *pc, int idx )
{
	float dist;

	if( !pc->active || !pc->alive ) return 0;
	if( stricmp( m->conInfo[idx].entityName, pc->name ) ) return 0;
	if( m->worldIdx != pc->worldIdx ) return 0;

	dist = GTH_GetDistance2D( pc->position, m->position );
	if( dist > 2400.0f ) return 0;

	return 1;
}

void GTH_InitContributionPoint( monsterCharacter_t *m )
{
	memset( &m->conInfo, 0, sizeof( monsterContribution_t ) * MAX_NUMBER_OF_MONSTER_CONTRIBUTION );	


	
	for (int i = 0; i < MAX_NUMBER_OF_MONSTER_CONTRIBUTION; i ++)	
		m->conInfo[i].entityIdx = -1;
	

	m->conInfoNumber = 0;


	
	m->maxAtkContributionPoint = 0;
}

int GTH_AddAtkContributionPoint( monsterCharacter_t *m, int entityType, int entityIdx, int point )
{
	
	
	if( m->conInfoNumber == MAX_NUMBER_OF_MONSTER_CONTRIBUTION ) return 0;

	





	
	
	
	



	
	int findidx		= -1;
	int findCount	= 0;
	bool flagfind	= 0;

	for (int i = 0; i < MAX_NUMBER_OF_MONSTER_CONTRIBUTION; i++)
	{		
		if ( m->conInfo[i].entityIdx >= 0 )
		{		
			if( m->conInfo[i].entityType == entityType && m->conInfo[i].entityIdx == entityIdx )
			{
				flagfind = 1;
				findidx = i;
				break;
			}
		}		
		else
		{	
			if ( findidx < 0)
			findidx = i;
		}			
	}

	
	
	
	if ( findidx < 0 || findidx >= MAX_NUMBER_OF_MONSTER_CONTRIBUTION ) return false;

	if ( !flagfind )		
	{		
		if ( findidx < 0 ) return false;;

		m->conInfo[findidx].flag = false;
		m->conInfo[findidx].entityType = entityType;
		m->conInfo[findidx].entityIdx = entityIdx;		
		m->conInfo[findidx].defPoint = 0;
		m->conInfoNumber++;

		if( entityType == ENTITY_PC ) strcpy( m->conInfo[findidx].entityName, g_pc[entityIdx].name );

		
		
		if ( entityType == ENTITY_PC)
		{
			if ( g_pc[entityIdx].targetIdx != m->idx)
				m->conInfo[findidx].ContributionPoints[FIRST_ATTACK_POINT] = m->conInfo[findidx].atkPoint * 1.8f;
		}
		else
		{
			if ( g_monster[entityIdx].targetIdx != m->idx)
				m->conInfo[findidx].ContributionPoints[FIRST_ATTACK_POINT] = m->conInfo[findidx].atkPoint * 1.8f;
		}		

	}			
	
	m->conInfo[findidx].atkPoint += point;		
	m->conInfo[findidx].time = g_globalTime;	
	
	

	
	
	
	
	if ( m->message[findidx].data[MSG_ATKDATA_ETC_DATA_2] == MSGINFO_TYPE_ATTACK )
		m->conInfo[findidx].ContributionPoints[MYSTERY_POINT] +=  m->message[m->messageNumber].data[MSG_ATKDATA_PHYSIC_DMG];
	else
	{	
		m->conInfo[findidx].ContributionPoints[ATTACK_POINT] += point;
	} 
	
	m->conInfo[findidx].ContributionPoints[TOTAL_POINT] += m->conInfo[findidx].ContributionPoints[FIRST_ATTACK_POINT];
	m->conInfo[findidx].ContributionPoints[TOTAL_POINT] += m->conInfo[findidx].ContributionPoints[FIRST_ATTACK_REVERSE_POINT];
	m->conInfo[findidx].ContributionPoints[TOTAL_POINT] += m->conInfo[findidx].ContributionPoints[MYSTERY_POINT];
	m->conInfo[findidx].ContributionPoints[TOTAL_POINT] += m->conInfo[findidx].ContributionPoints[RECOVER_POINT];	
	m->conInfo[findidx].ContributionPoints[TOTAL_POINT] += m->conInfo[findidx].ContributionPoints[ATTACK_POINT];	
	m->maxAtkContributionPoint += point;
	

	return 1;
}

int GTH_AddDefContributionPoint( monsterCharacter_t *m, int entityType, int entityIdx, int point )
{
	int i;

	
	if( m->conInfoNumber == MAX_NUMBER_OF_MONSTER_CONTRIBUTION ) return 0;

	
	
	bool flagfind = 0;
	int findcount = 0;
	int findidx = -1;
	
	
	for( i = 0; i < MAX_NUMBER_OF_MONSTER_CONTRIBUTION; i ++ )	
	{
		if ( m->conInfo[i].entityIdx >= 0)
		{			
			if( m->conInfo[i].entityType == entityType && m->conInfo[i].entityIdx == entityIdx ) 
			{
				
				flagfind = 1;
				findidx = i;
				break;
			}
			
		}
		else
		{
			if ( findidx < 0)
				findidx = i;
		}				
	}

	if ( findidx < 0 || findidx >= MAX_NUMBER_OF_MONSTER_CONTRIBUTION) 
		return 0;

	
	if( !flagfind ) 
	{
		m->conInfo[findidx].flag = false;
		m->conInfo[findidx].entityType = entityType;
		m->conInfo[findidx].entityIdx = entityIdx;
		m->conInfo[findidx].atkPoint = 0;
		m->conInfo[findidx].defPoint = point;
		if( entityType == ENTITY_PC ) strcpy( m->conInfo[findidx].entityName, g_pc[entityIdx].name );
		m->conInfo[findidx].time = g_globalTime;		
		
		
		
		if ( g_monster[m->idx].idx != g_pc[entityIdx].targetIdx )
		{	
			if ( m->conInfo[findidx].ContributionPoints[FIRST_ATTACK_REVERSE_POINT] == 0)
			m->conInfo[findidx].ContributionPoints[FIRST_ATTACK_REVERSE_POINT] += 	
				point * 0.5;	
		}
		
		m->conInfoNumber ++;
	}
	else
	{
		m->conInfo[findidx].defPoint += point;
		m->conInfo[findidx].time = g_globalTime;
	}
	
	return 1;
}





float GTH_ConvertAttackDelay( float currAtkDelay, float atkSpdRate )
{
	float velocity = 1.0f / currAtkDelay;
	float newAtkDelay;
	velocity = velocity * ( 100.0f - atkSpdRate ) * 0.01f;
	newAtkDelay = 1.0f / velocity;

	return( newAtkDelay );
}

int GTH_GetNextLevel( __int64& exp )
{
	int i;
	for( i = 0; i < MAX_NUMBER_OF_LEVEL; i ++ )
	{
		if( exp < g_expTable[i].totalexp )	return( i+1 );
	}
	exp =  g_expTable[MAX_NUMBER_OF_LEVEL-1].totalexp - 1;
	return( MAX_NUMBER_OF_LEVEL );
}


int GTH_GetNextGenLevel( __int64& exp )
{
	int i;
	for( i = 0; i < MAX_NUMBER_OF_LEVEL/2 ; i ++ )
	{
		if( exp < g_expTable[i].totalGenExp ) return( i+1 );
	}
	exp =  g_expTable[(MAX_NUMBER_OF_LEVEL/2)-1].totalGenExp - 1;
	return( MAX_NUMBER_OF_LEVEL/2 );
}

int GTH_CheckJob( playerCharacter_t *pc, int j0, int j1, int j2, int j3 )
{
	int reqBeforeAT, req1st, req2nd, req3rd;

	reqBeforeAT = 0;
	req1st = 0;
	req2nd = 0;
	req3rd = 0;
	
	switch( pc->pcJob )
	{
	case J_MENTALITY :
		reqBeforeAT |= AT_MENTALITY;
		break;
	case J_WIND :
		reqBeforeAT |= AT_WIND;
		break;
	case J_WATER :
		reqBeforeAT |= AT_WATER;
		break;
	case J_EARTH:
		reqBeforeAT |= AT_EARTH;
		break;
	case J_DEVOTIONAL_KNIGHT :
		reqBeforeAT |= AT_MENTALITY;
		req1st |= AT_DEVOTIONAL_KNIGHT;
		break;
	case J_ROMING_KNIGHT :
		reqBeforeAT |= AT_MENTALITY;
		req1st |= AT_ROMING_KNIGHT;
		break;
	case J_PIECING_EYES :
		reqBeforeAT |= AT_WIND;
		req1st |= AT_PIECING_EYES;
		break;
	case J_WINDY_ROMER :
		reqBeforeAT |= AT_WIND;
		req1st |= AT_WINDY_ROMER;
		break;
	case J_DESTROYER :
		reqBeforeAT |= AT_WATER;
		req1st |= AT_DESTROYER;
		break;
	case J_GUARDIAN :
		reqBeforeAT |= AT_WATER;
		req1st |= AT_GUARDIAN;
		break;
	case J_PROPHECIER :
		reqBeforeAT |= AT_WATER;
		req1st |= AT_PROPHECIER;
		break;
	case J_TRANSFORMER :
		reqBeforeAT |= AT_EARTH;
		req1st |= AT_TRANSFORMER;
		break;
	case J_DEFENDER :
		reqBeforeAT |= AT_EARTH;
		req1st |= AT_DEFENDER;
		break;
	case J_STRANGER :
		reqBeforeAT |= AT_EARTH;
		req1st |= AT_STRANGER;
		break;
	}
	if( j0 != 0 )
	{
		if( j0 & reqBeforeAT ) return 1;
	}
	if( j1 != 0 )
	{
		if( j1 & req1st ) return 1;
	}
	if( j2 != 0 )
	{
		if( j2 & req2nd ) return 1;
	}
	if( j3 != 0 )
	{
		if( j3 & req3rd ) return 1;
	}
	
	return 0;
}

//lucky 2012 ( weapon in ring slots patch #1 )
bool GTH_CheckWearPosion( playerCharacter_t *pc )
{
	int i, itemIdx;
	item_t *item;		
	itemTable_t *pItem;
	bool bRet = true;
	for( i = EQUIP_WEAPON_1; i < MAX_EQUIPMENT; i ++ )
	{
		if( pc->equipment[i] < 0 ) continue;
		itemIdx	= pc->equipment[i];
		item	=	&pc->item[itemIdx];
		pItem = &g_itemTable[item->itemTableIdx];
		if( i == EQUIP_WEAPON_1 || i == EQUIP_WEAPON_2 )
		{ 
			if( pItem->wearPosition == ITEM_WEAR_POS_WEAPON )
			{
				continue;
			}
		}

		if ( pItem->wearPosition == i - 1 )
		{
			continue;
		}
		memset( &pc->item[itemIdx], 0, sizeof( item_t ) );
		pc->equipment[i] = -1;
		bRet = false;
	}
	return bRet;

}
//end

int GTH_CheckRequire( playerCharacter_t *pc, int requireType, void* pReq )
{
	int level, genLevel, force, soul, agi, vit;
	int notReq;

	level = pc->level;
	genLevel = pc->genLevel;
	force = pc->calForce;
	soul = pc->calSoulpower;
	agi = pc->calAgility;
	vit = pc->calVitality;

	switch( requireType )
	{
		
	case 0 :
		{
			item_t *item = static_cast<item_t*>(pReq);
			itemTable_t *pItem;
			pItem = &g_itemTable[item->itemTableIdx];
			//lucky 2012 Blank sword not allowed exept for GM.
			if ((item->itemTableIdx <=0) && (pc->gameMaster<1))
			{
				return 0;
			}
			//end

			if( pItem->wearPosition == ITEM_WEAR_POS_NONE ) return 1;

			
			notReq = false;

			
			if( !GTH_CheckJob( pc, pItem->reqBeforeAT, pItem->req1stAT, pItem->req2ndAT, pItem->req3rdAT) ) return 0;
			if( GTH_IsRepairItem( item ) && item->durability <= 0 ) return 0;

			if( !notReq )
			{
				if( pItem->reqForce > force ) return 0;
				if( pItem->reqSoulpower > soul ) return 0;
				if( pItem->reqAgility > agi ) return 0;
				if( pItem->reqVitality > vit ) return 0;
				if( item->reqLevel > level ) return 0;
			}

			return 1;
		}
	case 1 :
		{
			int skillTableIdx = (* static_cast<int *>(pReq) );
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
			if(pSkillTable==NULL)
			{
				TRACE("Error! pSkillTable==NULL   \n");
				return 0;
			}
			
			
			if( !GTH_CheckJob( pc->pcJob, pSkillTable->m_nJobType ) ) return 0;
			if( genLevel < pSkillTable->GetSkillRequire()->m_nGenLevel ) return 0;




			return 1;
		}
		break;
	}
	return 0;
}

int GTH_GetEquipPos( item_t *item, int equipPos )
{
	int itemTableIdx;
	int tableEquipPos;

	itemTableIdx = item->itemTableIdx;
	tableEquipPos = g_itemTable[itemTableIdx].wearPosition;

	if( tableEquipPos == ITEM_WEAR_POS_WEAPON )
	{
		if( equipPos == EQUIP_WEAPON_1 || equipPos == EQUIP_WEAPON_2 ) return( equipPos );
		return( EQUIP_WEAPON_1 );
	}
	else if( tableEquipPos >= ITEM_WEAR_POS_UPPER && tableEquipPos <= ITEM_WEAR_POS_EPACK )
	{
		return( tableEquipPos + 1 );
	}
	else return( -1 );
}

int GTH_GetGenTypeFromJob( int pcJob )
{	
	if( pcJob == J_MENTALITY || pcJob == J_DEVOTIONAL_KNIGHT || 
		pcJob == J_ROMING_KNIGHT ) return( 0 );
	
	if( pcJob == J_WIND || pcJob == J_PIECING_EYES || 
		pcJob == J_WINDY_ROMER ) return( 1 );

	if( pcJob == J_WATER || pcJob == J_DESTROYER || 
		pcJob == J_GUARDIAN || pcJob == J_PROPHECIER ) return( 2 );

	if( pcJob == J_EARTH || pcJob == J_TRANSFORMER || 
		pcJob == J_DEFENDER || pcJob == J_STRANGER ) return( 3 );

	return( -1 );
}

int GTH_CheckJob( int job, int reqJob )
{
	
	if( reqJob == J_MENTALITY )
	{
		if( job == J_MENTALITY || job == J_DEVOTIONAL_KNIGHT || 
			job == J_ROMING_KNIGHT ) return 1;
	}
	if( reqJob == J_WIND )
	{
		if( job == J_WIND || job == J_PIECING_EYES ||
			job == J_WINDY_ROMER ) return 1;
	}
	if( reqJob == J_WATER )
	{
		if( job == J_WATER || job == J_DESTROYER || 
			job == J_GUARDIAN || job == J_PROPHECIER ) return 1;
	}
	if( reqJob == J_EARTH )
	{
		if( job == J_EARTH || job == J_TRANSFORMER || 
			job == J_DEFENDER || job == J_STRANGER ) return 1;
	}
	
	if( reqJob == job ) return 1;

	return 0;
}

int GTH_IsRepairItem( item_t *item )
{
	int itemTableIdx = item->itemTableIdx;
	if( !g_itemTable[itemTableIdx].durYn ) return 0;
	if( !g_itemTable[itemTableIdx].repairYn ) return 0;
	if( g_itemTable[itemTableIdx].durability == 0 ) return 0;
	if( item->durability == g_itemTable[itemTableIdx].durability ) return 0;

	return 1;
}
int GTH_GetAllRepairCost( playerCharacter_t* pc, int curSE )
{
	int i, itemIdx, dur, cost = 0;
	int itemTableIdx, itemType; 
	item_t *item;

	for( i = EQUIP_WEAPON_1; i < EQUIP_EPACK; i ++ )
	{
		itemIdx = pc->equipment[i];
		if( itemIdx < 0 ) continue;
		item = &pc->item[itemIdx];
		itemTableIdx = item->itemTableIdx;
		itemType = GetItemType( itemTableIdx );

		if( item->durability >= g_itemTable[itemTableIdx].durability ) continue;
		if( g_itemTable[itemTableIdx].durability <= 0 ) continue;
		
		if( !( itemType == 0 || itemType == 1 ) ) continue;
		
		cost = GTH_GetRepairCost( item, curSE, dur );
		
		if( curSE < cost || dur == 0 ) continue;
		curSE -= cost;
	}
	return( pc->curChargeSE - curSE );
}

int GTH_GetRepairCost( item_t* item, int curSE, int &checkDur )
{
	int itemTableIdx;
	int cost, dur, decDur, totalPrice;
	float a;
	itemTableIdx = item->itemTableIdx;
	cost = g_itemTable[itemTableIdx].sellCost;
	cost = GTH_GetBuyPrice( item );
	cost = (int)( (float)cost * 0.1f );

	dur = g_itemTable[itemTableIdx].durability;

	if( item->durability > dur ) item->durability = dur;

	decDur = dur - item->durability;

	
	a = (float)cost / (float)( dur * dur );

	
	totalPrice = (int)( (float)( decDur * decDur ) * a );

	if( totalPrice == 0 ) totalPrice = 1;

	if( curSE < 0 ) return( totalPrice );

	
	if( curSE < totalPrice )
	{
		
		checkDur = (int)sqrtf( (float)curSE / a );
		totalPrice = curSE;
	}
	else checkDur = decDur;

	return( totalPrice );
}

int GTH_GetBuyPrice( item_t* item )
{
	int cost, realCost;
 
	if( GTH_IsStackItem( item ) )
	{
		cost = g_itemTable[item->itemTableIdx].sellCost;
		cost *= ( item->durability + 1 );
		return( cost );
	}

	
	switch( item->itemTableIdx )
	{
	case QUESTSCROLL_ITEM_INDEX:
		realCost = (int)( 2.5f * (float) g_questscrollTable[item->optionIdx[0]].data.price );
		break;
	default:
		{
			realCost = cost = g_itemTable[item->itemTableIdx].sellCost;
			for( int i = 0; i < item->optionNumber; i ++ )
			{
				if( item->optionIdx[i] < 0 ) continue;
				if( item->optionIdx[i] >= MAX_NUMBER_OF_ITEM_OPTION_TABLE ) continue;
				realCost += (int)( (float)cost * 0.1f );
			}
		}
		break;
	}

	if( realCost < 0 ) realCost = 0;
	return( realCost );
}

int GTH_GetSellPrice( item_t* item )
{
	float a, cost;
	int dur, sellcost;
	dur = g_itemTable[item->itemTableIdx].durability;
	cost = (float)GTH_GetBuyPrice( item );
	if( dur <= 0 ) a = cost;
	else a = cost - (float)GTH_GetRepairCost( item, -1, dur );
	a *= 0.3f;
	sellcost = (int)a;
	if( sellcost <= 0 ) sellcost = 0;
	return( sellcost );
}


int GTH_GetDepotPrice( playerCharacter_t *pc, item_t *item )
{
	
	return( 300 );
}

int GTH_IsConsumptiveItem( item_t* item )
{
	int itemTableIdx, classIdx;
	itemTableIdx = item->itemTableIdx;
	classIdx = g_itemTable[itemTableIdx].classIdx;

	if( !g_itemClassTable[classIdx].etcAtt ) return 0;

	if( !g_itemClassTable[classIdx].etcAtt ) return 0;
	if( g_itemTable[itemTableIdx].useFlag ) return 1;

	return 0;
}


int GTH_IsStackItem( item_t* item )
{
	int itemTableIdx;
	itemTableIdx = item->itemTableIdx;

	if( g_itemTable[itemTableIdx].stackFlag ) return 1;

	return 0;
}

int GTH_CheckStackItem( item_t* item1, item_t* item2 )
{
	if( !GTH_IsStackItem( item1 ) ) return( 0 );
	if( !GTH_IsStackItem( item2 ) ) return( 0 );
	if( item1->itemTableIdx == item2->itemTableIdx ) return( 1 );
	return( 0 );
}

float GTH_GetWeaponTypeBonusPhyDamage( playerCharacter_t *pc, item_t *item )
{
	int classIdx;
	float bonus = 0.0f;
	float force, soul, agi, vit;

	force = (float)pc->calForce;
	soul = (float)pc->calSoulpower;
	agi = (float)pc->calAgility;
	vit = (float)pc->calVitality;

	classIdx = g_itemTable[item->itemTableIdx].classIdx;
	switch( g_itemClassTable[classIdx].weaponType )
	{
	case WEAPON_TYPE_DANGGER :
		bonus += force * 0.4f;
		bonus += soul * 0.0f;
		bonus += agi * 0.4f;
		bonus += vit * 0.0f;
		break;
	case WEAPON_TYPE_ONEHAND_SWORD :
		bonus += force * 0.6f;
		bonus += soul * 0.0f;
		bonus += agi * 0.2f;
		bonus += vit * 0.0f;
		break;
	case WEAPON_TYPE_TWOHAND_SWORD :
		bonus += force * 0.8f;
		bonus += soul * 0.0f;
		bonus += agi * 0.0f;
		bonus += vit * 0.0f;
		break;
	case WEAPON_TYPE_MAGIC_BOW :
		bonus += force * 0.5f;
		bonus += soul * 0.0f;
		bonus += agi * 0.3f;
		bonus += vit * 0.0f;
		break;
	case WEAPON_TYPE_STAFF :
		bonus += force * 0.5f;
		bonus += soul * 0.3f;
		bonus += agi * 0.0f;
		bonus += vit * 0.0f;
		break;
	case WEAPON_TYPE_SPIRITOR :
		bonus += force * 0.5f;
		bonus += soul * 0.0f;
		bonus += agi * 0.0f;
		bonus += vit * 0.3f;
		break;
	case WEAPON_TYPE_CLAW :
		bonus += force * 0.6f;
		bonus += soul * 0.0f;
		bonus += agi * 0.0f;
		bonus += vit * 0.2f;
		break;
	}
	return( bonus * 0.01f );
}

float GTH_GetWeaponTypeBonusMagDamage( playerCharacter_t *pc, item_t *item )
{
	float bonus = 0.0f;
	float force, soul, agi, vit;

	force = (float)pc->calForce;
	soul = (float)pc->calSoulpower;
	agi = (float)pc->calAgility;
	vit = (float)pc->calVitality;

	bonus += soul * 1.3f;

	return( bonus * 0.01f );
}



int GTH_SetTrainMonster( playerCharacter_t *pc, monsterCharacter_t *mon )
{
	int oldestTrainMonsterIdx;
	int oldestTrainMonsterTime;

	
	for( int i = 0; i < MAX_COUNT_TRAIN_MONSTER; i ++ )
	{
		if( pc->trainMonsterIdx[i] < 0 ) continue;
		if( pc->trainMonsterIdx[i] == mon->idx ) break;
	}

	
	if( i == MAX_COUNT_TRAIN_MONSTER )
	{
		
		for( i = 0; i < MAX_COUNT_TRAIN_MONSTER; i ++ )
		{
			if( pc->trainMonsterIdx[i] < 0 ) break;
		}

		
		if( i == MAX_COUNT_TRAIN_MONSTER ) 
		{
			oldestTrainMonsterIdx = 0;
			oldestTrainMonsterTime = pc->trainMonsterTime[0];

			
			for( i = 0; i < MAX_COUNT_TRAIN_MONSTER; i ++ )
			{
				if( pc->trainMonsterTime[i] < oldestTrainMonsterTime )
				{
					oldestTrainMonsterIdx = i;
					oldestTrainMonsterTime = pc->trainMonsterTime[i];
				}
			}
			
			i = oldestTrainMonsterIdx;

			
			AI_RemoveTarget( &g_monster[pc->trainMonsterIdx[i]] );
		}

		
		pc->trainMonsterIdx[i] = mon->idx;
		pc->trainMonsterTime[i] = g_globalTime;
	}

	return 1;
}

void GTH_UpdateTrainMonster( playerCharacter_t *pc )
{
	pc->numTrainMonster = 0;

	for( int i = 0; i < MAX_COUNT_TRAIN_MONSTER; i ++ )
	{
		if( pc->trainMonsterIdx[i] < 0 ) continue;
		
		if( !g_monster[pc->trainMonsterIdx[i]].active || g_monster[pc->trainMonsterIdx[i]].aliveState != MON_ALIVE_STATE_NORMAL )
		{
			pc->trainMonsterIdx[i] = -1;
		}
		else
		{
			
			if( g_monster[pc->trainMonsterIdx[i]].targetType == ENTITY_PC && g_monster[pc->trainMonsterIdx[i]].targetIdx == pc->idx )
			{
				pc->numTrainMonster ++;
			}
			else
			{
				pc->trainMonsterIdx[i] = -1;
			}
		}
	}
}

void GTH_DeleteTrainMonster( playerCharacter_t *pc, monsterCharacter_t *mon )
{
	for( int i = 0; i < MAX_COUNT_TRAIN_MONSTER; i ++ )
	{
		if( pc->trainMonsterIdx[i] < 0 ) continue;
		if( pc->trainMonsterIdx[i] == mon->idx ) break;
	}
	
	if( i == MAX_COUNT_TRAIN_MONSTER ) return;
	pc->trainMonsterIdx[i] = -1;
	pc->numTrainMonster --;
}

int LoadExpTableForServer( char* filename )
{
	int size;
	byte *buffer;
	char* token;
	int level;
	int expLevel=0;

	FILE *fp;

	fp = fopen( filename, "rb" );

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	SetTokenBuffer( buffer, size );

	for( int i = 0; i < MAX_NUMBER_OF_LEVEL; i ++ )
		memset( &g_expTable[i], 0, sizeof( expTable_t ) );

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		level = atoi( token );

		token = NextToken();		
		g_expTable[expLevel].totalexp = _atoi64( token );
		token = NextToken();
		g_expTable[expLevel].exp = _atoi64( token );

		expLevel++;
	}

	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return( 1 );
}

int LoadGenExpTableForServer( char* filename )
{
	int size;
	byte *buffer;
	char* token;
	int level;
	int genExpLevel=0;

	FILE *fp;

	fp = fopen( filename, "rb" );

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	SetTokenBuffer( buffer, size );

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		level = atoi( token );

		token = NextToken();
		g_expTable[genExpLevel].totalGenExp = _atoi64( token );
		token = NextToken();
		g_expTable[genExpLevel].genExp = _atoi64( token );

		genExpLevel++;
	}
	
	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return( 1 );
}


int LoadStoreTableForServer( char* filename )
{
	int size;
	byte *buffer;
	char* token;

	FILE *fp;

	fp = fopen( filename, "rb" );

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	SetTokenBuffer( buffer, size );

	for( int i = 0; i < MAX_STORES; i ++ )
		memset( &g_store[i], 0, sizeof( storeStruct_t ) );

	int idx = 0, page;
	int *pNumDefault, *pNumGenerate, *pNumSkill;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		if( !stricmp( token, "STORE_IDX" ) )
		{
			token = NextToken();
			idx = atoi( token );
		}
		else if( token[0] == '{' )
		{
			while ( 1 )
			{
				token = NextToken();
				if( token[0] == '}' ) break;
				else if( !stricmp( token, "CLOSE_TIME" ) )
				{
					token = NextToken();
					g_store[idx].closeTime = atoi( token );
				}
				else if( !stricmp( token, "READY_TIME" ) )
				{
					token = NextToken();
					g_store[idx].readyTime = atoi( token );
				}
				else if( !stricmp( token, "OPEN_TIME" ) )
				{
					token = NextToken();
					g_store[idx].openTime = atoi( token );
				}
				else if( !stricmp( token, "EPACK_CHARGE_RA_COST" ) )
				{
					token = NextToken();
					g_store[idx].chargeRACost = atof( token );
				} 
				else if( !stricmp( token, "EPACK_CHARGE_SA_COST" ) )
				{
					token = NextToken();
					g_store[idx].chargeSACost = atof( token );
				}
				else if( !stricmp( token, "PAGE_0" ) )
				{
					page = 0;
					pNumDefault = &g_store[idx].defaultItem[0].defaultNumber;
					pNumGenerate = &g_store[idx].generateItem[0].generateIdxNumber;
					pNumSkill = &g_store[idx].skill[0].skillIdxNumber;
				} 
				else if( !stricmp( token, "PAGE_1" ) )
				{
					page = 1;
					pNumDefault = &g_store[idx].defaultItem[1].defaultNumber;
					pNumGenerate = &g_store[idx].generateItem[1].generateIdxNumber;
					pNumSkill = &g_store[idx].skill[1].skillIdxNumber;
				} 
				else if( !stricmp( token, "PAGE_2" ) )
				{
					page = 2;
					pNumDefault = &g_store[idx].defaultItem[2].defaultNumber;
					pNumGenerate = &g_store[idx].generateItem[2].generateIdxNumber;
					pNumSkill = &g_store[idx].skill[2].skillIdxNumber;
				} 
				else if( token[0] == '{' )
				{
					while ( 1 )
					{
						token = NextToken();
						if( token[0] == '}' ) break;
						else if( !stricmp( token, "DEFAULT_ITEM_INDEX" ) )
						{
							token = NextToken();
							g_store[idx].defaultItem[page].itemIndex[*pNumDefault] = atoi( token );
							token = NextToken();
							g_store[idx].defaultItem[page].itemNumber[*pNumDefault] = atoi( token );
							(*pNumDefault) ++;
						} 
						else if( !stricmp( token, "GENERATE_INDEX" ) )
						{
							token = NextToken();
							g_store[idx].generateItem[page].generateIdx[*pNumGenerate] = atoi( token );
							token = NextToken();
							g_store[idx].generateItem[page].generateNumber[*pNumGenerate] = atoi( token );
							token = NextToken();
							g_store[idx].generateItem[page].generateLevel[*pNumGenerate] = atoi( token );
							(*pNumGenerate) ++;
						}
						else if( !stricmp( token, "STORE_SKILL_INDEX" ) )
						{
							token = NextToken();
							g_store[idx].skill[page].skillIndex[*pNumSkill] = atoi( token );
							(*pNumSkill) ++;
						}
						else if( !stricmp( token, "CHARGE_EPACK_RA" ) )
						{
							g_store[idx].defaultItem[page].itemIndex[*pNumDefault] = STORE_ITEM_INDEX_EPACK_RA;
							g_store[idx].defaultItem[page].itemNumber[*pNumDefault] = -1;
							(*pNumDefault) ++;
						}
						else if( !stricmp( token, "CHARGE_EPACK_SA" ) )
						{
							g_store[idx].defaultItem[page].itemIndex[*pNumDefault] = STORE_ITEM_INDEX_EPACK_SA;
							g_store[idx].defaultItem[page].itemNumber[*pNumDefault] = -1;
							(*pNumDefault) ++;
						}	
					}
				}	
			}
		}
	}
	for( i = 0; i < MAX_STORES; i ++ )
	{
		GTH_OpenStore( i );
	}

	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	
	return 1;
}

int LoadBindingTableForServer( char* filename )
{
	int size;
	byte *buffer;
	char* token;
	int idx;

	FILE *fp;

	fp = fopen( filename, "rb" );

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	SetTokenBuffer( buffer, size );

	for( int i = 0; i < MAX_BINDINGS; i ++ )
	{
		memset( &g_binding[i], 0, sizeof( bindingStruct_t ) );
		g_binding[i].idx = -1;
	}

	g_bindingNumber = 0;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;

		if( !stricmp( token, "BINDING_IDX" ) )
		{
			token = NextToken();
			idx = atoi( token );
			g_binding[idx].idx = idx;
			if( idx >= g_bindingNumber ) g_bindingNumber = idx + 1;
		} else if( !stricmp( token, "BINDING_WORLD" ) )
		{
			token = NextToken();
			g_binding[idx].worldIdx = atoi( token );
		} else if( !stricmp( token, "BINDING_POSITION" ) )
		{
			token = NextToken();
			g_binding[idx].pos[0] = atof( token ); 
			token = NextToken();
			g_binding[idx].pos[1] = atof( token ); 
			token = NextToken();
			g_binding[idx].pos[2] = atof( token ); 
		} else if( !stricmp( token, "BINDING_RANGE" ) )
		{
			token = NextToken();
			g_binding[idx].range = atof( token ); 
		} else if( !stricmp( token, "BINDING_COST" ) )
		{
			token = NextToken();
			g_binding[idx].cost = atoi( token );
		}
	}

	
	if(buffer!=NULL)
	{
		delete []buffer;
		buffer=NULL;
	}
	return( 1 );
}

int LoadScriptInfoTableForServer( char* filename )
{
	int size;
	byte *buffer;
	char* token;
	int type, idx;

	FILE *fp;

	fp = fopen( filename, "rb" );

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	SetTokenBuffer( buffer, size );

	memset( &g_scriptInfo, 0, sizeof( scriptInfoStruct_t ) );

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;

		if( !stricmp( token, "AI_SCRIPT" ) )
		{
			type = 0;
		} 
		else if( !stricmp( token, "NPC_SCRIPT" ) )
		{
			type = 1;
			
		} 
		else if( token[0] == '{' )
		{
			while ( 1 )
			{
				token = NextToken();
				if( token[0] == '}' ) break;

				idx = atoi( token );
				token = NextToken();
				
				if( type == 0 )
				{
					g_scriptInfo.aiScript[idx].idx = idx;
					strcpy( g_scriptInfo.aiScript[idx].filename, token );
					g_scriptInfo.numAiScript ++;
				}
				else
				{
					g_scriptInfo.npcScript[idx].idx = idx;
					strcpy( g_scriptInfo.npcScript[idx].filename, token );
					g_scriptInfo.numNpcScript ++;
				}
			}
		}
	}

	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return( 1 );
}

int LoadGenCapabilityTable( char *filename )
{
	int size;
	byte* buffer=NULL;
	char* token=NULL;
	int idx, level, genCapability, limitGenLevel, type;

	FILE* fp=NULL;

	fp = fopen( filename, "rb" );
	if(NULL == fp) return 0;

	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	buffer = new byte[size+1];
	if(NULL ==  buffer) return 0;

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	if(NULL != fp){ fclose(fp); fp=NULL; }

	SetTokenBuffer( buffer, size );

	for( int i = 0; i < 100; i ++ )
		memset( &g_skillGradeClassTable[i], 0, sizeof( skillGradeClass_t ) );
	
	memset( &g_genCapabilityTable, 0, sizeof( genCapabilityTable_t ) );

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;

		if( !stricmp( token, "SKILL_CLASS" ) )
		{
			token = NextToken();
			idx = atoi( token );
			type = 0;

			if(idx >= 100){
				MessageBox(g_hWnd,"g_skillGradeClassTable idx overflow!","Error!", MB_OK);
				return 0;
			}
		} 
		else if( !stricmp( token, "MONSTER_GEN_CAPABILITY" ) )
		{
			type = 1;
		}
		else if( token[0] == '{' )
		{
			while ( 1 )
			{
				token = NextToken();
				if( token[0] == '}' ) break;

				level = atoi( token );
				


				token = NextToken();
				
				if( type == 0 )
				{

					
					
					if(level >= 10){
						MessageBox(
							g_hWnd,
							"Gen Capability Table Data Loading Failure!\n Level Maximum 10 overflow found!",
							"Error!", MB_OK);
						return 0;
					}
					genCapability = atoi( token );
					token = NextToken();
					limitGenLevel = atoi( token );
					g_skillGradeClassTable[idx].genCapability[level] = genCapability;
					g_skillGradeClassTable[idx].limitGenLevel[level] = limitGenLevel;
				}
				else
				{
					
					
					if(level >= 100){
						MessageBox(
							g_hWnd,
							"Gen Capability Table Data Loading Failure!\n Level Maximum 100 overflow found!",
							"Error!", MB_OK);
						return 0;
					}

					g_genCapabilityTable.genCapability[level] = atoi( token );

					
				}
			}
		}
	}

	
	if(NULL != buffer){ delete [] buffer; buffer=NULL; }
	return( 1 );
}




int LoadItemCraftTableForServer( char* filename )
{
	int size;
	byte *buffer;
	char* token;
	int numSourceItem, numRequireSkill, numResultItem, numOptionIdx;

	FILE *fp;

	fp = fopen( filename, "rb" );

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	SetTokenBuffer( buffer, size );

	for( int i = 0; i < MAX_NUMBER_OF_ITEM_CRAFT_TABLE; i ++ )
		memset( &g_itemCraftTable[i], 0, sizeof( itemCraftTable_t ) );

	g_itemCraftTableNumber = 0;
	
	itemCraftTable_t		TempItemCraftTable;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;

		memset(&TempItemCraftTable,0,sizeof(itemCraftTable_t));

		
		TempItemCraftTable.m_nMembership_type=-1;

		if( token[0] == '{' )
		{
			while ( 1 )
			{
				token = NextToken();
				if( token[0] == '}' ) break;
				if( token[0] == NULL ) break;

				if( !stricmp( token, "craft_grade" ) )
				{
					token = NextToken();
					TempItemCraftTable.craftGrade = atoi( token );
				}
				else if( !stricmp( token, "success_probability" ) )
				{
					token = NextToken();
					TempItemCraftTable.successProbability = atof( token );
				}
				else if( !stricmp( token, "craft_cost" ) )
				{
					token = NextToken();
					TempItemCraftTable.craftCost = atoi( token );
				}
				
				else if( !stricmp( token, "craft_special_type" ) )
				{
					token = NextToken();
					TempItemCraftTable.specialType = atoi( token );
				}

				
				else if( !stricmp( token, "craft_event_index" ) )
				{
					token = NextToken();
					TempItemCraftTable.m_lEventIndex = atoi( token );
				}

				
				else if( !stricmp( token, "event_type" ) )
				{
					token = NextToken();
					TempItemCraftTable.m_btEventType = atoi( token );
				}

				
				else if( !stricmp( token, "craft_cheat_name" ) )
				{
					token = NextToken();
					int len = strlen(token);
					if(len > 20)
					{
						wsprintf(TempItemCraftTable.m_chCheatName,"NULL");
					}
					else
					{
						
						
						wsprintf(TempItemCraftTable.m_chCheatName,"%s",token);
					}
				}

				
#ifdef _GTH_KOREAN_VERSION
				else if( !stricmp( token, "korea" ) )
				{
					token = NextToken(  );		
					while( 1 )
					{
						token = NextToken();		
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							while( 1 )
							{
								token = NextToken(  );
								if( token[0] == '}' ) break;

								if( !stricmp( token, "membership_type" ) )
								{
									token = NextToken(  );
									TempItemCraftTable.m_nMembership_type = atoi( token );
								}
							}
						}
					}
				}
#endif
				
				else if( !stricmp( token, "result_etc" ) )
				{
					token = NextToken(  );		
					while( 1 )
					{
						token = NextToken();	
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							while( 1 )
							{
								token = NextToken(  );
								if( token[0] == '}' ) break;

								if( !stricmp( token, "nak" ) )
								{
									token = NextToken(  );
									TempItemCraftTable.m_lResultNak = atoi( token );
								}
								else if( !stricmp( token, "exp" ) )
								{
									token = NextToken(  );
									TempItemCraftTable.m_lResultExp = atoi( token );
								}
								else if( !stricmp( token, "icon" ) )
								{
									token = NextToken(  );
									TempItemCraftTable.m_nResultNakMark = atoi( token );
								}							
							}
						}
					}
				}				
				
				else if( !stricmp( token, "source_item" ) )
				{
					numSourceItem = 0;
					token = NextToken();		
					while( 1 )
					{
						token = NextToken();
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							numOptionIdx = 0;
							while( 1 )
							{
								token = NextToken();
								if( token[0] == '}' ) break;

								if( !stricmp( token, "index" ) )
								{
									token = NextToken();
									TempItemCraftTable.sourceItem[numSourceItem].itemIndex = atoi( token );
								}
								else if( !stricmp( token, "number" ) )
								{
									token = NextToken();
									TempItemCraftTable.sourceItem[numSourceItem].itemCount = atoi( token );
								}
								else if( !stricmp( token, "option_index" ) )
								{
									token = NextToken();
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionIndex[numOptionIdx] = atoi( token );
									numOptionIdx ++;
								}
								else if( !stricmp( token, "above_option_number" ) )
								{
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCountType = 0;
									token = NextToken();
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCount = atoi( token );
								}
								else if( !stricmp( token, "equal_option_number" ) )
								{
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCountType = 1;
									token = NextToken();
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCount = atoi( token );
								}
								else if( !stricmp( token, "below_option_number" ) )
								{
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCountType = 2;
									token = NextToken();
									TempItemCraftTable.sourceItem[numSourceItem].reqOptionCount = atoi( token );
								}
							}
							TempItemCraftTable.sourceItem[numSourceItem].numReqOptionIndex = numOptionIdx;
							numSourceItem ++;
						}
					}
					TempItemCraftTable.numSourceItem = numSourceItem;
				}
				
				else if( !stricmp( token, "require_skill" ) )
				{
					numRequireSkill = 0;
					token = NextToken();		
					while( 1 )
					{
						token = NextToken();
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							while( 1 )
							{
								token = NextToken();
								if( token[0] == '}' ) break;

								if( !stricmp( token, "index" ) )
								{
									token = NextToken();
									TempItemCraftTable.requireSkill[numRequireSkill].skillIndex = atoi( token );
								}
								else if( !stricmp( token, "above_level" ) )
								{
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevelType = 0;
									token = NextToken();
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevel = atoi( token );
								}
								else if( !stricmp( token, "equal_level" ) )
								{
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevelType = 1;
									token = NextToken();
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevel = atoi( token );
								}
								else if( !stricmp( token, "below_level" ) )
								{
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevelType = 2;
									token = NextToken();
									TempItemCraftTable.requireSkill[numRequireSkill].reqSkillLevel = atoi( token );
								}
							}
							numRequireSkill ++;
						}
					}
					TempItemCraftTable.numRequireSkill = numRequireSkill;
				}
				
				else if( !stricmp( token, "result_item" ) )
				{
					numResultItem = 0;
					token = NextToken();		
					while( 1 )
					{
						token = NextToken();
						if( token[0] == '}' ) break;
						if( token[0] == '{' )
						{
							
							TempItemCraftTable.resultItem[numResultItem].optionGroupIdx = -1;

							while( 1 )
							{
								token = NextToken();
								if( token[0] == '}' ) break;

								if( !stricmp( token, "index" ) )
								{
									token = NextToken();
									TempItemCraftTable.resultItem[numResultItem].itemIndex = atoi( token );
								}
								else if( !stricmp( token, "number" ) )
								{
									token = NextToken();
									TempItemCraftTable.resultItem[numResultItem].itemCount = atoi( token );
								}
								
								
								else if( !stricmp( token, "option_grade") )
								{
									token = NextToken();
									TempItemCraftTable.resultItem[numResultItem].optionGrade = atoi( token );
								}
								else if( !stricmp( token, "rareitem_grade") )
								{
									token = NextToken();
									TempItemCraftTable.resultItem[numResultItem].rareItemGrade = atoi( token );
								}
								else if( !stricmp( token, "optiongroup") )
								{
									token = NextToken();
									TempItemCraftTable.resultItem[numResultItem].optionGroupIdx = atoi( token );
								}
							}
							numResultItem ++;
						}
					}
					TempItemCraftTable.numResultItem = numResultItem;
				}
			}

			if(TempItemCraftTable.specialType == CRAFT_TYPE_EVENT)
			{
				TempItemCraftTable.idx = g_EventCraftSystem.GetTableSize();

				g_EventCraftSystem.Push(TempItemCraftTable);			
			}
			else
			{
				TempItemCraftTable.idx = g_itemCraftTableNumber;

				memcpy(&g_itemCraftTable[g_itemCraftTableNumber],&TempItemCraftTable,sizeof(itemCraftTable_t));
				g_itemCraftTableNumber ++;
			}			
		}
	}
	
	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return 1;
}


int GetChaosClass( playerCharacter_t *pc )
{
	
	if( pc->chaosPoint == CHAOSPURE ) return( CHAOS_PURE );
	
	else if( 0< pc->chaosPoint && pc->chaosPoint == CHAOSGENERATE ) return( CHAOS_GENERAL );
	else if( pc->chaosPoint >CHAOSGENERATE && pc->chaosPoint <= CHAOSDISORDER) return( CHAOS_DISORDER );
	else if( pc->chaosPoint >CHAOSDISORDER && pc->chaosPoint <=CHAOSMONSTER ) return( CHAOS_MONSTER );
	return( CHAOS_ATROCITY );
	
}

void AddChaosPoint( playerCharacter_t* attacker, playerCharacter_t* defender )
{
	
	
	if( attacker->pvpMode || defender->pvpMode) return;
	
	if(attacker->worldIdx == 0 || defender->worldIdx == 0) return;
	
	if(attacker->worldIdx == 9 || defender->worldIdx == 9) return;

	
	if(attacker->worldIdx==tagGolryunBattle::Golryun_Battle_Map_Index || 
	   defender->worldIdx==tagGolryunBattle::Golryun_Battle_Map_Index) return;
	
	
	
	
	if ( attacker->DanBattleInfo.nDanBattleIdx >= 0)
		return;

	int atkChaType, defChaType;

	atkChaType	= GetChaosClass( attacker );
	defChaType	= GetChaosClass( defender );


	
	
   
	
	if(CHAOSGENERATE > defender->chaosPoint)
	{
		int ChaosValue=0;
		bool Sendclass=false,Sendspeed=false;
		
		attacker->penaltyTime++;
		
		ChaosValue=(CHAOVALUE+(attacker->penaltyTime*CHAOSADDRATE));
		attacker->chaosPoint=attacker->chaosPoint+ChaosValue;
		
		
		if(attacker->chaosPoint>CHAOSMAX)
		{
			attacker->chaosPoint=CHAOSMAX;

		}
		
		int convertchaosType=GetChaosClass(attacker);
		int convertchaosTypedefend=GetChaosClass(defender);
		
		if(convertchaosTypedefend!=defChaType)
		{
		
		}
	
          
	   
       GTH_SendMessage_AddChaosInfo(attacker,ChaosValue,Sendclass,false);
	}

	
	if(defender->chaosPoint<CHAOSPURE)
	{
		defender->chaosPoint=0;
	}
	
}

void DecChaosPoint( playerCharacter_t *pc )
{
	
	if(pc->chaosPoint==0)return;
	if( pc->chaosTime == CHAOSPURE )
	{
		pc->chaosTime = g_globalTime;
		return;
	}
	
	if( g_globalTime - pc->chaosTime > CHAOSTIME ) 
	{
		int pcchaosclass =GetChaosClass(pc);
		pc->chaosTime = g_globalTime;
		pc->chaosPoint=pc->chaosPoint-10;
		
		int pcchaosclassaffer =GetChaosClass(pc);
		if(pcchaosclass!=pcchaosclassaffer)
		{
	
		}
	}
	if( pc->chaosPoint < CHAOSPURE ) pc->chaosPoint = 0;
	
}

void SetChaosGeneral( playerCharacter_t *attacker, playerCharacter_t *defender )
{
	if( defender->pvpMode ) return;				
    
	if( attacker->pvpMode ) return;	
	
	if ( attacker->worldIdx == DAN_BATTLEMAP_NO )
		return;	

	
	
	if ( attacker->DanBattleInfo.nDanBattleIdx >= 0)
		return;
	

	
	if(defender->worldIdx==21 || attacker->worldIdx==21) return;
	

	
	if(defender->worldIdx == 0 || attacker->worldIdx == 0) return;
	if(defender->worldIdx == 9 || attacker->worldIdx == 9) return;
	
	if(attacker->chaosPoint > CHAOSGENERATE) return;
	
	if(attacker->chaosPoint < CHAOSGENERATE)attacker->chaosPoint=0;

	if(defender->chaosPoint <CHAOSGENERATE)defender->chaosPoint=0;

	
	if(CHAOSPURE<=attacker->chaosPoint && attacker->chaosPoint<=CHAOSGENERATE)
	{
		if( CHAOSGENERATE > defender->chaosPoint )
	{
		attacker->chaosPoint=CHAOSGENERATE;
		attacker->chaosTime = g_globalTime;
	}
	}
	
}



void GTH_PickUpNak( playerCharacter_t *pc, itemObject_t *itemObject )
{
	playerCharacter_t	*member;
	int					memberNumber = 0, memberIdx, pcMemberIdx[MAX_NUMBER_OF_PARTY_MEMBER];
	float				dist;

	
	if( pc->organizeServer < 0 )
	{
		GTH_PC_GetLoot( pc->idx, 0, 0, itemObject->nak, 0, 0 );
	}
	
	else
	{
		
		for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
		{
			if( pc->member[i].serverIndex < 0 ) continue;
			if( pc->member[i].serverIndex != g_config.gameServerNo ) continue;



			memberIdx = pc->member[i].idxOnServer;
			if(memberIdx < 0) continue;
			if(memberIdx >= MAX_PCS) continue;

			
			member = &g_pc[memberIdx];
			if( !member->active ) continue;				
			if( !member->alive ) continue;
			if(0 != strcmp( member->name, pc->member[i].memberName ) ) continue;

			dist = GTH_GetDistance2D( pc->position, member->position );
			if( dist > 2400.0f ) continue;
			pcMemberIdx[memberNumber] = memberIdx;
			memberNumber ++;
		}



		if( memberNumber > 0 )
		{
			for( i = 0; i < memberNumber; i ++ )
			{
				memberIdx = pcMemberIdx[i];
				if(memberIdx < 0) continue;
				if(memberIdx >= MAX_PCS) continue;

				member = &g_pc[memberIdx];
				if( !member->active ) continue;				
				if( !member->alive ) continue;

				GTH_PC_GetLoot( member->idx, 0, 0, itemObject->nak / memberNumber, 0, 0 );
			}
		}
	}
	GTH_RemoveItemObject( itemObject );
}




void GetWord(int index, const char *source, char* token)
{
	int i=0;
	int j=0;

	while( source[i] != '\0' ) 
	{
		if( source[i] == ' ' )
		{
			token[j] = '\0';

			if( index == 0 )
				return;
			else
			{
				i++;
				j=0;
				index--;

				continue;
			}
		}
		else
		{
			token[j] = source[i];
		}

		i++;
		j++;
	}

	token[j] = '\0';
}


int IsPKDisableWorld( int worldIdx )
{
		
	
	

	return 0;
}







int IsPvPEnablePosition( int worldIdx, vec3_t pos)
{
	

	
	
	
	if(worldIdx== tagGolryunBattle::Golryun_Battle_Map_Index)
	{
		if((13615<=pos[0] && pos[0]<=15184)&&(2901<=pos[1]&&pos[1]<=4467)) return 30;
	}
	

	
	return 0;
}




float	GetEpackRechargePrice_RA( int onceRecovery, float cost, int level )
{
	return( (float)onceRecovery * cost * powf( (float)level, 0.67f ) * 1.5f / 8.0f  );
} 

float	GetEpackRechargePrice_SA( int onceRecovery, float cost, int level )
{
	return( (float)onceRecovery * cost * powf( (float)level, 0.67f ) * 1.7f / 8.0f  );
}





bool	CollectItemForGamble(GambleItem_t GambleItem[] )
{	
	
	for (int i = 0; i < g_itemTableNumber; i ++)
	{		
		if ( g_itemTable[i].idx < 0 )
			continue;

		
		int availableOn	= GetItemAvailalbeOn( g_itemTable[i].idx );
		int type = GetGambleItemAvallableOn(availableOn);
		
		
		



		if ( !strcmp (g_itemTable[i].name, "BLANK" ) ) 
			continue;

		
		
		if ( g_itemTable[i].sellYn < 2)
			continue;
		
		
		
		if ( type == -1)
			type = GambleItem_t.OPTION_AVAILABLE_ON_ETC;
		
		
		
		GambleItem[type].type = availableOn;

		if ( !GambleItem[type].vItem.size() )
			g_GambleItemTalbleNumber++;
		
			GambleItem[type].vGambleRate.push_back(g_logic.gambleitemSelectRate[g_itemTable[i].rarity]);
			GambleItem[type].TotalGamebleRate += g_logic.gambleitemSelectRate[g_itemTable[i].rarity];
		
		GambleItem[type].vItem.push_back(g_itemTable[i].idx);		
	}
	
	for (i =0; i < MAX_COUNT_OPTION_AVAILABLE_TYPE; i ++)
	{			
		if ( GambleItem[i].type  <= 0 ) continue;

		
		for( int o = 0; o < MAX_COUNT_OPTION_TABLE; o ++ )
		{												
			if( g_optionTable[o].idx < 0 ) continue;
			
			
			



			
			
			
			if( !( g_optionTable[o].availableOn & GambleItem[i].type ) ) 
				continue;
			
			GambleItem[i].vOption.push_back(g_optionTable[o].idx);
		}	
	}

	return 1;
}

int GetGambleItemAvallableOn( int availableon )
{
	if ( availableon & OPTION_AVAILABLE_ON_DAGGER )	
		return GambleItem_t.OPTION_AVAILABLE_ON_DAGGER ;
	
	if ( availableon & OPTION_AVAILABLE_ON_1HSWORD )	
		return GambleItem_t.OPTION_AVAILABLE_ON_1HSWORD ;

	if ( availableon & OPTION_AVAILABLE_ON_2HSWORD )	
		return GambleItem_t.OPTION_AVAILABLE_ON_2HSWORD;		

	if ( availableon & OPTION_AVAILABLE_ON_BOW )
		return GambleItem_t.OPTION_AVAILABLE_ON_BOW;

	if ( availableon & OPTION_AVAILABLE_ON_STAFF )			
		return GambleItem_t.OPTION_AVAILABLE_ON_STAFF;

	if ( availableon & OPTION_AVAILABLE_ON_SPIRITOR )	
		return GambleItem_t.OPTION_AVAILABLE_ON_SPIRITOR;

	if ( availableon & OPTION_AVAILABLE_ON_CLAW )		
		return GambleItem_t.OPTION_AVAILABLE_ON_CLAW;




	if ( availableon & OPTION_AVAILABLE_ON_UPPER )		
		return GambleItem_t.OPTION_AVAILABLE_ON_UPPER;

	if ( availableon & OPTION_AVAILABLE_ON_LOWER )		
		return GambleItem_t.OPTION_AVAILABLE_ON_LOWER;

	if ( availableon & OPTION_AVAILABLE_ON_AIRPROTECTOR )
		return GambleItem_t.OPTION_AVAILABLE_ON_AIRPROTECTOR;

	if ( availableon & OPTION_AVAILABLE_ON_BODYPROTECTOR )	
		return GambleItem_t.OPTION_AVAILABLE_ON_BODYPROTECTOR;

	if ( availableon & OPTION_AVAILABLE_ON_GLOVES )		
		return GambleItem_t.OPTION_AVAILABLE_ON_GLOVES;

	if ( availableon & OPTION_AVAILABLE_ON_BOOTS )		
		return GambleItem_t.OPTION_AVAILABLE_ON_BOOTS;

	if ( availableon & OPTION_AVAILABLE_ON_RING )		
		return GambleItem_t.OPTION_AVAILABLE_ON_RING;

	if ( availableon & OPTION_AVAILABLE_ON_NECKLACE )	
		return GambleItem_t.OPTION_AVAILABLE_ON_NECKLACE;

	if ( availableon & OPTION_AVAILABLE_ON_BRACELET )	
		return GambleItem_t.OPTION_AVAILABLE_ON_BRACELET;

	if ( availableon & OPTION_AVAILABLE_ON_EARRING )	
		return GambleItem_t.OPTION_AVAILABLE_ON_EARRING;

	if ( availableon & OPTION_AVAILABLE_ON_EPACK )
		return GambleItem_t.OPTION_AVAILABLE_ON_EPACK;	

	return -1;
}







void SelectGambleItem(storeStruct_t* Makestore, storeStruct_t* Gamblestore, int pcIdx )
{

	if ( pcIdx < 0)
		return;

	
	memcpy( Makestore, Gamblestore, sizeof(storeStruct_t) );
	
	memset(g_pc[pcIdx].RegGambleItems, -1, sizeof(int) *  MAX_NUMBER_OF_STORE_ITEM);
	
	memset(Makestore->item, 0, sizeof (storeItem_t ) * MAX_NUMBER_OF_STORE_ITEM);
	Makestore->itemNumber = 0;
	int curmakepos = 0;
	

	
	std::vector<int> vpageItems[3];	
	std::vector<int> vpageItemsRegCount[3];	
	
	int	WilldbiRegCountofPage[3];
	memset(WilldbiRegCountofPage, 0, sizeof (int) * 3);
	

	int SelectdCount = 0;	
	int	CopyItemCount;
	for (int i = 0; i < MAX_NUMBER_OF_STORE_ITEM; i ++)
	{
		
		if ( Gamblestore->item[i].maxNumber == 0 && Gamblestore->item[i].maxNumber == 0)		
			continue;

		
		vpageItems[Gamblestore->item[i].page].push_back(i);					
		
		float r = GTH_Random();			
		if ( r < 0.5 )
			CopyItemCount = 2;
		else
			CopyItemCount = 3;

		vpageItemsRegCount[Gamblestore->item[i].page].push_back(CopyItemCount);
		WilldbiRegCountofPage[Gamblestore->item[i].page] += CopyItemCount;
		SelectdCount++;

		
		if ( SelectdCount >= Gamblestore->itemNumber)
			break;
	}

	

	DWORD RegItemflag = 0;
	int frontPageRegeditCount  = 0;
	int CurSrcGamblePos = 0;

	for (int page = 0; page < 3; page ++)
	
	{	
		
		int itemcount = vpageItems[page].size();
		
		int regCountOfPage = 0;
		for (int i =0; i < itemcount; i ++)
		{		
			
			
			CurSrcGamblePos =  frontPageRegeditCount + i;			
			
			
			for ( int Idx = 0; Idx < vpageItemsRegCount[page][i]; Idx++)
			{	
				
				init_genrand( (unsigned) time( NULL ) );				
				int CurDestPos =  (Makestore->itemNumber - regCountOfPage) + (rand() % WilldbiRegCountofPage[page]);					
				

				if ( CurDestPos < 0 || CurDestPos >= MAX_NUMBER_OF_STORE_ITEM )
					CurDestPos = 0;				

				

				
				
				if ( Makestore->item[CurDestPos].curNumber != 0 && Makestore->item[CurDestPos].maxNumber != 0)
				{
					
					for (CurDestPos = 0; CurDestPos <MAX_NUMBER_OF_STORE_ITEM; CurDestPos++ )
					{
						if ( Makestore->item[CurDestPos].curNumber != 0 && Makestore->item[CurDestPos].maxNumber != 0)
						{
							
							continue;
						}




























						break;
					}

				}



					
					Makestore->item[CurDestPos].curNumber = Gamblestore->item[CurSrcGamblePos].curNumber;
					Makestore->item[CurDestPos].maxNumber = Gamblestore->item[CurSrcGamblePos].maxNumber;
					Makestore->item[CurDestPos].page = Gamblestore->item[CurSrcGamblePos].page;						
					
					memcpy ( &Makestore->item[CurDestPos].item, &Gamblestore->item[CurSrcGamblePos].item, sizeof (item_t ));
						
					
					
					int Selecteditemidx = SelectCollectedItem(&Makestore->item[CurDestPos]);
					float AddPrice = (rand() %( (int)g_logic.GamblePriceRate ) )* 0.01;

					int CalPrice = 0;
					
					
					if ( Makestore->item[CurDestPos].item.itemTableIdx == 97)
					{												
						g_pc[pcIdx].RegGamblePrice[CurDestPos] = g_itemTable[97].gambleCost;
						Makestore->item[CurDestPos].price = g_itemTable[97].gambleCost;
					}
					else
					{						
						if ( GTH_Random() > 0.5 )											
							CalPrice = g_itemTable[Selecteditemidx].gambleCost + (g_itemTable[Selecteditemidx].gambleCost) * AddPrice;
						else
							CalPrice = g_itemTable[Selecteditemidx].gambleCost - (g_itemTable[Selecteditemidx].gambleCost) * AddPrice;											
						
						g_pc[pcIdx].RegGamblePrice[CurDestPos] = CalPrice;
						Makestore->item[CurDestPos].price = CalPrice;				
					}

					g_pc[pcIdx].RegGambleItems[CurDestPos] = Selecteditemidx;					
				
					
					Makestore->itemNumber++;				
					regCountOfPage++;
				
			}						
		}		
		frontPageRegeditCount += itemcount;
	}
	
}



int SelectCollectedItem(storeItem_t* pItem)
{	
	
	int StorItemIdx		= pItem->item.itemTableIdx;
	int availableOn		= GetItemAvailalbeOn( g_itemTable[StorItemIdx].idx );

	
	int type			= GetGambleItemAvallableOn(availableOn);	

	
	
	if ( type < 0 || type > MAX_COUNT_OPTION_AVAILABLE_TYPE)	
	{
		init_genrand( (unsigned) time( NULL ) );		
		int SelecttypeNum = (rand() % g_GambleItemTalbleNumber+1);					
		

		int	CurSelectTypeNum = 0;

		for (int i = 0; i < MAX_COUNT_OPTION_AVAILABLE_TYPE; i ++)
		{
			if ( g_GambleItem[i].vItem.size() )
				CurSelectTypeNum++;

			if ( SelecttypeNum <= CurSelectTypeNum)
			{
				type = i;			
				break;
			}
		}		
				

		
		if ( !g_GambleItem[type].vItem.size() )
			type = 1;
	}

	
	float r			= 0.0f;
	float accPro	= 0.0f;
	init_genrand( (unsigned) time( NULL ) );
	r				= GTH_Random();

	float TotalRate = g_GambleItem[type].TotalGamebleRate;
	int selectGambleItemidx = -1;		

	int selectRanNum = rand() % g_GambleItem[type].vItem.size();

	for (int i =0; i < g_GambleItem[type].vItem.size(); i ++, selectRanNum++)
	{		
		
		if ( selectRanNum >= g_GambleItem[type].vItem.size())
			selectRanNum = 0;

		accPro += g_GambleItem[type].vGambleRate[selectRanNum] / TotalRate;		
		
		if ( r <  accPro )		
			break;

		TotalRate -= g_GambleItem[type].vGambleRate[selectRanNum];
	}	
	
	if ( selectRanNum < 0 )
		selectRanNum = 0 ;

	if ( selectRanNum > MAX_NUMBER_OF_ITEM_TABLE)
	{
		if ( g_GambleItem[type].vItem.size() > 0)
			selectRanNum = g_GambleItem[type].vItem.size() -1;
	}
	
	
	

	return  g_GambleItem[type].vItem[selectRanNum];
	
}







int GTH_MakePortal_fordan( int LootType, int makerIdx, int keepTime, int worldIdx, vec3_t position, int targetWorldIdx, vec3_t targetPosition, int targetWorldIdx2, vec3_t targetPosition2)
{
	int i;

	
	itemObject_t *itemObject;

	if ( g_objectNumber >= MAX_OBJECTS ) return -1;	
	
	itemObject = &g_object[0];

	for (i=0; i < MAX_OBJECTS; i++, itemObject++)
	{
		if ( !itemObject->active ) break;
	}

	if ( i >= MAX_OBJECTS ) return -1;

	itemObject->idx = i;
	itemObject->active = true;
	itemObject->objectType = OBJECT_TYPE_PORTAL;
	itemObject->worldIdx = worldIdx;	

	vec_copy(position, itemObject->position);

	itemObject->portal.portalType = 0;
	
	
	itemObject->portal.worldIdx[0] = targetWorldIdx;
	itemObject->portal.worldIdx[1] = targetWorldIdx2;
	
	itemObject->portal.numPerson = 1;
	if ( targetPosition != NULL)
		vec_copy( targetPosition, itemObject->portal.position[0] );
	if ( targetPosition2 != NULL)
		vec_copy( targetPosition2, itemObject->portal.position[1] );
	
	itemObject->entityType = ENTITY_OBJECT;

	vec_set(itemObject->angles, 0, 0, 0);

	itemObject->zoneIdx = GTH_Zone_GetZoneIdx(itemObject->position);

	if ( !GTH_Zone_AddEntityList(itemObject->worldIdx, itemObject->zoneIdx, ENTITY_OBJECT, itemObject->idx) )
	{
		return -1;
	}
	
	itemObject->lootPCIdx = makerIdx;
	itemObject->lootTime = keepTime;
	itemObject->destroyTime = g_globalTime + keepTime;

	if ( LootType == LOOTTYPE_DANBATTLE_TEAMS )		
		itemObject->lootType = LOOTTYPE_DANBATTLE_TEAMS;

	else if ( LootType == ENTITY_NPC )		
	{		
		itemObject->lootPCIdx = -1;
		itemObject->lootType = LOOTTYPE_EVERYONE;
	}
	else if ( LootType = LOOTTYPE_DAN )
	{
		{		
			itemObject->lootType = LOOTTYPE_DAN;
			strcpy(itemObject->DanName, g_pc[makerIdx].guildName);
		}
	}
	g_objectNumber++;

	
	GTH_SendMessage_AddItemObject( itemObject );

	return( 1 );
}










BOOL	IsValidHotKey_Skill(const playerCharacter_t *pc, const int HotkeyIdx) 
{		
	if ( (HotkeyIdx >= MAX_NUMBER_OF_HOTKEY) || (HotkeyIdx < 0) )
		return FALSE;

	int type		=pc->hotkeyType[HotkeyIdx];
	int learnIdx	=pc->hotkeyIdx[HotkeyIdx];
	
	if(0 != type) return TRUE;

	
	if( FALSE == SAFE_SKILL_INDEX(learnIdx) )
		return FALSE;

	const skill_t* pSkill = gcpTools->GetMySkillTpointer((playerCharacter_t*)pc,learnIdx);
	if( NULL == pSkill ) 
		return FALSE;
	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(pSkill->tableIdx);
	if( NULL == pSkillTable )
	{
		TRACE("Error! pSkillTable==NULL   \n");
		return FALSE;
	}

	
	
	if ((0 > learnIdx) || 
		(learnIdx >= pc->skillNumber) ||
		(pSkill->tableIdx < 0) ||
		(pSkill->tableIdx >= MAX_NUMBER_OF_SKILL) ||
		(pSkillTable->m_nUsageType == 1)){		
		
		return FALSE;
	}	

	return TRUE;	
}




void GTH_PartyRandomItemDivide( playerCharacter_t *pc,itemObject_t *itemObject )
{
		
	serverPartyInfo_t *pi=NULL;
	playerCharacter_t *member=NULL;
	playerCharacter_t *selectUser=NULL;
	int i=0,idx=-1,itemIdx;
	int reply, error;
	int inventoryPos;
	int stackItemIdx;
	item_t* upItem;
	item_t *item;
	int bIsStack = false;
	int stackNumber = 1;
	char generateID[20]; 

	idx = GTH_GetPartyInfo( pc->organizerName, pc->organizeServer,pc->organizeTime );
	
	pi = &g_partyInfo[idx];
	if(pi==NULL) return;
	
	int irandValue	= GTH_RandomizePower(0,pi->memberNumber);
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		int memberIdx = pi->member[i].idxOnServer;
		if( pi->member[i].serverIndex < 0 ) continue;
		
		
		if(pi->member[i].serverIndex != g_config.gameServerNo ) continue;
		
		member = &g_pc[memberIdx];
		if(member==NULL) return;
		if( !member->active || !member->alive ) continue;
		
	    if(irandValue>=i) break;
	}

	item = &itemObject->item;
	if( g_itemTable[item->itemTableIdx].stackFlag ) 
	{
		bIsStack = true;
		stackNumber = item->durability + 1;
	}

	
	if( member->curItemWeight > member->calItemWeight )
	{
		reply = 0;
		error = ERROR_PICKUPITEM_7;
		goto REPLY_PICKUPITEM;
	}

	stackItemIdx = GTH_StackMyItem( member, item );
	if( stackItemIdx >= 0 )
	{
		upItem = &member->item[stackItemIdx];
		upItem->durability += item->durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;

		g_DBGameServer->SaveItem( member, upItem, stackItemIdx );

		
		
		
		g_logSystem->WriteToLog( member, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_PICKUP, stackNumber, item->name, 
			GetItemInformationLog( bIsStack, item->itemTableIdx, stackItemIdx, -1 ) );
		
	}
	else
	{
		
		inventoryPos = GTH_FindEmptyInventoryPos( member );
		if( inventoryPos < 0 )	
		{
			reply = 0;
			error = ERROR_PICKUPITEM_4;

			g_logSystem->Write("Pick Up Error!! : Inventory Space Not Enough. %s(%s), objectIdx:%d)", member->name, member->userID, itemObject->idx);
			goto REPLY_PICKUPITEM;
		}

		
		itemIdx = GTH_AddMyItem( member, item );
		if (itemIdx < 0)	
		{
			reply = 0;
			error = ERROR_PICKUPITEM_5;
			goto REPLY_PICKUPITEM;
		}
		
		
		member->inventory[inventoryPos] = itemIdx;
		
		strcpy(generateID, "pickUpGenID");
		
		if ( !g_DBGameServer->AddItem(member, item, itemIdx, generateID) )
		{
			g_logSystem->Write("Can Not Insert in DataBase(Item Table).(%s, %d, %d)", member->userID, member->serverGroupID, member->characterID);
			reply = 0;
			error = ERROR_PICKUPITEM_6;
			goto REPLY_PICKUPITEM;
		}

		
		
		
		g_logSystem->WriteToLog( member, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_PICKUP, stackNumber, item->name, 
			GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, inventoryPos ) );
		
	}

	
	g_logSystem->Write("User '%s'(ID: %s) Obtain Item '%s'", member->name, member->userID, g_itemTable[item->itemTableIdx].name);

	
	GTH_RemoveItemObject( itemObject );
	reply = 1;
	error = ERROR_PICKUPITEM_NONE;

	member->curItemWeight += g_itemTable[item->itemTableIdx].weight;

REPLY_PICKUPITEM:
	GTH_SendMessage_ReplyPickUpItem( member, reply, error, item );
}


void GTH_PartyOrderItemDivide( playerCharacter_t *pc,itemObject_t *itemObject )
{
	
	
	serverPartyInfo_t *pi=NULL;
	playerCharacter_t *member=NULL;
	playerCharacter_t *selectUser=NULL;
	int i=0,idx=-1,itemIdx;
	int reply, error;
	int inventoryPos;
	int stackItemIdx;
	item_t* upItem;
	item_t *item;
	int bIsStack = false;
	int stackNumber = 1;
	char generateID[20]; 

	idx = GTH_GetPartyInfo( pc->organizerName, pc->organizeServer,pc->organizeTime );
	if(idx < 0) return;
	if(idx >= MAX_PCS) return;
	pi = &g_partyInfo[idx];
	if(pi==NULL) return;
	
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i++)
	{
		
		int memberIdx = pi->member[i].idxOnServer;
		if(memberIdx < 0) return;
		if(memberIdx >= MAX_PCS) return;
		if( pi->member[i].serverIndex < 0 ) continue;
		
		
		if(pi->member[i].serverIndex != g_config.gameServerNo ) continue;
		
		selectUser = &g_pc[memberIdx];
		if(!selectUser->active || !selectUser->alive) continue;
	
	
	}

	item = &itemObject->item;
	if( g_itemTable[item->itemTableIdx].stackFlag ) 
	{
		bIsStack = true;
		stackNumber = item->durability + 1;
	}

	
	if( member->curItemWeight > member->calItemWeight )
	{
		reply = 0;
		error = ERROR_PICKUPITEM_7;
		goto REPLY_PICKUPITEM;
	}

	stackItemIdx = GTH_StackMyItem( member, item );
	if( stackItemIdx >= 0 )
	{
		upItem = &member->item[stackItemIdx];
		upItem->durability += item->durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;

		g_DBGameServer->SaveItem( member, upItem, stackItemIdx );

		
		
		
		g_logSystem->WriteToLog( member, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_PICKUP, stackNumber, item->name, 
			GetItemInformationLog( bIsStack, item->itemTableIdx, stackItemIdx, -1 ) );
		
	}
	else
	{
		
		inventoryPos = GTH_FindEmptyInventoryPos( member );
		if( inventoryPos < 0 )	
		{
			reply = 0;
			error = ERROR_PICKUPITEM_4;

			g_logSystem->Write("Pick Up Error!! : Inventory Space Not Enough. %s(%s), objectIdx:%d)", member->name, member->userID, itemObject->idx);
			goto REPLY_PICKUPITEM;
		}

		
		itemIdx = GTH_AddMyItem( member, item );
		if (itemIdx < 0)	
		{
			reply = 0;
			error = ERROR_PICKUPITEM_5;
			goto REPLY_PICKUPITEM;
		}
		
		
		member->inventory[inventoryPos] = itemIdx;
		
		strcpy(generateID, "pickUpGenID");
		
		if ( !g_DBGameServer->AddItem(member, item, itemIdx, generateID) )
		{
			g_logSystem->Write("Can Not Insert in DataBase(Item Table).(%s, %d, %d)", member->userID, member->serverGroupID, member->characterID);
			reply = 0;
			error = ERROR_PICKUPITEM_6;
			goto REPLY_PICKUPITEM;
		}

		
		
		
		g_logSystem->WriteToLog( member, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_PICKUP, stackNumber, item->name, 
			GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, inventoryPos ) );
		
	}

	
	g_logSystem->Write("User '%s'(ID: %s) Obtain Item '%s'", member->name, member->userID, g_itemTable[item->itemTableIdx].name);

	
	GTH_RemoveItemObject( itemObject );
	reply = 1;
	error = ERROR_PICKUPITEM_NONE;

	member->curItemWeight += g_itemTable[item->itemTableIdx].weight;

REPLY_PICKUPITEM:
	GTH_SendMessage_ReplyPickUpItem( member, reply, error, item );
}






void GTH_SharingPartyLoot( 
	monsterCharacter_t *m, 
	playerCharacter_t *pc, 
	int exp, 
	int genExp, 
	int lootNSE, 
	int genCapability, 
	int atkConIdx, 
	int defConIdx )
{
	
	
	if(TRUE == IsBadReadPtr(pc,sizeof(playerCharacter_t*)))
	{
		g_logSystem->Write("GTH_SharingPartyLoot pc");
		return;
	}
	
	if(TRUE == IsBadReadPtr(m,sizeof(monsterCharacter_t*)))
	{
		g_logSystem->Write("GTH_SharingPartyLoot monster");
		return;
	}

	playerCharacter_t *member=NULL, *partymember=NULL;
	
	int memberNumber = 0, memberIdx, pcMemberIdx[MAX_NUMBER_OF_PARTY_MEMBER], levelDiffer;
	float incRate;
	int maxLv = 0, partyExp, partyGenExp, partyGenCapability, invPartyExp, invPartyGenExp, invPartyNSE, invPartyGenCapability;
	int sharingType = 0;
	
	
	for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		memberIdx = pc->member[i].idxOnServer;
		if( pc->member[i].serverIndex < 0 ) continue;

		
		if( pc->member[i].level > maxLv ) maxLv = pc->member[i].level;

		
		if( pc->member[i].serverIndex != g_config.gameServerNo ) continue;
		
		
		
		member = gcpTools->GetPlayerRecordPointer(memberIdx);
		if(NULL == member) continue;
		if(!member->alive) continue;
		if(0 !=stricmp( member->name, pc->member[i].memberName ) ) continue;

		if( !GTH_IsSharingParty( m, member ) ) continue;

		pcMemberIdx[memberNumber] = memberIdx;
		memberNumber ++;
	}
	if( memberNumber <= 0 ) return;
	
	if( memberNumber == 1 )
	{
		GTH_GetExpFromLevelDiffer( g_pc[pcMemberIdx[0]].level - m->basic.level, exp, genExp, genCapability );
		GTH_PC_GetLoot( pcMemberIdx[0], exp, genExp, lootNSE, genCapability, 0 );
		return;
	}

	
	switch( memberNumber )
	{
	case 1 :
		incRate = 0.0f;
		break;
	case 2 :
		incRate = 0.3f;
		break;
	case 3 :
		incRate = 0.6f;
		break;
	case 4 :
		incRate = 0.9f;
		break;
	case 5 :
		incRate = 1.25f;
		break;
	case 6 :
		incRate = 1.60f;
		break;
	case 7 :
		incRate = 1.95f;
		break;
	case 8 :
		incRate = 2.40f;
		break;

	}
	partyExp = (int)( (float)exp * incRate ) + exp;
	partyGenExp = (int)( (float)genExp * incRate ) + genExp;
	partyGenCapability = (int)( (float)genCapability * incRate ) + genCapability;

	if( atkConIdx == defConIdx ) defConIdx = -1;

	
	
	
	
	for( i = 0; i < memberNumber; i ++ )
	{
		
		
		partymember=gcpTools->GetPlayerRecordPointer(pcMemberIdx[i]);
		if(NULL==partymember) continue;
		
		levelDiffer = abs( partymember->level - maxLv );
		if( levelDiffer >= 16 ) continue;

		
		invPartyExp = (int)( (float)partyExp / (float)memberNumber );
		invPartyGenExp = (int)( (float)partyGenExp / (float)memberNumber );
		invPartyNSE = (int)( (float)lootNSE / (float)memberNumber );
		invPartyGenCapability = (int)( (float)partyGenCapability / (float)memberNumber );

		sharingType = 1;

		
		
		if( atkConIdx >= 0 )
		{
			if( partymember->idx == m->conInfo[atkConIdx].entityIdx ) 
			{
				invPartyExp = (int)( (float)invPartyExp * 0.2f ) + invPartyExp;
				invPartyGenExp = (int)( (float)invPartyGenExp * 0.2f ) + invPartyGenExp;
				invPartyGenCapability = (int)( (float)invPartyGenCapability * 0.2f ) + invPartyGenCapability;

				sharingType = 2;
			}
		}
		if( defConIdx >= 0 )
		{
			if( partymember->idx == m->conInfo[defConIdx].entityIdx ) 
			{
				invPartyExp = (int)( (float)invPartyExp * 0.1f ) + invPartyExp;
				invPartyGenExp = (int)( (float)invPartyGenExp * 0.1f ) + invPartyGenExp;
				invPartyGenCapability = (int)( (float)invPartyGenCapability * 0.1f ) + invPartyGenCapability;

				sharingType = 3;
			}
		}
		
		
		if( levelDiffer >= 8 )
		{
			incRate = 1.0f - 0.03f * (float)( levelDiffer - 7 );
		}
		else
		{
			incRate = 1.0f;
		}

		invPartyExp = (int)( (float)invPartyExp * incRate );
		invPartyGenExp = (int)( (float)invPartyGenExp * incRate );
		invPartyGenCapability = (int)( (float)invPartyGenCapability * incRate );

		
		
		GTH_GetExpFromLevelDiffer( partymember->level - m->basic.level, invPartyExp, invPartyGenExp, invPartyGenCapability );
		GTH_PC_GetLoot( partymember->idx, invPartyExp, invPartyGenExp, invPartyNSE, invPartyGenCapability, sharingType );
	}
}













BOOL GTH_IsHaveItem(playerCharacter_t* pPc, int itemTableIdx, int StackCount)
{
	if ( pPc == NULL )
		return FALSE;

	if ( itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
		return FALSE;

	for (int InvenIdx = 0; InvenIdx < MAX_INVENTORY_SIZE; InvenIdx ++)
	{
		item_t* pItem = NULL;

		int ItemIdx = pPc->inventory[InvenIdx];

		if ( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM )
			continue;
		
		
		if (pPc->item[ItemIdx].itemTableIdx == itemTableIdx)
		{					
			
			if ( pPc->item[ItemIdx].durability + 1 >= StackCount )
				return TRUE;
			else
				return FALSE;
				
		}
	}	
	return FALSE;
}





BOOL GTH_IsAwakenJob(playerCharacter_t* pPc)
{
	if ( pPc == NULL )
		return FALSE;

	if ( pPc->pcJob <= J_EARTH )
		return FALSE;	
	return TRUE;		
}




BOOL GTH_ClearAwakenJob(playerCharacter_t *pPC, std::vector<int>* pVSkill)
{
	if ( pPC == NULL )
		return FALSE;

	int GenType = GTH_GetGenTypeFromJob(g_curPC->pcJob);	

	int force		= 0;
	int SoulPower	= 0;
	int Agility		= 0;
	int Vitality	= 0;

	
	switch( g_curPC->pcJob )
	{	
		case J_DEVOTIONAL_KNIGHT :
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 5;
			Vitality	+= 0;
			break;


		case J_ROMING_KNIGHT :
			
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 0;
			Vitality	+= 5;
			break;		

		case J_PIECING_EYES :
			
			force		+= 0;
			SoulPower	+= 5;
			Agility		+= 0;
			Vitality	+= 0;
			break;
		case J_WINDY_ROMER :
			
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 0;
			Vitality	+= 5;
			break;
		case J_DESTROYER :
			
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 5;
			Vitality	+= 0;
			break;
		case J_GUARDIAN :
			
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 0;
			Vitality	+= 5;
			break;
		case J_PROPHECIER :
			
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 0;
			Vitality	+= 0;
			break;		
		case J_TRANSFORMER :
			
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 5;
			Vitality	+= 0;
			break;
		case J_DEFENDER :
			
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 0;
			Vitality	+= 5;
			break;
		case J_STRANGER :
			
			force		+= 0;
			SoulPower	+= 0;
			Agility		+= 0;
			Vitality	+= 0;
			break;
	}

	
	g_curPC->force		-= force;
	g_curPC->soulpower	-= SoulPower;
	g_curPC->agility	-= Agility;
	g_curPC->vitality	-= Vitality;

	if ( g_curPC->force < 0)
		g_curPC->force = 0;

	if ( g_curPC->soulpower < 0)
		g_curPC->soulpower = 0;
	
	if ( g_curPC->agility < 0)
		g_curPC->agility = 0;
	
	if ( g_curPC->vitality < 0)
		g_curPC->vitality = 0;

	

	
	for (int GenIdx = 0; GenIdx < GEN_NUMBER; GenIdx++)
	{
		if ( GenType != GenIdx)
			g_curPC->genCapability[GenIdx] = -1;
	}
	
	int QuestIdx = GetAwakenQuestIdx(GenType);

	
	if ( QuestIdx >= 0 || QuestIdx< MAX_COUNT_QUEST_VARIABLE)
	{		
		g_curPC->questVar[QuestIdx].iVar = 65280;
		GTH_ClearQuestStatus( g_curPC, QuestIdx, 0 );
	}


	

	for (int SkillIdx =0; SkillIdx < MAX_NUMBER_OF_SKILL; SkillIdx++ )
	{	
		skill_t* pSkill = &g_curPC->skill[SkillIdx];
		CSkilTable *pSkillTable = g_SkillTableDB.FindDB(pSkill->tableIdx);

		if ( pSkillTable == NULL)
			continue;
		
		
		if(  g_curPC->pcJob == pSkillTable->m_nJobType ) 			
		{
			DeleteSkill(g_curPC, SkillIdx);			
			pVSkill->push_back(SkillIdx);	

			
			for (int HotKeyIdx = 0; HotKeyIdx < MAX_NUMBER_OF_HOTKEY; HotKeyIdx++)
			{
				if ( g_curPC->hotkeyType[HotKeyIdx] == 0 && 
					g_curPC->hotkeyIdx[HotKeyIdx] == SkillIdx)
				{
					g_curPC->hotkeyIdx[HotKeyIdx] = -1;
					g_curPC->hotkeyType[HotKeyIdx] = -1;				
				}
			}
		}
	}

	g_curPC->pcJob = GenType;
	g_curPC->selectedGen = GenType;

	
	GTH_PC_Calculate( g_curPC );	

	return TRUE;
}




int GetAwakenQuestIdx(int pcJob)
{
	if( pcJob == J_MENTALITY || pcJob == J_DEVOTIONAL_KNIGHT || 
		pcJob == J_ROMING_KNIGHT ) 
		return( KNIGHT_QUEST_IDX );
	
	if( pcJob == J_WIND || pcJob == J_PIECING_EYES || 
		pcJob == J_WINDY_ROMER ) 
		return( WIND_QUEST_IDX );

	if( pcJob == J_WATER || pcJob == J_DESTROYER || 
		pcJob == J_GUARDIAN || pcJob == J_PROPHECIER ) 
		return( WATER_QUEST_IDX );

	if( pcJob == J_EARTH || pcJob == J_TRANSFORMER || 
		pcJob == J_DEFENDER || pcJob == J_STRANGER ) 
		return( EARTH_QUEST_IDX );

	return( -1 );
}



void DeleteSkill(playerCharacter_t* pPc, int SkillIdx)
{
	if ( SkillIdx <0 || SkillIdx >= MAX_NUMBER_OF_SKILL )
		return;
	skill_t* pSkill = &pPc->skill[SkillIdx];	

	pSkill->tableIdx = -1;
	pSkill->iLevel = 0;
	
	
	pPc->skillCooltime[SkillIdx] = 0.0f;
	pPc->skillNumber --;

}


BOOL GTH_IsEquipItem(playerCharacter_t* pPc)
{
	if ( pPc == NULL )
		return FALSE;

	for( int equipItemCount = 0; equipItemCount < MAX_EQUIPMENT; equipItemCount ++ )
	{
		int equipmentIdx = pPc->equipment[equipItemCount];
		
		if ( equipItemCount == EQUIP_EPACK)
			continue;
				
		if ( equipmentIdx >= 0)			
			return TRUE;

	}

	return FALSE;
}







int LoadMax_Defense_table( char* filename )
{
	int size;
	byte *buffer;
	char* token;
	int level;
	int expLevel=0;

	FILE *fp;

	fp = fopen( filename, "rb" );

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	SetTokenBuffer( buffer, size );

	for( int i = 0; i < MAX_NUMBER_OF_LEVEL; i ++ )
		memset( &g_MaxDefneseTable[i], 0, sizeof( MaxDefense_t ) );

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;

		level = atoi( token );	
		
	
		
		

		g_MaxDefneseTable[expLevel].Attacklevel = atoi( token );
		token = NextToken();
		g_MaxDefneseTable[expLevel].MaxDefense = atoi( token );		
		expLevel++;		

		if ( expLevel >= MAX_NUMBER_OF_LEVEL )
			break;

	}

	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return( 1 );
}


#include "../GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;





void PkTimeAutoProcess()
{	
	if ( g_config.isManager == false)
		return;

	if ( gcpGMCtrl == NULL)
		return;

	if ( gcpGMCtrl->m_cpPKTimeCtrl == NULL)
		return;

	if ( gcpGMCtrl->m_cpPKTimeCtrl->m_cpVirtualServerStateDataCtrl == NULL)
		return;
	
	
	struct tm* pNowTime = NULL;
	time_t ltime;
	time(&ltime);
	pNowTime = localtime(&ltime);
	if ( pNowTime == NULL)
		return;

	int DayNight = FALSE;
	if ( pNowTime->tm_hour >= 12)
	{
		DayNight = TRUE;
		pNowTime->tm_hour -= 12;
	}

 
}


