#include "../global.h"
#include "SkillApp.h"

skillObtain_t		g_autoObtainSkill;
skillReturnData_t	g_skillMultiTarget;
float				g_tempCoolTime;
int					g_affectSkillLevel;


CSkillLogic			g_SkillLogic;


#define MAX_COUNT_SKILL_FUNC		136







void CSkillLogic::SKILL_Empty( void* pPc, float* pValue )
{
}


void CSkillLogic::SKILL_MakeAutoObtain()
{
	int i;
	AUTO_OBTAIN_SKILL *pObSkill;

	memset( &g_autoObtainSkill, 0, sizeof( skillObtain_t ) );

	
	for( i = 0; i < MAX_SKILL_NUMBER; i ++ )
	{
		
		CSkilTable *pSkillTable = g_SkillTableDB.FindDB(i);
		if(pSkillTable==NULL)
		{
			TRACE("Error! pSkillTable==NULL   \n");
			continue;
		}

		if( pSkillTable->m_nId < 0 ) continue;
		if( !pSkillTable->m_nAutoObtainFlag) continue;

		pObSkill = &g_autoObtainSkill.autoObtainSkill[pSkillTable->m_nJobType];

		pObSkill->status[ pObSkill->count ].tableIdx = pSkillTable->m_nId;
		pObSkill->status[ pObSkill->count ].level = pSkillTable->GetSkillRequire()->m_nGenLevel;
		pObSkill->count ++;
	}


	
}

int CSkillLogic::SKILL_IsValid( playerCharacter_t* pc, int skillIdx )
{
	int i, count = 0;
	if( pc->skillNumber <= 0 ) return( false );
	for( i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
	{
		if( pc->skill[i].tableIdx < 0 ) continue;
		if( pc->skill[i].tableIdx == skillIdx ) return( true );
		count ++;
		if( pc->skillNumber == count ) return( false );
	}
	return( false );
}

void CSkillLogic::SKILL_GetAutoObtain( playerCharacter_t *pc, int newLevel, int skillIdx[], int &num )
{

}


void CSkillLogic::SKILL_GetInitSkillLevel( int skillTableIdx, SKILL_GEN_LEVEL *level )
{

	int gen[4] = { 0, 0, 0, 0 };
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL)
	{
		TRACE("Error! pSkillTable==NULL   \n");
		return;
	}

	switch( pSkillTable->m_nJobType)

	{
	case J_MENTALITY :	
		gen[0] = 1;
		break;
	case J_WIND :		
		gen[1] = 1;	
		break;
	case J_WATER :		
		gen[2] = 1;
		break;
	case J_EARTH :		
		gen[3] = 1;
		break;
	case J_DEVOTIONAL_KNIGHT :	
		gen[0] = 1;
		gen[3] = 1;
		break;
	case J_ROMING_KNIGHT :		
		gen[0] = 1;
		gen[1] = 1;
		break;
	case J_PIECING_EYES :		
		gen[1] = 1;
		gen[2] = 1;
		break;
	case J_WINDY_ROMER :		
		gen[1] = 1;
		gen[0] = 1;
		break;
	case J_DESTROYER :			
		gen[2] = 1;
		gen[0] = 1;
		break;
	case J_GUARDIAN :			
		gen[2] = 1;
		gen[3] = 1;
		break;
	case J_PROPHECIER :			
		gen[2] = 1;
		gen[1] = 1;
		break;

	case J_TRANSFORMER :		
		gen[3] = 1;
		gen[1] = 1;
		break;
	case J_DEFENDER :			
		gen[3] = 1;
		gen[0] = 1;
		break;
	case J_STRANGER :			
		gen[3] = 1;
		gen[2] = 1;
		break;
	}
	level->gen[0] = gen[0];
	level->gen[1] = gen[1];
	level->gen[2] = gen[2];
	level->gen[3] = gen[3];
}

int CSkillLogic::SKILL_Add( playerCharacter_t *pc, int skillTableIdx )
{
	int i;
	if( SKILL_IsValid( pc, skillTableIdx ) ) return( -1 );

	
	if ( skillTableIdx < 0 || skillTableIdx > g_skillTableNumber )
		return -1;
	


	for( i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
	{
		if( pc->skill[i].tableIdx < 0 ) break;
	}
	
	if( i == MAX_NUMBER_OF_SKILL ) return( -1 );
	pc->skill[i].idx = i;
	pc->skill[i].tableIdx = skillTableIdx;
	
	SKILL_GetInitSkillLevel( skillTableIdx, &pc->skill[i].level );
	pc->skillNumber ++;

	GTH_PC_Calculate( pc );

	return( i );
}

int CSkillLogic::SKILL_LevelUp( playerCharacter_t* pc, int idx, int genType )
{
	int skillTableIdx;
	int reply = 1, error = 0, skillClass = 0;
	int sphereIdx = 0;

	if( pc->skill[idx].tableIdx < 0 )
	{
		reply = 0;
		error = ERROR_SKILL_4;
		GTH_SendMessage_Reply_SkillLevelUp( pc, reply, error, idx, genType );
		return 0;
	}
	skillTableIdx = pc->skill[idx].tableIdx;

	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL)
	{
		TRACE("Error! pSkillTable==NULL   \n");
		return false;
	}
	
	
	int curLevel, gradeClass, limitLevel;
	int reqGenCapability, reqGenLevel;
	
	limitLevel = 1;
	gradeClass = 0;

	limitLevel = g_SkillDataGlobal.GetLimitlevel(skillTableIdx,genType);
	gradeClass = g_SkillDataGlobal.GetGradeClass(skillTableIdx,genType);

	curLevel = pc->skill[idx].level.gen[genType];
	if( curLevel >= limitLevel )
	{
		reply = 0;
		error = ERROR_SKILL_5;
		GTH_SendMessage_Reply_SkillLevelUp( pc, reply, error, idx, genType );
		return 0;
	}

	reqGenCapability = g_skillGradeClassTable[gradeClass].genCapability[curLevel];
	if( pc->genCapability[genType] < reqGenCapability || reqGenCapability <= 0 )
	{
		reply = 0;
		error = ERROR_SKILL_7;
		GTH_SendMessage_Reply_SkillLevelUp( pc, reply, error, idx, genType );
		return 0;
	}
	
	reqGenLevel = g_skillGradeClassTable[gradeClass].limitGenLevel[curLevel];
	if( pc->genLevel < reqGenLevel )
	{
		reply = 0;
		error = ERROR_SKILL_6;
		GTH_SendMessage_Reply_SkillLevelUp( pc, reply, error, idx, genType );
		return 0;
	}

	pc->genCapability[genType] -= reqGenCapability;
	pc->skill[idx].level.gen[genType] ++;

	GTH_PC_Calculate( pc );

	GTH_SendMessage_Reply_SkillLevelUp( pc, reply, error, idx, genType );



	return 1;
}

void CSkillLogic::SKILL_SendMessageMultiTarget( playerCharacter_t *pc, messageInfo_t *msginfo, int targetNumber, int dupflag, int delay )
{
	message_t message;
	float dist;
	int i, n;
	i3zoneScanList_t *targetList;
	int targetListNumber;

	
	msginfo->targetType = pc->targetType;
	msginfo->targetIdx = pc->targetIdx;
	
	g_skillMultiTarget.targetType[0] = pc->targetType;
	g_skillMultiTarget.targetIdx[0] = pc->targetIdx;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, msginfo );
	AI_SendMessage( &message );

	switch( pc->targetType )
	{
	case ENTITY_MONSTER :
		GTH_GetMonsterListInValidRange( pc->worldIdx, pc->position, 1 );
		targetList = &g_zoneMonsterList[0];
		targetListNumber = g_zoneMonsterListNumber;
		break;
	}
	
	if( targetListNumber < targetNumber ) targetNumber = targetListNumber;

	i = 0;
	n = 1;
	while( n < targetNumber && i < targetListNumber )
	{
		if( targetList[i].idx == pc->targetIdx && !dupflag )
		{
			i ++;
			continue;
		}
		dist = GTH_GetDistanceFromTarget( pc->position, targetList[i].idx, pc->targetType );
		if( dist > pc->baseAttackLength )
		{
			i ++;
			continue;
		}
		msginfo->targetType = pc->targetType;
		msginfo->targetIdx = targetList[i].idx;
		if( delay ) msginfo->attack.delayTime += pc->calAttackDelay;
		g_skillMultiTarget.targetType[n] = pc->targetType;
		g_skillMultiTarget.targetIdx[n] = targetList[i].idx;

		AI_MakeMessage( pc, &message, MESSAGE_USEINFO, msginfo );
		AI_SendMessage( &message );

		n ++;
		i ++;

		if( n == MAX_COUNT_SKILL_MULTI_TARGET ) break;
	}

	g_skillMultiTarget.targetNumber = n;
}

void CSkillLogic::SKILL_GetValue( playerCharacter_t *pc, int skillIdx, float value[] )
{

	
}

int CSkillLogic::SKILL_CheckPassive( playerCharacter_t* pc, int skillIdx )
{
	int skillTableIdx;
	int  checkItemType, checkItem;
	int weaponType, itemIdx, itemTableIdx, classIdx;

	if( skillIdx < 0 ) return( false );
	
	skillTableIdx = pc->skill[skillIdx].tableIdx;
	if( skillTableIdx < 0 ) return( false );
	
	
	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(pc->skill[skillIdx].tableIdx);
	if(pSkillTable==NULL)
	{
		TRACE("Error! pSkillTable==NULL   \n");
		return 0;
	}

	checkItemType = pSkillTable->GetSkillRequire()->m_nItemType;
	checkItem = pSkillTable->GetSkillRequire()->m_nItem;



	switch( checkItemType )
	{
	case -1 :
		{
			return( true );
		}
		break;
	case 0 :
		{
			itemIdx = pc->equipment[EQUIP_WEAPON_1 + pc->curWeapon];
			if( itemIdx < 0 ) return( false );
			itemTableIdx = pc->item[itemIdx].itemTableIdx;
			classIdx = g_itemTable[itemTableIdx].classIdx;
			weaponType = g_itemClassTable[classIdx].weaponType;
			if( weaponType == checkItem ) return( true );

			return( false );
		}
		break;
	case 1 :
		{
			itemIdx = pc->equipment[EQUIP_WEAPON_1 + pc->curWeapon];
			if( itemIdx < 0 ) return( false );
			itemTableIdx = pc->item[itemIdx].itemTableIdx;

			if( itemTableIdx == checkItem ) return( true );

			return( false );
		}
		break;
	}
	return( false );
}


void CSkillLogic::SKILL_ApplyPassive( playerCharacter_t* pc )
{
	int i, n;
	int tableIdx;

	n = 0;
	for( i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
	{
		if( pc->skill[i].tableIdx < 0 ) continue;
		tableIdx = pc->skill[i].tableIdx;

		if( SKILL_CheckPassive( pc, i ) )
		{			
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(pc->skill[i].tableIdx);
			if(pSkillTable==NULL)
			{
				TRACE("Error! pSkillTable==NULL   \n");
				continue;
			}

			if( pSkillTable->m_nUsageType)
			{
				
				CStr_Skill_Msg Msg;
				Msg.Set(tableIdx,i);

				Msg.pcIdx = pc->idx;
				g_SkillProc.Active(pc,&Msg);
			}
		}
		n ++;
		if( n >= pc->skillNumber ) break;
	}

	
}

int CSkillLogic::SKILL_CheckActive( playerCharacter_t* pc, int skillIdx )
{
	int skillTableIdx;
	int  checkItemType, checkItem;
	int weaponType, itemIdx, itemTableIdx, classIdx;

	if( skillIdx < 0 ) return( false );
	
	if( !pc->skillCastingState ) return( false );
	
	skillTableIdx = pc->skill[skillIdx].tableIdx;
	if( skillTableIdx < 0 ) return( false );

	
	if( pc->skillCooltime[skillIdx] > g_timer->GetAppMilliTime() ) return( false );


	
	
	


	
	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL)
	{
		TRACE("Error! pSkillTable==NULL   \n");
		return 0;
	}
	
	checkItemType = pSkillTable->GetSkillRequire()->m_nItemType;
	checkItem = pSkillTable->GetSkillRequire()->m_nItem;
	
	switch( checkItemType )
	{
	case -1 :
		{
			return( true );
		}
		break;
	case 0 :
		{
			itemIdx = pc->equipment[EQUIP_WEAPON_1 + pc->curWeapon];
			if( itemIdx < 0 ) return( false );
			itemTableIdx = pc->item[itemIdx].itemTableIdx;
			classIdx = g_itemTable[itemTableIdx].classIdx;
			weaponType = g_itemClassTable[classIdx].weaponType;
			if( weaponType == checkItem ) return( true );

			return( false );
		}
		break;
	case 1 :
		{
			itemIdx = pc->equipment[EQUIP_WEAPON_1 + pc->curWeapon];
			if( itemIdx < 0 ) return( false );
			itemTableIdx = pc->item[itemIdx].itemTableIdx;

			if( itemTableIdx == checkItem ) return( true );

			return( false );
		}
		break;
	}

	return( false );
}


void CSkillLogic::SKILL_ApplyActive( playerCharacter_t* pc, int skillIdx, int &coolTime )
{
	int tableIdx;
	int targetType;
	vec3_t pos, angles;

	
	g_skillMultiTarget.targetNumber = 0;

	tableIdx = pc->skill[skillIdx].tableIdx;

	
	if( tableIdx < 0 || tableIdx >= MAX_NUMBER_OF_SKILL ) return;

	
	g_affectSkillLevel = pc->skill[skillIdx].level.gen[0] + pc->skill[skillIdx].level.gen[1];
	g_affectSkillLevel += pc->skill[skillIdx].level.gen[2] + pc->skill[skillIdx].level.gen[3];
	
	
	if(pc->skillDamage == true )
	{
		
		CStr_Skill_Msg Msg;
		Msg.Set(tableIdx,skillIdx);

		Msg.pcIdx = pc->idx;
		g_SkillProc.Active(pc,&Msg);
	}

	
	CSkillCastValue *pCast = g_SkillDataGlobal.GetCastValue(pc,NULL,tableIdx,SKILL_CAST_TABLE_INDEX,0,skillIdx);
	if(pCast)
	{
		coolTime = (int)( pCast->m_fCool_Time * 1000.0f );
		coolTime += (int)( ( pCast->m_fCool_Time * 1000.0f ) * pc->optValue[OV_DEC_COOLTIME_RATE] * 0.01f );
	}

	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(tableIdx);
	if(pSkillTable==NULL)
	{
		TRACE("Error! pSkillTable==NULL   \n");
		return ;
	}

	targetType = pSkillTable->m_nTargetType;

	switch( targetType )
	{
	case SKILL_TARGET_MONSTER :
	case SKILL_TARGET_PLAYER:
		if( !GTH_GetTargetPosition( pc, pos ) ) return;
		vec_sub( pos, pc->position, pos );
		vectoangles( pos, angles );
		pc->angles[YAW] = angles[YAW];
		break;
	}


	
}



CSkillCastValue * CSkillLogic::SKILL_GetValueNotOwnSkill( int skillTableIdx, int level )
{
	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL)
	{
		TRACE("Error! pSkillTable==NULL   \n");
		return NULL;
	}

	CSkillCastValue * pCast = g_SkillDataGlobal.GetCastValue(NULL,NULL,skillTableIdx,SKILL_CAST_TABLE_INDEX,level);
	return pCast;



}




void CSkillLogic::SKILL_ApplyActiveNotOwnSkill( playerCharacter_t *pc, int skillTableIdx, int skillLevel )
{
	int targetType;
	vec3_t pos, angles;

	
	g_skillMultiTarget.targetNumber = 0;

	
	if( skillTableIdx < 0 ) return;

	
	g_affectSkillLevel = skillLevel;
	
	if( pc->skillDamage == true )
	{
		TRACE("SKILL_ApplyActiveNotOwnSkill \n");

		
		CStr_Skill_Msg Msg;
		Msg.Set(skillTableIdx,0,skillLevel);

		Msg.pcIdx = pc->idx;
		g_SkillProc.Active(pc,&Msg);
	}

	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL)
	{
		TRACE("Error! pSkillTable==NULL   \n");
		return ;
	}

	targetType = pSkillTable->m_nTargetType;

	switch( targetType )
	{
	case SKILL_TARGET_MONSTER :
		if( !GTH_GetTargetPosition( pc, pos ) ) return;
		vec_sub( pos, pc->position, pos );
		vectoangles( pos, angles );
		pc->angles[YAW] = angles[YAW];
		break;
	}

	
}


void CSkillLogic::SKILL_Monster_ApplyActive( monsterCharacter_t* pMonster, int skillIdx, int& coolTime )
{
	int tableIdx;
	int targetType;
	vec3_t pos, angles;
	


	pMonster->basic.skillNumber;
	pMonster->basic.skillIdx[skillIdx];
	pMonster->basic.skillLevel[skillIdx];

	
	g_skillMultiTarget.targetNumber = 1;

	tableIdx = pMonster->basic.skillIdx[skillIdx];

	
	if( tableIdx < 0 || tableIdx >= MAX_SKILL_NUMBER ) return;

	g_affectSkillLevel = pMonster->skill[skillIdx].level.gen[0] + pMonster->skill[skillIdx].level.gen[1];
	g_affectSkillLevel += pMonster->skill[skillIdx].level.gen[2] + pMonster->skill[skillIdx].level.gen[3];

	
	
	CStr_Skill_Msg Msg;
	Msg.Set(tableIdx,skillIdx);

	Msg.monsterIdx = pMonster->idx;
	g_SkillProc.Active(pMonster,&Msg);

	
	
	
	CSkillCastValue * pCast = g_SkillDataGlobal.GetCastValue(NULL,pMonster,tableIdx,SKILL_CAST_TABLE_INDEX);
	if(pCast)
	{
		coolTime = (int)( pCast->m_fCool_Time * 1000.0f );
	}
	else
	{
		TRACE("Error!! pCast == NULL \n");
		coolTime=0;
	}
	
	pMonster->skillCooltime[skillIdx] = g_timer->GetAbsoluteTime() * 1000.0f + coolTime;

	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(tableIdx);
	if(pSkillTable==NULL)
	{
		TRACE("Error! SKILL_Monster_ApplyActive pSkillTable==NULL   \n");
		return ;
	}

	targetType = pSkillTable->m_nTargetType;

	switch( targetType )
	{
	case SKILL_TARGET_MONSTER :	
		if( !GTH_GetTargetPosition( pMonster, pos ) ) return;
		vec_sub( pos, pMonster->position, pos );
		vectoangles( pos, angles );
		pMonster->angles[YAW] = angles[YAW];
		break;

	case SKILL_TARGET_PLAYER :	
		if( !GTH_GetTargetPosition( pMonster, pos) )	return;
		vec_sub(pos, pMonster->position, pos);
		vectoangles(pos, angles);
		pMonster->angles[YAW] = angles[YAW];
		break;
	}


	
}



void CSkillLogic::SKILL_Monster_GetValue( monsterCharacter_t *pMonster, int tableIdx, float value[] )
{

	
}

void CSkillLogic::GTH_UseSkillItem_DeleteSend(int invenPos)
{
	int itemIdx = g_curPC->inventory[invenPos];
	item_t *item = &g_curPC->item[itemIdx];

	
	g_logSystem->WriteItemDeleteLog(g_curPC,g_curPC->item[itemIdx]);	

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
			return;				
		}
	}	
	
	GTH_SendMessage_Reply_Use_Potion( g_curPC, 0, invenPos );
}



void CSkillLogic::GTH_ProcessMessage_Request_Use_Potion()
{
	item_t *item;
	int itemIdx, itemTableIdx; 
	int invenPos;
	int reply = 1;

	invenPos = MSG_ReadByte();

	
	int TargetIdx = MSG_ReadShort();
	int TargetType = MSG_ReadByte();

	itemIdx = g_curPC->inventory[invenPos];

	if( itemIdx < 0 )
	{
		reply = 0;
		int error = ERROR_CONSUMPTIVE_0;
		GTH_SendMessage_Reply_Use_Potion( g_curPC, error, invenPos );
		return;
	}
	else
	{
		item = &g_curPC->item[itemIdx];
		itemTableIdx = item->itemTableIdx;


		
		
		if(g_SkillItem.IfSkillItem(itemTableIdx))
		{
			int SkillTableIndex = g_SkillItem.GetSkillTableIndex(itemTableIdx);
			if(SkillTableIndex < 0 ) return;

			int level = g_SkillItem.GetLevel(itemTableIdx);
			if(level < 0) return;			

			if(TargetIdx != -1 && TargetType != -1)
			{
				g_curPC->targetIdx = TargetIdx;
				g_curPC->targetType= TargetType;
			}

			g_PcSkill.GTH_PC_SkillNotOwn( g_curPC, CSkillItem::SELECT_ITEM,invenPos,SkillTableIndex, level, 0 );  
		}
		
		else
		{
			GTH_UseSkillItem_DeleteSend(invenPos);
		}
	}
}
