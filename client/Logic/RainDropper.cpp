 
 
 
 
 
 
 
 
 

 
 
 
#include "../global_def.h"



 
 
 

 
 
 
 
CRainDropper::CRainDropper	()
{
	OneTimeSceneInit ();
}

CRainDropper::~CRainDropper	()
{
}


 
 
 
 
HRESULT	CRainDropper::OneTimeSceneInit	()
{
	m_areaSize		=	0.0f;
	m_velocity [0]	=	0.0f;
	m_velocity [1]	=	0.0f;
	m_velocity [2]	=	0.0f;
	m_lengthMin		=	0.0f;
	m_lengthMax		=	0.0f;
	m_generateRatio	=	0.0f;
	

	memset ( m_drops, 0, sizeof(m_drops) );
	memset ( m_dropBuffer, 0, sizeof(m_dropBuffer) );

	D3DXMatrixIdentity ( &m_matWorld );

	return	S_OK;
}

 
 
 
 
void	CRainDropper::Reset	()
{
	m_numUsedDrops	=	0;
	
	m_remainder		=	0.0f;
	m_numGenerating	=	0;

	m_firstDrop	=	NULL;

	GenerateEmptyDropLink ();
}

 
 
 
 
HRESULT	CRainDropper::GenerateEmptyDropLink	()
{
	m_drops [0].prev	=	NULL;
	m_drops [0].next	=	&m_drops [1];

	for	(	int	index	=	1;	\
				index	<	( MAX_NUM_DROPS - 1 );	\
				++index	)
	{
		m_drops [index].prev	=	&m_drops [index - 1];
		m_drops [index].next	=	&m_drops [index + 1];
	}

	m_drops [MAX_NUM_DROPS - 1].prev	=	&m_drops [MAX_NUM_DROPS - 2];
	m_drops [MAX_NUM_DROPS - 1].next	=	NULL;

	m_emptyDrop	=	&m_drops [0];

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::Setup	(	float		in_areaSize,
								vec3_t		in_velocity,
								float		in_lengthMin,
								float		in_lengthMax,
								int			in_ratio,
								D3DCOLOR	in_color	)
{
	ChangeAreaSize ( in_areaSize );
	ChangeDropColor ( in_color );
	ChangeDropRatio ( in_ratio );
	ChangeVelocity ( in_velocity );
	ChangeLength ( in_lengthMin, in_lengthMax );

	Reset ();

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::ChangeAreaSize	(	float	in_areaSize	)
{
	m_areaSize	=	in_areaSize;

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::ChangeDropColor	(	D3DCOLOR	in_color	)
{
	m_color	=	in_color;

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::ChangeDropRatio	(	int		in_ratio	)
{
	m_dropCount		=	in_ratio;
	m_generateRatio	=	1.0f / in_ratio;

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::ChangeLength	(	float	in_min,	
										float	in_max	)
{
	m_lengthMin	=	in_min;
	m_lengthMax	=	in_max;

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::ChangeVelocity	(	vec3_t	in_velocity	)
{
	m_velocity [0]	=	in_velocity [0];
	m_velocity [1]	=	in_velocity [1];
	m_velocity [2]	=	in_velocity [2];

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::InitDeviceObjects	()
{

	m_VB.Create ( MAX_NUM_DROPS * 2 * sizeof (DROPVERTEX), GTH_RAIN_FORMAT, GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER );

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::RestoreDeviceObjects	()
{
	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::FrameMove	()
{
	m_elapsedTime		=	m_timer ->GetElapsedTime ();
	m_elapsedMiliTime	=	m_timer ->GetElapsedMilliTime ();

	if	(	m_elapsedTime	>	0.1f	)	m_elapsedTime	=	0.1f;
	
	CheckDropGenerating ();
	UpdateDrops ();
	UpdateBuffer ();
	
	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::CheckDropGenerating	()
{
	float	curTime		=	m_timer ->GetAppMilliTime ();
	float	generate	=	m_elapsedTime / m_generateRatio + m_remainder;

	m_numGenerating	=	(int)generate;
	m_remainder		=	generate - (float)m_numGenerating;

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::UpdateDrops	()
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

 
 
 
 
HRESULT	CRainDropper::UpdatePosition	()
{
	DROPS_t	*entity	=	m_firstDrop;
	vec3_t	disp;

	disp [0]	=	m_velocity [0] * m_elapsedTime;
	disp [1]	=	m_velocity [1] * m_elapsedTime;
	disp [2]	=	m_velocity [2] * m_elapsedTime;

	while	(	entity	)
	{
		VectorAdd ( entity ->start, entity ->start, disp );
		VectorAdd ( entity ->end, entity ->end, disp );
		
		entity	=	entity ->next;
	}

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::CheckDeadDrops	()
{
	DROPS_t	*entity	=	m_firstDrop;

	while	(	entity	)
	{
		if ( entity ->end[2]	<	( m_origin[2]  -	(*m_charHeight) )	)
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

 
 
 
 
DROPS_t	*CRainDropper::DeleteDrop	(	DROPS_t	*in_drop	)
{
	int		propCount	=	0;
	DROPS_t	*ptr_return;

	 
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

 
 
 
 
HRESULT	CRainDropper::MakeDrop	()
{
	int	index;
	DROPS_t		*emptyDrop;
	vec3_t		disp;
	float		length;

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

		length	=	m_lengthMin + ( m_lengthMax - m_lengthMin ) * ( rand () / (float)RAND_MAX );

		emptyDrop ->start [0]	=	( ( rand () / (float)RAND_MAX ) - 0.5f ) * m_areaSize	+	m_origin[0];
		emptyDrop ->start [1]	=	( ( rand () / (float)RAND_MAX ) - 0.5f ) * m_areaSize	+	m_origin[1];
		 

		emptyDrop ->start [2]	=	m_origin[2]  -	(*m_charHeight)	+  INITIAL_DROP_HEIGHT;
			

		emptyDrop ->end [0]	=	emptyDrop ->start [0] + disp [0] * length;
		emptyDrop ->end [1]	=	emptyDrop ->start [1] + disp [1] * length;
		emptyDrop ->end [2]	=	emptyDrop ->start [2] + disp [2] * length;
	}

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::UpdateBuffer	()
{
	int		counter	=	0;
	DROPS_t	*drop	=	m_firstDrop;

	while	(	drop	)
	{
		m_dropBuffer [counter].pos.x	=	drop ->start [0];
		m_dropBuffer [counter].pos.y	=	drop ->start [1];
		m_dropBuffer [counter].pos.z	=	drop ->start [2];
		m_dropBuffer [counter].color	=	m_color;
		counter++;

		m_dropBuffer [counter].pos.x	=	drop ->end [0];
		m_dropBuffer [counter].pos.y	=	drop ->end [1];
		m_dropBuffer [counter].pos.z	=	drop ->end [2];
		m_dropBuffer [counter].color	=	m_color;
		counter++;

		drop	=	drop ->next;
	}

	m_VB.Lock ( counter );

	memcpy ( m_VB.GetBuffer (), m_dropBuffer, counter * sizeof(DROPVERTEX) );

	m_VB.Unlock ();

	m_drawCount	=	counter;

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::DeleteDeviceObjects	()
{
	m_VB.Release ();
	
	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::Render	()
{
	m_D3DRender ->SetVertexShader ( GTH_RAIN_FORMAT );
	m_D3DRender ->SetStreamSource ( 0, m_VB.GetD3DVertexBuffer (), sizeof (DROPVERTEX) );

	LPDIRECT3DDEVICE8	pd3dDevice	=	g_d3dDevice.GetD3DDevice();

	pd3dDevice ->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
	pd3dDevice ->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pd3dDevice ->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );

	pd3dDevice ->SetRenderState (	D3DRS_ZWRITEENABLE,		FALSE	);
	pd3dDevice ->SetRenderState (	D3DRS_ALPHABLENDENABLE,	FALSE	);

	pd3dDevice ->SetTransform ( D3DTS_WORLD, &m_matWorld );

	HRESULT	hr;
	hr	=	pd3dDevice ->DrawPrimitive ( D3DPT_LINELIST, m_VB.GetBase (), m_drawCount / 2 );
	if	(	FAILED ( hr )	)
	{
	
		return	E_FAIL;
	}

	return	S_OK;
}

 
 
 
 
HRESULT	CRainDropper::FinalCleanup	()
{
	delete	this;

	return	S_OK;
}

 
 
 
 
 
HRESULT	CRainDropper::InvalidateDeviceObjects	()
{
	m_VB.Release ();

	return	S_OK;
}