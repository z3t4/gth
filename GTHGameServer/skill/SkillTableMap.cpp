





#include "../global.h"
#include "SkillApp.h"

CSkillTableMap g_SkillTableDB;

CSkillTableMap::CSkillTableMap()
{
	for(int i=0;i<MAX_SKILL_NUMBER;i++)
	{
		m_SkillDB[i]=NULL;
	}
}

CSkillTableMap::~CSkillTableMap()
{
	ClearDB();
}

void CSkillTableMap::ClearDB()
{
	for(int i=0;i<MAX_SKILL_NUMBER;i++)
	{
		if(m_SkillDB[i] != NULL)
		{
			if(m_SkillDB[i]->m_nId >= 0)
			{
				SAFE_DELETE(m_SkillDB[i]);
			}
		}
	}

}

CSkilTable *CSkillTableMap::FindDB(int nID)
{

	if(nID < 0 || nID >= MAX_SKILL_NUMBER) 
	{
		TRACE(" Error CSkilTable *CSkillTableMap::FindDB( %d)\n",nID);
		return NULL;
	}

	return m_SkillDB[nID];
}

void CSkillTableMap::InsertDB(int nID, CSkilTable *pData)
{
	m_SkillDB[nID] = pData;
}

bool			CSkillTableMap::SAFE_ARRAY(int index)
{
	if(index < 0 || index >= MAX_SKILL_NUMBER) return false;
	return true;
}


