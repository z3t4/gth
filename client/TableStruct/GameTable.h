#ifndef _GAME_TABLE_HEADER_
#define _GAME_TABLE_HEADER_

#define CSV_ReadField	0
#define CSV_EndOfFile	1
#define CSV_EndOfLine	2



















enum
{
	ITEM_TYPE_ATTACK		= 1 << 0,
	ITEM_TYPE_DEFENSE		= 1 << 1,
	ITEM_TYPE_EPACK			= 1 << 2,
	ITEM_TYPE_UPGRADE		= 1 << 3,
	ITEM_TYPE_ACCESSORY		= 1 << 4
};

#define	BONUS_STATUS_POINT_LEVEL_UP		5

typedef struct
{
	
	__int64	totalexp;
	__int64	exp;
	
	__int64 totalGenExp;
	__int64 genExp;
	
} expTable_t;

typedef char npcMessage[2048];

typedef struct
{
	npcMessage	msg;

} npcChatTable_t;

typedef struct
{
	int		genCapability[10];
	int		limitGenLevel[10];

} skillGradeClass_t;

typedef	struct
{
	int		genCapability[100];
} genCapabilityTable_t;







#define MAX_NUMBER_OF_ITEM_CRAFT_TABLE			500

#define MAX_COUNT_ITEM_SYSTEM_SOURCE_ITEM		12
#define MAX_COUNT_ITEM_SYSTEM_REQUIRE_SKILL		4
#define MAX_COUNT_ITEM_SYSTEM_RESULT_ITEM		2

typedef struct
{
	int				itemIndex;
	int				itemCount;
	
	
	
	int				reqOptionCountType;
	int				reqOptionCount;
	
	int				numReqOptionIndex;
	int				reqOptionIndex[MAX_NUMBER_OF_ITEM_OPTION];
	
} itemSystemSourceItem_t;

typedef struct
{
	int				skillIndex;
	
	
	
	int				reqSkillLevelType;
	int				reqSkillLevel;
	
} itemSystemRequireSkill_t;

typedef struct
{
	int				itemIndex;
	int				itemCount;
	
	int				optionGroupIdx;
	int				optionGrade;
	int				rareItemGrade;





	
} itemCraftResultItem_t;





typedef struct
{
	int							idx;					
	int							craftGrade;				
	float						successProbability;
	int							craftCost;
	
	int							specialType;
	
	int							numSourceItem;
	itemSystemSourceItem_t		sourceItem[MAX_COUNT_ITEM_SYSTEM_SOURCE_ITEM];
	
	int							numRequireSkill;
	itemSystemRequireSkill_t	requireSkill[MAX_COUNT_ITEM_SYSTEM_REQUIRE_SKILL];
	
	int							numResultItem;
	itemCraftResultItem_t		resultItem[MAX_COUNT_ITEM_SYSTEM_RESULT_ITEM];
	
	
	int							m_nMembership_type;
	long						m_lResultNak;
	long						m_lResultExp;
	long						m_lEventIndex;
	int							m_nResultMark;
	BYTE						m_btEventType;
	char						m_chCheatName[20];
	

} itemCraftTable_t;

#define MAX_NUMBER_OF_NPC_CHAT_TABLE		256
#define MAX_NUMBER_OF_NPC_SELECT_TABLE		256
#define MAX_NUMBER_OF_HELP_TABLE			16

extern itemClassTable_t			g_itemClassTable[MAX_NUMBER_OF_ITEM_CLASS_TABLE];
extern int						g_nItemClassTable;

extern itemTable_t				g_itemTable[MAX_NUMBER_OF_ITEM_TABLE];
extern int						g_nItemTable;

extern optionTable_t			g_optionTable[MAX_COUNT_OPTION_TABLE];

extern itemGenerateTable_t		g_itemGenerateTable[MAX_NUMBER_OF_ITEM_GENERATE_TABLE];
extern int						g_nItemGenerateTable;

extern monsterBasicTable_t		g_monsterTable[MAX_NUMBER_OF_MONSTER_TABLE];
extern int						g_nMonsterTable;

extern int						g_skillNumber;
extern skillGradeClass_t		g_skillGradeClassTable[100];
extern genCapabilityTable_t		g_genCapabilityTable;


extern questscrollTable_t		g_questscrollTable[MAX_NUMBER_OF_QUESTSCROLL_TABLE];
extern int						g_questscrollTableNumber;


extern autoQuestScrollTable_t	g_autoquestscrollTable[MAX_NUMBER_OF_AUTOQUESTSCROLL_TABLE];
extern int						g_autoquestscrollTableNumber;



extern SummonMonster_t			g_summonMonstertable[MAX_NUMBER_OF_QUESTSCROLL_TABLE];
extern int						g_summonMonstertableNumber;


extern int						g_optionDescXSize;
extern int						g_optionDescYSize;

extern expTable_t				g_expTable[MAX_NUMBER_OF_LEVEL];

extern CLanguageManager			g_LPACK;								


extern actionTable_t			g_actionTable[MAX_NUMBER_OF_ACTION];


extern itemCraftTable_t		g_itemCraftTable[MAX_NUMBER_OF_ITEM_CRAFT_TABLE];
extern int						g_itemCraftTableNumber;




extern BattleRewordField_t		g_BattleRewordField;
extern BattleRewordItem_t		g_BattleRewordItem[5];



extern golryunManagerTable_t	g_golryunManagerTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
extern golryunProgressTable_t   g_golryunProgressTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];


int	LoadItemClassTable( char* filename );
int LoadItemTable( char* filename );
int LoadItemGenerateTable( char* filename );
int LoadSkillTable( char *filename );
void LoadActionTable( char *filename );


void GetActionTableData(actionTable_t *table,int id,char* token);


int LoadAllGameTable();
int LoadGenCapabilityTable( char *filename );

int LoadItemCraftTableForServer( char* filename );
int LoadQuestScrollTable( char* filename );

int LoadQuestScrollTable( char* filename );



int LoadOptionTable( char* filename );
int MakeOptionTable( char* optionTableFilename, char* valueTableFilename, optionTable_t optionTable[] );

void MakeScreenShot(IDirect3DDevice8* device, int screenx, int screeny);
int GetScreenShotName();
void TakeScreenShot(IDirect3DDevice8* device, char* file_name, int screenx, int screeny);


int LoadSummonMonsterTable( char *filename, SummonMonster_t summonTable[], int &summontablenumber);


int LoadGonryunBattlePracticeTableReword(char *filenameReword, BattleRewordField_t *pBattleRewordField);
int LoadGonryunBattlePracticeTableRewordItem(char *filenameRewordItem,BattleRewordItem_t g_BattleRewordItem[]);


#endif