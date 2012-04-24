



#include "../global_def.h"






CItemMng::CItemMng()
{
	m_currEntity = 0;
	m_frameFlag   = 0;
	m_visibleDist = 10000.0f;
	m_linked = NULL;
}

CItemMng::~CItemMng()
{
}

void CItemMng::Initialize( float visibleDist )
{
	m_frameFlag = 0;
	m_currEntity = 0;
	m_visibleDist = visibleDist;
	m_MDLMng->ClearItemUseFlag();

	m_linked = NULL;
	memset( m_entities , 0 , sizeof( itementity_t ) * MAX_SIZE_ITEM_ENTITY );
	memset( m_entityInUse , 0 , sizeof( int ) * MAX_SIZE_ITEM_ENTITY );

	for( int i = 0 ; i < MAX_SIZE_ITEM_ENTITY ; i++ )
	{
		m_entities[i].idx = i;
		m_entities[i].idxOnServer = -1;
	}
}

void CItemMng::ClearUseFlag()
{
	memset( m_entityInUse , 0 , sizeof( int ) * MAX_SIZE_ITEM_ENTITY );
	m_currEntity = 0;
	m_linked = NULL;
}

int CItemMng::FindEntity ( int idxOnServer )
{
	itementity_t *entity;

	entity = m_linked;

	while ( entity )
	{
		if ( entity->idxOnServer == idxOnServer )
			return entity->idx;

		
		entity = entity->next;
	}

	return -1;
}

void CItemMng::SyncItemObject( syncItemList_t *syncItem, int numSyncItem )
{
	itementity_t *entity, *nextEntity;
	int i, numValid;

	ClearUseFlag();
	m_MDLMng->ClearItemUseFlag();

	numValid = 0;
	nextEntity = NULL;
	entity = &m_entities[0];
	for( i = 0 ; i < numSyncItem ; i++, syncItem++)
	{
		 



		entity->prev = NULL;
		entity->next = nextEntity;

		 


		entity->classType = syncItem->classType;
		entity->itemID = syncItem->itemID;
		VectorCopy( entity->origin , syncItem->position );
		VectorClear( entity->angles );
		entity->idxOnServer = syncItem->idxOnServer;
		
		entity->itemTableIdx = syncItem->itemTableIdx;

		entity->idxMDLEntity = m_MDLMng->AddItemEntity( entity->classType , entity->itemID , entity->origin, entity->angles );
		if( entity->idxMDLEntity >= 0 )
		{
			if( !m_MDLMng->GetMDLItemClassBoundBox( entity->idxMDLEntity , entity->mins , entity->maxs ) )
			{
				VectorClear( entity->mins );
				VectorClear( entity->maxs );
			}
		}
		else
			continue;

		m_entityInUse[ numValid ] = true;

		if ( nextEntity ) nextEntity->prev = entity;
		nextEntity = entity;

		entity++ ;
		numValid++;
	}

	m_currEntity = numValid; 
	if ( m_currEntity > 0 )
		m_linked = &m_entities[ m_currEntity - 1 ];
	else
		m_linked = NULL;
}

int CItemMng::Add( GTH_ITEM_CLASS_TYPE classType , int itemID , vec3_t origin , int idxOnServer, int itemTableIdx )
{
	itementity_t *entity;
	int i , found;

	if( classType == GTH_ITEM_CLASS_NONE ) return -1;

	found = -1;
	for( i = m_currEntity ; i < MAX_SIZE_ITEM_ENTITY ; i++ )
	{
		if( !m_entityInUse[i] )
		{
			found = i;
			goto NEXT_FREE_ITEM_ENTITY;
		}
	}

	for( i = 0 ; i < m_currEntity ; i++ )
	{
		if( !m_entityInUse[i] )
		{
			found = i;
			goto NEXT_FREE_ITEM_ENTITY;
		}
	}

	if( found == -1 )
	{
		found = m_currEntity + 1;
		if( found >= MAX_SIZE_ITEM_ENTITY )
			found = 0;
		Delete( found );
	}

NEXT_FREE_ITEM_ENTITY :
	m_currEntity = found;

	entity = &m_entities[ m_currEntity ];

	entity->next = m_linked;
	entity->prev = NULL;
	if( m_linked )
		m_linked->prev = entity;
	m_linked = entity;

	entity->classType = classType;
	entity->itemID = itemID;
	VectorCopy( entity->origin , origin );
	VectorClear( entity->angles );
	entity->idxOnServer = idxOnServer;
	
	entity->itemTableIdx = itemTableIdx;

	 







	entity->idxMDLEntity = m_MDLMng->AddItemEntity( entity->classType , entity->itemID , entity->origin 
		                                          , entity->angles ); 

	if( entity->idxMDLEntity >= 0 )
	{
		if( !m_MDLMng->GetMDLItemClassBoundBox( entity->idxMDLEntity , entity->mins , entity->maxs ) )
		{
			VectorClear( entity->mins );
			VectorClear( entity->maxs );
		}
	}
	else
	{
		m_entityInUse[ m_currEntity ] = false;
		return -1;
	}

	m_entityInUse[ m_currEntity ] = true;
	
	return m_currEntity;
}

int CItemMng::Delete( int idxEntity )
{
	itementity_t *entity;
	
	entity = &m_entities[ idxEntity ];
	if( !m_entityInUse[ idxEntity ] ) return true;

	if( entity->prev )
	{
		entity->prev->next = entity->next;
	}
	else  
	{
		m_linked = entity->next;
	}

	if( entity->next )
		entity->next->prev = entity->prev;

	entity->next = NULL;
	entity->prev = NULL;
	m_entityInUse[ idxEntity ] = false;

	if( entity->idxMDLEntity >= 0 )
		m_MDLMng->DeleteItemEntity( entity->idxMDLEntity );

	return true;
}

inline int CItemMng::ChechVisibleDistance( vec3_t origin )
{
	vec3_t dist;

	VectorSubtract( dist , origin , m_camPos );

	if( VectorDotProduct( dist , dist ) > m_visibleDist * m_visibleDist )
		return false;

	return true;
}

void CItemMng::Process( vec3_t pos , DWORD frameFlag )
{
	m_frameFlag = frameFlag;

	VectorCopy( m_camPos , pos );
	m_visData = m_world->GetVisData();
	if( !m_visData ) return;

	ProcessEntityFrame();
}

inline int CItemMng::Cull( vec3_t mins , vec3_t maxs , vec3_t position , int linkedCluster )
{
	vec3_t trans_mins , trans_maxs;

	if( !ChechVisibleDistance( position ) )
		return true;

	 



	VectorAdd( trans_mins , position , mins );
	VectorAdd( trans_maxs , position , maxs );

	

	if( m_frustum->Cull_Exact( trans_mins , trans_maxs ) )
		return true;

	return false;
}

void CItemMng::ProcessEntityFrame()
{
	itementity_t *entity;
	int *linkedCount;

	m_MDLMng->ClearItemLinkedCount();
	linkedCount = m_MDLMng->GetItemVisibleLinkedCount();
	m_MDLMng->m_focusedItem = -1;

	entity = m_linked;
	while( entity )
	{
		if( Cull( entity->mins , entity->maxs , entity->origin , entity->linkedCluster ) )
		{
			m_MDLMng->m_MDLItemEntities[ entity->idxMDLEntity ].visible = false;
			entity->visible = false;
		}
		else
		{
			linkedCount[ m_MDLMng->m_MDLItemEntities[ entity->idxMDLEntity ].idxClass ]++;
			m_MDLMng->m_MDLItemEntities[ entity->idxMDLEntity ].visible = true;
			entity->visible = true;
		}
		
		entity = entity->next;
	}
}



itementity_t*	CItemMng::SearchCursorEntityNeo (	const vec3_t	in_camPos,			 
													const vec3_t	in_camAngle,		 
													matrix4x4_t		*in_camTransMat,	 
													const float		in_minSquareDist,	 
													const vec2_t	in_mousePos,		 
													const vec2_t	in_viewportSize	)	 
{
	itementity_t	*thisEntity			=	NULL;
	itementity_t	*firstCandidate		=	NULL;
	itementity_t	*thisCandidate		=	NULL;
	itementity_t	*closestCandidate	=	NULL;
	bool			isFirstCandidate	=	true;

	vec3_t		difference;
	vec3_t		screenPos;

	bboxf_t		bbox;
	float		halfWidth,	halfHeight;

	float		red_ratio;
	float		diff;
	float		angle;
	float		minx,	maxx,	miny,	maxy,	minz,	maxz;
	float		sqr_length;
	float		trival;
	float		ang_diff;

	const float	DISCARD_X		=	100.0f;
	const float	DISCARD_Y		=	100.0f;

	m_minSquareDist	=	100000000.0f;

	 
	thisEntity		=	m_linked;
	while	(	thisEntity	)
	{
		if	(	! thisEntity->visible	)
		{
			thisEntity	=	thisEntity->next;
			continue;
		}

		VectorSubtract( difference, thisEntity->origin, in_camPos );
		thisEntity->squareDist	=	VectorDotProduct( difference, difference );
		if	(	thisEntity->squareDist	>	m_minSquareDist	)
		{
			thisEntity	=	thisEntity->next;
			continue;
		}

		thisEntity->closerEntity	=	NULL;
		thisEntity->fartherEntity	=	NULL;
		if	(	! isFirstCandidate	)
		{
			thisCandidate	=	firstCandidate;
			if	(	thisEntity->squareDist	<	firstCandidate->squareDist	)
			{
				while	(	1	)
				{
					if	(	thisCandidate->squareDist	<	thisEntity->squareDist	)
					{
						if	(	thisCandidate->fartherEntity	)
						{
							thisEntity->fartherEntity	=	thisCandidate->fartherEntity;
							thisCandidate->fartherEntity->closerEntity	=	thisEntity;
						}
						thisCandidate->fartherEntity	=	thisEntity;
						thisEntity->closerEntity		=	thisCandidate;
						break;
					}
					else if	(	thisCandidate->closerEntity	==	NULL	)
					{
						thisCandidate->closerEntity	=	thisEntity;
						thisEntity->fartherEntity	=	thisCandidate;
						closestCandidate			=	thisEntity;
						break;
					}
					thisCandidate	=	thisCandidate->closerEntity;
				}
			}
			else
			{
				while	(	1	)
				{
					if	(	thisCandidate->squareDist	>	thisEntity->squareDist	)
					{
						if	(	thisCandidate->closerEntity	)
						{
							thisEntity->closerEntity	=	thisCandidate->closerEntity;
							thisCandidate->closerEntity->fartherEntity	=	thisEntity;
						}
						thisCandidate->closerEntity	=	thisEntity;
						thisEntity->fartherEntity	=	thisCandidate;
						break;
					}
					else if	(	thisCandidate->fartherEntity	==	NULL	)
					{
						thisCandidate->fartherEntity	=	thisEntity;
						thisEntity->closerEntity		=	thisCandidate;
						break;
					}
					thisCandidate	=	thisCandidate->fartherEntity;
				}
			}
		}
		else
		{
			firstCandidate		=	thisEntity;
			closestCandidate	=	thisEntity;
			isFirstCandidate	=	false;
		}

		thisEntity	=	thisEntity->next;
	}
	
	if	(	closestCandidate	==	NULL	)
		return	NULL;

	halfWidth	=	(float)in_viewportSize[ 0 ] * 0.5f;
	halfHeight	=	(float)in_viewportSize[ 1 ] * 0.5f;
	float	ratioFactor	=	0.5f / (float)tan( g_camera.m_projectParm.fov / g_camera.m_projectParm.aspect * 0.5f * FX_DEGTORAD );

	thisEntity	=	closestCandidate;
	do
	{
		 
		red_ratio	=	in_viewportSize[ 1 ] * ratioFactor / (float)sqrt( thisEntity->squareDist );

		 
		in_camTransMat->Transform( screenPos, thisEntity->origin	);
		screenPos[ 0 ]	=	( 1.0f + screenPos[ 0 ] ) * halfWidth;
		screenPos[ 1 ]	=	( 1.0f - screenPos[ 1 ] ) * halfHeight;

		 
		diff	=	fabs( in_mousePos[ 1 ] - screenPos[ 1 ] );
		if	(	diff	>	DISCARD_Y * red_ratio	 )		goto	SKIP;
		diff	=	fabs( in_mousePos[ 0 ] - screenPos[ 0 ] );
		if	(	diff	>	DISCARD_X * red_ratio	 )		goto	SKIP;

		 
		VectorCopy( &bbox[0], thisEntity->mins );
		VectorCopy( &bbox[3], thisEntity->maxs );
		

		 
		diff	*=	diff;		 
		ang_diff	=	in_camAngle[ YAW ] - ( thisEntity->angles[ YAW ] - 90.0f );
		if	(	ang_diff	<	0.0f	)		
			ang_diff	+=	360.0f;

		 
		
		
		
		

		minx	=	fabs( bbox[ 0 ] );
		maxx	=	fabs( bbox[ 3 ] );
		miny	=	fabs( bbox[ 1 ] );
		maxy	=	fabs( bbox[ 4 ] );

		if	(	(	ang_diff	>=	0.0f	)	&&
				(	ang_diff	<	90.0f	)	)
		{
			 
			if	(	in_mousePos[ 0 ]	<	screenPos[ 0 ]	)
			{
				sqr_length	=	maxx*maxx + miny*miny;
				angle		=	(float)atan2( miny, maxx ) + ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs( sin( angle ) ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	minx*minx + maxy*maxy;
				angle		=	(float)atan2( maxy, minx ) + ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs( sin( angle ) ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}
		else if	(	(	ang_diff	>=	90.0f	)	&&
					(	ang_diff	<	180.0f	)	)
		{
			 
			if	(	in_mousePos[ 0 ]	<	screenPos[ 0 ]	)
			{
				sqr_length	=	maxx*maxx + maxy*maxy;
				angle		=	(float)atan2( maxy, maxx )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs( sin( angle ) ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	minx*minx + miny*miny;
				angle		=	(float)atan2( miny, minx )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs( sin( angle ) ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}
		else if	(	(	ang_diff	>=	180.0f	)	&&
					(	ang_diff	<	270.0f	)	)
		{
			 
			if	(	in_mousePos[ 0 ]	<	screenPos[ 0 ]	)
			{
				sqr_length	=	minx*minx + maxy*maxy;
				angle		=	(float)atan2( maxy, minx )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs( sin( angle ) ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	maxx*maxx + miny*miny;
				angle		=	(float)atan2( miny, maxx )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs( sin( angle ) ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}
		else
		{
			 
			if	(	in_mousePos[ 0 ]	<	screenPos[ 0 ]	)
			{
				sqr_length	=	minx*minx + miny*miny;
				angle		=	(float)atan2( miny, minx )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs( sin( angle ) ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	maxx*maxx + maxy*maxy;
				angle		=	(float)atan2( maxy, maxx )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)fabs( sin( angle ) ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}

		diff	=	fabs( in_mousePos[ 1 ] - screenPos[ 1 ] );
		diff	*=	diff;		 
		ang_diff	=	-in_camAngle[ PITCH ];
		if	(	ang_diff	<	0.0f	)
			ang_diff	+=	180.0f;

		minz	=	fabs( bbox[ 2 ] );
		maxz	=	fabs( bbox[ 5 ] );

		if	(	(	in_camAngle[ PITCH ]	>=	-90.0f	)	&&
				(	in_camAngle[ PITCH ]	<	0.0f	)	)
		{
			if	(	in_mousePos[ 1 ]	<	screenPos[ 1 ]	)
			{
				sqr_length	=	maxy*maxy + maxz*maxz;
				angle		=	(float)atan2( maxz,	maxy )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)sin( angle ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival  )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	minz*minz + miny*miny;
				angle		=	(float)atan2( minz,	miny )	+	ang_diff * FX_DEGTORAD;
				trival		=	(float)sin( angle ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}
		else
		{
			if	(	in_mousePos[ 1 ]	<	screenPos[ 1 ]	)
			{
				sqr_length	=	miny*miny + maxz*maxz;
				angle		=	(float)atan2( maxz,	miny )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)sin( angle ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival  )	)
					goto	SKIP;
			}
			else
			{
				sqr_length	=	minz*minz + maxy*maxy;
				angle		=	(float)atan2( minz,	maxy )	-	ang_diff * FX_DEGTORAD;
				trival		=	(float)sin( angle ) * red_ratio;
				if	(	diff	>	( sqr_length * trival * trival )	)
					goto	SKIP;
			}
		}

		m_minSquareDist	=	thisEntity->squareDist;

		
		
			
		
		return		thisEntity;
SKIP:
		thisEntity	=	thisEntity->fartherEntity;
	}	while	(	thisEntity	!=	NULL	);

	return	NULL;
}


itementity_t  *CItemMng::SearchCursorEntity( vec3_t pos , matrix4x4_t *transMat , float minSquareDist , vec2_t mouse , vec2_t viewport )
{
	itementity_t *entity;
	vec3_t diff;
	vec3_t scrMins , scrMaxs;
	vec3_t mins , maxs;
	float  squareDist , tmp;
	int selected = -1;
	float halfWidth , halfHeight;
	
	halfWidth  = (float) viewport[0] / 2.0f;
	halfHeight = (float) viewport[1] / 2.0f;

	m_minSquareDist = 100000000.0f;
	entity = m_linked;
	while( entity )
	{
		if( !entity->visible )
		{
			entity = entity->next;
			continue;
		}

		VectorSubtract( diff , entity->origin , pos );
		squareDist = VectorDotProduct( diff , diff );
		if( squareDist > m_minSquareDist )
		{
			entity = entity->next;
			continue;
		}
		
		VectorCopy( mins , entity->mins );
		VectorCopy( maxs , entity->maxs );

		CalcZAxisRotate( g_camera.m_angles[ YAW ] , mins , maxs );

		VectorAdd( mins , entity->origin , mins );
		VectorAdd( maxs , entity->origin , maxs );
		
		

		transMat->Transform( scrMins , mins );
		transMat->Transform( scrMaxs , maxs );

		scrMins[0] = ( 1.0f + scrMins[0] ) * halfWidth;
		scrMins[1] = ( 1.0f - scrMins[1] ) * halfHeight;

		scrMaxs[0] = ( 1.0f + scrMaxs[0] ) * halfWidth;
		scrMaxs[1] = ( 1.0f - scrMaxs[1] ) * halfHeight;

		if( scrMins[0] > scrMaxs[0] )
		{
			tmp = scrMaxs[0];
			scrMaxs[0] = scrMins[0];
			scrMins[0] = tmp;
		}
		if( scrMins[1] > scrMaxs[1] )
		{
			tmp = scrMaxs[1];
			scrMaxs[1] = scrMins[1];
			scrMins[1] = tmp;
		}

		if( mouse[0] > scrMaxs[0] || mouse[0] < scrMins[0] ) 
		{
			entity = entity->next;
			continue;
		}

		if( mouse[1] > scrMaxs[1] || mouse[1] < scrMins[1] ) 
		{
			entity = entity->next;
			continue;
		}

		

		m_minSquareDist = squareDist;
		selected = entity->idx;

		entity = entity->next;
	}

	if( selected < 0 )
		return NULL;
	
	m_MDLMng->m_focusedItem = m_entities[ selected ].idxMDLEntity;
	minSquareDist = m_minSquareDist;

	return &m_entities[ selected ];
}



itementity_t *CItemMng::SearchShortestEntity( vec3_t pos )
{
	itementity_t *entity;
	float minDist, dist;
	
	int selected = -1;
	
	minDist = 100000000.0f;
	entity = m_linked;

	while( entity )
	{
		dist = VectorDistance( pos, entity->origin );
		if( dist < minDist )
		{
			minDist = dist;
			selected = entity->idx;
		}

		entity = entity->next;
	}

	if( selected < 0 ) return NULL;
	return &m_entities[ selected ];
}

