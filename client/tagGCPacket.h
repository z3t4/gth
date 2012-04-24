


#ifndef _TAGGCPACKET_H_
#define _TAGGCPACKET_H_



struct tagGCPacket_POSTSYSTEM_OPEN{
	enum enumCode{
		fail=0,
		success,
	};
};



struct tagGCPacketHeader{
	public:
		enum enumID{
			Transport_map_repy=GSC_TRANSPORTWORLDREP,
		};

	public:
		struct tagPosition{
			short int x;
			short int y;
			short int z;
		};


	public:
		unsigned char packetID;
};


struct tagGCPacket_TRANSPORTWORLDREP : public tagGCPacketHeader{
	public:
		enum enumCode{
			fail=0,
			success=1,
			access_deny_because_free_user,
			
			busy_state,
		};
	public:
		unsigned char		code;
		unsigned char		worldIdx;
		struct tagPosition	position;
		float				angle_y;

};





#endif
