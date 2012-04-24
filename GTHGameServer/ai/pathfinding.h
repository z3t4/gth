#ifndef _AI_PATHFINDING_H_
#define _AI_PATHFINDING_H_


class AI_NodeTotalGreater
{
public:
	bool operator()( node_t *first, node_t *second ) const
	{
		return( first->total > second->total );
	}
};

void AI_PreparePathfinding( monsterCharacter_t *monster, int pathfindingSize = 30 );
void AI_ReleasePathfinding( monsterCharacter_t *monster );

void AI_LoadCollisionMap( char* filename );
void AI_InitNodeBank( monsterCharacter_t* monster );
void AI_DeleteNodeBank( monsterCharacter_t* monster );
bool AI_FindPath( monsterCharacter_t* monster );
void AI_MakePath( monsterCharacter_t* monster );
void AI_StopPathFinding( monsterCharacter_t* monster );

#endif