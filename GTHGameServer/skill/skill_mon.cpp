#include "../global.h"

#ifdef _OLD_SKILL_VER

void SKILL_MON_MeteoStrike( void* pMonster, float* pValue )
{
	monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(pMonster);
	float dmg = pValue[SKILL_VALUE_0];
	float range = pValue[SKILL_VALUE_1];

	
	if( pMon->targetType == ENTITY_PC )
	{
		if( g_pc[pMon->targetIdx].event == GTH_EV_CHAR_DIE ) return;
	}

	messageInfo_t msginfo;
	message_t message;

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_ATTACK;
	msginfo.affect.skillTableIdx = 131;


	msginfo.attack.atkType = MSGINFO_ATK_TYPE_MAGIC;
	
	msginfo.attack.posMinDmg = (int)dmg + pMon->calMagMinDmg;
	msginfo.attack.posMaxDmg = (int)dmg + pMon->calMagMaxDmg;

	msginfo.attack.delayTime		= 400;

	
	msginfo.attack.etcInfo = MSGINFO_ATK_ETC_INFO_STUN;
	msginfo.attack.etcData0 = 100;

	if(pMon->targetType == ENTITY_PC )
	{
		if( pMon->targetIdx > 0 )
			msginfo.attack.etcData1 = g_pc[pMon->targetIdx].calStunTime;
	}
	else
	{
		if( pMon->targetIdx > 0 )
			msginfo.attack.etcData1 = g_monster[pMon->targetIdx].calStunTime;
	}
	
	msginfo.targetType = pMon->targetType;
	msginfo.targetIdx  = pMon->targetIdx;

	AI_MakeMessage( pMon, &message, MESSAGE_USEINFO, &msginfo );
	
	AI_SendMessage( &message );
}


void SKILL_MON_Slow( void* pMonster, float* pValue )
{
	float decVel, attackspeed, range, keepTime;

	monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(pMonster);

	decVel	    = pValue[SKILL_VALUE_0];
	attackspeed = pValue[SKILL_VALUE_1];
	range	    = pValue[SKILL_VALUE_2];
	keepTime    = pValue[SKILL_VALUE_3] * 1000.0f;

	if( g_pc[pMon->targetIdx].event == GTH_EV_CHAR_DIE ) return;

	messageInfo_t msginfo;
	message_t message;


	memset( &msginfo, 0, sizeof( messageInfo_t ) );


	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	msginfo.affect.skillTableIdx = 133;
	msginfo.affect.level = 1;

	msginfo.affect.type = AFFECT_ATTACK;
	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime = g_globalTime;
	msginfo.affect.keepTime = g_globalTime + (int)keepTime;
	msginfo.targetType = ENTITY_PC;
	msginfo.targetIdx = pMon->targetIdx;
	msginfo.affect.level = 1;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_ATTACKSPEEDRATE;
	msginfo.affect.common.data[0] = -(int)attackspeed;


	msginfo.affect.common.type[1] = AFFDATA_TYPE_VELOCITY_RATE;
	msginfo.affect.common.data[1] = -(int)decVel;

	AI_MakeMessage( pMon, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message, (int)range );
}


void SKILL_MON_BazmoStrike( void* pMonster, float* pValue )
{
	float dmg, range, pro;

	monsterCharacter_t* pMon = static_cast<monsterCharacter_t *>(pMonster);

	messageInfo_t	msginfo;
	message_t		message;

	memset( &msginfo, 0, sizeof(messageInfo_t) );

	msginfo.infoType = MSGINFO_TYPE_ATTACK;

	
	
	
	dmg   = pValue[SKILL_VALUE_0];
	range = pValue[SKILL_VALUE_1];
	pro   = pValue[SKILL_VALUE_2];

	msginfo.attack.atkType   = MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower  = (int)pro;
	msginfo.attack.phyMinDmg = (int)dmg + pMon->calPhyMinDmg;
	msginfo.attack.phyMaxDmg = (int)dmg + pMon->calPhyMaxDmg;

	msginfo.attack.criticalRate   = 0;
	msginfo.attack.criticalDamage = 0;
	msginfo.attack.delayTime = pMon->calAttackDelay;

	msginfo.attack.delayTime = 1000.0f;
	msginfo.attack.repeatNum = 3;
	msginfo.attack.repeatDelayTime = 300.0f;

	msginfo.targetType = pMon->targetType;
	msginfo.targetIdx  = pMon->targetIdx;

	AI_MakeMessage( pMon, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message, (int)range );

}

void SKILL_MON_SpikeShield( void* pMonster, float* pValue )
{
	float dmg, range, keepTime;
	messageInfo_t	msginfo;
	message_t		message;

	monsterCharacter_t* pMon = static_cast<monsterCharacter_t *>(pMonster);

	memset( &msginfo, 0, sizeof(messageInfo_t) );
	
	dmg		 = pValue[SKILL_VALUE_0];
	range	 = pValue[SKILL_VALUE_1];
	keepTime = pValue[SKILL_VALUE_2] * 1000.0f;

	msginfo.infoType		= MSGINFO_TYPE_ATTACK;

	msginfo.attack.atkType	= MSGINFO_ATK_TYPE_MAGIC;

	msginfo.attack.posMinDmg = (int)dmg + pMon->calMagMinDmg;
	msginfo.attack.posMaxDmg = (int)dmg + pMon->calMagMaxDmg;

	msginfo.targetType = pMon->targetType;
	msginfo.targetIdx  = pMon->targetIdx;

	AI_MakeMessage( pMon, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message, (int)range );
}

void SKILL_MON_DevonHead( void* pMonster, float* pValue )
{
	float dmg, pro;
	messageInfo_t	msginfo;
	message_t		message;

	monsterCharacter_t* pMon = static_cast<monsterCharacter_t *>(pMonster);
	memset( &msginfo, 0, sizeof(messageInfo_t) );

	dmg		= pValue[SKILL_VALUE_0];
	pro		= pValue[SKILL_VALUE_1];

	msginfo.infoType =  MSGINFO_TYPE_ATTACK;

	msginfo.attack.atkType = MSGINFO_ATK_TYPE_NORMAL;

	msginfo.attack.phyMinDmg = (int)dmg + pMon->calPhyMinDmg;
	msginfo.attack.phyMaxDmg = (int)dmg + pMon->calPhyMaxDmg;

	msginfo.attack.hitPower = (int)pro;

	msginfo.targetType = pMon->targetType;
	msginfo.targetIdx  = pMon->targetIdx;

	AI_MakeMessage( pMon, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message);

}

#endif