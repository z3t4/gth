#ifndef _CORE_DEF_H
#define _CORE_DEF_H

#include "../math/math.h"

typedef enum 
{
    POINT_LIGHT       = 1,
    SPOT_LIGHT        = 2,
    DIRECTIONAL_LIGHT = 3
} LIGHTTYPE;

typedef struct
{
	LIGHTTYPE type;
    vec4_t    diffuse;
    vec4_t    specular;
    vec4_t    ambient;
    vec3_t    position;
    vec3_t    direction;
    float     range;
    float     falloff;
    float     attenuation0;
    float     attenuation1;
    float     attenuation2;
    float     theta;
    float     phi;
} light_t;

typedef struct 
{
    vec4_t   diffuse;
    vec4_t   ambient;
    vec4_t   specular;
    vec4_t   emissive;
    float    power;
} material_t;

#define GTH_MAX_SIZE_FILENAME 128
#define GTH_MAX_SIZE_TOKEN    1024

 
#define	GTH_SAFE_FREE( p )			{ if( p ) { free ( (p) ); (p) = NULL; } }
#define GTH_SAFE_DELETE_ARRAY( p )	{ if( p ) { delete [] (p); (p) = NULL; } }

#define GTH_SAFE_DELETE( a )		{ if( a ) { delete a;  a = NULL; } }
#define GTH_SAFE_RELEASE( p )		{ if( p ) { (p)->Release(); (p) = NULL; } }

enum
{
	ZIP_VERSION_PACK,
	I3D_VERSION_PACK
};

enum 
{
	OWNER_TYPE_BSP          = 1 << 0,
	OWNER_TYPE_MODEL        = 1 << 1,
	OWNER_TYPE_CHARACTER    = 1 << 2,
	OWNER_TYPE_INTERFACE    = 1 << 3,
	OWNER_TYPE_BSP_LIGHTMAP = 1 << 4
};  


typedef enum 
{
	GTH_ERROR_NOT_EXIST_FILE = 0 ,
	GTH_ERROR_NOT_ENOUGH_MEMORY ,
	GTH_ERROR_OVERFLOW_SIZE ,
	GTH_ERROR_PRINT ,
	GTH_ERROR_LOAD_FAILURE , 
	GTH_ERROR_CREATE_VERTEX_BUFFER
} GTH_ERROR_TYPE;


#ifndef _INC_STDARG

typedef char * va_list;
#define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap) ( ap = (va_list)0 )

#endif

void Error( GTH_ERROR_TYPE error , char *str );	
void ErrorLog( char* str, ... );

#ifdef _DEBUG
#define GTH_ERROR( a , b )  Error( a , b )
#else
#define GTH_ERROR( a , b )
#endif
#endif