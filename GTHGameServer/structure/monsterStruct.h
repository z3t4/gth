

enum
{
	MON_ALIVE_STATE_SPAWN		= 0,
	MON_ALIVE_STATE_NORMAL,
	MON_ALIVE_STATE_CORPSE,
	MON_ALIVE_STATE_DISAPPEAR,
	MON_ALIVE_STATE_DIE,
	MON_ALIVE_STATE_FADE,
};

enum
{
	MON_TYPE_CHE		=	0,
	MON_TYPE_MAMUL,
	MON_TYPE_YOGOI,
	MON_TYPE_YOGOISUNIN,
	MON_TYPE_YAKRYOUNG,
};




enum
{
	FIRST_ATTACK_POINT = 0,
	FIRST_ATTACK_REVERSE_POINT,
	ATTACK_POINT,
	MYSTERY_POINT,
	RECOVER_POINT,
	TOTAL_POINT,
	MAX_MONSTER_POINTS_NUM
};


#define	MONSTER_SPAWN_ANIMATION_TIME				3000
#define	MONSTER_DISAPPEAR_ANIMATION_TIME			3000
#define	MONSTER_CORPSE_ANIMATION_TIME				5000

#define MAX_NUMBER_OF_MONSTER_SPAWN_TABLE			1000
#define MAX_NUMBER_OF_MONSTER_SPAWN_ZONE_TABLE		100
#define MAX_NUMBER_OF_SPAWN_ZONE_IDX				20


typedef struct
{
	
	int				idx;
	vec3_t			position;
	vec3_t			angle;
	int				range;

} monsterSpawnZoneTable_t;

typedef struct
{
	entityType_t		entityType;			

	int					aiType;				
	int					npcAiType;			

	int					idx;
	int					monsterTableIdx;	
	int					worldIdx;			
	
	int					npcTableIdx;			

	int					spawnZoneNumber;			
	int					spawnZoneIdx[MAX_NUMBER_OF_SPAWN_ZONE_IDX];			
	int					spawnZoneProbability[MAX_NUMBER_OF_SPAWN_ZONE_IDX];

	int					positionNumber;
	vec3_t				position[MAX_NUMBER_OF_RANDOM_POSITION];
	vec3_t				angles[MAX_NUMBER_OF_RANDOM_POSITION];

	int					wayPointNumber;
	vec3_t				wayPoint[MAX_NUMBER_OF_WAYPOINT];

	int					selectedIdx;			

	int					respawnTime;			
	
	
	int					smallZoneFlag;			
	vec3_t				smallZonePosition;		

} monsterSpawnTable_t;


typedef struct
{
	
	int					flag;					
	int					entityIdx;				
	int					entityType;				
	char				entityName[NAMESTRING];	

	int					atkPoint;				
	int					defPoint;				
	int					time;					


	
	
	float				ContributionPoints[MAX_MONSTER_POINTS_NUM];	

} monsterContribution_t;

#define MAX_NUMBER_OF_MONSTER_CONTRIBUTION		50

#define MAX_NUMBER_OF_JOINT_PC					10			

#define MAX_NUMBER_OF_MONSTER_TABLE				500			

#define MAX_NUMBER_OF_MONSTER_SKILL				5			


typedef struct
{
	
	enum ENUM_MONSTER_INFO
	{
		MAX_SUMMON_OPTION_VALUE=6,
	};

	
	int					idx;					
	int					active; 				
	int					aliveState;					

	float				recoveryTime;			

	i3characterEvent_t	event;					
	i3characterEvent_t	oldEvent;				

	monsterBasicTable_t	basic;
	int					tableIdx;			

	int					spawnWorldIdx;			

	int					spawnTableIdx;			







	int					wayPointNumber;
	vec3_t				wayPoint[MAX_NUMBER_OF_WAYPOINT];

	int					respawnTime;			
	int					deathTime;				

	
	BOOL				bIgnoreRespawnTime;		
	


	entityType_t		entityType; 			

	int					worldIdx;				
	int					zoneIdx;				
	vec3_t				position;				

	
	vec3_t				Spawnposition;			

	vec3_t				angles;					

	
	int					generalSpeed;			

	float				nextAngle;
	
	
	ai_t				ai;						


	
	message_t			message[MAX_NUMBER_OF_MESSAGE];
	int					messageNumber;

	

	float				curRA;
	float				curSA;
	float				calMaxRA;				
	float				calMaxSA;				
	

	int					recoveryRARate;		
	int					recoverySARate;		

	
	int					calPhyMinDmg;
	int					calPhyMaxDmg;
	int					calMagMinDmg;
	int					calMagMaxDmg;

	int					calPosMinDmg;
	int					calPosMaxDmg;
	int					calNegMinDmg;
	int					calNegMaxDmg;

	int					calHitPower;
	int					calAttackDelay;	
	int					attackSpeedRate;

	
	int					calAttackLength;	
	int					calAttackRange;		
	

	float				calCriticalRate;	
	int					calCriticalDmg;		

	int					calHitRate;				

	
	int					calDefensePower;		
	int					calPositiveRes;
	int					calNegativeRes;
	float				calVelocity;			
	int					calStunTime;			
	int					calAvoidPower;			

	
	affect_t			affect[MAX_AFFECT];
	int					affectNumber;

	




	
	
	monsterContribution_t		conInfo[MAX_NUMBER_OF_MONSTER_CONTRIBUTION];
	int							maxAtkContributionPoint;
	int							conInfoNumber;

	int							mostConEntityType;
	int							mostConEntityIdx;
	char						mostConEntityName[NAMESTRING];

	
	int					targetType;				
	int					targetIdx;				

	
	int					damagedEntityType;
	int					damagedEntityIdx;

	
	int					searchEntityType;
	int					searchEntityIdx;

	
	int					forcedEntityType;
	int					forcedEntityIdx;

	
	int					bossEntityType;
	int					bossEntityIdx;

	
	int					npcTableIdx;
	int					npcAIFlag;
	npcAi_t				npcAi;
	npcStruct_t			npc;

	int					killerIdx;
	int					killerType;

	int					curDamage;
	int					lastAttackState;

	
	int					isSummon;
	float				summonTime;
	float				summonValidTime;

	
	float				autoRecoveryRATime;
	float				autoRecoverySATime;

	
	int					specialMonster;

	
	
	
	int					skillNumber;				
	skill_t				skill[MAX_NUMBER_OF_MONSTER_SKILL];	
	float				skillCooltime[MAX_NUMBER_OF_MONSTER_SKILL];		
	float				skillCastingtime;
	int					skillCastingState;
	int					saveSkillIdx;
	vec3_t				saveSkillTargetPos;			
	int					bonusSkillPoint;
	int					skillVariable[MAX_NUMBER_OF_SKILL_VARIABLE];

	
	
	int					selectedSkillIdx;
	int					skillUsePercent[MAX_NUMBER_OF_MONSTER_SKILL]; 


	
	

	int					totalOptNum;
	int					playerOptNum;

	
	
	int					SummonOptIndex[MAX_SUMMON_OPTION_VALUE];
	float				SummonOptValue[MAX_SUMMON_OPTION_VALUE];		
	

	
	BOOL     			bUsedSkill[10]; 

	
	float				fSpawnElapsedTime;	
	float				fAiTempValue[5];
	float				fSkillAnimationTime[MAX_NUMBER_OF_MONSTER_SKILL];
	float				fSkillAnimationPassTime;
	
	
	float				optValue[NUM_OPTION_VALUE];

} monsterCharacter_t;


#define SAFE_MONSTER_SKILL_INDEX(p) ( p < 0 || p >= MAX_NUMBER_OF_MONSTER_SKILL) ? 0 : 1



typedef	std::vector<monsterCharacter_t *>			LIST_NONPLAYER_STRUCT;

