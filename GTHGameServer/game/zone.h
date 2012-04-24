
#define MAX_LOCALSERVICEWORLDS	10	


#define MAX_ZONE_PCS				200		
#define MAX_ZONE_MONSTERS			100		
#define MAX_ZONE_NPCS				10		
#define MAX_ZONE_OBJECTS			100		

#define MAX_SCAN_PCLIST				100 
#define MAX_SCAN_MONSTERLIST		200 
#define MAX_SCAN_NPCLIST			20  
#define MAX_SCAN_OBJECTLIST			200 
#define MAX_SCAN_ENTITYLIST			200 


#define MAX_LIMIT_PCLIST			50 
#define MAX_LIMIT_MONSTERLIST		50 
#define MAX_LIMIT_NPCLIST			20 
#define MAX_LIMIT_OBJECTLIST		100 
#define MAX_LIMIT_ENTITYLIST		50	










#define ZONE_GRIDSCALE				50	
#define ZONE_UNIT					20	
#define ZONE_DIV					34	

#define ZONE_SCANDEPTH			     2	
#define MAX_SEEINGDISTANCE			ZONE_UNIT * ZONE_SCANDEPTH * ZONE_GRIDSCALE 

typedef struct
{
	entityType_t	entityType;
	int				idx;
	float			distance;
} i3zoneScanList_t;


typedef struct {
	int localWorldIdx;
	int zoneIdx;
	int pcNumber;
	int pcList[MAX_ZONE_PCS];
	int monNumber;
	int monList[MAX_ZONE_MONSTERS];
	int npcNumber;
	int npcList[MAX_ZONE_NPCS];
	int objectNumber;
	int objectList[MAX_ZONE_OBJECTS];
} i3zone_t;


	

void GTH_Zone_Init();

int GTH_Zone_GetZoneIdx(vec3_t position);
int GTH_Zone_UpdateCurrentZone(entityType_t entType, int entIdx, int worldIdx, int preZoneIdx, vec3_t position);
int GTH_Zone_AddEntityList(int worldIdx, int zoneIdx, entityType_t entType, int entIdx);
int GTH_Zone_DeleteEntityList(int worldIdx, int zoneIdx, entityType_t entType, int entIdx);

int GTH_Zone_ScanPCList(int localWorldIdx, vec3_t position, int scanDepth);
int GTH_Zone_ScanMonsterList(int localWorldIdx, vec3_t position, int scanDepth);
int GTH_Zone_ScanNPCList(int localWorldIdx, vec3_t position, int scanDepth);
int GTH_Zone_ScanObjectList(int localWorldIdx, vec3_t position, int scanDepth);
int GTH_Zone_ScanEntityList(int localWorldIdx, vec3_t position, int scanDepth);

int GTH_CompareForQsort( const void *arg1, const void *arg2 );
int GTH_PCCompareForQsort( const void *arg1, const void *arg2 );


void GTH_GetEntityListInValidRange(int worldIdx, vec3_t pos, int zoneSize = 2 );
void GTH_GetPCListInValidRange(int worldIdx, vec3_t pos, int zoneSize = 2 );
void GTH_GetObjectListInValidRange(int worldIdx, vec3_t pos, int zoneSize = 1 );
void GTH_GetMonsterListInValidRange(int worldIdx, vec3_t pos, int zoneSize = 2 );
void GTH_GetNPCListInValidRange(int worldIdx, vec3_t pos, int zoneSize = 2 );
