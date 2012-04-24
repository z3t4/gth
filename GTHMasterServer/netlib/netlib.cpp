
#include "netlib.h"
#include "queue.h"
#include <time.h>




extern i3sizebuf_t		netMessage;
extern i3sizebuf_t		recvMessage;

extern i3packet_t		packetBuffer;


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


extern unsigned int		g_netTime;
extern unsigned int		prevTime;
extern unsigned int		curTime;
extern unsigned int g_globalTime;


extern int				channel[ MAX_CHANNELS ];
extern int				channelNumber;


extern HANDLE			hThread;
extern DWORD			SendingThreadID, ReceivingThreadID;
extern int				runSendingThread, runReceivingThread;





extern CSendingQueue	*sendingQueue;
extern CReceivingQueue	*receivingQueue;

extern HANDLE			hSQMutex, hRQMutex;






extern FILE				*packetErrorLog;


extern int				lastRecvSequence;
extern int				myRecvSequence;

extern CRITICAL_SECTION	spCrit;

using namespace std;

#include <string>
#include <vector>

#include "../CheckSum.h"
FILE					*g_logFile;
vector<string>			g_logFilename;







int sendFirstWrite = true;
int recvFirstWrite = true;

int	g_lastWriteSequence = -1;




void WriteCompletePacket( int idx, int type )
{
	return;

	int *firstwrite;
	char filename[256];
	char msg[128];

	if( type == 0 ) 
	{
		strcpy( filename, "send.txt" );
		firstwrite = &sendFirstWrite;
		strcpy( msg, "-" );
	}
	else if( type == 1 )
	{
		strcpy( filename, "recv.txt" );
		firstwrite = &recvFirstWrite;
		strcpy( msg, "-" );

		if( g_lastWriteSequence < 0 ) g_lastWriteSequence = idx;
		else if( g_lastWriteSequence != idx )
		{
			int checkSequence = 1;
		} 
		else g_lastWriteSequence = idx;
	}
	
	else if( type == 2 )
	{
		strcpy( filename, "recv.txt" );
		firstwrite = &recvFirstWrite;
		sprintf( msg, "dup : %d<%d", myRecvSequence, lastRecvSequence );
	}
	
	else if( type == 3 )
	{
		strcpy( filename, "recv.txt" );
		firstwrite = &recvFirstWrite;
		sprintf( msg, "drop : %d>%d", myRecvSequence, lastRecvSequence );
	}


	FILE *fp;
	
	if( (*firstwrite) )
	{
		fp = fopen( filename, "wt" );
		(*firstwrite) = false;
	}
	else
	{
		fp = fopen( filename, "at" );
	}

	fprintf( fp, "%s[%d]packet complete\n", msg, idx );

	fclose( fp );
}




void NET_WriteErrorLog( char* str, struct sockaddr_in *addr )
{
	return;
	
	char			cTime[32];
	time_t			logtime;
	
	time( &logtime );
	strcpy( cTime, ctime( &logtime ) );
	
	cTime[24]	=	'\0';	

#ifdef _COMPILE_FOR_SERVER
	packetErrorLog = fopen( "net_error.txt", "at" );
#else
	packetErrorLog = fopen( "../net_error.txt", "at" );
#endif
	fprintf( packetErrorLog, "%s : %s(%s)\n", cTime, str, inet_ntoa( addr->sin_addr ) );
	fclose( packetErrorLog );
}





int NET_AddrCompare (struct sockaddr *addr1, struct sockaddr *addr2)
{
	if (addr1->sa_family != addr2->sa_family)
		return -1;

	if (((struct sockaddr_in *)addr1)->sin_addr.s_addr != ((struct sockaddr_in *)addr2)->sin_addr.s_addr)
		return -1;

	if (((struct sockaddr_in *)addr1)->sin_port != ((struct sockaddr_in *)addr2)->sin_port)
		return 1;

	return 0;
}


char *NET_AddrToString (struct sockaddr *addr)
{
	static char buffer[22];
	int haddr;

	haddr = ntohl(((struct sockaddr_in *)addr)->sin_addr.s_addr);
	sprintf(buffer, "%d.%d.%d.%d:%d", (haddr >> 24) & 0xff, (haddr >> 16) & 0xff, (haddr >> 8) & 0xff, haddr & 0xff, ntohs(((struct sockaddr_in *)addr)->sin_port));
	return buffer;
}

int NET_StringToAddr (char *string, struct sockaddr *addr)
{
	int ha1, ha2, ha3, ha4, hp;
	int ipaddr;

	sscanf(string, "%d.%d.%d.%d:%d", &ha1, &ha2, &ha3, &ha4, &hp);
	ipaddr = (ha1 << 24) | (ha2 << 16) | (ha3 << 8) | ha4;

	addr->sa_family = AF_INET;
	((struct sockaddr_in *)addr)->sin_addr.s_addr = htonl(ipaddr);
	((struct sockaddr_in *)addr)->sin_port = htons((unsigned short)hp);
	return 0;
}

char *NET_GetLocalAddress()
{
	struct hostent *hostEntry;
	int haddr;
	static char buffer[16];
	char hostName[MAXHOSTNAMELEN];

	if ( gethostname(hostName, MAXHOSTNAMELEN) == SOCKET_ERROR ) return NULL;

	hostEntry = gethostbyname( hostName );
	if (hostEntry == NULL) return NULL;

	haddr = ntohl( *(int *)hostEntry->h_addr );
	sprintf(buffer, "%d.%d.%d.%d", (haddr >> 24) & 0xff, (haddr >> 16) & 0xff, (haddr >> 8) & 0xff, haddr & 0xff);
	return buffer;
}


char *NET_AddrToIPString (struct sockaddr *addr)
{
	static char buffer[16];
	int haddr;

	haddr = ntohl(((struct sockaddr_in *)addr)->sin_addr.s_addr);
	sprintf(buffer, "%d.%d.%d.%d", (haddr >> 24) & 0xff, (haddr >> 16) & 0xff, (haddr >> 8) & 0xff, haddr & 0xff);
	return buffer;
}


char *NET_GetHostName()
{
	static char hostNameString[MAXHOSTNAMELEN];

	if ( gethostname(hostNameString, MAXHOSTNAMELEN) == SOCKET_ERROR ) return NULL;
	return hostNameString;
}





void NET_ClearBank( PacketMap *pm )
{
	for( PacketMapItor itor = pm->begin(); itor != pm->end(); ++itor )
	{
		if( ((*itor).second) == NULL ) continue;
		delete ((*itor).second);
	}
	pm->clear();
}


int NET_InsertPacketBank( PacketMap *pm, i3packet_t *packet )
{
	PacketMapItor	itor	=	pm->find( packet->sequence );
	i3packet_t		*pk;
	
	
	if( itor != pm->end() ) return false;

	
	pk	=	new i3packet_t;

	memcpy( pk, packet, sizeof( i3packet_t ) );

	pm->insert( PacketMapPair( packet->sequence, pk ) );
	
	return true;
}

int NET_DeletePacketBank( PacketMap *pm, UINT sequence )
{
	PacketMapItor itor = pm->find( sequence );
	
	if( itor == pm->end() ) return false;	
	delete (*itor).second;

	pm->erase( itor );

	return true;
}


int NET_SendPacket (i3socket_t *sock, i3sizebuf_t *data, BOOL bEncrypt)
{
	unsigned int	packetLen;
	unsigned int	dataLen;
	byte			som;	
	byte			eom;	
	unsigned int	encrypt = 0;



	
	if (data->cursize == 0)
	{
		packetsDropCount++;
		return 0;
	}
	if (data->cursize > NET_MAXMESSAGE)
	{
		packetsDropCount++;
		return 0;
	}

	memcpy(sock->sendMessage, data->data, data->cursize);
	sock->sendMessageLength = data->cursize;

	som	=	0xff;

	while( 1 )
	{
		if (sock->sendMessageLength <= MAX_DATAGRAM) 
		{
			dataLen			=	sock->sendMessageLength;
			eom				=	NETFLAG_EOM;
			som				=	0;
		}
		else
		{
			dataLen			=	MAX_DATAGRAM;
			eom				=	0;
			
			
			
			
			if( som == 0xff ) som	=	NETFLAG_SOM;
			else som = 0;
		}
		packetLen = NET_HEADERSIZE + dataLen;
		
		
		
		
		
		if( sock->socketType == SocketType_Reliable) 
			packetBuffer.length			=	((NETFLAG_RELIABLE | eom) << 12) | packetLen;
		else
			packetBuffer.length			=	((NETFLAG_RELIABLE | eom | som) << 12) | packetLen;
		

		packetBuffer.sequence		=	sock->sendSequence++;
		packetBuffer.lastSendTime	=	g_netTime;

		
		packetBuffer.encrypt		= bEncrypt;

		memcpy( packetBuffer.data, sock->sendMessage, dataLen );

		if (bEncrypt == TRUE)	
		{
			Crypt((TCHAR*)packetBuffer.data, dataLen);		
			
			checksum packetsum;
			packetsum.clear();
			
			
			
			
			packetsum.add((BYTE*)packetBuffer.data, dataLen);
			packetBuffer.checksum = packetsum.get();
		}
		
		
		
		if( sock->sendPacketMap != NULL )		
		{
			if( sock->socketType == SocketType_Reliable ) NET_InsertPacketBank( sock->sendPacketMap, &packetBuffer );
		}

		
#ifndef _USE_SENDING_THREAD
		if (NET_Write(sock->socket, (byte *)&packetBuffer, packetLen, &sock->addr) == -1)
#else
		if (NET_WriteToSendingQueue(sock->socket, (byte *)&packetBuffer, packetLen, &sock->addr) == -1)
#endif
		return -1;




			
		sock->lastSendTime = NET_GetNetTime();

		totalPacketsSend ++;
		packetsSent ++;

		
		if( eom != 0 ) break;
		
		sock->sendMessageLength		-=	 MAX_DATAGRAM;

		
		memcpy( sock->sendMessage, sock->sendMessage+MAX_DATAGRAM, sock->sendMessageLength );
	}

	return 1;
}




int NET_SendPacket_Unreliable(i3socket_t *sock, i3sizebuf_t *data, BOOL bEncrypt)
{
	
	
	if( sock->socketType == SocketType_Unreliable )
	{
		return( NET_SendPacket( sock, data ) );
	}

	unsigned int	packetLen;
	unsigned int	dataLen;
	byte			eom;	

	
	if (data->cursize == 0)
	{
		un_packetsDropCount++;
		return 0;
	}
	if (data->cursize > MAX_DATAGRAM)
	{
		un_packetsDropCount++;
		return 0;
	}

	memcpy(sock->sendMessage, data->data, data->cursize);
	sock->sendMessageLength = data->cursize;

	dataLen			=	sock->sendMessageLength;
	eom				=	NETFLAG_EOM;
	packetLen		=	NET_HEADERSIZE + dataLen;
		
	packetBuffer.length			=	((NETFLAG_RELIABLE | eom) << 12) | packetLen;	
	packetBuffer.sequence		=	sock->unreliableSendSequence ++;
	
	packetBuffer.lastSendTime	=	NET_SENDTIME_UNRELIABLE;			

	
	unsigned int  CheckSum = 0;	
	packetBuffer.encrypt    =   bEncrypt;
	
	memcpy( packetBuffer.data, sock->sendMessage, dataLen );

	if (bEncrypt == TRUE)	
	{
		Crypt((TCHAR*)packetBuffer.data, dataLen);

		checksum packetsum;
		packetsum.clear();
	
	
	
	
		packetsum.add((BYTE*)packetBuffer.data, dataLen);
		packetBuffer.checksum = packetsum.get();
	}
	
	
		
#ifndef _USE_SENDING_THREAD
		if (NET_Write(sock->socket, (byte *)&packetBuffer, packetLen, &sock->addr) == -1)
#else
		if (NET_WriteToSendingQueue(sock->socket, (byte *)&packetBuffer, packetLen, &sock->addr) == -1)
#endif
		return -1;



			
	sock->lastSendTime = NET_GetNetTime();

	totalPacketsSend ++;
	un_packetsSent ++;

	return 1;
}


int NET_ControlSendBank( i3socket_t *sock )
{
	int packetLen;

	if( sock->sendPacketMap == NULL ) return 0;		

	
	if( sock->sendPacketMap->empty() ) return 0;

	int sendSize =  sock->sendPacketMap->size();

	i3packet_t		*packet;
	
	for( PacketMapItor itor = sock->sendPacketMap->begin(); itor != sock->sendPacketMap->end(); ++itor )
	{
		
		if( g_netTime - ((*itor).second)->lastSendTime < NET_RESENDTIME ) continue;

		packet					=	(*itor).second;
		packet->lastSendTime	=	g_netTime;

		packetLen				=	( packet->length & NETFLAG_LENGTH_MASK );

		packetsReSent ++;
		totalPacketsSend ++;

		
#ifndef _USE_SENDING_THREAD
		if (NET_Write(sock->socket, (byte *)packet, packetLen, &sock->addr) == -1)
#else
		if (NET_WriteToSendingQueue(sock->socket, (byte *)packet, packetLen, &sock->addr) == -1)
#endif
		return -1;



	}

	
	return 1;
}


int NET_ControlRecvBank( i3socket_t *sock )
{
	if( sock->recvPacketMap == NULL ) return false;			

	
	if( sock->recvPacketMap->empty() ) return false;

	i3packet_t		packet;
	unsigned int	length;
	unsigned int	flags;
	unsigned int	sequence;

	int recvSize =  sock->recvPacketMap->size();
	
	while( 1 )
	{
		for( PacketMapItor itor = sock->recvPacketMap->begin(); itor != sock->recvPacketMap->end(); ++itor )
		{
			
			if( ((*itor).second)->sequence != sock->receiveSequence ) continue;
			break;
		}

		
		if( itor == sock->recvPacketMap->end() || sock->recvPacketMap->size() == 0 ) return false;

		memcpy( &packet, (*itor).second, sizeof( i3packet_t ) );
		
		
		NET_DeletePacketBank( sock->recvPacketMap, packet.sequence );
		
		
		length		=		packet.length & NETFLAG_LENGTH_MASK;
		flags		=		packet.length >> 12;
		sequence	=		packet.sequence;
		length		-=		NET_HEADERSIZE;
		

		
		sock->receiveSequence++;
		
		memcpy(sock->receiveMessage + sock->receiveMessageLength, packet.data, length);
		sock->receiveMessageLength += length;
		
		if (flags & NETFLAG_EOM) 
		{
			SZ_Clear(&recvMessage);
			SZ_Write(&recvMessage, sock->receiveMessage, sock->receiveMessageLength);
			sock->receiveMessageLength = 0;
			
			
			return true;
		}
	}

	
	return false;
}


int NET_ControlPacket_ReliableSocket( i3socket_t *sock, i3packet_t *packet )
{
	unsigned int	length;
	unsigned int	flags;
	unsigned int	sequence;

	i3packet_t      ackPacket;
	int				ret = PacketAnalysis_Skip;
	int				count;

	
	sock->lastReceiveTime = g_netTime;

	length		=		packet->length & NETFLAG_LENGTH_MASK;
	flags		=		packet->length >> 12;
	sequence	=		packet->sequence;
	
	
	





	memset( &ackPacket, 0, sizeof( i3packet_t ) );
	
	totalPacketsReceived ++;

	
	if( flags & NETFLAG_ACK )
	{
		
		packetsAckReceived ++;

		if( sock->sendPacketMap != NULL ) NET_DeletePacketBank( sock->sendPacketMap, sequence );	

		
		
		
		
		
		
		


		sock->latencyTime = sock->lastReceiveTime - sock->lastSendTime;

		return ret = PacketAnalysis_Ack;
	}

		
	
	UINT DataSize = length - NET_HEADERSIZE;	

	
	
	if ( packet->encrypt )
	{
		checksum packetsum;
		packetsum.clear();
		
		
		
		
		
		packetsum.add((BYTE*)packetBuffer.data, DataSize);	
		if ( packetBuffer.checksum != packetsum.get() )
		{
			packetsDupCount++;
			ret = PacketAnalysis_Skip;
			return ret;
		}

		Crypt((TCHAR*)packet->data, DataSize);
	}


	if (flags & NETFLAG_RELIABLE)
	{
		
		if( packet->lastSendTime != NET_SENDTIME_UNRELIABLE ) 
		{
			packetsReceived ++;

			
			ackPacket.length			=	(NETFLAG_ACK << 12) | NET_HEADERSIZE;
			ackPacket.sequence			=	sequence; 
			ackPacket.lastSendTime		=	g_netTime;
			
#ifndef _USE_SENDING_THREAD
			NET_Write(sock->socket, (byte *)&ackPacket, NET_HEADERSIZE, &sock->addr);
#else
			NET_WriteToSendingQueue(sock->socket, (byte *)&ackPacket, NET_HEADERSIZE, &sock->addr);
#endif	
			packetsAckSent ++;




			lastRecvSequence = sequence;
			myRecvSequence = sock->receiveSequence;

			
			if( sequence == sock->receiveSequence )
			{


				if ( ( sock->receiveMessageLength + length ) >= NET_MAXMESSAGE )		
				{
					ret = PacketAnalysis_Skip;		
				}
				else
				{
					sock->receiveSequence++;
					
					length -= NET_HEADERSIZE;
					
					memcpy(sock->receiveMessage + sock->receiveMessageLength, packet->data, length);
					sock->receiveMessageLength += length;
					
					if (flags & NETFLAG_EOM) 
					{
						SZ_Clear(&recvMessage);
						SZ_Write(&recvMessage, sock->receiveMessage, sock->receiveMessageLength);
						sock->receiveMessageLength = 0;
						
						
						ret = PacketAnalysis_Start;
					}
					else
					{
						
						ret = PacketAnalysis_Skip;
					}
				}
			}
			
			
			else if( sequence < sock->receiveSequence )
			{

				
				packetsDupCount++;
				ret = PacketAnalysis_Skip;
			}
			
			
			
			else if( sequence > sock->receiveSequence )
			{

				
				packetsDropCount ++; 
				if( sock->recvPacketMap != NULL ) NET_InsertPacketBank( sock->recvPacketMap, packet );		
				ret = PacketAnalysis_Skip;
			}
		}
		
		else
		{
			un_packetsReceived ++;

			length -= NET_HEADERSIZE;

			if (sequence != sock->unreliableReceiveSequence) 
			{
				count = sequence - sock->unreliableReceiveSequence;
				un_packetsDropCount += count;
			}

			
			if ( sequence < sock->unreliableReceiveSequence )
			{
			 	packetsDupCount++;
				ret = PacketAnalysis_Skip;				
				return ret;
			}

			sock->unreliableReceiveSequence = sequence + 1;
			
			SZ_Clear (&recvMessage);
			SZ_Write (&recvMessage, packet->data, length);
				
			
			ret = PacketAnalysis_Start;
		}
	}

	if( ret != PacketAnalysis_Start )
	{
		
		
		if(  NET_ControlRecvBank( sock ) ) ret = PacketAnalysis_Start;
	}

	return ret;
}


int NET_ControlPacket_UnreliableSocket(i3socket_t *sock, i3packet_t *packet)
{	
	unsigned int	length;
	unsigned int	flags;
	unsigned int	sequence;

	i3packet_t      ackPacket;
	int				ret = PacketAnalysis_None;

	
	if( packet->length == 0 ) return PacketAnalysis_None;

	
	sock->lastReceiveTime = g_netTime;

	length		=		packet->length & NETFLAG_LENGTH_MASK;
	flags		=		packet->length >> 12;
	sequence	=		packet->sequence;

	
	
	




	memset( &ackPacket, 0, sizeof( i3packet_t ) );
	
	totalPacketsReceived ++;

	
	if( flags & NETFLAG_ACK )
	{
		return ret = PacketAnalysis_Ack;
	}

	UINT DataSize = length - NET_HEADERSIZE;


	if (packet->encrypt)
	{
		checksum packetsum;
		packetsum.clear();	
		
		
		
		
		packetsum.add((BYTE*)packetBuffer.data, DataSize);	
		if ( packetBuffer.checksum != packetsum.get() )
		{
			packetsDupCount++;
			ret = PacketAnalysis_Skip;
			return ret;
		}

		Crypt((TCHAR*)packet->data, DataSize);
	}

	if (flags & NETFLAG_RELIABLE)
	{
		packetsReceived ++;

		lastRecvSequence = sequence;
		myRecvSequence = sock->receiveSequence;

		


		sock->receiveSequence++;
		
		length -= NET_HEADERSIZE;

		
		if( sock->packetBlockSequence >= 0 )
		{
			
			if( sequence != sock->packetBlockSequence + 1 )
			{
				packetsDropCount++;

				ret = PacketAnalysis_Skip;

				
				
				if( flags & NETFLAG_EOM )
				{
					sock->receiveMessageLength = 0;
					sock->packetBlockSequence = -1;
				}
			}
			else
			{
				
				sock->packetBlockSequence ++;
				memcpy(sock->receiveMessage + sock->receiveMessageLength, packet->data, length);
				sock->receiveMessageLength += length;

				
				if( flags & NETFLAG_EOM )
				{
					sock->packetBlockSequence = -1;
					
					SZ_Clear(&recvMessage);
					SZ_Write(&recvMessage, sock->receiveMessage, sock->receiveMessageLength);
					sock->receiveMessageLength = 0;
					
					
					ret = PacketAnalysis_Start;
				}
				else
				{
					
					ret = PacketAnalysis_Skip;
				}
			}
		}
		
		else if(flags & NETFLAG_SOM)
		{
			
			sock->packetBlockSequence = sequence;

			memcpy(sock->receiveMessage + sock->receiveMessageLength, packet->data, length);
			sock->receiveMessageLength += length;
		
			
			ret = PacketAnalysis_Skip;
		}
		
		else if (flags & NETFLAG_EOM)
		{
			memcpy(sock->receiveMessage + sock->receiveMessageLength, packet->data, length);
			sock->receiveMessageLength += length;

			SZ_Clear(&recvMessage);
			SZ_Write(&recvMessage, sock->receiveMessage, sock->receiveMessageLength);
			sock->receiveMessageLength = 0;
			
			
			ret = PacketAnalysis_Start;
		}
		
		else
		{
			
			ret = PacketAnalysis_Skip;
		}
	}

	return ret;
}


int NET_ControlPacket(i3socket_t *sock, i3packet_t *packet)
{	
	if( sock->socketType == SocketType_Reliable ) 
		return( NET_ControlPacket_ReliableSocket( sock, packet ) );
	else
		return( NET_ControlPacket_UnreliableSocket( sock, packet ) );
}


int NET_CheckSockProblem( i3socket_t *sock, int checkTime )
{
	
	if( sock->sendPacketMap == NULL || sock->recvPacketMap == NULL ) return false;

	
	if( sock->socketType == SocketType_Unreliable ) return true;

	
	
	if( sock->sendPacketMap->size() > NET_MAXIMUM_PACKETMAP_SIZE || sock->recvPacketMap->size() > NET_MAXIMUM_PACKETMAP_SIZE )
	{
		return false;
	}
	if( checkTime < 0 ) return true;

	
	if( g_netTime - sock->lastReceiveTime > checkTime )
	{
		return false;
	}
	return true;
}



int NET_Write (int socket, byte *buf, int len, struct sockaddr_in *addr)
{
	int ret;

	ret = sendto (socket, (char *)buf, len, 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
	if (ret == -1)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{



			return 0;
		}

		return 0;
	}

	return ret;
}




int NET_Read (int socket, byte *buf, int len, struct sockaddr_in *addr)
{
	int addrlen = sizeof (struct sockaddr_in);
	int ret;

	ret = recvfrom (socket, (char *)buf, len, 0, (struct sockaddr *)addr, &addrlen);
	if (ret == -1){

		int errno;
		errno = WSAGetLastError();
		if(errno == WSAEWOULDBLOCK || errno == WSAECONNREFUSED){
			return 0;
		}else{



			return 0;
		}
	}
	return ret;
}




void NET_InitNetTime()
{
	prevTime = curTime = timeGetTime();
	g_netTime = 0;
}

unsigned int NET_GetNetTime()
{
	unsigned int tempTime;

	curTime = timeGetTime();
	if (curTime >= prevTime)
	{
		g_netTime = g_netTime + (curTime - prevTime);
	}
	else
	{
		tempTime = (unsigned int)pow(2, 32) - prevTime; 
		g_netTime = g_netTime + tempTime + curTime;
	}

	prevTime = curTime;

	return g_netTime;
}



void NET_InitPacketMap( i3socket_t *sock )
{
	if( sock->sendPacketMap != NULL )
	{
		NET_ClearBank( sock->sendPacketMap );
		delete sock->sendPacketMap;
		sock->sendPacketMap = NULL;
	}
	if( sock->recvPacketMap != NULL ) 
	{
		NET_ClearBank( sock->recvPacketMap );
		delete sock->recvPacketMap;
		sock->recvPacketMap = NULL;
	}
}

int NET_InitSocket(i3socket_t *sock, struct sockaddr_in *addr, int socket, int socketType)
{
	NET_InitPacketMap( sock );

	sock->socket					=	socket;
	sock->socketType				=	socketType;

	sock->connecttime				=	g_netTime;
	sock->lastReceiveTime			=	g_netTime;
	sock->lastSendTime				=	g_netTime;
	sock->latencyTime				=	0;
	sock->disconnected				=	false;
	sock->canSend					=	true;
	sock->sendNext					=	false;
	sock->resendSequence			=	0;

	sock->ackSequence				=	0;
	sock->sendSequence				=	0;
	sock->unreliableSendSequence	=	0;
	sock->sendMessageLength			=	0;
	sock->receiveSequence			=	0;
	sock->unreliableReceiveSequence	=	0;
	sock->receiveMessageLength		=	0;
	sock->packetBlockSequence		=	-1;

	memcpy(&sock->addr, addr, sizeof(sockaddr_in));

	
	sock->sendPacketMap		=	new PacketMap;
	sock->recvPacketMap		=	new PacketMap;
	sock->sendPacketMap->clear();
	sock->recvPacketMap->clear();

	return 1;
}

int NET_OpenNetwork()
{
    WORD wVersionRequested;
    WSADATA wsaData;
	int i;

	  
	wVersionRequested = MAKEWORD( 2, 2 );
	if (WSAStartup( wVersionRequested, &wsaData ) != 0 ) 
	{
		return false;
	}

	
	NET_InitNetTime();

	
	SZ_Alloc(&netMessage, NET_MAXMESSAGE); 
	SZ_Alloc(&recvMessage, NET_MAXMESSAGE); 

	
	channelNumber = 0;
	for (i=0; i<MAX_CHANNELS; i++)
		channel[i] = -1;

	
	runSendingThread = 0;	
	runReceivingThread = 0;

	
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

	
	InitializeCriticalSection( &spCrit );

	return true;
}


int NET_OpenSocketForServer(i3socket_t *sock, char *serverIP, int port, int socketType )
{
	SOCKET hSocket;
	struct sockaddr_in addr;
	unsigned long _true = 1;

	hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (hSocket == INVALID_SOCKET) 
	{ 
		return 0;
	}	

	
	if (ioctlsocket (hSocket, FIONBIO, &_true) == -1) 
	{ 
        return 0;
	}			

	memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
	if ( serverIP )
	    addr.sin_addr.s_addr = inet_addr(serverIP);
	else
	    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons( port );
   
    if ( bind(hSocket, (LPSOCKADDR) &addr, sizeof(addr)) != 0 )
	{
        return 0;
	}

	NET_InitSocket(sock, &addr, hSocket, socketType);

	if ( NET_AddListenChannel( sock->socket ) == 0 )
	{
		return 0;
	}

	
	sock->socketType = socketType;

	return 1;
}

int NET_OpenSocketForClient(i3socket_t *sock, char *serverIP, int port, int socketType )
{
	SOCKET hSocket;
	struct sockaddr_in addr;
	unsigned long _true = 1;

	hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (hSocket == INVALID_SOCKET) 
	{ 
		return 0;
	}	

	
	if (ioctlsocket (hSocket, FIONBIO, &_true) == -1)
	{ 
        return 0;
	}			

	memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(serverIP);
	addr.sin_port = htons( port );

	NET_InitSocket(sock, &addr, hSocket, socketType);

	if ( NET_AddListenChannel( sock->socket ) == 0 )
	{
		return 0;
	}

	
	sock->socketType = socketType;

	return 1;
}

void NET_CloseSocket(i3socket_t *sock)
{
	NET_DeleteListenChannel ( sock->socket );

    closesocket(sock->socket); 
	sock->socket = INVALID_SOCKET;
}

void NET_ClearSocket(i3socket_t *sock)
{
	if (sock->socket != INVALID_SOCKET)
	{
	    closesocket(sock->socket); 
		sock->socket = INVALID_SOCKET; 
	}
}

void NET_CloseNetwork()
{
	WSACleanup(); 
	DeleteCriticalSection( &spCrit );
}





int NET_CreateSendingThread()
{
	hSQMutex = CreateMutex(NULL, FALSE, NULL);
	hThread = CreateThread(NULL, 0, NET_SendingThread, NULL, 0, &SendingThreadID);
	if ( !hThread )
	{
		return 0;
	}	

	CloseHandle(hThread);
	return 1;
}




int NET_CreateReceivingThread()
{
	hRQMutex = CreateMutex(NULL, FALSE, NULL);
	hThread = CreateThread(NULL, 0, NET_ReceivingThread, NULL, 0, &ReceivingThreadID);
	if ( !hThread )
	{
		return 0;
	}	
	CloseHandle(hThread);
	return 1;
}




DWORD WINAPI NET_SendingThread(LPVOID param)
{
	int socket, len;
	struct sockaddr_in addr;
	i3packet_t buf;

	
	if ( sendingQueue )    { delete sendingQueue;   sendingQueue = NULL; }
	sendingQueue = new CSendingQueue;

	runSendingThread = 1;
	while ( runSendingThread )
	{
		Sleep(1); //lucky CPU
		WaitForSingleObject(hSQMutex, INFINITE);
		if (sendingQueue->m_count > 0)
		{
			sendingQueue->Pop(&socket, (byte *)&buf, &len, &addr);

			NET_Write(socket, (byte *)&buf, len, &addr);

		}
		ReleaseMutex(hSQMutex);
	}

	CloseHandle(hSQMutex); 

	
	if ( sendingQueue )    { delete sendingQueue;   sendingQueue = NULL; }

	return 0;
} 










DWORD WINAPI NET_ReceivingThread(LPVOID param)
{
	int socket, len;
	int addrlen = sizeof (struct sockaddr_in);
	struct sockaddr_in addr;
	i3packet_t buf;
	int i;

	
	if ( receivingQueue )    { delete receivingQueue;   receivingQueue = NULL; }
	receivingQueue = new CReceivingQueue;

	runReceivingThread = 1;
	while ( runReceivingThread )
	{
		Sleep(1); //lucky CPU
		for (i=0; i<channelNumber; i++)
		{
			socket = channel[i];
			len = NET_Read(socket, (byte *)&buf, NET_DATAGRAMSIZE, &addr);

			if (len > 0)
			{
				WaitForSingleObject(hRQMutex, INFINITE);
				receivingQueue->Push(socket, (byte *)&buf, len, &addr);
				ReleaseMutex(hRQMutex);
			}
		}
	}

	CloseHandle(hRQMutex); 

	
	if ( receivingQueue )    { delete receivingQueue;   receivingQueue = NULL; }

	return 0;
}





int NET_ReadFromReceivingQueue (int *socket, byte *buf, struct sockaddr_in *addr)
{
	int len;
	
	if ( !runReceivingThread ) return 0;

	WaitForSingleObject(hRQMutex, INFINITE);
	if (receivingQueue->m_count > 0)
	{
		receivingQueue->Pop(socket, buf, &len, addr);
	}
	else
		len = 0;
	ReleaseMutex(hRQMutex);

	return len;
}








int NET_WriteToSendingQueue (int socket, byte *buf, int len, struct sockaddr_in *addr)
{
	int ret;

	if ( !runSendingThread ) return -1;

	WaitForSingleObject(hSQMutex, INFINITE); 
	ret = sendingQueue->Push(socket, buf, len, addr);
	ReleaseMutex(hSQMutex);

	return ret;
}



void NET_DestroySendingThread()
{
	runSendingThread = 0;
}

void NET_DestroyReceivingThread()
{
	runReceivingThread = 0;	
}

char *NET_ErrorString ()
{
	int		code;

	code = WSAGetLastError ();
	switch (code)
	{
	case WSAEINTR: return "WSAEINTR";
	case WSAEBADF: return "WSAEBADF";
	case WSAEACCES: return "WSAEACCES";
	case WSAEDISCON: return "WSAEDISCON";
	case WSAEFAULT: return "WSAEFAULT";
	case WSAEINVAL: return "WSAEINVAL";
	case WSAEMFILE: return "WSAEMFILE";
	case WSAEWOULDBLOCK: return "WSAEWOULDBLOCK";
	case WSAEINPROGRESS: return "WSAEINPROGRESS";
	case WSAEALREADY: return "WSAEALREADY";
	case WSAENOTSOCK: return "WSAENOTSOCK";
	case WSAEDESTADDRREQ: return "WSAEDESTADDRREQ";
	case WSAEMSGSIZE: return "WSAEMSGSIZE";
	case WSAEPROTOTYPE: return "WSAEPROTOTYPE";
	case WSAENOPROTOOPT: return "WSAENOPROTOOPT";
	case WSAEPROTONOSUPPORT: return "WSAEPROTONOSUPPORT";
	case WSAESOCKTNOSUPPORT: return "WSAESOCKTNOSUPPORT";
	case WSAEOPNOTSUPP: return "WSAEOPNOTSUPP";
	case WSAEPFNOSUPPORT: return "WSAEPFNOSUPPORT";
	case WSAEAFNOSUPPORT: return "WSAEAFNOSUPPORT";
	case WSAEADDRINUSE: return "WSAEADDRINUSE";
	case WSAEADDRNOTAVAIL: return "WSAEADDRNOTAVAIL";
	case WSAENETDOWN: return "WSAENETDOWN";
	case WSAENETUNREACH: return "WSAENETUNREACH";
	case WSAENETRESET: return "WSAENETRESET";
	case WSAECONNABORTED: return "WSWSAECONNABORTEDAEINTR";
	case WSAECONNRESET: return "WSAECONNRESET";
	case WSAENOBUFS: return "WSAENOBUFS";
	case WSAEISCONN: return "WSAEISCONN";
	case WSAENOTCONN: return "WSAENOTCONN";
	case WSAESHUTDOWN: return "WSAESHUTDOWN";
	case WSAETOOMANYREFS: return "WSAETOOMANYREFS";
	case WSAETIMEDOUT: return "WSAETIMEDOUT";
	case WSAECONNREFUSED: return "WSAECONNREFUSED";
	case WSAELOOP: return "WSAELOOP";
	case WSAENAMETOOLONG: return "WSAENAMETOOLONG";
	case WSAEHOSTDOWN: return "WSAEHOSTDOWN";
	case WSASYSNOTREADY: return "WSASYSNOTREADY";
	case WSAVERNOTSUPPORTED: return "WSAVERNOTSUPPORTED";
	case WSANOTINITIALISED: return "WSANOTINITIALISED";
	case WSAHOST_NOT_FOUND: return "WSAHOST_NOT_FOUND";
	case WSATRY_AGAIN: return "WSATRY_AGAIN";
	case WSANO_RECOVERY: return "WSANO_RECOVERY";
	case WSANO_DATA: return "WSANO_DATA";
	default: return "NO ERROR";
	}
}



int	NET_AddListenChannel( int socket )
{
	if ( channelNumber < MAX_CHANNELS )
	{
		channel[channelNumber] = socket;
		channelNumber ++;
	}
	else
	{
		return 0;
	}

	return 1;
}

int	NET_DeleteListenChannel( int socket )
{
	int i;

	for (i=0; i<channelNumber; i++)
	{
		if ( channel[i] == socket )
		{
			if (i == channelNumber - 1)
				channel[i] = -1;
			else
				channel[i] = channel[channelNumber];

			channelNumber --;
			return 1;
		}
	}

	return 0;
}




int NET_ReadPacket(int *socket, byte *buf, struct sockaddr_in *addr)
{
	int length;

#ifndef _USE_RECEIVING_THREAD
	length = NET_Read(*socket, buf, NET_DATAGRAMSIZE, addr);	
#else
	length = NET_ReadFromReceivingQueue (socket, buf, addr);	
#endif

	return length;
}

int NET_PreviewMessage(i3packet_t *p)
{
	int length;

	length = p->length & NETFLAG_LENGTH_MASK;
	length -= NET_HEADERSIZE;

	if (length < 1) return false;

	SZ_Clear (&recvMessage);
	SZ_Write (&recvMessage, p->data, length);

	return true;
}



void NET_SetMessageOption_EnableOverwriting(i3sizebuf_t *data, int identity)
{
	data->overwriting = identity;
}




#include <tchar.h>

void Crypt(TCHAR *inp, DWORD inplen, TCHAR* key, DWORD keylen)
{
    
    
    TCHAR Sbox[129], Sbox2[129];
    unsigned long i, j, t, x;

    
    static const TCHAR  OurUnSecuredKey[] = "gth99comAAA" ;
    static const int OurKeyLen = _tcslen(OurUnSecuredKey);    
    TCHAR temp , k;
    i = j = k = t =  x = 0;
    temp = 0;

    
    ZeroMemory(Sbox, sizeof(Sbox));
    ZeroMemory(Sbox2, sizeof(Sbox2));

    
    for(i = 0; i < 128U; i++)
    {
        Sbox[i] = (TCHAR)i;
    }

    j = 0;
    
    if(keylen)
    {
        
        for(i = 0; i < 128U ; i++)
        {
            if(j == keylen)
            {
                j = 0;
            }
            Sbox2[i] = key[j++];
        }    
    }
    else
    {
        
        for(i = 0; i < 128U ; i++)
        {
            if(j == OurKeyLen)
            {
                j = 0;
            }
            Sbox2[i] = OurUnSecuredKey[j++];
        }
    }

    j = 0 ; 
    
    for(i = 0; i < 128; i++)
    {
        j = (j + (unsigned long) Sbox[i] + (unsigned long) Sbox2[i]) % 128U ;
        temp =  Sbox[i];                    
        Sbox[i] = Sbox[j];
        Sbox[j] =  temp;
    }

    i = j = 0;
    for(x = 0; x < inplen; x++)
    {
        
        i = (i + 1U) % 128U;
        
        j = (j + (unsigned long) Sbox[i]) % 128U;

        
        
        temp = Sbox[i];
        Sbox[i] = Sbox[j] ;
        Sbox[j] = temp;

        
        t = ((unsigned long) Sbox[i] + (unsigned long) Sbox[j]) %  128U ;

        
        k = Sbox[t];

        
        inp[x] = (inp[x] ^ k);
    }    
}