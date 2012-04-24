#include "../global.h"

i3socket_t msSocket[MAX_MASTERSERVER];	
i3socket_t gsSocket;	
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

#include "..\GonryunPracticeBattle\CGonryunPracticeBattle.h"
extern CGonryunPracticeBattle		g_GonryunPracticeBattle;					

#include "..\Tools\CTools.h"
extern CTools* gcpTools;


#include "..\ODBC(open database connectivity)\tagDataBase.h"

#include "../Terra\Billing\CTerraBillingCtrl.h"
extern CTerraBillingCtrl* gcpTerraBillingCtrl;

#include "..\GolryunBattle\CGolryunBattle_Observer.h"
extern CGolryunBattle_Observer* gcpGolryunBattle_Observer;

#include "..\GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;




#include "../network/Gs_To_Gms.h"
extern CGs_To_Gms g_GsToGms;

#include "../network/Gms_To_Gs.h"
extern CGms_To_Gs g_GmsToGs;




int CHECK_PROBLEM_FLAG	=	true;
 
int GTH_Network_Startup()
{
	int i;

	if ( !NET_OpenNetwork() )  
	{
		g_logSystem->Write("Network Initialize Fialure!!");
		MessageBox(g_hWnd, "Network Initialize Fialure!!", "Error!", MB_OK);
		return 0;
	}

	
	if ( !NET_OpenSocketForServer(&gsSocket, g_config.serviceIP, g_config.servicePort) )
	{
		g_logSystem->Write("Can not Opne Game Server Socket.");
		MessageBox(g_hWnd, "Can not Opne Game Server Socket.", "Error!", MB_OK);
		return 0;
	}
	
	if ( g_config.isManager )	
	{
		for (i=0; i<g_config.masterServerNumber; i++)
		{
			
			if ( !NET_OpenSocketForClient(&msSocket[i], g_config.masterServerIP[i], g_config.masterServerPort[i], SocketType_Unreliable ) )  
			{
				g_logSystem->Write("Can Not Open Master Server Socket.");
				MessageBox(g_hWnd, "Can Not Open Master Server Socket.", "Error!", MB_OK);
				return 0;
			}
		}

		if ( !NET_OpenSocketForServer(&localSocket, g_config.localIP, g_config.localPort, SocketType_Unreliable ) )
		{
			g_logSystem->Write("Can Not Open Local Manager Server Socket.");
			MessageBox(g_hWnd, "Can Not Open Local Manager Server Socket.", "Error!", MB_OK);
			return 0;
		}
	}
	else
	{
		
		if ( !NET_OpenSocketForClient(&localSocket, g_config.managerServerIP, g_config.managerServerPort, SocketType_Unreliable ) )  
		{
			g_logSystem->Write("Can Not Open Local Manager Server Socket.");
			MessageBox(g_hWnd, "Can Not Open Local Manager Server Socket.", "Error!", MB_OK);
			return 0;
		}
	}


#ifdef _USE_SENDING_THREAD
	if ( !NET_CreateSendingThread() )
	{
		char msg[256];

		sprintf(msg,"Sending Thread Creating Failure(ErrorCode:%d)", GetLastError());
		MessageBox(g_hWnd, msg, "Error", MB_OK); 
		return 0;
	}	
#endif

#ifdef _USE_RECEIVING_THREAD
	if ( !NET_CreateReceivingThread() )
	{
		char msg[256];

		sprintf(msg,"Receiving Thread Creating Failure(ErrorCode:%d)", GetLastError());
		MessageBox(g_hWnd, msg, "Error", MB_OK); 
		return 0;
	}	
#endif

	
	strcpy( g_hostName, NET_GetHostName() );

	return 1;
}

void GTH_Network_Shutdown()
{
	int i;
	
#ifdef _USE_SENDING_THREAD
	NET_DestroySendingThread();
#endif

#ifdef _USE_RECEIVING_THREAD
	NET_DestroyReceivingThread();
#endif

	if ( g_config.isManager )
	{
		for (i=0; i<g_config.masterServerNumber; i++)
			NET_CloseSocket(&msSocket[i]);
	}
	NET_CloseSocket(&localSocket);

	NET_CloseSocket(&gsSocket);
	NET_CloseNetwork();
}


void GTH_CheckProblemSocket()
{
	int i;
	playerCharacter_t *pc;
	int waitTime;

	pc = &g_pc[0];
	for (i=0; i < MAX_PCS; i++, pc++)
	{
		if ( !pc->active ) continue;
		if ( !pc->normalNetwork )			
		{
			continue;
		}

		if(NULL != gcpTerraBillingCtrl){
			gcpTerraBillingCtrl->CheckPlayer(pc);
		}


		
		NET_ControlSendBank( &pc->sock );

		if( !pc->ready ) waitTime = 5 * 60 * 1000;
		else waitTime = 2 * 60 * 1000;

		if( !NET_CheckSockProblem( &pc->sock, waitTime ) && CHECK_PROBLEM_FLAG )
		{
			g_logSystem->Write("User [%s] int ready = false Status, abnormal Connection Closed.(over packet map)", pc->userID);			
			g_logSystem->WriteToLog( pc, LOG_CLASS_DISCONNECT, "%d;;;;;%s;", 1, NET_AddrToIPString((struct sockaddr *)&pc->sock.addr) );
			
			
			
			GTH_ForcedQuit( pc );

			
			GTH_PartyInfo_Request_Update( pc, false );
			
			if ( pc->worldIdx == DAN_BATTLEMAP_NO)							
			{
				g_logSystem->WriteRLog("Delete Team....Call By GTH_DisconnectGameServer");
				g_pDanBattleSys->DeleteTeam( pc );
				
				
				pc->worldIdx = g_binding[pc->bindingIdx].worldIdx;
				vec_copy(g_binding[pc->bindingIdx].pos, pc->position);
				
			}

			
			GTH_RecoveryReliablePosition( pc );
			
			DB_SaveGameData( pc, SAVEGAMEDATA_QUIT,packet_communication_timeout,normal);
			pc->normalNetwork = false;
			continue;
		}
	}
}



























void TransmitePacket_sc_disconnect(playerCharacter_t* pPlayer, int MessageIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, SC_DISCONNECT);
		
		MSG_WriteLong(&netMessage, MessageIdx);
		NET_SendMessage(&pPlayer->sock, &netMessage);		
	}

	MSG_EndWriting(&netMessage);
}



















BOOL GTH_IsValidPacketForNewClient(i3packet_t *p)
{
	int command;
	char userID[IDSTRING+1];

	if( !NET_PreviewMessage(p) ) return FALSE;

	MSG_BeginReading();
	command = MSG_ReadByte();
	if(command != CC_CONNECT_GAMESERVER) return FALSE;

	strncpy(userID, MSG_ReadString() ,IDSTRING);
	userID[IDSTRING]=NULL;

	playerCharacter_t* pTempPlayer=NULL;

	for(int idx = 0; idx < MAX_PCS; idx ++ )
	{
		pTempPlayer = gcpTools->GetPlayerRecordPointer(idx);
		if(NULL == pTempPlayer) continue;
		
		if(0 ==  stricmp(userID, pTempPlayer->userID)){
			g_logSystem->Write( "Duplicate Login Error!! : User [%s] isolated. NormalNetwork=%d", 
				pTempPlayer->userID ,
				pTempPlayer->normalNetwork);
			return FALSE;
		}
	}


	return TRUE;
}




int GTH_FindPCWithAddr(struct sockaddr_in *addr)
{
	playerCharacter_t* pTempPlayer=NULL;

	for(int idx = 0; idx < MAX_PCS; idx++)
	{
		
		
		
		
		pTempPlayer = &g_pc[idx];
		if(!pTempPlayer->active) continue;

		if(0 == NET_AddrCompare((struct sockaddr *)addr, (struct sockaddr *)&pTempPlayer->sock.addr))
			return idx;

		
	}
	
	return -1; 
}




int	GTH_Network_GetMessage(i3socket_t *sock)
{
	struct sockaddr_in	readaddr;
	int					length;
	int				    pcIdx, memberIdx;
	int					ret;
	int					socket;	

	
	g_curPC = NULL;

	ret = PacketAnalysis_None;

	length = NET_ReadPacket(&socket, (byte *)&packetBuffer, &readaddr);

	
	if (length == 0 || length < NET_HEADERSIZE || length > NET_DATAGRAMSIZE ) 
	{
		return PacketAnalysis_None;
	}
	
	if ( socket == gsSocket.socket )
	{
		pcIdx = GTH_FindPCWithAddr(&readaddr);
		if ( pcIdx == -1 )	
		{
			if (FALSE == GTH_IsValidPacketForNewClient(&packetBuffer) ) 
				return 1; 
			
			if ( g_pcNumber >= MAX_SERVICE_PCS )
			{
				
				
				GTH_SendMessage_RejectConnection(&readaddr, REJECT_OVER_MAXCLIENT);
				g_logSystem->Write("Can Not Insert User, Over MAX_SERVICE_PCS");
				return 1; 
			}
			
			pcIdx = GTH_SpawnPC(&readaddr);						
			
			if ( pcIdx < 0 || pcIdx >= MAX_PCS ) return -1;
		}
		
		
		g_curPC = &g_pc[pcIdx];
		ret = NET_ControlPacket(&g_curPC->sock, &packetBuffer);	 
	}
	else if ( socket == localSocket.socket )
	{
		if ( g_config.isManager )	
		{
			memberIdx = GTH_FindMemberServerWithAddr( &readaddr );
			if ( memberIdx == -1 )	
			{
				memberIdx = GTH_GetMemberServerIdx( &packetBuffer );
				
				if ( memberIdx == -1 ) return 1; 
				GTH_AddToServerGroup(memberIdx, &readaddr);
			}
			
			ret = NET_ControlPacket(&g_memberServer[memberIdx].sock, &packetBuffer);	 
		}
		else 
		{
			ret = NET_ControlPacket(&localSocket, &packetBuffer);	 
		}
	}

	return ret;
}


static int receiveCount;

int GTH_ListenSocket() 
{
	int command;
	int state;
	int processedPacketCount;

	g_netTime = NET_GetNetTime();

	GTH_CheckProblemSocket();

	processedPacketCount = 0;
	while ( processedPacketCount < MAX_PACKET_PER_FRAME ) 
	{
		state = GTH_Network_GetMessage(&gsSocket); 
		
		if ( state < PacketAnalysis_Skip )  
			return 0;

		if ( state != PacketAnalysis_Start ) 
			continue;   

		processedPacketCount++; 
				
		MSG_BeginReading();
		command = MSG_ReadByte();



		


		

		switch(command)
		{
		case CC_TRANSPORTWORLD:
			GTH_ProcessMessage_TransportWorld();
			continue;			

		case CC_READYTOPLAY: 
			GTH_ProcessMessage_ReadyToPlay();
			continue;
			

		case CC_CONNECT_GAMESERVER:
 			GTH_ConnectGameServer();
			continue;
			

		case CC_DISCONNECT_GAMESERVER:
			GTH_DisconnectGameServer();
			continue;
			

			
		case GSC_DROPMYSERVER:
			GTH_DropMyServer();
			continue;
			

		case GSC_MYSERVERINFO:
			GTH_MyServerInfo();
			continue;
			

		case GSC_MEMBERSERVERINFO:
			GTH_MemberServerInfo();
			continue;
			

		case GSC_CHATMESSAGE_WHISPER:
			GTH_SendChatMessage_ServerWhisper(); 
			continue;
			

		case GSC_CALLGAMEMASTER:
			GTH_SendMessage_ServerCallGameMaster(); 
			continue;
			

		case GSC_REQUESTSUMMONS:
			GTH_SendMessage_ServerRequestSummons();
			continue;
			

		case GSC_REPLYSUMMONS:
			GTH_SendMessage_ServerReplySummons();
			continue;
			

		case GSC_SYSTEMADMIN:
			GTH_ProcessMessage_SystemAdmin();
			continue;
			

		
		case GSC_REQUEST_PARTYINFO :
			GTH_ProcessMessage_Request_PartyInfo();
			continue;
			

		
		case GSC_REPLY_PARTYINFO :
			GTH_ProcessMessage_Reply_PartyInfo();
			continue;
			

		case GSC_SERVERACKMSG:
			GTH_SendMessage_ServerAckMessage();
			continue;
			

			
		case GSC_REQUEST_OTHERSERVER :
			GTH_ProcessMessage_Request_OtherServer();
			continue;
			

		case GSC_REPLY_OTHERSERVER :
			GTH_ProcessMessage_Reply_OtherServer();
			continue;
			

		
		case GSC_GUILD_REPLY:
			GTH_ProcessMessage_Guild_Reply_From_ManagerServer();
			continue;
			
		
		case GSC_GUILD_REQUEST:
			GTH_ProcessMessage_Guild_Request_From_MemberServer();
			continue;
			

			
		case GSC_EXTEND :
			GTH_ProcessMessage_Server_Extend();
			continue;			



			
		
			
	
			

			
		case EXTEND_SECOND:
			GTH_ProcessMessage_Client_Extend_Second();
			continue;	
		}

		if( g_curPC == NULL ) continue;
		if( !g_curPC->ready ) continue;

		switch (command)
		{
			
		case CC_EVENT_IDLE:
			GTH_ProcessEventMessage_Idle();
			break;

		case CC_EVENT_WALK:
			GTH_ProcessEventMessage_Walk();
			break;

		case CC_EVENT_WALKLEFT:
			GTH_ProcessEventMessage_WalkLeft();
			break;

		case CC_EVENT_WALKRIGHT:
			GTH_ProcessEventMessage_WalkRight();
			break;

		case CC_EVENT_BACK:
			GTH_ProcessEventMessage_Back();
			break;

		case CC_EVENT_BACKLEFT:
			GTH_ProcessEventMessage_BackLeft();
			break;

		case CC_EVENT_BACKRIGHT:
			GTH_ProcessEventMessage_BackRight();
			break;

		case CC_EVENT_RUN:
			GTH_ProcessEventMessage_Run();
			break;

		case CC_EVENT_RUNLEFT:
			GTH_ProcessEventMessage_RunLeft();
			break;

		case CC_EVENT_RUNRIGHT:
			GTH_ProcessEventMessage_RunRight();
			break;

		case CC_EVENT_TURNLEFT:
			GTH_ProcessEventMessage_TurnLeft();
			break;

		case CC_EVENT_TURNRIGHT:
			GTH_ProcessEventMessage_TurnRight();
			break;

		case CC_EVENT_MOVELEFT:
			GTH_ProcessEventMessage_MoveLeft();
			break;

		case CC_EVENT_MOVERIGHT:
			GTH_ProcessEventMessage_MoveRight();
			break;

		case CC_EVENT_ATTACK:
			GTH_ProcessEventMessage_Attack();
			break;

		case CC_EVENT_RESPAWN:
			GTH_ProcessEventMessage_Respawn();
			break;

		case CC_EVENT_SIT :
			GTH_ProcessEventMessage_Sit();
			break;
		case CC_EVENT_STAND :
			GTH_ProcessEventMessage_Stand();
			break;

		case CC_EVENT_SKILL :

			
			g_PcSkill.GTH_ProcessEventMessage_Skill();
			break;

		case CC_CHATMESSAGE:
			GTH_SendChatMessage();
			break;

		case CC_CHATMESSAGE_WHISPER:
			GTH_SendChatMessage_Whisper();
			break;

		case CC_DROPITEM:
			GTH_ProcessMessage_DropItem();
			break;

		case CC_PICKUPITEM:
			GTH_ProcessMessage_PickUpItem();
			break;

		case CC_REQUEST_GETINVENTORYITEM:
			GTH_ProcessMessage_RequestGetInventoryItem();
			break;

		case CC_REQUEST_GETEQUIPITEM:
			GTH_ProcessMessage_RequestGetEquipItem();
			break;

		case CC_UPDATEPOSITION:
			GTH_ProcessMessage_UpdatePosition();
			break;

		case CC_REQUEST_NPCCHAT :
			GTH_ProcessMessage_RequestNpcChat();
			break;

		case CC_REQUEST_QUITNPCCHAT :
			GTH_ProcessMessage_RequestQuitNpcChat();
			break;

		case CC_REQUEST_NPCSELECTMESSAGE :
			GTH_ProcessMessage_RequestNpcSelectMessage();
			break;

		case CC_REQUEST_WEAPONCHANGE :
			GTH_ProcessMessage_WeaponChange();
			break;

		case CC_REQUEST_GENCHANGE :
			GTH_ProcessMessage_GenChange();
			break;

		case CC_REQUEST_STATUSUP :
			GTH_ProcessMessage_StatusUp();
			break;

		case CC_REQUEST_STORE :
			GTH_ProcessMessage_RequestStore();
			break;

		case CC_REQUEST_BUYITEM :
			GTH_ProcessMessage_RequestBuyItem();
			break;

		case CC_REQUEST_SELLITEM :
			GTH_ProcessMessage_RequestSellItem();
			break;

		case CC_REQUEST_CHEATCODE :
			GTH_ProcessMessage_CheatCode();
			break;

		case CC_REQUEST_BINDING :
			GTH_ProcessMessage_Binding();
			break;
		
		case CC_REQUEST_EPACKSTORE:
			GTH_ProcessMessage_EpackStore();
			break;
		
		case CC_REQUEST_EPACKUSE :
			GTH_ProcessMessage_EpackUse();
			break;

		case CC_REQUEST_REPAIR :
			GTH_ProcessMessage_RepairItem();
			break;

		case CC_REQUEST_DESTROY_ITEM :
			GTH_ProcessMessage_DestroyItem();
			break;

		case CC_EVENT_MOUSEWALK:
			GTH_ProcessEventMessage_MouseWalk();
			break;
			
		case CC_EVENT_MOUSERUN:
			GTH_ProcessEventMessage_MouseRun();
			break;

		case CC_CALLGAMEMASTER:
			GTH_SendMessage_CallGameMaster();
			break;

		case CC_REQUESTSUMMONS:
			GTH_SendMessage_RequestSummons();
			break;

		case CC_REPLYSUMMONS:
			GTH_ProcessMessage_ReplySummons( g_curPC );
			break;
		
		case CC_REQUEST_DELETE_QUESTSTATUS :
			GTH_ProcessMessage_DelQuestStatus( g_curPC );
			break;

		case CC_FRIENDS:
			GTH_ProcessMessage_Friends (g_curPC);
			break;


		case CC_SYSTEMADMIN:
			GTH_ProccessMessage_ValidationServerAdmin( g_curPC );
			break;

			





		case CC_REQUEST_PCTRADE :
			GTH_ProcessMessage_PCTrade();
			break;

		case CC_REQUEST_PARTY :								
			GTH_ProcessMessage_Party();
			break;

		case CC_REQUEST_SKILLLEVELUP :						
			
			g_PcSkill.GTH_ProcessMessage_Request_SkillLevelUp();
			break;

		case CC_REQUEST_LEARN_SKILL :
			
			g_PcSkill.GTH_ProcessMessage_Request_Learn_Skill();
			break;

		case CC_REQUEST_BUY_SKILL :
			GTH_ProcessMessage_Request_Buy_Skill();
			break;

		case CC_REQUEST_PCINFO :
			GTH_ProcessMessage_Request_PCInfo();
			break;

		case CC_HOTKEY :
			GTH_ProcessMessage_Hotkey();
			break;

		case CC_REQUEST_USE_PORTAL :
			GTH_ProcessMessage_Request_Use_Portal();
			break;


			
			
		case CC_REQUEST_USE_POTION :			
			
			g_SkillLogic.GTH_ProcessMessage_Request_Use_Potion();
			break;

		case CC_REQUEST_SAVE_PORTAL_POSITION :
			GTH_ProcessMessage_Request_SavePortalPosition();
			break;

		case CC_REQUEST_ITEMSYSTEM :
			GTH_ProcessMessage_ItemSystem();
			break;

			
		case CC_QUESTLOGIC :
			GTH_ProcessMessage_QuestLogic();
			break;





		
		case CC_TELEPORT:
			GTH_ProcessEventMessage_Teleport();

			
			receiveCount++;

			break;
		case CC_BEFORE_TELEPORT:
			GTH_ProcessEventMessage_Before_Teleport();
			break;

		case CC_EXTEND:
			GTH_ProcessMessage_Client_Extend();
			break;

		
		
			
		
		case CC_REQUEST_USE_MASIC_BOOK :
			GTH_ProcessMessage_Request_Use_Masic_Book();
			break;		
						

		
		case GC_RELIABLE_PING:
			GTH_ProcessMessage_PING();
			break;
		case GC_UNRELIABLE_PING:
			break;

		
		case CC_GUILD:
			GTH_ProcessMessage_Guild_From_Client();
			break;
	   
		case CC_REQUEST_DELETE_AUTOEFFECT :
			GTH_ProcessMessage_AutoQuestProgress();
			break;
		  
		case CC_REQUEST_CHATBAN_FLAG :

			break;
	   
		case CC_REQUEST_CHATBAN_START :

			break;
	   
		case CC_REQUEST_CHATBAN_CANCEL :

			break;
	   
		case CC_REQUEST_CHATBAN_GMCANCEL :

			break;
		
		case CC_REQUEST_CHAOVALUE :
			GTH_ProcessMessage_ChaosValue();
			break;	
		


		default :
			g_logSystem->Write("Protocol Mismatch.(ID:%s, Command:%x)", g_curPC->userID, command);
			
			break;
		}
	} 

	return 1;
}


void GTH_ProcessMessage_Server_Extend()
{
	byte command = MSG_ReadByte();

	switch(command)
	{
			
		case GSC_MAIL_REQUEST :
			GTH_ProcessMessage_Request_MailSystem();
			break;
		case GSC_MAIL_REPLY :
			GTH_ProcessMessage_Reply_MailSystem();
			break;

			
		case GSC_WORLDCHATTING_REQUEST :
			GTH_ProcessMessage_Request_WorldChatting();
			break;
		case GSC_WORLDCHATTING_REPLY :
			GTH_ProcessMessage_Reply_WorldChatting();
			break;
			
			
		case GSC_REPLY_CHRISTMAS_EVENT :
			g_ChristMasEvent.GTH_Process_ChristMas_Event();
			break;

		

	}
}





int GTH_ConnectGameServer()
{
	strncpy(g_curPC->userID, MSG_ReadString(),IDSTRING);
	g_curPC->userID[IDSTRING]=NULL;

	g_curPC->serverGroupID = MSG_ReadByte();
	g_curPC->characterID = MSG_ReadByte();
	g_logSystem->Write("User [%s] Request Connection.(%s)", g_curPC->userID, NET_AddrToString((struct sockaddr *)&g_curPC->sock.addr));

	
	g_DBAccountServer->InsertLogin( g_curPC );	
	g_curPC->guildIdx = -1;

	return 1;
}

int GTH_ReplyConnectGameServer(playerCharacter_t *pc)
{
	int j, k, equipNumber, itemIdx;

	pc->flagDBLoading = true;

	g_logSystem->Write("User [%s] Connected.(%s)", pc->userID, NET_AddrToString((struct sockaddr *)&pc->sock.addr));
	
	
	
	
	g_logSystem->WriteToLog( pc, LOG_CLASS_CONNECT, "%d;;;;;%s;", pc->flagChangeServer, NET_AddrToIPString((struct sockaddr *)&pc->sock.addr) );
	

	if ( pc->flagChangeServer >= 0 )	
	{
		pc->flagChangeServer = -1;
	}

	pc->angles[0] = 0;
	pc->angles[2] = 0;

	
	
	if (pc->position[0] < 0 || pc->position[1] < 0 || pc->position[2] < 0)
	{
		vec_copy(g_worldTable[pc->worldIdx].startPosition, pc->position);
		g_logSystem->Write( "Pos Revise : User [%s] Position Was Revised in World %d.", pc->name, g_worldTable[pc->worldIdx].name );
	}

	vec_copy( pc->position, pc->reliablePosition );

	if ( (int)pc->curRA <= 0 )
	{
		pc->curRA = 10.0f;
		pc->curSA = 10.0f;
	}

	
	pc->zoneIdx = GTH_Zone_GetZoneIdx(pc->position);
	GTH_Zone_AddEntityList(pc->worldIdx, pc->zoneIdx, ENTITY_PC, pc->idx);

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_CONNECTREP);
		MSG_WriteLong(&netMessage, g_globalTime);
		
		MSG_WriteShort(&netMessage, pc->idx); 
		MSG_WriteByte(&netMessage, pc->pcClass); 
		
		MSG_WriteByte(&netMessage, pc->skinPart[0]);
		MSG_WriteByte(&netMessage, pc->skinPart[1]);
		MSG_WriteByte(&netMessage, pc->skinPart[2]);
		MSG_WriteByte(&netMessage, pc->skinPart[3]);
		
		equipNumber = GetNumberEquipmentItem( pc );
		MSG_WriteByte(&netMessage, equipNumber); 
		
		for (k=0; k<EQUIP_RING; k++)
		{
			if( k == EQUIP_WEAPON_1 && pc->curWeapon == 1 ) continue;
			if( k == EQUIP_WEAPON_2 && pc->curWeapon == 0 ) continue;
			
			itemIdx = pc->equipment[k];
			if (itemIdx < 0) continue;
			
			if( !GTH_CheckRequire( pc, 0, &pc->item[itemIdx] ) ) continue;
			MSG_WriteShort(&netMessage, pc->item[itemIdx].itemTableIdx );
		}
		
		MSG_WriteByte(&netMessage, pc->worldIdx);
		
		MSG_WritePosition(&netMessage, pc->position); 
		MSG_WriteFloat(&netMessage, pc->angles[1]); 
		MSG_WriteByte(&netMessage, pc->event); 
		MSG_WriteLong(&netMessage, (int)pc->curRA);
		MSG_WriteShort(&netMessage, (int)pc->curSA);
		MSG_WriteLong(&netMessage, (int)pc->calMaxRA);
		MSG_WriteShort(&netMessage, (int)pc->calMaxSA);
		
		MSG_WriteFloat(&netMessage, pc->calVelocity );
		MSG_WriteString(&netMessage, pc->name); 
		MSG_WriteByte(&netMessage,pc->enumNameColor);
		MSG_WriteShort(&netMessage, pc->guildIdx );
		if(pc->GM.Chat.RemainBanTimePerSecond <= 0) MSG_WriteByte(&netMessage,TRUE);
		else MSG_WriteByte(&netMessage,FALSE);		
		MSG_WriteLong(&netMessage,gcpGMCtrl->isPK());		

		
		MSG_WriteByte(&netMessage, pc->tHelper.tPoint.iMyPoint);
		
		
		
		
		
		
		
		MSG_WriteByte( &netMessage, pc->itemNumber );
		for( j = 0; j < MAX_INVENTORY_SIZE; j ++ )
			MSG_WriteChar( &netMessage, pc->inventory[j] );
		
		for( j = 0; j < MAX_DEPOT_SIZE; j ++ )
			MSG_WriteChar( &netMessage, pc->depot[j] );
		
		for( j = 0; j < MAX_EQUIPMENT; j ++ )
			MSG_WriteChar( &netMessage, pc->equipment[j] );
		
		MSG_WriteChar( &netMessage, pc->mouseInventory );
		
		for( j = 0; j < MAX_PRECOCITY_SIZE; j ++ )
		{
			MSG_WriteChar( &netMessage, pc->precocityInventory[j] );
			MSG_WriteLong( &netMessage, pc->precocityTime[j] );
		}
		
		item_t *item = &pc->item[0];

		for( j = 0; j < MAX_NUMBER_OF_OWNITEM; j ++, item ++ )
		{
			if( item->itemTableIdx < 0 ) continue;
			
			MSG_WriteByte( &netMessage, j );
			MSG_WriteString( &netMessage, item->name );
			MSG_WriteShort( &netMessage, item->itemTableIdx );
			MSG_WriteByte( &netMessage, item->optionNumber );
			
			for( k = 0; k < item->optionNumber; k ++ )
				MSG_WriteShort( &netMessage, item->optionIdx[k] );
			
			for( k = 0; k < item->optionNumber; k ++ )
				MSG_WriteShort( &netMessage, item->optionValue[k] );
			
			MSG_WriteShort( &netMessage, item->durability );
			MSG_WriteByte( &netMessage, item->reqLevel );
			MSG_WriteLong( &netMessage, item->experience );
			MSG_WriteLong( &netMessage, item->timer );

			
			
			MSG_WriteLong(&netMessage, item->ItemExtendInfo.UseFlag);
			MSG_WriteLong(&netMessage, (int)item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType);
			MSG_WriteString(&netMessage, item->ItemExtendInfo.ItemSpawnInfomation.data);			
			
			
			
			
			
			if (IsItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_TIME) )
			{
				if ( item->ItemExtendInfo.AllowUseTime < 0 )
					item->ItemExtendInfo.AllowUseTime = 0;
			}
			
			MSG_WriteLong(&netMessage, item->ItemExtendInfo.AllowUseTime);
			
			if (IsItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_DATE) )
			{		
				item->ItemExtendInfo.CalclateUseDateToSec = difftime(item->ItemExtendInfo.AllowUseTime , time(NULL));
				if ( item->ItemExtendInfo.CalclateUseDateToSec < 0)
					item->ItemExtendInfo.CalclateUseDateToSec = 0;
			}
			
			MSG_WriteLong(&netMessage, item->ItemExtendInfo.CalclateUseDateToSec);
			


		}
		
		
		
		MSG_WriteLong(&netMessage,pc->GM.Chat.RemainBanTimePerSecond);

		
		MSG_WriteLong( &netMessage, gGameConfigFlag.GetFlag());

		
		if(pc->PremiumInfo.iMemberShipType == playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free)
		{
			MSG_WriteByte( &netMessage,0);
		}
		else
		{
			MSG_WriteByte( &netMessage,1);
		}
		
		
		if(gGameConfigFlag.m_bWorld_Chat_Limit)
		{
			MSG_WriteLong(&netMessage,	pc->PremiumInfo.WorldChatting.lDate);
			MSG_WriteLong(&netMessage,	pc->PremiumInfo.WorldChatting.iMaxCount);
			MSG_WriteLong(&netMessage,	pc->PremiumInfo.WorldChatting.iDecreaseCount);
		}
		

		
		if(gGameConfigFlag.m_bPrecocity_Time)
		{
			MSG_WriteFloat(&netMessage,	pc->PremiumInfo.fItemPrecocityTimeRatio);
		}
		//lucky 2012 NEW GM system
		MSG_WriteLong(&netMessage, pc->gameMaster);
		//end
		
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	pc->event = GTH_EV_CHAR_IDLE;

	
	if( pc->gameMaster ) 
	{
		pc->gmHide = true;
	}

	
	
	pc->sendReplyConnect = true;
	
	return 1;
}





void GTH_DisconnectPlayer(playerCharacter_t* pPlayer,const BOOL bForceQuit) 
{	
	if(g_config.isDanBattleMap){

		if(pPlayer->worldIdx == DAN_BATTLEMAP_NO){
			
			g_pDanBattleSys->DeleteTeam(pPlayer);			
			pPlayer->worldIdx = g_binding[pPlayer->bindingIdx].worldIdx;
			vec_copy(g_binding[pPlayer->bindingIdx].pos, pPlayer->position);			
		}
	}		
	
	GTH_ForcedQuit(pPlayer);	
	GTH_PartyInfo_Request_Update(pPlayer, false );
	GTH_RecoveryReliablePosition(pPlayer);	
	pPlayer->normalNetwork = false;

	DB_SaveGameData(pPlayer,SAVEGAMEDATA_QUIT,unknown,bForceQuit);
	if(TRUE == bForceQuit){
		g_logSystem->Write("User Name =%s was ForceQuit by GM.", pPlayer->name);
	}else{
		g_logSystem->Write("User [%s] was Normaly Closed.", pPlayer->userID);
	}
	g_logSystem->WriteToLog(
		pPlayer, 
		LOG_CLASS_DISCONNECT, 
		"%d;;;;;%s;",
		0, 
		NET_AddrToIPString((struct sockaddr *)&pPlayer->sock.addr));

}


int GTH_DisconnectGameServer() 
{
	BOOL bForceQuit_fromGM = MSG_ReadByte();
	GTH_DisconnectPlayer(g_curPC,bForceQuit_fromGM);	
	return 1;
}









int GTH_DisconnectGameServer (int idx)
{
	playerCharacter_t * pc = &g_pc[idx] ;

	
	if ( !pc->ready || !pc->active ) return 0 ;

	
	if ( g_config.isDanBattleMap )														
	{
		if ( g_curPC->worldIdx == DAN_BATTLEMAP_NO)
		{
			g_logSystem->WriteRLog("Delete Team....Call By GTH_DisconnectGameServer");
			g_pDanBattleSys->DeleteTeam( g_curPC );			

			
			g_curPC->worldIdx = g_binding[g_curPC->bindingIdx].worldIdx;
			vec_copy(g_binding[g_curPC->bindingIdx].pos, g_curPC->position);
			
			
		}
	}
	
	

	GTH_ForcedQuit( pc );
	GTH_PartyInfo_Request_Update( pc, false );
	GTH_RecoveryReliablePosition( pc );
	pc->normalNetwork = false;
	DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);

	g_logSystem->Write("User [%s] was Normaly Closed.", pc->userID);
	g_logSystem->WriteToLog( g_curPC, LOG_CLASS_DISCONNECT, ";0;;;;%s;", NET_AddrToIPString((struct sockaddr *)&pc->sock.addr) );

	return 1;
}

int GTH_SendSyncMessage()
{
	int i, j, k;
	int idx, itemIdx;
	int equipNumber;

	
	int affectData[MAX_AFFECT], numAffect;

	for (i=0; i < MAX_PCS; i++)
	{
		if ( !g_pc[i].active || !g_pc[i].ready ) continue;

		
		if (g_globalTime >= g_pc[i].syncTime)  
		{


			if(TRUE == g_pc[i].GonryunBattle.Observer.bActive){
				GTH_GetEntityListInValidRange(g_pc[i].worldIdx,CGolryunBattle_Observer::m_position);
			}else{
			
			GTH_GetEntityListInValidRange(g_pc[i].worldIdx, g_pc[i].position);
			}


			if (g_zoneEntityListNumber < 1) 
			{
				g_logSystem->Write("Can Not Found User from Zone.(name: %s, zoneNo: %d, pos(%f, %f, %f))", g_pc[i].name, g_pc[i].zoneIdx, g_pc[i].position[0], g_pc[i].position[1], g_pc[i].position[2]);
				GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_pc[i].idx, g_pc[i].worldIdx, g_pc[i].zoneIdx, g_pc[i].position);
				continue;
			}

			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_SYNCMESSAGE);
				MSG_WriteShort(&netMessage, g_pc[i].sock.latencyTime); 
				MSG_WriteByte(&netMessage, g_zonePCListNumber);
				MSG_WriteByte(&netMessage, g_zoneMonsterListNumber);
				MSG_WriteByte(&netMessage, g_zoneNPCListNumber);
				
				for (j=0; j < g_zonePCListNumber; j++)
				{
					idx = g_zonePCList[j].idx;
					
					MSG_WriteShort(&netMessage, idx); 
					MSG_WriteByte(&netMessage, g_pc[idx].pcClass);		
					
					MSG_WriteByte(&netMessage, g_pc[idx].pcJob );		
					
					
					if( g_pc[idx].gameMaster ) MSG_WriteByte(&netMessage, g_pc[idx].gmHide);
					else MSG_WriteByte(&netMessage, g_pc[idx].level );
					
					
					
					MSG_WriteByte(&netMessage, g_pc[idx].isTransform );
					
					if( !g_pc[idx].isTransform )
					{
						MSG_WriteByte(&netMessage, g_pc[idx].skinPart[0]);
						MSG_WriteByte(&netMessage, g_pc[idx].skinPart[1]);
						MSG_WriteByte(&netMessage, g_pc[idx].skinPart[2]);
						MSG_WriteByte(&netMessage, g_pc[idx].skinPart[3]);
						
						equipNumber = GetNumberEquipmentItem( &g_pc[idx] );
						MSG_WriteByte(&netMessage, equipNumber); 
						
						
						for (k=0; k<EQUIP_RING; k++)
						{
							if( k == EQUIP_WEAPON_1 && g_pc[idx].curWeapon == 1 ) continue;
							if( k == EQUIP_WEAPON_2 && g_pc[idx].curWeapon == 0 ) continue;
							
							itemIdx = g_pc[idx].equipment[k];
							if (itemIdx < 0) continue;
							
							if( !GTH_CheckRequire( &g_pc[idx], 0, &g_pc[idx].item[itemIdx] ) ) continue;
							MSG_WriteShort(&netMessage, g_pc[idx].item[itemIdx].itemTableIdx );
						}
					}
					else
					{
						MSG_WriteShort(&netMessage, g_pc[idx].transformEntityIdx );
						MSG_WriteByte(&netMessage, g_pc[idx].transformType );
					}
					
					MSG_WritePosition(&netMessage, g_pc[idx].position); 
					MSG_WriteFloat(&netMessage, g_pc[idx].angles[1]); 
					MSG_WriteByte(&netMessage, g_pc[idx].event); 
					MSG_WriteLong(&netMessage, (int)g_pc[idx].curRA);
					MSG_WriteShort(&netMessage, (int)g_pc[idx].curSA);
					MSG_WriteLong(&netMessage, (int)g_pc[idx].calMaxRA);
					MSG_WriteShort(&netMessage, (int)g_pc[idx].calMaxSA);
					MSG_WriteFloat(&netMessage, g_pc[idx].calVelocity );
					
					if( g_pc[idx].busyState == BUSY_STATE_BOOTH_SELL_PREPARE )
					{
						MSG_WriteByte(&netMessage, 1);
					}
					else if( g_pc[idx].busyState == BUSY_STATE_BOOTH_SELL )
					{
						MSG_WriteByte(&netMessage, 2);
						MSG_WriteString(&netMessage, g_pc[idx].boothName); 
					}
					else
					{
						MSG_WriteByte(&netMessage, 0);
					}
					MSG_WriteString(&netMessage, g_pc[idx].name); 
					MSG_WriteByte(&netMessage,g_pc[idx].enumNameColor);
					MSG_WriteByte(&netMessage, g_pc[idx].GonryunBattle.m_bWaitingRoom);
					MSG_WriteShort(&netMessage, g_pc[idx].GonryunBattle.m_iJoinNum);
					
					numAffect = GTH_MakeAffectTableIdx( &g_pc[idx], affectData, ENTITY_PC );
					
					MSG_WriteByte(&netMessage, numAffect);
					for( k = 0; k < numAffect; k ++ ) 
					{
						MSG_WriteShort(&netMessage, affectData[k] );
						
						
					}

					MSG_WriteByte(&netMessage, g_pc[idx].generalSpeed );
					
					
					
					MSG_WriteShort(&netMessage, g_pc[idx].chaosPoint );		
					MSG_WriteByte(&netMessage, g_pc[idx].pvpMode );			
					
					
					
					
					MSG_WriteShort(&netMessage, g_pc[idx].guildIdx );
					MSG_WriteString(&netMessage, g_pc[idx].guildName );

					
					MSG_WriteByte(&netMessage, g_pc[idx].m_bHide );

					
					MSG_WriteShort(&netMessage, g_pc[idx].DanBattleInfo.nDanBattleIdx);
					//lucky 2012 NEW GM system
					MSG_WriteLong(&netMessage, g_pc[idx].gameMaster);
					//end


				} 
				
				for (j=0; j< g_zoneMonsterListNumber; j++)
				{
					idx = g_zoneMonsterList[j].idx;
					
					MSG_WriteShort(&netMessage, idx);
					MSG_WriteByte(&netMessage, g_monster[idx].tableIdx );
					MSG_WriteByte(&netMessage, g_monster[idx].aliveState );
					MSG_WritePosition(&netMessage, g_monster[idx].position);
					MSG_WriteFloat(&netMessage, g_monster[idx].angles[1]);
					MSG_WriteByte(&netMessage, g_monster[idx].event);
					MSG_WriteLong(&netMessage, (int)g_monster[idx].curRA);
					MSG_WriteShort(&netMessage, (int)g_monster[idx].curSA);
					MSG_WriteLong(&netMessage, (int)g_monster[idx].calMaxRA);
					MSG_WriteShort(&netMessage, (int)g_monster[idx].calMaxSA);
					MSG_WriteFloat(&netMessage, g_monster[idx].calVelocity );
					
					MSG_WriteFloat(&netMessage, g_monster[idx].summonValidTime );
					
					if( (int)g_monster[idx].summonValidTime > 0 )
					{

					}
					
					
					
					numAffect = GTH_MakeAffectTableIdx( &g_monster[idx], affectData, g_monster[idx].entityType );
					MSG_WriteByte(&netMessage, numAffect);
					for( k = 0; k < numAffect; k ++ ) MSG_WriteShort(&netMessage, affectData[k] );
					
					
					MSG_WriteByte(&netMessage, g_monster[idx].generalSpeed );
					
					
					MSG_WriteByte(&netMessage, g_monster[idx].specialMonster );
				}
				
				
				
				for (j=0; j < g_zoneNPCListNumber; j++)
				{
					idx = g_zoneNPCList[j].idx;				
					
					MSG_WriteShort(&netMessage, idx);
					
					MSG_WriteByte(&netMessage, g_monster[idx].tableIdx );
					MSG_WriteByte(&netMessage, g_monster[idx].aliveState );
					
					MSG_WriteShort(&netMessage, g_monster[idx].npc.scriptFlag );
					
					MSG_WritePosition(&netMessage, g_monster[idx].position);
					MSG_WriteFloat(&netMessage, g_monster[idx].angles[1]);
					MSG_WriteByte(&netMessage, g_monster[idx].event);
					MSG_WriteLong(&netMessage, (int)g_monster[idx].curRA);
					MSG_WriteShort(&netMessage, (int)g_monster[idx].curSA);
					MSG_WriteLong(&netMessage, (int)g_monster[idx].calMaxRA);
					MSG_WriteShort(&netMessage, (int)g_monster[idx].calMaxSA);
					
					MSG_WriteFloat(&netMessage, g_monster[idx].calVelocity );
					
					MSG_WriteByte(&netMessage, g_monster[idx].generalSpeed );
				}
				
				if( g_pc[i].organizeServer >= 0 )
				{
					int curRA, curSA, maxRA, maxSA;
					
					
					MSG_WriteByte(&netMessage, 1);
					
					for( j = 0; j < MAX_NUMBER_OF_PARTY_MEMBER; j ++ )
					{
						if( g_pc[i].member[j].serverIndex < 0 ) 
						{
							MSG_WriteByte(&netMessage, 0 );
							continue;	
						}
						MSG_WriteByte(&netMessage, 1 );
						MSG_WriteByte( &netMessage, g_pc[i].member[j].idx );
						
						curRA = 0;
						curSA = 0;
						maxRA = 1;
						maxSA = 1;
						
						
						if( !g_pc[i].member[j].isActive )
						{
							MSG_WriteByte( &netMessage, 0 );
						}
						else if( g_pc[i].member[j].serverIndex != g_config.gameServerNo )	
						{
							MSG_WriteByte( &netMessage, 1 );
						}
						else if( g_pc[i].member[j].idxOnServer >= 0 )
						{
							idx = g_pc[i].member[j].idxOnServer;
							
							if( !stricmp( g_pc[i].member[j].memberName, g_pc[idx].name ) && g_pc[idx].active )
							{
								
							
								if(g_pc[i].worldIdx != g_pc[i].member[j].worldIdx)
								{
									MSG_WriteByte( &netMessage, 0 );
								}
															
								else
								{
									MSG_WriteByte( &netMessage, 1 );
									curRA = (int)g_pc[idx].curRA;
									curSA = (int)g_pc[idx].curSA;
									maxRA = (int)g_pc[idx].calMaxRA;
									maxSA = (int)g_pc[idx].calMaxSA;
								}
								
							}
							else
							{
								MSG_WriteByte( &netMessage, 0 );
							}
						}
						else
						{
							MSG_WriteByte( &netMessage, 0 );
						}
						MSG_WriteLong( &netMessage, curRA );
						MSG_WriteLong( &netMessage, maxRA );
						MSG_WriteShort( &netMessage, curSA );
						MSG_WriteShort( &netMessage, maxSA );
					}
				}
				else
				{
					
					MSG_WriteByte(&netMessage, 0);
				}
				
				
				for( j = 0; j < MAX_PRECOCITY_SIZE; j ++ )
				{
					MSG_WriteLong( &netMessage, g_pc[i].precocityTime[j] );
				}

				
				MSG_WriteLong( &netMessage, gGameConfigFlag.GetFlag());

				NET_SendMessage(&g_pc[i].sock, &netMessage);  
			}
			MSG_EndWriting(&netMessage);

			g_pc[i].syncTime += g_config.gameSyncTime; 

			
			{
				
				g_pc[i].age ++;
				
				GTH_PrecocityProcess( &g_pc[i] );
				
				GTH_UpdateTrainMonster( &g_pc[i] );
				
				
				DecChaosPoint( &g_pc[i] );

				
				gcpGMCtrl->ProcessPerSecond(&g_pc[i]);

				
				GTH_ItemUseTimeProcess( &g_pc[i]);

				
				






				
			
			}

		}  

		
		if ( g_globalTime > g_pc[i].nextSaveTime )
		{
			
			if( g_pc[i].busyState == BUSY_STATE_NONE && g_pc[i].normalNetwork ) 
			{
				g_pc[i].flagChangeServer = -2;	
				DB_SaveGameData( &g_pc[i], SAVEGAMEDATA_CONTINUE);				

			}
			else
			{
				g_pc[i].flagChangeServer = -1;
			}
		}
		
		
		if( !GTH_CheckServerAckMessage( &g_pc[i] ) )
		{
			GTH_SendMessage_NotFoundPC( &g_pc[i] );
		}

		
		if( !GTH_CheckServerPartyAckMessage( &g_pc[i] ) )
		{
			GTH_SendMessage_Party_Error( &g_pc[i] );
		}

		
		if( g_pc[i].partyInfoRequestTime < g_timer->GetAppMilliTime() )
		{
			g_pc[i].partyInfoRequestTime = g_timer->GetAppMilliTime() + PARTY_INFO_REQUESTTIME;

			GTH_PartyInfo_Request_Update( &g_pc[i], true );
			GTH_PartyInfo_Request_SendAll( &g_pc[i] );
		}

	
	} 

	return 1;
}




void GTH_SendChatMessage()
{
	int i, idx, type;
	char chatMessage[CHATSTRING];

	type = MSG_ReadByte();
	sstrncpy(chatMessage, MSG_ReadString(), CHATSTRING);

	
	GTH_GetPCListInValidRange(g_curPC->worldIdx, g_curPC->position);

	if (g_zonePCListNumber < 1) 
	{
		g_logSystem->Write("Can Not Found User from Zone.. in GTH_SendChatMessage");
		return;
	}

	
	
	if( type == CHATMSG_TYPE_DAN )
	{
		if( g_curPC->guildIdx != -1 )
		{
			if( g_config.isManager )
			{
				GTH_GuildChatMessage( g_curPC->guildIdx, g_curPC->name, chatMessage );
			}
			else
			{
				GTH_SendMessage_Guild_ChatMessage_Request_To_ManagerServer( g_curPC->guildIdx, g_curPC->name, chatMessage );
			}
		}
		return;
	}

	
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_CHATMESSAGE);
		MSG_WriteByte(&netMessage, type);
		MSG_WriteString(&netMessage, g_curPC->name);
		MSG_WriteString(&netMessage, chatMessage);
		
		for (i=0; i < g_zonePCListNumber; i++)
		{
			idx = g_zonePCList[i].idx;

			
			if (g_curPC->idx == idx) continue;
			
			if ( !g_pc[idx].ready ) continue;

			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		} 
	}


	MSG_EndWriting(&netMessage);

	
	if( !stricmp( chatMessage, "@ÀÏ¹Ý¸ðµå" ) ) g_curPC->targetingType = TARGETING_NORMAL;
	else if( !stricmp( chatMessage, "@¾ÈÀü¸ðµå" ) ) g_curPC->targetingType = TARGETING_SAFE;
	else if( !stricmp( chatMessage, "@¹«Â÷º°¸ðµå" ) ) g_curPC->targetingType = TARGETING_EVERY;
	else if( !stricmp( chatMessage, "@¸ðµå" ) ) g_curPC->targetingType = TARGETING_ONLY_MONSTER;
}





void GTH_Send_Subscribe_Request_Message_To_Client( char *toName )
{
	int idx = GTH_FindPCByName(toName);

	
	if (idx >= 0)
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
			MSG_WriteByte(&netMessage, GUILD_SUBSCRIBE_REQUEST);
			MSG_WriteString(&netMessage, g_curPC->name);	
			
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	
	else
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
			MSG_WriteByte(&netMessage, GUILD_SUBSCRIBE_REQUEST);
			MSG_WriteString(&netMessage, toName);	        
			MSG_WriteString(&netMessage, g_curPC->name);	
			
			
			
			for (int i=1; i < MAX_MEMBER_SERVER; i++)
			{
				if ( !g_memberServer[i].active ) continue;
				
				NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);
	}
}

void GTH_Send_Subscribe_Request_Message_To_Server( char *toName, char *fromName, int fromMemberIdx )
{
	int idx = GTH_FindPCByName( toName );

	
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
	
	else
	{
		if ( g_config.isManager )
		{			
			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
				MSG_WriteByte(&netMessage, GUILD_SUBSCRIBE_REQUEST);
				MSG_WriteString(&netMessage, toName);	
				MSG_WriteString(&netMessage, fromName);	
				
				
				for (int i=1; i < MAX_MEMBER_SERVER; i++)
				{
					if ( !g_memberServer[i].active ) continue;

					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}

void GTH_SendMessage_SetShutdownTimer( playerCharacter_t *pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SETSHUTDOWNTIMER);
		MSG_WriteShort(&netMessage, g_serverShutdownTimer.setSecond ); 
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_NoticeMessage( playerCharacter_t *pc, char *notice )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_NOTICEMESSAGE);
		MSG_WriteString(&netMessage, notice );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ReliablePing( i3socket_t *sock )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GC_RELIABLE_PING);
		NET_SendMessage(sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_UnreliablePing( i3socket_t *sock )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GC_UNRELIABLE_PING);
		NET_SendUnreliableMessage(sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

int GTH_FindPCByName(char *name)
{
	int pcIdx=-1;
	for(pcIdx=0; pcIdx< MAX_PCS; pcIdx++){
		if ( !g_pc[pcIdx].active ) continue;
		if ( 0 == stricmp(name, g_pc[pcIdx].name))  return pcIdx;
	}
	return -1;
}

int GTH_FindGameMaster()
{
	int i;

	for (i=0; i<MAX_PCS; i++)
	{
		if ( !g_pc[i].active ) continue;
		if ( g_pc[i].gmBusy ) continue;
		if ( g_pc[i].gameMaster > 0) 
			return i;
	}

	return -1;
}


int GTH_FindGameMasterWithCondition(int worldIdx)
{
	int i;
	int ret;

	ret = -1;
	for (i=0; i<MAX_PCS; i++)
	{
		if ( !g_pc[i].active ) continue;
		if ( g_pc[i].gameMaster > 0) 
		{
			if ( g_pc[i].worldIdx == worldIdx)
			{
				ret = i;
				if ( !g_pc[i].gmBusy )
					break;
			}
			else
			{
				if ( ret < 0)	ret = i;
				continue;
			}
		}
	}

	return ret;
}

int GetNumberEquipmentItem( playerCharacter_t* pc )
{
	int i;
	int count = 0, itemIdx;

	for (i=0; i< EQUIP_RING; i++ )
	{
		if( pc->equipment[i] < 0 ) continue;

		if( i == EQUIP_WEAPON_1 && pc->curWeapon == 1 ) continue;
		if( i == EQUIP_WEAPON_2 && pc->curWeapon == 0 ) continue;
		itemIdx = pc->equipment[i];
		
		if( !GTH_CheckRequire( pc, 0, &pc->item[itemIdx] ) ) continue;

		count++;
	}

	return count;
}





void MSG_WritePosition(i3sizebuf_t *sb, vec3_t pos)
{
	
	MSG_WriteShort(sb, pos[0]);
	MSG_WriteShort(sb, pos[1]);
	MSG_WriteShort(sb, pos[2]);
}

void MSG_ReadPosition(vec3_t v)
{
	v[0] = (float)MSG_ReadShort();
	v[1] = (float)MSG_ReadShort();
	v[2] = (float)MSG_ReadShort();
}

void MSG_WriteItem(i3sizebuf_t *sb, item_t *item)
{
	int i;

	MSG_WriteString(sb, item->name);
	MSG_WriteShort(sb, item->itemTableIdx);
	MSG_WriteByte(sb, item->optionNumber);

	for (i=0; i<item->optionNumber; i++)
		MSG_WriteShort(sb, item->optionIdx[i]);

	
	
	for (i=0; i<item->optionNumber; i++)
		MSG_WriteShort(sb, item->optionValue[i]);

	MSG_WriteShort(sb, item->durability);
	MSG_WriteByte(sb, item->reqLevel);
	MSG_WriteLong(sb, item->experience);
	MSG_WriteLong(sb, item->timer);

	
	MSG_WriteLong(sb, item->ItemExtendInfo.UseFlag);
	MSG_WriteLong(sb, (int)item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType);
	MSG_WriteString(sb, item->ItemExtendInfo.ItemSpawnInfomation.data);
	

	
	
	
	
	
	if (IsItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_TIME) )
	{
		if ( item->ItemExtendInfo.AllowUseTime < 0 )
			item->ItemExtendInfo.AllowUseTime = 0;
	}

	MSG_WriteLong(sb, item->ItemExtendInfo.AllowUseTime);

	if (IsItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_DATE) )
	{		
		item->ItemExtendInfo.CalclateUseDateToSec = difftime(item->ItemExtendInfo.AllowUseTime , time(NULL));
		if ( item->ItemExtendInfo.CalclateUseDateToSec < 0)
			item->ItemExtendInfo.CalclateUseDateToSec = 0;
	}

	MSG_WriteLong(sb, item->ItemExtendInfo.CalclateUseDateToSec);

	
	
	
	
}

void MSG_ReadItem(item_t *item)
{
	int i;

	strcpy(item->name, MSG_ReadString());
	item->itemTableIdx = MSG_ReadShort();
	item->optionNumber = MSG_ReadByte();

	for (i=0; i<item->optionNumber; i++)
		item->optionIdx[i] = MSG_ReadShort();

	
	
	for (i=0; i<item->optionNumber; i++)
		item->optionValue[i] = MSG_ReadShort();

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

void MSG_WriteMember(i3sizebuf_t *sb, partyMember_t *m)
{
	MSG_WriteByte(sb, m->idx);

	MSG_WriteString(sb, m->memberName);
	MSG_WriteByte(sb, m->isActive);
	MSG_WriteByte(sb, m->isLeader);

	MSG_WriteByte(sb, m->level);
	MSG_WriteByte(sb, m->gen);
	MSG_WriteChar(sb, m->serverIndex);
	MSG_WriteShort(sb, m->idxOnServer);
	MSG_WriteShort(sb, m->worldIdx);

}

void MSG_ReadMember( partyMember_t *m )
{
	m->idx = MSG_ReadByte();

	strcpy( m->memberName, MSG_ReadString() );
	m->isActive = MSG_ReadByte();
	m->isLeader = MSG_ReadByte();

	m->level = MSG_ReadByte();
	m->gen = MSG_ReadByte();
	m->serverIndex = MSG_ReadChar();
	m->idxOnServer = MSG_ReadShort();
	m->worldIdx = MSG_ReadShort();
}

void MSG_ReadAffect( affect_t *aff )
{
}

void MSG_WriteAffect( i3sizebuf_t *sb, affect_t *aff )
{
}



void MSG_ReadPCList( entityPCList_t *pl )
{


	pl->serverIdx = MSG_ReadByte ();
	pl->idxOnServer = MSG_ReadShort ();
	pl->characterID = MSG_ReadByte ();
	pl->serverGroupID = MSG_ReadByte ();
	pl->worldIdx = MSG_ReadByte ();
	pl->level = MSG_ReadByte ();
	pl->job = MSG_ReadByte ();

	strncpy( pl->userID, MSG_ReadString (),NAMESTRING ); pl->userID[NAMESTRING-1]=NULL;
	strncpy( pl->name, MSG_ReadString (),NAMESTRING ); pl->name[NAMESTRING-1]=NULL;
	



	






}

void MSG_WritePCList( i3sizebuf_t *sb, entityPCList_t *pl )
{
	MSG_WriteByte ( sb, pl->serverIdx );
	MSG_WriteShort( sb, pl->idxOnServer );
	MSG_WriteByte ( sb, pl->characterID );
	MSG_WriteByte ( sb, pl->serverGroupID );
	MSG_WriteByte ( sb, pl->worldIdx );
	MSG_WriteByte ( sb, pl->level );
	MSG_WriteByte ( sb, pl->job );

	MSG_WriteString( sb, pl->userID );
	MSG_WriteString( sb, pl->name );

	





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


void GTH_SendMessage_CallGameMaster()
{
	int i, idx;
	char reason[256];

	
	
	strcpy( reason, MSG_ReadString() );

	
	idx = GTH_FindGameMasterWithCondition(g_curPC->worldIdx);
	if (idx >= 0) 
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_CALLGAMEMASTER);
			MSG_WriteString(&netMessage, g_curPC->name);	
			MSG_WriteString(&netMessage, reason);	
			MSG_WriteShort(&netMessage, g_curPC->worldIdx);	
			NET_SendMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_CALLGAMEMASTER);
			MSG_WriteString(&netMessage, g_curPC->name);	
			MSG_WriteString(&netMessage, reason);	
			MSG_WriteShort(&netMessage, g_curPC->worldIdx);	
			
			if ( g_config.isManager )
			{
				
				for (i=1; i < MAX_MEMBER_SERVER; i++)	
				{
					if ( !g_memberServer[i].active ) continue;
					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			else
			{
				
				MSG_WriteByte(&netMessage, g_config.gameServerNo);
				
				NET_SendUnreliableMessage(&localSocket, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);
	}
}

void GTH_SendMessage_ServerCallGameMaster()
{
	int i, idx;
	char fromName[NAMESTRING], reason[256];
	int fromMemberIdx, worldIdx;

	
	

	strcpy(fromName, MSG_ReadString());
	strcpy(reason, MSG_ReadString());
	worldIdx = MSG_ReadShort();

	idx = GTH_FindGameMaster();
	if (idx >= 0) 
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_CALLGAMEMASTER);
			MSG_WriteString(&netMessage, fromName);	
			MSG_WriteString(&netMessage, reason);
			MSG_WriteShort(&netMessage, worldIdx);
			
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		if ( g_config.isManager )
		{	
			fromMemberIdx = MSG_ReadByte(); 
			
			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_CALLGAMEMASTER);
				MSG_WriteString(&netMessage, fromName);	
				MSG_WriteString(&netMessage, reason);
				MSG_WriteShort(&netMessage, worldIdx);
				
				
				for (i=1; i < MAX_MEMBER_SERVER; i++)	
				{
					if ( !g_memberServer[i].active ) continue;
					if ( i == fromMemberIdx ) continue;	
					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}


void GTH_PC_Summon(playerCharacter_t* pFromPlayer,char* toName,int forcedYn)
{
	playerCharacter_t*  pTargetPlayer = gcpTools->GetPlayerRecordPointer(toName);

	
	if (NULL != pTargetPlayer) 
	{
		if(FALSE == gcpTools->isSummonAllow(pTargetPlayer)) return;

		GTH_PC_SetSummonsInfo(pTargetPlayer, pFromPlayer->name, pFromPlayer->worldIdx, pFromPlayer->position, forcedYn);

		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_REQUESTSUMMONS);
			MSG_WriteString(&netMessage, pFromPlayer->name);	
			MSG_WriteByte(&netMessage, forcedYn);			
			NET_SendMessage(&pTargetPlayer->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{	
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage,	GSC_REQUESTSUMMONS);
			MSG_WriteString(&netMessage,	toName); 
			MSG_WriteString(&netMessage,	pFromPlayer->name);	
			MSG_WriteByte(&netMessage,		pFromPlayer->worldIdx);	
			MSG_WritePosition(&netMessage,	pFromPlayer->position); 
			MSG_WriteByte(&netMessage,		forcedYn);			
			
			if ( g_config.isManager )
			{
				
				for (int i=1; i < MAX_MEMBER_SERVER; i++)	
				{
					if ( !g_memberServer[i].active ) continue;
					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			else
			{
				
				MSG_WriteByte(&netMessage, g_config.gameServerNo);
				
				NET_SendUnreliableMessage(&localSocket, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);
	}
}


void GTH_PC_Summon(int targetWorldIdx, vec3_t targetPosition,  char* TargetName, char* requestName,int forcedYn)
{
	playerCharacter_t*  pTargetPlayer = gcpTools->GetPlayerRecordPointer(TargetName);

	
	if (NULL != pTargetPlayer) 
	{
		if(FALSE == gcpTools->isSummonAllow(pTargetPlayer)) return;

		GTH_PC_SetSummonsInfo(pTargetPlayer, TargetName, targetWorldIdx, targetPosition, forcedYn);

		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_REQUESTSUMMONS);
			MSG_WriteString(&netMessage, TargetName);			
			MSG_WriteByte(&netMessage, forcedYn);			
			NET_SendMessage(&pTargetPlayer->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{	
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage,	GSC_REQUESTSUMMONS);
			MSG_WriteString(&netMessage,	requestName);		
			MSG_WriteString(&netMessage,	TargetName);			
			MSG_WriteByte(&netMessage,		targetWorldIdx);	
			MSG_WritePosition(&netMessage,	targetPosition);	
			MSG_WriteByte(&netMessage,		forcedYn);			
			
			if ( g_config.isManager )
			{
				
				for (int i=1; i < MAX_MEMBER_SERVER; i++)	
				{
					if ( !g_memberServer[i].active ) continue;
					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			else
			{
				
				MSG_WriteByte(&netMessage, g_config.gameServerNo);
				
				NET_SendUnreliableMessage(&localSocket, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);
	}
}


void GTH_SendMessage_RequestSummons()
{
	char toName[NAMESTRING];
	int forcedYn;

	strcpy(toName, MSG_ReadString());
	forcedYn = MSG_ReadByte();

	
	if ( forcedYn == tagPcSummonTypedef::enumPcSummonType::PC_SUMMON_FORCED )	
	{
		
		if ( !GTH_CheckValidation_GameMaster( g_curPC ) )
		{
			g_logSystem->Write("Force Summon Used by User '%s'(ID: %s), User is not Operator", g_curPC->name, g_curPC->userID);
			return;
		}
	}
	else if ( forcedYn == tagPcSummonTypedef::enumPcSummonType::PC_SUMMON_ITEM )	
	{
		g_logSystem->Write("Summon Used by Item '%s'(ID: %s), User is Not Operator.", g_curPC->name, g_curPC->userID);
	}
	else 
	{
		
		if ( !GTH_CheckValidation_GameMaster( g_curPC ) )
		{
			g_logSystem->Write("Summon Used by User '%s'(ID: %s), User is Not Operator.", g_curPC->name, g_curPC->userID);
			return;
		}
	}

	
	GTH_PC_Summon(g_curPC,toName,forcedYn);
}


void GTH_SendMessage_ServerRequestSummons()
{
	int i, idx;
	char toName[NAMESTRING], summoner[NAMESTRING];
	vec3_t position;
	int worldIdx;
	int forcedYn;
	int fromMemberIdx;

	strcpy(toName, MSG_ReadString());
	strcpy(summoner, MSG_ReadString());
	worldIdx = MSG_ReadByte();
	MSG_ReadPosition( position );
	forcedYn = MSG_ReadByte();

	idx = GTH_FindPCByName(toName);
	if (idx >= 0) 
	{
		if(FALSE == gcpTools->isSummonAllow(&g_pc[idx])) return;
		
		GTH_PC_SetSummonsInfo(&g_pc[idx], summoner,worldIdx, position, forcedYn);

		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_REQUESTSUMMONS);
			MSG_WriteString(&netMessage, summoner);	
			MSG_WriteByte(&netMessage, forcedYn);			
			NET_SendMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		if ( g_config.isManager )
		{
			fromMemberIdx = MSG_ReadByte(); 

			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_REQUESTSUMMONS);
				MSG_WriteString(&netMessage, toName); 
				MSG_WriteString(&netMessage, summoner);	
				MSG_WriteByte(&netMessage, worldIdx);	
				MSG_WritePosition(&netMessage, position); 
				MSG_WriteByte(&netMessage, forcedYn);	
				
				
				for (i=1; i < MAX_MEMBER_SERVER; i++)	
				{
					if ( !g_memberServer[i].active ) continue;
					if ( i == fromMemberIdx ) continue;	
					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}

void GTH_SendMessage_ReplySummons(int reply, char *toName, char *fromName)
{
	int i, idx;

	idx = GTH_FindPCByName(toName);
	if (idx >= 0) 
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_REPLYSUMMONS);
			MSG_WriteByte(&netMessage, reply);	
			MSG_WriteString(&netMessage, fromName);	
			NET_SendMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_REPLYSUMMONS);
			MSG_WriteByte(&netMessage, reply);	
			MSG_WriteString(&netMessage, toName);	
			MSG_WriteString(&netMessage, fromName);	
			
			if ( g_config.isManager )
			{
				
				for (i=1; i < MAX_MEMBER_SERVER; i++)	
				{
					if ( !g_memberServer[i].active ) continue;
					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			else
			{
				
				MSG_WriteByte(&netMessage, g_config.gameServerNo);
				
				NET_SendUnreliableMessage(&localSocket, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);
	}
}

void GTH_SendMessage_ServerReplySummons()
{
	int i, idx;
	char toName[NAMESTRING], fromName[NAMESTRING];
	int fromMemberIdx;
	int reply;

	reply = MSG_ReadByte();
	strcpy(toName, MSG_ReadString());
	strcpy(fromName, MSG_ReadString());

	idx = GTH_FindPCByName(toName);
	if (idx >= 0) 
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_REPLYSUMMONS);
			MSG_WriteByte(&netMessage, reply);	
			MSG_WriteString(&netMessage, fromName);	
			NET_SendMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		if ( g_config.isManager )
		{	
			fromMemberIdx = MSG_ReadByte(); 
			
			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_REPLYSUMMONS);
				MSG_WriteByte(&netMessage, reply);	
				MSG_WriteString(&netMessage, toName);	
				MSG_WriteString(&netMessage, fromName);	
				
				for (i=1; i < MAX_MEMBER_SERVER; i++)	
				{
					if ( !g_memberServer[i].active ) continue;
					if ( i == fromMemberIdx ) continue;	
					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}






void GTH_SendMessage_NotFoundPC( playerCharacter_t *pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_NOTFOUNDPC);
		NET_SendUnreliableMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_RejectConnection(struct sockaddr_in *addr, int reason)
{
	i3socket_t tempSock;

	
	NET_InitSocket(&tempSock, addr, gsSocket.socket, SocketType_Reliable);

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REJECT_CLIENT);
		MSG_WriteByte(&netMessage, reason);
		NET_SendUnreliableMessage(&tempSock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
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

	
	g_logSystem->Write("Server Close Status Info : SendingQueue (%d, %d), ReceivingQueue (%d, %d), QReqQ(%d, %d), QResQ(%d, %d), PC:(%d, %d), Monster: %d, Item: %d", 
			sendingQueue->m_count, sendingQueue->m_maxCount, receivingQueue->m_count, receivingQueue->m_maxCount, 
			queryRequestQueue->m_count, queryRequestQueue->m_maxCount, queryResultQueue->m_count, queryResultQueue->m_maxCount, 
			g_pcNumber, g_maxPCNumber, g_monsterNumber, g_objectNumber);
}




void GTH_SendMessage_ReplyCheatCode( playerCharacter_t* pc, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_CHEATCODE );
		
		MSG_WriteByte(&netMessage, type );
		
		switch( type )
		{
		case CHEATCODE_SKILLPOINT :
			MSG_WriteLong(&netMessage, pc->bonusSkillPoint );
			break;
		case CHEATCODE_GETMAXSE :
			MSG_WriteLong(&netMessage, pc->curChargeSE);
			break;
		case CHEATCODE_GETGENCAPABILITY :
			MSG_WriteLong(&netMessage, pc->genCapability[pc->selectedGen]);
			break;		

		
		case CHEATCODE_CLEARINVEN:
			MSG_WriteLong(&netMessage, pc->curItemWeight);
			break;

		
		case CHEATCODE_DANBATTLE_PCPOINT:
			MSG_WriteLong(&netMessage, pc->nAccumulationBattleKillPoint);
			break;

		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}





int		FindMonsterNearPlace(int localWorldIdx, vec3_t position )
{


	if( !AI_CheckCollision(position, &g_cmap[localWorldIdx]) )
	{
		return -1;
	}

	return 1;
}


void GTH_SendMessage_ItemDropTest( playerCharacter_t* pc, int* resultItem, int* resultNumber )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_REPLY_CHEATCODE );
		MSG_WriteByte( &netMessage, CHEATCODE_ITEMDROPTEST );
		MSG_WriteLong( &netMessage, 0 );
		
		int itemTypeCount = 0;
		for(int i=0;i<1000;i++)
		{
			if ( resultItem[i] < 0 )
			{
				itemTypeCount = i;
				break;
			}
		}
		
		MSG_WriteShort(&netMessage, itemTypeCount);
		
		for(i=0;i<itemTypeCount;i++)
		{
			MSG_WriteShort(&netMessage, resultItem[i]);
			MSG_WriteShort(&netMessage, resultNumber[i]);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

























#include "..\GolryunBattle\CServerLpackCtrl.h"
extern CServerLpackCtrl* gcpServerLpackCtrl;

void GTH_SendMessage_ChattingMsg( playerCharacter_t *pc, int dlgType, int lpackType, int lpackIdx )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_CHATTINGMSG );
		MSG_WriteByte( &netMessage, dlgType );

		if ( dlgType != 2)
		{
			MSG_WriteShort( &netMessage, lpackType );		
			MSG_WriteShort( &netMessage, lpackIdx );
		}
		else				
			MSG_WriteString( &netMessage, gcpServerLpackCtrl->GetLpack(lpackIdx) );
		
		
		NET_SendMessage( &pc->sock, &netMessage );
	}
	MSG_EndWriting( &netMessage );
}


















void GTH_SendChatMessage_Whisper(void)
{
	int i=-1;
	int idx=-1;
	char toName[NAMESTRING]="";
	char chatMessage[CHATSTRING]="";

	sstrncpy( toName, MSG_ReadString(), NAMESTRING - 1 );
	toName[NAMESTRING-1]=NULL;

	sstrncpy( chatMessage, MSG_ReadString(), CHATSTRING );
	chatMessage[CHATSTRING-1]=NULL;

	idx = GTH_FindPCByName(toName);
	 
	if (idx >= 0){
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_CHATMESSAGE_WHISPER);
			MSG_WriteString(&netMessage, g_curPC->name);	
			MSG_WriteString(&netMessage, chatMessage);
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	
	else
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_CHATMESSAGE_WHISPER);
			MSG_WriteString(&netMessage, toName);	        
			MSG_WriteString(&netMessage, g_curPC->name);	
			MSG_WriteString(&netMessage, chatMessage);
			
			if(g_config.isManager){

				
				for (i=1; i < MAX_MEMBER_SERVER; i++){	
					if ( !g_memberServer[i].active ) continue;					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}else{
				
				MSG_WriteByte(&netMessage, g_config.gameServerNo);
				
				NET_SendUnreliableMessage(&localSocket, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);

		
		
		
		GTH_SetServerAckMessage( g_curPC );	
	}
}



void GTH_SendChatMessage_ServerWhisper(void)
{
	int  i=-1;
	int  idx=-1;
	char toName[NAMESTRING]="";
	char fromName[NAMESTRING]="";
	char chatMessage[CHATSTRING]="";
	int  fromMemberIdx=-1;

	sstrncpy( toName, MSG_ReadString(), NAMESTRING - 1 );	
	toName[NAMESTRING-1]=NULL;

	sstrncpy( fromName, MSG_ReadString(), NAMESTRING - 1 ); 
	fromName[NAMESTRING-1]=NULL;

	sstrncpy( chatMessage, MSG_ReadString(), CHATSTRING );  
	chatMessage[CHATSTRING-1]=NULL;

	idx = GTH_FindPCByName( toName );
	if (idx >= 0){ 
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_CHATMESSAGE_WHISPER);
			MSG_WriteString(&netMessage, fromName);	
			MSG_WriteString(&netMessage, chatMessage);
			NET_SendUnreliableMessage(&g_pc[idx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);

		
		GTH_SendMessage_AckMessage(fromName);

	}else{	
	
		if(g_config.isManager){

			fromMemberIdx = MSG_ReadByte(); 
			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_CHATMESSAGE_WHISPER);
				MSG_WriteString(&netMessage, toName);	            
				MSG_WriteString(&netMessage, fromName);	
				MSG_WriteString(&netMessage, chatMessage);
				
				for (i=1; i < MAX_MEMBER_SERVER; i++){	
					if ( !g_memberServer[i].active ) continue;
					if ( i == fromMemberIdx ) continue;	
					
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}



void GTH_SendMessage_AckMessage(const char* in_strToName)
{
	int i=-1;
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SERVERACKMSG);
		MSG_WriteString(&netMessage, (char*)in_strToName);	        
		
		if(g_config.isManager){
			
			for (i=1; i < MAX_MEMBER_SERVER; i++){
				if ( !g_memberServer[i].active ) continue;				
				NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
			}
		}else{
			
			MSG_WriteByte(&netMessage, g_config.gameServerNo);
			
			NET_SendUnreliableMessage(&localSocket, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}





void GTH_SendMessage_ServerAckMessage(void)
{
	int i=-1;
	int idx=-1;
	char toName[NAMESTRING]="";
	int fromMemberIdx=-1;


	strncpy(toName, MSG_ReadString(),NAMESTRING);
	toName[NAMESTRING-1]=NULL;

	idx = GTH_FindPCByName(toName);
	if (idx >= 0){ 
		GTH_ClearServerAckMessage( &g_pc[idx] );
	
	}else{	
		if(g_config.isManager){
			fromMemberIdx = MSG_ReadByte(); 
			
			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_SERVERACKMSG);
				MSG_WriteString(&netMessage, toName);	
				
				for (i=1; i < MAX_MEMBER_SERVER; i++){	
					if ( !g_memberServer[i].active ) continue;
					if ( i == fromMemberIdx ) continue;	
					NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}


void GTH_SendMessage_ChaoPointEvent(playerCharacter_t* pPc, int DecPoint)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);	
		MSG_WriteShort(&netMessage, CHAO_POINT_EVENT);		
		MSG_WriteByte(&netMessage,  DecPoint);				
		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_ProcessMessage_PING(void)
{
	int difftime = g_curPC->sock.lastSendTime - g_curPC->NetlastReceiveTime;


	ShowLogInfo("PINGTime : %s , %d ,%d ,Ê±¼ä²î %d",g_curPC->name,g_curPC->sock.lastSendTime,g_curPC->NetlastReceiveTime,difftime);

	if (difftime == 0)
	{
		g_curPC->NetlastReceiveTime = g_curPC->sock.lastSendTime;
	}


	if (difftime <= 5500) 
	{
		g_logSystem->WriteToLogTT("Òì³£ User Name =%s Was NetlastReceiveTime =%d lastReceiveTime = %d ,	Ê±²î %d", 
			g_curPC->name, 
			g_curPC->NetlastReceiveTime, 
			g_curPC->sock.lastReceiveTime, 
			difftime
			);
		GTH_DisconnectPlayer(g_curPC,FALSE);

	}else
	{
		g_curPC->NetlastReceiveTime = g_curPC->sock.lastSendTime;
	
	
	}
	
}