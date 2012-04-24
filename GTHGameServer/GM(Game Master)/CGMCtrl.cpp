



#include "..\global.h"
#include "CGMCtrl.h"


#include "..\CTools.h"
extern CTools* gcpTools;









CGMCtrl::CGMCtrl()
{
	m_bOK=FALSE;
	
	m_cpPKTimeCtrl = NULL;
	m_cpChatBlockCtrl = NULL;
	m_cpServerStateDataCtrl = NULL;


	if(FALSE == Init()) return;

	m_bOK=TRUE;

}







CGMCtrl::~CGMCtrl()
{
	if(NULL != m_cpPKTimeCtrl){ delete m_cpPKTimeCtrl; m_cpPKTimeCtrl=NULL; }
	if(NULL != m_cpChatBlockCtrl){ delete m_cpChatBlockCtrl; m_cpChatBlockCtrl=NULL; }
	if(NULL != m_cpServerStateDataCtrl){ delete m_cpServerStateDataCtrl; m_cpServerStateDataCtrl=NULL; }

}






BOOL CGMCtrl::isOK(void) const
{
	return m_bOK;
}




BOOL CGMCtrl::Init(void)
{
	m_cpServerStateDataCtrl = new CGMCtrl_ServerStateDataCtrl();
	if(NULL == m_cpServerStateDataCtrl) return FALSE;
	if(FALSE == m_cpServerStateDataCtrl->isOK()){ delete m_cpServerStateDataCtrl; m_cpServerStateDataCtrl=NULL; return FALSE; }

	m_cpPKTimeCtrl =  new CGMCtrl_PKTimeCtrl(m_cpServerStateDataCtrl);
	if(NULL == m_cpPKTimeCtrl) return FALSE;
	if(FALSE == m_cpPKTimeCtrl->isOK()){ delete m_cpPKTimeCtrl; m_cpPKTimeCtrl=NULL; return FALSE; }

	m_cpChatBlockCtrl = new CGMCtrl_ChatBlockCtrl();
	if(NULL == m_cpChatBlockCtrl) return FALSE;
	if(FALSE == m_cpChatBlockCtrl->isOK()){ delete m_cpChatBlockCtrl; m_cpChatBlockCtrl=NULL; return FALSE; }

	return TRUE;
}



CGMCtrl_ServerStateDataCtrl* CGMCtrl::GetServerStateDataCtrlPointer(void) const
{
	return m_cpServerStateDataCtrl;
}




void CGMCtrl::TransmitePacket_NProtect_sc_response_AUTH(playerCharacter_t* pPlayer) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_NProtect);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagNProtect::sc_response_AUTH);
		MSG_WriteLong(&netMessage,  m_cpServerStateDataCtrl->isNProtectAUTH());
		NET_SendMessage( &(pPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);
}





inline void CGMCtrl::TransmitePacket_NProtect_gsms_request_change(const BOOL in_bEnable) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_NProtect);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagNProtect::gsms_request_change);
		MSG_WriteLong(&netMessage,in_bEnable);
		NET_SendUnreliableMessage(&localSocket, &netMessage);		
	}
	MSG_EndWriting(&netMessage);

}



inline void CGMCtrl::TransmitePacket_NProtect_msgs_notify_change(const BOOL in_bEnable) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_NProtect);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagNProtect::msgs_notify_change);
		MSG_WriteLong(&netMessage,in_bEnable);

		for(int i=1; i < MAX_MEMBER_SERVER; i++){ 
			if ( !g_memberServer[i].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);

}





inline void CGMCtrl::Decoder_NProtect_gsms_request_change(void)
{
	BOOL bEnable = MSG_ReadLong();
	
	if(TRUE == bEnable) m_cpServerStateDataCtrl->SetNProtectAUTH(TRUE);
	else m_cpServerStateDataCtrl->SetNProtectAUTH(FALSE);
	
	m_cpServerStateDataCtrl->SaveODBC();

	TransmitePacket_NProtect_msgs_notify_change(m_cpServerStateDataCtrl->isNProtectAUTH());
}






inline void CGMCtrl::Decoder_NProtect_cs_request_AUTH(playerCharacter_t* pPlayer)
{
	BOOL bEnable = MSG_ReadLong();

	if(g_config.isManager){
		if(NULL == pPlayer) return;
		if(0 == GTH_CheckValidation_GameMaster(pPlayer)) return;
		
		if(TRUE == bEnable) m_cpServerStateDataCtrl->SetNProtectAUTH(TRUE);
		else m_cpServerStateDataCtrl->SetNProtectAUTH(FALSE);
		m_cpServerStateDataCtrl->SaveODBC();

		TransmitePacket_NProtect_msgs_notify_change(m_cpServerStateDataCtrl->isNProtectAUTH());
	TransmitePacket_NProtect_sc_response_AUTH(pPlayer);
	}else{		
		TransmitePacket_NProtect_gsms_request_change(bEnable);		
	}	
}




void CGMCtrl::TransmitePacket_NProtect_sc_response_AUTH_dll_change(playerCharacter_t* in_pPlayer,const BOOL in_bSuccess,const char* in_strDLLname)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_NProtect);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagNProtect::sc_response_AUTH_dll_change);
		MSG_WriteLong(&netMessage,in_bSuccess);
		MSG_WriteString(&netMessage,(char*)in_strDLLname);
		NET_SendMessage( &(in_pPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);
}






inline void CGMCtrl::Decoder_NProtect_cs_request_AUTH_dll_change(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer) return;
	if(0 == GTH_CheckValidation_GameMaster(pPlayer)) return;

	char strDLLname[_MAX_PATH];
	strncpy(strDLLname,MSG_ReadString(),_MAX_PATH);






}




void CGMCtrl::TransmitePacket_NProtect_sc_response_AUTHstate(playerCharacter_t* in_pPlayer) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_NProtect);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagNProtect::sc_response_AUTHstate);
		MSG_WriteLong(&netMessage,  m_cpServerStateDataCtrl->isNProtectAUTH());
		NET_SendMessage( &(in_pPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);

}




inline void CGMCtrl::Decoder_NProtect_cs_request_AUTHstate(playerCharacter_t* in_pPlayer)
{
	TransmitePacket_NProtect_sc_response_AUTHstate(in_pPlayer);
}





inline void CGMCtrl::Decoder_NProtect_msgs_notify_change(void)
{
	BOOL bEnable = MSG_ReadLong();
	m_cpServerStateDataCtrl->SetNProtectAUTH(bEnable);
}






void CGMCtrl::Decoder_NProtect(playerCharacter_t* pPlayer)
{
	switch(MSG_ReadShort()){
		case tagExtendSecondPacket_GMsystem::tagNProtect::gsms_request_change:
			Decoder_NProtect_gsms_request_change();
			return;

		case tagExtendSecondPacket_GMsystem::tagNProtect::msgs_notify_change:
			Decoder_NProtect_msgs_notify_change();
			return;


		case tagExtendSecondPacket_GMsystem::tagNProtect::cs_request_AUTHstate:
			Decoder_NProtect_cs_request_AUTHstate(pPlayer);
			return;

		case tagExtendSecondPacket_GMsystem::tagNProtect::cs_request_AUTH:		
			Decoder_NProtect_cs_request_AUTH(pPlayer);
			return;

		case tagExtendSecondPacket_GMsystem::tagNProtect::cs_request_AUTH_dll_change:
			Decoder_NProtect_cs_request_AUTH_dll_change(pPlayer);
			return;
	}

}







void CGMCtrl::Decoder(playerCharacter_t* pPlayer)
{
	
	
	


	switch(MSG_ReadShort()){
		case tagExtendSecondPacket_GMsystem::GMsystem_NProtect:
			
			
			Decoder_NProtect(pPlayer);
			return;

		case tagExtendSecondPacket_GMsystem::GMsystem_ChatBan:
			if(NULL == pPlayer) return;
			if(!pPlayer->ready) return;

			m_cpChatBlockCtrl->Decoder(pPlayer);
			return;
		case tagExtendSecondPacket_GMsystem::GMsystem_PKtime:
			m_cpPKTimeCtrl->Decoder(pPlayer);
			return;
	}
}





void CGMCtrl::ProcessPerSecond(playerCharacter_t* pPlayer)
{
	m_cpChatBlockCtrl->ProcessPerSecond(pPlayer);

}



BOOL CGMCtrl::isPK(void) const
{
	return m_cpServerStateDataCtrl->isPK();
}




BOOL CGMCtrl::isNProtectAUTH(void) const
{
	return m_cpServerStateDataCtrl->isNProtectAUTH();
}





