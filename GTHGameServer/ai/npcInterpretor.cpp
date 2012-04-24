#include "../global.h"

extern AIFUNC AI_NPC_FUNC[];




int AI_NPC_InterpretorFunction( playerCharacter_t* pc )
{
	int param[5];								
	int variable = false;						
	int paramAddr;								
	int argCount;

	paramAddr	= AI_NPC_GetCharData( pc );
	
	argCount	= AI_NPC_FUNC[paramAddr].argCount;
	if ( argCount > 0 )
		AI_NPC_InterpretorParameter( pc, param, argCount );

	variable	= AI_NPC_FUNC[paramAddr].pFunc( pc, param );
	return( variable );
}



int AI_NPC_InterpretorDefine( playerCharacter_t* pc, int paramAddr )
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
		variable = J_MENTALITY;
		break;
	
	case 3 :
		variable = J_WIND;
		break;
	
	case 4 :
		variable = J_WATER;
		break;
	
	case 5 :
		variable = J_EARTH;
		break;
	
	case 6 :
		variable = J_DEVOTIONAL_KNIGHT;
		break;
	
	case 7 :
		variable = J_ROMING_KNIGHT;
		break;
	
	case 8 :
		variable = J_PIECING_EYES;
		break;
	
	case 9 :
		variable = J_WINDY_ROMER;
		break;
	
	case 10 :
		variable = J_DESTROYER;
		break;
	
	case 11 :
		variable = J_GUARDIAN;
		break;
	
	case 12 :
		variable = J_PROPHECIER;
		break;
	
	case 13 :
		variable = J_TRANSFORMER;
		break;
	
	case 14 :
		variable = J_DEFENDER;
		break;
	
	case 15 :
		variable = J_STRANGER;
		break;
	}
	return( variable );
}



int AI_NPC_LoadScript( monsterCharacter_t* monster, char* filename )
{
	FILE *fp;									
	int n, l, type, var, i;						
	char name[256];								

	fp = fopen( filename, "rb" );
	if( !fp ) return 0;

	
	fread( &n, sizeof( int ), 1, fp );
	for( i = 0; i < n; i ++ )
	{
		
		fread( &l, sizeof( int ), 1, fp );
		fread( name, l, 1, fp );
		
		fread( &type, sizeof( int ), 1, fp );
		
		fread( &var, sizeof( int ), 1, fp );
		monster->npcAi.aiVariable[ i ] = var;
	}

	
	fread( &n, sizeof( int ), 1, fp );
	for( i = 0; i < n; i ++ )
	{
		
		fread( &l, sizeof( int ), 1, fp );
		fread( name, l, 1, fp );
		
		fread( &var, sizeof( int ), 1, fp );
		monster->npcAi.aiProcedureAddr[ i ] = var;
	}

	
	fread( &n, sizeof( int ), 1, fp );

	fread( monster->npcAi.aiData, n, 1, fp );
	fclose( fp );
	monster->npcAi.aiAddr = 0;

	
	monster->npcAi.callProcedureAddr = 0;
	monster->npcAi.isCall = false;

	return 1;
}


char AI_NPC_GetCharData( playerCharacter_t* pc )
{
	return( pc->npcAi.aiData [ pc->npcAi.aiAddr++ ] );
}


int AI_NPC_GetIntData( playerCharacter_t* pc )
{
	int n;
	memcpy( &n, &pc->npcAi.aiData[ pc->npcAi.aiAddr ], sizeof( int ) );
	pc->npcAi.aiAddr += sizeof( int );
	return( n );
}


int AI_NPC_Interpretor( playerCharacter_t* pc )
{
	char inter;								

	if( pc->npcIdx == -1 ) return( false );
	if( pc->npcAi.aiEvent == AI_NPC_WAIT ) return( false );

	
	if( pc->npcAi.isCall )
	{
		pc->npcAi.isCall = false;
	}
	
	pc->npcAi.callProcedureAddr = pc->npcAi.aiAddr;

	
	while( !pc->npcAi.isCall )
	{
		inter = AI_NPC_GetCharData( pc );
		
		if( inter == 'x' ) break;

		switch( inter )
		{
		
		case 'p':
			inter = AI_NPC_GetCharData( pc );
			break;
		
		case 'o':
			AI_NPC_InterpretorOperation( pc );
			break;
		
		case 'i':
			AI_NPC_InterpretorIF( pc );
			break;
		
		case 'c':
			AI_NPC_InterpretorCall( pc );
			break;
		
		case 'f':
			AI_NPC_InterpretorFunction( pc );
		}
	}
	
	pc->npcAi.aiAddr = pc->npcAi.callProcedureAddr;

	return(1);
}



void AI_NPC_InterpretorCall( playerCharacter_t* pc )
{	
	int paramAddr;								
	paramAddr = AI_NPC_GetCharData( pc );
	if( pc->npcAi.isCall ) return;

	pc->npcAi.callProcedureAddr = pc->npcAi.aiProcedureAddr[paramAddr];
	pc->npcAi.isCall = true;
}


void AI_NPC_InterpretorOperation( playerCharacter_t* pc )
{
	char inter;									
	int paramAddr1, paramAddr2, operatorParam;	
	int variable;								

	
	inter = AI_NPC_GetCharData( pc );
	
	if( inter == 'v' )
	{
		
		paramAddr1 = AI_NPC_GetCharData( pc );
	}

	
	inter = AI_NPC_GetCharData( pc );
	operatorParam = inter;
	
	
	inter = AI_NPC_GetCharData( pc );
	
	if( inter == 'v' )
	{

		paramAddr2 = AI_NPC_GetCharData( pc );
		
		variable = pc->npcAi.aiVariable[paramAddr2];
	}
	
	if( inter == 'n' )
	{
		variable = AI_NPC_GetIntData( pc );
	}
	
	if( inter == 'f' )
	{
		
		variable = AI_NPC_InterpretorFunction( pc );
	}
	
	if( inter == 'd' )
	{
		paramAddr2 = AI_NPC_GetCharData( pc );
		variable = AI_NPC_InterpretorDefine( pc, paramAddr2 );
	}

	
	switch( operatorParam )
	{
	case 0:
		pc->npcAi.aiVariable[paramAddr1] += variable;
		break;
	case 1:
		pc->npcAi.aiVariable[paramAddr1] -= variable;
		break;
	case 2:
		pc->npcAi.aiVariable[paramAddr1] *= variable;
		break;
	case 3:
		pc->npcAi.aiVariable[paramAddr1] /= variable;
		break;
	case 4:
		pc->npcAi.aiVariable[paramAddr1] = variable;
		break;
	}
}



void AI_NPC_InterpretorParameter( playerCharacter_t* pc, int param[], int n )
{
	char inter;									
	int variable, paramAddr;					
	int i;										
	
	for( i = 0; i < n; i ++ )
	{
		inter = AI_NPC_GetCharData( pc );
		
		if( inter == 'n' )
		{
			variable = AI_NPC_GetIntData( pc );
			param[ i ] = variable;
			continue;
		}
		
		if( inter == 'd' )
		{
			paramAddr = AI_NPC_GetCharData( pc );
			variable = AI_NPC_InterpretorDefine( pc, paramAddr );
			param[ i ]= variable;
			continue;
		}
		
		if( inter == 'v' )
		{
			paramAddr = AI_NPC_GetCharData( pc );
			variable = pc->npcAi.aiVariable[ paramAddr ];
			param[ i ] = variable;
			continue;
		}
		
		if( inter == 'f' )
		{
			paramAddr = AI_NPC_GetCharData( pc );
			variable = AI_NPC_InterpretorFunction( pc );
			param[ i ] = variable;
			continue;
		}
	}
}


int AI_NPC_InterpretorCase( playerCharacter_t* pc )
{
	char inter;											
	int cases[5], casesNumber = 0;						

	
	int variable1, variable2, paramAddr, operatorAddr;
	
	int i;												

	while( !pc->npcAi.isCall )
	{
		
		inter = AI_NPC_GetCharData( pc );
		
		if( inter == 't' ) break;
		
		if( inter == 'a' ) inter = AI_NPC_GetCharData( pc );
		
		if( inter == 'v' )
		{
			paramAddr = AI_NPC_GetCharData( pc );
			variable1 = pc->npcAi.aiVariable[ paramAddr ];
		}
		
		if( inter == 'n' )
		{
			variable1 = AI_NPC_GetIntData( pc );
		}
		
		if( inter == 'd' )
		{
			paramAddr = AI_NPC_GetCharData( pc );
			variable1 = AI_NPC_InterpretorDefine( pc, paramAddr );
		}
		
		if( inter == 'f' )
		{
			variable1 = AI_NPC_InterpretorFunction( pc );
		}
		
		inter = AI_NPC_GetCharData( pc );
		operatorAddr = inter;

		
		inter = AI_NPC_GetCharData( pc );
		if( inter == 'v' )
		{
			paramAddr = AI_NPC_GetCharData( pc );
			variable2 = pc->npcAi.aiVariable[ paramAddr ];
		}
		if( inter == 'n' )
		{
			variable2 = AI_NPC_GetIntData( pc );
		}
		if( inter == 'd' )
		{
			paramAddr = AI_NPC_GetCharData( pc );
			variable2 = AI_NPC_InterpretorDefine( pc, paramAddr );
		}
		if( inter == 'f' )
		{
			variable2 = AI_NPC_InterpretorFunction( pc );
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


void AI_NPC_InterpretorThen( playerCharacter_t* pc, int addrInfo[] )
{

	char inter;										

	
	pc->npcAi.aiAddr = addrInfo[0];

	while( !pc->npcAi.isCall )
	{
		inter = AI_NPC_GetCharData( pc );
		
		if( inter == 'e' || inter == 'q' ) 
		{
			pc->npcAi.aiAddr = addrInfo[ 2 ];
			break;
		}
		switch( inter )
		{
		
		case 'o':
			AI_NPC_InterpretorOperation( pc );
			break;
		
		case 'i':
			AI_NPC_InterpretorIF( pc );
			break;
		
		case 'c':
			
			AI_NPC_InterpretorCall( pc );
			break;
		
		case 'f':
			AI_NPC_InterpretorFunction( pc );
			break;
		}
	}
}


void AI_NPC_InterpretorElse( playerCharacter_t* pc, int addrInfo[] )
{
	char inter;									
	
	
	pc->npcAi.aiAddr = addrInfo[1];

	while( !pc->npcAi.isCall )
	{
		inter = AI_NPC_GetCharData( pc );
		
		if( inter == 'q' ) 
		{
			pc->npcAi.aiAddr = addrInfo[ 2 ];
			break;
		}
		switch( inter )
		{
		
		case 'o':
			AI_NPC_InterpretorOperation( pc );
			break;
		
		case 'i':
			AI_NPC_InterpretorIF( pc );
			break;
		
		case 'c':
			
			AI_NPC_InterpretorCall( pc );
			break;
		
		case 'f':
			AI_NPC_InterpretorFunction( pc );
			break;
		}
	}
}


void AI_NPC_InterpretorIF( playerCharacter_t* pc )
{
	char inter;									
	int addrInfo[3];							
	int cases;									

	

	
	addrInfo[0] = AI_NPC_GetIntData( pc );
	
	addrInfo[1] = AI_NPC_GetIntData( pc );
	
	addrInfo[2] = AI_NPC_GetIntData( pc );
	

	while( !pc->npcAi.isCall )
	{
		inter = AI_NPC_GetCharData( pc );
		if( inter == 'q' ) break;
		if( inter == 'a' ) 
		{
			cases = AI_NPC_InterpretorCase( pc );
			
			if( cases ) AI_NPC_InterpretorThen( pc, addrInfo );
			
			
			else if( addrInfo[1] ) AI_NPC_InterpretorElse( pc, addrInfo );
				else pc->npcAi.aiAddr = addrInfo[ 2 ];
		}
	}
}
