#include "../global.h"
extern i3socket_t gsSocket;

#include "..\GonryunPracticeBattle\CGonryunPracticeBattle.h"
#include "..\GonryunPracticeBattle\CGonryunPractice_ProcessCtrl.h"
#include "..\GonryunPracticeBattle\CGonryunPractice_WaiterCtrl.h"
extern CGonryunPracticeBattle	g_GonryunPracticeBattle;											



#include "..\CPremiumPlayerCtrl.h"
extern CPremiumPlayerCtrl* gcpPremiumPlayerCtrl;

#include "..\GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;

#include "..\GolryunBattle\CGolryunBattle_Log.h"
extern CGolryunBattle_Log* gcpGolryunBattleLog;


#include "..\Terra\Billing\CTerraBillingCtrl.h"
extern CTerraBillingCtrl* gcpTerraBillingCtrl;


extern CUseItem g_UseItem;


#include "../Tools/CTools.h"
extern CTools* gcpTools;
extern CItemTableData gItemTableData;



#include "..\CTools.h"
extern CTools* gcpTools;

#include "YDCDamageCal.h"


void GTH_PC_GetLoot(
	int idx, 
	__int64 exp, 
	__int64 genExp, 
	int lootNSE, 
	int genCapability, 
	int sharingType,
	BOOL bGoryunBattle,
	BOOL bUseItem)
{
	int upFlag = false;
	int level = 0, genLevel = 0, selectedGen;
	int skillIdx[50],skillNumber = 0;
	

	
	
	
	
	if(FALSE == bUseItem)
	{
			
		
		exp				= (__int64)( exp * g_logic.expRate );	
		genExp			= (__int64)( genExp	* g_logic.expRate );
		genCapability	= (int)( (float)genCapability	* g_logic.genCapabilityRate );	
			
	
		
		
		
		
		
		
		

		lootNSE			= (int)( (float)lootNSE			* g_logic.nseRate );		

		
		if((FALSE == bGoryunBattle) && (TRUE == gGameConfigFlag.m_bMap_Attr_Ratio))
		{
			PremiumMap	*pMap = g_pc[idx].GetCurMap();
			if(NULL != pMap)
			{
				exp					*= pMap->Cur.fAddExpRatio;
				genExp				*= pMap->Cur.fAddGenExpRatio;
				genCapability		*= pMap->Cur.fAddGenCapabilityRatio;
			}
		}	
	}
	

	
	

	
	
	if(FALSE == bUseItem)
	{




		
		exp			+= (__int64)( exp	 * g_pc[idx].optValue[OV_ADD_EXP_RATE] * 0.01f );
		genExp		+= (__int64)( genExp * g_pc[idx].optValue[OV_ADD_GENEXP_RATE] * 0.01f );
		lootNSE		+= (int)( (float)lootNSE		* g_pc[idx].optValue[OV_ADD_NAK_RATE] * 0.01f );
	}

	selectedGen = g_pc[idx].selectedGen;
	
	if( exp < 0 )			exp = 0;
	if( genExp < 0 )		genExp = 0;
	if( lootNSE < 0 )		lootNSE = 0;
	if( genCapability < 0 ) genCapability = 0;

	
	if( exp == 0 && genExp == 0 && lootNSE == 0 && genCapability == 0 ) return;
	
	
	GTH_PC_ExpUp( idx, exp, genExp, lootNSE, level, genLevel );

	if( genCapability > 0 && g_pc[idx].genCapability[selectedGen] >= 0 ) g_pc[idx].genCapability[selectedGen] += genCapability;

	g_pc[idx].curChargeSE += lootNSE;
	
	if( g_pc[idx].curChargeSE > MAX_OWN_NAK ) g_pc[idx].curChargeSE = MAX_OWN_NAK;

	if( level > g_pc[idx].level )
	{
		g_pc[idx].bonusStatusPoint += ( level - g_pc[idx].level );
		GTH_PC_LevelUp( &g_pc[idx], level );
		upFlag = true;
	}
	if( genLevel > g_pc[idx].genLevel )
	{

		GTH_PC_GenLevelUp( &g_pc[idx], genLevel );
		upFlag = true;
	}
	
	if( upFlag )
	{
		
		GTH_PC_Calculate( &g_pc[idx], PCINFO_BASIC );
		GTH_SendPCMessage_LevelUp( &g_pc[idx], g_pc[idx].exp, g_pc[idx].genExp, g_pc[idx].curChargeSE, g_pc[idx].genCapability[selectedGen], skillIdx, skillNumber, sharingType );

	}
	else
	{
		GTH_SendMessage_LootValue( &g_pc[idx], g_pc[idx].exp, g_pc[idx].genExp, g_pc[idx].curChargeSE, g_pc[idx].genCapability[selectedGen], sharingType );
	}
	
	
	
	if( ( 0 < exp) &&  ( 0 >= lootNSE ) )
	GTH_ProcessMessage_AutoQuestScroll_ObtainOpen(&g_pc[idx]);	
	
}




void GTH_PC_Idle(playerCharacter_t *pc)
{
	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_Idle( pc );
}

void GTH_PC_Walk(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);
	
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_Walk( pc );
}

void GTH_PC_WalkLeft(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	pc->angles[1] += TURN_SCALE * g_timer->GetElapsedMilliTime();

	if (pc->angles[1] >= 360.0f)
		pc->angles[1] -= 360.0f;

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_WalkLeft( pc );
}

void GTH_PC_WalkRight(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;
	
	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	pc->angles[1] -= TURN_SCALE * g_timer->GetElapsedMilliTime();

	if (pc->angles[1] < 0)
		pc->angles[1] += 360.0f;	

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_WalkRight( pc );
}

void GTH_PC_Back(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = -1 * BACK_SCALE * SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_Back( pc );
}

void GTH_PC_BackLeft(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = -1 * BACK_SCALE * SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	pc->angles[1] += TURN_SCALE * g_timer->GetElapsedMilliTime();

	if (pc->angles[1] >= 360.0f)
		pc->angles[1] -= 360.0f;

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_BackLeft( pc );
}

void GTH_PC_BackRight(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = -1 * BACK_SCALE * SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	pc->angles[1] -= TURN_SCALE * g_timer->GetElapsedMilliTime();


	if (pc->angles[1] < 0)
		pc->angles[1] += 360.0f;	

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_BackRight( pc );
}

void GTH_PC_MoveLeft(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] + 90 );
	Normalize( delta );

	speed = SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_MoveLeft( pc );
}

void GTH_PC_MoveRight(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] - 90 );
	Normalize( delta );

	speed = SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_MoveRight( pc );
}

void GTH_PC_Run(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = RUN_SCALE * SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_Run( pc );
}

void GTH_PC_RunLeft(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = RUN_SCALE * SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	pc->angles[1] += TURN_SCALE * g_timer->GetElapsedMilliTime();

	if (pc->angles[1] >= 360.0f)
		pc->angles[1] -= 360.0f;

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_RunLeft( pc );
}

void GTH_PC_RunRight(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = RUN_SCALE * SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	pc->angles[1] -= TURN_SCALE * g_timer->GetElapsedMilliTime();

	if (pc->angles[1] < 0)
		pc->angles[1] += 360.0f;	

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_RunRight( pc );
}

void GTH_PC_TurnLeft(playerCharacter_t *pc)
{
	pc->angles[1] += TURN_SCALE * g_timer->GetElapsedMilliTime();

	if (pc->angles[1] >= 360.0f)
		pc->angles[1] -= 360.0f;

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_TurnLeft( pc );
}

void GTH_PC_TurnRight(playerCharacter_t *pc)
{
	pc->angles[1] -= TURN_SCALE * g_timer->GetElapsedMilliTime();

	if (pc->angles[1] < 0)
		pc->angles[1] += 360.0f;	

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_TurnRight( pc );
}






















void GTH_PC_Die_ItemDrop( playerCharacter_t *pc, int& type, int& position )
{
	
	
	int		i, chaosClass, itemNumber, itemIdx;
	item_t *item, dropItem;
	
	chaosClass	= GetChaosClass( pc );
	
	
	if( GTH_Random() >= g_logic.dieItemLossRate[chaosClass] * 0.01f ) 
	{
		type = -1;
		position = 0;
		return;
	}
	
	
	if( GTH_Random() > 0.5f ) type = 0;
	else type = 1;

	
	if( !type )
	{
		itemNumber = 0;
		for( i = 0; i < NUM_EQUIP_POSITION; i ++ )
		{
			if( pc->equipment[i] < 0 ) continue;
			itemNumber ++;
		}
		
		if( itemNumber == 0 )
		{
			type = -1;
			position = 0;
			return;
		}
		while( 1 )
		{
			position = GTH_Randomize( NUM_EQUIP_POSITION );
			if( pc->equipment[position] < 0 ) continue;
			break;
		}
		itemIdx = pc->equipment[position];
		pc->equipment[position] = -1;
	}
	
	else
	{
		itemNumber = 0;
		for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
		{
			if( pc->inventory[i] < 0 ) continue;
			
			if( GTH_IsStackItem( &pc->item[pc->inventory[i]] ) ) continue;
			itemNumber ++;
		}
		
		if( itemNumber == 0 )
		{
			type = -1;
			position = 0;
			return;
		}
		while( 1 )
		{
			position = GTH_Randomize( MAX_INVENTORY_SIZE );
			if( pc->inventory[position] < 0 ) continue;
			
			if( GTH_IsStackItem( &pc->item[pc->inventory[position]] ) ) continue;
			break;
		}
		itemIdx = pc->inventory[position];
		pc->inventory[position] = -1;
	}

	item = &pc->item[itemIdx];
	memcpy( &dropItem, item, sizeof( item_t ) );
	GTH_DeleteMyItem( pc, item );
	g_DBGameServer->RemoveItem( pc, itemIdx );

	int itemObjectIdx;
	itemObjectIdx = GTH_SpawnItemObject(&dropItem, pc->worldIdx, pc->position, LOOTTYPE_EVERYONE, pc->idx);
	if( itemObjectIdx < 0 )
	{
		char szTemp[256];
		sprintf( "아이템드롭(죽음) : %s(%s), [%d,%d], %s아이템(%d) 스폰오류", 
			pc->name, pc->userID, type, position, dropItem.name, dropItem.itemTableIdx );
		g_logSystem->WriteToError( szTemp );
	}
	else
	{
		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;%d;;%s;%s", 
			LOG_CLASS_ITEM_DIE_DROP, type, position,
			dropItem.itemTableIdx,
			dropItem.name,
			GetItemInformationLog( false, dropItem.itemTableIdx, itemIdx, position ) );
		
	}
}




void GTH_PC_Die(playerCharacter_t *pc)
{	

	
	if(pc->event == pc->oldEvent) return;


	char	killerName[NAMESTRING+1];
	int		dropItemType		=	-1;
	int		dropItemPosition	=	0;
	int		reduceExp			=	0;
	int		chaosClass;
	float	expLossRate=0.0f;

	
	pc->alive = false;
	pc->curRA = 0.0f;
	
	CGonryunPractice_ProcessCtrl* pProcessCtrl = g_GonryunPracticeBattle.GetProcessCtrl();
	CGonryunPractice_ProcessCtrl::Step step = pProcessCtrl->GetStep(); 
	
	
	if ( step == CGonryunPractice_ProcessCtrl::STEP_FIGHTING){ 
		
		if((playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER == pc->GonryunBattlePractice.MemberShip)||
			(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT == pc->GonryunBattlePractice.MemberShip)){
			
			pProcessCtrl->CheckGameEnd(pc,CGonryunPractice_ProcessCtrl::BATTLE_END_RESULT_BORN);
			return;
	}
	}
	
	
	GTH_ForcedQuit( pc );	

	
	
	reduceExp		=	pc->exp;				
	chaosClass		=	GetChaosClass( pc );	

	
	__int64 lLossNark = pc->curChargeSE;	

	

	
	playerCharacter_t* pKillerPC = NULL;
	pKillerPC = gcpTools->GetPlayerRecordPointer(pc->killerIdx);
	if( pc->killerIdx >= 0 && pc->killerType >= 0 )
	{
		switch(pc->killerType){
			case ENTITY_PC:				
				{		
					if ( pKillerPC != NULL)
					{							
						if ( pc->DanBattleInfo.nDanBattleIdx == pKillerPC->DanBattleInfo.nDanBattleIdx)
						{
							NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
							pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(pc->DanBattleInfo.nDanBattleIdx, pc->guildIdx);
							if ( pDanBattleInfo != NULL)
							{								
								
								if ( TRUE == GTH_NewDanBattlePc_DIE(pKillerPC, pc) )
									return;								
								break;
							}
						}						
					}
					
					
					if( (TRUE == pc->pvpMode) || (DAN_BATTLEMAP_NO == pc->worldIdx)) 
						expLossRate = 0.0f;								
				else expLossRate = g_logic.dieExpLossRate[chaosClass];	
				}
				break;

			case ENTITY_MONSTER:
				if( (pc->worldIdx == 0) || (pc->worldIdx == 9)){
					expLossRate = 0.0f;				
				}else{
					expLossRate = g_logic.dieExpLossRate[chaosClass];	
				}
				break;
		}
	}

	
	GTH_PC_ReduceExp( pc, expLossRate * 0.01f,true );

	reduceExp			-=	pc->exp;
	pc->reduceExp		=	reduceExp;			
	
	lLossNark			-= pc->curChargeSE;		
	pc->reduceNark		= lLossNark;
	
	strcpy( killerName, "미확인" );

	if( pc->killerIdx >= 0 && pc->killerType >= 0 )
	{
		if( pc->killerType == ENTITY_PC ) 
		{
			if( g_pc[pc->killerIdx].active )
			{
				memcpy(killerName,g_pc[pc->killerIdx].name,NAMESTRING);
				killerName[NAMESTRING]=NULL;

				
			}




			
			
			


			
			
			if ( pc->worldIdx ==  DAN_BATTLEMAP_NO )								
			{
				g_pDanBattleSys->CalcPoint( pc, &g_pc[pc->killerIdx] );
			}
			
			else
			{
				AddChaosPoint( &g_pc[pc->killerIdx], pc );
			}
		}
		else
		{
			strncpy( killerName, g_monster[pc->killerIdx].basic.name,NAMESTRING);
			killerName[NAMESTRING]=NULL;
		}
	}

	
	

	GTH_SendPCEventMessage_Die( pc, dropItemType, dropItemPosition );

	
	
	int pvpResult=IsPvPEnablePosition( pc->worldIdx, pc->position );
	if(pvpResult == 30)
	{
		(gcpGolryunBattle->GetProcessClassPointer())->CheckMatchResult();
	}
	
	

	
	g_logSystem->Write( "User (%s) was Dead. decrease [%d] Experiance.", pc->name, reduceExp );
	g_logSystem->WriteToLog( pc, LOG_CLASS_PCDIE, "%d;%d;%d;;;%s;", reduceExp, pc->killerType, pc->curDamage, killerName );	
}


















void GTH_PC_Damage( playerCharacter_t *pc, message_t *message )
{
	if ( GTH_IsEnableHaveDamage(pc) == FALSE)	return;
	
	

	
	int dmg, eachDmg[3];
	int itemIdx;
	int transPoint			= 0;
	float hitRate			= (float)message->data[MSG_ATKDATA_HITRATE];
	float criticalRate		= (float)message->data[MSG_ATKDATA_CRITICAL_RATE];

	
	
	

	playerCharacter_t* pFromPlayer=NULL;
	monsterCharacter_t* pFromMonster=NULL;

	if( message->fromType == ENTITY_PC)
	{
		pFromPlayer =  gcpTools->GetPlayerRecordPointer(message->fromIdx);
		if(NULL == pFromPlayer) return;
		if( pc->idx == pFromPlayer->idx )
		return;
	}
	else 
	{
		pFromMonster = gcpTools->GetMonsterRecordPointer(message->fromIdx);
		if(NULL == pFromMonster) return;
	}


	
	pc->damagedEntityType	= message->fromType;
	pc->damagedEntityIdx	= message->fromIdx;

	
	if( message->fromType == ENTITY_PC )
	{
		if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_NORMAL )
		{
			hitRate			+= GTH_GetHitRate( pFromPlayer, ENTITY_PC, pc, ENTITY_PC, message->data[MSG_ATKDATA_HITPOWER] );
			criticalRate	+= GTH_GetCriticalRate(pFromPlayer, ENTITY_PC, pc, ENTITY_PC );

			pFromPlayer->resultAvoidRate = 100 - (int)hitRate;
		}
		else if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_MAGIC )
		{
			hitRate += GTH_GetMagicHitRate(pFromPlayer, ENTITY_PC, pc, ENTITY_PC );
			criticalRate = 0;

			pFromPlayer->resultHitRate = hitRate;
		}
		else
		{
			criticalRate = 0;
		}
		
		
		
		AddTargetList( pc, pFromPlayer->name );
		
		SetChaosGeneral( pFromPlayer, pc );
		
		if( pc->targetingType == TARGETING_NORMAL || pc->targetingType == TARGETING_SAFE )
		{
		}
	}
	else
	{
		if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_NORMAL )
		{
			hitRate			+= GTH_GetHitRate( pFromMonster, ENTITY_MONSTER, pc, ENTITY_PC, message->data[MSG_ATKDATA_HITPOWER] );
			criticalRate	+= GTH_GetCriticalRate( pFromMonster, ENTITY_MONSTER, pc, ENTITY_PC );
		}
		else if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_MAGIC )
		{
			hitRate			+= GTH_GetMagicHitRate( pFromMonster, ENTITY_MONSTER, pc, ENTITY_PC );
			criticalRate	= 0;
		}
		else
		{
			criticalRate	= 0;
		}
	}

	
	
	
	float fRandCriticalRate = GTH_Random();
	if( fRandCriticalRate < criticalRate * 0.01f )
	{
		message->data[MSG_ATKDATA_STUN_FLAG] = MSG_ATKDATA_STUN_TYPE_CRITICAL;
	}	
	
	
	
	dmg=YDCDamageCal::YDGetDamage( pc->idx, ENTITY_PC, message, eachDmg );

	
	if( GTH_Random() < hitRate * 0.01f )
	{
		pc->curDamage = dmg;
	}
	else
	{
		if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_MAGIC )
		{
			dmg = (int)( (float)dmg * 0.55f );
		}
		else
		{
			dmg = 0;
		}

		pc->curDamage = dmg;
	}

		
	
	if( pc->curDamage <= 0 )
	{			
		GTH_SendPCEventMessage_Damage( pc, -1 );
		return;
	}

	
	GTH_RemoveEntityCurseState( ENTITY_PC, pc->idx, AFF_DATA_TYPE_SLEEP );

	
	GTH_AnalyzeAttackMessageEtcInfo( message, eachDmg );

	
	if( pc->optValue[OV_SKILL_MANASHIELD] > 0.0f )
	{
		if( pc->curSA > 0 )
		{
			if( (int)pc->curDamage <= pc->curSA )
			{
				pc->curSA -= (int)pc->curDamage;
			}
			else
			{
				pc->curRA -= ((int)pc->curDamage - pc->curSA);
				pc->curSA = 0;
			}
		}
		else
		{
			pc->curRA -= (int)pc->curDamage;
		}
	}
	else
	{
		pc->curRA -= (int)pc->curDamage;
	}

	
	if( (pc->optValue[OV_REFLECTDMG_RATE] > 0.0f) || (pc->optValue[OV_REFLECTDMG] > 0.0f))
	{
		message_t copymsg;
		memset( &copymsg, 0, sizeof( message_t ) );

		copymsg.data[MSG_ATKDATA_PHYSIC_DMG] = (int)( (float)(float)eachDmg[0] * pc->optValue[OV_REFLECTDMG_RATE] * 0.01f );
		copymsg.data[MSG_ATKDATA_PHYSIC_DMG] += (int)pc->optValue[OV_REFLECTDMG];

		copymsg.data[MSG_ATKDATA_ATTACK_TYPE] = MSGINFO_ATK_TYPE_NONE;
		
		if( copymsg.data[MSG_ATKDATA_PHYSIC_DMG] > 0 )
		{
			copymsg.data[MSG_ATKDATA_HITRATE]			= 100;
			copymsg.type								= MSG_DAMEGED;
			copymsg.sendTime							= g_globalTime;
			copymsg.receiveTime							= g_globalTime;
			copymsg.worldIdx							= pc->worldIdx;
			copymsg.repeatNum							= 0;
			copymsg.fromType							= ENTITY_PC;
			copymsg.fromIdx								= pc->idx;
			copymsg.toType								= message->fromType;
			copymsg.toIdx								= message->fromIdx;
			AI_SendMessage( &copymsg );
		}
	}
	
	
	pc->curRA += (int)( (float)eachDmg[0] * pc->optValue[OV_RA_STEAL_DAMAGED] * 0.01f );
	pc->curSA += (int)( (float)eachDmg[0] * pc->optValue[OV_SA_STEAL_DAMAGED] * 0.01f );

	if( (int)pc->curRA < 0 ) pc->curRA = 0.0f;
	if( pc->curRA > pc->calMaxRA ) pc->curRA = pc->calMaxRA;
	if( (int)pc->curSA < 0 ) pc->curSA = 0.0f;
	if( pc->curSA > pc->calMaxSA ) pc->curSA = pc->calMaxSA;
	
	
	if ( (int)pc->curRA <= 0 ) 
	{
		
		pc->curRA				= 0.0f;
		pc->event				= GTH_EV_CHAR_DIE;
		pc->recoveryTime		= 0.0f;
		pc->killerIdx			= message->fromIdx;
		pc->killerType			= message->fromType;
		pc->lastAttackState		= message->data[MSG_ATKDATA_STUN_FLAG];
		return;
	}

	
	if( message->fromType != ENTITY_PC )
	{
		GTH_AddDefContributionPoint( &g_monster[message->fromIdx], ENTITY_PC, pc->idx, dmg );
	}
	
	
	itemIdx = GTH_PC_DecreaseDurability( pc, 1, message->data[MSG_ATKDATA_STUN_FLAG] );
	GTH_SendPCEventMessage_Damage( pc, itemIdx, message->data[MSG_ATKDATA_STUN_FLAG] );
}




void GTH_PC_Respawn( playerCharacter_t *pc, int curRA, int curSA, int recoveryExp, int resurrectionFlag )
{
	int zoneIdx;
	vec3_t pos, rpos;
	int worldIdx;
	int bindingIdx;
	int memberIdx;
	vec3_t savePosition;
	int saveWorldIdx;

	AI_InitMessage( pc );
	GTH_PC_Calculate( pc );

	
	if( resurrectionFlag )
	{
		pc->alive = true;
		pc->event = GTH_EV_CHAR_IDLE;
		pc->curRA = (float)curRA;
		pc->curSA = (float)curSA;

		if( recoveryExp > 0 )
		{
			GTH_PC_GetLoot( pc->idx, recoveryExp, 0, 0, 0, 0 );
		}

		GTH_SendPCEventMessage_Respawn( pc );
		return;
	}

	bindingIdx = pc->bindingIdx;
	if( bindingIdx < 0 ) bindingIdx = 0;
	if( g_binding[bindingIdx].idx < 0 ) bindingIdx = 0;
	worldIdx = g_binding[bindingIdx].worldIdx;

	
	if ( GTH_IsLocalServiceWorld(worldIdx) )
	{
		pc->alive = true;
		pc->event = GTH_EV_CHAR_IDLE;
		pc->curRA = (float)curRA;
		pc->curSA = (float)curSA;

		if( worldIdx == pc->worldIdx ) 
		{
			vec_copy( g_binding[bindingIdx].pos, pos );
			int rand_i;
			rand_i = GTH_Randomize( (int)g_binding[bindingIdx].range );
			rpos[0] = (float)rand_i * 2.0f - g_binding[bindingIdx].range;
			rand_i = GTH_Randomize( (int)g_binding[bindingIdx].range );
			rpos[1] = (float)rand_i * 2.0f - g_binding[bindingIdx].range;

			pos[0] += rpos[0];
			pos[1] += rpos[1];

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
			vec_copy( g_binding[bindingIdx].pos, pos );
			int rand_i;
			rand_i = GTH_Randomize( (int)g_binding[bindingIdx].range );
			rpos[0] = (float)rand_i * 2.0f - g_binding[bindingIdx].range;
			rand_i = GTH_Randomize( (int)g_binding[bindingIdx].range );
			rpos[1] = (float)rand_i * 2.0f - g_binding[bindingIdx].range;

			pos[0] += rpos[0];
			pos[1] += rpos[1];

			
			vec_copy( pos, pc->reliablePosition );
			pc->reliableWorldIdx = worldIdx;

			vec_copy( pos, pc->position );

			pc->zoneIdx = GTH_Zone_GetZoneIdx( pc->position );
			
			GTH_Zone_AddEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);

			GTH_SendPCEventMessage_Respawn( pc );
			pc->ready = false;

			
			g_logSystem->Write("User (%s) Move to Binding Pos. Pos(%.2f, %.2f, %.2f), World :%d", pc->userID, pc->position[0], pc->position[1], pc->position[2], pc->worldIdx);
		}
	}
	else 
	{
		
		saveWorldIdx = pc->worldIdx;
		vec_copy(pc->position, savePosition);

		
		pc->worldIdx = worldIdx;

		vec_copy( g_binding[bindingIdx].pos, pos );

		int rand_i;
		rand_i = GTH_Randomize( (int)g_binding[bindingIdx].range );
		rpos[0] = (float)rand_i * 2.0f - g_binding[bindingIdx].range;
		rand_i = GTH_Randomize( (int)g_binding[bindingIdx].range );
		rpos[1] = (float)rand_i * 2.0f - g_binding[bindingIdx].range;

		pos[0] += rpos[0];
		pos[1] += rpos[1];

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


void GTH_PC_MouseWalk(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );

	speed = SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);
	
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_MouseWalk( pc );
}

void GTH_PC_MouseRun(playerCharacter_t *pc)
{
	vec3_t delta;
	float speed;
	int zoneIdx;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, pc->angles[0] );
	RotateZCoord( delta, pc->angles[1] );
	Normalize( delta );	


	speed = RUN_SCALE * SPEED_CONSTANT * pc->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(pc->position, delta, pc->position);

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	if (pc->zoneIdx != zoneIdx)
	{
		pc->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( pc );
	}

	ShowLogInfo("skilldamage=%d, skillidx=%d", pc->skillDamage, pc->saveSkillIdx);
	
	if ( pc->saveSkillIdx != -1 )
	{
		GTH_SendPCEventMessage_Idle( pc );
		return ;
	}

	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_MouseRun( pc );	
}



void GTH_PC_AutoRecovery( playerCharacter_t* pc )
{
	float	recRAPoint	=	0.0f;
	float	recSAPoint	=	0.0f;
	float	tvalue1, tvalue2;
	int		optValueIdx;

	
	

	pc->autoRecoveryRATime -= g_timer->GetElapsedMilliTime();
	pc->autoRecoverySATime -= g_timer->GetElapsedMilliTime();

	if( pc->autoRecoveryRATime < 0.0f )
	{		
		if( pc->event == GTH_EV_CHAR_SIT ) optValueIdx = OV_AUTORECOVERY_RA_IDLE;
		else optValueIdx = OV_AUTORECOVERY_RA_ATTACK;



		
		if(pc->optValue[OV_AUTORECOVERY_RA_TIME] > -PC_AUTO_RECOVERY_DELAY && pc->optValue[OV_AUTORECOVERY_RA_TIME] < 30000)
		{
			pc->autoRecoveryRATime	=	 pc->optValue[OV_AUTORECOVERY_RA_TIME] + PC_AUTO_RECOVERY_DELAY;
		}
		else
		{
			pc->autoRecoveryRATime	=	 PC_AUTO_RECOVERY_DELAY;
		}

		if(pc->autoRecoveryRATime < 0) pc->autoRecoveryRATime =0;
		

		
		tvalue1			=	(float)pc->calVitality	*	(float)pc->calVitality;
		tvalue2			=	60.0f	+	(float)pc->calVitality	*	2.0f;
		recRAPoint		=	tvalue1 / tvalue2;		
		recRAPoint		+=	recRAPoint	* ( pc->optValue[optValueIdx]	* 0.01f );

		if( pc->event == GTH_EV_CHAR_SIT )	recRAPoint	*=	2.0f;
		
	}
	if( pc->autoRecoverySATime < 0.0f )
	{
		if( pc->event == GTH_EV_CHAR_SIT ) optValueIdx = OV_AUTORECOVERY_SA_IDLE;
		else optValueIdx = OV_AUTORECOVERY_SA_ATTACK;

		
		if(pc->optValue[OV_AUTORECOVERY_SA_TIME] > -PC_AUTO_RECOVERY_DELAY && pc->optValue[OV_AUTORECOVERY_SA_TIME] < 30000)
		{
			pc->autoRecoverySATime	=	pc->optValue[OV_AUTORECOVERY_SA_TIME] + PC_AUTO_RECOVERY_DELAY;
		}
		else
		{
			pc->autoRecoverySATime	=	PC_AUTO_RECOVERY_DELAY;
		}

		if(pc->autoRecoverySATime < 0) pc->autoRecoverySATime=0;
		

		
		tvalue1			=	(float)pc->calSoulpower	*	(float)pc->calSoulpower;
		tvalue2			=	80.0f	+	powf( (float)pc->calSoulpower, 1.3f );
		recSAPoint		=	tvalue1 / tvalue2;		
		recSAPoint		+=	recSAPoint	*	(	pc->optValue[optValueIdx] * 0.01f );	

		if( pc->event == GTH_EV_CHAR_SIT )	recSAPoint	*=	2.0f;
	}

	pc->curRA += recRAPoint;
	pc->curSA += recSAPoint;

	if( pc->curRA > pc->calMaxRA ) pc->curRA = pc->calMaxRA;
	if( pc->curSA > pc->calMaxSA ) pc->curSA = pc->calMaxSA;
}

void GTH_PC_Sit( playerCharacter_t *pc )
{
	if (pc->event != pc->oldEvent)
	{
		GTH_SendPCEventMessage_Sit( pc );
	}
}

void GTH_PC_Stand( playerCharacter_t *pc )
{
	
	if (pc->event != pc->oldEvent)
		GTH_SendPCEventMessage_Stand( pc );
}



void	GTH_PC_Shout( playerCharacter_t* pc )
{
	GTH_SendPCEventMessage_Shout( pc );
}


void	GTH_PC_BOW( playerCharacter_t* pc )
{
	GTH_SendPCEventMessage_Bow( pc );
}


void GTH_PC_LevelUp( playerCharacter_t *pc, int newLevel )
{
	int multipler = newLevel - pc->level;

	
	
	g_logSystem->Write( "User (%s) Level Up to %d.", pc->name, newLevel );
	g_logSystem->WriteToLog( pc, LOG_CLASS_LEVELUP, "%d;%d;;;;;", pc->age, pc->age - pc->levelUpTime );
	

	pc->levelUpTime		=	pc->age;
	pc->level			=	newLevel;
	
	
	
	pc->force			+=	multipler;
	pc->soulpower		+=	multipler;
	pc->agility			+=	multipler;
	pc->vitality		+=	multipler;

	
	GTH_PartyInfo_Request_Update( pc, true );
	GTH_PartyInfo_Request_SendAll( pc );

	entityPCList_t pl ;
	g_entityListMng.MakePCList ( pc, &pl );
	g_entityListMng.UpdatePCList ( &pl );
	
	
	GTH_FriendsList_Request_Update ( pc, FriendsUpdate );
	
	g_HelperManager.MemberList_Request_Update(pc,CHelperManager::Memberstate::HelpersUpdate);
	g_HelperManager.GiveHelpPointToUser(pc);
	
	
	
	if( pc->guildIdx != -1 )
	{
		
		
		if( pc->rankInGuild == -1 && pc->rankIdxInGuild == -1 )
			return;

		if( g_config.isManager )
		{
			GTH_GuildUpdateUser( pc->guildIdx, pc->name, pc->rankInGuild, pc->rankIdxInGuild, GUILD_USERLEVEL, pc->level );
		}
		else
		{
			GTH_SendMessage_Guild_Update_Request_To_ManagerServer( pc->guildIdx, pc->name, pc->rankInGuild, pc->rankIdxInGuild, GUILD_USERLEVEL, pc->level );
		}
	}
}

void GTH_PC_GenLevelUp( playerCharacter_t* pc, int newGenLevel )
{
	pc->genLevel = newGenLevel;

	
	
	
	g_logSystem->Write( "User (%s) Level(Gen) Up to %d.", pc->name, pc->genLevel );
	g_logSystem->WriteToLog( pc, LOG_CLASS_GENLEVELUP, "%d;%d;;;;;", pc->age, pc->age - pc->genLevelUpTime );
	
	
	pc->genLevelUpTime = pc->age;
}

void GTH_PC_ExpUp( int idx, __int64 &exp, __int64 &genExp, int &lootNSE, int &level, int &genLevel )
{
	__int64 pcExp, pcGenExp;

	if(g_pc[idx].level < 30){
		if(exp > g_expTable[29].exp) exp = g_expTable[29].exp;
	}else{
		if(exp > g_expTable[g_pc[idx].level-1].exp) exp = g_expTable[g_pc[idx].level-1].exp;
	}


	if(g_pc[idx].genLevel < 15){
		if(genExp > g_expTable[14].genExp) genExp = g_expTable[14].genExp;
	}else{
		if(genExp > g_expTable[g_pc[idx].genLevel-1].genExp) genExp = g_expTable[g_pc[idx].genLevel-1].genExp;
	}


	if( exp > 0 )
	{
		pcExp = g_pc[idx].exp + exp;
		level = GTH_GetNextLevel( pcExp );
		g_pc[idx].exp = pcExp;
	}
	
	if( genExp > 0 )
	{
		pcGenExp = g_pc[idx].genExp + genExp;
		genLevel = GTH_GetNextGenLevel( pcGenExp );
		g_pc[idx].genExp = pcGenExp;
	}
}


void GTH_PC_GetLoot_Cheat( int idx, __int64 exp, __int64 genExp, int lootNSE, int genCapability, int sharingType )
{
	int level = 0, genLevel = 0, selectedGen;
	int skillIdx[50], skillNumber = 0;
	
	selectedGen = g_pc[idx].selectedGen;
	
	if( exp < 0 ) exp = 0;
	if( genExp < 0 ) genExp = 0;
	if( lootNSE < 0 ) lootNSE = 0;
	if( genCapability < 0 ) genCapability = 0;

	
	if( exp == 0 && genExp == 0 && lootNSE == 0 && genCapability == 0 ) return;
	
	g_pc[idx].exp = 0;
	g_pc[idx].genExp = 0;
	g_pc[idx].curChargeSE = 0;
	g_pc[idx].level = 1;
	g_pc[idx].genLevel = 1;

	GTH_PC_ExpUp( idx, exp, genExp, lootNSE, level, genLevel );

	if( genCapability > 0 && g_pc[idx].genCapability[selectedGen] >= 0 ) g_pc[idx].genCapability[selectedGen] += genCapability;

	g_pc[idx].curChargeSE += lootNSE;
	
	if( g_pc[idx].curChargeSE > MAX_OWN_NAK ) g_pc[idx].curChargeSE = MAX_OWN_NAK;

	if( level > g_pc[idx].level )
	{
		g_pc[idx].bonusStatusPoint = level;
		GTH_PC_LevelUp( &g_pc[idx], level );
	}

	if( genLevel > g_pc[idx].genLevel )
	{

		GTH_PC_GenLevelUp( &g_pc[idx], genLevel );
	}
	
	
	GTH_PC_Calculate( &g_pc[idx], PCINFO_BASIC );
	GTH_SendPCMessage_LevelUp( &g_pc[idx], g_pc[idx].exp, g_pc[idx].genExp, g_pc[idx].curChargeSE, g_pc[idx].genCapability[selectedGen], skillIdx, skillNumber, sharingType );

}


void GTH_PC_GetLoot_QuestScroll( int idx, __int64 exp, __int64 genExp, int lootNSE, int genCapability )
{
	int upFlag = false;
	int level = 0, genLevel = 0, selectedGen;
	int skillIdx[50], skillNumber = 0;
	
	
	
	exp = (__int64)( exp * g_logic.expRate );
	genExp = (__int64)( genExp * g_logic.expRate );
	lootNSE = (int)( (float)lootNSE * g_logic.nseRate );
	genCapability = (int)( (float)genCapability * g_logic.genCapabilityRate );

	selectedGen = g_pc[idx].selectedGen;
	
	if( exp < 0 )			exp = 0;
	if( genExp < 0 )		genExp = 0;
	if( lootNSE < 0 )		lootNSE = 0;
	if( genCapability < 0 )	genCapability = 0;

	
	if( exp == 0 && genExp == 0 && lootNSE == 0 && genCapability == 0 ) 
		return;
	
	
	GTH_PC_ExpUp( idx, exp, genExp, lootNSE, level, genLevel );

	if( genCapability > 0 && g_pc[idx].genCapability[selectedGen] >= 0 ) 
		g_pc[idx].genCapability[selectedGen] += genCapability;

	g_pc[idx].curChargeSE += lootNSE;
	
	if( g_pc[idx].curChargeSE > MAX_OWN_NAK ) g_pc[idx].curChargeSE = MAX_OWN_NAK;

	if( level > g_pc[idx].level )
	{
		g_pc[idx].bonusStatusPoint += ( level - g_pc[idx].level );
		GTH_PC_LevelUp( &g_pc[idx], level );
		upFlag = true;
	}
	if( genLevel > g_pc[idx].genLevel )
	{

		GTH_PC_GenLevelUp( &g_pc[idx], genLevel );
		upFlag = true;
	}
	
	if( upFlag )
	{
		
		GTH_PC_Calculate( &g_pc[idx], PCINFO_BASIC );
		GTH_SendPCMessage_LevelUp( &g_pc[idx], g_pc[idx].exp, g_pc[idx].genExp, g_pc[idx].curChargeSE, g_pc[idx].genCapability[selectedGen], skillIdx, skillNumber, 10 );
	}
	else
	{
		GTH_SendMessage_LootValue( &g_pc[idx], g_pc[idx].exp, g_pc[idx].genExp, g_pc[idx].curChargeSE, g_pc[idx].genCapability[selectedGen], 10 );
	}

	
	
	
	
	if( 0 < exp )
			GTH_ProcessMessage_AutoQuestScroll_ObtainOpen(&g_pc[idx]);

	
	
}




void GTH_PC_GetLoot_AutoQuestScroll(playerCharacter_t* pPlayer, __int64 exp, __int64 genExp, int genCapability,int lootNSE,int fame, int chaoPoint) 
{
	int upFlag = FALSE;
	int level = 0, genLevel = 0, selectedGen;
	int skillIdx[50], skillNumber = 0;
	
	
	
	exp = (__int64)( exp * g_logic.expRate );
	
	genExp = (__int64)( genExp * g_logic.expRate );
	
	genCapability = (int)( (float)genCapability * g_logic.genCapabilityRate );
	
	lootNSE = (int)( (float)lootNSE * g_logic.nseRate );

	selectedGen = pPlayer->selectedGen;

	
	if( exp < 0 )			exp = 0;
	
	if( genExp < 0 )		genExp = 0;
	
	if( lootNSE < 0 )		lootNSE = 0;
	
	if( genCapability < 0 )	genCapability = 0;

	if(lootNSE < 0)	lootNSE = 0;
	
	if(chaoPoint < 0 ) chaoPoint = 0;
	
	if(fame < 0) fame = 0;

	
	pPlayer->rankPoint += fame;
	
	pPlayer->chaosPoint -= chaoPoint;
	
	if( 0 > pPlayer->chaosPoint) pPlayer->chaosPoint = 0;

	if( exp == 0 && genExp == 0 && lootNSE == 0 && genCapability == 0 ) 
		return;
	
	
	GTH_PC_ExpUp( pPlayer->idx, exp, genExp, lootNSE, level, genLevel );

	if( genCapability > 0 && pPlayer->genCapability[selectedGen] >= 0 ) 
		pPlayer->genCapability[selectedGen] += genCapability;

	pPlayer->curChargeSE += lootNSE;

	if( pPlayer->curChargeSE > MAX_OWN_NAK ) pPlayer->curChargeSE = MAX_OWN_NAK;

	if( level > pPlayer->level )
	{
		pPlayer->bonusStatusPoint += ( level - pPlayer->level );
		GTH_PC_LevelUp( pPlayer, level );
		upFlag = TRUE;
	}
	if( genLevel > pPlayer->genLevel )
	{

		GTH_PC_GenLevelUp( pPlayer, genLevel );
		upFlag = TRUE;
	}
	
	if( TRUE == upFlag ) {
		
		
		GTH_PC_Calculate( pPlayer, PCINFO_BASIC );
	
		GTH_SendPCMessage_LevelUp(
			pPlayer, 
			pPlayer->exp,
			pPlayer->genExp, 
			pPlayer->curChargeSE,
			pPlayer->genCapability[selectedGen], 
			skillIdx, 
			skillNumber, 
			10 );
	
	} else {
	
		GTH_SendMessage_LootValue( 
			pPlayer,
			pPlayer->exp,
			pPlayer->genExp,
			pPlayer->curChargeSE,
			pPlayer->genCapability[selectedGen],
			10 );
	}
	
	
	if ( 0 < exp)
		GTH_ProcessMessage_AutoQuestScroll_ObtainOpen(pPlayer);
	
	
	
}




int GTH_PC_StatusUp( int statusType )
{	
	if( g_curPC->bonusStatusPoint < 1 ) return 1;

	switch( statusType )
	{
	case 0 :
		if( !CheckLimitStatus( g_curPC->level, g_curPC->pcClass, g_curPC->calForce, STATUS_FORCE ) ) return 2;

		g_curPC->bonusStatusPoint --;
		g_curPC->force ++;
		break;
	case 1 :
		if( !CheckLimitStatus( g_curPC->level, g_curPC->pcClass, g_curPC->calSoulpower, STATUS_SOULPOWER ) ) return 2;

		g_curPC->bonusStatusPoint --;
		g_curPC->soulpower ++;
		break;
	case 2 :
		if( !CheckLimitStatus( g_curPC->level, g_curPC->pcClass, g_curPC->calAgility, STATUS_AGILITY ) ) return 2;

		g_curPC->bonusStatusPoint --;
		g_curPC->agility ++;
		break;
	case 3 :
		if( !CheckLimitStatus( g_curPC->level, g_curPC->pcClass, g_curPC->calVitality, STATUS_VITALITY ) ) return 2;

		g_curPC->bonusStatusPoint --;
		g_curPC->vitality ++;
		break;
	}
	return 0;
}

void GTH_PC_ChangeEnergyPack( item_t* item )
{
	int totalRA, totalSA;

	totalRA = g_curPC->curChargeRA * g_curPC->onceRecoveryRA;
	totalSA = g_curPC->curChargeSA * g_curPC->onceRecoverySA;

	if( g_itemTable[item->itemTableIdx].epkRecoveryRA > 0 )
		g_curPC->curChargeRA = totalRA / g_itemTable[item->itemTableIdx].epkRecoveryRA;
	else g_curPC->curChargeRA = 0;

	if( g_itemTable[item->itemTableIdx].epkRecoverySA > 0 )
		g_curPC->curChargeSA = totalSA / g_itemTable[item->itemTableIdx].epkRecoverySA;
	else g_curPC->curChargeSA = 0;

	if( g_curPC->curChargeRA > g_itemTable[item->itemTableIdx].epkMaxChargeRA )
	{
		g_curPC->curChargeRA = g_itemTable[item->itemTableIdx].epkMaxChargeRA;
	}
	if( g_curPC->curChargeSA > g_itemTable[item->itemTableIdx].epkMaxChargeSA )
	{
		g_curPC->curChargeSA = g_itemTable[item->itemTableIdx].epkMaxChargeSA;
	}
	g_curPC->onceRecoveryRA = g_itemTable[item->itemTableIdx].epkRecoveryRA;
	g_curPC->onceRecoverySA = g_itemTable[item->itemTableIdx].epkRecoverySA;
}

void GTH_PC_ReduceExp( playerCharacter_t *pc, float percent,bool bDie )
{
	__int64 currentExp, prevTotalExp;
	float reduceExp;
	int level;

	level = pc->level;

	if( level <= 5 ) return;
	if(pc->worldIdx == CPremiumPlayerCtrl::enumMapIdx::Gyorun_Colosseum) return;

	prevTotalExp = g_expTable[level - 2].totalexp;
	currentExp = pc->exp - prevTotalExp;
	if(currentExp < 0) return;
	
	
	if(TRUE == gGameConfigFlag.m_bDie_Reduce_panality)		
	{
		reduceExp = (float)currentExp * (percent * pc->PremiumInfo.Die.fExpRatio);

		if(bDie)
		{
			__int64 lLossNark = pc->curChargeSE *  pc->PremiumInfo.Die.fNakRatio;
			pc->curChargeSE -= lLossNark;
		}
	}
	else
	{
		reduceExp = (float)currentExp * percent;
	}

	if( pc->exp < (int)reduceExp ) pc->exp = 0;
	else 
	{
		pc->exp -= (int)reduceExp;
		if( pc->exp <= prevTotalExp ) pc->exp = prevTotalExp + 1;
	}

	
}


void GTH_PC_GetStealPoint( playerCharacter_t *pc, int dmg )
{
	
	pc->curChargeSE += (int)( (float)dmg * pc->optValue[OV_SESTEAL_RATE] * 0.01f );
	pc->curChargeSE += (int)pc->optValue[OV_SESTEAL];
	if( pc->curChargeSE > MAX_OWN_NAK ) pc->curChargeSE = MAX_OWN_NAK;

	
	
	if( GTH_Random() < 0.25f )
	{
		pc->curRA += (float)dmg * pc->optValue[OV_RASTEAL] * 0.01f;
		pc->curSA += (float)dmg * pc->optValue[OV_SASTEAL] * 0.01f;
		
		
		if( dmg > 0 )
		{
			pc->curRA += pc->optValue[OV_RASTEAL_POINT];
			pc->curSA += pc->optValue[OV_SASTEAL_POINT];
		}
		
		if( pc->curRA > pc->calMaxRA ) pc->curRA = pc->calMaxRA;
		if( pc->curSA > pc->calMaxSA ) pc->curSA = pc->calMaxSA;
	}
}

int GTH_PC_DecreaseDurability( playerCharacter_t* pc, int type, int shock )
{
	int i, itemIdx, result, numOfDefenseProb, selectedDefensePos[6];
	float r, accPro;
	float defenseProb[6], defenseTotalProb = 0, selectedDefenseProb[6];
	item_t* item;

	result = 0;

	switch( type )
	{
		
	case 0 :
		itemIdx = pc->equipment[EQUIP_WEAPON_1 + pc->curWeapon];
		if( itemIdx < 0 ) break;
		r = GTH_Random() * 100.0f;

		if( r < PC_DECEREASE_WEAPON_DURABILITY_PROBABILITY )
		{
			item = &pc->item[itemIdx];
			
			if( shock ) item->durability -= 2;
			else item->durability --;
			if( item->durability <= 0 ) 
			{
				item->durability = 0;
				result = 2;
				break;
			}
			result = 1;
			break;
		}
		result = 0;
		break;
	case 1 :
		
		r = GTH_Random() * 100.0f;

		if( r < PC_DECEREASE_DEFENSE_DURABILITY_PROBABILITY )
		{
			numOfDefenseProb = 0;
			defenseProb[0] = PC_DECEREASE_UPPER_DURABILITY_PROBABILITY;
			defenseProb[1] = PC_DECEREASE_LOWER_DURABILITY_PROBABILITY;
			defenseProb[2] = PC_DECEREASE_AIR_DURABILITY_PROBABILITY;
			defenseProb[3] = PC_DECEREASE_PROTECTOR_DURABILITY_PROBABILITY;
			defenseProb[4] = PC_DECEREASE_GLOVES_DURABILITY_PROBABILITY;
			defenseProb[5] = PC_DECEREASE_BOOTS_DURABILITY_PROBABILITY;
			
			for( i = 0; i < 6; i ++ )
			{
				itemIdx = pc->equipment[EQUIP_UPPER + i];
				if( itemIdx < 0 ) continue;
				defenseTotalProb += defenseProb[i];
				selectedDefenseProb[numOfDefenseProb] = defenseProb[i];
				selectedDefensePos[numOfDefenseProb] = i;
				numOfDefenseProb ++;
			}
			if( numOfDefenseProb == 0 ) break;
			
			r = GTH_Random() * defenseTotalProb;
			accPro = 0.0f;
			for( i = 0; i < numOfDefenseProb; i ++ )
			{
				accPro += selectedDefenseProb[i];
				if( r < accPro ) break;
			}
			if( i == numOfDefenseProb ) break;
			itemIdx = pc->equipment[EQUIP_UPPER + selectedDefensePos[i]];
			if( itemIdx < 0 ) break;

			item = &pc->item[itemIdx];
			
			if( shock ) item->durability -= 2;
			else item->durability --;
			if( item->durability <= 0 ) 
			{
				item->durability = 0;
				result = 2;
				break;
			}
			result = 1;
			break;
		}
		break;
	}

	switch( result )
	{
	case 0 :
		return( -1 );

	case 1 :
		g_DBGameServer->SaveItem( pc, item, itemIdx );
		return( itemIdx );

	case 2 :
		g_DBGameServer->SaveItem( pc, item, itemIdx );
		GTH_PC_Calculate( pc );
		return( itemIdx );
	}

	return -1;
}



#include "..\AHNLAB\AHNLAB.h"
extern CAhnLab* g_cpAhnLab;
void GTH_ExecutePCEvent()
{
	playerCharacter_t *pc;
	int i;

	
	for (i=0; i<MAX_PCS; i++)
	{
		pc = &g_pc[i];	
		if( !pc->active || !pc->ready ) continue;
		
		if( !pc->alive )
		{
			
			AI_ReceiveMessage( pc );

			g_PcSkill.GTH_PC_SkillCasting( pc );
			continue;
		}

		
		GTH_ProcessItemSync(pc);

		
		GTH_ProcessComplusionWorldChange(pc);
		

		
		AI_ReceiveMessage( pc );
		
		GTH_AffectProcess( pc );

		if( pc->recoveryTime > 0.0f ) 
		{
			pc->recoveryTime -= g_timer->GetElapsedMilliTime();
		}
		if( pc->recoveryTime < 0.0f ) 
		{
			pc->recoveryTime = 0.0f;
		}

		
		if ( NULL != g_cpAhnLab )			
			g_cpAhnLab->GTH_CheckReciveTIme(pc);



		
		
		g_PcSkill.GTH_PC_SkillCasting( pc );
		GTH_PC_AutoRecovery( pc );
		
		
		

		
	
		g_GonryunPracticeBattle.GetProcessCtrl()->CheckPlayerPos_and_Transport(pc);

		
		if(TRUE == (gcpGolryunBattle->GetProcessClassPointer())->CheckExpelUser(pc))
		{
			gcpGolryunBattleLog->Write("곤륜장에서 벽뚫고 들어간 유저: %s", pc->name);
		}
		

		switch( pc->event )
		{
		case GTH_EV_CHAR_IDLE:
			GTH_PC_Idle( pc );
			break;

		case GTH_EV_CHAR_WALK:
			GTH_PC_Walk( pc );
			break;

		case GTH_EV_CHAR_WALKLEFT:
			GTH_PC_WalkLeft( pc );
			break;

		case GTH_EV_CHAR_WALKRIGHT:
			GTH_PC_WalkRight( pc );
			break;

		case GTH_EV_CHAR_BACK:
			GTH_PC_Back( pc );
			break;

		case GTH_EV_CHAR_BACKLEFT:
			GTH_PC_BackLeft( pc );
			break;

		case GTH_EV_CHAR_BACKRIGHT:
			GTH_PC_BackRight( pc );
			break;

		case GTH_EV_CHAR_RUN:
			GTH_PC_Run( pc );
			break;

		case GTH_EV_CHAR_RUNLEFT:
			GTH_PC_RunLeft( pc );
			break;

		case GTH_EV_CHAR_RUNRIGHT:
			GTH_PC_RunRight( pc );
			break;

		case GTH_EV_CHAR_MOVELEFT:
			GTH_PC_MoveLeft( pc );
			break;

		case GTH_EV_CHAR_MOVERIGHT:
			GTH_PC_MoveRight( pc );
			break;

		case GTH_EV_CHAR_TURNLEFT:
			GTH_PC_TurnLeft( pc );
			break;

		case GTH_EV_CHAR_TURNRIGHT:
			GTH_PC_TurnRight( pc );
			break;

		case GTH_EV_CHAR_ATTACK:
			break;

		case GTH_EV_CHAR_DIE:
			GTH_PC_Die( pc );
			break;

		case GTH_EV_CHAR_MOUSEWALK:
			GTH_PC_MouseWalk( pc );
			break;

		case GTH_EV_CHAR_MOUSERUN:
			GTH_PC_MouseRun( pc );
			break;
		case GTH_EV_CHAR_SIT:
			GTH_PC_Sit( pc );
			break;

		case GTH_EV_CHAR_STAND:
			GTH_PC_Stand( pc );
			break;

		case GTH_EV_CHAR_SKILLCASTING :
			break;

		
		case GTH_EV_CHAR_TELEPORT:
			break;
		case GTH_EV_CHAR_SHOUT:
			GTH_PC_Shout( pc );
			break;
		case GTH_EV_CHAR_BOW:
			GTH_PC_BOW( pc );
			break;
		default:
			break;
		}

		
		pc->oldEvent = pc->event;
	} 
}





void GTH_ProcessEventMessage_Idle()
{
	vec3_t pos;
	float yawAngle;
	int zoneIdx;

	MSG_ReadPosition( pos );
	yawAngle = MSG_ReadFloat();
	

	g_curPC->event = GTH_EV_CHAR_IDLE;

	
	
	
	

	
	vec_copy(pos, g_curPC->position);
	
	vec_copy(g_curPC->position, g_curPC->reliablePosition );
	g_curPC->reliableWorldIdx = g_curPC->worldIdx;

	g_curPC->angles[1] = yawAngle;

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}


void GTH_ProcessEventMessage_Walk()
{
	int zoneIdx;
	g_curPC->event = GTH_EV_CHAR_WALK;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_WalkLeft()
{
	int zoneIdx;
	g_curPC->event = GTH_EV_CHAR_WALKLEFT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_WalkRight()
{
	int zoneIdx;
	g_curPC->event = GTH_EV_CHAR_WALKRIGHT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_Back()
{
	int zoneIdx;
	g_curPC->event = GTH_EV_CHAR_BACK;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_BackLeft()
{
	int zoneIdx;
	g_curPC->event = GTH_EV_CHAR_BACKLEFT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_BackRight()
{
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_BACKRIGHT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_Run()
{
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_RUN;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_RunLeft()
{
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_RUNLEFT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_RunRight()
{
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_RUNRIGHT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_TurnLeft()
{
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_TURNLEFT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_TurnRight()
{
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_TURNRIGHT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_MoveLeft()
{
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_MOVELEFT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_MoveRight()
{
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_MOVERIGHT;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}




void GTH_ProcessEventMessage_Respawn()
{
	
	if ( !g_curPC->alive )			
		GTH_PC_Respawn( g_curPC, 10, 10 );
}

void GTH_ProcessEventMessage_Sit()
{
	g_curPC->event = GTH_EV_CHAR_SIT;
}


void GTH_ProcessEventMessage_Stand()
{
	g_curPC->event = GTH_EV_CHAR_STAND;
}






void GTH_ProcessEventMessage_MouseWalk()
{
	
	
	
	if( g_curPC->event == GTH_EV_CHAR_DIE ) return;
	
	
	int zoneIdx;

	g_curPC->event = GTH_EV_CHAR_MOUSEWALK;
	g_curPC->oldEvent = GTH_EV_CHAR_IDLE; 

	MSG_ReadPosition( g_curPC->position );
	
	vec_copy(g_curPC->position, g_curPC->reliablePosition );
	g_curPC->reliableWorldIdx = g_curPC->worldIdx;

	g_curPC->angles[1] = MSG_ReadFloat();
	MSG_ReadPosition( g_curPC->mousePos );

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessEventMessage_MouseRun()
{
	
	if( g_curPC->event == GTH_EV_CHAR_DIE ) return;

	int zoneIdx;
	g_curPC->event = GTH_EV_CHAR_MOUSERUN;
	g_curPC->oldEvent = GTH_EV_CHAR_IDLE; 

	MSG_ReadPosition( g_curPC->position );
	
	vec_copy(g_curPC->position, g_curPC->reliablePosition );
	g_curPC->reliableWorldIdx = g_curPC->worldIdx;

	g_curPC->angles[1] = MSG_ReadFloat();
	MSG_ReadPosition( g_curPC->mousePos ); 

	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}
}

void GTH_ProcessMessage_ReadyToPlay()
{
	
	
	g_curPC->sendReplyConnect = false;

	g_curPC->ready = true;
	g_curPC->syncTime = g_globalTime; 

	
	
	GTH_DeleteAffectAll( g_curPC, ENTITY_PC );

	
	GTH_PC_Calculate( g_curPC, PCINFO_BASIC );

	
	GTH_SendMessage_SyncItemObject( g_curPC );
	
	
	GTH_PartyInfo_Request_Update( g_curPC, true );
	
	GTH_PartyInfo_Request_SendAll( g_curPC );

	
	GTH_SendMessage_NPCInfo( g_curPC );

	
	entityPCList_t pl;
	g_entityListMng.MakePCList ( g_curPC, &pl );
	g_entityListMng.UpdatePCList ( &pl );

	
	GTH_FriendsList_Request_List ( g_curPC );

	
	g_HelperManager.MemberList_Request_List(g_curPC);
	
	
	
	
	if( g_curPC->guildIdx != -1 )
	{
		if( g_config.isManager )
		{
			int rankInGuild;
			int rankIdxInGuild;
			
			GTH_GuildSetUserInfo( g_curPC->guildIdx, g_curPC->name, rankInGuild, rankIdxInGuild );

			if( rankInGuild != -1 )
			{
				g_curPC->rankInGuild = rankInGuild;
				g_curPC->rankIdxInGuild = rankIdxInGuild;
				strcpy( g_curPC->guildName, g_guild[g_curPC->guildIdx].name );

				
				GTH_GuildUpdate( g_curPC->guildIdx, GUILD_UPDATE_ALL, GUILD_ALLRANK, -1, rankInGuild, rankIdxInGuild );

				
				GTH_GuildUpdate( g_curPC->guildIdx, GUILD_UPDATE_USER, rankInGuild, rankIdxInGuild, GUILD_ALLRANK, -1 );
			}
			else
			{
				
				g_curPC->rankInGuild = -1;
				g_curPC->rankIdxInGuild = -1;
				memset( g_curPC->guildName, 0, GUILDNAMESTRING );
				g_curPC->guildIdx = -1;
				g_DBGameServer->UpdateGuildIdx(g_curPC->userID,g_curPC->serverGroupID,g_curPC->characterID,g_curPC->guildIdx);
			}
		}
		else
		{
			GTH_SendMessage_Guild_Connect_Reqeust_To_ManagerServer( g_curPC->guildIdx, g_curPC->name );
		}
	}

	
	if ( g_config.bCheckBilling )
	{
		Billing_SendGameStart( g_curPC );
	}

	
	if(NULL != gcpTerraBillingCtrl){
		gcpTerraBillingCtrl->Player_GameStart(g_curPC);
	}
	
	
	if ( g_config.isDanBattleMap )														
	{
		if (g_curPC->worldIdx == DAN_BATTLEMAP_NO )
		{
			
			int Status = g_pDanBattleSys->GetStatus();
			if ( Status == DBTL_STATUS_EVENT )			
				g_pDanBattleManager->SendGSM_Refresh_PersonData(g_curPC, 3, 5);
			else
				g_pDanBattleManager->SendGSM_Refresh_PersonData(g_curPC, 3, Status);

			 g_pDanBattleSys->InsertTeam( g_curPC );
		}		

	}
	
	
	
	g_DBGameServer->GetMailState( g_curPC );
	
	g_DBGameServer->PostSystem_LoadPackageCnt(g_curPC);

	
	
	if ( g_ChristMasEvent.m_EventState == CChristMasEvent::EVENT_ACTIVE)
	{			
		g_ChristMasEvent.GTH_SendMessage_Reply_WordlState(g_curPC, 
			TRUE, (int)g_ChristMasEvent.m_EventWorld[g_curPC->worldIdx].m_EventState);
	}
	

	
	g_WeatherSystem.GTH_SendMessage_Reply_WordlState(g_curPC, 
		TRUE, (int)g_WeatherSystem.m_EventWorld[g_curPC->worldIdx].m_WeatherType);
	
	
	
	GTH_CollectItemItem(g_curPC);
	GTH_SendMessage_ItemSyncTime(g_curPC);	
	g_curPC->m_ItemExtendInfo.fItemInfoRequestTime = 
		g_timer->GetAppMilliTime() + sItemUseTimeInfo::enumItemSyncInfo::ITEM_USETIME_SYNC_TERM;
	
			

	
	GTH_DeleteNoticePC(g_curPC);
	GTH_SendMessage_ClearWorldChageMode(g_curPC);

	
	ClearDanBattleInfo(g_curPC);
	GTH_ProcessDanBattleForReJoin(g_curPC);
	
	
	
	for (int ShareWinInventoryIdx =0; ShareWinInventoryIdx < \
		playerCharacter_t::tagGonryunBattle::tagShareWin::MAX_INVENTORY_NUMBER; 
		ShareWinInventoryIdx++)	
	g_curPC->GonryunBattle.ShareWin.Inventory[ShareWinInventoryIdx] = -1;

			
	
	InitItemCraftSystem(g_curPC);	
	InitItemUpgradeSystem(g_curPC);	
	InitBoothSystem(g_curPC);
	
	
	

	
	g_curPC->iChangeWorldIdx = -1;
	g_curPC->fWorldChangeStartTime = 0.0f;
	
}

#include "..\tagGCPacket.h"
void GTH_ProcessMessage_TransportWorld()
{
	int worldIdx, gateIdx;
	int  memberIdx;

	vec3_t spawnPosition, savePosition;
	float spawnYaw, saveYaw;
	int saveWorldIdx;


	worldIdx = MSG_ReadByte();
	gateIdx = MSG_ReadByte();	
	


	if (worldIdx >= g_worldTableNumber)
	{
		GTH_SendPCMessage_TransportReply( g_curPC,tagGCPacket_TRANSPORTWORLDREP::enumCode::fail);	
		g_logSystem->Write("User (%s) Move to Not Exist World(%d).", g_curPC->userID, worldIdx);
		return; 
	}

	if ( g_curPC->worldIdx == worldIdx )
	{
		GTH_SendPCMessage_TransportReply( g_curPC,tagGCPacket_TRANSPORTWORLDREP::enumCode::fail );	
		g_logSystem->Write("User (%s) Move To Same World.", g_curPC->userID);
		return; 
	}


	if((TRUE == g_config.bPremium) &&
	   (FALSE == gcpPremiumPlayerCtrl->isAccessMap(worldIdx,g_curPC))){
		GTH_SendPCMessage_TransportReply( g_curPC,tagGCPacket_TRANSPORTWORLDREP::enumCode::access_deny_because_free_user);
		return;
	}

	

	
	if ( g_curPC->busyState != BUSY_STATE_NONE) 
	{
		
		if (g_curPC->busyState != BUSY_STATE_WORLDCHAGNE)
		{		
			GTH_SendPCMessage_TransportReply( g_curPC,tagGCPacket_TRANSPORTWORLDREP::enumCode::busy_state );	
			g_logSystem->Write("User (%s) Don't Move. User Busy State.", g_curPC->userID);
			return;
		}
	
	}
	

	
	if ( GTH_IsLocalServiceWorld(worldIdx) )
	{
		
		GTH_Zone_DeleteEntityList(g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->entityType, g_curPC->idx);

		
		if(g_curPC->worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index)	
		{
			g_curPC->worldIdx = worldIdx;
			vec_copy( g_binding[0].pos, g_curPC->position);
		}
		else
		{
			
			g_curPC->worldIdx = worldIdx;
			vec_copy( g_worldTable[worldIdx].spawnPos[gateIdx], g_curPC->position);
			g_curPC->angles[1] = g_worldTable[worldIdx].spawnYaw[gateIdx];
		}
		

		
		g_curPC->reliableWorldIdx = g_curPC->worldIdx;
		vec_copy( g_curPC->position, g_curPC->reliablePosition );


		g_curPC->zoneIdx = GTH_Zone_GetZoneIdx(g_curPC->position);

		
		GTH_Zone_AddEntityList(g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->entityType, g_curPC->idx);

		g_curPC->ready = false;
		g_curPC->event = GTH_EV_CHAR_IDLE;

		
		GTH_SendPCMessage_TransportReply( g_curPC,tagGCPacket_TRANSPORTWORLDREP::enumCode::success);
		g_logSystem->Write("User (%s) Move To World %d", g_curPC->userID, g_curPC->worldIdx);

		
		g_curPC->m_WorldChangeSystem.m_SystemMode = 
			WorldChangeSystem::enumWorldChangeMode::WORLD_CHANGESYSTEM_NONE;
		
				
		
	}
	else	
	{
		memberIdx = GTH_FindServerWithinGroup( worldIdx );
		if (memberIdx < 0) 
		{
			GTH_SendPCMessage_TransportReply( g_curPC,tagGCPacket_TRANSPORTWORLDREP::enumCode::fail);	
			return;
		}
		else
		{
			
			
			
			
			
			
			

			gateIdx = GTH_World_GetGateIdx(worldIdx, g_curPC->worldIdx);
			if (gateIdx >= 0)
			{
				vec_copy( g_worldTable[worldIdx].spawnPos[gateIdx], spawnPosition );
				spawnYaw = g_worldTable[worldIdx].spawnYaw[gateIdx];
			}
			else
			{
				vec_copy( g_worldTable[worldIdx].startPosition, spawnPosition );
				spawnYaw = g_worldTable[worldIdx].startYaw;
			}

			
			saveWorldIdx = g_curPC->worldIdx;
			vec_copy(g_curPC->position, savePosition);
			saveYaw = g_curPC->angles[1];

			
			g_curPC->worldIdx = worldIdx;
			vec_copy(spawnPosition, g_curPC->position);
			g_curPC->angles[1] = spawnYaw;

			
			g_curPC->flagChangeServer = 1;
			g_curPC->changeServerWorldIdx = worldIdx;
			DB_SaveGameData( g_curPC, SAVEGAMEDATA_QUIT);

			
			g_curPC->worldIdx = saveWorldIdx;
			vec_copy(savePosition, g_curPC->position);
			g_curPC->angles[1] = saveYaw;

			
			g_curPC->m_WorldChangeSystem.m_SystemMode = 
				WorldChangeSystem::enumWorldChangeMode::WORLD_CHANGESYSTEM_NONE;
			

		}
	}
}

void GTH_ChangeServer( playerCharacter_t *pc )
{
	int memberIdx;

	memberIdx = GTH_FindServerWithinGroup( pc->changeServerWorldIdx ); 
	g_logSystem->Write("User (%s) Move to No [%d] Server", pc->userID, memberIdx);
	GTH_SendPCMessage_ChangeServer( pc, &g_memberServer[memberIdx] );

	
	if( pc->organizeServer >= 0 )
	{
		if( pc->isLeader ) GTH_PartyInfo_Request_Holding( pc, 1 );
		else GTH_PartyInfo_Request_Update( pc, false );
	}
}

void GTH_ProcessMessage_DropItem()
{
	item_t *item;
	int itemIdx, itemObjectIdx;
	int reply, error;
	vec3_t position;
	
	int bIsStack = false;
	int stackNumber = 1;	

	itemIdx = MSG_ReadByte();
	
	itemIdx = g_curPC->mouseInventory;

	MSG_ReadPosition( position ); 
	if( g_curPC->busyState != BUSY_STATE_NONE )
	{
		g_logSystem->Write("Drop Error!! : %s(%s), busyState : %d", g_curPC->name, g_curPC->userID, g_curPC->busyState);

		reply = 0;
		error = ERROR_DROPITEM_5;

		goto REPLY_DROPITEM;
	}

	
	if (itemIdx < 0 || itemIdx >= MAX_NUMBER_OF_OWNITEM)
	{
		g_logSystem->Write("Index Out of Range.(itemIdx:%d)", itemIdx);
		reply = 0;
		error = ERROR_DROPITEM_1;

		goto REPLY_DROPITEM;
	}
	
	item = &g_curPC->item[itemIdx];	


	if ( item->itemTableIdx == -1 ) 	
	{
		g_logSystem->Write("Can Not Drop Item(Not Exist).( char name:%s, item: %d )", g_curPC->name, itemIdx);
		reply = 0;
		error = ERROR_DROPITEM_2;
		goto REPLY_DROPITEM;
	}

	if( g_itemTable[item->itemTableIdx].stackFlag )
	{
		bIsStack = true;
		stackNumber = item->durability + 1;
	}

	itemObjectIdx = GTH_SpawnItemObject(item, g_curPC->worldIdx, position, LOOTTYPE_INDIVIDUAL, g_curPC->idx);
	if ( itemObjectIdx < 0 )
	{
		g_logSystem->Write("Can Not Spawn Item Object.");
		reply = 0;
		error = ERROR_DROPITEM_3;
		goto REPLY_DROPITEM;
	}

	
	g_logSystem->Write("User '%s'(ID: %s) Drop Item '%s' ", g_curPC->name, g_curPC->userID, g_itemTable[item->itemTableIdx].name);
	g_curPC->curItemWeight -= g_itemTable[item->itemTableIdx].weight;

	
	
	
	g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
		LOG_CLASS_ITEM_DROP, stackNumber, item->name, 
		GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, -1 ) );
	

	
	GTH_DeleteMyItem( g_curPC, item );
	g_curPC->mouseInventory = -1;

	g_DBGameServer->RemoveItem(g_curPC, itemIdx);
	
	reply = 1;
	error = ERROR_DROPITEM_NONE;


REPLY_DROPITEM:
	GTH_SendMessage_ReplyDropItem( g_curPC, reply, error, itemIdx );

}

void GTH_ProcessMessage_PickUpItem()
{
	item_t *item;
	itemObject_t *itemObject;
	int itemIdx, itemObjectIdx;
	int reply;
	int error = ERROR_PICKUPITEM_NONE;
	int inventoryPos;
	int stackItemIdx;
	item_t* upItem;
	
	int bIsStack = false;
	int stackNumber = 1;


	
	char generateID[IDSTRING+1]; 

	
	
	itemObjectIdx = MSG_ReadShort();


	if( g_curPC->busyState != BUSY_STATE_NONE )
	{
		g_logSystem->Write("Pick Up Error!! : %s(%s), busyState : %d", g_curPC->name, g_curPC->userID, g_curPC->busyState);
		reply = 0;
		error = ERROR_PICKUPITEM_8;
		goto REPLY_PICKUPITEM;
	}

	
	if (itemObjectIdx < 0 || itemObjectIdx >= MAX_OBJECTS)
	{
		g_logSystem->Write("Item Object,Index Out of Range.(objectIdx:%d)", itemObjectIdx);
		reply = 0;
		error = ERROR_PICKUPITEM_1;
		goto REPLY_PICKUPITEM;
	}

	itemObject = &g_object[ itemObjectIdx ];


	float dist;
	dist = GTH_GetDistance2D(itemObject->position, g_curPC->position);
	if ( dist > AVAILABLE_DISTANCE_PICKUPITEM * 0.5f){
		return;
	}


	if ( !itemObject->active ) 	
	{
		g_logSystem->Write("Select Lost Item Object.(objectIdx:%d)", itemObjectIdx);
		reply = 0;
		error = ERROR_PICKUPITEM_2;
		goto REPLY_PICKUPITEM;
	}

	
	if ( !GTH_CheckLootingValidation(g_curPC, itemObject) )
	{
		reply = 0;
		error = ERROR_PICKUPITEM_3;
		goto REPLY_PICKUPITEM;
	}

	if( itemObject->objectType == OBJECT_TYPE_PORTAL )
	{
		GTH_TransportWorldWithPortal( g_curPC, itemObject );
		return;
	}
	
	
	if( itemObject->objectType == OBJECT_TYPE_NAK )
	{
		GTH_PickUpNak( g_curPC, itemObject );
		return;
	}

	
	


	

	
	
	
			
			
			
			
			
	

	
	item = &itemObject->item;
	if( g_itemTable[item->itemTableIdx].stackFlag ) 
	{
		bIsStack = true;
		stackNumber = item->durability + 1;
	}

	
	if( g_curPC->curItemWeight > g_curPC->calItemWeight )
	{
		reply = 0;
		error = ERROR_PICKUPITEM_7;
		goto REPLY_PICKUPITEM;
	}

	stackItemIdx = GTH_StackMyItem( g_curPC, item );
	if( stackItemIdx >= 0 )
	{
		upItem = &g_curPC->item[stackItemIdx];
		upItem->durability += item->durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;

		g_DBGameServer->SaveItem( g_curPC, upItem, stackItemIdx );

		
		
		
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_PICKUP, stackNumber, item->name, 
			GetItemInformationLog( bIsStack, item->itemTableIdx, stackItemIdx, -1 ) );
		
	}
	else
	{
		
		inventoryPos = GTH_FindEmptyInventoryPos( g_curPC );
		if( inventoryPos < 0 )	
		{
			reply = 0;
			error = ERROR_PICKUPITEM_4;

			g_logSystem->Write("Pick Up Error!! : Inventory Space Not Enough. %s(%s), objectIdx:%d)", g_curPC->name, g_curPC->userID, itemObjectIdx);
			goto REPLY_PICKUPITEM;
		}

		
		itemIdx = GTH_AddMyItem( g_curPC, item );
		if (itemIdx < 0)	
		{
			reply = 0;
			error = ERROR_PICKUPITEM_5;
			goto REPLY_PICKUPITEM;
		}
		
		
		g_curPC->inventory[inventoryPos] = itemIdx;
		
		strncpy(generateID, "pickUpGenID",IDSTRING);
		generateID[IDSTRING]=NULL;

		
		if ( !g_DBGameServer->AddItem(g_curPC, item, itemIdx, generateID) )
		{
			g_logSystem->Write("Can Not Insert in DataBase(Item Table).(%s, %d, %d)", g_curPC->userID, g_curPC->serverGroupID, g_curPC->characterID);
			reply = 0;
			error = ERROR_PICKUPITEM_6;
			goto REPLY_PICKUPITEM;
		}

		
		
		
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_PICKUP, stackNumber, item->name, 
			GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, inventoryPos ) );
		
	}

	
	g_logSystem->Write("User '%s'(ID: %s) Obtain Item '%s'", g_curPC->name, g_curPC->userID, g_itemTable[item->itemTableIdx].name);

	
	GTH_RemoveItemObject( itemObject );
	reply = 1;
	error = ERROR_PICKUPITEM_NONE;

	g_curPC->curItemWeight += g_itemTable[item->itemTableIdx].weight;

REPLY_PICKUPITEM:
	GTH_SendMessage_ReplyPickUpItem( g_curPC, reply, error, item );
}

int GTH_FindEmptyInventoryPos(playerCharacter_t *pc)
{
	int i;

	for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		if( pc->inventory[i] == -1 )
		{
			return i;
		}
	}
	return -1;
}


int GTH_FindEmptyDepotPos(playerCharacter_t *pc)
{
	int i;

	for( i = 0; i < MAX_DEPOT_SIZE; i ++ )
	{
		if( pc->depot[i] == -1 )
		{
			return i;
		}
	}
	return -1;
}

int GTH_CheckLootingValidation(playerCharacter_t *pc, itemObject_t *object)
{
	switch ( object->lootType )
	{
	case LOOTTYPE_INDIVIDUAL:
		{
			if ( object->lootPCIdx != -1)
			{
				if ( object->lootPCIdx == pc->idx ) return true;
			}
		}
		break;

	case LOOTTYPE_PARTY:
		{
			
			if( object->lootPCIdx >= 0 && pc->organizeServer >= 0 )
			{
			
				
				if( stricmp( object->organizerName, pc->organizerName ) ) return false;
				if( object->organizeServer != pc->organizeServer ) return false;
				if( object->organizeTime != pc->organizeTime ) return false;
				
				return true;
			}
		}
		break;

	case LOOTTYPE_EVERYONE:
		{
			return true;
		}
		break;
	
	case LOOTTYPE_DANBATTLE_TEAMS:
		{	
			
			LPREQUEST_RECORD request	= NULL;
			LPREQUEST_RECORD sugueest	= NULL;

			request = static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetBattleRequestDan());
			sugueest = static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetBattleSuggestDan());

			if ( request == NULL || sugueest == NULL)
				return false;

			if ( !strcmp(request->DanName, pc->guildName) )
				return true;

			if ( !strcmp(sugueest->DanName, pc->guildName) )
				return true;

			return false;			
		}break;

	case LOOTTYPE_DAN:
		{
			
			LPREQUEST_RECORD dan	= NULL;
			if (!strcmp(object->DanName, pc->guildName))
				return true;
			

			return false;
		}break;
	}		
	
	return false;
}


bool CheckEpack(int _InventoryPos, int _EpackIdx)
{
	
	if(_EpackIdx < 0 || _EpackIdx >= MAX_NUMBER_OF_OWNITEM) return false;

	int epkItemTableIdx = g_curPC->item[_EpackIdx].itemTableIdx;

	if(epkItemTableIdx < 0 || epkItemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return false;
	
	if(g_itemTable[epkItemTableIdx].epkPackType != TypeItemTable_t::EPK_TYPE_MUCH_CAPACITY) return false;

	
	if(g_itemTable[epkItemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_UNCHARGE) return false;

	
	int EpkRemainCount = gcpTools->GetEpkChargeCount(&g_curPC->item[_EpackIdx]);
	if(EpkRemainCount > 0) return false;
	
	
	
		
	GTH_DeleteMyItem( g_curPC, &g_curPC->item[_EpackIdx] );

	if(_InventoryPos >= 0 && _InventoryPos < MAX_INVENTORY_SIZE)
	{
		g_curPC->inventory[_InventoryPos]= -1;
	}

	g_DBGameServer->RemoveItem( g_curPC, _EpackIdx );
	return true;
}


int GTH_ProcessMessage_RequestGetInventoryItem()
{
	int directEquip, equipPos = 0, tableEquipPos, itemTableIdx;
	int inventoryPos = 0, numStack;
	int itemIdx;
	int reply = 1, error = 0;

	
	directEquip = MSG_ReadByte();
	inventoryPos = MSG_ReadByte();
	numStack = MSG_ReadShort();

	
	if(inventoryPos < 0 || inventoryPos >= MAX_INVENTORY_SIZE) return 0;

	
	itemIdx = g_curPC->inventory[inventoryPos];
	
	
	if( itemIdx >= 0 )
	{
		if( g_curPC->item[itemIdx].itemTableIdx < 0 )
		{
			g_curPC->inventory[inventoryPos] = -1;
			reply = 8;
			GTH_SendMessage_ReplyGetInventoryItem( g_curPC, reply, error, inventoryPos, equipPos );
			return 0;
		}
	}
	

	
	if( !directEquip )
	{
		if( itemIdx < 0 && g_curPC->mouseInventory < 0 ) 
		{
			reply = 0;
		}
		else if( itemIdx >= 0 && g_curPC->mouseInventory < 0 )
		{
			if( GTH_IsStackItem( &g_curPC->item[itemIdx] ) && numStack > 0 )
			{
				reply = 5;
				GTH_GetStackItem( g_curPC, reply, error, inventoryPos, numStack );
			}
			else
			{
				reply = 1;
				g_curPC->inventory[inventoryPos] = -1;
				g_curPC->mouseInventory = itemIdx;
			}
		} 
		else if( itemIdx < 0 && g_curPC->mouseInventory >= 0 )
		{
			int stackItemIdx = GTH_StackMyItem( g_curPC, &g_curPC->item[g_curPC->mouseInventory] );

			
			if( GTH_IsStackItem( &g_curPC->item[g_curPC->mouseInventory] ) && stackItemIdx >= 0 )
			{
				reply = 6;
				GTH_SetStackItem( g_curPC, reply, error );
			}
			else
			{
				reply = 2;
				g_curPC->inventory[inventoryPos] = g_curPC->mouseInventory;
				g_curPC->mouseInventory = -1;
			}
		} 
		else if( itemIdx >= 0 && g_curPC->mouseInventory >= 0 )
		{
			
			if( GTH_IsStackItem( &g_curPC->item[g_curPC->mouseInventory] ) )
			{
				reply = 6;
				GTH_SetStackItem( g_curPC, reply, error );
			}
			else
			{
				reply = 3;
				g_curPC->inventory[inventoryPos] = g_curPC->mouseInventory;
				g_curPC->mouseInventory = itemIdx;
			}
		}
		else 
		{
			reply = 0;
		}
	}
	else
	{
		if( itemIdx < 0 ) 
		{
			reply = 0;
			error = 1;
		}
		else
		if( g_curPC->mouseInventory >= 0 ) 
		{
			reply = 0;
			error = 3;
		}
		else
		{
			
			if( !GTH_IsConsumptiveItem( &g_curPC->item[itemIdx] ) )
			{
				
				itemTableIdx = g_curPC->item[itemIdx].itemTableIdx;
				tableEquipPos = g_itemTable[itemTableIdx].wearPosition;

				
				
				if ( TRUE == g_ChristMasEvent.GTH_IsChristMasItem(itemTableIdx) )
				{
					if ( FALSE == g_ChristMasEvent.GTH_IsEnableEquipChristMasItem() )
					{
						reply = 0;					
						error = ERROR_EQUIPITEM_7;
						GTH_SendMessage_ReplyGetInventoryItem( g_curPC, reply, error, inventoryPos, equipPos );
						return FALSE;
					}
				}
				else
				{
					if ( TRUE == g_ChristMasEvent.GTH_IsEquipChristMasItem() &&
						FALSE == g_ChristMasEvent.GTH_IsAllowItemWerePos(itemTableIdx))

					{						
						reply = 0;					
						error = ERROR_EQUIPITEM_8;
						GTH_SendMessage_ReplyGetInventoryItem( g_curPC, reply, error, inventoryPos, equipPos );
						return FALSE;						
					}
				}

				
				
				
				equipPos = GTH_GetEquipPos( &g_curPC->item[itemIdx], equipPos );
				if( equipPos < 0 || !GTH_CheckRequire( g_curPC, 0, &g_curPC->item[itemIdx] ) ) 
				{
					reply = 0;
					error = ERROR_EQUIPITEM_1;
				}
				else
				{
					
					reply = 4;
					g_curPC->inventory[inventoryPos] = g_curPC->equipment[equipPos];
					g_curPC->equipment[equipPos] = itemIdx;

					
					if(equipPos == EQUIP_EPACK)
					{
						if(CheckEpack(inventoryPos,g_curPC->inventory[inventoryPos]))
						{
							
							reply=9;
						}
					}
				}
			}
			else
			{
				if( !GTH_ConsumptiveItemProcess( g_curPC, inventoryPos ) )
				{
					reply = 0;
					error = ERROR_EQUIPITEM_4;
				}
				else
				{
					
					reply = 7;
				}
			}
		}
	}

	
	if( reply )
	{
		if( g_curPC->mouseInventory >= 0 ) g_curPC->mouseInventoryType = MOUSEINVEN_TYPE_INVENTORY;
		else g_curPC->mouseInventoryType = MOUSEINVEN_TYPE_NONE;
	}

	GTH_SendMessage_ReplyGetInventoryItem( g_curPC, reply, error, inventoryPos, equipPos );
	return 1;
}

int GTH_ProcessMessage_RequestGetEquipItem()
{
	int directEquip, equipPos;
	int itemIdx;
	int reply, error = 0;
	int itemTableIdx, tableEquipPos, temp, inventoryPos = 0;

	directEquip = MSG_ReadByte();
	equipPos = MSG_ReadByte();

	
	itemIdx = g_curPC->equipment[equipPos];

	if( !directEquip )
	{
		if( itemIdx < 0 && g_curPC->mouseInventory < 0 ) 
		{
			reply = 0;
		}
		else if( itemIdx >= 0 && g_curPC->mouseInventory < 0 )
		{
			g_curPC->mouseInventory = itemIdx;
			g_curPC->equipment[equipPos] = -1;
			reply = 1;
		} 
		else if( itemIdx < 0 && g_curPC->mouseInventory >= 0 )
		{
			
			itemTableIdx = g_curPC->item[g_curPC->mouseInventory].itemTableIdx;
			tableEquipPos = g_itemTable[itemTableIdx].wearPosition;

					if ( TRUE == g_ChristMasEvent.GTH_IsChristMasItem(itemTableIdx) )
			{
				if ( FALSE == g_ChristMasEvent.GTH_IsEnableEquipChristMasItem() )
				{
					reply = 0;					
					error = ERROR_EQUIPITEM_7;
					GTH_SendMessage_ReplyGetEquipItem( g_curPC, reply, error, inventoryPos, equipPos );
					return FALSE;
				}
			}
			else
			{
				if ( TRUE == g_ChristMasEvent.GTH_IsEquipChristMasItem() &&
					FALSE == g_ChristMasEvent.GTH_IsAllowItemWerePos(itemTableIdx) )
				{						
					reply = 0;					
					error = ERROR_EQUIPITEM_8;
					GTH_SendMessage_ReplyGetEquipItem( g_curPC, reply, error, inventoryPos, equipPos );
					return FALSE;						
				}
			}
			
			
			
			
			equipPos = GTH_GetEquipPos( &g_curPC->item[g_curPC->mouseInventory], equipPos );
			if( equipPos < 0 || !GTH_CheckRequire( g_curPC, 0, &g_curPC->item[g_curPC->mouseInventory] ) ) 
			{
				reply = 0;
				error = ERROR_EQUIPITEM_1;
			}
			else
			{
				itemIdx = g_curPC->equipment[equipPos];
				
				if( itemIdx < 0 )
				{
					g_curPC->equipment[equipPos] = g_curPC->mouseInventory;
					g_curPC->mouseInventory = -1;
					reply = 2;
				}
				else
				{
					temp = g_curPC->equipment[equipPos];
					g_curPC->equipment[equipPos] = g_curPC->mouseInventory;
					g_curPC->mouseInventory = temp;
					reply = 3;
				}
			}
		}
		else if( itemIdx >= 0 && g_curPC->mouseInventory >= 0 )
		{
			
			itemTableIdx = g_curPC->item[g_curPC->mouseInventory].itemTableIdx;
			
			equipPos = GTH_GetEquipPos( &g_curPC->item[g_curPC->mouseInventory], equipPos );
			
			if ( TRUE == g_ChristMasEvent.GTH_IsChristMasItem(itemTableIdx) )
			{
				if ( FALSE == g_ChristMasEvent.GTH_IsEnableEquipChristMasItem() )
				{
					reply = 0;					
					error = ERROR_EQUIPITEM_7;
					GTH_SendMessage_ReplyGetEquipItem( g_curPC, reply, error, inventoryPos, equipPos );
					return FALSE;
				}
			}
			else
			{
				if ( TRUE == g_ChristMasEvent.GTH_IsEquipChristMasItem() && 
					FALSE == g_ChristMasEvent.GTH_IsAllowItemWerePos(itemTableIdx) )
				{						
					reply = 0;					
					error = ERROR_EQUIPITEM_8;
					GTH_SendMessage_ReplyGetEquipItem( g_curPC, reply, error, inventoryPos, equipPos );
					return FALSE;						
				}
			}
			
			
			
			if( equipPos < 0 || !GTH_CheckRequire( g_curPC, 0, &g_curPC->item[g_curPC->mouseInventory] ) ) 
			{
				reply = 0;
			}
			else
			{
				itemIdx = g_curPC->equipment[equipPos];
				
				if( itemIdx < 0 )
				{
					g_curPC->equipment[equipPos] = g_curPC->mouseInventory;
					g_curPC->mouseInventory = -1;
					reply = 2;
				}
				else
				{
					if( equipPos == EQUIP_EPACK ) GTH_PC_ChangeEnergyPack( &g_curPC->item[g_curPC->mouseInventory] );
					
					temp = g_curPC->equipment[equipPos];
					g_curPC->equipment[equipPos] = g_curPC->mouseInventory;
					g_curPC->mouseInventory = temp;
					reply = 3;
				}
			}
		}
	}
	
	else
	{
		if( itemIdx < 0 )
		{
			reply = 0;
			error = 1;			
		}
		else
		if( g_curPC->mouseInventory >= 0 )
		{
			reply = 0;
			error = 3;
		}
		else
		{
			
			inventoryPos = GTH_FindEmptyInventoryPos( g_curPC );
			if( inventoryPos < 0 )
			{
				reply = 0;
				
				error = 2;
			}
			else
			{
				reply = 4;
				g_curPC->inventory[inventoryPos] = itemIdx;
				g_curPC->equipment[equipPos] = -1;
			}
		}
	}

	
	if( reply )
	{
		if( g_curPC->mouseInventory >= 0 ) g_curPC->mouseInventoryType = MOUSEINVEN_TYPE_EQUIP;
		else g_curPC->mouseInventoryType = MOUSEINVEN_TYPE_NONE;
	}

	GTH_SendMessage_ReplyGetEquipItem( g_curPC, reply, error, inventoryPos, equipPos );
	return 1;
}

void GTH_ProcessMessage_UpdatePosition()
{
	vec3_t oldPosition;
	float height;

	int zoneIdx;

	if( !g_curPC->ready ) return;

	vec_copy( g_curPC->position, oldPosition );

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );
	}

	height = g_curPC->position[2] - oldPosition[2];
	










}


void GTH_ProcessMessage_Suicide()
{
	int zoneIdx;

	if( !g_curPC->ready ) return;

	MSG_ReadPosition( g_curPC->position );
	g_curPC->angles[1] = MSG_ReadFloat();
	
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
	
	if (g_curPC->zoneIdx != zoneIdx)
	{
		g_curPC->zoneIdx = zoneIdx;
		GTH_SendMessage_SyncItemObject( g_curPC );

		g_curPC->curRA = 0.0f;

		g_curPC->event = GTH_EV_CHAR_DIE;
		g_curPC->recoveryTime = 0.0f;
		
		GTH_PC_ReduceExp( g_curPC, 0.1f );
	}
}

void GTH_ProcessMessage_WeaponChange()
{
	int curWeapon, nextWeapon;
	curWeapon = g_curPC->curWeapon;
	if( curWeapon == 0 ) nextWeapon = 1;
	else nextWeapon = 0;

	g_curPC->curWeapon = nextWeapon;

	
	GTH_PC_Calculate( g_curPC, PCINFO_BASIC );

	GTH_SendPCMessage_ReplyWeaponChange( g_curPC );
}

void GTH_ProcessMessage_GenChange()
{
	int gen;
	gen = MSG_ReadByte();

	if( g_curPC->genCapability[gen] >= 0 )
	{
		g_curPC->selectedGen = gen;
	}

	GTH_PC_Calculate( g_curPC );

	GTH_SendPCMessage_ReplyGenChange( g_curPC );
}



void GTH_ProcessMessage_StatusUp()
{
	int reply = 1, error = 0;
	int statusType;
	statusType = MSG_ReadShort();

	error = GTH_PC_StatusUp( statusType );
	
	if( error > 0 )
	{
		reply = 0;
		GTH_SendPCMessage_ReplyStatusUp( g_curPC, reply, error, statusType );
		return;
	}

	
	float curRA, maxRA, curSA, maxSA;
	float rateRA, rateSA;

	curRA = g_curPC->curRA;
	maxRA = g_curPC->calMaxRA;
	rateRA = curRA / maxRA;

	curSA = g_curPC->curSA;
	maxSA = g_curPC->calMaxSA;
	rateSA = curSA / maxSA;

	GTH_PC_Calculate( g_curPC );

	curRA = g_curPC->calMaxRA * rateRA;
	g_curPC->curRA = curRA;
	curSA = g_curPC->calMaxSA * rateSA;
	g_curPC->curSA = curSA;

	GTH_SendPCMessage_ReplyStatusUp( g_curPC, reply, error, statusType );
}

void GTH_ProcessMessage_EpackUse()
{
	int type;

	
	type = MSG_ReadByte();
	
	if( !g_curPC->alive ) return;
	switch( type )
	{
	
	case 0 :
		
		if( (int)g_curPC->curChargeRA <= 0 ) return;
		if( g_curPC->curRA >= g_curPC->calMaxRA ) return;
		if( g_curPC->epackUseRAWaitTime > g_globalTime ) return;

		g_curPC->curChargeRA --;
		g_curPC->curRA += (float)g_curPC->onceRecoveryRA;
		g_curPC->curRA += (float)g_curPC->onceRecoveryRA * g_curPC->optValue[OV_EPACK_RA_EFFICIENCY] * 0.01f;
		if( g_curPC->curRA > g_curPC->calMaxRA ) g_curPC->curRA = g_curPC->calMaxRA;

		g_curPC->epackUseRAWaitTime = g_globalTime + PC_EPACK_USE_RA_DELAY;





		GTH_SendPCMessage_ReplyEpackUse( g_curPC, type );
		break;
	case 1 :

		
		if( (int)g_curPC->curChargeSA <= 0 ) return;

		if( g_curPC->curSA >= g_curPC->calMaxSA ) return;
		if( g_curPC->epackUseSAWaitTime > g_globalTime ) return;

		g_curPC->curChargeSA --;
		g_curPC->curSA += (float)g_curPC->onceRecoverySA;
		g_curPC->curSA += (float)g_curPC->onceRecoverySA * g_curPC->optValue[OV_EPACK_SA_EFFICIENCY] * 0.01f;

		if( g_curPC->curSA >= g_curPC->calMaxSA ) g_curPC->curSA = g_curPC->calMaxSA;
		
		g_curPC->epackUseSAWaitTime = g_globalTime + PC_EPACK_USE_SA_DELAY;




		
		GTH_SendPCMessage_ReplyEpackUse( g_curPC, type );
		break;
	}
}


void GTH_PC_SetSummonsInfo(playerCharacter_t *pc, char *name, int worldIdx, vec3_t position, int forcedYn)
{
	strcpy(pc->summonsInfo.summoner, name);
	pc->summonsInfo.worldIdx = worldIdx;
	vec_copy(position, pc->summonsInfo.position);
	pc->summonsInfo.forcedYn = forcedYn;
}

void GTH_PC_InitSummonsInfo(playerCharacter_t *pc)
{
	memset(&pc->summonsInfo, 0, sizeof(summons_t));
}


void GTH_ProcessMessage_ReplySummons(playerCharacter_t *pc,bool _bItemUse)
{
	
	int reply=1, worldIdx, zoneIdx, memberIdx;

	vec3_t position;

	vec3_t savePosition;
	int saveWorldIdx;

	
	if(false == _bItemUse)
	{
		reply = MSG_ReadByte();
	}

	worldIdx = pc->summonsInfo.worldIdx;
	vec_copy( pc->summonsInfo.position, position );

	if( g_curPC->isTransform ) GTH_DeleteTransform( g_curPC );

	if ( reply == 1 ) 
	{
		if ( GTH_IsLocalServiceWorld(worldIdx) )
		{
			if( worldIdx == pc->worldIdx ) 
			{
				vec_copy( position, pc->position );

				
				pc->reliableWorldIdx = pc->worldIdx;
				vec_copy( pc->position, pc->reliablePosition );

				zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
				pc->zoneIdx = zoneIdx;

				GTH_SendPCEventMessage_Respawn( pc );
				GTH_SendMessage_SyncItemObject( pc );
			}
			else 
			{
				
				GTH_Zone_DeleteEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);

				pc->worldIdx = worldIdx;
				vec_copy( position, pc->position );

				
				pc->reliableWorldIdx = pc->worldIdx;
				vec_copy( pc->position, pc->reliablePosition );

				pc->zoneIdx = GTH_Zone_GetZoneIdx( pc->position );
				
				GTH_Zone_AddEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);

				GTH_SendPCEventMessage_Respawn( pc );
				pc->ready = false;

				
				g_logSystem->Write("User [%s] Move To Summonning Pos. Pos(%.2f, %.2f, %.2f), World : %d", pc->userID, pc->position[0], pc->position[1], pc->position[2], pc->worldIdx);
			}
		}
		else 
		{
			memberIdx = GTH_FindServerWithinGroup( worldIdx );
			if (memberIdx < 0) 
			{
				g_logSystem->Write("User [%s] Summon Error(worldIdx:%d)", pc->name, pc->worldIdx);
				return;
			}
			else
			{
				
				saveWorldIdx = pc->worldIdx;
				vec_copy(pc->position, savePosition);

				
				pc->worldIdx = worldIdx;
				vec_copy(position, pc->position);

				pc->flagChangeServer = 1; 
				pc->changeServerWorldIdx = worldIdx;
				DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);

				
				pc->worldIdx = saveWorldIdx;
				vec_copy(savePosition, pc->position);
			}
		}
	}

	
	GTH_SendMessage_ReplySummons(reply, pc->summonsInfo.summoner, pc->name);

	
	GTH_PC_InitSummonsInfo( pc );
}





void GTH_ProcessMessage_ActiveItem( int itemIdx )
{
	int reply = 1, error = 0;
	int raCount = 0, saCount = 0;
	int level;
	int itemTableIdx, number;
	item_t *item;

	int bIsStack = false, stackNumber = 1;
	
	if( itemIdx < 0 )
	{
		g_logSystem->Write("Active Failure!! : Item Not Exist, %s(%s)", g_curPC->name, g_curPC->userID );
		
		reply = 0;
		error = ERROR_DESTROYITEM_2;
		goto REPLY_DESTROYITEM;
	}
	if( g_curPC->equipment[EQUIP_EPACK] < 0 )
	{
		g_logSystem->Write("Destroy Failure!! : Item Not Exist, %s(%s)", g_curPC->name, g_curPC->userID );
		reply = 0;
		error = ERROR_DESTROYITEM_1;
		goto REPLY_DESTROYITEM;
	}
	if( g_curPC->busyState != BUSY_STATE_NONE )
	{
		g_logSystem->Write("Destroy Failure!! : %s(%s), busyState : %d", g_curPC->name, g_curPC->userID, g_curPC->busyState);

		reply = 0;
		error = ERROR_DESTROYITEM_3;
		goto REPLY_DESTROYITEM;
	}

	itemTableIdx = g_curPC->item[itemIdx].itemTableIdx;
	item = &g_curPC->item[itemIdx];

	
	
	number = item->durability + 1;
	level = item->reqLevel;

	switch( itemTableIdx )
	{
	
	
	case QUESTSCROLL_ITEM_INDEX :
		{
			int idx;
			int dField[MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD];
			memset(dField, 0, sizeof(dField));

			int diffLevel;

			
			if ( g_questscrollTable[item->optionIdx[0]].data.level > 10 )
				diffLevel = g_curPC->level - g_questscrollTable[item->optionIdx[0]].data.level;
			else
				diffLevel = 0;

			int ret = 0;
			switch ( g_questscrollTable[item->optionIdx[0]].data.scrollType )
			{
			case 1 :
				if ( g_curPC->pcClass != GEN_MENTALITY && g_curPC->pcClass != GEN_WIND )
					ret = 1;
				break;
			case 2 :
				if ( g_curPC->pcClass != GEN_WATER && g_curPC->pcClass != GEN_EARTH )
					ret = 1;
				break;
			default :
				break;
			}

			if ( ret == 1 )
			{
				reply = 0;
				error = ERROR_DESTROYITEM_9;
				goto REPLY_DESTROYITEM;
			}

			if ( diffLevel < 0 || diffLevel > 2 )
			{
				reply = 0;
				error = ERROR_DESTROYITEM_8;
				goto REPLY_DESTROYITEM;
			}

			if ( GTH_IsOnPlayQuestScroll( g_curPC ) )
			{
				reply = 0;
				error = ERROR_DESTROYITEM_6;			
				goto REPLY_DESTROYITEM;
			}

			idx = GTH_GetEmptyQuestStatus( g_curPC );	

			if( idx >= 0 )
			{
				
				dField[0] = g_questscrollTable[item->optionIdx[0]].Index;
				dField[1] = g_questscrollTable[item->optionIdx[0]].data.monsterIndex;		
				dField[2] = g_questscrollTable[item->optionIdx[0]].data.monsterNo;			
				dField[3] = 0;																
				dField[4] = g_questscrollTable[item->optionIdx[0]].data.scrollType;			
				dField[5] = item->optionIdx[1];												
				dField[6] = g_questscrollTable[item->optionIdx[0]].data.rewardField[item->optionIdx[1]];	
				dField[7] = g_questscrollTable[item->optionIdx[0]].data.fame;				
				dField[8] = g_questscrollTable[item->optionIdx[0]].data.reward.itemNo;		
				dField[9] = g_questscrollTable[item->optionIdx[0]].data.reward.optionGroup;	
				GTH_SetQuestStatus( g_curPC, idx, QUEST_SCROLL_INDEX, -1, 0, dField );
			}
			else
			{
				reply = 0;
				error = ERROR_DESTROYITEM_7;			
				goto REPLY_DESTROYITEM;
			}

			break;
		}

	default :
		break;
	}

	g_curPC->curItemWeight -= g_itemTable[item->itemTableIdx].weight;

	if( g_itemTable[item->itemTableIdx].stackFlag )
	{
		bIsStack = true;
		stackNumber = item->durability + 1;
	}

	
	
	
	g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
		LOG_CLASS_ITEM_DESTROY, stackNumber, item->name, 
		GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, -1 ) );
	

	g_logSystem->Write( "Item Destroy : %s(%s), item : %s Destroyed", g_curPC->name, g_curPC->userID, item->name );

	
	GTH_DeleteMyItem( g_curPC, item );
	g_curPC->mouseInventory = -1;

	g_DBGameServer->RemoveItem(g_curPC, itemIdx);
	
REPLY_DESTROYITEM :

	GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );

}

int GTH_RecoveryReliablePosition( playerCharacter_t *pc )
{
	int zoneIdx;
	if ( pc->worldIdx == pc->reliableWorldIdx ) 
	{
		vec_copy( pc->reliablePosition, pc->position );
		zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
		if (pc->zoneIdx != zoneIdx)
			pc->zoneIdx = zoneIdx;

	}
	return( pc->zoneIdx );
}

void GTH_SetServerAckMessage(playerCharacter_t *pc)
{
	pc->serverAckMsgNeedFlag = true;
	pc->serverAckMsgWaitTimer = g_globalTime + 1000;	
}

void GTH_ClearServerAckMessage(playerCharacter_t *pc)
{
	pc->serverAckMsgNeedFlag = false;
	pc->serverAckMsgWaitTimer = 0;
}

int GTH_CheckServerAckMessage( playerCharacter_t *pc )
{
	
	if ( pc->serverAckMsgNeedFlag )
	{
		if ( g_globalTime >= pc->serverAckMsgWaitTimer )
		{
			GTH_ClearServerAckMessage( pc );
			return( false );
		}
	}
	return( true );
}


void GTH_ProcessEventMessage_Teleport()
{
	int skillIdx, skillTargetType;
	float angle;

	vec3_t	movePos;

	skillIdx = MSG_ReadShort();

	if( skillIdx == CANCEL_TRANSFORM_SKILL_INDEX )
	{
		if( g_curPC->isTransform ) GTH_DeleteTransform( g_curPC );
	}
	else
	{
		skillTargetType		= MSG_ReadByte();
		MSG_ReadPosition( g_curPC->saveSkillTargetPos );
		
		g_curPC->targetIdx	= MSG_ReadShort();
		g_curPC->targetType = MSG_ReadByte();
		MSG_ReadPosition( g_curPC->position );

		angle = MSG_ReadFloat();
		g_curPC->angles[ YAW ] = angle;

		MSG_ReadPosition( movePos );

		
		if ( g_curPC->targetType == ENTITY_NPC )
			return;

		
		
		if ( g_curPC->targetType == ENTITY_PC && skillTargetType == SKILL_TARGET_MONSTER )
		{
			if( g_curPC->targetIdx >= 0 )
			{
				if( !( g_curPC->pvpMode && g_pc[g_curPC->targetIdx].pvpMode ) )
				{					




					
					
					if ( (g_curPC->DanBattleInfo.nDanBattleIdx >= 0 ) && 
						( g_curPC->DanBattleInfo.nBattleMapIdx == g_curPC->worldIdx ) ){
						if (FALSE == g_NewDanBattleSystem.IsDanBattleEnemy(g_curPC->idx, g_curPC->targetIdx))
						return;
					}
					
					else{						
						if ( IsPKDisableWorld( g_curPC->worldIdx ) ) return;
						
						
						playerCharacter_t* pTargetPC = gcpTools->GetPlayerRecordPointer(g_curPC->targetIdx);
						if ( pTargetPC != NULL)
							if ( pTargetPC->DanBattleInfo.nDanBattleIdx >= 0) 	return;
					}				






					
				}
			}
		}

		GTH_SendPCEventMessage_Teleport(g_curPC, movePos);
	}
}



void GTH_ProcessEventMessage_Before_Teleport()
{
	float angle;

	g_curPC->curSkillIdx = MSG_ReadShort();

	if( g_curPC->curSkillIdx == CANCEL_TRANSFORM_SKILL_INDEX )
	{
		if( g_curPC->isTransform ) GTH_DeleteTransform( g_curPC );
	}
	else
	{
	
		MSG_ReadPosition( g_curPC->position );
		angle = MSG_ReadFloat();
		g_curPC->angles[ YAW ] = angle;

		GTH_SendPCEventMessage_Before_Teleport(g_curPC);
	}
}



void GTH_ProcessMessage_Request_Epack_ChargSoul()
{
	item_t *item;
	int itemIdx, itemTableIdx; 
	int invenPos;
	int reply = 1, error = 0;		

	invenPos = MSG_ReadByte();

	if(invenPos < 0 || invenPos >= MAX_INVENTORY_SIZE) return;
	itemIdx = g_curPC->inventory[invenPos];

	if( itemIdx < 0 )
	{
		reply = 0;
		error = ERROR_CONSUMPTIVE_0;
		GTH_SendMessage_ReplayEpack_ChargSoul(g_curPC, reply, error, invenPos);
		return;
	}
	else
	{
		item = &g_curPC->item[itemIdx];
		itemTableIdx = item->itemTableIdx;
	
		int epackIdx = g_curPC->equipment[EQUIP_EPACK];
		if(epackIdx < 0 || epackIdx >= MAX_NUMBER_OF_OWNITEM)
		{
			
			reply = 0;
			error = ERROR_CONSUMPTIVE_EQUIP;
			GTH_SendMessage_ReplayEpack_ChargSoul(g_curPC, reply, error, invenPos);
		}

		
		int epkItemTableIdx = g_curPC->item[epackIdx].itemTableIdx;
		if( g_itemTable[epkItemTableIdx].epkChargeMethod == TypeItemTable_t::EPK_CHARGE_TYPE_NPC)
		{
			
			reply = 0;
			error = ERROR_CONSUMPTIVE_CHARGE_ONLY_NPC;
			GTH_SendMessage_ReplayEpack_ChargSoul(g_curPC, reply, error, invenPos);
			return;
		}

		
		int EpkRemainCount=-1;
		if(g_itemTable[epkItemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_CHARGE)
		{
			
			EpkRemainCount = gcpTools->GetEpkChargeCount(&g_curPC->item[epackIdx]);
			if( EpkRemainCount <= 0 )
			{
				if((int)g_curPC->curChargeRA <=0 && (int)g_curPC->curChargeSA <=0 )
				{
					
					GTH_DeleteMyItem( g_curPC, &g_curPC->item[epackIdx] );
					g_curPC->equipment[EQUIP_EPACK] = -1;
					g_DBGameServer->RemoveItem( g_curPC, epackIdx );
					
					
					reply = 0;
					error = ERROR_CONSUMPTIVE_DELETE_CHARGE;
					GTH_SendMessage_ReplayEpack_ChargSoul(g_curPC, reply, error, invenPos);
				}
				else
				{
					
					reply = 0;
					error = ERROR_CONSUMPTIVE_CHARGE_COUNT;
					GTH_SendMessage_ReplayEpack_ChargSoul(g_curPC, reply, error, invenPos);
				}
				return;
			}
		}

		
		
		if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::USE_ITEM_FOR_EPK_MUCH_CHARGE))
		{
			float fRate = gItemTableData.GetData(itemTableIdx,CItemTableData::EPK_RATIO);
		
			g_curPC->curChargeRA += (g_curPC->maxChargeRA * fRate);
			g_curPC->curChargeSA += (g_curPC->maxChargeSA * fRate);

			if(g_curPC->curChargeRA > g_curPC->maxChargeRA)		
				g_curPC->curChargeRA = g_curPC->maxChargeRA;

			if(g_curPC->curChargeSA > g_curPC->maxChargeSA)		
				g_curPC->curChargeSA = g_curPC->maxChargeSA;

		}
		else
		{
			g_curPC->curChargeRA = g_curPC->maxChargeRA;
			g_curPC->curChargeSA = g_curPC->maxChargeSA;
		}

		if( item->durability > 0 )
		{
			item->durability --;
			g_DBGameServer->SaveItem( g_curPC, item, itemIdx );															
		}
		else
		{
			GTH_DeleteMyItem( g_curPC, item );
			g_curPC->inventory[invenPos] = -1;			
			if ( !g_DBGameServer->RemoveItem( g_curPC, itemIdx ) )
			{				
				reply = 0;							
				return;				
			}
		}

		
		
		if(g_itemTable[epkItemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_MUCH_CAPACITY && g_itemTable[epkItemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_CHARGE)
		{
			
			EpkRemainCount--;
			if(EpkRemainCount < 0)  EpkRemainCount=0;
			gcpTools->SetEpkChargeCount(&g_curPC->item[epackIdx],EpkRemainCount);
			g_DBGameServer->SaveItem( g_curPC, &g_curPC->item[epackIdx], epackIdx );	
		}

		GTH_SendMessage_ReplayEpack_ChargSoul(g_curPC, reply, error, invenPos,EpkRemainCount);
	}
}



item_t* GetMyItem(playerCharacter_t* pPc,const int Idx)
{
	if(Idx <0) return NULL;
	if(Idx >= MAX_NUMBER_OF_OWNITEM) return NULL;
	
	item_t* pItem= &pPc->item[Idx];
	if(pItem->itemTableIdx < 0) return NULL;
	if(pItem->itemTableIdx >= g_itemTableNumber) return NULL;	
	return pItem;
}










BOOL	GTH_MyItem_Drop_ForRegistedItem(playerCharacter_t* pPc,const int ItemArrayIdx )
{
	item_t *pItem = GetMyItem(pPc,ItemArrayIdx);
	if(NULL == pItem) return FALSE;	

	int itemObjectIdx = 
		GTH_SpawnItemObject( pItem, pPc->worldIdx, pPc->position, LOOTTYPE_INDIVIDUAL, pPc->idx );
				
	
	if ( itemObjectIdx < 0 )				
	{		
		ShowLogInfo("이곳에서 유저로그를 남겨주세요 인모씨~~~~~~~~ ");		
	}


	GTH_DeleteMyItem( pPc, pItem );
	g_DBGameServer->RemoveItem( pPc,  ItemArrayIdx);					
	
	return TRUE;
}












int GetMatchMyItemIdx_fromINVENTORY(playerCharacter_t* pPc,const int paramItemTableIdx)
{
	int inventory_idx;
	int ItemArryIdx;
	item_t* pTempItem=NULL;
	
	for(inventory_idx = 0; inventory_idx < MAX_INVENTORY_SIZE; inventory_idx++)
	{
		ItemArryIdx = pPc->inventory[inventory_idx];		
		if(ItemArryIdx < 0) continue;		
		if(ItemArryIdx >= MAX_NUMBER_OF_OWNITEM){
			pPc->inventory[inventory_idx]=-1;
			continue;
		}

		pTempItem= &pPc->item[ItemArryIdx];
		if((pTempItem->itemTableIdx < 0) ||
		   (pTempItem->itemTableIdx >= g_itemTableNumber)){

			GTH_DeleteMyItem( pPc, pTempItem );
			g_DBGameServer->RemoveItem(pPc,ItemArryIdx);
			pPc->inventory[inventory_idx]=-1;
			continue;
		}

		if(paramItemTableIdx == pTempItem->itemTableIdx) return ItemArryIdx;		
	}

	return -1;
}




void GTH_Item_AddInventoryEmptyBox_or_DropMap(
	playerCharacter_t* pPc,
	const int ItemArryIdx)	
{
	
	int inventoryIdx = GTH_FindEmptyInventoryPos( pPc );					
	
	if( inventoryIdx < 0)		
	{
		
		GTH_MyItem_Drop_ForRegistedItem(pPc, ItemArryIdx);							
		return;
	}
	
	pPc->inventory[inventoryIdx] = ItemArryIdx;	
	return;
}





BOOL	GTH_AddMyInventory_ForRegsitedItem(playerCharacter_t* pPc, int ItemArrayIdx )
{	
	if(ItemArrayIdx < 0)	return FALSE;
	if(ItemArrayIdx >= MAX_NUMBER_OF_OWNITEM) return FALSE;

	item_t *pItem = NULL;
	pItem = &pPc->item[ItemArrayIdx];
	if( pItem->itemTableIdx < 0 )	return FALSE;
	if(pItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return FALSE;

	if(FALSE == GTH_IsStackItem(pItem)){
		GTH_Item_AddInventoryEmptyBox_or_DropMap(pPc,ItemArrayIdx);			
		return TRUE;
	}


	int matchitemidx=GetMatchMyItemIdx_fromINVENTORY(pPc,pItem->itemTableIdx);
	if(matchitemidx < 0){
		GTH_Item_AddInventoryEmptyBox_or_DropMap(pPc,ItemArrayIdx);
		return TRUE;
	}

	
	item_t *pDestItem = GetMyItem(pPc,matchitemidx);
	if(NULL == pDestItem){
		GTH_Item_AddInventoryEmptyBox_or_DropMap(pPc,ItemArrayIdx);
		return TRUE;
	}

	
	
	pDestItem->durability += pItem->durability+1;			
	if( pDestItem->durability >= MAX_COUNT_STACK_ITEM ) 
		pDestItem->durability = MAX_COUNT_STACK_ITEM - 1;

	g_DBGameServer->SaveItem(pPc, pDestItem, matchitemidx );

	
	GTH_DeleteMyItem( pPc, pItem );
	g_DBGameServer->RemoveItem( pPc,ItemArrayIdx );
	return TRUE;

}











BOOL GTH_IsEnableHaveDamage(playerCharacter_t *pc)
{	
	if ( pc->m_WorldChangeSystem.m_SystemMode == 
		WorldChangeSystem::enumWorldChangeMode::WORLD_CHANGESYSTEM_EFFECT_MODE)		
		return 0;

	if ( pc->bautoeffectisvalid == true)
		return 0;


	return 1;

}








void GTH_ProcessMessage_DestroyItem(void)
{
	int reply = 1, error = -1;
	int Result = 0;
	int itemTableIdx = -1;
	int raCount = 0, saCount = 0;
	int itemIdx = -1;
	int UseItemType = -1;	
	item_t* item=NULL;
	BOOL bIsStack = FALSE;
	int stackNumber = 1;

	
	
	int invenPos = MSG_ReadShort();

	
	if ( invenPos == -1 )
		itemIdx = g_curPC->mouseInventory;
	else
		itemIdx = g_curPC->inventory[invenPos];

	if((itemIdx < 0) || 
	   (itemIdx >= MAX_NUMBER_OF_OWNITEM)){

		g_logSystem->Write("Destroy Failure!! : Item Not Exist, %s(%s)", g_curPC->name, g_curPC->userID );
		GTH_SendMessage_DestroyItem( g_curPC, 0, ERROR_DESTROYITEM_2, itemIdx );
		return;
	}

	
	
	if((invenPos == -1) && (g_curPC->equipment[EQUIP_EPACK] < 0)){
		g_logSystem->Write("Destroy Failure!! : Item Not Exist, %s(%s)", g_curPC->name, g_curPC->userID );
		GTH_SendMessage_DestroyItem( g_curPC, 0, ERROR_DESTROYITEM_1, itemIdx );
		return;
	}


	if( g_curPC->busyState != BUSY_STATE_NONE){
		g_logSystem->Write("Destroy Failure!! : %s(%s), busyState : %d", g_curPC->name, g_curPC->userID, g_curPC->busyState);		
		GTH_SendMessage_DestroyItem( g_curPC, 0, ERROR_DESTROYITEM_3, itemIdx );
		return;
	}



	itemTableIdx = g_curPC->item[itemIdx].itemTableIdx;
	item = &g_curPC->item[itemIdx];

	
	if(!g_UseItem.AutoQuestItemType(item,invenPos,raCount,saCount,bIsStack,stackNumber,reply,error,itemIdx)) return;
	
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return ;

	UseItemType = gItemTableData.GetUseItemType(&g_itemTable[itemTableIdx]);

	
	switch(UseItemType) {
		case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_RECYCLE :
			Result= g_UseItem.RecyleItem(item,raCount,saCount,reply,error,itemIdx,invenPos);
			break;

		case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_QUESTSCROLL :
			Result= g_UseItem.QuestScrollItem(item,invenPos,reply,error,itemIdx);
			break;

		case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_AUTOQUESTSCROLL :
			Result= g_UseItem.AutoQuestScrollItem(item,invenPos,reply,error,itemIdx);
			break;

		
		case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_SUMMONCARD_ITEM :
			Result= g_UseItem.MonsterCardItem(item,invenPos,reply,error,itemIdx);
			break;

		default:
			Result= g_UseItem.DefaultUsetItem(item,invenPos,reply,error,itemIdx,raCount,saCount);
			break;
	}	

	if(0 == Result) return;



	g_curPC->curChargeRA += raCount;
	if( g_curPC->curChargeRA > g_curPC->maxChargeRA ) g_curPC->curChargeRA = g_curPC->maxChargeRA;

	g_curPC->curChargeSA += saCount;
	if( g_curPC->curChargeSA > g_curPC->maxChargeSA ) g_curPC->curChargeSA = g_curPC->maxChargeSA;

	g_curPC->curItemWeight -= g_itemTable[item->itemTableIdx].weight;

	if( g_itemTable[item->itemTableIdx].stackFlag ){
		bIsStack = TRUE;
		stackNumber = item->durability + 1;
	}

	
	
	
	g_logSystem->WriteToLog( 
		g_curPC, 
		LOG_CLASS_ITEM, 
		"%d;%d;;;;%s;%s", 
		LOG_CLASS_ITEM_DESTROY, 
		stackNumber, 
		item->name, 
		GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, -1 ) );
	

	g_logSystem->Write( "Item Destroy : %s(%s), item : %s Destroyed", g_curPC->name, g_curPC->userID, item->name );

	
	GTH_DeleteMyItem( g_curPC, item );
	g_curPC->mouseInventory = -1;
	g_DBGameServer->RemoveItem(g_curPC, itemIdx);
	

	GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
}







void GTH_PC_GetLoot_ForDanBattle(
	int idx, 
	__int64 exp, 
	__int64 genExp, 
	int lootNSE, 
	int genCapability, 
	int sharingType,
	BOOL bGoryunBattle,
	BOOL bUseItem)
{

	if ( idx < 0 || idx >= MAX_PCS)
		return;

	int upFlag = false;
	int level = 0, genLevel = 0, selectedGen;
	int skillIdx[50],skillNumber = 0;		

	
	selectedGen = g_pc[idx].selectedGen;
	
	if( exp < 0 )			exp = 0;
	if( genExp < 0 )		genExp = 0;
	if( lootNSE < 0 )		lootNSE = 0;
	if( genCapability < 0 ) genCapability = 0;

	
	if( exp == 0 && genExp == 0 && lootNSE == 0 && genCapability == 0 ) return;
	
	
	GTH_PC_ExpUp( idx, exp, genExp, lootNSE, level, genLevel );

	if( genCapability > 0 && g_pc[idx].genCapability[selectedGen] >= 0 ) g_pc[idx].genCapability[selectedGen] += genCapability;

	g_pc[idx].curChargeSE += lootNSE;
	
	if( g_pc[idx].curChargeSE > MAX_OWN_NAK ) g_pc[idx].curChargeSE = MAX_OWN_NAK;

	if( level > g_pc[idx].level )
	{
		g_pc[idx].bonusStatusPoint += ( level - g_pc[idx].level );
		GTH_PC_LevelUp( &g_pc[idx], level );
		upFlag = true;
	}
	if( genLevel > g_pc[idx].genLevel )
	{
		GTH_PC_GenLevelUp( &g_pc[idx], genLevel );
		upFlag = true;
	}
	
	if( upFlag )
	{
		
		GTH_PC_Calculate( &g_pc[idx], PCINFO_BASIC );
		GTH_SendPCMessage_LevelUp( &g_pc[idx], g_pc[idx].exp, g_pc[idx].genExp, g_pc[idx].curChargeSE, g_pc[idx].genCapability[selectedGen], skillIdx, skillNumber, sharingType );

	}
	else
	{
		GTH_SendMessage_LootValue( &g_pc[idx], g_pc[idx].exp, g_pc[idx].genExp, g_pc[idx].curChargeSE, g_pc[idx].genCapability[selectedGen], sharingType );
	}
	
	
	
	if( ( 0 < exp) &&  ( 0 >= lootNSE ) )
	GTH_ProcessMessage_AutoQuestScroll_ObtainOpen(&g_pc[idx]);	
	
}


void GTH_ProcessComplusionWorldChange(playerCharacter_t *pPc)
{
	if ( NULL == pPc ) 	return;
	if ( BUSY_STATE_WORLDCHAGNE != pPc->busyState )	return;

	float currentTime = 0.0f;
	currentTime = g_timer->GetAppMilliTime();
	
	if ( currentTime > ( pPc->fWorldChangeStartTime + WorldChangeSystem::enumWOrldChangeTime::WORLD_CHANGE_TOTAL_TIME) )
		GTH_ProcessComplusionTransportWorld(pPc);	
}




void GTH_ProcessComplusionTransportWorld(playerCharacter_t *pPc)
{
	int worldIdx, gateIdx;
	int  memberIdx;

	vec3_t spawnPosition, savePosition;
	float spawnYaw, saveYaw;
	int saveWorldIdx;
	

	worldIdx = pPc->iChangeWorldIdx;
	gateIdx = 0;		

	if (worldIdx >= g_worldTableNumber)
	{
		GTH_SendPCMessage_TransportReply( g_curPC,tagGCPacket_TRANSPORTWORLDREP::enumCode::fail);	
		g_logSystem->Write("User (%s) Move to Not Exist World(%d).", g_curPC->userID, worldIdx);
		return; 
	}

	if ( pPc->worldIdx == worldIdx )
	{
		GTH_SendPCMessage_TransportReply( pPc,tagGCPacket_TRANSPORTWORLDREP::enumCode::fail );	
		g_logSystem->Write("User (%s) Move To Same World.", pPc->userID);
		return; 
	}


	if((TRUE == g_config.bPremium) &&
	   (FALSE == gcpPremiumPlayerCtrl->isAccessMap(worldIdx,pPc))){
		GTH_SendPCMessage_TransportReply( pPc,tagGCPacket_TRANSPORTWORLDREP::enumCode::access_deny_because_free_user);
		return;
	}

	

	
	if ( pPc->busyState != BUSY_STATE_NONE) 
	{
		
		if (pPc->busyState != BUSY_STATE_WORLDCHAGNE)
		{		
			GTH_SendPCMessage_TransportReply( pPc,tagGCPacket_TRANSPORTWORLDREP::enumCode::busy_state );	
			g_logSystem->Write("User (%s) Don't Move. User Busy State.", pPc->userID);
			return;
		}
	
	}
	

	
	if ( GTH_IsLocalServiceWorld(worldIdx) )
	{
		
		GTH_Zone_DeleteEntityList(pPc->worldIdx, pPc->zoneIdx, pPc->entityType, pPc->idx);

		
		if(pPc->worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index)	
		{
			pPc->worldIdx = worldIdx;
			vec_copy( g_binding[0].pos, pPc->position);
		}
		else
		{
			
			pPc->worldIdx = worldIdx;
			vec_copy( g_worldTable[worldIdx].spawnPos[gateIdx], pPc->position);
			pPc->angles[1] = g_worldTable[worldIdx].spawnYaw[gateIdx];
		}
		

		
		pPc->reliableWorldIdx = pPc->worldIdx;
		vec_copy( pPc->position, pPc->reliablePosition );


		pPc->zoneIdx = GTH_Zone_GetZoneIdx(pPc->position);

		
		GTH_Zone_AddEntityList(pPc->worldIdx, pPc->zoneIdx, pPc->entityType, pPc->idx);

		pPc->ready = false;
		pPc->event = GTH_EV_CHAR_IDLE;

		
		GTH_SendPCMessage_TransportReply( pPc,tagGCPacket_TRANSPORTWORLDREP::enumCode::success);
		g_logSystem->Write("User (%s) Move To World %d", pPc->userID, pPc->worldIdx);

		
		pPc->m_WorldChangeSystem.m_SystemMode = 
			WorldChangeSystem::enumWorldChangeMode::WORLD_CHANGESYSTEM_NONE;
		
				
		
	}
	else	
	{
		memberIdx = GTH_FindServerWithinGroup( worldIdx );
		if (memberIdx < 0) 
		{
			GTH_SendPCMessage_TransportReply( pPc,tagGCPacket_TRANSPORTWORLDREP::enumCode::fail);	
			return;
		}
		else
		{
			
			
			
			
			
			
			

			gateIdx = GTH_World_GetGateIdx(worldIdx, pPc->worldIdx);
			if (gateIdx >= 0)
			{
				vec_copy( g_worldTable[worldIdx].spawnPos[gateIdx], spawnPosition );
				spawnYaw = g_worldTable[worldIdx].spawnYaw[gateIdx];
			}
			else
			{
				vec_copy( g_worldTable[worldIdx].startPosition, spawnPosition );
				spawnYaw = g_worldTable[worldIdx].startYaw;
			}

			
			saveWorldIdx = pPc->worldIdx;
			vec_copy(pPc->position, savePosition);
			saveYaw = pPc->angles[1];

			
			pPc->worldIdx = worldIdx;
			vec_copy(spawnPosition, pPc->position);
			pPc->angles[1] = spawnYaw;

			
			pPc->flagChangeServer = 1;
			pPc->changeServerWorldIdx = worldIdx;
			DB_SaveGameData( pPc, SAVEGAMEDATA_QUIT);

			
			pPc->worldIdx = saveWorldIdx;
			vec_copy(savePosition, pPc->position);
			pPc->angles[1] = saveYaw;

			
			pPc->m_WorldChangeSystem.m_SystemMode = 
				WorldChangeSystem::enumWorldChangeMode::WORLD_CHANGESYSTEM_NONE;
			

		}
	}
}

