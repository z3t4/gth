#include <windows.h>
#include <basetsd.h>
#include <tchar.h>

#include "global_def.h"
#include "resource.h"

#include "GTH.h"

int GTH::ChangeLoadingProcessBefore()
{
	


	
	g_ifMng->InvalidateDeviceObjects();

	g_chatManager->InvalidateFont();
	g_texMng.Release();
	GTH_EnableMove();
	
	
	
	g_charManager->m_drawShadows	=	!g_userConfig.shadow;

	return true;
}

int GTH::ChangeLoadingProcessAfter()
{
	
	InitApp();
	RestoreApp();
	
	g_ifMng->m_dlgWin->SetDlgInput( false );



	return true;
}

int GTH::RestoreLoadingProcess()
{
	
	g_chatManager->SetDevice( m_pd3dDevice );		
	g_chatManager->CreateFont();
	g_ifMng->SetDevice( m_pd3dDevice );
	g_ifMng->PrepareAllControl( m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height );
	g_ifMng->RestoreDeviceObjects();
	
	g_texMng.Restore();

	if ( g_cgv.changeServerFlag )
	{
#ifdef _GTH_ONLINE_VERSION
		GTH_SendMessage_ConnectGameServer();
#endif
	}

	return true;
}

int GTH::InitLoadingProcess()
{
	return true;
}

int GTH::LoadingProcessFrame()
{
	check.time0 = check.timer.Tick();

	if( FAILED( BeginScene() ) )
		return E_FAIL;




	g_chatManager->Render(); 

	
	
	g_ifMng->PrepareRender();
	g_ifMng->Render();

	if( !AppProcess() ) return E_FAIL;
	EndScene();

	return true;
}

int GTH::LoadingProcess()
{


	switch( g_ifMng->ProcessInterface() )
	{
	case IF_CTL_EVENT :
		{
			ChangeClientState( CLIENT_GAME_PROCESS );
			return( 1 );
		}
		break;
	}
	
#ifdef _GTH_ONLINE_VERSION
	if (g_cgv.onGameServer)	GTH_ListenPacket(); 
#endif	
	
	return true;
}

