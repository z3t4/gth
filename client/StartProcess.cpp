#include <windows.h>
#include <basetsd.h>
#include <tchar.h>

#include "global_def.h"
#include "resource.h"

#include "GTH.h"

int GTH::ChangeStartProcessBefore()
{
	return S_OK;
}

int GTH::ChangeStartProcessAfter()
{
	
	if ( g_cgv.restartState != RESTART_NONE )
	{
		
		g_ifMng->RestartInterface( 0 );
		InitApp();
		RestoreApp();
	}

	g_charManager->m_drawShadows	=	false;

	return S_OK;
}

int GTH::RestoreStartProcess()
{
	int i;
	vec3_t delta;
	
	g_d3dRender.InitDevice();	
	
	g_chatManager->SetDevice( m_pd3dDevice );
	g_chatManager->CreateFont();
	g_chatManager->InitInput();
	
	g_ifMng->SetDevice( m_pd3dDevice );
	g_ifMng->PrepareAllControl( m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height );
	g_ifMng->RestoreDeviceObjects();
	
	m_MDLMng->SetDevice( m_pd3dDevice );

	for( i = 0 ; i < m_numWorldBSPs ; i++ )
	{
		m_worldBSPs[ i ]->Restore();
	}
	
	m_MDLMng->Restore( m_pd3dDevice );
	
	g_charManager->SetDevice( m_pd3dDevice );
	g_charManager->PrepareBuffers();
	g_charManager->UpdateCharacterState();
	g_charManager->RestoreDeviceObject();
	
	g_texMng.Restore();
	
	g_charManager->m_Characters[0].position[0] = -3.0f;
	g_charManager->m_Characters[0].position[1] = -411.0f;
	g_charManager->m_Characters[0].position[2] = -900.0f;
	g_charManager->m_Characters[0].angles[0] = -22.0f;
	g_charManager->m_Characters[0].angles[1] = 90.0f;
	g_charManager->m_Characters[0].angles[2] = 0.0f;
	
	m_myCharacter = &g_charManager->m_Characters[0];
	
	g_camera.m_angles[ YAW ] = m_myCharacter->angles[ YAW ];
	
	VectorClear( delta );
	
	g_entityMng->ActivateCameraPath( "cam_start" , "cam_start" , GTH_ENTITY_PLAY_FORWARD );

	g_input->Center();

	g_camera.m_projectParm.aspect = (float) m_d3dsdBackBuffer.Width / (float) m_d3dsdBackBuffer.Height;
	g_camera.SetProjectParameter( GTH_PROJECT_NEAR_PLANE , m_worldBSPs[0]->m_visibleDist );
    D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)&g_camera.m_projMat , deg2rad( g_camera.m_projectParm.fov / g_camera.m_projectParm.aspect ) 
		                      , g_camera.m_projectParm.aspect , g_camera.m_projectParm.znear , g_camera.m_projectParm.zfar );
	g_camera.SetupViewPort( 0 , 0 , m_d3dsdBackBuffer.Width , m_d3dsdBackBuffer.Height 
		                   , g_camera.m_projectParm.znear , g_camera.m_projectParm.zfar );

	g_cursor->SetupViewPort( 0 , 0 , m_d3dsdBackBuffer.Width , m_d3dsdBackBuffer.Height 
		                   , g_camera.m_projectParm.znear , g_camera.m_projectParm.zfar );


	g_timeSystem->SetFogParameter(m_mapInfos[ 0 ].fogColor,
		                          m_mapInfos[ 0 ].fogStart,
								  m_mapInfos[ 0 ].fogEnd,
								  g_camera.m_projectParm.znear,
								  g_camera.m_projectParm.zfar);

	return S_OK;
}

int GTH::InitStartProcess()
{
	int i, ret;


	
	LoadConfig( "select.cfg" );
	

	
    
	if(m_mapInfos[0].fogColor)
		g_timeSystem->SetFogEnable();
	else
		g_timeSystem->SetFogDisable();

	for( i = 0 ; i < m_numMapInfos ; i++ )
	{
		ret = LoadMap( m_mapInfos[ i ].mapName );
		if( !ret )	return false;
	}

	 
	g_move->SetWorldModel( m_worldBSPs[ m_currWorld ] );
	g_entityMng->SetWorldModel( m_worldBSPs[ m_currWorld ] );
	g_itemMng->SetWorldModel( m_worldBSPs[ m_currWorld ] );
	g_effectMng->SetWorldModel( m_worldBSPs[ m_currWorld ] );
	g_cursor->SetWorldModel( m_worldBSPs[ m_currWorld ] );
	 
	g_camera .SetWorldModel ( m_worldBSPs[ m_currWorld ] );
	
	ret = LoadMDLClasses( &m_mapInfos[0] );
	if( !ret )	return false;
	
	ret = LoadEntity( m_mapInfos[0].objectName );
	if( !ret )	return false;
	
	for( i = 0 ; i < m_numWorldBSPs ; i++ )
		m_worldBSPs[ i ]->Initialize( m_mapInfos[ 0 ].worldVisible );

	 

	g_entityMng->Initialize( m_mapInfos[ 0 ].modelVisible );

	 

	g_itemMng->Initialize( m_mapInfos[ 0 ].itemVisible );

	 

	g_effectMng->Initialize( m_mapInfos[ 0 ].itemVisible );
	
	g_cursor->Initialize();
	m_MDLMng->Initialize();
	
	g_camera.Reset();
	 


	m_frameFlag = 0;

	return( S_OK );
}

int GTH::StartProcessFrame()
{
	check.time0 = check.timer.Tick();

	 






	 



	
	
	if( m_frameFlag )
	{
		if( FAILED( BeginScene() ) )
			return E_FAIL;
		
		ApplyTimeEnv();
		check.time1 = check.timer.Tick();

		WorldRenderBefore();
		check.time2 = check.timer.Tick();

		 
		CharacterFrame();
		check.time3 = check.timer.Tick();

		EntityFrame();
		check.time4 = check.timer.Tick();

		WorldRenderAfter();
		check.time5 = check.timer.Tick();

		CursorFrame();
		
		
		 
		g_ifMng->PrepareRender();
		g_ifMng->Render();

		RenderText();
		
		g_chatManager->Render(); 

	}
	check.time6 = check.timer.Tick();

	g_entityMng->Process( m_myCharacter->position , g_camera.m_position , m_frameFlag , g_camera.m_distance );
	g_itemMng->Process( g_camera.m_position , m_frameFlag );
	g_effectMng->Process( g_camera.m_position , m_frameFlag );
	
	switch( AppProcess() )
	{
	case false :
		break;
	case 2 :
		if( m_frameFlag )
			EndScene();
		return E_FAIL;
	}
	
	check.time7 = check.timer.Tick();
	
	WorldFrame();
	
	check.time8 = check.timer.Tick();
		
	if( m_frameFlag )
	{
		
		check.time9 = check.timer.Tick();
		EndScene();
		check.time10 = check.timer.Tick();
	}
	
	m_frameFlag++;

	return S_OK;
}

int GTH::StartProcess()
{
	POINT p;
	
	g_charManager ->StartFrame ();
	g_charManager ->UpdateAnimations();

	g_input->GetCursorPos( p );







	if( g_ifMng->ProcessInterface() == IF_CTL_EVENT ) ChangeClientState( CLIENT_LOGIN_PROCESS );




#ifdef _GTH_ONLINE_VERSION
		if (g_cgv.onMasterServer)
		{
			GTH_MasterServer(); 
		}
#endif	
	GetKeyState();
	g_camera.SetupMatrices( true );

		
	return true;
}

