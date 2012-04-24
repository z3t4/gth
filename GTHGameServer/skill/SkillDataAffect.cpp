



#include "../global.h"
#include "SkillApp.h"





CSkillProcAffect::CSkillProcAffect()
{

}

CSkillProcAffect::~CSkillProcAffect()
{

}





int CSkillProcAffect::GetAffType(BYTE _Type)
{
	switch(_Type) {
	case SKILL_AFF_NONE:			return AFFECT_NONE;
	case SKILL_AFF_ATTACK:			return AFFECT_ATTACK;
	case SKILL_AFF_DEFENSE:			return AFFECT_DEFENSE;
	case SKILL_AFF_ASSIST:			return AFFECT_ASSIST;
	case SKILL_AFF_SPECIAL:			return AFFECT_SPECIAL;
	case SKILL_AFF_TOGGLE:			return AFFECT_TOGGLE;
	case SKILL_AFF_CURSE:			return AFFECT_CURSE;
	}

	TRACE("Error!!   GetAffType ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAffect::GetAffectType(BYTE _Type)
{
	switch(_Type) {
	case SKILL_AFF_TYPE_NORMAL:			return MSGINFO_AFF_TYPE_NORMAL ;
	case SKILL_AFF_TYPE_POSITIVE:		return MSGINFO_AFF_TYPE_POSITIVE ;
	case SKILL_AFF_TYPE_NEGATIVE:		return MSGINFO_AFF_TYPE_NEGATIVE ;
	case SKILL_AFF_TYPE_NONE:			return MSGINFO_AFF_TYPE_NONE ;
	}

	TRACE("Error!!   GetAffectType ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAffect::GetStartTime(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_AFF_STARTTIME_GLOBALTIME:		return g_globalTime;
		break;
	case SKILL_AFF_STARTTIME_PLUSVALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_AFF_STARTTIME,msg);
				return fVal;
			}	
		}
		break;
	}

	TRACE("Error!!  GetStartTime ... %d \n",_Type);
	return NO_TYPE;
}



int CSkillProcAffect::GetKeepTime(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case AFFECT_KEEP_TIME_DECDEFENSETIME:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_AFF_KEEPTIME,msg) * 1000;
				return (fVal + g_globalTime);
			}	
		}
		break;
	case AFFECT_KEEP_TIME_VALUETIME:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_AFF_KEEPTIME,msg) * 1000;
				return fVal+ g_globalTime;
			}	
		}
		break;
	case AFFECT_KEEP_TIME_STARTTIME:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_AFF_KEEPTIME,msg) * 1000;
				return (fVal + msg->msginfo->affect.startTime);
			}	
		}
		break;
	
	}

	TRACE("Error!!  GetKeepTime  ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAffect::GetPosMinDmg(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_AFF_POSMINDMG_AFFECTSKILLLEVEL:	 return g_affectSkillLevel;
	case SKILL_AFF_POSMINDMG_VALUE:				
		{
			return 0;
		}
		break;

	}

	TRACE("Error!!   GetPosMinDmg ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAffect::GetHitRate(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_AFF_HITRATE_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = (int)g_SkillDataGlobal.GetValue(msg->pPc,SKILL_AFF_HITRATE,msg);
				return fVal;
			}	
		}
		break;
	}

	TRACE("Error!!  GetHitRate  ... %d \n",_Type);
	return NO_TYPE;
}




int CSkillProcAffect::GetDelayTime(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_AFF_DELAYTIME_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = (int)g_SkillDataGlobal.GetValue(msg->pPc,SKILL_AFF_DELAYTIME,msg);
				return fVal;
			}	
		}
		break;

	}

	TRACE("Error!!   GetDelayTime ... %d \n",_Type);
	return NO_TYPE;
}
