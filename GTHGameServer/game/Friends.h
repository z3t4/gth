





#pragma once


void GTH_Friends_Add ( playerCharacter_t * src, playerCharacter_t * target );
void GTH_Friends_Remove ( playerCharacter_t *pc, char *name );



enum eFriendsState { 
	FriendsLogin = 0,
	FriendsLogout,
	FriendsUpdate,
} ;

enum
{
	CHATMSG_TYPE_NORMAL	= 0,
	CHATMSG_TYPE_PARTY,
	CHATMSG_TYPE_DAN,
	CHATMSG_TYPE_TRADE,
	CHATMSG_TYPE_WHISPER,
	CHATMSG_TYPE_WORLD,
	CHATMSG_TYPE_SYSTEM,
};

void GTH_FriendsList_Send_FriendsList ( entityPCList_t *pl );
void GTH_FriendsList_Send_FriendsInfo (entityPCList_t *pl, eFriendsState fstate );


void GTH_FriendsList_Request_List ( playerCharacter_t *pc );
void GTH_FriendsList_Request_Update ( playerCharacter_t *pc, eFriendsState fstate );


void GTH_FriendsList_Request_List_OtherServer ( playerCharacter_t *pc );
void GTH_FriendsList_Request_Update_OtherServer ( playerCharacter_t *pc, eFriendsState fstate );

void GTH_ProcessMessage_FriendsList_Request_List ();
void GTH_ProcessMessage_FriendsList_Request_Update ();

void GTH_ProcessMessage_Reply_Friends_List ();
void GTH_ProcessMessage_Reply_Friends_Update ();




void GTH_ProcessMessage_MailSystem();
void GTH_ProcessMessage_MailSystem_Send();
void GTH_ProcessMessage_MailSystem_Receive();
void GTH_ProcessMessage_MailSystem_Delete();
void GTH_ProcessMessage_MailSystem_View();
void GTH_ProcessMessage_MailSystem_Save();

void GTH_ProcessMessage_Request_MailSystem();
void GTH_ProcessMessage_Reply_MailSystem();
void GTH_MailSystem_Find_Receiver( char* toName, char* fromName );



struct WorldChattingEntry
{	
	int				curPCIdx;			
	char			name[NAMESTRING];	
	char			msg[256];			
};

enum
{
	WORLD_CHATTING_ERROR_SE=1,				
	WORLD_CHATTING_ERROR_COUNT,				
};





void GTH_WorldChatting_Process();
void GTH_WorldChatting_Prepare( char* name, char *msg );
void GTH_ProcessMessage_WorldChatting();
void GTH_ProcessMessage_Request_WorldChatting();
void GTH_ProcessMessage_Reply_WorldChatting();




void GTH_ProcessMessage_WorldChattingCount();
