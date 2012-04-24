#include "Global.h"






HINSTANCE hInst;								
TCHAR* g_strAppTitle = TEXT( "GTH Master Server Test Ver.06-04-13" );	
TCHAR* g_strWindowClass = TEXT( "MS" );			
TCHAR*  g_systemStatusMessage = TEXT( "MS: Shut down State" );



HWND g_hWnd;
i3notice_t g_notice[MAXNOTICE];
i3masterServerConfig_t g_config;
int g_systemOn;

i3client_t g_clients[MAX_CLIENTS];
i3client_t *g_curClient;
int g_clientNumber;         
int g_maxClientNumber;


CTimer *g_timer;


CDBAccount*   g_DBAccountServer=NULL; 
CTerraDBCtrl* g_TerraDBCtrl=NULL;
CDBGame*      g_DBGameServer[MAX_SERVER_GROUP];


CLogSystem *g_logSystem;

unsigned int g_globalTime; 

serverGroup_t	g_serverGroup[MAX_SERVER_GROUP];

characterTable_t	g_characterTable[NUM_TABLE_GEN_TYPE];
int					g_numCharacterTable;


int g_filterYn;		
int g_filterIdx;
char g_filterMessage[MAX_FILTER_MESSAGE][256];


masterServerInfo_t	g_masterServerInfo[MAX_MASTER_SERVER];
serverGroupInfo_t	g_serverGroupInfo[MAX_SERVER_GROUP];


char g_hostName[HOSTNAMESTRING];


int g_concurrentClients;
int g_maxConcurrentClients;


tcpSocket				*g_pBillingSocket ;					


void GTH_InitGlobalVariable()
{
	int i;
	
	g_systemOn = false;
	g_filterYn = 1;
	g_concurrentClients = 0;
	g_maxConcurrentClients = 0;
	g_filterIdx = 0;

	
	if ( g_timer ) { delete g_timer; g_timer = NULL; }
	g_timer = new CTimer;

	
	if ( g_DBAccountServer ) { delete g_DBAccountServer; g_DBAccountServer = NULL; }
	g_DBAccountServer = new CDBAccount;



	


	
	for (i=0; i<MAX_SERVER_GROUP; i++)
	{
		if ( g_DBGameServer[i] ) { delete g_DBGameServer[i]; g_DBGameServer[i] = NULL; }
		g_DBGameServer[i] = new CDBGame;
	}

	
	if ( g_logSystem )	{ delete g_logSystem; g_logSystem = NULL; }
	g_logSystem = new CLogSystem;
}

void GTH_DestroyGlobalVariable()
{
	int i;

	if ( g_timer ) { delete g_timer; g_timer = NULL; }
	if(NULL != g_DBAccountServer )	{ delete g_DBAccountServer; g_DBAccountServer = NULL; }


	


	for (i=0; i<MAX_SERVER_GROUP; i++)
	{
		if(NULL != g_DBGameServer[i]){ delete g_DBGameServer[i]; g_DBGameServer[i] = NULL; }
	}
	if(NULL != g_logSystem )	{ delete g_logSystem; g_logSystem = NULL; }

	
	if (NULL != g_pBillingSocket )
	{
		delete g_pBillingSocket;
		g_pBillingSocket = NULL;
	}
}



