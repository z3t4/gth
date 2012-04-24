 
 
 
 
 
 
 
 
 

 
 
 
#include "../global_def.h"



 
 
 
 
CSnowDropper::CSnowDropper	()
{
	OneTimeSceneInit ();
}

CSnowDropper::~CSnowDropper	()
{
}

 
 
 
 
HRESULT	CSnowDropper::OneTimeSceneInit	()
{
	m_areaSize		=	0.0f;
	m_velocity [0]	=	0.0f;
	m_velocity [1]	=	0.0f;
	m_velocity [2]	=	0.0f;
	m_generateRatio	=	0.0f;

	m_velocityMod [0]	=	0.0f;
	m_velocityMod [1]	=	0.0f;
	m_velocityMod [2]	=	0.0f;

	m_D3DRender	=	NULL;

	m_snowShaderRef	=	-1;

	memset ( m_drops, 0, sizeof(m_drops) );
	memset ( m_dropBuffer, 0, sizeof(m_dropBuffer) );

	for	(	int	index	=	0;	\
				index	<	MAX_NUM_SNOW;	\
				++index		)
	{
		m_dropBuffer [index].color	=	0xffffffff;
	}

	D3DXMatrixIdentity ( &m_matWorld );

	return	S_OK;
}

 
 
 
 
void	CSnowDropper::Reset	()
{
	m_numUsedDrops	=	0;
	m_remainder		=	0.0f;
	m_numGenerating	=	0;

	m_firstDrop	=	NULL;

	GenerateEmptyDropLink ();
}

 
 
 
 
HRESULT	CSnowDropper::GenerateEmptyDropLink	()
{
	m_drops [0].prev	=	NULL;
	m_drops [0].next	=	&m_drops [1];

	for	(	int	index	=	1;	\
				index	<	( MAX_NUM_SNOW - 1 );	\
				++index	)
	{
		m_drops [index].prev	=	&m_drops [index - 1];
		m_drops [index].next	=	&m_drops [index + 1];
	}

	m_drops [MAX_NUM_SNOW - 1].prev	=	&m_drops [MAX_NUM_SNOW - 2];
	m_drops [MAX_NUM_SNOW - 1].next	=	NULL;

	m_emptyDrop	=	&m_drops [0];

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::Setup	(	float		in_areaSize,
								vec3_t		in_velocity,
								int			in_ratio,
								D3DCOLOR	in_color	)
{
	ChangeAreaSize ( in_areaSize );
	ChangeDropColor ( in_color );
	ChangeDropRatio ( in_ratio );
	ChangeVelocity ( in_velocity );

	Reset ();

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::ChangeAreaSize	(	float	in_areaSize	)
{
	m_areaSize	=	in_areaSize;

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::ChangeDropColor	(	D3DCOLOR	in_color	)
{
	m_color	=	in_color;

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::ChangeDropRatio	(	int		in_ratio	)
{
	m_dropCount		=	in_ratio;
	m_generateRatio	=	1.0f / in_ratio;

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::ChangeVelocity	(	vec3_t	in_velocity	)
{
	m_velocity [0]	=	in_velocity [0];
	m_velocity [1]	=	in_velocity [1];
	m_velocity [2]	=	in_velocity [2];

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::InitDeviceObjects	()
{

	m_VB.Create ( MAX_NUM_SNOW * sizeof (SNOWVERTEX), GTH_SNOW_FORMAT, GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER );

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::RestoreDeviceObjects	()
{
	 
	m_snowShaderRef	=	g_texMng.Add ( "characters/tex_effect/snowflake", OWNER_TYPE_CHARACTER );

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::FrameMove	()
{
	m_elapsedTime		=	m_timer ->GetElapsedTime ();
	m_elapsedMiliTime	=	m_timer ->GetElapsedMilliTime ();

	if	(	m_elapsedTime	>	0.1f	)	m_elapsedTime	=	0.1f;
	
	CheckDropGenerating ();
	UpdateDrops ();
	UpdateBuffer ();

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::CheckDropGenerating	()
{
	float	curTime		=	m_timer ->GetAppMilliTime ();
	float	generate	=	m_elapsedTime / m_generateRatio + m_remainder;

	m_numGenerating	=	(int)generate;
	m_remainder		=	generate - (float)m_numGenerating;

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::UpdateDrops	()
{
	if	(	CheckDeadDrops ()	==	E_FAIL	)
		return	E_FAIL;
	if	(	UpdatePosition ()	==	E_FAIL	)
		return	E_FAIL;
	if	(	MakeDrop ()	==	E_FAIL	)
		return	E_FAIL;

	m_numGenerating	=	0;

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::UpdatePosition	()
{
	SNOW_t	*entity	=	m_firstDrop;
	vec3_t	disp;

	float	randVal	=	rand () / (float)RAND_MAX;
	randVal	+=	( rand () / (float)RAND_MAX );
	randVal	*=	0.5f;

	if	(	randVal	<	0.005f	)
	{
		 
		m_velocityMod [0]	-=	1.0f;
	}
	else if (	randVal	>	0.995f	)
	{
		 
		m_velocityMod [0]	+=	1.0f;
	}

	disp [0]	=	( m_velocity [0] + m_velocityMod [0] ) * m_elapsedTime;
	disp [1]	=	( m_velocity [1] + m_velocityMod [1] ) * m_elapsedTime;
	disp [2]	=	( m_velocity [2] + m_velocityMod [2] ) * m_elapsedTime;

	while	(	entity	)
	{
		VectorAdd ( entity ->pos, entity ->pos, disp );
	
		entity	=	entity ->next;
	}

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::CheckDeadDrops	()
{
	SNOW_t	*entity	=	m_firstDrop;

	while	(	entity	)
	{
		if ( entity ->pos[2]	<	( m_origin[2]  -	(*m_charHeight) )	)
		{
			entity	=	DeleteDrop ( entity );
			if	(	! entity	)
				break;
			else
				continue;
		}

		entity	=	entity ->next;
	}

	return	S_OK;
}

 
 
 
 
SNOW_t	*CSnowDropper::DeleteDrop	(	SNOW_t	*in_drop	)
{
	int		propCount	=	0;
	SNOW_t	*ptr_return;

	 
	if	(	in_drop ->prev	==	NULL	)
		propCount	+=	1;

	 
	if	(	in_drop ->next	==	NULL	)
	{
		propCount	+=	2;
		ptr_return	=	NULL;
	}
	else
	{
		ptr_return	=	in_drop ->next;
	}

	switch	(	propCount	)
	{
	case	0:
		 
		in_drop ->prev ->next 	=	in_drop ->next;
		in_drop ->next ->prev	=	in_drop ->prev;
		break;
	case	1:
		 
		in_drop ->next ->prev	=	NULL;
		m_firstDrop	=	in_drop ->next;
		break;
	case	2:
		 
		in_drop ->prev ->next	=	NULL;
		break;
	case	3:
		 
		m_firstDrop	=	NULL;
		break;
	}

	in_drop	->prev	=	NULL;
	in_drop ->next	=	NULL;

	 
	if	(	! m_emptyDrop	)
	{
		m_emptyDrop	=	in_drop;
	}
	else
	{
		in_drop ->next	=	m_emptyDrop;
		m_emptyDrop ->prev	=	in_drop;
		m_emptyDrop	=	in_drop;
	}

	return	ptr_return;
}

 
 
 
 
HRESULT	CSnowDropper::MakeDrop	()
{
	int	index;
	SNOW_t		*emptyDrop;
	vec3_t		disp;

	VectorNormalize2 ( disp, m_velocity );

	for	(	index	=	0;	\
			index	<	m_numGenerating;	\
			++index		)
	{
		 
		if	(	! m_emptyDrop	)
		{
			 
			return	E_FAIL;
		}
		else
			emptyDrop	=	m_emptyDrop;

		m_emptyDrop	=	m_emptyDrop ->next;
		if	(	m_emptyDrop	)
			m_emptyDrop ->prev	=	NULL;

		if	(	m_firstDrop	)
		{
			emptyDrop ->next	=	m_firstDrop;
			m_firstDrop ->prev	=	emptyDrop;
			m_firstDrop	=	emptyDrop;
		}
		else
		{
			emptyDrop ->next	=	NULL;
			emptyDrop ->prev	=	NULL;
			m_firstDrop	=	emptyDrop;
		}

		emptyDrop ->pos [0]	=	( ( rand () / (float)RAND_MAX ) - 0.5f ) * m_areaSize	+	m_origin[0];
		emptyDrop ->pos [1]	=	( ( rand () / (float)RAND_MAX ) - 0.5f ) * m_areaSize	+	m_origin[1];
		 

		emptyDrop ->pos [2]	=	m_origin[2]	-	(*m_charHeight)	+	INITIAL_DROP_HEIGHT;
			
	}

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::UpdateBuffer	()
{
	int		counter	=	0;
	SNOW_t	*drop	=	m_firstDrop;

	while	(	drop	)
	{
		m_dropBuffer [counter].pos.x	=	drop ->pos [0];
		m_dropBuffer [counter].pos.y	=	drop ->pos [1];
		m_dropBuffer [counter].pos.z	=	drop ->pos [2];
		counter++;

		drop	=	drop ->next;
	}

	m_VB.Lock ( counter );

	memcpy ( m_VB.GetBuffer (), m_dropBuffer, counter * sizeof(SNOWVERTEX) );

	m_VB.Unlock ();

	m_drawCount	=	counter;

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::DeleteDeviceObjects	()
{
	m_VB.Release ();
	
	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::Render	()
{
	m_D3DRender ->SetVertexShader ( GTH_SNOW_FORMAT );
	m_D3DRender ->SetStreamSource ( 0, m_VB.GetD3DVertexBuffer (), sizeof (SNOWVERTEX) );
	m_D3DRender ->SetTexture ( 0, g_texMng.GetD3DTexture( m_snowShaderRef ) );

	LPDIRECT3DDEVICE8	pd3dDevice	=	g_d3dDevice.GetD3DDevice();

	pd3dDevice ->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pd3dDevice ->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	pd3dDevice ->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pd3dDevice ->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	pd3dDevice ->SetRenderState (	D3DRS_ZWRITEENABLE,		FALSE	);
	pd3dDevice ->SetRenderState (	D3DRS_ALPHABLENDENABLE,	TRUE					);
	pd3dDevice ->SetRenderState (	D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA		);
	pd3dDevice ->SetRenderState (	D3DRS_DESTBLEND,		D3DBLEND_DESTALPHA	);

	pd3dDevice ->SetRenderState (	D3DRS_POINTSIZE,			FtoDW(5.0f)	);
	pd3dDevice ->SetRenderState (	D3DRS_POINTSPRITEENABLE,	true	);
	pd3dDevice ->SetRenderState (	D3DRS_POINTSCALEENABLE,		true	);
	pd3dDevice ->SetRenderState (	D3DRS_POINTSIZE_MIN,		FtoDW(0.0f)	);
	pd3dDevice ->SetRenderState (	D3DRS_POINTSIZE_MAX,		FtoDW(500.0f)	);
	pd3dDevice ->SetRenderState (	D3DRS_POINTSCALE_A,			FtoDW(0.00f)	);
	pd3dDevice ->SetRenderState (	D3DRS_POINTSCALE_B,			FtoDW(0.00f)	);
	pd3dDevice ->SetRenderState (	D3DRS_POINTSCALE_C,			FtoDW(1.00f)	);

	pd3dDevice ->SetTransform ( D3DTS_WORLD, &m_matWorld );

	pd3dDevice ->DrawPrimitive ( D3DPT_POINTLIST, m_VB.GetBase (), m_drawCount );

	pd3dDevice ->SetRenderState (	D3DRS_POINTSPRITEENABLE,	false	);
	pd3dDevice ->SetRenderState (	D3DRS_POINTSCALEENABLE,		false	);

	return	S_OK;
}

 
 
 
 
HRESULT	CSnowDropper::FinalCleanup	()
{
	delete	this;

	return	S_OK;
}

 
 
 
 
 
HRESULT	CSnowDropper::InvalidateDeviceObjects	()
{
	m_VB.Release ();

	return	S_OK;
}