#include "../global.h"

extern i3sizebuf_t netMessage; 



#include "..\GolryunBattle\CGolryunBattle_Observer.h"
extern CGolryunBattle_Observer* gcpGolryunBattle_Observer;

int GTH_SendPCEventMessage_Idle( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1)	return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_IDLE);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_Walk( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_WALK);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteFloat(&netMessage, pc->angles[1]);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_WalkLeft( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_WALKLEFT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_WalkRight( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_WALKRIGHT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_Back( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_BACK);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_BackLeft( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_BACKLEFT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_BackRight( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, GSC_EVENT_BACKRIGHT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);
		
	return true;
}

int GTH_SendPCEventMessage_Run( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_RUN);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteFloat(&netMessage, pc->angles[1]);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_RunLeft( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_RUNLEFT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_RunRight( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_RUNRIGHT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_TurnLeft( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_TURNLEFT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_TurnRight( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_TURNRIGHT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_MoveLeft( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_MOVELEFT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_MoveRight( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_MOVERIGHT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_Attack( playerCharacter_t *pc, int damageAttackFlag, int success, int itemIdx )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_ATTACK);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		MSG_WriteFloat(&netMessage, pc->angles[ YAW ] );
		
		
		MSG_WriteByte(&netMessage, damageAttackFlag );
		
		MSG_WriteByte(&netMessage, success );
		
		MSG_WriteShort(&netMessage, pc->targetIdx);
		MSG_WriteByte(&netMessage, pc->targetType);
		
		MSG_WriteShort(&netMessage, pc->calAttackDelay );
		MSG_WriteByte(&netMessage, pc->resultHitRate );
		MSG_WriteByte(&netMessage, pc->attackComboType );
		MSG_WriteShort(&netMessage, itemIdx );
		
		if( itemIdx >= 0 ) MSG_WriteShort(&netMessage, pc->item[itemIdx].durability );
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_SkillCasting( playerCharacter_t *pc, int castingTime,int skillIdx,int skillTargetType,int UseType )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_SKILLCASTING);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteShort(&netMessage, (int)pc->curSA);
		MSG_WriteLong(&netMessage, castingTime );

		
		MSG_WriteShort(&netMessage, skillIdx);
		MSG_WriteShort(&netMessage, skillTargetType);
		

		
		MSG_WriteLong(&netMessage, pc->generalSpeed);
	
		
		MSG_WriteByte(&netMessage, UseType);

		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			if( pc->idx == idx ) NET_SendMessage( &g_pc[idx].sock, &netMessage );
			else NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);
	MSG_EndWriting(&netMessage);

	return true;
}



int	GTH_SendPCEventMessage_Skill( playerCharacter_t *pc, int skillIdx, int coolTime, int delayTime )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_SKILL);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		MSG_WritePosition(&netMessage, pc->position );
		MSG_WriteFloat(&netMessage, pc->angles[ YAW ] );
		
		
		if( pc->bNotOwnSkill )
		{
			MSG_WriteShort(&netMessage, skillIdx );
		}
		else
		{
			MSG_WriteShort(&netMessage, pc->skill[skillIdx].tableIdx );
		}
		MSG_WriteByte(&netMessage, 0 );
		
		MSG_WriteLong(&netMessage, (int)pc->curRA);
		MSG_WriteShort(&netMessage, (int)pc->curSA);
		
		MSG_WriteShort(&netMessage, pc->targetIdx);
		MSG_WriteByte(&netMessage, pc->targetType);
		
		
		
		if( pc->bNotOwnSkill ) MSG_WriteChar(&netMessage, -1);
		else MSG_WriteChar(&netMessage, skillIdx);
		
		MSG_WriteLong(&netMessage, coolTime);
		MSG_WriteLong(&netMessage, delayTime);
		
		MSG_WriteByte(&netMessage, pc->resultValid );
		
		MSG_WriteByte(&netMessage, g_skillMultiTarget.targetNumber );
		for( i = 0; i < g_skillMultiTarget.targetNumber; i ++ )
		{
			MSG_WriteByte( &netMessage, g_skillMultiTarget.targetType[i] );
			MSG_WriteShort( &netMessage, g_skillMultiTarget.targetIdx[i] );
		}
		
		MSG_WriteByte(&netMessage, pc->resultHitRate);
		MSG_WriteShort(&netMessage, pc->recoveryTime);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			if( pc->idx == idx ) NET_SendMessage( &g_pc[idx].sock, &netMessage );
			else NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_Die( playerCharacter_t *pc, int dropItemType, int dropItemPosition )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_DIE);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		MSG_WriteShort(&netMessage, pc->curDamage);
		MSG_WriteByte(&netMessage, pc->lastAttackState);
		
		MSG_WriteShort(&netMessage, pc->killerIdx);
		MSG_WriteByte(&netMessage, pc->killerType);
		MSG_Write64Int(&netMessage, pc->exp );

		
		MSG_WriteLong(&netMessage, pc->curChargeSE);
		
		
		MSG_WriteShort(&netMessage, dropItemType);
		MSG_WriteShort(&netMessage, dropItemPosition);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			
			if( pc->idx == idx ) NET_SendMessage(&g_pc[idx].sock, &netMessage);
			else NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		}
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}


int GTH_SendPCEventMessage_Damage( playerCharacter_t *pc, int itemIdx, int stunFlag )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_DAMAGE);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteLong(&netMessage, (int)pc->curRA);			
		MSG_WriteShort(&netMessage, (int)pc->curSA);			
		
		
		
		MSG_WriteLong(&netMessage, pc->curDamage);	
		
		MSG_WriteShort(&netMessage, pc->damagedEntityIdx );	
		MSG_WriteByte(&netMessage, pc->damagedEntityType );	
		
		if( pc->damagedEntityType == ENTITY_PC )
		{
			MSG_WriteLong(&netMessage, (int)g_pc[pc->damagedEntityIdx].curRA);	
			MSG_WriteShort(&netMessage, (int)g_pc[pc->damagedEntityIdx].curSA);	
		}
		else
		{
			MSG_WriteLong(&netMessage, (int)g_monster[pc->damagedEntityIdx].curRA);	
			MSG_WriteShort(&netMessage, (int)g_monster[pc->damagedEntityIdx].curSA);	
		}
		
		MSG_WriteByte(&netMessage, stunFlag );
		
		MSG_WriteShort(&netMessage, pc->recoveryTime );	
		
		MSG_WriteByte(&netMessage, pc->resultAvoidRate );
		
		MSG_WriteShort(&netMessage, itemIdx );
		if( itemIdx >= 0 ) MSG_WriteShort(&netMessage, pc->item[itemIdx].durability );
		
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);
	MSG_EndWriting(&netMessage);
	return true;
}

int GTH_SendPCEventMessage_Respawn( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_RESPAWN);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteByte(&netMessage, pc->worldIdx);
		MSG_WritePosition(&netMessage, pc->position);
		MSG_WriteFloat(&netMessage, pc->angles[1]);
		MSG_WriteLong(&netMessage, (int)pc->curRA);
		MSG_WriteShort(&netMessage, (int)pc->curSA);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			if( g_pc[idx].idx == pc->idx ) 
			{
				NET_SendMessage(&g_pc[idx].sock, &netMessage);
			}
			else
			{
				NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
			}
		}
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);
	MSG_EndWriting(&netMessage);

	return true;
}


int GTH_SendPCEventMessage_Teleport( playerCharacter_t *pc, vec3_t	movePos )
{
	int i;
	int idx;
	int zoneIdx;

	float	dist;
	vec3_t	monsterPos;

	









	vec_copy( g_monster[pc->targetIdx].position, monsterPos );

	dist = GTH_GetDistance2D( pc->position, monsterPos );

	if( dist < 50.0f )		return false;

	

	int localWorldIdx = GTH_GetLocalWorldIdx( g_curPC->worldIdx );

	if( FindMonsterNearPlace(localWorldIdx, movePos) != -1 )
	{
		vec_copy( movePos, pc->position );
	}
	else
	{
		

	}

	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	pc->zoneIdx = zoneIdx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1)
	{

		return false;
	}

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_TELEPORT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteByte(&netMessage, pc->worldIdx);
		
		MSG_WritePosition(&netMessage, movePos);
		
		MSG_WriteFloat(&netMessage, pc->angles[1]);
		MSG_WriteLong(&netMessage, (int)pc->curRA);
		MSG_WriteShort(&netMessage, (int)pc->curSA);
		
		static int sendCount;
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			if( g_pc[idx].idx == pc->idx )
			{
				NET_SendMessage(&g_pc[idx].sock, &netMessage);
				
				
				sendCount++;

			}
			else
			{
				NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
			}
		}
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	

	MSG_EndWriting(&netMessage);

	return true;
}


int GTH_SendPCEventMessage_Before_Teleport( playerCharacter_t *pc )
{
	int i;
	int idx;




	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_BEFORE_TELEPORT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteByte(&netMessage, pc->worldIdx);
		MSG_WriteShort(&netMessage, pc->curSkillIdx);
		MSG_WriteShort(&netMessage, pc->pcJob);
		MSG_WritePosition(&netMessage, pc->position);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			if( g_pc[idx].idx == pc->idx )
			{
				NET_SendMessage(&g_pc[idx].sock, &netMessage);
			}
			else
			{
				NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
			}
		}
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	
	MSG_EndWriting(&netMessage);

	return true;
}


int GTH_SendPCEventMessage_MouseWalk( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_MOUSEWALK);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteFloat(&netMessage, pc->angles[YAW]);	
		MSG_WritePosition(&netMessage, pc->mousePos);	
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_MouseRun( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;
	

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_MOUSERUN);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WriteFloat(&netMessage, pc->angles[YAW]);	
		MSG_WritePosition(&netMessage, pc->mousePos);	
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if (pc->idx == idx) continue; 
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_Sit( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_SIT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendPCEventMessage_Stand( playerCharacter_t *pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_STAND);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int	GTH_SendPCEventMessage_Shout( playerCharacter_t* pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_SUB_EVENT_SOCIAL_SHOUT);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}

int	GTH_SendPCEventMessage_Bow( playerCharacter_t* pc )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_SUB_EVENT_SOCIAL_BOW);
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);

	return true;
}


int GTH_SendPCMessage_TransportReply(playerCharacter_t* pToPlayerRecord,const tagGCPacket_TRANSPORTWORLDREP::enumCode code)
{
	
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_TRANSPORTWORLDREP);
		MSG_WriteByte(&netMessage, code);		
		MSG_WriteByte(&netMessage, pToPlayerRecord->worldIdx);
		MSG_WritePosition(&netMessage, pToPlayerRecord->position);
		MSG_WriteFloat(&netMessage, pToPlayerRecord->angles[1]);		
		
		NET_SendMessage(&(pToPlayerRecord->sock), &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return true;
}





void GTH_SendPCMessage_ChangeServer( playerCharacter_t *pc, memberServer_t *member )
{
	
	
	
	
	
	
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_CHANGESERVER);
		MSG_WriteString(&netMessage, member->serviceIP);
		MSG_WriteShort(&netMessage, member->servicePort);
		NET_SendUnreliableMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendPCMessage_ChangeWorld( playerCharacter_t *pc )
{
	
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_CHANGEWORLD);
		MSG_WriteByte(&netMessage, pc->worldIdx);
		MSG_WritePosition(&netMessage, pc->position);
		MSG_WriteFloat(&netMessage, pc->angles[1]);
		
		NET_SendUnreliableMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendPCMessage_ReplyWeaponChange( playerCharacter_t* pc )
{
	
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_WEAPONCHANGE);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}



void GTH_SendPCMessage_LevelUp( playerCharacter_t* pc, __int64 exp, __int64 genExp, int lootNSE, int genCapability, int skillIdx[], int skillNumber, int sharingType )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return;
	
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_LEVELUP);
		
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		
		
		MSG_Write64Int(&netMessage, exp);
		MSG_Write64Int(&netMessage, genExp);
		MSG_WriteLong(&netMessage, lootNSE);
		MSG_WriteLong(&netMessage, genCapability);
		
		MSG_WriteShort(&netMessage, pc->level );
		MSG_WriteShort(&netMessage, pc->genLevel );
		MSG_WriteShort(&netMessage, pc->bonusStatusPoint );
		MSG_WriteShort(&netMessage, pc->force );
		MSG_WriteShort(&netMessage, pc->soulpower );
		MSG_WriteShort(&netMessage, pc->agility );
		MSG_WriteShort(&netMessage, pc->vitality );
		
		pc->curRA = pc->calMaxRA;
		pc->curSA = pc->calMaxSA;
		
		MSG_WriteLong(&netMessage, (int)pc->curRA );
		MSG_WriteShort(&netMessage, (int)pc->curSA );
		
		
		MSG_WriteByte(&netMessage, pc->bonusSkillPoint );
		
		
		
		
		
		MSG_WriteByte(&netMessage, sharingType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if ( !g_pc[idx].ready ) continue;
			if( pc->idx == idx ) NET_SendMessage(&pc->sock, &netMessage);	
			else NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);	
		}
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);
}

void GTH_SendPCMessage_ReplyGenChange( playerCharacter_t* pc)
{
	
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_GENCHANGE );
		MSG_WriteByte(&netMessage, pc->selectedGen );
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendPCMessage_ReplyStatusUp( playerCharacter_t* pc, int reply, int error, int statusType )
{
	int point, bonusPoint;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_STATUSUP );
		MSG_WriteByte(&netMessage, reply );
		
		if( reply )
		{
			switch( statusType )
			{
			case 0 :
				point = pc->force;
				break;
			case 1 :
				point = pc->soulpower;
				break;
			case 2 :
				point = pc->agility;
				break;
			case 3 :
				point = pc->vitality;
				break;
			}
			bonusPoint = pc->bonusStatusPoint;
			MSG_WriteShort(&netMessage, statusType);
			MSG_WriteShort(&netMessage, point);
			MSG_WriteShort(&netMessage, bonusPoint);
			MSG_WriteLong(&netMessage, (int)pc->curRA);
			MSG_WriteShort(&netMessage, (int)pc->curSA);
		}
		else
		{
			MSG_WriteByte(&netMessage, error );
		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendPCMessage_AutoRecovery( playerCharacter_t* pc )
{
	
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_AUTORECOVERY );
		MSG_WriteLong(&netMessage, (int)pc->curRA );
		MSG_WriteShort(&netMessage, (int)pc->curSA );
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendPCMessage_ReplyEpackUse( playerCharacter_t* pc, int type )
{
	
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_EPACKUSE );
		
		MSG_WriteByte(&netMessage, type );
		
		switch( type )
		{
		case 0 :
			MSG_WriteLong(&netMessage, (int)pc->curRA );
			MSG_WriteShort(&netMessage, pc->epackUseRAWaitTime - g_globalTime );

			
			MSG_WriteShort(&netMessage, (int)pc->curChargeRA );
			break;

		case 1 :
			MSG_WriteLong(&netMessage, (int)pc->curSA );
			MSG_WriteShort(&netMessage, pc->epackUseSAWaitTime - g_globalTime );

			
			MSG_WriteShort(&netMessage, (int)pc->curChargeSA );
			break;
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendPCMessage_Affect( playerCharacter_t *pc )
{
	int i, numAffect, affectData[MAX_AFFECT], affectTime[MAX_AFFECT];
	int idx;

	
	GTH_GetPCListInValidRange(pc->worldIdx, pc->position);
	if (g_zonePCListNumber < 1) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_AFFECT );
		
		MSG_WriteShort(&netMessage, pc->idx);
		MSG_WriteByte(&netMessage, pc->entityType);
		MSG_WritePosition(&netMessage, pc->position);
		MSG_WriteFloat(&netMessage, pc->calVelocity );
		
		numAffect = GTH_MakeAffectTableIdx( pc, affectData, ENTITY_PC );
		numAffect = GTH_MakeAffectTime( pc, affectTime, ENTITY_PC );
		MSG_WriteByte(&netMessage, numAffect);
		for( i = 0; i < numAffect; i ++ ) 
		{
			MSG_WriteShort(&netMessage, affectData[i] );
			MSG_WriteLong(&netMessage, affectTime[i] );
		}
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);	
		}
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pc);	
	MSG_EndWriting(&netMessage);
}

void GTH_SendMonsterMessage_Affect( monsterCharacter_t *mon )
{
	int i, numAffect, affectData[MAX_AFFECT], affectTime[MAX_AFFECT];
	int idx;

	
	GTH_GetPCListInValidRange(mon->worldIdx, mon->position);
	if (g_zonePCListNumber < 1) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_AFFECT );
		
		MSG_WriteShort(&netMessage, mon->idx);
		MSG_WriteByte(&netMessage, mon->entityType);
		MSG_WritePosition(&netMessage, mon->position);
		MSG_WriteFloat(&netMessage, mon->calVelocity );
		
		numAffect = GTH_MakeAffectTableIdx( mon, affectData, mon->entityType );
		numAffect = GTH_MakeAffectTime( mon, affectTime, ENTITY_MONSTER );
		MSG_WriteByte(&netMessage, numAffect);
		for( i = 0; i < numAffect; i ++ )
		{
			MSG_WriteShort(&netMessage, affectData[i] );
			MSG_WriteLong(&netMessage, affectTime[i] );
		}
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);	
		}
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(mon);	
	MSG_EndWriting(&netMessage);
}



int GTH_SendMonsterEventMessage_SkillCasting( monsterCharacter_t *pMonster, int castingTime )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(pMonster->worldIdx, pMonster->position);
	if (g_zonePCListNumber < 1) return false;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_MONSTER_SKILLCASTING);
		MSG_WriteShort(&netMessage, pMonster->idx);
		MSG_WriteByte(&netMessage, pMonster->entityType);
		MSG_WriteLong(&netMessage, pMonster->saveSkillIdx);
		MSG_WriteShort(&netMessage, (int)pMonster->curSA);
		MSG_WriteLong(&netMessage, castingTime );
		
		
		for(i = 0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);	
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(pMonster);	
	MSG_EndWriting(&netMessage);

	return true;
}



int	GTH_SendMonsterEventMessage_Skill( monsterCharacter_t *monster, int skillIdx, int coolTime, int delayTime )
{
	int i;
	int idx;

	



	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_MONSTER_SKILL);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		MSG_WritePosition(&netMessage, monster->position );
		MSG_WriteFloat(&netMessage, monster->angles[ YAW ] );
		MSG_WriteShort(&netMessage, monster->skill[skillIdx].tableIdx );
		
		
		
		MSG_WriteLong(&netMessage, monster->skill[skillIdx].iLevel );
		
		
		MSG_WriteLong(&netMessage, (int)monster->curRA);
		MSG_WriteShort(&netMessage, (int)monster->curSA);
		
		MSG_WriteShort(&netMessage, monster->targetIdx);
		MSG_WriteByte(&netMessage, monster->targetType);
		
		MSG_WriteByte(&netMessage, skillIdx);
		MSG_WriteLong(&netMessage, coolTime);
		MSG_WriteLong(&netMessage, delayTime);
		
		MSG_WriteByte(&netMessage, 1 );	
		
		MSG_WriteByte(&netMessage, g_skillMultiTarget.targetNumber );
		for( i = 0; i < g_skillMultiTarget.targetNumber; i ++ )
		{
			MSG_WriteByte( &netMessage, g_skillMultiTarget.targetType[i] );
			MSG_WriteShort( &netMessage, g_skillMultiTarget.targetIdx[i] );
		}
		
		MSG_WriteByte(&netMessage, 1);						
		MSG_WriteShort(&netMessage, monster->recoveryTime);	
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			if( monster->targetIdx == idx )
			{
				NET_SendMessage( &g_pc[idx].sock, &netMessage );
			}
			else
			{
				NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
			}
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);	
	MSG_EndWriting(&netMessage);

	return true;
}




void GTH_SendMessage_SkillError( playerCharacter_t* pc , int errorIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_EXTEND_EVENT_SKILL_ERROR);
		MSG_WriteByte(&netMessage, errorIdx);
	
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}