#include "../global.h"

extern i3sizebuf_t netMessage; 



int GTH_SendMessage_SyncItemObject( playerCharacter_t *pc )
{
	int i;
	int idx;

	if( !pc->ready ) return 0;

	GTH_GetObjectListInValidRange(pc->worldIdx, pc->position);

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYNCITEMOBJECT);
		MSG_WriteByte(&netMessage, g_zoneObjectListNumber);
		
		
		for (i=0; i < g_zoneObjectListNumber; i++)
		{
			idx = g_zoneObjectList[i].idx;
			MSG_WriteShort(&netMessage, g_object[idx].idx);
			MSG_WritePosition(&netMessage, g_object[idx].position);
			
			
			MSG_WriteByte(&netMessage, g_object[idx].objectType);
			
			if( g_object[idx].objectType == OBJECT_TYPE_ITEM )
			{
				MSG_WriteShort(&netMessage, g_object[idx].item.itemTableIdx);
			} else if( g_object[idx].objectType == OBJECT_TYPE_PORTAL )
			{
				MSG_WriteShort(&netMessage, g_object[idx].portal.portalType );
			}
			
		}
		
		NET_SendMessage(&pc->sock, &netMessage);  
	}
	MSG_EndWriting(&netMessage);
	return 1;
}

void GTH_SendMessage_AddMultiItemObject( itemObject_t itemObject[], int number, vec3_t origin )
{
	int i;
	int idx;


	GTH_GetPCListInValidRange( itemObject[0].worldIdx, itemObject[0].position );
	if ( g_zonePCListNumber < 1 ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_ADDMULTIITEMOBJECT);
		MSG_WritePosition(&netMessage, origin);
		
		MSG_WriteByte(&netMessage, number );
		
		for( i = 0; i < number; i ++ )
		{
			MSG_WriteShort(&netMessage, itemObject[i].idx);
			MSG_WritePosition(&netMessage, itemObject[i].position);
			
			MSG_WriteByte(&netMessage, itemObject[i].objectType);
			
			if( itemObject[i].objectType == OBJECT_TYPE_ITEM )
			{
				MSG_WriteShort(&netMessage, itemObject[i].item.itemTableIdx);
			} else if( itemObject[i].objectType == OBJECT_TYPE_PORTAL )
			{
				MSG_WriteShort(&netMessage, itemObject[i].portal.portalType );
			}
			
		}
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	MSG_EndWriting(&netMessage);
}
void GTH_SendMessage_AddItemObject(itemObject_t *itemObject)
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(itemObject->worldIdx, itemObject->position);
	if (g_zonePCListNumber < 1)	return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_ADDITEMOBJECT);
		MSG_WriteShort(&netMessage, itemObject->idx);
		MSG_WritePosition(&netMessage, itemObject->position);
		MSG_WriteByte(&netMessage, itemObject->objectType);
		
		if( itemObject->objectType == OBJECT_TYPE_ITEM )
		{
			MSG_WriteShort(&netMessage, itemObject->item.itemTableIdx);
		} else if( itemObject->objectType == OBJECT_TYPE_PORTAL )
		{
			MSG_WriteShort(&netMessage, itemObject->portal.portalType);
		}
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendMessage(&g_pc[idx].sock, &netMessage);
		} 
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_DeleteItemObject(itemObject_t *itemObject)
{
	int i;
	int idx;

	
	GTH_GetPCListInValidRange(itemObject->worldIdx, itemObject->position);
	if (g_zonePCListNumber < 1)	return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_DELETEITEMOBJECT);
		MSG_WriteShort(&netMessage, itemObject->idx);
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;
			if ( !g_pc[idx].ready ) continue;
			NET_SendMessage(&g_pc[idx].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ReplyDropItem(playerCharacter_t *pc, int reply, int error, int itemIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_DROPITEM);
		MSG_WriteByte(&netMessage, reply);
		if (reply > 0)
		{
			MSG_WriteByte(&netMessage, itemIdx);
			MSG_WriteShort(&netMessage, pc->curItemWeight);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ReplyPickUpItem(playerCharacter_t *pc, int reply, int error, item_t *item )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PICKUPITEM);
		MSG_WriteByte(&netMessage, reply);
		
		if (reply > 0)
		{
			
			MSG_WriteItem(&netMessage, item);
			MSG_WriteShort(&netMessage, pc->curItemWeight);
		}
		else 
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_ReplyGetInventoryItem(playerCharacter_t *pc, int reply, int error, int inventoryPos, int equipPos )
{
	int itemIdx;
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_GETINVENTORYITEM);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, inventoryPos);
			MSG_WriteByte(&netMessage, equipPos);
			
			if( reply == 5 )
			{
				MSG_WriteByte(&netMessage, pc->mouseInventory);
				itemIdx = pc->inventory[inventoryPos];
				MSG_WriteItem( &netMessage, &pc->item[itemIdx] );
				MSG_WriteItem( &netMessage, &pc->item[pc->mouseInventory] );
			}
		}
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	GTH_PC_Calculate( pc, PCINFO_BASIC );
}

void GTH_SendMessage_ReplyGetEquipItem(playerCharacter_t *pc, int reply, int error, int inventoryPos, int equipPos)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_GETEQUIPITEM);
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, inventoryPos);
			MSG_WriteByte(&netMessage, equipPos);
		}
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	GTH_PC_Calculate( pc, PCINFO_BASIC );
}

void GTH_SendMessage_LootValue( playerCharacter_t *pc, __int64 exp, __int64 genExp, int lootNSE, int genCapability, int sharingType )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		
		MSG_WriteByte(&netMessage, GSC_LOOTVALUE);
		MSG_Write64Int(&netMessage, exp);
		MSG_Write64Int(&netMessage, genExp);
		MSG_WriteLong(&netMessage, lootNSE);
		MSG_WriteLong(&netMessage, genCapability);
		
		MSG_WriteByte(&netMessage, sharingType);
		
		
		
		
		NET_SendUnreliableMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_DestroyItem( playerCharacter_t *pc, int reply, int error, int itemIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_DESTROY_ITEM );
		
		MSG_WriteByte(&netMessage, reply );
		
		if( reply > 0 )
		{
			
			MSG_WriteShort(&netMessage, (int)pc->curChargeRA );

			
			MSG_WriteShort(&netMessage, (int)pc->curChargeSA );

			MSG_WriteShort(&netMessage, itemIdx );
			MSG_WriteShort(&netMessage, pc->curItemWeight );
			
			
			MSG_WriteShort(&netMessage, error);
		}
		else
		{
			MSG_WriteByte(&netMessage, error );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_AddInvItem( playerCharacter_t *pc, int reply, int error, item_t *item )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_ADDITEMINV);
		
		MSG_WriteByte(&netMessage, reply);
		if( reply )
		{
			MSG_WriteItem(&netMessage, item);
			
			
			
			MSG_WriteByte(&netMessage, error);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_DeleteInvItem( playerCharacter_t *pc, int reply, int error, int itemIdx, int number )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_DELETEITEMINV);
		
		MSG_WriteByte(&netMessage, reply);
		if( reply )
		{
			MSG_WriteShort(&netMessage, itemIdx);
			MSG_WriteShort(&netMessage, number);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}
