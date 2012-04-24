
#include "../global.h"


#define ERROR_MESSAGE_STRING	255
#define QUEST_LIMITETIME        1000

#include "../Tools/CTools.h"
extern CTools* gcpTools;

#include "YDCDamageCal.h"


extern playerCharacter_t* GetPlayerRecordPointer(const int Idx);
int LoadMonsterSpawnTableForServer( char *filename, worldTable_t* wt )
{
	int size;
	byte *buffer;
	char* token;
	int idx, wn = -1, i;
	vec3_t pos;
	
	monsterSpawnTable_t			*ms;
	monsterSpawnZoneTable_t		*sz;

	FILE *fp;

	fp = fopen( filename, "rb" );

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	SetTokenBuffer( buffer, size );

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		if( !stricmp( token, "SPAWN_MONSTER_NUMBER" ) )
		{
			token = NextToken();
			wt->monsterSpawnTableNumber = atoi( token );
		} else if( !stricmp( token, "SPAWN_IDX" ) )
		{
			token = NextToken();
			idx = atoi( token );
			ms = &wt->monsterSpawnTable[idx];
			ms->idx = idx;
			wn = -1;
		} else if( !stricmp( token, "ENTITY_TYPE" ) )
		{
			token = NextToken();
			if( !stricmp( token, "MONSTER" ) )
			{
				ms->entityType = ENTITY_MONSTER;
			} else if( !stricmp( token, "NPC" ) )
			{
				ms->entityType = ENTITY_NPC;
			}
		} else if( !stricmp( token, "AI_TYPE" ) )
		{
			token = NextToken();
			ms->aiType = atoi( token );
		} else if( !stricmp( token, "NPC_AI_TYPE" ) )
		{
			token = NextToken();
			ms->npcAiType = atoi( token );
		} else if( !stricmp( token, "WORLD_IDX" ) )
		{
			
			token = NextToken();
			ms->worldIdx = atoi( token );
			
			ms->worldIdx = wt->idx;

		} else if( !stricmp( token, "NPC_TABLE_IDX" ) )
		{
			token = NextToken();
			ms->npcTableIdx = atoi( token );
		} else if( !stricmp( token, "MONSTER_TABLE_IDX" ) )
		{
			token = NextToken();
			ms->monsterTableIdx = atoi( token );
		} 
		
		else if( !stricmp( token, "SMALL_ZONE_FLAG" ) )
		{
			token = NextToken();
			ms->smallZoneFlag = atoi( token );
		}
		else if( !stricmp( token, "SMALL_ZONE_POSITION" ) )
		{
			token = NextToken();
			pos[0] = atof( token );
			token = NextToken();
			pos[1] = atof( token );
			token = NextToken();
			pos[2] = atof( token );
			vec_copy( pos, ms->smallZonePosition );
		}
		else if( !stricmp( token, "RANDOM_POSITION_NUMBER" ) )
		{
			token = NextToken();
			ms->positionNumber = atoi( token );
			for( i = 0; i < ms->positionNumber; i ++ )
			{
				token = NextToken();		
				token = NextToken();
				pos[0] = atof( token );
				token = NextToken();
				pos[1] = atof( token );
				token = NextToken();
				pos[2] = atof( token );
				vec_copy( pos, ms->position[i] );
				token = NextToken();		
				token = NextToken();
				pos[0] = atof( token );
				token = NextToken();
				pos[1] = atof( token );
				token = NextToken();
				pos[2] = atof( token );
				vec_copy( pos, ms->angles[i] );
			}
		} else if( !stricmp( token, "SPAWN_WAYPOINT_NUMBER" ) )
		{
			token = NextToken();
			ms->wayPointNumber = atoi( token );
			for( i = 0; i < ms->wayPointNumber; i ++ )
			{
				token = NextToken();		
				token = NextToken();
				pos[0] = atof( token );
				token = NextToken();
				pos[1] = atof( token );
				token = NextToken();
				pos[2] = atof( token );
				vec_copy( pos, ms->wayPoint[i] );
				token = NextToken();		
				token = NextToken();
				pos[0] = atof( token );
				token = NextToken();
				pos[1] = atof( token );
				token = NextToken();
				pos[2] = atof( token );
				vec_copy( pos, ms->angles[i] );
			}
		} else if( !stricmp( token, "SPAWN_ZONE_IDX_NUMBER" ) )
		{
			token = NextToken();
			ms->spawnZoneNumber = atoi( token );
			for( i = 0; i < ms->spawnZoneNumber; i ++ )
			{
				token = NextToken();		
				token = NextToken();
				ms->spawnZoneIdx[i] = atoi( token );
				token = NextToken();
				ms->spawnZoneProbability[i] = atoi( token );
			}

		} else if( !stricmp( token, "RESPAWN_TIME" ) )
		{
			token = NextToken();
			ms->respawnTime = atoi( token );
		}
		
		else if( !stricmp( token, "SPAWN_ZONE_NUMBER" ) )
		{
			token = NextToken();
			wt->monsterSpawnZoneTableNumber = atoi( token );
		} else if( !stricmp( token, "SPAWN_ZONE_IDX" ) )
		{
			token = NextToken();
			idx = atoi( token );
			sz = &wt->monsterSpawnZoneTable[idx];
			sz->idx = idx;
		} else if( !stricmp( token, "SPAWN_ZONE_POSITION" ) )
		{
			token = NextToken();
			pos[0] = atof( token );
			token = NextToken();
			pos[1] = atof( token );
			token = NextToken();
			pos[2] = atof( token );
			vec_copy( pos, sz->position );			
		} else if( !stricmp( token, "SPAWN_ZONE_ANGLE" ) )
		{
			token = NextToken();
			pos[0] = atof( token );
			token = NextToken();
			pos[1] = atof( token );
			token = NextToken();
			pos[2] = atof( token );
			vec_copy( pos, sz->angle );			
		} 
		else if( !stricmp( token, "SPAWN_ZONE_RANGE" ) )
		{
			token = NextToken();
			sz->range = atoi( token );
		}
	}

	
	if(buffer!=NULL)
	{
		delete []buffer;
		buffer=NULL;
	}
	
	return 1;
}

int GTH_LoadMonsterSpawnTable(worldTable_t *worldTable)
{
	char monsterSpawnTableFile[256];

	if( !stricmp( worldTable->monsterSpawnTableFileName, "NULL" ) )
	{
		worldTable->monsterSpawnNumber = 0;
		return 1;
	}

	sprintf(monsterSpawnTableFile, "tables/%s", worldTable->monsterSpawnTableFileName);
	LoadMonsterSpawnTableForServer( monsterSpawnTableFile, worldTable );
	worldTable->monsterSpawnNumber = worldTable->monsterSpawnTableNumber;

	
	worldTable->specialMonsterCount = 0;
	
	return 1;
}



bool	CheckSameMonsterIndex(int monsterIndex, worldTable_t* pWorld)
{
	for(int i = 0; i < MAX_WORLD_MONSTER_TYPE; i++)
	{
		if( i == pWorld->monsterTypeNum ) break;
		if( pWorld->monsterType[i] == monsterIndex )
			return false;
	}

	return true;
}



void	SetMonsterTable(worldTable_t *pWorld, int worldindex)
{
	int monsterIndex;

	pWorld->monsterTypeNum = 0;
	memset( pWorld->monsterType, -1, sizeof(int) * MAX_WORLD_MONSTER_TYPE);

	for(int i = 0; i < pWorld->monsterSpawnNumber; i++ )
	{
		monsterIndex = pWorld->monsterSpawnTable[i].monsterTableIdx;
		if( CheckSameMonsterIndex(monsterIndex, pWorld) )
		{
			pWorld->monsterType[pWorld->monsterTypeNum] = monsterIndex;
			pWorld->monsterTypeNum++;

		}
	}
}





int GTH_LoadMonster()
{
	int i, j;

	g_monsterNumber = 0;
	for (i=0; i < g_worldTableNumber; i++)
	{

		
		
		if ( !GTH_LoadMonsterSpawnTable(&g_worldTable[i]) )
		{
			continue;
		}

		
		SetMonsterTable(&g_worldTable[i], i);

		
		if ( !GTH_IsLocalServiceWorld( g_worldTable[i].idx ) ) 
		{
			continue;
		}

		for( j = 0; j < g_worldTable[i].monsterSpawnNumber; j ++ )
		{
			if ( g_monsterNumber >= MAX_MONSTERS)
				return 0;

			memset( &g_monster[ g_monsterNumber ], 0, sizeof( monsterCharacter_t ) );
			g_monster[g_monsterNumber].entityType = ENTITY_MONSTER;

			MONSTER_Initialize( &g_monster[ g_monsterNumber ], g_worldTable[i].idx, j );
			g_monsterNumber++;
		}

	}

	return true;
}


void Monster_CalculateValue( monsterCharacter_t* monster )
{

}

void GTH_ExecuteMonsterEvent()
{
	int i;
	monsterCharacter_t *monster;

	monster = &g_monster[0];
	for (i=0; i<MAX_MONSTERS; i++, monster++)
	{
		if( !monster->active ) continue;

		
		
		AI_ReceiveMessage( monster );								
		GTH_AffectProcess( monster, monster->entityType );			

		
		if( monster->event != GTH_EV_CHAR_DIE )
		{
			if( monster->aliveState != MON_ALIVE_STATE_NORMAL || monster->generalSpeed <= 0 ) continue;	
			
			GTH_Monster_SkillCasting( monster );
			GTH_Monster_AutoRecovery( monster );
			
			
			if( monster->recoveryTime < 0.0f ) monster->recoveryTime = 0.0f;
			if( monster->recoveryTime > 0.0f && monster->aliveState != MON_ALIVE_STATE_SPAWN ) 
				continue;
		}

		switch(monster->event)
		{
		case GTH_EV_CHAR_SKIP :
			monster->event = monster->oldEvent;	
			break;

		case GTH_EV_CHAR_IDLE:
			GTH_Monster_Idle( monster );
			break;

		case GTH_EV_CHAR_WALK:
			GTH_Monster_Walk( monster );
			break;

		case GTH_EV_CHAR_BACK:
			GTH_Monster_Back( monster );
			break;

		case GTH_EV_CHAR_TURNLEFT:
			GTH_Monster_TurnLeft( monster );
			break;

		case GTH_EV_CHAR_TURNRIGHT:
			GTH_Monster_TurnRight( monster );
			break;

		case GTH_EV_CHAR_RUN:
			GTH_Monster_Run( monster );
			break;

		case GTH_EV_CHAR_ATTACK:
			GTH_Monster_Attack( monster );
			break;

		case GTH_EV_CHAR_DIE:
			GTH_Monster_Die( monster );
			break;

		case GTH_EV_CHAR_STUN :
			monster->event = GTH_EV_CHAR_IDLE;
			GTH_Monster_Idle( monster );
			break;

		








			break;

		
		case GTH_EV_CHAR_SKILL:
			
			
			if( monster->selectedSkillIdx != -1 )
			{
				for( int i=0; i<monster->skillNumber; i++)
				{
					if( monster->skill[i].tableIdx == monster->selectedSkillIdx )
					{
						GTH_Monster_Skill(monster, i);

						char szTemp[64];
						sprintf( szTemp, "%d, GTH_Monster_Skill, %d\n", monster->idx, monster->selectedSkillIdx );
						OutputDebugString( szTemp );
					}
				}
			}

			
			monster->selectedSkillIdx = -1;

			break;

		default:
			break;
		}

		
		monster->oldEvent = monster->event;
	} 
}



void GTH_Monster_Idle(monsterCharacter_t *m)
{
	
	if ( m->event != m->oldEvent )
		GTH_SendMonsterEventMessage_Idle( m );
}

void GTH_Monster_Walk(monsterCharacter_t *m)
{
	vec3_t delta;
	float speed;
	int isChangeAngle;

	isChangeAngle = false;
	if (m->angles[1] != m->nextAngle)
	{
		isChangeAngle = true;
		m->angles[1] = m->nextAngle;
	}

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, m->angles[0] );
	RotateZCoord( delta, m->angles[1] );
	Normalize( delta );

	speed = SPEED_CONSTANT * m->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(m->position, delta, m->position);
	m->position[2] = GTH_GetHeightFromCollisionMap( m->worldIdx, m->position );


	
	m->zoneIdx = GTH_Zone_UpdateCurrentZone(m->entityType, m->idx, m->worldIdx, m->zoneIdx, m->position);

	if( m->zoneIdx < 0 )
	{
		MONSTER_ForcedDelete( m );
	}

	
	if ( m->event != m->oldEvent || isChangeAngle )
		GTH_SendMonsterEventMessage_Walk( m );
}

void GTH_Monster_Back(monsterCharacter_t *m)
{
	vec3_t delta;
	float speed;

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, m->angles[0] );
	RotateZCoord( delta, m->angles[1] );
	Normalize( delta );

	speed = -1 * BACK_SCALE * SPEED_CONSTANT * m->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(m->position, delta, m->position);
	m->position[2] = GTH_GetHeightFromCollisionMap( m->worldIdx, m->position );

	
	m->zoneIdx = GTH_Zone_UpdateCurrentZone(m->entityType, m->idx, m->worldIdx, m->zoneIdx, m->position);

	if( m->zoneIdx < 0 )
	{
		MONSTER_ForcedDelete( m );
	}


	
	if ( m->event != m->oldEvent )
		GTH_SendMonsterEventMessage_Back( m );
}

void GTH_Monster_Run(monsterCharacter_t *m)
{
	vec3_t delta;
	float speed;
	int isChangeAngle;

	isChangeAngle = false;
	if (m->angles[1] != m->nextAngle)
	{
		isChangeAngle = true;
		m->angles[1] = m->nextAngle;
	}

	
	vec_set(delta, 1, 0, 0);
	RotateYCoord( delta, m->angles[0] );
	RotateZCoord( delta, m->angles[1] );
	Normalize( delta );

	speed = MONSTER_RUN_SCALE * SPEED_CONSTANT * m->calVelocity * g_timer->GetElapsedMilliTime();

	vec_scale(delta, speed, delta); 
	vec_add(m->position, delta, m->position);
	m->position[2] = GTH_GetHeightFromCollisionMap( m->worldIdx, m->position );

	
	m->zoneIdx = GTH_Zone_UpdateCurrentZone(m->entityType, m->idx, m->worldIdx, m->zoneIdx, m->position);

	if( m->zoneIdx < 0 )
	{
		MONSTER_ForcedDelete( m );
	}

	
	if ( m->event != m->oldEvent || isChangeAngle )
		GTH_SendMonsterEventMessage_Run( m );
}



void GTH_Monster_TurnLeft(monsterCharacter_t *m)
{
	m->angles[1] += TURN_SCALE * g_timer->GetElapsedMilliTime();

	if (m->angles[1] >= 360.0f)
		m->angles[1] -= 360.0f;

	
	if ( m->event != m->oldEvent )
		GTH_SendMonsterEventMessage_TurnLeft( m );

}

void GTH_Monster_TurnRight(monsterCharacter_t *m)
{
	m->angles[1] -= TURN_SCALE * g_timer->GetElapsedMilliTime(); 

	if (m->angles[1] < 0)
		m->angles[1] += 360.0f;	

	
	if ( m->event != m->oldEvent )
		GTH_SendMonsterEventMessage_TurnRight( m );
}


void GTH_Monster_Attack( monsterCharacter_t* monster )
{
	message_t message;
	int range;

	if( !AI_IsWithinAttackLength( monster ) ) 
	{
		monster->event = GTH_EV_CHAR_IDLE;
		return;
	}

	monster->recoveryTime = (float)monster->calAttackDelay;

	AI_MakeMessage( monster, &message, MESSAGE_NORMAL_ATTACK );

	range = monster->calAttackRange;
	if( range < MON_MIN_RATTACK_RANGE ) range = 0;
	
	CAISendMessage AIMsg;
	AIMsg.range = range;
	AI_SendMessage( &message, &AIMsg );

	vec3_t dir, target, angles;
	switch( monster->targetType )
	{
	case ENTITY_PC :
		vec_copy( g_pc[monster->targetIdx].position, target );
		break;
	case ENTITY_MONSTER : case ENTITY_NPC :
		vec_copy( g_monster[monster->targetIdx].position, target );
		break;

	}
	vec_sub( target , monster->position, dir );
	dir[2] = 0.0f;
	vectoangles( dir , angles );
	monster->angles[ YAW ] = angles[ YAW ];


	
	GTH_GetPCListInValidRange(monster->worldIdx, monster->position);
	if (g_zonePCListNumber < 1 && monster->entityType != ENTITY_MONSTER) return;	


	GTH_SendMonsterEventMessage_Attack( monster );
	
}



void GTH_Monster_Damage( monsterCharacter_t* monster, message_t* message )
{
	int		dmg, eachDmg[3];
	int		aiFlag			=	false;
	float	hitRate			=	(float)message->data[MSG_ATKDATA_HITRATE];
	float	criticalRate	=	(float)message->data[MSG_ATKDATA_CRITICAL_RATE];

	
	if( monster->damagedEntityIdx < 0 || monster->targetIdx < 0 ) aiFlag = true;
	
	monster->damagedEntityType = message->fromType;
	monster->damagedEntityIdx = message->fromIdx;

	
	if( message->fromType == ENTITY_PC )
	{
		if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_NORMAL )
		{
			hitRate	+= GTH_GetHitRate( &g_pc[message->fromIdx], ENTITY_PC, monster, ENTITY_MONSTER, message->data[MSG_ATKDATA_HITPOWER] );
			criticalRate += GTH_GetCriticalRate( &g_pc[message->fromIdx], ENTITY_PC, monster, ENTITY_MONSTER );
		}
		else if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_MAGIC )
		{
			hitRate += GTH_GetMagicHitRate( &g_pc[message->fromIdx], ENTITY_PC, monster, ENTITY_MONSTER );
			criticalRate = 0;
		}
		else
		{
			criticalRate = 0;
		}
		g_pc[message->fromIdx].resultHitRate = (int)hitRate;
	}
	else
	{
		if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_NORMAL )
		{
			hitRate += GTH_GetHitRate( &g_monster[message->fromIdx], ENTITY_MONSTER, monster, ENTITY_MONSTER, message->data[MSG_ATKDATA_HITPOWER] );
			criticalRate += GTH_GetCriticalRate( &g_monster[message->fromIdx], ENTITY_MONSTER, monster, ENTITY_MONSTER );
		}
		else if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_MAGIC )
		{
			hitRate += GTH_GetMagicHitRate( &g_monster[message->fromIdx], ENTITY_MONSTER, monster, ENTITY_MONSTER );
			criticalRate = 0;
		}
		else
		{
			criticalRate = 0;
		}
	}
	
	
	float fRandCriticalRate = GTH_Random();
	if( fRandCriticalRate < criticalRate * 0.01f )
	{
		message->data[MSG_ATKDATA_STUN_FLAG] = MSG_ATKDATA_STUN_TYPE_CRITICAL;
	}

	
	
	dmg=YDCDamageCal::YDGetDamage( monster->idx, ENTITY_MONSTER, message, eachDmg );


	
	if( GTH_Random() < hitRate * 0.01f ) 
	{
		monster->curDamage = dmg;
	}
	else
	{
		dmg = 0;
		if( message->data[MSG_ATKDATA_ATTACK_TYPE] == MSGINFO_ATK_TYPE_MAGIC ) 
		{
			dmg = (int)( (float)dmg * 0.55f );
		}

		
		if( message->data[MSG_ATKDATA_STUN_FLAG] != MSG_ATKDATA_STUN_TYPE_FREEZE )
			message->data[MSG_ATKDATA_STUN_FLAG] = MSG_ATKDATA_STUN_TYPE_NONE;
		monster->curDamage = dmg;
	}

	if( monster->curDamage > 0 )
	{
		
		GTH_RemoveEntityCurseState( ENTITY_MONSTER, monster->idx, AFF_DATA_TYPE_SLEEP );
		
		GTH_AnalyzeAttackMessageEtcInfo( message, eachDmg );

		
		monster->curRA -= (float)monster->curDamage;
		
		
		GTH_AddAtkContributionPoint( monster, message->fromType, message->fromIdx, dmg );
		
		if( message->fromType == ENTITY_PC )
		{
			GTH_PC_GetStealPoint( &g_pc[message->fromIdx], eachDmg[0] );
		}

		if( (int)monster->curRA <= 0 )
		{
			monster->curRA = 0.0f;
			monster->ai.aiEvent = AI_DIE;
			monster->event = GTH_EV_CHAR_DIE;
			monster->recoveryTime = 0.0f;
			monster->killerIdx = message->fromIdx;
			monster->killerType = message->fromType;
			monster->lastAttackState = message->data[MSG_ATKDATA_STUN_FLAG];
			
			return;
		}		
	}

	
	if( aiFlag ) AI_InterpretImmediately( monster );

	
	
	GTH_SendMonsterEventMessage_Damage( monster, message->data[MSG_ATKDATA_STUN_FLAG] );

	
	
	
	
	
	
	

	
	
	
	
	
	
}

void GTH_Monster_Stun(monsterCharacter_t *m)
{
	
}


int FindRewardField( int index )
{
	for( int i=0; i<QS_REWARD_NUMBER; i++ )
	{
		if ( g_questscrollTable[index].data.rewardField[i] > 0 )
			return i;
	}

	return -1;
}



void GTH_ProcessQuestScrollExecute( monsterCharacter_t *monster, playerCharacter_t *pc )
{
	int i, idx = -1;

	for ( i=0 ; i<MAX_COUNT_QUEST_STATUS ; i++ )
	{
		if ( pc->questStatus[i].questIdx == QUEST_SCROLL_INDEX )	idx = i;
	}

	if ( idx >= 0 )
	{
		if ( monster->tableIdx == pc->questStatus[idx].dField[1] )
		{
			pc->questStatus[idx].dField[3]++;		

			if ( pc->questStatus[idx].dField[3] >= pc->questStatus[idx].dField[2] )		
			{
				
				int reward = pc->questStatus[idx].dField[6];		
				int rewardType, itemNumber;

				switch( pc->questStatus[idx].dField[5] )			
				{
				case 0 :	
					GTH_PC_GetLoot_QuestScroll( pc->idx, reward, 0, 0, 0 );

					rewardType = QS_REWARD_EXP;
					itemNumber = 0;

					break;

				case 1 :	
					GTH_PC_GetLoot_QuestScroll( pc->idx, 0, reward, 0, 0 );

					rewardType = QS_REWARD_GENEXP;
					itemNumber = 0;

					break;
				case 2 :	
					GTH_PC_GetLoot_QuestScroll( pc->idx, 0, 0, 0, reward );

					rewardType = QS_REWARD_GENCAP;
					itemNumber = 0;

					break;

				case 3 :	
					GTH_PC_GetLoot_QuestScroll( pc->idx, 0, 0, reward, 0 );

					rewardType = QS_REWARD_NAK;
					itemNumber = 0;

					break;

				case 4 :	
					{
						int addItemData[4];

						addItemData[0] = reward;							
						addItemData[1] = pc->questStatus[idx].dField[9];	
						addItemData[2] = pc->questStatus[idx].dField[8];	
						addItemData[3] = 0;									

						AI_NPC_AddItem( pc, addItemData );

						rewardType = QS_REWARD_ITEM;
						itemNumber = pc->questStatus[idx].dField[8];
					}

					break;
				}

				int nextScroll = g_questscrollTable[pc->questStatus[idx].dField[0]].data.nextScroll;

				if( nextScroll > 0 )
				{
					int addItemData2[6];

					addItemData2[0] = QUESTSCROLL_ITEM_INDEX;			
					addItemData2[1] = -1;								
					addItemData2[2] = 1;								
					addItemData2[3] = 0;								
					addItemData2[4] = nextScroll;						
					addItemData2[5] = FindRewardField( nextScroll );	

					
					if ( addItemData2[5] < 0 )
					{
						
						g_logSystem->Write("Quest Scroll Table has an Error! (User: %s, Index : %d)", pc->name, nextScroll );
						
						pc->questStatus[idx].dField[3]--;					
						return;
					}

					AI_NPC_AddItem( pc, addItemData2 );
				}

				pc->rankPoint += pc->questStatus[idx].dField[7];	

				GTH_ClearQuestStatus( pc, QUEST_SCROLL_INDEX, 1 );
				GTH_SendMessage_QuestScrollComplete( pc, rewardType, reward, itemNumber, pc->rankPoint, nextScroll );

			}
			else
			{
				g_DBGameServer->SaveQuestStatus( pc, &pc->questStatus[idx] );
				GTH_SendMessage_QuestStatusInfo( pc, 3, QUEST_SCROLL_INDEX );
			}
		}
	}
}


int AI_NPC_AutoCompleteQuest(playerCharacter_t *pPc, int *pParam )
{
	int questIdx = pParam[0];
	int autoquestindex = pParam[1];
	int talbe=questIdx+autoquestindex;
	pPc->questVar[talbe].var.isComplete=1;
	pPc->questVar[talbe].var.completeCount+=1;
	
	if( pPc->questVar[talbe].var.completeCount > MAX_COUNT_QUEST_COMPLETE )
		pPc->questVar[talbe].var.completeCount = MAX_COUNT_QUEST_COMPLETE;
	
	
	g_DBGameServer->SaveQuest( pPc );
	
	GTH_AutoClearQuestStatus( pPc, questIdx, 1 );

	return 0;
}



void GTH_ProcessAutoQuestScrollExecute( monsterCharacter_t* pMonster, playerCharacter_t* pPlayer)
{

	int nQuestArrayIndex = -1;
   
	for ( int nIdx = 0 ; nIdx < MAX_COUNT_QUEST_STATUS ; nIdx++ ) {
		
		if ( pPlayer->questStatus[nIdx].questIdx != AUTOQUESTSCROLL_ITEM_INDEX ) continue;
		
		nQuestArrayIndex = nIdx;
		break;
	}
	
	if( -1 == nQuestArrayIndex ) return;


	
	if ( pMonster->tableIdx != pPlayer->questStatus[nQuestArrayIndex].dField[1] ) return;


	
	pPlayer->questStatus[nQuestArrayIndex].dField[3]++;	

	
	if ( pPlayer->questStatus[nQuestArrayIndex].dField[3] < pPlayer->questStatus[nQuestArrayIndex].dField[2] ) {
		
		g_DBGameServer->SaveQuestStatus( pPlayer, &pPlayer->questStatus[nQuestArrayIndex] );
		
		GTH_SendMessage_AutoQuestStatusInfo( pPlayer, 3,  pPlayer->questStatus[nQuestArrayIndex].dField[0] );
			return;
		
	}
		
	

	switch( pPlayer->questStatus[nQuestArrayIndex].dField[4] ) {
	
	case 1:
		break;
	case 3:
		
		if( pPlayer->questStatus[nQuestArrayIndex].clearTime < pPlayer->questStatus[nQuestArrayIndex].limitTime) {
	
			int addItemData[4];
			
			addItemData[0] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].dataField[15];	
			addItemData[1] = -1;																						
			addItemData[2] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].dataField[16];	
			addItemData[3] = 0;																							
			
			AI_NPC_AddItem( pPlayer, addItemData );
			ShowLogInfo("autoQuest item reword name: %s,index : %d,option : %d,count : %d",pPlayer->name,addItemData[0],addItemData[1],-1);
		}
		
		break;

	case 2: case 4:
		
		g_DBGameServer->SaveQuestStatus( pPlayer, &pPlayer->questStatus[nQuestArrayIndex] );
		GTH_SendMessage_AutoQuestStatusInfo( pPlayer, 3,  pPlayer->questStatus[nQuestArrayIndex].dField[0] );
	
		return;

	}

	GTH_RewordAutoQuestScroll(pPlayer,nQuestArrayIndex);
}



void GTH_RewordAutoQuestScroll(playerCharacter_t* pPlayer,const int& nQuestArrayIndex)
{
	
	int exp = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].dataField[10];

	int genexp = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].dataField[11];
	
	int gencap = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].dataField[12];
	
	int nak = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].dataField[13];
	
	int fame = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].dataField[18];
	
	int chaoPoint = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].dataField[21];
	
	GTH_PC_GetLoot_AutoQuestScroll( pPlayer, exp, genexp, gencap, nak,fame,chaoPoint );
	
	
	if(g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.nextScroll != -1) {

		int addItemData[5];
		int nNextScroll = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.nextScroll;
	
		addItemData[0] = AUTOQUESTSCROLL_ITEM_INDEX;	
		addItemData[1] = -1;							
		addItemData[2] = 1;								
		addItemData[3] = 1;								
		addItemData[4] = nNextScroll;	
		addItemData[5] = -1;							
		AI_NPC_AddItem( pPlayer, addItemData );
		
		ShowLogInfo(
			"autoQuest item reword name: %s,index : %d,count : %d,option : %d",
			pPlayer->name,
			addItemData[0],
			addItemData[1],
			addItemData[2]);
	}
	
	if(g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.itemIdx != -1) {
		
		int addItemData[4];
		addItemData[0] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.itemIdx;			
		addItemData[1] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.optionGroup;								
		addItemData[2] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.itemNo;								
		addItemData[3] = 0;							
		AI_NPC_AddItem( pPlayer, addItemData );
	
		ShowLogInfo(
			"autoQuest item reword name: %s,index : %d,count : %d,option : %d",
			pPlayer->name,
			addItemData[0],
			addItemData[1],
			addItemData[2]);
	}
	
	
	
	if(g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemIndex0 != -1) {
	
		int addItemData[4];
		addItemData[0] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemIndex0;			
		addItemData[1] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemOption0;								
		addItemData[2] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemNo0;								
		addItemData[3] = 0;								
		AI_NPC_AddItem( pPlayer, addItemData );
		
		ShowLogInfo(
			"autoQuest item reword name: %s,index : %d,count : %d,option : %d",
			pPlayer->name,
			addItemData[0],
			addItemData[1],
			addItemData[2]);
	}
	
	if(g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemIndex1 != -1) {
	
		int addItemData[4];
		addItemData[0] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemIndex1;			
		addItemData[1] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemOption1;								
		addItemData[2] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemNo1;								
		addItemData[3] = 0;								
		AI_NPC_AddItem( pPlayer, addItemData );
		
		ShowLogInfo(
			"autoQuest item reword name: %s,index : %d,count : %d,option : %d",
			pPlayer->name,
			addItemData[0],
			addItemData[1],
			addItemData[2]);
	}
	
	if(g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemIndex2 != -1) {
	
		int addItemData[4];
		addItemData[0] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemIndex2;			
		addItemData[1] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemOption2;								
		addItemData[2] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemNo2;								
		addItemData[3] = 0;								
		AI_NPC_AddItem( pPlayer, addItemData );
		
		ShowLogInfo(
			"autoQuest item reword name: %s,index : %d,count : %d,option : %d",
			pPlayer->name,
			addItemData[0],
			addItemData[1],
			addItemData[2]);
	}
	
	if(g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemIndex3 != -1) {
		
		int addItemData[4];
		addItemData[0] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemIndex3;			
		addItemData[1] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemOption3;								
		addItemData[2] = g_autoQuestscrollTable[pPlayer->questStatus[nQuestArrayIndex].dField[0]].data.reward.regItemNo3;								
		addItemData[3] = 0;								
		AI_NPC_AddItem( pPlayer, addItemData );
		ShowLogInfo(
			"autoQuest item reword name: %s,index : %d,count : %d,option : %d",
			pPlayer->name,
			addItemData[0],
			addItemData[1],
			addItemData[2]);
	}
	
	int pParam[2];
	pParam[0] = AUTOQUESTSCROLL_ITEM_INDEX;
	pParam[1] = pPlayer->questStatus[nQuestArrayIndex].dField[0];
	AI_NPC_AutoCompleteQuest(pPlayer, pParam );
	pPlayer->bautoeffectisvalid = TRUE; 
	GTH_SendMessage_AutoQuestScrollComplete( pPlayer,  pPlayer->questStatus[nQuestArrayIndex].dField[0]);
}




void GTH_Monster_Loot( monsterCharacter_t *monster )
{
	
	playerCharacter_t *pc=NULL;
	
	
	

	int exp=0, genExp=0, lootNSE=0, genCapability=0;
	float lootRate;
	int levelDiffer;

	int entityIdx, entityType, contributionCount = 0;
	
	int contributionPartyIdx[MAX_NUMBER_OF_PARTY_MEMBER], partyIdxNumber = 0, firstPartyMemberIdx=-1;
	
	int maxPartyContributionPoint=0, maxAtkConPoint, maxDefConPoint, atkConIdx, defConIdx;
	int i;
	int maxAtkConPointForQuestItem = 0, maxAtkConIdxForQuestItem = -1;

	monster->mostConEntityType = -1;
	monster->mostConEntityIdx = -1;
	
	
	if( monster->maxAtkContributionPoint <= 0 )
	{

		return;
	}

	
	
	

	int findcount = 0;
	
	for(  i=0; i<MAX_NUMBER_OF_MONSTER_CONTRIBUTION; i++)
	{			
		if ( findcount >= monster->conInfoNumber) break;
		if ( monster->conInfo[i].entityIdx < 0) continue;
		findcount++;
	

		
		entityType = monster->conInfo[i].entityType;
		entityIdx = monster->conInfo[i].entityIdx;

		
		if( monster->conInfo[i].atkPoint > maxAtkConPointForQuestItem ) 
		{
			maxAtkConPointForQuestItem = monster->conInfo[i].atkPoint;
			maxAtkConIdxForQuestItem = i;
		}

		if( entityType != ENTITY_PC )
		{
			monster->conInfo[i].flag = true;
			contributionCount ++;

			continue;
		}
		else if( g_pc[entityIdx].organizeServer < 0 )
		{
			monster->conInfo[i].flag = true;
			contributionCount ++;

			if( !GTH_IsValidContributionPoint( monster, &g_pc[entityIdx], i ) ) continue;
			
			lootRate = (float)monster->conInfo[i].atkPoint / (float)monster->maxAtkContributionPoint;

			exp = (int)( (float)monster->basic.exp * lootRate );
			genExp = (int)( (float)monster->basic.genExp * lootRate );
			lootNSE = (int)( (float)monster->basic.lootSE * lootRate );
			genCapability = (int)( (float)g_genCapabilityTable.genCapability[monster->basic.level] * lootRate );

			levelDiffer = g_pc[entityIdx].level - monster->basic.level;
			GTH_GetExpFromLevelDiffer( levelDiffer, exp, genExp, genCapability );
			
			GTH_PC_GetLoot( entityIdx, exp, genExp, 0, genCapability );
		}
	}

	i = 0;
	
	while( contributionCount < monster->conInfoNumber )
	{
		
		if( monster->conInfo[i].flag ) 
		{
			i ++;
			continue;
		}

		monster->conInfo[i].flag = true;
		
		pc=gcpTools->GetPlayerRecordPointer(monster->conInfo[i].entityIdx);
		if(NULL==pc)continue;
		firstPartyMemberIdx = pc->idx;
		

		contributionPartyIdx[0] = i;
		maxPartyContributionPoint = monster->conInfo[i].atkPoint;
		partyIdxNumber = 1;
		maxAtkConPoint = 0;
		maxDefConPoint = 0;
		atkConIdx = -1;
		defConIdx = -1;

		contributionCount ++;

		
		
	
		
		int findcount = 0;					
		for( int j = i+1; j < MAX_NUMBER_OF_MONSTER_CONTRIBUTION; j ++)
		{
			if ( findcount >= monster->conInfoNumber) break;
			if ( monster->conInfo[j].entityIdx < 0) continue;
			findcount++;
		
			
		
			if( monster->conInfo[j].flag ) continue;
			entityIdx = monster->conInfo[j].entityIdx;

			if( !GTH_CheckSameParty( &g_pc[firstPartyMemberIdx], &g_pc[entityIdx] ) ) continue;
			
			monster->conInfo[j].flag = true;
			contributionPartyIdx[partyIdxNumber] = j;
			maxPartyContributionPoint += monster->conInfo[j].atkPoint;
			partyIdxNumber ++;

			contributionCount ++;
		}
		
		for( j = 0; j < partyIdxNumber; j ++ )
		{
			if( monster->conInfo[contributionPartyIdx[j]].atkPoint > maxAtkConPoint )
			{
				atkConIdx = j;
				maxAtkConPoint = monster->conInfo[contributionPartyIdx[j]].atkPoint;
			}
			if( monster->conInfo[contributionPartyIdx[j]].defPoint > maxDefConPoint )
			{
				defConIdx = j;
				maxDefConPoint = monster->conInfo[contributionPartyIdx[j]].defPoint;
			}
		}
		lootRate = (float)maxPartyContributionPoint / (float)monster->maxAtkContributionPoint;
		exp = (int)( (float)monster->basic.exp * lootRate );
		genExp = (int)( (float)monster->basic.genExp * lootRate );
		lootNSE = (int)( (float)monster->basic.lootSE * lootRate );
		genCapability = (int)( (float)g_genCapabilityTable.genCapability[monster->basic.level] * lootRate );

		
		GTH_SharingPartyLoot( monster, &g_pc[firstPartyMemberIdx], exp, genExp, 0, genCapability, atkConIdx, defConIdx );
	}

	monster->mostConEntityIdx = -1;
	monster->mostConEntityType = -1;

	if( maxAtkConIdxForQuestItem >= 0 )
	{
		monster->mostConEntityIdx = monster->conInfo[maxAtkConIdxForQuestItem].entityIdx;
		monster->mostConEntityType = monster->conInfo[maxAtkConIdxForQuestItem].entityType;
		strcpy( monster->mostConEntityName, monster->conInfo[maxAtkConIdxForQuestItem].entityName );
	}

	
	
	if( monster->mostConEntityType == ENTITY_PC )
	{
		GTH_GenerateQuestItemFromMonster( monster, maxAtkConIdxForQuestItem );
		GTH_GenerateItemFromMonster( monster );

		
		entityIdx = monster->conInfo[maxAtkConIdxForQuestItem].entityIdx;
		if( !g_pc[entityIdx].active || !g_pc[entityIdx].alive ) return;
		if( stricmp( g_pc[entityIdx].name, monster->conInfo[maxAtkConIdxForQuestItem].entityName ) ) return;

		g_pc[entityIdx].curRA += g_pc[entityIdx].optValue[OV_RA_STEAL_DIE_MONSTER];
		g_pc[entityIdx].curSA += g_pc[entityIdx].optValue[OV_SA_STEAL_DIE_MONSTER];
		
		
		
		GTH_ProcessQuestScrollExecute( monster, &g_pc[entityIdx] );
	    
		
		GTH_ProcessAutoQuestScrollExecute( monster, &g_pc[entityIdx] );

		
		



		{		
			int change_point=(exp/((monster->basic.level*CHAOSADDRATE)+CHAOSGENERATE));
			gcpTools->IncDecChaosPoint(&(g_pc[entityIdx]),change_point* (-1));
		}

		


		

	

	}
}

void GTH_Monster_Die(monsterCharacter_t *monster )
{
	
	if ( monster->aliveState != MON_ALIVE_STATE_NORMAL ) return; 

	AI_InitMessage( monster );
	MONSTER_Calculate( monster );

	monster->aliveState = MON_ALIVE_STATE_CORPSE;
	monster->deathTime = g_globalTime;

	
	if ( monster->event != monster->oldEvent )
	{
		
		GTH_Monster_Loot( monster );
		GTH_SendMonsterEventMessage_Die( monster );
	}

	
	
	if( monster->specialMonster == 1 ) 
	{
		--g_worldTable[monster->spawnWorldIdx].specialMonsterCount;
		monster->specialMonster = 0;
	}
}


void GTH_Monster_AutoRecovery( monsterCharacter_t *m )
{
	float recRAPoint = 0.0f, recSAPoint = 0.0f;

	
	m->autoRecoveryRATime -= g_timer->GetElapsedMilliTime();
	m->autoRecoverySATime -= g_timer->GetElapsedMilliTime();

	if( m->autoRecoveryRATime < 0.0f )
	{
		recRAPoint = m->basic.recoveryRARate;
		m->autoRecoveryRATime = MONSTER_AUTO_RECOVERY_DELAY;
	}
	if( m->autoRecoverySATime < 0.0f )
	{
		recSAPoint = m->basic.recoverySARate;
		m->autoRecoverySATime = MONSTER_AUTO_RECOVERY_DELAY;
	}
	m->curRA += recRAPoint;
	m->curSA += recSAPoint;

	if( m->curRA > m->calMaxRA ) m->curRA = m->calMaxRA;
	if( m->curSA > m->calMaxSA ) m->curSA = m->calMaxSA;
}



void GTH_Monster_Skill(monsterCharacter_t *monster, int skillIndex)
{

	int castingTime;
	float value[NUM_OF_SKILL_VALUE];

	int tableIdx = monster->skill[skillIndex].idx;

	if( tableIdx == - 1) return;

	
	g_SkillLogic.SKILL_Monster_GetValue( monster, tableIdx, value );
	castingTime = (int)value[1];

	monster->curSA -= value[0];

	if( (int)monster->curSA < 0 ) monster->curSA = 0.0f;

	
	if( castingTime > 0 )
	{
		monster->skillCastingtime = (float)castingTime;
		monster->skillCastingState = false;
		monster->event = GTH_EV_CHAR_IDLE;
		monster->saveSkillIdx = skillIndex;
		GTH_SendMonsterEventMessage_SkillCasting( monster, castingTime);
		
	}
	else
	{
		monster->event = GTH_EV_CHAR_SKILL;
		monster->saveSkillIdx = skillIndex;
		return;
	}
}

void GTH_Monster_SkillCasting( monsterCharacter_t* pMonster )
{
	pMonster->skillCastingtime -= g_timer->GetElapsedMilliTime();
	if( pMonster->skillCastingtime < 0.0f )
	{
		pMonster->skillCastingtime = 0.0f;
		pMonster->skillCastingState = true;

		if( pMonster->saveSkillIdx >= 0 ) GTH_Monster_SkillExecute( pMonster );
		pMonster->saveSkillIdx = -1;
	}
}

void GTH_Monster_SkillExecute( monsterCharacter_t *pMonster )
{
	int coolTime;

	if( pMonster->saveSkillIdx < 0 )		return;
	if( pMonster->skill[pMonster->saveSkillIdx].tableIdx  < 0 )	return;

	MONSTER_Calculate( pMonster );

	
	pMonster->recoveryTime = pMonster->calAttackDelay;

	if( pMonster->basic.skillNumber == 0 )	return;	

	
	if( !AI_IsWithinAttackLength( pMonster ) )
	{
		pMonster->event = GTH_EV_CHAR_IDLE;
		return;
	}

	
	g_SkillLogic.SKILL_Monster_ApplyActive( pMonster, pMonster->saveSkillIdx, coolTime );
	GTH_SendMonsterEventMessage_Skill( pMonster, pMonster->saveSkillIdx, coolTime, pMonster->recoveryTime );

}



int	GTH_ProcessMessage_SummonMonster(item_t* pItem)
{
	monsterCharacter_t* monster = NULL;
	int localWorldIdx;

	int idx, i;
	char summonerName[255];

	idx = MONSTER_GetNotActiveIdx();
	if( idx < 0 ) return -1;
	
	localWorldIdx = GTH_GetLocalWorldIdx( g_curPC->worldIdx );
	if( !AI_CheckCollision( g_curPC->position, &g_cmap[localWorldIdx] ) ) return -2;

	monster = &g_monster[idx];
	monster->worldIdx = g_curPC->worldIdx;

	memset( monster, 0, sizeof( monsterCharacter_t ) );

	monster->idx = idx;

	
	monster->tableIdx = g_summonMonstertable[pItem->optionIdx[0]].idx;

	monster->spawnWorldIdx = g_curPC->worldIdx;
	monster->spawnTableIdx = -1;
	
	monster->respawnTime = -1.0f;

	memcpy( &monster->basic, &g_monsterTable[monster->tableIdx], sizeof( monsterBasicTable_t ) );	

	monster->entityType = ENTITY_MONSTER;

	monster->ai.aiType = -1;
	monster->npcAi.aiType = -1;
	monster->npcTableIdx = -1;
	monster->npc.scriptFlag = false;
	
	
	
	
	
	monsterBasicTable_t		*mb;
	mb = &g_monsterTable[monster->tableIdx];

	monster->skillNumber = mb->skillNumber;	
	for(int skillidx = 0; skillidx< monster->skillNumber; skillidx++)
	{
		monster->skill[skillidx].idx		= mb->skillIdx[skillidx];
		monster->skill[skillidx].tableIdx	= mb->skillIdx[skillidx];
	}
	monster->saveSkillIdx = -1;



	int ScriptIdx = g_monsterTable[monster->tableIdx].AIScriptIdx;
	AI_LoadScript( monster, g_scriptInfo.aiScript[ScriptIdx].filename );

	
	AI_PreparePathfinding( monster );

	monster->deathTime = 0;
	monster->worldIdx = monster->spawnWorldIdx;

	vec3_t			position;

	
	if ( g_curPC != NULL ) 
	{
		if( FindMonsterSpawnPlace(localWorldIdx, position ) == -1)
		{
			
			return -2;
		}
	}

	vec_copy(g_curPC->position, position);
	vec_copy( position, monster->position );

	monster->aliveState = MON_ALIVE_STATE_SPAWN;
	monster->recoveryTime = MONSTER_SPAWN_ANIMATION_TIME;

	
	MONSTER_Calculate( monster );

	monster->curRA = monster->calMaxRA;
	monster->curSA = monster->calMaxSA;

	monster->totalOptNum  = 0;
	monster->playerOptNum = 0;

	
	










	
	SetMonsterOptionValue(monster);

	monster->event = GTH_EV_CHAR_IDLE;
	monster->oldEvent = GTH_EV_CHAR_DIE;

	
	for ( i =0; i < MAX_AFFECT; i++ )
	{
		memset( &monster->affect[i], 0, sizeof( affect_t ) );
		monster->affect[i].type = AFFECT_NONE;
	}
	monster->affectNumber = 0;

	GTH_InitContributionPoint( monster );

	sprintf( summonerName, "%s", g_monster[monster->tableIdx].basic.name );
	g_logSystem->Write( "Monster Summon Complete : %s(%d) Success...pos(%.2f, %.2f, %.2f), Summoner : (%s)",
		monster->basic.name, monster->idx, monster->position[0], monster->position[1], monster->position[2], summonerName );

	
	MONSTER_InitAI( monster );
	AI_InitMessage( monster );

	
	monster->isSummon = true;
	monster->summonTime = g_globalTime;

	
	
	

	
	monster->summonValidTime = (int)g_summonMonstertable[pItem->optionIdx[0]].SummonTime * 1000;
	

	monster->active = 1;
	monster->zoneIdx = -1;

	monster->zoneIdx = GTH_Zone_GetZoneIdx( monster->position );
	GTH_Zone_AddEntityList(monster->worldIdx, monster->zoneIdx, monster->entityType, monster->idx);

	return idx;
}



bool	CanSummonMonster(int monsterIndex)
{
	for(int i = 0; i < g_worldTable[g_curPC->worldIdx].monsterTypeNum; i++ )
	{
		
		if( g_worldTable[g_curPC->worldIdx].monsterType[i] == monsterIndex )		
			return true;
	}

	return false;
		
}


int		FindMonsterSpawnPlace(int localWorldIdx, vec3_t& position )
{
	vec_copy( g_curPC->position, position );

	int rx = GTH_Randomize( 3 ) - 1;
	int ry = GTH_Randomize( 3 ) - 1;
	
	position[2] = GTH_GetHeightFromCollisionMap(g_curPC->worldIdx, g_curPC->position);
	position[0] += (float)rx * 80.0f;
	position[1] += (float)ry * 80.0f;

	if( !AI_CheckCollision(position, &g_cmap[localWorldIdx]) )
	{
		return -1;
	}

	return 1;
}



void GTH_Monster_State(monsterCharacter_t *m)
{

}









int	 AddMonster_ContributionPoint( playerCharacter_t *pPc, int monsterIdx,  int PointType,  int point)
{

	
	if ( monsterIdx < 0 || monsterIdx > MAX_MONSTERS ) return 0;	

	
	if ( pPc == NULL ) return 0;

	
	if ( PointType > TOTAL_POINT) return 0;

	monsterCharacter_t* pMonster = NULL;
	pMonster = &g_monster[monsterIdx];
	
	if ( pMonster == NULL )
		return 0;
	
	
	bool findPc = 0;	

	
	int findidx		= -1;
	int findCount	= 0;
	bool flagfind	= 0;

	for (int ConIdx = 0; ConIdx < MAX_NUMBER_OF_MONSTER_CONTRIBUTION; ConIdx++)
	{		
		if ( pMonster->conInfo[ConIdx].entityIdx >= 0 )
		{	
			if ( pMonster->conInfo[ConIdx].entityIdx == pPc->idx &&
			pMonster->conInfo[ConIdx].entityType == pPc->entityType )	
			{
				findPc = 1;
				findidx = ConIdx;
				break;	
			}
		}		
		else
		{	
			if ( findidx < 0)
			findidx = ConIdx;			
		}				
	}	
	
	


	










	

	if ( findidx < 0 || findidx >= MAX_NUMBER_OF_MONSTER_CONTRIBUTION) return false;
	
	
	if ( !findPc )
	{
		

		pMonster->conInfo[findidx].entityType = pPc->entityType;
		pMonster->conInfo[findidx].entityIdx = pPc->entityType;	

		if( pPc->entityType == ENTITY_PC ) 
			strcpy( pMonster->conInfo[findidx].entityName, pPc->name);

		pMonster->conInfo[findidx].atkPoint = point;
		pMonster->conInfo[findidx].defPoint = 0;

		pMonster->conInfoNumber++;
	}
	else	
		pMonster->conInfo[findidx].atkPoint += point;
	

	pMonster->conInfo[findidx].flag = false;		
	pMonster->conInfo[findidx].time = g_globalTime;			
	
	
	switch ( PointType )
	{
	case FIRST_ATTACK_POINT:
		pMonster->conInfo[findidx].ContributionPoints[FIRST_ATTACK_POINT] = pMonster->conInfo[findidx].atkPoint * 1.8f;
		break;

	case MYSTERY_POINT:
		if ( pMonster->message[findidx].data[MSG_ATKDATA_ETC_DATA_2] == MSGINFO_TYPE_ATTACK )
		pMonster->conInfo[findidx].ContributionPoints[MYSTERY_POINT] +=  pMonster->message[pMonster->messageNumber].data[MSG_ATKDATA_PHYSIC_DMG];
		break;	

	default:
		pMonster->conInfo[findidx].ContributionPoints[PointType] += point;		

	}

	pMonster->conInfo[findidx].ContributionPoints[TOTAL_POINT] += pMonster->conInfo[findidx].ContributionPoints[FIRST_ATTACK_POINT];
	pMonster->conInfo[findidx].ContributionPoints[TOTAL_POINT] += pMonster->conInfo[findidx].ContributionPoints[FIRST_ATTACK_REVERSE_POINT];
	pMonster->conInfo[findidx].ContributionPoints[TOTAL_POINT] += pMonster->conInfo[findidx].ContributionPoints[MYSTERY_POINT];
	pMonster->conInfo[findidx].ContributionPoints[TOTAL_POINT] += pMonster->conInfo[findidx].ContributionPoints[RECOVER_POINT];	
	pMonster->conInfo[findidx].ContributionPoints[TOTAL_POINT] += pMonster->conInfo[findidx].ContributionPoints[ATTACK_POINT];					
	return 1;
}

void GTH_Process_QuestlimiteTimerCheck()
{
  	for(int i=0; i<g_pcNumber; i++)
	{
		if(g_pc[i].active == 1  && g_pc[i].questStatus[i].limitTime > 0 )
		{
			
			g_pc[i].questStatus[i].clearTime+=g_timer->GetElapsedTime();
			
			if(g_pc[i].questStatus[i].clearTime > g_pc[i].questStatus[i].limitTime)
			{
				
				GTH_AutoClearQuestStatus( g_pc, AUTOQUESTSCROLL_ITEM_INDEX, -1 );		
			}
		
		}
	}
}







