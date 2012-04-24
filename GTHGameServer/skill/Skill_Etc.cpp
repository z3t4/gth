 #include "../global.h"

#ifdef _OLD_SKILL_VER



void SKILL_ETC_WhiteDayEventPhyDmg( void* pPc, float* pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	

	float addDmg;
	messageInfo_t msginfo;
	message_t message;

	addDmg = pValue[SKILL_VALUE_0];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;

	msginfo.affect.skillTableIdx = 16;
	msginfo.affect.level = g_affectSkillLevel;

	msginfo.affect.type = AFFECT_ATTACK;
	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime = g_globalTime;
	
	msginfo.affect.keepTime = g_globalTime + 180000;
	msginfo.targetType = ENTITY_PC;
	msginfo.targetIdx = pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_PHYDAMAGE;
	msginfo.affect.common.data[0] = (int)addDmg;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}




void SKILL_ETC_WhiteDayEventHitPower( void* pPc, float* pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	

	float addHitPower;
	messageInfo_t msginfo;
	message_t message;

	addHitPower = pValue[SKILL_VALUE_0];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;

	msginfo.affect.skillTableIdx = 21;
	msginfo.affect.level = g_affectSkillLevel;

	msginfo.affect.type = AFFECT_ATTACK;
	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime = g_globalTime;
	
	msginfo.affect.keepTime = g_globalTime + 180000;
	msginfo.targetType = ENTITY_PC;
	msginfo.targetIdx = pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_HITPOWER;
	msginfo.affect.common.data[0] = (int)addHitPower;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void SKILL_ETC_WhiteDayEventMagDmg( void* pPc, float* pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	

	float addDmg;
	messageInfo_t msginfo;
	message_t message;

	addDmg = pValue[SKILL_VALUE_0];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;

	msginfo.affect.skillTableIdx = 41;
	msginfo.affect.level = g_affectSkillLevel;

	msginfo.affect.type = AFFECT_ATTACK;
	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime = g_globalTime;
	
	msginfo.affect.keepTime = g_globalTime + 180000;
	msginfo.targetType = ENTITY_PC;
	msginfo.targetIdx = pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_MAGDAMAGE;
	msginfo.affect.common.data[0] = (int)addDmg;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void SKILL_ETC_MedicineOFAtkSpeed( void* pPc, float* pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	

	
	float atkSpeed, keeptime, sktidx;

	
	messageInfo_t msginfo;
	message_t message;

	atkSpeed					= pValue[SKILL_VALUE_0];
	sktidx						= 67;
	keeptime					= pValue[SKILL_VALUE_1];	


	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx = sktidx;
	msginfo.affect.level		= g_affectSkillLevel;

	msginfo.affect.type			= AFFECT_ATTACK;
	msginfo.affect.affectType	= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime	= g_globalTime;	
	
	msginfo.affect.keepTime		= g_globalTime + (keeptime * 1000 );
	msginfo.targetType			= pc->entityType;
	msginfo.targetIdx			= pc->idx;	

	msginfo.affect.common.type[0] = AFFDATA_TYPE_ATTACKSPEED;
	msginfo.affect.common.data[0] = atkSpeed;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void SKILL_ETC_MedicineOFVelocity( void* pPc, float* pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	

	float Velocity, keeptime, sktidx, initvar;
	messageInfo_t msginfo;
	message_t message;

	Velocity					= pValue[SKILL_VALUE_0];
	sktidx						= 68;
	initvar						= pValue[3] * 100;
	keeptime					= pValue[4];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;

	
	msginfo.affect.skillTableIdx = sktidx;
	msginfo.affect.level = g_affectSkillLevel;

	msginfo.affect.type			= AFFECT_ATTACK;
	msginfo.affect.affectType	= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime	= g_globalTime;	
	msginfo.affect.keepTime		= g_globalTime + ( keeptime * 1000 );	
	msginfo.targetType			= ENTITY_PC;
	msginfo.targetIdx			= pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_VELOCITY;
	msginfo.affect.common.data[0] = (int)initvar ;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}




void SKILL_ETC_MedicineOFSpeedAndAttack( void* pPc, float* pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	

	float Velocity, atkSpeed, keeptime, sktidx, initvar;
	messageInfo_t msginfo;
	message_t message;

	atkSpeed					= pValue[SKILL_VALUE_0];
	Velocity					= pValue[SKILL_VALUE_1];
	sktidx						= 69;
	initvar						= Velocity * 100;
	keeptime					= pValue[SKILL_VALUE_2];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;

	
	msginfo.affect.skillTableIdx = sktidx;
	msginfo.affect.level = g_affectSkillLevel;

	msginfo.affect.type			= AFFECT_ATTACK;
	msginfo.affect.affectType	= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime	= g_globalTime;	
	msginfo.affect.keepTime		= g_globalTime + ( keeptime * 1000 );	
	
	msginfo.targetType			= pc->entityType;
	msginfo.targetIdx			= pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_VELOCITY;
	msginfo.affect.common.data[0] = (int)initvar ;

	msginfo.affect.common.type[1] = AFFDATA_TYPE_ATTACKSPEED;
	msginfo.affect.common.data[1] = atkSpeed;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void SKILL_ETC_MedicineOFAtkSpeed_China( void* pPc, float* pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	
	
	float atkSpeed, keeptime, sktidx;

	
	messageInfo_t msginfo;
	message_t message;

	atkSpeed					= pValue[SKILL_VALUE_0];
	sktidx						= 70;
	keeptime					= pValue[SKILL_VALUE_1];	


	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx = sktidx;
	msginfo.affect.level		= g_affectSkillLevel;

	msginfo.affect.type			= AFFECT_ATTACK;
	msginfo.affect.affectType	= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime	= g_globalTime;	
	
	msginfo.affect.keepTime		= g_globalTime + (keeptime * 1000 );
	msginfo.targetType			= pc->entityType;
	msginfo.targetIdx			= pc->idx;	

	msginfo.affect.common.type[0] = AFFDATA_TYPE_ATTACKSPEED;
	msginfo.affect.common.data[0] = atkSpeed;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void SKILL_ETC_MedicineOFVelocity_China( void* pPc, float* pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	

	float Velocity, keeptime, sktidx, initvar;
	messageInfo_t msginfo;
	message_t message;

	Velocity					= pValue[SKILL_VALUE_0];
	sktidx						= 71;
	initvar						= Velocity * 100;
	keeptime					= pValue[SKILL_VALUE_1];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;

	
	msginfo.affect.skillTableIdx = sktidx;
	msginfo.affect.level = g_affectSkillLevel;

	msginfo.affect.type			= AFFECT_ATTACK;
	msginfo.affect.affectType	= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime	= g_globalTime;	
	msginfo.affect.keepTime		= g_globalTime + ( keeptime * 1000 );	
	msginfo.targetType			= ENTITY_PC;
	msginfo.targetIdx			= pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_VELOCITY;
	msginfo.affect.common.data[0] = (int)initvar ;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}

	
#endif
