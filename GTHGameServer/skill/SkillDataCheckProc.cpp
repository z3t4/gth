



#include "../Global.h"




CSkillDataCheckProc g_SkillDataCheckProc;

CSkillDataCheckProc::CSkillDataCheckProc()
{

}

CSkillDataCheckProc::~CSkillDataCheckProc()
{

}

bool CSkillDataCheckProc::CheckSkillProc(void *pPc,CStr_Skill_Msg *msg)
{
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(msg->nSkillID);
	if(pSkillTable==NULL) return false;

	CSkillDataFormat *pDataFormat = pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return false;
	

	for(int i=0;i<MAX_SKILL_CHECK_FLAG;i++)
	{
		switch(pDataFormat->m_CheckFlag[i]) {

		case CHECK_PC:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;
			}
			break;
		case CHECK_SKILL_MON_ALIVE_CHECK:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;
				if(pc->targetIdx >= 0 && pc->targetIdx < MAX_MONSTERS)
				{
					if( g_monster[pc->targetIdx].aliveState != MON_ALIVE_STATE_NORMAL )	return false;
				}					
			}				
			break;

		case CHECK_SKILL_TO_USE_MY:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;

				
				if ( pc->idx == pc->targetIdx) return false;
				if ( pc->curRA <= 0)  return false;
			}
			break;

		case CHECK_TARGETPC_ALIVE:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;

				switch(pc->targetType)
				{
					case ENTITY_PC :
						if(!SAFE_PCS(pc->targetIdx)) return false;
						if( g_pc[pc->targetIdx].alive ) return false;
						break;
				}
			}
			break;

		case CHECK_TARGETALL_ALIVE:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;

				switch( pc->targetType )
				{
				case ENTITY_PC :
					if(!SAFE_PCS(pc->targetIdx)) return false;
					if( g_pc[pc->targetIdx].alive ) return false;
					break;
					
				case ENTITY_MONSTER : 
				case ENTITY_NPC :		return	false;
				}
			}
			break;

		case CHECK_BREEDRATE:
			{
				
				
				
			}
			break;

		case CHECK_ACTIVEAFFECT:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;

				
				playerCharacter_t* pTargetpc = NULL;
				if ( pc->targetIdx < 0 || pc->targetIdx >= MAX_PCS)		return false;

				pTargetpc = &g_pc[pc->targetIdx];
				if ( pTargetpc == NULL)		return false;
				
				
				if ( GTH_IsActiveAffect(pTargetpc, ENTITY_PC, 38) )	
				{
					GTH_SendMessage_ChattingMsg(pc, 1, 9, 20 );	
					return  false;
				}

				
				if ( GTH_IsActiveAffect(pTargetpc, ENTITY_PC, 34) )	
				{
					GTH_SendMessage_ChattingMsg(pc, 1,  9, 20 );
					return false;
				}
			}
			break;

		case CHECK_GTH_EV_CHAR_DIE:
			{
				monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(pPc);
				if(!pMon) return false;
				if( pMon->targetType == ENTITY_PC )
				{
					if( g_pc[pMon->targetIdx].event == GTH_EV_CHAR_DIE ) return false;
				}
			}
			break;

		case CHECK_CRITICAL_RATE:
			{
				
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;

				float mastercalCriticalRate = g_pc[pc->idx].calCriticalRate;
				if ( mastercalCriticalRate < 0.0f) return false;
			}
			break;

		case CHECK_NOT_USE_MYSELF:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;

				
				if ( pc->idx == pc->targetIdx) return false;
			}
			break;

		
		case CHECK_ONLY_USE_MYSELF:
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(!pc) return false;

				
				if ( pc->idx != pc->targetIdx) return false;
			}
			break;
		}
	}	

	return true;
}

