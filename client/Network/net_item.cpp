#include "../global_def.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 


#include "../CTools.h"
extern CTools gcTools;



void GTH_SendMessage_DropItem(int itemIdx, vec3_t itemPos)
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_DROPITEM);
		MSG_WriteByte(&netMessage, itemIdx);
		MSG_WritePosition(&netMessage, itemPos);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_PickUpItem(int itemObjectIdx)
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_PICKUPITEM);
		
		
		MSG_WriteShort(&netMessage, itemObjectIdx);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_RequestGetInventoryItem(int inventoryPos, int directEquip, int numStack )
{
	
	
	if( !g_pApp->m_myCharacter->isAlive ) return;

	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_GETINVENTORYITEM);
		
		MSG_WriteByte(&netMessage, directEquip);
		MSG_WriteByte(&netMessage, inventoryPos);
		MSG_WriteShort(&netMessage, numStack);
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );	
}

void GTH_SendMessage_RequestGetEquipItem(int equipPos, int directEquip )
{
	
	
	if( !g_pApp->m_myCharacter->isAlive ) return;

	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_GETEQUIPITEM);
		
		MSG_WriteByte(&netMessage, directEquip);
		MSG_WriteByte(&netMessage, equipPos);
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
}

int GTH_ProcessMessage_SyncItemObject()
{
	syncItemList_t syncItem[100];

	int i;
	int objectNumber;
	int objectType;
	int classType, mdlID;

	objectNumber = MSG_ReadByte();

	for (i=0; i < objectNumber; i++)
	{	
		syncItem[i].idxOnServer = MSG_ReadShort();
		MSG_ReadPosition( syncItem[i].position );
		syncItem[i].position[2] = g_move->GetHeight(syncItem[i].position) + ITEMOBJECT_DISPLAY_HEIGHT;

		objectType = MSG_ReadByte();

		
		
		if( objectType == OBJECT_TYPE_ITEM || objectType == OBJECT_TYPE_PORTAL )
		{
			syncItem[i].itemTableIdx = MSG_ReadShort();
			
			GTH_SetItemObjectModel( objectType, syncItem[i].itemTableIdx, classType, mdlID );
			syncItem[i].classType = (GTH_ITEM_CLASS_TYPE)classType;
			syncItem[i].itemID = mdlID;
		}
		else if( objectType == OBJECT_TYPE_NAK )
		{
			syncItem[i].classType =	GTH_ITEM_CLASS_ACCESSORY;
			syncItem[i].itemID = 6;
		}
		else 
		{
			return false;
		}
	}

	
	g_itemMng->SyncItemObject( &syncItem[0], objectNumber );

	return 1;
}

int GTH_ProcessMessage_AddMultiItemObject()
{
	int i, idxOnServer, itemTableIdx, number;
	vec3_t position, origin, itempos;
	int objectType;
	int classType, mdlID;

	MSG_ReadPosition( origin );
	number = MSG_ReadByte();

	for( i = 0; i < number; i ++ )
	{
		idxOnServer = MSG_ReadShort();
		MSG_ReadPosition( position );
		position[2] = g_move->GetHeight(position) + ITEMOBJECT_DISPLAY_HEIGHT;
		origin[2] = g_move->GetHeight(origin);

		objectType = MSG_ReadByte();
		itemTableIdx = MSG_ReadShort();

		
		GTH_SetItemObjectModel( objectType, itemTableIdx, classType, mdlID );
		
		if ( g_itemMng->FindEntity(idxOnServer) >= 0 )	
			continue;

		g_itemMng->Add( (GTH_ITEM_CLASS_TYPE) classType , mdlID , position, idxOnServer, itemTableIdx );
		
		
		if( objectType == OBJECT_TYPE_ITEM )
		{
		}

		VectorCopy( itempos, position );
		itempos[2] = origin[2];
		g_effectMng->Add( GTH_EFFECT_CLASS_MODEL, 10, origin, itempos );
	}
	return( true );
}

int GTH_ProcessMessage_AddItemObject()
{
	int idxOnServer, objectType, itemTableIdx;
	vec3_t position;
	int classType;
	int mdlID;

	idxOnServer = MSG_ReadShort();
	MSG_ReadPosition( position );
	position[2] = g_move->GetHeight(position) + ITEMOBJECT_DISPLAY_HEIGHT;
	
	objectType = MSG_ReadByte();
	
	
	
	if( objectType == OBJECT_TYPE_ITEM || objectType == OBJECT_TYPE_PORTAL )
	{
		itemTableIdx = MSG_ReadShort();
		
		GTH_SetItemObjectModel( objectType, itemTableIdx, classType, mdlID );
	}
	else if( objectType == OBJECT_TYPE_NAK )
	{
		classType	=	GTH_ITEM_CLASS_ACCESSORY;
		mdlID		=	6;
	}
	else 
	{
		return false;
	}

	if ( g_itemMng->FindEntity(idxOnServer) >= 0 )	
		return false;

	g_itemMng->Add( (GTH_ITEM_CLASS_TYPE) classType , mdlID , position, idxOnServer, itemTableIdx );

	
	
	
	if( objectType == OBJECT_TYPE_ITEM  )
	{
		g_musicMng->PlaySample3D( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_DROP_ITEM ), GTH_SOUND_PLAY_ONCE, position );
	}


	return true;
}

int GTH_ProcessMessage_DeleteItemObject()
{
	int idxOnServer, itemEntityIdx;

	idxOnServer = MSG_ReadShort();
	
	if( g_cgv.targetItemIdxOnServer == idxOnServer ) g_cgv.targetItemIdxOnServer = -1;

	itemEntityIdx = g_itemMng->FindEntity(idxOnServer);
	if ( itemEntityIdx < 0 )	
		return false;

	g_itemMng->Delete( itemEntityIdx );
	return true;
}

int GTH_ProcessMessage_ReplyGetInventoryItem()
{
	GTH_UnlockSending();

	int reply, error;
	int invenPos, itemIdx, equipPos, stackItemIdx, emptyInvenPos;
	int mouseInventory;
	item_t* item;
	item_t stackItem1, stackItem2;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		invenPos = MSG_ReadByte();
		equipPos = MSG_ReadByte();
		itemIdx = g_cgv.myCharacterInfo->inventory[invenPos];
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
		g_cgv.myCharacterInfo->inventory[invenPos] = -1;
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		break;
		
	case 2:
		g_cgv.myCharacterInfo->inventory[invenPos] = g_cgv.myCharacterInfo->mouseInventory;
		g_cgv.myCharacterInfo->mouseInventory = -1;
		break;

	case 3:
		g_cgv.myCharacterInfo->inventory[invenPos] = g_cgv.myCharacterInfo->mouseInventory;
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		break;
		
	case 4 :
		g_cgv.myCharacterInfo->inventory[invenPos] = g_cgv.myCharacterInfo->equipment[equipPos];
		g_cgv.myCharacterInfo->equipment[equipPos] = itemIdx;
		g_charManager->GenerateMyCharacter();
		break;
	case 5 :
		GTH_AddMyItem( &stackItem2 );
		g_cgv.myCharacterInfo->item[itemIdx].durability = stackItem1.durability;
		g_cgv.myCharacterInfo->mouseInventory = mouseInventory;
		break;
		
	case 6 :
		item = &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->mouseInventory];
		stackItemIdx = GTH_StackMyItem( item );
		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item->durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
			GTH_DeleteMyItem( item );
		}
		else
		{
			emptyInvenPos = GTH_FindEmptyInventoryPos();
			g_cgv.myCharacterInfo->inventory[emptyInvenPos] = g_cgv.myCharacterInfo->mouseInventory;
		}
		g_cgv.myCharacterInfo->mouseInventory = -1;
		break;
	case 7 :
		{
			
		}
		break;
		
		
	case 8 :
		
		itemIdx = g_cgv.myCharacterInfo->inventory[invenPos];
		if( itemIdx >= 0 )
		{
			if( g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx >= 0 )
			{
				GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
			}
		}
		g_cgv.myCharacterInfo->inventory[invenPos] = -1;
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,25) , 1, IF_MESSAGE_NONE);
		break;

		
		
	case 9:
		{
			g_cgv.myCharacterInfo->inventory[invenPos] = g_cgv.myCharacterInfo->equipment[equipPos];
			g_cgv.myCharacterInfo->equipment[equipPos] = itemIdx;

			itemIdx = g_cgv.myCharacterInfo->inventory[invenPos];
			if( itemIdx >= 0 )
			{
				if( g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx >= 0 )
				{
					GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
				}
			}
			g_cgv.myCharacterInfo->inventory[invenPos] = -1;
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,502));
			break;

		}
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

		
		if( g_cgv.myCharacterInfo->mouseInventory >= 0 ) g_cgv.myCharacterInfo->mouseInventoryType = MOUSEINVEN_TYPE_INVENTORY;
		else g_cgv.myCharacterInfo->mouseInventoryType = MOUSEINVEN_TYPE_NONE;
	}
	else
	{
		switch( error )
		{
		case ERROR_EQUIPITEM_1 :
			
			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,326) );
			break;
		case ERROR_EQUIPITEM_2 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,327) );
			break;
		case ERROR_EQUIPITEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,23) );
			break;

		case ERROR_EQUIPITEM_4 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,395) );
			break;
		case ERROR_EQUIPITEM_5 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,396) );
			break;
		case ERROR_EQUIPITEM_6 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,397) );
			break;

			
		case ERROR_EQUIPITEM_7:
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,250) );
			break;
		case ERROR_EQUIPITEM_8:
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,253) );
			break;
			
		}
	}

	return 1;
}

int GTH_ProcessMessage_ReplyGetEquipItem()
{
	GTH_UnlockSending();
	
	int reply, error;
	int invenPos, equipPos, itemIdx, temp;
	
	
	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		invenPos = MSG_ReadByte();
		equipPos = MSG_ReadByte();
		itemIdx = g_cgv.myCharacterInfo->equipment[equipPos];
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
		g_cgv.myCharacterInfo->mouseInventory = itemIdx;
		g_cgv.myCharacterInfo->equipment[equipPos] = -1;
		break;
		
	case 2:
		g_cgv.myCharacterInfo->equipment[equipPos] = g_cgv.myCharacterInfo->mouseInventory;
		g_cgv.myCharacterInfo->mouseInventory = -1;
		break;

	case 3 :
		temp = g_cgv.myCharacterInfo->equipment[equipPos];
		g_cgv.myCharacterInfo->equipment[equipPos] = g_cgv.myCharacterInfo->mouseInventory;
		g_cgv.myCharacterInfo->mouseInventory = temp;
		break;

	case 4 :
		g_cgv.myCharacterInfo->inventory[invenPos] = itemIdx;
		g_cgv.myCharacterInfo->equipment[equipPos] = -1;
		break;
	}

	if( reply > 0 )
	{
		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_EQUIP_ITEM ), GTH_SOUND_PLAY_ONCE );

		
		if( g_cgv.myCharacterInfo->mouseInventory >= 0 ) g_cgv.myCharacterInfo->mouseInventoryType = MOUSEINVEN_TYPE_EQUIP;
		else g_cgv.myCharacterInfo->mouseInventoryType = MOUSEINVEN_TYPE_NONE;
	}
	else
	{
		switch( error )
		{
		case ERROR_EQUIPITEM_1 :		
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,326) );
			break;
		case ERROR_EQUIPITEM_2 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,327) );
			break;
		case ERROR_EQUIPITEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,23) );
			break;
		case ERROR_EQUIPITEM_4 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,395) );
			break;
		case ERROR_EQUIPITEM_5 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,396) );
			break;
		case ERROR_EQUIPITEM_6 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,397) );
			break;
			
		case ERROR_EQUIPITEM_7:
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,250) );
			break;
		case ERROR_EQUIPITEM_8:
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(9,253) );
			break;
			

		}
	}

	g_charManager->GenerateMyCharacter();
	return 1;
}

int GTH_ProcessMessage_ReplyDropItem()
{
	GTH_UnlockSending();

	int reply, error;
	int itemIdx;

	reply = MSG_ReadByte();

	if (reply > 0)
	{
		itemIdx = MSG_ReadByte();
		g_cgv.myCharacterInfo->curItemWeight = MSG_ReadShort();

		GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_cgv.myCharacterInfo->mouseInventory = -1;
	}
	else
	{
		error = MSG_ReadByte();
		switch ( error )
		{
		case ERROR_DROPITEM_1:

			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,24), 1, IF_MESSAGE_NONE );			
			break;
		case ERROR_DROPITEM_2:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,25) , 1, IF_MESSAGE_NONE);
			break;
		case ERROR_DROPITEM_3:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,26), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_DROPITEM_4:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,27), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_DROPITEM_5 :
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,359) );
			break;
		default:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,28), 1, IF_MESSAGE_NONE );			
			break;
		}
	}

	

	return 1;
}
	
int GTH_ProcessMessage_ReplyPickUpItem()
{
	GTH_UnlockSending();

	int reply, error;
	int itemIdx, inventoryPos;
	item_t item;

	
	char temp[256];

	reply = MSG_ReadByte();

	if ( reply > 0 )
	{
		MSG_ReadItem( &item );
		g_cgv.myCharacterInfo->curItemWeight = MSG_ReadShort();

		int stackItemIdx = GTH_StackMyItem( &item );
		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item.durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos();
			itemIdx = GTH_AddMyItem( &item );
			g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
		}


		sprintf( temp, g_LPACK.GetMassage(0,159), item.name );
		g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );

		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PICKUP_ITEM ), GTH_SOUND_PLAY_ONCE );
	}
	else
	{
		error = MSG_ReadByte();
		switch ( error )
		{
		case ERROR_PICKUPITEM_1:
			
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,29) );
			break;
		case ERROR_PICKUPITEM_2:

			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,30) );
			break;
		case ERROR_PICKUPITEM_3:
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,31) );
 
			break;
		case ERROR_PICKUPITEM_4: 
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,32) );
			break;
		case ERROR_PICKUPITEM_5:
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,33) );
			break;
		case ERROR_PICKUPITEM_6:
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,34) );
			break;
		case ERROR_PICKUPITEM_7:
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,35) );
			break;
		case ERROR_PICKUPITEM_8:
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,451) );
			break;
		default:
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,28) );
			break;
		}
	}
	

	return 1;
}

int GTH_ProcessMessage_LootValue()
{
	__int64 exp, genExp, exp2, genExp2;
	int lootNak, genCapability, gen, sharingType;
	int lootNak2, genCapability2;
	char temp[256];
	char szTemp2[64], szTemp3[128];


	exp = MSG_Read64Int();
	genExp = MSG_Read64Int();
	lootNak = MSG_ReadLong();
	genCapability = MSG_ReadLong();

	sharingType = MSG_ReadByte();	
	
	gen = g_cgv.myCharacterInfo->selectedGen;
		
	exp2 = exp - g_cgv.myCharacterInfo->exp;
	genExp2 = genExp - g_cgv.myCharacterInfo->genExp;
	if( exp2 < 0 ) exp2 = 0;
	if( genExp2 < 0 ) genExp2 = 0;
		
	if( exp2 > 0 || genExp2 > 0 )
	{
		switch( sharingType )
		{
		case 0 : case 1 :
			
			
			strcpy( szTemp2, g_ifMng->GetNumberString( exp2 ) );
			strcpy( szTemp3, g_ifMng->GetNumberString( genExp2 ) );
			sprintf( temp, g_LPACK.GetMassage(0,183), szTemp2, szTemp3 );
			g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
			break;
		case 2 :
			
			
			strcpy( szTemp2, g_ifMng->GetNumberString( exp2 ) );
			strcpy( szTemp3, g_ifMng->GetNumberString( genExp2 ) );
			sprintf( temp, g_LPACK.GetMassage(0,402), szTemp2, szTemp3 );
			g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
			break;
		case 3 :
			
			
			strcpy( szTemp2, g_ifMng->GetNumberString( exp2 ) );
			strcpy( szTemp3, g_ifMng->GetNumberString( genExp2 ) );
			sprintf( temp, g_LPACK.GetMassage(0,403), szTemp2, szTemp3 );
			g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
			break;
			
		case 4:
			strcpy( szTemp2, g_ifMng->GetNumberString( exp2 ) );
			strcpy( szTemp3, g_ifMng->GetNumberString( genExp2 ) );
			sprintf( temp, g_LPACK.GetMassage(0,183), szTemp2, szTemp3 );
			g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );
			break;
		case 10 :
			
			break;
		}
	}	
	g_cgv.myCharacterInfo->exp = exp;
	g_cgv.myCharacterInfo->genExp = genExp;
	
	genCapability2 = genCapability - g_cgv.myCharacterInfo->genCapability[gen];
	if( genCapability2 > 0 && sharingType != 10 )
	{			
		strcpy( szTemp2, g_ifMng->GetNumberString( genCapability2 ) );
		sprintf( temp, g_LPACK.GetMassage(0,412), 
			g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GEN_ATTRIBUTE, gen ), 
			szTemp2 );
		g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );		
	}
	g_cgv.myCharacterInfo->genCapability[g_cgv.myCharacterInfo->selectedGen] = genCapability;

	lootNak2 = lootNak - g_cgv.myCharacterInfo->curChargeSE;
	if( lootNak2 > 0  && sharingType != 10 )
	{
		
		strcpy( szTemp2, g_ifMng->GetNumberString( lootNak2 ) );
		sprintf( temp, g_LPACK.GetMassage(0,184), szTemp2 );
		g_ifMng->AddSysMessage( temp, GTHCOLOR_LOOTMESSAGE );

		
		
		
		
		GTH_UnlockSending();
	}
	g_cgv.myCharacterInfo->curChargeSE = lootNak;

	return 1;
}

int GTH_SendMessage_RequestRepairItem( int type, int pos )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_REPAIR );
		MSG_WriteByte(&netMessage, type );
		MSG_WriteShort(&netMessage, pos );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}
int GTH_ProcessMessage_ReplyRepairItem()
{
	int reply, error, i, num;
	int type, pos, dur;
	int curSE;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		curSE = MSG_ReadLong();
		type = MSG_ReadByte();
		g_cgv.myCharacterInfo->curChargeSE = curSE;
		switch( type )
		{
		case 0 : case 1 :
			pos = MSG_ReadShort();
			dur = MSG_ReadShort();
			g_cgv.myCharacterInfo->item[pos].durability = dur;
			break;
		case 2 :
			num = MSG_ReadByte();
			for( i = 0; i < num; i ++ )
			{
				pos = MSG_ReadShort();
				dur = MSG_ReadShort();
				g_cgv.myCharacterInfo->item[pos].durability = dur;
			}
			break;
		}
		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_REPAIR ), GTH_SOUND_PLAY_ONCE );
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_REPAIRITEM_NONE :
			break;
		case ERROR_REPAIRITEM_1 :
			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,36), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_REPAIRITEM_2 :			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,37), 1, IF_MESSAGE_NONE );

			
		case ERROR_REPAIRITEM_3 :					
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322),  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,314), 1, IF_MESSAGE_NONE );
			break;
		}
	}

	return 1;
}


int GTH_SendMessage_RequestDestroyItem( int invenPos )
{
	
	
	if( !g_pApp->m_myCharacter->isAlive ) return 0;
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_DESTROY_ITEM );
		
		
		
		
		MSG_WriteShort(&netMessage, invenPos);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_ReplyDestroyItem()
{
	GTH_UnlockSending();
	int reply, error;

	int curRA, curSA, itemIdx;

	reply = MSG_ReadByte();

	if( reply > 0 )
	{
		curRA = MSG_ReadShort();
		curSA = MSG_ReadShort();
		itemIdx = MSG_ReadShort();
		g_cgv.myCharacterInfo->curItemWeight = MSG_ReadShort();
		
		
		int invenPos = MSG_ReadShort();

		if ( invenPos != -1 )
			g_cgv.myCharacterInfo->inventory[invenPos] = -1;
	
		g_cgv.myCharacterInfo->curChargeRA = curRA;
		g_cgv.myCharacterInfo->curChargeSA = curSA;
		
		GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_cgv.myCharacterInfo->mouseInventory = -1;

		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_LOGIN_ITEMABSORB ), GTH_SOUND_PLAY_ONCE );
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_DESTROYITEM_NONE :
			break;
		case ERROR_DESTROYITEM_1 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,24), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_DESTROYITEM_2 :			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,25), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_DESTROYITEM_3 :
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,452) );
			break;

		
#pragma chMSG("이 부분은 언어팩에 들어가야 하나???")

		case ERROR_DESTROYITEM_4 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,895) );
			break;

			
		case ERROR_DESTROYITEM_5:
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,897) );
			break;

			
		case ERROR_DESTROYITEM_6 :
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,736) ); 
			break;
		case ERROR_DESTROYITEM_7 :
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,737) ); 
			break;
		case ERROR_DESTROYITEM_8 :
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,757) ); 
			break;
		case ERROR_DESTROYITEM_9 :
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,754) ); 
			break;
		case ERROR_DESTROYITEM_10 :
			
			
			g_ifMng->AddSysMessage( g_LPACK.GetMessage(9,18) );
			break;

		   
		   
		case ERROR_DESTROYITEM_11:
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,238) );
			break;
			
		}
	}
	
	return 1;
}

int GTH_ProcessMessage_AddItemInv()								
{
	int reply, error;

	item_t item;
	int inventoryPos;
	int itemIdx;
	char tempMessage[256];

	reply = MSG_ReadByte();

	if( reply )
	{
		MSG_ReadItem( &item );
		
		int messageFlag = MSG_ReadByte();

		int stackItemIdx = GTH_StackMyItem( &item );
		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += item.durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos();
			itemIdx = GTH_AddMyItem( &item );
			g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
		}

		
		
		
		
		
		if ( 1 == messageFlag )
		{
			
			sprintf(tempMessage,g_LPACK.GetMassage(0,162),item.name);	
			g_ifMng->AddSysMessage(tempMessage,D3DCOLOR_ARGB( 255, 100,255,255 ) );
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch ( error )
		{
		case ERROR_PICKUPITEM_4: 
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,32), 1, IF_MESSAGE_NONE );
			break;
		case ERROR_PICKUPITEM_5:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,33), 1, IF_MESSAGE_NONE );
			break;
		default:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,28), 1, IF_MESSAGE_NONE );
			break;
		}
	}

	return TRUE;
}

int GTH_ProcessMessage_DeleteItemInv()							
{	
	int reply, error;
	int itemTableIdx, itemNumber, itemIdx;
	int invenPos = -1, equipPos = -1;
	item_t *item;

	char tempMessage[256];

	reply = MSG_ReadByte();

	if( reply )
	{
		itemTableIdx = MSG_ReadShort();
		itemNumber = MSG_ReadShort();

		
		for( int i = 0; i < MAX_INVENTORY_SIZE; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->inventory[i];
			if( itemIdx < 0 ) continue;
			
			if( g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx == itemTableIdx ) break;
		}
		if( i == MAX_INVENTORY_SIZE )
		{
			
			for( i = 0; i < MAX_EQUIPMENT; i ++ )
			{
				itemIdx = g_cgv.myCharacterInfo->equipment[i];
				if( itemIdx < 0 ) continue;
				if( g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx == itemTableIdx ) break;
			}
			if( i == MAX_EQUIPMENT ) 
			{	
				return 0;
			}
			
			equipPos = i;
		}
		else
		{
			invenPos = i;
		}
		item = &g_cgv.myCharacterInfo->item[itemIdx];
		
		if( GTH_IsStackItem( item ) )
		{
			if( item->durability + 1 < itemNumber )
			{
				return 0;
			}
			
			else if( item->durability + 1 == itemNumber )
			{
				if( invenPos >= 0 ) g_cgv.myCharacterInfo->inventory[invenPos] = -1;
				else if( equipPos >= 0 ) g_cgv.myCharacterInfo->equipment[equipPos] = -1;
				
				GTH_DeleteMyItem( item );
			}
			else
			{
				item->durability -= itemNumber;
			}
		}
		else
		{
			if( invenPos >= 0 ) g_cgv.myCharacterInfo->inventory[invenPos] = -1;
			else if( equipPos >= 0 ) g_cgv.myCharacterInfo->equipment[equipPos] = -1;
			
			GTH_DeleteMyItem( item );
		}

		
		sprintf(tempMessage,g_LPACK.GetMassage(0,164),g_cgv.myCharacterInfo->item[itemIdx].name);
		g_ifMng->AddSysMessage(tempMessage,D3DCOLOR_ARGB( 255, 100, 255, 255 ) );
	}
	else
	{
		error = MSG_ReadByte();
		return 0;
	}
	return TRUE;
}


int GTH_ProcessMessage_Reply_MasicBook_RepairItem()
{
	int reply, error, i, num;
	int  pos, dur;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		num = MSG_ReadByte();
		for( i = 0; i < num; i ++ )
		{
			pos = MSG_ReadShort();
			dur = MSG_ReadShort();
			g_cgv.myCharacterInfo->item[pos].durability = dur;
		}

		int InvenPos = MSG_ReadShort();
		GTH_UseItem_Delete(InvenPos);
		
		
		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_REPAIR ), GTH_SOUND_PLAY_ONCE );

		
		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
									74, 
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
		case ERROR_REPAIRITEM_NONE :
			break;
		}
	}

	return 1;
}



bool	GTH_UseItem_Delete(int invenPos)
{
	int itemIdx = g_cgv.myCharacterInfo->inventory[invenPos];
	if(itemIdx < 0) 
	{
		assert(0);
		return false;
	}

	item_t *item;
	item = &g_cgv.myCharacterInfo->item[itemIdx];
	if( item->durability > 0 )
	{
		item->durability --;
	}
	else
	{
		GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_cgv.myCharacterInfo->inventory[invenPos] = -1;
	}
	return true;
}


void GTH_ProcessMessage_EpackChargSoul()
{
	GTH_UnlockSending();		

	int reply = 1;
	int error = 1;
	reply = MSG_ReadLong();	

	if ( reply == 1 )
	{
		int			invenPos;	
		int			itemIdx;
		
		item_t		*item	= NULL;
		invenPos	= MSG_ReadByte();
		
		if ( invenPos < 0 )
			goto error;
		
		itemIdx		= g_cgv.myCharacterInfo->inventory[invenPos];
		
		if ( itemIdx < 0 )
			goto error;
		
		item		= &g_cgv.myCharacterInfo->item[itemIdx];
		if ( item == NULL)	
			goto error;
		
		
		if( item->durability > 0 )	
		{
			item->durability --;	
		}
		
		else	
		{			
			GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_cgv.myCharacterInfo->inventory[invenPos] = -1;
		}

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

		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_RECOVERY_SA ), 
			GTH_SOUND_PLAY_ONCE );	
		return;

	}
	else
	{
		error = MSG_ReadLong();		

		
		switch(error) {
		case ERROR_CONSUMPTIVE_CHARGE_ONLY_NPC:
			g_ifMng->SetMessage(g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 405 )); 
			break;
		case ERROR_CONSUMPTIVE_EQUIP:
			g_ifMng->SetMessage(g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 406 )); 
			break;
		case ERROR_CONSUMPTIVE_CHARGE_COUNT:
			g_ifMng->SetMessage(g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322),  g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 407 ));
			break;
		case ERROR_CONSUMPTIVE_DELETE_CHARGE:
			{
				g_cgv.myCharacterInfo->equipment[EQUIP_EPACK]=-1;

				
				int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
				if( epackIdx >= 0 && epackIdx < MAX_NUMBER_OF_OWNITEM)
				{
					GTH_DeleteMyItem(&g_cgv.myCharacterInfo->item[epackIdx]);
				}

				g_ifMng->SetMessage(g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 408 ));
			}
			break;

		default:
			goto error;
		}
		return;
	}

error:
	
	g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL, 395));

	
}









void GTH_SendMessageRequest_RepairItemConfirm(int RepairType, int invenType, int pos )
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);
		MSG_WriteShort(&netMessage, ITEM_REPARE );		
		MSG_WriteShort(&netMessage, CC_ITEM_REPARE );		
		MSG_WriteByte(&netMessage,  RepairType);		
		MSG_WriteByte(&netMessage,  invenType);		
		MSG_WriteByte(&netMessage,  pos);		
		NET_SendMessage(&gsSocket,  &netMessage);
	}
	MSG_EndWriting(&netMessage);		
}


int GTH_ProcessMessageReply_RepairItemConfirm()
{
	int reply, error, i, num;
	int pos, dur;
	int curSE;
	char szTemp[_MAX_PATH+1];
	enumRepairType type;

	enum enumExtendSecondPacket_ItemRepare packetGroup;	
	packetGroup = static_cast<enumExtendSecondPacket_ItemRepare>(MSG_ReadShort());
	if ( packetGroup != GSC_ITEM_REPARE)
		return 0;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{		
		type = static_cast<enumRepairType>(MSG_ReadLong());
		int cost = MSG_ReadLong();
		int RepairItemCount = MSG_ReadByte();
		int ItemTableIdx = MSG_ReadLong();

		switch( type )
		{
		case REPAIR_ITEM_SUB0 : 
		case REPAIR_ITEM_SUB1 :
			{							
				
				if ( ItemTableIdx >= 0)
				{				
					_snprintf( szTemp,_MAX_PATH, g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 251 ), 
						g_itemTable[ItemTableIdx].name, RepairItemCount,  g_ifMng->GetNumberString( cost ));
					
					szTemp[_MAX_PATH] = NULL;
					g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_REPAIR_ITEM );
				}
			}
			break;

		case REPAIR_ITEM_ALL :	
			{
				_snprintf( szTemp,_MAX_PATH, g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 152 ), 
					g_ifMng->GetNumberString( cost ));
				
				szTemp[_MAX_PATH] = NULL;					
				g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_ALLREPAIR_ITEM );	
				
			}
			break;
		}		
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_REPAIRITEM_NONE :
			break;

		case ERROR_REPAIRITEM_1 :
			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322),  
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,36), 1, IF_MESSAGE_NONE );
			break;

		case ERROR_REPAIRITEM_2 :			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322),  
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,37), 1, IF_MESSAGE_NONE );
			break;

			
		case ERROR_REPAIRITEM_3 :				
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322),  
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,314), 1, IF_MESSAGE_NONE );						
			break;

		case ERROR_REPAIRITEM_4:
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322),  
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,315), 1, IF_MESSAGE_NONE );						
			break;		

		}
	}

	return 1;
}