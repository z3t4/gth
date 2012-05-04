


#define TIMER_SEC 1


#include "../global_def.h"
#include "game/GolryunBattle/GolryunBattle.h"

CAttackProc g_AttackProc;


#include "CTools.h"
extern CTools gcTools;




CAttackProc::CAttackProc()
{
	m_ntempAttackSuccessCount	=	0;
	m_ntempAttackMissCount		=	0;
}

CAttackProc::~CAttackProc()
{

}


int CAttackProc::InTargetList( char* name )
{
	switch( g_cgv.myCharacterInfo->targetingType )
	{
		
	case TARGETING_ONLY_MONSTER :
		return false;
		break;
		
	case TARGETING_NORMAL : case TARGETING_SAFE :
		{
			for( int i = 0; i < MAX_TARGET_PCLIST; i ++ )
			{
				if( !stricmp( name, g_cgv.myCharacterInfo->targetPCList[i] ) )
				{
					if( g_timer.GetAppMilliTime() - g_cgv.myCharacterInfo->targetPCListTime[i] < 60000 ) break;
				}
			}
			if( i == MAX_TARGET_PCLIST ) return false;
			return true;
		}
		break;
		
	case TARGETING_EXCEPT_PARTY :
		{
			if( g_cgv.myCharacterInfo->organizeServer < 0 ) return true;
			for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
			{
				if( g_cgv.myCharacterInfo->member[i].serverIndex < 0 ) continue;
				if( !stricmp( name, g_cgv.myCharacterInfo->member[i].memberName ) ) return false;
			}
			return true;
		}
		break;
		
	case TARGETING_EXCEPT_GUILD :
		{
			return true;
		}
		break;
		
	case TARGETING_EVERY :
		{ 
			return true;
		}
		break;
	}

	return false;
}

int CAttackProc::IsSaftTargeting()
{
	
	
	if ( (g_pApp->m_myCharacter)->idxOnServer == (g_cgv.pTargetChar)->idxOnServer )
		return false;

	
	
	float Distance =  abs(g_pApp->m_myCharacter->position[2] -
		                  g_cgv.syncCharacter[g_cgv.pTargetChar->ID].position[2]);	
	if ( Distance >= 300 )
		return false;	
	


	
	if( g_pApp->m_myCharacter->pvpMode && g_cgv.pTargetChar->pvpMode ) return true;

	
	
	if ( (g_cgv.syncCharacter[0].nDanBattleIdx >= 0 )&& 
		( g_cgv.syncCharacter[0].worldIdx == g_NewDanBattleSystem.GetDanBattleInfo()->nBattleMapIdx)){
		if ( TRUE == g_NewDanBattleSystem.IsDanBattleEnemy(g_cgv.pTargetChar->ID) )	{			
			return true;
		}
	}
	
	else{
		
		if ( IsPKDisableWorld( g_pApp->m_myCharacter->worldIdx ) )return false;
		
		if ( g_cgv.pTargetChar->idxOnServer >= 0 && g_cgv.pTargetChar->idxOnServer < MAX_CHARACTERS){
			if ( g_cgv.syncCharacter[g_cgv.pTargetChar->ID].nDanBattleIdx >= 0)
			{
			
				return false;
			}
		}		
	}
	
	
	
	
	if ( g_pApp->m_myCharacter->worldIdx == DAN_BATTLEMAP_NUM )
	{		
		
		if ( g_ifMng->m_guildArenaWin->m_ReadyBattleFlag == CIFGuildArenaWin::STATE_EVENT)
			return true;

		if ( g_cgv.myCharacterInfo->guildIdx == g_cgv.syncCharacter[g_cgv.pTargetChar->ID].guildIdx ||
			g_ifMng->m_guildArenaWin->m_ReadyBattleFlag != g_ifMng->m_guildArenaWin->STATE_BATTLE)
			return false;
		else		
			return true;		
	}		
	

	
	
	if ( g_pApp->m_myCharacter->worldIdx == CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX )
	{
		if(g_GolryunBattle.GetStatus() == tagGolryun_Packet_Status::GB_REQUEST_PROGRESS_MATCH)
			return true;
		else
			return false;
	}		
	


	


	switch( g_cgv.myCharacterInfo->targetingType )
	{
		
	case TARGETING_ONLY_MONSTER :
		{
			return false;
		}
		break;
		
	case TARGETING_NORMAL : 
		{
			
			if( !g_AttackProc.InTargetList( g_cgv.pTargetChar->name ) )
			{

				
				if ( TRUE== g_NewDanBattleSystem.IsDanBattleEnemy(g_cgv.pTargetChar->ID) )
					return true;

				
				
				if(g_cgv.pTargetChar->chaosPoint==0)
				{
					if(g_input->KeyEvent( DIK_LCONTROL )==1)
					{
						return true;
					}

					else return false;
				
				}

				else return true;

				
			
			}
			
			else 
				return true;
		}
		break;
	
		
	case TARGETING_SAFE :
		{
			
			if( !g_AttackProc.InTargetList( g_cgv.pTargetChar->name ) )
			{

				
				
				
				
				
				

			
				
				if ( TRUE== g_NewDanBattleSystem.IsDanBattleEnemy(g_cgv.pTargetChar->ID) )
					return true;
			
				
				
				if(g_cgv.pTargetChar->chaosPoint==0)
				{
					if(g_input->KeyEvent( DIK_LCONTROL )==1)
					{
						return true;
					}

					else return false;
				
				}

				else return true;

			
			
					
			
			}
			
			else return true;
		}
		break;
		
	case TARGETING_EXCEPT_PARTY :
		{
			if( g_cgv.myCharacterInfo->organizeServer < 0 ) return true;
			for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
			{
				if( g_cgv.myCharacterInfo->member[i].serverIndex < 0 ) continue;
				if( !stricmp( g_cgv.pTargetChar->name, g_cgv.myCharacterInfo->member[i].memberName ) ) return false;
			}
			return true;
		}
		break;
		
	case TARGETING_EXCEPT_GUILD :
		{
			return true;
		}
		break;
		
	case TARGETING_EVERY :
		{ 
			return true;
		}
		break;
	}
	return false;
}

void CAttackProc::GTH_SendEventMessage_Attack()	
{
	 
	 
	
	if(g_ifMng->m_autoQuestWin->m_iautoflag == AUTOQUESTSCROLL_START) return;
	
	int		animIndex	=	GTH_GetAttackAnimationIndex ();
	int		animCount	=	g_charManager ->m_pAnimManager ->GetComboCount ( animIndex );
	int		attackTimeList [MAX_DAMAGE_FRAMES];
	g_charManager ->m_pAnimManager ->CalcAttackTime ( animIndex, (float)g_cgv.myCharacterInfo->calAttackDelay , attackTimeList );
	
	
	
	
	float Distance =  abs(g_pApp->m_myCharacter->position[2] -
		                  g_cgv.syncCharacter[g_cgv.pTargetChar->ID].position[2]);	
	if ( Distance >= 300 )
		return;	
	

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_ATTACK);
		MSG_WriteShort(&netMessage, g_pApp->m_myCharacter->targetIdx);
		MSG_WriteByte(&netMessage, g_pApp->m_myCharacter->targetType);
		
		
		MSG_WriteByte(&netMessage, g_pApp->m_myCharacter->atk_resend );
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		MSG_WriteByte(&netMessage, g_pApp->m_myCharacter->event);
		//lucky 2012 no range hack
		MSG_WriteShort(&netMessage, g_cgv.myCharacterInfo->calAttackLength);
		//end
		//lucky 2012 speed hack
		MSG_WriteLong(&netMessage, g_cgv.myCharacterInfo->calAttackDelay);
		//end
		 
		 
		
		
		
		
		
		
		
		
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	


	g_pApp->m_myCharacter->atk_resend = false;
	g_cgv.saveTargetType = g_pApp->m_myCharacter->targetType;
	g_cgv.saveTargetIdx = g_pApp->m_myCharacter->targetIdx;

	
}



bool CAttackProc::GTH_EnableSkillExcuteDist(int SkillIndex,int TargetType,float fGap,float fAddGap)
{		
	g_SkillFile.DebugOutString("GTH_EnableSkillExcuteDist \n");
	
	
	if( SkillIndex == CANCEL_TRANSFORM_SKILL_INDEX) return true;

	
	int UseType = g_cgv.Skill.iSelectSkillUseType;
	
	if(UseType == globalVariable_t::SELECT_ITEM)
	{
		return true;
	}

	
	if(0 < g_cgv.Skill.iRecvSkillCastingCount) 
	{
		g_SkillFile.DebugOutString("설정 %d 이면 리턴 \n",g_cgv.Skill.iRecvSkillCastingCount);
		
		return true;
	}

	if(SkillIndex < 0)
	{
		SkillIndex = g_cgv.Skill.iSkillExecuteIndex;
		TargetType = g_cgv.Skill.iSkillExecuteTargetType;
		UseType    = g_cgv.Skill.iSkillExecuteUseType;

	
	}

	
	
	
	
	
	
	

	int skillTableIdx;
	skillTableIdx = g_cgv.myCharacterInfo->skill[SkillIndex].tableIdx;

	g_SkillFile.DebugOutString("\n마지막으로 타겟과의 거리 체크 ... SkillIndex %d, TargetType %d, skillTableIdx %d \n", SkillIndex,TargetType,skillTableIdx);
	
	
	
	
	if(	7 == skillTableIdx || 12 == skillTableIdx )  return true;

	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
	if(NULL == pSkillTable)
	{
		g_SkillFile.DebugOutString("마지막으로 타겟과의 거리 체크 ... 스킬 테이블 없음 %d\n",skillTableIdx );
		return false;
	}
	else
	{
		if( pSkillTable->m_nTargetType == SKILL_TARGET_MYSELF ) return true;
	}

	
	float TargetDist = g_GameTarget.GTH_GetTargetDistance( g_pApp->m_myCharacter );
	if( TargetDist < 0.00000f )
	{
		g_SkillFile.DebugOutString("마지막으로 타겟과의 거리 체크 ... 타겟과의 거리가 0보다 작다 %f\n",TargetDist);
		return false;
	}

	
	float SkillDist = g_SkillLogic.GTH_GetSkillActiveDistance(SkillIndex,TargetType);
	SkillDist *= fGap;	
	if( SkillDist < 0.0f )
	{
		g_SkillFile.DebugOutString("마지막으로 타겟과의 거리 체크 ... 스킬 유효거리가 0보다 작다 %f\n",SkillDist);
		return false;
	}

	
	SkillDist += fAddGap;

	if(TargetDist > SkillDist)
	{
		g_SkillFile.DebugOutString("마지막으로 타겟과의 거리 체크 ... TargetDist %f > SkillDist %f\n",TargetDist,SkillDist);
		return false;
	}

	return true;
}
int CAttackProc::GTH_SendEventMessage_Skill()
{
	
	if(!GTH_EnableSkillExcuteDist(g_cgv.saveSkillIdx,g_cgv.saveSkillTarget )) 
	{


		return false;
	}

	if( g_pApp->m_myCharacter->atk_resend )
	{
		if( g_SkillApp.skillResendFlag <= 0 ) return false;
		g_SkillApp.skillResendFlag --;
	}

	g_SkillFile.DebugOutString("스킬 전송 CC_EVENT_SKILL atk_resend %d",g_pApp->m_myCharacter->atk_resend);

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_SKILL);
		
		MSG_WriteByte(&netMessage, g_cgv.Skill.iSelectSkillUseType);
		
		
		if(g_cgv.Skill.iSelectSkillUseType == globalVariable_t::SELECT_ITEM)
		{
			MSG_WriteShort(&netMessage, g_cgv.Skill.iUseInvenPos);
		}
		else
		{
			MSG_WriteShort(&netMessage, g_cgv.saveSkillIdx);
		}
		
		MSG_WriteByte(&netMessage, g_cgv.saveSkillTarget);
		MSG_WritePosition(&netMessage, g_cgv.saveSkillTargetPos);
		
		MSG_WriteShort(&netMessage, g_cgv.saveTargetIdx);	
		MSG_WriteByte(&netMessage, g_cgv.saveTargetType);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		//lucky 2012 no range hack
		MSG_WriteShort(&netMessage,g_cgv.myCharacterInfo->calAttackLength);
		//end
		MSG_WriteByte(&netMessage, g_pApp->m_myCharacter->atk_resend );
		
		NET_SendMessage(&gsSocket, &netMessage,	true);
	}
	MSG_EndWriting( &netMessage );



	g_cgv.sendSkillFlag = false;
	g_pApp->m_myCharacter->atk_resend = false;

	
	g_cgv.Skill.iRecvSkillCastingCount--;
	g_SkillFile.DebugOutString("설정 %d  \n",g_cgv.Skill.iRecvSkillCastingCount);
		
	return 1;
}



int CAttackProc::GTH_ProcessEventMessage_Attack()
{

	int idx;
	
	int entityType;
	int targetIdx, targetType, atkDelay;
	float angles;
	int success, resultHitRate, itemIdx, dur, damageAttackFlag;
	int attackComboType;

	idx			= MSG_ReadShort();
	entityType	= MSG_ReadByte();
	angles		= MSG_ReadFloat();

	
	if( entityType == ENTITY_PC )
	{
		damageAttackFlag = MSG_ReadByte();

		success			= MSG_ReadByte();
		targetIdx		= MSG_ReadShort();
		targetType		= MSG_ReadByte();
		atkDelay		= MSG_ReadShort();
		resultHitRate	= MSG_ReadByte();
		attackComboType = MSG_ReadByte();

		
		itemIdx = MSG_ReadShort();
		if( itemIdx >= 0 ) dur = MSG_ReadShort();
	} 

	
	else
	{
		targetIdx	= MSG_ReadShort();
		targetType	= MSG_ReadByte();
		atkDelay	= MSG_ReadShort();
	}


	
	Fx_CHARACTER_t *character = gcTools.FindCharacter(entityType,idx);
	if (NULL == character) return 0;

	if( character->event == GTH_EV_CHAR_MOUSERUN )	return 0;
	if( character->event == GTH_EV_CHAR_SKILL )		return 0;

	character->atk_playAnim			= true;
	character->angles[ YAW ]		= angles;
	character->stat_attackSpeed		= atkDelay;
	character->event				= GTH_EV_CHAR_ATTACK;

	
	if(gcTools.IfMyCharacter(character))
	{
		
		if( damageAttackFlag )
		{
			
			if( !success )
			{
				
				g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,180), D3DCOLOR_ARGB(255, 255, 55, 55) );
			}	
			
			g_cgv.myCharacterInfo->resultHitRate			= resultHitRate;
			g_cgv.myCharacterInfo->item[itemIdx].durability = dur;
		}

		g_cgv.saveEvent			= GTH_EV_CHAR_ATTACK;
		g_cgv.saveTargetType	= character->targetType;
		g_cgv.saveTargetIdx		= character->targetIdx;
	}

	
	else
	{
		character->targetIdx	= targetIdx;
		character->targetType	= targetType;
	}

	
	
	if(character->entityType == ENTITY_MONSTER) 
	{
		int EffectID = g_SkillTransInfo.m_TransList.GetEffectID(character->tableIdx);
		if( -1 != EffectID)
		{
			g_charManager->AddEffect( FX_CCharManager::EFFECT_BASIC_ATTACK_EFFECT, EffectID, character, 0.0f );
		}
	}
	else if(character->entityType == ENTITY_PC) 
	{
		if(character->isTransform)
		{
			int EffectID = g_SkillTransInfo.m_TransList.GetEffectID(character->transformEntityIdx);
			if( -1 != EffectID)
			{
				g_charManager->AddEffect( FX_CCharManager::EFFECT_BASIC_ATTACK_EFFECT, EffectID, character, 0.0f );
			}
		}
	}	

	character->attackComboType	= attackComboType;
	g_GameTarget.GTH_SetAngleToTarget( character );
	
	return 1;
}

int CAttackProc::GTH_ProcessEventMessage_Damage()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character, *attacker;
	int entityType;
	int attackerType, attackerIdx, damage;
	int recoveryTime;
	int attackerRA, attackerSA, itemIdx, dur;
	int resultAvoidRate, stealNak = 0;
	int isStun;
	char szTemp[256], szTemp2[128];

	idx				= MSG_ReadShort();
	entityType		= MSG_ReadByte();

	character		= &g_charManager->m_Characters[0];
	numCharacter	= g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->curRA	= MSG_ReadLong();
	character->curSA	= MSG_ReadShort();
	
	
	
	damage				= MSG_ReadLong();
	if( damage < 0 )	damage = 0;

	attackerIdx			= MSG_ReadShort();		
	attackerType		= MSG_ReadByte();		

	attackerRA			= MSG_ReadLong();
	attackerSA			= MSG_ReadShort();
	
	
	
	if( attackerType == ENTITY_PC && character->entityType != ENTITY_PC )
	{
		stealNak = MSG_ReadLong();
	}

	attacker		= &g_charManager->m_Characters[0];
	numCharacter	= g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, attacker++) 
	{
		if (attackerType == attacker->entityType && attackerIdx == attacker->idxOnServer)
			break;
	}
	if( i == numCharacter ) return 0;

	






	if (i < numCharacter)
	{
		attacker->curRA = attackerRA;
		attacker->curSA = attackerSA;
	}

	
	isStun			= MSG_ReadByte();
	recoveryTime	= MSG_ReadLong();

	if( entityType == ENTITY_PC ) 
	{
		resultAvoidRate			= MSG_ReadByte();
		itemIdx = MSG_ReadShort();
		if( itemIdx >= 0 ) dur	= MSG_ReadShort();
	}	

	if( attackerType == ENTITY_PC && attackerIdx == g_cgv.myPCIdxOnServer )
	{
		
		if( isStun == 1 ) 
			g_charManager->SetTrembling( character->position, 0.1f, 0.2f, 0.3f );
		

		stealNak = stealNak - g_cgv.myCharacterInfo->curChargeSE;
		
		if( stealNak > 0 && g_pApp->m_myCharacter->isTransform )
		{
			
			strcpy( szTemp2, g_ifMng->GetNumberString( stealNak ) );
			sprintf( szTemp, g_LPACK.GetMassage( 0, 450 ), szTemp2 );
			g_ifMng->AddSysMessage( szTemp );
			g_cgv.myCharacterInfo->curChargeSE += stealNak;
		}
		if( damage > 0 ) m_ntempAttackSuccessCount ++;
		else m_ntempAttackMissCount ++;
	}

	if( isStun > 0 ) 
	{
		if( character->idxOnServer == g_cgv.myPCIdxOnServer && character->entityType == ENTITY_PC ) 
		{
			if( character->event == GTH_EV_CHAR_SIT || character->event == GTH_EV_CHAR_STAND ) 
			{
				g_cgv.myCharacterInfo->skillCastingtime = 0.0f;
				g_cgv.myCharacterInfo->skillCastingState = true;
				GTH_EnableMove();
			}
		}		

		
		
		if( isStun > 1 ) 
		{
			
			if( isStun == 3 ) 
			{
				int StunEffect=0;
				if( entityType == ENTITY_PC)
				{
					character->event = GTH_EV_CHAR_PC_STUN;
					StunEffect=82;
				}
				else
				{
					character->event = GTH_EV_CHAR_STUN;
					StunEffect=92;
				}
					
				character->stat_recoveryTime = recoveryTime;
				character->flag_replayAnim = 1;
				
				g_effectMng->Add(	GTH_EFFECT_CLASS_MODEL, StunEffect, 
									character ->position, 
									NULL, character ->angles, 
									0.0f, character ->entityType, 
									character ->idxOnServer );
			}

			
			
			else if( isStun == 4 )	character->event = GTH_EV_CHAR_FREEZE;
			else if( isStun == 5 )	character->event = GTH_EV_CHAR_PUSHED;
		}
	}

	
	
	if( character->idxOnServer == g_cgv.myPCIdxOnServer && character->entityType == ENTITY_PC )
	{
		
		if( attackerType == ENTITY_PC ) AddTargetList( attacker->name );

		g_cgv.myCharacterInfo->resultAvoidRate = resultAvoidRate;
		if( itemIdx >= 0 ) g_cgv.myCharacterInfo->item[itemIdx].durability = dur;
		if( g_pApp->m_myCharacter->event == GTH_EV_CHAR_SIT )
		{
			g_pApp->m_myCharacter->event = GTH_EV_CHAR_STAND;
		}
	}

	if( attackerIdx == g_cgv.myPCIdxOnServer && attackerType == ENTITY_PC )
	{
		strcpy( g_cgv.targetName, character->name );
	}
#pragma chMSG("이부분에서 Damage 판단 루틴...추가....")
	
	


	

	
	{
		
		
		 



		if( damage )
		{
			
			
			if	(	g_charManager->m_pAnimManager->ShowAttackEffect( attacker->ani_curIdx )	)
			{
				
				
				int EffectID = g_SkillTransInfo.m_TransList.GetEffectID(attacker->tableIdx);
				if(-1 == EffectID )
				{
					g_charManager->ShowAttackEffect ( attacker, damage );
				}
			}
		}

		g_ifMng ->m_numCtl ->AddDamageNumber ( character->idxOnServer, entityType, attackerIdx, attackerType, damage, isStun );
		g_ifMng ->m_numCtl ->ReadyDamage ( attacker );
	}

	return 1;
}