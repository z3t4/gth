



#include "../global_def.h"





CTimeSystem::CTimeSystem()
{
	ColorSet( m_material.diffuse  , 1.0f , 1.0f , 1.0f , 1.0f );
	ColorSet( m_material.ambient  , 0.5f , 0.5f , 0.5f , 1.0f );
	ColorSet( m_material.specular , 0.0f , 0.0f , 0.0f , 0.0f );
	ColorSet( m_material.emissive , 0.0f , 0.0f , 0.0f , 0.0f );
	
	ColorSet( m_light.diffuse  , 1.0f , 1.0f , 1.0f , 1.0f );
	ColorSet( m_light.ambient  , 0.5f , 0.5f , 0.5f , 1.0f );
	ColorSet( m_light.specular , 0.0f , 0.0f , 0.0f , 0.0f );

	m_brightness = 0.5f;
	m_gamma = 1.0f;
	m_contrast = 0.7f;

	SetTimeStateEnv( TIME_DAY );

	m_fogColor = 0x00000000;

		 
	m_pRainDropper = NULL;
	m_pSnowDropper = NULL;
		

}

CTimeSystem::~CTimeSystem()
{
}

void CTimeSystem::SetLightPos( vec3_t pos )
{
	VectorCopy( m_light.position , pos );
}

void CTimeSystem::SetLightDir( vec3_t dir )
{
	VectorCopy( m_light.direction , dir );
}

void CTimeSystem::ApplyTimeEnv()
{
	 

	ColorSet(m_material.diffuse, 1.0f, 1.0f, 1.0f, 1.0f);
	ColorSet(m_material.ambient, 0.5f, 0.5f, 0.5f, 1.0f);
	m_D3DRender->SetLight( 0 , &m_light );
	m_D3DRender->SetMaterial( &m_material );

	


}

void CTimeSystem::SetLightAmbientParam( vec4_t ambient )
{
	ColorSet( m_light.ambient , ambient[0] , ambient[1] , ambient[2] , ambient[3] );
}

void CTimeSystem::SetFogParameter(DWORD fogColor, float fogStart, float fogEnd, float zNear, float zFar)
{
	m_fogColor = fogColor;
	m_fogStart = fogStart;
	m_fogEnd   = fogEnd;

	if( m_fogColor )
	{
		m_D3DRender->SetFogParameter(m_fogColor, m_fogStart, m_fogEnd);

	}
}

void CTimeSystem::SetFogEnable()
{
	m_D3DRender->SetFogEnable();
}

void CTimeSystem::SetFogDisable()
{
	m_D3DRender->SetFogDisable();
}

void CTimeSystem::LoadTimeEnv()
{
	char* token;
	g_script.Load( "timesystem.cfg" );

	g_script.BeginParse();

	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( token[0] == 0 ) break;
		if( !stricmp( token, "GAMMA" ) )
		{
			token = g_script.GetNextToken( true );
			m_gamma = (float) atof( token );
		} else if( !stricmp( token, "BRIGHTNESS" ) )
		{
			token = g_script.GetNextToken( true );
			m_brightness = (float) atof( token );
		} else if( !stricmp( token, "CONTRAST" ) )
		{
			token = g_script.GetNextToken( true );
			m_contrast = (float) atof( token );
		} else if( !stricmp( token, "DAY_MATERIAL_DIFFUSE" ) )
		{
			token = g_script.GetNextToken( true );
			m_dayMatDiffuse[0] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatDiffuse[1] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatDiffuse[2] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatDiffuse[3] = (float) atof( token );
		} else if( !stricmp( token, "DAY_MATERIAL_AMBIENT" ) )
		{
			token = g_script.GetNextToken( true );
			m_dayMatAmbient[0] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatAmbient[1] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatAmbient[2] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatAmbient[3] = (float) atof( token );
		} else if( !stricmp( token, "DAY_MATERIAL_SPECULAR" ) )
		{
			token = g_script.GetNextToken( true );
			m_dayMatSpecular[0] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatSpecular[1] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatSpecular[2] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatSpecular[3] = (float) atof( token );
		} else if( !stricmp( token, "DAY_MATERIAL_EMISSIVE" ) )
		{
			token = g_script.GetNextToken( true );
			m_dayMatEmissive[0] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatEmissive[1] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatEmissive[2] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayMatEmissive[3] = (float) atof( token );
		} else if( !stricmp( token, "DAY_LIGHT_DIFFUSE" ) )
		{
			token = g_script.GetNextToken( true );
			m_dayLightDiffuse[0] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightDiffuse[1] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightDiffuse[2] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightDiffuse[3] = (float) atof( token );
		} else if( !stricmp( token, "DAY_LIGHT_AMBIENT" ) )
		{
			token = g_script.GetNextToken( true );
			m_dayLightAmbient[0] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightAmbient[1] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightAmbient[2] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightAmbient[3] = (float) atof( token );
		} else if( !stricmp( token, "DAY_LIGHT_SPECULAR" ) )
		{
			token = g_script.GetNextToken( true );
			m_dayLightSpecular[0] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightSpecular[1] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightSpecular[2] = (float) atof( token );
			token = g_script.GetNextToken( true );
			m_dayLightSpecular[3] = (float) atof( token );
		}
	}
}

void CTimeSystem::SetTimeStateEnv( TIMESTATE state )
{
	m_light.type = DIRECTIONAL_LIGHT;
	VectorCopy( m_light.position  , m_lightPos );
	VectorCopy( m_light.direction , m_lightDir );
	m_light.range     = 1000.0f;
	m_light.falloff   = 1.0f;
	m_light.attenuation0 = 0.0f;
	m_light.attenuation1 = 1.0f;
	m_light.attenuation2 = 0.0f;
	m_light.phi = 0.0f;
	m_light.theta = 0.0f;

	m_timeState = state;

	if     ( state == TIME_DAY )
	{
		
		

	}
	else if( state == TIME_DAWN )
	{
		
		ColorSet( m_material.diffuse  , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.ambient  , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.specular , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.emissive , 0.0f , 0.0f , 0.0f , 0.0f );
		m_material.power = 32.0f;

		ColorSet( m_light.diffuse  , 180.0f / 255.0f , 196.0f / 255.0f , 90.0f / 255.0f , 0.0f );
		ColorSet( m_light.ambient  , 0.0f , 0.0f , 0.0f , 0.0f );
		ColorSet( m_light.specular , 0.0f , 0.0f , 0.0f , 0.0f );

		m_staticColor = 0xffb4c45a;
		m_dynamicColor = 0xffb4c45a;
		m_fogColor  = 0xffe4f48a;
		m_fogStart  = 6000.0f;
		m_fogEnd    = 10000.0f;
	}
	else if( state == TIME_DUSK ){
		
		ColorSet( m_material.diffuse  , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.ambient  , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.specular , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.emissive , 0.0f , 0.0f , 0.0f , 0.0f );
		m_material.power = 32.0f;

		ColorSet( m_light.diffuse  , 196.0f / 255.0f , 121.0f / 255.0f , 66.0f / 255.0f , 0.0f );
		ColorSet( m_light.ambient  , 0.0f , 0.0f , 0.0f , 0.0f );
		ColorSet( m_light.specular , 0.0f , 0.0f , 0.0f , 0.0f );

		m_staticColor = 0xffc47942;
		m_dynamicColor = 0xffc47942;
		m_fogColor  = 0xffc47942;
		m_fogStart  = 1000.0f;
		m_fogEnd    = 10000.0f;
	}
	else if( state == TIME_NIGHT ){
		
		ColorSet( m_material.diffuse  , 0.6f , 0.6f , 0.6f , 0.0f );
		ColorSet( m_material.ambient  , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.specular , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.emissive , 0.0f , 0.0f , 0.0f , 0.0f );
		m_material.power = 32.0f;

		ColorSet( m_light.diffuse  , 51.0f / 255.0f , 51.0f / 255.0f , 51.0f / 255.0f , 0.0f );
		ColorSet( m_light.ambient  , 0.0f , 0.0f , 0.0f , 0.0f );
		ColorSet( m_light.specular , 0.0f , 0.0f , 0.0f , 0.0f );

		m_staticColor = 0xff333333;
		m_dynamicColor = 0xff333333;
		m_fogColor  = 0xff444444;
		m_fogStart  = 6000.0f;
		m_fogEnd    = 10000.0f;
	}
	else if( state == TIME_FOG ){
		
		ColorSet( m_material.diffuse  , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.ambient  , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.specular , 0.4f , 0.4f , 0.4f , 0.0f );
		ColorSet( m_material.emissive , 0.0f , 0.0f , 0.0f , 0.0f );
		m_material.power = 32.0f;

		ColorSet( m_light.diffuse  , 133.0f / 255.0f , 133.0f / 255.0f , 207.0f / 255.0f , 0.0f );
		ColorSet( m_light.ambient  , 0.0f , 0.0f , 0.0f , 0.0f );
		ColorSet( m_light.specular , 0.0f , 0.0f , 0.0f , 0.0f );

		m_staticColor  = 0xff8585cf;
		m_dynamicColor = 0xff8585cf;
		m_fogColor     = 0xffb5b5ff;
		m_fogStart     = 1000.0f;
		m_fogEnd       = 10000.0f;
	}
}

	 
HRESULT CTimeSystem::OneTimeSceneInit()
{
	m_pRainDropper = new CRainDropper();
	m_pSnowDropper = new CSnowDropper();

	m_pRainDropper->SetRenderer ( m_D3DRender );
	m_pSnowDropper->SetRenderer ( m_D3DRender );

	m_pRainDropper->SetTimer ( &g_timer );
	m_pSnowDropper->SetTimer ( &g_timer );

	return S_OK;
}

HRESULT CTimeSystem::InitDeviceObjects()
{
	 
	vec3_t velocity = { 10.0f, 10.0f, -600.0f };
	m_pRainDropper->Setup ( 2000.0f, velocity, 20.0f, 40.0f, 300, 0xffffffff );
	m_pRainDropper->InitDeviceObjects();

	 
	velocity[0] = 0.0f;
	velocity[1] = 0.0f;
	velocity[2] = -150.0f;
	m_pSnowDropper->Setup ( 3000.0f, velocity, 300, 0xffffffff );
	m_pSnowDropper->InitDeviceObjects();

	return S_OK;
}

HRESULT CTimeSystem::RestoreDeviceObjects()
{
	m_pRainDropper->RestoreDeviceObjects();
	m_pSnowDropper->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CTimeSystem::FrameMove()
{
	switch ( m_timeState )
	{
	case TIME_RAIN:
		m_pRainDropper->FrameMove();
		break;
	case TIME_SNOW:
		m_pSnowDropper->FrameMove();
		break;
	}

	return S_OK;
}

HRESULT	CTimeSystem::Render()
{
	switch ( m_timeState )
	{
	case TIME_RAIN:
		m_pRainDropper->Render();
		break;
	case TIME_SNOW:
		m_pSnowDropper->Render();
		break;
	}

	return S_OK;
}

HRESULT CTimeSystem::InvalidateDeviceObjects()
{
	m_pRainDropper->InvalidateDeviceObjects();
	m_pSnowDropper->InvalidateDeviceObjects();

	return S_OK;
}

HRESULT CTimeSystem::DeleteDeviceObjects()
{
	m_pRainDropper->DeleteDeviceObjects();
	m_pSnowDropper->DeleteDeviceObjects();

	return S_OK;
}

HRESULT CTimeSystem::FinalCleanup()
{
	m_pRainDropper->FinalCleanup();
	m_pSnowDropper->FinalCleanup();

	return S_OK;
}

void CTimeSystem::SetDropperOrigin( float* origin )
{
	m_pRainDropper->SetOrigin( origin );
	m_pSnowDropper->SetOrigin( origin );
}

 
void CTimeSystem::SetDropperCharHeight( float* height )
{
	m_pRainDropper->SetCharHeight ( height );
	m_pSnowDropper->SetCharHeight ( height );
}
	

	