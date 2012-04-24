#include "../global.h"

float				g_optValue[NUM_OPTION_VALUE];
itemTable_t			g_optItem;
itemTable_t			g_oriItem;

void OPTION_InitGlobal()
{
	memset( g_optValue, 0, sizeof( g_optValue ) );
}


int OPTION_GetItemType_MatchingAvailableOn( DWORD availableOn )
{
	if( availableOn & OPTION_AVAILABLE_ON_WEAPONS		) return OPTION_ITEMTYPE_WEAPONS;
	if( availableOn & OPTION_AVAILABLE_ON_DAGGER		) return OPTION_ITEMTYPE_DAGGER;
	if( availableOn & OPTION_AVAILABLE_ON_1HSWORD		) return OPTION_ITEMTYPE_1HSWORD;
	if( availableOn & OPTION_AVAILABLE_ON_2HSWORD		) return OPTION_ITEMTYPE_2HSWORD;
	if( availableOn & OPTION_AVAILABLE_ON_BOW			) return OPTION_ITEMTYPE_BOW;
	if( availableOn & OPTION_AVAILABLE_ON_STAFF			) return OPTION_ITEMTYPE_STAFF;
	if( availableOn & OPTION_AVAILABLE_ON_SPIRITOR		) return OPTION_ITEMTYPE_SPIRITOR;
	if( availableOn & OPTION_AVAILABLE_ON_CLAW			) return OPTION_ITEMTYPE_CLAW;
	if( availableOn & OPTION_AVAILABLE_ON_ARMORS		) return OPTION_ITEMTYPE_ARMORS;
	if( availableOn & OPTION_AVAILABLE_ON_UPPER			) return OPTION_ITEMTYPE_UPPER;
	if( availableOn & OPTION_AVAILABLE_ON_LOWER			) return OPTION_ITEMTYPE_LOWER;
	if( availableOn & OPTION_AVAILABLE_ON_AIRPROTECTOR	) return OPTION_ITEMTYPE_AIRPROTECTOR;
	if( availableOn & OPTION_AVAILABLE_ON_BODYPROTECTOR	) return OPTION_ITEMTYPE_BODYPROTECTOR;
	if( availableOn & OPTION_AVAILABLE_ON_GLOVES		) return OPTION_ITEMTYPE_GLOVES;
	if( availableOn & OPTION_AVAILABLE_ON_BOOTS			) return OPTION_ITEMTYPE_BOOTS;
	if( availableOn & OPTION_AVAILABLE_ON_RING			) return OPTION_ITEMTYPE_RING;
	if( availableOn & OPTION_AVAILABLE_ON_NECKLACE		) return OPTION_ITEMTYPE_NECKLACE;
	if( availableOn & OPTION_AVAILABLE_ON_BRACELET		) return OPTION_ITEMTYPE_BRACELET;
	if( availableOn & OPTION_AVAILABLE_ON_EARRING		) return OPTION_ITEMTYPE_EARRING;
	if( availableOn & OPTION_AVAILABLE_ON_EPACK			) return OPTION_ITEMTYPE_EPACK;

	return -1;
}

DWORD OPTION_GetItemAvailalbeOn( int itemTableIdx )
{
	int itemClassTableIdx = g_itemTable[itemTableIdx].classIdx;
	DWORD availableOn = 0;

	
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_WEAPON ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_WEAPONS;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_DANGGER ) availableOn |= OPTION_AVAILABLE_ON_DAGGER;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_ONEHAND_SWORD ) availableOn |= OPTION_AVAILABLE_ON_1HSWORD;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_TWOHAND_SWORD ) availableOn |= OPTION_AVAILABLE_ON_2HSWORD;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_MAGIC_BOW ) availableOn |= OPTION_AVAILABLE_ON_BOW;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_STAFF ) availableOn |= OPTION_AVAILABLE_ON_STAFF;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_SPIRITOR ) availableOn |= OPTION_AVAILABLE_ON_SPIRITOR;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_CLAW ) availableOn |= OPTION_AVAILABLE_ON_CLAW;
	}
	
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_UPPER ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_UPPER;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_LOWER ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_LOWER;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_AIR_PROTECTOR ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_AIRPROTECTOR;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_BODY_PROTECTOR ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_BODYPROTECTOR;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_GLOVES ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_GLOVES;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_BOOTS ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_BOOTS;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}

	
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_EPACK ) availableOn |= OPTION_AVAILABLE_ON_EPACK;

	
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_RING ) availableOn |= OPTION_AVAILABLE_ON_RING;
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_NECKLACE ) availableOn |= OPTION_AVAILABLE_ON_NECKLACE;
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_BRACELET ) availableOn |= OPTION_AVAILABLE_ON_BRACELET;
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_EARRING ) availableOn |= OPTION_AVAILABLE_ON_EARRING;

	return( availableOn );
	
}

int OPTION_GetOptionStoneGrade( item_t *item )
{
	if( item->reqLevel >= 0 && item->reqLevel <= 29 ) return 0;
	else if( item->reqLevel >= 30 && item->reqLevel <= 49 ) return 1;
	else if( item->reqLevel >= 50 && item->reqLevel <= 69 ) return 2;
	else if( item->reqLevel >= 70 ) return 3;
	
	return 0;
}

float OPTION_GetOptionValue( item_t *item, int optionTableIdx, int optionGrade )
{
	int		optionStoneGrade	=	OPTION_GetOptionStoneGrade( item );
	DWORD	itemAvailableOn;
	DWORD	optAvailableOn;
	int		itemType;
	
	
	if( optionGrade <= 0 ) return( 0 );

	itemAvailableOn		=	OPTION_GetItemAvailalbeOn( item->itemTableIdx );
	optAvailableOn		=	g_optionTable[optionTableIdx].optValue[optionStoneGrade].availableOn;
	itemType			=	OPTION_GetItemType_MatchingAvailableOn( itemAvailableOn & optAvailableOn );

	
	if( itemType < 0 ) return( 0 );
	if( g_optionTable[optionTableIdx].optValue[optionStoneGrade].availableOn == 0 ) return( 0 );

	return( g_optionTable[optionTableIdx].optValue[optionStoneGrade].value_item[itemType].value[optionGrade - 1] );
}

void OPTION_ApplyStatusOption( item_t *item, int status[] )
{
	int		i, statusIdx, optIdx, optGrade;
	float	optValue;
	
	for( i = 0; i < item->optionNumber; i ++ )
	{
		statusIdx		=	-1;
		optIdx			=	item->optionIdx[i];
		optGrade		=	item->optionValue[i]; 
		optValue		=	0.0f;
		
		if( optGrade > 10 ) optGrade = 10;
		optValue =	OPTION_GetOptionValue( item, optIdx, optGrade );
		
		switch( optIdx )
		{
			
		case Opt_Bracelet_Add_Force :
			statusIdx = 0;
			break;

			
		case Opt_Bracelet_Add_Soulpower :
			statusIdx = 1;
			break;

			
		case Opt_Bracelet_Add_Agility :
			statusIdx = 2;
			break;

			
		case Opt_Bracelet_Add_Vitality : 
			statusIdx = 3;
			break;
		}
		if( statusIdx >= 0 ) status[statusIdx] += (int)optValue;
	}
}

int OPTION_ApplyItemOption( item_t *item )
{
	int		i, optIdx, optGrade;
	float	optValue;
	
	for( i = 0; i < item->optionNumber; i ++ )
	{
		optIdx			=	item->optionIdx[i];
		optGrade		=	item->optionValue[i];
		optValue		=	0.0f;
				
		if( optGrade > 10 ) optGrade = 10;
		optValue =	OPTION_GetOptionValue( item, optIdx, optGrade );

		switch( optIdx )
		{
		case Opt_Add_PhyDamange				:
			optIdx = OV_W_PHYDMG;
			break;
		
		case Opt_Add_Min_PhyDamage			:
			optIdx = OV_W_MIN_PHYDMG;
			break;

		case Opt_Add_Max_PhyDamage			:
			optIdx = OV_W_MAX_PHYDMG;
			break;

		case Opt_Add_MagDamage				:
			optIdx = OV_W_MAGDMG;
			break;

		case Opt_Add_Min_MagDamage			:
			optIdx = OV_W_MIN_MAGDMG;
			break;

		case Opt_Add_Max_MagDamage			:
			optIdx = OV_W_MAX_MAGDMG;
			break;

		case Opt_Add_CriticalPro			:	case Opt_Ring_Add_CriticalPro		:
			optIdx = OV_CRITICAL_PRO;
			break;

			
			




			
		case Opt_Ring_Mul_CriticalDamage:
			optIdx = OV_CRITICAL_DMG_RATE;
			break;

		case Opt_Mul_CriticalDamage:
			optIdx = OV_CRITICAL_DMG;
			break;
			


		case Opt_Dec_CastingTime			:	case Opt_Bracelet_Dec_CastingTime	:
			optIdx = OV_DEC_CASTINGTIME_RATE;
			optValue = -optValue;
			break;

		case Opt_Add_AttackSpeed			:
			optIdx = OV_W_ATKSPEED;
			break;

		case Opt_Add_HitPower				:	case Opt_Gloves_Add_HitPower		:
			optIdx = OV_W_HITPOWER;
			break;

		case Opt_Steal_RA					:	case Opt_Ring_Steal_RA				:
			optIdx = OV_RASTEAL;
			break;

		case Opt_Steal_SA					:	case Opt_Ring_Steal_SA				:
			optIdx = OV_SASTEAL;
			break;

		case Opt_Steal_RA_Point				:	case Opt_Ring_Steal_RA_Point		:
			optIdx = OV_RASTEAL_POINT;
			break;

		case Opt_Steal_SA_Point				:	case Opt_Ring_Steal_SA_Point		:
			optIdx = OV_SASTEAL_POINT;
			break;

		case Opt_Mul_Che_PhyDamage			 :
			optIdx = OV_CHE_PHY_DMG_RATE;
			break;
		case Opt_Mul_Che_MagDamage			 :
			optIdx = OV_CHE_MAG_DMG_RATE;
			break;
		case Opt_Mul_Mamul_PhyDamage		 :
			optIdx = OV_MAMUL_PHY_DMG_RATE;
			break;
		case Opt_Mul_Mamul_MagDamage		 :
			optIdx = OV_MAMUL_MAG_DMG_RATE;
			break;
		case Opt_Mul_Yogui_PhyDamage		 :
			optIdx = OV_YO_PHY_DMG_RATE;
			break;
		case Opt_Mul_Yogui_MagDamage		 :
			optIdx = OV_YO_MAG_DMG_RATE;
			break;
		case Opt_Mul_Yoguisun_PhyDamage		 :
			optIdx = OV_YOSUN_PHY_DMG_RATE;
			break;
		case Opt_Mul_Yoguisun_MagDamage		 :
			optIdx = OV_YOSUN_MAG_DMG_RATE;
			break;
		case Opt_Mul_Younghon_PhyDamage		 :
			optIdx = OV_YOUNG_PHY_DMG_RATE;
			break;
		case Opt_Mul_Younghon_MagDamage		 :
			optIdx = OV_YOUNG_MAG_DMG_RATE;
			break;

		case Opt_Armor_Add_DefensePower		:	case Opt_Bracelet_Add_DefensePower	:
			optIdx = OV_D_DEFPOWER;
			break;

		case Opt_Upper_Add_PositiveRes		:	case Opt_Necklace_Add_PositiveRes	:
			optIdx = OV_POSRES;
			break;
		case Opt_Upper_Add_NegativeRes		:	case Opt_Necklace_Add_NegativeRes	:
			optIdx = OV_NEGRES;
			break;

		case Opt_Boots_Add_AvoidPower		:
			optIdx = OV_AVOIDPOWER;
			break;

		case Opt_Boots_Add_Velocity			:
			optIdx = OV_VELOCITY;
			break;

		case Opt_Ring_Mul_PhyDamage			:
			optIdx = OV_PHYDMG_RATE;
			break;
		case Opt_Ring_Mul_MagDamage			:
			optIdx = OV_MAGDMG_RATE;
			break;

		case Opt_Necklace_Add_MaxRA			:
			optIdx = OV_MAX_RA;
			break;
		case Opt_Necklace_Add_MaxSA			:
			optIdx = OV_MAX_SA;
			break;

		case Opt_Necklace_Add_AvoidRate		:
			optIdx = OV_HIT_RATE;
			break;
		case Opt_Necklace_Add_HitRate		:
			optIdx = OV_AVOID_RATE;
			break;

		case Opt_Necklace_Dec_StunTime		:
			optIdx = OV_DEC_STUNDELAY_RATE;
			break;

		case Opt_Necklace_Dec_CursePro		:
			optIdx = OV_DEC_CURSESKILL_RATE;
			break;

			
			






			

			
		case Opt_Necklace_Trans_DamageToRA	:
			optIdx = OV_RA_STEAL_DAMAGED;
			break;
		case Opt_Necklace_Trans_DamageToSA	:			
			optIdx = OV_SA_STEAL_DAMAGED;
			break;
			
			

		case Opt_Necklace_Mul_Recovery_RA_Normal	:
			optIdx = OV_AUTORECOVERY_RA_ATTACK;
			break;
		case Opt_Necklace_Mul_Recovery_SA_Normal	:
			optIdx = OV_AUTORECOVERY_SA_ATTACK;
			break;
		case Opt_Necklace_Mul_Recovery_RA_Rest	:
			optIdx = OV_AUTORECOVERY_RA_IDLE;
			break;
		case Opt_Necklace_Mul_Recovery_SA_Rest	:
			optIdx = OV_AUTORECOVERY_SA_IDLE;
			break;
		
		case Opt_Necklace_Mul_Recovery_EpackRA	:
			optIdx = OV_EPACK_RA_EFFICIENCY;
			break;
		case Opt_Necklace_Mul_Recovery_EpackSA	:
			optIdx = OV_EPACK_SA_EFFICIENCY;
			break;

		case Opt_Bracelet_Dec_CoolTime			:
			optIdx = OV_DEC_COOLTIME_RATE;
			optValue = -optValue;
			break;

		case Opt_Bracelet_Steal_RAPoint_MonsterDie	:
			optIdx = OV_RA_STEAL_DIE_MONSTER;
			break;
		case Opt_Bracelet_Steal_SAPoint_MonsterDie	:
			optIdx = OV_SA_STEAL_DIE_MONSTER;
			break;

			
		case Opt_Gloves_Add_LootExp:
		case Opt_Bracelet_Mul_LootExp:
			optIdx = OV_ADD_EXP_RATE;
			break;
			
		case Opt_Bracelet_Mul_LootGenExp		:
			optIdx = OV_ADD_GENEXP_RATE;
			break;
			
			
		case Opt_Upper_Add_LootNak :
		case Opt_Bracelet_Mul_LootNak :		
			optIdx = OV_ADD_NAK_RATE;
			break;		
			
			
		default :
			continue;
			break;
		}
		if( optIdx >= 0 ) g_optValue[optIdx] += optValue;
	}
	return( 1 );
}


void OPTION_ApplyItem( item_t* item )
{
	float backupOptValue[NUM_OPTION_VALUE];

	int idx = item->itemTableIdx;
	int classIdx = g_itemTable[idx].classIdx;

	memcpy( &g_oriItem, &g_itemTable[idx], sizeof( itemTable_t ) );
	memcpy( &g_optItem, &g_itemTable[idx], sizeof( itemTable_t ) );
	memcpy( &backupOptValue, &g_optValue, sizeof( g_optValue ) );

	OPTION_InitGlobal();
	OPTION_ApplyItemOption( item );
	
	
	
	g_optItem.atkPhyMinDmg += (int)g_optValue[OV_W_MIN_PHYDMG];
	g_optItem.atkPhyMaxDmg += (int)g_optValue[OV_W_MAX_PHYDMG];
	g_optItem.atkPhyMinDmg += (int)g_optValue[OV_W_PHYDMG];
	g_optItem.atkPhyMaxDmg += (int)g_optValue[OV_W_PHYDMG];

	g_optItem.atkMagMinDmg += (int)g_optValue[OV_W_MIN_MAGDMG];
	g_optItem.atkMagMaxDmg += (int)g_optValue[OV_W_MAX_MAGDMG];
	g_optItem.atkMagMinDmg += (int)g_optValue[OV_W_MAGDMG];
	g_optItem.atkMagMaxDmg += (int)g_optValue[OV_W_MAGDMG];

	if( g_itemClassTable[classIdx].attackAtt )
	{
		g_optItem.atkHitPower += (int)g_optValue[OV_W_HITPOWER];
	}
	else if( g_itemClassTable[classIdx].defenseAtt )
	{
		g_optItem.defHitPower += (int)g_optValue[OV_W_HITPOWER];
	}
	g_optItem.defVelocity += (int)g_optValue[OV_VELOCITY];
	
	g_optItem.defAvoidPower += (int)g_optValue[OV_AVOIDPOWER];

	g_optItem.atkDelay += (int)g_optValue[OV_W_ATKSPEED];

	
	g_optItem.defPower += (int)g_optValue[OV_D_DEFPOWER];

	if( g_optItem.atkPhyMinDmg > g_optItem.atkPhyMaxDmg )
	{
		g_optItem.atkPhyMinDmg = g_optItem.atkPhyMaxDmg;
	}
	if( g_optItem.atkMagMinDmg > g_optItem.atkMagMaxDmg )
	{
		g_optItem.atkMagMinDmg = g_optItem.atkMagMaxDmg;
	}

	memcpy( &g_optValue, &backupOptValue, sizeof( g_optValue ) );
}


void OPTION_CheckOptionValue( float optValue[] )
{
}
