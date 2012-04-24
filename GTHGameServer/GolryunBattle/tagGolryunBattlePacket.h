

#ifndef _TAGGOLRYUNBATTLEPACKET_H_
#define _TAGGOLRYUNBATTLEPACKET_H_


struct tagGolryunBattlePacket{

	enum {
		GB_REQUEST_INIT_BATTLE			= 0,	
		GB_REQUEST_START_BATTLE,
		GB_REQUEST_START_JOIN,
		GB_REQUEST_CLOSE_JOIN,
		GB_REQUEST_START_BETTING,
		GB_REQUEST_READY_TOURNAMENT,
		GB_REQUEST_START_COUNT,
		GB_REQUEST_PROGRESS_MATCH,
		GB_REQUEST_END_MATCH,
		GB_REQUEST_WAIT_TOURNAMENT,
		GB_REQUEST_CLOSE_BATTLE,
		GB_REQUEST_START_PRIZE,
		GB_REQUEST_CANCEL_TOURNAMENT,
		
		GB_TRANS_GOLRYUN,
		GB_ACCEPT_TRANS_GOLRYUN,			
		GB_TRANS_ARAYAN,
		GB_BUY_BATTLE_TICKET,				
		GB_RENEW_BATTLE_TICKET,				
		GB_RENEW_DISPLAY_TOURNAMENT,		
		GB_SUGGEST_BATTLE,				
		GB_ACCEPT_BATTLE,				
		GB_OPEN_BATTLE_TICKET,			
		GB_SHARE_RATE,
		
		GB_OPEN_SHARE_WIN,					
		GB_DISPLAY_TOURNAMENT,			
		GB_CHECK_PRIZE,					
		GB_TIME_OVER,
		GB_EFFECT_RESULT,				
		GB_CHECK_START_MATCH,					
		GB_ERROR_CODE,
		GB_LOCAL_USER_NOTICE,
		GB_ALL_USER_DATA,
		GB_START_MATCH_NOTICE,		
		
		GB_END_MATCH_NOTICE,	
		GB_BATTLE_RASA_Sync,	
		GB_CHECK_JOINER_NUM,
		GB_STATUS_MATCH_NOTICE,
		GB_TRANS_WAITINGROOM,
		GB_TRANS_WATCHINGROOM,
		GB_TRANS_BATTLESTADIUM,
		GB_NOTICE_BATTLE,
		GB_WALKOVER_MATCH,
		GB_TWOGYUN_TOTAL_NAK,		

		GB_OBSERVE_START,
		GB_OBSERVE_END,
		GB_OBSERVE_MATCH_TIME,

		GB_MATCH_PROGRESS_TIME,
		GB_PAUSE_MATCH,
		GB_RESTART_MATCH,
		GB_GM_NOTICE,		
		
		GB_INSERT_JOIN_LIST,
		GB_ASSIGN_FIGHTER_NUMBER,
		GB_START_RECALL_FIGHTER,
		GB_FORCE_EJECT,
		GB_CHECK_STATUS_PvPMODE,
	};
};


struct tagGolryunBattlePacket_GB_ERROR_CODE{
	enum enumCode{
		overflow_fighter_num = 0,           
		not_enough_nak,					    
		not_time_buy_Twogyun,				
		unknown_item,						
		not_sixteen_fighter,				
		not_exchange_twogyun,				
		not_selected_twogryun_figtherNum,	
		not_enough_inventory,				
	};
};

struct tagGolryunBattlePacket_GB_STATUS_NOTICE{
	enum enumCode{
		unknown=0,
		playing_tournament=1,
		cancel_tournament=2,
		invalid_towgun=3,	
		draw_tournament=4,
	};
};


struct tagGolryunBattlePacket_GB_REQUEST_END_MATCH{
	enum enumCode{
		winner=0,
		loser=1,
	};
};



#endif