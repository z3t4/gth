


enum
{
	Opt_Add_PhyDamange		=	0,	
	Opt_Add_Min_PhyDamage,			
	Opt_Add_Max_PhyDamage,
	Opt_Add_MagDamage,
	Opt_Add_Min_MagDamage,
	Opt_Add_Max_MagDamage,
	Opt_Add_CriticalPro,
	Opt_Mul_CriticalDamage,
	Opt_Dec_CastingTime,
	Opt_Add_AttackSpeed,			

	Opt_Add_HitPower,				
	Opt_Steal_RA,
	Opt_Steal_SA,
	Opt_Steal_RA_Point,
	Opt_Steal_SA_Point,
	Opt_Mul_Che_PhyDamage,
	Opt_Mul_Che_MagDamage,
	Opt_Mul_Mamul_PhyDamage,
	Opt_Mul_Mamul_MagDamage,
	Opt_Mul_Yogui_PhyDamage,		

	Opt_Mul_Yogui_MagDamage,		
	Opt_Mul_Yoguisun_PhyDamage,
	Opt_Mul_Yoguisun_MagDamage,
	Opt_Mul_Younghon_PhyDamage,
	Opt_Mul_Younghon_MagDamage,
	Opt_Armor_Add_DefensePower,
	Opt_Upper_Add_PositiveRes,
	Opt_Upper_Add_NegativeRes,
	Opt_Gloves_Add_HitPower,
	Opt_Boots_Add_AvoidPower,		

	Opt_Boots_Add_Velocity,			
	Opt_Ring_Mul_PhyDamage,
	Opt_Ring_Mul_MagDamage,
	Opt_Ring_Add_CriticalPro,
	Opt_Ring_Mul_CriticalDamage,
	Opt_Ring_Steal_RA_Point,
	Opt_Ring_Steal_SA_Point,
	Opt_Ring_Steal_RA,
	Opt_Ring_Steal_SA,
	Opt_Necklace_Add_MaxRA,			
	
	Opt_Necklace_Add_MaxSA,			
	Opt_Necklace_Add_AvoidRate,
	Opt_Necklace_Add_HitRate,
	Opt_Necklace_Add_PositiveRes,
	Opt_Necklace_Add_NegativeRes,
	Opt_Necklace_Dec_StunTime,
	Opt_Necklace_Dec_CursePro,
	Opt_Necklace_Trans_DamageToRA,
	Opt_Necklace_Trans_DamageToSA,
	Opt_Necklace_Mul_Recovery_RA_Normal,	

	Opt_Necklace_Mul_Recovery_SA_Normal,		
	Opt_Necklace_Mul_Recovery_RA_Rest,
	Opt_Necklace_Mul_Recovery_SA_Rest,
	Opt_Necklace_Mul_Recovery_EpackRA,
	Opt_Necklace_Mul_Recovery_EpackSA,
	Opt_Bracelet_Add_DefensePower,
	Opt_Bracelet_Add_Force,
	Opt_Bracelet_Add_Agility,
	Opt_Bracelet_Add_Soulpower,
	Opt_Bracelet_Add_Vitality,

	Opt_Bracelet_Dec_CastingTime,				
	Opt_Bracelet_Dec_CoolTime,
	Opt_Bracelet_Steal_RAPoint_MonsterDie,
	Opt_Bracelet_Steal_SAPoint_MonsterDie,
	Opt_Bracelet_Mul_LootExp,
	Opt_Bracelet_Mul_LootGenExp,
	Opt_Bracelet_Mul_LootNak,					

	Opt_Gloves_Add_LootExp,						
	Opt_Upper_Add_LootNak,						

};



enum
{
	OV_W_MAX_PHYDMG				=	0,		
	OV_W_MIN_PHYDMG,						
	

	OV_W_PHYDMG_RATE,						
	OV_W_PHYDMG,							

	OV_W_MAX_MAGDMG,						
	OV_W_MIN_MAGDMG,						
	OV_W_MAGDMG_RATE,						

	OV_W_MAGDMG,							

	OV_W_HITPOWER,							

	OV_W_ATKSPEED,							

	OV_CRITICAL_DMG,						

	OV_CRITICAL_DMG_RATE,

	OV_CRITICAL_PRO,						

	OV_RASTEAL,								
	OV_SASTEAL,								

	OV_D_DEFPOWER,							
	OV_POSRES,								
	OV_NEGRES,								
	OV_AVOIDPOWER,							

	OV_VELOCITY,							

	
	OV_FORCE,								
	OV_AGI,									
	OV_SOUL,								
	OV_VIT,									
	
	OV_MIN_PHYDMG_RATE,						
	OV_MAX_PHYDMG_RATE,						
	OV_MIN_MAGDMG_RATE,						
	OV_MAX_MAGDMG_RATE,						

	OV_HIT_RATE,							

	OV_CHE_PHY_DMG_RATE,					
	OV_MAMUL_PHY_DMG_RATE,					
	OV_YO_PHY_DMG_RATE,						
	OV_YOSUN_PHY_DMG_RATE,					
	OV_YOUNG_PHY_DMG_RATE,					
	OV_CHE_MAG_DMG_RATE,					
	OV_MAMUL_MAG_DMG_RATE,					
	OV_YO_MAG_DMG_RATE,						
	OV_YOSUN_MAG_DMG_RATE,					
	OV_YOUNG_MAG_DMG_RATE,					
	
	OV_MAX_RA,								
	OV_MAX_SA,								

	OV_AVOID_RATE,							
	

	OV_EPACK_RA_EFFICIENCY,					
	OV_EPACK_SA_EFFICIENCY,					

	OV_DEC_STUNDELAY_RATE,					
	OV_DEC_CURSESKILL_RATE,					

	OV_RA_STEAL_DIE_MONSTER,				
	OV_SA_STEAL_DIE_MONSTER,				

	OV_RA_STEAL_DAMAGED,					
	OV_SA_STEAL_DAMAGED,					

	OV_AUTORECOVERY_RA_ATTACK,				
	OV_AUTORECOVERY_SA_ATTACK,				

	OV_AUTORECOVERY_RA_IDLE,				
	OV_AUTORECOVERY_SA_IDLE,				

	OV_DEC_CASTINGTIME_RATE,				
	OV_DEC_COOLTIME_RATE,					
	OV_ADD_EXP_RATE,						
	OV_ADD_GENEXP_RATE,						
	OV_ADD_NAK_RATE,						

	
	
	OV_MIN_PHYDMG,							
	OV_MAX_PHYDMG,							
	OV_PHYDMG,								
	OV_RASTEAL_POINT,						
	OV_SASTEAL_POINT,						
	OV_DEFPOWER,							
	OV_MIN_MAGDMG,							
	OV_MAX_MAGDMG,							
	OV_MAGDMG,								

	OV_EXHAUST_SA_RATE,

	OV_SESTEAL,
	OV_SESTEAL_RATE,

	OV_DECRITICAL,							
	OV_HEAL_EFFICIENCY,						
	OV_SKILL_LENGTH_RATE,					

	SV_WARNING_ADD_DMG_RATE,				
	SV_SECEDE_ADD_AVOID_RATE,				

	OV_POLYMORPH_KEEPTIME,					
	OV_POLYMORPH_KEEPTIME_RATE,				

	OV_REFLECTDMG,							
	OV_REFLECTDMG_RATE,						

	OV_ATK_RACOST,
	OV_ATK_SACOST,
	
	OV_SKILL_DMG_RATE,						
	
	OV_PHYDMG_RATE,
	OV_MAGDMG_RATE,

	OV_DECSA_USE,
	OV_DEFENCE_RATE,

	OV_SKILL_MANASHIELD,
	
	OV_DECRA_USE,								

	OV_AUTORECOVERY_RA_TIME,					
	OV_AUTORECOVERY_SA_TIME,					

	
	OV_ADD_GENCAPABILITY_RATE,

	
	OV_SKILL_FINAL_ADD_PHY_DMG_RATE,						
	OV_SKILL_FINAL_ADD_MYSTERY_POS_DMG_RATE,				
	OV_SKILL_FINAL_ADD_MYSTERY_NEG_DMG_RATE,				

	OV_SKILL_FINAL_ADD_DEFENCE_RATE,						

	NUM_OPTION_VALUE
};





enum
{
	MONSTER_OPTION_RA_INC,			
	MONSTER_OPTION_SA_INC,			
	MONSTER_OPTION_RA_REC,			
	MONSTER_OPTION_SA_REC,			
	MONSTER_OPTION_MIN_ATT,			
	MONSTER_OPTION_MAX_ATT,			
	MONSTER_OPTION_MIN_MAG,			
	MONSTER_OPTION_MAX_MAG,			
	MONSTER_OPTION_ATT_INC,			
	MONSTER_OPTION_ASP_INC,			
	MONSTER_OPTION_DEX_INC,			
	MONSTER_OPTION_DEF_INC,			
	MONSTER_OPTION_ESC_INC,			
	MONSTER_OPTION_PREG_INC,		
	MONSTER_OPTION_NREG_INC,		
	MONSTER_OPTION_REG_INC,			
	MONSTER_OPTION_CRTDMG_INC,		
	MONSTER_OPTION_CRTRAT_INC,		
	MONSTER_OPTION_NOSTUN,			


	MONSTER_OPTION,					
};



enum
{
	PLAYER_OPTION_EXP_INC = MONSTER_OPTION,		
	PLAYER_OPTION_GENEXP_INC,					
	PLAYER_OPTION_GENSKILL_INC,					
	PLAYER_OPTION_NAC_INC,						
	PLAYER_OPTION_DROPRATE_INC,					

	PLAYER_OPTION,								
};



extern float				g_optValue[NUM_OPTION_VALUE];
extern itemTable_t			g_optItem;
extern itemTable_t			g_oriItem;

void OPTION_InitGlobal();
void OPTION_ApplyStatusOption( item_t *item, int status[] );
int OPTION_ApplyItemOption( item_t *item );
void OPTION_ApplyItem( item_t* item );
void OPTION_CheckOptionValue( float optValue[] );


int OPTION_GetItemType_MatchingAvailableOn( DWORD availableOn );
int OPTION_GetOptionStoneGrade( item_t *item );
float OPTION_GetOptionValue( item_t *item, int optionTableIdx, int optionGrade );
