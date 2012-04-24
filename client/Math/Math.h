#ifndef _MATH_H
#define _MATH_H

#include "vector.h"
#include "matrix.h"


typedef unsigned char color_t[4];  
typedef int   bbox_t[6];    
typedef float bboxf_t[6];   


#define	PITCH	0


#define	YAW		1


#define	ROLL	2

void  AngleToVector( vec3_t angles , vec3_t forward );
void  AngleVectors( vec3_t angles , vec3_t forward , vec3_t right , vec3_t up );
void  VectorToAngles( const vec3_t value1, vec3_t angles );
float fabs( float f );
void  ClearBounds( vec3_t mins , vec3_t maxs );
void  AddPointToBounds( vec3_t v , vec3_t mins , vec3_t maxs );
void  MakeTriNormal( float* t , vec3_t fn );
inline void ClearBounds2D( vec2_t mins , vec2_t maxs );
inline void AddPointToBounds2D( vec2_t v , vec2_t mins , vec2_t maxs );
void CalcSphereBoundBox( vec3_t mins , vec3_t maxs );
void CalcZAxisRotateBoundBox( float angle , vec3_t mins , vec3_t maxs );
void CalcZAxisRotate( float angle , vec3_t mins , vec3_t maxs );

#endif