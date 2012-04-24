 
 
 
 
 
 
 
 
 
#ifndef _SKINNING_MESH_MANAGER_H_J0J092JD0293
#define _SKINNING_MESH_MANAGER_H_J0J092JD0293

 
 
  

typedef struct
{
	FX_CMesh		mesh;
	int				iRefID;
	int				itemID;			 
	int				partID;
} SKIN_t;

#define	MAX_DEF_SKIN	10		 

typedef struct
{
	int		list [MAX_COUNT_SKIN_PART][MAX_DEF_SKIN];
	int		count [MAX_COUNT_SKIN_PART];
}	FX_DEF_SKIN_t;

 
 
 


class FX_CSkinManager
{
	 
public:
	FX_CSkinManager();
	~FX_CSkinManager() {};

	 
public:
		 
	SKIN_t			m_Skins	[MAX_COUNT_SKIN_MESH];
	int				m_iNumSkins;

	CHAR_BUFFER_t	m_CharBuffer	[MAX_COUNT_AVAIL_ITEM];	
	int				m_iNumMeshInBuffer;
	int				m_iBufferIndexMap [MAX_COUNT_SKIN_MESH];

	Fx_SKIN_BUF_t	m_SkinBuffer [MAX_COUNT_SKIN_MESH];

	int				m_meshOffset [MAX_COUNT_SKIN_PART];

		 
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	FX_CSkinRenderer	*m_pSkinRenderer;

		 
	FX_CCharManager		*m_pCharManager;
	FX_CCharUtil		*m_pCharUtil;
	FX_CSkelManager		*m_pSkelManager;
	FX_CCharShaderMng	*m_pCharShaderMng;

	CScript				*m_pScript;
	CD3DTextureMng		*m_texMng;
	CFileMng			*m_fileMng;
	CD3DRender			*m_D3DRender;
	CShaderMng			*m_shaderMng;

		 
	FX_DEF_SKIN_t		m_defSkinList [NUM_GEN_TYPE];

	 
public:
	bool		LoadSkinList();
	void		UpdateTextureID();
	bool		PrepareRenderBuffers();
	bool		ReleaseRenderBuffers();

	bool		RenderAllNormal	(	int		*in_numprims	);
#ifdef	_FX_CLIENT
	bool		RenderAllDisappear	(	int		*in_numprims	);
#endif	 

	bool		UpdateCharBuffer();			 
	void		Cleanup();

		 
	void		SetCharManager	(	FX_CCharManager		*charManager	)		{	m_pCharManager	=	charManager;	}
	void		SetCharUtil		(	FX_CCharUtil		*charUtil		)		{	m_pCharUtil		=	charUtil;		}
	void		SetSkelManager	(	FX_CSkelManager		*skelManager	);

	void		SetScript		(	CScript			*script		)		{	m_pScript	=	script;		}

	void		SetDevice		(	LPDIRECT3DDEVICE8	device	);
	void		SetTexManager	(	CD3DTextureMng	*texMng		);
	void		SetFileManager	(	CFileMng		*fileMng	)		{	m_fileMng	=	fileMng;	}
	void		SetRenderer		(	CD3DRender		*d3dRender	);
	void		SetShaderMng	(	CShaderMng		*shaderMng	);
	void		SetCharShaderMng	(	FX_CCharShaderMng	*charShaderMng	)	{	m_pCharShaderMng	=	charShaderMng;	};

	int			MakeSkinID	(	const int	in_type,	const int	in_internalID	);
	void		SplitSkinID	(	const int	in_skinID,	int		&out_type,	int		&out_internalID		);

		 
	int			LoadDefSkinList ();
	int			ParseSubDefSkinList (	char	*in_token	);
	int			ParseNextLevel (	int		*out_list,	int		&out_count	);
	void		GetDefaultSkin	(	const int	in_gentype,		const int	in_type,	int		*out_list,	int		&out_count	);

	int			GetDefaultSkinID	(	const int	in_gentype,	const int	in_type,	const int	idx	);

	int			GetNumVertex	(	const int	in_skinID,	const int	in_LODLevel	);
	int			GetNumIndex		(	const int	in_skinID,	const int	in_LODLevel	);
	void		SetSkinRenderer (	FX_CSkinRenderer	*skinRenderer	)	{	m_pSkinRenderer		=	skinRenderer;	};

	void		SetTextureByRef	(	int		in_ref	);

	void		DiscardAllIndexBuffers	();
	void		DiscardIndexBuffer	(	Fx_CHARACTER_t	*in_char	);
	void		DiscardAllVertexBuffers	();
	void		DiscardVertexBuffer	(	Fx_CHARACTER_t	*in_char	);

private:
	bool		LoadSkin	(	char	*pszSkinName, const	unsigned int	&iSkinID,	const unsigned int	&iTexID		);
	bool		SetRenderState	();
};

#endif