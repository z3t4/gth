



#include "../global.h"





CGameTarget::CGameTarget()
{

}

CGameTarget::~CGameTarget()
{

}
int GTH_GetTargetPosition( playerCharacter_t *pc, vec3_t pos )
{
	playerCharacter_t *pTargetPC;
	monsterCharacter_t *pTargetMon;

	if( pc->targetIdx < 0 ) return( false );
	if( pc->targetType < 0 ) return( false );
	switch( pc->targetType )
	{
	case ENTITY_PC :
		pTargetPC = &g_pc[pc->targetIdx];
		vec_copy( pTargetPC->position, pos );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		pTargetMon = &g_monster[pc->targetIdx];
		vec_copy( pTargetMon->position, pos );
		break;
	}
	return( true );
}

int GTH_GetTargetPosition( monsterCharacter_t *pMonster, vec3_t pos )
{
	playerCharacter_t *pTargetPC;
	monsterCharacter_t *pTargetMon;

	if( pMonster->targetIdx < 0 ) return( false );
	if( pMonster->targetType < 0 ) return( false );
	switch( pMonster->targetType )
	{
	case ENTITY_PC :
		pTargetPC = &g_pc[pMonster->targetIdx];
		vec_copy( pTargetPC->position, pos );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		pTargetMon = &g_monster[pMonster->targetIdx];
		vec_copy( pTargetMon->position, pos );
		break;
	}
	return( true );
}


float GTH_GetDistanceFromTarget( playerCharacter_t *pc )
{
	float dist = -1.0f;
	playerCharacter_t *pTargetPC;
	monsterCharacter_t *pTargetMon;

	if( pc->targetIdx < 0 ) return( -1.0f );
	if( pc->targetType < 0 ) return( -1.0f );
	switch( pc->targetType )
	{
	case ENTITY_PC :
		pTargetPC = &g_pc[pc->targetIdx];
		dist = GTH_GetDistance2D( pc->position, pTargetPC->position );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		pTargetMon = &g_monster[pc->targetIdx];
		dist = GTH_GetDistance2D( pc->position, pTargetMon->position );		
		break;
	}
	
	if(dist < 0.0000f)
	{
		
	}
	return( dist );
}


float GTH_GetDistanceFromTarget( vec3_t pos, int targetIdx, int targetType )
{
	float dist = -1.0f;
	playerCharacter_t *pTargetPC;
	monsterCharacter_t *pTargetMon;

	if( targetIdx < 0 ) return( -1.0f );
	if( targetType < 0 ) return( -1.0f );

	switch( targetType )
	{
	case ENTITY_PC :
		pTargetPC = &g_pc[targetIdx];
		dist = GTH_GetDistance2D( pos, pTargetPC->position );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		pTargetMon = &g_monster[targetIdx];
		dist = GTH_GetDistance2D( pos, pTargetMon->position );		
		break;
	}

	if(dist < 0.0000f)
	{
		
	}

	return( dist );
}


char* GTH_GetTargetName( int entityType, int entityIdx )
{
	if( entityIdx < 0 || entityType < 0 ) return NULL;
	switch( entityType )
	{
	case ENTITY_PC :
		if( entityIdx >= MAX_PCS ) return NULL;
		if( !g_pc[entityIdx].active || !g_pc[entityIdx].ready ) return NULL;
		return( g_pc[entityIdx].name );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		if( entityIdx >= MAX_MONSTERS ) return NULL;
		if( !g_monster[entityIdx].active ) return NULL;
		return( g_monster[entityIdx].basic.name );
		break;
	}
	return NULL;
}



void AddTargetList( playerCharacter_t *pc, char *name )
{
	for( int i = 0; i < MAX_TARGET_PCLIST; i ++ )
	{
		if( !stricmp( name, pc->targetPCList[i] ) ) break;
	}
	
	if( i == MAX_TARGET_PCLIST )
	{
		for( i = 0; i < MAX_TARGET_PCLIST; i ++ )
		{
			if( pc->targetPCList[i][0] == 0 )
			{
				strcpy( pc->targetPCList[i], name );
				pc->targetPCListTime[i] = g_globalTime;
				break;
			}
			else if( g_globalTime - pc->targetPCListTime[i] > 6000 )
			{
				strcpy( pc->targetPCList[i], name );
				pc->targetPCListTime[i] = g_globalTime;
				break;
			}
		}
	}
	
	else
	{
		pc->targetPCListTime[i] = g_globalTime;
	}
}


int InTargetList( playerCharacter_t *pc, char* name )
{ 
	switch( pc->targetingType )
	{
		
	case TARGETING_ONLY_MONSTER :
		return false;
		break;
		
	case TARGETING_NORMAL : case TARGETING_SAFE :
		{
			for( int i = 0; i < MAX_TARGET_PCLIST; i ++ )
			{
				if( !stricmp( name, pc->targetPCList[i] ) )
				{
					if( g_globalTime - pc->targetPCListTime[i] < 60000 ) break;
				}
			}
			if( i == MAX_TARGET_PCLIST ) return false;
			return true;
		}
		break;
		
	case TARGETING_EXCEPT_PARTY :
		{
			if( pc->organizeServer < 0 ) return true;
			for( int i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
			{
				if( pc->member[i].serverIndex < 0 ) continue;
				if( !stricmp( name, pc->member[i].memberName ) ) return false;
			}
			return true;
		}
		break;
		
	case TARGETING_EXCEPT_GUILD :
		{
			return true;
		}
		break;
		
	case TARGETING_EVERY :
		{ 
			return true;
		}
		break;
	}

	return false;
}