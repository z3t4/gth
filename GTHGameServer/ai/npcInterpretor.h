int AI_NPC_InterpretorFunction( playerCharacter_t* pc );

int AI_NPC_InterpretorDefine( playerCharacter_t* pc, int paramAddr );
int AI_NPC_LoadScript( monsterCharacter_t* monster, char* filename );

char AI_NPC_GetCharData( playerCharacter_t* pc );
int AI__NPC_GetIntData( playerCharacter_t* pc );
int AI_NPC_Interpretor( playerCharacter_t* pc );
void AI_NPC_InterpretorCall( playerCharacter_t* pc );
void AI_NPC_InterpretorOperation( playerCharacter_t* pc );
void AI_NPC_InterpretorParameter( playerCharacter_t* pc, int param[], int n );
int AI_InterpretorCase( playerCharacter_t* pc );
void AI_NPC_InterpretorThen( playerCharacter_t* pc, int addrInfo[] );
void AI_NPC_InterpretorElse( playerCharacter_t* monster, int addrInfo[] );
void AI_NPC_InterpretorIF( playerCharacter_t* pc );





void NPC_InitAI( playerCharacter_t* pc, int npcIdx );
void NPC_SendEvent( playerCharacter_t *pc );




int AI_NPC_FindEmptyInventoryPos(void *pPc,int *pParam);
int AI_NPC_AddQuestItem(void *pPc,int *pParam);
int AI_SendDialogMessage(void *pPc,int *pParam);
int AI_OpenNpcInventory(void *pPc, int *pParam);
int AI_NPC_CheckAutoQuestType(void *pPc,int *pParam);
int AI_NPC_AutoSetQuestType(void *pPc, int *pParam );

int AI_NPC_IsMessageWating( void* pPc, int *pParam );							
int AI_NPC_SendMessage( void* pPc, int *pParam );
int AI_NPC_SendMessageButton( void* pPc, int *pParam );
int AI_NPC_GetMessageSelect( void* pPc, int *pParam );
int AI_NPC_CloseMessage( void* pPc, int *pParam );
int AI_NPC_OpenStore( void* pPc, int *pParam );
int AI_NPC_CloseStore(void *pPc, int *pParam );
int AI_NPC_CheckQuestStep(void *pPc, int *pParam );
int AI_NPC_SetQuestStep(void *pPc, int *pParam );
int AI_NPC_CheckQuestItem(void *pPc, int *pParam );
int AI_NPC_DeleteQuestItem(void *pPc, int *pParam );
int AI_NPC_SetQuestStatus(void *pPc, int *pParam );
int AI_NPC_ClearQuestStatus(void *pPc, int *pParam );
int AI_NPC_AddItem(void *pPc, int *pParam );
int AI_NPC_DeleteItem(void *pPc, int *pParam );
int AI_NPC_CompleteQuest(void *pPc, int *pParam );
int AI_NPC_OpenSkillLevelUp(void *pPc, int *pParam );
int AI_NPC_OpenBinding(void *pPc, int *pParam );
int AI_NPC_AddLoot(void *pPc, int *pParam );
int AI_SendChatMessage(void *pPc, int *pParam );
int AI_SetDynamicField(void *pPc, int *pParam );
int AI_GetDynamicField(void *pPc, int *pParam );
int AI_CheckPlayerClass(void *pPc, int *pParam );
int AI_CheckPlayerJob(void *pPc, int *pParam );
int AI_CheckPlayerLevel(void *pPc, int *pParam );
int AI_CheckPlayerGenLevel(void *pPc, int *pParam );
int AI_CheckPlayerStatus(void *pPc, int *pParam );
int AI_MoveToBindingPosition(void *pPc, int *pParam );
int AI_AwakenPlayerJob( void *pPc, int *pParam );
int AI_GetCompleteCount( void *pPc, int *pParam );
int AI_OpenDepot( void *pPc, int *pParam );
int AI_CloseDepot( void *pPc, int *pParam );
int AI_OpenPrecocity( void *pPc, int *pParam );
int AI_OpenCraft( void *pPc, int *pParam );


int AI_OpenEventCraft( void *pPc, int *pParam );

int AI_OpenUpgrade( void *pPc, int *pParam );

int AI_InitPlayerStatus( void *pPc, int *pParam );

int AI_OpenQuestScroll( void *pPc, int *pParam );

int NPC_AddNoticePC( monsterCharacter_t* monster, playerCharacter_t* pc );
int NPC_DeleteNoticePC( monsterCharacter_t* monster, playerCharacter_t* pc );
int NPC_CheckNoticePC( monsterCharacter_t* monster );


int AI_OpenGambleStore( void *pPc, int *pParam );


int AI_OpenRequestDanBattle( void *pPc, int *pParam );

int AI_GonryunBattle(void *pPc, int *pParam);



int AI_NPC_TransportGolryun( void *pPc, int *pParam );
int AI_NPC_TransportArayan( void *pPc, int *pParam );
int AI_NPC_SuggestGolryunBattle(void *pPc, int *pParam );
int AI_NPC_BuyGolryunBattleTicket(void *pPc, int *pParam );

int AI_NPC_DisplayTournament(void *pPc, int *pParam );
int AI_NPC_CheckPrize(void *pPc, int *pParam );
int AI_NPC_ObserveTournament(void *pPc, int *pParam );




int AI_OpenPackage(void *pPc, int *pParam );



int AI_helpOpen(void *pPc, int *pParam);


int AI_OpenGiftCardWin(void *pPc, int *pParam);


int AI_DanBattleEvent(void *pPc, int *pParam);
int AI_DanBattlePcPoint(void *pPc, int *pParam);