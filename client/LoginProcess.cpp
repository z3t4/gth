#include <windows.h>
#include <basetsd.h>
#include <tchar.h>

#include "global_def.h"
#include "resource.h"

#include "GTH.h"

int GTH::ChangeLoginProcessBefore()
{
	
	g_cgv.restartState = RESTART_NONE;
	return true;
}

int GTH::ChangeLoginProcessAfter() 
{
	
	g_ifMng->InitSoundVolume();
	g_musicMng->PlayBGMusic( m_mapInfos[ 0 ].BGMusic );

	
	if( g_cgv.restartState != RESTART_RESERVER )
	{
		
		if ( g_cgv.restartState == RESTART_SERVER ||  
			g_cgv.restartState == RESTART_CHARACTER )
		{

		}
	}
	else
	{
		

	}

	return true;
}

int GTH::RestoreLoginProcess()
{
	 
	RestoreStartProcess ();
	
	return true;
}

int GTH::InitLoginProcess()
{
	return true;
}

int	GTH::LoginProcessFrame	()
{
	check.time0	=	check.timer.Tick ();

	 










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
	
	switch	(	AppProcess ()	)
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

 












































































int GTH::LoginProcess()
{
	POINT p;
	
	g_charManager ->StartFrame ();
	g_charManager->UpdateAnimations();

	g_input->GetCursorPos( p );

	if( g_ifMng->ProcessInterface() == IF_CTL_EVENT )
	{
		ChangeClientState( CLIENT_CHARACTER_PROCESS );
		return( 2 );
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

