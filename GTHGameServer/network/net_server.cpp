#include "../global.h"

extern i3sizebuf_t netMessage; 

extern i3socket_t msSocket[MAX_MASTERSERVER];	
extern i3socket_t localSocket;

#include "..\\Guild\\CGuildCtrl.h"
extern CGuildCtrl* gcpGuildCtrl;




int GTH_GetMemberServerIdx(i3packet_t *p)
{
	int command;
	int	idx;

	if ( !NET_PreviewMessage(p) ) return -1;

	MSG_BeginReading();
	command = MSG_ReadByte();

	if (command != GSC_MYSERVERINFO) return -1;

	idx = MSG_ReadByte();

	return idx;
}
 

int GTH_RegistMemberServer()
{
	int i;
	memberServer_t *member;

	
	if (g_config.gameServerNo >= MAX_MEMBER_SERVER || g_config.gameServerNo < 0)
		return false;

	member = &g_memberServer[ g_config.gameServerNo ];

	member->idx = g_config.gameServerNo;
	member->active = true;
	member->isManager = g_config.isManager;
	strcpy( member->name, g_config.gameServerName );
	strcpy( member->hostName, g_hostName );

	strcpy( member->serviceIP, g_config.serviceIP );
	member->servicePort = g_config.servicePort;
	strcpy( member->localIP, g_config.localIP );
	member->localPort = g_config.localPort;
	member->serviceWorldNumber = g_config.serviceWorldNumber;

	for (i=0; i<g_config.serviceWorldNumber; i ++)
		member->serviceWorldList[i] = g_config.serviceWorldList[i];

	member->curClientNumber = 0;
	member->maxClientNumber = 0;
	g_memberServerNumber = 1;

	return true;
}


void GTH_UpdateMyServerInfo()
{

	memberServer_t *member;
	
	member = &g_memberServer[ g_config.gameServerNo ];

	member->curClientNumber = g_pcNumber;
	member->maxClientNumber = g_maxPCNumber;
}



int GTH_AddToServerGroup(int memberIdx, struct sockaddr_in *addr)
{
	memberServer_t *member;

	if (memberIdx >= MAX_MEMBER_SERVER || memberIdx < 0) return -1;

	member = &g_memberServer[memberIdx];

	if ( member->active ) return -1;	

	member->idx = memberIdx;
	member->active = true;

	NET_InitSocket(&member->sock, addr, localSocket.socket, SocketType_Unreliable);

	g_memberServerNumber++;

	return memberIdx;
}

int GTH_FindMemberServerWithAddr(struct sockaddr_in *addr)
{
	memberServer_t *member;
	int i;

	member = &g_memberServer[1];	
	for (i = 1; i < MAX_MEMBER_SERVER; i++, member++)
	{
		if (!member->active)
			continue;

		if (NET_AddrCompare((struct sockaddr *)addr, (struct sockaddr *)&member->sock.addr) == 0)
			return i;
	}
	
	return -1; 
}

int GTH_DropMyServer()
{
	int memberIdx;
	memberServer_t	*member;

	memberIdx = MSG_ReadByte();

	if (memberIdx >= MAX_MEMBER_SERVER || memberIdx < 0) return -1;

	member = &g_memberServer[memberIdx];

	if ( !member->active ) return false;
	if ( member->idx != memberIdx ) return false;

	member->active = false;

	g_memberServerNumber --;

	return true;
}


int GTH_MyServerInfo()
{
	int memberIdx;
	memberServer_t	*member;
	int i;

	memberIdx = MSG_ReadByte();

	if (memberIdx >= MAX_MEMBER_SERVER || memberIdx < 0) return 0;

	member = &g_memberServer[memberIdx];

	strcpy( member->name, MSG_ReadString() );
	strcpy( member->hostName, MSG_ReadString() );
	strcpy( member->serviceIP, MSG_ReadString() );
	member->servicePort = MSG_ReadShort();
	strcpy( member->localIP, MSG_ReadString() );
	member->localPort = MSG_ReadShort();
	member->serviceWorldNumber = MSG_ReadByte();
	for (i=0; i<member->serviceWorldNumber; i++)
		member->serviceWorldList[i] = MSG_ReadByte();

	member->curClientNumber = MSG_ReadShort();
	member->maxClientNumber = MSG_ReadShort();
	member->updateTime = g_globalTime;
	member->active = true;

	return 1;
}



int GTH_InitMemberServer()
{
	int memberIdx;
	memberServer_t	*member;
	int i;

	memset(&g_memberServer[0], 0, sizeof(memberServer_t)*MAX_MEMBER_SERVER);

	memberIdx = g_config.gameServerNo;

	if (memberIdx >= MAX_MEMBER_SERVER || memberIdx < 0) 
	{
		MessageBox(g_hWnd, "Member Server'number is wrong!!", "Error!", MB_OK);
		return 0;
	}

	member = &g_memberServer[memberIdx];

	member->active = true;
	strcpy( member->name, g_config.gameServerName );
	strcpy( member->hostName, "" );
	strcpy( member->serviceIP, g_config.serviceIP );
	member->servicePort = g_config.servicePort;
	strcpy( member->localIP, g_config.localIP );
	member->localPort = g_config.localPort;
	member->serviceWorldNumber = g_config.serviceWorldNumber;
	for (i=0; i<member->serviceWorldNumber; i++)
		member->serviceWorldList[i] = g_config.serviceWorldList[i];

	member->curClientNumber = 0;
	member->maxClientNumber = 0;

	member->updateTime = g_globalTime;

	return 1;
}




void GTH_SendMessage_GameServerGroupInfo()
{
	int i, j;

    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SERVERGROUPINFO);
		MSG_WriteByte(&netMessage, g_config.serverGroupNo);
		MSG_WriteString(&netMessage, g_config.serverGroupName);
		MSG_WriteString(&netMessage, g_config.gameDatabase);	
		
		
		for (i=0; i < MAX_MEMBER_SERVER; i++)
		{
			MSG_WriteByte(&netMessage, g_memberServer[i].active);
			if ( !g_memberServer[i].active )	continue;
			
			MSG_WriteString(&netMessage, g_memberServer[i].name);
			MSG_WriteString(&netMessage, g_memberServer[i].hostName);
			
			MSG_WriteString(&netMessage, g_memberServer[i].serviceIP);
			MSG_WriteShort(&netMessage, g_memberServer[i].servicePort);
			MSG_WriteString(&netMessage, g_memberServer[i].localIP);
			MSG_WriteShort(&netMessage, g_memberServer[i].localPort);
			MSG_WriteByte(&netMessage, g_memberServer[i].serviceWorldNumber);
			
			for (j=0; j < g_memberServer[i].serviceWorldNumber; j++)
				MSG_WriteByte(&netMessage, g_memberServer[i].serviceWorldList[j]);
			
			MSG_WriteShort(&netMessage, g_memberServer[i].curClientNumber);  
			MSG_WriteShort(&netMessage, g_memberServer[i].maxClientNumber); 
		}
		
		for (i=0; i<g_config.masterServerNumber; i++)
			NET_SendUnreliableMessage(&msSocket[i], &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_DropServerGroup()
{
	int i;
    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_DROPSERVERGROUP);
		MSG_WriteByte(&netMessage, g_config.serverGroupNo);	

		for (i=0; i<g_config.masterServerNumber; i++)
			NET_SendUnreliableMessage(&msSocket[i], &netMessage);
	}
	MSG_EndWriting(&netMessage);
}





void GTH_SendMessage_MemberServerInfo()
{
	int i, j;

    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_MEMBERSERVERINFO );
		
		for (i=0; i < MAX_MEMBER_SERVER; i++)
		{
			MSG_WriteByte(&netMessage, g_memberServer[i].active);
			if ( !g_memberServer[i].active )	continue;
			
			MSG_WriteString(&netMessage, g_memberServer[i].name);
			MSG_WriteString(&netMessage, g_memberServer[i].hostName);
			MSG_WriteString(&netMessage, g_memberServer[i].serviceIP);
			MSG_WriteShort(&netMessage, g_memberServer[i].servicePort);
			MSG_WriteString(&netMessage, g_memberServer[i].localIP);
			MSG_WriteShort(&netMessage, g_memberServer[i].localPort);
			MSG_WriteByte(&netMessage, g_memberServer[i].serviceWorldNumber);
			
			for (j=0; j < g_memberServer[i].serviceWorldNumber; j++)
				MSG_WriteByte(&netMessage, g_memberServer[i].serviceWorldList[j]);
			
			MSG_WriteShort(&netMessage, g_memberServer[i].curClientNumber);  
			MSG_WriteShort(&netMessage, g_memberServer[i].maxClientNumber);  
		}
		
		for (i=1; i < MAX_MEMBER_SERVER; i++) 
		{
			if ( !g_memberServer[i].active )	continue;
			
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}


int GTH_MemberServerInfo() 
{
	int i, j;
	int count;

	count = 0;
	for (i=0; i < MAX_MEMBER_SERVER; i++)
	{
		g_memberServer[i].idx = i;
		g_memberServer[i].active = MSG_ReadByte();

		if ( !g_memberServer[i].active ) continue;

		strcpy( g_memberServer[i].name, MSG_ReadString() );
		strcpy( g_memberServer[i].hostName, MSG_ReadString() );
		strcpy( g_memberServer[i].serviceIP, MSG_ReadString() );
		g_memberServer[i].servicePort = MSG_ReadShort();
		strcpy( g_memberServer[i].localIP, MSG_ReadString() );
		g_memberServer[i].localPort = MSG_ReadShort();
		g_memberServer[i].serviceWorldNumber = MSG_ReadByte();
		for (j=0; j < g_memberServer[i].serviceWorldNumber; j++ )
			g_memberServer[i].serviceWorldList[j] = MSG_ReadByte();
		g_memberServer[i].curClientNumber = MSG_ReadShort();
		g_memberServer[i].maxClientNumber = MSG_ReadShort();

		count++;
	}

	
	
	g_memberServer[0].updateTime = g_globalTime;

	g_memberServerNumber = count;
	return 1;
}




void GTH_SendMessage_MyServerInfo()
{
	int i;

    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_MYSERVERINFO);
		
		MSG_WriteByte(&netMessage, g_config.gameServerNo);
		MSG_WriteString(&netMessage, g_config.gameServerName);
		MSG_WriteString(&netMessage, g_hostName );
		MSG_WriteString(&netMessage, g_config.serviceIP);
		MSG_WriteShort(&netMessage, g_config.servicePort);
		MSG_WriteString(&netMessage, g_config.localIP);
		MSG_WriteShort(&netMessage, g_config.localPort);
		MSG_WriteByte(&netMessage, g_config.serviceWorldNumber);
		for (i=0; i < g_config.serviceWorldNumber; i++)
			MSG_WriteByte(&netMessage, g_config.serviceWorldList[i]);
		
		MSG_WriteShort(&netMessage, g_pcNumber);  
		MSG_WriteShort(&netMessage, g_maxPCNumber);  
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_DropMyServer()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_DROPMYSERVER);
		MSG_WriteByte(&netMessage, g_config.gameServerNo);
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


int GTH_FindServerWithinGroup(int worldIdx)
{
	int i, j;
	memberServer_t *member;

	member = &g_memberServer[0];
	for (i=0; i<MAX_MEMBER_SERVER; i++, member++)
	{
		if ( !member->active ) continue;
		for (j=0; j<member->serviceWorldNumber; j++)
		{
			if ( member->serviceWorldList[j] == worldIdx ) 
				return i;
		}
	}

	return -1;
}

int GTH_IsLocalServiceWorld(int worldIdx)
{
	int i;
	memberServer_t *member;

	member = &g_memberServer[ g_config.gameServerNo ];
	if ( !member->active ) return false;

	for (i=0; i<member->serviceWorldNumber; i++)
	{
		if ( member->serviceWorldList[i] == worldIdx ) 
			return true;
	}

	return false;
}

void GTH_CheckProblemMemberServer()
{
	int i;
	memberServer_t *ms;

	ms = &g_memberServer[0];
	for (i=0; i<MAX_MEMBER_SERVER; i++, ms++)
	{
		if ( !ms->active ) continue;
		if ( ms->isManager ) continue; 	

		if ( g_globalTime - ms->updateTime > 3000 ) 
		{
			ms->active = false;
		}
	}
}


void GTH_CheckProblemMasterServer()
{

	memberServer_t *ms;

	ms = &g_memberServer[0];
	if ( ms->active )
	{
		if ( g_globalTime - ms->updateTime > 3000 ) 
		{
			ms->active = false;
			localSocket.unreliableReceiveSequence = 0;
			localSocket.receiveSequence = 0;
		}
	}
}



void GTH_ProcessMessage_SystemAdmin()
{
	int command;
	char notice[256];

	command = MSG_ReadByte();

	switch ( command )
	{
	case SYSTEMADMIN_SERVERSHUTDOWN:
		GTH_SystemAdmin_ServerShutdown();
		break;

	case SYSTEMADMIN_SETSHUTDOWNTIMER:
		GTH_SystemAdmin_SetShutDownTimer();

		
		GTH_ProcessDanbattlePowerEnd();
		
		break;

	case SYSTEMSDMIN_GOTOPLAYER:
		GTH_SystemAdmin_GoToPlayer( g_curPC );
		break;

	case SYSTEMSDMIN_NOTICE:
		GTH_SystemAdmin_Notice();
		break;

	case SYSTEMADMIN_SETNOTICEINFO:
		strcpy(notice, MSG_ReadString());
		GTH_SystemAdmin_SetNoticeInfo( notice );
		break;

	case SYSTEMADMIN_REQUESTFINDPC:
		GTH_SystemAdmin_RequestFindPC();
		break;

	case SYSTEMADMIN_REPLYFINDPC:
		GTH_SystemAdmin_ReplyFindPC();
		break;

	case SYSTEMSDMIN_GOTOMONSTER:
		GTH_SystemAdmin_GoToMonster( g_curPC );
		break;

	case SYSTEMSDMIN_GMBUSYONOFF:
		GTH_SystemAdmin_GMBusyOnOff( g_curPC );
		break;
	case SYSTEMSDMIN_GMHIDEONOFF :
		GTH_SystemAdmin_GMHideOnOff( g_curPC );
		break;

	case SYSTEMSDMIN_REQUESTSUMMONMONSTER :
		GTH_SystemAdmin_SummonMonster( g_curPC );
		break;

	default:
		break;
	}
}

void GTH_SendMessage_SystemShutdownToMember()
{
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_SETSHUTDOWNTIMER);
		
		for (i=1; i < MAX_MEMBER_SERVER; i++) 
		{
			if ( !g_memberServer[i].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_SystemShutdownToManager()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_SERVERSHUTDOWN);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_NoticeToMember( char *notice )
{
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_SETNOTICEINFO);
		MSG_WriteString(&netMessage, notice);
		
		for (i=1; i < MAX_MEMBER_SERVER; i++) 
		{
			if ( !g_memberServer[i].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_NoticeToManager( char *notice )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMSDMIN_NOTICE);
		MSG_WriteString(&netMessage, notice);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProccessMessage_ValidationServerAdmin( playerCharacter_t *pc )
{
	
#ifndef _DEV_TEST
	if ( !GTH_CheckValidation_GameMaster( pc ) )
	{
		int command = MSG_ReadByte();
		g_logSystem->Write("User '%s'(ID: %s) Use Operator Command(%d), User is Not Operator.", g_curPC->name, g_curPC->userID, command);
		return;
	}
#endif

	GTH_ProcessMessage_SystemAdmin();
}

void GTH_SendMessage_RequestFindPCToManager(int reqMemberIdx, char *toName, char *fromName )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_REQUESTFINDPC);
		MSG_WriteByte(&netMessage, reqMemberIdx);
		MSG_WriteString(&netMessage, toName);
		MSG_WriteString(&netMessage, fromName);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_RequestFindPCToMember( int reqMemberIdx, char *toName, char *fromName )
{
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_REQUESTFINDPC);
		MSG_WriteByte(&netMessage, reqMemberIdx);
		MSG_WriteString(&netMessage, toName);
		MSG_WriteString(&netMessage, fromName);
		
		for (i=1; i < MAX_MEMBER_SERVER; i++) 
		{
			if ( !g_memberServer[i].active )	continue;
			if ( i == reqMemberIdx )	continue;	
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ReplyFindPCToManager(int reqMemberIdx, int worldIdx, vec3_t position, char *fromName )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_REPLYFINDPC);
		MSG_WriteByte(&netMessage, reqMemberIdx);
		MSG_WriteByte(&netMessage, worldIdx);
		MSG_WritePosition(&netMessage, position);
		MSG_WriteString(&netMessage, fromName);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ReplyFindPCToMember(int reqMemberIdx, int worldIdx, vec3_t position, char *fromName )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_REPLYFINDPC);
		MSG_WriteByte(&netMessage, reqMemberIdx);
		MSG_WriteByte(&netMessage, worldIdx);
		MSG_WritePosition(&netMessage, position);
		MSG_WriteString(&netMessage, fromName);
		
		NET_SendUnreliableMessage(&g_memberServer[reqMemberIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ReplySummonMonster( playerCharacter_t* pc, int reply, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_REPLYSUMMONMONSTER );
		MSG_WriteByte(&netMessage, reply );
		MSG_WriteByte(&netMessage, error );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_ProcessMessage_Request_PartyInfo()
{
	int command;

	command = MSG_ReadByte();

	switch( command )
	{
	case PARTY_REQUEST_ORGANIZE :
		GTH_ProcessMessage_PartyInfo_Request_Organize();
		break;
	case PARTY_REQUEST_JOIN :
		GTH_ProcessMessage_PartyInfo_Request_Join();
		break;

	case PARTY_OUT :
		GTH_ProcessMessage_PartyInfo_Request_Out();
		break;
	case PARTY_FORCEDOUT :
		GTH_ProcessMessage_PartyInfo_Request_ForcedOut();
		break;
	case PARTY_DISORGANIZE :
		GTH_ProcessMessage_PartyInfo_Request_Disorganize();
		break;


	case PARTY_SENDALL:
		GTH_ProcessMessage_PartyInfo_Request_SendAll();
		break;
	case PARTY_UPDATE:
		GTH_ProcessMessage_PartyInfo_Request_Update();
		break;

	case PARTY_ITEMDIVIDETYPE :

		break;

	case PARTY_HOLDING :
		GTH_ProcessMessage_PartyInfo_Request_Holding();
		break;

	case PARTY_CHATMESSAGE :
		GTH_ProcessMessage_PartyInfo_Request_ChatMessage();
		break;
	}
}






void GTH_ProcessMessage_PartyInfo_Request_Organize()
{
	partyMember_t l, m;
	int organizeTime;

	MSG_ReadMember( &l );
	MSG_ReadMember( &m );
	organizeTime = MSG_ReadLong();
	
	
	GTH_PartyInfo_Organize( &l, &m, organizeTime );
}


void GTH_SendMessage_PartyInfo_Reply_Organize( int memberServerIdx, int partyIdx, partyMember_t *l, partyMember_t *m, int organizeTime, int isLeader )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_REQUEST_ORGANIZE);
		
		MSG_WriteShort(&netMessage, partyIdx);
		MSG_WriteByte(&netMessage, isLeader);
		
		MSG_WriteMember(&netMessage, l);
		MSG_WriteMember(&netMessage, m);
		MSG_WriteLong(&netMessage, organizeTime);
		
		NET_SendUnreliableMessage(&g_memberServer[memberServerIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_ProcessMessage_PartyInfo_Request_Join()
{
	partyMember_t m;
	char organizerName[NAMESTRING];
	int organizeTime, organizeServer;

	MSG_ReadMember( &m );
	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTime = MSG_ReadLong();

	
	GTH_PartyInfo_Join( &m, organizerName, organizeServer, organizeTime );
}

void GTH_SendMessage_PartyInfo_Reply_Join( serverPartyInfo_t *pi, int memberServerIdx, int pcIdx, int isNewMember )
{
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_REQUEST_JOIN);
		MSG_WriteByte( &netMessage, isNewMember );
		
		MSG_WriteShort( &netMessage, pcIdx );
		
		MSG_WriteByte( &netMessage, pi->canJoin );
		MSG_WriteShort( &netMessage, pi->idx );
		MSG_WriteByte( &netMessage, pi->itemDivideType );
		MSG_WriteByte( &netMessage, pi->memberNumber );
		MSG_WriteString( &netMessage, pi->organizerName );
		MSG_WriteChar( &netMessage, pi->organizeServer );
		MSG_WriteLong( &netMessage, pi->organizeTime );
		
	
	
		
		for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
		{
			if( pi->member[i].serverIndex == -1 ) continue;
			MSG_WriteMember( &netMessage, &pi->member[i] );
		}
		
		NET_SendUnreliableMessage(&g_memberServer[memberServerIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_ProcessMessage_PartyInfo_Request_Out()
{
	char memberName[NAMESTRING];
	char organizerName[NAMESTRING];
	int organizeServer, organizeTime;

	strcpy( memberName, MSG_ReadString() );
	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTime = MSG_ReadLong();

	GTH_PartyInfo_Out( false, memberName, organizerName, organizeServer, organizeTime );
}

void GTH_ProcessMessage_PartyInfo_Request_ForcedOut()
{
	char memberName[NAMESTRING];
	char organizerName[NAMESTRING];
	int organizeServer, organizeTime;

	strcpy( memberName, MSG_ReadString() );
	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTime = MSG_ReadLong();

	GTH_PartyInfo_Out( true, memberName, organizerName, organizeServer, organizeTime );
}

void GTH_SendMessage_PartyInfo_Reply_Out( int isForced, partyMember_t *m, int memberIdx, int nextLeaderIdx, int reqFlag )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTYINFO);
		
		if( !isForced ) MSG_WriteByte(&netMessage, PARTY_OUT);
		else MSG_WriteByte(&netMessage, PARTY_FORCEDOUT);
		
		MSG_WriteMember( &netMessage, m );
		MSG_WriteByte( &netMessage, memberIdx );
		MSG_WriteByte( &netMessage, nextLeaderIdx );
		MSG_WriteByte( &netMessage, reqFlag );
		
		NET_SendUnreliableMessage(&g_memberServer[m->serverIndex].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Request_Disorganize()
{
	char organizerName[NAMESTRING];
	int organizeServer, organizeTime;

	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTime = MSG_ReadLong();

	GTH_PartyInfo_Disorganize( organizerName, organizeServer, organizeTime );
}

void GTH_SendMessage_PartyInfo_Reply_Disorganize( partyMember_t *m )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTYINFO);
		
		MSG_WriteByte(&netMessage, PARTY_DISORGANIZE);
		
		MSG_WriteMember( &netMessage, m );
		
		NET_SendUnreliableMessage(&g_memberServer[m->serverIndex].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_ProcessMessage_PartyInfo_Request_SendAll()
{
	int pcIdx, serverIdx;
	char organizerName[NAMESTRING];
	int organizeServer, organizeTIme;

	serverIdx = MSG_ReadChar();
	pcIdx = MSG_ReadShort();
	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTIme = MSG_ReadLong();

	
	GTH_PartyInfo_SendAll( serverIdx, pcIdx, organizerName, organizeServer, organizeTIme );
}

void GTH_SendMessage_PartyInfo_Reply_SendAll( int reply, int serverIdx, int pcIdx, serverPartyInfo_t *pi )
{
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_SENDALL);
		MSG_WriteByte( &netMessage, reply );
		
		
		if( reply )
		{
			MSG_WriteShort( &netMessage, pcIdx );
			
			MSG_WriteByte( &netMessage, pi->canJoin );
			MSG_WriteShort( &netMessage, pi->idx );
			MSG_WriteByte( &netMessage, pi->itemDivideType );
			MSG_WriteByte( &netMessage, pi->memberNumber );
			MSG_WriteString( &netMessage, pi->organizerName );
			MSG_WriteChar( &netMessage, pi->organizeServer );
			MSG_WriteLong( &netMessage, pi->organizeTime );
			
			for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
			{
				if( pi->member[i].serverIndex == -1 ) continue;
				MSG_WriteMember( &netMessage, &pi->member[i] );
			}
		}
		else
		{
			MSG_WriteShort(&netMessage, pcIdx);
		}
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Request_Update()
{
	char memberName[NAMESTRING];
	int active, serverIdx, pcIdx, level, worldIdx;
	char organizerName[NAMESTRING];
	int organizeServer, organizeTime;

	strcpy( memberName, MSG_ReadString() );

	active = MSG_ReadByte();
	serverIdx = MSG_ReadChar();
	pcIdx = MSG_ReadShort();
	level = MSG_ReadByte();
	worldIdx = MSG_ReadByte();

	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTime = MSG_ReadLong();

	GTH_PartyInfo_Update( memberName, active, serverIdx, pcIdx, level, worldIdx, organizerName, organizeServer, organizeTime );
}

void GTH_SendMessage_PartyInfo_Reply_Update( partyMember_t *m, int memberIdx, int nextLeaderIdx, int active, int serverIdx, int pcIdx, int level, int worldIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_UPDATE);
		
		MSG_WriteMember( &netMessage, m );
		
		
		MSG_WriteByte( &netMessage, memberIdx );
		MSG_WriteByte( &netMessage, nextLeaderIdx );
		MSG_WriteByte( &netMessage, active );
		MSG_WriteChar( &netMessage, serverIdx );
		MSG_WriteShort( &netMessage, pcIdx );
		MSG_WriteByte( &netMessage, level );
		MSG_WriteShort( &netMessage, worldIdx );
		
		NET_SendUnreliableMessage(&g_memberServer[m->serverIndex].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Request_ItemDivideType()
{
	char organizerName[NAMESTRING];
	int organizeServer, organizeTime, type;

	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTime = MSG_ReadLong();
	type = MSG_ReadByte();

	GTH_PartyInfo_ItemDivideType( organizerName, organizeServer, organizeTime, type );
}

void GTH_SendMessage_PartyInfo_Reply_ItemDivideType( partyMember_t *m, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_ITEMDIVIDETYPE);
		MSG_WriteMember( &netMessage, m );
		
		MSG_WriteByte( &netMessage, type );
		
		NET_SendUnreliableMessage(&g_memberServer[m->serverIndex].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Request_Holding()
{
	char organizerName[NAMESTRING];
	int organizeServer, organizeTime, holdingFlag;

	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTime = MSG_ReadLong();
	holdingFlag = MSG_ReadChar();

	GTH_PartyInfo_Holding( organizerName, organizeServer, organizeTime, holdingFlag );
}

void GTH_ProcessMessage_PartyInfo_Request_ChatMessage()
{
	char fromName[NAMESTRING];
	char organizerName[NAMESTRING], message[256];
	int organizeServer, organizeTime;

	strcpy( fromName, MSG_ReadString() );
	strcpy( organizerName, MSG_ReadString() );
	organizeServer = MSG_ReadChar();
	organizeTime = MSG_ReadLong();
	strcpy( message, MSG_ReadString() );

	GTH_PartyInfo_ChatMessage( fromName, organizerName, organizeServer, organizeTime, message );
}

void GTH_SendMessage_PartyInfo_Reply_ChatMessage( partyMember_t *m, char* fromName, char *message )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_CHATMESSAGE);
		MSG_WriteMember( &netMessage, m );
		
		MSG_WriteString( &netMessage, fromName );
		MSG_WriteString( &netMessage, message );
		
		NET_SendUnreliableMessage(&g_memberServer[m->serverIndex].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_ProcessMessage_Reply_PartyInfo()
{
	int command;

	command = MSG_ReadByte();

	switch( command )
	{
	case PARTY_REQUEST_ORGANIZE :
		GTH_ProcessMessage_PartyInfo_Reply_Organize();
		break;
	case PARTY_REQUEST_JOIN :
		GTH_ProcessMessage_PartyInfo_Reply_Join();
		break;

	case PARTY_OUT :
		GTH_ProcessMessage_PartyInfo_Reply_Out();
		break;
	case PARTY_FORCEDOUT :
		GTH_ProcessMessage_PartyInfo_Reply_ForcedOut();
		break;
	case PARTY_DISORGANIZE :
		GTH_ProcessMessage_PartyInfo_Reply_Disorganize();
		break;

	case PARTY_SENDALL:
		GTH_ProcessMessage_PartyInfo_Reply_SendAll();
		break;
	case PARTY_UPDATE:
		GTH_ProcessMessage_PartyInfo_Reply_Update();
		break;

	case PARTY_ITEMDIVIDETYPE :
		GTH_ProcessMessage_PartyInfo_Reply_ItemDivideType();
		break;

	case PARTY_CHATMESSAGE :
		GTH_ProcessMessage_PartyInfo_Reply_ChatMessage();
		break;
	}
}






void GTH_SendMessage_PartyInfo_Request_Organize( partyMember_t* l, partyMember_t *m, int organizeTime)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_REQUEST_ORGANIZE);
		MSG_WriteMember(&netMessage, l);
		MSG_WriteMember(&netMessage, m);
		MSG_WriteLong(&netMessage, organizeTime);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Reply_Organize()
{
	partyMember_t l, m;
	int organizeTime, partyIdx, isLeader;
	
	partyIdx = MSG_ReadShort();
	isLeader = MSG_ReadByte();

	MSG_ReadMember( &l );
	MSG_ReadMember( &m );
	organizeTime = MSG_ReadLong();
	
	
	GTH_PartyInfo_Reply_Organize( &l, &m, partyIdx, organizeTime, isLeader );
}







void GTH_SendMessage_PartyInfo_Request_Join( partyMember_t *m, char *organizerName, int organizeServer, int organizeTime )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_REQUEST_JOIN);
		
		MSG_WriteMember(&netMessage, m);
		MSG_WriteString(&netMessage, organizerName);
		MSG_WriteChar(&netMessage, organizeServer);
		MSG_WriteLong(&netMessage, organizeTime);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Reply_Join()
{
	serverPartyInfo_t pi;
	partyMember_t m;
	int i, pcIdx;
	int isNewMember;

	GTH_InitPartyInfo( &pi );

	isNewMember = MSG_ReadByte();

	if( isNewMember )
		i = 0;

	pcIdx = MSG_ReadShort();
	
	pi.canJoin = MSG_ReadByte();
	pi.idx = MSG_ReadShort();
	pi.itemDivideType = MSG_ReadByte();
	pi.memberNumber = MSG_ReadByte();
	strcpy( pi.organizerName, MSG_ReadString() );
	pi.organizeServer = MSG_ReadChar();
	pi.organizeTime = MSG_ReadLong();
	
	for( i = 0; i < pi.memberNumber; i ++ )
	{
		MSG_ReadMember( &m );
		memcpy( &pi.member[m.idx], &m, sizeof( partyMember_t ) );
	}

	GTH_PartyInfo_Reply_Join( &pi, pcIdx, isNewMember );
}





void GTH_SendMessage_PartyInfo_Request_Out( int isForced, char* memberName, char* organizerName, int organizeServer, int organizeTime )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		
		if( !isForced ) MSG_WriteByte(&netMessage, PARTY_OUT);
		else MSG_WriteByte(&netMessage, PARTY_FORCEDOUT);
		
		MSG_WriteString(&netMessage, memberName);
		
		MSG_WriteString(&netMessage, organizerName);
		MSG_WriteChar(&netMessage, organizeServer);
		MSG_WriteLong(&netMessage, organizeTime);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Reply_Out()
{
	partyMember_t m;
	int memberIdx, nextLeaderIdx, reqFlag;

	memset( &m, 0, sizeof( partyMember_t ) );

	MSG_ReadMember( &m );
	memberIdx = MSG_ReadByte();
	nextLeaderIdx = MSG_ReadByte();
	reqFlag = MSG_ReadByte();

	GTH_PartyInfo_Reply_Out( false, &m, memberIdx, nextLeaderIdx, reqFlag );
}

void GTH_ProcessMessage_PartyInfo_Reply_ForcedOut()
{
	partyMember_t m;
	int memberIdx, nextLeaderIdx, reqFlag;

	memset( &m, 0, sizeof( partyMember_t ) );

	MSG_ReadMember( &m );
	memberIdx = MSG_ReadByte();
	nextLeaderIdx = MSG_ReadByte();
	reqFlag = MSG_ReadByte();

	GTH_PartyInfo_Reply_Out( true, &m, memberIdx, nextLeaderIdx, reqFlag );
}

void GTH_SendMessage_PartyInfo_Request_Disorganize( char *organizerName, int organizeServer, int organizeTime )
{
	MSG_BeginWriting(&netMessage);
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		
		MSG_WriteByte(&netMessage, PARTY_DISORGANIZE);
		
		MSG_WriteString(&netMessage, organizerName);
		MSG_WriteChar(&netMessage, organizeServer);
		MSG_WriteLong(&netMessage, organizeTime);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Reply_Disorganize()
{
	partyMember_t m;

	memset( &m, 0, sizeof( partyMember_t ) );

	MSG_ReadMember( &m );

	GTH_PartyInfo_Reply_Disorganize( &m );
}






void GTH_SendMessage_PartyInfo_Request_SendAll( playerCharacter_t *pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_SENDALL);
		
		MSG_WriteByte(&netMessage, g_config.gameServerNo);
		MSG_WriteShort(&netMessage, pc->idx);
		
		MSG_WriteString(&netMessage, pc->organizerName);
		MSG_WriteChar(&netMessage, pc->organizeServer);
		MSG_WriteLong(&netMessage, pc->organizeTime);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Reply_SendAll()
{
	serverPartyInfo_t pi;
	partyMember_t m;
	int i, pcIdx;
	int reply;

	GTH_InitPartyInfo( &pi );

	reply = MSG_ReadByte();
	if( reply )
	{
		pcIdx = MSG_ReadShort();

		pi.canJoin = MSG_ReadByte();
		pi.idx = MSG_ReadShort();
		pi.itemDivideType = MSG_ReadByte();
		pi.memberNumber = MSG_ReadByte();
		strcpy( pi.organizerName, MSG_ReadString() );
		pi.organizeServer = MSG_ReadChar();
		pi.organizeTime = MSG_ReadLong();

		for( i = 0; i < pi.memberNumber; i ++ )
		{
			MSG_ReadMember( &m );
			memcpy( &pi.member[m.idx], &m, sizeof( partyMember_t ) );
		}
	}
	else
	{
		pcIdx = MSG_ReadShort();
	}
	
	GTH_PartyInfo_Reply_SendAll( reply, pcIdx, &pi );
}

void GTH_SendMessage_PartyInfo_Request_Update( char* memberName, int active, int serverIdx, int pcIdx, int level, int worldIdx, char *organizerName, int organizeServer, int organizeTime )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_UPDATE);
		
		MSG_WriteString(&netMessage, memberName);
		
		MSG_WriteByte(&netMessage, active);
		MSG_WriteChar(&netMessage, serverIdx);
		MSG_WriteShort(&netMessage, pcIdx);
		MSG_WriteByte(&netMessage, level);
		MSG_WriteByte(&netMessage, worldIdx);
		
		
		MSG_WriteString(&netMessage, organizerName);
		MSG_WriteChar(&netMessage, organizeServer);
		MSG_WriteLong(&netMessage, organizeTime);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Reply_Update()
{
	partyMember_t m;
	int memberIdx, nextLeaderIdx, active, pcIdx, level, serverIdx;
	int worldIdx;

	memset( &m, 0, sizeof( partyMember_t ) );

	MSG_ReadMember( &m );	

	memberIdx = MSG_ReadByte();
	nextLeaderIdx = MSG_ReadByte();
	active = MSG_ReadByte();
	serverIdx = MSG_ReadChar();
	pcIdx = MSG_ReadShort();
	level = MSG_ReadByte();
	worldIdx = MSG_ReadShort();


	GTH_PartyInfo_Reply_Update( &m, memberIdx, nextLeaderIdx, active, serverIdx, pcIdx, level, worldIdx );
}

void GTH_SendMessage_PartyInfo_Request_ItemDivideType( char *organizerName, int organizeServer, int organizeTime, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		
		MSG_WriteByte(&netMessage, PARTY_ITEMDIVIDETYPE);
		
		MSG_WriteString(&netMessage, organizerName);
		MSG_WriteChar(&netMessage, organizeServer);
		MSG_WriteLong(&netMessage, organizeTime);
		
		MSG_WriteByte(&netMessage, type);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Reply_ItemDivideType()
{
	partyMember_t m;
	int type;

	memset( &m, 0, sizeof( partyMember_t ) );
	MSG_ReadMember( &m );	
	type = MSG_ReadByte();

	GTH_PartyInfo_Reply_ItemDivideType( &m, type );
}

void GTH_SendMessage_PartyInfo_Request_Holding( char *organizerName, int organizeServer, int organizeTime, int holdingFlag )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		
		MSG_WriteByte(&netMessage, PARTY_HOLDING);
		
		MSG_WriteString(&netMessage, organizerName);
		MSG_WriteChar(&netMessage, organizeServer);
		MSG_WriteLong(&netMessage, organizeTime);
		
		MSG_WriteChar(&netMessage, holdingFlag);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PartyInfo_Request_ChatMessage( char* fromName, char *organizerName, int organizeServer, int organizeTime, char *message )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_PARTYINFO);
		MSG_WriteByte(&netMessage, PARTY_CHATMESSAGE);
		
		MSG_WriteString(&netMessage, fromName);
		MSG_WriteString(&netMessage, organizerName);
		MSG_WriteChar(&netMessage, organizeServer);
		MSG_WriteLong(&netMessage, organizeTime);
		
		MSG_WriteString(&netMessage, message);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PartyInfo_Reply_ChatMessage()
{
	partyMember_t m;
	char fromName[256], message[256];

	memset( &m, 0, sizeof( partyMember_t ) );
	MSG_ReadMember( &m );	
	strcpy( fromName, MSG_ReadString() );
	strcpy( message, MSG_ReadString() );

	GTH_PartyInfo_Reply_ChatMessage( &m, fromName, message );
}














void GTH_ProcessMessage_Request_OtherServer()
{
	int command;

	command = MSG_ReadByte();
	switch( command )
	{
	
	case PCLIST_ADD	:
		GTH_ProcessMessage_PCList_Request_Add();
		break;

	
	case PCLIST_DELETE :
		GTH_ProcessMessage_PCList_Request_Delete();
		break;

	
	case PCLIST_UPDATE:
		GTH_ProcessMessage_PCList_Request_Update ();
		break ;

	case PCLIST_FRIENDS_LIST:
		GTH_ProcessMessage_FriendsList_Request_List ();
		break;

	case PCLIST_FRIENDS_UPDATE:
		GTH_ProcessMessage_FriendsList_Request_Update ();
		break;
	}
}


void GTH_ProcessMessage_PCList_Request_Update ()
{
	entityPCList_t pl;
	MSG_ReadPCList ( &pl );

	g_entityListMng.UpdatePCList ( &pl );
}

void GTH_ProcessMessage_PCList_Request_Add()
{
	

	entityPCList_t pl;
	MSG_ReadPCList ( &pl );

	g_entityListMng.AddPCList ( &pl );
}

void GTH_ProcessMessage_PCList_Request_Delete()
{
	entityPCList_t pl;
	MSG_ReadPCList ( &pl );

	g_entityListMng.DeletePCList ( &pl );
}







void GTH_SendMessage_PCList_Request_Add( entityPCList_t *pl )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_OTHERSERVER);
		MSG_WriteByte(&netMessage, PCLIST_ADD);
		
		MSG_WritePCList(&netMessage, pl);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCList_Request_Delete( entityPCList_t *pl )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_OTHERSERVER);
		MSG_WriteByte(&netMessage, PCLIST_DELETE);
		
		MSG_WritePCList(&netMessage, pl);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCList_Request_Update( entityPCList_t *pl )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REQUEST_OTHERSERVER);
		MSG_WriteByte(&netMessage, PCLIST_UPDATE);
		
		MSG_WritePCList(&netMessage, pl);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCList_Reply_Add( entityPCList_t *pl )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_OTHERSERVER);
		MSG_WriteByte(&netMessage, PCLIST_ADD);
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCList_Reply_Delete( entityPCList_t *pl )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_OTHERSERVER);
		MSG_WriteByte(&netMessage, PCLIST_DELETE);
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCList_Reply_Update( entityPCList_t *pl )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_OTHERSERVER);
		MSG_WriteByte(&netMessage, PCLIST_UPDATE);
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_ProcessMessage_Reply_OtherServer()
{
	int command;

	command = MSG_ReadByte();
	switch( command )
	{
	case PCLIST_ADD:
		
		
		break;

	case PCLIST_DELETE:
		
		
		break;

	case PCLIST_FRIENDS_LIST:
		GTH_ProcessMessage_Reply_Friends_List ();
		break;

	case PCLIST_FRIENDS_UPDATE:
		GTH_ProcessMessage_Reply_Friends_Update ();
		break;
	}
}







void GTH_ProcessMessage_Guild_Request_From_MemberServer()
{
	int command;

	command = MSG_ReadByte();

	switch( command )
	{
		
		case GUILD_SYNC:
			gcpGuildCtrl->BetweenMsGs_ManageServerDecoder();
			break;	
		

		case GUILD_ORGANIZE :
			GTH_ProcessMessage_Guild_Organize_Request_From_MemberServer();
			break;

		case GUILD_DISORGANIZE :
			GTH_ProcessMessage_Guild_Disorganize_Request_From_MemberServer();
			break;
		case GUILD_INVITE_REQUEST :
			GTH_ProcessMessage_Guild_Add_Request_From_MemberServer();
			break;
		case GUILD_SECEDE :
			GTH_ProcessMessage_Guild_Remove_Request_From_MemberServer();
			break;
		case GUILD_BANISH:
			GTH_ProcessMessage_Guild_Banish_Request_From_MemberServer();
			break;
		case GUILD_UPDATE:
			GTH_ProcessMessage_Guild_Update_Request_From_MemberServer();
			break;
		case GUILD_CONNECT:
			GTH_ProcessMessage_Guild_Connect_Request_From_MemberServer();
			break;
		case GUILD_CHATMESSAGE:
			GTH_ProcessMessage_Guild_ChatMessage_Request_From_MemberServer();
			break;
		case GUILD_CHANGEMASTER:
			GTH_ProcessMessage_Guild_Change_Master_Request_From_MemberServer();
			break;
		case GUILD_ADDSUBMASTER:
			GTH_ProcessMessage_Guild_Add_SubMaster_Request_From_MemberServer();
			break;
		case GUILD_REMOVESUBMASTER:
			GTH_ProcessMessage_Guild_Remove_SubMaster_Request_From_MemberServer();
			break;
		case GUILD_ADDFACTIONMASTER:
			GTH_ProcessMessage_Guild_Add_FactionMaster_Request_From_MemberServer();
			break;
		case GUILD_REMOVEFACTIONMASTER:
			GTH_ProcessMessage_Guild_Remove_FactionMaster_Request_From_MemberServer();
			break;
		case GUILD_SCHEDULE:
			GTH_ProcessMessage_Guild_Schedule_Request_From_MemberServer();
			break;
		case GUILD_GUILDMARK:
			GTH_ProcessMessage_Guild_GuildMark_Request_From_MemberServer();
			break;
		case GUILD_SUBSCRIBE:
			GTH_ProcessMessage_Guild_Subscribe_Request_From_MemberServer();
			break;
	}
}


void GTH_ProcessMessage_Guild_Organize_Request_From_MemberServer()
{
	if( !g_config.isManager )
		return;

	int error = -1;
	char reqName[NAMESTRING];
	char guildName[GUILDNAMESTRING];
	char guildMark[GUILDMARKSIZE];

	memset( guildMark, 0, GUILDMARKSIZE );

	sstrncpy( reqName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );

	for( int i=0; i<GUILDMARKSIZE; i++ )
		guildMark[i] = MSG_ReadByte();
	
	entityPCList_t reqPl;

	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;

	short guildIdx = GTH_GuildOrganize( reqName, guildName, guildMark, reqPl.serverGroupID, error );


	
	if(guildIdx >= 0){
		gcpGuildCtrl->ManagerServerProc_Organize(guildIdx,guildName,(const unsigned char*)guildMark);
		g_DBGameServer->UpdateGuildIdx(
			reqPl.userID,
			reqPl.serverGroupID,
			reqPl.characterID,
			guildIdx);		
	}
	






	
	GTH_SendMessage_Guild_Organize_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, guildIdx, error );
}


void GTH_ProcessMessage_Guild_Disorganize_Request_From_MemberServer()
{
	if( !g_config.isManager )
		return;

	int error = -1;
	char reqName[NAMESTRING];

	sstrncpy( reqName, MSG_ReadString(), NAMESTRING - 1 );

	short guildIdx = MSG_ReadShort();

	int result = GTH_GuildDisorganize( guildIdx, reqName, error );

	entityPCList_t reqPl;
	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;


	
	if(-1 != result){		
		gcpGuildCtrl->ManagerServerProc_DisOrganize(guildIdx);		
		g_DBGameServer->UpdateGuildIdx(
			reqPl.userID,
			reqPl.serverGroupID,
			reqPl.characterID,
			-1);					
	}
	




	
	GTH_SendMessage_Guild_Disorganize_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, result, error );
}





void GTH_ProcessMessage_Guild_Add_Request_From_MemberServer()
{
	char reqName[NAMESTRING];
	char accName[NAMESTRING];
	int error = -1;
	int accJob, accRank;

	if( !g_config.isManager )
		return;

	short guildIdx = MSG_ReadShort();

	sstrncpy( reqName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( accName, MSG_ReadString(), NAMESTRING - 1 );

	accJob  = MSG_ReadLong();
	accRank = MSG_ReadLong();

	
	if( guildIdx < 0 )
		return;

	entityPCList_t reqPl, accPl;

	
	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;

	
	if( g_entityListMng.FindPCList( accName, &accPl ) == -1 )
		return;

	if( reqPl.serverIdx != accPl.serverIdx )
		return;

	int rankIdxInGuild = -1;
	int result = GTH_GuildAddUser( guildIdx, accName, accJob, rankIdxInGuild, error );

	
	if(-1 != result){		
		g_DBGameServer->UpdateGuildIdx(
			accPl.userID,
			accPl.serverGroupID,
			accPl.characterID,
			guildIdx);
	}
	




	


	GTH_SendMessage_Guild_Add_Reply_To_MemberServer( accPl.serverIdx, reqPl.idxOnServer, accPl.idxOnServer, guildIdx, accRank, rankIdxInGuild, result, error );
}


void GTH_ProcessMessage_Guild_Remove_Request_From_MemberServer()
{
	char name[NAMESTRING];
	int error = -1;

	if( !g_config.isManager )
		return;

	short guildIdx = MSG_ReadShort();

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );

	int rank = MSG_ReadLong();
	


	if( guildIdx < 0 )
		return;

	entityPCList_t pl;

	if( g_entityListMng.FindPCList( name, &pl ) == -1 )
		return;

	
	if( stricmp( g_guild[guildIdx].masterName, name ) == 0 )
	{
		error = GUILD_ERROR_SECEDE_3;
		GTH_SendMessage_Guild_Remove_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, rank, -1, error );
		return;
	}

	int result = GTH_GuildRemoveUser( guildIdx, name, error );

	
	if(-1 != result){	
		g_DBGameServer->UpdateGuildIdx(
			pl.userID,
			pl.serverGroupID,
			pl.characterID,
			-1);
	}
	


	GTH_SendMessage_Guild_Remove_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, rank, result, error );
}


void GTH_ProcessMessage_Guild_Banish_Request_From_MemberServer()
{
	char reqName[NAMESTRING];
	char accName[NAMESTRING];
	int error = -1;

	if( !g_config.isManager )
		return;

	short guildIdx = MSG_ReadShort();

	memset( reqName, 0, NAMESTRING );
	memset( accName, 0, NAMESTRING );
	
	strcpy( reqName, MSG_ReadString() );
	strcpy( accName, MSG_ReadString() );

	int rank = MSG_ReadLong();



	if( guildIdx < 0 )
		return;

	entityPCList_t reqPl, accPl;

	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;

	
	if( stricmp( g_guild[guildIdx].masterName, reqName ) != 0 )
	{
		int i;
		for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
		{
			if( stricmp( g_guild[guildIdx].subMasterName[i], reqName ) == 0 )
				break;
		}
		if( i == MAX_GUILD_SUBMASTERS )
		{
			error = GUILD_ERROR_BANISH_5;
			GTH_SendMessage_Guild_Banish_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, rank, -1, error, 0, accName );
			return;
		}
	}
	
	
	int result = GTH_GuildRemoveUser( guildIdx, accName, error );

	if( result != -1 )
	{
		if( g_entityListMng.FindPCList( accName, &accPl ) == -1 )
		{			
			
			
			error = 0;
			GTH_SendMessage_Guild_Banish_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, rank, result, error, 0, accName );
			return;

		}else{			
			g_DBGameServer->UpdateGuildIdx(
				accPl.userID,
				accPl.serverGroupID,
				accPl.characterID,
				-1);			
		}

		GTH_SendMessage_Guild_Banish_Reply_To_MemberServer( accPl.serverIdx, accPl.idxOnServer, rank, result, error, 1, g_guild[guildIdx].name );
	}

	GTH_SendMessage_Guild_Banish_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, rank, result, error, 0, accName );
}


void GTH_ProcessMessage_Guild_Update_Request_From_MemberServer()
{
	char name[NAMESTRING];

	short guildIdx = MSG_ReadShort();

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );

	int rank	 = MSG_ReadLong();
	int rankIdx  = MSG_ReadLong();
	int type	 = MSG_ReadLong();
	int value	 = MSG_ReadLong();

	if( guildIdx < 0 )
		return;

	entityPCList_t pl;

	if( g_entityListMng.FindPCList( name, &pl ) == -1 )
		return;

	if( rank == -1 && rankIdx == -1 )
		GTH_GuildSetUserInfo( guildIdx, name, rank, rankIdx );

	GTH_GuildUpdateUser( guildIdx, name, rank, rankIdx, type, value );
}

void GTH_ProcessMessage_Guild_Connect_Request_From_MemberServer()
{
	char name[NAMESTRING];

	short guildIdx = MSG_ReadShort();

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );

	if( guildIdx < 0 )
		return;

	entityPCList_t pl;

	if( g_entityListMng.FindPCList( name, &pl ) == -1 )
	{
		return;
	}

	int rankInGuild;
	int rankIdxInGuild;
	
	GTH_GuildSetUserInfo( guildIdx, name, rankInGuild, rankIdxInGuild );

	if( rankInGuild != -1 )
	{
		
		GTH_GuildUpdate( guildIdx, GUILD_UPDATE_ALL, GUILD_ALLRANK, -1, rankInGuild, rankIdxInGuild );

		
		GTH_GuildUpdate( guildIdx, GUILD_UPDATE_USER, rankInGuild, rankIdxInGuild, GUILD_ALLRANK, -1 );

		GTH_SendMessage_Guild_Connect_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, rankInGuild, rankIdxInGuild, g_guild[guildIdx].name );
	}
	else
	{
		
		GTH_SendMessage_Guild_Connect_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, rankInGuild, rankIdxInGuild, g_guild[guildIdx].name );
	}
}

void GTH_ProcessMessage_Guild_ChatMessage_Request_From_MemberServer()
{
	short guildIdx;
	char name[NAMESTRING];
	char chatMessage[CHATSTRING];

	guildIdx = MSG_ReadShort();

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( chatMessage, MSG_ReadString(), CHATSTRING );
	
	GTH_GuildChatMessage( guildIdx, name, chatMessage );


}

void GTH_ProcessMessage_Guild_Change_Master_Request_From_MemberServer()
{
	short guildIdx;
	char reqName[NAMESTRING];
	char accName[NAMESTRING];
	int accJob;
	int error = -1;

	guildIdx = MSG_ReadShort();
	sstrncpy( reqName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( accName, MSG_ReadString(), NAMESTRING - 1 );
	accJob	 = MSG_ReadLong();



	if( guildIdx < 0 )
		return;

	entityPCList_t reqPl, accPl;

	
	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;

	
	if( g_entityListMng.FindPCList( accName, &accPl ) == -1 )
		return;

	if( reqPl.serverIdx != accPl.serverIdx )
		return;

	int rankIdxInGuild = -1;
	int result = GTH_GuildChangeMaster( guildIdx, reqName, accName, accJob, rankIdxInGuild, error );

	GTH_SendMessage_Guild_Change_Master_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, accPl.idxOnServer, result, error, rankIdxInGuild );
}

void GTH_ProcessMessage_Guild_Add_SubMaster_Request_From_MemberServer()
{
	short guildIdx;
	char reqName[NAMESTRING];
	char accName[NAMESTRING];
	int accJob;
	int error = -1;

	guildIdx = MSG_ReadShort();
	sstrncpy( reqName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( accName, MSG_ReadString(), NAMESTRING - 1 );
	accJob	 = MSG_ReadLong();



	if( guildIdx < 0 )
		return;

	entityPCList_t reqPl, accPl;

	
	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;

	
	if( g_entityListMng.FindPCList( accName, &accPl ) == -1 )
		return;

	if( reqPl.serverIdx != accPl.serverIdx )
		return;

	int rankIdxInGuild = -1;
	int result = GTH_GuildAddSubMaster( guildIdx, accName, accJob, rankIdxInGuild, error );

	GTH_SendMessage_Guild_Add_SubMaster_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, accPl.idxOnServer, result, error, rankIdxInGuild );
}

void GTH_ProcessMessage_Guild_Remove_SubMaster_Request_From_MemberServer()
{
	short guildIdx;
	char reqName[NAMESTRING];
	char accName[NAMESTRING];
	int accJob;
	int error = -1;
	int rankIdxInGuild;
	int result;

	guildIdx = MSG_ReadShort();
	sstrncpy( reqName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( accName, MSG_ReadString(), NAMESTRING - 1 );
	accJob	 = MSG_ReadLong();



	if( guildIdx < 0 )
		return;

	entityPCList_t reqPl, accPl;

	
	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;

	
	if( g_entityListMng.FindPCList( accName, &accPl ) == -1 )
	{
		rankIdxInGuild = -1;
		result = GTH_GuildRemoveSubMaster( guildIdx, accName, error );

		GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, -1, accName, result, error );

		return;
	}

	if( reqPl.serverIdx != accPl.serverIdx )
		return;

	
	if( stricmp( g_guild[guildIdx].masterName, reqName ) != 0 )
	{
		error = GUILD_ERROR_AUTHORITY;
		GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, accPl.idxOnServer, accName, -1, error );

		return;
	}

	rankIdxInGuild = -1;
	result = GTH_GuildRemoveSubMaster( guildIdx, accName, error );

	GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, accPl.idxOnServer, accName, result, error );
}

void GTH_ProcessMessage_Guild_Add_FactionMaster_Request_From_MemberServer()
{
	short guildIdx;
	char reqName[NAMESTRING];
	char accName[NAMESTRING];
	int accJob;
	int error = -1;

	guildIdx = MSG_ReadShort();
	sstrncpy( reqName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( accName, MSG_ReadString(), NAMESTRING - 1 );
	accJob = MSG_ReadLong();



	if( guildIdx < 0 )
		return;

	entityPCList_t reqPl, accPl;

	
	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;

	
	if( g_entityListMng.FindPCList( accName, &accPl ) == -1 )
		return;

	if( reqPl.serverIdx != accPl.serverIdx )
		return;

	int rankIdxInGuild = -1;
	int result = GTH_GuildAddFactionMaster( guildIdx, accName, accJob, rankIdxInGuild, error );

	GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, accPl.idxOnServer, result, error, rankIdxInGuild );
}

void GTH_ProcessMessage_Guild_Remove_FactionMaster_Request_From_MemberServer()
{
	short guildIdx;
	char reqName[NAMESTRING];
	char accName[NAMESTRING];
	int accJob;
	int error = -1;
	int rankIdxInGuild;
	int result;

	guildIdx = MSG_ReadShort();
	sstrncpy( reqName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( accName, MSG_ReadString(), NAMESTRING - 1 );
	accJob = MSG_ReadLong();



	if( guildIdx < 0 )
		return;

	entityPCList_t reqPl, accPl;

	
	if( g_entityListMng.FindPCList( reqName, &reqPl ) == -1 )
		return;

	
	if( g_entityListMng.FindPCList( accName, &accPl ) == -1 )
	{
		rankIdxInGuild = -1;
		result = GTH_GuildRemoveFactionMaster( guildIdx, accName, error );

		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, -1, accName, result, error );

		return;
	}

	if( reqPl.serverIdx != accPl.serverIdx )
		return;

	
	char authority = 0;
	if( stricmp( g_guild[guildIdx].masterName, reqName ) == 0 )
		authority = 1;

	for( int i=0; i<MAX_GUILD_SUBMASTERS; i++ )
	{
		if( stricmp( g_guild[guildIdx].subMasterName[i], reqName ) == 0 )
		{
			authority = 1;
			break;
		}
	}

	if( authority == 0 )
	{
		error = GUILD_ERROR_AUTHORITY;
		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, accPl.idxOnServer, accName, -1, error );

		return;
	}

	rankIdxInGuild = -1;
	result = GTH_GuildRemoveFactionMaster( guildIdx, accName, error );

	GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_MemberServer( reqPl.serverIdx, reqPl.idxOnServer, accPl.idxOnServer, accName, result, error );
}

void GTH_ProcessMessage_Guild_Schedule_Request_From_MemberServer()
{
	short guildIdx;
	char name[NAMESTRING];
	char notice[100];
	bool flag = false;
	int i;

	guildIdx = MSG_ReadShort();

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( notice, MSG_ReadString(), sizeof( notice ) );




	if( !stricmp( g_guild[guildIdx].masterName, name ) )
	{
		flag = true;
	}
	else
	{
		
		for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
		{
			if( g_guild[guildIdx].subMasterName[i][0] == 0 ) continue;
				if( !stricmp( g_guild[guildIdx].subMasterName[i], name ) )
			{
				flag = true;
				break;
			}
		}
			for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
		{
			if( g_guild[guildIdx].factionMasterName[i][0] == 0 ) continue;
				if( !stricmp( g_guild[guildIdx].factionMasterName[i], name ) )
			{
				flag = true;
				break;
			}
		}
	}

	if( flag )
	{
		strcpy( g_guild[guildIdx].notice, notice );

		int update = GUILD_NOTICE;
		GTH_GuildUpdate( guildIdx, update, -1, -1, GUILD_ALLRANK, -1 );
	}

}

void GTH_ProcessMessage_Guild_GuildMark_Request_From_MemberServer()
{
	short guildIdx;
	char name[NAMESTRING];
	char guildMark[GUILDMARKSIZE];
	unsigned int updateTime;

	guildIdx = MSG_ReadShort();
	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );



	entityPCList_t pl;

	if( g_entityListMng.FindPCList( name, &pl ) == -1 )
		return;

	
	int i;
	for( i=0; i<MAX_GUILDS; i++ )
	{
		if( guildIdx == g_guild[i].idx )
		{
			updateTime = g_guild[i].idx;
			memcpy( guildMark, g_guild[guildIdx].mark, GUILDMARKSIZE );
			break;
		}
	}

	if (i<MAX_GUILDS)
		GTH_SendMessage_Guild_GuildMark_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, guildIdx, updateTime, guildMark );

}

void GTH_ProcessMessage_Guild_Subscribe_Request_From_MemberServer()
{
	int guildIdx;
	int error;
	int fromMemberIdx;
	char fromName[NAMESTRING], guildName[GUILDNAMESTRING];

	sstrncpy( fromName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );
	fromMemberIdx = MSG_ReadByte();



	entityPCList_t pl;

	if( g_entityListMng.FindPCList( fromName, &pl ) == -1 )
		return;

	for( int i=0; i<MAX_GUILDS; i++ )
	{
		if( !stricmp( g_guild[i].name, guildName ) )
		{
			guildIdx = i;
			break;
		}
	}

	if( i == MAX_GUILDS )
	{
		error = GUILD_ERROR_NOGUILD;
		GTH_SendMessage_Guild_Subscribe_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, -1, error );
		return;
	}

	int bSended = 0;
	
	if( g_guild[guildIdx].userLocation[0] != -1 )
	{
		
		GTH_Send_Subscribe_Request_Message_To_Server( g_guild[guildIdx].masterName, fromName, fromMemberIdx );
		bSended = 1;
		GTH_SendMessage_Guild_Subscribe_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, 0, 0, guildName );
	}
	else
	{
		
		int temp = MAX_GUILD_MASTERS;
		for( int i=0; i<MAX_GUILD_SUBMASTERS; i++ )
		{
			if( g_guild[guildIdx].userLocation[i+temp] != -1 )
			{
				
				GTH_Send_Subscribe_Request_Message_To_Server( g_guild[guildIdx].subMasterName[i], fromName, fromMemberIdx );
				bSended = 1;
				GTH_SendMessage_Guild_Subscribe_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, 0, i+temp, guildName );
			}
		}
	}
	
	if( bSended == 0 )
	{
		error = GUILD_ERROR_NOMASTER;
		GTH_SendMessage_Guild_Subscribe_Reply_To_MemberServer( pl.serverIdx, pl.idxOnServer, -1, error );
		return;
	}

}

void GTH_ProcessMessage_Guild_Subscribe_Request_From_ManagerServer()
{
	int idx;
	char toName[NAMESTRING], fromName[NAMESTRING];

	sstrncpy(toName, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy(fromName, MSG_ReadString(), NAMESTRING - 1 );

	idx = GTH_FindPCByName( toName );
	if (idx >= 0)		
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
			MSG_WriteByte(&netMessage, GUILD_SUBSCRIBE_REQUEST);
			MSG_WriteString(&netMessage, fromName);	
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
}




void GTH_SendMessage_Guild_Organize_Reply_To_MemberServer( int serverIdx, int idxOnServer, short guildIdx, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_ORGANIZE);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteShort(&netMessage, guildIdx);
		
		if( guildIdx != -1 )
		{
			
			MSG_WriteLong(&netMessage, g_guild[guildIdx].markUpdateTime);
			MSG_WriteString(&netMessage, g_guild[guildIdx].name);
			
			for( int i=0; i<GUILDMARKSIZE; i++ )
				MSG_WriteByte(&netMessage, g_guild[guildIdx].mark[i]);
		}
		else
		{
			MSG_WriteShort(&netMessage, error);
		}
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Disorganize_Reply_To_MemberServer( int serverIdx, int idxOnServer, int result, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_DISORGANIZE);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Add_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, short guildIdx, int rank, int rankIdx, int result, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_INVITE_REPLY);
		
		MSG_WriteLong(&netMessage, reqIdx);
		MSG_WriteLong(&netMessage, accIdx);
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteLong(&netMessage, rank);
		MSG_WriteLong(&netMessage, rankIdx);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Remove_Reply_To_MemberServer( int serverIdx, int idxOnServer, int rank, int result, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SECEDE);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteLong(&netMessage, rank);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Banish_Reply_To_MemberServer( int serverIdx, int idxOnServer, int rank, int result, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_BANISH);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteLong(&netMessage, rank);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		MSG_WriteString(&netMessage, name);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Subscribe_Reply_To_MemberServer( int serverIdx, int idxOnServer, int result, int error, char *guildName )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SUBSCRIBE);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		
		if( result != -1 )
			MSG_WriteString(&netMessage, guildName);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}


void GTH_SendMessage_Guild_Update_To_MemberServer( int serverIdx, int idxOnServer, guild_t *guild, int update, int rank, int rankIdx )
{
	int i;
	
	
	if( idxOnServer < 0 || idxOnServer >= MAX_PCS ) return;
	if( guild->idx < 0 || guild->idx > MAX_GUILDS )  return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_UPDATE);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteShort(&netMessage, guild->idx);
		MSG_WriteLong(&netMessage, update);
		MSG_WriteLong(&netMessage, rank);
		MSG_WriteLong(&netMessage, rankIdx);
		
		int addRankIdx = 0;
		
		if( rank == GUILD_SUBMASTER )
			addRankIdx = MAX_GUILD_MASTERS;
		else if( rank == GUILD_FACTIONMASTER )
			addRankIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS;
		else if( rank == GUILD_MATE )
			addRankIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS;
		else
			addRankIdx = 0;
		
		if( ( update & GUILD_IDX ) != 0 )
		{
			MSG_WriteShort(&netMessage, guild->idx);
		}
		
		if( ( update & GUILD_SERVERGROUPID ) != 0 )
		{
			MSG_WriteShort(&netMessage, guild->serverGroupID);
		}
		
		if( ( update & GUILD_NAME ) != 0 )
		{
			MSG_WriteString(&netMessage, guild->name);
		}
		
		if( ( update & GUILD_MARK ) != 0 )
		{
			
			
		}
		
		if( ( update & GUILD_MARKUPDATETIME ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->markUpdateTime);
		}
		
		if( ( update & GUILD_NOTICE ) != 0 )
		{
			MSG_WriteString(&netMessage, guild->notice);
		}
		
		if( ( update & GUILD_FAMEPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->famePoint);
		}
		
		if( ( update & GUILD_EXPPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->expPoint);
		}
		
		if( ( update & GUILD_RAIDPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->raidPoint);
		}
		
		if( ( update & GUILD_PVPPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->pvpPoint);
		}
		
		if( ( update & GUILD_HUNTERPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->hunterPoint);
		}
		
		if( ( update & GUILD_LEVEL ) != 0 )
		{
			MSG_WriteShort(&netMessage, guild->level);
		}
		
		if( ( update & GUILD_MASTERNAME ) != 0 )
		{
			MSG_WriteString(&netMessage, guild->masterName);
		}
		
		if( ( update & GUILD_SUBMASTERNAME ) != 0 )
		{
			if( rankIdx == -1 )
			{
				for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
					MSG_WriteString(&netMessage, guild->subMasterName[i]);
			}
			else
				MSG_WriteString(&netMessage, guild->subMasterName[rankIdx]);
		}
		
		if( ( update & GUILD_FACTIONMASTERNAME ) != 0 )
		{
			if( rankIdx == -1 )
			{
				for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
					MSG_WriteString(&netMessage, guild->factionMasterName[i]);
			}
			else
				MSG_WriteString(&netMessage, guild->factionMasterName[rankIdx]);
		}
		
		if( ( update & GUILD_MATENAME ) != 0 )
		{
			if( rankIdx == -1 )
			{
				for( i=0; i<MAX_GUILD_MATES; i++ )
					MSG_WriteString(&netMessage, guild->mateName[i]);
			}
			else
				MSG_WriteString(&netMessage, guild->mateName[rankIdx]);
		}
		
		if( ( update & GUILD_MATEAUTHORITY ) != 0 )
		{
			if( rankIdx == -1 )
			{
				for( i=0; i<MAX_GUILD_MATES; i++ )
					MSG_WriteByte(&netMessage, guild->mateAuthority[i]);
			}
			else
				MSG_WriteByte(&netMessage, guild->mateAuthority[rankIdx]);
		}
		
		if( ( update & GUILD_MATELASTCONNECTTIME ) != 0 )
		{
			if( rankIdx == -1 )
			{
				for( i=0; i<MAX_GUILD_MATES; i++ )
					MSG_WriteLong(&netMessage, guild->mateLastConnectTime[i]);
			}
			else
				MSG_WriteLong(&netMessage, guild->mateLastConnectTime[rankIdx]);
		}
		
		if( ( update & GUILD_MASTERNOTE ) != 0 )
		{
			if( rankIdx == -1 )
			{
				
			}
			else
				MSG_WriteString(&netMessage, guild->masterNote[rankIdx]);
		}
		
		if( ( update & GUILD_USERLEVEL ) != 0 )
		{
			if( rankIdx == -1 )
			{
				int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;
				
				for( i=0; i<max; i++ )
					MSG_WriteLong(&netMessage, guild->userLevel[i]);
			}
			else
				MSG_WriteLong(&netMessage, guild->userLevel[rankIdx+addRankIdx]);
		}
		
		if( ( update & GUILD_USERJOB ) != 0 )
		{
			if( rankIdx == -1 )
			{
				int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;
				
				for( i=0; i<max; i++ )
					MSG_WriteLong(&netMessage, guild->userJob[i]);
			}
			else
				MSG_WriteLong(&netMessage, guild->userJob[rankIdx+addRankIdx]);
		}
		
		if( ( update & GUILD_USERLOCATION ) != 0 )
		{
			if( rankIdx == -1 )
			{
				int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;
				
				for( i=0; i<max; i++ )
					MSG_WriteLong(&netMessage, guild->userLocation[i]);
			}
			else
				MSG_WriteLong(&netMessage, guild->userLocation[rankIdx+addRankIdx]);
		}
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}


void GTH_SendMessage_Guild_Connect_Reply_To_MemberServer( int serverIndex, int idxOnServer, int rankInGuild, int rankIdxInGuild, char *guildName )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_CONNECT);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteLong(&netMessage, rankInGuild);
		MSG_WriteLong(&netMessage, rankIdxInGuild);
		MSG_WriteString(&netMessage, guildName);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIndex].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}


void GTH_SendMessage_Guild_ChatMessage_Reply_To_MemberServer( int serverIndex, int idxOnServer, char *name, char *chatMessage )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_CHATMESSAGE);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteString(&netMessage, name);
		MSG_WriteString(&netMessage, chatMessage);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIndex].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Change_Master_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, int result, int error, int rankIdxInGuild )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_CHANGEMASTER);
		
		MSG_WriteLong(&netMessage, reqIdx);
		MSG_WriteLong(&netMessage, accIdx);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, rankIdxInGuild);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Add_SubMaster_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, int result, int error, int rankIdxInGuild )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_ADDSUBMASTER);
		
		MSG_WriteLong(&netMessage, reqIdx);
		MSG_WriteLong(&netMessage, accIdx);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, rankIdxInGuild);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, char *accName, int result, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_REMOVESUBMASTER);
		
		MSG_WriteLong(&netMessage, reqIdx);
		MSG_WriteLong(&netMessage, accIdx);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteString(&netMessage, accName);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, int result, int error, int rankIdxInGuild )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_ADDFACTIONMASTER);
		
		MSG_WriteLong(&netMessage, reqIdx);
		MSG_WriteLong(&netMessage, accIdx);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, rankIdxInGuild);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_MemberServer( int serverIdx, int reqIdx, int accIdx, char *accName, int result, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_REMOVEFACTIONMASTER);
		
		MSG_WriteLong(&netMessage, reqIdx);
		MSG_WriteLong(&netMessage, accIdx);
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteString(&netMessage, accName);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}

void GTH_SendMessage_Guild_GuildMark_Reply_To_MemberServer( int serverIdx, int idxOnServer, short guildIdx, unsigned int updateTime, char *guildMark )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_GUILDMARK);
		
		MSG_WriteLong(&netMessage, idxOnServer);
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteLong(&netMessage, updateTime);
		
		for( int i=0; i<GUILDMARKSIZE; i++ )
			MSG_WriteByte(&netMessage, guildMark[i]);
		
		NET_SendUnreliableMessage(&g_memberServer[serverIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);


}







void GTH_ProcessMessage_Guild_Reply_From_ManagerServer()
{
	int command;
	int i, idx, guildIdx, reqIdx, accIdx, rank, rankIdx;
	int result, who, error = -1;
	


	char name[NAMESTRING];
	char guildName[GUILDNAMESTRING];
	char chatMessage[CHATSTRING];

	command = MSG_ReadByte();

	switch( command )
	{
		
		case GUILD_SYNC:
			gcpGuildCtrl->BetweenMsGs_GameServerDecoder();
			break;	
		

	case GUILD_ORGANIZE :

		idx		 = MSG_ReadLong();
		guildIdx = MSG_ReadShort();

		if( guildIdx != -1 )
		{
			g_guild[guildIdx].markUpdateTime = MSG_ReadLong();

			sstrncpy( g_guild[guildIdx].name, MSG_ReadString(), GUILDNAMESTRING - 1 );

			for( i=0; i<GUILDMARKSIZE; i++ )
				g_guild[guildIdx].mark[i] = MSG_ReadByte();
		}
		else
		{	
			error = MSG_ReadShort();
		}



		
		if( idx < 0 || idx >= MAX_PCS ) break;


		
		g_pc[idx].guildIdx = guildIdx;

		

		if( g_pc[idx].guildIdx != -1 )
		{
			g_pc[idx].curChargeSE -= GUILD_ORGANIZABLE_NAK;
			g_pc[idx].rankPoint -= GUILD_ORGANIZABLE_RANKPOINT;
	
			if( 0 > g_pc[idx].rankPoint) g_pc[idx].rankPoint = 0;
					
			strcpy( g_pc[idx].guildName, g_guild[guildIdx].name );
			GTH_SendMessage_Guild_Organize_Reply_To_Client( &g_pc[idx], 0, error );
		}
		else
		{
			GTH_SendMessage_Guild_Organize_Reply_To_Client( &g_pc[idx], -1, error );
		}

		break;

	case GUILD_DISORGANIZE :
		{		
			
			idx		= MSG_ReadLong();
			result	= MSG_ReadShort();
			error	= MSG_ReadShort();
			
			
			
			
			if( idx < 0 || idx >= MAX_PCS ) break;
			
			
			
			
			
			
			
			if( result != -1 )
				g_pc[idx].guildIdx = -1;
			
			
			if( result == 1 )
			{
				
				
				
			}
			
			
			if( result == 0 )
				g_pc[idx].rankPoint -= 50;
			
			
			time_t NowTime = time(NULL) + DELAY_DAN_CREATE_SEC;
			g_pc[idx].GuildDeconstructionTime = NowTime;
			
			
			NowTime = time(NULL) + DELAY_DAN_REJOIN_SEC_FOR_GENERAL_REMOVE;
			g_pc[idx].GuildsecedeTime = NowTime;
			
			
			if( 0 > g_pc[idx].rankPoint) g_pc[idx].rankPoint = 0;
			
			GTH_SendMessage_Guild_Disorganize_Reply_To_Client( &g_pc[idx], result, error );
			
		}
		break;

	case GUILD_UPDATE :

		GTH_ProcessMessage_Guild_Update_From_ManagerServer();

		break;

	case GUILD_INVITE_REPLY:

		reqIdx	 = MSG_ReadLong();
		accIdx	 = MSG_ReadLong();
		guildIdx = MSG_ReadShort();
		rank	 = MSG_ReadLong();
		rankIdx  = MSG_ReadLong();
		result	 = MSG_ReadShort();
		error	 = MSG_ReadShort();

		
		if( reqIdx < 0 || reqIdx >= MAX_PCS ) break;
		if( accIdx < 0 || accIdx >= MAX_PCS ) break;




		
		if( result != -1 )
		{
			g_pc[accIdx].guildIdx = guildIdx;
			g_pc[accIdx].rankInGuild = rank;
			g_pc[accIdx].rankIdxInGuild = rankIdx;
			strcpy( g_pc[accIdx].guildName, g_guild[guildIdx].name );
		}
		else
		{
			error = GUILD_ERROR_CANNOTFIND;
		}



		GTH_SendMessage_Guild_Invite_Reply_To_Client( &g_pc[reqIdx], result, error, 0, g_pc[accIdx].name );			
		GTH_SendMessage_Guild_Invite_Reply_To_Client( &g_pc[accIdx], result, error, 1, g_pc[reqIdx].guildName );	

		GTH_DeleteNoticePC( &g_pc[reqIdx] );
		GTH_DeleteNoticePC( &g_pc[accIdx] );

		break;

	case GUILD_SECEDE:

		idx		= MSG_ReadLong();
		rank	= MSG_ReadLong();
		result	= MSG_ReadShort();
		error	= MSG_ReadShort();



		
		if( idx < 0 || idx >= MAX_PCS ) break;


		
		if( result != -1 )
		{
			g_pc[idx].guildIdx = -1;
			g_pc[idx].rankPoint -= 10;

			if( 0 > g_pc[idx].rankPoint) g_pc[idx].rankPoint = 0;

			
			time_t NowTime = time(NULL) + DELAY_DAN_REJOIN_SEC_FOR_GENERAL_REMOVE;
			g_pc[idx].GuildsecedeTime = NowTime;

		}

		GTH_SendMessage_Guild_Secede_Reply_To_Client( &g_pc[idx], result, error );

		break;

	case GUILD_BANISH:

		idx		= MSG_ReadLong();
		rank	= MSG_ReadLong();
		result	= MSG_ReadShort();
		error	= MSG_ReadShort();
		who		= MSG_ReadShort();

		sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );



		
		if( idx < 0 || idx >= MAX_PCS ) break;


		if( result != -1 )
		{
			if( who == 0 )
			{
				if( error == 0 )
				{
					
					
					g_DBGameServer->BanishMemberGuild(name,10);
						
				}
			}
			else
			{
				g_pc[idx].guildIdx = -1;
				g_pc[idx].rankPoint -= 10;

				if( 0 > g_pc[idx].rankPoint) g_pc[idx].rankPoint = 0;

			
				time_t NowTime = time(NULL) + DELAY_DAN_REJOIN_SEC_FOR_POWER_REMOVE;
				g_pc[idx].GuildsecedeTime = NowTime;

			}

			
			
			
		}

		GTH_SendMessage_Guild_Banish_Reply_To_Client( &g_pc[idx], result, error, who, name );

		break;

	case GUILD_CONNECT:

		idx		= MSG_ReadLong();
		rank	= MSG_ReadLong();
		rankIdx = MSG_ReadLong();

		sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );



		
		if( idx < 0 || idx >= MAX_PCS ) break;


		g_pc[idx].rankInGuild = rank;
		g_pc[idx].rankIdxInGuild = rankIdx;

		
		if( g_pc[idx].rankInGuild == -1 )
		{
			g_pc[idx].rankInGuild = -1;
			g_pc[idx].rankIdxInGuild = -1;
			memset( g_pc[idx].guildName, 0, GUILDNAMESTRING );
			g_pc[idx].guildIdx = -1;
		}

		break;

	case GUILD_CHATMESSAGE:

		idx = MSG_ReadLong();

		sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
		sstrncpy( chatMessage, MSG_ReadString(), CHATSTRING );



		
		if( idx < 0 || idx >= MAX_PCS ) break;

	
		GTH_SendMessage_Guild_ChatMessage_Reply_To_Client( &g_pc[idx], name, chatMessage );

		break;

	case GUILD_CHANGEMASTER:

		reqIdx  = MSG_ReadLong();
		accIdx  = MSG_ReadLong();
		result  = MSG_ReadShort();
		error   = MSG_ReadShort();
		rankIdx = MSG_ReadShort();



		
		if( reqIdx < 0 || reqIdx >= MAX_PCS ) break;
		if( accIdx < 0 || accIdx >= MAX_PCS ) break;



		if( result != -1 )
		{
			g_pc[accIdx].rankInGuild = GUILD_MASTER;
			g_pc[accIdx].rankIdxInGuild = 0;
		}

		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( &g_pc[reqIdx], result, error, 0, g_pc[accIdx].name ); 
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( &g_pc[accIdx], result, error, 1, g_pc[accIdx].name ); 

		break;

	case GUILD_ADDSUBMASTER:

		reqIdx  = MSG_ReadLong();
		accIdx  = MSG_ReadLong();
		result  = MSG_ReadShort();
		error   = MSG_ReadShort();
		rankIdx = MSG_ReadShort();



		
		if( reqIdx < 0 || reqIdx >= MAX_PCS ) break;
		if( accIdx < 0 || accIdx >= MAX_PCS ) break;



		if( result != -1 )
		{
			g_pc[accIdx].rankInGuild = GUILD_SUBMASTER;
			g_pc[accIdx].rankIdxInGuild = rankIdx;
		}

		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( &g_pc[reqIdx], result, error, 0, g_pc[accIdx].name ); 
		GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( &g_pc[accIdx], result, error, 1, g_pc[accIdx].name ); 

		break;

	case GUILD_REMOVESUBMASTER:

		reqIdx = MSG_ReadLong();
		accIdx = MSG_ReadLong();
		result = MSG_ReadShort();
		error  = MSG_ReadShort();
		sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );



		
		if( reqIdx < 0 || reqIdx >= MAX_PCS ) break;
		if( !g_pc[reqIdx].active || !g_pc[reqIdx].ready )
		{
			error = GUILD_ERROR_OFFLINE;
			GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( &g_pc[reqIdx], -1, error, 0, name ); 

			break;
		}

		GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( &g_pc[reqIdx], result, error, 0, name ); 

		if( accIdx != -1 )
		{
			if( !g_pc[accIdx].active || !g_pc[accIdx].ready ) break; 

			GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( &g_pc[accIdx], result, error, 1, g_pc[accIdx].name ); 
		}

		break;

	case GUILD_ADDFACTIONMASTER:

		reqIdx  = MSG_ReadLong();
		accIdx  = MSG_ReadLong();
		result  = MSG_ReadShort();
		error   = MSG_ReadShort();
		rankIdx = MSG_ReadShort();



		
		if( reqIdx < 0 || reqIdx >= MAX_PCS ) break;
		if( accIdx < 0 || accIdx >= MAX_PCS ) break;



		if( result != -1 )
		{
			g_pc[accIdx].rankInGuild = GUILD_FACTIONMASTER;
			g_pc[accIdx].rankIdxInGuild = rankIdx;
		}

		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( &g_pc[accIdx], result, error, 1, g_pc[accIdx].name ); 
		GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( &g_pc[reqIdx], result, error, 0, g_pc[accIdx].name ); 

		break;

	case GUILD_REMOVEFACTIONMASTER:

		reqIdx = MSG_ReadLong();
		accIdx = MSG_ReadLong();
		result = MSG_ReadShort();
		error  = MSG_ReadShort();
		sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );



		
		if( reqIdx < 0 || reqIdx >= MAX_PCS ) break;
		if( !g_pc[reqIdx].active || !g_pc[reqIdx].ready )
		{
			error = GUILD_ERROR_OFFLINE;
			GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( &g_pc[reqIdx], -1, error, 0, name ); 

			break; 
		}

		GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( &g_pc[reqIdx], result, error, 0, g_pc[accIdx].name ); 

		if( accIdx != -1 )
		{
			if( !g_pc[accIdx].active || !g_pc[accIdx].ready ) break; 

			GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( &g_pc[accIdx], result, error, 1, g_pc[accIdx].name ); 
		}

		break;

	case GUILD_GUILDMARK:
		
		
		

		
		break;

	
	
	
	case GUILD_SUBSCRIBE:

		idx		= MSG_ReadLong();
		result	= MSG_ReadShort();
		error	= MSG_ReadShort();

		if( result != -1 )
		{
			sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );
			GTH_SendMessage_Guild_Subscribe_Reply_To_Client( &g_pc[idx], result, error, guildName );
		}
		else
		{
			GTH_SendMessage_Guild_Subscribe_Reply_To_Client( &g_pc[idx], result, error );
		}

		break;

	
	
	
	case GUILD_SUBSCRIBE_REQUEST:

		GTH_ProcessMessage_Guild_Subscribe_Request_From_ManagerServer();

		break;

	}
}


void GTH_ProcessMessage_Guild_Update_From_ManagerServer()
{
	int i, idx;
	short guildIdx;
	int update;
	int rank, rankIdx;
	char name[NAMESTRING];
	char guildName[GUILDNAMESTRING];

	idx		 = MSG_ReadLong();
	guildIdx = MSG_ReadShort();
	update	 = MSG_ReadLong();
	rank	 = MSG_ReadLong();
	rankIdx  = MSG_ReadLong();

	
	if( idx < 0 || idx >= MAX_PCS ) return;
	if( guildIdx < 0 || guildIdx >= MAX_GUILDS ) return;

	int addRankIdx = 0;

	if( rank == GUILD_SUBMASTER )
		addRankIdx = MAX_GUILD_MASTERS;
	else if( rank == GUILD_FACTIONMASTER )
		addRankIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS;
	else if( rank == GUILD_MATE )
		addRankIdx = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS;
	else
		addRankIdx = 0;

	if( ( update & GUILD_IDX ) != 0 )
	{
		g_guild[guildIdx].idx = MSG_ReadShort();
	}
	
	if( ( update & GUILD_SERVERGROUPID ) != 0 )
	{
		g_guild[guildIdx].serverGroupID = MSG_ReadShort();
	}
	
	if( ( update & GUILD_NAME ) != 0 )
	{
		sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );
		strcpy( g_guild[guildIdx].name, guildName );
		strcpy( g_pc[idx].guildName, guildName );
	}
	
	if( ( update & GUILD_MARK ) != 0 )
	{


	}
	
	if( ( update & GUILD_MARKUPDATETIME ) != 0 )
	{
		g_guild[guildIdx].markUpdateTime = MSG_ReadLong();
	}
	
	if( ( update & GUILD_NOTICE ) != 0 )
	{
		sstrncpy( g_guild[guildIdx].notice, MSG_ReadString(), sizeof( g_guild[guildIdx].notice ) );
	}
	
	if( ( update & GUILD_FAMEPOINT ) != 0 )
	{
		g_guild[guildIdx].famePoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_EXPPOINT ) != 0 )
	{
		g_guild[guildIdx].expPoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_RAIDPOINT ) != 0 )
	{
		g_guild[guildIdx].raidPoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_PVPPOINT ) != 0 )
	{
		g_guild[guildIdx].pvpPoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_HUNTERPOINT ) != 0 )
	{
		g_guild[guildIdx].hunterPoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_LEVEL ) != 0 )
	{
		g_guild[guildIdx].level = MSG_ReadShort();
	}
	
	if( ( update & GUILD_MASTERNAME ) != 0 )
	{
		sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
		strcpy( g_guild[guildIdx].masterName, name );
	}
	
	if( ( update & GUILD_SUBMASTERNAME ) != 0 )
	{
		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
			{
				sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
				strcpy( g_guild[guildIdx].subMasterName[i], name );
			}
		}
		else
		{
			sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
			strcpy( g_guild[guildIdx].subMasterName[rankIdx], name );
		}
	}
	
	if( ( update & GUILD_FACTIONMASTERNAME ) != 0 )
	{
		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++)
			{
				sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
				strcpy( g_guild[guildIdx].factionMasterName[i], name );
			}
		}
		else
		{
			sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
			strcpy( g_guild[guildIdx].factionMasterName[rankIdx], name );
		}
	}
	
	if( ( update & GUILD_MATENAME ) != 0 )
	{
		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_MATES; i++)
			{
				sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
				strcpy( g_guild[guildIdx].mateName[i], name );
			}
		}
		else
		{
			sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
			strcpy( g_guild[guildIdx].mateName[rankIdx], name );
		}
	}
	
	if( ( update & GUILD_MATEAUTHORITY ) != 0 )
	{
		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_MATES; i++)
				g_guild[guildIdx].mateAuthority[i] = MSG_ReadByte();
		}
		else
			g_guild[guildIdx].mateAuthority[rankIdx] = MSG_ReadByte();
	}
	
	if( ( update & GUILD_MATELASTCONNECTTIME ) != 0 )
	{
		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_MATES; i++)
				g_guild[guildIdx].mateLastConnectTime[i] = MSG_ReadLong();
		}
		else
			g_guild[guildIdx].mateLastConnectTime[rankIdx] = MSG_ReadLong();
	}
	
	if( ( update & GUILD_MASTERNOTE ) != 0 )
	{
		if( rankIdx == -1 )
		{


		}
		else
			strcpy( g_guild[guildIdx].masterNote[rankIdx], MSG_ReadString() );
	}
	
	if( ( update & GUILD_USERLEVEL ) != 0 )
	{
		if( rankIdx == -1 )
		{
			int max = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + MAX_GUILD_MATES;

			for( i=0; i<max; i++ )
				g_guild[guildIdx].userLevel[i] = MSG_ReadLong();
		}
		else
			g_guild[guildIdx].userLevel[rankIdx+addRankIdx] = MSG_ReadLong();
	}
	
	if( ( update & GUILD_USERJOB ) != 0 )
	{
		if( rankIdx == -1 )
		{
			int max = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + MAX_GUILD_MATES;

			for( i=0; i<max; i++ )
				g_guild[guildIdx].userJob[i] = MSG_ReadLong();
		}
		else
			g_guild[guildIdx].userJob[rankIdx+addRankIdx] = MSG_ReadLong();
	}
	
	if( ( update & GUILD_USERLOCATION ) != 0 )
	{
		if( rankIdx == -1 )
		{
			int max = MAX_GUILD_MASTERS + MAX_GUILD_SUBMASTERS + MAX_GUILD_FACTIONMASTERS + MAX_GUILD_MATES;

			for( i=0; i<max; i++ )
				g_guild[guildIdx].userLocation[i] = MSG_ReadLong();
		}
		else
			g_guild[guildIdx].userLocation[rankIdx+addRankIdx] = MSG_ReadLong();
	}

	if( !g_pc[idx].active || !g_pc[idx].ready ) return;

	GTH_SendMessage_Guild_Update_To_Client( &g_pc[idx], &g_guild[guildIdx], update, rank, rankIdx );
}






void GTH_SendMessage_Guild_Organize_Request_To_ManagerServer( playerCharacter_t *pc, char* guildName, char* guildMark )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_ORGANIZE);
		
		MSG_WriteString(&netMessage, pc->name);
		MSG_WriteString(&netMessage, guildName);
		
		for( int i=0; i<GUILDMARKSIZE; i++ )
			MSG_WriteByte(&netMessage, guildMark[i]);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Disorganize_Request_To_ManagerServer( playerCharacter_t* pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_DISORGANIZE);
		
		MSG_WriteString(&netMessage, pc->name);
		MSG_WriteShort(&netMessage, pc->guildIdx);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}





void GTH_SendMessage_Guild_Add_Request_To_ManagerServer( playerCharacter_t *reqPC, playerCharacter_t *accPC, int rank )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_INVITE_REQUEST);
		
		MSG_WriteShort(&netMessage, reqPC->guildIdx);
		MSG_WriteString(&netMessage, reqPC->name);
		MSG_WriteString(&netMessage, accPC->name);
		MSG_WriteLong(&netMessage, accPC->pcJob);
		MSG_WriteLong(&netMessage, rank);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Remove_Request_To_ManagerServer( playerCharacter_t *pc, int rank )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_SECEDE);
		
		MSG_WriteShort(&netMessage, pc->guildIdx);
		MSG_WriteString(&netMessage, pc->name);
		MSG_WriteLong(&netMessage, rank);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Banish_Request_To_ManagerServer( short guildIdx, char *reqName, char *accName, int rank )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_BANISH);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, reqName);
		MSG_WriteString(&netMessage, accName);
		MSG_WriteLong(&netMessage, rank);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Update_Request_To_ManagerServer( short guildIdx, char *name, int rank, int rankIdx, int type, int value )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_UPDATE);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, name);
		MSG_WriteLong(&netMessage, rank);
		MSG_WriteLong(&netMessage, rankIdx);
		MSG_WriteLong(&netMessage, type);
		MSG_WriteLong(&netMessage, value);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Connect_Reqeust_To_ManagerServer( short guildIdx, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_CONNECT);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, name);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_ChatMessage_Request_To_ManagerServer( short guildIdx, char *name, char *chatMessage )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_CHATMESSAGE);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, name);
		MSG_WriteString(&netMessage, chatMessage);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Change_Master_Request_To_ManagerServer( short guildIdx, playerCharacter_t *reqPC, playerCharacter_t *accPC )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_CHANGEMASTER);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, reqPC->name);
		MSG_WriteString(&netMessage, accPC->name);
		MSG_WriteLong(&netMessage, accPC->pcJob);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}



void GTH_SendMessage_Guild_Add_SubMaster_Request_To_ManagerServer( short guildIdx, playerCharacter_t *reqPC, playerCharacter_t *accPC )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_ADDSUBMASTER);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, reqPC->name);
		MSG_WriteString(&netMessage, accPC->name);
		MSG_WriteLong(&netMessage, accPC->pcJob);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Remove_SubMaster_Request_To_ManagerServer( playerCharacter_t *reqPC, char *accName, int accPCJob )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_REMOVESUBMASTER);
		
		MSG_WriteShort(&netMessage, reqPC->guildIdx);
		MSG_WriteString(&netMessage, reqPC->name);
		MSG_WriteString(&netMessage, accName);
		MSG_WriteLong(&netMessage, accPCJob);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Add_FactionMaster_Request_To_ManagerServer( short guildIdx, playerCharacter_t *reqPC, playerCharacter_t *accPC )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_ADDFACTIONMASTER);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, reqPC->name);
		MSG_WriteString(&netMessage, accPC->name);
		MSG_WriteLong(&netMessage, accPC->pcJob);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Remove_FactionMaster_Request_To_ManagerServer( playerCharacter_t *reqPC, char *accName, int accPCJob )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_REMOVEFACTIONMASTER);
		
		MSG_WriteShort(&netMessage, reqPC->guildIdx);
		MSG_WriteString(&netMessage, reqPC->name);
		MSG_WriteString(&netMessage, accName);
		MSG_WriteLong(&netMessage, accPCJob);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Guild_Schedule_Request_To_ManagerServer( short guildIdx, playerCharacter_t *pc, char *notice )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_SCHEDULE);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, pc->name);
		MSG_WriteString(&netMessage, notice);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Guild_GuildMark_Request_To_ManagerServer( playerCharacter_t *pc, short guildIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_GUILDMARK);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteString(&netMessage, pc->name);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Guild_Subscribe_Request_To_ManagerServer( playerCharacter_t *pc, char *guildName )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_SUBSCRIBE);
		
		MSG_WriteString(&netMessage, pc->name);
		MSG_WriteString(&netMessage, guildName);
		MSG_WriteByte(&netMessage, g_config.gameServerNo);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}
