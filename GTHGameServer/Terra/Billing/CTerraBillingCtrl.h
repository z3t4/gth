



#if !defined(AFX_CTERRABILLINGCTRL_H__F366D29E_FA42_4018_8DBA_0A9E735D3184__INCLUDED_)
#define AFX_CTERRABILLINGCTRL_H__F366D29E_FA42_4018_8DBA_0A9E735D3184__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CTerraBillingCtrl  
{
	private:
		enum{
			MaxNum_TrnamisteCircularQueueEle = 512,
			MaxNum_ReceiveCircularQueueEle = 512,
		};
		BOOL m_bOK;

		enum enumStep{
			step_none=0,
			step_connect_try,
			step_transmite_packet_conn_and_reset,
			step_service,
			step_disconnect_try,			
		};

		enum enumStep m_Step;



	private:
		BOOL Init(void);
		BOOL LoadServerConfigFile(void);

		inline void TransmiteAllPacketInTransmiteCircularQueue(void) const;


	#ifdef _MASTER_SERVER
	public:
		inline i3client_t* GetPlayerRecodPointer(const struct tagAddBillPacket& Packet) const;
		inline void Decoder_billing_authorization(const struct tagAddBillPacket& Packet) const;
	#endif



		

	public:
		BOOL isOK(void) const;
		void Process(void);
		inline void Process_step_service(void);

		BOOL ServiceStart(void);
		void ServiceEnd(void);
		static void OpenNotePadMessage(const char* pMessage);


	
	public:
		#ifdef _GAME_SERVER
		void Player_GameStart(playerCharacter_t* pPlayerRecord);
		void Player_GameEnd(playerCharacter_t* pPlayerRecord);
		BOOL CheckPlayer(playerCharacter_t* pPlayerRecord);
		#endif

	

	
	public:
		CTerraBillingCtrl();
		virtual ~CTerraBillingCtrl();

};

#endif 
