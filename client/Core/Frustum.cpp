#include "frustum.h"

void SetPlaneSignbits( plane_t *out ) 
{
	int	bits, j;

	
	bits = 0;
	for( j = 0 ; j < 3 ; j++ ) 
	{
		if( out->normal[j] < 0 ) 
		{
			bits |= 1<<j;
		}
	}
	out->signBits = bits;
}


#if !(defined __linux__ && defined __i386__ && !defined C_ONLY)
#if defined __LCC__ || defined C_ONLY || !id386

int BoxOnPlaneSide( vec3_t emins , vec3_t emaxs , plane_t *p )
{
	float	dist1, dist2;
	int		sides;


	if( p->type < 3 )
	{
		if( p->dist <= emins[p->type] )
			return 1;
		if( p->dist >= emaxs[p->type] )
			return 2;
		return 3;
	}


	switch( p->signBits )
	{
	case 0:
		dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		dist2 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		break;
	case 1:
		dist1 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		break;
	case 2:
		dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		dist2 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		break;
	case 3:
		dist1 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		break;
	case 4:
		dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		dist2 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		break;
	case 5:
		dist1 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		break;
	case 6:
		dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		dist2 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		break;
	case 7:
		dist1 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		break;
	default:
		dist1 = dist2 = 0;		
		break;
	}

	sides = 0;
	if( dist1 >= p->dist )  
		sides = 1;
	if( dist2 < p->dist )   
		sides |= 2;

	return sides;

	
	
	
	
	
	
	
	
	
	
	
	
	
}
#else
#pragma warning( disable: 4035 )

#define dword unsigned int

__declspec( naked ) int BoxOnPlaneSide( vec3_t emins, vec3_t emaxs, plane_t *p)
{
	static int bops_initialized;
	static int Ljmptab[8];

	__asm {

		push ebx
			
		cmp bops_initialized, 1
		je  initialized
		mov bops_initialized, 1
		
		mov Ljmptab[0*4], offset Lcase0
		mov Ljmptab[1*4], offset Lcase1
		mov Ljmptab[2*4], offset Lcase2
		mov Ljmptab[3*4], offset Lcase3
		mov Ljmptab[4*4], offset Lcase4
		mov Ljmptab[5*4], offset Lcase5
		mov Ljmptab[6*4], offset Lcase6
		mov Ljmptab[7*4], offset Lcase7
			
initialized:

		mov edx,dword ptr[4+12+esp]
		mov ecx,dword ptr[4+4+esp]
		xor eax,eax
		mov ebx,dword ptr[4+8+esp]
		mov al,byte ptr[17+edx]
		cmp al,8
		jge Lerror
		fld dword ptr[0+edx]
		fld st(0)
		jmp dword ptr[Ljmptab+eax*4]
Lcase0:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase1:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase2:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase3:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase4:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase5:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase6:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase7:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
LSetSides:
		faddp st(2),st(0)
		fcomp dword ptr[12+edx]
		xor ecx,ecx
		fnstsw ax
		fcomp dword ptr[12+edx]
		and ah,1
		xor ah,1
		add cl,ah
		fnstsw ax
		and ah,1
		add ah,ah
		add cl,ah
		pop ebx
		mov eax,ecx
		ret
Lerror:
		int 3
	}
}
#pragma warning( default: 4035 )

#endif
#endif


#define BOX_ON_PLANE_SIDE(emins, emaxs, p)	\
	(((p)->type < 3)?						\
	(										\
		((p)->dist <= (emins)[(p)->type])?	\
			1								\
		:									\
		(									\
			((p)->dist >= (emaxs)[(p)->type])?\
				2							\
			:								\
				3							\
		)									\
	)										\
	:										\
		BoxOnPlaneSide( (emins), (emaxs), (p)))

void CFrustum::Setup( vec3_t angles , vec3_t position , projectionparam_t *projectParm )
{
    vec3_t rotate , normal , znear;
	float  halfFov;

	
    rotate[ YAW ]   = angles[ YAW ];
	rotate[ PITCH ] = angles[ PITCH ];
	AngleToVector( rotate , normal );
	VectorScale( znear , normal , projectParm->znear );
	VectorAdd( znear , znear , position );
	
	VectorCopy( planes[ FRUSTUM_NEAR_PLAIN ].normal , normal );
	planes[ FRUSTUM_NEAR_PLAIN ].dist = VectorDotProduct( normal , znear );
	planes[ FRUSTUM_NEAR_PLAIN ].type = PlaneTypeForNormal( normal );
	SetPlaneSignbits( &planes[ FRUSTUM_NEAR_PLAIN ] );


	
	halfFov = projectParm->fov / 2.0f / projectParm->aspect;


	
	rotate[ YAW ]   = angles[ YAW ];
	rotate[ PITCH ] = angles[ PITCH ] - ( 90.0f - halfFov );
	AngleToVector( rotate , normal );
	
	VectorCopy( planes[ FRUSTUM_TOP_PLAIN ].normal , normal );
	planes[ FRUSTUM_TOP_PLAIN ].dist = VectorDotProduct( normal , position );
	planes[ FRUSTUM_TOP_PLAIN ].type = PlaneTypeForNormal( normal );
	SetPlaneSignbits( &planes[ FRUSTUM_TOP_PLAIN ] );

	
	rotate[ YAW ]   = angles[ YAW ];
	rotate[ PITCH ] = angles[ PITCH ] + ( 90.0f - halfFov );
	AngleToVector( rotate , normal );
	
	VectorCopy( planes[ FRUSTUM_BOTTOM_PLAIN ].normal , normal );
	planes[ FRUSTUM_BOTTOM_PLAIN ].dist = VectorDotProduct( normal , position );
	planes[ FRUSTUM_BOTTOM_PLAIN ].type = PlaneTypeForNormal( normal );
	SetPlaneSignbits( &planes[ FRUSTUM_BOTTOM_PLAIN ] );


	
	halfFov = projectParm->fov / 2.0f;


	
	rotate[ YAW ]   = angles[ YAW ] + ( 90.0f - halfFov );
	rotate[ PITCH ] = angles[ PITCH ];
	AngleToVector( rotate , normal );
	
	VectorCopy( planes[ FRUSTUM_LEFT_PLAIN ].normal , normal );
	planes[ FRUSTUM_LEFT_PLAIN ].dist = VectorDotProduct( normal , position );
	planes[ FRUSTUM_LEFT_PLAIN ].type = PlaneTypeForNormal( normal );
	SetPlaneSignbits( &planes[ FRUSTUM_LEFT_PLAIN ] );

	
	rotate[ YAW ]   = angles[ YAW ] - ( 90.0f - halfFov );
	rotate[ PITCH ] = angles[ PITCH ];
	AngleToVector( rotate , normal );
	
	VectorCopy( planes[ FRUSTUM_RIGHT_PLAIN ].normal , normal );
	planes[ FRUSTUM_RIGHT_PLAIN ].dist = VectorDotProduct( normal , position );
	planes[ FRUSTUM_RIGHT_PLAIN ].type = PlaneTypeForNormal( normal );
	SetPlaneSignbits( &planes[ FRUSTUM_RIGHT_PLAIN ] );
}

int CFrustum::Cull( vec3_t mins , vec3_t maxs )
{
	int side;

	side = BOX_ON_PLANE_SIDE( mins , maxs , &planes[ FRUSTUM_NEAR_PLAIN ] );
	if( side == 2 ) return true;
	if( side != 1 ) return false;

	side = BOX_ON_PLANE_SIDE( mins , maxs , &planes[ FRUSTUM_LEFT_PLAIN ] );
	if( side == 2 ) return true;
	if( side != 1 ) return false;

	side = BOX_ON_PLANE_SIDE( mins , maxs , &planes[ FRUSTUM_RIGHT_PLAIN ] );
	if( side == 2 ) return true;
	if( side != 1 ) return false;

	side = BOX_ON_PLANE_SIDE( mins , maxs , &planes[ FRUSTUM_TOP_PLAIN ] );
	if( side == 2 ) return true;
	if( side != 1 ) return false;

	side = BOX_ON_PLANE_SIDE( mins , maxs , &planes[ FRUSTUM_BOTTOM_PLAIN ] );
	if( side == 2 ) return true;
	if( side != 1 ) return false;

    return false;
}

int CFrustum::Cull_Exact( vec3_t mins , vec3_t maxs )
{
	int i;
	float* p;
	float XxMinX , YxMinY , ZxMinZ , XxMaxX , YxMaxY , ZxMaxZ , dist;

    for( i = 0 ; i < 5 ; ++i ) 
	{
		p = planes[i].normal;

		XxMinX = p[0] * mins[0];   XxMaxX = p[0] * maxs[0];
		YxMinY = p[1] * mins[1];   YxMaxY = p[1] * maxs[1];
		ZxMinZ = p[2] * mins[2];   ZxMaxZ = p[2] * maxs[2];
		dist = planes[i].dist;
   
        if( XxMinX + YxMinY + ZxMinZ - dist > 0 ) continue;
        if( XxMaxX + YxMinY + ZxMinZ - dist > 0 ) continue;
        if( XxMinX + YxMaxY + ZxMinZ - dist > 0 ) continue;
        if( XxMaxX + YxMaxY + ZxMinZ - dist > 0 ) continue;
        if( XxMinX + YxMinY + ZxMaxZ - dist > 0 ) continue;
        if( XxMaxX + YxMinY + ZxMaxZ - dist > 0 ) continue;
        if( XxMinX + YxMaxY + ZxMaxZ - dist > 0 ) continue;
        if( XxMaxX + YxMaxY + ZxMaxZ - dist > 0 ) continue;

        return true;
    }

    return false;
}



