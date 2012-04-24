








#include "..\\global.h"



#define		CANT_FIND_CLIENT	-1



void ProcessBillingQueue ()
{
	PACKET_GAME packetGame ;

	while ( g_pBillingSocket->m_recv.Get ( &packetGame ) )
	{






		switch ( ntohl (packetGame.PacketType) )
		{
		case SeverAlive:
			Billing_ProcessServerAlive (&packetGame) ;
			break ;

		case BillingAuth:
			Billing_ProcessAuth (&packetGame) ;
			break ;

		default:
			Billing_WrongPacket (&packetGame) ;
			break ;
		}
	}
}




void Billing_ProcessServerAlive (PPACKET_GAME pPacket)
{
}


void Billing_SendAuth(LPSTR UserID)
{
	PACKET_GAME packetGame = { 0, } ;

	packetGame.PacketType = htonl (BillingAuth) ;
	strncpy (packetGame.UserID, UserID,
						sizeof (packetGame.UserID)) ;
	packetGame.GameNo = htonl (1) ;		

	g_pBillingSocket->Send (&packetGame) ;
}

void Billing_ProcessAuth (PPACKET_GAME pPacket)						
{
	int idx = Billing_FindClientbyUserID (pPacket->UserID) ;
	if (CANT_FIND_CLIENT == idx) return ;
	
	i3client_t *client = &g_clients[idx] ;

	if (g_config.bCheckBilling)
	{
		if (0 != ntohl (pPacket->PacketResult))
		{
			switch ( ntohl (pPacket->PacketResult) )
			{
			case 3   : 
				GTH_ReplyLoginV2(client, FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::billing_network_error);
				break;			

			case 10  :
			case 11  :
			case 33  : 
				GTH_ReplyLoginV2 (client, FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::billing_point_not_enough);
				break;
			default  : 
				GTH_ReplyLoginV2 (client, FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::billing_error);
				break;
			}
			
			return;
		}
	}
	
	
	memcpy( &client->billMethod,	&pPacket->BillMethod, 2);
	memcpy( &client->billRemain,	&pPacket->BillRemain, sizeof(int) );
	memcpy( &client->billExpire,	&pPacket->BillExpire, 12);

	g_DBAccountServer->InsertLogin( client );
	g_logSystem->Write("ID:%s is connected(%d).(%s)",
						client->id,
						client->dbCheckAccount,
						NET_AddrToString((sockaddr*)&client->sock.addr));
}



void Billing_WrongPacket (PPACKET_GAME pPacket)
{
	g_logSystem->Write ("Wrong Billing Packet: "
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




int Billing_FindClientbyUserID ( char *userID )
{
	i3client_t *client = &g_clients[0] ;

	for (int idx = 0 ; idx < MAX_CLIENTS ; ++idx, ++client)
	{
		if (!client->active) continue ;

		if (!stricmp (userID, client->id))
			return idx ;
	}

	return CANT_FIND_CLIENT ;
}

bool InitBilling ()
{
	if (!g_config.bCheckBilling)				
	{
		char tempString[MAX_PATH];
		sprintf(tempString,"CHECK!! Billing System Do not Active By Server.cfg File!!\n");
		OutputDebugString(tempString);

		g_logSystem->Write ("CHECK!! Billing System Do not Active By Server.cfg File!!");
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

	g_logSystem->Write ("Billing System Initialize Complete!!");
	g_pBillingSocket->Start () ;

	return true;
}


