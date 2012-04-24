#include "../global.h"

void GTH_DeleteAffectAll( void *ent, int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		for( int i = 0; i < MAX_AFFECT; i ++ )
		{
			memset( &pc->affect[i], 0, sizeof( affect_t ) );
			pc->affect[i].type = AFFECT_NONE;
		}
		pc->affectNumber = 0;

		
		
		pc->isTransform = false;
	}
	else 
	{
		m = static_cast<monsterCharacter_t*>(ent);
		for( int i = 0; i < MAX_AFFECT; i ++ )
		{
			memset( &m->affect[i], 0, sizeof( affect_t ) );
			m->affect[i].type = AFFECT_NONE;
		}
		m->affectNumber = 0;
	}
}


void GTH_DeleteAffect( void *ent, int entType, int skillTableIdx )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		for( int i = 0; i < pc->affectNumber; i ++ )
		{
			if( pc->affect[i].skillTableIdx == skillTableIdx )
			{
				pc->affect[i].forcedDelete = true;
				break;
			}
		}
	}
	else 
	{
		m = static_cast<monsterCharacter_t*>(ent);
		for( int i = 0; i < m->affectNumber; i ++ )
		{
			if( m->affect[i].skillTableIdx == skillTableIdx )
			{
				m->affect[i].forcedDelete = true;
				break;
			}
		}
	}
}


bool GTH_IsActiveAffect( void *ent, int entType, int skillTableIdx )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		for( int i = 0; i < pc->affectNumber; i ++ )
		{
			if( pc->affect[i].skillTableIdx == skillTableIdx )			
				return 1;							
		}
	}
	else 
	{
		m = static_cast<monsterCharacter_t*>(ent);
		for( int i = 0; i < m->affectNumber; i ++ )
		{
			if( m->affect[i].skillTableIdx == skillTableIdx )			
				return 1;							
		}
	}
	return 0;
}




void GTH_DeleteTransform( void *ent, int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		for( int i = 0; i < pc->affectNumber; i ++ )
		{
			if( pc->affect[i].type == AFFECT_SPECIAL && pc->affect[i].spe.specialType == 0 )
			{
				pc->affect[i].forcedDelete = true;
				break;
			}
		}
	}
	else 
	{
		m = static_cast<monsterCharacter_t*>(ent);
		for( int i = 0; i < m->affectNumber; i ++ )
		{
			if( m->affect[i].type == AFFECT_SPECIAL && m->affect[i].spe.specialType == 0 )
			{
				m->affect[i].forcedDelete = true;
				break;
			}
		}
	}
}

void GTH_DeleteToggle( void *ent, int entType, int skillTableIdx )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		for( int i = 0; i < pc->affectNumber; i ++ )
		{
			if( pc->affect[i].type == AFFECT_TOGGLE && pc->affect[i].skillTableIdx == skillTableIdx )
			{
				pc->affect[i].forcedDelete = true;
				break;
			}
		}
	}
	else 
	{
		m = static_cast<monsterCharacter_t*>(ent);
		for( int i = 0; i < m->affectNumber; i ++ )
		{
			if( m->affect[i].type == AFFECT_TOGGLE )
			{
				m->affect[i].forcedDelete = true;
				break;
			}
		}
	}
}

void GTH_AffectTransform( void *ent, int transformIdx, int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		switch( transformIdx )
		{
			
		case 35 :
			{
				pc->isTransform = true;
				pc->transformEntityIdx = 10;
				pc->transformType = TRANSFORM_ETHEREAL;
			}
			break;
			
		case 70 :
			{
				pc->isTransform = true;
				pc->transformEntityIdx = 2;
				pc->transformType = TRANSFORM_NORMAL;
			}
			break;

		}
	}
	else 
	{
		m = static_cast<monsterCharacter_t*>(ent);
	}
}

void GTH_Affect( void *ent, message_t *msg, int entType )
{
	int i, j;
	float hitRate;
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	hitRate = (float)msg->data[MSG_AFFDATA_HITRATE];

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		
		if( msg->fromType == ENTITY_PC )
		{
			hitRate += GTH_GetAffectHitRate( &g_pc[msg->fromIdx], ENTITY_PC, pc, ENTITY_PC, msg->data[MSG_AFFDATA_AFFECT_HITTYPE] );
		}
		else
		{
			hitRate += GTH_GetAffectHitRate( &g_monster[msg->fromIdx], ENTITY_MONSTER, pc, ENTITY_PC, msg->data[MSG_AFFDATA_AFFECT_HITTYPE] );
		}
		if( GTH_Random() > hitRate * 0.01f ) return;

		for( i = 0; i < MAX_AFFECT; i ++ )
		{
			if( pc->affect[i].type == AFFECT_NONE ) break;

			
			
			if( pc->affect[i].skillTableIdx == msg->data[MSG_AFFDATA_SKILLTABLEIDX] )
			{
				
				if( pc->affect[i].type == AFFECT_TOGGLE )
				{
					
					
					GTH_DeleteToggle( pc, ENTITY_PC, pc->affect[i].skillTableIdx );
					return;
				}
				
				if( pc->affect[i].level <= msg->data[MSG_AFFDATA_LEVEL] )
				{
					pc->affect[i].startTime = g_globalTime;
					pc->affect[i].keepTime = msg->data[MSG_AFFDATA_KEEPTIME];
					
					
					if( pc->affect[i].type == AFFECT_TOGGLE )
					{
						pc->affect[i].raCostPerSec = msg->data[MSG_AFFDATA_RACOST_PER_SEC];
						pc->affect[i].saCostPerSec = msg->data[MSG_AFFDATA_SACOST_PER_SEC];
						pc->affect[i].delayTime	   = msg->data[MSG_AFFDATA_DELAY_TIME];
					}

					
					if( msg->data[MSG_AFFDATA_AFFECT_TYPE] == AFFECT_SPECIAL )
					{
						pc->affect[i].keepTime += pc->optValue[OV_POLYMORPH_KEEPTIME];
						pc->affect[i].keepTime += (int)( (float)( msg->data[MSG_AFFDATA_KEEPTIME] - g_globalTime ) * pc->optValue[OV_POLYMORPH_KEEPTIME_RATE] * 0.01f );
					}

					pc->affect[i].skillTableIdx = msg->data[MSG_AFFDATA_SKILLTABLEIDX];
					
					for( j = 0; j < MAX_COUNT_AFFECT_DATA; j ++ )
					{
						pc->affect[i].data[j] = msg->data[j + CONVERT_MSG_TO_AFFECT_SIZE];
					}
					
					
					pc->affect[i].level = msg->data[MSG_AFFDATA_LEVEL];
					pc->affect[i].flagApply = false;
					
					return;
				}
				
				else if( pc->affect[i].level > msg->data[MSG_AFFDATA_LEVEL] )
				{
					return;
				}
			}
		}
		if( i == MAX_AFFECT ) return;
		
		
		pc->affect[i].type = msg->data[MSG_AFFDATA_AFFECT_TYPE];
		pc->affect[i].startTime = g_globalTime;
		pc->affect[i].level = msg->data[MSG_AFFDATA_LEVEL];
		pc->affect[i].keepTime = msg->data[MSG_AFFDATA_KEEPTIME];

		
		if( msg->data[MSG_AFFDATA_AFFECT_TYPE] == AFFECT_SPECIAL )
		{
			pc->affect[i].keepTime += pc->optValue[OV_POLYMORPH_KEEPTIME];
			pc->affect[i].keepTime += (int)( (float)( msg->data[MSG_AFFDATA_KEEPTIME] - g_globalTime ) * pc->optValue[OV_POLYMORPH_KEEPTIME_RATE] * 0.01f );
		}
		
		if( pc->affect[i].type == AFFECT_TOGGLE )
		{
			pc->affect[i].raCostPerSec = msg->data[MSG_AFFDATA_RACOST_PER_SEC];
			pc->affect[i].saCostPerSec = msg->data[MSG_AFFDATA_SACOST_PER_SEC];
			pc->affect[i].delayTime	   = msg->data[MSG_AFFDATA_DELAY_TIME];
		}

		pc->affect[i].skillTableIdx = msg->data[MSG_AFFDATA_SKILLTABLEIDX];
		for( j = 0; j < MAX_COUNT_AFFECT_DATA; j ++ )
		{
			pc->affect[i].data[j] = msg->data[j + CONVERT_MSG_TO_AFFECT_SIZE];
		}
		pc->affect[i].flagApply = false;
		pc->affectNumber ++;
		

	}
	else 
	{
		m = static_cast<monsterCharacter_t*>(ent);

		
		if( msg->fromType == ENTITY_PC )
		{
			hitRate += GTH_GetAffectHitRate( &g_pc[msg->fromIdx], ENTITY_PC, m, ENTITY_MONSTER, msg->data[MSG_AFFDATA_AFFECT_HITTYPE] );
		}
		else
		{
			hitRate += GTH_GetAffectHitRate( &g_monster[msg->fromIdx], ENTITY_MONSTER, m, ENTITY_MONSTER, msg->data[MSG_AFFDATA_AFFECT_HITTYPE] );
		}
		if( GTH_Random() > hitRate * 0.01f ) return;

		for( i = 0; i < MAX_AFFECT; i ++ )
		{
			if( m->affect[i].type == AFFECT_NONE ) break;

			
			if( m->affect[i].skillTableIdx == msg->data[MSG_AFFDATA_SKILLTABLEIDX] )
			{
				
				if( m->affect[i].type == AFFECT_TOGGLE )
				{
					GTH_DeleteToggle( m, m->entityType, m->affect[i].skillTableIdx );
					return;
				}
				
				if( m->affect[i].level <= msg->data[MSG_AFFDATA_LEVEL] )
				{
					m->affect[i].startTime = g_globalTime;
					m->affect[i].keepTime = msg->data[MSG_AFFDATA_KEEPTIME];
					m->affect[i].skillTableIdx = msg->data[MSG_AFFDATA_SKILLTABLEIDX];
					
					for( j = 0; j < MAX_COUNT_AFFECT_DATA; j ++ )
					{
						m->affect[i].data[j] = msg->data[j + CONVERT_MSG_TO_AFFECT_SIZE];
					}
					
					m->affect[i].flagApply = false;
					
					return;
				}
				
				else if( m->affect[i].level > msg->data[MSG_AFFDATA_LEVEL] )
				{
					return;
				}
			}
		}
		if( i == MAX_AFFECT ) return;
		
		
		m->affect[i].type = msg->data[MSG_AFFDATA_AFFECT_TYPE];
		m->affect[i].startTime = g_globalTime;
		m->affect[i].level = msg->data[MSG_AFFDATA_LEVEL];
		m->affect[i].keepTime = msg->data[MSG_AFFDATA_KEEPTIME];
		m->affect[i].skillTableIdx = msg->data[MSG_AFFDATA_SKILLTABLEIDX];
		for( j = 0; j < MAX_COUNT_AFFECT_DATA; j ++ )
		{
			m->affect[i].data[j] = msg->data[j + CONVERT_MSG_TO_AFFECT_SIZE];
		}
		m->affect[i].flagApply = false;
		m->affectNumber ++;
		

	}
}

void GTH_AffectProcess( void *ent, int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	int i = 0, n;
	int flagApply = true;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		
		n = pc->affectNumber;

		
		
		if( n > MAX_AFFECT || n < 0 )
		{
			

			
			GTH_DeleteAffectAll( pc, ENTITY_PC );

			g_logSystem->WriteToError( "Error : Affect Number, PC(%s,%s), affect number : %d", 
				pc->name, pc->userID, pc->affectNumber );

			n = 0;
			pc->affectNumber = 0;
			flagApply = false;
		}
		
		while( i < n )
		{
			if( pc->affect[i].keepTime <= g_globalTime || pc->affect[i].forcedDelete ) 
			{
				
				if( pc->affect[i].type == AFFECT_TOGGLE )
				{
					
					pc->affect[i].keepTime = g_globalTime + pc->affect[i].delayTime;
					
					
					if(pc->affect[i].delayTime <= 0)
					{
						TRACE("AFFECT_TOGGLE ... delayTime %d \n",pc->affect[i].delayTime);
						pc->affect[i].delayTime = SKILL_AFFECT_TOGGLE_DEFAULT_DELAY_TIME;
					}
					

					if( pc->curRA - pc->affect[i].raCostPerSec < 0 ) pc->affect[i].forcedDelete = true;
					if( pc->curSA - pc->affect[i].saCostPerSec < 0 ) pc->affect[i].forcedDelete = true;
					
					if( !pc->affect[i].forcedDelete )
					{
						pc->curRA -= pc->affect[i].raCostPerSec;
						pc->curSA -= pc->affect[i].saCostPerSec;
					}
					
					
					

					
					
					if( pc->curRA <= 0 ) 										
						pc->affect[i].forcedDelete = true;																
					if( pc->curSA <= 0 ) 					
						pc->affect[i].forcedDelete = true;											
					

				}
				if( pc->affect[i].forcedDelete || pc->affect[i].type != AFFECT_TOGGLE )
				{
					if( pc->affect[i].type == AFFECT_SPECIAL && pc->affect[i].spe.specialType == 0 )
					{
						pc->isTransform = false;
					}
					
					if( i < n - 1 )
					{
						memcpy( &pc->affect[i], &pc->affect[n - 1], sizeof( affect_t ) );
						memset( &pc->affect[n - 1], 0, sizeof( affect_t ) );
						pc->affect[n - 1].type = AFFECT_NONE;
					}
					else
					{
						memset( &pc->affect[i], 0, sizeof( affect_t ) );
						pc->affect[i].type = AFFECT_NONE;
					}
					n --;
					pc->affectNumber --;
					
					flagApply = false;
				}
			}
			else 
			{
				if( !pc->affect[i].flagApply )
				{
					pc->affect[i].flagApply = true;
					flagApply = false;
				}
				i ++;
			}
		}


		
		if( pc->curRA < 0 ) 										
			pc->curRA = 0.0F;				
		if( pc->curSA < 0 ) 					
			pc->curSA = 0.0F;
		

		if( !flagApply )
		{
			GTH_PC_Calculate( pc );
			GTH_SendPCMessage_Affect( pc );
		}
	}
	else
	{
		m = static_cast<monsterCharacter_t*>(ent);

		n = m->affectNumber;

		
		if( n > MAX_AFFECT || n < 0 )
		{
			
			for( int index = 0; index < MAX_AFFECT; index++ )
			{
				memset( &m->affect[i], 0, sizeof( affect_t ) );
				m->affect[i].type = AFFECT_NONE;
			}
			g_logSystem->WriteToError( "Error : Affect Number, Monster(%s,%d), affect number : %d", 
				m->basic.name, m->idx, m->affectNumber );

			n = 0;
			m->affectNumber = 0;
			flagApply = false;
		}

		while( i < n )
		{
			
			if( m->affect[i].keepTime <= g_globalTime || m->affect[i].forcedDelete ) 
			{
				
				if( m->affect[i].type == AFFECT_TOGGLE )
				{
					m->affect[i].keepTime = g_globalTime + PC_AUTO_RECOVERY_DELAY;
					if( m->curRA - m->affect[i].raCostPerSec < 0 ) m->affect[i].forcedDelete = true;
					if( m->curSA - m->affect[i].saCostPerSec < 0 ) m->affect[i].forcedDelete = true;
					
					if( !m->affect[i].forcedDelete )
					{
						m->curRA -= m->affect[i].raCostPerSec;
						m->curSA -= m->affect[i].saCostPerSec;
					}
				}
				if( m->affect[i].forcedDelete || m->affect[i].type != AFFECT_TOGGLE )
				{
					if( m->affect[i].type == AFFECT_SPECIAL && m->affect[i].spe.specialType == 0 )
					{
						
					}
					
					if( i < n - 1 )
					{
						memcpy( &m->affect[i], &m->affect[n - 1], sizeof( affect_t ) );
						memset( &m->affect[n - 1], 0, sizeof( affect_t ) );
						m->affect[n - 1].type = AFFECT_NONE;
					}
					else
					{
						memset( &m->affect[i], 0, sizeof( affect_t ) );
						m->affect[i].type = AFFECT_NONE;
					}
					n --;
					m->affectNumber --;
					
					flagApply = false;
				}
			}
			else 
			{
				if( !m->affect[i].flagApply )
				{
					m->affect[i].flagApply = true;
					flagApply = false;
				}
				i ++;
			}
		}
		if( !flagApply )
		{
			MONSTER_Calculate( m );
			GTH_SendMonsterMessage_Affect( m );
		}
	}
}

void GTH_ApplyCommonAffect( void *pEntity, AFFECTDATA_COMMON *data, int entType )
{
	playerCharacter_t*  pPlayer = NULL;
	monsterCharacter_t* pMonster = NULL;

	float *pOptValue=NULL;

	if( entType == ENTITY_PC )
	{
		pPlayer = static_cast<playerCharacter_t*>(pEntity);
		pOptValue= &(*(pPlayer->optValue));
	}
	else
	{
		pMonster = static_cast<monsterCharacter_t*>(pEntity);
		pOptValue= &(*(pMonster->optValue));
	}

	for( int i = 0; i < MAX_COUNT_AFFECT_DATA; i ++ )
	{
		switch( data->type[i] )
		{
		
		case AFF_DATA_TYPE_PHYDAMAGE_RATE_TOTAL :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calPhyMaxDmg += (int)( (float)pPlayer->calPhyMaxDmg * (float)data->data[i] * 0.01f );
					pPlayer->calPhyMinDmg += (int)( (float)pPlayer->calPhyMaxDmg * (float)data->data[i] * 0.01f );
				}
				else
				{
					pMonster->calPhyMaxDmg += (int)( (float)pMonster->calPhyMaxDmg * (float)data->data[i] * 0.01f );
					pMonster->calPhyMinDmg += (int)( (float)pMonster->calPhyMaxDmg * (float)data->data[i] * 0.01f );
				}
			}
			break;


		case AFF_DATA_TYPE_NONE :
			
			break;
		case AFF_DATA_TYPE_PHYDAMAGE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calPhyMaxDmg += data->data[i];
					pPlayer->calPhyMinDmg += data->data[i];
				}
				else
				{
					pMonster->calPhyMaxDmg += data->data[i];
					pMonster->calPhyMinDmg += data->data[i];
				}
			}
			break;

			
		case AFF_DATA_TYPE_PHYDAMAGE_RATE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calPhyMaxDmg += (int)( (float)pPlayer->basePhyMaxDmg * (float)data->data[i] * 0.01f );
					pPlayer->calPhyMinDmg += (int)( (float)pPlayer->basePhyMinDmg * (float)data->data[i] * 0.01f );
				}
				else
				{
					pMonster->calPhyMaxDmg += (int)( (float)pMonster->basic.atkPhyMaxDmg * (float)data->data[i] * 0.01f );
					pMonster->calPhyMinDmg += (int)( (float)pMonster->basic.atkPhyMinDmg * (float)data->data[i] * 0.01f );
				}
			}
			break;
		case AFF_DATA_TYPE_MAGDAMAGE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calMagMaxDmg += data->data[i];
					pPlayer->calMagMinDmg += data->data[i];
				}
				else
				{
					pMonster->calMagMaxDmg += data->data[i];
					pMonster->calMagMinDmg += data->data[i];
				}
			}
			break;
		case AFF_DATA_TYPE_MAGDAMAGE_RATE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calMagMaxDmg += (int)( (float)pPlayer->baseMagMaxDmg * (float)data->data[i] * 0.01f );
					pPlayer->calMagMinDmg += (int)( (float)pPlayer->baseMagMinDmg * (float)data->data[i] * 0.01f );
				}
				else
				{
					pMonster->calMagMaxDmg += (int)( (float)pMonster->basic.atkMagMaxDmg * (float)data->data[i] * 0.01f );
					pMonster->calMagMinDmg += (int)( (float)pMonster->basic.atkMagMinDmg * (float)data->data[i] * 0.01f );
				}
			}
			break;
		case AFF_DATA_TYPE_POSITIVE_DAMAGE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calPosMinDmg += data->data[i];
					pPlayer->calPosMaxDmg += data->data[i];
				}
				else
				{
					pMonster->calPosMaxDmg += data->data[i];
					pMonster->calPosMinDmg += data->data[i];
				}
			}
			break;
		case AFF_DATA_TYPE_NEGATIVE_DAMAGE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calNegMinDmg += data->data[i];
					pPlayer->calNegMaxDmg += data->data[i];
				}
				else
				{
					pMonster->calNegMaxDmg += data->data[i];
					pMonster->calNegMinDmg += data->data[i];
				}
			}
			break;
			
		case AFF_DATA_TYPE_ATTACK_SPEED_RATE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->attackSpeedRate += data->data[i];
					if( pPlayer->attackSpeedRate < 0 )	pPlayer->attackSpeedRate = 0;
				}
				else
				{
					pMonster->attackSpeedRate += data->data[i];
					if( pMonster->attackSpeedRate < 0 )	pMonster->attackSpeedRate = 0;
				}
			}
			break;
		case AFF_DATA_TYPE_ATTACK_SPEED :
			{
				if( entType == ENTITY_PC )
				{
					
					pPlayer->baseAttackSpeed += (int)data->data[i];
				}
			}
			break;
		case AFF_DATA_TYPE_HIT_POWER :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calHitPower += data->data[i];
				}
				else
				{
					pMonster->calHitPower += data->data[i];
				}
			}
			break;
		case AFF_DATA_TYPE_HIT_POWRE_RATE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calHitPower += (int)( (float)pPlayer->baseHitPower * (float)data->data[i] * 0.01f );
				}
				else
				{
					pMonster->calHitPower += (int)( (float)pMonster->basic.atkHitPower * (float)data->data[i] * 0.01f );
				}
			}
			break;
			
		case AFF_DATA_TYPE_DEFENSE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calDefensePower += data->data[i];
				}
				else
				{
					pMonster->calDefensePower += data->data[i];
				}
			}
			break;

		case AFF_DATA_TYPE_DEFENSE_RATE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calDefensePower += (int)( (float)pPlayer->baseItemDefensePower * (float)data->data[i] * 0.01f );
				}
				else
				{
					pMonster->calDefensePower += (int)( (float)pMonster->basic.defPower * (float)data->data[i] * 0.01f );
				}
			}
			break;

		case AFF_DATA_TYPE_AVOID:
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calAvoidPower += data->data[i];
				}
				else
				{
					pMonster->calAvoidPower += data->data[i];
				}
			}
			break;
		case AFF_DATA_TYPE_AVOID_POWER_RATE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calAvoidPower += (int)( (float)pPlayer->baseAvoidPower * (float)data->data[i] * 0.01f );
				}
				else
				{
					pMonster->calAvoidPower += (int)( (float)pMonster->basic.defAvoidPower * (float)data->data[i] * 0.01f );
				}
			}
			break;

		case AFF_DATA_TYPE_AVOID_RATE :
			{
				pOptValue[OV_AVOID_RATE] += (int)data->data[i];
			}
			break;
			

		case AFF_DATA_TYPE_MAX_RA:
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calMaxRA += (float)data->data[i];
				}
				else
				{
					pMonster->calMaxRA += (float)data->data[i];
				}
			}
			break;
		case AFF_DATA_TYPE_MAX_RA_RATE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calMaxRA += pPlayer->baseMaxRA * (float)data->data[i] * 0.01f;
				}
				else
				{
					pMonster->calMaxRA += (float)pMonster->basic.maxRA * (float)data->data[i] * 0.01f;
				}
			}
			break;
		case AFF_DATA_TYPE_MAX_SA :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calMaxSA += (float)data->data[i];
				}
				else
				{
					pMonster->calMaxSA += (float)data->data[i];
				}
			}
			break;
		case AFF_DATA_TYPE_MAX_SA_RATE :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calMaxSA += (float)pPlayer->baseMaxSA * (float)data->data[i] * 0.01f;
				}
				else
				{
					pMonster->calMaxSA += (float)pMonster->basic.maxSA * (float)data->data[i] * 0.01f;
				}
			}
			break;
		case AFF_DATA_TYPE_VELOCITY :
			{
				
				if( entType == ENTITY_PC )
				{
					pPlayer->calVelocity += (float)data->data[i] * 0.01f;
				}
				else
				{
					pMonster->calVelocity += (float)data->data[i] * 0.01f;
				}
			}
			break;

		case AFF_DATA_TYPE_VELOCITY_RATE:
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->calVelocity += (float)pPlayer->baseVelocity * (float)data->data[i] * 0.01f;
					if( pPlayer->calVelocity < 0.0f ) pPlayer->calVelocity = 0.0f;
					
					

					if( pPlayer->generalSpeed < 0 )
					{
						pPlayer->generalSpeed = 0;
						pPlayer->calVelocity = 0.0f;
					}
				}
				else
				{
					pMonster->calVelocity += (float)pMonster->basic.velocity * (float)data->data[i] * 0.01f;
					
					
					

					if( pMonster->generalSpeed < 0 )
					{
						pMonster->generalSpeed = 0;
						pMonster->calVelocity = 0.0f;
					}
				}
			}
			break;

		case AFF_DATA_TYPE_RA_RECOVERY:
			{					
				pOptValue[OV_AUTORECOVERY_RA_IDLE]	+= (float)data->data[i];
				pOptValue[OV_AUTORECOVERY_RA_ATTACK] += (float)data->data[i];
			
			}
			break;

		case AFF_DATA_TYPE_SA_RECOVERY:
			{
				pOptValue[OV_AUTORECOVERY_SA_IDLE]	+= (float)data->data[i];
				pOptValue[OV_AUTORECOVERY_SA_ATTACK] += (float)data->data[i];
			}
			break;

		
		case AFF_DATA_TYPE_DECREASE_SA_USE:
			{
				pOptValue[OV_DECSA_USE] += (float)data->data[i];
			}
			break;

		
		case AFF_DATA_TYPE_DECREASE_RA_USE:
			{
				pOptValue[OV_DECRA_USE] = (float)data->data[i];
			}
			break;

		case AFF_DATA_TYPE_DECREASE_RA:
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->curRA -= (float)data->data[i];
					if( pPlayer->curRA < 0 )	pPlayer->curRA = 0;
				}
				else
				{
					pMonster->curRA -= (float)data->data[i];
					if( pMonster->curRA < 0 )	pMonster->curRA = 0;
				}
			}
			break;

		
			
		case AFF_DATA_TYPE_DECREASE_SA:
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->curSA += (float)data->data[i];
					if( pPlayer->curSA < 0 )	pPlayer->curSA = 0;
				}
				else
				{
					pMonster->curSA += (float)data->data[i];
					if( pMonster->curSA < 0 )	pMonster->curSA = 0;
				}
			}
			break;

		case AFF_DATA_TYPE_OV_DEFENCE_RATE:
			{
				pOptValue[OV_DEFENCE_RATE] = (float)data->data[i];
			}
			break;
		case AFF_DATA_TYPE_CASTING_TIME_RATE :
			{
				pOptValue[OV_DEC_CASTINGTIME_RATE] += (float)data->data[i];
			}
			break;
		case AFF_DATA_TYPE_REFLECT_DAMAGE :
			{
				pOptValue[OV_REFLECTDMG] += (float)data->data[i];
			}
			break;
		case AFF_DATA_TYPE_REFLECT_DAMAGE_RATE :
			{
				pOptValue[OV_REFLECTDMG_RATE] += (float)data->data[i];
			}
			break;

		case AFF_DATA_TYPE_MANASHIELD :
			{
				pOptValue[OV_SKILL_MANASHIELD] = (float)data->data[i];
			}
			break;
		case AFF_DATA_TYPE_PARALYSIS :
			{
				if( entType == ENTITY_PC )
				{
					pPlayer->generalSpeed = 0;
				}
				else
				{
					pMonster->generalSpeed = 0;
				}
			}
			break;
		
		
		
		case AFF_DATA_TYPE_CRITICAL_RATE:
			{
				if ( entType == ENTITY_PC)
				{
					pPlayer->calCriticalRate += (float)data->data[i] * 0.01f;
				}
				else
				{
					pMonster->calCriticalRate += (float)data->data[i] * 0.01f;
				}
			}break;	

		case AFF_DATA_TYPE_ONCE_ATTACK_RACOST:
			{					
				pOptValue[OV_ATK_RACOST] += (float)data->data[i];
			}break;

		case AFF_DATA_TYPE_ONCE_ATTACK_SACOST:
			{
				pOptValue[OV_ATK_SACOST] += (float)data->data[i];
			}break;

		case AFF_DATA_TYPE_CONFUSION:
			break;
		case AFF_DATA_TYPE_SCANED:
			break;

		
		case AFF_DATA_TYPE_GET_EXP_GENEXP_GENCAPABILITY_RATE_INC:
			{
				pOptValue[OV_ADD_EXP_RATE]			+= (float)data->data[i];
				pOptValue[OV_ADD_GENEXP_RATE]		+= (float)data->data[i];
				pOptValue[OV_ADD_GENCAPABILITY_RATE]+= (float)data->data[i];
			}
			break;

		
		
		case AFF_DATA_TYPE_FINAL_ADD_PHY_DMG_RATE:
			{
				pOptValue[OV_SKILL_FINAL_ADD_PHY_DMG_RATE]	+= (float)data->data[i];
			}
			break;

		
		
		case AFF_DATA_TYPE_FINAL_ADD_MYSTERY_POS_DMG_RATE:
			{
				pOptValue[OV_SKILL_FINAL_ADD_MYSTERY_POS_DMG_RATE]	+= (float)data->data[i];				
			}
			break;

		
		
		case AFF_DATA_TYPE_FINAL_ADD_MYSTERY_NEG_DMG_RATE:
			{
				pOptValue[OV_SKILL_FINAL_ADD_MYSTERY_NEG_DMG_RATE]	+= (float)data->data[i];
			}
			break;

		
		
		case AFF_DATA_TYPE_DEC_COOLTIME_RATE:
			{
				pOptValue[OV_DEC_COOLTIME_RATE]	+= (float)data->data[i];
			}
			break;

		
		
		case AFF_DATA_TYPE_FINAL_ADD_DEFENCE_RATE:
			{
				pOptValue[OV_SKILL_FINAL_ADD_DEFENCE_RATE]	+= (float)data->data[i];
			}
			break;

		
		
		case AFF_DATA_TYPE_RA_RECOVERY_TIME:
			{
				pOptValue[OV_AUTORECOVERY_RA_TIME]	+= (float)data->data[i];
			}
			break;		

		
		
		case AFF_DATA_TYPE_SA_RECOVERY_TIME:
			{
				pOptValue[OV_AUTORECOVERY_SA_TIME]	+= (float)data->data[i];
			}
			break;

		
		case AFF_DATA_TYPE_CRITICAL_DMG_RATE:
			{
				pOptValue[OV_CRITICAL_DMG_RATE] += (float)data->data[i];
			}
			break;

		case AFF_DATA_TYPE_CRITICAL_DMG:
			{
				pOptValue[OV_CRITICAL_DMG] += (float)data->data[i];
			}
			break;
		}
	}
}

void GTH_ApplySpecialAffect( void *ent, AFFECTDATA_SPECIAL *spe, int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		
		
		if( spe->specialType == 0 )
		{
			
			CSkillProcAffectSpe		*pSpe = g_SkillProc.GetAffectSpe();
			if(pSpe)
			{
				bool succ = pSpe->Proc(spe->specialIdx,ent,spe);
			}	
		}
	}
	else
	{
		m = static_cast<monsterCharacter_t*>(ent);
	}
}


void GTH_ApplyAffect( void *ent, int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;
	int i;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		for( i = 0; i < pc->affectNumber; i ++ )
		{
			switch( pc->affect[i].type )
			{
			case AFFECT_ATTACK : case AFFECT_DEFENSE :
			case AFFECT_ASSIST : case AFFECT_TOGGLE : case AFFECT_CURSE :
				{
					GTH_ApplyCommonAffect( pc, &pc->affect[i].common );
				}
				break;
			case AFFECT_SPECIAL :
				{
					GTH_ApplySpecialAffect( pc, &pc->affect[i].spe );
				}
				break;
			default :
				break;
			}
		}
	}
	else
	{
		m = static_cast<monsterCharacter_t*>(ent);
		for( i = 0; i < m->affectNumber; i ++ )
		{
			switch( m->affect[i].type )
			{
			case AFFECT_ATTACK : case AFFECT_DEFENSE :
			case AFFECT_ASSIST : case AFFECT_TOGGLE : case AFFECT_CURSE :
				{
					GTH_ApplyCommonAffect( m, &m->affect[i].common, m->entityType );
				}
				break;
			case AFFECT_SPECIAL :
				{
					GTH_ApplySpecialAffect( m, &m->affect[i].spe, m->entityType );
				}
				break;
			default :
				break;
			}
		}
	}
}


int GTH_MakeAffectTableIdx( void *ent, int affectIdx[], int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;
	int i, n = 0;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		n = 0;

		for( i = 0; i < pc->affectNumber; i ++ )
		{
			if( pc->affect[i].type == AFFECT_NONE ) continue;
			affectIdx[n] = pc->affect[i].skillTableIdx;
			n ++;
		}
	}
	else
	{
		m = static_cast<monsterCharacter_t*>(ent);
		n = 0;

		for( i = 0; i < m->affectNumber; i ++ )
		{
			if( m->affect[i].type == AFFECT_NONE ) continue;
			affectIdx[n] = m->affect[i].skillTableIdx;
			n ++;
		}
	}
	return( n );
}


int GTH_MakeAffectTime( void *ent, int affectTime[], int entType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;
	int i, n = 0;

	if( entType == ENTITY_PC ) 
	{
		pc = static_cast<playerCharacter_t*>(ent);
		n = 0;

		for( i = 0; i < pc->affectNumber; i ++ )
		{
			if( pc->affect[i].type == AFFECT_NONE ) continue;
			affectTime[n] = pc->affect[i].keepTime - g_globalTime;
			n ++;
		}
	}
	else
	{
		m = static_cast<monsterCharacter_t*>(ent);
		n = 0;

		for( i = 0; i < m->affectNumber; i ++ )
		{
			if( m->affect[i].type == AFFECT_NONE ) continue;
			affectTime[n] = m->affect[i].keepTime - g_globalTime;
			n ++;
		}
	}
	return( n );
}



int GTH_GetEntityCurseState( int entType, int entIdx, int curseType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;
	int i, j;

	if( entType == ENTITY_PC ) 
	{
		pc = &g_pc[entIdx];
		if( !pc->active || !pc->ready || !pc->alive ) return 0;

		for( i = 0; i < pc->affectNumber; i ++ )
		{
			if( pc->affect[i].type == AFFECT_NONE || pc->affect[i].type == AFFECT_SPECIAL ) continue;
			for( j = 0; j < MAX_COUNT_AFFECT_DATA; j ++ )
			{
				if( pc->affect[i].common.type[j] == curseType ) return 1;
			}
		}
		return 0;
	}
	else
	{
		m = &g_monster[entIdx];
		if( !m->active || m->aliveState != MON_ALIVE_STATE_NORMAL ) return 0;

		for( i = 0; i < m->affectNumber; i ++ )
		{
			if( m->affect[i].type == AFFECT_NONE || m->affect[i].type == AFFECT_SPECIAL ) continue;
			for( j = 0; j < MAX_COUNT_AFFECT_DATA; j ++ )
			{
				if( m->affect[i].common.type[j] == curseType ) return 1;
			}
		}
	}
	return 0;
}


int GTH_RemoveEntityCurseState( int entType, int entIdx, int curseType )
{
	playerCharacter_t *pc;
	monsterCharacter_t *m;
	int i, j;

	if( entType == ENTITY_PC ) 
	{
		pc = &g_pc[entIdx];
		if( !pc->active || !pc->ready || !pc->alive ) return 0;

		for( i = 0; i < pc->affectNumber; i ++ )
		{
			if( pc->affect[i].type == AFFECT_NONE || pc->affect[i].type == AFFECT_SPECIAL ) continue;
			for( j = 0; j < MAX_COUNT_AFFECT_DATA; j ++ )
			{
				if( pc->affect[i].common.type[j] == curseType ) 
				{
					
					pc->affect[i].forcedDelete = true;
					return 1;
				}
			}
		}
		return 0;
	}
	else
	{
		m = &g_monster[entIdx];
		if( !m->active || m->aliveState != MON_ALIVE_STATE_NORMAL ) return 0;

		for( i = 0; i < m->affectNumber; i ++ )
		{
			if( m->affect[i].type == AFFECT_NONE || m->affect[i].type == AFFECT_SPECIAL ) continue;
			for( j = 0; j < MAX_COUNT_AFFECT_DATA; j ++ )
			{
				if( m->affect[i].common.type[j] == curseType ) 
				{
					
					m->affect[i].forcedDelete = true;
					return 1;
				}
			}
		}
	}
	return 0;
}
