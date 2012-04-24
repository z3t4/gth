 #include "../global.h"

#ifdef _OLD_SKILL_VER



void	SKILL_WaterFreezer( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg, slowPro, slowRate, magbonus;
	int				slowTime;
	messageInfo_t	msginfo;
	message_t		message;

	dmg				=	pValue[SKILL_VALUE_0];
	slowPro			=	pValue[SKILL_VALUE_1];
	slowTime		=	(int)pValue[SKILL_VALUE_2] * 1000;
	slowRate		=	pValue[SKILL_VALUE_3];
	magbonus		=	pValue[SKILL_VALUE_4];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_MAGIC;
	msginfo.attack.hitRate			=	100;
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calMagMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMinDmg	+	magbonus);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calMagMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMaxDmg	+	magbonus);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;




	
	msginfo.attack.etcInfo			= MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
	msginfo.attack.etcData0			= 2000;
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_AFFECT;
	msginfo.affect.skillTableIdx	=	10;						
	msginfo.affect.level			=	g_affectSkillLevel;		
	msginfo.affect.type				=	AFFECT_CURSE;
	msginfo.affect.affectType		=	MSGINFO_AFF_TYPE_POSITIVE;

	
	msginfo.affect.startTime		=	g_globalTime	+	1000;
	msginfo.affect.keepTime			=	msginfo.affect.startTime	+	slowTime;
	msginfo.affect.hitRate			=	(int)slowPro;

	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;

	msginfo.affect.common.type[0]	= AFFDATA_TYPE_VELOCITY_RATE;
	msginfo.affect.common.data[0]	= -(int)slowRate;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}
	

void	SKILL_RARecovery(void* pPc, float* pValue)
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);
	float raRecovery = pValue[SKILL_VALUE_0];

	switch( pc->targetType )
	{
	case ENTITY_PC :
		g_pc[pc->targetIdx].curRA += raRecovery;
		g_pc[pc->targetIdx].curRA += (float)raRecovery * g_pc[pc->targetIdx].optValue[OV_HEAL_EFFICIENCY] * 0.01f;

		if( g_pc[pc->targetIdx].curRA > g_pc[pc->targetIdx].calMaxRA ) 
			g_pc[pc->targetIdx].curRA = g_pc[pc->targetIdx].calMaxRA;
		break;
	}
}



void	SKILL_HitandReturn(void* pPc, float* pValue)
{
	float			dmg, hitPro;
	float			moveSp, criPro, phybonus;

	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);

	dmg			= pValue[SKILL_VALUE_0];
	hitPro		= pValue[SKILL_VALUE_1];
	moveSp		= pValue[SKILL_VALUE_2];
	criPro		= pValue[SKILL_VALUE_3];
	phybonus	= pValue[SKILL_VALUE_4];

	memset( &msginfo, 0, sizeof(messageInfo_t) );

	msginfo.infoType				= MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			= MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitPower			= (int)((float)pc->calHitPower * hitPro * 0.01f);
	
	msginfo.attack.phyMinDmg		= (int)((float)pc->calPhyMinDmg + (float)pc->calPhyMinDmg * hitPro * 0.01 + (float)pc->calMagMinDmg + phybonus);
	msginfo.attack.phyMaxDmg		= (int)((float)pc->calPhyMaxDmg + (float)pc->calPhyMaxDmg * hitPro * 0.01 + (float)pc->calMagMaxDmg + phybonus);
	
	msginfo.attack.criticalRate		= (int)criPro;
	


	
	
	msginfo.targetType				= pc->targetType;
	msginfo.targetIdx				= pc->targetIdx;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}


void	SKILL_BeFrozen(void* pPc, float* pValue)
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg, freezePro, magbonus;
	int				keepTime;
	messageInfo_t	msginfo;
	message_t		message;

	dmg				=	pValue[SKILL_VALUE_0];
	freezePro		=	pValue[SKILL_VALUE_1];
	keepTime		=	(int)pValue[SKILL_VALUE_2] * 1000;
	magbonus		=	pValue[SKILL_VALUE_3];
	
	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_MAGIC;
	msginfo.attack.hitRate			=	100;
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calMagMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMinDmg	+	magbonus);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calMagMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMaxDmg	+	magbonus);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;




	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );

	memset( &msginfo, 0, sizeof( messageInfo_t ) );
	
	msginfo.infoType				=	MSGINFO_TYPE_AFFECT;
	msginfo.affect.skillTableIdx	=	13;
	msginfo.affect.level			=	g_affectSkillLevel;
	msginfo.affect.type				=	AFFECT_CURSE;
	msginfo.affect.affectType		=	MSGINFO_AFF_TYPE_POSITIVE;
	msginfo.affect.startTime		=	g_globalTime + 1000;		
	msginfo.affect.keepTime			=	msginfo.affect.startTime	+	(int)keepTime;
	msginfo.affect.hitRate			=	(int)freezePro;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;

	msginfo.affect.common.type[0]	=	AFFDATA_TYPE_PARALYSIS;
	msginfo.affect.common.data[0]	=	0;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}


void	SKILL_TrainStaff( void *pPc, float *pValue )
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	float			dmg;


	dmg					=	pValue[SKILL_VALUE_0];
	
	
 	
	pc->calMagMinDmg	+=	dmg;	
	pc->calMagMaxDmg	+=	dmg;	
}



void	SKILL_CoolFrame(void* pPc, float* pValue)
{
	float	dmg;
	float	magbonus;

	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pc == NULL )	return;

	dmg			=	pValue[SKILL_VALUE_0];		
	magbonus	=	pValue[SKILL_VALUE_1];		

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_MAGIC;
	msginfo.attack.hitRate			=	100;
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calMagMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMinDmg	+	magbonus);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calMagMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMaxDmg	+	magbonus);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void	SKILL_SAUseDecrease( void* pPc, float* pValue)
{
	int				keepTime;
	float			DecSa;

	messageInfo_t	msginfo;
	message_t		message;


	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pc == NULL )	return;

	DecSa		=	pValue[SKILL_VALUE_0];
	keepTime	=	(int)pValue[SKILL_VALUE_1] * 1000;

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_AFFECT;
	msginfo.affect.skillTableIdx	=	31;
	msginfo.affect.level			=	g_affectSkillLevel;
	msginfo.affect.type				=	AFFECT_ASSIST;
	msginfo.affect.affectType		=	MSGINFO_AFF_TYPE_NORMAL;

	msginfo.affect.startTime		=	g_globalTime;
	msginfo.affect.keepTime			=	g_globalTime	+	keepTime;

	msginfo.targetType				=	ENTITY_PC;
	msginfo.targetIdx				=	pc->idx;

	msginfo.affect.common.type[0]	=	AFFDATA_TYPE_DECREASESAUSE;
	msginfo.affect.common.data[0]	=	DecSa;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void	SKILL_Resurrection(void* pPc, float* pValue)
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);

	float raRec			= pValue[SKILL_VALUE_0];
	float saRec			= pValue[SKILL_VALUE_1];
	float expRecRate	= pValue[SKILL_VALUE_2];

	message_t message;

	switch(pc->targetType){
		case ENTITY_PC :
			if( g_pc[pc->targetIdx].alive ) return;
			break;

		default:
		case ENTITY_MONSTER : 
		case ENTITY_NPC :
			
			return;
			break;
	}

	message.type		= MSG_ETC;
	message.fromIdx		= pc->idx;
	message.fromType	= ENTITY_PC;
	strcpy( message.fromName, pc->name );

	message.toType		= pc->targetType;
	message.toIdx		= pc->targetIdx;
	
	message.worldIdx	= pc->worldIdx;
	message.sendTime	= g_globalTime;
	message.receiveTime = g_globalTime + 1500;
	
	message.data[0]		= MSG_ETC_RESURRECTION;
	message.data[1]		= (int)raRec;
	message.data[2]		= (int)saRec;
	message.data[3]		= (int)expRecRate;

	AI_SendMessage( &message );
}




void	SKILL_WatSelfHeal(void* pPc, float* pValue)
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



void	SKILL_ManaShield( void* pPc, float* pValue )
{
	float			keepTime;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t* pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);

	if( pc == NULL )	return;

	keepTime	= pValue[SKILL_VALUE_0] * 1000.0f;

	memset( &msginfo, 0, sizeof(messageInfo_t) );

	msginfo.infoType				= MSGINFO_TYPE_AFFECT;
	msginfo.affect.skillTableIdx	= 47;
	msginfo.affect.level			= g_affectSkillLevel;
	msginfo.affect.type				= AFFECT_ASSIST;
	msginfo.affect.affectType		= MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime		= g_globalTime;
	msginfo.affect.keepTime			= g_globalTime + (int)keepTime;
	msginfo.targetType				= ENTITY_PC;
	msginfo.targetIdx				= pc->idx;

	msginfo.affect.common.type[0] = AFFDATA_TYPE_MANASHIELD;
	msginfo.affect.common.data[0] = (int)1;

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void	SKILL_SummonJude(void* pPc, float* pValue)
{
	float			dmg, magBonus, range;
	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t*	pc = NULL;
	pc = static_cast<playerCharacter_t *>(pPc);
	if( pc == NULL )	return;

	dmg			=	pValue[SKILL_VALUE_0];		
	magBonus	=	pValue[SKILL_VALUE_1];		
	range		=	pValue[SKILL_VALUE_2];		

	memset( &msginfo, 0, sizeof( messageInfo_t ) );

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_NORMAL;
	msginfo.attack.hitRate			=	100;
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calMagMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMinDmg	+	magBonus);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calMagMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMaxDmg	+	magBonus);
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;
	
	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message, (int)range, MSG_RANGE_TYPE_TARGET );
}


void	SKILL_RARecovery2(void* pPc, float* pValue)
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);
	float raRecovery = pValue[SKILL_VALUE_0];

	switch( pc->targetType )
	{
	case ENTITY_PC :
		g_pc[pc->targetIdx].curRA += raRecovery;
		g_pc[pc->targetIdx].curRA += (float)raRecovery * g_pc[pc->targetIdx].optValue[OV_HEAL_EFFICIENCY] * 0.01f;

		if( g_pc[pc->targetIdx].curRA > g_pc[pc->targetIdx].calMaxRA ) 
			g_pc[pc->targetIdx].curRA = g_pc[pc->targetIdx].calMaxRA;
		break;
	}
}



void	SKILL_ResurrectionUpgrade(void* pPc, float* pValue)
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);

	float raRec			= pValue[SKILL_VALUE_0];
	float saRec			= pValue[SKILL_VALUE_1];
	float expRecRate	= pValue[SKILL_VALUE_2];

	message_t message;

	switch( pc->targetType )
	{
	case ENTITY_PC :
		if( g_pc[pc->targetIdx].alive ) return;
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		
		return;
		break;
	}

	message.type		= MSG_ETC;
	message.fromIdx		= pc->idx;
	message.fromType	= ENTITY_PC;
	strcpy( message.fromName, pc->name );

	message.toType		= pc->targetType;
	message.toIdx		= pc->targetIdx;
	
	message.worldIdx	= pc->worldIdx;
	message.sendTime	= g_globalTime;
	message.receiveTime = g_globalTime + 1500;
	
	message.data[0]		= MSG_ETC_RESURRECTION;
	message.data[1]		= (int)raRec;
	message.data[2]		= (int)saRec;
	message.data[3]		= (int)expRecRate;

	AI_SendMessage( &message );
}




void SKILL_AttackDmgUp(void* pPc, float* pValue)
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);
	float dmg	   = pValue[SKILL_VALUE_0];
	float keepTime = pValue[SKILL_VALUE_1] * 1000.0f;
	
	messageInfo_t msginfo;
	message_t message;

	memset( &msginfo, 0, sizeof( messageInfo_t ) );
	memset( &message, 0, sizeof( message_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx = 52;
	msginfo.affect.level = g_affectSkillLevel;	

	msginfo.affect.type = AFFECT_ASSIST;

	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime = g_globalTime;
	msginfo.affect.keepTime = g_globalTime + (int)keepTime;

	
	msginfo.affect.common.type[0] = AFFDATA_TYPE_PHYDAMAGE;
	msginfo.affect.common.data[0] = dmg;
	
	msginfo.targetType	= pc->targetType;
	msginfo.targetIdx	= pc->targetIdx;	
	

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
}



void SKILL_SkillSooho(void* pPc, float* pValue)
{

	float			dmg;
	float			magbonus;

	messageInfo_t	msginfo;
	message_t		message;

	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);

	dmg			= pValue[SKILL_VALUE_0];	
	magbonus	= pValue[SKILL_VALUE_1];

	memset( &msginfo, 0, sizeof(messageInfo_t) );	

	msginfo.infoType				=	MSGINFO_TYPE_ATTACK;
	msginfo.attack.atkType			=	MSGINFO_ATK_TYPE_MAGIC;
	msginfo.attack.hitRate			=	100;
	msginfo.attack.phyMinDmg		=	(int)(	(float)pc->calMagMinDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMinDmg	+	magbonus);
	msginfo.attack.phyMaxDmg		=	(int)(	(float)pc->calMagMaxDmg	*	(	1.0f	+	dmg		*	0.01f	)	+	pc->calPhyMaxDmg	+	magbonus);
	msginfo.attack.criticalRate		=	0;
	msginfo.attack.criticalDamage	=	0;
	msginfo.attack.delayTime		=	pc->calAttackDelay;
	msginfo.attack.repeatNum		=	0;
	msginfo.targetType				=	pc->targetType;
	msginfo.targetIdx				=	pc->targetIdx;


	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );
	
}



void SKILL_IncreaseRa(void* pPc, float* pValue )
{

	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);
	float increaseRA	= pValue[SKILL_VALUE_0];
	float keepTime		= pValue[SKILL_VALUE_1] * 1000.0f;
	
	messageInfo_t msginfo;
	message_t message;

	memset( &msginfo, 0, sizeof( messageInfo_t ) );
	memset( &message, 0, sizeof( message_t ) );

	msginfo.infoType = MSGINFO_TYPE_AFFECT;
	
	msginfo.affect.skillTableIdx = 59;
	msginfo.affect.level = g_affectSkillLevel;	

	msginfo.affect.type = AFFECT_ASSIST;

	msginfo.affect.affectType = MSGINFO_AFF_TYPE_NORMAL;
	msginfo.affect.startTime = g_globalTime;
	msginfo.affect.keepTime = g_globalTime + (int)keepTime;

	
	msginfo.affect.common.type[0] = AFFDATA_TYPE_MAXRA;
	msginfo.affect.common.data[0] = increaseRA;
	
	
	msginfo.targetType	= pc->targetType;
	msginfo.targetIdx	= pc->targetIdx;		

	AI_MakeMessage( pc, &message, MESSAGE_USEINFO, &msginfo );
	AI_SendMessage( &message );


}



void	SKILL_RARecovery3(void* pPc, float* pValue)
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);
	float raRecovery = pValue[SKILL_VALUE_0];

	switch( pc->targetType )
	{
	case ENTITY_PC :
		g_pc[pc->targetIdx].curRA += raRecovery;
		g_pc[pc->targetIdx].curRA += (float)raRecovery * g_pc[pc->targetIdx].optValue[OV_HEAL_EFFICIENCY] * 0.01f;

		if( g_pc[pc->targetIdx].curRA > g_pc[pc->targetIdx].calMaxRA ) 
			g_pc[pc->targetIdx].curRA = g_pc[pc->targetIdx].calMaxRA;
		break;
	}
}



playerCharacter_t* GetPCPointer(const int Idx)
{
	if((Idx < 0) || (Idx >= MAX_PCS)) return NULL;
	if(FALSE == g_pc[Idx].active) return NULL;

	return &g_pc[Idx];
}







void	SKILL_Groupheal(void* parampPc, float* pValue)
{	
	playerCharacter_t*	pPc	= NULL;
	pPc = static_cast<playerCharacter_t *>(parampPc);
	if( NULL == pPc) return;	
	
	float raRecovery=0.0f;
	float range=0.0f;

	raRecovery		= pValue[SKILL_VALUE_0];	
	range			= pValue[SKILL_VALUE_1];	


	pPc->curRA += raRecovery;
	pPc->curRA += (float)raRecovery * pPc->optValue[OV_HEAL_EFFICIENCY] * 0.01f;	
	if(pPc->curRA > pPc->calMaxRA) pPc->curRA = pPc->calMaxRA;


	playerCharacter_t*	pMemberPC=NULL;
	partyMember_t* pPartyMemberList=NULL;
	int PartyMemberIdx=0;

	for(PartyMemberIdx = 0; PartyMemberIdx < MAX_NUMBER_OF_PARTY_MEMBER ; PartyMemberIdx ++){
		
		pPartyMemberList=&(pPc->member[PartyMemberIdx]);

		if ( pPartyMemberList == NULL)	continue;

		if(pPartyMemberList->serverIndex < 0 ) continue;

		
		if( pPartyMemberList->serverIndex != g_config.gameServerNo ) continue;
		
		pMemberPC=GetPCPointer(pPartyMemberList->idxOnServer);
		if(NULL == pMemberPC) continue;
		
		if(FALSE == pMemberPC->alive ) continue;
		if(pPc == pMemberPC) continue;
		

		
		if ( pMemberPC->worldIdx != pPc->worldIdx ) 	continue;	
		

		
		if(0 != stricmp(pMemberPC->name, pPartyMemberList->memberName ) ) continue;
		if( GTH_GetDistance2D( pPc->position, pMemberPC->position) > range ) continue;
		
		pMemberPC->curRA += raRecovery;
		pMemberPC->curRA += (float)raRecovery * pMemberPC->optValue[OV_HEAL_EFFICIENCY] * 0.01f;
		if( pMemberPC->curRA > pMemberPC->calMaxRA ) pMemberPC->curRA = pMemberPC->calMaxRA;
	}

}





#endif