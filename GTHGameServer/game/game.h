
#define F_PROBABILITY( p )   (p > rand() % 1000 ? 1 : 0) 
#define F_RANDOMBETWEEN(a, b)	( a + rand() % (b - a + 1) ) 





#define F_HOTKEY(t, i) (t < 1 ? i : ( t < 2 ? MASK_HOTKEY_ACTION + i : MASK_HOTKEY_ITEM + i) )

#define MAX_PARTY_NUMBER		500


#define MOVE_SPEED				0.08 
#define RUN_SPEED				0.2  
#define TURN_SCALE				0.12


#define SAFE_SKILL_INDEX(p) (p < 0 || p >= MAX_NUMBER_OF_SKILL) ?  0 : 1





#define GRID_SCALE	3	
#define SPEED_CONSTANT		0.033 
#define RUN_SCALE	2		
#define MONSTER_RUN_SCALE	1.5		
#define BACK_SCALE	0.5f	

#define MAX_MEMBER_SERVER	10
#define MAX_SERVICE_WORLD	10


#define AVAILABLE_DISTANCE_DROPITEM		100
#define AVAILABLE_DISTANCE_PICKUPITEM	100


#define AVAILABLE_DISTANCE_ENTITYMENU	200

#define AVAILABLE_DISTANCE_PCMENU		400

#define PC_AUTO_RECOVERY_DELAY			3000



#define MONSTER_AUTO_RECOVERY_DELAY		3000

#define PC_EPACK_USE_RA_DELAY			3000
#define PC_EPACK_USE_SA_DELAY			3000

#define CANCEL_TRANSFORM_SKILL_INDEX	3000


#define PC_DECEREASE_WEAPON_DURABILITY_PROBABILITY			1.0f
#define PC_DECEREASE_DEFENSE_DURABILITY_PROBABILITY			1.0f
#define PC_DECEREASE_PROTECTOR_DURABILITY_PROBABILITY		0.4f
#define PC_DECEREASE_UPPER_DURABILITY_PROBABILITY			0.3f
#define PC_DECEREASE_LOWER_DURABILITY_PROBABILITY			0.2f
#define PC_DECEREASE_BOOTS_DURABILITY_PROBABILITY			0.05f
#define PC_DECEREASE_GLOVES_DURABILITY_PROBABILITY			0.05f
#define PC_DECEREASE_AIR_DURABILITY_PROBABILITY				0.0f


#define PC_FALL_DAMAGE_HEIGHT		300.0f
#define MAX_COUNT_STACK_ITEM		9999					
#define MAX_OWN_NAK					1000000000				



#define MAX_MASTERSERVER	10



enum
{
	GTH_LANGUAGE_TYPE_KOREAN	=	0,
	GTH_LANGUAGE_TYPE_ENGLISH,
	GTH_LANGUAGE_TYPE_JAPANESE,
	
	GTH_LANGUAGE_TYPE_SIMPLIFIED_CHINESE,
	GTH_LANGUAGE_TYPE_TRADITIONAL_CHINESE,
};

typedef struct 
{
	char serverGroupName[NAMESTRING];
	int	 serverGroupNo;
	char gameServerName[SERVERSTRING];
	int	 gameServerNo;	
	char serviceIP[IPSTRING];
	int  servicePort;
	char localIP[IPSTRING];
	int	 localPort;
	int	 masterServerNumber;
	char masterServerIP[MAX_MASTERSERVER][IPSTRING];
	int  masterServerPort[MAX_MASTERSERVER];
	int  isManager;
	char managerServerIP[IPSTRING];
	int  managerServerPort;
	int  serviceWorldNumber;
	int	 serviceWorldList[MAX_SERVICE_WORLD];
	int	 ServiceIsPkMapNumber;
	int  ServiceIsPkMapList[MAX_SERVICE_WORLD];
	int  odbcDSNType;
	char accountDatabase[DATABASESTRING];
	char gameDatabase[DATABASESTRING];
	int  gameSyncTime;             
	int	 createLog;
	int	 createDebug;
	int  languageType;
	int	 isTestServer;
	
	

	
	char	serverLocation[16];
	char	billingServerIP[IPSTRING];
	int		billingServerPort;
	BOOL	bCheckBilling;

	
	int		resendTime;
	BOOL	isDanBattleMap;																
	BOOL    bPremium;

	struct tagTerra{
		BOOL bBilling;
	}Terra;

	
	BOOL	bAhnLab;
	
} i3gameServerConfig_t;



#define		MAX_COUNT_ITEM_UPGRADE_TYPE		10

typedef struct 
{
	float		successPro[MAX_COUNT_ITEM_UPGRADE_TYPE+1];			
	int			cost[MAX_COUNT_ITEM_UPGRADE_TYPE+1];
	float		vanishPro[MAX_COUNT_ITEM_UPGRADE_TYPE+1];			
	float		gradeDownPro[MAX_COUNT_ITEM_UPGRADE_TYPE+1];		
	int			gradeDownLevel[MAX_COUNT_ITEM_UPGRADE_TYPE+1];		
} itemUpgradeProbabilityTable_t;


typedef struct
{
	float		successPro;
	int			cost;
	float		vanishPro;
	float		gradePro[5];
} optionGambleStoneTable_t;


#define MAX_GAMBLE_SELECTOPTION			7
typedef struct 
{
	int			SelectOptionCount;
	float		SelectOptionRate;	
	
	
	
	float		SelectOptionGraderate[MAX_NUMBER_OF_OPTION_VALUE_GRADE];	
} gambleSelectOptionTable_t;


#define MAX_GAMBLESTONE					2
#define MAX_ITEMRARITY					5




#define MAX_GAMBLESTONE					2


typedef struct 
{
	vec3_t	StartPos;
	int		StartworldIdx;
	vec3_t	TargetPos;
	int		TargetWorldIdx;

	
	vec3_t	TargetPos2;
	int		TargetWorldIdx2;
	
	

} DanBattlePortalInfo;

enum	
{
	DANBATTLE_START_PORTAL,
	DANBATTLE_ATEAM_PORTAL,
	DANBATTLE_BTEAM_PORTAL,
	MAX_BATTLE_PORTAL
};


enum																					
{
	DANB_WIN_DAN,
	DANB_WIN_USER,
	DANB_LOOSE_DAN,
	DANB_LOOSE_USER,
	DANB_MAX_REWARD
};


typedef struct {
	float m_fbattlein_x;
	float m_fbattlein_y;
	float m_fbattlein_z;
	float m_fbattlein_yaw;
} gonryunFieldPos_t;

#include "../GonryunPracticeBattle/CGonryunPractice_ProcessCtrl.h"
typedef struct {
	 
	 DWORD	   PCBStatusTime[CGonryunPractice_ProcessCtrl::Step::STEP_MAX];	
     int	   m_iminiLevel;
	 int	   m_imaxLevel;
	 int	   m_idifferLevel;
	 int       m_iroyalty;
	 int	   m_ibattingRoyalty;
	 int	   m_iuseCount;
	 int	   m_isyenminilevel;
	 int	   m_isyenmaxlevel;
	 float     fExpRate;
	 float	   fItemRate;
	 gonryunFieldPos_t m_tarayanReaderPos;
	 gonryunFieldPos_t m_tarayanOpenentPos;
	 gonryunFieldPos_t m_tarayanwinnerOutPos;
	 gonryunFieldPos_t m_tarayandefeatorOutPos;
	 gonryunFieldPos_t m_tasyenReaderPos;
	 gonryunFieldPos_t m_tasyenOpenentPos;
	 gonryunFieldPos_t m_tsyenOutPos;

	 
	 vec3_t origin_ArayanOutPos;
	 vec3_t origin_syenOutPos;	
	 float	pcpBattleIn;
} gonryunBattle_t;



struct tagHelperLevelPoint
{
	int iLevel1;
	int iLevel4;
	int iLevel10;
	int iLevel20;
	int iLevel30;

};


typedef struct
{
	
	float expRate;
	float nseRate;
	float itemDropRate;
	float genCapabilityRate;
	int	  cheatEnable;

	
	itemUpgradeProbabilityTable_t		leafStoneProTbl;
	itemUpgradeProbabilityTable_t		madStoneProTbl;
	itemUpgradeProbabilityTable_t		waveStoneProTbl;
	itemUpgradeProbabilityTable_t		fireStoneProTbl;
	itemUpgradeProbabilityTable_t		artStoneProTbl;

	
	
	optionGambleStoneTable_t			gambleTable[MAX_GAMBLESTONE];
	
	
	
	float	dieItemLossRate[NUM_CHAOS_TYPE];
	float	dieExpLossRate[NUM_CHAOS_TYPE];
	
	
	
	int		worldChattingPrice;	

	
	gambleSelectOptionTable_t			gambleSelectOptionTable[MAX_GAMBLE_SELECTOPTION];
	float								gambleitemSelectRate[MAX_ITEMRARITY];
	int									GamblePriceRate;

	int									DanBattlePrice;
	DWORD								danBattleStatusTime[DBTL_STATUS_MAX];			
	DanBattlePortalInfo					danbattlePortal[MAX_BATTLE_PORTAL];
	
	int									danBattleReward[DANB_MAX_REWARD];				

	
	int									danMarkChangeDelayHour;
	
	
	DWORD								PCBStatusTime[CGonryunPractice_ProcessCtrl::Step::STEP_MAX];							
	gonryunBattle_t						PCBGonryunCondition;
	
	
	struct tagHelperLevelPoint			HelperLevelPoint;
	

	
	
	
	
	struct CashMallItem_OptionStone::CashMallItemLogic	
										OptionStoneItemLogic[CashMallItem_OptionStone::MAX_NUMBER_OF_STONE];
	
	
	
	float								m_SpawnPosOverRate;	
	

} gthGameLogicCofing_t;


typedef struct 
{
	int			idx;						
	int			active;						
	int			isManager;					
	unsigned int updateTime;
	char		name[NAMESTRING];			
	char		hostName[HOSTNAMESTRING];	
	char		serviceIP[IPSTRING];		
	int			servicePort;				
	char		localIP[IPSTRING];			
	int			localPort;					
	int			serviceWorldNumber;			
	int			serviceWorldList[MAX_SERVICE_WORLD]; 
	int			curClientNumber;			
	int			maxClientNumber;			
	i3socket_t	sock;						
} memberServer_t;


typedef struct
{
	
	__int64	totalexp;
	__int64	exp;
	
	__int64 totalGenExp;
	__int64 genExp;
	
} expTable_t;


typedef struct
{
	int		genCapability[10];
	int		limitGenLevel[10];

} skillGradeClass_t;

typedef	struct
{
	int		genCapability[100];
} genCapabilityTable_t;



typedef struct  {
	int Attacklevel;
	int MaxDefense;
}MaxDefense_t;



#define			BONUS_STATUS_POINT_LEVEL_UP		4
#define			BONUS_SKILL_POINT_GEN_LEVEL_UP	1

void GTH_InitGameServer();
float GTH_GetDistance(vec3_t s, vec3_t t);
float GTH_GetDistance2D(vec3_t s, vec3_t t);
void GTH_SaveData_AlivedClient();

int GTH_GetHeightFromCollisionMap( int worldIdx, vec3_t position );

int GTH_GetLocalWorldIdx( int worldIdx );





int GTH_GetEmptyAffectIdx( playerCharacter_t *pc );

int GTH_GetWeaponItemIdx( playerCharacter_t *pc );
int GTH_GetWeaponClass( playerCharacter_t *pc );

int GTH_OpenStore( int idx );

float GTH_GetAffectHitRate( void *attacker, int attackerType, void *defender, int defenderType, int type );


float GTH_GetHitRate( void *attacker, int attackerType, void *defender, int defenderType, int addHitPower	=	0 );
float GTH_GetMagicHitRate( void *attacker, int attackerType, void *defender, int defenderType );
float GTH_GetCriticalRate( void *attacker, int attackerType, void *defender, int defenderType );


void GTH_GetExpFromLevelDiffer( int levelDiffer, int &exp, int &genExp, int &genCapability );

int GTH_IsSharingParty( monsterCharacter_t *m, playerCharacter_t *pc );
void GTH_SharingPartyLoot( monsterCharacter_t *m, playerCharacter_t *pc, int exp, int genExp, int lootNSE, int genCapability, int atkConIdx, int defConIdx );
int GTH_MakePortal( int makerType, int makerIdx, int keepTime, int numPerson, int worldIdx, vec3_t position, int targetWorldIdx, vec3_t targetPosition );
void GTH_TransportWorldWithPortal( playerCharacter_t *pc, itemObject_t *itemObject );
void GTH_TransportWorldWithNPC( playerCharacter_t *pc, int worldIdx, vec3_t position );

int GTH_IsEtherealForm( playerCharacter_t *pc );



int GTH_IsValidContributionPoint( monsterCharacter_t *m, playerCharacter_t *pc, int idx );
void GTH_InitContributionPoint( monsterCharacter_t *m );
int GTH_AddAtkContributionPoint( monsterCharacter_t *m, int entityType, int entityIdx, int point );
int GTH_AddDefContributionPoint( monsterCharacter_t *m, int entityType, int entityIdx, int point );



float GTH_ConvertAttackDelay( float currAtkDelay, float atkSpdRate );
int GTH_GetNextLevel( __int64& exp );
int GTH_GetNextGenLevel( __int64& exp );
int GTH_CheckJob( playerCharacter_t *pc, int j0 = 0, int j1 = 0, int j2 = 0, int j3 = 0 );
int GTH_CheckJob( int job, int reqJob );
int GTH_CheckRequire( playerCharacter_t *pc, int requireType, void* pReq );
bool GTH_CheckWearPosion( playerCharacter_t *pc );
int GTH_GetGenTypeFromJob( int pcJob );
int GTH_GetEquipPos( item_t *item, int equipPos );

int GTH_IsRepairItem( item_t *item );
int GTH_GetAllRepairCost( playerCharacter_t* pc, int curSE );
int GTH_GetRepairCost( item_t* item, int curSE, int &checkDur );
int GTH_IsConsumptiveItem( item_t* item );
int GTH_IsStackItem( item_t * item );
int GTH_CheckStackItem( item_t* item1, item_t* item2 );
int GTH_GetBuyPrice( item_t* item );
int GTH_GetSellPrice( item_t* item );

int GTH_GetDepotPrice( playerCharacter_t *pc, item_t *item );

float GTH_GetWeaponTypeBonusPhyDamage( playerCharacter_t *pc, item_t *item );
float GTH_GetWeaponTypeBonusMagDamage( playerCharacter_t *pc, item_t *item );

void GTH_AnalyzeAttackMessageEtcInfo( message_t *message, int eachDmg[] );

int GTH_SetTrainMonster( playerCharacter_t *pc, monsterCharacter_t *mon );
void GTH_UpdateTrainMonster( playerCharacter_t *pc );
void GTH_DeleteTrainMonster( playerCharacter_t *pc, monsterCharacter_t *mon );


int LoadExpTableForServer( char* filename );
int LoadGenExpTableForServer( char* filename );
int LoadStoreTableForServer( char* filename );
int LoadBindingTableForServer( char* filename );
int LoadScriptInfoTableForServer( char* filename );
int LoadGenCapabilityTable( char *filename );
int LoadItemCraftTableForServer( char* filename );



int GetChaosClass( playerCharacter_t *pc );
void AddChaosPoint( playerCharacter_t *attacker, playerCharacter_t *defender );
void DecChaosPoint( playerCharacter_t *pc );
void SetChaosGeneral( playerCharacter_t *attacker, playerCharacter_t *defender );


void GTH_PickUpNak( playerCharacter_t *pc, itemObject_t *itemObject );

void GetWord(int index, const char *source, char* token);


int IsPKDisableWorld( int worldIdx );

int IsPvPEnablePosition( int worldIdx, vec3_t pos);

void GTH_PC_LevelUp( playerCharacter_t *pc, int newLevel );
void GTH_PC_GenLevelUp( playerCharacter_t* pc, int newGenLevel );



float	GetEpackRechargePrice_RA( int onceRecovery, float cost, int level );
float	GetEpackRechargePrice_SA( int onceRecovery, float cost, int level );


bool	CollectItemForGamble(GambleItem_t GambleItem[] );
int		GetGambleItemAvallableOn( int availableon );
void	SelectGambleItem(storeStruct_t* Makestore, storeStruct_t* Gamblestore, int pcidx);
int		SelectCollectedItem(storeItem_t* pItem);


int		GTH_MakePortal_fordan( int LootType, int makerIdx, int keepTime, int worldIdx, vec3_t position, int targetWorldIdx, vec3_t targetPosition , int targetWorldIdx2, vec3_t targetPosition2 );


BOOL	IsValidHotKey_Skill(const playerCharacter_t *pc, const int HotkeyIdx);


void GTH_PartyRandomItemDivide(playerCharacter_t *pc,itemObject_t *itemObject);
void GTH_PartyOrderItemDivide(playerCharacter_t *pc,itemObject_t *itemObject);




BOOL	GTH_IsEquipItem(playerCharacter_t* pPc);

BOOL	GTH_IsAwakenJob(playerCharacter_t* pPc);


BOOL	GTH_ClearAwakenJob(playerCharacter_t *pPC, std::vector<int>* pVSkill);


int		GetAwakenQuestIdx(int pcjob);
void	DeleteSkill(playerCharacter_t* pPc, int SkillIdx);
BOOL	GTH_IsHaveItem(playerCharacter_t* pPc, int itemTableIdx, int StackCount);



int LoadMax_Defense_table( char* filename );

void	PkTimeAutoProcess();