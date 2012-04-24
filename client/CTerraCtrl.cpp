




#include "CTerraCtrl.h"
#include <Shlwapi.h>

#include "CTerraLpack.h"




CTerraCtrl::CTerraCtrl()
{
	m_bOK=FALSE;

	m_bOK=TRUE;

}






CTerraCtrl::~CTerraCtrl()
{

}





BOOL CTerraCtrl::isOK(void) const
{
	return m_bOK; 
}






BOOL CTerraCtrl::isTerraIDUse(void) const
{
	return m_Identify.Terra.bActive;
}




void CTerraCtrl::CommandlineParam(const char* pCommandLine) 
{
	char* pStPos;

	
	pStPos = strstr(pCommandLine,"id=");
	if(NULL != pStPos){
		pStPos=pStPos+strlen("id=");
		
		StrCpyN(m_Identify.id,pStPos,tagIdentify::MaxBytes_ID);

		m_Identify.id[tagIdentify::MaxBytes_ID]=NULL;
		if(NULL != strchr(m_Identify.id,' ')) *strchr(m_Identify.id,' ')=NULL;
		if(NULL != strchr(m_Identify.id,',')) *strchr(m_Identify.id,',')=NULL;		
	}

	
	pStPos = strstr(pCommandLine,"password=");
	if(NULL != pStPos){
		pStPos=pStPos+strlen("password=");
		StrCpyN(m_Identify.password,pStPos,tagIdentify::MaxBytes_Password);
		m_Identify.password[tagIdentify::MaxBytes_Password]=NULL;
		if(NULL != strchr(m_Identify.password,' ')) *strchr(m_Identify.password,' ')=NULL;
		if(NULL != strchr(m_Identify.password,',')) *strchr(m_Identify.password,',')=NULL;
		
	}
	
	
	m_Identify.Terra.bActive=FALSE;	
	pStPos = strstr(pCommandLine,"tid=");
	if(NULL != pStPos){
		pStPos=pStPos+strlen("tid=");
		StrCpyN(m_Identify.Terra.id,pStPos,tagIdentify::tagTerra::maxbytes_tid);

		m_Identify.Terra.id[tagIdentify::tagTerra::maxbytes_tid]=NULL;
		if(NULL != strchr(m_Identify.Terra.id,' ')) *strchr(m_Identify.Terra.id,' ')=NULL;
		if(NULL != strchr(m_Identify.Terra.id,',')) *strchr(m_Identify.Terra.id,',')=NULL;
		m_Identify.Terra.bActive = TRUE;
	}

	

}




void CTerraCtrl::GTH_ProcessMessage_ReplyLogin_SetID(const char* pID)
{
	StrCpyN( g_cgv.clientID,pID, IDSTRING);
	g_cgv.clientID[IDSTRING-1]=NULL;

	StrCpyN(m_Identify.id,pID,tagIdentify::MaxBytes_ID);
	m_Identify.id[tagIdentify::MaxBytes_ID]=NULL;


	


}


extern i3socket_t msSocket;



void CTerraCtrl::GTH_SendMessage_RequestLogin( void )
{

	
#ifdef _GTH_MALAYSIA_CHN_VERSION
	strcpy(g_cgv.clientID, m_Identify.id);
	strcpy(g_cgv.clientPassword, m_Identify.password);
#endif
	
	
		
	g_ifMng->m_loginWin->m_loginOkBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	if( !GTH_LockSending() ) return;

    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_LOGIN);			

		if ( TRUE == m_Identify.Terra.bActive)
		{
			MSG_WriteString(&netMessage, "1");
			MSG_WriteString(&netMessage, m_Identify.Terra.id);
		}
		else
		{

			
		#ifdef _GTH_MALAYSIA_CHN_VERSION
			MSG_WriteString(&netMessage, m_Identify.id);
			MSG_WriteString(&netMessage, m_Identify.password);
		#else
			MSG_WriteString(&netMessage, g_cgv.clientID);
			MSG_WriteString(&netMessage, g_cgv.clientPassword);
		#endif
			
		}
				
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



#include "tagTerraPacket.h"




void CTerraCtrl::GTH_ProcessMessage_ReplyLogin(void)
{
	g_ifMng->m_loginWin->m_loginOkBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	GTH_UnlockSending();
	g_ifMng->ExitMessage();
	
	enum tagTerraGCPacket_MSC_REPLYLOGIN::enumCode code =
		(enum tagTerraGCPacket_MSC_REPLYLOGIN::enumCode)MSG_ReadByte();
	

	memset( g_chatManager->m_returnStr, 0, sizeof( chatMessage ) );


	switch(code){
		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::success:
			{
				GTH_ProcessMessage_ReplyLogin_SetID(MSG_ReadString());
				
				
				switch(g_cgv.restartState){
					
					case RESTART_NONE : 
					case RESTART_SERVER : 
					case RESTART_RESERVER :
						g_ifMng->m_loginWin->ChangeLoginStep( 1 );
						break;
					case RESTART_CHARACTER :
						break;
				}
				GTH_SendMessage_RequestServerGroup();
			}
			return;

		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::mismatch_password:
			g_ifMng->SetMessage(
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,106), 
				1, 
				IF_MESSAGE_EXIT_CLIENT );
			return;


		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::not_register_user:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(0,105), 
				g_LPACK.GetMassage(0,107), 
				1, 
				IF_MESSAGE_EXIT_CLIENT );
			return;

		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::pre_login_user:
			if(!g_cgv.restartState){
				g_ifMng->SetMessage( 
					g_LPACK.GetMassage(0,105), 
					g_LPACK.GetMassage(0,108), 
					1, 
					IF_MESSAGE_EXIT_CLIENT );

			}else{
				g_ifMng->SetMessage( 
					g_LPACK.GetMassage(0,105), 
					g_LPACK.GetMassage(0,108), 
					1, 
					IF_MESSAGE_EXIT_CLIENT );
			}
			return;

		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::database:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(0,105), 
				g_LPACK.GetMassage(0,109), 
				1, 
				IF_MESSAGE_EXIT_CLIENT );
			return;
		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::block_user:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(0,105), 
				g_LPACK.GetMassage(0,453), 
				1, 
				IF_MESSAGE_EXIT_CLIENT );
			return;

		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::billing_network:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(0,105), 
				g_LPACK.GetMessage(0,940), 
				1, 
				IF_MESSAGE_EXIT_CLIENT );
			return;

		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::billing_point_not_enough:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(0,105), 
				g_LPACK.GetMessage(0,941), 
				1, 
				IF_MESSAGE_EXIT_CLIENT );
			return;

		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::billing_error:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(0,105), 
				g_LPACK.GetMessage(0,942), 
				1, 
				IF_MESSAGE_EXIT_CLIENT );
			return;

		
		case tagTerraGCPacket_MSC_REPLYLOGIN::enumCode::unknown:
		default:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(0,105), 
				g_LPACK.GetMassage(0,110), 
				1, 
				IF_MESSAGE_EXIT_CLIENT);
			return;
	}
	
}





inline void CTerraCtrl::Decoder_Open_MessageBox(void)
{
	int LPACKTyep = MSG_ReadByte();
	int LPACKIdx  = MSG_ReadByte();
	enum tagTerraGCPacket_OpenMessageBox::enumAfter after = 
		(enum tagTerraGCPacket_OpenMessageBox::enumAfter)MSG_ReadByte();

	switch(after){
		case tagTerraGCPacket_OpenMessageBox::enumAfter::after_none:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_TERRA,LPACKTyep,0),
				g_LPACK.GetMassage(LPACK_TYPE_TERRA,LPACKTyep,LPACKIdx),
				MSG_BUTTONTYPE_OK,
				IF_MESSAGE_NONE);
			break;


		case tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_TERRA,LPACKTyep,0),
				g_LPACK.GetMassage(LPACK_TYPE_TERRA,LPACKTyep,LPACKIdx),
				MSG_BUTTONTYPE_OK,
				IF_MESSAGE_EXIT_CLIENT);			
			break;
	}


}




inline void CTerraCtrl::Decoder_NoticeMessage(void)
{
	int LPACKTyep = MSG_ReadByte();
	int LPACKIdx  = MSG_ReadByte();

	
	
	const int maxbytes=1024;
	char message[maxbytes+1];
	strncpy(message,g_LPACK.GetMassage(LPACK_TYPE_TERRA,LPACKTyep,LPACKIdx),maxbytes);
	message[maxbytes]=NULL;	

	g_ifMng->AddSysMessage(message,GTHCOLOR_NOTICEMESSAGE );
	g_ifMng->AddNoticeMessage(message,GTHCOLOR_NOTICEMESSAGE );	
	
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_WARNING_2 ), GTH_SOUND_PLAY_ONCE );
}




inline void CTerraCtrl::
Decoder_Remain_Minute_NoticeMessage(void)
{
	int minute = MSG_ReadShort();


	const int maxbytes=1024;
	char message[maxbytes+1];
	wsprintf(message,g_LPACK.GetMassage(LPACK_TYPE_TERRA,CTerraLpack::type_Billing,8),minute);	
	message[maxbytes]=NULL;	

	g_ifMng->AddSysMessage(message,GTHCOLOR_NOTICEMESSAGE );
	g_ifMng->AddNoticeMessage(message,GTHCOLOR_NOTICEMESSAGE );	
	
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_WARNING_2 ), GTH_SOUND_PLAY_ONCE );

}





void CTerraCtrl::Decoder(void)
{
	switch(MSG_ReadByte()){
		case tagTerraGCPacket::Remain_Minute_NoticeMessage:
			Decoder_Remain_Minute_NoticeMessage();
			break;

		case tagTerraGCPacket::NoticeMessage:
			Decoder_NoticeMessage();
			break;

		case tagTerraGCPacket::Open_MessageBox:
			Decoder_Open_MessageBox();
			break;
	}
}



