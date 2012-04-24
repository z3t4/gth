


#include "../global.h"
#include "SkillApp.h"





CSkillProcBasic::CSkillProcBasic()
{

}

CSkillProcBasic::~CSkillProcBasic()
{

}

int CSkillProcBasic::GetInfoType(BYTE _Type)
{

	switch(_Type) {
	case SKILL_TYPE_COMMON:		return	MSGINFO_TYPE_COMMON	;
	case SKILL_TYPE_ATTACK:		return	MSGINFO_TYPE_ATTACK	;
	case SKILL_TYPE_AFFECT:		return	MSGINFO_TYPE_AFFECT	;
	}

	
	return NO_TYPE;
}

int CSkillProcBasic::GetTargetType(BYTE _Type,void *pPc)
{
	switch(_Type) {
	case ATTACK_TARGET_PC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)	return pc->targetType;
		}
		break;

	case ATTACK_TARGET_ENTITY_PC:
		return ENTITY_PC;

	case ATTACK_TARGET_MONSTER:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(pPc);
			if(pMon) return pMon->targetType;
		}
		break;

	case ATTACK_TARGET_ENTITY:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)	return pc->entityType;
		}
		break;
	}

	
	return NO_TYPE;
}

int CSkillProcBasic::GetTargetIdx(BYTE _Type,void *pPc,int index)
{
	switch(_Type) {
	case ATTACK_TARGET_INDEX_TARGETPC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)	return pc->targetIdx;
		}
		break;

	case ATTACK_TARGET_INDEX_PC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)	return pc->idx;
		}
		break;

	case ATTACK_TARGET_INDEX_MON_TARGETPC:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(pPc);
			if(pMon) return pMon->targetIdx;
		}
		break;


	case ATTACK_TARGET_INDEX_IDXONSERVER:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)	
			{
				if(index <0 || index >= MAX_NUMBER_OF_PARTY_MEMBER) 
				{
					
					return NO_TYPE;
				}

				return pc->member[index].idxOnServer;
			}
		}
		break;

	
	case ATTACK_TARGET_INDEX_MONSTER:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(pPc);
			if(pMon) return pMon->idx;
		}
		break;

	}

	
	return NO_TYPE;
}
