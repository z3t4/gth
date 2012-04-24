#include "bsp.h"
#include "camera.h"
#include "d3drender.h"

#define GTH_BSP_MAIN_MODEL   0

void CBSP::PrepareScene( vec3_t pos , DWORD frameFlag )
{
	m_frameFlag = frameFlag + 1;

	
	if(g_d3dRender.m_bFog)
	{
		if( m_visibleQuality == GTH_WORLD_VISIBLE_QUALITY_HIGH )
		{
			m_removeDist = m_visibleDist;
			
			m_skyHeight = m_visibleDist * 0.4f;
		}
		else
		{
			m_removeDist = m_visibleDist * 0.5f;
			
			m_skyHeight = m_visibleDist * 0.6f;
		}
	}
	else
	{
		if( m_visibleQuality == GTH_WORLD_VISIBLE_QUALITY_HIGH )
		{
			m_removeDist = m_visibleDist;
			m_skyHeight = m_visibleDist * 0.7f;
		}
		else
		{
			m_removeDist = m_visibleDist * 0.5f;
			m_skyHeight = m_removeDist * 0.7f;
		}
	}

	VectorCopy( m_camPos , pos );
	MakeVis();
	TravelModel( GTH_BSP_MAIN_MODEL );
}

void CBSP::MakeVis()
{
	int   cluster;
	int   currLeaf;
	
	currLeaf = FindLeaf( m_camPos );
    cluster = m_leaves[ currLeaf ].cluster;

	if( cluster >= 0 )
	    m_visData = (BYTE *) &m_vis->data[ cluster * m_vis->rowSize ];
	else
		m_visData = (BYTE *) &m_vis->data[ m_vis->numClusters * m_vis->rowSize ];  
}

int CBSP::FindCluster( vec3_t pos )
{
	int   currLeaf;

	currLeaf = FindLeaf( pos );
    return m_leaves[ currLeaf ].cluster;
}

int CBSP::FindLeaf( vec3_t pos )
{
	float     d;
	node_t   *node;
	plane_t  *plane;
	int       num = 0;

	while( num >= 0 )
	{
		node = m_nodes + num;
		plane = &m_planes[ node->plane ];
		
		if( plane->type < 3 )
			d = pos[plane->type] - plane->dist;
		else
			d = VectorDotProduct( plane->normal , pos ) - plane->dist;

		if( d < 0 )
			num = node->child[1];
		else
			num = node->child[0];
	}

	return -1 - num;
}

void CBSP::TravelModel( int modelIdx )
{
	int i, j, cluster;
	leafbaseface_t *face;
	leaf_t *leaf;
	int numFaces;
	int *list;

	if( modelIdx != GTH_BSP_MAIN_MODEL )
		return;

	

	for( i = 0 ; i < m_numLeaves ; i++ )
	{
		cluster = m_leaves[i].cluster;
		if( cluster < 0 ) continue;
		if( !( m_visData[cluster >> 3] & ( 1 << (cluster & 7) ) ) )
			continue;

		leaf = &m_leaves[i];


		if( g_camera.m_frustum.Cull_Exact( leaf->mins , leaf->maxs ) )
			continue;
		
		if( !ChechVisibleDistance( leaf->mins , leaf->maxs ) )
			continue;

		list = &m_faceLists[ leaf->firstFace ];
		numFaces = leaf->numFaces;
		for ( j = 0 ; j < numFaces ; j++ , list++ )
		{
			face = &m_leafBaseFaces[ *list ];
			if( face->frameFlag == m_frameFlag )   continue;
			face->frameFlag = m_frameFlag;
			
			TravelFace( face );
		}
	}
}

inline int CBSP::ChechVisibleDistance( const vec3_t mins , const vec3_t maxs )
{
	vec3_t subMins, subMaxs;

	VectorSubtract( subMins , mins , m_camPos );
	VectorSubtract( subMaxs , maxs , m_camPos );
	
	VectorAverage( subMins , subMins , subMaxs );
	if( VectorDotProduct( subMins , subMins ) > m_removeDist * m_removeDist )
		return false;

	return true;
}

#define	NEG_DIST_EPSILON   -100.000f

inline void CBSP::TravelFace( leafbaseface_t *face )
{
	shadermap_t *shaderMap;
	float dist;
	
	if( face->type == FACETYPE_NORMAL )
	{
		if( !( m_shaderMaps[ face->shader ].flags & SHADER_CULL_NONE ) )
		{
			dist = VectorDotProduct( face->normal , m_camPos ) - face->texDist;
			if( ( m_shaderMaps[ face->shader ].flags & SHADER_CULL_FRONT ) && ( dist > 0 ) )
				return;
			else if( dist < NEG_DIST_EPSILON )
				return;
		}
	}
	else if( face->type == FACETYPE_PATCH )
	{
		if( g_camera.m_frustum.Cull_Exact( face->mins , face->maxs ) )
			return;
	}
	
	shaderMap = &m_shaderMaps[ face->shader ];
	if( shaderMap->LMFrameFlags[ face->lmap ] != m_frameFlag )
	{
		shaderMap->LMFrameFlags[ face->lmap ] = m_frameFlag;
		shaderMap->frameFlag = m_frameFlag;
		face->next = NULL;
		shaderMap->LMMaps[ face->lmap ].linked = face;
	}
	else
	{
		face->next = shaderMap->LMMaps[ face->lmap ].linked;
		shaderMap->LMMaps[ face->lmap ].linked = face;
	}
}