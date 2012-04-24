#ifndef _NETLIB_H_
#define _NETLIB_H_

#pragma warning( disable : 4786 )
#pragma warning( disable : 4251 )	

#define NETLIB_VERSION	"03.10.03"


#include <stdio.h>
#include <math.h>
#include <ctime>
#include <map>
#include <stack>

#include <winsock.h>








#define _COMPILE_FOR_CLIENT

#ifdef _COMPILE_FOR_SERVER
#define _USE_SENDING_THREAD
#define _USE_RECEIVING_THREAD
#endif



#define NET_MAXMESSAGE					8192

#define MAX_DATAGRAM					1024  

#define NET_HEADERSIZE					(sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int) +sizeof(unsigned short)) 
#define NET_DATAGRAMSIZE				(MAX_DATAGRAM + NET_HEADERSIZE)
#define NET_MAXWAITINGMESSAGE			8	


#define NETFLAG_LENGTH_MASK				0x0fff

#define NETFLAG_SOM						1 << 0   
#define NETFLAG_RELIABLE				1 << 1   
#define NETFLAG_ACK						1 << 2   
#define NETFLAG_EOM						1 << 3   

#define NETFLAG_ENCRYPT					1 << 0   
#define NETFLAG_ENCRYPT_CHECKSUM		0X7FFFFFFF 


#define NET_RESENDTIME					800		

#define NET_MAXIMUM_PACKETMAP_SIZE		100		

#define NET_SENDTIME_UNRELIABLE			0



#define MAXHOSTNAMELEN		256

#define MAX_CHANNELS		20		



typedef float   vec3_t[3];

typedef struct 
{
	int	allowoverflow;	
	int overflowed;		
	byte    data[NET_MAXMESSAGE];
	int		maxsize;
	int		cursize;
	int		overwriting; 
} i3sizebuf_t;

#pragma pack ( push , 1 )
typedef struct
{
	unsigned int	sequence;	
	unsigned int	encrypt :1 ;
	unsigned int	checksum : 31;
	unsigned int	lastSendTime;			
	
	unsigned short	length;	
	byte			data[MAX_DATAGRAM];
} i3packet_t;
#pragma pack ( pop )


typedef std::map<UINT, i3packet_t*>			PacketMap;
typedef PacketMap::iterator					PacketMapItor;
typedef PacketMap::value_type				PacketMapPair;

enum
{
	SocketType_Reliable,
	SocketType_Unreliable
};

enum
{
	PacketBlock_None,
	PacketBlock_Wait,
	
};

typedef struct i3socket_s 
{
	i3socket_s()
	{
		sendPacketMap	=	NULL;
		recvPacketMap	=	NULL;
		socket			=	INVALID_SOCKET;
		socketType		=	SocketType_Reliable;
	}
	int				socket;
	int				socketType;

	unsigned int	connecttime;
	unsigned int	lastReceiveTime;			
	unsigned int	lastSendTime;				
	unsigned int	latencyTime;				

	int				disconnected;
	int				canSend;
	int				sendNext;

	unsigned int    resendSequence;              

	unsigned int	ackSequence;
	unsigned int	sendSequence;
	unsigned int	unreliableSendSequence;
	int				sendMessageLength;
	byte			sendMessage [NET_MAXMESSAGE];

	unsigned int	receiveSequence;
	unsigned int	unreliableReceiveSequence;
	int				receiveMessageLength;
	byte			receiveMessage [NET_MAXMESSAGE];

	int				packetBlockSequence;

	struct sockaddr_in	addr;

	
	PacketMap		*sendPacketMap;
	
	PacketMap		*recvPacketMap;
	
} i3socket_t;

enum
{
	PacketAnalysis_None		=	0,		
	PacketAnalysis_Skip,				
	PacketAnalysis_Ack,					
	PacketAnalysis_Start,				
};



void SZ_Write (i3sizebuf_t *buf, void *data, int length);	
void SZ_Clear (i3sizebuf_t *buf);							
void SZ_Alloc (i3sizebuf_t *buf, int startsize);
unsigned char *SZ_GetSpace (i3sizebuf_t *buf, int length);
void SZ_Load (i3sizebuf_t *buf, void *data);





void MSG_BeginWriting(i3sizebuf_t *sb);
void MSG_EndWriting(i3sizebuf_t *sb);
void MSG_Clear( i3sizebuf_t *sb );

void MSG_WriteChar (i3sizebuf_t *sb, int c);
void MSG_WriteByte (i3sizebuf_t *sb, int c);
void MSG_WriteShort (i3sizebuf_t *sb, int c);
void MSG_WriteLong (i3sizebuf_t *sb, int c);
void MSG_Write64Int (i3sizebuf_t *sb, __int64 c);
void MSG_WriteFloat (i3sizebuf_t *sb, float f);
void MSG_WriteString (i3sizebuf_t *sb, char *s);
void MSG_WriteAngle (i3sizebuf_t *sb, float f);
void MSG_WriteVector(i3sizebuf_t *sb, vec3_t v);

void MSG_BeginReading (void);
int MSG_ReadChar (void);
int MSG_ReadByte (void);
int MSG_ReadShort (void);
int MSG_ReadLong (void);
__int64 MSG_Read64Int (void);
float MSG_ReadFloat (void);
char *MSG_ReadString (void);
float MSG_ReadAngle (void);
void MSG_ReadVector(vec3_t v);



int NET_AddrCompare (struct sockaddr *addr1, struct sockaddr *addr2);
char *NET_AddrToString (struct sockaddr *addr);
int NET_StringToAddr (char *string, struct sockaddr *addr);

char *NET_GetLocalAddress();
char *NET_GetHostName();

char *NET_AddrToIPString (struct sockaddr *addr);


int NET_SendPacket (i3socket_t *sock, i3sizebuf_t *data, BOOL encrypt = FALSE);
int NET_SendPacket_Unreliable(i3socket_t *sock, i3sizebuf_t *data, BOOL encrypt = FALSE);
int NET_ReadPacket(int *socket, byte *buf, struct sockaddr_in *addr);




int NET_ControlPacket(i3socket_t *sock, i3packet_t *packet);
int NET_ControlRecvBank( i3socket_t *sock );
int NET_ControlSendBank( i3socket_t *sock );
int NET_CheckSockProblem( i3socket_t *sock, int checkTime = -1 );
int NET_PreviewMessage(i3packet_t *p);




int NET_Write (int socket, byte *buf, int len, struct sockaddr_in *addr);
int NET_Read (int socket, byte *buf, int len, struct sockaddr_in *addr);


int NET_ReadFromReceivingQueue (int *socket, byte *buf, struct sockaddr_in *addr);
int NET_WriteToSendingQueue (int socket, byte *buf, int len, struct sockaddr_in *addr);




void NET_InitNetTime();
unsigned int NET_GetNetTime();



int NET_OpenNetwork();
void NET_InitPacketMap( i3socket_t *sock );
int NET_InitSocket(i3socket_t *sock, struct sockaddr_in *addr, int socket, int socketType );
int NET_OpenSocketForServer(i3socket_t *sock, char *serverIP, int port, int socketType = SocketType_Reliable );
int NET_OpenSocketForClient(i3socket_t *sock, char *serverIP, int port, int socketType = SocketType_Reliable );
void NET_CloseSocket(i3socket_t *sock);
void NET_ClearSocket(i3socket_t *sock);
void NET_CloseNetwork();





int NET_CreateSendingThread();
int NET_CreateReceivingThread();
void NET_DestroySendingThread();
void NET_DestroyReceivingThread();

DWORD WINAPI NET_SendingThread(LPVOID param);
DWORD WINAPI NET_ReceivingThread(LPVOID param);



char *NET_ErrorString ();
int	NET_AddListenChannel( int socket );
int	NET_DeleteListenChannel( int socket );

#define NET_SendMessage					NET_SendPacket
#define NET_SendUnreliableMessage		NET_SendPacket_Unreliable


void Crypt(TCHAR *inp, DWORD inplen, TCHAR* key = "", DWORD keylen = 0);

#endif
