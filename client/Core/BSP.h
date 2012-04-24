



#if !defined(AFX_BSP_H__12B14CD1_433E_4248_A0AE_84CB4822E7A5__INCLUDED_)
#define AFX_BSP_H__12B14CD1_433E_4248_A0AE_84CB4822E7A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "gth_bsp.h"
#include "frustum.h"
#include "shadermng.h"
#include "d3dbuffer.h"
#include "collide.h"
#include "decal.h"

enum
{
    SKYBOX_TOP    = 0,
    SKYBOX_FRONT,
    SKYBOX_RIGHT,
    SKYBOX_BACK,
    SKYBOX_LEFT,
	SKYBOX_DOWN
};

typedef struct
{
	int  bank;
	int  baseVB;
	int  sizeVB;
	leafbaseface_t *linked;
} shaderLMmap_t;

#define MAX_SHADERMAP_LM        256

typedef struct
{
	DWORD  flags;
	int    shader;    
	DWORD  frameFlag;
	DWORD  LMFrameFlags[ MAX_SHADERMAP_LM ];
	shaderLMmap_t LMMaps[ MAX_SHADERMAP_LM ];
} shadermap_t;

typedef struct
{
	vec3_t point;
	vec3_t normal;
	vec2_t texUV;
} shadowvertex_t;

#define GTH_BSP_FORMAT       ( GTH_FORMAT_XYZ | GTH_FORMAT_NORMAL | GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX( 2 ) )
#define GTH_SHADOW_FORMAT    ( GTH_FORMAT_XYZ | GTH_FORMAT_NORMAL | GTH_FORMAT_TEXVERTEX( 1 ) )

#define		GTH_SHADOW_FORMAT_NEO	( GTH_FORMAT_XYZ | GTH_FORMAT_TEXVERTEX( 1 ) )

#define MAX_SIZE_LIGHTMAP_TEXTURE_REFERENCE  256
#define MAX_SIZE_STATIC_VERTEX_BUFFER_BANK   16
#define MAX_SIZE_DYNAMIC_VERTEX_BUFFER       1024 * 10
#define MAX_SIZE_DYNAMIC_INDEX_BUFFER        1024 * 64
#define MAX_SIZE_TEMP_VERTEX_BUFFER          1024 * 10

#define MAX_SIZE_DYNAMIC_SHADOW_VERTEX_BUFFER  1024 * 4

#define MAX_BANKSIZE_VERTEXBUFFER            1024 * 64

#define GTH_WORLD_VISIBLE_QUALITY_LOW   1
#define GTH_WORLD_VISIBLE_QUALITY_HIGH  0

#define MAX_SIZE_FILENAME               128

class CBSP  
{
public:

	int      m_id;
	char     m_name[GTH_MAX_SIZE_FILENAME];
	vec3_t   m_camPos;
	vec3_t   m_camDir;
	float    m_skyHeight;
	int      m_totalRenderPolygon;
	
	shadermap_t *m_shaderMaps;
	int          m_lmapTexRefs[ MAX_SIZE_LIGHTMAP_TEXTURE_REFERENCE ];

	CFXDecal   m_decal;

	


	
	int m_numBank;
	int m_numDeformVerts;
	int m_numStaticElems;
	int             m_bankSize[ MAX_SIZE_STATIC_VERTEX_BUFFER_BANK ];
	CD3DBuffer     *m_staticVB[ MAX_SIZE_STATIC_VERTEX_BUFFER_BANK ];
	CD3DBuffer     *m_dynamicVB;
	CD3DBuffer     *m_dynamicIB;
	CD3DBuffer     *m_shadowVB;
	WORD           *m_staticEB;
	vertex_t       *m_deformVB;
	vertex_t       *m_tempVB;

	
	DWORD    m_frameFlag;
	float    m_visibleDist;
	float    m_removeDist;
	int      m_visibleQuality;
	BYTE    *m_visData;

	
	int m_maxPatchLevel;
	int m_subDivTolerance;

	
	DWORD m_format;
	DWORD m_shadowFormat;
	int   m_currBank;

	
	BYTE    *m_buffer;

	shadertypehead_t *m_shaderType;
	shaderref_t      *m_shaderRefs;
	int               m_numShaderRefs;
	plane_t          *m_planes;
	int               m_numPlanes;
	node_t           *m_nodes;
	int               m_numNodes;
	leaf_t           *m_leaves;
	int               m_numLeaves;
	facelist_t       *m_faceLists;
	int               m_numFaceLists;
	brushlist_t      *m_brushLists;
	int               m_numBrushLists;
	model_t          *m_models;
	int               m_numModels;
    brush_t          *m_brushes;
	int               m_numBrushes;
	brushside_t      *m_brushSides;
	int               m_numBrushSides;
	vertex_t         *m_vertices;
	int               m_numVertices;
	element_t        *m_elems;
	int               m_numElems;
	face_t           *m_faces;
	int               m_numFaces;
	lightmap_t       *m_lightmaps;
	int               m_numLightmaps;
	gridlight_t      *m_gridLights;
	int               m_numGridLights;
	visibility_t     *m_vis;
	leafbaseface_t   *m_leafBaseFaces;

	gridfacelumphead_t *m_gridFaceLumpHead;
	gridfacelump_t     *m_gridFaceLumps;
	int                 m_numGridFaceLumps;
	gridfacelist_t     *m_gridFaceLists;
	int                 m_numGridFaceLists;
	

	
	DWORD    m_shadowFrameFlag;

	
	CBSP();
	~CBSP();
	void  Release();

	int   Load( char *mapName );
	int   GenDynamicMem();
	
	void  SetID( int id ) { m_id = id; }
	
	void  SetFrameFlag( DWORD frameFlag )           { m_frameFlag = frameFlag; }
	void  SetvisibleQuality( int quality )          { m_visibleQuality = quality; }

	BYTE *GetVisData() { return m_visData; }

	int   PrepareShaderMap();
	int   Initialize( float visibleDist );
	int   Restore();
	void  PrepareVertexBuffer();
	int   UploadVertexBuffer();

	
	void  PrepareScene( vec3_t pos , DWORD frameFlag );
	void  MakeVis();
	int   FindCluster( vec3_t pos );
	int   FindLeaf( vec3_t pos );
	void  TravelModel( int modelIdx );
	inline int  ChechVisibleDistance( const vec3_t mins , const vec3_t maxs );
	inline void TravelFace( leafbaseface_t *face );

	
	void RenderBefore();
	void RenderAfter();
	inline void RenderDefault( const shaderLMmap_t *LMMap );
	inline void RenderCustom( shader_t *shader , const shaderLMmap_t *LMMap , const int idxLM );
	inline void RenderDeform( shader_t *shader , const shaderLMmap_t *LMMap , const int idxLM );
	inline void RenderSky( shader_t *shader , const shaderLMmap_t *LMMap , const int idxLM );

	
	trace_t  Trace( vec3_t start , vec3_t mins , vec3_t maxs , vec3_t end , int brushmask );
	void     RecursiveHullCheck( int num , float p1f , float p2f , vec3_t p1 , vec3_t p2 );
	void     TraceToLeaf( int leafnum );
	void     ClipBoxToBrush( vec3_t mins , vec3_t maxs , vec3_t p1 , vec3_t p2 
						 , trace_t *trace , brush_t *brush );
	int      PointContents( vec3_t pos );

	
	void SetShadowRenderState();
	void RenderProjectiveShadow( vec3_t p1 , vec3_t p2 , vec3_t p3 , vec3_t p4 , LPDIRECT3DTEXTURE8 d3dTex );
	 
	
		
	
	bool RenderProjectiveShadowNeo( float in_size, vec3_t p1, CD3DBuffer* pBuffer, int& vertexSize);
	void SetDecalBuffer();
	void SetDecalInit(matrix4x4_t* matShadow, vec3_t& vLight, vec3_t& sunAngle, float dispPS, float min, float max);

private:

	
	int  CalcPatchSize( face_t *face , int *numVerts ,  int *numElems );
	void GenPatch( face_t *face , int baseVB , vertex_t *ptrStaticVB , WORD *ptrStaticEB );
	void FillBezierCurve( int , unsigned int , unsigned int , unsigned int , vertex_t* );
	int  FindPatchLevel( vec3_t *v );
	int  FindPatchSize( int *patchSize, vertex_t *cp, int *size );

	
	void CalcSkySize( int shaderFlag , int *numVerts ,  int *numElems );
	void GenSky( face_t *face , int baseVB , int shaderFlag , vertex_t *ptrStaticVB , WORD *ptrStaticEB );
	void GenEnvSkyBox( vertex_t *ptrVB , WORD *ptrEB , int baseVB );
	void GenSkyBox( vertex_t *ptrVB );
	void GenSkyBoxSide( int side, vec3_t orig, vec3_t drow, vec3_t dcol, vertex_t * ptrVB );
	 
	void	GenSkyBox_Neo	(	vertex_t	*in_vb	);
	void	GenSkyVertices	(	vec3_t	in_forward,	vec3_t	in_up,	vec3_t	in_right,	int	in_numTess,	float	in_skySize,	vertex_t	*in_vb	);
	void	GenSkyTexCoords	(	float	in_skyHeight,	int	in_numVtx,	vertex_t	*in_vb	);

	
	void GenSkyBox_Lee	(	vertex_t	*in_vb	);
};

#endif 
