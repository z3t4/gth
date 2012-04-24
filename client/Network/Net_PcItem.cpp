 #include "../global_def.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 



void GTH_ProcessMessage_ItemSystem()
{
	int command;
	
	command = MSG_ReadByte();

	switch( command )
	{
		
	case DEPOT_OPEN :
		GTH_ProcessMessage_ItemSystem_DepotOpen();
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
		GTH_ProcessMessage_ItemSystem_PrecocityOpen();
		break;
	case PRECOCITY_ITEM :
		GTH_ProcessMessage_ItemSystem_PrecocityItem();
		break;
	case PRECOCITY_COMPLETE :
		GTH_ProcessMessage_ItemSystem_PrecocityComplete();
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
		GTH_ProcessMessage_ItemSystem_EventCraftLoad();
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
	case BOOTH_BUY_UPDATE :
		GTH_ProcessMessage_ItemSystem_BoothBuyUpdate();
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

	
	case QUESTSCROLL_OBTAINOPEN :
		GTH_ProcessMessage_QuestScroll_ObtainOpen();
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
	case QUESTSCROLL_COMPLETE :
		GTH_ProcessMessage_QuestScrollComplete();
		break;
	case QUESTSCROLL_COMBOLISTCHANGE :
		GTH_ProcessMessage_QuestScroll_ComboListChange();
		break;
		
	
	case AUTOQUESTSCROLL_OBTAINOPEN :
		GTH_ProcessMessage_AutoQuestScroll_ObtainOpen();
		break;

	case AUTOQUESTSCROLL_COMPLETE :
		GTH_ProcessMessage_AutoQuestScrollComplete();
		break;

	case AUTOQUESTSCROLL_NPCINVENTORYOPEN :
		GTH_ProcessMessage_ItemSystem_NpcInventoryOpen();
		break;

	case AUTOQUESTSCROLL_NPCINVENTORYCLOSE :
		GTH_ProcessMessage_ItemSystem_NpcInventoryClose();
		break;

	case AUTOQUESTSCROLL_NPCINVENTORYITEM :
		GTH_ProcessMessage_ItemSystem_AutoCraftItem();
		break;

	case AUTOQUESTSCROLL_NPCINVENTORYPREPARE :
		GTH_ProcessMessage_ItemSystem_AutoCraftPrepare();
		break;
	case AUTOQUESTSCROLL_ERROR:
		GTH_ProcessMessage_ItemSystem_AutoError();
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







void GTH_SendMessage_ItemSystem_DepotGetItem( int pos, int numStack )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DEPOT_GETITEM );
		MSG_WriteShort(&netMessage, pos);
		MSG_WriteShort(&netMessage, numStack);
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_DepotItem( int pos )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DEPOT_ITEM );
		MSG_WriteShort(&netMessage, pos);
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_DepotNak( int type, int depotSE )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DEPOT_NAK );
		MSG_WriteByte(&netMessage, type);
		MSG_WriteLong(&netMessage, depotSE);
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ItemSystem_DepotOpen()
{
	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_depotWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_depotWin );
}

void GTH_ProcessMessage_ItemSystem_DepotGetItem()
{
	GTH_UnlockSending();

	int reply, error;
	int depotPos, itemIdx, stackItemIdx, emptyDepotPos;
	int mouseInventory;
	item_t* item;
	item_t stackItem1, stackItem2;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		depotPos = MSG_ReadByte();
		itemIdx = g_cgv.myCharacterInfo->depot[depotPos];
		if( reply == 5 )
		{
			mouseInventory = MSG_ReadByte();
			MSG_ReadItem( &stackItem1 );
			MSG_ReadItem( &stackItem2 );
		}
	}
	else
	{
		error = MSG_ReadByte();
	}

	switch ( reply )
	{
	case 0:
		break;
	case 1:
		g_cgv.myCharacterInfo->depot[depotPos] = -1;
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		break;
		
	case 2:
		g_cgv.myCharacterInfo->depot[depotPos] = g_cgv.myCharacterInfo->mouseInventory;
		g_cgv.myCharacterInfo->mouseInventory = -1;
		break;

	case 3:
		g_cgv.myCharacterInfo->depot[depotPos] = g_cgv.myCharacterInfo->mouseInventory;
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		break;
		
	case 4 :



		break;
	case 5 :
		GTH_AddMyItem( &stackItem2 );
		g_cgv.myCharacterInfo->item[itemIdx].durability = stackItem1.durability;
		g_cgv.myCharacterInfo->mouseInventory = mouseInventory;
		break;
		
	case 6 :
		item = &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->mouseInventory];
		stackItemIdx = GTH_StackMyItemForDepot( item );
		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item->durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
			GTH_DeleteMyItem( item );
		}
		else
		{
			emptyDepotPos = GTH_FindEmptyDepotPos();
			g_cgv.myCharacterInfo->depot[emptyDepotPos] = g_cgv.myCharacterInfo->mouseInventory;
		}
		g_cgv.myCharacterInfo->mouseInventory = -1;
		break;
	case 7 :
		break;
		
	case 8 :
		
		itemIdx = g_cgv.myCharacterInfo->depot[depotPos];
		if( itemIdx >= 0 )
		{
			if( g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx >= 0 )
			{
				GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
			}
		}
		g_cgv.myCharacterInfo->depot[depotPos] = -1;
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,25) , 1, IF_MESSAGE_NONE);
		break;
	}
	if( reply > 0 && itemIdx >= 0 )
	{
		item = &g_cgv.myCharacterInfo->item[itemIdx];

		if( g_itemClassTable[g_itemTable[item->itemTableIdx].classIdx].attackAtt )
		{
			
			g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_ITEM_CLICK_1 ), GTH_SOUND_PLAY_ONCE );
		}
		else
		{
			
			g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_ITEM_CLICK_2 ), GTH_SOUND_PLAY_ONCE );
		}

		
		if( g_cgv.myCharacterInfo->mouseInventory >= 0 ) g_cgv.myCharacterInfo->mouseInventoryType = MOUSEINVEN_TYPE_DEPOT;
		else g_cgv.myCharacterInfo->mouseInventoryType = MOUSEINVEN_TYPE_NONE;
	}
	else
	{
		switch( error )
		{		
		case ERROR_DEPOT_1 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,396) );
			break;
		case ERROR_DEPOT_2 :
			break;
		case ERROR_DEPOT_4 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,589) );
			break;
		}
	}
}

void GTH_ProcessMessage_ItemSystem_DepotItem()
{
	GTH_UnlockSending();

	int reply, error;
	int depotPos, curNak, stackItemIdx, emptyDepotPos;
	item_t *item;
	
	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		depotPos = MSG_ReadByte();
		curNak = MSG_ReadLong();

		g_cgv.myCharacterInfo->curChargeSE = curNak;

		if( reply == 1 ) 
		{
			g_cgv.myCharacterInfo->depot[depotPos] = g_cgv.myCharacterInfo->mouseInventory;
			g_cgv.myCharacterInfo->mouseInventory = -1;
		}
		
		else
		{
			item = &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->mouseInventory];
			stackItemIdx = GTH_StackMyItemForDepot( item );
			if( stackItemIdx >= 0 )
			{
				g_cgv.myCharacterInfo->item[stackItemIdx].durability += item->durability + 1;
				if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
					g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
				GTH_DeleteMyItem( item );
			}
			else
			{
				emptyDepotPos = GTH_FindEmptyDepotPos();
				g_cgv.myCharacterInfo->depot[emptyDepotPos] = g_cgv.myCharacterInfo->mouseInventory;
			}
			g_cgv.myCharacterInfo->mouseInventory = -1;
			g_cgv.myCharacterInfo->mouseInventoryType = MOUSEINVEN_TYPE_NONE;
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
		case ERROR_DEPOT_1 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,396) );
			break;
		case ERROR_DEPOT_2 :
			break;
		case ERROR_DEPOT_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,590) );
			break;
		case ERROR_DEPOT_4 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,589) );
			break;
		}
	}
}

void GTH_ProcessMessage_ItemSystem_DepotNak()
{
	GTH_UnlockSending();

	int reply, error;

	reply = MSG_ReadByte();

	if( reply )
	{
		g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();
		g_cgv.myCharacterInfo->depotSE = MSG_ReadLong();
	}
	else
	{
		error = MSG_ReadByte();
	}
}





void GTH_SendMessage_ItemSystem_ChatCommand(int nEventIndex)
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_LOADEVENT);
		MSG_WriteLong(&netMessage, nEventIndex);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_EventCraftClose()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_CLOSE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_EventCraftItem( int posType, int pos )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_ITEM );
		MSG_WriteShort(&netMessage, posType);
		MSG_WriteShort(&netMessage, pos);
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_EventCraftPrepare()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_PREPARE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_EventCraftComplete()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_COMPLETE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void GTH_ProcessMessage_ItemSystem_EventCraftOpen()
{
	g_ifMng->m_EventCraftWin->InitCraft();

	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_EventCraftWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_craftWin );

}

void GTH_ProcessMessage_ItemSystem_EventCraftClose()
{
	GTH_UnlockSending();

	g_ifMng->m_EventCraftWin->Enable( false );
	g_ifMng->m_EventCraftWin->UndoCraft();
}


void GTH_ProcessMessage_ItemSystem_EventCraftItem()
{
	GTH_UnlockSending();
	int reply, error;
	int posType, pos;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		posType = MSG_ReadChar();
		pos = MSG_ReadByte();
		g_cgv.myCharacterInfo->mouseInventory = MSG_ReadShort();
		g_cgv.myCharacterInfo->craftSourceInventory[pos] = MSG_ReadShort();

		g_ifMng->m_EventCraftWin->m_nSelectEventCraftIndex = g_ifMng->m_EventCraftWin->GetEventCraftFormula();
	}
	else
	{
		error = MSG_ReadByte();
	}
}

void GTH_ProcessMessage_ItemSystem_EventCraftPrepare()
{
	GTH_UnlockSending();


	int reply, error;

	
	reply = MSG_ReadByte();

	if( reply )
	{
		g_ifMng->m_EventCraftWin->m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		g_ifMng->m_EventCraftWin->StartCraft( 10000.0f );
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_0 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,80) );
			break;
		}
		
	}
}

void GTH_ProcessMessage_ItemSystem_EventCraftComplete()
{
	GTH_UnlockSending();

	int i,  itemIdx, reply, error,craftIdx;
	
	reply		= MSG_ReadByte();
	craftIdx	= MSG_ReadByte();

	if( reply )
	{
		long curChargeSE = MSG_ReadLong();
		if(curChargeSE < 0) 
		{
			curChargeSE =0;
		}

		g_cgv.myCharacterInfo->curChargeSE = curChargeSE;
		
		
		for( i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[i];
			if( itemIdx < 0 ) continue;
			GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_cgv.myCharacterInfo->craftSourceInventory[i] = -1;
		}

		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,79) );

		
		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
									35, 
									g_pApp->m_myCharacter->position,
									NULL, 
									true, 
									g_pApp->m_myCharacter->entityType, 
									g_pApp->m_myCharacter->idxOnServer,
									0.0f, 
									0.0f, 
									0.0f, 
									-1.0f,
									false );
		
	
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_0 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,80) );
			break;

		}
	}

	g_ifMng->m_EventCraftWin->InitCraft();
	g_ifMng->m_EventCraftWin->m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
}

void GTH_ProcessMessage_ItemSystem_EventCraftLoad()
{
	GTH_UnlockSending();

	char szTemp[256];

	short sEventIdx;
	int n1PointCnt,n3PointCnt,nTotalPoint;

	sEventIdx = MSG_ReadShort();

	n1PointCnt = MSG_ReadLong();
	n3PointCnt = MSG_ReadLong();
	nTotalPoint = MSG_ReadLong();


	wsprintf(szTemp, g_LPACK.GetMassage(9,91),	g_cgv.myCharacterInfo->name,nTotalPoint); 
	

	g_ifMng->AddSysMessage(szTemp); 
}












void GTH_SendMessage_ItemSystem_CraftClose()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_CLOSE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_CraftItem( int posType, int pos,int craftIdx )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_ITEM );
		MSG_WriteShort(&netMessage, posType);
		MSG_WriteShort(&netMessage, pos);
		MSG_WriteShort(&netMessage, craftIdx);
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_CraftPrepare()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_PREPARE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_CraftComplete()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_COMPLETE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void GTH_ProcessMessage_ItemSystem_CraftOpen()
{
	g_ifMng->m_craftWin->InitCraft();

	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_craftWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_craftWin );
}

void GTH_ProcessMessage_ItemSystem_CraftClose()
{
	GTH_UnlockSending();

	g_ifMng->m_craftWin->Enable( false );
	g_ifMng->m_craftWin->UndoCraft();
}


void GTH_ProcessMessage_ItemSystem_CraftItem()
{
	GTH_UnlockSending();
	int reply, error;
	int posType, pos;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		posType			= MSG_ReadChar();
		pos				= MSG_ReadByte();
		g_cgv.myCharacterInfo->mouseInventory = MSG_ReadShort();
		if( !posType ) 
		{
			g_cgv.myCharacterInfo->craftSourceInventory[pos] = MSG_ReadShort();
		}
		else 
		{
			g_cgv.myCharacterInfo->craftResultInventory[pos] = MSG_ReadShort();
		}
	}
	else
	{
		error = MSG_ReadByte();
	}
}

void GTH_ProcessMessage_ItemSystem_CraftPrepare()
{
	GTH_UnlockSending();

	char szTemp[256], szTemp2[256];
	int reply, error;
	int itemTableIdx1, itemTableIdx2, nak;
	float pro;
	
	reply = MSG_ReadByte();

	if( reply )
	{
		itemTableIdx1 = MSG_ReadShort();
		itemTableIdx2 = MSG_ReadShort();	
		nak = MSG_ReadLong();
		pro = MSG_ReadFloat();
		
		g_ifMng->m_craftWin->m_nCraftIdx = MSG_ReadLong();

		strcpy( szTemp, g_itemTable[itemTableIdx1].name );
		if( itemTableIdx2 >= 0 )
		{
			strcat( szTemp, ", ");
			strcat( szTemp, g_itemTable[itemTableIdx2].name );
		}
		
		sprintf( szTemp2, g_LPACK.GetMassage(0,601), szTemp, nak, pro );
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp2, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_CRAFT_COMPLETE );
		g_ifMng->m_craftWin->m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_0 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,602) );
			break;
			
		case ERROR_ITEMSYSTEM_1 :
			nak = MSG_ReadLong();
			sprintf( szTemp, g_LPACK.GetMassage(0,603), nak );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
			break;
			
		case ERROR_ITEMSYSTEM_2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,604) );
			break;
		case ERROR_ITEMSYSTEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		
		case ERROR_ITEMSYSTEM_24 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,230) );
			break;
		}
	}
}

void GTH_ProcessMessage_ItemSystem_CraftComplete()
{
	GTH_UnlockSending();

	char szTemp[256];
	item_t item1, item2;
	int i, success, itemIdx, reply, error, nak;
	
	reply = MSG_ReadByte();

	if( reply )
	{
		success = MSG_ReadByte();
		g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();

		
		for( i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->craftSourceInventory[i];
			if( itemIdx < 0 ) continue;
			GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_cgv.myCharacterInfo->craftSourceInventory[i] = -1;
		}

		if( success )
		{
			MSG_ReadItem( &item1 );
			MSG_ReadItem( &item2 );
			
			if( item1.itemTableIdx >= 0 )
			{
				itemIdx = GTH_AddMyItem( &item1 );
				g_cgv.myCharacterInfo->craftResultInventory[0] = itemIdx;
			}
			if( item2.itemTableIdx >= 0 ) 
			{
				itemIdx = GTH_AddMyItem( &item2 );
				g_cgv.myCharacterInfo->craftResultInventory[1] = itemIdx;
			}
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,605) );

			
			g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
										35, 
										g_pApp->m_myCharacter->position,
										NULL, 
										true, 
										g_pApp->m_myCharacter->entityType, 
										g_pApp->m_myCharacter->idxOnServer,
										0.0f, 
										0.0f, 
										0.0f, 
										-1.0f,
										false );

			
		}
		else
		{
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,606) );
			
			g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
										36, 
										g_pApp->m_myCharacter->position,
										NULL, 
										true, 
										g_pApp->m_myCharacter->entityType, 
										g_pApp->m_myCharacter->idxOnServer,
										0.0f, 
										0.0f, 
										0.0f, 
										-1.0f,
										false );
			
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_0 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,602) );
			break;
			
		case ERROR_ITEMSYSTEM_1 :
			nak = MSG_ReadLong();
			sprintf( szTemp, g_LPACK.GetMassage(0,603), nak );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
			break;
			
		case ERROR_ITEMSYSTEM_2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,604) );
			break;
		case ERROR_ITEMSYSTEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		}
	}

	g_ifMng->m_craftWin->m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
}










void GTH_SendMessage_ItemSystem_UpgradeClose()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_CLOSE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_UpgradeItem( int posType, int pos )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_ITEM );
		MSG_WriteShort(&netMessage, posType);
		MSG_WriteShort(&netMessage, pos);
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_UpgradePrepare( int selectedOptionIdx )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_PREPARE );
		MSG_WriteByte(&netMessage, selectedOptionIdx);
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_UpgradeComplete(  int selectedOptionIdx ) 
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_COMPLETE );
		MSG_WriteByte(&netMessage, selectedOptionIdx);
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}


void GTH_SendMessage_ItemSystem_NpcInventoryClose()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROLL_NPCINVENTORYCLOSE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_AutoCraftItem( int posType, int pos )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROLL_NPCINVENTORYITEM );
		MSG_WriteShort(&netMessage, posType);
		MSG_WriteShort(&netMessage, pos);
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_AutoCraftPrepare()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROLL_NPCINVENTORYPREPARE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void GTH_SendMessage_ItemSystem_DeletePrepare( int selectedOptionIdx )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DELETE_PREPARE );
		MSG_WriteByte(&netMessage, selectedOptionIdx);
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_DeleteComplete( int selectedOptionIdx )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DELETE_COMPLETE );
		MSG_WriteByte(&netMessage, selectedOptionIdx);
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}





void GTH_ProcessMessage_ItemSystem_UpgradeOpen()
{
	g_ifMng->m_upgradeWin->InitUpgrade();

	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_upgradeWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_upgradeWin );
}

void GTH_ProcessMessage_ItemSystem_UpgradeClose()
{
	GTH_UnlockSending();

	g_ifMng->m_upgradeWin->Enable( false );
	g_ifMng->m_upgradeWin->UndoUpgrade();
}

void GTH_ProcessMessage_ItemSystem_UpgradeItem()
{
	GTH_UnlockSending();
	int reply, error;
	int posType, pos;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		posType = MSG_ReadChar();
		pos = MSG_ReadByte();
		g_cgv.myCharacterInfo->mouseInventory = MSG_ReadShort();
		if( !posType )
		{
			g_cgv.myCharacterInfo->upgradeSourceInventory[pos] = MSG_ReadShort();

			
			if( pos == 0 )
			{
				





				g_ifMng->m_upgradeWin->SetOptionComboBox();
			}
			else if( pos == 1 )
			{
				
				
				if( g_cgv.myCharacterInfo->item[ g_cgv.myCharacterInfo->upgradeSourceInventory[1] ].itemTableIdx == 861 )
					g_ifMng->m_upgradeWin->m_bDeleteOption = true;
			}
		}
		else
		{
			g_cgv.myCharacterInfo->upgradeResultInventory = MSG_ReadShort();
		}
	}
	else
	{
		error = MSG_ReadByte();
	}
}


void GTH_ProcessMessage_ItemSystem_NpcInventoryOpen()
{
	g_ifMng->m_npcInvWin->InitNpcInventory();

	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_npcInvWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_npcInvWin );

}

void GTH_ProcessMessage_ItemSystem_NpcInventoryClose()
{
	GTH_UnlockSending();

	g_ifMng->m_npcInvWin->Enable( false );
	g_ifMng->m_npcInvWin->UndoNpcInventory();
}

void GTH_ProcessMessage_ItemSystem_AutoCraftItem()
{
	GTH_UnlockSending();
	int reply, error;
	int posType, pos;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		posType = MSG_ReadChar();
		pos = MSG_ReadByte();
		g_cgv.myCharacterInfo->mouseInventory = MSG_ReadShort();
		if( !posType )
		{
			g_cgv.myCharacterInfo->upgradeSourceInventory[pos] = MSG_ReadShort();

#ifdef _NEW_SERVER

			
			if( pos == 0 )
			{
				






			}
			else if( pos == 1 )
			{
				
				


			}

#else

			
			if( pos == 0 || pos == 1 )
			{
				
				if( g_cgv.myCharacterInfo->upgradeSourceInventory[1] == -1 )
					return;
				if( g_cgv.myCharacterInfo->item[ g_cgv.myCharacterInfo->upgradeSourceInventory[1] ].itemTableIdx != 861 )
					return;


			}

#endif
		}
		else
		{
			g_cgv.myCharacterInfo->upgradeResultInventory = MSG_ReadShort();
		}
	}
	else
	{
		error = MSG_ReadByte();
	}
}

void GTH_ProcessMessage_ItemSystem_AutoCraftPrepare()
{
	GTH_UnlockSending();


	int reply, error;

	
	reply = MSG_ReadByte();

	if( reply )
	{
		g_ifMng->m_npcInvWin->Enable(false);
		g_ifMng->m_npcInvWin->m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_0 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,602) );
			break;
		case ERROR_ITEMSYSTEM_21 :
		
		
			break;
		case ERROR_ITEMSYSTEM_22 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		case ERROR_ITEMSYSTEM_23 :
			
		
		
			break;
		}
	}
}

void GTH_ProcessMessage_ItemSystem_AutoError(void)
{
	GTH_UnlockSending();
	int nError = MSG_ReadByte();

	switch( nError ) {
	case ERROR_ITEMSYSTEM_22:
	
		g_ifMng->AddSysMessage( 
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 541 ),
			GTHCOLOR_QUESTMESSAGE );
		break;
	
	case ERROR_ITEMSYSTEM_2:
		
		g_ifMng->SetMessage(
			g_LPACK.GetMassage( 0, 322 ),
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 542),
			MSG_BUTTONTYPE_OK,
			IF_MESSAGE_NONE );
		break;

	case ERROR_ITEMSYSTEM_3:
		g_ifMng->SetMessage(
			g_LPACK.GetMassage( 0, 322 ),
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 543 ),	
			MSG_BUTTONTYPE_OK,
			IF_MESSAGE_NONE );
		break;
	case ERROR_ITEMSYSTEM_26:
		
		g_ifMng->SetMessage( 
			g_LPACK.GetMassage( 0, 322 ),
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 544,
			MSG_BUTTONTYPE_OK,
			IF_MESSAGE_NONE));	
		break;
	}
}



void MakeOptionStoneName( int type, int rarity, char* szTemp )
{
	if ( type == 0 )
	{
		switch( rarity )
		{
		case 0 :	
			strcpy( szTemp, g_itemTable[444].name );
			break;
		case 1 :	
			strcpy( szTemp, g_itemTable[447].name );
			break;
		case 2 : case 3 : case 4 :	
			strcpy( szTemp, g_itemTable[451].name );
			break;

		default :
			strcpy( szTemp, "" );
		}
	}
	else if ( type == 1 )
	{
		switch( rarity )
		{
		case 0 :	
			strcpy( szTemp, g_itemTable[464].name );
			break;
		case 1 :	
			strcpy( szTemp, g_itemTable[467].name );
			break;
		case 2 : case 3 : case 4 :	
			strcpy( szTemp, g_itemTable[471].name );
			break;

		default :
			strcpy( szTemp, "" );
		}
	}
	else if ( type == 3 )
	{
		switch( rarity )
		{
		case 0 :

		case 1 :

		case 2 :

		default :
			strcpy( szTemp, "" );
			break;
		}
	}
}


void GTH_ProcessMessage_ItemSystem_UpgradePrepare()
{
	GTH_UnlockSending();

	
	
	char szTemp[256+1]; 
	char szNak[128+1]; 
	char szItemName[128+1]; 
	char szOptionName[128+1];
	
	

	int reply, error;
	int itemTableIdx, nak, currentGrade, nextGrade;
	int proStrIdx, vanishProStrIdx;
	float pro, vanishPro, downGradePro;
	int optionIdx;
	
	reply = MSG_ReadByte();

	if( reply == 2 ) 
	{
		itemTableIdx = MSG_ReadShort();		
		nak = MSG_ReadLong();				
		pro = MSG_ReadFloat();				
		vanishPro = MSG_ReadFloat();		
		downGradePro = MSG_ReadFloat();		
		currentGrade = MSG_ReadShort();		
		nextGrade = MSG_ReadShort();		
		
		optionIdx = MSG_ReadShort();		

		
		
		_snprintf( szItemName, 256, g_itemTable[itemTableIdx].name );
		szTemp[256] = NULL;

		
		if( pro >= 100.0f ) proStrIdx = 0;
		else if( pro < 100.0f && pro >= 80.0f ) proStrIdx = 1;
		else if( pro < 80.0f && pro >= 40.0f ) proStrIdx = 2;
		else if( pro < 40.0f && pro >= 20.0f ) proStrIdx = 3;
		else if( pro < 20.0f && pro >= 10.0f ) proStrIdx = 4;
		else if( pro < 10.0f && pro >= 5.0f ) proStrIdx = 5;
		else proStrIdx = 6;
		
		if( vanishPro <= 0.0f ) vanishProStrIdx = 0;
		else if( vanishPro > 0.0f && vanishPro <= 20.0f ) vanishProStrIdx = 1;
		else if( vanishPro > 20.0f && vanishPro <= 30.0f ) vanishProStrIdx = 2;
		else vanishProStrIdx = 3;
		
		
		
		
		_snprintf( szTemp, 256, g_LPACK.GetMessage(0, 711), szItemName, nak, g_LPACK.GetMassage(0,664 + proStrIdx), g_LPACK.GetMassage(0,671 + vanishProStrIdx) );
		szTemp[256] = NULL;

		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_UPGRADE_COMPLETE );
		g_ifMng->m_upgradeWin->m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	else if( reply == 1 ) 
	{
		itemTableIdx = MSG_ReadShort();		
		nak = MSG_ReadLong();				
		pro = MSG_ReadFloat();				
		vanishPro = MSG_ReadFloat();		
		downGradePro = MSG_ReadFloat();		
		currentGrade = MSG_ReadShort();		
		nextGrade = MSG_ReadShort();		
		
		optionIdx = MSG_ReadShort();		
		
				
		
		













		_snprintf( szItemName, 128, g_itemTable[itemTableIdx].name );	
		szItemName[128] = NULL;

		_snprintf( szNak, 128, g_ifMng->GetNumberString( nak ) );
		szNak[128] = NULL;

		_snprintf( szOptionName, 128, g_optionTable[g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->upgradeSourceInventory[0]].optionIdx[optionIdx]].name );
		szOptionName[128] = NULL;


		_snprintf( szTemp, 256,
				 g_LPACK.GetMassage(0,784),
				 szItemName,
				 szOptionName,
				 nextGrade,
				 pro,
				 szItemName,
				 vanishPro,
				 downGradePro,
				 szNak );
		szTemp[256] = NULL;

		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_UPGRADE_COMPLETE );
		g_ifMng->m_upgradeWin->m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	else
	{
		error = MSG_ReadByte();
		nak = MSG_ReadLong();
		currentGrade = MSG_ReadShort(); 
		nextGrade = MSG_ReadShort();
		itemTableIdx = MSG_ReadShort();
		optionIdx = MSG_ReadShort();

		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_12 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,638) );
			break;
			
		case ERROR_ITEMSYSTEM_1 :
			sprintf( szTemp, g_LPACK.GetMassage(0,603), nak );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
			break;
			
		case ERROR_ITEMSYSTEM_2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,604) );
			break;
		case ERROR_ITEMSYSTEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		case ERROR_ITEMSYSTEM_13 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,639) );
			break;
		case ERROR_ITEMSYSTEM_14 :
			
			break;

		case ERROR_ITEMSYSTEM_21 :
			{
				if ( itemTableIdx == -1 )
				{
					g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,638) );
					break;
				}

				
				int optSphereType = OPTION_GetOptionStoneGrade( &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->upgradeSourceInventory[0]] );

				switch ( optSphereType )
				{
				case OPTION_SPHERE_LEAF :
					strcpy( szOptionName, "&c5" );		
					strcat( szOptionName, g_itemTable[451].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 790 ), szOptionName );
					break;
				case OPTION_SPHERE_MAD :
					strcpy( szOptionName, "&c8" );		
					strcat( szOptionName, g_itemTable[452].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 791 ), szOptionName );
					break;
				case OPTION_SPHERE_WAVE :
					strcpy( szOptionName, "&c4" );		
					strcat( szOptionName, g_itemTable[453].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 792 ), szOptionName );
					break;
				case OPTION_SPHERE_FIRE :
					strcpy( szOptionName, "&c7" );		
					strcat( szOptionName, g_itemTable[454].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 793 ), szOptionName );
					break;
				case OPTION_SPHERE_ART :
					strcpy( szOptionName, "&c2" );		
					strcat( szOptionName, g_itemTable[455].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 794 ), szOptionName );
					break;

				default :	
					sprintf( szTemp, "%s", g_LPACK.GetMassage(0,638) );
					break;
				}

				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
				break;
			}

		
		case ERROR_ITEMSYSTEM_25:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,237) );
			break;
		}
	}
}

void GTH_ProcessMessage_ItemSystem_UpgradeComplete()
{
	GTH_UnlockSending();

	
	char szTemp[256], szItemName[128], szOptionName[128];

	item_t item;
	item_t *sourceItem;
	
	int i, success, itemIdx, reply, error, nak;

	
	int itemTableIdx, optionIdx, nextGrade, prevCurrentGrade, currentGrade;
	bool bDeleteItem = false;
	bool bDownGrade = false;
	
	reply = MSG_ReadByte();

	if( reply )
	{
		success = MSG_ReadByte();
		g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();

		
		itemTableIdx = MSG_ReadShort();
		optionIdx = MSG_ReadShort();
		nextGrade = MSG_ReadShort();

		
		
		memset( szItemName, 0, sizeof( szItemName ) );
		if( g_cgv.myCharacterInfo->upgradeSourceInventory[0] >= 0 )
		{
			sourceItem = &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->upgradeSourceInventory[0]];

			strcpy( szItemName, sourceItem->name );
			
			strcpy( szOptionName, g_optionTable[sourceItem->optionIdx[optionIdx]].name );

			prevCurrentGrade = sourceItem->optionValue[optionIdx];

		}

		
		for( i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[i];

			if( itemIdx < 0 ) continue;

			GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_cgv.myCharacterInfo->upgradeSourceInventory[i] = -1;
		}

		
		g_ifMng->m_upgradeWin->ClearOptionComboBox();

		itemIdx = MSG_ReadShort();

		if( itemIdx >= 0 )
		{
			MSG_ReadItem( &item );
			if( item.itemTableIdx >= 0 )
			{
				itemIdx = GTH_AddMyItem( &item );
				g_cgv.myCharacterInfo->upgradeResultInventory = itemIdx;

				currentGrade = item.optionValue[optionIdx];

				
				if ( currentGrade < prevCurrentGrade )
					bDownGrade = true;

			}
		}
		else
		{
			sprintf( szTemp, g_LPACK.GetMassage( 0, 659 ), szItemName );
			g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
			bDeleteItem = true;
		}

		strcpy( szItemName, g_itemTable[itemTableIdx].name );

		if ( success )
		{
			sprintf( szTemp, g_LPACK.GetMassage(0,785), szItemName, szOptionName, nextGrade );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );

			
			g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
										37, 
										g_pApp->m_myCharacter->position,
										NULL, 
										true, 
										g_pApp->m_myCharacter->entityType, 
										g_pApp->m_myCharacter->idxOnServer,
										0.0f, 
										0.0f, 
										0.0f, 
										-1.0f,
										false );

		}
		else
		{
			if ( !bDeleteItem )
			{
				if ( bDownGrade )
					sprintf( szTemp, g_LPACK.GetMassage(0,788), szItemName, szOptionName, nextGrade, szItemName, szOptionName, currentGrade );
				else
					sprintf( szTemp, g_LPACK.GetMassage(0,786), szItemName, szOptionName, nextGrade );
			}
			else
				sprintf( szTemp, g_LPACK.GetMassage(0,787), szItemName, szOptionName, nextGrade, szItemName );

			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );

			
			g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
										38, 
										g_pApp->m_myCharacter->position,
										NULL, 
										true, 
										g_pApp->m_myCharacter->entityType, 
										g_pApp->m_myCharacter->idxOnServer,
										0.0f, 
										0.0f, 
										0.0f, 
										-1.0f,
										false );

		}
	}
	else
	{
		error = MSG_ReadByte();
		itemTableIdx = MSG_ReadShort();
		optionIdx = MSG_ReadShort();

		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_12 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,638) );
			break;
			
		case ERROR_ITEMSYSTEM_1 :
			nak = MSG_ReadLong();
			sprintf( szTemp, g_LPACK.GetMassage(0,603), nak );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
			break;
			
		case ERROR_ITEMSYSTEM_2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,604) );
			break;
		case ERROR_ITEMSYSTEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		case ERROR_ITEMSYSTEM_13 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,639) );
			break;

		case ERROR_ITEMSYSTEM_21 :
			{
				if ( itemTableIdx == -1 )
				{
					g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,638) );
					break;
				}

				

				int optSphereType = OPTION_GetOptionStoneGrade( &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->upgradeSourceInventory[0]] );

				switch ( optSphereType )
				{
				case OPTION_SPHERE_LEAF :
					strcpy( szOptionName, "&c5" );		
					strcat( szOptionName, g_itemTable[451].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 790 ), szOptionName );
					break;
				case OPTION_SPHERE_MAD :
					strcpy( szOptionName, "&c8" );		
					strcat( szOptionName, g_itemTable[452].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 791 ), szOptionName );
					break;
				case OPTION_SPHERE_WAVE :
					strcpy( szOptionName, "&c4" );		
					strcat( szOptionName, g_itemTable[453].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 792 ), szOptionName );
					break;
				case OPTION_SPHERE_FIRE :
					strcpy( szOptionName, "&c7" );		
					strcat( szOptionName, g_itemTable[454].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 793 ), szOptionName );
					break;
				case OPTION_SPHERE_ART :
					strcpy( szOptionName, "&c2" );		
					strcat( szOptionName, g_itemTable[455].name );
					strcat( szOptionName, "&c0" );
					sprintf( szTemp, g_LPACK.GetMassage(0,789), g_LPACK.GetMessage( 0, 794 ), szOptionName );
					break;

				default :	
					sprintf( szTemp, "%s", g_LPACK.GetMassage(0,638) );
					break;
				}

				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
				break;
			}

		
		case ERROR_ITEMSYSTEM_25:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,237) );
			break;
		}
	}

	g_ifMng->m_upgradeWin->m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
}


void GTH_ProcessMessage_ItemSystem_DeletePrepare()
{
	GTH_UnlockSending();

	char szTemp[256], optionName[128];
	int reply, error;
	
	reply = MSG_ReadByte();

	if( reply )
	{
		int itemIdx = MSG_ReadShort();
		if( itemIdx < 0 ) return;

		
		
		
		int optionIndex = MSG_ReadByte();

		item_t *item = &g_cgv.myCharacterInfo->item[itemIdx];



		
	
		

		sprintf( szTemp, g_LPACK.GetMassage(0,700), optionName, item->name );
			
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_UPGRADE_COMPLETE );
		g_ifMng->m_upgradeWin->m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	else
	{
		error = MSG_ReadByte();

		switch( error )
		{		
		case ERROR_ITEMSYSTEM_15 : 
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMessage(0, 703) );
			break;
		case ERROR_ITEMSYSTEM_16 : 
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMessage(0, 704) );
			break;
		case ERROR_ITEMSYSTEM_17 : 
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMessage(0, 705) );
			break;
		}
	}
}

void GTH_ProcessMessage_ItemSystem_DeleteComplete()
{
	GTH_UnlockSending();

	char szTemp[256], optionName[128], itemName[128];
	item_t item;
	int i, success, itemIdx, reply, error;
	
	reply = MSG_ReadByte();

	if( reply )
	{
		success = MSG_ReadByte();
		g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();

		int optionIndex = MSG_ReadByte();

		item_t *sourceItem = &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->upgradeSourceInventory[0]];

		
		memset( itemName, 0, sizeof( itemName ) );
		if( g_cgv.myCharacterInfo->upgradeSourceInventory[0] >= 0 )
		{
			
			strcpy( itemName, g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->upgradeSourceInventory[0]].name );
			
			

		}

		
		for( i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->upgradeSourceInventory[i];
			if( itemIdx < 0 ) continue;
			GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_cgv.myCharacterInfo->upgradeSourceInventory[i] = -1;
		}

		
		itemIdx = MSG_ReadShort();
		

		
		
		
		
		if( itemIdx >= 0 )
		{
			MSG_ReadItem( &item );
			if( item.itemTableIdx >= 0 )
			{
				itemIdx = GTH_AddMyItem( &item );
				g_cgv.myCharacterInfo->upgradeResultInventory = itemIdx;
			}
		}
		else
		{
			sprintf( szTemp, g_LPACK.GetMassage( 0, 659 ), itemName );
			g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
		}

		sprintf( szTemp, g_LPACK.GetMassage(0,701), optionName, itemName );


		if( success )
		{	
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
		}
		else
		{
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,702) );
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
		case ERROR_ITEMSYSTEM_15 : 
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMessage(0, 703) );
			break;
		case ERROR_ITEMSYSTEM_16 : 
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMessage(0, 704) );
			break;
		case ERROR_ITEMSYSTEM_17 : 
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMessage(0, 705) );
			break;
		}
	}

	g_ifMng->m_upgradeWin->m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

	
	
	g_ifMng->m_upgradeWin->m_optionCombo->Enable( false );
	g_ifMng->m_upgradeWin->m_optionCombo->ClearString();
	g_ifMng->m_upgradeWin->m_bDeleteOption = false;
}
