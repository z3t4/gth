


#ifndef _TAGGMPACKET_H_
#define _TAGGMPACKET_H_



struct tagGMPacket_ChatBan_cs_response_chat_allow{
	public:
		enum enumCode{			
			code_success=0,
			code_not_find_player,
		};

	public:
		int  code;
		char ApplyPlayerName[NAMESTRING];
};


struct tagGMPacket_ChatBan_sc_response_chat_block{
	public:
		enum enumCode{
			code_success=0,
			code_not_find_player,
		};

	public:
		int  code;
		char ApplyPlayerName[NAMESTRING];
		int  Time;
};


struct tagGMPacket_ChatBan_cs_request_chat_block{
	char TargetName[NAMESTRING];
	int  Time;
};

struct tagGMPacket_ChatBan_sc_chat_allow{
	enum enumCode{
		code_by_GM=0,
		code_by_TimeOut,
	};
	int code;

};





#endif