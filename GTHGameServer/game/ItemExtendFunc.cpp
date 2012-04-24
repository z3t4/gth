
#include "../Global.h"


void	GTH_ItemExtendSystemDecode()
{
	SHORT Packet = static_cast<enum enumGambleSystem>(MSG_ReadShort());	
	switch(Packet)
	{		
		
	case REQUEST_ITEM_SYNC_TIME:		
		GTH_ProcessMessageRequestItemSyncTIme();
		break;
		
		
	case REQUEST_ITEM_TIME_SET:
		GTH_ProcessMessageRequestItemTimeSet();
		break;		
	}

}







void GTH_ProcessMessageRequestItemSyncTIme()
{
	GTH_CollectItemItem(g_curPC);
	GTH_SendMessage_ItemSyncTime(g_curPC);

	
	g_curPC->m_ItemExtendInfo.fItemInfoRequestTime = 
		g_timer->GetAppMilliTime() + sItemUseTimeInfo::enumItemSyncInfo::ITEM_USETIME_SYNC_TERM;
}






void	GTH_ItemUseTimeProcess(playerCharacter_t* pPc)
{
	if ( pPc == NULL )
		return;

	for (int iItemIdx = 0; iItemIdx < MAX_NUMBER_OF_OWNITEM; iItemIdx++)
	{
		item_t *pItem = &pPc->item[iItemIdx];
		int *pItemUseTime = NULL;

		if ( IsItemUseFlag( pItem, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_TIME) )
			pItemUseTime = &pItem->ItemExtendInfo.AllowUseTime;

		if ( IsItemUseFlag( pItem, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_DATE) )
		{
			time_t NowTime = time(NULL);
			pItem->ItemExtendInfo.CalclateUseDateToSec = 
				difftime((time_t)pItem->ItemExtendInfo.AllowUseDate, NowTime);

			pItemUseTime = &pItem->ItemExtendInfo.AllowUseDate;
		}

		if ( NULL == pItemUseTime)
			continue;

		
		if ( (*pItemUseTime) < 0)
			continue;
		
		if ( IsItemUseFlag( pItem, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_TIME) )
			(*pItemUseTime) --;
	}
}







void	GTH_SendMessage_ItemSyncTime(playerCharacter_t* pPc)
{			
		
	int MaxSize = pPc->m_ItemExtendInfo.v_UseTimeItemInfo.size();
	if ( MaxSize <= 0 )
		return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, ITEM_EXTEND_SYSTEM);
		MSG_WriteShort(&netMessage,	REPLY_ITEM_SYNC_TIME);
		MSG_WriteLong(&netMessage,	MaxSize);

		for (int nSendIdx =0; nSendIdx < MaxSize; nSendIdx ++)
		{
			int ItemIdx = pPc->m_ItemExtendInfo.v_UseTimeItemInfo[nSendIdx];

			if ( ItemIdx >= 0 || ItemIdx < MAX_NUMBER_OF_OWNITEM )
			{
				MSG_WriteLong(&netMessage, ItemIdx);
				MSG_WriteItem(&netMessage,	&pPc->item[ItemIdx]);
			}
		}

		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}






void	GTH_SendMessage_ItemTimeSet(playerCharacter_t* pPc, int Reply, int iItemIdx , int ErrorIdx ,
									item_t *pReplaceItem , enum sItemUseTimeInfo::enumInventoryType type , int InvenIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, ITEM_EXTEND_SYSTEM);
		MSG_WriteShort(&netMessage,	REPLY_ITEM_TIME_SET);
		MSG_WriteLong(&netMessage,	Reply);		

		if ( Reply == TRUE)
		{
			MSG_WriteItem(&netMessage,  pReplaceItem);
			MSG_WriteLong(&netMessage,	(int)type);		
			MSG_WriteLong(&netMessage,	iItemIdx);		
			MSG_WriteLong(&netMessage,	InvenIdx);
		}
		else 
			MSG_WriteLong(&netMessage,	ErrorIdx);
		
		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}


#include "../CTools.h"
extern CTools* gcpTools;





void GTH_ProcessMessageRequestItemTimeSet()
{
	int iItemIdx = -1;
	int iInvenIdx = -1;

	enum sItemUseTimeInfo::enumInventoryType invenType;

	invenType	= static_cast<enum sItemUseTimeInfo::enumInventoryType>(MSG_ReadLong());	
	iItemIdx	= MSG_ReadLong();
	iInvenIdx	= MSG_ReadLong();

	
	if ( iItemIdx < 0 || iItemIdx >= MAX_NUMBER_OF_OWNITEM)
	{
		GTH_SendMessage_ItemTimeSet(g_curPC, 0, 0, sItemUseTimeInfo::enumItemExptendError::ITEM_EXTEND_ERROR1);
		return;
	}
	
	
	
	
	
	if ( IsInvalideItem_FromInvenType(g_curPC, invenType, iInvenIdx, iItemIdx) == FALSE )
	{
		GTH_SendMessage_ItemTimeSet(g_curPC, 0, 0, sItemUseTimeInfo::enumItemExptendError::ITEM_EXTEND_ERROR2);
		return;
	}

	
	item_t Replaceitem;
	InitItem( &Replaceitem );

	
	
	
	if ( FALSE == SpawnItem(&Replaceitem, sItemUseTimeInfo::enumItemSyncInfo::ITEM_TIMESET_REPLACE_ITEMTABLEIDX))
	
	{
		GTH_SendMessage_ItemTimeSet(g_curPC, 0, 0, sItemUseTimeInfo::enumItemExptendError::ITEM_EXTEND_ERROR3);
		return;
	}

	
	GTH_DeleteMyItem(g_curPC, &g_curPC->item[iItemIdx] );
	g_DBGameServer->RemoveItem(g_curPC, iItemIdx);



	
	if ( invenType == sItemUseTimeInfo::enumInventoryType::ITEM_EQUIP)		
	{
		GTH_ReplaceItem(g_curPC, invenType, iInvenIdx, -1);	
		
		int Result = gcpTools->AddItem( g_curPC, Replaceitem );
		
		switch(Result) 
		{
			
		case -1:
			GTH_SendMessage_ItemTimeSet( g_curPC, 0, 0, sItemUseTimeInfo::enumItemExptendError::ITEM_EXTEND_ERROR3 );
			return;
			break;				
		case 1:
		case 2:	
			
			GTH_SendMessage_ItemTimeSet(g_curPC, 1, iItemIdx, -1, &Replaceitem, invenType, iInvenIdx );
			return;
			break;	
		}		
	}
	
	
	
	int RegItemIdx = GTH_AddMyItem(g_curPC, &Replaceitem);
	
	
	if ( RegItemIdx < 0)
	{
		
		GTH_SendMessage_ItemTimeSet(g_curPC, 0, 0, sItemUseTimeInfo::enumItemExptendError::ITEM_EXTEND_ERROR3);
		
		return;
	}	

	g_DBGameServer->AddItem(g_curPC, &Replaceitem, RegItemIdx, "REPLACEMENT");
	GTH_ReplaceItem(g_curPC, invenType, iInvenIdx, RegItemIdx);	
	
	
	GTH_SendMessage_ItemTimeSet(g_curPC, 1, iItemIdx, -1, &Replaceitem, invenType, iInvenIdx );
	return;
}








BOOL	IsInvalideItem_FromInvenType(playerCharacter_t* pPc, int InvenType, int iInvenPos, int ItemIdx)
{	
	switch(InvenType)
	{
		
	case sItemUseTimeInfo::enumInventoryType::ITEM_INVEN	:			
		{				
			if ( iInvenPos < 0 || iInvenPos >= MAX_INVENTORY_SIZE)
				return FALSE;

			if ( pPc->inventory[iInvenPos] < 0 || 
				pPc->inventory[iInvenPos] >= MAX_NUMBER_OF_OWNITEM )
				return FALSE;

			if ( pPc->inventory[iInvenPos] != ItemIdx)
				return FALSE;
			
			
		}break;
		
	case sItemUseTimeInfo::enumInventoryType::ITEM_UPGRADE:
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_UPGRADE_SOURCE_ITEM_SIZE)
				return FALSE;

			if ( pPc->upgradeSourceInventory[iInvenPos] < 0 || 
				pPc->upgradeSourceInventory[iInvenPos] >= MAX_NUMBER_OF_OWNITEM )			
				return FALSE;

			if ( pPc->upgradeSourceInventory[iInvenPos] != ItemIdx)
				return FALSE;
		}break;

		
	case sItemUseTimeInfo::enumInventoryType::ITEM_EQUIP:
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_EQUIPMENT)
				return FALSE;

			if ( pPc->equipment[iInvenPos] < 0 || 
				pPc->equipment[iInvenPos] >= MAX_NUMBER_OF_OWNITEM )			
				return FALSE;

			if ( pPc->equipment[iInvenPos] != ItemIdx)
				return FALSE;
		}
		break;
		
	case sItemUseTimeInfo::enumInventoryType::ITEM_GB_BATTING:
		{
			if ( iInvenPos < 0 || iInvenPos >= playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE)
				return FALSE;
			
			if ( pPc->GonryunBattlePractice.Inventory[iInvenPos] < 0 || 
				pPc->GonryunBattlePractice.Inventory[iInvenPos] >= MAX_NUMBER_OF_OWNITEM )			
				return FALSE;

			if ( pPc->GonryunBattlePractice.Inventory[iInvenPos] != ItemIdx)			
				return FALSE;

		}break;
		
	case sItemUseTimeInfo::enumInventoryType::ITEM_PRECOCITY:			
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_PRECOCITY_SIZE)
				return FALSE;
			
			if ( pPc->precocityInventory[iInvenPos] < 0 || 
				pPc->precocityInventory[iInvenPos] >= MAX_NUMBER_OF_OWNITEM )			
				return FALSE;

			if ( pPc->precocityInventory[iInvenPos] != ItemIdx)			
				return FALSE;

		}break;
		
	case sItemUseTimeInfo::enumInventoryType::ITEM_CRAFT:
		{
			
			if ( iInvenPos < 0 || iInvenPos >= MAX_CRAFT_SOURCE_ITEM_SIZE)
				return FALSE;
			
			if ( pPc->craftSourceInventory[iInvenPos] < 0 || 
				pPc->craftSourceInventory[iInvenPos] >= MAX_NUMBER_OF_OWNITEM )			
				return FALSE;

			if ( pPc->craftSourceInventory[iInvenPos] != ItemIdx)			
				return FALSE;
		}break;
		
	case sItemUseTimeInfo::enumInventoryType::ITEM_DEPOT:
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_DEPOT_SIZE)
				return FALSE;
			
			if ( pPc->depot[iInvenPos] < 0 || 
				pPc->depot[iInvenPos] >= MAX_NUMBER_OF_OWNITEM )			
				return FALSE;

			if ( pPc->depot[iInvenPos] != ItemIdx)			
				return FALSE;


		}break;
		
	case sItemUseTimeInfo::enumInventoryType::ITEM_BOOTHINVENTORY:	
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_BOOTH_INVENTORY_SIZE)
				return FALSE;

			if ( pPc->boothInventory[iInvenPos].itemIdx < 0 || 
				pPc->boothInventory[iInvenPos].itemIdx >= MAX_NUMBER_OF_OWNITEM)
				return FALSE;

			if ( pPc->boothInventory[iInvenPos].itemIdx != ItemIdx)			
				return FALSE;
		}break;
	case sItemUseTimeInfo::enumInventoryType::ITEM_MOUSEIN:
		if ( pPc->mouseInventory != ItemIdx)			
			return FALSE;
		break;

	}

	

	return TRUE;
}






void	GTH_ProcessItemSync(playerCharacter_t *pPc)
{	
	
	if ( pPc->m_ItemExtendInfo.fItemInfoRequestTime >= g_timer->GetAppMilliTime() )
		return;
	
	
	GTH_CollectItemItem(pPc);
	
	GTH_SendMessage_ItemSyncTime(pPc);

	
	pPc->m_ItemExtendInfo.fItemInfoRequestTime = 
		g_timer->GetAppMilliTime() + sItemUseTimeInfo::enumItemSyncInfo::ITEM_USETIME_SYNC_TERM;

}






void	GTH_CollectItemItem(playerCharacter_t *pPc)
{
	pPc->m_ItemExtendInfo.v_UseTimeItemInfo.clear();		
	
	for (int iItemIdx = 0; iItemIdx < MAX_NUMBER_OF_OWNITEM; iItemIdx++)
	{
		item_t *pItem = &pPc->item[iItemIdx];
		
		
		if ( IsItemUseFlag( pItem, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_TIME) ||
			IsItemUseFlag( pItem, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_DATE))
		{
			
			pPc->m_ItemExtendInfo.v_UseTimeItemInfo.push_back(iItemIdx);
		}
	}
}







void GTH_DBSaveItemForTimeType(playerCharacter_t *pPc)
{
	if ( NULL == pPc )
		return ;

	int MaxSize = pPc->m_ItemExtendInfo.v_UseTimeItemInfo.size();
	
	for (int nSaveIdx = 0 ;nSaveIdx < MaxSize ; nSaveIdx ++)
	{
		int ItemIdx = pPc->m_ItemExtendInfo.v_UseTimeItemInfo[nSaveIdx];		
		if ( ItemIdx >= 0 || ItemIdx < MAX_NUMBER_OF_OWNITEM)
		{
			item_t *pITem = &pPc->item[ItemIdx];
			g_DBGameServer->SaveItem(pPc, pITem,  ItemIdx);
		}		
	}
}







BOOL	GTH_ReplaceItem(playerCharacter_t *pPc, enum sItemUseTimeInfo::enumInventoryType type, int iInvenPos, int NewItemIdx)
{
	switch (type)
	{		
		case	sItemUseTimeInfo::enumInventoryType::ITEM_INVEN:
			{
				if ( iInvenPos < 0 || iInvenPos >= MAX_INVENTORY_SIZE)
					return FALSE;
				pPc->inventory[iInvenPos] = NewItemIdx;					
					
			}
			break;
		case	sItemUseTimeInfo::enumInventoryType::ITEM_UPGRADE:
			{
				if ( iInvenPos < 0 || iInvenPos >= MAX_UPGRADE_SOURCE_ITEM_SIZE)
					return FALSE;
				pPc->upgradeSourceInventory[iInvenPos] = NewItemIdx;
			}
			break;
		case	sItemUseTimeInfo::enumInventoryType::ITEM_EQUIP:	
			{
				if ( iInvenPos < 0 || iInvenPos >= MAX_EQUIPMENT)
					return FALSE;
				pPc->equipment[iInvenPos] = NewItemIdx;
			}
			break;
		case	sItemUseTimeInfo::enumInventoryType::ITEM_GB_BATTING:
			{
				if ( iInvenPos < 0 || iInvenPos >= 
					playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE)
					return FALSE;
				pPc->GonryunBattlePractice.Inventory[iInvenPos] = NewItemIdx;
			}
			break;
		case	sItemUseTimeInfo::enumInventoryType::ITEM_PRECOCITY:	
			{
				if ( iInvenPos < 0 || iInvenPos >= MAX_PRECOCITY_SIZE)
					return FALSE;
				pPc->precocityInventory[iInvenPos] = NewItemIdx;
			}
			break;
		case	sItemUseTimeInfo::enumInventoryType::ITEM_CRAFT:		
			{
				if ( iInvenPos < 0 || iInvenPos >= MAX_CRAFT_SOURCE_ITEM_SIZE)
					return FALSE;
				pPc->craftSourceInventory[iInvenPos] = NewItemIdx;
			}
			break;
		case	sItemUseTimeInfo::enumInventoryType::ITEM_DEPOT:
			{
				if ( iInvenPos < 0 || iInvenPos >= MAX_DEPOT_SIZE)
					return FALSE;
				pPc->depot[iInvenPos] = NewItemIdx;
			}
			break;
		case	sItemUseTimeInfo::enumInventoryType::ITEM_BOOTHINVENTORY:
			{
				if ( iInvenPos < 0 || iInvenPos >= MAX_BOOTH_INVENTORY_SIZE)
					return FALSE;
				pPc->boothInventory[iInvenPos].itemIdx = NewItemIdx;
			}
			break;

		case sItemUseTimeInfo::enumInventoryType::ITEM_MOUSEIN:
			if ( pPc->mouseInventory = NewItemIdx)			
				return FALSE;
			break;
	}
	return TRUE;
}