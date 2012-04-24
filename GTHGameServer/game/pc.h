enum
{
	LOOTTYPE_INDIVIDUAL = 0, 
	LOOTTYPE_PARTY,
	LOOTTYPE_EVERYONE,
	
	LOOTTYPE_DANBATTLE_TEAMS,
	LOOTTYPE_DAN,
	
	

};



enum
{
	ATTACK_COMBO_TYPE_NOT	=	0,		
	ATTACK_COMBO_TYPE_0,				
	ATTACK_COMBO_TYPE_1,				
	ATTACK_COMBO_TYPE_2,				
	ATTACK_COMBO_TYPE_RANGE,			
	ATTACK_COMBO_TYPE_NEAR_RANGE,		
};



enum
{
	PC_CALCULATE_TYPE_ALL	=	0,
	PC_CALCULATE_TYPE_ATTACK,
	PC_CALCULATE_TYPE_EPACK,
};


enum
{
	QS_REWARD_EXP			=	0,
	QS_REWARD_GENEXP,
	QS_REWARD_GENCAP,
	QS_REWARD_NAK,
	QS_REWARD_ITEM,

	QS_REWARD_NUMBER,
};

enum
{
	OPTION_SPHERE_LEAF		=	0,
	OPTION_SPHERE_MAD,
	OPTION_SPHERE_WAVE,
	OPTION_SPHERE_FIRE,
	OPTION_SPHERE_ART,
};

#define DEFAULT_MELEE_ATTACK_LENGTH		150.0f			
#define DEFAULT_RANGE_ATTACK_LEGNTH		300.0f			

int GTH_SpawnPC(struct sockaddr_in *addr);
int GTH_RemovePC(playerCharacter_t *pc);

void GTH_PC_Idle(playerCharacter_t *pc);
void GTH_PC_Walk(playerCharacter_t *pc);
void GTH_PC_WalkLeft(playerCharacter_t *pc);
void GTH_PC_WalkRight(playerCharacter_t *pc);
void GTH_PC_Back(playerCharacter_t *pc);
void GTH_PC_BackLeft(playerCharacter_t *pc);
void GTH_PC_BackRight(playerCharacter_t *pc);
void GTH_PC_Run(playerCharacter_t *pc);
void GTH_PC_RunLeft(playerCharacter_t *pc);
void GTH_PC_RunRight(playerCharacter_t *pc);
void GTH_PC_TurnLeft(playerCharacter_t *pc);
void GTH_PC_TurnRight(playerCharacter_t *pc);
void GTH_PC_Attack(playerCharacter_t *pc);












void GTH_PC_MoveLeft(playerCharacter_t *pc);
void GTH_PC_MoveRight(playerCharacter_t *pc);

void GTH_PC_Damage( playerCharacter_t* pc, message_t* message );
void GTH_PC_Die(playerCharacter_t *pc);
void GTH_PC_Respawn( playerCharacter_t *pc, int curRA, int curSA, int recoveryExp = 0, int resurrectionFlag = false );

void GTH_PC_MouseWalk(playerCharacter_t *pc);
void GTH_PC_MouseRun(playerCharacter_t *pc);

void GTH_PC_AutoRecovery( playerCharacter_t* pc );
void GTH_PC_Sit(playerCharacter_t *pc);
void GTH_PC_Stand(playerCharacter_t *pc);


void GTH_PC_Shout(playerCharacter_t* pc);
void GTH_PC_BOW(playerCharacter_t* pc);


void GTH_PC_GetLoot( int idx, __int64 exp, __int64 genExp, int lootNSE, int genCapability, int sharingType = 0, BOOL bGoryunBattle=FALSE,BOOL bUseItem=FALSE);


void GTH_PC_GetLoot_QuestScroll( int idx, __int64 exp, __int64 genExp, int lootNSE, int genCapability );

 

int GTH_PC_StatusUp( int statusType );
void GTH_PC_ChangeEnergyPack( item_t* item );
void GTH_PC_ReduceExp( playerCharacter_t *pc, float percent,bool bDie =false );
void GTH_PC_GetStealPoint( playerCharacter_t *pc, int dmg );
int GTH_PC_DecreaseDurability( playerCharacter_t* pc, int type, int shock );


void GTH_ExecutePCEvent();


void GTH_ProcessEventMessage_Idle();
void GTH_ProcessEventMessage_Walk();
void GTH_ProcessEventMessage_WalkLeft();
void GTH_ProcessEventMessage_WalkRight();
void GTH_ProcessEventMessage_Back();
void GTH_ProcessEventMessage_BackLeft();
void GTH_ProcessEventMessage_BackRight();
void GTH_ProcessEventMessage_Run();
void GTH_ProcessEventMessage_RunLeft();
void GTH_ProcessEventMessage_RunRight();
void GTH_ProcessEventMessage_TurnLeft();
void GTH_ProcessEventMessage_TurnRight();
void GTH_ProcessEventMessage_MoveLeft();
void GTH_ProcessEventMessage_MoveRight();
void GTH_ProcessEventMessage_Attack();
void GTH_ProcessEventMessage_Respawn();
void GTH_ProcessEventMessage_Sit();
void GTH_ProcessEventMessage_Stand();



void GTH_ProcessEventMessage_MouseWalk();
void GTH_ProcessEventMessage_MouseRun();

void GTH_ProcessMessage_ReadyToPlay();
void GTH_ProcessMessage_TransportWorld();

void GTH_ProcessMessage_DropItem();
void GTH_ProcessMessage_PickUpItem();

int GTH_ProcessMessage_RequestGetInventoryItem();
int GTH_ProcessMessage_RequestGetEquipItem();

int GTH_FindEmptyInventoryPos(playerCharacter_t *pc);
int GTH_FindEmptyDepotPos(playerCharacter_t *pc);
int GTH_CheckLootingValidation(playerCharacter_t *pc, itemObject_t *object);

void GTH_ChangeServer( playerCharacter_t *pc );

void GTH_ProcessMessage_UpdatePosition();
void GTH_ProcessMessage_WeaponChange();
void GTH_ProcessMessage_GenChange();
void GTH_ProcessMessage_StatusUp();
void GTH_ProcessMessage_CheatCode();
void GTH_ProcessMessage_EpackUse();


void GTH_CheatCode_AddItem( playerCharacter_t* pc, item_t *cheatItem );
void GTH_CheatCode_AddSkill( playerCharacter_t* pc, int skillNumber, int* skillTableIdx, int* skillLevel );
void GTH_CheatCode_InitSkill( playerCharacter_t* pc );

void GTH_CheatCode_ItemDropTest( int monsterTableIdx, long killNumber );


void GTH_CheatCode_MonsterKill( int range );
void GTH_CheatCode_AddQuestItem( int idx );







int PC_GetAttackComboType( playerCharacter_t *pc );
int PC_IsValidAttack( playerCharacter_t* pc, int entityType, int entityIdx );
int PC_IsValidRangeAttack( vec3_t playerPos, vec3_t monsterPos, int worldIdx );







int GetTotalStatusPoint( playerCharacter_t *pc, int statusType );
float GetStatusWeightPoint( playerCharacter_t *pc, int statusType );
int GetAtkDefenseAffectedDegree( int weaponType );



float GetStausWeight( playerCharacter_t *pc, int statusType );



int CheckLimitStatus( int level, int pcJob, int status, int statusType );
void GTH_PC_CalculateByAffect();
void GTH_PC_CalculateBySkill();
void GTH_PC_CalculateStatus( playerCharacter_t* pc );
void GTH_PC_CalculateBaseValue( playerCharacter_t* pc );
void GTH_PC_CalculateValue( playerCharacter_t* pc );
void GTH_PC_CalculateHitRate( playerCharacter_t* pc );
void GTH_PC_CalculateAvoidRate( playerCharacter_t* pc );
void GTH_PC_Calculate( playerCharacter_t* pc, int type = -1 );



void GTH_PC_SetSummonsInfo(playerCharacter_t *pc, char *name, int worldIdx, vec3_t position, int forcedYn);
void GTH_PC_InitSummonsInfo(playerCharacter_t *pc);

void GTH_ProcessMessage_ReplySummons(playerCharacter_t *pc,bool _bItemUse=false);

void GTH_ProcessMessage_DestroyItem(void);
void GTH_ProcessMessage_ActiveItem( int itemIdx );

int GTH_RecoveryReliablePosition( playerCharacter_t *pc );


void GTH_PrecocityProcess( playerCharacter_t *pc );


void GTH_ForcedQuitTrade( playerCharacter_t* pc );
void GTH_ForcedQuitCraft( playerCharacter_t* pc );
void GTH_ForcedQuitUpgrade( playerCharacter_t* pc );
void GTH_ForcedQuitBooth( playerCharacter_t* pc );


BOOL GTH_ForcedQuit( playerCharacter_t *pc );

void GTH_ForcedQuitQuestScroll( playerCharacter_t *pc );


void GTH_ProcessMessage_PCTrade();
void GTH_ProcessMessage_PCTrade_Request();
void GTH_ProcessMessage_PCTrade_Admit();
void GTH_ProcessMessage_PCTrade_Reject();
void GTH_ProcessMessage_PCTrade_Exit();
void GTH_ProcessMessage_PCTrade_Item();
void GTH_ProcessMessage_PCTrade_Ok();
void GTH_ProcessMessage_PCTrade_Cancel();
int GTH_CheckConfirmTrade( playerCharacter_t *pc, int& reply, int& error );
int GTH_ConfirmTrade( playerCharacter_t *pc, int& reply, int &error );
void GTH_ProcessMessage_PCTrade_Confirm();
void GTH_ProcessMessage_PCTrade_SE();

void GTH_InitTrade( playerCharacter_t* pc );
int GTH_AddNoticePC( playerCharacter_t *pc, int entityType, int entityIdx, int busyState );
int GTH_DeleteNoticePC( playerCharacter_t *pc );
int GTH_CheckNoticePC( playerCharacter_t *pc );
int GTH_GetPCMenuType( playerCharacter_t* requestPC, playerCharacter_t* targetPC );
int GTH_GetPCMenuFlag( playerCharacter_t* requestPC, playerCharacter_t* targetPC );
int GTH_UndoPCTrade( playerCharacter_t *pc, int flagFromConfirm );



void GTH_GetStackItem( playerCharacter_t *pc, int &reply, int &error, int inventoryPos, int numStack );
void GTH_SetStackItem( playerCharacter_t *pc, int &reply, int &error );
void GTH_GetStackItemForDepot( playerCharacter_t *pc, int &reply, int &error, int depotPos, int numStack );
void GTH_SetStackItemForDepot( playerCharacter_t *pc, int &reply, int &error );;
int GTH_ConsumptiveItemProcess( playerCharacter_t *pc, int inventoryPos );
void GTH_ProcessMessage_Request_PCInfo();
void GTH_ProcessMessage_Hotkey();
void GTH_ProcessMessage_Request_Use_Portal();
void GTH_ProcessMessage_Request_SavePortalPosition();








void GTH_ProcessMessage_Party();
void GTH_ProcessMessage_Party_Request_Organize();
void GTH_ProcessMessage_Party_Admit_Organize();
void GTH_ProcessMessage_Party_Reject_Organize();

void GTH_ProcessMessage_Party_Request_Join();
void GTH_ProcessMessage_Party_Admit_Join();
void GTH_ProcessMessage_Party_Reject_Join();


void GTH_ProcessMessage_Party_Out();
void GTH_ProcessMessage_Party_ForcedOut();
void GTH_ProcessMessage_Party_Disorganize();
void GTH_ProcessMessage_Party_CanJoin();
void GTH_ProcessMessage_Party_ItemDivideType();
void GTH_ProcessMessage_Party_ChatMessage();






void GTH_SetServerAckMessage(playerCharacter_t *pc);
void GTH_ClearServerAckMessage(playerCharacter_t *pc);
int GTH_CheckServerAckMessage( playerCharacter_t *pc );



bool GTH_IsOnPlayQuestScroll( playerCharacter_t *pc );


int GTH_GetEmptyQuestStatus( playerCharacter_t *pc );
void GTH_SetQuestStatus( playerCharacter_t *pc, int idx, int questIdx, int limitTime = -1, int partyFlag = 0, int *dField = NULL );


void GTH_ClearQuestStatus( playerCharacter_t *pc, int idx, int type );
int GTH_GetQuestStatusDynamicField( playerCharacter_t *pc, int questIdx, int fieldIdx );
int GTH_SetQuestStatusDynamicField( playerCharacter_t *pc, int questIdx, int fieldIdx, int value );

void GTH_ProcessMessage_QuestLogic();


void GTH_ProcessMessage_ItemSystem();
void GTH_ProcessMessage_ItemSystem_DepotGetItem();
void GTH_ProcessMessage_ItemSystem_DepotItem();
void GTH_ProcessMessage_ItemSystem_DepotNak();


void GTH_ProcessMessage_ItemSystem_PrecocityItem();


int GTH_UndoPCCraft( playerCharacter_t *pc, int completeFlag = false );
void GTH_ProcessMessage_ItemSystem_CraftOpen();
void GTH_ProcessMessage_ItemSystem_CraftClose();
void GTH_ProcessMessage_ItemSystem_CraftItem();
void GTH_ProcessMessage_ItemSystem_CraftPrepare();
void GTH_ProcessMessage_ItemSystem_CraftComplete();


int GTH_UndoPCEventCraft( playerCharacter_t *pc, int completeFlag = false );
void GTH_ProcessMessage_ItemSystem_EventCraftOpen();
void GTH_ProcessMessage_ItemSystem_EventCraftClose();
void GTH_ProcessMessage_ItemSystem_EventCraftItem();
void GTH_ProcessMessage_ItemSystem_EventCraftPrepare();
void GTH_ProcessMessage_ItemSystem_EventCraftComplete();
void GTH_ProcessMessage_ItemSystem_EventCraftLoadEvent();



int GTH_UndoPCBooth( playerCharacter_t *pc );
void GTH_ProcessMessage_ItemSystem_BoothSellOpen();
void GTH_ProcessMessage_ItemSystem_BoothSellItem();
void GTH_ProcessMessage_ItemSystem_BoothSellStart();
void GTH_ProcessMessage_ItemSystem_BoothSellClose();
void GTH_ProcessMessage_ItemSystem_BoothBuyOpen();
void GTH_ProcessMessage_ItemSystem_BoothBuyItem();
void GTH_ProcessMessage_ItemSystem_BoothBuyClose();


int GTH_UndoPCUpgrade( playerCharacter_t *pc, int completeFlag = false );
void GTH_ProcessMessage_ItemSystem_UpgradeOpen();
void GTH_ProcessMessage_ItemSystem_UpgradeClose();
void GTH_ProcessMessage_ItemSystem_UpgradeItem();
void GTH_ProcessMessage_ItemSystem_UpgradePrepare();
void GTH_ProcessMessage_ItemSystem_UpgradeComplete();

void GTH_ProcessMessage_ItemSystem_DeletePrepare();
void GTH_ProcessMessage_ItemSystem_DeleteComplete();



void GTH_ProcessMessage_Friends (playerCharacter_t* pPlayer);
void GTH_ProcessMessage_Friends_Invite (playerCharacter_t* pPlayer);
void GTH_ProcessMessage_Friends_Accept ();
void GTH_ProcessMessage_Friends_Reject ();
void GTH_ProcessMessage_Friends_Remove ();


void GTH_ProcessMessage_QuestScroll_ObtainOpen();
void GTH_ProcessMessage_QuestScroll_ObtainPrepare();
void GTH_ProcessMessage_QuestScroll_ObtainComplete();
void GTH_ProcessMessage_QuestScroll_ObtainClose();
void GTH_ProcessMessage_QuestScroll_ComboListChange();



void GTH_ForcedQuitAutoQuestScroll( playerCharacter_t *pc );
bool GTH_IsOnPlayAutoQuestScroll(playerCharacter_t *pc);
void GTH_AutoSetQuestStatus( playerCharacter_t *pc, int idx, int questIdx, int limitTime = -1, int partyFlag = 0, int *dField = NULL );
void GTH_AutoClearQuestStatus( playerCharacter_t *pc, int idx, int type );
void GTH_PC_GetLoot_AutoQuestScroll(playerCharacter_t* pPlayer, __int64  exp, __int64 genExp, int lootNSE, int genCapability ,int fame,int chaoPoint); 

int AI_CheckItem(playerCharacter_t* pc, int *pPrarm);
void  GTH_ProcessMessage_AutoQuestScroll_ObtainOpen( playerCharacter_t *pc);
void GTH_ProcessMessage_ItemSystem_NpcInventoryOpen();
void GTH_ProcessMessage_ItemSystem_NpcInventoryClose();
void GTH_ProcessMessage_ItemSystem_AutoCraftItem();
void GTH_ProcessMessage_ItemSystem_AutoCraftPrepare();
int	 GTH_AutoUndoPCCraft( playerCharacter_t *pc, int completeFlag = false );
void GTH_ProcessMessage_AutoQuestProgress();

int FindRewardField( int index );


void GTH_Monster_SkillExecute( monsterCharacter_t *monster );





void GTH_ProcessMessage_Guild_From_Client();
void GTH_ProcessMessage_Guild_Organize_Request_From_Client();
void GTH_ProcessMessage_Guild_Disorganize_Request_From_Client();
void GTH_ProcessMessage_Guild_Invite_Request_From_Client();
void GTH_ProcessMessage_Guild_Invite_Reply_From_Client();
void GTH_ProcessMessage_Guild_Invite_Cancel_From_Client();
void GTH_ProcessMessage_Guild_Secede_Request_From_Client();
void GTH_ProcessMessage_Guild_Banish_Request_From_Client();

void GTH_ProcessMessage_Guild_Change_Master_Request_From_Client();
void GTH_ProcessMessage_Guild_Add_SubMaster_Request_From_Client();
void GTH_ProcessMessage_Guild_Add_FactionMaster_Request_From_Client();
void GTH_ProcessMessage_Guild_Delete_Master_Request_From_Client();
void GTH_ProcessMessage_Guild_Schedule_Request();
void GTH_ProcessMessage_Guild_Subscribe_Request_From_Client();

void GTH_ProcessMessage_Guild_Remove_SubMaster_Request_From_Client( char *name );
void GTH_ProcessMessage_Guild_Remove_FactionMaster_Request_From_Client( char *name );

void GTH_ProcessMessage_Guild_GuildMark_From_Client(void);


void GTH_PC_GetLoot_Cheat( int idx, __int64 exp, __int64 genExp, int lootNSE, int genCapability, int sharingType = 0 );















void OutputGuildInfomation(void);



void GTH_ProcessMessage_ChaosValue();



void GTH_ProcessMessage_SkinPart();




void GTH_ProcessMessage_Request_Use_Masic_Book();


bool	GTH_UseItem_Delete(int invenPos );




void GTH_ProcessMessage_Request_Epack_ChargSoul(void);


BOOL GTH_IsEnableHaveDamage(playerCharacter_t *pc);


BOOL	GTH_AddMyInventory_ForRegsitedItem(playerCharacter_t* pPc, int ItemArrayIdx );


int GTH_CheatAddItem(void *pPc, int *pParam );



void GTH_PC_GetLoot_ForDanBattle(int idx, __int64 exp, __int64 genExp, int lootNSE, int genCapability, int sharingType = 0, BOOL bGoryunBattle=FALSE,BOOL bUseItem=FALSE);


BOOL isValidPCBusy(playerCharacter_t* pPlayer, int busystate);
void InitItemCraftSystem(playerCharacter_t *pPlayer);
void InitItemUpgradeSystem(playerCharacter_t *pPlayer);
void InitBoothSystem(playerCharacter_t* pPlayer);
void InitPrecocitySystem(playerCharacter_t* pPlayer);


void GTH_ProcessComplusionWorldChange(playerCharacter_t *pPc);
void GTH_ProcessComplusionTransportWorld(playerCharacter_t *pPc);
