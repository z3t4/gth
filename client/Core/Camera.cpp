


#include "camera.h"
#include "surfaceflags.h"

#define DEGREE_QUARTER_VIEW  20.0f
#define DEGREE_TOP_VIEW      89.0f

CCamera g_camera;





CCamera::CCamera()
{
	m_distance = -200.0f;
	m_isThirdPerson = true;
	m_autoFollow = true;
	VectorClear( m_position );
	VectorSet( m_angles , DEGREE_QUARTER_VIEW , 0.0f , 0.0f );

	m_projectParm.fov  = 90.0f;
    m_projectParm.znear = GTH_PROJECT_NEAR_PLANE;
    m_projectParm.zfar  = 5000.0f;

	m_minCamDist = -40.0f;
	m_maxCamDist = -950.0f; //lucky Cam distance.

	m_viewType = CAMERA_VIEWTYPE_PLAYER_BACK | CAMERA_VIEWTYPE_QUATER_VIEW;
	m_characterAngle = 0.0f;

	 
	m_world	=	NULL;
}

CCamera::~CCamera()
{
}

void CCamera::SetProjectParameter( float znear , float zfar )
{
	m_projectParm.znear = znear;
    m_projectParm.zfar  = zfar;
}

#define YAW   1
#define PITCH 0

void CCamera::SetupMatrices( int isFrustumSetup )
{
    vec3_t negPos;
	
	SetupVectors();
	m_worldMat.Identity();
	m_viewMat.Identity();
	m_viewMat.m[0]  = m_right[0];
	m_viewMat.m[4]  = m_right[1];
	m_viewMat.m[8]  = m_right[2];

	m_viewMat.m[1]  = m_up[0];
	m_viewMat.m[5]  = m_up[1];
	m_viewMat.m[9]  = m_up[2];

	m_viewMat.m[2]  = m_forward[0];
	m_viewMat.m[6]  = m_forward[1];
	m_viewMat.m[10] = m_forward[2];

	VectorNegate( negPos , m_position );
	m_viewMat.Translate( negPos );

	D3DXMatrixInverse( (D3DXMATRIX*) &m_invProjMat , NULL , (D3DXMATRIX*) &m_projMat );
	D3DXMatrixInverse( (D3DXMATRIX*) &m_invViewMat , NULL , (D3DXMATRIX*) &m_viewMat );
	D3DXMatrixInverse( (D3DXMATRIX*) &m_invWorldMat , NULL , (D3DXMATRIX*) &m_worldMat );

    matrix4x4_t::Multiply( m_envMat , m_worldMat , m_viewMat );
	matrix4x4_t::Multiply( m_transMat , m_envMat , m_projMat );

	matrix4x4_t::Multiply( m_envMat , m_invProjMat , m_invViewMat );
	matrix4x4_t::Multiply( m_invTransMat , m_envMat , m_invWorldMat );

	matrix4x4_t::Multiply( m_envMat , m_viewMat , m_worldMat );

	if( isFrustumSetup )
		m_frustum.Setup( m_angles , m_position , &m_projectParm );
}

void CCamera::SetFovY(double fovy, double aspect, double znear, double zfar)
{ 
	m_projMat.Projection_LH( fovy , aspect , znear , zfar );
}

void CCamera::SetupViewPort( int x , int y , int width , int height , float minZ , float maxZ )
{
	viewport_t *viewport;
	viewport = &m_projectParm.viewport;

	viewport->X = x;
	viewport->Y = y;
	viewport->Width = width;
	viewport->Height = height;
	viewport->MinZ = minZ;
	viewport->MaxZ = maxZ;
}


#define CAMERA_VIEW_HEIGHT 60.0f
#define CAMERA_AUTO_ROTATE_ANGLE 5.0f 

void CCamera::AdjustCameraAngle( vec3_t pos , vec3_t angles , float elapsedTime )
{
	float deltaAngle , rotate;

	if( !m_autoFollow ) return;

	if( m_angles[ YAW ] != angles[ YAW ] && ( m_characterPos[0] != pos[0] || m_characterPos[1] != pos[1] ) )
	{
		rotate = CAMERA_AUTO_ROTATE_ANGLE * elapsedTime;
		deltaAngle = angles[ YAW ] - m_angles[ YAW ];
		if( deltaAngle > 10.0f )
		{
			if( deltaAngle > 190.0f )
			{
				deltaAngle = 360.0f - deltaAngle;
				if( rotate > deltaAngle )   rotate = deltaAngle;
				m_angles[ YAW ] -= rotate;
			}
			else if(  deltaAngle < 170.0f )
			{
				if( rotate > deltaAngle )   rotate = deltaAngle;
				m_angles[ YAW ] += rotate;
			}
		}
		else if( deltaAngle < -10.0f )
		{
			if( deltaAngle < -190.0f )
			{
				deltaAngle = deltaAngle + 360.0f;
				if( rotate > deltaAngle )   rotate = deltaAngle;
				m_angles[ YAW ] += rotate;
			}
			else if(  deltaAngle > -170.0f )
			{
				if( rotate > -deltaAngle )   rotate = deltaAngle;
				m_angles[ YAW ] -= rotate;
			}
		}

		if( m_angles[ YAW ] <  0.0f )     m_angles[ YAW ] += 360.0f;
		if( m_angles[ YAW ] >  360.0f )   m_angles[ YAW ] -= 360.0f;
	}

}

void CCamera::AdjustCamera( vec3_t pos , vec3_t angles )
{
	int   flag;
	vec3_t point;
	
	m_characterAngle = angles[ YAW ];
	VectorCopy( m_characterPos , pos );

	VectorCopy( point , pos );

	point[2] = point[2] + CAMERA_VIEW_HEIGHT;

	if( !m_isThirdPerson )
	{
		VectorCopy( m_position , pos );
		m_position[2] = m_position[2] + CAMERA_VIEW_HEIGHT;
		return;
	}
	

	flag = CheckCameraPos( point , m_distance , m_angles );

	if( !flag )
	{
		Zoom(15*5);
		if( m_distance >= -40.0f ) m_distance = -30.0f;
		

		flag = CheckCameraPos( point , m_distance , m_angles );
	}
}

int CCamera::CheckCameraPos( vec3_t pos , float dist , vec3_t angles )
{
	vec3_t forward , end;
	trace_t trace;
	vec3_t mins , maxs;

	AngleToVector( angles , forward );

	VectorScale( forward , forward , dist );
	VectorAdd( end , pos , forward );

	mins[0] = -10.0f;  mins[1] = -10.0f;  mins[2] = -10.0f;
	maxs[0] =  10.0f;  maxs[1] =  10.0f;  maxs[2] =  10.0f;
	trace = m_world->Trace( pos , mins , maxs , end , MASK_PLAYERSOLID );

	if( trace.fraction == 1.0f )
	{
		VectorCopy( m_position , end );
		return true;
	}
	
	
	if( trace.fraction < 0.2f || trace.fraction * dist < 30.0f )
	{
		VectorScale( forward , forward , 1.0f );
		VectorAdd( m_position , pos , forward );
		return false;
	}
	else if( trace.fraction < 0.5f )
	{
		VectorScale( forward , forward , trace.fraction * 0.8f );
		VectorAdd( m_position , pos , forward );
		return true;
	}
	else
	{
		VectorScale( forward , forward , trace.fraction * 0.9f );
		VectorAdd( m_position , pos , forward );
		return true;
	}
}

void CCamera::Zoom( int value )
{
	float zoomScale;

	zoomScale = (float) value / 5.0f;
	m_distance += zoomScale;

	if( m_distance > m_minCamDist )
		m_distance = m_minCamDist;
	if( m_distance < m_maxCamDist )
		m_distance = m_maxCamDist;
}

void CCamera::RotateAngles( vec3_t rotate ) 
{
	VectorAdd( m_angles , m_angles , rotate );

	AdjustAngles();
}

inline void CCamera::AdjustAngles()
{
	if( m_angles[ PITCH ] < -60.0f )  m_angles[ PITCH ] = -60.0f;
	if( m_angles[ PITCH ] >  89.0f )  m_angles[ PITCH ] = 89.0f;
	if( m_angles[ YAW ] <  0.0f )     m_angles[ YAW ] += 360.0f;
	if( m_angles[ YAW ] >  360.0f )   m_angles[ YAW ] -= 360.0f;
}

void CCamera::SetAngles( vec3_t angles )   
{ 
	VectorCopy( m_angles , angles ); 
	AdjustAngles(); 
}

void CCamera::GetFwdVector( vec3_t vec )
{
	VectorSet( vec , m_forward[0] , m_forward[1] , m_forward[2] );
	
}

void CCamera::GetRightVector( vec3_t vec )
{
	VectorSet( vec , m_right[0] , m_right[1] , m_right[2] );
	
}

void CCamera::GetUpVector( vec3_t vec )
{
	VectorSet( vec , m_up[0] , m_up[1] , m_up[2] );
	
}

void CCamera::Reset() 
{
	VectorClear( m_position );
	VectorSet( m_angles , m_angles[0] , 0.0f , 0.0f );
}


void CCamera::SetViewType( int type )
{
	

	m_viewType = type;

	if     ( m_viewType & CAMERA_VIEWTYPE_PLAYER_BACK )
		m_angles[ YAW ] = m_characterAngle;
	else if( m_viewType & CAMERA_VIEWTYPE_PLAYER_FRONT )
		m_angles[ YAW ] = m_characterAngle + 180.0f;
	else if( m_viewType & CAMERA_VIEWTYPE_PLAYER_RIGHT )
		m_angles[ YAW ] = m_characterAngle + 90.0f;
	else if( m_viewType & CAMERA_VIEWTYPE_PLAYER_LEFT )
		m_angles[ YAW ] = m_characterAngle + 270.0f;

	if     ( m_viewType & CAMERA_VIEWTYPE_QUATER_VIEW )
		m_angles[ PITCH ] = DEGREE_QUARTER_VIEW;
	else if( m_viewType & CAMERA_VIEWTYPE_TOP_VIEW )
		m_angles[ PITCH ] = DEGREE_TOP_VIEW;

	AdjustAngles();
}