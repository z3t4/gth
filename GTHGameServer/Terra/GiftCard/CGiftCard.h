



#if !defined(AFX_CGIFTCARD_H__ED7B3319_E69E_4406_9AF7_13EE4CBBE340__INCLUDED_)
#define AFX_CGIFTCARD_H__ED7B3319_E69E_4406_9AF7_13EE4CBBE340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGiftCard  
{
	public:
		enum enumCode{
			success=0,
			can_not_find,
			password_not_match,
			pre_use,
			unknown,
		};

	private:
		BOOL m_bOK;

	public:
		void Decoder(playerCharacter_t* pPlayer) const;
		inline void Decoder_CS_Request(playerCharacter_t* pPlayer) const;		
		inline void Decoder_CS_CloseWindow(playerCharacter_t* pPlayer) const;		



	public:
		BOOL isOK(void) const;
		void TransmitePacket_SC_Notify(playerCharacter_t* pPlayer,enum enumCode code) const;
		void TransmitePacket_SC_AddItem(playerCharacter_t* pPlayer,item_t& Item) const;
		void TransmitePacket_SC_Close(playerCharacter_t* pPlayer) const;

		void TransmitePacket_SC_Open(playerCharacter_t* pPlayer) const;		


	
	public:
		CGiftCard();
		virtual ~CGiftCard();

};

#endif 
