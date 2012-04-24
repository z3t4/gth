#include "../global_def.h"

extern itementity_t *g_mouseSelectedItemObject;
extern Fx_CHARACTER_t *g_mouseSelectedCharacter;

CSkillLogic	g_SkillLogic;

CSkillLogic::CSkillLogic()
{
	m_bSendTransform=false;
}

CSkillLogic::~CSkillLogic()
{
}


void CSkillLogic::UnSelectSkill()
{
	g_cgv.saveSkillIdx	= -1;
	g_cgv.Skill.iUseInvenPos = -1;
	g_cgv.Skill.iSelectSkillUseType = globalVariable_t::SELECT_NONE;
}


void CSkillLogic::CancelSkillProcess()
{
	g_SkillFile.DebugOutString("CancelSkillProcess()  스킬 취소");

	g_pApp->m_myCharacter->event = GTH_EV_CHAR_IDLE;
	g_cgv.saveEvent = -1;
	g_cgv.resendTargeting = false;
	g_cgv.resendFlag = false;
	g_pApp->m_myCharacter->atk_resend = false;
}

int CSkillLogic::GTH_HaveSkill( int skillTableIdx )
{
	int i, n = 0;
	for( i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
	{
		if( g_cgv.myCharacterInfo->skill[i].tableIdx < 0 ) continue;
		if( g_cgv.myCharacterInfo->skill[i].tableIdx == skillTableIdx )
		{
			return( g_cgv.myCharacterInfo->skill[i].iLevel );
		}
		n ++;
		if( n == g_cgv.myCharacterInfo->skillNumber ) break;
	}
	return( -1 );
}

void CSkillLogic::GTH_AddSkill( int idx, int skillTableIdx, int initLevel )
{	
	char szTemp[256];
	g_cgv.myCharacterInfo->skill[idx].idx = idx;
	g_cgv.myCharacterInfo->skill[idx].tableIdx = skillTableIdx;
	g_cgv.myCharacterInfo->skill[idx].iLevel = initLevel;
	g_cgv.myCharacterInfo->skillNumber ++;
	
	
	
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL,341),g_SkillTableDB.GetSkillName(skillTableIdx));
	 
	g_ifMng->AddSysMessage( szTemp,  D3DCOLOR_ARGB(255, 100, 255, 255));
}



void CSkillLogic::GTH_GetSkillValue( int skillIdx, float value[] )
{

	
}



void CSkillLogic::GTH_GetSkillValue( int skillTableIdx, float value[], SKILL_GEN_LEVEL skillLevel )
{

	
}

int CSkillLogic::GTH_SkillCheckActive( int skillIdx )
{
	int skillTableIdx;
	int reqItemType, reqItem;
	char szTemp[256];
	
	
	if( g_cgv.myCharacterInfo->skillCastingtime > 0.0f ) 
	{
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 421 ) );
		return 0;
	}
	
	
	
	
	if( g_cgv.myCharacterInfo->skillCurCooltime[skillIdx] - g_timer.GetAbsoluteTime() * 1000.0f > 0.0f ) 
	{
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 422 ) );
		return 0;
	}
	else
	{
		g_cgv.myCharacterInfo->skillCurCooltime[skillIdx] = 0.0f;
	}

	
	
	if( g_pApp->m_myCharacter->ani_curSetType == ANIMTYPE_BYSKILL ) return 0;

	skillTableIdx = g_cgv.myCharacterInfo->skill[skillIdx].tableIdx;

	


	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable!=NULL)
	{
		reqItemType		= pSkillTable->GetSkillRequire()->m_nItemType;
		reqItem			= pSkillTable->GetSkillRequire()->m_nItem;
	}


	
	switch( reqItemType )
	{
		
	case SKILL_WEPON_TYPE :
		{
			int itemIdx, itemTableIdx, classIdx, weaponType;
			itemIdx = g_cgv.myCharacterInfo->equipment[g_cgv.myCharacterInfo->curWeapon + EQUIP_WEAPON_1];
			if( itemIdx < 0 ) 
			{
				sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 423 ), g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_WEAPON_TYPE, reqItem ) );
				g_ifMng->AddSysMessage( szTemp );
				return 0;
			}
			itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
			classIdx = g_itemTable[itemTableIdx].classIdx;
			weaponType = g_itemClassTable[classIdx].weaponType;
			
			
			






			

			
			
			if( reqItem != weaponType && itemTableIdx != 384) 
			{
				sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 423 ), g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_WEAPON_TYPE, reqItem ) );
				g_ifMng->AddSysMessage( szTemp );
				return 0;
			}

		}
		break;
		
		
	case SKILL_ITEM_TYPE :
		{
			int itemIdx, itemTableIdx;
			itemIdx = g_cgv.myCharacterInfo->equipment[g_cgv.myCharacterInfo->curWeapon + EQUIP_WEAPON_1];
			if( itemIdx < 0 ) 
			{
				sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 424 ), g_itemTable[reqItem].name );
				g_ifMng->AddSysMessage( szTemp );
				return 0;
			}
			itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
			
			







			
			if( reqItem != itemTableIdx && itemTableIdx != 384) 
			{
				sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 424 ), g_itemTable[reqItem].name );
				g_ifMng->AddSysMessage( szTemp );
				return 0;
			}


		}
		break;
	}	

	
	

	
	int genType = g_SkillDataGlobal.GetGrade(pSkillTable->m_nJobType);
	if(genType <0 ) return 0;

	int saCost;
	CSkillCastValue * pCastValue = g_SkillDataGlobal.GetCastValue(SKILL_CAST_TABLE_INDEX,skillTableIdx ,g_cgv.myCharacterInfo->skill[skillIdx].level.gen[genType]);
	if(pCastValue)
	{
		saCost = pCastValue->m_nSa_Comsumption - g_cgv.myCharacterInfo->calDecSaUse;	
		chASSERT(saCost >= 0 );
	
		if( saCost == 0 ) return 1;
		if( g_cgv.myCharacterInfo->curSA < saCost ) 
		{
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 425 ) );
			g_ifMng->AddSysMessage( szTemp );
			return 0;
		}
	}
	
	
	

	
	return( true );
}

void CSkillLogic::GTH_SkillCoolTimeProcess()
{
	
	
	return;

	int i, n = 0;
	for( i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
	{
		if( g_cgv.myCharacterInfo->skill[i].tableIdx < 0 ) continue;
		if( g_cgv.myCharacterInfo->skillCurCooltime[i] > 0.0f )
		{
			g_cgv.myCharacterInfo->skillCurCooltime[i] -= g_timer.GetElapsedMilliTime();
		}
		if( g_cgv.myCharacterInfo->skillCurCooltime[i] < 0.0f )
		{
			g_cgv.myCharacterInfo->skillCurCooltime[i] = 0.0f;
		}
		n ++;
		if( n == g_cgv.myCharacterInfo->skillNumber ) break;
	}
}


float CSkillLogic::GTH_GetSkillActiveDistance()
{
	float dist = -1.0f;
	
	int skillTableIdx;
	skillTableIdx = g_cgv.myCharacterInfo->skill[g_cgv.saveSkillIdx].tableIdx;

	
	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable!=NULL)
	{
		










		if( pSkillTable->GetSkillRequire()->m_nLength <= 0 )
		{
			
			
			dist = (float)g_cgv.myCharacterInfo->calAttackLength;
			return( dist );
		}
		
		dist = (float)( pSkillTable->GetSkillRequire()->m_nLength ) * 0.8f;
	}
	return( dist );
}


float CSkillLogic::GTH_GetSkillActiveDistance(int SkillIndex,int TargetType)
{
	float dist = -1.0f;
	
	int skillTableIdx;
	skillTableIdx = g_cgv.myCharacterInfo->skill[SkillIndex].tableIdx;

	
	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable!=NULL)
	{
		if( pSkillTable->GetSkillRequire()->m_nLength <= 0 )
		{
			dist = (float)g_cgv.myCharacterInfo->calAttackLength;
			return( dist );
		}
		
		dist = (float)( pSkillTable->GetSkillRequire()->m_nLength ) * 0.8f;
	}
	return( dist );
}






bool CSkillLogic::CancelTransform(bool bSendCheck)
{
	if(bSendCheck && GetMyCharTransform()) return false;

	
	if(!g_pApp->m_myCharacter->isTransform ) return false;

	g_cgv.saveSkillIdx = CANCEL_TRANSFORM_SKILL_INDEX;
	GTH_SendEventMessage_Skill();

	
	g_SkillLogic.UnSelectSkill();

	g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 81 , g_pApp ->m_myCharacter ->position , NULL , NULL , 0.0f , g_pApp ->m_myCharacter ->entityType , g_pApp ->m_myCharacter ->idxOnServer );
	g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 7 ) );
	
	SetMyCharTransform(true);
	return true;
}



int CSkillLogic::GTH_SkillProcess_Item(int Idx)
{
	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;
	vec3_t touched, delta;

	int skillTableIdx=-1;
	int SelectMySkillIndex=-1;
	
	skillTableIdx = g_SkillItem.GetSkillItemTableIndex(Idx);
	if(skillTableIdx <0) 
	{
		g_SkillFile.DebugOutString("if(skillTableIdx <0)");
		return 0;
	}

	if(!GTH_EnableSkillProcess(Idx)) return 0;

	CSkilTable *pSkillTable	= g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL) 
	{
		g_SkillFile.DebugOutString("스킬 pSkillTable==NULL");
		return 0;
	}

	int SkillTargetType = pSkillTable->m_nTargetType;

	
	if(GTH_SkillProcess_TargetSetting(SkillTargetType, touched,delta))
	{
		g_cgv.Skill.iSelectSkillUseType = globalVariable_t::SELECT_ITEM;

		g_cgv.Skill.iSelectSkillTableIdx	= skillTableIdx;
		g_cgv.Skill.iUseInvenPos			= Idx;

		
		
		CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
		if(NULL != pSkillTable)
		{
			if(pSkillTable->m_nTargetType == SKILL_TARGET_MYSELF)
			{
				if(pSkillTable->m_nTargetType == SKILL_TARGET_MYSELF)	
				{
					
					
					
					g_cgv.clickEvent		= GTH_CEV_CHAR_SKILL;
					g_cgv.saveSkillTarget	= SkillTargetType;
					
					
					g_cgv.saveTargetType	= ENTITY_PC;
					g_cgv.saveTargetIdx		= g_cgv.myPCIdxOnServer;
					
					g_SkillFile.DebugOutString("스킬 처리요청 SkillIndex %d, Target %d",g_cgv.saveSkillIdx,g_cgv.saveSkillTarget);
					return true;
				}
			}
		}

		g_cgv.clickEvent		= GTH_CEV_CHAR_SKILL;
		g_cgv.saveSkillTarget	= SkillTargetType;
		g_cgv.saveTargetType	= g_pApp->m_myCharacter->targetType;
		g_cgv.saveTargetIdx		= g_pApp->m_myCharacter->targetIdx;

		g_SkillFile.DebugOutString("스킬 처리요청 SkillIndex %d, Target %d",g_cgv.saveSkillIdx,g_cgv.saveSkillTarget);
	}
	
	else
	{
		g_SkillFile.DebugOutString("스킬 타겟 셋팅 실패 SkillskillTableIdxIndex %d, Target %d",skillTableIdx,SkillTargetType);
		g_SkillLogic.UnSelectSkill();
		return false;
	}

	return true;	
}


bool CSkillLogic::GTH_EnableSkillProcess(int Idx)
{
	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;

	
	if(!g_pApp->m_myCharacter->isAlive) 
	{
		g_SkillFile.DebugOutString("죽은 상태에서 스킬 사용 불가");
		return false;
	}

	if( g_cgv.sendSkillFlag && myChar->event == GTH_EV_CHAR_SKILL && g_cgv.resendFlag )				
	{
		g_SkillFile.DebugOutString("스킬 g_cgv.sendSkillFlag && myChar->event == GTH_EV_CHAR_SKILL && g_cgv.resendFlag");
		return false;
	}

	if( g_cgv.sendTeleportFlag && myChar->event == GTH_EV_CHAR_BEFORE_TELEPORT && g_cgv.resendFlag  )	
	{
		g_SkillFile.DebugOutString("스킬 g_cgv.sendTeleportFlag && myChar->event == GTH_EV_CHAR_BEFORE_TELEPORT && g_cgv.resendFlag");
		return false;
	}

	
	
	if( myChar->ani_curSetType == ANIMTYPE_BYSKILL ) 
	{
		g_SkillFile.DebugOutString("스킬 myChar->ani_curSetType == ANIMTYPE_BYSKILL");
		return false;
	}
	return true;
}


int CSkillLogic::GTH_SkillProcess_Skill(int Idx)
{
	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;
	vec3_t touched, delta;

	int skillTableIdx=-1;
	int SelectMySkillIndex=-1;

	SelectMySkillIndex = Idx;

	if( SelectMySkillIndex < 0 || SelectMySkillIndex >= MAX_NUMBER_OF_SKILL) 
	{
		return 0;
	}

	if( g_cgv.myCharacterInfo->skill[SelectMySkillIndex].tableIdx < 0 ) 
	{	
		g_SkillFile.DebugOutString("GTH_SkillProcess ... g_cgv.myCharacterInfo->skill[SelectMySkillIndex].tableIdx < 0");
		return 0;
	}

	
	
	if( !GTH_SkillCheckActive( SelectMySkillIndex ) ) 
	{
		g_SkillFile.DebugOutString("스킬 !GTH_SkillCheckActive( SelectMySkillIndex )");
		return 0;
	}

	skillTableIdx		= g_cgv.myCharacterInfo->skill[SelectMySkillIndex].tableIdx;

	if(!GTH_EnableSkillProcess(Idx)) return 0;

	
	if(CancelTransform()) 
	{
		g_SkillFile.DebugOutString("스킬 CancelTransform()");
		return 0;
	}

	CSkilTable *pSkillTable	= g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL) 
	{
		g_SkillFile.DebugOutString("스킬 pSkillTable==NULL");
		return 0;
	}

	int SkillTargetType = pSkillTable->m_nTargetType;

	
	if(GTH_SkillProcess_TargetSetting(SkillTargetType, touched,delta))
	{
		g_cgv.Skill.iSelectSkillUseType = globalVariable_t::SELECT_NONE;

		g_cgv.saveSkillIdx		= SelectMySkillIndex;
		g_cgv.clickEvent		= GTH_CEV_CHAR_SKILL;
		g_cgv.saveSkillTarget	= SkillTargetType;
		g_cgv.saveTargetType	= g_pApp->m_myCharacter->targetType;
		g_cgv.saveTargetIdx		= g_pApp->m_myCharacter->targetIdx;

		g_SkillFile.DebugOutString("스킬 처리요청 SkillIndex %d, Target %d",g_cgv.saveSkillIdx,g_cgv.saveSkillTarget);
	}
	
	else
	{
		g_SkillFile.DebugOutString("스킬 타겟 셋팅 실패 SkillskillTableIdxIndex %d, Target %d",skillTableIdx,SkillTargetType);
		g_SkillLogic.UnSelectSkill();
		return false;
	}

	return true;	
}

int CSkillLogic::GTH_SkillProcess(int UseType, int Idx)
{
	if(UseType == globalVariable_t::SELECT_ITEM)
	{
		return GTH_SkillProcess_Item(Idx);
	}
	else
	{
		return GTH_SkillProcess_Skill(Idx);
	}

	return true;	
}


int CSkillLogic::GTH_SkillProcess_TargetSetting(int &SkillTargetType,vec3_t &touched,vec3_t  &delta)
{
	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	

	switch( SkillTargetType )
	{
	
	case SKILL_TARGET_NONE :
		{
		}
		break;
	
	case SKILL_TARGET_MYSELF :
		{
			VectorCopy( g_cgv.saveSkillTargetPos, g_pApp->m_myCharacter->position );
		}
		break;
	
	case SKILL_TARGET_OTHER_PC :
		{
			g_cgv.pTargetChar = g_GameTarget.GTH_GetTargetPointer( g_pApp->m_myCharacter );
			if( g_cgv.pTargetChar == NULL ) return 0;

			
			if( !g_SkillApp.CheckActionTarget( ENTITY_PC ) ) return 0;	

			VectorClear( g_cgv.oldTargetPosition );	
			VectorCopy( touched, g_cgv.pTargetChar->position );

			g_cursor->SetPointerTargetPos( touched , myChar->position );
			g_cgv.clickEvent = GTH_CEV_CHAR_SKILL;
			
			VectorCopy( g_cgv.saveSkillTargetPos, g_cgv.pTargetChar->position );
		}
		break;
		
	case SKILL_TARGET_PLAYER :
		{
			g_cgv.pTargetChar = g_GameTarget.GTH_GetTargetPointer( g_pApp->m_myCharacter );
			if( g_cgv.pTargetChar == NULL ) return 0;

			if( !g_SkillApp.CheckActionTarget( ENTITY_PC ) ) return 0;	
			
			SkillTargetType = SKILL_TARGET_OTHER_PC;

			VectorClear( g_cgv.oldTargetPosition );	
			VectorCopy( touched, g_cgv.pTargetChar->position );
			g_cursor->SetPointerTargetPos( touched , myChar->position );				
			g_cgv.clickEvent = GTH_CEV_CHAR_SKILL;
			
			VectorCopy( g_cgv.saveSkillTargetPos, g_cgv.pTargetChar->position );
		}
		break;
		
	
	case SKILL_TARGET_NPC :
		{
		}
		break;
		
	
	case SKILL_TARGET_MONSTER :
		{
			g_cgv.pTargetChar = g_GameTarget.GTH_GetTargetPointer( g_pApp->m_myCharacter );
			if( g_cgv.pTargetChar == NULL ) return 0;

			
			if( g_cgv.pTargetChar->entityType == ENTITY_PC )
			{
				if( !g_AttackProc.IsSaftTargeting() ) return 0;

				
				AddTargetList( g_cgv.pTargetChar->name );
			}
			else
			{
				if( !g_SkillApp.CheckActionTarget( ENTITY_MONSTER ) ) return 0;	
				if( g_cgv.pTargetChar->stat_alive > MON_ALIVE_STATE_NORMAL ) return 0;
			}
			
			VectorClear( g_cgv.oldTargetPosition );	
			VectorCopy( touched, g_cgv.pTargetChar->position );
			g_cursor->SetPointerTargetPos( touched , myChar->position );				
			
			g_cgv.clickEvent = GTH_CEV_CHAR_SKILL;
			
			VectorCopy( g_cgv.saveSkillTargetPos, g_cgv.pTargetChar->position );
		}
		break;
		
	case SKILL_TARGET_AREA :
		{
			
			AngleToVector( g_pApp->m_myCharacter->angles, delta );
			VectorNormalize( delta );
			VectorScale( delta, delta, 200.0f );
			VectorAdd( touched, delta, g_pApp->m_myCharacter->position );

			g_cgv.clickEvent = GTH_CEV_CHAR_SKILL;
			VectorCopy( g_cgv.saveSkillTargetPos, touched );
		}
		break;

	case SKILL_TARGET_ITEM :
		{
			return 0;
		}
		break;

	case SKILL_TARGET_CORPSE :
		{
			g_cgv.pTargetChar = g_GameTarget.GTH_GetTargetPointer( g_pApp->m_myCharacter );
			if( g_cgv.pTargetChar == NULL ) return 0;

			if( !g_SkillApp.CheckActionTarget( ENTITY_MONSTER ) ) return 0;	
			if( g_cgv.pTargetChar->stat_alive != MON_ALIVE_STATE_CORPSE ) return 0;
			
			VectorClear( g_cgv.oldTargetPosition );	
			VectorCopy( touched, g_cgv.pTargetChar->position );
			g_cursor->SetPointerTargetPos( touched , myChar->position );
			g_cgv.clickEvent = GTH_CEV_CHAR_SKILL;
			VectorCopy( g_cgv.saveSkillTargetPos, g_cgv.pTargetChar->position );
		}
		break;
	default :
		{
			return 0;
		}
		break;
	}

	return 1;
}



int CSkillLogic::GTH_SkillClickEvent()
{
	float dist;

	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;
	
	
	int SkillTableIndex = -1;

	
	if(g_cgv.Skill.iSelectSkillUseType == globalVariable_t::SELECT_ITEM)
	{
		if( g_cgv.Skill.iUseInvenPos < 0 ) return 0;
		SkillTableIndex = g_cgv.Skill.iSelectSkillTableIdx;
	}
	else
	{
	if( g_cgv.saveSkillIdx < 0 ) return 0;
	if( g_cgv.myCharacterInfo->skill[g_cgv.saveSkillIdx].tableIdx < 0 ) return 0;

		SkillTableIndex = g_cgv.myCharacterInfo->skill[g_cgv.saveSkillIdx].tableIdx;
	}

	switch( g_cgv.saveSkillTarget )
	{
	case SKILL_TARGET_NONE :
		{
			myChar->event = GTH_EV_CHAR_SKILL;
			g_cgv.sendSkillFlag = true;
			g_cgv.resendFlag = true;
			g_cgv.resendAttack = false;
			g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		}
		break;
	case SKILL_TARGET_MYSELF :
		{
			myChar->event = GTH_EV_CHAR_SKILL;
			g_cgv.sendSkillFlag = true;
			g_cgv.resendFlag = true;
			g_cgv.resendAttack = false;
			g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		}
		break;
		
	case SKILL_TARGET_OTHER_PC :
		{
			dist = g_GameTarget.GTH_GetTargetDistance( myChar );
			if( dist < 0.00000f )
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				myChar->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendTargeting = false;
				return 0;
			}

			g_cgv.mouseClickEventDistance = GTH_GetSkillActiveDistance();
			if( g_cgv.mouseClickEventDistance < 0.0f )
			{
				g_cgv.resendTargeting = false;
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				myChar->event = GTH_EV_CHAR_IDLE;
				return 0;
			}
			if( dist < g_cgv.mouseClickEventDistance )
			{
				g_cursor->QuitPlayerTargetedMove();
				g_GameTarget.GTH_SetAngleToTarget( myChar );
				
				g_cgv.resendTargeting = false;				
				myChar->event = GTH_EV_CHAR_SKILL;
				g_cgv.sendSkillFlag = true;
				g_cgv.resendFlag = true;
				g_cgv.resendAttack = false;
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				return 0;
			}
			else
			{
				g_cgv.resendTargeting = true;
			}
		}
		break;





	case SKILL_TARGET_NPC :
		{
			return 0;
		}
		break;

	case SKILL_TARGET_PLAYER :
	case SKILL_TARGET_MONSTER :
		{
			dist = g_GameTarget.GTH_GetTargetDistance( myChar );
			if( dist < 0.00000f )
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				myChar->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendTargeting = false;
				return 0;
			}

			g_cgv.mouseClickEventDistance = GTH_GetSkillActiveDistance();
			if( g_cgv.mouseClickEventDistance < 0.0f )
			{
				g_cgv.resendTargeting = false;
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				myChar->event = GTH_EV_CHAR_IDLE;
				return 0;
			}

			if( dist < g_cgv.mouseClickEventDistance )
			{
				
				myChar->event = GTH_EV_CHAR_SKILL;

				if( SkillTableIndex == 7 ||
					SkillTableIndex == 12 )
				{
					if( dist > 150 )
					{
						if( myChar->event != GTH_EV_CHAR_BEFORE_TELEPORT )
						{
							myChar->event = GTH_EV_CHAR_BEFORE_TELEPORT;
							g_cursor->QuitPlayerTargetedMove();
							g_GameTarget.GTH_SetAngleToTarget( myChar );

							g_cgv.resendTargeting	= false;				
							g_cgv.sendSkillFlag		= false;
							g_cgv.sendTeleportFlag	= true;
							g_cgv.resendFlag		= true;
							g_cgv.clickEvent		= GTH_CEV_CHAR_NONE;
							break;
						}
					}
				}

				g_cursor->QuitPlayerTargetedMove();
				g_GameTarget.GTH_SetAngleToTarget( myChar );

				g_cgv.resendTargeting	= false;				
				g_cgv.sendSkillFlag		= true;
				g_cgv.sendTeleportFlag	= false;
				g_cgv.resendFlag		= true;
				g_cgv.clickEvent		= GTH_CEV_CHAR_NONE;
				return 0;
			}
			else
			{
				if( ( SkillTableIndex == 7 || 
					  SkillTableIndex == 12 ) && dist < 1000 )
				{
					if( myChar->event != GTH_EV_CHAR_BEFORE_TELEPORT )
					{
						g_cgv.resendTargeting	= false;
						g_cgv.sendSkillFlag		= false;
						g_cgv.sendTeleportFlag	= true;
						g_cgv.resendFlag		= true;
						myChar->event			= GTH_EV_CHAR_BEFORE_TELEPORT;
					}

					g_cursor->QuitPlayerTargetedMove();
					g_GameTarget.GTH_SetAngleToTarget( myChar );

					g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
					return 0;
				}

				g_cgv.resendTargeting = true;
			}
		}
		break;
	case SKILL_TARGET_AREA :
		{
			myChar->event = GTH_EV_CHAR_SKILL;
			g_cgv.sendSkillFlag = true;
			g_cgv.resendFlag = true;
			g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		}
		break;
	case SKILL_TARGET_ITEM :
		{
			return 0;
		}
		break;
	case SKILL_TARGET_CORPSE :
		{
			dist = g_GameTarget.GTH_GetTargetDistance( myChar );
			if( dist < 0.00000f )
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				myChar->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendTargeting = false;
				return 0;
			}

			g_cgv.mouseClickEventDistance = GTH_GetSkillActiveDistance();
			if( g_cgv.mouseClickEventDistance < 0.0f )
			{
				g_cgv.resendTargeting = false;
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				myChar->event = GTH_EV_CHAR_IDLE;
				return 0;
			}
			if( dist < g_cgv.mouseClickEventDistance )
			{
				g_cursor->QuitPlayerTargetedMove();
				g_GameTarget.GTH_SetAngleToTarget( myChar );
				
				g_cgv.resendTargeting = false;				
				myChar->event = GTH_EV_CHAR_SKILL;
				g_cgv.sendSkillFlag = true;
				g_cgv.resendFlag = true;
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				return 0;
			}
			else
			{
				g_cgv.resendTargeting = true;
			}
		}
		break;
	default :
		{
			return 0;
		}
		break;
	}
	return 1;	
}

int CSkillLogic::GTH_GetSkillEffectLevel( int skillTableIdx, int skillLevel )
{


	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL) return 0;

	for( int i = 0; i < MAX_COUNT_EFFECT_LEVEL; i ++ )
	{
		
		if( pSkillTable->GetSkillEffect()->m_nEffectLevel[i] == 0 ) return( 4 );
		if( skillLevel <= pSkillTable->GetSkillEffect()->m_nEffectLevel[i] ) return( i );
	}
	return( i );
}

void CSkillLogic::GTH_AddSkillEffect( CSkillEffect *pEffect, Fx_CHARACTER_t *attacker, Fx_CHARACTER_t *defender, int effectLevel )
{
	int classType, effectID;
	
	vec3_t origin, target;



	classType = pEffect->m_nClassType;
	effectID  = pEffect->m_nEffectID;

	switch( pEffect->m_nTarget)
	{
		
	case 0 :
		break;
		
	case 1 :
		VectorCopy( origin, attacker->position );
		
		g_effectMng ->Add ( (GTH_EFFECT_CLASS_TYPE)classType , effectID , origin , NULL , attacker ->angles, -20.0f, attacker ->entityType , attacker ->idxOnServer , -1 , -1 , effectLevel );
		break;
		
	case 2 :
		if( defender == NULL ) return;
		
		VectorSubtract ( target, defender->position, attacker->position );
		VectorAdd ( target, target, defender->position );
		VectorCopy( origin, defender->position );
		
		g_effectMng ->Add ( (GTH_EFFECT_CLASS_TYPE)classType , effectID , origin , target , NULL , -20.0f , attacker ->entityType , attacker ->idxOnServer , defender ->entityType , defender ->idxOnServer , effectLevel );
		break;
		
	case 3 :
		if( defender == NULL ) return;
		
		VectorCopy( origin, attacker->position ); 
#pragma chMSG( "이넘을 지워야 할 것 같은데..." )

		VectorCopy( target, defender->position );

		


		g_effectMng ->Add( (GTH_EFFECT_CLASS_TYPE)classType, effectID, origin, target, NULL, 0.0f, 
			attacker->entityType, attacker->idxOnServer, defender->entityType, defender->idxOnServer, effectLevel, false );
		break;
		
	case 4 :		
		if( defender == NULL )
		{
			VectorCopy( origin, g_cgv.saveSkillTargetPos );
			g_effectMng ->Add ( (GTH_EFFECT_CLASS_TYPE)classType , effectID , origin );
		}
		else
		{
			VectorCopy( origin, defender->position );
			g_effectMng ->Add ( (GTH_EFFECT_CLASS_TYPE)classType , effectID , origin , NULL , NULL , -20.0f , defender ->entityType , defender ->idxOnServer , -1 , -1 , effectLevel );
		}
		
		break;		
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}


void	CSkillLogic::CheckSkillExecption(int weaponType, int skillIdx, int& aniID,CSkillEffect *pEffect)
{
	if( weaponType < 0 ) return;
	if( skillIdx != 0 && skillIdx != 5 && skillIdx != 15 )	return;






	if( skillIdx == 0 )	{
		if( weaponType == WEAPON_TYPE_ONEHAND_SWORD ) {
			aniID = 6;
			 
			pEffect->m_nClassType	=	GTH_EFFECT_CLASS_MULTI_MODEL;
			pEffect->m_nEffectID	=	28;
		} else if( weaponType == WEAPON_TYPE_TWOHAND_SWORD ) {
			aniID = 5;
			 
			pEffect->m_nClassType	=	GTH_EFFECT_CLASS_MULTI_MODEL;
			pEffect->m_nEffectID	=	29;
		}
	} else if( skillIdx == 5 ) {
		if( weaponType == WEAPON_TYPE_MAGIC_BOW ) {
			aniID = 3;
			 
			pEffect->m_nClassType	=	GTH_EFFECT_CLASS_MULTI_MODEL;
			pEffect->m_nEffectID	=	30;
		} else if( weaponType == WEAPON_TYPE_DANGGER ) {
			aniID = 5;
			 
			pEffect->m_nClassType	=	GTH_EFFECT_CLASS_MODEL;
			pEffect->m_nEffectID	=	134;
		}
	} else if( skillIdx == 15 )	{
		if( weaponType == WEAPON_TYPE_CLAW ) {
			aniID = 2;
			pEffect->m_nClassType	= GTH_EFFECT_CLASS_MULTI_MODEL;
			pEffect->m_nEffectID	= 32;
		} else if( weaponType == WEAPON_TYPE_SPIRITOR ) {
			aniID = 5;
			pEffect->m_nClassType	= GTH_EFFECT_CLASS_MODEL;
			pEffect->m_nEffectID	= 135;
		}
	} else if( skillIdx == 16 ) {
		if( weaponType == WEAPON_TYPE_CLAW ) {
			aniID = 4;
		} else if( weaponType == WEAPON_TYPE_SPIRITOR ) {
			aniID = 3;
		}
	}
}

int CSkillLogic::GTH_SkillExecute( Fx_CHARACTER_t* character, int tableIdx, int level, int multiTargetNumber, int multiTargetType[], int multiTargetIdx[] ,int targetType)
{
	
	static int count;

	Fx_CHARACTER_t *defender = NULL;

	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(tableIdx);
	if(pSkillTable==NULL) return 0;

	CSkillEffect *pEffect = pSkillTable->GetSkillEffect();
	if(pEffect==NULL) return 0;

	int skillAniID = pSkillTable->m_nAnimID;

	int effectLevel, weaponType = -1;
	float skillAniSpeed;

#pragma chMSG("스킬 애니관련 예외처리부분.....")

	
	if ( character->equipNumber > 0 )
	{
		int itemIdx = character->equipment[0];
		int mainClass = g_itemTable[itemIdx].classIdx;
		
		if( g_itemClassTable[mainClass].attackAtt )
		{
			weaponType = g_itemClassTable[mainClass].weaponType;
		}
	}

	CheckSkillExecption(weaponType, tableIdx, skillAniID, pEffect);

	if( skillAniID < 0 ) skillAniID = 0;

	effectLevel = GTH_GetSkillEffectLevel( tableIdx, level );

	if( multiTargetNumber == 0 )
	{
		if( g_cgv.saveSkillTarget != SKILL_TARGET_AREA ) defender = g_GameTarget.GTH_GetTargetPointer( character );
		CSkillEffect pTempEffect;
		pTempEffect = *pEffect;

		int itemIdx			= g_cgv.myCharacterInfo->equipment[EQUIP_WEAPON_1 + g_cgv.myCharacterInfo->curWeapon];
		int itemTableIdx	= g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
		int classIdx		= g_itemTable[itemTableIdx].classIdx;
		int weaponType		= g_itemClassTable[classIdx].weaponType;

		if( tableIdx == 5 )
		{
			if( weaponType == WEAPON_TYPE_MAGIC_BOW ) {
				pTempEffect.m_nTarget =	3;
			} else if( weaponType == WEAPON_TYPE_DANGGER ) {
				pTempEffect.m_nTarget =	1;
			}
		}

		else if( tableIdx == 15 )
		{
			pTempEffect.m_nTarget	=	3;
		}
		g_SkillLogic.GTH_AddSkillEffect( &pTempEffect, character, defender, effectLevel );		

	}
	else
	{
		for( int i = 0; i < multiTargetNumber; i ++ )
		{
			defender = g_GameTarget.GTH_GetTargetPointer( multiTargetType[i], multiTargetIdx[i] );
			g_SkillLogic.GTH_AddSkillEffect( pEffect, character, defender, effectLevel );
		}
	}
	skillAniSpeed = character->stat_attackSpeed;
	if ( character->entityType == ENTITY_MONSTER )
	{		
		for (int i =0; i < g_monsterTable[character->tableIdx].skillNumber; i++)
		{
			if ( g_monsterTable[character->tableIdx].skillIdx[i] == tableIdx )
			{
				skillAniID = g_monsterTable[character->tableIdx].skillAni[i];
				break;  
			}			
		}		
	}	

	GTH_ChangeAnimation(character, ANIMTYPE_BYSKILL, skillAniID, true, &skillAniSpeed, true );
	ShowLogInfo( "Execute Skill! : AnimIdx %d", g_pApp->m_myCharacter->ani_curIdx );

	g_SkillApp.skillResendFlag = g_charManager->m_pAnimManager->GetComboCount( g_pApp->m_myCharacter->ani_curIdx );	
		
	
	return( true );
}



int CSkillLogic::GTH_BeforeTeleportExecute( Fx_CHARACTER_t* character, int pcJob)
{
	int		skillAniID, effectID;
	float	skillAniSpeed;

	Fx_CHARACTER_t *defender = NULL;

	
	if( pcJob == J_WINDY_ROMER ) {
		effectID	= 137;
		skillAniID	= 6;
	} else if( pcJob == J_WATER || pcJob == J_DESTROYER || pcJob == J_GUARDIAN) {
		effectID	= 139;
		skillAniID	= 4;
	}

	skillAniSpeed = character->stat_attackSpeed;

	GTH_ChangeAnimation(character, ANIMTYPE_BYSKILL, skillAniID, true, &skillAniSpeed, true );

	g_effectMng->Add( GTH_EFFECT_CLASS_MODEL, effectID, character->position, NULL,
					  character->angles, 0.0f, character->entityType, character->idxOnServer );

	
	if( character->entityType == ENTITY_PC && character->idxOnServer == g_cgv.myPCIdxOnServer )
	{
		character->event = GTH_EV_CHAR_BEFORE_TELEPORT;
		g_cgv.saveEvent	 = GTH_EV_CHAR_BEFORE_TELEPORT;
	}

	return( true );
}



void CSkillLogic::GTH_AddSkillCastingEffect( Fx_CHARACTER_t *character, float castingTime )
{
	int itemTableIdx	=	-1;
	int classIdx		=	-1;
	int weaponType		=	-1;
	int effectID		=	-1;

	if( castingTime <= 10.0f ) return;

	if( character->entityType == ENTITY_PC )
	{
		
		if( character->tableIdx == 0 ) effectID = 11;
		else if( character->tableIdx == 1 ) effectID = 28;
		else if( character->tableIdx == 2 ) effectID = 20;
		else if( character->tableIdx == 3 ) effectID = 27;
		
		itemTableIdx	= character->equipment[0];
		if( itemTableIdx >= 0 )
		{
			classIdx		= g_itemTable[itemTableIdx].classIdx;
			weaponType		= g_itemClassTable[classIdx].weaponType;
		}
		switch( weaponType )
		{
			
		case WEAPON_TYPE_DANGGER :
			effectID	=	31;
			break;
			
		case WEAPON_TYPE_ONEHAND_SWORD :
			effectID	=	11;
			break;
			
		case WEAPON_TYPE_TWOHAND_SWORD :
			effectID	=	16;
			break;
			
		case WEAPON_TYPE_MAGIC_BOW :
			effectID	=	28;
			break;
			
		case WEAPON_TYPE_STAFF :
			effectID	=	20;
			break;
			
		case WEAPON_TYPE_SPIRITOR :
			effectID	=	27;
			break;
			
		case WEAPON_TYPE_CLAW :
			effectID	=	27;
			break;
		}
		if( effectID < 0 ) return;

		
		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
									effectID, 
									character->position, 
									NULL, 
									true, 
									character->entityType,
									character->idxOnServer, 
									0.0f, 
									0.0f, 
									0.0f, 
									castingTime );

		g_SkillFile.DebugOutString("캐스팅 effect를 뿌려줘! .. effectID %d, castingTime %f",effectID,castingTime);
	}
}








void CSkillLogic::GTH_DeleteSKill( int idx )
{		
	if ( idx < 0 || idx >= MAX_NUMBER_OF_SKILL )
		return;

	g_cgv.myCharacterInfo->skill[idx].idx = idx;
	g_cgv.myCharacterInfo->skill[idx].tableIdx = -1;
	g_cgv.myCharacterInfo->skill[idx].iLevel = 0;	
	g_cgv.myCharacterInfo->skillNumber --;

}
