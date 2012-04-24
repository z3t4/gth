#include "../global.h"
extern i3socket_t gsSocket;

#include "../Guild/CGuildCtrl.h"
extern CGuildCtrl* gcpGuildCtrl;
#include "../GonryunPracticeBattle/CGonryunPracticeBattle.h"
extern CGonryunPracticeBattle	g_GonryunPracticeBattle;


#include "..\Tools\CTools.h"
extern CTools* gcpTools;

extern void TransmitePacket_sc_disconnect(playerCharacter_t* pPlayer, int MessageIdx =0);
extern void GTH_DisconnectPlayer(playerCharacter_t* pPlayer,const BOOL bForceQuit);




int GTH_AddNoticePC( playerCharacter_t *pc, int entityType, int entityIdx, int busyState )
{
	if( entityType == -1 )
	{
		pc->busyState = busyState;
		return 1;
	}
	else if( entityType != ENTITY_PC )
	{
		pc->friendlyEntityType = entityType;
		pc->friendlyEntityIdx = entityIdx;
		pc->friendlyEntityWorldIdx = pc->worldIdx;
		pc->busyState = busyState;
		strncpy( pc->friendlyEntityName, g_monster[entityIdx].basic.name,NAMESTRING);
		pc->friendlyEntityName[NAMESTRING]=NULL;
	}
	else
	{
		pc->friendlyEntityType = entityType;
		pc->friendlyEntityIdx = entityIdx;
		pc->busyState = busyState;

		strncpy(pc->friendlyEntityName, g_pc[entityIdx].name,NAMESTRING);
		pc->friendlyEntityName[NAMESTRING]=NULL;
	}

	return 1;
}
 
int GTH_DeleteNoticePC( playerCharacter_t *pc )
{
	pc->friendlyEntityType = -1;
	pc->friendlyEntityIdx = -1;
	pc->friendlyEntityWorldIdx = -1;
	pc->friendlyEntityName[0] = 0;
	pc->busyState = BUSY_STATE_NONE;

	return 1;
}


int GTH_CheckNoticePC( playerCharacter_t *pc )
{


	if( pc->busyState == BUSY_STATE_NONE )
	{
		GTH_DeleteNoticePC( pc );
		return( false );
	}
	if( pc->busyState == BUSY_STATE_NPC )
	{
		return( true );
	}

	int idx = pc->friendlyEntityIdx;
	
	if( idx < 0 )
	{
		GTH_DeleteNoticePC( pc );
		return( false );
	}
	
	
	
	if( !g_pc[idx].active || !g_pc[idx].ready )
	{
		GTH_DeleteNoticePC( pc );
		return( false );
	}

	if( stricmp( g_pc[idx].name, pc->friendlyEntityName ) )
	{
		GTH_DeleteNoticePC( pc );
		return( false );
	}
	if( pc->worldIdx != g_pc[idx].worldIdx )
	{
		GTH_DeleteNoticePC( pc );
		return( false );
	}
	return( true );
}



int GTH_GetPCMenuType( playerCharacter_t* requestPC, playerCharacter_t* targetPC )
{
	return 0;

}



int GTH_GetPCMenuFlag( playerCharacter_t* requestPC, playerCharacter_t* targetPC )
{
	int menuFlag = PC_MENU_EXIT;
	menuFlag |= PC_MENU_TRADE;
	
	if( requestPC->memberNumber == 0 )
	{
		
		if( targetPC->memberNumber == 0 )
		{
			menuFlag |= PC_MENU_PARTY;
		}
	}
	
	else
	{
		
		if( requestPC->isLeader )
		{
			
			
			if( targetPC->memberNumber == 0 && requestPC->memberNumber < MAX_NUMBER_OF_PARTY_MEMBER ) 
				menuFlag |= PC_MENU_PARTY;
		}
	}

	return( menuFlag );
}



BOOL GTH_ForcedQuit( playerCharacter_t *pc )
{
	BOOL BusyState = FALSE;
	switch( pc->busyState )
	{
	
	case BUSY_STATE_REQUEST :
		GTH_DeleteNoticePC( pc );
		if( pc->friendlyEntityIdx >= 0 )
			GTH_DeleteNoticePC( &g_pc[pc->friendlyEntityIdx] );
		BusyState = TRUE;
		break;
	case BUSY_STATE_NPC :
		break;
	case BUSY_STATE_TRADE :
		
		GTH_ForcedQuitTrade( pc );
		BusyState = TRUE;
		break;
		
	case BUSY_STATE_CRAFT :
		GTH_ForcedQuitCraft( pc );
		BusyState = TRUE;
		break;
	case BUSY_STATE_UPGRADE :
		GTH_ForcedQuitUpgrade( pc );
		BusyState = TRUE;
		break;
	case BUSY_STATE_BOOTH_SELL : case BUSY_STATE_BOOTH_SELL_PREPARE :
		GTH_ForcedQuitBooth( pc );
		BusyState = TRUE;
		break;
	case BUSY_STATE_BOOTH_BUY :
		GTH_ForcedQuitBooth( pc );
		BusyState = TRUE;
		break;
	
	case BUSY_STATE_QUESTSCROLL :
		GTH_ForcedQuitQuestScroll( pc );			
		BusyState = TRUE;
		break;
	
	case BUSY_STATE_NPC_INVENTORY	:
		GTH_ForcedQuitAutoQuestScroll( pc );
		BusyState = TRUE;
		break;
	}

	return BusyState;
}





int GTH_UndoPCTrade( playerCharacter_t *pc, int flagFromConfirm )
{
	int itemIdx, inventoryPos;
	

	int bIsStack = false, stackNumber = 1;

	for( int i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		itemIdx = pc->myTrade[i];
		
		if ( itemIdx < 0 || itemIdx >= MAX_NUMBER_OF_OWNITEM)
			continue;
		
		item_t* pItem = NULL;
		pItem = gcpTools->GetMyItem(pc, itemIdx);
		if ( NULL == pItem )
		{			
			continue;
		}
		
		if ( pItem->itemTableIdx < 0 || pItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
			continue;

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pItem->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pItem->durability + 1;
		}

		int stackItemInvenIdx = GTH_StackMyItem( pc, pItem );

		
		if( stackItemInvenIdx >= 0 )
		{
			
			item_t * pInvenItem = NULL;
			pInvenItem = gcpTools->GetMyItem(pc, stackItemInvenIdx);

			if (NULL == pInvenItem)
				continue;
			
			
			if ( pInvenItem->itemTableIdx <0 || pInvenItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
				continue;

			pInvenItem->durability += pItem->durability + 1;
			if( pInvenItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) 
				pInvenItem->durability = MAX_COUNT_STACK_ITEM - 1;
			
			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_TRADE, stackNumber, pInvenItem->name,
				GetItemInformationLog( bIsStack, pInvenItem->itemTableIdx, stackItemInvenIdx, -1 ) );
			

			GTH_DeleteMyItem( pc, pItem );

			g_DBGameServer->SaveItem( pc, pInvenItem, stackItemInvenIdx );
			g_DBGameServer->RemoveItem( pc, itemIdx );
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos( pc );
			if( inventoryPos >= 0 )
			{
				pc->inventory[inventoryPos] = itemIdx;
			}

			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_TRADE, stackNumber, pItem->name,
				GetItemInformationLog( bIsStack, pItem->itemTableIdx, itemIdx, inventoryPos ) );
			
		}
	}

	switch( flagFromConfirm )
	{
	case 0 :
		g_logSystem->Write("Trade Cancel : %s(%s) To:(%s) (Direct Cancel)", pc->name, pc->userID, pc->friendlyEntityName );
		break;
	case 1 :
		g_logSystem->Write("Trade Cancel : %s(%s) To:(%s) (Error Cancel)", pc->name, pc->userID, pc->friendlyEntityName );
		break;
	case 2 :
		g_logSystem->Write("Trade Cancel : %s(%s) To:(%s) (Force Cancel)", pc->name, pc->userID, pc->friendlyEntityName );
		break;
	}

	

	return 1;	
}






















int GTH_CheckConfirmTrade( playerCharacter_t *pc, int& reply, int& error )
{
	int myEmptyInven, myItemInven, otherItemInven;
	int myItemWeight, otherItemWeight, i;
	int itemIdx, itemTableIdx;

	if( reply == 0 ) return( 0 );
	
	if( pc->curChargeSE - pc->tradeSE < 0 )
	{
		reply = 0;
		error = ERROR_PCTRADE_5;
		return( 0 );
	}
	
	
	
	if( pc->otherTradeSE + pc->curChargeSE - pc->tradeSE > MAX_OWN_NAK )
	{
		reply = 0;
		error = ERROR_PCTRADE_3;
		return( 0 );
	}

	myItemWeight = 0;
	myItemInven = 0;
	myEmptyInven = 0;
	otherItemInven = 0;
	otherItemWeight = 0;
	
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		if( pc->otherTrade[i] < 0 ) continue;
		itemIdx = pc->otherTrade[i];
		itemTableIdx = pc->tradeItem[itemIdx].itemTableIdx;
		myItemWeight += g_itemTable[itemTableIdx].weight;
		myItemInven ++;
	}
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		if( pc->myTrade[i] < 0 ) continue;
		itemIdx = pc->myTrade[i];
		itemTableIdx = pc->item[itemIdx].itemTableIdx;
		otherItemWeight += g_itemTable[itemTableIdx].weight;
		otherItemInven ++;
	}
	if( pc->curItemWeight + myItemWeight - otherItemWeight > pc->calItemWeight )
	{
		reply = 0;
		error = ERROR_PCTRADE_1;
		g_logSystem->Write("Trade Failure : Weight Exceed. %s(%s)", pc->name, pc->userID );
		return( 0 );
	}
	
	for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		if( pc->inventory[i] < 0 ) myEmptyInven ++;
	}
	if( myEmptyInven < myItemInven )
	{
		reply = 0;
		error = ERROR_PCTRADE_2;
		g_logSystem->Write("Trade Failure : Inventory Space Not Enough. %s(%s)", pc->name, pc->userID );
		return( 0 );
	}
	reply = 1;
	error = 0;
	return( 1 );
}

int GTH_ConfirmTrade( playerCharacter_t *pc, int& reply, int &error )
{
	int i;
	int itemIdx, inventoryPos, stackItemIdx;
	char generateID[20]; 
	item_t* item, *upItem;

	char logBuffer[100];
	int bIsStack = false, stackNumber = 1;

	if( reply == 0 ) return( 0 );

	
	pc->curChargeSE -= pc->tradeSE;
	pc->curChargeSE += pc->otherTradeSE;
	
	if( pc->curChargeSE > MAX_OWN_NAK ) pc->curChargeSE = MAX_OWN_NAK;

	if (pc->tradeSE > 0 || pc->otherTradeSE > 0 )
	{
		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_CONFIRM_TRADE, 0, 
			pc->otherTrade - pc->tradeSE, 
			"NAK",
			pc->friendlyEntityName );
		
	}

	
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		if( pc->myTrade[i] < 0 ) continue;
		itemIdx = pc->myTrade[i];
		item = &pc->item[itemIdx];

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[item->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = item->durability + 1;
		}

		_snprintf( logBuffer, 100, "%s, %s", pc->friendlyEntityName, 
			GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, -1 ));
		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_CONFIRM_TRADE, 1, stackNumber, item->name,
			logBuffer );
		

		g_logSystem->Write("Delete Trade Item : %s(%s) item : %s, To:(%s) ", pc->name, pc->userID, item->name, pc->friendlyEntityName );

		GTH_DeleteMyItem( pc, item );
		
		if ( !g_DBGameServer->RemoveItem(pc, itemIdx) )
		{
			g_logSystem->Write("Can not Delete Item in Database(Item table).(%s, %d, %d, %d", pc->userID, pc->serverGroupID, pc->characterID, itemIdx);
			reply = 0;
			error = ERROR_PCTRADE_4;
			return( 0 );
		}
	}
	
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		if( pc->otherTrade[i] < 0 ) continue;
		itemIdx = pc->otherTrade[i];
		item = &pc->tradeItem[itemIdx];

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[item->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = item->durability + 1;
		}

		stackItemIdx = GTH_StackMyItem( pc, item );
		if( stackItemIdx >= 0 )
		{
			upItem = &pc->item[stackItemIdx];
			upItem->durability += item->durability + 1;
			if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;

			g_DBGameServer->SaveItem( pc, upItem, stackItemIdx );
						
			_snprintf( logBuffer, 100, "%s, %s", pc->friendlyEntityName, 
				GetItemInformationLog( bIsStack, upItem->itemTableIdx, stackItemIdx, -1 ));

			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_CONFIRM_TRADE, 2, stackNumber, upItem->name,
				logBuffer );
			
		}
		else
		{
			
			inventoryPos = GTH_FindEmptyInventoryPos( pc );
			
			itemIdx = GTH_AddMyItem( pc, item );
			
			pc->inventory[inventoryPos] = itemIdx;
			
			strcpy(generateID, "tradeGenID");
			if ( !g_DBGameServer->AddItem(pc, item, itemIdx, generateID) )
			{
				g_logSystem->Write("Can not Insert to Database(Item Table).(%s, %d, %d)", pc->userID, pc->serverGroupID, pc->characterID);
				reply = 0;
				error = ERROR_PCTRADE_4;
				return( 0 );
			}

			_snprintf( logBuffer, 100, "%s, %s", pc->friendlyEntityName, 
				GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, inventoryPos ));

			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_CONFIRM_TRADE, 2, stackNumber, item->name,
				logBuffer );
			
		}
		
		g_logSystem->Write("Insert Trade Item : %s(%s) item : %s, From:(%s) ", pc->name, pc->userID, item->name, pc->friendlyEntityName );
	}
	reply = 1;
	error = 0;

	return( 1 );
}

void GTH_InitTrade( playerCharacter_t* pc )
{
	int i;
	for( i = 0; i < MAX_TRADE_SIZE; i ++ )
	{
		pc->myTrade[i] = -1;
		pc->otherTrade[i] = -1;
		memset( &pc->tradeItem[i], 0, sizeof( item_t ) );
		pc->tradeItem[i].itemTableIdx = -1;
		pc->tradeSE = 0;
		pc->otherTradeSE = 0;
	}
}

void GTH_ForcedQuitTrade( playerCharacter_t* pc )
{
	int reply = 1, error = 0, idx;

	idx = pc->friendlyEntityIdx;
	if( idx < 0 ) return;
	if( !g_pc[idx].active ) return;
	if( stricmp( g_pc[idx].name, pc->friendlyEntityName ) ) return;

	
	GTH_UndoPCTrade( pc, 2 );
	GTH_UndoPCTrade( &g_pc[idx], 2 );

	GTH_InitTrade( pc );
	GTH_InitTrade( &g_pc[idx] );

	
	GTH_SendMessage_PCTrade_Exit( pc, reply, error );
	GTH_SendMessage_PCTrade_Exit( &g_pc[idx], reply, error );

	
	
	GTH_DeleteNoticePC( pc );
	GTH_DeleteNoticePC( &g_pc[idx] );

}


void GTH_ForcedQuitCraft( playerCharacter_t *pc )
{
	GTH_UndoPCCraft( pc );
	GTH_DeleteNoticePC( pc );
}

void GTH_ForcedQuitUpgrade( playerCharacter_t *pc )
{
	GTH_UndoPCUpgrade( pc );
	GTH_DeleteNoticePC( pc );
}

void GTH_ForcedQuitBooth( playerCharacter_t *pc )
{
	if( pc->busyState == BUSY_STATE_BOOTH_SELL_PREPARE )
	{
		GTH_UndoPCBooth( pc );
		GTH_DeleteNoticePC( pc );
	}
	else if( pc->busyState == BUSY_STATE_BOOTH_SELL )
	{
		
		int pcIdx;
		for( int i = 0; i < MAX_COUNT_BOOTH_BUYER; i ++ )
		{
			if( pc->boothBuyerIdx[i] < 0 ) continue;
			pcIdx = pc->boothBuyerIdx[i];
		
			
			
			if( g_pc[pcIdx].busyState == BUSY_STATE_BOOTH_BUY )
			{
				GTH_SendMessage_ItemSystem_BoothBuyClose( &g_pc[pcIdx], 1, 0, 0 );
				GTH_DeleteNoticePC( &g_pc[pcIdx] );
			}
			else
			{
				g_logSystem->Write("Shop DeleteBusyState Error : %s(%s) Cur Status(%d)", g_pc[pcIdx].name, g_pc[pcIdx].userID, g_pc[pcIdx].busyState );
				
				g_logSystem->WriteToLog( &g_pc[pcIdx], LOG_CLASS_ITEM, "%d;;;;;%s;%s", g_pc[pcIdx].busyState, "Shop Error", "ForcedQuitBooth" );
			}
			pc->boothBuyerIdx[i] = -1;
		}
		GTH_UndoPCBooth( pc );
		GTH_DeleteNoticePC( pc );
	}
	else if( pc->busyState == BUSY_STATE_BOOTH_BUY )
	{
		int idx = pc->friendlyEntityIdx;
		
		
		GTH_DeleteNoticePC( pc );
		if( idx < 0 ) return;

		for( int i = 0; i < MAX_COUNT_BOOTH_BUYER; i ++ )
		{
			if( g_pc[idx].boothBuyerIdx[i] < 0 ) continue;
			if( g_pc[idx].boothBuyerIdx[i] == pc->idx )
			{
				g_pc[idx].boothBuyerIdx[i] = -1;
				break;
			}
		}
		GTH_SendMessage_ItemSystem_BoothBuyClose( &g_pc[idx], 1, 0, 1, pc->name );
	}
}


void GTH_ForcedQuitQuestScroll( playerCharacter_t *pc )
{
	GTH_DeleteNoticePC( pc );
}


void GTH_ForcedQuitAutoQuestScroll( playerCharacter_t *pc )
{
	GTH_DeleteNoticePC( pc );
}




void GTH_UpdatePartyInfo( playerCharacter_t *pc )
{
}





void GTH_GetStackItem( playerCharacter_t *pc, int &reply, int &error, int inventoryPos, int numStack )
{
	
	item_t *item, stackItem;
	int itemIdx = pc->inventory[inventoryPos], oldItemIdx;
	char generateID[20];

	item = &pc->item[itemIdx];
	if( item->durability <= numStack - 1 )
	{
		pc->inventory[inventoryPos] = -1;
		pc->mouseInventory = itemIdx;
		reply = 1;
		return;
	}

	oldItemIdx = pc->inventory[inventoryPos];

	memcpy( &stackItem, item, sizeof( item_t ) );
	stackItem.durability = numStack - 1;
	
	itemIdx = GTH_AddMyItem( pc, &stackItem );
	if (itemIdx < 0 )	
	{
		reply = 0;
		error = ERROR_EQUIPITEM_5;
		return;
	}
	item->durability -= numStack;
	pc->mouseInventory = itemIdx;

	
	g_DBGameServer->SaveItem( pc, item, oldItemIdx );

	strcpy(generateID, "stackGenID");
	g_DBGameServer->AddItem(pc, &stackItem, itemIdx, generateID);
	
	return;
}


void GTH_SetStackItem( playerCharacter_t *pc, int &reply, int &error )
{
	int stackItemIdx, mouseItemIdx;
	item_t* item, *upItem;

	mouseItemIdx = pc->mouseInventory;
	item = &pc->item[mouseItemIdx];

	stackItemIdx = GTH_StackMyItem( pc, item );

	if( stackItemIdx >= 0 )
	{
		upItem = &pc->item[stackItemIdx];
		upItem->durability += item->durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;
		
		pc->mouseInventory = -1;
		GTH_DeleteMyItem( pc, item );

		g_DBGameServer->SaveItem( pc, upItem, stackItemIdx );
		g_DBGameServer->RemoveItem( pc, mouseItemIdx );
	}
	else
	{
		int inventoryPos;

		
		inventoryPos = GTH_FindEmptyInventoryPos( pc );
		if( inventoryPos < 0 )	
		{
			reply = 0;
			error = ERROR_EQUIPITEM_5;
			return;
		}
		pc->inventory[inventoryPos] = pc->mouseInventory;
		
		pc->mouseInventory = -1;
	}
}


void GTH_GetStackItemForDepot( playerCharacter_t *pc, int &reply, int &error, int depotPos, int numStack )
{
	
	item_t *item, stackItem;
	int itemIdx = pc->depot[depotPos], oldItemIdx;
	char generateID[20];

	item = &pc->item[itemIdx];
	if( item->durability <= numStack - 1 )
	{
		pc->depot[depotPos] = -1;
		pc->mouseInventory = itemIdx;
		reply = 1;
		return;
	}

	oldItemIdx = pc->depot[depotPos];

	memcpy( &stackItem, item, sizeof( item_t ) );
	stackItem.durability = numStack - 1;
	
	itemIdx = GTH_AddMyItem( pc, &stackItem );
	if (itemIdx < 0 )	
	{
		reply = 0;
		error = ERROR_DEPOT_1;
		return;
	}
	item->durability -= numStack;
	pc->mouseInventory = itemIdx;

	
	g_DBGameServer->SaveItem( pc, item, oldItemIdx );

	strcpy(generateID, "stackGenID");
	g_DBGameServer->AddItem(pc, &stackItem, itemIdx, generateID);
	
	return;
}


void GTH_SetStackItemForDepot( playerCharacter_t *pc, int &reply, int &error )
{
	int stackItemIdx, mouseItemIdx;
	item_t* item, *upItem;

	mouseItemIdx = pc->mouseInventory;
	item = &pc->item[mouseItemIdx];

	stackItemIdx = GTH_StackMyItemForDepot( pc, item );

	if( stackItemIdx >= 0 )
	{
		upItem = &pc->item[stackItemIdx];
		upItem->durability += item->durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;
		
		pc->mouseInventory = -1;
		GTH_DeleteMyItem( pc, item );

		g_DBGameServer->SaveItem( pc, upItem, stackItemIdx );
		g_DBGameServer->RemoveItem( pc, mouseItemIdx );
	}
	else
	{
		int depotPos;

		
		depotPos = GTH_FindEmptyDepotPos( pc );
		if( depotPos < 0 )	
		{
			reply = 0;
			error = ERROR_DEPOT_1;
			return;
		}
		pc->depot[depotPos] = pc->mouseInventory;
		pc->mouseInventory = -1;
	}
}






bool GTH_IsOnPlayQuestScroll( playerCharacter_t *pc )
{
	int i;
	for ( i = 0; i< MAX_COUNT_QUEST_STATUS; i++ )
	{
		if ( pc->questStatus[i].questIdx == QUEST_SCROLL_INDEX )
			return 1;
	}
	return 0;
}


bool GTH_IsOnPlayAutoQuestScroll(playerCharacter_t *pc)
{
	
	for ( int i = 0; i< MAX_COUNT_QUEST_STATUS; i++ )
	{
		
		if ( pc->questStatus[i].questIdx == AUTOQUESTSCROLL_ITEM_INDEX )
		{
			return 1;
		}
	}
	return 0;
}

int GTH_GetEmptyQuestStatus( playerCharacter_t *pc )
{
	int i;
	for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		if( pc->questStatus[i].questIdx < 0 ) return i;
	}
	return -1;
}

void GTH_SetQuestStatus( playerCharacter_t *pc, int idx, int questIdx, int limitTime, int partyFlag, int *dField )
{
	pc->questStatus[idx].idx = idx;
	pc->questStatus[idx].questIdx = questIdx;
	pc->questStatus[idx].limitTime = limitTime;
	
	if( partyFlag )
	{
		strcpy( pc->questStatus[idx].partyOrganizerName, pc->organizerName );
		pc->questStatus[idx].partyOrganizeServer = pc->organizeServer;
		pc->questStatus[idx].partyOrganizeTime = pc->organizeTime;
	}
	if( dField != NULL )
	{
		memcpy( &pc->questStatus[idx].dField, dField, sizeof( pc->questStatus[idx].dField ) );
	}
	else
	{
		memset( pc->questStatus[idx].dField, 0, sizeof( pc->questStatus[idx].dField ) );
	}
	pc->questStatus[idx].clearTime = 0;

	g_DBGameServer->AddQuestStatus( pc, &pc->questStatus[idx] );
	pc->questNumber++;

	GTH_SendMessage_QuestStatusInfo( pc, 1, questIdx );
}


void GTH_AutoSetQuestStatus( playerCharacter_t *pc, int idx, int questIdx, int limitTime , int partyFlag , int *dField)
{
	pc->questStatus[idx].idx = idx; 
	pc->questStatus[idx].questIdx = questIdx;
	pc->questStatus[idx].limitTime = limitTime;

	if( partyFlag )
	{
		strcpy( pc->questStatus[idx].partyOrganizerName, pc->organizerName );
		pc->questStatus[idx].partyOrganizeServer = pc->organizeServer;
		pc->questStatus[idx].partyOrganizeTime = pc->organizeTime;
	}
	if( dField != NULL )
	{
		memcpy( &pc->questStatus[idx].dField, dField, sizeof( pc->questStatus[idx].dField ) );
	}
	else
	{
		memset( pc->questStatus[idx].dField, 0, sizeof( pc->questStatus[idx].dField ) );
	}
	pc->questStatus[idx].clearTime = 0;

	g_DBGameServer->AddQuestStatus( pc, &pc->questStatus[idx] );
	pc->questNumber++;



		int ifild[2];
		ifild[0]=questIdx+pc->questStatus[idx].dField[0];
		ifild[1]=pc->questStatus[idx].dField[0];
		AI_NPC_AutoSetQuestType(pc,ifild);
		
		int type=1;
		GTH_SendMessage_AutoQuestStatusInfo( pc, type,  pc->questStatus[idx].dField[0] );
		ShowLogInfo("autoqueststatusInfo: userID:%s,questIdx:%d,type:%d",pc->name,pc->questStatus[idx].dField[0],type);
}


void GTH_ClearQuestStatus( playerCharacter_t *pc, int idx, int type )
{
	
	int QuestStatusIdx = 0;
	
	
	for( int i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		if( pc->questStatus[i].questIdx == idx ) 
		{
			
			QuestStatusIdx = i;
			break;
		}
	}
	if( i == MAX_COUNT_QUEST_STATUS ) return ;

	pc->questStatus[i].questIdx = -1;
	g_DBGameServer->RemoveQuestStatus( pc, i );
	pc->questNumber--;
    int  autoquestindex=-1;
	if( !type ) 
	{
		
		
		
		if ( idx == QUEST_SCROLL_INDEX )
		{
			int addItemData[6];

			addItemData[0] = QUESTSCROLL_ITEM_INDEX;		
			addItemData[1] = -1;							
			addItemData[2] = 1;								
			addItemData[3] = 0;								
			addItemData[4] = pc->questStatus[i].dField[0];	
			addItemData[5] = pc->questStatus[i].dField[5];	
			
			if ( g_questscrollTable[pc->questStatus[i].dField[0]].data.rewardField[pc->questStatus[i].dField[5]] < 0 )
				addItemData[5] = FindRewardField( pc->questStatus[i].dField[0] );

			if ( addItemData[5] < 0 )
			{
				
				g_logSystem->Write("Quest Scroll Table has an Error! (User: %s, Index : %d)", pc->name, pc->questStatus[i].dField[0] );
			}
			else
				AI_NPC_AddItem( pc, addItemData );

		}

		
		
		if(idx == AUTOQUESTSCROLL_ITEM_INDEX)
		{
			
			
			
			

		}

		
		for( i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
		{
			if( pc->questInventory[i].item.itemIdx < 0 ) continue;

			
			switch( idx )
			{
				
			case 0 : case 2 :
				if( pc->questInventory[i].item.itemIdx != 210 ) continue;
				break;
				
			case 1 :
				if( pc->questInventory[i].item.itemIdx != 211 ) continue;
				break;
				
			case 3 :
				if( pc->questInventory[i].item.itemIdx != 268 ) continue;
				break;
			default :
				if( pc->questInventory[i].item.questIdx != idx ) continue;
				break;
			}
			pc->questInventory[i].item.itemIdx = -1;
			pc->questInventory[i].item.questIdx = 0;
			pc->questInventory[i].item.itemCount = 0;
		}
		GTH_SendMessage_QuestItemInfo( pc, -1, 0 );

		
		pc->questVar[idx].var.step = -1;
		pc->questVar[idx].var.isComplete = 0;
		pc->questVar[idx].var.dummy = 0;

		
		if(idx == AUTOQUESTSCROLL_ITEM_INDEX)
		{
			GTH_SendMessage_AutoQuestStatusInfo(pc,0,autoquestindex);
		}
		
		else
		{
			GTH_SendMessage_QuestStatusInfo( pc, 0, idx );
		}
	
	}
	else
	{
		
		
		GTH_SendMessage_QuestStatusInfo( pc, 2, idx );
	}
	
	
	
	memset(pc->questStatus[QuestStatusIdx].dField, -1, sizeof(int)*MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD );
	
	
}



void GTH_AutoClearQuestStatus( playerCharacter_t *pc, int idx, int type )
{
	for( int i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		if( pc->questStatus[i].questIdx == idx ) break;
	}
	if( i == MAX_COUNT_QUEST_STATUS ) return ;

	pc->questStatus[i].questIdx = -1;
	pc->questStatus[i].limitTime= -1;
	pc->questStatus[i].clearTime= -1;
	g_DBGameServer->RemoveQuestStatus( pc, i );
	pc->questNumber--;


	switch(type) {

	 case -1: 
		{
			if(idx == AUTOQUESTSCROLL_ITEM_INDEX)
			{
				int addItemData[6];
				
				addItemData[0] = AUTOQUESTSCROLL_ITEM_INDEX;		
				addItemData[1] = -1;							
				addItemData[2] = 1;								
				addItemData[3] = 0;								
				addItemData[5] = -1;	
				addItemData[4] = pc->questStatus[i].dField[0];	
				AI_NPC_AddItem( pc, addItemData );
				
			}
			
			
			for( i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
			{
				if( pc->questInventory[i].item.itemIdx < 0 ) continue;
				
				
				switch( idx )
				{
					
				case 0 : case 2 :
					if( pc->questInventory[i].item.itemIdx != 210 ) continue;
					break;
					
				case 1 :
					if( pc->questInventory[i].item.itemIdx != 211 ) continue;
					break;
					
				case 3 :
					if( pc->questInventory[i].item.itemIdx != 268 ) continue;
					break;
				default :
					if( pc->questInventory[i].item.questIdx != idx ) continue;
					break;
				}
				pc->questInventory[i].item.itemIdx = -1;
				pc->questInventory[i].item.questIdx = 0;
				pc->questInventory[i].item.itemCount = 0;
			}
			GTH_SendMessage_QuestItemInfo( pc, -1, 0 );
			
			
			pc->questVar[idx].var.step = -1;
			pc->questVar[idx].var.isComplete = 0;
			pc->questVar[idx].var.dummy = 0;
			pc->questVar[idx].var.completeCount=0;

			
			
			GTH_SendMessage_AutoQuestStatusInfo(pc,type,pc->questStatus[i].dField[0]);
			
		}
		break;
	case 1: 
		{
			GTH_SendMessage_AutoQuestStatusInfo(pc,2,pc->questStatus[i].dField[0]);
		}
		
		break;
	}
	
	memset(pc->questStatus[idx].dField, -1, sizeof(int)*MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD );
}

int GTH_GetQuestStatusDynamicField( playerCharacter_t *pc, int questIdx, int fieldIdx )
{
	for( int i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		if( pc->questStatus[i].questIdx == questIdx ) break;
	}
	if( i == MAX_COUNT_QUEST_STATUS ) return -1;

	return( pc->questStatus[i].dField[fieldIdx] );
}

int GTH_SetQuestStatusDynamicField( playerCharacter_t *pc, int questIdx, int fieldIdx, int value )
{
	for( int i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		if( pc->questStatus[i].questIdx == questIdx ) break;
	}
	if( i == MAX_COUNT_QUEST_STATUS ) return -1;

	pc->questStatus[i].dField[fieldIdx] = value;

	return 1;
}

int GTH_ConsumptiveItemProcess( playerCharacter_t *pc, int inventoryPos )
{
	int itemIdx = pc->inventory[inventoryPos];
	int itemTableIdx = pc->item[itemIdx].itemTableIdx;
	int param[10];

	switch( itemTableIdx )
	{
	case ITEM_TABLE_IDX_ERIN_OF_TIME :
		{
			GTH_PC_GetLoot( pc->idx, g_itemTable[itemTableIdx].commonField[0], 0, 0, 0, 0 );
			param[0] = itemIdx;
			param[1] = 0;
			AI_NPC_DeleteItem( pc, param );
		}
		break;
	default :
		return 0;
	}
	return 1;
}



void GTH_InitPlayerStatus( playerCharacter_t *pc )
{
	int reply = 1, error = 0;
	int force, agi, soul, vit;


	
	if( pc->curChargeSE < pc->npcAi.globalData[0] )
	{
		reply = 0;
		error = 1;
		GTH_SendMessage_QuestLogic_InitStatus( pc, reply, error, 1, 0 );

		return;
	}

	
	for( int i = 0; i < MAX_EQUIPMENT; i ++ )
	{
		if( i == EQUIP_EPACK ) continue;
		if( pc->equipment[i] >= 0 ) break;
	}
	if( i != MAX_EQUIPMENT )
	{
		reply = 0;
		error = 2;
		GTH_SendMessage_QuestLogic_InitStatus( pc, reply, error, 1, 0 );

		return;
	}

	if(pc->PremiumInfo.Status.iDefaultInitCount + pc->PremiumInfo.Status.iInitCount <= 0)
	{
		reply = 0;
		error = 3;
		GTH_SendMessage_QuestLogic_InitStatus(pc, reply, error, 1, 0);

		return;
	}


	
	
	extern int PC_INITIAL_STATUS[8][4];
	
	force	=	PC_INITIAL_STATUS[pc->pcClass][STATUS_FORCE];
	soul	=	PC_INITIAL_STATUS[pc->pcClass][STATUS_SOULPOWER];
	agi		=	PC_INITIAL_STATUS[pc->pcClass][STATUS_AGILITY];
	vit		=	PC_INITIAL_STATUS[pc->pcClass][STATUS_VITALITY];

	pc->curChargeSE -= pc->npcAi.globalData[0];

	pc->force				=	force	+	( pc->level - 1 );
	pc->soulpower			=	soul	+	( pc->level - 1 );
	pc->agility				=	agi		+	( pc->level - 1 );
	pc->vitality			=	vit		+	( pc->level - 1 );
	pc->bonusStatusPoint	=	pc->level;

	switch(pc->pcJob){
		case J_MENTALITY :
			break;			
		case J_WIND :
			break;
		case J_WATER :
			break;
		case J_EARTH:
			break;
		case J_DEVOTIONAL_KNIGHT :
			pc->agility += 5;
			break;

		case J_ROMING_KNIGHT :
			pc->vitality += 5;
			break;

		case J_PIECING_EYES :
			pc->soulpower += 5;
			break;

		case J_WINDY_ROMER :
			pc->vitality += 5;
			break;

		case J_DESTROYER :
			pc->agility += 5;
			break;

		case J_GUARDIAN :
			pc->vitality += 5;
			break;

		case J_PROPHECIER :
			break;		
		case J_TRANSFORMER :
			pc->agility += 5;
			break;

		case J_DEFENDER :
			pc->vitality += 5;
			break;

		case J_STRANGER :
			break;
	}

	
	
	
	pc->PremiumInfo.Status.iInitCount -=1;
	if(pc->PremiumInfo.Status.iInitCount <= 0){	
		pc->PremiumInfo.Status.iInitCount=0;
		pc->PremiumInfo.Status.iDefaultInitCount-=1;
		if(pc->PremiumInfo.Status.iDefaultInitCount < 0)
			pc->PremiumInfo.Status.iDefaultInitCount=0;
	}


	GTH_SendMessage_QuestLogic_InitStatus( pc, reply, error, 1, 0 );
	GTH_PC_Calculate( pc );
}

void GTH_ProcessMessage_QuestLogic()
{
	int command;

	command = MSG_ReadByte();

	switch( command )
	{
	case QUESTLOGIC_INITSTATUS :
		GTH_InitPlayerStatus( g_curPC );
		break;
	}
}





void GTH_PrecocityProcess( playerCharacter_t *pc )
{
	int itemTableIndex, outComeItemTableIndex;
	item_t *item;

	for( int i = 0; i < MAX_PRECOCITY_SIZE; i ++ )
	{
		if( pc->precocityInventory[i] < 0 ) continue;
		if( pc->precocityTime[i] < 0 ) 
		{
			item = &pc->item[pc->precocityInventory[i]];
			itemTableIndex = item->itemTableIdx;

			if( g_itemTable[itemTableIndex].etcItemType == ETCITEM_TYPE_PRECOCITY )
			{
				
				if(gGameConfigFlag.m_bPrecocity_Time)
				{
					pc->precocityTime[i] = g_itemTable[itemTableIndex].precocity.precocityTime * pc->PremiumInfo.fItemPrecocityTimeRatio;
				}
				else
				{
					pc->precocityTime[i] = g_itemTable[itemTableIndex].precocity.precocityTime;
				}
				
			}

			continue;
		}
		
		pc->precocityTime[i] --;
		
		if( pc->precocityTime[i] == 0 )
		{
			item = &pc->item[pc->precocityInventory[i]];

			pc->precocityTime[i] = -1;
			itemTableIndex = item->itemTableIdx;

			if( g_itemTable[itemTableIndex].etcItemType != ETCITEM_TYPE_PRECOCITY ) continue;

			outComeItemTableIndex = g_itemTable[itemTableIndex].precocity.outComeItemIndex;
			InitItem( item );

			
			item->itemTableIdx = outComeItemTableIndex;
			strcpy( item->name, g_itemTable[outComeItemTableIndex].name );
			if( g_itemTable[outComeItemTableIndex].durability > 0 ) item->durability = g_itemTable[outComeItemTableIndex].durability;

			
			g_DBGameServer->SaveItem( pc, item, pc->precocityInventory[i] );
			GTH_SendMessage_ItemSystem_PrecocityComplete( pc, i );

			
			if( g_itemTable[outComeItemTableIndex].etcItemType == ETCITEM_TYPE_PRECOCITY ) 
			{
				
				if(gGameConfigFlag.m_bPrecocity_Time)
				{
					pc->precocityTime[i] = g_itemTable[outComeItemTableIndex].precocity.precocityTime * pc->PremiumInfo.fItemPrecocityTimeRatio;
				}
				else
				{
					pc->precocityTime[i] = g_itemTable[outComeItemTableIndex].precocity.precocityTime;
				}
				
			}
		}
	}
}





void GTH_ProcessMessage_PCTrade()
{
	int command;

	command = MSG_ReadByte();

	switch( command )
	{
		
	case PCTRADE_REQUEST :
		GTH_ProcessMessage_PCTrade_Request();
		break;
	case PCTRADE_ADMIT :
		GTH_ProcessMessage_PCTrade_Admit();
		break;
	case PCTRADE_REJECT :
		GTH_ProcessMessage_PCTrade_Reject();
		break;

		
	case PCTRADE_EXIT :
		if( g_curPC->busyState != BUSY_STATE_TRADE ) break;
		GTH_ProcessMessage_PCTrade_Exit();
		break;
	case PCTRADE_ITEM :
		if( g_curPC->busyState != BUSY_STATE_TRADE ) break;
		GTH_ProcessMessage_PCTrade_Item();
		break;
	case PCTRADE_OK :
		if( g_curPC->busyState != BUSY_STATE_TRADE ) break;
		GTH_ProcessMessage_PCTrade_Ok();
		break;
	case PCTRADE_CANCEL :
		if( g_curPC->busyState != BUSY_STATE_TRADE ) break;
		GTH_ProcessMessage_PCTrade_Cancel();
		break;
	case PCTRADE_CONFIRM :
		if( g_curPC->busyState != BUSY_STATE_TRADE ) break;
		GTH_ProcessMessage_PCTrade_Confirm();
		break;
	case PCTRADE_SE :
		if( g_curPC->busyState != BUSY_STATE_TRADE ) break;
		GTH_ProcessMessage_PCTrade_SE();
		break;
	}
}


void GTH_ProcessMessage_PCTrade_Request()
{
	int idx, reply = 1, error = 0;
	idx = MSG_ReadShort();

	
	if( !g_pc[idx].active || !g_pc[idx].ready || !g_pc[idx].alive ) 
	{
		reply = 0;
		error = ERROR_ENTITYMENU_1;
	}
	else if( g_pc[idx].worldIdx != g_curPC->worldIdx )
	{
		reply = 0;
		error = ERROR_ENTITYMENU_2;
	}
	else if( g_curPC->busyState != BUSY_STATE_NONE || g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		reply = 0;
		error = ERROR_ENTITYMENU_2;
	}

	
	if( reply )
	{
		
		
		GTH_AddNoticePC( g_curPC, ENTITY_PC, idx, BUSY_STATE_REQUEST );
		GTH_AddNoticePC( &g_pc[idx], ENTITY_PC, g_curPC->idx, BUSY_STATE_REQUEST );

		GTH_SendMessage_PCTrade_Request( &g_pc[idx], reply, error, g_curPC->idx, 0 );
	}
	GTH_SendMessage_PCTrade_Request( g_curPC, reply, error, idx, 1 );
}
void GTH_ProcessMessage_PCTrade_Admit()
{
	int idx, reply = 1, error = 0;
	idx = MSG_ReadShort();

	
	if( !g_pc[idx].active || !g_pc[idx].ready || !g_pc[idx].alive ) 
	{
		reply = 0;
		error = ERROR_ENTITYMENU_1;
	}
	else if( g_pc[idx].worldIdx != g_curPC->worldIdx )
	{
		reply = 0;
		error = ERROR_ENTITYMENU_2;
	}
	else if( g_pc[idx].busyState != BUSY_STATE_REQUEST )
	{
		reply = 0;
		error = ERROR_ENTITYMENU_2;
	}
	else
	{		
		g_curPC->tradeSE = 0;
		g_curPC->otherTradeSE = 0;
		g_curPC->tradeAdmitSignal = false;
		g_curPC->tradeOkSignal = false;
		
		g_pc[idx].tradeSE = 0;
		g_pc[idx].otherTradeSE = 0;
		g_pc[idx].tradeAdmitSignal = false;
		g_pc[idx].tradeOkSignal = false;

		
		GTH_AddNoticePC( g_curPC, ENTITY_PC, idx, BUSY_STATE_TRADE );
		GTH_AddNoticePC( &g_pc[idx], ENTITY_PC, g_curPC->idx, BUSY_STATE_TRADE );
		
		GTH_InitTrade( g_curPC );
		GTH_InitTrade( &g_pc[idx] );

		g_logSystem->Write("Trade : Start %s(%s, %d, %d), %s(%s, %d, %d)", g_curPC->name, g_curPC->userID, g_curPC->serverGroupID, g_curPC->characterID,
			g_pc[idx].name, g_pc[idx].userID, g_pc[idx].serverGroupID, g_pc[idx].characterID );
	}

	GTH_SendMessage_PCTrade_Admit( g_curPC, reply, error );
	GTH_SendMessage_PCTrade_Admit( &g_pc[idx], reply, error );
}

void GTH_ProcessMessage_PCTrade_Reject()
{
	int type, idx, reply = 1, error = 0;
	
	
	
	type = MSG_ReadByte();
	idx = MSG_ReadShort();

	
	
	
	
	
	GTH_DeleteNoticePC( g_curPC );
	if( idx >= 0 )
		GTH_DeleteNoticePC( &g_pc[idx] );

	if( !type )
	{
		GTH_SendMessage_PCTrade_Reject( g_curPC, reply, error, false );
		GTH_SendMessage_PCTrade_Reject( &g_pc[idx], reply, error, true );
	}
	else
	{
		GTH_SendMessage_PCTrade_Reject( g_curPC, reply, error, true );
		GTH_SendMessage_PCTrade_Reject( &g_pc[idx], reply, error, false );
	}
}

void GTH_ProcessMessage_PCTrade_Exit()
{
	int idx, reply = 1, error = 0;

	idx = MSG_ReadShort();

	GTH_UndoPCTrade( g_curPC, 0 );
	GTH_UndoPCTrade( &g_pc[idx], 0 );

	GTH_InitTrade( g_curPC );
	GTH_InitTrade( &g_pc[idx] );

	GTH_SendMessage_PCTrade_Exit( g_curPC, reply, error );
	GTH_SendMessage_PCTrade_Exit( &g_pc[idx], reply, error );

	GTH_DeleteNoticePC( g_curPC );
	GTH_DeleteNoticePC( &g_pc[idx] );
}

void GTH_ProcessMessage_PCTrade_Item()
{
	int idx, pos, itemIdx;
	int type = 0;
	item_t item;
	int reply = 1, error = 0;

	idx = MSG_ReadShort();

	pos = MSG_ReadByte();
	InitItem( &item );


	
	
	if( g_curPC->friendlyEntityIdx != idx || g_pc[idx].friendlyEntityIdx != g_curPC->idx )
	{
		
		reply = 0;
		error = 1;
		
		
		GTH_SendMessage_PCTrade_Item( g_curPC, reply, error, true, type, pos, &item );
		return;
	}

	
	
	if( g_curPC->mouseInventory >= 0 && g_curPC->myTrade[pos] < 0 )
	{
		
		memcpy( &g_pc[idx].tradeItem[pos], &g_curPC->item[g_curPC->mouseInventory], sizeof( item_t ) );
		g_pc[idx].otherTrade[pos] = pos;
		memcpy( &item, &g_pc[idx].tradeItem[pos], sizeof( item_t ) );
		
		g_curPC->myTrade[pos] = g_curPC->mouseInventory;
		g_curPC->mouseInventory = -1;
		type = 0;
	}
	
	else if( g_curPC->mouseInventory < 0 && g_curPC->myTrade[pos] >= 0 )
	{
		
		InitItem( &g_pc[idx].tradeItem[pos] );
		g_pc[idx].otherTrade[pos] = -1;
		
		g_curPC->mouseInventory = g_curPC->myTrade[pos];
		g_curPC->myTrade[pos] = -1;
		type = 1;
	}
	
	else if( g_curPC->mouseInventory >= 0 && g_curPC->myTrade[pos] >= 0 )
	{
		
		memcpy( &g_pc[idx].tradeItem[pos], &g_curPC->item[g_curPC->mouseInventory], sizeof( item_t ) );
		g_pc[idx].otherTrade[pos] = pos;
		memcpy( &item, &g_pc[idx].tradeItem[pos], sizeof( item_t ) );
		
		itemIdx = g_curPC->mouseInventory;
		g_curPC->mouseInventory = g_curPC->myTrade[pos];
		g_curPC->myTrade[pos] = itemIdx;
		type = 2;
	}
	else
	{
		reply = 0;
		error = 1;
	}
	


	GTH_SendMessage_PCTrade_Item( &g_pc[idx], reply, error, false, type, pos, &item );
	GTH_SendMessage_PCTrade_Item( g_curPC, reply, error, true, type, pos, &item );
}

void GTH_ProcessMessage_PCTrade_Ok()
{
	int idx, reply = 1, error = 0;

	idx = MSG_ReadShort();

	g_curPC->tradeOkSignal = true;

	GTH_SendMessage_PCTrade_Ok( g_curPC, reply, error, true );
	GTH_SendMessage_PCTrade_Ok( &g_pc[idx], reply, error, false );
}

void GTH_ProcessMessage_PCTrade_Cancel()
{
	int idx, reply = 1, error = 0;

	idx = MSG_ReadShort();

	g_curPC->tradeOkSignal = false;
	g_curPC->tradeAdmitSignal = false;
	
	g_pc[idx].tradeOkSignal = false;
	g_pc[idx].tradeAdmitSignal = false;

	GTH_SendMessage_PCTrade_Cancel( g_curPC, reply, error, true );
	GTH_SendMessage_PCTrade_Cancel( &g_pc[idx], reply, error, false );
}

void GTH_ProcessMessage_PCTrade_Confirm()
{
	int idx, reply = 1, error = 0, type;

	type = MSG_ReadByte();
	idx = MSG_ReadShort();

	if( type )
	{
		g_curPC->tradeAdmitSignal = true;
		
		
		
		
		

		
		if( g_curPC->tradeAdmitSignal && g_pc[idx].tradeAdmitSignal )
		{
			GTH_CheckConfirmTrade( g_curPC, reply, error );
			GTH_CheckConfirmTrade( &g_pc[idx], reply, error );
			GTH_ConfirmTrade( g_curPC, reply, error );
			GTH_ConfirmTrade( &g_pc[idx], reply, error );
			
			
			GTH_SendMessage_PCTrade_Confirm( g_curPC, reply, error, 2 );
			GTH_SendMessage_PCTrade_Confirm( &g_pc[idx], reply, error, 2 );

			GTH_InitTrade( g_curPC );
			GTH_InitTrade( &g_pc[idx] );

			GTH_PC_Calculate( g_curPC, PCINFO_BASIC );
			GTH_PC_Calculate( &g_pc[idx], PCINFO_BASIC );

			GTH_DeleteNoticePC( g_curPC );
			GTH_DeleteNoticePC( &g_pc[idx] );

			g_logSystem->Write("Trade Successful : %s(%s) To:(%s)", g_curPC->name, g_curPC->userID, g_curPC->friendlyEntityName );
		}
		else
		{
			
			GTH_SendMessage_PCTrade_Confirm( &g_pc[idx], reply, error, 1 );
		}
	}
	else
	{
		
		g_curPC->tradeAdmitSignal = false;
		GTH_SendMessage_PCTrade_Confirm( g_curPC, reply, error, 0 );





	}
}

void GTH_ProcessMessage_PCTrade_SE()
{
	int reply = 1, error = 0, idx, se;
	
	idx = MSG_ReadShort();
	se = MSG_ReadLong();

	
	
	if( g_curPC->friendlyEntityIdx != idx || g_pc[idx].friendlyEntityIdx != g_curPC->idx )
	{
		
		reply = 0;
		error = 1;
		
		GTH_SendMessage_PCTrade_SE( g_curPC, reply, error, true, se );
		
		return;
	}
	
	if( se > g_curPC->curChargeSE )
	{
		reply = 0;
		error = ERROR_PCTRADE_5;
		GTH_SendMessage_PCTrade_SE( g_curPC, reply, error, true, se );
	}
	else
	{
		g_curPC->tradeSE = se;
		g_pc[idx].otherTradeSE = se;
		GTH_SendMessage_PCTrade_SE( g_curPC, reply, error, true, se );
		GTH_SendMessage_PCTrade_SE( &g_pc[idx], reply, error, false, se );
	}
}







void GTH_ProcessMessage_Party()
{
	int command;

	command = MSG_ReadByte();

	switch( command )
	{
	case PARTY_REQUEST_ORGANIZE :
		GTH_ProcessMessage_Party_Request_Organize();
		break;

	case PARTY_ADMIT_ORGANIZE :
		GTH_ProcessMessage_Party_Admit_Organize();
		break;

	case PARTY_REJECT_ORGANIZE :
		GTH_ProcessMessage_Party_Reject_Organize();
		break;
	
	case PARTY_REQUEST_JOIN :
		GTH_ProcessMessage_Party_Request_Join();
		break;


	case PARTY_ADMIT_JOIN :
		GTH_ProcessMessage_Party_Admit_Join();
		break;

	case PARTY_REJECT_JOIN :
		GTH_ProcessMessage_Party_Reject_Join();
		break;

	case PARTY_OUT :
		GTH_ProcessMessage_Party_Out();
		break;
	case PARTY_FORCEDOUT :
		GTH_ProcessMessage_Party_ForcedOut();
		break;
	case PARTY_DISORGANIZE :
		GTH_ProcessMessage_Party_Disorganize();
		break;

	case PARTY_CANJOIN :
		GTH_ProcessMessage_Party_CanJoin();
		break;

	case PARTY_ITEMDIVIDETYPE :
		GTH_ProcessMessage_Party_ItemDivideType();
		break;

	case PARTY_CHATMESSAGE :
		GTH_ProcessMessage_Party_ChatMessage();
		break;
	}
}







void GTH_ProcessMessage_Party_Request_Organize()
{
	int idx;
	int reply = 1, error = 0;

	idx = MSG_ReadShort();

	if( !g_pc[idx].active ) 
	{
		reply = 0;
		error = ERROR_PARTY_1;
		GTH_SendMessage_Party_Request_Organize( g_curPC, reply, error, idx, true );
	} else if( !g_pc[idx].ready || !g_pc[idx].alive || g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		reply = 0;
		error = ERROR_PARTY_2;
		GTH_SendMessage_Party_Request_Organize( g_curPC, reply, error, idx, true );
	}
	else if( g_pc[idx].memberNumber != 0 )
	{
		reply = 0;
		error = ERROR_PARTY_3;
		GTH_SendMessage_Party_Request_Organize( g_curPC, reply, error, idx, true );
	} else if( g_pc[idx].worldIdx != g_curPC->worldIdx )
	{
		reply = 0;
		error = ERROR_PARTY_4;
		GTH_SendMessage_Party_Request_Organize( g_curPC, reply, error, idx, true );
	} else if( !g_pc[idx].canJoin )
	{
		reply = 0;
		error = ERROR_PARTY_2;
		GTH_SendMessage_Party_Request_Organize( g_curPC, reply, error, idx, true );
	}
	else
	{
		GTH_SendMessage_Party_Request_Organize( g_curPC, reply, error, idx, true );
		GTH_SendMessage_Party_Request_Organize( &g_pc[idx], reply, error, g_curPC->idx, false );
	}
}

void GTH_ProcessMessage_Party_Admit_Organize()
{
	int idx;
	int reply = 1, error = 0;

	idx = MSG_ReadShort();

	if( g_curPC->memberNumber != 0 || g_pc[idx].memberNumber != 0 )
	{
		reply = 0;
		error = ERROR_PARTY_3;
		GTH_SendMessage_Party_Admit_Organize( g_curPC, reply, error, false );
		GTH_SendMessage_Party_Admit_Organize( &g_pc[idx], reply, error, true );
	}
	else if ( !g_pc[idx].active ) 
	{
		reply = 0;
		error = ERROR_PARTY_1;
		GTH_SendMessage_Party_Admit_Organize( g_curPC, reply, error, false );
		GTH_SendMessage_Party_Admit_Organize( &g_pc[idx], reply, error, true );
	}
	else if( !g_pc[idx].ready || !g_pc[idx].alive || g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		reply = 0;
		error = ERROR_PARTY_2;
		GTH_SendMessage_Party_Admit_Organize( g_curPC, reply, error, false );
		GTH_SendMessage_Party_Admit_Organize( &g_pc[idx], reply, error, true );

	}
	else if( g_pc[idx].worldIdx != g_curPC->worldIdx )
	{
		reply = 0;
		error = ERROR_PARTY_4;
		GTH_SendMessage_Party_Admit_Organize( g_curPC, reply, error, false );
		GTH_SendMessage_Party_Admit_Organize( &g_pc[idx], reply, error, true );	
	}
	else
	{
		
		GTH_PartyInfo_Request_Organize( &g_pc[idx], g_curPC );
	}
}


void GTH_ProcessMessage_Party_Reject_Organize()
{
	int idx, type;

	type = MSG_ReadByte();

	idx = MSG_ReadShort();

	if( type ) 
	{
		GTH_SendMessage_Party_Reject_Organize( g_curPC, true );
		GTH_SendMessage_Party_Reject_Organize( &g_pc[idx], false );
	}
	else
	{
		GTH_SendMessage_Party_Reject_Organize( g_curPC, false );
		GTH_SendMessage_Party_Reject_Organize( &g_pc[idx], true );
	}
}







void GTH_ProcessMessage_Party_Request_Join()
{
	int idx;
	int reply = 1, error = 0;

	idx = MSG_ReadShort();

	if( !g_pc[idx].active ) 
	{
		reply = 0;
		error = ERROR_PARTY_1;
		GTH_SendMessage_Party_Request_Join( g_curPC, reply, error, idx, true );
	} else if( !g_pc[idx].ready || !g_pc[idx].alive || g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		reply = 0;
		error = ERROR_PARTY_2;
		GTH_SendMessage_Party_Request_Join( g_curPC, reply, error, idx, true );
	}
	else if( g_pc[idx].memberNumber != 0 )
	{
		reply = 0;
		error = ERROR_PARTY_3;
		GTH_SendMessage_Party_Request_Join( g_curPC, reply, error, idx, true );
	} else if( g_pc[idx].worldIdx != g_curPC->worldIdx )
	{
		reply = 0;
		error = ERROR_PARTY_4;
		GTH_SendMessage_Party_Request_Join( g_curPC, reply, error, idx, true );
	} else if( g_curPC->memberNumber >= MAX_NUMBER_OF_PARTY_MEMBER )
	{
		reply = 0;
		error = ERROR_PARTY_6;
		GTH_SendMessage_Party_Request_Join( g_curPC, reply, error, idx, true );
	} else if( !g_pc[idx].canJoin )
	{
		reply = 0;
		error = ERROR_PARTY_2;
		GTH_SendMessage_Party_Request_Join( g_curPC, reply, error, idx, true );
	}
	else
	{
		GTH_SendMessage_Party_Request_Join( g_curPC, reply, error, idx, true );
		GTH_SendMessage_Party_Request_Join( &g_pc[idx], reply, error, g_curPC->idx, false );
	}
}


void GTH_ProcessMessage_Party_Admit_Join()
{
	int idx;
	int reply = 1, error = 0;

	idx = MSG_ReadShort();

	if( !g_pc[idx].active ) 
	{
		reply = 0;
		error = ERROR_PARTY_1;
		GTH_SendMessage_Party_Admit_Join( g_curPC, reply, error, false );
		GTH_SendMessage_Party_Admit_Join( &g_pc[idx], reply, error, true );
	} else if( !g_pc[idx].ready || !g_pc[idx].alive )
	{
		reply = 0;
		error = ERROR_PARTY_2;
		GTH_SendMessage_Party_Admit_Join( g_curPC, reply, error, false );
		GTH_SendMessage_Party_Admit_Join( &g_pc[idx], reply, error, true );
	}
	else if( g_pc[idx].worldIdx != g_curPC->worldIdx )
	{
		reply = 0;
		error = ERROR_PARTY_4;
		GTH_SendMessage_Party_Admit_Join( g_curPC, reply, error, false );
		GTH_SendMessage_Party_Admit_Join( &g_pc[idx], reply, error, true );
	}
	else if( g_pc[idx].memberNumber >= MAX_NUMBER_OF_PARTY_MEMBER )
	{
		reply = 0;
		error = ERROR_PARTY_6;
		GTH_SendMessage_Party_Request_Join( g_curPC, reply, error, -1, false );
		GTH_SendMessage_Party_Admit_Join( &g_pc[idx], reply, error, true );
	}
	else
	{
		
		GTH_PartyInfo_Request_Join( &g_pc[idx], g_curPC );
	}
}


void GTH_ProcessMessage_Party_Reject_Join()
{
	int idx, type;

	type = MSG_ReadByte();

	idx = MSG_ReadShort();

	if( type ) 
	{
		GTH_SendMessage_Party_Reject_Join( g_curPC, true );
		GTH_SendMessage_Party_Reject_Join( &g_pc[idx], false );
	}
	else
	{
		GTH_SendMessage_Party_Reject_Join( g_curPC, false );
		GTH_SendMessage_Party_Reject_Join( &g_pc[idx], true );
	}
}





void GTH_ProcessMessage_Party_Out()
{
	int reply = 1, error = 0;

	if( g_curPC->organizeServer == -1 )
	{
		reply = 0;
		error = ERROR_PARTY_5;

	}
	else
	{
		GTH_PartyInfo_Request_Out( g_curPC );
	}
}

void GTH_ProcessMessage_Party_ForcedOut()
{
	int reply = 1, error = 0;

	char memberName[NAMESTRING];

	strcpy( memberName, MSG_ReadString() );

	
	if( g_curPC->organizeServer == -1 )
	{
		reply = 0;
		error = ERROR_PARTY_7;
		GTH_SendMessage_Party_Out( g_curPC, reply, error, 0, 0, 0, 0 );
	}
	
	else if( !g_curPC->isLeader )
	{
		reply = 0;
		error = ERROR_PARTY_7;
		GTH_SendMessage_Party_Out( g_curPC, reply, error, 0, 0, 0, 0 );
	}
	else
	{
		for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
		{
			if( g_curPC->member[i].serverIndex == -1  ) continue;
			if( !stricmp( g_curPC->member[i].memberName, memberName ) ) break;
		}
		
		if( i == MAX_NUMBER_OF_PARTY_MEMBER )
		{
			reply = 0;
			error = ERROR_PARTY_5;
			GTH_SendMessage_Party_Out( g_curPC, reply, error, 0, 0, 0, 0 );
		}
		else
		{
			GTH_PartyInfo_Request_ForcedOut( g_curPC, g_curPC->member[i].memberName );
		}
	}
}

void GTH_ProcessMessage_Party_Disorganize()
{
	int reply = 1, error = 0;

	if( g_curPC->organizeServer == -1 )
	{
		reply = 0;
		error = ERROR_PARTY_5;
		GTH_SendMessage_Party_Out( g_curPC, reply, error, 0, 0, 0, 0 );
	}
	else
	{
		GTH_PartyInfo_Request_Disorganize( g_curPC );
	}
}

void GTH_ProcessMessage_Party_CanJoin()
{
	int type;
	type = MSG_ReadByte();
	g_curPC->canJoin = type;

	GTH_SendMessage_Party_CanJoin( g_curPC, type );
}

void GTH_ProcessMessage_Party_ItemDivideType()
{
	int type;
	type = MSG_ReadByte();

	GTH_PartyInfo_Request_ItemDivideType( g_curPC, type );
}

void GTH_ProcessMessage_Party_ChatMessage()
{
	char message[256];
	if( g_curPC->organizeServer < 0 ) return;

	strcpy( message, MSG_ReadString() );

	GTH_PartyInfo_Request_ChatMessage( g_curPC, message );
}












void GTH_ProcessMessage_Request_PCInfo()
{	
	GTH_SendMessage_Reply_PCInfo( g_curPC );
}





void GTH_ProcessMessage_Hotkey()
{
	int curIdx;
	int hotIdx, hotType;
	curIdx = MSG_ReadByte();
	hotType = MSG_ReadByte();
	hotIdx = MSG_ReadChar();

	g_curPC->hotkeyType[curIdx] = hotType;
	g_curPC->hotkeyIdx[curIdx] = hotIdx;
}






void GTH_ProcessMessage_Request_Use_Masic_Book()
{
	item_t *item;
	int itemIdx, itemTableIdx;
	int invenPos;
	int reply = 1, error = 0;


	invenPos = MSG_ReadByte();
	itemIdx = g_curPC->inventory[invenPos];

	if( itemIdx < 0 )
	{
		reply = 0;
		error = ERROR_CONSUMPTIVE_0;
		GTH_SendMessage_Reply_Use_Masic_Book( g_curPC, reply, error, invenPos );
		return;
	}
	else
	{
		item = &g_curPC->item[itemIdx];
		itemTableIdx = item->itemTableIdx;

		
		if(itemTableIdx == 465)
		{
			GTH_ProcessMessage_AllRepairItem(invenPos);
			GTH_UseItem_Delete(invenPos);
		}		
	}
}


bool	GTH_UseItem_Delete(int invenPos )
{
	int	itemIdx = g_curPC->inventory[invenPos];
	if(itemIdx < 0) 
	{
		return false;
	}

	item_t *item = item = &g_curPC->item[itemIdx];

	if( item->durability > 0 )
	{
		item->durability --;
		g_DBGameServer->SaveItem( g_curPC, item, itemIdx );															
	}
	else
	{	
		GTH_DeleteMyItem( g_curPC, item );
		g_curPC->inventory[invenPos] = -1;
		if ( !g_DBGameServer->RemoveItem( g_curPC, itemIdx ) )
		{	
			return false;				
		}
	}
	return true;
}

void GTH_ProcessMessage_Request_Use_Portal()
{
	item_t *item;
	int itemIdx, itemTableIdx, bindingIdx;
	int invenPos;
	int reply = 1, error = 0;
	int zoneIdx;
	vec3_t pos, rpos;
	int worldIdx;
	int memberIdx;
	vec3_t savePosition;
	int saveWorldIdx;

	invenPos = MSG_ReadByte();
	itemIdx = g_curPC->inventory[invenPos];

	
	
	
	if ( g_curPC->busyState != BUSY_STATE_NONE)
	{
		reply = 0;
		error = ERROR_CONSUMPTIVE_1;
		GTH_SendMessage_Reply_Use_Portal( g_curPC, reply, error, invenPos );		
		g_logSystem->Write("User (%s) Don't Move. User Busy State.", g_curPC->userID);
		return;
	}
	


	if( itemIdx < 0 )
	{
		reply = 0;
		error = ERROR_CONSUMPTIVE_0;
		GTH_SendMessage_Reply_Use_Portal( g_curPC, reply, error, invenPos );
		return;
	}
	else
	{
		item = &g_curPC->item[itemIdx];
		itemTableIdx = item->itemTableIdx;
		if( g_itemTable[itemTableIdx].etcItemType != ETCITEM_TYPE_PORTAL )
		{
			reply = 0;
			error = ERROR_CONSUMPTIVE_0;

			GTH_SendMessage_Reply_Use_Portal( g_curPC, reply, error, invenPos );
			return;
		}
		else
		{
			bindingIdx = g_itemTable[itemTableIdx].portal.bindingIdx;

			if( bindingIdx < 0 ) bindingIdx = 0;
			if( g_binding[bindingIdx].idx < 0 ) bindingIdx = 0;

			worldIdx = g_binding[bindingIdx].worldIdx;

			if( item->durability > 0 )
			{
				item->durability --;
				g_DBGameServer->SaveItem( g_curPC, item, itemIdx );
				g_logSystem->Write("Use Portal : From:(%s) %s", g_curPC->name, item->name);
				
				
				
				
				g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;;;;;%s;%s", 
					LOG_CLASS_ITEM_USE_PORTAL, item->name, 
					GetItemInformationLog( 1, item->itemTableIdx, itemIdx, -1 ) );
				
			}
			else
			{
				
				
				
				g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;;;;;%s;%s", 
					LOG_CLASS_ITEM_USE_PORTAL, item->name, 
					GetItemInformationLog( 0, item->itemTableIdx, itemIdx, -1 ) );
				

				g_logSystem->Write("Use Portal : From:(%s) %s", g_curPC->name, item->name);

				GTH_DeleteMyItem( g_curPC, item );
				g_curPC->inventory[invenPos] = -1;
				reply = 1;
				if ( !g_DBGameServer->RemoveItem( g_curPC, itemIdx ) )
				{
					g_logSystem->Write("Can not Item int DataBase(Item Table).(%s, %d, %d, %d", g_curPC->userID, g_curPC->serverGroupID, g_curPC->characterID, itemIdx);
					reply = 0;
					error = ERROR_CONSUMPTIVE_0;
					GTH_SendMessage_Reply_Use_Portal( g_curPC, reply, error, invenPos );
					return;
					
				}
			}

			
			if ( GTH_IsLocalServiceWorld(worldIdx) )
			{
				if( worldIdx == g_curPC->worldIdx ) 
				{
					vec_copy( g_binding[bindingIdx].pos, pos );
					rpos[0] = (float)GTH_Randomize( (int)g_binding[bindingIdx].range ) * 2.0f - g_binding[bindingIdx].range;
					rpos[1] = (float)GTH_Randomize( (int)g_binding[bindingIdx].range ) * 2.0f - g_binding[bindingIdx].range;
					
					pos[0] += rpos[0];
					pos[1] += rpos[1];
					
					vec_copy( pos, g_curPC->position );

					
					g_curPC->reliableWorldIdx = g_curPC->worldIdx;
					vec_copy( g_curPC->position, g_curPC->reliablePosition );

					zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
					g_curPC->zoneIdx = zoneIdx;

					
					GTH_SendMessage_Reply_Use_Portal( g_curPC, reply, error, invenPos );
					GTH_SendMessage_SyncItemObject( g_curPC );
				}
				else 
				{

					if ( g_curPC->worldIdx == DAN_BATTLEMAP_NO )								
					{
						g_logSystem->WriteRLog("Delete Team....Call by Same world");
						g_pDanBattleSys->DeleteTeam( g_curPC );
					}
			
					
					GTH_Zone_DeleteEntityList(g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->entityType, g_curPC->idx);
					
					g_curPC->worldIdx = worldIdx;
					vec_copy( g_binding[bindingIdx].pos, pos );
					rpos[0] = (float)GTH_Randomize( (int)g_binding[bindingIdx].range ) * 2.0f - g_binding[bindingIdx].range;
					rpos[1] = (float)GTH_Randomize( (int)g_binding[bindingIdx].range ) * 2.0f - g_binding[bindingIdx].range;
					
					pos[0] += rpos[0];
					pos[1] += rpos[1];
					
					vec_copy( pos, g_curPC->position );

					
					g_curPC->reliableWorldIdx = g_curPC->worldIdx;
					vec_copy( g_curPC->position, g_curPC->reliablePosition );
					
					g_curPC->zoneIdx = GTH_Zone_GetZoneIdx( g_curPC->position );
					
					GTH_SendMessage_Reply_Use_Portal( g_curPC, reply, error, invenPos );

					
					GTH_Zone_AddEntityList(g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->entityType, g_curPC->idx);

					g_curPC->ready = false;
					
					
					g_logSystem->Write("Use Portal : (%s) Move to Binding Pos, Pos(%.2f, %.2f, %.2f), World : %d", 
						g_curPC->userID, g_curPC->position[0], g_curPC->position[1], g_curPC->position[2], g_curPC->worldIdx);
				}
			}
			else 
			{

				
				if ( g_config.isDanBattleMap )													
				{
					if ( g_curPC->worldIdx == DAN_BATTLEMAP_NO )
					{
						g_logSystem->WriteRLog("Delete Team call by ....DB_SaveGameData");
						g_pDanBattleSys->DeleteTeam( g_curPC );
					}
				}
		

				
				saveWorldIdx = g_curPC->worldIdx;
				vec_copy(g_curPC->position, savePosition);
				
				
				g_curPC->worldIdx = worldIdx;
				
				vec_copy( g_binding[bindingIdx].pos, pos );
				rpos[0] = (float)( rand() % (int)g_binding[bindingIdx].range ) * 2.0f - g_binding[bindingIdx].range;
				rpos[1] = (float)( rand() % (int)g_binding[bindingIdx].range ) * 2.0f - g_binding[bindingIdx].range;
				
				pos[0] += rpos[0];
				pos[1] += rpos[1];
				
				vec_copy( pos, g_curPC->position );

				
				GTH_SendMessage_Reply_Use_Portal( g_curPC, reply, error, invenPos, true );
				
				memberIdx = GTH_FindServerWithinGroup( worldIdx );
				if (memberIdx < 0) 
				{
					
					g_curPC->flagChangeServer = -1;
					DB_SaveGameData( g_curPC, SAVEGAMEDATA_QUIT);
					return;
				}
				else
				{
					
					g_curPC->flagChangeServer = 1;
					g_curPC->changeServerWorldIdx = worldIdx;
					DB_SaveGameData( g_curPC, SAVEGAMEDATA_QUIT);
				}

				
				g_curPC->worldIdx = saveWorldIdx;
				vec_copy(savePosition, g_curPC->position);
			}
		}
	}
}

void GTH_ProcessMessage_Request_SavePortalPosition()
{
	int type;
	type = MSG_ReadByte();

	
	g_curPC->skillVariable[0] = g_curPC->worldIdx;
	g_curPC->skillVariable[1] = (int)g_curPC->position[0];
	g_curPC->skillVariable[2] = (int)g_curPC->position[1];
	g_curPC->skillVariable[3] = (int)g_curPC->position[2];

	GTH_SendMessage_Reply_SavePortalPosition( g_curPC, 1, 0 );
}




void GTH_ProcessMessage_ItemSystem()
{
	int command;

	command = MSG_ReadByte();

	switch( command )
	{
	case DEPOT_OPEN :
		break;
	case DEPOT_GETITEM :
		GTH_ProcessMessage_ItemSystem_DepotGetItem();
		break;
	case DEPOT_ITEM :
		GTH_ProcessMessage_ItemSystem_DepotItem();
		break;
	case DEPOT_NAK :
		GTH_ProcessMessage_ItemSystem_DepotNak();
		break;

	
	case PRECOCITY_OPEN :
		break;
	case PRECOCITY_ITEM :
		GTH_ProcessMessage_ItemSystem_PrecocityItem();
		break;
	case PRECOCITY_COMPLETE :
		break;

	
	case CRAFT_OPEN :
		GTH_ProcessMessage_ItemSystem_CraftOpen();
		break;
	case CRAFT_CLOSE :		
		GTH_ProcessMessage_ItemSystem_CraftClose();
		break;
	case CRAFT_ITEM :
		GTH_ProcessMessage_ItemSystem_CraftItem();
		break;
	case CRAFT_PREPARE :
		GTH_ProcessMessage_ItemSystem_CraftPrepare();
		break;
	case CRAFT_COMPLETE :
		GTH_ProcessMessage_ItemSystem_CraftComplete();
		break;
		
	
	case EVENT_CRAFT_OPEN :
		GTH_ProcessMessage_ItemSystem_EventCraftOpen();
		break;


	case EVENT_CRAFT_CLOSE :
		GTH_ProcessMessage_ItemSystem_EventCraftClose();
		break;
	case EVENT_CRAFT_ITEM :
		GTH_ProcessMessage_ItemSystem_EventCraftItem();
		break;
	case EVENT_CRAFT_PREPARE :
		GTH_ProcessMessage_ItemSystem_EventCraftPrepare();
		break;
	case EVENT_CRAFT_COMPLETE :
		GTH_ProcessMessage_ItemSystem_EventCraftComplete();
		break;
	case EVENT_CRAFT_LOADEVENT:
		GTH_ProcessMessage_ItemSystem_EventCraftLoadEvent();
		break;
	

	
	case BOOTH_SELL_OPEN :
		GTH_ProcessMessage_ItemSystem_BoothSellOpen();
		break;
	case BOOTH_SELL_ITEM :
		GTH_ProcessMessage_ItemSystem_BoothSellItem();
		break;
	case BOOTH_SELL_START :
		GTH_ProcessMessage_ItemSystem_BoothSellStart();
		break;
	case BOOTH_SELL_CLOSE :
		GTH_ProcessMessage_ItemSystem_BoothSellClose();
		break;

	case BOOTH_BUY_OPEN :
		GTH_ProcessMessage_ItemSystem_BoothBuyOpen();
		break;
	case BOOTH_BUY_ITEM :
		GTH_ProcessMessage_ItemSystem_BoothBuyItem();
		break;
	case BOOTH_BUY_CLOSE :
		GTH_ProcessMessage_ItemSystem_BoothBuyClose();
		break;

	
	case UPGRADE_OPEN :
		GTH_ProcessMessage_ItemSystem_UpgradeOpen();
		break;
	case UPGRADE_CLOSE :
		GTH_ProcessMessage_ItemSystem_UpgradeClose();
		break;
	case UPGRADE_ITEM :
		GTH_ProcessMessage_ItemSystem_UpgradeItem();
		break;
	case UPGRADE_PREPARE :
		GTH_ProcessMessage_ItemSystem_UpgradePrepare();
		break;
	case UPGRADE_COMPLETE :
		GTH_ProcessMessage_ItemSystem_UpgradeComplete();
		break;
	
	case DELETE_PREPARE :
		GTH_ProcessMessage_ItemSystem_DeletePrepare();
		break;
	case DELETE_COMPLETE :
		GTH_ProcessMessage_ItemSystem_DeleteComplete();
		break;
	
	case QUESTSCROLL_OBTAINPREPARE :
		GTH_ProcessMessage_QuestScroll_ObtainPrepare();
		break;
	case QUESTSCROLL_OBTAINCOMPLETE :
		GTH_ProcessMessage_QuestScroll_ObtainComplete();
		break;
	case QUESTSCROLL_OBTAINCLOSE :
		GTH_ProcessMessage_QuestScroll_ObtainClose();
		break;
	case QUESTSCROLL_COMBOLISTCHANGE :
		GTH_ProcessMessage_QuestScroll_ComboListChange();
		break;
  
	case AUTOQUESTSCROLL_NPCINVENTORYITEM:
	    GTH_ProcessMessage_ItemSystem_AutoCraftItem();  
		break;

	case AUTOQUESTSCROLL_NPCINVENTORYOPEN :
		GTH_ProcessMessage_ItemSystem_NpcInventoryOpen();
		break;
	case AUTOQUESTSCROLL_NPCINVENTORYCLOSE :
		GTH_ProcessMessage_ItemSystem_NpcInventoryClose();
		break;
	case AUTOQUESTSCROLL_NPCINVENTORYPREPARE :
		GTH_ProcessMessage_ItemSystem_AutoCraftPrepare();
		break;
	
	case EXCHANGE_CLOSE :
		GTH_ProcessMessage_ItemSystem_ExchangeClose();
		break;
	case EXCHANGE_ITEM :
		GTH_ProcessMessage_ItemSystem_ExchangeItem();
		break;
	case EXCHANGE_PREPARE :
		GTH_ProcessMessage_ItemSystem_ExchangePrepare();
		break;
	

	}
}

void GTH_ProcessMessage_ItemSystem_DepotGetItem()
{
	int depotPos, numStack;
	int itemIdx;
	int reply = 1, error = 0;

	depotPos = MSG_ReadShort();
	numStack = MSG_ReadShort();

	itemIdx = g_curPC->depot[depotPos];
	
	
	if( itemIdx >= 0 )
	{
		if( g_curPC->item[itemIdx].itemTableIdx < 0 )
		{
			g_curPC->depot[depotPos] = -1;
			reply = 8;

			GTH_SendMessage_ItemSystem_DepotGetItem( g_curPC, reply, error, depotPos );
			return;
		}
	}
	
	if( g_curPC->curItemWeight > g_curPC->calItemWeight )
	{
		
		reply = 0;
		error = ERROR_DEPOT_4;
		GTH_SendMessage_ItemSystem_DepotGetItem( g_curPC, reply, error, depotPos );
		return;
	}

	if( itemIdx < 0 && g_curPC->mouseInventory < 0 ) 
	{
		reply = 0;
	}
	else if( itemIdx >= 0 && g_curPC->mouseInventory < 0 )
	{
		if( GTH_IsStackItem( &g_curPC->item[itemIdx] ) && numStack > 0 )
		{
			reply = 5;
			GTH_GetStackItemForDepot( g_curPC, reply, error, depotPos, numStack );
		}
		else
		{
			reply = 1;
			g_curPC->depot[depotPos] = -1;
			g_curPC->mouseInventory = itemIdx;
		}
	} 
	else if( itemIdx < 0 && g_curPC->mouseInventory >= 0 )
	{
		int stackItemIdx = GTH_StackMyItemForDepot( g_curPC, &g_curPC->item[g_curPC->mouseInventory] );
		
		
		if( GTH_IsStackItem( &g_curPC->item[g_curPC->mouseInventory] ) && stackItemIdx >= 0 )
		{
			reply = 6;
			GTH_SetStackItemForDepot( g_curPC, reply, error );
		}
		else
		{
			reply = 2;
			g_curPC->depot[depotPos] = g_curPC->mouseInventory;
			g_curPC->mouseInventory = -1;
		}
	} 
	else if( itemIdx >= 0 && g_curPC->mouseInventory >= 0 )
	{
		
		if( GTH_IsStackItem( &g_curPC->item[g_curPC->mouseInventory] ) )
		{
			reply = 6;
			GTH_SetStackItemForDepot( g_curPC, reply, error );
		}
		else
		{
			reply = 3;
			g_curPC->depot[depotPos] = g_curPC->mouseInventory;
			g_curPC->mouseInventory = itemIdx;
		}
	}
	else 
	{
		reply = 0;
	}

	
	if( reply )
	{
		if( g_curPC->mouseInventory >= 0 ) g_curPC->mouseInventoryType = MOUSEINVEN_TYPE_DEPOT;
		else g_curPC->mouseInventoryType = MOUSEINVEN_TYPE_NONE;
	}


	GTH_SendMessage_ItemSystem_DepotGetItem( g_curPC, reply, error, depotPos );
}

void GTH_ProcessMessage_ItemSystem_DepotItem()
{
	int depotPos = MSG_ReadShort();
	int reply = 1, error = 0;
	int depotPrice;

	if( g_curPC->depot[depotPos] >= 0 || g_curPC->mouseInventory < 0 || 
		g_curPC->mouseInventoryType != MOUSEINVEN_TYPE_INVENTORY )
	{
		
		reply = 0;
		error = ERROR_DEPOT_2;
	}
	else
	{
		depotPrice = GTH_GetDepotPrice( g_curPC, &g_curPC->item[g_curPC->mouseInventory] );
		
		if( g_curPC->curChargeSE < depotPrice )
		{
			reply = 0;
			error = ERROR_DEPOT_3;
		}
		else
		{
			g_curPC->curChargeSE -= depotPrice;

			int stackItemIdx = GTH_StackMyItemForDepot( g_curPC, &g_curPC->item[g_curPC->mouseInventory] );
			
			
			if( GTH_IsStackItem( &g_curPC->item[g_curPC->mouseInventory] ) && stackItemIdx >= 0 )
			{
				reply = 2;
				GTH_SetStackItemForDepot( g_curPC, reply, error );
			}
			else
			{
				reply = 1;
				g_curPC->depot[depotPos] = g_curPC->mouseInventory;
				g_curPC->mouseInventory = -1;
			}
			g_curPC->mouseInventoryType = MOUSEINVEN_TYPE_NONE;
		}
	}

	GTH_SendMessage_ItemSystem_DepotItem( g_curPC, reply, error, depotPos );
}

void GTH_ProcessMessage_ItemSystem_DepotNak()
{
	int reply = 1, error = 0;
	int type;
	int depotSE;
	int tempSE;

	type = MSG_ReadByte();
	depotSE = MSG_ReadLong();
	
		
	if( type == 0 )
	{
		
		
		if( g_curPC->curChargeSE < depotSE )
		{
			reply = 0;
			error = 1;
			GTH_SendMessage_ItemSystem_DepotNak( g_curPC, reply, error );
			return;
		}

		if( g_curPC->depotSE + depotSE > MAX_OWN_NAK )
		{
			tempSE = g_curPC->depotSE + depotSE - MAX_OWN_NAK;
			g_curPC->depotSE = MAX_OWN_NAK;
			g_curPC->curChargeSE -= depotSE;
			g_curPC->curChargeSE += tempSE;
		}
		else
		{
			g_curPC->depotSE += depotSE;
			g_curPC->curChargeSE -= depotSE;
		}
	}
	
	else
	{
		
		
		if( g_curPC->depotSE < depotSE )
		{
			reply = 0;
			error = 1;
			GTH_SendMessage_ItemSystem_DepotNak( g_curPC, reply, error );
			return;
		}

		if( g_curPC->curChargeSE + depotSE > MAX_OWN_NAK )
		{
			tempSE = g_curPC->curChargeSE + depotSE - MAX_OWN_NAK;
			g_curPC->curChargeSE = MAX_OWN_NAK;
			g_curPC->depotSE -= depotSE;
			g_curPC->depotSE += tempSE;
		}
		else
		{
			g_curPC->depotSE -= depotSE;
			g_curPC->curChargeSE += depotSE;
		}
	}



	if( g_curPC->curChargeSE > MAX_OWN_NAK ) g_curPC->curChargeSE = MAX_OWN_NAK;
	if( g_curPC->curChargeSE < 0 ) g_curPC->curChargeSE = 0;
	if( g_curPC->depotSE > MAX_OWN_NAK ) g_curPC->depotSE = MAX_OWN_NAK;
	if( g_curPC->depotSE < 0 ) g_curPC->depotSE = 0;

	GTH_SendMessage_ItemSystem_DepotNak( g_curPC, reply, error );
}




void GTH_ProcessMessage_ItemSystem_PrecocityItem()
{
	int pos = MSG_ReadShort();
	int tempInven, itemTableIndex;
	int reply = 1, error = 0;

	if( g_curPC->mouseInventory >= 0 )
	{
		if( g_curPC->precocityInventory[pos] >= 0 )
		{
			tempInven = g_curPC->mouseInventory;
			g_curPC->mouseInventory = g_curPC->precocityInventory[pos];
			g_curPC->precocityInventory[pos] = tempInven;
			
			itemTableIndex = g_curPC->item[g_curPC->precocityInventory[pos]].itemTableIdx;
			if( g_itemTable[itemTableIndex].etcItemType == ETCITEM_TYPE_PRECOCITY )
			{
				
				if(gGameConfigFlag.m_bPrecocity_Time)
				{
					g_curPC->precocityTime[pos] = g_itemTable[itemTableIndex].precocity.precocityTime * g_curPC->PremiumInfo.fItemPrecocityTimeRatio;
				}
				else
				{
					g_curPC->precocityTime[pos] = g_itemTable[itemTableIndex].precocity.precocityTime;
				}
				
			}
			else
			{
				g_curPC->precocityTime[pos] = -1;
			}
		}
		else
		{
			g_curPC->precocityInventory[pos] = g_curPC->mouseInventory;
			g_curPC->mouseInventory = -1;
			
			itemTableIndex = g_curPC->item[g_curPC->precocityInventory[pos]].itemTableIdx;
			if( g_itemTable[itemTableIndex].etcItemType == ETCITEM_TYPE_PRECOCITY )
			{
				
				if(gGameConfigFlag.m_bPrecocity_Time)
				{
					g_curPC->precocityTime[pos] = g_itemTable[itemTableIndex].precocity.precocityTime * g_curPC->PremiumInfo.fItemPrecocityTimeRatio;
				}
				else
				{
					g_curPC->precocityTime[pos] = g_itemTable[itemTableIndex].precocity.precocityTime;
				}
				
			}
			else
			{
				g_curPC->precocityTime[pos] = -1;
			}
		}
	}
	else
	{
		g_curPC->mouseInventory = g_curPC->precocityInventory[pos];
		g_curPC->precocityInventory[pos] = -1;
		g_curPC->precocityTime[pos] = -1;
	}

	GTH_SendMessage_ItemSystem_PrecocityItem( g_curPC, reply, error, pos );
}






int GTH_AutoUndoPCCraft( playerCharacter_t *pc, int completeFlag )
{


	int itemIdx, inventoryPos;


	int bIsStack = false, stackNumber = 1;

	for( int i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
	{
		itemIdx = pc->npcAutoSourceInventory[i];
		
		if ( itemIdx < 0 || itemIdx >= MAX_NUMBER_OF_OWNITEM)
			continue;
		
		item_t* pItem = NULL;
		pItem = gcpTools->GetMyItem(pc, itemIdx);
		if ( NULL == pItem )
		{			
			continue;
		}
		
		if ( pItem->itemTableIdx < 0 || pItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
			continue;

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pItem->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pItem->durability + 1;
		}

		int stackItemInvenIdx = GTH_StackMyItem( pc, pItem );

		
		if( stackItemInvenIdx >= 0 )
		{
			
			item_t * pInvenItem = NULL;
			pInvenItem = gcpTools->GetMyItem(pc, stackItemInvenIdx);

			if (NULL == pInvenItem)
				continue;
			
			
			if ( pInvenItem->itemTableIdx <0 || pInvenItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
				continue;

			pInvenItem->durability += pItem->durability + 1;
			if( pInvenItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) 
				pInvenItem->durability = MAX_COUNT_STACK_ITEM - 1;
			
			
			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_CRAFT, 0, stackNumber, pInvenItem->name,
				GetItemInformationLog( bIsStack, pInvenItem->itemTableIdx, stackItemInvenIdx, -1 ));
			

			GTH_DeleteMyItem( pc, pItem );

			g_DBGameServer->SaveItem( pc, pInvenItem, stackItemInvenIdx );
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
				LOG_CLASS_ITEM_UNDO_CRAFT, 0, stackNumber, pItem->name,
				GetItemInformationLog( bIsStack, pItem->itemTableIdx, itemIdx, inventoryPos ));
			
		}
	}




	if( !completeFlag ) g_logSystem->Write("Make Item Cancel  : %s(%s)", pc->name, pc->userID );
	else g_logSystem->Write("Make Item Complete : %s(%s)", pc->name, pc->userID );
	
	
	for( i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
	{
		pc->npcAutoSourceInventory[i] = -1;
	}

	


	return 1;
}


int GTH_UndoPCCraft( playerCharacter_t *pc, int completeFlag )
{
	int itemIdx, inventoryPos;
	

	int bIsStack = false, stackNumber = 1;

	for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		itemIdx = pc->craftSourceInventory[i];
		
		if ( itemIdx < 0 || itemIdx >= MAX_NUMBER_OF_OWNITEM)
			continue;
		
		item_t* pItem = NULL;
		pItem = gcpTools->GetMyItem(pc, itemIdx);
		if ( NULL == pItem )
		{			
			continue;
		}
		
		if ( pItem->itemTableIdx < 0 || pItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
			continue;

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pItem->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pItem->durability + 1;
		}

		int stackItemInvenIdx = GTH_StackMyItem( pc, pItem );

		
		if( stackItemInvenIdx >= 0 )
		{
			
			item_t * pInvenItem = NULL;
			pInvenItem = gcpTools->GetMyItem(pc, stackItemInvenIdx);

			if (NULL == pInvenItem)
				continue;
			
			
			if ( pInvenItem->itemTableIdx <0 || pInvenItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
				continue;

			pInvenItem->durability += pItem->durability + 1;
			if( pInvenItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) 
				pInvenItem->durability = MAX_COUNT_STACK_ITEM - 1;
			
			
			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_CRAFT, 0, stackNumber, pInvenItem->name,
				GetItemInformationLog( bIsStack, pInvenItem->itemTableIdx, stackItemInvenIdx, -1 ));
			

			GTH_DeleteMyItem( pc, pItem );

			g_DBGameServer->SaveItem( pc, pInvenItem, stackItemInvenIdx );
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
				LOG_CLASS_ITEM_UNDO_CRAFT, 0, stackNumber, pItem->name,
				GetItemInformationLog( bIsStack, pItem->itemTableIdx, itemIdx, inventoryPos ));
			
		}
	}

	for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
	{
		itemIdx = pc->craftResultInventory[i];

		if ( itemIdx < 0 || itemIdx >= MAX_NUMBER_OF_OWNITEM)
			continue;
		
		item_t* pItem = NULL;
		pItem = gcpTools->GetMyItem(pc, itemIdx);
		if ( NULL == pItem )
		{			
			continue;
		}
		
		if ( pItem->itemTableIdx < 0 || pItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
			continue;

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pItem->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pItem->durability + 1;
		}

		int stackItemInvenIdx = GTH_StackMyItem( pc, pItem );

		
		if( stackItemInvenIdx >= 0 )
		{
			
			item_t * pInvenItem = NULL;
			pInvenItem = gcpTools->GetMyItem(pc, stackItemInvenIdx);

			if (NULL == pInvenItem)
				continue;
			
			
			if ( pInvenItem->itemTableIdx <0 || pInvenItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
				continue;

			pInvenItem->durability += pItem->durability + 1;
			if( pInvenItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) 
				pInvenItem->durability = MAX_COUNT_STACK_ITEM - 1;
			

			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_CRAFT, 1, stackNumber, pInvenItem->name,
				GetItemInformationLog( bIsStack, pInvenItem->itemTableIdx, stackItemInvenIdx, -1 ));
			

			GTH_DeleteMyItem( pc, pItem );

			g_DBGameServer->SaveItem( pc, pInvenItem, stackItemInvenIdx );
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
				LOG_CLASS_ITEM_UNDO_CRAFT, 1, stackNumber, pItem->name,
				GetItemInformationLog( bIsStack, pItem->itemTableIdx, itemIdx, inventoryPos ));
			
		}
	}

	if( !completeFlag ) g_logSystem->Write("Make Item Cancel  : %s(%s)", pc->name, pc->userID );
	else g_logSystem->Write("Make Item Complete : %s(%s)", pc->name, pc->userID );
	
	
	for( i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		pc->craftSourceInventory[i] = -1;
	}
	for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
	{
		pc->craftResultInventory[i] = -1;
	}

	


	return 1;
}

void GTH_ProcessMessage_ItemSystem_CraftClose()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;

	GTH_UndoPCCraft( g_curPC );
	GTH_DeleteNoticePC( g_curPC );

	GTH_SendMessage_ItemSystem_CraftClose( g_curPC );
}

void GTH_ProcessMessage_ItemSystem_CraftOpen()
{
	int i;
	
	
	for( i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		g_curPC->craftSourceInventory[i] = -1;
	}
	for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
	{
		g_curPC->craftResultInventory[i] = -1;
	}
	
	GTH_AddNoticePC( g_curPC, -1, -1, BUSY_STATE_CRAFT );

	GTH_SendMessage_ItemSystem_CraftOpen( g_curPC );
}






void GTH_ProcessMessage_ItemSystem_NpcInventoryOpen()
{
	int i;
	
	

	for( i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
	{
		g_curPC->npcAutoSourceInventory[i] = -1;
	}
	
	GTH_AddNoticePC( g_curPC, -1, -1, BUSY_STATE_NPC_INVENTORY );
	GTH_SendMessage_ItemSystem_NpcInventoryOpen( g_curPC );
}


void GTH_ProcessMessage_ItemSystem_NpcInventoryClose()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_NPC_INVENTORY)) 
		return;

	GTH_AutoUndoPCCraft( g_curPC ); 

	GTH_DeleteNoticePC( g_curPC );

	GTH_SendMessage_ItemSystem_NpcInventoryClose( g_curPC );
}


void GTH_ProcessMessage_ItemSystem_AutoCraftItem()
{
 	int posType = MSG_ReadShort();
	int pos = MSG_ReadShort();

	int tempInven;
	int reply = 1, error = 0;

	
	if( posType == 0 )
	{
		if( g_curPC->mouseInventory >= 0 )
		{
			if( g_curPC->npcAutoSourceInventory[pos] >= 0 )
			{
				tempInven = g_curPC->mouseInventory;
				g_curPC->mouseInventory = g_curPC->npcAutoSourceInventory[pos];
				g_curPC->npcAutoSourceInventory[pos] = tempInven;
			}
			else
			{
				g_curPC->npcAutoSourceInventory[pos] = g_curPC->mouseInventory;
				g_curPC->mouseInventory = -1;
			}
		}
		else
		{
			g_curPC->mouseInventory = g_curPC->npcAutoSourceInventory[pos];
			g_curPC->npcAutoSourceInventory[pos] = -1;
		}
	}
	GTH_SendMessage_ItemSystem_AutoQuestCraftItem( g_curPC, reply, error, posType, pos );
}


void GTH_ProcessMessage_ItemSystem_CraftItem()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;

	int posType = MSG_ReadShort();
	int pos = MSG_ReadShort();

	
	int craftIdx = MSG_ReadShort();

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

	GTH_SendMessage_ItemSystem_CraftItem( g_curPC, reply, error, posType, pos );
}



void GTH_ProcessMessage_ItemSystem_CraftPrepare()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;
	
	int reply = 1, error = 0,craftIdx=-1;
	int nak = 0, resultItemIdx[2];
	float pro = 0.0f;

	resultItemIdx[0] = -1;
	resultItemIdx[1] = -1;
		
	GTH_CraftProcess( g_curPC, reply, error, nak, pro, resultItemIdx, true ,craftIdx);
	GTH_SendMessage_ItemSystem_CraftPrepare( g_curPC, reply, error, resultItemIdx, nak, pro,craftIdx);
}


void GTH_ProcessMessage_ItemSystem_AutoCraftPrepare()
{
	int reply = 1, error = 0;
	int idx=-1;
   
    for(int i=0; i<MAX_COUNT_QUEST_STATUS; i++)
	{
		if(g_curPC->questStatus[i].questIdx == AUTOQUESTSCROLL_ITEM_INDEX)
		{
			idx=i;
			break;
		}	
		
	}
	
	
	
	GTH_AutoCraftProcess( g_curPC, idx,reply, error);
   
	
	GTH_SendMessage_ItemSystem_AutoCraftPrepare( g_curPC, reply, error);

	
	if(reply)
	{

		
		int &fame=g_autoQuestscrollTable[g_curPC->questStatus[idx].dField[0]].dataField[18];
		if(fame == -1) fame=0;
		g_curPC->rankPoint+=fame;
		
		
		int pParam[2];
		pParam[0]=AUTOQUESTSCROLL_ITEM_INDEX;
		pParam[1]=g_curPC->questStatus[idx].dField[0];
		AI_NPC_AutoCompleteQuest(g_curPC, pParam );
		
		
		g_curPC->bautoeffectisvalid=true; 
		
		
		int addItemData2[4];
		addItemData2[0] = g_autoQuestscrollTable[g_curPC->questStatus[idx].dField[0]].dataField[15];			
		addItemData2[1] = -1;								
		addItemData2[2] = g_autoQuestscrollTable[g_curPC->questStatus[idx].dField[0]].dataField[16];								
		addItemData2[3] = 0;
		AI_NPC_AddItem( g_curPC, addItemData2 );
		int itemIdx=-1;
		int bIsStack, stackNumber;
		
		for( i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
		{
			if( g_curPC->npcAutoSourceInventory[i] < 0 ) continue;
			itemIdx = g_curPC->npcAutoSourceInventory[i];
			
			bIsStack = false;
			stackNumber = 1;
			
			if( g_itemTable[g_curPC->item[itemIdx].itemTableIdx].stackFlag )
			{
				bIsStack = true;
				stackNumber = g_curPC->item[itemIdx].durability + 1;
			}
			
			
			
			
			g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
				LOG_CLASS_ITEM_CRAFT_SOURCE, stackNumber, g_curPC->item[itemIdx].name,
				GetItemInformationLog( bIsStack, g_curPC->item[itemIdx].itemTableIdx, itemIdx, -1 ));
			
			
			GTH_DeleteMyItem( g_curPC, &g_curPC->item[itemIdx] );
			g_curPC->npcAutoSourceInventory[i] = -1;
			
			g_DBGameServer->RemoveItem( g_curPC, itemIdx );
		}
		
		GTH_SendMessage_AutoQuestScrollComplete( g_curPC,  g_curPC->questStatus[idx].dField[0]);
	}
}



void GTH_ProcessMessage_ItemSystem_CraftComplete()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_CRAFT ) )
		return;

	int reply = 1, error = 0;
	int nak = 0, resultItemIdx[2],craftIdx;
	float pro = 0.0f;

	int bIsSuccess;
	char logBuffer[20];

	resultItemIdx[0] = -1;
	resultItemIdx[1] = -1;

	bIsSuccess = GTH_CraftProcess( g_curPC, reply, error, nak, pro, resultItemIdx, false,craftIdx );

	
	
	
	_snprintf( logBuffer, 20, "%.5f", pro );
	g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;", 
		LOG_CLASS_ITEM_COMPLETE_CRAFT, bIsSuccess, nak, logBuffer );
	


	GTH_SendMessage_ItemSystem_CraftComplete( g_curPC, reply, error, nak );
}




int GTH_UndoPCBooth( playerCharacter_t *pc )
{
	int itemIdx, inventoryPos;
	
	int bIsStack = false, stackNumber = 1;

	for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
	{
		itemIdx = pc->boothInventory[i].itemIdx;		

		if ( itemIdx < 0 || itemIdx >= MAX_NUMBER_OF_OWNITEM)
			continue;
		
		item_t* pItem = NULL;
		pItem = gcpTools->GetMyItem(pc, itemIdx);
		if ( NULL == pItem )
		{			
			continue;
		}
		
		if ( pItem->itemTableIdx < 0 || pItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
			continue;

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pItem->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pItem->durability + 1;
		}

		int stackItemInvenIdx = GTH_StackMyItem( pc, pItem );

		
		if( stackItemInvenIdx >= 0 )
		{
			
			item_t * pInvenItem = NULL;
			pInvenItem = gcpTools->GetMyItem(pc, stackItemInvenIdx);

			if (NULL == pInvenItem)
				continue;
			
			
			if ( pInvenItem->itemTableIdx <0 || pInvenItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
				continue;

			pInvenItem->durability += pItem->durability + 1;
			if( pInvenItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) 
				pInvenItem->durability = MAX_COUNT_STACK_ITEM - 1;

			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_BOOTH, stackNumber, pInvenItem->name,
				GetItemInformationLog( bIsStack, pInvenItem->itemTableIdx, stackItemInvenIdx, -1 ));
			

			GTH_DeleteMyItem( pc, pItem );

			g_DBGameServer->SaveItem( pc, pInvenItem, stackItemInvenIdx );
			g_DBGameServer->RemoveItem( pc, itemIdx );
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos( pc );
			if( inventoryPos >= 0 ) 
			{
				pc->inventory[inventoryPos] = itemIdx;
			}
			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_BOOTH, stackNumber, pItem->name,
				GetItemInformationLog( bIsStack, pItem->itemTableIdx, itemIdx, inventoryPos ));
			
		}
	}

	g_logSystem->Write("Shop Close : %s(%s)", pc->name, pc->userID );
	
	
	for( i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
	{
		pc->boothInventory[i].itemIdx = -1;
	}

	


	return 1;
}


void GTH_ProcessMessage_ItemSystem_BoothSellOpen()
{
	int i;
	int reply = 1, error = 0;

	
	
	

	
	
	if ( g_curPC->event != GTH_EV_CHAR_SIT )
	{
		reply = 0;
		error = 0;
		GTH_SendMessage_ItemSystem_BoothSellOpen( g_curPC, reply, error );
		return;
	}

	if( g_curPC->busyState != BUSY_STATE_NONE || !g_curPC->alive  || !g_curPC->ready )
	{
		reply = 0;
		error = 0;
		GTH_SendMessage_ItemSystem_BoothSellOpen( g_curPC, reply, error );
		return;
	}

	
	for( i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
	{
		g_curPC->boothInventory[i].itemIdx = -1;

		
		g_curPC->boothInventory[i].price = 0;
	}
	
	for( i = 0; i < MAX_COUNT_BOOTH_BUYER; i ++ )
	{
		g_curPC->boothBuyerIdx[i] = -1;
	}

	
	GTH_AddNoticePC( g_curPC, -1, -1, BUSY_STATE_BOOTH_SELL_PREPARE );	
	GTH_SendMessage_ItemSystem_BoothSellOpen( g_curPC, reply, error );

	
	g_logSystem->Write("Shop Open : %s(%s)", g_curPC->name, g_curPC->userID );
}




#include "../Tools/CTools.h"
extern CTools* gcpTools;

void GTH_ProcessMessage_ItemSystem_BoothSellItem()
{
	int Invenpos	= MSG_ReadShort();
	int nak			= MSG_ReadLong();
	int tempInven;

	if ( Invenpos < 0 || Invenpos >= MAX_BOOTH_INVENTORY_SIZE)
	{
		GTH_SendMessage_ItemSystem_BoothSellItem( g_curPC, 0, 0, 0 );
		return;
	}

	if ( g_curPC->busyState != BUSY_STATE_BOOTH_SELL_PREPARE  )
	{
		GTH_SendMessage_ItemSystem_BoothSellItem( g_curPC, 0, 0, 0 );
		return;
	}
	
	







	

















	
	item_t* pMouseInItem = NULL;	
	pMouseInItem = gcpTools->GetMyItem(g_curPC, g_curPC->mouseInventory);

	if ( pMouseInItem != NULL)	
	{		
		int totalSellPrice = 0;
		for (int BoothInvenIdx = 0; BoothInvenIdx < MAX_BOOTH_INVENTORY_SIZE; BoothInvenIdx ++)	
		{
			if ( BoothInvenIdx == Invenpos ) continue;
			totalSellPrice += g_curPC->boothInventory[BoothInvenIdx].price;
		}

		
		if( GTH_IsStackItem( pMouseInItem ) )		
			totalSellPrice += (pMouseInItem->durability + 1 ) * nak;
		else
			totalSellPrice += nak;

		if (g_curPC->curChargeSE + totalSellPrice > MAX_OWN_NAK)
		{
			GTH_SendMessage_ItemSystem_BoothSellItem( g_curPC, 0, 0, 1 );
			return;
		}

		if( g_curPC->boothInventory[Invenpos].itemIdx >= 0 )
		{
			tempInven = g_curPC->mouseInventory;
			g_curPC->mouseInventory = g_curPC->boothInventory[Invenpos].itemIdx;
			g_curPC->boothInventory[Invenpos].itemIdx = tempInven;
			g_curPC->boothInventory[Invenpos].price = nak;
		}
		else
		{
			g_curPC->boothInventory[Invenpos].itemIdx = g_curPC->mouseInventory;
			g_curPC->boothInventory[Invenpos].price = nak;
			g_curPC->mouseInventory = -1;
		}
	}
	else
	{
		g_curPC->mouseInventory = g_curPC->boothInventory[Invenpos].itemIdx;
		g_curPC->boothInventory[Invenpos].itemIdx = -1;

		
		g_curPC->boothInventory[Invenpos].price = 0;
	}
	

	GTH_SendMessage_ItemSystem_BoothSellItem( g_curPC, 1, Invenpos );
}


void GTH_ProcessMessage_ItemSystem_BoothSellStart()
{
	strcpy( g_curPC->boothName, MSG_ReadString() );
	strcpy( g_curPC->boothInfo, MSG_ReadString() );
	
	
	GTH_AddNoticePC( g_curPC, -1, -1, BUSY_STATE_BOOTH_SELL );	
	GTH_SendMessage_ItemSystem_BoothSellStart( g_curPC );

	
	g_logSystem->Write("Shop Sell Start : %s(%s)", g_curPC->name, g_curPC->userID );
}


void GTH_ProcessMessage_ItemSystem_BoothSellClose()
{
	
	if( g_curPC->busyState == BUSY_STATE_BOOTH_SELL_PREPARE )
	{
		GTH_UndoPCBooth( g_curPC );
		GTH_DeleteNoticePC( g_curPC );
		
		GTH_SendMessage_ItemSystem_BoothSellClose( g_curPC, 0 );
	}
	
	else if( g_curPC->busyState == BUSY_STATE_BOOTH_SELL )
	{
		int pcIdx;
		for( int i = 0; i < MAX_COUNT_BOOTH_BUYER; i ++ )
		{
			if( g_curPC->boothBuyerIdx[i] < 0 ) continue;
			pcIdx = g_curPC->boothBuyerIdx[i];
		
			
			
			if( g_pc[pcIdx].busyState == BUSY_STATE_BOOTH_BUY ) 
			{
				GTH_SendMessage_ItemSystem_BoothBuyClose( &g_pc[pcIdx], 1, 0, 0 );
				GTH_DeleteNoticePC( &g_pc[pcIdx] );
			}
			else
			{
				g_logSystem->Write("Shop DeleteBusyState Error : %s(%s) Cur Status(%d)", g_pc[pcIdx].name, g_pc[pcIdx].userID, g_pc[pcIdx].busyState );
				
				g_logSystem->WriteToLog( &g_pc[pcIdx], LOG_CLASS_ITEM, "%d;;;;;%s;%s", g_pc[pcIdx].busyState, "Shop Error", "ProcessSellClose" );
			}

			g_curPC->boothBuyerIdx[i] = -1;
		}

		GTH_AddNoticePC( g_curPC, -1, -1, BUSY_STATE_BOOTH_SELL_PREPARE );
		GTH_SendMessage_ItemSystem_BoothSellClose( g_curPC, 1 );
	}
}


void GTH_ProcessMessage_ItemSystem_BoothBuyOpen()
{
	int boothPCIdx = MSG_ReadShort();
	int reply = 1, error = 0;
	int boothBuyerIdx;

	
	if( boothPCIdx < 0 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_6;
		GTH_SendMessage_ItemSystem_BoothBuyOpen( g_curPC, reply, error, NULL, 0 );
		return;
	}
	
	if( g_pc[boothPCIdx].busyState != BUSY_STATE_BOOTH_SELL )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_4;
		GTH_SendMessage_ItemSystem_BoothBuyOpen( g_curPC, reply, error, &g_pc[boothPCIdx], 0 );
		return;
	}

	
	for( int i = 0 ; i < MAX_COUNT_BOOTH_BUYER; i ++ )
	{
		if( g_pc[boothPCIdx].boothBuyerIdx[i] < 0 ) break;
	}
	
	if( i == MAX_COUNT_BOOTH_BUYER ) 
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_5;
		GTH_SendMessage_ItemSystem_BoothBuyOpen( g_curPC, reply, error, &g_pc[boothPCIdx], 0 );
		return;
	}
	boothBuyerIdx = i;
	
	GTH_AddNoticePC( g_curPC, ENTITY_PC, boothPCIdx, BUSY_STATE_BOOTH_BUY );
	
	g_pc[boothPCIdx].boothBuyerIdx[boothBuyerIdx] = g_curPC->idx;
	
	
	GTH_SendMessage_ItemSystem_BoothBuyOpen( g_curPC, reply, error, &g_pc[boothPCIdx], 0 );
	
	GTH_SendMessage_ItemSystem_BoothBuyOpen( &g_pc[boothPCIdx], reply, error, g_curPC, 1 );
}

void GTH_ProcessMessage_ItemSystem_BoothBuyItem()
{
	int pos, reply = 1, error = 0;
	int itemCount;
	int boothPCIdx = g_curPC->friendlyEntityIdx;
	int itemIdx, price, itemTableIdx, inventoryPos, buyItemIdx;
	item_t buyItem, *upItem;
	char generateID[20];

	int bIsStack = false, stackNumber = 1;
	char logBuffer[100];

	pos = MSG_ReadShort();
	itemCount = MSG_ReadShort();

	
	if( boothPCIdx < 0 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_6;
		GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, 0, 0 );
		return;
	}
	
	if( g_pc[boothPCIdx].busyState != BUSY_STATE_BOOTH_SELL )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_4;
		GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, 0, 0 );
		return;
	}

	itemIdx = g_pc[boothPCIdx].boothInventory[pos].itemIdx;
	
	if( itemIdx < 0 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_10;
		GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, 0, 0 );
		return;
	}
	
	if( itemCount > g_pc[boothPCIdx].item[itemIdx].durability + 1 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_8;
		GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, 0, 0 );
		return;
	}
	//lucky 2012 Dupe metod  #1 patched.
	if( itemCount < 0 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_8;
		GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, 0, 0 );
		return;
	}
	//end
	
	price = g_pc[boothPCIdx].boothInventory[pos].price * itemCount;
	if( price > g_curPC->curChargeSE )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_7;
		GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, 0, 0 );
		return;
	}
	
	inventoryPos = GTH_FindEmptyInventoryPos( g_curPC );
	if( inventoryPos < 0 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_9;
		GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, 0, 0 );
		return;
	}
	
	if( g_curPC->curItemWeight > g_curPC->calItemWeight )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_11;
		GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, 0, 0 );
		return;
	}

	
	memcpy( &buyItem, &g_pc[boothPCIdx].item[itemIdx], sizeof( item_t ) );
	itemTableIdx = buyItem.itemTableIdx;

	if( g_itemTable[itemTableIdx].stackFlag )
	{
		bIsStack = true;
		stackNumber = itemCount;
	}

	
	
	
	_snprintf( logBuffer, 100, "%s, %s", g_curPC->name, 
		GetItemInformationLog( bIsStack, g_pc[boothPCIdx].item[itemIdx].itemTableIdx, itemIdx, -1 ) );
	g_logSystem->WriteToLog( &g_pc[boothPCIdx], LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
		LOG_CLASS_ITEM_SELL_BOOTH, stackNumber, price,
		g_pc[boothPCIdx].item[itemIdx].name, logBuffer );
	

	
	if( g_itemTable[itemTableIdx].stackFlag )
	{
		bIsStack = true;
		stackNumber = itemCount;

		
		if( g_pc[boothPCIdx].item[itemIdx].durability + 1 - itemCount <= 0 )
		{
			g_pc[boothPCIdx].boothInventory[pos].itemIdx = -1;
			GTH_DeleteMyItem( &g_pc[boothPCIdx], &g_pc[boothPCIdx].item[itemIdx] );
			g_DBGameServer->RemoveItem( &g_pc[boothPCIdx], itemIdx );
		}
		
		else
		{
			g_pc[boothPCIdx].item[itemIdx].durability -= itemCount;
			g_DBGameServer->SaveItem( &g_pc[boothPCIdx], &g_pc[boothPCIdx].item[itemIdx], itemIdx );
		}
		buyItem.durability = itemCount - 1;
	}
	else
	{
		g_pc[boothPCIdx].boothInventory[pos].itemIdx = -1;
		GTH_DeleteMyItem( &g_pc[boothPCIdx], &g_pc[boothPCIdx].item[itemIdx] );
		g_DBGameServer->RemoveItem( &g_pc[boothPCIdx], itemIdx );
	}

	
	int stackItemIdx = GTH_StackMyItem( g_curPC, &buyItem );
	if( stackItemIdx >= 0 )
	{
		upItem = &g_curPC->item[stackItemIdx];
		upItem->durability += buyItem.durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;
		
		
		
		
		_snprintf( logBuffer, 100, "%s, %s", g_pc[boothPCIdx].name, 
			GetItemInformationLog( bIsStack, upItem->itemTableIdx, stackItemIdx, -1 ) );
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_BUY_BOOTH, stackNumber, price,
			upItem->name, logBuffer );
		
			
		g_DBGameServer->SaveItem( g_curPC, upItem, stackItemIdx );
	}
	else
	{
		buyItemIdx = GTH_AddMyItem( g_curPC, &buyItem );
		inventoryPos = GTH_FindEmptyInventoryPos( g_curPC );
		if( inventoryPos >= 0 ) 
		{
			g_curPC->inventory[inventoryPos] = buyItemIdx;
		}

		
		
		
		_snprintf( logBuffer, 100, "%s, %s", g_pc[boothPCIdx].name, 
			GetItemInformationLog( bIsStack, buyItem.itemTableIdx, buyItemIdx, inventoryPos ) );
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_BUY_BOOTH, stackNumber, price,
			buyItem.name, logBuffer );
		
		
		strcpy( generateID, "boothItem" );
		g_DBGameServer->AddItem( g_curPC, &buyItem, buyItemIdx, generateID );
	}


	g_pc[boothPCIdx].curChargeSE += price;
	
	
	if( g_pc[boothPCIdx].curChargeSE > MAX_OWN_NAK ) g_pc[boothPCIdx].curChargeSE = MAX_OWN_NAK;
	

	g_curPC->curChargeSE -= price;

	GTH_SendMessage_ItemSystem_BoothBuyItem( g_curPC, reply, error, pos, itemCount, 0 );
	GTH_SendMessage_ItemSystem_BoothBuyItem( &g_pc[boothPCIdx], reply, error, pos, itemCount, 1 );

	
	
	int pcIdx;
	for( int i = 0; i < MAX_COUNT_BOOTH_BUYER; i ++ )
	{
		if( g_pc[boothPCIdx].boothBuyerIdx[i] < 0 ) continue;
		
		
		pcIdx = g_pc[boothPCIdx].boothBuyerIdx[i];
		if( g_pc[pcIdx].busyState == BUSY_STATE_BOOTH_BUY )
		{
			GTH_SendMessage_ItemSystem_BoothBuyUpdate( &g_pc[pcIdx], &g_pc[boothPCIdx] );
		}
		else
		{
			g_logSystem->Write("Shop Update Error : %s(%s) Status(%d)", g_pc[pcIdx].name, g_pc[pcIdx].userID, g_pc[pcIdx].busyState );
			
			g_logSystem->WriteToLog( &g_pc[pcIdx], LOG_CLASS_ITEM, "%d;;;;;%s;%s", g_pc[pcIdx].busyState, "Shop Error", "Update" );
		}
	}
}

void GTH_ProcessMessage_ItemSystem_BoothBuyClose()
{
	int boothPCIdx = g_curPC->friendlyEntityIdx;
	int reply = 1, error = 0;

	
	if( boothPCIdx < 0 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_6;
		GTH_SendMessage_ItemSystem_BoothBuyClose( g_curPC, reply, error, 0 );
		return;
	}

	
	
	for( int i = 0; i < MAX_COUNT_BOOTH_BUYER; i ++ )
	{
		if( g_pc[boothPCIdx].boothBuyerIdx[i] < 0 ) continue;
		if( g_pc[boothPCIdx].boothBuyerIdx[i] == g_curPC->idx )
		{
			g_pc[boothPCIdx].boothBuyerIdx[i] = -1;
			break;
		}
	}

	GTH_DeleteNoticePC( g_curPC );
	
	
	GTH_SendMessage_ItemSystem_BoothBuyClose( g_curPC, reply, error, 0 );
	GTH_SendMessage_ItemSystem_BoothBuyClose( &g_pc[boothPCIdx], reply, error, 1, g_curPC->name );
}






int GTH_UndoPCUpgrade( playerCharacter_t *pc, int completeFlag )
{
	
	if ( FALSE == isValidPCBusy(pc, BUSY_STATE_UPGRADE))
		return FALSE;

	int itemIdx, inventoryPos;
	

	int bIsStack, stackNumber;

	for( int i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		itemIdx = pc->upgradeSourceInventory[i];
		
		if ( itemIdx < 0 || itemIdx >= MAX_NUMBER_OF_OWNITEM)
			continue;
		
		item_t* pItem = NULL;
		pItem = gcpTools->GetMyItem(pc, itemIdx);
		if ( NULL == pItem )
		{			
			continue;
		}
		
		if ( pItem->itemTableIdx < 0 || pItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
			continue;

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pItem->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pItem->durability + 1;
		}

		int stackItemInvenIdx = GTH_StackMyItem( pc, pItem );

		
		if( stackItemInvenIdx >= 0 )
		{
			
			item_t * pInvenItem = NULL;
			pInvenItem = gcpTools->GetMyItem(pc, stackItemInvenIdx);

			if (NULL == pInvenItem)
				continue;
			
			
			if ( pInvenItem->itemTableIdx <0 || pInvenItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
				continue;

			pInvenItem->durability += pItem->durability + 1;
			if( pInvenItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) 
				pInvenItem->durability = MAX_COUNT_STACK_ITEM - 1;
			
			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_UPGRADE, 0, stackNumber, pInvenItem->name,
				GetItemInformationLog( bIsStack, pInvenItem->itemTableIdx, stackItemInvenIdx, -1 ));
			

			GTH_DeleteMyItem( pc, pItem );

			g_DBGameServer->SaveItem( pc, pInvenItem, stackItemInvenIdx);
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
				LOG_CLASS_ITEM_UNDO_UPGRADE, 0, stackNumber, pItem->name,
				GetItemInformationLog( bIsStack, pItem->itemTableIdx, itemIdx, inventoryPos ));
			
		}
	}

	itemIdx = pc->upgradeResultInventory;
	if( itemIdx >= 0 || itemIdx < MAX_NUMBER_OF_OWNITEM )
	{
		item_t* pItem = NULL;
		pItem = gcpTools->GetMyItem(pc, itemIdx);
		if ( NULL == pItem )
		{	
			goto UPGRADE_OUT;

			
		}
		
		if ( pItem->itemTableIdx < 0 || pItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
			goto UPGRADE_OUT;

		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pItem->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pItem->durability + 1;
		}

		int stackItemInvenIdx = GTH_StackMyItem( pc, pItem );

		
		if( stackItemInvenIdx >= 0 )
		{
			
			item_t * pInvenItem = NULL;
			pInvenItem = gcpTools->GetMyItem(pc, stackItemInvenIdx);

			if (NULL == pInvenItem)
				goto UPGRADE_OUT;
			
			
			if ( pInvenItem->itemTableIdx <0 || pInvenItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
				goto UPGRADE_OUT;

			pInvenItem->durability += pItem->durability + 1;
			if( pInvenItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) 
				pInvenItem->durability = MAX_COUNT_STACK_ITEM - 1;

			
			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
				LOG_CLASS_ITEM_UNDO_UPGRADE, 1, stackNumber, pInvenItem->name,
				GetItemInformationLog( bIsStack, pInvenItem->itemTableIdx, stackItemInvenIdx, -1 ));
			
			
			GTH_DeleteMyItem( pc, pItem );
			
			g_DBGameServer->SaveItem( pc, pInvenItem, stackItemInvenIdx );
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
				LOG_CLASS_ITEM_UNDO_UPGRADE, 1, stackNumber, pItem->name,
				GetItemInformationLog( bIsStack, pItem->itemTableIdx, itemIdx, inventoryPos ));
			
		}
	}


UPGRADE_OUT:
	if( !completeFlag ) g_logSystem->Write("Item Upgrade Cancel : %s(%s)", pc->name, pc->userID );
	else g_logSystem->Write("Item Upgrade Complete : %s(%s)", pc->name, pc->userID );
	
	
	for( i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		pc->upgradeSourceInventory[i] = -1;
	}
	pc->upgradeResultInventory = -1;

	


	return 1;
}

void GTH_ProcessMessage_ItemSystem_UpgradeClose()
{
	GTH_UndoPCUpgrade( g_curPC );
	GTH_DeleteNoticePC( g_curPC );

	GTH_SendMessage_ItemSystem_UpgradeClose( g_curPC );
}

void GTH_ProcessMessage_ItemSystem_UpgradeOpen()
{
	int i;
	
	
	for( i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		g_curPC->upgradeSourceInventory[i] = -1;
	}
	g_curPC->upgradeResultInventory = -1;
	
	GTH_AddNoticePC( g_curPC, -1, -1, BUSY_STATE_UPGRADE );

	GTH_SendMessage_ItemSystem_UpgradeOpen( g_curPC );
}

void GTH_ProcessMessage_ItemSystem_UpgradeItem()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_UPGRADE))
		return;

	int posType = MSG_ReadShort();
	int pos = MSG_ReadShort();

	int tempInven;
	int reply = 1, error = 0;

	
	if( posType == 0 )
	{
		if( g_curPC->mouseInventory >= 0 )
		{
			if( g_curPC->upgradeSourceInventory[pos] >= 0 )
			{
				tempInven = g_curPC->mouseInventory;
				g_curPC->mouseInventory = g_curPC->upgradeSourceInventory[pos];
				g_curPC->upgradeSourceInventory[pos] = tempInven;
			}
			else
			{
				g_curPC->upgradeSourceInventory[pos] = g_curPC->mouseInventory;
				g_curPC->mouseInventory = -1;
			}
		}
		else
		{
			g_curPC->mouseInventory = g_curPC->upgradeSourceInventory[pos];
			g_curPC->upgradeSourceInventory[pos] = -1;
		}
	}
	else
	{
		if( g_curPC->mouseInventory < 0 && g_curPC->upgradeResultInventory >= 0 )
		{
			g_curPC->mouseInventory = g_curPC->upgradeResultInventory;
			g_curPC->upgradeResultInventory = -1;
		}
	}

	GTH_SendMessage_ItemSystem_UpgradeItem( g_curPC, reply, error, posType, pos );
}

void GTH_ProcessMessage_ItemSystem_UpgradePrepare()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_UPGRADE))
		return;

	int reply = 1, error = 0;
	int nak = 0, resultItemIdx = 0;
	int currentGrade = 0, nextGrade = 0;
	float pro = 0.0f, vanishPro = 0.0f, downGradePro = 0.0f;
	int optionIdx = 0;

	resultItemIdx = -1;

	
	optionIdx = MSG_ReadByte();
	GTH_UpgradeProcess( g_curPC, optionIdx, reply, error, nak, pro, vanishPro, downGradePro, currentGrade, nextGrade, resultItemIdx, true );

	GTH_SendMessage_ItemSystem_UpgradePrepare( g_curPC, reply, error, resultItemIdx, nak, pro, vanishPro, downGradePro, currentGrade, nextGrade, optionIdx );
}

void GTH_ProcessMessage_ItemSystem_UpgradeComplete()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_UPGRADE))
		return;

	int reply = 1, error = 0;
	int nak = 0, resultItemIdx, success;
	int currentGrade = 0, nextGrade = 0;
	float pro = 0.0f, vanishPro = 0.0f, downGradePro = 0.0f;
	int optionIdx = 0;

	char logBuffer[20];

	resultItemIdx = -1;

	
	optionIdx = MSG_ReadByte();
	success = GTH_UpgradeProcess( g_curPC, optionIdx, reply, error, nak, pro, vanishPro, downGradePro, currentGrade, nextGrade, resultItemIdx, false );

	
	
	
	_snprintf( logBuffer, 20, "%.5f", pro );
	g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;", 
		LOG_CLASS_ITEM_COMPLETE_UPGRADE, success, nak, logBuffer );
	

	GTH_SendMessage_ItemSystem_UpgradeComplete( g_curPC, reply, error, nak, success, resultItemIdx, optionIdx, nextGrade );

}


void GTH_ProcessMessage_ItemSystem_DeletePrepare()
{	

	int reply = 1, error = 0;
	int nak = 0, resultItemIdx = 0;
	int currentGrade = 0, nextGrade = 0;
	float pro = 0.0f, vanishPro = 0.0f;

	resultItemIdx = -1;
	
	int optionIdx = MSG_ReadByte();
	GTH_DeleteProcess( g_curPC, reply, error, optionIdx, true );
	GTH_SendMessage_ItemSystem_DeletePrepare( g_curPC, reply, error, optionIdx );
}

void GTH_ProcessMessage_ItemSystem_DeleteComplete()
{
	int reply = 1, error = 0;
	int nak = 0, resultItemIdx, success;

	resultItemIdx = -1;

	int optionIdx = MSG_ReadByte();
	success = GTH_DeleteProcess( g_curPC, reply, error, optionIdx, false );
	GTH_SendMessage_ItemSystem_DeleteComplete( g_curPC, reply, error, success, optionIdx );
}








void GTH_ProcessMessage_Friends (playerCharacter_t* pPlayer)
{
	switch ( MSG_ReadByte () )
	{
	case FRIENDS_INVITE:
		GTH_ProcessMessage_Friends_Invite (pPlayer) ;
		break ;

	case FRIENDS_ACCEPT:
		GTH_ProcessMessage_Friends_Accept () ;
		break ;

	case FRIENDS_REJECT:
		GTH_ProcessMessage_Friends_Reject () ;
		break ;

	case FRIENDS_REMOVE:
		GTH_ProcessMessage_Friends_Remove () ;
		break ;

	default:
		break ;
	}
}




void GTH_ProcessMessage_Friends_Invite (playerCharacter_t* pPlayer)
{
	int idx = MSG_ReadShort ();
	playerCharacter_t* pTarketPlayer = gcpTools->GetPlayerRecordPointer(idx);

	if( NULL == pTarketPlayer){
	
		GTH_DisconnectPlayer( pPlayer,FALSE);
		return;
	}

	if ( FALSE == pPlayer->active || FALSE == pTarketPlayer->ready )
	{
		GTH_SendMessage_Friends_Invite ( pPlayer, 0, FRIENDS_ERROR_NOT_READY, pTarketPlayer->idx, true );
		return ;
	}

	if ( pPlayer->friends.count >= MAX_FRIENDS || pTarketPlayer->friends.count >= MAX_FRIENDS)
	{
		return;
	}

	if ( pTarketPlayer->worldIdx != pPlayer->worldIdx )
	{
		GTH_SendMessage_Friends_Invite ( pPlayer, 0, FRIENDS_ERROR_WRONG_USER, pTarketPlayer->idx, true );
		return ;
	}

	if ( pTarketPlayer->friendsBusy == true || pPlayer->friendsBusy == true )
	{
		GTH_SendMessage_Friends_Invite ( pPlayer, 0, FRIENDS_ERROR_BUSY, pTarketPlayer->idx, true );
		return ;
	}



	if( (pPlayer->busyState == BUSY_STATE_BATTLEPRACTICE) ||
		(pTarketPlayer->busyState == BUSY_STATE_BATTLEPRACTICE)){
		
		GTH_SendMessage_Friends_Invite ( pPlayer, 0, FRIENDS_ERROR_BUSY, pTarketPlayer->idx, true );
		return ;
	}

	bool registered = false;
	for (int i = 0 ; i < pPlayer->friends.count ; ++i)
	{
		if ( !stricmp (pTarketPlayer->name, pPlayer->friends.names[i] ) )
			registered = true;
	}

	if ( registered )
	{
		GTH_SendMessage_Friends_Invite ( pPlayer, 0, FRIENDS_ERROR_WRONG_USER, idx, true );
		return;
	}

	
	
	pPlayer->friendsBusy = true ;
	pTarketPlayer->friendsBusy = true ;

	
	GTH_SendMessage_Friends_Invite ( pPlayer, 1, 0, pTarketPlayer->idx, true );
	GTH_SendMessage_Friends_Invite ( pTarketPlayer, 1, 0, pPlayer->idx, false );
}



void GTH_ProcessMessage_Friends_Accept ()
{
	int idx = MSG_ReadShort();

	if ( !g_pc[idx].active || !g_pc[idx].ready )
	{
		GTH_SendMessage_Friends_Invite ( g_curPC, 0, FRIENDS_ERROR_NOT_READY, idx, true );
		return ;
	}

	if ( g_pc[idx].worldIdx != g_curPC->worldIdx )
	{
		GTH_SendMessage_Friends_Invite ( g_curPC, 0, FRIENDS_ERROR_WRONG_USER, idx, true );
		return ;
	}

	bool registered = false ;
	for (int i = 0; i < g_curPC->friends.count; ++i)
	{
		if ( !stricmp (g_pc[idx].name, g_curPC->friends.names[i]) )
		{
			registered = true ;
		}
	}
	if ( !registered ) GTH_Friends_Add ( g_curPC, &g_pc[idx] );
	GTH_Friends_Add ( &g_pc[idx], g_curPC );

	
	g_curPC->friendsBusy = false;
	g_pc[idx].friendsBusy = false;
}



void GTH_ProcessMessage_Friends_Reject ()
{
	int idx = MSG_ReadShort ();

	if ( !g_pc[idx].active || !g_pc[idx].ready )
	{
		return;
	}

	if ( g_pc[idx].worldIdx != g_curPC->worldIdx )
	{
		return;
	}

	GTH_SendMessage_Friends_Reject ( &g_pc[idx], idx );

	g_curPC->friendsBusy = false ;
	g_pc[idx].friendsBusy = false ;
}

void GTH_ProcessMessage_Friends_Remove ()
{
	char targetName[NAMESTRING] ;
	strcpy ( targetName, MSG_ReadString () );

	
	GTH_Friends_Remove ( g_curPC, targetName );
}






void GTH_ProcessMessage_Guild_From_Client()
{
	int command;

	command = MSG_ReadByte();

	switch(command){
		
		
		case GUILD_SYNC:
			gcpGuildCtrl->BetweenClientGs_Decoder();
			break;	
		

		case GUILD_ORGANIZE :
			GTH_ProcessMessage_Guild_Organize_Request_From_Client();
			break;

		case GUILD_DISORGANIZE :
			GTH_ProcessMessage_Guild_Disorganize_Request_From_Client();
			break;

		case GUILD_INVITE_REQUEST :
			GTH_ProcessMessage_Guild_Invite_Request_From_Client();
			break;

		case GUILD_INVITE_CANCEL :
			GTH_ProcessMessage_Guild_Invite_Cancel_From_Client();
			break;

		case GUILD_INVITE_REPLY :
			GTH_ProcessMessage_Guild_Invite_Reply_From_Client();
			break;

		case GUILD_SECEDE:
			GTH_ProcessMessage_Guild_Secede_Request_From_Client();
			break;


		case GUILD_BANISH:
			GTH_ProcessMessage_Guild_Banish_Request_From_Client();
			break;


		case GUILD_GUILDMARK:
			GTH_ProcessMessage_Guild_GuildMark_From_Client();
			break;


		case GUILD_CHANGEMASTER:
			GTH_ProcessMessage_Guild_Change_Master_Request_From_Client();
			break;

		case GUILD_ADDSUBMASTER:
			GTH_ProcessMessage_Guild_Add_SubMaster_Request_From_Client();
			break;

		case GUILD_ADDFACTIONMASTER:
			GTH_ProcessMessage_Guild_Add_FactionMaster_Request_From_Client();
			break;

		case GUILD_DELETE_MASTER:
			GTH_ProcessMessage_Guild_Delete_Master_Request_From_Client();
			break;

		case GUILD_SCHEDULE:
			GTH_ProcessMessage_Guild_Schedule_Request();
			break;

		case GUILD_SUBSCRIBE:
			GTH_ProcessMessage_Guild_Subscribe_Request_From_Client();
			break;
	}
}

void GTH_ProcessMessage_Guild_Organize_Request_From_Client()
{
	int i;
	int error = -1;
	char guildName[GUILDNAMESTRING+1];
	char guildMark[GUILDMARKSIZE];

	memset( guildMark, 0, GUILDMARKSIZE );

	
	sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );
	guildName[GUILDNAMESTRING]=NULL;


	for(i=0; i<GUILDMARKSIZE; i++ ) guildMark[i] = MSG_ReadByte();



	
	if( g_curPC->guildIdx != -1 )
	{
		
		error = GUILD_ERROR_ALREADYJOINED;
		GTH_SendMessage_Guild_Organize_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	
	if( g_curPC->level < GUILD_ORGANIZABLE_LEVEL )
	{
		
		error = GUILD_ERROR_LEVEL;
		GTH_SendMessage_Guild_Organize_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	
	if( g_curPC->pcJob < J_DEVOTIONAL_KNIGHT )
	{
		error = GUILD_ERROR_NOTAWAKENED;
		GTH_SendMessage_Guild_Organize_Reply_To_Client( g_curPC, -1, error );
		return;
	}

#ifndef _DEV_TEST
	
	if( g_curPC->rankPoint < GUILD_ORGANIZABLE_RANKPOINT )
	{
		
		error = GUILD_ERROR_RANKPOINT;
		GTH_SendMessage_Guild_Organize_Reply_To_Client( g_curPC, -1, error );
		return;
	}
#endif

	
	if( g_curPC->curChargeSE < GUILD_ORGANIZABLE_NAK )
	{
		
		error = GUILD_ERROR_NAK;
		GTH_SendMessage_Guild_Organize_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	
	
	time_t NowTime = time(NULL);
	if (g_curPC->GuildDeconstructionTime > NowTime)
	{
		
		error = GUILD_ERROR_ALLOW_CREATE;
		GTH_SendMessage_Guild_Organize_Reply_To_Client( g_curPC, -1, error , (g_curPC->GuildDeconstructionTime - NowTime));
		return;
	}

	if ( g_config.isManager )
	{
		
		g_curPC->guildIdx = GTH_GuildOrganize( g_curPC->name, guildName, guildMark, g_config.serverGroupNo, error );

		if( g_curPC->guildIdx >= 0 )
		{
			g_curPC->curChargeSE -= GUILD_ORGANIZABLE_NAK;
			g_curPC->rankPoint -= GUILD_ORGANIZABLE_RANKPOINT;
			
			if( 0 > g_curPC->rankPoint ) g_curPC->rankPoint = 0;			
		
			strncpy(g_curPC->guildName,guildName,GUILDNAMESTRING);
			g_curPC->guildName[GUILDNAMESTRING]=NULL;			
			
			GTH_SendMessage_Guild_Organize_Reply_To_Client( g_curPC, 0, error );


			
			gcpGuildCtrl->ManagerServerProc_Organize(g_curPC->guildIdx,guildName,(const unsigned char*)guildMark);
			g_DBGameServer->UpdateGuildIdx(g_curPC->userID,g_curPC->serverGroupID, g_curPC->characterID, g_curPC->guildIdx);
			
		}
		else
		{
			GTH_SendMessage_Guild_Organize_Reply_To_Client( g_curPC, -1, error );
		}
	}
	else
	{
		
		GTH_SendMessage_Guild_Organize_Request_To_ManagerServer( g_curPC, guildName, guildMark );
	}
}


void GTH_ProcessMessage_Guild_Disorganize_Request_From_Client()
{
	int error = -1;
	

	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_NOTJOINED;
		GTH_SendMessage_Guild_Disorganize_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	
	if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) != 0 )
	{
		error = GUILD_ERROR_NOMASTER;
		GTH_SendMessage_Guild_Disorganize_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	
	
	if ( ( g_curPC->ready == TRUE) && (g_curPC->active == TRUE) ) {
		NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
		pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(g_curPC->DanBattleInfo.nDanBattleIdx, g_curPC->guildIdx);
		if( pDanBattleInfo != NULL ) {
			
			error = -1;
			GTH_SendMessage_Guild_Disorganize_Reply_To_Client( g_curPC, -1, error);
			return;
		}
	}
	

	
	if( g_config.isManager )
	{

		int result = GTH_GuildDisorganize( g_curPC->guildIdx, g_curPC->name, error );
		if ( result != -1 )
		{
			
			gcpGuildCtrl->ManagerServerProc_DisOrganize(g_curPC->guildIdx);		
			g_DBGameServer->UpdateGuildIdx(g_curPC->userID,g_curPC->serverGroupID, g_curPC->characterID,-1);			
			

			g_curPC->guildIdx = -1;
			g_curPC->rankPoint -= 50; 
			if( 0 > g_curPC->rankPoint ) g_curPC->rankPoint = 0;

			
			time_t NowTime = time(NULL) + DELAY_DAN_CREATE_SEC;
			g_curPC->GuildDeconstructionTime = NowTime;

			
			NowTime = time(NULL) + DELAY_DAN_REJOIN_SEC_FOR_GENERAL_REMOVE;
			g_curPC->GuildsecedeTime = NowTime;
		}

		GTH_SendMessage_Guild_Disorganize_Reply_To_Client( g_curPC, result, error );
		
	}
	
	else
	{
		
		GTH_SendMessage_Guild_Disorganize_Request_To_ManagerServer( g_curPC );
	}
}

bool IsGuildMasters( playerCharacter_t *pc )
{
	int i;

	if( stricmp( g_guild[pc->guildIdx].masterName, pc->name ) == 0 )
		return true;
	
	for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
	{
		if( stricmp( g_guild[pc->guildIdx].subMasterName[i], pc->name ) == 0 )
			return true;
	}

	for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
	{
		if( stricmp( g_guild[pc->guildIdx].factionMasterName[i], pc->name ) == 0 )
			return true;
	}

	return false;
}


void GTH_ProcessMessage_Guild_Invite_Request_From_Client()
{
	int error = -1;
	char name[NAMESTRING+1];

	
	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
	name[NAMESTRING]=NULL;



	strcpy( g_curPC->tempGuildMateName, name );

	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_NOTJOINED;
		
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 );
		return;
	}

	
	if( !IsGuildMasters( g_curPC ) )
	{
		error = GUILD_ERROR_INVITE_NOTMASTER;
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 );
		return;
	}

	int idx = GTH_FindPCByName( name );
	if( idx == -1 )
	{
		error = GUILD_ERROR_INVITE_NONEPCINDEX;
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 );
		return;
	}

	
	if( g_pc[idx].guildIdx != -1 )
	{
		error = GUILD_ERROR_INVITE_ALREADY;
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 );
		return;
	}

	
	if( g_pc[idx].level < GUILD_SUBSCRIBABLE_LEVEL )
	{
		error = GUILD_ERROR_INVITE_NOTLEVEL;
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 );
		return;
	}

	if( !g_curPC->ready || !g_pc[idx].ready || !g_curPC->alive || !g_pc[idx].alive )
	{
		error = GUILD_ERROR_INVITE_NOTREADY;
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 );
		return;
	}

	
	if( g_curPC->worldIdx != g_pc[idx].worldIdx )
	{
		error = GUILD_ERROR_INVITE_DIFFWORLDINDEX;
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 );
		return;
	}

	
	if( g_curPC->busyState != BUSY_STATE_NONE || g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_INVITE_PCSTATE;
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 );
		return;
	}

	
	
	time_t NowTime = time(NULL);		
	if (g_pc[idx].GuildsecedeTime > NowTime)
	{
		
		error = GUILD_ERROR_ALLOW_JOIN;
		GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, -1, error, 0 , (g_pc[idx].GuildsecedeTime - NowTime ));
		return;
	}

	
	
	GTH_AddNoticePC( g_curPC, ENTITY_PC, idx, BUSY_STATE_REQUEST );
	GTH_AddNoticePC( &g_pc[idx], ENTITY_PC, g_curPC->idx, BUSY_STATE_REQUEST );

	
	strcpy( g_curPC->tempGuildMateName, g_pc[idx].name );

	
	GTH_SendMessage_Guild_Invite_Request_To_Client( g_curPC, 0, error, 0 );

	
	strcpy( g_pc[idx].guildName, g_guild[g_curPC->guildIdx].name );
	
	
	strncpy(g_pc[idx].Guild.FromInvite.Name,g_curPC->name,NAMESTRING);
	g_pc[idx].Guild.FromInvite.Name[NAMESTRING]=NULL;

	
	GTH_SendMessage_Guild_Invite_Request_To_Client( &g_pc[idx], 0, error, 1 );
}


void GTH_ProcessMessage_Guild_Invite_Cancel_From_Client()
{
	int error = -1;

	GTH_SendMessage_Guild_Invite_Cancel_To_Client( g_curPC, error, 0, g_curPC->tempGuildMateName );

	
	GTH_DeleteNoticePC( g_curPC );

	
	int idx = GTH_FindPCByName( g_curPC->tempGuildMateName );
	if( idx == -1 ) return;

	GTH_SendMessage_Guild_Invite_Cancel_To_Client( &g_pc[idx], error, 1, g_curPC->guildName );
	
	GTH_DeleteNoticePC( &g_pc[idx] );

}


void GTH_ProcessMessage_Guild_Invite_Reply_From_Client()
{
	int reply;
	int error = -1;

	reply = MSG_ReadShort();



	int idx = GTH_FindPCByName(g_curPC->Guild.FromInvite.Name);

	
	
	
	if( reply == 0 )
	{
		
		GTH_SendMessage_Guild_Invite_Reply_To_Client( g_curPC, -1, error, 1, g_curPC->guildName );
		GTH_DeleteNoticePC( g_curPC );

		if( idx >= 0 )
		{
			GTH_SendMessage_Guild_Invite_Reply_To_Client( &g_pc[idx], -1, error, 0, g_curPC->name );
			GTH_DeleteNoticePC( &g_pc[idx] );
		}

		return;
	}

	
	
	
	if( idx < 0 )
	{
		error = GUILD_ERROR_CANNOTFIND;
		GTH_SendMessage_Guild_Invite_Reply_To_Client( g_curPC, -1, error, 1, g_curPC->guildName );
		GTH_DeleteNoticePC( g_curPC );
		return;
	}

	short guildIdx = GTH_GetGuildIndexByName( g_curPC->guildName );
	if( g_curPC->guildIdx != -1 || guildIdx == -1 )
	{
		error = GUILD_ERROR_CANNOTFIND;
		GTH_SendMessage_Guild_Invite_Reply_To_Client( &g_pc[idx], -1, error, 0, g_curPC->name );
		GTH_SendMessage_Guild_Invite_Reply_To_Client( g_curPC, -1, error, 1, g_curPC->guildName );

		GTH_DeleteNoticePC( &g_pc[idx] );
		GTH_DeleteNoticePC( g_curPC );

		return;
	}

	if( !g_curPC->ready || !g_curPC->alive || !g_pc[idx].ready || !g_pc[idx].alive )
	{
		error = GUILD_ERROR_CANNOTFIND;
		GTH_SendMessage_Guild_Invite_Reply_To_Client( &g_pc[idx], -1, error, 0, g_curPC->name );
		GTH_SendMessage_Guild_Invite_Reply_To_Client( g_curPC, -1, error, 1, g_curPC->guildName );

		GTH_DeleteNoticePC( &g_pc[idx] );
		GTH_DeleteNoticePC( g_curPC );

		return;
	}

	if( g_curPC->worldIdx != g_pc[idx].worldIdx )
	{
		error = GUILD_ERROR_CANNOTFIND;
		GTH_SendMessage_Guild_Invite_Reply_To_Client( &g_pc[idx], -1, error, 0, g_curPC->name );
		GTH_SendMessage_Guild_Invite_Reply_To_Client( g_curPC, -1, error, 1, g_curPC->guildName );

		GTH_DeleteNoticePC( &g_pc[idx] );
		GTH_DeleteNoticePC( g_curPC );

		return;
	}

	
	
	
	if( g_config.isManager )
	{
		
		int rankIdxInGuild = -1;
		int result = GTH_GuildAddUser( guildIdx, g_curPC->name, g_curPC->pcJob, rankIdxInGuild, error );

		if( result != -1 )
		{
			g_curPC->guildIdx = guildIdx;
			g_curPC->rankInGuild = GUILD_MATE;
			g_curPC->rankIdxInGuild = rankIdxInGuild;
			strcpy( g_curPC->guildName, g_guild[guildIdx].name );

			
			g_DBGameServer->UpdateGuildIdx(g_curPC->userID,g_curPC->serverGroupID, g_curPC->characterID, g_curPC->guildIdx);			
			
		}
		else
		{
			error = GUILD_ERROR_CANNOTFIND;
		}

		GTH_SendMessage_Guild_Invite_Reply_To_Client( &g_pc[idx], result, error, 0, g_curPC->name );
		GTH_SendMessage_Guild_Invite_Reply_To_Client( g_curPC, result, error, 1, g_curPC->guildName );

		GTH_DeleteNoticePC( &g_pc[idx] );
		GTH_DeleteNoticePC( g_curPC );
		
	}
	else
	{
		
		GTH_SendMessage_Guild_Add_Request_To_ManagerServer( &g_pc[idx], g_curPC, GUILD_MATE );
	}
}



void GTH_ProcessMessage_Guild_Secede_Request_From_Client()
{
	int error = -1;

	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_SECEDE_0;
		GTH_SendMessage_Guild_Secede_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	if( !g_curPC->ready || !g_curPC->alive )
	{
		error = GUILD_ERROR_SECEDE_1;
		GTH_SendMessage_Guild_Secede_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	if( g_curPC->busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_SECEDE_2;
		GTH_SendMessage_Guild_Secede_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo  = NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(g_curPC->DanBattleInfo.nDanBattleIdx, g_curPC->guildIdx);
	if ( pDanBattleInfo != NULL)
	{
		
		error = GUILD_ERROR_BANISH_1;
		GTH_SendMessage_Guild_Secede_Reply_To_Client( g_curPC, -1, error );
		return;
	}


	
	if( g_config.isManager )
	{
		
		if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) == 0 )
		{
			error = GUILD_ERROR_SECEDE_3;
			GTH_SendMessage_Guild_Secede_Reply_To_Client( g_curPC, -1, error );
			return;
		}

		
		int result = GTH_GuildRemoveUser( g_curPC->guildIdx, g_curPC->name, error );

		if( result != -1)
		{
			g_curPC->guildIdx = -1;
			g_curPC->rankPoint -= 10;
		
			if( 0 > g_curPC->rankPoint) g_curPC->rankPoint = 0;
		
			
			time_t NowTime = time(NULL) + DELAY_DAN_REJOIN_SEC_FOR_GENERAL_REMOVE;
			g_curPC->GuildsecedeTime = NowTime;
		
			
			g_DBGameServer->UpdateGuildIdx(g_curPC->userID,g_curPC->serverGroupID, g_curPC->characterID, g_curPC->guildIdx);			
			

		}

		GTH_SendMessage_Guild_Secede_Reply_To_Client( g_curPC, result, error );
	}
	else
	{
		
		GTH_SendMessage_Guild_Remove_Request_To_ManagerServer( g_curPC, GUILD_MATE );
	}
}


void GTH_ProcessMessage_Guild_Banish_Request_From_Client()
{
	int result;
	int error = -1;
	char name[NAMESTRING+1];

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
	name[NAMESTRING]=NULL;



	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_BANISH_0;
		GTH_SendMessage_Guild_Banish_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( !g_curPC->ready || !g_curPC->alive )
	{
		error = GUILD_ERROR_BANISH_2;
		GTH_SendMessage_Guild_Banish_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( g_curPC->busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_BANISH_3;
		GTH_SendMessage_Guild_Banish_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( stricmp( g_guild[g_curPC->guildIdx].masterName, name ) == 0 )
	{
		error = GUILD_ERROR_BANISH_4;
		GTH_SendMessage_Guild_Banish_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}


	int idx = GTH_FindPCByName( name );
	
	if( idx != -1 )
	{
		if( !g_pc[idx].ready || !g_pc[idx].alive )
		{
			error = GUILD_ERROR_BANISH_2;
			GTH_SendMessage_Guild_Banish_Reply_To_Client( g_curPC, -1, error, 0, name );
			return;
		}
	}


	
	
	
	playerCharacter_t* pPc = gcpTools->GetPlayerRecordPointer(idx);	
	if (pPc != NULL) {
		if ( ( pPc->ready == TRUE) && (pPc->active == TRUE) ) {
			NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
			pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(pPc->DanBattleInfo.nDanBattleIdx, pPc->guildIdx);
			if( pDanBattleInfo != NULL ) {
				
				error = GUILD_ERROR_BANISH_3;
				GTH_SendMessage_Guild_Banish_Reply_To_Client( g_curPC, -1, error, 0, name );
				return;
			}
		}
	}
	

	
	if( g_config.isManager )
	{
		
		if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) != 0 )
		{
			int i;
			for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
			{
				if( stricmp( g_guild[g_curPC->guildIdx].subMasterName[i], g_curPC->name ) == 0 )
					break;
			}
			
			if( i == MAX_GUILD_SUBMASTERS )
			{
				error = GUILD_ERROR_BANISH_5;
				GTH_SendMessage_Guild_Banish_Reply_To_Client( g_curPC, -1, error, 0, name );
				return;
			}
		}

		
		result = GTH_GuildRemoveUser( g_curPC->guildIdx, name, error );
		if( result != -1 )
		{
			
			if( idx == -1 )
			{
				
				
				g_DBGameServer->BanishMemberGuild(name, 10 );
			}
			
			else
			{
				g_pc[idx].guildIdx = -1;
				g_pc[idx].rankPoint -= 10;
				
				if( 0 > g_pc[idx].rankPoint) g_pc[idx].rankPoint = 0;
			
				
				g_DBGameServer->UpdateGuildIdx(g_pc[idx].userID,g_pc[idx].serverGroupID, g_pc[idx].characterID, g_pc[idx].guildIdx);			
				

				


				
				
				time_t NowTime = time(NULL) + DELAY_DAN_REJOIN_SEC_FOR_POWER_REMOVE;
				g_pc[idx].GuildsecedeTime = NowTime;

				GTH_SendMessage_Guild_Banish_Reply_To_Client( &g_pc[idx], result, error, 1, g_curPC->guildName );
			}
		}

		GTH_SendMessage_Guild_Banish_Reply_To_Client( g_curPC, result, error, 0, name );
	}
	else
	{
		GTH_SendMessage_Guild_Banish_Request_To_ManagerServer( g_curPC->guildIdx, g_curPC->name, name, GUILD_MATE );
	}
}




void GTH_ProcessMessage_Guild_Change_Master_Request_From_Client()
{
	int result;
	int error = -1;
	char name[NAMESTRING];

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );



	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_NOGUILD;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	int idx = GTH_FindPCByName( name );
	if( idx == -1 )
	{
		error = GUILD_ERROR_CANNOTFIND;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].guildIdx != g_curPC->guildIdx )
	{
		error = GUILD_ERROR_NOTJOINED;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( !g_curPC->ready || !g_pc[idx].ready || !g_curPC->alive || !g_pc[idx].alive )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( g_curPC->busyState != BUSY_STATE_NONE || g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( stricmp( g_guild[g_curPC->guildIdx].masterName, name ) == 0 )
	{
		error = GUILD_ERROR_SAMEMATENAME;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) != 0 )
	{
		error = GUILD_ERROR_AUTHORITY;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].level < GUILD_ORGANIZABLE_LEVEL )
	{
		error = GUILD_ERROR_LEVEL;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].pcJob < J_DEVOTIONAL_KNIGHT )
	{
		error = GUILD_ERROR_NOTAWAKENED;
		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_config.isManager )
	{
		
		int rankIdxInGuild = -1;
		result = GTH_GuildChangeMaster( g_curPC->guildIdx, g_curPC->name, name, g_pc[idx].pcJob, rankIdxInGuild, error );

		if( result != -1 )
		{
			g_pc[idx].rankInGuild = GUILD_MASTER;
			g_pc[idx].rankIdxInGuild = rankIdxInGuild;
			
			GTH_SendMessage_Guild_Change_Master_Reply_To_Client( &g_pc[idx], result, error, 1, name );
		}

		GTH_SendMessage_Guild_Change_Master_Reply_To_Client( g_curPC, result, error, 0, name );
	}
	else
	{
		
		GTH_SendMessage_Guild_Change_Master_Request_To_ManagerServer( g_curPC->guildIdx, g_curPC, &g_pc[idx] );
	}

}


void GTH_ProcessMessage_Guild_Add_SubMaster_Request_From_Client()
{
	int result;
	int error = -1;
	char name[NAMESTRING];

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );



	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_NOGUILD;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	int idx = GTH_FindPCByName( name );
	if( idx == -1 )
	{
		error = GUILD_ERROR_CANNOTFIND;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].guildIdx != g_curPC->guildIdx )
	{
		error = GUILD_ERROR_NOTJOINED;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( !g_curPC->ready || !g_pc[idx].ready || !g_curPC->alive || !g_pc[idx].alive )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( g_curPC->busyState != BUSY_STATE_NONE || g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( stricmp( g_guild[g_curPC->guildIdx].masterName, name ) == 0 )
	{
		error = GUILD_ERROR_NOMASTER;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) != 0 )
	{
		error = GUILD_ERROR_AUTHORITY;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].level < GUILD_ORGANIZABLE_LEVEL )
	{
		error = GUILD_ERROR_LEVEL;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].pcJob < J_DEVOTIONAL_KNIGHT )
	{
		error = GUILD_ERROR_NOTAWAKENED;
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_config.isManager )
	{
		
		int rankIdxInGuild = -1;
		result = GTH_GuildAddSubMaster( g_curPC->guildIdx, name, g_pc[idx].pcJob, rankIdxInGuild, error );

		if( result != -1 )
		{
			g_pc[idx].rankInGuild = GUILD_SUBMASTER;
			g_pc[idx].rankIdxInGuild = rankIdxInGuild;
			
			GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( &g_pc[idx], result, error, 1, name );
		}

		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( g_curPC, result, error, 0, name );
	}
	else
	{
		
		GTH_SendMessage_Guild_Add_SubMaster_Request_To_ManagerServer( g_curPC->guildIdx, g_curPC, &g_pc[idx] );
	}
}

void GTH_ProcessMessage_Guild_Add_FactionMaster_Request_From_Client()
{
	int i;
	int result;
	int error = -1;
	char name[NAMESTRING];

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );



	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_NOGUILD;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	int idx = GTH_FindPCByName( name );
	if( idx == -1 )
	{
		error = GUILD_ERROR_CANNOTFIND;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].guildIdx != g_curPC->guildIdx )
	{
		error = GUILD_ERROR_NOTJOINED;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( !g_curPC->ready || !g_pc[idx].ready || !g_curPC->alive || !g_pc[idx].alive )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( g_curPC->busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( stricmp( g_guild[g_curPC->guildIdx].masterName, name ) == 0 )
	{
		error = GUILD_ERROR_NOMASTER;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	
	int authority = 0;
	if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) == 0 )
		authority = 1;

	for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
	{
		if( stricmp( g_guild[g_curPC->guildIdx].subMasterName[i], g_curPC->name ) == 0 )
		{
			authority = 1;
			break;
		}
	}

	if( authority == 0 )
	{
		error = GUILD_ERROR_AUTHORITY;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].level < GUILD_ORGANIZABLE_LEVEL )
	{
		error = GUILD_ERROR_LEVEL;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_pc[idx].pcJob < J_DEVOTIONAL_KNIGHT )
	{
		error = GUILD_ERROR_NOTAWAKENED;
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_config.isManager )
	{
		
		int rankIdxInGuild = -1;
		result = GTH_GuildAddFactionMaster( g_curPC->guildIdx, name, g_pc[idx].pcJob, rankIdxInGuild, error );

		if( result != -1 )
		{
			g_pc[idx].rankInGuild = GUILD_FACTIONMASTER;
			g_pc[idx].rankIdxInGuild = rankIdxInGuild;
			
			GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( &g_pc[idx], result, error, 1, name );
		}

		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( g_curPC, result, error, 0, name );
	}
	else
	{
		
		GTH_SendMessage_Guild_Add_FactionMaster_Request_To_ManagerServer( g_curPC->guildIdx, g_curPC, &g_pc[idx] );
	}
}

void GTH_ProcessMessage_Guild_Remove_SubMaster_Request_From_Client( char *name )
{
	int result;
	int error = -1;



	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_NOGUILD;
		GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( !g_curPC->ready || !g_curPC->alive )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( g_curPC->busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	int idx = GTH_FindPCByName( name );

	
	if( idx == -1 )
	{
		
		if( g_config.isManager )
		{
			
			if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) != 0 )
			{
				error = GUILD_ERROR_AUTHORITY;
				GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
				return;
			}

			
			result = GTH_GuildRemoveSubMaster( g_curPC->guildIdx, name, error );

			GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, result, error, 0, name );
		}
		else
		{
			
			GTH_SendMessage_Guild_Remove_SubMaster_Request_To_ManagerServer( g_curPC, name, -1 );
		}

		return;
	}
	
	else
	{
		if( !g_pc[idx].ready || !g_pc[idx].alive )
		{
			error = GUILD_ERROR_OFFLINE;
			GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
			return;
		}

		if( g_pc[idx].busyState != BUSY_STATE_NONE )
		{
			error = GUILD_ERROR_OFFLINE;
			GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
			return;
		}
	}

	
	if( g_config.isManager )
	{
		
		if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) != 0 )
		{
			error = GUILD_ERROR_AUTHORITY;
			GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
			return;
		}

		
		result = GTH_GuildRemoveSubMaster( g_curPC->guildIdx, name, error );

		if( result != -1 )
		{
			GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( &g_pc[idx], result, error, 1, name );
		}

		GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( g_curPC, result, error, 0, name );
	}
	else
	{
		
		GTH_SendMessage_Guild_Remove_SubMaster_Request_To_ManagerServer( g_curPC, g_pc[idx].name, g_pc[idx].pcJob );
	}
}

void GTH_ProcessMessage_Guild_Remove_FactionMaster_Request_From_Client( char *name )
{
	int i;
	int result;
	int error = -1;



	if( g_curPC->guildIdx == -1 )
	{
		error = GUILD_ERROR_NOGUILD;
		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( !g_curPC->ready || !g_curPC->alive )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( g_curPC->busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	int idx = GTH_FindPCByName( name );

	if( idx == -1 )
	{
		if( g_config.isManager )
		{
			
			char authority = 0;
			if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) == 0 )
				authority = 1;

			for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
			{
				if( stricmp( g_guild[g_curPC->guildIdx].subMasterName[i], g_curPC->name ) == 0 )
				{
					authority = 1;
					break;
				}
			}

			if( authority == 0 )
			{
				error = GUILD_ERROR_AUTHORITY;
				GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
				return;
			}

			
			result = GTH_GuildRemoveFactionMaster( g_curPC->guildIdx, name, error );

			GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, result, error, 0, name );
		}
		else
		{
			
			GTH_SendMessage_Guild_Remove_FactionMaster_Request_To_ManagerServer( g_curPC, name, -1 );
		}

		return;
	}

	
	if( !g_pc[idx].ready || !g_pc[idx].alive )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	if( g_pc[idx].busyState != BUSY_STATE_NONE )
	{
		error = GUILD_ERROR_OFFLINE;
		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
		return;
	}

	
	if( g_config.isManager )
	{
		
		char authority = 0;
		if( stricmp( g_guild[g_curPC->guildIdx].masterName, g_curPC->name ) == 0 )
			authority = 1;

		for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
		{
			if( stricmp( g_guild[g_curPC->guildIdx].subMasterName[i], g_curPC->name ) == 0 )
			{
				authority = 1;
				break;
			}
		}

		if( authority == 0 )
		{
			error = GUILD_ERROR_AUTHORITY;
			GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, -1, error, 0, name );
			return;
		}

		
		result = GTH_GuildRemoveFactionMaster( g_curPC->guildIdx, name, error );

		if( result != -1 )
		{
			GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( &g_pc[idx], result, error, 1, name );
		}

		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( g_curPC, result, error, 0, name );
	}
	else
	{
		
		GTH_SendMessage_Guild_Remove_FactionMaster_Request_To_ManagerServer( g_curPC, g_pc[idx].name, g_pc[idx].pcJob );
	}
}

void GTH_ProcessMessage_Guild_Delete_Master_Request_From_Client()
{
	int i, error = -1;
	int flag = 0;
	char name[NAMESTRING];

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );

	short guildIdx = g_curPC->guildIdx;

	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
	{
		error = GUILD_ERROR_NOGUILD;
		GTH_SendMessage_Guild_Delete_Master_Reply_To_Client( g_curPC, error, name );
	}

	
	for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
	{
		if( g_guild[guildIdx].subMasterName[i][0] == 0 ) continue;

		if( stricmp( g_guild[guildIdx].subMasterName[i], name ) == 0 )
		{
			flag = 1;
		}
	}

	for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
	{
		if( g_guild[guildIdx].factionMasterName[i][0] == 0 ) continue;

		if( stricmp( g_guild[guildIdx].factionMasterName[i], name ) == 0 )
		{
			flag = 2;
		}
	}

	if( stricmp( g_guild[guildIdx].masterName, name ) == 0 )
		flag = 3;

	switch( flag )
	{
	case 1:
		GTH_ProcessMessage_Guild_Remove_SubMaster_Request_From_Client( name );
		break;
	case 2:
		GTH_ProcessMessage_Guild_Remove_FactionMaster_Request_From_Client( name );
		break;
	case 3:
		error = GUILD_ERROR_NOMASTER;
		GTH_SendMessage_Guild_Delete_Master_Reply_To_Client( g_curPC, error, name );
		break;
	default:
		error = GUILD_ERROR_CANNOTAPPOINT;
		GTH_SendMessage_Guild_Delete_Master_Reply_To_Client( g_curPC, error, name );
		break;
	}

}

void GTH_ProcessMessage_Guild_Schedule_Request()
{
	char notice[128];
	char name[NAMESTRING];
	strcpy( notice, MSG_ReadString() );
	bool flag = false;
	int i;

	short guildIdx = g_curPC->guildIdx;
	strcpy( name, g_curPC->name );

	if( guildIdx != -1 )
	{
		if( !stricmp( g_guild[guildIdx].masterName, name ) )
		{
			flag = true;
		}
		else
		{
			
			for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
			{
				if( g_guild[guildIdx].subMasterName[i][0] == 0 ) continue;

				if( !stricmp( g_guild[guildIdx].subMasterName[i], name ) )
				{
					flag = true;
					break;
				}
			}

			for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
			{
				if( g_guild[guildIdx].factionMasterName[i][0] == 0 ) continue;

				if( !stricmp( g_guild[guildIdx].factionMasterName[i], name ) )
				{
					flag = true;
					break;
				}
			}
		}
	}

	if( flag )
	{
		
		if( g_config.isManager )
		{
			
			strcpy( g_guild[guildIdx].notice, notice );

			int update = GUILD_NOTICE;
			GTH_GuildUpdate( guildIdx, update, -1, -1, GUILD_ALLRANK, -1 );
		}
		else
		{
			
			GTH_SendMessage_Guild_Schedule_Request_To_ManagerServer( g_curPC->guildIdx, g_curPC, notice );
		}
	}
}


void GTH_ProcessMessage_Guild_Subscribe_Request_From_Client()
{
	short guildIdx;
	int error = -1;
	char guildName[GUILDNAMESTRING];

	
	sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );

	
	if( g_curPC->guildIdx != -1 )
	{
		
		error = GUILD_ERROR_ALREADYJOINED;
		GTH_SendMessage_Guild_Subscribe_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	
	if( g_curPC->level < GUILD_SUBSCRIBABLE_LEVEL )
	{
		
		error = GUILD_ERROR_LEVEL;
		GTH_SendMessage_Guild_Subscribe_Reply_To_Client( g_curPC, -1, error );
		return;
	}

	if( g_config.isManager )
	{
		for( int i=0; i<MAX_GUILDS; i++ )
		{
			if( !stricmp( g_guild[i].name, guildName ) )
			{
				guildIdx = i;
				break;
			}
		}

		if( i == MAX_GUILDS )
		{
			error = GUILD_ERROR_NOGUILD;
			GTH_SendMessage_Guild_Subscribe_Reply_To_Client( g_curPC, -1, error );
			return;
		}

		int bSended = 0;
		
		if( g_guild[guildIdx].userLocation[0] != -1 )
		{
			
			GTH_Send_Subscribe_Request_Message_To_Client( g_guild[guildIdx].masterName );
			bSended = 1;
			GTH_SendMessage_Guild_Subscribe_Reply_To_Client( g_curPC, 0, 0, guildName );
		}
		else
		{
			
			int temp = MAX_GUILD_MASTERS;
			for( int i=0; i<MAX_GUILD_SUBMASTERS; i++ )
			{
				if( g_guild[guildIdx].userLocation[i+temp] != -1 )
				{
					
					GTH_Send_Subscribe_Request_Message_To_Client( g_guild[guildIdx].subMasterName[i] );
					bSended = 1;
					GTH_SendMessage_Guild_Subscribe_Reply_To_Client( g_curPC, 0, i+temp, guildName );
				}
			}
		}
		
		if( bSended == 0 )
		{
			error = GUILD_ERROR_NOMASTER;
			GTH_SendMessage_Guild_Subscribe_Reply_To_Client( g_curPC, -1, error );
			return;
		}
	}
	else
	{
		GTH_SendMessage_Guild_Subscribe_Request_To_ManagerServer( g_curPC, guildName );
	}

}







void GTH_ProcessMessage_Guild_GuildMark_From_Client(void)
{
	int guildIdx = MSG_ReadShort();

	struct CGuildCtrl::tagGuildSummarySyncRecord* pGuildSummarySyncRecord=gcpGuildCtrl->GetSummaryRecordPointer(guildIdx);
	if(NULL == pGuildSummarySyncRecord) return;

	GTH_SendMessage_Guild_GuildMark_Reply_To_Client(
		g_curPC, 
		guildIdx, 
		pGuildSummarySyncRecord->Mark.ChangeDate,
		pGuildSummarySyncRecord->Mark.Image);
}










void GTH_ProcessMessage_QuestScroll_ObtainOpen()
{
	GTH_AddNoticePC( g_curPC, -1, -1, BUSY_STATE_QUESTSCROLL );

	GTH_SendMessage_QuestScroll_ObtainOpen( g_curPC, 1 );
}

void GTH_ProcessMessage_QuestScroll_ObtainPrepare()
{

	int reply = 1, error = 0, reqNak = 0;
	int tableIdx, depart, level, reward;

	depart = MSG_ReadByte();
	level = MSG_ReadByte();
	reward = MSG_ReadByte();

	GTH_QuestScroll_ObtainProcess( g_curPC, reply, error, reqNak, tableIdx, depart, level, reward, 0, true );
	GTH_SendMessage_QuestScroll_ObtainPrepare( g_curPC, reply, error, reqNak, tableIdx );

}

void GTH_ProcessMessage_QuestScroll_ObtainComplete()
{
	int reply = 1, error = 0, reqNak = 0;
	int tableIdx, depart, level, reward, selectedQSTableIdx;

	depart = MSG_ReadByte();
	level = MSG_ReadByte();
	reward = MSG_ReadByte();
	selectedQSTableIdx = MSG_ReadShort();

	GTH_QuestScroll_ObtainProcess( g_curPC, reply, error, reqNak, tableIdx, depart, level, reward, selectedQSTableIdx, false );
	GTH_SendMessage_QuestScroll_ObtainComplete( g_curPC, reply, error, reqNak, tableIdx );
}

void GTH_ProcessMessage_QuestScroll_ObtainClose()
{
	GTH_DeleteNoticePC( g_curPC );

	GTH_SendMessage_QuestScroll_ObtainClose( g_curPC );
}

void GTH_ProcessMessage_QuestScroll_ComboListChange()
{

	int depart = MSG_ReadByte();

	GTH_SendMessage_QuestScroll_ComboListChange( g_curPC, depart );

}






void GTH_ProcessMessage_AutoQuestScroll_ObtainOpen( playerCharacter_t* pPlayer )
{
	
	
	for( int idx = 0; idx < g_autoQuestscrollTableNumber; idx++ ){
		
		if( pPlayer->level != g_autoQuestscrollTable[idx].data.level ) continue;

		if( ( g_autoQuestscrollTable[idx].data.reward.regClass != -1 )&&
			( g_autoQuestscrollTable[idx].data.reward.regClass != pPlayer->pcClass ) ) continue;		
			
		float totalexp = g_expTable[pPlayer->level-1].totalexp-pPlayer->exp;
		float expRate = ( 1.0f-totalexp / g_expTable[pPlayer->level-1].exp ) * 100.0f;
		
		if( g_autoQuestscrollTable[idx].data.startexp > expRate ) return;

		
		int table = AUTOQUESTSCROLL_ITEM_INDEX + g_autoQuestscrollTable[idx].Index;
       	
		if( ( 0 > table ) && ( table >= MAX_COUNT_QUEST_VARIABLE ) ) return;
				
		if( 0 != pPlayer->questVar[table].var.completeCount ) return;

		int	error = GTH_AutoQuestScroll_ObtainProcess( pPlayer, g_autoQuestscrollTable[idx].Index );
		
		if( error != ERROR_ITEMSYSTEM_NONE ) {
			GTH_SendMessage_AutoQuest_Error( pPlayer,error );
			return;
				}

		pPlayer->bautoeffectisvalid = TRUE;		

		int addItemData[5];
		addItemData[0] = AUTOQUESTSCROLL_ITEM_INDEX;		
		addItemData[1] = -1;								
		addItemData[2] = 1;									
		addItemData[3] = 1;									
		addItemData[4] = g_autoQuestscrollTable[idx].Index;	
		addItemData[5] = -1;								
			
		AI_NPC_AddItem( pPlayer, addItemData );
		GTH_SendMessage_AutoQuestScroll_ObtainOpen( pPlayer,g_autoQuestscrollTable[idx].Index );
		
		return;
		}
}


int AI_CheckItem( playerCharacter_t* pc, int* pPrarm )
{
	int itemIdx;
	int itemTableIdx=pPrarm[0];
	for( int i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		itemIdx = pc->inventory[i];
		if( itemIdx < 0 ) continue;

		if( pc->item[itemIdx].itemTableIdx == itemTableIdx ) return 1;
	}
	
	return 0;
}



void GTH_ProcessMessage_AutoQuestProgress()
{
	int type = MSG_ReadByte();
	int reply = 0;
	
 
	switch(type) {
	
	case AUTOQUESTSCROLL_START:
		{	
			
			if ( TRUE == GTH_IsOnPlayAutoQuestScroll( g_curPC) ) {
				
				GTH_SendMessage_AutoQuest_Error(g_curPC,ERROR_ITEMSYSTEM_26);
				reply = 0;
			}
			
			
			else if( ( g_curPC->oldEvent!=GTH_EV_CHAR_ATTACK && g_curPC->event != GTH_EV_CHAR_SKIP ) ) {
				
				
			  g_curPC->bautoeffectisvalid=true;  
			  reply=1;
			  
			} else { 
			  reply=0;
			}
		
			GTH_SendMessage_AutoQuestScroll_effectisvalid( g_curPC,reply,type);
	
		}
		
		
		break;
	case AUTOQUESTSCROLL_END:
		{
			g_curPC->bautoeffectisvalid=false; 
		}
	
		break;

	}
}























void GTH_ProcessMessage_ChaosValue()
{
	GTH_SendMessag_ChaosValue();
}



void GTH_ProcessMessage_SkinPart()
{
	BYTE    bSaveSQLDB = MSG_ReadByte();
	BYTE	bHairID = MSG_ReadByte();
	BYTE	bFaceID = MSG_ReadByte();
	g_curPC->skinPart[0]=bHairID;
	g_curPC->skinPart[1]=bFaceID;



	if(TRUE == bSaveSQLDB)	g_DBGameServer->SaveSkin(g_curPC);

}












BOOL isValidPCBusy(playerCharacter_t* pPlayer, int busystate)
{
	if ( pPlayer == NULL)
		return FALSE;

	if ( pPlayer->busyState != busystate)	
	{			
		
		g_logSystem->WriteToHackingLog("[HackingSystemType]:%d [AccountID]=%s, [UserID]=%s", busystate, pPlayer->userID, pPlayer->name);

		TransmitePacket_sc_disconnect(pPlayer);
		
		GTH_DisconnectPlayer(pPlayer,TRUE);

		return FALSE;
	}
	
	return TRUE;
}







void InitItemCraftSystem(playerCharacter_t *pPlayer)
{
	if ( pPlayer == NULL)
		return;

	
	for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		pPlayer->craftSourceInventory[i] = -1;
	}
	for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
	{
		pPlayer->craftResultInventory[i] = -1;
	}

}








void InitItemUpgradeSystem(playerCharacter_t *pPlayer)
{
	if ( pPlayer == NULL)
		return;

	
	for(int  i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		pPlayer->upgradeSourceInventory[i] = -1;
	}
	pPlayer->upgradeResultInventory = -1;

}






void InitBoothSystem(playerCharacter_t* pPlayer)
{
	if ( pPlayer == NULL)
		return;

	
	for(int  i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )	
		pPlayer->boothInventory[i].itemIdx = -1;	
}







void InitPrecocitySystem(playerCharacter_t* pPlayer)
{
	if ( pPlayer == NULL)	
		return;

	for (int nIdx =0; nIdx < MAX_PRECOCITY_SIZE; nIdx++ )
	{
		pPlayer->precocityTime[nIdx] = -1;
		pPlayer->precocityInventory[nIdx] = -1;
	}
}
