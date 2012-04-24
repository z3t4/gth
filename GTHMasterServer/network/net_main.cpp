#include "../global.h"

i3socket_t msSocket;	
i3socket_t localSocket;	

extern unsigned int g_netTime;
extern i3sizebuf_t netMessage; 
extern i3packet_t packetBuffer;	



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


int GTH_Network_Startup()
{
	if ( !NET_OpenNetwork() )  
	{
		
		MessageBox(g_hWnd, "네트워크 초기화를 할 수 없습니다.", "에러", MB_OK); 
		return 0;
	}

	
	if ( !NET_OpenSocketForServer( &msSocket, g_config.serviceIP, g_config.servicePort) )
	{
		MessageBox(g_hWnd, "마스터서버 소켓을 오픈할 수 없습니다.", "에러", MB_OK); 
		return 0;
	}

	
	if ( !NET_OpenSocketForServer( &localSocket, g_config.localIP, g_config.localPort, SocketType_Unreliable) )
	{
		MessageBox(g_hWnd, "마스터서버 로컬 소켓을 오픈할 수 없습니다.", "에러", MB_OK); 
		return 0;
	}
	
	
#ifdef _USE_SENDING_THREAD
	if ( !NET_CreateSendingThread() )
	{
		char msg[256];
		sprintf(msg,"송신 쓰레드 생성 실패(ErrorCode:%d)", GetLastError());
		MessageBox(g_hWnd, msg, "에러", MB_OK); 
		return 0;
	}	
#endif

#ifdef _USE_RECEIVING_THREAD
	if ( !NET_CreateReceivingThread() )
	{
		char msg[256];
		sprintf(msg,"수신 쓰레드 생성 실패(ErrorCode:%d)", GetLastError());
		MessageBox(g_hWnd, msg, "에러", MB_OK); 
		return 0;
	}	
#endif

	
	strcpy( g_hostName, NET_GetHostName() );

	Sleep(100);	

	return 1;
}

void GTH_Network_Shutdown()
{
	
#ifdef _USE_SENDING_THREAD
	NET_DestroySendingThread();
#endif

#ifdef _USE_RECEIVING_THREAD
	NET_DestroyReceivingThread();
#endif

	NET_CloseSocket(&localSocket);
	NET_CloseSocket(&msSocket);
	NET_CloseNetwork();
}

void GTH_InitMasterServer()
{
	int i;

    for (i=0; i<MAX_CLIENTS; i++)
	{
		memset(&g_clients[i], 0, sizeof(i3client_t));
		g_clients[i].active = false;
	}

	
	totalPacketsSend		=	0;
	totalPacketsReceived	=	0;
	packetsSent				=	0;			
	packetsReceived			=	0;			
	packetsReSent			=	0;			
	packetsDropCount		=	0;			
	packetsDupCount			=	0;			
	packetsAckSent			=	0;			
	packetsAckReceived		=	0;			
	un_packetsSent			=	0;			
	un_packetsReceived		=	0;			
	un_packetsDropCount		=	0;			

}



int	GTH_Network_GetMessage(i3socket_t *sock)
{
	struct sockaddr_in	readaddr;
	int					clientIdx;
	int					length;
	int					ret = 0;
	int					socket;	
	int					serverGroupIdx;



	length = NET_ReadPacket(&socket, (byte *)&packetBuffer, &readaddr);

	
	if (length == 0 || length < NET_HEADERSIZE || length > NET_DATAGRAMSIZE ) 
	{
		return PacketAnalysis_None;
	}
	
	
	if ( socket == msSocket.socket )
	{
		clientIdx = GTH_FindClientWithAddr(&readaddr); 
		if (clientIdx == -1) 
		{
			if ( !GTH_IsValidPacketForNewClient(&packetBuffer) ) 
				return 1; 
			
			clientIdx = GTH_CreateClient(&readaddr);
			if (clientIdx < 0 || clientIdx >= MAX_CLIENTS ) 
				return 1; 
		}
		
		g_curClient = &g_clients[clientIdx];
		
		ret = NET_ControlPacket(&g_curClient->sock, &packetBuffer);	 
	}
	else if ( socket == localSocket.socket )
	{
		serverGroupIdx = GTH_FindServerGroupWithAddr( &readaddr );
		if ( serverGroupIdx == -1 )	
		{
			serverGroupIdx = GTH_GetServerGroupIdx( &packetBuffer );
			
			if ( serverGroupIdx == -1 ) return 1; 
			GTH_AddToServerGroup(serverGroupIdx, &readaddr);
		}
		
		ret = NET_ControlPacket(&g_serverGroup[serverGroupIdx].sock, &packetBuffer);	 
	}

	return ret;
}






void GTH_CheckProblemSocket()
{
	int idx;

	for(idx=0; idx < MAX_CLIENTS; idx++)
	{
		if ( !g_clients[idx].active ) continue;

		
		NET_ControlSendBank(&(g_clients[idx].sock));

		
		if( !NET_CheckSockProblem( &g_clients[idx].sock, 5 * 60 * 1000) )
		{
			g_logSystem->Write("%s님이 비정상적으로 종료합니다.", g_clients[idx].id);
			GTH_RemoveClient( &g_clients[idx]);
			continue;
		}


		
		if(g_netTime - g_clients[idx].sock.lastReceiveTime >= DISCONNECT_WAITTIME )  
		{
			GTH_SendMessage_ReliablePing( &g_clients[idx].sock );
			continue;
		}
	}
} 





int GTH_MasterServer()
{
	int command;
	int state;

	g_netTime = NET_GetNetTime();

	GTH_CheckProblemSocket();

	while (1)
	{
		state = GTH_Network_GetMessage(&msSocket); 

		if (state < PacketAnalysis_Skip )  
			return 0;    

		if (state != PacketAnalysis_Start ) 
			continue;   
			
		MSG_BeginReading();
		command = MSG_ReadByte();

		

		switch(command){
			case GSC_SERVERGROUPINFO:
			case CC_CONNECT_MASTERSERVER:
			case CC_REQUEST_LOGIN:
			case CC_REQUEST_SERVERGROUP:
			case CC_REQUEST_GAMEDATA:
			case CC_REQUEST_READYCREATE:
				break;
			default:
				Sleep(0);
				break;
		}


		switch (command)
		{
		case GSC_SERVERGROUPINFO :  
			GTH_ServerGroupInfo();
			break;

		case GSC_DROPSERVERGROUP :  
			GTH_DropServerGroup();
			break;

		case CC_CONNECT_MASTERSERVER:    
			GTH_ConnectMasterServer();
			break;

		case CC_DISCONNECT_MASTERSERVER: 
			GTH_DisconnectMasterServer();
			break;

		case CC_REQUEST_SERVERGROUP:     
			GTH_SendMessage_GameServerGroup();
			break;

		case CC_REQUEST_LOGIN:     
			GTH_RequestLogin();
			break;

		case CC_REQUEST_GAMEDATA:     
			GTH_RequestGameData();
			break;

		case CC_REQUEST_GAMESERVERADDR:
			GTH_RequestGameServerAddr();
			break;

		case CC_REQUEST_READYCREATE:
			GTH_RequestReadyCreate();
			break;

		case CC_REQUEST_CREATECHARACTER:
			GTH_RequestCreateCharacter();
			break;

		case CC_REQUEST_DELETECHARACTER:
			GTH_RequestDeleteCharacter();
			break;

		case CC_REQUEST_CHECKCHARNAME:
			GTH_RequestCheckCharacterName();
			break;

		
		case CC_REQUEST_LOADCHARACTER_SLOT:
			GTH_LoadCharacterSlot();
			break;

		
		case CC_REQUEST_SAVECHARACTER_SLOT:
			GTH_SaveCharacterSlot();
			break;

		
		case GC_RELIABLE_PING:
		case GC_UNRELIABLE_PING:
			break;

		case CC_MONITOR:
			GTH_ProcessMessage_Monitor();
			break;

		default :
			g_logSystem->Write("명령(프로토콜)이 일치하지 않습니다.(Command:%x)", command);
			if ( g_curClient != NULL ) 
				g_logSystem->Write("프로토콜 불일치 추정 사용자: %s, IP: %s", g_curClient->id, NET_AddrToString((struct sockaddr *)&g_curClient->sock.addr));
			break;
		}
	}
	return 1;
}



int GTH_ServerGroupInfo() 
{
	int idx;
	int i, j;
	int activeServerNumber;
	
	activeServerNumber = 0;

	idx = MSG_ReadByte();	
	
	
	if (idx >= g_config.serviceServerGroupNumber || idx < 0) return 0;

	
	
	
	

	g_serverGroup[idx].updateTime = g_globalTime;
	strcpy( g_serverGroup[idx].name, MSG_ReadString() );
	strcpy( g_serverGroup[idx].DSN, MSG_ReadString() );

	g_serverGroup[idx].totalClient = 0;

	for (i=0; i < MAX_MEMBER_SERVER; i++)
	{
		g_serverGroup[idx].memberServer[i].idx = i;
		g_serverGroup[idx].memberServer[i].active = MSG_ReadByte();
		if ( !g_serverGroup[idx].memberServer[i].active ) continue;

		activeServerNumber ++ ;

		strcpy( g_serverGroup[idx].memberServer[i].name, MSG_ReadString() );
		strcpy( g_serverGroup[idx].memberServer[i].hostName, MSG_ReadString() );
		strcpy( g_serverGroup[idx].memberServer[i].serviceIP, MSG_ReadString() );
		g_serverGroup[idx].memberServer[i].servicePort = MSG_ReadShort();
		strcpy( g_serverGroup[idx].memberServer[i].localIP, MSG_ReadString() );
		g_serverGroup[idx].memberServer[i].localPort = MSG_ReadShort();
		g_serverGroup[idx].memberServer[i].worldNumber = MSG_ReadByte();
		for (j=0; j < g_serverGroup[idx].memberServer[i].worldNumber; j++ )
			g_serverGroup[idx].memberServer[i].worldList[j] = MSG_ReadByte();
		g_serverGroup[idx].memberServer[i].clientNumber = MSG_ReadShort();
		g_serverGroup[idx].memberServer[i].maxClientNumber = MSG_ReadShort();

		g_serverGroup[idx].totalClient += g_serverGroup[idx].memberServer[i].clientNumber;
	}
	
	if ( g_serverGroup[idx].totalClient > g_serverGroup[idx].maxTotalClient )
		g_serverGroup[idx].maxTotalClient = g_serverGroup[idx].totalClient;

	if ( activeServerNumber > g_serverGroup[idx].memberServerNumber )	
		g_serverGroup[idx].memberServerNumber = activeServerNumber;

	
	GTH_CalculateConcurrentClients();
	return 1;
}



int GTH_DropServerGroup() 
{
	int idx;
	
	idx = MSG_ReadByte();	

	g_serverGroup[idx].active = false;
	g_serverGroup[idx].memberServerNumber = 0;

	return 1;
}


void GTH_ConnectMasterServer(void)
{
	char version[VERSIONSTRING+1];	
	strncpy(version, MSG_ReadString(),VERSIONSTRING); version[VERSIONSTRING]=NULL;


	
	if(0 == stricmp(version, g_config.monitorVersion))
	{
		
		if ( g_clientNumber >= g_config.maxServiceClientNumber )
		{
			GTH_SendMessage_RejectClient( 1 );
			
			GTH_RemoveClient( g_curClient );
		}

		g_curClient->isMonitor = true;
		GTH_SendMessage_AcceptClient();
		g_logSystem->Write("서버 모니터링 클라이언트가 접속을 시도합니다.(IP:%s)", 
			               NET_AddrToString((struct sockaddr *)&g_curClient->sock.addr));		
		return;
	}


	if(NULL != strstr(version,"MONITOR"))
	{
		g_logSystem->Write("이전 버전 모니터링 툴(Ver: %s)로 접속을 시도 했습니다.(%s)", version, NET_AddrToString((struct sockaddr *)&g_curClient->sock.addr));
		GTH_SendMessage_RejectClient( 0 );
		
		GTH_RemoveClient( g_curClient );
		return;
	}


	char subVersion[VERSIONSTRING+1];
	strncpy(subVersion, MSG_ReadString(),VERSIONSTRING); subVersion[VERSIONSTRING]=NULL;
	
	
	
	switch(CheckConnectionIP( inet_ntoa( g_curClient->sock.addr.sin_addr))){
		case ConnectionReject:
			GTH_SendMessage_RejectClient( 2 );
			GTH_RemoveClient( g_curClient );
			return;

		case ConnectionFilterTest:
			
			if(g_filterYn){
				if(0 != stricmp(subVersion, "DEVELOPER")){
					GTH_SendMessage_RejectClient( 2 );
					
					GTH_RemoveClient( g_curClient );
					return;
				}
			}			
			break;
		case ConnectionAccept:
			break;		
	}

	
	if(0 != stricmp(g_config.gameVersion, version)){
		GTH_SendMessage_RejectClient( 0 );
		
		GTH_RemoveClient( g_curClient );
		return;
	}

	
	if(g_clientNumber >= g_config.maxServiceClientNumber )
	{
		GTH_SendMessage_RejectClient( 1 );
		
		GTH_RemoveClient( g_curClient );
	}
	
	
	
	GTH_SendMessage_RejectClient( 3 );
	return;
}








void GTH_SendMessage_ReliablePing( i3socket_t *sock )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GC_RELIABLE_PING);
		NET_SendMessage(sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_UnreliablePing( i3socket_t *sock )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GC_UNRELIABLE_PING);
		NET_SendUnreliableMessage(sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_AcceptClient()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_ACCEPTCLIENT);
		NET_SendMessage(&g_curClient->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_RejectClient(int reason)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_REJECTCLIENT);
		switch ( reason )
		{
		case 0 :	
			MSG_WriteByte(&netMessage, reason);
			break;
		case 1:		
			MSG_WriteByte(&netMessage, reason);
			break;
		case 2:		
			MSG_WriteByte(&netMessage, reason);
			MSG_WriteString(&netMessage, g_filterMessage[g_filterIdx]);		
			break;
		case 3 :
			MSG_WriteByte(&netMessage, 0);
			MSG_WriteByte(&netMessage, 1);
			break;
		}
		
		NET_SendMessage(&g_curClient->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

int GTH_DisconnectMasterServer() 
{
	int success;
	
	success = GTH_RemoveClient( g_curClient );

	return 1;
}

int GTH_AddToServerGroup(int serverGroupIdx, struct sockaddr_in *addr)
{
	serverGroup_t *serverGroup;


	if (serverGroupIdx >= g_config.serviceServerGroupNumber || serverGroupIdx < 0) return -1;

	serverGroup = &g_serverGroup[serverGroupIdx];

	if ( serverGroup->active ) return -1;	

	serverGroup->idx = serverGroupIdx;
	serverGroup->active = true;

	NET_InitSocket(&serverGroup->sock, addr, localSocket.socket, SocketType_Unreliable);

	return serverGroupIdx;
}

int GTH_GetServerGroupIdx(i3packet_t *p)
{
	int command;
	int	idx;

	if ( !NET_PreviewMessage(p) ) return -1;

	MSG_BeginReading();
	command = MSG_ReadByte();

	if (command != GSC_SERVERGROUPINFO) return -1;

	idx = MSG_ReadByte();

	return idx;
}

int GTH_FindServerGroupWithAddr(struct sockaddr_in *addr)
{
	serverGroup_t *serverGroup;
	int i;

	serverGroup = &g_serverGroup[0];

	for (i = 0; i < g_config.serviceServerGroupNumber; i++, serverGroup++)
	{
		if (!serverGroup->active)
			continue;

		if (NET_AddrCompare((struct sockaddr *)addr, (struct sockaddr *)&serverGroup->sock.addr) == 0)
			return i;
	}
	
	return -1; 
}

int GTH_IsValidPacketForNewClient(i3packet_t *p)
{
	int command;

	NET_PreviewMessage( p );

	MSG_BeginReading();
	command = MSG_ReadByte();

	if (command != CC_CONNECT_MASTERSERVER) return false;

	return true;
}

int GTH_FindClientWithAddr(struct sockaddr_in *addr)
{
	i3client_t *client;
	int i;

	client = &g_clients[0];
	for (i = 0; i < MAX_CLIENTS ; i++, client++) 
	{
		if (!client->active)
			continue;

		if (NET_AddrCompare((struct sockaddr *)addr, (struct sockaddr *)&client->sock.addr) == 0)
			return i;
	}

	return -1;
}

int GTH_CreateClient(struct sockaddr_in *addr)
{
	int i, j, k;
	i3client_t *client;

	client = &g_clients[0];
	
	
	for (i=0; i<MAX_CLIENTS; i++, client++)
	{
		if ( !client->active ) break;
	}	
	

	if (i >= MAX_CLIENTS) 
	{
		g_logSystem->Write("치명적오류: 클라이언트 한계 초과 생성");
		g_logSystem->Update();
		return -1;
	}

	
	
	NET_InitPacketMap( &client->sock );	

	
	memset(client, 0, sizeof(i3client_t));	

	client->idx = i;
	client->active = true;
	client->isMonitor = false;
	client->isComplete = false;
	client->isSuccessLogin = false;

	client->dbCheckAccount = -1;

	NET_InitSocket(&client->sock, addr, msSocket.socket, SocketType_Reliable);
	g_clientNumber++;

	if ( g_clientNumber > g_maxClientNumber ) 
	{
		g_maxClientNumber = g_clientNumber;
		
		if ( g_config.serverNo == 0 )	
		{
			g_logSystem->Write("전체 서버군 최대 동접자 갱신: %d명", g_maxClientNumber);
		}
	}

	
	client->dataNumber = 0;
	for (j=0; j<MAX_CHARACTER_DATA; j++)
	{
		memset(&client->data[j], 0, sizeof(characterData_t) );
		for (k=0; k<MAX_NUMBER_OF_OWNITEM; k++)
			client->data[j].item[k].itemTableIdx = -1;

		
		for (k=0; k<MAX_NUMBER_OF_SKILL; k++)
			client->data[j].skill[k].tableIdx = -1;

		for (k=0; k<MAX_NUMBER_OF_SKILL_VARIABLE; k++)
			client->data[j].skillVariable[k] = -1;

		
		for (k=0; k<MAX_NUMBER_OF_HOTKEY; k++)
			client->data[j].hotkeyIdx[k] = -1;

		
		for (k=0; k<MAX_COUNT_QUEST_VARIABLE; k++)
		{
			client->data[j].questVar[k].iVar = 65280;	
		}

		
		for (k=0; k<MAX_COUNT_QUEST_INVENTORY; k++)
		{
			client->data[j].questInventory[k].iItem = 65535;	
		}

		
		for (k=0; k<MAX_COUNT_QUEST_STATUS; k++)
		{
			client->data[j].questStatus[k].idx = k;
			client->data[j].questStatus[k].questIdx = -1;
		}

		
		for (k=0; k<MAX_PRECOCITY_SIZE; k++)
			client->data[j].precocityInventory[k] = -1;

		
	}

	return client->idx;
}

int GTH_RemoveClient(i3client_t *client)
{
	if ( !client->active )	return false;

	if ( client->isMonitor ) 
	{
		g_logSystem->Write("서버 모니터링 클라이언트가 종료했습니다.(ID:%s, IP:%s)", client->id, NET_AddrToString((struct sockaddr *)&client->sock.addr));
	}
	else	
	{
		if ( client->isSuccessLogin )
		{
	

			if ( client->isComplete )
			{
				
				g_logSystem->Write("%s님이 종료(게임서버로 접속)했습니다.", client->id);
			}
			else
			{
				g_DBAccountServer->DeleteLogin( client );
				g_logSystem->Write("%s님이 접속 종료 했습니다.", client->id);
			}
		}
	}

	client->active = false;

	g_clientNumber--;

	return true;
}











void GTH_RequestLogin_HiCocoon(void);


void GTH_RequestLogin(void)
{
	if(TRUE == g_config.Hicocoon.bActive){
		GTH_RequestLogin_HiCocoon();
		return;
	}








	char id[IDSTRING+1]="";
	char password[PASSWORDSTRING+1]="";
	
	strncpy(id, MSG_ReadString(),IDSTRING); id[IDSTRING]=NULL;
	strncpy(password, MSG_ReadString(),PASSWORDSTRING); password[PASSWORDSTRING]=NULL;
	strncpy(g_curClient->id, id,IDSTRING);  g_curClient->id[IDSTRING]=NULL;
	g_DBAccountServer->CheckAccount( g_curClient, id, password); 
}





#include "../HiCocoon/HiCocoonCtrl.h"
extern CHiCocoonCtrl*	g_pHiCocoonCtrl;


BOOL ProcessHiCocoonLogin(CDaumGameAuth* DaumGameAuth, char* IN_Hicocoonauth, char* OUT_ID)
{	

	if ( DaumGameAuth== NULL )
		return FALSE;

	if ( IN_Hicocoonauth == NULL)
		return FALSE;

	if ( OUT_ID == NULL)
		return FALSE;

	
	if(FALSE == DaumGameAuth->Init (
		_T("z6DuDLA1NcyXge3psD63Uu0o3GqpAzyBjm7pD0hNDYKefE37fNM1tVng34ejixgA") ) )
		return FALSE;	

	char GAMEID[MAX_PATH];			
	char UniqueID[MAX_PATH];		
	char Ssnhead[MAX_PATH];			
	char Sex[MAX_PATH];				
	char NAMECHECK[MAX_PATH];		
	char STATUS[MAX_PATH];			
	
	
	if( FALSE == DaumGameAuth->SetSource(IN_Hicocoonauth) )				
	{
		g_curClient->dbCheckAccount = 1;
		GTH_ReplyLogin_Hicocoon(g_curClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user);
		return FALSE;
	}

	
	if ( FALSE == DaumGameAuth->GetData( "GID",  GAMEID, IDSTRING ) )
	{
		g_curClient->dbCheckAccount = 1;
		GTH_ReplyLogin_Hicocoon(g_curClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user);
		return FALSE;
	}
	
	
	
	if ( FALSE == DaumGameAuth->GetData( "UID",  UniqueID, MAX_PATH ) )
	{
		g_curClient->dbCheckAccount = 1;
		GTH_ReplyLogin_Hicocoon(g_curClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user);
		return FALSE;
	}
	
	
	
	
	if ( FALSE == DaumGameAuth->GetData( "SEX",  Sex, MAX_PATH ) )
	{
		g_curClient->dbCheckAccount = 1;
		GTH_ReplyLogin_Hicocoon(g_curClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user);
		return FALSE;
	}
	
	
	
	


	
	
	

	
	
	if ( FALSE == DaumGameAuth->GetData( "STATUS",  STATUS, MAX_PATH ) )
	{
		g_curClient->dbCheckAccount = 1;
		GTH_ReplyLogin_Hicocoon(g_curClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user);
		return FALSE;
	}

	
	

	
	if ( 0 == stricmp( "B", STATUS) )
	{
		GTH_ReplyLogin_Hicocoon(g_curClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::block_user);
		return FALSE;
	}	
	
	
	
	
	if( TRUE == DaumGameAuth->IsTimeExpired() )
	{
		GTH_ReplyLogin_Hicocoon(g_curClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::over_auth_time);
		return FALSE;	
		
	}
 
	strncpy(OUT_ID, GAMEID, IDSTRING);
	return TRUE;



}


void GTH_RequestLogin_HiCocoon(void)
{
	char szpId[IDSTRING+1]="";		
	char szpPassword[CHiCocoonCtrl::tagCoCoonAUth::maxbytes_CoCoonAuth+1]="";
	BOOL bCoCoonID=FALSE;


	if ( g_curClient->isMonitor == FALSE)
		bCoCoonID = MSG_ReadByte();

	strncpy(szpId,MSG_ReadString(),IDSTRING);  
	szpId[IDSTRING]=NULL;

	strncpy(szpPassword, MSG_ReadString(),CHiCocoonCtrl::tagCoCoonAUth::maxbytes_CoCoonAuth);
	szpPassword[CHiCocoonCtrl::tagCoCoonAUth::maxbytes_CoCoonAuth]=NULL;

	
	
	if(TRUE == bCoCoonID){
		if(0 == strcmp((char*)szpId,"1")){
			CDaumGameAuth auth;			
			if ( FALSE == ProcessHiCocoonLogin(&auth, szpPassword, szpId) ){
				g_logSystem->Write("loginFaild HiCocoon id=[%s] password=[%s] CoCoonAuth=[%s]", szpId, szpPassword, szpPassword);			
				return;
			}


			strncpy(g_curClient->id, szpId, IDSTRING); 
			g_curClient->id[IDSTRING]=NULL;					
			g_logSystem->Write("login HiCocoon id=[%s] password=[%s] CoCoonAuth=[%s]", szpId, szpPassword, szpPassword);			
			g_DBAccountServer->CheckAccountHicocoon(g_curClient, g_curClient->id);
			return;
		}
		

		return;
	}
	
	else{
		
		if ( CheckConnectionIP( inet_ntoa( g_curClient->sock.addr.sin_addr)) == ConnectionReject )
		{
			
			
			g_curClient->dbCheckAccount = 1;
			GTH_ReplyLogin_Hicocoon(g_curClient,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::unregister_user);
			return;
		}

		
		strncpy(g_curClient->id, szpId,IDSTRING);  g_curClient->id[IDSTRING]=NULL;
		g_DBAccountServer->CheckAccount( g_curClient, g_curClient->id, szpPassword); 
		g_logSystem->Write("login HiCocoon Developer id=[%s] password=[%s]", g_curClient->id, szpPassword);
		
	}
	return;
}








#include "..\tagMCPacket.h"
void GTH_ReplyLoginV2(
		i3client_t* pClient,
		const BOOL bSUCCESSorFAIL,
		enum tagMCPaket_MSC_REPLYLOGIN::enumError enumErrorCode)
{
	
    MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_REPLYLOGIN);		
		MSG_WriteByte(&netMessage, pClient->Identify.Certification);
		MSG_WriteByte(&netMessage, bSUCCESSorFAIL);	


		
		if(TRUE == bSUCCESSorFAIL)
		{
			MSG_WriteString(&netMessage,pClient->id);

			
			MSG_WriteByte(&netMessage, (BYTE)gGameConfigFlag.m_bCharacter_Storage);	
			
			
			MSG_WriteChar(&netMessage,pClient->billMethod[0]);
			MSG_WriteChar(&netMessage,pClient->billMethod[1]);
			MSG_WriteLong(&netMessage,ntohl(pClient->billRemain));
			MSG_WriteChar(&netMessage,pClient->billExpire[0]);
			MSG_WriteChar(&netMessage,pClient->billExpire[1]);
			MSG_WriteChar(&netMessage,pClient->billExpire[2]);
			MSG_WriteChar(&netMessage,pClient->billExpire[3]);
			MSG_WriteChar(&netMessage,pClient->billExpire[4]);
			MSG_WriteChar(&netMessage,pClient->billExpire[5]);
			MSG_WriteChar(&netMessage,pClient->billExpire[6]);
			MSG_WriteChar(&netMessage,pClient->billExpire[7]);
			MSG_WriteChar(&netMessage,pClient->billExpire[8]);
			MSG_WriteChar(&netMessage,pClient->billExpire[9]);
			MSG_WriteChar(&netMessage,pClient->billExpire[10]);
			MSG_WriteChar(&netMessage,pClient->billExpire[11]);

		}else{		
			MSG_WriteByte(&netMessage, enumErrorCode);
			
			
			if ( enumErrorCode == tagMCPaket_MSC_REPLYLOGIN::enumError::mismatch_password )
			{
				g_logSystem->Write("암호불일치: %s, (%s)", 
					pClient->id, 
					NET_AddrToString((struct sockaddr *)&pClient->sock.addr));
			}
		}
		NET_SendMessage(&pClient->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}









void GTH_ReplyLogin_Hicocoon(i3client_t* pClient,
		const BOOL bSUCCESSorFAIL,
		enum tagMCPaket_MSC_REPLYLOGIN::enumError enumErrorCode)
{
	
    MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_REPLYLOGIN);		
		MSG_WriteByte(&netMessage, pClient->Identify.Certification);
		MSG_WriteByte(&netMessage, bSUCCESSorFAIL);	

		
		if(TRUE == bSUCCESSorFAIL)
		{
			MSG_WriteString(&netMessage,pClient->id);
			
			MSG_WriteByte(&netMessage, (BYTE)gGameConfigFlag.m_bCharacter_Storage);	
			

		}else{		
			MSG_WriteByte(&netMessage, enumErrorCode);
			
			
			if ( enumErrorCode == tagMCPaket_MSC_REPLYLOGIN::enumError::mismatch_password )
			{
				g_logSystem->Write("암호불일치: %s, (%s)", 
					pClient->id, 
					NET_AddrToString((struct sockaddr *)&pClient->sock.addr));
			}
		}
		NET_SendMessage(&pClient->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );

}




int GTH_SendMessage_GameServerGroup()
{
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_SERVERGROUPINFO);
		MSG_WriteByte(&netMessage, g_config.serviceServerGroupNumber);
		
		for (i=0; i<g_config.serviceServerGroupNumber; i++)
		{
			MSG_WriteByte(&netMessage, g_serverGroup[i].active);
			MSG_WriteByte(&netMessage, g_serverGroup[i].idx);  
			MSG_WriteString(&netMessage, g_serverGroup[i].name); 
			MSG_WriteShort(&netMessage, g_serverGroup[i].totalClient);
			
			
			if ( g_serverGroup[i].totalClient < g_config.maxServerGroupServiceClientNumber )
			{
				MSG_WriteByte(&netMessage, 1);		
			}
			else
			{
				MSG_WriteByte(&netMessage, 0);		
			}
		}
		
		NET_SendMessage(&g_curClient->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}





void GTH_Debug_PacketInfo()
{
	char debug[1024];

	sprintf( debug, "total send:%d, total recv:%d, send:%d, recv:%d, resend:%d, drop:%d, dup:%d, acksend:%d, ackrecv:%d, un_send:%d, un_recv:%d, un_drop:%d", 
		totalPacketsSend, totalPacketsReceived,
		packetsSent, packetsReceived, packetsReSent, packetsDropCount, packetsDupCount,
		packetsAckSent, packetsAckReceived, un_packetsSent, un_packetsReceived, un_packetsDropCount );

	MessageBox(g_hWnd, debug, "DebugInfo", MB_OK);
}




void GTH_RequestGameData()
{
	g_curClient->serverGroupID = MSG_ReadByte();

	

	if (g_curClient->serverGroupID < 0 || g_curClient->serverGroupID >= g_config.serviceServerGroupNumber)
		return;

	
	GTH_InitAllCharacterData( g_curClient );

	
	

	g_DBGameServer[g_curClient->serverGroupID]->LoadPremiumInfo(g_curClient);
	g_DBGameServer[g_curClient->serverGroupID]->LoadAllGame(g_curClient);
	
	g_DBGameServer[g_curClient->serverGroupID]->LoadAllQuest( g_curClient );
	g_DBGameServer[g_curClient->serverGroupID]->LoadAllQuestStatus( g_curClient );
	
	g_DBGameServer[g_curClient->serverGroupID]->LoadAllSkill( g_curClient );
	
	g_DBGameServer[g_curClient->serverGroupID]->LoadAllItem( g_curClient );

	return;
}




#include "..\datastruct.h"
#include "..\CTools.h"





void GTH_SendGameData( i3client_t *client)
{
	


	if(0 ==  client->dataNumber){
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, MSC_GAMEDATA);
			MSG_WriteByte(&netMessage, client->dataNumber);
			NET_SendMessage(&client->sock, &netMessage);
		}
		MSG_EndWriting( &netMessage );
		return;
	}





	int i=0, j=0, k=0;
	item_t*				item=NULL;
	skill_t*			skill=NULL;
	characterData_t*	data=NULL;
	questVariable_t*	questVar = NULL;
	questInventory_t*	questInventory = NULL;
	questStatus_t*		questStatus =NULL;



	for (i=0; i<client->dataNumber; i++)
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, MSC_GAMEDATA);
			MSG_WriteByte(&netMessage, client->dataNumber);
			
			data = &client->data[i];
			
			MSG_WriteByte(&netMessage, data->characterID);
			MSG_WriteByte(&netMessage, data->pcClass);
			MSG_WriteByte(&netMessage, data->pcJob);
			
			MSG_WriteByte(&netMessage, data->skinPart[0]);
			MSG_WriteByte(&netMessage, data->skinPart[1]);
			MSG_WriteByte(&netMessage, data->skinPart[2]);
			MSG_WriteByte(&netMessage, data->skinPart[3]);
			
			MSG_WriteString(&netMessage, data->name);
			
			MSG_WriteLong(&netMessage, data->age);
			
			MSG_WriteString(&netMessage, data->rank);
			
			
			MSG_WriteLong(&netMessage, data->rankPoint);
			
			MSG_WriteByte(&netMessage, data->worldIdx);
			
			
			
			MSG_WriteByte(&netMessage, data->level);
			MSG_Write64Int(&netMessage, data->exp);
			MSG_WriteByte(&netMessage, data->increaseExpRate);
			
			MSG_WriteByte(&netMessage, data->selectedGen);
			MSG_WriteByte(&netMessage, data->genLevel);
			MSG_Write64Int(&netMessage, data->genExp);
			MSG_WriteByte(&netMessage, data->increaseGenExpRate);
			for (j=0; j<GEN_NUMBER; j++)
				MSG_WriteLong(&netMessage, data->genCapability[j]);
			
			MSG_WriteShort(&netMessage, data->force);
			MSG_WriteShort(&netMessage, data->soulpower);
			MSG_WriteShort(&netMessage, data->agility);
			MSG_WriteShort(&netMessage, data->vitality);
			
			MSG_WriteShort(&netMessage, data->bonusStatusPoint);
			
			MSG_WriteLong(&netMessage, data->curRA);
			MSG_WriteShort(&netMessage, data->curSA);
			MSG_WriteLong(&netMessage, data->curChargeNSE);
			MSG_WriteLong(&netMessage, data->curChargeSE);
			MSG_WriteLong(&netMessage, data->curChargeRA);
			MSG_WriteLong(&netMessage, data->curChargeSA);
			MSG_WriteLong(&netMessage, data->depotSE);
			
			
			MSG_WriteByte(&netMessage, data->curWeapon);
			
			MSG_WriteByte(&netMessage, data->itemNumber);
			for (j=0; j<MAX_INVENTORY_SIZE; j++)
				MSG_WriteChar(&netMessage, data->inventory[j]);
			
			for (j=0; j<MAX_DEPOT_SIZE; j++)
				MSG_WriteChar(&netMessage, data->depot[j]);
			
			for (j=0; j<MAX_EQUIPMENT; j++)
				MSG_WriteChar(&netMessage, data->equipment[j]);
			
			MSG_WriteChar(&netMessage, data->mouseInventory);
			
			
			MSG_WriteByte(&netMessage, data->bonusSkillPoint);
			
			for (j=0; j<MAX_NUMBER_OF_SKILL_VARIABLE; j++)
				MSG_WriteShort(&netMessage, data->skillVariable[j]);
			
			
			
			for (j=0; j<MAX_PRECOCITY_SIZE; j++)
			{
				MSG_WriteChar(&netMessage, data->precocityInventory[j]);
				MSG_WriteLong(&netMessage, data->precocityTime[j]);
			}
			
			
			
			MSG_WriteShort(&netMessage, data->chaosPoint);
			
			MSG_WriteShort(&netMessage, data->guildIdx);
			MSG_WriteLong(&netMessage, data->guildJoinTime);
			MSG_WriteLong(&netMessage, data->guildSecedeTime);
			
			
			item = &data->item[0];
			for (j=0; j<MAX_NUMBER_OF_OWNITEM; j++, item++)
			{
				if (item->itemTableIdx < 0) continue;
				
				MSG_WriteByte(&netMessage, j);
				MSG_WriteString(&netMessage, item->name);
				MSG_WriteShort(&netMessage, item->itemTableIdx);
				MSG_WriteByte(&netMessage, item->optionNumber);
				
				for (k=0; k<item->optionNumber; k++)
					MSG_WriteShort(&netMessage, item->optionIdx[k]);
				
				for (k=0; k<item->optionNumber; k++)
					MSG_WriteShort(&netMessage, item->optionValue[k]);
				
				MSG_WriteShort(&netMessage, item->durability);
				MSG_WriteByte(&netMessage, item->reqLevel);
				MSG_WriteLong(&netMessage, item->experience);
				MSG_WriteLong(&netMessage, item->timer);


				
				MSG_WriteLong(&netMessage, item->ItemExtendInfo.UseFlag);
				MSG_WriteLong(&netMessage, (int)item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType);
				MSG_WriteString(&netMessage, item->ItemExtendInfo.ItemSpawnInfomation.data);			
				
				
				
				
				
				if (CTools::IsItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_TIME) )
				{
					if ( item->ItemExtendInfo.AllowUseTime < 0 )
						item->ItemExtendInfo.AllowUseTime = 0;
				}
				
				MSG_WriteLong(&netMessage, item->ItemExtendInfo.AllowUseTime);
				
				if (CTools::IsItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_DATE) )
				{		
					item->ItemExtendInfo.CalclateUseDateToSec = difftime(item->ItemExtendInfo.AllowUseTime , time(NULL));
					if ( item->ItemExtendInfo.CalclateUseDateToSec < 0)
						item->ItemExtendInfo.CalclateUseDateToSec = 0;
				}
				
				MSG_WriteLong(&netMessage, item->ItemExtendInfo.CalclateUseDateToSec);
				
			}
			
			
			MSG_WriteByte(&netMessage, data->skillNumber);
			skill = &data->skill[0];
			for (j=0; j<MAX_NUMBER_OF_SKILL; j++, skill++)
			{
				if (skill->tableIdx < 0) continue;
				
				MSG_WriteByte(&netMessage, j);
				MSG_WriteShort(&netMessage, skill->tableIdx);
				
				MSG_WriteLong(&netMessage, skill->iLevel);
			}
			
			
			
			for (j=0; j<MAX_NUMBER_OF_HOTKEY; j++)
			{
				MSG_WriteByte(&netMessage, data->hotkeyType[j]);
				MSG_WriteChar(&netMessage, data->hotkeyIdx[j]);
			}
			
			
			MSG_WriteLong(&netMessage, data->questPoint);
			MSG_WriteByte(&netMessage, data->questNumber);
			questVar = &data->questVar[0];
			for (j=0; j<MAX_COUNT_QUEST_VARIABLE; j++, questVar++)
				MSG_WriteLong(&netMessage, questVar->iVar);
			
			questInventory = &data->questInventory[0];
			for (j=0; j<MAX_COUNT_QUEST_INVENTORY; j++, questInventory++)
				MSG_WriteLong(&netMessage, questInventory->iItem);
			
			questStatus = &data->questStatus[0];
			
			for (j=0; j<MAX_COUNT_QUEST_STATUS; j++, questStatus++)
			{
				if (questStatus->questIdx < 0) continue;
				
				MSG_WriteByte(&netMessage, questStatus->idx);
				MSG_WriteShort(&netMessage, questStatus->questIdx);
				for (k=0; k<MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD; k++)
					MSG_WriteLong(&netMessage, questStatus->dField[k]);
				
				MSG_WriteString(&netMessage, questStatus->partyOrganizerName);
				MSG_WriteByte(&netMessage, questStatus->partyOrganizeServer);
				MSG_WriteLong(&netMessage, questStatus->partyOrganizeTime);
				MSG_WriteLong(&netMessage, questStatus->clearTime);
				MSG_WriteLong(&netMessage, questStatus->limitTime);
			}
			
			NET_SendMessage(&client->sock, &netMessage);
		}
		MSG_EndWriting( &netMessage );
	} 
}

#include "..\CPremiumPlayerCtrl.h"
extern CPremiumPlayerCtrl* gcpPremiumPlayerCtrl;

#include "..\CTools.h"


void TransmitePacket_MSC_GAMESERVERADDR(i3client_t* pClient,const int characterID)
{
	characterData_t*  pSelectCharactorSlot=NULL;
	pSelectCharactorSlot=CTools::GetCharactorWithCharacterID(pClient,characterID);
	if(NULL == pSelectCharactorSlot) return;


	int groupIdx, memberIdx;
	groupIdx  = pClient->serverGroupID;
	memberIdx = GTH_FindServerWithinGroup(&g_serverGroup[groupIdx],pSelectCharactorSlot->worldIdx);
	if(memberIdx < 0 ) return;


	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_GAMESERVERADDR);
		//lucky
		MSG_WriteString(&netMessage, LANSERVICE_IP);
		//end
		MSG_WriteShort(&netMessage,  g_serverGroup[groupIdx].memberServer[memberIdx].servicePort);
		MSG_WriteByte(&netMessage,  pSelectCharactorSlot->worldIdx);
		NET_SendMessage(&pClient->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



BOOL GTH_RequestGameServerAddr(void)
{
	int CharactorID=-1;
	characterData_t*  pSelectCharactorSlot=NULL;
	characterTable_t* pGenTableRecoder=NULL;
	
	CharactorID  = MSG_ReadByte();	
	pSelectCharactorSlot=CTools::GetCharactorWithCharacterID(g_curClient,CharactorID);
	if(NULL == pSelectCharactorSlot) return FALSE;

	pGenTableRecoder = CTools::GetCharactorGenTableRecord(pSelectCharactorSlot->pcClass);
	if(NULL == pGenTableRecoder) return FALSE;


	int groupIdx=-1;
	int memberIdx=-1;
	groupIdx = g_curClient->serverGroupID;
	
	if(groupIdx < 0) return FALSE;
	if(groupIdx >= MAX_SERVER_GROUP) return FALSE;

	memberIdx = GTH_FindServerWithinGroup(&g_serverGroup[groupIdx], pSelectCharactorSlot->worldIdx);	
	
	if( (memberIdx < 0) ||
		(FALSE == gcpPremiumPlayerCtrl->isAccessMap(pSelectCharactorSlot->worldIdx,g_curClient))){

		pSelectCharactorSlot->worldIdx = pGenTableRecoder->worldIdx;
		vec_copy(pGenTableRecoder->position, pSelectCharactorSlot->position);
		vec_copy(pGenTableRecoder->angles,   pSelectCharactorSlot->angles);		

		
		return TRUE;
	}	

	g_curClient->isComplete = true;	
	g_DBAccountServer->DeleteLogin( g_curClient );
	TransmitePacket_MSC_GAMESERVERADDR(g_curClient,CharactorID);
	

	return TRUE;
}

int GTH_FindServerWithinGroup(serverGroup_t *group, int worldIdx)
{
	int i, j;
	memberServer_t *member;

	member = &group->memberServer[0];
	for (i=0; i<MAX_MEMBER_SERVER; i++, member++)
	{
		if ( !member->active ) continue;
		for (j=0; j<member->worldNumber; j++)
		{
			if ( member->worldList[j] == worldIdx ) 
				return i;
		}
	}

	return -1;
}


void GTH_RequestReadyCreate()
{
	int reply, i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		
		if ( g_curClient->dataNumber >= MAX_CHARACTER_DATA )
		{
			reply = 0;
			MSG_WriteByte(&netMessage, MSC_REPLY_READYCREATE);
			MSG_WriteByte(&netMessage, reply);
		}
		else
		{
			reply = 1;
			MSG_WriteByte(&netMessage, MSC_REPLY_READYCREATE);
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, g_numCharacterTable);
			for (i=0; i<g_numCharacterTable; i++)
			{
				MSG_WriteByte(&netMessage, g_characterTable[i].pcClass);
				MSG_WriteShort(&netMessage, g_characterTable[i].force);
				MSG_WriteShort(&netMessage, g_characterTable[i].soulpower);
				MSG_WriteShort(&netMessage, g_characterTable[i].agility);
				MSG_WriteShort(&netMessage, g_characterTable[i].vitality);
				MSG_WriteShort(&netMessage, g_characterTable[i].bonusStatusPoint);
			}
		}
		NET_SendMessage(&g_curClient->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



BOOL CheckStatusValue(charEditData_t* pEditData)
{
	if ( pEditData == NULL)
		return FALSE;

	
	characterTable_t* pCharTable = NULL;
	pCharTable = CTools::GetCharactorGenTableRecord(pEditData->pcClass);
	if ( pCharTable == NULL )
		return FALSE;

	
	int TotalPoint = 0;
	TotalPoint += pEditData->force;
	TotalPoint += pEditData->soulpower;
	TotalPoint += pEditData->agility;
	TotalPoint += pEditData->vitality;	
	TotalPoint += pEditData->bonusPoint;


	if ( (pEditData->force < 0) || 
		( pEditData->force > pCharTable->force + pCharTable->bonusStatusPoint))
		return FALSE;

	if ( (pEditData->soulpower < 0) || 
		( pEditData->soulpower > pCharTable->soulpower + pCharTable->bonusStatusPoint))
		return FALSE;

	if ( (pEditData->agility < 0) || 
		( pEditData->agility > pCharTable->agility + pCharTable->bonusStatusPoint))
		return FALSE;

	if ( (pEditData->vitality < 0) || 
		( pEditData->vitality > pCharTable->vitality + pCharTable->bonusStatusPoint))
		return FALSE;

	if ( (pEditData->bonusPoint < 0) || 
		( pEditData->bonusPoint > pCharTable->bonusStatusPoint))
		return FALSE;

	
	if ( TotalPoint > pCharTable->TotalStatusPointForGenType )
		return FALSE;

	int MaxHairNumber;
	int MaxFaceNumber;
	
	
	
	if ( ( pEditData->skinPart[0] < 0 )||
		 ( pEditData->skinPart[0] >= pCharTable->MaxHairNumber) ||
		 ( pEditData->skinPart[1] < 0 ) ||
		 ( pEditData->skinPart[1] >=pCharTable->MaxFaceNumber) )
		return FALSE;

	return TRUE;
}

int GTH_RequestCreateCharacter()
{
	charEditData_t	data;
	int characterID;
	int error;

	characterID			= MSG_ReadByte();
	data.pcClass		= MSG_ReadByte();
	strcpy(data.name, MSG_ReadString());
	data.skinPart[0]	= MSG_ReadByte();
	data.skinPart[1]	= MSG_ReadByte();
	data.skinPart[2]	= MSG_ReadByte();
	data.skinPart[3]	= MSG_ReadByte();
	data.force			= MSG_ReadShort();
	data.soulpower		= MSG_ReadShort();
	data.agility		= MSG_ReadShort();
	data.vitality		= MSG_ReadShort();
	data.bonusPoint		= MSG_ReadShort();

	int MembershipType	= MSG_ReadShort();

	
	if (data.pcClass < 0 || data.pcClass > NUM_TABLE_GEN_TYPE) 
	{
		error = 0;
		goto REPLY_ERROR_CREATECHARACTER;
	}

	
	
	
	if ( FALSE == GTH_CheckValidName(data.name))
	{
		error = 0;
		goto REPLY_ERROR_CREATECHARACTER;
	}
	
	
	if ( FALSE == CheckStatusValue(&data) )
	{
		error = 0;
		goto REPLY_ERROR_CREATECHARACTER;
	}

	

	
	if ( !GTH_CheckValidationCharacterID(g_curClient, characterID) )
	{
		error = 0;
		goto REPLY_ERROR_CREATECHARACTER;
	}

	GTH_SetCharacterData(&g_curClient->createData, characterID, &data);

	g_DBGameServer[g_curClient->serverGroupID]->CreateCharacter( g_curClient ); 

	
	
	if(MembershipType)
	{
		g_DBGameServer[g_curClient->serverGroupID]->SaveCharacterSlot(g_curClient,MembershipType);
	}

	return 1;
REPLY_ERROR_CREATECHARACTER:
	GTH_SendMessage_ReplyCreateCharacter(g_curClient, error);
	return 0;
}


void GTH_SendMessage_ReplyCreateCharacter(i3client_t *client, int reply)
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_REPLY_CREATECHARACTER);
		MSG_WriteByte(&netMessage, reply);
		NET_SendMessage(&client->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

int GTH_RequestDeleteCharacter()
{
	int characterID;
	int error;

	characterID = MSG_ReadByte();

	if (characterID < 0 || characterID >= MAX_CHARACTER_DATA) 
	{
		error = 0;
		goto REPLY_ERROR_DELETECHARACTER;
	}

	g_curClient->deleteCharacterID = characterID;
	g_DBGameServer[g_curClient->serverGroupID]->DeleteCharacter(g_curClient, g_curClient->deleteCharacterID);
	return 1;

REPLY_ERROR_DELETECHARACTER:
	GTH_SendMessage_ReplyDeleteCharacter(g_curClient, error);
	return 0;
}

void GTH_SendMessage_ReplyDeleteCharacter(i3client_t *client, int reply)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_REPLY_DELETECHARACTER);
		MSG_WriteByte(&netMessage, reply);
		NET_SendMessage(&client->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

int GTH_RequestCheckCharacterName()
{
	strcpy(g_curClient->createData.name, MSG_ReadString());
	g_DBGameServer[g_curClient->serverGroupID]->CheckCharacterName(g_curClient, g_curClient->serverGroupID, g_curClient->createData.name);

	return 1;
}

void GTH_ReplyCheckCharacterName(i3client_t *client, int reply)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_REPLY_CHECKCHARNAME);
		MSG_WriteByte(&netMessage, reply);
		NET_SendMessage(&client->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



extern CSendingQueue *sendingQueue;
extern CReceivingQueue *receivingQueue;
extern CQueryQueue		*queryRequestQueue, *queryResultQueue;
void GTH_Debug_ServerStatusInfo()
{	
	g_logSystem->Write( "server close packet info -> total send:%d, total recv:%d, send:%d, recv:%d, resend:%d, drop:%d, dup:%d, acksend:%d, ackrecv:%d, un_send:%d, un_recv:%d, un_drop:%d", 
		totalPacketsSend, totalPacketsReceived,
		packetsSent, packetsReceived, packetsReSent, packetsDropCount, packetsDupCount,
		packetsAckSent, packetsAckReceived, un_packetsSent, un_packetsReceived, un_packetsDropCount );

	g_logSystem->Write("Server shutdown status Info.: SendingQueue (%d, %d), ReceivingQueue (%d, %d), QReqQ(%d, %d), QResQ(%d, %d), ClientNumber: %d", 
			sendingQueue->m_count, sendingQueue->m_maxCount, receivingQueue->m_count, receivingQueue->m_maxCount, 
			queryRequestQueue->m_count, queryRequestQueue->m_maxCount, queryResultQueue->m_count, queryResultQueue->m_maxCount, g_clientNumber);
}




void GTH_ProcessMessage_Monitor()
{
	int command;

	if ( !g_curClient->isSuccessLogin )	
	{
		g_logSystem->Write("서버 모니터링 클라이언트에서 로그인 안된 상태에서 서비스를 요청하고 있습니다.(ID: %s, IP:%s)", 
			g_curClient->id, NET_AddrToString((struct sockaddr *)&g_curClient->sock.addr));
		return;
	}

	command = MSG_ReadByte();

	switch ( command )
	{
	case MONITOR_REQUEST_SERVERINFO:
		GTH_SendMessage_Monitor_ServerInfo( g_curClient );
		break;

	case MONITOR_REQUEST_FILTERING:
		if ( g_filterYn )
			g_logSystem->Write("서버 모니터링 클라이언트에서 접속모드를 변경(운영자전용->모두허용)합니다.(ID:%s, IP:%s)", g_curClient->id, NET_AddrToString((struct sockaddr *)&g_curClient->sock.addr));
		else
			g_logSystem->Write("서버 모니터링 클라이언트에서 접속모드를 변경(모두허용->운영자전용)합니다.(ID:%s, IP:%s)", g_curClient->id, NET_AddrToString((struct sockaddr *)&g_curClient->sock.addr));

		g_filterYn = !g_filterYn;

	case MONITOR_CHANGE_FILLTERING_MESSAGE:
		if ( g_filterYn )
		{
			g_filterIdx++;
			if ( g_filterIdx >= MAX_FILTER_MESSAGE )
				g_filterIdx = 0;
		}
		break;

	default :
		break;
	}
}

void GTH_Fill_Monitor_ServerInfo(int startIdx)
{
	int i, j, k;
	serverGroup_t *serverGroup;

	MSG_WriteByte(&netMessage, MSC_MONITOR);
	MSG_WriteByte(&netMessage, MONITOR_REPLY_SERVERINFO);

	
	MSG_WriteByte(&netMessage, 1);	
	MSG_WriteByte(&netMessage, g_config.serverNo);
	MSG_WriteByte(&netMessage, 1); 
	MSG_WriteString(&netMessage, g_hostName);
	MSG_WriteString(&netMessage, g_config.serviceIP);
	MSG_WriteShort(&netMessage, g_config.servicePort);
	MSG_WriteString(&netMessage, g_config.localIP);
	MSG_WriteShort(&netMessage, g_config.localPort);
	MSG_WriteString(&netMessage, g_config.accountDatabaseDSN);
	MSG_WriteString(&netMessage, g_config.gameVersion);
	MSG_WriteByte(&netMessage, g_filterYn);
	MSG_WriteByte(&netMessage, g_filterIdx); 
	MSG_WriteShort(&netMessage, g_clientNumber);
	MSG_WriteShort(&netMessage, g_maxClientNumber);
	MSG_WriteLong(&netMessage, g_concurrentClients);
	MSG_WriteLong(&netMessage, g_maxConcurrentClients);

	
	MSG_WriteByte( &netMessage, GTH_GetNumberActiveServerGroup() );	
	serverGroup = &g_serverGroup[startIdx * MAX_SERVER_GROUP_PER_MESSAGE];
	
	int start = startIdx*MAX_SERVER_GROUP_PER_MESSAGE;
	int end = MAX_SERVER_GROUP_PER_MESSAGE * (startIdx+1);
	
	for (i=start; i<end; i++, serverGroup++)
	{
		MSG_WriteByte(&netMessage, serverGroup->idx);
		MSG_WriteByte(&netMessage, serverGroup->active);
		if ( !serverGroup->active ) continue;
		MSG_WriteString(&netMessage, serverGroup->name);
		MSG_WriteString(&netMessage, serverGroup->DSN);
		MSG_WriteShort(&netMessage, serverGroup->totalClient);
		MSG_WriteShort(&netMessage, serverGroup->maxTotalClient);
		MSG_WriteByte(&netMessage, serverGroup->memberServerNumber);

		for (j=0; j<MAX_MEMBER_SERVER; j++)
		{
			MSG_WriteByte(&netMessage, serverGroup->memberServer[j].idx);
			MSG_WriteByte(&netMessage, serverGroup->memberServer[j].active);
			MSG_WriteString(&netMessage, serverGroup->memberServer[j].name);	
			MSG_WriteString(&netMessage, serverGroup->memberServer[j].hostName);
			MSG_WriteString(&netMessage, serverGroup->memberServer[j].serviceIP);
			MSG_WriteShort(&netMessage, serverGroup->memberServer[j].servicePort);
			MSG_WriteString(&netMessage, serverGroup->memberServer[j].localIP);
			MSG_WriteShort(&netMessage, serverGroup->memberServer[j].localPort);
			MSG_WriteByte(&netMessage, serverGroup->memberServer[j].worldNumber);
			for (k=0; k < serverGroup->memberServer[j].worldNumber; k++)
				MSG_WriteByte(&netMessage, serverGroup->memberServer[j].worldList[k]);
			MSG_WriteShort(&netMessage, serverGroup->memberServer[j].clientNumber);
			MSG_WriteShort(&netMessage, serverGroup->memberServer[j].maxClientNumber);
		}	
	}	
}

void GTH_SendMessage_Monitor_ServerInfo(i3client_t *client)
{
	int serverGroupIdx = 0 ;

	for (int i = 0; i < (MAX_SERVER_GROUP/MAX_SERVER_GROUP_PER_MESSAGE); ++i)
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			GTH_Fill_Monitor_ServerInfo(i);
			NET_SendMessage(&client->sock, &netMessage);
		}
		MSG_EndWriting( &netMessage );
	}
}

int GTH_GetNumberActiveServerGroup()
{
	int i;
	int activeNumber = 0;

	for (i=0; i<MAX_SERVER_GROUP; i++)
	{
		if ( g_serverGroup[i].active ) activeNumber ++;
	}

	return activeNumber;
}

int GTH_GetNumberActiveMemberServer(int groupIdx)
{
	int i;
	int activeNumber = 0;

	if ( !g_serverGroup[groupIdx].active ) return 0;

	for (i=0; i<MAX_MEMBER_SERVER; i++)
	{
		if ( g_serverGroup[groupIdx].memberServer[i].active ) activeNumber ++;
	}

	return activeNumber;
}

int GTH_CalculateConcurrentClients()
{
	int i;
	serverGroup_t *serverGroup;
	int concurrentClients;

	concurrentClients = 0;
	serverGroup = &g_serverGroup[0];
	for (i=0; i<MAX_SERVER_GROUP; i++, serverGroup++)
	{
		if ( !serverGroup->active ) continue;
		concurrentClients += serverGroup->totalClient;
	}

	
	if ( concurrentClients > g_maxConcurrentClients) 
		g_maxConcurrentClients = concurrentClients;

	g_concurrentClients = concurrentClients;
	return g_concurrentClients;
}





void GTH_SendCharacterLoadSlot(i3client_t *client,int day)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, MSC_LOAD_CHARACTER_SLOT);
		
		MSG_WriteByte(&netMessage, client->m_nCharacterSlotType);

		MSG_WriteShort(&netMessage, day);

		NET_SendMessage(&client->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_LoadCharacterSlot()
{
	g_DBGameServer[g_curClient->serverGroupID]->LoadCharacterSlot();
}

void GTH_SaveCharacterSlot()
{
	int type = MSG_ReadByte();
	g_DBGameServer[g_curClient->serverGroupID]->SaveCharacterSlot(g_curClient,type);
}





BOOL	IsVaildString( char *str )
{
	bool en, skipchar;
	
	wchar_t uniCode;
	unsigned char aniCode[3];
	
	while( *str )
	{		
		memset( aniCode, 0, sizeof( aniCode ) );
		
		aniCode[0] = *str;
		
		if( aniCode[0] == '\n' )
		{
			str ++;
			continue;
		}
		
		
		if( aniCode[0] == ' ' )
		{
			str ++; 
			
			
			return 0;
		}
		
		
		if( IsDBCSLeadByte( (BYTE)aniCode[0] ) )
		{
			aniCode[1] = * ++str;
			aniCode[2] = 0;
			MultiByteToWideChar( CP_ACP, MB_COMPOSITE, (char*)aniCode, -1, &uniCode, 1 );
			en = false;
		}
		else
		{
			aniCode[1] = 0;
			MultiByteToWideChar( CP_ACP, MB_COMPOSITE, (char*)aniCode, -1, &uniCode, 1 );
			en = true;
		}
		
		if( uniCode < 0xc800 && uniCode >= 0xac00 ) 
		{
			skipchar = true;
		} 
		else if( uniCode >= 0xc800 && uniCode < 0xd800 )
		{
			skipchar = true;
		} 
		else if( uniCode >= 0x0000 && uniCode <= 0x0100 )
		{
			return TRUE;
		} 
		else if( uniCode < 0x3400 && uniCode >= 0x3100 )
		{
			return FALSE;
		} 
		else
		{
			return FALSE;
		}
		str ++;
	}
	return TRUE;
}


BOOL GTH_CheckValidName( char *name )
{
	char substr[2];
	if( name[0] == 0 )	
		return FALSE;
	
	if ( strstr(name, "GM") != NULL )		
		return FALSE;
	
	if ( strstr(name, "gm") != NULL )	
		return FALSE;	
	
	if ( strstr( name, "gM") != NULL )		
		return FALSE;	
	
	if ( strstr( name, "Gm") != NULL )	
		return FALSE;	
	


	
	for ( int i = 0; i < 128; i++)
	{
		if ( i == 0 ) continue;				
		if (i > 47 && i < 58) continue;		
		if (i > 64 && i < 91) continue;		
		if (i > 96 && i < 123) continue;	
		
		sprintf(substr, "%c", i);
		if ( strstr( name, substr) != NULL )		
			return FALSE;		
	}

	
	if ( strlen( name ) > NAMESTRING - 2 )	
		return FALSE;	
	
	return TRUE;
}
