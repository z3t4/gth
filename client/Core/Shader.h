#ifndef _SHADER_H
#define _SHADER_H

#include <windows.h>
#include "core_def.h"
#include "wavefunc.h"
#include "tcmod.h"

#define MAX_SHADER_TCMOD        6
#define MAX_SHADERPASS          6
#define MAX_SHADER_ANIM_FRAMES  10
#define MAX_SIZE_MAP_NAME       128


typedef enum
{
	SHADER_SKY                     = 1 << 0,
	SHADER_SKY_ENVIRON             = 1 << 1,
	SHADER_CULL                    = 1 << 2,
	SHADER_CULL_NONE               = 1 << 3,
	SHADER_CULL_BACK               = 1 << 4,
	SHADER_CULL_FRONT              = 1 << 5,
	SHADER_DEFORMVERTEXES          = 1 << 6,
	SHADER_DEFORMVERT_AUTOSPRITE   = 1 << 7,
	SHADER_DEFORMVERT_AUTOSPRITE2  = 1 << 8,
	SHADER_SORT                    = 1 << 9,
	SHADER_SORT_PORTAL             = 1 << 10,
	SHADER_SORT_SKY                = 1 << 11,
	SHADER_SORT_OPAQUE             = 1 << 12,
	SHADER_SORT_BANNER             = 1 << 13,
	SHADER_SORT_UNDERWATER         = 1 << 14,
	SHADER_SORT_ADDITIVE           = 1 << 15,
	SHADER_SORT_NEAREST            = 1 << 16,
	SHADER_SURF                    = 1 << 17,
	SHADER_SURF_TRANS              = 1 << 18,
	SHADER_SURF_SKY                = 1 << 19,
	SHADER_SURF_NODRAW             = 1 << 20,
	SHADER_SURF_NOLIGHTMAP         = 1 << 21,
	SHADER_NOMIPMAPS               = 1 << 22,
	SHADER_TCGEN                   = 1 << 23
} SHADER_FLAG;

typedef enum
{
	SHADERPASS_RGBGEN_WAVE      = 1 << 0,
    SHADERPASS_RGBGEN_IDENTITY  = 1 << 1,
    SHADERPASS_RGBGEN_VERTEX    = 1 << 2
} SHADERPASS_RGBGEN_FLAG;

typedef enum
{
	SHADERPASS_TCGEN_BASE         = 1 << 0,
    SHADERPASS_TCGEN_LIGHTMAP     = 1 << 1,
    SHADERPASS_TCGEN_ENVIRONMENT  = 1 << 2
} SHADERPASS_TCGEN_FLAG;


typedef enum
{
	SHADERPASS_MAP             = 1 << 0,
	SHADERPASS_MAP_LIGHTMAP    = 1 << 1,
	SHADERPASS_MAP_WHITEIMAGE  = 1 << 2,
	SHADERPASS_MAP_ANIMMAP     = 1 << 3,
	SHADERPASS_CLAMPMAP        = 1 << 4,
    SHADERPASS_BLEND           = 1 << 5,
    SHADERPASS_RGBGEN          = 1 << 6,
	SHADERPASS_ALPHAGEN        = 1 << 7,
    SHADERPASS_TCGEN           = 1 << 8,
	SHADERPASS_DEPTHFUNC       = 1 << 9,
	SHADERPASS_ALPHAFUNC	   = 1 << 10,
	SHADERPASS_TCMOD           = 1 << 11,
	SHADERPASS_DEPTHWRITE      = 1 << 12,
	SHADERPASS_NONDEPTHWRITE   = 1 << 13
} SHADER_PASS_FLAG;


typedef struct
{
	int func;
	int ref;
} alphafunc_t;

typedef struct
{
	int src;
	int dest;
} blendfunc_t;

typedef struct
{
	SHADERPASS_RGBGEN_FLAG        type;
	wavefunc_t func;
	DWORD      color;
} rgbgen_t;

typedef struct
{
	int         numTcModFuncs;
    tcmodfunc_t tcModFuncs[MAX_SHADER_TCMOD];
	matrix4x4_t tcMatrix;
} tcmod_t;

typedef struct 
{
	int GetFrame(float time) 
	{
		int i = (int)( time * fps ) % numFrames;
		return frames[i];
	}

	float       fps;            
    int         numFrames;
	char        maps[MAX_SHADER_ANIM_FRAMES][GTH_MAX_SIZE_FILENAME];
    int         frames[MAX_SHADER_ANIM_FRAMES];
} anim_t;


typedef struct
{
	char                   map[ GTH_MAX_SIZE_FILENAME ];
    DWORD                  flags;
    int                    texRef;
	blendfunc_t            blend;
    alphafunc_t            alphaFunc;
	DWORD                  depthFunc;
    rgbgen_t               rgbGen;
	rgbgen_t               alphaGen;
    SHADERPASS_TCGEN_FLAG  tcGen;
	tcmod_t                tcMod;
	anim_t                *animMap;
} shaderpass_t;

typedef struct
{
	float      div;
	wavefunc_t func;
} deform_t;

#define SIZE_ENVIRONMENT_SIDE 6
typedef struct
{
	int   texRef[ SIZE_ENVIRONMENT_SIDE ];
	char  map[ SIZE_ENVIRONMENT_SIDE ][ GTH_MAX_SIZE_FILENAME ];
} envsky_t;

#define  MAX_SIZE_SHADER_NAME  64
typedef struct
{
	char         name[ MAX_SIZE_SHADER_NAME ];
	int          shaderRefIdx;
    DWORD        flags;
    int          numPasses;
    shaderpass_t passes[ MAX_SHADERPASS ];
    float        skyheight;
	deform_t     deform;
	int          inited;
	matrix4x4_t  tcGenMatrix;
	envsky_t    *envSky;
	DWORD        frameFlag;
} shader_t;


#endif


