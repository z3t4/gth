



#if !defined(AFX_MDL_H__DAB6496D_0AF2_4D88_B853_78A981B4994F__INCLUDED_)
#define AFX_MDL_H__DAB6496D_0AF2_4D88_B853_78A981B4994F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

enum
{
	GTH_MODEL_BONEFRAME = 0 ,
	GTH_MODEL_TAG ,
	GTH_MODEL_MESH ,
	GTH_MODEL_SHADERREFS ,
	GTH_MODEL_VERTICES ,
	GTH_MODEL_ELEMENTS ,
	GTH_MODEL_BOUND_VERTICES ,	 
	GTH_MODEL_BOUND_ELEMENTS ,	 
	GTH_MODEL_BOUND_PLANES ,	 
	GTH_MODEL_BOUND_MESH ,		 
	GTH_MODEL_NUM_LUMPS
};

typedef struct
{
	unsigned int	offset, size;
} mdllump_t;

typedef struct
{
	char    id[4];
    float   version;
    char    name[ 64 ];
	vec3_t	mins;
	vec3_t	maxs;
	mdllump_t  lumps[ GTH_MODEL_NUM_LUMPS ];
	char    dummy[12];
	int		hasbou12ndmesh;
} mdlhead_t;

typedef struct
{
    vec3_t mins;
	vec3_t maxs;
    vec3_t position;
    float  scale;
} mdlboneframe_t;

typedef struct
{
    char name[64];
	
    vec3_t origin;
	vec3_t axis[3];
} mdltag3x3_t;

typedef struct
{
	vec3_t   point;
	vec3_t   normal;
	vec2_t   texUV;
} mdlvertex_t;

typedef int element_t;

typedef struct
{
	char  name[ 68 ];
	int   shader;
	int   surfaceFlags;
	int   isShader;
} mdlshaderref_t;

typedef struct
{
	char   name[ 52 ] ; 

	
	int    needTCGen;
	int    offsetTCGenVB;
	int    offsetTCGenEB;
	

	int    numVertices;
	int    vertexOffset;
	int    vertexSize;
	int    numElems;
    int    elemOffset;
	int    elemSize;
	int    skinInfo;
    int    numMeshFrames;
	
	int    offsetVB;
	int    sizeVB;
	int    offsetEB;
	int    sizeEB;
} mdlmesh_t;

 
typedef struct
{
	int		numVertices;
	int		numElems;
} mdlboundmesh_t;

 
typedef struct
{
	vec3_t	 point;
	vec3_t   normal;
} mdlboundvertex_t;

 
typedef struct
{
	vec3_t	normal;
	float   dist;
} mdlboundplane_t;




class CMDLMng;

class CMDL  :	public BaseResource
{
public:

	int              m_numBoneFrames;
	int              m_numTags;
	int              m_numMeshes;
	int              m_numShaderRefs;
	int              m_numVertices;
	int              m_numElems;

	int              m_numBoundVertices;
	int              m_numBoundElems;
	int              m_numBoundPlanes;
	int              m_numBoundMeshes;

	int              m_numFrameVertices;
	int              m_numFrameElems;

	mdlboneframe_t  *m_boneFrames;
	mdltag3x3_t     *m_tags;
	mdlmesh_t       *m_meshes;
	mdlshaderref_t  *m_shaderRefs;
	mdlvertex_t     *m_vertices;
	element_t       *m_elems;

	mdlboundvertex_t  *m_boundVertices;
	element_t         *m_boundElems;
	mdlboundplane_t   *m_boundPlanes;
	mdlboundmesh_t    *m_boundMeshes;

	vec3_t  m_mins;
	vec3_t  m_maxs;

	char  m_name[ GTH_MAX_SIZE_FILENAME ];
	int	  m_soundIdx;

	
	CFileMng       *m_fileMng;     


	BYTE    *m_buffer;
	
	
	int				m_iBufferSize;
	mdlmesh_t       *m_storedMeshes;
	int				m_iStoredMeshesSize;


	int      m_bank;
	int      m_baseVB;
	int      m_baseEB;
	int      m_sizeVB;
	int      m_sizeEB;
	int      m_needDynVB;

	CMDL();
	virtual ~CMDL();

	void  SetFileMng( CFileMng *fileMng )           { m_fileMng = fileMng; }
	
	int   Load( char *fname );
	int   GetShaderRef( mdlshaderref_t **shaderRef );
	void  Release();
	void  GenNormals();

	
	
	void			Clear()
	{
		m_buffer		=	NULL;
	}
	
	bool			Create( char *fname, int iSize );
	virtual bool	Recreate();
	
	virtual void	Dispose()						
	{	
		GTH_SAFE_FREE( m_buffer );
	}
	size_t			GetSize()						
	{	
		if( m_buffer ) return m_iBufferSize;
		else return 0;
	}
	bool			IsDisposed()
	{
		if( m_buffer ) return false;
		else return true;
	}

	CMDLMng		*m_mdlMng;
	void	SetMDLManager( CMDLMng *mdlMng )	{	m_mdlMng	=	mdlMng;	}
};

#endif 
