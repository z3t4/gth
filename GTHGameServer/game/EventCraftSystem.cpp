



#include "../global.h"

CEventCraftSystem g_EventCraftSystem;





CEventCraftSystem::CEventCraftSystem()
{

}

CEventCraftSystem::~CEventCraftSystem()
{

}

int GTH_EventCraftProcess( playerCharacter_t *pc, int &reply, int &error ,bool checkFlag)
{
	int i, craftIdx, itemIdx;

	int bIsStack, stackNumber;

	
	craftIdx = GTH_CheckEventCraftFormula( pc );
	if( craftIdx < 0 ) 
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_0;			
		return -1;
	}

	if( checkFlag ) return 1;

	
	for( i = 0; i < MAX_EVENT_CRAFT_ITEM; i ++ )
	{
		if( pc->craftSourceInventory[i] < 0 ) continue;
		itemIdx = pc->craftSourceInventory[i];

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pc->item[itemIdx].itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pc->item[itemIdx].durability + 1;
		}
		
		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_CRAFT_SOURCE, stackNumber, pc->item[itemIdx].name,
			GetItemInformationLog( bIsStack, pc->item[itemIdx].itemTableIdx, itemIdx, -1 ));
		

		GTH_DeleteMyItem( pc, &pc->item[itemIdx] );
		pc->craftSourceInventory[i] = -1;

		g_DBGameServer->RemoveItem( pc, itemIdx );

		reply = 1;
	}

	itemCraftTable_t	*pData = g_EventCraftSystem.Get(craftIdx);
	if(pData != NULL)
	{
		
		g_curPC->curChargeSE	+=	pData->m_lResultNak;

		
		g_curPC->exp			+= pData->m_lResultExp;
	}


	return craftIdx;
}

void GTH_ProcessMessage_ItemSystem_EventCraftPrepare()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;

	int reply = 1, error = 0;
	int nak = 0;
	float pro = 0.0f;

	GTH_EventCraftProcess( g_curPC, reply, error,true);
	GTH_SendMessage_ItemSystem_EventCraftPrepare( g_curPC, reply, error );
}

void GTH_ProcessMessage_ItemSystem_EventCraftComplete()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;

	int reply = 1, error = 0;

	int craftIdx;

	craftIdx = GTH_EventCraftProcess( g_curPC, reply, error);

	itemCraftTable_t * craftData = g_EventCraftSystem.Get(craftIdx);
	if(craftData != NULL)
	{
		if(craftData->m_btEventType == CEventCraftSystem::CRAFT_RESISTER)
		{
			
			g_DBGameServer->Send_Save_EventCraftItem(g_curPC,craftIdx);
		}
	}

	GTH_SendMessage_ItemSystem_EventCraftComplete( g_curPC, reply, error ,craftIdx);
}

void GTH_ProcessMessage_ItemSystem_EventCraftLoadEvent()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;

	int nEventIndex = MSG_ReadLong();
	g_DBGameServer->Recv_Load_EventCraftItem(g_curPC,nEventIndex);
}


int GTH_UndoPCEventCraft( playerCharacter_t *pc, int completeFlag )
{
	int itemIdx, inventoryPos;
	item_t *item, *upItem;

	int bIsStack = false, stackNumber = 1;

	for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		itemIdx = pc->craftSourceInventory[i];
		if( itemIdx < 0 ) continue;
		item = &pc->item[itemIdx];

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[item->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = item->durability + 1;
		}

		int stackItemIdx = GTH_StackMyItem( pc, item );
		if( stackItemIdx >= 0 )
		{
			upItem = &pc->item[stackItemIdx];
			upItem->durability += item->durability + 1;
			if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;
			
			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_CRAFT, 0, stackNumber, upItem->name,
				GetItemInformationLog( bIsStack, upItem->itemTableIdx, stackItemIdx, -1 ));
			

			GTH_DeleteMyItem( pc, item );

			g_DBGameServer->SaveItem( pc, upItem, stackItemIdx );
			g_DBGameServer->RemoveItem( pc, itemIdx );
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos( pc );
			if( inventoryPos >= 0 ) 
			{
				pc->inventory[inventoryPos] = itemIdx;
			}

			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_CRAFT, 0, stackNumber, item->name,
				GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, inventoryPos ));
			
		}
	}

	if( !completeFlag ) g_logSystem->Write("Make Item Cancel  : %s(%s)", pc->name, pc->userID );
	else g_logSystem->Write("Make Item Complete : %s(%s)", pc->name, pc->userID );
	
	
	for( i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		pc->craftSourceInventory[i] = -1;
	}

	
	

	return 1;
}

void GTH_ProcessMessage_ItemSystem_EventCraftClose()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;

	GTH_UndoPCEventCraft( g_curPC );
	GTH_DeleteNoticePC( g_curPC );

	GTH_SendMessage_ItemSystem_EventCraftClose( g_curPC );
}

void GTH_ProcessMessage_ItemSystem_EventCraftOpen()
{
	int i;
	
	
	for( i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		g_curPC->craftSourceInventory[i] = -1;
	}

	
	GTH_AddNoticePC( g_curPC, -1, -1, BUSY_STATE_CRAFT );
	GTH_SendMessage_ItemSystem_EventCraftOpen( g_curPC );
}

void GTH_ProcessMessage_ItemSystem_EventCraftItem()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;

	int posType = MSG_ReadShort();
	int pos = MSG_ReadShort();

	int tempInven;
	int reply = 1, error = 0;

	
	if( posType == 0 )
	{
		if( g_curPC->mouseInventory >= 0 )
		{
			if( g_curPC->craftSourceInventory[pos] >= 0 )
			{
				tempInven = g_curPC->mouseInventory;
				g_curPC->mouseInventory = g_curPC->craftSourceInventory[pos];
				g_curPC->craftSourceInventory[pos] = tempInven;
			}
			else
			{
				g_curPC->craftSourceInventory[pos] = g_curPC->mouseInventory;
				g_curPC->mouseInventory = -1;
			}
		}
		else
		{
			g_curPC->mouseInventory = g_curPC->craftSourceInventory[pos];
			g_curPC->craftSourceInventory[pos] = -1;
		}
	}
	else
	{
		if( g_curPC->mouseInventory < 0 && g_curPC->craftResultInventory[pos] >= 0 )
		{
			g_curPC->mouseInventory = g_curPC->craftResultInventory[pos];
			g_curPC->craftResultInventory[pos] = -1;
		}
	}

	GTH_SendMessage_ItemSystem_EventCraftItem( g_curPC, reply, error, posType, pos );
}

int GTH_CheckEventCraftFormula( playerCharacter_t *pc )
{
	int i, j, k, itemIdx;
	int itemTableIdx1, itemTableIdx2, checkedNumber;
	int numSourceItem, checkedItem[MAX_EVENT_CRAFT_ITEM], checkedIdx[MAX_EVENT_CRAFT_ITEM];
	item_t *sourceItem;
	
	
	numSourceItem = 0;
	for( i = 0; i < MAX_EVENT_CRAFT_ITEM; i ++ )
	{
		if( pc->craftSourceInventory[i] < 0 ) continue;
		
		sourceItem = &pc->item[pc->craftSourceInventory[i]];

		
		
		checkedIdx[numSourceItem] = i;
		numSourceItem ++;
	}
	if( numSourceItem == 0 ) return -1;

	for( i = 0; i < g_EventCraftSystem.GetTableSize(); i ++ )
	{
		itemCraftTable_t *pData = g_EventCraftSystem.Get(i);
		if(pData == NULL) break;

		
		if( numSourceItem != pData->numSourceItem ) continue;

		
		checkedNumber = numSourceItem;
		
		for( j = 0; j < MAX_EVENT_CRAFT_ITEM; j ++ )
		{
			checkedItem[j] = false;
		}
		
		for( j = 0; j < MAX_EVENT_CRAFT_ITEM; j ++ )
		{
			for( k = 0; k < numSourceItem; k ++ )
			{
				
				if( checkedItem[k] ) continue;

				itemIdx = pc->craftSourceInventory[checkedIdx[k]];
				sourceItem = &pc->item[itemIdx];
				
				itemTableIdx1 = sourceItem->itemTableIdx;
				itemTableIdx2 = pData->sourceItem[j].itemIndex;
				if( itemTableIdx1 != itemTableIdx2 ) continue;
				
				if( g_itemTable[itemTableIdx1].stackFlag && g_itemTable[itemTableIdx2].stackFlag )
				{
					if( pData->sourceItem[j].itemCount != sourceItem->durability + 1 ) continue;
				}
				

				break;
			}
			if( k == numSourceItem ) break;
			checkedItem[k] = true;
			checkedNumber --;
		}
		if( checkedNumber == 0 ) 
		{
		
			return i;
		}
	}

	return -1;
}

