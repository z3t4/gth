



#include "../Global.h"

CUseItem g_UseItem;
extern CItemTableData gItemTableData;





CUseItem::CUseItem()
{

}

CUseItem::~CUseItem()
{

}

int CUseItem::DefaultUsetItem(item_t *item,int &invenPos,int &reply,int &error,int &itemIdx,int &raCount,int &saCount)
{			
	int itemTableIdx = item->itemTableIdx;
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return 0;

	int chargeRA, chargeSA;
	chargeRA = g_itemTable[itemTableIdx].supplyRA;
	chargeSA = g_itemTable[itemTableIdx].supplySA;

	if ( g_curPC->PremiumInfo.bItemRecycle == FALSE )
	{
		reply = 0;
		error = ERROR_DESTROYITEM_11;	
		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;
	}

	
	
	storeStruct_t	*store;
	store = &g_store[g_curPC->storeIdx];
	int valueRa = (int)GetEpackRechargePrice_RA( g_curPC->onceRecoveryRA, store->chargeRACost, g_itemTable[itemTableIdx].level );
	int valueSa = (int)GetEpackRechargePrice_SA( g_curPC->onceRecoverySA, store->chargeSACost, g_itemTable[itemTableIdx].level );

	if( g_curPC->onceRecoveryRA > 0 ) 
		raCount = g_itemTable[itemTableIdx].sellCost * 3/10 * chargeRA * 0.01 / valueRa;
	else raCount = 0;

	if( g_curPC->onceRecoverySA > 0 ) 
		saCount = g_itemTable[itemTableIdx].sellCost * 3/10 * chargeSA * 0.01 / valueSa;
	else saCount = 0;

	return 1;
}

int CUseItem::MonsterCardItem(item_t *item,int &invenPos,int &reply,int &error,int &itemIdx)
{
	
	
	if ( invenPos == -1 )	return 0;
	
	if( !CanSummonMonster(g_summonMonstertable[item->optionIdx[0]].idx))
	{
		g_logSystem->Write("Summonning Failure!! : This Map Can Not Summon.");

		reply = 0;
		error = ERROR_DESTROYITEM_5;
		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;
	}

	
	int ret =  GTH_ProcessMessage_SummonMonster(item);	
	if (ret < 0)
	{
		g_logSystem->Write("Summonning Failure!! : This Monster Count Is Full.");					
		reply = 0;				

		if ( ret == -1)
			error = ERROR_DESTROYITEM_4;				

		if ( ret == -2)
			error = ERROR_DESTROYITEM_5;				

		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;
	}		
				
	if( invenPos != -1 )
	{
		g_curPC->inventory[invenPos] = -1;
		error = invenPos;			
	}
	
	return 1;
}

int CUseItem::AutoQuestScrollItem(item_t *item,int &invenPos,int &reply,int &error,int &itemIdx)
{
	
	
	
	
	if ( invenPos == -1 )		return 0;
	
	int itemTableIdx = item->itemTableIdx;
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return 0;

	int idx;
	int dField[MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD];
	memset( dField, 0, sizeof(dField) );
	
	idx = GTH_GetEmptyQuestStatus( g_curPC );	
	
	if( idx >= 0 )
	{
		
		if( item->optionIdx[0] < 0 )
		{
			g_logSystem->Write("Auto Quest Scroll Item has invalidate data! (User: %s, Item Table Index : %d)", g_curPC->name, itemTableIdx );
			reply = 0;
			error = ERROR_DESTROYITEM_10;	
			GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
			return 0;
		}
		
		dField[0] = g_autoQuestscrollTable[item->optionIdx[0]].Index;			
		dField[1] = g_autoQuestscrollTable[item->optionIdx[0]].dataField[0];	
		dField[2] = g_autoQuestscrollTable[item->optionIdx[0]].dataField[1];	
		dField[3] = 0;															
		dField[4] = g_autoQuestscrollTable[item->optionIdx[0]].dataField[5];	
		dField[5] = g_autoQuestscrollTable[item->optionIdx[0]].dataField[2];	
		dField[6] = g_autoQuestscrollTable[item->optionIdx[0]].dataField[6];	
		dField[7] = g_autoQuestscrollTable[item->optionIdx[0]].dataField[7];	
		dField[8] = g_autoQuestscrollTable[item->optionIdx[0]].dataField[3];	
		dField[9] = g_autoQuestscrollTable[item->optionIdx[0]].dataField[4];	
		
		int limittime=g_autoQuestscrollTable[item->optionIdx[0]].dataField[4];
		GTH_AutoSetQuestStatus( g_curPC, idx, AUTOQUESTSCROLL_ITEM_INDEX, limittime, 0, dField );
		
		if( invenPos != -1 )
		{
			g_curPC->inventory[invenPos] = -1;
			error = invenPos;			
		}
	}
	else
	{
		reply = 0;
		error = ERROR_DESTROYITEM_7;	
		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;
	}

	return 1;
}

int CUseItem::QuestScrollItem(item_t *item,int &invenPos,int &reply,int &error,int &itemIdx)
{
	
	if ( invenPos == -1 )	return 0;

	int itemTableIdx = item->itemTableIdx;
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return 0;

	int idx;
	int dField[MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD];
	memset( dField, 0, sizeof(dField) );

	int diffLevel;

	
	if ( g_questscrollTable[item->optionIdx[0]].data.level > 10 )
		diffLevel = g_curPC->level - g_questscrollTable[item->optionIdx[0]].data.level;
	else
		diffLevel = 0;

	int ret = 0;
	switch ( g_questscrollTable[item->optionIdx[0]].data.scrollType )
	{
	case 1 :
		if ( g_curPC->pcClass != GEN_MENTALITY && g_curPC->pcClass != GEN_WIND )
			ret = 1;
		break;
	case 2 :
		if ( g_curPC->pcClass != GEN_WATER && g_curPC->pcClass != GEN_EARTH )
			ret = 1;
		break;
	default :
		break;
	}

	if ( ret == 1 )
	{
		reply = 0;
		error = ERROR_DESTROYITEM_9;	
		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;
	}
	if ( diffLevel < 0 || diffLevel > 2 )
	{
		reply = 0;
		error = ERROR_DESTROYITEM_8;	
		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;

	}
	if ( GTH_IsOnPlayQuestScroll( g_curPC ) )
	{
		reply = 0;
		error = ERROR_DESTROYITEM_6;	
		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;

	}

	idx = GTH_GetEmptyQuestStatus( g_curPC );	

	if( idx >= 0 )
	{
		

		if( item->optionIdx[0] < 0 )
		{
			g_logSystem->Write("Quest Scroll Item has invalidate data! (User: %s, Item Table Index : %d)", g_curPC->name, itemTableIdx );
			reply = 0;
			error = ERROR_DESTROYITEM_10;	
			GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
			return 0;
		}

		dField[0] = g_questscrollTable[item->optionIdx[0]].Index;					
		dField[1] = g_questscrollTable[item->optionIdx[0]].data.monsterIndex;		
		dField[2] = g_questscrollTable[item->optionIdx[0]].data.monsterNo;			
		dField[3] = 0;																
		dField[4] = g_questscrollTable[item->optionIdx[0]].data.scrollType;			
		dField[5] = item->optionIdx[1];												
		dField[6] = g_questscrollTable[item->optionIdx[0]].data.rewardField[item->optionIdx[1]];	
		dField[7] = g_questscrollTable[item->optionIdx[0]].data.fame;				
		dField[8] = g_questscrollTable[item->optionIdx[0]].data.reward.itemNo;		
		dField[9] = g_questscrollTable[item->optionIdx[0]].data.reward.optionGroup;	

		if( g_questscrollTable[item->optionIdx[0]].data.rewardField[dField[5]] < 0 )
		{
			dField[5] = FindRewardField( item->optionIdx[0] );
			dField[6] = g_questscrollTable[item->optionIdx[0]].data.rewardField[dField[5]];
			if ( dField[5] < 0 )
			{
				
				g_logSystem->Write("Quest Scroll Table has an Error! (User: %s, Index : %d)", g_curPC->name, dField[0] );
				reply = 0;
				error = ERROR_DESTROYITEM_10;	
				GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
				return 0;

			}
		}

		GTH_SetQuestStatus( g_curPC, idx, QUEST_SCROLL_INDEX, -1, 0, dField );

		
		if( invenPos != -1 )
		{
			g_curPC->inventory[invenPos] = -1;
			error = invenPos;			
		}
	}
	else
	{
		reply = 0;
		error = ERROR_DESTROYITEM_7;	
		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;
	}

	return 1;

}

int CUseItem::RecyleItem(item_t *item,int &raCount,int &saCount,int &reply,int &error,int &itemIdx,int invenPos)

{
	
	
	int number = item->durability + 1;	
	int level = item->reqLevel;

	int itemTableIdx = item->itemTableIdx;
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return 0;


	if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_RA_CHARGE))
	{
		int iVal = gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_RA_CHARGE);
		raCount = number * iVal;
	}
	else if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_SA_CHARGE))
	{
		int iVal = gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_SA_CHARGE);
		saCount = number * iVal;
	}
	else if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_LEVEL_EXP_AND_GENEXP_MULTIPLY))
	{
		int iExpVal= (int)gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_LEVEL_EXP_MULTIPLY);
		int iGenExpVal= (int)gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_LEVEL_GENEXP_MULTIPLY);

		int exp, genExp;
		exp			= level * iExpVal;
		genExp		= level * iGenExpVal;

		
		
		
		GTH_PC_GetLoot( g_curPC->idx, exp, genExp, 0, 0 ,FALSE,TRUE);
	}
	else if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_GEN_CAPABLITY))
	{
		int iVal = gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_GEN_CAPABLITY);

		int genCap;
		genCap = level * iVal;
		
		
		
		GTH_PC_GetLoot( g_curPC->idx, 0, 0, 0, genCap ,FALSE,TRUE);
	}
	else if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_DESTROYITEM))
	{
		reply = 0;
		error = ERROR_DESTROYITEM_3;
		return -1;
	}
	else if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RUN_SKILL_ITEM))
	{
		int SkillIndex = gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RUN_SKILL_INDEX);
		int AddLevel = gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RUN_SKILL_ADD_LEVEL);

		g_PcSkill.GTH_PC_SkillNotOwn( g_curPC,CSkillItem::SELECT_ITEM, invenPos,SkillIndex, level + AddLevel, 0);
	}
	
	return 1;
}

int	CUseItem::AutoQuestItemType(item_t *item,int &invenPos,int &raCount,int &saCount,int &bIsStack,int &stackNumber,int &reply,int &error,int &itemIdx)
{
	int itemTableIdx = item->itemTableIdx;
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return 0;

	
	if(g_itemTable[itemTableIdx].etcItemType == ETCITEM_TYPE_AUTOQUESTITEM)
	{
		if ( invenPos == -1 )	return 0;
		
		int idx;
		int dField[MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD];
		memset( dField, 0, sizeof(dField) );
		
		idx = GTH_GetEmptyQuestStatus( g_curPC );	
		
		if( idx >= 0 )
		{
			
			int questIndex=-1;
			
			for(int index=0; index<4; index++)
			{
				if(questIndex!=-1)break;
				for(int i=0; i<g_autoQuestscrollTableNumber; i++)
				{
					if(g_itemTable[itemTableIdx].commonField[index] == g_autoQuestscrollTable[i].Index)
					{
						
						if(g_autoQuestscrollTable[i].data.reward.regClass ==-1||g_curPC->pcClass == g_autoQuestscrollTable[i].data.reward.regClass)
						{
							questIndex=i;
							break;
						}
						
					}
				}
			}
		
		 
			if(index == 4)
			{
				
				return 0;
			}
			
			dField[0] = g_autoQuestscrollTable[questIndex].Index;			
			dField[1] = g_autoQuestscrollTable[questIndex].dataField[0];	
			dField[2] = g_autoQuestscrollTable[questIndex].dataField[1];	
			dField[3] = 0;															
			dField[4] = g_autoQuestscrollTable[questIndex].dataField[5];	
			dField[5] = g_autoQuestscrollTable[questIndex].dataField[2];	
			dField[6] = g_autoQuestscrollTable[questIndex].dataField[6];	
			dField[7] = g_autoQuestscrollTable[questIndex].dataField[7];	
			dField[8] = g_autoQuestscrollTable[questIndex].dataField[3];	
			dField[9] = g_autoQuestscrollTable[questIndex].dataField[4];	
			
			int limittime=g_autoQuestscrollTable[questIndex].dataField[4];
			GTH_AutoSetQuestStatus( g_curPC, idx, AUTOQUESTSCROLL_ITEM_INDEX, limittime, 0, dField );
			
			if( invenPos != -1 )
			{
				g_curPC->inventory[invenPos] = -1;
				error = invenPos;			
			}
		}
		else
		{
			reply = 0;
			error = ERROR_DESTROYITEM_7;	
			GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
			return 0;
		}
		
		g_curPC->curChargeRA += raCount;
		if( g_curPC->curChargeRA > g_curPC->maxChargeRA ) g_curPC->curChargeRA = g_curPC->maxChargeRA;
		g_curPC->curChargeSA += saCount;
		if( g_curPC->curChargeSA > g_curPC->maxChargeSA ) g_curPC->curChargeSA = g_curPC->maxChargeSA;
		
		g_curPC->curItemWeight -= g_itemTable[item->itemTableIdx].weight;
		
		if( g_itemTable[item->itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = item->durability + 1;
		}
		
		
		
		
		g_logSystem->WriteToLog( g_curPC, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_DESTROY, stackNumber, item->name, 
			GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, -1 ) );
		
		
		g_logSystem->Write( "Item Destroy : %s(%s), item : %s Destroyed", g_curPC->name, g_curPC->userID, item->name );
		
		
		GTH_DeleteMyItem( g_curPC, item );
		g_curPC->mouseInventory = -1;
		
		g_DBGameServer->RemoveItem(g_curPC, itemIdx);
		GTH_SendMessage_DestroyItem( g_curPC, reply, error, itemIdx );
		return 0;
	}

	return 1;
	
}

