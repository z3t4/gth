#include "bsp.h"
#include "d3drender.h"

#define SKY_SIDE_SIZE  6
#define SIDE_DIV_SIZE  11

#define SPHERE_RAD  10.0
#define EYE_RAD      9.0

#define SCALE_S 4.0f
#define SCALE_T 4.0f

#define VERTEX_SIZE_ENVIRON_SKY   ( 4 * 6 )
#define ELEMENT_SIZE_ENVIRON_SKY  ( 6 * 6 )
#define SIDE_VERTEX_SIZE_SKY      ( SIDE_DIV_SIZE * SIDE_DIV_SIZE )
#define SIDE_ELEMENT_SIZE_SKY     ( ( SIDE_DIV_SIZE - 1 ) * ( SIDE_DIV_SIZE - 1 ) * 6 )

 
#define	SKY_SIZE		2.0

 
#include <assert.h>


#define SIDE_SKY          20
#define PLANETRADIUS      280.0f
#define ATMOSPHERERADIUS  295.0f


void CBSP::CalcSkySize( int shaderFlag , int *numVerts ,  int *numElems )
{
	if( shaderFlag & SHADER_SKY_ENVIRON )
	{
		*numVerts = VERTEX_SIZE_ENVIRON_SKY;
		*numElems = ELEMENT_SIZE_ENVIRON_SKY;
	}
	else
	{
		if(g_d3dRender.m_bFog)
		{
			
			*numVerts = (SIDE_SKY + 1) * (SIDE_SKY + 1);   
			*numElems = SIDE_SKY * SIDE_SKY * 6;       
		}
		else
		{
			*numVerts = SIDE_VERTEX_SIZE_SKY * SKY_SIDE_SIZE;
			*numElems = SIDE_ELEMENT_SIZE_SKY * SKY_SIDE_SIZE;
		}
	}
}

void CBSP::GenSky( face_t *face, int baseVB, int shaderFlag, vertex_t *ptrStaticVB, WORD *ptrStaticEB )
{
	int i , u , v;

	vertex_t *ptrVB = ptrStaticVB;
	WORD     *ptrEB = ptrStaticEB;
	
	 
	if( shaderFlag & SHADER_SKY_ENVIRON )
	{
		GenEnvSkyBox( ptrVB, ptrEB, baseVB );
	}
	else
	{
		if(g_d3dRender.m_bFog)
		{
			GenSkyBox_Lee( ptrVB );
		}
		else
		{
			GenSkyBox_Neo( ptrVB );

		}

	}

	if( shaderFlag & SHADER_SKY_ENVIRON )
	{
		face->sizeVB = VERTEX_SIZE_ENVIRON_SKY;
		face->sizeEB = ELEMENT_SIZE_ENVIRON_SKY;
	}
	else
	{
		if(g_d3dRender.m_bFog)
		{

			int divisions = (int)sqrt(SIDE_SKY * SIDE_SKY);
			int index = 0;
			for ( int z = 0; z < divisions; z++)
			{
				for (int x = 0; x < divisions; x++)
				{
					int startvert = (z * (divisions+1) + x);
				  
					
					*ptrEB++ = (WORD)(baseVB + startvert);
					*ptrEB++ = (WORD)(baseVB + startvert + 1);
					*ptrEB++ = (WORD)(baseVB + startvert + divisions + 1);
				  
					
					*ptrEB++ = (WORD)(baseVB + startvert + 1);
					*ptrEB++ = (WORD)(baseVB + startvert + divisions + 2);
					*ptrEB++ = (WORD)(baseVB + startvert + divisions + 1);
				}
			}
			face->sizeVB = (SIDE_SKY + 1) * (SIDE_SKY + 1);
			face->sizeEB = SIDE_SKY * SIDE_SKY * 6;
		}
		else
		{
			for( i = 0 ; i < SKY_SIDE_SIZE ; i++ )
			{
				for( v = 0 ; v < SIDE_DIV_SIZE - 1 ; v++ )
				{
					for( u = 0 ; u < SIDE_DIV_SIZE - 1 ; u++ )
					{
						*ptrEB++ = (WORD) ( baseVB + v * SIDE_DIV_SIZE + u );
						*ptrEB++ = (WORD) ( baseVB + (v+1) * SIDE_DIV_SIZE + u );
						*ptrEB++ = (WORD) ( baseVB + v * SIDE_DIV_SIZE + u + 1 );

						*ptrEB++ = (WORD) ( baseVB + v * SIDE_DIV_SIZE + u + 1 );
						*ptrEB++ = (WORD) ( baseVB + (v+1) * SIDE_DIV_SIZE + u );
						*ptrEB++ = (WORD) ( baseVB + (v+1) * SIDE_DIV_SIZE + u + 1 );
					}
				}
				baseVB += SIDE_VERTEX_SIZE_SKY;
			}
			face->sizeVB = SIDE_VERTEX_SIZE_SKY * SKY_SIDE_SIZE;
			face->sizeEB = SIDE_ELEMENT_SIZE_SKY * SKY_SIDE_SIZE;
		}
	}
}

void CBSP::GenEnvSkyBox( vertex_t *ptrVB , WORD *ptrEB , int baseVB )
{
	
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
    VectorSet( ptrVB->point , -1.0f ,  1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f ,  1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 1.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f , -1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 1.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f , -1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 0.0f );           ptrVB++;

    
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f ,  1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f , -1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 1.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f , -1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f ,  1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 1.0f );           ptrVB++;

    
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f ,  1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f ,  1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 1.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f ,  1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f ,  1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 1.0f );           ptrVB++;

    
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f , -1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f , -1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 1.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f , -1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f , -1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 1.0f );           ptrVB++;
	
    
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f ,  1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f ,  1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 1.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f , -1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point ,  1.0f , -1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 1.0f );           ptrVB++;
    
    
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f ,  1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f , -1.0f , -1.0f );  
	Vector2Set( ptrVB->texUV , 0.0f , 1.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f , -1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 0.0f );           ptrVB++;
	ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	VectorSet( ptrVB->point , -1.0f ,  1.0f ,  1.0f );  
	Vector2Set( ptrVB->texUV , 1.0f , 1.0f );           ptrVB++;

	
    *ptrEB++ = (WORD) ( baseVB + 0+0 );   *ptrEB++ = (WORD) ( baseVB + 0+3 );   *ptrEB++ = (WORD) ( baseVB + 0+2 );
    *ptrEB++ = (WORD) ( baseVB + 0+2 );   *ptrEB++ = (WORD) ( baseVB + 0+1 );   *ptrEB++ = (WORD) ( baseVB + 0+0 );
    *ptrEB++ = (WORD) ( baseVB + 4+0 );   *ptrEB++ = (WORD) ( baseVB + 4+3 );   *ptrEB++ = (WORD) ( baseVB + 4+2 );
    *ptrEB++ = (WORD) ( baseVB + 4+2 );   *ptrEB++ = (WORD) ( baseVB + 4+1 );   *ptrEB++ = (WORD) ( baseVB + 4+0 );
    *ptrEB++ = (WORD) ( baseVB + 8+0 );   *ptrEB++ = (WORD) ( baseVB + 8+3 );   *ptrEB++ = (WORD) ( baseVB + 8+2 );
    *ptrEB++ = (WORD) ( baseVB + 8+2 );   *ptrEB++ = (WORD) ( baseVB + 8+1 );   *ptrEB++ = (WORD) ( baseVB + 8+0 );
    *ptrEB++ = (WORD) ( baseVB +12+0 );   *ptrEB++ = (WORD) ( baseVB +12+3 );   *ptrEB++ = (WORD) ( baseVB +12+2 );
    *ptrEB++ = (WORD) ( baseVB +12+2 );   *ptrEB++ = (WORD) ( baseVB +12+1 );   *ptrEB++ = (WORD) ( baseVB +12+0 );
    *ptrEB++ = (WORD) ( baseVB +16+0 );   *ptrEB++ = (WORD) ( baseVB +16+3 );   *ptrEB++ = (WORD) ( baseVB +16+2 );
    *ptrEB++ = (WORD) ( baseVB +16+2 );   *ptrEB++ = (WORD) ( baseVB +16+1 );   *ptrEB++ = (WORD) ( baseVB +16+0 );
    *ptrEB++ = (WORD) ( baseVB +20+0 );   *ptrEB++ = (WORD) ( baseVB +20+3 );   *ptrEB++ = (WORD) ( baseVB +20+2 );
    *ptrEB++ = (WORD) ( baseVB +20+2 );   *ptrEB++ = (WORD) ( baseVB +20+1 );   *ptrEB++ = (WORD) ( baseVB +20+0 );

}


void CBSP::GenSkyBox( vertex_t *ptrVB )
{
	vec3_t orig, drow, dcol;
    float startXY = 1.0f;
	float stepXY = 2.0f / ( SIDE_DIV_SIZE - 1 );
	float startZ  = 0.5f;
    float stepZ  = 1.0f / ( SIDE_DIV_SIZE - 1 );
    
    
    orig[0] = -startXY;
    orig[1] = startXY;
    orig[2] = startZ;
    drow[0] = 0.0;
    drow[1] = -stepXY;
    drow[2] = 0.0;
    dcol[0] = stepXY;
    dcol[1] = 0.0;
    dcol[2] = 0.0;
    GenSkyBoxSide( SKYBOX_TOP , orig , drow , dcol , ptrVB );
	ptrVB += SIDE_DIV_SIZE * SIDE_DIV_SIZE;

	

    
    orig[0] = startXY;
    orig[1] = startXY;
    orig[2] = startZ;
    drow[0] = 0.0;
    drow[1] = 0.0;
    drow[2] = -stepZ;
    dcol[0] = -stepXY;
    dcol[1] = 0.0;
    dcol[2] = 0.0;
    GenSkyBoxSide( SKYBOX_FRONT , orig , drow , dcol , ptrVB );
	ptrVB += SIDE_DIV_SIZE * SIDE_DIV_SIZE;

    
    orig[0] = startXY;
    orig[1] = -startXY;
    orig[2] = startZ;
    drow[0] = 0.0;
    drow[1] = 0.0;
    drow[2] = -stepZ;
    dcol[0] = 0.0;
    dcol[1] = stepXY;
    dcol[2] = 0.0;
    GenSkyBoxSide( SKYBOX_RIGHT , orig , drow , dcol , ptrVB );
	ptrVB += SIDE_DIV_SIZE * SIDE_DIV_SIZE;

    
    orig[0] = -startXY;
    orig[1] = -startXY;
    orig[2] = startZ;
    drow[0] = 0.0;
    drow[1] = 0.0;
    drow[2] = -stepZ;
    dcol[0] = stepXY;
    dcol[1] = 0.0;
    dcol[2] = 0.0;
    GenSkyBoxSide( SKYBOX_BACK , orig , drow , dcol , ptrVB );
	ptrVB += SIDE_DIV_SIZE * SIDE_DIV_SIZE;

    
    orig[0] = -startXY;
    orig[1] = startXY;
    orig[2] = startZ;
    drow[0] = 0.0;
    drow[1] = 0.0;
    drow[2] = -stepZ;
    dcol[0] = 0.0;
    dcol[1] = -stepXY;
    dcol[2] = 0.0;
    GenSkyBoxSide( SKYBOX_LEFT , orig , drow , dcol , ptrVB );

	
	
}


void CBSP::GenSkyBoxSide( int side, vec3_t orig, vec3_t drow, vec3_t dcol, vertex_t * ptrVB )
{
    vec3_t pos, w, row, *v;
    vec2_t *tc;
    float p;
    int r, c;
    float d, b, t;

    d = EYE_RAD;      
    b = SPHERE_RAD;  
    
    VectorCopy( row , orig );
    for (r = 0; r < SIDE_DIV_SIZE; ++r)
    {
		VectorCopy( pos , row );
	    for (c = 0; c < SIDE_DIV_SIZE; ++c)
		{
			v  = &ptrVB->point;
            tc = &ptrVB->texUV;
    
	        
	        VectorCopy( (*v) , pos );
	        VectorCopy( w , pos );

	        
	        p = (float) sqrt( VectorDotProduct(w, w) );
	        w[0] /= p;
	        w[1] /= p;
	        w[2] /= p;

	        
	        t = (float) sqrt(d*d*(w[2]*w[2]-1.0) + b*b) - d*w[2];
	        w[0] *= t;
	        w[1] *= t;

	        
	        (*tc)[0] = w[0] / (2.0f * SCALE_S);
	        (*tc)[1] = w[1] / (2.0f * SCALE_T);
	    
			ColorSet( ptrVB->diffuse , 255 , 255 , 255 , 255 );
	        VectorAdd( pos , pos , dcol );
	        ptrVB++;
		}
	    VectorAdd( row , row , drow );
    }
}

 
 
 
 
 
 
 
 
void CBSP::GenSkyBox_Neo	(	vertex_t	*in_vb	)
{
	vec3_t	baseaxis[18]	=
	{
		 1, 0, 0,	 0,-1, 0,	 0, 0, 1,		 
		 0, 0, 1,	 0, 1, 0,	 1, 0, 0,		 
		 0,-1, 0,	-1, 0, 0,	 0, 0, 1,		 
		-1, 0, 0,	 0, 1, 0,	 0, 0, 1,		 
		 0, 0,-1,	 0, 1, 0,	-1, 0, 0,		 
		 0, 1, 0,	 1, 0, 0,	 0, 0, 1		 
	};

	int	sideIdx;
	for	(	sideIdx	=	0;	\
			sideIdx	<	SKY_SIDE_SIZE;	\
			++sideIdx	)
	{
		GenSkyVertices( baseaxis[ sideIdx * 3 ], baseaxis[ sideIdx * 3 + 2 ], baseaxis[ sideIdx * 3 + 1 ],
						SIDE_DIV_SIZE, SKY_SIZE, &in_vb[ sideIdx * SIDE_DIV_SIZE * SIDE_DIV_SIZE ] );
	}
	
	const float	SKYHEIGHT	=	5.0f;
	GenSkyTexCoords( SKYHEIGHT, SKY_SIDE_SIZE * SIDE_DIV_SIZE * SIDE_DIV_SIZE, in_vb );
}

 
 
 
 
 
 
 
void	CBSP::GenSkyVertices	(	vec3_t		in_forward,
									vec3_t		in_up,
									vec3_t		in_right,
									int			in_numTess,
									float		in_skySize,
									vertex_t	*in_vb	)
{
	float	distance	=	in_skySize / (float)( in_numTess - 1 );
	vec3_t	origin;
	
	origin[0]	=	in_forward[0] - in_up[0] - in_right[0];
	origin[1]	=	in_forward[1] - in_up[1] - in_right[1];
	origin[2]	=	in_forward[2] - in_up[2] - in_right[2];

	float	halfsize	=	in_skySize * 0.5f;
	VectorScale( origin, origin, halfsize );

	vec3_t	up,	right;
	VectorCopy( up, in_up );
	VectorScale( up, up, distance );
	VectorCopy( right, in_right );
	VectorScale( right, right, distance );

	int	rowidx,	colidx;

	vec3_t	vertex;
	for	(	rowidx	=	0;	\
			rowidx	<	in_numTess;	\
			++rowidx	)
	{
		VectorCopy( vertex, origin );
		for	(	colidx	=	0;	\
				colidx	<	in_numTess;	\
				++colidx	)
		{
			VectorCopy( in_vb->point, vertex );
			VectorAdd( vertex, vertex, right );
			ColorSet( in_vb->diffuse, 255, 255, 255, 255 );
			in_vb++;
		}
		VectorAdd( origin, origin, up );
	}
}

 
 
 
 
 
 
 
void	CBSP::GenSkyTexCoords	(	float		in_skyHeight,
									int			in_numVtx,
									vertex_t	*in_vb	)
{
	const float	SKY_RADIUS	=	5.25f;
	float	temp	=	in_skyHeight * in_skyHeight - SKY_RADIUS * SKY_RADIUS;
	int		index;
	vec3_t	direction;
	for	(index = 0; index < in_numVtx; ++index)
	{
		VectorCopy( direction, in_vb->point );

		float	SQRDIST_DIR	=	VectorDotProduct( direction, direction );
		float	INTER_A		=	in_skyHeight * fabsf( direction[2] ) / SQRDIST_DIR;
		float	INTER_B		=	temp / SQRDIST_DIR;
		float	INTER_C		=	sqrtf( INTER_A * INTER_A - INTER_B ) - INTER_A;

		assert( INTER_C > 0.0f );

		float	height	=	( fabsf( direction[2] ) * INTER_C + in_skyHeight ) / SKY_RADIUS;
		height	=	fabsf( height );
		if	(	height	>	1.0f	)
			height	=	1.0f;
		float	latitude	=	( __HALFPI - asinf( height ) ) / __HALFPI;

		INTER_C	=	direction[0] * direction[0] + direction[1] * direction[1];
		if	(	INTER_C	>	0.000001f	)
		{
			INTER_C	=	1.0f / sqrtf( INTER_C );
		}
		in_vb->texUV[0]	=	0.5f + direction[0] * latitude * INTER_C * 2.0f;
		in_vb->texUV[1]	=	1.0f - ( 0.5f + direction[1] * latitude * INTER_C * 2.0f );
		in_vb++;
	}
}

void CBSP::GenSkyBox_Lee(vertex_t *in_vb)
{
    
	int divisions    = (int)sqrt(SIDE_SKY * SIDE_SKY);
	float plane_size = 2.0f * (float)sqrt( (ATMOSPHERERADIUS*ATMOSPHERERADIUS) - (PLANETRADIUS*PLANETRADIUS) );
	float delta      = plane_size / (float)divisions;
	float tex_delta1 = 1.0f / (float)divisions;
	float tex_delta2 = 1.0f / (float)divisions;
	
	float x_dist     = 0.0f;
	float z_dist     = 0.0f;
	float x_height   = 0.0f;
	float z_height   = 0.0f;
	float height     = 0.0f;

	for(int z = 0; z <= divisions; z++)
	{
		for(int x = 0; x <= divisions; x++)
		{
			x_dist = (-0.5f * plane_size) + ((float)x * delta);
			z_dist = (-0.5f * plane_size) + ((float)z * delta);
			
			x_height = (x_dist * x_dist) / ATMOSPHERERADIUS;
			z_height = (z_dist * z_dist) / ATMOSPHERERADIUS;
			height   = x_height + z_height;

			in_vb->point[0] = x_dist * 800;
			in_vb->point[2] = (0.0f - height) * 800;
			in_vb->point[1] = z_dist * 800;

			in_vb->texUV[0] = ((float)x * tex_delta1);
			in_vb->texUV[1] = ((float)z * tex_delta1);

			ColorSet( in_vb->diffuse, 255, 255, 255, 255 );
			in_vb++;
		}
	}
}


