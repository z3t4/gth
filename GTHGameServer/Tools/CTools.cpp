


#include "../global.h"
#include "CTools.h"




CTools::CTools()
{
	m_bOK=FALSE;
	m_bOK=TRUE;
}






CTools::~CTools()
{

}





void CTools::
IncDecChaosPoint(playerCharacter_t* pPlayer,const int changePoint)
{
	int point =  pPlayer->chaosPoint + changePoint;
	if( point < 0) point=0;
	if( point > CHAOSMAX) point = CHAOSMAX;
	
	pPlayer->chaosPoint = point;
}





BOOL CTools::isOK(void) const 
{
	return m_bOK; 
}





playerCharacter_t* CTools::GetPlayerRecordPointer(const querySet_t&	querySet)
{
	playerCharacter_t* pPlayerRecord = GetPlayerRecordPointer(querySet.ownerIdx);
	if(NULL == pPlayerRecord) return NULL;
	if(0 != strcmp(pPlayerRecord->userID,querySet.ownerID)) return NULL;

	return pPlayerRecord;
}




playerCharacter_t* CTools::GetPlayerRecordPointer(const int Idx)
{
	if(Idx < 0) return NULL;
	if(Idx >= MAX_PCS) return NULL;

	playerCharacter_t* pPlayerRecord=&g_pc[Idx];
	if(!pPlayerRecord->active) return NULL;	
	if(FALSE == pPlayerRecord->flagDBLoading) return NULL;
	

	return pPlayerRecord;
}






skill_t* CTools::GetMySkillTpointer(playerCharacter_t* pPlayer,const int Idx)
{
	if(Idx < 0) return NULL;
	if(Idx >= MAX_NUMBER_OF_SKILL) return NULL;
	return &(pPlayer->skill[Idx]);

}







playerCharacter_t* CTools::GetPlayerRecordPointer(const char* pName)
{
	playerCharacter_t* pTempPlayer=NULL;
	for(int idx=0; idx < MAX_PCS; idx++){
		pTempPlayer = GetPlayerRecordPointer(idx);
		if(NULL == pTempPlayer) continue;
		if(0 == strcmp(pTempPlayer->name,pName)) return pTempPlayer;
	}
	return NULL;
}





itemTable_t* CTools::GetItemTablePointer(const int in_nItemTableIdx)
{
	if(in_nItemTableIdx < 0) return NULL;
	if(in_nItemTableIdx >= g_itemTableNumber) return NULL;
	return &g_itemTable[in_nItemTableIdx];
}




item_t* CTools::GetMyItem(playerCharacter_t* in_pPlayer,const int in_nMyItemArrayIdx)
{
	if(in_nMyItemArrayIdx < 0) return NULL;
	if(in_nMyItemArrayIdx >= MAX_NUMBER_OF_OWNITEM) return NULL;

	itemTable_t* pItemTable=GetItemTablePointer((in_pPlayer->item[in_nMyItemArrayIdx]).itemTableIdx);
	if(NULL == pItemTable) return NULL;

	return &(in_pPlayer->item[in_nMyItemArrayIdx]);
}




BOOL CTools::IsStackItem(const item_t* in_pItem)
{
	itemTable_t* pItemTable= GetItemTablePointer(in_pItem->itemTableIdx);
	if(NULL == pItemTable) return FALSE;
	if(pItemTable->stackFlag) return TRUE;
	return FALSE;	
}


































BOOL CTools::DeleteItemFromMyInventory(playerCharacter_t* pPlayer,const int iInventoryIdx,const int ItemTableIdx, int DeleteNum)
{
	if( iInventoryIdx < 0 ) return FALSE;
	if( iInventoryIdx >= MAX_INVENTORY_SIZE) return FALSE;	
							
	int itemIdx = pPlayer->inventory[iInventoryIdx];	
	if( itemIdx < 0 ) return FALSE;	
							
	item_t* pItem = GetMyItem(pPlayer,itemIdx);
	if(NULL == pItem) return FALSE;	
	if(pItem->itemTableIdx != ItemTableIdx) return FALSE;

	











	

	if ( IsStackItem(pItem) == FALSE)
	{
		GTH_DeleteMyItem(pPlayer, pItem );
		pPlayer->inventory[iInventoryIdx] = -1;			
		g_DBGameServer->RemoveItem(pPlayer, itemIdx);	
	}
	else
	{			
	if (pItem->durability - DeleteNum >= 0 )
	{
		pItem->durability -= DeleteNum;
		g_DBGameServer->SaveItem(pPlayer, pItem, itemIdx );															
	}
	else
	{		
		GTH_DeleteMyItem(pPlayer, pItem );
		pPlayer->inventory[iInventoryIdx] = -1;			
		g_DBGameServer->RemoveItem(pPlayer, itemIdx);		
	}
	}
	

	return TRUE;
}







inline BOOL CTools::AddItem_DropMap(playerCharacter_t* pPlayer,const int ItemArrayIdx)	
{
	item_t* pItem = GetMyItem(pPlayer,ItemArrayIdx);
	if(NULL == pItem) return FALSE;	

	int itemObjectIdx = 
		GTH_SpawnItemObject( pItem, pPlayer->worldIdx, pPlayer->position, LOOTTYPE_INDIVIDUAL, pPlayer->idx );
				
	
	if ( itemObjectIdx < 0 )				
	{		
		ShowLogInfo("이곳에서 유저로그를 남겨주세요 인모씨~~~~~~~~ ");		
	}


	GTH_DeleteMyItem(pPlayer,pItem);
	
	
	return TRUE;
}



inline BOOL CTools::AddItem_Inventory(playerCharacter_t* pPlayer,const int ItemArrayIdx)	
{	
	item_t* pItem = GetMyItem(pPlayer,ItemArrayIdx);
	if(NULL == pItem) return FALSE;	


	int MatchItemArrayIdx=GetMatchMyItemIdx_fromINVENTORY(pPlayer,pItem->itemTableIdx);
	if((MatchItemArrayIdx > 0) && (TRUE == GTH_IsStackItem(pItem))){

		
		item_t* pDestItem = GetMyItem(pPlayer,MatchItemArrayIdx);
		if(NULL == pDestItem) return FALSE;

		
		
		pDestItem->durability += pItem->durability+1;			
		if( pDestItem->durability >= MAX_COUNT_STACK_ITEM ) 
			pDestItem->durability = MAX_COUNT_STACK_ITEM - 1;

		g_DBGameServer->SaveItem(pPlayer, pDestItem, MatchItemArrayIdx );		
		GTH_DeleteMyItem( pPlayer, pItem );
		
		
		return TRUE;
	}	

	
	int inventoryIdx = GTH_FindEmptyInventoryPos(pPlayer);					
	
	if( inventoryIdx < 0) return FALSE;
		
	
	pPlayer->inventory[inventoryIdx] = ItemArrayIdx;	

	
	g_DBGameServer->AddItem(pPlayer,pItem,ItemArrayIdx,"Server");	
	return TRUE;	
}



























int CTools::AddItem(playerCharacter_t* pPlayer,item_t& item, BOOL bDropItem)
{
	int ItemArrayIdx = GTH_AddMyItem(pPlayer,&item);
	if(ItemArrayIdx < 0)	return -1;
	if(ItemArrayIdx >= MAX_NUMBER_OF_OWNITEM) return -1;


	
	

	if(TRUE == AddItem_Inventory(pPlayer,ItemArrayIdx)) return 1;

	
	if ( bDropItem == TRUE)
		if(TRUE == AddItem_DropMap(pPlayer,ItemArrayIdx)) return 2;

	return -1;
}
BOOL CTools::AddItemBatting(playerCharacter_t* pPlayer,item_t& item)
{
	int ItemArrayIdx = GTH_AddMyItem(pPlayer,&item);
	if(ItemArrayIdx < 0)	return FALSE;
	if(ItemArrayIdx >= MAX_NUMBER_OF_OWNITEM) return FALSE;

	if(TRUE == AddItem_Inventory(pPlayer,ItemArrayIdx)) return TRUE;

	return FALSE;
}




int CTools::GetMatchMyItemIdx_fromINVENTORY(playerCharacter_t* pPlayer,const int paramItemTableIdx)
{
	int inventory_idx=-1;
	int ItemArryIdx=-1;
	
	
	for(inventory_idx = 0; inventory_idx < MAX_INVENTORY_SIZE; inventory_idx++)
	{
		ItemArryIdx = pPlayer->inventory[inventory_idx];		
		if(ItemArryIdx < 0) continue;		
		if(ItemArryIdx >= MAX_NUMBER_OF_OWNITEM){
			pPlayer->inventory[inventory_idx]=-1;
			continue;
		}

		item_t* pTempItem= &pPlayer->item[ItemArryIdx];
		if((pTempItem->itemTableIdx < 0) ||
		   (pTempItem->itemTableIdx >= g_itemTableNumber)){

			GTH_DeleteMyItem( pPlayer, pTempItem );
			g_DBGameServer->RemoveItem(pPlayer,ItemArryIdx);
			pPlayer->inventory[inventory_idx]=-1;
			continue;
		}

		if(paramItemTableIdx == pTempItem->itemTableIdx) return ItemArryIdx;		
	}

	return -1;
}




monsterCharacter_t* CTools::GetMonsterRecordPointer(const int Idx)
{
	if(Idx < 0) return NULL;
	if(Idx >= MAX_MONSTERS) return NULL;

	
	monsterCharacter_t* pMonster=&g_monster[Idx];
	if(!pMonster->active) return NULL;
	if(pMonster->entityType != ENTITY_MONSTER) return NULL;

	
	return pMonster;
}







monsterCharacter_t* CTools::GetNPCRecordPointer(const int Idx)
{
	if(Idx < 0) return NULL;
	if(Idx >= MAX_MONSTERS) return NULL;

	
	monsterCharacter_t* pNPC=&g_monster[Idx];
	if(!pNPC->active) return NULL;
	if(pNPC->entityType != ENTITY_NPC) return NULL;
	
	
	return pNPC;
}




int	 CTools::GetEpkChargeCount(item_t *pItem)							
{
	return pItem->optionValue[item_t::ITEM_T_CHARGE_VALUE];
}



void  CTools::SetEpkChargeCount(item_t *pItem,int count)				
{
	pItem->optionIdx[item_t::ITEM_T_CHARGE_VALUE]   = -1;
	pItem->optionValue[item_t::ITEM_T_CHARGE_VALUE] = count;
}



#include "../GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;
BOOL CTools::isSummonAllow(playerCharacter_t* pPlayer)
{
	if(TRUE == pPlayer->GonryunBattle.Observer.bActive){
		if(NULL != gcpGolryunBattle){

			(gcpGolryunBattle->GetDecoderClassPointer())->RecvObserveEnd(pPlayer);

			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}





bool CTools::IsTransformEntity( void *ent, int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		for( int i = 0; i < MAX_AFFECT; i ++ )
		{
			if( pc->affect[i].type == AFFECT_SPECIAL && pc->affect[i].spe.specialType == 0 )
			{
				return true;
			}
		}
	}
	else 
	{
		m = static_cast<monsterCharacter_t*>(ent);
		for( int i = 0; i < MAX_AFFECT; i ++ )
		{
			if( m->affect[i].type == AFFECT_SPECIAL && m->affect[i].spe.specialType == 0 )
			{
				return true;
			}
		}
	}

	return false;
}




BOOL CTools::isReady(const playerCharacter_t* in_pPlayer)
{
	if(FALSE == in_pPlayer->ready) return FALSE;
	if(WorldChangeSystem::WORLD_CHANGESYSTEM_NONE != in_pPlayer->m_WorldChangeSystem.m_SystemMode) return FALSE;
	return TRUE;
}











int CTools::GetMatchMyInventoryIdx_fromItemTable(playerCharacter_t* pPlayer,const int paramItemTableIdx)
{
	int inventory_idx=-1;
	int ItemArryIdx=-1;
	
	
	for(inventory_idx = 0; inventory_idx < MAX_INVENTORY_SIZE; inventory_idx++)
	{
		ItemArryIdx = pPlayer->inventory[inventory_idx];		
		if(ItemArryIdx < 0) continue;		
		if(ItemArryIdx >= MAX_NUMBER_OF_OWNITEM){
			pPlayer->inventory[inventory_idx]=-1;
			continue;
		}
		
		item_t* pTempItem= &pPlayer->item[ItemArryIdx];
		if((pTempItem->itemTableIdx < 0) ||
			(pTempItem->itemTableIdx >= g_itemTableNumber)){
			
			GTH_DeleteMyItem( pPlayer, pTempItem );
			g_DBGameServer->RemoveItem(pPlayer,ItemArryIdx);
			pPlayer->inventory[inventory_idx]=-1;
			continue;
		}
		
		if(paramItemTableIdx == pTempItem->itemTableIdx) return inventory_idx;		
	}
	
	return -1;
}

item_t* CTools::GetMyInventoryItem(playerCharacter_t* pPlayer,const int InventoryPos)
{
	if(InventoryPos < 0 || InventoryPos >= MAX_INVENTORY_SIZE) return NULL;
	int ItemIdx = pPlayer->inventory[InventoryPos];

	if(ItemIdx < 0) return NULL;
	if(ItemIdx >= MAX_NUMBER_OF_OWNITEM) return NULL;
	return &pPlayer->item[ItemIdx];
}


BOOL CTools::DeleteItemFromMyInventory(playerCharacter_t* pPlayer,const int iInventoryIdx)
{
	if( iInventoryIdx < 0 ) return FALSE;
	if( iInventoryIdx >= MAX_INVENTORY_SIZE) return FALSE;	
							
	int itemIdx = pPlayer->inventory[iInventoryIdx];	
	if( itemIdx < 0 ) return FALSE;	
							
	item_t* pItem = GetMyItem(pPlayer,itemIdx);
	if(NULL == pItem) return FALSE;	

	if (pItem->durability - 1 >= 0 )
	{
		pItem->durability --;
		g_DBGameServer->SaveItem(pPlayer, pItem, itemIdx );															
	}
	else
	{		
		GTH_DeleteMyItem(pPlayer, pItem );
		pPlayer->inventory[iInventoryIdx] = -1;			
		g_DBGameServer->RemoveItem(pPlayer, itemIdx);		
	}

	return TRUE;
}
