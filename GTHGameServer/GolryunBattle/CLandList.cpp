




#include "..\global.h"
#include "CLandList.h"

CRandList::CRandList()
{
	Initialize();
}

CRandList::~CRandList()
{
	m_numList.clear();
}

void CRandList::Initialize()
{
	m_numList.clear();
}

void CRandList::PushData(int iData)
{
	m_numList.push_back(iData);
}


BOOL CRandList::GetRandomData(int& iData)
{
	if (m_numList.empty())
		return FALSE;

	int randomValue=rand()%m_numList.size();
	int i=0;
	NUMLIST_ITOR itor;

	for (itor=m_numList.begin();itor!=m_numList.end();)
	{
		if (i==randomValue)
		{
			iData=(*itor);
			m_numList.erase(itor++);
			return TRUE;
		}
		else
		{
			itor++;
		}
		i++;
	}
	return FALSE;

}


