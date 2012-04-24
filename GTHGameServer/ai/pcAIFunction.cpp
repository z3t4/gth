#include "../global.h"


int PC_GetAttackComboType( playerCharacter_t *pc )
{
	float dist;
	int itemIdx, itemTableIdx, weaponType, classIdx, comboLevel;

	if( pc->calAttackLength < DEFAULT_RANGE_ATTACK_LEGNTH )
	{
		pc->attackComboType = ATTACK_COMBO_TYPE_NOT;
		itemIdx = pc->equipment[EQUIP_WEAPON_1 + pc->curWeapon];
		
		if( itemIdx < 0 || pc->isTransform ) 
		{
			pc->attackComboType = ATTACK_COMBO_TYPE_NOT;
		}
		else
		{
			if( pc->level > 0 && pc->level <= 10 ) comboLevel = 0;
			else if( pc->level >= 11 && pc->level <= 30 ) comboLevel = 1;
			else comboLevel = 2;

			itemTableIdx = pc->item[itemIdx].itemTableIdx;
			classIdx = g_itemTable[itemTableIdx].classIdx;
			weaponType = g_itemClassTable[classIdx].weaponType;

			switch( weaponType )
			{
				
			case -1 :
				pc->attackComboType = ATTACK_COMBO_TYPE_NOT;
				break;
				
			case WEAPON_TYPE_DANGGER :
				pc->attackComboType = ATTACK_COMBO_TYPE_0 + comboLevel;
				break;
				
			case WEAPON_TYPE_ONEHAND_SWORD :
				pc->attackComboType = ATTACK_COMBO_TYPE_0 + comboLevel;
				break;
				
			case WEAPON_TYPE_TWOHAND_SWORD :
				pc->attackComboType = ATTACK_COMBO_TYPE_0 + comboLevel;
				break;
				
			case WEAPON_TYPE_MAGIC_BOW :
				pc->attackComboType = ATTACK_COMBO_TYPE_NOT;
				break;
				
			case WEAPON_TYPE_STAFF :
				pc->attackComboType = ATTACK_COMBO_TYPE_NOT;
				break;
				
			case WEAPON_TYPE_SPIRITOR :
				pc->attackComboType = ATTACK_COMBO_TYPE_0 + comboLevel;
				break;
				
			case WEAPON_TYPE_CLAW :
				pc->attackComboType = ATTACK_COMBO_TYPE_0 + comboLevel;
				break;
			}
		}
		
		if( pc->targetIdx < 0 || pc->targetType < 0 ) return( false );

		return( true );
	}
	else
	{
		pc->attackComboType = ATTACK_COMBO_TYPE_RANGE;
		if( pc->targetIdx < 0 || pc->targetType < 0 ) return( false );

		dist = GTH_GetDistanceFromTarget( pc );
		if( dist < 0.0000f ) return( false );

		if( (int)pc->curSA - pc->atkBowSACost < 0 || dist < DEFAULT_MELEE_ATTACK_LENGTH )
		{
			pc->attackComboType = ATTACK_COMBO_TYPE_NEAR_RANGE;
		}
		return( true );
	}

	return( false );

	

	

	
	
}

int PC_IsValidRangeAttack( vec3_t playerPos, vec3_t monsterPos, int worldIdx )
{
	vec3_t spos, dpos, sub, cpos, differ;
	int localWorldIdx;
	
	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );

	vec_copy( playerPos, spos );
	vec_copy( monsterPos, dpos );

	vec_sub( dpos, spos, sub );
	Normalize( sub );
	
	sub[0] *= ZONE_GRIDSCALE;
	sub[1] *= ZONE_GRIDSCALE;
	sub[2] *= ZONE_GRIDSCALE;

	vec_copy ( spos, cpos );
	vec_sub( dpos, cpos, differ );
	while( 1 )
	{
		if( vec_dot( cpos, differ ) < 0.0f ) break;
		
		if( !AI_CheckCollision( cpos, &g_cmap[localWorldIdx] ) ) return 0;

		vec_add( cpos, sub, cpos );
		vec_sub( dpos, cpos, differ );
	}
	return 1;
}


int PC_IsValidAttack( playerCharacter_t* pc, int entityType, int entityIdx )
{
	vec3_t spos, dpos;
	float dist;

	grid_t col;
	int localWorldIdx;

	if( entityType < 0 || entityIdx < 0 ) return( false );
	
	if( entityType == ENTITY_PC ) 
	{
		return( true );
	}
	else
	{
		vec_copy( pc->position, spos );
		vec_copy( g_monster[entityIdx].position, dpos );
	}

	localWorldIdx = GTH_GetLocalWorldIdx( pc->worldIdx );
	AI_GetGridPosition( pc->position, col, &g_cmap[localWorldIdx] );	
	if( !AI_CheckCollision( col[0], col[1], &g_cmap[localWorldIdx] ) ) return( false );

	dist = GTH_GetDistance2D( spos, dpos );
	
	if( dist > DEFAULT_RANGE_ATTACK_LEGNTH )
	{
		return( PC_IsValidRangeAttack( spos, dpos, pc->worldIdx ) );
	}

	return( true );
}

