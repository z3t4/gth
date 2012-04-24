#ifndef	_MUSIC_MANAGER_FMOD_H_LKDAS95DDSL20GS
#define	_MUSIC_MANAGER_FMOD_H_LKDAS95DDSL20GS




#define		MAX_SIZE_SOUND    600

typedef enum
{
	GTH_SOUND_PLAY_ONCE	=	0,
	GTH_SOUND_PLAY_LOOP,
	GTH_SOUND_PLAY_TIME
}	GTH_SOUND_PLAY_TYPE;

class CSoundSample	:	public BaseResource
{
public :

	CSoundSample();
	virtual ~CSoundSample();

	char			m_name[GTH_MAX_SIZE_FILENAME];
	int				m_idxSound;
	int				m_is3D;
	int				m_isPlaying;		
	int				m_isLoaded;
	int				m_channel;
	FSOUND_SAMPLE	*m_sample;
	int				m_size;

	
	void			Clear()
	{
		m_sample		=	NULL;
	}
	
	bool			Create( char *fname, int iSize );
	bool			Register( char *fname, int iSize )
	{	
		m_size	=	iSize;	
		return true ;
	}

	virtual bool	Recreate();
	virtual void	Dispose()						
	{	
		if( m_sample ) FSOUND_Sample_Free( m_sample );
		m_sample	=	NULL;
	}
	size_t			GetSize()						
	{	
		if( m_sample ) return m_size;
		else return 0;
	}
	bool			IsDisposed()
	{
		if( m_sample ) return false;
		else return true;
	}
};



class CMusicMng
{

public:

	
	FSOUND_STREAM	*m_BGMStream;
	char			m_bgFilename[GTH_MAX_SIZE_FILENAME];
	
	int				m_bgHandle;
	int				m_numSounds;		 
	int				m_numSamples;		 

	int				m_inited;
	int				m_inited3D;
	int				m_supporteax2;
	int				m_supporteax3;

	CSoundSample	m_samples [MAX_SIZE_SOUND];

	bool			m_bEffect;
	bool			m_bBgm;
	bool			m_bEnable;

	int				m_bgmVolume;
	int				m_effectVolume;

	bool			m_enabledState;
	bool			m_lockedState;

	int				m_bgVolumePercentage;
	int				m_effectVolumePercentage;

	int				m_bgmDefaultVolume;
	int				m_effectDefaultVolume;

	CResManager		m_resManager;

	 
public:
	CMusicMng	();
	~CMusicMng	();

	 
public:
	int		Initialize(	HWND in_hwnd );
	void	CloseBGSample();
	int		PlayBGMusic( char *in_musicName );
	void	StopBGMusic();
	int		ChangeBGMusic( char *in_musicName );
	void	SetBGMusicVolume( int in_volume );
	void	SetVolume( int in_masterVolume );
	void	UpdateListener( vec3_t position, vec3_t	velocity, vec3_t orientation );
	void	Stop();
	int		Stop( int in_index );
	int		LoadSamples();
	void	ReleaseAllSamples();

	int		IsPlaying( int idxSample );
	void	PlaySample(	int idxSample, GTH_SOUND_PLAY_TYPE play );
	void	PlaySample3D( int idxSample, GTH_SOUND_PLAY_TYPE play, vec3_t pos );
	void	InitBGMusic()	{};
	
	void		SetBgm( bool bBgm );
	inline void	SetEffect( bool bEffect )	{ m_bEffect = bEffect; }
	inline bool GetBgm()					{ return m_bBgm; }
	inline bool GetEffect()					{ return m_bEffect; }
	inline int	GetBgmVolume()				{ return m_bgmVolume; }
	inline int	GetEffectVolume()			{ return m_effectVolume; }

	inline void	SetBgmVolumeDefault(	int	in_bgVolDefault	)				{	m_bgmVolume					=	in_bgVolDefault;		}
	inline void	SetEffectVolumeDefault(	int	in_effectVolDefault	)			{	m_effectVolume				=	 in_effectVolDefault;	}
	inline void	SetBgmVolumePercentage(	int	in_bgVolPercentage	)			{	m_bgVolumePercentage		=	in_bgVolPercentage;		}
	inline void	SetEffectVolumePercentage(	int	in_effectVolPercentage	)	{	m_effectVolumePercentage	=	in_effectVolPercentage;	}

	inline int	GetBgmVolumePercentage()	{	return	m_bgVolumePercentage;	}
	inline int	GetEffectVolumePercentage()	{	return	m_effectVolumePercentage;	}
	inline void	RefreshBgmVolume()			{	SetBGMusicVolume( int( m_bgmVolume * m_bgVolumePercentage * 0.01f ) );	}
	inline void	RefreshEffectVolume()		{	SetVolume( int( m_effectVolume * m_effectVolumePercentage * 0.01f ) );	}

	void	EnableMusicMng();
	void	DisableMusicMng();
	void	LockMusicMng();
	bool	Enabled()					{	return	m_enabledState;	}

	
	inline bool	RegisterSample( char* fname, RHANDLE rhID, BaseResource *pRes, DWORD initialType )
	{
		FILE		*fp;
		DWORD		size;

		fp = fopen( fname, "rb" );
		if( !fp )
		{
			return false;
		}
		fseek( fp, 0, SEEK_END );
		size	=	ftell( fp );
		fclose( fp );
	
		if( initialType & RES_INIT_NOTCREATE )
		{
			
			if( !m_resManager.RegisterResource( rhID, pRes ) ) return false;
			
			if( !pRes->Register( fname, size ) ) return false;
		}
		else
		{
			
			if( !m_resManager.ReserveMemory( size ) ) return false;	
			
			if( !pRes->Create( fname, size ) ) return false;
			
			if( !m_resManager.InsertResource( rhID, pRes ) ) return false;
		}

		return true;
	}
	inline CSoundSample* GetSample( int idx )
	{
		CSoundSample	*sample		=	&m_samples[idx];
		bool			bIsDisposed	=	sample->IsDisposed();
		
		if( m_resManager.GetResource( (RHANDLE)sample ) == NULL ) 
		{

			return NULL;	
		}
		
		if( bIsDisposed )
		{

		}
		
		return sample;
	}
};

#endif