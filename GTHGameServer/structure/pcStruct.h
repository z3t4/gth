



#include "..\HelperSystem\tagHelper.h"

#include "../AhnLab/AntiCpSvrFunc.h"
#include "../AhnLab/AntiCpCnt.h"

#define MAX_AFFECT								8		
#define MAX_NUMBER_OF_SKILL						100		
#define MAX_NUMBER_OF_SKILL_VARIABLE			20		
#define	MAX_NUMBER_OF_OWNITEM					125		
#define MAX_INVENTORY_SIZE						48		
#define MAX_DEPOT_SIZE							42		
#define MAX_EQUIPMENT							14		
#define MAX_TRADE_SIZE							12		
#define MAX_NUMBER_OF_HOTKEY					32		
#define MAX_PRECOCITY_SIZE						6		

#define MASK_HOTKEY_ACTION						1000	
#define MASK_HOTKEY_ITEM						2000	

#define MAX_NUMBER_OF_PARTY_MEMBER				8		

#define MAX_COUNT_QUEST_VARIABLE				200		
#define MAX_COUNT_QUEST_INVENTORY				24		
#define MAX_COUNT_QUEST_STATUS					5		
#define MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD	10		

#define DEFAULT_SKINPART_NUMBER					4		
#define PC_GAMEDATA_SAVETIME					600000	

#define PARTY_INFO_REQUESTTIME					300000	


#define MAX_CRAFT_SOURCE_ITEM_SIZE		12
#define MAX_CRAFT_RESULT_ITEM_SIZE		2



#define MAX_BOOTH_INVENTORY_SIZE		18
#define MAX_COUNT_BOOTH_BUYER			50
#define MAX_UPGRADE_SOURCE_ITEM_SIZE	3
#define MAX_AUTO_TRADEITEMFILD			6


#define MAX_COUNT_TRAIN_MONSTER					8
#define MAX_COUNT_AGGRESSIVE_TRAIN_MONSTER		4
#define TRAIN_MONSTER_LIMIT_TIME		30000


#define	MAX_FRIENDS				20

#define MAX_TARGET_PCLIST		8


#define MAX_GUILDS				500


#define	MAX_NUMBER_OF_STORE_ITEM			( 36 * 3 )


#define CHAOVALUE				300
#define CHAOSTIME				60000
#define CHAOSDECVALUE			100
#define CHAOSDECSPEEDRATE		30.f
#define CHAOSPURE				0
#define CHAOSGENERATE			10
#define CHAOSDISORDER           10000
#define CHAOSMONSTER			29500
#define CHAOSMAX     			30000
#define CHAOSADDRATE			1.2f
#define CHAOSVALUEFALTE         100.0f




#define PARTYITEMGENERATE			0
#define PARTYITEMRANDOM             1
#define PARTYITEMORDER				2





#define SKILL_MAX_BASIC_JOB_TYPE 4


enum
{
	MONEY_FREE_USER=0,
	MONEY_PAY_USER,
	MONEY_PRIMIUM_USER
};






enum
{
	PC_MENU_PARTY		=	1 << 0,
	PC_MENU_GUILD		=	1 << 1,
	PC_MENU_TRADE		=	1 << 2,
	PC_MENU_VS			=	1 << 3,
	PC_MENU_EXIT		=	1 << 4,
};

enum
{
	PC_MENU_TYPE_NONE	=	-1,
	PC_MENU_TYPE_NORMAL	=	0,
};

enum
{
	BUSY_STATE_NONE		=	0,
	BUSY_STATE_NPC,
	BUSY_STATE_TRADE,
	BUSY_STATE_CRAFT,
	BUSY_STATE_UPGRADE,
	BUSY_STATE_BOOTH_SELL_PREPARE,
	BUSY_STATE_BOOTH_SELL,
	BUSY_STATE_BOOTH_BUY,
	
	BUSY_STATE_REQUEST, 
	
	BUSY_STATE_QUESTSCROLL,
	
	BUSY_STATE_NPC_INVENTORY,
	
	BUSY_STATE_BATTLEPRACTICE,
	
	BUSY_STATE_DAN_BATTLE,
	
	BUSY_STATE_WORLDCHAGNE,

	
};

enum
{
	MOUSEINVEN_TYPE_NONE		=	-1,
	MOUSEINVEN_TYPE_INVENTORY	=	0,
	MOUSEINVEN_TYPE_EQUIP		=	1,
	MOUSEINVEN_TYPE_DEPOT		=	2,
};








typedef struct
{
	char	summoner[NAMESTRING];
	int		worldIdx;
	vec3_t	position;
	int		forcedYn;
} summons_t;


typedef struct
{
	int idx;

	
	char	memberName[NAMESTRING];

	int		isActive;					

	int		isLeader;					
	int		level;						
	int		gen;						
	int		serverIndex;				
	int		idxOnServer;				

	int		isSharingExp;				
		

	int		worldIdx;					
	
	bool				m_bischange;					
	int                 m_iorderindex; 
	
} partyMember_t;

#define	MAX_COUNT_QUEST_COMPLETE	100


struct QUESTVARIABLE
{
	char	isComplete;		
	char	step;			
	byte	completeCount;	
	byte	dummy;			
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

typedef struct 
{
	int			itemIdx;
	int			price;
} boothItem_t;

typedef char	FriendName[NAMESTRING] ;
struct FriendsList
{
	int			count ;
	FriendName	names[MAX_FRIENDS] ;
	
} ;


enum																					
{
	BILL_SEND_NONE,
	BILL_SEND_START,
	BILL_SEND_END,
};


typedef struct
{
	
	
	unsigned	int					normalLastAttackTime;	
	int					idx;				
	int					active;				
	int					ready;				
	
	
	int					sendReplyConnect;	
	
	int					alive;			

	char				userID[IDSTRING+1];
	int					characterID;	
	int					serverGroupID;	

	i3characterEvent_t	event;			
	i3characterEvent_t	oldEvent;		
	

	
	
	message_t			message[MAX_NUMBER_OF_MESSAGE];
	int					messageNumber;

	entityType_t		entityType;				
	int					pcClass;				
	int					pcJob;
	int					skinPart[DEFAULT_SKINPART_NUMBER];

	char				name[NAMESTRING+1];		
	int					age;					

	char				rank[NAMESTRING+1];		
	int					rankPoint;				

	float				recoveryTime;			
	
	

	
	
	int					moveType;					
	int					worldIdx;					
	
	int					zoneIdx;					
	vec3_t				position;					
	vec3_t				reliablePosition;
	int					reliableWorldIdx;
	vec3_t				angles;					

	int					generalSpeed;			
	
	
	
	
	int					level;					
	__int64				exp;					
	int					nextExp;				
	int					reduceExp;				

	
	int					reduceNark;				

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
	

	
	
	float				curRA;				
	float				curSA;				

	float				baseMaxRA;			
	float				baseMaxSA;			
	float				calMaxRA;			
	float				calMaxSA;			

	int					curChargeNSE;		
	int					curChargeSE;		

	
	float				curChargeRA;		
	float				curChargeSA;		
	

	int					maxChargeRA;		
	int					maxChargeSA;		
	int					onceRecoveryRA;		
	int					onceRecoverySA;		

	int					epackUseRAWaitTime;
	int					epackUseSAWaitTime;
	

	
	
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

	int					baseCriticalDmg;
	float				baseCriticalRate;

	int					baseHitPower;			

	int					baseItemPhyMinDmg;			
	int					baseItemPhyMaxDmg;
	int					baseItemMagMinDmg;			
	int					baseItemMagMaxDmg;

	int					baseItemCriticalDmg;
	float				baseItemCriticalRate;

	int					baseItemHitPower;

	int					baseAttackSpeed;
	int					baseAttackLength;		
	int					baseAttackRange;		
	
	
	int					atkWeaponType;			
	

	
	
	int					calPhyMinDmg;	
	int					calPhyMaxDmg;
	int					calMagMinDmg;	
	int					calMagMaxDmg;

	int					calCriticalDmg;
	float				calCriticalRate;

	int					calHitPower;	
	int					calItemHitPower;

	int					calAttackDelay;		
	int					calHitRate;			
	int					calAttackLength;

	int					calPosMinDmg;
	int					calPosMaxDmg;
	int					calNegMinDmg;
	int					calNegMaxDmg;

	int					attackSpeedRate;

	int					atkPowerRACost;
	int					atkPowerSACost;
	int					atkBowSACost;
	int					resultHitRate;		
	int					resultAvoidRate;	
	int					resultValid;
	int					attackComboType;	
	
	int					forceAtkDelay;		

	

	
	
	int					baseDefensePower;				
	int					baseAvoidPower;					
	float				baseVelocity;					
	int					baseStunTime;					

	int					basePositiveRes;
	int					baseNegativeRes;

	int					baseItemDefensePower;
	int					baseItemPositiveRes;
	int					baseItemNegativeRes;
	int					baseItemAvoidPower;				
	

	
	
	int					calDefensePower;
	int					calAvoidPower;					
	float				calVelocity;					
	int					calStunTime;					
	
	int					calPositiveRes;
	int					calNegativeRes;
	
	

	int					curItemWeight;					
	int					baseItemWeight;					
	int					calItemWeight;					
	
	
	
	affect_t			affect[MAX_AFFECT];
	int					affectNumber;

	

	
	
	int					skillNumber;				
	skill_t				skill[MAX_NUMBER_OF_SKILL];	
	float				skillCooltime[MAX_NUMBER_OF_SKILL];		
	float				skillCastingtime;
	int					skillCastingState;
	
	int					saveSkillIdx;				
	int					curSkillIdx;				
	int					bNotOwnSkill;				
	int					saveSkillLevel;				

	vec3_t				saveSkillTargetPos;			
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
	
	int					npcAutoSourceInventory[MAX_AUTO_TRADEITEMFILD];


		
	int					upgradeSourceInventory[MAX_UPGRADE_SOURCE_ITEM_SIZE];
	int					upgradeResultInventory;
	boothItem_t			boothInventory[MAX_BOOTH_INVENTORY_SIZE];
	char				boothName[128];
	char				boothInfo[128];
	int					boothBuyerIdx[MAX_COUNT_BOOTH_BUYER];

	
	
	int					myTrade[MAX_TRADE_SIZE];
	int					otherTrade[MAX_TRADE_SIZE];
	item_t				tradeItem[MAX_TRADE_SIZE];
	int					tradeSE;
	int					otherTradeSE;
	int					tradeOkSignal;
	int					tradeAdmitSignal;
	

	

	
	
	int					flagChangeServer;		
	i3socket_t			sock;					
	unsigned int		syncTime;				
	unsigned int		nextSaveTime;			
	vec3_t				mousePos;				
	int					isSetServerShutdown;	
	int					gameMaster;				
	summons_t			summonsInfo;			
	int					changeServerWorldIdx;	
	int					normalNetwork;			
	int					gmBusy;
	int					gmHide;
	int					serverAckMsgNeedFlag;	
	int					serverAckMsgWaitTimer;
	int					flagDBLoading;			
	int					penaltyTime;			
	int					levelUpTime;			
	int					genLevelUpTime;			
	
	

	
	
	npcAiInPc_t			npcAi;					
	int					npcIdx;					
	int					storeIdx;				
	int					bindingIdx;				
	int					killerIdx;				
	int					killerType;				
	int					curDamage;				
	int					lastAttackState;		
	int					busyState;				
	int					friendlyEntityType;				
	int					friendlyEntityIdx;				
	int					friendlyEntityWorldIdx;			
	char				friendlyEntityName[NAMESTRING+1];	
	
	int					damagedEntityType;
	int					damagedEntityIdx;

	
	int					pvpMode;
	int					trainMonsterIdx[MAX_COUNT_TRAIN_MONSTER];
	int					trainMonsterTime[MAX_COUNT_TRAIN_MONSTER];
	int					numTrainMonster;
	

	
	
	float				autoRecoveryRATime;
	float				autoRecoverySATime;
	float				optValue[NUM_OPTION_VALUE];
	int					isTransform;				
	int					transformType;				
	int					transformEntityIdx;			
	

	
	
	char				organizerName[NAMESTRING+1];			
	int					organizeServer;						
	int					organizeTime;						
	int					memberNumber;						

	int					canJoin;							
	partyMember_t		member[MAX_NUMBER_OF_PARTY_MEMBER];	
	int					partyIndex;							
	int					isLeader;							
	int					isSharingParty;						
	int					partyExp;
	int					partyGenExp;
	int					serverAckMsgPartyFlag;				
	int					serverAckMsgPartyWaitTimer;
	int					serverPartyErrorFlag;
	float				partyInfoRequestTime;
	
	
	struct PartyRandom{
		bool                m_bischange;
		int					itemDivideType;	
		int                 m_iorderindex; 
	}_partyRandomInfo_t;

	
	

	
	
	questVariable_t		questVar[MAX_COUNT_QUEST_VARIABLE];
	questInventory_t	questInventory[MAX_COUNT_QUEST_INVENTORY];
	questStatus_t		questStatus[MAX_COUNT_QUEST_STATUS];
	int					questNumber;						
	int					questPoint;
	bool				bautoeffectisvalid;
 	

	
	
	int					hotkeyType[MAX_NUMBER_OF_HOTKEY];
	int					hotkeyIdx[MAX_NUMBER_OF_HOTKEY];
	

	
	
	
	bool				friendsBusy ;
	FriendsList			friends ;			

	
	int					raCostPerSec;
	int					saCostPerSec;

	
	
	short				guildIdx;

	
	
	int					rankInGuild;
	int					rankIdxInGuild;
	char				guildName[GUILDNAMESTRING+1];


	char				tempGuildMateName[NAMESTRING+1];
	
	


	struct tagGuild{
		public:
			struct tagFromInvite{
				char Name[NAMESTRING+1];
			};
		public:
			struct tagFromInvite FromInvite;
	}Guild;





	
	


	
	int					skillDamage;

	
	
	int					RegGambleItems[MAX_NUMBER_OF_STORE_ITEM];
	int					RegGamblePrice[MAX_NUMBER_OF_STORE_ITEM];
	

	
	struct tagGonryunBattle{
		public:
			enum{
				NAK_per_ONEGOLD = 1000000,				
			};
			
			struct tagTowGyun{
				int iTournamentGUID;
				int BuyNum;
			};

			struct tagShareWin{
				public:
					enum{
						MAX_INVENTORY_NUMBER = 18,
					};

				public:
					__int64 i64TotalNak;
					int		iFortuneStone;
					int		Inventory[MAX_INVENTORY_NUMBER];
			};

			struct tagObserver{
				BOOL bActive;
			};

		public:
			struct tagTowGyun  TwoGyun;
			struct tagShareWin ShareWin;			
			int				   m_iJoinNum;		
			BOOL			   m_bWaitingRoom; 


			struct tagObserver Observer;
	}GonryunBattle;
	


	
	struct tagGonryunBattlePractice{		
		public:
			enum enumMemberShip{
				MEMBERSHIP_NONE=0,
				MEMBERSHIP_LEADER,
				MEMBERSHIP_OPENENT,
				MEMBERSHIP_WAITER,	
			};		

			enum {
				MAX_GBP_INVENTORY_SIZE = 12,
			};


			struct tagRecord{
				int	iwin;						
				int	idraw;						
				int	idefeat;					
			};

		public:
		
			enum enumMemberShip MemberShip;
			enum enumState		PC_State;
			
			int					Inventory[MAX_GBP_INVENTORY_SIZE];
			int					iBattingSE;
			BOOL				bBattingOkSignal;
			BOOL				bBattingAdmitSignal;
			struct tagRecord	Record;
			BOOL				bisWaitrespond;

	};


		
	

	int					BillSendType;													


	
	struct tagAddBill{
		public:
			enum enumStep{
				step_none=0,
				step_wait_game_start_packet,
				step_billing,
				step_end,
			};
		public:
			int step;
			
			char User_Status[3];
			int  time_trGameStartPacket;
	}AddBill;
	


	public:	
		struct tagPremiumInfo{
			public:
				enum{
					maxmap_number=30,										
					max_charactor_slot_num=3,
				};

				enum enumMeberShipType{
					Free=0,
					Pay,
					Max,
				};



				struct tagMap{
					struct tagAttr
					{
						BOOL    bAllow;													
						int		iRemainSecond;
						BOOL    bDecreseRemainSecond;									
						BOOL	bDecreseAllRemainSecond;

						
						float	fAddExpRatio;											
						float	fAddGenExpRatio;										
						float	fAddGenCapabilityRatio;									
						float	fAddItemDropRatio;										
						float	fAddNarkRatio;											
						
					};

					struct tagAttr Default;
					struct tagAttr Cur;
				};

				struct tagGamble{
					public:
						enum{
							maxnum_apperance_item_group=16,							
						};
						struct tagItemGroup{
							BOOL bApperance;
						};
					public:
						float fAddPriceRatio;												
						struct tagItemGroup ItemGroupArray[maxnum_apperance_item_group];	
				};

				
				struct tagDie{
					float fExpRatio;												
					float fNakRatio;												
					
				};
				

				struct tagItemOptionUpgrade{
					int   iMaxlevel;												
					float fAddPriceRatio;											
				};


				
				
				struct tagStatus{
					int iDefaultInitCount;	
					int iInitCount;			
				};
				
				
				
				struct tagGonyounPracticeBattle{
					long lDateV2;		
					int  iMaxCount;	
					int  iDecreseCount; 
				};

				struct tagWorldChatting{
					long lDate;
					int  iMaxCount;
					int  iDecreaseCount;
				};

				
				struct tagItemDurability{
					float fRepareCostRate;				
					int   iPrepareRequirdurability;		
				};
				

			public:		
				BOOL						    bLoad;
				enum enumMeberShipType			iMemberShipType;

				int								iRemainSecond;
				struct tagMap					Map[maxmap_number];				
				struct tagGamble				Gamble;
				struct tagItemOptionUpgrade		ItemOptionUpgrade;		
				struct tagDie					Die;		
				BOOL							bCreatePremiumBooth;	 
				int								iMaxLevelItemCraft;		 
				float							fItemPrecocityTimeRatio; 
				BOOL							bItemRecycle;			 
				struct tagGonyounPracticeBattle GonyounPracticeBattle;
				struct tagWorldChatting			WorldChatting;
				struct tagStatus				Status;			
				struct tagItemDurability		ItemDurability;
		};


		enum enumNameColor{
			normal=0,
			week=1,
			month=2,
			ramification=3,
			year=4,
		};

		struct tagGiftCard{
			public:
				enum{
					maxbytes_serial_num=20,
					maxbytes_password=20,

					step_none=0,
					step_request,
				};


			public:
				int  step;
				char serial[maxbytes_serial_num+1];
				char password[maxbytes_password+1];
		};

		
		struct tagLogin{
			public:
				struct tagLoading{
					BOOL bPremiumInfo;
					BOOL bCharactor;
					BOOL bQuest;
					BOOL bQuestStatus;
					BOOL bSkill;
					BOOL bFriends;
					BOOL bHelpList;
					BOOL bItem;
				};
			public:
				BOOL bComplete;
				struct tagLoading Loading;
		};


		struct tagHelperAdd{
			public:
				struct tagReqPlayer{
					int  pcIdx;
					char Name[NAMESTRING+1];
				};
			public:
				struct tagReqPlayer RequestPlayer;

		};



	public:
		enum enumNameColor enumNameColor;
		struct tagPremiumInfo PremiumInfo;

		
		CPostManager			m_PostMng;
		int						m_ItemsInPost[CPostManager::MAX_POSTPACKAGE_COUNT];	
		
		
		
		struct tagPremiumInfo::tagMap *GetCurMap()
		{
			if(worldIdx < 0 || worldIdx >= tagPremiumInfo::maxmap_number) return NULL;

			return &PremiumInfo.Map[worldIdx];
		};

		
		struct WorldChangeSystem	m_WorldChangeSystem;
		
		
		
		struct tagHelper tHelper;
		struct tagHelperAdd HelperAdd;
		struct tagGonryunBattlePractice GonryunBattlePractice;
		

		struct tagGiftCard GiftCard;

		
		BOOL					m_bHide;
		struct tagLogin			Login;

	public:
		struct tagGM{
			public:
				struct tagChat{
					public:
						enum{
							max_ban_time=60 * 60 * 24 * 30,
						};
					public:
						int RemainBanTimePerSecond;
				};
			public:
				struct tagChat Chat;
		};

		struct tagGM GM;

		
		struct CashMallItem_OptionStone	m_CashMall_OptionStone;
		struct sPlayerItemExtendItemInfo m_ItemExtendInfo;

	public:







		
		struct sUserHackAckMsg{
			BYTE byReqInfo[ANTICPSVRREQLEN];
			BYTE byReqMsg[ANTICPSVRREQLEN];
			BYTE byAckMsg[ANTICPCNTACKLEN];
			float SendTime;
			float ReciveTime;
			BOOL  MessageSend;
			BYTE  SendCount;
		};
		struct sUserHackAckMsg	m_AhnLabMsg;


		NewDanBattleSystem::DanBattleInfo DanBattleInfo;
		int				nAccumulationBattleKillPoint;			
		

		
		int					GuildDeconstructionTime;
		int					GuildsecedeTime;

		
		float			fWorldChangeStartTime;
		int				iChangeWorldIdx;
		int				NetlastReceiveTime;
	
} playerCharacter_t;




typedef playerCharacter_t::tagPremiumInfo::tagMap	PremiumMap;
typedef playerCharacter_t::tagPremiumInfo::enumMeberShipType PREMIUM_MEMBERSHIP_TYPE;

#define SAFE_PCS(p) (p < 0 || p >= MAX_PCS) ? 0 : 1


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
	CHAOS_PURE				=	0,
	CHAOS_GENERAL,
	CHAOS_DISORDER,
	CHAOS_MONSTER,
	CHAOS_ATROCITY,
	
	NUM_CHAOS_TYPE,
};

typedef	std::vector<playerCharacter_t *>			LIST_PLAYER_STRUCT;



struct FriendEntry
{
	byte	level ;
	byte	job ;
	byte	worldIdx ;
	char	name[NAMESTRING];
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
};


enum enumInvenType
{
	INVENTYPE_INVENTORY = 0,
	INVENTYPE_EQUIP,
};


enum enumRepairType
{
	REPAIR_ITEM_SUB0=0,
	REPAIR_ITEM_SUB1,
	REPAIR_ITEM_ALL,	
};



enum enumAwakenQuest
{
	KNIGHT_QUEST_IDX	= 5,
	WIND_QUEST_IDX		= 4,
	WATER_QUEST_IDX		= 6,
	EARTH_QUEST_IDX		= 8

};

