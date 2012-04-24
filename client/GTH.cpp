





#include "global_def.h"
#include "resource.h"

#include "GTH.h"
#include "../game/GolryunBattle/GolryunBattle.h"







VOID GTH::ReadSettings()
{
    HKEY hkey;
    if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, DXAPP_KEY, 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        

        
        
        DXUtil_ReadIntRegKey( hkey, TEXT("Width"), &m_dwCreationWidth, m_dwCreationWidth );
        DXUtil_ReadIntRegKey( hkey, TEXT("Height"), &m_dwCreationHeight, m_dwCreationHeight );

        RegCloseKey( hkey ); 
    }
}








VOID GTH::WriteSettings()
{
    HKEY hkey;
    DWORD dwType = REG_DWORD;
    DWORD dwLength = sizeof(DWORD);

    if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, DXAPP_KEY, 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        

        
        
        DXUtil_WriteIntRegKey( hkey, TEXT("Width"), m_rcWindowClient.right );
        DXUtil_WriteIntRegKey( hkey, TEXT("Height"), m_rcWindowClient.bottom );

        RegCloseKey( hkey );
    }
}










HRESULT GTH::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
    BOOL bCapsAcceptable;

    
    bCapsAcceptable = TRUE;

    if( bCapsAcceptable )         
        return S_OK;
    else
        return E_FAIL;
}



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

HRESULT GTH::RenderText()	
{
	 
	if	(	! g_pApp ->m_renderEnabled	)
		return	S_OK;

	int i;
	float sum, fps;
	vec3_t svrPosition; 
	
	g_chatManager->SetFontColor( 0xffffffff );
	g_chatManager->SetDFontColor( 0xffffffff );

	if ( !g_cgv.displayDebugMessage ) return S_OK;

	
	if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return S_OK;

	g_chatManager->SetFontSize( 12.0f );
	g_chatManager->DPrint( 2, 20, m_strDeviceStats );
	g_chatManager->DPrint( 2, 40, m_strFrameStats );

	extern itementity_t *g_mouseSelectedItemObject;
	if( g_mouseSelectedItemObject )
		g_chatManager->DPrint( 2, 140, "serverID : %d  IDX : %d" , g_mouseSelectedItemObject->idxOnServer , g_mouseSelectedItemObject->idx );

	extern Fx_CHARACTER_t *g_mouseSelectedCharacter;
	if( g_mouseSelectedCharacter )
	{
		g_chatManager->HPrint( 2, 160, "serverID : %d  name : %s" , g_mouseSelectedCharacter->idxOnServer , g_mouseSelectedCharacter->name );
		g_chatManager->HPrint( 2, 180, "pos(%.2f,%.2f,%.2f)", g_mouseSelectedCharacter->position[0], 
			g_mouseSelectedCharacter->position[1], g_mouseSelectedCharacter->position[2] );
	}

	g_chatManager->SetOutputType( CHAT_OUTPUT_BORDER );

	
	
	


	g_chatManager->HPrint( 2, 60, "camPos : %10.4f %10.4f %10.4f , %d %d" , g_camera.m_position[0] , g_camera.m_position[1] , g_camera.m_position[2] , m_worldBSPs[ m_currWorld ]->m_totalRenderPolygon , m_MDLMng->m_totalRenderPolygon );

	g_chatManager->DPrint( 2, 80, "time : %10.6f %10.6f %10.6f %10.6f %10.6f %10.6f" , check.time0 , check.time1 , check.time2 , check.time3 , check.time4 , check.time5 );
	g_chatManager->DPrint( 2, 100, "time : %10.6f %10.6f %10.6f %10.6f %10.6f" , check.time6 , check.time7 , check.time8 , check.time9 , check.time10 );

	sum = check.time0 + check.time1 + check.time2 + check.time3 + check.time4 + check.time5 
	      + check.time6 + check.time7 + check.time8 + check.time9 + check.time10;
	fps = 1.0f / sum;

	g_chatManager->DPrint( 2, 120, "time : %10.6f %10.6f fps " , sum , fps );

	if ( g_cgv.displayDebugMessage )
	{

		g_chatManager->DPrint( 2, 140, "total send:%d, total recv:%d, send:%d, recv:%d, resend:%d, drop:%d, dup:%d", 
			totalPacketsSend, totalPacketsReceived,
			packetsSent, packetsReceived, packetsReSent, packetsDropCount, packetsDupCount );
			

		g_chatManager->DPrint( 2, 160, "acksend:%d, ackrecv:%d, un_send:%d, un_recv:%d, un_drop:%d", 
			packetsAckSent, packetsAckReceived, un_packetsSent, un_packetsReceived, un_packetsDropCount );










		g_chatManager->DPrint( 2, 180, "LatencyTime: %d, character Number: %d", g_cgv.latencyTime, g_charManager->m_iNumCharacters);

		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		g_chatManager->HPrint( 2, 200, "event : %d, aniSetType : %d, aniItemSetIndex : %d, aniSetID: %d" , m_myCharacter->event, m_myCharacter->ani_curSetType, m_myCharacter->ani_curItemSetIndex, m_myCharacter->ani_curSetID);

		g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
		g_chatManager->HPrint( 2, 220, "Name: %s, RA: %d, SA: %d, Speed: %f", m_myCharacter->name, m_myCharacter->curRA, m_myCharacter->curSA, m_myCharacter->moveSpeed);

		g_chatManager->DPrint( 2, 240, "Position: (%f, %f, %f), Yaw: %f", m_myCharacter->position[0], m_myCharacter->position[1], m_myCharacter->position[2], m_myCharacter->angles[ YAW ]);

		g_chatManager->DPrint( 2, 260, "mycharacterYaw: %f, mouseAngle: %f", m_myCharacter->angles[ YAW ], m_myCharacter->mouseAngle);


		
		svrPosition[0] = m_myCharacter->position[0] - m_worldBSPs[0]->m_models->bbox[0];
		svrPosition[1] = m_myCharacter->position[1] - m_worldBSPs[0]->m_models->bbox[1];
		svrPosition[2] = m_myCharacter->position[2] - m_worldBSPs[0]->m_models->bbox[2];
		g_chatManager->DPrint( 2, 280, "Server Position: (%f, %f, %f)", svrPosition[0], svrPosition[1], svrPosition[2]);

		g_chatManager->DPrint( 400, 280, "ChatManager Frame : %d, Vertex : %d", g_chatManager->m_frameFlag, g_chatManager->m_saveFrameFlag );

		g_chatManager->SetOutputType( CHAT_OUTPUT_BORDER );

		g_chatManager->HPrint( 2, 300, "Memory Info : MDL (%.2f/%.2f), Texture (%.2f/%.2f), Sound(%.2f/%.2f)",
			(float)m_MDLMng->m_resManager.GetUsedMemory() * 0.001f, (float)m_MDLMng->m_resManager.GetMaximumMemory() * 0.001f,
			(float)g_texMng.m_resManager.GetUsedMemory() * 0.001f, (float)g_texMng.m_resManager.GetMaximumMemory() * 0.001f,
			(float)g_musicMng->m_resManager.GetUsedMemory() * 0.001f, (float)g_musicMng->m_resManager.GetMaximumMemory() * 0.001f );


		for ( i=0; i<g_charManager->m_iNumCharacters; i++)
		{
			if ( g_charManager->m_Characters[i].entityType == ENTITY_PC)
				g_chatManager->HPrint( 2, 340 + i * 20, "%s(%d) => RA: %d, Speed: %f", g_charManager->m_Characters[i].name, g_charManager->m_Characters[i].idxOnServer, g_charManager->m_Characters[i].curRA, g_charManager->m_Characters[i].moveSpeed);
			else
				
			g_chatManager->HPrint( 2, 340 + i * 20, "%s(%d) => RA: %d, Speed: %f", 
			g_charManager->m_Characters[i].name, g_charManager->m_Characters[i].idxOnServer, g_charManager->m_Characters[i].curRA, g_charManager->m_Characters[i].moveSpeed);
		}
	}	

    return S_OK;
}





INT GTH::Run()
{
    
    HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

    
    BOOL bGotMsg;
    MSG  msg;
    msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

    while( WM_QUIT != msg.message && !m_bAppExit )
    {
        
        
		if( m_bActive )
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0U, 0U );

        if( bGotMsg )
        {
			
            if( 0 == TranslateAccelerator( m_hWnd, hAccel, &msg ) )
            {
				TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        else
        { 
            
            if( m_bActive && m_bReady )
            {
				
				
				
				
				
				

                if( FAILED( AppFrame() ) )
				{
                    SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
				}
				
#ifdef	_FMOD_SOUND
				FSOUND_Update ();
#endif
				

					
            }
        }
    }

	
	
	Cleanup3DEnvironment();

    return (INT)msg.wParam;
}


HRESULT GTH::BeginScene()
{
	HRESULT hr;

    
    if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
    {
		 
		m_renderEnabled	=	false;
		
        
        if( D3DERR_DEVICELOST == hr )
            return S_OK;

        
        if( D3DERR_DEVICENOTRESET == hr )
        {
            
            
            if( m_bWindowed )
            {
                D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
                m_pD3D->GetAdapterDisplayMode( m_dwAdapter, &pAdapterInfo->d3ddmDesktop );
                m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
            }

            if( FAILED( hr = Resize3DEnvironment() ) )
                return hr;
        }
        return hr;
    }

	 
	m_renderEnabled	=	true;

	extern int g_freeMove;
	if( g_freeMove )
	{
		
		m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			0, 1.0f, 0L );
	}
	else
	{
		
		



	    m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(0.5f, 0.5f, 0.6f, 1.0f), 1.0f, 0L);

	}

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return hr;

	return S_OK;
}


void GTH::EndScene()
{
	m_pd3dDevice->EndScene();

	static FLOAT fLastTime = 0.0f;
	static DWORD dwFrames  = 0L;
	FLOAT fTime = g_timer.GetAppTime();
	++dwFrames;

	
	if( fTime - fLastTime > 1.0f )
	{
		m_fFPS    = dwFrames / (fTime - fLastTime);
		fLastTime = fTime;
		dwFrames  = 0L;

		sprintf( m_strFrameStats, "%.2f", m_fFPS );

		
	}

	
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}








HRESULT GTH::OneTimeSceneInit()
{
	g_timeSystem->OneTimeSceneInit();
	g_timeSystem->SetDropperOrigin( &g_charManager->m_Characters [0].position [0] );
	 
	g_timeSystem->SetDropperCharHeight( &g_charManager->m_Characters [0].stat_height );
		

    
    SendMessage( m_hWnd, WM_PAINT, 0, 0 );

	m_bLoadingApp = FALSE;

    return S_OK;
}





HRESULT GTH::InitDeviceObjects()
{
	g_d3dDevice.SetD3DDevice(m_pd3dDevice);
	if( !InitApp() )
		return E_FAIL;

    return S_OK;
}







HRESULT GTH::DeleteDeviceObjects()
{
	int i;
	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :
		
	case CLIENT_LOGIN_PROCESS :
		
	case CLIENT_CHARACTER_PROCESS :
		
	case CLIENT_LOADING_PROCESS :
		

	
	case CLIENT_GAME_PROCESS : 
		for( i = 0 ; i < m_numWorldBSPs ; i++ )
			GTH_SAFE_DELETE( m_worldBSPs[ i ] );
		
		m_numWorldBSPs = 0;
		
		GTH_SAFE_DELETE( m_MDLMng );

		g_timeSystem->DeleteDeviceObjects();

		break;
	}

    return S_OK;
}





HRESULT GTH::InvalidateDeviceObjects()
{
    
    SAFE_RELEASE( m_pD3DXFont );

	int i;

	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS : case CLIENT_LOGIN_PROCESS : case CLIENT_CHARACTER_PROCESS :
		


		

		g_ifMng->InvalidateDeviceObjects();
		g_chatManager->InvalidateFont();
		
		
		for( i = 0 ; i < m_numWorldBSPs ; i++ )
		{
			m_worldBSPs[ i ]->Release();
			GTH_SAFE_DELETE( m_worldBSPs[ i ] );
		}
		memset ( m_worldBSPs, 0, sizeof(m_worldBSPs) );

		m_numWorldBSPs = 0;
		if( m_MDLMng )
			m_MDLMng->Release();
		GTH_SAFE_DELETE( m_MDLMng );
		
		g_shaderMng.Release();
		g_modelShaderMng->Release();
		g_charShaderMng->Release();

		g_charManager->ReleaseBuffers();

		g_texMng.Release();
		break;

	case CLIENT_LOADING_PROCESS :
		
		g_ifMng->InvalidateDeviceObjects();
		g_chatManager->InvalidateFont();
		g_texMng.Release();
		
		break;

	
	case CLIENT_GAME_PROCESS : 
		


		
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
		break;
	}
	





    return S_OK;
}







HRESULT GTH::FinalCleanup()
{
		 
	g_timeSystem->FinalCleanup();
		

    
    WriteSettings();

    return S_OK;
}






void GTH::WritePackFileLog( char* str, ... )
{
#ifdef _DEBUG
	FILE		*fp;
	char		buf[4096];
	va_list		marker;

	memset( buf, 0, sizeof( buf ) );
	
	va_start( marker, str );
	vsprintf( buf, str, marker);

	fp = fopen( "gth.log", "at" );
	
	fprintf( fp, "%s\n", buf );

	fclose( fp );
#endif
}

LRESULT GTH::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	HIMC m_hIMC = ImmGetContext(hWnd);

	DWORD dwBufLen;

	LPCANDIDATELIST m_CandList;
	LPCANDIDATELIST lpCandList;
	
	int	 CanNum;


    switch( msg )
    {
		



	case WM_IME_NOTIFY:

		switch( wParam )
		{
		case IMN_CHANGECANDIDATE:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_CHANGECANDIDATE", wParam );
			break;
		case IMN_CLOSECANDIDATE:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_CLOSECANDIDATE", wParam );
			break;
		case IMN_CLOSESTATUSWINDOW:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_CLOSESTATUSWINDOW", wParam );
			break;
		case IMN_GUIDELINE:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_GUIDELINE", wParam );
			break;
		case IMN_OPENCANDIDATE:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_OPENCANDIDATE", wParam );
			break;

		case IMN_SETCANDIDATEPOS:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_SETCANDIDATEPOS", wParam );
			break;
		case IMN_SETCOMPOSITIONFONT:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_SETCOMPOSITIONFONT", wParam );
			break;
		case IMN_SETCOMPOSITIONWINDOW:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_SETCOMPOSITIONWINDOW", wParam );
			break;
		case IMN_SETCONVERSIONMODE:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_SETCONVERSIONMODE", wParam );
			break;
		case IMN_SETOPENSTATUS:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_SETOPENSTATUS", wParam );
			break;
		case IMN_SETSENTENCEMODE:
			ShowLogInfo( "WM_IME_NOTIFY : IMN_SETSENTENCEMODE", wParam );
			break;
		case IMN_SETSTATUSWINDOWPOS :
			ShowLogInfo( "WM_IME_NOTIFY : IMN_SETSTATUSWINDOWPOS", wParam );
			
			break;
		}




		
	case WM_GRAPHNOTIFY:
		{
			 
#ifndef	_FMOD_SOUND

#endif
		}

	
		
		break;
    }

	
	if( g_cgv.enableChat && !g_ifMng->m_msgWin->State() )
	{
		switch( g_chatManager->GetString( hWnd, msg, wParam, lParam ) )
		{
		case CHAT_INPUT_RETURN_NONE :
			return 0;
		
		case CHAT_INPUT_RETURN_OK :
			{
				switch( g_cgv.state )
				{
					
				case CLIENT_GAME_PROCESS :
					{
						switch( g_chatManager->GetCurrentInputIndex() )
						{
							
						case INPUT_INDEX_DIALOGBOX :						


							
							if(TRUE == g_pApp->m_myCharacter->m_bWaitingRoom)
							{
								chatMessage	tempstr;
								strcpy(tempstr,g_chatManager->m_returnStr);
								if(NULL != strchr(tempstr,' ')) *strchr(tempstr,' ')=NULL;
								
								
								if(0 != strcmp(tempstr,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,385))){
								
									g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,148), D3DCOLOR_ARGB(255, 55, 255, 55) );
									g_ifMng->m_dlgWin->SetSendString();
									break;
								}
							}

							
							if(g_cgv.myCharacterInfo->m_dwLimite_ChatTime<=0.0f)
							{
								if(g_pApp->m_myCharacter->isTransform == 1 &&
								   g_pApp->m_myCharacter->transformEntityIdx == 237)
								{
									
									sprintf( g_chatManager->m_returnStr, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,399));

								}

								GTH_ChatCommand( g_chatManager->m_returnStr );
								g_ifMng->m_dlgWin->SetScrollBarPositionMax();

							}
							break;
							
							
						case INPUT_INDEX_BOOTH_NAME :
							g_chatManager->ChangeInputMessage( INPUT_INDEX_BOOTH_INFO );
						

							break;
							
						case INPUT_INDEX_BOOTH_INFO :
							g_chatManager->ChangeInputMessage( INPUT_INDEX_BOOTH_NAME );
							break;
							
						
						case INPUT_INDEX_MAIL_NAME :
							g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_TITLE );
							break;
						case INPUT_INDEX_MAIL_TITLE :
							g_ifMng->m_mailEditWin->m_bVisibleCursor = true;
							g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_CONTENT );
							break;
						case INPUT_INDEX_MAIL_CONTENT :
							break;

						case INPUT_INDEX_GUILD_SCHEDULE :
							{
								strcpy( g_ifMng->m_guildWin->m_scheduleString, g_chatManager->m_returnStr );

								memset( g_chatManager->m_returnStr, 0, sizeof( chatMessage ) );
								g_chatManager->InitInputStr( INPUT_INDEX_GUILD_SCHEDULE );

								g_ifMng->m_guildWin->m_scheduleBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

								GTH_SendMessage_Guild_Schedule_Request( g_ifMng->m_guildWin->m_scheduleString );
								g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
							}
							
							break;
							
						case INPUT_INDEX_WORLDCHAT :
							{
								
								if(g_cgv.myCharacterInfo->m_dwLimite_ChatTime<=0.0f)
								{
									char msg[CHATSTRING];
									strcpy( msg, g_chatManager->m_returnStr );
									
									if( msg[0] != 0 )
										GTH_SendMessage_WorldChatting( msg, g_ifMng->m_worldChatWin->GetType());
									
									g_chatManager->InitInputStr( INPUT_INDEX_WORLDCHAT );
									g_ifMng->m_worldChatWin->Enable( false );
									
								
								}
								
								memset( g_chatManager->m_returnStr, 0, sizeof( g_chatManager->m_returnStr ) );
								g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
							}
							break;
					
						case INPUT_INDEX_POST_NAME:
							g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_MESSAGE);
							break;

						case INPUT_INDEX_POST_TITLE:
							g_ifMng->m_mailBoxWin->m_bVisibleCursor = true;
							g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_NAME);
							break;

						case INPUT_INDEX_POST_MESSAGE:
							break;

					
						case INPUT_INDEX_GIFT_CARD_ID:							
							g_chatManager->ChangeInputMessage(INPUT_INDEX_GIFT_CARD_ID);
							break;
						case INPUT_INDEX_GIFT_CARD_PASSWORD:
							g_chatManager->ChangeInputMessage(INPUT_INDEX_GIFT_CARD_PASSWORD);
							break;

					
						default:
							break;
						}
					}
					break;
				}
			}
			return 0;

		case CHAT_INPUT_RETURN_TAB :
			switch( g_cgv.state )
			{
			case CLIENT_LOGIN_PROCESS :
				g_chatManager->m_curIdx ++;
				g_chatManager->m_curIdx = g_chatManager->m_curIdx % 2;
				break;
			case CLIENT_GAME_PROCESS :
				{
					switch( g_chatManager->GetCurrentInputIndex() )
					{
						
					case INPUT_INDEX_BOOTH_NAME :
						g_chatManager->ChangeInputMessage( INPUT_INDEX_BOOTH_INFO );
						break;
						
					case INPUT_INDEX_BOOTH_INFO :
						g_chatManager->ChangeInputMessage( INPUT_INDEX_BOOTH_NAME );
						break;
					case INPUT_INDEX_MAIL_NAME :
						g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_TITLE );
						break;
					case INPUT_INDEX_MAIL_TITLE :
						g_ifMng->m_mailEditWin->m_bVisibleCursor = true;
						g_chatManager->ChangeInputMessage( INPUT_INDEX_MAIL_CONTENT );
						break;
					case INPUT_INDEX_MAIL_CONTENT :
						break;
				
					case INPUT_INDEX_POST_NAME:
						g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_MESSAGE);
						break;
						
					case INPUT_INDEX_POST_TITLE:
						g_ifMng->m_mailBoxWin->m_bVisibleCursor = true;
						g_chatManager->ChangeInputMessage(INPUT_INDEX_POST_NAME);
						break;
						
					case INPUT_INDEX_POST_MESSAGE:
						break;
				

					
					case INPUT_INDEX_GIFT_CARD_ID:
						g_chatManager->ChangeInputMessage(INPUT_INDEX_GIFT_CARD_PASSWORD);
						break;

					case INPUT_INDEX_GIFT_CARD_PASSWORD:
						g_chatManager->ChangeInputMessage(INPUT_INDEX_GIFT_CARD_ID);					
						break;


					}
				}
				break;
			}
			return 0;

		case CHAT_INPUT_RETURN_EXIT :
			switch( g_cgv.state )
			{
				
			case CLIENT_GAME_PROCESS : 
				GTH_ChatCommand( "" );
				break;
			}
			return 0;
		}
	}

	if( g_input->Handle_messages( hWnd, msg, wParam, lParam ) ) 
		return 0;

    return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}





HRESULT GTH::RestoreDeviceObjects()
{   
	g_camera.m_projectParm.aspect = (float) m_d3dsdBackBuffer.Width / (float) m_d3dsdBackBuffer.Height;
    D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)&g_camera.m_projMat , deg2rad( g_camera.m_projectParm.fov / g_camera.m_projectParm.aspect ) 
		                      , g_camera.m_projectParm.aspect , g_camera.m_projectParm.znear , g_camera.m_projectParm.zfar );
	g_camera.SetupViewPort( 0 , 0 , m_d3dsdBackBuffer.Width , m_d3dsdBackBuffer.Height 
		                   , g_camera.m_projectParm.znear , g_camera.m_projectParm.zfar );

	g_cursor->SetupViewPort( 0 , 0 , m_d3dsdBackBuffer.Width , m_d3dsdBackBuffer.Height 
		                   , g_camera.m_projectParm.znear , g_camera.m_projectParm.zfar );

	g_input->SetHWND( m_hWnd );
	g_input->Resize( m_d3dpp.BackBufferWidth,m_d3dpp.BackBufferHeight );

	RestoreApp();

    return S_OK;
}

    





	
	