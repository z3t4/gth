  #include "../global.h"


int AI_SetTimer( monsterCharacter_t* monster, int thinkTime )
{
	
	
	monster->ai.thinkTime = thinkTime + GTH_Randomize( 1000 ) - 500;

	return( true );
}


int AI_SetAIEvent( monsterCharacter_t* monster, int event )
{	if( monster->ai.aiEvent == AI_DIE ) return( 1 );
	switch( event )
	{
	case AI_MOVE :
		monster->ai.aiEvent = AI_MOVE;
		break;

	case AI_ATTACK :
		monster->ai.aiEvent = AI_ATTACK;	
		break;

		
	case AI_TRACE :
		monster->ai.aiEvent	= AI_TRACE;		
		break;

	case AI_RUNAWAY :
		monster->ai.aiEvent = AI_RUNAWAY;
		break;
	}

	
		monster->ai.OldAiEvent  = monster->ai.aiEvent;
		

	return( 1 );
}


int AI_HaveDamaged( monsterCharacter_t* monster )
{
	if( monster->damagedEntityType < 0 ) return( false );
	else
	{
		if( monster->damagedEntityIdx == monster->targetIdx &&
			monster->damagedEntityType == monster->targetType ) 
			return( false );
	}
	return( true );
}



int AI_SetTarget( monsterCharacter_t* monster, int type )
{
	switch( type )
	{
	
	case AI_DEF_DAMAGED_ENTITY :

		if( monster->damagedEntityType == monster->targetType &&
			monster->damagedEntityIdx == monster->targetIdx ) return( false );

		if( monster->damagedEntityType >= 0 )
		{
			monster->targetType = monster->damagedEntityType;
			monster->targetIdx = monster->damagedEntityIdx;
			
			AI_StopPathFinding( monster );
			AI_InterpretImmediately( monster );			
			break;
		}

		break;
	
	
	case AI_DEF_NEAR_ENTITY :

		if( monster->searchEntityType < 0 ) return( false );

		if( monster->searchEntityType == monster->targetType &&
			monster->searchEntityIdx == monster->targetIdx ) return( false );

		monster->targetType = monster->searchEntityType; 
		monster->targetIdx = monster->searchEntityIdx;
				

		
		AI_InterpretImmediately( monster );
		AI_StopPathFinding( monster );
		break;


	
	
	case AI_DEF_LESS_RA_ENTITY:		
		{						

			int localWorldIdx;

			localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );

			int RememIdx = -1;				
			
			int findcount = 0;					
			for( int entityIdx = 0; entityIdx <MAX_NUMBER_OF_MONSTER_CONTRIBUTION; entityIdx++)
			{
				if ( findcount >= monster->conInfoNumber) break;
				if ( monster->conInfo[entityIdx].entityIdx < 0) continue;
				findcount++;

				
				
				if ( monster->conInfo[entityIdx].entityIdx >= 0 && monster->conInfo[entityIdx].time != 0 )
				{
					int Idx = -1;

					
					Idx = monster->conInfo[entityIdx].entityIdx;
					
					if ( Idx >= MAX_PCS || Idx < 0) continue;
					if ( monster->worldIdx != g_pc[Idx].worldIdx) continue;					
					

					int dist = GTH_GetDistance2D( monster->position, g_pc[Idx].position);
					if( dist > g_monsterTable[monster->tableIdx].traceSize *  g_cmap[localWorldIdx].scale) 
						continue;			
					
					
					grid_t pcPos;
					AI_GetGridPosition( g_pc[Idx].position, pcPos, &g_cmap[localWorldIdx] );
					if( !AI_CheckCollision( pcPos[0], pcPos[1], &g_cmap[localWorldIdx] ) ) 
						continue;							
					

					
					
					if( !g_pc[Idx].active || !g_pc[Idx].alive  ) continue;
					
					if( g_pc[ Idx ].curRA < monster->curRA  )				
					{	
						if ( RememIdx < 0 )
							RememIdx = entityIdx;
						else
						{
							if ( g_pc[ RememIdx ].curRA > g_pc[ Idx ].curRA )
							RememIdx = entityIdx;
						}
						
					}						
				}						
			}			

			
			if ( RememIdx < 0) 
			{				
				if ( monster->searchEntityType >= 0 )
				{
					monster->targetType = monster->searchEntityType;

					if ( monster->searchEntityIdx < 0 || monster->searchEntityIdx > 10000) return false;
					monster->targetIdx = monster->searchEntityIdx;		

						
					
					
					
					
					
					if( monster->targetType == ENTITY_PC && monster->targetIdx >= 0 )
					{
						GTH_SetTrainMonster( &g_pc[monster->targetIdx], monster );
					}					
				}
				return -1;
			}
			else
			{
			
				if ( monster->conInfo[RememIdx].entityType >= 0 )
				{
					
					if ( monster->conInfo[RememIdx].entityIdx< 0 || monster->conInfo[RememIdx].entityIdx > 10000)  return false;

					
					monster->targetType = monster->conInfo[RememIdx].entityType;					
					monster->targetIdx = monster->conInfo[RememIdx].entityIdx;					

					
					
					AI_InterpretImmediately( monster );
					AI_StopPathFinding( monster );		
					
					
					if( monster->targetType == ENTITY_PC && monster->targetIdx >= 0 )
					{
						GTH_SetTrainMonster( &g_pc[monster->targetIdx], monster );
					}					
						
				}	
				return 1;
			}				

			
			
		}
		break;

	
	case AI_DEF_MORE_POINT_ENTITY:
		
		{							
			int RememIdx = -1;				
			int oldpoint = -1;
			int localWorldIdx;

			localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );

			
			int findcount = 0;					
			for( int entityIdx = 0; entityIdx <MAX_NUMBER_OF_MONSTER_CONTRIBUTION; entityIdx++)
			{
				if ( findcount >= monster->conInfoNumber) break;
				if ( monster->conInfo[entityIdx].entityIdx < 0) continue;
				findcount++;

				if ( monster->conInfo[entityIdx].entityIdx >= 0 )
				{
					int Idx = -1;

					Idx = monster->conInfo[entityIdx].entityIdx;

					if ( Idx >= MAX_PCS || Idx < 0) continue;					
					
					if( !g_pc[Idx].active || !g_pc[Idx].alive ) continue;

					if ( monster->worldIdx != g_pc[Idx].worldIdx) continue;
					
					

					
					
					grid_t pcPos;
					AI_GetGridPosition( g_pc[Idx].position, pcPos, &g_cmap[localWorldIdx] );
					if( !AI_CheckCollision( pcPos[0], pcPos[1], &g_cmap[localWorldIdx] ) ) continue;
					
					
					int dist = GTH_GetDistance2D( monster->position, g_pc[Idx].position);
					if( dist > g_monsterTable[monster->tableIdx].traceSize * g_cmap[localWorldIdx].scale ) 
						continue;

					int point = monster->conInfo[entityIdx].ContributionPoints[TOTAL_POINT];	

					if ( oldpoint < 0 )
					{
						oldpoint = point;
						RememIdx = entityIdx;
					}
					else
					{
						if ( oldpoint < point )
						{
							oldpoint = point;
							RememIdx = entityIdx;
						}
					}					
				}						
			}
			
			
			
			
			if ( RememIdx < 0) 
			{				
				if ( monster->searchEntityType >= 0 )
				{
					monster->targetType = monster->searchEntityType;					
					if ( monster->searchEntityIdx < 0 || monster->searchEntityIdx > 10000)  
						return false;
					monster->targetIdx = monster->searchEntityIdx;



				}
			}
			else
			{
			
				if ( monster->conInfo[RememIdx].entityType >= 0 )
				{
					if ( monster->conInfo[RememIdx].entityIdx < 0|| monster->conInfo[RememIdx].entityIdx > 10000) 
						return false;

					
					monster->targetType = monster->conInfo[RememIdx].entityType;					
					monster->targetIdx = monster->conInfo[RememIdx].entityIdx;					

				}				
			}				

			
			AI_InterpretImmediately( monster );
			AI_StopPathFinding( monster );		
				
		}
		break;
		
		
	}
	
	if( monster->targetType == ENTITY_PC && monster->targetIdx >= 0 )
	{
		GTH_SetTrainMonster( &g_pc[monster->targetIdx], monster );
	}

	return( true );
}

int AI_IsNearTarget( monsterCharacter_t* monster, int entity )
{
	float range1, range2;
	vec3_t pos1, pos2;
	int targetIdx;
	int forcedChange = false;

	switch( entity )
	{
	
	case AI_DEF_DAMAGED_ENTITY :
		targetIdx = monster->damagedEntityIdx;
		if( monster->damagedEntityType < 0 ) return( false );
		if( targetIdx < 0 ) return( false );

		switch( monster->damagedEntityType )
		{
		case ENTITY_PC :
			vec_copy( g_pc[targetIdx].position, pos1 );
			break;
		case ENTITY_MONSTER : case ENTITY_NPC :
			  vec_copy( g_monster[targetIdx].position, pos1 );
			break;
		}

		targetIdx = monster->targetIdx;
		if( targetIdx < 0 ) return( true );
		if( monster->targetType < 0 ) return( true );

		switch( monster->targetType )
		{
		case ENTITY_PC :
			vec_copy( g_pc[targetIdx].position, pos2 );
			break;
		case ENTITY_MONSTER : case ENTITY_NPC :
			vec_copy( g_monster[targetIdx].position, pos2 );
			break;
		}
		break;
	
	case AI_DEF_NEAR_ENTITY :
		
		if( monster->damagedEntityType >= 0 )
		{
			if( !( monster->targetType == monster->damagedEntityType &&
				monster->targetIdx == monster->damagedEntityIdx ) )
			{
				monster->searchEntityType = monster->damagedEntityType;
				monster->searchEntityIdx = monster->damagedEntityIdx;
				monster->targetIdx = monster->searchEntityIdx;
				monster->targetType = monster->searchEntityType;
				forcedChange = true;
			} 
			else
			{
				return( false );
			}
		} 
		
		else if( monster->forcedEntityType >= 0 )
		{
			if( !( monster->targetType == monster->forcedEntityType &&
				monster->targetIdx == monster->forcedEntityIdx ) )
			{
				monster->searchEntityType = monster->forcedEntityType;
				monster->searchEntityIdx = monster->forcedEntityIdx;
				monster->targetIdx = monster->searchEntityIdx;
				monster->targetType = monster->searchEntityType;
				forcedChange = true;
			}
			else
			{
				return( false );
			}
		}

		targetIdx = monster->searchEntityIdx;
		if( monster->searchEntityType < 0 ) return( false );
		if( targetIdx < 0 ) return( false );

		switch( monster->searchEntityType )
		{
		case ENTITY_PC :
			vec_copy( g_pc[targetIdx].position, pos1 );
			break;
		case ENTITY_MONSTER : case ENTITY_NPC :
			vec_copy( g_monster[targetIdx].position, pos1 );
			break;
		}

		targetIdx = monster->targetIdx;
		if( targetIdx < 0 ) return( true );
		if( monster->targetType < 0 ) return( true );

		switch( monster->targetType )
		{
		case ENTITY_PC :
			vec_copy( g_pc[targetIdx].position, pos2 );
			break;
		case ENTITY_MONSTER : case ENTITY_NPC :
			vec_copy( g_monster[targetIdx].position, pos2 );
			break;
		}
		break;
	}
	range1 = GTH_GetDistance2D( pos1, monster->position );
	range2 = GTH_GetDistance2D( pos2, monster->position );
	if( range1 < range2 || forcedChange ) return( true );

	return( false );
}


int AI_MoveToAnywhere( monsterCharacter_t* monster, int range )
{	
	

	if( monster->ai.wayPointNumber > 0 )
	{
		AI_MoveToWayPoint( monster );
		return 1;
	}

	int maxRange;
	grid_t dest, myPos;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );

	

	if( !monster->ai.isGoal )
	{
		AI_StopPathFinding( monster );
		return( 0 );
	}
	

	
	if ( TRUE == AI_ProcessOverSpawnPos(monster))
		return 0;
	
	
	
	if( GTH_Random() > 0.1f ) return( 1 );

	
	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	
	dest[0] = GTH_Randomize( 5 ) + 1;
	dest[1] = GTH_Randomize( 5 ) + 1;
	if( GTH_Random() < 0.5f ) dest[0] = -dest[0];
	if( GTH_Random() < 0.5f ) dest[1] = -dest[1];

	dest[0] += myPos[0];
	dest[1] += myPos[1];

	
	
	range *= g_cmap[localWorldIdx].scale;

	if( range > 0 )
	{
		
		maxRange = ( dest[0] - monster->ai.startPos[0] ) * ( dest[0] - monster->ai.startPos[0] );
		maxRange += ( dest[1] - monster->ai.startPos[1] ) * ( dest[1] - monster->ai.startPos[1] );
		
		if( maxRange > range * range ) return( 1 );
	}

	
	if( !AI_CheckCollision( dest[0], dest[1], &g_cmap[localWorldIdx] ) ) return( 1 );

	
	if( dest[0] == myPos[0] && dest[1] == myPos[1] ) return( 1 );

	monster->ai.curPos[0] = myPos[0];
	monster->ai.curPos[1] = myPos[1];

	
	monster->ai.goalPos[0] = dest[0];
	monster->ai.goalPos[1] = dest[1];

	
	AI_FindPath( monster );

	return( 1 );
}


int	AI_IsActiveTarget( monsterCharacter_t* monster )
{
	grid_t col;
	int localWorldIdx;

	switch( monster->targetType )
	{
	case ENTITY_PC :
		if( !g_pc[monster->targetIdx].alive ) 
			return( false );
		if( GTH_IsEtherealForm( &g_pc[monster->targetIdx] ) ) 
			return( false );

		
		if (g_PcSkill.FindHideSkill(monster->targetIdx) )
			return (false );	
		
		
		if ( monster->worldIdx != g_pc[monster->targetIdx].worldIdx)
			return (false);
		 

		localWorldIdx = GTH_GetLocalWorldIdx( g_pc[monster->targetIdx].worldIdx );
		

		AI_GetGridPosition( g_pc[monster->targetIdx].position, col, &g_cmap[localWorldIdx] );
		if( !AI_CheckCollision( col[0], col[1], &g_cmap[localWorldIdx] ) ) 
			return( false );

		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		if( !( g_monster[monster->targetIdx].aliveState == MON_ALIVE_STATE_NORMAL ||
			g_monster[monster->targetIdx].aliveState == MON_ALIVE_STATE_SPAWN ) ) 
			return( false );
		break;
	}
	return( true );
}


int	AI_RemoveTarget( monsterCharacter_t* monster )
{
	if ( monster->targetIdx < 0 || monster->targetIdx > 10000 ) 
		return 0;
	
	if( monster->targetType == ENTITY_PC && monster->targetIdx >= 0 )		
	{
		GTH_DeleteTrainMonster( &g_pc[monster->targetIdx], monster );
		
		
		
		for (int ConIdx = 0; ConIdx < MAX_NUMBER_OF_MONSTER_CONTRIBUTION; ConIdx ++)
		{	
			if ( monster->conInfo[ConIdx].entityIdx == g_pc[monster->targetIdx].idx )		
			{
				memset( &monster->conInfo[ConIdx], 0, sizeof (monsterContribution_t) );
				monster->conInfo[ConIdx].entityIdx = -1;
				
				if ( monster->conInfoNumber > 0)
					monster->conInfoNumber--;
				break;
			}

		}	
		
	}
	
	
	
	
	
	
	

	monster->targetIdx = -1;
	monster->targetType = -1;
	monster->damagedEntityIdx = -1;
	monster->damagedEntityType = -1;
	monster->searchEntityType = -1;
	monster->searchEntityIdx = -1;
	monster->forcedEntityType = -1;
	monster->forcedEntityIdx = -1;	
	
	

	return( 1 );
}


int	AI_IsWithinAttackLength( monsterCharacter_t* monster )
{
	float range, range2;
	float x, y;

	if( monster->targetType < 0 || monster->targetIdx < 0 ) return( 0 );

	switch( monster->targetType )
	{
	case ENTITY_PC :
		x = g_pc[monster->targetIdx].position[0];
		y = g_pc[monster->targetIdx].position[1];
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		x = g_monster[monster->targetIdx].position[0];
		y = g_monster[monster->targetIdx].position[1];
	}

	

	
	x -= monster->position[0];
	y -= monster->position[1];

	range = x * x + y * y;
	range2 = monster->calAttackLength;
	
	if( (-1 != monster->selectedSkillIdx ) &&
		( 0 !=monster->skillNumber ) )		
	{
		
		
		
		if ( monster->skillUsePercent[monster->selectedSkillIdx] != 0)
		{
			
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB( monster->selectedSkillIdx);
			if(pSkillTable==NULL)
			{
				TRACE("Error! pSkillTable==NULL   \n");
				return false;
			}
			
			range2 = pSkillTable->GetSkillRequire()->m_nLength;
		}




	}

	range2 = range2 * range2;
	if( range <= range2 ) return( true );
	
	return( false );
}


int	AI_CanTraceTarget( monsterCharacter_t* monster, int traceRange )
{

	float traceDist, dist;
	float x, y;

	grid_t dest;
	vec3_t wpos, tpos;

	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );

	
	
	
	
	
	
	traceDist = (float)g_monsterTable[monster->tableIdx].traceSize ;	

	if( traceDist <= 0.0f || traceDist > (float)monster->ai.pathFindSize * 0.4f ) 
		traceDist = (float)monster->ai.pathFindSize * 0.4f;
	
	traceDist = traceDist * g_cmap[localWorldIdx].scale;

	switch( monster->targetType )
	{
	case ENTITY_PC :
		x = g_pc[monster->targetIdx].position[0];
		y = g_pc[monster->targetIdx].position[1];
		vec_copy( g_pc[monster->targetIdx].position, tpos );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		x = g_monster[monster->targetIdx].position[0];
		y = g_monster[monster->targetIdx].position[1];
		vec_copy( g_monster[monster->targetIdx].position, tpos );
	}

	
	AI_GetGridPosition( tpos, dest, &g_cmap[localWorldIdx] );
	if( !AI_CheckCollision( dest[0], dest[1], &g_cmap[localWorldIdx] ) )
		return( false );

	if( monster->ai.wayPointNumber == 0 )
	{
		dist = GTH_GetDistance2D( monster->position, tpos );
		if( dist > traceDist ) 
		{
			AI_StopPathFinding( monster );
			return( false );
		}
	}
	else
	{
		dist = GTH_GetDistance2D( monster->position, tpos );
		if( dist > traceDist ) 
		{
			AI_StopPathFinding( monster );
			return( false );
		}
		
		
		
		if( monster->ai.curWayPath == -1 ) AI_GetWorldPosition( monster->ai.startPos, wpos, &g_cmap[localWorldIdx] );
		else AI_GetWorldPosition( monster->ai.wayPath[monster->ai.curWayPath], wpos, &g_cmap[localWorldIdx] );
		dist = GTH_GetDistance2D( monster->position, wpos );
		
		if( dist > traceDist ) 
		{
			AI_StopPathFinding( monster );
			return( false );
		}
	}
	
	return( true );
}


int	AI_MoveToTarget( monsterCharacter_t* monster )
{
	vec3_t tpos;
	grid_t myPos, dest;

	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	if (localWorldIdx < 0) return 0;

	switch( monster->targetType )
	{
	case ENTITY_PC :
		tpos[0] = g_pc[monster->targetIdx].position[0];
		tpos[1] = g_pc[monster->targetIdx].position[1];
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		tpos[0] = g_monster[monster->targetIdx].position[0];
		tpos[1] = g_monster[monster->targetIdx].position[1];
	}
	AI_GetGridPosition( tpos, dest, &g_cmap[localWorldIdx] );
	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	
	if ( TRUE == AI_ProcessOverSpawnPos(monster) )
		return 1;
	
	
	monster->ai.curPos[0] = myPos[0];
	monster->ai.curPos[1] = myPos[1];

	
	if( !monster->ai.isGoal ) 
	{
		if( !( monster->ai.goalPos[0] == dest[0] && monster->ai.goalPos[1] == dest[1] ) )
			AI_StopPathFinding( monster );
	}

	
	monster->ai.goalPos[0] = dest[0];
	monster->ai.goalPos[1] = dest[1];

	
	AI_FindPath( monster );
	
	if( !monster->ai.returnFlag && monster->wayPointNumber > 0 ) 
	{
		monster->ai.returnFlag = 1;
		
		
		if( monster->ai.curWayPath == -1 && monster->ai.directWayPath >= 0 ) monster->ai.directWayPath --;
	}

	return( 1 );
}

int AI_SearchTarget( monsterCharacter_t* monster, int entity, int range )
{
	int i, entityType;
	float wayDist, traceDist, recognizeDist;
	vec3_t wpos, targetPos;
	int idx;
	float nearest;
	grid_t col;

	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	if (localWorldIdx < 0) return 0;

	
	

	
	recognizeDist = (float)g_monsterTable[monster->tableIdx].recognizeSize;	
	

	if( recognizeDist <= 0.0f || recognizeDist > (float)monster->ai.pathFindSize * 0.3f ) 
		recognizeDist = (float)monster->ai.pathFindSize * 0.3f;
	
	recognizeDist = recognizeDist * g_cmap[localWorldIdx].scale;

	
	traceDist = (float)g_monsterTable[monster->tableIdx].traceSize;
	if( traceDist <= 0.0f || traceDist > (float)monster->ai.pathFindSize * 0.4f ) 
		traceDist = (float)monster->ai.pathFindSize * 0.4f;
	
	traceDist = traceDist * g_cmap[localWorldIdx].scale;

	
	
	
	wayDist = -1.0f;
	if( monster->ai.returnFlag && monster->wayPointNumber > 0 ) 
	{
		if( monster->ai.curWayPath == -1 ) AI_GetWorldPosition( monster->ai.startPos, wpos, &g_cmap[localWorldIdx] );
		else AI_GetWorldPosition( monster->ai.wayPath[monster->ai.curWayPath], wpos, &g_cmap[localWorldIdx] );
	}
	
	
	if( monster->forcedEntityType >= 0 && monster->forcedEntityIdx >= 0 )
	{
		idx = monster->forcedEntityIdx;
		entityType = monster->forcedEntityType;

		monster->forcedEntityIdx = -1;
		monster->forcedEntityType = -1;

		if( entityType == ENTITY_MONSTER || entityType == ENTITY_NPC )
		{
			if( GTH_GetDistance2D( monster->position, g_monster[idx].position ) < traceDist &&
				monster->worldIdx == g_monster[idx].worldIdx )
			{
				
				if ( g_PcSkill.FindHideSkill(idx) ) return false;
				monster->searchEntityType = entityType;
				monster->searchEntityIdx = idx;
			}
			else return false;
		}
		else
		{
			if( GTH_GetDistance2D( monster->position, g_pc[idx].position ) < traceDist &&
				monster->worldIdx == g_pc[idx].worldIdx )
			{
				
				if ( g_PcSkill.FindHideSkill(idx) ) return false;
				monster->searchEntityType = entityType;
				monster->searchEntityIdx = idx;
			}
			else return false;
		}
	}
	
	if( monster->damagedEntityType >= 0 && monster->damagedEntityIdx >= 0 )
	{
		idx = monster->damagedEntityIdx;
		entityType = monster->damagedEntityType;
		
		if( entityType == ENTITY_MONSTER || entityType == ENTITY_NPC )
		{
			if( GTH_GetDistance2D( monster->position, g_monster[idx].position ) < traceDist &&
				monster->worldIdx == g_monster[idx].worldIdx )
			{
				monster->searchEntityType = entityType;
				monster->searchEntityIdx = idx;
			} else return false;
		}
		else
		{
			if( GTH_GetDistance2D( monster->position, g_pc[idx].position ) < traceDist &&
				monster->worldIdx == g_pc[idx].worldIdx )
			{
				monster->searchEntityType = entityType;
				monster->searchEntityIdx = idx;
			} else return false;
		}
	}
	else
	{
		
		switch( entity )
		{
		case AI_DEF_ENTITY_PC :
			{
				GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
				
				if (g_zonePCListNumber < 1) return false;
				
				
				nearest = 9999999;
				idx = -1;
				for( i = 0; i < g_zonePCListNumber; i ++ )
				{
					if( !g_pc[ g_zonePCList[i].idx ].alive || !g_pc[ g_zonePCList[i].idx ].ready  ) continue;
					if( g_zonePCList[i].distance > nearest || g_zonePCList[i].distance > recognizeDist ) continue;
					
					if( GTH_IsEtherealForm( &g_pc[ g_zonePCList[i].idx ] ) ) continue;
					if( g_pc[ g_zonePCList[i].idx ].gameMaster && g_pc[ g_zonePCList[i].idx ].gmHide ) continue;

					if( !( g_pc[ g_zonePCList[i].idx ].idx == monster->damagedEntityIdx && \
							monster->damagedEntityType == ENTITY_PC ) )
					{
						
						if( g_pc[ g_zonePCList[i].idx ].numTrainMonster >= MAX_COUNT_AGGRESSIVE_TRAIN_MONSTER ) continue;
					}

					AI_GetGridPosition( g_pc[ g_zonePCList[i].idx ].position, col, &g_cmap[localWorldIdx] );
					if( !AI_CheckCollision( col[0], col[1], &g_cmap[localWorldIdx] ) ) continue;
					
					nearest = g_zonePCList[i].distance;
					idx = g_zonePCList[i].idx;					
				}	
				if (idx < 0) return false;

				
				if ( g_PcSkill.FindHideSkill(idx) ) return false;
				entityType = ENTITY_PC;		
			}
			break;
		case AI_DEF_ENTITY_MONSTER :
			{
				GTH_GetMonsterListInValidRange(monster->worldIdx, monster->position);
				if (g_zoneMonsterListNumber < 1) return false;	
				nearest = 9999999;
				idx = -1;
				for( i = 0; i < g_zoneMonsterListNumber; i ++ )
				{
					if( g_monster[ g_zoneMonsterList[i].idx ].aliveState != MON_ALIVE_STATE_NORMAL ) continue;
					if( g_zoneMonsterList[i].distance > nearest || g_zoneMonsterList[i].distance > recognizeDist ) continue;
	
					nearest = g_zoneMonsterList[i].distance;
					idx = g_zoneMonsterList[i].idx;
				}
				
				if (idx < 0) return false;
				
				entityType = ENTITY_MONSTER;
			}
			break;
		}
	}

	if( entityType == ENTITY_PC ) 
	{
		vec_copy( g_pc[idx].position, targetPos );
	}
	else 
	{
		vec_copy( g_monster[idx].position, targetPos );
	}

	
	wayDist = GTH_GetDistance2D( targetPos, monster->position );
	if( wayDist > traceDist )
	{
		monster->searchEntityType = -1;
		monster->searchEntityIdx = -1;
		return( false );
	}
	
	monster->searchEntityType = entityType;
	monster->searchEntityIdx = idx;
	return( true );
}


int	AI_MoveToWayPoint( monsterCharacter_t* monster )
{
	grid_t dest, myPos;

	vec3_t targetPos;
	int tRange;
	float dist, prange;

	int localWorldIdx;

	tRange = (int)( (float)monster->ai.pathFindSize * 0.5f );

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	
	prange = tRange * g_cmap[localWorldIdx].scale;
	prange = prange * prange;

	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	
	
	if( monster->ai.returnFlag )
	{
		if( !monster->ai.isGoal ) return( 1 );

		if( monster->ai.curWayPath == -1 )
		{
			dest[0] = monster->ai.startPos[0];
			dest[1] = monster->ai.startPos[1];
		}
		else
		{
			dest[0] = monster->ai.wayPath[monster->ai.curWayPath][0];
			dest[1] = monster->ai.wayPath[monster->ai.curWayPath][1];
		}

		if( myPos[0] == dest[0] && myPos[1] == dest[1] ) 
		{
			monster->ai.returnFlag = 0;
			AI_StopPathFinding( monster );
			return( 1 );
		}

		monster->ai.curPos[0] = myPos[0];
		monster->ai.curPos[1] = myPos[1];

		
		monster->ai.goalPos[0] = dest[0];
		monster->ai.goalPos[1] = dest[1];

		AI_GetWorldPosition( dest, targetPos, &g_cmap[localWorldIdx] );
		dist = GTH_GetDistance2D( targetPos, monster->position );

		if( dist > sqrtf( prange ) ) 
		{
			MONSTER_ForcedDelete( monster );
			return( false );
		}

		
		AI_FindPath( monster );
		return( 1 );
	}

	
	if( monster->ai.curWayPath == -1 )
	{
		
		if( !monster->ai.isGoal ) return( 1 );

		if( monster->ai.directWayPath < 0 )
		{
			monster->ai.wayPathNumber = 0;
		}
		else
		{
			
			for( int i = monster->ai.pathNumber; i >= 0; i -- )
			{
				monster->ai.wayPath[monster->ai.wayPathNumber][0] = monster->ai.path[i][0];
				monster->ai.wayPath[monster->ai.wayPathNumber][1] = monster->ai.path[i][1];
				monster->ai.wayPathNumber ++;
			}
		}
		
		
		monster->ai.directWayPath ++;
		
		if( monster->ai.directWayPath == monster->ai.wayPointNumber - 1 )
		{
			monster->ai.curWayPath = 0;
			monster->ai.directWayPath = 1;
			AI_GetGridPosition( monster->position, monster->ai.curPos, &g_cmap[localWorldIdx] );

			return 1;
		}

		AI_GetGridPosition( monster->ai.wayPoint[monster->ai.directWayPath], myPos, &g_cmap[localWorldIdx] );
		AI_GetGridPosition( monster->ai.wayPoint[monster->ai.directWayPath + 1], dest, &g_cmap[localWorldIdx] );

		monster->ai.curPos[0] = myPos[0];
		monster->ai.curPos[1] = myPos[1];

		
		monster->ai.goalPos[0] = dest[0];
		monster->ai.goalPos[1] = dest[1];

		
		AI_FindPath( monster );
	}
	else
	{
		
		
		float directionPro;
		if( GTH_Random() < 0.33f ) 
		{
			directionPro = GTH_Random();
			if( directionPro < 0.3f ) monster->ai.directWayPath = -1;
			else if( directionPro < 0.6f ) monster->ai.directWayPath = 1;
			else monster->ai.directWayPath = 0;
		}
	}
	
	return( 1 );
}


int AI_FleeToTarget( monsterCharacter_t* monster, int flag )
{
	return( 1 );
}

int AI_SearchFellow( monsterCharacter_t* monster, int range )
{
	return( 1 );
}

int AI_CheckAbilityRate( monsterCharacter_t* monster, int flag )
{
	switch( flag )
	{
	case AI_DEF_MY_RA :
		return( (int)( monster->curRA * 100.0f / monster->calMaxRA ) );
		break;
	
	case AI_DEF_MY_SA :
		return( (int)( monster->curSA * 100.0f / monster->calMaxSA ) );
		break;
	}
	return( 100 );
}

int AI_GetDistanceFromTarget( monsterCharacter_t* monster )
{
	float range;
	int x, y;
	grid_t pos, mypos;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	switch( monster->targetType )
	{
	case ENTITY_PC :
		AI_GetGridPosition( g_pc[monster->targetIdx].position, pos, &g_cmap[localWorldIdx] );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		AI_GetGridPosition( g_monster[monster->targetIdx].position, pos, &g_cmap[localWorldIdx] );
		break;
	}
	AI_GetGridPosition( monster->position, mypos, &g_cmap[localWorldIdx] );

	
	x = (float)(mypos[0] - pos[0]);
	y = (float)(mypos[1] - pos[1]);
	range = x * x + y * y;
	return( sqrtf( range ) );
}

int AI_IsWithInWayPoint( monsterCharacter_t* monster )
{
	return( 1 );
}

int AI_RunFromTarget( monsterCharacter_t* monster )
{
	float range, range2, range3, prange;
	float x, y, xx, yy;
	vec3_t wpos, dpos;
	grid_t dest, myPos;
	int localWorldIdx, tRange;
	
	tRange = (int)( (float)monster->ai.pathFindSize * 0.5f );

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	
	prange = tRange * g_cmap[localWorldIdx].scale;
	prange = prange * prange;

	switch( monster->targetType )
	{
	case ENTITY_PC :
		x = g_pc[monster->targetIdx].position[0];
		y = g_pc[monster->targetIdx].position[1];
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		x = g_monster[monster->targetIdx].position[0];
		y = g_monster[monster->targetIdx].position[1];
	}

	
	xx = x - monster->position[0];
	yy = y - monster->position[1];
	range = xx * xx + yy * yy;

	
	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	while( 1 )
	{
		

		
		
		
		
		


		
		dest[0] = GTH_Randomize( 25 );
		dest[1] = GTH_Randomize( 25 );		
		



		if( GTH_Random() < 0.5f ) dest[0] = -dest[0];
		if( GTH_Random() < 0.5f ) dest[1] = -dest[1];
		

		
		dest[0] += myPos[0];
		dest[1] += myPos[1];

		
		if( !AI_CheckCollision( dest[0], dest[1], &g_cmap[localWorldIdx] ) ) 
			continue; 
		

		AI_GetWorldPosition( dest, wpos, &g_cmap[localWorldIdx] );

		xx = x - wpos[0];
		yy = y - wpos[1];

		
		range2 = xx * xx + yy * yy;

		
		if( range2 > range ) 
		{
			
			if( monster->ai.wayPointNumber == 0 ) break;
			
			
			if( monster->ai.curWayPath == -1 ) AI_GetWorldPosition( monster->ai.startPos, dpos, &g_cmap[localWorldIdx] );
			else AI_GetWorldPosition( monster->ai.wayPath[monster->ai.curWayPath], dpos, &g_cmap[localWorldIdx] );
	
			x = wpos[0] - dpos[0];
			y = wpos[1] - dpos[1];
			
			range3 = x * x + y * y;
			
			if( range3 < prange ) break;
			
			
			

			
			else return( false );
			

		}
		
		
		
		

		
		else return( false );
		


	}

	monster->ai.curPos[0] = myPos[0];
	monster->ai.curPos[1] = myPos[1];
	
	
	monster->ai.goalPos[0] = dest[0];
	monster->ai.goalPos[1] = dest[1];
	
	
	AI_FindPath( monster );

	if( monster->ai.wayPointNumber > 0 && !monster->ai.returnFlag ) 
	{
		monster->ai.returnFlag = 1;
		
		
		if( monster->ai.curWayPath == -1 && monster->ai.directWayPath >= 0 ) monster->ai.directWayPath = -1;
	}


	return( true );
}

int AI_SetMoveType( monsterCharacter_t *monster, int type )
{
	monster->ai.moveType = type;
	monster->ai.oldMoveType = type;

	return( true );
}

int AI_HaveBoss( monsterCharacter_t* monster, int monTableIdx )
{
	int i, idx, nearest;
	int range;
	int localWorldIdx;

	monster->bossEntityIdx = -1;
	monster->bossEntityType = -1;

	GTH_GetMonsterListInValidRange(monster->worldIdx, monster->position);
	
	if (g_zoneMonsterListNumber < 1) return false;
	
	nearest = 99999;
	idx = -1;

	for( i = 0; i < g_zoneMonsterListNumber; i ++ )
	{
		if ( g_zoneMonsterList[i].distance < nearest )
		{
			if ( g_monster[ g_zoneMonsterList[i].idx ].aliveState == MON_ALIVE_STATE_NORMAL && 
				g_monster[ g_zoneMonsterList[i].idx ].tableIdx == monTableIdx )
			{
				nearest = g_zoneMonsterList[i].distance;
				idx = g_zoneMonsterList[i].idx;
			}
		}
	}
	
	if ( idx < 0 )
	{
		return false;
	}

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	range = 10 * g_cmap[localWorldIdx].scale;
	if( nearest > range ) return false;

	monster->bossEntityIdx = idx;
	monster->bossEntityType = ENTITY_MONSTER;
	
	return true;
}

int AI_MoveToBoss( monsterCharacter_t* monster )
{
	vec3_t tpos;
	grid_t myPos, dest, randDest;
	int event;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );



	
	if( !monster->ai.isGoal ) 
	{
		AI_StopPathFinding( monster );
	}

	if( monster->bossEntityIdx < 0 || monster->bossEntityType < 0 ) return false;

	switch( monster->bossEntityType )
	{
	case ENTITY_PC :
		tpos[0] = g_pc[monster->bossEntityIdx].position[0];
		tpos[1] = g_pc[monster->bossEntityIdx].position[1];
		event = g_pc[monster->bossEntityIdx].event;
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		tpos[0] = g_monster[monster->bossEntityIdx].position[0];
		tpos[1] = g_monster[monster->bossEntityIdx].position[1];
		event = g_monster[monster->bossEntityIdx].event;
		break;
	}
	AI_GetGridPosition( tpos, dest, &g_cmap[localWorldIdx] );
	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	if( event == GTH_EV_CHAR_IDLE ) return true;

	monster->ai.curPos[0] = myPos[0];
	monster->ai.curPos[1] = myPos[1];

	randDest[0] = GTH_Randomize( 4 );
	randDest[1] = GTH_Randomize( 4 );

	dest[0] += randDest[0];
	dest[1] += randDest[1];

	if( !AI_CheckCollision( dest[0], dest[1], &g_cmap[localWorldIdx] ) ) 
		return false;

	
	monster->ai.goalPos[0] = dest[0];
	monster->ai.goalPos[1] = dest[1];

	
	AI_FindPath( monster );

	return( 1 );
}

int AI_SendHelpMessage( monsterCharacter_t* monster, int monTableIdx, int range )
{

	

	int i, idx;
	int sendRange;

	int damagedType, damagedIdx;
	int targetType, targetIdx;
	int searchType, searchIdx;

	int localWorldIdx;

	damagedType = monster->damagedEntityType;
	damagedIdx = monster->damagedEntityIdx;
	targetType = monster->targetType;
	targetIdx = monster->targetIdx;

	searchType = monster->searchEntityType;
	searchIdx = monster->searchEntityIdx;

	GTH_GetMonsterListInValidRange(monster->worldIdx, monster->position);
	
	if (g_zoneMonsterListNumber < 1) return false;
	
	idx = -1;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	sendRange = range * g_cmap[localWorldIdx].scale;

	monster->searchEntityType = monster->targetType;
	monster->searchEntityIdx = monster->targetIdx;



		for( i = 0; i < g_zoneMonsterListNumber; i ++ )
	{
		if ( g_monster[ g_zoneMonsterList[i].idx ].aliveState == MON_ALIVE_STATE_NORMAL)
		{		

			if( monTableIdx >= 0 )
				if( g_monster[ g_zoneMonsterList[i].idx ].tableIdx != monTableIdx ) continue;
			
			if( g_zoneMonsterList[i].distance < sendRange )
			{
				idx = g_zoneMonsterList[i].idx;
				monster->targetType = ENTITY_MONSTER;
				monster->targetIdx = idx;			
			}
		}
	}




	monster->damagedEntityType = damagedType;
	monster->damagedEntityIdx = damagedIdx;
	monster->targetType = targetType;
	monster->targetIdx = targetIdx;

	monster->searchEntityType = searchType;
	monster->searchEntityIdx = searchIdx;

	
	


	return true;	
}


int AI_ReceiveHelpMessage( monsterCharacter_t* monster )
{
	
	
	


	
	if( monster->forcedEntityType >= 0 && monster->forcedEntityIdx >= 0 )
	{		

		int entityType;
		float wayDist, traceDist, recognizeDist;
		vec3_t wpos, targetPos;
		int idx;


		
		int localWorldIdx;
		
		localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
		if (localWorldIdx < 0) return 0;
		
		
		
		
		
		recognizeDist = (float)g_monsterTable[monster->tableIdx].recognizeSize;
		

		if( recognizeDist <= 0.0f || recognizeDist > (float)monster->ai.pathFindSize * 0.3f ) 
			recognizeDist = (float)monster->ai.pathFindSize * 0.3f;
		
		recognizeDist = recognizeDist * g_cmap[localWorldIdx].scale;
		
		
		traceDist = (float)g_monsterTable[monster->tableIdx].traceSize;
		if( traceDist <= 0.0f || traceDist > (float)monster->ai.pathFindSize * 0.4f ) 
			traceDist = (float)monster->ai.pathFindSize * 0.4f;
		
		traceDist = traceDist * g_cmap[localWorldIdx].scale;
		
		
		
		
		wayDist = -1.0f;
		if( monster->ai.returnFlag && monster->wayPointNumber > 0 ) 
		{
			if( monster->ai.curWayPath == -1 ) AI_GetWorldPosition( monster->ai.startPos, wpos, &g_cmap[localWorldIdx] );
			else AI_GetWorldPosition( monster->ai.wayPath[monster->ai.curWayPath], wpos, &g_cmap[localWorldIdx] );
		}
		
		
		idx = monster->forcedEntityIdx;
		entityType = monster->forcedEntityType;
		
		
		monster->forcedEntityIdx = -1;
		monster->forcedEntityType = -1;
		
		if( entityType == ENTITY_MONSTER || entityType == ENTITY_NPC )
		{
			if( GTH_GetDistance2D( monster->position, g_monster[idx].position ) < traceDist &&
				monster->worldIdx == g_monster[idx].worldIdx )
			{
				monster->searchEntityType = entityType;
				monster->searchEntityIdx = idx;
			}
			else return false;
		}
		else
		{
			if( GTH_GetDistance2D( monster->position, g_pc[idx].position ) < traceDist &&
				monster->worldIdx == g_pc[idx].worldIdx )
			{
				monster->searchEntityType = entityType;
				monster->searchEntityIdx = idx;
			}
			else return false;
		}
		
		
		vec_copy( g_pc[idx].position, targetPos );
		
		
		
		wayDist = GTH_GetDistance2D( targetPos, monster->position );
		
		if( wayDist > traceDist )
		{
			monster->searchEntityType = -1;
			monster->searchEntityIdx = -1;
			return( false );
		}
		
		monster->searchEntityType = entityType;
		monster->searchEntityIdx = idx;
		
		return 1;
		
	}	
	
	
	return 0;	
	
	
}




int AI_IsPartyPlayer( monsterCharacter_t *monster )
{
	int entityIdx;

	

	int findcount = 0;
	
	for( int i=0; i<MAX_NUMBER_OF_MONSTER_CONTRIBUTION; i++)
	{			
		if ( findcount >= monster->conInfoNumber) break;
		if ( monster->conInfo[i].entityIdx < 0) continue;
		findcount++;

		
		entityIdx = monster->conInfo[i].entityIdx;

		
		if ( entityIdx >= MAX_PCS ) continue; 
		
		if( !g_pc[entityIdx].active || !g_pc[entityIdx].alive ) continue;

		
		int localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
		if ( monster->worldIdx != g_pc[entityIdx].worldIdx) continue;				

		grid_t pcPos;
		AI_GetGridPosition( g_pc[entityIdx].position, pcPos, &g_cmap[localWorldIdx] );
		if( !AI_CheckCollision( pcPos[0], pcPos[1], &g_cmap[localWorldIdx] ) ) 
			continue;		
		
		int dist = GTH_GetDistance2D( monster->position, g_pc[entityIdx].position);
		if( dist > g_monsterTable[monster->tableIdx].traceSize * g_cmap[localWorldIdx].scale ) 
			continue;
		
		

		if( g_pc[ entityIdx ].organizerName[0] != 0 )
			return TRUE;
	}

	return FALSE;
}


int AI_IsAttackerLevelHigherThanMe( monsterCharacter_t *monster )
{
	int entityIdx;


	

	
	int findcount = 0;				
	for( int i=0; i<MAX_NUMBER_OF_MONSTER_CONTRIBUTION; i++)
	{
		if ( findcount >= monster->conInfoNumber) break;
		if ( monster->conInfo[i].entityIdx < 0) continue;
		findcount++;
		
		

		
		entityIdx = monster->conInfo[i].entityIdx;

		
		if( !g_pc[entityIdx].active || !g_pc[entityIdx].alive ) continue;

		
		int localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
		if ( monster->worldIdx != g_pc[entityIdx].worldIdx) continue;				
		grid_t pcPos;
		AI_GetGridPosition( g_pc[entityIdx].position, pcPos, &g_cmap[localWorldIdx] );
		if( !AI_CheckCollision( pcPos[0], pcPos[1], &g_cmap[localWorldIdx] ) ) 
			continue;		
		
		int dist = GTH_GetDistance2D( monster->position, g_pc[entityIdx].position);
		if( dist > g_monsterTable[monster->tableIdx].traceSize * g_cmap[localWorldIdx].scale ) 
			continue;
		


		
		if( g_pc[ entityIdx ].entityType == ENTITY_PC )
			
				
				return  monster->basic.level - g_pc[entityIdx].level ;								

	}

	if ( monster->conInfoNumber == 0)
	{
		return  monster->basic.level - g_pc[monster->searchEntityIdx].level ;

	}

	return FALSE;
}

int AI_GetAttackerCount( monsterCharacter_t *monster )
{




	return monster->conInfoNumber;
}



int AI_IsAtkRngLongerThanValue( monsterCharacter_t *monster, int atkRange )
{

	

	
	int findcount = 0;					
	for( int i=0; i<MAX_NUMBER_OF_MONSTER_CONTRIBUTION; i++)
	{
		if ( findcount >= monster->conInfoNumber) break;
		if ( monster->conInfo[i].entityIdx < 0) continue;
		findcount++;
		

		int entityIdx = monster->conInfo[i].entityIdx;

		
		if( !g_pc[entityIdx].active || !g_pc[entityIdx].alive ) continue;

		
		
		int localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
		if ( monster->worldIdx != g_pc[entityIdx].worldIdx) continue;				
		grid_t pcPos;
		AI_GetGridPosition( g_pc[entityIdx].position, pcPos, &g_cmap[localWorldIdx] );
		if( !AI_CheckCollision( pcPos[0], pcPos[1], &g_cmap[localWorldIdx] ) ) 
			continue;		
		
		int dist = GTH_GetDistance2D( monster->position, g_pc[entityIdx].position);
		if( dist > g_monsterTable[monster->tableIdx].traceSize * g_cmap[localWorldIdx].scale ) 
			continue;
		


		
		if( g_pc[ entityIdx ].entityType == ENTITY_PC )
			if( g_pc[ entityIdx ].calAttackLength > atkRange )
				return TRUE;
	}

	return FALSE;
}

int AI_IsAtkRngShorterThanValue( monsterCharacter_t *monster, int atkRange )
{

	
	
	
	int findcount = 0;					
	for( int i=0; i<MAX_NUMBER_OF_MONSTER_CONTRIBUTION; i++)
	{
		if ( findcount >= monster->conInfoNumber) break;
		if ( monster->conInfo[i].entityIdx < 0) continue;
		findcount++;
		
	
		int entityIdx = monster->conInfo[i].entityIdx;

		
		if ( entityIdx < 0 || entityIdx > MAX_PCS ) continue;
		
		
		if( !g_pc[entityIdx].active || !g_pc[entityIdx].alive ) continue;

		
		
		int localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
		if ( monster->worldIdx != g_pc[entityIdx].worldIdx) continue;				
		grid_t pcPos;
		AI_GetGridPosition( g_pc[entityIdx].position, pcPos, &g_cmap[localWorldIdx] );
		if( !AI_CheckCollision( pcPos[0], pcPos[1], &g_cmap[localWorldIdx] ) ) 
			continue;		
		
		int dist = GTH_GetDistance2D( monster->position, g_pc[entityIdx].position);
		if( dist > g_monsterTable[monster->tableIdx].traceSize * g_cmap[localWorldIdx].scale ) 
			continue;
		


		
		if( g_pc[ entityIdx ].entityType == ENTITY_PC )
			if( g_pc[ entityIdx ].calAttackLength < atkRange )
				return TRUE;
	}

	return FALSE;
}



int AI_RemoveSkillUsePercent( monsterCharacter_t* monster )
{
	for( int i=0; i<monster->skillNumber; i++ )
	{
		monster->skillUsePercent[i] = 0;
	}

	return 1;
}



int AI_AddSkillUsePercent( monsterCharacter_t* monster, int skillid, int percent )
{
	char szTemp[64];

	for( int i=0; i<monster->skillNumber; i++ )
	{
		if( monster->skill[i].idx == skillid )
		{
			monster->skillUsePercent[i] = monster->skillUsePercent[i] + percent;

			
			if(monster->skillUsePercent[i] < 0) monster->skillUsePercent[i] =0;

			sprintf( szTemp, "%d, AddSkillUsePercent, %d, %d\n", monster->idx, skillid, monster->skillUsePercent[i] );
			OutputDebugString( szTemp );
		}
	}

	
	

	
	

	return 1;
}

int AI_UseSkill( monsterCharacter_t* monster )
{
	int percent;
	int selected = -1;
	
	
	
	

	
	float fDefaultTime = g_timer->GetAbsoluteTime() * 1000.0f ;
	if(monster->fSkillAnimationPassTime - fDefaultTime > 0.0f  )
	{
		
		return selected;
	}

	float skill_total_percent = 0.0f;
	for( int i=0; i<monster->skillNumber; i++ )	
	{
		skill_total_percent += monster->skillUsePercent[i];
	}

	monster->selectedSkillIdx = -1;

	percent = genrand_real3() * 100.0f;

	float fSkillRatio[MAX_NUMBER_OF_MONSTER_SKILL+1];
	memset(&fSkillRatio,0,sizeof(fSkillRatio));
	
	
	fSkillRatio[MAX_NUMBER_OF_MONSTER_SKILL]=100.0f;

	
	if(skill_total_percent <= 0.0) return selected;

	float SaveRatio=0;
	for( i=0; i<monster->skillNumber; i++ )	
	{
		if(monster->skillUsePercent[i] > 0.0f)
		{
			fSkillRatio[i+1] = SaveRatio + ((float) monster->skillUsePercent[i] / skill_total_percent) * 100;
			SaveRatio+=fSkillRatio[i+1];
		}
	}

	for( i=0; i<monster->skillNumber ; i++ )	
	{
		






		if( fSkillRatio[i]  <= percent && fSkillRatio[i+1] >= percent  )
		{		
			
			if ( monster->skillCooltime[i] - fDefaultTime < 0.0f )
			{
				if( selected != -1 )
				{
					if( monster->skillUsePercent[selected] < monster->skillUsePercent[i] )
					{
						monster->selectedSkillIdx = monster->skill[i].tableIdx;												
						selected = i;	

						monster->fSkillAnimationPassTime = monster->fSkillAnimationTime[i] + fDefaultTime;
						break;
					}
				}
				else
				{
					monster->selectedSkillIdx = monster->skill[i].tableIdx;										
					selected = i;

					monster->fSkillAnimationPassTime = monster->fSkillAnimationTime[i] + fDefaultTime;
					break;
				}
			}
		}
	}


	





	if( selected == -1 )
		return selected;
	else
	{
		char szTemp[64];
		sprintf( szTemp, "%d, UseSkill, %d, %d, %d, %d\n", monster->idx, monster->selectedSkillIdx, monster->skillUsePercent[selected], percent, monster->skillCooltime[selected] );
		OutputDebugString( szTemp );

		return selected;
	}
}


int AI_HaveSelectedSkill( monsterCharacter_t* monster )
{
	
	
	

	if( monster->selectedSkillIdx == -1 )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}






int AI_IsHaveTargetTrace( monsterCharacter_t* monster )
{
	
	if ( !monster ) return false;
	
	if ( monster->ai.OldAiEvent == AI_TRACE || monster->ai.OldAiEvent == AI_ATTACK)
	{		
		return true;
	}
	return false;
	
}



int AI_MoveToSpawnPos( monsterCharacter_t* monster )
{

	if( monster->ai.wayPointNumber > 0 )
	{
		AI_MoveToWayPoint( monster );
		return 1;
	}


	grid_t myPos, dest;
	int event = 0;
	int localWorldIdx;
	memset( dest, 0, sizeof( grid_t ) );

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	
	
	if( !( monster->ai.goalPos[0] == dest[0] && monster->ai.goalPos[1] == dest[1] ) )
	{
		AI_StopPathFinding( monster );		
	}	
	

	AI_GetGridPosition( monster->Spawnposition, dest, &g_cmap[localWorldIdx] );
	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	if( event == GTH_EV_CHAR_IDLE ) 
		return true;

	monster->ai.curPos[0] = myPos[0];
	monster->ai.curPos[1] = myPos[1];

	if( !AI_CheckCollision( dest[0], dest[1], &g_cmap[localWorldIdx] ) ) 
		return false;

	
	monster->ai.goalPos[0] = dest[0];
	monster->ai.goalPos[1] = dest[1];

	
	AI_FindPath( monster );
	
	if( !monster->ai.returnFlag && monster->wayPointNumber > 0 ) 
	{
		monster->ai.returnFlag = 1;
		
		
		if( monster->ai.curWayPath == -1 && monster->ai.directWayPath >= 0 ) monster->ai.directWayPath --;
	}
	return( 1 );
}

int AI_IsOverRangeFromSpawnPos ( monsterCharacter_t* monster, int range )
{
	int localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	float CalRange;	

	CalRange = GTH_GetDistance2D( monster->Spawnposition, monster->position);

	
	if ( (float) CalRange >= (float) range )
		return true;

	return false;
}




int AI_MonsterReSpawn ( monsterCharacter_t* monster )
{	
	if ( monster == NULL) return false;	

	
	monster->aliveState = MON_ALIVE_STATE_CORPSE;
	monster->ai.aiEvent = AI_DIE;
	monster->event = GTH_EV_CHAR_DIE;
	monster->oldEvent = GTH_EV_CHAR_DIE;
	monster->deathTime = g_globalTime;

	return true;
}



int AI_GetDamage ( monsterCharacter_t* monster)
{
	if ( monster == NULL ) return 0;
	return monster->curDamage;
}



int AI_MonsterSummon( monsterCharacter_t* monster, int index, int persent)
{	
	if( index < 0 ) return -1;
	monsterCharacter_t* pmonster = NULL;
	int localWorldIdx;


	if ( persent < genrand_real3() * 100.0F )
		return -1;

	int idx, i;
	char summonerName[255];

	idx = MONSTER_GetNotActiveIdx();
	if( idx < 0 ) return -1;

	
	localWorldIdx = monster->worldIdx;

	pmonster = &g_monster[idx];
	memset( pmonster, 0, sizeof( monsterCharacter_t ) );

	pmonster->worldIdx = localWorldIdx;
	

	pmonster->idx = idx;
	pmonster->tableIdx = index;

	pmonster->spawnWorldIdx = pmonster->worldIdx;
	pmonster->spawnTableIdx = -1;
	
	pmonster->respawnTime = -1.0f;

	memcpy( &pmonster->basic, &g_monsterTable[pmonster->tableIdx], sizeof( monsterBasicTable_t ) );


	pmonster->entityType = ENTITY_MONSTER;

	
	pmonster->ai.aiType = g_monsterTable[pmonster->basic.AIScriptIdx].AIScriptIdx;
	pmonster->npcAi.aiType = -1;
	pmonster->npcTableIdx = -1;
	pmonster->npc.scriptFlag = false;
	

	
	AI_LoadScript( pmonster, g_scriptInfo.aiScript[pmonster->ai.aiType].filename );

	
	AI_PreparePathfinding( pmonster );

	pmonster->deathTime = 0;
	pmonster->worldIdx = pmonster->spawnWorldIdx;



	
	

	vec_copy(monster->position, pmonster->position);

	
	grid_t dest;

	
	
	dest[0] = GTH_Randomize( 5 ); 
	dest[1] = GTH_Randomize( 5 );
	
	
	if( GTH_Random() < 0.5f ) dest[0] = -dest[0] ;
	if( GTH_Random() < 0.5f ) dest[1] = -dest[1] ;
	
	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );		
	
	if( AI_CheckCollision( dest[0], dest[1], &g_cmap[localWorldIdx] ) ) 
		return -1;
		

	pmonster->position[0] += dest[0] * g_cmap[localWorldIdx].scale;
	pmonster->position[1] += dest[1] * g_cmap[localWorldIdx].scale;

	

	pmonster->aliveState = MON_ALIVE_STATE_SPAWN;
	pmonster->recoveryTime = MONSTER_SPAWN_ANIMATION_TIME;

	
	MONSTER_Calculate( pmonster );

	pmonster->curRA = pmonster->calMaxRA;
	pmonster->curSA = pmonster->calMaxSA;

	pmonster->totalOptNum  = 0;
	pmonster->playerOptNum = 0;
	

	pmonster->event = GTH_EV_CHAR_IDLE;
	pmonster->oldEvent = GTH_EV_CHAR_DIE;

	
	for ( i =0; i < MAX_AFFECT; i++ )
	{
		memset( &pmonster->affect[i], 0, sizeof( affect_t ) );
		pmonster->affect[i].type = AFFECT_NONE;
	}
	pmonster->affectNumber = 0;

	GTH_InitContributionPoint( pmonster );

	sprintf( summonerName, "%s", g_monster[pmonster->tableIdx].basic.name );
	g_logSystem->Write( "몬스터 소환 성공 : %s(%d) Success...pos(%.2f, %.2f, %.2f), 소환자 : (%s)",
		pmonster->basic.name, pmonster->idx, pmonster->position[0], pmonster->position[1], pmonster->position[2], summonerName );

	
	MONSTER_InitAI( pmonster );
	AI_InitMessage( pmonster );

	
	pmonster->isSummon = true;
	pmonster->summonTime = g_globalTime;
	pmonster->summonValidTime = pmonster->basic.level * 30000;

	pmonster->active = 1;
	pmonster->zoneIdx = -1;

	pmonster->zoneIdx = GTH_Zone_GetZoneIdx( pmonster->position );
	GTH_Zone_AddEntityList(pmonster->worldIdx, pmonster->zoneIdx, pmonster->entityType, pmonster->idx);

	
	
	monster->summonTime = g_globalTime; 
	

	return idx;
}

int AI_IsUsedSkill ( monsterCharacter_t* monster, int idx )
{
	if ( monster == NULL) return 1;

	

	

	
	if(!SAFE_MONSTER_SKILL_INDEX(idx))
	{
		TRACE("ERROR!!! AI_IsUsedSkill %d \n",idx);
		return false;
	}

	return monster->bUsedSkill[idx];
}

int AI_SetUsedSkill ( monsterCharacter_t* monster, int idx, int active )
{
	if ( monster == NULL) return false;

	

	
	if(!SAFE_MONSTER_SKILL_INDEX(idx))
	{
		TRACE("ERROR!!! AI_SetUsedSkill %d \n",idx);
		return false;
	}

	monster->bUsedSkill[idx] = active;
	return true;
}


int AI_SetSkillAnimationTime ( monsterCharacter_t* monster, int idx, int time )
{
	if ( monster == NULL) return false;

	if(!SAFE_MONSTER_SKILL_INDEX(idx)) return false;

	monster->fSkillAnimationTime[idx] = (float) time;
	return true;
}

int AI_IsOverDIstansAttackRangeRate ( monsterCharacter_t * monster, int rate)
{
	float range, range2;
	float x, y;

	if( monster->targetType < 0 || monster->targetIdx < 0 ) return( true );

	switch( monster->targetType )
	{
	case ENTITY_PC :
		x = g_pc[monster->targetIdx].position[0];
		y = g_pc[monster->targetIdx].position[1];
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		x = g_monster[monster->targetIdx].position[0];
		y = g_monster[monster->targetIdx].position[1];
	}	

	
	x -= monster->position[0];
	y -= monster->position[1];


	
	range = x * x + y * y;

	
	range2 = monster->calAttackLength * rate / 100;

	range2 = range2 * range2;
	if( range <= range2 ) 
		return( false );
	
	return( true );
}

int AI_IsHighVelocityThenMonster (monsterCharacter_t* monster )
{
	if ( monster == NULL ) return 0;

	float velocity;	

	if( monster->targetType < 0 || monster->targetIdx < 0 ) return( FALSE );

	switch( monster->targetType )
	{
	case ENTITY_PC :
		velocity = g_pc[monster->targetIdx].calVelocity;
		break;

	case ENTITY_MONSTER : case ENTITY_NPC :
		velocity = g_monster[monster->targetIdx].calVelocity;
		break;
	}	
	
	return velocity > monster->calVelocity + monster->calVelocity * 0.2;	
}


int AI_GetSkillElapsedTime (monsterCharacter_t* monster, int idx )
{
	if ( monster == NULL ) return -1;	

	if ( monster->skillNumber <= 0) return -1;

	if ( monster->skill[idx].idx < 0 )
		return -1;

	return (int) ( g_timer->GetAppMilliTime() - monster->skillCooltime[idx] ) * 0.001f;
}



int AI_GetSpawnElapsedTime(monsterCharacter_t* monster )
{
	if ( monster == NULL ) return -1;	

	return (int) ( g_timer->GetAppMilliTime() - monster->fSpawnElapsedTime ) * 0.001f;
}


int AI_GetTimeSkill (monsterCharacter_t* monster,int start_time,int roop_time,int roop_total)
{
	if ( monster == NULL ) return -1;	

	
	if(!monster->fAiTempValue[0])
	{
		float ftime = AI_GetSpawnElapsedTime(monster);

		
		if(ftime >= start_time && rand() % 7 == 0)
		{
			
			monster->fAiTempValue[0]=1.0f;
			monster->fAiTempValue[1]=g_timer->GetAppMilliTime();
			monster->fAiTempValue[2]++;
			return 1;
		}
	}

	
	else if(monster->fAiTempValue[2] < roop_total)
	{	
		float ftime = g_timer->GetAppMilliTime() - monster->fAiTempValue[1];

		
		if(ftime >=roop_time && rand() % 7 == 0)
		{
			
			monster->fAiTempValue[1]=g_timer->GetAppMilliTime();
			monster->fAiTempValue[2]++;
			return 1;
		}
	}

	return 0;
}



int AI_SendHelpMsg(monsterCharacter_t* monster, int monTableIdx, int range, int count)
{	
	int i, idx;
	int sendRange;

	int damagedType, damagedIdx;
	int targetType, targetIdx;
	int searchType, searchIdx;

	int localWorldIdx;

	damagedType = monster->damagedEntityType;
	damagedIdx = monster->damagedEntityIdx;
	targetType = monster->targetType;
	targetIdx = monster->targetIdx;

	searchType = monster->searchEntityType;
	searchIdx = monster->searchEntityIdx;

	GTH_GetMonsterListInValidRange(monster->worldIdx, monster->position);
	
	if (g_zoneMonsterListNumber < 1) return false;
	
	idx = -1;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	sendRange = range * g_cmap[localWorldIdx].scale;

	monster->searchEntityType = monster->targetType;
	monster->searchEntityIdx = monster->targetIdx;



	
	

	message_t message;
	memset(&message, -1, sizeof (message_t));	

	message.type		= MSG_ETC;
	message.fromIdx	= monster->idx;
	message.fromType	= monster->entityType;
	strncpy(message.fromNameV2, monster->basic.name,NAMESTRING);
	message.fromNameV2[NAMESTRING]=NULL;

	message.toType		= monster->targetType;
	message.toIdx		= monster->targetIdx;
	message.worldIdx	= monster->worldIdx;

	message.sendTime	= g_globalTime;

	
	message.receiveTime = g_globalTime;

	message.repeatNum = 0;

	
	message.data[0]	= MSG_ETC_FORCED_CHANGE_TARGET;
	message.data[1]	= monster->targetType;
	message.data[2]	= monster->targetIdx;
	

	int receivCount = 0;

	for( i = 0; i < g_zoneMonsterListNumber; i ++ )
	{
		if ( g_monster[ g_zoneMonsterList[i].idx ].aliveState == MON_ALIVE_STATE_NORMAL)
		{
			
			
			if ( g_monster[ g_zoneMonsterList[i].idx ].idx == monster->idx)
				continue;

			if ( g_monster[ g_zoneMonsterList[i].idx ].targetIdx >= 0 && 
				g_monster[ g_zoneMonsterList[i].idx ].targetType >= 0)
				continue;
			

			if( monTableIdx >= 0 )
				if( g_monster[ g_zoneMonsterList[i].idx ].tableIdx != monTableIdx ) continue;
			
			if( g_zoneMonsterList[i].distance < sendRange )
			{
				idx = g_zoneMonsterList[i].idx;
				monster->targetType = ENTITY_MONSTER;
				monster->targetIdx = idx;

				
				AI_SendMessageToMONSTER(&g_monster[ g_zoneMonsterList[i].idx ], &message);
				

				receivCount++;			

			}
		}
		if ( receivCount >= count ) 
			break;
	}
	
	

	monster->damagedEntityType = damagedType;
	monster->damagedEntityIdx = damagedIdx;
	monster->targetType = targetType;
	monster->targetIdx = targetIdx;

	monster->searchEntityType = searchType;
	monster->searchEntityIdx = searchIdx;


	
	if ( receivCount <= 0)
		return false;


	return true;	
}



int AI_GetAIEvent(monsterCharacter_t *monster )
{
	if ( monster == NULL ) return -1;		

	
	return monster->ai.aiEvent;

	
}

int AI_GetSummonElapsedTime (monsterCharacter_t* monster )
{
	if ( monster == NULL ) return -1;	
	return (int) ( g_timer->GetAppMilliTime() - monster->summonTime ) * 0.001f;
}


int AI_GetRandom(monsterCharacter_t *monster )
{
	int iRandom= (int) (GTH_Random() * 100);
	return iRandom;
}







int AI_ProcessOverSpawnPos(monsterCharacter_t *monster)
{
	if ( NULL == monster)
		return FALSE;

	int localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	int dist = GTH_GetDistance2D( monster->position, monster->Spawnposition );

	if ( localWorldIdx < 0 || localWorldIdx >= MAX_LOCALSERVICEWORLDS)
		return FALSE;
	
	
	float OverRange = (monster->basic.traceSize * g_logic.m_SpawnPosOverRate) * g_cmap[localWorldIdx].scale;

	if ( dist >=  OverRange)
	{
		monster->aliveState = MON_ALIVE_STATE_CORPSE;
		monster->ai.aiEvent = AI_DIE;
		monster->event = GTH_EV_CHAR_DIE;
		monster->oldEvent = GTH_EV_CHAR_DIE;		
		monster->bIgnoreRespawnTime = TRUE;	
		return TRUE;
	}

	return FALSE;

}
