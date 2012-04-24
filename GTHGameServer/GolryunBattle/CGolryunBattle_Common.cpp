



#include "..\global.h"
#include "CGolryunBattle_Common.h"
#include "..\TableStruct\TableStruct.h"


#include "..\GolryunBattle\tagGolryunBattle.h"
#include "..\GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;

#include "..\Tools\CTools.h"
extern CTools* gcpTools;





CGolryunBattle_Common::CGolryunBattle_Common()
{
	m_bOK=FALSE;

	m_bOK=TRUE;

}

CGolryunBattle_Common::~CGolryunBattle_Common()
{

}

BOOL CGolryunBattle_Common::isOK(void) const
{
	return m_bOK;
}

itemTable_t* CGolryunBattle_Common::GetItemTablePointer(const int Idx)
{
	if(Idx < 0) return NULL;
	if(Idx >= MAX_NUMBER_OF_ITEM_TABLE) return NULL;
	return &g_itemTable[Idx];
}



playerCharacter_t* CGolryunBattle_Common::
GetPlayer(const CGolryunBattle_Data::tagFighter& Fighter)
{
	playerCharacter_t* pTempPlayer= gcpTools->GetPlayerRecordPointer(Fighter.pcIdx);
	if(NULL == pTempPlayer) return NULL;	
	if(0 != strcmp(Fighter.id,pTempPlayer->userID)) return NULL;
	if(0 != strcmp(Fighter.name,pTempPlayer->name)) return NULL;

	return pTempPlayer;
}

playerCharacter_t* CGolryunBattle_Common::
GetArenaPlayer(const CGolryunBattle_Data::tagFighter& Fighter)
{
	playerCharacter_t* pTempPlayer= gcpTools->GetPlayerRecordPointer(Fighter.pcIdx);
	if(NULL == pTempPlayer) return NULL;
	if(tagGolryunBattle::Golryun_Battle_Map_Index != pTempPlayer->worldIdx) return NULL;
	if(0 != strcmp(Fighter.id,pTempPlayer->userID)) return NULL;
	if(0 != strcmp(Fighter.name,pTempPlayer->name)) return NULL;

	return pTempPlayer;
}


playerCharacter_t* CGolryunBattle_Common::GetArenaPlayer(const int Idx)
{
	playerCharacter_t* pTempPlayer= gcpTools->GetPlayerRecordPointer(Idx);
	if(NULL == pTempPlayer) return NULL;
	if(tagGolryunBattle::Golryun_Battle_Map_Index != pTempPlayer->worldIdx) return NULL;

	return pTempPlayer;
}


void CGolryunBattle_Common::Change_Inventory_Nak(const __int64 i64ParamNak,playerCharacter_t* pPc)
{
	__int64 Nak = (__int64)pPc->curChargeSE + i64ParamNak;
	if(Nak <= 0) Nak=0;
	if(Nak > MAX_OWN_NAK) Nak=MAX_OWN_NAK;
	pPc->curChargeSE = (int)Nak;
}




int AI_NPC_ShareMoney( void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );
	

	AI_NPC_CloseMessage( pc, pParam );
	



	pc->GonryunBattle.ShareWin.i64TotalNak=0;
	pc->GonryunBattle.ShareWin.iFortuneStone=0;
	for(int idx = 0; idx < playerCharacter_t::tagGonryunBattle::tagShareWin::MAX_INVENTORY_NUMBER; idx ++ )
	{
		pc->GonryunBattle.ShareWin.Inventory[idx] = -1;
	}

	
	GTH_AddNoticePC( pc, -1, -1, BUSY_STATE_NPC_INVENTORY );
	(gcpGolryunBattle->GetEncoderClassPointer())->SendOpenShareWin(&pc->sock);
	
	

	return 1;
}



void GTH_SendMessage_ItemSystem_ExchangeClose( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EXCHANGE_CLOSE);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_ExchangeItem( playerCharacter_t *pc, int reply, int error, int pos, int nak )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EXCHANGE_ITEM);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			
			MSG_WriteByte(&netMessage, pos);
			
			MSG_WriteShort( &netMessage, pc->mouseInventory );
			MSG_WriteShort( &netMessage, pc->GonryunBattle.ShareWin.Inventory[pos] );
			MSG_WriteLong( &netMessage, nak );
		}	
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_ExchangePrepare( playerCharacter_t *pc, int reply, int error, int nak, float pro )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EXCHANGE_PREPARE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteLong( &netMessage, nak );
			MSG_WriteFloat( &netMessage, pro );
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
			MSG_WriteLong( &netMessage, nak );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_ExchangeProcess_Goldbar( playerCharacter_t *pPlayer)
{
	
	__int64 i64GoldNum    = pPlayer->GonryunBattle.ShareWin.i64TotalNak / (__int64)playerCharacter_t::tagGonryunBattle::NAK_per_ONEGOLD;
	__int64 i64RemainNak  = pPlayer->GonryunBattle.ShareWin.i64TotalNak - (i64GoldNum * (__int64)playerCharacter_t::tagGonryunBattle::NAK_per_ONEGOLD);
	
	g_logSystem->WriteToLog( 
		pPlayer, 
		LOG_CLASS_GOLRYUN, 
		"%d;%d;%d;;;;", 
		CLogSystem::enumGolryunSubLog::ShareBattleTicket,
		pPlayer->GonryunBattle.ShareWin.i64TotalNak,
		(int)i64GoldNum);

	CGolryunBattle_Common::Change_Inventory_Nak(i64RemainNak,pPlayer);
	pPlayer->GonryunBattle.ShareWin.i64TotalNak=0;

	if(i64GoldNum == 0) return;	
	int addItemData[6];

	addItemData[0] = tagGolryunBattle::GoldBar_Item_Index;		
	addItemData[1] = -1;						
	addItemData[2] = (int)i64GoldNum;			
	addItemData[3] = 1;							
	addItemData[4] = -1;					
	addItemData[5] = -1;					

	AI_NPC_AddItem( pPlayer, addItemData );
}

void GTH_ExchangeProcess_FortuneStone( playerCharacter_t *pPlayer)
{
	int stoneNum    = pPlayer->GonryunBattle.ShareWin.iFortuneStone;
	
	g_logSystem->WriteToLog( 
		pPlayer, 
		LOG_CLASS_GOLRYUN, 
		"%d;%d;;;;;", 
		CLogSystem::enumGolryunSubLog::ShareBattleTicket,
		stoneNum);

	pPlayer->GonryunBattle.ShareWin.iFortuneStone=0;


	if(stoneNum == 0) return;	
	int addItemData[6];

	addItemData[0] = tagGolryunBattle::Fortune_Stone_Item_Index;		
	addItemData[1] = -1;						
	addItemData[2] = stoneNum;			
	addItemData[3] = 1;							
	addItemData[4] = -1;					
	addItemData[5] = -1;					

	AI_NPC_AddItem( pPlayer, addItemData );

}


void GTH_ExchangeProcess( playerCharacter_t *pc, int &reply, int &error, int &reqNak )
{
	int i, itemIdx;

	

	if( pc->curItemWeight >= pc->calItemWeight )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_3;			
		return;
	}
	

	



	
	for( i = 0; i < playerCharacter_t::tagGonryunBattle::tagShareWin::MAX_INVENTORY_NUMBER; i ++ )
	{
		if( pc->GonryunBattle.ShareWin.Inventory[i] < 0 ) continue;
		itemIdx = pc->GonryunBattle.ShareWin.Inventory[i];
		

		
		

		GTH_DeleteMyItem( pc, &pc->item[itemIdx] );
		pc->GonryunBattle.ShareWin.Inventory[i] = -1;

		g_DBGameServer->RemoveItem( pc, itemIdx );
	}
	
	
	
	
	GTH_ExchangeProcess_Goldbar( pc );
	
	
	GTH_ExchangeProcess_FortuneStone( pc );

}


int GetTournamentIdxFromTwoGyun(int level)
{
	int iTournamentIdx=0;

	switch(level)
	{
	case 30:
		iTournamentIdx = 0;
		break;
	case 40:
		iTournamentIdx = 1;
		break;
	case 50:
		iTournamentIdx = 2;
		break;
	case 60:
		iTournamentIdx = 3;
		break;
	case 70:
		iTournamentIdx = 4;
		break;
	case 80:
		iTournamentIdx = 5;
		break;

	case 90:
		iTournamentIdx = 6;
		break;
	}

	return iTournamentIdx;
}

__int64 GetTowGyunPrice(const item_t& item)
{
	if((item.itemTableIdx != tagGolryunBattle::TwoGyun_100000_Item_Index) && 
	   (item.itemTableIdx != tagGolryunBattle::TwoGyun_1000000_Item_Index)) return 0;
	itemTable_t* pItemTableRecord=CGolryunBattle_Common::GetItemTablePointer(item.itemTableIdx);
	if(NULL == pItemTableRecord) return 0;	
		

	int iBattleGUID		= item.optionIdx[0];
	int iTournamentGUID = item.experience;	
	int level			= item.optionIdx[1];
	int winNum			= item.optionIdx[2];
	int iTournamentLevelIdx = GetTournamentIdxFromTwoGyun(level);

	CGolryunBattle_Data*    cpData    = gcpGolryunBattle->GetDataClassPointer();
	CGolryunBattle_Encoder* cpEncoder = gcpGolryunBattle->GetEncoderClassPointer();
	CGolryunBattle_Process* cpProcess = gcpGolryunBattle->GetProcessClassPointer();	
	

	
	
	if(cpData->m_iBattleGUID == iBattleGUID){

		
		if(cpProcess->m_iTournamentGUID == iTournamentGUID){
			if(cpProcess->GetTournamentStatus() == CGolryunBattle_Process::eTournamentStep::TournamentStep_None){			
				
				return pItemTableRecord->sellCost;				
			}			
			
			return 0;
		}



		struct CGolryunBattle_Data::tagTournament* pTournament=&(cpData->m_Tournament[iTournamentLevelIdx]);
		
		if(pTournament->iTournamentGUID != iTournamentGUID)
		{
			return pItemTableRecord->sellCost;
		}
		
		if(FALSE == pTournament->bComplete)	
		{
			return pItemTableRecord->sellCost;
		}	

		if(0 >= pTournament->FirstWinner.FightNum) 
		{
			return pItemTableRecord->sellCost;
		}
		
		
		if(winNum == pTournament->FirstWinner.FightNum)
		{
			float rate = pTournament->TwoGyun.fShareRate;
			return pItemTableRecord->sellCost * rate;
		}		

		return 0;
	}


	
	if(cpData->m_iBattleGUID != iBattleGUID){
		
		CGolryunBattle_DBV2* cpDBV2=gcpGolryunBattle->GetDBV2ClassPointer();
		const struct CGolryunBattle_DBV2::tagBattle* pBattleRecord=
			cpDBV2->GetBattleRecordPointer(iBattleGUID);
		if(NULL == pBattleRecord){
			if(cpProcess->m_iTournamentGUIDminmum > iTournamentGUID) return 0;
			return pItemTableRecord->sellCost;
		}

		const struct CGolryunBattle_DBV2::tagTournament* pTournament=&(pBattleRecord->Tournament[iTournamentLevelIdx]);
		if(pTournament->nGUID != iTournamentGUID)
		{
			return pItemTableRecord->sellCost;
		}

		if(FALSE == pTournament->bComplete)	
		{
			return pItemTableRecord->sellCost;
		}		

		if(0 >= pTournament->firstWinner.Num)
		{
			return pItemTableRecord->sellCost;
		}

		

		if(winNum == pTournament->firstWinner.Num)
		{
			float rate = pTournament->TwoGyun.Rate;
			return pItemTableRecord->sellCost * rate;
		}

		return 0;
	}

	return 0;
}



int GetFortuneStone(const item_t& item)
{
	if((item.itemTableIdx != tagGolryunBattle::TwoGyun_100000_Item_Index) && 
	   (item.itemTableIdx != tagGolryunBattle::TwoGyun_1000000_Item_Index)) return 0;
	itemTable_t* pItemTableRecord=CGolryunBattle_Common::GetItemTablePointer(item.itemTableIdx);
	if(NULL == pItemTableRecord) return 0;	
		

	int iBattleGUID		= item.optionIdx[0];
	int iTournamentGUID = item.experience;	
	int level			= item.optionIdx[1];
	int winNum			= item.optionIdx[2];
	int iTournamentLevelIdx = GetTournamentIdxFromTwoGyun(level);

	CGolryunBattle_Data*    cpData    = gcpGolryunBattle->GetDataClassPointer();
	CGolryunBattle_Encoder* cpEncoder = gcpGolryunBattle->GetEncoderClassPointer();
	CGolryunBattle_Process* cpProcess = gcpGolryunBattle->GetProcessClassPointer();
	
	int	stoneNum=0;

	
	if(cpData->m_iBattleGUID == iBattleGUID){

		
		if(cpProcess->m_iTournamentGUID == iTournamentGUID){
			if(cpProcess->GetTournamentStatus() == CGolryunBattle_Process::eTournamentStep::TournamentStep_None){			
				
				return 0;				
			}			
			
			return 0;
		}



		struct CGolryunBattle_Data::tagTournament* pTournament=&(cpData->m_Tournament[iTournamentLevelIdx]);
		
		if(pTournament->iTournamentGUID != iTournamentGUID)
		{
			return 0;
		}
		
		if(FALSE == pTournament->bComplete)	
		{
			return 0;
		}	

		if(0 >= pTournament->FirstWinner.FightNum) 
		{
			return 0;
		}
		
		if(winNum != pTournament->FirstWinner.FightNum)
		{
			stoneNum = pItemTableRecord->sellCost/100000;
			return stoneNum;
		}		

		return 0;
	}


	
	if(cpData->m_iBattleGUID != iBattleGUID){
		
		CGolryunBattle_DBV2* cpDBV2=gcpGolryunBattle->GetDBV2ClassPointer();
		const struct CGolryunBattle_DBV2::tagBattle* pBattleRecord=cpDBV2->GetBattleRecordPointer(iBattleGUID);
		if(NULL == pBattleRecord){
			if(cpProcess->m_iTournamentGUIDminmum > iTournamentGUID) return 0;
			return 0;
		}

		const struct CGolryunBattle_DBV2::tagTournament* 
			pTournament=&(pBattleRecord->Tournament[iTournamentLevelIdx]);

		if(pTournament->nGUID != iTournamentGUID)
		{
			return 0;
		}

		if(FALSE == pTournament->bComplete)	
		{
			return 0;
		}		

		if(0 >= pTournament->firstWinner.Num)
		{
			return 0;
		}

		if(winNum != pTournament->firstWinner.Num)
		{
			stoneNum = pItemTableRecord->sellCost/100000;
			return stoneNum;
		}

		return 0;
	}

	return 0;
}





BOOL CheckTowGyunStatus(const item_t& item)
{
		
	int iBattleGUID		= item.optionIdx[0];
	int iTournamentGUID = item.experience;	
	int level			= item.optionIdx[1];
	int winNum			= item.optionIdx[2];
	int iTournamentLevelIdx = GetTournamentIdxFromTwoGyun(level);


	CGolryunBattle_Data*    cpData	  = gcpGolryunBattle->GetDataClassPointer();
	CGolryunBattle_Encoder* cpEncoder = gcpGolryunBattle->GetEncoderClassPointer();
	CGolryunBattle_Process* cpProcess = gcpGolryunBattle->GetProcessClassPointer();	


	
	
	if(cpData->m_iBattleGUID == iBattleGUID){

		
		if(cpProcess->m_iTournamentGUID == iTournamentGUID){
			if(cpProcess->GetTournamentStatus() == CGolryunBattle_Process::eTournamentStep::TournamentStep_None){
				
				cpEncoder->SendStatusMatchNotice(&g_curPC->sock,
					tagGolryunBattlePacket_GB_STATUS_NOTICE::enumCode::cancel_tournament);
					
				return TRUE;
			}

			
			cpEncoder->SendErrorCode(&g_curPC->sock, 0, tagGolryunBattlePacket_GB_ERROR_CODE::not_exchange_twogyun);
			return FALSE;
		}


		struct CGolryunBattle_Data::tagTournament* pTournament=&(cpData->m_Tournament[iTournamentLevelIdx]);

		if(pTournament->iTournamentGUID != iTournamentGUID)
		{
			
			cpEncoder->SendStatusMatchNotice(&g_curPC->sock,
				tagGolryunBattlePacket_GB_STATUS_NOTICE::enumCode::cancel_tournament);
			return TRUE;
		}		
		
		
		if(FALSE == pTournament->bComplete)	
		{
			
			cpEncoder->SendStatusMatchNotice(&g_curPC->sock,
				tagGolryunBattlePacket_GB_STATUS_NOTICE::enumCode::cancel_tournament);
			return TRUE;
		}		
		
		if(0 >= pTournament->FirstWinner.FightNum){
			cpEncoder->SendStatusMatchNotice(&g_curPC->sock, 
				tagGolryunBattlePacket_GB_STATUS_NOTICE::enumCode::draw_tournament);
			return TRUE;
		}

		
		
		return TRUE;
	}
	

	
	if(cpData->m_iBattleGUID != iBattleGUID){	
		CGolryunBattle_DBV2* cpDBV2=gcpGolryunBattle->GetDBV2ClassPointer();
		const struct CGolryunBattle_DBV2::tagBattle* pBattleRecord=cpDBV2->GetBattleRecordPointer(iBattleGUID);
		if(NULL == pBattleRecord) return TRUE;
		
		const struct CGolryunBattle_DBV2::tagTournament* pTournament=&(pBattleRecord->Tournament[iTournamentLevelIdx]);

		if(FALSE == pTournament->bComplete)	
		{
			cpEncoder->SendStatusMatchNotice(&g_curPC->sock,
				tagGolryunBattlePacket_GB_STATUS_NOTICE::enumCode::cancel_tournament);

			return TRUE;
		}
		
		if(pTournament->nGUID != iTournamentGUID)
		{
			cpEncoder->SendStatusMatchNotice(&g_curPC->sock,
				tagGolryunBattlePacket_GB_STATUS_NOTICE::enumCode::cancel_tournament);
			return TRUE;
		}


		if(0 >= pTournament->firstWinner.Num){
			cpEncoder->SendStatusMatchNotice(&g_curPC->sock, 
				tagGolryunBattlePacket_GB_STATUS_NOTICE::enumCode::draw_tournament);
			return TRUE;
		}

		
		

		
		return TRUE;		
	}


	return FALSE;
}





void GTH_ProcessMessage_ItemSystem_ExchangeItem()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_NPC_INVENTORY))
		return;

	int battleType = MSG_ReadShort();
	int levelType  = MSG_ReadShort();
	int pos        = MSG_ReadShort();

	int tempInven;
	int reply = 1, error = 0;

	if( g_curPC->mouseInventory >= 0 )
	{
		if( g_curPC->GonryunBattle.ShareWin.Inventory[pos] >= 0 )
		{
				int itemIdx = g_curPC->mouseInventory;
				int itemTableIdx = g_curPC->item[itemIdx].itemTableIdx;

				if((itemTableIdx == tagGolryunBattle::TwoGyun_100000_Item_Index) || 
				   (itemTableIdx == tagGolryunBattle::TwoGyun_1000000_Item_Index))
				{
					tempInven = g_curPC->mouseInventory;
					g_curPC->GonryunBattle.ShareWin.i64TotalNak += GetTowGyunPrice(g_curPC->item[tempInven]);
					g_curPC->GonryunBattle.ShareWin.iFortuneStone += GetFortuneStone(g_curPC->item[tempInven]);

					int InventoryItemIdx = g_curPC->GonryunBattle.ShareWin.Inventory[pos];
					g_curPC->GonryunBattle.ShareWin.i64TotalNak -= GetTowGyunPrice(g_curPC->item[InventoryItemIdx]);
					g_curPC->GonryunBattle.ShareWin.iFortuneStone -= GetFortuneStone(g_curPC->item[InventoryItemIdx]);
					
					g_curPC->mouseInventory = g_curPC->GonryunBattle.ShareWin.Inventory[pos];
					g_curPC->GonryunBattle.ShareWin.Inventory[pos] = tempInven;
					
				}
				else
				{
					(gcpGolryunBattle->GetEncoderClassPointer())->SendErrorCode(&g_curPC->sock, 0, tagGolryunBattlePacket_GB_ERROR_CODE::unknown_item);
					reply = 0;
				}
			
		}
		
		else
		{
			g_curPC->GonryunBattle.ShareWin.Inventory[pos] = g_curPC->mouseInventory;
			
			int itemIdx = g_curPC->GonryunBattle.ShareWin.Inventory[pos];
			int itemTableIdx = g_curPC->item[itemIdx].itemTableIdx;

			if((itemTableIdx == tagGolryunBattle::TwoGyun_100000_Item_Index) || 
			   (itemTableIdx == tagGolryunBattle::TwoGyun_1000000_Item_Index))
			{

				
				if(TRUE == CheckTowGyunStatus(g_curPC->item[itemIdx]))
				{
					g_curPC->GonryunBattle.ShareWin.i64TotalNak += GetTowGyunPrice(g_curPC->item[itemIdx]);
					g_curPC->GonryunBattle.ShareWin.iFortuneStone += GetFortuneStone(g_curPC->item[itemIdx]);
					g_curPC->mouseInventory = -1;
					reply = 1;
				}
				else
				{
					g_curPC->GonryunBattle.ShareWin.Inventory[pos] = -1;
					reply = 0;
				}

			}
			else 
			{
				
				g_curPC->GonryunBattle.ShareWin.Inventory[pos] = -1;
				(gcpGolryunBattle->GetEncoderClassPointer())->SendErrorCode(&g_curPC->sock, 0, tagGolryunBattlePacket_GB_ERROR_CODE::unknown_item);
				reply = 0;
			}			
		}
	}
	
	else
	{
		int itemIdx = g_curPC->GonryunBattle.ShareWin.Inventory[pos];
		int itemTableIdx = g_curPC->item[itemIdx].itemTableIdx;
		
		if( (itemTableIdx == tagGolryunBattle::TwoGyun_100000_Item_Index) || 
			(itemTableIdx == tagGolryunBattle::TwoGyun_1000000_Item_Index))
		{

			g_curPC->GonryunBattle.ShareWin.i64TotalNak -= GetTowGyunPrice(g_curPC->item[itemIdx]);
			g_curPC->GonryunBattle.ShareWin.iFortuneStone -= GetFortuneStone(g_curPC->item[itemIdx]);

			g_curPC->mouseInventory = g_curPC->GonryunBattle.ShareWin.Inventory[pos];
			g_curPC->GonryunBattle.ShareWin.Inventory[pos] = -1;
			
		}
		else
			reply = 0;
		
	}

	
	GTH_SendMessage_ItemSystem_ExchangeItem( g_curPC, reply, error, pos, g_curPC->GonryunBattle.ShareWin.i64TotalNak );

}


int GTH_UndoPCExchange( playerCharacter_t *pc, int completeFlag )
{
	
	int itemIdx, inventoryPos;
	item_t *item, *upItem;

	int bIsStack = false, stackNumber = 1;

	for( int i = 0; i < playerCharacter_t::tagGonryunBattle::tagShareWin::MAX_INVENTORY_NUMBER; i ++ )
	{
		itemIdx = pc->GonryunBattle.ShareWin.Inventory[i];
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
	
	
	for( i = 0; i < playerCharacter_t::tagGonryunBattle::tagShareWin::MAX_INVENTORY_NUMBER; i ++ )
	{
		pc->GonryunBattle.ShareWin.Inventory[i] = -1;
	}

	


	return 1;
}

void GTH_ProcessMessage_ItemSystem_ExchangeClose()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_NPC_INVENTORY))
		return;
	
	GTH_UndoPCExchange( g_curPC );
	GTH_DeleteNoticePC( g_curPC );

	GTH_SendMessage_ItemSystem_ExchangeClose( g_curPC );
}



void GTH_ProcessMessage_ItemSystem_ExchangePrepare()
{
	
	if ( FALSE == isValidPCBusy(g_curPC, BUSY_STATE_NPC_INVENTORY))
		return;

	int reply = 1, error = 0;
	int nak = 0;
	float pro = 0.0f;
	
	GTH_ExchangeProcess( g_curPC, reply, error, nak );

	GTH_SendMessage_ItemSystem_ExchangePrepare( g_curPC, reply, error, nak, pro );

	GTH_DeleteNoticePC(g_curPC);
}




