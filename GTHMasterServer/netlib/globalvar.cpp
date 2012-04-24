#include "netlib.h"
#include "queue.h"
#include <time.h>





i3sizebuf_t		netMessage;
i3sizebuf_t		recvMessage;
i3packet_t		packetBuffer;


int				msgReadCount;
int				msgBadRead;


int				totalPacketsSend;				
int				totalPacketsReceived;			

int				packetsSent;					
int				packetsReceived;				
int				packetsReSent;					
int				packetsDropCount;				
int				packetsDupCount;				

int				packetsAckSent;					
int				packetsAckReceived;				

int				un_packetsSent;					
int				un_packetsReceived;				
int				un_packetsDropCount;			


unsigned int	g_netTime;
unsigned int	prevTime, curTime;


int				channel[ MAX_CHANNELS ];
int				channelNumber;


HANDLE			hThread;
DWORD			SendingThreadID, ReceivingThreadID;
int				runSendingThread, runReceivingThread;

CSendingQueue	*sendingQueue;
CReceivingQueue	*receivingQueue;

HANDLE			hSQMutex, hRQMutex;


FILE *packetErrorLog;


int				readSizePerSecond;
int				lastRecvSequence;
int				myRecvSequence;



CRITICAL_SECTION	spCrit;
