#ifndef _GTHBSP_H
#define _GTHBSP_H

#include <windows.h>
#include "../math/math.h"

#define  GTH_SIGNATURE    "GTH BSP DATA"
#define  GTH_BSPVERSION   2.1f

#define  LIGHTMAPSIZE_WIDTH   128
#define  LIGHTMAPSIZE_HEIGHT  128
#define  LIGHTMAPSIZE    ( LIGHTMAPSIZE_WIDTH * LIGHTMAPSIZE_HEIGHT )

#define  MAX_SIZE_SHADERREF_NAME  64
enum
{
    FACETYPE_NORMAL   = 1,
    FACETYPE_PATCH    = 2,
    FACETYPE_TRISURF  = 3,
    FACETYPE_FLARE    = 4,
	FACETYPE_ENTITY   = 5
};



#define	PLANE_X			0
#define	PLANE_Y			1
#define	PLANE_Z			2
#define	PLANE_NON_AXIAL	3

#define PlaneTypeForNormal(x) (x[0] == 1.0 ? PLANE_X : (x[1] == 1.0 ? PLANE_Y : (x[2] == 1.0 ? PLANE_Z : PLANE_NON_AXIAL) ) )


typedef struct
{
	unsigned int	offset, size;
} lump_t;


enum  
{
    GTH_SHADERREFS,
    GTH_PLANES,
    GTH_NODES,
    GTH_LEAFS,
    GTH_FACELISTS,
    GTH_BRUSHELISTS,
    GTH_MODELS,
    GTH_BRUSHES,
    GTH_BRUSHSIDES,
    GTH_VERTICES,
    GTH_ELEMENTS,
    GTH_FACES,
    GTH_LIGHTMAPS,
	GTH_GRIDLIGHTS,
    GTH_VISIBILITY,
	GTH_LEAFBASEFACES,
    GTH_GRIDFACELUMPHEAD,
	GTH_GRIDFACELUMP,
	GTH_GRIDFACELISTS,
	GTH_NUM_LUMPS
};

typedef struct
{
	int baseDefault;
	int sizeDefault;
	int baseCustom;
	int sizeCustom;
	int baseSky;
	int sizeSky;
	int baseTrans;
	int sizeTrans;
	int baseDeform;
	int sizeDeform;
	int baseDefromTrans;
	int sizeDeformTrans;
} shadertypehead_t;

typedef struct
{
    char    signature[16];
	float   version;
    lump_t  lumps[ GTH_NUM_LUMPS ];
	shadertypehead_t shaderTypeHead;
	int     dummy[3];
} bspheader_t;


typedef struct
{
	char  name[ MAX_SIZE_SHADERREF_NAME ];
	int   shader;
	int   surfaceFlags;
	int   contents;
} shaderref_t;


typedef struct
{
    vec3_t   normal;
    float    dist;
	BYTE     type;
	BYTE     signBits;
	BYTE     pad[2];
} plane_t;



typedef struct
{
    int      plane; 
    int      child[2];
	
	vec3_t   mins;
	vec3_t   maxs;
} node_t;


typedef struct
{
    int      cluster;
    int      area;
    
	vec3_t   mins;
	vec3_t   maxs;
    int      firstFace, numFaces;
	int      firstBrush, numBrushes;
	
} leaf_t;


typedef int  facelist_t;
typedef int  brushlist_t;


typedef struct
{
	bboxf_t  bbox;
	
	
	int	     firstFace, numFaces;
    int      firstBrush, numBrushes;
} model_t;


typedef struct
{
	int      firstSide;
	int	     numSides;
	int	     shaderInfo;
	int      checkCount;
} brush_t;


typedef struct
{
	unsigned int	plane;
	int 	        shaderInfo;
} brushside_t;


typedef struct
{
	vec3_t   point;
	vec3_t   normal;
	color_t  diffuse;
	vec2_t   texUV;
	vec2_t   lmapUV;
} vertex_t;

typedef   int    element_t;


typedef struct
{
	int      shader;
    int      type;
    int      vertexOffset;
	int      vertexSize;
    int      elemOffset;
	int      elemSize;
    int      lmap;
    float    planeDist;
    vec3_t   planeNormal;
	vec3_t   planeRight;
	vec3_t   planeUp;
	
	vec3_t   mins;
	vec3_t   maxs;
    
	int      patchSize[2];

	int      offsetVB;
	int      sizeVB;
	int      offsetEB;
	int      sizeEB;

	DWORD    frameFlag;
} face_t;


typedef DWORD  lightmap_t[ LIGHTMAPSIZE ];

typedef struct
{
    unsigned char ambient[3]; 
    unsigned char directional[3]; 
    unsigned char direction[2];
} gridlight_t;

typedef struct
{
    int      numClusters;
    int      rowSize;
    BYTE     data[1];
} visibility_t;

typedef struct leafbaseface_s
{
    int      shader;
    int      type;
    int      lmap;
    float    texDist;
    vec3_t   normal;
	vec3_t   origin;
	
	vec3_t   mins;
	vec3_t   maxs;
    
	DWORD    frameFlag;
	int      idxOriginFace;
	leafbaseface_s  *next;
} leafbaseface_t;

typedef struct
{
	vec3_t mins , maxs;
	float gridSize;
	int   width , height;
	int   sizeList;
} gridfacelumphead_t;

typedef struct 
{
	int base;
	int sizeLumps;
} gridfacelump_t;

typedef   int    gridfacelist_t;


#endif
