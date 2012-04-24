#include "../global.h"


char sterMonsterAI[60][100]=
{
	" 0 : AI_SetTimer",
	" 1 : AI_HaveDamaged",
	" 2 : AI_SetTarget",
	" 3 : AI_MoveToAnywhere",
	" 4 : AI_SetAIEvent",
	" 5 : AI_IsActiveTarget", 
	" 6 : AI_RemoveTarget",
	" 7 : AI_IsWithinAttackLength",
	" 8 : AI_CanTraceTarget",
	" 9 : AI_MoveToTarget",
	" 10 : AI_SearchTarget",
	" 11 : AI_MoveToWayPoint",
	" 12 : AI_FleeToTarget", 
	" 13 : AI_SearchFellow",
	" 14 : AI_CheckAbilityRate",
	" 15 : AI_GetDistanceFromTarget",
	" 16 : AI_IsWithInWayPoint",
	" 17 : AI_RunFromTarget",
	" 18 : AI_SetMoveType",
	" 19 : AI_IsNearTarget", 
	" 20 : AI_ReceiveHelpMessage",
	" 21 : AI_HaveBoss ",
	" 22 : AI_MoveToBoss",
	" 23 : AI_SendHelpMessage",
	" 24 : AI_GetAttackerCount",
	" 25 : AI_IsAtkRngLongerThanValue",
	" 26 : AI_IsAtkRngShorterThanValue", 
	" 27 : AI_AddSkillUsePercent",
	" 28 : AI_RemoveSkillUsePercent ",
	" 29 : AI_UseSkill",
	" 30 : AI_HaveSelectedSkill",
	" 31 : AI_IsAttackerLevelHigherThanMe",
	" 32 : AI_IsPartyPlayer",
	" 33 : AI_IsHaveTargetTrace", 
	" 34 : AI_MoveToSpawnPos",	
	" 35 : AI_IsOverRangeFromSpawnPos ",
	" 36 : AI_GetDamage",
	" 37 : AI_MonsterSummon",
	" 38 : AI_IsUsedSkill",
	" 39 : AI_SetUsedSkill",
	" 40 : AI_IsOverDIstansAttackRangeRate", 
	" 41 : AI_IsHighVelocityThenMonster",	
	" 42 : AI_GetSkillElapsedTime ",
	" 43 : AI_SendHelpMsg",
	" 44 : AI_GetAIEvent",
	" 45 : AI_GetSummonElapsedTime",
	" 46 : AI_GetTimeSkill",
	" 47 : AI_SetSkillAnimationTime", 

	
	" 48 : NULL", 
	" 49 : NULL", 
	" 50 : NULL", 
	" 51 : AI_GetRandom", 
	" 52 : NULL", 
	" 53 : NULL", 
	" 54 : NULL", 
	" 55 : NULL", 
	" 56 : NULL", 
	" 57 : NULL", 
	" 58 : NULL", 
	" 59 : NULL", 
};





int AI_InterpretorFunction( monsterCharacter_t* monster )
{
	
	int param[5]={0,0,0,0,0};					

	int variable = false;						
	int paramAddr;								
	paramAddr = AI_GetCharData( monster );
	
	
	



		


	switch( paramAddr )
	{
	
	
	case 0:
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_SetTimer( monster, param[0] );
		break;
	
	
	case 1 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_HaveDamaged( monster );
		break;
	
	
	case 2 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_SetTarget( monster, param[0] );
		break;
	
	
	case 3 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_MoveToAnywhere( monster, param[0] );
		break;
	
	
	case 4 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_SetAIEvent( monster, param[0] );
		break;
	
	
	case 5 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_IsActiveTarget( monster );
		break;
	
	
	case 6 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_RemoveTarget( monster );
		break;
	
	
	case 7 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_IsWithinAttackLength( monster );
		break;
	
	
	case 8 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_CanTraceTarget( monster, param[0] );
		break;
	
	
	case 9 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_MoveToTarget( monster );
		break;
	
	
	case 10 :
		AI_InterpretorParameter( monster, param, 2 );
		variable = AI_SearchTarget( monster, param[0], param[1] );
		break;
	
	
	case 11 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_MoveToWayPoint( monster );
		break;
	
	
	case 12 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_FleeToTarget( monster, param[0] );
		break;
	
	
	case 13 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_SearchFellow( monster, param[0] );
		break;
	
	
	case 14 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_CheckAbilityRate( monster, param[0] );
		break;	

	
	
	case 15 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_GetDistanceFromTarget( monster );
		break;
	
	
	case 16 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_IsWithInWayPoint( monster );
		break;
	
	
	case 17 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_RunFromTarget( monster );
		break;
	
	
	case 18 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_SetMoveType( monster, param[0] );
		break;
	
	
	case 19 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_IsNearTarget( monster, param[0] );
		break;
	
	
	case 20 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_ReceiveHelpMessage( monster );
		break;
	
	
	case 21 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_HaveBoss( monster, param[0] );
		break;
	
	
	case 22 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_MoveToBoss( monster );
		break;
	
	
	case 23 :
		AI_InterpretorParameter( monster, param, 2 );
		variable = AI_SendHelpMessage( monster, param[0], param[1] );
		break;
	
	
	case 24 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_GetAttackerCount( monster );
		break;
	case 25 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_IsAtkRngLongerThanValue( monster, param[0] );
		break;
	case 26 :
		AI_InterpretorParameter( monster, param, 1 );
		variable = AI_IsAtkRngShorterThanValue( monster, param[0] );
		break;
	case 27 :
		AI_InterpretorParameter( monster, param, 2 );
		variable = AI_AddSkillUsePercent( monster, param[0], param[1] );
		break;
	case 28 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_RemoveSkillUsePercent( monster );
		break;
	case 29 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_UseSkill( monster );
		break;
	case 30 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_HaveSelectedSkill( monster );
		break;
	case 31 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_IsAttackerLevelHigherThanMe( monster );
		break;
	case 32 : 
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_IsPartyPlayer( monster );
		break;
		
		
	case 33 : 
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_IsHaveTargetTrace( monster );
		break;

		
	case 34 :
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_MoveToSpawnPos( monster );
		break;		

		
	case 35:
		AI_InterpretorParameter ( monster, param, 1);
		variable = AI_IsOverRangeFromSpawnPos( monster , param[0]);
		break;

		
	case 36:
		AI_InterpretorParameter( monster, param, 0 );
		variable = AI_GetDamage( monster );
		break;

		
	case 37:
		AI_InterpretorParameter( monster, param, 2 );
		variable = AI_MonsterSummon( monster, param[0], param[1] );
		break;

		
	case 38:
		AI_InterpretorParameter(monster, param, 1);
		variable = AI_IsUsedSkill( monster, param[0] );
		break;

		
	case 39:
		AI_InterpretorParameter(monster, param, 2);
		variable = AI_SetUsedSkill( monster, param[0], param[1] );
		break;

		
	case 40:
		AI_InterpretorParameter(monster, param, 1);
		variable = AI_IsOverDIstansAttackRangeRate (monster, param[0] );
		break;

		
	case 41:
		AI_InterpretorParameter(monster, param, 0);
		variable = AI_IsHighVelocityThenMonster(monster);
		break;

		
	case 42:
		AI_InterpretorParameter(monster, param, 1);
		variable = AI_GetSkillElapsedTime(monster, param[0]);
		break;

		
	case 43:
		AI_InterpretorParameter(monster, param, 3);
		variable = AI_SendHelpMsg(monster, param[0], param[1], param[2]);
		break;

		
	case 44:
		AI_InterpretorParameter(monster, param, 0);
		variable = AI_GetAIEvent(monster);
		break;

	case 45:
		AI_InterpretorParameter(monster, param, 0);
		variable = AI_GetSummonElapsedTime(monster);		
		break;

		
	case 46:
		AI_InterpretorParameter(monster, param, 3);
		variable = AI_GetTimeSkill(monster,param[0], param[1], param[2]);
		break;

		
	case 47 : 
		AI_InterpretorParameter(monster, param, 2);
		variable = AI_SetSkillAnimationTime(monster,param[0], param[1]);
		break;


		
	case 51 : 
		AI_InterpretorParameter(monster, param,0);
		variable = AI_GetRandom(monster);
		break;
	default:
		break;
		
	}	
	
	






	return( variable );
}


int AI_InterpretorDefine( monsterCharacter_t* monster, int paramAddr )
{
	int variable;								

	switch( paramAddr )
	{
	
	case 0 :
		variable = true;
		break;
	
	case 1 :
		variable = false;
		break;
	
	case 2 :
		variable = AI_MOVE;
		break;
	
	case 3 :
		variable = AI_ATTACK;
		break;
	
	case 4 :
		variable = AI_TRACE;
		break;
	
	case 5 :
		variable = AI_RUNAWAY;
		break;
	
	case 6 :
		variable = AI_DEF_VERYSLOW_TIMER;
		break;
	
	case 7 :
		variable = AI_DEF_SLOW_TIMER;
		break;
	
	case 8 :
		variable = AI_DEF_NORMAL_TIMER;
		break;
	
	case 9 :
		variable = AI_DEF_FAST_TIMER;
		break;
	
	case 10 :
		variable = AI_DEF_VERYFAST_TIMER;
		break;
	
	case 11 :
		variable = AI_DEF_DAMAGED_ENTITY;
		break;
	
	case 12 :
		variable = AI_DEF_NEAR_ENTITY;
		break;
	
	case 13 :
		variable = AI_DEF_RANDOM;
		break;
	
	case 14 :
		variable = AI_DEF_NEAR_FELLOW;
		break;
	
	case 15 :
		variable = AI_DEF_MY_RA;
		break;
	
	case 16 :
		variable = AI_DEF_ENABLE_WALK;
		break;
	
	case 17 :
		variable = AI_DEF_DISABLE_MOVE;
		break;
	
	case 18 :
		variable = AI_DEF_DISABLE_WALK;
		break;
	
	case 19 :
		variable = AI_DEF_ENTITY_PC;
		break;
	
	case 20 :
		variable = AI_DEF_ENTITY_MONSTER;
		break;
	
	case 21 :
		variable = AI_DEF_ENTITY_NPC;
		break;
	
	
	case 22 :
		variable = AI_DEF_MY_SA;
		break;

	
	
	case 23 :
		variable = AI_DEF_LESS_RA_ENTITY;
		break;

	
	case 24 :
		variable = AI_DEF_MORE_POINT_ENTITY; 
		break;

	}
	return( variable );
}






int AI_LoadScript( monsterCharacter_t* monster, char* filename )
{
	FILE *fp;									
	int n, l, type, var, i;						

	
	
	char name[MAX_SCRIPT_VAL_LEN];					


	fp = fopen( filename, "rb" );
	if( !fp ) return 0;

	
	fread( &n, sizeof( int ), 1, fp );
	for( i = 0; i < n; i ++ )
	{
		
		fread( &l, sizeof( int ), 1, fp );
		fread( name, l, 1, fp );
		
		fread( &type, sizeof( int ), 1, fp );
		
		fread( &var, sizeof( int ), 1, fp );
		monster->ai.aiVariable[ i ] = var;
	}

	
	fread( &n, sizeof( int ), 1, fp );
	for( i = 0; i < n; i ++ )
	{
		
		fread( &l, sizeof( int ), 1, fp );
		fread( name, l, 1, fp );
		
		fread( &var, sizeof( int ), 1, fp );
		monster->ai.aiProcedureAddr[ i ] = var;
	}

	
	fread( &n, sizeof( int ), 1, fp );
	monster->ai.aiData = (char*)malloc( n );
	fread( monster->ai.aiData, n, 1, fp );
	fclose( fp );
	monster->ai.aiAddr = 0;

	
	monster->ai.callProcedureAddr = 0;
	monster->ai.isCall = false;

	return 1;
}


char AI_GetCharData( monsterCharacter_t* monster )
{
	return( monster->ai.aiData [ monster->ai.aiAddr++ ] );
}


int AI_GetIntData( monsterCharacter_t* monster )
{
	int n;
	memcpy( &n, &monster->ai.aiData[ monster->ai.aiAddr ], sizeof( int ) );
	monster->ai.aiAddr += sizeof( int );
	return( n );
}


void AI_InterpretImmediately( monsterCharacter_t* monster )
{
	monster->ai.saveTime = 0;
}


int AI_Interpretor( monsterCharacter_t* monster )
{
	char inter;								

	if( monster->aliveState != MON_ALIVE_STATE_NORMAL ) return( 0 );

	if( monster->entityType == ENTITY_NPC )
	{
		NPC_CheckNoticePC( monster );
	}
	if( monster->npc.noticePcNumber > 0 ) return( 0 );

	
	if( monster->ai.isCall )
	{
		monster->ai.isCall = false;
	}
	
	if( g_globalTime - monster->ai.saveTime < monster->ai.thinkTime ) 
		return(0);

	
	monster->ai.callProcedureAddr = monster->ai.aiAddr;

	
	while( !monster->ai.isCall )
	{
		inter = AI_GetCharData( monster );
		
		if( inter == 'x' ) break;

		switch( inter )
		{
		
		case 'p':
			inter = AI_GetCharData( monster );
			break;
		
		case 'o':
			AI_InterpretorOperation( monster );
			break;
		
		case 'i':
			AI_InterpretorIF( monster );
			break;
		
		case 'c':
			AI_InterpretorCall( monster );
			break;
		
		case 'f':
			AI_InterpretorFunction( monster );
		}
	}

	if( monster->aliveState != MON_ALIVE_STATE_NORMAL )
		monster->ai.aiEvent = AI_DIE;

	
	
	if( !monster->ai.isCall ) 
	{
		
		monster->ai.saveTime = g_globalTime;
	}
	
	
	monster->ai.aiAddr = monster->ai.callProcedureAddr;


	return(1);
}



void AI_InterpretorCall( monsterCharacter_t* monster )
{	
	int paramAddr;								
	paramAddr = AI_GetCharData( monster );
	if( monster->ai.isCall ) return;

	monster->ai.callProcedureAddr = monster->ai.aiProcedureAddr[paramAddr];
	monster->ai.isCall = true;
}


void AI_InterpretorOperation( monsterCharacter_t* monster )
{
	char inter;									
	int paramAddr1, paramAddr2, operatorParam;	
	int variable;								

	
	inter = AI_GetCharData( monster );
	
	if( inter == 'v' )
	{
		
		paramAddr1 = AI_GetCharData( monster );
	}

	
	inter = AI_GetCharData( monster );
	operatorParam = inter;
	
	
	inter = AI_GetCharData( monster );
	
	if( inter == 'v' )
	{

		paramAddr2 = AI_GetCharData( monster );
		
		variable = monster->ai.aiVariable[paramAddr2];
	}
	
	if( inter == 'n' )
	{
		variable = AI_GetIntData( monster );
	}
	
	if( inter == 'f' )
	{
		
		variable = AI_InterpretorFunction( monster );
	}
	
	if( inter == 'd' )
	{
		paramAddr2 = AI_GetCharData( monster );
		variable = AI_InterpretorDefine( monster, paramAddr2 );
	}

	
	switch( operatorParam )
	{
	case 0:
		monster->ai.aiVariable[paramAddr1] += variable;
		break;
	case 1:
		monster->ai.aiVariable[paramAddr1] -= variable;
		break;
	case 2:
		monster->ai.aiVariable[paramAddr1] *= variable;
		break;
	case 3:
		monster->ai.aiVariable[paramAddr1] /= variable;
		break;
	case 4:
		monster->ai.aiVariable[paramAddr1] = variable;
		break;
	}
}



void AI_InterpretorParameter( monsterCharacter_t* monster, int param[], int n )
{
	char inter;									
	int variable, paramAddr;					
	int i;										
	
	for( i = 0; i < n; i ++ )
	{
		inter = AI_GetCharData( monster );
		
		if( inter == 'n' )
		{
			variable = AI_GetIntData( monster );
			param[ i ] = variable;
			continue;
		}
		
		if( inter == 'd' )
		{
			paramAddr = AI_GetCharData( monster );
			variable = AI_InterpretorDefine( monster, paramAddr );
			param[ i ]= variable;
			continue;
		}
		
		if( inter == 'v' )
		{
			paramAddr = AI_GetCharData( monster );
			variable = monster->ai.aiVariable[ paramAddr ];
			param[ i ] = variable;
			continue;
		}
		
		if( inter == 'f' )
		{
			paramAddr = AI_GetCharData( monster );
			variable = AI_InterpretorFunction( monster );
			param[ i ] = variable;
			continue;
		}
	}
}


int AI_InterpretorCase( monsterCharacter_t* monster )
{
	char inter;											
	int cases[5], casesNumber = 0;						

	
	int variable1, variable2, paramAddr, operatorAddr;
	
	int i;												

	while( !monster->ai.isCall )
	{
		
		inter = AI_GetCharData( monster );
		
		if( inter == 't' ) break;
		
		if( inter == 'a' ) inter = AI_GetCharData( monster );
		
		if( inter == 'v' )
		{
			paramAddr = AI_GetCharData( monster );
			variable1 = monster->ai.aiVariable[ paramAddr ];
		}
		
		if( inter == 'n' )
		{
			variable1 = AI_GetIntData( monster );
		}
		
		if( inter == 'd' )
		{
			paramAddr = AI_GetCharData( monster );
			variable1 = AI_InterpretorDefine( monster, paramAddr );
		}
		
		if( inter == 'f' )
		{
			variable1 = AI_InterpretorFunction( monster );
		}
		
		inter = AI_GetCharData( monster );
		operatorAddr = inter;

		
		inter = AI_GetCharData( monster );
		if( inter == 'v' )
		{
			paramAddr = AI_GetCharData( monster );
			variable2 = monster->ai.aiVariable[ paramAddr ];
		}
		if( inter == 'n' )
		{
			variable2 = AI_GetIntData( monster );
		}
		if( inter == 'd' )
		{
			paramAddr = AI_GetCharData( monster );
			variable2 = AI_InterpretorDefine( monster, paramAddr );
		}
		if( inter == 'f' )
		{
			variable2 = AI_InterpretorFunction( monster );
		}
		cases[ casesNumber ] = 0;
		switch( operatorAddr )
		{
		
		case 0:
			if( variable1 == variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 1:
			if( variable1 != variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 2:
			if( variable1 < variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 3:
			if( variable1 > variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 4:
			if( variable1 <= variable2 ) cases[ casesNumber ] = 1;
			break;
		
		case 5:
			if( variable1 >= variable2 ) cases[ casesNumber ] = 1;
			break;
		}
		casesNumber ++;
	}
	
	for( i = 0; i < casesNumber; i ++ ) if( !cases[ i ] ) return( 0 );
	return( 1 );
}


void AI_InterpretorThen( monsterCharacter_t* monster, int addrInfo[] )
{

	char inter;										

	
	monster->ai.aiAddr = addrInfo[0];

	while( !monster->ai.isCall )
	{
		inter = AI_GetCharData( monster );
		
		if( inter == 'e' || inter == 'q' ) 
		{
			monster->ai.aiAddr = addrInfo[ 2 ];
			break;
		}
		switch( inter )
		{
		
		case 'o':
			AI_InterpretorOperation( monster );
			break;
		
		case 'i':
			AI_InterpretorIF( monster );
			break;
		
		case 'c':
			
			AI_InterpretorCall( monster );
			break;
		
		case 'f':
			AI_InterpretorFunction( monster );
			break;
		}
	}
}


void AI_InterpretorElse( monsterCharacter_t* monster, int addrInfo[] )
{
	char inter;									
	
	
	monster->ai.aiAddr = addrInfo[1];

	while( !monster->ai.isCall )
	{
		inter = AI_GetCharData( monster );
		
		if( inter == 'q' ) 
		{
			monster->ai.aiAddr = addrInfo[ 2 ];
			break;
		}
		switch( inter )
		{
		
		case 'o':
			AI_InterpretorOperation( monster );
			break;
		
		case 'i':
			AI_InterpretorIF( monster );
			break;
		
		case 'c':
			
			AI_InterpretorCall( monster );
			break;
		
		case 'f':
			AI_InterpretorFunction( monster );
			break;
		}
	}
}


void AI_InterpretorIF( monsterCharacter_t* monster )
{
	char inter;									
	int addrInfo[3];							
	int cases;									

	

	
	addrInfo[0] = AI_GetIntData( monster );
	
	addrInfo[1] = AI_GetIntData( monster );
	
	addrInfo[2] = AI_GetIntData( monster );
	

	while( !monster->ai.isCall )
	{
		inter = AI_GetCharData( monster );
		if( inter == 'q' ) break;
		if( inter == 'a' ) 
		{
			cases = AI_InterpretorCase( monster );
			
			if( cases ) AI_InterpretorThen( monster, addrInfo );
			
			
			else if( addrInfo[1] ) AI_InterpretorElse( monster, addrInfo );
				else monster->ai.aiAddr = addrInfo[ 2 ];
		}
	}
}

