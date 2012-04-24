




#include "../global_def.h"





CChristMasEvent::CChristMasEvent()
{	
	
	m_EventState = EVENT_INACTIVE;
}
CChristMasEvent::~CChristMasEvent()
{

}







void CChristMasEvent::GTH_Process_ChristMas_Event()
{
	int type = MSG_ReadByte();

	switch(type)
	{
		
	case GSC_CHRISTMAS_EVENT_REPLY_USE_SNOWITEM:
		GTH_ProcessMessage_Reply_ChristMas_UseItem();		
		break;	

		
	case GSC_CHRISTMAS_EVENT_REPLY_CHRISTEVENT:
		GTH_ProcessMessage_ChristMasEvent();
		break;		

	case GSC_CHRISTMAS_EVENT_REQUEST_WORLD_STATE:		
		GTH_ProcessMessage_Request_WorldState();
		break;

	default:
		return;
	}
	
}







void CChristMasEvent::GTH_ProcessMessage_Reply_ChristMas_UseItem(void)
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



		
		sprintf(str, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 247), g_cgv.myCharacterInfo->name, 
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
		case ERROR_CHRISTMAS_EVENT_USE_ITEM:
			{				
					g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
					g_LPACK.GetMessage(LPACK_TYPE_NORMAL, 395));
			}
			break;			
			
		case ERROR_CHRISTMAS_EVENT_USE_ITEM2:
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









void CChristMasEvent::GTH_SendMessage_Request_ChristMas_UseItem(int invenpos)					  
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND);
		MSG_WriteByte(&netMessage, CC_REQUEST_CHRISTMAS_EVENT);		
		MSG_WriteByte(&netMessage, CC_CHRISTMAS_EVENT_REQUEST_USE_SNOWITEM);		
		MSG_WriteByte(&netMessage, invenpos);					
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	
}











void CChristMasEvent::GTH_ProcessMessage_ChristMasEvent(void)
{
	int state		= MSG_ReadLong();

	
	m_EventState	= static_cast<enumChristMasEventState>(MSG_ReadLong());
	int worldidx	= -1;



	
	if ( state == (int)CHRISTMAS_EVENT_WORLD_PROGRESS)
	{
		g_timeSystem->SetTimeStateEnv (TIME_SNOW);			
	}
	else
	{
		g_timeSystem->SetTimeStateEnv (TIME_DAY);	
	}	
}








void CChristMasEvent::GTH_SendMessageEventSeting(int event)
{
	if ( event < 0)	
		event = 0;			

	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND);
		MSG_WriteByte(&netMessage, CC_REQUEST_CHRISTMAS_EVENT);				
		MSG_WriteByte(&netMessage, CC_CHRISTMAS_BROAD_CASTING_EVENT_SETING);				
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








void CChristMasEvent::GTH_SendMessage_Request_WorldState(int worldidx)
{	
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND);
		MSG_WriteByte(&netMessage, CC_REQUEST_CHRISTMAS_EVENT);		
		MSG_WriteByte(&netMessage, CC_CHRISTMAS_EVENT_REQUEST_WORLD_STATE);		
		MSG_WriteByte(&netMessage, worldidx);					
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}








void CChristMasEvent::GTH_ProcessMessage_Request_WorldState()
{
	int reply = MSG_ReadByte();
	int state = 0;

	if ( reply == 1)
	{
		state = MSG_ReadLong();

		if ( state == CHRISTMAS_EVENT_WORLD_PROGRESS)
		{
			g_timeSystem->SetTimeStateEnv (TIME_SNOW);		
		}
		else
		{
			g_timeSystem->SetTimeStateEnv (TIME_DAY);			
		}

		
		
		
		
		
		m_EventState = EVENT_ACTIVE;
	}
	
	else
	{
		
		g_timeSystem->SetTimeStateEnv (TIME_DAY);
	}
}






void CChristMasEvent::GTH_SendUserNotice(char *notice)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND);
		MSG_WriteByte(&netMessage, CC_REQUEST_CHRISTMAS_EVENT);		
		MSG_WriteByte(&netMessage, CC_CHRISTMAS_EVENT_NOTICE);		
		MSG_WriteString(&netMessage, notice);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);		
}
