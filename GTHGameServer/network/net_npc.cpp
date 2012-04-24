#include "../global.h"

extern i3sizebuf_t netMessage; 


int GTH_SendMessage_ReplyNpcChat( playerCharacter_t* pc, int reply, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_NPCCHAT);
		
		MSG_WriteByte(&netMessage, reply );
		
		NET_SendMessage(&pc->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);

	return 1;
}

int GTH_SendMessage_ReplyQuitNpcChat( playerCharacter_t* pc, int reply, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_QUITNPCCHAT);
		NET_SendMessage(&pc->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);

	return 1;
}


int GTH_SendMessage_NpcMessage( playerCharacter_t* pc )
{
	int dialogIdx, buttonNumber, buttonIdx;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_NPCMESSAGE);
		
		dialogIdx = pc->npcAi.sendMsgIdx;
		MSG_WriteShort(&netMessage, dialogIdx);
		
		buttonNumber = pc->npcAi.replyMsgIdxNumber;
		MSG_WriteByte(&netMessage, buttonNumber);
		
		for( int i = 0; i < buttonNumber; i ++ )
		{
			MSG_WriteByte(&netMessage, i);
			buttonIdx = pc->npcAi.replyMsgIdx[i];
			MSG_WriteShort(&netMessage, buttonIdx);
		}
		
		NET_SendMessage(&pc->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);

	return 1;
}

int GTH_SendMessage_ReplyStore( playerCharacter_t* pc, storeStruct_t *store, int type )
{
    
    
	int chaoclass=GetChaosClass(pc);
	float chargeRACost=0.0f;
	float chargeSACost=0.0f;
    unsigned int storevalue=1;
	switch(chaoclass) {
	case CHAOS_DISORDER:
		storevalue=storevalue*3;
		 
		break;
	case CHAOS_MONSTER:
		storevalue=storevalue*5;
		break;
	default:
		storevalue=1;
		break;
	}
	
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_STORE );
		
		if( store != NULL )
		{
			
			
			MSG_WriteByte(&netMessage, type);									
			
			MSG_WriteShort(&netMessage, store->itemNumber );
			
			for( i = 0; i < store->itemNumber; i ++ )
			{
				MSG_WriteByte(&netMessage, store->item[i].page );
				MSG_WriteShort(&netMessage, store->item[i].curNumber );
				MSG_WriteLong(&netMessage, store->item[i].price*storevalue);
				
				MSG_WriteShort(&netMessage, i );
				MSG_WriteItem(&netMessage, &store->item[i].item );
			}
			
			
			
		
		
		
			MSG_WriteFloat(&netMessage, store->chargeRACost );
			MSG_WriteFloat(&netMessage, store->chargeSACost );
		}
		else
		{
			MSG_WriteByte(&netMessage, 1);
			MSG_WriteShort(&netMessage, type );
		}
		
		NET_SendMessage(&pc->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);

	return 1;
}

int GTH_SendMessage_ReplyBuyItem(playerCharacter_t *pc, int reply, int error, item_t *item, int storeItemIdx, int curNumber )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_BUYITEM);
		MSG_WriteByte(&netMessage, reply);
		
		if (reply > 0)
		{
			MSG_WriteShort(&netMessage, storeItemIdx );
			MSG_WriteShort(&netMessage, curNumber);
			
			MSG_WriteItem(&netMessage, item);
			MSG_WriteLong(&netMessage, pc->curChargeSE);
			MSG_WriteShort(&netMessage, pc->curItemWeight);
		}
		else 
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return 1;
}

int GTH_SendMessage_ReplySellItem(playerCharacter_t *pc, int reply, int error, int itemIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		
		MSG_WriteByte(&netMessage, GSC_REPLY_SELLITEM);
		MSG_WriteByte(&netMessage, reply);
		
		if (reply > 0)
		{
			MSG_WriteByte(&netMessage, itemIdx);
			MSG_WriteLong(&netMessage, pc->curChargeSE);
			MSG_WriteShort(&netMessage, pc->curItemWeight);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return 1;
}

int GTH_SendMessage_ReplyBinding( playerCharacter_t* pc, int reply, int error, int type, int bindingIdx, int bindingCost )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_BINDING);
		MSG_WriteByte(&netMessage, reply);
		
		if (reply > 0)
		{
			MSG_WriteByte(&netMessage, type);
			MSG_WriteByte(&netMessage, bindingIdx);
			MSG_WriteLong(&netMessage, bindingCost );
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return 1;
}


int GTH_SendMessage_ReplyEpackStore( playerCharacter_t* pc, int reply, int error,int EpkRemainCount)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_EPACKSTORE);
		MSG_WriteByte(&netMessage, reply);
		
		if (reply > 0)
		{
			MSG_WriteLong(&netMessage, pc->curChargeSE );

			
			MSG_WriteLong(&netMessage, (int) pc->curChargeRA );

			
			MSG_WriteLong(&netMessage, (int) pc->curChargeSA );

			
			MSG_WriteShort(&netMessage, EpkRemainCount);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return 1;
}

int GTH_SendMessage_ReplyRepairItem( playerCharacter_t* pc, int reply, int error, int type, int repairItemIdx[], int repairItemDur[], int numOfRepairItem )
{
	int i;
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_REPAIR );
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteLong(&netMessage, pc->curChargeSE );
			MSG_WriteByte(&netMessage, type );
			switch( type )
			{
			case 0 : case 1 :
				MSG_WriteShort(&netMessage, repairItemIdx[0] );
				MSG_WriteShort(&netMessage, repairItemDur[0] );
				break;
			case 2 :
				MSG_WriteByte(&netMessage, numOfRepairItem );
				for( i = 0; i < numOfRepairItem; i ++ )
				{
					MSG_WriteShort(&netMessage, repairItemIdx[i] );
					MSG_WriteShort(&netMessage, repairItemDur[i] );
				}
				break;
			}
		}
		else
		{
			MSG_WriteByte(&netMessage, error );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return 1;
}

int GTH_SendMessage_ReplyBuySkill( playerCharacter_t *pc, int reply, int error, int inventoryPos, int buySphereCost, int skillIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_BUY_SKILL);
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteByte(&netMessage, inventoryPos);
			MSG_WriteByte(&netMessage, buySphereCost);
			MSG_WriteByte(&netMessage, skillIdx);
			MSG_WriteShort(&netMessage, pc->skill[skillIdx].tableIdx);
			MSG_WriteLong(&netMessage, pc->skill[skillIdx].iLevel);
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	return 1;
}




int GTH_SendMessage_QuestLogic_ChatMsg( playerCharacter_t *pc, int idx ,int type)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_QUESTLOGIC );
		MSG_WriteByte(&netMessage, QUESTLOGIC_CHATMSG);
		MSG_WriteShort(&netMessage, type);
		MSG_WriteShort(&netMessage, idx);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	return 1;
}

int GTH_SendMessage_QuestLogic_Awaken( playerCharacter_t *pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_QUESTLOGIC );
		MSG_WriteByte(&netMessage, QUESTLOGIC_AWAKEN);
		
		MSG_WriteShort(&netMessage, pc->pcJob);
		
		for( int i = 0 ; i < GEN_NUMBER; i ++ )
			MSG_WriteLong(&netMessage, pc->genCapability[i]);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return 1;
}


int GTH_SendMessage_QuestLogic_InitStatus( playerCharacter_t *pc, int reply, int error, int type, int nak )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_QUESTLOGIC );
		MSG_WriteByte(&netMessage, QUESTLOGIC_INITSTATUS );
		
		MSG_WriteByte(&netMessage, reply);
		if( reply )
		{
			MSG_WriteByte(&netMessage, type);
			
			if( !type )
			{
				MSG_WriteLong(&netMessage, nak);
			}
			else
			{
				MSG_WriteShort(&netMessage, pc->bonusStatusPoint);
			}
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	return 1;
}



int GTH_SendMessage_NPCInfo( playerCharacter_t * pc )
{
	int npcNumber;
	monsterCharacter_t *pNpc;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_NPCINFO );
		
		LIST_NONPLAYER_STRUCT::iterator itor;
		
		npcNumber = 0;
		
		for( itor = g_sNpcList.begin();	itor != g_sNpcList.end(); itor ++ )
		{
			pNpc = ( *itor );
			if( pNpc->worldIdx == pc->worldIdx ) npcNumber ++;
		}
		
		MSG_WriteByte( &netMessage, npcNumber );
		
		for( itor = g_sNpcList.begin();	itor != g_sNpcList.end(); itor ++ )
		{
			pNpc = ( *itor );
			if( pNpc->worldIdx != pc->worldIdx ) continue;
			
			MSG_WritePosition( &netMessage, pNpc->position );
			MSG_WriteShort( &netMessage, pNpc->tableIdx );
		}
		
		MSG_WriteByte( &netMessage, pc->worldIdx );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return 1;
}



int GTH_SendMessage_Reply_MasicBook_RepairItem( playerCharacter_t* pc, int reply, int error, int repairItemIdx[], int repairItemDur[], int numOfRepairItem ,int InvenPos )
{
	int i;
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,   GSC_EXTEND);
		MSG_WriteByte(&netMessage,   GSC_EXTEND_REPLY_USE_MASIC_BOOK);
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, numOfRepairItem );
			for( i = 0; i < numOfRepairItem; i ++ )
			{
				MSG_WriteShort(&netMessage, repairItemIdx[i] );
				MSG_WriteShort(&netMessage, repairItemDur[i] );
			}

			MSG_WriteShort(&netMessage, InvenPos );
		}
		else
		{
			MSG_WriteByte(&netMessage, error );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	return 1;
}


void GTH_SendMessage_Reply_RepairItemConfirm
(playerCharacter_t* pc, int reply, int erroridx, enumRepairType repairtype, int cost, int repairCount, int itemtableidx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);
		MSG_WriteShort(&netMessage, ITEM_REPAIR);
		MSG_WriteShort(&netMessage, GSC_ITEM_REPAIR);				
		MSG_WriteByte(&netMessage,  reply);	

		if ( reply == 0)		
			MSG_WriteByte(&netMessage, erroridx);			
		else
		{
			MSG_WriteLong(&netMessage, repairtype);
			MSG_WriteLong(&netMessage, cost);
			MSG_WriteByte(&netMessage, repairCount);
			MSG_WriteLong(&netMessage, itemtableidx);
		}		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}