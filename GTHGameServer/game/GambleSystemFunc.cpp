




#include "../global.h"








void GambleSystemDecode()
{
	SHORT Packet = static_cast<enum enumGambleSystem>(MSG_ReadShort());
	
	switch(Packet)
	{
		
	case ACTIVE_SYSTEM_REQUEST:
		GambleSystem_ActiveSystem_ProcessMessage();
		break;
		
		
	case BUY_GAMBLE_ITEM_REQUEST:
		BuyGambleItem_ProcessMessage();
		break;
		
	}
}







void GambleSystem_ActiveSystem_ProcessMessage()
{	
	g_GambleSystem.SetActiveSystem(MSG_ReadByte());	
}

#include "../CTools.h"
extern CTools* gcpTools;






void BuyGambleItem_ProcessMessage()
{
	if ( g_curPC == NULL)
		return;
	
	storeStruct_t *pStore	= NULL;
	int StoreIdx			= 0;
	int StoreItemIdx		= 0;
	int ItemNumber			= 0;	
	int NpcIdx				= 0;

	
	StoreItemIdx			= MSG_ReadLong();
	
	
	ItemNumber				= MSG_ReadLong();	

	StoreIdx				= g_curPC->storeIdx;

	
	if (StoreIdx< 0 || StoreIdx>= MAX_STORES)
	{
		BuyGambleItem_ReplyMessage( g_curPC, 0, ERROR_BUYITEM_7, NULL, 0, 0 );
		return;
	}

	
	if ( StoreItemIdx < 0 || StoreItemIdx >= MAX_NUMBER_OF_STORE_ITEM)
	{
		BuyGambleItem_ReplyMessage( g_curPC, 0, ERROR_BUYITEM_2, NULL, 0, 0 );
		return;
	}

	
	if( g_curPC->curItemWeight > g_curPC->calItemWeight )
	{
		BuyGambleItem_ReplyMessage( g_curPC, 0, ERROR_BUYITEM_5, NULL, 0, 0 );
		return;
	}

	
	int Selectidx		= g_curPC->RegGambleItems[StoreItemIdx];

	
	if ( (Selectidx < 0) || 
		(Selectidx >= MAX_NUMBER_OF_ITEM_TABLE) )
	{
		BuyGambleItem_ReplyMessage( g_curPC, 0, ERROR_BUYITEM_2, NULL, 0, 0 );
		return;
	}		
	

	
	NpcIdx				= g_curPC->npcIdx;	
	pStore				= &g_store[StoreIdx];	
	int ByeCost			= 0;

	
	
	ByeCost				= g_curPC->RegGamblePrice[StoreItemIdx] * ItemNumber;		
	

	if( GTH_IsHaveItem(g_curPC, CGambleSystem::LUCKY_STONE_ITEM_TABLE_IDX, ByeCost) == FALSE )
	{		
		BuyGambleItem_ReplyMessage( g_curPC, 0, ERROR_BUYITEM_6, NULL, 0, 0 );
		return;
	}
	
	
	
	

	float SelectRate	= 0.0f;
	float accPro		= 0.0f;
	SelectRate			= GTH_Random();			
	int	  OptionCount	= 0;


	for( int OptionTableIdx = 0; OptionTableIdx< MAX_GAMBLE_SELECTOPTION ; OptionTableIdx ++ )
	{				



		accPro += (float) g_logic.gambleSelectOptionTable[OptionTableIdx].SelectOptionRate * 0.01;
		if( SelectRate < accPro ) break;
		OptionCount++;
	}


	
	if ( OptionCount > MAX_NUMBER_OF_ITEM_OPTION ) 
		OptionCount = MAX_NUMBER_OF_ITEM_OPTION;
	
	
	

	
	item_t buyItem;	
	SpawnItem(&buyItem, Selectidx, OptionCount, 1, 0, 1 );
	int itemTableIdx = buyItem.itemTableIdx;


	
	if( itemTableIdx >= g_itemTableNumber || itemTableIdx < 0)
	{
		g_logSystem->Write( "Shop Error!! Index Out of Range : %s(%s), %s(%d)npc , BuyItem %s(%d), Table Index : %d", g_curPC->name, g_curPC->userID,
			g_monster[NpcIdx].basic.name, NpcIdx, buyItem.name, StoreItemIdx, itemTableIdx );		
		BuyGambleItem_ReplyMessage( g_curPC, 0, ERROR_BUYITEM_2, NULL, 0, 0 );
		return;
	}		


	
	if ( gcpTools->AddItem(g_curPC, buyItem, FALSE) == -1 )
	{		
		BuyGambleItem_ReplyMessage( g_curPC, 0, ERROR_BUYITEM_3, NULL, 0, 0 );
		return;	
	}
	
	
	g_curPC->curItemWeight += g_itemTable[buyItem.itemTableIdx].weight;
	g_logSystem->Write("User [%s](%s) Buy '%s' Item, Pay : %dSE.", g_curPC->name, g_curPC->userID, g_itemTable[itemTableIdx].name, ByeCost );
	
	
	
	if( pStore->item[StoreItemIdx].curNumber > 0 ) 
		pStore->item[StoreItemIdx].curNumber --;

	
	int GoldItemIdxOfInven = gcpTools->GetMatchMyInventoryIdx_fromItemTable(g_curPC, CGambleSystem::LUCKY_STONE_ITEM_TABLE_IDX);	
	gcpTools->DeleteItemFromMyInventory(g_curPC, GoldItemIdxOfInven,  CGambleSystem::LUCKY_STONE_ITEM_TABLE_IDX, ByeCost);


	BuyGambleItem_ReplyMessage( g_curPC, 1, 0, &buyItem, StoreItemIdx, pStore->item[StoreItemIdx].curNumber );
}






void BuyGambleItem_ReplyMessage(playerCharacter_t *pPc, int reply, int error, item_t *item, 
								int storeItemIdx, int curNumber)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GAMBLE_SYSTEM);
		MSG_WriteShort(&netMessage,	BUY_GAMBLE_ITEM_REPLY);
		MSG_WriteByte(&netMessage,	reply);
		
		if (reply > 0)
		{
			MSG_WriteShort(&netMessage, storeItemIdx );
			MSG_WriteShort(&netMessage, curNumber);

			
			MSG_WriteItem(&netMessage,	item);			
			MSG_WriteShort(&netMessage, pPc->curItemWeight);
		}
		else 
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
	

}