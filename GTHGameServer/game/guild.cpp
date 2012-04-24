






#include "..\\global.h"


#include "../Tools/CTools.h"
extern CTools* gcpTools;




void GTH_InitGuildList()
{
	int i, j, max;

	max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;
	for( i=0; i<MAX_GUILDS; i++ )
	{
		memset( &g_guild[i], 0, sizeof(guild_t) );

		g_guild[i].idx = -1;
		g_guild[i].serverGroupID = -1;

		for( j=0; j<max; j++ )
		{
			g_guild[i].userJob[j] = -1;
			g_guild[i].userLevel[j] = 0;
			g_guild[i].userLocation[j] = -1;
		}
	}
}


void GTH_InitGuild( guild_t *guild )
{
	int i, max;
	int idx, serverGroupID;
	char name[GUILDNAMESTRING];

	memset( name, 0, GUILDNAMESTRING );

	
	idx = guild->idx;
	serverGroupID = guild->serverGroupID;
	strcpy( name, guild->name );

	
	memset( guild, 0, sizeof(guild_t) );

	
	guild->idx = idx;
	guild->serverGroupID = serverGroupID;
	strcpy( guild->name, name );

	max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;

	for( i=0; i<max; i++ )
	{
		guild->userJob[i] = -1;
		guild->userLevel[i] = 0;
		guild->userLocation[i] = -1;
	}
}

int GTH_GetEmptyGuildIndex()
{
	int i;

	for( i=0; i<MAX_GUILDS; i++ )
	{
		if( g_guild[i].name[0] == 0 ) break;
	}

	if( i == MAX_GUILDS ) return -1;

	return i;
}


int GTH_GetGuildEmptyIndex( short guildIdx, int rank )
{
	
	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
		return -1;

	int i;

	if( rank == GUILD_SUBMASTER )
	{
		for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
		{
			if( g_guild[guildIdx].subMasterName[i][0] == 0 ) break;
		}

		if( i == MAX_GUILD_SUBMASTERS ) return -1;
	}
	else if( rank == GUILD_FACTIONMASTER )
	{
		for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
		{
			if( g_guild[guildIdx].factionMasterName[i][0] == 0 ) break;
		}

		if( i == MAX_GUILD_FACTIONMASTERS ) return -1;
	}
	else if( rank == GUILD_MATE )
	{
		for( i=0; i<MAX_GUILD_MATES; i++ )
		{
			if( g_guild[guildIdx].mateName[i][0] == 0 ) break;
		}

		if( i == MAX_GUILD_MATES ) return -1;
	}

	return i;
}


int GTH_GetGuildMemberIndex(short guildIdx,int rank,char *name)
{
	
	if( guildIdx < 0 || guildIdx >= MAX_GUILDS ) return -1;

	int idx=-1;
	switch(rank){
		case GUILD_MASTER:
			if(0 == stricmp( g_guild[guildIdx].masterName,name)) return 0;		
			return -1;

		case GUILD_SUBMASTER:
			for(idx=0; idx < MAX_GUILD_SUBMASTERS; idx++ ){
				if(0 == stricmp( g_guild[guildIdx].subMasterName[idx], name)) return idx;
			}
			return -1;

		case GUILD_FACTIONMASTER:
			for(idx=0; idx < MAX_GUILD_FACTIONMASTERS; idx++){
				if(0 == stricmp( g_guild[guildIdx].factionMasterName[idx],name)) return idx;
			}
			return -1;

		case GUILD_MATE:
			for( idx=0; idx < MAX_GUILD_MATES; idx++ ){
				if(0 == stricmp(g_guild[guildIdx].mateName[idx],name)) return idx;
			}
			return -1;
	}

	return -1;
}





int GTH_IsGuildMemberName( short guildIdx, int rank, char *name )
{
	
	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
		return 0;

	int i;

	if( rank == GUILD_SUBMASTER )
	{
		for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
		{
			if( stricmp( g_guild[guildIdx].subMasterName[i], name ) == 0 ) break;
		}

		if( i == MAX_GUILD_SUBMASTERS ) return 0;
	}
	else if( rank == GUILD_FACTIONMASTER )
	{
		for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
		{
			if( stricmp( g_guild[guildIdx].factionMasterName[i], name ) == 0 ) break;
		}

		if( i == MAX_GUILD_FACTIONMASTERS ) return 0;
	}
	else if( rank == GUILD_MATE )
	{
		for( i=0; i<MAX_GUILD_MATES; i++ )
		{
			if( stricmp( g_guild[guildIdx].mateName[i], name ) == 0 ) break;
		}

		if( i == MAX_GUILD_MATES ) return 0;
	}

	return 1;
}


int GTH_GetMateIndexByName( short guildIdx, char *name )
{
	
	if( guildIdx < 0 )
		return -1;

	int i;

	for( i=0; i<MAX_GUILD_MATES; i++ )
	{
		if( g_guild[guildIdx].mateName[i][0] == 0 ) return -1;
		if( stricmp( g_guild[guildIdx].mateName[i], name ) == 0 ) break;
	}

	if( i == MAX_GUILD_MATES ) return -1;

	return i;
}


int GTH_GetGuildIndexByName( char *guildName )
{
	int i;
	for( i=0; i<MAX_GUILDS; i++ )
	{
		if( stricmp(g_guild[i].name, guildName) == 0 ) break;
	}

	if( i == MAX_GUILDS ) return -1;

	return i;
}


short GTH_GuildOrganize( char *name, char* guildName, char* guildMark, int serverGroupID, int &error )
{
	
	if( GTH_GetGuildIndexByName( guildName ) != -1 )
	{
		error = GUILD_ERROR_SAMEGUILDNAME;
		return -1;
	}

	
	short guildIdx = GTH_GetEmptyGuildIndex();

	
	if( guildIdx < 0 )
	{
		error = GUILD_ERROR_MAXOVERGUILDNUM;
		return -1;
	}

	memset( &g_guild[guildIdx], 0, sizeof( guild_t ) );

	g_guild[guildIdx].idx			= guildIdx; 
	g_guild[guildIdx].serverGroupID = serverGroupID;
	strncpy(g_guild[guildIdx].name, guildName,GUILDNAMESTRING);
	g_guild[guildIdx].name[GUILDNAMESTRING]=NULL;


	memcpy( g_guild[guildIdx].mark, guildMark, GUILDMARKSIZE );
	g_guild[guildIdx].markUpdateTime = 0;



	g_guild[guildIdx].famePoint = 0;
	g_guild[guildIdx].expPoint = 0;
	g_guild[guildIdx].raidPoint = 0;
	g_guild[guildIdx].pvpPoint = 0;
	g_guild[guildIdx].hunterPoint = 0;
	g_guild[guildIdx].level = 1;

	strncpy(g_guild[guildIdx].masterName, name ,NAMESTRING);
	g_guild[guildIdx].masterName[NAMESTRING]=NULL;


	entityPCList_t pl;
	if( g_entityListMng.FindPCList( name, &pl ) != -1 )
	{
		g_guild[guildIdx].userJob[0] = pl.job;
		g_guild[guildIdx].userLevel[0] = pl.level;
		g_guild[guildIdx].userLocation[0] = pl.worldIdx;
	}

	
	g_DBGameServer->AddGuild( guildIdx );

	
	
	
	
	

	
	

	
	GTH_GuildUpdate( guildIdx, GUILD_UPDATE_ALL, GUILD_ALLRANK, -1, GUILD_MASTER, -1 );


	
	return guildIdx;
}


int GTH_GuildDisorganize( short guildIdx, char *name, int &error )
{


	if( guildIdx < 0 )
	{
		error = GUILD_ERROR_NOGUILD;
		return -1;
	}

	if( g_guild[guildIdx].idx == -1 && g_guild[guildIdx].name[0] == 0 )
	{
		
		error = GUILD_ERROR_NOGUILD;
		return -1;
	}

	if( stricmp( g_guild[guildIdx].masterName, name ) != 0 )
	{
		
		error = GUILD_ERROR_NOMASTER;
		return -1;
	}

	
	g_DBGameServer->RemoveGuild( guildIdx, g_config.serverGroupNo );

	
	GTH_GuildUpdateDisorganize( guildIdx, 1, 0 );
	
	
	GTH_InitGuild( &g_guild[guildIdx] );

	return 0;
}


int GTH_GuildAddUser( short guildIdx, char *name, int job, int &rankIdxInGuild, int &error )
{
	int update = 0;

	entityPCList_t pl;

	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
		return -1;

	
	int mateIdx = GTH_GetGuildEmptyIndex( guildIdx, GUILD_MATE );
	if( mateIdx == -1 )
	{
		error = GUILD_ERROR_NOTEMPTY;
		return -1;

	}
	

	if( GTH_GetGuildMemberIndex( guildIdx, GUILD_MATE, name )			!= -1 || 
		GTH_GetGuildMemberIndex( guildIdx, GUILD_FACTIONMASTER, name )	!= -1 ||
		GTH_GetGuildMemberIndex( guildIdx, GUILD_SUBMASTER, name )		!= -1 ||
		GTH_GetGuildMemberIndex( guildIdx, GUILD_MASTER, name )			!= -1 )
	{
		error = GUILD_ERROR_SAMEMATENAME;
		return -1;
	}


	
	if( g_entityListMng.FindPCList( name, &pl ) != -1 )
	{
		strcpy( g_guild[guildIdx].mateName[mateIdx], name );
		

		
		
		rankIdxInGuild = mateIdx;
		

		int userIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + mateIdx;

		g_guild[guildIdx].userJob[userIdx] = pl.job;
		g_guild[guildIdx].userLevel[userIdx] = pl.level;
		g_guild[guildIdx].userLocation[userIdx] = pl.worldIdx;

		update = GUILD_UPDATE_ADDUSER | GUILD_MATENAME;
	}

	
	
	
	
	
	
	GTH_GuildUpdate( guildIdx, GUILD_UPDATE_ALL, GUILD_ALLRANK, -1, GUILD_MATE, mateIdx );

	
	
	
	
	
	
	GTH_GuildUpdate( guildIdx, update, GUILD_MATE, mateIdx, GUILD_ALLRANK, -1 );

	
	g_DBGameServer->SaveGuild( guildIdx );



	return 0;
}


int GTH_GuildChangeMaster( short guildIdx, char *reqName, char *accName, int job, int &rankIdxInGuild, int &error )
{
	int update = 0;
	entityPCList_t pl;

	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
		return -1;

	
	int smIdx = GTH_GetGuildMemberIndex( guildIdx, GUILD_SUBMASTER, accName );
	if( smIdx != -1 )
	{
		
		if( GTH_GuildRemoveMaster( guildIdx, reqName, error ) != 0 )
			return -1;

		memset( g_guild[guildIdx].subMasterName[smIdx], 0, NAMESTRING );

		int userIdx = MAX_GUILD_MASTERS + smIdx;

		g_guild[guildIdx].userJob[userIdx] = -1;
		g_guild[guildIdx].userLevel[userIdx] = 0;
		g_guild[guildIdx].userLocation[userIdx] = -1;

		update = GUILD_UPDATE_REMOVEUSER | GUILD_SUBMASTERNAME;

		
		GTH_GuildUpdate( guildIdx, update, GUILD_SUBMASTER, smIdx, GUILD_ALLRANK, -1 );
	}
	else
	{
		
		int fmIdx = GTH_GetGuildMemberIndex( guildIdx, GUILD_FACTIONMASTER, accName );
		if( fmIdx != -1 )
		{
			
			if( GTH_GuildRemoveMaster( guildIdx, reqName, error ) != 0 )
				return -1;

			memset( g_guild[guildIdx].factionMasterName[fmIdx], 0, NAMESTRING );

			int userIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + fmIdx;

			g_guild[guildIdx].userJob[userIdx] = -1;
			g_guild[guildIdx].userLevel[userIdx] = 0;
			g_guild[guildIdx].userLocation[userIdx] = -1;

			update = GUILD_UPDATE_REMOVEUSER | GUILD_FACTIONMASTERNAME;

			
			GTH_GuildUpdate( guildIdx, update, GUILD_FACTIONMASTER, fmIdx, GUILD_ALLRANK, -1 );
		}
		else
		{
			
			int mateIdx = GTH_GetGuildMemberIndex( guildIdx, GUILD_MATE, accName );
			if( mateIdx != -1 )
			{
				bool tempFlag = false;
				
				if( GTH_GuildRemoveMaster( guildIdx, reqName, error ) != 0 )
				{
					if( error == GUILD_ERROR_NOTEMPTY )
						tempFlag = true;
					else
						return -1;
				}

				memset( g_guild[guildIdx].mateName[mateIdx], 0, NAMESTRING );

				int userIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + mateIdx;

				g_guild[guildIdx].userJob[userIdx] = -1;
				g_guild[guildIdx].userLevel[userIdx] = 0;
				g_guild[guildIdx].userLocation[userIdx] = -1;

				update = GUILD_UPDATE_REMOVEUSER | GUILD_MATENAME;

				
				GTH_GuildUpdate( guildIdx, update, GUILD_MATE, mateIdx, GUILD_ALLRANK, -1 );

				if( tempFlag )
				{
					GTH_GuildRemoveMaster( guildIdx, reqName, error );
				}
			}
			else
			{
				error = GUILD_ERROR_NOTJOINED;
				return -1;
			}
		}
	}

	
	strcpy( g_guild[guildIdx].masterName, accName );

	if( g_entityListMng.FindPCList( accName, &pl ) != -1 )
	{
		int userIdx = 0;

		g_guild[guildIdx].userJob[userIdx] = pl.job;
		g_guild[guildIdx].userLevel[userIdx] = pl.level;
		g_guild[guildIdx].userLocation[userIdx] = pl.worldIdx;
	}

	update = GUILD_UPDATE_ADDUSER | GUILD_MASTERNAME;

	
	GTH_GuildUpdate( guildIdx, GUILD_UPDATE_ALL, GUILD_ALLRANK, -1, GUILD_MASTER, 0 );

	
	GTH_GuildUpdate( guildIdx, update, GUILD_MASTER, 0, GUILD_ALLRANK, -1 );

	
	g_DBGameServer->SaveGuild( guildIdx );



	return 0;
}


int GTH_GuildAddSubMaster( short guildIdx, char *name, int job, int &rankIdxInGuild, int &error )
{
	int update = 0;
	entityPCList_t pl;

	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
		return -1;

	
	int smIdx = GTH_GetGuildEmptyIndex( guildIdx, GUILD_SUBMASTER );
	if( smIdx == -1 )
	{
		error = GUILD_ERROR_NOTEMPTY;
		return -1;
	}
	else if( GTH_GetGuildMemberIndex( guildIdx, GUILD_SUBMASTER, name ) != -1)
	{
		error = GUILD_ERROR_SAMEMATENAME;
		return -1;
	}

	
	
	if( GTH_GetGuildMemberIndex( guildIdx, GUILD_FACTIONMASTER, name ) != -1 )
	{
		error = GUILD_ERROR_CANNOTAPPOINT;
		return -1;

	}

	
	
	int mateIdx = GTH_GetGuildMemberIndex( guildIdx, GUILD_MATE, name );
	if( mateIdx != -1 )
	{
		memset( g_guild[guildIdx].mateName[mateIdx], 0, NAMESTRING );

		int userIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + mateIdx;

		g_guild[guildIdx].userJob[userIdx] = -1;
		g_guild[guildIdx].userLevel[userIdx] = 0;
		g_guild[guildIdx].userLocation[userIdx] = -1;

		update = GUILD_UPDATE_REMOVEUSER | GUILD_MATENAME;

		
		GTH_GuildUpdate( guildIdx, update, GUILD_MATE, mateIdx, GUILD_ALLRANK, -1 );
	}
	else
	{
		error = GUILD_ERROR_NOTJOINED;
		return -1;
	}

	update = 0;
	
	
	strcpy( g_guild[guildIdx].subMasterName[smIdx], name );

	if( g_entityListMng.FindPCList( name, &pl ) != -1 )
	{
		int userIdx = MAX_GUILD_MASTERS + smIdx;

		g_guild[guildIdx].userJob[userIdx] = pl.job;
		g_guild[guildIdx].userLevel[userIdx] = pl.level;
		g_guild[guildIdx].userLocation[userIdx] = pl.worldIdx;

		update = GUILD_UPDATE_ADDUSER | GUILD_SUBMASTERNAME;
	}

	
	GTH_GuildUpdate( guildIdx, GUILD_UPDATE_ALL, GUILD_ALLRANK, -1, GUILD_SUBMASTER, smIdx );

	
	GTH_GuildUpdate( guildIdx, update, GUILD_SUBMASTER, smIdx, GUILD_ALLRANK, -1 );

	
	g_DBGameServer->SaveGuild( guildIdx );



	return 0;
}



int GTH_GuildAddFactionMaster( short guildIdx, char *name, int job, int &rankIdxInGuild, int &error )
{
	int update = 0;
	entityPCList_t pl;

	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
		return -1;

	
	int fmIdx = GTH_GetGenTypeFromJob(job);

	if( fmIdx == -1 )
	{
		return -1;
	}
	else if( g_guild[guildIdx].factionMasterName[fmIdx][0] != 0 )
	{
		if( !stricmp( g_guild[guildIdx].factionMasterName[fmIdx], name ) )
		{
			error = GUILD_ERROR_SAMEMATENAME;
			return -1;
		}
		else
		{
			error = GUILD_ERROR_NOTEMPTY;
			return -1;
		}
	}

	
	
	if( GTH_GetGuildMemberIndex( guildIdx, GUILD_SUBMASTER, name ) != -1 )
	{
		error = GUILD_ERROR_CANNOTAPPOINT;
		return -1;
	}

	
	
	int mateIdx = GTH_GetGuildMemberIndex( guildIdx, GUILD_MATE, name );
	if( mateIdx != -1 )
	{
		memset( g_guild[guildIdx].mateName[mateIdx], 0, NAMESTRING );

		int userIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + mateIdx;

		g_guild[guildIdx].userJob[userIdx] = -1;
		g_guild[guildIdx].userLevel[userIdx] = 0;
		g_guild[guildIdx].userLocation[userIdx] = -1;

		update = GUILD_UPDATE_REMOVEUSER | GUILD_MATENAME;

		
		GTH_GuildUpdate( guildIdx, update, GUILD_MATE, mateIdx, GUILD_ALLRANK, -1 );
	}
	else
	{
		error = GUILD_ERROR_NOTJOINED;
		return -1;
	}

	update = 0;
	
	strcpy( g_guild[guildIdx].factionMasterName[fmIdx], name );

	if( g_entityListMng.FindPCList( name, &pl ) != -1 )
	{
		int userIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + fmIdx;

		g_guild[guildIdx].userJob[userIdx] = pl.job;
		g_guild[guildIdx].userLevel[userIdx] = pl.level;
		g_guild[guildIdx].userLocation[userIdx] = pl.worldIdx;

		update = GUILD_UPDATE_ADDUSER | GUILD_FACTIONMASTERNAME;
	}

	
	GTH_GuildUpdate( guildIdx, GUILD_UPDATE_ALL, GUILD_ALLRANK, -1, GUILD_FACTIONMASTER, fmIdx );

	
	GTH_GuildUpdate( guildIdx, update, GUILD_FACTIONMASTER, fmIdx, GUILD_ALLRANK, -1 );

	
	g_DBGameServer->SaveGuild( guildIdx );



	return 0;
}


int GTH_GuildRemoveUser( short guildIdx, char *name, int &error )
{
	int i, update;
	int idx;



	if( guildIdx < 0 )
	{
		return -1;
	}

	
	for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
	{
		if( g_guild[guildIdx].subMasterName[i][0] == 0 ) continue;

		if( stricmp( g_guild[guildIdx].subMasterName[i], name ) == 0 )
		{
			memset( g_guild[guildIdx].subMasterName[i], 0, NAMESTRING );

			idx = MAX_GUILD_MASTERS + i;

			g_guild[guildIdx].userJob[idx] = -1;
			g_guild[guildIdx].userLevel[idx] = 0;
			g_guild[guildIdx].userLocation[idx] = -1;

			update = GUILD_UPDATE_REMOVEUSER | GUILD_SUBMASTERNAME;

			
			GTH_GuildUpdate( guildIdx, update, GUILD_SUBMASTER, i, GUILD_ALLRANK, -1 );

			
			g_DBGameServer->SaveGuild( guildIdx );

			return 0;
		}
	}

	for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
	{
		if( g_guild[guildIdx].factionMasterName[i][0] == 0 ) continue;

		if( stricmp( g_guild[guildIdx].factionMasterName[i], name ) == 0 )
		{
			memset( g_guild[guildIdx].factionMasterName[i], 0, NAMESTRING );

			idx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + i;

			g_guild[guildIdx].userJob[idx] = -1;
			g_guild[guildIdx].userLevel[idx] = 0;
			g_guild[guildIdx].userLocation[idx] = -1;

			update = GUILD_UPDATE_REMOVEUSER | GUILD_FACTIONMASTERNAME;
			
			
			GTH_GuildUpdate( guildIdx, update, GUILD_FACTIONMASTER, i, GUILD_ALLRANK, -1 );

			
			g_DBGameServer->SaveGuild( guildIdx );

			return 0;
		}
	}

	
	for( i=0; i<MAX_GUILD_MATES; i++)
	{
		if( g_guild[guildIdx].mateName[i][0] == 0 ) continue;

		if( stricmp( g_guild[guildIdx].mateName[i], name ) == 0 )
		{
			
			memset( g_guild[guildIdx].mateName[i], 0, NAMESTRING );
			idx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + i;

			g_guild[guildIdx].userJob[idx] = -1;
			g_guild[guildIdx].userLevel[idx] = 0;
			g_guild[guildIdx].userLocation[idx] = -1;

			update = GUILD_UPDATE_REMOVEUSER | GUILD_MATENAME;

			
			GTH_GuildUpdate( guildIdx, update, GUILD_MATE, i, GUILD_ALLRANK, -1 );

			
			g_DBGameServer->SaveGuild( guildIdx );

			return 0;
		}
	}
	error = GUILD_ERROR_CANNOTFIND;
	return -1;
}





int GTH_GuildRemoveMaster( short guildIdx, char *name, int &error )
{
	int update1, update2;
	int emptyIdx, idx;
	entityPCList_t pl;



	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
	{
		error = -1;
		return -1;
	}

	if( stricmp( g_guild[guildIdx].masterName, name ) == 0 )
	{
		
		emptyIdx = GTH_GetGuildEmptyIndex( guildIdx, GUILD_MATE );
		if( emptyIdx == -1 )
		{
			error = GUILD_ERROR_NOTEMPTY;
			return -1;
		}
		else
		{
			
			if( stricmp( g_guild[guildIdx].mateName[emptyIdx], name ) == 0 )
			{
				error = GUILD_ERROR_CANNOTAPPOINT;
				return -1;
			}
		}

		
		memset( g_guild[guildIdx].masterName, 0, NAMESTRING );

		idx = 0;

		g_guild[guildIdx].userJob[idx] = -1;
		g_guild[guildIdx].userLevel[idx] = 0;
		g_guild[guildIdx].userLocation[idx] = -1;

		update1 = GUILD_UPDATE_REMOVEUSER | GUILD_MASTERNAME;

		
		strcpy( g_guild[guildIdx].mateName[emptyIdx], name );

		idx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + emptyIdx;

		
		if( g_entityListMng.FindPCList( name, &pl ) != -1 )
		{
			g_guild[guildIdx].userJob[idx] = pl.job;
			g_guild[guildIdx].userLevel[idx] = pl.level;
			g_guild[guildIdx].userLocation[idx] = pl.worldIdx;
		}

		update2 = GUILD_UPDATE_ADDUSER | GUILD_MATENAME;

		
		GTH_GuildUpdate( guildIdx, update1, GUILD_MASTER, 0, GUILD_ALLRANK, -1 );

		
		GTH_GuildUpdate( guildIdx, update2, GUILD_MATE, emptyIdx, GUILD_ALLRANK, -1 );

		
		g_DBGameServer->SaveGuild( guildIdx );

		return 0;
	}

	error = GUILD_ERROR_CANNOTFIND;

	return -1;
}


int GTH_GuildRemoveSubMaster( short guildIdx, char *name, int &error )
{
	int i;
	int update1, update2;
	int emptyIdx, idx;
	entityPCList_t pl;



	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
	{
		error = -1;
		return -1;
	}

	
	for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
	{
		if( g_guild[guildIdx].subMasterName[i][0] == 0 ) continue;

		if( stricmp( g_guild[guildIdx].subMasterName[i], name ) == 0 )
		{
			
			emptyIdx = GTH_GetGuildEmptyIndex( guildIdx, GUILD_MATE );
			if( emptyIdx == -1 )
			{
				error = GUILD_ERROR_NOTEMPTY;
				return -1;
			}
			else
			{
				
				if( stricmp( g_guild[guildIdx].mateName[emptyIdx], name ) == 0 )
				{
					error = GUILD_ERROR_SAMEMATENAME;
					return -1;
				}
			}

			
			memset( g_guild[guildIdx].subMasterName[i], 0, NAMESTRING );

			idx = MAX_GUILD_MASTERS + i;

			g_guild[guildIdx].userJob[idx] = -1;
			g_guild[guildIdx].userLevel[idx] = 0;
			g_guild[guildIdx].userLocation[idx] = -1;

			update1 = GUILD_UPDATE_REMOVEUSER | GUILD_SUBMASTERNAME;

			
			strcpy( g_guild[guildIdx].mateName[emptyIdx], name );

			idx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + emptyIdx;

			
			if( g_entityListMng.FindPCList( name, &pl ) != -1 )
			{
				g_guild[guildIdx].userJob[idx] = pl.job;
				g_guild[guildIdx].userLevel[idx] = pl.level;
				g_guild[guildIdx].userLocation[idx] = pl.worldIdx;
			}
			
			else
			{
				g_guild[guildIdx].userJob[idx] = -1;
				g_guild[guildIdx].userLevel[idx] = 0;
				g_guild[guildIdx].userLocation[idx] = -1;
			}

			update2 = GUILD_UPDATE_ADDUSER | GUILD_MATENAME;

			
			GTH_GuildUpdate( guildIdx, update1, GUILD_SUBMASTER, i, GUILD_ALLRANK, -1 );

			
			GTH_GuildUpdate( guildIdx, update2, GUILD_MATE, emptyIdx, GUILD_ALLRANK, -1 );

			
			g_DBGameServer->SaveGuild( guildIdx );

			return 0;
		}
	}

	error = GUILD_ERROR_CANNOTFIND;

	return -1;
}


int GTH_GuildRemoveFactionMaster( short guildIdx, char *name, int &error )
{
	int i;
	int update1, update2;
	int emptyIdx, idx;
	entityPCList_t pl;



	if( guildIdx < 0 || guildIdx >= MAX_GUILDS )
	{
		error = -1;
		return -1;
	}

	
	for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
	{
		if( g_guild[guildIdx].factionMasterName[i][0] == 0 ) continue;

		if( stricmp( g_guild[guildIdx].factionMasterName[i], name ) == 0 )
		{
			
			emptyIdx = GTH_GetGuildEmptyIndex( guildIdx, GUILD_MATE );
			if( emptyIdx == -1 )
			{
				error = GUILD_ERROR_NOTEMPTY;
				return -1;
			}
			else
			{
				
				if( stricmp( g_guild[guildIdx].mateName[emptyIdx], name ) == 0 )
				{
					error = GUILD_ERROR_SAMEMATENAME;
					return -1;
				}
			}

			
			memset( g_guild[guildIdx].factionMasterName[i], 0, NAMESTRING );

			idx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + i;

			g_guild[guildIdx].userJob[idx] = -1;
			g_guild[guildIdx].userLevel[idx] = 0;
			g_guild[guildIdx].userLocation[idx] = -1;

			update1 = GUILD_UPDATE_REMOVEUSER | GUILD_FACTIONMASTERNAME;

			
			
			strcpy( g_guild[guildIdx].mateName[emptyIdx], name );

			idx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + emptyIdx;

			
			if( g_entityListMng.FindPCList( name, &pl ) != -1 )
			{
				g_guild[guildIdx].userJob[idx] = pl.job;
				g_guild[guildIdx].userLevel[idx] = pl.level;
				g_guild[guildIdx].userLocation[idx] = pl.worldIdx;
			}
			
			else
			{
				g_guild[guildIdx].userJob[idx] = -1;
				g_guild[guildIdx].userLevel[idx] = 0;
				g_guild[guildIdx].userLocation[idx] = -1;
			}

			update2 = GUILD_UPDATE_ADDUSER | GUILD_MATENAME;

			
			GTH_GuildUpdate( guildIdx, update1, GUILD_FACTIONMASTER, i, GUILD_ALLRANK, -1 );

			
			GTH_GuildUpdate( guildIdx, update2, GUILD_MATE, emptyIdx, GUILD_ALLRANK, -1 );

			
			g_DBGameServer->SaveGuild( guildIdx );

			return 0;
		}
	}

	error = GUILD_ERROR_CANNOTFIND;

	return -1;
}


void GTH_GuildUpdateUser( short guildIdx, char *name, int rank, int rankIdx, int type, int value )
{
	int idx = -1;

	if( rank == GUILD_MATE )
	{
		
		if( g_guild[guildIdx].mateName[rankIdx][0] == 0 ) return;

		if( stricmp( g_guild[guildIdx].mateName[rankIdx], name ) == 0 )
		{
			idx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+rankIdx;
		}
	}
	else if( rank == GUILD_FACTIONMASTER )
	{
		
		if( g_guild[guildIdx].factionMasterName[rankIdx][0] == 0 ) return;

		if( stricmp( g_guild[guildIdx].factionMasterName[rankIdx], name ) == 0 )
		{
			idx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+rankIdx;
		}
	}
	else if( rank == GUILD_SUBMASTER )
	{
		
		if( g_guild[guildIdx].subMasterName[rankIdx][0] == 0 ) return;

		if( stricmp( g_guild[guildIdx].subMasterName[rankIdx], name ) == 0 )
		{
			idx = MAX_GUILD_MASTERS+rankIdx;
		}
	}
	else if( rank = GUILD_MASTER )
	{
		
		if( stricmp( g_guild[guildIdx].masterName, name ) == 0 )
		{
			idx = 0;
		}
	}

	if( idx == -1 ) return;

	if( type == GUILD_USERLEVEL )
	{
		g_guild[guildIdx].userLevel[idx] = value;
	}
	else if( type == GUILD_USERJOB )
	{
		g_guild[guildIdx].userJob[idx] = value;
	}
	else if( type == GUILD_USERLOCATION )
	{
		g_guild[guildIdx].userLocation[idx] = value;
	}

	GTH_GuildUpdate( guildIdx, type, rank, rankIdx, GUILD_ALLRANK, -1 );
}


void GTH_GuildUpdate( short guildIdx, int update, int value1, int value2, int rank, int rankIdx )
{
	int i;
	entityPCList_t pl;

	
	
	
	

	
	
	

	ShowLogInfo( "GuildUpdate, %d, %d, %d, what %d, %d, To %d, %d", guildIdx, update, value1, value2, rank, rankIdx );

	if( ( rank & GUILD_MASTER ) != 0 )
	{
		
		if( g_entityListMng.FindPCList( g_guild[guildIdx].masterName, &pl ) != -1 )
		{
			if( pl.serverIdx == 0 )
			{
				
				GTH_SendMessage_Guild_Update_To_Client( &g_pc[pl.idxOnServer], &g_guild[guildIdx], update, value1, value2 );
			}
			else
			{
				GTH_SendMessage_Guild_Update_To_MemberServer( pl.serverIdx, pl.idxOnServer, &g_guild[guildIdx], update, value1, value2 );
			}
		}
	}
	
	if( ( rank & GUILD_SUBMASTER ) != 0 )
	{
		
		for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
		{
			if( rankIdx == -1 || rankIdx == i )
			{
				if( g_guild[guildIdx].subMasterName[i][0] != 0 )
				{
					if( g_entityListMng.FindPCList( g_guild[guildIdx].subMasterName[i], &pl ) != -1 )
					{
						if( pl.serverIdx == 0 )
						{
							
							GTH_SendMessage_Guild_Update_To_Client( &g_pc[pl.idxOnServer], &g_guild[guildIdx], update, value1, value2 );
						}
						else
						{
							GTH_SendMessage_Guild_Update_To_MemberServer( pl.serverIdx, pl.idxOnServer, &g_guild[guildIdx], update, value1, value2 );
						}
					}
				}
			}
		}
	}
	
	if( ( rank & GUILD_FACTIONMASTER ) != 0 )
	{
		
		for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++)
		{
			if( rankIdx == -1 || rankIdx == i )
			{
				if( g_guild[guildIdx].factionMasterName[i][0] != 0 )
				{
					if( g_entityListMng.FindPCList( g_guild[guildIdx].factionMasterName[i], &pl ) != -1 )
					{
						if( pl.serverIdx == 0 )
						{
							
							GTH_SendMessage_Guild_Update_To_Client( &g_pc[pl.idxOnServer], &g_guild[guildIdx], update, value1, value2 );
						}
						else
						{
							GTH_SendMessage_Guild_Update_To_MemberServer( pl.serverIdx, pl.idxOnServer, &g_guild[guildIdx], update, value1, value2 );
						}
					}
				}
			}
		}
	}
	
	if( ( rank & GUILD_MATE ) != 0 )
	{
		
		for( i=0; i<MAX_GUILD_MATES; i++)
		{
			if( rankIdx == -1 || rankIdx == i )
			{
				if( g_guild[guildIdx].mateName[i][0] != 0 )
				{
					if( g_entityListMng.FindPCList( g_guild[guildIdx].mateName[i], &pl ) != -1 )
					{
						if( pl.serverIdx == 0 )
						{
							
							GTH_SendMessage_Guild_Update_To_Client( &g_pc[pl.idxOnServer], &g_guild[guildIdx], update, value1, value2 );
						}
						else
						{
							GTH_SendMessage_Guild_Update_To_MemberServer( pl.serverIdx, pl.idxOnServer, &g_guild[guildIdx], update, value1, value2 );
						}
					}
				}
			}
		}
	}
}




void GTH_GuildSetUserInfo( short guildIdx, char *name, int &rankInGuild, int &rankIdxInGuild )
{
	int i, userIdx;
	entityPCList_t pl;

	rankInGuild = -1;
	rankIdxInGuild = -1;
	userIdx = -1;

	if( stricmp( g_guild[guildIdx].masterName, name ) == 0 )
	{
		rankInGuild = GUILD_MASTER;
		userIdx = 0;
	}

	if( rankInGuild == -1 )
	{
		for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
		{
			if( stricmp( g_guild[guildIdx].subMasterName[i], name ) == 0 )
			{
				rankInGuild = GUILD_SUBMASTER;
				rankIdxInGuild = i;
				userIdx = MAX_GUILD_MASTERS + i;
				break;
			}
		}
	}

	if( rankInGuild == -1 )
	{
		for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++)
		{
			if( stricmp( g_guild[guildIdx].factionMasterName[i], name ) == 0 )
			{
				rankInGuild = GUILD_FACTIONMASTER;
				rankIdxInGuild = i;
				userIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + i;
				break;
			}
		}
	}

	if( rankInGuild == -1 )
	{
		for( i=0; i<MAX_GUILD_MATES; i++)
		{
			if( stricmp( g_guild[guildIdx].mateName[i], name ) == 0 )
			{
				rankInGuild = GUILD_MATE;
				rankIdxInGuild = i;
				userIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + i;
				break;
			}
		}
	}

	if( rankInGuild == -1 && rankIdxInGuild == -1 )
	{

		return;
	}

	if( g_entityListMng.FindPCList( name, &pl ) != -1 )
	{
		g_guild[guildIdx].userJob[userIdx] = pl.job;
		g_guild[guildIdx].userLevel[userIdx] = pl.level;
		g_guild[guildIdx].userLocation[userIdx] = pl.worldIdx;
	}
}



void GTH_GuildChatMessage( short guildIdx, char *name, char *chatMessage )
{
	int i;
	entityPCList_t pl;



	
	if( g_entityListMng.FindPCList( g_guild[guildIdx].masterName, &pl ) != -1 )
	{
		if( pl.serverIdx == 0 )
		{
			
			GTH_SendMessage_Guild_ChatMessage_Reply_To_Client( &g_pc[pl.idxOnServer], name, chatMessage );
		}
		else
		{
			GTH_SendMessage_Guild_ChatMessage_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, name, chatMessage );
		}
	}
	
	
	for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
	{
		if( g_guild[guildIdx].subMasterName[i][0] != 0 )
		{
			if( g_entityListMng.FindPCList( g_guild[guildIdx].subMasterName[i], &pl ) != -1 )
			{
				if( pl.serverIdx == 0 )
				{
					
					GTH_SendMessage_Guild_ChatMessage_Reply_To_Client( &g_pc[pl.idxOnServer], name, chatMessage );
				}
				else
				{
					GTH_SendMessage_Guild_ChatMessage_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, name, chatMessage );
				}
			}
		}
	}
	
	
	for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++)
	{
		if( g_guild[guildIdx].factionMasterName[i][0] != 0 )
		{
			if( g_entityListMng.FindPCList( g_guild[guildIdx].factionMasterName[i], &pl ) != -1 )
			{
				if( pl.serverIdx == 0 )
				{
					
					GTH_SendMessage_Guild_ChatMessage_Reply_To_Client( &g_pc[pl.idxOnServer], name, chatMessage );
				}
				else
				{
					GTH_SendMessage_Guild_ChatMessage_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, name, chatMessage );
				}
			}
		}
	}

	
	for( i=0; i<MAX_GUILD_MATES; i++)
	{
		if( g_guild[guildIdx].mateName[i][0] != 0 )
		{
			if( g_entityListMng.FindPCList( g_guild[guildIdx].mateName[i], &pl ) != -1 )
			{
				if( pl.serverIdx == 0 )
				{
					
					GTH_SendMessage_Guild_ChatMessage_Reply_To_Client( &g_pc[pl.idxOnServer], name, chatMessage );
				}
				else
				{
					GTH_SendMessage_Guild_ChatMessage_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, name, chatMessage );
				}
			}
		}
	}
}



void GTH_GuildUpdateDisorganize( short guildIdx, int result, int error )
{
	int i;
	entityPCList_t pl;



	
	for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
	{
		if( g_guild[guildIdx].subMasterName[i][0] != 0 )
		{
			if( g_entityListMng.FindPCList( g_guild[guildIdx].subMasterName[i], &pl ) != -1 )
			{
				if( pl.serverIdx == 0 )
				{
					
					g_pc[pl.idxOnServer].guildIdx = -1;
					GTH_SendMessage_Guild_Disorganize_Reply_To_Client( &g_pc[pl.idxOnServer], result, error );
				}
				else
				{
					GTH_SendMessage_Guild_Disorganize_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, result, error );
				}
			}
		}
	}
	
	
	for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++)
	{
		if( g_guild[guildIdx].factionMasterName[i][0] != 0 )
		{
			if( g_entityListMng.FindPCList( g_guild[guildIdx].factionMasterName[i], &pl ) != -1 )
			{
				if( pl.serverIdx == 0 )
				{
					
					g_pc[pl.idxOnServer].guildIdx = -1;
					GTH_SendMessage_Guild_Disorganize_Reply_To_Client( &g_pc[pl.idxOnServer], result, error );
				}
				else
				{
					GTH_SendMessage_Guild_Disorganize_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, result, error );
				}
			}
		}
	}
	
	
	for( i=0; i<MAX_GUILD_MATES; i++)
	{
		if( g_guild[guildIdx].mateName[i][0] != 0 )
		{
			if( g_entityListMng.FindPCList( g_guild[guildIdx].mateName[i], &pl ) != -1 )
			{
				if( pl.serverIdx == 0 )
				{
					
					g_pc[pl.idxOnServer].guildIdx = -1;
					GTH_SendMessage_Guild_Disorganize_Reply_To_Client( &g_pc[pl.idxOnServer], result, error );
				}
				else
				{
					GTH_SendMessage_Guild_Disorganize_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, result, error );
				}
			}
		}
	}
}







playerCharacter_t* GTH_GetGuildNoneBusyMemberPtr(int GuildIdx, int WorldIdx, int MemberType)
{
	if ( WorldIdx < 0 || WorldIdx >= MAX_GUILDS)
		return 0;
	
	guild_t* pGuild = NULL;
	pGuild = &g_guild[GuildIdx];
	playerCharacter_t* pPC  = NULL;	
	
	if ( MemberType == MAX_GUILD_MASTERS)
	{	
		
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->masterName);	
		if ( pPC != NULL )
		{
			if ( pPC->worldIdx  == WorldIdx && pPC->busyState == BUSY_STATE_NONE)
				return pPC;
		}
	}

	if ( MemberType == MAX_GUILD_SUBMASTERS)
	{	
		
		for(int SubMasterIdx=0; SubMasterIdx < MAX_GUILD_SUBMASTERS; SubMasterIdx++)
		{
			pPC = gcpTools->GetPlayerRecordPointer(pGuild->subMasterName[SubMasterIdx]);
			if ( pPC != NULL )
			{
				if ( pPC->worldIdx  == WorldIdx && pPC->busyState == BUSY_STATE_NONE)
					return pPC;
			}		
		}
	}

	return NULL;

}

void GTH_GetGuildNameByIdx(const int idx, char* szString_OUT)
{
	if ( idx <0 || idx >= MAX_GUILDS)
	{
		strncpy(szString_OUT, "UnknowGuild", GUILDNAMESTRING);
		szString_OUT[GUILDNAMESTRING-1] = NULL;
		return;
	}
	strncpy(szString_OUT, g_guild[idx].name, GUILDNAMESTRING);
	
	
}