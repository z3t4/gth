



#include "../global_def.h"






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

bool CSkillItem::CheckValid_UseSkillItem(const int _invenPos)
{
	int itemIdx		= g_cgv.myCharacterInfo->inventory[_invenPos];

	if ( itemIdx < 0 )	return false;

	item_t *item		= &g_cgv.myCharacterInfo->item[itemIdx];
	if ( item == NULL)	return false;

	int skillTableIdx = GetSkillTableIndex(item->itemTableIdx);
	if(skillTableIdx < 0) return false;

	g_SkillFile.DebugOutString("CheckValid_UseSkillItem");

	if(!g_SkillLogic.GTH_SkillProcess(globalVariable_t::SELECT_ITEM,_invenPos))
	{
		return false;
	}
	return true;
}

int CSkillItem::GetSkillItemTableIndex(int InvenPos)
{
	if(InvenPos < 0) return -1;

	int itemIdx		= g_cgv.myCharacterInfo->inventory[InvenPos];
	if ( itemIdx < 0 )	return -1;

	item_t *item		= &g_cgv.myCharacterInfo->item[itemIdx];
	if ( item == NULL)	return -1;

	return g_SkillItem.GetSkillTableIndex(item->itemTableIdx);	
}


bool CSkillItem::IsType(int itemTableIdx, int Type)
{
	CSkillItemSet * pSkillItem= GetSkillItem(itemTableIdx);
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


CSkillItemSet * CSkillItem::GetSkillItem(int itemTableIdx)
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
