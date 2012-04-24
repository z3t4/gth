#ifndef _TABLE_TOTAL_OLD_STRUCTURE_
#define _TABLE_TOTAL_OLD_STRUCTURE_

typedef struct
{
	
	int				idx;					
	char			name[20];				
	int				level;					
	int				type;					
	int				subClass;				
	int				mainClass;				
	int				rarity;					
	int				wearPosition;			
	int				size;					
	int				reqGender;				
	
	int				reqType;				
											
	int				reqLevel;				
	
	char			document[255];			

	int				reqForce;				
	int				reqAgility;				
	int				reqSoulpower;			
	int				reqVitality;			

	int				sellYn;					
	int				sellCost;				
	int				gambleCost;				

	int				durYn;					
	int				durabilty;				
	int				repairYn;				

	int				onceRepairDecDur;		

	int				weight;					
	int				sound;					

	
	int				skinningYn;				
	int				skinningPosition;		
	int				tagID;					


	int				not_used;								

	int				skinModelID[NUM_TABLE_GEN_TYPE];		

	int				tagModelID[MAX_NUMBER_OF_ITEM_PART];	
	int				tagMoveID[MAX_NUMBER_OF_ITEM_PART];		

	int				mdlItemType;			
	int				mdlItemID;				

	int				iconItemID;				

	int				reqNumber;				
	int				effect;					

	int				supplyRA;				
	int				supplySA;				

	int				quest;					
	

	
	int				attackPattern;				

	int				attackAni[NUM_TABLE_GEN_TYPE];					
	int				attackEffect;				
	
	int				attackMinDmg;				
	int				attackMaxDmg;

	int				handed;						

	int				attackDelay;				
	int				hitPower;					
	int				criticalRate;				
	int				criticalDmg;				

	int				attackLength;					
	int				attackRange;					

	int				onceRACost;					
	int				onceSACost;					
	int				onceSECost;					

	
	int				defensePower;			
	int				avoidRate;				
	int				velocity;				

	int				resistance[8];			

	
	int				basicAbsorb;			
	int				maxChargeNSE;			
	int				maxChargeSE;			

	int				maxChargeRA;			
	int				maxChargeSA;			

	int				onceRecoveryRA;			
	int				onceRecoverySA;			

	

	int				basicEfficiency;		
	int				gradeNumber;			
	int				upgradeExp[6];			

	
	int				haveFixedOpt;									
	int				isUniqueItem;									
	
	int				fixedOptionIdx[MAX_NUMBER_OF_ITEM_OPTION];			
	int				fixedOptionMinValue[MAX_NUMBER_OF_ITEM_OPTION];		
	int				fixedOptionMaxValue[MAX_NUMBER_OF_ITEM_OPTION];		
	

	
	int				unreserved[108];


} oldItemTable103_t;

typedef struct
{
	
	int				idx;					
	char			name[20];	
	char			document[255];	

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

	int				atkPowerRACost;
	int				atkPowerSACost;
	int				atkBowSACost;

	
	int				defPower;					
	int				defAvoidPower;				
	int				defPositiveRes;				
	int				defNegativeRes;				

	
	int				epkMaxChargeRA;			
	int				epkMaxChargeSA;			
	int				epkRecoveryRA;			
	int				epkRecoverySA;			

	
	int				upgBasicEfficiency;		
	int				upgNumber;				
	int				upgReqExp[6];			

	
	int				etcItemType;

	
	union
	{
		int								commonField[16];
		ITEMCOMMONFIELD_SKILLSPHERE		skillScroll;
		ITEMCOMMONFIELD_SPHERE			sphere;
	};

	itemOptionSlot_t	slotOpt[MAX_NUMBER_OF_ITEM_OPTION];

	int				stackFlag;
	int				useFlag;
	int				unreserved[126];

} oldItemTable104_t;

typedef struct
{
	int				idx;										
	
	int				optionIdx[MAX_NUMBER_OF_OPTION_IN_CLASS];
	int				optionIdxNumber;							

	char			name[20];									

	int				wearPosition;								

	
	int				attackAtt;
	int				defenseAtt;
	int				packAtt;
	int				upgradeAtt;

	
	int				unreserved[ITEM_CLASS_TABLE_UNRESERVED_TOTAL_SIZE - ITEM_CLASS_TABLE_UNRESERVED_USED_SIZE];

} oldItemClassTable_t;


typedef struct
{
	int					idx;					

	int					modelID;				
	int					aniSetType;
	
	int					tagModelID[MAX_NUMBER_OF_ITEM_PART];	
	int					tagMoveID[MAX_NUMBER_OF_ITEM_PART];	

	int					level;					
	
	char				name[20];
	
	char				document[255];
	int					size;					
	int					type;					
	int					gen;					

	int					maxRA;					
	int					maxSA;					

	int					recoveryRARate;			
	int					recoverySARate;			

	
	int					attackNumber;			

	int					attackAni[MAX_NUMBER_OF_MONSTER_ATTACK];	

	
	int					attackGenType[MAX_NUMBER_OF_MONSTER_ATTACK];
	int					minDmg[MAX_NUMBER_OF_MONSTER_ATTACK];
	int					maxDmg[MAX_NUMBER_OF_MONSTER_ATTACK];

	
	int					hitPower[MAX_NUMBER_OF_MONSTER_ATTACK];

	
	int					attackDelay[MAX_NUMBER_OF_MONSTER_ATTACK];

	
	int					attackLength[MAX_NUMBER_OF_MONSTER_ATTACK];
	
	int					attackRange[MAX_NUMBER_OF_MONSTER_ATTACK];

	
	int					criticalRate;
	
	int					criticalDmg;

	
	int					defensePower;		
	int					resistance[8];		
	int					velocity;			
	int					stunTime;			


	int					skillIdx[MAX_NUMBER_OF_MONSTER_SKILL];		
	int					skillLevel[MAX_NUMBER_OF_MONSTER_SKILL];	
	int					skillAni[MAX_NUMBER_OF_MONSTER_SKILL];		
	int					skillNumber;

	
	int					exp;					
	int					genExp;					
	int					lootNSE;				
	lootItem_t			lootItem;				

	int					pathFindingSize;		

	
	int					unreserved[127];

	

} oldMonsterBasicTable_t;

#endif
