


#include "../global.h"
#include "SkillApp.h"






CSkillTransInfo m_SkillTransInfo;

CSkillTransInfo::CSkillTransInfo()
{

}

CSkillTransInfo::~CSkillTransInfo()
{

}

CSkillTransformUnit * CSkillTransformData::Get(int Index)
{
	if(Index < 0 || Index >= m_TransData.size())
	{
		return NULL;
	}

	return m_TransData[Index];
}

void CSkillTransformData::Insert(CSkillTransformUnit *temp)
{
	m_TransData.push_back(temp);
}

void	CSkillTransformData::Clear()
{
	int size = m_TransData.size();

	for(int i=0;i<size;i++)
	{
		CSkillTransformUnit *temp = m_TransData[i];
		SAFE_DELETE(temp);
	}

	m_TransData.clear();
}


int		CSkillTransformData::GetEffectID(const int _EntityIdx)
{
	int size = m_TransData.size();

	for(int i=0;i<size;i++)
	{
		if(m_TransData[i]->m_nTransformEntityIdx == _EntityIdx)
		{
			return m_TransData[i]->m_nEffectID;
		}
		
	}

	return -1;
}
