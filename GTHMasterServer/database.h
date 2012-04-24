#define COLUMNSIZE 200
#define QUERYSIZE 4096
#define OUTCONNECTIONSIZE 1024
#define MAXNOTICE 5
#define MAX_QUERY_PER_FRAME		50

typedef struct
{
	char date[11]; 
	char content[256];
} i3notice_t;


class CDBServer
{

protected:
	RETCODE m_rCode;
	HENV m_hEnv;
	HDBC m_hDbc;
	HSTMT m_hStmt;
	char m_query[QUERYSIZE+1];

public:
	CDBServer();
	~CDBServer();

	int Initialize(char *DSN, int DSNType); 
							 
	int Initialize(char *DSN, int DSNType, char *UID, char *PWD);

	void PrintError ( SQLSMALLINT handleType = SQL_HANDLE_STMT ) ;


	HSTMT GetStmt () { return m_hStmt ; }

	void Close();
	char *ReturnString();
};


class CDBAccount : public CDBServer
{
	public:
	CDBAccount();
	~CDBAccount();

	
	public:
		void MalCheckAccount(i3client_t* pClient);
		void CheckAccount_Monitor(i3client_t* pClient,const char* szpID,const char* szpPassword);

	public:
	void SaveServerStatus(
			const int   iServerGroupIdx,
			const char* szpServerGroupName,
			const int   iAcceptPlayerCnt);

	void SaveCharactorPostion(const i3client_t* pClient,const int CharacterID);
	int CheckAccount(i3client_t *client, char *id, char *password); 
	
	void CheckAccountHicocoon(i3client_t *client, char *id); 
	
	int CheckUserID(i3client_t *client); 

	int InsertLogin(i3client_t *client);
	int DeleteLogin(i3client_t *client); 
	int DeleteLoginHost( char *hostName ); 

	
	int InsertServerGroupUsers(int serverGroupID, int currentUsers, int maxUsers);

};


class CDBGame : public CDBServer
{
public:
	CDBGame();
	~CDBGame();



	

	void LoadPremiumInfo(i3client_t* pClient);
	int CheckCharacterName(i3client_t *client, int serverGroupID, char *charName);

	int LoadAllGame(i3client_t *client);
	int LoadAllItem(i3client_t *client);
	int LoadAllQuest(i3client_t *client);
	int LoadAllQuestStatus(i3client_t *client);
	int LoadAllSkill(i3client_t *client);

	int DeleteCharacter(i3client_t *client, int characterID);
	int CreateCharacter(i3client_t *client);

	int UpdateGameOwnItemInfo(i3client_t *client, characterData_t *data);

	
	int DeleteItem (i3client_t *client, int characterID, int idx) ;

	
	int	LoadCharacterSlot(void);
	int	SaveCharacterSlot(i3client_t* pClient,int type);
};



class CTerraDBCtrl : public CDBServer
{
	public:
		void SelectTmember(i3client_t* pClient,const char* pTID); 
	public:
		CTerraDBCtrl();
		~CTerraDBCtrl();
};








enum
{
	QUERY_CHECK_ACCOUNT,
	QUERY_CHECK_USERID,
	QUERY_INSERT_LOGIN,
	QUERY_DELETE_LOGIN,
	QUERY_DELETE_LOGIN_HOST,

	QUERY_CHECK_CHARACTER_NAME,

	QUERY_LOAD_ALL_GAME,
	QUERY_LOAD_ALL_SKILL,
	QUERY_LOAD_ALL_ITEM,
	QUERY_LOAD_ALL_QUEST,
	QUERY_LOAD_ALL_QUESTSTATUS,

	QUERY_CREATE_CHARACTER,
	QUERY_DELETE_CHARACTER,

	QUERY_UPDATE_GAMEOWNITEMINFO,
	QUERY_INSERT_SERVER_GROUP_USERS,

	QUERY_DELETE_ITEM,

	
	QUERY_LOAD_CHARACTER_SLOT,
	QUERY_SAVE_CHARACTER_SLOT,

	QUERY_LOAD_PREMIUMINFO,
	QUERY_SAVE_CHARACTOR_POSTION,
	QUERY_SAVESERVERSTATUS,

	QUERY_TERRA_CHECK,

	
	QUERY_CHECK_ACCOUNT_HICOCOON,
};



enum
{
	
	LOGIN_CHAR_FREE_NEW=0,
	LOGIN_CHAR_FREE_LEFT,
	LOGIN_CHAR_FREE_CENTER,
	LOGIN_CHAR_FREE_RIGHT,
	
	
	LOGIN_CHAR_PU_NEW=10,
	LOGIN_CHAR_PU,	
};



#define MAX_QUERY_NUMBER  1000

typedef struct
{
	public:
		struct tagCheckAccountVar{
			struct i3client_t::tagIdentification Identity;
			int	dbCheckAccount;
		};

		struct tagPremiumVar{
			public:
				struct tagWorldChatting{
					long lDate;
					int  iDecreaseCount;
				};

			public:
				enum i3client_t::tagPremiumInfo::enumMeberShipType iMemberShipType;
				int  iRemainSecond;
				struct tagWorldChatting WorldChatting;
		};

		struct tagSaveCharactorPosition{
			int CharacterID;
		};

		struct tagLoadCharaterSlot{
			int				iCharacterSlotType;
			struct tm		tmDate;
		};

		struct tagQUERY_TERRA_CHECK{
			char TLoginName[i3client_t::tagTerra::MaxBytesTLoginName+1];
		};

	public:
		int		ownerIdx;	
		char	ownerID[IDSTRING+1];
		int		queryType;
		char	query[QUERYSIZE+1];
		HSTMT	hStmt;
		RETCODE	retCode;
		void	*retStructVariable;
		union{
			struct tagCheckAccountVar		CheckAccountVar;			
			struct tagPremiumVar			PremiumVar;
			struct tagSaveCharactorPosition SaveCharactorPosition;
			struct tagLoadCharaterSlot	    LoadCharaterSlot;
			struct tagQUERY_TERRA_CHECK     TerraCheck;
		};
} querySet_t;




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
void DB_ProcessQueryResultQueue();


void DB_ResultQuery_CheckAccount(const querySet_t& request,querySet_t& result);
void DB_ResultQuery_CheckUserID(querySet_t *request, querySet_t *result );
void DB_ResultQuery_LoadAllGame(querySet_t *request, querySet_t *result);
void DB_ResultQuery_LoadAllItem(querySet_t *request, querySet_t *result);
void DB_ResultQuery_CheckCharacterName(querySet_t *request, querySet_t *result);
void DB_ResultQuery_LoadAllSkill(querySet_t *request, querySet_t *result);
void DB_ResultQuery_LoadAllQuest(querySet_t *request, querySet_t *result);
void DB_ResultQuery_LoadAllQuestStatus(querySet_t *request, querySet_t *result);


void DB_ResultQuery_LoadCharaterSlot(querySet_t& request,querySet_t& result);

void DB_Waiting_ProcessRemainderQuery();


bool DB_ReconnectAccount ( querySet_t & request ) ;
bool DB_ReconnectGame ( querySet_t & request ) ;
bool DB_Reconnect ( querySet_t & request ) ;
bool CheckDisconnect ( HSTMT hStmt ) ;
