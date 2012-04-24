 #include "Global.h"

HINSTANCE g_hInst;								
TCHAR* g_strAppTitle = TEXT( "GTH GameServer Ver.06-04-13" );	
TCHAR* g_strWindowClass = TEXT( "MS" );			
TCHAR*  g_systemStatusMessage = TEXT( "Starting Game Server..." );


HWND g_hWnd;

unsigned int g_globalTime;



i3gameServerConfig_t	g_config;	
gthGameLogicCofing_t	g_logic;	



i3zone_t g_zone[MAX_LOCALSERVICEWORLDS][ZONE_DIV * ZONE_DIV];

i3zoneScanList_t			g_zoneEntityList[MAX_SCAN_ENTITYLIST];
int							g_zoneEntityListNumber;

i3zoneScanList_t			g_zonePCList[MAX_SCAN_PCLIST];
int							g_zonePCListNumber;


i3zoneScanList_t			g_zoneMonsterList[MAX_SCAN_MONSTERLIST];
int							g_zoneMonsterListNumber;

i3zoneScanList_t			g_zoneNPCList[MAX_SCAN_NPCLIST];
int							g_zoneNPCListNumber;


i3zoneScanList_t			g_zoneObjectList[MAX_SCAN_OBJECTLIST];
int							g_zoneObjectListNumber;



int					g_pcNumber;					
playerCharacter_t	g_pc[MAX_PCS];
playerCharacter_t	*g_curPC;
int					g_maxPCNumber;


int							g_monsterNumber;					
monsterCharacter_t			g_monster[MAX_MONSTERS];

LIST_NONPLAYER_STRUCT		g_sNpcList;				

int						g_objectNumber;					
itemObject_t			g_object[MAX_OBJECTS];
itemObject_t			*g_curobject;


CTimer *g_timer;


CDBAccount *g_DBAccountServer; 
CDBGame *g_DBGameServer; 


CLogSystem *g_logSystem;


int g_fixedScreenSize;
unsigned long g_dwMaxTextureHeight;
unsigned long g_dwMaxTextureWidth;


worldTable_t			g_worldTable[MAX_WORLDS];
int						g_worldTableNumber;

monsterBasicTable_t		g_monsterTable[MAX_NUMBER_OF_MONSTER_TABLE];
int						g_monsterTableNumber;

itemTable_t				g_itemTable[MAX_NUMBER_OF_ITEM_TABLE];
int						g_itemTableNumber;
itemClassTable_t		g_itemClassTable[MAX_NUMBER_OF_ITEM_CLASS_TABLE];
int						g_itemClassTableNumber;


GambleItem_t			g_GambleItem[MAX_COUNT_OPTION_AVAILABLE_TYPE];
int						g_GambleItemTalbleNumber;


optionTable_t			g_optionTable[MAX_COUNT_OPTION_TABLE];
optionGroupTable_t		g_optionGroupTable[MAX_COUNT_OPTION_GROUP_TABLE];

itemGenerateTable_t		g_itemGenerateTable[MAX_NUMBER_OF_ITEM_GENERATE_TABLE];
int						g_itemGenerateTableNumber;
questItemGenerate_t		g_qigTable[MAX_NUMBER_OF_MONSTER_TABLE];
int						g_qigTableNumber;

itemCraftTable_t		g_itemCraftTable[MAX_NUMBER_OF_ITEM_CRAFT_TABLE];
int						g_itemCraftTableNumber;



questscrollTable_t		g_questscrollTable[MAX_NUMBER_OF_QUESTSCROLL_TABLE];
int						g_questscrollTableNumber;	

autoQuestScrollTable_t  g_autoQuestscrollTable[MAX_NUMBER_OF_AUTO_QUESTSCROLL_TABLE];
int					    g_autoQuestscrollTableNumber;
bool					g_AutoQuestScrolltype2 = false;
bool					g_AutoQuestScrolltype4 = false;



SummonMonster_t			g_summonMonstertable[MAX_NUMBER_OF_SUMMON_MONSTER_TABLE];
int						g_summonMonstertableNumber;


Consideration_t			g_ConsiderationTable[MAX_NUMBER_OF_CONSIDERATION_TABLE];
int						g_ConsiderationTableNumber;



int						g_skillTableNumber;
skillGradeClass_t		g_skillGradeClassTable[100];
genCapabilityTable_t	g_genCapabilityTable;


expTable_t				g_expTable[MAX_NUMBER_OF_LEVEL];


MaxDefense_t			g_MaxDefneseTable[MAX_NUMBER_OF_LEVEL];


int						g_storeNumber;
storeStruct_t			g_store[MAX_STORES];

int						g_bindingNumber;
bindingStruct_t			g_binding[MAX_BINDINGS];
scriptInfoStruct_t		g_scriptInfo;


collisionMap_t	g_cmap[MAX_LOCALSERVICEWORLDS];

tcpSocket				*g_pBillingSocket ;				


int g_memberServerNumber;
memberServer_t g_memberServer[MAX_MEMBER_SERVER];
memberServer_t *g_curMemberServer;

CEntityListMng		g_entityListMng;
serverPartyInfo_t	g_partyInfo[MAX_PARTY_NUMBER];
FriendsList			g_friendsList[MAX_FRIENDS_LIST];

serverShutdownTimer_t g_serverShutdownTimer;
noticeInfo_t g_noticeInfo;



golryunManagerTable_t	g_golryunManagerTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
golryunProgressTable_t	g_golryunProgressTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
golryunRewardManagerTable_t  g_golryunRewardManagerTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
golryunItemGroupTable_t  g_golryunItemGroupTable[MAX_NUMBER_OF_GOLRYUN_ITEM_GROUP_TABLE];


char g_hostName[HOSTNAMESTRING];


LPDAN_BATTLE_MANAGER	g_pDanBattleManager	= NULL;										
LPDAN_BATTLE_SYS		g_pDanBattleSys		= NULL;
CDanBattleFunc			g_DanBattleFunc;
	

CChristMasEvent			g_ChristMasEvent;



CPostFunc				g_PostFunc;


CHelperManager g_HelperManager;

CWeatherSystem			g_WeatherSystem;


CExpEventMng			g_ExpEventMng;


CGambleSystem			g_GambleSystem;


NewDanBattleSystem		g_NewDanBattleSystem;




guild_t				g_guild[MAX_GUILDS];

void GTH_InitGlobalVariable()
{
	
	g_pcNumber = 0;

	if ( g_timer ) { delete g_timer; g_timer = NULL; }
	g_timer = new CTimer;

	
	if ( g_DBAccountServer ) { delete g_DBAccountServer; g_DBAccountServer = NULL; }
	g_DBAccountServer = new CDBAccount;

	if ( g_DBGameServer )	{ delete g_DBGameServer; g_DBGameServer = NULL; }
	g_DBGameServer = new CDBGame;

	if ( g_logSystem )	{ delete g_logSystem; g_logSystem = NULL; }
	g_logSystem = new CLogSystem;

	GTH_Init_ServerShutdownTimer();
	GTH_Init_ServerNoticeInfo();
	GTH_InitPartyInfo();

	
	GTH_InitGuildList();
}


void GTH_DestroyVariables()
{
	
	if ( g_timer )				{ delete g_timer;			g_timer = NULL; }
	if ( g_DBAccountServer )	{ delete g_DBAccountServer; g_DBAccountServer = NULL; }
	if ( g_DBGameServer )		{ delete g_DBGameServer;	g_DBGameServer = NULL; }
	if ( g_logSystem )			{ delete g_logSystem;		g_logSystem = NULL; }
	
	
	CloseBilling();

}




monsterSpawnOptionTable_t	g_monsterSpawnOption[100] = 
{
	
	
	
	{ 0,		1,			1,			(float)0.8,		10,				400	},
	{ 1,		1,			6,			(float)1.2,		10,				600	},
	{ 2,		1,			1,			(float)0.8,		10,				 50	},
	{ 3,		1,			3,			(float)0.8,		10,				 50	},
	{ 4,		1,			3,			(float)0.8,		10,				 70	},
	{ 5,		1,			1,			(float)0.8,		10,				 30	},
	{ 6,		1,			1,			(float)0.8,		10,				 50	},
	{ 7,		1,			1,			(float)0.8,		10,				400	},
	{ 8,		1,			3,			(float)0.8,		10,				400	},
	{ 9,		1,			3,			(float)0.8,		10,				400	},
	{ 10,		1,			3,			(float)0.8,		10,				400	},
	{ 11,		1,			5,			(float)0.8,		10,				400	},
	{ 12,		1,			5,			(float)0.8,		10,				400	},
	{ 13,		1,			5,			(float)0.8,		10,				400	},
	{ 14,		1,			5,			(float)0.8,		10,				400	},
	{ 15,		1,			5,			(float)0.8,		10,				400	},
	{ 16,		1,			1,			(float)0.8,		10,				400	},
	{ 17,		1,			1,			(float)0.8,		10,				400	},
	{ 18,		1,			7,			(float)0.8,		10,				400	},
	{ 19,		1,			7,			(float)0.8,		10,				400	},

	
	{ 20,		0,			1,			(float)0.8,		10,				500	},
	{ 21,		0,			1,			(float)1.5,		10,				500	},
	{ 22,		0,			1,			(float)1.5,		10,				500	},
	{ 23,		0,			1,			(float)1.2,		10,				500	},
	{ 24,		0,			5,			(float)0.08,		10,				1000},

};