 
 
 
 
 
 
 
 
 

 
 
 
#ifndef	_FX_CHARACTER_SHADER_MANAGER_H_ASDIH9384GY
#define	_FX_CHARACTER_SHADER_MANAGER_H_ASDIH9384GY

 
 
 

 
 
 

class	FX_CCharShaderMng
{
	 
public:
	FX_CCharShaderMng ();
	~FX_CCharShaderMng ();

	 
public:
	int					m_iNumTexture;							 
	Fx_TEX_BUF_t		m_TexBuffer [MAX_COUNT_TEXTURE];		 
	CHAR_SHADERREF_t	m_shaderRefs [MAX_COUNT_TEXTURE];

	int					m_numEffectTexture;							 
	char				m_effectTexName [MAX_COUNT_EFFECT_TEXTURE][MAX_PATH];	 
	CHAR_SHADERREF_t	m_effectShaderRefs [MAX_COUNT_EFFECT_TEXTURE];

		 
	CD3DTextureMng		*m_pTexMng;
	CScript				*m_pScript;
	CShaderMng			*m_shaderMng;
	FX_CCharUtil		*m_pCharUtil;
	DWORD				m_frameFlag;
	
	 
public:
		 
	inline	void	SetScript		(	CScript			*script		)		{	m_pScript		=	script;			};
	inline	void	SetShaderMng	(	CShaderMng		*shaderMng	)		{	m_shaderMng		=	shaderMng;		};
	inline	void	SetTexManager	(	CD3DTextureMng	*texMng		)		{	m_pTexMng		=	texMng;			};
	inline	void	SetCharUtil		(	FX_CCharUtil	*charUtil	)		{	m_pCharUtil		=	charUtil;		};

			bool	LoadTextureList			();
			bool	LoadEffectTextureList	();
			int		PrepareCharShaderLogin ();
			int		PrepareCharShaderCommon ();
			int		PrepareEffectShader		();
			int		SearchShaderRef			(	char	*in_name		);
	inline	void	PrepareShader			(	int		in_shaderIdx	)			{	m_shaderMng ->PrepareShader (	in_shaderIdx,	m_frameFlag	);	};
	inline	void	PrepareShaderByRef		(	int		in_shaderIdx	)			{	PrepareShader (	GetShaderIdx ( in_shaderIdx )	);	};
	inline	void	PrepareEffectShaderByRef(	int		in_effectIdx	)			{	PrepareShader (	GetEffectShaderIdx ( in_effectIdx )	);	};
	inline	void	PrepareTimeShader			(	int		in_shaderIdx,	float	in_elapsedTime	)	
												{	m_shaderMng ->PrepareShader (	in_shaderIdx,	in_elapsedTime	);	};
	inline	void	PrepareTimeShaderByRef		(	int		in_shaderIdx,	float	in_elapsedTime	)
												{	PrepareTimeShader (	GetShaderIdx ( in_shaderIdx ),	in_elapsedTime	);	};
	inline	void	PrepareTimeEffectShaderByRef(	int		in_effectIdx,	float	in_elapsedTime	)
												{	PrepareTimeShader (	GetEffectShaderIdx ( in_effectIdx ),	in_elapsedTime	);	};
	inline	void	UpdateFrameFlag			()										{	++m_frameFlag;	};
	inline	int		CheckIsShader			(	int		in_shaderIdx	)			{	return	m_shaderRefs [in_shaderIdx].isShader;		};
	inline	int		CheckIsEffectShader		(	int		in_effectIdx	)			{	return	m_effectShaderRefs [in_effectIdx].isShader;	}
	inline	int		GetShaderIdx			(	int		in_shaderIdx	)			{	return	m_shaderRefs [in_shaderIdx].shader;		};
	inline	int		GetEffectShaderIdx		(	int		in_effectIdx	)			{	return	m_effectShaderRefs [in_effectIdx].shader;		};
	inline	LPDIRECT3DTEXTURE8	GetD3DTextureByRef		(	int	in_shaderIdx	)	{	return	m_pTexMng ->GetD3DTexture (	GetShaderIdx ( in_shaderIdx )	);	};
	inline	LPDIRECT3DTEXTURE8	GetD3DEffectTextureByRef(	int	in_effectIdx	)	{	return	m_pTexMng ->GetD3DTexture (	GetEffectShaderIdx ( in_effectIdx )	);	};
	inline	shader_t*	GetShader			(	int		in_shaderIdx	)			{	return	&m_shaderMng ->m_shaders [in_shaderIdx];	};
	inline	shader_t*	GetShaderByRef		(	int		in_shaderRef	)			{	return	GetShader (		GetShaderIdx ( in_shaderRef )	);	};
	inline	shader_t*	GetEffectShaderByRef(	int		in_shaderRef	)			{	return	GetShader (		GetEffectShaderIdx ( in_shaderRef )	);	};
};

#endif