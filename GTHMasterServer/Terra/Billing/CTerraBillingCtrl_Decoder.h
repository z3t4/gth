



#if !defined(AFX_CTERRABILLINGCTRL_DECODER_H__0C021EEE_EF4D_445A_89C9_EF91A9ABCB06__INCLUDED_)
#define AFX_CTERRABILLINGCTRL_DECODER_H__0C021EEE_EF4D_445A_89C9_EF91A9ABCB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "tagTerraPacket.h"
class CTerraBillingCtrl_Decoder  
{
	private:
		BOOL m_bOK;

	
	private:
		#ifdef _MASTER_SERVER
		inline i3client_t* GetPlayerRecodPointer(const struct tagAddBillPacket& Packet) const;
		inline void Decoder_billing_authorization(const struct tagAddBillPacket& Packet) const;
		#endif

	public:

		#ifdef _GAME_SERVER
		inline playerCharacter_t* GetPlayerRecodPointer(const struct tagAddBillPacket& Packet) const;
		inline void Decoder_game_start(const struct tagAddBillPacket& Packet) const;
		inline void Decoder_user_sync(const struct tagAddBillPacket& Packet) const;
		inline void Decoder_user_alert(const struct tagAddBillPacket& Packet) const;
		#endif

	public:
		inline void Decoder_server_alive(const struct tagAddBillPacket& Packet) const;


	public:
		BOOL isOK(void) const;
		void Decoder(const struct tagAddBillPacket& Packet);

	private:

		#ifdef _GAME_SERVER
		void Transmite_TerraGCPacket_OpenMessageBox(
				playerCharacter_t* pToPlayerRecord,
				enum tagTerraGCPacket_OpenMessageBox::enumLPACKType LPACKType,
				const int LPACKIdx,
				enum tagTerraGCPacket_OpenMessageBox::enumAfter after) const;

		void Transmite_TerraGCPacket_NoticeMessage(
			playerCharacter_t* pToPlayerRecord,
			enum tagTerraGCPacket_NoticeMessage::enumLPACKType LPACKType,
			const int LPACKIdx) const;

		void Transmite_TerraGCPacket_Remain_Minute_NoticeMessage(
			playerCharacter_t* pToPlayerRecord,
			const int RemainMinute) const;
		#endif

		





	
	public:
		CTerraBillingCtrl_Decoder();
		virtual ~CTerraBillingCtrl_Decoder();

};

#endif 
