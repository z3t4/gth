 
 
 
 
 
 
 
 
 

 
 
 
#ifndef	_FX_RAIN_DROPPER_H_ALKDJASD903294SF
#define _FX_RAIN_DROPPER_H_ALKDJASD903294SF

 
 
 

 
 
 
typedef struct
{
	D3DXVECTOR3		pos;
	D3DCOLOR		color;
}	DROPVERTEX;

#define	GTH_RAIN_FORMAT	( GTH_FORMAT_XYZ | GTH_FORMAT_DIFFUSE )

typedef struct	DROPS_
{
	vec3_t	start;
	vec3_t	end;

	DROPS_	*prev;
	DROPS_	*next;

}	DROPS_t;

 

#define	MAX_NUM_DROPS	2000
	
#define	INITIAL_DROP_HEIGHT	500.0f

 
 
 
class	CRainDropper
{
	 
public:
	CRainDropper	();
	virtual	~CRainDropper	();

	 
	float		m_areaSize;
	vec3_t		m_velocity;
	float		m_lengthMin;
	float		m_lengthMax;
	float		m_generateRatio;	 
	int			m_dropCount;
	D3DCOLOR	m_color;

	int			m_numGenerating;
	float		m_remainder;
	float		m_elapsedMiliTime;
	float		m_elapsedTime;

	DROPS_t			m_drops [MAX_NUM_DROPS];
	DROPVERTEX		m_dropBuffer [MAX_NUM_DROPS * 2];
	int				m_numUsedDrops;
	DROPS_t			*m_firstDrop;
	DROPS_t			*m_emptyDrop;

	CD3DRender		*m_D3DRender;
	CTimer			*m_timer;

	CD3DBuffer		m_VB;

	int				m_drawCount;

	D3DXMATRIX		m_matWorld;

	float			*m_origin;
	float			*m_charHeight;

	 
	void	Reset	();
		 
	HRESULT	Setup	(	float	in_areaSize,	vec3_t	in_velocity,	float	in_lengthMin,	float	in_lengthMax,	int		in_ratio,
						D3DCOLOR	in_coior	);
	HRESULT	ChangeAreaSize	(	float	in_areaSize	);
	HRESULT	ChangeVelocity	(	vec3_t	in_velocity	);
	HRESULT	ChangeLength	(	float	in_min,	float	in_max	);
		 
	HRESULT	ChangeDropRatio	(	int		in_ratio	);
	HRESULT	ChangeDropColor	(	D3DCOLOR	in_color	);
	HRESULT	CheckDropGenerating	();
	HRESULT	UpdateDrops	();
	HRESULT	UpdatePosition	();
	HRESULT	UpdateBuffer ();
	HRESULT	CheckDeadDrops	();
	DROPS_t*	DeleteDrop	(	DROPS_t	*in_drop	);
	HRESULT	MakeDrop	();
	HRESULT	GenerateEmptyDropLink	();

	void	SetRenderer	(	CD3DRender	*d3dRender	)	{	m_D3DRender	=	d3dRender;	};
	void	SetTimer	(	CTimer		*timer		)	{	m_timer		=	timer;	};

	void	SetOrigin	(	float	*in_origin	)		{	m_origin		=	in_origin;	};
	void	SetCharHeight	(	float	*in_height	)	{	m_charHeight	=	in_height;	};

	HRESULT	OneTimeSceneInit	();
    HRESULT	InitDeviceObjects	();
    HRESULT	RestoreDeviceObjects	();
    HRESULT	FrameMove	();
    HRESULT	Render	();
	 
    HRESULT	InvalidateDeviceObjects	();
    HRESULT	DeleteDeviceObjects	();
    HRESULT	FinalCleanup	();

	inline DWORD	FtoDW (		FLOAT	f	)
	{
		return	*( (DWORD*)&f );
	}
};

#endif