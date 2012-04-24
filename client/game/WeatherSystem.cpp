




#include "../global_def.h"


extern CItemTableData gItemTableData;





CWeatherSystem::CWeatherSystem()
{	
	
}
CWeatherSystem::~CWeatherSystem()
{

}


void CWeatherSystem::InitWeatherSystem()
{	

}










void CWeatherSystem::GTH_Process_Weather_System()
{
	enum enumExtendSecondPacket_WeaterSystem SecondPackaetGroup;	
	SecondPackaetGroup = static_cast<enum enumExtendSecondPacket_WeaterSystem>(MSG_ReadShort());

	if ( SecondPackaetGroup != GSC_WEATHER_SYSTEM )
		return;

	int type = MSG_ReadByte();

	switch(type)
	{
		
	case GSC_WEATHER_SYSTEM_REPLY_USE_WEATHER_ITEM:
		GTH_ProcessMessage_Reply_WeatherSetting();		
		break;	

		
	case GSC_WEATHER_SYSTEM_REPLY_CURRENT_WEATHER:
		GTH_ProcessMessage_WeatherSystemEvent();
		break;		

	case GSC_WEATHER_SYSTEM_REQUEST_WORLD_STATE:		
		GTH_ProcessMessage_Request_WorldState();
		break;

	default:
		return;
	}
	
}







void CWeatherSystem::GTH_ProcessMessage_Reply_WeatherSetting(void)
{
	char str[MAX_PATH+1];	

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

		
		
		int	lpackTab = LPACK_TYPE_NORMAL2;
		int lpackidx = 247;

		
		if(item->itemTableIdx >= 0 && item->itemTableIdx < MAX_NUMBER_OF_ITEM_TABLE)
		{
			if(gItemTableData.IsType(&g_itemTable[item->itemTableIdx],CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_WEATHER_RAIN))
			{
				lpackTab = gItemTableData.GetData(&g_itemTable[item->itemTableIdx],CItemTableData::ItemTable_FluxDataValue::LPACK_TAB);
				lpackidx = gItemTableData.GetData(&g_itemTable[item->itemTableIdx],CItemTableData::ItemTable_FluxDataValue::LPACK_INDEX);
			}
		}

		
		sprintf(str, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, lpackidx), g_cgv.myCharacterInfo->name, 
		g_cgv.worldTable[g_cgv.syncCharacter[0].worldIdx].hanName);		
		str[MAX_PATH] = NULL;

		GTH_SendUserNotice(str);

		return;

	}
	else
	{
		error = MSG_ReadLong();

		switch(error) 
		{
		case ERROR_WEATHER_SYSTEM_USE_ITEM:
			{				
					g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
					g_LPACK.GetMessage(LPACK_TYPE_NORMAL, 395));
			}
			break;			
			
		case ERROR_WEATHER_SYSTEM_USE_ITEM2:
			{
				g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
					g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 248));
			}
			break;
		}

		return;
	}

error:
	
	g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
		g_LPACK.GetMessage(LPACK_TYPE_NORMAL, 395));
}









void CWeatherSystem::
GTH_SendMessage_Request_WeatherSetting
( TIMESTATE weather, float limittime, bool IsUseItem , int invenpos )
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, WEATHER_SYSTEM );
		MSG_WriteShort(&netMessage, CC_WEATHER_SYSTEM );		
		MSG_WriteByte(&netMessage,	CC_WEATHER_SYSTEM_REQUEST_USE_WEATHER_ITEM);		
		MSG_WriteByte(&netMessage,	weather);				
		MSG_WriteLong(&netMessage,	limittime);						
		MSG_WriteByte(&netMessage,	IsUseItem);								

		if ( IsUseItem )
		 MSG_WriteByte(&netMessage,  invenpos);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	
}











void CWeatherSystem::GTH_ProcessMessage_WeatherSystemEvent(void)
{
	int state		= MSG_ReadLong();
	

	if ( state == (int)WEATHER_SYSTEM_WORLD_PROGRESS)
	{
		TIMESTATE  WeatherType;
		WeatherType = static_cast <TIMESTATE>(MSG_ReadLong());

		g_timeSystem->SetTimeStateEnv (WeatherType);	
	}
	else
	{
		g_timeSystem->SetTimeStateEnv (TIME_DAY);	
	}	
}








void CWeatherSystem::GTH_SendMessageEventSeting(int event)
{
	if ( event < 0)	
		event = 0;			

	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, WEATHER_SYSTEM );				
		MSG_WriteShort(&netMessage, CC_WEATHER_SYSTEM );
		MSG_WriteByte(&netMessage, CC_WEATHER_SYSTEM_BROAD_CASTING_EVENT_SETING);				
		MSG_WriteByte(&netMessage, event);					
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	if ( event == 0)	
	{
		g_ifMng->AddChatMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 251), GTHCOLOR_SYSMESSAGE, g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322));			
	}
	else
	{
		g_ifMng->AddChatMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 252), GTHCOLOR_SYSMESSAGE, g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322));			
	}

	
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_WARNING_2 ), GTH_SOUND_PLAY_ONCE );

}








void CWeatherSystem::GTH_SendMessage_Request_WorldState(int worldidx)
{	
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, WEATHER_SYSTEM );		
		MSG_WriteShort(&netMessage, CC_WEATHER_SYSTEM );
		MSG_WriteByte(&netMessage, CC_WEATHER_SYSTEM_REQUEST_WORLD_STATE);		
		MSG_WriteByte(&netMessage, worldidx);					
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}








void CWeatherSystem::GTH_ProcessMessage_Request_WorldState()
{
	int reply = MSG_ReadByte();
	TIMESTATE state = TIME_DAY;

	if ( reply == 1)
	{
		state = static_cast<TIMESTATE>(MSG_ReadLong());		
		g_timeSystem->SetTimeStateEnv (state);

	}
	
	else
	{
		
		g_timeSystem->SetTimeStateEnv (TIME_DAY);
	}
}






void CWeatherSystem::GTH_SendUserNotice(char *notice)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, WEATHER_SYSTEM );		
		MSG_WriteShort(&netMessage, CC_WEATHER_SYSTEM );		
		MSG_WriteByte(&netMessage, CC_WEATHER_SYSTEM_NOTICE);		
		MSG_WriteString(&netMessage, notice);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);		
}
