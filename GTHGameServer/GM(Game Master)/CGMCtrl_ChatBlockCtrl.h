



#if !defined(AFX_CGMCTRL_CHATBLOCKCTRL_H__E9E8F063_E4AB_4B17_859A_FDDC362B9F95__INCLUDED_)
#define AFX_CGMCTRL_CHATBLOCKCTRL_H__E9E8F063_E4AB_4B17_859A_FDDC362B9F95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "tagGMPacket.h"
class CGMCtrl_ChatBlockCtrl  
{
	private:
		BOOL m_bOK;
	public:
		BOOL isOK(void) const;

	
	public:
		void Decoder(playerCharacter_t* pPlayer);
		void Decoder_cs_request_chat_block(playerCharacter_t* pPlayer);
		void Decoder_cs_request_chat_allow(playerCharacter_t* pPlayer);
		void Decoder_cs_chat_request_reamin_block_time(playerCharacter_t* pPlayer);

		void ProcessPerSecond(playerCharacter_t* pPlayer);

	
	public:
		void TransmitePacket_sc_chat_response_reamin_blick_time(playerCharacter_t* pToPlayer);
		void TransmitePacket_ChatBlock(playerCharacter_t* pPlayer);
		void TransmitePacket_sc_response_chat_block(
				playerCharacter_t* pToPlayer,
				const char* szpApplyPlayerName,
				const int   Time,
				const enum tagGMPacket_ChatBan_sc_response_chat_block::enumCode code) const;

		void TransmitePacket_sc_response_chat_allow(
				playerCharacter_t* pToPlayer,	
				enum tagGMPacket_ChatBan_cs_response_chat_allow::enumCode code,
				const char* szpApplyPlayerName) const;

		void TransmitePacket_ChatAllow(
				playerCharacter_t* pPlayer,	
				const enum tagGMPacket_ChatBan_sc_chat_allow::enumCode code) const;



	
	public:
		CGMCtrl_ChatBlockCtrl();
		virtual ~CGMCtrl_ChatBlockCtrl();

};

#endif 
