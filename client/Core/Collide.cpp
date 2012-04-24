#include "bsp.h"
#include "collide.h"


#define	DIST_EPSILON	(0.03125f)

vec3_t  trace_start , trace_end;
vec3_t  trace_mins , trace_maxs;
vec3_t  trace_extents;

trace_t trace_trace;
int     trace_contents;
int     trace_ispoint;		
int     checkCount = 0;

trace_t CBSP::Trace( vec3_t start , vec3_t mins , vec3_t maxs , vec3_t end , int brushmask )
{
	int i;
	int headnode = 0;

	checkCount++;

	
	memset (&trace_trace, 0, sizeof(trace_trace));
	trace_trace.fraction = 1.0f;
	trace_trace.surfaceFlags = 0;

	trace_contents = brushmask;
	VectorCopy( trace_start , start );
	VectorCopy( trace_end   , end  );
	VectorCopy( trace_mins  , mins );
	VectorCopy( trace_maxs  , maxs );

	if( VectorCompare( start , end ) )
	{
		VectorCopy( trace_trace.endpos , start );
		return trace_trace;
	}

	
	
	
	if( mins[0] == 0 && mins[1] == 0 && mins[2] == 0
		&& maxs[0] == 0 && maxs[1] == 0 && maxs[2] == 0 )
	{
		trace_ispoint = true;
		VectorClear( trace_extents );
	}
	else
	{
		trace_ispoint = false;
		trace_extents[0] = -mins[0] > maxs[0] ? -mins[0] : maxs[0];
		trace_extents[1] = -mins[1] > maxs[1] ? -mins[1] : maxs[1];
		trace_extents[2] = -mins[2] > maxs[2] ? -mins[2] : maxs[2];
	}

	
	
	
	RecursiveHullCheck( headnode , 0 , 1 , start , end );

	if( trace_trace.fraction == 1.0f )
	{
		VectorCopy( trace_trace.endpos , end );
	}
	else
	{
		for( i = 0 ; i < 3 ; i++ )
			trace_trace.endpos[i] = start[i] + trace_trace.fraction * ( end[i] - start[i] );
	}
	return trace_trace;
}


void CBSP::RecursiveHullCheck( int num , float p1f , float p2f , vec3_t p1 , vec3_t p2 )
{
	node_t   *node;
	plane_t	 *plane;
	float     t1, t2, offset;
	float     frac, frac2;
	float     idist;
	int       i;
	vec3_t    mid;
	int       side;
	float     midf;

	if( trace_trace.fraction <= p1f )
		return;		

	
	if( num < 0 )
	{
		TraceToLeaf( -1-num );
		return;
	}

	
	
	
	
	node = m_nodes + num;
	plane = &m_planes[ node->plane ];

	if( plane->type < 3 )
	{
		t1 = p1[plane->type] - plane->dist;
		t2 = p2[plane->type] - plane->dist;
		offset = trace_extents[plane->type];
	}
	else
	{
		t1 = VectorDotProduct( plane->normal , p1 ) - plane->dist;
		t2 = VectorDotProduct( plane->normal , p2 ) - plane->dist;
		if( trace_ispoint )
			offset = 0;
		else
			offset = fabs(trace_extents[0]*plane->normal[0]) 
			       + fabs(trace_extents[1]*plane->normal[1]) 
				   + fabs(trace_extents[2]*plane->normal[2]);
	}

	
	if( t1 >= offset && t2 >= offset )
	{
		RecursiveHullCheck( node->child[0] , p1f , p2f , p1 , p2 );
		return;
	}
	if( t1 < -offset && t2 < -offset )
	{
		RecursiveHullCheck( node->child[1] , p1f , p2f , p1 , p2 );
		return;
	}

	
	if( t1 < t2 )
	{
		idist = 1.0f / ( t1 - t2 );
		side = 1;
		frac2 = ( t1 + offset + DIST_EPSILON ) * idist;
		frac = ( t1 - offset + DIST_EPSILON ) * idist;
	}
	else if (t1 > t2)
	{
		idist = 1.0f / ( t1 - t2 );
		side = 0;
		frac2 = ( t1 - offset - DIST_EPSILON ) * idist;
		frac = ( t1 + offset + DIST_EPSILON ) * idist;
	}
	else
	{
		side = 0;
		frac = 1.0f;
		frac2 = 0;
	}

	
	if( frac < 0.0f )
		frac = 0.0f;
	if( frac > 1.0f )
		frac = 1.0f;
		
	midf = p1f + ( p2f - p1f ) * frac;
	for( i = 0 ; i < 3 ; i++ )
		mid[i] = p1[i] + frac * ( p2[i] - p1[i] );

	RecursiveHullCheck( node->child[ side ] , p1f , midf , p1 , mid );


	
	if( frac2 < 0.0f )
		frac2 = 0.0f;
	if( frac2 > 1.0f )
		frac2 = 1.0f;
		
	midf = p1f + ( p2f - p1f ) * frac2;
	for( i = 0 ; i < 3 ; i++ )
		mid[i] = p1[i] + frac2 * ( p2[i] - p1[i] );

	RecursiveHullCheck( node->child[ side^1 ] , midf , p2f , mid , p2 );
}

void CBSP::TraceToLeaf( int leafnum )
{
	int       k;
	int       brushnum;
	leaf_t	 *leaf;
	brush_t  *b;

	leaf = &m_leaves[ leafnum ];
	
	
	for( k = 0 ; k < leaf->numBrushes ; k++ )
	{
		brushnum = m_brushLists[ leaf->firstBrush + k ];
		b = &m_brushes[ brushnum ];
		if( b->checkCount == checkCount )
			continue;	
		b->checkCount = checkCount;

		if( !( m_shaderRefs[ b->shaderInfo ].contents & trace_contents ) )
			continue;

		ClipBoxToBrush( trace_mins , trace_maxs , trace_start , trace_end , &trace_trace , b );
		if( !trace_trace.fraction )
			return;
	}

}

void CBSP::ClipBoxToBrush( vec3_t mins , vec3_t maxs , vec3_t p1 , vec3_t p2 
						 , trace_t *trace , brush_t *brush )
{
	int       i, j;
	plane_t  *plane, *clipplane;
	float     dist;
	float     enterfrac, leavefrac;
	vec3_t    ofs;
	float     d1, d2;
	int       getout, startout;
	float     f;
	brushside_t  *side, *leadside;

	enterfrac = -1;
	leavefrac = 1;
	clipplane = NULL;

	if( !brush->numSides )
		return;

	getout = false;
	startout = false;
	leadside = NULL;

	for( i = 0 ; i < brush->numSides ; i++ )
	{
		side = &m_brushSides[ brush->firstSide + i ];
		plane = &m_planes[ side->plane ];

		

		if( !trace_ispoint )
		{	

			

			
			for( j = 0 ; j < 3 ; j++ )
			{
				if( plane->normal[j] < 0 )
					ofs[j] = maxs[j];
				else
					ofs[j] = mins[j];
			}
			dist = VectorDotProduct( ofs, plane->normal );
			dist = plane->dist - dist;
		}
		else
		{	
			dist = plane->dist;
		}

		d1 = VectorDotProduct( p1 , plane->normal ) - dist;
		d2 = VectorDotProduct( p2 , plane->normal ) - dist;

		if( d2 > 0 )
			getout = true;	
		if( d1 > 0 )
			startout = true;

		
		if( d1 > 0 && d2 >= d1 )
			return;

		if( d1 <= 0 && d2 <= 0 )
			continue;

		
		if( d1 > d2 )
		{	
			f = ( d1 - DIST_EPSILON ) / ( d1 - d2 );
			if( f > enterfrac )
			{
				enterfrac = f;
				clipplane = plane;
				leadside = side;
			}
		}
		else
		{	
			f = ( d1 + DIST_EPSILON ) / ( d1 - d2 );
			if( f < leavefrac )
				leavefrac = f;
		}
	}

	if( !startout )
	{	
		trace->startsolid = true;
		if( !getout )
			trace->allsolid = true;
		return;
	}
	if( enterfrac < leavefrac )
	{
		if( enterfrac > -1 && enterfrac < trace->fraction )
		{
			if( enterfrac < 0 )
				enterfrac = 0;
			trace->fraction = enterfrac;
			trace->plane = clipplane;
			trace->surfaceFlags = m_shaderRefs[ leadside->shaderInfo ].surfaceFlags;
			trace->contents = m_shaderRefs[ leadside->shaderInfo ].contents;
		}
	}
}


int CBSP::PointContents( vec3_t pos )
{
	int contents;
	int leaf;
	int *idxBrush;
	plane_t *plane;
	brush_t *brush;
	brushside_t *side;
	int numBrushes,numSides;
	int i,j;
	float dist;
	
	contents = 0;

	leaf = FindLeaf( pos );
	if( leaf < 0 )
        return contents;

	numBrushes = m_leaves[ leaf ].numBrushes;
	idxBrush = &m_brushLists[ m_leaves[ leaf ].firstBrush ];

	for ( i = 0 ; i < numBrushes ; i++ , idxBrush++ )
	{
		brush = &m_brushes[ *idxBrush ];

		side = &m_brushSides[ brush->firstSide ];
		numSides = brush->numSides;
		for( j = 0 ; j < numSides ; j++ , side++ )
		{
			plane = &m_planes[ side->plane ];

			if( plane->type < 3 )
			{
				dist = pos[ plane->type ] - plane->dist;
			}
			else
			{
				dist = VectorDotProduct( plane->normal , pos ) - plane->dist;
			}
		
			if( dist >= 0 )	 break;
		}
		
		if( j == numSides )
		{
			contents |= m_shaderRefs[ brush->shaderInfo ].contents;
			return contents;
		}
	}

	return contents;
}


