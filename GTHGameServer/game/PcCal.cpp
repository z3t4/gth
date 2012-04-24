#include "../global.h"
extern i3socket_t gsSocket;

#include "../Tools/CTools.h"
extern CTools* gcpTools;



void GTH_PC_CalcItemWeightPoint(playerCharacter_t* in_pPlayer);








int	lastSpawnPCIndex = -1;

int GTH_GetSpawnPCIndex()
{
	int		i;
	int		idx;

	if( lastSpawnPCIndex < 0 )
	{
		lastSpawnPCIndex = 0;
	}
	else
	{
		lastSpawnPCIndex ++;
		if( lastSpawnPCIndex >= MAX_PCS ) lastSpawnPCIndex = 0;
	}

	idx = lastSpawnPCIndex;
	i =	0;

	while( 1 )
	{
		if( i >= MAX_PCS ) break;
		if( !g_pc[idx].active ) break;
		
		idx ++;
		if( idx >= MAX_PCS ) idx = 0;

		i ++;
	}

	
	if( i >= MAX_PCS ) return -1;
	lastSpawnPCIndex = idx;
	
	return idx;
}

int GTH_SpawnPC(struct sockaddr_in *addr)
{
	int i, j;
	playerCharacter_t *pc;
	
	

 
	i = GTH_GetSpawnPCIndex();
	if( i < 0 )
	{
		g_logSystem->Write("Critical Error!! : PC over Max PC Number");
		g_logSystem->Update();
		return -1;
	}
	pc = &g_pc[i];

	g_pcNumber++;
	if ( g_pcNumber > g_maxPCNumber ) g_maxPCNumber = g_pcNumber;

	
	
	NET_InitPacketMap( &pc->sock );

	memset( pc, 0, sizeof( playerCharacter_t ) );
	pc->idx = i;
	pc->active = true;
	pc->ready = false;
	pc->alive = true;
	
	
	
	pc->sendReplyConnect = false;

	
	pc->flagDBLoading = false;
	pc->normalNetwork = true;
	
	



	memset( pc->userID, 0, sizeof( pc->userID ) ); 
	pc->pcClass = 0;
	pc->entityType = ENTITY_PC;
	pc->rankPoint = 0;
	
	vec_set(pc->position, 0, 0, 0);
	vec_set(pc->angles, 0, 0, 0);

	pc->event = GTH_EV_CHAR_IDLE;

	pc->flagChangeServer = -1;
	pc->changeServerWorldIdx = -1;

	
	NET_InitSocket(&pc->sock, addr, gsSocket.socket, SocketType_Reliable);
	pc->syncTime = g_globalTime + g_config.gameSyncTime;

	
	pc->moveType = 0;
	pc->worldIdx = 0;
	pc->zoneIdx = 0;
	vec_set( pc->reliablePosition, 0, 0, 0);
	pc->reliableWorldIdx = -1;
	
	
	pc->level = 1;
	pc->exp = 0;
	pc->nextExp = 0;
	pc->increaseExpRate = 0;
	pc->genLevel = 0;
	pc->genExp = 0;
	pc->nextGenExp = 0;
	pc->increaseGenExpRate = 0;
	for( j = 0; j < GEN_NUMBER; j ++ )
	{
		pc->genCapability[j] = 0;
	}
	pc->force = 0;
	pc->soulpower = 0;
	pc->agility = 0;
	pc->vitality = 0;

	pc->calForce = 0;
	pc->calSoulpower = 0;
	pc->calAgility = 0;
	pc->calVitality = 0;

	pc->bonusStatusPoint = 0;

	
	pc->curRA = 0.0f;
	pc->curSA = 0.0f;
	pc->baseMaxRA = 0.0f;
	pc->baseMaxSA = 0.0f;
	pc->calMaxRA = 0.0f;
	pc->calMaxSA = 0.0f;

	pc->maxChargeRA = 0;
	pc->maxChargeSA = 0;
	pc->onceRecoveryRA = 0;
	pc->onceRecoverySA = 0;

	
	pc->curWeapon = 0;
	pc->chaosPoint = 0;

	pc->targetingType = TARGETING_NORMAL;

	pc->targetType = -1;
	pc->targetIdx = -1;

	pc->basePhyMinDmg = 0;
	pc->basePhyMaxDmg = 0;
	pc->baseMagMinDmg = 0;
	pc->baseMagMaxDmg = 0;
	pc->baseHitPower = 0;
	pc->baseAttackSpeed = 0;
	pc->baseAttackLength = 0;
	pc->baseAttackRange = 0;

	pc->calPhyMinDmg = 0;
	pc->calPhyMaxDmg = 0;
	pc->calMagMinDmg = 0;
	pc->calMagMaxDmg = 0;
	pc->calHitPower = 0;
	pc->calAttackDelay = 0;
	pc->attackSpeedRate = 0;
	pc->calHitRate = 0;

	
	pc->baseDefensePower = 0;
	pc->baseVelocity = 0.0f;
	pc->baseStunTime = 0;
	pc->baseAvoidPower = 0;

	pc->calDefensePower = 0;
	pc->calPositiveRes = 0;
	pc->calNegativeRes = 0;
	pc->calAvoidPower = 0;
	pc->calVelocity = 0.0f;
	pc->calStunTime = 0;
	pc->bautoeffectisvalid=false;

	
	
	GTH_DeleteAffectAll( pc, ENTITY_PC );


	
	for( j = 0; j < MAX_NUMBER_OF_SKILL; j ++ )
	{
		pc->skill[j].idx = j;
		pc->skill[j].tableIdx = -1;
		pc->skill[j].iLevel = 0;		
		pc->skillCooltime[j] = 0.0f;
	}
	pc->skillCastingtime = 0.0f;
	pc->skillCastingState = true;
	pc->saveSkillIdx = -1;

	for ( j = 0; j < MAX_NUMBER_OF_SKILL_VARIABLE; j ++ )
		pc->skillVariable[j] = -1;

	pc->skillNumber = 0;
	pc->bonusSkillPoint = 0;

	
	pc->itemNumber = 0;
	for( j = 0; j < MAX_NUMBER_OF_OWNITEM; j ++ ) InitItem( &pc->item[j] );
	
	for( j = 0; j < MAX_INVENTORY_SIZE; j ++ ) pc->inventory[j] = -1;
	for( j = 0; j < MAX_DEPOT_SIZE; j ++ ) pc->depot[j] = -1;
	for( j = 0; j < MAX_EQUIPMENT; j ++ ) pc->equipment[j] = -1;
	pc->mouseInventory = -1;

	
	for( j = 0; j < MAX_PRECOCITY_SIZE; j ++ ) 
	{
		pc->precocityInventory[j] = -1;
		pc->precocityTime[j] = 0;
	}

	pc->curItemWeight = 0;
	pc->baseItemWeight = 0;
	pc->calItemWeight = 0;

	
	for( i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
	{
		pc->questInventory[i].iItem = 65535;
	}
	for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		pc->questStatus[i].idx = i;
		pc->questStatus[i].questIdx=-1;
		pc->questStatus[i].limitTime=-1;
		memset(pc->questStatus[i].partyOrganizerName,0,sizeof(char)*NAMESTRING);
		pc->questStatus[i].partyOrganizeServer=-1;
		pc->questStatus[i].partyOrganizeTime=-1;
	
		memset(pc->questStatus[i].dField, -1, sizeof(int)*MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD );
	}
	for( i = 0; i < MAX_COUNT_QUEST_VARIABLE; i ++ )
		pc->questVar[i].iVar = 65280;

	
	GTH_InitTrade( pc );
	
	memset( &pc->organizerName, 0, sizeof( pc->organizerName ) );
	pc->organizeServer = -1;
	pc->organizeTime = -1;
	pc->partyIndex = -1;
	pc->memberNumber = 0;

	pc->canJoin = true;
	pc->isLeader = false;
	pc->partyExp = 0;
	pc->partyGenExp = 0;
	pc->isSharingParty = false;
	
	pc->_partyRandomInfo_t.itemDivideType = 0;
	pc->_partyRandomInfo_t.m_bischange =false;
	
	
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		memset( &pc->member[i], 0, sizeof( partyMember_t ) );
		pc->member[i].serverIndex = -1;
	}

	
	AI_InitMessage( pc );
	pc->isTransform = false;

	
	pc->isSetServerShutdown = false;
	pc->gameMaster = 0;
	pc->nextSaveTime = g_globalTime + PC_GAMEDATA_SAVETIME;
	pc->npcIdx = -1;
	pc->autoRecoveryRATime = 0;
	pc->autoRecoverySATime = 0;
	pc->gmBusy = 0;
	pc->gmHide = 0;
	GTH_DeleteNoticePC( pc );
	pc->serverAckMsgNeedFlag = false;
	pc->serverAckMsgPartyFlag = false;
	pc->recoveryTime = 1000.0f;
	pc->pvpMode = false;

	
	pc->GonryunBattlePractice.MemberShip = playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE;
	pc->GonryunBattlePractice.bisWaitrespond = FALSE;
	
	pc->forceAtkDelay = -1;
    
	

	pc->GM.Chat.RemainBanTimePerSecond=0;

	
	pc->m_CashMall_OptionStone.InitCashMallItem_OptionStone();

	
	
	
	g_NewDanBattleSystem.InitDanBatlleInfo(&(pc->DanBattleInfo));

	
	for (int ShareWinInventoryIdx =0; ShareWinInventoryIdx < \
		playerCharacter_t::tagGonryunBattle::tagShareWin::MAX_INVENTORY_NUMBER; 
		ShareWinInventoryIdx++)	
	pc->GonryunBattle.ShareWin.Inventory[ShareWinInventoryIdx] = -1;

	
	
	InitItemCraftSystem(pc);	
	InitItemUpgradeSystem(pc);
	InitBoothSystem(pc);	
	InitPrecocitySystem(pc);

	
	pc->fWorldChangeStartTime = 0.0f;
	pc->iChangeWorldIdx = -1;

	return( pc->idx );
}

int GTH_RemovePC(playerCharacter_t *pc)
{
	
	

	if ( !pc->active ) return false;

	
	GTH_Zone_DeleteEntityList(pc->worldIdx, pc->zoneIdx, ENTITY_PC, pc->idx);

	pc->active = false;
	pc->ready = false;
	pc->alive = false;
	
	
	memset( &pc->organizerName, 0, sizeof( pc->organizerName ) );
	pc->organizeServer = -1;
	pc->organizeTime = -1;

	g_pcNumber --;

	return true;
}





void GTH_ApplyOption( playerCharacter_t *pc )
{
	
	
	
	
	

	
	
	pc->calCriticalDmg += (int)(pc->optValue[OV_CRITICAL_DMG]);
	

	
	
	pc->calCriticalRate += pc->optValue[OV_CRITICAL_PRO] * 0.01f;
		
	
	pc->calPositiveRes += (int)pc->optValue[OV_POSRES];
	pc->calNegativeRes += (int)pc->optValue[OV_NEGRES];
	
	

	

	pc->calPhyMinDmg += (int)( (float)pc->basePhyMinDmg * pc->optValue[OV_MIN_PHYDMG_RATE] * 0.01f );
	pc->calPhyMaxDmg += (int)( (float)pc->basePhyMaxDmg * pc->optValue[OV_MAX_PHYDMG_RATE] * 0.01f );	
	pc->calMagMinDmg += (int)( (float)pc->baseMagMinDmg * pc->optValue[OV_MIN_MAGDMG_RATE] * 0.01f );
	pc->calMagMaxDmg += (int)( (float)pc->baseMagMaxDmg * pc->optValue[OV_MAX_MAGDMG_RATE] * 0.01f );

	pc->calPhyMinDmg += (int)( (float)pc->basePhyMinDmg * pc->optValue[OV_PHYDMG_RATE] * 0.01f );
	pc->calPhyMaxDmg += (int)( (float)pc->basePhyMaxDmg * pc->optValue[OV_PHYDMG_RATE] * 0.01f );	
	pc->calMagMinDmg += (int)( (float)pc->baseMagMinDmg * pc->optValue[OV_MAGDMG_RATE] * 0.01f );
	pc->calMagMaxDmg += (int)( (float)pc->baseMagMaxDmg * pc->optValue[OV_MAGDMG_RATE] * 0.01f );

	
	pc->calMaxRA += pc->optValue[OV_MAX_RA];
	pc->calMaxSA += pc->optValue[OV_MAX_SA];

	
	
	
	pc->calDefensePower += (int)pc->optValue[OV_DEFPOWER];

	
	pc->calPhyMinDmg += (int)pc->optValue[OV_MIN_PHYDMG] + (int)pc->optValue[OV_PHYDMG];
	pc->calPhyMaxDmg += (int)pc->optValue[OV_MAX_PHYDMG] + (int)pc->optValue[OV_PHYDMG];

	
	pc->calMagMinDmg += (int)pc->optValue[OV_MIN_MAGDMG] + (int)pc->optValue[OV_MAGDMG];
	pc->calMagMaxDmg += (int)pc->optValue[OV_MAX_MAGDMG] + (int)pc->optValue[OV_MAGDMG];
}


















enum
{	
	DivideValue_MaxSA,						
	DivideValue_PhyDamage_Except_Wind,		
	DivideValue_PhyDamage_Wind_By_Bow,		
	DivideValue_PhyDamage_Wind_By_Dagger,	
	DivideValue_MagDamage,					
	DivideValue_HitPower,					
	DivideValue_AvoidPower,					
};

int PC_INITIAL_STATUS[8][4]	=
{
	{	37,		11,		30,		22	},
	{	22,		19,		44,		15	},
	{	20,		44+1,		24,		11	},
	{	33,		12,		24,		31	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
};

#define LimitStatusLevel_BeforeAwaken	29
#define LimitStatusLevel_FirstAwaken	69
#define LimitStatusLevel_LastAwaken		99


int PC_LIMIT_STATUS_BEFORE_AWAKEN[8][4]	=
{
	
	{	80,		44,		70,		62	}, 
	{	55,		59,		87,		55	}, 
	{	53,		87+1,		64,		52	}, 
	{	75,		45,		59,		73	}, 
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
};
int PC_LIMIT_STATUS_FIRST_AWAKEN[8][4]	=
{
	
	{	140,	96,    131,	  124	},
	{	107,   121,	   147,	  116	},
	{	105,   147+1,	   125,	  114	},
	{	133,	97,	   127,	  137	},
	{	0,		 0,		 0,		0	},
	{	0,		 0,		 0,		0	},
	{	0,		 0,		 0,		0	},
	{	0,		 0,		 0,		0	},
};
int PC_LIMIT_STATUS_LAST_AWAKEN[8][4]	=
{
	
	{	216,	131,	174,	167	},
	{	142,	164,	223,	159	},
	{	140,	223+1,	168,	157	},
	{	212,	132,	169,	181	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
};


int CheckLimitStatus( int level, int pcJob, int status, int statusType )
{
	int		*limitStatus;	

	
	if( level <= LimitStatusLevel_BeforeAwaken ) limitStatus = PC_LIMIT_STATUS_BEFORE_AWAKEN[pcJob];
	else if( level <= LimitStatusLevel_FirstAwaken ) limitStatus = PC_LIMIT_STATUS_FIRST_AWAKEN[pcJob];
	else if( level <= LimitStatusLevel_LastAwaken ) limitStatus = PC_LIMIT_STATUS_LAST_AWAKEN[pcJob];

	


	if( status >= limitStatus[statusType] ) return false;

	return true;
}

playerCharacter_t	*pCalcPC;

float GetStausWeight( playerCharacter_t *pc, int statusType )
{
	switch( statusType )
	{
	case STATUS_FORCE :
		return( (float)pc->calForce );
		break;
	case STATUS_SOULPOWER :
		return( (float)pc->calSoulpower );
		break;
	case STATUS_AGILITY :
		return( (float)pc->calAgility );
		break;
	case STATUS_VITALITY :
		return( (float)pc->calVitality );
		break;
	}
	return( 0.0f );
}


float GetBaseMaxRA()
{
	int		pcJob	=	pCalcPC->pcJob;
	int		level	=	pCalcPC->level;
	float	vital	=	(float)pCalcPC->calVitality;

	float	ret;
	float	tvalue1;
	float	tvalue2;

	if( pcJob == J_MENTALITY || pcJob == J_DEVOTIONAL_KNIGHT || pcJob == J_ROMING_KNIGHT )
	{
		tvalue1	=	vital	*	vital	-	200.0f;
		tvalue2	=	powf( vital, 0.15f );
		ret		=	tvalue1	/	tvalue2;
	}
	else if( pcJob == J_EARTH || pcJob == J_TRANSFORMER || pcJob == J_STRANGER )
	{
		






		tvalue1	=	vital	*	vital	-	500.0f;
		tvalue2	=	powf( vital, 0.18f );
		ret		=	tvalue1	/	tvalue2;
	}
	else if( pcJob == J_DEFENDER )			
	{
		tvalue1	=	vital	*	vital;
		tvalue2	=	powf( vital, 0.15f );
		ret		=	tvalue1	/	tvalue2;
	}
	else if( pcJob == J_WIND || pcJob == J_PIECING_EYES || pcJob == J_WINDY_ROMER )
	{
		tvalue1	=	vital	*	vital;
		tvalue2	=	powf( vital, 0.2f );
		ret		=	tvalue1	/	tvalue2;
	}
	else if( pcJob == J_WATER || pcJob == J_DESTROYER || pcJob == J_PROPHECIER )
	{





		
		tvalue1	=	vital	*	vital	+	100.0f;
		tvalue2	=	powf( vital, 0.30f );
		ret		=	tvalue1	/	tvalue2;
	}
	else if( pcJob == J_GUARDIAN )
	{
		tvalue1	=	vital	*	vital;
		tvalue2	=	powf( vital, 0.2f );
		ret		=	tvalue1	/	tvalue2;
	}

	return( ret );
}

float GetDivideValueInBaseFormula( int valueType )
{
	switch( valueType )
	{
	case DivideValue_MaxSA :
		{
			return( 6.0f );
		}
		break;
	case DivideValue_PhyDamage_Except_Wind :
		{
			return( 10.0f * 8.0f );
		}
		break;
	case DivideValue_PhyDamage_Wind_By_Bow :
		{
			return( 10.0f * 8.0f );
		}
		break;
	case DivideValue_PhyDamage_Wind_By_Dagger :
		{
			return( 10.0f * 12.0f );
		}
		break;
	case DivideValue_MagDamage :
		{
			return( 10.0f * 20.0f );
		}
		break;
	case DivideValue_HitPower :
		{
			return( 20.0f * 12.0f );
		}
		break;
	case DivideValue_AvoidPower :
		{
			return( 20.0f * 12.0f );
		}
		break;
	}
	return( 1.0f );
}

void GTH_PC_InitPCPoint()
{
	pCalcPC->baseItemPhyMinDmg		=	0;
	pCalcPC->baseItemPhyMaxDmg		=	0;
	pCalcPC->baseItemMagMinDmg		=	0;
	pCalcPC->baseItemMagMaxDmg		=	0;
	pCalcPC->baseItemCriticalDmg	=	0;
	pCalcPC->baseItemCriticalRate	=	0;
	pCalcPC->baseAttackSpeed		=	0;
	pCalcPC->atkBowSACost			=	0;
	pCalcPC->baseDefensePower		=	1;
	pCalcPC->baseItemDefensePower	=	0;
	pCalcPC->baseItemPositiveRes	=	0;
	pCalcPC->baseItemNegativeRes	=	0;
	pCalcPC->baseVelocity			=	0.0f;
	pCalcPC->baseItemHitPower		=	0;
	pCalcPC->baseItemAvoidPower		=	0;
	
	pCalcPC->baseAttackLength		=	60;
	pCalcPC->baseAttackRange		=	0;
	pCalcPC->curItemWeight			=	0;
	pCalcPC->generalSpeed			=	100;
	pCalcPC->atkWeaponType			=	WEAPON_TYPE_ONEHAND_SWORD;
	
	
	pCalcPC->attackSpeedRate		=	0;
}

void GTH_PC_CalcItemAttackPoint()
{
	if( !g_itemClassTable[g_optItem.classIdx].attackAtt ) return;

	
	pCalcPC->baseItemPhyMinDmg		+=	g_optItem.atkPhyMinDmg;
	pCalcPC->baseItemPhyMaxDmg		+=	g_optItem.atkPhyMaxDmg;
	pCalcPC->baseItemMagMinDmg		+=	g_optItem.atkMagMinDmg;
	pCalcPC->baseItemMagMaxDmg		+=	g_optItem.atkMagMaxDmg;
	pCalcPC->baseItemHitPower		+=	g_optItem.atkHitPower;
	pCalcPC->baseItemCriticalDmg	+=	g_optItem.atkCriticalDmg;
	pCalcPC->baseItemCriticalRate	+=	g_optItem.atkCriticalRate;
	pCalcPC->baseAttackSpeed		+=	g_optItem.atkDelay;
	pCalcPC->atkBowSACost			+=	g_optItem.atkBowSACost;
}

void GTH_PC_CalcItemDefensePoint()
{
	if( !g_itemClassTable[g_optItem.classIdx].defenseAtt ) return;

	
	pCalcPC->baseItemDefensePower	+=	g_optItem.defPower;
	pCalcPC->baseItemAvoidPower		+=	g_optItem.defAvoidPower;
	pCalcPC->baseItemPositiveRes	+=	g_optItem.defPositiveRes;
	pCalcPC->baseItemNegativeRes	+=	g_optItem.defNegativeRes;
	pCalcPC->baseItemHitPower		+=	g_optItem.defHitPower;
	pCalcPC->baseVelocity			+=	g_optItem.defVelocity	*	0.01f;
}

void GTH_PC_CalcItemEpackPoint()
{
	if(g_optItem.classIdx < 0 || g_optItem.classIdx >= MAX_NUMBER_OF_ITEM_CLASS_TABLE) return;
	if( !g_itemClassTable[g_optItem.classIdx].packAtt ) return;

	int epackIdx;
	int totalRA, totalSA;

	
	epackIdx = pCalcPC->equipment[EQUIP_EPACK];
	if( epackIdx < 0 )
	{
		pCalcPC->maxChargeRA		=	0;
		pCalcPC->maxChargeSA		=	0;
		pCalcPC->curChargeRA		=	0;
		pCalcPC->curChargeSA		=	0;
		pCalcPC->onceRecoveryRA		=	0;
		pCalcPC->onceRecoverySA		=	0;
		return;
	}
	if( !GTH_CheckRequire( pCalcPC, 0, &pCalcPC->item[epackIdx] ) ) 
	{
		pCalcPC->maxChargeRA		=	0;
		pCalcPC->maxChargeSA		=	0;
		pCalcPC->curChargeRA		=	0;
		pCalcPC->curChargeSA		=	0;
		pCalcPC->onceRecoveryRA		=	0;
		pCalcPC->onceRecoverySA		=	0;
		return;
	}

	
	OPTION_ApplyItem( &pCalcPC->item[epackIdx] );

	
	totalRA				=	pCalcPC->curChargeRA	*	pCalcPC->onceRecoveryRA;
	totalSA				=	pCalcPC->curChargeSA	*	pCalcPC->onceRecoverySA;

	pCalcPC->maxChargeRA		=	g_optItem.epkMaxChargeRA;
	pCalcPC->maxChargeSA		=	g_optItem.epkMaxChargeSA;
	pCalcPC->onceRecoveryRA		=	g_optItem.epkRecoveryRA;
	pCalcPC->onceRecoverySA		=	g_optItem.epkRecoverySA;

	
	if( totalRA > 0 )
	{
		if( pCalcPC->onceRecoveryRA > 0 ) 
		{
			
			pCalcPC->curChargeRA = (float)totalRA / (float)pCalcPC->onceRecoveryRA;
		}
		else pCalcPC->curChargeRA = 0;

		if( pCalcPC->curChargeRA > pCalcPC->maxChargeRA ) pCalcPC->curChargeRA = pCalcPC->maxChargeRA;
	}
	if( totalSA > 0 )
	{
		if( pCalcPC->onceRecoverySA > 0 ) 
		{
			
			pCalcPC->curChargeSA = (float)totalSA / (float)pCalcPC->onceRecoverySA;
		}
		else pCalcPC->curChargeSA = 0;
		if( pCalcPC->curChargeSA > pCalcPC->maxChargeSA ) pCalcPC->curChargeSA = pCalcPC->maxChargeSA;
	}
}



void GTH_PC_CalcStatus()
{
	int i, itemIdx;
	item_t *item;
	int status[4] = { 0, 0, 0, 0 };
	
	
	for( i = EQUIP_WEAPON_1; i < MAX_EQUIPMENT; i ++ )
	{
		if( i == EQUIP_WEAPON_1 && pCalcPC->curWeapon == 1 ) continue;
		if( i == EQUIP_WEAPON_2 && pCalcPC->curWeapon == 0 ) continue;
		if( pCalcPC->equipment[i] < 0 ) continue;

		itemIdx = pCalcPC->equipment[i];
		item = &pCalcPC->item[itemIdx];

		if( !GTH_CheckRequire( pCalcPC, 0, item ) ) continue;
		OPTION_ApplyStatusOption( item, status );
	}

	pCalcPC->calForce		=	pCalcPC->force		+	status[0];
	pCalcPC->calSoulpower	=	pCalcPC->soulpower	+	status[1];
	pCalcPC->calAgility		=	pCalcPC->agility	+	status[2];
	pCalcPC->calVitality	=	pCalcPC->vitality	+	status[3];
}

void GTH_PC_CalcItemPoint()
{
	int i, itemIdx;
	item_t *item;

	
	for( i = EQUIP_WEAPON_1; i < MAX_EQUIPMENT; i ++ )
	{
		if( i == EQUIP_WEAPON_1 && pCalcPC->curWeapon == 1 ) continue;
		if( i == EQUIP_WEAPON_2 && pCalcPC->curWeapon == 0 ) continue;
		if( pCalcPC->equipment[i] < 0 ) continue;

		itemIdx	=	 pCalcPC->equipment[i];
		item	=	&pCalcPC->item[itemIdx];
		
		if( !GTH_CheckRequire( pCalcPC, 0, item ) ) continue;
		OPTION_ApplyItem( item );
		GTH_PC_CalcItemAttackPoint();

		if( i == pCalcPC->curWeapon )
		{
			
			pCalcPC->baseAttackLength		=	g_optItem.atkLength;
			pCalcPC->baseAttackRange		=	g_optItem.atkRange;
			pCalcPC->atkWeaponType			=	g_itemClassTable[g_itemTable[item->itemTableIdx].classIdx].weaponType;
		}
	}
	
	for( i = EQUIP_WEAPON_1; i < MAX_EQUIPMENT; i ++ )
	{
		if( i == EQUIP_WEAPON_1 && pCalcPC->curWeapon == 1 ) continue;
		if( i == EQUIP_WEAPON_2 && pCalcPC->curWeapon == 0 ) continue;
		if( pCalcPC->equipment[i] < 0 ) continue;

		itemIdx	=	pCalcPC->equipment[i];
		item	=	&pCalcPC->item[itemIdx];
		
		if( !GTH_CheckRequire( pCalcPC, 0, item ) ) continue;
		OPTION_ApplyItem( item );
		GTH_PC_CalcItemDefensePoint();
	}
	
	GTH_PC_CalcItemEpackPoint();
	
	GTH_PC_CalcItemWeightPoint(pCalcPC);
}

void GTH_PC_CalcOptionPoint()
{
	item_t *item;
	int i, itemIdx;

	OPTION_InitGlobal();
	for( i = EQUIP_WEAPON_1; i < MAX_EQUIPMENT; i ++ )
	{
		if( i == EQUIP_WEAPON_1 && pCalcPC->curWeapon == 1 ) continue;
		if( i == EQUIP_WEAPON_2 && pCalcPC->curWeapon == 0 ) continue;
		if( pCalcPC->equipment[i] < 0 ) continue;

		itemIdx	=	pCalcPC->equipment[i];
		item	=	&pCalcPC->item[itemIdx];
		
		if( !GTH_CheckRequire( pCalcPC, 0, item ) ) continue;
		OPTION_ApplyItemOption( item );
	}
	memcpy( pCalcPC->optValue, g_optValue, sizeof( g_optValue ) );
	OPTION_CheckOptionValue( pCalcPC->optValue );
}

void GTH_PC_CalcBasePoint()
{
	float tvalue1, tvalue2;

	GTH_PC_CalcStatus();
	GTH_PC_InitPCPoint();
	GTH_PC_CalcItemPoint();
	GTH_PC_CalcOptionPoint();

	
	pCalcPC->baseMaxRA		=	GetBaseMaxRA();

	tvalue1				=	(float)pCalcPC->calSoulpower;
	tvalue2				=	GetDivideValueInBaseFormula( DivideValue_MaxSA );

	pCalcPC->baseMaxSA	=	( tvalue1 * tvalue1 )	/	tvalue2;
	
	pCalcPC->calMaxRA	=	pCalcPC->baseMaxRA;
	pCalcPC->calMaxSA	=	pCalcPC->baseMaxSA;


	
	switch(pCalcPC->pcClass){
		case GEN_WIND: 
			pCalcPC->basePhyMaxDmg=
				pCalcPC->basePhyMinDmg=	
					(int)(( pCalcPC->calAgility * pCalcPC->calAgility )	/	(10.0* 16));
			
			break;

		default:
		case GEN_MENTALITY:
		case GEN_WATER:
		case GEN_EARTH:

			



			

			pCalcPC->basePhyMaxDmg=
				pCalcPC->basePhyMinDmg=	
					(int)(( pCalcPC->calForce * pCalcPC->calForce )	/	(10.0* 16));			

			
			

			break;

	}
	


	



	pCalcPC->calPhyMinDmg	=	pCalcPC->basePhyMinDmg	+	pCalcPC->baseItemPhyMinDmg;
	pCalcPC->calPhyMaxDmg	=	pCalcPC->basePhyMaxDmg	+	pCalcPC->baseItemPhyMaxDmg;

	
	tvalue1		=	(float)pCalcPC->calSoulpower;
	tvalue2		=	GetDivideValueInBaseFormula( DivideValue_MagDamage );

	pCalcPC->baseMagMinDmg	=	(int)(	( tvalue1 * tvalue1 )	/	tvalue2	);
	pCalcPC->baseMagMaxDmg	=	(int)(	( tvalue1 * tvalue1 )	/	tvalue2	);
	pCalcPC->calMagMinDmg	=	pCalcPC->baseMagMinDmg	+	pCalcPC->baseItemMagMinDmg;
	pCalcPC->calMagMaxDmg	=	pCalcPC->baseMagMaxDmg	+	pCalcPC->baseItemMagMaxDmg;

	
	pCalcPC->baseDefensePower	+=	pCalcPC->baseItemDefensePower;
	pCalcPC->calDefensePower	=	pCalcPC->baseDefensePower;

	
	tvalue1		=	100.0f + (float)pCalcPC->level - 1.0f;
	tvalue1		=	( tvalue1 * tvalue1 )	+	( (float)pCalcPC->calAgility / 4.0f );
	tvalue2		=	GetDivideValueInBaseFormula( DivideValue_HitPower );
	
	pCalcPC->baseHitPower	=	(int)( tvalue1	/ tvalue2 ) + pCalcPC->baseItemHitPower;
	pCalcPC->calHitPower	=	pCalcPC->baseHitPower;

	
	tvalue1		=	(float)pCalcPC->calAgility;
	tvalue2		=	GetDivideValueInBaseFormula( DivideValue_AvoidPower );
	
	pCalcPC->baseAvoidPower	=	(int)( ( tvalue1 * tvalue1 ) / tvalue2 ) + pCalcPC->baseItemAvoidPower;
	pCalcPC->calAvoidPower	=	pCalcPC->baseAvoidPower;
	
	
	pCalcPC->basePositiveRes	=	pCalcPC->baseItemPositiveRes;
	pCalcPC->baseNegativeRes	=	pCalcPC->baseItemNegativeRes;
	pCalcPC->calPositiveRes		=	pCalcPC->basePositiveRes;
	pCalcPC->calNegativeRes		=	pCalcPC->baseNegativeRes;

	
	pCalcPC->baseCriticalDmg	=	pCalcPC->baseItemCriticalDmg;
	pCalcPC->baseCriticalRate	=	pCalcPC->baseItemCriticalRate;
	pCalcPC->calCriticalDmg		=	pCalcPC->baseCriticalDmg;
	pCalcPC->calCriticalRate	=	pCalcPC->baseCriticalRate;

	
	pCalcPC->baseVelocity		+=	2.4f + 0.005f * (float)pCalcPC->calAgility;
	pCalcPC->calVelocity		=	pCalcPC->baseVelocity;

	
	pCalcPC->baseStunTime		=	(int)(	800.0f	-	(	(float)pCalcPC->calVitality	*	2.5f	)	);
	pCalcPC->calStunTime		=	pCalcPC->baseStunTime;

	
	pCalcPC->baseItemWeight		=	(int)(	(float)pCalcPC->calVitality	* 100.0f );
	pCalcPC->calItemWeight		=	pCalcPC->baseItemWeight;

	

	



	
	pCalcPC->calAttackLength	=	pCalcPC->baseAttackLength;

	
	
	g_SkillLogic.SKILL_ApplyPassive( pCalcPC );

	
	GTH_ApplyOption( pCalcPC );
	
	GTH_ApplyAffect( pCalcPC );

	
	pCalcPC->calAttackDelay		=	(int)( 2500.0f	-	(float)pCalcPC->baseAttackSpeed );

	pCalcPC->calAttackDelay		=	(int)(	(float)pCalcPC->calAttackDelay	/ \
									(	(	100.0f	+	(float)pCalcPC->attackSpeedRate	)	/	100.0f )	);	

	
	if(	pCalcPC->calAttackDelay	<	500.0f	)	pCalcPC->calAttackDelay	=	500.0f;
	if(	pCalcPC->calStunTime	<	100		)	pCalcPC->baseStunTime	=	100;
	if( pCalcPC->calPositiveRes	>	75.0f	)	pCalcPC->calPositiveRes	=	75.0f;
	if( pCalcPC->calPositiveRes	<	-100.0f	)	pCalcPC->calPositiveRes	=	-100.0f;
	if( pCalcPC->calNegativeRes	>	75.0f	)	pCalcPC->calNegativeRes	=	75.0f;
	if( pCalcPC->calNegativeRes	<	-100.0f	)	pCalcPC->calNegativeRes	=	-100.0f;
	
	
	
	if( pCalcPC->calPhyMinDmg	>	pCalcPC->calPhyMaxDmg	)	pCalcPC->calPhyMinDmg	=	pCalcPC->calPhyMaxDmg;
	if( pCalcPC->calMagMinDmg	>	pCalcPC->calMagMaxDmg	)	pCalcPC->calMagMinDmg	=	pCalcPC->calMagMaxDmg;
}


void GTH_PC_Calculate( playerCharacter_t* pc, int type )
{
	pCalcPC	=	pc;

	GTH_PC_CalcBasePoint();

	if( pc->curRA > pc->calMaxRA ) pc->curRA = pc->calMaxRA;
	if( pc->curSA > pc->calMaxSA ) pc->curSA = pc->calMaxSA;

	GTH_SendMessage_Reply_PCInfo( pc );
}








void GTH_PC_CalcItemWeightPoint(playerCharacter_t* in_pPlayer)
{
	if(NULL == in_pPlayer) return;	
	item_t*			pItemRecord=NULL;
	itemTable_t*	pItemTableRecord=NULL;

	
	for(int nEquipmentArrayIdx=0;  nEquipmentArrayIdx < MAX_EQUIPMENT; nEquipmentArrayIdx++){	
		pItemRecord = gcpTools->GetMyItem(in_pPlayer,in_pPlayer->equipment[nEquipmentArrayIdx]);
		if(NULL == pItemRecord) continue;

		pItemTableRecord = gcpTools->GetItemTablePointer(pItemRecord->itemTableIdx);
		if(NULL == pItemTableRecord) continue;
		
		in_pPlayer->curItemWeight += pItemTableRecord->weight;
	}


	for(int nInventoryArrayIdx=0; nInventoryArrayIdx < MAX_INVENTORY_SIZE; nInventoryArrayIdx++){
		pItemRecord = gcpTools->GetMyItem(in_pPlayer,in_pPlayer->inventory[nInventoryArrayIdx]);
		if(NULL == pItemRecord) continue;

		pItemTableRecord = gcpTools->GetItemTablePointer(pItemRecord->itemTableIdx);
		if(NULL == pItemTableRecord) continue;

		in_pPlayer->curItemWeight += pItemTableRecord->weight;		
	}


	for(int nTradeBoxArrayIdx=0; nTradeBoxArrayIdx < MAX_TRADE_SIZE; nTradeBoxArrayIdx++){
		pItemRecord = gcpTools->GetMyItem(in_pPlayer,in_pPlayer->myTrade[nTradeBoxArrayIdx]);
		if(NULL == pItemRecord) continue;

		pItemTableRecord = gcpTools->GetItemTablePointer(pItemRecord->itemTableIdx);
		if(NULL == pItemTableRecord) continue;

		in_pPlayer->curItemWeight += pItemTableRecord->weight;
	}


	if( in_pPlayer->mouseInventory >= 0 ){

		pItemRecord = gcpTools->GetMyItem(in_pPlayer,in_pPlayer->mouseInventory);
		if(NULL == pItemRecord) return;

		pItemTableRecord = gcpTools->GetItemTablePointer(pItemRecord->itemTableIdx);
		if(NULL == pItemTableRecord) return;

		in_pPlayer->curItemWeight += pItemTableRecord->weight;		
	}	
}



