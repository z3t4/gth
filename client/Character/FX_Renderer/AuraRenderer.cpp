 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

 
 
 
FX_CAuraRenderer::FX_CAuraRenderer	()
{
	m_VB	=	NULL;
	m_IB	=	NULL;

	Initialize();
	
	if	(	g_resCounter->Enabled()	)
	{
		g_resCounter->Add_DataType( RESCOUNTER_CHAR_AURARENDERER, sizeof(FX_CAuraRenderer) );
	}
}

 
 
 
bool	FX_CAuraRenderer::Initialize	()
{
	return	true;
}

 
 
 
 
bool	FX_CAuraRenderer::CreateBuffers ()
{
	m_VB	=	new	CD3DBuffer ();
	m_IB	=	new	CD3DBuffer ();

	int		indicesize	=	(AURA_MAX_DIVIDE + 1) * (MAX_AURA_IN_CHAR - 1) * 6;

	 
	m_VB ->Create	(	VERTEX_BUFFER_SIZE,
					GTH_FORMAT_XYZ | GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX(1),
					GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER	);

	 
	m_IB ->Create	(	indicesize,
					GTH_FORMAT_INDEX16,
					GTH_INDEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY	);

	return	true;
}

 
 
 
void	FX_CAuraRenderer::ReleaseBuffers ()
{
	if	(	m_VB	)	m_VB ->Release ();
	if	(	m_IB	)	m_IB ->Release ();
	GTH_SAFE_DELETE ( m_VB );
	GTH_SAFE_DELETE ( m_IB );

	return;
}

 
 
 
void	FX_CAuraRenderer::Cleanup()
{
	ReleaseBuffers();
	
	return;
}

 
 
 
bool	FX_CAuraRenderer::PrepareRender	()
{
	m_D3DRender ->SetVertexShader	(	GTH_AURA_FORMAT	);
	m_D3DRender ->SetStreamSource	(	0,	m_VB ->GetD3DVertexBuffer(),	sizeof (DIFFUSETEXVERTEX)	);

	m_D3DRender ->SetIndices		(	m_IB ->GetD3DIndexBuffer (),	0L	);

	m_D3DRender ->SetTransform	(	D3DTS_WORLD,	(matrix4x4_t *)&IDENTITY_MATRIX	);

	return	true;
}

 
 
 
 
void	FX_CAuraRenderer::LockVB	(	DIFFUSETEXVERTEX	*in_preVB,	
										const int			in_vertSize		)
{
		 
	m_VB ->Lock	(	in_vertSize	);

		 
	memcpy(	m_VB->GetBuffer(), in_preVB, sizeof(DIFFUSETEXVERTEX) * in_vertSize );

		 
	m_VB ->Unlock ();

	m_vertexCount	=	in_vertSize;

	return;
}

 
 
 
 
void	FX_CAuraRenderer::LockIB	(	WORD		*in_preIB,
										const int	in_indexSize	)
{
	WORD	*buffer;
	WORD	*word	=	in_preIB;
	WORD	base	=	(WORD)m_VB ->GetBase ();

	 
	m_IB ->Lock	();

	buffer	=	(WORD *)m_IB ->GetBuffer ();
	for	(	int	index	=	0;	\
				index	<	in_indexSize;	\
				++index,	++buffer,	++word		)
	{
		(*buffer)	=	(*word)	+ base;
	}

		 
	m_IB ->Unlock();

	m_indexCount	=	in_indexSize;

	return;
}

 
 
 
 
void	FX_CAuraRenderer::Render (	int	*in_numprims	)
{
	m_pd3dDevice ->SetRenderState( D3DRS_LIGHTING, FALSE );

	m_D3DRender ->RenderDefault	(	m_VB ->GetBase (),
									m_vertexCount,
									0,
									(*in_numprims) * 3	);

	m_pd3dDevice ->SetRenderState( D3DRS_LIGHTING, TRUE );

	return;
}


 
 
 
 
void	FX_CAuraRenderer::ShaderRender (	Fx_CHARACTER_t	*in_char,
											const int		in_shaderRef,
											int				*in_numprims	)
{
	m_pd3dDevice ->SetRenderState ( D3DRS_LIGHTING, FALSE );

	shader_t	*thisShader	=	&m_shaderMng ->m_shaders[ in_shaderRef ];
#ifdef	_FX_CLIENT
	if	(	thisShader ->flags	&	SHADER_TCGEN	)
		m_shaderMng ->PrepareTCGen (	thisShader,	in_char ->position	);
#endif

	m_D3DRender ->RenderMDLCustom (	thisShader,	m_VB ->GetBase (),	m_vertexCount,	0,	(*in_numprims) * 3	);

	m_pd3dDevice ->SetRenderState ( D3DRS_LIGHTING, TRUE );

	return;
}
