
#include "vector.h"
#include <math.h>
#include "fastmath.h"


int VectorCompare( const vec3_t v1, const vec3_t v2 ) {
	if (v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2]) {
		return 0;
	}
			
	return 1;
}


float VectorNormalize( vec3_t v ) {
	float	length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = (float) sqrt(length);

	if ( length ) {
		ilength = 1 / length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}
		
	return length;
}





void VectorNormalizeFast( vec3_t v )
{
	float ilength;

	ilength = fast_rsqrt( VectorDotProduct( v, v ) );

	v[0] *= ilength;
	v[1] *= ilength;
	v[2] *= ilength;
}

float VectorNormalize2( vec3_t out , const vec3_t v ) {
	float	length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = (float) sqrt(length);

	if (length)
	{
		ilength = 1 / length;
		out[0] = v[0] * ilength;
		out[1] = v[1] * ilength;
		out[2] = v[2] * ilength;
	} else {
		VectorClear( out );
	}
		
	return length;

}


void VectorCrossProduct( vec3_t cross , const vec3_t v1 , const vec3_t v2 ) {
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

float VectorLength( const vec3_t v ) {
	return (float) sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] );
}


float VectorDistance( const vec3_t p1, const vec3_t p2 ) {
	vec3_t	v;

	VectorSubtract( v , p2 , p1 );
	return VectorLength( v );
}

float VectorDistanceSquared( const vec3_t p1, const vec3_t p2 ) {
	vec3_t	v;

	VectorSubtract ( v , p2, p1 );
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}


void VectorInverse( vec3_t v ){
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

void Vector4Scale( vec4_t out , const vec4_t in , float scale ) {
	out[0] = in[0] * scale;
	out[1] = in[1] * scale;
	out[2] = in[2] * scale;
	out[3] = in[3] * scale;
}

void VectorRotate( vec3_t out , vec3_t in , vec3_t matrix[3] )
{
	out[0] = VectorDotProduct( in , matrix[0] );
	out[1] = VectorDotProduct( in , matrix[1] );
	out[2] = VectorDotProduct( in , matrix[2] );
}

void VectorProjectOnPlane( vec3_t dest , vec3_t point , vec3_t planeRight , vec3_t planeUp )
{
   vec3_t right , up;
   float ratio;
   
   
   ratio = ( VectorDotProduct( point , planeRight ) ) / ( VectorDotProduct( planeRight , planeRight ) );
   VectorScale( right , planeRight , ratio );

   
   ratio = ( VectorDotProduct( point , planeUp ) ) / ( VectorDotProduct( planeUp , planeUp ) );
   VectorScale( up , planeUp , ratio );

   VectorAdd( dest , right , up );
}