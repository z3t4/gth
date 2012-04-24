



#include "../global.h"
#include "SkillApp.h"





CSkillProcAttack::CSkillProcAttack()
{

}

CSkillProcAttack::~CSkillProcAttack()
{

}






int CSkillProcAttack::GetAttackType(BYTE _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_ATK_TYPE_NORMAL:			return MSGINFO_ATK_TYPE_NORMAL;
	case SKILL_ATK_TYPE_MAGIC:			return MSGINFO_ATK_TYPE_MAGIC;
	case SKILL_ATK_TYPE_EFFECT:			return MSGINFO_ATK_TYPE_EFFECT;
	case SKILL_ATK_TYPE_NONE:			return MSGINFO_ATK_TYPE_NONE;
	}

	TRACE("Error!!   GetAttackType ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAttack::GetHitPower(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_HITPOWER_HITPRO:
		{
			float fHitPro = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_HITPOWER,msg);
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)	
			{
				return (int)((float)pc->calHitPower * fHitPro * 0.01f);
			}
		}
		break;

		
	case SKILL_HITPOWER_NORMAL:
		{
			switch(msg->pDataFormat->m_ClassType)
			{
			case SKILL_CLASS_PC: 
				{
					playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
					if(NULL == pc) return 0;

					float fHitPro = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_HITPOWER,msg);
					return (int)((float)pc->calHitPower * (1.0f + fHitPro * 0.01f));
				}
				break;
			
			case SKILL_CLASS_MON:
				{
					monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
					if(NULL == pMon) return 0;

					float fHitPro = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_HITPOWER,msg);					
					return (int)((float)pMon->calHitPower * (1.0f + fHitPro * 0.01f));
				}
				break;
			}
		}
		break;


	case SKILL_HITPOWER_VALUE:
		{
			float fHitPro = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_HITPOWER,msg);
			return fHitPro;
		}
		break;
	}

	TRACE("Error!!  GetHitPower  ... %d \n",_Type);
	return NO_TYPE;
}



int CSkillProcAttack::GetHiteRate(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_HITRATE_VALUE:
		{
			float fHitRate = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_HITRATE,msg);
			return fHitRate;
		}
		break;
	case SKILL_HITRATE_PANELTY:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);

			float fHitRate = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_HITRATE,msg);
			if(pc)
			{
				if(!SAFE_MONSTER(pc->targetIdx)) return 0;

				int level = (pc->level - g_monster[pc->targetIdx].basic.level);
				if( level < 0 )	level = 0;

				float panelty = 1.0f - 0.03 * (float)level;
				return (fHitRate * 0.01 * panelty);
			}
		break;
		}
	}

	TRACE("Error!!  GetHiteRate  ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAttack::GetPhyMinDmg(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_PHYMINDMG_NORMAL:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYMINDMG,msg);
				return (int)(	(float)pc->calPhyMinDmg	* (1.0f + fDmg * 0.01f) + pc->calMagMinDmg );
			}
		}
		break;











	case SKILL_PHYMINDMG_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYMINDMG,msg);
				return fDmg;
			}	
		}
		break;	
	case SKILL_PHYMINDMG_MON:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
			if(pMon)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYMINDMG,msg);
				return fDmg + pMon->calPhyMinDmg;
			}	
		}
		break;

	case SKILL_PC_PHYMINDMG_ADD_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				int nGrade = g_SkillDataGlobal.GetGrade(pc->pcJob);

				
				if(nGrade <0 || nGrade >= SKILL_MAX_BASIC_JOB_TYPE) return 0;

				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYBONUS_MENTALITY+nGrade,msg);
				pc->calPhyMinDmg += fDmg;
				return 0;
			}	
		}
		break;	

	case SKILL_PHYMINDMG_MAGIC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMINDMG,msg);
				return (int)((float)pc->calMagMinDmg * ( 1.0f + fDmg  * 0.01f ) + pc->calMagMinDmg );
			}
		}
		break;
	case SKILL_PHYMINDMG_NORMAL_PHYBONUS :
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg		= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYMINDMG,msg);
				float fBonus	= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYBONUS,msg);
				return (int)(	(float)pc->calPhyMinDmg	* (1.0f + fDmg  * 0.01f ) + pc->calMagMinDmg + fBonus );
			}
		}
		break;

	case SKILL_PHYMINDMG_MAGIC_PHYBONUS:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg		= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMINDMG,msg);
				float fPosBonus	= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSBONUS,msg);
				return (int)((float)pc->calMagMinDmg * ( 1.0f + fDmg  * 0.01f ) + pc->calMagMinDmg + fPosBonus);
			}
		}
		break;
	}


	TRACE("Error!! GetPhyMinDmg   ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAttack::GetPhyMaxDmg(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_PHYMAXDMG_NORMAL:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYMAXDMG,msg);
				return (int)(	(float)pc->calPhyMaxDmg	* (1.0f + fDmg * 0.01f) + pc->calMagMaxDmg );
			}
		}
		break;

	case SKILL_PHYMAXDMG_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYMAXDMG,msg);
				return fDmg;
			}	
		}
		break;	
	case SKILL_PHYMAXDMG_MON:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
			if(pMon)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYMAXDMG,msg);
				return fDmg + pMon->calPhyMaxDmg;
			}	
		}
		break;

	case SKILL_PC_PHYMAXDMG_ADD_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				int nGrade = g_SkillDataGlobal.GetGrade(pc->pcJob);

				
				if(nGrade <0 || nGrade >= SKILL_MAX_BASIC_JOB_TYPE) return 0;
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYBONUS_MENTALITY+nGrade,msg);
				pc->calPhyMaxDmg += fDmg;
				return 0;
			}	
		}
		break;

	case SKILL_PHYMAXDMG_MAGIC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMAXDMG,msg);
				return (int)((float)pc->calMagMaxDmg * ( 1.0f + fDmg  * 0.01f ) + pc->calMagMaxDmg );
			}
		}
		break;

	case SKILL_PHYMAXDMG_NORMAL_PHYBONUS :
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg		= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYMAXDMG,msg);
				float fBonus	= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_PHYBONUS,msg);
				return (int)(	(float)pc->calPhyMaxDmg	* (1.0f + fDmg  * 0.01f)  + pc->calMagMaxDmg + fBonus );
			}
		}
		break;	
		
	case SKILL_PHYMAXDMG_MAGIC_PHYBONUS:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg		= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMAXDMG,msg);
				float fPosBonus	= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSBONUS,msg);
				return (int)((float)pc->calMagMaxDmg * ( 1.0f + fDmg  * 0.01f ) + pc->calMagMaxDmg + fPosBonus);
			}
		}
		break;
	}

	TRACE("Error!!   GetPhyMaxDmg ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAttack::GetPosMinDmg(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_POSMINDMG_MON_MAGIC:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
			if(pMon)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMINDMG,msg);
				return fVal + pMon->calMagMinDmg;
			}	
		}
		break;

	case SKILL_PC_POSMINDMG_ADD_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				int nGrade = g_SkillDataGlobal.GetGrade(pc->pcJob);

				
				if(nGrade <0 || nGrade >= SKILL_MAX_BASIC_JOB_TYPE) return 0;
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSBONUS_MENTALITY+nGrade,msg);

				pc->calMagMinDmg += fDmg;
				return 0;
			}	
		}
		break;


	
	case SKILL_POSMINDMG_MAGIC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMINDMG,msg);
				return (int)((float)pc->calMagMinDmg * ( 1.0f + fDmg  * 0.01f ) + pc->calMagMinDmg );
			}
		}
		break;

	
	case SKILL_POSMINDMG_MAGIC_PHYBONUS:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg		= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMINDMG,msg);
				float fPosBonus	= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSBONUS,msg);
				return (int)((float)pc->calMagMinDmg * ( 1.0f + fDmg  * 0.01f ) + pc->calMagMinDmg + fPosBonus);
			}
		}
		break;

	}

	TRACE("Error!!  GetPosMinDmg  ... %d \n",_Type);
	return NO_TYPE;
}



int CSkillProcAttack::GetPosMaxDmg(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_POSMAXDMG_MON_MAGIC:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
			if(pMon)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMAXDMG,msg);
				return fVal + pMon->calMagMaxDmg;
			}	
		}
		break;

	case SKILL_PC_POSMAXDMG_ADD_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				int nGrade = g_SkillDataGlobal.GetGrade(pc->pcJob);

				
				if(nGrade <0 || nGrade >= SKILL_MAX_BASIC_JOB_TYPE) return 0;
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSBONUS_MENTALITY+nGrade,msg);

				pc->calMagMaxDmg += fDmg;
				return 0;
			}	
		}
		break;

	
	case SKILL_POSMAXDMG_MAGIC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMAXDMG,msg);
				return (int)((float)pc->calMagMaxDmg * ( 1.0f + fDmg  * 0.01f ) + pc->calMagMaxDmg );
			}
		}
		break;

	
	case SKILL_POSMAXDMG_MAGIC_PHYBONUS:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fDmg		= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSMAXDMG,msg);
				float fPosBonus	= g_SkillDataGlobal.GetValue(msg->pPc,SKILL_POSBONUS,msg);
				return (int)((float)pc->calMagMaxDmg * ( 1.0f + fDmg  * 0.01f ) + pc->calMagMaxDmg + fPosBonus);
			}
		}
		break;
	}

	TRACE("Error!!   GetPosMaxDmg ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAttack::GetNegMinDmg(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case NUM_SKILL_NEGMINDMG:
		break;
	}

	
	return NO_TYPE;
}



int CSkillProcAttack::GetNegMaxDmg(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case NUM_SKILL_NEGMAXDMG:
		break;
	}

	
	return NO_TYPE;
}



int CSkillProcAttack::GetCriticalRate(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_CRITICALRATE_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_CRITICALRATE,msg);
				return fVal;
			}	
		}
		break;

	case SKILL_CRITICALRATE_CALCRITICALRATE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_CRITICALRATE,msg);
				return fVal + pc->calCriticalRate;
			}	
		}

		break;
	}

	TRACE("Error!! GetCriticalRate   ... %d \n",_Type);
	return NO_TYPE;
}



int CSkillProcAttack::GetCriticalDamage(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_CRITICALDAMAGE_VALUE:
		{
			return 0;
		}
		break;
	}

	TRACE("Error!!   GetCriticalDamage ... %d \n",_Type);
	return NO_TYPE;
}


int CSkillProcAttack::GetDelayTime(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case ATTACK_DELAY_PC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				return pc->calAttackDelay;
			}	
		}
		break;
	case ATTACK_DELAY_MONSTER:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
			if(pMon)
			{
				return pMon->calAttackDelay;
			}	
		}
		break;
	case ATTACK_DELAY_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_DELAYTIME,msg);
				return fVal;
			}	
		}
		break;
	}

	TRACE("Error!!   GetDelayTime \n");
	return NO_TYPE;
}



int CSkillProcAttack::GetRepeatNum(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_REPEATNUM_VALUE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_REPEATNUM,msg);
				return fVal;			
			}	
		}
		break;
	case SKILL_REPEATNUM_CAL:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fKeepTime = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_AFF_KEEPTIME,msg);
				float fRepeatTime = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_REPEAT_DELAY_TIME,msg);

				int nVal		= (int)( fKeepTime / fRepeatTime );
				return nVal;			
			}	
		}
		break;
	}

	
	return NO_TYPE;
}


int CSkillProcAttack::GetRepeatDelayTime(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_REPEATDELAYTIME_VALUE:
		{
			return 0;
		}
		break;
	}

	
	return NO_TYPE;
}


int CSkillProcAttack::GetEtcType(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) 
	{
	case SKILL_ATK_ETC_INFO_STUNPRO:
	case SKILL_ATK_ETC_INFO_STUNTIME:
		{
																	return MSGINFO_ATK_ETC_INFO_STUN;
		}
		break;

	case SKILL_ATK_ETC_INFO_RASTEAL :								return MSGINFO_ATK_ETC_INFO_RASTEAL;
		break;

	case SKILL_ATK_ETC_INFO_STUN:									return MSGINFO_ATK_ETC_INFO_STUN;
		break;

	case SKILL_ATK_ETC_INFO_SHOCK :									return MSGINFO_ATK_ETC_INFO_SHOCK;
		break;

	case SKILL_ATK_ETC_INFO_FREEZE:									return MSGINFO_ATK_ETC_INFO_FREEZE;
		break;

	case SKILL_ATK_ECT_INFO_PUSHED:									return MSGINFO_ATK_ECT_INFO_PUSHED;
		break;

	case SKILL_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER:					return MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER;
		break;

	case SKILL_ATK_ETC_MULTIDAMAGE_DELAY:							return MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
		break;

	case SKILL_ATK_ETC_INFO_HIDE:									return MSGINFO_ATK_ETC_INFO_HIDE;
		break;

	case SKILL_ATK_ETC_MULTIDAMAGE_DELAY_WEAPON_TYPE_MAGIC_BOW:		return MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY;
		break;

	case SKILL_ATK_DAMAGE_TARGET_100_ELSE_LITE_DAMAGE:				return MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_100_ELSE_LITE_DAMAGE;	
		break;

	
	case SKILL_ATK_ETC_RA_RATIO_POS_DAMAGE:							return MSGINFO_ATK_ETC_RA_RATIO_DAMAGE;
		break;
	case SKILL_ATK_ETC_RA_RATIO_PHY_DAMAGE:							return MSGINFO_ATK_ETC_RA_RATIO_DAMAGE;
		break;

	}

	return MSGINFO_ATK_ETC_INFO_NONE;							
}



int CSkillProcAttack::GetEtcInfo(BYTE _Type,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_ATK_ETC_INFO_NONE:
		{
			return 0;
		}
		break;

	case SKILL_ATK_ETC_INFO_RASTEAL:
		break;

	
	case SKILL_ATK_ETC_INFO_STUNPRO:
		{
			if(msg->pDataFormat->m_ClassType == SKILL_CLASS_PC)
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
				if(pc)
				{
					float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ATK_ETC_INFO_STUN_PRO,msg);
					return fVal;
				}
			}
			else if(msg->pDataFormat->m_ClassType == SKILL_CLASS_MON)
			{
				monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
				if(pMon)
				{
					float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ATK_ETC_INFO_STUN_PRO,msg);
					return fVal;
				}
			}
		}
		break;

	
	case SKILL_ATK_ETC_INFO_STUNTIME:
		{
			if(msg->pDataFormat->m_ClassType == SKILL_CLASS_PC)
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
				if(pc)
				{
					float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ATK_ETC_INFO_STUN_TIME,msg);
				
					return fVal;
				}
			}
			else if(msg->pDataFormat->m_ClassType == SKILL_CLASS_MON)
			{
				monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
				if(pMon)
				{
					float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ATK_ETC_INFO_STUN_TIME,msg);
				
					return fVal;
				}
			}
		}
		break;

	case SKILL_ATK_ETC_INFO_SHOCK:
		break;

	case SKILL_ATK_ETC_INFO_FREEZE:
		break;

	case SKILL_ATK_ECT_INFO_PUSHED:
		{
			return 0;
		}
		break;

	case SKILL_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER:
		break;

	case SKILL_ATK_ETC_MULTIDAMAGE_DELAY:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
			if(pc)
			{
				float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ETC_MULTIDAMAGE_DELAY,msg);
				return fVal;
			}	
		}
		break;

		case SKILL_ATK_ETC_MULTIDAMAGE_DELAY_INC:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
				if(pc)
				{
					float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ETC_MULTIDAMAGE_DELAY_INC,msg);
					return fVal;
				}	
			}
			break;

	case SKILL_ATK_ETC_INFO_HIDE:
		break;
		
		
	case SKILL_ATK_ETC_RA_RATIO_POS_DAMAGE:
			{
				if(msg->pDataFormat->m_ClassType == SKILL_CLASS_PC)
				{
					playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
					if(pc)
					{
						float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ETC_RA_RATIO_POS_DAMAGE,msg);
					
						return fVal;
					}
				}
				else if(msg->pDataFormat->m_ClassType == SKILL_CLASS_MON)
				{
					monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
					if(pMon)
					{
						float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ETC_RA_RATIO_POS_DAMAGE,msg);
					
						return fVal;
					}
				}	
			}
		break;


		
	case SKILL_ATK_ETC_RA_RATIO_PHY_DAMAGE:
			{
				if(msg->pDataFormat->m_ClassType == SKILL_CLASS_PC)
				{
					playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPc);
					if(pc)
					{
						float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ETC_RA_RATIO_PHY_DAMAGE,msg);
					
						return fVal;
					}
				}
				else if(msg->pDataFormat->m_ClassType == SKILL_CLASS_MON)
				{
					monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
					if(pMon)
					{
						float fVal = g_SkillDataGlobal.GetValue(msg->pPc,SKILL_ETC_RA_RATIO_PHY_DAMAGE,msg);
					
						return fVal;
					}
				}	
			}
		break;
	}

	
	return NO_TYPE;
}
