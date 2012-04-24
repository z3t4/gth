#include "../global.h"


int g_monsterSpawnCount;




void MONSTER_Spawn( monsterCharacter_t* monster )
{
	
	
	

	int npcIdx, i;
	char szTemp[256];

	memset( &monster->npc, 0, sizeof( npcStruct_t ) );
	if( monster->ai.aiType < 0 || monster->ai.aiType >= g_scriptInfo.numAiScript ) 
	{
		sprintf( szTemp, "%s Monster's %d Script Index is out of range!", monster->basic.name, monster->ai.aiType );

		MessageBox(g_hWnd, szTemp, "Error!", MB_OK);
		if( !AI_LoadScript( monster, g_scriptInfo.aiScript[0].filename ) )
		{
			sprintf( szTemp, "%s Monster's %s AI Script File not found!", 

				monster->basic.name, g_scriptInfo.aiScript[0].filename );
			MessageBox(g_hWnd, szTemp, "Error", MB_OK);
		}
	}
	else
	{
		if( !AI_LoadScript( monster, g_scriptInfo.aiScript[monster->ai.aiType].filename ) )
		{
			sprintf( szTemp, "%s Monster's %s AI Script File not found!", 

				monster->basic.name, g_scriptInfo.aiScript[0].filename );
			MessageBox(g_hWnd, szTemp, "Error!", MB_OK);
		}
	}

	if( monster->npcAi.aiType >= 0 )
	{
		if( monster->npcAi.aiType < 0 || monster->npcAi.aiType >= g_scriptInfo.numNpcScript ) 
		{
			sprintf( szTemp, "%s Monster's %d Script Index is out of range!", monster->basic.name, monster->npcAi.aiType );

			MessageBox(g_hWnd, szTemp, "Error!", MB_OK);
			
			if( !AI_NPC_LoadScript( monster, g_scriptInfo.npcScript[0].filename ) )
			{
				sprintf( szTemp, "%s Monster's %s AI Script File not found!", 

					monster->basic.name, g_scriptInfo.npcScript[0].filename );
				MessageBox(g_hWnd, szTemp, "Error!", MB_OK);
			}
		}
		else
		{
			if( !AI_NPC_LoadScript( monster, g_scriptInfo.npcScript[monster->npcAi.aiType].filename ) )
			{
				sprintf( szTemp, "%s Monster's %s AI Script File not found!", 

					monster->basic.name, g_scriptInfo.npcScript[monster->npcAi.aiType].filename );
				MessageBox(g_hWnd, szTemp, "Error!", MB_OK);
			}
		}
		monster->npc.scriptFlag = true;
	}
	else
	{
		monster->npc.scriptFlag = false;
	}

	if( monster->entityType == ENTITY_NPC )
	{
		npcIdx = monster->npcTableIdx;
		if( npcIdx >= 0 )
		{	
			monster->npc.active	= 1;
			monster->npc.noticePcNumber	= 0;
			monster->npc.maxNoticePcNumber = 200; 
			
			for( i = 0; i < MAX_NUMBER_OF_NPC_NOTICE_PC; i ++ )
			{
				monster->npc.noticePcIdx[i] = -1;
			}
		}
		
		
		g_sNpcList.push_back( monster );
	}
	monster->active = true;

	
	AI_PreparePathfinding( monster, monster->basic.pathFindingSize );
}


void MONSTER_Respawn( monsterCharacter_t* monster )
{
	monsterSpawnTable_t			*ms;
	monsterSpawnZoneTable_t		*sz;
	worldTable_t				*wt;

	vec3_t		pos, angle, rpos;
	grid_t		gpos;
	float		range;
	int			i, zoneIdx, r, accPro;

	int localWorldIdx;

	
	memcpy( &monster->basic, &g_monsterTable[monster->tableIdx], sizeof(monsterBasicTable_t) );

	ms = &g_worldTable[monster->spawnWorldIdx].monsterSpawnTable[monster->spawnTableIdx];
	wt = &g_worldTable[monster->spawnWorldIdx];

	monster->deathTime = 0;
	monster->worldIdx = monster->spawnWorldIdx;
	localWorldIdx = GTH_GetLocalWorldIdx( monster->spawnWorldIdx );

	monster->aliveState = MON_ALIVE_STATE_DIE;
	monster->ai.aiEvent = AI_DIE;
	monster->event = GTH_EV_CHAR_DIE;
	monster->oldEvent = GTH_EV_CHAR_DIE;

	
	
	if( ms->smallZoneFlag )
	{
		float szRange;

		vec_copy( ms->smallZonePosition, pos );
		szRange = sqrtf( (float)g_monsterTable[monster->tableIdx].mapDensityWeight ) * 25.0f;
		
		int rand_i;

		rand_i = GTH_Randomize( (int)szRange * 2 );
		rpos[0] = pos[0] + (float)( (float)rand_i - szRange );
		
		rand_i = GTH_Randomize( (int)szRange * 2 );
		rpos[1] = pos[1] + (float)( (float)rand_i - szRange );
		rpos[2] = pos[2];

		
		if( rpos[0] < 0.0f || rpos[1] < 0.0f ) 
		{
			monster->aliveState = MON_ALIVE_STATE_DIE;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->oldEvent = GTH_EV_CHAR_DIE;
			return;
		}
		range = GTH_GetDistance2D( rpos, pos );

		AI_GetGridPosition( rpos, gpos, &g_cmap[localWorldIdx] );

		if( range > szRange || !AI_CheckCollision( gpos[0], gpos[1], &g_cmap[localWorldIdx] ) )
		{
			monster->aliveState = MON_ALIVE_STATE_DIE;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->oldEvent = GTH_EV_CHAR_DIE;
			return;
		}

		rpos[2] = GTH_GetHeightFromCollisionMap( monster->spawnWorldIdx, rpos );
		vec_copy( rpos, monster->position );
	}
	else if( ms->spawnZoneNumber > 0 )
	{
		r = (int)( GTH_Random() * 100.0f );
		accPro = 0;
		for( i = 0; i < ms->spawnZoneNumber; i ++ )
		{
			accPro += ms->spawnZoneProbability[i];
			if( r < accPro ) break;
		}
		if( i == ms->spawnZoneNumber ) 
		{
			monster->aliveState = MON_ALIVE_STATE_DIE;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->oldEvent = GTH_EV_CHAR_DIE;
			return;
		}
		zoneIdx = ms->spawnZoneIdx[i];

		sz = &wt->monsterSpawnZoneTable[zoneIdx];

		vec_copy( sz->position, pos );
		vec_copy( sz->angle, angle );
		
		int rand_i;
		rand_i = GTH_Randomize( sz->range * 2 );
		rpos[0] = pos[0] + (float)( (float)rand_i - sz->range );
		rand_i = GTH_Randomize( sz->range * 2 );
		rpos[1] = pos[1] + (float)( (float)rand_i - sz->range );
		rpos[2] = pos[2];

		
		if( rpos[0] < 0.0f || rpos[1] < 0.0f ) 
		{
			monster->aliveState = MON_ALIVE_STATE_DIE;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->oldEvent = GTH_EV_CHAR_DIE;
			return;
		}
		range = GTH_GetDistance2D( rpos, pos );

		AI_GetGridPosition( rpos, gpos, &g_cmap[localWorldIdx] );

		if( range > sz->range || !AI_CheckCollision( gpos[0], gpos[1], &g_cmap[localWorldIdx] ) )
		{
			monster->aliveState = MON_ALIVE_STATE_DIE;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->oldEvent = GTH_EV_CHAR_DIE;
			return;
		}
		rpos[2] = GTH_GetHeightFromCollisionMap( monster->spawnWorldIdx, rpos );
		vec_copy( rpos, monster->position );
		vec_copy( angle, monster->angles );

	}
	else if( ms->positionNumber > 0 )
	{
		r = GTH_Randomize( ms->positionNumber );
		if( r == ms->positionNumber )
		{
			monster->aliveState = MON_ALIVE_STATE_DIE;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->oldEvent = GTH_EV_CHAR_DIE;
			return;
		}
		vec_copy( ms->position[r], monster->position );
		vec_copy( ms->angles[r], monster->angles );

		AI_GetGridPosition( monster->position, gpos, &g_cmap[localWorldIdx] );
		if( !AI_CheckCollision( gpos[0], gpos[1], &g_cmap[localWorldIdx] ) )
		{
			monster->aliveState = MON_ALIVE_STATE_DIE;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->oldEvent = GTH_EV_CHAR_DIE;
			return;
		}
	} else if( ms->wayPointNumber > 0 )
	{
		vec_copy( ms->wayPoint[0], monster->position );
		vec_copy( ms->angles[0], monster->angles );

		AI_GetGridPosition( monster->position, gpos, &g_cmap[localWorldIdx] );
		if( !AI_CheckCollision( gpos[0], gpos[1], &g_cmap[localWorldIdx] ) )
		{
			monster->aliveState = MON_ALIVE_STATE_DIE;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->oldEvent = GTH_EV_CHAR_DIE;
			return;
		}

		monster->wayPointNumber = ms->wayPointNumber;
		monster->ai.wayPointNumber = ms->wayPointNumber;

		for( i = 0; i < ms->wayPointNumber; i ++ )
		{
			vec_copy( ms->wayPoint[i], monster->wayPoint[i] );
			vec_copy( ms->wayPoint[i], monster->ai.wayPoint[i] );
		}
	}

	
	monster->specialMonster = 0;

	
	
	


	monster->aliveState = MON_ALIVE_STATE_SPAWN;
	
	monster->recoveryTime = MONSTER_SPAWN_ANIMATION_TIME;

	
	MONSTER_Calculate( monster );
	monster->curRA = monster->calMaxRA;
	monster->curSA = monster->calMaxSA;

	monster->event = GTH_EV_CHAR_IDLE;
	monster->oldEvent = GTH_EV_CHAR_DIE;

	GTH_InitContributionPoint( monster );
	
	for ( i =0; i < MAX_AFFECT; i++ )
	{
		memset( &monster->affect[i], 0, sizeof( affect_t ) );
		monster->affect[i].type = AFFECT_NONE;
	}
	monster->affectNumber = 0;




	g_monsterSpawnCount ++;

	
	MONSTER_InitAI( monster );

	AI_InitMessage( monster );

	monster->isSummon		 = false;
	monster->summonTime      = 0.0f;
	monster->summonValidTime = -1.0f;

	monster->autoRecoveryRATime = 0.0f;
	monster->autoRecoverySATime = 0.0f;

	
	
	monster->selectedSkillIdx = -1;	

	
	memset(monster->skillUsePercent, 0, sizeof(int)*MAX_NUMBER_OF_MONSTER_SKILL);

	
	vec_copy(monster->position, monster->Spawnposition);	
	memset(monster->bUsedSkill, 0, sizeof (bool) * 10 );
	

	
	monster->bIgnoreRespawnTime = FALSE;	
	
}



void	MONSTER_SkillInitialize( monsterCharacter_t* monster)
{
	
	monsterBasicTable_t		*mb;
	mb = &g_monsterTable[monster->tableIdx];

	monster->skillNumber = mb->skillNumber;
	
	for(int i = 0; i < monster->skillNumber; i++)
	{
		
		monster->skill[i].idx		= mb->skillIdx[i];
		monster->skill[i].tableIdx	= mb->skillIdx[i];
	}

	monster->saveSkillIdx = -1;
}




void MONSTER_Initialize( monsterCharacter_t* monster, int worldIdx, int spawnIdx )
{
	monsterSpawnTable_t* ms;
	ms = &g_worldTable[worldIdx].monsterSpawnTable[spawnIdx];

	monster->idx = g_monsterNumber; 

	
	if( ms->monsterTableIdx < g_monsterTableNumber ) monster->tableIdx = ms->monsterTableIdx;
	else monster->tableIdx = 0;

	monster->spawnWorldIdx = worldIdx;
	monster->spawnTableIdx = spawnIdx;
	
	monster->respawnTime = ms->respawnTime;

	memcpy( &monster->basic, &g_monsterTable[monster->tableIdx], sizeof( monsterBasicTable_t ) );

	monster->entityType = ms->entityType;

	monster->ai.aiType = ms->aiType;
	monster->npcAi.aiType = ms->npcAiType;

	monster->npcTableIdx = ms->npcTableIdx;
	monster->npc.scriptFlag = false;

	
	monster->respawnTime = monster->basic.respawnTime;
	monster->ai.aiType = monster->basic.AIScriptIdx;
	monster->npcAi.aiType = monster->basic.npcAIScriptIdx;
	
	MONSTER_Spawn( monster );
	MONSTER_Respawn( monster );

	
	MONSTER_SkillInitialize( monster );

	monster->zoneIdx = -1;

	if( monster->event == GTH_EV_CHAR_IDLE )
	{
		monster->zoneIdx = GTH_Zone_UpdateCurrentZone(monster->entityType, monster->idx, monster->worldIdx, monster->zoneIdx, monster->position);
	}

	
	
	
	memset(monster->SummonOptIndex, -1, sizeof(int) * monsterCharacter_t::MAX_SUMMON_OPTION_VALUE);
	memset(monster->SummonOptValue, 0, sizeof(float) * monsterCharacter_t::MAX_SUMMON_OPTION_VALUE);
	

	
	
	monster->isSummon = false;
	monster->summonTime = 0.0f;
	monster->summonValidTime = -1.0f;

	
	monster->fSpawnElapsedTime = g_timer->GetAppMilliTime();

	memset(&monster->fAiTempValue,0,sizeof(monster->fAiTempValue));
}

void MONSTER_Delete( monsterCharacter_t* monster )
{
	monster->active = false;
	AI_ReleasePathfinding( monster );
}

void MONSTER_Free()
{
	for( int i = 0; i < MAX_MONSTERS; i ++ )
	{
		if( !g_monster[i].active ) continue;
		AI_ReleasePathfinding( &g_monster[i] );
	}
}

int MONSTER_GetNotActiveIdx()
{
	int i;
	for( i = 0; i < MAX_MONSTERS; i ++ )
	{
		if( !g_monster[i].active ) return( i );
	}
	return( -1 );
}
int MONSTER_Summon( int monsterTableIdx, int worldIdx, vec3_t position, int aiType, entityType_t entityType, int summonerIdx, int summonerType, float validTime )
{
	monsterCharacter_t* monster;
	int localWorldIdx;

	int idx, i;
	char summonerName[255];
	idx = MONSTER_GetNotActiveIdx();
	if( idx < 0 ) return( -1 );

	localWorldIdx = GTH_GetLocalWorldIdx( worldIdx );
	if( !AI_CheckCollision( position, &g_cmap[localWorldIdx] ) ) return( -1 );
	
	
	
	
	
	
	
	
	
	
	
	
	


	monster = &g_monster[idx];
	monster->worldIdx = worldIdx;

	memset( monster, 0, sizeof( monsterCharacter_t ) );

	monster->idx = idx;
	monster->tableIdx = monsterTableIdx;

	monster->spawnWorldIdx = worldIdx;
	monster->spawnTableIdx = -1;
	
	monster->respawnTime = -1.0f;

	
	monster->selectedSkillIdx = -1;
	
	memcpy( &monster->basic, &g_monsterTable[monster->tableIdx], sizeof( monsterBasicTable_t ) );


	monster->entityType = entityType;

	monster->ai.aiType = aiType;
	monster->npcAi.aiType = -1;
	monster->npcTableIdx = -1;
	monster->npc.scriptFlag = false;
	
	
	MONSTER_SkillInitialize( monster );


	
	



	
	
	if( monster->ai.aiType < 0 || monster->ai.aiType >= g_scriptInfo.numAiScript ) 
	{
		AI_LoadScript( monster, g_scriptInfo.aiScript[0].filename );
	}
	else
	{
		AI_LoadScript( monster, g_scriptInfo.aiScript[monster->ai.aiType].filename );
	}

	
	AI_PreparePathfinding( monster );

	monster->deathTime = 0;
	monster->worldIdx = monster->spawnWorldIdx;

	vec_copy( position, monster->position );

	
	vec_copy(monster->position, monster->Spawnposition);

	monster->aliveState = MON_ALIVE_STATE_SPAWN;
	monster->recoveryTime = MONSTER_SPAWN_ANIMATION_TIME;

	
	
	
	
	MONSTER_Calculate( monster );
	monster->curRA = monster->calMaxRA;
	monster->curSA = monster->calMaxSA;

	monster->event = GTH_EV_CHAR_IDLE;
	monster->oldEvent = GTH_EV_CHAR_DIE;

	
	for ( i =0; i < MAX_AFFECT; i++ )
	{
		memset( &monster->affect[i], 0, sizeof( affect_t ) );
		monster->affect[i].type = AFFECT_NONE;
	}
	monster->affectNumber = 0;

	GTH_InitContributionPoint( monster );

	if( summonerType < 0 || summonerIdx < 0 )
	{
		sprintf( summonerName, "¾øÀ½" );
	}
	else
	{
		if( summonerType == ENTITY_PC )
		{
			sprintf( summonerName, "%s", g_pc[summonerIdx].name );
		}
		else sprintf( summonerName, "%s", g_monster[summonerIdx].basic.name );
	}

	g_logSystem->Write( "Monster Summon Complete : %s(%d) Success...pos(%.2f, %.2f, %.2f), Summoner : (%s)",
		monster->basic.name, monster->idx, monster->position[0], monster->position[1], monster->position[2], summonerName );

	
	MONSTER_InitAI( monster );

	AI_InitMessage( monster );

	
	monster->isSummon = true;
	monster->summonTime = g_timer->GetAppMilliTime();
	monster->summonValidTime = validTime;

	monster->active = 1;
	monster->zoneIdx = -1;

	monster->zoneIdx = GTH_Zone_GetZoneIdx( monster->position );
	GTH_Zone_AddEntityList(monster->worldIdx, monster->zoneIdx, monster->entityType, monster->idx);
	
	return( monster->idx );
}






void MONSTER_CalculateSkill()
{
}


void MONSTER_CalculateValue( monsterCharacter_t* monster )
{
}



void MONSTER_CalcualteHitRate()
{
}



void MONSTER_CalculateAvoidRate()
{
}



void MONSTER_Calculate( monsterCharacter_t* monster )
{
	monsterBasicTable_t		*mb;
	
	
	

	mb = &monster->basic;

	
	monster->calMaxRA = (float)mb->maxRA;
	monster->calMaxSA = (float)mb->maxSA;

	monster->calPhyMinDmg = mb->atkPhyMinDmg;
	monster->calPhyMaxDmg = mb->atkPhyMaxDmg;
	monster->calHitPower = mb->atkHitPower;
	monster->calAttackDelay = mb->atkDelay;
	
	monster->attackSpeedRate = 0;
	monster->generalSpeed = 100;

	monster->recoveryRARate = mb->recoveryRARate;
	monster->recoverySARate = mb->recoverySARate;

	monster->calAttackLength = mb->atkLength;
	monster->calAttackRange = mb->atkRange;
	monster->calCriticalDmg = mb->atkCriticalDmg;
	monster->calCriticalRate = mb->atkCriticalPro;

	monster->calHitRate = 0;
	monster->calDefensePower = mb->defPower;
	monster->calAvoidPower = mb->defAvoidPower;
	monster->calPositiveRes = mb->defPositiveRes;
	monster->calNegativeRes = mb->defNegativeRes;

	monster->calVelocity = (float)monster->basic.velocity;
	monster->calStunTime = monster->basic.stunTime;
	
	MONSTER_CalculateSkill();
	MONSTER_CalculateValue( monster );

	
	GTH_ApplyAffect( monster, monster->entityType );
	monster->calAttackDelay = GTH_ConvertAttackDelay( monster->calAttackDelay, monster->attackSpeedRate );

	if( monster->isSummon )
		SetMonsterOptionValue(monster);
}



void	SetMonsterOptionValue(monsterCharacter_t* monster)
{
	int index, value;

	
	
	
	
	
	for(int i = 1; i < monsterCharacter_t::MAX_SUMMON_OPTION_VALUE; i++ )
	{
	

		index = monster->SummonOptIndex[i];
		value = monster->SummonOptValue[i];
	

		if( index == -1 || value == - 1) continue;

		switch(index)
		{
		case MONSTER_OPTION_RA_INC:		monster->calMaxRA = (float)monster->basic.maxRA * (100.0f + (float)value * 0.8f) / 100.0f;	break;
		case MONSTER_OPTION_SA_INC:		monster->calMaxSA =	(float)monster->basic.maxSA * (100.0f + (float)value * 1.0f) / 100.0f;	break;
		case MONSTER_OPTION_RA_REC:		monster->recoveryRARate = (float)monster->basic.recoveryRARate * (100.0f + (float)value * 1.0f) / 100.0f;	break;
		case MONSTER_OPTION_SA_REC:		monster->recoverySARate = (float)monster->basic.recoverySARate * (100.0f + (float)value * 1.0f) / 100.0f;	break;
		case MONSTER_OPTION_MIN_ATT:	monster->calPhyMinDmg = (float)monster->basic.atkPhyMinDmg * (100.0f + (float)value * 1.0f) / 100.0f;		break;
		case MONSTER_OPTION_MAX_ATT:	monster->calPhyMaxDmg = (float)monster->basic.atkPhyMaxDmg * (100.0f + (float)value * 1.0f) / 100.0f;		break;
		case MONSTER_OPTION_MIN_MAG:	monster->calMagMinDmg = (float)monster->basic.atkMagMinDmg * (100.0f + (float)value * 1.0f) / 100.0f;		break;
		case MONSTER_OPTION_MAX_MAG:	monster->calMagMaxDmg = (float)monster->basic.atkMagMaxDmg  * (100.0f + (float)value * 1.0f) / 100.0f;		break;
		case MONSTER_OPTION_ATT_INC:	monster->calHitPower = (float)monster->basic.atkHitPower * (100.0f + (float)value * 1.0f) / 100.0f;			break;

		case MONSTER_OPTION_ASP_INC:	monster->attackSpeedRate = (float)monster->attackSpeedRate * (100.0f + (float)value * 1.0f) / 100.0f;	break;
		case MONSTER_OPTION_DEX_INC:	monster->calHitRate = (float)monster->calHitRate * (100.0f + (float)value * 1.0f) / 100.0f;			break;

		case MONSTER_OPTION_DEF_INC:	monster->calDefensePower = (float)monster->basic.defPower * (100.0f + (float)value * 1.0f) / 100.0f;	break;
		case MONSTER_OPTION_ESC_INC:	monster->calAvoidPower = (float)monster->basic.defAvoidPower * (100.0f + (float)value * 1.0f) / 100.0f;		break;
		case MONSTER_OPTION_PREG_INC:	monster->calPositiveRes = (float)monster->basic.defPositiveRes * (100.0f + (float)value * 1.0f) / 100.0f;	break;
		case MONSTER_OPTION_NREG_INC:	monster->calNegativeRes = (float)monster->basic.defNegativeRes * (100.0f + (float)value * 1.0f) / 100.0f;	break;
		case MONSTER_OPTION_REG_INC:			break;
		case MONSTER_OPTION_CRTDMG_INC:	monster->calCriticalDmg =  (float)monster->basic.atkCriticalDmg * (100.0f + (float)value * 1.0f) / 100.0f;	break;
		case MONSTER_OPTION_CRTRAT_INC:	monster->calCriticalRate = (float)monster->basic.atkCriticalPro * (100.0f + (float)value * 1.0f) / 100.0f;	break;

		case MONSTER_OPTION_NOSTUN:		monster->calStunTime = 0;					break;


		case PLAYER_OPTION_EXP_INC:								
			monster->basic.exp = (100.0f + (float)value * 0.8f) / 100.0f;	break;
			break;
		case PLAYER_OPTION_GENEXP_INC:							
			monster->basic.genExp = (100.0f + (float)value * 1.5f) / 100.0f;	break;
			break;
		case PLAYER_OPTION_GENSKILL_INC:						
			break;
		case PLAYER_OPTION_NAC_INC:								
			monster->basic.lootSE = (100.0f + (float)value * 1.2f) / 100.0f;	break; 
			break;
		case PLAYER_OPTION_DROPRATE_INC:						
			break;
		default: break;
		}
	}

}

