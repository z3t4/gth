#ifndef _COLLIDE_H
#define _COLLIDE_H

typedef struct 
{
	int       allsolid;
	int       startsolid;
	float     fraction;
	vec3_t    endpos;
	plane_t  *plane;
	int       surfaceFlags;
	int       contents;
	int	      entityNum;
} trace_t;

#endif