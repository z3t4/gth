



#if !defined(AFX_CAMERA_H__7F4A10A7_1E7F_4FA6_97B6_01AEE4AEDCC8__INCLUDED_)
#define AFX_CAMERA_H__7F4A10A7_1E7F_4FA6_97B6_01AEE4AEDCC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "../math/math.h"
#include "bsp.h"

#define CAMERA_VIEWTYPE_PLAYER_BACK   0x00000000
#define CAMERA_VIEWTYPE_PLAYER_FRONT  0x00000001
#define CAMERA_VIEWTYPE_PLAYER_RIGHT  0x00000002
#define CAMERA_VIEWTYPE_PLAYER_LEFT   0x00000004
#define CAMERA_VIEWTYPE_QUATER_VIEW   0x00000010
#define CAMERA_VIEWTYPE_TOP_VIEW      0x00000020

#define GTH_PROJECT_NEAR_PLANE 10.0f

class CCamera  
{
public:

	CBSP* m_world;   

	matrix4x4_t  m_projMat;
    matrix4x4_t  m_viewMat;
	matrix4x4_t  m_worldMat;
	matrix4x4_t  m_transMat;
	matrix4x4_t  m_envMat;
	
	matrix4x4_t  m_invProjMat;
	matrix4x4_t  m_invViewMat;
	matrix4x4_t  m_invWorldMat;
	matrix4x4_t  m_invTransMat;
	

    vec3_t   m_position;
	vec3_t   m_angles;
	vec3_t   m_forward;
	vec3_t   m_right;
	vec3_t   m_up;
	float    m_distance;
	int      m_isThirdPerson;
	int      m_autoFollow;

	float    m_characterAngle;
	int      m_viewType;

	float    m_minCamDist;
	float    m_maxCamDist;

	vec3_t   m_characterPos;

	CFrustum  m_frustum;

	
	projectionparam_t m_projectParm;

 	         CCamera();
	virtual ~CCamera();

	void SetWorldModel( CBSP *world ) { m_world = world; }
	void SetupVectors() { AngleVectors( m_angles , m_forward , m_right , m_up ); }
	void SetMinCameraDistance( float minDist )  { m_minCamDist = minDist; }
	void SetMaxCameraDistance( float maxDist )  { m_maxCamDist = maxDist; }

	void SetPosition( vec3_t pos )    { VectorCopy( m_position , pos ); }
	void SetAngles( vec3_t angles );

	void SetupMatrices( int isFrustumSetup );
	void SetFovY(double fovy, double aspect, double znear, double zfar);

	void SetupViewPort( int x , int y , int width , int height , float minZ , float maxZ );

	void  AdjustCameraAngle( vec3_t pos , vec3_t angles , float elapsedTime );
	void  AdjustCamera( vec3_t pos , vec3_t angles );
	int   CheckCameraPos( vec3_t pos , float dist , vec3_t angles );
	void  Zoom( int value );
	void  RotateAngles( vec3_t rotate );
	inline void  AdjustAngles();

	void  SetAuto( int autoFollow ) { m_autoFollow = autoFollow; }
	int   IsAuto() { return m_autoFollow; }

	matrix4x4_t *GetTransformMatrix()          { return &m_transMat; }
	matrix4x4_t *GetInverseTransformMatrix()   { return &m_invTransMat; }
	matrix4x4_t *GetEnvironmentMatrix()        { return &m_envMat; }

	void GetFwdVector( vec3_t vec );
	void GetRightVector( vec3_t vec );
    void GetUpVector( vec3_t vec );
	void Reset();

	void SetViewType( int type );
	void SetProjectParameter( float znear , float zfar );
};

extern CCamera g_camera;

#endif 
