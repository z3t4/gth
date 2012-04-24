 #include "../global.h"


#include "../Tools/CTools.h"
extern CTools* gcpTools;

#include "..\GolryunBattle\tagGolryunBattle.h"
#include "..\GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;

#include <assert.h>
#include "..\log.h"

#include "..\Terra\GiftCard\CGiftCard.h"
extern CGiftCard* gcpGiftCard;

#include "..\GolryunBattle\CGolryunBattle_Observer.h"
extern CGolryunBattle_Observer* gcpGolryunBattle_Observer;

extern int AI_NPC_ShareMoney( void *pPc, int *pParam );
 

AIFUNC AI_NPC_FUNC[] =
{
	
	{ AI_NPC_IsMessageWating	,	0 },							
	{ AI_NPC_SendMessage		,	1 },							
	{ AI_NPC_SendMessageButton	,	2 },							
	{ AI_NPC_GetMessageSelect	,	0 },							
	{ AI_NPC_CloseMessage		,	0 },							
	{ AI_NPC_OpenStore			,	1 },							
	{ AI_NPC_CloseStore			,	0 },							
	{ AI_NPC_CheckQuestStep		,	2 },							
	{ AI_NPC_SetQuestStep		,	2 },							
	{ AI_NPC_CheckQuestItem		,	3 },							
	{ AI_NPC_DeleteQuestItem	,	3 },							
	{ AI_NPC_SetQuestStatus		,	1 },							
	{ AI_NPC_ClearQuestStatus	,	1 },							
	{ AI_NPC_AddItem			,	4 },							
	{ AI_NPC_DeleteItem			,	2 },							
	{ AI_NPC_CompleteQuest		,	1 },							
	{ AI_NPC_OpenSkillLevelUp	,	1 },							
	{ AI_NPC_OpenBinding		,	1 },							
	{ AI_NPC_AddLoot			,	4 },							
	{ AI_SendChatMessage		,	1 },							
	{ AI_SetDynamicField		,	3 },							
	{ AI_GetDynamicField		,	2 },							
	{ AI_CheckPlayerClass		,	0 },							
	{ AI_CheckPlayerJob			,	0 },							
	{ AI_CheckPlayerLevel		,	0 },							
	{ AI_CheckPlayerGenLevel	,	0 },							
	{ AI_CheckPlayerStatus		,	1 },							
	{ AI_MoveToBindingPosition	,	2 },							
	{ AI_AwakenPlayerJob		,	1 },							
	{ AI_GetCompleteCount		,	1 },							
	{ AI_OpenDepot				,	0 },							
	{ AI_CloseDepot				,	0 },							
	{ AI_OpenPrecocity			,	0 },							
	{ AI_OpenCraft				,	0 },							
	{ AI_OpenUpgrade			,	0 },							
	{ AI_InitPlayerStatus		,	1 },							
	
	{ AI_OpenQuestScroll		,	0 },							
	
	{ AI_NPC_AddQuestItem		,	4 },							
	{ AI_NPC_FindEmptyInventoryPos, 0 },							
	{ AI_SendDialogMessage		,   0 },							
	{ AI_NPC_CheckAutoQuestType	,   0 },						    
	{ AI_OpenNpcInventory		,	0 },							
	
	{ AI_OpenGambleStore		,   1 },							
	{ AI_OpenRequestDanBattle	,   0 },							

	
	{ AI_OpenEventCraft			,	0 },							
	
	{ AI_GonryunBattle			,	0 },							

	
	{ AI_NPC_TransportGolryun	,	0 },							
	{ AI_NPC_TransportArayan	,	0 },							
	{ AI_NPC_SuggestGolryunBattle,	0 },							
	{ AI_NPC_BuyGolryunBattleTicket,0 },							
	{ AI_NPC_ShareMoney         ,   0 },							
	{ AI_NPC_DisplayTournament  ,   0 },							
	{ AI_NPC_CheckPrize		    ,   0 },							
	
	
	{ AI_OpenPackage			,	0 },							
	
	{ AI_helpOpen				,	0 },							
	{ AI_OpenGiftCardWin		,   0 },							
	
	{ AI_NPC_ObserveTournament  ,   0 },							
	
	{ AI_DanBattleEvent			,  0  },
	{ AI_DanBattlePcPoint		,  0  },

	
	
};


int AI_NPC_IsMessageWating( void* pPc, int *pParam )
{
	playerCharacter_t* pc = static_cast<playerCharacter_t*>(pPc);

	if( pc->npcAi.msgWaiting ) return( true );
	else return( false );
}

int AI_NPC_SendMessage( void* pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast<playerCharacter_t*>(pPc);
	int idx					= pParam[0];

	pc->npcAi.msgWaiting = true;
	pc->npcAi.msgFlag = true;

	pc->npcAi.sendMsgIdx = idx;
	pc->npcAi.pcReplyIdx = -1;

	return( true );
}

int AI_NPC_SendMessageButton( void* pPc, int *pParam )
{
	playerCharacter_t *pc	= static_cast<playerCharacter_t*>(pPc);
	int buttonType			= pParam[0];
	int idx					= pParam[1];

	if( buttonType + 1 > pc->npcAi.replyMsgIdxNumber )
		pc->npcAi.replyMsgIdxNumber = buttonType + 1;

	pc->npcAi.replyMsgIdx[buttonType] = idx;

	return( true );
}

int AI_NPC_GetMessageSelect( void* pPc, int *pParam )
{
	playerCharacter_t *pc	= static_cast<playerCharacter_t*>(pPc);

	if( pc->npcAi.pcReplyIdx >= 0 ) 
	{
		
		pc->npcAi.msgWaiting = false;
		return( pc->npcAi.pcReplyIdx );
	}
	else return( -1 );
}

int AI_NPC_CloseMessage( void* pPc, int *pParam )
{
	playerCharacter_t *pc	= static_cast<playerCharacter_t*>(pPc);

	if( pc->npcIdx < 0 ) return 0;

	NPC_DeleteNoticePC( &g_monster[pc->npcIdx], pc );
	GTH_SendMessage_ReplyQuitNpcChat( pc, 1, 1 );

	return( true );
}

int AI_NPC_OpenStore(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int storeIdx = pParam[0];

	GTH_SendMessage_ReplyStore( pc, &g_store[storeIdx] );
	pc->storeIdx = storeIdx;

	return 0;
}

int AI_NPC_CloseStore(void *pPc, int *pParam )
{
	return 1;
}

int AI_NPC_CheckQuestStep(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];
	int step = pParam[1];

	if( pc->questVar[questIdx].var.step == step ) return 1;

	return 0;
}


int AI_NPC_CheckAutoQuestType(void *pPc,int *pParam)
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
   	int idx=-1;

	
	for(int i=0; i<MAX_COUNT_QUEST_STATUS; i++)
	{
		
		if(pc->questStatus[i].questIdx == AUTOQUESTSCROLL_ITEM_INDEX )
		{
			idx=i;
			break;		
		}

	}
    
	
	if(idx>-1 && idx<MAX_COUNT_QUEST_STATUS)
	{
		
		if(pc->questStatus[idx].dField[5] != -1 )
		{
			
			if(pc->questStatus[idx].dField[5] == pc->npcIdx)
			{
				return 1;
			}
		
		}
	}
	return 0;
}

int AI_NPC_SetQuestStep(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];
	int step = pParam[1];

	pc->questVar[questIdx].var.step = step;
	
	
	g_DBGameServer->SaveQuest( pc );

	GTH_SendMessage_QuestStep( pc, questIdx, step, false );

	return 1;
}



int AI_NPC_AutoSetQuestType(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];
	int autoindex = pParam[1];
	
	pc->questVar[questIdx].var.step = autoindex;
    
	pc->questVar[questIdx].var.dummy = 0;
	pc->questVar[questIdx].var.isComplete=false;
	pc->questVar[questIdx].var.completeCount=1;

	
	g_DBGameServer->SaveQuest( pc );

	return 1;
}


int AI_NPC_CheckQuestItem(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int itemTableIdx = pParam[0];
	int	questIdx = pParam[1];
	int number = pParam[2];

	if( GTH_GetQuestItemCount( pc, itemTableIdx, questIdx ) == number ) return 1;
	
	return 0;
}

int AI_NPC_DeleteQuestItem(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int itemTableIdx = pParam[0];
	int	questIdx = pParam[1];
	int number = pParam[2];

	GTH_DeleteQuestItem( pc, itemTableIdx, questIdx, number );

	return 1;
}

int AI_NPC_SetQuestStatus(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];
	int idx;

	idx = GTH_GetEmptyQuestStatus( pc );

	if( idx < 0 ) return 0;
	GTH_SetQuestStatus( pc, idx, questIdx );

	return 1;
}

int AI_NPC_ClearQuestStatus(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];

	GTH_ClearQuestStatus( pc, questIdx, 0 );

	return 1;
}



int AI_NPC_AddItem(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );

	int		itemTableIdx	= pParam[0];	
	int		optionGroupIdx	= pParam[1];	
	int		itemNumber		= pParam[2];	
	int		messageFlag		= pParam[3];	
	int		stackItemIdx, inventoryPos, itemIdx;
	int		reply = 1, error = 0;
	int		bIsStack = false, stackNumber = 1;
	char	generateID[20];
	item_t	item, *upItem;

	
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return false;
	
	
	if( g_itemTable[itemTableIdx].idx < 0 ) return false;

	
	
	if( optionGroupIdx >= 0 )
	{
		SpawnItemByOptionGroup( &item, itemTableIdx, optionGroupIdx );
	}
	else
	{
		
		
		SpawnItem( &item, itemTableIdx );
	}
	
	
	
	if (itemTableIdx == MONSTERCARD_ITEM_INDEX )
	{
		
		if(itemNumber < 0 || itemNumber >= MAX_NUMBER_OF_SUMMON_MONSTER_TABLE) return false;

		item.optionIdx[0] = itemNumber;	
		item.timer = g_summonMonstertable[itemNumber].SummonTime;				
		item.optionNumber ++;		
	}	
	

	if( GTH_IsStackItem( &item ) )
	{
		item.durability = itemNumber - 1;
		
		bIsStack = true;
		stackNumber = itemNumber;
	}
	else if( g_itemTable[itemTableIdx].durability > 0 )
	{
		item.durability = 100;
	}

	
	
	if(g_itemTable[itemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_MUCH_CAPACITY)
	{
		gcpTools->SetEpkChargeCount(&item,g_itemTable[itemTableIdx].epkChargeCount);
		if(g_itemTable[itemTableIdx].epkChargeCount > 0)
		{
			item.optionNumber++;
		}
	}
	

	stackItemIdx = GTH_StackMyItem( pc, &item );
	if( stackItemIdx >= 0 )
	{
		upItem = &pc->item[stackItemIdx];
		upItem->durability += item.durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;
		
		g_DBGameServer->SaveItem( pc, upItem, stackItemIdx );
		
		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_ADD_QUEST, stackNumber, 0, upItem->name,
			GetItemInformationLog( bIsStack, upItem->itemTableIdx, stackItemIdx, -1 ) );
		
	}
	else
	{
		strcpy( item.name, g_itemTable[itemTableIdx].name );

		
		if (itemTableIdx == QUESTSCROLL_ITEM_INDEX)
		{
			item.optionIdx[0] = pParam[4];	
			item.optionIdx[1] = pParam[5];	

			item.optionNumber = 2;
		}
		
		
		if (itemTableIdx == AUTOQUESTSCROLL_ITEM_INDEX)
		{
			item.optionIdx[0] = pParam[4];	
			item.optionIdx[1] = pParam[5];	

			item.optionNumber = 2;
		}

		
		
		{
			CGolryunBattle_Data* cpData=gcpGolryunBattle->GetDataClassPointer();
			assert(NULL != cpData);

			CGolryunBattle_Common* cpCommon = gcpGolryunBattle->GetCommonClassPointer();
			assert(NULL != cpCommon);

			CGolryunBattle_Process* cpProcess = gcpGolryunBattle->GetProcessClassPointer();
			assert(NULL != cpProcess);

			
			if (tagGolryunBattle::TwoGyun_100000_Item_Index == itemTableIdx ||
				tagGolryunBattle::TwoGyun_1000000_Item_Index == itemTableIdx)
			{
				
				int cnt    = cpProcess->m_iTournamentIdx;
				int time   = cpData->m_iBattleGUID;
				int winNum = pParam[5];


				item.experience   = cpProcess->m_iTournamentGUID;
				item.optionIdx[0] = time;
				item.optionIdx[1] = g_golryunManagerTable[cnt].levelMin;
				item.optionIdx[2] = winNum;

				item.optionNumber = 3;

				
				g_logSystem->WriteToLog( 
					pc, 
					LOG_CLASS_GOLRYUN, 
					"%d;%d;%d;%d;%d;;", 
					CLogSystem::enumGolryunSubLog::BattleTicket,
					item.experience,		
					item.optionIdx[0],		
					item.optionIdx[1],   	
					item.optionIdx[2]);   	
			}

			if (itemTableIdx == tagGolryunBattle::Entrance_Ticket_Item_Index)
			{
			
				item.experience = cpData->m_EntranceTicket.iMaxAllocGUID++;	
				item.optionIdx[0] = cpData->m_iBattleGUID;		
				item.optionNumber = 1;

				
				g_logSystem->WriteToLog( 
					pc, 
					LOG_CLASS_GOLRYUN, 
					"%d;%d;%d;;;;", 
					CLogSystem::enumGolryunSubLog::EnterTicket,
					item.experience,
					item.optionIdx[0]);				
			}
		}
		
		
		
		inventoryPos = GTH_FindEmptyInventoryPos( pc );
		if( inventoryPos < 0 )	
		{
			
			int itemObjectIdx = GTH_SpawnItemObject( &item, pc->worldIdx, pc->position, LOOTTYPE_INDIVIDUAL, pc->idx );
		
			if ( itemObjectIdx < 0 )
			{
				g_logSystem->Write("Can't Obtain Item Error!!(pc name:%s, item index:%d, option group index:%d", pc->name, item.itemTableIdx, optionGroupIdx );
			}
			return 0;


		}
		
		itemIdx = GTH_AddMyItem( pc, &item );

		if (itemIdx < 0)	
		{
			
			int itemObjectIdx = GTH_SpawnItemObject( &item, pc->worldIdx, pc->position, LOOTTYPE_INDIVIDUAL, pc->idx );

			if ( itemObjectIdx < 0 )
			{
				g_logSystem->Write("Can't Obtain Item Error!!(pc name:%s, item index:%d, option group index:%d", pc->name, item.itemTableIdx, optionGroupIdx );
			}
			return 0;

		}
		
		
		pc->inventory[inventoryPos] = itemIdx;
		
		strcpy(generateID, "questGenID");
		g_DBGameServer->AddItem(pc, &item, itemIdx, generateID);

		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;%d;;;%s;%s", 
			LOG_CLASS_ITEM_ADD_QUEST, stackNumber, 0, item.name,
			GetItemInformationLog( bIsStack, item.itemTableIdx, itemIdx, inventoryPos ) );
		
	}

	
	if ( reply == 1 )
	{
		
		error = pParam[3];	
	}
	
	GTH_SendMessage_AddInvItem( pc, reply, error, &item );

	return 1;
}

int AI_NPC_DeleteItem(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int itemTableIdx = pParam[0];
	int itemNumber = pParam[1];

	int reply = 1, error = 0;
	int itemIdx, invenPos = -1, equipPos = -1;
	item_t *item;

	int bIsStack = false, stackNumber = 1;

	
	for( int i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		itemIdx = pc->inventory[i];
		if( itemIdx < 0 ) continue;

		if( pc->item[itemIdx].itemTableIdx == itemTableIdx ) break;
	}
	if( i == MAX_INVENTORY_SIZE )
	{
		
		for( i = 0; i < MAX_EQUIPMENT; i ++ )
		{
			itemIdx = pc->equipment[i];
			if( itemIdx < 0 ) continue;
			if( pc->item[itemIdx].itemTableIdx == itemTableIdx ) break;
		}
		if( i == MAX_EQUIPMENT ) 
		{
			GTH_SendMessage_DeleteInvItem( pc, reply, error, itemTableIdx, itemNumber );
			return 0;
		}

		equipPos = i;
	}
	else
	{
		invenPos = i;
	}
	item = &pc->item[itemIdx];

	if( g_itemTable[item->itemTableIdx].stackFlag )
	{
		bIsStack = true;
		stackNumber = itemNumber;
	}

	
	
	
	g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
		LOG_CLASS_ITEM_DEL_QUEST, stackNumber, item->name,
		GetItemInformationLog( bIsStack, item->itemTableIdx, itemIdx, -1 ) );
	

	if( GTH_IsStackItem( item ) )
	{
		if( item->durability + 1 < itemNumber )
		{
			reply = 0;
			error = 1;

			GTH_SendMessage_DeleteInvItem( pc, reply, error, itemTableIdx, itemNumber );
		}
		
		else if( item->durability + 1 == itemNumber )
		{
			if( invenPos >= 0 ) 
			{
				pc->inventory[invenPos] = -1;
			}
			else if( equipPos >= 0 ) 
			{
				pc->equipment[equipPos] = -1;
			}

			GTH_DeleteMyItem( pc, item );
			g_DBGameServer->RemoveItem( pc, itemIdx );
		}
		else
		{
			item->durability -= itemNumber;
			g_DBGameServer->SaveItem( pc, item, itemIdx );
		}
	}
	else
	{
		if( invenPos >= 0 ) 
		{
			pc->inventory[invenPos] = -1;
		}
		else if( equipPos >= 0 ) 
		{
			pc->equipment[equipPos] = -1;
		}
		
		GTH_DeleteMyItem( pc, item );
		g_DBGameServer->RemoveItem( pc, itemIdx );
	}

	GTH_SendMessage_DeleteInvItem( pc, reply, error, itemTableIdx, itemNumber );

	return 1;
}

int AI_NPC_CompleteQuest(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];
	
	pc->questVar[questIdx].var.isComplete = true;
	pc->questVar[questIdx].var.completeCount ++;
	
	if( pc->questVar[questIdx].var.completeCount > MAX_COUNT_QUEST_COMPLETE )
		pc->questVar[questIdx].var.completeCount = MAX_COUNT_QUEST_COMPLETE;


	
	g_DBGameServer->SaveQuest( pc );
	
	GTH_ClearQuestStatus( pc, questIdx, 1 );

	return 1;
}

int AI_NPC_OpenSkillLevelUp(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int genType = pParam[0];

	
	GTH_SendMessage_ReplyStore( pc, NULL, genType );

	return 1;
}

int AI_NPC_OpenBinding(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int bindingIdx = pParam[0];

	
	GTH_SendMessage_ReplyBinding( pc, 1, 0, 0, bindingIdx, g_binding[bindingIdx].cost );

	return 1;
}

int AI_NPC_AddLoot(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int exp = 0, genExp = 0, nak = 0, genCap = 0;

	exp = pParam[0];
	genExp = pParam[1];
	nak = pParam[2];
	genCap = pParam[3];

	
	GTH_PC_GetLoot( pc->idx, exp, genExp, nak, genCap, 0 );

	return 1;
}

int AI_SendChatMessage(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int idx = pParam[0];

	GTH_SendMessage_QuestLogic_ChatMsg( pc, idx,0 );

	return 1;
}

int AI_SetDynamicField(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];
	int fieldIdx = pParam[1];
	int value = pParam[2];
	
	return( GTH_SetQuestStatusDynamicField( pc, questIdx, fieldIdx, value ) );
}

int AI_GetDynamicField(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];
	int fieldIdx = pParam[1];

	return( GTH_GetQuestStatusDynamicField( pc, questIdx, fieldIdx ) );
}

int AI_CheckPlayerClass(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	return( pc->pcClass );
}

int AI_CheckPlayerJob(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	return( pc->pcJob );
}

int AI_CheckPlayerLevel(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	return( pc->level );
}

int AI_CheckPlayerGenLevel(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	return( pc->genLevel );
}

int AI_CheckPlayerStatus(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int statType = pParam[0];
	switch( statType )
	{
	case 0 :
		return( pc->calForce );
		break;
	case 1 :
		return( pc->calSoulpower );
		break;
	case 2 :
		return( pc->calAgility );
		break;
	case 3 :
		return( pc->calVitality );
		break;
	}
	return 0;
}

int AI_MoveToBindingPosition(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int bindingIdx = pParam[0];
	int bindingCost = pParam[1];

	
	switch(GetChaosClass(pc)) {
	
	case CHAOS_DISORDER:
		bindingCost=bindingCost*3;
		break;
	case CHAOS_MONSTER:
		bindingCost=bindingCost*5;
		break;
	}
	
	GTH_SendMessage_ReplyBinding( pc, 1, 0, 2, bindingIdx, bindingCost );

	return 1;
}

int AI_AwakenPlayerJob( void *pPc, int *pParam )
{
	playerCharacter_t*	pc		=	static_cast< playerCharacter_t* >( pPc );
	int					jobType =	pParam[0];
	int					genType =	-1;
	int					backGenCap[GEN_NUMBER];

	for( int i = 0; i < GEN_NUMBER; i ++ ) backGenCap[i] = -1;

	
	backGenCap[pc->pcClass] = pc->genCapability[pc->pcClass];


	
	switch( jobType )
	{
		case J_MENTALITY :
			break;			
		case J_WIND :
			break;
		case J_WATER :
			break;
		case J_EARTH:
			break;
		case J_DEVOTIONAL_KNIGHT :
			genType = 3;

			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 5;
			pc->vitality	+= 0;
			break;


		case J_ROMING_KNIGHT :
			genType = 1;
			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 0;
			pc->vitality	+= 5;
			break;		

		case J_PIECING_EYES :
			genType = 2;		
			pc->force		+= 0;
			pc->soulpower	+= 5;
			pc->agility		+= 0;
			pc->vitality	+= 0;
			break;
		case J_WINDY_ROMER :
			genType = 0;		
			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 0;
			pc->vitality	+= 5;
			break;
		case J_DESTROYER :
			genType = 0;		
			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 5;
			pc->vitality	+= 0;
			break;
		case J_GUARDIAN :
			genType = 3;			
			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 0;
			pc->vitality	+= 5;
			break;
		case J_PROPHECIER :
			genType = 1;		
			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 0;
			pc->vitality	+= 0;
			break;		
		case J_TRANSFORMER :
			genType = 1;			
			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 5;
			pc->vitality	+= 0;
			break;
		case J_DEFENDER :
			genType = 0;		
			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 0;
			pc->vitality	+= 5;
			break;
		case J_STRANGER :
			genType = 2;		
			pc->force		+= 0;
			pc->soulpower	+= 0;
			pc->agility		+= 0;
			pc->vitality	+= 0;
			break;
	}
	




	




	
	pc->pcJob = jobType;

	
	
	
	if( pc->genCapability[genType] < 0 ) backGenCap[genType] = 0;
	else backGenCap[genType] = pc->genCapability[genType];
	
	
	for( i = 0; i < GEN_NUMBER; i ++ )		
	{
		pc->genCapability[i] = backGenCap[i];
	}

	GTH_SendMessage_QuestLogic_Awaken( pc );
	GTH_PC_Calculate( pc );

	return 1;
}

int AI_GetCompleteCount( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int questIdx = pParam[0];
	return( pc->questVar[questIdx].var.completeCount );
}


int AI_OpenDepot( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	GTH_SendMessage_ItemSystem_DepotOpen( pc );	
	return 1;
}

int AI_CloseDepot( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );	

	return 1;
}


int AI_OpenPrecocity( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	GTH_SendMessage_ItemSystem_PrecocityOpen( pc );

	return 1;
}

int AI_OpenCraft( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	AI_NPC_CloseMessage( pc, pParam );
	
	for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		pc->craftSourceInventory[i] = -1;
	}
	for( i = 0; i < MAX_CRAFT_RESULT_ITEM_SIZE; i ++ )
	{
		pc->craftResultInventory[i] = -1;
	}
	
	GTH_AddNoticePC( pc, -1, -1, BUSY_STATE_CRAFT );
	GTH_SendMessage_ItemSystem_CraftOpen( pc );
	
	return 1;
}

int AI_OpenEventCraft( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	AI_NPC_CloseMessage( pc, pParam );
	
	for( int i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		pc->craftSourceInventory[i] = -1;
	}

	
	GTH_AddNoticePC( pc, -1, -1, BUSY_STATE_CRAFT );
	GTH_SendMessage_ItemSystem_EventCraftOpen( pc );
	
	return 1;
}



int AI_OpenNpcInventory(void *pPc, int *pParam)
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	AI_NPC_CloseMessage( pc, pParam );
	

	for( int i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ )
	{
		pc->npcAutoSourceInventory[i] = -1;
	}

	
	GTH_AddNoticePC( pc, -1, -1, BUSY_STATE_NPC_INVENTORY );
	GTH_SendMessage_ItemSystem_NpcInventoryOpen( pc );

	return 1;
}

int AI_OpenUpgrade( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	AI_NPC_CloseMessage( pc, pParam );
	
	for( int i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		pc->upgradeSourceInventory[i] = -1;
	}
	pc->upgradeResultInventory = -1;
	
	
	GTH_AddNoticePC( pc, -1, -1, BUSY_STATE_UPGRADE );
	GTH_SendMessage_ItemSystem_UpgradeOpen( pc );
	
	return 1;
}

int AI_InitPlayerStatus( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int nak;
	int level = pc->level;

	
	nak = ( 100 * level * level * level + 50 * level * level ) / 8;
	
	pc->npcAi.globalData[0] = nak;

	
	GTH_SendMessage_QuestLogic_InitStatus( pc, 1, 0, 0, nak );

	return 1;
}


int AI_OpenQuestScroll( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	AI_NPC_CloseMessage( pc, pParam );

	if( pc->level > 10 )
	{
		
		GTH_AddNoticePC( pc, -1, -1, BUSY_STATE_QUESTSCROLL );
		GTH_SendMessage_QuestScroll_ObtainOpen( pc, 1 );
	}
	else
	{
		GTH_SendMessage_QuestScroll_ObtainOpen( pc, 0 );
	}
	
	return 1;
}



int AI_NPC_AddQuestItem(void *pPc,int *pParam)
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	int itemTableIdx = pParam[0];
	int	questIdx = pParam[1];
	int number = pParam[2];
    int maxnum = pParam[3];

	GTH_AddQuestItem(pc,itemTableIdx,questIdx,number,maxnum);

	return 1;
}



int AI_NPC_FindEmptyInventoryPos(void *pPc,int *pParam)
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	int inventoryPos=-1;
	return inventoryPos = GTH_FindEmptyInventoryPos( pc );
}


   
int AI_SendDialogMessage(void *pPc,int *pParam)
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );

  
	GTH_SendMessage_QuestLogic_ChatMsg( pc, 1, 1 );
	return 1;
}


int AI_OpenGambleStore( void *pPc, int *pParam )
{

	

	
	
	
	
	


	

	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );	
	int storeIdx = pParam[0];			
	storeStruct_t Makestore;
	if ( FALSE == g_GambleSystem.MakeGambleStore(&Makestore, &g_store[storeIdx], pc) )
	{
		GTH_SendMessage_ChattingMsg(pc, 0,  9, 193 );
		return -1;	
	}
		
	GTH_SendMessage_ReplyStore( pc, &Makestore, 2 );
	pc->storeIdx = storeIdx;	
	return 1;
	


}



int AI_OpenRequestDanBattle( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	
	int ret =  g_DanBattleFunc.RecvCC_ReqeustBattle(pc);
	
	if ( ret <= 0)
	{
		int lpackIdx = 0;
		switch ( ret )
		{
		case 0:
			lpackIdx = 60;
			break;
		case -1:
			lpackIdx = 59;
			break;			
		case -2:
			
			lpackIdx = 74;
			break;
		case -3:
			lpackIdx = 60;
			break;
		}
		GTH_SendMessage_ChattingMsg(pc, 0, 9, lpackIdx );		
		return -1;
	}	
	

	return 1;
}

#include "..\GonryunPracticeBattle\CGonryunPracticeBattle.h"
extern CGonryunPracticeBattle	g_GonryunPracticeBattle;
#include "..\GonryunPracticeBattle\CGonryunPractice_Encoder.h"
#include "..\GonryunPracticeBattle\CGonryunPractice_ProcessCtrl.h"



int AI_GonryunBattle(void *pPc, int *pParam)											
{	
	
	playerCharacter_t* pPlayer	= static_cast< playerCharacter_t* >( pPc );
	if((NULL == pPlayer) || (FALSE == pPlayer->active)) return 0;

	return g_GonryunPracticeBattle.NPCClick(pPlayer);
}




int AI_OpenPackage(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );	
	g_DBGameServer->LoadPackageList(pc);
	return 1;
}


int AI_NPC_TransportGolryun( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	

	
	
	(gcpGolryunBattle->GetEncoderClassPointer())->SendTrasportGolryun(&pc->sock);
	GTH_DeleteNoticePC(pc);

	return 1;
}

int AI_NPC_TransportArayan( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	

	(gcpGolryunBattle->GetEncoderClassPointer())->SendTrasportArayan(&pc->sock);
	GTH_DeleteNoticePC(pc);

	return 1;
}

int AI_NPC_SuggestGolryunBattle( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	

	BYTE type=0;
	
	
	switch((gcpGolryunBattle->GetProcessClassPointer())->GetTournamentStatus())
	{
	case CGolryunBattle_Process::eTournamentStep::TournamentStep_START_JOIN:
		type = 1;
		break;
	case CGolryunBattle_Process::eTournamentStep::TournamentStep_CLOSE_JOIN:
		type = 2;
		break;
	}


	(gcpGolryunBattle->GetEncoderClassPointer())->SendSuggestBattle(&pc->sock, type);
	GTH_DeleteNoticePC(pc);

	return 1;
}

int AI_NPC_BuyGolryunBattleTicket( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	




	
	(gcpGolryunBattle->GetEncoderClassPointer())->SendOpenBattleTicket(&pc->sock, 1);
	GTH_DeleteNoticePC(pc);

	return 1;
}

int AI_NPC_DisplayTournament( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	

	(gcpGolryunBattle->GetEncoderClassPointer())->SendDisplayTournament(&pc->sock);
	GTH_DeleteNoticePC(pc);


	return 1;
}

int AI_NPC_CheckPrize( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	

	(gcpGolryunBattle->GetEncoderClassPointer())->SendCheckPrize(&pc->sock);
	GTH_DeleteNoticePC(pc);

	return 1;
}




int AI_NPC_ObserveTournament( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	

	if((TRUE == pc->GonryunBattle.Observer.bActive) ||
	   (pc->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index)){

		(gcpGolryunBattle->GetEncoderClassPointer())->SendObserveStart(pc,FALSE);
		GTH_DeleteNoticePC(pc);
		return 1;
	}

	pc->GonryunBattle.Observer.bActive = gcpGolryunBattle_Observer->Insert(pc);
	if(TRUE == pc->GonryunBattle.Observer.bActive){
		(gcpGolryunBattle->GetEncoderClassPointer())->SendObserveStart(pc,TRUE);
		(gcpGolryunBattle->GetProcessClassPointer())->FindOutFighterNumber();
	}else{
		(gcpGolryunBattle->GetEncoderClassPointer())->SendObserveStart(pc,FALSE);
	}
	
	GTH_DeleteNoticePC(pc);

	return 1;
}



int AI_helpOpen( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	if((NULL == pc) || (FALSE == pc->active)) return 0;


	return 1;
}


int AI_OpenGiftCardWin( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );	
	
	
	gcpGiftCard->TransmitePacket_SC_Open(pc);

	return 1;
}





int AI_DanBattleEvent(void *pPc, int *pParam)
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );	
	
	
	int ChoasClass = GetChaosClass(pc);	

	
	int KillPoint = pc->nAccumulationBattleKillPoint;

	
	int decKillpoint =  10;
	int NextChaosPoint = 0;
	
	switch(ChoasClass)
	{	
		
	case  CHAOS_PURE:	
		
	case CHAOS_GENERAL:	
		{		
			
			GTH_SendMessage_ChattingMsg(pc, 0,  9, 657 );		
			return 1;
		}
		break;
		
	case CHAOS_DISORDER:NextChaosPoint = CHAOSGENERATE;	break;

		
	case CHAOS_MONSTER:	NextChaosPoint = CHAOSDISORDER;	break;
		
	case CHAOS_ATROCITY:NextChaosPoint = CHAOSMONSTER;	break;
	}

	
	if ( (KillPoint - decKillpoint) < 0 )
	{
		GTH_SendMessage_ChattingMsg(pc, 0,  9, 656);		
		return 2;
	}

	
	pc->nAccumulationBattleKillPoint -= decKillpoint;
	g_DBGameServer->Save_danbattle_pcpoint(pc,  -(decKillpoint) );
	pc->chaosPoint = NextChaosPoint;
	GTH_SendMessage_ChattingMsg(pc, 0,  9, 655);
	GTH_SendMessage_ChaoPointEvent(pc, decKillpoint);
	return 1;
}

int AI_DanBattlePcPoint(void *pPc, int *pParam)
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );	
	GTH_SendMessage_ReplyCheatCode(pc, CHEATCODE_DANBATTLE_PCPOINT);
	return 1;
}		
