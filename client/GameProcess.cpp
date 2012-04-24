#include <windows.h>
#include <basetsd.h>
#include <tchar.h>

#include "global_def.h"
#include "resource.h"

#include "GTH.h"
#include "../game/GolryunBattle/GolryunBattle.h"

int GTH::ChangeGameProcessBefore()
{
	int i;

	


	

	g_chatManager->InvalidateFont();

	
	g_ifMng->InvalidateDeviceObjects();
	
	
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
	
	g_musicMng->Stop( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_HEARTBEAT_0 ) );
	
	
	g_ifMng->m_renderLoadingFlag = false;

	return true;
}

int GTH::ChangeGameProcessAfter()
{
	
	InitApp();
	RestoreApp();
	InitCharacter();

	g_ifMng->m_dlgWin->SetDlgInput( true );
	g_cgv.missSyncTimeCount --;

	
	g_cgv.flagChangeWorld = false;

	g_itemMng->m_MDLMng->SetFocusItemUsage( false );

	return true;
}

int GTH::RestoreGameProcess()
{
	int i;

	
	g_chatManager->SetDevice( m_pd3dDevice );		
	g_chatManager->CreateFont();
	
	g_ifMng->SetDevice( m_pd3dDevice );
	g_ifMng->PrepareAllControl( m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height );
	g_ifMng->RestoreDeviceObjects();
	
	m_MDLMng->SetDevice( m_pd3dDevice );
	
	for( i = 0 ; i < m_numWorldBSPs ; i++ )
		m_worldBSPs[ i ]->Restore();
	
	m_MDLMng->Restore( m_pd3dDevice );
	
	g_charManager->SetDevice( m_pd3dDevice );
	g_charManager->PrepareBuffers();
	g_charManager->UpdateCharacterState();
	g_charManager->RestoreDeviceObject();

	g_timeSystem->InitDeviceObjects();
	g_timeSystem->RestoreDeviceObjects();
	
	g_texMng.Restore();
	
	
	g_ifMng->InitSoundVolume();
	g_musicMng->PlayBGMusic( m_mapInfos[ 0 ].BGMusic );

	
	g_camera.m_projectParm.aspect = (float) m_d3dsdBackBuffer.Width / (float) m_d3dsdBackBuffer.Height;
	g_camera.SetProjectParameter(GTH_PROJECT_NEAR_PLANE,
		                         m_worldBSPs[0]->m_visibleDist);
    D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)&g_camera.m_projMat,
		                        deg2rad( g_camera.m_projectParm.fov / g_camera.m_projectParm.aspect ),
								g_camera.m_projectParm.aspect,
								g_camera.m_projectParm.znear,
								50000.0f);

	g_camera.SetupViewPort(0,
		                   0,
						   m_d3dsdBackBuffer.Width,
						   m_d3dsdBackBuffer.Height,
						   g_camera.m_projectParm.znear,
						   50000.0f);

	g_cursor->SetupViewPort(0,
		                    0,
							m_d3dsdBackBuffer.Width,
							m_d3dsdBackBuffer.Height,
							g_camera.m_projectParm.znear,
							50000.0f);

	g_timeSystem->SetFogParameter(m_mapInfos[ 0 ].fogColor,
		                          m_mapInfos[ 0 ].fogStart,
								  m_mapInfos[ 0 ].fogEnd,
								  g_camera.m_projectParm.znear,
								  g_camera.m_projectParm.zfar);


	
	

	return true;
}


int GTH::InitGameProcess()
{
	int i, ret;

	
	LoadConfig( g_cgv.worldInfo );
	

	
    

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
	 
	g_camera.SetWorldModel ( m_worldBSPs[ m_currWorld ] );
	
	ret = LoadMDLClasses( &m_mapInfos[0] );
	if( !ret )	return false;
	
	ret = LoadEntity( m_mapInfos[0].objectName );
	if( !ret )	return false;
	
	for( i = 0 ; i < m_numWorldBSPs ; i++ )
		m_worldBSPs[ i ]->Initialize( m_mapInfos[ 0 ].worldVisible );

	 

	g_entityMng->Initialize( m_mapInfos[ 0 ].modelVisible );
	
	 

	g_itemMng->Initialize( m_mapInfos[ 0 ].itemVisible );

	 

	g_effectMng->Initialize( m_mapInfos[ 0 ].effectVisible );
	
	g_cursor->Initialize();
	m_MDLMng->Initialize();

	 
	m_MDLMng ->BindSoundSample ();








		
	
	g_camera.Reset();
	m_frameFlag = 0;


	g_TutorialMng->Initialize();					

	return true;
}

int GTH::GameProcessFrame()
{
	check.time0 = check.timer.Tick();

	 






		 
	if	(	m_worldBSPs[ m_currWorld ]	)
	{
		m_worldBSPs[ m_currWorld ]->SetvisibleQuality( g_userConfig.vision ); 
	}
	g_entityMng->SetvisibleQuality( g_userConfig.vision );


	g_TutorialMng->Process();				

	 









	if( m_frameFlag == 2 ) 
	{
		
		GTH_SendMessage_ReadyToPlay();

		
		if ( g_ifMng->m_boothWin->State() )
			g_ifMng->m_boothWin->Enable(FALSE);

		
		if ( g_ifMng->m_stackWin->State() )
		{
			g_ifMng->m_stackWin->m_saveInvenPos = -1;
			g_ifMng->m_stackWin->m_saveItemPrice = 0;
			g_ifMng->m_stackWin->m_stackType = -1;
			g_ifMng->m_stackWin->Enable(FALSE);
		}

		
		

	}

	if( m_frameFlag )
	{
		if( FAILED( BeginScene() ) )
		{
			return E_FAIL;
		}





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
		


		 
		 


		g_ifMng->RenderChar();
		g_ifMng->PrepareRender();
		g_ifMng->Render();
		g_chatManager->Render();

		RenderText();


		
		g_timeSystem->Render();

	}
	check.time6 = check.timer.Tick();
	
	 
	vec3_t	forward;
	
	g_camera.GetFwdVector( forward );

	g_musicMng ->UpdateListener ( m_myCharacter ->position, NULL, forward );

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

	extern int g_viewSide;
	if( g_viewSide == 1 )
	{
		g_camera.m_angles[ YAW ] += 90.0f;
		g_camera.SetupMatrices( false );
		g_camera.m_angles[ YAW ] -= 90.0f;
		g_camera.SetupVectors();
	}
	else if( g_viewSide == 2 )
	{
		g_camera.m_angles[ YAW ] += 180.0f;
		g_camera.SetupMatrices( false );
		g_camera.m_angles[ YAW ] -= 180.0f;
		g_camera.SetupVectors();
	}
	else if( g_viewSide == 3 )
	{
		g_camera.m_angles[ YAW ] -= 90.0f;
		g_camera.SetupMatrices( false );
		g_camera.m_angles[ YAW ] += 90.0f;
		g_camera.SetupVectors();
	}


	return true;
}

int GTH::GameProcess()
{


	
	g_charManager->StartFrame ();
	g_charManager->UpdateAnimations();

	 
	 

#ifndef	_GTH_NO_INTERFACE
	if( g_ifMng->ProcessSelection() ) 
	{
		if( g_input->GetMouseMode() != INPUT_MOUSE_SCROLL_MODE ) g_input->SetMouseMode( INPUT_MOUSE_INTERFACE_MODE );
	}
	else
	{
		if( g_input->GetMouseMode() != INPUT_MOUSE_SCROLL_MODE ) g_input->SetMouseMode( INPUT_MOUSE_NORMAL_MODE );
	}
#endif

	
	switch( g_ifMng->ProcessInterface() )
	{
	case IF_CTL_EVENT :
		ChangeClientState( CLIENT_LOADING_PROCESS );
		return( 2 );
		break;
	case IF_CTL_EXIT :
		GTH_ReturnMasterServer( g_cgv.restartState );
		ChangeClientState( CLIENT_START_PROCESS );
		return( 2 );
		break;
	}


	


	
	
	
	
	













	
	



	
	FrameMove();
	
	
	g_ifMng->ProcessIFKeyState();
	extern int g_freeMove;
	if( g_freeMove )
	{
		if( FAILED( FreeMove() ) )
			return false;	
	}
	else
		
		SetupViewWithKey();



	
#ifdef _GTH_ONLINE_VERSION
	if (g_cgv.onGameServer)
	{
		
		GTH_SendMyCharacterEvent();
		
		
		GTH_MyCharacter_UpdatePosition();
	
		
		GTH_ListenPacket(); 
	}
#endif	
	
	GTH_ExecuteCharacterEvent(); 
	
	GTH_ServerShutdownTimer();

	

	SetupView();

	switch(g_GolryunBattle.GetObserverMode())
	{
	case CGolryunBattle::eObserverMode::general:
		
		
		if ( g_freeMove == FALSE)		
			SetupCamera();		

		break;
	case CGolryunBattle::eObserverMode::observe:
		g_GolryunBattle.SetupObserver();
		break;
	}

	float heartbeat = 1.0f;

	if( m_myCharacter->calMaxRA > 0 && m_myCharacter->curRA > 0 )
	{
		heartbeat = (float)m_myCharacter->curRA / (float)m_myCharacter->calMaxRA;
		if( heartbeat <= 0.2f )
		{
			if( !g_musicMng->IsPlaying( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_HEARTBEAT_0 ) ) )
				g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_HEARTBEAT_0 ), GTH_SOUND_PLAY_ONCE );
		}
	}
	if( heartbeat > 0.2f )
	{
		if( g_musicMng->IsPlaying( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_HEARTBEAT_0 ) ) ) 
			g_musicMng->Stop( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_HEARTBEAT_0 ) );
	}

		 
	g_timeSystem->FrameMove();
		

	return true;
}
