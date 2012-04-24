#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "gth_bsp.h"

#define FRUSTUM_NEAR_PLAIN     0
#define FRUSTUM_LEFT_PLAIN     1
#define FRUSTUM_RIGHT_PLAIN    2
#define FRUSTUM_TOP_PLAIN      3
#define FRUSTUM_BOTTOM_PLAIN   4

typedef struct 
{
    int       X;
    int       Y;
    int       Width;
    int       Height;
    float     MinZ;
    float     MaxZ;
} viewport_t;

typedef struct
{
	float fov;
	float aspect;
	float znear;
	float zfar;
	viewport_t viewport;
} projectionparam_t;

class CFrustum
{
public:
	CFrustum() {};
	~CFrustum() {};
    plane_t planes[5];

	void  Setup( vec3_t angles , vec3_t position , projectionparam_t *projectParm );
    int   Cull( vec3_t mins , vec3_t maxs );
	int   Cull_Exact( vec3_t mins , vec3_t maxs );
};

#endif