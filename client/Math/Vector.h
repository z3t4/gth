#ifndef _VECTOR_H
#define _VECTOR_H

typedef float   vec2_t[2];
typedef float   vec3_t[3];
typedef float   vec4_t[4];

#define VectorDotProduct(x,y)   ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(c,a,b)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(c,a,b)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(b,a)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(o, v, s)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(o, v, b, s)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))

#define VectorClear(a)			((a)[0]=(a)[1]=(a)[2]=0)
#define VectorNegate(b,a)		((b)[0]=-(a)[0],(b)[1]=-(a)[1],(b)[2]=-(a)[2])
#define VectorSet(v, x, y, z)	((v)[0]=(x), (v)[1]=(y), (v)[2]=(z))
#define Vector4Copy(b,a)		((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])

#define	SnapVector(v) {v[0]=(int)v[0];v[1]=(int)v[1];v[2]=(int)v[2];}


#define VectorAverage(c,a,b)  ((c)[0]=((a)[0]+(b)[0])*0.5f,(c)[1]=((a)[1]+(b)[1])*0.5f,(c)[2]=((a)[2]+(b)[2])*0.5f)
#define VectorInterpolate(o,a,b,s)  ((o)[0]=(a)[0]+((b)[0]-(a)[0])*s,(o)[1]=(a)[1]+((b)[1]-(a)[1])*s,(o)[2]=(a)[2]+((b)[2]-(a)[2])*s)

#define Vector2Copy(b,a)      ((b)[0]=(a)[0],(b)[1]=(a)[1])
#define Vector2Average(c,a,b) ((c)[0]=((a)[0]+(b)[0])*0.5f,(c)[1]=((a)[1]+(b)[1])*0.5f)
#define Vector2Clear(a)       ((a)[0]=(a)[1]=0)
#define Vector2Set(v, x, y)   ((v)[0]=(x), (v)[1]=(y))
#define Vector2Add(c,a,b)     ((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1])

#define ColorCopy(b,a)        ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])
#define ColorAverage(c,a,b)   ((c)[0]=((int)(a)[0]+(int)(b)[0])/2,(c)[1]=((int)(a)[1]+(int)(b)[1])/2,\
                               (c)[2]=((int)(a)[2]+(int)(b)[2])/2,(c)[3]=((int)(a)[3]+(int)(b)[3])/2)
#define ColorClear(a)         ((a)[0]=(a)[1]=(a)[2]=(a)[3]=0)
#define ColorSet(v,a,b,c,d)   ((v)[0]=(a),(v)[1]=(b),(v)[2]=(c),(v)[3]=(d))

#define Vector4Set(v,a,b,c,d)   ((v)[0]=(a),(v)[1]=(b),(v)[2]=(c),(v)[3]=(d))
#define MAKE_COLOR_ARGB(a,r,g,b)  ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))


#define VectorCopyFromD3DVector(a, b) { a[0]=b.x; a[1]=b.y; a[2]=b.z; }
#define VectorCopyToD3DVector(a, b)   { a.x=b[0]; a.y=b[1]; a.z=b[2]; }
#define D3DVectorClear(a)             { a.x=0; a.y=0; a.z=0; }
#define D3DVectorSet(v, a, b, c)	  { v.x=a; v.y=b; v.z=c; }


int   VectorCompare( const vec3_t v1, const vec3_t v2 );
float VectorLength( const vec3_t v );
float VectorDistance( const vec3_t p1, const vec3_t p2 );
float VectorDistanceSquared( const vec3_t p1, const vec3_t p2 );
void  VectorCrossProduct( vec3_t cross , const vec3_t v1, const vec3_t v2 );
float VectorNormalize( vec3_t v );		
void  VectorNormalizeFast( vec3_t v) ;		
float VectorNormalize2( vec3_t out , const vec3_t v );
void  VectorInverse( vec3_t v );
void  Vector4Scale( vec4_t out , const vec4_t in, float scale );
void  VectorRotate( vec3_t out , vec3_t in, vec3_t matrix[3] );
void  VectorProjectOnPlane( vec3_t dest , vec3_t point , vec3_t planeRight , vec3_t planeUp );

#endif