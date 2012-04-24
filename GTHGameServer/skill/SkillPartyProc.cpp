



#include "../Global.h"





CSkillPartyProc g_SkillPartyProc;

CSkillPartyProc::CSkillPartyProc()
{

}

CSkillPartyProc::~CSkillPartyProc()
{

}

bool CSkillPartyProc::Proc(void *pPc,CStr_Skill_Msg *msg)
{
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(msg->nSkillID);
	if(pSkillTable==NULL) return false;

	CSkillDataFormat *pDataFormat = pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return false;

	CSkillDataParty * pParty = pDataFormat->GetParty();
	if(pParty == NULL) return false;
	
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
	if(pc==NULL) return false;
	
	msg->pParty = pParty;
	int i;
	for(i=0;i<MAX_NUMBER_OF_PARTY_MEMBER; i++)
	{
		if(!Check(pPc,i,msg)) continue;

		
		msg->pPartyMeber = pPc;
		playerCharacter_t* pMemberPc=
			g_SkillDataGlobal.GetPCPointer(pc->member[i].idxOnServer);

		
		
		g_SkillProc.CastActive(pMemberPc,msg);

		
		g_SkillProc.Proc(pMemberPc,msg);	
	}

	return true;
}

bool CSkillPartyProc::Check(void* pPc,int nIndex,CStr_Skill_Msg *msg )
{
	for(int i=0;i<MAX_SKILL_PARTY_CHECK;i++)
	{
		switch(msg->pParty->m_Check[i]) {
		case SKILL_PARTY_CHECK_NORMAL:
			{
				if(!PartyCheck(pPc,nIndex,msg)) return false;
			}
			break;
		case SKILL_PARTY_CHECK_CRITICAL_RATE:
			{
				playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);
				if(pc==NULL) return false;

				
				float IncCriticalRate = g_SkillDataGlobal.GetValue(pPc,SKILL_AFFDATA_TYPE_CRITICALRATE,msg);
				int pcIdx				= pc->member[nIndex].idxOnServer;	

				
				float mastercalCriticalRate = g_pc[pc->idx].calCriticalRate;
				if ( mastercalCriticalRate < 0.0f) return false;

				float calcriticalrate			= IncCriticalRate + g_pc[pc->idx].calCriticalRate;				

				
				if ( pc->idx != pcIdx )
				{
					
					
					if ( IncCriticalRate + mastercalCriticalRate < calcriticalrate)
					calcriticalrate = IncCriticalRate + mastercalCriticalRate;

					
					if ( calcriticalrate <= g_pc[pcIdx].calCriticalRate)			
						continue;
				}
			}
			break;
		}	
	}
	return true;
}

bool CSkillPartyProc::PartyCheck(void* pPc,int nIndex,CStr_Skill_Msg *msg)
{
	playerCharacter_t *pc = static_cast<playerCharacter_t*>(pPc);
	if(pc==NULL) return false;

	if(nIndex < 0 || nIndex >= MAX_NUMBER_OF_PARTY_MEMBER) return false;
	
	
	int pcIdx				= pc->member[nIndex].idxOnServer;				
	if ( pcIdx < 0 || pcIdx >= MAX_PCS)	return false;

	
	if ( pcIdx == pc->idx)	return false;
			
	
	if( pc->member[nIndex].serverIndex < 0 ) 	return false;	

	
	if ( pc->member[nIndex].serverIndex != g_config.gameServerNo ) 		return false;

	
	if ( g_pc[pcIdx].worldIdx != pc->worldIdx ) 	return false;

	
	if ( !g_pc[pcIdx].active || !g_pc[pcIdx].alive ) return false;

	
	if( stricmp( g_pc[pcIdx].name, pc->member[nIndex].memberName ) )  return false;

	
	if ( g_pc[pcIdx].worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index)  return false; 
	
	float fRange = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_RANGE,msg);			
	if(fRange > 0.0f)
	{
		if( GTH_GetDistance2D( pc->position, g_pc[nIndex].position) > fRange )return false;
	}

	return true;
}

bool CSkillPartyProc::IfPartyMember(int MyIndex,int PartyMemberIndex)
{
	if(MyIndex < 0 || MyIndex >= MAX_PCS) return false;
	if(PartyMemberIndex < 0 || PartyMemberIndex >= MAX_PCS) return false;

	int i;
	for(i=0;i<MAX_NUMBER_OF_PARTY_MEMBER; i++)
	{
		if(g_pc[MyIndex].member[i].idxOnServer == PartyMemberIndex)
		{
			return true;
		}
	}
	
	return false;
}