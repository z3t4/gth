



#include "../Global.h"
#include "CTempPlayerInventoryX.h"
#include "../Tools/CTools.h"
extern CTools* gcpTools;




CTempPlayerInventoryX::CTempPlayerInventoryX(const playerCharacter_t* pPlayer)
{
	m_bOK=FALSE;
	m_curWeight=0;
	m_maxWeight=pPlayer->calItemWeight;

	if(FALSE == Init(pPlayer)) return;
	
	m_bOK=FALSE;
}






CTempPlayerInventoryX::~CTempPlayerInventoryX()
{

}





BOOL CTempPlayerInventoryX::isOK(void) const
{
	return m_bOK;
}




BOOL CTempPlayerInventoryX::Init(const playerCharacter_t* pPlayer)
{
	int ItemArrayidx=-1;	
	int InventoryIdx=-1;		
	item_t* pItem=NULL;
	itemTable_t* pItemTable=NULL;
	m_curWeight=0;


	for(InventoryIdx=0; InventoryIdx < MAX_INVENTORY_SIZE; InventoryIdx++){
		m_TempInventory[InventoryIdx].TableIdx=-1;
		m_TempInventory[InventoryIdx].numberIdx=-1;

		pItem = gcpTools->GetMyItem((playerCharacter_t*)pPlayer,pPlayer->inventory[InventoryIdx]);
		if(NULL == pItem) continue;	
		pItemTable = gcpTools->GetItemTablePointer(pItem->itemTableIdx);
		if(NULL == pItemTable) continue;

		m_curWeight = m_curWeight + pItemTable->weight;

		m_TempInventory[InventoryIdx].TableIdx = pItem->itemTableIdx;
		if(pItemTable->stackFlag) m_TempInventory[InventoryIdx].numberIdx= pItem->durability;
		else m_TempInventory[InventoryIdx].numberIdx=-1;			
	}

	return TRUE;	
}





const CTempPlayerInventoryX::ErrorCode CTempPlayerInventoryX::Insert(const item_t* pItem)
{	
	itemTable_t* pItemTable=NULL;
	pItemTable = gcpTools->GetItemTablePointer(pItem->itemTableIdx);
	if(NULL == pItemTable){		
		
		return ErrorCode_unknown;
	}

	
	int weight = m_curWeight + pItemTable->weight;
	if(weight > m_maxWeight) return ErrorCode_Over_Weight;
	

	
	
	int InventoryIdx=-1;		
	if(pItemTable->stackFlag){
		for(InventoryIdx=0; InventoryIdx < MAX_INVENTORY_SIZE; InventoryIdx++){			
			
			if(m_TempInventory[InventoryIdx].TableIdx != pItem->itemTableIdx) continue;
			int numberIdx = m_TempInventory[InventoryIdx].numberIdx + (pItem->durability + 1);
			
			if(numberIdx >= MAX_COUNT_STACK_ITEM) return ErrorCode_stacknum_overflow;
			m_TempInventory[InventoryIdx].numberIdx = numberIdx;			
			return ErrorCode_Success;
		}
	}


	for(InventoryIdx=0; InventoryIdx < MAX_INVENTORY_SIZE; InventoryIdx++){			
		if(m_TempInventory[InventoryIdx].TableIdx >= 0) continue;
		m_TempInventory[InventoryIdx].TableIdx = pItem->itemTableIdx;
		m_TempInventory[InventoryIdx].numberIdx = pItem->durability;
		
		return ErrorCode_Success;
	}
	return ErrorCode_NotEnough_Inventory;
}



