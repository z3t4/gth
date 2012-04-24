
#include "../global.h"




void GTH_Zone_Init()
{
	int x, y, zoneIdx;
	int localWorldIdx;

	g_zonePCListNumber = 0;
	for (localWorldIdx=0; localWorldIdx < MAX_LOCALSERVICEWORLDS; localWorldIdx++)
	{
		zoneIdx = 0;
		for (x=0 ; x<ZONE_DIV; x++)
		{
			for (y=0; y<ZONE_DIV; y++, zoneIdx++)
			{
				g_zone[localWorldIdx][zoneIdx].localWorldIdx = localWorldIdx;
				g_zone[localWorldIdx][zoneIdx].zoneIdx = zoneIdx;      

				g_zone[localWorldIdx][zoneIdx].pcNumber = 0;
				memset(g_zone[localWorldIdx][zoneIdx].pcList, 0, MAX_ZONE_PCS * sizeof(int));

				g_zone[localWorldIdx][zoneIdx].monNumber = 0;
				memset(g_zone[localWorldIdx][zoneIdx].monList, 0, MAX_ZONE_MONSTERS * sizeof(int));

				g_zone[localWorldIdx][zoneIdx].npcNumber = 0;
				memset(g_zone[localWorldIdx][zoneIdx].npcList, 0, MAX_ZONE_NPCS * sizeof(int));

				g_zone[localWorldIdx][zoneIdx].objectNumber = 0;
				memset(g_zone[localWorldIdx][zoneIdx].objectList, 0, MAX_ZONE_OBJECTS * sizeof(int));
			}
		}
	}
}


int GTH_Zone_GetZoneIdx(vec3_t position)
{
	int posX, posY;
	int zoneX, zoneY;
	int zoneIdx;

	posX = (int) position[0] / ZONE_GRIDSCALE;
	posY = (int) position[1] / ZONE_GRIDSCALE;

	zoneX = (int) posX / ZONE_UNIT;
	zoneY = (int) posY / ZONE_UNIT;

	if (zoneX >= ZONE_DIV || zoneY >= ZONE_DIV)
		return -1;

	if( zoneX < 0.0f || zoneY < 0.0f ) 
		return -1;

	zoneIdx = zoneX * ZONE_DIV + zoneY;

	return zoneIdx;
}

int GTH_Zone_UpdateCurrentZone(entityType_t entType, int entIdx, int worldIdx, int preZoneIdx, vec3_t position)
{
	int curZoneIdx;

	curZoneIdx = GTH_Zone_GetZoneIdx(position);

	if ( curZoneIdx == preZoneIdx ) 
		return curZoneIdx;
	else if ( curZoneIdx == -1 ) 
	{
		if( entType == ENTITY_PC )
		{
			g_logSystem->Write("Zone Range Error(name: %s, worldIdx: %d, pos: %f, %f, %f)", 
				g_pc[entIdx].name, g_pc[entIdx].worldIdx, g_pc[entIdx].position[0], g_pc[entIdx].position[1], g_pc[entIdx].position[2]);
			vec_copy( g_worldTable[g_pc[entIdx].worldIdx].startPosition, g_pc[entIdx].reliablePosition );
			vec_copy( g_pc[entIdx].reliablePosition, g_pc[entIdx].position );
			curZoneIdx = GTH_Zone_GetZoneIdx( g_pc[entIdx].position );
		}
	}

	
	
	if( preZoneIdx >= 0 ) GTH_Zone_DeleteEntityList(worldIdx, preZoneIdx, entType, entIdx);

	
	GTH_Zone_AddEntityList(worldIdx, curZoneIdx, entType, entIdx);
	return curZoneIdx;
}


int GTH_Zone_AddEntityList(int worldIdx, int zoneIdx, entityType_t entType, int entIdx)
{
	int number;
	vec3_t pos;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	if (localWorldIdx < 0) return 0;

	if( zoneIdx < 0 || localWorldIdx < 0 || entIdx < 0 )
	{
		if( entType == ENTITY_PC )
		{
			vec_copy( g_pc[entIdx].position, pos );
		} else if( entType == ENTITY_MONSTER || entType == ENTITY_NPC )
		{
			vec_copy( g_monster[entIdx].position, pos );
		} else if( entType == ENTITY_OBJECT )
		{
			vec_copy( g_object[entIdx].position, pos );
		} 

		g_logSystem->Write( "zoneIdx : %d, worldIdx : %d, localWorldIdx: %d, entIdx : %d, entType : %d, pos(%.2f,%.2f,%.2f)",
			zoneIdx, worldIdx, localWorldIdx, entIdx, (int)entType, pos[0], pos[1], pos[2] );
		g_logSystem->Update();
	}

	if (entType == ENTITY_PC) 
	{
		number = g_zone[localWorldIdx][zoneIdx].pcNumber;
		if (number >= MAX_ZONE_PCS) 
			return 0;	
		g_zone[localWorldIdx][zoneIdx].pcList[number] = entIdx;
		g_zone[localWorldIdx][zoneIdx].pcNumber++;
	} 
	else if (entType == ENTITY_MONSTER)
	{
		number = g_zone[localWorldIdx][zoneIdx].monNumber;
		if (number >= MAX_ZONE_MONSTERS)
			return 0;
		g_zone[localWorldIdx][zoneIdx].monList[number] = entIdx;
		g_zone[localWorldIdx][zoneIdx].monNumber++;
	}
	else if (entType == ENTITY_NPC)
	{
		number = g_zone[localWorldIdx][zoneIdx].npcNumber;
		if (number >= MAX_ZONE_NPCS)
			return 0;
		g_zone[localWorldIdx][zoneIdx].npcList[number] = entIdx;
		g_zone[localWorldIdx][zoneIdx].npcNumber++;
	}
	else if (entType == ENTITY_OBJECT)
	{
		number = g_zone[localWorldIdx][zoneIdx].objectNumber;
		if (number >= MAX_ZONE_OBJECTS)
			return 0;
		g_zone[localWorldIdx][zoneIdx].objectList[number] = entIdx;
		g_zone[localWorldIdx][zoneIdx].objectNumber++;
	}

	return 1;
}


int GTH_Zone_DeleteEntityList(int worldIdx, int zoneIdx, entityType_t entType, int entIdx)
{
	int i;
	int number;
	int success ;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	if (localWorldIdx < 0) return 0;

	success = 0;
	if (entType == ENTITY_PC)
	{
		number = g_zone[localWorldIdx][zoneIdx].pcNumber;
		for (i=0; i<number; i++)
		{
			if (g_zone[localWorldIdx][zoneIdx].pcList[i] == entIdx)
			{
				if (i != number-1)
					g_zone[localWorldIdx][zoneIdx].pcList[i] = g_zone[localWorldIdx][zoneIdx].pcList[number-1];
				g_zone[localWorldIdx][zoneIdx].pcNumber--;
				success = 1;
				break;
			}
		}
	}
	else if (entType == ENTITY_MONSTER)
	{
		number = g_zone[localWorldIdx][zoneIdx].monNumber;
		for (i=0; i<number; i++)
		{
			if (g_zone[localWorldIdx][zoneIdx].monList[i] == entIdx)
			{
				if (i != number-1)
					g_zone[localWorldIdx][zoneIdx].monList[i] = g_zone[localWorldIdx][zoneIdx].monList[number-1];
				g_zone[localWorldIdx][zoneIdx].monNumber--;
				success = 1;
				break;
			}
		}
	}
	else if (entType == ENTITY_NPC)
	{
		number = g_zone[localWorldIdx][zoneIdx].npcNumber;
		for (i=0; i<number; i++)
		{
			if (g_zone[localWorldIdx][zoneIdx].npcList[i] == entIdx)
			{
				if (i != number-1)
					g_zone[localWorldIdx][zoneIdx].npcList[i] = g_zone[localWorldIdx][zoneIdx].npcList[number-1];
				g_zone[localWorldIdx][zoneIdx].npcNumber--;
				success = 1;
				break;
			}
		}
	}
	else if (entType == ENTITY_OBJECT)
	{
		number = g_zone[localWorldIdx][zoneIdx].objectNumber;
		for (i=0; i<number; i++)
		{
			if (g_zone[localWorldIdx][zoneIdx].objectList[i] == entIdx)
			{
				if (i != number-1)
					g_zone[localWorldIdx][zoneIdx].objectList[i] = g_zone[localWorldIdx][zoneIdx].objectList[number-1];
				g_zone[localWorldIdx][zoneIdx].objectNumber--;
				success = 1;
				break;
			}
		}
	}

	return success; 
}








int GTH_Zone_ScanPCList(int localWorldIdx, vec3_t position, int scanDepth)
{
	int posX, posY;
	int zoneX, zoneY;
	int pX, pY;
	int x, y;
	int i;
	float distance;
	int zoneIdx;
	int myZoneIdx;

	posX = (int) position[0] / ZONE_GRIDSCALE;
	posY = (int) position[1] / ZONE_GRIDSCALE;

	zoneX = (int) posX / ZONE_UNIT;
	zoneY = (int) posY / ZONE_UNIT;

	if (zoneX >= ZONE_DIV || zoneY >= ZONE_DIV)
		return -1;

	g_zonePCListNumber = 0;


	zoneIdx = zoneX * ZONE_DIV + zoneY;
	myZoneIdx = zoneIdx; 
	for (i=0; i<g_zone[localWorldIdx][zoneIdx].pcNumber; i++)
	{
		distance = GTH_GetDistance2D(position, g_pc[ g_zone[localWorldIdx][zoneIdx].pcList[i] ].position);
		if (distance <= MAX_SEEINGDISTANCE) 
		{
			g_zonePCList[g_zonePCListNumber].entityType = ENTITY_PC;
			g_zonePCList[g_zonePCListNumber].idx = g_zone[localWorldIdx][zoneIdx].pcList[i];
			g_zonePCList[g_zonePCListNumber].distance = distance;
			g_zonePCListNumber++;
			

		}
	}
	if ( g_zonePCListNumber >= MAX_LIMIT_PCLIST ) return g_zonePCListNumber;


	scanDepth = 1;
	for (x=-scanDepth; x<=scanDepth; x++)
	{
		pX = zoneX + x;
		if (pX < 0 || pX > ZONE_DIV - 1)
			continue;

		for (y=-scanDepth; y<=scanDepth; y++)
		{
			pY = zoneY + y;
			if (pY < 0 || pY > ZONE_DIV - 1)
				continue;

			zoneIdx = pX * ZONE_DIV + pY;

			if ( zoneIdx == myZoneIdx ) 
				continue;

			
			for (i=0; i<g_zone[localWorldIdx][zoneIdx].pcNumber; i++)
			{
				distance = GTH_GetDistance2D(position, g_pc[ g_zone[localWorldIdx][zoneIdx].pcList[i] ].position);
				if (distance <= MAX_SEEINGDISTANCE) 
				{
					g_zonePCList[g_zonePCListNumber].entityType = ENTITY_PC;
					g_zonePCList[g_zonePCListNumber].idx = g_zone[localWorldIdx][zoneIdx].pcList[i];
					g_zonePCList[g_zonePCListNumber].distance = distance;
					g_zonePCListNumber++;
					

				}
			}
		}
		if ( g_zonePCListNumber >= MAX_LIMIT_PCLIST ) return g_zonePCListNumber;
	}


	scanDepth = 2;
	for (x=-scanDepth; x<=scanDepth; x++)
	{
		if ( x > -scanDepth || x < scanDepth )	
			continue;

		pX = zoneX + x;
		if (pX < 0 || pX > ZONE_DIV - 1)
			continue;

		for (y=-scanDepth; y<=scanDepth; y++)
		{
			if ( y > -scanDepth || y < scanDepth )	
				continue;

			pY = zoneY + y;
			if (pY < 0 || pY > ZONE_DIV - 1)
				continue;

			zoneIdx = pX * ZONE_DIV + pY;

			
			for (i=0; i<g_zone[localWorldIdx][zoneIdx].pcNumber; i++)
			{
				distance = GTH_GetDistance2D(position, g_pc[ g_zone[localWorldIdx][zoneIdx].pcList[i] ].position);
				if (distance <= MAX_SEEINGDISTANCE) 
				{
					g_zonePCList[g_zonePCListNumber].entityType = ENTITY_PC;
					g_zonePCList[g_zonePCListNumber].idx = g_zone[localWorldIdx][zoneIdx].pcList[i];
					g_zonePCList[g_zonePCListNumber].distance = distance;
					g_zonePCListNumber++;
				}
				
				if ( g_zonePCListNumber >= MAX_SCAN_PCLIST ) return g_zonePCListNumber;
			}
		}
	}

	return g_zonePCListNumber;
}



int GTH_Zone_ScanMonsterList(int localWorldIdx, vec3_t position, int scanDepth)
{
	int posX, posY;
	int zoneX, zoneY;
	int pX, pY;
	int x, y;
	int i;
	float distance;
	int zoneIdx;

	posX = (int) position[0] / ZONE_GRIDSCALE;
	posY = (int) position[1] / ZONE_GRIDSCALE;

	zoneX = (int) posX / ZONE_UNIT;
	zoneY = (int) posY / ZONE_UNIT;

	if (zoneX >= ZONE_DIV || zoneY >= ZONE_DIV)
		return -1;

	g_zoneMonsterListNumber = 0;
	for (x=-scanDepth; x<=scanDepth; x++)
	{
		pX = zoneX + x;
		if (pX < 0 || pX > ZONE_DIV - 1)
			continue;

		for (y=-scanDepth; y<=scanDepth; y++)
		{
			pY = zoneY + y;
			if (pY < 0 || pY > ZONE_DIV - 1)
				continue;

			zoneIdx = pX * ZONE_DIV + pY;

			
			for (i=0; i<g_zone[localWorldIdx][zoneIdx].monNumber; i++)
			{
				distance = GTH_GetDistance2D(position, g_monster[ g_zone[localWorldIdx][zoneIdx].monList[i] ].position);
				if (distance <= MAX_SEEINGDISTANCE) 
				{
					g_zoneMonsterList[g_zoneMonsterListNumber].entityType = ENTITY_MONSTER;
					g_zoneMonsterList[g_zoneMonsterListNumber].idx = g_zone[localWorldIdx][zoneIdx].monList[i];
					g_zoneMonsterList[g_zoneMonsterListNumber].distance = distance;
					g_zoneMonsterListNumber++;
					
					if ( g_zoneMonsterListNumber >= MAX_SCAN_MONSTERLIST ) return g_zoneMonsterListNumber;
				}
			}
		}
	}
	return g_zoneMonsterListNumber;
}

int GTH_Zone_ScanNPCList(int localWorldIdx, vec3_t position, int scanDepth)
{
	int posX, posY;
	int zoneX, zoneY;
	int pX, pY;
	int x, y;
	int i;
	float distance;
	int zoneIdx;

	posX = (int) position[0] / ZONE_GRIDSCALE;
	posY = (int) position[1] / ZONE_GRIDSCALE;

	zoneX = (int) posX / ZONE_UNIT;
	zoneY = (int) posY / ZONE_UNIT;

	if (zoneX >= ZONE_DIV || zoneY >= ZONE_DIV)
		return -1;

	g_zoneNPCListNumber = 0;
	for (x=-scanDepth; x<=scanDepth; x++)
	{
		pX = zoneX + x;
		if (pX < 0 || pX > ZONE_DIV - 1)
			continue;

		for (y=-scanDepth; y<=scanDepth; y++)
		{
			pY = zoneY + y;
			if (pY < 0 || pY > ZONE_DIV - 1)
				continue;

			zoneIdx = pX * ZONE_DIV + pY;

			
			for (i=0; i<g_zone[localWorldIdx][zoneIdx].npcNumber; i++)
			{
				distance = GTH_GetDistance2D(position, g_monster[ g_zone[localWorldIdx][zoneIdx].npcList[i] ].position);
				if (distance <= MAX_SEEINGDISTANCE) 
				{
					g_zoneNPCList[g_zoneNPCListNumber].entityType = ENTITY_NPC;
					g_zoneNPCList[g_zoneNPCListNumber].idx = g_zone[localWorldIdx][zoneIdx].npcList[i];
					g_zoneNPCList[g_zoneNPCListNumber].distance = distance;
					g_zoneNPCListNumber++;
					
					if ( g_zoneNPCListNumber >= MAX_SCAN_NPCLIST ) return g_zoneNPCListNumber;
				}
			}
		}
	}
	return g_zoneNPCListNumber;
}






int GTH_Zone_ScanObjectList(int localWorldIdx, vec3_t position, int scanDepth)
{
	int posX, posY;
	int zoneX, zoneY;
	int pX, pY;
	int x, y;
	int i;
	float distance;
	int zoneIdx;

	posX = (int) position[0] / ZONE_GRIDSCALE;
	posY = (int) position[1] / ZONE_GRIDSCALE;

	zoneX = (int) posX / ZONE_UNIT;
	zoneY = (int) posY / ZONE_UNIT;

	if (zoneX >= ZONE_DIV || zoneY >= ZONE_DIV)
		return -1;

	g_zoneObjectListNumber = 0;
	for (x=-scanDepth; x<=scanDepth; x++)
	{
		pX = zoneX + x;
		if (pX < 0 || pX > ZONE_DIV - 1)
			continue;

		for (y=-scanDepth; y<=scanDepth; y++)
		{
			pY = zoneY + y;
			if (pY < 0 || pY > ZONE_DIV - 1)
				continue;

			zoneIdx = pX * ZONE_DIV + pY;

			
			for (i=0; i<g_zone[localWorldIdx][zoneIdx].objectNumber; i++)
			{
				distance = GTH_GetDistance2D(position, g_object[ g_zone[localWorldIdx][zoneIdx].objectList[i] ].position);
				if (distance <= MAX_SEEINGDISTANCE) 
				{
					g_zoneObjectList[g_zoneObjectListNumber].entityType = ENTITY_OBJECT;
					g_zoneObjectList[g_zoneObjectListNumber].idx = g_zone[localWorldIdx][zoneIdx].objectList[i];
					g_zoneObjectList[g_zoneObjectListNumber].distance = distance;
					g_zoneObjectListNumber++;
					
					if ( g_zoneObjectListNumber >= MAX_SCAN_OBJECTLIST ) return g_zoneObjectListNumber;
				}
			}
		}
	}
	return g_zoneObjectListNumber;
}


int GTH_Zone_ScanEntityList(int localWorldIdx, vec3_t position, int scanDepth)
{
	int posX, posY;
	int zoneX, zoneY;
	int pX, pY;
	int x, y;
	int i;
	float distance;
	int zoneIdx;

	posX = (int) position[0] / ZONE_GRIDSCALE;
	posY = (int) position[1] / ZONE_GRIDSCALE;

	zoneX = (int) posX / ZONE_UNIT;
	zoneY = (int) posY / ZONE_UNIT;

	if (zoneX > ZONE_DIV - 1 || zoneY > ZONE_DIV - 1)
		return -1;

	g_zoneEntityListNumber = 0;
	for (x=-scanDepth; x<=scanDepth; x++)
	{
		pX = zoneX + x;
		if (pX < 0 || pX > ZONE_DIV - 1)
			continue;

		for (y=-scanDepth; y<=scanDepth; y++)
		{
			pY = zoneY + y;
			if (pY < 0 || pY > ZONE_DIV - 1)
				continue;

			zoneIdx = pX * ZONE_DIV + pY;

			
			for (i=0; i<g_zone[localWorldIdx][zoneIdx].pcNumber; i++)
			{
				distance = GTH_GetDistance2D(position, g_pc[ g_zone[localWorldIdx][zoneIdx].pcList[i] ].position);
				if (distance <= MAX_SEEINGDISTANCE) 
				{
					g_zoneEntityList[g_zoneEntityListNumber].entityType = ENTITY_PC;
					g_zoneEntityList[g_zoneEntityListNumber].idx = g_zone[localWorldIdx][zoneIdx].pcList[i];
					g_zoneEntityList[g_zoneEntityListNumber].distance = distance;
					g_zoneEntityListNumber++;
					
					if ( g_zoneEntityListNumber >= MAX_SCAN_ENTITYLIST ) return g_zoneEntityListNumber;
				}
			}

			
			for (i=0; i<g_zone[localWorldIdx][zoneIdx].monNumber; i++)
			{
				distance = GTH_GetDistance2D(position, g_monster[ g_zone[localWorldIdx][zoneIdx].monList[i] ].position);
				if (distance <= MAX_SEEINGDISTANCE) 
				{
					g_zoneEntityList[g_zoneEntityListNumber].entityType = ENTITY_MONSTER;
					g_zoneEntityList[g_zoneEntityListNumber].idx = g_zone[localWorldIdx][zoneIdx].monList[i];
					g_zoneEntityList[g_zoneEntityListNumber].distance = distance;
					g_zoneEntityListNumber++;
					
					if ( g_zoneEntityListNumber >= MAX_SCAN_ENTITYLIST ) return g_zoneEntityListNumber;
				}
			}

			
			for (i=0; i<g_zone[localWorldIdx][zoneIdx].npcNumber; i++)
			{
				distance = GTH_GetDistance2D(position, g_monster[ g_zone[localWorldIdx][zoneIdx].npcList[i] ].position);
				if (distance <= MAX_SEEINGDISTANCE) 
				{
					g_zoneEntityList[g_zoneEntityListNumber].entityType = ENTITY_NPC;
					g_zoneEntityList[g_zoneEntityListNumber].idx = g_zone[localWorldIdx][zoneIdx].npcList[i];
					g_zoneEntityList[g_zoneEntityListNumber].distance = distance;
					g_zoneEntityListNumber++;
					
					if ( g_zoneEntityListNumber >= MAX_SCAN_ENTITYLIST ) return g_zoneEntityListNumber;
				}
			}

			
		}
	}
	return g_zoneEntityListNumber;
}

int GTH_CompareForQsort( const void *arg1, const void *arg2 )
{
	int ret;
	float dist1, dist2;
	
	dist1 = ((i3zoneScanList_t *) arg1)->distance ;
	dist2 = ((i3zoneScanList_t *) arg2)->distance ;

	if (dist1 > dist2)
		ret = 1;
	else if (dist1 < dist2)
		ret = -1;
	else
		ret = 0;

	return ret;
}

void GTH_GetEntityListInValidRange(int worldIdx, vec3_t pos, int zoneSize )
{
	int i;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	if (localWorldIdx < 0) 
	{
		g_logSystem->Write("Critical Error1: World(%d) Can not zoning to LocalWorld.", worldIdx);
		return ;
	}

	
	GTH_Zone_ScanEntityList(localWorldIdx, pos, zoneSize );

	
	if (g_zoneEntityListNumber > MAX_LIMIT_ENTITYLIST)
	{
		qsort(g_zoneEntityList, g_zoneEntityListNumber, sizeof(i3zoneScanList_t), GTH_CompareForQsort);
		g_zoneEntityListNumber = MAX_LIMIT_ENTITYLIST;
	}

	g_zonePCListNumber = 0;
	g_zoneMonsterListNumber = 0;
	g_zoneNPCListNumber = 0;
	g_zoneObjectListNumber = 0;
	for (i=0; i<g_zoneEntityListNumber; i++)
	{
		
		if (g_zoneEntityList[i].entityType == ENTITY_PC)
		{
			
			if( !g_pc[g_zoneEntityList[i].idx].ready ) continue;

			g_zonePCList[g_zonePCListNumber].entityType = ENTITY_PC;
			g_zonePCList[g_zonePCListNumber].idx = g_zoneEntityList[i].idx;
			g_zonePCList[g_zonePCListNumber].distance = g_zoneEntityList[i].distance;
			g_zonePCListNumber++;
		}

		
		if (g_zoneEntityList[i].entityType == ENTITY_MONSTER)
		{
			if( g_monster[g_zoneEntityList[i].idx].aliveState == MON_ALIVE_STATE_DIE ) continue;

			g_zoneMonsterList[g_zoneMonsterListNumber].entityType = ENTITY_MONSTER;
			g_zoneMonsterList[g_zoneMonsterListNumber].idx = g_zoneEntityList[i].idx;
			g_zoneMonsterList[g_zoneMonsterListNumber].distance = g_zoneEntityList[i].distance;
			g_zoneMonsterListNumber++;
		}

		
		if (g_zoneEntityList[i].entityType == ENTITY_NPC)
		{
			if( g_monster[g_zoneEntityList[i].idx].aliveState == MON_ALIVE_STATE_DIE ) continue;

			g_zoneNPCList[g_zoneNPCListNumber].idx = g_zoneEntityList[i].idx;
			g_zoneNPCList[g_zoneNPCListNumber].distance = g_zoneEntityList[i].distance;
			g_zoneNPCListNumber++;
		}

	}
}



void GTH_GetPCListInValidRange(int worldIdx, vec3_t pos, int zoneSize )
{
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	if (localWorldIdx < 0) 
	{
		g_logSystem->Write("Critical Error2: World(%d) Can not zoning to LocalWorld.", worldIdx);
		return ;
	}

	
	GTH_Zone_ScanPCList(localWorldIdx, pos, zoneSize );

	
	if (g_zonePCListNumber > MAX_LIMIT_PCLIST)
	{
		qsort(g_zonePCList, g_zonePCListNumber, sizeof(i3zoneScanList_t), GTH_CompareForQsort);
		g_zonePCListNumber = MAX_LIMIT_PCLIST;
	}
}

void GTH_GetObjectListInValidRange(int worldIdx, vec3_t pos, int zoneSize )
{
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	if (localWorldIdx < 0) 
	{
		g_logSystem->Write("Critical Error3: World(%d) Can not zoning to LocalWorld.", worldIdx);
		return ;
	}

	
	GTH_Zone_ScanObjectList(localWorldIdx, pos, zoneSize );

	
	
	if (g_zoneObjectListNumber > MAX_LIMIT_OBJECTLIST)
	{
		qsort(g_zoneObjectList, g_zoneObjectListNumber, sizeof(i3zoneScanList_t), GTH_CompareForQsort);
		g_zoneObjectListNumber = MAX_LIMIT_OBJECTLIST;
	}
}

void GTH_GetMonsterListInValidRange(int worldIdx, vec3_t pos, int zoneSize )
{
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	if (localWorldIdx < 0) 
	{
		g_logSystem->Write("Critical Error4: World(%d) Can not zoning to LocalWorld.", worldIdx);
		return ;
	}

	
	GTH_Zone_ScanMonsterList(localWorldIdx, pos, zoneSize );

	
	if (g_zoneMonsterListNumber > MAX_LIMIT_MONSTERLIST )
	{
		qsort(g_zoneMonsterList, g_zoneMonsterListNumber, sizeof(i3zoneScanList_t), GTH_CompareForQsort);
		g_zoneMonsterListNumber = MAX_LIMIT_MONSTERLIST;
	}
}

void GTH_GetNPCListInValidRange(int worldIdx, vec3_t pos, int zoneSize )
{
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	if (localWorldIdx < 0) 
	{
		g_logSystem->Write("Critical Error5: World(%d) Can not zoning to LocalWorld.", worldIdx);
		return ;
	}
	
	GTH_Zone_ScanNPCList(localWorldIdx, pos, zoneSize );

	
	if (g_zoneNPCListNumber > MAX_LIMIT_NPCLIST)
	{
		qsort(g_zoneNPCList, g_zoneNPCListNumber, sizeof(i3zoneScanList_t), GTH_CompareForQsort);
		g_zoneNPCListNumber = MAX_LIMIT_NPCLIST;
	}
}


