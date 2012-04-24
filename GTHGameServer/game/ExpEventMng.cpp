




#include "../Global.h"
#include "../new.h"










void CExpEventMng::ProcessExpEvent()
{
	short Protocal = static_cast<enum enumProtocal>(MSG_ReadShort());

	switch(Protocal)
	{
	case CC_BROAD_CAST_SET_EVENT:
	case SS_BROAD_CAST_SET_EVENT:
		{	
			enumEventType eventType  = static_cast<enum enumEventType>(MSG_ReadLong());	
			float fValue	= MSG_ReadFloat();

			if ( Protocal == CC_BROAD_CAST_SET_EVENT)
			{
				if ( g_curPC != NULL)
				{
					g_logSystem->Write("[EXP_EVENT] USE CHEAT, NAME = %s, CHEAT_EVENT=%d, VALUE=%f", 
					g_curPC->name, (int)eventType, fValue);
				}
			}			

			ProcessMessage_BroadCast_SetEvent(eventType, fValue);
		}
		break;

	case SS_SET_EVENT:
		{		
			enumEventType eventType  = static_cast<enum enumEventType>(MSG_ReadLong());	
			float fValue			 = MSG_ReadFloat();
			SetEvent(eventType, fValue);
		}
		break;

	case SS_REQUEST_EVENT:
		ProcessMessage_RequestAllEvent();
		break;	

	case SS_REPLY_EVENT:
		ProcessMessage_ReplyAllEvent();
		break;

	case CC_SET_ACTIVE_SYSTEM:
	case SS_SET_ACTIVE_SYSTEM:
		ProcessMessage_RequestActiveSystem();
		break;		

	case CC_REQUEST_EVENT_STATUS:
		if ( g_curPC != NULL)			
		{
			if (g_curPC->ready == TRUE)
				SendMessage_ReplyEventStatus(g_curPC);		
		}
		break;
	}
}








void CExpEventMng::ProcessMessage_BroadCast_SetEvent(enum enumEventType eventType, float fValue)
{
	
	if ( g_config.isManager )
	{				
		
		if ( m_ActiveMng == TRUE)
		{		
			
			SetEvent(eventType, fValue);
			SendMessage_SetEventTo_MemberServer(eventType, fValue);
		}
	}
	else
	{
		
		SendMessage_SetEventTo_ManagerServer(eventType, fValue);	
	}
	
}






void CExpEventMng::SendMessage_SetEventTo_ManagerServer(enumEventType eventType, float value)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, EXP_EVENT);
		MSG_WriteShort(&netMessage, SS_BROAD_CAST_SET_EVENT);
		MSG_WriteLong(&netMessage,  eventType);
		MSG_WriteFloat(&netMessage,  value);		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
		
	}
	MSG_EndWriting(&netMessage);	
	
}







void CExpEventMng::SendMessage_SetEventTo_MemberServer(enumEventType eventType, float value)
{	
	SetEvent(eventType, value);
		
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, EXP_EVENT);
		MSG_WriteShort(&netMessage, SS_SET_EVENT);
		MSG_WriteLong(&netMessage,  eventType);
		MSG_WriteFloat(&netMessage,  value);
		
		for (int i=1; i < MAX_MEMBER_SERVER; i++) 
		{
			if ( !g_memberServer[i].active )	
				continue;

			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}








void CExpEventMng::SendMessage_RequestEventToManagerServer()
{		
	
	
	
	if ( g_config.isManager == TRUE)
		return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, EXP_EVENT);
		MSG_WriteShort(&netMessage, SS_REQUEST_EVENT);
		MSG_WriteLong(&netMessage, g_config.gameServerNo);		
		NET_SendUnreliableMessage(&localSocket, &netMessage);		
	}
	MSG_EndWriting(&netMessage);
}






void CExpEventMng::SendMessage_ReplyEventToServer(int MemberServerIdx, BOOL Active)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, EXP_EVENT);
		MSG_WriteShort(&netMessage, SS_REPLY_EVENT);		
		MSG_WriteLong(&netMessage,  Active);				

		if ( Active == TRUE)
		{		
			MSG_WriteFloat(&netMessage, m_fExpRate);
			MSG_WriteFloat(&netMessage, m_fItemDropRate);
			MSG_WriteFloat(&netMessage, m_fNakDropRate);		
			MSG_WriteFloat(&netMessage, m_fGencapability_rate);	
		}

		NET_SendUnreliableMessage(&g_memberServer[MemberServerIdx].sock, &netMessage);		
		
	}
	MSG_EndWriting(&netMessage);	
}








void CExpEventMng::SetEvent(enumEventType eventType, float fValue)
{		
	switch(eventType)
	{
	case EXP_RATE:
		m_fExpRate				= fValue;
		g_logic.expRate			= fValue;
		break;

	case ITEM_DROP_RATE:
		m_fItemDropRate			= fValue;
		g_logic.itemDropRate	= fValue;
		break;

	case NAK_DROP_RATE:
		m_fNakDropRate			= fValue;
		g_logic.nseRate			= fValue;	
		break;

	case GENCAPABILITY_RATE:
		m_fGencapability_rate	= fValue;
		g_logic.genCapabilityRate= fValue;
		break;

	case ALL_READ_LOGIC:
		GTH_ProcessMessage_LogicReload();
		
		m_fExpRate				= g_logic.expRate;
		m_fItemDropRate			= g_logic.itemDropRate;
		m_fNakDropRate			= g_logic.nseRate;	
		m_fGencapability_rate	= g_logic.genCapabilityRate;

		g_logSystem->Write("[EXP_EVENT], EVENT_END, [EXP_RATE] = %f," 
			"[ItemDrop_RATE] = %f, [NakDrop] =%f, [GenCapability] = %f", 
			g_logic.expRate, g_logic.itemDropRate, g_logic.nseRate, m_fGencapability_rate);

		break;
		
	}

}






void CExpEventMng::InitExpEventMng()
{
	m_fExpRate				= g_logic.expRate;
	m_fItemDropRate			= g_logic.itemDropRate;
	m_fNakDropRate			= g_logic.nseRate;	
	m_fGencapability_rate	= g_logic.genCapabilityRate;

	m_ActiveMng				= FALSE;
}

CExpEventMng::CExpEventMng()
{
}

CExpEventMng::~CExpEventMng()
{

}







void CExpEventMng::ProcessMessage_RequestAllEvent()
{
	int MemberServerIdx = MSG_ReadLong();

	if ( MemberServerIdx > MAX_MEMBER_SERVER || MemberServerIdx < 0)		
		return;
	
	if ( g_memberServer[MemberServerIdx].active == FALSE )	
		return;	

	SendMessage_ReplyEventToServer(MemberServerIdx, m_ActiveMng);
}







void CExpEventMng::ProcessMessage_ReplyAllEvent()
{		
	int Reply  = 0;
	Reply  = MSG_ReadLong();

	if ( Reply == 0)
	{
		g_logSystem->Write("[EXP_EVENT] Can't EventSeting Because EventSystem Is inactive ");
		return;
	}

	m_fExpRate				= MSG_ReadFloat();
	g_logic.expRate			= m_fExpRate;

	m_fItemDropRate			= MSG_ReadFloat();
	g_logic.itemDropRate	= m_fItemDropRate;

	m_fNakDropRate			= MSG_ReadFloat();
	g_logic.nseRate			= m_fNakDropRate;	

	m_fGencapability_rate	= MSG_ReadFloat();
	g_logic.genCapabilityRate = m_fGencapability_rate;

	g_logSystem->Write("[EXP_EVENT], Restart and EventSeting, [EXP_RATE] = %f," 
		"[ItemDrop_RATE] = %f, [NakDrop] =%f, [GenCapability] = %f", 
		g_logic.expRate, g_logic.itemDropRate, g_logic.nseRate, m_fGencapability_rate);
}






void CExpEventMng::ProcessMessage_RequestActiveSystem()
{	
	m_ActiveMng = MSG_ReadLong();
	
	if (g_config.isManager == TRUE)	
	{
		
		if ( m_ActiveMng == FALSE)		
			SendMessage_SetEventTo_MemberServer(ALL_READ_LOGIC, 1.0);
	}
	else	
		
		SendMessage_SetActiveToManagerServer(m_ActiveMng);	
}







void CExpEventMng::SendMessage_SetActiveToManagerServer(BOOL Active)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, EXP_EVENT);
		MSG_WriteShort(&netMessage, SS_SET_ACTIVE_SYSTEM);
		MSG_WriteLong(&netMessage,	Active);		
		NET_SendUnreliableMessage(&localSocket, &netMessage);		
	}
	MSG_EndWriting(&netMessage);
}







void CExpEventMng::SendMessage_ReplyEventStatus(playerCharacter_t* pc)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, EXP_EVENT);
		MSG_WriteShort(&netMessage, GSC_REPLY_EVENT_STATUS);						
		MSG_WriteFloat(&netMessage, m_fExpRate);
		MSG_WriteFloat(&netMessage, m_fItemDropRate);
		MSG_WriteFloat(&netMessage, m_fNakDropRate);		
		MSG_WriteFloat(&netMessage, m_fGencapability_rate);			
		NET_SendUnreliableMessage(&pc->sock, &netMessage);				
	}
	MSG_EndWriting(&netMessage);	
}

