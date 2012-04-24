
#define MAX_SERVER_GROUP				20
#define MAX_MEMBER_SERVER				10
#define MAX_SERVICE_WORLD				10
#define MAX_SERVER_GROUP_PER_MESSAGE	5



#define MAX_CONNECTION_IP				512
enum
{
	ConnectionReject		=	0,
	ConnectionAccept,
	ConnectionFilterTest,
};

enum
{
	GTH_LANGUAGE_TYPE_KOREAN	=	0,
	GTH_LANGUAGE_TYPE_ENGLISH,
	GTH_LANGUAGE_TYPE_JAPANESE,
	GTH_LANGUAGE_TYPE_CHINESE,
};

typedef struct {
	int		serverNo;
	char	serviceIP[IPSTRING];
	int		servicePort;
	char	localIP[IPSTRING];
	int		localPort;
	int		serviceServerGroupNumber;
	int		odbcDSNType;

	
	char	accountDatabaseDSN[DATABASESTRING];
	char	gameDatabaseDSN[MAX_SERVER_GROUP][DATABASESTRING];
	char	gameVersion[VERSIONSTRING];
	int		createStatistics;
	int		updateStatisticsTime;
	int		maxServiceClientNumber;
	int		maxServerGroupServiceClientNumber;
	char	monitorVersion[VERSIONSTRING];
	int		createLog;
	int		createDebug;
	int		languageType;

	
	
	char	acceptIP[MAX_CONNECTION_IP][IPSTRING];
	char	rejectIP[MAX_CONNECTION_IP][IPSTRING];

	
	char	billingServerIP[IPSTRING];
	int		billingServerPort;
	BOOL	bCheckBilling;									
	BOOL    bBlockNotRealNameUser;

	struct tagTerraV2{
		BOOL	bActive;
		BOOL	bBilling;
		char    MemberShipDSN[DATABASESTRING+1];
	}TerraV2;

	struct tagHiCocoon{
		BOOL	bActive;
		BOOL	bBilling;
		char    MemberShipDSN[DATABASESTRING+1];
	}Hicocoon;
	

	
} i3masterServerConfig_t;



ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int GTH_ParseMasterServerConfig();

void GTH_InitGlobalTime();
unsigned int GTH_GetGlobalTime();
void GTH_UpdateFrameRate();
void GTH_InitGameServerGroup();
void GTH_CheckProblemServerGroup();
int GTH_Load_Connection_Filtering();
void GTH_UpdateTimer();
void GTH_UpdateStatistics();
char* GTH_Debug_ServerGroupInfo(char *msg);


int GTH_ParseIPList();
int CheckConnectionIP( char *ipStr );
