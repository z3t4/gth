



#include "../global.h"
#include "SkillApp.h"





CSkillProcAffectSpe::CSkillProcAffectSpe()
{

}

CSkillProcAffectSpe::~CSkillProcAffectSpe()
{

}

int CSkillProcAffectSpe::GetType(BYTE _Type)
{
	switch(_Type) {
	case SKILL_AFF_SPE_LEVEL_VALUE:		return 0;
		break;
	}
	TRACE("Error!!  CSkillProcAffectSpe::GetType  \n");
	return NO_TYPE;
}



int CSkillProcAffectSpe::GetSpecialIdx(BYTE _Type,void *pPc,CStr_Skill_Msg *msg)
{
	switch(_Type) {
	case SKILL_AFF_SPE_SPECIALIDX_TABLEIDX:
		{
			return msg->msginfo->affect.skillTableIdx;
		}
		break;
	}

	TRACE("Error!! GetSpecialIdx   \n");
	return NO_TYPE;
}
			

float CSkillProcAffectSpe::GetSpecialData(BYTE _Type,BYTE _Index, void *pPc,CStr_Skill_Msg *msg)
{
	float fVal = g_SkillDataGlobal.GetValue(pPc,_Index,msg);
	return fVal;
}


bool CSkillProcAffectSpe::Proc(int nSkillID,void *pPc,AFFECTDATA_SPECIAL *spe)
{
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(nSkillID);
	if(pSkillTable==NULL) return false;

	CSkillDataFormat *pDataFormat = pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return false;

	CSkillDataFormat_Affect *pAffect	= pDataFormat->GetAffect();
	if(pAffect == NULL) return false;

	CSkillDataAffectSpe *pSpe			= pAffect->GetSpe();
	if(pSpe == NULL) return false;

	if(!pSpe->m_bUse) return false;

	playerCharacter_t *pc= static_cast<playerCharacter_t*>(pPc);
	if(pc == NULL) return false;

	pc->isTransform			= pSpe->m_IsTransform;
	pc->transformEntityIdx	= pSpe->m_TransformEntityIndex;
	pc->transformType		= pSpe->m_TransformType-1;

	
	int nMySkillIndex = spe->specialData[MAX_COUNT_AFFECT_SPE_DATA-1];

	for(int i=0;i<MAX_COUNT_AFFECT_SPE_DATA-1;i++)
	{
		float fVal = g_SkillDataGlobal.GetValue(pPc,pSpe->m_Data[i].m_nIndex,NULL,nSkillID);
		switch(pSpe->m_Data[i].m_Type)
		{
		case SKILL_AFF_SPEC_VELOCITY:
			{
				pc->calVelocity		+= pc->baseVelocity * ( fVal * 0.01f );
			}
			break;
		case SKILL_AFF_SPEC_MAXRA:
			{				
				pc->calMaxRA		+= fVal;
				if( pc->curRA > pc->calMaxRA )	pc->curRA = pc->calMaxRA;
			}
			break;
		case SKILL_AFF_SPEC_PHYMINDMG:
			{
				pc->calPhyMinDmg	+= (int)( (float)pc->basePhyMinDmg * (float)fVal * 0.01f );				
			}
			break;
		case SKILL_AFF_SPEC_PHYMAXDMG:
			{
				pc->calPhyMaxDmg	+= (int)( (float)pc->basePhyMaxDmg * (float)fVal * 0.01f );
			}
			break;
		case SKILL_AFF_SPEC_HIT_POWER:
			{
				pc->calHitPower		+= (int)( (float)pc->baseHitPower * fVal * 0.01f );
			}
			break;
		case SKILL_AFF_SPEC_AVOID_POWER:
			{
				pc->calAvoidPower	+= (int)( (float)pc->baseAvoidPower * fVal * 0.01f );
			}
			break;
		case SKILL_AFF_SPEC_ATTACK_SPPED_RATE:
			{
				pc->attackSpeedRate += fVal;
			}
			break;
		default: 
			return false;
		}
	}	
	return true;
}	



				
											
	
