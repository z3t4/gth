



#include "..\global_def.h"
#include "CGMCtrl_PKTimeCtrl.h"




CGMCtrl_PKTimeCtrl::CGMCtrl_PKTimeCtrl()
{
	m_bOK=FALSE;
	m_bPK=TRUE;
	m_bOK=TRUE;
}




CGMCtrl_PKTimeCtrl::~CGMCtrl_PKTimeCtrl()
{

}





BOOL CGMCtrl_PKTimeCtrl::isOK(void) const
{
	return m_bOK;
}







inline void CGMCtrl_PKTimeCtrl::Decoder_sc_notify_change(void)
{
	m_bPK = MSG_ReadLong();
	if(TRUE == m_bPK){
		
		g_ifMng->AddChatMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,486),GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );

	}else{
		
		g_ifMng->AddChatMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,487),GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
	}
}




inline void CGMCtrl_PKTimeCtrl::Decoder_sc_response_status(void)
{
	if(TRUE == MSG_ReadLong()){
		
		g_ifMng->AddChatMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,488),GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );

	}else{
		
		g_ifMng->AddChatMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,489),GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
	}
}




void CGMCtrl_PKTimeCtrl::Decoder(void)
{
	switch(MSG_ReadShort()){		
		case tagExtendSecondPacket_GMsystem::tagPKtime::sc_notify_change:
			Decoder_sc_notify_change();
			return;

		case tagExtendSecondPacket_GMsystem::tagPKtime::sc_response_status:
			Decoder_sc_response_status();
			return;
	}

}




void CGMCtrl_PKTimeCtrl::TransmitePacket_cs_request_change(const BOOL bPK)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);	
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_PKtime);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagPKtime::cs_request_change);
		MSG_WriteLong(&netMessage,  bPK);
		NET_SendMessage(&gsSocket,  &netMessage);
	}
	MSG_EndWriting( &netMessage );
}




void CGMCtrl_PKTimeCtrl::TransmitePacket_cs_request_status(void)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_PKtime);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagPKtime::cs_request_status);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

}





BOOL CGMCtrl_PKTimeCtrl::isPK(void) const
{
	return m_bPK;
}




void CGMCtrl_PKTimeCtrl::SetPK(const BOOL bPK)
{
	m_bPK=bPK;
}





