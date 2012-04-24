








#include "..\\global.h"



#define		CANT_FIND_PC	-1



void ProcessBillingQueue ()
{
	PACKET_GAME packetGame ;

	while ( g_pBillingSocket->m_recv.Get ( &packetGame ) )
	{
		switch ( ntohl (packetGame.PacketType) )
		{
		case GameStart:
			Billing_ProcessGameStart (&packetGame) ;
			break ;

		case SeverAlive:
			Billing_ProcessServerAlive (&packetGame) ;
			break ;

		case UserAlert:
			Billing_ProcessUserAlert (&packetGame) ;
			break ;
		
		case UserSync:
			Billing_ProcessUserSync (&packetGame) ;
			break ;
		
		default:
			Billing_WrongPacket (&packetGame) ;
			break ;
		}
	}






}




void Billing_SendGameStart(playerCharacter_t* pc)				
{

	if (g_pBillingSocket->GetStatus()==FALSE)											
		return;

	if (pc->BillSendType==BILL_SEND_START)												
		return;

	pc->BillSendType=BILL_SEND_START;													
	
	PACKET_GAME packetGame ;

	memset(&packetGame,0,sizeof(PACKET_GAME));

	packetGame.PacketType = htonl (GameStart) ;
	
	strncpy (packetGame.UserID,
					pc->userID,
					sizeof (packetGame.UserID));
	
	strncpy (packetGame.UserIP,
					inet_ntoa (pc->sock.addr.sin_addr),
					sizeof (packetGame.UserIP));
	
	packetGame.GameNo = htonl (1) ;	
				
	
	
	_snprintf(
				packetGame.GameServer, sizeof (packetGame.GameServer),
				"%s%03d:s%03d",	_strlwr (g_config.serverLocation), g_config.serverGroupNo,
				g_config.gameServerNo
				);

	g_logSystem->WriteRLog( "Send Game Start !! UserID : %s, Character Name : %s",pc->userID,pc->name );
	g_pBillingSocket->Send(&packetGame);
}


void Billing_SendMessage(playerCharacter_t *pc,int msgIdx)								
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_BILLING);
		MSG_WriteByte(&netMessage, msgIdx);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void Billing_ProcessGameStart (PPACKET_GAME pPacket)
{


	int idx = Billing_FindPCbyUserID (pPacket->UserID) ;
	if (CANT_FIND_PC == idx)
	{
		g_logSystem->Write ("Billing: recv GameStart "
			"but can't find userID: %s",
			pPacket->UserID) ;
		


		Billing_SendGameEnd( pPacket );	
		
		return ;
	}

	if ( 0 != ntohl(pPacket->PacketResult) )		
	{
		g_logSystem->WriteRLog("Disconnect User User ID : %s, PacketResult : %d",pPacket->UserID,ntohl(pPacket->PacketResult));

		char	msg[MAX_PATH];
		
		memset(msg,0,MAX_PATH);

		switch ( ntohl(pPacket->PacketResult) )
		{
		case 3  :
					Billing_SendMessage ( &g_pc[idx], 0 ) ;	
					break;
		case 10 :
		case 11 :
		case 33 :
					Billing_SendMessage ( &g_pc[idx], 1 ) ;
					break;
		default :
					Billing_SendMessage ( &g_pc[idx], 2 ) ;
					break;
		}			

		GTH_DisconnectGameServer (idx) ;
	}
}



void Billing_SendGameEnd (playerCharacter_t *pc)
{
	if (pc == NULL) return ;
	if (pc->userID == NULL || pc->userID[0] == '\0') return ;

	if ( pc->BillSendType==BILL_SEND_END)
		return;

	pc->BillSendType=BILL_SEND_END;

	PACKET_GAME packetGame ;

	memset(&packetGame,0,sizeof(PACKET_GAME));		

	packetGame.PacketType = htonl (GameEnd) ;
	_snprintf (packetGame.GameServer, sizeof (packetGame.GameServer),
			   "%s%03d:s%03d",
			   g_config.serverLocation,
			   g_config.serverGroupNo,
			   g_config.gameServerNo) ;
	_snprintf (packetGame.UserID, sizeof (packetGame.UserID),
			   pc->userID) ;
	_snprintf (packetGame.UserIP, sizeof (packetGame.UserIP),
			   inet_ntoa (pc->sock.addr.sin_addr)) ;
	packetGame.GameNo = htonl (1) ;	

	g_logSystem->WriteRLog( "Send Game End !! UserID : %s, Character Name : %s",pc->userID, pc->name );	
	g_pBillingSocket->Send (&packetGame) ;

}

void Billing_SendGameEnd (PPACKET_GAME pRecvPacket)
{


	PACKET_GAME packetGame ;

	memset(&packetGame,0,sizeof(PACKET_GAME));				

	packetGame.PacketType = htonl (GameEnd) ;
	_snprintf (packetGame.GameServer, sizeof (packetGame.GameServer),
			   "%s%03d:s%03d",
			   g_config.serverLocation,
			   g_config.serverGroupNo,
			   g_config.gameServerNo) ;
	_snprintf (packetGame.UserID, sizeof (packetGame.UserID),
			   pRecvPacket->UserID) ;
	_snprintf (packetGame.UserIP, sizeof (packetGame.UserIP),
			   pRecvPacket->UserIP) ;
	packetGame.GameNo = htonl (1) ;	

	g_logSystem->WriteRLog( "Send Game End Recv Packet!! UserID : %s",packetGame.UserID );	
	g_pBillingSocket->Send (&packetGame) ;
}




void Billing_ProcessServerAlive (PPACKET_GAME pPacket)
{
}



void Billing_ProcessUserAlert (PPACKET_GAME pPacket)
{
	int idx = Billing_FindPCbyUserID (pPacket->UserID) ;
	if ( CANT_FIND_PC == idx )
	{
		return ;
	}

	char msg[MAX_PATH] ;

	if ( 0 < ntohl (pPacket->BillRemain) )
	{
		int	 hours = ntohl (pPacket->BillRemain)/3600 ;

		_snprintf ( msg, MAX_PATH, "Your remain time is "
			"%02d hours %02d minutes %02d seconds",
			hours,
			(ntohl (pPacket->BillRemain)/60) - (hours * 60),
			ntohl (pPacket->BillRemain)%60 ) ;

		GTH_SendMessage_NoticeMessage ( &g_pc[idx], msg ) ;
	}
	else
	{
		Billing_SendMessage( &g_pc[idx], 3 );
		GTH_DisconnectGameServer (idx) ;
	}
}



void Billing_ProcessUserSync (PPACKET_GAME pPacket)
{
	PACKET_GAME packetGame = { 0, } ;

	packetGame.PacketType = htonl (UserSync) ;
	strcpy ( packetGame.UserID, pPacket->UserID ) ;

	int idx = Billing_FindPCbyUserID (pPacket->UserID) ;
	if ( CANT_FIND_PC == idx )
	{
		packetGame.PacketResult = htonl ( 0 ) ;
	}
	else
	{
		packetGame.PacketResult = htonl ( 1 ) ;

		strncpy ( packetGame.UserID,
			inet_ntoa (g_pc[idx].sock.addr.sin_addr),
			sizeof (packetGame.UserID) ) ;
	}

	if ( !g_pBillingSocket->Send ( &packetGame ) )
	{
		
		g_logSystem->WriteToError ("Billing link broken");
	}
}



void Billing_WrongPacket (PPACKET_GAME pPacket)
{
	g_logSystem->WriteToError ("Wrong Billing Packet: "
		"Type: %d, "
		"Result: %d, "
		"GameServer: %s, "
		"User_ID: %s, "
		"User_IP: %s, "
		"Game_No: %d, "
		"BillMethod: %c%c, "
		"BillExpire: %c%c%c%c%c%c%c%c%c%c%c%c, "
		"BillRemain: %d",
		ntohl (pPacket->PacketType),
		ntohl (pPacket->PacketResult),
		pPacket->GameServer,
		pPacket->UserID,
		pPacket->UserIP,
		ntohl (pPacket->GameNo),
		pPacket->BillMethod[0], pPacket->BillMethod[1],
		pPacket->BillExpire[0],
		pPacket->BillExpire[1],
		pPacket->BillExpire[2],
		pPacket->BillExpire[3],
		pPacket->BillExpire[4],
		pPacket->BillExpire[5],
		pPacket->BillExpire[6],
		pPacket->BillExpire[7],
		pPacket->BillExpire[8],
		pPacket->BillExpire[9],
		pPacket->BillExpire[10],
		pPacket->BillExpire[11],
		ntohl (pPacket->BillRemain) ) ;
}



void Billing_ServerConnect()
{
	PACKET_GAME packetGame ;

	DWORD kkk=ServerConnection;

	memset(&packetGame,0,sizeof(PACKET_GAME));

	packetGame.PacketType = htonl (ServerConnection) ;
	_snprintf (packetGame.GameServer, sizeof (packetGame.GameServer),
			   "%s%03d:s%03d",
			   g_config.serverLocation,
			   g_config.serverGroupNo,
			   g_config.gameServerNo) ;
	packetGame.GameNo = htonl (1) ;	

	g_pBillingSocket->Send (&packetGame) ;
}

void Billing_ServerReset()
{
	PACKET_GAME packetGame ;

	DWORD kkk=ServerReset;

	memset(&packetGame,0,sizeof(PACKET_GAME));

	packetGame.PacketType = htonl (ServerReset) ;
	_snprintf (packetGame.GameServer, sizeof (packetGame.GameServer),
			   "%s%03d:s%03d",
			   g_config.serverLocation,
			   g_config.serverGroupNo,
			   g_config.gameServerNo) ;
	packetGame.GameNo = htonl (1) ;	

	g_pBillingSocket->Send (&packetGame) ;
}





int Billing_FindPCbyUserID ( char *userID )
{
	playerCharacter_t *pc = &g_pc[0] ;

	for (int idx = 0 ; idx < MAX_PCS ; ++idx, ++pc)
	{
		if (!pc->active || !pc->ready )		
			continue ;

		if ( !stricmp (userID, pc->userID ) )
			return idx ;
	}

	return CANT_FIND_PC ;
}



bool InitBilling ()
{
	if (!g_config.bCheckBilling)			
	{

		g_logSystem->Write("CHECK!! Billing System Do Not Active By Server.cfg File!!");
		return TRUE;
	}

	if ( NULL != g_pBillingSocket )
	{
		delete g_pBillingSocket ;
		g_pBillingSocket = NULL ;
	}

	g_pBillingSocket = new tcpSocket ;
	if (!g_pBillingSocket->Create ())
	{
		char tbuf[128] ;
		sprintf (tbuf, "ret: %d", WSAGetLastError ()) ;
		MessageBox (NULL, tbuf, "Error", MB_OK) ;
	}

	if (!g_pBillingSocket->Connect (inet_addr (g_config.billingServerIP),
									g_config.billingServerPort))
	{
		MessageBox(NULL,"Can't Connect Billing Server!!","Error",MB_OK);
		return FALSE;
	}


	g_logSystem->Write("Billing System Initialize Complete!!");

	if (g_pBillingSocket->Start ())
	{

		
		Billing_ServerConnect();
		
		Billing_ServerReset();
	}

	return true;
}

void CloseBilling()																		
{
	if (!g_config.bCheckBilling)
		return;

	if ( NULL != g_pBillingSocket )
	{

		g_pBillingSocket->Close();														

		delete g_pBillingSocket ;
		g_pBillingSocket = NULL ;
	}
}




