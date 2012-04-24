

#include "../global_def.h"


void GambleSystemDecode()
{
	SHORT Packet = static_cast<enum enumGambleSystem>(MSG_ReadShort());
	
	switch(Packet) 
	{
		
	case BUY_GAMBLE_ITEM_REPLY:
		GTH_ReplytMessage_BuyGambleItem();
		break;
		
		
	}
}


	
	
void GTH_ReplytMessage_BuyGambleItem()
{
	int reply			= 0;
	int error			= 0;
	int inventoryPos	= 0;
	int itemIdx			= 0;
	int storeItemIdx	= 0;
	int curNumber		= 0;
	
	item_t item;		
	reply = MSG_ReadByte();	

	
	g_ifMng->m_storeWin->m_LockGambleStore = FALSE;

	if( reply > 0 )
	{		
		storeItemIdx	= MSG_ReadShort();
		curNumber		= MSG_ReadShort();
		
		MSG_ReadItem( &item );		
		int stackItemIdx = GTH_StackMyItem( &item );		
		int itemidx = g_ifMng->m_storeWin->m_curPage *
			CGambleSystem::MAX_NUMBER_OF_STORE_DEFAULT_ITEM 
			+storeItemIdx;
		
		for (int PageIdx = 0; PageIdx < g_ifMng->m_storeWin->m_curPage ;PageIdx++)
			itemidx -= g_ifMng->m_storeWin->m_PageItemCount[PageIdx];
		
		if ( itemidx >=0 || itemidx < MAX_NUMBER_OF_STORE_ITEM)
		{		
			
			g_ifMng->m_storeWin->m_storeItem[itemidx].item.itemTableIdx = -1;
			
			int FindItemIdx = GTH_GetInvenPosMatchMyItemTableIdx(CGambleSystem::LUCKY_STONE_ITEM_TABLE_IDX);			
				
			
			if ( FindItemIdx != -1 )
			{
				int price = g_ifMng->m_storeWin->m_storeItem[itemidx].price;
				DeleteItemFromMyInventoryByInvenPos(FindItemIdx, price);		
			}
		}

		
		
		if( stackItemIdx >= 0 )
		{							
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item.durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
			
			
		}
		else
		{									
			itemIdx = GTH_AddMyItem( &item );
			inventoryPos = GTH_FindEmptyInventoryPos();						

			if ( inventoryPos != -1)
				g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;

		}	
		g_ifMng->m_storeWin->m_storeItem[storeItemIdx].curNumber = curNumber;
		g_cgv.myCharacterInfo->curItemWeight = MSG_ReadShort();
		
	}
	
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_BUYITEM_1 :			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,36), 1, IF_MESSAGE_NONE );
			
			break;
		case ERROR_BUYITEM_2 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,48), 1, IF_MESSAGE_NONE );
			
			break;
		case ERROR_BUYITEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,49), 1, IF_MESSAGE_NONE );
			break;

		case ERROR_BUYITEM_5:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_BUYITEM_6:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 530 ), 1, IF_MESSAGE_NONE );
			break;			
		case ERROR_BUYITEM_7:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 531 ), 1, IF_MESSAGE_NONE );
			break;
		}
	}	
}


	





void GTH_SendMessageRequest_BuyGambleItem(int SlotIdx, int Num)
{
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	 EXTEND_SECOND);
		MSG_WriteShort(&netMessage,  GAMBLE_SYSTEM );		
		MSG_WriteShort(&netMessage,  BUY_GAMBLE_ITEM_REQUEST );
		MSG_WriteLong(&netMessage,   SlotIdx);		
		MSG_WriteLong(&netMessage,	 Num);	
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
	
}







void GTH_SendMessage_GambleSystem_Active(BOOL bActive)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	 EXTEND_SECOND);
		MSG_WriteShort(&netMessage,  GAMBLE_SYSTEM );		
		MSG_WriteShort(&netMessage,  ACTIVE_SYSTEM_REQUEST );
		MSG_WriteLong(&netMessage,   bActive);
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
}