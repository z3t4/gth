







#include "../Global.h"





CWeatherSystem::CWeatherSystem()
{	
	m_EventState = EVENT_ACTIVE;	
	InitWeatherSystem();
}







void CWeatherSystem::InitWeatherSystem()
{		
	for (int WorldCount = 0; WorldCount < MAX_WORLDS; WorldCount++)
	{
		m_EventWorld[WorldCount].m_EventLimitTime = 0;								
		m_EventWorld[WorldCount].m_EventState = WEATHER_SYSTEM_WORLD_NONE;
		m_EventWorld[WorldCount].m_WeatherType = TIME_DAY;
	}
}

CWeatherSystem::~CWeatherSystem()
{

}







void CWeatherSystem::GTH_Process_Weather_System()
{
	
	enum enumExtendSecondPacket_WeatherSystem SecoundPacketGroup;
	SecoundPacketGroup = static_cast <enum enumExtendSecondPacket_WeatherSystem>(MSG_ReadShort());

	if ( SecoundPacketGroup != CC_WEATHER_SYSTEM )
		return;

	int type = MSG_ReadByte();

	switch(type)
	{
		
	case CC_WEATHER_SYSTEM_REQUEST_USE_WEATHER_ITEM:
		
		GTH_ProcessMessage_WeatherSetting();
		break;

		
	case CC_WEATHER_SYSTEM_CHEAT_SETING:		
		GTH_ProcessMessageEventSeting();
		break;
	case CC_WEATHER_SYSTEM_REQUEST_WORLD_STATE:
		GTH_ProcessMessage_Request_WordlState();
		break;

	case CC_WEATHER_SYSTEM_NOTICE:
		GTH_ProcessMessage_Event_UserNotice();
		break;
	case CC_WEATHER_SYSTEM_CASTING_EVENT_SETING:
		GTH_ProcessMessageBroadCastEventSeting();
		break;;
		



	default:
		return;
	}
	
}








void CWeatherSystem::GTH_ProcessMessage_WeatherSetting(void)
{
	item_t *item;
	int itemIdx, itemTableIdx; 
	int invenPos = -1;
	int reply = 1, error = 0;
	int isUseItem = 0;	
	float limitTime;
	TIMESTATE weathertype;

	
	weathertype = static_cast<TIMESTATE>(MSG_ReadByte());		
	limitTime   = MSG_ReadLong();
	isUseItem   = MSG_ReadByte();
	

	
	if ( isUseItem == 1 )
	{			
		invenPos   = MSG_ReadByte();
		if ( invenPos < 0 || invenPos >= MAX_INVENTORY_SIZE)
		{
			reply = 0;
			error = ERROR_WEATHER_SYSTEM_USE_ITEM;
			GTH_SendMessage_Reply_WeatherSystem_UseItem(g_curPC, reply, error, invenPos);		
			return;
		}

		itemIdx = g_curPC->inventory[invenPos];
		
		if( itemIdx < 0 )
		{
			reply = 0;
			error = ERROR_WEATHER_SYSTEM_USE_ITEM;
			GTH_SendMessage_Reply_WeatherSystem_UseItem(g_curPC, reply, error, invenPos);		
			return;
		}
		
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
				error = ERROR_WEATHER_SYSTEM_USE_ITEM;				
				GTH_SendMessage_Reply_WeatherSystem_UseItem(g_curPC, reply, error, invenPos);
				return;
			}
		}
		GTH_SendMessage_Reply_WeatherSystem_UseItem(g_curPC, reply, error, invenPos);
	}

	
	
	m_EventWorld[g_curPC->worldIdx].m_EventState	 = WEATHER_SYSTEM_WORLD_PROGRESS;		
	m_EventWorld[g_curPC->worldIdx].m_EventLimitTime = limitTime;
	m_EventWorld[g_curPC->worldIdx].m_WorldIdx		 = g_curPC->worldIdx;
	m_EventWorld[g_curPC->worldIdx].m_WeatherType	 = weathertype;	
	
	GTH_SendMessage_BroadCast_WeatherSystemEvent(g_curPC, g_curPC->worldIdx);

}







void CWeatherSystem::GTH_SendMessage_Reply_WeatherSystem_UseItem(playerCharacter_t* pc, int reply, int error, int invenpos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, WEATHER_SYSTEM);
		MSG_WriteShort(&netMessage, GSC_WEATHER_SYSTEM);
		MSG_WriteByte(&netMessage, GSC_WEATHER_SYSTEM_REPLY_USE_WEATHER_ITEM);
		MSG_WriteLong(&netMessage, reply);

		if ( reply == 1)		
			MSG_WriteByte(&netMessage, invenpos);					
		else
			MSG_WriteLong(&netMessage, error);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	
}








void CWeatherSystem::GTH_SendMessage_BroadCast_WeatherSystemEvent(playerCharacter_t* pPc, int worldidx)
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
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort(&netMessage, WEATHER_SYSTEM);
			MSG_WriteShort(&netMessage, GSC_WEATHER_SYSTEM);
			MSG_WriteByte(&netMessage, GSC_WEATHER_SYSTEM_REPLY_CURRENT_WEATHER);
			MSG_WriteLong(&netMessage, (int)m_EventWorld[worldidx].m_EventState);	
			MSG_WriteLong(&netMessage, (int)m_EventWorld[worldidx].m_WeatherType);








			NET_SendMessage(&g_pc[PcCount].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);			
	}		
}






void CWeatherSystem::ProcessEventTIme()
{


	
	for (int worldCount = 0; worldCount < MAX_WORLDS; worldCount++)
	{
		if ( (m_EventWorld[worldCount].m_WorldIdx < 0) || 
			(m_EventWorld[worldCount].m_WorldIdx >= MAX_WORLDS) ||  
			(m_EventWorld[worldCount].m_EventState == WEATHER_SYSTEM_WORLD_NONE )	)
			continue;

		if ( m_EventWorld[worldCount].m_EventLimitTime > 0 )
			m_EventWorld[worldCount].m_EventLimitTime -= g_timer->GetElapsedMilliTime();		
		
		
		if ( m_EventWorld[worldCount].m_EventLimitTime < 0 )			
		{
			m_EventWorld[worldCount].m_EventState = WEATHER_SYSTEM_WORLD_NONE;
			m_EventWorld[worldCount].m_WorldIdx = -1;
			m_EventWorld[worldCount].m_WeatherType = TIME_DAY;
			m_EventWorld[worldCount].m_EventLimitTime = 0;

			GTH_SendMessage_BroadCast_WeatherSystemEvent(NULL, worldCount);
		}
	}
}







void CWeatherSystem::GTH_ProcessMessageEventSeting()
{
	int event = MSG_ReadByte();			
	if ( event == 1)
	{
		m_EventState = EVENT_ACTIVE;
		g_logSystem->WriteRLog("WeatherSystem EventStting");
	}		
	else 
	{				
		InitWeatherSystem();
		m_EventState = EVENT_INACTIVE;

		for (int WorldCount = 0; WorldCount < MAX_WORLDS; WorldCount++)
			GTH_SendMessage_BroadCast_WeatherSystemEvent(NULL, WorldCount);
		
	}	
}







void CWeatherSystem::GTH_ProcessMessageBroadCastEventSeting()
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








void CWeatherSystem::GTH_ProcessMessage_Request_WordlState()
{
	
	int worldidx = MSG_ReadLong();	
	int reply = 1;
	enumEventWorldState state = WEATHER_SYSTEM_WORLD_NONE;

	if ( (worldidx < 0) || 
		 (worldidx >= MAX_WORLDS) ||
		 (m_EventState == EVENT_INACTIVE) )		
	{
		reply = 0;
		state = WEATHER_SYSTEM_WORLD_NONE;
		GTH_SendMessage_Reply_WordlState(g_curPC, reply, state);
		return;
	}	

	state = m_EventWorld[worldidx].m_EventState;
	GTH_SendMessage_Reply_WordlState(g_curPC, reply, (int)state);	
}







void CWeatherSystem::GTH_SendMessage_Reply_WordlState(playerCharacter_t* pc, int reply, int state)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, WEATHER_SYSTEM);
		MSG_WriteShort(&netMessage, GSC_WEATHER_SYSTEM);
		MSG_WriteByte(&netMessage, GSC_WEATHER_SYSTEM_REQUEST_WORLD_STATE);
		MSG_WriteByte(&netMessage, reply);
		
		if ( reply == 1)
			MSG_WriteLong(&netMessage, state);

		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void CWeatherSystem::GTH_SendMessage_Reply_WordlStateToMemberServer(int state)
{

	
	if ( state == 1)
	{
		m_EventState = EVENT_ACTIVE;
		
	}		
	else 
	{				
		InitWeatherSystem();
		m_EventState = EVENT_INACTIVE;
	}


	
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		
		MSG_WriteShort(&netMessage, WEATHER_SYSTEM);
		MSG_WriteShort(&netMessage, GSC_WEATHER_SYSTEM);
		MSG_WriteByte(&netMessage, CC_WEATHER_SYSTEM_CHEAT_SETING);		
		MSG_WriteByte(&netMessage, state);
		
		for (i=1; i < MAX_MEMBER_SERVER; i++) 
		{
			if ( !g_memberServer[i].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);



	

}






void CWeatherSystem::GTH_SendMessage_Reply_WordlStateToManagerServer(int state)
{
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, WEATHER_SYSTEM);
		MSG_WriteShort(&netMessage, GSC_WEATHER_SYSTEM);
		MSG_WriteByte(&netMessage, CC_WEATHER_SYSTEM_CASTING_EVENT_SETING);		
		MSG_WriteByte(&netMessage, state);	
		NET_SendUnreliableMessage(&localSocket, &netMessage);
		
	}
	MSG_EndWriting(&netMessage);	
}







void CWeatherSystem::GTH_ProcessMessage_Event_UserNotice()
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

