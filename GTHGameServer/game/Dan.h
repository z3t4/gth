

#ifndef _DAN_H_
#define _DAN_H_																			

#include <LIST>

#define DAN_BATTLEMAP_NO							20
#define DAN_KILL_POINT								1
#define DAN_BATTLE_BROADCAST_TIME					60000
#define SZ_BATTLE_CACHE								24
#define SZ_BATTLE_REQUEST							20

typedef struct BATTLE_RECORD	
{
	int No;
}*LPBATTLE_RECORD;

typedef struct REQUEST_RECORD
{
	int		No;
	int		ServerNo;
	int		ServerID;
	int		DanIdx;
	char	DanName[GUILDNAMESTRING];
	char	MasterName[NAMESTRING];
}*LPREQUEST_RECORD;


enum									
{
	DBTL_STATUS_STARTUP		= 0,		
	DBTL_STATUS_READY,
	DBTL_STATUS_MOVE,
	DBTL_STATUS_START,
	DBTL_STATUS_END,
	DBTL_STATUS_LOOSE_MOVE,
	DBTL_STATUS_CLEAR,
	DBTL_STATUS_EVENT,					
	DBTL_STATUS_MAX
};



typedef class CDanBattleManager
{
	typedef std::list<LPVOID>			BATTLE_LIST;
	typedef BATTLE_LIST::iterator		BATTLE_ITOR;

	BATTLE_LIST			m_BattleCache;								
	BATTLE_LIST			m_BattleResultList;							

	BATTLE_LIST			m_RequestCache;
	BATTLE_LIST			m_RequestList;
	LPREQUEST_RECORD	m_pSuggester;								

	int					m_iBattleStatus;							
	DWORD				m_dwStatusStartTime;
	BATTLE_ITOR			m_FindNextItor;

	REQUEST_RECORD		m_BattleDan[2];
public:
	BOOL				m_bDanTestMode;

public :

	CDanBattleManager()
	{
		
		m_iBattleStatus = 0;
		m_dwStatusStartTime = 0;
		memset(m_BattleDan, 0, sizeof (REQUEST_RECORD) * 2);		
		

		m_bDanTestMode = FALSE;

	}		
	~CDanBattleManager()
	{
	
		Destroy();
	}

protected :
	LPVOID		Get(BATTLE_LIST* pList);
	void		Push(BATTLE_LIST* pList, LPVOID  pVoid);
	LPVOID		ViewFirst(BATTLE_LIST* pList);
	BOOL		First(BATTLE_LIST* pList, LPVOID* lppVoid);
	BOOL		Next(BATTLE_LIST* pList, LPVOID* lppVoid);
	void		Destroy(); 
public :
	BOOL		InitManager();

	void		SetStatus(int bStatus);
	int			GetStatus();
	
	

	void		SendStatus2Sys();								

public:
	BOOL		FindRequester(LPREQUEST_RECORD pRequest);
	LPVOID		GetRequestPointer(LPREQUEST_RECORD pRequest);
	LPVOID		GetRequestPointer();

public:
	
	LPVOID		GetBattleRequestDan();	
	LPVOID		GetBattleSuggestDan();	
	

	
	

public :
	void		SendMessage(i3socket_t* sock,BYTE msgType,BYTE mCode,BYTE mType);
	void		SendRequesterList(i3socket_t* sock,BYTE msgType,int serverID);
	void		BroadCast_RequesterList();
	void		SendBattleDan(i3socket_t* sock,BYTE msgType,int serverID);
	void		BroadCast_BattleDan();

	
	
	void		SendGSM_Refresh_PersonData(playerCharacter_t* pc, int type, int etc1 = 0, int etc2 = 0);
	
	

public :
	void		ClearRequestList();
	
public :
	i3socket_t* FindBattleMapServer();
	BOOL		HaveBattleMap(int serviceCount,int* pMemberServer,int MapNo);

public :
	BOOL		InsertRquester(LPREQUEST_RECORD pRequest);
	BOOL		DeleteRequester(LPREQUEST_RECORD pRequest);
	BOOL		isSuggester(LPREQUEST_RECORD pSuggest);
	BOOL		SetSuggester(LPREQUEST_RECORD pSuggest);
	LPREQUEST_RECORD GetSuggester();
	BOOL		MakeBattleDan();
	void		Update();

	
	void UpdateRequester();
	

}*LPDAN_BATTLE_MANAGER;

	
typedef struct BATTLE_TEAM_INFO
{
	int		danIdx;
	char	DanName[GUILDNAMESTRING];
	int		BattlePoint;
	int		startDanUserCount;
}*LPBATTLE_TEAM_INFO;




typedef class CDanBattleSys
{
	typedef std::list<int>		TEAM_LIST;
	typedef TEAM_LIST::iterator	TEAM_ITOR;

	int				m_iBattleStatus;							
	DWORD			m_dwStatusStartTime;
	DWORD			m_dwBroadCastTime;

	BATTLE_TEAM_INFO	m_BattleTeamInfo[2];

	TEAM_LIST			m_BattleTeam[2];	

public :
	CDanBattleSys()
	{
		
		m_iBattleStatus = 0;
		m_dwStatusStartTime = 0;
		m_dwBroadCastTime = 0;
		memset(m_BattleTeamInfo, 0, sizeof(BATTLE_TEAM_INFO) * 2 );
		
		
		
	}
	~CDanBattleSys(){}

public :
	void		SendBattleResult();
	void		SendBattleStatus();

protected :
	void		InsertTeam(int teamNo,int ServerID);
	void		DeleteTeam(int teamNo,int ServerID);
	void		ClearTeam(int teamNo);
	int			FindMyTeam(const int danIdx) const;

public :
	void		DeleteTeam(playerCharacter_t* pUser);
	void		InsertTeam(playerCharacter_t* pUser);
public :
	void		SetTeamInfo(int teamNo, LPBATTLE_TEAM_INFO pInfo );
	void		IncTeamPoint(int teamNo, int incValue);
	void		DecTeamPoint(int teamNo, int decValue);

public :
	void		CalcPoint(playerCharacter_t* pDead, playerCharacter_t* pKiller);
	void		CalcBattleResult();
public :
	int			GetWinTeam();
	int			GetLooseTeam();

public :
	void		SendBattleMassage(i3socket_t* sock,BYTE msgType,BYTE mCode,BYTE mType);
	void		SendBattleMassage(int teamNo,BYTE msgType,BYTE mCode,BYTE mType);
	void		SendBattleMassage(BYTE msgType,BYTE mCode,BYTE mType);
	void		BroadCast_BattleInfo();
	void		SendBattleInfo();
	void		SendBattleStart();
	void		SendBattleReward(i3socket_t* sock,int type,int danFame,int userFame);
	


public :
	BOOL		InitDanBattleSys();
	void		SetStatus(int iStatus);
	int			GetStatus();
	
	DWORD		GetStatusStartTime();		
	void		Update();
	void		UpdateStatus();

protected :
	int			CountLiveUser(int teamNo);
	
public:
	void		MakeDanBattlePortal(int);
	void		AutoMoveDanMember();	
	
	void		TakeConsideration(playerCharacter_t *pc, int considerTableIdx);	

	void		SendRemainTime();

	


}*LPDAN_BATTLE_SYS;




class CDanBattleFunc
{
public :
	CDanBattleFunc(){}
	~CDanBattleFunc(){}

public :
	void RecvCC_DanBattleMessage();				

	
	
	

	
	int	 RecvCC_ReqeustBattle(playerCharacter_t* pc = NULL);	

	
	void RecvCC_BattleReady();					
	void SendGSM_BattleReady(i3socket_t* sock);
												
	

	void SendGSM_SuggestResult(BYTE isAllow);

	void SendGSM_RequestBattle();
	void RecvCC_SetStatus();
	void SendGSM_SetStatus();
	void RecvCC_SuggestBattle();	
	void SendGSM_SuggestBattle();				
	void RecvCC_SuggestResult();
	void RecvCC_ChangeBattleMap();	
	void RecvCC_Battle_Start();					
	void RecvCC_CancelRequest();	
	void RecvCC_InitChao();
	void RecvCC_SetDanTestMode();
public :
	void RecvGSM_DanBattleMessage();			

	
	void RecvGSM_RequestBattle();
	void RecvGSM_SetStatus();

public :
	void SendErrorCode(i3socket_t* sock,BYTE msgType,BYTE eCode,BYTE eType);

	
	int	CountDanUser(int danIdx);	
	
		

};


#endif