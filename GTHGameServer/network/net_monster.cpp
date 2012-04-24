#include "../global.h"



extern i3sizebuf_t netMessage; 

#include "..\GolryunBattle\CGolryunBattle_Observer.h"
extern CGolryunBattle_Observer* gcpGolryunBattle_Observer;

int GTH_SendMonsterEventMessage_Idle( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1)	return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_IDLE);
		
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_Walk( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_WALK);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		MSG_WriteFloat(&netMessage, monster->angles[1]);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_Back( monsterCharacter_t *monster)
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_BACK);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		}
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_Run( monsterCharacter_t *monster)
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_RUN);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		MSG_WriteFloat(&netMessage, monster->angles[1]);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_TurnLeft( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_TURNLEFT);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_TurnRight( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_TURNRIGHT);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_MoveLeft( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_MOVERIGHT);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_MoveRight( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_MOVERIGHT);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_Attack( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_ATTACK);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		MSG_WriteFloat(&netMessage, monster->angles[ YAW ] );
		MSG_WriteShort(&netMessage, monster->targetIdx);
		MSG_WriteByte(&netMessage, monster->targetType);
		
		MSG_WriteShort(&netMessage, monster->calAttackDelay);
		
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_Damage( monsterCharacter_t *monster, int stunFlag )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_DAMAGE);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		MSG_WriteLong(&netMessage, (int)monster->curRA);	
		MSG_WriteShort(&netMessage, (int)monster->curSA);	
				
		
		
		MSG_WriteLong(&netMessage, monster->curDamage );	
		
		MSG_WriteShort(&netMessage, monster->damagedEntityIdx );	
		MSG_WriteByte(&netMessage, monster->damagedEntityType );	
		
		if( monster->damagedEntityType == ENTITY_PC )
		{
			MSG_WriteLong(&netMessage, (int)g_pc[monster->damagedEntityIdx].curRA);	
			MSG_WriteShort(&netMessage, (int)g_pc[monster->damagedEntityIdx].curSA);	
			MSG_WriteLong( &netMessage, g_pc[monster->damagedEntityIdx].curChargeSE );
		}
		else
		{
			MSG_WriteLong(&netMessage, (int)g_monster[monster->damagedEntityIdx].curRA);	
			MSG_WriteShort(&netMessage, (int)g_monster[monster->damagedEntityIdx].curSA);	
		}
		
		MSG_WriteByte(&netMessage, stunFlag );
		
		MSG_WriteShort(&netMessage, monster->recoveryTime );
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}

int GTH_SendMonsterEventMessage_Die( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_DIE);
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		
		MSG_WriteShort(&netMessage, monster->curDamage );	
		MSG_WriteByte(&netMessage, monster->lastAttackState);
		
		MSG_WriteShort(&netMessage, monster->killerIdx );	
		MSG_WriteByte(&netMessage, monster->killerType );	
		
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		}
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}



int GTH_SendMonsterEventMessage_Skill( monsterCharacter_t *monster )
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		
		MSG_WriteByte(&netMessage, GSC_EVENT_ATTACK);
		
		MSG_WriteShort(&netMessage, monster->idx);
		MSG_WriteByte(&netMessage, monster->entityType);
		MSG_WriteFloat(&netMessage, monster->angles[ YAW ] );
		MSG_WriteShort(&netMessage, monster->targetIdx);
		MSG_WriteByte(&netMessage, monster->targetType);
		
		
		MSG_WriteShort(&netMessage, monster->calAttackDelay);
		
		
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}

	gcpGolryunBattle_Observer->NET_SendUnreliableMessage(monster);
	MSG_EndWriting(&netMessage);

	return true;
}



int GTH_SendMonsterEventMessage_State( monsterCharacter_t *monster )
{

	return true;
}

