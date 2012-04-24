#define MAX_WORLD_GATE_NUMBER	5 
#define MAX_WORLD_MONSTER_TYPE	30

typedef struct 
{
	int		idx;									
	char	name[WORLDNAMESTRING];						
	char	world2DFileName[WORLDNAMESTRING];			
	char	monsterSpawnTableFileName[WORLDNAMESTRING];	
	int		monsterSpawnNumber;						

	vec3_t	startPosition;							
	float	startYaw;							

	int		gateNumber;
	int		gateIdx[MAX_WORLD_GATE_NUMBER];
	vec3_t	spawnPos[MAX_WORLD_GATE_NUMBER];
	float	spawnYaw[MAX_WORLD_GATE_NUMBER];
	int		linkedWorldIdx[MAX_WORLD_GATE_NUMBER];
	int		linkedGateIdx[MAX_WORLD_GATE_NUMBER];

	monsterSpawnTable_t		monsterSpawnTable[MAX_NUMBER_OF_MONSTER_SPAWN_TABLE];
	int						monsterSpawnTableNumber;
	monsterSpawnZoneTable_t	monsterSpawnZoneTable[MAX_NUMBER_OF_MONSTER_SPAWN_ZONE_TABLE];
	int						monsterSpawnZoneTableNumber;

	
	int						monsterTypeNum;	
	int						monsterType[MAX_WORLD_MONSTER_TYPE];	


	
	int					specialMonsterCount; 
} worldTable_t;


int GTH_World_GetGateIdx(int sourceIdx, int targetIdx);

int GTH_LoadWorldTable();
int GTH_Load2DWorld();
void GTH_Free2DWorld();
