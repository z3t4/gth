

#define MAX_NUMBER_OF_STORE_DEFAULT_ITEM	36
#define	MAX_NUMBER_OF_STORE_GENERATE_ITEM	36
#define MAX_NUMBER_OF_STORE_SKILL			36

#define MAX_NUMBER_OF_STORE_ITEM_PAGE		3
#define	MAX_NUMBER_OF_STORE_ITEM			( 36 * 3 )
#define MAX_NUMBER_OF_STORE_SKILL			36

#define STORE_SKILL_MASK					10000
#define STORE_ITEM_INDEX_EPACK_RA			20000
#define STORE_ITEM_INDEX_EPACK_SA			20001

enum
{
	ENTITY_MENU_SAVE	=	1 << 0,
	ENTITY_MENU_CHAT	=	1 << 1,
	ENTITY_MENU_TRADE	=	1 << 2,
	ENTITY_MENU_EPACK	=	1 << 3,
	ENTITY_MENU_EXIT	=	1 << 4
};

typedef struct
{
	int			idx;
	char		filename[64];
} scriptInfo_t;

#define	MAX_SCRIPTINFO		300
typedef struct
{
	scriptInfo_t	aiScript[MAX_SCRIPTINFO];
	scriptInfo_t	npcScript[MAX_SCRIPTINFO];
	
	int				numAiScript;
	int				numNpcScript;
	
} scriptInfoStruct_t;

typedef struct
{
	int			generateIdxNumber;
	int			generateIdx[MAX_NUMBER_OF_STORE_GENERATE_ITEM];
	int			generateNumber[MAX_NUMBER_OF_STORE_GENERATE_ITEM];
	int			generateLevel[MAX_NUMBER_OF_STORE_GENERATE_ITEM];
} storeItemGenerateStruct_t;

typedef struct
{
	int			itemIndex[MAX_NUMBER_OF_STORE_DEFAULT_ITEM];
	int			itemNumber[MAX_NUMBER_OF_STORE_DEFAULT_ITEM];
	int			defaultNumber;
} storeDefaultItemStruct_t;

typedef struct
{
	int			skillIndex[MAX_NUMBER_OF_STORE_DEFAULT_ITEM];
	int			skillIdxNumber;
} storeSkillStruct_t;

typedef struct
{
	
	
	item_t		item;					
	int			maxNumber;				
	int			curNumber;
	int			price;					
	int			page;
} storeItem_t;

typedef struct
{
	int			active;					

	int			closeTime;				
	int			readyTime;				
	int			openTime;				

	
	storeDefaultItemStruct_t		defaultItem[MAX_NUMBER_OF_STORE_ITEM_PAGE];
	
	
	storeItemGenerateStruct_t		generateItem[MAX_NUMBER_OF_STORE_ITEM_PAGE];

	
	storeSkillStruct_t				skill[MAX_NUMBER_OF_STORE_ITEM_PAGE];

	
	int			itemNumber;
	storeItem_t	item[MAX_NUMBER_OF_STORE_ITEM];

	
	int			costDay;				
	int			refineTax;				
	float		chargeRACost;			
	float		chargeSACost;			

} storeStruct_t;

typedef struct
{
	int			idx;
	int			worldIdx;
	vec3_t		pos;				
	float		range;				
	int			cost;				
} bindingStruct_t;

#define MAX_NUMBER_OF_NPC_NOTICE_PC			300

typedef struct
{
	int			active;

	int			noticePcNumber;
	int			maxNoticePcNumber;
	int			noticePcIdx[MAX_NUMBER_OF_NPC_NOTICE_PC];

	int			scriptFlag;				
	


} npcStruct_t;
