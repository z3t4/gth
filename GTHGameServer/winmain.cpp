#include "Global.h"
#include <assert.h>
float msTime;

extern unsigned int g_netTime;


extern CSendingQueue *sendingQueue;
extern CReceivingQueue *receivingQueue;




#include "LogEvent.h"
extern CLogEvent gLogEvent;

int		g_debugCount;
int		g_aliveStateCount[5];


extern BOOL InitDanBattleManager();														
extern BOOL InitDanBattleSystem();	
extern void GTH_ProcessDanBattleSys();





#include ".\Tools\CTools.h"
CTools* gcpTools=NULL;

#include ".\GolryunBattle\CServerLpackCtrl.h"
CServerLpackCtrl* gcpServerLpackCtrl=NULL;

#include "Guild/CGuildCtrl.h"
CGuildCtrl* gcpGuildCtrl=NULL;

#include ".\GolryunBattle\CGolryunBattle.h"
CGolryunBattle* gcpGolryunBattle=NULL;

#include "CPremiumPlayerCtrl.h"
CPremiumPlayerCtrl* gcpPremiumPlayerCtrl=NULL;


#include "Terra\Billing\CTerraBillingCtrl.h"
CTerraBillingCtrl* gcpTerraBillingCtrl=NULL;

#include ".\GonryunPracticeBattle\CGonryunPracticeBattle.h"
CGonryunPracticeBattle  g_GonryunPracticeBattle;



#include "Terra\GiftCard\CGiftCard.h"
CGiftCard* gcpGiftCard=NULL;

#include "GM(Game Master)\CGMCtrl.h"
CGMCtrl* gcpGMCtrl=NULL;






#include "AHNLAB\AHNLAB.h"
CAhnLab* g_cpAhnLab=NULL;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	int i;
	MSG msg;
	
	
	MyRegisterClass(hInstance);

	
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		MessageBox(g_hWnd, "InitInstance Failed!!", "Error!", MB_OK);
		return FALSE;
	}
	
	GTH_InitGlobalVariable();

	
	if ( !GTH_ParseGameServerConfig() ) 
		return FALSE;  
	
	if( !GTH_ParseGameLogicConfig() )
		return FALSE;




	



	
	if ( !GTH_InitMemberServer() )
		return FALSE;  

	g_logSystem->Initialize(g_config.createLog, g_config.createDebug);
	if ( !g_logSystem->Open() )
		return FALSE;
	
	gLogEvent.LogEvent(CLogEvent::GTH_EVENTLOG_ERROR_TYPE,"\n\n 서버 시작 \n");

	
	GTH_InitGameServer();

	
	if ( !g_DBAccountServer->Initialize( g_config.accountDatabase, g_config.odbcDSNType ) ) 
	{
		g_logSystem->Write("Account Database Connecting Failure!!");
		MessageBox(g_hWnd, "Account Database Connecting Failure!", "Error!", MB_OK);
		return 0;
	}

	
	if ( !g_DBGameServer->Initialize(g_config.gameDatabase, g_config.odbcDSNType ) ) 
	{
		g_logSystem->Write("Game Database Connecting Failure!!");
		MessageBox(g_hWnd, "Game Database Connecting Failure!", "Error!", MB_OK);
		return 0; 
	}


	

	
	if(!gGameConfigFlag.LoadFlagFile()) return 0;

	
	
	if( !LoadAllGameTable() ) return 0;

	
	if ( !GTH_LoadWorldTable() )
	{
		g_logSystem->Write("World Table Data Loading Failure!!");
		MessageBox(g_hWnd, "World Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	
	if ( !GTH_Load2DWorld() )
	{
		g_logSystem->Write("2D Map Data Loading Failure!!");
		MessageBox(g_hWnd, "2D Map Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	if( !LoadExpTableForServer("tables/newexptable.tbl" ) )
	{
		g_logSystem->Write("Exp Table Data Loading Failure!!" );
		MessageBox(g_hWnd, "Exp Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	
	if( !LoadGenExpTableForServer("tables/newgenexptable.tbl" ) )
	{
		g_logSystem->Write("Gen Exp Table Data Loading Failure!!" );
		MessageBox(g_hWnd, "Gen Exp Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	if( !LoadStoreTableForServer("tables/storetable.tbl" ) )
	{
		g_logSystem->Write("Store Table Data Loading Failure!" );
		MessageBox(g_hWnd, "Store Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	if( !LoadBindingTableForServer( "tables/bindingTable.tbl" ) )
	{
		g_logSystem->Write("Binding Table Data Loading Failure!" );
		MessageBox(g_hWnd, "Binding Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	if( !LoadGenCapabilityTable( "tables/GenCapability.tbl" ) )
	{
		g_logSystem->Write("Gen Capability Table Data Loading Failure!" );
		MessageBox(g_hWnd, "Gen Capability Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	
	if( !LoadScriptInfoTableForServer( "tables/ScriptInfo.tbl" ) )
	{
		g_logSystem->Write("Script Info Table Data Loading Failure!" );
		MessageBox(g_hWnd, "Script Info Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	
	
	if( !LoadItemCraftTableForServer( "tables/itemcraft.tbl" ) )
	{
		g_logSystem->Write("Item Craft Table Data Loading Failure!" );
		MessageBox(g_hWnd, "Item Craft Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	
	if ( g_config.isManager )															
	{
		if ( !InitDanBattleManager())
		{
			MessageBox(g_hWnd, "Dan Battle Manager Initializing Fail!!!", "Error!", MB_OK);
			return 0;
		}
	}
	
	if ( g_config.isDanBattleMap )														
	{
		if ( !InitDanBattleSystem() )
		{
			MessageBox(g_hWnd, "Dan Battle System Initializing Fail!!", "Error!", MB_OK);
			return 0;
		}
	}
	
	
	
	
	g_monsterSpawnCount = 0;

	
	if ( !GTH_LoadMonster() )
	{
		g_logSystem->Write("Monster Spawn Data Loading Failure!");
		MessageBox(g_hWnd, "Monster Spawn Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	
	if ( FALSE == LoadMax_Defense_table("tables/max_defense_table.tbl") )
	{
		g_logSystem->Write("MaxDefense Table Data Loading Failure!" );
		MessageBox(g_hWnd, "MaxDefense Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	


	
	if ( TRUE == g_config.bAhnLab )
	{
		g_cpAhnLab = new CAhnLab("AntiCpSvr.dat");
		if ( NULL == g_cpAhnLab) return 0;
		if ( FALSE == g_cpAhnLab->IsOK() ) {
			delete g_cpAhnLab; g_cpAhnLab= NULL; 
			g_logSystem->Write("AhnLab Init Faild" );
			MessageBox(g_hWnd, "AhnLab Init Faild!", "Error!", MB_OK);
			return 0; 
		};
	}
	

	
	
	

	
	if ( !DB_CreateQueryExecuterThread() )
	{
		MessageBox(g_hWnd, "Database Thread Creating Failure!", "Error!", MB_OK);
		return 0;
	}

	
	if ( !GTH_Network_Startup() )
		return 0;


	if(TRUE == g_config.Terra.bBilling){	
		gcpTerraBillingCtrl= new CTerraBillingCtrl();
		if(NULL == gcpTerraBillingCtrl) return 0;
		if(FALSE == gcpTerraBillingCtrl->isOK()){ delete gcpTerraBillingCtrl; gcpTerraBillingCtrl=NULL; return 0; }

		if(FALSE == gcpTerraBillingCtrl->ServiceStart()){
			CTerraBillingCtrl::OpenNotePadMessage("billing service start error!!");						
		};	
	}
	


	
	if (!InitBilling ())
	{
		MessageBox(g_hWnd, "Initialize Billing Failure!", "Error!", MB_OK);
		return 0;
	}

	Sleep( 1000 );	

	
	
	

	if ( !GTH_RegistMemberServer() )
	{
		g_logSystem->Write("Member Server Register Failure!");
		MessageBox(g_hWnd, "Member Server Register Failure!", "Error!", MB_OK);
		return 0;
	}

	if ( g_config.isManager )
	{
		
		GTH_SendMessage_GameServerGroupInfo();
	}
	else
	{
		
		GTH_SendMessage_MyServerInfo();
	}

	g_DBAccountServer->DeleteLoginHost( g_hostName );
	
	
	g_DBGameServer->Delete_Alldanbattle( g_config.gameServerNo );
	
	
	
	gcpTools = new CTools();
	if(NULL == gcpTools) return 0;
	if(FALSE == gcpTools->isOK()){ delete gcpTools; gcpTools=NULL; return 0; }

	gcpServerLpackCtrl = new CServerLpackCtrl();
	if(NULL == gcpServerLpackCtrl) return 0;
	if(FALSE == gcpServerLpackCtrl->isOK()){ 
		MessageBox(NULL,"initilize gcpServerLpackCtrl error",NULL,MB_OK);		
		delete gcpServerLpackCtrl; 
		gcpServerLpackCtrl=NULL; 
		return 0; 
	}

	gcpGuildCtrl=new CGuildCtrl();
	if(NULL == gcpGuildCtrl) return 0;
	if(FALSE == gcpGuildCtrl->isOK()){ delete gcpGuildCtrl; gcpGuildCtrl=NULL; return 0; }

	gcpPremiumPlayerCtrl = new CPremiumPlayerCtrl();
	if(NULL == gcpPremiumPlayerCtrl) return 0;
	if(FALSE == gcpPremiumPlayerCtrl->isOK()){ delete gcpPremiumPlayerCtrl; gcpPremiumPlayerCtrl=NULL; return 0; }

	gcpGolryunBattle =  new CGolryunBattle();
	if(NULL == gcpGolryunBattle) return 0;
	if(FALSE == gcpGolryunBattle->isOK()){ delete gcpGolryunBattle; gcpGolryunBattle=NULL; return 0; }		



	gcpGiftCard =  new CGiftCard();
	if(NULL == gcpGiftCard) return 0;
	if(FALSE == gcpGiftCard->isOK()){ delete gcpGiftCard; gcpGiftCard=NULL; return 0; }
	

	gcpGMCtrl = new CGMCtrl();
	if(NULL == gcpGMCtrl) return 0;
	if(FALSE == gcpGMCtrl->isOK()){ delete gcpGMCtrl; gcpGMCtrl=NULL; return 0; }
	g_DBGameServer->LoadServerStatus();

	





	if( g_config.isManager )
	{
		g_DBGameServer->LoadGuild( g_config.serverGroupNo );
	}
	
	g_logSystem->Write("Server Start up Complete.. !!");
	
	
	
	init_genrand( (unsigned) time( NULL ) );

	
	g_ExpEventMng.InitExpEventMng();
	g_ExpEventMng.SendMessage_RequestEventToManagerServer();



	while (WM_QUIT != msg.message) 
	{
		
		GTH_UpdateTimer();
	
		g_netTime = NET_GetNetTime();


		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GTH_ListenSocket();  

		
			g_aliveStateCount[0] = 0;
			g_aliveStateCount[1] = 0;
			g_aliveStateCount[2] = 0;
			g_aliveStateCount[3] = 0;
			g_aliveStateCount[4] = 0;


			
			for (i=0; i<MAX_MONSTERS; i++)
			{
				if ( !g_monster[i].active ) 
					continue;

				switch( g_monster[i].aliveState )
				{
				case MON_ALIVE_STATE_SPAWN :
					g_aliveStateCount[0] ++;
					break;
				case MON_ALIVE_STATE_NORMAL :
					g_aliveStateCount[1] ++;
					break;
				case MON_ALIVE_STATE_CORPSE :
					g_aliveStateCount[2] ++;
					break;
				case MON_ALIVE_STATE_DISAPPEAR :
					g_aliveStateCount[3] ++;
					break;
				case MON_ALIVE_STATE_DIE :
					g_aliveStateCount[4] ++;
					break;
				}

				
				AI_Interpretor( &g_monster[i] );
				MONSTER_ExecuteAIEvent( &g_monster[i] );
			}




			
			g_debugCount = 0;
			for( i = 0; i < MAX_PCS; i ++ )
			{
				if ( !g_pc[i].active || !g_pc[i].ready ) continue;
				if( AI_NPC_Interpretor( &g_pc[i] ) ) g_debugCount ++;
				NPC_SendEvent( &g_pc[i] );
			}


			GTH_ExecutePCEvent();			
			GTH_ExecuteMonsterEvent();		
			GTH_UpdateItemObject();			
			GTH_SendSyncMessage();			
			DB_ProcessQueryResultQueue();	
			GTH_UpdateFrameRate();
			Sleep(10);//lucky CPU
			
			PkTimeAutoProcess();

			
			g_ChristMasEvent.ProcessEventTIme();

			
			g_WeatherSystem.ProcessEventTIme();
			

			
			if( g_config.isManager ){
				GTH_PartyInfo_CheckHolding();
			}

			
			GTH_UpdateServerGroupInfo();

			
			GTH_Process_NoticeInfo();
			
			
			GTH_WorldChatting_Process();

			
			GTH_Process_ServerShutdownTimer();

			
			

			

			
			if ( NULL != g_cpAhnLab )
				g_cpAhnLab->UpdateAhnLab();
			


			
			if(g_config.bCheckBilling){
				ProcessBillingQueue () ;
			}

			if(NULL != gcpTerraBillingCtrl){
				gcpTerraBillingCtrl->Process();
			}
			
			
			
			GTH_ProcessDanBattleSys();													
			

			
			g_NewDanBattleSystem.Update();

			
			g_GonryunPracticeBattle.GetProcessCtrl()->Update();
			
			
			gcpGolryunBattle->GTH_ProcessGBBattleSys();
			


		}

		
		g_logSystem->Update();	
	
	}
	


	





	
	if(NULL != gcpGolryunBattle){ delete gcpGolryunBattle; gcpGolryunBattle=NULL; }


	
	DB_DestroyQueryExecuterThread();

	
	if(NULL != gcpTerraBillingCtrl) gcpTerraBillingCtrl->ServiceEnd();
	if(NULL != gcpTerraBillingCtrl){ delete gcpTerraBillingCtrl; gcpTerraBillingCtrl=NULL; }



	
	
	if(NULL != gcpGMCtrl){ delete gcpGMCtrl; gcpGMCtrl=NULL; }
	if(NULL != gcpGiftCard){ delete gcpGiftCard; gcpGiftCard=NULL; }
	

	
	
	if(NULL != gcpPremiumPlayerCtrl){ delete gcpPremiumPlayerCtrl; gcpPremiumPlayerCtrl=NULL; }
	if(NULL != gcpGuildCtrl){ delete gcpGuildCtrl; gcpGuildCtrl=NULL; }
	if(NULL != gcpServerLpackCtrl){ delete gcpServerLpackCtrl; gcpServerLpackCtrl=NULL; }
	if(NULL != gcpTools){ delete gcpTools; gcpTools=NULL; }

	
	if(NULL != g_cpAhnLab){ delete g_cpAhnLab; g_cpAhnLab=NULL; }

	Sleep( 1000 );

	
	GTH_Free2DWorld();

	
	MONSTER_Free();

	
	GTH_DestroyVariables();

	return msg.wParam;
}

void GTH_UpdateTimer()
{
	g_timer->Tick();
	g_globalTime = (unsigned int)g_timer->GetAppMilliTime();
}

void GTH_InitGlobalTime()
{
	g_globalTime = 0;
	msTime = 0;
}

void GTH_UpdateServerGroupInfo()
{
	msTime += g_timer->GetElapsedMilliTime();
	if (msTime >= 1000) 
	{
		if ( g_config.isManager )
		{
			GTH_UpdateMyServerInfo();	

			if ( !g_serverShutdownTimer.active )
				GTH_SendMessage_GameServerGroupInfo();		

			GTH_SendMessage_MemberServerInfo();			
			
			GTH_CheckProblemMemberServer();
		}
		else
		{
			GTH_SendMessage_MyServerInfo();
			
			GTH_CheckProblemMasterServer();
		}

		msTime = 0;

		
		

	}
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MS);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= (LPCSTR)IDC_MS;
	wcex.lpszClassName	= g_strWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInst = hInstance; 

   hWnd = CreateWindow(g_strWindowClass, g_strAppTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd; 

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}




char* GTH_Debug_ZoneInfo(char *msg);
char* GTH_Debug_MemberServerInfo(char *msg);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
		case WM_CREATE :
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			

			switch (wmId)
			{
				case IDM_ABOUT:
				    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				    break;

				case IDM_DEBUG_CONSOLE :
					CreateConsoleInfo();
					break;
				case IDM_RESPAWN_MONSTER_ALL :

					break;
 
				case IDM_EXIT:
					if ( g_config.isManager )
					{
						
						GTH_SendMessage_DropServerGroup();
					}
					else
					{
						
						GTH_SendMessage_DropMyServer();
					}

					
					GTH_SaveData_AlivedClient();

					
					Sleep(1000); 

					
					


					GTH_Debug_ServerStatusInfo();					

					
					GTH_Network_Shutdown(); 

					
					g_DBAccountServer->Close();
					g_DBGameServer->Close();

					g_logSystem->Write("Sever Normaly Closed..!!");
					g_logSystem->Close();
			
				    DestroyWindow(hWnd);
				    break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_TIMER:
			{
				InvalidateRect(g_hWnd,NULL,1);

			}
			break;

		case WM_KEYDOWN:
			if(wParam == VK_SPACE){
 				Sleep(0);
				static int GuildIdx=0;
				gcpGuildCtrl->GetSummaryRecordPointer(GuildIdx);
				GuildIdx++;
			}
			break;

		case WM_PAINT:

			hdc = BeginPaint(hWnd, &ps);
			
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, g_systemStatusMessage, strlen(g_systemStatusMessage), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;

        case WM_CLOSE:
            return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

extern char *curpos, *endpos;
int GTH_ParseGameServerConfig()
{

	g_config.Terra.bBilling = FALSE;



	FILE *fp;
	int size;
	byte *buffer;
	char *token, windowTitle[256];
	int i;

	fp = fopen( "server.cfg" , "rb" );

	if( !fp )   
	{
		MessageBox(NULL, "does not exist 'server.cfg' file", "Error", MB_ICONHAND|MB_OK);
		return false;
	}

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	fclose(fp);

	curpos = (char *)buffer;
	endpos = curpos + size;
	
	g_config.languageType	=	GTH_LANGUAGE_TYPE_KOREAN;	
	g_config.isTestServer	=	false;						
	
	
	g_config.resendTime = 200;								
	g_config.isDanBattleMap		= FALSE;												
	g_config.bPremium = FALSE;

	
	g_config.bAhnLab = FALSE;		
	
	sprintf( windowTitle, "%s (%s)", g_strAppTitle, "KOREAN" );

	while ((token = NextToken()) != NULL)
    {
		if( !stricmp( token, "SERVER_GROUP_NAME" ) )
		{
			strcpy ( g_config.serverGroupName, NextArg() );
		}
		else if( !stricmp( token, "SERVER_GROUP_NO" ) )
		{
			g_config.serverGroupNo = atoi( NextArg() );
		}
		else if( !stricmp( token, "GAME_SERVER_NAME" ) )
		{
			strcpy ( g_config.gameServerName, NextArg() );
		}
		else if( !stricmp( token, "GAME_SERVER_NO" ) )
		{
			g_config.gameServerNo = atoi( NextArg() );
		}
		else if( !stricmp( token, "SERVICE_IP" ) )
		{
			strcpy ( g_config.serviceIP, NextArg() );
		}
		else if( !stricmp( token, "SERVICE_PORT" ) )
		{
			g_config.servicePort = atoi( NextArg() );
		}
		else if( !stricmp( token, "LOCAL_IP" ) )
		{
			strcpy ( g_config.localIP, NextArg() );
		}
		else if( !stricmp( token, "LOCAL_PORT" ) )
		{
			g_config.localPort = atoi( NextArg() );
		}
		else if( !stricmp( token, "MASTER_SERVER_NUMBER" ) )
		{
			g_config.masterServerNumber = atoi( NextArg() );
		}
		else if( !stricmp( token, "MASTER_SERVER_IP" ) )
		{
			for (i=0; i<g_config.masterServerNumber; i++)
				strcpy ( g_config.masterServerIP[i], NextArg() );
		}
		else if( !stricmp( token, "MASTER_SERVER_PORT" ) )
		{
			for (i=0; i<g_config.masterServerNumber; i++)
				g_config.masterServerPort[i] = atoi( NextArg() );
		}
		else if( !stricmp( token, "MANAGER_YN" ) )
		{
			g_config.isManager = atoi( NextArg() );
		}
		else if( !stricmp( token, "MANAGER_SERVER_IP" ) )
		{
			strcpy ( g_config.managerServerIP, NextArg() );
		}
		else if( !stricmp( token, "MANAGER_SERVER_PORT" ) )
		{
			g_config.managerServerPort = atoi( NextArg() );
		}
		else if( !stricmp( token, "SERVICE_WORLD_NUMBER" ) )
		{
			g_config.serviceWorldNumber = atoi( NextArg() );
		}
		else if( !stricmp( token, "SERVICE_WORLD_LIST" ) )
		{
			for (i=0; i < g_config.serviceWorldNumber; i++)
			{
				g_config.serviceWorldList[i] = atoi( NextArg() );
				if ( g_config.serviceWorldList[i] == DAN_BATTLEMAP_NO )						
				{
					g_config.isDanBattleMap	= TRUE;
				}
			}
		}
		else if ( !stricmp( token , "SERVICE_ISPKMAP_NUMBER" ) )
		{
			g_config.ServiceIsPkMapNumber = atoi( NextArg() );
		}   
		else if ( !stricmp( token , "SERVICE_ISPKMAP_LIST" ) )
		{
			for (i=0; i < g_config.ServiceIsPkMapNumber; i++)
			{
				g_config.ServiceIsPkMapList[i] = atoi( NextArg() );
			}
		}   
		else if ( !stricmp( token , "ODBC_DSN_TYPE" ) )
		{
			g_config.odbcDSNType = atoi( NextArg() );
		}          
		else if( !stricmp( token , "ACCOUNT_ODBC_DSN" ) )
		{
			strcpy( g_config.accountDatabase , NextArg() );
		}
		else if( !stricmp( token , "GAME_ODBC_DSN" ) )
		{
			strcpy( g_config.gameDatabase , NextArg() );
		}
		else if ( !stricmp( token , "GAME_SYNCTIME" ) )
		{
			g_config.gameSyncTime = atoi( NextArg() );
		}          
		else if ( !stricmp( token , "CREATE_GAME_LOG" ) )
		{
			g_config.createLog = atoi( NextArg() );
		}          
		else if ( !stricmp( token , "CREATE_GAME_DEBUG" ) )
		{
			g_config.createDebug = atoi( NextArg() );
		}
		else if( !stricmp( token, "LANGUAGE_TYPE" ) )
		{
			token = NextArg();
			sprintf( windowTitle, "%s (%s)", g_strAppTitle, token );
			
			if( !stricmp( token, "KOERAN" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_KOREAN;
			else if( !stricmp( token, "ENGLISH" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_ENGLISH;
			else if( !stricmp( token, "JAPANESE" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_JAPANESE;
			
			else if( !stricmp( token, "CHINESE" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_SIMPLIFIED_CHINESE;
			else if( !stricmp( token, "TAIWAN" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_TRADITIONAL_CHINESE;
		}
		else if( !stricmp( token, "TEST_SERVER" ) )
		{
			token = NextArg();
			if( !stricmp( token, "TRUE" ) ) g_config.isTestServer = true;
		}
		
		
		else if( !stricmp( token, "RESEND_TIME" ) )
		{
			token = NextArg();
			g_config.resendTime = atoi( token );
			if( g_config.resendTime < 50 ) g_config.resendTime = 50;
		}
		
		else if ( !stricmp ( token, "BILLING_SERVER_ADDR") )
		{
			strcpy (g_config.billingServerIP, NextArg ()) ;
		}
		else if ( !strcmp ( token, "BILLING_SERVER_PORT") )
		{
			g_config.billingServerPort = atoi ( NextArg () );
		}
		else if ( !strcmp ( token, "SERVER_LOCATION") )
		{
			strncpy (g_config.serverLocation,
					 NextArg (),
					 sizeof (g_config.serverLocation));
		}
		else if ( !strcmp ( token, "USE_BILLING") )
		{
			g_config.bCheckBilling = atoi ( NextArg () );
		}

		else if ( !strcmp ( token, "USE_PREMIUM") )
		{
			g_config.bPremium = atoi ( NextArg () );
		}

		else if ( !strcmp ( token, "TERRA_BILLING") )
		{
			char* pTemp = NextArg();
			if((NULL != strstr("yes",pTemp)) || (NULL != strstr("YES",pTemp))) g_config.Terra.bBilling = TRUE;
			else g_config.Terra.bBilling = FALSE;
			
		}
		
		else if (!strcmp(token, "AHNLAB"))
		{
			token = NextArg();
			if( !stricmp( token, "TRUE" ) ) 
				g_config.bAhnLab = TRUE;			
			else
				g_config.bAhnLab = FALSE;
		}
		

	}

	fclose(fp);

	SetWindowText( g_hWnd, windowTitle );

	if(buffer!=NULL)
	{
		delete [] buffer;
	}
	return true;
}





extern int packetsReceived, packetsSent;
extern int droppedDatagrams, receivedDuplicateCount;
extern int packetsReSent, shortPacketCount;


extern int				totalPacketsSend;				
extern int				totalPacketsReceived;			
extern int				packetsSent;					
extern int				packetsReceived;				
extern int				packetsReSent;					
extern int				packetsDropCount;				
extern int				packetsDupCount;				
extern int				packetsAckSent;					
extern int				packetsAckReceived;				
extern int				un_packetsSent;					
extern int				un_packetsReceived;				
extern int				un_packetsDropCount;			

extern int lastRecvSequence;
extern int myRecvSequence;


char* GTH_Debug_ZoneInfo(char *msg)
{
	int zoneNumber;
	char tempMsg[2048];

	zoneNumber = ZONE_DIV * ZONE_DIV;

	sprintf( tempMsg, "%s\n%d PC is in npc ai", msg, g_debugCount );
	strcpy( msg, tempMsg );
	sprintf( tempMsg, "%s\nSPAWN : %d", msg, g_aliveStateCount[0] );
	strcpy( msg, tempMsg );
	sprintf( tempMsg, "%s\nNORMAL : %d", msg, g_aliveStateCount[1] );
	strcpy( msg, tempMsg );
	sprintf( tempMsg, "%s\nCORPSE : %d", msg, g_aliveStateCount[2] );
	strcpy( msg, tempMsg );
	sprintf( tempMsg, "%s\nDISAPPEAR : %d", msg, g_aliveStateCount[3] );
	strcpy( msg, tempMsg );
	sprintf( tempMsg, "%s\nDIE : %d", msg, g_aliveStateCount[4] );
	sprintf( tempMsg, "%s\nSpawnCount : %d", msg, g_monsterSpawnCount );
	strcpy( msg, tempMsg );

	sprintf( tempMsg, "%s\n\ntotal send:%d, total recv:%d, send:%d, recv:%d, resend:%d, drop:%d, dup:%d", msg,
		totalPacketsSend, totalPacketsReceived,
		packetsSent, packetsReceived, packetsReSent, packetsDropCount, packetsDupCount );
	
	strcpy( msg, tempMsg );

	sprintf( tempMsg, "%s\n\nacksend:%d, ackrecv:%d, un_send:%d, un_recv:%d, un_drop:%d", msg,
		packetsAckSent, packetsAckReceived, un_packetsSent, un_packetsReceived, un_packetsDropCount );

	strcpy( msg, tempMsg );

	return msg;
}

char* GTH_Debug_MemberServerInfo(char *msg)
{
	int i, j;
	char tempMsg[1024];
	memberServer_t *m;

	sprintf(msg, "Server Group Name: %s, Member Server Number : %d", g_config.serverGroupName, g_memberServerNumber);
	m = &g_memberServer[0];
	for (i=0; i<MAX_MEMBER_SERVER; i++, m++)
	{
		if ( m->active ) 
		{
			sprintf(tempMsg, "%s\nNo: %d, Name: %s, Addr: %s:%d, Service World: ", msg,  m->idx, m->name, m->serviceIP, m->servicePort);
			for (j=0; j<m->serviceWorldNumber; j++)
				sprintf(tempMsg, "%s %d ", tempMsg,  m->serviceWorldList[j]);
		}
		else
		{
			sprintf(tempMsg, "%s\nNo: %d, NOT PREPARE", msg, m->idx);
		}

		strcpy(msg, tempMsg);
	}

	return msg;
}





char *g_lTableToken[] =
{
	"_kr",
	"_en",
	"_jp",
	"_ch",
	"_tw",		
};


int LoadAllGameTable()
{
	char filename[128];
	char filename2[128];


	
	sprintf( filename, "l_tables/open%s.txt", g_lTableToken[g_config.languageType] );
	if(!g_TxtApp.LoadFile(filename)) 
	{
		g_logSystem->Write("Item Table Txt Data Loading Failure!");
		MessageBox(g_hWnd, "Item Table Txt Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	
	sprintf( filename, "l_tables/open%s.sit", g_lTableToken[g_config.languageType] );
	if ( !LoadItemTable( filename, g_itemTable, g_itemTableNumber ) ) 
	{
		g_logSystem->Write("Item Table Data Loading Failure!");
		MessageBox(g_hWnd, "Item Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	sprintf( filename, "l_tables/open%s.slt", g_lTableToken[g_config.languageType] );
	
	if ( !LoadItemClassTable( filename, g_itemClassTable, g_itemClassTableNumber ) ) 
	{
		g_logSystem->Write("Item Class Table Data Loading Failure!");
		MessageBox(g_hWnd, "Item Class Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}


	
	sprintf( filename, "l_tables/open%s.sqg", g_lTableToken[g_config.languageType] );
	InitQIGTable( g_qigTable );
	if( !LoadQIGTable( filename, g_qigTable ) )
	{
		g_logSystem->Write("Quest Item Generate Table Data Loading Failure!");
		MessageBox(g_hWnd, "Quest Item Generate Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	
	sprintf( filename, "l_tables/open%s.sgt", g_lTableToken[g_config.languageType] );
	if ( !LoadItemGenerateTable( filename, g_itemGenerateTable, g_itemGenerateTableNumber ) ) 
	{
		g_logSystem->Write("Item Generating Table Data Loading Failure!");
		MessageBox(g_hWnd, "Item Generating Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	
	sprintf( filename, "l_tables/open%s.smt", g_lTableToken[g_config.languageType] );
	if(FALSE == LoadMonsterTable( filename, g_monsterTable, g_monsterTableNumber) )
	{
		g_logSystem->Write("Monster Table Data Loading Failure!");
		MessageBox(g_hWnd, "Monster Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	
	sprintf( filename, "l_tables/open%s.skt", g_lTableToken[g_config.languageType] );
	
	
	if( !g_SkillApp.LoadSkillTable(filename, g_skillTableNumber ) )
	{
		g_logSystem->Write("Skill Table Data Loading Failure!");
		MessageBox(g_hWnd, "Skill Table Data Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	
	sprintf( filename, "l_tables/QSTable%s.csv", g_lTableToken[g_config.languageType] );
	if( !LoadQuestScrollTable( filename, g_questscrollTable, g_questscrollTableNumber ) )
	{
		g_logSystem->Write("Quest Scroll Table Loading Failure!");
		MessageBox(g_hWnd, "Quest Scroll Table Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	
	sprintf( filename, "l_tables/AqsTable%s.csv", g_lTableToken[g_config.languageType] );
   	if( !LoadAutoQuestScrollTable( filename, g_autoQuestscrollTable, g_autoQuestscrollTableNumber ) )
	{
		g_logSystem->Write("Quest Scroll Table Loading Failure!");
		MessageBox(g_hWnd, "Quest Scroll Table Loading Failure!", "Error!", MB_OK);
		return 0;
	}
    
	sprintf( filename, "l_tables/GonryunBattlePractice_RewardField%s.csv", g_lTableToken[g_config.languageType] );

	if( !LoadGonryunBattlePracticeTableReword(filename, g_GonryunPracticeBattle.GetRewordCtrl()) )
	{
		g_logSystem->Write("LoadGonryunBattlePracticeTableReword!");
		MessageBox(g_hWnd, "LoadGonryunBattlePracticeTableReword!", "Error!", MB_OK);
		return 0;
	}

	sprintf( filename, "l_tables/GonryunBattlePractice_RewardItem%s.csv", g_lTableToken[g_config.languageType] );
	if( !LoadGonryunBattlePracticeTableRewordItem(filename, g_GonryunPracticeBattle.GetRewordCtrl()) )
	{
		g_logSystem->Write("LoadGonryunBattlePracticeTableRewordItem!");
		MessageBox(g_hWnd, "LoadGonryunBattlePracticeTableRewordItem!", "Error!", MB_OK);
		return 0;
	}

	 

	
	sprintf( filename, "l_tables/GolryunBattle_Manager.csv" );
   	if( !LoadGolryunManagerTable( filename ) )
	{
		g_logSystem->Write("Golryun Manager Table Loading Failure!");
		MessageBox(g_hWnd, "Golryun Manager Table Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	sprintf( filename, "l_tables/GolryunBattle_Progress.csv" );
   	if( !LoadGolryunProgressTable( filename ) )
	{
		g_logSystem->Write("Golryun Progress Table Loading Failure!");
		MessageBox(g_hWnd, "Golryun Progress Table Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	sprintf( filename, "l_tables/GolryunBattle_RewardManager.csv" );
   	if( !LoadGolryunRewardManagerTable( filename ) )
	{
		g_logSystem->Write("Golryun Reward Manager Table Loading Failure!");
		MessageBox(g_hWnd, "Golryun Reward Manager Table Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	sprintf( filename, "l_tables/GolryunBattle_ItemGroup.csv" );
   	if( !LoadGolryunItemGroupTable( filename ) )
	{
		g_logSystem->Write("Golryun Item Group Table Loading Failure!");
		MessageBox(g_hWnd, "Golryun Item Group Table Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	

	
	
	sprintf( filename, "l_tables/optTable%s.csv", g_lTableToken[g_config.languageType] );
	sprintf( filename2, "l_tables/valueTable%s.csv", g_lTableToken[g_config.languageType] );
	if( !MakeOptionTable( filename, filename2, g_optionTable ) )
	{
		g_logSystem->Write("Option Table Loading Failure!");
		MessageBox(g_hWnd, "Option Table Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	
	sprintf( filename, "l_tables/OptGroupTable.csv" );
	if( !LoadOptionGroupTable( filename, g_optionGroupTable ) )
	{
		g_logSystem->Write("Option Group Table Loading Failure!");
		MessageBox(g_hWnd, "Option Group Loading Failure!", "Error!", MB_OK);
		return 0;
	}
	
	
	sprintf( filename, "l_tables/summontable%s.csv" , g_lTableToken[g_config.languageType]);
	if( !LoadSummonMonsterTable( filename,  g_summonMonstertable, g_summonMonstertableNumber) )
	{
		g_logSystem->Write("Monster Summon Table Loading Failure!");
		MessageBox(g_hWnd, "Monster Summon Loading Failure!", "Error!", MB_OK);
		return 0;
	}

	

	
	g_GambleSystem.CollectGambleItems();
	g_GambleSystem.CollectGambleOptions();

	
	


	






	
	

	return 1;
}








extern CQueryQueue		*queryRequestQueue, *queryResultQueue;

unsigned int frameTime = 0;
int frameCount = 0;
char frameInfoMsg[4096];
int g_FPS;


void GTH_UpdateFrameRate()
{
	char zoneInfo[2048];
	char memberServerInfo[1024];

	zoneInfo[0] = '\0';
	memberServerInfo[0] = '\0';

	if ( g_globalTime < frameTime + 1000  )
	{
		frameCount ++;
	}
	else
	{
		GTH_Debug_ZoneInfo(zoneInfo);
		g_FPS = frameCount;
		GTH_Debug_MemberServerInfo(memberServerInfo);


		if(NULL == gcpGuildCtrl){
			sprintf(frameInfoMsg, "FPS : %d, SendingQueue (%d, %d), ReceivingQueue (%d, %d), QReqQ(%d, %d), QResQ(%d, %d), PC:(%d, %d), Monster: %d, Item: %d\n%s\n\n%s", 
				frameCount, sendingQueue->m_count, sendingQueue->m_maxCount, receivingQueue->m_count, receivingQueue->m_maxCount, 
				queryRequestQueue->m_count, queryRequestQueue->m_maxCount, queryResultQueue->m_count, queryResultQueue->m_maxCount, 
				g_pcNumber, g_maxPCNumber, g_monsterNumber, g_objectNumber, memberServerInfo, zoneInfo);
		}else{
			sprintf(frameInfoMsg, "FPS : %d, SendingQueue (%d, %d), ReceivingQueue (%d, %d), QReqQ(%d, %d), QResQ(%d, %d), PC:(%d, %d), Monster: %d, Item: %d\n%s\n\n%s\nguild sync tr num=%d", 
				frameCount, sendingQueue->m_count, sendingQueue->m_maxCount, receivingQueue->m_count, receivingQueue->m_maxCount, 
				queryRequestQueue->m_count, queryRequestQueue->m_maxCount, queryResultQueue->m_count, queryResultQueue->m_maxCount, 
				g_pcNumber, g_maxPCNumber, g_monsterNumber, g_objectNumber, memberServerInfo, zoneInfo,gcpGuildCtrl->Get_transmite_req_sync_at_manager_server_number());
		}


		g_systemStatusMessage = TEXT( frameInfoMsg );
		InvalidateRect(g_hWnd,NULL,1);
		frameTime = g_globalTime;
		frameCount = 0;
	}
}


