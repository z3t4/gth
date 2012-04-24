 
 
 
 
 
 
 
 
 
#ifndef _SKIN_RENDERER_H_QOIHF32HR0HE20
#define _SKIN_RENDERER_H_QOIHF32HR0HE20

 
 
 
class FX_CSkinRenderer
{
	 
public:
	FX_CSkinRenderer	();
	~FX_CSkinRenderer	()	{};
	
	 
public:
	CD3DBuffer				*m_charVB[ MAX_CHAR_A_SCENE ];
	CD3DBuffer				*m_charIB[ MAX_CHAR_A_SCENE ];
	CD3DBuffer				*m_creatureVB[ MAX_CREATURE_A_SCENE ];
	CD3DBuffer				*m_creatureIB[ MAX_CREATURE_A_SCENE ];

	LPDIRECT3DDEVICE8		m_pd3dDevice;

	int						m_iCurNumVertices;		 
	int						m_iCurNumPrimitives;	 

	int						m_iCurBaseIndex;		 

	CD3DRender				*m_D3DRender;
	CShaderMng				*m_shaderMng;

		 
	KEYFRAME			*m_pInterpolatedFrame;
	VERTEXMAP			*m_pVertexMap;
	INFLUENCE			*m_pInfluence;
	BONEINFLUENCE		*m_pBoneInfluence;
	FLOAT				m_fWeight;

		 
	TEXTUREVERTEX		*m_pVertices;
	WORD				*m_pIndices;

	FX_CSkelManager		*m_pSkelManager;

	TEXTUREVERTEX		m_vertexBuffer[ VERTEX_BUFFER_SIZE ];

	 
public:
	bool		CreateBuffers();
	void		ReleaseBuffers();

	bool		TransformMesh	(	Fx_CHARACTER_t	*in_char,	const FX_CMesh	*pMesh,	const KEYFRAME	*pKeyFrames,
									const int	in_sysVBNum,	const int	in_skinPartID	);
	bool		PrepareRender();
	bool		Render (	Fx_CHARACTER_t	*in_char,	int		*in_numprims,	const int	in_skinPartIdx	);
	void		ShaderRender (	Fx_CHARACTER_t	*in_char,	const int	in_shaderRef,	int	*in_numprims,	const int	in_skinPartIdx	);
	void		Cleanup();

	void		SetDevice	(	LPDIRECT3DDEVICE8	device	);
	void		SetRenderer		(	CD3DRender		*d3dRender	)			{	m_D3DRender	=	d3dRender;	}

	void		SetShaderMng	(	CShaderMng		*shaderMng	)	{	m_shaderMng	=	shaderMng;	};

	void		SetSkelManager	(	FX_CSkelManager		*skelManager	)		{	m_pSkelManager	=	skelManager;	}

	void		SetIndices (	Fx_CHARACTER_t	*in_char,	const FACEINDEX	*in_indices,	const int	in_numIndices,	const int	in_skinPartIdx	);
	void		CopyVertex (	Fx_CHARACTER_t	*in_char,	TEXTUREVERTEX	*in_sysVB,	const int	in_sysVBNum,	const int	in_skelPartIdx	);
	void		RenderShadow	(	Fx_CHARACTER_t	*in_char,	FX_CSkinManager		*skinManager	);
	void		SetStreamSource	(	Fx_CHARACTER_t	*in_char,	const int	in_streamIdx	);
	void		DiscardIndexBuffer	(	Fx_CHARACTER_t	*in_char	);
	void		DiscardVertexBuffer	(	Fx_CHARACTER_t	*in_char	);

	LPDIRECT3DINDEXBUFFER8	GetIndexBuffer	(	Fx_CHARACTER_t	*in_char	);

private:
	bool		Initialize();
};	

#endif
