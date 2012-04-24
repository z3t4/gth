


#include "../global_def.h"

CExpRateMng::CExpRateMng()
{

}

CExpRateMng::~CExpRateMng()
{

}


void CExpRateMng::ProcessMessage()
{
	short Protocal = static_cast<enum enumProtocal>(MSG_ReadShort());

	switch(Protocal)
	{
	case GSC_REPLY_EVENT_STATUS:
		Processage_ReplyEventStatus();
		break;
	}

}






void CExpRateMng::SendMessage_SetEvent(enum enumEventType eventTYpe, float fvalue )
{
	if ( fvalue <= 0.0f)
		return;

	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, EXP_EVENT );
		MSG_WriteShort(&netMessage, CC_BROAD_CASE_SET_EVENT );						
		MSG_WriteLong(&netMessage,	eventTYpe);									
		MSG_WriteFloat(&netMessage,	fvalue);	

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}








void CExpRateMng::SendMessage_SetEventActive(BOOL active)
{		
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, EXP_EVENT );
		MSG_WriteShort(&netMessage, CC_SET_ACTIVE_SYSTEM );						
		MSG_WriteLong(&netMessage,	active);										

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void CExpRateMng::SendMessage_RequestEventStatus()
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, EXP_EVENT );
		MSG_WriteShort(&netMessage, CC_REQUEST_EVENT_STATUS );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}








void CExpRateMng::Processage_ReplyEventStatus()
{
	
	
	float Expvalue		= MSG_ReadFloat();
	float Itemdropvalue = MSG_ReadFloat();
	float NakDropvalue	= MSG_ReadFloat();
	float Gencapvalue	= MSG_ReadFloat();
	
	char msg[256];
	_snprintf(msg, 256, "[EXP]=%.1f, [ITEMDROP]=%.1f, [NAK]=%.1f, [GenCap]=%.1f",
		Expvalue, Itemdropvalue, NakDropvalue, Gencapvalue);
	msg[255]= NULL;
	
	g_ifMng->AddSysMessage( msg ,GTHCOLOR_TRADEMESSAGE);
	
}