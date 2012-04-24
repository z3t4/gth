


#include "../global.h"
#include "SkillApp.h"





CSkillProcAffectCommon::CSkillProcAffectCommon()
{

}

CSkillProcAffectCommon::~CSkillProcAffectCommon()
{

}



short CSkillProcAffectCommon::GetData(int idx,BYTE _Type,BYTE _Index,void *pPc,CStr_Skill_Msg *msg)
{
	float fVal = g_SkillDataGlobal.GetValue(pPc,_Index,msg);
	if(fVal == 0.0f)
	{
		if(msg)		TRACE("Error!!   GetData ... SkillID %d ... idx %d, _Type %d, _Index %d \n",msg->nSkillID,idx,_Type,_Index);
		
	}

	msg->msginfo->affect.common.type[idx] = _Type;
	msg->msginfo->affect.common.data[idx] = fVal;
	return true;
}


short CSkillProcAffectCommon::DeleteBuffSkill(void *pPc)
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
	if(NULL == pc) return 0;
	

	for( int i = 0; i < pc->affectNumber; i ++ )
			{
		CSkilTable *pSkillTable = g_SkillTableDB.FindDB(pc->affect[i].skillTableIdx);
		if(NULL != pSkillTable)
				{
			for(int j=0;j<CSkillDataFormat::MAX_NUM_OF_DATA;j++)
			{
				if(0 == pSkillTable->m_DataFormat.m_Data[j]) break;

				if(SKILL_UNIQUE_TYPE_BUFF == pSkillTable->m_DataFormat.m_Data[j])
				{
					pc->affect[i].forcedDelete = true;
			
					break;
				}
			}
				}
			}

	return 0;
}


short CSkillProcAffectCommon::OnceAffectProcess(int idx,BYTE _Type,BYTE _Index,void *pPc,CStr_Skill_Msg *msg)
{
	float fVal = g_SkillDataGlobal.GetValue(pPc,_Index,msg);
	if(fVal == 0.0f)
				{
		if(msg)		TRACE("Error!!   GetData ... SkillID %d ... idx %d, _Type %d, _Index %d \n",msg->nSkillID,idx,_Type,_Index);

			}

	switch(_Type)
			{
		
		case AFF_DATA_TYPE_DECREASE_SA:
			{
				playerCharacter_t *pPlayer	=	static_cast<playerCharacter_t*>(pPc);
				if(pPlayer !=NULL)
				{
					pPlayer->curSA		+= (float)fVal;
					if( pPlayer->curSA < 0 )	pPlayer->curSA = 0;
				}
			}
			break;

		
		case AFF_DATA_TYPE_DELETE_BUFF_SKILL:
			{
				DeleteBuffSkill(pPc);
			}
			break;

	}	
	return true;
}
