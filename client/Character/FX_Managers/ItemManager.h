 
 
 
 
 
 
 
 
 
#ifndef _NON_SKINNING_MESH_MANAGER_H_J09D3J09JD
#define _NON_SKINNING_MESH_MANAGER_H_J09D3J09JD

 
 
 
typedef struct
{
	FX_CMesh		mesh;
	UINT			iRefID;
	UINT			iVertexStart;
	UINT			iNumVertex;
	UINT			iIndexStart;
	UINT			iPrimitiveCount;
	int				iTagIndex;
	int				iParentTagIndex;
} ITEM_t;

 
 
 

class FX_CItemManager
{
	 
public:
	FX_CItemManager();
	~FX_CItemManager();
		
	 
public:
		 
	ITEM_t				m_Items	[MAX_COUNT_ITEM_MESH];
	int					m_iNumItems;

	Fx_ITEM_BUF_t		m_ItemBuffer [MAX_COUNT_ITEM_MESH];

	CHAR_BUFFER_t		m_CharBuffer [MAX_COUNT_AVAIL_ITEM];	
	int					m_iNumMeshInBuffer;
	int					m_iBufferIndexMap [MAX_COUNT_ITEM_MESH];		 

	FX_CItemRenderer	*m_pItemRenderer;

		 
	FX_CCharManager		*m_pCharManager;
	FX_CSkinManager		*m_pSkinManager;
	FX_CSkelManager		*m_pSkelManager;
	FX_CCharUtil		*m_pCharUtil;
	FX_CCharShaderMng	*m_pCharShaderMng;

	CTagManager			*m_pTagManager;
	FX_CBoneManager		*m_pBoneManager;
	
	CScript				*m_pScript;
	CD3DTextureMng		*m_texMng;
	CFileMng			*m_fileMng;
	CD3DRender			*m_D3DRender;
	CShaderMng			*m_shaderMng;
	CTimer				*m_timer;

		 
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	 
public:
	bool		LoadItemList();
	void		UpdateTextureID();

	bool		CountVertices	(	UINT	&iVertexResult,		UINT	&iIndexResult	);
	void		PrepareRenderBuffers ();
	void		ReleaseRenderBuffers ();

	bool		UpdateCharBuffer();			 

	bool		RenderAll	(	int		*in_numprims	);
	void		Render		(	Fx_CHARACTER_t	*in_char,	const int	in_refIdx,	const ITEM_t	*pThisItem	);
	void		EffectRender	(	Fx_CHARACTER_t	*in_char,	const int	in_refIdx,	const ITEM_t	*pThisItem	);
	void		Cleanup();					 

		 
	void		SetCharManager	(	FX_CCharManager		*charManager	)		{	m_pCharManager	=	charManager;	}
	void		SetSkinManager	(	FX_CSkinManager		*skinManager	)		{	m_pSkinManager	=	skinManager;	}
	void		SetSkelManager	(	FX_CSkelManager		*skelManager	)		{	m_pSkelManager	=	skelManager;	}
	void		SetCharUtil		(	FX_CCharUtil		*charUtil		)		{	m_pCharUtil		=	charUtil;		}

	void		SetTagManager	(	CTagManager			*tagManager		)		{	m_pTagManager	=	tagManager;		}
	void		SetBoneManager	(	FX_CBoneManager		*boneManager	)		{	m_pBoneManager	=	boneManager;	}

	void		SetScript		(	CScript			*script		)		{	m_pScript	=	script;		}

	void		SetDevice		(	LPDIRECT3DDEVICE8	device	);
	void		SetTexManager	(	CD3DTextureMng	*texMng		);
	void		SetFileManager	(	CFileMng		*fileMng	)		{	m_fileMng	=	fileMng;		}
	void		SetRenderer		(	CD3DRender		*d3dRender	);
	void		SetShaderMng	(	CShaderMng		*shaderMng	);
	void		SetCharShaderMng	(	FX_CCharShaderMng	*charShaderMng	)	{	m_pCharShaderMng	=	charShaderMng;	};

	matrix4x4_t*	GetTagMatrixOut (	Fx_CHARACTER_t	*in_char,	const int	in_tag	);
	matrix4x4_t*	GetTagMatrix (	Fx_CHARACTER_t	*in_char,	const int	in_tag,		const int	in_parentTag	);
	void			GetTagMatrixUntransformed (	Fx_CHARACTER_t	*in_char,	const int	in_tag,		const int	in_parentTag,	matrix4x4_t		&out_matrix	);
	void			MakeTagMatrix	(	const Fx_CHARACTER_t	*in_char,	const KEYFRAME	*in_frame,	const TAG	*in_tag,	const int	in_tagID,	
										matrix4x4_t	&out_matrix,	const bool		in_transform	);
	void			SetTimer		(	CTimer	*timer	)		{	m_timer	=	timer;	}
	void			GetCharPosByIdxOnSvr	(	const int	in_targetIdx,	vec3_t	out_pos	);
	void			TagLookTarget	(	const vec3_t	in_mypos,	const int	in_tgtIdx,	vec4_t	out_quat	);
	void			TagLookCenter	(	const vec3_t	in_direction,	vec4_t	out_quat	);
	
private:
	bool		LoadItem	(	char	*pszSkinName, 	const unsigned int	iSkinID,	const unsigned int	iTexID		);
};

#endif