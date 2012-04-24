 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

 
 
 
 
FX_CAuraManager::FX_CAuraManager	()
{
	 
	m_auraRenderer	=	new	FX_CAuraRenderer ();
}

FX_CAuraManager::~FX_CAuraManager	()
{
}

 
 
 
 
int		FX_CAuraManager::Update	(	float		&in_update,
									const float	in_cur		)
{
	if	(	in_cur	<	in_update	)
		return	false;

	in_update	+=	AURA_UPDATE_TIME;

	return	true;
}

 
 
 
 
void	FX_CAuraManager::UpdatePos (	AURA_t			&in_aura,
										const vec3_t	in_pttop,
										const vec3_t	in_ptbottom		)
{
	if	(	in_aura.num_used	<	MAX_AURA_IN_CHAR	)
	{
		VectorCopy ( in_aura.start [in_aura.num_used].vecPos,	in_pttop );
		VectorCopy ( in_aura.end [in_aura.num_used].vecPos,		in_ptbottom );
		in_aura.num_used++;
	}
	else
	{
		memmove( &in_aura.start[ 0 ], &in_aura.start[ 1 ], sizeof(DIFFUSETEXVERTEX) * (MAX_AURA_IN_CHAR - 1) );
		memmove( &in_aura.end[ 0 ], &in_aura.end[ 1 ], sizeof(DIFFUSETEXVERTEX) * (MAX_AURA_IN_CHAR - 1) );

		VectorCopy ( in_aura.start [MAX_AURA_IN_CHAR - 1].vecPos,	in_pttop );
		VectorCopy ( in_aura.end [MAX_AURA_IN_CHAR - 1].vecPos,		in_ptbottom );
	}

	return;
}

 
 
 
 
 
bool	FX_CAuraManager::MakePreVertexBuffer	(	const AURA_t	*in_aura	)
{
	int		startctrl;
	int		endctrl;
	int		index,	subindex;
	int		counter;
	const float	tex_disp		=	0.98f / ( (in_aura ->num_used - 1) * (AURA_NUM_DIVIDE + 1) );
	float	tex_coord_u;

	tex_coord_u	=	0.99f;
	counter		=	0;

	VectorCopy ( m_PreVB [counter].vecPos,	in_aura ->start [0].vecPos );
	m_PreVB [counter].d3dColor	=	D3DCOLOR_WHITE;
	m_PreVB [counter].tu		=	tex_coord_u;
	m_PreVB [counter].tv		=	0.01f;
	counter++;

	VectorCopy ( m_PreVB [counter].vecPos,	in_aura ->start [0].vecPos );
	m_PreVB [counter].d3dColor	=	D3DCOLOR_WHITE;
	m_PreVB [counter].tu		=	tex_coord_u;
	m_PreVB [counter].tv		=	0.99f;
	counter++;
	tex_coord_u	-=	tex_disp;

	int		startidx,	endidx;
	for	(	index	=	0;	\
			index	<	( in_aura ->num_used - 1 );	\
			++index		)
	{
		if	(	index	==	0	)
			startctrl	=	0;
		else
			startctrl	=	index	-	1;

		if	(	index	==	in_aura ->num_used - 2	)
			endctrl	=	in_aura ->num_used - 1;
		else
			endctrl	=	index	+	2;

		if	(	( startctrl < 0 ) || ( endctrl < 0 ) || ( startctrl > (in_aura ->num_used - 1) ) || ( endctrl > (in_aura ->num_used - 1) )	)
		{
			ErrorLog( "index : %d, startstrl : %d, endctrl : %d, AURA_t::num_used : %d", index, startctrl, endctrl, in_aura->num_used );
			return	false;
		}

		FX_CatmullRomMulti	(	&m_PreVB [counter],
								in_aura ->start [index].vecPos ,		in_aura ->start [startctrl].vecPos,
								in_aura ->start [index + 1].vecPos,		in_aura ->start [endctrl].vecPos,
								AURA_NUM_DIVIDE	);
		FX_CatmullRomMulti	(	&m_PreVB [counter + 1],
								in_aura ->end [index].vecPos,			in_aura ->end [startctrl].vecPos,
								in_aura ->end [index + 1].vecPos,		in_aura ->end [endctrl].vecPos,
								AURA_NUM_DIVIDE	);

		startidx	=	counter		+	(AURA_NUM_DIVIDE * 2);
		endidx		=	startidx	+	1;
		VectorCopy ( m_PreVB [startidx].vecPos,	in_aura ->start [index + 1].vecPos );
		VectorCopy ( m_PreVB [endidx].vecPos,	in_aura ->end [index + 1].vecPos );

		for	(	subindex	=	0;	\
				subindex	<	( AURA_NUM_DIVIDE + 1 );	\
				++subindex		)
		{
			m_PreVB [counter].d3dColor	=	D3DCOLOR_WHITE;
			m_PreVB [counter].tu		=	tex_coord_u;
			m_PreVB [counter].tv		=	0.01f;
			counter++;

			m_PreVB [counter].d3dColor	=	D3DCOLOR_WHITE;
			m_PreVB [counter].tu		=	tex_coord_u;
			m_PreVB [counter].tv		=	0.99f;
			counter++;
			tex_coord_u	-=	tex_disp;
		}
	}

	m_numVertexToDraw	=	counter;

	if	(	m_numVertexToDraw	<	0	)
	{
		ErrorLog( "m_numVertexToDraw : %d", m_numVertexToDraw );
		return	false;
	}

	m_auraRenderer ->LockVB (	m_PreVB,	m_numVertexToDraw	);
	m_auraRenderer ->LockIB (	m_PreIB,	m_numIndexToDraw	);

	return	true;
}

 
 
 
 
void	FX_CAuraManager::MakePreIndexBuffer	()
{
	int			counter	=	0;
	const int	max	=	(MAX_AURA_IN_CHAR - 1) * (AURA_MAX_DIVIDE + 1) * 2;

	for	(	WORD	index	=	0;	\
					index	<	max;		\
					index	+=	2		)
	{
		m_PreIB [counter++]	=	index;
		m_PreIB [counter++]	=	index + 1;
		m_PreIB [counter++]	=	index + 2;

		m_PreIB [counter++]	=	index + 2;
		m_PreIB [counter++]	=	index + 1;
		m_PreIB [counter++]	=	index + 3;
	}

	m_numIndexToDraw	=	counter;

	return;
}

 
 
 
 
void	FX_CAuraManager::Render (	Fx_CHARACTER_t	*in_char,
									int				*in_numprims, 
									const int		in_shaderIdx,
									const int		in_numUsed		)
{
		 
	*in_numprims	=	(in_numUsed - 1) * (AURA_NUM_DIVIDE + 1) * 2;

	m_auraRenderer ->PrepareRender ();

		 
	if	(	m_pCharShaderMng ->CheckIsEffectShader ( in_shaderIdx )	)
	{
		m_pCharShaderMng ->PrepareEffectShaderByRef (	in_shaderIdx	);
		m_auraRenderer ->ShaderRender	(	in_char,	m_pCharShaderMng ->GetEffectShaderIdx ( in_shaderIdx ),	in_numprims	);
	}
	else
	{
		m_pd3dDevice ->SetTexture	(	0,	m_pCharShaderMng ->GetD3DEffectTextureByRef ( in_shaderIdx )	);
		m_auraRenderer ->Render (	in_numprims	);
	}

	return;
}


 
 
 
void	FX_CAuraManager::SetDevice	(	const LPDIRECT3DDEVICE8	device	)
{
	m_pd3dDevice	=	device;

	m_auraRenderer ->SetDevice	(	m_pd3dDevice	);
}

 
 
 
 
void	FX_CAuraManager::SetRenderer (	CD3DRender		*d3dRender	)
{
	m_D3DRender	=	d3dRender;

	m_auraRenderer ->SetRenderer (	d3dRender	);

	return;
}

 
 
 
 
void	FX_CAuraManager::SetShaderMng	(	CShaderMng		*shaderMng	)
{
	m_auraRenderer ->SetShaderMng (	shaderMng	);

	return;
}

 
 
 
bool	FX_CAuraManager::PrepareRenderBuffers()
{
	m_auraRenderer ->CreateBuffers ();
	MakePreIndexBuffer ();

	return	true;
}

 
 
 
bool	FX_CAuraManager::ReleaseRenderBuffers()
{
	m_auraRenderer ->ReleaseBuffers ();
		
	return	true;
}

 
 
 
 
void	FX_CAuraManager::SetInitialPos	(	AURA_t	*in_aura,
											const vec3_t	in_pttop,
											const vec3_t	in_ptbottom		)
{
	VectorCopy ( in_aura ->start [0].vecPos,	in_pttop );
	VectorCopy ( in_aura ->end [0].vecPos,		in_ptbottom );

	in_aura ->num_used	=	1;

	return;
}

 
 
 
 
void	FX_CAuraManager::FadeAura	(	AURA_t		&in_aura	)
{
	if	(	false	==	in_aura.render	)
		return;

	if	(	in_aura.num_used	==	0	)
	{
		in_aura.render	=	false;
		return;
	}

	memmove( &in_aura.start[ 0 ], &in_aura.start[ 1 ], sizeof(DIFFUSETEXVERTEX) * (in_aura.num_used - 1) );
	memmove( &in_aura.end[ 0 ], &in_aura.end[ 1 ], sizeof(DIFFUSETEXVERTEX) * (in_aura.num_used - 1) );
	if	(	in_aura.num_used	>	1	)
		in_aura.num_used--;
	else
		in_aura.render	=	false;

	return;
}

 
 
 
 
void	FX_CAuraManager::Cleanup ()
{
	GTH_SAFE_DELETE (	m_auraRenderer );

	return;
}