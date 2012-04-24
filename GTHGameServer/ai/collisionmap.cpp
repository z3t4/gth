#include "../global.h"

int AI_LoadCollisionMap( char *fname, collisionMap_t *mapt )
{
	FILE *fp;
	int size;
	GTH_2DMap_Head_t *head;
	
	fp = fopen( fname , "rb" );
	if ( !fp ) return 0;

	fseek( fp , 0 , SEEK_END );
	size = ftell( fp );

	fseek( fp , 0 , SEEK_SET );
	mapt->buffer = new BYTE[ size ];
	fread( mapt->buffer , size , 1 , fp );
	fclose( fp );

	head = ( GTH_2DMap_Head_t * ) mapt->buffer;

	mapt->scale = head->gridSize;
	mapt->xsize = head->width;
	mapt->ysize = head->height;
	mapt->heightMap = (float*) ( mapt->buffer + head->offsetHeightArray );
	mapt->flagMap = (BYTE*) ( mapt->buffer + head->offsetFlagArray );






















	return 1;
}


int AI_CheckCollision( int x, int y, collisionMap_t* mapt )
{
	
	if( x < 0 || x >= mapt->xsize ) return( 0 );
	if( y < 0 || y >= mapt->ysize ) return( 0 );

	
	if( mapt->flagMap[ y * mapt->xsize + x ] == 1 ) return( 1 );
	else return( 0 );
}

int AI_CheckCollision( vec3_t pos, collisionMap_t* mapt )
{
	grid_t gridPos;
	AI_GetGridPosition( pos, gridPos, mapt );
	return( AI_CheckCollision( gridPos[0], gridPos[1], mapt ) );
}


int AI_GetGridPosition( vec3_t wpos, grid_t cpos, collisionMap_t* mapt )
{
	cpos[0] = (int)(wpos[0] / (float)mapt->scale);
	cpos[1] = (int)(wpos[1] / (float)mapt->scale);

	
	if( cpos[0] < 0 || cpos[0] >= mapt->xsize ) return( 0 );
	if( cpos[1] < 0 || cpos[1] >= mapt->ysize ) return( 0 );

	return( 1 );
}

int AI_GetWorldPosition( grid_t cpos, vec3_t wpos, collisionMap_t* mapt )
{
	wpos[0] = (float)cpos[0] * (float)mapt->scale;
	wpos[1] = (float)cpos[1] * (float)mapt->scale;

	
	if( cpos[0] < 0 || cpos[0] >= mapt->xsize ) return( 0 );
	if( cpos[1] < 0 || cpos[1] >= mapt->ysize ) return( 0 );

	return( 1 );
}

