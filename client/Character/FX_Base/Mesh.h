 
 
 
 
 
 
 
 
 

 
 
 
#ifndef _MESH_H_HF02HF23H03HESKJDSJ12EF
#define _MESH_H_HF02HF23H03HESKJDSJ12EF

 
 
 
#define	MAX_IMAGEMAP				8
#define	MAX_VERTEXCOLOR				4
#define SIZE_MESH_FILENAME			256
#define	NORMAL_LENGTH_MULTIPLIER	3.0f
 
#define	MAX_MESHES					5

	 
typedef struct
{
	int				iBoneID;				 
	float			fWeight;				 
	D3DXQUATERNION	quatPos;				 
	D3DXQUATERNION	quatNormal;				 
} BONEINFLUENCE;

	 
typedef struct
{
	UINT			iNumBoneInfluences;		 
	BONEINFLUENCE*	boneInfluences;			 
} INFLUENCE;	

	  
typedef struct
{
	D3DXCOLOR		clrAmbient;
	D3DXCOLOR		clrDiffuse;
	D3DXCOLOR		clrSpecular;
	FLOAT			fShininess;
} MATERIAL;

	 
typedef struct
{
	FLOAT			tu;
	FLOAT			tv;
} TEXCOORD;

	 
typedef struct
{
	INT				iVertexID;				 
	D3DXCOLOR		clrVertexColor;			 
	TEXCOORD		VertexTC	[MAX_IMAGEMAP];	 
} VERTEXMAP;

	 
typedef struct
{
	WORD				index	[3];				
} FACEINDEX;

	 
typedef struct
{
	MATERIAL		material;					 
	int				iNumImageMap;				 
	int				iImageMaps	[MAX_IMAGEMAP];	 

	int				iNumFace;					 
	FACEINDEX		*faceLists;					 

	int				iNumVertexMap;				 
	int				iNumVertexColor;			 

	VERTEXMAP		*vertexMaps;					 

	vec3_t			bound_min;
	vec3_t			bound_max;
	int				numBoundPlanes;				 

} MESHCHUNK;

typedef	struct
{
	int			numVertices;
	INFLUENCE	*influences;
	MESHCHUNK	*meshchunk;
}	MESH_t;

 
 
 
class FX_CMesh
{
	 
public:
	FX_CMesh	()	{};
	~FX_CMesh();

	 
public:
		 
	char			m_strFileName[SIZE_MESH_FILENAME];
	char			m_strMeshName[64];

	 
	int				m_iNumLODMesh;
	MESH_t			m_meshes [MAX_MESHES];	 

	 
	char				m_strDesc [256];		
	FX_BONESTRUCT_ID	m_pBoneStID;
	MESHINFO_t			m_pMeshInfo;

		 
	LPDIRECT3DDEVICE8			m_lpD3DDevice;
	DWORD						m_dwNumVertices;
	DWORD						m_dwNumIndices;

		 
	DWORD						m_dwNumNormalVertices;

	int			m_fileVersion;

	 
public:
		 
			VOID		Initialize	(	LPDIRECT3DDEVICE8	lpD3DDevice	);
	virtual	HRESULT		RestoreDeviceObjects();

		 
	virtual	VOID		PreRenderSetting()	{};
	virtual	VOID		PostRenderSetting()	{};
	virtual	VOID		Clear();
	virtual	bool		LoadData(	TCHAR		*strFilename,
									CFileMng	*fileMng	);
			void		LoadLODMeshes	(	BYTE	*in_buffer,	
											int		offset	);
			VOID		DeleteThis();
			void		Cleanup();					 
};

#endif