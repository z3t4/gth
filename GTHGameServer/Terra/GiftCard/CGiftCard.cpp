




#include "../../global.h"
#include "CGiftCard.h"
#include "../../Tools/CTools.h"
extern CTools* gcpTools;

#include "tagGiftCardPacket.h"




CGiftCard::CGiftCard()
{
	m_bOK=FALSE;

	m_bOK=TRUE;

}





CGiftCard::~CGiftCard()
{

}





BOOL CGiftCard::isOK(void) const
{
	return m_bOK;
}






void CGiftCard::TransmitePacket_SC_Notify(playerCharacter_t* pPlayer,enum enumCode code) const
{
	enum tagGiftCardPacket_Notify::enumCode tr_code=
		tagGiftCardPacket_Notify::enumCode::unknown;

	switch(code){
		case success:
			tr_code=tagGiftCardPacket_Notify::enumCode::success;
			break;

		case can_not_find:
			tr_code=tagGiftCardPacket_Notify::enumCode::can_not_find;
			break;

		case password_not_match:
			tr_code=tagGiftCardPacket_Notify::enumCode::password_not_match;
			break;

		case pre_use:
			tr_code=tagGiftCardPacket_Notify::enumCode::pre_use;
			break;

		case unknown:
			tr_code=tagGiftCardPacket_Notify::enumCode::unknown;
			break;
	};



	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GIFTCARD);
		MSG_WriteShort(&netMessage, tagGiftCardPacket::enumPacketID::SC_Notify);		
		MSG_WriteByte(&netMessage, tr_code);		
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void CGiftCard::TransmitePacket_SC_Close(playerCharacter_t* pPlayer) const
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GIFTCARD);
		MSG_WriteShort(&netMessage, tagGiftCardPacket::enumPacketID::SC_Close);		
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void CGiftCard::TransmitePacket_SC_AddItem(playerCharacter_t* pPlayer,item_t& Item) const
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GIFTCARD);
		MSG_WriteShort(&netMessage, tagGiftCardPacket::enumPacketID::SC_AddItem);
		MSG_WriteItem(&netMessage,&Item);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void CGiftCard::TransmitePacket_SC_Open(playerCharacter_t* pPlayer) const
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GIFTCARD);
		MSG_WriteShort(&netMessage, tagGiftCardPacket::enumPacketID::SC_Open);		
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}


void CGiftCard::Decoder_CS_Request(playerCharacter_t* pPlayer) const
{
	if(pPlayer->GiftCard.step != playerCharacter_t::tagGiftCard::step_none){
		TransmitePacket_SC_Notify(pPlayer,CGiftCard::enumCode::unknown);
		return;
	}

	pPlayer->GiftCard.step = playerCharacter_t::tagGiftCard::step_request;
	strncpy(pPlayer->GiftCard.serial,MSG_ReadString(),playerCharacter_t::tagGiftCard::maxbytes_serial_num);
	pPlayer->GiftCard.serial[playerCharacter_t::tagGiftCard::maxbytes_serial_num]=NULL;

	strncpy(pPlayer->GiftCard.password,MSG_ReadString(),
		playerCharacter_t::tagGiftCard::maxbytes_password);
	pPlayer->GiftCard.password[playerCharacter_t::tagGiftCard::maxbytes_password]=NULL;

	g_DBGameServer->LoadGiftCard(pPlayer);
}

void CGiftCard::Decoder_CS_CloseWindow(playerCharacter_t* pPlayer) const
{
	if(pPlayer->busyState == BUSY_STATE_NPC){
		pPlayer->busyState = BUSY_STATE_NONE;
		monsterCharacter_t* pNPC=gcpTools->GetNPCRecordPointer(pPlayer->npcIdx);
		if(NULL != pNPC){			
			NPC_DeleteNoticePC(pNPC,pPlayer);
		}
	}
}




void CGiftCard::Decoder(playerCharacter_t* pPlayer) const
{
	enum tagGiftCardPacket::enumPacketID PacketID=
		(enum tagGiftCardPacket::enumPacketID)MSG_ReadShort();

	switch(PacketID){
		case tagGiftCardPacket::enumPacketID::CS_Request:
			Decoder_CS_Request(pPlayer);
			break;
		case tagGiftCardPacket::enumPacketID::CS_Close:
			Decoder_CS_CloseWindow(pPlayer);
			break;
	}

}










