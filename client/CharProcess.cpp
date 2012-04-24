#include <windows.h>
#include <basetsd.h>
#include <tchar.h>

#include "global_def.h"
#include "resource.h"

#include "GTH.h"

int GTH::ChangeCharProcessBefore()
{
	int i;

	
	if( g_cgv.restartState != RESTART_RESERVER )
	{

		g_ifMng->InvalidateDeviceObjects();
		g_chatManager->InvalidateFont();
		
		
		for( i = 0 ; i < m_numWorldBSPs ; i++ )
		{
			m_worldBSPs[ i ]->Release();
			GTH_SAFE_DELETE( m_worldBSPs[ i ] );
		}

		m_numWorldBSPs = 0;
		if( m_MDLMng )
			m_MDLMng->Release();
		GTH_SAFE_DELETE( m_MDLMng );
		
		g_shaderMng.Release();
		g_modelShaderMng->Release();
		g_charShaderMng->Release();

		g_charManager->ReleaseBuffers();
		g_timeSystem->InvalidateDeviceObjects();
		g_texMng.Release();		
		g_musicMng->StopBGMusic();
		g_musicMng->Stop();
		g_camera.SetViewType( CAMERA_VIEWTYPE_QUATER_VIEW );
	}
	else
	{
		
		g_cgv.restartState = RESTART_NONE;
	}

	return true;
}

int GTH::ChangeCharProcessAfter()
{
	int i;
	vec3_t delta;
	syncCharacter_t* selectCharacter;
	
	GTH_GenerateMyCharacter();
	VectorClear( delta );
	
	for( i = 0; i < g_cgv.syncCharacterNumber; i ++ )
	{
		selectCharacter = &g_cgv.syncCharacter[i];
		g_move->Pmove( selectCharacter->position , delta , selectCharacter->angles );
	}
	g_charManager->GenerateCharacter( g_cgv.syncCharacter, g_cgv.syncCharacterNumber );

	
	CreateDefaultSelectChar();	
	
	

	
	g_ifMng->m_renderLoadingFlag = false;

	return true;
}

int GTH::RestoreCharProcess()
{
		 
	int i;

	
	g_chatManager->SetDevice( m_pd3dDevice );		
	g_chatManager->CreateFont();
	
	g_ifMng->SetDevice( m_pd3dDevice );
	g_ifMng->PrepareAllControl( m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height );
	g_ifMng->RestoreDeviceObjects();
	
	for( i = 0 ; i < m_numWorldBSPs ; i++ )
		m_worldBSPs[ i ]->Restore();
	
	m_MDLMng->SetDevice( m_pd3dDevice );
	m_MDLMng->Restore( m_pd3dDevice );
	
	g_charManager->SetDevice( m_pd3dDevice );
	g_charManager->PrepareBuffers();
	g_charManager->UpdateCharacterState();
	g_charManager->RestoreDeviceObject();

	g_texMng.Restore();

	g_entityMng->ActivateCameraPath( "cam_start" , "cam_start" , GTH_ENTITY_PLAY_FORWARD );
	g_ifMng ->m_loginWin ->m_cameraStep	=	0;

	
	g_camera.m_projectParm.aspect = (float) m_d3dsdBackBuffer.Width / (float) m_d3dsdBackBuffer.Height;
	g_camera.SetProjectParameter( GTH_PROJECT_NEAR_PLANE , m_worldBSPs[0]->m_visibleDist );
    D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)&g_camera.m_projMat,
		                         deg2rad( g_camera.m_projectParm.fov / g_camera.m_projectParm.aspect ),
								 g_camera.m_projectParm.aspect,
								 g_camera.m_projectParm.znear,
								 g_camera.m_projectParm.zfar);
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

int GTH::InitCharProcess()
{
	return true;
}

int GTH::CharProcessFrame()
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
		if( m_frameFlag )
			EndScene();
		return E_FAIL;
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
	return true;
}


int GTH::CharProcess()
{
	POINT p;
	
	g_charManager ->StartFrame ();
	g_charManager->UpdateAnimations();
	
	g_input->GetCursorPos( p );

	switch( g_ifMng->ProcessInterface() )
	{
	case IF_CTL_EVENT :
		{
			ChangeClientState( CLIENT_LOADING_PROCESS );
			return( 2 );
		}
		break;
	case IF_CTL_EXIT :
		{
			
			
			g_charManager->GenerateCharacter( NULL, 0 );
			g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_SELECT_SERVER );
			GTH_SendMessage_RequestServerGroup();
			ChangeClientState( CLIENT_LOGIN_PROCESS );
			return( 2 );
		}
		break;
	}

	extern int g_freeMove;
	if( g_freeMove )
	{
		if( FAILED( FreeMove() ) )
			return false;
		
		GetKeyState();
	}

	
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
