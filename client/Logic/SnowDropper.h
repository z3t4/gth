 
 
 
 
 
 
 
 
 

 
 
 
#ifndef	_FX_SNOW_DROPPER_H_ALKJDASD0980238JD
#define	_FX_SNOW_DROPPER_H_ALKJDASD0980238JD

 
 
 

 
 
 
typedef struct
{
	D3DXVECTOR3		pos;
	D3DCOLOR		color;
}	SNOWVERTEX;

#define	GTH_SNOW_FORMAT	( GTH_FORMAT_XYZ | GTH_FORMAT_DIFFUSE )

typedef struct	SNOW_
{
	vec3_t	pos;

	SNOW_	*prev;
	SNOW_	*next;

}	SNOW_t;

 

#define	MAX_NUM_SNOW	2500
	
#define	INITIAL_SNOW_HEIGHT	500.0f

 
 
 

class	CSnowDropper
{
	 
public:
	CSnowDropper	();
	virtual	~CSnowDropper	();

	 
	float		m_areaSize;
	vec3_t		m_velocity;
	float		m_generateRatio;	 
	int			m_dropCount;
	D3DCOLOR	m_color;

	vec3_t		m_velocityMod;

	int			m_numGenerating;
	float		m_remainder;
	float		m_elapsedMiliTime;
	float		m_elapsedTime;

	SNOW_t			m_drops [MAX_NUM_SNOW];
	SNOWVERTEX		m_dropBuffer [MAX_NUM_SNOW];
	int				m_numUsedDrops;
	SNOW_t			*m_firstDrop;
	SNOW_t			*m_emptyDrop;

	CD3DRender		*m_D3DRender;
	CTimer			*m_timer;

	CD3DBuffer		m_VB;
	int				m_snowShaderRef;
	
	int				m_drawCount;

	D3DXMATRIX		m_matWorld;

	float			*m_origin;
	float			*m_charHeight;

	 
	void	Reset	();
		 
	HRESULT	Setup	(	float	in_areaSize,	vec3_t	in_velocity,	int		in_ratio,	D3DCOLOR	in_coior	);
	HRESULT	ChangeAreaSize	(	float	in_areaSize	);
	HRESULT	ChangeVelocity	(	vec3_t	in_velocity	);
		 
	HRESULT	ChangeDropRatio	(	int		in_ratio	);
	HRESULT	ChangeDropColor	(	D3DCOLOR	in_color	);
	HRESULT	CheckDropGenerating	();
	HRESULT	UpdateDrops	();
	HRESULT	UpdatePosition	();
	HRESULT	UpdateBuffer ();
	HRESULT	CheckDeadDrops	();
	SNOW_t*	DeleteDrop	(	SNOW_t	*in_drop	);
	HRESULT	MakeDrop	();
	HRESULT	GenerateEmptyDropLink	();

	void	SetRenderer	(	CD3DRender	*d3dRender	)	{	m_D3DRender		=	d3dRender;	};
	void	SetTimer	(	CTimer		*timer		)	{	m_timer	=	timer;	};

	void	SetOrigin	(	float	*in_origin	)		{	m_origin	=	in_origin;	};
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