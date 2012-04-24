#include "bsp.h"

#define LEVEL_WIDTH(lvl) ((1 << (lvl+1)) + 1)

int CBSP::CalcPatchSize( face_t *face , int *numVerts ,  int *numElems )
{
	int size[2];

	vertex_t *vert = &m_vertices[ face->vertexOffset ];

	if( !FindPatchSize( face->patchSize , vert , size ) )
		return false;

	*numVerts = size[0] * size[1];
	*numElems = ( size[0] - 1 ) * ( size[1] - 1 ) * 6;

	return true;
}

void CBSP::GenPatch( face_t *face , int baseVB , vertex_t *ptrStaticVB , WORD *ptrStaticEB )
{
	int step[2], size[2], len, u, v, p;
    vertex_t *vert;
	
	vertex_t *ptrVB = m_tempVB;
	WORD     *ptrEB = ptrStaticEB;
	
    vert = &m_vertices[ face->vertexOffset ];

	
	if( !FindPatchSize( face->patchSize , vert , size ) )
	{
		face->sizeVB = 0;
		face->sizeEB = 0;
		return;
	}

	len = size[0] * size[1];
    
    
    step[0] = (size[0]-1) / (face->patchSize[0]-1);
    step[1] = (size[1]-1) / (face->patchSize[1]-1);
	
    for( v = 0 ; v < size[1] ; v += step[1] )
    {
	    for( u = 0 ; u < size[0] ; u += step[0] , vert++ )
		{
	        p = v * size[0] + u;
			memcpy( ptrVB + p , vert , sizeof( vertex_t ) );
		}
    }

	

	for( u = 0; u < size[0] ; u += step[0] )
	{
		FillBezierCurve( face->patchSize[1] , size[1] , size[0] , u , ptrVB );
	}

	for( v = 0; v < size[1]; v++ )
	{
		FillBezierCurve( face->patchSize[0] , size[0] , 1 , v * size[0] , ptrVB );
	}

	memcpy( ptrStaticVB , ptrVB , sizeof( vertex_t ) * len );

    for (v = 0; v < size[1]-1; ++v)
    {
	    for (u = 0; u < size[0]-1; ++u)
		{
	        *ptrEB++ = (WORD) ( baseVB + v * size[0] + u );
	        *ptrEB++ = (WORD) ( baseVB + (v+1) * size[0] + u );
	        *ptrEB++ = (WORD) ( baseVB + v * size[0] + u + 1 );
	        *ptrEB++ = (WORD) ( baseVB + v * size[0] + u + 1 );
	        *ptrEB++ = (WORD) ( baseVB + (v+1) * size[0] + u );
	        *ptrEB++ = (WORD) ( baseVB + (v+1) * size[0] + u + 1 );
		}
    }

	face->sizeVB = len;
	face->sizeEB = ( size[0] - 1 ) * ( size[1] - 1 ) * 6;

}


void CBSP::FillBezierCurve( int patchSize, unsigned int size, unsigned int stride, 
							    unsigned int pos, vertex_t *patchVertex )
{
	vertex_t *vertex;
	unsigned int step, halfstep, i, mid, p0, p1, dounlestep ;
	unsigned int half0, half1;

	vec3_t  positionA , positionB;
	vec3_t  normalA   , normalB;
	vec2_t  texCoordA , texCoordB;
	vec2_t  LMCoordA  , LMCoordB;
	color_t colorA    , colorB;

	step = ( size - 1 ) / ( patchSize - 1 );
	vertex = &patchVertex[pos];
	
	while( step > 0 )
	{
		halfstep = step >> 1 ;
		dounlestep = step << 1;
	    for( i = 0; i < size - 1 ; i += dounlestep )
		{

		   	p0  = ( i + step*0 ) * stride;
			mid = ( i + step*1 ) * stride;
			p1  = ( i + step*2 ) * stride;

	        VectorAverage( positionA , vertex[p0].point  , vertex[mid].point );
	        VectorAverage( positionB , vertex[mid].point , vertex[p1].point );
	        VectorAverage( vertex[mid].point , positionA , positionB );

			VectorAverage( normalA , vertex[p0].normal  , vertex[mid].normal );
	        VectorAverage( normalB , vertex[mid].normal , vertex[p1].normal );
	        VectorAverage( vertex[mid].normal , normalA , normalB );

			Vector2Average( texCoordA , vertex[p0].texUV  , vertex[mid].texUV );
	        Vector2Average( texCoordB , vertex[mid].texUV , vertex[p1].texUV  );
	        Vector2Average( vertex[mid].texUV , texCoordA , texCoordB );

			Vector2Average( LMCoordA , vertex[p0].lmapUV  , vertex[mid].lmapUV );
	        Vector2Average( LMCoordB , vertex[mid].lmapUV , vertex[p1].lmapUV );
	        Vector2Average( vertex[mid].lmapUV , LMCoordA , LMCoordB );

			ColorAverage( colorA , vertex[p0].diffuse  , vertex[mid].diffuse );
	        ColorAverage( colorB , vertex[mid].diffuse , vertex[p1].diffuse );
	        ColorAverage( vertex[mid].diffuse , colorA , colorB );

			if ( halfstep > 0 )
			{
				half0 = ( i + halfstep*1 ) * stride;
				half1 = ( i + halfstep*3 ) * stride;

		        VectorCopy( vertex[half0].point , positionA );
		        VectorCopy( vertex[half1].point , positionB );

				VectorCopy( vertex[half0].normal , normalA );
		        VectorCopy( vertex[half1].normal , normalB );

				Vector2Copy( vertex[half0].texUV , texCoordA );
		        Vector2Copy( vertex[half1].texUV , texCoordB );

				Vector2Copy( vertex[half0].lmapUV , LMCoordA );
		        Vector2Copy( vertex[half1].lmapUV , LMCoordB );

				ColorCopy( vertex[half0].diffuse , colorA );
		        ColorCopy( vertex[half1].diffuse , colorB );
			}
		}

		step = halfstep;
	}
}

int CBSP::FindPatchLevel( vec3_t *v )
{
    int level;
    vec3_t a, b, dist;

    
	for( level=0; level < m_maxPatchLevel - 1; level++ ) {
		
		VectorAverage( a , v[0], v[1] );
		VectorAverage( b , v[1], v[2] );
		VectorAverage( v[2] , a, b );

		
		VectorSubtract( dist , v[2], v[1] );

		
		if( VectorDotProduct( dist, dist ) < m_subDivTolerance * m_subDivTolerance )
			break;

		
		VectorCopy( v[1] , a );
	}

	return( level );
}

int CBSP::FindPatchSize( int *patchSize, vertex_t *cp, int *size )
{
    int u, v, found, level;
    vec3_t a, b;
    vec3_t test[3];
    
    
    found = 0;
    for (v=0; v < patchSize[1]; v++)
    {
		for (u=0; u < patchSize[0]-1; u += 2)
		{
			VectorCopy( a , cp[v * patchSize[0] + u].point );
			VectorCopy( b , cp[v * patchSize[0] + u + 2].point );

			if (!VectorCompare(a,b))
			{
				found = 1;
				break;
			}
		}

		if (found) break;
    }

    if (!found) {
		GTH_ERROR( GTH_ERROR_PRINT , "Bad Patch control points" );
		return false;
	}

    
    VectorCopy( test[0] , a );
    VectorCopy( test[1] , (a+3) );
    VectorCopy( test[2] , b );
    level = FindPatchLevel(test);
    
	size[0] = 2 * ( patchSize[0] - 1 ) + 1;
    
    
    found = 0;
    for (u=0; u < patchSize[0]; u++)
    {
		for (v=0; v < patchSize[1]-1; v += 2)
		{
			VectorCopy( a , cp[v * patchSize[0] + u].point );
			VectorCopy( b , cp[(v + 2) * patchSize[0] + u].point );
			if (!VectorCompare(a,b))
			{
				found = 1;
				break;
			}
		}

		if (found) 
			break;
    }

    if (!found) {
		GTH_ERROR( GTH_ERROR_PRINT , "Bad Patch control points" );
		return false;
	}

    
    VectorCopy( test[0] , a );
    VectorCopy( test[1] , (a+patchSize[0]*3) );
    VectorCopy( test[2] , b );
    level = FindPatchLevel(test);
    
	size[1] = 2 * ( patchSize[1] - 1 ) + 1;

	return true;
}