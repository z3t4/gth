



#if !defined(AFX_CTERRABILLINGCTRL_ENCODER_H__CDF45D1F_C7F7_4963_8751_F14D246399C9__INCLUDED_)
#define AFX_CTERRABILLINGCTRL_ENCODER_H__CDF45D1F_C7F7_4963_8751_F14D246399C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CTerraBillingCtrl_Encoder  
{
	private:
		BOOL m_bOK;

	public:
		BOOL isOK(void) const;
		void Transmite_Server_Conn(void);
		void Transmite_Server_Reset(void);
		
		#ifdef _GAME_SERVER
		void Transmite_Game_Start(const playerCharacter_t* pPlayerRecord) const;	

		void Transmite_Game_End(const playerCharacter_t* pPlayerRecord) const;
		void Transmite_Game_End(
				const char* pszPlayerIdx,
				const char* pPlayerID,
				const char* pIPaddress,
				const char* pUser_Status) const;
		void Transmite_Game_End(const struct tagAddBillPacket& Packet) const;

		void Transmite_User_Sync(
				BOOL bExist,
				const char* pszPlayerIdx,
				const char* pPlayerID,
				const char* pIPaddress,
				const char* pUser_Status) const;
		void Transmite_User_Sync(const struct tagAddBillPacket& Packet) const;
		void Transmite_User_Sync(const playerCharacter_t* pPlayerRecord) const;
		#endif


		
		
		


		




	
	#ifdef _MASTER_SERVER	
	public:
		void Transmite_Billing_Authorization(const i3client_t* pPlayer);

	#endif


	
	public:
		CTerraBillingCtrl_Encoder();
		virtual ~CTerraBillingCtrl_Encoder();

};

#endif 
