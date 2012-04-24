#include "../global.h"

#ifdef _OLD_SKILL_VER



void	SKILL_FastConsecutiveAttack( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
	
	float			dmg, hitPro, phybonus;
	messageInfo_t	msginfo;
	message_t		message;

	dmg				=	pValue[SKILL_VALUE_0];
	hitPro			=	pValue[SKILL_VALUE_1];
	phybonus		=	pValue[SKILL_VALUE_2];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	
	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	(	1.0f    + hitPro	*	0.01f	));

	



	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMinDmg	) / 2;
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMaxDmg	) / 2;
	

	
	
	
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;

	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;

	



















	
	

	
	if( pc->atkWeaponType == WEAPON_TYPE_MAGIC_BOW )
	{
		msginfo.attack.etcInfo			= MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
		msginfo.attack.etcData0			= (int)(1400) ;
	}

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	

	if( pc->atkWeaponType == WEAPON_TYPE_MAGIC_BOW )
	{
		msginfo.attack.etcInfo			= MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
		msginfo.attack.etcData0			+= (int)(400) ;

		AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
		AI_SendMessage( &message );
	}

}


void	SKILL_FiveConsecutiveAttack( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
	
	float			dmg, hitPro, phybonus;
	messageInfo_t	msginfo;
	message_t		message;

	dmg				=	pValue[SKILL_VALUE_0];
	hitPro			=	pValue[SKILL_VALUE_1];
	phybonus		=	pValue[SKILL_VALUE_2];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	
	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	hitPro	*	0.01f	);

	


	
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calPhyMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMinDmg	) / 2;
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calPhyMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	phybonus	+	pc->calMagMaxDmg	) / 2;
	
	

	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;

	
	msginfo.attack.etcInfo			= MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
	msginfo.attack.etcData0			= 2500;
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	
	msginfo.attack.hitPower			=	(int)(	(float)pc->calHitPower	*	hitPro	*	0.01f	);

	
	for(int i = 0; i < 4; i++)
	{
		
		msginfo.attack.etcInfo			= MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
		msginfo.attack.etcData0			+= (int)(300);
		
		AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
		AI_SendMessage( &message );
	}

}


void	SKILL_MomentNearing( void *pPc, float *pValue )
{
	playerCharacter_t *pc = static_cast<playerCharacter_t *>(pPc);

	float			dmg, hitPro, phybonus, criPro;
	float			hitdecPro, hitdecTime;

	messageInfo_t	msginfo;
	message_t		message;

	dmg			= pValue[SKILL_VALUE_0];
	phybonus	= pValue[SKILL_VALUE_1];
	criPro		= pValue[SKILL_VALUE_2];
	hitdecPro	= pValue[SKILL_VALUE_3];		
	hitdecTime	= pValue[SKILL_VALUE_4];		
	hitPro		= pValue[SKILL_VALUE_5];

	memset( &msginfo, 0, sizeof(messageInfo_t) );

	msginfo.infoType				= MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			= MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			= (int)((float)pc->calHitPower * hitPro * 0.01f);
	
	msginfo.attack.phyMinDmg		= (int)((float)pc->calPhyMinDmg + (float)pc->calPhyMinDmg * hitPro * 0.01 + phybonus + (float)pc->calMagMinDmg);
	msginfo.attack.phyMaxDmg		= (int)((float)pc->calPhyMaxDmg + (float)pc->calPhyMaxDmg * hitPro * 0.01 + phybonus + (float)pc->calMagMaxDmg);
	msginfo.attack.criticalRate		= pc->calCriticalRate + criPro;
	msginfo.attack.delayTime		= pc->calAttackDelay;
	msginfo.targetType				= pc->targetType;
	msginfo.targetIdx				= pc->targetIdx;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}


void	SKILL_TrainMagicBow( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg;


	dmg					=	pValue[SKILL_VALUE_0];
	
	

	pc->calPhyMinDmg	+=	dmg;	
	pc->calPhyMaxDmg	+=	dmg;	
}


void	SKILL_TrainDagger( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg;


	dmg					=	pValue[SKILL_VALUE_0];
	
	

	pc->calPhyMinDmg	+=	dmg;	
	pc->calPhyMaxDmg	+=	dmg;	
}




void	SKILL_SARecoveryInc( void* pPc, float* pValue)
{
	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pPc == NULL )	return;

	float		SaRec, RaUse, RaDec;

	SaRec	= pValue[SKILL_VALUE_0];	
	RaUse	= pValue[SKILL_VALUE_1];	
	RaDec	= pValue[SKILL_VALUE_2];	

	
	if( pc->curRA < RaUse ) pc->curRA = 1;
	else pc->curRA -= RaUse;

	messageInfo_t	msginfo;
	message_t		message;

	memset( &msginfo, 0, sizeof(messageInfo_t) );
	memset( &message, 0, sizeof( message_t ) );

	msginfo.targetType				= ENTITY_PC;
	msginfo.targetIdx				= pc->idx;

	msginfo.infoType				= MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx	= 28;
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_TOGGLE;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		= g_globalTime;

	
	
	
	
	msginfo.affect.delayTime		=  1000;

	msginfo.affect.common.type[0]	= AFFDATA_TYPE_INCSARECOVERY;
	msginfo.affect.common.data[0]	= (int)SaRec;

	msginfo.affect.common.type[1]	= AFFDATA_TYPE_INCRARECOVERY;
	msginfo.affect.common.data[1]	= -(int)RaDec;
	

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

}



void	SKILL_RARecoveryInc( void* pPc, float* pValue)
{
	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pPc == NULL )	return;

	float		RaRec, SaDec;

	

	RaRec	= pValue[SKILL_VALUE_0];	
	SaDec	= pValue[SKILL_VALUE_1];	
		
	
	messageInfo_t	msginfo;
	message_t		message;

	memset( &msginfo, 0, sizeof(messageInfo_t) );
	memset( &message, 0, sizeof( message_t ) );
	
	msginfo.targetType	= ENTITY_PC;
	msginfo.targetIdx	= pc->idx;

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx = 29;
	msginfo.affect.level = g_affectSkillLevel;
	msginfo.affect.type = AFFECT_TOGGLE;
	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime = g_globalTime;	

	msginfo.affect.delayTime	=  1000;
	msginfo.affect.common.type[0] = AFFDATA_TYPE_INCRARECOVERY;
	msginfo.affect.common.data[0] = (int)RaRec;

	
	msginfo.affect.common.type[1]	= AFFDATA_TYPE_INCSARECOVERY;
	msginfo.affect.common.data[1]	= -(int)SaDec;
	
	

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );


}



void SKILL_DefenceRate( void* pPc, float* pValue )
{
	float			keepTime, decDefenseRate;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);

	decDefenseRate	= pValue[SKILL_VALUE_0];
	keepTime		= pValue[SKILL_VALUE_1] * 1000.0f;

	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );
	memset( &message, 0, sizeof( message_t ) );
	
	msginfo.infoType				= MSGINFO_TYPE_AFFECT;
	msginfo.affect.skillTableIdx	= 41;
	msginfo.affect.level			= g_affectSkillLevel;
	
	msginfo.affect.type				= AFFECT_ASSIST;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;

	
	

	
	msginfo.targetType				= pc->entityType;
	msginfo.targetIdx				= pc->idx;
	





	msginfo.affect.common.type[0] = AFFDATA_TYPE_MAGDAMAGE;
	msginfo.affect.common.data[0] = decDefenseRate;
	

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	
}



void SKILL_BloodArrow( void* pPc, float* pValue )
{
	float			hitPro, phyBonus, breedRate, keepDmg, keepTime;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t*	pc = static_cast<playerCharacter_t *>(pPc);

	hitPro		= pValue[SKILL_VALUE_0];
	phyBonus	= pValue[SKILL_VALUE_1];
	breedRate	= pValue[SKILL_VALUE_2];
	keepDmg		= pValue[SKILL_VALUE_3];
	keepTime	= pValue[SKILL_VALUE_4] * 1000.0f;

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				= MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			= MSGINFO_ATK_TYPE_MAGIC;

	msginfo.attack.phyMinDmg		= (int)((float)pc->calPhyMinDmg + (float)pc->calPhyMinDmg * hitPro * 0.01 + phyBonus + (float)pc->calMagMinDmg);
	msginfo.attack.phyMaxDmg		= (int)((float)pc->calPhyMaxDmg + (float)pc->calPhyMaxDmg * hitPro * 0.01 + phyBonus + (float)pc->calMagMaxDmg);
	msginfo.attack.delayTime		= pc->calAttackDelay;

	msginfo.targetType				= pc->targetType;
	msginfo.targetIdx				= pc->targetIdx;

	
	msginfo.attack.etcInfo			= MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
	msginfo.attack.etcData0			= 3000;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	
	if( GTH_Random() > breedRate * 0.01f )	return;

	
	memset( &message, 0, sizeof( message_t ) );
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.targetType				= pc->targetType;
	msginfo.targetIdx				= pc->targetIdx;

	msginfo.infoType				= MSGINFO_TYPE_ATTACK;

	msginfo.attack.atkType			= MSGINFO_ATK_TYPE_MAGIC;
	msginfo.attack.phyMinDmg		= (int)keepDmg;
	msginfo.attack.phyMaxDmg		= (int)keepDmg;
	
	msginfo.attack.repeatNum		= (int)( keepTime / 3000.0f );
	msginfo.attack.repeatDelayTime	= 3000.0f;

	msginfo.targetType = pc->targetType;
	msginfo.targetIdx = pc->targetIdx;
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}




void SKILL_windwing(void* pPc, float* pValue )
{	
	
	float	incVel, keepTime;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);	

	incVel		= pValue[SKILL_VALUE_0];
	keepTime	= pValue[SKILL_VALUE_1] * 1000.0f;


	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;	
	
	msginfo.affect.skillTableIdx	= 43;
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_ASSIST;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;	

	msginfo.targetType				= pc->targetType;
	msginfo.targetIdx				= pc->targetIdx;

	
	playerCharacter_t* pTargetpc = NULL;
	if ( pc->targetIdx < 0 || pc->targetIdx >= MAX_PCS)
		return;

	pTargetpc = &g_pc[pc->targetIdx];
	if ( pTargetpc == NULL)		
		return;
	
	
	if ( GTH_IsActiveAffect(pTargetpc, ENTITY_PC, 38) )	
	{
		GTH_SendMessage_ChattingMsg(pc, 1, 9, 20 );	
		return ;
	}

	
	if ( GTH_IsActiveAffect(pTargetpc, ENTITY_PC, 34) )	
	{
		GTH_SendMessage_ChattingMsg(pc, 1,  9, 20 );
		return;
	}
	
	

	msginfo.affect.common.type[0]	= AFFDATA_TYPE_VELOCITY_RATE;
	msginfo.affect.common.data[0]	= (int)incVel;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}





void SKILL_Hide( void* pPc, float* pValue)
{
#pragma chMSG( 이거 캐릭이 안보여야 하는데... 일단 수치만 세팅하자.)
#pragma chMSG( 캐릭이 공격 시작과 동시에 다시 보여야 하는군...)

	float	addAttInc, phyBonus, hitPowerRate, conSaUse;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t* pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pc == NULL )	return;

	addAttInc		= pValue[SKILL_VALUE_0];
	phyBonus		= pValue[SKILL_VALUE_1];
	hitPowerRate	= pValue[SKILL_VALUE_2];
	conSaUse		= pValue[SKILL_VALUE_3];	

	memset( &msginfo, 0, sizeof(messageInfo_t) );
	memset( &message, 0, sizeof( message_t ) );

	msginfo.targetType				= ENTITY_PC;
	msginfo.targetIdx				= pc->idx;
	
	msginfo.infoType				= MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx	= 44;
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_TOGGLE;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		= g_globalTime;

	
	
	
	
	msginfo.affect.delayTime		=  3000;

	msginfo.affect.common.type[0]	= AFFDATA_TYPE_PHYDAMAGE_RATE;
	msginfo.affect.common.data[0]	= addAttInc;

	msginfo.affect.common.type[1]	= AFFDATA_TYPE_PHYDAMAGE;
	msginfo.affect.common.data[1]	= phyBonus;

	msginfo.affect.common.type[2]	= AFFDATA_TYPE_HITPOWRE_RATE;
	msginfo.affect.common.data[2]	= hitPowerRate;

	
	msginfo.affect.common.type[3]	= AFFDATA_TYPE_INCSARECOVERY;
	msginfo.affect.common.data[3]	= -(int)conSaUse;
	
	

	



	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

}



void SKILL_AttackRage( void* pPc, float* pValue )
{
	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pPc == NULL )	return;

	float		SaDec, SaUse, dmg, phyBonus;

	SaDec		= pValue[SKILL_VALUE_0];	
	SaUse		= pValue[SKILL_VALUE_1];	
	dmg			= pValue[SKILL_VALUE_2];	
	phyBonus	= pValue[SKILL_VALUE_3];	

	
	
	


	

	messageInfo_t	msginfo;
	message_t		message;

	memset( &msginfo, 0, sizeof(messageInfo_t) );
	memset( &message, 0, sizeof( message_t ) );

	msginfo.targetType				= ENTITY_PC;
	msginfo.targetIdx				= pc->idx;
	
	msginfo.infoType				= MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx	= 45;
	msginfo.affect.level			= g_affectSkillLevel;

	msginfo.affect.type				= AFFECT_TOGGLE;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		= g_globalTime;

	
	
	
	
	msginfo.affect.delayTime		=  3000;

	msginfo.affect.common.type[0]	= AFFDATA_TYPE_PHYDAMAGE_RATE;
	msginfo.affect.common.data[0]	= dmg;

	msginfo.affect.common.type[1]	= AFFDATA_TYPE_PHYDAMAGE;
	msginfo.affect.common.data[1]	= phyBonus;

	
	
	msginfo.affect.common.type[2]	= AFFDATA_TYPE_INCSARECOVERY;
	msginfo.affect.common.data[2]	= -(int)SaDec;

	
	msginfo.affect.common.type[3]	= AFFDATA_TYPE_ONCE_ATTACK_SACOST; 
	msginfo.affect.common.data[3]	= SaUse;	
	
	

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void SKILL_IncreaseCriticalRate(void* pPc, float* pValue )
{
	
	playerCharacter_t*	pc = NULL;
	serverPartyInfo_t*	pi= NULL;

	pc = static_cast<playerCharacter_t *>(pPc);
	if( pPc == NULL )	return;
	
	float IncCriticalRate	= pValue[SKILL_VALUE_0];	
	float KeepTime			= pValue[SKILL_VALUE_1] * 1000.0f;	

	

	messageInfo_t	msginfo;
	message_t		message;

	memset( &msginfo, 0, sizeof(messageInfo_t) );
	memset( &message, 0, sizeof( message_t ) );
	
	msginfo.infoType				= MSGINFO_TYPE_AFFECT;
	msginfo.affect.skillTableIdx	= 46;
	msginfo.affect.level			= g_affectSkillLevel;
	
	msginfo.affect.type				= AFFECT_ASSIST;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)KeepTime;	

	
	

	msginfo.affect.common.type[0]	= AFFDATA_TYPE_CRITICALRATE;
	msginfo.affect.common.data[0]	= IncCriticalRate;	
	
	
	float mastercalCriticalRate = g_pc[pc->idx].calCriticalRate;
	if ( mastercalCriticalRate < 0.0f) return;

	float calcriticalrate	= 0.0f;
	msginfo.targetIdx		= pc->idx;		
	msginfo.targetType		= ENTITY_PC;
	calcriticalrate			= IncCriticalRate + g_pc[pc->idx].calCriticalRate;				
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	for (int i = 0; i < pc->memberNumber; i ++)	
	{	
		
		int pcIdx				= pc->member[i].idxOnServer;				
		if ( pcIdx < 0)	
			continue;	

		
		if ( pcIdx == pc->idx)
			continue;
		
				
		if( pc->member[i].serverIndex < 0 ) 
			continue;		


		
		if ( pc->member[i].serverIndex != g_config.gameServerNo ) 
			continue;

		
		if ( g_pc[pcIdx].worldIdx != pc->worldIdx ) 
			continue;	

		
		if ( !g_pc[pcIdx].active || !g_pc[pcIdx].alive ) 
			continue;

		
		if( stricmp( g_pc[pcIdx].name, pc->member[i].memberName ) ) 
			continue;

		float calcriticalrate	= 0.0f;
		msginfo.targetIdx		= pcIdx;		
		msginfo.targetType		= ENTITY_PC;
		calcriticalrate			= IncCriticalRate + g_pc[pcIdx].calCriticalRate;		

		
		if ( pc->idx != pcIdx )
		{
			
			
			if ( IncCriticalRate + mastercalCriticalRate < calcriticalrate)
			calcriticalrate = IncCriticalRate + mastercalCriticalRate;

			
			if ( calcriticalrate <= g_pc[pcIdx].calCriticalRate)			
				continue;
		}

		AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
		AI_SendMessage( &message );
	}
	
}

#endif