#include "../global.h"
extern i3socket_t gsSocket;


#include "../Tools/CTools.h"
extern CTools* gcpTools;

void GTH_ProcessMessage_RequestNpcChat()
{
	int idx, reply = 1, error = 1;
	idx = MSG_ReadShort();

	NPC_InitAI( g_curPC, idx );
	GTH_SendMessage_ReplyNpcChat( g_curPC, reply, error );
}

void GTH_ProcessMessage_RequestQuitNpcChat()
{
	AI_NPC_CloseMessage( (void*)g_curPC,NULL );
}

void GTH_ProcessMessage_RequestNpcSelectMessage()
{
	int reply = 1, error = 1;
	int curIdx = -1;
	
	curIdx = MSG_ReadByte();

	g_curPC->npcAi.pcReplyIdx = curIdx;
}

void GTH_ProcessMessage_RequestStore()
{

}

void GTH_ProcessMessage_RequestBuyItem()
{
	storeStruct_t *store;
	int storeItemIdx, itemIdx, npcIdx, storeIdx, inventoryPos, itemTableIdx, num;
	int reply = 1, error = 0, buycost;
	item_t buyItem;
	char generateID[20]; 
	item_t *upItem;
	int stackItemIdx;

	int bIsStack = false;
	int stackNumber = 1;

	storeIdx = g_curPC->storeIdx;
	npcIdx = g_curPC->npcIdx;

	
	if( storeIdx < 0 ) 
	{
		reply = 0;
		error = ERROR_BUYITEM_2;
		goto REPLY_BUYITEM;
	}
	
	
	
	
	





	 
	 

	store = &g_store[storeIdx];

	storeItemIdx = MSG_ReadShort();
	num = MSG_ReadShort();

	
	if( store->item[storeItemIdx].curNumber == 0 ) 
	{
		reply = 0;
		error = ERROR_BUYITEM_2;
		goto REPLY_BUYITEM;
	}

	memcpy( &buyItem, &store->item[storeItemIdx].item, sizeof( item_t ) );
	itemTableIdx = buyItem.itemTableIdx;

	
	if( itemTableIdx >= g_itemTableNumber || itemTableIdx < 0 || num <= 0 )
	{
		g_logSystem->Write( "Shop Error!! Index Out of Range : %s(%s), %s(%d)npc , BuyItem %s(%d), Table Index : %d", g_curPC->name, g_curPC->userID,
			g_monster[npcIdx].basic.name, npcIdx, buyItem.name, storeItemIdx, itemTableIdx );

		reply = 0;
		error = ERROR_BUYITEM_2;
		goto REPLY_BUYITEM;
	}

	buycost = store->item[storeItemIdx].price * num;

	if( buycost > g_curPC->curChargeSE )
	{
		reply = 0;
		error = ERROR_BUYITEM_1;
		goto REPLY_BUYITEM;
	}

	
	if( g_curPC->curItemWeight > g_curPC->calItemWeight )
	{
		reply = 0;
		error = ERROR_BUYITEM_5;
		goto REPLY_BUYITEM;
	}

	if( GTH_IsStackItem( &buyItem ) )
	{
		buyItem.durability = num - 1;

		bIsStack = true;
		stackNumber = num;
	}

	stackItemIdx = GTH_StackMyItem( g_curPC, &buyItem );
	if( stackItemIdx >= 0 )
	{
		upItem = &g_curPC->item[stackItemIdx];
		upItem->durability += buyItem.durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;

		g_DBGameServer->SaveItem( g_curPC, upItem, stackItemIdx );

		
		
		
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_BUY_STORE, stackNumber, buycost, buyItem.name, 
			GetItemInformationLog( bIsStack, buyItem.itemTableIdx, stackItemIdx, -1 ) );
		
	}
	
	else
	{
		
		inventoryPos = GTH_FindEmptyInventoryPos( g_curPC );
		if( inventoryPos < 0 )	
		{
			reply = 0;
			error = ERROR_BUYITEM_3;
			goto REPLY_BUYITEM;
		}

		
		itemIdx = GTH_AddMyItem( g_curPC, &buyItem );
		if (itemIdx == -1)	
		{
			reply = 0;
			error = ERROR_BUYITEM_3;
			goto REPLY_BUYITEM;
		}
		
		
		g_curPC->inventory[inventoryPos] = itemIdx;
		
		
		strcpy(generateID, "storeGenID");
		g_DBGameServer->AddItem(g_curPC, &buyItem, itemIdx, generateID);


		
		
		
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_BUY_STORE, stackNumber, buycost, buyItem.name, 
			GetItemInformationLog( bIsStack, buyItem.itemTableIdx, itemIdx, inventoryPos ) );
		
	}

	
	g_curPC->curChargeSE -= buycost;
	g_curPC->curItemWeight += g_itemTable[buyItem.itemTableIdx].weight;
	
	g_logSystem->Write("User [%s](%s) Buy '%s' Item, Pay : %dSE.", g_curPC->name, g_curPC->userID, g_itemTable[itemTableIdx].name, buycost );

REPLY_BUYITEM :

	if( reply && store->item[storeItemIdx].curNumber > 0 ) store->item[storeItemIdx].curNumber --;
	GTH_SendMessage_ReplyBuyItem( g_curPC, reply, error, &buyItem, storeItemIdx, store->item[storeItemIdx].curNumber );
}

void GTH_ProcessMessage_RequestSellItem()
{
	int reply = 1, error = 0;
	int itemIdx, itemTableIdx;
	int sellcost;
	item_t *item;

	int bIsStack = false, stackNumber = 1;

	itemIdx = g_curPC->mouseInventory;

	if( itemIdx < 0 )
	{
		reply = 0;
		error = ERROR_SELLITEM_1;

		g_logSystem->Write( "Shop Error!! : %s(%s), Mouse Inventory, Index Out of Range. %d", g_curPC->name, g_curPC->userID, g_curPC->mouseInventory );

		goto REPLY_SELLITEM;
	}

	item = &g_curPC->item[itemIdx];
	itemTableIdx = item->itemTableIdx;	

	if( itemTableIdx < 0 )
	{
		reply = 0;
		error = ERROR_SELLITEM_1;

		g_logSystem->Write( "Shop Error!! : %s(%s), Mouse Inventory, Index Out of Range. %d", g_curPC->name, g_curPC->userID, g_curPC->mouseInventory );

		goto REPLY_SELLITEM;
	}

	if( g_itemTable[itemTableIdx].stackFlag )
	{
		bIsStack = true;
		stackNumber = item->durability + 1;
	}

	sellcost = GTH_GetSellPrice( item );

	g_curPC->curChargeSE += sellcost;

	
	if( g_curPC->curChargeSE > MAX_OWN_NAK ) g_curPC->curChargeSE = MAX_OWN_NAK;

	g_curPC->curItemWeight -= g_itemTable[item->itemTableIdx].weight;

	
	
	
	g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
		LOG_CLASS_ITEM_SELL_STORE, stackNumber, sellcost, item->name, 
		GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, -1 ) );
	

	
	GTH_DeleteMyItem( g_curPC, item );
	g_curPC->mouseInventory = -1;

	
	g_DBGameServer->RemoveItem(g_curPC, itemIdx);

	
	g_logSystem->Write("User [%s](ID: %s) Sell Item '%s', Receive : %dSE", g_curPC->name, g_curPC->userID, g_itemTable[itemTableIdx].name, sellcost);

REPLY_SELLITEM:
	GTH_SendMessage_ReplySellItem( g_curPC, reply, error, itemIdx );
}

void GTH_ProcessMessage_Binding()
{
	int bindingIdx;
	int reply = 1, error = 0;
	int bindingCost = 0, type;
	int worldIdx;
	vec3_t position;

	type = MSG_ReadByte();
	bindingIdx = MSG_ReadByte();
	bindingCost = MSG_ReadLong();

	if( type == 0 )
	{
		bindingCost = g_binding[bindingIdx].cost;
		
		if( bindingIdx < 0 || bindingIdx > g_bindingNumber ) 
		{
			reply = 0;
			error = 0;
			GTH_SendMessage_ReplyBinding( g_curPC, reply, error, 1, bindingIdx, bindingCost );
			return;
		}
		if( g_curPC->curChargeSE < bindingCost )
		{
			reply = 0;
			error = 1;
			GTH_SendMessage_ReplyBinding( g_curPC, reply, error, 1, bindingIdx, bindingCost );
			return;
		}
		
		g_curPC->curChargeSE -= bindingCost;
		g_curPC->bindingIdx = bindingIdx;
		
		GTH_SendMessage_ReplyBinding( g_curPC, reply, error, 1, bindingIdx, bindingCost );
	}
	else
	{	
		if( bindingIdx < 0 || bindingIdx > g_bindingNumber ) 
		{
			reply = 0;
			error = 0;
			GTH_SendMessage_ReplyBinding( g_curPC, reply, error, 3, bindingIdx, bindingCost );
			return;
		}
		if( g_curPC->curChargeSE < bindingCost || bindingCost < 0 )
		{
			reply = 0;
			error = 1;
			GTH_SendMessage_ReplyBinding( g_curPC, reply, error, 3, bindingIdx, bindingCost );
			return;
		}
		worldIdx = g_binding[bindingIdx].worldIdx;
		vec_copy( g_binding[bindingIdx].pos, position );
		g_curPC->curChargeSE -= bindingCost;

		AI_NPC_CloseMessage( (void*)g_curPC,NULL );

		GTH_SendMessage_ReplyBinding( g_curPC, reply, error, 3, bindingIdx, bindingCost );
		GTH_TransportWorldWithNPC( g_curPC, worldIdx, position );
	}
}



enum
{
	EPACK_STORE_ERROR_CHARGE_SE=0,
	EPACK_STORE_ERROR_CHARGE_TYPE,
	EPACK_STORE_ERROR_EQUIP,
	EPACK_STORE_ERROR_CHARGE_COUNT,
	EPACK_STORE_ERROR_DELETE_EPK,
};


void GTH_ProcessMessage_EpackStore()
{
	int  type, storeIdx;
	int num;
	int reply = 1, error = 0;
	int value;

	int epackIdx;
	int itemTableIdx;
	int EpkRemainCount=0;

	storeStruct_t *store;

	type = MSG_ReadByte();
	num = MSG_ReadShort();

	storeIdx = g_curPC->storeIdx;

	
	if( storeIdx < 0 ) 
	{
		reply = 0;
		error = 0;
		goto REPLY_EPACKSTORE;
	}

	
	if(storeIdx < 0 || storeIdx >= MAX_STORES) return;

	store = &g_store[storeIdx];

	epackIdx = g_curPC->equipment[EQUIP_EPACK];
	if(epackIdx < 0 || epackIdx >= MAX_NUMBER_OF_OWNITEM)
	{
		
		reply = 0;
		error = EPACK_STORE_ERROR_EQUIP;
		goto REPLY_EPACKSTORE;
	}

	itemTableIdx = g_curPC->item[epackIdx].itemTableIdx;
	
	
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return;
	if( g_itemTable[itemTableIdx].epkChargeMethod == TypeItemTable_t::EPK_CHARGE_TYPE_ITEM)
	{
		
		reply = 0;
		error = EPACK_STORE_ERROR_CHARGE_TYPE;
		goto REPLY_EPACKSTORE;
	}

	
	if(g_itemTable[itemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_CHARGE)
	{
		EpkRemainCount = gcpTools->GetEpkChargeCount(&g_curPC->item[epackIdx]);
		if( EpkRemainCount <= 0 )
		{
			if((int)g_curPC->curChargeRA <=0 && (int)g_curPC->curChargeSA <=0 )
			{
				
				GTH_DeleteMyItem( g_curPC, &g_curPC->item[epackIdx] );
				g_curPC->equipment[EQUIP_EPACK] = -1;
				g_DBGameServer->RemoveItem( g_curPC, epackIdx );
				
				
				reply = 0;
				error = EPACK_STORE_ERROR_DELETE_EPK;
				goto REPLY_EPACKSTORE;			
			}
			else
			{
				
				reply = 0;
				error = EPACK_STORE_ERROR_CHARGE_COUNT;
				goto REPLY_EPACKSTORE;			
			}
			return;
		}
	}
	
	switch( type )
	{
	
	case 0 :
		
		value = (int)GetEpackRechargePrice_RA( g_curPC->onceRecoveryRA, store->chargeRACost, g_itemTable[itemTableIdx].level ) * num;
		if( value > g_curPC->curChargeSE )
		{
			reply = 0;
			error = EPACK_STORE_ERROR_CHARGE_SE;
			goto REPLY_EPACKSTORE;
		}
		g_curPC->curChargeSE -= value;
		g_curPC->curChargeRA += num;
		if( g_curPC->curChargeRA > g_curPC->maxChargeRA ) g_curPC->curChargeRA = g_curPC->maxChargeRA;
		break;
	
	case 1 :
		
		value = (int)GetEpackRechargePrice_SA( g_curPC->onceRecoverySA, store->chargeSACost, g_itemTable[itemTableIdx].level ) * num;
		if( value > g_curPC->curChargeSE )
		{
			reply = 0;
			error = EPACK_STORE_ERROR_CHARGE_SE;
			goto REPLY_EPACKSTORE;
		}
		g_curPC->curChargeSE -= value;
		g_curPC->curChargeSA += num;
		if( g_curPC->curChargeSA > g_curPC->maxChargeSA ) g_curPC->curChargeSA = g_curPC->maxChargeSA;
		break;
	}

	
	
	
	if(g_itemTable[itemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_MUCH_CAPACITY && g_itemTable[itemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_CHARGE)
	{
		EpkRemainCount--;
		gcpTools->SetEpkChargeCount(&g_curPC->item[epackIdx],EpkRemainCount);
		g_DBGameServer->SaveItem( g_curPC, &g_curPC->item[epackIdx], epackIdx);
	}

REPLY_EPACKSTORE :
	GTH_SendMessage_ReplyEpackStore( g_curPC, reply, error,EpkRemainCount  );
}




void GTH_ProcessMessage_AllRepairItem(int invenPos)
{
	int reply = 1, error = 0;
	int itemIdx, itemTableIdx, dur;
	int numOfRepairItem = 0, i;
	int repairItemIdx[NUM_EQUIP_POSITION], repairItemDur[NUM_EQUIP_POSITION];
	item_t *item;

	for( i = EQUIP_WEAPON_1; i < EQUIP_EPACK; i ++ )
	{
		itemIdx = g_curPC->equipment[i];
		if( itemIdx < 0 ) continue;
		item = &g_curPC->item[itemIdx];
		itemTableIdx = item->itemTableIdx;

		if( !GTH_IsRepairItem( item ) ) continue;
		
		GTH_GetRepairCost( item, g_curPC->curChargeSE, dur );
		if( dur == 0 ) continue;
		
		item->durability += dur;
		if( item->durability > g_itemTable[itemTableIdx].durability ) 
			item->durability = g_itemTable[itemTableIdx].durability;

		g_DBGameServer->SaveItem( g_curPC, item, itemIdx );

		repairItemIdx[numOfRepairItem] = itemIdx;
		repairItemDur[numOfRepairItem] = item->durability;

		numOfRepairItem ++;
	}

	GTH_SendMessage_Reply_MasicBook_RepairItem( g_curPC, reply, error,  repairItemIdx, repairItemDur, numOfRepairItem ,invenPos);
}


void GTH_ProcessMessage_RepairItem()
{
	int reply = 1, error = 0;
	int type, pos, itemIdx, itemTableIdx, dur;
	int cost, numOfRepairItem = 0, i;
	int repairItemIdx[NUM_EQUIP_POSITION], repairItemDur[NUM_EQUIP_POSITION];
	item_t *item;

	type = MSG_ReadByte();
	pos = MSG_ReadShort();

	if( pos < 0 ) return;

	switch( type )
	{
	case REPAIR_ITEM_SUB0 : case REPAIR_ITEM_SUB1 :
		if( type == 0 ) itemIdx = g_curPC->inventory[pos];
		else itemIdx = g_curPC->equipment[pos];

		if( itemIdx < 0 )
		{
			reply = 0;
			
			
			error = ERROR_REPAIRITEM_2;
			goto REPLY_REPAIR_ITEM;
		}

		item = &g_curPC->item[itemIdx];

		if( !GTH_IsRepairItem( item ) ) 
		{
			reply = 0;
			error = ERROR_REPAIRITEM_2;
			goto REPLY_REPAIR_ITEM;
		}
		
		if ( g_curPC->PremiumInfo.iMemberShipType == 
			playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free)
		{
			
			if ( item->durability < g_curPC->PremiumInfo.ItemDurability.iPrepareRequirdurability )
			{
				reply = 0;
				error = ERROR_REPAIRITEM_3;
				goto REPLY_REPAIR_ITEM;
			}
		}
		
		

		itemTableIdx = item->itemTableIdx;

		cost = GTH_GetRepairCost( item, g_curPC->curChargeSE, dur );
		
		
		cost = cost * g_curPC->PremiumInfo.ItemDurability.fRepareCostRate;
		
		if( g_curPC->curChargeSE < cost || dur == 0 )
		{
			reply = 0;
			error = ERROR_REPAIRITEM_1;
			goto REPLY_REPAIR_ITEM;
		}
		g_curPC->curChargeSE -= cost;
		item->durability += dur;
		if( item->durability > g_itemTable[itemTableIdx].durability ) 
			item->durability = g_itemTable[itemTableIdx].durability;

		g_DBGameServer->SaveItem( g_curPC, item, itemIdx );

		repairItemIdx[0] = itemIdx;
		repairItemDur[0] = item->durability;

		goto REPLY_REPAIR_ITEM;
		break;

	case REPAIR_ITEM_ALL :
		for( i = EQUIP_WEAPON_1; i < EQUIP_EPACK; i ++ )
		{
			itemIdx = g_curPC->equipment[i];
			if( itemIdx < 0 ) continue;
			item = &g_curPC->item[itemIdx];
			itemTableIdx = item->itemTableIdx;

			if( !GTH_IsRepairItem( item ) ) continue;
			
			cost = GTH_GetRepairCost( item, g_curPC->curChargeSE, dur );

			
			cost = cost * g_curPC->PremiumInfo.ItemDurability.fRepareCostRate;
			
			if( g_curPC->curChargeSE < cost || dur == 0 ) continue;

			g_curPC->curChargeSE -= cost;
			item->durability += dur;
			if( item->durability > g_itemTable[itemTableIdx].durability ) 
				item->durability = g_itemTable[itemTableIdx].durability;

			g_DBGameServer->SaveItem( g_curPC, item, itemIdx );

			repairItemIdx[numOfRepairItem] = itemIdx;
			repairItemDur[numOfRepairItem] = item->durability;

			numOfRepairItem ++;
		}
		goto REPLY_REPAIR_ITEM;

		break;
	}

REPLY_REPAIR_ITEM :

	GTH_SendMessage_ReplyRepairItem( g_curPC, reply, error, type, repairItemIdx, repairItemDur, numOfRepairItem );
}

void GTH_ProcessMessage_Request_Buy_Skill()
{
	storeStruct_t *store;
	int storeIdx, storeItemIdx, itemIdx, inventoryPos,  i;
	int reply = 1, error = 0, buySphereCost;
	int skillTableIdx, skillIdx;
	item_t *upItem;

	CSkilTable *pSkillTable;

	storeIdx = g_curPC->storeIdx;

	
	if( storeIdx < 0 ) 
	{
		reply = 0;
		error = ERROR_BUYSKILL_2;
		goto REPLY_BUYSKILL;
	}

	store = &g_store[storeIdx];

	storeItemIdx = MSG_ReadShort();

	skillTableIdx = store->item[storeItemIdx].item.itemTableIdx - STORE_SKILL_MASK;
	buySphereCost = store->item[storeItemIdx].price;

	if( skillTableIdx < 0 || skillTableIdx >= MAX_SKILL_NUMBER )
	{
		reply = 0;
		error = ERROR_BUYSKILL_2;
		
		g_logSystem->Write("User '%s'(ID: %s) Buy TableIdx : %d Skill, Index Out of Range.", g_curPC->name, g_curPC->userID, skillTableIdx );
		goto REPLY_BUYSKILL;
	}

	
	if( !GTH_CheckRequire( g_curPC, 1, &skillTableIdx ) )
	{
		reply = 0;
		error = ERROR_BUYSKILL_3;
		goto REPLY_BUYSKILL;
	}
	
	
	if( g_SkillLogic.SKILL_IsValid( g_curPC, skillTableIdx ) )
	{
		reply = 0;
		error = ERROR_BUYSKILL_4;
		goto REPLY_BUYSKILL;
	}
	
	for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		itemIdx = g_curPC->inventory[i];
		if( itemIdx < 0 ) continue;
		upItem = &g_curPC->item[itemIdx];
		if( upItem->itemTableIdx == ITEM_TABLE_IDX_GEN_SPHERE ) break;
	}
	
	if( i == MAX_INVENTORY_SIZE )
	{
		reply = 0;
		error = ERROR_BUYSKILL_1;
		goto REPLY_BUYSKILL;
	}
	if( upItem->durability + 1 < buySphereCost )
	{
		reply = 0;
		error = ERROR_BUYSKILL_1;
		goto REPLY_BUYSKILL;
	}
	inventoryPos = i;

	
	skillIdx = g_SkillLogic.SKILL_Add( g_curPC, skillTableIdx );
	
	if( skillIdx < 0 )
	{
		reply = 0;
		error = ERROR_BUYSKILL_5;
		goto REPLY_BUYSKILL;
	}

	
	upItem->durability -= buySphereCost;
	if( upItem->durability < 0 )
	{
		g_curPC->inventory[inventoryPos] = -1;

		
		
		
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_LEARN_SKILL, buySphereCost, upItem->name, 
			GetItemInformationLog( 0, upItem->itemTableIdx, itemIdx, -1 ) );
		

		GTH_DeleteMyItem( g_curPC, upItem );
		g_DBGameServer->RemoveItem( g_curPC, itemIdx );
	}
	else
	{
		
		
		
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_LEARN_SKILL, buySphereCost, upItem->name, 
			GetItemInformationLog( 1, upItem->itemTableIdx, itemIdx, -1 ) );
		

		g_DBGameServer->SaveItem( g_curPC, upItem, itemIdx );
	}
	
	
	pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL)
	{
		g_logSystem->Write("User '%s'(ID: %s) Buy '%s' Skill.", g_curPC->name, g_curPC->userID,"pSkillTable==NULL");
	}
	else
	{
		g_logSystem->Write("User '%s'(ID: %s) Buy Skill Table Index %d Skill.", g_curPC->name, g_curPC->userID,skillTableIdx);
	}	

REPLY_BUYSKILL :
	
	GTH_SendMessage_ReplyBuySkill( g_curPC, reply, error, inventoryPos, buySphereCost, skillIdx );
}


void GTH_ProcessMessage_Request_Buy_GambleItem()
{
	storeStruct_t *store;
	int storeItemIdx, itemIdx, npcIdx, storeIdx, inventoryPos, itemTableIdx, num;
	int reply = 1, error = 0, buycost;
	item_t buyItem;
	char generateID[20]; 
	item_t *upItem;
	int stackItemIdx;

	int bIsStack = false;
	int stackNumber = 1;
	int itemTblIdx = -1;

	storeIdx = g_curPC->storeIdx;
	npcIdx = g_curPC->npcIdx;	

	store = &g_store[storeIdx];

	storeItemIdx = MSG_ReadShort();
	num = MSG_ReadShort();	


	float r = 0.0f, accPro = 0.0f;
	r = GTH_Random();

	
	
	int selectidx = g_curPC->RegGambleItems[storeItemIdx];;
	
	
	
	accPro			= 0.0f;
	r				= GTH_Random();
	int OptionCount = 0;

	for(  int i = 0; i < MAX_GAMBLE_SELECTOPTION ; i ++ )
	{				
		if ( g_logic.gambleSelectOptionTable[i].SelectOptionRate <= 0.0)
			continue;

		accPro += (float) g_logic.gambleSelectOptionTable[i].SelectOptionRate * 0.01;
		if( r < accPro ) break;
		OptionCount++;
	}
	
	if ( OptionCount > 6) OptionCount = 6;	
	
	
	switch(g_itemTable[selectidx].rarity)
	{
		
		case 0:
			if ( OptionCount > 2 ) OptionCount = 2;
			break;

		case 1:
			if ( OptionCount > 3 ) OptionCount = 3;
			break;		
		
	}

	SpawnItem(&buyItem,selectidx, OptionCount, 1, 0, 1 );
	
	
	itemTableIdx = buyItem.itemTableIdx;

	
	if( itemTableIdx >= g_itemTableNumber || itemTableIdx < 0 || num <= 0 )
	{
		g_logSystem->Write( "Shop Error!! Index Out of Range : %s(%s), %s(%d)npc , BuyItem %s(%d), Table Index : %d", g_curPC->name, g_curPC->userID,
			g_monster[npcIdx].basic.name, npcIdx, buyItem.name, storeItemIdx, itemTableIdx );

		reply = 0;
		error = ERROR_BUYITEM_2;
		goto REPLY_BUYITEM;
	}	

	

	
	buycost = g_curPC->RegGamblePrice[storeItemIdx] * num;

	if( buycost > g_curPC->curChargeSE )
	{
		reply = 0;
		error = ERROR_BUYITEM_1;
		goto REPLY_BUYITEM;
	}

	
	if( g_curPC->curItemWeight > g_curPC->calItemWeight )
	{
		reply = 0;
		error = ERROR_BUYITEM_5;
		goto REPLY_BUYITEM;
	}

	if( GTH_IsStackItem( &buyItem ) )
	{
		buyItem.durability = num - 1;

		bIsStack = true;
		stackNumber = num;
	}

	stackItemIdx = GTH_StackMyItem( g_curPC, &buyItem );
	
	if( stackItemIdx >= 0 )
	{
		upItem = &g_curPC->item[stackItemIdx];
		upItem->durability += buyItem.durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;

		g_DBGameServer->SaveItem( g_curPC, upItem, stackItemIdx );

		
		
		
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_BUY_STORE, stackNumber, buycost, buyItem.name, 
			GetItemInformationLog( bIsStack, buyItem.itemTableIdx, stackItemIdx, -1 ) );
		
	}
	
	else
	{
		
		inventoryPos = GTH_FindEmptyInventoryPos( g_curPC );
		if( inventoryPos < 0 )	
		{
			reply = 0;
			error = ERROR_BUYITEM_3;
			goto REPLY_BUYITEM;
		}

		
		itemIdx = GTH_AddMyItem( g_curPC, &buyItem );
		if (itemIdx == -1)	
		{
			reply = 0;
			error = ERROR_BUYITEM_3;
			goto REPLY_BUYITEM;
		}
		
		
		


		
		g_curPC->inventory[inventoryPos] = itemIdx;
		
		
		strcpy(generateID, "storeGenID");
		g_DBGameServer->AddItem(g_curPC, &buyItem, itemIdx, generateID);


		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_BUY_STORE, stackNumber, buycost, buyItem.name, 
			GetItemInformationLog( bIsStack, buyItem.itemTableIdx, itemIdx, inventoryPos ) );
		
	}

	
	g_curPC->curChargeSE -= buycost;
	g_curPC->curItemWeight += g_itemTable[buyItem.itemTableIdx].weight;
	
	g_logSystem->Write("User [%s](%s) Buy '%s' Item, Pay : %dSE.", g_curPC->name, g_curPC->userID, g_itemTable[itemTableIdx].name, buycost );

REPLY_BUYITEM :

	if( reply && store->item[storeItemIdx].curNumber > 0 ) store->item[storeItemIdx].curNumber --;
	
	GTH_SendMessage_ReplyBuyItem( g_curPC, reply, error, &buyItem, storeItemIdx, store->item[storeItemIdx].curNumber );

}






void GTH_ProcessMessage_RepairItemConfirm()
{
	short PacketGroup = static_cast<enumExtendSecondPacket_ItemRepare>(MSG_ReadShort());
	if ( PacketGroup != CC_ITEM_REPAIR)
		return;

	enumRepairType repairType;	
	enumInvenType InvenType;	
	int pos			= 0;
	int itemIdxInPc	= 0;
	int reply		= 0;
	int	error		= ERROR_REPAIRITEM_2;
	int RepairItemCount = 0;
	int cost		= 0;
	int itemTableIdx= -1;
	item_t *pItem    = NULL;	
 

	repairType = static_cast<enumRepairType>(MSG_ReadByte());
	InvenType  = static_cast<enumInvenType>(MSG_ReadByte());
	pos		   = MSG_ReadByte();
	


	switch(repairType)
	{
	case REPAIR_ITEM_SUB0:
	case REPAIR_ITEM_SUB1:
		{			
			if( InvenType == INVENTYPE_INVENTORY ) 
				itemIdxInPc = g_curPC->inventory[pos];
			else itemIdxInPc = g_curPC->equipment[pos];
			
			if( itemIdxInPc < 0 )
			{
				reply = 0;
				error = ERROR_REPAIRITEM_2;
				goto REPLY_REPAIR_ITEM;
			}
			
			pItem = &g_curPC->item[itemIdxInPc];			
			
			if( !GTH_IsRepairItem( pItem ) ) 
			{
				reply = 0;
				error = ERROR_REPAIRITEM_2;
				goto REPLY_REPAIR_ITEM;
			}
			
			
			if ( g_curPC->PremiumInfo.iMemberShipType == 
				playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free)
			{
				
				if ( pItem->durability < g_curPC->PremiumInfo.ItemDurability.iPrepareRequirdurability )
				{
					reply = 0;
					error = ERROR_REPAIRITEM_3;
					goto REPLY_REPAIR_ITEM;
				}
			}
			
			
			
			if ( pItem->itemTableIdx < 0 )
			{
				reply = 0;
				error = ERROR_REPAIRITEM_2;
				goto REPLY_REPAIR_ITEM;
			}
			itemTableIdx = g_itemTable[pItem->itemTableIdx].idx;	
			cost = GTH_GetRepairCost( pItem, g_curPC->curChargeSE, RepairItemCount );	
			cost = cost * g_curPC->PremiumInfo.ItemDurability.fRepareCostRate;
			reply = 1;

			
		}break;
		
	case REPAIR_ITEM_ALL:
		{

			cost = 0;
			for( int i = EQUIP_WEAPON_1; i < EQUIP_EPACK; i ++ )
			{
				itemIdxInPc= g_curPC->equipment[i];
				if( itemIdxInPc < 0 ) continue;
				pItem = &g_curPC->item[itemIdxInPc];
				itemTableIdx = pItem->itemTableIdx;
				
				if( !GTH_IsRepairItem( pItem ) ) continue;
				
				int RepairCost = GTH_GetRepairCost( pItem, g_curPC->curChargeSE, RepairItemCount );				

				if ( g_curPC->PremiumInfo.iMemberShipType == 
					playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free)
				{
					
					if ( pItem->durability < g_curPC->PremiumInfo.ItemDurability.iPrepareRequirdurability )
					{
						reply = 0;
						error = ERROR_REPAIRITEM_4;
						goto REPLY_REPAIR_ITEM;
					}
				}
				RepairCost = RepairCost * g_curPC->PremiumInfo.ItemDurability.fRepareCostRate;

				if( g_curPC->curChargeSE < RepairCost || RepairItemCount == 0 ) continue;		
				cost += RepairCost;				
			}
			reply = 1;
		}
		break;
	}

REPLY_REPAIR_ITEM:
	GTH_SendMessage_Reply_RepairItemConfirm(g_curPC, reply, error, repairType, cost,  RepairItemCount, itemTableIdx);
	
}