





#include "../global_def.h"

CSkillTableMap g_SkillTableDB;

CSkillTableMap::CSkillTableMap()
{

}

CSkillTableMap::~CSkillTableMap()
{
	ClearDB();
}

void CSkillTableMap::ClearDB()
{
	int size = m_SkillDB.size();
	for(int i=0;i<size;i++)
	{
		SAFE_DELETE(m_SkillDB[i]);
	}

	m_SkillDB.clear();
}

CSkilTable *CSkillTableMap::FindDB(int nID)
{
	int size = m_SkillDB.size();

	if(nID < 0 || nID >= size) 
	{
	
		return NULL;
	}

	return m_SkillDB[nID];
}

void CSkillTableMap::InsertDB(int nID, CSkilTable *pData)
{
	m_SkillDB.push_back(pData);
}


LPSTR		CSkillTableMap::GetSkillName(int index)
{
	if(index < 0 || index >= m_SkillDB.size()) return "NULL";

	
	return  g_LPACK.Get(LPACK_TYPE_SKILL,m_SkillDB[index]->m_NameIdx);
}





bool			CSkillTableMap::SAFE_ARRAY(int index)
{
	if(index < 0 || index >= m_SkillList.size()) return false;
	return true;
}

int				CSkillTableMap::GetID(int index)
{
	if(!SAFE_ARRAY(index)) return -1;
	return m_SkillList[index];
}

