#include "../global_def.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 





#include "../CTools.h"
extern CTools gcTools;

int GTH_SendMessage_RequestNpcChat( int idx )
{
	if( !GTH_LockSending() )	return( false );

	MSG_BeginWriting(&netMessage);
	MSG_Clear(&netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_NPCCHAT );
		MSG_WriteShort(&netMessage, idx);
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	
	strcpy( g_ifMng->m_chatWin->m_npcName, g_cgv.pTargetChar->name );
	g_ifMng->m_chatWin->m_npcIdxOnServer = idx;

	return( true );
}

int GTH_ProcessMessage_ReplyNpcChat()
{
	GTH_UnlockSending();

	int reply, error;

	reply = MSG_ReadByte();

	if( reply > 0 )
	{
		g_ifMng->m_chatWin->InitChat();
	}
	else
	{
		error = MSG_ReadByte();
	}
	return 1;
}

int GTH_SendMessage_RequestQuitNpcChat()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_QUITNPCCHAT );
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return( true );
}

int GTH_ProcessMessage_ReplyQuitNpcChat()
{
	g_ifMng->m_chatWin->QuitChat();
	
	return( true );
}


int GTH_ProcessMessage_NpcMessage()
{
	GTH_UnlockSending();

	int dialogIdx;
	int i, buttonNumber, buttonPos, buttonIdx;

	dialogIdx = MSG_ReadShort();

	
	g_ifMng->m_chatWin->MakeNpcMessage( dialogIdx );
	buttonNumber = MSG_ReadByte();
	
	for( i = 0; i < buttonNumber; i ++ )
	{
		buttonPos = MSG_ReadByte();
		buttonIdx = MSG_ReadShort();
		g_ifMng->m_chatWin->MakeSelectMessage( buttonPos, buttonIdx );
	}
	return( true );
}

int GTH_ProcessMessage_ReplyStore()
{
	GTH_UnlockSending();

	int itemNumber, type;
	int i, idx, page, curNumber, price, itemIdx;
	int pageItemIdx[3] = { 0, 0, 0 };
	
	g_ifMng->m_storeWin->m_PageItemCount[0] = 0;
	g_ifMng->m_storeWin->m_PageItemCount[1] = 0;
	g_ifMng->m_storeWin->m_PageItemCount[2] = 0;

	type = MSG_ReadByte();

	
	
	
	
	
	

	
	switch ( type )
	{
		
		case 0:		
		case 2:	
		{
			g_ifMng->m_storeWin->InitStore();
			
			itemNumber = MSG_ReadShort();
			for( i = 0; i < itemNumber; i ++ )
			{
				page = MSG_ReadByte();
				curNumber = MSG_ReadShort();
				price = MSG_ReadLong();
				idx = MSG_ReadShort();
				
				
				
				itemIdx = page * 36 + g_ifMng->m_storeWin->m_PageItemCount[page];				
				
				g_ifMng->m_storeWin->m_storeItem[itemIdx].curNumber = curNumber;
				g_ifMng->m_storeWin->m_storeItem[itemIdx].price = price;
				g_ifMng->m_storeWin->m_storeItem[itemIdx].idxOnServer = idx;	
				
				
				MSG_ReadItem( &g_ifMng->m_storeWin->m_storeItem[itemIdx].item );
				
				
				
				g_ifMng->m_storeWin->m_PageItemCount[page] ++;
			}
			
			if ( type == 0)
			{
				
				g_ifMng->m_storeWin->m_raCost= MSG_ReadFloat();
				g_ifMng->m_storeWin->m_saCost = MSG_ReadFloat();
				g_ifMng->m_storeWin->m_StoreType = CIFStoreWin.GENERAL_ITME_STORE;
			}
			if ( type == 2)
			{
				
				g_ifMng->m_storeWin->m_LockGambleStore = FALSE;
				g_ifMng->m_storeWin->m_StoreType = CIFStoreWin.GAMBLE_ITEM_STORE;
			}
			
			
			
			g_ifMng->m_chatWin->Enable( false );
			g_ifMng->m_storeWin->Enable();
			g_ifMng->m_itemWin->Enable();
			g_ifMng->SetFocus( g_ifMng->m_storeWin );		
		}
		break;

		
		case 1:
		{
			int genType;
			genType = MSG_ReadShort();
			g_ifMng->m_learnWin->InitLearnWindow( genType );
			g_ifMng->m_chatWin->Enable( false );
			g_ifMng->m_learnWin->Enable();
			g_ifMng->SetFocus( g_ifMng->m_learnWin );			
			g_ifMng->m_storeWin->m_StoreType = CIFStoreWin.LEARN_SKILL_STORE;
		}break;		
	}
	
	

	
	












































	return 1;
}

int GTH_ProcessMessage_ReplyBuyItem()
{
	GTH_UnlockSending();

	int reply, error, inventoryPos;
	int itemIdx;
	int storeItemIdx, curNumber;
	item_t item;	

	reply = MSG_ReadByte();

	if( reply > 0 )
	{
		
		storeItemIdx = MSG_ReadShort();
		curNumber = MSG_ReadShort();
		MSG_ReadItem( &item );

		int stackItemIdx = GTH_StackMyItem( &item );
		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item.durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
			
			g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();
			g_cgv.myCharacterInfo->curItemWeight = MSG_ReadShort();
		}
		else
		{
			itemIdx = GTH_AddMyItem( &item );
			inventoryPos = GTH_FindEmptyInventoryPos();

			g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
			
			g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();
			g_cgv.myCharacterInfo->curItemWeight = MSG_ReadShort();
		}

		g_ifMng->m_storeWin->m_storeItem[storeItemIdx].curNumber = curNumber;

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
		case ERROR_BUYITEM_4 :
			
			break;
		case ERROR_BUYITEM_5:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35), 1, IF_MESSAGE_NONE );
			break;
		}
	}
	return 1;
}

int GTH_ProcessMessage_ReplySellItem()
{
	GTH_UnlockSending();

	int reply, error;
	int itemIdx;

	reply = MSG_ReadByte();

	if( reply > 0 )
	{
		itemIdx = MSG_ReadByte();

		GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_cgv.myCharacterInfo->mouseInventory = -1;

		
		g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();
		g_cgv.myCharacterInfo->curItemWeight = MSG_ReadShort();
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_SELLITEM_1 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,50), 1, IF_MESSAGE_NONE );
			
			break;
		case ERROR_SELLITEM_2 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,51), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_SELLITEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,52), 1, IF_MESSAGE_NONE );
			break;
		}
	}

	return 1;
}

int GTH_SendMessage_RequestNpcSelectMessage( int curIdx )
{


	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_NPCSELECTMESSAGE );
		
		MSG_WriteByte(&netMessage, curIdx);
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
	return( true );
}

int GTH_SendMessage_RequestStore( int idx, int type )
{
	if( !GTH_LockSending() )	return( false );

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_STORE );
		
		MSG_WriteShort(&netMessage, idx);
		MSG_WriteByte(&netMessage, type );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return( true );
}

int GTH_SendMessage_RequestBuyItem( int itemIdx, int num )
{
	if( !GTH_LockSending() )	return( false );

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_BUYITEM );
		
		MSG_WriteShort(&netMessage, itemIdx);		
		MSG_WriteShort(&netMessage, num);	
		
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE); 
	}
	MSG_EndWriting( &netMessage );


	return( true );
}

int GTH_SendMessage_RequestSellItem()
{
	if( !GTH_LockSending() )	return( false );

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_SELLITEM );
		NET_SendMessage(&gsSocket, &netMessage, TRUE); 
		
	}
	MSG_EndWriting( &netMessage );

	return( true );
}

int GTH_SendMessage_RequestBinding( int type, int bindingIdx, int bindingCost )
{
	if( !GTH_LockSending() )	return( false );

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_BINDING );
		
		MSG_WriteByte(&netMessage, type);
		MSG_WriteByte(&netMessage, bindingIdx);
		MSG_WriteLong(&netMessage, bindingCost);
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return( true );
}

int GTH_ProcessMessage_ReplyBinding()
{
	GTH_UnlockSending();

	int reply, error, type;
	int bindingIdx, bindingCost;
	char szTemp[256], szTemp2[128];

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		type = MSG_ReadByte();
		bindingIdx = MSG_ReadByte();
		bindingCost = MSG_ReadLong();

		
		switch( type )
		{
			
		case 0 :
			{				

			    
				
				if(	g_ifMng->m_pgonryunBattlePractice->m_bbattleMode)
				{
					return true;
				}
				
				g_ifMng->m_saveBindingIdx = bindingIdx;
				g_ifMng->m_saveBindingCost = bindingCost;

				
				strcpy( szTemp2, g_ifMng->GetNumberString( bindingCost ) );
				sprintf( szTemp, g_LPACK.GetMassage(0,102), szTemp2 );
				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_BINDING );
			}
			break;
			
		case 1 :
			{
				g_cgv.myCharacterInfo->curChargeSE -= bindingCost;
				
				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,53), 1, IF_MESSAGE_NONE );
			}
			break;
			
		case 2 :
			{
				g_ifMng->m_saveBindingIdx = bindingIdx;
				g_ifMng->m_saveBindingCost = bindingCost;
				
				strcpy( szTemp2, g_ifMng->GetNumberString( bindingCost ) );
				sprintf( szTemp, g_LPACK.GetMassage( 0, 432 ), g_ifMng->m_chatWin->m_saveSelectStr, szTemp2 );
				g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_TRANSFER_WITH_NPC );

				g_ifMng->RestartInterface( 1 );
			}
			break;
			
		case 3 :
			{
				g_cgv.myCharacterInfo->curChargeSE -= bindingCost;
			}
			break;
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case 0 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,54), 1, IF_MESSAGE_NONE );
			break;
		case 1 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,36), 1, IF_MESSAGE_NONE );
			break;
		}
	}
	return( true );
}




enum
{
	EPACK_STORE_ERROR_CHARGE_SE=0,
	EPACK_STORE_ERROR_CHARGE_TYPE,
	EPACK_STORE_ERROR_EQUIP,
	EPACK_STORE_ERROR_CHARGE_COUNT,
	EPACK_STORE_ERROR_DELETE_EPK,
};


int GTH_ProcessMessage_ReplyEpackStore()
{
	GTH_UnlockSending();

	int reply, error;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();
		g_cgv.myCharacterInfo->curChargeRA = MSG_ReadLong();
		g_cgv.myCharacterInfo->curChargeSA = MSG_ReadLong();

		
		
		int EpkRemainCount = MSG_ReadShort();
				
		
		int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
		if( epackIdx >= 0 && epackIdx < MAX_NUMBER_OF_OWNITEM)
		{
			int EpackItemTableIdx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;

			
			if(g_itemTable[EpackItemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_CHARGE)
			{
				gcTools.SetEpkChargeCount(&g_cgv.myCharacterInfo->item[epackIdx],EpkRemainCount);
			}
		}

	}
	else
	{
		error = MSG_ReadByte();

		switch(error) {
		case EPACK_STORE_ERROR_CHARGE_SE :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,36), 1, IF_MESSAGE_NONE );
			break;

		case EPACK_STORE_ERROR_CHARGE_TYPE:
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 409 ), 1, IF_MESSAGE_NONE );
			break;

		case EPACK_STORE_ERROR_EQUIP:
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 410 ), 1, IF_MESSAGE_NONE );
			break;
	
		case EPACK_STORE_ERROR_CHARGE_COUNT:
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 411 ), 1, IF_MESSAGE_NONE );
			break;
		case EPACK_STORE_ERROR_DELETE_EPK:
			{
				g_cgv.myCharacterInfo->equipment[EQUIP_EPACK]=-1;

				int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
				if( epackIdx >= 0 && epackIdx < MAX_NUMBER_OF_OWNITEM)
				{
					GTH_DeleteMyItem(&g_cgv.myCharacterInfo->item[epackIdx]);
				}

				g_ifMng->SetMessage(g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 412 ));
			}
			break;;
		}
	}
	return 1;
}

int GTH_SendMessage_RequestEpackStore( int type, int num )
{
	if( !GTH_LockSending() )	return( false );

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_EPACKSTORE );
		
		MSG_WriteByte(&netMessage, type );
		MSG_WriteShort(&netMessage, num );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
	
	return 1;
}

int GTH_SendMessage_Request_BuySkill( int idx )
{
	if( !GTH_LockSending() ) return false;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, CC_REQUEST_BUY_SKILL );
		MSG_WriteShort(&netMessage, idx );	
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Reply_BuySkill()
{
	GTH_UnlockSending();

	int reply, error;
	int inventoryPos, buySphereCost, skillIdx, skillTableIdx, iLevel;
	char szTemp[256], szTemp2[128];

	reply = MSG_ReadByte();
	if( reply )
	{
		inventoryPos = MSG_ReadByte();
		buySphereCost = MSG_ReadByte();
		skillIdx = MSG_ReadByte();
		skillTableIdx = MSG_ReadShort();
		iLevel = MSG_ReadLong();

		int itemIdx;
		item_t *item;

		itemIdx = g_cgv.myCharacterInfo->inventory[inventoryPos];
		item = &g_cgv.myCharacterInfo->item[itemIdx];

		if( item->durability - buySphereCost < 0 )
		{
			GTH_DeleteMyItem( item );
			g_cgv.myCharacterInfo->inventory[inventoryPos] = -1;
		}
		else
		{
			item->durability -= buySphereCost;
		}
		
		g_cgv.myCharacterInfo->skill[skillIdx].tableIdx = skillTableIdx;
		g_cgv.myCharacterInfo->skill[skillIdx].iLevel = iLevel;
		g_cgv.myCharacterInfo->skillNumber ++;

		
		
		strcpy( szTemp2, g_ifMng->GetNumberString( buySphereCost ) );

		
		
		sprintf( szTemp, g_LPACK.GetMassage(0,416), g_SkillTableDB.GetSkillName(skillTableIdx), szTemp2 );
			
		g_ifMng->AddSysMessage( szTemp );
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_BUYSKILL_1 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,406), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_BUYSKILL_2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,407), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_BUYSKILL_3 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,408), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_BUYSKILL_4 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,409), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_BUYSKILL_5 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,410), 1, IF_MESSAGE_NONE );
			break;
		}
	}
	return 1;
}




int GTH_SendMessage_QuestLogic_InitStatus()
{
	if( !GTH_LockSending() ) return false;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_QUESTLOGIC);
		MSG_WriteShort(&netMessage, QUESTLOGIC_INITSTATUS);
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_QuestLogic()
{
	int command;
	command = MSG_ReadByte();

	switch( command )
	{
	case QUESTLOGIC_CHATMSG :
		GTH_ProcessMessage_QuestLogic_ChatMsg();
		break;
	case QUESTLOGIC_AWAKEN :
		GTH_ProcessMessage_QuestLogic_Awaken();
		break;
	
	case QUESTLOGIC_INITSTATUS :
		GTH_ProcessMessage_QuestLogic_InitStatus();
		break;
	}
	return 1;
}

int GTH_ProcessMessage_QuestLogic_ChatMsg()
{
	int idx;
	int type;

	type= MSG_ReadShort();
	idx = MSG_ReadShort();
	switch(type) {
	case 0:
		g_ifMng->AddSysMessage( g_LPACK.GetMassage( LPACK_TYPE_NPC, idx ), GTHCOLOR_NOTICEMESSAGE );
		break;
	case 1:
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,32), 1, IF_MESSAGE_NONE );
		break;
	
	}
	return 1;
}

int GTH_ProcessMessage_QuestLogic_Awaken()
{
	int pcJob;
	char szTemp[256];

	pcJob = MSG_ReadShort();
	g_cgv.myCharacterInfo->pcJob = pcJob;

	for( int i = 0; i < GEN_NUMBER; i ++ )
		g_cgv.myCharacterInfo->genCapability[i] = MSG_ReadLong();

	sprintf( szTemp, g_LPACK.GetMassage( 0, 578 ), g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, pcJob ) );
	g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );

	return 1;
}


int GTH_ProcessMessage_QuestLogic_InitStatus()
{
	GTH_UnlockSending();

	int reply, error;
	int type, nak;
	char szTemp[256];

	reply = MSG_ReadByte();
	if( reply )
	{
		type = MSG_ReadByte();
		if( !type )
		{
			nak = MSG_ReadLong();
			sprintf( szTemp, g_LPACK.GetMassage( 0, 660 ), nak );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_INITSTATUS );
		}
		else
		{
			
			g_cgv.myCharacterInfo->bonusStatusPoint = MSG_ReadShort();
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,663) );
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
			
		case 1 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,662) );
			break;
			
		case 2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,661) );
			break;
		
		case 3 :
			g_ifMng->SetMessage(g_LPACK.GetMassage(0, 322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 232));
			break;
		
		}
	}
	return 1;
}


int GTH_ProcessMessage_NPCInfo()
{
	int i, npcNumber, worldIdx;

	npcNumber = MSG_ReadByte();
	for( i = 0; i < npcNumber; i ++ )
	{
		MSG_ReadPosition( g_ifMng->m_mapWin->m_npcInfo[i].position );
		g_ifMng->m_mapWin->m_npcInfo[i].iconType = 0;
		g_ifMng->m_mapWin->m_npcInfo[i].tableIdx = MSG_ReadShort();
	}

	worldIdx = MSG_ReadByte();

	for( i = 0; i < g_cgv.worldTable[worldIdx].gateNumber; i ++ )
	{
		VectorCopy( g_ifMng->m_mapWin->m_npcInfo[npcNumber + i].position, g_cgv.worldTable[worldIdx].spawnPos[i] );
		
		g_ifMng->m_mapWin->m_npcInfo[npcNumber + i].iconType = 1;
		g_ifMng->m_mapWin->m_npcInfo[npcNumber + i].linkedWorldIdx = g_cgv.worldTable[worldIdx].linkedWorldIdx[i];
	}

	g_ifMng->m_mapWin->m_numNpcInfo = npcNumber + g_cgv.worldTable[worldIdx].gateNumber;

	return 1;
}


