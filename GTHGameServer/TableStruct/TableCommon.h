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


#define SAFE_PC_GEN_TYPE(p)  (p < 0 || p >= NUM_TABLE_GEN_TYPE ) ? 0 : 1


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
	
	
	MAX_COUNT_WEAPON_TYPE,

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
	
	OPTION_AVAILABLE_ON_ETC				=	1	<<	20,		
	MAX_COUNT_OPTION_AVAILABLE_TYPE		=	21				
	
	
	

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



#define	MAX_NUMBER_OF_OPTION_VALUE_GRADE				10



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

#define	MAX_NUMBER_OF_LEVEL								98


#define MAX_NUMBER_OF_QUESTSCROLL_TABLE					500

#define MAX_NUMBER_OF_AUTO_QUESTSCROLL_TABLE			100

#define MAX_NUMBER_OF_SUMMON_MONSTER_TABLE				50



#define MAX_NUMBER_OF_CONSIDERATION_TABLE				50


#define MAX_NUMBER_OF_GOLRYUN_MANAGER_TABLE				7
#define MAX_NUMBER_OF_GOLRYUN_ITEM_GROUP_TABLE			15




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








static char* g_genAttributeString[] = 
{
	"지혜(정신)",
	"도전(바람)",
	"조화(물)",
	"생명(흙)",
	"죽음(독)",
	"인내(쇠)",
	"창조(불)",
	"투지(신체)",
	NULL
};


static char* g_attackSpeedString[] =
{
	"UNSEEN",
	"Extremely Fast",
	"Very Fast",
	"Fast",
	"Slightly Fast",
	"Normal",
	"Slightly Slow",
	"Slow",
	"Very Slow",
	"Extremely Slow",
	"STOPPED",
	NULL
}; 


static char* g_velocityString[] =
{
	"매우빠름",
	"빠름",
	"보통",
	"느림",
	"매우느림",
	NULL
};


static char* g_stunDelayString[] =
{
	"매우빠름",
	"빠름",
	"보통",
	"느림",
	"매우느림",
	NULL
};


static char* g_characterAnimationString[] =
{
	"ANIMTYPE_IDLE",
	"ANIMTYPE_WALK",
	"ANIMTYPE_WALKBACK",
	"ANIMTYPE_JOG",
	"ANIMTYPE_RUN",
	"ANIMTYPE_BASIC_RESERVED_1",
	"ANIMTYPE_BASIC_RESERVED_2",

	"ANIMTYPE_DIE",
	"ANIMTYPE_DEAD",
	"ANIMTYPE_RESPAWN",
	
	"ANIMTYPE_ATTACK_1",
	"ANIMTYPE_ATTACK_2",
	"ANIMTYPE_ATTACK_3",
	"ANIMTYPE_ATTACK_4",

	"ANIMTYPE_DEFENSE_1",
	"ANIMTYPE_DEFENSE_2",
	"ANIMTYPE_DEFENSE_3",
	"ANIMTYPE_DEFENSE_4",
	
	"ANIMTYPE_MAGIC_1",
	"ANIMTYPE_MAGIC_2",
	"ANIMTYPE_MAGIC_3",
	"ANIMTYPE_MAGIC_4",
	"ANIMTYPE_MAGIC_5",
	"ANIMTYPE_MAGIC_6",
	"ANIMTYPE_MAGIC_7",
	"ANIMTYPE_MAGIC_8",

	"ANIMTYPE_SOCIAL_MOREIDLE_1",
	"ANIMTYPE_SOCIAL_MOREIDLE_2",
	"ANIMTYPE_SOCIAL_STAND",
	"ANIMTYPE_SOCIAL_SIT",
	"ANIMTYPE_SOCIAL_BOW",

	"ANIMTYPE_RESERVED_1",
	"ANIMTYPE_RESERVED_2",
	NULL
};


static char* g_characterAnimationSetString[] =
{
	"COMMON_SET",
	"SKILL_SET",
	"ITEM_SET_0",
	"ITEM_SET_1",
	"ITEM_SET_2",
	"ITEM_SET_3",
	"ITEM_SET_4",
	"ITEM_SET_5",
	"ITEM_SET_6",
	"ITEM_SET_7",
	"ITEM_SET_8",
	"ITEM_SET_9",
	"ITEM_SET_10",
	"ITEM_SET_11",
	"ITEM_SET_12",
	"ITEM_SET_13",
	"ITEM_SET_14",
	"ITEM_SET_15",
	"ITEM_SET_16",
	"ITEM_SET_17",
	"ITEM_SET_18",
	"ITEM_SET_19",
	NULL
};


static char* g_characterAnimationCommonSetIDString[] = 
{
	"COMMON_0",
	"COMMON_1",
	"COMMON_2",
	"COMMON_3",
	"COMMON_4",
	"COMMON_5",
	"COMMON_6",
	"COMMON_7",
	"COMMON_8",
	"COMMON_9",
	"COMMON_10",
	"COMMON_11",
	"COMMON_12",
	"COMMON_13",
	"COMMON_14",
	"COMMON_15",
	"COMMON_16",
	"COMMON_17",
	"COMMON_18",
	"COMMON_19",
	"COMMON_20",
	"COMMON_21",
	NULL
};


static char* g_characterAnimationSkillSetIDString[] = 
{
	"SKILL_0",
	"SKILL_1",
	"SKILL_2",
	"SKILL_3",
	"SKILL_4",
	"SKILL_5",
	"SKILL_6",
	"SKILL_7",
	"SKILL_8",
	"SKILL_9",
	"SKILL_10",
	"SKILL_11",
	"SKILL_12",
	"SKILL_13",
	"SKILL_14",
	"SKILL_15",
	"SKILL_16",
	"SKILL_17",
	"SKILL_18",
	"SKILL_19",
	"SKILL_20",
	NULL
};



static char* g_characterAnimationItemSetIDString[] = 
{
	"IDLE",
	"WALK",
	"WALK_BACK",
	"WALK_LEFT",
	"WALK_RIGHT",
	"RUN",
	"PRE_ATTACK",
	"ATTACK_0",
	"ATTACK_1",
	"ATTACK_2",
	"ATTACK_3",
	"ATTACK_4",
	"DEFENSE",
	"AVOID",
	"RECOVERY",
	"ATTACKED_0",
	"ATTACKED_1",
	"DEAD_0",
	"DEAD_1",
	"DYING_0",
	"DYING_1",
	"FLY",
	"SIT",
	"STAND",
	"LIE",
	"EVENT",
	"SOCIAL",
	"JUMP",
	"RESERVED_0",
	"RESERVED_1",
	"RESERVED_2",
	"RESERVED_3",
	"RESERVED_4",
	"RESERVED_5",
	"RESERVED_6",
	"RESERVED_7",
	"RESERVED_8",
	"RESERVED_9",
	"RESERVED_10",
	"RESERVED_11",
	NULL
};

static char* g_monsterAnimationSetIDString[] =
{	
	"IDLE",
	"WALK",
	"WALK_BACK",
	"WALK_LEFT",
	"WALK_RIGHT",
	"RUN",
	"PRE_ATTACK",
	"ATTACK_0",
	"ATTACK_1",
	"ATTACK_2",
	"ATTACK_3",
	"ATTACK_4",
	"DEFENSE",
	"AVOID",
	"RECOVERY",
	"ATTACKED_0",
	"ATTACKED_1",
	"DEAD_0",
	"DEAD_1",
	"DYING_0",
	"DYING_1",
	"FLY",
	"SIT",
	"STAND",
	"LIE",
	"EVENT",
	"SOCIAL",
	"JUMP",
	"RESERVED_0",
	"RESERVED_1",
	"RESERVED_2",
	"RESERVED_3",
	"RESERVED_4",
	"RESERVED_5",
	"RESERVED_6",
	"RESERVED_7",
	"RESERVED_8",
	"RESERVED_9",
	"RESERVED_10",
	"RESERVED_11",
	NULL
};

static char* g_characterAnimationGroupIDString[] = 
{
	"GROUP_NULL",
	"GROUP_MOVE",
	"GROUP_BASIC",
	"GROUP_ATTACK",
	"GROUP_DEFENSE",
	"GROUP_MAGIC",
	"GROUP_SOCIAL",
	NULL
};






static char* g_itemReqClassString[] = 
{
	"정신선인",
	"바람선인",
	"물 선인",
	"흙 선인",
	"헌신의 기사",
	"방랑의 기사",
	"혜안의 신비궁수",
	"방랑의 신비궁수",
	"파괴의 신비술사",
	"수호의 신비술사",
	"예언의 신비술사",
	"변신의 정령사",
	"방어의 정령사",
	"미지의 정령사",
	NULL,
};

static char* g_itemAttackPatternString[] =
{
	"일반공격",
	"Positive",
	"Negative",
	"정신",
	"바람",
	"물",
	"흙",
	"독",
	"쇠",
	"불",
	"신체",
	"무속성",
	NULL
};


static char* g_itemAttackHanded[] =
{
	"한손 무기",
	"양손 무기",
	"쌍 무기",
	NULL
};


static char* g_itemWearPositionString[] =
{
	"무기(EQUIP_WEAPON)",
	"상의 (EQUIP_UPPER)",
	"하의 (EQUIP_LOWER)",
	"에어프로텍터 (EQUIP_AIR_PROTECTOR)",
	"바디프로텍터 (EQUIP_BODY_PROTECTOR)",
	"장갑 (EQUIP_GLOVES)",
	"신발 (EQUIP_BOOTS)",
	"반지 (EQUIP_RING)",
	"목걸이 (EQUIP_NECKLACE)",
	"팔찌 (EQUIP_BRACELET)",
	"귀고리 (EQUIP_EARRING)",
	"이동수단 (EQUIP_TRANSFER)",
	"에너지팩 (EQUIP_EPACK)",
	"착용불가 (EQUIP_NONE)",
	NULL
};


static char* g_etcItemTypeString[] =
{
	"없음",
	"스킬스피어",
	"젠스피어",
	"스크롤",
	
	"숙성아이템",
	"옵션스피어",
	"몬스터스피어",
	NULL
};



static char* g_itemSkinningPositionString[] =
{
	"HAIR(머리카락)",
	"HEAD(머리)",
	"UPPER(상의)",
	"LOWER(하의)",
	"HAND(손)",
	"LEG(다리)",
	"FOOT(발)",
	NULL
};


static char* g_itemTagIDString[] =
{
	"OVERHEAD_FAR",			
	"OVERHEAD_NEAR",		
	"ARM_R",				
	"ARM_L",				
	"NECK",					
	"BODY",					
	"HAND_R",				
	"HAND_L",				
	"BACKHAND_R",			
	"BACKHAND_L",			
	"KNEE_R",				
	"KNEE_L",				
	"MIDDLEAIR_R",			
	"MIDDLEAIR_L",			
	"WAIST",				
	"SHOULDER",				
	"BACK",					
	"WAIST_R",				
	"WAIST_L",				
	"ELBOW_R",				
	"ELBOW_L",				
	"HAIR",					
	"HEAD",					
	"FACE",					
	"TRANS",				
	"GROUNDAIR_R",			
	"GROUNDAIR_L",			
	"GROUND",				
	"RESERVED",				
	NULL
};



static char* g_itemTagMoveString[] = 
{
	"NONE",
	"MOVE_1",
	"MOVE_2",
	"MOVE_3",
	"MOVE_4",
	NULL
};


static char* g_itemTagTypeString[] =
{
	"오버헤드",
	"방어구",
	"공중보패",
	"무기(손)",
	NULL
};


static char* g_itemRarityString[] = 
{
	"ESSENCE",
	"ADVANCED",
	"MYSTIC",
	"CLASSIC",
	"UNIQUE",
	NULL
};


static char* g_itemSizeString[] =
{
	"매우 큼",
	"큼",
	"보통",
	"작음",
	"매우 작음",
	NULL
};


static char* g_mdlItemClassType[] =
{
	"GTH_ITEM_CLASS_NONE",
	"GTH_ITEM_CLASS_WEAPON" ,
	"GTH_ITEM_CLASS_UPPER" ,
	"GTH_ITEM_CLASS_LOWER" ,
	"GTH_ITEM_CLASS_PROTECTOR" ,
	"GTH_ITEM_CLASS_HAND" ,
	"GTH_ITEM_CLASS_FOOT" ,
	"GTH_ITEM_CLASS_AIR_PROTECTOR" ,
	"GTH_ITEM_CLASS_ACCESSORY",
	"GTH_ITEM_CLASS_RESERVED",
	NULL
};


static char* g_itemIconClassType[] =
{
	"GTH_ITEM_CLASS_NONE",
	"GTH_ITEM_CLASS_WEAPON" ,
	"GTH_ITEM_CLASS_UPPER" ,
	"GTH_ITEM_CLASS_LOWER" ,
	"GTH_ITEM_CLASS_PROTECTOR" ,
	"GTH_ITEM_CLASS_HAND" ,
	"GTH_ITEM_CLASS_FOOT" ,
	"GTH_ITEM_CLASS_AIR_PROTECTOR" ,
	"GTH_ITEM_CLASS_ACCESSORY",
	"GTH_ITEM_CLASS_RESERVED",
	NULL
};



static char* g_itemWeaponType[] =
{
	"단검",
	"한손검",
	"양손검",
	"매직보우",
	"신비편",
	"스피리터",
	"클로우",
	NULL
};





static char* g_monsterTypeString[] = 
{
	"Type 1",
	"Type 2",
	"Type 3",
	"Type 4",
	"Type 5",
	NULL
};


static char* g_monsterSizeString[] =
{
	"Size 1",
	"Size 2",
	"Size 3",
	"Size 4",
	"Size 5",
	NULL
};

static char* g_OptionTypeString[] =
{
	"축복",
	"저주",	
	NULL
};





static char* g_skillTargetTypeString[] =
{
	"None",
	"Myself",
	"Other PC",
	"Player",
	"NPC",
	"Monster",
	"Area",
	"Item",

	NULL
};




#define ITEM_TABLE_HEADER								"S-Cube Item Basic Table BN"
#define ITEM_TABLE_VERSION								109

#define ITEM_CLASS_TABLE_HEADER							"S-Cube Item Class Table BN"
#define ITEM_CLASS_TABLE_VERSION_103					103
#define ITEM_CLASS_TABLE_VERSION_102					102

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
#define QIG_TABLE_VERSION								101

#define TABLE_HEADER_UNRESERVED_TOTAL_SIZE				512
#define TABLE_HEADER_UNRESERVED_USED_SIZE				56
#define	OPTABLE_FILE_VERSION							100

#define MAX_WORLDS										50	

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

class CGonryunPractice_RewordCtrl;

int	LoadItemClassTable( char* filename, itemClassTable_t itemClassTable[], int &itemClassTableNumber );

int LoadItemTable( char* filename, itemTable_t itemTable[], int &itemTableNumber );
int SaveItemTable( char* filename, itemTable_t itemTable[], int itemTableNumber );
int LoadItemOptionTable( char* filename, itemOptionTable_t itemOptionTable[], int &itemOptionTableNumber );
int SaveItemOptionTable( char* filename, itemOptionTable_t itemOptionTable[], int itemOptionTableNumber );
int LoadItemGenerateTable( char* filename, itemGenerateTable_t itemGenerateTable[], int &itemGenerateTableNumber );
int SaveItemGenerateTable( char* filename, itemGenerateTable_t itemGenerateTable[], int itemGenerateTableNumber );

int LoadQuestScrollTable( char *filename, questscrollTable_t qsTable[], int &qsTableNumber );


int LoadAutoQuestScrollTable( char *filename, autoQuestScrollTable_t autoqsTable[], int &autoTableNumber); 


int LoadGonryunBattlePracticeTableReword(char *filenameReword, CGonryunPractice_RewordCtrl* pPracticeRewordManager);
int LoadGonryunBattlePracticeTableRewordItem(char *filenameRewordItem, CGonryunPractice_RewordCtrl* pPracticeRewordManager);



int LoadGolryunManagerTable( char *filename); 
int LoadGolryunProgressTable( char *filename); 
int LoadGolryunRewardManagerTable( char *filename); 
int LoadGolryunItemGroupTable( char *filename);





int LoadSummonMonsterTable( char *filename, SummonMonster_t summonTable[], int &summontablenumber);


int	LoadConsiderationTable(char *filename, Consideration_t considerationTable[], int &considerationTableNumber);


BOOL LoadMonsterTable(const char* filename,monsterBasicTable_t monsterTable[],int &monsterTableNumber);

int SaveMonsterTable( char* filename, monsterBasicTable_t monsterTable[], int monsterTableNumber );


int LoadTokenBuffer( char* filename );
int DeleteTokenBuffer();
int SetTokenBuffer( byte *buffer , int size );
char* NextToken();
char* NextArg();
void NextCVSToken( char token[], int &endFile );


void NextCVSTokenReturn( char token[], int &endFile );



void InitSkillTable( skillTable_t skill[] );
void DeleteSkillTable( skillTable_t *skill_one );
int LoadSkillTable( char *filename, skillTable_t skill[], int &skillNumber );
int SaveSkillTable( char *filename, skillTable_t skill[], int skillNumber );
int LoadSkillTable( BYTE *buffer, skillTable_t skill[], int &skillNumber );


void InitQIGTable( questItemGenerate_t qigTable[] );
int SaveQIGTable( char *filename, questItemGenerate_t qigTable[] );
int LoadQIGTable( char* filename, questItemGenerate_t qigTable[] );


int LoadOptionGroupTable( char* filename, optionGroupTable_t groupTable[] );
int MakeOptionTable( char* optionTableFilename, char* valueTableFilename, optionTable_t optionTable[] );


