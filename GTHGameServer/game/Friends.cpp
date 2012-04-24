





#include "..\\global.h"
extern i3sizebuf_t netMessage;
extern i3socket_t localSocket;

#include "../Tools/CTools.h"
extern CTools* gcpTools;









void GTH_FriendsList_Send_FriendsList ( entityPCList_t *pl )
{
	MSG_BeginWriting ( &netMessage ) ;
	MSG_Clear( &netMessage );
	{
		if ( pl->serverIdx == 0 )
		{
			MSG_WriteByte ( &netMessage, GSC_REPLY_FRIENDS );
			MSG_WriteByte ( &netMessage, FRIENDS_LIST );
		}
		else
		{
			MSG_WriteByte ( &netMessage, GSC_REPLY_OTHERSERVER );
			MSG_WriteByte ( &netMessage, PCLIST_FRIENDS_LIST );
			
			MSG_WriteByte ( &netMessage, pl->serverIdx );
			MSG_WriteShort ( &netMessage, pl->idxOnServer );
		}
		
		MSG_WriteByte ( &netMessage, g_friendsList[pl->idx].count );
		for ( int i = 0; i < g_friendsList[pl->idx].count; ++i )
		{
			
			
			entityPCList_t target = { 0, };
			int idx = g_entityListMng.FindPCList ( g_friendsList[pl->idx].names[i], &target );
			
			MSG_WriteByte ( &netMessage, target.level );
			MSG_WriteByte ( &netMessage, target.job );
			MSG_WriteByte ( &netMessage, idx < 0 ? -1 : target.worldIdx );
			MSG_WriteString ( &netMessage, g_friendsList[pl->idx].names[i] );
		}
		
		if ( pl->serverIdx == 0 )
		{
			if ( g_pc[pl->idxOnServer].ready )
			{
				NET_SendMessage ( &g_pc[pl->idxOnServer].sock, &netMessage );
			}
		}
		else
		{
			NET_SendUnreliableMessage ( &g_memberServer[pl->serverIdx].sock, &netMessage );
		}
	}
	MSG_EndWriting(&netMessage);
}



void GTH_FriendsList_Send_FriendsInfo (entityPCList_t *pl, eFriendsState fstate )
{
	for (int i = 0; i < g_friendsList[pl->idx].count; ++i )
	{
		entityPCList_t target = { 0, };
		
		MSG_BeginWriting ( &netMessage );
		MSG_Clear( &netMessage );
		{
			if ( g_entityListMng.FindPCList ( g_friendsList[pl->idx].names[i], &target ) >= 0 )
			{
				
				if ( target.serverIdx == 0 )
				{
					MSG_WriteByte ( &netMessage, GSC_REPLY_FRIENDS );
					MSG_WriteByte ( &netMessage, FRIENDS_CHANGE );
					MSG_WriteByte ( &netMessage, pl->level );
					MSG_WriteByte ( &netMessage, pl->job );
					MSG_WriteByte ( &netMessage, fstate == FriendsLogout ? -1 : pl->worldIdx );
					MSG_WriteString ( &netMessage, pl->name );
					
					if ( g_pc[target.idxOnServer].ready )
					{
						NET_SendMessage ( &g_pc[target.idxOnServer].sock, &netMessage );
					}
				}
				
				else
				{
					MSG_WriteByte ( &netMessage, GSC_REPLY_OTHERSERVER );
					MSG_WriteByte ( &netMessage, PCLIST_FRIENDS_UPDATE );
					
					MSG_WriteByte ( &netMessage, target.serverIdx );
					MSG_WriteShort ( &netMessage, target.idxOnServer );
					
					MSG_WriteByte ( &netMessage, pl->level );
					MSG_WriteByte ( &netMessage, pl->job );
					MSG_WriteByte ( &netMessage, fstate == FriendsLogout ? -1 : pl->worldIdx );
					MSG_WriteString ( &netMessage, pl->name );
					
					NET_SendUnreliableMessage ( &g_memberServer[target.serverIdx].sock, &netMessage );
				}
			}
		}
		MSG_EndWriting(&netMessage);
	}
}








void GTH_FriendsList_Request_List ( playerCharacter_t *pc )
{
	if ( g_config.isManager )
	{
		entityPCList_t pl;

		int idx = g_entityListMng.FindPCList ( pc->name, &pl );
		if ( idx < 0 )
		{
			g_logSystem->Write ("Error: PCList에 없는 유저가 추가요청. userID: %s, name: %s",
				pc->userID, pc->name );
			return;
		}
		else
		{
			
			memcpy (&g_friendsList[idx], &pc->friends, sizeof (FriendsList) );

			
 			GTH_FriendsList_Send_FriendsList ( &pl );

			
			GTH_FriendsList_Send_FriendsInfo ( &pl, FriendsLogin );
		}
	}
	else
	{
		
		GTH_FriendsList_Request_List_OtherServer ( pc );
	}
}


void GTH_FriendsList_Request_Update ( playerCharacter_t *pc, eFriendsState fstate )
{
	if ( g_config.isManager )
	{
		entityPCList_t pl ;
		if ( g_entityListMng.FindPCList ( pc->name, &pl ) < 0 )
		{
			g_logSystem->Write ("Error: g_entityListMng에서 자신을 찾는데 실패."
				"userID: %s, "
				"name: %s",
				pc->userID,
				pc->name );
			return;
		}

		GTH_FriendsList_Send_FriendsInfo ( &pl, fstate );
	}
	else
	{
		GTH_FriendsList_Request_Update_OtherServer ( pc, fstate );
	}
}








void GTH_FriendsList_Request_List_OtherServer ( playerCharacter_t *pc )
{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte ( &netMessage, GSC_REQUEST_OTHERSERVER );
		MSG_WriteByte ( &netMessage, PCLIST_FRIENDS_LIST );
		MSG_WriteString ( &netMessage, pc->name );
		MSG_WriteByte ( &netMessage, pc->friends.count );
		
		for (int i = 0; i < pc->friends.count; ++i )
		{
			MSG_WriteString ( &netMessage, pc->friends.names[i] );
		}
		
		NET_SendUnreliableMessage ( &localSocket, &netMessage );
	}
	MSG_EndWriting(&netMessage);
}


void GTH_FriendsList_Request_Update_OtherServer ( playerCharacter_t *pc, eFriendsState fstate )
{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte ( &netMessage, GSC_REQUEST_OTHERSERVER );
		MSG_WriteByte ( &netMessage, PCLIST_FRIENDS_UPDATE );
		MSG_WriteString ( &netMessage, pc->name );
		MSG_WriteByte ( &netMessage, fstate );
		NET_SendUnreliableMessage ( &localSocket, &netMessage );
	}
	MSG_EndWriting(&netMessage);
}







void GTH_ProcessMessage_FriendsList_Request_List ()
{
	if ( !g_config.isManager )
	{
		g_logSystem->Write (
			"Error: 매니저가 아닌곳에서 GTH_ProcessMessage_FriendsList_Request_List가 호출됨");

		return;
	}

	char targetName[NAMESTRING];
	strcpy (targetName, MSG_ReadString () );

	FriendsList friendsList = { 0, };
	friendsList.count = MSG_ReadByte () ;

	for ( int i = 0 ; i < friendsList.count; ++i )
	{
		strcpy (friendsList.names[i], MSG_ReadString () );
	}

	entityPCList_t pl;
	int idx = g_entityListMng.FindPCList ( targetName, &pl );
	if ( idx < 0 )
	{
		g_logSystem->Write ("Error: PCList에 없는 유저가 추가요청. name: %s", targetName );
		return;
	}
	else
	{
		
		memcpy (&g_friendsList[idx], &friendsList, sizeof (FriendsList) );

		
		GTH_FriendsList_Send_FriendsList ( &pl );

		
		GTH_FriendsList_Send_FriendsInfo ( &pl, FriendsLogin );
	}
}

void GTH_ProcessMessage_FriendsList_Request_Update ()
{
	char targetName[NAMESTRING];
	strcpy ( targetName, MSG_ReadString () );

	eFriendsState fstate = (eFriendsState)MSG_ReadByte ();

	entityPCList_t pl;
	if ( g_entityListMng.FindPCList ( targetName, &pl ) < 0 )
	{
		g_logSystem->Write ("Error: PCLIST_FRIENDS_UPDATE를 받았지만 PCLIST에 없음: name: %s",
			targetName );
		return;
	}
	else
	{
		GTH_FriendsList_Send_FriendsInfo ( &pl, fstate );
	}
}







void GTH_ProcessMessage_Reply_Friends_List ()
{
	if ( g_config.gameServerNo == MSG_ReadByte () )
	{
		int idx = MSG_ReadShort ();
		int count = MSG_ReadByte ();

		FriendEntry friendEntries[MAX_FRIENDS];

		int i;
		for ( i = 0; i < count; ++i )
		{
			friendEntries[i].level = MSG_ReadByte ();
			friendEntries[i].job = MSG_ReadByte ();
			friendEntries[i].worldIdx = MSG_ReadByte ();
			strcpy (friendEntries[i].name, MSG_ReadString () );
		}

		MSG_BeginWriting ( &netMessage );
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte ( &netMessage, GSC_REPLY_FRIENDS );
			MSG_WriteByte ( &netMessage, FRIENDS_LIST );
			MSG_WriteByte ( &netMessage, count );
			
			for ( i = 0; i < count; ++i )
			{
				MSG_WriteByte ( &netMessage, friendEntries[i].level );
				MSG_WriteByte ( &netMessage, friendEntries[i].job );
				MSG_WriteByte ( &netMessage, friendEntries[i].worldIdx );
				MSG_WriteString ( &netMessage, friendEntries[i].name );
			}
			
			if ( g_pc[idx].ready )
			{
				NET_SendMessage ( &g_pc[idx].sock, &netMessage );
			}
		}
		MSG_EndWriting(&netMessage);
	}
}

void GTH_ProcessMessage_Reply_Friends_Update ()
{
	if ( g_config.gameServerNo == MSG_ReadByte () )
	{
		int idx = MSG_ReadShort ();
		
		FriendEntry friendEntry;
		friendEntry.level = MSG_ReadByte ();
		friendEntry.job = MSG_ReadByte ();
		friendEntry.worldIdx = MSG_ReadByte ();
		strcpy ( friendEntry.name, MSG_ReadString () );

		MSG_BeginWriting( &netMessage );
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte ( &netMessage, GSC_REPLY_FRIENDS );
			MSG_WriteByte ( &netMessage, FRIENDS_CHANGE );
			
			MSG_WriteByte ( &netMessage, friendEntry.level );
			MSG_WriteByte ( &netMessage, friendEntry.job );
			MSG_WriteByte ( &netMessage, friendEntry.worldIdx );
			MSG_WriteString ( &netMessage, friendEntry.name );
			
			if ( g_pc[idx].ready )
			{
				NET_SendMessage ( &g_pc[idx].sock, &netMessage );
			}
		}
		MSG_EndWriting(&netMessage);
	}
}


void GTH_Friends_Add ( playerCharacter_t * src, playerCharacter_t * target )
{
	_snprintf (src->friends.names[src->friends.count], NAMESTRING, target->name );
	++src->friends.count;

	
	g_DBGameServer->SaveFriends ( src );

	
	GTH_SendMessage_Friends_Added ( src, target->name, target->level, target->pcJob, target->worldIdx );

	
	GTH_FriendsList_Request_List ( src );
}

void GTH_Friends_Remove ( playerCharacter_t *pc, char* name )
{
	int i;

	
	for ( i = 0 ; i < pc->friends.count ; ++i )
	{
		if ( !stricmp ( pc->friends.names[i], name ) )
		{
			break ;
		}
	}

	
	
	if( i == pc->friends.count ) return;

	
	
	for (++i ; i < pc->friends.count ; ++i )
	{
		strcpy ( pc->friends.names[i - 1], pc->friends.names[i] );
	}
	strcpy ( pc->friends.names[i - 1], "" );

	
	--pc->friends.count;
	if ( pc->friends.count < 0 )
	{
		pc->friends.count = 0 ;
	}

	g_DBGameServer->SaveFriends ( pc );
	GTH_SendMessage_Friends_Remove ( pc, name );

	
	GTH_FriendsList_Request_List ( pc );
}




void GTH_ProcessMessage_MailSystem()
{
	int command = MSG_ReadByte();

	switch( command )
	{
	case MAIL_SEND :
		GTH_ProcessMessage_MailSystem_Send();
		break;
	case MAIL_RECEIVE :
		GTH_ProcessMessage_MailSystem_Receive();
		break;
	case MAIL_DELETE :
		GTH_ProcessMessage_MailSystem_Delete();
		break;
	case MAIL_VIEW :
		GTH_ProcessMessage_MailSystem_View();
		break;
	case MAIL_SAVE :
		GTH_ProcessMessage_MailSystem_Save();
		break;
	}
}


void GTH_ProcessMessage_MailSystem_Send()
{
	char		toName[NAMESTRING+1];
	char		mailTitle[MAIL_TITLESIZE+1];
	char		mailStr[MAIL_STRSIZE+1];
	char		mailTime[32+1];
	time_t		curTime;
	struct tm	*date_tm;

	
    time( &curTime );	
	date_tm = localtime( &curTime );	
	sprintf( mailTime, "%4d-%02d-%02d %02d:%02d:%02d", 
		date_tm->tm_year + 1900, 
		date_tm->tm_mon + 1, 
		date_tm->tm_mday, 
		date_tm->tm_hour, 
		date_tm->tm_min, 
		date_tm->tm_sec);



	sstrncpy( toName, MSG_ReadString(), NAMESTRING - 1 );
	toName[NAMESTRING]=NULL;

	sstrncpy( mailTitle, MSG_ReadString(), MAIL_TITLESIZE );
	mailTitle[MAIL_TITLESIZE]=NULL;

	sstrncpy( mailStr, MSG_ReadString(), MAIL_STRSIZE );
	mailStr[MAIL_STRSIZE]=NULL;

	
	g_DBGameServer->SendMail( g_curPC, toName, mailTitle, mailTime, mailStr );
}







void GTH_ProcessMessage_MailSystem_Receive()
{
	
	g_DBGameServer->RecvMail( g_curPC );
}
void GTH_ProcessMessage_MailSystem_Delete()
{
	int		mailIdx[MAX_MAIL_COUNT];
	int		i, count;

	for( i=0; i<MAX_MAIL_COUNT; i++ )
		mailIdx[i] = -1;

	count = MSG_ReadByte();

	for( i = 0; i < count; i ++ )
	{
		mailIdx[i] = MSG_ReadByte();
	}

	
	g_DBGameServer->DeleteMail( g_curPC, mailIdx );
}
void GTH_ProcessMessage_MailSystem_View()
{
	int		mailIdx;

	mailIdx = MSG_ReadByte();

	
	g_DBGameServer->ViewMail( g_curPC, mailIdx );
}

void GTH_ProcessMessage_MailSystem_Save()
{ 
	int		mailIdx[MAX_MAIL_COUNT];
	int		i, count;

	for( i=0; i<MAX_MAIL_COUNT; i++ )
		mailIdx[i] = -1;

	count = MSG_ReadByte();

	for( i = 0; i < count; i ++ )
	{
		mailIdx[i] = MSG_ReadByte();
	}

	
	g_DBGameServer->SaveMail( g_curPC, mailIdx );
}


void GTH_ProcessMessage_Request_MailSystem()			
{
	int command = MSG_ReadByte();

	switch( command )
	{
		
	case MAIL_ARRIVE :
		{
			char	toName[NAMESTRING];
			char	fromName[NAMESTRING];

			sstrncpy( toName, MSG_ReadString(), NAMESTRING - 1 );
			sstrncpy( fromName, MSG_ReadString(), NAMESTRING - 1 );

			
			GTH_MailSystem_Find_Receiver( toName, fromName );
		}
		break;
	}
}

void GTH_ProcessMessage_Reply_MailSystem()				
{
	int command = MSG_ReadByte();

	switch( command )
	{
		
	case MAIL_ARRIVE :
		{
			int					idx;
			char				fromName[NAMESTRING];
			playerCharacter_t	*receiver;

			idx = MSG_ReadShort();
			sstrncpy( fromName, MSG_ReadString(), NAMESTRING - 1 );
		
			if( idx >= 0 ) receiver = &g_pc[idx];
			else return;
			if( !receiver->ready || !receiver->active ) return;
			
			
			MSG_BeginWriting( &netMessage );
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte( &netMessage, GSC_EXTEND );
				MSG_WriteByte( &netMessage, GSC_MAILSYSTEM );
				MSG_WriteByte( &netMessage, MAIL_ARRIVE );
				MSG_WriteString( &netMessage, fromName );					
				NET_SendMessage( &receiver->sock, &netMessage );
			}
			MSG_EndWriting(&netMessage);
		}
		break;
	}
}


void GTH_MailSystem_Find_Receiver( char* toName, char* fromName )
{
	if( !g_config.isManager ) return;

	entityPCList_t		pl;
	int					idx;

	idx = g_entityListMng.FindPCList ( toName, &pl );
	
	
	if( idx < 0 ) return;

	
	if( pl.serverIdx == 0 )
	{
		playerCharacter_t	*receiver;

		if( pl.idxOnServer >= 0 ) receiver = &g_pc[pl.idxOnServer];
		else return;
		if( !receiver->ready || !receiver->active ) return;

		
		MSG_BeginWriting( &netMessage );
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte( &netMessage, GSC_EXTEND );
			MSG_WriteByte( &netMessage, GSC_MAILSYSTEM );
			MSG_WriteByte( &netMessage, MAIL_ARRIVE );
			MSG_WriteString( &netMessage, fromName );					
			NET_SendMessage( &receiver->sock, &netMessage );
		}
		MSG_EndWriting(&netMessage);
	}
	
	else
	{
		MSG_BeginWriting( &netMessage );
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte( &netMessage, GSC_EXTEND );
			MSG_WriteByte( &netMessage, GSC_MAIL_REPLY );
			MSG_WriteByte( &netMessage, MAIL_ARRIVE );
	
			MSG_WriteShort( &netMessage, pl.idxOnServer );
			MSG_WriteString( &netMessage, fromName );					
		
			NET_SendUnreliableMessage( &g_memberServer[pl.serverIdx].sock, &netMessage );
		}
		MSG_EndWriting(&netMessage);
	}
}





std::deque<WorldChattingEntry *>			g_worldChattingInfo;

void GTH_WorldChatting_Process()
{
	std::deque<WorldChattingEntry *>::iterator	itor;

	WorldChattingEntry		*chat;
	playerCharacter_t		*pc;
	int						i, startPCIdx, count;

	
	if( g_worldChattingInfo.size() == 0 ) return;

	
	itor	=	g_worldChattingInfo.begin();
	chat	=	(*itor);
	

	
	startPCIdx	=	chat->curPCIdx;

	
	if(startPCIdx < 0 ) startPCIdx = 0;

	pc			=	&g_pc[startPCIdx];
	count		=	0;
	
	for( i = startPCIdx; i < MAX_PCS; i ++, pc ++ )
	{
		if( count >= 20 ) break;		

		if( !pc->active || !pc->ready ) continue;
		MSG_BeginWriting( &netMessage );
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte( &netMessage, GSC_CHATMESSAGE );
			MSG_WriteByte( &netMessage, CHATMSG_TYPE_WORLD );				
			MSG_WriteString( &netMessage, chat->name );
			MSG_WriteString( &netMessage, chat->msg );
			NET_SendUnreliableMessage( &pc->sock, &netMessage );
		}
		MSG_EndWriting( &netMessage );
		
		count ++;		
	}
	
	if( i >= MAX_PCS )
	{
		
		
		delete chat;
		g_worldChattingInfo.pop_front();
	}
	else
	{
		chat->curPCIdx	=	i;			
	}
}


void GTH_WorldChatting_Prepare( char* name, char *msg )
{
	WorldChattingEntry	*chat	=	new WorldChattingEntry;
	memset( chat, 0, sizeof( WorldChattingEntry ) );

	sstrncpy( chat->name, name, NAMESTRING - 1 );
	sstrncpy( chat->msg, msg, 113 );

	
	
	chat->curPCIdx		=	0;	

	g_worldChattingInfo.push_back( chat );
}


void GTH_Transmite_Res_WorldChatting(
	playerCharacter_t* pPlayer,
	const int reply,
	const int error)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_WORLDCHATTING );
		MSG_WriteByte( &netMessage, reply );
		MSG_WriteByte( &netMessage, error );
		MSG_WriteLong( &netMessage, pPlayer->curChargeSE );

		if(gGameConfigFlag.m_bWorld_Chat_Limit && !error){
			MSG_WriteLong( &netMessage,pPlayer->PremiumInfo.WorldChatting.iDecreaseCount );
		}
		NET_SendMessage( &(pPlayer->sock), &netMessage );
	}
	MSG_EndWriting( &netMessage );
}
	
	
void GTH_Transmite_GSC_WORLDCHATTING_REPLY_toAllMemberServer(const char* pName,const char* pMsg)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_WORLDCHATTING_REPLY );

		MSG_WriteString( &netMessage, (char*)pName );
		MSG_WriteString( &netMessage, (char*)pMsg);

		for( int i = 1; i < MAX_MEMBER_SERVER; i ++ )		
		{
			if( !g_memberServer[i].active )	continue;		
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting( &netMessage );

}
	
void GTH_Transmite_GSC_WORLDCHATTING_REQUEST_toManagerServer(const char* pName,const char* pMsg)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_WORLDCHATTING_REQUEST );

		
		MSG_WriteByte( &netMessage, g_config.gameServerNo );
		MSG_WriteString( &netMessage, (char*)pName );
		MSG_WriteString( &netMessage, (char*)pMsg );

		NET_SendUnreliableMessage( &localSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );
}
	

void GTH_ProcessMessage_WorldChatting()
{
	char			name[NAMESTRING+1];
	char			msg[CHATSTRING+1];
	BOOL            bSpeaker=FALSE;
	int			    iInventoryPosIdx=-1;
	
	
	strncpy(name,g_curPC->name,	NAMESTRING);
	name[NAMESTRING]=NULL;

	bSpeaker = (BOOL)MSG_ReadByte();
	iInventoryPosIdx = MSG_ReadByte();
	strncpy(msg,MSG_ReadString(),CHATSTRING);
	msg[CHATSTRING]=NULL;	


	if(FALSE == bSpeaker){
		
		if( g_curPC->curChargeSE < g_logic.worldChattingPrice )
		{
			
			GTH_Transmite_Res_WorldChatting(g_curPC,0,WORLD_CHATTING_ERROR_SE);
			return;
		}
		
		
		
		if(gGameConfigFlag.m_bWorld_Chat_Limit && g_curPC->PremiumInfo.WorldChatting.iDecreaseCount <= 0)
		{		
			GTH_Transmite_Res_WorldChatting(g_curPC,0,WORLD_CHATTING_ERROR_COUNT);
			return;
		}	

		
		g_curPC->curChargeSE	-=	g_logic.worldChattingPrice;		
		if(gGameConfigFlag.m_bWorld_Chat_Limit)
		{
			if(g_curPC->PremiumInfo.iMemberShipType == playerCharacter_t::tagPremiumInfo::enumMeberShipType::Free){
				g_curPC->PremiumInfo.WorldChatting.iDecreaseCount --;		
			}
		}
	}else{
		gcpTools->DeleteItemFromMyInventory(g_curPC,iInventoryPosIdx,574);
	}

	GTH_Transmite_Res_WorldChatting(g_curPC,1,0);


	
	if( g_config.isManager ) GTH_Transmite_GSC_WORLDCHATTING_REPLY_toAllMemberServer(name,msg);
	
	else GTH_Transmite_GSC_WORLDCHATTING_REQUEST_toManagerServer(name,msg);	
	

	
	GTH_WorldChatting_Prepare( name, msg );	
}























	




void GTH_ProcessMessage_Request_WorldChatting()
{
	char			name[NAMESTRING];
	char			msg[CHATSTRING];
	int				exceptServerNO;

	exceptServerNO	=	MSG_ReadByte();
	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( msg, MSG_ReadString(), 113 );
	
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_WORLDCHATTING_REPLY );
		
		MSG_WriteString( &netMessage, name );
		MSG_WriteString( &netMessage, msg );
		
		for( int i = 1; i < MAX_MEMBER_SERVER; i ++ )		
		{
			if( !g_memberServer[i].active )	continue;		
			if( i == exceptServerNO ) continue;				

			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting( &netMessage );

	
	GTH_WorldChatting_Prepare( name, msg );	
}


void GTH_ProcessMessage_Reply_WorldChatting()
{
	char			name[NAMESTRING];
	char			msg[CHATSTRING];

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );
	sstrncpy( msg, MSG_ReadString(), 113 );

	
	GTH_WorldChatting_Prepare( name, msg );	
}

void GTH_ProcessMessage_WorldChattingCount()
{
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_WORLDCHATTING_COUNT );
		MSG_WriteByte( &netMessage, g_curPC->PremiumInfo.WorldChatting.lDate );
		MSG_WriteByte( &netMessage, g_curPC->PremiumInfo.WorldChatting.iMaxCount);
		MSG_WriteLong( &netMessage, g_curPC->PremiumInfo.WorldChatting.iDecreaseCount );

		NET_SendMessage( &g_curPC->sock, &netMessage );
	}
	MSG_EndWriting( &netMessage );

}