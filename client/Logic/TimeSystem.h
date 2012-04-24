



#if !defined(AFX_TIMESYSTEM_H__E4D42FD7_C3CC_4358_A7D7_14DF03FCA3A4__INCLUDED_)
#define AFX_TIMESYSTEM_H__E4D42FD7_C3CC_4358_A7D7_14DF03FCA3A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


typedef enum 
{
    TIME_DAY       = 0,
    TIME_DAWN      = 1,
    TIME_DUSK      = 2,
	TIME_NIGHT     = 3,
	TIME_FOG       = 4,
		 
	TIME_RAIN      = 5,
	TIME_SNOW      = 6,
		
	TIME_TYPE_NUM,
} TIMESTATE;

#define MAX_SIZE_LIGHT              8
#define MAX_SIZE_MATERIAL           8

#define SUN_LIGHT                        0
#define DIRECTIONAL_STATIC_LIGHT         0
#define DIRECTIONAL_DYMANIC_LIGHT        1
#define POINT_STATIC_LIGHT               5
#define POINT_DYMANIC_LIGHT              8
#define SPOT_STATIC_LIGHT                5
#define SPOT_DYMANIC_LIGHT               8

#define DIRECTIONAL_DYMANIC_LIGHT_SIZE   4
#define POINT_STATIC_LIGHT_SIZE          5
#define POINT_DYMANIC_LIGHT_SIZE         8
#define SPOT_STATIC_LIGHT_SIZE           5
#define SPOT_DYMANIC_LIGHT_SIZE          8

class CTimeSystem  
{
public:
	float      m_time;
	TIMESTATE  m_timeState;
	DWORD      m_staticColor;
	DWORD      m_dynamicColor;
	DWORD      m_fogColor;
	float      m_fogStart;
	float      m_fogEnd;
	light_t    m_light;
	material_t m_material;
	vec3_t     m_lightPos;
	vec3_t     m_lightDir;

	vec4_t		m_dayMatDiffuse;
	vec4_t		m_dayMatAmbient;
	vec4_t		m_dayMatSpecular;
	vec4_t		m_dayMatEmissive;

	vec4_t		m_dayLightDiffuse;
	vec4_t		m_dayLightAmbient;
	vec4_t		m_dayLightSpecular;


	float		m_gamma;
	float		m_contrast;
	float		m_brightness;

	CD3DRender     *m_D3DRender;   

		 
	CRainDropper	*m_pRainDropper;
	CSnowDropper	*m_pSnowDropper;

		

	CTimeSystem();
	virtual ~CTimeSystem();

	void  SetRenderer( CD3DRender *d3dRender )      { m_D3DRender = d3dRender; }

	void SetLightPos( vec3_t pos );
	void SetLightDir( vec3_t dir );
	void ApplyTimeEnv();
	void SetLightAmbientParam( vec4_t ambient );
	void SetTimeStateEnv( TIMESTATE state );
	
	void SetFogParameter(DWORD fogColor, float fogStart, float fogEnd, float zNear, float zFar);

	void SetFogEnable();
	void SetFogDisable();

	
	void	LoadTimeEnv();

		 
	HRESULT	OneTimeSceneInit();
    HRESULT	InitDeviceObjects();
    HRESULT	RestoreDeviceObjects();
    HRESULT	FrameMove();
    HRESULT	Render();
    HRESULT	InvalidateDeviceObjects();
    HRESULT	DeleteDeviceObjects();
    HRESULT	FinalCleanup();
	void	SetDropperOrigin( float* origin );
	 
	void	SetDropperCharHeight( float* height );
		

		
};

#endif 
