

#include "../global_def.h"







void GTH_DecodeCashMallItem()
{	
	SHORT Packet = static_cast<enum enumPacketCashMallItem>(MSG_ReadShort());

	switch(Packet) 
	{
		
	case REPLY_USE_STONE_OF_SAGE:
		GTH_ProcessMessageReply_StoneOfSage();
		break;

		
	case REPLY_USE_STONE_OF_LUCKY:
		GTH_ProcessMessageReply_StoneOfLucky();
		break;

		
	case REPLY_USE_STONE_OF_EACH_NAME:
		GTH_ProcessMessageReply_StoneOfEachName();
		break;

		
	case REPLY_USE_REVERSE_OF_MEMORY:
		GTH_ProcessMessageReply_ReverseOfMemory();
		break;

		
	case REPLY_OPTION_ITEM_EDIT:		
		GTH_ProcessMessageReply_OptionItemEdit();
		break;
		
	case REPLY_OPTION_ITEM_WINCLOSE:
		GTH_ProcessMessageReply_OptionItemWinClose();
		break;

	case REPLY_OPTION_ITEM_MODIFY:
		GTH_ProcessMessageReply_ItemModify();
		break;
	}

}






void GTH_ProcessMessageReply_StoneOfSage()
{	
	BOOL Reply = -1;
	Reply = MSG_ReadLong();

	if ( Reply == TRUE)
	{	
		g_ifMng->m_CashMallItem->InitCashMallItem();
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.InitCashMallItem_OptionStone();
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx = MSG_ReadLong();
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_DestroyRate =  MSG_ReadFloat();				

		
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_SuccecsRate =  MSG_ReadFloat();				

		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemMode = 
			CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE;
		g_ifMng->m_CashMallItem->Enable();


		
		CIFItemWin::sLockItem lockitem;		
		lockitem.nInvenPos = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx;		
		if ( lockitem.nInvenPos >= 0 || lockitem.nInvenPos < MAX_INVENTORY_SIZE)
		{
			int nitemIdx = g_cgv.myCharacterInfo->inventory[lockitem.nInvenPos];
			if ( nitemIdx >= 0 || nitemIdx < MAX_NUMBER_OF_OWNITEM )
			{
				item_t* pItem = &g_cgv.myCharacterInfo->item[nitemIdx];
				lockitem.nItemTableIdx = pItem->itemTableIdx;
				g_ifMng->m_itemWin->LockItem(lockitem);
			}
		}
	}
	else
	{
		
		g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 506));
	}
}





void GTH_ProcessMessageReply_StoneOfLucky()
{
	BOOL Reply = -1;
	Reply = MSG_ReadLong();

	if ( Reply == TRUE)
	{
		
		g_ifMng->m_CashMallItem->InitCashMallItem();
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.InitCashMallItem_OptionStone();

		int InvenPos = MSG_ReadLong();
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx= InvenPos;
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_DestroyRate =  MSG_ReadFloat();

		
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_SuccecsRate=  MSG_ReadFloat();
		
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemMode = 
			CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY;

		
		g_ifMng->m_CashMallItem->Enable();		

		
		CIFItemWin::sLockItem lockitem;		
		lockitem.nInvenPos = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx;		
		if ( lockitem.nInvenPos >= 0 || lockitem.nInvenPos < MAX_INVENTORY_SIZE)
		{
			int nitemIdx = g_cgv.myCharacterInfo->inventory[lockitem.nInvenPos];
			if ( nitemIdx >= 0 || nitemIdx < MAX_NUMBER_OF_OWNITEM )
			{
				item_t* pItem = &g_cgv.myCharacterInfo->item[nitemIdx];
				lockitem.nItemTableIdx = pItem->itemTableIdx;
				g_ifMng->m_itemWin->LockItem(lockitem);
			}
		}

	}
	else
	{
		g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 506));
	}

}






void GTH_ProcessMessageReply_StoneOfEachName()
{

	BOOL Reply = -1;
	Reply = MSG_ReadLong();

	if ( Reply == TRUE)
	{
		g_ifMng->m_CashMallItem->InitCashMallItem();		
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.InitCashMallItem_OptionStone();
		int InvenPos = MSG_ReadLong();

		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx= InvenPos;
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemMode = 
			CashMallItem_OptionStone::enumUseItemMode::STONE_OF_EACH_NAME;

		
		g_ifMng->m_CashMallItem->Enable();		

		
		CIFItemWin::sLockItem lockitem;		
		lockitem.nInvenPos = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx;		
		if ( lockitem.nInvenPos >= 0 || lockitem.nInvenPos < MAX_INVENTORY_SIZE)
		{
			int nitemIdx = g_cgv.myCharacterInfo->inventory[lockitem.nInvenPos];
			if ( nitemIdx >= 0 || nitemIdx < MAX_NUMBER_OF_OWNITEM )
			{
				item_t* pItem = &g_cgv.myCharacterInfo->item[nitemIdx];
				lockitem.nItemTableIdx = pItem->itemTableIdx;
				g_ifMng->m_itemWin->LockItem(lockitem);
			}
		}

	}
	else
	{
		g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 506));
	}

}

void GTH_ProcessMessageReply_ReverseOfMemory()
{
	BOOL Reply = -1;
	Reply = MSG_ReadLong();

	if ( Reply == TRUE)
	{
		
		int ItemPos = MSG_ReadLong();
		GTH_UseItem_Delete(ItemPos);		
		
		
		g_cgv.myCharacterInfo->pcJob = MSG_ReadLong();

		
		for (int idx = 0; idx < GEN_NUMBER; idx ++)
			g_cgv.myCharacterInfo->genCapability[idx] = MSG_ReadLong();		
		

		
		for (idx = 0; idx < MAX_NUMBER_OF_HOTKEY; idx ++)
		{
			g_cgv.myCharacterInfo->hotkeyType[idx] = MSG_ReadLong();
			g_cgv.myCharacterInfo->hotkeyIdx[idx] = MSG_ReadLong();		
		}
		
		
		int InitSKillNumber = MSG_ReadLong();
		for (idx = 0; idx < InitSKillNumber; idx++)
			g_SkillLogic.GTH_DeleteSKill(MSG_ReadLong());


		
		g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 507));

	}
	else
	{
		enum enumCashMallItem_Error Error = static_cast<enum enumCashMallItem_Error>(MSG_ReadLong());
		switch(Error) 
		{
		case CASHMALL_ITEM_ERROR_0:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 508));
			break;
		case CASHMALL_ITEM_ERROR_1:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 509));
			break;
		case CASHMALL_ITEM_ERROR_2:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 506));
			break;
		case CASHMALL_ITEM_ERROR_3:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 510));
			break;
		case CASHMALL_ITEM_ERROR_4:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 511));
			break;
		case CASHMALL_ITEM_ERROR_5:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 512));
			break;									
			
		}
	}
}


void GTH_ProcessMessageReply_OptionItemEdit()
{			
	BOOL bError = MSG_ReadLong();

	if( bError == FALSE )
	{		
		g_cgv.myCharacterInfo->mouseInventory		= MSG_ReadLong();
		g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx	= MSG_ReadLong();

		
		if ( g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx != -1)
			g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_EQUIP_ITEM ), GTH_SOUND_PLAY_ONCE );

		
		else
			g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_ITEM_CLICK_2 ), GTH_SOUND_PLAY_ONCE );

		
		g_ifMng->m_CashMallItem->SortItemOptionList();
	}
	else
	{		
		
		g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 269));
		
	}

}






void GTH_SendMessageRequest_StoneOfSage(const int InvenPos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REQUEST_USE_STONE_OF_SAGE );						
		MSG_WriteLong(&netMessage,	InvenPos);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	
}






void GTH_SendMessageRequest_StoneOfLucky(const int InvenPos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REQUEST_USE_STONE_OF_LUCKY );						
		MSG_WriteLong(&netMessage,	InvenPos);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_SendMessageRequest_StoneOfEachName(const int InvenPos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REQUEST_USE_STONE_OF_EACH_NAME );						
		MSG_WriteLong(&netMessage,	InvenPos);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessageRequest_ReverseOfMemory(const int InvenPos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REQUEST_USE_REVERSE_OF_MEMORY );						
		MSG_WriteLong(&netMessage,	InvenPos);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessageRequest_OptionItemEdit()
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REQUEST_OPTION_ITEM_EDIT );						
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_ProcessMessageReply_OptionItemWinClose()
{
	BOOL bError = MSG_ReadLong();

	g_ifMng->m_CashMallItem->InitCashMallItem();
	g_ifMng->m_CashMallItem->Enable(FALSE);	
	
	g_ifMng->m_itemWin->ClearLockItems();

	if( bError == FALSE )
	{	
		item_t *pSrcitem;
		int itemIdx, inventoryPos;		
		itemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;

		if(itemIdx < 0) 			
			return;
		
		pSrcitem = &g_cgv.myCharacterInfo->item[itemIdx];		
		int stackItemIdx = GTH_StackMyItem(pSrcitem);
		
		
		if(stackItemIdx >= 0)
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += pSrcitem->durability+1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability  >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
			GTH_DeleteMyItem( pSrcitem );
		}
		
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos();
			
			if( inventoryPos >= 0 ) 
				g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
			
			
			else
				GTH_DeleteMyItem( pSrcitem );	
		}		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_EQUIP_ITEM ), GTH_SOUND_PLAY_ONCE );
		
	}
	else
	{		
		
		
		g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 506));
	}
}






void GTH_SendMessageRequest_OptionItemWinClose()
{
	MSG_BeginWriting( &netMessage );	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REQUEST_OPTION_ITEM_WINCLOSE );								
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessageRequest_ItemModify(CashMallItem_OptionStone::enumUseItemMode mode, int SelectOptionIdx)
{
	MSG_BeginWriting( &netMessage );	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, CASH_MALL_ITEM );
		MSG_WriteShort(&netMessage, REQUEST_OPTION_ITEM_MODIFY );								
		MSG_WriteLong(&netMessage, (int)mode);

		if ( mode != CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY)
			MSG_WriteLong(&netMessage, SelectOptionIdx);


		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_ProcessMessageReply_ItemModify()
{
	
	CashMallItem_OptionStone::enumUseItemMode mode;
	mode = static_cast<enum CashMallItem_OptionStone::enumUseItemMode> (MSG_ReadLong());

	switch(mode)
	{
	case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE:
		GTH_ProcessMessageReply_ItemModify_Stone_Of_Sage();
		break;

	case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_LUCKY:
		GTH_ProcessMessageReply_ItemModify_Stone_Of_lucky();
		break;

	case CashMallItem_OptionStone::enumUseItemMode::STONE_OF_EACH_NAME:
		GTH_ProcessMessageReply_ItemModify_Stone_Of_EachName();
		break;

	}	
}









void GTH_ProcessMessageReply_ItemModify_Stone_Of_Sage()
{
	BOOL Reply = FALSE;
	Reply = MSG_ReadLong();
	
	if ( Reply == TRUE )
	{
		item_t *pItem = NULL;
		int ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;		
		
		if ( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
			return;
		
		
		

		pItem = &g_cgv.myCharacterInfo->item[ItemIdx];		
		
		char szOptionText[256];
		char szMsg[256];
		g_ifMng->m_CashMallItem->GetItemOptionString(szOptionText, 256, 
			g_ifMng->m_CashMallItem->m_OptionArryIdx);	
		
		
		MSG_ReadItem( pItem );		
		g_ifMng->m_CashMallItem->SortItemOptionList();
		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_GEN_CLICK ), 
			GTH_SOUND_PLAY_ONCE );
		
		char szOptionText2[256];		
		g_ifMng->m_CashMallItem->GetItemOptionString(szOptionText2, 256, 
			g_ifMng->m_CashMallItem->m_OptionArryIdx);
		
		
		_snprintf( szMsg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 513), szOptionText, szOptionText2);
		szMsg[255] = NULL;		
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szMsg, MSG_BUTTONTYPE_OK, 
			IF_MESSAGE_MODIFY_OPTION_SUCCECS);
		
		
		GTH_UseItem_Delete(g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx);				
		
	}
	else
	{
		
		enum enumCashMallItem_Error Error = static_cast<enum enumCashMallItem_Error>(MSG_ReadLong());
		switch(Error) 
		{
		case CASHMALL_ITEM_ERROR_0:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 506));
			
			break;
		case CASHMALL_ITEM_ERROR_1:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 514));
			break;
		case CASHMALL_ITEM_ERROR_2:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 515));
			break;
		case CASHMALL_ITEM_ERROR_3:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 510));
			break;
		case CASHMALL_ITEM_ERROR_4:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 529));
			break;

			
		case CASHMALL_ITEM_ERROR_5:			
			GTH_ItemModify_DestroyItem();			
			break;

			
			
		case CASHMALL_ITEM_ERROR_6:			
			GTH_ItemModify_FailedItem();		
		case CASHMALL_ITEM_ERROR_7:			


			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 556));
			break;
			break;
		default:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 516));			
			
		}
		
	}
}
void GTH_ItemModify_DestroyItem()
{		
	int OptionItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;
	item_t *pItem = NULL;

	if ( OptionItemIdx >= 0 || OptionItemIdx < MAX_NUMBER_OF_OWNITEM)
	{	
		pItem = &g_cgv.myCharacterInfo->item[OptionItemIdx];	
		if ( pItem != NULL)		
			GTH_DeleteMyItem( pItem );		
	}	
	GTH_UseItem_Delete(g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx);
	
	

	g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx = -1;
	g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx = -1;		
	g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_CashMallItem_DestroyRate = 0.0f;
	memset(g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iSortOptionIdx, -1, MAX_NUMBER_OF_ITEM_OPTION);




	
	
	g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
		g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 517), MSG_BUTTONTYPE_OK, IF_MESSAGE_MODIFY_ITEM_DESTROY);

}

void GTH_ProcessMessageReply_ItemModify_Stone_Of_lucky()
{
	BOOL Reply = FALSE;
	Reply = MSG_ReadLong();
	
	if ( Reply == TRUE )
	{
		item_t *pItem = NULL;
		int ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;		
		
		if ( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
			return;
		
		pItem = &g_cgv.myCharacterInfo->item[ItemIdx];		
		
		MSG_ReadItem( pItem );
		
		char szOptionText[256];
		char szMsg[256];

		g_ifMng->m_CashMallItem->GetItemOptionString(szOptionText, 256, 
			pItem->optionNumber-1 );
		g_ifMng->m_CashMallItem->SortItemOptionList();
		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_GEN_CLICK ), 
			GTH_SOUND_PLAY_ONCE );		
		
		
		_snprintf( szMsg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 518), szOptionText);
		szMsg[255] = NULL;
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szMsg, MSG_BUTTONTYPE_OK, 
			IF_MESSAGE_MODIFY_OPTION_SUCCECS);
		
		
		GTH_UseItem_Delete(g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx);		
	}
	else
	{
		
		enum enumCashMallItem_Error Error = static_cast<enum enumCashMallItem_Error>(MSG_ReadLong());
		switch(Error) 
		{
		case CASHMALL_ITEM_ERROR_0:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 506));
			break;
		case CASHMALL_ITEM_ERROR_1:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 514));
			break;
		case CASHMALL_ITEM_ERROR_2:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 515));
			break;
		case CASHMALL_ITEM_ERROR_3:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 510));
			break;
		case CASHMALL_ITEM_ERROR_4:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 529));
			break;		
			
		case CASHMALL_ITEM_ERROR_5:			
			GTH_ItemModify_DestroyItem();			
			break;

			
			
		case CASHMALL_ITEM_ERROR_6:			
			GTH_ItemModify_FailedItem();	
			break;
		case CASHMALL_ITEM_ERROR_7:			



			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 555));
			break;
			
		default:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 516));
			
		}
		
	}
}

void GTH_ProcessMessageReply_ItemModify_Stone_Of_EachName()
{
	BOOL Reply = FALSE;
	Reply = MSG_ReadLong();
	
	if ( Reply == TRUE )
	{
		item_t *pItem = NULL;
		int ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;		
		
		if ( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
			return;
		
		pItem = &g_cgv.myCharacterInfo->item[ItemIdx];
		
		char szOptionText[256];
		char szMsg[256];
		g_ifMng->m_CashMallItem->GetItemOptionString(szOptionText, 256, 
			g_ifMng->m_CashMallItem->m_OptionArryIdx);	
		
		MSG_ReadItem( pItem );		
		g_ifMng->m_CashMallItem->SortItemOptionList();
		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_GEN_CLICK ), 
			GTH_SOUND_PLAY_ONCE );		
		
		
		_snprintf( szMsg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 520), szOptionText);						
		szMsg[255] = NULL;
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szMsg, MSG_BUTTONTYPE_OK, 
			IF_MESSAGE_MODIFY_OPTION_SUCCECS);
		
		
		GTH_UseItem_Delete(g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx);		
	}
	else
	{
		
		enum enumCashMallItem_Error Error = static_cast<enum enumCashMallItem_Error>(MSG_ReadLong());
		switch(Error) 
		{
		case CASHMALL_ITEM_ERROR_0:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 506));
			break;
		case CASHMALL_ITEM_ERROR_1:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 514));
			break;
		case CASHMALL_ITEM_ERROR_2:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 515));
			break;
		case CASHMALL_ITEM_ERROR_3:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 510));
			break;
		case CASHMALL_ITEM_ERROR_4:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 529));
			break;		
		case CASHMALL_ITEM_ERROR_5:				
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 519));
			break;		
			
		default:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 516));
			
			
		}
		
	}

}






void GTH_ItemModify_FailedItem(void)
{
	item_t *pItem = NULL;
	int ItemIdx = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iEditOptionItemIdx;		
	
	if ( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
		return;
	
	pItem = &g_cgv.myCharacterInfo->item[ItemIdx];

	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_GEN_CLICK ), 
		GTH_SOUND_PLAY_ONCE );				
	
	if ( g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemMode == 
		CashMallItem_OptionStone::enumUseItemMode::STONE_OF_SAGE)
	{		
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 558 ), MSG_BUTTONTYPE_OK, 
			IF_MESSAGE_MODIFY_OPTION_SUCCECS);
	}
	else
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 557 ), MSG_BUTTONTYPE_OK, 
			IF_MESSAGE_MODIFY_OPTION_SUCCECS);
	}
	
	
	GTH_UseItem_Delete(g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx);	
}