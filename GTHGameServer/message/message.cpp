
#pragma warning(disable:4786)
#include "../global.h"


#define MAX_RANGE_ATTACK_TARGET		5


int AI_SendMessageToPC( playerCharacter_t* pc, message_t* message )
{
	if( pc->messageNumber >= MAX_NUMBER_OF_MESSAGE ) return 0;
	if( pc->messageNumber < 0 ) pc->messageNumber = 0;
	if( pc->worldIdx != message->worldIdx ) return 0;
	
	if( message->fromType == ENTITY_PC && message->type == MSG_DAMEGED )
	{
		
		
		
		if( !( g_pc[message->fromIdx].pvpMode && pc->pvpMode ) )
		{
			if( !InTargetList( &g_pc[message->fromIdx], pc->name ) ) return 0;
		}
		
		if( !PC_IsValidAttack( &g_pc[message->fromIdx], pc->entityType, pc->idx ) )
		{
			g_pc[message->fromIdx].resultValid = false;
			return 0;
		}
	}

	int n = pc->messageNumber;
	memcpy( &pc->message[n], message, sizeof( message_t ) );

	pc->message[n].toType = ENTITY_PC;
	pc->message[n].toIdx = pc->idx;
	strncpy(pc->message[n].toNameV2,pc->name,NAMESTRING);
	pc->message[n].toNameV2[NAMESTRING]=NULL;

	pc->messageNumber ++;

	return 1;
}


int AI_SendMessageToMONSTER( monsterCharacter_t* monster, message_t* message )
{
	if( monster->messageNumber >= MAX_NUMBER_OF_MESSAGE ) return 0;
	if( monster->messageNumber < 0 ) monster->messageNumber = 0;
	if( monster->worldIdx != message->worldIdx ) return 0;

	if( message->fromType == ENTITY_PC && message->type == MSG_DAMEGED )
	{
		if( monster->aliveState != MON_ALIVE_STATE_NORMAL ) return 0;
		
		if( !PC_IsValidAttack( &g_pc[message->fromIdx], monster->entityType, monster->idx ) )
		{
			g_pc[message->fromIdx].resultValid = false;
			return 0;
		}
	}

	int n = monster->messageNumber;
	memcpy( &monster->message[n], message, sizeof( message_t ) );
	monster->message[n].toType = ENTITY_MONSTER;
	monster->message[n].toIdx = monster->idx;
	
	strncpy( monster->message[n].toNameV2, monster->basic.name,NAMESTRING);
	monster->message[n].toNameV2[NAMESTRING]=NULL;
	monster->messageNumber ++;

	return 1;
}



#define MAX_ATTACK_NUMBER 32


void AI_SendMessage( message_t* message, CAISendMessage *pAIMsg)
{
	int i, worldIdx, n = 0;
	int idx, idxBuffer[MAX_ATTACK_NUMBER];	
	int exceptType = -1, exceptIdx = -1;
	vec3_t position;

	
	for(int k=0;k<MAX_ATTACK_NUMBER;k++)
	{
		idxBuffer[k]=-1;
	}

	CAISendMessage AIMsg;
	if(pAIMsg)
	{
		AIMsg = *pAIMsg;
	}

	if( message->fromType == ENTITY_PC )
	{
		
		if( !g_pc[message->fromIdx].active) return;
		if( !g_pc[message->fromIdx].alive )
		{
			
			if(!g_SkillItem.IsType_UseSkillID(message->data[MSG_SKILL_TABLE_INDEX],CSkillItem::ITEM_TYPE_ENABLE_DEAD_SELF_ITEM)) return;
		}

		
		g_pc[message->fromIdx].resultValid = true;
	}

	if( AIMsg.range == 0 )
	{
		switch( message->toType )
		{
		case ENTITY_PC :
			{
				if( g_pc[message->toIdx].active && g_pc[message->toIdx].ready ) 
				{
					AI_SendMessageToPC( &g_pc[message->toIdx], message );
				}
			}
			break;
		case ENTITY_MONSTER : case ENTITY_NPC :
			{
				if( g_monster[message->toIdx].active ) 
				{
					AI_SendMessageToMONSTER( &g_monster[message->toIdx], message );
				}
			}
			break;
		}

		return;
	}
	
	
	switch( AIMsg.rangeType )
	{
	case MSG_RANGE_TYPE_TARGET : 
	case MSG_RANGE_TYPE_EXCEPT_TARGET :
	case MSG_RANGE_TYPE_EXCEPT_PARTY:
		{
			switch( message->toType )
			{
			case ENTITY_PC :
				{
					worldIdx = g_pc[message->toIdx].worldIdx;
					vec_copy( g_pc[message->toIdx].position, position );
				}
				break;
			case ENTITY_MONSTER : case ENTITY_NPC :
				{
					worldIdx = g_monster[message->toIdx].worldIdx;
					vec_copy( g_monster[message->toIdx].position, position );
				}
				break;
			default :
				return;
				break;
			}
		}
		break;
	case MSG_RANGE_TYPE_MYSELF :
		{
			switch( message->fromType )
			{
			case ENTITY_PC :
				{
					worldIdx = g_pc[message->fromIdx].worldIdx;
					vec_copy( g_pc[message->fromIdx].position, position );
				}
				break;
			case ENTITY_MONSTER : case ENTITY_NPC :
				{
					worldIdx = g_monster[message->fromIdx].worldIdx;
					vec_copy( g_monster[message->fromIdx].position, position );
				}
				break;
			default :
				return;
				break;
			}
		}
		break;
	}

	if( AIMsg.basePosition != NULL &&
		AIMsg.rangeType != MSG_RANGE_TYPE_MYSELF &&
		AIMsg.rangeType != MSG_RANGE_TYPE_TARGET &&
		message->data[MSG_ATKDATA_ETC_INFO] != MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER &&
		message->data[MSG_ATKDATA_ETC_INFO] != MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_100_ELSE_LITE_DAMAGE)
	{
		vec_copy( AIMsg.basePosition, position );
	}
	
	
	if( AIMsg.rangeType == MSG_RANGE_TYPE_EXCEPT_TARGET )
	{
		exceptType = message->toType;
		exceptIdx = message->toIdx;
	}

	switch( message->toType )
	{
	case ENTITY_PC :
		{
			GTH_GetPCListInValidRange( worldIdx, position );
			if (g_zonePCListNumber < 1) return;
			
			for( i = 0; i < g_zonePCListNumber; i ++ )
			{
				
				
				if ( (int)g_zonePCList[i].distance < AIMsg.range || AIMsg.range == SENDALL_RANGE )
				{
					idx = g_zonePCList[i].idx;
					if( exceptType == ENTITY_PC && exceptIdx == idx ) continue;
					
					
					
					if( message->fromType == ENTITY_PC && message->type == MSG_DAMEGED)
					{
						if(idx ==message->fromIdx) continue;

						
						if(message->fromType == MSG_RANGE_TYPE_EXCEPT_PARTY)
						{
							if(g_SkillPartyProc.IfPartyMember(message->fromIdx, idx))
							{
								 continue;
							}
						}
					}

					if ( g_pc[idx].alive && g_pc[idx].ready )
					{
						
						
						if( message->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER )
						{
							idxBuffer[n] = idx;
							n ++;
							
							if( n >= MAX_ATTACK_NUMBER ) break;
						}
						else
						{
							if( AI_SendMessageToPC( &g_pc[idx], message ) ) n ++;
						}
					}		
				}
				
				if( n > AIMsg.maxMsgNumber && AIMsg.maxMsgNumber > 0 ) break;
			}
			if( message->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER && n > 0 )
			{
				message->data[MSG_ATKDATA_PHYSIC_DMG]	/=	n;
				for( i = 0; i < n; i ++ )
				{
					AI_SendMessageToPC( &g_pc[idxBuffer[i]], message );
				}
			}
		}
		break;

	case ENTITY_MONSTER :
		{
			GTH_GetMonsterListInValidRange( worldIdx, position);
			if (g_zoneMonsterListNumber < 1) return;
			
			for( i = 0; i < g_zoneMonsterListNumber; i ++ )
			{
				if ( (int)g_zoneMonsterList[i].distance < AIMsg.range || AIMsg.range == SENDALL_RANGE )
				{
					idx = g_zoneMonsterList[i].idx;
					if( exceptType == ENTITY_MONSTER && exceptIdx == idx ) continue;
					
					
					if( message->fromType == ENTITY_MONSTER && idx == message->fromIdx )
					{
						if( message->type == MSG_DAMEGED ) continue;
					}
					
					
					if( g_monster[idx].active &&  g_monster[idx].aliveState == MON_ALIVE_STATE_NORMAL) 
					{
						
						
						if( message->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER ||
							message->data[MSG_ATKDATA_ETC_INFO] ==  MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_100_ELSE_LITE_DAMAGE)
						{
							idxBuffer[n] = idx;
							n ++;
							
							if( n >= MAX_ATTACK_NUMBER ) break;
						}
						else
						{
							if( AI_SendMessageToMONSTER( &g_monster[idx], message ) ) n ++;
						}				
					}
					
					if( n > AIMsg.maxMsgNumber && AIMsg.maxMsgNumber > 0 ) break;
				}
			}


			if( message->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER && n > 0 )
			{
				message->data[MSG_ATKDATA_PHYSIC_DMG]	/=	n;
				for( i = 0; i < n; i ++ )
				{
					AI_SendMessageToMONSTER( &g_monster[idxBuffer[i]], message );
				}
			}
		
			
			else if(message->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_100_ELSE_LITE_DAMAGE && n > 0 )
			{
				float fDamageRatio =1.0f;
				CStr_Skill_Msg *msg = AIMsg.m_pSkillMsg;
				if(msg != NULL)
				{
					fDamageRatio = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_DAMAGE_TARGET_100_ELSE_LITE_DAMAGE,msg);
				}

				for( i = 0; i < n; i ++ )
				{
					if(message->toIdx == idxBuffer[i])
					{
						if(SAFE_MONSTER(idxBuffer[i]))
							AI_SendMessageToMONSTER( &g_monster[idxBuffer[i]], message );						
					}
					else
					{
						message->data[MSG_ATKDATA_PHYSIC_DMG] *= fDamageRatio;

						if(SAFE_MONSTER(idxBuffer[i]))
							AI_SendMessageToMONSTER( &g_monster[idxBuffer[i]], message );	
					}
				}
			}
		}
		break;

	case ENTITY_NPC :
		{
			GTH_GetNPCListInValidRange( worldIdx, position);
			if (g_zoneNPCListNumber < 1) return;
			
			for( i = 0; i < g_zoneNPCListNumber; i ++ )
			{
				if ( (int)g_zoneNPCList[i].distance < AIMsg.range || AIMsg.range == SENDALL_RANGE )
				{
					idx = g_zoneNPCList[i].idx;
					if( exceptType == ENTITY_NPC && exceptIdx == idx ) continue;

					
					
					if( message->fromType == ENTITY_NPC && idx == message->fromIdx )
					{
						if( message->type == MSG_DAMEGED ) continue;
					}

					if( g_monster[idx].active ) 
					{
						
						
						if( message->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER )
						{
							idxBuffer[n] = idx;
							n ++;
							
							if( n >= MAX_ATTACK_NUMBER ) break;
						}
						else
						{
							if( AI_SendMessageToMONSTER( &g_monster[idx], message ) ) n ++;
						}
					}
					
					if( n > AIMsg.maxMsgNumber && AIMsg.maxMsgNumber > 0 ) break;
				}
			}
			if( message->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER && n > 0 )
			{
				message->data[MSG_ATKDATA_PHYSIC_DMG]	/=	n;
				for( i = 0; i < n; i ++ )
				{
					AI_SendMessageToMONSTER( &g_monster[idxBuffer[i]], message );
				}
			}
		}
		break;
	}

	return;
}


void AI_ReceiveMessage( monsterCharacter_t* monster )
{
	int i = 0, n;
	n = monster->messageNumber;
	while( i < n )
	{
		if( monster->message[i].receiveTime <= g_globalTime ) 
		{
			if( monster->message[i].worldIdx == monster->worldIdx)
				
			{
				if( !AI_InterpretMessage( monster, &monster->message[i] ) )
				{
					AI_InitMessage( monster );
					return;
				}
			}
			
			if( monster->message[i].repeatNum <= 0 )
			{
				
				
				if( i < n - 1 )
				{
					memcpy( &monster->message[i], &monster->message[n - 1], sizeof( message_t ) );
				}
				n --;
				monster->messageNumber --;
			}
		}
		else i ++;
	}
}


void AI_ReceiveMessage( playerCharacter_t* pc )
{
	int i = 0, n;
	n = pc->messageNumber;
	while( i < n )
	{
		if( pc->message[i].receiveTime <= g_globalTime ) 
		{
			if( pc->message[i].worldIdx == pc->worldIdx && !stricmp( pc->name, pc->message[i].toNameV2 ) )
			{
				if( !AI_InterpretMessage( pc, &pc->message[i] ) )
				{
					AI_InitMessage( pc );
					return;
				}
			}
			if( pc->message[i].repeatNum <= 0 )
			{
				
				
				if( i < n - 1 )
				{
					memcpy( &pc->message[i], &pc->message[n - 1], sizeof( message_t ) );
				}
				n --;
				pc->messageNumber --;
			}
		}
		else i ++;
	}
}


void AI_InitMessage( playerCharacter_t* pc )
{
	int i;
	for( i = 0; i < MAX_NUMBER_OF_MESSAGE; i ++ )
		memset( &pc->message[i], 0, sizeof( message_t ) );

	pc->messageNumber = 0;

	
    
	

	
	
	if ( pc->worldIdx != DAN_BATTLEMAP_NO )			
		GTH_DeleteAffectAll( pc, ENTITY_PC );
	else
	{
		
		if ( g_pDanBattleManager != NULL) 
		{
			if ( g_pDanBattleManager->GetStatus() == DBTL_STATUS_EVENT )
			{
				GTH_DeleteAffectAll( pc, ENTITY_PC );
			}
		}
	}
			
			
		

	for( i = 0; i < MAX_COUNT_TRAIN_MONSTER; i ++ )
	{
		pc->trainMonsterIdx[i] = -1;
		pc->trainMonsterTime[i] = 0;
	}
	pc->numTrainMonster = 0;
}

void AI_InitMessage( monsterCharacter_t* monster )
{
	int i;
	for( i = 0; i < MAX_NUMBER_OF_MESSAGE; i ++ )
		memset( &monster->message[i], 0, sizeof( message_t ) );

	monster->messageNumber = 0;

	
	for ( i = 0; i < MAX_AFFECT; i++ )
	{
		memset( &monster->affect[i], 0, sizeof( affect_t ) );
		monster->affect[i].type = AFFECT_NONE;
	}
	monster->affectNumber = 0;
}
