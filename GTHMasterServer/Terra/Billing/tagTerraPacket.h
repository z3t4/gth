





#ifndef _TAGTERRAPACKET_H_
#define _TAGTERRAPACKET_H_


struct tagTerraGCPacket{
	enum{
		Open_MessageBox=0,
		NoticeMessage,
		Remain_Minute_NoticeMessage,
	};
};

struct tagTerraGCPacket_Remain_Minute_NoticeMessage{
	int RemainMinute;
};


struct tagTerraGCPacket_NoticeMessage{
	public:
		enum enumLPACKType{
			Billing=0,
			HackShield,
		};
	public:
		char LPACKType; 
		char LPACKIdx; 
};



struct tagTerraGCPacket_OpenMessageBox{
	public:
		enum enumAfter{
			after_none=0,
			after_client_exit,
		};

		enum enumLPACKType{
			Billing=0,
			HackShield,
		};
	public:
		char LPACKType; 
		char LPACKIdx; 
		char after;
};



#endif