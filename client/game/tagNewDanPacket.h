
#ifndef _tagNewDanBattle_H
#define _tagNewDanBattle_H


struct tagGCPacket_NewDanBattle{
	public:	
		
		enum 
		{
			msg_max_len = 256,
		};
		enum enumCode{
			fail	= 0,
			success = 1,
		};

		enum enumwincode{
			loss    = 0,
			win     = 1,
			draw    = 2,
		};

		enum enumError{
			none=0,
			not_join_guild,		
			not_capacity,			
			nonexstent_guild,		
			not_enough_my_member,	
			myself_guild,			
			not_enough_target_member,
			target_can_not_reciveMessage, 
			battleInsert_Error,	
			not_Accept_DanBattle,
			Error_Full_danbattleCount,	
		};


		enum PlunderPcCode
		{
			Attacker = 0,
			Dier     = 1,
		};

		enum PlunderItemCode
		{
			itemplunderFail    = 0,
			itemplunderSuccecs = 1,
			itemDeleteSuccecs  = 2,
			NomoreitemPlunder  = 3,
		};
};

#endif