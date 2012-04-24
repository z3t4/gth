



#include "../Global.h"





CSkillDataSubProc g_SkillDataSubProc;

CSkillDataSubProc::CSkillDataSubProc()
{

}

CSkillDataSubProc::~CSkillDataSubProc()
{

}


bool CSkillDataSubProc::SubActionProc(void *pPc,CStr_Skill_Msg *msg)
{
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(msg->nSkillID);
	if(pSkillTable==NULL) return false;

	CSkillDataFormat *pDataFormat = pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return false;

	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	

	
	for(int i=0;i<MAX_SKILL_SUB_ACTION;i++)
	{
		switch(pDataFormat->m_SubAtion[i].m_Type)
		{
		
		case SKILL_SUB_ACTION_MORE_AFFECT :
			{
				
				CStr_Skill_Msg SubMsg;

				SubMsg.nSkillID			=(int)pDataFormat->m_SubAtion[i].m_fValue;
				SubMsg.nMySkillIndex	= msg->nMySkillIndex;

				
				SubMsg.pcIdx = msg->pcIdx;
				SubMsg.monsterIdx = msg->monsterIdx;

				g_SkillProc.Active(pPc,&SubMsg);
			}
			break;
		case SKILL_SUB_ACTION_DELETEAFFECT:
			{
				
				if(pc)	
				{
					int nVal = (int) pDataFormat->m_SubAtion[i].m_fValue;
					GTH_DeleteAffect( pc, ENTITY_PC, nVal);
				}
			}
			break;

		case SKILL_SUB_ACTION_ADDMONSTER_CONTRIBUTIONPOINT:
			{
				if(pc)
				{
					if( pc->targetType == ENTITY_MONSTER )
					{
						float fVal = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_LISTPOINT,msg);
						AddMonster_ContributionPoint(pc, pc->targetIdx, ATTACK_POINT, fVal);	
					}
				}
			}
		case SKILL_SUB_ACTION_ATTRACT_ATTENTION:
			SKILL_AttractAttention(pPc,msg);
			break;
		}
	}

	return true;
}




void	CSkillDataSubProc::SKILL_AttractAttention(void *pPc,CStr_Skill_Msg *msg)
{
	float fLevel, fPanelty;

	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);

	if(!SAFE_MONSTER(pc->targetIdx)) return ;
	if( g_monster[pc->targetIdx].aliveState != MON_ALIVE_STATE_NORMAL )	return;

	fLevel = fabs((float)(g_monster[pc->targetIdx].basic.level - pc->level));

	if( fLevel > 33.0f )	fLevel = 33.0f;
	fPanelty = 1.0f - fLevel * 0.03f;
	
	float fHitPro = g_SkillDataGlobal.GetValue(pPc,SKILL_HITPOWER,msg);
	fHitPro *= fPanelty;

	if( GTH_Random() < fHitPro )
	{
		g_monster[pc->targetIdx].targetIdx = pc->idx;
		g_monster[pc->targetIdx].targetType = ENTITY_PC;

		
		g_monster[pc->targetIdx].ai.callProcedureAddr = g_monster[pc->targetIdx].ai.aiProcedureAddr[1];
		g_monster[pc->targetIdx].ai.aiAddr = g_monster[pc->targetIdx].ai.aiProcedureAddr[1];
		g_monster[pc->targetIdx].ai.isCall = true;
		
		
		AI_StopPathFinding( &g_monster[pc->targetIdx] );
		AI_InterpretImmediately( &g_monster[pc->targetIdx] );
	}
	else
	{
		
	}
}


