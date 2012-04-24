
#ifndef _tagNewDanBattle_H
#define _tagNewDanBattle_H


struct tagGCPacket_NewDanBattle{
	public:	
		enum enumCode{
			fail	= 0,
			success = 1,
		};

		enum enumwincode{
			loss	= 0,
			win		= 1,
			draw    = 2,
		};
		
		enum enumDanBattleInsertcode{
			insert_fail = -1,
			insert_success =0,
			already_active_requestdan,
			already_active_acceptdan,
			full_danbattleCount,
			
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
			attleInsert_Error,					
			Not_Accept_danBattle,				
			
			Error_Full_danbattleCount,
			
		};


		enum PlunderPcCode
		{
			 Attacker = 0,						
			 Dier = 1,							
		};

		enum PlunderItemCode
		{
			itemplunderFail    = 0,				
			itemplunderSuccecs = 1,				
			itemDeleteSuccecs  = 2,				
			Nomoreitemplunder  = 3,				

		};
};

#endif