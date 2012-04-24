

#ifndef _TAGGONRYUNPRACTICEPACKET_H_
#define _TAGGONRYUNPRACTICEPACKET_H_







struct tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM{
	public:		
		enum enumType{
			TYPE_ADD = 0,
			TYPE_DELETE = 1,
			TYPE_EXCHANGE = 2,
			TYPE_STACK_ADD = 3,
		};		
};



struct tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM{
public:
	enum errorCode{
		success = 0,							
		ERROR_Weight_Over,						
		ERROR_NotEnough_InventorySpace,			
		ERROR_NotEnough_Nake,					
		ERROR_Over_BattingNake,
		ERROR_InvalidUser,						
		ErrorCode_unknown,						
		ErrorCode_stacknum_overflow,
	};

	enum enumState{
		State_None		= 0,
		State_Admit		= 1,
		State_Win		= 2,
		State_Defeate	= 3,
		State_Fail		= 4,
		State_Check		= 5,

	};
};


struct tagGonryunPracticePacket_BATTLE_PRACTICE_MSG{
	public:
		enum enumCode{	
			success						=  1,		
			overflow_oneday_use_count	=  2,		
			missmatch_user_level		=  3,		
			not_enoughf_nak				=  4,		
			busy_GonryunBattlePractice	=  5,		
			overflow_user_number		=  6,		
			gonryun_NotUse				=  7,		
			differLevel					=  8,		
			busy_suggest                =  9,       
			unkonwn						= 10,       
			Weight_Over					= 11,
			NotEnough_InventorySpace	= 12,
			gonryun_Lest_UseCount		= 13,		
			
	};
};



#endif