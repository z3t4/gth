#include "../global_def.h"
#include "../game/GolryunBattle/GolryunBattle.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 






int	GTH_SendEventMessage_Idle()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_IDLE);
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_Walk()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_WALK);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);   
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_WalkLeft()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_WALKLEFT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_WalkRight()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_WALKRIGHT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_Back()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_BACK);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_BackLeft()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_BACKLEFT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_BackRight()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_BACKRIGHT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_Run()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_RUN);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_RunLeft()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_RUNLEFT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_RunRight()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_RUNRIGHT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_TurnLeft()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_TURNLEFT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_TurnRight()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_TURNRIGHT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_MoveLeft()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_MOVELEFT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_MoveRight()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_MOVERIGHT);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

void GTH_SendEventMessage_Attack()	
{
	g_AttackProc.GTH_SendEventMessage_Attack();
}

int GTH_ProcessEventMessage_Attack()
{
	return g_AttackProc.GTH_ProcessEventMessage_Attack();
}



int	GTH_SendEventMessage_Teleport()
{
	bool	bFindPos = false;
	vec3_t	movePos;

	bFindPos = g_charManager->GetPositionForTeleport(g_cgv.saveTargetIdx, g_cgv.saveTargetType, movePos);

	if( bFindPos == false )
	{
		g_cgv.sendSkillFlag			= false;
		g_cgv.resendFlag			= false;
		g_cgv.resendTargeting		= false;
		g_pApp->m_myCharacter->atk_resend	= false;

		
		g_SkillLogic.UnSelectSkill();

		g_cgv.saveEvent = GTH_EV_CHAR_IDLE;
		GTH_ChangeAnimation(g_pApp->m_myCharacter, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE);
		return 0;
	}

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_TELEPORT);
		
		MSG_WriteShort(&netMessage, g_cgv.saveSkillIdx);
		MSG_WriteByte(&netMessage, g_cgv.saveSkillTarget);
		MSG_WritePosition(&netMessage, g_cgv.saveSkillTargetPos);
		
		MSG_WriteShort(&netMessage, g_cgv.saveTargetIdx);	
		MSG_WriteByte(&netMessage, g_cgv.saveTargetType);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		MSG_WritePosition(&netMessage, movePos);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}


int	GTH_SendEventMessage_Before_Teleport()
{
	
	if( g_pApp->m_myCharacter->event == g_pApp->m_myCharacter->oldEvent )	return 0;
	if( g_pApp->m_myCharacter->event == g_cgv.saveEvent )					return 0;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_BEFORE_TELEPORT);
		
		MSG_WriteShort(&netMessage, g_cgv.saveSkillIdx);
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	g_cgv.sendSkillFlag = false;
	g_pApp->m_myCharacter->atk_resend = false;

	return 1;
}

int	GTH_SendEventMessage_Respawn()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_RESPAWN);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int	GTH_SendEventMessage_MouseWalk()
{
	vec3_t mousePos;

	GTH_GetRealMousePosition( g_pApp->m_myCharacter->mousePos, mousePos );

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_MOUSEWALK);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->mouseAngle);
		
		MSG_WritePosition(&netMessage, mousePos);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendEventMessage_MouseRun()
{
	vec3_t mousePos;

	GTH_GetRealMousePosition( g_pApp->m_myCharacter->mousePos, mousePos );

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_MOUSERUN);
		
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->mouseAngle);
		
		MSG_WritePosition(&netMessage, mousePos);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int GTH_SendMessage_RequestWeaponChange()
{

	
	
	

	
	if ( !CheckStateWeaponChange(g_pApp->m_myCharacter) )
		return 1;
	
	

	
	
	if( !g_pApp->m_myCharacter->isAlive ) return 1;
	
	if( !GTH_LockSending() ) return 0;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_WEAPONCHANGE );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_SendMessage_RequestGenChange( int gen )
{
	if( !GTH_LockSending() ) return 0;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_GENCHANGE );
		MSG_WriteByte(&netMessage, gen );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_SendMessage_RequestStatusUp( int statusType )
{
	if( !GTH_LockSending() ) return 0;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_STATUSUP );
		MSG_WriteShort(&netMessage, statusType );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_SendMessage_CheatCode( int type, int value, int value2 )
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE );
		MSG_WriteByte(&netMessage, type );
		MSG_WriteLong(&netMessage, value );
		
		MSG_WriteShort(&netMessage, value2);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
 
	return 1;
}

int GTH_SendEventMessage_Sit()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_SIT );	
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_SendEventMessage_Stand()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EVENT_STAND );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}



int	GTH_SendEventMessage_SocialAnimation(int event)
{
	byte sendEvent;

	switch(event)
	{
		case GTH_EV_CHAR_SHOUT :
			sendEvent = CC_SUB_EVENT_SOCIAL_SHOUT;
			break;
		case GTH_EV_CHAR_BOW:
			sendEvent = CC_SUB_EVENT_SOCIAL_BOW;
			break;
	}

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, sendEvent );
		NET_SendMessage(&gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );

	return 1;
}




int GTH_ProcessEventMessage_Idle()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	
	if ( ( character->event == GTH_EV_CHAR_MOUSEWALK || character->event == GTH_EV_CHAR_MOUSERUN ) && character->ren_generalSpeed > 0 )
		return 1;

	character->event = GTH_EV_CHAR_IDLE;

	return 1;
}

int GTH_ProcessEventMessage_Walk()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;
	float angle;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	angle = MSG_ReadFloat();

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_WALK;
	if ( character->entityType == ENTITY_PC ) character->angles[YAW] = angle;
	else if (character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC ) character->mouseAngle = angle;

	return 1;
}

int GTH_ProcessEventMessage_WalkLeft()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_WALKLEFT;

	return 1;
}

int GTH_ProcessEventMessage_WalkRight()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_WALKRIGHT;

	return 1;
}

int GTH_ProcessEventMessage_Back()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_BACK;

	return 1;
}


int GTH_ProcessEventMessage_BackLeft()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_BACKLEFT;

	return 1;
}

int GTH_ProcessEventMessage_BackRight()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_BACKRIGHT;

	return 1;
}

int GTH_ProcessEventMessage_Run()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;
	float angle;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	angle = MSG_ReadFloat();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_RUN;
	if ( character->entityType == ENTITY_PC )
		character->angles[YAW] = angle;
	else if (character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC )
		character->mouseAngle = angle;

	return 1;
}

int GTH_ProcessEventMessage_RunLeft()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_RUNLEFT;

	return 1;
}

int GTH_ProcessEventMessage_RunRight()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_RUNRIGHT;

	return 1;
}

int GTH_ProcessEventMessage_TurnLeft()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_TURNLEFT;

	return 1;
}

int GTH_ProcessEventMessage_TurnRight()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_TURNRIGHT;

	return 1;
}

int GTH_ProcessEventMessage_MoveLeft()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_MOVELEFT;

	return 1;
}


int GTH_ProcessEventMessage_MoveRight()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_MOVERIGHT;

	return 1;
}

int	GTH_SendEventMessage_Skill()
{
	return g_AttackProc.GTH_SendEventMessage_Skill();
}

int	GTH_ProcessEventMessage_Damage()
{
	return g_AttackProc.GTH_ProcessEventMessage_Damage();
}



int GTH_ProcessEventMessage_Die()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *defender, *attacker;
	int entityType, attackState;
	int attackerType, attackerIdx, damage;
	int dropItemType, dropItemPosition;
	__int64 exp,curChargeSE;
	
	char temp[256], szTemp2[256];

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();

	damage = MSG_ReadShort();
	attackState = MSG_ReadByte();

	attackerIdx = MSG_ReadShort();
	attackerType = MSG_ReadByte();
	
	
	
	attacker = &g_charManager->m_Characters[0];	
	numCharacter = g_charManager->m_iNumCharacters;
	for ( i = 0; i < numCharacter; i++, attacker++) 
	{
		if (attackerType == attacker->entityType && attackerIdx == attacker->idxOnServer)
			break;
	}

	
	if (i < numCharacter)
	{
		
		attacker->atk_playAnim = false;		
	}
	else
	{
		attacker = NULL;
	}
	

	if( entityType == ENTITY_PC )
	{
		exp = MSG_Read64Int();
		
		
		curChargeSE = MSG_ReadLong();

		dropItemType = MSG_ReadShort();
		dropItemPosition = MSG_ReadShort();
	}

	defender = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, defender++) 
	{
		if (entityType == defender->entityType && idx == defender->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	defender->event = GTH_EV_CHAR_DIE;
	if( entityType == ENTITY_MONSTER )
	{
		if( damage > 0 )
		{
			if( !g_charManager->GotDamageMessage( defender->idxOnServer, ENTITY_MONSTER, attackerIdx, attackerType, damage, attackState )	)
				return	1;
		}
	}
	else if (entityType == ENTITY_PC || entityType == ENTITY_NPC )
	{
		if ( damage > 0 )
		{
			g_ifMng->m_numCtl->AddDamageNumber( defender->idxOnServer, entityType, attackerIdx, attackerType, damage, attackState );
		}
	}
	
	
	
	if(	attackerIdx	>= 0 && damage > 0 )
	{
		if( attacker != NULL )
		{
			if( g_charManager->m_pAnimManager->ShowAttackEffect( attacker->ani_curIdx )	)
			{
				g_charManager->ShowAttackEffect ( attacker, damage );
			}
		}
	}

	if( defender->entityType == ENTITY_PC && defender->idxOnServer == g_cgv.myPCIdxOnServer )
	{
		if( g_cgv.myCharacterInfo->level > 5 )
		{
			
			
			if (g_cgv.myCharacterInfo->worldIdx != DAN_BATTLEMAP_NUM)				
			{
				
				if(g_cgv.myCharacterInfo->exp - exp > 0)
				{
					strcpy( szTemp2, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->exp - exp ) );
					sprintf( temp, g_LPACK.GetMassage(0,181), szTemp2 );
					g_ifMng->AddSysMessage( temp, D3DCOLOR_ARGB(255, 100, 255, 255) );
				}
		
				
				
				if(g_cgv.myCharacterInfo->curChargeSE - curChargeSE > 0)
				{
					strcpy( szTemp2, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->curChargeSE - curChargeSE ) );
					sprintf( temp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,255), szTemp2 );
					g_ifMng->AddSysMessage( temp, D3DCOLOR_ARGB(255, 100, 255, 255) );
				}
				
			}
			

			
			if( dropItemType == 0 )
			{
				int itemIdx = g_cgv.myCharacterInfo->equipment[dropItemPosition];
				
				sprintf( temp, g_LPACK.GetMassage( 0, 659 ), g_cgv.myCharacterInfo->item[itemIdx].name );
				g_ifMng->AddSysMessage( temp, GTHCOLOR_NOTICEMESSAGE );

				GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
				g_cgv.myCharacterInfo->equipment[dropItemPosition] = -1;
			}
			else if( dropItemType == 1 )
			{
				int itemIdx = g_cgv.myCharacterInfo->inventory[dropItemPosition];
				
				sprintf( temp, g_LPACK.GetMassage( 0, 659 ), g_cgv.myCharacterInfo->item[itemIdx].name );
				g_ifMng->AddSysMessage( temp, GTHCOLOR_NOTICEMESSAGE );

				GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
				g_cgv.myCharacterInfo->inventory[dropItemPosition] = -1;
			}
		}
		g_cgv.myCharacterInfo->exp = exp;
		
		
		if((FALSE == g_pApp->m_myCharacter->m_bWaitingRoom) && (PCB_NONE == g_GonryunBattlePractic.m_ibattleMode))
		{
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,418), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_RESTART );
			g_ifMng->RestartInterface( 1 );	
		}
		
	}

	
	if( attacker != NULL )
	{	
		
		if( defender->entityType == ENTITY_PC && defender->idxOnServer == g_cgv.myPCIdxOnServer )
		{
			if( attacker->entityType == ENTITY_PC )
			{
				sprintf( temp, g_LPACK.GetMessage(0,1017), attacker->name );
				g_ifMng->AddSysMessage( temp );
			}
		}
		
		if( attacker->entityType == ENTITY_PC && attacker->idxOnServer == g_cgv.myPCIdxOnServer )
		{
			if( defender->entityType == ENTITY_PC )
			{
				
				sprintf( temp, g_LPACK.GetMessage(0,1018), defender->name );
				
				g_ifMng->AddSysMessage( temp );
			}
		}
	}	
	
#ifdef _DEBUG




#endif
	

	return 1;
}

int GTH_ProcessEventMessage_Respawn()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;
	int worldIdx;
	vec3_t position;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	if( character->idxOnServer == g_cgv.myPCIdxOnServer &&
		character->entityType == ENTITY_PC )
	{
		
		GTH_UnlockSending();

	}

    worldIdx = MSG_ReadByte();

	if ( character->idxOnServer == g_cgv.myPCIdxOnServer && 
		 character->entityType == ENTITY_PC )
	{
		g_ifMng->ExitMessage();

		
		if ( g_ifMng->m_stackWin->State() )
		{
			g_ifMng->m_stackWin->m_saveInvenPos = -1;
			g_ifMng->m_stackWin->m_saveItemPrice = 0;
			g_ifMng->m_stackWin->m_stackType = -1;
			g_ifMng->m_stackWin->Enable(FALSE);
		}


		if( worldIdx != character->worldIdx )	
		{
			position[0] = (float)MSG_ReadShort();
			position[1] = (float)MSG_ReadShort();
			position[2] = (float)MSG_ReadShort();
			
			g_cgv.syncCharacter[0].worldIdx = worldIdx;
			VectorCopy(g_cgv.syncCharacter[0].position, position);
			g_cgv.syncCharacter[0].angles[YAW] =  MSG_ReadFloat();
			g_cgv.syncCharacter[0].curRA = MSG_ReadLong();
			g_cgv.syncCharacter[0].curSA = MSG_ReadShort();
			g_cgv.syncCharacter[0].event = GTH_EV_CHAR_RESPAWN;
			
			character->event = GTH_EV_CHAR_IDLE;
			
			GTH_PrepareChangeWorld();
		}
		else
		{			
			MSG_ReadPosition( character->position );
			character->angles[ YAW ] = MSG_ReadFloat();
			character->curRA = MSG_ReadLong();
			character->curSA = MSG_ReadShort();
			character->isAlive = true;
			character->event = GTH_EV_CHAR_RESPAWN;
		}
	}
	
	else
	{
		MSG_ReadPosition( character->position );
		character->angles[ YAW ] = MSG_ReadFloat();
		character->curRA = MSG_ReadLong();
		character->curSA = MSG_ReadShort();
		character->isAlive = true;
		character->event = GTH_EV_CHAR_RESPAWN;
	}

	return 1;
}

int GTH_ProcessEventMessage_MouseWalk()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_MOUSEWALK;
	
	character->mouseAngle = MSG_ReadFloat();
	MSG_ReadPosition( character->mousePos );

	return 1;
}

int GTH_ProcessEventMessage_MouseRun()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	character->event = GTH_EV_CHAR_MOUSERUN;

	character->mouseAngle = MSG_ReadFloat();
	MSG_ReadPosition( character->mousePos );

	return 1;
}


int GTH_ProcessEventMessage_Skill()
{
	return g_SkillApp.Recv_ProcessEventMessage_Skill();
}



int GTH_ProcessEventMessage_SkillCasting()
{
	return g_SkillApp.Recv_ProcessEventMessage_SkillCasting();
}


int GTH_ProcessMessage_LevelUp()
{
	int i;
	__int64 exp, genExp, exp2, genExp2; 
	int lootNak, level, gen, bonusStatusPoint, genLevel;
	int bonusSkillPoint;
	int lootNak2;
	int force, soul, agi, vit;
	int idx, numCharacter, isSharingExp, genCapability, genCapability2;

	Fx_CHARACTER_t *character;
	int entityType;

	
	char temp[256];
	char szTemp2[256], szTemp3[256];

	gen = g_cgv.myCharacterInfo->selectedGen;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();

	


	exp = MSG_Read64Int();
	genExp = MSG_Read64Int();
	lootNak = MSG_ReadLong();
	genCapability = MSG_ReadLong();

	level = MSG_ReadShort();
	genLevel = MSG_ReadShort();
	bonusStatusPoint = MSG_ReadShort();
	force = MSG_ReadShort();
	soul = MSG_ReadShort();
	agi = MSG_ReadShort();
	vit = MSG_ReadShort();

	int curRA, curSA;
	curRA = MSG_ReadLong();
	curSA = MSG_ReadShort();
	bonusSkillPoint = MSG_ReadByte();




	isSharingExp = MSG_ReadByte();		

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for ( i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	if( character->idxOnServer == g_cgv.myPCIdxOnServer && character->entityType == ENTITY_PC )
	{
		exp2 = exp - g_cgv.myCharacterInfo->exp;
		genExp2 = genExp - g_cgv.myCharacterInfo->genExp;
		if( exp2 < 0 ) exp2 = 0;
		if( genExp2 < 0 ) genExp2 = 0;
		
		if( exp2 > 0 || genExp2 > 0 )
		{
			switch( isSharingExp )
			{
			case 0 : case 1 :
				
				
				strcpy( szTemp2, g_ifMng->GetNumberString( exp2 ) );
				strcpy( szTemp3, g_ifMng->GetNumberString( genExp2 ) );
				sprintf( temp, g_LPACK.GetMassage(0,183), szTemp2, szTemp3 );
				g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
				break;
			case 2 :
				
				
				strcpy( szTemp2, g_ifMng->GetNumberString( exp2 ) );
				strcpy( szTemp3, g_ifMng->GetNumberString( genExp2 ) );
				sprintf( temp, g_LPACK.GetMassage(0,402), szTemp2, szTemp3 );
				g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
				break;
			case 3 :
				
				
				strcpy( szTemp2, g_ifMng->GetNumberString( exp2 ) );
				strcpy( szTemp3, g_ifMng->GetNumberString( genExp2 ) );
				sprintf( temp, g_LPACK.GetMassage(0,403), szTemp2, szTemp3 );
				g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
				break;
			case 10 :
				
				break;
			}
		}

		g_cgv.myCharacterInfo->exp = exp;
		g_cgv.myCharacterInfo->genExp = genExp;
		
		genCapability2 = genCapability - g_cgv.myCharacterInfo->genCapability[gen];
		if( genCapability2 > 0 && isSharingExp != 10 )
		{
				strcpy( szTemp2, g_ifMng->GetNumberString( genCapability2 ) );
				sprintf( temp, g_LPACK.GetMassage(0,412), 
					g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GEN_ATTRIBUTE, gen ), 
					szTemp2 );
				g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
		}
		g_cgv.myCharacterInfo->genCapability[g_cgv.myCharacterInfo->selectedGen] = genCapability;

		lootNak2 = lootNak - g_cgv.myCharacterInfo->curChargeSE;
		if( lootNak2 > 0 && isSharingExp != 10 )
		{
			
			strcpy( szTemp2, g_ifMng->GetNumberString( lootNak2 ) );
			sprintf( temp, g_LPACK.GetMassage(0,184), szTemp2 );
			g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
		}
		g_cgv.myCharacterInfo->curChargeSE = lootNak;
		
		g_cgv.myCharacterInfo->bonusStatusPoint = bonusStatusPoint;
		g_cgv.myCharacterInfo->bonusSkillPoint = bonusSkillPoint;
		g_cgv.myCharacterInfo->genCapability[g_cgv.myCharacterInfo->selectedGen] = genCapability;
		g_cgv.myCharacterInfo->force = force;
		g_cgv.myCharacterInfo->soulpower = soul;
		g_cgv.myCharacterInfo->agility = agi;
		g_cgv.myCharacterInfo->vitality = vit;

		g_cgv.myCharacterInfo->level = level;
		g_cgv.myCharacterInfo->genLevel = genLevel;

		g_cgv.myCharacterInfo->curRA = curRA;
		g_cgv.myCharacterInfo->curSA = curSA;
		character->curRA = curRA;
		character->curSA = curSA;

		

	}

	 
	switch( character->tableIdx )
	{
	case 0 :
		g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 3 , character ->position , NULL , character ->angles , 0.0f , character ->entityType , character ->idxOnServer );
		break;
	case 1 :
		g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 4 , character ->position , NULL , character ->angles , 0.0f , character ->entityType , character ->idxOnServer );
		break;
	case 2 :
		g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 6 , character ->position , NULL , character ->angles , 0.0f , character ->entityType , character ->idxOnServer );
		break;
	case 3 :
		g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 5 , character ->position , NULL , character ->angles , 0.0f , character ->entityType , character ->idxOnServer );
		break;

	}
	return 1;
}

int GTH_ProcessEventMessage_Sit()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 





	character->event = GTH_EV_CHAR_SIT;

	return 1;
}

int GTH_ProcessEventMessage_Stand()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 





	character->event = GTH_EV_CHAR_STAND;

	return 1;
}





int GTH_ProcessMessage_WeaponChange()
{
	GTH_UnlockSending();

	int curWeapon;
	int nextWeapon;
	curWeapon = g_cgv.myCharacterInfo->curWeapon;
	if( curWeapon == 0 ) nextWeapon = 1;
	else nextWeapon = 0;

	g_cgv.myCharacterInfo->curWeapon = nextWeapon;
	g_charManager->GenerateMyCharacter();

	return 1;
}


int GTH_ProcessMessage_ReplyGenChange()
{
	GTH_UnlockSending();
	
	int gen;
	gen = MSG_ReadByte();
	g_cgv.myCharacterInfo->selectedGen = gen;
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_GEN_CLICK ), GTH_SOUND_PLAY_ONCE );
	return 1;
}

int GTH_ProcessMessage_ReplyStatusUp()
{
	GTH_UnlockSending();

	int reply, error;
	int statusType, point, bonusPoint;
	int curRA, curSA;

	reply = MSG_ReadByte();
	if( reply )
	{
		statusType = MSG_ReadShort();
		point = MSG_ReadShort();
		bonusPoint = MSG_ReadShort();
		
		curRA = MSG_ReadLong();
		curSA = MSG_ReadShort();
		
		g_cgv.myCharacterInfo->curRA = curRA;
		g_cgv.myCharacterInfo->curSA = curSA;
		
		g_pApp->m_myCharacter->curRA = curRA;
		g_pApp->m_myCharacter->curSA = curSA;
		
		switch( statusType )
		{
			
		case 0 :
			g_cgv.myCharacterInfo->force = point;
			break;
		case 1 :
			g_cgv.myCharacterInfo->soulpower = point;
			break;
		case 2 :
			g_cgv.myCharacterInfo->agility = point;
			break;
		case 3 :
			g_cgv.myCharacterInfo->vitality = point;
			break;
		}
		g_cgv.myCharacterInfo->bonusStatusPoint = bonusPoint;
	}
	else
	{
		
		error = MSG_ReadByte();
		if( error == 1 ) g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,411) );
		else if( error == 2 ) g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMessage(0,915) );
	}

	return 1;
}

int GTH_ProcessMessage_AutoRecovery()
{
	int curRA, curSA;
	curRA = MSG_ReadLong();
	curSA = MSG_ReadShort();
	g_pApp->m_myCharacter->curRA = curRA;
	g_cgv.myCharacterInfo->curRA = curRA;
	g_cgv.myCharacterInfo->curSA = curSA;
	g_cgv.myCharacterInfo->curSA = curSA;
	return 1;
}

int GTH_SendMessage_RequestEpackUse( int type )
{
	

	
	
	if(type==0 || type==1){
		if(g_GonryunBattlePractic.m_ibattleMode==PCB_START )
		{
		
			g_ifMng->AddSysMessage(g_LPACK.GetMassage(9,195), D3DCOLOR_ARGB(255, 255, 125, 100) );
		
			return 0;
		}
		
		
		
		if(g_GolryunBattle.GetStatus()==tagGolryun_Packet_Status::GB_REQUEST_PROGRESS_MATCH )
		{
		
				g_ifMng->AddSysMessage(g_LPACK.GetMassage(9,195), D3DCOLOR_ARGB(255, 255, 125, 100) );
			return 0;
		}
		
	}


	MSG_BeginWriting(&netMessage);
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_EPACKUSE );
		MSG_WriteByte(&netMessage, type );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_ReplyEpackUse()
{


	int type, recoveryTime, curCharge, curRASA;
	type = MSG_ReadByte();

	
	if( type == 2 ) return 0;

	curRASA = MSG_ReadLong();
	recoveryTime = MSG_ReadShort();
	curCharge = MSG_ReadShort();

	switch( type )
	{
	case 0 :
		g_pApp->m_myCharacter->curRA = curRASA;
		g_cgv.myCharacterInfo->curRA = curRASA;
		g_cgv.myCharacterInfo->curChargeRA = curCharge;
		g_cgv.epackRADelayMaxTime = (float)recoveryTime;
		g_cgv.epackRADelayTime = (float)recoveryTime;
		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_RECOVERY_RA ), GTH_SOUND_PLAY_ONCE );
		break;
	case 1 :
		g_pApp->m_myCharacter->curSA = curRASA;
		g_cgv.myCharacterInfo->curSA = curRASA;
		g_cgv.myCharacterInfo->curChargeSA = curCharge;
		g_cgv.epackSADelayMaxTime = (float)recoveryTime;
		g_cgv.epackSADelayTime = (float)recoveryTime;
		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_RECOVERY_SA ), GTH_SOUND_PLAY_ONCE );
		break;
	}

	return 1;
}






int GTH_ProcessMessage_Affect()
{
	int i, entityIdx, entityType, numCharacter;
	int affectNumber, affectIdx[MAX_AFFECT], affectTime[MAX_AFFECT];
	vec3_t pos;
	float velocity;
	Fx_CHARACTER_t *character;

	entityIdx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	MSG_ReadPosition( pos );
	velocity = MSG_ReadFloat();

	affectNumber = MSG_ReadByte();
	for( i = 0; i < affectNumber; i ++ )
	{
		affectIdx[i] = MSG_ReadShort();
		affectTime[i] = MSG_ReadLong();
	}
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && entityIdx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	
	if( character->entityType == ENTITY_PC && character->idxOnServer == g_cgv.myPCIdxOnServer )
	{
		for( i = 0; i < affectNumber; i ++ )
		{
			g_ifMng->m_nameWin->m_affectTime[i] = (float)affectTime[i];
		}
	}

	character->moveSpeed = velocity;

	g_charManager->GenerateCharacterAffect( character, affectNumber, affectIdx );	
	return 1;
}



int	GTH_ProcessEventMessage_MonsterSkillCasting()
{
	int				i;
	int				idx, entityType, curSA, castingTime, saveSkillIdx;
	
	int				numCharacter;

	Fx_CHARACTER_t*	character;

	idx				= MSG_ReadShort();
	entityType		= MSG_ReadByte();
	saveSkillIdx	= MSG_ReadLong();
	curSA			= MSG_ReadShort();
	castingTime		= MSG_ReadLong();

	character		= &g_charManager->m_Characters[0];

	numCharacter	= g_charManager->m_iNumCharacters;

	
	g_SkillFile.DebugOutString("GTH_ProcessEventMessage_MonsterSkillCasting ...  saveSkillIdx %d \n",saveSkillIdx);
	
	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	
	character->event = GTH_EV_CHAR_IDLE;

	return 1;
}


int GTH_ProcessEventMessage_MonsterSkill()
{
	int				i;
	int				idx, numCharacter;
	int				entityType;
	int				targetIdx, targetType;
	int				tableIdx, level, curRA, curSA;
	int				skillIdx, coolTime, delayTime;
	int				multiTargetNumber;
	int				multiTargetType[8], multiTargetIdx[8];
	int				resultHitRate;
	int				isValid;
	float			angles;

	vec3_t			pos, delta;
	Fx_CHARACTER_t*	character;

	idx			= MSG_ReadShort();
	entityType	= MSG_ReadByte();

	MSG_ReadPosition( pos );

	angles		= MSG_ReadFloat();
	tableIdx	= MSG_ReadShort();
	level		= MSG_ReadLong();

	curRA		= MSG_ReadLong();
	curSA		= MSG_ReadShort();

	targetIdx	= MSG_ReadShort();
	targetType	= MSG_ReadByte();

	skillIdx	= MSG_ReadByte();
	coolTime	= MSG_ReadLong();
	delayTime	= MSG_ReadLong();

	isValid		= MSG_ReadByte();

	multiTargetNumber = MSG_ReadByte();

	
	g_SkillFile.DebugOutString("GTH_ProcessEventMessage_MonsterSkill ... tableIdx %d.. skillIdx %d .. coolTime %d .. delayTime %d \n",
		tableIdx,skillIdx,coolTime,delayTime);

	for( i = 0; i < multiTargetNumber; i ++ )
	{
		multiTargetType[i]	= MSG_ReadByte();
		multiTargetIdx[i]	= MSG_ReadShort();
	}

	resultHitRate	= MSG_ReadByte();

	character		= &g_charManager->m_Characters[0];
	numCharacter	= g_charManager->m_iNumCharacters;

	for( i = 0; i < numCharacter; i++, character++ ) 
	{
		if( entityType == character->entityType && idx == character->idxOnServer )
			break;
	}

	if(i >= numCharacter) return 0; 

	character->stat_attackSpeed = MSG_ReadShort();
	character->angles[ YAW ]	= angles;
	character->stat_attackSpeed = (float)delayTime;

	character->targetIdx  = targetIdx;
	character->targetType = targetType;

	if( !VectorCompare( character->position, pos ) )
	{
		VectorClear(delta);
		g_move->Pmove( pos, delta, character->angles );
		VectorCopy( character->position, pos );
		g_charManager->FixCharacterPosition();
	}

	
	character->event = GTH_EV_CHAR_IDLE;
	character->curRA = curRA;
	character->curSA = curSA;
	
	g_SkillLogic.GTH_SkillExecute( character, tableIdx, level, multiTargetNumber, multiTargetType, multiTargetIdx ,targetType);

	return 1;
}



int	GTH_ProcessEventMessage_Teleport()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;
	int worldIdx;
	vec3_t position;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

    worldIdx = MSG_ReadByte();
	
	MSG_ReadPosition(position);

	if( position[0] == character->position[0] &&  position[1] == character->position[1] && position[2] == character->position[2])
	{
		
		character->event	= GTH_EV_CHAR_IDLE;
		g_ifMng ->m_numCtl ->AddDamageNumber ( character->idxOnServer, ENTITY_PC, character->idxOnServer, ENTITY_PC, 0, 0 );
		g_ifMng ->m_numCtl ->ReadyDamage ( character );

		character->angles[ YAW ] = MSG_ReadFloat();
		character->curRA = MSG_ReadLong();
		character->curSA = MSG_ReadShort();

		return 0;

	}
	else
	{
		character->position[0] = position[0];
		character->position[1] = position[1];
		character->position[2] = position[2];
	}


	character->angles[ YAW ] = MSG_ReadFloat();
	character->curRA = MSG_ReadLong();
	character->curSA = MSG_ReadShort();

	
	vec3_t delta;
	VectorClear(delta);
	g_move->Pmove( character->position , delta , character->angles );

	if( character->idxOnServer == g_cgv.myPCIdxOnServer && character->entityType == ENTITY_PC )
	{
		
		GTH_UnlockSending();

		g_ifMng->ExitMessage();
		
		character->isAlive	= true;
		character->event	= GTH_EV_CHAR_TELEPORT;

		GTH_SendEventMessage_Skill();
		g_cgv.myCharacterInfo->skillCastingtime = g_cgv.myCharacterInfo->calAttackDelay;

	}

	return 1;
}


int		GTH_ProcessEventMessage_Before_Teleport()
{
	int i, idx, entityType, numCharacter, worldIdx, saveSkillIdx;
	Fx_CHARACTER_t *pCharacter = NULL;

	idx				= MSG_ReadShort();
	entityType		= MSG_ReadByte();

	pCharacter		= &g_charManager->m_Characters[0];
	numCharacter	= g_charManager->m_iNumCharacters;

	for(i = 0; i < numCharacter; i++, pCharacter++)
	{
		if( entityType == pCharacter->entityType && idx == pCharacter->idxOnServer )
			break;
	}

	if (i >= numCharacter) return 0; 

	worldIdx		= MSG_ReadByte();
	saveSkillIdx	= MSG_ReadShort();
	int pcjob		= MSG_ReadShort();
	MSG_ReadPosition( pCharacter->position );

	
	
	g_SkillLogic.GTH_BeforeTeleportExecute(pCharacter, pcjob);

	return 1;
}



int GTH_ProcessEventMessage_State()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	int charEvent = MSG_ReadByte();

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	
	if ( ( character->event == GTH_EV_CHAR_MOUSEWALK || character->event == GTH_EV_CHAR_MOUSERUN ) && character->ren_generalSpeed > 0 )
		return 1;

	character->event = (i3characterEvent_t)charEvent;

	return 1;
}






int GTH_ProcessEventMessage_SocialAnimaton(int event)
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;

	idx = MSG_ReadShort();
	entityType = MSG_ReadByte();
	
	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return 0; 

	
	
	switch(event)
	{
		case GSC_SUB_EVENT_SOCIAL_SHOUT:	character->event = GTH_EV_CHAR_SHOUT;	break;
		case GSC_SUB_EVENT_SOCIAL_BOW:		character->event = GTH_EV_CHAR_BOW;		break;
	}
	
	

	return 1;
}

void GTH_ProcessEventMessage_CompleteEffect()
{
	int idx, entityType, effectIdx;
	int numCharacter;
	Fx_CHARACTER_t *character;

	idx = MSG_ReadShort();
	entityType = MSG_ReadShort();
	effectIdx = MSG_ReadShort();

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (int i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	if (i >= numCharacter) return; 

	g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
								effectIdx, 
								character->position,
								NULL, 
								true, 
								character->entityType, 
								character->idxOnServer,
								0.0f, 
								0.0f, 
								0.0f, 
								-1.0f,
								false );


}
