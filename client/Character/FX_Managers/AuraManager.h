 
 
 
 
 
 
 
 
 

 
 
 
#ifndef	_FX_AURA_MANAGER_H_ALKJSHD48VU4HRUA
#define _FX_AURA_MANAGER_H_ALKJSHD48VU4HRUA

 
 
 

class	FX_CAuraManager
{
	 
public:
	FX_CAuraManager ();
	~FX_CAuraManager ();

	 
public:
	int					m_numAuraShader;

	DIFFUSETEXVERTEX	m_PreVB [SIZE_PREVB_AURA];
	WORD				m_PreIB [(AURA_MAX_DIVIDE + 1) * (MAX_AURA_IN_CHAR - 1) * 6];

	float				m_texfactor;

	int					m_numVertexToDraw;
	int					m_numIndexToDraw;

		 
	FX_CCharManager		*m_pCharManager;
	CD3DTextureMng		*m_texMng;
	CScript				*m_pScript;
	CD3DRender			*m_D3DRender;
	FX_CCharShaderMng	*m_pCharShaderMng;

		 
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	FX_CAuraRenderer	*m_auraRenderer;

	 
public:
	void		Render (	Fx_CHARACTER_t	*in_char,	int		*in_numprims,	const int	in_shaderIdx,	const int	in_numUsed	);

	void		SetDevice		(	const LPDIRECT3DDEVICE8	device	);
	void		SetScript		(	CScript			*script		)		{	m_pScript	=	script;		};

		 
	void		SetCharManager	(	FX_CCharManager		*charManager	)	{	m_pCharManager	=	charManager;	};
	void		SetTexManager	(	CD3DTextureMng	*texMng		)			{	m_texMng		=	texMng;			};
	void		SetRenderer		(	CD3DRender		*d3dRender	);
	void		SetShaderMng	(	CShaderMng		*shaderMng	);
	void		SetCharShaderMng	(	FX_CCharShaderMng	*charShaderMng	)	{	m_pCharShaderMng	=	charShaderMng;	};
	
	bool		PrepareRenderBuffers();
	bool		ReleaseRenderBuffers();

	int			Update		(	float	&in_update,	const float	in_cur		);
	void		UpdatePos (	AURA_t	&in_aura,	const vec3_t	in_pttop,	const vec3_t	in_ptbottom		);

	void		ResetUpdateTime (	int		in_index	);
	void		SetInitialPos	(	AURA_t	*in_aura,	const vec3_t	in_pttop,	const vec3_t	in_ptbottom		);

	void		FadeAura	(	AURA_t		&in_aura	);

	bool		MakePreVertexBuffer	(	const AURA_t	*in_aura	);
	void		MakePreIndexBuffer	();
	void		Cleanup ();
};

#endif