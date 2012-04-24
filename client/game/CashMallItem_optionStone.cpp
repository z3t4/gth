


#include "../global_def.h"


void CashMallItem_OptionStone::InitCashMallItem_OptionStone()
{
	m_iEditOptionItemIdx = -1;
	m_iUsingItemInvenIdx = -1;	
	m_iUsingItemMode	 = NONE_ITEM;
	m_CashMallItem_DestroyRate = 0.0f;
	
	m_CashMallItem_SuccecsRate = 0.0f;
	
	memset(m_iSortOptionIdx, -1, MAX_NUMBER_OF_ITEM_OPTION);
}
