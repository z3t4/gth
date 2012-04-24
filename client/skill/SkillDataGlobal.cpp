




#include "../global_def.h"

CSkillDataGlobal g_SkillDataGlobal;
CSkillCastData	g_SkillCastDataList;
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

CSkillTableData * CSkillDataGlobal::GetValue(BYTE _Type,int _nSkillID,int _nMySkillIndex,int nSkillLevel)
{
	int nSkillID=_nSkillID;
	int nMySkillIndex=_nMySkillIndex;

	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(nSkillID);
	if(pSkillTable==NULL) return NULL;

	CSkillDataFormat *pDataFormat			= pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return NULL;

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

			if(nSkillLevel > 0)
			{
				GenLevel = nSkillLevel;
			}
			
			else if(pTable->m_bGenGrade != J_NONE)
			{				
				GenLevel = g_cgv.myCharacterInfo->skill[nMySkillIndex].level.gen[pTable->m_bGenGrade];
			}
			
			
			if(pTable->m_bMaxLv < GenLevel)
			{
				GenLevel = pTable->m_bMaxLv;
			}

			if(GenLevel <= 0) GenLevel=CHARACTER_DEFAULT_GEN_LEVEL;

			if(GenLevel ==CHARACTER_DEFAULT_GEN_LEVEL)
			{
				return pTable;
			}
			else
			{
				CSkillTableData *pvalue =
					g_SkillDataTableList.Get(_Type,pDataFormat->m_ValueList[i].m_nIndex-1+GenLevel-CHARACTER_DEFAULT_GEN_LEVEL);

				if(pvalue == NULL) 
				{
					
					break;
				}

				return pvalue;			
			}
		}
		else if(!pDataFormat->m_ValueList[i].m_Type)
		{
			break;
		}

	}

	
	return NULL;
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

	
	return -1;
} 


CSkillCastValue * CSkillDataGlobal::GetCastValue(int _Type,int skillTableIdx,int nLevel)
{
	if(skillTableIdx < 0 && skillTableIdx >= MAX_NUMBER_OF_SKILL)
	{
		return NULL;
	}

	CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
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

			int nGrade = GetGrade(g_cgv.myCharacterInfo->pcJob);
			if(nGrade <0) continue;

			GenLevel=nLevel;
			
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
	

	return NULL;
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

