



#if !defined(AFX_CGOLRYUNBATTLE_DBV2_H__9A72C178_836E_43E8_B7BD_1EB9128D6228__INCLUDED_)
#define AFX_CGOLRYUNBATTLE_DBV2_H__9A72C178_836E_43E8_B7BD_1EB9128D6228__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CGolryunBattle_DBV2  
{
	public:
		enum enumBattleState{
			BattleState_Create=0,
			BattleState_Playing=1,
			BattleState_Complete=2,
		};

		struct tagTournament{
			public:
				struct tagFighter{
					char Name[NAMESTRING+1];
					int  Num;
				};
				struct tagTwoGyun{
					int   nTotalNak;
					float Rate;
				};

			public:
				int nGUID;
				int nLevelGroupIdx;
				BOOL bComplete;
				struct tagTwoGyun TwoGyun;
				struct tagFighter firstWinner;
				struct tagFighter secondWinner;			
		};


		struct tagBattle{			
			BOOL bActive;
			int  nGUID;
			int  nMaxTournamentGUID;
			int  nMaxEntranceTicketGUID;
			int  nLotteryGUIDList[tagGolryunBattle::Max_Num_Prize_List_Per_Battle];
			struct tagTournament Tournament[tagGolryunBattle::Max_Num_Tournament_Per_Battle];
		};

		enum enumError{
			ERROR_NONE=0,
			ERROR_NOT_READYa,
			ERROR_SQL_LOADING_WHEN_START,
			ERROR_SQL_query_AddBattle,
			ERROR_SQL_query_SaveBattle,
			ERROR_SQL_query_AddTournament,
			ERROR_SQL_query_SaveTournament,
		};

	private:

		enum{
			MAXNUM_RECORD=10,
		};		
		struct tagLoading{
			public:
				enum enumLoadingStep{
					loadingSTEP_START=0,
					loadingSTEP_WAIT,
					loadingSTEP_tournament,
					loadingSTEP_tournament_wait,
					loadingSTEP_complete,
				};
			public:
				enum enumLoadingStep step;
		};
		struct tagQueryExeMon{
			int refcount;
		};		

		enum enumError m_Error;
		BOOL m_bOK;
		
		
		
		
		struct tagBattle m_TemporaryPlayingBattle;
		struct tagBattle m_Record[MAXNUM_RECORD];
		struct tagLoading m_Loading;
		int     m_nTournamentGUIDminmum;		
		struct tagQueryExeMon m_QueryExeMon;

	private:
		inline void SetMinimumTournamentGUID(void);	

	
	public:
		BOOL				isOK(void) const;		
		void				Process(void);
		inline void			Process_loadingSTEP_tournament(void);		
		int					GetMinimumTournamentGUID(void) const;
		BOOL				Update(const int in_nUpdateTornamentGroupIdx,const struct tagBattle& in_Battle);
		void				Update(const struct tagBattle& in_Battle);
		void				Add(const struct tagBattle& in_Battle);
		void				Save(const struct tagBattle& in_Battle);

		struct tagBattle*	GetBattleRecordPointer(const int in_nBattleGUID);	
		struct tagBattle*	GetTemporaryPlayingBattle(void);

	
	public:
		enum enumError		GetLastError(void) const;
		char*               GetLastErrorStr(void) const;
		BOOL				isReady(void) const;
		
	
	public:
		
		void ODBC_LoadLastBattle(void);		
		void ODBC_LoadTournament(const int nBattleGUID);		
		void ODBC_AddBatle(const enum enumBattleState in_BattleState,const struct tagBattle& in_Battle);
		void ODBC_SaveBattle(const enum enumBattleState in_BattleState,const struct tagBattle& in_Battle);
		void ODBC_AddTournament(const int in_nBattleGUID,const struct tagTournament& in_Tournament);
		void ODBC_SaveTournament(const int in_nBattleGUID,const struct tagTournament& in_Tournament);
		
		void ODBC_QueryExecuterThread_Query_GolryunBattle_SaveTournament(const querySet_t& in_request,querySet_t& out_result);
		void ODBC_QueryExecuterThread_Query_GolryunBattle_AddTournament(const querySet_t& in_request,querySet_t& out_result);
		void ODBC_QueryExecuterThread_Query_GolryunBattle_SaveBattle(const querySet_t& in_request,querySet_t& out_result);
		void ODBC_QueryExecuterThread_Query_GolryunBattle_AddBattle(const querySet_t& request,querySet_t& result);		
		void ODBC_QueryExecuterThread_Query_GolryunBattle_LoadTournament(const querySet_t& request,querySet_t& result);
		void ODBC_QueryExecuterThread_Query_GolryunBattle_LoadLastBattle(const querySet_t& in_request,querySet_t& out_result);	
		
		void ODBC_ProcessQueryResultQueue_Query_GolryunBattle_LoadLastBattle(const querySet_t& in_result);
		void ODBC_ProcessQueryResultQueue_Query_GolryunBattle_AddBattle(const querySet_t& in_result);
		void ODBC_ProcessQueryResultQueue_Query_GolryunBattle_LoadTournament(const querySet_t& in_result);
		void ODBC_ProcessQueryResultQueue_Query_GolryunBattle_SaveBattle(const querySet_t& in_result);
		void ODBC_ProcessQueryResultQueue_Query_GolryunBattle_AddTournament(const querySet_t& in_result);
		void ODBC_ProcessQueryResultQueue_Query_GolryunBattle_SaveTournament(const querySet_t& out_result);
		

	
	public:
		CGolryunBattle_DBV2();
		virtual ~CGolryunBattle_DBV2();

};

#endif 
