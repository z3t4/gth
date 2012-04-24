#ifndef _CLIENT_COMMON_H_
#define _CLIENT_COMMON_H_


#define MAX_CHARACTERS			50
#define	MAX_GROUND_ITEMS		50




#define MAX_SAVED_DATA			6	




#define SERVERSTRING           20
#define DATABASESTRING         20
#define IPSTRING               16
#define PORTSTRING              8
#define VERSIONSTRING          20


#define IDSTRING               20
#define PASSWORDSTRING         20
#define NAMESTRING             20
#define WORLDNAMESTRING        64

#define CHATSTRING			  256


#define GUILDNAMESTRING						20
#define GUILDNOTICESTRING					100
#define MASTERNOTESTRING					20



typedef enum 
{
	GTH_EV_CHAR_IDLE,				
	GTH_EV_CHAR_WALK,				
	GTH_EV_CHAR_WALKLEFT,			
	GTH_EV_CHAR_WALKRIGHT,			
	GTH_EV_CHAR_BACK,				
	GTH_EV_CHAR_BACKLEFT,			
	GTH_EV_CHAR_BACKRIGHT,			
	GTH_EV_CHAR_RUN,				
	GTH_EV_CHAR_RUNLEFT,			
	GTH_EV_CHAR_RUNRIGHT,			
	GTH_EV_CHAR_TURNLEFT,			
	GTH_EV_CHAR_TURNRIGHT,			
	GTH_EV_CHAR_MOVELEFT,			
	GTH_EV_CHAR_MOVERIGHT,			
	GTH_EV_CHAR_ATTACK,				
	GTH_EV_CHAR_DIE,				
	GTH_EV_CHAR_STUN,				
	GTH_EV_CHAR_DAMAGE,				
	GTH_EV_CHAR_RESPAWN,			
	GTH_EV_CHAR_SKIP,				
	GTH_EV_CHAR_MOUSEWALK,			
	GTH_EV_CHAR_MOUSERUN,			
	GTH_EV_CHAR_SIT,				
	GTH_EV_CHAR_STAND,				
	GTH_EV_CHAR_SKILL,				
	GTH_EV_CHAR_SKILLCASTING,		
	GTH_EV_CHAR_PREATTACK,			
	
	GTH_EV_CHAR_BEFORE_TELEPORT,	
	GTH_EV_CHAR_TELEPORT,			
	
	GTH_EV_CHAR_FREEZE,				
	GTH_EV_CHAR_PUSHED,				
	GTH_EV_CHAR_HIDE,				

	
	GTH_EV_CHAR_SHOUT,					
	GTH_EV_CHAR_BOW,				

	
	GTH_EV_CHAR_PC_STUN,

} i3characterEvent_t;

typedef enum
{
	GTH_CEV_CHAR_NONE		=	-1,
	GTH_CEV_CHAR_IDLE		=	0,
	GTH_CEV_CHAR_ATTACK,
	GTH_CEV_CHAR_PICKUP_ITEM,
	GTH_CEV_CHAR_NPC_REQUEST,
	GTH_CEV_CHAR_PC_REQUEST,
	GTH_CEV_CHAR_SKILL,
	GTH_CEV_CHAR_FOLLOW,

} i3characterClickEvent_t;


typedef enum
{
	ENTITY_PC,
	ENTITY_MONSTER,
	ENTITY_NPC,
	ENTITY_OBJECT,
	ENTITY_PARTY,
} entityType_t;

typedef enum
{
	MON_TYPE_CHE			=	0,
	MON_TYPE_MAMUL,
	MON_TYPE_YOGOI,
	MON_TYPE_YOGOISUNIN,
	MON_TYPE_YAKRYOUNG,
} MON_TYPE_t;


enum
{
	MOUSEINVEN_TYPE_NONE		=	-1,
	MOUSEINVEN_TYPE_INVENTORY	=	0,
	MOUSEINVEN_TYPE_EQUIP		=	1,
	MOUSEINVEN_TYPE_DEPOT		=	2,
};


#define DEFAULT_SKIN_PART_NUMBER	4	
#define SKIN_EQUIPMENT_NUMBER		7
 
#define		MAX_NUM_STATIC_EFFECT		10				 

typedef struct
{
public:
	enum{
		maxbytes_boothName=64,
	};

public:

	int					idxOnServer;
	entityType_t		entityType;
	int					tableIdx;
	
	int					pcJob;				
	
	int					aliveState;
	
	
	
	int					isTransform;
	int					transformEntityIdx;
	int					transformType;
	int					skinPart[DEFAULT_SKIN_PART_NUMBER];
	int					equipNumber;
	int					equipment[SKIN_EQUIPMENT_NUMBER];

	
	int					worldIdx;
	vec3_t				position;
	vec3_t				angles;
	int					curRA;
	int					curSA;
	
	int					calMaxRA;
	int					calMaxSA;

	float				speed;
	i3characterEvent_t	event;
	char				name[NAMESTRING];
	int					nameColorIdx;
	//lucky 2012 NEW GM SYSTEM
	int					gameMaster;
	//end
	
	int					scriptFlag;

	
	int					numStaticEffects;
	int					staticEffectIndex [MAX_NUM_STATIC_EFFECT];
	int					staticEffectLevel [MAX_NUM_STATIC_EFFECT];

	
	int					generalSpeed;

	
	char				boothName[maxbytes_boothName];
	int					boothState;
	
	
	int					specialMonster;
	
	
	int					chaosPoint;			
	int					pvpMode;			

	
	short				guildIdx;
	char				guildName[GUILDNAMESTRING];

	
	int					joinNum;
	BOOL				waitingRoom;

	
	BOOL				hide;

	
	int					nDanBattleIdx;	

} syncCharacter_t;

#define GEN_NUMBER				8
#define GEN_HALF_NUMBER			4



#define MAX_AFFECT								8		
#define MAX_NUMBER_OF_SKILL						100		
#define MAX_NUMBER_OF_SKILL_VARIABLE			20		
#define	MAX_NUMBER_OF_OWNITEM					125		
#define MAX_INVENTORY_SIZE						48		
#define MAX_DEPOT_SIZE							42		
#define MAX_EQUIPMENT							14		
#define MAX_TRADE_SIZE							12		
#define MAX_NUMBER_OF_HOTKEY					32		
#define MASK_HOTKEY_ITEM						1000	
#define MAX_NUMBER_OF_PARTY_MEMBER				8		
#define MAX_COUNT_QUEST_VARIABLE				200		
#define MAX_COUNT_QUEST_INVENTORY				24		
#define MAX_COUNT_QUEST_STATUS					5		
#define MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD	10		

#define MAX_PRECOCITY_SIZE						6		


#define MAX_CRAFT_SOURCE_ITEM_SIZE		12
#define MAX_CRAFT_RESULT_ITEM_SIZE		2


#define MAX_UPGRADE_SOURCE_ITEM_SIZE	3

#define MAX_AUTO_TRADEITEMFILD			6		


#define MAX_BOOTH_INVENTORY_SIZE		18


#define MAX_SHARE_INVENTORY_SIZE		18




#define MAX_NUMBER_OF_FRIEND			20

#define MAX_TARGET_PCLIST		8

typedef enum 
{
	EQUIP_WEAPON_1,
	EQUIP_WEAPON_2,
	EQUIP_UPPER,
	EQUIP_LOWER,
	EQUIP_AIR_PROTECTOR,
	EQUIP_BODY_PROTECTOR,
	EQUIP_GLOVES,
	EQUIP_BOOTS,
	EQUIP_RING,
	EQUIP_NECKLACE,
	EQUIP_BRACELET,
	EQUIP_EARRING,
	EQUIP_TRANSFER,
	EQUIP_EPACK,

	NUM_EQUIP_POSITION

} characterEquipPosition_t;





typedef struct
{
	int idx;

	
	char memberName[NAMESTRING];

	int isActive;			

	int isLeader;			
	int level;				
	int gen;				
	int serverIndex;		
	int idxOnServer;		

	int isSharingExp;		
		
	int worldIdx;
	
} partyMember_t;

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

} skill_t;

#define		MAX_COUNT_AFFECT_DATA		5

typedef struct
{
	int				type;				

	int				startTime;			
	int				keepTime;			
	int				flagApply;			

	int				level;
	int				data[MAX_COUNT_AFFECT_DATA];

} affect_t;


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

struct QUESTINVENTORY
{
	short	itemIdx;		
	
	byte	itemCount;		
	byte	questIdx;		
};


typedef struct
{
	union
	{
		int					iItem;
		QUESTINVENTORY		item;
	};
		
} questInventory_t;

typedef struct
{
	int		idx;			
	int		questIdx;		

	
	int		dField[MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD];

	char	partyOrganizerName[NAMESTRING];	
	int		partyOrganizeServer;			
	int		partyOrganizeTime;				

	int		clearTime;		
	int		limitTime;		

} questStatus_t;



#define MAX_NUMBER_OF_ACTION	64

enum
{
	ACTION_ATTACK		=	0,
	ACTION_ASSIST,
	ACTION_FOLLOW,
	ACTION_EPACK_RA,
	ACTION_EPACK_SA,
	ACTION_SIT,
	ACTION_PICKUP_ITEM,
	ACTION_TRADE,
	ACTION_PARTY_INVITE,
	ACTION_PARTY_OUT,
	ACTION_PARTY_FORCEDOUT,
	ACTION_PARTY_DISORGANIZE,

	
	ACTION_MAP,
	ACTION_BOOTH,

	
	ACTION_FRIEND_MAKE,
	ACTION_FRIEND_BYE,

	
	ACTION_SHOUT,
	ACTION_BOW,
	
	ACTION_CHATTING,


	ACTION_REGIST_HELPER,
	ACTION_GIVE_POINT,

	NUM_ACTION_TYPE,

};

typedef struct
{
	int				id;					
	int				iconIdx;			

	
	int				nLPackID;			
	int				nDescIdx;			

	
	int				nFreeLPackID;		
	int				nFreeDescIdx;		

} actionTable_t;


typedef struct 
{
	int			itemIdx;
	int			price;
} boothItem_t;


typedef struct
{
	unsigned char		level;
	unsigned char		job;
	unsigned char		worldIdx;
	char				name[NAMESTRING];
} friend_t;



typedef struct 
{
	char	str[CHATSTRING];
	DWORD	color;
	int		type;
} chatMessage_t;





#define MAX_GUILDS								500		
#define MAX_GUILD_MASTERS						1		
#define MAX_GUILD_SUBMASTERS					4		
#define MAX_GUILD_FACTIONMASTERS				8		
#define MAX_GUILD_MATES							50		

#define GUILDMARKSIZE							768		

#define GUILD_ORGANIZABLE_NAK					4000000	
#define GUILD_ORGANIZABLE_LEVEL					30		
#define GUILD_ORGANIZABLE_RANKPOINT				100		

enum
{
	GUILD_AUTHORITY_ADDUSER = 1,
	GUILD_AUTHORITY_REMOVEUSER = 2,

};

enum
{
	GUILD_MASTER = 1,
	GUILD_SUBMASTER = 2,
	GUILD_FACTIONMASTER = 4,
	GUILD_MATE = 8,
	GUILD_ALLRANK = 15,

};

#define	GUILD_IDX							0x00000001
#define	GUILD_SERVERGROUPID					0x00000002
#define	GUILD_NAME							0x00000004
#define	GUILD_MARK							0x00000008
#define	GUILD_MARKUPDATETIME				0x00000010
#define	GUILD_NOTICE						0x00000020
#define	GUILD_FAMEPOINT						0x00000040
#define	GUILD_EXPPOINT						0x00000080
#define	GUILD_RAIDPOINT						0x00000100
#define	GUILD_PVPPOINT						0x00000200
#define	GUILD_HUNTERPOINT					0x00000400
#define	GUILD_LEVEL							0x00000800
#define	GUILD_MASTERNAME					0x00001000
#define	GUILD_SUBMASTERNAME					0x00002000
#define	GUILD_FACTIONMASTERNAME				0x00004000

#define	GUILD_MATENAME						0x00008000
#define	GUILD_MATEAUTHORITY					0x00010000
#define	GUILD_MATELASTCONNECTTIME			0x00020000
#define	GUILD_MASTERNOTE					0x00040000

#define GUILD_USERLEVEL						0x00080000
#define GUILD_USERJOB						0x00100000
#define GUILD_USERLOCATION					0x00200000

#define	GUILD_ALL1							0x00007FFF
#define	GUILD_ALL2							0xFFFF8000
#define	GUILD_ALL							0xFFFFFFFF

#define	GUILD_UPDATE_ORGANIZE				0x00000000
#define	GUILD_UPDATE_DISORGANIZE			0x00000000
#define	GUILD_UPDATE_ADDUSER				( GUILD_MATENAME )
#define	GUILD_UPDATE_REMOVEUSER				( GUILD_MATENAME )



#define	GUILD_UPDATE_USER					( GUILD_USERLEVEL | GUILD_USERJOB | GUILD_USERLOCATION )
#define	GUILD_UPDATE_ALL					0xFFFFFFFF


#define GUILD_MASTER_AUTHORITY					( GUILD_AUTHORITY_ADDMATE | GUILD_AUTHORITY_DELETEMATE )
#define GUILD_SUBMASTER_AUTHORITY				( 0 )
#define GUILD_FACTIONMASTER_AUTHORITY			( 0 )
#define GUILD_MATE_AUTHORITY					( 0 )











typedef struct
{
	short			idx;														
	char			serverGroupID;												
	char			name[GUILDNAMESTRING];										
	char			mark[GUILDMARKSIZE];										
	unsigned int	markUpdateTime;												
	char			notice[GUILDNOTICESTRING];									
	int				famePoint;													
	int				expPoint;													
	int				raidPoint;													
	int				pvpPoint;													
	int				hunterPoint;												
	unsigned short	level;														
	char			masterName[NAMESTRING];										
	char			subMasterName[MAX_GUILD_SUBMASTERS][NAMESTRING];			
	char			factionMasterName[MAX_GUILD_FACTIONMASTERS][NAMESTRING];	
	char			mateName[MAX_GUILD_MATES][NAMESTRING];						
	unsigned char	mateAuthority[MAX_GUILD_MATES];								
	unsigned int	mateLastConnectTime[MAX_GUILD_MATES];						
	char			masterNote[MAX_GUILD_MATES][MASTERNOTESTRING];				

	
	char			userName[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES][NAMESTRING];
	int				userLevel[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES];
	int				userJob[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES];
	int				userLocation[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES];

	int				totalUser;		
	int				onlineUser;		
	int				offlineUser;	
} guild_t;

typedef struct _battleuserInfo{
	short int m_iwin;
	short int m_idraw;
	short int m_idefeat;
	short int m_igonryunUseCount;
	
	BOOL  m_bstate;
    _battleuserInfo():m_iwin(0),m_idraw(0),m_idefeat(0),m_igonryunUseCount(0),m_bstate(FALSE){}
} BattleUserInfo_t;



struct tagWorldChatting{
	long lDate;
	int  iMaxCount;
	int  iDecreaseCount;
};



typedef struct
{
	
	

	int					characterID;			


	int					pcClass;				
	int					pcJob;
	int					skinPart[DEFAULT_SKIN_PART_NUMBER];
	char				name[NAMESTRING];		

	char				rank[NAMESTRING];		
	int					rankPoint;				

	int					age;					
	int					worldIdx;				
	
	
	
	
	int					level;					
	__int64				exp;					
	int					nextExp;				
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
	int					calForce;			
	int					calSoulpower;		
	int					calAgility;			
	int					calVitality;		
	int					bonusStatusPoint;	
	

	
	
	int					curRA;				
	int					curSA;				
	int					baseMaxRA;			
	int					baseMaxSA;			
	int					calMaxRA;			
	int					calMaxSA;			
	int					curChargeNSE;		
	int					curChargeSE;		
	int					curChargeRA;		
	int					curChargeSA;		
	int					maxChargeRA;		
	int					maxChargeSA;		
	int					onceRecoveryRA;		
	int					onceRecoverySA;		
	int					baseRecoveryRARate;	
	int					baseRecoverySARate;	

	int					calRecoveryRARate;	
	int					calRecoverySARate;	

	
	int					calDecSaUse;		

	
	
	int					curWeapon;			
	
	int					chaosPoint;				
	int					chaosTime;
	int					targetingType;			
	
	char				targetPCList[MAX_TARGET_PCLIST][20];	
	int					targetPCListTime[MAX_TARGET_PCLIST];

	int					targetType;			
	int					targetIdx;			

	int					basePhyMinDmg;
	int					basePhyMaxDmg;
	int					baseMagMinDmg;
	int					baseMagMaxDmg;
	int					baseHitPower;
	int					baseAttackDelay;
	int					baseAttackLength;
	int					baseAttackRange;
	

	
	
	int					calPhyMinDmg;
	int					calPhyMaxDmg;
	int					calMagMinDmg;
	int					calMagMaxDmg;
	int					calHitPower;
	int					calAttackDelay;
	int					calHitRate;	
	int					atkPowerRACost;
	int					atkPowerSACost;
	int					atkBowSACost;
	int					attackSpeedRate;
	int					calAttackLength;
	int					resultHitRate;
	int					resultAvoidRate;
	float				calCriticalRate;
	int					calCriticalDmg;
	

	
	
	int					baseDefensePower;
	int					basePositiveRes;
	int					baseNegativeRes;
	float				baseVelocity;
	int					baseStunTime;
	int					baseAvoidPower;
	

	
	
	int					calDefensePower;		
	int					calPositiveRes;
	int					calNegativeRes;
	float				calVelocity;			
	int					calStunTime;			
	int					calAvoidPower;			
	
	
	int					curItemWeight;			
	int					baseItemWeight;			
	int					calItemWeight;			

	
	
	int					affectNumber;			
	affect_t			affect[MAX_AFFECT];		
	

	
	
	int					skillNumber;								
	skill_t				skill[MAX_NUMBER_OF_SKILL];					
	float				skillCurCooltime[MAX_NUMBER_OF_SKILL];		
	float				skillMaxCooltime[MAX_NUMBER_OF_SKILL];		
	float				skillCastingtime;
	int					skillCastingState;
	float				skillMaxCastingtime;
	int					bonusSkillPoint;
	int					skillVariable[MAX_NUMBER_OF_SKILL_VARIABLE];

	
	
	item_t				item[MAX_NUMBER_OF_OWNITEM];
	int					itemNumber;
	int					inventory[MAX_INVENTORY_SIZE];
	int					depot[MAX_DEPOT_SIZE];
	int					depotSE;
	int					equipment[MAX_EQUIPMENT];
	int					mouseInventory;
	int					mouseInventoryType;
	
	
	int					precocityTime[MAX_PRECOCITY_SIZE];
	int					precocityInventory[MAX_PRECOCITY_SIZE];
	int					craftSourceInventory[MAX_CRAFT_SOURCE_ITEM_SIZE];
	int					craftResultInventory[MAX_CRAFT_RESULT_ITEM_SIZE];
	boothItem_t			boothInventory[MAX_BOOTH_INVENTORY_SIZE];
	int					upgradeSourceInventory[MAX_UPGRADE_SOURCE_ITEM_SIZE];
	int					upgradeResultInventory;

	
	int					shareInventory[MAX_SHARE_INVENTORY_SIZE];
	

	
	
	int					myTrade[MAX_TRADE_SIZE];
	int					otherTrade[MAX_TRADE_SIZE];
	item_t				tradeItem[MAX_TRADE_SIZE];
	

	
	
	char				organizerName[NAMESTRING];			
	int					organizeServer;						
	int					organizeTime;						
	int					memberNumber;						
	int					itemDivideType;						
	int					canJoin;							
	partyMember_t		member[MAX_NUMBER_OF_PARTY_MEMBER];	
	int					partyIndex;							
	int					isLeader;							
    
	bool				m_bischange;						
	

	
	
	questVariable_t		questVar[MAX_COUNT_QUEST_VARIABLE];
	questInventory_t	questInventory[MAX_COUNT_QUEST_INVENTORY];
	questStatus_t		questStatus[MAX_COUNT_QUEST_STATUS];
	int					questNumber;						
	int					questPoint;
	

	
	
	int					hotkeyType[MAX_NUMBER_OF_HOTKEY];
	int					hotkeyIdx[MAX_NUMBER_OF_HOTKEY];
	

	
	
	int					tempFriendIdx;
	friend_t			friendList[MAX_NUMBER_OF_FRIEND];
	
	
	
	short				guildIdx;
	unsigned int		guildJoinTime; 
	unsigned int		guildSecedeTime; 
	guild_t				guild;

	
	bool				m_bChatBan_Flag;  
	DWORD				m_dwLimite_ChatTime; 

	
	BOOL				m_bbattleMode;		
	BattleUserInfo_t    m_tbattleInfo;		
	int					m_imyTrade[MAX_TRADE_SIZE];	
	int					m_iotherTrade[MAX_TRADE_SIZE];
	item_t				m_itradeItem[MAX_TRADE_SIZE];
	

	
	CPostManager		m_PostMng;
	int					m_ItemsInPost[CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE];	

	
	struct tagWorldChatting			WorldChatting;
	
	
	float				m_fItemPrecocityTimeRatio;	

	int					m_nMemberShipType;


	
	struct WorldChangeSystem  m_WorldChageSystem;
	
	public:
	
		
		struct tagGM{
			public:
				struct tagChat{
					int RemainBanTime;
				};
			public:
				struct tagChat Chat;

		};
		struct tagGM GM;	
		

	
	struct CashMallItem_OptionStone	m_CashMall_OptionStone;
	
	int		naccumulationKillpoint;	
	int		Killpoint;				
} myCharacterInfo_t;


typedef	struct
{
	int			idxOnServer;
	char		itemName [32];
	vec3_t		position;
	vec3_t		angles;
} itemGameData_t;

typedef struct
{
	int					pcClass;
	
	int					skinPart[DEFAULT_SKIN_PART_NUMBER];
	char				name[NAMESTRING];
	
	int					force; 			
	int					soulpower;		
	int					agility; 		
	int					vitality;		

	int					maxHairCount;
	int					maxFaceCount;

	int					curForce; 			
	int					curSoulpower;		
	int					curAgility; 		
	int					curVitality;		

	int					bonusPoint;

} charEditData_t;

typedef struct
{
	int active;
	unsigned int setTime;
	int countdown;
	int isComplete;
} serverShutdownTimer_t;


#define		MAX_NUMBER_OF_STORE_ITEM		( 3 * 36 )
#define		STORE_ITEM_INDEX_EPACK_RA			20000
#define		STORE_ITEM_INDEX_EPACK_SA			20001
#define		STORE_SKILL_MASK					10000

typedef struct
{
	item_t		item;					
	int			maxNumber;				
	int			curNumber;
	int			price;					
	int			idxOnServer;
} storeItem_t;


enum
{
	STATUS_FORCE		=	0,
	STATUS_SOULPOWER,
	STATUS_AGILITY,
	STATUS_VITALITY,
};



enum
{
	TARGETING_ONLY_MONSTER	=	0,
	TARGETING_NORMAL,
	TARGETING_SAFE,
	TARGETING_EVERY,
	TARGETING_EXCEPT_PARTY,
	TARGETING_EXCEPT_GUILD,
};








enum
{
	CHAOS_PURE,
	CHAOS_GENERAL,
	CHAOS_DISORDER,
	CHAOS_MONSTER,
	CHAOS_ATROCITY,
};

enum
{
	QS_REWARD_EXP	=	0,
	QS_REWARD_GENEXP,
	QS_REWARD_GENCAP,
	QS_REWARD_NAK,
	QS_REWARD_ITEM,
	QS_REWARD_FAME,
};

enum
{
	OPTION_SPHERE_LEAF	=	0,
	OPTION_SPHERE_MAD,
	OPTION_SPHERE_WAVE,
	OPTION_SPHERE_FIRE,
	OPTION_SPHERE_ART,

};


#define		MAX_MAIL_COUNT		50				
#define		MAIL_TITLESIZE		60				
#define		MAIL_STRSIZE		2000			

struct MailEntry
{
	byte	mailIdx;
	char	fromName[NAMESTRING];
	char	mailTitle[MAIL_TITLESIZE];
	char	mailTime[32];						
	char	mailStr[MAIL_STRSIZE];
	byte	mailState;
	bool	checkFlag;
};


enum enumRepairType
{
	REPAIR_ITEM_SUB0,
	REPAIR_ITEM_SUB1,
	REPAIR_ITEM_ALL,
};


enum enumInvenType
{
	INVENTYPE_INVENTORY,
	INVENTYPE_EQUIP
};



#endif
