#ifndef _TABLE_STRUCT_TOTAL_DEFINE_DATA_
#define _TABLE_STRUCT_TOTAL_DEFINE_DATA_










typedef enum
{
	GEN_MENTALITY		=	0,
	GEN_WIND,
	GEN_WATER,
	GEN_EARTH,
	GEN_POISON,
	GEN_IRON,
	GEN_FIRE,
	GEN_BODY,

	NUM_TABLE_GEN_TYPE

} TABLE_GEN_TYPE_t;

typedef enum
{
	ITEM_REQ_TYPE_ALL				=	0,
	ITEM_REQ_TYPE_POSITIVE,
	ITEM_REQ_TYPE_NEGATIVE,
	ITEM_REQ_TYPE_TAE,
	ITEM_REQ_TYPE_JA,
	ITEM_REQ_TYPE_SHIN,
	ITEM_REQ_TYPE_BO,
	ITEM_REQ_TYPE_MENTALITY,
	ITEM_REQ_TYPE_WIND,
	ITEM_REQ_TYPE_WATER,
	ITEM_REQ_TYPE_EARTH,
	ITEM_REQ_TYPE_POISON,
	ITEM_REQ_TYPE_IRON,
	ITEM_REQ_TYPE_FIRE,
	ITEM_REQ_TYPE_BODY,

	NUM_TABLE_ITEM_REQ_TYPE,

} TABLE_ITEM_REQ_TYPE_t;

typedef enum
{
	ETCITEM_TYPE_NONE,
	ETCITEM_TYPE_SKILLSPHERE,
	ETCITEM_TYPE_GENSPHERE,
	ETCITEM_TYPE_PORTAL,
	
	ETCITEM_TYPE_PRECOCITY,
	ETCITEM_TYPE_OPTIONSPHERE,
	ETCITEM_TYPE_MONSTERSPHERE,
	
	ETCITEM_TYPE_AUTOQUESTITEM,

	
	ETCITEM_TYPE_WORLD_PORTAL,		



	NUM_ETCITEM_TYPE
} TABLE_ETCITEM_TYPE_t;

typedef enum
{
	DAMAGE_NORMAL_TYPE		=	0,
	DAMAGE_POSITIVE_TYPE,
	DAMAGE_NEGATIVE_TYPE,
	DAMAGE_MEN_TYPE,
	DAMAGE_WIN_TYPE,
	DAMAGE_WAT_TYPE,
	DAMAGE_EAR_TYPE,
	DAMAGE_POI_TYPE,
	DAMAGE_IRO_TYPE,
	DAMAGE_FIR_TYPE,
	DAMAGE_BOD_TYPE,
	DAMAGE_NONE_TYPE,

	NUM_TABLE_DAMAGE_TYPE,

} TABLE_ATTACK_DAMAGE_TYPE_t;


typedef enum
{
	ITEM_WEAR_POS_WEAPON		=	0,
	ITEM_WEAR_POS_UPPER,
	ITEM_WEAR_POS_LOWER,
	ITEM_WEAR_POS_AIR_PROTECTOR,
	ITEM_WEAR_POS_BODY_PROTECTOR,
	ITEM_WEAR_POS_GLOVES,
	ITEM_WEAR_POS_BOOTS,
	ITEM_WEAR_POS_RING,
	ITEM_WEAR_POS_NECKLACE,
	ITEM_WEAR_POS_BRACELET,
	ITEM_WEAR_POS_EARRING,
	ITEM_WEAR_POS_TRANSFER,
	ITEM_WEAR_POS_EPACK,
	ITEM_WEAR_POS_NONE,

	NUM_TABLE_ITEM_WEARPOSITION,

} TABLE_ITEM_WEARPOSITION_t;


typedef enum
{
	SKILL_BASIC_TYPE		=	0,
	SKILL_ATTRIBUTE_TYPE,
	SKILL_LEGENDATY_TYPE,

} TABLE_SKILL_TYPE_t;


typedef enum
{
	SKILL_TARGET_NONE			=	0,
	SKILL_TARGET_MYSELF,
	SKILL_TARGET_OTHER_PC,
	SKILL_TARGET_PLAYER,
	SKILL_TARGET_NPC,
	SKILL_TARGET_MONSTER,
	SKILL_TARGET_AREA,
	SKILL_TARGET_CORPSE,
	SKILL_TARGET_ITEM,

	NUM_TABLE_SKILL_TARGET_TYPE,

} TABLE_SKILL_TARGET_TYPE_t;

typedef enum
{
	SKILL_REQITEM_NONE			=	-1,
	SKILL_REQITEM_CLASS			=	0,
	SKILL_REQITEM_ITEM,
	SKILL_REQITEM_RANGE,

} TABLE_SKILL_REQ_ITEM_TYEP_t;

typedef enum
{
	SKILL_POSITIVE_SPHERE_0		=	69,
	SKILL_POSITIVE_SPHERE_1		=	134,
	SKILL_POSITIVE_SPHERE_2		=	135,
	SKILL_POSITIVE_SPHERE_3		=	136,

} TABLE_SKILL_SPHERE_TYPE_t;

typedef enum
{
	WEAPON_TYPE_DANGGER			=	0,
	WEAPON_TYPE_ONEHAND_SWORD,
	WEAPON_TYPE_TWOHAND_SWORD,
	WEAPON_TYPE_MAGIC_BOW,
	WEAPON_TYPE_STAFF,
	WEAPON_TYPE_SPIRITOR,
	WEAPON_TYPE_CLAW,

} TABLE_ITEM_WEAPON_TYPE_t;

typedef enum
{
	
	AT_MENTALITY			=	1	<<	0,
	AT_WIND					=	1	<<	1,
	AT_WATER				=	1	<<	2,
	AT_EARTH				=	1	<<	3,

} BEFORE_AWAKENING_TYPE_t;

typedef enum
{

	AT_DEVOTIONAL_KNIGHT	=	1	<<	0,
	AT_ROMING_KNIGHT		=	1	<<	1,
	AT_PIECING_EYES			=	1	<<	2,
	AT_WINDY_ROMER			=	1	<<	3,
	AT_DESTROYER			=	1	<<	4,
	AT_GUARDIAN				=	1	<<	5,
	AT_PROPHECIER			=	1	<<	6,
	AT_TRANSFORMER			=	1	<<	7,
	AT_DEFENDER				=	1	<<	8,
	AT_STRANGER				=	1	<<	9,

} FIRST_AWAKENING_TYPE_t;


typedef enum
{
	J_MENTALITY		=	0,
	J_WIND,
	J_WATER,
	J_EARTH,
	J_DEVOTIONAL_KNIGHT,
	J_ROMING_KNIGHT,
	J_PIECING_EYES,
	J_WINDY_ROMER,
	J_DESTROYER,
	J_GUARDIAN,
	J_PROPHECIER,
	J_TRANSFORMER,
	J_DEFENDER,
	J_STRANGER,
	
	NUM_JOB_TYPE,
} JOB_TYPE_t;

#define J_NONE  NUM_JOB_TYPE-1		


typedef enum
{
	OPTION_AVAILABLE_ON_WEAPONS			=	1	<<	0,
	OPTION_AVAILABLE_ON_DAGGER			=	1	<<	1,
	OPTION_AVAILABLE_ON_1HSWORD			=	1	<<	2,
	OPTION_AVAILABLE_ON_2HSWORD			=	1	<<	3,
	OPTION_AVAILABLE_ON_BOW				=	1	<<	4,
	OPTION_AVAILABLE_ON_STAFF			=	1	<<	5,
	OPTION_AVAILABLE_ON_SPIRITOR		=	1	<<	6,
	OPTION_AVAILABLE_ON_CLAW			=	1	<<	7,
	OPTION_AVAILABLE_ON_ARMORS			=	1	<<	8,
	OPTION_AVAILABLE_ON_UPPER			=	1	<<	9,
	OPTION_AVAILABLE_ON_LOWER			=	1	<<	10,
	OPTION_AVAILABLE_ON_AIRPROTECTOR	=	1	<<	11,
	OPTION_AVAILABLE_ON_BODYPROTECTOR	=	1	<<	12,
	OPTION_AVAILABLE_ON_GLOVES			=	1	<<	13,
	OPTION_AVAILABLE_ON_BOOTS			=	1	<<	14,
	OPTION_AVAILABLE_ON_RING			=	1	<<	15,
	OPTION_AVAILABLE_ON_NECKLACE		=	1	<<	16,
	OPTION_AVAILABLE_ON_BRACELET		=	1	<<	17,
	OPTION_AVAILABLE_ON_EARRING			=	1	<<	18,
	OPTION_AVAILABLE_ON_EPACK			=	1	<<	19,

	MAX_COUNT_OPTION_AVAILABLE_TYPE		=	20

} OPTION_AVAILABLE_ON_TYPE_t;


typedef enum
{
	OPTION_ITEMTYPE_WEAPONS				=	0,
	OPTION_ITEMTYPE_DAGGER,
	OPTION_ITEMTYPE_1HSWORD,
	OPTION_ITEMTYPE_2HSWORD,
	OPTION_ITEMTYPE_BOW,
	OPTION_ITEMTYPE_STAFF,
	OPTION_ITEMTYPE_SPIRITOR,
	OPTION_ITEMTYPE_CLAW,
	OPTION_ITEMTYPE_ARMORS,
	OPTION_ITEMTYPE_UPPER,
	OPTION_ITEMTYPE_LOWER,
	OPTION_ITEMTYPE_AIRPROTECTOR,
	OPTION_ITEMTYPE_BODYPROTECTOR,
	OPTION_ITEMTYPE_GLOVES,
	OPTION_ITEMTYPE_BOOTS,
	OPTION_ITEMTYPE_RING,
	OPTION_ITEMTYPE_NECKLACE,
	OPTION_ITEMTYPE_BRACELET,
	OPTION_ITEMTYPE_EARRING,
	OPTION_ITEMTYPE_EPACK,

	MAX_COUNT_OPTION_ITEMTYPE		=	20
} OPTION_ITEMTYPE_t;



#define	MAX_NUMBER_OF_ITEM_OPTION_TABLE					400
#define	MAX_NUMBER_OF_OPTION_VALUE_GRADE				6




#define	MAX_NUMBER_OF_ITEM_TABLE						1000
#define	ITEM_TABLE_UNRESERVED_TOTAL_SIZE				128
#define	ITEM_TABLE_UNRESERVED_USED_SIZE					0

#define	MAX_NUMBER_OF_ITEM_CLASS_TABLE					100
#define	MAX_NUMBER_OF_ITEM_IN_CLASS						100
#define	MAX_NUMBER_OF_OPTION_IN_CLASS					20

#define	ITEM_CLASS_TABLE_UNRESERVED_TOTAL_SIZE			128
#define	ITEM_CLASS_TABLE_UNRESERVED_USED_SIZE			0

#define	MAX_NUMBER_OF_ITEM_GENERATE_TABLE				1000
#define ITEM_CLASS_GENERATE_UNRESERVED_TOTAL_SIZE		128
#define ITEM_CLASS_GENERATE_UNRESERVED_USED_SIZE		0
#define MAX_NUMBER_OF_ITEM_OPTION						6

#define	MAX_NUMBER_OF_ITEM_PART							100

#define MAX_NUMBER_OF_LEVEL								98


#define MAX_NUMBER_OF_QUESTSCROLL_TABLE					500


#define MAX_NUMBER_OF_AUTOQUESTSCROLL_TABLE				100


#define MAX_NUMBER_OF_SUMMON_MONSTER_TABLE				50


#define MAX_EVENT_ITEM_TABLE							100


#define MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE				7


enum
{
	CRAFT_TYPE_NORMAL=0,
	CRAFT_TYPE_RESULT,
	CRAFT_TYPE_EVENT,
};






#define	MAX_SKILL_NUMBER								500
#define	NUM_OF_SKILL_VALUE								10
#define	NUM_OF_REQUIRE_SKILL							5
#define	MAX_COUNT_EFFECT_LEVEL							5



#define MAX_NUMBER_OF_MONSTER_TABLE						500
#define MAX_NUMBER_OF_MONSTER_ATTACK					5
#define MAX_NUMBER_OF_MONSTER_SKILL						5
#define MAX_NUMBER_OF_MONSTER_UNRESERVED_TOTAL_SIZE		128
#define MAX_NUMBER_OF_MONSTER_UNRESERVED_USED_SIZE		0

#define	MAX_NUMBER_OF_ITEM_LOOT							10
#define MAX_NUMBER_OF_LOOT_ITEM_UNRESERVED_TOTAL_SIZE	128
#define MAX_NUMBER_OF_LOOT_ITEM_UNRESERVED_USED_SIZE	0




#define ITEM_TABLE_HEADER								"S-Cube Item Basic Table BN"
#define ITEM_TABLE_VERSION								109

#define ITEM_CLASS_TABLE_HEADER							"S-Cube Item Class Table BN"
#define ITEM_CLASS_TABLE_VERSION_103					103
#define ITEM_CLASS_TABLE_VERSION						102

#define ITEM_OPTION_TABLE_HEADER						"S-Cube Item Option Table BN"
#define ITEM_OPTION_TABLE_VERSION						103


#define ITEM_GENERATE_TABLE_HEADER						"S-Cube Item Generate Table BN"
#define ITEM_GENERATE_TABLE_VERSION						101

#define MONSTER_TABLE_HEADER							"S-Cube Monster Table BN"
#define MONSTER_TABLE_VERSION							104

#define SKILL_TABLE_HEADER								"S-Cube Skill Table BN"



#define SKILL_TABLE_VERSION_101							101
#define SKILL_TABLE_VERSION_102							102
#define SKILL_TABLE_VERSION_103							103
#define SKILL_TABLE_VERSION_104							104


#define SKILL_TABLE_VERSION_105							105




#define QIG_TABLE_HEADER								"S-Cube QIG Table BN"
#define QIG_TABLE_VERSION								100

#define TABLE_HEADER_UNRESERVED_TOTAL_SIZE				512
#define TABLE_HEADER_UNRESERVED_USED_SIZE				56
#define	OPTABLE_FILE_VERSION							100



#define		MAX_WORLD_NUMBER			50

typedef struct
{
	
	char	id[64];					
	int		version;				
	char	password[64];			
	int		recordNumber;			
	

	
	char	unreserved[TABLE_HEADER_UNRESERVED_TOTAL_SIZE - TABLE_HEADER_UNRESERVED_USED_SIZE];

} tableHeader_t;

#endif

#include "TableStruct.h"
#include "TableOldStruct.h"


int	LoadItemClassTable( char* filename, itemClassTable_t itemClassTable[], int &itemClassTableNumber );
int SaveItemClassTable( char* filename, itemClassTable_t itemClassTable[], int itemClassTableNumber );
int LoadItemTable( char* filename, itemTable_t itemTable[], int &itemTableNumber );
int SaveItemTable( char* filename, itemTable_t itemTable[], int itemTableNumber );
int SaveItemOptionTable( char* filename, itemOptionTable_t itemOptionTable[], int itemOptionTableNumber );
int LoadItemGenerateTable( char* filename, itemGenerateTable_t itemGenerateTable[], int &itemGenerateTableNumber );
int SaveItemGenerateTable( char* filename, itemGenerateTable_t itemGenerateTable[], int itemGenerateTableNumber );

int	LoadQuestScrollTable( char *filename, questscrollTable_t qsTable[], int &qsTableNumber );


int LoadMonsterTable( char* filename, monsterBasicTable_t monsterTable[], int &monsterTableNumber );
int SaveMonsterTable( char* filename, monsterBasicTable_t monsterTable[], int monsterTableNumber );


int LoadTokenBuffer( char* filename );
int DeleteTokenBuffer();
int SetTokenBuffer( byte *buffer , int size );
char* NextToken();
char* NextArg();
void NextCVSToken( char token[], int &endFile );


void InitSkillTable( skillTable_t skill[] );
void DeleteSkillTable( skillTable_t *skill_one );
int LoadSkillTable( char *filename, skillTable_t skill[], int &skillNumber );
int SaveSkillTable( char *filename, skillTable_t skill[], int skillNumber );
int LoadSkillTable( BYTE *buffer, skillTable_t skill[], int &skillNumber );


void InitQIGTable( questItemGenerate_t qigTable[] );
int SaveQIGTable( char *filename, questItemGenerate_t qigTable[] );
int LoadQIGTable( char* filename, questItemGenerate_t qigTable[] );
