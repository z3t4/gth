


#ifndef _TAGGIFTCARDPACKET_H_
#define _TAGGIFTCARDPACKET_H_



struct tagGiftCardPacket{
	public:
		enum enumPacketID{
			SC_Open=0x0001,
			CS_Request,
			SC_Notify,
			SC_AddItem,
			SC_Close,
			CS_Close,
		};
};


struct tagGiftCardPacket_Notify{	
	public:
		enum enumCode{
			success=0,
			can_not_find,
			password_not_match,
			pre_use,
			unknown,
		};
};


#endif