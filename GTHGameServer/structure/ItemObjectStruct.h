













typedef enum
{
	OBJECT_TYPE_ITEM		=	0,
	OBJECT_TYPE_PORTAL,
	
	
	OBJECT_TYPE_NAK,
	OBJECT_TYPE_ETC,
} ITEM_OBJECT_TYPE_t;












#define MAX_NUM_OF_PORTAL 2
struct ITEMOBJECT_PORTAL
{
	short			portalType;
	short			worldIdx[MAX_NUM_OF_PORTAL];
	vec3_t			position[MAX_NUM_OF_PORTAL];
	short			numPerson;
};


typedef struct
{
	int				idx;			
	int				active;			
	int 			worldIdx;		
	vec3_t 			position;		
	vec3_t			angles;			
	int 			zoneIdx;		
	entityType_t	entityType;		
	
	int				objectType;		

	union
	{
		item_t	 				item;			
		ITEMOBJECT_PORTAL		portal;
		
		
		int						nak;			
	};

	int 			lootType;		
	int 			lootPCIdx;		
	int 			lootTime;		

	
	int				organizeServer;
	int				organizeTime;
	char			organizerName[NAMESTRING];
	
	
	
	char			DanName[NAMESTRING];

	unsigned int	destroyTime;	
} itemObject_t;








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
	
	int							m_nResultNakMark;
	long						m_lResultNak;

	int							m_nResultExpMark;
	long						m_lResultExp;

	long						m_lEventIndex;
	BYTE						m_btEventType;
	char						m_chCheatName[20];
	
	
} itemCraftTable_t;

