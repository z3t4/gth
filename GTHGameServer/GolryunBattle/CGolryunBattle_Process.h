



#if !defined(AFX_CGOLRYUNBATTLE_PROCESS_H__E16ED788_4286_4757_884F_BDAE691AC9D3__INCLUDED_)
#define AFX_CGOLRYUNBATTLE_PROCESS_H__E16ED788_4286_4757_884F_BDAE691AC9D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 




#include "CGolryunBattle_Encoder.h"

#include "CGolryunBattle_Data.h"
#include "CGolryunBattle_DBV2.h"

#include <map>

class CGolryunBattle_Process  
{
	public:		
		enum enumManor{
			normal=0,
			week=1,
			month=2,
			ramification=3,
			year=4,
		};

		
		enum eRound
		{
			Round16 = 16,
			Round8 = 8,
			Round4 = 4,
			Round2 = 2,
		};

		
		enum eMatchIndexRound
		{
			
			Offset_MatchIdx_Round16			= 0,		
			Offset_MatchIdx_Round8			= 8,		
			Offset_MatchIdx_Round4			= 12,		
			Offset_MatchIdx_RoundFinal		= 14,		
		
			
			End_MatchIdx_Round16		= 7,		
			End_MatchIdx_Round8			= 11,		
			End_MatchIdx_Round4			= 13,		
			End_MatchIdx_RoundFinal		= 14,		
		}; 

		
		enum eMatchResult
		{
			RESULT_NORMAL_MATCH	= 0,
			RESULT_TIMEOVER_MATCH,
			RESULT_DRAWN_MATCH,
			RESULT_OFFLINE_MATCH,
			RESULT_DEFAULT_MATCH,
			RESULT_ERROR_MATCH,
		}; 

		
		enum eReward
		{
			REWARD_WINNER			= 0,
			REWARD_LOSER,
			REWARD_WAITER,
		};


		
		enum eTournamentStep{
			TournamentStep_None=0,
			TournamentStep_Match_Pause,				
			TournamentStep_END_MATCH,
			TournamentStep_PROGRESS_MATCH,
			
			TournamentStep_START_COUNT,
			TournamentStep_START_BETTING,
			TournamentStep_START_JOIN,
			TournamentStep_CLOSE_JOIN,

			
			TournamentStep_READY_TOURNAMENT,
			TournamentStep_CANCEL_TOURNAMENT,

			TournamentStep_INIT_BATTLE,
			TournamentStep_START_BATTLE,
			TournamentStep_CLOSE_BATTLE,

			TournamentStep_START_PRIZE,
		};

		struct tagBattleTimer{		
			public:
				enum{
					timeSyncPacket_Transmite_Time = 1* 1000,
					match_start_count_time = 10*1000,
					end_check_time=5 * 60 * 1000,
				};
			
			public:
				int start_time;
				int old_timeSync_transmite_Time;
		};


		
		
		

		

		
		
		
		struct tagFightPlayerInfo{			
			public:
				struct tagRecord{
					int win;
					int lose;
					int draw;
				};
			public:
				int Idx;
				int Job;
				char Name[NAMESTRING+1];
				char ID[IDSTRING+1];
				struct tagRecord Record;
		};		


		struct tagTicket{		
			private:
				struct tagTicketFighter{
					int   iTotalNak;
					float fShareRate;
				};
			public:					
				int    iTotalNak;
				
				int	   iFighterNum;   
				struct tagTicketFighter Fighter[tagGolryunBattle::Max_Num_Fighter_Per_Tournament];					
		};
	private:
		struct tagCheckNormalMatchParam{
			public:
				struct tagMatchFighterInfo{
					struct CGolryunBattle_Data::tagFighter* pFighter;
					playerCharacter_t* pPlayer;
				};

			public:
				struct CGolryunBattle_Data::tagMatch*   pMatch;
				struct tagMatchFighterInfo Red;
				struct tagMatchFighterInfo Blue;
				struct tagMatchFighterInfo Winner;
				struct tagMatchFighterInfo Loser;				
				enum CGolryunBattle_Data::tagMatch::enumCorner WinnerCorner;				
		};




	private:
		BOOL m_bOK;
		CGolryunBattle_Data*		m_cpData;
		CGolryunBattle_Encoder*		m_cpEncoder;
		
		CGolryunBattle_DBV2*		m_cpDBV2;
		eMatchResult			    m_eResultStatus;
		enum eTournamentStep	   m_eTournamentStep;
		enumManor				    m_eManor;
		DWORD						m_iStartTime;
		struct tagBattleTimer		m_BattleTimer;
		int							m_iNextRound_MatchIdx;			
		int							m_iMatchIdx;					
		int							m_iOffSetMatchIdx;				

		

		
		typedef std::list<CGolryunBattle_Data::tagFighter>	ALL_JOINER_LIST;
		typedef ALL_JOINER_LIST::iterator	ALL_JOINER_ITOR;
		ALL_JOINER_LIST				m_AllJoinList;	


		typedef std::map<int,struct CGolryunBattle_Data::tagFighter> JOINER_MAP;
		typedef JOINER_MAP::iterator JOINTER_MAP_ITOR;
		JOINER_MAP					m_JoinMap;		
		
		struct CGolryunBattle_Data::tagMatch   m_Match[tagGolryunBattle::Max_Num_Match_Per_Tournament];
		struct CGolryunBattle_Data::tagFighter m_FirstWinner;
		struct CGolryunBattle_Data::tagFighter m_SecondWinner;		

		

	public:
		struct tagFightPlayerInfo	m_Fighter[tagGolryunBattle::Max_Num_Fighter_Per_Tournament];	
		struct tagTicket			m_Ticket;		
		int							m_iTournamentGUID;				
		int							m_iTournamentGUIDminmum;
		int							m_iTournamentIdx;				
		BOOL                        m_bPause;
		

	private:		
		

		void		SetResultStatus(eMatchResult eResultStatus);		
		void		InitFighterArray(void);	
		inline void SetCheckNormalMatchParam(struct tagCheckNormalMatchParam& Var);
		inline void InitCheckNormalMatchParam(struct tagCheckNormalMatchParam& Var);
		
		void		SettingManorColor(playerCharacter_t* pc);

	public:
		BOOL		isOK(void) const;
		void		SetTournamentStatus(const enum eTournamentStep eTournamentStep);
		enum eTournamentStep GetTournamentStatus(void);
		void		SetManorStatus(enumManor eManor);
		BOOL		isReady(void);
		
		struct CGolryunBattle_Data::tagMatch* GetCurMatch(void);		
		
		void		SetProgressMatchStatus(BOOL bPause){ m_bPause = bPause; };
		BOOL		GetProgressMatchStatus(void){ return m_bPause; };

		void		InitMatchArray(void);
		BOOL		InitTournament();		
		BOOL		InitBattle(void);
		BOOL		isJoinPlayer(const playerCharacter_t *pc);
		BOOL		isFighterPlayer_and_LogOffMark(const playerCharacter_t *pc);
		void		CheckJoinerNumGM(void);	
		void		TransWaitingRoomGM(playerCharacter_t* pc); 
		void		TransWatchingRoomGM(playerCharacter_t* pc); 
		void		TransBattleStadiumGM(playerCharacter_t* pc); 

		BOOL		RemoveFighterArrayV2(playerCharacter_t* pc);
		
		struct tagFightPlayerInfo*					GetFighterPointer(const int fighterIdx);
		struct CGolryunBattle_Data::tagMatch* 		GetMatchPointer(const int matchIdx);		
		void		UpdateAutoStatus();
		
		

		
		

	

		


	
	public:
		
		void		UpdateHandleStatus(void);
		inline void	UpdateHandleStatus_TournamentStep_PROGRESS_MATCH(void);
		
		
		void Enter_By_Tournament(playerCharacter_t* pPlayer);		
		
		
		BOOL MoveToWaitingRoom(playerCharacter_t* pPlayer);

		
		void		ExtractJoinNumber(void);

		
		void		ExtractMatchNumber(void);
		void        ExtractPrizeNumber();



		void 		MoveToWatchingRoom(playerCharacter_t* pc);	
		void		MoveToWatchingRoomAll_NotBattlePlayer(void);
		void		MoveToWatchingRoomAll_CancelBattlePlayer(void);

		int 		BuyEntranceTicket();
		int 		BuyBattleTicket();
		void		ReNewBattleTicket();
		inline BOOL inner_MoveToWatchingRoom(playerCharacter_t* pc);
		

	
	public:
		void		CheckGameStartMatch();
		void		FindOutFighterNumber();	
		void		MoveToStartMatch();
		void		MoveToRedCorner(playerCharacter_t* pc);
		void		MoveToBlueCorner(playerCharacter_t* pc);

	
	public:
		void		MoveToEndMatch(void);
		void		MoveToEndMatch_Result(void);
		inline void MoveToEndMatch_NextMatchProgress(void);
		inline void MoveToEndMatch_NextMatchProgress_RoundEnd(void);
		void		CheckFighterWaitingRoomAndJoinNumber(void);

	
	public:
		void		CheckMatchResult(const int WinnerFightNum=-100);
		inline void CheckMatchResult_Effect(struct tagCheckNormalMatchParam& Var);
		inline void CheckMatchResult_SetWinnerAndLoser(struct tagCheckNormalMatchParam& Var);
		inline void CheckMatchResult_when_BothLive(struct tagCheckNormalMatchParam& Var);


	
	public:
		void		CheckQuitUser(playerCharacter_t *pc);	
		inline BOOL CheckQuitUser_Waiter(playerCharacter_t *pc);
		BOOL	    CheckExpelUser(playerCharacter_t *pc);
		
		
	public:	
		void		RewardBattleResult(playerCharacter_t *pc, int type);
		void		RewardForLoser(playerCharacter_t *pc);
		void		RewardForWinner(playerCharacter_t *pc);
		void		RewardForWaiter(playerCharacter_t *pc);


	
	public:
		
		playerCharacter_t* GetFighterPlayerPointer(const struct tagFightPlayerInfo* pFighter);
		playerCharacter_t* GetFighterPlayerPointer(const struct CGolryunBattle_Data::tagFighter* pFighter);
		BOOL InsertJoinMap(playerCharacter_t* pPlayer);
		struct tagFightPlayerInfo* GetMatchFighter(playerCharacter_t* pPlayer);
		BOOL AssignFightNum(const char* szpJoinerName,const int FighterNum);
		struct CGolryunBattle_Data::tagMatch* GetMatch(const int FightNum);

		
		BOOL ForceEjectToFighter(playerCharacter_t* pPlayer);

	private:
		
		
		inline void Copy_toDATA_fromDB(const struct CGolryunBattle_DBV2::tagBattle* pBattle);

		
		
		
		inline void Copy_toDB_fromDATA(struct CGolryunBattle_DBV2::tagBattle& Battle);
	public:
		void SaveBattleRecoder(void);

	public:
		CGolryunBattle_Process(
			
			CGolryunBattle_DBV2*	 cpDBV2,
			CGolryunBattle_Data*     cpData,
			CGolryunBattle_Encoder*  cpEncoder);


		virtual ~CGolryunBattle_Process();

};

#endif 
