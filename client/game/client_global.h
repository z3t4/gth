#ifndef _CLIENT_GLOBAL_H_
#define _CLIENT_GLOBAL_H_

#include "../game/gameError.h"
#include "../netlib/netlib.h"
#include "../network/protocol.h"
#include "../network/net_main.h"
#include "game.h"



#include "../TableStruct/TableCommon.h"
#include "../TableStruct/GameTable.h"

#include "../game/Option.h"

#include "../IF/IFManager.h"


#include "../game/Token.h"
#include "../game/ConfigApp.h"


#define		TARGETING_RESEND_DELAY		1000


 
#define		MAX_SYNC_SAMPLE_COUNT		20

typedef enum
{

	CLIENT_START_PROCESS		=	0,
	CLIENT_LOGIN_PROCESS,
	CLIENT_CHARACTER_PROCESS,
	CLIENT_LOADING_PROCESS,
	CLIENT_GAME_PROCESS

} clientProcessState_t;

enum
{
	CLIENT_INPUT_MOUSE_MODE			= 0,
	CLIENT_INPUT_KEY_MODE
};

enum
{
	RESTART_NONE				=	0,
	RESTART_CHARACTER,
	RESTART_SERVER,
	RESTART_RESERVER,
};


enum
{
	GTH_LANGUAGE_TYPE_KOREAN	=	0,
	GTH_LANGUAGE_TYPE_ENGLISH,
	GTH_LANGUAGE_TYPE_JAPANESE,
	GTH_LANGUAGE_TYPE_SIMPLIFIED_CHINESE,
	GTH_LANGUAGE_TYPE_TRADITIONAL_CHINESE,
};


enum
{
	INPUT_INDEX_DIALOGBOX		= 0,
	INPUT_INDEX_BOOTH_NAME,
	INPUT_INDEX_BOOTH_INFO,
	INPUT_INDEX_CRAFTHELP,
	INPUT_INDEX_MAIL_NAME,
	INPUT_INDEX_MAIL_TITLE,
	INPUT_INDEX_MAIL_CONTENT,
	INPUT_INDEX_GUILD_SCHEDULE,
	INPUT_INDEX_WORLDCHAT,
	INPUT_INDEX_POST_NAME,
	INPUT_INDEX_POST_TITLE,
	INPUT_INDEX_POST_MESSAGE,		
	INPUT_INDEX_GIFT_CARD_ID,		
	INPUT_INDEX_GIFT_CARD_PASSWORD,	
	
	INPUT_INDEX_ITEM_SCROLL_WINDOW_ID,
	
	INPUT_INDEX_DAN_NAME,	
};



typedef struct
{
	int					masterServerNumber;
	char				listMasterServerIP[MAX_MASTERSERVER][IPSTRING];
	int					listMasterServerPort[MAX_MASTERSERVER];
	char				masterServerGroupName[NAMESTRING];

} masterServerGroup_t;

typedef struct 
{
	
	clientProcessState_t	state;
	
	
	int					processChangeType;
	int					changeServerFlag;

	
	i3notice_t			notice[MAXNOTICE];
	int					noticeNumber;
	serverGroup_t		serverGroup[MAX_SERVER_GROUP];
	int					selectedGroupIdx;

	
	char				gameServerIP[IPSTRING];
	int					gameServerPort;

	
	int					masterServerNumber;
	char				listMasterServerIP[MAX_MASTERSERVER][IPSTRING];
	int					listMasterServerPort[MAX_MASTERSERVER];
	char				gameVersion[VERSIONSTRING];
	char				subVersion[VERSIONSTRING];
	
	
	
	int					masterServerGroupNumber;
	masterServerGroup_t	masterServerGroupInfo[MAX_MASTERSERVER];
	


	
	char				clientID[IDSTRING];
	char				clientPassword[PASSWORDSTRING];

	
	int					keyRun;

	
	int					onMasterServer; 
	int					onGameServer;

	
	syncCharacter_t		syncCharacter[MAX_CHARACTERS];
	int					syncCharacterNumber;
	int					myPCIdxOnServer;

	
	int					displayDebugMessage;

	
	bool				enableChat;
	DWORD				chatColor;

	
	bool				isLogin;

	
	char				whisperName[255];
	int					whisperType;

	
	unsigned int		latencyTime;

	
	char				worldInfo[255];

	
	worldTable_t		worldTable[MAX_WORLD_NUMBER];
	int					worldTableNumber;

	
	int					flagChangeWorld;


	
	HWND				hDlg;

	
	int					savedDataNumber;
	int					isReceiveAllSavedData;
	myCharacterInfo_t	savedData[MAX_SAVED_DATA];
	myCharacterInfo_t	*myCharacterInfo;
	int					selectedGameDataIdx;

	
	int					characterMoveFlag;

	int					offLineStartFlag;

	float				sendedZPos;

	i3characterClickEvent_t		clickEvent;

	
	int					inputMode;

	
	int					resendFlag;
	int					enablePK;
	int					resendTargeting;
	float				resendTime;
	int					attackSuccess;
	int					resendAttack;
	Fx_CHARACTER_t*		pTargetChar;
	int					targetItemIdxOnServer;
	vec3_t				oldTargetPosition;

	
	float				epackRADelayMaxTime;
	float				epackSADelayMaxTime;
	float				epackRADelayTime;
	float				epackSADelayTime;
	
	serverShutdownTimer_t serverShutdownTimer;

	int					waitReply;
	float				lastLockingTime;

	int					saveEvent;

	vec3_t				saveMovePosition;

	
	int					sendSkillFlag;
	int					sendTeleportFlag;
	char				targetName[NAMESTRING];

	unsigned int		callGMTimer;
	
	float				mouseClickEventDistance;


	int					isDeveloper;
	int					isUpdatePatch;

	
	chatMessage			callerName[20];
	chatMessage			callerReason[20];
	int					isGmBusy;
	int					isGmHide;

	int					summonMonsterIdx;
	int					summonAiType;
	float				summonValidTime;

	
	float				saveMenuTime;
	int					saveInventoryIdx;




	int					restartState;

	 
	int					currentWorldIdx;
		

	float				currentSyncTime;
	float				lastSyncTime;
	float				minSyncTime;
	float				maxSyncTime;
	int					missSyncTimeCount;

	
	
	

	 	
	float				syncTimeList [MAX_SYNC_SAMPLE_COUNT];
	int					syncSampleCount;
	float				syncAverage;
	int					syncOverAverageCount;

	int					isTestServer;

	
	int					languageType;
	char				TIDString[64];

	int					sendGameServerConnectFlag;

	
	
	int					pvpMove;
	
	
	float				m_fClientGameVersion;			

	enum
	{
		SELECT_NONE=0,
		SELECT_ITEM,
		SELECT_SKILL_HOTKEY,
	};

	public:
		struct tagGM{
			BOOL bChatAllow;
		};
		struct tagGM GM;
	
	
	struct 
	{
		
		int					iSkillExecuteIndex;
		int					iSkillExecuteTargetType;
		int					iSkillExecuteUseType;

		int					iSelectSkillUseType;
		int					iSelectSkillTableIdx;
		int					iUseInvenPos;

		
		int					iRecvSkillCastingCount;
	}Skill;

	int					saveSkillIdx;
	int					saveSkillTarget;
	vec3_t				saveSkillTargetPos;
	int					saveTargetType;
	int					saveTargetIdx;

	
	enum CharacterSlotMode
	{
		FRONT,
		BACK,
	};
	enum CharacterSlotMode	characterSlotMode;
} globalVariable_t;

extern globalVariable_t		g_cgv;	


void GTH_InitClientGlobalVariable();
void GTH_InitSavedData();


int GTH_CheckChatCommandState();
void GTH_RunGMChatCommand( char message[], char command[], chatMessage param[] );
void GTH_RunChatCommand( char command[], chatMessage param[] );

void GTH_ChatCommand( char* message );
int GTH_InputState();
int GTH_CheckMoveable();
void GTH_EnableMove();
void GTH_DisableMove();
int GTH_CanMove();


int GTH_CanOpenBooth();


int GTH_SaveOptionToFile();
int GTH_LoadOptionFromFile();


#endif 
