#include <math.h>
#include "math.h"
#include "fastmath.h"

void AngleToVector( vec3_t angles , vec3_t forward )
{
	float  angle;
	float  sp, sy, cp, cy;

    angle = deg2rad( angles[ YAW ] );
	sy = (float) sin( angle );
	cy = (float) cos( angle );
	angle = deg2rad( angles[ PITCH ] );
	sp = (float) sin( angle );
	cp = (float) cos( angle );

	forward[0] = cp * cy;
	forward[1] = cp * sy;
	forward[2] = -sp;
}

void AngleVectors( vec3_t angles , vec3_t forward , vec3_t right , vec3_t up )
{
	float  angle;
	float sr, sp, sy, cr, cp, cy;
	
	

	angle = deg2rad( angles[ YAW ] );
	sy = (float) sin( angle );
	cy = (float) cos( angle );
	angle = deg2rad( angles[ PITCH ] );
	sp = (float) sin( angle );
	cp = (float) cos( angle );
	angle = deg2rad( angles[ ROLL ] );
	sr = (float) sin( angle );
	cr = (float) cos( angle );

	if( forward )
	{
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}
	if( right )
	{
		right[0] = ( -1 * sr * sp * cy + -1 * cr * -sy );
		right[1] = ( -1 * sr * sp * sy + -1 * cr * cy );
		right[2] = -1 * sr * cp;
	}
	if( up )
	{
		up[0] = ( cr * sp * cy + -sr * -sy );
		up[1] = ( cr * sp * sy + -sr * cy );
		up[2] = cr * cp;
	}
}

void VectorToAngles( const vec3_t value1, vec3_t angles ) 
{
	float	forward;
	float	yaw, pitch;
	
	if( value1[1] == 0.0f && value1[0] == 0.0f ) 
	{
		yaw = 0.0f;
		if ( value1[2] > 0.0f ) {
			pitch = 90.0f;
		}
		else {
			pitch = 270.0f;
		}
	}
	else 
	{
		if ( value1[0] ) 
		{
			yaw = (float)( atan2( (double)value1[1] , (double)value1[0] ) * 180.0 / __PID );
		}
		else if ( value1[1] > 0.0f )
		{
			yaw = 90.0f;
		}
		else
		{
			yaw = 270.0f;
		}
		if ( yaw < 0.0f )
		{
			yaw += 360.0f;
		}

		forward = (float)( sqrt( (double)( value1[0] * value1[0] + value1[1] * value1[1] ) ) );
		pitch = (float)( atan2( (double)value1[2] , (double)forward ) * 180.0 / __PID );
		if ( pitch < 0.0f )
		{
			pitch += 360.0f;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0.0f;
}

float fabs( float f )
{
	int tmp = * ( int * ) &f;
	tmp &= 0x7FFFFFFF;
	return * ( float * ) &tmp;
}


void ClearBounds( vec3_t mins , vec3_t maxs )
{
	mins[0] = mins[1] = mins[2] = 99999.0f;
	maxs[0] = maxs[1] = maxs[2] = -99999.0f;
}

void AddPointToBounds( vec3_t v , vec3_t mins , vec3_t maxs )
{
	

	if( v[0] < mins[0] )  mins[0] = v[0];
	if( v[0] > maxs[0] )  maxs[0] = v[0];

	if( v[1] < mins[1] )  mins[1] = v[1];
	if( v[1] > maxs[1] )  maxs[1] = v[1];

	if( v[2] < mins[2] )  mins[2] = v[2];
	if( v[2] > maxs[2] )  maxs[2] = v[2];
}


void MakeTriNormal( float* t , vec3_t fn )
{
	vec3_t v1 , v2;
	
	v1[0] = t[0] - t[3];
	v1[1] = t[1] - t[4];
	v1[2] = t[2] - t[5];

	v2[0] = t[6] - t[3];
	v2[1] = t[7] - t[4];
	v2[2] = t[8] - t[5];

	VectorCrossProduct( fn , v1 , v2 );
}

inline void ClearBounds2D( vec2_t mins , vec2_t maxs )
{
	mins[0] = mins[1] = 99999.0f;
	maxs[0] = maxs[1] = -99999.0f;
}

inline void AddPointToBounds2D( vec2_t v , vec2_t mins , vec2_t maxs )
{
	

	if( v[0] < mins[0] )  mins[0] = v[0];
	if( v[0] > maxs[0] )  maxs[0] = v[0];

	if( v[1] < mins[1] )  mins[1] = v[1];
	if( v[1] > maxs[1] )  maxs[1] = v[1];
}

void CalcSphereBoundBox( vec3_t mins , vec3_t maxs )
{
	float distMins , distMaxs , radius;

	distMins = VectorDotProduct( mins , mins );
	distMaxs = VectorDotProduct( maxs , maxs );

	if( distMins > distMaxs )
		radius = (float) sqrt( distMins );
	else
		radius = (float) sqrt( distMaxs );

	VectorSet( mins , -radius , -radius , mins[2] );
	VectorSet( maxs ,  radius ,  radius , maxs[2] );
}

void CalcZAxisRotateBoundBox( float angle , vec3_t mins , vec3_t maxs )
{
	vec2_t point2D , mins2D , maxs2D;
	float  cosVal , sinVal;

	ClearBounds2D( mins2D , maxs2D );

	cosVal = (float) cos( deg2rad( angle ) );
	sinVal = (float) sin( deg2rad( angle ) );

	point2D[0] = mins[0] * cosVal - mins[1] * sinVal;
	point2D[1] = mins[0] * sinVal + mins[1] * cosVal;

	AddPointToBounds2D( point2D , mins2D , maxs2D );

	point2D[0] = mins[0] * cosVal - maxs[1] * sinVal;
	point2D[1] = mins[0] * sinVal + maxs[1] * cosVal;

	AddPointToBounds2D( point2D , mins2D , maxs2D );

	point2D[0] = maxs[0] * cosVal - mins[1] * sinVal;
	point2D[1] = maxs[0] * sinVal + mins[1] * cosVal;

	AddPointToBounds2D( point2D , mins2D , maxs2D );

	point2D[0] = maxs[0] * cosVal - maxs[1] * sinVal;
	point2D[1] = maxs[0] * sinVal + maxs[1] * cosVal;

	AddPointToBounds2D( point2D , mins2D , maxs2D );

	VectorSet( mins , mins2D[0] , mins2D[1] , mins[2] );
	VectorSet( maxs , maxs2D[0] , maxs2D[1] , maxs[2] );
}

void CalcZAxisRotate( float angle , vec3_t mins , vec3_t maxs )
{
	float  cosVal , sinVal;
	vec3_t tmp;

	cosVal = (float) cos( deg2rad( angle ) );
	sinVal = (float) sin( deg2rad( angle ) );
	
	tmp[0] = mins[0] * cosVal - mins[1] * sinVal;
	tmp[1] = mins[0] * sinVal + mins[1] * cosVal;
	mins[0] = tmp[0];
	mins[1] = tmp[1];

	tmp[0] = maxs[0] * cosVal - maxs[1] * sinVal;
	tmp[1] = maxs[0] * sinVal + maxs[1] * cosVal;
	maxs[0] = tmp[0];
	maxs[1] = tmp[1];
}