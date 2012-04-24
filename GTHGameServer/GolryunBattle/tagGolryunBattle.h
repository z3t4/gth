	

#ifndef _TAGGOLRYUNBATTLE_H_
#define _TAGGOLRYUNBATTLE_H_


struct tagGolryunBattleMessage{
	enum{
		PAUSE_MATCH=0,
		PAUSE_MATCH_RESERVED=1,
		PAUSE_MATCH_REPLAY=2,
		AUTO_PAUSE_PRE_FINAL_MATCH=3,
		ERROR_INSERT_JOINMAP=4,
		SUCCESS_INSERT_JOINMAP=5,
		ERROR_ASSIGN_FIGHTER_NUMBER=6,
		SUCCESS_ASSIGN_FIGHTER_NUMBER=7,
		ERROR_FORCE_EJECT=8,
		SUCCESS_FORCE_EJECT=9,
		ERROR_STATUS_PvPMODE=10,
		SUCCESS_STATUS_PvPMODE=11,
		ERROR_SQL = 12,	
		NOTREADY=13,
		ERROR_OVERFLOW_TOURNAMNETIDX=14,
		ERROR_PREVIOUS_PLAY_TOURNAMENT=15,
		ERROR_UNKNOWN=16,
	};
};


struct tagGolryunBattleVar{
	public:
		struct tagLog{
			BOOL bOutput;
			int  UpdateTimeMiliSecond;
		};
	public:
		struct tagLog Log;
		int GOLRYUNBATTLE_BEFORE_MATCH_WAIT_TIME;
		int GOLRYUNBATTLE_CEREMONY_WAIT_TIME;
};

struct tagGolryunBattle{
	public:
		enum{

			Golryun_Battle_Map_Index		= 21,
			Max_Num_Fighter_Per_Tournament	= 16,
			Max_Num_Tournament_Per_Battle	=  7,
			Max_Num_Match_Per_Tournament	= 15,
			Max_Num_Battle_Record			= 10,
			Max_Num_Prize_List_Per_Battle	= 16,
			Max_Num_Join_Per_Tournament		= 30,

			Entrance_Ticket_Item_Index		= 450,
			TwoGyun_100000_Item_Index		= 456,
			TwoGyun_1000000_Item_Index		= 457,
			GoldBar_Item_Index				= 459,	
			Fortune_Stone_Item_Index		= 620,  
		};		
};


#endif