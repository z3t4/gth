#ifdef _GTH_CHINESE_VERSION

#define GTHCOLOR_SYSMESSAGE			D3DCOLOR_ARGB( 255, 229, 110, 29 )
#define GTHCOLOR_QUESTMESSAGE		D3DCOLOR_ARGB( 255, 253, 253, 3 )
#define GTHCOLOR_WORLDMESSAGE		D3DCOLOR_ARGB( 255, 255, 128, 0 )
#define	GTHCOLOR_CHATMESSAGE		D3DCOLOR_ARGB( 255, 255, 255, 255 )
#define	GTHCOLOR_PARTYMESSAGE		D3DCOLOR_ARGB( 255, 128, 128, 255 )
#define GTHCOLOR_TRADEMESSAGE		D3DCOLOR_ARGB( 255, 255, 0, 255 )
#define GTHCOLOR_DANMESSAGE			D3DCOLOR_ARGB( 255, 255, 255, 0 )
#define GTHCOLOR_WHISPERMESSAGE		D3DCOLOR_ARGB( 255, 61 , 250, 52 )

#else

#define GTHCOLOR_SYSMESSAGE			D3DCOLOR_ARGB( 255, 229, 110, 29 )	
#define GTHCOLOR_QUESTMESSAGE		D3DCOLOR_ARGB( 255, 253, 253, 3 )	
#define GTHCOLOR_WORLDMESSAGE		D3DCOLOR_ARGB( 255, 253, 165, 21 )	
#define	GTHCOLOR_CHATMESSAGE		D3DCOLOR_ARGB( 255, 255, 255, 255 )	
#define	GTHCOLOR_PARTYMESSAGE		D3DCOLOR_ARGB( 255, 2,   226, 13 )	
#define GTHCOLOR_TRADEMESSAGE		D3DCOLOR_ARGB( 255, 31,  251, 251 )	
#define GTHCOLOR_DANMESSAGE			D3DCOLOR_ARGB( 255, 235, 147, 128 )	
#define GTHCOLOR_WHISPERMESSAGE		D3DCOLOR_ARGB( 255, 225, 173, 224 )	

#endif

#define GTHCOLOR_LOOTMESSAGE		D3DCOLOR_ARGB( 255, 100, 255, 255 ) 
#define GTHCOLOR_NOTICEMESSAGE		D3DCOLOR_ARGB( 255, 255, 255, 100 )	
#define GTHCOLOR_ERRORMESSAGE		D3DCOLOR_ARGB( 255, 255, 55, 55 )	


#define GTHCOLOR_ITEMNOTICE_MESSAGE D3DCOLOR_ARGB( 255, 0, 255, 255 )	









#define GRID_SCALE	3	
#define RUN_SCALE	2		
#define MONSTER_RUN_SCALE 1.5
#define BACK_SCALE	0.5f	


#define INTERPOLATION_SPEED		50
#define SPEED_CONSTANT		33	
#define TURN_SCALE			120		







#define AVAILABLE_DISTANCE_DROPITEM		100
#define AVAILABLE_DISTANCE_PICKUPITEM	100


#define AVAILABLE_DISTANCE_ENTITYMENU	200

#define AVAILABLE_DISTANCE_PCMENU		400

#define PC_REQUEST_MENU_WAIT_TIME		5000.0f
#define ITEMOBJECT_DISPLAY_HEIGHT	30

#define PC_EPACK_USE_RA_DELAY			3000
#define PC_EPACK_USE_SA_DELAY			3000
#define PC_CHAT_MESSAGE_RENDER_TIME		6000

#define CANCEL_TRANSFORM_SKILL_INDEX	3000
#define MAX_COUNT_STACK_ITEM			9999


enum
{
	ATTACK_COMBO_TYPE_NOT	=	0,		
	ATTACK_COMBO_TYPE_0,				
	ATTACK_COMBO_TYPE_1,				
	ATTACK_COMBO_TYPE_2,				
	ATTACK_COMBO_TYPE_RANGE,			
	ATTACK_COMBO_TYPE_NEAR_RANGE,		
};


typedef enum
{
	TRANSFORM_NORMAL		=	0,		
	TRANSFORM_ETHEREAL,					
} transformType_t;

#define DEFAULT_MELEE_ATTACK_LENGTH		150.0f
#define DEFAULT_RANGE_ATTACK_LEGNTH		300.0f



enum
{
	REQUIRE_TYPE_ITEM=0,
	REQUIRE_TYPE_SKILL,
};



int GTH_StartUpGame(int _Type = -1);
void GTH_ShutdownGame();
int GTH_ParseGameConfig();


int GTH_ParseGameConfig_MasterServerGroup();
void GTH_ProcessGame();
void GTH_GenerateEventMessageByInput();


void GTH_ExecuteCharacterEvent();
void GTH_Character_Idle(Fx_CHARACTER_t *character);
void GTH_Character_Walk(Fx_CHARACTER_t *character);
void GTH_Character_Back(Fx_CHARACTER_t *character);
void GTH_Character_TurnLeft(Fx_CHARACTER_t *character);
void GTH_Character_TurnRight(Fx_CHARACTER_t *character);
void GTH_Character_MoveLeft(Fx_CHARACTER_t *character);
void GTH_Character_MoveRight(Fx_CHARACTER_t *character);
void GTH_Character_Run(Fx_CHARACTER_t *character);
void GTH_Character_Attack(Fx_CHARACTER_t *character);
void GTH_Character_Die(Fx_CHARACTER_t *character);
void GTH_Character_Respawn(Fx_CHARACTER_t *character);

void GTH_Character_MouseWalk(Fx_CHARACTER_t *character);
void GTH_Character_MouseRun(Fx_CHARACTER_t *character);
void GTH_Character_MouseLeft(Fx_CHARACTER_t *character);
void GTH_Character_MouseRight(Fx_CHARACTER_t *character);


void GTH_Character_Pushed(Fx_CHARACTER_t *character);

float GTH_GetDistance(vec3_t s, vec3_t t);
float GTH_GetDistance2D(vec3_t s, vec3_t t);
void GTH_RequestChangeWorld( int targetWorldIdx, int targetGateIdx );
void GTH_PrepareChangeWorld();

void GTH_SetWorldInfo();

void GTH_SetMyCharacterEvent( Fx_CHARACTER_t *character );
void GTH_SetMyCharacterMouseEvent( Fx_CHARACTER_t *character );
void GTH_SendMyCharacterEvent();
void GTH_GenerateMyCharacter();

int GTH_Temp_GetTargetMonster();
void GTH_DeadReckoning(Fx_CHARACTER_t *character);
void GTH_CharacterInterpolation(Fx_CHARACTER_t *character);


int GTH_AdjustNextAngle( float target, float angle );
int GTH_AdjustDirection( float target, float angle );

void GTH_InitItem( item_t *item );
void GTH_DeleteMyItem( item_t *item );
int GTH_AddMyItem( int itemIdx, item_t *item );
int GTH_AddMyItem( item_t *item );
int GTH_StackMyItem( item_t *item );
int GTH_FindEmptyInventoryPos();


int GTH_StackMyItemForDepot( item_t *item );
int GTH_FindEmptyDepotPos();


void GTH_ChangeAnimation(Fx_CHARACTER_t *character, int	aniSetType,	int aniSetID, bool in_forceUpdate = false, float* attackSpeed = NULL, bool in_forceChange = false );
void GTH_Character_Stun( Fx_CHARACTER_t *character );


void GTH_Character_PcStun( Fx_CHARACTER_t *character );

void GTH_Character_Sit( Fx_CHARACTER_t *character,float fSpeed =2000.0f,bool bNewFlag =true);
void GTH_Character_Stand( Fx_CHARACTER_t *character );





void GTH_Character_Shout( Fx_CHARACTER_t* character );
void GTH_Character_Bow( Fx_CHARACTER_t* character );

#define MAX_EQUIPTMENT	20
typedef struct
{
	char				userID[NAMESTRING];		
	int					characterID;			
	int					gameServerID;			

	int					pcClass;				
	char				name[NAMESTRING];		
	int					age;					

	int					worldIdx;				
	
	
	int					level;					
	int					exp;					
	int					nextExp;				

	
	int					force; 					
	int					soulpower;				
	int					agility; 				
	int					vitality;				

	
	int					curRA;					
	int					curSA;					

	
	int					equipment[MAX_EQUIPTMENT];
	int					NetLastTime;
} characterData_t;



#define MAX_WORLD_GATE_NUMBER	5 

typedef struct 
{
	int		idx;
	char	hanName[WORLDNAMESTRING];
	char	name[NAMESTRING];
	char	cfgFile[64];
	vec3_t	startPos;
	float	startYaw;

	int		gateNumber;
	int		gateIdx[MAX_WORLD_GATE_NUMBER];
	vec3_t	spawnPos[MAX_WORLD_GATE_NUMBER];
	float	spawnYaw[MAX_WORLD_GATE_NUMBER];
	int		linkedWorldIdx[MAX_WORLD_GATE_NUMBER];
	int		linkedGateIdx[MAX_WORLD_GATE_NUMBER];
} worldTable_t;

int GTH_WorldToIndex( char *world );
int GTH_LoadWorldTable();

void GTH_EpackProcess();



void GTH_SetItemObjectModel( int objectType, int itemTableIdx, int &classType, int &mdlID );



#define UPDATE_ZPOS_HEIGHT	100.0f
#define SUICIDE_ZPOS_HEIGHT	300.0f

int GTH_MyCharacter_CheckCollision();
void GTH_UpdateSendedZPos();
int GTH_MyCharacter_UpdatePosition();







void GTH_Character_MonsterWalk(Fx_CHARACTER_t *character);
void GTH_Character_MonsterRun(Fx_CHARACTER_t *character);






int GTH_GetTargetEntityForKeyMode( Fx_CHARACTER_t* pThisChar );
int GTH_IsValidTargetAngleForKeyMode( Fx_CHARACTER_t* pThisChar );

void GTH_ServerShutdownTimer();
void GTH_Init_ServerShutdownTimer();

int GTH_GetSkillState();


void GTH_GetRealMousePosition( vec3_t mousePos, vec3_t position );
void GTH_GetRealPosition( vec3_t position, float dist );

int GTH_CheckValidCharacterEvent( Fx_CHARACTER_t* character );
int GTH_HaveWeapon( Fx_CHARACTER_t* character );

int GTH_LockSending();
void GTH_UnlockSending();
int GTH_IsLock();

int GTH_DeleteList();
int GTH_RenameList();
int GTH_IsGameMaster( Fx_CHARACTER_t *character );
int GTH_RenderGM( Fx_CHARACTER_t* character );


void GTH_RecoverySafePosition();
void GTH_InitPartyInfo();

int GTH_CheckJob( myCharacterInfo_t *pc, int j0 = 0, int j1 = 0, int j2 = 0, int j3 = 0 );
int GTH_CheckRequire( myCharacterInfo_t *pc, int requireType, void* pReq );
int GTH_CheckJob( int job, int reqJob );

int GTH_GetGenTypeFromJob( int pcJob );

int GTH_GetEquipPos( item_t *item, int equipPos );
int GTH_IsRepairItem( item_t *item );
int GTH_GetRepairCost( item_t* item, int curSE, int &checkDur );
int GTH_GetAllRepairCost( myCharacterInfo_t* pc, int curSE );
int GTH_GetBuyPrice( item_t* item );
int GTH_GetSellPrice( item_t* item );
int GTH_GetDepotPrice( item_t *item );
int GTH_IsStackItem( item_t* item );
int GTH_IsConsumptiveItem( item_t* item );
int GTH_CheckStackItem( item_t* item1, item_t* item2 );
int GTH_GetAttackAnimationIndex();
int GTH_GetItemType( int itemTableIdx );
int GTH_GetItemGrade( item_t *item );

void AddTargetList( char *name );

int GetChaosClass( Fx_CHARACTER_t *character );


int IsPKDisableWorld( int worldIdx );



bool GetGuildMarkInfomationByFileName( const char *fname, int &serverGroupId, int &guildIdx, int &updateTime );
void MakeGuildMarkInfo();
void LoadGuildMark();
bool MakeGuildMarkFile( short guildIdx, int updateTime, char *buffer );
bool CheckGuildMarkFile( char *fname, char *buffer );



typedef struct
{
	int serverGroupId;
	short guildIdx;
	int updateTime;
	char fname[256];
	bool bLoaded;
} guildMarkInfo_t;

extern guildMarkInfo_t		g_guildMarkInfo[MAX_GUILDS];


int GTH_GetUsedMonster_AnimationList( int list[] );

float	FindTransformAttackLength(int transformEntityIdx, int transformType);
float	GetEpackRechargePrice_RA( int onceRecovery, float cost, int level );
float	GetEpackRechargePrice_SA( int onceRecovery, float cost, int level );
 


bool CheckStateWeaponChange(Fx_CHARACTER_t* character);

bool CheckAvailableGenCapability( myCharacterInfo_t *pc, int genType );

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



int		GTH_GetMatchMyItemIdx_fromInventory(const int itemtableIdx);
BOOL	DeleteItemFromMyInventoryByInvenPos(const int invenPos, const int StackConut = 1);
int		GTH_GetInvenPosMatchMyItemTableIdx(const int itemtableIdx);
