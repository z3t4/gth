



#include "../global.h"
#include "../GonryunPracticeBattle/CGonryunPracticeBattle.h"
#include "../Tools/CTools.h"
extern CTools* gcpTools;

extern CGonryunPracticeBattle	g_GonryunPracticeBattle;
		


#include "../LogEvent.h"
extern CLogEvent gLogEvent;

#include "../GolryunBattle/CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;




CPcSkill g_PcSkill;

CPcSkill::CPcSkill()
{

}

CPcSkill::~CPcSkill()
{

}

void CPcSkill::GTH_PC_Skill(playerCharacter_t *pc, int skillIdx, int skillTargetType)
{
	int castingTime=0;


	
	pc->resultValid = 1;
	
	pc->bNotOwnSkill = false;

	
	
	if( skillIdx < 0 ) return;
	if( pc->skill[skillIdx].tableIdx < 0 ) return;

	
	
	

	
	if ( g_curPC->skillDamage == 0)
	{
		if( skillTargetType == SKILL_TARGET_OTHER_PC || skillTargetType == SKILL_TARGET_MONSTER || 
			skillTargetType == SKILL_TARGET_CORPSE )
		{
			
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(pc->skill[skillIdx].tableIdx);
			if(pSkillTable==NULL)
			{
				TRACE("Error! pSkillTable==NULL   \n");
				return;
			}

			if( pSkillTable->GetSkillRequire()->m_nLength > 0 )
			{
				float dist = GTH_GetDistanceFromTarget( pc );
				if( dist > pSkillTable->GetSkillRequire()->m_nLength ) return;
			}







		}
	}

	if( pc->skillDamage == 0 )
	{
		
		
		CSkillCastValue *pCast = g_SkillDataGlobal.GetCastValue(pc,NULL,pc->skill[skillIdx].tableIdx,SKILL_CAST_TABLE_INDEX,0,skillIdx);

		if(pCast)
		{
			
			
			castingTime = (int)( ( pCast->m_fCasting_Time * 1000.0f ) * (1 + pc->optValue[OV_DEC_CASTINGTIME_RATE] * 0.01f));
			if(castingTime < 0) castingTime =0; 

			
			

			pc->curSA -= pCast->m_nSa_Comsumption;
			pc->curSA += pCast->m_nSa_Comsumption * pc->optValue[OV_EXHAUST_SA_RATE] * 0.01f;
		}
		else
		{
			TRACE("Error!! GTH_PC_Skill pCast==NULL ... skillIdx %d ... pc->skill[skillIdx].tableIdx %d \n",skillIdx,pc->skill[skillIdx].tableIdx);
			castingTime=0;
		}

		
		
		
		if( (int)pc->curSA < 0 ) 
		{
			pc->curSA = 0.0f;
			return;
		}

		
	}

	
	
	if( pc->targetType == ENTITY_PC && skillTargetType == SKILL_TARGET_MONSTER )
	{
		if(SAFE_PCS(pc->targetIdx))
			AddTargetList( pc, g_pc[pc->targetIdx].name );
	}

	
	if( castingTime > 0 )
	{
		pc->skillCastingtime = (float)castingTime;
		pc->skillCastingState = false;

		
		pc->generalSpeed = 100 - ( (float)pc->generalSpeed * ( (pc->optValue[OV_DEC_CASTINGTIME_RATE]) * 0.01f) ) ;		
		
		
		GTH_SendPCEventMessage_SkillCasting( pc, castingTime,skillIdx, skillTargetType ,CSkillItem::SELECT_SKILL_HOTKEY);
		pc->event = GTH_EV_CHAR_SKILLCASTING;
		pc->saveSkillIdx = skillIdx;
	}
	else
	{
		pc->event = GTH_EV_CHAR_SKILL;
		pc->saveSkillIdx = skillIdx;
		return;
	}
}




void CPcSkill::GTH_PC_SkillNotOwn(playerCharacter_t *pc,int UseType, int InvenPos, int SkillTableIndex, int level, int skillTargetType)
{
	TRACE(" GTH_PC_SkillNotOwn \n");
	
	int castingTime=0;
	
	
	pc->resultValid = 1;
	
	pc->bNotOwnSkill = true;
	pc->saveSkillLevel = level;

	
	if ( g_curPC->skillDamage == 0)
	{
		
		CSkillCastValue * pCast = g_SkillLogic.SKILL_GetValueNotOwnSkill( SkillTableIndex, level);
		if(pCast)
		{
			castingTime = (int)( ( pCast->m_fCasting_Time * 1000.0f ) * (1 + pc->optValue[OV_DEC_CASTINGTIME_RATE] * 0.01f));
			
			if(castingTime < 0)		castingTime = 0;
			
			pc->curSA -= pCast->m_nSa_Comsumption;
			pc->curSA += pCast->m_nSa_Comsumption * pc->optValue[OV_EXHAUST_SA_RATE] * 0.01f;
		}
		
		if( (int)pc->curSA < 0 ) pc->curSA = 0.0f;
	}
	
	
	if( castingTime > 0 )
	{
		pc->skillCastingtime = (float)castingTime;
		pc->skillCastingState = false;
		
		
		GTH_SendPCEventMessage_SkillCasting( pc, castingTime ,InvenPos, skillTargetType,UseType);
		
		pc->event = GTH_EV_CHAR_SKILLCASTING;
		pc->saveSkillIdx = SkillTableIndex;
		
		TRACE("skill casting 전송 \n");
	}
	else
	{
		pc->event = GTH_EV_CHAR_SKILL;
		pc->saveSkillIdx = SkillTableIndex;		
		
		
		pc->skillDamage = 1;
		
		TRACE("skill 전송 \n");
		
		if(CSkillItem::SELECT_ITEM == UseType)		g_SkillLogic.GTH_UseSkillItem_DeleteSend(InvenPos);
		return;
	}	
}


void CPcSkill::GTH_PC_SkillCooltime( playerCharacter_t *pc )
{
	int SkillNumber = 0;
	for (int i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
	{
		if ( pc->skill[i].tableIdx < 0  ) 
			continue;
		
		if ( pc->skillCooltime[i] > 0.0f)
			pc->skillCooltime[i] -= g_timer->GetElapsedMilliTime();
		
		if ( pc->skillCooltime[i] < 0.0f)
			pc->skillCooltime[i] = 0.0f;

		SkillNumber++;
		
		if ( SkillNumber == pc->skillNumber)
			break;	
	}
}




void CPcSkill::GTH_PC_SkillCasting( playerCharacter_t *pc )
{
	pc->skillCastingtime -= g_timer->GetElapsedMilliTime();
	if( pc->skillCastingtime < 0.0f )
	{
		pc->skillCastingtime = 0.0f;
		pc->skillCastingState = true;

		if( pc->saveSkillIdx >= 0 || pc->skillDamage ) 
		{
			GTH_PC_SkillExecute( pc );		
		}
		pc->saveSkillIdx = -1;
	}
}




void CPcSkill::GTH_PC_SkillExecute( playerCharacter_t *pc )
{
	int coolTime=0;
	
	if( pc->skillDamage == 0 && pc->saveSkillIdx < 0 ) return;
	
	
	
	
	
	
	
	pc->recoveryTime = pc->calAttackDelay;
	
	if( pc->bNotOwnSkill )
	{
		
		
		if(!pc->alive)
		{
			if(!g_SkillItem.IsType_UseSkillID(pc->saveSkillIdx,CSkillItem::ITEM_TYPE_ENABLE_DEAD_SELF_ITEM)) return;
		}
		
		
		g_SkillLogic.SKILL_ApplyActiveNotOwnSkill( pc, pc->saveSkillIdx, pc->saveSkillLevel );


		
		if( 109 == pc->saveSkillIdx  ||
			110 == pc->saveSkillIdx  ||
			111 == pc->saveSkillIdx  ||
			112 == pc->saveSkillIdx  )
		{
			GTH_SendPCEventMessage_Skill( pc, pc->saveSkillIdx, coolTime, pc->recoveryTime );
		}
	}
	else
	{	
		if ( pc->saveSkillIdx  >= 0)
		{
			if( pc->skill[pc->saveSkillIdx].tableIdx < 0 ) return;
			g_SkillLogic.SKILL_ApplyActive( pc, pc->saveSkillIdx, coolTime );
		}

		if( pc->skillDamage == 0 )
		{
			GTH_SendPCEventMessage_Skill( pc, pc->saveSkillIdx, coolTime, pc->recoveryTime );
			pc->skillCooltime[pc->saveSkillIdx]	= (float)( coolTime + (int)( g_timer->GetAbsoluteTime() * 1000.0f ) );
		}
	}
	pc->skillDamage = 0;
}






void CPcSkill::GTH_ProcessMessage_Request_SkillLevelUp()
{
	int  skillIdx, genType;
	char szTemp[256];
	
	skillIdx = MSG_ReadByte();
	genType = MSG_ReadByte();
	
	if( skillIdx < 0 || skillIdx >= MAX_NUMBER_OF_SKILL )
	{
		sprintf( szTemp, "Name : %s, skillIdx : %d, Level up 요청", g_curPC->name, skillIdx );
		g_logSystem->Write( szTemp );
		return;
	}

	
	g_SkillLogic.SKILL_LevelUp( g_curPC, skillIdx, genType );


}

void CPcSkill::GTH_ProcessMessage_Request_Learn_Skill()
{
	item_t *item;
	int itemIdx, itemTableIdx, skillTableIdx, idx, classIdx;
	int invenPos;
	int reply = 1, error = 0;
	invenPos = MSG_ReadByte();
	itemIdx = g_curPC->inventory[invenPos];

	if( itemIdx < 0 )
	{
		reply = 0;
		error = ERROR_SKILL_0;
		goto REPLY_LEARN_SKILL;
		return;
	}

	item = &g_curPC->item[itemIdx];
	itemTableIdx = item->itemTableIdx;
	classIdx = g_itemTable[itemTableIdx].classIdx;

	
	if( !g_itemClassTable[classIdx].etcAtt || g_itemTable[itemTableIdx].etcItemType != ETCITEM_TYPE_SKILLSPHERE )
	{
		reply = 0;
		error = ERROR_SKILL_0;
		goto REPLY_LEARN_SKILL;
	}

	skillTableIdx = g_itemTable[itemTableIdx].skillSphere.skillTableIndex;
	
	if( skillTableIdx < 0 || skillTableIdx >= MAX_SKILL_NUMBER )
	{
		reply = 0;
		error = ERROR_SKILL_4;
		goto REPLY_LEARN_SKILL;
	}

	
	if( !GTH_CheckRequire( g_curPC, 1, &skillTableIdx ) )
	{
		reply = 0;
		error = ERROR_SKILL_1;
		goto REPLY_LEARN_SKILL;
	}

	
	
	idx = g_SkillLogic.SKILL_Add( g_curPC, skillTableIdx );
	if( idx < 0 )
	{
		reply = 0;
		error = ERROR_SKILL_2;
		goto REPLY_LEARN_SKILL;
	}
	
	
	g_logSystem->Write("Skill Learn : (%s) %s", g_curPC->name, item->name);

	
	
	
	g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;;;;;%s;%s", 
		LOG_CLASS_ITEM_SKILL_SCROLL, item->name, 
		GetItemInformationLog( 0, item->itemTableIdx, itemIdx, invenPos ) );
	

	GTH_DeleteMyItem( g_curPC, item );
	g_curPC->inventory[invenPos] = -1;
	
	reply = 1;
	g_DBGameServer->RemoveItem( g_curPC, itemIdx );
	
REPLY_LEARN_SKILL :

	GTH_SendMessage_Reply_Learn_Skill( g_curPC, reply, error, invenPos, idx );

	GTH_PC_Calculate( g_curPC );
}


void CPcSkill::GTH_Recv_UseItemSkill()
{
	int skillIdx=-1, skillTargetType,InvenPos=-1;
	int targetIdx, targetType;
	float angle;
	int SkillDamage;
	
	InvenPos		= MSG_ReadShort();
	skillTargetType = MSG_ReadByte();
	MSG_ReadPosition( g_curPC->saveSkillTargetPos );
	targetIdx		= MSG_ReadShort();
	targetType		= MSG_ReadByte();
	MSG_ReadPosition( g_curPC->position );
	angle			= MSG_ReadFloat();
	SkillDamage		= MSG_ReadByte();
	
	
	
	
	if(InvenPos < 0 || InvenPos >= MAX_INVENTORY_SIZE)
	{
		
		GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_NOT_HAVE_ITEM, InvenPos );
		return;
	}

	
	
	int itemIdx = g_curPC->inventory[InvenPos];
	if( itemIdx < 0 )
	{
		
		GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_0, InvenPos );
		return;
	}		
	
	item_t *item = &g_curPC->item[itemIdx];
	int itemTableIdx = item->itemTableIdx;
	if(!g_SkillItem.IfSkillItem(itemTableIdx)) 
	{
		
		GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_NOT_SKILL_ITEM, InvenPos );
		return;
	}
	
	int SkillTableIndex = g_SkillItem.GetSkillTableIndex(itemTableIdx);
	if(SkillTableIndex < 0 ) 
	{
		
		GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_NOT_SKILL_ITEM, InvenPos );
		return;
	}
	
	int level = g_SkillItem.GetLevel(itemTableIdx);
	if(level < 0) 
	{
		
		GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_NOT_LEVEL, InvenPos );
		return;
	}
	
	
	playerCharacter_t* pTargetPlayer = gcpTools->GetPlayerRecordPointer(targetIdx);

	
	if(g_SkillDataGlobal.IsDataFormatType(SkillTableIndex,SKILL_UNIQUE_TYPE_ONLY_USE_PC))
	{						
		if(ENTITY_PC != targetType)
		{
			GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_ONLY_USE_PC, InvenPos );
			return;
		}
	}

	
	if(g_SkillDataGlobal.IsDataFormatType(SkillTableIndex,SKILL_UNIQUE_TYPE_NOT_USE_NULL_TARGET_PLAYER))
	{	
		if(NULL == pTargetPlayer)
		{
			GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_NULL_TARGET_PLAYER, InvenPos );
			return;
		}
	}

	
	if(NULL != pTargetPlayer && g_SkillDataGlobal.IsDataFormatType(SkillTableIndex,SKILL_UNIQUE_TYPE_NOT_USE_ALIVE_PLAYER))
	{			
		if(pTargetPlayer->alive)
		{
			GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_ALIVE_TARGET_PLAYER, InvenPos );
			return;
		}
	}

	
	if(NULL != pTargetPlayer && g_SkillDataGlobal.IsDataFormatType(SkillTableIndex,SKILL_UNIQUE_TYPE_ONLY_USE_SELF))
	{						
		if(pTargetPlayer->idx != g_curPC->idx)
		{
			GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_ONLY_USE_SELF, InvenPos );
			return;
		}
	}

	
	if(NULL != pTargetPlayer && g_SkillDataGlobal.IsDataFormatType(SkillTableIndex,SKILL_UNIQUE_TYPE_ONLY_USE_OTHER))
	{						
		if(pTargetPlayer->idx == g_curPC->idx)
		{
			GTH_SendMessage_Reply_Use_Potion( g_curPC, ERROR_CONSUMPTIVE_ONLY_USE_OTHER, InvenPos );
			return;
		}
	}


	g_curPC->targetIdx		= targetIdx;
	g_curPC->targetType		= targetType;
	
	g_curPC->skillDamage	= SkillDamage;
	g_curPC->angles[ YAW ]	= angle;
	
	g_PcSkill.GTH_PC_SkillNotOwn( g_curPC,CSkillItem::SELECT_ITEM,InvenPos, SkillTableIndex, level, 0 ); 
}





inline bool CPcSkill::CheckEnableUseSkill_Player(const int targetType,const int targetIdx,const int skillTargetType)
{
	






	playerCharacter_t* pTargetPlayer = gcpTools->GetPlayerRecordPointer(targetIdx);
	if(NULL == pTargetPlayer){
		gLogEvent.LogEvent(CLogEvent::GTH_EVENTLOG_ERROR_TYPE,"CPcSkill::CheckEnableUseSkill_Player  \n\n pTargetPlayer가 NULL [%d] \n",targetIdx);
		return true;
	}
	
	
	if(pTargetPlayer->idx == g_curPC->idx) return true;

	
	if ( SKILL_TARGET_MONSTER == skillTargetType)
	{
		if( !( g_curPC->pvpMode && pTargetPlayer->pvpMode ) )
		{









			
			
			if ( (g_curPC->DanBattleInfo.nDanBattleIdx >= 0 )&& 
				( g_curPC->DanBattleInfo.nBattleMapIdx == g_curPC->worldIdx)){
				if (FALSE == g_NewDanBattleSystem.IsDanBattleEnemy(g_curPC->idx, pTargetPlayer->idx)){
					
					return false;					
				}
			}
			
			else{		
					if ( IsPKDisableWorld( g_curPC->worldIdx ) ) {
						GTH_SendMessage_SkillError(g_curPC,SKILL_ERROR_PK_DISABLE);
						return false;				
				}
				
				playerCharacter_t* pTargetPC = gcpTools->GetPlayerRecordPointer(g_curPC->targetIdx);
				if ( pTargetPC != NULL)
					if ( pTargetPC->DanBattleInfo.nDanBattleIdx >= 0) {
						
						return false;					
					}
			}

			
		}
	}


	
	
	CGonryunPractice_ProcessCtrl* pProcess = g_GonryunPracticeBattle.GetProcessCtrl();
	if(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE == g_curPC->GonryunBattlePractice.MemberShip)
	{
		if( (playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER	== pTargetPlayer->GonryunBattlePractice.MemberShip) ||
			(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT == pTargetPlayer->GonryunBattlePractice.MemberShip))
		{			
			
			GTH_SendMessage_SkillError(g_curPC,SKILL_ERROR_BATTLE_TEST);
			return false;
		}
	}

	if(tagGolryunBattle::Golryun_Battle_Map_Index == pTargetPlayer->worldIdx)
	{
		if(FALSE == (gcpGolryunBattle->GetProcessClassPointer())->isJoinPlayer(g_curPC))
		{				
			
			if(TRUE == (gcpGolryunBattle->GetProcessClassPointer())->isJoinPlayer(pTargetPlayer))
			{
				
				GTH_SendMessage_SkillError(g_curPC,SKILL_ERROR_BATTLE);
				return false;
			}				
		}
	}	

	return true;
}

inline bool CPcSkill::CheckEnableUseSkill_Monster(const int targetType,const int targetIdx,const int skillTargetType)
{
	return true;
}

bool CPcSkill::CheckEnableUseSkill(int targetType,int targetIdx,int skillTargetType)
{
	
	
	
	
	
	

	switch(targetType) {
	case ENTITY_PC:
		return CheckEnableUseSkill_Player(targetType,targetIdx,skillTargetType);
		break;

	case ENTITY_NPC:
	case ENTITY_MONSTER:
		return CheckEnableUseSkill_Monster(targetType,targetIdx,skillTargetType);
		break;

	default:
		
		
		
		
		break;
	}	
		
	return true;
}



void CPcSkill::GTH_Recv_UseSkill()
{
	int skillIdx=-1, skillTargetType;
	int targetIdx, targetType;
	float angle;
	int SkillDamage;
	skillIdx		= MSG_ReadShort();
	skillTargetType = MSG_ReadByte();
	MSG_ReadPosition( g_curPC->saveSkillTargetPos );
	targetIdx		= MSG_ReadShort();
	targetType		= MSG_ReadByte();
	MSG_ReadPosition( g_curPC->position );
	angle			= MSG_ReadFloat();
	//lucky 2012 no range hack
	int AttckLen = MSG_ReadShort();
	if (g_curPC->calAttackLength != AttckLen) 
	{
		g_logSystem->WriteToHackingLog("[HACKING_SKILL] UserID:[%s]  char:[%s] 톂ryed to No range attak with skills.",g_curPC->userID,g_curPC->name);
		return;
	}
	//end
	SkillDamage		= MSG_ReadByte();	

	if ( SkillDamage == 1){
		if ( skillIdx >= 0)
			return;			
	}
	
	
	if( 0 > skillIdx || skillIdx  >=  MAX_NUMBER_OF_SKILL)
	{
		if (skillIdx != CANCEL_TRANSFORM_SKILL_INDEX)
		{
			skillIdx = g_curPC->curSkillIdx;
		}
	}
	else
	{   
		g_curPC->curSkillIdx = skillIdx;
	}
	
	
	if(targetIdx <0)
	{

		gLogEvent.LogEvent(CLogEvent::GTH_EVENTLOG_ERROR_TYPE,
			"\n\n CPcSkill::GTH_Recv_UseSkill \n targtIdx Error %d \n info == SkilllIdx %d, skillTargetType %d, targetType %d \n",
			targetIdx, skillIdx,skillTargetType,targetType);

	}


	
	
	if( skillIdx == CANCEL_TRANSFORM_SKILL_INDEX )
	{
		if(gcpTools->IsTransformEntity(g_curPC,ENTITY_PC))
		{
			GTH_DeleteTransform( g_curPC );
		}

		
		else
		{
			gLogEvent.LogEvent(CLogEvent::GTH_EVENTLOG_ERROR_TYPE,"\n\n 변신 아닌데.. 변신 해제 요청  \n\n g_curPC->idx %d  ",g_curPC->idx);
		}
		return;
	}
	
	
	
	if( g_curPC->skill[skillIdx].tableIdx != 44 )
	{
		for(int i = 0; i < MAX_AFFECT; i++ )
		{
			if( g_curPC->affect[i].skillTableIdx == 44)
			{
				GTH_DeleteToggle( g_curPC, ENTITY_PC, g_curPC->affect[i].skillTableIdx );
				break;
			}
		}
	}
	
	
	if(FALSE == CheckEnableUseSkill(targetType,targetIdx,skillTargetType)) return;
	
	if ( targetType == ENTITY_NPC )	return;
	if( SkillDamage == false)
	{
		float fTime =g_timer->GetAbsoluteTime();
		if ( g_curPC->skillCooltime[skillIdx] - g_timer->GetAbsoluteTime() * 1000.0f > 0.0f )
		{	
			GTH_SendMessage_SkillError(g_curPC,SKILL_ERROR_COOLTIME);			
			return;
		}			
	}
	
	g_curPC->targetIdx		= targetIdx;
	g_curPC->targetType		= targetType;
	g_curPC->angles[ YAW ]	= angle;
	g_curPC->skillDamage	= SkillDamage;
	
	g_PcSkill.GTH_PC_Skill(g_curPC,skillIdx,skillTargetType);
	return;
}


void CPcSkill::GTH_ProcessEventMessage_Skill(void)
{
	int UseType		= MSG_ReadByte();
	
	if(CSkillItem::SELECT_ITEM == UseType)
	{
		GTH_Recv_UseItemSkill();
	}
	else
	{
		GTH_Recv_UseSkill();
	}
	
	return;
}

bool	CPcSkill::FindHideSkill(int index)
{
	for(int i = 0; i < MAX_AFFECT; i++ )
	{
		if( g_pc[index].affect[i].skillTableIdx == 44 )
			return true;
	}

	return false;
}

