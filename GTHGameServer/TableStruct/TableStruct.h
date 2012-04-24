#ifndef _TABLE_TOTAL_STRUCTURE_
#define _TABLE_TOTAL_STRUCTURE_






typedef struct
{
	float	pro;												

	int		optionNumber;
	float	optionPro[MAX_NUMBER_OF_OPTION_IN_CLASS];
	int		optionIdx[MAX_NUMBER_OF_OPTION_IN_CLASS];			
	int		optionMinValue[MAX_NUMBER_OF_OPTION_IN_CLASS];		
	int		optionMaxValue[MAX_NUMBER_OF_OPTION_IN_CLASS];		

} itemOptionSlot_t;

struct ITEMCOMMONFIELD_SKILLSPHERE
{
	int				skillTableIndex;
};

struct ITEMCOMMONFIELD_SPHERE
{
	int				sphereGrade;
};

struct ITEMCOMMONFIELD_PORTAL
{
	int				bindingIdx;
};


struct ITEMCOMMONFIELD_PRECOCITY
{
	int				precocityTime;
	int				outComeItemIndex;
	int				craftAdditionalPro;
	int				upgradeAdditionalPro;
};

struct ITEMCOMMONFIELD_OPTIONSPHERE
{
	
	int				optionSphereType;
	int				optionSphereGrade;

};

struct ITEMCOMMONFIELD_MONSTERSPHERE
{
	int				monsterIndex;
	int				number;
};


struct ITEMCOMMONFIELD_WORLD_PORTAL
{
	BYTE			IsActiveWorldPortal[MAX_WORLDS];
};





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

} SERVER_JOB_TYPE_t;



typedef struct 
{

	
	enum
	{
		EPK_TYPE_NORMAL=0,
		EPK_TYPE_MUCH_CAPACITY,
	};

	
	enum
	{
		EPK_TYPE_UNCHARGE=0,
		EPK_TYPE_CHARGE,
	};

	
	enum
	{
		EPK_CHARGE_TYPE_NPC=0,
		EPK_CHARGE_TYPE_ITEM,
		EPK_CHARGE_TYPE_BOTH,
	};

	
	enum
	{
		EPK_RECYCLE_TYPE_ENABLE=0,
		EPK_RECYCLE_TYPE_DISABLE,
	};

}TypeItemTable_t;

typedef struct
{
public:
	enum{
		MAXBYTES_NAME=31,
		MAXBYTES_DOCUMENT=242,
	};

	enum
	{
		
		MAX_ITEM_TABLE_FLOAT_DATA=10,
		MAX_ITEM_TABLE_INT_UNRESERVED = 110,
	};

	
	int				idx;					

	int				NameIdx;
	int				DocumentIdx;

	char			name[MAXBYTES_NAME+1];
	char			document[MAXBYTES_DOCUMENT+1];
	
	int				level;					
	int				type;					
	int				classIdx;				
	int				rarity;					
	int				wearPosition;			
	
	int				size;					
	int				weight;					

	int				supplyRA;				
	int				supplySA;				

	

	DWORD			reqBeforeAT;
	DWORD			req1stAT;
	DWORD			req2ndAT;
	DWORD			req3rdAT;
	
	int				reqForce;				
	int				reqAgility;				
	int				reqSoulpower;			
	int				reqVitality;			
	int				reqNumber;				

	
	int				sellYn;					
	int				sellCost;				
	int				gambleCost;				

	int				durYn;					
	int				durability;				
	int				repairYn;				
	int				onceRepairDecDur;		

	
	int				skinningYn;								
	int				skinningPosition;						
	int				tagID;									
	int				skinModelID[NUM_TABLE_GEN_TYPE];		
	int				tagModelID[MAX_NUMBER_OF_ITEM_PART];	
	int				tagMoveID[MAX_NUMBER_OF_ITEM_PART];		
	int				mdlItemType;							
	int				mdlItemID;								
	int				iconItemID;								

	int				attackAni[NUM_TABLE_GEN_TYPE];			


	
	int				atkHanded;					
	int				atkEffect;					
	
	int				atkPhyMinDmg;				
	int				atkPhyMaxDmg;
	int				atkMagMinDmg;				
	int				atkMagMaxDmg;

	int				atkDelay;					
	int				atkHitPower;				

	int				atkLength;					
	int				atkRange;					




	float			atkCriticalRate;
	int				atkCriticalDmg;

	int				atkBowSACost;

	
	int				defPower;					
	int				defAvoidPower;				
	int				defPositiveRes;				
	int				defNegativeRes;				

	
	int				epkMaxChargeRA;			
	int				epkMaxChargeSA;			
	int				epkRecoveryRA;			
	int				epkRecoverySA;			

	
	BYTE			epkChargeMethod;		
	BYTE			epkChargeType;			
	BYTE			epkPackType;			
	int				epkChargeCount;			
	BYTE			epkRecycleType;			

	
	int				upgBasicEfficiency;		
	int				upgNumber;				
	int				upgReqExp[6];			

	
	int				etcItemType;

	
	union
	{
		int								commonField[16];
		ITEMCOMMONFIELD_SKILLSPHERE		skillSphere;
		ITEMCOMMONFIELD_SPHERE			sphere;
		ITEMCOMMONFIELD_PORTAL			portal;
		
		ITEMCOMMONFIELD_PRECOCITY		precocity;
		ITEMCOMMONFIELD_OPTIONSPHERE	optionSphere;
		ITEMCOMMONFIELD_MONSTERSPHERE	monsterSphere;
		ITEMCOMMONFIELD_WORLD_PORTAL	WorldPortal;
	};

	itemOptionSlot_t	slotOpt[MAX_NUMBER_OF_ITEM_OPTION];

	int				stackFlag;
	int				useFlag;
	

	
	
	int				skinModelID_job[NUM_JOB_TYPE];
	
	float			defVelocity;
	int				defHitPower;

	
	BYTE			AuthorityType;

	
	
	
	int				iUnReserved[MAX_ITEM_TABLE_INT_UNRESERVED];

	
	
	int				iFluxDataIndex[MAX_ITEM_TABLE_FLOAT_DATA];
	float			fFluxData[MAX_ITEM_TABLE_FLOAT_DATA];
} itemTable_t;



typedef struct 
{
	int			idx;
	char		name[128];
	char		printing[128];
	float		minValue;
	float		maxValue;
	int			group;
	int			grade;
	DWORD		availableOn;
	int			optionSphereType;
	int			dropOptionYn;
	int			unreserved[31];

} itemOptionTable_t;




#define MAX_COUNT_OPTION_TABLE			1000
#define MAX_COUNT_OPTION_GROUP_TABLE	1100
#define MAX_OPTION_GRADE				10




#define MAX_OPTIONSTONE_GRADE			5
#define MAX_OPTION_ITEM_TYPE			20


struct OPTIONVALUE
{
	char		valueName[64];
	float		value[MAX_OPTION_GRADE];
};

typedef struct 
{
	DWORD			availableOn;
	OPTIONVALUE		value_item[MAX_OPTION_ITEM_TYPE];
} optionValue_t;

typedef struct  
{
	
	int				idx;								
	char			name[64];							
	char			printing[64];						
	int				dropYN;								
	int				fixationPro;						
	int				optionType;							
	int				itemType[MAX_OPTION_ITEM_TYPE];		
	DWORD			availableOn;						
	optionValue_t	optValue[MAX_OPTIONSTONE_GRADE];	
} optionTable_t;


typedef struct
{
	int			idx;										
	char		name[64];									
	int			optionIdx[MAX_NUMBER_OF_ITEM_OPTION];		
	int			optionGrade[MAX_NUMBER_OF_ITEM_OPTION];		
} optionGroupTable_t;


typedef struct
{
	int		index;
	char	type;
	int		optionGrade;
	float	point;
	int		minValue;
	int		maxValue;

} monsterSpawnOptionTable_t;





typedef struct
{
	int					idx;										

	int					ClassNameIdx;

	int					wearPosition;								

	
	int					weaponType;									

	
	int					attackAtt;
	int					defenseAtt;
	int					packAtt;
	int					upgradeAtt;
	int					etcAtt;

	int					optionIdx[MAX_NUMBER_OF_OPTION_IN_CLASS];
	int					optionNumber;

	
	int					unreserved[ITEM_CLASS_TABLE_UNRESERVED_TOTAL_SIZE - ITEM_CLASS_TABLE_UNRESERVED_USED_SIZE];

} itemClassTable_t;





typedef struct
{
	int			idx;											

	char		name[20];										
	char		document[255];									

	
	
	
	int			classIdx;										
	

	int			itemIdx[MAX_NUMBER_OF_ITEM_IN_CLASS];			
	int			itemNumber;										
	float		itemPro[MAX_NUMBER_OF_ITEM_IN_CLASS];			

	
	
	int			optionPro[MAX_NUMBER_OF_ITEM_OPTION + 1];				
	int			optionNumber;										
	int			balanceOptionLevel[MAX_NUMBER_OF_OPTION_IN_CLASS];	
	

	
	int			graphType;
	float		graphXpos[MAX_NUMBER_OF_ITEM_IN_CLASS];			
	float		abc[3];

	
	int			unreserved[ITEM_CLASS_GENERATE_UNRESERVED_TOTAL_SIZE - ITEM_CLASS_GENERATE_UNRESERVED_USED_SIZE];

} itemGenerateTable_t;



typedef struct
{
	
	
	enum
	{
		ITEM_T_CHARGE_VALUE =0,
		
		MAX_SIZE_OF_ITEM_SPAWN_DATA = 20,		
		
	};

	
	
	char			name[32];									
	int				itemTableIdx;								
	int				optionNumber;								

	
	int				optionIdx[MAX_NUMBER_OF_ITEM_OPTION];		
	
	
	int				optionValue[MAX_NUMBER_OF_ITEM_OPTION];		

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







typedef struct
{
	int					maxLootNumber;			

	
	int					generateIdx[MAX_NUMBER_OF_ITEM_LOOT];
	int					generateIdxNumber;		

	
	float				generateProbaility[MAX_NUMBER_OF_ITEM_LOOT];
	
	
	float				lootingProbability;	

	
	int					unreserved[MAX_NUMBER_OF_LOOT_ITEM_UNRESERVED_TOTAL_SIZE - MAX_NUMBER_OF_LOOT_ITEM_UNRESERVED_USED_SIZE];

} lootItem_t;

typedef struct
{
public:
	enum{
		MAXBYTES_NAME=31,
		MAXBYTES_DOCUMENT=242,
	};

public:
	int					idx;					

	int					modelID;				
	int					aniSetType;
	
	int					tagModelID[MAX_NUMBER_OF_ITEM_PART];	
	int					tagMoveID[MAX_NUMBER_OF_ITEM_PART];	

	int					level;					
	
	char				name[MAXBYTES_NAME+1];
	
	char				document[MAXBYTES_DOCUMENT+1];
	
	float				phySize;				
	int					size;					
	int					type;					
	int					gen;					

	int					maxRA;					
	int					maxSA;					

	int					recoveryRARate;			
	int					recoverySARate;			

	int					attackAni;
	int					atkPhyMinDmg;
	int					atkPhyMaxDmg;
	int					atkMagMinDmg;
	int					atkMagMaxDmg;
	int					atkHitPower;
	int					atkDelay;
	int					atkLength;
	int					atkRange;

	float				atkCriticalPro;
	int					atkCriticalDmg;

	
	int					defPower;					
	int					defAvoidPower;				
	int					defPositiveRes;				
	int					defNegativeRes;				
	float				velocity;
	int					stunTime;

	int					skillIdx[MAX_NUMBER_OF_MONSTER_SKILL];		
	int					skillLevel[MAX_NUMBER_OF_MONSTER_SKILL];	
	int					skillAni[MAX_NUMBER_OF_MONSTER_SKILL];		
	int					skillNumber;

	
	int					exp;					
	int					genExp;					
	int					lootSE;					
	lootItem_t			lootItem;				

	int					pathFindingSize;		

	int					optionGrade;
	int					rareItemGrade;

	
	int					mapIconType;

	
	int					defDamageReduce;		
	int					atkDefAffectedDegree;	
	int					mapDensityWeight;		
	
	int					respawnTime;			
	int					AIScriptIdx;			
	int					npcAIScriptIdx;			

	
	float				physicalSize;			
	int					traceSize;				
	int					recognizeSize;			

	
	
	int					unreserved[116];

} monsterBasicTable_t;





typedef struct
{
	int				classType;
	int				effectID;
	int				effectLevel[MAX_COUNT_EFFECT_LEVEL];
	float			fraction;
	int				zpos;

	int				target;
	int				activity;	

	int				isSubEffect;
	int				subEffectID;

	int				notDefined[10];
} skillEffect_t;


typedef struct
{
	
	
	int				id;					
	char			name[64];			
	int				iconID;				
	int				skillClass;			
	char			desc[128];			
	int				usageType;			
	int				jobType;			
	int				autoObtainFlag;		
	
	
	
	skillEffect_t	effect;		
	int				animID;		
	
	
	
	int				reqGenLevel;	
	int				reqBaseLevel;	
	int				reqSkillID[NUM_OF_REQUIRE_SKILL];	

	int				reqItemType;	
	int				reqItem;		
	int				reqLength;
	int				reqPartyGen[8];	
	
	

	int				attackSkillFlag;
	int				attackAfterFlag;

	int				targetType;			
	
	char			valueName[NUM_OF_SKILL_VALUE][128];	
	float			valueInitial[NUM_OF_SKILL_VALUE];	
	float			valueLevelUp[NUM_OF_SKILL_VALUE];	
	int				valueMaxLevel[NUM_OF_SKILL_VALUE];
	int				valueGenGradeType[NUM_OF_SKILL_VALUE];
	int				valueGenGradeClass[NUM_OF_SKILL_VALUE];
	int				numValue;

	int				sphereCost;
	int				unreserved[127];

} skillTable_t;


typedef struct 
{
	enum SKILL_DAMAGE_TYPE
	{
		NONE_TYPE=0,
		SKILL_DMG_PHYDMG,
		SKILL_DMG_MYSTERY_POS_DMG,
		SKILL_DMG_MYSTERY_NEG_DMG,
		SKILL_DMG_DEFENCE,
	};
} Skill_Enum_Data_Type;

#define MAX_NUMBER_OF_QUEST_ITEM_GENERATE	8

typedef struct
{
	
	int				monsterTableIdx;
	int				questIdx;
	int				step;
	
	int				itemNumber;
	int				itemTableIdx[MAX_NUMBER_OF_QUEST_ITEM_GENERATE];
	float			generateProbaility[MAX_NUMBER_OF_QUEST_ITEM_GENERATE];
	int				maxItemCount[MAX_NUMBER_OF_QUEST_ITEM_GENERATE];

} oldQuestItemGenerate_t;

typedef struct
{
	int				monsterTableIdx;

	int				itemNumber;
	int				questIdx[MAX_NUMBER_OF_QUEST_ITEM_GENERATE];
	int				step[MAX_NUMBER_OF_QUEST_ITEM_GENERATE];
	int				itemTableIdx[MAX_NUMBER_OF_QUEST_ITEM_GENERATE];
	float			generateProbaility[MAX_NUMBER_OF_QUEST_ITEM_GENERATE];
	int				maxItemCount[MAX_NUMBER_OF_QUEST_ITEM_GENERATE];

} questItemGenerate_t;


struct REWARDFIELD
{
	int				exp;
	int				genExp;
	int				genCap;
	int				nak;
	int				itemIndex;
	int				itemNo;
	
	int				optionGroup;
};

struct QSDATAFIELD
{
	int				monsterIndex;
	int				monsterNo;
	int				scrollType;
	union
	{
		int			rewardField[7];
		REWARDFIELD	reward;
	};
	int				price;
	int				fame;
	int				level;
	
	int				nextScroll;

};

typedef struct 
{
	int				Index;
	char			name[32];
	char			description[512];		
	union
	{
		int			dataField[14];
		QSDATAFIELD	data;
	};
} questscrollTable_t;



struct SummonMonster_t
{
	int				idx;				
	char			name[32];			
	char			doc[512];			
	float			SummonTime;			
	UINT			SummonWorldIdx;		


};




struct GambleItem_t
{
	enum AVAILABLE 
	{		
			OPTION_AVAILABLE_ON_DAGGER			= 1,
			OPTION_AVAILABLE_ON_1HSWORD,			
			OPTION_AVAILABLE_ON_2HSWORD,			
			OPTION_AVAILABLE_ON_BOW,				
			OPTION_AVAILABLE_ON_STAFF,			
			OPTION_AVAILABLE_ON_SPIRITOR,		
			OPTION_AVAILABLE_ON_CLAW,			
			OPTION_AVAILABLE_ON_ARMORS,			
			OPTION_AVAILABLE_ON_UPPER,			
			OPTION_AVAILABLE_ON_LOWER,			
			OPTION_AVAILABLE_ON_AIRPROTECTOR,	
			OPTION_AVAILABLE_ON_BODYPROTECTOR,	
			OPTION_AVAILABLE_ON_GLOVES,			
			OPTION_AVAILABLE_ON_BOOTS,			
			OPTION_AVAILABLE_ON_RING,			
			OPTION_AVAILABLE_ON_NECKLACE,		
			OPTION_AVAILABLE_ON_BRACELET,		
			OPTION_AVAILABLE_ON_EARRING,			
			OPTION_AVAILABLE_ON_EPACK,	
			OPTION_AVAILABLE_ON_ETC,		
	};

	int type;
	std::vector<int>	vItem;
	std::vector<int>	vOption;
	std::vector<float>	vGambleRate;
	float	TotalGamebleRate;
	
	~GambleItem_t()
	{		
		vItem.clear();
		vOption.clear();
		vGambleRate.clear();
	};
};




struct AUTOREWARDFIELD
{
	int				exp;
	int				genExp;
	int				genCap;
	int				nak;
	int				skillIdx;
	int				itemIdx;
	int				itemNo;
	int				optionGroup;
	int				fame;
	int				nextScroll;
	int             chaoPoin;
	int				rewardchaoPoint;
	
	
	int				regClass;
	
	int				regItemIndex0;
	int				regItemNo0;
	int				regItemOption0;
	
	int				regItemIndex1;
	int				regItemNo1;
	int				regItemOption1;

	int				regItemIndex2;
	int				regItemNo2;
	int				regItemOption2;

	int				regItemIndex3;
	int				regItemNo3;
	int				regItemOption3;
	int				questexcute;
	int				questitemindex;
	
};

struct AUTOQSDATAFIELD
{
	int				monsterIdx;
	int				monsterNo;
	int				npcIdx;
	int				missionItemIdx;
	int				missionTime;
	int				type;
	int				level;
	int             startexp;
	int				startLpack;
	int				endLpack;
	union
	{
		
		int			rewardField[27];
		AUTOREWARDFIELD	reward;
	};

};

typedef struct 
{
	
	int				Index;
	char			name[32];
	union
	{
		
		int				dataField[35];
		AUTOQSDATAFIELD	data;
	};

} autoQuestScrollTable_t;





struct Consideration_t
{
	int				Idx;				
	char			Name[32];			
	int				Quantity;			
	int				Exp;				
	int				GenExp;				
	float			SelectRate;			
};



typedef struct 
{
	int			index;
	BOOL		bcontrol;
	BOOL		bgamble;
	int			levelMin;
	int			levelMax;
	int			joinNum;
	BOOL		bdeposit;
	int			deposit;
	int			progressIdx;

} golryunManagerTable_t;

typedef struct 
{
	int				index;
	int				rewardIdx;
	unsigned int	statusTime[7];
	
} golryunProgressTable_t;


typedef struct 
{
	int			chooseRate;
	int			expRate;
	int			rewardExp;
	int			rewardItem;
	int			provisionNum;
	bool		setOption;

} golryunRewardField;

typedef struct 
{
	int			id;
	int			groupIdx;

	golryunRewardField	rewardField[3];

} golryunItemGroupTable_t;

typedef struct 
{
	int			index;
	int			rewardType;
	BOOL		bsetWaiting;
	int			waitingExp;
	BOOL		bsetDefeat;
	int			defeatExp;
	int			setReward;
	int			rewardItemRate;
	int			rewardGroupIdx[3];

} golryunRewardManagerTable_t;


#endif