#ifndef _TCMOD_H
#define _TCMOD_H

#include "../math/math.h"

typedef enum 
{
	TCMOD_NONE      = 0,
    TCMOD_ROTATE    = 1,
	TCMOD_SCALE     = 2,
	TCMOD_SCROLL    = 3,
	TCMOD_STRETCH   = 4,
	TCMOD_TRANSFORM = 5,
	TCMOD_TURB      = 6
} tcmodfunctype_t;

typedef struct 
{

    tcmodfunctype_t type;
    wavefunc_t      func;
    float           params[2];
	matrix4x4_t     mat;

} tcmodfunc_t;

#endif