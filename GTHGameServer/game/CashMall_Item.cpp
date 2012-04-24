

#include "../Global.h"


#include "../Tools/CTools.h"
extern CTools* gcpTools;


extern void TransmitePacket_sc_disconnect(playerCharacter_t* pPlayer, int MessageIdx =0);







void GTH_DecodeCashMallItem()
{	
	SHORT Packet = static_cast<enum enumPacketCashMallItem>(MSG_ReadShort());

	switch(Packet) 
	{
		
	case REQUEST_USE_STONE_OF_SAGE:
		GTH_ProcessMessageRequest_StoneOfSage();
		break;

		
	case REQUEST_USE_STONE_OF_LUCKY:
		GTH_ProcessMessageRequest_StoneOfLucky();
		break;

		
	case REQUEST_USE_STONE_OF_EACH_NAME:
		GTH_ProcessMessageRequest_StoneOfEachName();
		break;

		
	case REQUEST_USE_REVERSE_OF_MEMORY:
		GTH_ProcessMessageRequest_ReverseOfMemory();
		break;

		
	case REQUEST_OPTION_ITEM_EDIT:		
		GTH_ProcessMessageRequest_OptionItemEdit();
		break;

		
	case REQUEST_OPTION_ITEM_WINCLOSE:
		GTH_ProcessMessageRequest_OptionItemWinClose();
		break;

		
	case REQUEST_OPTION_ITEM_MODIFY:
		GTH_ProcessMessageRequest_OptionItemModify();
		break;

	}

}






void GTH_ProcessMessageRequest_StoneOfSage()
{		
	int InvenPos = MSG_ReadLong();

	
	if ( InvenPos < 0 || InvenPos >= MAX_NUMBER_OF_OWNITEM )
	{
		GTH_SendMessageReply_StoneOfSage(g_curPC, FALSE, InvenPos );
	}
	

	g_curPC->m_CashMall_OptionStone.InitCashMallItem_OptionStone();
	g_curPC->m_CashMall_OptionStone.m_iUsingItemInvenIdx = InvenPos;
	g_curPC->m_CashMall_OptionStone.m_iUsingItemMode = 
		CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE;	

	float DestroyRate = 
		g_logic.OptionStoneItemLogic[CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE].DestroyRate;	

	
	float SuccecsyRate = 
		g_logic.OptionStoneItemLogic[CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE].SuccecsRate;	
	
	
	GTH_SendMessageReply_StoneOfSage(g_curPC, TRUE, InvenPos , DestroyRate, SuccecsyRate);	

}






void GTH_ProcessMessageRequest_StoneOfLucky()
{
	int InvenPos = MSG_ReadLong();

	
	
	if ( InvenPos < 0 || InvenPos >= MAX_NUMBER_OF_OWNITEM )
	{
		GTH_SendMessageReply_StoneOfLucky(g_curPC, FALSE, InvenPos );
	}
	
	g_curPC->m_CashMall_OptionStone.InitCashMallItem_OptionStone();

	g_curPC->m_CashMall_OptionStone.m_iUsingItemInvenIdx = InvenPos;
	g_curPC->m_CashMall_OptionStone.m_iUsingItemMode = 
		CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY;	

	float DestroyRate = 
		g_logic.OptionStoneItemLogic[CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY].DestroyRate;

	float SuccecsyRate = 
		g_logic.OptionStoneItemLogic[CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY].SuccecsRate;	


	GTH_SendMessageReply_StoneOfLucky(g_curPC, TRUE, InvenPos , DestroyRate, SuccecsyRate);
}






void GTH_ProcessMessageRequest_StoneOfEachName()
{
	int InvenPos = MSG_ReadLong();

	
	
	if ( InvenPos < 0 || InvenPos >= MAX_NUMBER_OF_OWNITEM )
	{
		GTH_SendMessageReply_StoneOfEachName(g_curPC, FALSE, InvenPos );
		return;
	}
	
	
	g_curPC->m_CashMall_OptionStone.InitCashMallItem_OptionStone();

	g_curPC->m_CashMall_OptionStone.m_iUsingItemInvenIdx = InvenPos;
	g_curPC->m_CashMall_OptionStone.m_iUsingItemMode = 
		CashMallItem_OptionStone::enumUseItemMode::STONE_OF_EACH_NAME;	

	GTH_SendMessageReply_StoneOfEachName(g_curPC, TRUE, InvenPos );
}






void GTH_ProcessMessageRequest_ReverseOfMemory()
{
	int InvenPos = MSG_ReadLong();

	
	if ( GTH_IsEquipItem(g_curPC) == TRUE )		
	{
		GTH_SendMessageReply_ReverseOfMemory(g_curPC, FALSE, CASHMALL_ITEM_ERROR_0, InvenPos );
		return;
	}

	
	if ( GTH_IsAwakenJob ( g_curPC ) == FALSE )
	{
		GTH_SendMessageReply_ReverseOfMemory(g_curPC, FALSE, CASHMALL_ITEM_ERROR_1, InvenPos);
		return;
	}

	item_t* pUseitem = NULL;	
	pUseitem = gcpTools->GetMyInventoryItem(g_curPC, InvenPos);

	
	if ( pUseitem == NULL )
	{
		GTH_SendMessageReply_ReverseOfMemory(g_curPC, FALSE, CASHMALL_ITEM_ERROR_4, InvenPos);
		return;

	}

	vector<int> vDeletedSkill;
	
	if ( GTH_ClearAwakenJob(g_curPC, &vDeletedSkill) == FALSE )
	{
		GTH_SendMessageReply_ReverseOfMemory(g_curPC, FALSE, CASHMALL_ITEM_ERROR_5, InvenPos);
		return;
	}

	
	GTH_UseItem_Delete(InvenPos);

	
	GTH_SendPCMessage_ReplyGenChange(g_curPC);


	GTH_SendMessageReply_ReverseOfMemory(g_curPC, TRUE, -1, InvenPos , &vDeletedSkill);
	return;

}






void GTH_ProcessMessageRequest_OptionItemEdit()
{	
	
	if( g_curPC->mouseInventory >= 0 )
	{		
		
		if ( g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx >= 0)
		{
			int tempInven = g_curPC->mouseInventory;
			g_curPC->mouseInventory = g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx;
			g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx = tempInven;
		}
		
		else
		{
			g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx = g_curPC->mouseInventory;
			g_curPC->mouseInventory = -1;			
		}
	}
	
	else
	{		
		if ( g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx >= 0)
		{		
			g_curPC->mouseInventory = g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx;
			g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx =-1;
		}		
	}

	GTH_SendMessageReply_OptionItemEdit( g_curPC, FALSE, g_curPC->mouseInventory, 
		g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx);

}






void GTH_SendMessageReply_StoneOfSage(playerCharacter_t* pPc, BOOL Reply, int InvenPos, float DestroyRate, float SuccecsRate)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REPLY_USE_STONE_OF_SAGE );	
		MSG_WriteLong(&netMessage,  Reply);		

		if ( Reply == TRUE)
		{
			MSG_WriteLong(&netMessage,	InvenPos);		
			MSG_WriteFloat(&netMessage, DestroyRate);
			
			MSG_WriteFloat(&netMessage, SuccecsRate);
		}

		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	
}






void GTH_SendMessageReply_StoneOfLucky(playerCharacter_t* pPc, BOOL Reply, int InvenPos, float DestroyRate, float SuccecsRate)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REPLY_USE_STONE_OF_LUCKY );						
		MSG_WriteLong(&netMessage,  Reply);

		if ( Reply == TRUE)
		{
			MSG_WriteLong(&netMessage,	InvenPos);	
			MSG_WriteFloat(&netMessage, DestroyRate);
			
			MSG_WriteFloat(&netMessage, SuccecsRate);
		}

		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_SendMessageReply_StoneOfEachName(playerCharacter_t* pPc, BOOL Reply, int InvenPos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REPLY_USE_STONE_OF_EACH_NAME );						
		MSG_WriteLong(&netMessage,  Reply);

		if ( Reply == TRUE)
			MSG_WriteLong(&netMessage,	InvenPos);	
			
		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessageReply_ReverseOfMemory(playerCharacter_t* pPc, BOOL Reply, int ErrorIdx, int InvenPos, vector<int>* pVSkill)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REPLY_USE_REVERSE_OF_MEMORY );						
		MSG_WriteLong(&netMessage,	Reply);	

		if ( Reply == TRUE)
		{
			MSG_WriteLong(&netMessage,	InvenPos);	
			MSG_WriteLong(&netMessage,  pPc->pcJob);
			
			
			for (int Idx =0; Idx  < GEN_NUMBER; Idx ++)
				MSG_WriteLong(&netMessage, pPc->genCapability[Idx]);			


			
			for (int HotKeyIdx = 0 ; HotKeyIdx< MAX_NUMBER_OF_HOTKEY; HotKeyIdx ++)
			{
				MSG_WriteLong(&netMessage, pPc->hotkeyType[HotKeyIdx]);
				MSG_WriteLong(&netMessage, pPc->hotkeyIdx[HotKeyIdx]);				
			}
			

			
			if ( pVSkill != NULL )
			{
				MSG_WriteLong(&netMessage, pVSkill->size());
					for (int SkillIdx = 0; SkillIdx < pVSkill->size(); SkillIdx++)								
					{
						Idx = (*pVSkill)[SkillIdx];
						MSG_WriteLong(&netMessage, Idx);
					}
			}			
			else
				MSG_WriteLong(&netMessage, 0);
		}
		else
			MSG_WriteLong(&netMessage,	ErrorIdx);


		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessageReply_OptionItemEdit(playerCharacter_t* pPc, BOOL bError, 
										 int MouseInvenPos, int EditInevenPos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REPLY_OPTION_ITEM_EDIT );
		MSG_WriteLong(&netMessage,  bError);

		if ( bError == FALSE)
		{
			MSG_WriteLong(&netMessage,	MouseInvenPos);	
			MSG_WriteLong(&netMessage,	EditInevenPos);	
		}		
		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void InitCashMallItem(playerCharacter_t* pPc)
{
	if ( pPc == NULL)
		return ;

	pPc->m_CashMall_OptionStone.m_iEditOptionItemIdx = -1;
}







void GTH_ProcessMessageRequest_OptionItemWinClose()
{
	BOOL bError = FALSE;

	if ( g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx != -1)
	{
		bError = !GTH_AddMyInventory_ForRegsitedItem(g_curPC,
			g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx);	
		InitCashMallItem(g_curPC);
	}

	
	GTH_SendMessageRePly_OptionItemWinClose(g_curPC, bError);
}








void GTH_SendMessageRePly_OptionItemWinClose(playerCharacter_t* pPc, BOOL bError)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REPLY_OPTION_ITEM_WINCLOSE );
		MSG_WriteLong(&netMessage,  bError);		
		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_ProcessMessageRequest_OptionItemModify()
{
	enum CashMallItem_OptionStone::enumUseItemMode mode;
	mode = static_cast<enum CashMallItem_OptionStone::enumUseItemMode>(MSG_ReadLong());

	switch(mode) 
	{
	case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE:
		ItemModify_Stone_of_sage();
		break;
	case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY:
		ItemModify_Stone_of_lucky();
		break;
	case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_EACH_NAME:
		ItemModify_Stone_of_each_name();
		break;	
	}
}






void GTH_SendMessageRePly_OptionItemModify(playerCharacter_t* pPc, enum CashMallItem_OptionStone::enumUseItemMode mode, 
										   BOOL Reply, item_t* pItem, int ErrorIdx)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REPLY_OPTION_ITEM_MODIFY );
		MSG_WriteLong(&netMessage,	mode );		
		MSG_WriteLong(&netMessage,  Reply);	

		if ( Reply == TRUE)		
			MSG_WriteItem(&netMessage, pItem);		
		else
			MSG_WriteLong(&netMessage, ErrorIdx);

		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void ItemModify_Stone_of_sage()
{
	int SelectOptionIdx = MSG_ReadLong();

	int ItemIdx		= g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx;
	int UseItemIdx	= g_curPC->m_CashMall_OptionStone.m_iUsingItemInvenIdx;	

	CashMallItem_OptionStone::enumUseItemMode mode = 
		CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE ;


	item_t* pSrcItem	= NULL;
	item_t* pUsetem		= NULL;

	item_t TempItem;
	
	pSrcItem = gcpTools->GetMyItem(g_curPC, ItemIdx);
	pUsetem  = gcpTools->GetMyInventoryItem(g_curPC, UseItemIdx);

	
	if ( pSrcItem == NULL )
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, 
			FALSE, NULL, CASHMALL_ITEM_ERROR_0);
		return;
	}

	
	if ( SelectOptionIdx < 0 || SelectOptionIdx >= MAX_NUMBER_OF_ITEM_OPTION)
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
			FALSE, NULL, CASHMALL_ITEM_ERROR_1);
		return;
	}

	
	






	if ( pUsetem == NULL )
	{		
		TransmitePacket_sc_disconnect(g_curPC);
		return;
	}
	if ( pUsetem->itemTableIdx != 
		CashMallItem_OptionStone::enumCashMall_ItemIdx::STONE_OF_SAGE_IDX)
	{
		TransmitePacket_sc_disconnect(g_curPC);
		return;
	}

	


	
	
	if ( pSrcItem->itemTableIdx < 0 || pSrcItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, 
			FALSE, NULL, CASHMALL_ITEM_ERROR_0);
		return;
	}

	int availableOn	= GetItemAvailalbeOn( pSrcItem->itemTableIdx );

	
	if ( availableOn >= OPTION_AVAILABLE_ON_RING || pSrcItem->itemTableIdx == MONSTERCARD_ITEM_INDEX)
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
			FALSE, NULL, CASHMALL_ITEM_ERROR_7 );
		return;
	}	
	
	float SuccecsRate = 
		g_logic.OptionStoneItemLogic[CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE].SuccecsRate;					

	float DestroyRate = 
		g_logic.OptionStoneItemLogic[CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE].DestroyRate;

	
	
	
	if ( GTH_Random() >= SuccecsRate * 0.01)
	{
		
		if ( GTH_Random() < DestroyRate * 0.01)
		{
			GTH_DeleteMyItem(g_curPC, pSrcItem );		
			g_DBGameServer->RemoveItem(g_curPC, ItemIdx);	
			
			GTH_UseItem_Delete(UseItemIdx);
			g_curPC->m_CashMall_OptionStone.InitCashMallItem_OptionStone();
			g_curPC->m_CashMall_OptionStone.m_iUsingItemMode = 
				CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE;
			
			GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, FALSE, NULL, CASHMALL_ITEM_ERROR_5);
			return;
		}
		
		else
		{
			
			GTH_UseItem_Delete(UseItemIdx);			
			
			GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, FALSE, NULL, CASHMALL_ITEM_ERROR_6);
			return;
		}
	}

	














	int OptionTableIdx = pSrcItem->optionIdx[SelectOptionIdx];
	int LoopCount = 0;
	while ( 1 )
	{
		if ( FALSE == SpawnItem(&TempItem, pSrcItem->itemTableIdx, MAX_NUMBER_OF_ITEM_OPTION, 
			1 ) )
		{
			
			
			GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
				FALSE, NULL, CASHMALL_ITEM_ERROR_2);
			return;
		}
		
		
		for (int SpawnOptionArryIdx = 0; SpawnOptionArryIdx < TempItem.optionNumber; SpawnOptionArryIdx++)
		{			
			if ( IsExistenceItemOption(pSrcItem, 
				TempItem.optionIdx[SpawnOptionArryIdx]) == 0)
			{
				pSrcItem->optionIdx[SelectOptionIdx]		= TempItem.optionIdx[SpawnOptionArryIdx];
				pSrcItem->optionValue[SelectOptionIdx]		= SelectOptionGrade(g_curPC->m_CashMall_OptionStone.m_iUsingItemMode);;				
				
				
				
				g_DBGameServer->SaveItem( g_curPC, pSrcItem, ItemIdx );
				
				
				GTH_UseItem_Delete(UseItemIdx);
				
				GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, TRUE, pSrcItem);
				return;
			}
		}		
		LoopCount++;
		
		
		if ( LoopCount >= MAX_NUMBER_OF_ITEM_OPTION )
		{
			GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
				FALSE, pSrcItem, CASHMALL_ITEM_ERROR_3);
			return;
		}
	}
	
}






void ItemModify_Stone_of_lucky()
{
	int ItemIdx		= g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx;
	int UseItemIdx	= g_curPC->m_CashMall_OptionStone.m_iUsingItemInvenIdx;	

	CashMallItem_OptionStone::enumUseItemMode mode = 
		CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY ;


	item_t* pSrcItem	= NULL;
	item_t* pUsetem		= NULL;

	item_t TempItem;
	
	pSrcItem = gcpTools->GetMyItem(g_curPC, ItemIdx);
	pUsetem  = gcpTools->GetMyInventoryItem(g_curPC, UseItemIdx);

	
	if ( pSrcItem == NULL )
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, 
			FALSE, NULL, CASHMALL_ITEM_ERROR_0);
		return;
	}
	

	
	






	if ( pUsetem == NULL )
	{		
		TransmitePacket_sc_disconnect(g_curPC);
		return;
	}
	if ( pUsetem->itemTableIdx != 
		CashMallItem_OptionStone::enumCashMall_ItemIdx::STONE_OF_LUCKY_IDX)
	{
		TransmitePacket_sc_disconnect(g_curPC);
		return;
	}

	
	
	if ( pSrcItem->optionNumber >= MAX_NUMBER_OF_ITEM_OPTION )
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
			FALSE, NULL, CASHMALL_ITEM_ERROR_3);
		return;
	}	

	
	
	if ( pSrcItem->itemTableIdx < 0 || pSrcItem->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, 
			FALSE, NULL, CASHMALL_ITEM_ERROR_0);
		return;
	}

	int availableOn	= GetItemAvailalbeOn( pSrcItem->itemTableIdx );

	
	if ( availableOn >= OPTION_AVAILABLE_ON_RING || pSrcItem->itemTableIdx == MONSTERCARD_ITEM_INDEX )
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
			FALSE, NULL, CASHMALL_ITEM_ERROR_7 );
		return;
	}	

	float SuccecsRate = 
		g_logic.OptionStoneItemLogic[CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY].SuccecsRate;

	float DestroyRate = 
		g_logic.OptionStoneItemLogic[CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY].DestroyRate;


	if ( GTH_Random() >= SuccecsRate * 0.01)
	{
		if ( GTH_Random() < DestroyRate * 0.01)
		{			
			GTH_DeleteMyItem(g_curPC, pSrcItem );		
			g_DBGameServer->RemoveItem(g_curPC, ItemIdx);	
			
			GTH_UseItem_Delete(UseItemIdx);
			g_curPC->m_CashMall_OptionStone.InitCashMallItem_OptionStone();
			g_curPC->m_CashMall_OptionStone.m_iUsingItemMode = 
				CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY;
			
			GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, FALSE, NULL, CASHMALL_ITEM_ERROR_5);
			return;
		}		
		
		else
		{
			
			GTH_UseItem_Delete(UseItemIdx);
			
			GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, FALSE, NULL, CASHMALL_ITEM_ERROR_6);
			return;
		}		

	}

	














	int LoopCount = 0;
	while ( 1 )
	{
		if ( FALSE == SpawnItem(&TempItem, pSrcItem->itemTableIdx, MAX_NUMBER_OF_ITEM_OPTION, 
			1 ) )
		{
			
			
			GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
				FALSE, NULL, CASHMALL_ITEM_ERROR_2);
			return;
		}
		
		
		for (int SpawnOptionArryIdx = 0; SpawnOptionArryIdx < TempItem.optionNumber; SpawnOptionArryIdx++)
		{			
			if ( IsExistenceItemOption(pSrcItem, 
				TempItem.optionIdx[SpawnOptionArryIdx]) == 0)
			{
				pSrcItem->optionIdx[pSrcItem->optionNumber]		= TempItem.optionIdx[SpawnOptionArryIdx];
				pSrcItem->optionValue[pSrcItem->optionNumber]	= SelectOptionGrade(g_curPC->m_CashMall_OptionStone.m_iUsingItemMode);
				pSrcItem->optionNumber++;				
				
				
				
				g_DBGameServer->SaveItem( g_curPC, pSrcItem, ItemIdx );
				
				
				GTH_UseItem_Delete(UseItemIdx);
				
				GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, TRUE, pSrcItem);
				return;
			}
		}		
		
		LoopCount++;
		
		
		if ( LoopCount >= MAX_NUMBER_OF_ITEM_OPTION )
		{
			GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
				FALSE, pSrcItem, CASHMALL_ITEM_ERROR_3);
			return;
		}
	}
	
}






void ItemModify_Stone_of_each_name()
{
	int SelectOptionIdx = MSG_ReadLong();

	int ItemIdx		= g_curPC->m_CashMall_OptionStone.m_iEditOptionItemIdx;
	int UseItemIdx	= g_curPC->m_CashMall_OptionStone.m_iUsingItemInvenIdx;	

	CashMallItem_OptionStone::enumUseItemMode mode = 
		CashMallItem_OptionStone::enumUseItemMode::STONE_OF_EACH_NAME ;


	item_t* pSrcItem	= NULL;
	item_t* pUsetem		= NULL;	
	
	pSrcItem = gcpTools->GetMyItem(g_curPC, ItemIdx);
	pUsetem  = gcpTools->GetMyInventoryItem(g_curPC, UseItemIdx);

	
	if ( pSrcItem == NULL )
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, 
			FALSE, NULL, CASHMALL_ITEM_ERROR_0);
		return;
	}

	
	if ( SelectOptionIdx < 0 || SelectOptionIdx >= MAX_NUMBER_OF_ITEM_OPTION)
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
			FALSE, NULL, CASHMALL_ITEM_ERROR_1);
		return;
	}
	
	
	
	
	
	
	
	
	if ( pUsetem == NULL )
	{		
		TransmitePacket_sc_disconnect(g_curPC);
		return;
	}
	if ( pUsetem->itemTableIdx != 
		CashMallItem_OptionStone::enumCashMall_ItemIdx::STONE_OF_EACH_NAME_IDX)
	{
		TransmitePacket_sc_disconnect(g_curPC);
		return;
	}
	
	
	
	if ( pSrcItem->optionNumber <= 0)
	{
		GTH_SendMessageRePly_OptionItemModify(g_curPC, mode,
			FALSE, NULL, CASHMALL_ITEM_ERROR_5);
		return;
	}


	for(int ArryIdx =0; ArryIdx<pSrcItem->optionNumber; ArryIdx++)
	{
		if( ArryIdx == SelectOptionIdx )
		{
			pSrcItem->optionIdx[SelectOptionIdx] = -1;
			pSrcItem->optionValue[SelectOptionIdx] = -1;
		}
		else if( ArryIdx > SelectOptionIdx )
		{
			pSrcItem->optionIdx[ArryIdx-1] = pSrcItem->optionIdx[ArryIdx];
			pSrcItem->optionValue[ArryIdx-1] = pSrcItem->optionValue[ArryIdx];

			pSrcItem->optionIdx[ArryIdx] = -1;
			pSrcItem->optionValue[ArryIdx] = -1;
		}
	}
	pSrcItem->optionNumber--;
	
	
	
	g_DBGameServer->SaveItem( g_curPC, pSrcItem, ItemIdx );

	
	GTH_UseItem_Delete(UseItemIdx);

	GTH_SendMessageRePly_OptionItemModify(g_curPC, mode, TRUE, pSrcItem);
}






int IsExistenceItemOption(item_t *pItem, int OptionIdx)
{
	if ( pItem == NULL)
		return -1;

	for (int OptionArryIdx = 0; OptionArryIdx < pItem->optionNumber; OptionArryIdx++)
	{
		if ( pItem->optionIdx[OptionArryIdx] == OptionIdx)
			return 1;
	}		
	return 0;
}


int	SelectOptionGrade(enum CashMallItem_OptionStone::enumUseItemMode mode)
{
	float GraderandomVal	= 0.0f;
	float GradeaccPro		= 0.0f;

	GraderandomVal			= GTH_Random();			

	for( int OptionGradeNum = 0; OptionGradeNum < MAX_NUMBER_OF_OPTION_VALUE_GRADE ; OptionGradeNum ++ )
	{								
		GradeaccPro += (float) g_logic.OptionStoneItemLogic[mode].OptionGrade[OptionGradeNum] * 0.01;
		if( GraderandomVal < GradeaccPro ) break;
	}
	
	
	OptionGradeNum += 1;
	
	
	if ( OptionGradeNum < 0 ) 
		OptionGradeNum = 1;	
	
	
	
	if (OptionGradeNum > MAX_NUMBER_OF_OPTION_VALUE_GRADE )
		OptionGradeNum = MAX_NUMBER_OF_OPTION_VALUE_GRADE;	

	return OptionGradeNum;
}