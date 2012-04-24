#include "../global.h"


#ifdef _OLD_SKILL_VER



void SKILL_ConcentrateHit( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg, hitPro, oneHandShockPro, twoHandShockPro, stunTime, cursePro, curseTime, phybonus;
	int				curseType;
	messageInfo_t	msginfo;
	message_t		message;

	dmg				=	pValue[SKILL_VALUE_0];
	hitPro			=	pValue[SKILL_VALUE_1];
	oneHandShockPro	=	pValue[SKILL_VALUE_2];
	twoHandShockPro	=	pValue[SKILL_VALUE_3];
	stunTime		=	pValue[SKILL_VALUE_4];
	phybonus		=	pValue[SKILL_VALUE_5];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	hitPro	*	0.01f	);
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMinDmg	);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMinDmg	);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;

	
	if( pc->atkWeaponType	==	WEAPON_TYPE_ONEHAND_SWORD	)
	{
		curseType					=	MSGINFO_ATK_ETC_INFO_SHOCK;
		cursePro					=	(int)oneHandShockPro;
		
		
		if(	pc->targetType	==	ENTITY_PC	) curseTime	=	g_pc[pc->targetIdx].calAttackDelay;
		else curseTime	=	g_monster[pc->targetIdx].calAttackDelay;
	}
	else if( pc->atkWeaponType	==	WEAPON_TYPE_TWOHAND_SWORD	)
	{
		curseType					=	MSGINFO_ATK_ETC_INFO_STUN;
		cursePro					=	(int)twoHandShockPro;
		curseTime					=	stunTime	*	1000.0f;
	}

	
	msginfo.attack.etcInfo			=	curseType;
	msginfo.attack.etcData0			=	cursePro;
	msginfo.attack.etcData1			=	curseTime;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}


void SKILL_ConsecutiveSlash( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
	
	float			dmg, hitPro, criticalPro, phybonus;
	messageInfo_t	msginfo;
	message_t		message;	

	dmg				=	pValue[SKILL_VALUE_0];
	hitPro			=	pValue[SKILL_VALUE_1];
	criticalPro		=	pValue[SKILL_VALUE_2];
	phybonus		=	pValue[SKILL_VALUE_3];
	
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	
	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	hitPro	*	0.01f	);

	



	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMinDmg	) / 3;
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMaxDmg	) / 3;	

	msginfo.attack.criticalRate		=	(int)criticalPro;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;


	
	

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

}


void SKILL_SpinSlash( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg, hitPro, shockPro, cursePro, curseTime, phybonus;
	int				curseType;

	messageInfo_t	msginfo;
	message_t		message;

	dmg				=	pValue[SKILL_VALUE_0];
	hitPro			=	pValue[SKILL_VALUE_1];
	shockPro		=	pValue[SKILL_VALUE_2];
	phybonus		=	pValue[SKILL_VALUE_3];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	hitPro	*	0.01f	);
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMinDmg	);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMaxDmg	);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;
	msginfo.attack.etcInfo			=	MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER;
	
	curseType						=	MSGINFO_ATK_ETC_INFO_SHOCK;
	cursePro						=	(int)shockPro;
	
	
	if(	pc->targetType	==	ENTITY_PC	) curseTime	=	g_pc[pc->targetIdx].calAttackDelay;
	else curseTime	=	g_monster[pc->targetIdx].calAttackDelay;

	




	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message, pc->calAttackLength );
}


void SKILL_TrainOneHandSword( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg;


	dmg					=	pValue[SKILL_VALUE_0];
	
	
	

	pc->calPhyMinDmg	+=	dmg;	
	pc->calPhyMaxDmg	+=	dmg;	
}


void SKILL_TrainTwoHandSword( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg;


	dmg					=	pValue[SKILL_VALUE_0];
	
	

	pc->calPhyMinDmg	+=	dmg;	
	pc->calPhyMaxDmg	+=	dmg;	
}


void	SKILL_IncreaseAttackPower(void* pPc, float* pValue)
{
	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pc == NULL ) return;

	float			incDmg, useSA;
	message_t		message;
	messageInfo_t	msginfo;
	

	incDmg	= pValue[SKILL_VALUE_0];
	useSA	= pValue[SKILL_VALUE_1];

	memset( &msginfo, 0, sizeof(messageInfo_t) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;	

	msginfo.affect.skillTableIdx = 26;
	msginfo.affect.level = g_affectSkillLevel;

	
	
	

	msginfo.affect.type = AFFECT_TOGGLE;
	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime = g_globalTime;
	msginfo.targetType = ENTITY_PC;
	msginfo.targetIdx = pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_PHYDAMAGE;
	msginfo.affect.common.data[0] = (int)incDmg;

	
	msginfo.affect.common.type[1] = AFFDATA_TYPE_INCSARECOVERY;
	msginfo.affect.common.data[1] = -(int)useSA;
	
	
	
	msginfo.affect.delayTime	= 1000;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}


void	SKILL_IncreaseRARecovery(void* pPc, float* pValue)
{
	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t*>(pPc);
	if( pc == NULL )	return;

	float			raRecInc, keepTime;
	message_t		message;
	messageInfo_t	msginfo;

	raRecInc = pValue[SKILL_VALUE_0];
	keepTime = pValue[SKILL_VALUE_1] * 1000.0f;

	memset( &msginfo, 0, sizeof(messageInfo_t) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;	
	
	msginfo.affect.skillTableIdx = 27;
	msginfo.affect.level = g_affectSkillLevel;

	msginfo.affect.type = AFFECT_ASSIST;
	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime = g_globalTime;
	msginfo.affect.keepTime = g_globalTime + (int)keepTime;

	msginfo.targetType = ENTITY_PC;
	msginfo.targetIdx = pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_INCRARECOVERY;
	msginfo.affect.common.data[0] = (int)raRecInc;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	
}






void SKILL_DisturbMental( void* pPc, float* pValue)
{
	float			hitPro, dmg, phyBonus, listPoint;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t* pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pc == NULL )	return;

	hitPro		= pValue[SKILL_VALUE_0];
	dmg			= pValue[SKILL_VALUE_1];
	phyBonus	= pValue[SKILL_VALUE_2];
	listPoint	= pValue[SKILL_VALUE_3];

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	hitPro	*	0.01f	);
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phyBonus	+	pc->calMagMinDmg	);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phyBonus	+	pc->calMagMinDmg	);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;

	if( pc->targetType == ENTITY_MONSTER )
		AddMonster_ContributionPoint(pc, pc->targetIdx, ATTACK_POINT, listPoint);	

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void SKILL_ConcentrateHitEx( void* pPc, float* pValue )
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

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	hitPro	*	0.01f	);
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phyBonus	+	pc->calMagMinDmg	);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phyBonus	+	pc->calMagMinDmg	);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;

	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;

	curseType					=	MSGINFO_ATK_ETC_INFO_STUN;
	cursePro					=	(int)stunPro;
	curseTime					=	stunTime	*	1000.0f;

	
	msginfo.attack.etcInfo			=	curseType;
	msginfo.attack.etcData0			=	cursePro;
	msginfo.attack.etcData1			=	curseTime;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}




void	SKILL_layhands( void* pPc, float* pValue )
{
	float	incVel;	
	float   RADecRate, RAIncreRate;



	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	RADecRate	= pValue[SKILL_VALUE_0];	
	RAIncreRate	= pValue[SKILL_VALUE_1];

	
	if ( pc->idx == pc->targetIdx) return;

	if ( pc->curRA <= 0)  return;
	incVel					  = pc->curRA * ( RADecRate / 100 );

	if ( incVel <= 0)  return;

	pc->curRA				  -= incVel;
	incVel					  *= RAIncreRate;
	g_pc[pc->targetIdx].curRA += incVel;

	if( g_pc[pc->targetIdx].curRA > g_pc[pc->targetIdx].calMaxRA ) 
		g_pc[pc->targetIdx].curRA = g_pc[pc->targetIdx].calMaxRA;

}




void SKILL_IncreaseMoveSpeed( void* pPc, float* pValue )			
{
	float	incVel, keepTime;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	incVel		= pValue[SKILL_VALUE_0];
	keepTime	= pValue[SKILL_VALUE_1] * 1000.0f;


	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;	
	
	msginfo.affect.skillTableIdx	= 38;
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_ASSIST;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;
	msginfo.targetType				= ENTITY_PC;
	msginfo.targetIdx				= pc->idx;

	msginfo.affect.common.type[0]	= AFFDATA_TYPE_VELOCITY_RATE;
	msginfo.affect.common.data[0]	= (int)incVel;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
	
	
	GTH_DeleteAffect( pc, ENTITY_PC, 43 );
}



void	SKILL_SoulBreaker(void* pPc, float* pValue)
{
	float	hitPro, dmg, phyBonus;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	hitPro		= pValue[SKILL_VALUE_0];
	dmg			= pValue[SKILL_VALUE_1];
	phyBonus	= pValue[SKILL_VALUE_2];

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType			=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType		=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower		=	(int)( (float)pc->calHitPower  * hitPro	* 0.01f	);
	msginfo.attack.phyMinDmg	=	(int)( (float)pc->calPhyMinDmg * ( 1.0f	+ dmg *	0.01f	)	+	phyBonus	+	pc->calMagMinDmg	);
	msginfo.attack.phyMaxDmg	=	(int)( (float)pc->calPhyMinDmg * ( 1.0f	+ dmg *	0.01f	)	+	phyBonus	+	pc->calMagMinDmg	);
	msginfo.attack.delayTime	=	pc->calAttackDelay;
	msginfo.targetType			=	pc->targetType;
	msginfo.targetIdx			=	pc->targetIdx;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

}




void	SKILL_SelfHeal(void* pPc, float* pValue)
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
	float raRecovery = pValue[SKILL_VALUE_0];	
	switch( pc->entityType )
	{	
	case ENTITY_PC :
		g_pc[pc->idx].curRA += raRecovery;
		g_pc[pc->idx].curRA += (float)raRecovery  * g_pc[pc->idx].optValue[OV_HEAL_EFFICIENCY] * 0.01f;

		if( g_pc[pc->idx].curRA > g_pc[pc->idx].calMaxRA )
			g_pc[pc->idx].curRA = g_pc[pc->idx].calMaxRA;
		break;
	}
	
}

#endif
