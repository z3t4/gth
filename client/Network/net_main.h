 #define RELIABLE_WAITTIME   1000 
#define CONNECT_WAITTIME    5000         
#define MAX_RESENDING_TIMES 10   
#define ALIVECONNECTION_TIME 10000 

#define MAXNOTICE			5







	



#define MAX_SERVER_GROUP    20
#define F_RANDOMBETWEEN(a, b)	( a + rand() % (b - a + 1) ) 
#define MAX_MASTERSERVER	10


enum
{
	WORLD_CHATTING_ERROR_SE=1,				
	WORLD_CHATTING_ERROR_COUNT,				
};


typedef struct 
{
	int		active;
	int		idx;
	char	name[SERVERSTRING];
	int		clientNumber;
	int		access;
} serverGroup_t;

typedef struct
{
	char date[11]; 
	char content[256];
} i3notice_t;


int GTH_Network_StartUp();
void GTH_Network_ShutDown();
int GTH_Network_OpenGameServerSocket();
int	GTH_Network_GetMessage(i3socket_t *sock);
void GTH_Network_CheckProblemSocket(i3socket_t *sock);
void GTH_Network_InitNetTime();

int GTH_ListenPacket();

int GTH_SendMessage_DisconnectGameServer(BOOL bForceQuit_fromGM); 
int	GTH_SendChatMessage(char *chatMessage, int type); 
int	GTH_SendChatMessage_Whisper(char *name, char *chatMessage);
void GTH_SendMessage_ConnectGameServer();
int GTH_SendMessage_ReadyToPlay();
int	GTH_SendMessage_TransportWorld(int worldIdx, int gateIdx);

int	GTH_CheckSpeedHack	();		 

void GTH_SyncCharacter(void);
void GTH_ProcessChatMessage();
void GTH_ProcessChatMessage_Whisper();
int GTH_ProcessMessage_TransportRep();
int GTH_ConnectReplyFromGameServer();
int GTH_ChangeServer();
int GTH_ProcessMessage_ChangeWorld();

void MSG_WritePosition(i3sizebuf_t *sb, vec3_t pos);
void MSG_ReadPosition(vec3_t v);
void MSG_WriteItem(i3sizebuf_t *sb, item_t *item);
void MSG_ReadItem(item_t *item);

void MSG_WriteMember(i3sizebuf_t *sb, partyMember_t *m);
void MSG_ReadMember( partyMember_t *m );
void MSG_WriteAffect( i3sizebuf_t *sb, affect_t *aff );
void MSG_ReadAffect( affect_t *aff );

void GTH_SendMessage_ReliablePing( i3socket_t *sock );
void GTH_SendMessage_UnreliablePing( i3socket_t *sock );

int	GTH_SendMessage_SystemAdimin_ShutdownServer();
void GTH_ProcessMessage_SetShutdownTimer();
int	GTH_SendMessage_SystemAdimin_GoToPlayer(char *name);
int	GTH_SendMessage_SystemAdimin_GoToMonster(int idx);
int	GTH_SendMessage_SystemAdimin_Notice(char *notice);
void GTH_SendMessage_SystemAdimin_Busy();
void GTH_SendMessage_SystemAdimin_Hide();
void GTH_ProcessMessage_NoticeMessage();
int	GTH_SendMessage_CallGameMaster( char* reason );
void GTH_ProcessMessage_CallGameMaster();
int	GTH_SendMessage_RequestSummons(char *name, int forcedYn);
int GTH_SendMessage_ReplySummons(int reply);

void GTH_ProcessMessage_ReplySummons();
int	GTH_SendMessage_RequestSummonMonster( int num );
int	GTH_ProcessMessage_ReplySummonMonster();
int	GTH_SendMessage_UpdatePosition();
int	GTH_ProcessMessage_NotFoundPC();

int GTH_ProcessMessage_AddItemInv();
int GTH_ProcessMessage_DeleteItemInv();

void GTH_ProcessMessage_ReplyCheatCode();






void GTH_ProcessMessage_ChattingMsg();


enum 
{
	REJECT_OVER_MAXCLIENT = 0,
	REJECT_VERSION_MISMATCH = 1,
};

int GTH_ProcessMessage_RejectClient();


void GTH_ReturnMasterServer( int restartflag );


void GTH_Debug_PacketInfo();



void GTH_SendMessage_DropItem(int itemIdx, vec3_t itemPos);
void GTH_SendMessage_PickUpItem(int itemObjectIdx);

int GTH_ProcessMessage_AddMultiItemObject();
int GTH_ProcessMessage_LootValue();

int GTH_ProcessMessage_AddItemObject();
int GTH_ProcessMessage_DeleteItemObject();

int GTH_ProcessMessage_SyncItemObject();
void GTH_SendMessage_RequestGetInventoryItem(int inventoryPos, int directEquip, int numStack = 0 );
int GTH_ProcessMessage_ReplyGetInventoryItem();
void GTH_SendMessage_RequestGetEquipItem(int equipPos, int directEquip );

int GTH_ProcessMessage_ReplyGetEquipItem();
int GTH_ProcessMessage_ReplyDropItem();
int GTH_ProcessMessage_ReplyPickUpItem();
int GTH_SendMessage_RequestRepairItem( int type, int pos );
int GTH_ProcessMessage_ReplyRepairItem();


int GTH_SendMessage_RequestDestroyItem( int invenPos = -1 );

int GTH_ProcessMessage_ReplyDestroyItem();





int	GTH_SendEventMessage_Idle();
int	GTH_SendEventMessage_Walk();
int	GTH_SendEventMessage_WalkLeft();
int	GTH_SendEventMessage_WalkRight();
int	GTH_SendEventMessage_Back();
int	GTH_SendEventMessage_BackLeft();
int	GTH_SendEventMessage_BackRight();
int	GTH_SendEventMessage_Run();
int	GTH_SendEventMessage_RunLeft();
int	GTH_SendEventMessage_RunRight();
int	GTH_SendEventMessage_TurnLeft();
int	GTH_SendEventMessage_TurnRight();
int	GTH_SendEventMessage_MoveLeft();
int	GTH_SendEventMessage_MoveRight();
void GTH_SendEventMessage_Attack();			

int	GTH_SendEventMessage_Respawn();
int	GTH_SendEventMessage_MouseWalk();
int	GTH_SendEventMessage_MouseRun();
int GTH_SendEventMessage_Skill();


int GTH_SendEventMessage_Before_Teleport();
int	GTH_SendEventMessage_Teleport();

int GTH_ProcessEventMessage_State();

int GTH_ProcessEventMessage_Idle();
int GTH_ProcessEventMessage_Walk();
int GTH_ProcessEventMessage_WalkLeft();
int GTH_ProcessEventMessage_WalkRight();
int GTH_ProcessEventMessage_Back();
int GTH_ProcessEventMessage_BackLeft();
int GTH_ProcessEventMessage_BackRight();
int GTH_ProcessEventMessage_Run();
int GTH_ProcessEventMessage_RunLeft();
int GTH_ProcessEventMessage_RunRight();
int GTH_ProcessEventMessage_TurnLeft();
int GTH_ProcessEventMessage_TurnRight();
int GTH_ProcessEventMessage_MoveLeft();
int GTH_ProcessEventMessage_MoveRight();
int GTH_ProcessEventMessage_Attack();
int GTH_ProcessEventMessage_Damage();
int GTH_ProcessEventMessage_Die();
int GTH_ProcessEventMessage_Respawn();
int GTH_ProcessEventMessage_MouseWalk();
int GTH_ProcessEventMessage_MouseRun();
int GTH_ProcessEventMessage_Skill();
int GTH_ProcessEventMessage_SkillCasting();

int GTH_ProcessEventMessage_Sit();
int GTH_ProcessEventMessage_Stand();
int GTH_ProcessMessage_AutoRecovery();

int GTH_ProcessMessage_WeaponChange();
int GTH_SendMessage_RequestWeaponChange();
int GTH_ProcessMessage_LevelUp();
int GTH_SendMessage_RequestGenChange( int gen );
int GTH_ProcessMessage_ReplyGenChange();
int GTH_SendMessage_RequestStatusUp( int statusType );
int GTH_ProcessMessage_ReplyStatusUp();

int GTH_SendMessage_CheatCode( int type, int value, int value2 = -1 );
int GTH_SendEventMessage_Sit();
int GTH_SendEventMessage_Stand();
int GTH_SendMessage_RequestEpackUse( int type );
int GTH_ProcessMessage_ReplyEpackUse();

int GTH_ProcessEventMessage_Sit();
int GTH_ProcessEventMessage_Stand();


int	GTH_SendEventMessage_SocialAnimation(int event);


int GTH_Network_EstablishMasterServerConnection();
int GTH_MasterServer();
int GTH_ConnectMasterServer();

void GTH_SendMessage_ConnectMasterServer();
void GTH_SendMessage_DisconnectMasterServer();
void GTH_SendMessage_RequestNotice();
void GTH_SendMessage_RequestLogin(char *id, char *password);
void GTH_ProcessMessage_ReplyLogin();

void GTH_SendMessage_RequestServerGroup();
void GTH_ProcessMessage_ReplyServerGroupInfo();

void GTH_SendMessage_RequestGameData();
void GTH_ProcessMessage_ReplyGameData();

void GTH_SendMessage_RequestGameServerAddr();
void GTH_ProcessMessage_ReplyServerAddress();

void GTH_NoticeInfo();
int GTH_SetSavedData();

void GTH_SendMessage_RequestReadyCreate();
void GTH_ProcessMessage_ReplyReadyCreate();

void GTH_SendMessage_RequestCreateCharacter(int characterID, charEditData_t *data);
void GTH_ProcessMessage_ReplyCreateCharacter();

void GTH_SendMessage_RequestDeleteCharacter();
void GTH_ProcessMessage_ReplyDeleteCharacter();

void GTH_SendMessage_RequestCheckCharacterName(char *characterName);
void GTH_ProcessMessage_ReplyCheckCharacterName();


void	CreateDefaultSelectChar();


void		GTH_SendMessage_Request_SaveCharacterSlot(int nSlotType);
void		GTH_ProcessMessage_ReplyLoadCharacterSlot();
void		GTH_SendMessage_Request_LoadCharacterSlot();



void		ChangeCharacterSlot();




int GTH_SendMessage_RequestNpcChat( int idx );
int GTH_ProcessMessage_ReplyNpcChat();
int GTH_SendMessage_RequestQuitNpcChat();
int GTH_ProcessMessage_ReplyQuitNpcChat();
int GTH_ProcessMessage_NpcMessage();
int GTH_SendMessage_RequestNpcSelectMessage( int curIdx );
int GTH_SendMessage_RequestStore( int idx, int type );
int GTH_ProcessMessage_ReplyStore();
int GTH_ProcessMessage_ReplyBuyItem();
int GTH_ProcessMessage_ReplySellItem();
int GTH_SendMessage_RequestBuyItem( int itemIdx, int num = 1 );
int GTH_SendMessage_RequestSellItem();

int GTH_SendMessage_RequestBinding( int type, int bindingIdx, int bindingCost );
int GTH_ProcessMessage_ReplyBinding();

int GTH_ProcessMessage_ReplyEpackStore();
int GTH_SendMessage_RequestEpackStore( int type, int num );

int GTH_SendMessage_Request_BuySkill( int idx );
int GTH_ProcessMessage_Reply_BuySkill();
int GTH_ProcessMessage_QuestLogic();
int GTH_ProcessMessage_QuestLogic_ChatMsg();
int GTH_ProcessMessage_QuestLogic_Awaken();

int GTH_ProcessMessage_QuestLogic_InitStatus();
int GTH_SendMessage_QuestLogic_InitStatus();

int GTH_ProcessMessage_NPCInfo();


int GTH_ProcessMessage_PCTrade();
int GTH_SendMessage_PCTrade_Request( int idxOnServer );
int GTH_ProcessMessage_PCTrade_Request();
int GTH_SendMessage_PCTrade_Admit();
int GTH_ProcessMessage_PCTrade_Admit();
int GTH_SendMessage_PCTrade_Reject( int type );
int GTH_ProcessMessage_PCTrade_Reject();
int GTH_SendMessage_PCTrade_Exit();
int GTH_ProcessMessage_PCTrade_Exit();

int GTH_SendMessage_PCTrade_Item( int idx, int pos );
int GTH_ProcessMessage_PCTrade_Item();
int GTH_SendMessage_PCTrade_Ok( int idx );
int GTH_ProcessMessage_PCTrade_Ok();
int GTH_SendMessage_PCTrade_Cancel( int idx );
int GTH_ProcessMessage_PCTrade_Cancel();
int GTH_SendMessage_PCTrade_Confirm( int idx, int type );
int GTH_ProcessMessage_PCTrade_Confirm();
int GTH_SendMessage_PCTrade_SE( int idx, int se );
int GTH_ProcessMessage_PCTrade_SE();


int GTH_SendMessage_PCTrade_RequestBatting( char *pname );
int GTH_ProcessMessage_PCTrade_RequestBatting();
int GTH_SendMessage_PCTrade_AdmitBatting();
int GTH_ProcessMessage_PCTrade_AdmitBatting();
int GTH_SendMessage_PCTrade_RejectBatting();
int GTH_ProcessMessage_PCTrade_RejectBatting();
int GTH_SendMessage_PCTrade_ExitBatting();
int GTH_ProcessMessage_PCTrade_ExitBatting();

int GTH_SendMessage_PCTrade_ItemBatting( int idx, int pos );
int GTH_ProcessMessage_PCTrade_ItemBatting();
int GTH_SendMessage_PCTrade_OkBatting( int idx );
int GTH_ProcessMessage_PCTrade_OkBatting();
int GTH_SendMessage_PCTrade_CancelBatting( int idx );
int GTH_ProcessMessage_PCTrade_CancelBatting();
int GTH_SendMessage_PCTrade_ConfirmBatting( int idx, int type );
int GTH_ProcessMessage_PCTrade_ConfirmBatting();
int GTH_SendMessage_PCTrade_SEBatting( int idx, int se );
int GTH_ProcessMessage_PCTrade_SEBatting();


int GTH_ProcessMessage_Party();
int GTH_ProcessMessage_Party_Error();
int GTH_SendMessage_Party_Request_Organize( int idxOnServer );
int GTH_ProcessMessage_Party_Request_Organize();
int GTH_SendMessage_Party_Admit_Organize();	
int GTH_ProcessMessage_Party_Admit_Organize();	
int GTH_SendMessage_Party_Reject_Organize( int type );
int GTH_ProcessMessage_Party_Reject_Organize();

int GTH_SendMessage_Party_Request_Join( int idxOnServer );
int GTH_ProcessMessage_Party_Request_Join();
int GTH_SendMessage_Party_Admit_Join();	
int GTH_ProcessMessage_Party_Admit_Join();	
int GTH_SendMessage_Party_Reject_Join( int type );
int GTH_ProcessMessage_Party_Reject_Join();

int GTH_SendMessage_Party_Out();
int GTH_ProcessMessage_Party_Out();

int GTH_SendMessage_Party_ForcedOut( char* memberName );
int GTH_ProcessMessage_Party_ForcedOut();

int GTH_SendMessage_Party_Disorganize();
int GTH_ProcessMessage_Party_Disorganize();

int GTH_ProcessMessage_Party_Request_SendAll();
int GTH_ProcessMessage_Party_Update();


int GTH_SendMessage_Party_Request_CanJoin( int type );
int GTH_ProcessMessage_Party_Request_CanJoin();
int GTH_SendMessage_Party_Request_ItemDivideType( int type );
int GTH_ProcessMessage_Party_Request_ItemDivideType();
int GTH_SendMessage_Party_ChatMessage( char *str );
int GTH_ProcessMessage_Party_ChatMessage();



int GTH_SendMessage_Request_SkillLevelUp( int skillIdx, int genType );
int GTH_ProcessMessage_Reply_SkillLevelUp();
int GTH_ProcessMessage_Reply_AddSkill();


bool	GTH_ProcessMessage_Repl_SkillError();



int GTH_ProcessMessage_Affect();
int GTH_SendMessage_HotKey( int curIdx );

int GTH_SendMessage_Request_LearnSkill( int invenPos );
int GTH_ProcessMessage_Reply_LearnSkill();
int GTH_SendMessage_Request_UsePortal( int invenPos );
int GTH_ProcessMessage_Reply_UsePortal();

int GTH_SendMessage_Request_SavePortalPosition();
int GTH_ProcessMessage_Reply_SavePortalPosition();

int GTH_SendMessage_Request_PCInfo( int type, int skillType = 0, int curPage = 0 );
int GTH_ProcessMessage_Reply_PCInfo();


int	GTH_SendMessage_Request_UsePotion(int invenPos);
int	GTH_ProcessMessage_Reply_UsePotion();


void GTH_ProcessMessage_QuestStatusInfo();
void GTH_ProcessMessage_QuestStep();
void GTH_ProcessMessage_QuestItemInfo();
void GTH_SendMessage_DeleteQuestStatus( int questIdx );


void GTH_ProcessMessage_AutoQuestStatusInfo();
void GTH_ProcessMessage_AutoQuestIsValid();


void GTH_SendMessage_ItemSystem_DepotGetItem( int pos, int numStack = 0 );
void GTH_SendMessage_ItemSystem_DepotItem( int pos );
void GTH_SendMessage_ItemSystem_DepotNak( int type, int depotSE );

void GTH_ProcessMessage_ItemSystem();
void GTH_ProcessMessage_ItemSystem_DepotOpen();
void GTH_ProcessMessage_ItemSystem_DepotGetItem();
void GTH_ProcessMessage_ItemSystem_DepotItem();
void GTH_ProcessMessage_ItemSystem_DepotNak();


void GTH_SendMessage_ItemSystem_PrecocityItem( int pos );
void GTH_ProcessMessage_ItemSystem_PrecocityOpen();
void GTH_ProcessMessage_ItemSystem_PrecocityItem();
void GTH_ProcessMessage_ItemSystem_PrecocityComplete();


void GTH_SendMessage_ItemSystem_CraftClose();


void GTH_SendMessage_ItemSystem_CraftItem( int posType, int pos,int craftIdx  );

void GTH_SendMessage_ItemSystem_CraftPrepare();
void GTH_SendMessage_ItemSystem_CraftComplete();

void GTH_ProcessMessage_ItemSystem_CraftOpen();
void GTH_ProcessMessage_ItemSystem_CraftClose();
void GTH_ProcessMessage_ItemSystem_CraftItem();
void GTH_ProcessMessage_ItemSystem_CraftPrepare();
void GTH_ProcessMessage_ItemSystem_CraftComplete();


void GTH_SendMessage_ItemSystem_ChatCommand(int nEventIndex);
void GTH_SendMessage_ItemSystem_EventCraftClose();
void GTH_SendMessage_ItemSystem_EventCraftItem( int posType, int pos );
void GTH_SendMessage_ItemSystem_EventCraftPrepare();
void GTH_SendMessage_ItemSystem_EventCraftComplete();

void GTH_ProcessMessage_ItemSystem_EventCraftOpen();
void GTH_ProcessMessage_ItemSystem_EventCraftClose();
void GTH_ProcessMessage_ItemSystem_EventCraftItem();
void GTH_ProcessMessage_ItemSystem_EventCraftPrepare();
void GTH_ProcessMessage_ItemSystem_EventCraftComplete();
void GTH_ProcessMessage_ItemSystem_EventCraftLoad();





void GTH_SendMessage_ItemSystem_BoothSellOpen();
void GTH_SendMessage_ItemSystem_BoothSellItem( int pos, int price );
void GTH_SendMessage_ItemSystem_BoothSellStart( char* boothName, char* boothInfo );
void GTH_SendMessage_ItemSystem_BoothSellClose();
void GTH_SendMessage_ItemSystem_BoothBuyOpen( int idxOnServer );
void GTH_SendMessage_ItemSystem_BoothBuyItem( int pos, int itemCount );
void GTH_SendMessage_ItemSystem_BoothBuyClose();

void GTH_ProcessMessage_ItemSystem_BoothSellOpen();
void GTH_ProcessMessage_ItemSystem_BoothSellItem();
void GTH_ProcessMessage_ItemSystem_BoothSellStart();
void GTH_ProcessMessage_ItemSystem_BoothSellClose();
void GTH_ProcessMessage_ItemSystem_BoothBuyOpen();
void GTH_ProcessMessage_ItemSystem_BoothBuyItem();
void GTH_ProcessMessage_ItemSystem_BoothBuyClose();
void GTH_ProcessMessage_ItemSystem_BoothBuyUpdate();



void GTH_SendMessage_ItemSystem_ExchangeClose();
void GTH_SendMessage_ItemSystem_ExchangeItem( int battleType, int levelType, int pos );
void GTH_SendMessage_ItemSystem_ExchangePrepare();


void GTH_ProcessMessage_ItemSystem_ExchangeClose();
void GTH_ProcessMessage_ItemSystem_ExchangeItem();
void GTH_ProcessMessage_ItemSystem_ExchangePrepare();



void GTH_SendMessage_ItemSystem_UpgradeClose();
void GTH_SendMessage_ItemSystem_UpgradeItem( int posType, int pos );

void GTH_SendMessage_ItemSystem_UpgradePrepare( int selectedOptionIdx );
void GTH_SendMessage_ItemSystem_UpgradeComplete( int selectedOptionIdx );



void GTH_SendMessage_ItemSystem_NpcInventoryClose();
void GTH_SendMessage_ItemSystem_AutoCraftItem( int posType, int pos );
void GTH_SendMessage_ItemSystem_AutoCraftPrepare();


void GTH_ProcessMessage_ItemSystem_NpcInventoryOpen();
void GTH_ProcessMessage_ItemSystem_NpcInventoryClose();
void GTH_ProcessMessage_ItemSystem_AutoCraftItem();
void GTH_ProcessMessage_ItemSystem_AutoCraftPrepare();


void GTH_ProcessMessage_ItemSystem_UpgradeOpen();
void GTH_ProcessMessage_ItemSystem_UpgradeClose();
void GTH_ProcessMessage_ItemSystem_UpgradeItem();
void GTH_ProcessMessage_ItemSystem_UpgradePrepare();
void GTH_ProcessMessage_ItemSystem_UpgradeComplete();


void GTH_SendMessage_ItemSystem_DeletePrepare(int selectedOptionIdx);
void GTH_SendMessage_ItemSystem_DeleteComplete(int selectedOptionIdx);
void GTH_ProcessMessage_ItemSystem_DeletePrepare();
void GTH_ProcessMessage_ItemSystem_DeleteComplete();



int GTH_ProcessMessage_Friends();

void GTH_ProcessMessage_Friends_Invite();
void GTH_ProcessMessage_Friends_Reject();
void GTH_ProcessMessage_Friends_Remove();
void GTH_ProcessMessage_Friends_List();
void GTH_ProcessMessage_Friends_Add();
void GTH_ProcessMessage_Friends_Change();

int GTH_SendMessage_Friends_Invite( int idx );
void GTH_SendMessage_Friends_Accept(int idx);
void GTH_SendMessage_Friends_Reject(int idx);
void GTH_SendMessage_Friends_Remove(char *name);


int GTH_ProcessEventMessage_MonsterSkill();


int	GTH_ProcessEventMessage_MonsterSkillCasting();




void GTH_SendMessage_Guild_Organize_Request( char *name, char *guildMark );
void GTH_SendMessage_Guild_Disorganize_Request();
void GTH_SendMessage_Guild_Secede_Request();
void GTH_SendMessage_Guild_Banish_Request( char *name );
void GTH_SendMessage_Guild_Invite_Request( char *name );
void GTH_SendMessage_Guild_Invite_Reply( int reply );
void GTH_SendMessage_Guild_Invite_Cancel();
void GTH_SendMessage_Guild_GuildMarkInfo();
void GTH_SendMessage_Guild_GuildMark( int guildIdx );
void GTH_SendMessage_Guild_Subscribe_Request( char *guildName );
void GTH_SendMessage_Guild_Change_Master_Request( char *name );
void GTH_SendMessage_Guild_Add_Submaster_Request( char *name );
void GTH_SendMessage_Guild_Add_Factionmaster_Request( char *name );
void GTH_SendMessage_Guild_Delete_Master_Request( char *name );
void GTH_SendMessage_Guild_Schedule_Request( char *notice );


void GTH_ProcessMessage_Guild_Organize_Reply();
void GTH_ProcessMessage_Guild_Disorganize_Reply();
void GTH_ProcessMessage_Guild_Invite_Request();
void GTH_ProcessMessage_Guild_Invite_Cancel();
void GTH_ProcessMessage_Guild_Invite_Reply();
void GTH_ProcessMessage_Guild_Secede_Reply();
void GTH_ProcessMessage_Guild_Banish_Reply();
void GTH_ProcessMessage_Guild_Change_Master_Reply();
void GTH_ProcessMessage_Guild_Add_SubMaster_Reply();
void GTH_ProcessMessage_Guild_Remove_SubMaster_Reply();
void GTH_ProcessMessage_Guild_Add_FactionMaster_Reply();
void GTH_ProcessMessage_Guild_Remove_FactionMaster_Reply();
void GTH_ProcessMessage_Guild_Delete_Master_Reply();
void GTH_ProcessMessage_Guild_Subscribe_Reply();
void GTH_ProcessMessage_Guild_Subscribe_Request();
void GTH_ProcessMessage_Guild_Update();
void GTH_ProcessMessage_Guild_Reply();
void GTH_ProcessMessage_Guild_Request();



void GTH_SendMessage_QuestScroll_ObtainPrepare( int depart, int level, int reward );
void GTH_SendMessage_QuestScroll_ObtainComplete( int depart, int level, int reward, int qsTableIdx );
void GTH_SendMessage_QuestScroll_ObtainClose();
void GTH_SendMessage_QuestScroll_ComboListChange( int depart );

void GTH_ProcessMessage_QuestScroll_ObtainOpen();
void GTH_ProcessMessage_QuestScroll_ObtainPrepare();
void GTH_ProcessMessage_QuestScroll_ObtainComplete();
void GTH_ProcessMessage_QuestScroll_ObtainClose();
void GTH_ProcessMessage_QuestScroll_ComboListChange();
void GTH_ProcessMessage_QuestScrollComplete();


void GTH_ProcessMessage_AutoQuestScroll_ObtainOpen();
void GTH_ProcessMessage_AutoQuestScrollComplete();
void GTH_ProcessMessage_ItemSystem_AutoError(void);

void GTH_SendMessage_AutoQuestScroll_Progress(int msgProgress);


int GTH_ProcessEventMessage_Before_Teleport();
int	GTH_ProcessEventMessage_Teleport();
int GTH_ProcessEventMessage_SocialAnimaton(int event);
void GTH_ProcessEventMessage_CompleteEffect();

int GTH_ProcessMessage_Extend();


int	GTH_ProcessMessage_Extend_Second();						



void GTH_ProcessMessage_MailSystem();
void GTH_ProcessMessage_MailSystem_Send();
void GTH_ProcessMessage_MailSystem_Receive();
void GTH_ProcessMessage_MailSystem_State();
void GTH_ProcessMessage_MailSystem_View();
void GTH_ProcessMessage_MailSystem_Arrive();
void GTH_SendMessage_MailSystem_Send( char *toName, char *mailTitle, char *mailStr );
void GTH_SendMessage_MailSystem_Receive();
void GTH_SendMessage_MailSystem_Delete( int mailIdx[], int count );
void GTH_SendMessage_MailSystem_View( int mailIdx );
void GTH_SendMessage_MailSystem_Save( int mailIdx[], int count );



void GTH_ProcessMessage_WorldChatting();
void GTH_SendMessage_WorldChatting(char* msg,const BOOL bUseSpeaker=FALSE);




void GTH_ProcessMessage_WorldChatting_Count();


void GTH_SendMessage_WorldChatting_Count( );



struct tagPcSummonTypedef{
public:
	enum enumPcSummonType
	{
		PC_SUMMPN_CHOICE=0,
		PC_SUMMON_FORCED,	
		PC_SUMMON_ITEM,
	};
};

void GTH_ProcessMessage_RequestSummons();




void GTH_SendChatBan_Flag();
void GTH_ProcessChatBan_Answerflag();
void GTH_ProcessChatBan_AnswerStart();
void GTH_SendChatBanRequestMessage(char* pName, char* pLimiteTime);
void GTH_SendChatBan_Cancel();
void GTH_ProcessChatBan_AnswerCancel();
void GTH_SendChatBan_GMCancel(char* pName);
void GTH_ProcessChatBan_AnswerGMCancel();
void GTH_ProcessChatBan_AnswerGMStart();


void GTH_SendMessage_SkinPart(const BOOL bSaveSQLDB);




void GTH_SendMessage_Request_ChaosValue();
void GTH_ProcessMessage_AnswerChaosValue();
void GTH_ProcessMessage_SendAddChaosInfo();
void GTH_ProcessMessage_SendDecChaosInfo();

void GTH_SendMessage_ReloadLogic();										



int GTH_SendMessage_Request_MasicBook(int invenPos);


int GTH_ProcessMessage_Reply_UseMasicBook();
int GTH_ProcessMessage_Reply_MasicBook_RepairItem();


bool	GTH_UseItem_Delete(int invenPos);


void GTH_ProcessMessage_EpackChargSoul(void);
void GTH_SendMessage_Request_EpackChargSoul(int invenPos);


int GTH_ProcessMessageReply_RepairItemConfirm();
void GTH_SendMessageRequest_RepairItemConfirm(int RepairType, int invenType, int pos );



#include "../global_def.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 



struct LessServerGroup_descending_state : public binary_function < const serverGroup_t&, const serverGroup_t &, bool>
{
    LessServerGroup_descending_state(int nB = 0) : nBasic(nB) {} 
	
    bool operator() (const serverGroup_t &ritem_state, const serverGroup_t &litem_state) const
    {	
		
		if( ritem_state.active == FALSE )
			return FALSE;
		
		if( ritem_state.name == NULL)
			return FALSE;
		else
		{
			if ( litem_state.active == TRUE)
			{
				
				if ( nBasic == 0)													
					return (strcmp(ritem_state.name, litem_state.name) < 0);			      
				
				
				if ( nBasic == 1)				
					return !(strcmp(ritem_state.name, litem_state.name) < 0);
				
				
				if ( nBasic == 2)				
					return abs(ritem_state.clientNumber ) < abs(litem_state.clientNumber);															
				
				
				if ( nBasic == 3)				
					return abs(ritem_state.clientNumber ) > abs(litem_state.clientNumber );														
				
			}			
		}
		return TRUE;		
    }
    int nBasic;
};

	

void GTH_Chao_point_event();

