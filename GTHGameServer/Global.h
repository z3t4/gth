
#pragma warning(disable:4786)
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <tchar.h>
#include <time.h>
#include <vector>
#include <queue>
#include <deque>



#include <winsock2.h>



#include <sql.h>
#include <sqlext.h>



#define SERVERSTRING           20
#define DATABASESTRING         64
#define IPSTRING               16
#define PORTSTRING              8
#define VERSIONSTRING          20
#define IDSTRING               20
#define PASSWORDSTRING         20

#define USERNAMESTRING         20
#define JUMINSTRING            14
#define ADDRESSSTRING          50
#define EMAILSTRING            30
#define TELEPHONESTRING        16

#define NAMESTRING             20
#define PATHSTRING             64
#define WORLDNAMESTRING		   64

#define CHATSTRING			  256
#define HOSTNAMESTRING               16
#define MAX_FRIENDS_LIST		5000


#define GUILDNAMESTRING			20
#define	GUILDNOTICESTRING		100
#define MASTERNOTESTRING		20


#define PITCH 0
#define YAW   1
#define ROLL  2


#define SAFE_MONSTER(p)  (p < 0 || p >= MAX_MONSTERS) ? 0 : 1

#include "netlib/netlib.h"
#include "netlib/queue.h"
#include "etc/mtrand.h"

#include "resource.h"
#include "winmain.h"

#include "etc/token.h"
#include "etc/math.h"
#include "etc/timer.h"







#include "TableStruct/TableCommon.h"


#include "game/ItemExtendSystem.h"



#include "structure/aiCommon.h"
#include "structure/entityStruct.h"
#include "structure/itemObjectStruct.h"
#include "game/option.h"


#include "game/PostSystem.h"


#include "game/WorldChangeSystem.h"


#include "game/CashMallItem_OptionStone.h"


#include "game/tagNewDanPacket.h"
#include "game/NewDanBattleSystem.h"



#include "structure/pcStruct.h"
#include "structure/npcStruct.h"
#include "structure/monsterStruct.h"


#include "game/guild.h"
#include "game/Dan.h"		



#include "EntityListMng.h"
#include "game/game.h"
#include "game/gameError.h"
#include "game/party.h"

#include "systemAdmin.h"

#include "network/protocol.h"
#include "network/net_main.h"

#include "game/world.h"
#include "game/pc.h"
#include "game/monster.h"
#include "game/bopeitem.h"
#include "game/npc.h"
#include "game/friends.h"


#include "FuntionObject.h"



#include "game/PostFunc.h"


#include "game/WorldChangeFunc.h"



#include "ai/monsterAIEvent.h"
#include "ai/MonsterAIFunction.h"
#include "ai/interpretor.h"
#include "ai/pathfinding.h"
#include "ai/collisionmap.h"
#include "ai/npcInterpretor.h"


#include "skill/skillapp.h"
#include "skill/skills.h"
#include "skill/skillLogic.h"


#include "message/message.h"
#include "message/MessageReceive.h"
#include "message/MessageSend.h"

#include "game/affect.h"

#include "game/zone.h"
#include "log.h"


#include "debugInfo/ConsoleInfo.h"


#include "debugInfo/VCommon.h"
#include "debugInfo/VUtil.h"


#include "Billing/billing.h"
#include "Billing/tcpSocket.h"



#include "battle/GameTarget.h"
#include "battle/Attack.h"


#include "game/ChristMasEvent.h"



#include "game/EventCraftSystem.h"


#include "game/GameConfigFlag.h"


#include "game/WeatherSystem.h"


#include "HelperSystem\HelperManager.h"



#include "game/ItemTxt.h"
#include "game/ItemTableData.h"
#include "game/UseItem.h"


#include "game/ExpEventMng.h"


#include "game/CashMall_Item.h"


#include "game/GambleSystem.h"
#include "game/GambleSystemFunc.h"


#include "game/ItemExtendFunc.h"


#include "game/Item_Scroll_Proc.h"


#include "game/NewDanBattleFunc_Broadcast.h"
#include "game/NewDanBattleFunc.h"



#define MAX_PCS				1000				
#define MAX_SERVICE_PCS		MAX_PCS - 400		
//lucky 2012 Max monster changed to 4000 due of CPU patch :D
#define MAX_MONSTERS		4000
//end				
#define MAX_NPCS			100					
#define MAX_OBJECTS			1000				


#define MAX_STORES			100

#define MAX_BINDINGS		100


#define MAX_GUILDS			500					


#define QUEST_SCROLL_INDEX		1000
#define QUESTSCROLL_ITEM_INDEX	869


#define AUTOQUESTSCROLL_ITEM_INDEX	45


#define MONSTERCARD_ITEM_INDEX	868


#define GAMBLE_STORE_IDX		23




extern HINSTANCE g_hInst;								
extern TCHAR* g_strAppTitle;					
extern TCHAR* g_strWindowClass;			
extern TCHAR* g_systemStatusMessage;



extern HWND g_hWnd;




extern unsigned int g_globalTime;



extern i3gameServerConfig_t g_config;	

extern gthGameLogicCofing_t	g_logic;	


extern i3zone_t g_zone[MAX_LOCALSERVICEWORLDS][ZONE_DIV * ZONE_DIV];

extern i3zoneScanList_t				g_zoneEntityList[MAX_SCAN_ENTITYLIST];
extern int							g_zoneEntityListNumber;

extern i3zoneScanList_t				g_zonePCList[MAX_SCAN_PCLIST];
extern int							g_zonePCListNumber;

extern i3zoneScanList_t				g_zoneMonsterList[MAX_SCAN_MONSTERLIST];
extern int							g_zoneMonsterListNumber;

extern i3zoneScanList_t			g_zoneNPCList[MAX_SCAN_NPCLIST];
extern int							g_zoneNPCListNumber;


extern i3zoneScanList_t			g_zoneObjectList[MAX_SCAN_OBJECTLIST];
extern int							g_zoneObjectListNumber;



extern int					g_pcNumber;					
extern playerCharacter_t	g_pc[MAX_PCS];
extern playerCharacter_t	*g_curPC;
extern int					g_maxPCNumber;




extern golryunManagerTable_t	g_golryunManagerTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
extern golryunProgressTable_t	g_golryunProgressTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
extern golryunRewardManagerTable_t  g_golryunRewardManagerTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
extern golryunItemGroupTable_t  g_golryunItemGroupTable[MAX_NUMBER_OF_GOLRYUN_ITEM_GROUP_TABLE];


extern int					g_monsterNumber;					
extern monsterCharacter_t	g_monster[MAX_MONSTERS];

extern LIST_NONPLAYER_STRUCT		g_sNpcList;				

extern int					g_objectNumber;					
extern itemObject_t			g_object[MAX_OBJECTS];
extern itemObject_t			*g_curObject;

extern int					g_storeNumber;
extern storeStruct_t		g_store[MAX_STORES];
extern int					g_bindingNumber;
extern bindingStruct_t		g_binding[MAX_BINDINGS];

extern scriptInfoStruct_t	g_scriptInfo;


extern CTimer *g_timer;




extern CLogSystem *g_logSystem;



extern int g_fixedScreenSize;
extern unsigned long g_dwMaxTextureHeight;
extern unsigned long g_dwMaxTextureWidth;



extern worldTable_t				g_worldTable[MAX_WORLDS];
extern int						g_worldTableNumber;

extern monsterBasicTable_t		g_monsterTable[MAX_NUMBER_OF_MONSTER_TABLE];
extern int						g_monsterTableNumber;


extern expTable_t				g_expTable[MAX_NUMBER_OF_LEVEL];


extern MaxDefense_t				g_MaxDefneseTable[MAX_NUMBER_OF_LEVEL];


extern itemTable_t				g_itemTable[MAX_NUMBER_OF_ITEM_TABLE];
extern int						g_itemTableNumber;
extern itemClassTable_t			g_itemClassTable[MAX_NUMBER_OF_ITEM_CLASS_TABLE];
extern int						g_itemClassTableNumber;


extern optionTable_t			g_optionTable[MAX_COUNT_OPTION_TABLE];
extern optionGroupTable_t		g_optionGroupTable[MAX_COUNT_OPTION_GROUP_TABLE];

extern itemGenerateTable_t		g_itemGenerateTable[MAX_NUMBER_OF_ITEM_GENERATE_TABLE];
extern int						g_itemGenerateTableNumber;
extern questItemGenerate_t		g_qigTable[MAX_NUMBER_OF_MONSTER_TABLE];
extern int						g_qigTableNumber;

extern itemCraftTable_t			g_itemCraftTable[MAX_NUMBER_OF_ITEM_CRAFT_TABLE];
extern int						g_itemCraftTableNumber;




extern GambleItem_t				g_GambleItem[MAX_COUNT_OPTION_AVAILABLE_TYPE];
extern int						g_GambleItemTalbleNumber;



extern questscrollTable_t		g_questscrollTable[MAX_NUMBER_OF_QUESTSCROLL_TABLE];
extern int						g_questscrollTableNumber;

extern autoQuestScrollTable_t   g_autoQuestscrollTable[MAX_NUMBER_OF_AUTO_QUESTSCROLL_TABLE];
extern int					    g_autoQuestscrollTableNumber;
extern bool						g_AutoQuestScrolltype2;
extern bool						g_AutoQuestScrolltype4;



extern SummonMonster_t			g_summonMonstertable[MAX_NUMBER_OF_SUMMON_MONSTER_TABLE];
extern int						g_summonMonstertableNumber;


extern Consideration_t			g_ConsiderationTable[MAX_NUMBER_OF_CONSIDERATION_TABLE];
extern int						g_ConsiderationTableNumber;





extern int						g_skillTableNumber;
extern skillGradeClass_t		g_skillGradeClassTable[100];
extern genCapabilityTable_t		g_genCapabilityTable;


extern monsterSpawnOptionTable_t	g_monsterSpawnOption[100];


extern collisionMap_t			g_cmap[MAX_LOCALSERVICEWORLDS];


extern int g_memberServerNumber;
extern memberServer_t g_memberServer[MAX_MEMBER_SERVER];
extern memberServer_t *g_curMemberServer;

extern serverShutdownTimer_t g_serverShutdownTimer;
extern noticeInfo_t g_noticeInfo;

extern char g_hostName[HOSTNAMESTRING];
extern serverPartyInfo_t	g_partyInfo[MAX_PARTY_NUMBER];
extern CEntityListMng		g_entityListMng;
extern FriendsList			g_friendsList[MAX_FRIENDS_LIST];


extern guild_t				g_guild[MAX_GUILDS];



extern CDanBattleFunc	g_DanBattleFunc;												



extern LPDAN_BATTLE_MANAGER	g_pDanBattleManager;										
extern LPDAN_BATTLE_SYS		g_pDanBattleSys;

extern i3sizebuf_t			netMessage; 
extern i3socket_t			localSocket;







extern CChristMasEvent		g_ChristMasEvent;


extern CPostFunc			g_PostFunc;


extern CWeatherSystem		g_WeatherSystem;




extern CHelperManager g_HelperManager;


extern CExpEventMng		g_ExpEventMng;


extern CGambleSystem	g_GambleSystem;


extern NewDanBattleSystem		g_NewDanBattleSystem;



void GTH_InitGlobalVariable();
void GTH_DestroyVariables();

inline void sstrncpy( char *destStr, const char *sourceStr, int size )
{
	memset( destStr, 0, size );
	memcpy( destStr, sourceStr, size - 1 );

	int idx = 0;
	while( idx < size - 1 )
	{
		if( IsDBCSLeadByte( sourceStr[idx] ) )
			idx += 2;
		else
			idx ++;
	}

	if( idx == size )
		destStr[idx - 2] = 0;
};

extern tcpSocket	*g_pBillingSocket ;				



#include "Terra\GiftCard\CGiftCard.h"
#include "ODBC(open database connectivity)\tagQuerySet.h"
#include "ODBC(open database connectivity)\database.h"

extern CDBAccount *g_DBAccountServer; 
extern CDBGame *g_DBGameServer; 


