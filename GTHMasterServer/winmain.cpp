#include "Global.h"

unsigned int curGlobalTime, prevGlobalTime;

unsigned int nextStatisticsTime;


extern CSendingQueue *sendingQueue;
extern CReceivingQueue *receivingQueue;


#include "CPremiumPlayerCtrl.h"
CPremiumPlayerCtrl* gcpPremiumPlayerCtrl=NULL;


#include ".\Terra\Billing\CTerraBillingCtrl.h"
CTerraBillingCtrl* gcpTerraBillingCtrl=NULL;
//lucky 2012
#include "Ini.h"
//end




#include "./HiCocoon/HiCocoonCtrl.h"
CHiCocoonCtrl*	g_pHiCocoonCtrl = NULL;

//lucky 2012 local ip
//IP
char LANSERVICE_IP[40];
//end

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	
	MSG msg;
	int i;


	
	MyRegisterClass(hInstance);

	
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	
	GTH_InitGlobalVariable(); 

	


	
	if ( !GTH_ParseMasterServerConfig() )
		return FALSE;  


	if(TRUE == g_config.TerraV2.bActive){		
		if(NULL != g_TerraDBCtrl){ delete g_TerraDBCtrl; g_TerraDBCtrl=NULL; }
		g_TerraDBCtrl= new CTerraDBCtrl();		




	}

	
	if(TRUE == g_config.Hicocoon.bActive){		
		g_pHiCocoonCtrl = new CHiCocoonCtrl();
		if(NULL == g_pHiCocoonCtrl) 
		{
			MessageBox(g_hWnd, "Init Failed For HiCocoon.", "Error", MB_OK);
			return 0;
		}
		if(FALSE == g_pHiCocoonCtrl->IsOK())
		{

			MessageBox(g_hWnd, "Init Failed For HiCocoon.", "Error", MB_OK);			
			return 0; 
		}		
	}
	



	
	if(!gGameConfigFlag.LoadFlagFile())
		return FALSE;

	
	GTH_InitGameServerGroup();

	GTH_ParseIPList();

	
	g_logSystem->Initialize(g_config.createLog, g_config.createDebug);
	g_logSystem->Open();

	
	nextStatisticsTime = g_config.updateStatisticsTime * 60 * 1000;

	
	GTH_InitGlobalTime(); 

	if ( !GTH_Load_Connection_Filtering() )
	{
		g_logSystem->Write("Filtering Data Loading Failure.");
		MessageBox(g_hWnd, "Filtering Data Loading Failure.", "Error", MB_OK);
		return FALSE;
	}

	
	if ( !GTH_LoadCharacterTable() )
	{
		g_logSystem->Write("Character Table Data Loading Failure");
		MessageBox(g_hWnd, "Character Table Data Loading Failure", "Error", MB_OK);
		return FALSE;
	}

	
	if ( !g_DBAccountServer->Initialize( g_config.accountDatabaseDSN, g_config.odbcDSNType ) ) 
	{
		g_logSystem->Write("Account Database Connection Failure");
		MessageBox(g_hWnd, "Account Database Connection Failure", "Error", MB_OK);
		return FALSE;
	}
	else
	{
		g_logSystem->Write("Account Database Connection Complete.");
	}



	if(NULL != g_TerraDBCtrl){
		if(!g_TerraDBCtrl->Initialize( g_config.TerraV2.MemberShipDSN, g_config.odbcDSNType,"TGTH","gth%terra6345")){
			g_logSystem->Write("Terra Database Connection Failure");
			MessageBox(g_hWnd, "Terra Database Connection Failure", "Error", MB_OK);
			return FALSE;
		}else{
			g_logSystem->Write("Terra Database Connection Complete.");
		}
	}




	
	for (i = 0; i<g_config.serviceServerGroupNumber; i++)
	{
		if ( !g_DBGameServer[i]->Initialize( g_config.gameDatabaseDSN[i], g_config.odbcDSNType ) ) 
		{
			char errMsg[256];
			sprintf(errMsg, "Game Database(SG:%d) Connection Failure", i);
			g_logSystem->Write(errMsg);
			MessageBox(g_hWnd, errMsg, "Error", MB_OK);
			return FALSE;
		}
		else
		{

			g_logSystem->Write("Game Database(ServerGroup:%d) Server Connection Complete.", i);
		}

	}

	
	if ( !DB_CreateQueryExecuterThread() )
	{
		g_logSystem->Write("Database Thread Creation Failure");
		
		MessageBox(g_hWnd, "Database Thread Creation Failure", "Error", MB_OK);
		return FALSE;
	}

	gcpPremiumPlayerCtrl = new CPremiumPlayerCtrl();
	if(NULL == gcpPremiumPlayerCtrl) return FALSE;
	if(FALSE == gcpPremiumPlayerCtrl->isOK()){ delete gcpPremiumPlayerCtrl; gcpPremiumPlayerCtrl=NULL; return FALSE; }


	if((TRUE == g_config.TerraV2.bActive) && (TRUE == g_config.TerraV2.bBilling)){
		gcpTerraBillingCtrl= new CTerraBillingCtrl();
		if(NULL == gcpTerraBillingCtrl) return 0;
		if(FALSE == gcpTerraBillingCtrl->isOK()){ delete gcpTerraBillingCtrl; gcpTerraBillingCtrl=NULL; return 0; }
	}


	
	if (!InitBilling ()) 
		return FALSE ;

	
	while (WM_QUIT != msg.message) 
	{

		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			
			g_globalTime = GTH_GetGlobalTime();

			
			if (g_systemOn)
			{
				GTH_MasterServer();
				GTH_UpdateFrameRate(); 
			}	
			
			
			DB_ProcessQueryResultQueue();
		}

		
		if ( g_config.bCheckBilling)
		{
			
			ProcessBillingQueue () ;
		}

		if(NULL != gcpTerraBillingCtrl) gcpTerraBillingCtrl->Process();

		
		GTH_CheckProblemServerGroup();

		
		if ( g_config.createStatistics )
		{
			GTH_UpdateStatistics();
		}

		
		g_logSystem->Update();

		
		Sleep(10) ; //lucky CPU
	}

	
	DB_DestroyQueryExecuterThread();

	
	if ( g_config.bCheckBilling )
	{
		g_pBillingSocket->Close () ;
		g_pBillingSocket->Wait () ;
	}
	if(NULL != gcpPremiumPlayerCtrl){ delete gcpPremiumPlayerCtrl; gcpPremiumPlayerCtrl=NULL; }
	if(NULL != gcpTerraBillingCtrl){ delete gcpTerraBillingCtrl; gcpTerraBillingCtrl=NULL; }

	if(NULL != g_TerraDBCtrl){ delete g_TerraDBCtrl; g_TerraDBCtrl=NULL; }
	
	if(NULL != g_pHiCocoonCtrl) {delete g_pHiCocoonCtrl; g_pHiCocoonCtrl=NULL; }

	
	GTH_DestroyGlobalVariable();

	return msg.wParam;
}

void GTH_UpdateTimer()
{
	g_timer->Tick();
	g_globalTime = (unsigned int)g_timer->GetAppMilliTime();
}

void GTH_UpdateStatistics()
{
	int i;
	serverGroup_t *serverGroup;

	if (g_globalTime >= nextStatisticsTime)
	{
		nextStatisticsTime = g_globalTime + ( g_config.updateStatisticsTime * 60000 ); 
		
		serverGroup = &g_serverGroup[0];
		for (i=0; i<MAX_SERVER_GROUP; i++, serverGroup++)
		{
			if ( !serverGroup->active ) continue;
#ifdef _TEST_SERVER	
			g_DBAccountServer->InsertServerGroupUsers(255, serverGroup->totalClient, serverGroup->maxTotalClient);
#else
			g_DBAccountServer->InsertServerGroupUsers(i, serverGroup->totalClient, serverGroup->maxTotalClient);
			g_DBAccountServer->SaveServerStatus(
				i,
				serverGroup->name,
				serverGroup->totalClient);
#endif
		}
	}
}








void GTH_CheckProblemServerGroup()
{
	int i;
	serverGroup_t *sg;

	sg = &g_serverGroup[0];

	for (i=0; i<g_config.serviceServerGroupNumber; i++, sg++)
	{
		if ( !sg->active ) continue;

		if ( g_globalTime - sg->updateTime > 3000 ) 
		{
			sg->active = false;
			sg->memberServerNumber = 0;
		}
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

   hInst = hInstance; 

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











LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int i;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			
			switch (wmId)
			{
				case IDM_ABOUT:
				    DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				    break;

				case IDM_START:
					if (g_systemOn) return 0;

					
					g_systemStatusMessage = TEXT( "MS: worming up State" );

					GTH_InitMasterServer();

					
					if ( !GTH_Network_Startup() )
						return 0;

					
					g_DBAccountServer->DeleteLoginHost( g_hostName );

					
					g_systemStatusMessage = TEXT( "MS: Normal State" );
					InvalidateRect(hWnd,NULL,1);
					g_logSystem->Write("Starting Master Server Service.");



					g_systemOn = true;

					if(NULL != gcpTerraBillingCtrl){
						if(FALSE == gcpTerraBillingCtrl->ServiceStart()){
							CTerraBillingCtrl::OpenNotePadMessage("billing service start error!!");						
						}
					}

					break;

				case IDM_STOP:
					if (!g_systemOn) return 0;

					if(NULL != gcpTerraBillingCtrl){
						gcpTerraBillingCtrl->ServiceEnd();
					}

					
					g_systemStatusMessage = TEXT( "MS: Stopping State" );

					
					
					Sleep(1000); 

					
					DB_Waiting_ProcessRemainderQuery();

					
					GTH_Debug_ServerStatusInfo();

					g_systemOn = false;
					GTH_Network_Shutdown(); 
					
					g_systemStatusMessage = TEXT( "MS: Shut down State" );
					InvalidateRect(hWnd,NULL,1);
					g_logSystem->Write("Stopping Master Server Service.");
					break;

				case IDM_FILTER :
					g_filterYn = !g_filterYn;
					if ( g_filterYn ) 
					{
						GTH_Load_Connection_Filtering();
						ModifyMenu(GetMenu(hWnd), IDM_FILTER, MF_BYCOMMAND | MF_STRING, IDM_FILTER, "Test Mode OFF");
					}
					else
					{
						ModifyMenu(GetMenu(hWnd), IDM_FILTER, MF_BYCOMMAND | MF_STRING, IDM_FILTER, "Test Mode ON");
					}
					break;

				case IDM_EXIT:
					if (g_systemOn) 
					{
						
						MessageBox(hWnd, "Do shutdown server before Exit", "Notice", MB_OK);
						return 0;
					}

					
					g_DBAccountServer->Close();


					if(NULL != g_TerraDBCtrl){
						g_TerraDBCtrl->Close();
					}

					for (i=0; i<g_config.serviceServerGroupNumber; i++)
						g_DBGameServer[i]->Close();

					
					g_logSystem->Close();

				    DestroyWindow(hWnd);
				    break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
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
			if (g_systemOn) 
			{
				
				MessageBox(hWnd, "Do shutdown server before Exit", "Notice", MB_OK);
				return 0;
			}
			DestroyWindow( hWnd );
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
int GTH_ParseMasterServerConfig()
{
	g_config.bBlockNotRealNameUser=FALSE;
	g_config.TerraV2.bActive=FALSE;
	g_config.TerraV2.bBilling=FALSE;
	g_config.TerraV2.MemberShipDSN[0]=NULL;

	
	g_config.Hicocoon.bActive = FALSE;
	g_config.Hicocoon.bBilling = FALSE;
	
	

	FILE *fp;
	int size;
	byte *buffer;
	char *token;
	char  windowTitle[256];

	fp = fopen( "server.cfg" , "rb" );

	if( !fp )   
	{
		MessageBox(NULL, "Does not exist 'server.cfg' file", "GTH_Master_Server", MB_ICONHAND|MB_OK);
		return false;
	}

	fseek(fp,0,SEEK_END);

	size = ftell(fp);

	buffer = new byte[size+1];

	fseek(fp,0,SEEK_SET);
	fread(buffer,size,1,fp);
	if(NULL != fp){ fclose(fp); fp=NULL; }

	curpos = (char *)buffer;
	endpos = curpos + size;

	
	
	g_config.languageType = GTH_LANGUAGE_TYPE_KOREAN;
	sprintf( windowTitle, "%s (%s)", g_strAppTitle, "KOREAN" );

	
	while ((token = NextToken()) != NULL)
    {
		if( !stricmp( token, "SERVER_NO" ) )
		{
			g_config.serverNo = atoi( NextArg() );
		}
		else if( !stricmp( token , "SERVICE_IP" ) )
		{
			strcpy( g_config.serviceIP , NextArg() );
		}
		else if( !stricmp( token, "SERVICE_PORT" ) )
		{
			g_config.servicePort = atoi( NextArg() );
		}
		else if( !stricmp( token , "LOCAL_IP" ) )
		{
			strcpy( g_config.localIP , NextArg() );
		}
		else if( !stricmp( token, "LOCAL_PORT" ) )
		{
			g_config.localPort = atoi( NextArg() );
		}
		//lucky 2012 network between Client / MS / GS
		else if( !stricmp( token, "LANSERVICE_IP" ) )
		{

			strcpy(LANSERVICE_IP , NextArg() );
		}
		//end
		else if( !stricmp( token, "SERVICE_SERVER_GROUP_NUMBER" ) )
		{
			g_config.serviceServerGroupNumber = atoi( NextArg() );
		}
		else if ( !stricmp( token , "ODBC_DSN_TYPE" ) )
		{
			g_config.odbcDSNType = atoi( NextArg() );
		}          
		else if( !stricmp( token , "ACCOUNT_ODBC_DSN" ) )
		{
			strcpy( g_config.accountDatabaseDSN , NextArg() );
		}

		else if( !stricmp( token , "TERRA"))
		{
			char* pTemp = NextArg();			
			if((NULL != strstr("yes",pTemp)) || (NULL != strstr("YES",pTemp))) g_config.TerraV2.bActive=TRUE;
			else g_config.TerraV2.bActive=FALSE;		
		}


		else if( !stricmp( token , "TERRA_BILLING"))
		{
			char* pTemp = NextArg();			
			if((NULL != strstr("yes",pTemp)) || (NULL != strstr("YES",pTemp))) g_config.TerraV2.bBilling=TRUE;
			else g_config.TerraV2.bBilling=FALSE;		
			if(FALSE == g_config.TerraV2.bActive) g_config.TerraV2.bBilling=FALSE;
		}


		else if( !stricmp( token , "TERRA_MEMBERSHIPDSN"))
		{
			strncpy( g_config.TerraV2.MemberShipDSN , NextArg(),DATABASESTRING);
			g_config.TerraV2.MemberShipDSN[DATABASESTRING]=NULL;		
		}


		
		else if( !stricmp( token , "HICOCOON"))
		{
			char* pTemp = NextArg();			
			if((NULL != strstr("yes",pTemp)) || (NULL != strstr("YES",pTemp))) g_config.Hicocoon.bActive=TRUE;
			else g_config.Hicocoon.bActive=FALSE;		
		}
		
		


		
		
		
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_00" ) )
		{
			strcpy( g_config.gameDatabaseDSN[0] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_01" ) )
		{
			strcpy( g_config.gameDatabaseDSN[1] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_02" ) )
		{
			strcpy( g_config.gameDatabaseDSN[2] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_03" ) )
		{
			strcpy( g_config.gameDatabaseDSN[3] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_04" ) )
		{
			strcpy( g_config.gameDatabaseDSN[4] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_05" ) )
		{
			strcpy( g_config.gameDatabaseDSN[5] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_06" ) )
		{
			strcpy( g_config.gameDatabaseDSN[6] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_07" ) )
		{
			strcpy( g_config.gameDatabaseDSN[7] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_08" ) )
		{
			strcpy( g_config.gameDatabaseDSN[8] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_09" ) )
		{
			strcpy( g_config.gameDatabaseDSN[9] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_10" ) )
		{
			strcpy( g_config.gameDatabaseDSN[10] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_11" ) )
		{
			strcpy( g_config.gameDatabaseDSN[11] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_12" ) )
		{
			strcpy( g_config.gameDatabaseDSN[12] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_13" ) )
		{
			strcpy( g_config.gameDatabaseDSN[13] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_14" ) )
		{
			strcpy( g_config.gameDatabaseDSN[14] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_15" ) )
		{
			strcpy( g_config.gameDatabaseDSN[15] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_16" ) )
		{
			strcpy( g_config.gameDatabaseDSN[16] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_17" ) )
		{
			strcpy( g_config.gameDatabaseDSN[17] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_18" ) )
		{
			strcpy( g_config.gameDatabaseDSN[18] , NextArg() );
		}	
		else if( !stricmp( token , "GAME_ODBC_DSN_GROUP_19" ) )
		{
			strcpy( g_config.gameDatabaseDSN[19] , NextArg() );
		}
		else if( !stricmp( token , "GAME_VERSION" ) )
		{
			strcpy( g_config.gameVersion , NextArg() );
		}
		else if( !stricmp( token , "CREATE_STATISTICS" ) )
		{
			g_config.createStatistics = atoi( NextArg() );
		}
		else if( !stricmp( token , "UPDATE_STATISTICS_TIME" ) )
		{
			g_config.updateStatisticsTime = atoi( NextArg() );
		}
		else if( !stricmp( token , "MAX_SERVICE_CLIENT_NUMBER" ) )
		{
			g_config.maxServiceClientNumber = atoi( NextArg() );
		}
		else if( !stricmp( token , "MAX_SERVERGROUP_CLIENT_NUMBER" ) )
		{
			g_config.maxServerGroupServiceClientNumber = atoi( NextArg() );
		}
		else if( !stricmp( token , "MONITOR_VERSION" ) )
		{
			strcpy( g_config.monitorVersion, NextArg() );
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
			if( !stricmp( token, "KOERAN" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_KOREAN;
			else if( !stricmp( token, "ENGLISH" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_ENGLISH;
			else if( !stricmp( token, "JAPANESE" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_JAPANESE;
			else if( !stricmp( token, "CHINESE" ) ) g_config.languageType = GTH_LANGUAGE_TYPE_CHINESE;

			sprintf( windowTitle, "%s (%s)", g_strAppTitle, token );
			SetWindowText( g_hWnd, windowTitle );
		}
		
		else if ( !stricmp ( token, "BILLING_SERVER_ADDR") )
		{
			strcpy (g_config.billingServerIP, NextArg ()) ;
		}
		else if ( !strcmp ( token, "BILLING_SERVER_PORT") )
		{
			g_config.billingServerPort = atoi ( NextArg () );
		}
		else if ( !strcmp ( token, "USE_BILLING") )
		{
			g_config.bCheckBilling = atoi ( NextArg () );
		}

		else if ( !strcmp ( token, "BLOCK_NOT_REAL_NAME_USER") )
		{
			g_config.bBlockNotRealNameUser = atoi ( NextArg () );
		}
	}

	if(NULL != fp){ fclose(fp); fp=NULL; }

	return true;
}


int GTH_Load_Connection_Filtering()
{

	
	if ( g_config.languageType == GTH_LANGUAGE_TYPE_KOREAN)
	{
		strcpy(g_filterMessage[0], "辑厚胶 矫埃捞 酒凑聪促.");
		strcpy(g_filterMessage[1], "辑滚 沥扁 痢八 矫埃涝聪促.");
		strcpy(g_filterMessage[2], "辑滚 烙矫 痢八 吝涝聪促.");
	}
	else if( g_config.languageType == GTH_LANGUAGE_TYPE_CHINESE )
	{
		strcpy(g_filterMessage[0], "现在不是服务时间。");
		strcpy(g_filterMessage[1], "服务器正在进行定期检查。");
		strcpy(g_filterMessage[2], "服务器正在进行临时检查。");
	}
	else
	{
		strcpy(g_filterMessage[0], "Not the service time.");
		strcpy(g_filterMessage[1], "Server regular inspection");
		strcpy(g_filterMessage[2], "Server extra inspection");
	}

	return 1;
}



int GTH_ParseIPList()
{
	FILE	*fp;
	int		size;
	byte	*buffer=NULL;
	char	*token;
	int		i, n;
	int		listType = 0;		

	
	for( i = 0; i < MAX_CONNECTION_IP; i ++ )
	{
		memset( g_config.acceptIP[i], 0, sizeof( g_config.acceptIP[i] ) );
		memset( g_config.rejectIP[i], 0, sizeof( g_config.rejectIP[i] ) );
	}

	fp = fopen( "iplist.cfg" , "rb" );

	if( !fp )   
	{
		return false;
	}

	fseek( fp, 0, SEEK_END );
	
	size	=	ftell(fp);
	buffer	=	new byte[size+1];

	fseek( fp , 0, SEEK_SET );
	fread( buffer, size, 1, fp );
	if(NULL != fp){ fclose( fp ); fp=NULL; }

	curpos = (char *)buffer;
	endpos = curpos + size;

	while( ( token = NextToken() ) != NULL )
    {
		if( !stricmp( token, "ACCEPT_IPLIST" ) )
		{
			listType	=	0;
			n			=	0;
		}
		else if( !stricmp( token, "REJECT_IPLIST" ) )
		{
			listType	=	1;
			n			=	0;
		}
		else
		{
			if( listType == 0 ) strcpy( g_config.acceptIP[n ++], token );
			else strcpy( g_config.rejectIP[n ++], token );
		}
	}

	if(NULL != fp){ fclose(fp); fp=NULL; }
	if(NULL != buffer){ delete []buffer; buffer=NULL; }
	return true;
}



int CheckConnectionIP( char *ipStr )
{
	int		i;
	
	
	for( i = 0; i < MAX_CONNECTION_IP; i ++ )
	{
		if( g_config.rejectIP[i][0] == 0 ) break;
		if( !stricmp( g_config.rejectIP[i], ipStr ) ) return ConnectionReject;
	}
	
	for( i = 0; i < MAX_CONNECTION_IP; i ++ )
	{
		if( g_config.acceptIP[i][0] == 0 ) break;
		if( !stricmp( g_config.acceptIP[i], ipStr ) ) return ConnectionAccept;
	}

	return ConnectionFilterTest;
}



void GTH_InitGameServerGroup() 
{
	int i, j, k;

	for (i=0; i<MAX_SERVER_GROUP; i++)
	{
		g_serverGroup[i].idx = i;
		g_serverGroup[i].active = false;

		if ( g_config.languageType == GTH_LANGUAGE_TYPE_KOREAN)
			sprintf( g_serverGroup[i].name, "辑滚焙%02d", i);
		else if( g_config.languageType == GTH_LANGUAGE_TYPE_CHINESE )
			sprintf( g_serverGroup[i].name, "服务器群%02d", i);
	
		strcpy( g_serverGroup[i].DSN, "" );
		g_serverGroup[i].totalClient = 0;
		g_serverGroup[i].maxTotalClient = 0;
		g_serverGroup[i].memberServerNumber = 0;
		for (j=0; j < MAX_MEMBER_SERVER; j++)
		{
			g_serverGroup[i].memberServer[j].idx = j;
			g_serverGroup[i].memberServer[j].active = false;

			strcpy( g_serverGroup[i].memberServer[j].hostName, "" );
			strcpy( g_serverGroup[i].memberServer[j].serviceIP, "" );
			g_serverGroup[i].memberServer[j].servicePort = 0;
			strcpy( g_serverGroup[i].memberServer[j].localIP, "" );
			g_serverGroup[i].memberServer[j].localPort = 0;

			g_serverGroup[i].memberServer[j].worldNumber = 0;
			for (k=0; k < MAX_SERVICE_WORLD; k++ )
				g_serverGroup[i].memberServer[j].worldList[k] = 0;
			g_serverGroup[i].memberServer[j].clientNumber = 0;
			g_serverGroup[i].memberServer[j].maxClientNumber = 0;
		}
	}
}






void GTH_InitGlobalTime()
{
	prevGlobalTime = curGlobalTime = timeGetTime();
	g_globalTime = 0;
}

unsigned int GTH_GetGlobalTime()
{
	unsigned int tempTime;

	curGlobalTime = timeGetTime();
	if (curGlobalTime >= prevGlobalTime)
	{
		g_globalTime = g_globalTime + (curGlobalTime - prevGlobalTime);
	}
	else
	{
		tempTime = (unsigned int)pow(2, 32) - prevGlobalTime; 
		g_globalTime = g_globalTime + tempTime + curGlobalTime;
	}

	prevGlobalTime = curGlobalTime;

	return g_globalTime;
}




char* GTH_Debug_ServerGroupInfo(char *msg)
{
	int i ;
	char tempMsg[4096], tempMsg2[4096];
	serverGroup_t *g;

	strcpy(tempMsg, "");
	strcpy(tempMsg2, "");

	g = &g_serverGroup[0];
	for (i=0; i<g_config.serviceServerGroupNumber; i++, g++)
	{
		if ( !g->active )	continue;

		sprintf(tempMsg2, "%s\n[ SG_NO: %02d( %s ) ] -> Member Server: %d, Total Client: (%d, %d)", 
			tempMsg, g->idx, g->name, GTH_GetNumberActiveMemberServer(i), g->totalClient, g->maxTotalClient);
		strcpy(tempMsg, tempMsg2);
	}
	strcpy(msg, tempMsg);

	return msg;
}


extern CQueryQueue		*queryRequestQueue, *queryResultQueue;


unsigned int frameTime = 0;
int frameCount = 0;
int minFrame, maxFrame;
char frameInfoMsg[2048];
void GTH_UpdateFrameRate()
{
	char filterMsg[512];

	char serverGroupInfo[4096];
	serverGroupInfo[0] = '\0';
	GTH_Debug_ServerGroupInfo( serverGroupInfo );

	if ( g_globalTime < frameTime + 1000  )
	{
		frameCount ++;
	}
	else
	{
		if ( g_filterYn ) 

			sprintf(filterMsg, "Not Available Connection\n%s", g_filterMessage[g_filterIdx]);	
		else
			strcpy(filterMsg, "Available Connection");	

		sprintf(frameInfoMsg, "%s\nFPS : %d, SQ (%d, %d), RQ (%d, %d), QReqQ(%d, %d), QResQ(%d, %d)\nClients: %d, TotalConcurrents: %d / %d\n%s", 
			filterMsg, 
			frameCount, sendingQueue->m_count, sendingQueue->m_maxCount, receivingQueue->m_count, receivingQueue->m_maxCount,  
			queryRequestQueue->m_count, queryRequestQueue->m_maxCount, queryResultQueue->m_count, queryResultQueue->m_maxCount, 
			g_clientNumber, g_concurrentClients, g_maxConcurrentClients, serverGroupInfo);

		g_systemStatusMessage = TEXT( frameInfoMsg );
		InvalidateRect(g_hWnd,NULL,1);
		frameTime = g_globalTime;
		if ( frameCount > maxFrame ) maxFrame = frameCount;
		if ( frameCount < minFrame ) minFrame = frameCount;
		frameCount = 0;
	}
}
