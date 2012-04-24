#ifndef _AI_COLLISION_MAP_H_
#define _AI_COLLISION_MAP_H_


#define	MAX_COLLISION_MAP_SIZE			600 

typedef struct
{
	float  gridSize;
	int    width;
	int    height;
	vec3_t mins;
	vec3_t maxs;
	int    offsetHeightArray;
	int    offsetFlagArray;
	int    dummy[5];    
} GTH_2DMap_Head_t;

typedef struct
{
	int				xsize;							
	int				ysize;							
	int				scale;							

	
	

	BYTE *buffer;
	float *heightMap;
	BYTE  *flagMap;

} collisionMap_t;



int AI_LoadCollisionMap( char* filename, collisionMap_t *map_t );

int AI_CheckCollision( int x, int y, collisionMap_t* map_t );
int AI_CheckCollision( vec3_t pos, collisionMap_t* map_t );

int AI_GetGridPosition( vec3_t wpos, grid_t cpos, collisionMap_t* map_t );
int AI_GetWorldPosition( grid_t cpos, vec3_t wpos, collisionMap_t* map_t );

int AI_CheckWorldCollision( monsterCharacter_t* monster, collisionMap_t* map_t );
#endif