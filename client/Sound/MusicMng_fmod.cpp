#include "../global_def.h"




#ifdef	_FMOD_SOUND

CSoundSample::CSoundSample()
{
	memset( m_name, 0, sizeof( m_name ) );
	m_idxSound		=	-1;
	m_is3D			=	false;
	m_isPlaying		=	false;
	m_isLoaded		=	false;
	m_channel		=	-1;
	m_sample		=	NULL;
	m_size			=	0;
}
CSoundSample::~CSoundSample()
{
	Dispose();
	Clear();
}

bool CSoundSample::Create( char *fname, int iSize )
{
	m_size = iSize;
	strcpy( m_name, fname );

	if( m_is3D )
	{
		m_sample	=	FSOUND_Sample_Load( FSOUND_FREE, m_name, FSOUND_HW3D | FSOUND_LOOP_OFF, 0, 0 );
		FSOUND_Sample_SetMinMaxDistance( m_sample, 2.0f, 1500.0f );
	}
	else
	{
		m_sample	=	FSOUND_Sample_Load( FSOUND_FREE, m_name, FSOUND_2D | FSOUND_LOOP_OFF, 0, 0 );
	}
	if( m_sample ) return true;

	return false;
}

bool CSoundSample::Recreate()
{
	if( m_is3D )
	{
		m_sample	=	FSOUND_Sample_Load( FSOUND_FREE, m_name, FSOUND_HW3D | FSOUND_LOOP_OFF, 0, 0 );
		FSOUND_Sample_SetMinMaxDistance( m_sample, 2.0f, 1500.0f );
	}
	else
	{
		m_sample	=	FSOUND_Sample_Load( FSOUND_FREE, m_name, FSOUND_2D | FSOUND_LOOP_OFF, 0, 0 );
	}

	if( m_sample ) return true;

	return false;
}

CMusicMng::CMusicMng()
{
	m_inited			=	false;
	m_inited3D			=	false;
	m_supporteax2		=	false;
	m_supporteax3		=	false;
	m_numSamples		=	0;
	m_numSounds			=	0;
	
	m_bBgm				=	true;
	m_bEffect			=	true;
	m_bEnable			=	true;

	m_bgmVolume			=	100;
	m_effectVolume		=	128;
	m_bgHandle			=	-1;
	m_BGMStream			=	NULL;

	m_enabledState		=	false;
	m_lockedState		=	false;

	m_bgVolumePercentage		=	100;
	m_effectVolumePercentage	=	100;
	m_bgmDefaultVolume			=	60;
	m_effectDefaultVolume		=	128;

	memset( m_bgFilename, 0, sizeof( m_bgFilename ) );
}

CMusicMng::~CMusicMng()
{
	ReleaseAllSamples();
}

int	CMusicMng::Initialize( HWND in_hwnd )
{
	if( !Enabled() ) return	true;

	unsigned int	devcaps		=	0;

	if( FSOUND_GetVersion() < FMOD_VERSION )
    {
		m_inited	=	false;
		m_inited3D	=	false;

        return	false;
    }
	
	
	FSOUND_SetHWND( in_hwnd );
	FSOUND_SetOutput( FSOUND_OUTPUT_DSOUND );
	FSOUND_SetDriver( 0 );

	
	FSOUND_GetDriverCaps ( 0, &devcaps );

	
	if(	devcaps & FSOUND_CAPS_HARDWARE ) m_inited3D		=	true;

    if(	devcaps & FSOUND_CAPS_EAX2	) m_supporteax2	=	true;

    if( devcaps & FSOUND_CAPS_EAX3	) m_supporteax3	=	true;

	FSOUND_SetMixer( FSOUND_MIXER_AUTODETECT );
	FSOUND_SetMinHardwareChannels( 4 );
	
	
	FSOUND_SetMaxHardwareChannels( 32 );			
	FSOUND_3D_SetDistanceFactor( 1.0f );
	
	FSOUND_3D_SetRolloffFactor( 0.75f );

	
	if(	!FSOUND_Init( 44100, 32, 0 ) )
    {
		m_bEnable	=	false;
		m_inited	=	false;
		m_inited3D	=	false;

        return	false;
    }

	m_bEnable	=	true;
	m_inited	=	true;

	
	FSOUND_Stream_SetBufferSize( 1000 );

	
	ConfigGameSamples();
	ConfigMDLSamples();

	
	m_resManager.Create( 1024 * 1024 * 5 );

	
	LoadSamples();

	return	true;
}

int CMusicMng::PlayBGMusic(	char *in_musicName )
{
	char		error[MAX_PATH];

	if(	!Enabled() ) return true;
	if( !m_bEnable ) return false;

	
	if( !stricmp( m_bgFilename, in_musicName ) && m_bgHandle != -1 ) return true;

	if( m_bgHandle != -1 )	StopBGMusic();
		
	m_BGMStream = FSOUND_Stream_Open( in_musicName, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_LOOP_NORMAL, 0, 0 );

	if(	!m_BGMStream )
    {
		sprintf( error, "CMusicMng::PlayBGMusic - %s\n", FMOD_ErrorString ( FSOUND_GetError () ) );
		CloseBGSample();
        return false;
    }

	strcpy( m_bgFilename, in_musicName );

	
	if( m_bBgm )
	{
		m_bgHandle	=	FSOUND_Stream_PlayEx( FSOUND_FREE, m_BGMStream, NULL, TRUE );
		FSOUND_SetPaused( m_bgHandle, FALSE );
	}
	else
	{
		m_bgHandle	=	-1;
	}

	g_musicMng->RefreshBgmVolume();
	g_musicMng->RefreshEffectVolume();
	
	return	true;
}

int	CMusicMng::ChangeBGMusic( char *in_musicName )
{
	PlayBGMusic( in_musicName );

	return true;
}

void CMusicMng::StopBGMusic()
{
	if( !Enabled() ) return;
	if( !m_bEnable ) return;

	FSOUND_Stream_Stop( m_BGMStream );
	FSOUND_Stream_Close( m_BGMStream );

	m_bgHandle = -1;
}

void CMusicMng::SetBgm( bool bBgm )
{
	if( !Enabled() ) return;
	
	m_bBgm = bBgm;
	if( !m_bEnable ) return;

	if( m_bgHandle != -1 )
		FSOUND_SetPaused( m_bgHandle, !m_bBgm );

	if( m_bBgm )
	{
		if( m_bgHandle == -1 )
		{
			if ( m_BGMStream )
			{
				m_bgHandle	=	FSOUND_Stream_PlayEx( FSOUND_FREE, m_BGMStream, NULL, TRUE );
				FSOUND_SetPaused( m_bgHandle, FALSE );
			}
		}
	}
}

void CMusicMng::ReleaseAllSamples()
{
	if( !Enabled() ) return;
	if( !m_bEnable ) return;

	CloseBGSample();

	for( int i = 0;	i < MAX_SIZE_SOUND;	i ++ )
	{
		if(	!m_samples[i].m_isLoaded	)	continue;
		if(	!m_samples[i].m_sample	)	continue;

		m_samples[i].Dispose();

	}
}

void CMusicMng::Stop()
{
	if(	!Enabled()	) return;
	if( !m_bEnable ) return;

	for( int i = 0; i < MAX_SIZE_SOUND;	i ++ )
	{
		if(	!m_samples [i].m_isLoaded	)	continue;
		if(	!m_samples [i].m_sample	)	continue;

		Stop( i );
	}
}

int CMusicMng::Stop( int in_index )
{
	if(	!Enabled()	) return true;
	if( !m_bEnable ) return false;

	if( !m_inited ) return	true;

	if( m_samples [in_index].m_sample	)
		FSOUND_StopSound( m_samples[in_index].m_channel );

    return true;
}

void CMusicMng::CloseBGSample()
{
	if(	!Enabled()	) return;
	if( !m_bEnable ) return;

	if( m_BGMStream )
	{
		FSOUND_Stream_Stop( m_BGMStream );
		FSOUND_Stream_Close( m_BGMStream );
	}
}

void CMusicMng::SetBGMusicVolume( int in_volume )
{
	if( !Enabled() ) return;	
	if( !m_bEnable ) return;

	FSOUND_SetVolumeAbsolute( m_bgHandle, in_volume );
}


void CMusicMng::SetVolume( int in_masterVolume )
{
	if( !Enabled() ) return;	
	if( !m_bEnable ) return;	

	FSOUND_SetSFXMasterVolume( in_masterVolume );
}

void CMusicMng::UpdateListener( vec3_t position, vec3_t velocity, vec3_t orientation	 )
{
	if( !Enabled() ) return;	
	if( !m_bEnable ) return;	

	vec3_t	convertPos;

	convertPos[0] = position[0] / 100.0f;
	convertPos[1] = 0.0f;
	convertPos[2] = position[1] / 100.0f;

	static vec3_t oldPos;

	
	{
		

        
        
        

		FSOUND_3D_Listener_SetAttributes (	&convertPos [0], 
											&velocity [0],
											
											orientation [0], 0.0f, orientation [1],
											
											0.0f, 1.0f, 0.0f );

		oldPos[0] = convertPos[0];
		oldPos[1] = convertPos[1];
		oldPos[2] = convertPos[2];

		
		
		
	}
}

int CMusicMng::IsPlaying( int idxSample )
{
	if( !Enabled() ) return false;
	if( !m_bEnable ) return false;

	if(	!m_samples[idxSample].m_isPlaying ) return false;
	
	return true;
}

void CMusicMng::PlaySample(	int idxSample, GTH_SOUND_PLAY_TYPE play )
{	
	if( !Enabled() ) return;
	if( !m_bEnable ) return;	
	if( !m_bEffect ) return;
	if( !m_inited ) return;

	
	vec3_t			pos;
	CSoundSample	*thisSample	=	GetSample( idxSample );	

	if( !thisSample ) return;
	if( !thisSample->m_isLoaded ) return;

	VectorClear( pos );

	if( thisSample->m_is3D && m_inited3D )
	{

		PlaySample3D ( idxSample, play, pos );
		return;
	}

	if( IsPlaying( idxSample ) )
	{
		FSOUND_StopSound ( thisSample->m_channel );
	}

	thisSample->m_channel	=	FSOUND_PlaySound( FSOUND_FREE, thisSample->m_sample );
	if(	thisSample->m_channel < 0 ) thisSample->m_isPlaying	=	false;
	else thisSample->m_isPlaying	=	true;

	if(	play ==	GTH_SOUND_PLAY_LOOP	)
		FSOUND_SetLoopMode( thisSample->m_channel, FSOUND_LOOP_NORMAL );
	else
		FSOUND_SetLoopMode( thisSample->m_channel, FSOUND_LOOP_OFF );
}

void CMusicMng::PlaySample3D( int idxSample, GTH_SOUND_PLAY_TYPE play, vec3_t pos )
{
	if( !Enabled() ) return;
	if( !m_bEnable ) return;	
	if( !m_bEffect ) return;
	if( !m_inited ) return;

	CSoundSample	*thisSample	=	GetSample( idxSample );	
	vec3_t			convertPos;

	if( !thisSample ) return;
	if( !thisSample->m_isLoaded ) return;

	


	 
	thisSample->m_channel	=	FSOUND_PlaySoundEx ( FSOUND_FREE, thisSample->m_sample, 0, TRUE );
	if( thisSample->m_channel < 0 )
	{
		thisSample->m_isPlaying	=	false;
		return;
	}
	convertPos[0]	=	pos[0] / 100.0f;
	convertPos[1]	=	0.0f;
	convertPos[2]	=	pos[1] / 100.0f;

	thisSample->m_isPlaying	=	true;
	
	if( !FSOUND_3D_SetAttributes( thisSample->m_channel, convertPos, NULL ) )
	{
		char	error[ MAX_PATH ];
		sprintf( error, "%s\n", FMOD_ErrorString( FSOUND_GetError() ) );
		OutputDebugString( error );
	}

	if( play == GTH_SOUND_PLAY_LOOP	)
		FSOUND_SetLoopMode( thisSample->m_channel, FSOUND_LOOP_NORMAL );
	else
		FSOUND_SetLoopMode( thisSample->m_channel, FSOUND_LOOP_OFF );

	if(	!FSOUND_SetPaused( thisSample->m_channel, FALSE ) )
	{
		char	error[ MAX_PATH ];
		sprintf( error, "%s\n", FMOD_ErrorString( FSOUND_GetError() ) );
		ShowError( error, __FILE__, __LINE__ );
	}
}


int	CMusicMng::LoadSamples	()
{
	if(	!Enabled() ) return true;
	if( !m_bEnable ) return false;
	if(	!m_inited ) return true;
	
	int	i;

	
	for( i = 0; i < m_numSamples; i ++ )
	{
		if( !m_inited3D ) m_samples[i].m_is3D = false;

		if( RegisterSample( m_samples[i].m_name, (RHANDLE)&m_samples[i], &m_samples[i], RES_INIT_NOTCREATE ) )
		{
			m_samples [i].m_idxSound	=	m_numSounds++;
			m_samples [i].m_isLoaded	=	true;
		}
		else
		{
			m_samples [i].m_isLoaded	=	false;
		}
		m_samples [i].m_isPlaying	=	false;

	}

	return true;
}

void CMusicMng::LockMusicMng()
{

	
	if(	m_lockedState )	return;
	m_lockedState	=	true;
}

void CMusicMng::EnableMusicMng()
{
	if(	m_lockedState )	return;
	m_enabledState	=	true;
}

void CMusicMng::DisableMusicMng()
{
	if ( m_lockedState ) return;
	
	m_enabledState	=	false;
}

#endif		 