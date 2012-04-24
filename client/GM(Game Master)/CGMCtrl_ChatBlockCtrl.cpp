



#include "..\global_def.h"
#include "CGMCtrl_ChatBlockCtrl.h"


#include "tagGMPacket.h"




CGMCtrl_ChatBlockCtrl::CGMCtrl_ChatBlockCtrl()
{
	m_bOK=FALSE;
	m_bOK=TRUE;

}





CGMCtrl_ChatBlockCtrl::~CGMCtrl_ChatBlockCtrl()
{

}





BOOL CGMCtrl_ChatBlockCtrl::isOK(void) const
{
	return m_bOK; 
}






char* CGMCtrl_ChatBlockCtrl::GetHourMinSecondString(const int Time) const
{
	static char str[100];
	char   temp[100];

	int Hour   = Time / (60 * 60);
	int Min    = (Time - (Hour * 60 * 60)) / 60;
	int Second = (Time - (Hour * 60 * 60) - (Min * 60));


	str[0]=NULL;

	if(0 != Hour){ wsprintf(temp," %d%s",Hour,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,482)); strcat(str,temp); }
	if(0 != Min){ wsprintf(temp," %d%s",Min,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,483)); strcat(str,temp); }
	wsprintf(temp," %d%s",Second,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,484)); strcat(str,temp);

	return str;
}








void CGMCtrl_ChatBlockCtrl::Decoder_ChatBan_sc_response_chat_block(void)
{
	
	enum tagGMPacket_ChatBan_sc_response_chat_block::enumCode code=
		(enum tagGMPacket_ChatBan_sc_response_chat_block::enumCode)MSG_ReadLong();
	char szApplyPlayerName[NAMESTRING+1];
	strncpy(szApplyPlayerName,MSG_ReadString(),NAMESTRING);
	szApplyPlayerName[NAMESTRING-1]=NULL;
	int Time = MSG_ReadLong();

	char buffer[2048]="";

	switch(code){
		case tagGMPacket_ChatBan_sc_response_chat_block::code_success:
			
			wsprintf(buffer,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,474),szApplyPlayerName,GetHourMinSecondString(Time));
			
			g_ifMng->AddChatMessage(buffer,GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
			return;

		case tagGMPacket_ChatBan_sc_response_chat_block::code_not_find_player:
			wsprintf(buffer,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,475),szApplyPlayerName);
			
			g_ifMng->AddChatMessage(buffer,GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
			return;
	}	
}




void CGMCtrl_ChatBlockCtrl::Decoder_ChatBan_sc_chat_block(void)
{
	char buffer[2048]="";
	int  time= MSG_ReadLong();

	
	wsprintf(buffer,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,476),GetHourMinSecondString(time));		
	g_ifMng->AddChatMessage(buffer,GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
	g_cgv.GM.bChatAllow=FALSE;
}




void CGMCtrl_ChatBlockCtrl::Decoder_ChatBan_sc_chat_allow(void)
{

	
	enum tagGMPacket_ChatBan_sc_chat_allow::enumCode code = 
		(enum tagGMPacket_ChatBan_sc_chat_allow::enumCode)MSG_ReadLong();

	
	g_cgv.GM.bChatAllow=TRUE;

	switch(code){
		case tagGMPacket_ChatBan_sc_chat_allow::enumCode::code_by_GM:
			
			g_ifMng->AddChatMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,477),GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
			break;

		case tagGMPacket_ChatBan_sc_chat_allow::enumCode::code_by_TimeOut:
			
			g_ifMng->AddChatMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,478),GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
			break;
		default:
			
			g_ifMng->AddChatMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,479),GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
			break;
	}		
	
}




void CGMCtrl_ChatBlockCtrl::Decoder_ChatBan_sc_response_chat_allow(void)
{
	
	enum tagGMPacket_ChatBan_cs_response_chat_allow::enumCode code=
		(enum tagGMPacket_ChatBan_cs_response_chat_allow::enumCode)MSG_ReadLong();
	char ApplyPlayerName[NAMESTRING+1]="";
	strncpy(ApplyPlayerName,MSG_ReadString(),NAMESTRING);
	ApplyPlayerName[NAMESTRING-1]=NULL;

	char buffer[2048]="";
	switch(code){
		case tagGMPacket_ChatBan_cs_response_chat_allow::code_success:			
			
			wsprintf(buffer,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,480),ApplyPlayerName);
			g_ifMng->AddChatMessage(buffer,GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
			
			
			break;

		case tagGMPacket_ChatBan_cs_response_chat_allow::code_not_find_player:
			
			wsprintf(buffer,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,475),ApplyPlayerName);
			g_ifMng->AddChatMessage(buffer,GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
			break;

		default:
			
			g_ifMng->AddChatMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,481),GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );
			break;
	}

}






void CGMCtrl_ChatBlockCtrl::Decoder_ChatBan_sc_chat_response_remain_block_time(void) const
{
	int Time=MSG_ReadLong();


	
	char buffer[2048]="";
	wsprintf(buffer,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,485),GetHourMinSecondString(Time));
	g_ifMng->AddChatMessage(buffer,GTHCOLOR_SYSMESSAGE,"[GTH]",IF_CHATMSG_TYPE_SYSTEM );

}



void CGMCtrl_ChatBlockCtrl::Decoder_ChatBan(void)
{
	switch(MSG_ReadShort()){		
		case tagExtendSecondPacket_GMsystem::tagChatBan::sc_chat_allow:
			Decoder_ChatBan_sc_chat_allow();
			break;

		case tagExtendSecondPacket_GMsystem::tagChatBan::sc_chat_block:
			Decoder_ChatBan_sc_chat_block();
			break;

		case tagExtendSecondPacket_GMsystem::tagChatBan::sc_response_chat_block:
			Decoder_ChatBan_sc_response_chat_block();
			break;

		case tagExtendSecondPacket_GMsystem::tagChatBan::sc_response_chat_allow:
			Decoder_ChatBan_sc_response_chat_allow();
			break;
		case tagExtendSecondPacket_GMsystem::tagChatBan::sc_chat_response_remain_block_time:
			Decoder_ChatBan_sc_chat_response_remain_block_time();
			break;

			
	}

}





void CGMCtrl_ChatBlockCtrl::
TransmitePacket_GMsystem_ChatBan_Req_ReaminTime(void) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_ChatBan);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagChatBan::cs_chat_request_remain_block_time);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}




void CGMCtrl_ChatBlockCtrl::TransmitePacket_GMsystem_ChatBan_Allow(const char* szpApplyPlayerName)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_ChatBan);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagChatBan::cs_request_chat_allow);
		MSG_WriteString(&netMessage, (char*)szpApplyPlayerName);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}







void CGMCtrl_ChatBlockCtrl::TransmitePacket_GMsystem_ChatBan_Block(const char* szpApplyPlayerName,const int ApplyTime)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_ChatBan);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagChatBan::cs_request_chat_block);
		MSG_WriteString(&netMessage, (char*)szpApplyPlayerName);
		MSG_WriteLong(&netMessage,ApplyTime);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}
