



#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <time.h>


#include <winsock2.h>
#include <wsipx.h>



#include <sql.h>
#include <sqlext.h>



#define SERVERSTRING			20
#define DATABASESTRING			64
#define IPSTRING				16
#define PORTSTRING				 8
#define VERSIONSTRING			20 
#define IDSTRING				20
#define PASSWORDSTRING			100

#define USERNAMESTRING			20
#define JUMINSTRING				14
#define ADDRESSSTRING			50
#define EMAILSTRING				30
#define TELEPHONESTRING			16

#define NAMESTRING				20
#define PATHSTRING				64

#define HOSTNAMESTRING			64


#define MAX_FILTER_MESSAGE		3


#include "resource.h"
#include "winmain.h"
#include "etc/token.h"
#include "etc/math.h"
#include "etc/timer.h"
#include "dataStruct.h"
#include "netlib/netlib.h"
#include "netlib/queue.h"
#include "network/protocol.h"
#include "network/net_main.h"
#include "monitor.h"
#include "Log\CLogSystem.h"

#include "database.h"
#include "logic.h"

#include "Billing/billing.h"
#include "Billing/tcpSocket.h"


#include "game/GameConfigFlag.h"
#include "etc/Days.h"



extern HINSTANCE hInst;								
extern TCHAR* g_strAppTitle;					
extern TCHAR* g_strWindowClass;			
extern TCHAR* g_systemStatusMessage;



extern HWND g_hWnd;



extern i3notice_t g_notice[MAXNOTICE];
extern i3masterServerConfig_t g_config;

extern int g_systemOn;

extern i3client_t g_clients[MAX_CLIENTS];
extern i3client_t *g_curClient;
extern int g_clientNumber;         
extern int g_maxClientNumber;

extern CTimer *g_timer;


extern CDBAccount*   g_DBAccountServer; 
extern CTerraDBCtrl* g_TerraDBCtrl;


extern CDBGame*      g_DBGameServer[MAX_SERVER_GROUP];


extern CLogSystem *g_logSystem;

extern unsigned int g_globalTime;

extern serverGroup_t	g_serverGroup[MAX_SERVER_GROUP];


extern characterTable_t	g_characterTable[NUM_TABLE_GEN_TYPE];
extern int					g_numCharacterTable;

extern int g_filterYn;		
extern int g_filterIdx;
extern char g_filterMessage[MAX_FILTER_MESSAGE][256];


extern masterServerInfo_t	g_masterServerInfo[MAX_MASTER_SERVER];
extern serverGroupInfo_t	g_serverGroupInfo[MAX_SERVER_GROUP];


extern char g_hostName[HOSTNAMESTRING];


extern int g_concurrentClients;
extern int g_maxConcurrentClients;



void GTH_InitGlobalVariable();
void GTH_DestroyGlobalVariable();


extern tcpSocket	*g_pBillingSocket ;

//lucky 2012 network between client / ms and GS for local server on normal computer
extern char    LANSERVICE_IP[];
//end							
