








#pragma once





typedef struct _PACKET_GAME
{
	int		PacketType ;			
	int		PacketResult ;			
	char	GameServer[32] ;		
	char	Session[32] ;			
	char	UserCC[4] ;				
	char	UserNo[20] ;			
	char	UserID[40] ;			
	char	UserIP[24] ;			
	char	UserGender ;			
	char	UserStatus[3] ;			
	char	UserPayType[4] ;		
	int		UserAge ;				
	int		GameNo ;				
	char	BillPayType[2] ;		
	char	BillMethod[2] ;			
	char	BillExpire[12] ;		
	int		BillRemain ;			
	
} PACKET_GAME, *PPACKET_GAME ;


enum PacketType
{
	GameStart = 1,				
	GameEnd = 2,				
	SeverAlive = 3,				
	ServerReset = 4,
	UserAlert = 5,
	UserSync = 6,
	BillingAuth = 8,
	ServerConnection = 0x0C,	
} ;





bool InitBilling () ;
void ProcessBillingQueue () ;
void Billing_WrongPacket (PPACKET_GAME pPacket) ;
void Billing_ProcessServerAlive (PPACKET_GAME pPacket) ;
void Billing_SendAuth(LPSTR UserID);
void Billing_ProcessAuth (PPACKET_GAME pPacket) ;
int	 Billing_FindClientbyUserID ( char *userID ) ;



