#ifndef _GOLRYUNBATTLE_H_																			
#define _GOLRYUNBATTLE_H_


struct TicketSerialInfo
{
	int TIME		: 8;  
	int LEVEL		: 8;  
	int WINNUM		: 6;  
};

class CGolryunBattle
{

public :

	enum {
		
		GOLRYUN_BATTLE_MAP_INDEX			= 21,
		MAX_NUM_FIGHTER_PER_TOURNAMENT		= 16,		
		MAX_NUM_TOURNAMENT_PER_BATTLE		=  7,		
		MAX_NUM_MATCH_PER_TOURNAMENT		= 15,
		MAX_NUM_BATTLE_RECORD				= 10,
		MAX_NUM_PRIZE_LIST_PER_BATTLE		= 16,

		ENTERANCE_TICKET_ITEM_INDEX			= 450,
		TWOGYUN_100000_ITEM_INDEX			= 456,
		TWOGYUN_1000000_ITEM_INDEX			= 457,
		BOKGYUN_ITEM_INDEX				    = 458,
		GOLDBAR_ITEM_INDEX					= 459,
	
	};

	enum eManor
	{
		normal =0,
		week,
		month,
		ramification,
		year,
	};

	enum eRoundStep{
		round_16 = 16,
		round_8 =8,
		round_4 =4,
		round_final =2,
	};
	
	enum eObserverMode{	
		general		=0,
		observe		  ,	
	};

	struct tagFighter{
		bool bWin;
		int  iFightNum;
	};

	struct tagMatch{
		public:
			BOOL bComplete;
			struct tagFighter Fighter[2];			
	};

	struct tagTournament{
		public:
			BOOL	bComplete;
			int		m_iTournamentGUID;
			float	m_ShareRate;
			struct tagFighter	FirstWinner;
			struct tagFighter	SecondWinner;
	};

	struct tagBattle{
		public:
			BOOL	bComplete;
			int		m_iBattleGUID;
			int		m_PrizeNumber[MAX_NUM_PRIZE_LIST_PER_BATTLE];
			struct tagTournament Tournament[MAX_NUM_TOURNAMENT_PER_BATTLE];
	};

private :
	
	unsigned int	m_iUpdateTime;
	unsigned int	m_iStartTime;
	int				m_iStartCnt;		
	int				m_iStatus;
	
	
	tagMatch		*m_Match;
	

	
	tagTournament	*m_Tournament;
	int				m_iTournamentIdx;			

	
	tagBattle		*m_BattleCurrent;
	int				m_iBattleIdx;

	
	tagBattle		*m_BattleRecord;
	
	eObserverMode   m_eObserverMode;

public :
	
	vec3_t			m_OldPosition;
	
	
	vec3_t			m_ObserverPos;
	
public :
	CGolryunBattle();
	virtual ~CGolryunBattle();

public :
	void	InitMatch();
	int		InitTournament(tagTournament* pTournament);
	void	InitBattle(tagBattle* pBattle);
	void    InitBattleRecord();

	void	SetStatus(int iStatus);
	int		GetStatus();

	struct tagMatch*	  GetMatchPointer(const int matchIdx);
	struct tagTournament* GetTournamentPointer(){ return m_Tournament; };
	struct tagBattle*	  FindBattleRecordPointer(const int battleIdx);
	struct tagBattle*	  GetBattleCurrentPointer(){ return m_BattleCurrent; };
	struct tagBattle*	  GetBattleRecordPointer(const int battleIdx);
	int					  GetTournamentIdx(){ return m_iTournamentIdx; };
	void				  SetTournamentIdx(int idx){ m_iTournamentIdx = idx; };
	int					  GetBattleIdx(){ return m_iBattleIdx; };
	void				  SetBattleIdx(int idx){ m_iBattleIdx = idx; };

	int		EncodeSerial(int time,int level,int winNum);
	TicketSerialInfo DecodeSerial(int Serial);

	void	SetManorGrade(Fx_CHARACTER_t *character );

	void	SetObserverMode(eObserverMode eMode){ m_eObserverMode = eMode; };
	eObserverMode	GetObserverMode(){ return m_eObserverMode; };


public :
	void    ProcessTransGolryunBattle();
	void    ProcessSuggestBattle();
	void	ProcessEffectCount();
	BOOL	CheckInventoryMaxSize();
	void	SetupObserver(void);
	void	SetupReStartGame(void);

public :
	void	RecvGolryunBattle();
	void    RecvTransGolryun();
	void    RecvTransArayan();
	void    RecvSuggestBattle();
	void	RecvActiveBattle();
	void	RecvOpenBattleTicket();
	void	RecvShareRate();
	void	RecvReNewBattleTicket(void);
	void	RecvOpenShareWin();
	void	RecvDisplayTournament();
	void	RecvCheckPrize();
	void	RecvEffectCount();
	void	RecvEffectResult();
	void	RecvStartMatch();
	void	RecvEndMatch();
	void	RecvInitTournament();
	void	RecvErrorCode();
	void	RecvJoinNumber();
	void	RecvLocalUserNotice();
	void    RecvAllUserData();
	void	RecvStartMatchNotice(void);
	void	RecvEndMatchNotice(void);
	void	RecvSyncRASAMatch(void);
	void	RecvCheckJoinerNum(void);
	void	RecvStatusMatchNotice(void);
	void	RecvNoticeBattle(void);
	void	RecvBuyBattleTicket(void);
	void	RecvTwoGyunTotalNak(void);
	void	RecvObserveStart(void);
	void	RecvObserveEnd(void);
	void	RecvObserveMatchTime(void);
	void	RecvMatchProgressTime(void);
	void	RecvPauseMatchNotice(void);

public :
	void	SendRequestStartBattle(int iManorIdx);
	void	SendRequestStartJoin();
	void	SendRequestCloseJoin();
	void	SendRequestStartBetting();
	void    SendRequestReadyTournament();
	void	SendRequestStartCount();
	void	SendRequestEndMatch();
	void	SendRequestWaitTournament();
	void	SendRequestCloseBattle();
	void    SendRequestStartPrize();
	void	SendRequestCancelTournament();
	
	void    SendAcceptTransGolryun();
	void    SendAcceptToBattle();
	void    SendBuyBattleTicket(int itemTableIdx, int number);
	void    SendReNewBattleTicket();
	void	SendReNewDisplayTournament();

	void	SendCheckStartMatch();
	void	SendCheckJoinerNum();
	void	SendTransWaitingRoom();
	void	SendTransWatchingRoom();
	void	SendTransBattleStadium();
	void	SendNoticeBattle(char* notice);
	void	SendWalkOverMatch(int joinNum);
	void	SendTwoGyunTotalNak(void);
	void	SendObserveEnd(void);
	void	SendOpenBattleTicket(void);
	void	SendOpenShareWin(void);
	void	SendDisplayTournament(void);
	void    SendRequestPauseMatch(void);
	void    SendRequestReStartMatch(void);
	void	SendInsertJoinList(char* name);
	void    SendAssignFighterNumber(char* name, int number);
	void    SendRequestStartReCallFighter(void);

};

struct tagGolryun_Packet_Status{
	public:
		enum{		
			GB_REQUEST_INIT_BATTLE				= 0,
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

struct tagGolryun_Packet_Error_Code{
	public:
		enum{		
			GB_ERROR_CODE_1				= 0,		
			GB_ERROR_CODE_2				,			
			GB_ERROR_CODE_3				,			
			GB_ERROR_CODE_4				,			
			GB_ERROR_CODE_5				,			
			GB_ERROR_CODE_6				,			
			GB_ERROR_CODE_7				,			
			GB_ERROR_CODE_8				,			
		};
};

extern CGolryunBattle g_GolryunBattle;

#endif