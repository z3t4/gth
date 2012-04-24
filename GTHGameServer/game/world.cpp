#include "../global.h"

extern char *curpos, *endpos;
int GTH_LoadWorldTable()
{
	FILE *fp;
	int size;
	byte *buffer;
	char *token;
	int idx;
	int i;

	fp = fopen( "tables/world.tbl" , "rb" );
	if( !fp )   
	{
		MessageBox(NULL, "Does not exist 'tables/world.tbl' file", "iWorld3D", MB_ICONHAND|MB_OK);
		return false;
	}

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;

	g_worldTableNumber = 0;
	idx = 0;
	while ((token = NextToken()) != NULL)
    {
		if( !stricmp( token, "WORLD_IDX" ) )
		{
			idx = atoi( NextArg() );
			memset( &g_worldTable[idx], 0, sizeof( worldTable_t ) );
			g_worldTable[idx].idx = idx;
			g_worldTableNumber++;
		}
		else if( !stricmp( token, "NAME" ) )
		{
			strcpy ( g_worldTable[idx].name, NextArg() );
		}
		else if( !stricmp( token, "2DWORLD_FILE_NAME" ) )
		{
			strcpy ( g_worldTable[idx].world2DFileName, NextArg() );
		}
		else if( !stricmp( token, "MONSTER_SPAWN_TABLE" ) )
		{
			strcpy ( g_worldTable[idx].monsterSpawnTableFileName, NextArg() );
		}
		else if( !stricmp( token, "MONSTER_SPAWN_NUMBER" ) )
		{
			g_worldTable[idx].monsterSpawnNumber = atoi( NextArg() );
		}
		else if( !stricmp( token, "START_POSITION" ) )
		{
			g_worldTable[idx].startPosition[0] = atof( NextArg() );
			g_worldTable[idx].startPosition[1] = atof( NextArg() );
			g_worldTable[idx].startPosition[2] = atof( NextArg() );
		}
		else if( !stricmp( token, "START_YAW" ) )
		{
			g_worldTable[idx].startYaw = atof( NextArg() );
		}
		else if( !stricmp( token, "GATE_NUMBER" ) )
		{
			g_worldTable[idx].gateNumber = atoi( NextArg() );
		}
		else if( !stricmp( token, "GATE_IDX" ) )
		{
			for (i=0; i<g_worldTable[idx].gateNumber; i++)
				g_worldTable[idx].gateIdx[i] = atoi( NextArg() );
		}
		else if( !stricmp( token, "SPAWN_POSITION" ) )
		{
			for (i=0; i<g_worldTable[idx].gateNumber; i++)
			{
				g_worldTable[idx].spawnPos[i][0] = atof( NextArg() );
				g_worldTable[idx].spawnPos[i][1] = atof( NextArg() );
				g_worldTable[idx].spawnPos[i][2] = atof( NextArg() );
			}
		}
		else if( !stricmp( token, "SPAWN_YAW" ) )
		{
			for (i=0; i<g_worldTable[idx].gateNumber; i++)
				g_worldTable[idx].spawnYaw[i] = atof( NextArg() );
		}
		else if( !stricmp( token, "LINKED_WORLD_IDX" ) )
		{
			for (i=0; i<g_worldTable[idx].gateNumber; i++)
				g_worldTable[idx].linkedWorldIdx[i] = atoi( NextArg() );
		}
		else if( !stricmp( token, "LINKED_GATE_IDX" ) )
		{
			for (i=0; i<g_worldTable[idx].gateNumber; i++)
				g_worldTable[idx].linkedGateIdx[i] = atoi( NextArg() );
		}
	}

	fclose(fp);
	
	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return true;
}

int GTH_Load2DWorld()
{
	char filename[64];
	int i;
	int localWorldIdx;

	for (i=0; i<g_worldTableNumber; i++)
	{
		
		if ( !GTH_IsLocalServiceWorld( g_worldTable[i].idx ) ) 
		{
			continue;
		}

		localWorldIdx = GTH_GetLocalWorldIdx( g_worldTable[i].idx );

		sprintf(filename, "worlds/%s", g_worldTable[i].world2DFileName);
		if ( !AI_LoadCollisionMap( filename, &g_cmap[localWorldIdx] ) )
		{
			g_logSystem->Write("%s - 2D Map Data Load Failure!!", filename);
			MessageBox(g_hWnd, filename, "Error!", MB_OK);
			return 0;
		}
	}

	return 1;
}

void GTH_Free2DWorld()
{
	int i;

	for (i=0; i<g_config.serviceWorldNumber; i++)
	{
		free( g_cmap[i].buffer );
	}
}


int GTH_World_GetGateIdx(int sourceIdx, int targetIdx)
{
	int i;

	if (sourceIdx < 0 || sourceIdx >= MAX_WORLDS) 
		return -1;

	for (i=0; i<g_worldTable[sourceIdx].gateNumber; i++)
	{
		if (g_worldTable[sourceIdx].linkedWorldIdx[i] == targetIdx )
			return g_worldTable[sourceIdx].gateIdx[i];
	}

	return -1;
}
