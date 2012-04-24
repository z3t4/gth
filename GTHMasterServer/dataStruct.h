
#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_




#define MAX_AFFECT				5		
#define MAX_NUMBER_OF_SKILL		100		
#define	MAX_NUMBER_OF_OWNITEM	125		

#define MAX_INVENTORY_SIZE		48		
#define MAX_DEPOT_SIZE			42		
#define MAX_EQUIPMENT			14		
#define MAX_TRADE_SIZE			30		


#define MAX_NUMBER_OF_SKILL_VARIABLE	20




#define GEN_HALF_NUMBER		4

struct SKILL_GEN_LEVEL
{
	char		gen[GEN_HALF_NUMBER];
};


typedef struct
{
	
	int			idx;
	int			tableIdx;

	union
	{
		SKILL_GEN_LEVEL		level;
		DWORD				iLevel;
	};
	union
	{
		SKILL_GEN_LEVEL		calLevel;
		DWORD				iCalLevel;
	};

} skill_t;



#define	ITEM_NAMESTRING						32	

#define ITEM_OPTION_NUMBER					6

typedef struct
{

	enum
	{
		ITEM_T_CHARGE_VALUE =0,
		
		MAX_SIZE_OF_ITEM_SPAWN_DATA = 20,		
		
	};

	char			name[ITEM_NAMESTRING];						
	int				itemTableIdx;								
	int				optionNumber;								
	int				optionIdx[ITEM_OPTION_NUMBER];				
	int				optionValue[ITEM_OPTION_NUMBER];			
	int				durability;									
	int				reqLevel;									
	int				experience;									
	int				timer;										

	
	
	
	struct sItemExtendInfo
	{	
		
		enum enumItemUseFlag
		{
				ITEM_USE_FLAG_UN_CHECK		= 1 << 0,				
				ITEM_USE_FLAG_ALLOW_DESTROY	= 1 << 1,				
				ITEM_USE_FLAG_ALLOW_REPAIR	= 1 << 2,				
				ITEM_USE_FLAG_PRIMIUM		= 1 << 3,				
				ITEM_USE_FLAG_USE_TIME		= 1 << 4,				
				ITEM_USE_FLAG_USE_DATE		= 1 << 5,				
				
		};
		
		
		struct sItemSpawnInfoMation
		{
			enum enumItemSpawnType
			{					
					ITEM_SPAWN_TYPE_UNKNOW		= 0,				
					ITEM_SPAWN_TYPE_OLDVER		= 1,				
					ITEM_SPAWN_TYPE_STORE		= 2,				
					ITEM_SPAWN_TYPE_MONSTER		= 3,				
					ITEM_SPAWN_TYPE_GAMBLE		= 4,				
					ITEM_SPAWN_TYPE_PRECOCITY	= 5,				
					ITEM_SPAWN_TYPE_WEB_POST	= 6,				
					ITEM_SPAWN_TYPE_CRAFT		= 7,				
					ITEM_SPAWN_TYPE_QUEST		= 8,				
			};
			
			enum enumItemSpawnType	ItemSpawnType;
			char data[MAX_SIZE_OF_ITEM_SPAWN_DATA];		
			
		};
		
		__int64						Serial;							
		sItemSpawnInfoMation		ItemSpawnInfomation;			
		int							UseFlag;						
		
		
		
		
		
		union
		{
			int						AllowUseTime;
			int						AllowUseDate;
		};		
		
		int							CalclateUseDateToSec;
	};	
	sItemExtendInfo					ItemExtendInfo;			

} item_t;

typedef item_t::sItemExtendInfo::sItemSpawnInfoMation	ITEM_SPAWN_INFO;		
typedef item_t::sItemExtendInfo::enumItemUseFlag		ENUM_ITEM_USE_FLAG;



#define MAX_COUNT_QUEST_VARIABLE		200
struct QUESTVARIABLE
{
	char	isComplete;	
	char	step;	
	short	dummy;	
};

typedef struct
{
	union
	{
		DWORD				iVar;
		QUESTVARIABLE		var;
	};
} questVariable_t;


#define MAX_COUNT_QUEST_INVENTORY		24
struct QUESTINVENTORY
{
	short	itemIdx;		
	byte	maxItemCount;	
	byte	curItemCount;	
};

typedef struct
{
	union
	{
		DWORD				iItem;
		QUESTINVENTORY		item;
	};
		
} questInventory_t;


#define MAX_COUNT_QUEST_STATUS		5
#define MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD	10
typedef struct
{
	int	idx;		
	int	questIdx;		

	
	int	dField[MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD];

	char	partyOrganizerName[NAMESTRING];	
	int	partyOrganizeServer;	
	int	partyOrganizeTime;		

	int	clearTime;		
	int	limitTime;		

} questStatus_t;


typedef float   vec3_t[3];

#define GEN_NUMBER					8
#define DEFAULT_SKINPART_NUMBER		4


#define MAX_NUMBER_OF_HOTKEY	32

#define MASK_HOTKEY_ACTION		1000	
#define MASK_HOTKEY_ITEM		2000	


#define MAX_PRECOCITY_SIZE		6

typedef struct
{
	int					characterID;	

	char				name[NAMESTRING];		
	int					pcClass;		
	int					pcJob;			
	int					skinPart[DEFAULT_SKINPART_NUMBER];
	int					age;			

	char				rank[NAMESTRING];		
	int					rankPoint;				

	
	int					worldIdx;		
	vec3_t				position;		
	vec3_t				angles;			
	
	
	int					level;					
	__int64				exp;					
	int					increaseExpRate;		

	
	int					selectedGen;					
	int					genLevel;
	__int64				genExp;
	int					nextGenExp;
	int					increaseGenExpRate;
	int					genCapability[GEN_NUMBER];			
	int					genCrossStep;					

	
	int					force; 			
	int					soulpower;		
	int					agility; 		
	int					vitality;		

	int					bonusStatusPoint;	

	
	int					curRA;				
	int					curSA;				

	
	int					curChargeNSE;		
	int					curChargeSE;		
	int					curChargeRA;		
	int					curChargeSA;		
	int					depotSE;			

	
	int					curWeapon;
	
	int					bindingIdx;				

	
	int					skillNumber;				
	skill_t				skill[MAX_NUMBER_OF_SKILL];	
	int					bonusSkillPoint;
	int					skillVariable[MAX_NUMBER_OF_SKILL_VARIABLE];

	
	item_t				item[MAX_NUMBER_OF_OWNITEM];
	int					itemNumber;
	int					inventory[MAX_INVENTORY_SIZE];
	int					depot[MAX_DEPOT_SIZE];
	int					equipment[MAX_EQUIPMENT];
	int					mouseInventory;

	
	int					hotkeyType[MAX_NUMBER_OF_HOTKEY];
	int					hotkeyIdx[MAX_NUMBER_OF_HOTKEY];

	
	questVariable_t		questVar[MAX_COUNT_QUEST_VARIABLE];
	questInventory_t	questInventory[MAX_COUNT_QUEST_INVENTORY];
	questStatus_t		questStatus[MAX_COUNT_QUEST_STATUS];
	int					questNumber;		
	int					questPoint;

	
	int					precocityTime[MAX_PRECOCITY_SIZE];
	int					precocityInventory[MAX_PRECOCITY_SIZE];

	
	
	int					chaosPoint;
	
	
	int					guildIdx;
	unsigned int		guildJoinTime; 
	unsigned int		guildSecedeTime; 

} characterData_t;


#define NUM_TABLE_GEN_TYPE			8
#define DEFAULT_SKIN_PART_NUMBER	4
typedef struct
{
	int					characterID;
	int					pcClass;
	
	int					skinPart[DEFAULT_SKIN_PART_NUMBER];
	char				name[NAMESTRING];
	
	int					force; 			
	int					soulpower;		
	int					agility; 		
	int					vitality;		

	int					bonusPoint;

	item_t				defaultItem;
} charEditData_t;



typedef struct
{
	
	char				name[NAMESTRING];		
	int					pcClass;		
	int					pcJob;
	int					skinPart[DEFAULT_SKINPART_NUMBER];
	int					age;			

	char				rank[NAMESTRING];		
	int					rankPoint;				

	
	int					worldIdx;		
	vec3_t				position;		
	vec3_t				angles;			
	
	
	int					level;					
	__int64				exp;					
	int					increaseExpRate;		

	int					selectedGen;					
	int					genLevel;			
	__int64				genExp;				
	int					increaseGenExpRate;	
	int					genCapability[GEN_NUMBER];	

	
	int					force; 			
	int					soulpower;		
	int					agility; 		
	int					vitality;		
	int					bonusStatusPoint;	

	
	int					curRA;				
	int					curSA;				

	
	int					curChargeNSE;		
	int					curChargeSE;		
	int					curChargeRA;		
	int					curChargeSA;		

	
	int					bindingIdx;				

	
	int					skillNumber;				
	skill_t				skill[MAX_NUMBER_OF_SKILL];	
	int					bonusSkillPoint;

	
	item_t				item[MAX_NUMBER_OF_OWNITEM];
	int					itemNumber;
	int					inventory[MAX_INVENTORY_SIZE];
	int					depot[MAX_DEPOT_SIZE];
	int					equipment[MAX_EQUIPMENT];
	int					mouseInventory;

	
	int					hotkeyType[MAX_NUMBER_OF_HOTKEY];
	int					hotkeyIdx[MAX_NUMBER_OF_HOTKEY];
	
	
	
	
	int					TotalStatusPointForGenType;	
	int					MaxHairNumber;
	int					MaxFaceNumber;	
	
} characterTable_t;


#endif