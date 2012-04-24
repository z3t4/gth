



#include "../Global.h"















#include "../Tools/CTools.h"
extern CTools* gcpTools;

CSkillCast g_SkillCast;

CSkillCast::CSkillCast()
{

}

CSkillCast::~CSkillCast()
{

}

int CSkillCast::RaProc(BYTE _Type,void *pPc,CStr_Skill_Msg *msg)
{
	int nSkillID = msg->nSkillID;
	int nMySkillIndex = msg->nMySkillIndex;
	

	switch(_Type) {
	case SKILL_RA_TARGETIDX:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(!pc) 
			{
				
				break;
			}

			float raRecovery = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_RA_RECOVERY ,msg);

			playerCharacter_t* pTargetPlayer=gcpTools->GetPlayerRecordPointer(pc->targetIdx);
			if(NULL == pTargetPlayer) return false;

			pTargetPlayer->curRA += raRecovery;
			pTargetPlayer->curRA += (float)raRecovery * pTargetPlayer->optValue[OV_HEAL_EFFICIENCY] * 0.01f;

			if(pTargetPlayer->curRA > pTargetPlayer->calMaxRA ) 
					pTargetPlayer->curRA = pTargetPlayer->calMaxRA;
			
			return true;
		}
		break;

	
	case SKILL_RA_ENTITY_TYPE_ENTITYPC:
		{
			if(msg->pDataFormat->m_ClassType == SKILL_CLASS_PC)
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc)
				{
					break;
				}

				float raRecovery = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_RA_RECOVERY,msg);
				
				switch( pc->entityType){
					case ENTITY_PC :
						pc->curRA += raRecovery;
						pc->curRA += (float)raRecovery  * pc->optValue[OV_HEAL_EFFICIENCY] * 0.01f;
						if( pc->curRA > pc->calMaxRA ) pc->curRA = pc->calMaxRA;

						return true;
				}			
			}
			else if(msg->pDataFormat->m_ClassType == SKILL_CLASS_MON)
			{
				monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(msg->pPc);
				if(!pMon)
				{
					break;
				}

				float raRecovery = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_RA_RECOVERY,msg);

				switch( pMon->entityType)
				{	
				case ENTITY_MONSTER :
					pMon->curRA += raRecovery;

					if( pMon->curRA > pMon->calMaxRA )	pMon->curRA = pMon->calMaxRA;
					return true;
				}
			}
		}

		break;
	case SKILL_RA_TARGETTYPE_ENTITY_PC:
		break;
	case SKILL_RA_LAYHAND:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)
			{
				
				float fRADecRate = g_SkillDataGlobal.GetValue(pPc,SKILL_RA_DEC_RATE,msg);
				float fRAIncreRate = g_SkillDataGlobal.GetValue(pPc,SKILL_RA_INC_RATE,msg);
					
				
				if ( pc->idx == pc->targetIdx) return false;
				if ( pc->curRA <= 0)  return	false;
				
				float incVel  = pc->curRA * ( fRADecRate / 100 );

				if ( incVel <= 0)  return false;

				
				
				playerCharacter_t* pTargetPlayer=gcpTools->GetPlayerRecordPointer(pc->targetIdx);
				if(NULL == pTargetPlayer) return false;


				pc->curRA				  -= incVel;
				incVel					  *= fRAIncreRate;
				pTargetPlayer->curRA	  += incVel;
				if(pTargetPlayer->curRA > pTargetPlayer->calMaxRA ) 
					pTargetPlayer->curRA = pTargetPlayer->calMaxRA;
				return true;
			}
		}
		break;

	case SKILL_RA_PC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(!pc) return false;

			float raRecovery = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_RA_RECOVERY,msg);

			pc->curRA += raRecovery;
			pc->curRA += (float)raRecovery * pc->optValue[OV_HEAL_EFFICIENCY] * 0.01f;	
			if(pc->curRA > pc->calMaxRA) pc->curRA = pc->calMaxRA;
			return true;
		}
		break;

	}

	TRACE("Error!!  RaProc %d \n",_Type);
	return NO_TYPE;
}





