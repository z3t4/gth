 



#include "../global.h"
#include "SkillApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define CHARACTER_DEFAULT_GEN_LEVEL	 1

CSkillCastData	g_SkillCastDataList;
CSkillDataGlobal g_SkillDataGlobal;
CSkillDataTableList g_SkillDataTableList;







CSkillDataGlobal::CSkillDataGlobal()
{

}

CSkillDataGlobal::~CSkillDataGlobal()
{
	Clear();
}

void CSkillDataGlobal::Clear()
{
	g_SkillCastDataList.Clear();
	g_SkillDataTableList.Clear();

	g_SkillTableDB.ClearDB();
}

float CSkillDataGlobal::GetValue(void *pPc,BYTE _Type,CStr_Skill_Msg *msg,int _nSkillID,int _nMySkillIndex)
{
	int nSkillID=_nSkillID;
	int nMySkillIndex=_nMySkillIndex;

	if(msg)
	{
		nSkillID = msg->nSkillID;
		nMySkillIndex = msg->nMySkillIndex;
	}
	float fError = 0.0f;

	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(nSkillID);
	if(pSkillTable==NULL) return fError;

	CSkillDataFormat *pDataFormat			= pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return fError;

	for(int i=0;i<MAX_SKILL_VALUE_LIST;i++)
	{

		if(pDataFormat->m_ValueList[i].m_Type == _Type)
		{
			
			CSkillTableData *pTable = g_SkillDataTableList.Get(_Type,pDataFormat->m_ValueList[i].m_nIndex-1);
			if(pTable == NULL) 
			{
				
				break;
			}

			
			int GenLevel = CHARACTER_DEFAULT_GEN_LEVEL;

			if(msg != NULL && msg->nSkillLevel > 0)
			{
				GenLevel = msg->nSkillLevel;
			}
			
			else if(msg != NULL && msg->pPartyMeber != NULL)
			{	
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(msg->pPartyMeber);
				if(pc !=NULL && (nMySkillIndex >=0 && nMySkillIndex <MAX_NUMBER_OF_SKILL ))
				{
					GenLevel = pc->skill[nMySkillIndex].level.gen[pTable->m_bGenGrade];					
				}
			}
			
			
			else if(pTable->m_bGenGrade < NUM_JOB_TYPE)
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				if(pc !=NULL && (nMySkillIndex >=0 && nMySkillIndex <MAX_NUMBER_OF_SKILL ))
				{
					GenLevel = pc->skill[nMySkillIndex].level.gen[pTable->m_bGenGrade];					
				}
			}
			
			
			
			if(pTable->m_bMaxLv < GenLevel)
			{
				GenLevel = pTable->m_bMaxLv;
			}

			if(GenLevel <= 0) GenLevel=CHARACTER_DEFAULT_GEN_LEVEL;

			if(GenLevel ==CHARACTER_DEFAULT_GEN_LEVEL)
			{
				return pTable->m_fValue;
			}
			else
			{
				CSkillTableData *pvalue =
					g_SkillDataTableList.Get(_Type,pDataFormat->m_ValueList[i].m_nIndex-1+GenLevel-CHARACTER_DEFAULT_GEN_LEVEL);

				if(pvalue == NULL) 
				{
					
					break;
				}

				return pvalue->m_fValue;			
			}
		}
		else if(!pDataFormat->m_ValueList[i].m_Type)
		{
			break;
		}
	}

#ifdef SKILL_TEST_VERSION
	TRACE("Error!!  CSkillProcAttack::GetValue ...Type  %d .. SkillID %d \n",_Type,nSkillID);
#endif	
	
	return fError;
}

int		CSkillDataGlobal::GetGenLevel(void *pPc,int nSkillID,int _nMySkillIdx)
{
	return CHARACTER_DEFAULT_GEN_LEVEL;

}

int  CSkillDataGlobal::GetGrade(int pcJob)
{
	switch(pcJob)
	{
	case J_MENTALITY:
	case J_DEVOTIONAL_KNIGHT:
	case J_ROMING_KNIGHT:
		return J_MENTALITY;

	case J_WIND	:
	case J_PIECING_EYES:
	case J_WINDY_ROMER:
		return J_WIND;

	case J_WATER:
	case J_DESTROYER:
	case J_GUARDIAN:
		return J_WATER;

	case J_EARTH:
	case J_TRANSFORMER:
	case J_DEFENDER:
		return J_EARTH;
	}
#ifdef SKILL_TEST_VERSION
	TRACE("Error!! GetGrade  %d \n,pcJob");
#endif
	return -1;
} 


CSkillCastValue * CSkillDataGlobal::GetCastValue(void *pPc,void *pMon,int nSkillID,BYTE _Type,int nLevel,int _nMySkillIdx)
{
	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(nSkillID);
	if(pSkillTable==NULL) return NULL;

	CSkillDataFormat *pDataFormat			= pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return NULL;

	CSkillCastValue *pTable =NULL;

	int i;
	
	
	int GenLevel=CHARACTER_DEFAULT_GEN_LEVEL,MaxGenLevel=CHARACTER_DEFAULT_GEN_LEVEL;

	for(i=0;i<MAX_SKILL_VALUE_LIST;i++)
	{
		if(pDataFormat->m_ValueList[i].m_Type)
		{
			if(pDataFormat->m_ValueList[i].m_Type == SKILL_CAST_TABLE_INDEX) continue;

			if(pPc)
			{
				playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
				
				int nGrade = GetGrade(pc->pcJob);
				if(nGrade <0) continue;

				if(_nMySkillIdx > -1 && _nMySkillIdx < MAX_NUMBER_OF_SKILL)
				{
					if(pc)	GenLevel = pc->skill[_nMySkillIdx].level.gen[nGrade];
				}
				else if(pPc != NULL)
				{
					if(pc)
					{
						for(int j=0;j<MAX_NUMBER_OF_SKILL;j++)
						{
							if(pc->skill[j].tableIdx == nSkillID)
							{
								GenLevel = pc->skill[j].level.gen[nGrade];
								break;
							}
							
						}
					}
				}
				else
				{
					GenLevel=nLevel;
				}
			}
			else 
			{
				GenLevel=CHARACTER_DEFAULT_GEN_LEVEL;
			}
		
			if(GenLevel <= 0) GenLevel=CHARACTER_DEFAULT_GEN_LEVEL;

			if(MaxGenLevel < GenLevel)
			{
				MaxGenLevel = GenLevel;
			}
		}
		else if(!pDataFormat->m_ValueList[i].m_Type)
		{
			break;
		}
	}
	
	for(i=0;i<MAX_SKILL_VALUE_LIST;i++)
	{
		if(pDataFormat->m_ValueList[i].m_Type == _Type)
		{
			
			pTable = g_SkillCastDataList.Get(pDataFormat->m_ValueList[i].m_nIndex);
			if(pTable == NULL) 
			{
				return NULL;
			}

			CSkillCastValue *pLevelValue =
				g_SkillCastDataList.Get(pDataFormat->m_ValueList[i].m_nIndex+(MaxGenLevel-CHARACTER_DEFAULT_GEN_LEVEL));

			if(pLevelValue == NULL) return pTable;
			return pLevelValue;	

		}
		else if(!pDataFormat->m_ValueList[i].m_Type)
		{
			break;
		}
	}

#ifdef SKILL_TEST_VERSION	
	TRACE("Error! GetCastValue nSkillID %d, _Type %d\n", nSkillID , _Type );
#endif

	return NULL;
}

playerCharacter_t* CSkillDataGlobal::GetPCPointer(const int Idx)
{
	if((Idx < 0) || (Idx >= MAX_PCS)) 
	{
		
		return NULL;
	}

	if(FALSE == g_pc[Idx].active) 
	{
		
		return NULL;
	}

	return &g_pc[Idx];
}

int CSkillDataGlobal::GetLimitlevel(int skillTableIdx,int nGenType)
{
	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL) return NULL;

	CSkillDataFormat *pDataFormat			= pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return 0;

	int nGenLevel=0;

	for(int i=0;i<MAX_SKILL_VALUE_LIST;i++)
	{
		if(pDataFormat->m_ValueList[i].m_Type)
		{
			
			CSkillTableData *pTable = g_SkillDataTableList.Get(pDataFormat->m_ValueList[i].m_Type,pDataFormat->m_ValueList[i].m_nIndex-1);
			if(pTable == NULL) continue;

			
			if(pTable->m_bMaxLv > nGenLevel)
			{
				nGenLevel = pTable->m_bMaxLv;
			}
		}
		else if(!pDataFormat->m_ValueList[i].m_Type)
		{
			
			break;
		}
	}

	return nGenLevel;
}

int CSkillDataGlobal::GetGradeClass(int skillTableIdx,int nGenType)
{
	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL) return NULL;

	CSkillDataFormat *pDataFormat			= pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return 0;

	int nGenCapablity=0;

	for(int i=0;i<MAX_SKILL_VALUE_LIST;i++)
	{
		if(pDataFormat->m_ValueList[i].m_Type)
		{
			if( pDataFormat->m_ValueList[i].m_Type == SKILL_CAST_TABLE_INDEX) continue;

			
			CSkillTableData *pTable = g_SkillDataTableList.Get(pDataFormat->m_ValueList[i].m_Type,pDataFormat->m_ValueList[i].m_nIndex-1);
			if(pTable == NULL) continue;

			
			if(pTable->m_bGenGrade == nGenType && pTable->m_bGenCapablity > nGenCapablity)
			{
				nGenCapablity = pTable->m_bGenCapablity;
			}
		}
		else if(!pDataFormat->m_ValueList[i].m_Type)
		{
			
			break;
		}
	}
	return nGenCapablity;
}



int CSkillDataGlobal::GetMessageType(int Type, message_t *message )
{
	for(int i=0;i<MAX_SKILL_MESSAGE_DATA;i++)
	{
		if(message->Type[i] == Type)
		{
			return message->data[i];
		}
	}

	return 0;
}


int CSkillDataGlobal::IsDataFormatType(const int skillTableIdx,const int Type)
{
	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL) return 0;

	CSkillDataFormat *pDataFormat			= pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return 0;

	for(int i=0;i<CSkillDataFormat::MAX_NUM_OF_DATA;i++)
	{
		if(0 == pDataFormat->m_Data[i]) return 0;

		if(Type == pDataFormat->m_Data[i])
		{
			return true;
		}
	}

	return 0;
}
