

#define		PARTY_HOLDING_CHECK_TIME		1000
#define		PARTY_HOLDING_DELAY_TIME		30000

extern int g_partyTime;



typedef struct
{
	int					idx;
	
	char				organizerName[NAMESTRING];
	int					organizeServer;
	int					organizeTime;

	int					memberNumber;					
	int					itemDivideType;					
	int					canJoin;						
	partyMember_t		member[MAX_NUMBER_OF_PARTY_MEMBER];	

	int					isHolding;						
	float				holdingCheckTime;				
	
	bool				m_bischange;					
    int                 m_iorderindex; 
	
} serverPartyInfo_t;
 
void GTH_InitPartyInfo();
void GTH_InitPartyInfo( serverPartyInfo_t *p );
int GTH_GetEmptyPartyInfo();
int GTH_GetPartyInfo( char* organizerName, int organizeServer, int organizeTime );
int GTH_GetEmptyMemberIdx( serverPartyInfo_t *p );
int GTH_FindMemberIdx( serverPartyInfo_t *p, char* memberName );

void GTH_SetServerPartyAckMessage( playerCharacter_t *pc, int errorFlag );
void GTH_ClearServerPartyAckMessage( playerCharacter_t *pc );
int GTH_CheckServerPartyAckMessage( playerCharacter_t *pc );
int GTH_CheckSameParty( playerCharacter_t *pc1, playerCharacter_t *pc2 );

void GTH_PartyInfo_Organize( partyMember_t* l, partyMember_t* m, float organizeTime );
void GTH_PartyInfo_Request_Organize( playerCharacter_t *leader, playerCharacter_t *firstMember );
void GTH_PartyInfo_Reply_Organize( partyMember_t* l, partyMember_t* m, int partyIdx, float organizeTime, int isLeader );


void GTH_PartyInfo_Join( partyMember_t* m, char* organizerName, int organizeServer, int organizeTime );
void GTH_PartyInfo_Request_Join( playerCharacter_t *leader, playerCharacter_t *newMember );
void GTH_PartyInfo_Reply_Join( serverPartyInfo_t* pi, int currentMemberIdx, int isNewMember );

void GTH_PartyInfo_Out( int isForced, char* memberName, char* organizerName, int organizeServer, int organizeTime );
void GTH_PartyInfo_Request_Out( playerCharacter_t *pc );
void GTH_PartyInfo_Request_ForcedOut( playerCharacter_t* leader, char* memberName );
void GTH_PartyInfo_Reply_Out( int isForced, partyMember_t *m, int reqMemberIdx, int newLeaderIdx, int reqFlag );

void GTH_PartyInfo_Disorganize( char *organizerName, int organizeServer, int organizeTime );
void GTH_PartyInfo_Request_Disorganize( playerCharacter_t *pc );
void GTH_PartyInfo_Reply_Disorganize( partyMember_t *m );


void GTH_PartyInfo_SendAll( int serverIdx, int pcIdx, char* organizerName, int organizeServer, int organizeTime );
void GTH_PartyInfo_Request_SendAll( playerCharacter_t *pc );
void GTH_PartyInfo_Reply_SendAll( int reply, int pcIdx, serverPartyInfo_t *pi );

void GTH_PartyInfo_Update( char* memberName, int active, int serverIdx, int pcIdx, int level, int worldIdx, char* organizerName, int organizeServer, int organizeTime );
void GTH_PartyInfo_Request_Update( playerCharacter_t *pc, int active);
void GTH_PartyInfo_Reply_Update( partyMember_t *m, int memberIdx, int nextLeaderIdx, int active, int serverIdx, int pcIdx, int level, int worldIdx );

void GTH_PartyInfo_Request_ItemDivideType( playerCharacter_t *pc, int type );
void GTH_PartyInfo_ItemDivideType( char* organizerName, int organizeServer, int organizeTime, int type );
void GTH_PartyInfo_Reply_ItemDivideType( partyMember_t *m_, int type );


void GTH_PartyInfo_Request_Holding( playerCharacter_t *pc, int holdingFlag );
void GTH_PartyInfo_Holding( char* organizerName, int organizeServer, int organizeTime, int holdingFlag );
void GTH_PartyInfo_CheckHolding();

void GTH_PartyInfo_Request_ChatMessage( playerCharacter_t *pc, char* message );
void GTH_PartyInfo_ChatMessage( char* fromName, char* organizerName, int organizeServer, int organizeTime, char* message );
void GTH_PartyInfo_Reply_ChatMessage( partyMember_t *m, char *fromName, char* message );

