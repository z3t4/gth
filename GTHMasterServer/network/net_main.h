#define RELIABLE_WAITTIME   1000
#define DISCONNECT_WAITTIME 60000      
#define MAX_RESENDING_TIMES   10       

#define MAX_CLIENTS			1500          




#define MAX_CHARACTER_DATA	6	

#include "hicocoon/hicocoonctrl.h"
typedef struct 
{
	public:
		struct tagIdentification{
			public:
				enum enumCertification{
					nick_name=0,
					real_name=1,
					developer=2,
					unknown=3,
				};
				
			public:
				enum enumCertification Certification;
				int  PayLevel;
		};

		struct tagPremiumInfo{
			public:
				enum{
					maxmap_number=30,										
					max_charactor_slot_num=3,
				};

				enum enumMeberShipType{
					Free=0,
					Pay,
					Max,
				};



				struct tagMap{
					struct tagAttr
					{
						BOOL    bAllow;													
						int		iRemainSecond;
						BOOL    bDecreseRemainSecond;									
						BOOL	bDecreseAllRemainSecond;

						
						float	fAddExpRatio;											
						float	fAddGenExpRatio;										
						float	fAddGenCapabilityRatio;									
						float	fAddItemDropRatio;										
						float	fAddNarkRatio;											
						
					};

					struct tagAttr Default;
					struct tagAttr Cur;
				};

				struct tagGamble{
					public:
						enum{
							maxnum_apperance_item_group=16,							
						};
						struct tagItemGroup{
							BOOL bApperance;
						};
					public:
						float fAddPriceRatio;												
						struct tagItemGroup ItemGroupArray[maxnum_apperance_item_group];	
				};

				
				struct tagDie{
					float fExpRatio;												
					float fNakRatio;												
					
				};
				

				struct tagItemOptionUpgrade{
					int   iMaxlevel;												
					float fAddPriceRatio;											
				};


				
				
				struct tagStatus{
					int iDefaultInitCount;	
					int iInitCount;			
				};
				
				
				
				struct tagGonyounPracticeBattle{
					long lDate;		
					int  iMaxCount;	
					int  iDecreseCount; 
				};

				struct tagWorldChatting{
					long lDate;
					int  iMaxCount;
					int  iDecreaseCount;
				};
				

			public:		
				enum enumMeberShipType			iMemberShipType;

				int								iRemainSecond;
				struct tagMap					Map[maxmap_number];				
				struct tagGamble				Gamble;
				struct tagItemOptionUpgrade		ItemOptionUpgrade;		
				struct tagDie					Die;		
				BOOL							bCreatePremiumBooth;	 
				int								iMaxLevelItemCraft;		 
				float							fItemPrecocityTimeRatio; 
				BOOL							bItemRecycle;			 
				struct tagGonyounPracticeBattle GonyounPracticeBattle;
				struct tagWorldChatting			WorldChatting;
				struct tagStatus				Status;			
		};

		struct tagTerra{		
			public:
				enum{
					MaxBytesTLoginName=20,
				};

			public:
				char TLoginName[MaxBytesTLoginName+1];
		};

		
	public:
		int				idx;
		int				active;		
		int				isMonitor;	
		char			id[IDSTRING+1];
		int				serverGroupID;		
		char			name[SERVERSTRING];
		int				dataNumber;	
		characterData_t	data[MAX_CHARACTER_DATA];
		characterData_t createData;
		int				deleteCharacterID;
		int				dbCheckCharacterName;
		int				dbCheckAccount;
		int				dbCheckID;
		int				isComplete;
		int				isSuccessLogin;
 		i3socket_t		sock;
		
		
		char			billMethod[2] ;			
		int				billRemain;
		char			billExpire[12] ;	

		
		int				m_nCharacterSlotType;
		struct tm		m_tmDate;

		struct tagIdentification Identify;
		struct tagPremiumInfo    PremiumInfo;
		struct tagTerra			 Terra;	

}i3client_t;





typedef struct
{
	int				idx;
	int				active;

	char			name[NAMESTRING];
	char			hostName[HOSTNAMESTRING];	
	char			serviceIP[IPSTRING];		
	int				servicePort;				
	char			localIP[IPSTRING];		
	int				localPort;				

	int				worldNumber;			
	int				worldList[MAX_SERVICE_WORLD]; 
	int				clientNumber;			
	int				maxClientNumber;		
} memberServer_t;


typedef struct 
{
	int				idx;						
	int				active;	

	char			name[NAMESTRING];			
	char			DSN[NAMESTRING];		
	int				totalClient;
	int				maxTotalClient;
	int				memberServerNumber;
	memberServer_t	memberServer[MAX_MEMBER_SERVER];

	unsigned int	updateTime;
	i3socket_t		sock;
} serverGroup_t;

typedef struct 
{
	int					idx;						
	int					active;

	char				hostName[NAMESTRING];
	char				serviceIP[IPSTRING];
	int					servicePort;
	char				localIP[IPSTRING];
	int					localPort;

	char 				DSN[NAMESTRING];
	char				gameVersion[VERSIONSTRING];
	int					filterYn; 
	int					clientNumber;
	int					maxClientNumber;

	int					concurrentClientNumber;			
	int					maxConcurrentClientNumber;

	unsigned int		updateTime;
} masterServer_t;



void GTH_InitMasterServer();
int GTH_MasterServer();

int GTH_Network_Startup();
void GTH_Network_Shutdown();

int GTH_FindClientWithAddr(struct sockaddr_in *addr);
int GTH_IsValidPacketForNewClient(i3packet_t *p);

int GTH_CreateClient(struct sockaddr_in *addr);
int GTH_RemoveClient(i3client_t *client);

int	GTH_Network_GetMessage(i3socket_t *sock);

void GTH_ConnectMasterServer(void);
int GTH_DisconnectMasterServer();
int GTH_AddGameServer(char *svname, char *ip, int port);
int GTH_RemoveGameServer(char *svname);
int GTH_FindGameServer(char *svname);

void GTH_RequestLogin(void);

#include "tagMCPacket.h"
void GTH_ReplyLoginV2(
		i3client_t* pClient,
		const BOOL bSUCCESSorFAIL,
		enum tagMCPaket_MSC_REPLYLOGIN::enumError enumErrorCode);				
					



void GTH_ReplyLogin_Hicocoon(i3client_t* pClient,
		const BOOL bSUCCESSorFAIL,
		enum tagMCPaket_MSC_REPLYLOGIN::enumError enumErrorCode);

int GTH_SendMessage_GameServerGroup();
int GTH_GameServer();


void GTH_CheckProblemSocket();

void GTH_Debug_PacketInfo();
int GTH_ServerGroupInfo();
int GTH_DropServerGroup();


BOOL GTH_RequestGameServerAddr(void);
int GTH_FindServerWithinGroup(serverGroup_t *group, int worldIdx);
void GTH_SendGameData( i3client_t *client );

void GTH_RequestGameData();

void GTH_SendMessage_AcceptClient();
void GTH_SendMessage_RejectClient( int reason );

void GTH_SendMessage_ReliablePing( i3socket_t *sock );
void GTH_SendMessage_UnreliablePing( i3socket_t *sock );


void GTH_RequestReadyCreate();
int GTH_RequestCreateCharacter();
void GTH_SendMessage_ReplyCreateCharacter(i3client_t *client, int reply);
int GTH_RequestDeleteCharacter();
void GTH_SendMessage_ReplyDeleteCharacter(i3client_t *client, int reply);
int GTH_RequestCheckCharacterName();
void GTH_ReplyCheckCharacterName(i3client_t *client, int reply);



int GTH_AddToServerGroup(int serverGroupIdx, struct sockaddr_in *addr);
int GTH_GetServerGroupIdx(i3packet_t *p);
int GTH_FindServerGroupWithAddr(struct sockaddr_in *addr);


void GTH_ProcessMessage_Monitor();
void GTH_Fill_Monitor_ServerInfo(int startIdx) ;
void GTH_SendMessage_Monitor_ServerInfo( i3client_t *client );
int GTH_CalculateConcurrentClients();
int GTH_GetNumberActiveServerGroup();
int GTH_GetNumberActiveMemberServer(int groupIdx);


void GTH_Debug_ServerStatusInfo();


void GTH_LoadCharacterSlot();
void GTH_SaveCharacterSlot();

void GTH_SendCharacterLoadSlot(i3client_t *client,int day);


BOOL CheckStatusValue(charEditData_t* pEditData);




BOOL GTH_CheckValidName( char *name );
BOOL IsVaildString(char *str);
