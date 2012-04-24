




#include "../Global.h"





CChristMasEvent::CChristMasEvent()
{	
	m_EventState = EVENT_INACTIVE;	
	InitChristMasEvent();
}







void CChristMasEvent::InitChristMasEvent()
{	


	
	if ( m_EventState == EVENT_ACTIVE)	
	{
		for (int WorldCount = 0; WorldCount < MAX_WORLDS; WorldCount++)
		{
			if ( m_EventWorld[WorldCount].m_EventState == CHRISTMAS_EVENT_WORLD_PROGRESS )		
			{
				m_EventWorld[WorldCount].m_EventState = CHRISTMAS_EVENT_WORLD_NONE;				
				m_EventWorld[WorldCount].m_EventLimitTime = 0;
				GTH_SendMessage_BroadCast_ChristMasEvent(NULL, WorldCount);		
			}
		}
	}
	else
	{
		
		for (int WorldCount = 0; WorldCount < MAX_WORLDS; WorldCount++)
		{
			m_EventWorld[WorldCount].m_EventLimitTime = 0;								
			m_EventWorld[WorldCount].m_EventState = CHRISTMAS_EVENT_WORLD_NONE;
		}
		
		m_EventState = EVENT_INACTIVE;
	}
}

CChristMasEvent::~CChristMasEvent()
{

}







void CChristMasEvent::GTH_Process_ChristMas_Event()
{
	int type = MSG_ReadByte();
	switch(type)
	{
	case CC_CHRISTMAS_EVENT_REQUEST_USE_SNOWITEM:
		GTH_ProcessMessage_ChristMas_UseItem();
		break;
	case CC_CHRISTMAS_EVENT_SETING:		
		GTH_ProcessMessageEventSeting();
		break;
	case CC_CHRISTMAS_EVENT_REQUEST_WORLD_STATE:
		GTH_ProcessMessage_Request_WordlState();
		break;

	case CC_CHRISTMAS_EVENT_NOTICE:
		GTH_ProcessMessage_Event_UserNotice();
		break;
	case CC_CHRISTMAS_BROAD_CASTING_EVENT_SETING:
		GTH_ProcessMessageBroadCastEventSeting();
		break;;
		



	default:
		return;
	}
	
}







void CChristMasEvent::GTH_ProcessMessage_ChristMas_UseItem(void)
{
	item_t *item;
	int itemIdx, itemTableIdx; 
	int invenPos;
	int reply = 1, error = 0;		
	
	invenPos = MSG_ReadByte();
	itemIdx = g_curPC->inventory[invenPos];

	if( itemIdx < 0 )
	{
		reply = 0;
		error = ERROR_CHRISTMAS_EVENT_USE_ITEM;
		GTH_SendMessage_Reply_ChristMas_UseItem(g_curPC, reply, error, invenPos);		
		return;
	}
	
	if ( m_EventState == EVENT_INACTIVE)
	{
		reply = 0;
		error = ERROR_CHRISTMAS_EVENT_USE_ITEM2;
		GTH_SendMessage_Reply_ChristMas_UseItem(g_curPC, reply, error, invenPos);		
		return;
	}
	

	else
	{		
		item = &g_curPC->item[itemIdx];
		itemTableIdx = item->itemTableIdx;
		
		
		if( item->durability > 0 )
		{
			item->durability --;
			g_DBGameServer->SaveItem( g_curPC, item, itemIdx );															
		}
		else
		{			
			
			GTH_DeleteMyItem( g_curPC, item );
			g_curPC->inventory[invenPos] = -1;			
			if ( !g_DBGameServer->RemoveItem( g_curPC, itemIdx ) )
			{				
				reply = 0;							
				error = ERROR_CHRISTMAS_EVENT_USE_ITEM;				
				GTH_SendMessage_Reply_ChristMas_UseItem(g_curPC, reply, error, invenPos);
				return;
			}
		}
		GTH_SendMessage_Reply_ChristMas_UseItem(g_curPC, reply, error, invenPos);

		
		m_EventWorld[g_curPC->worldIdx].m_EventState	 = CHRISTMAS_EVENT_WORLD_PROGRESS;

		


		m_EventWorld[g_curPC->worldIdx].m_EventLimitTime = 	CHRISTMASEVENT_LIMITTIME;

		m_EventWorld[g_curPC->worldIdx].m_WorldIdx		 = g_curPC->worldIdx;


		GTH_SendMessage_BroadCast_ChristMasEvent(g_curPC, g_curPC->worldIdx);
	}

}







void CChristMasEvent::GTH_SendMessage_Reply_ChristMas_UseItem(playerCharacter_t* pc, int reply, int error, int invenpos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_REPLY_CHRISTMAS_EVENT);
		MSG_WriteByte(&netMessage, GSC_CHRISTMAS_EVENT_REPLY_USE_SNOWITEM);
		MSG_WriteLong(&netMessage, reply);

		if ( reply == 1)		
			MSG_WriteByte(&netMessage, invenpos);					
		else
			MSG_WriteLong(&netMessage, error);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	
}








void CChristMasEvent::GTH_SendMessage_BroadCast_ChristMasEvent(playerCharacter_t* pPc, int worldidx)
{		
	for( int PcCount = 0; PcCount < MAX_PCS; PcCount ++ )
	{
		if ( g_pc[PcCount].idx < 0 || g_pc[PcCount].idx >= MAX_PCS)
			continue;
		
		if ( !g_pc[PcCount].active ) 
			continue;			
		
		if( g_pc[PcCount].worldIdx != worldidx )		
			continue;			
		
		MSG_BeginWriting( &netMessage );
		
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_EXTEND);
			MSG_WriteByte(&netMessage, GSC_REPLY_CHRISTMAS_EVENT);
			MSG_WriteByte(&netMessage, GSC_CHRISTMAS_EVENT_REPLY_CHRISTEVENT);
			MSG_WriteLong(&netMessage, (int)m_EventWorld[worldidx].m_EventState);	

			
			MSG_WriteLong(&netMessage, m_EventState);	








			NET_SendMessage(&g_pc[PcCount].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);			
	}		
}






void CChristMasEvent::ProcessEventTIme()
{
	if ( m_EventState == EVENT_INACTIVE)
		return;
	
	for (int worldCount = 0; worldCount < MAX_WORLDS; worldCount++)
	{
		if ( (m_EventWorld[worldCount].m_WorldIdx < 0) || 
			(m_EventWorld[worldCount].m_WorldIdx >= MAX_WORLDS ) )
			continue;

		if ( m_EventWorld[worldCount].m_EventLimitTime > 0 )
			m_EventWorld[worldCount].m_EventLimitTime -= g_timer->GetElapsedMilliTime();		
		
		
		if ( m_EventWorld[worldCount].m_EventLimitTime < 0 )			
		{
			m_EventWorld[worldCount].m_EventState = CHRISTMAS_EVENT_WORLD_NONE;
			m_EventWorld[worldCount].m_WorldIdx = -1;

			m_EventWorld[worldCount].m_EventLimitTime = 0;

			GTH_SendMessage_BroadCast_ChristMasEvent(NULL, worldCount);
		}
	}
}







void CChristMasEvent::GTH_ProcessMessageEventSeting()
{
	int event = MSG_ReadByte();			
	if ( event == 1)
	{
		m_EventState = EVENT_ACTIVE;
		g_logSystem->WriteRLog("ChristMas EventStting");
	}		
	else 
	{				
		InitChristMasEvent();
		m_EventState = EVENT_INACTIVE;
	}	
}







void CChristMasEvent::GTH_ProcessMessageBroadCastEventSeting()
{
	
	if ( g_config.isManager )
	{		
		
		GTH_SendMessage_Reply_WordlStateToMemberServer( MSG_ReadByte());
	}
	else
	{
		
		GTH_SendMessage_Reply_WordlStateToManagerServer( MSG_ReadByte() );
	}
}








void CChristMasEvent::GTH_ProcessMessage_Request_WordlState()
{
	
	int worldidx = MSG_ReadLong();	
	int reply = 1;
	enumEventWorldState state = CHRISTMAS_EVENT_WORLD_NONE;

	if ( (worldidx < 0) || 
		 (worldidx >= MAX_WORLDS) ||
		 (m_EventState == EVENT_INACTIVE) )		
	{
		reply = 0;
		state = CHRISTMAS_EVENT_WORLD_NONE;
		GTH_SendMessage_Reply_WordlState(g_curPC, reply, state);
		return;
	}	

	state = m_EventWorld[worldidx].m_EventState;
	GTH_SendMessage_Reply_WordlState(g_curPC, reply, (int)state);	
}







void CChristMasEvent::GTH_SendMessage_Reply_WordlState(playerCharacter_t* pc, int reply, int state)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_REPLY_CHRISTMAS_EVENT);
		MSG_WriteByte(&netMessage, GSC_CHRISTMAS_EVENT_REQUEST_WORLD_STATE);
		MSG_WriteByte(&netMessage, reply);
		
		if ( reply == 1)
			MSG_WriteLong(&netMessage, state);

		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void CChristMasEvent::GTH_SendMessage_Reply_WordlStateToMemberServer(int state)
{

	
	if ( state == 1)
	{
		m_EventState = EVENT_ACTIVE;
		
	}		
	else 
	{				
		InitChristMasEvent();
		m_EventState = EVENT_INACTIVE;
	}


	
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_REPLY_CHRISTMAS_EVENT);
		MSG_WriteByte(&netMessage, CC_CHRISTMAS_EVENT_SETING);		
		MSG_WriteByte(&netMessage, state);
		
		for (i=1; i < MAX_MEMBER_SERVER; i++) 
		{
			if ( !g_memberServer[i].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);



	

}






void CChristMasEvent::GTH_SendMessage_Reply_WordlStateToManagerServer(int state)
{
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_REPLY_CHRISTMAS_EVENT);
		MSG_WriteByte(&netMessage, CC_CHRISTMAS_BROAD_CASTING_EVENT_SETING);		
		MSG_WriteByte(&netMessage, state);	
		NET_SendUnreliableMessage(&localSocket, &netMessage);
		
	}
	MSG_EndWriting(&netMessage);	
}








BOOL CChristMasEvent::GTH_IsChristMasItem(int ItemIdx)
{	
	if (SANTA_UPPER		== ItemIdx || SANTA_LOWER		== ItemIdx ||	
		SANTA_GLOVES	== ItemIdx || SANTA_BOOTS		== ItemIdx )
		return TRUE;


	return FALSE;

}







BOOL CChristMasEvent::GTH_IsEnableEquipChristMasItem()
{	
	for( int equipItemCount = 0; equipItemCount < MAX_EQUIPMENT; equipItemCount ++ )
	{
		int equipmentIdx = g_curPC->equipment[equipItemCount];

		if ( equipmentIdx < 0)					
			continue;

		int itemTableIdx = g_curPC->item[equipmentIdx].itemTableIdx;
		if ( GTH_IsChristMasItem(itemTableIdx) == TRUE)
			continue;
		

		int tableEquipPos = g_itemTable[itemTableIdx].wearPosition;
		if ( tableEquipPos == ITEM_WEAR_POS_UPPER ||
			tableEquipPos == ITEM_WEAR_POS_LOWER ||	
			tableEquipPos == ITEM_WEAR_POS_AIR_PROTECTOR ||	
			tableEquipPos == ITEM_WEAR_POS_BODY_PROTECTOR ||
			tableEquipPos == ITEM_WEAR_POS_GLOVES || 
			tableEquipPos == ITEM_WEAR_POS_BOOTS  )
			return FALSE;

	}
	return TRUE;
}








BOOL CChristMasEvent::GTH_IsEquipChristMasItem()
{
	for( int equipItemCount = 0; equipItemCount < MAX_EQUIPMENT; equipItemCount ++ )
	{
		int equipmentIdx = g_curPC->equipment[equipItemCount];

		if ( equipmentIdx < 0)
			continue;

		int itemTableIdx = g_curPC->item[equipmentIdx].itemTableIdx;
		if ( GTH_IsChristMasItem(itemTableIdx) == TRUE )
			return TRUE;
	}
	return FALSE;
}







BOOL CChristMasEvent::GTH_IsAllowItemWerePos(int ItemIdx)
{	
	if ( g_itemTable[ItemIdx].wearPosition != ITEM_WEAR_POS_WEAPON &&
		g_itemTable[ItemIdx].wearPosition != ITEM_WEAR_POS_RING &&
		g_itemTable[ItemIdx].wearPosition != ITEM_WEAR_POS_NECKLACE&&
		g_itemTable[ItemIdx].wearPosition != ITEM_WEAR_POS_BRACELET&&
		g_itemTable[ItemIdx].wearPosition != ITEM_WEAR_POS_EARRING&&
		g_itemTable[ItemIdx].wearPosition != ITEM_WEAR_POS_TRANSFER&&
		g_itemTable[ItemIdx].wearPosition != ITEM_WEAR_POS_EPACK )
		return FALSE;

	return TRUE;

}






void CChristMasEvent::GTH_ProcessMessage_Event_UserNotice()
{
	

	char str[MAX_PATH+1];
	_snprintf(str, MAX_PATH, MSG_ReadString());
	str[MAX_PATH] = NULL;

	if ( g_config.isManager )
	{

		
		GTH_SendMessage_NoticeToMember( str );

		
		GTH_SystemAdmin_SetNoticeInfo( str );
	}
	else
	{
		
		GTH_SendMessage_NoticeToManager( str );
	}
}
