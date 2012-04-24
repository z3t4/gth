#define RELIABLE_WAITTIME		1000		
#define CONNECT_WAITTIME		5000		
#define MAX_RESENDING_TIMES		10			
#define MAX_PACKET_PER_FRAME	200			




int GTH_Network_Startup();
void GTH_Network_Shutdown();

void GTH_CheckProblemSocket();
BOOL GTH_IsValidPacketForNewClient(i3packet_t *p);
int GTH_FindPCWithAddr(struct sockaddr_in *addr);
int	GTH_Network_GetMessage(i3socket_t *sock);
int GTH_ListenSocket();

int GTH_ConnectGameServer();
int GTH_DisconnectGameServer();
int GTH_DisconnectGameServer(int idx);							
int GTH_ReplyConnectGameServer(playerCharacter_t *pc);
 
int GTH_SendSyncMessage();
void GTH_SendChatMessage();
void GTH_SendChatMessage_Whisper(void);
void GTH_SendChatMessage_ServerWhisper(void);
void GTH_SendMessage_ReliablePing( i3socket_t *sock );
void GTH_SendMessage_UnreliablePing( i3socket_t *sock );

void MSG_WritePosition(i3sizebuf_t *sb, vec3_t pos);
void MSG_ReadPosition(vec3_t v);
void MSG_WriteItem(i3sizebuf_t *sb, item_t *item);
void MSG_ReadItem(item_t *item);
void MSG_WriteMember(i3sizebuf_t *sb, partyMember_t *m);
void MSG_ReadMember( partyMember_t *m );
void MSG_WriteAffect( i3sizebuf_t *sb, affect_t *aff );
void MSG_ReadAffect( affect_t *aff );


void MSG_ReadPCList( entityPCList_t *pl );
void MSG_WritePCList( i3sizebuf_t *sb, entityPCList_t *pl );


int GetNumberEquipmentItem( playerCharacter_t* pc );
int GTH_FindPCByName(char *name);
int GTH_FindGameMaster();
int GTH_FindGameMasterWithCondition(int worldIdx);


void GTH_Send_Subscribe_Request_Message_To_Client( char *toName );
void GTH_Send_Subscribe_Request_Message_To_Server( char *toName, char *fromName, int fromMemberIdx );


void GTH_DisplayClientList();
void GTH_Debug_PacketInfo();
void GTH_Debug_ServerStatusInfo();

void GTH_SendMessage_SetShutdownTimer( playerCharacter_t *pc );
void GTH_SendMessage_NoticeMessage( playerCharacter_t *pc, char *notice );
void GTH_ProccessMessage_ValidationServerAdmin( playerCharacter_t *pc );

void GTH_SendMessage_RequestSummons();
void GTH_SendMessage_ServerRequestSummons();
void GTH_SendMessage_ReplySummons(int reply, char *toName, char *fromName);
void GTH_SendMessage_ServerReplySummons();


void GTH_SendMessage_AckMessage(const char* in_strToName);
void GTH_SendMessage_ServerAckMessage(void);
void GTH_SendMessage_NotFoundPC( playerCharacter_t *pc );


void GTH_SendMessag_AckMessageChatBan(char *fromName,bool bretval);



void GTH_SendMessage_ChatBanFlagAnswer(playerCharacter_t *pc);

void GTH_SendMessag_ChaosValue();
void GTH_SendMessage_AddChaosInfo(playerCharacter_t *pc,int chaosvalue,bool sendclass,bool sendspeed);
void GTH_SendMessage_DecChaosInfo(playerCharacter_t *pc,int classtype);

enum 
{
	REJECT_OVER_MAXCLIENT = 0,
	REJECT_VERSION_MISMATCH = 1,
};

void GTH_SendMessage_RejectConnection(struct sockaddr_in *addr, int reason);


void GTH_SendMessage_ReplyCheatCode( playerCharacter_t *pc, int type );





void GTH_ProcessMessage_Request_OtherServer();
void GTH_ProcessMessage_Reply_OtherServer();

void GTH_SendMessage_PCList_Request_Add( entityPCList_t *pl );
void GTH_SendMessage_PCList_Request_Delete( entityPCList_t *pl );
void GTH_SendMessage_PCList_Request_Update ( entityPCList_t *pl );

void GTH_SendMessage_PCList_Reply_Add( entityPCList_t *pl );
void GTH_SendMessage_PCList_Reply_Delete( entityPCList_t *pl );
void GTH_SendMessage_PCList_Reply_Update ( entityPCList_t *pl );

void GTH_ProcessMessage_PCList_Request_Add();
void GTH_ProcessMessage_PCList_Request_Delete();
void GTH_ProcessMessage_PCList_Request_Update();

int GTH_RegistMemberServer();
int GTH_AddToServerGroup(int memberIdx, struct sockaddr_in *addr);
int GTH_FindMemberServerWithAddr(struct sockaddr_in *addr);
void GTH_SendMessage_DropServerGroup();
void GTH_SendServerGroupInfo();

int GTH_DropMyServer();
int GTH_MyServerInfo();
int GTH_InitMemberServer();


void GTH_UpdateMyServerInfo();

void GTH_SendMessage_MyServerInfo();
void GTH_SendMessage_DropMyServer();
void GTH_SendMessage_GameServerGroupInfo();

int GTH_GetMemberServerIdx(i3packet_t *p);

void GTH_SendMessage_MemberServerInfo();
int GTH_MemberServerInfo();

int GTH_FindServerWithinGroup(int worldIdx);
int GTH_IsLocalServiceWorld(int worldIdx);

void GTH_CheckProblemMemberServer();
void GTH_CheckProblemMasterServer();

void GTH_ProcessMessage_SystemAdmin();
void GTH_SendMessage_SystemShutdownToMember();
void GTH_SendMessage_SystemShutdownToManager();

void GTH_SendMessage_NoticeToMember( char *notice );
void GTH_SendMessage_NoticeToManager( char *notice );
void GTH_SendMessage_CallGameMaster();
void GTH_SendMessage_ServerCallGameMaster();

void GTH_SendMessage_RequestFindPCToManager(int reqMemberIdx, char *toName, char *fromName );
void GTH_SendMessage_RequestFindPCToMember( int reqMemberIdx, char *toName, char *fromName );
void GTH_SendMessage_ReplyFindPCToManager(int reqMemberIdx, int worldIdx, vec3_t position, char *fromName );
void GTH_SendMessage_ReplyFindPCToMember(int reqMemberIdx, int worldIdx, vec3_t position, char *fromName );
void GTH_SendMessage_ReplySummonMonster( playerCharacter_t* pc, int reply, int error );


void GTH_ProcessMessage_Request_PartyInfo();

void GTH_ProcessMessage_PartyInfo_Request_Organize();
void GTH_SendMessage_PartyInfo_Reply_Organize( int memberServerIdx, int partyIdx, partyMember_t *l, partyMember_t *m, int organizeTime, int isLeader );

void GTH_ProcessMessage_PartyInfo_Request_Join();
void GTH_SendMessage_PartyInfo_Reply_Join( serverPartyInfo_t *pi, int memberServerIdx, int pcIdx, int isNewMember );

void GTH_ProcessMessage_PartyInfo_Request_Out();
void GTH_ProcessMessage_PartyInfo_Request_ForcedOut();
void GTH_SendMessage_PartyInfo_Reply_Out( int isForced, partyMember_t *m, int memberIdx, int nextLeaderIdx, int reqFlag );
void GTH_ProcessMessage_PartyInfo_Request_Disorganize();
void GTH_SendMessage_PartyInfo_Reply_Disorganize( partyMember_t *m );

void GTH_ProcessMessage_PartyInfo_Request_SendAll();
void GTH_SendMessage_PartyInfo_Reply_SendAll( int reply, int serverIdx, int pcIdx, serverPartyInfo_t *pi );
void GTH_ProcessMessage_PartyInfo_Request_Update();
void GTH_SendMessage_PartyInfo_Reply_Update( partyMember_t *m, int memberIdx, int nextLeaderIdx, int active, int serverIdx, int pcIdx, int level, int worldIdx );
void GTH_ProcessMessage_PartyInfo_Request_ItemDivideType();
void GTH_SendMessage_PartyInfo_Reply_ItemDivideType( partyMember_t *m, int type );
void GTH_ProcessMessage_PartyInfo_Request_Holding();
void GTH_ProcessMessage_PartyInfo_Request_ChatMessage();
void GTH_SendMessage_PartyInfo_Reply_ChatMessage( partyMember_t *m, char* fromName, char *message );


void GTH_ProcessMessage_Reply_PartyInfo();

void GTH_ProcessMessage_PartyInfo_Reply_Organize();
void GTH_SendMessage_PartyInfo_Request_Organize( partyMember_t* l, partyMember_t *m, int organizeTime);

void GTH_ProcessMessage_PartyInfo_Reply_Join();
void GTH_SendMessage_PartyInfo_Request_Join( partyMember_t *m, char *organizerName, int organizeServer, int organizeTime );

void GTH_SendMessage_PartyInfo_Request_Out( int isForced, char* memberName, char* organizerName, int organizeServer, int organizeTime );
void GTH_ProcessMessage_PartyInfo_Reply_Out();
void GTH_ProcessMessage_PartyInfo_Reply_ForcedOut();
void GTH_SendMessage_PartyInfo_Request_Disorganize( char *organizerName, int organizeServer, int organizeTime );
void GTH_ProcessMessage_PartyInfo_Reply_Disorganize();


void GTH_SendMessage_PartyInfo_Request_SendAll( playerCharacter_t *pc );
void GTH_ProcessMessage_PartyInfo_Reply_SendAll();
void GTH_SendMessage_PartyInfo_Request_Update( char* memberName, int active, int serverIdx, int pcIdx, int level, int worldIdx, char *organizerName, int organizeServer, int organizeTime );
void GTH_ProcessMessage_PartyInfo_Reply_Update();

void GTH_SendMessage_PartyInfo_Request_ItemDivideType( char *organizerName, int organizeServer, int organizeTime, int type );
void GTH_ProcessMessage_PartyInfo_Reply_ItemDivideType();

void GTH_SendMessage_PartyInfo_Request_Holding( char *organizerName, int organizeServer, int organizeTime, int holdingFlag );

void GTH_SendMessage_PartyInfo_Request_ChatMessage( char *fromName, char *organizerName, int organizeServer, int organizeTime, char *message );
void GTH_ProcessMessage_PartyInfo_Reply_ChatMessage();



int GTH_SendMonsterEventMessage_Idle( monsterCharacter_t *monster );
int GTH_SendMonsterEventMessage_Walk( monsterCharacter_t *monster );
int GTH_SendMonsterEventMessage_Back( monsterCharacter_t *monster);
int GTH_SendMonsterEventMessage_Run( monsterCharacter_t *monster);
int GTH_SendMonsterEventMessage_TurnLeft( monsterCharacter_t *monster );
int GTH_SendMonsterEventMessage_TurnRight( monsterCharacter_t *monster );
int GTH_SendMonsterEventMessage_MoveLeft( monsterCharacter_t *monster );
int GTH_SendMonsterEventMessage_MoveRight( monsterCharacter_t *monster );
int GTH_SendMonsterEventMessage_Attack( monsterCharacter_t *monster );
int GTH_SendMonsterEventMessage_Damage( monsterCharacter_t *monster, int stunFlag = false );
int GTH_SendMonsterEventMessage_Die( monsterCharacter_t *monster );


int	GTH_SendMonsterEventMessage_Skill( monsterCharacter_t* monster );

int GTH_SendMonsterEventMessage_State( monsterCharacter_t *monster );


int GTH_SendPCEventMessage_Idle( playerCharacter_t *pc );
int GTH_SendPCEventMessage_Walk( playerCharacter_t *pc );
int GTH_SendPCEventMessage_WalkLeft( playerCharacter_t *pc );
int GTH_SendPCEventMessage_WalkRight( playerCharacter_t *pc );
int GTH_SendPCEventMessage_Back( playerCharacter_t *pc );
int GTH_SendPCEventMessage_BackLeft( playerCharacter_t *pc );
int GTH_SendPCEventMessage_BackRight( playerCharacter_t *pc );
int GTH_SendPCEventMessage_Run( playerCharacter_t *pc );
int GTH_SendPCEventMessage_RunLeft( playerCharacter_t *pc );
int GTH_SendPCEventMessage_RunRight( playerCharacter_t *pc );
int GTH_SendPCEventMessage_TurnLeft( playerCharacter_t *pc );
int GTH_SendPCEventMessage_TurnRight( playerCharacter_t *pc );
int GTH_SendPCEventMessage_MoveLeft( playerCharacter_t *pc );
int GTH_SendPCEventMessage_MoveRight( playerCharacter_t *pc );
int GTH_SendPCEventMessage_Attack( playerCharacter_t *pc, int damageAttackFlag, int success, int itemIdx );
int	GTH_SendPCEventMessage_Skill( playerCharacter_t *pc, int skillIdx, int coolTime, int delayTime );


int GTH_SendPCEventMessage_SkillCasting( playerCharacter_t *pc, int castingTime ,int skillIdx,int skillTargetType , int UseType );


int GTH_SendPCEventMessage_Die( playerCharacter_t *pc, int dropItemType, int dropItemPosition );



int GTH_SendPCEventMessage_Damage( playerCharacter_t *pc, int itemIdx, int stunFlag = false );
int GTH_SendPCEventMessage_Respawn( playerCharacter_t *pc );
int GTH_SendPCEventMessage_MouseWalk( playerCharacter_t *pc );
int GTH_SendPCEventMessage_MouseRun( playerCharacter_t *pc );
int GTH_SendPCEventMessage_Sit( playerCharacter_t *pc );
int GTH_SendPCEventMessage_Stand( playerCharacter_t *pc );
int GTH_SendPCEventMessage_Preattack( playerCharacter_t *pc );


int GTH_SendPCEventMessage_Shout( playerCharacter_t *pc );
int GTH_SendPCEventMessage_Bow( playerCharacter_t *pc );


#include "..\tagGCPacket.h"
int GTH_SendPCMessage_TransportReply(playerCharacter_t* pToPlayerRecord,const tagGCPacket_TRANSPORTWORLDREP::enumCode code);
void GTH_SendPCMessage_ChangeServer( playerCharacter_t *pc, memberServer_t *member );
void GTH_SendPCMessage_ReplyWeaponChange( playerCharacter_t* pc );
void GTH_SendPCMessage_LevelUp( playerCharacter_t* pc, __int64 exp, __int64 genExp, int lootNSE, int genCapability, int skillIdx[], int skillNumber, int sharingType );
void GTH_SendPCMessage_ReplyGenChange( playerCharacter_t* pc);
void GTH_SendPCMessage_ReplyStatusUp( playerCharacter_t* pc, int reply, int error, int statusType );
void GTH_SendPCMessage_AutoRecovery( playerCharacter_t* pc );
void GTH_SendPCMessage_ReplyEpackUse( playerCharacter_t* pc, int type );


void GTH_SendPCMessage_Affect( playerCharacter_t *pc );
void GTH_SendMonsterMessage_Affect( monsterCharacter_t *mon );



int GTH_SendMessage_SyncItemObject( playerCharacter_t *pc );

void GTH_SendMessage_AddMultiItemObject( itemObject_t itemObject[], int number, vec3_t origin );
void GTH_SendMessage_AddItemObject(itemObject_t *itemObject);
void GTH_SendMessage_DeleteItemObject(itemObject_t *itemObject);

void GTH_SendMessage_ReplyGetInventoryItem(playerCharacter_t *pc, int reply, int error, int inventoryPos, int equipPos);
void GTH_SendMessage_ReplyGetEquipItem(playerCharacter_t *pc, int reply, int error, int inventoryPos, int equipPos);

void GTH_SendMessage_ReplyDropItem(playerCharacter_t *pc, int reply, int error, int itemIdx);
void GTH_SendMessage_ReplyPickUpItem(playerCharacter_t *pc, int reply, int error, item_t *item );

void GTH_SendMessage_LootValue( playerCharacter_t *pc, __int64 exp, __int64 genExp, int lootNSE, int genCapability, int sharingType );
void GTH_SendMessage_DestroyItem( playerCharacter_t *pc, int reply, int error, int itemIdx );

void GTH_SendMessage_AddInvItem( playerCharacter_t *pc, int reply, int error, item_t *item );
void GTH_SendMessage_DeleteInvItem( playerCharacter_t *pc, int reply, int error, int itemIdx, int number );



int GTH_SendMessage_ReplyEntityMenu( playerCharacter_t* pc, int reply, int error, int entityType, int menuType, int menuFlag, int bindingCost, int type );
int GTH_SendMessage_ReplyStore( playerCharacter_t* pc, storeStruct_t *store, int type = 0 );
int GTH_SendMessage_ReplyBuyItem(playerCharacter_t *pc, int reply, int error, item_t *item, int storeItemIdx, int curNumber );
int GTH_SendMessage_ReplySellItem(playerCharacter_t *pc, int reply, int error, int itemIdx);

int GTH_SendMessage_ReplyNpcChat( playerCharacter_t* pc, int reply, int error );
int GTH_SendMessage_ReplyQuitNpcChat( playerCharacter_t* pc, int reply, int error );
int GTH_SendMessage_NpcMessage( playerCharacter_t* pc );

int GTH_SendMessage_ReplyBinding( playerCharacter_t* pc, int reply, int error, int type, int bindingIdx, int bindingCost );


int GTH_SendMessage_ReplyEpackStore( playerCharacter_t* pc, int reply, int error,int EpkRemainCount );

int GTH_SendMessage_ReplyRepairItem( playerCharacter_t* pc, int reply, int error, int type, int repairItemIdx[], int repairItemDur[], int numOfRepairItem );
int GTH_SendMessage_ReplyBuySkill( playerCharacter_t *pc, int reply, int error, int inventoryPos, int buySphereCost, int skillIdx );

int GTH_SendMessage_QuestLogic_ChatMsg( playerCharacter_t *pc, int idx,int type );
int GTH_SendMessage_QuestLogic_Awaken( playerCharacter_t *pc );
int GTH_SendMessage_QuestLogic_InitStatus( playerCharacter_t *pc, int reply, int error, int type, int nak );

int GTH_SendMessage_NPCInfo( playerCharacter_t * pc );



void GTH_SendMessage_PCTrade_Request( playerCharacter_t* pc, int reply, int error, int targetIdx, int type  );
void GTH_SendMessage_PCTrade_Admit( playerCharacter_t* pc, int reply, int error );
void GTH_SendMessage_PCTrade_Reject( playerCharacter_t* pc, int reply, int error, int type );
void GTH_SendMessage_PCTrade_Exit( playerCharacter_t* pc, int reply, int error );
void GTH_SendMessage_PCTrade_Item( playerCharacter_t* pc, int reply, int error, int isOwner, int type, int pos, item_t *item );
void GTH_SendMessage_PCTrade_Ok( playerCharacter_t* pc, int reply, int error, int type );
void GTH_SendMessage_PCTrade_Cancel( playerCharacter_t* pc, int reply, int error, int type );
void GTH_SendMessage_PCTrade_Confirm( playerCharacter_t* pc, int reply, int error, int type );
void GTH_SendMessage_PCTrade_SE( playerCharacter_t *pc, int reply, int error, int type, int se );
void GTH_SendMessage_Party_Error( playerCharacter_t* pc );
void GTH_SendMessage_Party_Request_Organize( playerCharacter_t* pc, int reply, int error, int idx, int type );
void GTH_SendMessage_Party_Admit_Organize( playerCharacter_t* pc, int reply, int error, int type );
void GTH_SendMessage_Party_Reject_Organize( playerCharacter_t* pc, int type );

void GTH_SendMessage_Party_Request_Join( playerCharacter_t* pc, int reply, int error, int idx, int type );
void GTH_SendMessage_Party_Admit_Join( playerCharacter_t* pc, int reply, int error, int isNewMember );
void GTH_SendMessage_Party_Reject_Join( playerCharacter_t* pc, int type );

void GTH_SendMessage_Party_Out( playerCharacter_t* pc, int reply, int error, int isForced, int outMemberIdx, int newLeaderIdx, int reqFlag );
void GTH_SendMessage_Party_Disorganize( playerCharacter_t *pc );

void GTH_SendMessage_Party_SendAll( playerCharacter_t *pc, int reply );
void GTH_SendMessage_Party_Update( playerCharacter_t *pc, int memberIdx, int newLeaderIdx, int active, int pcIdx, int level, int worldIdx );
void GTH_SendMessage_Party_CanJoin( playerCharacter_t *pc, int type );
void GTH_SendMessage_Party_ItemDivideType( playerCharacter_t *pc, int type );
void GTH_SendMessage_Party_ChatMessage( playerCharacter_t *pc, char* fromName, char *message );


void GTH_SendMessage_Reply_SkillLevelUp( playerCharacter_t *pc, int reply, int error, int skillIdx, int genType );
void GTH_SendMessage_Reply_AddSkill( playerCharacter_t *pc, int skillNumber, int skillIdx[] );
void GTH_SendMessage_Reply_Learn_Skill( playerCharacter_t *pc, int reply, int error, int invenPos, int skillIdx );
void GTH_SendMessage_Reply_Use_Portal( playerCharacter_t *pc, int reply, int error, int invenPos, int isChangeServer = false );
void GTH_SendMessage_Reply_SavePortalPosition( playerCharacter_t *pc, int reply, int error );
void GTH_SendMessage_Reply_PCInfo( playerCharacter_t *pc );

void GTH_SendMessage_Reply_InitSkill( playerCharacter_t* pc );


void GTH_SendMessage_Reply_Use_Potion( playerCharacter_t* pc, int error, int invenPos);




void GTH_ProcessMessage_DelQuestStatus(playerCharacter_t *pc);
void GTH_SendMessage_QuestStatusInfo(playerCharacter_t *pc, int type, int questIdx );

void GTH_SendMessage_AutoQuestStatusInfo( playerCharacter_t *pc, int type, int questIdx );

void GTH_SendMessage_QuestStep(playerCharacter_t *pc, int questIdx, int questStep, int completeFlag );
void GTH_SendMessage_QuestItemInfo( playerCharacter_t *pc, int itemTableIdx, int itemNumber );


void GTH_SendMessage_ItemSystem_DepotOpen( playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_DepotGetItem( playerCharacter_t *pc, int reply, int error, int depotPos );
void GTH_SendMessage_ItemSystem_DepotItem( playerCharacter_t *pc, int reply, int error, int depotPos );
void GTH_SendMessage_ItemSystem_DepotNak( playerCharacter_t *pc, int reply, int error );



void GTH_SendMessage_ItemSystem_PrecocityOpen( playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_PrecocityItem( playerCharacter_t *pc, int reply, int error, int pos );
void GTH_SendMessage_ItemSystem_PrecocityComplete( playerCharacter_t *pc, int pos );


void GTH_SendMessage_ItemSystem_CraftOpen( playerCharacter_t *pc );


void GTH_SendMessage_ItemSystem_EventCraftOpen( playerCharacter_t *pc );


void GTH_SendMessage_ItemSystem_NpcInventoryOpen(playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_NpcInventoryClose(playerCharacter_t *pc);
void GTH_SendMessage_ItemSystem_AutoQuestCraftItem( playerCharacter_t *pc, int reply, int error, int posType, int pos );
void GTH_SendMessage_ItemSystem_AutoCraftPrepare( playerCharacter_t *pc, int &reply, int &error);

void GTH_SendMessage_ItemSystem_CraftClose( playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_CraftItem( playerCharacter_t *pc, int reply, int error, int posType, int pos );


void GTH_SendMessage_ItemSystem_CraftPrepare( playerCharacter_t *pc, int reply, int error, int resultItemIdx[], int nak, float pro ,int craftIdx);

void GTH_SendMessage_ItemSystem_CraftComplete( playerCharacter_t *pc, int reply, int error, int nak );


int	 GTH_UndoPCExchange( playerCharacter_t *pc, int completeFlag = false );
void GTH_SendMessage_ItemSystem_ExchangeClose( playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_ExchangeItem( playerCharacter_t *pc, int reply, int error, int pos, int nak );
void GTH_ExchangeProcess( playerCharacter_t *pc, int &reply, int &error, int &reqNak);
void GTH_SendMessage_ItemSystem_ExchangePrepare( playerCharacter_t *pc, int reply, int error, int nak, float pro );

void GTH_ProcessMessage_ItemSystem_ExchangeClose();
void GTH_ProcessMessage_ItemSystem_ExchangeItem();
void GTH_ProcessMessage_ItemSystem_ExchangePrepare();




void GTH_SendMessage_ItemSystem_EventCraftClose( playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_EventCraftItem( playerCharacter_t *pc, int reply, int error, int posType, int pos );
void GTH_SendMessage_ItemSystem_EventCraftPrepare( playerCharacter_t *pc, int reply, int error );
void GTH_SendMessage_ItemSystem_EventCraftComplete( playerCharacter_t *pc, int reply, int error ,int craftIdx);
void GTH_SendMessage_Reply_Load_EventCraftItem( playerCharacter_t *pc, short sEventIdx , int n1PointCnt, int n3PointCnt,int nTotalPoint );



void GTH_SendMessage_ItemSystem_BoothSellOpen( playerCharacter_t *pc, int reply, int error );





void GTH_SendMessage_ItemSystem_BoothSellItem( playerCharacter_t *pc, int reply, int pos, int  ErrorIdx = 0);

void GTH_SendMessage_ItemSystem_BoothSellStart( playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_BoothSellClose( playerCharacter_t *pc, int type );
void GTH_SendMessage_ItemSystem_BoothBuyOpen( playerCharacter_t *pc, int reply, int error, playerCharacter_t *targetPC, int type );
void GTH_SendMessage_ItemSystem_BoothBuyItem( playerCharacter_t *pc, int reply, int error, int pos, int itemCount, int type );
void GTH_SendMessage_ItemSystem_BoothBuyClose( playerCharacter_t *pc, int reply, int error, int type, char* targetName = NULL );
void GTH_SendMessage_ItemSystem_BoothBuyUpdate( playerCharacter_t *pc, playerCharacter_t *seller );


void GTH_SendMessage_ItemSystem_UpgradeOpen( playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_UpgradeClose( playerCharacter_t *pc );
void GTH_SendMessage_ItemSystem_UpgradeItem( playerCharacter_t *pc, int reply, int error, int posType, int pos );
void GTH_SendMessage_ItemSystem_UpgradePrepare( playerCharacter_t *pc, int reply, int error, int resultItemIdx, int nak, float pro, float vanishPro, float downGradePro, int currentGrade, int nextGrade, int optionIdx );
void GTH_SendMessage_ItemSystem_UpgradeComplete( playerCharacter_t *pc, int reply, int error, int nak, int success, int resultItemIdx, int optionIdx, int nextGrade );


void GTH_SendMessage_ItemSystem_DeletePrepare( playerCharacter_t *pc, int reply, int error, int optionIdx );
void GTH_SendMessage_ItemSystem_DeleteComplete( playerCharacter_t *pc, int reply, int error, int success, int optionIdx );


void GTH_SendMessage_QuestScroll_ObtainOpen( playerCharacter_t *pc, int openFlag );
void GTH_SendMessage_QuestScroll_ObtainPrepare( playerCharacter_t *pc, int reply, int error, int reqNak, int tableIdx );
void GTH_SendMessage_QuestScroll_ObtainComplete( playerCharacter_t *pc, int reply, int error, int reqNak, int tableIdx );
void GTH_SendMessage_QuestScroll_ObtainClose( playerCharacter_t *pc );
void GTH_SendMessage_QuestScrollComplete( playerCharacter_t *pc, int type, int reward, int itemNumber, int rankPoint, int nextScroll=0 );
void GTH_SendMessage_QuestScroll_ComboListChange( playerCharacter_t *pc, int depart );


void GTH_SendMessage_CompleteEffect_To_AllPC( playerCharacter_t *pc, int effectIdx );


void GTH_SendMessage_AutoQuestScroll_ObtainOpen( playerCharacter_t* pPlayer,const int& autoquestindex);
void GTH_SendMessage_AutoQuestScrollComplete( playerCharacter_t *pc, int questindex);
void GTH_SendMessage_AutoQuestScroll_effectisvalid( playerCharacter_t *pc,int &reply,int type);
void GTH_SendMessage_AutoQuest_Error(playerCharacter_t* pc,const int& error);
;








void GTH_SendMessage_Friends_Invite ( playerCharacter_t * pc, int reply, int error, int idx, int type );
void GTH_SendMessage_Friends_Added ( playerCharacter_t *pc, char *name, int level, int job, int worldIdx );
void GTH_SendMessage_Friends_Reject ( playerCharacter_t *pc, int idx );
void GTH_SendMessage_Friends_Remove ( playerCharacter_t *pc, char *name );
void GTH_SendMessage_Friends_List ( playerCharacter_t *pc, entityPCList_t *pl );

void MSG_WriteFriendsList_ManagerToMember ( i3sizebuf_t *sb, entityPCList_t *pl );
void MSG_WriteFriendsLogin_ManagerToMember ( i3sizebuf_t *netMessage, entityPCList_t *src, entityPCList_t *dest );
void MSG_WriteFriendsLogout_ManagerToMember ( i3sizebuf_t *netMessage, entityPCList_t *src, entityPCList_t *dest );
void MSG_WriteFriendsUpdate_ManagerToMember ( i3sizebuf_t *netMessage, entityPCList_t *src, entityPCList_t *dest );

void GTH_SendMessage_Friends_Quit ( playerCharacter_t* pc, char* name );
void GTH_SendMessage_Friends_login ( playerCharacter_t *pc, int level, int job, int worldIdx, char *name );

void GTH_ProcessMessage_Reply_OtherServer_Friends_Change ();
void GTH_ProcessMessage_Reply_OtherServer_Friends_List ();



void GTH_SendMessage_Reply_SendMail( playerCharacter_t *pc, int errorCode, char *toName );
void GTH_SendMessage_Reply_RecvMail( playerCharacter_t *pc, int mailCount, MailEntry *mail );
void GTH_SendMessage_Reply_GetMailState( playerCharacter_t *pc, int notReadNumber, int readNumber, int s_notReadNumber, int s_readNumber );
void GTH_SendMessage_Reply_ViewMail( playerCharacter_t *pc, MailEntry *mail );



int GTH_SendMonsterEventMessage_SkillCasting( monsterCharacter_t *pMonster, int castingTime );
int	GTH_SendMonsterEventMessage_Skill( monsterCharacter_t *monster, int skillIdx, int coolTime, int delayTime );


void GTH_SendMessage_SkillError( playerCharacter_t* pc , int errorIdx);








void GTH_ProcessMessage_Guild_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Organize_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Disorganize_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Add_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Remove_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Banish_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Update_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Connect_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_ChatMessage_Request_From_MemberServer();

void GTH_ProcessMessage_Guild_Change_Master_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Add_SubMaster_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Remove_SubMaster_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Add_FactionMaster_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Remove_FactionMaster_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Schedule_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_GuildMark_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Subscribe_Request_From_MemberServer();
void GTH_ProcessMessage_Guild_Subscribe_Request_From_ManagerServer();

void GTH_SendMessage_Guild_Organize_Reply_To_MemberServer( int serverIdx, int idxOnServer, short guildIdx, int error );
void GTH_SendMessage_Guild_Disorganize_Reply_To_MemberServer( int serverIdx, int idxOnServer, int result, int error );
void GTH_SendMessage_Guild_Update_To_MemberServer( int serverIdx, int idxOnServer, guild_t *guild, int update, int rank, int rankIdx );
void GTH_SendMessage_Guild_Add_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, short guildIdx, int rank, int rankIdx, int result, int error );
void GTH_SendMessage_Guild_Remove_Reply_To_MemberServer( int serverIdx, int idxOnServer, int rank, int result, int error );
void GTH_SendMessage_Guild_Banish_Reply_To_MemberServer( int serverIdx, int idxOnServer, int rank, int result, int error, int who, char *name );
void GTH_SendMessage_Guild_Subscribe_Reply_To_MemberServer( int serverIdx, int idxOnServer, int result, int error, char *guildName = NULL );

void GTH_SendMessage_Guild_Connect_Reply_To_MemberServer( int serverIdx, int idxOnServer, int rankInGuild, int rankIdxInGuild, char *guildName );
void GTH_SendMessage_Guild_ChatMessage_Reply_To_MemberServer( int serverIdx, int idxOnServer, char *name, char *chatMessage );

void GTH_SendMessage_Guild_Change_Master_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, int result, int error, int rankIdxInGuild );
void GTH_SendMessage_Guild_Add_SubMaster_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, int result, int error, int rankIdxInGuild );
void GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, char *accName, int result, int error );
void GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, int result, int error, int rankIdxInGuild );
void GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, char *accName, int result, int error );
void GTH_SendMessage_Guild_GuildMark_Reply_To_MemberServer( int serverIdx, int idxOnServer, short guildIdx, unsigned int updateTime, char *guildMark );


void GTH_ProcessMessage_Guild_Reply_From_ManagerServer();
void GTH_ProcessMessage_Guild_Update_From_ManagerServer();

void GTH_SendMessage_Guild_Organize_Request_To_ManagerServer( playerCharacter_t *pc, char* guildName, char* guildMark );
void GTH_SendMessage_Guild_Disorganize_Request_To_ManagerServer( playerCharacter_t* pc );
void GTH_SendMessage_Guild_Add_Request_To_ManagerServer( playerCharacter_t *reqPC, playerCharacter_t *accPC, int rank );
void GTH_SendMessage_Guild_Remove_Request_To_ManagerServer( playerCharacter_t *pc, int rank );
void GTH_SendMessage_Guild_Banish_Request_To_ManagerServer( short guildIdx, char *reqName, char *accName, int rank );
void GTH_SendMessage_Guild_Update_Request_To_ManagerServer( short guildIdx, char *name, int rank, int rankIdx, int type, int value );

void GTH_SendMessage_Guild_Connect_Reqeust_To_ManagerServer( short guildIdx, char *name );

void GTH_SendMessage_Guild_Change_Master_Request_To_ManagerServer( short guildIdx, playerCharacter_t *reqPC, playerCharacter_t *accPC );
void GTH_SendMessage_Guild_Add_SubMaster_Request_To_ManagerServer( short guildIdx, playerCharacter_t *reqPC, playerCharacter_t *accPC );
void GTH_SendMessage_Guild_Remove_SubMaster_Request_To_ManagerServer( playerCharacter_t *reqPC, char *accName, int accPCJob );
void GTH_SendMessage_Guild_Add_FactionMaster_Request_To_ManagerServer( short guildIdx, playerCharacter_t *reqPC, playerCharacter_t *accPC );
void GTH_SendMessage_Guild_Remove_FactionMaster_Request_To_ManagerServer( playerCharacter_t *reqPC, char *accName, int accPCJob );
void GTH_SendMessage_Guild_Schedule_Request_To_ManagerServer( short guildIdx, playerCharacter_t *pc, char *notice );
void GTH_SendMessage_Guild_GuildMark_Request_To_ManagerServer( playerCharacter_t *pc, short guildIdx );
void GTH_SendMessage_Guild_Subscribe_Request_To_ManagerServer( playerCharacter_t *pc, char *guildName );




void GTH_SendMessage_Guild_Organize_Reply_To_Client( playerCharacter_t *pc, int result, int error , int RemainTime = 0);
void GTH_SendMessage_Guild_Disorganize_Reply_To_Client( playerCharacter_t *pc, int result, int error );
void GTH_SendMessage_Guild_Update_To_Client( playerCharacter_t *pc, guild_t* guild, int update, int rank, int rankIdx );



void GTH_SendMessage_Guild_Invite_Request_To_Client( playerCharacter_t *pc, int result, int error, int who, int	RemainTime = 0);



void GTH_SendMessage_Guild_Invite_Cancel_To_Client( playerCharacter_t *pc, int error, int who, char *name );
void GTH_SendMessage_Guild_Invite_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name );
void GTH_SendMessage_Guild_Secede_Reply_To_Client( playerCharacter_t *pc, int result, int error );
void GTH_SendMessage_Guild_Banish_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name );
void GTH_SendMessage_Guild_Subscribe_Reply_To_Client( playerCharacter_t *pc, int result, int error, char *guildName = NULL );

void GTH_SendMessage_Guild_Message_To_Client( playerCharacter_t *pc, short guildIndex, char *message );
void GTH_SendMessage_Guild_List_Request_From_Client( playerCharacter_t *pc );

void GTH_SendMessage_Guild_Organize_Request_From_Client( playerCharacter_t *pc );

void GTH_SendMessage_Guild_Change_Master_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name );
void GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name );
void GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name );
void GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name );
void GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name );
void GTH_SendMessage_Guild_Delete_Master_Reply_To_Client( playerCharacter_t *pc, int error, char *name );




void GTH_SendMessage_Guild_GuildMark_Reply_To_Client(
	playerCharacter_t*	pc,
	const int			guildIdx, 
	const int			markChnageCnt, 
	unsigned char*		pMarkImage);





void GTH_SendMessage_Guild_ChatMessage_Request_To_ManagerServer( short guildIdx, char *name, char *chatMessage );
void GTH_SendMessage_Guild_ChatMessage_Reply_To_Client( playerCharacter_t *pc, char *message, char *chatMessage );


int		GTH_SendPCEventMessage_Teleport( playerCharacter_t *pc, vec3_t	movePos );
int		GTH_SendPCEventMessage_Before_Teleport( playerCharacter_t *pc );

void	GTH_ProcessEventMessage_Teleport();
void	GTH_ProcessEventMessage_Before_Teleport();

int		FindMonsterNearPlace(int localWorldIdx, vec3_t position );


void	GTH_SendMessage_ItemDropTest( playerCharacter_t* pc, int* resultItem, int* resultNumber );


void	GTH_ProcessMessage_Client_Extend();


void	GTH_ProcessMessage_Client_Extend_Second();
void	GTH_ProcessMessage_Server_Extend();


void GTH_SendMessage_ChattingMsg( playerCharacter_t *pc, int dlgType, int lpackType, int lpackIdx );


void GTH_SendMessage_Reply_Use_Masic_Book(playerCharacter_t *pc, int reply, int error, int invenPos);


int GTH_SendMessage_Reply_MasicBook_RepairItem( playerCharacter_t* pc, int reply, int error, int repairItemIdx[], int repairItemDur[], int numOfRepairItem ,int InvenPos );



void GTH_SendMessage_ReplayEpack_ChargSoul(playerCharacter_t *pc, int reply, int error, int invenpos,int EpkRemainCount=-1);


void GTH_SendMessage_Reply_RepairItemConfirm(playerCharacter_t* pc, int reply, int erroridx, enumRepairType repairtype, int cost, int repairCount, int itemtableidx);


struct tagPcSummonTypedef{
public:
	enum enumPcSummonType
	{
		PC_SUMMPN_CHOICE=0,
		PC_SUMMON_FORCED,	
		PC_SUMMON_ITEM,
		PC_SUMMON_MOVE_ITEM,
	};
};

void GTH_PC_Summon(playerCharacter_t* pFromPlayer,char* toName,int forcedYn);
void GTH_PC_Summon(int worldIdx, vec3_t position,  char* fromName, char* toName,int forcedYn);




void GTH_SendMessage_ChaoPointEvent(playerCharacter_t* pPc, int DecPoint);

void GTH_ProcessMessage_PING(); 
