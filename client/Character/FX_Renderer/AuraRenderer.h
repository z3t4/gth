 
 
 
 
 
 
 
 
 

 
 
 
#ifndef	_FX_AURA_RENDERER_H_AKJSHD34998Y4R
#define _FX_AURA_RENDERER_H_AKJSHD34998Y4R

 
 
 

class	FX_CAuraRenderer
{
	 
public:
	FX_CAuraRenderer ();
	~FX_CAuraRenderer () {};

	 
public:
		 
	CD3DBuffer				*m_VB;
	CD3DBuffer				*m_IB;

	LPDIRECT3DDEVICE8		m_pd3dDevice;

	int						m_vertexCount;
	int						m_indexCount;

		 
	FX_CAuraManager			*m_pAuraManager;

	CShaderMng				*m_shaderMng;
	CD3DRender				*m_D3DRender;
	

	 
public:
	bool		CreateBuffers	();
	bool		FillBuffers		();
	void		ReleaseBuffers	();
	bool		PrepareRender	();
	void		Cleanup	();
	void		LockVB	(	DIFFUSETEXVERTEX	*in_preVB,	const int		in_vertSize		);
	void		LockIB	(	WORD	*in_preIB,	const int	in_indexSize	);
	void		Render	(	int		*in_numprims	);

	void		SetDevice	(	LPDIRECT3DDEVICE8	device	)	{	m_pd3dDevice	=	device;	};
	int			GetBaseVertexIndex	()		{	return	m_VB ->GetBase ();	};

	void		SetRenderer		(	CD3DRender		*d3dRender	)			{	m_D3DRender		=	d3dRender;		};
	void		SetShaderMng	(	CShaderMng		*shaderMng	)	{	m_shaderMng	=	shaderMng;	};
	void		ShaderRender	(	Fx_CHARACTER_t	*in_char,	const int	in_shaderRef,	int		*in_numprims	);

private:
	bool		Initialize();
};

#endif