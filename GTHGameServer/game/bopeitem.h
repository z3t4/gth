




typedef enum
{
	ITEM_TABLE_IDX_GEN_SPHERE		=	135,
	ITEM_TABLE_IDX_ERIN_OF_TIME		=	209,
} SPECIAL_ITEM_TABLE_INDEX_t;

void MakeItemClassOptPro();

void InitItem(item_t *item);
int GetItemType( int itemTableIdx );

int CheckDuplicateOption( item_t *item, int optionIndex );
DWORD GetItemAvailalbeOn( int itemTableIdx );
int MakeItemOptionList( int itemTableIndex, int OptionGradeType, int optionList[] );

int FixationUniqueOption( item_t *item );
int FixationOption( item_t *item, int optionGradeType, int optionNumber );
int SpawnItemOptionSphere( item_t *item, int itemTableIdx, int specialType );
int SpawnItemAccessory( item_t *item, int optionGradeType );



int SpawnItem( item_t *item, int itemTableIdx, int optionNumber	= 0, int optionGradeType = -1, int spawnLevel = 0 , int bGamble = 0);



int SpawnItemByOptionGroup( item_t *item, int itemTableIdx, int optionGroupIdx );

int GTH_GenerateItemFromMonster( monsterCharacter_t *monster );
int GTH_GenerateQuestItemFromMonster( monsterCharacter_t *monster, int conIdx );


int GTH_SpawnNak( int nak, int worldIdx, vec3_t position, int lootType, int lootPCIdx );
int GTH_SpawnMultiItemObject( item_t item[], int worldIdx, vec3_t position[], int lootType, int lootPCIdx , int number, vec3_t origin );
int GTH_SpawnItemObject(item_t *item, int worldIdx, vec3_t position, int lootType, int lootPCIdx);
int GTH_RemoveItemObject(itemObject_t *itemObject);
void GTH_UpdateItemObject();

void GTH_DeleteMyItem( playerCharacter_t *pc, item_t *item );
int GTH_AddMyItem( playerCharacter_t *pc, item_t *item );
int GTH_StackMyItem( playerCharacter_t* pc, item_t *item );
int GTH_StackMyItemForDepot( playerCharacter_t* pc, item_t *item );

int GTH_AddQuestItem( playerCharacter_t *pc, int itemTableIdx, int questIdx, int itemNumber, int maxNumber );
int GTH_DeleteQuestItem( playerCharacter_t *pc, int itemTableIdx, int questIdx, int itemNumber );
int GTH_GetQuestItemCount( playerCharacter_t* pc, int itemTableIdx, int questIdx );

int GTH_CheckCraftFormula( playerCharacter_t *pc );

int GTH_CraftProcess( playerCharacter_t *pc, int &reply, int &error, int &reqNak, float &pro, int resultItemIdx[], int checkFlag  ,int &craftIdx );

int GTH_UpgradeProcess( playerCharacter_t *pc, int selectedOptionIdx, int &reply, int &error, int &reqNak, float &pro, float &vanishPro, float &downGradePro, int &currentGrade, int &nextGrade, int& resultItemIdx, int checkFlag );




int GTH_DeleteProcess( playerCharacter_t *pc, int &reply, int &error, int optionIndex, int checkFlag );


int GTH_QuestScroll_ObtainProcess( playerCharacter_t *pc, int &reply, int &error, int &reqNak, int &tableIdx, int depart, int level, int reward, int selectedQSTableIdx, int checkFlag );

int GTH_AutoQuestScroll_ObtainProcess( playerCharacter_t* pPlayer, const int& tableIdx);
int GTH_AutoCraftProcess( playerCharacter_t *pc, int idx,int &reply, int &error);


void	GenerateMonsterSpawnItem(item_t& item, int itemTableIdx, int monsterLevel);
int		DecideMonsterType(int monsterLevel, int & selectedMonsterLevel);
int		CalcMonsterLevel(int monsterLevel);
bool	IsBossMonster();
int		GetSpawnMapID(int monsterIdx);

int		FindMonster(int level);
int		FindBossMonster(int level);
int		FindNormalMonster(int level);


int		GetOptionNum(int monsterLevel, int limit);
void	CalcMonsterOptionNum(item_t& item);

void	ShareTotalPoint(item_t &item, int monsterOptNum, int playerOptNum, int totPoint);
bool	CheckOptionGrade(int MonGrade, int optionIdx);
bool	CheckSameOptionIdx(int index, int *temp, int tmepcount );



int		CheckMinMaxValue(int optionIdx, int& optionValue, int& resValue, int maxValue);
void	FindSpawnCardMinMaxValue(int optionIdx, int& minValue, int& maxValue);


int		GTH_SimulateGenerateItemFromMonster( monsterCharacter_t *monster, item_t* spawnItem, int& nSpawnItem );


int		Get_EmptyInventoryCount(playerCharacter_t *pc);



