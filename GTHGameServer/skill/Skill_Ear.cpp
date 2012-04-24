#include "../global.h"

#ifdef _OLD_SKILL_VER



void	SKILL_Blow( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg, hitPro, decDefensePro, decDefenseRate, decDefenseTime, phybonus;
	messageInfo_t	msginfo;
	message_t		message;

	dmg				=	pValue[SKILL_VALUE_0];
	hitPro			=	pValue[SKILL_VALUE_1];
	decDefensePro	=	pValue[SKILL_VALUE_2];
	decDefenseRate	=	pValue[SKILL_VALUE_3];
	decDefenseTime	=	pValue[SKILL_VALUE_4];
	phybonus		=	pValue[SKILL_VALUE_5];
	
	if( pc->atkWeaponType == WEAPON_TYPE_CLAW ) dmg += 30.0f;

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	hitPro	*	0.01f	);
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calMagMinDmg	+	phybonus);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calMagMaxDmg	+	phybonus);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	if( pc->atkWeaponType == WEAPON_TYPE_SPIRITOR )
	{
		memset( &msginfo, 0, sizeof( messageInfo_t ) );
		
		msginfo.infoType				=	MSGINFO_TYPE_AFFECT;
		msginfo.affect.skillTableIdx	=	15;						
		msginfo.affect.level			=	g_affectSkillLevel;		
		msginfo.affect.type				=	AFFECT_CURSE;
		msginfo.affect.affectType		=	MSGINFO_AFF_TYPE_POSITIVE;
		msginfo.affect.startTime		=	g_globalTime;
		msginfo.affect.keepTime			=	g_globalTime	+	(int)decDefenseTime	*	1000;
		msginfo.affect.hitRate			=	(int)decDefensePro;
		msginfo.targetType				=	pc->targetType;
		msginfo.targetIdx				=	pc->targetIdx;
		
		msginfo.affect.common.type[0]	=	AFFDATA_TYPE_DEFENSE_RATE;
		msginfo.affect.common.data[0]	=	-(int)decDefenseRate;
		
		AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
		AI_SendMessage( &message );
	}
}


void	SKILL_AttractAttention(void *pPc, float *pValue)
{
	float	hitPro, level, panelty;

	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);

	if( g_monster[pc->targetIdx].aliveState != MON_ALIVE_STATE_NORMAL )	return;

	level = fabs((float)(g_monster[pc->targetIdx].basic.level - pc->level));

	if( level > 33.0f )	level = 33.0f;

	panelty = 1.0f - level * 0.03f;
	
	hitPro = pValue[SKILL_VALUE_0] * panelty;

	if( GTH_Random() < hitPro )
	{
		g_monster[pc->targetIdx].targetIdx = pc->idx;
		g_monster[pc->targetIdx].targetType = ENTITY_PC;

		
		g_monster[pc->targetIdx].ai.callProcedureAddr = g_monster[pc->targetIdx].ai.aiProcedureAddr[1];
		g_monster[pc->targetIdx].ai.aiAddr = g_monster[pc->targetIdx].ai.aiProcedureAddr[1];
		g_monster[pc->targetIdx].ai.isCall = true;
		
		
		AI_StopPathFinding( &g_monster[pc->targetIdx] );
		AI_InterpretImmediately( &g_monster[pc->targetIdx] );
	}
	else
	{
		
	}
}


void	SKILL_MightyBlow(void *pPc, float *pValue)
{
	playerCharacter_t* pc = static_cast<playerCharacter_t*>(pPc);

	float			dmg, pushrate, phybonus;
	messageInfo_t	msginfo;
	message_t		message;

	dmg			= pValue[SKILL_VALUE_0];	
	pushrate	= pValue[SKILL_VALUE_1];
	phybonus	= pValue[SKILL_VALUE_2];

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calMagMinDmg	+	phybonus);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calMagMaxDmg	+	phybonus);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;


	msginfo.attack.etcInfo = MSGINFO_ATK_ECT_INFO_PUSHED;
	msginfo.attack.etcData0 = 700;
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	float	level, panelty;

	level = (float)(pc->level - g_monster[pc->targetIdx].basic.level);
	if( level < 0.0f )	level = 0.0f;

	panelty = 1.0f - 0.03 * level;
	
	pushrate = pValue[SKILL_VALUE_1] * 0.01f * panelty;

}



void	SKILL_TrainSpiritor( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg;

	dmg					=	pValue[SKILL_VALUE_0];
	
	

	pc->calPhyMinDmg	+=	dmg;	
	pc->calPhyMaxDmg	+=	dmg;	
}


void	SKILL_TrainClaw( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg;
	

	dmg					=	pValue[SKILL_VALUE_0];
	
	

	pc->calPhyMinDmg	+=	dmg;	
	pc->calPhyMaxDmg	+=	dmg;	
}



void	SKILL_CrossClaw(void *pPc, float *pValue)
{
	playerCharacter_t*	pc = static_cast<playerCharacter_t*>(pPc);

	float			dmg, hitPro, dmg1, phybonus;
	int				count;
	messageInfo_t	msginfo;
	message_t		message;

	dmg			= pValue[SKILL_VALUE_0];		
	dmg1		= pValue[SKILL_VALUE_2];		
	count		= (int)pValue[SKILL_VALUE_3];
	phybonus	= pValue[SKILL_VALUE_4];

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	int level = (pc->level - g_monster[pc->targetIdx].basic.level);
	if( level < 0 )	level = 0;

	float panelty = 1.0f - 0.03 * (float)level;

	hitPro = pValue[SKILL_VALUE_1] * 0.01 * panelty;

	dmg1 = dmg1 * (1.0f - (float)g_monster[pc->targetIdx].basic.defPower * 0.01f) - (float)g_monster[pc->targetIdx].basic.defDamageReduce;
	if( dmg1 < 1.0f )	dmg1 = 1.0f;

	
	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitRate			=	(int)hitPro;

	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calMagMinDmg	+	phybonus);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calMagMaxDmg	+	phybonus);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay	/	2;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	
	

	
	dmg1		= pValue[SKILL_VALUE_2];
	for(int i = 0; i < count; i++)
	{
		dmg1 = dmg1 * (1.0f - (float)g_monster[pc->targetIdx].basic.defPower * 0.01f) - (float)g_monster[pc->targetIdx].basic.defDamageReduce;
		if( dmg1 < 1.0f )	dmg1 = 1.0f;

		msginfo.attack.phyMinDmg		=	dmg1;
		msginfo.attack.phyMaxDmg		=	dmg1;
		msginfo.attack.etcInfo			= MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
		msginfo.attack.etcData0			+= 500;
		
		AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
		AI_SendMessage( &message );
	}
}


void	SKILL_IncreaseAttack1(void* pPc, float* pValue)
{
	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pc == NULL )	return;

	float			dmg, RaUse, RaDec;
	message_t		message;
	messageInfo_t	msginfo;

	dmg		= pValue[SKILL_VALUE_0];
	RaUse	= pValue[SKILL_VALUE_1];
	RaDec	= pValue[SKILL_VALUE_2];

	memset( &msginfo, 0, sizeof(messageInfo_t) );
	memset( &message, 0, sizeof( message_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_AFFECT;

	msginfo.affect.skillTableIdx	=	33;						
	msginfo.affect.level			=	g_affectSkillLevel;		

	msginfo.targetType				=	ENTITY_PC;
	msginfo.targetIdx				=	pc->idx;

	msginfo.affect.type				=	AFFECT_TOGGLE;
	msginfo.affect.affectType		=	MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		=	g_globalTime;

	
	
	
	
	msginfo.affect.delayTime	= 1000;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_DECREASERA;
	msginfo.affect.common.data[0] = (int)RaUse;

	msginfo.affect.common.type[1] = AFFDATA_TYPE_PHYDAMAGE;
	msginfo.affect.common.data[1] = (int)dmg;

	
	msginfo.affect.common.type[2] = AFFDATA_TYPE_INCRARECOVERY;
	msginfo.affect.common.data[2] = -(int)RaDec;
	
	

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

}

void	SKILL_EAR_PolymorphMimin( void* pPc, float *pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);	
	
	float addmoveSpeed	= pValue[SKILL_VALUE_0];
	float RaUse			= pValue[SKILL_VALUE_1];
	float keepTime		= pValue[SKILL_VALUE_2] * 1000.0f;


	messageInfo_t msginfo;
	message_t message;
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx	= 34;					
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_SPECIAL;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;
	msginfo.affect.spe.specialType	= 0;
	msginfo.affect.spe.specialIdx	= 34;
	
	msginfo.affect.spe.specialData[0] = (int)addmoveSpeed;
	msginfo.affect.spe.specialData[1] = RaUse;

	msginfo.targetType	= ENTITY_PC;
	msginfo.targetIdx	= pc->idx;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	
	GTH_DeleteAffect( pc, ENTITY_PC, 43 );
}



void	SKILL_MaxRAIncrease(void* pPc, float* pValue)
{
	float			maxRaInc, keepTime;
	messageInfo_t	msgInfo;
	message_t		message;

	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pc == NULL )	return;

	maxRaInc = pValue[SKILL_VALUE_0];
	keepTime = pValue[SKILL_VALUE_1] * 1000.0f;

	memset( &msgInfo, 0, sizeof(messageInfo_t) );

	msgInfo.infoType				= MSGINFO_TYPE_AFFECT;
	msgInfo.affect.skillTableIdx	= 54;
	msgInfo.affect.level			= g_affectSkillLevel;
	msgInfo.affect.type				= AFFECT_ASSIST;
	msgInfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msgInfo.affect.startTime		= g_globalTime;
	msgInfo.affect.keepTime			= g_globalTime + (int)keepTime;
	msgInfo.targetType				= ENTITY_PC;
	msgInfo.targetIdx				= pc->idx;

	msgInfo.affect.common.type[0]	= AFFDATA_TYPE_MAXRA;
	msgInfo.affect.common.data[0]	= (int)maxRaInc;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msgInfo );
	AI_SendMessage( &message );
}



void	SKILL_StunAttack(void* pPc, float* pValue)
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg, hitPro, phyBonus, stunPro, stunTime, cursePro, curseTime;
	int				curseType;
	messageInfo_t	msginfo;
	message_t		message;

	hitPro		=	pValue[SKILL_VALUE_0];
	dmg			=	pValue[SKILL_VALUE_1];
	phyBonus	=	pValue[SKILL_VALUE_2];
	stunPro		=	pValue[SKILL_VALUE_3];
	stunTime	=	pValue[SKILL_VALUE_4];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType			=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType		=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower		=	(int)( (float)pc->calHitPower  * hitPro * 0.01f );
	msginfo.attack.phyMinDmg	=	(int)( (float)pc->calPhyMinDmg * ( 1.0f	+ dmg *	0.01f )	+ phyBonus + pc->calMagMinDmg );
	msginfo.attack.phyMaxDmg	=	(int)( (float)pc->calPhyMinDmg * ( 1.0f	+ dmg *	0.01f )	+ phyBonus + pc->calMagMinDmg );
	msginfo.attack.delayTime	=	pc->calAttackDelay;
	msginfo.targetType			=	pc->targetType;
	msginfo.targetIdx			=	pc->targetIdx;

	curseType					=	MSGINFO_ATK_ETC_INFO_STUN;
	cursePro					=	(int)stunPro;
	curseTime					=	stunTime	*	1000.0f;

	
	msginfo.attack.etcInfo		=	curseType;
	msginfo.attack.etcData0		=	cursePro;
	msginfo.attack.etcData1		=	curseTime;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}


void	SKILL_EAR_PolymorphRifle( void* pPc, float *pValue )
{
	float	range, addAttInc, phyBonus, raUse, hitPowInc, avoidDec, keepTime;
	messageInfo_t msginfo;
	message_t message;

	playerCharacter_t *pc = NULL;
	pc = static_cast<playerCharacter_t*>(pPc);
	if( pc == NULL )	return;
	
	range			= pValue[SKILL_VALUE_0];				
	addAttInc		= pValue[SKILL_VALUE_1];				
	phyBonus		= pValue[SKILL_VALUE_2];				
	raUse			= pValue[SKILL_VALUE_3];				
	hitPowInc		= pValue[SKILL_VALUE_4];				
	avoidDec		= pValue[SKILL_VALUE_5];				
	keepTime		= pValue[SKILL_VALUE_6] * 1000.0f;		

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx	= 57;					
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_SPECIAL;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;
	msginfo.affect.spe.specialType	= 0;
	msginfo.affect.spe.specialIdx	= 57;
	
	msginfo.affect.spe.specialData[0] = addAttInc;
	msginfo.affect.spe.specialData[1] = phyBonus;
	msginfo.affect.spe.specialData[2] = raUse;
	msginfo.affect.spe.specialData[3] = hitPowInc;
	msginfo.affect.spe.specialData[4] = avoidDec;

	msginfo.targetType	= ENTITY_PC;
	msginfo.targetIdx	= pc->idx;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void	SKILL_EAR_PolymorphFast( void* pPc, float *pValue )
{
	float	addAttInc, phyBonus, raUse, hitPowInc, avoidInc, keepTime, attSpeed;
	messageInfo_t msginfo;
	message_t message;

	playerCharacter_t *pc = NULL;
	pc = static_cast<playerCharacter_t*>(pPc);
	if( pc == NULL )	return;
	
	addAttInc		= pValue[SKILL_VALUE_0];				
	phyBonus		= pValue[SKILL_VALUE_1];				
	raUse			= pValue[SKILL_VALUE_2];				
	hitPowInc		= pValue[SKILL_VALUE_3];				
	avoidInc		= pValue[SKILL_VALUE_4];				
	attSpeed		= pValue[SKILL_VALUE_5];				
	keepTime		= pValue[SKILL_VALUE_6] * 1000.0f;		

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx	= 58;					
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_SPECIAL;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;
	msginfo.affect.spe.specialType	= 0;
	msginfo.affect.spe.specialIdx	= 58;
	
	msginfo.affect.spe.specialData[0] = addAttInc;
	msginfo.affect.spe.specialData[1] = phyBonus;
	msginfo.affect.spe.specialData[2] = raUse;
	msginfo.affect.spe.specialData[3] = hitPowInc;
	msginfo.affect.spe.specialData[4] = avoidInc;
	msginfo.affect.spe.specialData[5] = attSpeed;

	msginfo.targetType	= ENTITY_PC;
	msginfo.targetIdx	= pc->idx;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
	
	
}





void SKILL_Plymoffpower( void* pPc, float *pValue )
{
	float	addAttInc, hitPowInc, avoidInc, keepTime, IncVelocity;
	
	messageInfo_t msginfo;
	message_t message;

	playerCharacter_t *pc = NULL;
	pc = static_cast<playerCharacter_t*>(pPc);
	if( pc == NULL )	return;
	
	addAttInc		= pValue[SKILL_VALUE_0];				
	hitPowInc		= pValue[SKILL_VALUE_1];				
	avoidInc		= pValue[SKILL_VALUE_2];				
	IncVelocity		= pValue[SKILL_VALUE_3];				
	keepTime		= pValue[SKILL_VALUE_4] * 1000.0f;		

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx	= 62;					
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_SPECIAL;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;
	msginfo.affect.spe.specialType	= 0;
	msginfo.affect.spe.specialIdx	= 62;						
	
	msginfo.affect.spe.specialData[0] = addAttInc;
	msginfo.affect.spe.specialData[1] = hitPowInc;
	msginfo.affect.spe.specialData[2] = avoidInc;	
	msginfo.affect.spe.specialData[3] = IncVelocity;

	msginfo.targetType	= ENTITY_PC;
	msginfo.targetIdx	= pc->idx;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );	
	
	

}





void SKILL_Defenceup(void* pPc, float* pValue)
{
	float	decVel, keepTime, incDefenseRate, incDefensvar;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	incDefenseRate					=  pValue[SKILL_VALUE_0];			
	incDefensvar					=  pValue[SKILL_VALUE_1];			
	decVel							= -pValue[SKILL_VALUE_2];			
	keepTime						=  pValue[SKILL_VALUE_3] * 1000.0f;		

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;		
	
	msginfo.affect.skillTableIdx	= 56;
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_TOGGLE;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;
	msginfo.affect.delayTime		= 1000;

	msginfo.targetType				= ENTITY_PC;
	msginfo.targetIdx				= pc->idx;

	msginfo.affect.common.type[0]	= AFFDATA_TYPE_DEFENSE_RATE;
	msginfo.affect.common.data[0]	= (int)incDefenseRate;

	msginfo.affect.common.type[1]	= AFFDATA_TYPE_DEFENSE;
	msginfo.affect.common.data[1]	= (int)incDefensvar;

	msginfo.affect.common.type[2]	= AFFDATA_TYPE_VELOCITY_RATE;
	msginfo.affect.common.data[2]	= (int)decVel;


	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}


#endif