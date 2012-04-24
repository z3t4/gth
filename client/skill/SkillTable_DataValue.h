#ifndef SKILL_TABLE_DATAVALUE_H
#define SKILL_TABLE_DATAVALUE_H


#define  SKILL_DEFAULT_DESC_INDEX	1001


#define  SKILL_DEFAULT_NAME_INDEX	2001


#define  SKILL_DEFAULT_TYPE_INDEX	3000

#define SKILL_TEST_VERSION			1


enum
{
	NO_TYPE =0,

	SKILL_CAST_TABLE_INDEX, 
	SKILL_RA_DEC_RATE,									
	SKILL_RA_INC_RATE,									
	SKILL_HITPOWER,										
	SKILL_HITRATE,										
	SKILL_PHYMINDMG,									
	SKILL_PHYMAXDMG,									
	SKILL_POSMINDMG,									
	SKILL_POSMAXDMG,									
	SKILL_CRITICALRATE,									
	SKILL_DELAYTIME,									
	SKILL_ETC_MULTIDAMAGE_DELAY,						
	SKILL_AFF_STARTTIME,								
	SKILL_AFF_KEEPTIME,									
	SKILL_AFF_HITRATE,									
	SKILL_AFF_SKILLTABLEIDX,							
	SKILL_AFF_DELAYTIME,								

	
	

	SKILL_AFFDATA_TYPE_DEFENSE_RATE_DEC,				
	SKILL_AFFDATA_TYPE_DEFENSE_RATE_INC,				

	SKILL_AFFDATA_TYPE_DECREASERA, 						
	SKILL_AFFDATA_TYPE_PHYDAMAGE,						

	SKILL_AFFDATA_TYPE_INCRARECOVERY,					
	SKILL_AFFDATA_TYPE_DECRARECOVERY, 					

	SKILL_AFFDATA_TYPE_INCSARECOVERY,					
	SKILL_AFFDATA_TYPE_DECSARECOVERY,					

	SKILL_AFFDATA_TYPE_MAXRA_INC,						
	SKILL_AFFDATA_TYPE_MAXRA_DEC,						

	SKILL_AFFDATA_TYPE_DEFENSE,							

	SKILL_AFFDATA_TYPE_VELOCITY_RATE_DEC,				
	SKILL_AFFDATA_TYPE_VELOCITY_RATE_INC,				

	SKILL_AFFDATA_TYPE_VELOCITY_INC,					

	SKILL_AFFDATA_TYPE_HITPOWER, 						
	SKILL_AFFDATA_TYPE_MAGDAMAGE ,						

	SKILL_AFFDATA_TYPE_ATTACKSPEED ,					
	SKILL_AFFDATA_TYPE_ATTACKSPEEDRATE,					

	SKILL_AFFDATA_TYPE_DECREASESAUSE,					

	SKILL_AFFDATA_TYPE_PHYDAMAGE_RATE,					
	SKILL_AFFDATA_TYPE_HITPOWRE_RATE,					

	SKILL_AFFDATA_TYPE_ONCE_ATTACK_SACOST,				
	SKILL_AFFDATA_TYPE_CRITICALRATE,					

	SKILL_AFFDATA_TYPE_AVOID_DEC,						
	SKILL_AFFDATA_TYPE_AVOID_INC,						

	
	SKILL_MSG_TYPE_RAREC,								
	SKILL_MSG_TYPE_SAREC ,								
	SKILL_MSG_TYPE_EXP_REC_RATE,						

	
	SKILL_TYPE_RA_RECOVERY, 							

	SKILL_TYPE_RANGE,									
	SKILL_TYPE_LISTPOINT,								

	
	SKILL_REPEATNUM,									
	SKILL_REPEAT_DELAY_TIME,							
	SKILL_MANASHIELD,									

	SKILL_ATK_ETC_INFO_STUN_PRO,						
	SKILL_ATK_ETC_INFO_STUN_TIME,						
	
	SKILL_PHYBONUS,										
	SKILL_POSBONUS,										

	SKILL_PHYBONUS_MENTALITY,							
	SKILL_PHYBONUS_WIND,								
	SKILL_PHYBONUS_WATER,								
	SKILL_PHYBONUS_EARTH,								

	SKILL_POSBONUS_MENTALITY,							
	SKILL_POSBONUS_WIND,								
	SKILL_POSBONUS_WATER,								
	SKILL_POSBONUS_EARTH,								

	SKILL_CASTINGTIME_RATE,								
	SKILL_ETC_MULTIDAMAGE_DELAY_INC,
	SKILL_DAMAGE_TARGET_100_ELSE_LITE_DAMAGE,

	
	SKILL_RA_RECOVERY_TIME,
	SKILL_SA_RECOVERY_TIME,
	
	
	SKILL_ETC_RA_RATIO_POS_DAMAGE,
	SKILL_ETC_RA_RATIO_PHY_DAMAGE,

	
	
	SKILL_AFFDATA_TYPE_PHYDAMAGE_RATE_TOTAL,

	
	
	SKILL_AFFDATA_TYPE_GET_EXP_GENEXP_GENCAPABILITY_RATE_INC,

	
	SKILL_AFFDATA_TYPE_FINAL_ADD_PHY_DMG_RATE,						
	SKILL_AFFDATA_TYPE_FINAL_ADD_MYSTERY_POS_DMG_RATE,				
	SKILL_AFFDATA_TYPE_FINAL_ADD_MYSTERY_NEG_DMG_RATE,				

	SKILL_AFFDATA_TYPE_DEC_COOLTIME_RATE,							
	SKILL_AFFDATA_TYPE_FINAL_ADD_DEFENCE_RATE,						

	
	SKILL_AFFDATA_TYPE_HIDEALL,										
	SKILL_AFFDATA_TYPE_FINDHIDE,									
	SKILL_AFFDATA_TYPE_SHORTENING_SIGHT,							

	
	SKILL_MSG_TYPE_RA_REC_RATE,								
	SKILL_MSG_TYPE_SA_REC_RATE,								

	SKILL_AFFDATA_TYPE_CRITICAL_DMG_RATE,						
	SKILL_AFFDATA_TYPE_CRITICAL_DMG,								

	NUM_SKILL_TYPE,
};






enum
{
	SKILL_TYPE_COMMON=	1,
	SKILL_TYPE_ATTACK,
	SKILL_TYPE_AFFECT,

	NUM_SKILL_INFOTYPE,
};


enum
{
	ATTACK_TARGET_PC =1,			
	ATTACK_TARGET_ENTITY_PC,		
	ATTACK_TARGET_MONSTER,			
	ATTACK_TARGET_ENTITY,			

	NUM_SKILL_TARGETTYPE,
};


enum
{
	ATTACK_TARGET_INDEX_TARGETPC	=1,	
	ATTACK_TARGET_INDEX_PC,				
	ATTACK_TARGET_INDEX_MON_TARGETPC,	
	ATTACK_TARGET_INDEX_IDXONSERVER,	

	
	ATTACK_TARGET_INDEX_MONSTER,

	NUM_SKILL_TARGETINDEX	,
};

enum
{
	SKILL_MSG_TYPE_ONLYSEND=1,
	SKILL_MSG_TYPE_MAKESEND,
	SKILL_MSG_TYPE_ATTACK_LENGTH,
	SKILL_MSG_TYPE_RANGE_TYPE_TARGET,
	SKILL_MSG_TYPE_RANGE,
	SKILL_MSG_TYPE_PASSIVE,
	SKILL_MSG_TYPE_DAMAGE_TARGET_100_ELSE_LITE_DAMAGE,

	
	SKILL_MSG_TYPE_RANGE_OF_SELF,


	NUM_SKILL_MESSAGE_TYPE,
};


enum
{
	SKILL_SUB_ACTION_MORE_AFFECT=1,
	SKILL_SUB_ACTION_DELETEAFFECT,
	SKILL_SUB_ACTION_ADDMONSTER_CONTRIBUTIONPOINT,
	SKILL_SUB_ACTION_ATTRACT_ATTENTION,

	NUM_SKILL_SUB_ACTION,
};

enum
{
	CHECK_SKILL_MON_ALIVE_CHECK=1,
	CHECK_SKILL_TO_USE_MY,
	CHECK_TARGETPC_ALIVE,
	CHECK_TARGETALL_ALIVE,
	CHECK_BREEDRATE,
	CHECK_ACTIVEAFFECT,
	CHECK_PC,
	CHECK_GTH_EV_CHAR_DIE,
	CHECK_CRITICAL_RATE,
	CHECK_NOT_USE_MYSELF,
	
	CHECK_ONLY_USE_MYSELF,

	NUM_SKILL_CHECK,
};

enum
{
	SKILL_CLASS_PC=1,
	SKILL_CLASS_MON,

	NUM_OF_SKILL_CLASS,
};


enum
{
	SKILL_UNIQUE_TYPE_BUFF=1,
	SKILL_UNIQUE_TYPE_NOT_USE_ALIVE_PLAYER,
	SKILL_UNIQUE_TYPE_NOT_USE_NULL_TARGET_PLAYER,
	SKILL_UNIQUE_TYPE_ONLY_USE_SELF,
	SKILL_UNIQUE_TYPE_ONLY_USE_OTHER,
	SKILL_UNIQUE_TYPE_ONLY_USE_PC,


	NUM_SKILL_UNIQUE_TYPE,
};







enum			
{
	SKILL_ATK_TYPE_NORMAL =1,
	SKILL_ATK_TYPE_MAGIC,
	SKILL_ATK_TYPE_EFFECT,
	SKILL_ATK_TYPE_NONE,

	NUM_SKILL_ATTACKTYPE,
};


enum
{
	SKILL_HITPOWER_HITPRO=1,
	SKILL_HITPOWER_NORMAL,
	SKILL_HITPOWER_VALUE,

	NUM_SKILL_HITPOWER,
};


enum
{
	SKILL_HITRATE_VALUE=1,
	SKILL_HITRATE_PANELTY,

	NUM_SKILL_HITRATE,
};

enum
{
	SKILL_PHYMINDMG_NORMAL=1,
	SKILL_PHYMINDMG_REDUCE,
	SKILL_PHYMINDMG_HITPRO,
	SKILL_PHYMINDMG_VALUE,
	SKILL_PHYMINDMG_MON,
	SKILL_PC_PHYMINDMG_ADD_VALUE,
	SKILL_PHYMINDMG_MAGIC,
	SKILL_PHYMINDMG_NORMAL_PHYBONUS,
	SKILL_PHYMINDMG_MAGIC_PHYBONUS,

	NUM_SKILL_PHYMINDMG,
};


enum
{		
	SKILL_PHYMAXDMG_NORMAL =1,
	
	SKILL_PHYMAXDMG_REDUCE,
	SKILL_PHYMAXDMG_HITPRO,
	SKILL_PHYMAXDMG_VALUE,
	SKILL_PHYMAXDMG_MON,
	SKILL_PC_PHYMAXDMG_ADD_VALUE,
	SKILL_PHYMAXDMG_MAGIC,
	SKILL_PHYMAXDMG_NORMAL_PHYBONUS,
	SKILL_PHYMAXDMG_MAGIC_PHYBONUS,

	NUM_SKILL_PHYMAXDMG,
};


enum
{
	SKILL_POSMINDMG_MON_MAGIC =1,
	SKILL_PC_POSMINDMG_ADD_VALUE,

	
	SKILL_POSMINDMG_MAGIC,
	SKILL_POSMINDMG_MAGIC_PHYBONUS,

	NUM_SKILL_POSMINDMG,
};


enum
{
	SKILL_POSMAXDMG_MON_MAGIC =1,
	SKILL_PC_POSMAXDMG_ADD_VALUE,

	
	SKILL_POSMAXDMG_MAGIC,
	SKILL_POSMAXDMG_MAGIC_PHYBONUS,

	NUM_SKILL_POSMAXDMG,
};



enum
{
	NUM_SKILL_NEGMINDMG,
};


enum
{
	NUM_SKILL_NEGMAXDMG,
};


enum
{
	SKILL_CRITICALRATE_VALUE=1,
	SKILL_CRITICALRATE_CALCRITICALRATE,

	NUM_SKILL_CRITICALRATE,
};



enum
{
	SKILL_CRITICALDAMAGE_VALUE =1,

	NUM_SKILL_CRITICALDAMAGE,
};


enum
{
	ATTACK_DELAY_PC=1,			
	ATTACK_DELAY_MONSTER,			
	ATTACK_DELAY_VALUE,			

	NUM_SKILL_DELAYTIME,
};



enum
{
	SKILL_REPEATNUM_VALUE =1,
	SKILL_REPEATNUM_CAL, 

	NUM_SKILL_REPEATNUM,
};


enum
{
	SKILL_REPEATDELAYTIME_VALUE=1, 

	NUM_SKILL_REPEATDELAYTIME,	
};



enum
{
	SKILL_ATK_ETC_INFO_NONE	=	1,
	SKILL_ATK_ETC_INFO_RASTEAL,
	SKILL_ATK_ETC_INFO_STUN	,			
	SKILL_ATK_ETC_INFO_SHOCK	,			
	SKILL_ATK_ETC_INFO_FREEZE	,			
	SKILL_ATK_ECT_INFO_PUSHED	,			
	SKILL_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER,		
	SKILL_ATK_ETC_MULTIDAMAGE_DELAY,			
	SKILL_ATK_ETC_INFO_HIDE,				

	SKILL_ATK_ETC_MULTIDAMAGE_DELAY_WEAPON_TYPE_MAGIC_BOW,  
	SKILL_ATK_ETC_INFO_STUNPRO,
	SKILL_ATK_ETC_INFO_STUNTIME,

	SKILL_ATK_ETC_MULTIDAMAGE_DELAY_INC,
	SKILL_ATK_DAMAGE_TARGET_100_ELSE_LITE_DAMAGE,
	
	SKILL_ATK_ETC_RA_RATIO_POS_DAMAGE,
	SKILL_ATK_ETC_RA_RATIO_PHY_DAMAGE,

	NUM_SKILL_ETCINFO,
};






enum
{
	SKILL_AFF_NONE =1,
	SKILL_AFF_ATTACK,
	SKILL_AFF_DEFENSE,
	SKILL_AFF_ASSIST,
	SKILL_AFF_SPECIAL,
	SKILL_AFF_TOGGLE,
	SKILL_AFF_CURSE,

	NUM_SKILL_AFF_TYPE,
};



enum
{
	SKILL_AFF_TYPE_NORMAL=1,						
	SKILL_AFF_TYPE_POSITIVE,
	SKILL_AFF_TYPE_NEGATIVE,
	SKILL_AFF_TYPE_NONE,

	NUM_SKILL_AFF_AFFECTYPE,
};



enum
{
	SKILL_AFF_STARTTIME_GLOBALTIME=1,
	SKILL_AFF_STARTTIME_PLUSVALUE,

	NUM_SKILL_AFF_STARTTIME,
};



enum
{
	AFFECT_KEEP_TIME_DECDEFENSETIME=1,
	AFFECT_KEEP_TIME_VALUETIME,
	AFFECT_KEEP_TIME_STARTTIME,

	NUM_SKILL_AFF_KEEPTIME
};


enum
{
	SKILL_AFF_POSMINDMG_AFFECTSKILLLEVEL =1,
	SKILL_AFF_POSMINDMG_VALUE,

	NUM_SKILL_AFF_POSMINDMG
};


enum
{
	SKILL_AFF_HITRATE_VALUE=1,

	NUM_SKILL_AFF_HITRATE
};


enum
{
	SKILL_AFF_SKILLTABLEIDX_VALUE=1,
	NUM_SKILL_AFF_SKILLTABLEIDX
};


enum
{
	SKILL_AFF_DELAYTIME_VALUE=1,

	NUM_SKILL_AFF_DELAYTIME
};








enum
{
	AFFDATA_TYPE_NONE				=	0,

	
	AFFDATA_TYPE_PHYDAMAGE,
	AFFDATA_TYPE_PHYDAMAGE_RATE,
	AFFDATA_TYPE_MAGDAMAGE,
	AFFDATA_TYPE_MAGDAMAGE_RATE,
	AFFDATA_TYPE_POSITIVEDAMAGE,
	AFFDATA_TYPE_NEGATIVEDAMAGE,
	
	AFFDATA_TYPE_CRITICALRATE,
	
	AFFDATA_TYPE_ATTACKSPEEDRATE_DEC,
	AFFDATA_TYPE_ATTACKSPEED,
	AFFDATA_TYPE_HITPOWER,
	AFFDATA_TYPE_HITPOWRE_RATE,

	
	AFFDATA_TYPE_DEFENSE,

	AFFDATA_TYPE_DEFENSE_RATE_INC, 
	AFFDATA_TYPE_DEFENSE_RATE_DEC, 


	AFFDATA_TYPE_AVOIDPOWER,
	AFFDATA_TYPE_AVOIDPOWER_RATE,
	AFFDATA_TYPE_AVOID_RATE,
	
	


	AFFDATA_TYPE_MAXRA_INC, 
	AFFDATA_TYPE_MAXRA_DEC, 

	AFFDATA_TYPE_MAXRA_RATE,
	AFFDATA_TYPE_MAXSA,
	AFFDATA_TYPE_MAXSA_RATE,
	AFFDATA_TYPE_VELOCITY,

	

	AFFDATA_TYPE_VELOCITY_RATE_DEC,
	AFFDATA_TYPE_VELOCITY_RATE_INC,

	AFFDATA_TYPE_CASTINGTIME_RATE,

	AFFDATA_TYPE_REFLECTDAMAGE,
	AFFDATA_TYPE_REFLECTDAMAGE_RATE,

	AFFDATA_TYPE_INCRARECOVERY,
	AFFDATA_TYPE_INCSARECOVERY,

	AFFDATA_TYPE_DECREASESAUSE,
	AFFDATA_TYPE_DECREASERAUSE,

	AFFDATA_TYPE_DECREASERA,
	AFFDATA_TYPE_DECREASESA,

	AFFDATA_TYPE_DEFENCERATE,
	AFFDATA_TYPE_MANASHIELD,

	
	AFFDATA_TYPE_ONCE_ATTACK_RACOST,
	AFFDATA_TYPE_ONCE_ATTACK_SACOST,

	
	AFFDATA_TYPE_SLEEP,				
	AFFDATA_TYPE_SCANED,			
	AFFDATA_TYPE_PARALYSIS,			
	AFFDATA_TYPE_STOPPED,			
	AFFDATA_TYPE_CONFUSION,			

	AFFDATA_TYPE_AVOID_DEC ,
	AFFDATA_TYPE_AVOID_INC ,

};


enum
{
	AFF_DATA_TYPE_NONE=0,

	AFF_DATA_TYPE_PHYDAMAGE,
	AFF_DATA_TYPE_PHYDAMAGE_RATE,

	AFF_DATA_TYPE_MAGDAMAGE,
	AFF_DATA_TYPE_MAGDAMAGE_RATE,

	AFF_DATA_TYPE_POSITIVE_DAMAGE,
	AFF_DATA_TYPE_NEGATIVE_DAMAGE,

	AFF_DATA_TYPE_CRITICAL_RATE,

	AFF_DATA_TYPE_ATTACK_SPEED,
	AFF_DATA_TYPE_ATTACK_SPEED_RATE,
	
	AFF_DATA_TYPE_HIT_POWER,
	AFF_DATA_TYPE_HIT_POWRE_RATE,

	AFF_DATA_TYPE_DEFENSE,
	AFF_DATA_TYPE_DEFENSE_RATE, 

	AFF_DATA_TYPE_AVOID_RATE,
	AFF_DATA_TYPE_AVOID_POWER_RATE,
	
	AFF_DATA_TYPE_MAX_RA,
	AFF_DATA_TYPE_MAX_RA_RATE,

	AFF_DATA_TYPE_MAX_SA,
	AFF_DATA_TYPE_MAX_SA_RATE,

	AFF_DATA_TYPE_VELOCITY,
	AFF_DATA_TYPE_VELOCITY_RATE,

	AFF_DATA_TYPE_CASTING_TIME_RATE,

	AFF_DATA_TYPE_REFLECT_DAMAGE,
	AFF_DATA_TYPE_REFLECT_DAMAGE_RATE,

	AFF_DATA_TYPE_RA_RECOVERY,
	AFF_DATA_TYPE_SA_RECOVERY,

	AFF_DATA_TYPE_DECREASE_SA_USE,
	AFF_DATA_TYPE_DECREASE_RA_USE,

	AFF_DATA_TYPE_DECREASE_RA,
	AFF_DATA_TYPE_DECREASE_SA,

	AFF_DATA_TYPE_MANASHIELD,

	AFF_DATA_TYPE_ONCE_ATTACK_RACOST,
	AFF_DATA_TYPE_ONCE_ATTACK_SACOST,

	AFF_DATA_TYPE_SLEEP,		
	AFF_DATA_TYPE_SCANED,		
	AFF_DATA_TYPE_PARALYSIS,
	AFF_DATA_TYPE_STOPPED,
	AFF_DATA_TYPE_CONFUSION,

	AFF_DATA_TYPE_OV_DEFENCE_RATE,

	AFF_DATA_TYPE_AVOID,

	AFF_DATA_TYPE_CASTINGTIME_RATE,
	
	
	AFF_DATA_TYPE_RA_RECOVERY_TIME,
	AFF_DATA_TYPE_SA_RECOVERY_TIME,
	
	
	
	AFF_DATA_TYPE_PHYDAMAGE_RATE_TOTAL,

	
	
	AFF_DATA_TYPE_GET_EXP_GENEXP_GENCAPABILITY_RATE_INC,
	
	
	AFF_DATA_TYPE_FINAL_ADD_PHY_DMG_RATE,						
	AFF_DATA_TYPE_FINAL_ADD_MYSTERY_POS_DMG_RATE,				
	AFF_DATA_TYPE_FINAL_ADD_MYSTERY_NEG_DMG_RATE,				
	
	AFF_DATA_TYPE_DEC_COOLTIME_RATE,						
	AFF_DATA_TYPE_FINAL_ADD_DEFENCE_RATE,					

	AFF_DATA_TYPE_HIDEALL,									
	AFF_DATA_TYPE_FINDHIDE,									
	AFF_DATA_TYPE_SHORTENING_SIGHT,							

	
	AFF_DATA_TYPE_DELETE_BUFF_SKILL,						

	
	AFF_DATA_TYPE_CRITICAL_DMG_RATE,						
	AFF_DATA_TYPE_CRITICAL_DMG,								

	NUM_SKILL_AFF_DATA_COMMON,
};







enum
{
	SKILL_AFF_SPE_LEVEL_VALUE=1,
	NUM_SKILL_AFF_SPE_TYPE
};


enum
{
	SKILL_AFF_SPE_SPECIALIDX_TABLEIDX =1,
	NUM_SKILL_AFF_SPE_SPECIALIDX
};
	
			

enum
{
	SKILL_AFF_SPEC_IS_TRANSFORM=1,
	SKILL_AFF_SPEC_TRANSFORM_ENTITY_IDX,		
	SKILL_AFF_SPEC_TRANSFORM_TYPE,
	SKILL_AFF_SPEC_VELOCITY,
	SKILL_AFF_SPEC_MAXRA,
	SKILL_AFF_SPEC_PHYMINDMG,
	SKILL_AFF_SPEC_PHYMAXDMG,
	SKILL_AFF_SPEC_HIT_POWER,
	SKILL_AFF_SPEC_AVOID_POWER,
	SKILL_AFF_SPEC_ATTACK_SPPED_RATE,

	NUM_SKILL_AFF_SPE_SPECIALDATA
};


enum
{
	SKILL_MES_TYPE=1,

	NUM_OF_SKILL_MES_TYPE,
};

enum
{
	SKILL_MES_FROMIDX=1,

	NUM_OF_SKILL_MES_FROMIDX,
};

enum
{
	SKILL_MES_FROMTYPE=1,

	NUM_OF_SKILL_MES_FROMTYPE,
};

enum
{
	SKILL_MES_NAME=1,

	NUM_OF_SKILL_MES_NAME,
};

enum
{
	SKILL_MES_TOTYPE=1,

	NUM_OF_SKILL_MES_TOTYPE,
};

enum
{
	SKILL_MES_TOIDX=1,

	NUM_OF_SKILL_MES_TOIDX,
};

enum
{
	SKILL_MES_WORLDIDX=1,

	NUM_OF_SKILL_MES_WORLDIDX,
};


enum
{
	SKILL_MES_SENDTIME=1,

	NUM_OF_SKILL_MES_SENDTIME,
};


enum
{
	SKILL_MES_RECEIVETIME=1,

	NUM_OF_SKILL_MES_RECEIVETIME,
};

enum
{
	SKILL_MES_DATA_MSG_ETC_RESURRECTION=1,
	SKILL_MES_DATA_RAREC,
	SKILL_MES_DATA_SAREC,
	SKILL_MES_DATA_EXRECRATE,
	
	SKILL_MES_DATA_RA_REC_RATE,
	SKILL_MES_DATA_SA_REC_RATE,

	NUM_OF_SKILL_MES_DATA,
};



enum
{
	SKILL_MES_TYPE_ETC_TYPE=1,
	SKILL_MES_TYPE_RA,
	SKILL_MES_TYPE_SA,
	SKILL_MES_TYPE_EXP,

	NUM_OF_SKILL_MES_DATA_TYPE,
};






enum
{
	SKILL_PARTY_CHECK_NORMAL=1,
	SKILL_PARTY_CHECK_CRITICAL_RATE,

	NUM_OF_SKILL_PARTY_CHECK,
};

enum
{
	SKILL_PARTY_MODE=1,
	NUM_OF_SKILL_PARTY_MODE,
};






enum
{
	SKILL_RA_TARGETIDX=1,
	SKILL_RA_ENTITY_TYPE_ENTITYPC,
	SKILL_RA_TARGETTYPE_ENTITY_PC,
	SKILL_RA_LAYHAND,
	SKILL_RA_PC,

	NUM_OF_SKILL_RA,
};



#endif