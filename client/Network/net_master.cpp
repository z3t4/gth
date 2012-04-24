#include "../global_def.h"
#include "../MfcLib/MfcLib.h"


extern i3socket_t msSocket;	
extern i3sizebuf_t netMessage; 
extern unsigned int g_netTime;












int GTH_Network_EstablishMasterServerConnection()
{
	char cErrorMsg[256];
	int tryCount;
	int masterIdx;
	int state;

	tryCount = 0;
	
	masterIdx = F_RANDOMBETWEEN(0, g_cgv.masterServerNumber-1);
	if (masterIdx < 0) return 0;

	do
	{
		tryCount ++;
		//lucky 2011 IP of master server
		if ( !NET_OpenSocketForClient(&msSocket, "88.190.30.48", 26000 ) )
		{
			


			wsprintf(cErrorMsg, g_LPACK.GetMassage(LPACK_TYPE_NORMAL,252), WSAGetLastError()); 
			MessageBox(g_pApp->m_hWnd, cErrorMsg, g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), MB_OK);

			return 0;
		}

		state = GTH_ConnectMasterServer(); 
		if (state == 1)
		{
			
			g_cgv.onMasterServer = true;
			return 1;
		}
		else if (state == 2 || state == 3 || state == 4) return 0;

		masterIdx ++;
		if (masterIdx >= g_cgv.masterServerNumber) 
			masterIdx = 0;

	} while ( tryCount < g_cgv.masterServerNumber );

	
	
	if( g_cgv.state == CLIENT_START_PROCESS ) MessageBox(g_pApp->m_hWnd, g_LPACK.GetMassage(LPACK_TYPE_NORMAL,253), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), MB_OK);
	return 0;
}


int GTH_NewVersionCheck()
{
	int response;

	response = MSG_ReadByte();

	if( response == 1 )
	{
		return 1;
	}

	return 0;
}
	
int GTH_ConnectMasterServer()
{
	int command;
	int state;
	unsigned int waitStartTime;
	int reason;
	char msg[256];

	

#if defined (_AHNLAB_HACKSHIELD) && defined (_GTH_MALAYSIA_CHN_VERSION )
	g_AhnLab.GTH_SendMessageGameVersionCheck();
#else
	GTH_SendMessage_ConnectMasterServer();
#endif	
	

	waitStartTime = NET_GetNetTime(); 
	while (1)
	{
		if (g_netTime - waitStartTime > CONNECT_WAITTIME) 
		{
			MessageBox(g_pApp->m_hWnd, g_LPACK.GetMassage(LPACK_TYPE_NORMAL,253), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), MB_OK);
			return 0;
		}
		
		g_netTime = NET_GetNetTime();

		state = GTH_Network_GetMessage(&msSocket); 

		if (state != PacketAnalysis_Start )
			continue;
				
		MSG_BeginReading();
		command = MSG_ReadByte();

		switch (command)
		{
		case MSC_ACCEPTCLIENT :        
			return 1;

		case MSC_REJECTCLIENT:
			reason = MSG_ReadByte();
			switch ( reason )
			{
			case 0 :
				
				if( GTH_NewVersionCheck() == 1 )
					return 1;

				
				MessageBox(g_pApp->m_hWnd, g_LPACK.GetMassage(LPACK_TYPE_NORMAL,254), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), MB_OK);
				return 2;
			case 1 :
				
				MessageBox(g_pApp->m_hWnd, g_LPACK.GetMassage(LPACK_TYPE_NORMAL,255), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), MB_OK);
				return 3;
			case 2 :
				strcpy(msg, MSG_ReadString());	
				
				MessageBox(g_pApp->m_hWnd, msg, g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), MB_OK);
				return 4;
			default :
				break;
			}
			return 0;	
			break;


		
		case EXTEND_SECOND:
			{
				enum enumExtendSecondPacket SecondPackaet;	
				SecondPackaet = static_cast<enum enumExtendSecondPacket>(MSG_ReadShort());
				switch(SecondPackaet)
				{
				case 0:
					
					break;
				case 1:
					
					break;
				}
			}
			break;
			
				

		default :
			
			


			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,41), 1, IF_MESSAGE_EXIT_CLIENT );

			break;
		}
	} 

	return 0;
}

int GTH_MasterServer()
{
	int command;
	int state;


	
	GTH_Network_CheckProblemSocket(&msSocket);

	state = GTH_Network_GetMessage(&msSocket); 

	if (state < 0) return 0;

	if (state != PacketAnalysis_Start) 
		return 0;
			
	MSG_BeginReading();
	command = MSG_ReadByte();

	switch (command)
	{
	case MSC_REPLYLOGIN:        
		GTH_ProcessMessage_ReplyLogin();
		break;

	case MSC_NOTICEINFO:          
		GTH_NoticeInfo();
		break;

	case MSC_SERVERGROUPINFO :     
		GTH_ProcessMessage_ReplyServerGroupInfo();
		break;

	case MSC_GAMEDATA:
		GTH_ProcessMessage_ReplyGameData();
		break;

	case MSC_GAMESERVERADDR:
		GTH_ProcessMessage_ReplyServerAddress();
		return 1;

	case MSC_REPLY_READYCREATE:
		GTH_ProcessMessage_ReplyReadyCreate();
		break;

	case MSC_REPLY_CREATECHARACTER:
		GTH_ProcessMessage_ReplyCreateCharacter();
		break;

	case MSC_REPLY_DELETECHARACTER:
		GTH_ProcessMessage_ReplyDeleteCharacter();
		break;

	case MSC_REPLY_CHECKCHARNAME:
		GTH_ProcessMessage_ReplyCheckCharacterName();
		break;

	
	case MSC_LOAD_CHARACTER_SLOT:
		GTH_ProcessMessage_ReplyLoadCharacterSlot();
		break;

	
	case EXTEND_SECOND:
		{
			enum enumExtendSecondPacket SecondPackaet;	
			SecondPackaet = static_cast<enum enumExtendSecondPacket>(MSG_ReadShort());
			switch(SecondPackaet)
			{
			case AHNLAB:
			
				break;
			case AHNLAB_ERROR:
			
				break;
			}							
		}
		break;

		
	case GC_RELIABLE_PING: case MSC_REJECTCLIENT :
	case GC_UNRELIABLE_PING: case MSC_ACCEPTCLIENT :
		break;

	default :
		
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,41), 1, IF_MESSAGE_EXIT_CLIENT );
		return 0;
		
	}

	return 0;
}

void GTH_SendMessage_ConnectMasterServer()
{
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_CONNECT_MASTERSERVER);
		MSG_WriteString(&netMessage, g_cgv.gameVersion);
		MSG_WriteString(&netMessage, g_cgv.subVersion);		
		NET_SendMessage(&msSocket, &netMessage);		

	}	
	MSG_EndWriting( &netMessage );	
}

void GTH_SendMessage_RequestLogin(char *id, char *password)
{
	
	g_ifMng->m_loginWin->m_loginOkBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	if( !GTH_LockSending() ) return;

    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_LOGIN);
		MSG_WriteString(&netMessage, id);
		MSG_WriteString(&netMessage, password);		
		
		
		NET_SendMessage(&msSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}


#ifdef _TERRA
#include "..\Terra ICT\CTerraCtrl.h"
extern CTerraCtrl gcTerraCtrl;
#endif



#include "..\tagMCPacket.h"
void GTH_ProcessMessage_ReplyLogin_Continue(void);
void GTH_ProcessMessage_ReplyLogin_Continue_China(void);

void GTH_ProcessMessage_ReplyLogin()
{
	enum tagMCPaket_MSC_REPLYLOGIN::enumCertification Certification = 
		(enum tagMCPaket_MSC_REPLYLOGIN::enumCertification)MSG_ReadByte();
	switch(Certification){		
		case tagMCPaket_MSC_REPLYLOGIN::enumCertification::nick_name:
			#ifndef _TERRA
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,256), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_REPLY_LOGIN);
			return;
			#endif


		case tagMCPaket_MSC_REPLYLOGIN::enumCertification::real_name:
		case tagMCPaket_MSC_REPLYLOGIN::enumCertification::developer:
			break;
	}

	GTH_ProcessMessage_ReplyLogin_Continue();
}


void GTH_ProcessMessage_ReplyLogin_Continue_Error(const enum tagMCPaket_MSC_REPLYLOGIN::enumError& ErrorCode);
void GTH_ProcessMessage_ReplyLogin_Continue(void)
{
	
	g_ifMng->m_loginWin->m_loginOkBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
	GTH_UnlockSending();
	g_ifMng->ExitMessage();
	memset( g_chatManager->m_returnStr, 0, sizeof( chatMessage ) );	
	BOOL    bSUCCESSorFAIL=FALSE;
	

	bSUCCESSorFAIL = (BOOL)MSG_ReadByte();
	if(FALSE == bSUCCESSorFAIL){
		enum tagMCPaket_MSC_REPLYLOGIN::enumError ErrorCode=tagMCPaket_MSC_REPLYLOGIN::enumError::none;
		ErrorCode = (enum tagMCPaket_MSC_REPLYLOGIN::enumError)MSG_ReadByte();
		GTH_ProcessMessage_ReplyLogin_Continue_Error(ErrorCode);
		
	}else{		
		strncpy( g_cgv.clientID,MSG_ReadString(),IDSTRING);
		g_cgv.clientID[IDSTRING-1]=NULL;

		#if defined (_GTH_TERRA_VERSION) || (_GTH_MALAYSIA_CHN_VERSION)						
		gcTerraCtrl.GTH_ProcessMessage_ReplyLogin_SetID(g_cgv.clientID);
		#endif


		gGameConfigFlag.m_bCharacter_Storage= MSG_ReadByte();

		#if defined (_GTH_CHINESE_VERSION)
		GTH_ProcessMessage_ReplyLogin_Continue_China();
		#endif

		

		
		switch(g_cgv.restartState){
			
			case RESTART_NONE : 
			case RESTART_SERVER : 
			case RESTART_RESERVER :
					g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_SELECT_SERVER );
				break;
			case RESTART_CHARACTER :
				break;
		}

		#ifdef _GTH_ONLINE_VERSION
		GTH_SendMessage_RequestServerGroup();
		#endif
	}

}

void GTH_ProcessMessage_ReplyLogin_Continue_China(void)
{	
	char	billMethod[2] ;			
	int		billRemain;
	char	billExpire[12] ;
	char	msgForBill[256];


	
	
	
	billMethod[0]	= MSG_ReadChar();
	billMethod[1]	= MSG_ReadChar();
	billRemain		= MSG_ReadLong();
	billExpire[0]	= MSG_ReadChar();
	billExpire[1]	= MSG_ReadChar();
	billExpire[2]	= MSG_ReadChar();
	billExpire[3]	= MSG_ReadChar();
	billExpire[4]	= MSG_ReadChar();
	billExpire[5]	= MSG_ReadChar();
	billExpire[6]	= MSG_ReadChar();
	billExpire[7]	= MSG_ReadChar();
	billExpire[8]	= MSG_ReadChar();
	billExpire[9]	= MSG_ReadChar();
	billExpire[10]	= MSG_ReadChar();
	billExpire[11]	= MSG_ReadChar();
	
	if( billMethod[0] != 0 )
	{
		if( billMethod[0] =='F' && billMethod[1] =='M' ) 
		{
			
			sprintf( msgForBill, g_LPACK.GetMassage( 0, 944),
									billExpire[0],
									billExpire[1],
									billExpire[2],
									billExpire[3],
									billExpire[4],
									billExpire[5],
									billExpire[6],
									billExpire[7],
									billExpire[8],
									billExpire[9],
									billExpire[10],
									billExpire[11] );

			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), msgForBill );
		}
		else
		{
			if( billMethod[0] == 'T' && billMethod[1] == 'B' ) 
			{
				int remainhour = billRemain / 3600;
				if( 0 == remainhour )
				{
					int remainMin = billRemain / 60;
					
					sprintf( msgForBill, g_LPACK.GetMassage( 0, 945 ), remainMin );
					g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), msgForBill );
				}
				else
				{
					
					sprintf( msgForBill, g_LPACK.GetMassage( 0, 946 ), remainhour );
					g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), msgForBill );
				}
			}
		}
	}
}






void GTH_ProcessMessage_ReplyLogin_Continue_Error(const enum tagMCPaket_MSC_REPLYLOGIN::enumError& ErrorCode)
{
	switch(ErrorCode ){
		case tagMCPaket_MSC_REPLYLOGIN::enumError::mismatch_password:

			#ifdef _TERRA
			
			
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105),
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,106),
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_EXIT_CLIENT );
			#else
			
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105),
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,106),
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_NONE );
			#endif
			break;

		case tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user:
			#ifdef _TERRA
			
			
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,107),
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_EXIT_CLIENT );
			#else
			
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,107),
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_NONE );
			#endif
			break;





		case tagMCPaket_MSC_REPLYLOGIN::enumError::double_login:

			#ifdef _TERRA
			
			
			g_ifMng->SetMessage( 
					g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
					g_LPACK.GetMassage(LPACK_TYPE_NORMAL,108), 
					MSG_BUTTONTYPE_OK, 
					IF_MESSAGE_EXIT_CLIENT );
			#else
			
			g_ifMng->SetMessage( 
					g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
					g_LPACK.GetMassage(LPACK_TYPE_NORMAL,108), 
					MSG_BUTTONTYPE_OK, 
					IF_MESSAGE_NONE );

			#endif

			break;


			

		case tagMCPaket_MSC_REPLYLOGIN::enumError::database_error:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,109), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_EXIT_CLIENT );
			break;


		case tagMCPaket_MSC_REPLYLOGIN::enumError::block_user:

			#ifdef _TERRA
			
			
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,453), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_EXIT_CLIENT );
			#else
			
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,453), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_NONE );
			#endif
			break;


		
		
		case tagMCPaket_MSC_REPLYLOGIN::enumError::billing_network_error:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMessage(0,940), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_EXIT_CLIENT);
			break;


		case tagMCPaket_MSC_REPLYLOGIN::enumError::billing_point_not_enough:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105),
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,941), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_EXIT_CLIENT );
			break;
		
		case tagMCPaket_MSC_REPLYLOGIN::enumError::billing_error:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMessage(0,942), 
				1, 
				IF_MESSAGE_EXIT_CLIENT );
			break;

		case tagMCPaket_MSC_REPLYLOGIN::enumError::not_check_real_name:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,256), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_EXIT_CLIENT);			
			break;


		default:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,105), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,110), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_EXIT_CLIENT );
			break;
	}
}



void GTH_SendMessage_DisconnectMasterServer() 
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_DISCONNECT_MASTERSERVER);
		
		
		NET_SendUnreliableMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_RequestNotice()
{
	if( !GTH_LockSending() ) return;


	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_NOTICE);
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_RequestServerGroup() 
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_SERVERGROUP);
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void GTH_ProcessMessage_ReplyServerGroupInfo()
{
	GTH_UnlockSending();

	
	int iServerGroupCount = 0;

	
	












	
	std::vector<serverGroup_t>* vpServerList = 	&g_ifMng->m_ServerListWin->m_vpServerList;		
	iServerGroupCount	= MSG_ReadByte(); 

	

	if ( iServerGroupCount > MAX_SERVER_GROUP)
		iServerGroupCount = MAX_SERVER_GROUP;

	if ( (g_ifMng->m_ServerListWin->m_ServerSort_Flag != true) && 
		(g_ifMng->m_ServerListWin->m_Server_State_Sort_Flag != true) ) 
	{
		
		vpServerList->clear();		
		g_ifMng->m_ServerListWin->m_ActiveServerCount = 0;
		
		
		for (int iServerGroupIdx=0; (iServerGroupIdx< iServerGroupCount) &&
			(iServerGroupIdx < MAX_SERVER_GROUP) ; iServerGroupIdx++)		
		{
			g_cgv.serverGroup[iServerGroupIdx].active = MSG_ReadByte();
			g_cgv.serverGroup[iServerGroupIdx].idx = MSG_ReadByte();
			strcpy(g_cgv.serverGroup[iServerGroupIdx].name, MSG_ReadString());
			g_cgv.serverGroup[iServerGroupIdx].clientNumber = MSG_ReadShort();
			g_cgv.serverGroup[iServerGroupIdx].access = MSG_ReadByte();			
			vpServerList->push_back(g_cgv.serverGroup[iServerGroupIdx]);
			
			if ( g_cgv.serverGroup[iServerGroupIdx].active == TRUE)
				g_ifMng->m_ServerListWin->m_ActiveServerCount++;			
			
		}		
	}	
	std::sort(vpServerList->begin(), vpServerList->end(),
		LessServerGroup_descending_state(g_ifMng->m_ServerListWin->m_Server_Sort));

	if( g_cgv.restartState == RESTART_CHARACTER )
	{
		g_ifMng->m_loginWin->m_cameraStep = 0; 
		g_ifMng->m_loginWin->m_selectedCharacter = NULL;

#ifdef _GTH_ONLINE_VERSION
		GTH_SendMessage_RequestGameData();
#endif
		return;
	}
	if( g_cgv.restartState == RESTART_SERVER )
	{
		g_ifMng->m_loginWin->m_cameraStep = 0; 
		g_ifMng->m_loginWin->m_selectedCharacter = NULL;
		return;
	}
	
	g_ifMng->m_ServerListWin->Enable();	

}

void GTH_SendMessage_RequestGameData()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_GAMEDATA);
		MSG_WriteByte(&netMessage, g_cgv.selectedGroupIdx);
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ReplyGameData()
{
	GTH_UnlockSending();

	GTH_SetSavedData();
	if ( g_cgv.isReceiveAllSavedData )
	{			



		
		if( g_cgv.state == CLIENT_LOGIN_PROCESS )
		{
			
			g_ifMng->m_changeStateFlag = true;				

		}
		
		else if( g_cgv.state == CLIENT_CHARACTER_PROCESS )
		{
			
			syncCharacter_t* selectCharacter;
			vec3_t delta;
			int i;

			g_charManager->m_iNumCharacters = 0;
			g_charManager->m_iNumBackupCharacters = 0;

			GTH_GenerateMyCharacter();			
			
			VectorClear( delta );
			for( i = 0; i < g_cgv.syncCharacterNumber; i ++ )
			{
				selectCharacter = &g_cgv.syncCharacter[i];
				g_move->Pmove( selectCharacter->position , delta , selectCharacter->angles );
			}			
			g_charManager->GenerateCharacter( g_cgv.syncCharacter, g_cgv.syncCharacterNumber );
			
			
			if( g_ifMng->m_loginWin->m_deleteFlag )
			{
				g_ifMng->m_loginWin->m_cameraStep = 1;
				g_ifMng->m_loginWin->m_deleteFlag = false;

				
				CreateDefaultSelectChar();
				

			}
			else if( g_ifMng->m_loginWin->m_makeFlag )
			{
				g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_SELECT_CHARACTER );
				g_ifMng->m_loginWin->m_makeFlag = false;
				g_ifMng->m_loginWin->m_cameraStep = 3 + g_ifMng->m_loginWin->m_curSelectedMakeCharacter;
				g_ifMng->m_loginWin->m_cameraStepFlag = false;

				
				CreateDefaultSelectChar();
				
				
			}













			
				CreateDefaultSelectChar();
			

		}
		g_cgv.isReceiveAllSavedData = false;
		g_cgv.savedDataNumber = 0;

		
		
		

	}

	
	
	GTH_SendMessage_Request_LoadCharacterSlot();
}

void GTH_SendMessage_RequestGameServerAddr()
{
	if( !GTH_LockSending() ) return;

    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_GAMESERVERADDR);				
		
		
		MSG_WriteByte(&netMessage,g_cgv.savedData[ g_cgv.selectedGameDataIdx ].characterID);
		
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ReplyServerAddress()
{
	GTH_UnlockSending();

	g_ifMng->ExitMessage();
#ifdef _GTH_ONLINE_VERSION
	g_cgv.changeServerFlag = true;
	
	g_cgv.sendGameServerConnectFlag = true;
	
	strcpy( g_cgv.gameServerIP, MSG_ReadString() );
	g_cgv.gameServerPort = MSG_ReadShort();
	g_cgv.savedData[ g_cgv.selectedGameDataIdx ].worldIdx = MSG_ReadByte();


	GTH_SendMessage_DisconnectMasterServer();

	Sleep( 1000 );

	g_cgv.onMasterServer = false;
	g_cgv.onGameServer = true;

	GTH_Network_OpenGameServerSocket();
#endif
	g_cgv.myCharacterInfo = &g_cgv.savedData[g_cgv.selectedGameDataIdx];
	
	g_ifMng->m_changeStateFlag = true;
}

void GTH_NoticeInfo()
{
	int i;

	g_cgv.noticeNumber = MSG_ReadByte();
	for (i=0; i<g_cgv.noticeNumber; i++)
	{
		strcpy(g_cgv.notice[i].date, MSG_ReadString());
		strcpy(g_cgv.notice[i].content, MSG_ReadString());
	}
}

int GTH_SetSavedData()
{
	int charNumber;
	int j, k;
	myCharacterInfo_t *myCharacterInfo;
	item_t *item;
	skill_t *skill;
	int itemIdx, skillIdx;
	int characterID;
	questStatus_t *questStatus;
	int questStatusIdx;


	if ( g_cgv.savedDataNumber == 0 )
	{
		
		GTH_InitSavedData();
	}
	charNumber = MSG_ReadByte();


	if ( charNumber == 0 ) 
	{
		g_cgv.savedDataNumber = 0;
		g_cgv.isReceiveAllSavedData = true;
		return 1;
	}

	g_cgv.savedDataNumber ++;

	if (charNumber == g_cgv.savedDataNumber)
	{
		g_cgv.isReceiveAllSavedData = true;
	}

	characterID = MSG_ReadByte();
	if ( characterID < 0 || characterID >= MAX_SAVED_DATA) 
	{
		
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,46), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,47), 1, IF_MESSAGE_EXIT_CLIENT );
		return 0;
	}

	myCharacterInfo = &g_cgv.savedData[ characterID ];

	myCharacterInfo->characterID = characterID;
	myCharacterInfo->pcClass = MSG_ReadByte();
	myCharacterInfo->pcJob = MSG_ReadByte();

	myCharacterInfo->skinPart[0] = MSG_ReadByte();
	myCharacterInfo->skinPart[1] = MSG_ReadByte();
	myCharacterInfo->skinPart[2] = MSG_ReadByte();
	myCharacterInfo->skinPart[3] = MSG_ReadByte();

	strcpy(myCharacterInfo->name, MSG_ReadString());
	myCharacterInfo->age = MSG_ReadLong(); 
	strcpy(myCharacterInfo->rank, MSG_ReadString());
	
	
	
	myCharacterInfo->rankPoint = MSG_ReadLong();
	myCharacterInfo->worldIdx = MSG_ReadByte();
	
	
	myCharacterInfo->level = MSG_ReadByte();
	myCharacterInfo->exp = MSG_Read64Int();
	myCharacterInfo->increaseExpRate = MSG_ReadByte();

	myCharacterInfo->selectedGen = MSG_ReadByte();
	myCharacterInfo->genLevel = MSG_ReadByte();
	myCharacterInfo->genExp = MSG_Read64Int();
	myCharacterInfo->increaseGenExpRate = MSG_ReadByte();
	for (j=0; j<GEN_NUMBER; j++)
		myCharacterInfo->genCapability[j] = MSG_ReadLong();

	myCharacterInfo->force = MSG_ReadShort();
	myCharacterInfo->soulpower = MSG_ReadShort();
	myCharacterInfo->agility = MSG_ReadShort();
	myCharacterInfo->vitality = MSG_ReadShort();

	myCharacterInfo->bonusStatusPoint = MSG_ReadShort();

	myCharacterInfo->curRA = MSG_ReadLong();
	myCharacterInfo->curSA = MSG_ReadShort();
	myCharacterInfo->curChargeNSE = MSG_ReadLong();
	myCharacterInfo->curChargeSE = MSG_ReadLong();
	myCharacterInfo->curChargeRA = MSG_ReadLong();
	myCharacterInfo->curChargeSA = MSG_ReadLong();
	myCharacterInfo->depotSE = MSG_ReadLong();

	myCharacterInfo->curWeapon = MSG_ReadByte();
	myCharacterInfo->itemNumber = MSG_ReadByte();

	for (j=0; j<MAX_INVENTORY_SIZE; j++)
		myCharacterInfo->inventory[j] = MSG_ReadChar();

	for (j=0; j<MAX_DEPOT_SIZE; j++)
		myCharacterInfo->depot[j] = MSG_ReadChar();

	for (j=0; j<MAX_EQUIPMENT; j++)
		myCharacterInfo->equipment[j] = MSG_ReadChar();

	myCharacterInfo->mouseInventory = MSG_ReadChar();

	
	myCharacterInfo->bonusSkillPoint = MSG_ReadByte();

	for (j=0; j<MAX_NUMBER_OF_SKILL_VARIABLE; j++)
		myCharacterInfo->skillVariable[j] = MSG_ReadShort();

	
	for (j=0; j<MAX_PRECOCITY_SIZE; j++)
	{
		myCharacterInfo->precocityInventory[j] = MSG_ReadChar();
		myCharacterInfo->precocityTime[j] = MSG_ReadLong();
	}

	
	
	myCharacterInfo->chaosPoint = MSG_ReadShort();
	
	myCharacterInfo->guildIdx = MSG_ReadShort();
	myCharacterInfo->guildJoinTime = MSG_ReadLong();
	myCharacterInfo->guildSecedeTime = MSG_ReadLong();

	
	for (j=0; j<myCharacterInfo->itemNumber; j++)
	{
		itemIdx = MSG_ReadByte();
		item = &myCharacterInfo->item[itemIdx];

		strcpy(item->name, MSG_ReadString());
		item->itemTableIdx = MSG_ReadShort();
		item->optionNumber = MSG_ReadByte();

		for (k=0; k<item->optionNumber; k++)
			item->optionIdx[k] = MSG_ReadShort();

		for (k=0; k<item->optionNumber; k++)
		{
			item->optionValue[k] = MSG_ReadShort();
		}

		item->durability = MSG_ReadShort();
		item->reqLevel = MSG_ReadByte();
		item->experience = MSG_ReadLong();
		item->timer = MSG_ReadLong();

		
		item->ItemExtendInfo.UseFlag = MSG_ReadLong();
		
		item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType = 
			static_cast<ITEM_SPAWN_INFO::enumItemSpawnType>(MSG_ReadLong());
		
		strncpy(item->ItemExtendInfo.ItemSpawnInfomation.data, 
			MSG_ReadString(), item_t::MAX_SIZE_OF_ITEM_SPAWN_DATA);
		
		item->ItemExtendInfo.ItemSpawnInfomation.data[item_t::MAX_SIZE_OF_ITEM_SPAWN_DATA -1] = NULL;
		item->ItemExtendInfo.AllowUseTime	= MSG_ReadLong();	
		item->ItemExtendInfo.CalclateUseDateToSec	= MSG_ReadLong();	



	}

	
	
	myCharacterInfo->skillNumber = MSG_ReadByte();
	for (j=0; j<myCharacterInfo->skillNumber; j++)
	{
		skillIdx = MSG_ReadByte();
		skill = &myCharacterInfo->skill[skillIdx];

		skill->tableIdx = MSG_ReadShort();
		skill->iLevel = MSG_ReadLong();
	}

	for (j=0; j<MAX_NUMBER_OF_HOTKEY; j++)
	{
		myCharacterInfo->hotkeyType[j] = MSG_ReadByte();
		myCharacterInfo->hotkeyIdx[j] = MSG_ReadChar();
	}

	
	myCharacterInfo->questPoint = MSG_ReadLong();
	myCharacterInfo->questNumber = MSG_ReadByte();

	for (j=0; j<MAX_COUNT_QUEST_VARIABLE; j++)
		myCharacterInfo->questVar[j].iVar = MSG_ReadLong();

	for (j=0; j<MAX_COUNT_QUEST_INVENTORY; j++)
		myCharacterInfo->questInventory[j].iItem = MSG_ReadLong();

	for( j = 0; j < MAX_COUNT_QUEST_STATUS; j ++ )
	{
		memset( &myCharacterInfo->questStatus[j], 0, sizeof( questStatus_t ) );
		myCharacterInfo->questStatus[j].questIdx = -1;
	}
 
	for (j=0; j<myCharacterInfo->questNumber; j++)
	{
		questStatusIdx = MSG_ReadByte();
		questStatus = &myCharacterInfo->questStatus[questStatusIdx];

		questStatus->idx = questStatusIdx;
		questStatus->questIdx = MSG_ReadShort();
		for (k=0; k<MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD; k++)
			questStatus->dField[k] = MSG_ReadLong();
		strcpy(questStatus->partyOrganizerName, MSG_ReadString());
		questStatus->partyOrganizeServer = MSG_ReadByte();
		questStatus->partyOrganizeTime = MSG_ReadLong();
		questStatus->clearTime = MSG_ReadLong();
		questStatus->limitTime = MSG_ReadLong();
	}

	return 1;
}



void GTH_SendMessage_RequestReadyCreate()
{
	
	g_ifMng->m_loginWin->m_cselMakeBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_READYCREATE);
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ReplyReadyCreate()
{
	
	g_ifMng->m_loginWin->m_cselMakeBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

	GTH_UnlockSending();

	int i, num, reply;
	charEditData_t* data;

#ifdef _GTH_ONLINE_VERSION
	reply = MSG_ReadByte();

	if ( reply < 1 )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,116), 1, IF_MESSAGE_NONE );
		return;
	}

	num = MSG_ReadByte();	
	g_ifMng->m_loginWin->m_numOfMakeCharacter = num;	
	
	
	for( i = 0; i < num; i ++ )
	{
		data = &g_ifMng->m_loginWin->m_makeCharData[i];
		strcpy( data->name, "" );
		data->pcClass = MSG_ReadByte();
		data->curForce = data->force = MSG_ReadShort();
		data->curSoulpower = data->soulpower = MSG_ReadShort();
		data->curAgility = data->agility = MSG_ReadShort();
		data->curVitality = data->vitality = MSG_ReadShort();
		data->bonusPoint = MSG_ReadShort();

		data->skinPart[0] = 0;
		data->skinPart[1] = 0;
		data->skinPart[2] = 0;
		data->skinPart[3] = 0;

		data->maxFaceCount = g_charManager->m_pSkinManager->m_defSkinList[i].count[SKIN_PART_HEAD];
		
		
		data->maxHairCount = g_charManager->m_pSkinManager->m_defSkinList[i].count[SKIN_PART_HAIR]-1;
		
	}	 
	
#else
	
	num = 2;
	g_ifMng->m_loginWin->m_numOfMakeCharacter = num;
	
	for( i = 0; i < num; i ++ )
	{
		data = &g_ifMng->m_loginWin->m_makeCharData[i];
		strcpy( data->name, "" );
		data->pcClass = i;
		data->curForce = data->force = 10;
		data->curSoulpower = data->soulpower = 10;
		data->curAgility = data->agility = 10;
		data->curVitality = data->vitality = 10;
		data->bonusPoint = 5;

		data->skinPart[0] = 0;
		data->skinPart[1] = 0;
		data->skinPart[2] = 0;
		data->skinPart[3] = 0;
	}
#endif


	g_ifMng->m_loginWin->RequestMakeCharacter();
}

void GTH_SendMessage_RequestCreateCharacter(int characterID, charEditData_t *data)
{
	if( !GTH_LockSending() ) return;

	int MembershipType = 0;
	
	
	if(g_ifMng->m_loginWin->m_nMembershipType == LOGIN_CHAR_FREE_NEW &&
		g_ifMng->m_loginWin->m_btSelectCharStage == CREATE_NONE_CHAR)
	{			
		MembershipType = characterID + CREATE_ONLY_LEFT_CHAR;
		g_ifMng->m_loginWin->m_nMembershipType = MembershipType;
	}

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CREATECHARACTER);
		MSG_WriteByte(&netMessage, characterID);
		MSG_WriteByte(&netMessage, data->pcClass);
		MSG_WriteString(&netMessage, data->name);
		MSG_WriteByte(&netMessage, data->skinPart[0]);
		MSG_WriteByte(&netMessage, data->skinPart[1]);
		MSG_WriteByte(&netMessage, data->skinPart[2]);
		MSG_WriteByte(&netMessage, data->skinPart[3]);
		MSG_WriteShort(&netMessage, data->curForce);
		MSG_WriteShort(&netMessage, data->curSoulpower);
		MSG_WriteShort(&netMessage, data->curAgility);
		MSG_WriteShort(&netMessage, data->curVitality);
		MSG_WriteShort(&netMessage, data->bonusPoint);

		
		MSG_WriteShort(&netMessage, MembershipType);
		
		NET_SendMessage(&msSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ReplyCreateCharacter()
{
	
	g_ifMng->m_loginWin->m_cmakeOkBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

	GTH_UnlockSending();

	int reply, curGen;
#ifdef _GTH_ONLINE_VERSION
	reply = MSG_ReadByte();
	if (reply < 1)
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,117) , 1, IF_MESSAGE_NONE );
		return;
	}
	GTH_SendMessage_RequestGameData();
	curGen = g_ifMng->m_loginWin->m_curSelectedMakeCharacter;
	if( g_ifMng->m_loginWin->m_makeCharData[curGen].bonusPoint > 0 )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,118), MSG_BUTTONTYPE_OK, IF_MESSAGE_NONE );
	}
#endif
}

void GTH_SendMessage_RequestDeleteCharacter()
{
	
	g_ifMng->m_loginWin->m_cselDeleteBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_DELETECHARACTER);
		MSG_WriteByte(&netMessage, g_cgv.selectedGameDataIdx);
	
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ReplyDeleteCharacter()
{
	
	g_ifMng->m_loginWin->m_cselDeleteBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

	GTH_UnlockSending();

	int reply;

	reply = MSG_ReadByte();

	if (reply < 1)
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,119), 1, IF_MESSAGE_NONE );
		return;
	}
	g_ifMng->ExitMessage();

	GTH_SendMessage_RequestGameData();
}

void GTH_SendMessage_RequestCheckCharacterName(char *characterName)
{
	
	g_ifMng->m_loginWin->m_cmakeOkBtn->SetButtonState( IF_BTN_TYPE_DISABLE );

	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHECKCHARNAME);
		MSG_WriteString(&netMessage, characterName);
		
		NET_SendMessage(&msSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ReplyCheckCharacterName()
{
	GTH_UnlockSending();

	int reply;

	reply = MSG_ReadByte();
	if (reply < 1)
	{
		
		g_ifMng->m_loginWin->m_cmakeOkBtn->SetButtonState( IF_BTN_TYPE_ENABLE );

		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,120), 1, IF_MESSAGE_NONE );
		return;
	}

	charEditData_t *data;
	int curGen;

	curGen = g_ifMng->m_loginWin->m_curSelectedMakeCharacter;
	data = &g_ifMng->m_loginWin->m_makeCharData[curGen];

	
#ifdef _GTH_ONLINE_VERSION
	GTH_SendMessage_RequestCreateCharacter( g_cgv.selectedGameDataIdx, data );
#endif
}



void	CreateDefaultSelectChar()
{

	
	
	
	g_charManager->m_Characters[3].pcJob = 5;
	g_charManager->m_Characters[3].equipNumber = 6;	
	g_charManager->m_Characters[3].equipment[0] = 649;
	g_charManager->m_Characters[3].equipment[1] = 698;
	g_charManager->m_Characters[3].equipment[2] = 228;
	g_charManager->m_Characters[3].equipment[3] = 230;
	g_charManager->m_Characters[3].equipment[4] = 221;
	g_charManager->m_Characters[3].equipment[5] = 78;
	


	

	g_charManager->m_Characters[4].pcJob = 7;
	g_charManager->m_Characters[4].equipNumber = 6;	
	g_charManager->m_Characters[4].equipment[0] = 654;
	g_charManager->m_Characters[4].equipment[1] = 702;
	g_charManager->m_Characters[4].equipment[2] = 579;
	g_charManager->m_Characters[4].equipment[3] = 557;
	g_charManager->m_Characters[4].equipment[4] = 543;
	g_charManager->m_Characters[4].equipment[5] = 383;
	
	
	

	

	g_charManager->m_Characters[5].pcJob = 9;
	g_charManager->m_Characters[5].equipNumber = 6;	
	g_charManager->m_Characters[5].equipment[0] = 586;
	g_charManager->m_Characters[5].equipment[1] = 653;
	g_charManager->m_Characters[5].equipment[2] = 579;
	g_charManager->m_Characters[5].equipment[3] = 557;
	g_charManager->m_Characters[5].equipment[4] = 762;	
	g_charManager->m_Characters[5].equipment[5] = 85;	
	


	
	
	g_charManager->m_Characters[6].pcJob = 12;
	g_charManager->m_Characters[6].equipNumber = 6;	
	g_charManager->m_Characters[6].equipment[0] = 803;
	g_charManager->m_Characters[6].equipment[1] = 115;
	g_charManager->m_Characters[6].equipment[2] = 24;
	g_charManager->m_Characters[6].equipment[3] = 21;
	g_charManager->m_Characters[6].equipment[4] = 773;	
	g_charManager->m_Characters[6].equipment[5] = 60;
	
	g_charManager->GenerateOtherCharacter(3);
	g_charManager->GenerateOtherCharacter(4);
	
	g_charManager->GenerateOtherCharacter(5);
	g_charManager->GenerateOtherCharacter(6);

}








void GTH_SendMessage_Request_LoadCharacterSlot()
{
	
	if(!gGameConfigFlag.m_bCharacter_Storage) return;

	
	if(g_ifMng->m_loginWin->m_bSendLoadCharacterSlot) return;
	g_ifMng->m_loginWin->m_bSendLoadCharacterSlot=false;

	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_LOADCHARACTER_SLOT);
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_Request_SaveCharacterSlot(int nSlotType)
{
	
	if(!gGameConfigFlag.m_bCharacter_Storage) return;
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_SAVECHARACTER_SLOT);
		MSG_WriteByte(&netMessage, nSlotType);
		NET_SendMessage(&msSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ReplyLoadCharacterSlot()
{	
	GTH_UnlockSending();

	int type = MSG_ReadByte();
	int day = MSG_ReadShort();

	
	if(type < LOGIN_CHAR_FREE_NEW || type > LOGIN_CHAR_PU) return;

	g_ifMng->m_loginWin->m_nMembershipType = type;
	g_ifMng->m_loginWin->m_nDate = day;
	g_ifMng->m_loginWin->m_bLoadCharacterSlot=true;

	g_ifMng->m_loginWin->SettingCharacterSlot();

	
	if(type == LOGIN_CHAR_PU_NEW)
	{
		GTH_SendMessage_Request_SaveCharacterSlot(LOGIN_CHAR_PU);
	}
}





void ChangeCharacterSlot(void)
{
	
	syncCharacter_t* selectCharacter;
	vec3_t delta;
	int i;
	
	g_charManager->m_iNumCharacters = 0;
	g_charManager->m_iNumBackupCharacters = 0;
	
	GTH_GenerateMyCharacter();			
	
	VectorClear( delta );
	for( i = 0; i < g_cgv.syncCharacterNumber; i ++ )
	{
		selectCharacter = &g_cgv.syncCharacter[i];
		g_move->Pmove( selectCharacter->position , delta , selectCharacter->angles );
	}			
	g_charManager->GenerateCharacter( g_cgv.syncCharacter, g_cgv.syncCharacterNumber );
	
	
	if( g_ifMng->m_loginWin->m_deleteFlag )
	{
		
		
		
		g_ifMng->m_loginWin->m_deleteFlag = false;
		
		
		CreateDefaultSelectChar();
		
		
	}
	else if( g_ifMng->m_loginWin->m_makeFlag )
	{
		g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_SELECT_CHARACTER );
		g_ifMng->m_loginWin->m_makeFlag = false;
		g_ifMng->m_loginWin->m_cameraStep = 3 + g_ifMng->m_loginWin->m_curSelectedMakeCharacter;
		g_ifMng->m_loginWin->m_cameraStepFlag = false;
		
		
		CreateDefaultSelectChar();
		
		
	}
	
	CreateDefaultSelectChar();
	
}