#define STRICT





#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <basetsd.h>
#include <tchar.h>

#include "global_def.h"
#include "resource.h"

#include "GTH.h"
#include "game/GolryunBattle/GolryunBattle.h"










itementity_t *g_mouseSelectedItemObject;
Fx_CHARACTER_t *g_mouseSelectedCharacter;


int g_stop = false;
int g_freeMove = 0;
int g_viewSide = 0;
int g_timetype = 0;
int g_volume = 0;

extern int testSyncNum;




HHOOK	g_hHook;

char gszInstallFolder[MAX_PATH]="";



LRESULT CALLBACK LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)
{
	
	KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
	BOOL bControlKeyDown = 0;



	switch (nCode)
	{
		
		case HC_ACTION:
		{
			
			bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
			
			if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
			{

				return 1;
			}
			
			if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
			{

				return 1;
			}
			
			if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
			{

				return 1;
			}
			
			if( pkbhs->vkCode == VK_LWIN )
			{

				return 1;
			}
			if( pkbhs->vkCode == VK_RWIN )
			{

				return 1;
			}
			if( pkbhs->vkCode == VK_APPS )
			{

				return 1;
			}

			break;
		}

		default:

			break;
	}

	
	return CallNextHookEx (g_hHook, nCode, wParam, lParam);
	

}


BOOL InstallKeyHookProc( HINSTANCE hInstance )
{
	

	g_hHook = SetWindowsHookEx( WH_KEYBOARD_LL , LowLevelKeyboardProc , hInstance , 0 );

	if( g_hHook == NULL )
	{
		return FALSE;
	}

	return TRUE;
}



#ifdef _TERRA
#include ".\Terra ICT\CTerraCtrl.h"
CTerraCtrl gcTerraCtrl;
#endif








#include "GM(Game Master)\CGMCtrl.h"
CGMCtrl* gcpGMCtrl=NULL;













INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR lpszCmdParam, INT )
{
	
	

	GetCurrentDirectory(MAX_PATH,gszInstallFolder);

	
	
	if( !InstallKeyHookProc( hInst ) )
		SystemParametersInfo( SPI_SCREENSAVERRUNNING , TRUE , NULL , 0 );
	

	
	#ifdef _AHNLAB_HACKSHIELD
	if(FALSE == gcAhnLabHackShieldCtrl.ServiceStart()){
		assert(FALSE);
		MessageBox(NULL, "HackShield Error","AhnLab's HackShield", MB_OK);
		return 0;
	}
	#endif
	
	

	
	
	

	

	
	
	
	g_cgv.isDeveloper = true;				
	g_cgv.isUpdatePatch = false;			
	//lucky 2012 Launcher ADD
	if( !strstr( lpszCmdParam, "/xhKdu78838J" ) )
	{
		MessageBox(NULL, "Please run GTH from the launcher GTH.exe","error", MB_OK);
		return 0;
	}
	//end

	//lucky 2012 start only 1 GTH client . 
	HANDLE winmutex = CreateMutex(NULL,FALSE,"Gate to Heavens");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		
		MessageBox(NULL, "Gate to Heavens is already started","Notice", MB_OK);
		return 0;
	}
	//end
	

	GTH_Setup_LoadConfig( "gthconfig.cfg" );

	g_resCounter	=	new	CResCounter();
	g_resCounter->DisableResCounter();
#ifdef	_DEBUG
	if( strstr( lpszCmdParam, "/rescount" ) )
	{
		g_resCounter->EnableResCounter();
	}
#endif
	g_resCounter->LockResCounter();

    GTH d3dApp;

    g_pApp  = &d3dApp;
    g_hInst = hInst;

	 
	g_musicMng->EnableMusicMng();
	 
	g_musicMng->LockMusicMng();

#ifdef _GTH_ONLINE_VERSION
	g_cgv.state = CLIENT_START_PROCESS;
#else
	GTH_InitClientGlobalVariable();
	 

	g_cgv.state = CLIENT_LOADING_PROCESS;
#endif
	 
	g_cgv.currentWorldIdx = -1;
	

    if( !d3dApp.InitEngine( hInst ) )
	{
		return 0;
	}



	

	#if defined(_DEBUG) && defined(_AHNLAB_HACKSHIELD)
	gcAhnLabHackShieldCtrl.LPackTest();
	#endif
	



	
	g_timer.Start();
	
	g_musicMng->Initialize( g_pApp->m_hWnd );

	GTH_LoadOptionFromFile();

	
	if( !g_input->CreateDevice() ) return( 0 );
	if( !g_input->CreateKeyDevice( DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY ) ) return 0;
	if( !g_input->CreateMouseDevice( DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY ) ) return 0;



	
	d3dApp.InitIME();	

	

    d3dApp.Run();

	
#ifdef _GTH_ONLINE_VERSION
	GTH_ShutdownGame();
#endif

	#ifdef _AHNLAB_HACKSHIELD
	gcAhnLabHackShieldCtrl.ServiceEnd();
	#endif




	return 0;
}


int GTH::InitEngine( HINSTANCE hInst )
{
	if( !PrepareApp() )
		return false;

	if( FAILED( Create( hInst ) ) )
		return false;

	assert(NULL != g_cpNProtect);


	
	
#ifdef _GTH_ONLINE_VERSION
	
	#ifdef _GTH_ENABLE_GOODNOTICE
		if ( !GTH_StartUpGame(NONE_MASTERSERVER_CONNECT) )		return false;
	#else
		if ( !GTH_StartUpGame() )		return false;
	#endif
#endif

	

	return true;
}



int GTH::InitIME()
{
	
	
	
	int nKLList = GetKeyboardLayoutList( 0, NULL );
	HKL *pHKLList = new HKL[nKLList];
	GetKeyboardLayoutList( nKLList, pHKLList );

	int ret = 1;

	for( int i=0 ; i<nKLList ; i++ )
	{
		if( !ActivateKeyboardLayout( pHKLList[i], KLF_SETFORPROCESS ) )
		{
			ErrorLog( "ActivateKeyboardLayout Failed(%d)", pHKLList[i]); 
			ret = 0;
		}
	}

	ActivateKeyboardLayout( pHKLList[0], KLF_SETFORPROCESS );

	delete[] pHKLList;

	m_IMEReady = true;

	return ret;
	
	return 1;
}


GTH::GTH()
{
    m_dwCreationWidth  = 1024;
    m_dwCreationHeight = 768;
    m_strWindowTitle   = TEXT( "GTH" );
    m_bUseDepthBuffer  = TRUE;

    m_pD3DXFont        = NULL;
    m_bLoadingApp      = TRUE;

    
    

	m_numWorldBSPs     = 0;
	m_numMapInfos      = 0;
	m_currWorld        = 0;
	m_frameFlag        = 0;



	gcpGMCtrl = new CGMCtrl();
	if(NULL == gcpGMCtrl) return;
	if(FALSE == gcpGMCtrl->isOK()){ delete gcpGMCtrl; gcpGMCtrl=NULL; return; }


	g_modelShaderMng = new CShaderMng;
	g_charShaderMng  = new CShaderMng;





	g_move           = new CMove;
	g_entityMng      = new CEntityMng;
	g_itemMng        = new CItemMng;
	g_effectMng      = new CEffectMng;



	g_cursor         = new CCursor;
	g_timeSystem     = new CTimeSystem;
	g_musicMng       = new CMusicMng;

	input            = new input_t;   

	g_charManager    = new FX_CCharManager;
	g_chatManager	 = new CChatManager; 
	g_soundMng		 = new CSoundMng;

	g_input			 = new CInputMng;

	
	g_ifMng			 = new CIFManager;

	
	g_cpGuildCtrl	 = new CGuildCtrl();
	

	g_TutorialMng	 = new CTutorialMng;
	
	
	
	
	
	memset ( m_worldBSPs, 0, sizeof(m_worldBSPs) );
	m_MDLMng	=	NULL;

	 
	memset( m_mapInfos, 0, sizeof(m_mapInfos) );

	
	m_IMEReady = false;
	//lucky 2012 Windowed all gth .
	m_bFullScreen		= false;
	m_bWindowed			= true;
	//end



#if defined _GTH_CHINESE_VERSION || _GTH_TERRA_VERSION || _GTH_TAIWAN_VERSION || _GTH_MALAYSIA_CHN_VERSION 

	HDC hDC;
	hDC = GetDC( NULL );
	GetDeviceGammaRamp( hDC, &m_ramp );
	ReleaseDC( NULL, hDC );

	

#endif


}


GTH::~GTH()
{


#if defined _GTH_CHINESE_VERSION || _GTH_TERRA_VERSION || _GTH_TAIWAN_VERSION || _GTH_MALAYSIA_CHN_VERSION 

	if( m_bChangeDisplay ) ChangeDisplaySettingsEx( NULL ,NULL, NULL, 0, NULL );

	HDC hDC;
	hDC = GetDC( NULL );
	SetDeviceGammaRamp( hDC, &m_ramp );
	ReleaseDC( NULL, hDC );

#endif

	int i = 0;
	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :
		
	case CLIENT_LOGIN_PROCESS :
		
	case CLIENT_CHARACTER_PROCESS :
		
	case CLIENT_LOADING_PROCESS :
		

	
	case CLIENT_GAME_PROCESS : 

		for( i = 0 ; i < m_numWorldBSPs ; i++ )
			GTH_SAFE_DELETE( m_worldBSPs[ i ] );
		GTH_SAFE_DELETE( m_MDLMng );

		break;
	}


	if(NULL != gcpGMCtrl){ delete gcpGMCtrl; gcpGMCtrl=NULL; }

	GTH_SAFE_DELETE( g_modelShaderMng );
	GTH_SAFE_DELETE( g_charShaderMng );





	GTH_SAFE_DELETE( g_move );
	GTH_SAFE_DELETE( g_entityMng );
	GTH_SAFE_DELETE( g_itemMng );
	GTH_SAFE_DELETE( g_effectMng );



	GTH_SAFE_DELETE( g_cursor );
	GTH_SAFE_DELETE( g_timeSystem );
	GTH_SAFE_DELETE( g_musicMng );

	GTH_SAFE_DELETE( g_chatManager );

	g_ifMng->FinalCleanup();
	GTH_SAFE_DELETE( g_ifMng );

	
	if(NULL != g_cpGuildCtrl){ delete g_cpGuildCtrl; g_cpGuildCtrl=NULL; }
	

	g_charManager ->Cleanup ();
	GTH_SAFE_DELETE( g_charManager );
	GTH_SAFE_DELETE( g_soundMng );

	GTH_SAFE_DELETE( input );

	
	g_input->FreeMouseDevice();
	g_input->FreeKeyDevice();
	g_input->FreeDevice();
	GTH_SAFE_DELETE( g_input );
	GTH_SAFE_DELETE( g_TutorialMng );

	
	

	 
	g_resCounter->PrintOut();


}


void GTH::InitGlobalClass()
{



	g_shaderMng.SetOwner( OWNER_TYPE_BSP );
	
	
	
	g_modelShaderMng->SetOwner( OWNER_TYPE_MODEL );
	
	
	
	 

	g_charShaderMng->SetOwner( OWNER_TYPE_CHARACTER );
	
	
	


	
	g_move->SetTimer( &g_timer );

	g_entityMng->SetScript( &g_script );
	g_entityMng->SetTimer( &g_timer );
	g_entityMng->SetCamara( &g_camera );
	g_entityMng->SetMover( g_move );
	g_entityMng->SetCharacterMng( g_charManager );
	g_entityMng->SetFrustum( &(g_camera.m_frustum) );
	g_itemMng->SetScript( &g_script );
	g_itemMng->SetFrustum( &(g_camera.m_frustum) );
	g_effectMng->SetScript( &g_script );
	g_effectMng->SetFrustum( &(g_camera.m_frustum) );
	g_cursor->SetEffectMng( g_effectMng );
	g_timeSystem->SetRenderer( &g_d3dRender );
	g_charManager->SetFileManager( &g_fileMng );
	g_charManager->SetTexManager( &g_texMng );
	g_charManager->SetScript( &g_script );
	g_charManager->SetTimer( &g_timer );
	g_charManager->SetShaderMng( g_charShaderMng );
	g_charManager->SetRenderer( &g_d3dRender );
	g_charManager ->SetFrustum (	&(g_camera.m_frustum)	);


	g_chatManager->SetFileMng( &g_fileMng );		

	g_ifMng->InitDeviceObjects();
	g_ifMng->SetFileManager( &g_fileMng );
	g_ifMng->SetScript( &g_script );
	g_ifMng->SetTexManager( &g_texMng );

	g_TutorialMng->SetFileMng(&g_fileMng);		
	g_LPACK.SetFileManager(&g_fileMng);			

	g_effectMng->SetNumberCtl( g_ifMng->m_numCtl );
}




#include "CGameVersion.h"
CGameVersion* g_GameVersion = NULL;
int GTH::PrepareApp()
{
	char* token;
	int ret;

	InitGlobalClass();

	
#ifdef _GTH_ZIP_VERSION_PACK
	ret = g_fileMng.PreparePackFiles(ZIP_VERSION_PACK);
#else
	ret = g_fileMng.PreparePackFiles(I3D_VERSION_PACK);
#endif

	if( !ret )
		return false;

	g_charManager->LoadAllResources();
	
	ret = LoadShaders();
	if( !ret )	return false;

	g_script.Load( "start.cfg" );
	g_script.BeginParse();
	token = g_script.GetNextToken( true );
	strcpy( g_cgv.worldInfo, token );

	if ( !GTH_LoadWorldTable() ) return false;


		
	
	if ( !GTH_ParseGameConfig() ) return false;

	
	g_LPACK.Init();

	#ifdef _AHNLAB_HACKSHIELD
	if(FALSE == gcAhnLabHackShieldCtrl.ServiceStart()){				
		return 0;
	}
	#endif






	
	g_GameVersion = new CGameVersion;
	if ( g_GameVersion == NULL)
	{
		MessageBox( NULL, "Create Gameversion Error", "Error", MB_OK );
		return false;
	}
	strncpy(g_cgv.gameVersion, g_GameVersion->m_szMasterServerVersion, VERSIONSTRING);
	g_cgv.gameVersion[VERSIONSTRING-1] = NULL;
	
	




	
	if( !LoadAllGameTable() ) return 0;

	
	GTH_GenerateMyCharacter();

	check.timer.Start();


#ifdef _GTH_MALAYSIA_CHN_VERSION
	g_chatManager->SetChat(GTH_LANGUAGE_TYPE_SIMPLIFIED_CHINESE, 1 );
#else
	#if defined _GTH_CHINESE_VERSION || _GTH_TERRA_VERSION || _GTH_TAIWAN_VERSION  
		g_chatManager->SetChat( g_cgv.languageType, 1 );
	#else		
		g_chatManager->SetChat( g_cgv.languageType, 0 );
	#endif
#endif
	
	if( !g_ifMng->LoadConfig( "if.cfg" ) ) 
	{
		MessageBox( NULL, "Interface config loading fail!!", "Error", MB_OK );
		return false;
	}
	g_ifMng->m_cursor->MakeCursor();

	g_soundMng->LoadSoundConfig( "sound.cfg" );
	 
	g_soundMng ->LoadMDLSoundConfig ( "test.sound.class" );
		



	return true;
}


int GTH::ChangeClientState( int state )
{
	switch( g_cgv.state )
	{ 
	case CLIENT_START_PROCESS :
		ChangeStartProcessBefore();
		g_input->ChangeCursor();
		g_input->Show();
		break;
	case CLIENT_LOGIN_PROCESS :
		break;
	case CLIENT_CHARACTER_PROCESS :
		ChangeCharProcessBefore();
		break;
	case CLIENT_LOADING_PROCESS :
		ChangeLoadingProcessBefore();
		break;
	case CLIENT_GAME_PROCESS :
		ChangeGameProcessBefore();
		break;
	}

	
	g_cgv.state = (clientProcessState_t)state;

	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :
		ChangeStartProcessAfter();
		break;
	case CLIENT_LOGIN_PROCESS :
		ChangeLoginProcessAfter();
		break;
	case CLIENT_CHARACTER_PROCESS :
		ChangeCharProcessAfter();
		break;
	case CLIENT_LOADING_PROCESS :
		ChangeLoadingProcessAfter();
		break;
	
	case CLIENT_GAME_PROCESS :
		ChangeGameProcessAfter();
		break;
	}
	return( 1 );
}


int GTH::RestoreApp()
{
	int before, after, tick;
	before = timeGetTime();
	
	g_texMng.SetTextureQuality( g_userConfig.texture ); 


#if defined _GTH_CHINESE_VERSION || _GTH_TERRA_VERSION || _GTH_TAIWAN_VERSION || _GTH_MALAYSIA_CHN_VERSION 
	g_display.m_gammaRampType = 2;
#else
	g_display.m_gammaRampType = 1;
#endif
	
	g_display.SetupGamma();
	
	 
	g_resCounter->Reset_DataType( RESCOUNTER_TEXTURE_MODEL_MAP );
	g_resCounter->Reset_DataType( RESCOUNTER_TEXTURE_MODEL_LIGHTMAP );
	g_resCounter->Reset_DataType( RESCOUNTER_TEXTURE_MODEL_OTHERS );
	g_resCounter->Reset_DataType( RESCOUNTER_TEXTURE_CHAR_PC );
	g_resCounter->Reset_DataType( RESCOUNTER_TEXTURE_CHAR_NPC );
	g_resCounter->Reset_DataType( RESCOUNTER_TEXTURE_CHAR_MONSTER );
	g_resCounter->Reset_DataType( RESCOUNTER_TEXTURE_INTERFACE );

	 

	g_charManager->m_pAnimManager->LoadActualData();

	if( m_MDLMng == NULL )
	{
		switch( g_cgv.state )
		{
		case CLIENT_START_PROCESS : case CLIENT_LOGIN_PROCESS : case CLIENT_CHARACTER_PROCESS :
			InitStartProcess();
			break;
		case CLIENT_LOADING_PROCESS :
			InitLoadingProcess();
			break;
			
		case CLIENT_GAME_PROCESS : 
			InitGameProcess();
			break;
		}
	}

	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :
		RestoreStartProcess();
		break;
	case CLIENT_LOGIN_PROCESS :
		RestoreLoginProcess();
		break;
	case CLIENT_CHARACTER_PROCESS :
		RestoreCharProcess();
		break;
	case CLIENT_LOADING_PROCESS :
		RestoreLoadingProcess();
		break;
	
	case CLIENT_GAME_PROCESS : 
		RestoreGameProcess();
		break;
	}

	after = timeGetTime();
	tick = after - before;


	return true;
}


int GTH::InitApp()
{
	switch( g_cgv.state ) 
	{
	case CLIENT_START_PROCESS :
		InitStartProcess();
		break;
	case CLIENT_LOGIN_PROCESS :
		InitLoginProcess();
		break;
	case CLIENT_CHARACTER_PROCESS :
		InitCharProcess();
		break;
	case CLIENT_LOADING_PROCESS :
		InitLoadingProcess();
		break;
	
	case CLIENT_GAME_PROCESS : 
		InitGameProcess();
		break;
	}
	return true;
}
void GTH::InitCharacter()	
{
	g_move->SetWorldModel( m_worldBSPs[ 0 ] );



#ifdef _GTH_ONLINE_VERSION
	g_charManager->m_iNumCharacters = 0;
	g_charManager->m_iNumBackupCharacters = 0; 

	
	g_cgv.syncCharacter[0].position[0] += m_worldBSPs[0]->m_models->bbox[0];
	g_cgv.syncCharacter[0].position[1] += m_worldBSPs[0]->m_models->bbox[1];
	g_cgv.syncCharacter[0].position[2] += m_worldBSPs[0]->m_models->bbox[2];
	

	g_charManager->GenerateCharacter(g_cgv.syncCharacter, 1);
	SetMyCharacter();

	GTH_Network_InitNetTime();	
	

	g_cgv.enableChat = true;

	 
	if	(	m_mapInfos [m_myCharacter ->worldIdx].weather	!=	TIME_DAY	)
		g_timeSystem->SetTimeStateEnv ( m_mapInfos [m_myCharacter ->worldIdx].weather );
		

#else
	
	g_charManager->m_iNumCharacters = 0;
	g_charManager->m_iNumBackupCharacters = 0; 
	g_cgv.enableChat = true;
	g_cgv.syncCharacterNumber = 1;
	g_cgv.syncCharacter[0].speed = 10.0f;

	g_charManager->GenerateCharacter(g_cgv.syncCharacter, g_cgv.syncCharacterNumber);
	g_charManager->m_Characters[0].isAlive = true;
	SetMyCharacter();		
#endif
}

void GTH::ApplyTimeEnv()
{ 
	vec3_t pos;

	VectorSet( pos , 0.0f , 0.0f , 0.0f );

	g_timeSystem->SetLightPos( pos );
	g_timeSystem->SetLightDir( m_mapInfos[ 0 ].sunAngle );
	g_timeSystem->ApplyTimeEnv();
}

void GTH::OutputClientState()
{
	g_chatManager->SetDFontColor( 0xffff0000 );
	char temp[255];

#ifdef _GTH_ONLINE_VERSION
	strcpy( temp, "ONLINE - " );
#else
	strcpy( temp, "OFFLINE - " );
#endif
	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :
		strcat( temp, "STATE : CLIENT_START_PROCESS" );
		break;
	case CLIENT_LOGIN_PROCESS :
		strcat( temp, "STATE : CLIENT_LOGIN_PROCESS" );;
		break;
	case CLIENT_CHARACTER_PROCESS :
		strcat( temp, "STATE : CLIENT_CHARACTER_PROCESS" );
		break;
	case CLIENT_LOADING_PROCESS :
		strcat( temp, "STATE : CLIENT_LOADING_PROCESS" );
		break;
	case CLIENT_GAME_PROCESS : 
		strcat( temp, "STATE : CLIENT_GAME_PROCESS" );
		break;
	}
	g_chatManager->DPrint( 10, 10, temp );
}


HRESULT GTH::AppFrame()
{
	OutputClientState();

	m_fElapsedTime = g_timer.Tick();
	m_fTime        = g_timer.GetAppTime();

	
	g_input->ReadKeyImmediateData();
	g_input->ReadMouseImmediateData();


	
	if( g_input->KeyEvent( DIK_LALT ) || g_input->KeyEvent( DIK_RALT ) )
	{
		if( g_input->KeyDown( DIK_TAB ) )
		{

		}
		

	}
#ifndef _GTH_ONLINE_VERSION
	
	if( g_input->KeyEvent( DIK_LCONTROL ) )
	{
		if(g_input->KeyDown(DIK_1))
		{
			g_display.SetBrightness(g_display.GetBrightness() + 0.1f);
			g_display.SetupGamma();
		}
		else if(g_input->KeyDown(DIK_2))
		{
			g_display.SetBrightness(g_display.GetBrightness() - 0.1f);
			g_display.SetupGamma();
		}
		else if(g_input->KeyDown(DIK_3))
		{
			g_display.SetGamma(g_display.GetGamma() + 0.1f);
			g_display.SetupGamma();
		}
		else if(g_input->KeyDown(DIK_4))
		{
			g_display.SetGamma(g_display.GetGamma() - 0.1f);
			g_display.SetupGamma();
		}
		else if(g_input->KeyDown(DIK_5))
		{
			g_display.SetContrast(g_display.GetContrast() + 0.1f);
			g_display.SetupGamma();
		}
		else if(g_input->KeyDown(DIK_6))
		{
			g_display.SetContrast(g_display.GetContrast() - 0.1f);
			g_display.SetupGamma();
		}
		
		
		if(g_input->KeyDown( DIK_R))
		{
			g_display.SetGamma( 1.3f );
			g_display.SetBrightness( 0.6f );
			g_display.SetContrast( 1.0f );
			g_display.SetupGamma();
		}
	}

	g_chatManager->HPrint( 200, 100, 
		"Brightness : %.2f(Ctrl + 1,2), Gamma : %.2f(Ctrl + 3,4), Contrast : %.2f(Ctrl + 5,6) - reset(Ctrl + R)", 
		g_display.GetBrightness(), g_display.GetGamma(), g_display.GetContrast() );
#endif


	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :
		if( StartProcessFrame() == E_FAIL ) return S_OK;
		break;
	case CLIENT_LOGIN_PROCESS :
		if( LoginProcessFrame() == E_FAIL ) return S_OK;
		break;
	case CLIENT_CHARACTER_PROCESS :
		if( CharProcessFrame() == E_FAIL ) return S_OK;
		break;
	case CLIENT_LOADING_PROCESS :
		if( LoadingProcessFrame() == E_FAIL ) return S_OK;
		break;
	case CLIENT_GAME_PROCESS : 
		if( GameProcessFrame() == E_FAIL ) 
		{
			return S_OK;
		}
		break;
	}

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

	
	g_input->UpdateKeyData();
	g_input->UpdateMouseData();
	g_chatManager->Update();


	

	return S_OK;
}

int GTH::AppProcess()
{
	int ret = false;

	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :
		ret = StartProcess();
		break;
	case CLIENT_LOGIN_PROCESS :
		ret = LoginProcess();
		break;
	case CLIENT_CHARACTER_PROCESS :
		ret = CharProcess();

		
		g_charManager->FixCharacterPosition();
		break;
	case CLIENT_LOADING_PROCESS :
		ret = LoadingProcess();
		break;
	case CLIENT_GAME_PROCESS : 
		ret = GameProcess();
		g_charManager->FixCharacterPosition();
	
		break;
	}

	
	if( g_input->KeyDown( DIK_SYSRQ ) )
			MakeScreenShot( m_pd3dDevice, m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height );	

	
	return( ret );
}




void GTH::WorldRenderBefore()
{
	SetWorldTransform();
	 
	if	(	m_renderEnabled	)
	{
		
		
		m_worldBSPs[ m_currWorld ]->RenderBefore();
	}
}

void GTH::WorldRenderAfter()
{
	SetWorldTransform();
	 
	if	(	m_renderEnabled	)
	{
		
		
		m_worldBSPs[ m_currWorld ]->RenderAfter();
	}
}

void GTH::WorldFrame()
{
	if( g_stop ) return;

	
	
	m_worldBSPs[ m_currWorld ]->PrepareScene( g_camera.m_position , m_frameFlag );
}

void GTH::EntityFrame()
{
	m_MDLMng->SetEnvironmentMatrix( g_camera.GetEnvironmentMatrix() );
	m_MDLMng->SetCamera( g_camera.m_position , g_camera.m_angles );
	m_MDLMng->SetVisData( m_worldBSPs[ m_currWorld ]->GetVisData() );
	 
	if	(	m_renderEnabled	)
		m_MDLMng->Render();
}
void GTH::CharacterFrame()
{
	 
	if	(	m_renderEnabled	)
	{
		g_charManager->SetRenderState ();

		g_charManager->RenderItems();
		 
		g_charManager->PrepareSkinRendering();
		g_charManager->RenderSkinsNormal();	
		g_charManager->RenderAuras();
		g_charManager->RenderSkinsDisappear();
		g_charManager->RenderShadow ();
		g_charManager->RenderEffects();
	}

	g_charManager->EndFrame ();
}


void GTH::CursorFrame()
{
}


void GTH::SetupCamera()
{
		 

	g_camera.AdjustCameraAngle( m_myCharacter->position , m_myCharacter->angles , g_timer.GetElapsedTime() );  
	g_camera.AdjustCamera( m_myCharacter->position , m_myCharacter->angles );

	if	(	g_charManager ->m_tbDo	)
	{
		vec3_t	trembleDelta;
		trembleDelta [0]	=	sin ( g_timer .GetAppTime () * 70.0f ) * 5.0f * g_charManager ->m_tbPower;
		trembleDelta [1]	=	cos ( g_timer .GetAppTime () * 68.0f ) * 5.0f * g_charManager ->m_tbPower;
		trembleDelta [2]	=	sin ( g_timer .GetAppTime () * 66.0f ) * cos ( g_timer .GetAppTime () * 64.0f ) * 5.0f * g_charManager ->m_tbPower;

		VectorAdd (	g_camera .m_position,	g_camera .m_position,	trembleDelta	);
	}

	g_camera.SetupMatrices( true );
}
void GTH::SetWorldTransform()
{
	g_d3dRender.SetTransform( D3DTS_WORLD      , &g_camera.m_worldMat );
	g_d3dRender.SetTransform( D3DTS_VIEW       , &g_camera.m_viewMat );
	g_d3dRender.SetTransform( D3DTS_PROJECTION , &g_camera.m_projMat );
}

void GTH::SetMyCharacter()
{
	vec3_t delta;

	VectorClear( delta );
	
	m_myCharacter = &g_charManager->m_Characters[0];

	VectorSet( m_myCharacter->mins , -16.0f , -16.0f , -24.0f );
	VectorSet( m_myCharacter->maxs ,  16.0f ,  16.0f ,  32.0f );
	
	
	
	g_camera.m_angles[ YAW ] = m_myCharacter->angles[ YAW ] + 180;	 
	g_camera.m_distance = -100;	
	

	m_myCharacter->SetupVectors();
	g_move->Pmove( m_myCharacter->position , delta , m_myCharacter->angles );
	
	
	g_charManager->StartFrame();
	g_charManager->UpdateAnimations();

	m_myCharacter->targetIdx  = -1;
	m_myCharacter->targetType = -1;
	m_myCharacter->atk_resend = false;
}

char* g_eventString[] = 
{
	"GTH_EV_CHAR_IDLE",				
	"GTH_EV_CHAR_WALK",				
	"GTH_EV_CHAR_WALKLEFT",			
	"GTH_EV_CHAR_WALKRIGHT",			
	"GTH_EV_CHAR_BACK",				
	"GTH_EV_CHAR_BACKLEFT",			
	"GTH_EV_CHAR_BACKRIGHT",			
	"GTH_EV_CHAR_RUN",				
	"GTH_EV_CHAR_RUNLEFT",			
	"GTH_EV_CHAR_RUNRIGHT",			
	"GTH_EV_CHAR_TURNLEFT",			
	"GTH_EV_CHAR_TURNRIGHT",			
	"GTH_EV_CHAR_MOVELEFT",			
	"GTH_EV_CHAR_MOVERIGHT",			
	"GTH_EV_CHAR_ATTACK",				
	"GTH_EV_CHAR_DIE",				
	"GTH_EV_CHAR_STUN",				
	"GTH_EV_CHAR_DAMAGE",				
	"GTH_EV_CHAR_RESPAWN",			
	"GTH_EV_CHAR_SKIP",				
	"GTH_EV_CHAR_MOUSEWALK",			
	"GTH_EV_CHAR_MOUSERUN",			
	"GTH_EV_CHAR_SIT",
	"GTH_EV_CHAR_STAND",
	"GTH_EV_CHAR_SKILL",
};

char* g_ceventString[] = 
{
	"GTH_CEV_CHAR_IDLE",
	"GTH_CEV_CHAR_ATTACK",
	"GTH_CEV_CHAR_PICKUP_ITEM",
	"GTH_CEV_CHAR_NPC_REQUEST",
	"GTH_CEV_CHAR_PC_REQUEST",
	"GTH_CEV_CHAR_SKILL",
};

HRESULT GTH::FrameMove()
{
	


	
	if( m_myCharacter->isAlive && g_cgv.state == CLIENT_GAME_PROCESS ) 
	{
		SetupCursor_Neo();
		GTH_SetMyCharacterMouseEvent( m_myCharacter );
	}

	
	
	if ( g_cgv.state == CLIENT_GAME_PROCESS)
		g_NewDanBattleSystem.Update();


	float succeccRate = 0.0f;
	int totalCount = g_AttackProc.m_ntempAttackMissCount + g_AttackProc.m_ntempAttackSuccessCount;
	if( totalCount > 0 )
	{
		succeccRate = (float)g_AttackProc.m_ntempAttackSuccessCount / (float)totalCount * 100.0f;
	}

	g_chatManager->DPrint( 300, 100, "Result HitRate : %d, Avoid Rate : %d", g_cgv.myCharacterInfo->resultHitRate, g_cgv.myCharacterInfo->resultAvoidRate );
	g_chatManager->DPrint( 300, 130, "Succuss : %d, Miss : %d, Rate : %.2f", g_AttackProc.m_ntempAttackSuccessCount, g_AttackProc.m_ntempAttackMissCount, succeccRate );
	g_chatManager->DPrint( 200, 0, "resendAttack(%d), saveEvent(%d), atk_changeEvent(%d), atk_replyAnim(%d), atk_resend(%d)",
		g_cgv.resendAttack, g_cgv.saveEvent, m_myCharacter->atk_changeEvent, m_myCharacter->atk_playAnim, m_myCharacter->atk_resend );
	g_chatManager->DPrint( 350, 400, "missCount : %d, minSyncTime : %.2f, maxSyncTime : %.2f",
		g_cgv.missSyncTimeCount, g_cgv.minSyncTime, g_cgv.maxSyncTime );
	g_chatManager->DPrint( 350, 408, "saveEvent : %d, event : %d", g_cgv.saveEvent, g_pApp->m_myCharacter->event );

    return S_OK;
}

 
 
 
 
 
int	GTH::ChangeResolution	(	int		in_mode		)
{
	 
    D3DAdapterInfo	*pAdapterInfo	=	&m_Adapters [m_dwAdapter];
    D3DDeviceInfo	*pDeviceInfo	=	&pAdapterInfo ->devices [pAdapterInfo ->dwCurrentDevice];
    D3DModeInfo		*pModeInfo		=	&pDeviceInfo ->modes [pDeviceInfo ->dwCurrentMode];

	 
	D3DModeInfo		newMode;
	memcpy ( &newMode, pModeInfo, sizeof(D3DModeInfo) );

	switch	(	in_mode	)
	{
	case	GTH_RES_800_600:
		newMode.Width	=	800;
		newMode.Height	=	600;
		break;
	case	GTH_RES_1024_768:
		newMode.Width	=	1024;
		newMode.Height	=	768;
	case	GTH_RES_1280_1024:
		newMode.Width	=	1280;
		newMode.Height	=	1024;
	case	GTH_RES_1600_1200:
		newMode.Width	=	1600;
		newMode.Height	=	1200;
	}

	 
	D3DModeInfo		*thisMode;
	int	found	=	false;
	for	(	int	index	=	0;	\
				index	<	pDeviceInfo ->dwNumModes;	\
				index++		)
    {
		thisMode	=	&pDeviceInfo ->modes [index];

		if	(	0	==	memcmp ( thisMode, &newMode, sizeof(D3DModeInfo) )	)
		{
			found	=	index;
			break;
		}
    }

	if	(	found	)
	{
		pDeviceInfo ->dwCurrentMode	=	found;
	}
	else
	{
		return	false;
	}

	InvalidateDeviceObjects ();
    DeleteDeviceObjects ();

    
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

	HRESULT	hr;

    
    
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

    
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

	g_ifMng ->ChangeResolution ( newMode.Width, newMode.Height );

	return	true;
}

 
 
 
 
 
void	GTH::ToggleFullScreen	()
{
	if	(	! m_bWindowed		)
	{
		ChangeResolution ( 0 );
	}

	CD3DApplication::ToggleFullscreen ();
}


