#include "../global.h"
 
HANDLE	g_consoleThreadHandle;
DWORD	g_consoleThreadID;

HANDLE	g_consoleOutHandle;
HANDLE	g_consoleInHandle;

char	g_seps[4] = { 13, 10, ' ', 0 };

CGTHConsoleInfo		g_gthConsoleInfo;

void ConsoleOutput( const char *format, ... )
{
	DWORD dw;
	
	va_list argptr;
	char buffer[4096];
	
	
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

	WriteFile( g_consoleOutHandle, buffer, strlen( buffer ), &dw, NULL );
}

BOOL ConsoleHandler( DWORD dwEvent )
{
	switch( dwEvent )
	{
	case CTRL_C_EVENT :
		return true;
	case CTRL_BREAK_EVENT :
		return true;
	}
	return false;
}

void CreateConsoleInfo()
{
	AllocConsole();

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)ConsoleHandler, TRUE );

	g_consoleOutHandle = GetStdHandle( STD_OUTPUT_HANDLE );
	g_consoleInHandle = GetStdHandle( STD_INPUT_HANDLE );

	SetConsoleMode( g_consoleInHandle, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT );

	g_consoleThreadHandle = CreateThread( NULL, 0, ConsoleInfoThread, NULL, 0, &g_consoleThreadID );
	CloseHandle( g_consoleThreadHandle );
}

DWORD WINAPI ConsoleInfoThread(LPVOID param)
{
	DWORD dw;
	char szOutput[64];
	char *token;
	char title[100];
	if( g_config.isManager )
		strcpy( title, "GTH Manager server debug console window - \n\"quit\" to exit\n\n" );
	else 
		strcpy( title, "GTH Game server debug console window - \n\"quit\" to exit\n\n" );

	WriteFile( g_consoleOutHandle, title, strlen( title ), &dw, NULL );

	while( 1 )
	{
		memset( szOutput, 0, sizeof( szOutput ) );
		WriteFile( g_consoleOutHandle, "> ", 2, &dw, NULL );
		ReadFile( g_consoleInHandle, szOutput, 63, &dw, NULL );
		token = strtok( szOutput, g_seps );

		if( !g_gthConsoleInfo.Process( token ) ) break;
	}
	
	FreeConsoleInfo();

	return 0;
}

void FreeConsoleInfo()
{
	FreeConsole();
}





int CGTHConsoleInfo::Process( char* token )
{
	if( token == NULL ) return true;
	if( !stricmp( token, "quit" ) ) return false;
	
	else if( !stricmp( token, "party" ) )
	{
		Party( token );
	}
	
	else if( !stricmp( token, "checkdupuser" ) )
	{
		CheckDupUser();
	}
	else if( !stricmp( token, "viewpclist" ) )
	{
		ViewPCList( 1 );
	}
	else if( !stricmp( token, "countpclist" ) )
	{
		ViewPCList( 0 );
	}
	else if( !stricmp( token, "reloadtable" ) )
	{
		ReloadTable();
	}
	else if( !stricmp( token, "respawnall" ) )
	{
		RespawnAllMonster();
	}
	
	else if( !stricmp( token, "checkexp" ) )
	{
		for( int i = 0; i < MAX_MONSTERS; i ++ )
		{
			if( !g_monster[i].active || g_monster[i].entityType == ENTITY_MONSTER ) continue;
			
			if( g_monster[i].basic.exp <= 0 )
			{
				ConsoleOutput( "[%d]%s monster exp = 0", g_monster[i].idx, g_monster[i].basic.name );
			}
		}
	}
	
	else
	{
		ConsoleOutput( "invaild command\n" );
	}

	return true;
}


void CGTHConsoleInfo::RespawnAllMonster()
{
	for( int i = 0; i < MAX_MONSTERS; i ++ )
	{
		if( !g_monster[i].active ) continue;

		g_monster[i].aliveState = MON_ALIVE_STATE_CORPSE;
		g_monster[i].ai.aiEvent = AI_DIE;
		g_monster[i].event = GTH_EV_CHAR_DIE;
		g_monster[i].oldEvent = GTH_EV_CHAR_DIE;
		g_monster[i].deathTime = g_globalTime;
	}
	ConsoleOutput( "all monster respawn complete!\n\n" );
}

void CGTHConsoleInfo::ReloadTable()
{
	LoadAllGameTable();
	ConsoleOutput( "reload - sit, slt, sot, sqg, sgt, smt, skt\n" );
	
	LoadBindingTableForServer( "tables/bindingTable.tbl" );
	ConsoleOutput( "reload - binding table\n" );

	LoadItemCraftTableForServer( "tables/itemcraft.tbl" );
	ConsoleOutput( "reload - itemcraft table\n" );

	ConsoleOutput( "all table reload complete!\n\n" );
}

void CGTHConsoleInfo::PartyOutput( serverPartyInfo_t *p )
{
	char outputStr[2550];
	int i;

	memset( outputStr, 0, sizeof( outputStr ) );

	if( p == NULL )
	{
		ConsoleOutput( "don't find party data\n" );
		return;
	}
	
	sprintf( outputStr, "%sparty idx : %d\norganizer name : %s, organize time : %d, organize server : %d\n", outputStr,
		p->idx, p->organizerName, p->organizeTime, p->organizeServer );

	sprintf( outputStr, "%scan join : %d, item divide type : %d\n", outputStr,
		p->canJoin, p->itemDivideType );
	

	sprintf( outputStr, "%smember number : %d\n", outputStr,
		p->memberNumber );

	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( p->member[i].serverIndex == -1 ) continue;
		
		sprintf( outputStr, "%smember name : %s(Lv %d), gen type : %d, leader : %d\n", outputStr,
			p->member[i].memberName, p->member[i].level, p->member[i].gen, p->member[i].isLeader );
		
		sprintf( outputStr, "%sactive : %d, server index : %d, world index : %d\n", outputStr,
			p->member[i].isActive, p->member[i].serverIndex, p->member[i].worldIdx );
	}
	ConsoleOutput( outputStr );
}


void CGTHConsoleInfo::Party( char* token )
{
	
	
	
	int sort = 0;
	char valueStr[255];
	int valueInt = 0;
	int i, j, ret = 0;

	if( !g_config.isManager )
	{
		ConsoleOutput( "\"party\" command use only manager server\n" );
		return;
	}

	token = strtok( NULL, g_seps );
	if( token != NULL )
	{
		
		if( !stricmp( token, "name" ) ) sort = 1;
		else if( !stricmp( token, "idx" ) ) sort = 2;
		else if( !stricmp( token, "?") )
		{
			ConsoleOutput( "usage : party [sort] [value]\n" );
			ConsoleOutput( "[sort] : name, idx\n" );
			ConsoleOutput( "[value] : each value\n\n" );
			return;
		}

		memset( valueStr, 0, sizeof( valueStr ) );
		
		token = strtok( NULL, g_seps );
		if( token != NULL )
		{
			if( sort == 1 ) strcpy( valueStr, token );
			else if( sort == 2 ) valueInt = atoi( token );
		}
	}
	
	for( i = 0; i < MAX_PARTY_NUMBER; i ++ )
	{
		if( g_partyInfo[i].memberNumber == 0 ) continue;

		
		if( sort == 1 )
		{
			for( j = 0; j < g_partyInfo[i].memberNumber; j ++ )
			{
				if( g_partyInfo[i].member[j].serverIndex == -1 ) continue;
				if( !stricmp( g_partyInfo[i].member[j].memberName, valueStr ) ) break;
			}
			
			if( j == g_partyInfo[i].memberNumber ) continue;
			PartyOutput( &g_partyInfo[i] );
			ret = 1;
			break;
		} 
		
		else if( sort == 2 )
		{
			if( g_partyInfo[i].idx == valueInt )
			{
				PartyOutput( &g_partyInfo[i] );
				ret = 1;
				break;
			}
		}
		
		else
		{
			PartyOutput( &g_partyInfo[i] );
			ret = 1;
		}
	}
	if( !ret )
	{
		ConsoleOutput( "has no party result\n" );
	}
}

void CGTHConsoleInfo::CheckDupUser()
{
	int count = 0;

	char szTemp[256];
	for( int i = 0; i < MAX_PCS; i ++ )
	{
		if( !g_pc[i].active ) continue;
		if( g_pc[i].idx != i )
		{
			sprintf( szTemp, "%d Index %s(%d) PC is index miss-match error!\n", i, g_pc[i].name, g_pc[i].idx );
			ConsoleOutput( szTemp );
			count ++;
		}

		for( int j = 0; j < MAX_PCS; j ++ )
		{
			if( !g_pc[j].active ) continue;
			if( i == j ) continue;

			if( g_pc[i].idx == g_pc[j].idx )
			{
				sprintf( szTemp, "%d Index %s(%d) PC is index duplicate error!\n", i, g_pc[i].name, g_pc[i].idx );
				ConsoleOutput( szTemp );

				count ++;
			}
			if( !stricmp( g_pc[i].name, g_pc[j].name ) )
			{
				sprintf( szTemp, "%d Index %s(%d) PC is name duplicate error!\n", i, g_pc[i].name, g_pc[i].idx );
				ConsoleOutput( szTemp );

				count ++;
			}
		}
	}
	if( count > 0 ) 
	{
		sprintf( szTemp, "%d user error!\n", count );
		ConsoleOutput( szTemp );
	}
	else
	{
		ConsoleOutput( "no error\n\n" );
	}
}

void CGTHConsoleInfo::ViewPCList( int type )
{
	char szTemp[256];

	if( !g_config.isManager )
	{
		ConsoleOutput( "\"viewpclist\" command use only manager server\n" );
		return;
	}
	if( !type )
	{
		sprintf( szTemp, "pc number in server group : %d\n", g_entityListMng.GetPCListNumber() );
		ConsoleOutput( szTemp );
	}
	else
	{
		entityPCList_t *pPCList = NULL;
		VECTOR_ENTITYPCLIST_STRUCT::iterator	itor;
		
		for( itor = g_entityListMng.m_vEntityPCList.begin(); itor != g_entityListMng.m_vEntityPCList.end(); itor ++ )
		{
			pPCList = ( *itor );
			sprintf( szTemp, "userID : %s, name : %s, serverIdx : %d, worldName : %s, charInfo : %s(Lv.%d)\n", 
				pPCList->userID, pPCList->name, pPCList->serverIdx, 
				g_worldTable[pPCList->worldIdx].name, g_itemReqClassString[pPCList->job], pPCList->level );
			ConsoleOutput( szTemp );
		}
	}
}

