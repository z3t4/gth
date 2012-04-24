



#include "..\global_def.h"
#include "CGMCtrl.h"
#include "tagGMPacket.h"



CGMCtrl::CGMCtrl()
{
	m_bOK=FALSE;
	m_cpPKTimeCtrl = NULL;
	m_cpChatBlockCtrl = NULL;
	g_cgv.GM.bChatAllow=TRUE;

	if(FALSE == Init()) return;
	m_bOK=TRUE;

}





CGMCtrl::~CGMCtrl()
{
	if(NULL != m_cpPKTimeCtrl){ delete m_cpPKTimeCtrl; m_cpPKTimeCtrl=NULL; }
	if(NULL != m_cpChatBlockCtrl){ delete m_cpChatBlockCtrl; m_cpChatBlockCtrl=NULL; }

}





void CGMCtrl::TransmitePacket_cs_request_CSAUTH(const BOOL bEnable)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_NProtect);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagNProtect::cs_request_AUTH);
		MSG_WriteLong(&netMessage,bEnable);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



void CGMCtrl::TransmitePacket_cs_request_AUTHstate(void)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_NProtect);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagNProtect::cs_request_AUTHstate);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}




void CGMCtrl::TransmitePacket_cs_request_AUTH_dll_change(const char* in_strDLLFileName)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_NProtect);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagNProtect::cs_request_AUTH_dll_change);
		MSG_WriteString(&netMessage,(char*)in_strDLLFileName);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}






BOOL CGMCtrl::RunGMChatCommand( char message[], char command[], chatMessage param[] )
{

#ifdef _DEBUG
	
	if(0 == stricmp(command,"NProtect")){
		if(0 == stricmp(param[0],"changeDLL")){
			if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return TRUE;

			if(NULL == strrchr(param[1],'.dll')) return FALSE;
			TransmitePacket_cs_request_AUTH_dll_change(param[1]);
			return TRUE;
		}

		if(0 == stricmp(param[0],"AUTH")){
			if(0 == stricmp(param[1],"enable")){
				if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return TRUE;

				TransmitePacket_cs_request_CSAUTH(TRUE);
				return TRUE;
			}

			if(0 == stricmp(param[1],"disable")){
				if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return TRUE;

				TransmitePacket_cs_request_CSAUTH(FALSE);
				return TRUE;
			}

			if(0 == stricmp(param[1],"check")){
				if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return TRUE;

				TransmitePacket_cs_request_AUTHstate();
				return TRUE;
			}

		}
	}
#endif


	
	if(0 == stricmp(command,"pktime")){
		if(0 == stricmp(param[0],"start")){

			if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return TRUE;

			m_cpPKTimeCtrl->TransmitePacket_cs_request_change(TRUE);
			return TRUE;
		}

		if(0 == stricmp(param[0],"end")){
			if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return TRUE;

			m_cpPKTimeCtrl->TransmitePacket_cs_request_change(FALSE);
			return TRUE;
		}

		if(0 == stricmp(param[0],"check")){
			m_cpPKTimeCtrl->TransmitePacket_cs_request_status();
			return TRUE;
		}
	}



	
	if(0 == stricmp( command,"chat")){

		if(0 == stricmp(param[0],"block")){
			if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return TRUE;

			m_cpChatBlockCtrl->TransmitePacket_GMsystem_ChatBan_Block(param[1],atoi(param[2]));			
			return TRUE;
		}

		if(0 == stricmp(param[0],"allow")){
			if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return TRUE;

			m_cpChatBlockCtrl->TransmitePacket_GMsystem_ChatBan_Allow(param[1]);
			return TRUE;
		}

		if(0 == stricmp(param[0],"check")){
			m_cpChatBlockCtrl->TransmitePacket_GMsystem_ChatBan_Req_ReaminTime();
			return TRUE;
		}
	}

	return FALSE;

}




BOOL CGMCtrl::isOK(void) const
{
	return m_bOK;
}




BOOL CGMCtrl::Init(void)
{

	m_cpPKTimeCtrl = new CGMCtrl_PKTimeCtrl();
	if(NULL == m_cpPKTimeCtrl) return FALSE;
	if(FALSE == m_cpPKTimeCtrl->isOK()){ delete m_cpPKTimeCtrl; m_cpPKTimeCtrl=NULL; return FALSE; }

	m_cpChatBlockCtrl = new CGMCtrl_ChatBlockCtrl();
	if(NULL == m_cpChatBlockCtrl) return FALSE;
	if(FALSE == m_cpChatBlockCtrl->isOK()){ delete m_cpChatBlockCtrl; m_cpChatBlockCtrl=NULL; return FALSE; }



	return TRUE;
}





inline void CGMCtrl::Decoder_NProtect_sc_response_AUTH(void)
{
	BOOL bNprotectAUTHenable  = MSG_ReadLong();
	if(TRUE == bNprotectAUTHenable){
		g_ifMng->AddChatMessage("NProtect CSAUTH enable.",GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
	}else{
		g_ifMng->AddChatMessage("NProtect CSAUTH disable.",GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
	}

}



inline void CGMCtrl::Decoder_NProtect_sc_response_AUTH_dll_change(void)
{
	char strDLLname[_MAX_PATH]="";
	BOOL bSuccess=MSG_ReadLong();
	strncpy(strDLLname,MSG_ReadString(),_MAX_PATH);

	if(TRUE == bSuccess){
		g_ifMng->AddChatMessage("NProtect CSAUTH dll change success.",GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
		g_ifMng->AddChatMessage(strDLLname,GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
	}else{
		g_ifMng->AddChatMessage("NProtect CSAUTH dll change fail.",GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
		g_ifMng->AddChatMessage(strDLLname,GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
	}
}






inline void CGMCtrl::Decoder_NProtect_sc_response_AUTHstate(void)
{
	BOOL bEnable=MSG_ReadLong();

	if(TRUE == bEnable) g_ifMng->AddChatMessage("NProtect CSAUTH enable",GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );		
	else g_ifMng->AddChatMessage("NProtect CSAUTH disable",GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
}





void CGMCtrl::Decoder_NProtect(void)
{
	switch(MSG_ReadShort()){			
		case tagExtendSecondPacket_GMsystem::tagNProtect::sc_response_AUTHstate:
			Decoder_NProtect_sc_response_AUTHstate();
			return;


		case tagExtendSecondPacket_GMsystem::tagNProtect::sc_response_AUTH:
			Decoder_NProtect_sc_response_AUTH();
			return;

		case tagExtendSecondPacket_GMsystem::tagNProtect::sc_response_AUTH_dll_change:
			Decoder_NProtect_sc_response_AUTH_dll_change();
			return;
	}


}











void CGMCtrl::Decoder(void)
{
	switch(MSG_ReadShort()){
		case tagExtendSecondPacket_GMsystem::GMsystem_NProtect:
			Decoder_NProtect();
			return;

		case tagExtendSecondPacket_GMsystem::GMsystem_ChatBan:
			m_cpChatBlockCtrl->Decoder_ChatBan();
			return;

		case tagExtendSecondPacket_GMsystem::GMsystem_PKtime:
			m_cpPKTimeCtrl->Decoder();
			return;
	}
}

