



#include "..\global.h"
#include "CGMCtrl_ChatBlockCtrl.h"


#include "..\CTools.h"
extern CTools* gcpTools;




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






void CGMCtrl_ChatBlockCtrl::ProcessPerSecond(playerCharacter_t* pPlayer)
{
	if(pPlayer->GM.Chat.RemainBanTimePerSecond <= 0) return;

	pPlayer->GM.Chat.RemainBanTimePerSecond--;
	if(pPlayer->GM.Chat.RemainBanTimePerSecond <= 0){	
		TransmitePacket_ChatAllow(pPlayer,tagGMPacket_ChatBan_sc_chat_allow::code_by_TimeOut);
	}
}





void CGMCtrl_ChatBlockCtrl::
TransmitePacket_ChatAllow(
	playerCharacter_t* pPlayer,	
	const enum tagGMPacket_ChatBan_sc_chat_allow::enumCode code) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_ChatBan);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagChatBan::sc_chat_allow);
		MSG_WriteLong(&netMessage,code);

		NET_SendMessage( &(pPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);
}




void CGMCtrl_ChatBlockCtrl::
TransmitePacket_ChatBlock(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_ChatBan);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagChatBan::sc_chat_block);
		MSG_WriteLong(&netMessage,pPlayer->GM.Chat.RemainBanTimePerSecond);
		NET_SendMessage( &(pPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);
}




void CGMCtrl_ChatBlockCtrl::
TransmitePacket_sc_response_chat_block(
	playerCharacter_t* pToPlayer,
	const char* szpApplyPlayerName,
	const int   Time,
	const enum tagGMPacket_ChatBan_sc_response_chat_block::enumCode code) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_ChatBan);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagChatBan::sc_response_chat_block);

		MSG_WriteLong(&netMessage, code);
		MSG_WriteString(&netMessage,(char*)szpApplyPlayerName);
		MSG_WriteLong(&netMessage,Time);	
		
		NET_SendMessage( &(pToPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);
}




void CGMCtrl_ChatBlockCtrl::TransmitePacket_sc_response_chat_allow(
	playerCharacter_t* pToPlayer,	
	enum tagGMPacket_ChatBan_cs_response_chat_allow::enumCode code,
	const char* szpApplyPlayerName) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_ChatBan);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagChatBan::sc_response_chat_allow);

		MSG_WriteLong(&netMessage, code);		
		MSG_WriteString(&netMessage,(char*)szpApplyPlayerName);
		NET_SendMessage( &(pToPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);
}




 


void CGMCtrl_ChatBlockCtrl::Decoder_cs_request_chat_allow(playerCharacter_t* pPlayer)
{

	char TargetName[NAMESTRING+1];
	strncpy(TargetName,MSG_ReadString(),NAMESTRING);
	TargetName[NAMESTRING-1]=NULL;

	playerCharacter_t* pTargetPlayer= gcpTools->GetPlayerRecordPointer(TargetName);
	if(NULL == pTargetPlayer){
		TransmitePacket_sc_response_chat_allow(
			pPlayer,	
			tagGMPacket_ChatBan_cs_response_chat_allow::code_not_find_player,
			TargetName);
		return;
	}


	pTargetPlayer->GM.Chat.RemainBanTimePerSecond = 0;
	
	TransmitePacket_ChatAllow(pTargetPlayer,tagGMPacket_ChatBan_sc_chat_allow::code_by_GM);
	TransmitePacket_sc_response_chat_allow(
			pPlayer,	
			tagGMPacket_ChatBan_cs_response_chat_allow::code_success,
			TargetName);
}





void CGMCtrl_ChatBlockCtrl::Decoder_cs_request_chat_block(playerCharacter_t* pPlayer)
{
	char TargetName[NAMESTRING+1];
	strncpy(TargetName,MSG_ReadString(),NAMESTRING);
	TargetName[NAMESTRING-1]=NULL;
	int Time= MSG_ReadLong();


	playerCharacter_t* pTargetPlayer= gcpTools->GetPlayerRecordPointer(TargetName);
	if(NULL == pTargetPlayer){

		TransmitePacket_sc_response_chat_block(
			pPlayer,
			TargetName,
			Time,
			tagGMPacket_ChatBan_sc_response_chat_block::enumCode::code_not_find_player);
		return;
	}

	pTargetPlayer->GM.Chat.RemainBanTimePerSecond = Time;
	if(pTargetPlayer->GM.Chat.RemainBanTimePerSecond < 0) pTargetPlayer->GM.Chat.RemainBanTimePerSecond=0;
	if(pTargetPlayer->GM.Chat.RemainBanTimePerSecond > playerCharacter_t::tagGM::tagChat::max_ban_time) pTargetPlayer->GM.Chat.RemainBanTimePerSecond = playerCharacter_t::tagGM::tagChat::max_ban_time;

	TransmitePacket_ChatBlock(pTargetPlayer);
	TransmitePacket_sc_response_chat_block(
		pPlayer,
		pTargetPlayer->name,
		pTargetPlayer->GM.Chat.RemainBanTimePerSecond,
		tagGMPacket_ChatBan_sc_response_chat_block::enumCode::code_success);
}





void CGMCtrl_ChatBlockCtrl::Decoder_cs_chat_request_reamin_block_time(playerCharacter_t* pPlayer)
{
	TransmitePacket_sc_chat_response_reamin_blick_time(pPlayer);
}



void CGMCtrl_ChatBlockCtrl::TransmitePacket_sc_chat_response_reamin_blick_time(playerCharacter_t* pToPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_ChatBan);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagChatBan::sc_chat_response_reamin_blick_time);
		MSG_WriteLong(&netMessage, pToPlayer->GM.Chat.RemainBanTimePerSecond);
		NET_SendMessage( &(pToPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);
}
	





void CGMCtrl_ChatBlockCtrl::Decoder(playerCharacter_t* pPlayer)
{
	switch(MSG_ReadShort()){
		case tagExtendSecondPacket_GMsystem::tagChatBan::cs_request_chat_block:
			Decoder_cs_request_chat_block(pPlayer);
			break;

		case tagExtendSecondPacket_GMsystem::tagChatBan::cs_request_chat_allow:
			Decoder_cs_request_chat_allow(pPlayer);
			break;

		case tagExtendSecondPacket_GMsystem::tagChatBan::cs_chat_request_reamin_block_time:
			Decoder_cs_chat_request_reamin_block_time(pPlayer);
			break;
	}



}