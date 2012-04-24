#include "../global.h"

void MONSTER_InitAI( monsterCharacter_t* monster, int isLost )
{
	int localWorldIdx;


	AI_InitNodeBank( monster );

	
	monster->ai.aiEvent = AI_IDLE;
	monster->ai.thinkTime = GTH_Randomize( 5000 );
	monster->ai.saveTime = 0;
	monster->ai.curWayPath = -1;
	monster->ai.directWayPath = -1;
	monster->ai.pathNumber = -1;
	monster->ai.returnFlag = 0;
	
	
	monster->ai.moveType = AI_DEF_ENABLE_WALK;
	monster->ai.oldMoveType = AI_DEF_ENABLE_WALK;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	
	AI_GetGridPosition( monster->position, monster->ai.startPos, &g_cmap[localWorldIdx] );

	monster->ai.aiAddr = 0;

	
	monster->ai.callProcedureAddr = 0;
	monster->ai.isCall = false;

	monster->forcedEntityType = -1;
	monster->forcedEntityIdx = -1;
	monster->damagedEntityType = -1;
	monster->damagedEntityIdx = -1;
	monster->searchEntityType = -1;
	monster->searchEntityIdx = -1;
	monster->targetIdx = -1;
	monster->targetType = -1;
	monster->bossEntityIdx = -1;
	monster->bossEntityType = -1;

	
	if( !isLost ) monster->ai.lostPathCount = 0;

	AI_StopPathFinding( monster );
}




int MONSTER_AdjustNextAngle( monsterCharacter_t* monster )
{
	return ( 0 ); 
}

int AI_CheckCorrectWay( monsterCharacter_t* monster, int wayType )
{
	vec3_t angles, delta, pos;
	grid_t nextPos;
	float speed;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );


	vec_copy( monster->angles, angles );
	vec_copy( monster->position, pos );
	angles[1] = monster->nextAngle;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, angles[0] );
	RotateZCoord( delta, angles[1] );
	Normalize( delta );

	speed = SPEED_CONSTANT * monster->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale( delta, speed, delta ); 
	vec_add( pos, delta, pos);

	AI_GetGridPosition( pos, nextPos, &g_cmap[localWorldIdx] );

	if( !AI_CheckCollision( nextPos[0], nextPos[1], &g_cmap[localWorldIdx] ) )
	{
		g_logSystem->Write( "%s(%d) Lost the path!, Initialize AI again... speed:%.2f, pos(%.2f,%.2f,%.2f), event:%d, ai event:%d, lostPathCount:%d",
			monster->basic.name, monster->idx, speed, pos[0], pos[1], pos[2], monster->event, monster->ai.aiEvent, monster->ai.lostPathCount );
		return( 0 );
	}
	else return( 1 );
}

void MONSTER_Lost( monsterCharacter_t* monster )
{
	grid_t goalPos, curPos;
	int localWorldIdx;

	if( monster->ai.lostPathCount > 5 )
	{
		MONSTER_ForcedDelete( monster );
		return;
	}

	if( monster->ai.wayPointNumber > 0 )
	{
		goalPos[0] = monster->ai.goalPos[0];
		goalPos[1] = monster->ai.goalPos[1];
		
		if( monster->ai.curWayPath == -1 ) 
		{
			goalPos[0] = monster->ai.startPos[0];
			goalPos[1] = monster->ai.startPos[1];
		}
		else 
		{
			goalPos[0] = monster->ai.wayPath[monster->ai.curWayPath][0];
			goalPos[1] = monster->ai.wayPath[monster->ai.curWayPath][1];
		}
		
		localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
		AI_GetGridPosition( monster->position, curPos, &g_cmap[localWorldIdx] );
		AI_StopPathFinding( monster );
		
		
		monster->ai.curPos[0] = curPos[0];
		monster->ai.curPos[1] = curPos[1];
		monster->ai.goalPos[0] = goalPos[0];
		monster->ai.goalPos[1] = goalPos[1];
		
		
		AI_FindPath( monster );
		monster->ai.aiEvent = AI_MOVE;
	}
	else
	{
		MONSTER_InitAI( monster, true );
	}

	monster->ai.lostPathCount ++;
}

void MONSTER_ForcedDelete( monsterCharacter_t* monster )
{
	monster->deathTime = g_globalTime;
	g_logSystem->Write( "%s(%d) Forced Delete! Respawn again... speed:%.2f, pos(%.2f,%.2f,%.2f), event:%d, ai event:%d",
		monster->basic.name, monster->idx, monster->calVelocity, 
		monster->position[0], monster->position[1], monster->position[2], (int)monster->event, (int)monster->ai.aiEvent );

	monster->aliveState = MON_ALIVE_STATE_DIE;
	monster->ai.aiEvent = AI_DIE;
	monster->event = GTH_EV_CHAR_DIE;
	monster->oldEvent = GTH_EV_CHAR_DIE;
}


void MONSTER_MovePath( monsterCharacter_t* monster )
{
	vec3_t tpos;					
	grid_t myPos;					
	vec3_t na;						
	int localWorldIdx;

	
	if( !monster->ai.isGoal )
	{
		if( AI_FindPath( monster ) )
		{
			monster->oldEvent =	monster->event; 
			monster->event = GTH_EV_CHAR_SKIP;

			return;
		}
		
		else
		{
			if( monster->wayPointNumber > 0 )
			{
				MONSTER_ForcedDelete( monster );
				return;
			}
			else
			{
				monster->event = GTH_EV_CHAR_IDLE;
				monster->ai.aiEvent = AI_MOVE;
				AI_StopPathFinding( monster );
				return;
			}
		}
	}
	
	if( monster->ai.pathNumber < 0 ) 
	{
		monster->oldEvent =	monster->event; 
		monster->event = GTH_EV_CHAR_IDLE;
		
		
		monster->ai.aiEvent = AI_IDLE;
		
		
		return;
	}
	
	
	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	
	
	if( myPos[0] == monster->ai.path[ monster->ai.pathNumber ][0] &&
		myPos[1] == monster->ai.path[ monster->ai.pathNumber ][1] )
	{
		monster->ai.pathNumber --;
		
		if( monster->ai.pathNumber < 0 )
		{
			monster->oldEvent =	monster->event; 
			monster->event = GTH_EV_CHAR_IDLE;
			return;
		}

		
		AI_GetWorldPosition( monster->ai.path[monster->ai.pathNumber], tpos, &g_cmap[localWorldIdx] );
		
		tpos[0] += g_cmap[localWorldIdx].scale * 0.5f;
		tpos[1] += g_cmap[localWorldIdx].scale * 0.5f;
		tpos[2] = monster->position[2];
		
		
		vec_sub( tpos, monster->position, tpos );
		
		
		vectoangles( tpos, na );
		monster->nextAngle = na[1];

		
		
		
		
		
	}
	
	switch( MONSTER_AdjustNextAngle( monster ) )
	{

	
	
	
	
		

	
	case -1:
		if ( monster->ai.aiEvent == AI_RUNAWAY)		
			monster->event = GTH_EV_CHAR_TURNLEFT;
		else
			monster->event = GTH_EV_CHAR_RUNLEFT;
		break;
	case 1:
		if ( monster->ai.aiEvent == AI_RUNAWAY)		
			monster->event = GTH_EV_CHAR_RUNRIGHT;
		else
			monster->event = GTH_EV_CHAR_TURNRIGHT;
		break;
	case 0:
		if ( monster->ai.aiEvent == AI_RUNAWAY)		
			monster->event = GTH_EV_CHAR_RUN;
		else		
			monster->event = GTH_EV_CHAR_WALK;		
	

		
		if( !AI_CheckCorrectWay( monster, 0 ) )
		{
			monster->event = GTH_EV_CHAR_IDLE;
			monster->ai.aiEvent = AI_LOST;
		}

		break;
	}
}


void MONSTER_MoveWayPoint( monsterCharacter_t* monster )
{
	vec3_t tpos;					
	grid_t myPos;					
	vec3_t na;						
	int localWorldIdx;

	
	if( monster->ai.returnFlag ) 
	{
		MONSTER_MovePath( monster );
		return;
	}

	
	if( !monster->ai.isGoal )
	{
		if( AI_FindPath( monster ) )
		{
			monster->oldEvent =	monster->event; 
			monster->event = GTH_EV_CHAR_SKIP;
			return;
		} else MONSTER_ForcedDelete( monster );
	}
	
	
	if( monster->ai.curWayPath < 0 || monster->ai.directWayPath == 0 || monster->ai.wayPointNumber <= 1 )
	{
		monster->event = GTH_EV_CHAR_IDLE;
		return;
	}
	
	
	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	
	
	if( myPos[0] == monster->ai.wayPath[monster->ai.curWayPath][0] &&
		myPos[1] == monster->ai.wayPath[monster->ai.curWayPath][1] )
	{
		monster->ai.curWayPath += monster->ai.directWayPath;

		
		
		if( monster->ai.curWayPath < 0 )
		{
			monster->ai.curWayPath = 0;
			monster->event = GTH_EV_CHAR_IDLE;
			return;
		}
		if( monster->ai.curWayPath >= monster->ai.wayPathNumber )
		{
			monster->ai.curWayPath = monster->ai.wayPathNumber - 1;
			monster->event = GTH_EV_CHAR_IDLE;
			return;
		}

		
		AI_GetWorldPosition( monster->ai.wayPath[monster->ai.curWayPath], tpos, &g_cmap[localWorldIdx] );
		
		tpos[0] += g_cmap[localWorldIdx].scale * 0.5f;
		tpos[1] += g_cmap[localWorldIdx].scale * 0.5f;
		tpos[2] = monster->position[2];
		
		
		vec_sub( tpos, monster->position, tpos );
		
		
		vectoangles( tpos, na );
		monster->nextAngle = na[1];
	}
	
	switch( MONSTER_AdjustNextAngle( monster ) )
	{
	case -1:
		monster->event = GTH_EV_CHAR_TURNLEFT;
		break;
	case 1:
		monster->event = GTH_EV_CHAR_TURNRIGHT;
		break;
	case 0:
		monster->event = GTH_EV_CHAR_WALK;
		
		if( !AI_CheckCorrectWay( monster, 0 ) )
		{
			monster->event = GTH_EV_CHAR_IDLE;
			monster->ai.aiEvent = AI_LOST;
		}

		break;
	}
}

void MONSTER_Move( monsterCharacter_t* monster )
{
	
	
	if( monster->ai.moveType == AI_DEF_ENABLE_WALK )
	{
		if( monster->ai.wayPointNumber > 0 ) MONSTER_MoveWayPoint( monster );
		else MONSTER_MovePath( monster );
	}
	
	else 
	{
		monster->event = GTH_EV_CHAR_IDLE;
	}
}

void MONSTER_Trace( monsterCharacter_t* monster )
{
	vec3_t tpos;					
	grid_t myPos;					
	vec3_t na;						
	int localWorldIdx;

	
	
	if( AI_IsWithinAttackLength( monster ) )
	{
		AI_StopPathFinding( monster );
		monster->oldEvent =	monster->event; 
		monster->event = GTH_EV_CHAR_SKIP;
		MONSTER_Attach( monster );
		return;
	}

	if( monster->ai.moveType != AI_DEF_ENABLE_WALK ) return;
	
	
	if( !monster->ai.isGoal )
	{
		if( AI_FindPath( monster ) )
		{
			monster->oldEvent =	monster->event; 
			monster->event = GTH_EV_CHAR_SKIP;
			return;
		}
		
		else
		{
			monster->event = GTH_EV_CHAR_IDLE;
			monster->ai.aiEvent = AI_MOVE;
			AI_StopPathFinding( monster );
			return;
		}
	}
	
	if( monster->ai.pathNumber < 0 )
	{
		monster->oldEvent =	monster->event; 
		monster->event = GTH_EV_CHAR_SKIP;
		MONSTER_Attach( monster );
		return;
	}
	
	
	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	AI_GetGridPosition( monster->position, myPos, &g_cmap[localWorldIdx] );

	
	
	if( myPos[0] == monster->ai.path[ monster->ai.pathNumber ][0] &&
		myPos[1] == monster->ai.path[ monster->ai.pathNumber ][1] )
	{
		monster->ai.pathNumber --;
		
		if( monster->ai.pathNumber < 0 )
		{
			monster->oldEvent =	monster->event; 
			monster->event = GTH_EV_CHAR_SKIP;
			AI_InterpretImmediately( monster );
			return;
		}

		
		AI_GetWorldPosition( monster->ai.path[monster->ai.pathNumber], tpos, &g_cmap[localWorldIdx] );
		
		tpos[0] += g_cmap[localWorldIdx].scale * 0.5f;
		tpos[1] += g_cmap[localWorldIdx].scale * 0.5f;
		tpos[2] = monster->position[2];
		
		
		vec_sub( tpos, monster->position, tpos );
		
		
		vectoangles( tpos, na );
		monster->nextAngle = na[1];

		
		
		

	}
	
	switch( MONSTER_AdjustNextAngle( monster ) )
	{
	case -1:
		monster->event = GTH_EV_CHAR_TURNLEFT;
		break;
	case 1:
		monster->event = GTH_EV_CHAR_TURNRIGHT;
		
		break;
	case 0:
		monster->event = GTH_EV_CHAR_WALK;
		monster->event = GTH_EV_CHAR_RUN;			

		
		if( !AI_CheckCorrectWay( monster, 0 ) )
		{
			monster->event = GTH_EV_CHAR_IDLE;
			monster->ai.aiEvent = AI_LOST;
		}

		break;
	}
}


void MONSTER_Attach( monsterCharacter_t* monster )
{
	float range, range2;
	float x, y;
	grid_t mypos, dest;

	vec3_t tpos, na;
	int localWorldIdx;

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	
	switch( monster->targetType )
	{
	case ENTITY_PC :
		x = g_pc[monster->targetIdx].position[0];
		y = g_pc[monster->targetIdx].position[1];
		vec_copy( g_pc[monster->targetIdx].position, tpos );
		AI_GetGridPosition( g_pc[monster->targetIdx].position, dest, &g_cmap[localWorldIdx] );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		x = g_monster[monster->targetIdx].position[0];
		y = g_monster[monster->targetIdx].position[1];
		vec_copy( g_monster[monster->targetIdx].position, tpos );
		AI_GetGridPosition( g_monster[monster->targetIdx].position, dest, &g_cmap[localWorldIdx] );
		break;
	}
	AI_GetGridPosition( monster->position, mypos, &g_cmap[localWorldIdx] );

	
	x -= monster->position[0];
	y -= monster->position[1];
	range = x * x + y * y;
	
	range2 = monster->calAttackLength;

	if( (-1 != monster->selectedSkillIdx ) &&
		( 0 !=monster->skillNumber ) )		
	{
		
		
		
		if ( monster->skillUsePercent[monster->selectedSkillIdx] != 0)
		{
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(monster->selectedSkillIdx);
			if(pSkillTable==NULL)
			{
				TRACE("Error! pSkillTable==NULL   \n");
				return;
			}
			
			range2 = pSkillTable->GetSkillRequire()->m_nLength;
		}



	}

	range2 = range2 * range2;

	
	
	
	
	


	
	if( mypos[0] == dest[0] && mypos[1] == dest[1] )
	{
		
		if( range > range2 )
		{
			
			vec_sub( tpos, monster->position, tpos );
			
			
			vectoangles( tpos, na );
			monster->nextAngle = na[1];
			
			
			switch( MONSTER_AdjustNextAngle( monster ) )
			{
			case -1:
				monster->event = GTH_EV_CHAR_TURNLEFT;
				break;
			case 1:
				monster->event = GTH_EV_CHAR_TURNRIGHT;
				break;
			case 0:
				
				if( monster->ai.moveType == AI_DEF_ENABLE_WALK )
				{
					
					monster->event = GTH_EV_CHAR_RUN;
				}
				else 
				{
					monster->event = GTH_EV_CHAR_IDLE;
				}
				break;
			}
			return;
		}
		
		else
		{
			vec_sub( tpos, monster->position, tpos );
			vectoangles( tpos, na );
			
			if( abs( na[1] - monster->angles[1] ) < 15.0f )
			{
				
				if( monster->selectedSkillIdx != -1 )
				{
					monster->event = GTH_EV_CHAR_SKILL;
				}
				else
				{
					monster->event = GTH_EV_CHAR_ATTACK;
				}
				return;
			}
			else
			{
				monster->nextAngle = na[1];
				
				
				switch( MONSTER_AdjustNextAngle( monster ) )
				{
				case -1:
					monster->event = GTH_EV_CHAR_TURNLEFT;
					break;
				case 1:
					monster->event = GTH_EV_CHAR_TURNRIGHT;
					break;
				case 0:
					
					if( monster->ai.moveType == AI_DEF_ENABLE_WALK )
					{
						
						monster->event = GTH_EV_CHAR_RUN;
					}
					else
					{
						monster->event = GTH_EV_CHAR_IDLE;
					}
					break;
				}
				return;				
			}
		}
	}
	
	else
	{
		
		if( range > range2 )
		{
			
			if( monster->ai.moveType == AI_DEF_ENABLE_WALK )
			{
				monster->ai.aiEvent = AI_TRACE;
				monster->oldEvent =	monster->event; 
				monster->event = GTH_EV_CHAR_SKIP;
				AI_InterpretImmediately( monster );
			}
			else 
			{
				monster->event = GTH_EV_CHAR_IDLE;
			}
			return;
		}
		
		else 
		{
			vec_sub( tpos, monster->position, tpos );
			vectoangles( tpos, na );
			
			if( abs( na[1] - monster->angles[1] ) < 15.0f )
			{ 
				
				if( monster->selectedSkillIdx != -1 )
				{
					monster->event = GTH_EV_CHAR_SKILL;
				}
				else
				{
					monster->event = GTH_EV_CHAR_ATTACK;
				}
				return;
			}
			else
			{
				monster->nextAngle = na[1];
				
				
				switch( MONSTER_AdjustNextAngle( monster ) )
				{
				case -1:
					monster->event = GTH_EV_CHAR_TURNLEFT;
					break;
				case 1:
					monster->event = GTH_EV_CHAR_TURNRIGHT;
					break;
				case 0:
					if( monster->ai.moveType == AI_DEF_ENABLE_WALK )
					{
						
						monster->event = GTH_EV_CHAR_RUN;
					}
					else 
					{
						monster->event = GTH_EV_CHAR_IDLE;
					}
					break;
				}
				return;				
			}
		}
	}
}



void MONSTER_ExecuteAIEvent( monsterCharacter_t* monster )
{
	
	if( monster->isSummon && monster->summonValidTime > 0.0f )
	{
		monster->summonValidTime -= g_timer->GetElapsedMilliTime();
		if( monster->summonValidTime <= 0.0f )
		{
			monster->aliveState = MON_ALIVE_STATE_DISAPPEAR;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_IDLE;
			monster->deathTime = g_globalTime;
			monster->summonValidTime = -1.0f;
			monster->recoveryTime = 0.0f;
		}
	}
	
	if( monster->aliveState == MON_ALIVE_STATE_NORMAL )
	{
		monster->recoveryTime -= g_timer->GetElapsedMilliTime();
		
		if( monster->recoveryTime < 0.0f ) monster->recoveryTime = 0.0f;
		if( monster->recoveryTime > 0.0f ) return;
	}


	
	
	if( GTH_GetEntityCurseState( ENTITY_MONSTER, monster->idx, AFF_DATA_TYPE_PARALYSIS ) )
	{
		
		monster->ai.moveType = AI_DEF_DISABLE_MOVE;
	} 
	else if( GTH_GetEntityCurseState( ENTITY_MONSTER, monster->idx, AFF_DATA_TYPE_SLEEP ) )
	{
		
		monster->ai.moveType = AI_DEF_DISABLE_MOVE;
	}
	else if( GTH_GetEntityCurseState( ENTITY_MONSTER, monster->idx, AFF_DATA_TYPE_STOPPED ) )
	{
		
		monster->ai.moveType = AI_DEF_DISABLE_WALK;
	}
	else
	{
		monster->ai.moveType = monster->ai.oldMoveType;
	}




	
	if( monster->ai.moveType == AI_DEF_DISABLE_MOVE )
	{
		if( monster->ai.aiEvent != AI_DIE )
		{
			
			
			if( monster->event != GTH_EV_CHAR_FREEZE && monster->event != GTH_EV_CHAR_PUSHED )
				monster->event = GTH_EV_CHAR_IDLE;
			return;
		}
	}

	
	
	
	

	switch( monster->ai.aiEvent )
	{
	
	case AI_IDLE :
		if( monster->aliveState == MON_ALIVE_STATE_SPAWN )
		{
			monster->recoveryTime -= g_timer->GetElapsedMilliTime();
			if( monster->recoveryTime < 0.0f ) 
			{
				monster->recoveryTime = 0.0f;
				monster->aliveState = MON_ALIVE_STATE_NORMAL;
			}
		}
		break;
	
	case AI_MOVE :
		MONSTER_Move( monster );
		break;
	case AI_TRACE :
		MONSTER_Trace( monster );
		break;
	case AI_DIE :
		if( monster->aliveState == MON_ALIVE_STATE_CORPSE )
		{
			if( g_globalTime - monster->deathTime >= MONSTER_CORPSE_ANIMATION_TIME )
			{
				monster->aliveState = MON_ALIVE_STATE_DISAPPEAR;
				monster->deathTime = g_globalTime;
				return;
			}
		}
		if( monster->aliveState == MON_ALIVE_STATE_DISAPPEAR ) 		
		{
			
			if( g_globalTime - monster->deathTime >= MONSTER_DISAPPEAR_ANIMATION_TIME ) 
			{
				monster->deathTime = g_globalTime;
				monster->aliveState = MON_ALIVE_STATE_DIE;
				return;
			}
		}
		















		if( monster->aliveState == MON_ALIVE_STATE_DIE )
		{
			if( monster->isSummon )
			{
				MONSTER_Delete( monster );
				GTH_Zone_DeleteEntityList(monster->worldIdx, monster->zoneIdx, monster->entityType, monster->idx);
				return;
			}
			float LimitTime = 0.0f;
			if (TRUE == monster->bIgnoreRespawnTime) 			
				LimitTime = monster->deathTime + 1000;				
			else
				LimitTime =monster->deathTime + monster->respawnTime;			

			if ( g_globalTime >= LimitTime ) 		
			{
				MONSTER_Respawn( monster );
				if( monster->aliveState == MON_ALIVE_STATE_SPAWN )
					monster->zoneIdx = GTH_Zone_UpdateCurrentZone(monster->entityType, monster->idx, monster->worldIdx, monster->zoneIdx, monster->position);
			}			
		}
		

		if( monster->aliveState == MON_ALIVE_STATE_FADE )
		{
			
			if( g_globalTime - monster->deathTime >= MONSTER_DISAPPEAR_ANIMATION_TIME ) 
			{
				monster->deathTime = g_globalTime;
				monster->aliveState = MON_ALIVE_STATE_DIE;
				MONSTER_Delete( monster );

				GTH_Zone_DeleteEntityList(monster->worldIdx, monster->zoneIdx, monster->entityType, monster->idx);
				return;
			}
		}
		break;
	case AI_ATTACK :
		MONSTER_Attach( monster );
		break;
	case AI_RUNAWAY :
		MONSTER_MovePath( monster );
		break;
	case AI_LOST :
		MONSTER_Lost( monster );
		break;
	}
}