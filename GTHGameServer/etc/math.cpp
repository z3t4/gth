#include "../global.h"

void Normalize( float* n )
{
	float length;

	length = n[0]*n[0] + n[1]*n[1] + n[2]*n[2];
	length = (float)sqrt(length);
	if (length == 0.0f) length = 0.000001f;	
	n[0] /= length;
	n[1] /= length;	
	n[2] /= length;	
}

void RotateXCoord(vec3_t A, float Angle)
{
	float c,s;
	float tempY, tempZ;


	s=sin(DEG2RAD(Angle));
	c=cos(DEG2RAD(Angle));

	tempY = A[1];
	tempZ = A[2];
	A[1] = tempY * c - tempZ * s;
	A[2] = tempY * s + tempZ * c;
}

void RotateYCoord(vec3_t A, float Angle)
{
	float c,s;
	float tempX, tempZ;


	s=sin(DEG2RAD(Angle));
	c=cos(DEG2RAD(Angle));

	tempX = A[0];
	tempZ = A[2];
	A[0] = tempX * c + tempZ * s;
	A[2] = tempZ * c - tempX * s;
}

void RotateZCoord(vec3_t A, float Angle)
{
	float c,s;
	float tempX, tempY;


	s=sin(DEG2RAD(Angle));
	c=cos(DEG2RAD(Angle));

	tempX = A[0];
	tempY = A[1];
	A[0] = tempX * c - tempY * s;
	A[1] = tempX * s + tempY * c;
}


void vectoangles( const vec3_t value1, vec3_t angles ) 
{
	float	forward;
	float	yaw, pitch;
	
	if ( value1[1] == 0 && value1[0] == 0 ) 
	{
		yaw = 0;
		if ( value1[2] > 0 ) 
		{
			pitch = 90;
		}
		else 
		{
			pitch = 270;
		}
	}
	else {
		if ( value1[0] ) 
		{
			yaw = ( atan2 ( value1[1], value1[0] ) * 180 / M_PI );
		}
		else if ( value1[1] > 0 ) 
		{
			yaw = 90;
		}
		else 
		{
			yaw = 270;
		}

		if ( yaw < 0 ) {
			yaw += 360;
		}

		forward = sqrt ( value1[0]*value1[0] + value1[1]*value1[1] );
		pitch = ( atan2(value1[2], forward) * 180 / M_PI );
		if ( pitch < 0 ) 
		{
			pitch += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}

int GTH_Randomize( int maxGen )
{
	double r;
	

	r = genrand_real3();
	return( (int)( r * (double)maxGen ) );
}

int GTH_RandomizePower( int minValue, int maxValue )
{
	float r;
	
	r = genrand_real3();
	r = 1.0f - sqrtf( r );
	r = r * ( (float)maxValue - (float)minValue + 1.0f ) + (float)minValue;

	return( (int)r );
}

float GTH_Random()
{
	return( (float)genrand_real1() );
}
