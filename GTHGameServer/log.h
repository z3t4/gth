#ifndef _LOG_H_
#define _LOG_H_


#include "./Billing/CriticalSection.h"
#include "./log/CLogFileCtrl.h"





class CLogSystem
{
private:
	CLogFileCtrl	m_UserLogTTCtrl;
	CLogFileCtrl	m_UserLogCtrl;
	CLogFileCtrl	m_DebugLogCtrl;
	CLogFileCtrl	m_ErrorLogCtrl;
	
	CLogFileCtrl	m_NewDanbattleLogCtrl;
	
	CLogFileCtrl	m_HackingUserLogCtrl;

	char			m_logFileName[MAX_PATH];
	char			m_logTTFileName[MAX_PATH];
	char			m_debugFileName[MAX_PATH];
	char			m_NewDanBattleFileName[MAX_PATH];
	char			m_HackingUserFileName[MAX_PATH];



	
	public:
	enum enumGolryunSubLog{
		EnterTicket=0,			
		BattleTicket=1,			
		JoinBattle=2,			
		StartMatch=3,			
		MatchResult=4,			
		TournamentResult=5,		
		TournamentCancel=6,		
		RewardWinner=7,			
		RewardLoser=8,			
		RewardWaiter=9,			
		ShareBattleTicket=10,	
		RewardWinner_Item=11,	
	};
	


		
		
		

private:	
	unsigned int	m_updateTime;
	int				m_updateCycle;
	int				m_generationHour;

	
public:	
	void Initialize(const BOOL in_bCreateLog,const BOOL in_bCreateDebug);		
	BOOL Open(void);		
	BOOL Close(void);		
	BOOL Update(void);

public:
	
	
	void WriteToLog(const playerCharacter_t* in_pPlayer,const int in_logClass,const char* in_format, ...  );
	void Write( const char *format, ... );
	void WriteToError( const char *format, ... );
	void WriteToLog( const char *format, ... );
	void WriteToLogTT( const char *format, ... );
	void WriteRLog( const char *format, ... );
	
	void WriteToNewDanBattleLog( const char *format, ... );

	
	void WriteToHackingLog( const char *format, ... );


	
public:
	void WriteItemDeleteLog(const playerCharacter_t* in_pPlayer,const item_t& in_item);


	
public:
		CLogSystem();
		~CLogSystem();
};


enum
{
	LOG_CLASS_LEVELUP = 0,		
	LOG_CLASS_GENLEVELUP,		
	LOG_CLASS_ITEM,				
	LOG_CLASS_PCDIE,			
	LOG_CLASS_CONNECT,			
	LOG_CLASS_DISCONNECT,		
	LOG_CLASS_PCCREATE,			
	LOG_CLASS_PCDELETE,			
	LOG_CLASS_PCERROR,			
	LOG_CLASS_SKILL,            
	LOG_CLASS_CHEAT,			
	LOG_CLASS_GOLRYUN,

	LOG_CLASS_POSTSYSTEM,



	LOG_CLASS_NEWDANBATTLE,     

};



enum
{
	LOG_CLASS_NEWDANBATTLE_START =0,
	LOG_CLASS_NEWDANBATTLE_END,
	LOG_CLASS_NEWDANBATTLE_RESULT
};

enum
{
	LOG_CLASS_NEWDANBATTLE_END_LIVECOUNT =0,
	LOG_CLASS_NEWDANBATTLE_END_TIMESET,
	LOG_CLASS_NEWDANBATTLE_END_GM_COMMAND,
};






enum{
	LOG_CLASS_SKILL_RESURRECTION=0,
};

enum
{
	LOG_CLASS_ITEM_PICKUP	=	0,
	LOG_CLASS_ITEM_DROP,
	LOG_CLASS_ITEM_STACK,
	LOG_CLASS_ITEM_BUY_STORE,
	LOG_CLASS_ITEM_SELL_STORE,
	LOG_CLASS_ITEM_LEARN_SKILL,
	LOG_CLASS_ITEM_SKILL_SCROLL,
	LOG_CLASS_ITEM_USE_PORTAL,
	LOG_CLASS_ITEM_DESTROY,
	LOG_CLASS_ITEM_CONFIRM_TRADE,
	LOG_CLASS_ITEM_UNDO_TRADE,
	LOG_CLASS_ITEM_ADD_QUEST,
	LOG_CLASS_ITEM_DEL_QUEST,
	LOG_CLASS_ITEM_DIE_DROP,
	LOG_CLASS_ITEM_DIE_DESTROY,
	
	
	LOG_CLASS_ITEM_COMPLETE_CRAFT,
	LOG_CLASS_ITEM_CRAFT_SOURCE,
	LOG_CLASS_ITEM_CRAFT_RESULT,
	LOG_CLASS_ITEM_UNDO_CRAFT,
	
	
	LOG_CLASS_ITEM_UNDO_BOOTH,
	LOG_CLASS_ITEM_SELL_BOOTH,
	LOG_CLASS_ITEM_BUY_BOOTH,

	
	LOG_CLASS_ITEM_COMPLETE_UPGRADE,
	LOG_CLASS_ITEM_UPGRADE_SOURCE,
	LOG_CLASS_ITEM_UPGRADE_RESULT,
	LOG_CLASS_ITEM_UNDO_UPGRADE,
	
	
	
	LOG_CLASS_ITEM_CHEAT_LEVEL,
	LOG_CLASS_ITEM_CHEAT_ITEM,
	LOG_CLASS_ITEM_CHEAT_NAK,
	LOG_CLASS_ITEM_CHEAT_SKILL,
	LOG_CLASS_ITEM_CHEAT_QUEST,

	
	LOG_CLASS_ITEM_GAMBLE,
	
	LOG_CLASS_ITEM_USE,
};
	

enum
{
	LOG_CLASS_POSTSYSTEM_RECEIVE_WEB = 0,
	LOG_CLASS_POSTSYSTEM_SEND_CHA,
	LOG_CLASS_POSTSYSTEM_RECEIVE_CHA,
	LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR,
};

enum{
	LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR_NOT_DELETE_POST=0,
	LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR_UNKNOWN_PACKAGE,
	LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR_NOT_INSERT_INVENTORY,
};



enum
{
	LOG_CLASS_ITEM_MISSMATCH	=	0,
};


enum
{
	LOG_CLASS_ITEM_MISSMATCH_TYPE_SAVE	=	0,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_TRADE,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_CRAFT,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_UPGRADE,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_BOOTH,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_AUTOQUESTCRAFT,
	
	LOG_CLASS_ITEM_MISSMATCH_TYPE_BATTINGTRADE,
	LOG_CLASS_ITEM_UNDO_BATTINGTRADE,
};






char* GetItemInformationLog( int isStack, int itemTableIdx, int itemIdx, int inventoryPos );

#endif

