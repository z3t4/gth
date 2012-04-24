#include "../global_def.h"

itemClassTable_t		g_itemClassTable[MAX_NUMBER_OF_ITEM_CLASS_TABLE];
int						g_nItemClassTable;

itemTable_t				g_itemTable[MAX_NUMBER_OF_ITEM_TABLE];
int						g_nItemTable;



optionTable_t			g_optionTable[MAX_COUNT_OPTION_TABLE];

itemGenerateTable_t		g_itemGenerateTable[MAX_NUMBER_OF_ITEM_GENERATE_TABLE];
int						g_nItemGenerateTable;

monsterBasicTable_t		g_monsterTable[MAX_NUMBER_OF_MONSTER_TABLE];
int						g_nMonsterTable;

int						g_skillNumber;
skillGradeClass_t		g_skillGradeClassTable[100];
genCapabilityTable_t	g_genCapabilityTable;

int						g_optionDescXSize;
int						g_optionDescYSize;

expTable_t				g_expTable[MAX_NUMBER_OF_LEVEL];



actionTable_t			g_actionTable[MAX_NUMBER_OF_ACTION];


itemCraftTable_t		g_itemCraftTable[MAX_NUMBER_OF_ITEM_CRAFT_TABLE];
int						g_itemCraftTableNumber;



questscrollTable_t		g_questscrollTable[MAX_NUMBER_OF_QUESTSCROLL_TABLE];
int						g_questscrollTableNumber;


autoQuestScrollTable_t	g_autoquestscrollTable[MAX_NUMBER_OF_AUTOQUESTSCROLL_TABLE];
int						g_autoquestscrollTableNumber;



SummonMonster_t			g_summonMonstertable[MAX_NUMBER_OF_QUESTSCROLL_TABLE];
int						g_summonMonstertableNumber;

BattleRewordField_t		g_BattleRewordField;
BattleRewordItem_t		g_BattleRewordItem[5];



golryunManagerTable_t	g_golryunManagerTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
golryunProgressTable_t  g_golryunProgressTable[MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE];
