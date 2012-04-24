









#include "../global_def.h"


void	GTH_ItemExtendSystemDecode(void)
{
	SHORT Packet = static_cast<enum enumItemExtendSystemPacket>(MSG_ReadShort());	
	switch(Packet)
	{		

		
	case REPLY_ITEM_SYNC_TIME:		
		GTH_ProcessMessage_ItemSyncTime();
		break;
		
		
	case REPLY_ITEM_TIME_SET:
		GTH_ProcessMessageReplyItemTimeSet();
		break;		
	}
}








void GTH_ProcessMessage_ItemSyncTime()
{
	int MaxSize = 0;

	MaxSize = MSG_ReadLong();

	for (int nReciveIdx =0; nReciveIdx < MaxSize; nReciveIdx++)
	{
		int ItemIdx = MSG_ReadLong();

		item_t Item;			 
		GTH_InitItem(&Item);
		MSG_ReadItem(&Item);

		if ( ItemIdx >= 0|| ItemIdx < MAX_NUMBER_OF_OWNITEM )
			memcpy(&g_cgv.myCharacterInfo->item[ItemIdx], &Item, sizeof(item_t));
	}

}







void GTH_SendMessageRequestItemSyncItem()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, ITEM_EXTEND_SYSTEM);
		MSG_WriteShort(&netMessage,	REQUEST_ITEM_SYNC_TIME);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}






void	GTH_SendMessageRequestItemTimeSet(const enum sItemUseTimeInfo::enumInventoryType type, int iItemIdx, int iinvenIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, ITEM_EXTEND_SYSTEM);
		MSG_WriteShort(&netMessage,	REQUEST_ITEM_TIME_SET);	
		
		MSG_WriteLong(&netMessage,	(int)type);
		MSG_WriteLong(&netMessage,	iItemIdx);
		MSG_WriteLong(&netMessage,	iinvenIdx);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}








void	GTH_ProcessItemTimeSet(const int nItemIdx)
{	

	enum sItemUseTimeInfo::enumInventoryType type;
	int nInvenIdx = -1;
	type = GetItemInvenType(nItemIdx, nInvenIdx);

	
	if ( type != sItemUseTimeInfo::enumInventoryType::ITEM_UNKNOW)
		GTH_SendMessageRequestItemTimeSet(type, nItemIdx, nInvenIdx);	
}







enum sItemUseTimeInfo::enumInventoryType	GetItemInvenType(const int nItemIdx, int &iInvenToryPos)
{

	int nInvenIdx = 0;

	
	for ( nInvenIdx = 0; nInvenIdx < MAX_INVENTORY_SIZE ; nInvenIdx++)		
	{
		if (g_cgv.myCharacterInfo->inventory[nInvenIdx] == nItemIdx )
		{
			iInvenToryPos = nInvenIdx;
			return sItemUseTimeInfo::enumInventoryType::ITEM_INVEN;
		}
	}

	
	for (nInvenIdx = 0; nInvenIdx < MAX_EQUIPMENT ; nInvenIdx++)		
	{		

		if (g_cgv.myCharacterInfo->equipment[nInvenIdx] == nItemIdx )
		{
			iInvenToryPos = nInvenIdx;
			return sItemUseTimeInfo::enumInventoryType::ITEM_EQUIP;
		}
	}

	
	for ( nInvenIdx = 0; nInvenIdx < MAX_UPGRADE_SOURCE_ITEM_SIZE ; nInvenIdx++)		
	{	
		if ( g_cgv.myCharacterInfo->upgradeSourceInventory[nInvenIdx] == nItemIdx )
		{
			iInvenToryPos = nInvenIdx;
			return sItemUseTimeInfo::enumInventoryType::ITEM_UPGRADE;
		}
	}

	
	for ( nInvenIdx = 0; nInvenIdx < MAX_TRADE_SIZE; nInvenIdx ++)
	{
		if ( g_GonryunBattlePractic.m_imyTradetemp[nInvenIdx] == nItemIdx )
		{
			iInvenToryPos = nInvenIdx;
			return sItemUseTimeInfo::enumInventoryType::ITEM_GB_BATTING;
		}			
	}

	
	for ( nInvenIdx = 0; nInvenIdx < MAX_PRECOCITY_SIZE; nInvenIdx ++)
	{
		if ( g_cgv.myCharacterInfo->precocityInventory[nInvenIdx] == nItemIdx )
		{
			iInvenToryPos = nInvenIdx;
			return sItemUseTimeInfo::enumInventoryType::ITEM_PRECOCITY;
		}			
	}

	
	for ( nInvenIdx = 0; nInvenIdx < MAX_CRAFT_SOURCE_ITEM_SIZE; nInvenIdx ++)
	{
		if ( g_cgv.myCharacterInfo->craftSourceInventory[nInvenIdx] == nItemIdx )
		{
			iInvenToryPos = nInvenIdx;
			return sItemUseTimeInfo::enumInventoryType::ITEM_CRAFT;
		}			
	}

	
	for ( nInvenIdx = 0; nInvenIdx < MAX_DEPOT_SIZE; nInvenIdx ++)
	{
		if ( g_cgv.myCharacterInfo->depot[nInvenIdx] == nItemIdx )
		{
			iInvenToryPos = nInvenIdx;
			return sItemUseTimeInfo::enumInventoryType::ITEM_DEPOT;
		}			
	}
	
	
	for ( nInvenIdx = 0; nInvenIdx < MAX_BOOTH_INVENTORY_SIZE; nInvenIdx ++)
	{
		if ( g_cgv.myCharacterInfo->boothInventory[nInvenIdx].itemIdx == nItemIdx )
		{
			iInvenToryPos = nInvenIdx;
			return sItemUseTimeInfo::enumInventoryType::ITEM_BOOTHINVENTORY;
		}			
	}

	
	if ( g_cgv.myCharacterInfo->mouseInventory == nItemIdx)
		return sItemUseTimeInfo::enumInventoryType::ITEM_MOUSEIN;
	

	return sItemUseTimeInfo::enumInventoryType::ITEM_UNKNOW;
};








void GTH_ProcessMessageReplyItemTimeSet()
{	

	int Reply = MSG_ReadLong();

	if ( Reply == 1)
	{	
		char msg[256];
		item_t item;

		MSG_ReadItem(&item);
		sItemUseTimeInfo::enumInventoryType type;
		type = static_cast<enum sItemUseTimeInfo::enumInventoryType>(MSG_ReadLong());		
		
		int nItemIdx		= MSG_ReadLong();
		int nInvenIdx		= MSG_ReadLong();
		BOOL bReplaceItem	= MSG_ReadLong();


		
		if (nItemIdx < 0 || nItemIdx >= MAX_NUMBER_OF_OWNITEM )			
			return;

		
		_snprintf(msg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 547), g_cgv.myCharacterInfo->item[nItemIdx].name);
		msg[255] = NULL;
		ItemDestroyNotice(msg);
		
		GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[nItemIdx] );

		
		if ( type == sItemUseTimeInfo::enumInventoryType::ITEM_EQUIP)						
		{				
			
			GTH_ReplaceItem(type, nInvenIdx, -1);	

			
			int inventoryPos = GTH_FindEmptyInventoryPos();

			if( inventoryPos >= 0 ) 
			{
				
				int RegItemIdx = GTH_AddMyItem(&item);
				g_cgv.myCharacterInfo->inventory[inventoryPos] = RegItemIdx;
			}
		}		
		
		else
		{
			int RegItemIdx = GTH_AddMyItem(&item);
			GTH_ReplaceItem(type, nInvenIdx, RegItemIdx);
		}

		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_EQUIP_ITEM ), GTH_SOUND_PLAY_ONCE );
	}
	else
	{
		
		int ErrorIdx = MSG_ReadLong();
		switch(ErrorIdx)
		{
			
		case sItemUseTimeInfo::enumItemExptendError::ITEM_EXTEND_ERROR1:
			{
				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,48), 1, IF_MESSAGE_NONE );
			}
			break;

			
		case sItemUseTimeInfo::enumItemExptendError::ITEM_EXTEND_ERROR2:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,548), 1, IF_MESSAGE_NONE );
			break;

			
		case sItemUseTimeInfo::enumItemExptendError::ITEM_EXTEND_ERROR3:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,549), 1, IF_MESSAGE_NONE );
			break;










		}
	}
	
}






BOOL	GTH_ReplaceItem(const enum sItemUseTimeInfo::enumInventoryType type, int iInvenPos, int NewItemIdx)
{
	switch (type)
	{		
		
	case	sItemUseTimeInfo::enumInventoryType::ITEM_INVEN:
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_INVENTORY_SIZE)
				return FALSE;
			g_cgv.myCharacterInfo->inventory[iInvenPos] = NewItemIdx;					
			
		}
		break;
		
	case	sItemUseTimeInfo::enumInventoryType::ITEM_UPGRADE:
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_UPGRADE_SOURCE_ITEM_SIZE)
				return FALSE;
			g_cgv.myCharacterInfo->upgradeSourceInventory[iInvenPos] = NewItemIdx;
		}
		break;
		
	case	sItemUseTimeInfo::enumInventoryType::ITEM_EQUIP:	
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_EQUIPMENT)
				return FALSE;
			g_cgv.myCharacterInfo->equipment[iInvenPos] = NewItemIdx;
		}
		break;
		
	case	sItemUseTimeInfo::enumInventoryType::ITEM_GB_BATTING:
		{
			if ( iInvenPos < 0 || iInvenPos >= 	MAX_TRADE_SIZE)
				return FALSE;
			
			g_GonryunBattlePractic.m_imyTradetemp[iInvenPos] 
				= NewItemIdx;
		}
		break;
		
	case	sItemUseTimeInfo::enumInventoryType::ITEM_PRECOCITY:	
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_PRECOCITY_SIZE)
				return FALSE;
			g_cgv.myCharacterInfo->precocityInventory[iInvenPos] = NewItemIdx;
		}
		break;
		
	case	sItemUseTimeInfo::enumInventoryType::ITEM_CRAFT:		
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_CRAFT_SOURCE_ITEM_SIZE)
				return FALSE;
			g_cgv.myCharacterInfo->craftSourceInventory[iInvenPos] = NewItemIdx;
		}
		break;
		
	case	sItemUseTimeInfo::enumInventoryType::ITEM_DEPOT:
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_DEPOT_SIZE)
				return FALSE;
			g_cgv.myCharacterInfo->depot[iInvenPos] = NewItemIdx;
		}
		break;
		
	case	sItemUseTimeInfo::enumInventoryType::ITEM_BOOTHINVENTORY:
		{
			if ( iInvenPos < 0 || iInvenPos >= MAX_BOOTH_INVENTORY_SIZE)
				return FALSE;
			g_cgv.myCharacterInfo->boothInventory[iInvenPos].itemIdx = NewItemIdx;
		}
		break;
		
		
	case sItemUseTimeInfo::enumInventoryType::ITEM_MOUSEIN:
		{			
			g_cgv.myCharacterInfo->mouseInventoryType = NewItemIdx;
		}
		break;
	}
	
	return TRUE;
}







void GTH_ItemUseTimeProcess()
{		
	
	if ( iSTickPerSec() == FALSE)
		return;

	char msg[256];

	for( int iItemIdx = 0; iItemIdx < MAX_NUMBER_OF_OWNITEM; iItemIdx ++ )
	{		
		item_t *pItem	= &g_cgv.myCharacterInfo->item[iItemIdx];
		int *pitemTime	=  NULL;

		
		if ( IsItemUseFlag(pItem, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_TIME) )
			pitemTime = &pItem->ItemExtendInfo.AllowUseTime;		

		
		if ( IsItemUseFlag(pItem, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_DATE ) )
			pitemTime = &pItem->ItemExtendInfo.CalclateUseDateToSec;	

		
		if ( NULL == pitemTime )
			continue;		

		
		if ( (*pitemTime) < 0 )
			continue;
		
		
		(*pitemTime) --;					
		
		
		
		if ( (*pitemTime) == sItemUseTimeInfo::enumItemSyncInfo::ITEM_DESTROY_NOTICE_5MIN)
		{			
			
			_snprintf(msg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 550),  			
				pItem->name, 5, 5, pItem->name );		

			ItemDestroyNotice(msg);					
		}		
		
		if ( (*pitemTime) == sItemUseTimeInfo::enumItemSyncInfo::ITEM_DESTROY_NOTICE_1MIN )
		{			



			_snprintf(msg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 550),  			
				pItem->name, 1, 1, pItem->name );		

			ItemDestroyNotice(msg);					
		}			
		
		
		if ( (*pitemTime) < 0 )
			
			GTH_ProcessItemTimeSet(iItemIdx);
	}	

}








BOOL iSTickPerSec()
{
	static float fLastAppTime = g_timer.GetAppTime();
	if ( g_timer.GetAppTime() - fLastAppTime >= 1.0F)
	{		
		fLastAppTime = g_timer.GetAppTime();
		return TRUE;
	}
	return FALSE;	
}








void	ItemDestroyNotice(char* szmsg)
{
	if ( NULL == szmsg)
		return;

	g_ifMng->m_GmNotice = FALSE;
	g_ifMng->AddSysMessage( szmsg, GTHCOLOR_ITEMNOTICE_MESSAGE );
	g_ifMng->AddNoticeMessage( szmsg, GTHCOLOR_ITEMNOTICE_MESSAGE );					

	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_WARNING_2 ), GTH_SOUND_PLAY_ONCE );					

}