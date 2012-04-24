#define COLUMNSIZE 200


#define OUTCONNECTIONSIZE 1024
#define MAXNOTICE 5
#define MAX_QUERY_PER_FRAME	200



#include "..\GolryunBattle\CGolryunBattle_Process.h"
#include "tagDataBase.h"
#include "..\Terra\GiftCard\CGiftCard.h"

#define MAX_QUERY_NUMBER  1000


#include "tagQuerySet.h"


class CDBServer
{

protected:
	RETCODE m_rCode;
	HENV m_hEnv;
	HDBC m_hDbc;
	HSTMT m_hStmt;

public:
	CDBServer();
	~CDBServer();

	int Initialize(char *DSN, int DSNType); 
							 
	void Close();

	void PrintError ( SQLSMALLINT handleType = SQL_HANDLE_STMT ) ;
	void PrintError ( HSTMT hstmt );
	void PrintError(const querySet_t& request);

	HSTMT GetStmt () { return m_hStmt ; }
	HDBC GethDBC() { return m_hDbc; }

	char *ReturnString();

	char m_query[QUERYSIZE+1];
};

class CDBAccount : public CDBServer
{
public:
	CDBAccount();
	~CDBAccount();

	
	void InsertLogin(playerCharacter_t* pPlayer);
	int DeleteLogin(playerCharacter_t *pc);
	int DeleteLoginHost( char *hostName );

};


enum enumQUERY_SAVE_GAMEparameter{
	normal=0,
	force,
};

class CDBGame : public CDBServer
{
public:
	CDBGame();
	~CDBGame();


public:
	void LoadServerStatus(void);
	void SaveServerStatus(const BOOL bPK);

public:
	void SaveGiftCard(const char* szpGiftCardSerial,const char* szpGiveItemList,const char* szpPlayerAccountID);
	void LoadGiftCard(const playerCharacter_t* pPlayer);
	void LoadGiftCardItembytype(const playerCharacter_t* pPlayer,const int type,const int Cnt);


public:
	void GolryunBattle_SaveBattleInfo(
			playerCharacter_t* pPlayer,
			const int iBattleGUID,
			const int TournamentLevelIdx,
			enum playerCharacter_t::enumNameColor enumManor);


public:
	void LoadPremiumInfo(playerCharacter_t *pc);
	void SavePremiumInfo(playerCharacter_t* pPlayer);


public:
	BOOL PostSystem_LoadPackageCnt(playerCharacter_t* pPc);


public:	
	void LoadGame(playerCharacter_t* pPlayer);
	int LoadItem(playerCharacter_t *pc);
	void LoadQuest(playerCharacter_t* pPlayer);
	void LoadQuestStatus(playerCharacter_t* pPlayer);
	int LoadSkill(playerCharacter_t *pc);
	int LoadFriends (playerCharacter_t *pc) ;

	
	void SaveGame(
			playerCharacter_t *pc,
			enum enumWhySaveGame why,			
			enum enumQUERY_SAVE_GAMEparameter parameter);

	int SaveQuest(playerCharacter_t *pc);
	int SaveSkill(playerCharacter_t *pc);

	int AddItem(playerCharacter_t *pc, item_t *item, int idx, char *generateID);
	int RemoveItem(playerCharacter_t *pc, int idx);
	int SaveItem(playerCharacter_t *pc, item_t *item, int idx);

	int AddQuestStatus(playerCharacter_t *pc, questStatus_t *questStatus);
	int RemoveQuestStatus(playerCharacter_t *pc,int idx);
	int SaveQuestStatus(playerCharacter_t *pc, questStatus_t *questStatus);

	int SaveFriends ( playerCharacter_t * pc );

	
	void SaveHelpList(const playerCharacter_t* pPlayer);
	void LoadHelpList(const playerCharacter_t* pPlayer);	
	void SaveWepPoint(const playerCharacter_t* pPlayer,const int WepPoint);
	

	
	
	int AddGuild( short guildIdx );
	int SaveGuild( short guildIdx );
	
	
	int BanishMemberGuild(const char* szpName,const int decRankPoint);

	

	

	
	
	int SendMail( playerCharacter_t *pc, char* toName, char* mailTitle, char *sendTime, char* mailStr );
	int RecvMail( playerCharacter_t *pc );
	int GetMailState( playerCharacter_t *pc );
	int DeleteMail( playerCharacter_t *pc, int mailIdx[] );
	int ViewMail( playerCharacter_t *pc, int mailIdx );
	int SaveMail( playerCharacter_t *pc, int mailIdx[] );

	

	
	
	
	int	Send_Save_EventCraftItem( playerCharacter_t *pc, int _Point);
	int	Recv_Load_EventCraftItem( playerCharacter_t *pc,int nEventIndex );
	
	
	
	
	
	
	int SaveSkin(playerCharacter_t *pc);
	
	
	int LoadGonryunBattle(playerCharacter_t *pc);
	int SaveGonryunBattle(playerCharacter_t *pc);
	
	BOOL UpdateGuildIdx(const char* pUserID,const int ServerGroupIdx,const int CharactorSlotIdx,const int GuildIdx);
	BOOL RemoveGuild(const int guildIdx,const int serverGroupIdx);
	BOOL LoadGuild(const int serverGroupIdx);
	BOOL SaveGuildMark(const int guildIdx);
	

	
	
	int LoadPackageList(playerCharacter_t *pc);	
	int DeletePost( playerCharacter_t *pc, int PostType, int PostIdx );
	
	void DeletePostItems(const playerCharacter_t* pPlayer,const int PostSendType,const int PostIdx);
	int OpenPost( playerCharacter_t *pc, int PostType, int PostIdx );	
	
	void SavePackage( playerCharacter_t* pPlayer, char* title, char* message, char* toname, int nak);	

	void SaveChaPackageListBySystem(
			const char* pFromName,
			const char* pToName,
			const char* title, 
			const char* message, 
			const int   nak);

	


	void SavePost(playerCharacter_t *pPlayer,const int packageidx,
		const int itempos,
		const char* generateID,
		const char* toname,
		item_t* pitem);
	

	
	int SaveChaPackageNak(playerCharacter_t *pc, CPostManager::PostPackage_t* pPostPackage);

	
	void Insert_Danbattle(int nDanBattleIdx, int nRequestIdx, int AcceptIdx);
	void Delete_Danbattle(int nRequestIdx, int nAcceptIdx);
	void Delete_Alldanbattle(int MemberServerIdx);
	void Save_danbattle_guild_AddWinpoint(const int nGuildIdx);
	void Save_danbattle_guild_AddLosepoint(const int nGuildIdx);
	void Save_danbattle_guild_AddAccpoint(const int nGuildIdx, int nAccPoint);
	void Load_danbattle_guild_Info(const int nGuildIdx);
	void Save_danbattle_pcpoint(playerCharacter_t *pc, int nPoint);


};






enum
{
	QUERY_INSERT_LOGIN,
	QUERY_DELETE_LOGIN,
	QUERY_DELETE_LOGIN_HOST,

	QUERY_LOAD_CHARACTER,
	QUERY_LOAD_QUESTSTATUS,
	QUERY_LOAD_QUEST,	
	QUERY_LOAD_SKILL,
	QUERY_LOAD_ITEM,
	QUERY_LOAD_FRIENDS,

	QUERY_SAVE_GAME,
	QUERY_SAVE_SKILL,
	QUERY_SAVE_QUEST,	

	QUERY_ADD_QUESTSTATUS,
	QUERY_REMOVE_QUESTSTATUS,
	QUERY_SAVE_QUESTSTATUS,

	QUERY_ADD_ITEM,
	QUERY_REMOVE_ITEM,
	QUERY_SAVE_ITEM,
	QUERY_SAVE_FRIENDS,
	
	QUERY_ADD_GUILD,
	QUERY_LOAD_GUILD,
	QUERY_SAVE_GUILD,
	QUERY_SAVE_GUILDMARK,
	QUERY_REMOVE_GUILD,
	QUERY_BANISHMEMBER_GUILD,

	
	QUERY_SEND_MAIL,
	QUERY_RECV_MAIL,
	QUERY_GET_MAILSTATE,
	QUERY_DELETE_MAIL,
	QUERY_VIEW_MAIL,
	QUERY_SAVE_MAIL,
	
	
	
	
	
	
	QUERY_SAVE_SKIN,
	

	
	QUERY_SAVE_EVENTCRAFTITEM,
	QUERY_LOAD_EVENTCRAFTITEM,
	

	
	QUERY_UPDATE_GUILDIDX,
	
	
	QUERY_LOAD_GONRYUNBATTLE,
	QUERY_SAVE_GONRYUNBATTLE,

	
	QUERY_LOAD_PACKAGELIST,
	QUERY_SEND_POST,	
	QUERY_DELETE_POST,
	QUERY_OPEN_POST,	
	QUERY_DELETE_POSTITEMS,
	QUERY_SAVECHA_PACKAGE,
	QUERY_SAVECHA_ITEM_DETAIL,
	
	

	QUERY_LOAD_PREMIUMINFO,
	QUERY_SAVE_PREMIUMINFO,
	QUERY_SAVECHAPACKAGELIST_BY_SYTEM,

	
	QUERY_SAVECHA_PACKAGENAK,
	
	QUERY_POSTSYSTEM_LOADPACKAGECNT,

	QUERY_GolryunBattle_SaveBattleInfo,

	
	QUERY_SAVE_HELPLIST,
	QUERY_LOAD_HELPLIST,
	QUERY_SAVE_WEBPOINT,
	
	
	QUERY_GIFTCARD_LoadGiftCard,
	QUERY_GIFTCARD_LoadGiftCardItembytype,
	QUERY_GIFTCARD_SaveGiftCard,

	QUERY_LOADSERVERSTATUS,
	QUERY_SAVESERVERSTATUS,

	Query_GolryunBattle_LoadLastBattle,
	Query_GolryunBattle_AddBattle,
	Query_GolryunBattle_LoadTournament,
	Query_GolryunBattle_SaveBattle,
	Query_GolryunBattle_AddTournament,
	Query_GolryunBattle_SaveTournament,
	

	
	QUERY_INSERT_DANBATTLE,
	QUERY_DELETE_DANBATTLE,
	QUERY_DELETE_ALLDANBATTLE,
	QUERY_SAVE_DANBATTLE_GUILD_ADD_WINPOINT,
	QUERY_SAVE_DANBATTLE_GUILD_ADD_LOSEPOINT,
	QUERY_SAVE_DANBATTLE_GUILD_ADD_ACCPOINT,
	QUERY_LOAD_DANBATTLE_GUILD_INFO,
	QUERY_SAVE_DANBATTLE_PCPOINT,
	NUM_QUERY_TYPE,
};










class CQueryQueue
{
private:
	int m_first;
	int m_last;
	int m_size;
	int m_overflow;

	querySet_t m_queue[MAX_QUERY_NUMBER];

public:
	int m_count;
	int m_maxCount;

	CQueryQueue();
	~CQueryQueue();

	int Push (querySet_t *qSet);
	int Pop (querySet_t *qSet);
	void Clear ();
};



int DB_CreateQueryExecuterThread();
void DB_DestroyQueryExecuterThread();
DWORD WINAPI DB_QueryExecuterThread(LPVOID param);
void DB_ProcessQueryResultQueue(void);


void DB_ResultQuery_LoadCharacter(querySet_t *request, querySet_t *result );
void DB_ResultQuery_LoadItem(querySet_t *request, querySet_t *result );
void DB_ResultQuery_LoadQuest(querySet_t *request, querySet_t *result );
void DB_ResultQuery_LoadQuestStatus(querySet_t *request, querySet_t *result );
void DB_ResultQuery_LoadSkill( querySet_t *request, querySet_t *result );
void DB_ResultQuery_LoadFriends ( querySet_t *request, querySet_t *result ) ;
void DB_ResultQuery_LoadGuild(querySet_t *request, querySet_t *result);

void DB_ResultQuery_LoadChattingBan(querySet_t *request, querySet_t *result);




void DB_ResultQuery_LoadGonryunBattle(querySet_t *request, querySet_t *result);

void DB_ResultQuery_LoadHelpList(const querySet_t& request, querySet_t& result);




void DB_ResultQuery_SendMail(querySet_t *request, querySet_t *result);
void DB_ResultQuery_RecvMail(querySet_t *request, querySet_t *result);
void DB_ResultQuery_GetMailState(querySet_t *request, querySet_t *result);
void DB_ResultQuery_DeleteMail(querySet_t *request, querySet_t *result);
void DB_ResultQuery_ViewMail(querySet_t *request, querySet_t *result);
void DB_ResultQuery_SaveMail(querySet_t *request, querySet_t *result);


void DB_ResultQuery_Save_EventCraftItem(querySet_t *request, querySet_t *result);
void DB_ResultQuery_Load_EventCraftItem(querySet_t *request, querySet_t *result);


enum
{
	SAVEGAMEDATA_CONTINUE = 0,	
	SAVEGAMEDATA_QUIT,			
};


void DB_SaveGameData( 
	playerCharacter_t *pc,
	int quit,
	const enum enumWhySaveGame why=unknown,
	const BOOL bForceQuit_fromGM=FALSE);

void DB_LoadGameData( playerCharacter_t *pc );

void DB_Waiting_ProcessRemainderQuery();


bool DB_ReconnectAccount ( querySet_t & request ) ;
bool DB_ReconnectGame ( querySet_t & request ) ;
bool DB_Reconnect ( querySet_t & request ) ;
bool CheckDisconnect ( HSTMT hStmt ) ;





void DB_ResultQuery_LoadWebPackageList(const querySet_t& request,querySet_t& result);
BOOL DB_ResultQuery_DeletePost( querySet_t *request, querySet_t *result );			
BOOL DB_ResultQuery_OpenPost( querySet_t *request, querySet_t *result );			


void DB_ProcessQueryResultQueue_QUERY_LOAD_PACKAGELIST(querySet_t& result);

void DB_ProcessQueryResultQueue_QUERY_LOAD_PACKAGELIST(querySet_t& result);
void DB_ProcessQueryResultQueue_QUERY_DELETE_POST(querySet_t& result);



void DB_ProcessQueryResultQueue_QUERY_INSERT_DANBATTLE(querySet_t& result);
void DB_QueryExecuterThread_INSERT_DANBATTLE(const querySet_t& request,querySet_t& result);

void DB_QueryExecuterThread_DELETE_ALLDANBATTLE(const querySet_t& request,querySet_t& result);
void DB_ProcessQueryResultQueue_DELETE_ALLDANBATTLE(querySet_t& result);
void DB_QueryExecuterThread_DELETE_DANBATTLE(const querySet_t& request,querySet_t& result);
void DB_ProcessQueryResultQueue_DELETE_DANBATTLE(querySet_t& result);





void DB_QueryExecuterThread_Query_LOAD_DANBATTLE_GUILD_INFO(const querySet_t& request,querySet_t& result);
void DB_ProcessQueryResultQueue_LOAD_DANBATTLE_GUILD_INFO(querySet_t& result);
void DB_QueryExecuterThread_Query_SAVE_DANBATTLE_PCPOINT(const querySet_t& request,querySet_t& result);
