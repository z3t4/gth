



#include "../Global.h"
#include "YDCDamageCal.h"


#include "../Tools/CTools.h"
extern CTools* gcpTools;




YDCDamageCal::YDCDamageCal()
{
	m_bOK=FALSE;

	m_bOK=TRUE;

}




YDCDamageCal::~YDCDamageCal()
{

}





BOOL YDCDamageCal::isOK(void) const
{
	return m_bOK;
}







void YDCDamageCal::YDCalResistance(
	const struct tagYDCharactor* in_pDefenceCharactor,
	struct tagYDResistanceVar& out_ResistanceVar)
{	
	out_ResistanceVar.physics		=	in_pDefenceCharactor->calDefensePower;
	out_ResistanceVar.positive		=	in_pDefenceCharactor->calPositiveRes;
	out_ResistanceVar.negative		=	in_pDefenceCharactor->calNegativeRes;		


	if(	out_ResistanceVar.positive	<	-100.0f	)	out_ResistanceVar.positive	=	-100.0f;
	if(	out_ResistanceVar.positive	>	75.0f	)	out_ResistanceVar.positive	=	75.0f;

	if(	out_ResistanceVar.negative	<	-100.0f	)	out_ResistanceVar.negative	=	-100.0f;	
	if(	out_ResistanceVar.negative	>	75.0f	)	out_ResistanceVar.negative	=	75.0f;	
	
	out_ResistanceVar.physics *= ( 1 + (float) in_pDefenceCharactor->optValue[OV_SKILL_FINAL_ADD_DEFENCE_RATE] * 0.01f);
}





inline void YDCDamageCal::YDMakeCharactorStruct(struct tagYDCharactor& out_Charactor,const playerCharacter_t* in_pPlayer)
{
	out_Charactor.Entity.Type     = in_pPlayer->entityType;
	out_Charactor.Entity.Idx	   = in_pPlayer->idx;
	memcpy(out_Charactor.optValue, in_pPlayer->optValue, sizeof(float) * NUM_OPTION_VALUE);
	out_Charactor.calDefensePower =in_pPlayer->calDefensePower;
	out_Charactor.calPositiveRes  =in_pPlayer->calPositiveRes;
	out_Charactor.calNegativeRes  =in_pPlayer->calNegativeRes;
	out_Charactor.level		   =in_pPlayer->level;
	out_Charactor.calCriticalDmg  =in_pPlayer->calCriticalDmg;
	out_Charactor.calMaxRA		   =in_pPlayer->calMaxRA;
}



inline void YDCDamageCal::YDMakeCharactorStruct(struct tagYDCharactor& out_Charactor,const monsterCharacter_t* in_pMonster)
{
	out_Charactor.Entity.Type	   = in_pMonster->entityType;
	out_Charactor.Entity.Idx      = in_pMonster->idx;
	memcpy(out_Charactor.optValue, in_pMonster->optValue, sizeof(float) * NUM_OPTION_VALUE);
	out_Charactor.calDefensePower = in_pMonster->calDefensePower;
	out_Charactor.calPositiveRes  = in_pMonster->calPositiveRes;
	out_Charactor.calNegativeRes  = in_pMonster->calNegativeRes;
	out_Charactor.level		   = in_pMonster->basic.level;;
	out_Charactor.calCriticalDmg  = in_pMonster->calCriticalDmg;
	out_Charactor.calMaxRA        = in_pMonster->calMaxRA;
}







void YDCDamageCal::YDCalAdditionalDamage(	
	const struct tagYDCharactor& in_pDefenceCharactor,
	const message_t*			 in_pMessage,
	struct tagYDDamageVar&		 out_AdditionalDamageVar)	
{
	out_AdditionalDamageVar.physics =0;
	out_AdditionalDamageVar.positive=0;
	out_AdditionalDamageVar.negative=0;

	
	if(in_pMessage->data[MSG_ATKDATA_ETC_INFO] == MSGINFO_ATK_ETC_RA_RATIO_DAMAGE)
	{
		out_AdditionalDamageVar.physics  = in_pDefenceCharactor.calMaxRA * ( (float) in_pMessage->data[MSG_ATKDATA_ETC_DATA_1] * 0.01f);
		out_AdditionalDamageVar.positive = in_pDefenceCharactor.calMaxRA * ( (float) in_pMessage->data[MSG_ATKDATA_ETC_DATA_0] * 0.01f);
	}
}






void YDCDamageCal::YDCalDamage(const message_t* in_pMessage,struct tagYDDamageVar& out_DamageVar)
{
	out_DamageVar.physics =in_pMessage->data[MSG_ATKDATA_PHYSIC_DMG];
	out_DamageVar.positive=in_pMessage->data[MSG_ATKDATA_POSITIVE_DMG];
	out_DamageVar.negative=in_pMessage->data[MSG_ATKDATA_NEGATIVE_DMG];
}
	




void YDCDamageCal::YDApplyResistanceAtDamage(
    const message_t* in_pMessage,
	const struct tagYDDamageVar&	 in_DamageVar,
	const struct tagYDDamageVar&	 in_AdditionalDamageVar,
	const struct tagYDResistanceVar& in_ResistanceVar,
	struct tagYDDamageVar&			 out_ApplyDamageVar,
	int	in_AttackerLevel)
{

	
	
	tagYDResistanceVar ResistanceRatio = {0,0,0};
	int MaxDefenseTableIdx = in_AttackerLevel-1;

	
	if ( MaxDefenseTableIdx >= MAX_NUMBER_OF_LEVEL)
		MaxDefenseTableIdx = MAX_NUMBER_OF_LEVEL -1;

	if ( (MaxDefenseTableIdx < 0) || 
		(MaxDefenseTableIdx >= MAX_NUMBER_OF_LEVEL))
		return;

	int DefenseVar = g_MaxDefneseTable[MaxDefenseTableIdx].MaxDefense;

	ResistanceRatio.physics		= in_ResistanceVar.physics / DefenseVar;
	ResistanceRatio.negative	= in_ResistanceVar.negative/ DefenseVar;
	ResistanceRatio.positive	= in_ResistanceVar.positive/ DefenseVar;
	
	if ( ResistanceRatio.physics < 0 ) ResistanceRatio.physics =0;
	if ( ResistanceRatio.physics > 0.8 ) ResistanceRatio.physics=0.8;

	if ( ResistanceRatio.negative < 0 ) ResistanceRatio.negative=0;
	if ( ResistanceRatio.negative > 0.8 ) ResistanceRatio.negative=0.8;

	if ( ResistanceRatio.positive < 0 ) ResistanceRatio.positive=0;	
	if ( ResistanceRatio.positive > 0.8 ) ResistanceRatio.positive=0.8;
	





	out_ApplyDamageVar.physics  = (in_DamageVar.physics  + in_AdditionalDamageVar.physics)   *	(1.0f - ResistanceRatio.physics);
	out_ApplyDamageVar.positive = (in_DamageVar.positive + in_AdditionalDamageVar.positive)	 *	(1.0f - ResistanceRatio.positive);
	out_ApplyDamageVar.negative = (in_DamageVar.negative + in_AdditionalDamageVar.negative)	 *	(1.0f -	ResistanceRatio.negative);

	
	
	if( in_pMessage->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_NONE )
	{
		out_ApplyDamageVar = in_DamageVar;			
	}

	if( out_ApplyDamageVar.physics  < 1 )  out_ApplyDamageVar.physics = 1;
	if( out_ApplyDamageVar.positive < 1 ) out_ApplyDamageVar.positive = 1;
	if( out_ApplyDamageVar.negative < 1 ) out_ApplyDamageVar.negative = 1;
}






void YDCDamageCal::YDApplyRatioAtDamage(
	const message_t*			 in_pMessage,
	const struct tagYDCharactor& in_AttackCharactor,
	const struct tagYDCharactor& in_DefenceCharactor,
	const struct tagYDDamageVar& in_ApplyDamageVar,
	struct tagYDDamageVar&       out_ApplyRatioDamageVar)
{	
	out_ApplyRatioDamageVar = in_ApplyDamageVar;

	
	if(	in_pMessage->data[MSG_ATKDATA_STUN_FLAG] > 0)
	{		
		out_ApplyRatioDamageVar.physics	= out_ApplyRatioDamageVar.physics *	2;		

		if(in_AttackCharactor.Entity.Type == ENTITY_PC)
		{			
			
			out_ApplyRatioDamageVar.physics	+= 
				out_ApplyRatioDamageVar.physics * in_AttackCharactor.optValue[OV_CRITICAL_DMG_RATE] * 0.01f;

			
			out_ApplyRatioDamageVar.physics	+=	in_AttackCharactor.calCriticalDmg;
		}		
	}	



	
	if((in_AttackCharactor.Entity.Type	==	ENTITY_PC)	&&	(in_DefenceCharactor.Entity.Type ==	ENTITY_MONSTER))
	{

		monsterCharacter_t* pDefenceMonster = gcpTools->GetMonsterRecordPointer(in_DefenceCharactor.Entity.Idx);
		if(NULL != pDefenceMonster){

			int monsterType	= pDefenceMonster->basic.type;
			if((monsterType >= 0) && (monsterType <= 4)){
				int optionPhyGroupIdx	=	monsterType	+	OV_CHE_PHY_DMG_RATE;
				int optionMagGroupIdx	=	monsterType	+	OV_CHE_MAG_DMG_RATE;

				out_ApplyRatioDamageVar.physics	    += out_ApplyRatioDamageVar.physics  * in_AttackCharactor.optValue[optionPhyGroupIdx] * 0.01f;
				out_ApplyRatioDamageVar.positive	+= out_ApplyRatioDamageVar.positive	* in_AttackCharactor.optValue[optionMagGroupIdx] * 0.01f;
				out_ApplyRatioDamageVar.negative	+= out_ApplyRatioDamageVar.negative	* in_AttackCharactor.optValue[optionMagGroupIdx] * 0.01f;
			}
		}
	}

	out_ApplyRatioDamageVar.physics	    *= ( 1 + (float) in_AttackCharactor.optValue[OV_SKILL_FINAL_ADD_PHY_DMG_RATE] * 0.01f);			
	out_ApplyRatioDamageVar.positive	*= ( 1 + (float) in_AttackCharactor.optValue[OV_SKILL_FINAL_ADD_MYSTERY_POS_DMG_RATE] * 0.01f);	
	out_ApplyRatioDamageVar.negative	*= ( 1 + (float) in_AttackCharactor.optValue[OV_SKILL_FINAL_ADD_MYSTERY_NEG_DMG_RATE] * 0.01f);	
}







int YDCDamageCal::YDGetDamage( int TargetEntityIdx, int TargetEntityType, message_t* message, int eachDmg[] )
{
	eachDmg[0] = 0;
	eachDmg[1] = 0;
	eachDmg[2] = 0;

	struct tagYDCharactor AttackCharactor;
	struct tagYDCharactor DefenceCharactor;

	
	int AttackerLevel =1;


	switch(message->fromType){
		case ENTITY_PC:	
			{
				playerCharacter_t* pAttackPlayer=NULL;
				pAttackPlayer =  gcpTools->GetPlayerRecordPointer(message->fromIdx);
				if(NULL == pAttackPlayer) return 0;
				YDMakeCharactorStruct(AttackCharactor,pAttackPlayer);

				AttackerLevel = pAttackPlayer->level;

			}
			break;
		case ENTITY_MONSTER:
		case ENTITY_NPC:
			{
				monsterCharacter_t* pAttackMonster=NULL;
				pAttackMonster = gcpTools->GetMonsterRecordPointer(message->fromIdx);
				if(NULL == pAttackMonster) return 0;
				YDMakeCharactorStruct(AttackCharactor,pAttackMonster);

				AttackerLevel = pAttackMonster->basic.level;
			}
			break;
		default:
			return 0;
	}	

	switch(TargetEntityType){
		case ENTITY_PC:
			{
				playerCharacter_t* pDefencePlayer=NULL;
				pDefencePlayer =  gcpTools->GetPlayerRecordPointer(TargetEntityIdx);
				if(NULL == pDefencePlayer) return 0;

				YDMakeCharactorStruct(DefenceCharactor,pDefencePlayer);			
			}
			break;
		case ENTITY_MONSTER:
		case ENTITY_NPC:	
			{
				monsterCharacter_t* pDefencetMonster=NULL;
				pDefencetMonster = gcpTools->GetMonsterRecordPointer(TargetEntityIdx);
				if(NULL == pDefencetMonster) return 0;
				YDMakeCharactorStruct(DefenceCharactor,pDefencetMonster);
			}
			break;
		default:
			return 0;
	}
	
	
	
	
	
	
	

	
	struct tagYDDamageVar DamageVar={0,0,0};
	YDCalDamage(message,DamageVar);

	
	struct tagYDDamageVar AdditionalDamageVar={0,0,0};
	YDCalAdditionalDamage(DefenceCharactor,message,AdditionalDamageVar);

	
	struct tagYDResistanceVar ResistanceVar={0,0,0};
	YDCalResistance(&DefenceCharactor,ResistanceVar);

	
	struct tagYDDamageVar ApplyDamageVar={0,0,0};
	YDApplyResistanceAtDamage(message,DamageVar,AdditionalDamageVar,ResistanceVar,ApplyDamageVar, AttackerLevel);

	
	struct tagYDDamageVar ApplyRatioDamageVar={0,0,0};
	YDApplyRatioAtDamage(message,AttackCharactor,DefenceCharactor,ApplyDamageVar,ApplyRatioDamageVar);


	
	int dmg = ApplyRatioDamageVar.physics + ApplyRatioDamageVar.positive + ApplyRatioDamageVar.negative;	
	if( dmg <= 0 ) dmg=1;

	
	if((AttackCharactor.Entity.Type == ENTITY_PC) && (DefenceCharactor.Entity.Type == ENTITY_PC)) 
		dmg = (int)( (float)dmg * 0.7f );



	eachDmg[0] = ApplyRatioDamageVar.physics;
	eachDmg[1] = ApplyRatioDamageVar.positive;
	eachDmg[2] = ApplyRatioDamageVar.negative;
	
	
	return dmg;
}










