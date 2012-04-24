



#if !defined(AFX_NEWDANBATTLESYSTEM_H__12E6C569_671E_44EE_93DD_EE548AC47392__INCLUDED_)
#define AFX_NEWDANBATTLESYSTEM_H__12E6C569_671E_44EE_93DD_EE548AC47392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  _MSC_VER > 1000


class NewDanBattleSystem
{
public:

	struct DanBaseInfo
	{
		int		nDanIdx;				
		char	szGuildName[GUILDNAMESTRING+1];
		int		nKillPoint;				
		int		nRemainPoint;			
		int		nRequest_AcceptPcIdx;	
		__int64	nRequest_AcceptPcEXP;	

		tagGCPacket_NewDanBattle::enumwincode wincode;
		struct sDanBattleMember
		{
			char	name[NAMESTRING+1];
			int		PcIdx;
			int		killPoint;		    
			int		nAccumulationBattleKillPoint; 
			__int64 exp;				
		};

		std::vector<sDanBattleMember> v_GuildMembers;
	};


	struct DanBattleInfo
	{	
		
		enum enumBattleMode
		{
			BATTLEMODE_FRIENDSHIP,
			BATTLEMODE_GENERAL,
			
		};

		enum enumBattleStatus
		{
			BATTLESTATUS_NONE,
			BATTLESTATUS_REGIST,
			BATTLESTATUS_READY,
			BATTLESTATUS_BATTLE,
			BATTLESTATUS_END,
		};

		enum enumEndType
		{
			REMAIN_MEMBER_COUNT =0 ,
			TIME_OUT,
			SYSTEM,
		};

		enum enumBattleMode		BattleMode;	
		enum enumBattleStatus	BattleStatus;

		struct DanBaseInfo		RequestDanInfo;			
		struct DanBaseInfo		AcceptDanInfo;			

		

		float					fRemainTime;			

		int						nBattleMapIdx;
		float					fItemDropRate;			
		int						nDanBattleIdx;
		float					fWaitTimeforReady;		
		float					fBattleSec;				
		float					fWaitTimeforEnd;		
		float					fNextSyncdTime;			
		
		float					fMinBattleProcessTime;	

		enumEndType				endType;				

		
		BOOL					bPlunderPremiumItem;

		int						ServerGroupIdx;
		char					szBestID[NAMESTRING];
	};	

	enum noticetype
	{
		notice =0,
		killpoint_notice,
		plunder_notice,
		max_notice,
	};

public:
	enum
	{
		MIN_GUILD_MEMBER_NUMBER_FOR_DAN_BATTLE = 3,
		MAX_DANBATTLE_COUNT		= 500,
		SYNC_TIME				= 60000 * 5, 
		POWE_END_WAIT_SEC		= 60000,	 
		
		
		MIN_BATTLE_PROCESS_SEC	= 600000,	 
		
	};

public:
	BOOL						m_bActive;
	int							m_nMinPcCount;			
	int							m_nBattleTimeForMin1;	
	int							m_nBattleTimeForMin2;	
	float						m_fItemDropRate;		

	float						m_WaitTimeforReady;		
	float						m_WaitTimeforEnd;		

	

protected:
	
	DanBattleInfo				m_DanBattleInfo[MAX_DANBATTLE_COUNT];	
	

public:
	
	
	

	
	void						Update();
	
	void						BattleReady(DanBattleInfo* pDanBattleInfo);
	
	void						Battle(DanBattleInfo* pDanBattleInfo);
	
	void						BattleEnd(DanBattleInfo* pDanBattleInfo);
	
	void						SetDanBatttleStatus(DanBattleInfo* pDanBattleInfo, DanBattleInfo::enumBattleStatus status);

	
	
	

	
	DanBattleInfo*				GetEmptyDanBattleSlot(int& nIdx_OUT);
	
	void						InitDanBatlleInfo(DanBattleInfo* pDanBattleInfo);
	
	DanBattleInfo*				InsertDanBattle(int RequestDanIdx, 
												int RequestPcIdx, 
												std::vector<DanBaseInfo::sDanBattleMember>& vRequestGuildMember,
												int AcceptDanIdx, 
												int AcceptPcIdx, 
												std::vector<DanBaseInfo::sDanBattleMember>& vAcceptGuildMember,
												enum NewDanBattleSystem::DanBattleInfo::enumBattleMode battlemode, 
												int WorldIdx, 
												int BattleTIme ,
												__int64 RequestPcExp,
												__int64 AcceptPcExp,
												BOOL IsAllowPrimiumItemPlunder);
	
	BOOL						InsertDanBattleMemeber(int DanBattleIdx, int GuildIdx, int Pcidx);
	

	
	
	
	DanBattleInfo*				GetDanBattleInfo(const int nGuildIdx);
	DanBattleInfo*				GetDanBattleInfo(const char* szGuildName);
	DanBattleInfo*				GetDanBattleInfo(const int nBattleIdx, const int nGuildIdx);
	DanBattleInfo*				GetDanBattleInfo(const int nBattleIdx, DanBattleInfo::enumBattleStatus status);
	DanBaseInfo*				GetEnemyDanBaseInfo(const int nBattleIdx, const int nGuildIdx);	
	DanBaseInfo*				GetDanBaseInfo(DanBattleInfo* pDanBattleInfo, const int nGuildIdx);
	DanBaseInfo::sDanBattleMember*GetDanBattleMember(DanBattleInfo* pDanBattleInfo, int GuildIdx, char* pcname);

	
	
	
	
	
	BOOL						IsDanBattleEnemy(int myPcIdx, int EnemyIdx);	

	
	BOOL						IsInsertdDanBattle(int GuildIdx);	

	
	BOOL						DecsionDanBattleWinTO_LiveMemberNumber(DanBattleInfo* pDanBattleInfo);
	
	BOOL						DecsionDanBattleWinTO_TimeOut(DanBattleInfo* pDanBattleInfo);
	
	void						DecsionDanBattleBestMember(DanBattleInfo* pDanBattleInfo);
	void						RemainTimeBroadCastMsg(DanBattleInfo* pDanBattleInfo);


public:
	NewDanBattleSystem();
	virtual ~NewDanBattleSystem();

};

#endif 
