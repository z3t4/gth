#include "../global.h"

#define HIT_RATE_ADD_PROB			10		
#define HIT_CRIRATE_ADD_PROB		20		
#define HIT_CRIDMG_ADD_PROB			5.0f	


void AI_MakeMessage( playerCharacter_t* pc, message_t* message, int type, messageInfo_t *msginfo )
{
	memset( message, 0, sizeof( message_t ) );
	
	switch( type )
	{
	case MESSAGE_NORMAL_ATTACK :
		AI_MakeAttackMessage( pc, message );
		break;
	case MESSAGE_USEINFO :
		if( msginfo->infoType == MSGINFO_TYPE_ATTACK ) AI_MakeMsgInfoAttack( pc, message, msginfo );
		else if( msginfo->infoType == MSGINFO_TYPE_AFFECT ) AI_MakeMsgInfoAffect( pc, message, msginfo );
		break;
	}
}


void AI_MakeAttackMessage( playerCharacter_t* pc, message_t* message  )
{
	int d, attackType = 0, criticalRate = 0;


	message->type = MSG_DAMEGED;
	message->fromIdx = pc->idx;
	message->fromType = ENTITY_PC;
	
	strncpy( message->fromNameV2, pc->name,NAMESTRING);
	message->fromNameV2[NAMESTRING]=NULL;


	message->toType = pc->targetType;
	message->toIdx = pc->targetIdx;

	message->worldIdx = pc->worldIdx;
	message->sendTime = g_globalTime;

	pc->curSA -= (float)pc->atkBowSACost;

	
	pc->curRA -= pc->optValue[OV_ATK_RACOST];
	pc->curSA -= pc->optValue[OV_ATK_SACOST];
	if( (int)pc->curRA < 0 ) pc->curRA = 1.0f;
	if( (int)pc->curSA < 0 ) pc->curSA = 0.0f;
	

	
	PC_GetAttackComboType( pc );
	message->receiveTime = g_globalTime + (int)( (float)pc->sock.latencyTime );

	message->repeatNum = 0;

	
	message->data[MSG_ATKDATA_ATTACK_TYPE] = MSGINFO_ATK_TYPE_NORMAL;

	
	d = pc->calPhyMaxDmg - pc->calPhyMinDmg	+ 1;
	message->data[MSG_ATKDATA_PHYSIC_DMG] = GTH_Randomize( d ) + pc->calPhyMinDmg;		

	d = pc->calPosMaxDmg - pc->calPosMinDmg + 1;
	message->data[MSG_ATKDATA_POSITIVE_DMG] = GTH_Randomize( d ) + pc->calPosMinDmg;	
	
	d = pc->calNegMaxDmg - pc->calNegMinDmg + 1;
	message->data[MSG_ATKDATA_NEGATIVE_DMG] = GTH_Randomize( d ) + pc->calNegMinDmg;	

	message->data[MSG_ATKDATA_WEAPON_TYPE] = 0;
	

	
	message->data[MSG_ATKDATA_HITRATE] = 0;
	message->data[MSG_ATKDATA_CRITICAL_RATE] = 0;
	
	message->data[MSG_ATKDATA_CRITICAL_DAMAGE] = 0;
	message->data[MSG_ATKDATA_STUN_FLAG] = 0;
}

void AI_MakeMsgInfoAttack( playerCharacter_t *pc, message_t* message, messageInfo_t* msginfo )
{
	MSGINFO_ATTACK* atkinfo = &msginfo->attack;
	int d, attackType = 0, criticalRate = -1;

	message->type = MSG_DAMEGED;
	message->fromIdx = pc->idx;
	message->fromType = ENTITY_PC;
	strncpy( message->fromNameV2, pc->name,NAMESTRING );
	message->fromNameV2[NAMESTRING]=NULL;

	message->toType = msginfo->targetType;
	message->toIdx = msginfo->targetIdx;

	message->sendTime = g_globalTime;

	
	
	
	
	if( msginfo->attack.etcInfo == MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY )
	{
		message->receiveTime = g_globalTime + msginfo->attack.etcData0;
	}
	else
	{
		message->receiveTime = g_globalTime + msginfo->attack.delayTime;
	}

	message->worldIdx = pc->worldIdx;
	message->repeatNum = atkinfo->repeatNum;
	message->repeatDelayTime = atkinfo->repeatDelayTime;

	
	d = atkinfo->phyMaxDmg - atkinfo->phyMinDmg + 1;
	message->data[MSG_ATKDATA_PHYSIC_DMG] = GTH_Randomize( d ) + atkinfo->phyMinDmg;	
	
	d = atkinfo->posMaxDmg - atkinfo->posMinDmg + 1;
	message->data[MSG_ATKDATA_POSITIVE_DMG] = GTH_Randomize( d ) + atkinfo->posMinDmg;	
	message->data[MSG_ATKDATA_POSITIVE_DMG] += (int)( (float)message->data[MSG_ATKDATA_POSITIVE_DMG] * pc->optValue[OV_SKILL_DMG_RATE] * 0.01f );

	d = atkinfo->negMaxDmg - atkinfo->negMinDmg + 1;
	message->data[MSG_ATKDATA_NEGATIVE_DMG] = GTH_Randomize( d ) + atkinfo->negMinDmg;	
	message->data[MSG_ATKDATA_NEGATIVE_DMG] += (int)( (float)message->data[MSG_ATKDATA_NEGATIVE_DMG] * pc->optValue[OV_SKILL_DMG_RATE] * 0.01f );

	message->data[MSG_ATKDATA_WEAPON_TYPE] = 0;

	
	message->data[MSG_ATKDATA_ATTACK_TYPE] = atkinfo->atkType;
	
	message->data[MSG_ATKDATA_HITPOWER] = atkinfo->hitPower;
	message->data[MSG_ATKDATA_HITRATE] = atkinfo->hitRate;

	if( atkinfo->atkType == MSGINFO_ATK_TYPE_NORMAL )
	{
		
		message->data[MSG_ATKDATA_CRITICAL_RATE] = atkinfo->criticalRate;
		
		message->data[MSG_ATKDATA_CRITICAL_DAMAGE] = atkinfo->criticalDamage;
	}
	else
	{
		message->data[MSG_ATKDATA_CRITICAL_RATE] = 0;
		message->data[MSG_ATKDATA_CRITICAL_DAMAGE] = 0;
	}
	message->data[MSG_ATKDATA_STUN_FLAG] = 0;
	message->data[MSG_ATKDATA_ETC_INFO] = atkinfo->etcInfo;
	message->data[MSG_ATKDATA_ETC_DATA_0] = atkinfo->etcData0;
	message->data[MSG_ATKDATA_ETC_DATA_1] = atkinfo->etcData1;
}

void AI_MakeMsgInfoAffect( playerCharacter_t *pc, message_t* message, messageInfo_t* msginfo )
{
	MSGINFO_AFFECT* affinfo = &msginfo->affect;
	int attackType = 0;

	message->type = MSG_AFFECT;
	message->fromIdx = pc->idx;
	message->fromType = ENTITY_PC;
	strncpy( message->fromNameV2, pc->name,NAMESTRING);
	message->fromNameV2[NAMESTRING]=NULL;

	message->toType = msginfo->targetType;
	message->toIdx = msginfo->targetIdx;

	message->sendTime = g_globalTime;
	message->receiveTime = affinfo->startTime;


	message->worldIdx = pc->worldIdx;
	message->repeatNum = 0;

	
	
	pc->curSA += pc->optValue[OV_DECSA_USE];
	pc->curRA += pc->optValue[OV_DECRA_USE];

	message->data[MSG_AFFDATA_AFFECT_TYPE] = affinfo->type;
	message->data[MSG_AFFDATA_AFFECT_HITTYPE] = affinfo->affectType;

	message->data[MSG_AFFDATA_LEVEL] = affinfo->level;
	message->data[MSG_AFFDATA_KEEPTIME] = affinfo->keepTime;
	message->data[MSG_AFFDATA_SKILLTABLEIDX] = affinfo->skillTableIdx;
	message->data[MSG_AFFDATA_HITRATE] = affinfo->hitRate;

	if( affinfo->affectType == MSGINFO_AFF_TYPE_NORMAL )
		message->data[MSG_AFFDATA_HITRATE] = 100;

	
	if( affinfo->type == AFFECT_TOGGLE )
	{
		message->data[MSG_AFFDATA_RACOST_PER_SEC] = affinfo->raCostPerSec;
		message->data[MSG_AFFDATA_SACOST_PER_SEC] = affinfo->saCostPerSec;
		message->data[MSG_AFFDATA_DELAY_TIME]	  = affinfo->delayTime;
	}
	
	for( int i = 0; i < MAX_COUNT_AFFECT_DATA; i ++ )
	{
		message->data[i + CONVERT_MSG_TO_AFFECT_SIZE] = affinfo->data[i];
	}
}

void AI_MakeMessage( monsterCharacter_t* monster, message_t* message, int type, messageInfo_t* msginfo )
{
	switch( type )
	{
	case MESSAGE_NORMAL_ATTACK :
		AI_MakeAttackMessage( monster, message );
		break;
	
	case MESSAGE_USEINFO :
		if	   ( msginfo->infoType == MSGINFO_TYPE_ATTACK )	AI_Make_Monster_MsgInfoAttack( monster, message, msginfo );
		else if( msginfo->infoType == MSGINFO_TYPE_AFFECT ) AI_Make_Monster_MsgInfoAffect( monster, message, msginfo );
		break;
	}
}


void AI_MakeAttackMessage( monsterCharacter_t* monster, message_t* message  )
{
	
	int trainMonsterAddDmg[8] = { 0, 0, 0, 0, 5, 15, 25, 50 };
	int trainMonsterAddHitRate[8] = { 0, 0, 0, 0, 30, 50, 70, 90 };

	int d, attackType = 0, criticalRate = -1;


	message->type = MSG_DAMEGED;

	message->fromIdx = monster->idx;
	message->fromType = monster->entityType;
	strncpy( message->fromNameV2, monster->basic.name,NAMESTRING);
	message->fromNameV2[NAMESTRING]=NULL;

	message->toType = monster->targetType;
	message->toIdx = monster->targetIdx;
	message->worldIdx = monster->worldIdx;

	message->sendTime = g_globalTime;
	
	message->receiveTime = g_globalTime + (int)( (float)monster->calAttackDelay * 0.3f );

	message->repeatNum = 0;

	
	d = monster->calPhyMaxDmg - monster->calPhyMinDmg + 1;
	message->data[MSG_ATKDATA_PHYSIC_DMG] = GTH_Randomize( d ) + monster->calPhyMinDmg;		
	d = monster->calPosMaxDmg - monster->calPosMinDmg + 1;
	message->data[MSG_ATKDATA_POSITIVE_DMG] = GTH_Randomize( d ) + monster->calPosMinDmg;	
	d = monster->calNegMaxDmg - monster->calNegMinDmg + 1;
	message->data[MSG_ATKDATA_NEGATIVE_DMG] = GTH_Randomize( d ) + monster->calNegMinDmg;	
	message->data[MSG_ATKDATA_WEAPON_TYPE] = 0;
	message->data[MSG_ATKDATA_HITRATE] = 0;

	
	message->data[MSG_ATKDATA_ATTACK_TYPE] = MSGINFO_ATK_TYPE_NORMAL;

	message->data[MSG_ATKDATA_CRITICAL_RATE] = 0;
	
	message->data[MSG_ATKDATA_HITPOWER ] = 0;
	

	
	message->data[MSG_ATKDATA_CRITICAL_DAMAGE] = 0;
	message->data[MSG_ATKDATA_STUN_FLAG] = 0;
	
	
	
	if( message->toType == ENTITY_PC )
	{
		if( g_pc[message->toIdx].numTrainMonster > 0 && g_pc[message->toIdx].numTrainMonster <= MAX_COUNT_TRAIN_MONSTER )
		{
			message->data[MSG_ATKDATA_HITRATE] += trainMonsterAddHitRate[g_pc[message->toIdx].numTrainMonster - 1];
			message->data[MSG_ATKDATA_PHYSIC_DMG] += trainMonsterAddDmg[g_pc[message->toIdx].numTrainMonster - 1];
		}	
	}
}



void AI_Make_Monster_MsgInfoAttack( monsterCharacter_t *pMonster, message_t* message, messageInfo_t* msginfo )
{
	MSGINFO_ATTACK* atkinfo = &msginfo->attack;
	int d, attackType = 0, criticalRate = -1;

	message->type		= MSG_DAMEGED;
	message->fromIdx	= pMonster->idx;
	message->fromType	= ENTITY_MONSTER;
	message->toType		= msginfo->targetType;
	message->toIdx		= msginfo->targetIdx;
	message->sendTime	= g_globalTime;

	strncpy( message->fromNameV2, pMonster->basic.name,NAMESTRING);
	message->fromNameV2[NAMESTRING]=NULL;


	
	message->receiveTime	 = g_globalTime + atkinfo->delayTime;
	message->worldIdx		 = pMonster->worldIdx;
	message->repeatNum		 = atkinfo->repeatNum;
	message->repeatDelayTime = atkinfo->repeatDelayTime;

	
	d = atkinfo->phyMaxDmg - atkinfo->phyMinDmg + 1;
	message->data[MSG_ATKDATA_PHYSIC_DMG] = GTH_Randomize( d ) + atkinfo->phyMinDmg;	
	
	d = atkinfo->posMaxDmg - atkinfo->posMinDmg + 1;
	message->data[MSG_ATKDATA_POSITIVE_DMG] = GTH_Randomize( d ) + atkinfo->posMinDmg;	

	d = atkinfo->negMaxDmg - atkinfo->negMinDmg + 1;
	message->data[MSG_ATKDATA_NEGATIVE_DMG] = GTH_Randomize( d ) + atkinfo->negMinDmg;	

	message->data[MSG_ATKDATA_WEAPON_TYPE] = 0;

	
	message->data[MSG_ATKDATA_ATTACK_TYPE] = atkinfo->atkType;

	
	
	message->data[MSG_ATKDATA_HITPOWER] = atkinfo->hitPower;
	message->data[MSG_ATKDATA_HITRATE] = atkinfo->hitRate;

	message->data[MSG_ATKDATA_HITRATE] = atkinfo->hitPower;

	if( atkinfo->atkType == MSGINFO_ATK_TYPE_NORMAL )
	{
		message->data[MSG_ATKDATA_CRITICAL_RATE] = atkinfo->criticalRate;
		message->data[MSG_ATKDATA_CRITICAL_DAMAGE] = atkinfo->criticalDamage;
	}
	else
	{
		message->data[MSG_ATKDATA_CRITICAL_RATE] = 0;
		message->data[MSG_ATKDATA_CRITICAL_DAMAGE] = 0;
	}
	message->data[MSG_ATKDATA_STUN_FLAG] = 0;
	message->data[MSG_ATKDATA_ETC_INFO] = atkinfo->etcInfo;
	message->data[MSG_ATKDATA_ETC_DATA_0] = atkinfo->etcData0;
	message->data[MSG_ATKDATA_ETC_DATA_1] = atkinfo->etcData1;

	
	
	message->data[MSG_ATKDATA_ETC_DATA_2] = msginfo->infoType;
}


void AI_Make_Monster_MsgInfoAffect( monsterCharacter_t *pMonster, message_t* message, messageInfo_t* msginfo )
{
	MSGINFO_AFFECT* affinfo = &msginfo->affect;
	int attackType = 0;

	message->type = MSG_AFFECT;
	message->fromIdx = pMonster->idx;
	message->fromType = ENTITY_MONSTER;
	
	strncpy( message->fromNameV2, pMonster->basic.name,NAMESTRING);
	message->fromNameV2[NAMESTRING]=NULL;

	message->toType = msginfo->targetType;
	message->toIdx = msginfo->targetIdx;

	message->sendTime = g_globalTime;
	message->receiveTime = g_globalTime;
	message->worldIdx = pMonster->worldIdx;
	message->repeatNum = 0;

	message->data[MSG_AFFDATA_AFFECT_TYPE]		= affinfo->type;
	message->data[MSG_AFFDATA_AFFECT_HITTYPE]	= affinfo->affectType;

	message->data[MSG_AFFDATA_LEVEL] = affinfo->level;
	message->data[MSG_AFFDATA_KEEPTIME] = affinfo->keepTime;
	message->data[MSG_AFFDATA_SKILLTABLEIDX] = affinfo->skillTableIdx;
	message->data[MSG_AFFDATA_HITRATE] = affinfo->hitRate;

	if( affinfo->affectType == MSGINFO_AFF_TYPE_NORMAL )
		message->data[MSG_AFFDATA_HITRATE] = 100;
	
	for( int i = 0; i < MAX_COUNT_AFFECT_DATA; i ++ )
	{
		message->data[i + CONVERT_MSG_TO_AFFECT_SIZE] = affinfo->data[i];
	}
}
