#include "../global.h"


#define					NUMBER_OF_PATH_SEARCH		10		
#define					MAX_NUMBER_OF_PATH_SEARCH	2000	

void AI_PreparePathfinding( monsterCharacter_t *monster, int pathfindingSize )
{
	
	if( pathfindingSize < 50 || pathfindingSize > 300 ) pathfindingSize = 50;

	monster->ai.pathFindSize = pathfindingSize;

	int size = monster->ai.pathFindSize;
	
	monster->ai.nodeHashTable = (node_t **)malloc( size * size * sizeof( node_t ) );
	
	monster->ai.path = (grid_t *)malloc( size * size * sizeof( grid_t ) );
	memset(monster->ai.path,0x00,size * size * sizeof( grid_t ));


	monster->ai.wayPath = (grid_t *)malloc( size * size * sizeof( grid_t ) );
	
	monster->ai.nodeBank = (node_t *)malloc( size * size * sizeof( node_t ) );
}

void AI_ReleasePathfinding( monsterCharacter_t *monster )
{
	free( monster->ai.nodeHashTable );
	free( monster->ai.path );
	free( monster->ai.wayPath );
	free( monster->ai.nodeBank );
	
	free(monster->ai.aiData);
}


void AI_InitNodeBank( monsterCharacter_t *monster )
{
	int size = monster->ai.pathFindSize;

	
	monster->ai.nodeBankNumber = 0;
	memset( monster->ai.nodeHashTable, NULL, size * size * sizeof( node_t ) );

	
	monster->ai.heruristic = (float)5.0f;
	monster->ai.isGoal = true;
}


void AI_DeleteNodeBank( monsterCharacter_t* monster )
{
	int size = monster->ai.pathFindSize;

	
	memset( monster->ai.nodeHashTable, NULL, size * size * sizeof( node_t ) );
	monster->ai.nodeBankNumber = 0;
}


node_t *AI_GetFreeNodeFromNodeBank( monsterCharacter_t *monster )
{
	node_t *rNode;
	rNode = monster->ai.nodeBank;
	rNode = rNode + monster->ai.nodeBankNumber ++;
	return( rNode );
}


node_t *AI_GetNode( monsterCharacter_t* monster, grid_t pos )
{
	node_t *node;
	grid_t cpos;
	int size = monster->ai.pathFindSize;

	
	cpos[0] = pos[0] - monster->ai.curPos[0];
	cpos[1] = pos[1] - monster->ai.curPos[1];
	
	
	if( abs( cpos[0] ) >= size / 2 || abs( cpos[1] ) >= size / 2 ) return( NULL );

	cpos[0] += size / 2;
	cpos[1] += size / 2;

	node = monster->ai.nodeHashTable[ cpos[1] * size + cpos[0] ];
	if( node ) return( node );
	else
	{
		node_t *newNode = AI_GetFreeNodeFromNodeBank( monster );
		newNode->pos[0] = pos[0];
		newNode->pos[1] = pos[1];
		newNode->onOpen = false;
		newNode->onClosed = false;

		
		monster->ai.nodeHashTable[ cpos[1] * size + cpos[0] ] = newNode;
		return( newNode );
	}
}


node_t *AI_PopPriorityQueue( queue_t &pqueue )
{
	node_t *node = pqueue.heap.front();

	std::pop_heap( pqueue.heap.begin(), pqueue.heap.end(), AI_NodeTotalGreater() );

	pqueue.heap.pop_back();

	return( node );
}


void AI_PushPriorityQueue( queue_t &pqueue, node_t *node)
{
	pqueue.heap.push_back( node );
	std::push_heap( pqueue.heap.begin(), pqueue.heap.end(), AI_NodeTotalGreater() );
}


void AI_UpdateNodeOnPriorityQueue( queue_t &pqueue, node_t *node )
{
	std::vector<node_t*>::iterator i;

	for( i = pqueue.heap.begin(); i != pqueue.heap.end(); i ++ )
	{
		if( (*i)->pos[0] == node->pos[0] && (*i)->pos[1] == node->pos[1] )
		{
			std::push_heap( pqueue.heap.begin(), i + 1, AI_NodeTotalGreater() );
			return;
		}
	}
}


bool AI_IsPriorityQueueEmpty( queue_t &pqueue )
{
	return( pqueue.heap.empty() );
}


int AI_CanMove( monsterCharacter_t* monster, grid_t pos, int lines )
{	
	int localWorldIdx;

	if( pos[1] < 0 || pos[0] < 0 ) return( 0 );

	localWorldIdx = GTH_GetLocalWorldIdx( monster->worldIdx );
	if( !AI_CheckCollision( pos[0], pos[1], &g_cmap[localWorldIdx] ) ) return( 0 );

	
	switch( lines )
	{
	case 1 :
		if( AI_CheckCollision( pos[0], pos[1] - 1, &g_cmap[localWorldIdx] ) &&
			AI_CheckCollision( pos[0] + 1, pos[1], &g_cmap[localWorldIdx] ) )
			return( 1 );
		break;
	case 3 :
		if( AI_CheckCollision( pos[0], pos[1] - 1, &g_cmap[localWorldIdx] ) &&
			AI_CheckCollision( pos[0] - 1, pos[1], &g_cmap[localWorldIdx] ) )
			return( 1 );
		break;
	case 5 :
		if( AI_CheckCollision( pos[0], pos[1] + 1, &g_cmap[localWorldIdx] ) &&
			AI_CheckCollision( pos[0] - 1, pos[1], &g_cmap[localWorldIdx] ) )
			return( 1 );
		break;
	case 7 :
		if( AI_CheckCollision( pos[0], pos[1] + 1, &g_cmap[localWorldIdx] ) &&
			AI_CheckCollision( pos[0] + 1, pos[1], &g_cmap[localWorldIdx] ) )
			return( 1 );
		break;
	default :
		return( 1 );
		break;
	}
	return( 0 );
}


void AI_DeleteQueue( path_t &path )
{
	while( !AI_IsPriorityQueueEmpty( path.open ) )
	{
		path.open.heap.pop_back();
	}
}


float AI_CostFromNodeToNode( grid_t pos1, grid_t pos2 )
{
	return( sqrtf( ( pos1[0] - pos2[0] ) * (pos1[0] - pos2[0] ) + ( pos1[1] - pos2[1] ) * ( pos1[1] - pos2[1] ) ) );
}


bool AI_FindPath( monsterCharacter_t* monster )
{
	node_t *startNode;							
	node_t *bestNode;							
	node_t newNode;								
	node_t *actualNode;							

	int i;										
	
	monster->ai.searchNumber = 0;



	if( monster->ai.curPos[0] == monster->ai.goalPos[0] && 
		monster->ai.curPos[1] == monster->ai.goalPos[1] ) return( 1 );

	
	if( monster->ai.goalPos[0] == monster->ai.path[0][0] &&
		 monster->ai.goalPos[1] == monster->ai.path[0][1] ) return( 1 );

	
	if( monster->ai.isGoal )
	{
		
		if( monster->ai.totalSearch == 0 )
		{
			startNode = AI_GetNode( monster, monster->ai.curPos );
			
			startNode->onClosed = false;
			startNode->onOpen = true;
			startNode->parent = NULL;
			startNode->cost = 0;
			
			startNode->total = AI_CostFromNodeToNode( monster->ai.curPos, monster->ai.goalPos )  * monster->ai.heruristic;
			
			monster->ai.pathQueue.goal[0] = monster->ai.goalPos[0];
			monster->ai.pathQueue.goal[1] = monster->ai.goalPos[1];
			
			
			AI_PushPriorityQueue( monster->ai.pathQueue.open, startNode );
			
			
			monster->ai.totalSearch = 0;
		}
	}

	while( !AI_IsPriorityQueueEmpty( monster->ai.pathQueue.open ) )
	{
		
		if( monster->ai.searchNumber > NUMBER_OF_PATH_SEARCH )
		{
			monster->ai.totalSearch += monster->ai.searchNumber;
			if( monster->ai.totalSearch > MAX_NUMBER_OF_PATH_SEARCH ) 
				return( false );
			
			monster->ai.isGoal = false;
			return( true );
		}

		
		bestNode = AI_PopPriorityQueue( monster->ai.pathQueue.open );

		
		if( bestNode->pos[0] == monster->ai.pathQueue.goal[0] && 
			bestNode->pos[1] == monster->ai.pathQueue.goal[1] )
		{
			AI_PushPriorityQueue( monster->ai.pathQueue.open, bestNode );
			
			AI_MakePath( monster );
			
			monster->ai.isGoal = true;
			return( true );
		}

		
		for( i = 0; i < 8; i ++ )
		{
			switch( i )
			{
			case 0:
				newNode.pos[0] = bestNode->pos[0] - 1;
				newNode.pos[1] = bestNode->pos[1];
				break;
			case 1:
				newNode.pos[0] = bestNode->pos[0] - 1;
				newNode.pos[1] = bestNode->pos[1] + 1;
				break;
			case 2:
				newNode.pos[0] = bestNode->pos[0];
				newNode.pos[1] = bestNode->pos[1] + 1;
				break;
			case 3:
				newNode.pos[0] = bestNode->pos[0] + 1;
				newNode.pos[1] = bestNode->pos[1] + 1;
				break;
			case 4:
				newNode.pos[0] = bestNode->pos[0] + 1;
				newNode.pos[1] = bestNode->pos[1];
				break;
			case 5:
				newNode.pos[0] = bestNode->pos[0] + 1;
				newNode.pos[1] = bestNode->pos[1] - 1;
				break;
			case 6:
				newNode.pos[0] = bestNode->pos[0];
				newNode.pos[1] = bestNode->pos[1] - 1;
				break;
			case 7:
				newNode.pos[0] = bestNode->pos[0] - 1;
				newNode.pos[1] = bestNode->pos[1] - 1;
				break;
			}
			
			if( !AI_CanMove( monster, newNode.pos, i ) ) continue;

			if( bestNode->parent == NULL || \
				bestNode->parent->pos[0] != newNode.pos[0] || \
				bestNode->parent->pos[1] != newNode.pos[1] )
			{
				newNode.parent = bestNode;
				newNode.cost = bestNode->cost + AI_CostFromNodeToNode( newNode.pos, bestNode->pos );
				newNode.total = newNode.cost + AI_CostFromNodeToNode( newNode.pos, monster->ai.pathQueue.goal ) * monster->ai.heruristic;

				actualNode = AI_GetNode( monster, newNode.pos );
				if( actualNode == NULL ) return( false );

				if( !( actualNode->onOpen && newNode.total > actualNode->total ) && \
					!( actualNode->onClosed && newNode.total > actualNode->total ) )
				{
					actualNode->onClosed = false;
					actualNode->parent = newNode.parent;
					actualNode->cost = newNode.cost;
					actualNode->total = newNode.total;

					if( actualNode->onOpen )
					{
						AI_UpdateNodeOnPriorityQueue( monster->ai.pathQueue.open, actualNode );
					}
					else
					{
						AI_PushPriorityQueue( monster->ai.pathQueue.open, actualNode );
						actualNode->onOpen = true;
					}
					monster->ai.searchNumber ++;
				}
			}
		}
		bestNode->onClosed = true;
	}
	return( false );
}


void AI_StopPathFinding( monsterCharacter_t* monster )
{
	AI_DeleteNodeBank( monster );
	AI_DeleteQueue( monster->ai.pathQueue );
	monster->ai.isGoal = true;
	monster->ai.totalSearch = 0;


	int size=monster->ai.pathFindSize;
	memset(monster->ai.path,0x00,size * size * sizeof( grid_t ));
}

void AI_MakePath( monsterCharacter_t* monster )
{
	node_t *node;						
	int i = 0;							

	
	if( monster->ai.curPos[0] == monster->ai.goalPos[0] &&
		monster->ai.curPos[1] == monster->ai.goalPos[1] )
	{
		monster->ai.pathNumber = 0;
	}
	else
	{
		node = AI_PopPriorityQueue( monster->ai.pathQueue.open );
		while( 1 )
		{
			if( !node ) break;
			
			if( i >= monster->ai.pathFindSize * monster->ai.pathFindSize ) break;
			monster->ai.path[i][0] = node->pos[0];
			monster->ai.path[i][1] = node->pos[1];
			i ++;
			node = node->parent;
		}
		monster->ai.pathNumber = i - 1;
	}
	AI_DeleteNodeBank( monster );
	AI_DeleteQueue( monster->ai.pathQueue );
	monster->ai.totalSearch = 0;
}
