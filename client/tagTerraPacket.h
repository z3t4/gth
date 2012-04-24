





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


struct tagTerraGCPacket_MSC_REPLYLOGIN{
	public:
		enum enumCode{
			success=0,
			mismatch_password=1,
			not_register_user=2,
			pre_login_user=3,
			database=4,
			block_user=5,
			billing_network=6,
			billing_point_not_enough=7,
			billing_error=8,
			unknown,
		};
	public:
		unsigned char code;
		char          account_id;
};

#endif