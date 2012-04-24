#include "../global.h"


int AI_InterpretMessage( monsterCharacter_t* monster, message_t* message )
{
	playerCharacter_t  *pToPC = NULL;
	monsterCharacter_t *pToMon = NULL;

	if( message->fromType == ENTITY_PC ) pToPC = &g_pc[message->fromIdx];
	else pToMon = &g_monster[message->fromIdx];

	switch( message->type )
	{
	case MSG_DAMEGED :
		
		if( monster->aliveState == MON_ALIVE_STATE_NORMAL ) 
		{
			
			if( message->fromType == ENTITY_PC )
			{
				if( pToPC->event != GTH_EV_CHAR_STUN && pToPC->event != GTH_EV_CHAR_DIE ) GTH_Monster_Damage(monster, message);
			}
			else
			{
				if( pToMon->event != GTH_EV_CHAR_STUN && pToMon->event != GTH_EV_CHAR_DIE && pToMon->event != GTH_EV_CHAR_FREEZE) 
					GTH_Monster_Damage(monster, message);
			}
		}
		break;
	case MSG_AFFECT :
		{
			GTH_Affect( monster, message, monster->entityType );
		}
		break;
	case MSG_ETC :
		if( !AI_InterpretMessage_Etc( monster, message ) ) return( false );
		break;
	}
	if( message->repeatNum > 0 ) 
	{
		message->repeatNum --;
		
		message->receiveTime = g_globalTime + message->repeatDelayTime;
	}
	return( true );
}

int AI_InterpretMessage_Etc( monsterCharacter_t* monster, message_t *message )
{
	switch( message->data[0] )
	{
	case MSG_ETC_RESURRECTION :
		return( false );
		break;
	case MSG_ETC_FORCED_CHANGE_TARGET :
		AI_RemoveTarget( monster );
		monster->forcedEntityType = message->data[1];
		monster->forcedEntityIdx = message->data[2];
		break;
	}
	return( true );
}

int AI_InterpretMessage( playerCharacter_t* pc, message_t* message )
{
	playerCharacter_t  *pToPC;
	monsterCharacter_t *pToMon;

	if( message->fromType == ENTITY_PC ) pToPC = &g_pc[message->fromIdx];
	else pToMon = &g_monster[message->fromIdx];

	switch( message->type )
	{
	case MSG_DAMEGED :
		
		if( pc->alive ) 
		{
			
			if( message->fromType == ENTITY_PC )
			{
				if( pToPC->event != GTH_EV_CHAR_STUN && pToPC->event != GTH_EV_CHAR_DIE ) GTH_PC_Damage(pc, message); 
			}
			else
			{
				if( pToMon->event != GTH_EV_CHAR_STUN && 
					pToMon->event != GTH_EV_CHAR_DIE && 
					pToMon->event != GTH_EV_CHAR_FREEZE ) 
					GTH_PC_Damage(pc, message); 
			}
		}
		break;
	case MSG_AFFECT :
		{
			GTH_Affect( pc, message );
		}
		break;
	case MSG_ETC :
		if( !AI_InterpretMessage_Etc( pc, message ) ) return( false );
		break;
	}
	if( message->repeatNum > 0 ) 
	{
		message->repeatNum --;
		
		message->receiveTime = g_globalTime + message->repeatDelayTime;
	}
	return( true );
}


int AI_InterpretMessage_Etc( playerCharacter_t* pc, message_t *message )
{

	switch( g_SkillDataGlobal.GetMessageType(SKILL_MES_TYPE_ETC_TYPE,message) )
	{
	case MSG_ETC_RESURRECTION :
		{
			float curRA, curSA;
			int recoveryExp;

			curRA		= (float)g_SkillDataGlobal.GetMessageType(SKILL_MES_TYPE_RA,message);
			curSA		= (float)g_SkillDataGlobal.GetMessageType(SKILL_MES_TYPE_SA,message);

			if(curRA > pc->calMaxRA) curRA = pc->calMaxRA;
			if(curSA > pc->calMaxSA) curSA = pc->calMaxSA;
			
			
			
			
			
			

			
			recoveryExp	= (int)( (float)g_SkillDataGlobal.GetMessageType(SKILL_MES_TYPE_EXP,message) / g_logic.expRate );	
			GTH_PC_Respawn( pc, (int)curRA, (int)curSA, recoveryExp, true );

			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_SKILL,"%d;;;;;;operator=%s recoveryExp=%d", 
				LOG_CLASS_SKILL_RESURRECTION, 
				message->fromNameV2,				
				recoveryExp);

		}
		return( false );
		break;
	case MSG_ETC_FORCED_CHANGE_TARGET :
		break;
	}
	return( true );
}
