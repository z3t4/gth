
int AI_SetTimer( monsterCharacter_t* monster, int thinkTime );

int AI_SetAIEvent( monsterCharacter_t* monster, int event );

int AI_HaveDamaged( monsterCharacter_t* monster );

int AI_SetTarget( monsterCharacter_t* monster, int type );

int AI_MoveToAnywhere( monsterCharacter_t* monster, int range );

int	AI_IsActiveTarget( monsterCharacter_t* monster );

int	AI_RemoveTarget( monsterCharacter_t* monster );
int	AI_IsWithinAttackLength( monsterCharacter_t* monster );
int	AI_CanTraceTarget( monsterCharacter_t* monster, int traceRange );

int	AI_MoveToTarget( monsterCharacter_t* monster );
int AI_SearchTarget( monsterCharacter_t* monster, int entity, int range );
int	AI_MoveToWayPoint( monsterCharacter_t* monster );

int AI_FleeToTarget( monsterCharacter_t* monster, int flag );
int AI_SearchFellow( monsterCharacter_t* monster, int range );
int AI_CheckAbilityRate( monsterCharacter_t* monster, int flag );
int AI_GetDistanceFromTarget( monsterCharacter_t* monster );
int AI_IsWithInWayPoint( monsterCharacter_t* monster );
int AI_RunFromTarget( monsterCharacter_t* monster );
int AI_SetMoveType( monsterCharacter_t *monster, int type );


int AI_IsNearTarget( monsterCharacter_t* monster, int entity );

int AI_ReceiveHelpMessage( monsterCharacter_t* monster );
int AI_HaveBoss( monsterCharacter_t* monster, int monTableIdx );
int AI_MoveToBoss( monsterCharacter_t* monster );
int AI_SendHelpMessage( monsterCharacter_t* monster, int monTableIdx, int range );



int AI_IsPartyPlayer( monsterCharacter_t *monster );
int AI_IsAttackerLevelHigherThanMe( monsterCharacter_t *monster );
int AI_GetAttackerCount( monsterCharacter_t *monster );
int AI_IsAtkRngLongerThanValue( monsterCharacter_t *monster, int atkRange );
int AI_IsAtkRngShorterThanValue( monsterCharacter_t *monster, int atkRange );
int AI_RemoveSkillUsePercent( monsterCharacter_t* monster );
int AI_AddSkillUsePercent( monsterCharacter_t* monster, int skillid, int percent );
int AI_UseSkill( monsterCharacter_t* monster );
int AI_HaveSelectedSkill( monsterCharacter_t* monster );



int AI_MonsterSummon ( monsterCharacter_t* monster, int index, int persent);
int AI_IsHaveTargetTrace( monsterCharacter_t* monster );
int AI_MoveToSpawnPos( monsterCharacter_t* monster );
int AI_IsOverRangeFromSpawnPos( monsterCharacter_t* monster, int range );
int AI_MonsterReSpawn ( monsterCharacter_t* monster );
int AI_GetDamage ( monsterCharacter_t* monster);


int	AI_IsUsedSkill ( monsterCharacter_t* monster, int idx );		
int AI_SetUsedSkill ( monsterCharacter_t* monster, int idx , int active);

int AI_IsOverDIstansAttackRangeRate (monsterCharacter_t* monster, int rate );

int AI_IsHighVelocityThenMonster (monsterCharacter_t* monster );


int AI_GetSkillElapsedTime (monsterCharacter_t* monster, int idx );


int AI_SendHelpMsg(monsterCharacter_t* monster, int monTableIdx, int range, int count);


int AI_GetAIEvent(monsterCharacter_t *monster );


int AI_GetSummonElapsedTime (monsterCharacter_t* monster );



int AI_GetSpawnElapsedTime(monsterCharacter_t* monster );
int AI_GetTimeSkill (monsterCharacter_t* monster,int start_time,int roop_time,int roop_total);


int AI_SetSkillAnimationTime ( monsterCharacter_t* monster, int idx, int time );



int AI_GetRandom(monsterCharacter_t *monster );


int AI_ProcessOverSpawnPos(monsterCharacter_t *monster);



#ifndef _QUEST_DEFINE_
#define _QUEST_DEFINE_

struct AIFUNC
{
	int (*pFunc)(void* pc,int* pParam);
	int	argCount;	
};

#endif

