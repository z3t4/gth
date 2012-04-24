



#include "../global.h"
#include "SkillApp.h"






CSkillItem g_SkillItem;

CSkillItem::CSkillItem()
{

}

CSkillItem::~CSkillItem()
{

}


bool CSkillItem::IfSkillItem(int itemTableIdx)
{

	CMap_SkillItemData::iterator iter;
	iter=m_mapSkillItem.find(itemTableIdx);

	if(iter==m_mapSkillItem.end())	return false;

	return true;
}

int CSkillItem::GetLevel(int itemTableIdx)
{
	CMap_SkillItemData::iterator iter;
	iter=m_mapSkillItem.find(itemTableIdx);

	if(iter==m_mapSkillItem.end())	return -1;

	int SkillItemIndex = (iter->second);

	CSkillItemSet *pSet=  m_SkillItemData.Get(SkillItemIndex);
	if(pSet == NULL) return -1;

	for(int i=0;i<MAX_SKILLITEM_UNIT;i++)
	{
		if(pSet->m_ItemList[i].m_nItemTableIdx < 0) break;

		if(pSet->m_ItemList[i].m_nItemTableIdx == itemTableIdx)
		{
			return pSet->m_ItemList[i].m_nLevel;
		}
	}

	return -1;
		
}

int CSkillItem::GetSkillTableIndex(int itemTableIdx)
{
	CMap_SkillItemData::iterator iter;
	iter=m_mapSkillItem.find(itemTableIdx);

	if(iter==m_mapSkillItem.end())	return -1;

	int SkillItemIndex = (iter->second);
	CSkillItemSet *pSet=  m_SkillItemData.Get(SkillItemIndex);
	if(pSet == NULL) return -1;

	return pSet->m_nSkillID;
}


void CSkillItem::SetDataList()
{
	int size = m_SkillItemData.m_SkillItemList.size();

	for(int i=0;i<size;i++)
	{
		CSkillItemSet *pSet=  m_SkillItemData.Get(i);
		if(pSet)
		{
			for(int j=0;j<MAX_SKILLITEM_UNIT;j++)
			{
				if(pSet->m_ItemList[j].m_nItemTableIdx)
				{
					m_mapSkillItem.insert(CMap_SkillItemData::value_type(
						
						pSet->m_ItemList[j].m_nItemTableIdx, 

						
						i));
				}
				else
				{
					break;
				}
			}
			
		}
	}
	
}


bool CSkillItem::IsType_UseItemID(int itemTableIdx, int Type)
{
	CSkillItemSet * pSkillItem= GetSkillItem_UseItemTableIdx(itemTableIdx);
	if(NULL == pSkillItem) return false;

	for(int i=0;i<MAX_SKILLITEM_UNIT;i++)
	{
		if(pSkillItem->m_ItemList[i].m_nItemTableIdx < 0) break;

		if(pSkillItem->m_ItemList[i].m_nType == Type)
		{
			return true;
		}
	}

	return false;
}


CSkillItemSet * CSkillItem::GetSkillItem_UseItemTableIdx(int itemTableIdx)
{
	CMap_SkillItemData::iterator iter;
	iter=m_mapSkillItem.find(itemTableIdx);	

	if(iter==m_mapSkillItem.end())	return NULL;

	int SkillItemIndex = (iter->second);

	CSkillItemSet *pSet=  m_SkillItemData.Get(SkillItemIndex);
	if(pSet == NULL) return NULL;

	for(int i=0;i<MAX_SKILLITEM_UNIT;i++)
	{
		if(pSet->m_ItemList[i].m_nItemTableIdx < 0) break;

		if(pSet->m_ItemList[i].m_nItemTableIdx == itemTableIdx)
		{
			return pSet;
		}
	}

	return NULL;
}

CSkillItemSet * CSkillItem::GetSkillItem_UseSkillTableIdx(int SkillTableIdx)
{
	int size = m_SkillItemData.m_SkillItemList.size();

	for(int i=0;i<size;i++)
	{
		CSkillItemSet *pSet=  m_SkillItemData.Get(i);
		if(pSet)
		{
			if(pSet->m_nSkillID == SkillTableIdx)	
			{
				return pSet;
			}
		}
	}

	return NULL;
}


bool CSkillItem::IsType_UseSkillID(int SkillTableIdx, int Type)
{
	CSkillItemSet * pSkillItem= GetSkillItem_UseSkillTableIdx(SkillTableIdx);
	if(NULL == pSkillItem) return false;

	for(int i=0;i<MAX_SKILLITEM_UNIT;i++)
	{
		if(pSkillItem->m_ItemList[i].m_nItemTableIdx < 0) break;

		if(pSkillItem->m_ItemList[i].m_nType == Type)
		{
			return true;
		}
	}

	return false;
}
