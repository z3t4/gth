 #include "../global_def.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 




   
int GTH_ProcessMessage_Friends()
{
	int command;
	
	command = MSG_ReadByte();

	switch( command )
	{
	case FRIENDS_INVITE:			
		GTH_ProcessMessage_Friends_Invite();
		break;
	case FRIENDS_REJECT:				
		GTH_ProcessMessage_Friends_Reject();
		break;
	case FRIENDS_REMOVE:				
		GTH_ProcessMessage_Friends_Remove();
		break;
	case FRIENDS_LIST:				
		GTH_ProcessMessage_Friends_List();
		break;
	case FRIENDS_ADD:				
		GTH_ProcessMessage_Friends_Add();
		break;
	case FRIENDS_CHANGE:				
		GTH_ProcessMessage_Friends_Change();
		break;
	}
	return 1;
}

int GTH_SendMessage_Friends_Invite( int idx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_FRIENDS );
		
		MSG_WriteByte(&netMessage, FRIENDS_INVITE );
		MSG_WriteShort(&netMessage, idx );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

void GTH_ProcessMessage_Friends_Invite()
{
	int reply = 1, error = 0, type;

	reply = MSG_ReadByte();
	if( reply == 1 )
	{
		char szTemp[128];
		char name[NAMESTRING];

		g_cgv.myCharacterInfo->tempFriendIdx = MSG_ReadShort();
		type = MSG_ReadByte();
		strcpy( name, MSG_ReadString() );
		
		
		
		
		
		
		if( type ) 
		{	
			
			sprintf( szTemp, g_LPACK.GetMessage(0, 725), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_CANCEL, IF_MESSAGE_FRIENDS_REQUEST_INVITE, 10000.0f );
		}
		else 
		{
			sprintf( szTemp, g_LPACK.GetMessage(0, 726), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_FRIENDS_REPLY_INVITE, 10000.0f );
		}

		
	}
	else 
	{
		error = MSG_ReadByte();

		switch( error )
		{
		case FRIENDS_ERROR_BUSY:
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), g_LPACK.GetMessage(0, 727) );
			break;
		case FRIENDS_ERROR_NOT_READY:
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), g_LPACK.GetMessage(0, 729) );
			break;
		case FRIENDS_ERROR_WRONG_USER:
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), g_LPACK.GetMessage(0, 729) );
			break;
		default:
			
			break;
		}
		
		
		
	}
	
}

void GTH_SendMessage_Friends_Accept(int idx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_FRIENDS );
		
		MSG_WriteByte(&netMessage, FRIENDS_ACCEPT );
		MSG_WriteShort(&netMessage, idx );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_Friends_Reject(int idx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_FRIENDS );
		
		MSG_WriteByte(&netMessage, FRIENDS_REJECT );
		MSG_WriteShort(&netMessage, idx );
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_Friends_Reject()
{
	
	
	char szTemp[256];
	int idx = MSG_ReadShort();

	

	if( g_ifMng->m_msgType == IF_MESSAGE_FRIENDS_REQUEST_INVITE || 
		g_ifMng->m_msgType == IF_MESSAGE_FRIENDS_REPLY_INVITE )
	{
		g_ifMng->ExitMessage();
	}
	
	memset( szTemp, 0, sizeof(szTemp) );
	
	if( g_cgv.targetName[0] != 0 )
	{
		sprintf( szTemp, g_LPACK.GetMessage(0, 730), g_cgv.targetName );
	}

	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );
}

void GTH_ProcessMessage_Friends_Add()
{
	char szTemp[128];
	friend_t newFriend;

	if( g_ifMng->m_msgType == IF_MESSAGE_FRIENDS_REQUEST_INVITE || 
		g_ifMng->m_msgType == IF_MESSAGE_FRIENDS_REPLY_INVITE )
	{
		g_ifMng->ExitMessage();
	}

	newFriend.level = MSG_ReadByte();
	newFriend.job = MSG_ReadByte();
	newFriend.worldIdx = MSG_ReadByte();
	strcpy( newFriend.name, MSG_ReadString() );

	g_ifMng->m_friendWin->AddFriend(&newFriend);

	sprintf( szTemp, g_LPACK.GetMessage(0, 731), newFriend.name );
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );
}

void GTH_ProcessMessage_Friends_Remove()
{
	char name[NAMESTRING];
	char szTemp[128];

	
	
	g_ifMng->m_friendWin->m_selectedFriendName[0] = 0;
	g_ifMng->m_friendWin->m_selectedFriendIdx = -1;

	strcpy( name, MSG_ReadString() );

	g_ifMng->m_friendWin->RemoveFriend( name );

	
	sprintf( szTemp, g_LPACK.GetMessage(0, 732), name );
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );
}

void GTH_SendMessage_Friends_Remove(char *name)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_FRIENDS );
		
		MSG_WriteByte(&netMessage, FRIENDS_REMOVE );
		MSG_WriteString(&netMessage, name );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_Friends_Change()
{
	friend_t oldFriend;

	
	
	g_ifMng->m_friendWin->m_selectedFriendName[0] = 0;
	g_ifMng->m_friendWin->m_selectedFriendIdx = -1;

	oldFriend.level = MSG_ReadByte();
	oldFriend.job = MSG_ReadByte();
	oldFriend.worldIdx = MSG_ReadByte();
	strcpy( oldFriend.name, MSG_ReadString() );

	g_ifMng->m_friendWin->UpdateFriend(&oldFriend);
}

void GTH_ProcessMessage_Friends_List()
{
	int i, count;
	friend_t newFriend;

	count = MSG_ReadByte();

	
	
	g_ifMng->m_friendWin->m_selectedFriendName[0] = 0;
	g_ifMng->m_friendWin->m_selectedFriendIdx = -1;

	for( i = 0; i < MAX_NUMBER_OF_FRIEND; i ++ )
	{
		g_cgv.myCharacterInfo->friendList[i].name[0] = 0;
		g_cgv.myCharacterInfo->friendList[i].worldIdx = -1;
		g_cgv.myCharacterInfo->friendList[i].level = 0;
		g_cgv.myCharacterInfo->friendList[i].job = 0;
	}

	g_cgv.myCharacterInfo->tempFriendIdx = -1;

	g_ifMng->m_friendWin->m_totalOnlineFriendCount = 0; 
	g_ifMng->m_friendWin->m_totalFriendCount = 0; 

	for( i=0; i<count; i++ )
	{
		newFriend.level = MSG_ReadByte();
		newFriend.job = MSG_ReadByte();
		newFriend.worldIdx = MSG_ReadByte();
		strcpy( newFriend.name, MSG_ReadString() );

		g_ifMng->m_friendWin->AddFriend(&newFriend);
	}
}









void GTH_ProcessMessage_Guild_Reply()
{
	int command = MSG_ReadByte();

	switch( command )
	{
	
	case GUILD_SYNC:
		g_cpGuildCtrl->Decoder();
		break;
	

	case GUILD_ORGANIZE:
		GTH_ProcessMessage_Guild_Organize_Reply();
		break;
	case GUILD_DISORGANIZE:
		GTH_ProcessMessage_Guild_Disorganize_Reply();
		break;
	case GUILD_INVITE_REQUEST:
		GTH_ProcessMessage_Guild_Invite_Request();
		break;
	case GUILD_INVITE_CANCEL:
		GTH_ProcessMessage_Guild_Invite_Cancel();
		break;
	case GUILD_INVITE_REPLY:
		GTH_ProcessMessage_Guild_Invite_Reply();
		break;
	case GUILD_SECEDE:
		GTH_ProcessMessage_Guild_Secede_Reply();
		break;
	case GUILD_BANISH:
		GTH_ProcessMessage_Guild_Banish_Reply();
		break;
	case GUILD_UPDATE:
		GTH_ProcessMessage_Guild_Update();
		break;
	case GUILD_CHANGEMASTER:
		GTH_ProcessMessage_Guild_Change_Master_Reply();
		break;
	case GUILD_ADDSUBMASTER:
		GTH_ProcessMessage_Guild_Add_SubMaster_Reply();
		break;
	case GUILD_REMOVESUBMASTER:
		GTH_ProcessMessage_Guild_Remove_SubMaster_Reply();
		break;
	case GUILD_ADDFACTIONMASTER:
		GTH_ProcessMessage_Guild_Add_FactionMaster_Reply();
		break;
	case GUILD_REMOVEFACTIONMASTER:
		GTH_ProcessMessage_Guild_Remove_FactionMaster_Reply();
		break;
	case GUILD_GUILDMARK:
		
		break;
	case GUILD_DELETE_MASTER:
		GTH_ProcessMessage_Guild_Delete_Master_Reply();
		break;
	
	case GUILD_SUBSCRIBE:
		GTH_ProcessMessage_Guild_Subscribe_Reply();
		break;
	
	case GUILD_SUBSCRIBE_REQUEST:
		GTH_ProcessMessage_Guild_Subscribe_Request();
		break;		
	}
}



void GTH_SendMessage_Guild_Organize_Request( char *name, char *guildMark )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_ORGANIZE );
		
		MSG_WriteString( &netMessage, name );
		
		int i;
		for( i=0; i<GUILDMARKSIZE; i++ )
			MSG_WriteByte( &netMessage, guildMark[i] );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );


}




void GTH_ProcessMessage_Guild_Organize_Reply()
{
	GTH_UnlockSending();

	int result, error, currentNak;
	int rankPoint;
	
	
	char buffer[512];
	char guildName[GUILDNAMESTRING];

	memset( buffer, 0, sizeof(buffer) );

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	rankPoint = MSG_ReadShort();
	currentNak = MSG_ReadLong();

	sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );

	
	int RemainTime = MSG_ReadLong();

	g_cgv.myCharacterInfo->rankPoint = rankPoint;
	g_cgv.myCharacterInfo->curChargeSE = currentNak;

	if( result != -1 )
	{
		
		
		sprintf( buffer, g_LPACK.GetMessage( 0, 833 ), guildName );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
	}
	else
	{
		
		switch( error )
		{
		case GUILD_ERROR_ALREADYJOINED :
			sprintf( buffer, g_LPACK.GetMessage(0,869) );
			break;
		case GUILD_ERROR_NAK :
			sprintf( buffer, g_LPACK.GetMessage(0,870) );
			break;
		case GUILD_ERROR_LEVEL :
			sprintf( buffer, g_LPACK.GetMessage(0,871) );
			break;
		case GUILD_ERROR_RANKPOINT :
			sprintf( buffer, g_LPACK.GetMessage(0,872) );
			break;
		case GUILD_ERROR_NOTAWAKENED :
			sprintf( buffer, g_LPACK.GetMessage(0,873) );
			break;
		case GUILD_ERROR_SAMEGUILDNAME :
			sprintf( buffer, g_LPACK.GetMessage(0,874) );
			break;
		case GUILD_ERROR_MAXOVERGUILDNUM :
			sprintf( buffer, g_LPACK.GetMessage(0,951) );
			break;

			
		case GUILD_ERROR_ALLOW_CREATE:
			
			{			
			char szRemainTime[30];
			strncpy(szRemainTime, g_ifMng->GetTimeString(RemainTime), 30);
			szRemainTime[29] = NULL;
			sprintf( buffer, g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,669),  
				g_cgv.myCharacterInfo->name, szRemainTime);
			}

			break;
		
			
			
			
		default :
			sprintf( buffer, g_LPACK.GetMessage(0,834) );
			break;
		}
		
		buffer[511] = NULL;
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
	}
}

void GTH_SendMessage_Guild_Disorganize_Request()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_DISORGANIZE );
		
		NET_SendMessage( &gsSocket, &netMessage ); 
	}
	MSG_EndWriting( &netMessage );


}




void GTH_ProcessMessage_Guild_Disorganize_Reply()
{
	GTH_UnlockSending();

	int result, error, rankpoint;
	char buffer[256];

	memset( buffer, 0, sizeof(buffer) );

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	rankpoint = MSG_ReadShort();

	if( result == -1 )
	{
		switch( error )
		{
		case GUILD_ERROR_NOTJOINED :
			sprintf( buffer, g_LPACK.GetMessage( 0, 876 ) );
			break;
		case GUILD_ERROR_NOMASTER :
			sprintf( buffer, g_LPACK.GetMessage( 0, 877 ) );
			break;
		case GUILD_ERROR_NOGUILD :
			sprintf( buffer, g_LPACK.GetMessage( 0, 878 ) );
			break;
		default :
			
			sprintf( buffer, g_LPACK.GetMessage( 0, 836 ), g_cgv.myCharacterInfo->guild.name );
			break;
		}
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
	}
	else
	{
		


		
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322),  g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 663));
		

		g_cgv.myCharacterInfo->guildIdx = -1;
		g_cgv.myCharacterInfo->guild.idx = -1;
		g_cgv.myCharacterInfo->rankPoint = rankpoint;
	}
}

void GTH_SendMessage_Guild_Invite_Request( char *name )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_INVITE_REQUEST );
		
		MSG_WriteString( &netMessage, name );
		
		NET_SendMessage( &gsSocket, &netMessage ); 
	}
	MSG_EndWriting( &netMessage );


}



void GTH_ProcessMessage_Guild_Invite_Request()
{
	int who, result, error;
	
	
	char buffer[512];
	char name[256];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	who = MSG_ReadShort();
	
	int RemainTime = MSG_ReadLong();
	
	strcpy( name, MSG_ReadString() );

	

	if( who == 0 ) 
	{
		GTH_UnlockSending();

		if( result != -1 )
		{
			
			sprintf( buffer, g_LPACK.GetMessage(0,837), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), buffer, MSG_BUTTONTYPE_CANCEL, IF_MESSAGE_GUILD_INVITE_CANCEL, 10000.0f );
		}
		else
		{
			switch( error )
			{
			case GUILD_ERROR_INVITE_NONEPCINDEX:
				sprintf( buffer, g_LPACK.GetMessage(0,879) );
				break;
			case GUILD_ERROR_INVITE_NOTREADY:
				sprintf( buffer, g_LPACK.GetMessage(0,882) );
				break;
			case GUILD_ERROR_INVITE_DIFFWORLDINDEX:
				sprintf( buffer, g_LPACK.GetMessage(0,881) );
				break;
			case GUILD_ERROR_INVITE_PCSTATE:
				sprintf( buffer, g_LPACK.GetMessage(0,882) );
				break;
			case GUILD_ERROR_INVITE_ALREADY:
				sprintf( buffer, g_LPACK.GetMessage(0,883) );
				break;
			case GUILD_ERROR_INVITE_NOTLEVEL:
				sprintf( buffer, g_LPACK.GetMessage(0,884) );
				break;
			case GUILD_ERROR_INVITE_NOTMASTER:
				sprintf( buffer, g_LPACK.GetMessage(0,885) );
				break;
			case GUILD_ERROR_NOTJOINED:
				sprintf( buffer, g_LPACK.GetMessage(0,960) );
				break;

				
			case GUILD_ERROR_ALLOW_JOIN:
				
				{			
					char szRemainTime[30];
					strncpy(szRemainTime, g_ifMng->GetTimeString(RemainTime), 30);
					szRemainTime[29] = NULL;
						sprintf( buffer, g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,668), 
						g_cgv.myCharacterInfo->name, szRemainTime);
				}			
				break;			

			default:
				
				sprintf( buffer, g_LPACK.GetMessage(0,838), name );
				break;
			}
			
			buffer[511]= NULL;
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
	else 
	{
		if( result != -1 )
		{
			
			sprintf( buffer, g_LPACK.GetMessage( 0, 823 ), name );
			
			buffer[511]= NULL;
			g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), buffer, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_INVITE_REPLY, 10000.0f );
		}
	}
}

void GTH_SendMessage_Guild_Invite_Cancel()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_INVITE_CANCEL );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );


}


void GTH_ProcessMessage_Guild_Invite_Cancel()
{
	int who, error;
	char name[256];
	char buffer[256];

	error = MSG_ReadShort();
	who = MSG_ReadShort();
	strcpy( name, MSG_ReadString() );

	g_ifMng->ExitMessage();

	if( who == 0 )
	{
		GTH_UnlockSending();
		sprintf( buffer, g_LPACK.GetMessage(0,961), name );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
	}
	else
	{
		sprintf( buffer, g_LPACK.GetMessage(0,962), name );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
	}
}



void GTH_SendMessage_Guild_Invite_Reply( int reply )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_INVITE_REPLY );
		
		MSG_WriteShort( &netMessage, reply );
		
		NET_SendMessage( &gsSocket, &netMessage ); 
	}
	MSG_EndWriting( &netMessage );


}




void GTH_ProcessMessage_Guild_Invite_Reply()
{
	int who, result, error;
	char buffer[256];
	char name[256];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	who = MSG_ReadShort();
	strcpy( name, MSG_ReadString() );

	g_ifMng->ExitMessage();

	if( who == 0 ) 
	{
		if( result != -1 )
		{
			
			sprintf( buffer, g_LPACK.GetMessage(0,824), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
		else
		{
			switch( error )
			{
			case GUILD_ERROR_CANNOTFIND:
				
				sprintf( buffer, g_LPACK.GetMessage(0,880) );
				break;
			default:
				
				sprintf( buffer, g_LPACK.GetMessage(0,825), name );
				break;
			}

			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
	else 
	{
		GTH_UnlockSending();

		if( result != -1 )
		{
			
			sprintf( buffer, g_LPACK.GetMessage(0,826), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
		else
		{
			switch( error )
			{
			case GUILD_ERROR_CANNOTFIND:
				
				sprintf( buffer, g_LPACK.GetMessage(0,891) );
				break;
			default:
				
				sprintf( buffer, g_LPACK.GetMessage(0,840), name );
				break;
			}

			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
}

void GTH_SendMessage_Guild_Secede_Request()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_SECEDE );
		
		NET_SendMessage( &gsSocket, &netMessage ); 
	}
	MSG_EndWriting( &netMessage );


}



void GTH_ProcessMessage_Guild_Secede_Reply()
{
	GTH_UnlockSending();

	int result, error, rankPoint;
	char buffer[256];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	rankPoint = MSG_ReadShort();

	g_cgv.myCharacterInfo->rankPoint = rankPoint;

	if( result != -1 )
	{
		
		
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 661) );

		g_cgv.myCharacterInfo->guildIdx = -1;
		g_cgv.myCharacterInfo->guild.idx = -1;
	}
	else
	{
		switch( error )
		{
		case GUILD_ERROR_SECEDE_0:
			sprintf( buffer, g_LPACK.GetMessage(0,889) );
			break;
		case GUILD_ERROR_SECEDE_1:
			sprintf( buffer, g_LPACK.GetMessage(0,890) );
			break;
		case GUILD_ERROR_SECEDE_2:
			sprintf( buffer, g_LPACK.GetMessage(0,890) );
			break;
		case GUILD_ERROR_SECEDE_3:
			sprintf( buffer, g_LPACK.GetMessage(0,892) );
			break;
		case GUILD_ERROR_CANNOTFIND:
			sprintf( buffer, g_LPACK.GetMessage(0,893) );
			break;
		default:
			
			sprintf( buffer, g_LPACK.GetMessage( 0, 844 ), g_cgv.myCharacterInfo->guild.name );
			break;
		}
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
	}
}

void GTH_SendMessage_Guild_Banish_Request( char *name )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_BANISH );
		
		MSG_WriteString( &netMessage, name );
		
		NET_SendMessage( &gsSocket, &netMessage ); 
	}
	MSG_EndWriting( &netMessage );


}



void GTH_ProcessMessage_Guild_Banish_Reply()
{
	int who, result, error;
	int rankPoint;
	char buffer[256];
	char name[NAMESTRING];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	who = MSG_ReadShort();
	rankPoint = MSG_ReadShort();
	strcpy( name, MSG_ReadString() );

	g_cgv.myCharacterInfo->rankPoint = rankPoint;

	if( who == 0 ) 
	{
		GTH_UnlockSending();

		if( result != -1 )
		{
			
			sprintf( buffer, g_LPACK.GetMessage( 0, 829 ), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
		else
		{
			switch( error )
			{
			case GUILD_ERROR_BANISH_0:
				strcpy( buffer, g_LPACK.GetMessage(0,889) );
				break;
			case GUILD_ERROR_BANISH_1:
				strcpy( buffer, g_LPACK.GetMessage(0,70) );
				break;
			case GUILD_ERROR_BANISH_2:
				strcpy( buffer, g_LPACK.GetMessage(0,896) );
				break;
			case GUILD_ERROR_BANISH_3:
				strcpy( buffer, g_LPACK.GetMessage(0,896) );
				break;
			case GUILD_ERROR_BANISH_4:
				strcpy( buffer, g_LPACK.GetMessage(0,898) );
				break;
			case GUILD_ERROR_BANISH_5:
				strcpy( buffer, g_LPACK.GetMessage(0,899) );
				break;
			case GUILD_ERROR_CANNOTFIND:
				strcpy( buffer, g_LPACK.GetMessage(0,900) );
			default:
				
				sprintf( buffer, g_LPACK.GetMessage( 0, 843 ), name );
				break;
			}
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
	else 
	{
		if( result != -1 )
		{
			
			
			
			sprintf( buffer, g_LPACK.GetMessage( LPACK_TYPE_NORMAL2, 667 ), g_cgv.myCharacterInfo->name, name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );

			g_cgv.myCharacterInfo->guildIdx = -1;
			g_cgv.myCharacterInfo->guild.idx = -1;
		}
	}
}

void GTH_SendMessage_Guild_Change_Master_Request( char *name )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_CHANGEMASTER );
		
		MSG_WriteString( &netMessage, name );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );



}



void GTH_ProcessMessage_Guild_Change_Master_Reply()
{
	int who, result, error;
	char buffer[256];
	char name[NAMESTRING];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	who = MSG_ReadShort();

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );

	g_ifMng->ExitMessage();

	
	if( who == 0 )
	{
		GTH_UnlockSending();

		if( result != -1 )
		{
			
			sprintf( buffer, g_LPACK.GetMassage(9,28) , name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
		else
		{
			
			switch( error )
			{
			case GUILD_ERROR_NOGUILD:
				sprintf( buffer, g_LPACK.GetMessage(0,889) );
				break;
			case GUILD_ERROR_CANNOTFIND:
				sprintf( buffer, g_LPACK.GetMessage(0,70) );
				break;
			case GUILD_ERROR_NOTJOINED:
				sprintf( buffer, g_LPACK.GetMassage(9,29), name );
				break;
			case GUILD_ERROR_OFFLINE:
				sprintf( buffer, g_LPACK.GetMessage(0,1003), name );
				break;
			case GUILD_ERROR_SAMEMATENAME:
				sprintf( buffer, g_LPACK.GetMassage(9,30), name );
				break;
			case GUILD_ERROR_AUTHORITY:
				sprintf( buffer, g_LPACK.GetMassage(9,31) );
				break;
			case GUILD_ERROR_LEVEL:
				sprintf( buffer, g_LPACK.GetMassage(9,32), name );
				break;
			case GUILD_ERROR_NOTAWAKENED:
				sprintf( buffer, g_LPACK.GetMassage(9,33), name );
				break;
			case GUILD_ERROR_NOTEMPTY:
				sprintf( buffer, g_LPACK.GetMassage(9,34) );
				break;
			default:
				sprintf( buffer, g_LPACK.GetMassage(9,35), name );
				break;
			}
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
	
	else
	{
		if( result != -1 )
		{
			
			sprintf( buffer, g_LPACK.GetMassage(9,36), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );

		}
	}
}


void GTH_SendMessage_Guild_Add_Submaster_Request( char *name )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_ADDSUBMASTER );
		
		MSG_WriteString( &netMessage, name );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );


}



void GTH_ProcessMessage_Guild_Add_SubMaster_Reply()
{
	int who, result, error;
	char buffer[256];
	char name[NAMESTRING];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	who = MSG_ReadShort();

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );

	g_ifMng->ExitMessage();

	
	if( who == 0 )
	{
		GTH_UnlockSending();

		if( result != -1 )
		{
			sprintf( buffer, g_LPACK.GetMessage(0,919), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
		else
		{
			switch( error )
			{
			case GUILD_ERROR_NOGUILD:
				sprintf( buffer, g_LPACK.GetMessage(0,889) );
				break;
			case GUILD_ERROR_CANNOTFIND:
				sprintf( buffer, g_LPACK.GetMessage(0,70) );
				break;
			case GUILD_ERROR_LEVEL:
				sprintf( buffer, g_LPACK.GetMessage(0,1019), name );
				break;
			case GUILD_ERROR_AUTHORITY:
				sprintf( buffer, g_LPACK.GetMessage(0,1020) );
				break;
			case GUILD_ERROR_NOMASTER:
				sprintf( buffer, g_LPACK.GetMessage(0,1021), name );
				break;
			case GUILD_ERROR_CANNOTAPPOINT:
				sprintf( buffer, g_LPACK.GetMessage(0,1022), name );
				break;
			case GUILD_ERROR_NOTJOINED:
				sprintf( buffer, g_LPACK.GetMessage(0,1000), name );
				break;
			case GUILD_ERROR_NOTEMPTY:
				sprintf( buffer, g_LPACK.GetMessage(0,1001) );
				break;
			case GUILD_ERROR_SAMEMATENAME:
				sprintf( buffer, g_LPACK.GetMessage(0,1002), name );
				break;
			case GUILD_ERROR_OFFLINE:
				sprintf( buffer, g_LPACK.GetMessage(0,1003), name );
				break;
			
			case GUILD_ERROR_NOTAWAKENED:
				sprintf( buffer, g_LPACK.GetMessage(9,24), name );
				break;
			default:
				sprintf( buffer, g_LPACK.GetMessage(0,920), name );
				break;
			}
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
	
	else
	{
		if( result != -1 )
		{
			sprintf( buffer, g_LPACK.GetMessage(0,921), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );

		}
	}
}

void GTH_SendMessage_Guild_Add_Factionmaster_Request( char *name )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_ADDFACTIONMASTER );
		
		MSG_WriteString( &netMessage, name );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );


	
}



void GTH_ProcessMessage_Guild_Add_FactionMaster_Reply()
{
	int who, result, error;
	char buffer[256];
	char name[NAMESTRING];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	who = MSG_ReadShort();

	sstrncpy( name, MSG_ReadString(), NAMESTRING - 1 );

	g_ifMng->ExitMessage();

	if( who == 0 ) 
	{
		GTH_UnlockSending();

		if( result != -1 )
		{
			sprintf( buffer, g_LPACK.GetMessage(0,925), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
		else
		{
			switch( error )
			{
			case GUILD_ERROR_NOGUILD:
				sprintf( buffer, g_LPACK.GetMessage(0,889) );
				break;
			case GUILD_ERROR_CANNOTFIND:
				sprintf( buffer, g_LPACK.GetMessage(0,70) );
				break;
			case GUILD_ERROR_LEVEL:
				sprintf( buffer, g_LPACK.GetMessage(0,1004), name );
				break;
			case GUILD_ERROR_AUTHORITY:
				sprintf( buffer, g_LPACK.GetMessage(0,1005) );
				break;
			case GUILD_ERROR_NOMASTER:
				sprintf( buffer, g_LPACK.GetMessage(0,1006), name );
				break;
			case GUILD_ERROR_CANNOTAPPOINT:
				sprintf( buffer, g_LPACK.GetMessage(0,1007), name );
				break;
			case GUILD_ERROR_NOTJOINED:
				sprintf( buffer, g_LPACK.GetMessage(0,1008), name );
				break;
			case GUILD_ERROR_NOTEMPTY:
				sprintf( buffer, g_LPACK.GetMessage(0,1009) );
				break;
			case GUILD_ERROR_SAMEMATENAME:
				sprintf( buffer, g_LPACK.GetMessage(0,1010), name );
				break;
			case GUILD_ERROR_OFFLINE:
				sprintf( buffer, g_LPACK.GetMessage(0,1003), name );
				break;
			
			case GUILD_ERROR_NOTAWAKENED:
				sprintf( buffer, g_LPACK.GetMessage(9,25), name );
				break;
			default:
				sprintf( buffer, g_LPACK.GetMessage(0,926), name );
				break;
			}
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
	else 
	{
		if( result != -1 )
		{
			sprintf( buffer, g_LPACK.GetMessage(0,927), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
}

void GTH_SendMessage_Guild_Delete_Master_Request( char *name )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_DELETE_MASTER );
		
		MSG_WriteString( &netMessage, name );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );


	
}


void GTH_ProcessMessage_Guild_Delete_Master_Reply()
{
	GTH_UnlockSending();

	int error;
	char buffer[128];
	char name[NAMESTRING];

	error = MSG_ReadShort();
	strcpy( name, MSG_ReadString() );

	if( error )
	{
		switch( error )
		{
		case GUILD_ERROR_NOGUILD:
			sprintf( buffer, g_LPACK.GetMessage(0,889), name );
			break;
		case GUILD_ERROR_CANNOTAPPOINT:
			
			sprintf( buffer, g_LPACK.GetMessage(0,931), name );
			break;
		case GUILD_ERROR_NOMASTER:
			
			sprintf( buffer, g_LPACK.GetMessage(9,17) );
			break;
		default:
			sprintf( buffer, g_LPACK.GetMessage(0,1011), name );
			break;

		}
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
	}
}






void GTH_ProcessMessage_Guild_Remove_SubMaster_Reply()
{
	int who, result, error;
	char buffer[128];
	char name[NAMESTRING];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	who = MSG_ReadShort();
	strcpy( name, MSG_ReadString() );

	g_ifMng->ExitMessage();

	if( who == 0 ) 
	{
		GTH_UnlockSending();

		if( result != -1 )
		{
			sprintf( buffer, g_LPACK.GetMessage(0,922), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
		else
		{
			switch( error )
			{
			case GUILD_ERROR_NOGUILD:
				sprintf( buffer, g_LPACK.GetMessage(0,889) );
				break;
				
			case GUILD_ERROR_CANNOTFIND:
				sprintf( buffer, g_LPACK.GetMessage(9,23) );
				break;
			case GUILD_ERROR_AUTHORITY:
				sprintf( buffer, g_LPACK.GetMessage(0,1012) );
				break;
			case GUILD_ERROR_NOTEMPTY:
				sprintf( buffer, g_LPACK.GetMessage(0,1013) );
				break;
			case GUILD_ERROR_SAMEMATENAME:
				sprintf( buffer, g_LPACK.GetMessage(0,1014) );
				break;
			case GUILD_ERROR_OFFLINE:
				sprintf( buffer, g_LPACK.GetMessage(0,1015), name );
				break;
			default:
				sprintf( buffer, g_LPACK.GetMessage(0,923), name );
				break;
			}
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
	else 
	{
		if( result != -1 )
		{
			sprintf( buffer, g_LPACK.GetMessage(0,924), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
}






void GTH_ProcessMessage_Guild_Remove_FactionMaster_Reply()
{
	int who, result, error;
	char buffer[128];
	char name[NAMESTRING];

	result = MSG_ReadShort();
	error = MSG_ReadShort();
	who = MSG_ReadShort();
	strcpy( name, MSG_ReadString() );

	g_ifMng->ExitMessage();

	if( who == 0 ) 
	{
		GTH_UnlockSending();

		if( result != -1 )
		{
			sprintf( buffer, g_LPACK.GetMessage(0,928), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
		else
		{
			switch( error )
			{
			case GUILD_ERROR_NOGUILD:
				sprintf( buffer, g_LPACK.GetMessage(0,889) );
				break;
				
			case GUILD_ERROR_CANNOTFIND:
				sprintf( buffer, g_LPACK.GetMessage(9,23) );
				break;
			case GUILD_ERROR_AUTHORITY:
				sprintf( buffer, g_LPACK.GetMessage(0,1016) );
				break;
			case GUILD_ERROR_NOTEMPTY:
				sprintf( buffer, g_LPACK.GetMessage(0,1013) );
				break;
			case GUILD_ERROR_SAMEMATENAME:
				sprintf( buffer, g_LPACK.GetMessage(0,1014) );
				break;
			case GUILD_ERROR_OFFLINE:
				sprintf( buffer, g_LPACK.GetMessage(0,1015), name );
				break;
			default:
				sprintf( buffer, g_LPACK.GetMessage(0,929), name );
				break;
			}
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
	else 
	{
		if( result != -1 )
		{
			sprintf( buffer, g_LPACK.GetMessage(0,930), name );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), buffer );
		}
	}
}


void GTH_SendMessage_Guild_GuildMarkInfo()
{

}

static bool bLockSendGuildMark = false;
void GTH_SendMessage_Guild_GuildMark( int guildIdx )
{
	if ( bLockSendGuildMark )
		return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_GUILDMARK );
		MSG_WriteShort( &netMessage, guildIdx );
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );

	bLockSendGuildMark = true;


}




void GTH_SendMessage_Guild_Schedule_Request( char *notice )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_SCHEDULE );
		
		MSG_WriteString( &netMessage, notice );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );


}


void GTH_SendMessage_Guild_Subscribe_Request( char *guildName )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_GUILD );
		MSG_WriteByte( &netMessage, GUILD_SUBSCRIBE );
		
		MSG_WriteString( &netMessage, guildName );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );


}




void GTH_ProcessMessage_Guild_Subscribe_Reply()
{
	GTH_UnlockSending();

	int result, error;
	char szTemp[256];
	char guildName[GUILDNAMESTRING];

	result = MSG_ReadShort();
	error = MSG_ReadShort();

	if( result != -1 )
	{
		sstrncpy( guildName, MSG_ReadString(), GUILDNAMESTRING - 1 );

		switch( error )
		{
		case 0 :
			sprintf( szTemp, g_LPACK.GetMessage(0,957), guildName );
			break;
		default:
			sprintf( szTemp, g_LPACK.GetMessage(0,958), guildName );
			break;
		}
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
	}
	else
	{
		switch( error )
		{
		case GUILD_ERROR_ALREADYJOINED:
			sprintf( szTemp, g_LPACK.GetMessage(0,952) );
			break;
		case GUILD_ERROR_LEVEL:
			sprintf( szTemp, g_LPACK.GetMessage(0,953) );
			break;
		case GUILD_ERROR_NOGUILD:
			sprintf( szTemp, g_LPACK.GetMessage(0,954) );
			break;
		case GUILD_ERROR_NOMASTER:
			sprintf( szTemp, g_LPACK.GetMessage(0,955) );
			break;
		default:
			sprintf( szTemp, g_LPACK.GetMessage(0,956) );
			break;
		}
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
	}
}

void GTH_ProcessMessage_Guild_Subscribe_Request()
{
	char fromName[NAMESTRING], szTemp[256];

	sstrncpy( fromName, MSG_ReadString(), NAMESTRING - 1 );

	sprintf( szTemp, g_LPACK.GetMessage(0,959), fromName );

	g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );

}


void CalcGuildUserCount()
{
	g_ifMng->m_guildWin->UpdateMemberList();
}


void GTH_ProcessMessage_Guild_Update()
{
	short guildIdx;
	int rank, rankIdx;
	int update;
	int i;

	guildIdx = MSG_ReadShort();
	update = MSG_ReadLong();
	rank = MSG_ReadLong();
	rankIdx = MSG_ReadLong();

	int addRankIdx = 0;

	if( rank == GUILD_SUBMASTER )
		addRankIdx = MAX_GUILD_MASTERS;
	else if( rank == GUILD_FACTIONMASTER )
		addRankIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS;
	else if( rank == GUILD_MATE )
		addRankIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS;
	else
		addRankIdx = 0;

	if( ( update & GUILD_IDX ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.idx = MSG_ReadShort();
		g_cgv.myCharacterInfo->guildIdx = g_cgv.myCharacterInfo->guild.idx;
	}
	
	if( ( update & GUILD_SERVERGROUPID ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.serverGroupID = MSG_ReadShort();
	}
	
	if( ( update & GUILD_NAME ) != 0 )
	{
		strcpy( g_cgv.myCharacterInfo->guild.name, MSG_ReadString() );
	}
	
	if( ( update & GUILD_MARK ) != 0 )
	{


	}
	
	if( ( update & GUILD_MARKUPDATETIME ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.markUpdateTime = MSG_ReadLong();
	}
	
	if( ( update & GUILD_NOTICE ) != 0 )
	{
		strcpy( g_cgv.myCharacterInfo->guild.notice, MSG_ReadString() );
	}
	
	if( ( update & GUILD_FAMEPOINT ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.famePoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_EXPPOINT ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.expPoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_RAIDPOINT ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.raidPoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_PVPPOINT ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.pvpPoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_HUNTERPOINT ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.hunterPoint = MSG_ReadLong();
	}
	
	if( ( update & GUILD_LEVEL ) != 0 )
	{
		g_cgv.myCharacterInfo->guild.level = MSG_ReadShort();
	}
	
	if( ( update & GUILD_MASTERNAME ) != 0 )
	{
		
		strcpy( g_cgv.myCharacterInfo->guild.userName[0], MSG_ReadString() );






	}
	
	if( ( update & GUILD_SUBMASTERNAME ) != 0 )
	{
		int temp = MAX_GUILD_MASTERS;

		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_SUBMASTERS; i++)
			{
				
				strcpy( g_cgv.myCharacterInfo->guild.userName[i+temp], MSG_ReadString() );





			}

		}
		else
		{
			strcpy( g_cgv.myCharacterInfo->guild.userName[rankIdx+temp], MSG_ReadString() );





		}

	}

	if( ( update & GUILD_FACTIONMASTERNAME ) != 0 )
	{
		int temp = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS;

		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++)
			{
				
				strcpy( g_cgv.myCharacterInfo->guild.userName[i+temp], MSG_ReadString() );





			}

		}
		else
		{
			strcpy( g_cgv.myCharacterInfo->guild.userName[rankIdx+temp], MSG_ReadString() );





		}
	}
	
	if( ( update & GUILD_MATENAME ) != 0 )
	{
		int temp = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS;

		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_MATES; i++)
				
				strcpy( g_cgv.myCharacterInfo->guild.userName[i+temp], MSG_ReadString() );
		}
		else
			strcpy( g_cgv.myCharacterInfo->guild.userName[rankIdx+temp], MSG_ReadString() );
	}
	
	if( ( update & GUILD_MATEAUTHORITY ) != 0 )
	{
		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_MATES; i++)
				g_cgv.myCharacterInfo->guild.mateAuthority[i] = MSG_ReadByte();
		}
		else
			g_cgv.myCharacterInfo->guild.mateAuthority[rankIdx] = MSG_ReadByte();
	}
	
	if( ( update & GUILD_MATELASTCONNECTTIME ) != 0 )
	{
		if( rankIdx == -1 )
		{
			for( i=0; i<MAX_GUILD_MATES; i++)
				g_cgv.myCharacterInfo->guild.mateLastConnectTime[i] = MSG_ReadLong();
		}
		else
			g_cgv.myCharacterInfo->guild.mateLastConnectTime[rankIdx] = MSG_ReadLong();
	}
	
	if( ( update & GUILD_MASTERNOTE ) != 0 )
	{
		if( rankIdx == -1 )
		{


		}
		else
			strcpy( g_cgv.myCharacterInfo->guild.masterNote[rankIdx], MSG_ReadString() );
	}

	if( ( update & GUILD_USERLEVEL ) != 0 )
	{
		if( rankIdx == -1 )
		{
			int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;

			for( i=0; i<max; i++ )
				g_cgv.myCharacterInfo->guild.userLevel[i] = MSG_ReadLong();
		}
		else
			g_cgv.myCharacterInfo->guild.userLevel[rankIdx+addRankIdx] = MSG_ReadLong();
	}
	
	if( ( update & GUILD_USERJOB ) != 0 )
	{
		if( rankIdx == -1 )
		{
			int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;

			for( i=0; i<max; i++ )
				g_cgv.myCharacterInfo->guild.userJob[i] = MSG_ReadLong();
		}
		else
			g_cgv.myCharacterInfo->guild.userJob[rankIdx+addRankIdx] = MSG_ReadLong();
	}
	
	if( ( update & GUILD_USERLOCATION ) != 0 )
	{
		if( rankIdx == -1 )
		{
			int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;

			for( i=0; i<max; i++ )
				g_cgv.myCharacterInfo->guild.userLocation[i] = MSG_ReadLong();
		}
		else
			g_cgv.myCharacterInfo->guild.userLocation[rankIdx+addRankIdx] = MSG_ReadLong();
	}

	CalcGuildUserCount();


}



void GTH_ProcessMessage_Guild_Request()
{
	int command = MSG_ReadByte();



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
	case MAIL_STATE :			
		GTH_ProcessMessage_MailSystem_State();
		break;
	case MAIL_VIEW :			
		GTH_ProcessMessage_MailSystem_View();
		break;
	case MAIL_ARRIVE :			
		GTH_ProcessMessage_MailSystem_Arrive();
		break;
	}
}

void GTH_ProcessMessage_MailSystem_Send()
{
	char	toName[NAMESTRING+1];
	char	szTemp[256];
	int		reply;
	int		error;

	sstrncpy( toName, MSG_ReadString(), NAMESTRING - 1 );
	toName[NAMESTRING]=NULL;

	reply		=	MSG_ReadByte();

	
	if( reply )
	{
		sprintf( szTemp, g_LPACK.GetMessage(0,994), toName );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );

		g_ifMng->m_mailEditWin->Enable( false );

		g_chatManager->InitInputStr( INPUT_INDEX_MAIL_NAME );
		g_chatManager->InitInputStr( INPUT_INDEX_MAIL_TITLE );
		g_chatManager->InitInputStr( INPUT_INDEX_MAIL_CONTENT );

		g_chatManager->SetInputState( INPUT_INDEX_MAIL_NAME, -1 );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_TITLE, -1 );
		g_chatManager->SetInputState( INPUT_INDEX_MAIL_CONTENT, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		g_ifMng->m_mailWin->SetDisplayMode( false );
		g_ifMng->m_mailWin->DeleteMailList();
		GTH_SendMessage_MailSystem_Receive();

		g_ifMng->m_mailWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_mailWin );
	}
	else
	{
		error	=	MSG_ReadByte();
		switch( error )
		{
		case 1 :
			sprintf( szTemp, g_LPACK.GetMessage(0,995), toName );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
			break;
		case 2 :
			sprintf( szTemp, g_LPACK.GetMessage(0,996), toName );
			g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
			break;
		}
	}
}


class mail_comparedate
{
public :
	bool operator() ( MailEntry *x, MailEntry *y )
	{
		if( stricmp( x->mailTime, y->mailTime ) > 0 ) return true;
		return false;
	}
};

#include "..\CTools.h"
void GTH_ProcessMessage_MailSystem_Receive()
{
	MailEntry			mail[MAX_MAIL_COUNT], *pMail;
	int					count;
	vector<MailEntry *>	sortedMail;

	count		=	MSG_ReadByte();
	
	memset( &mail[0], 0, sizeof( MailEntry ) * MAX_MAIL_COUNT );

	pMail		=	&mail[0];
	for( int i = 0; i < count; i ++, pMail ++ )
	{
		pMail->mailIdx		=	MSG_ReadByte();
		
		sstrncpy( pMail->fromName, MSG_ReadString(), NAMESTRING - 1 );
		CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(pMail->fromName);

		sstrncpy( pMail->mailTitle, MSG_ReadString(), MAIL_TITLESIZE );
		CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(pMail->mailTitle);

		strncpy( pMail->mailTime, MSG_ReadString(), 19 );
		
		pMail->mailState	=	MSG_ReadByte();

		
		sortedMail.push_back( &mail[i] );
	}

	sort( sortedMail.begin(), sortedMail.end(), mail_comparedate() );

	MailEntry	*pRecvMail, *pStoredMail, *pThisMail;

	g_ifMng->m_mailWin->DeleteMailList();
	
	pRecvMail	=	&g_ifMng->m_mailWin->m_mailList[MAIL_INTERFACE_TYPE_RECVMAIL][0];
	pStoredMail	=	&g_ifMng->m_mailWin->m_mailList[MAIL_INTERFACE_TYPE_STOREMAIL][0];

	for( i = 0; i < sortedMail.size(); i ++ )
	{
		pThisMail = sortedMail[i];

		switch( pThisMail->mailState )
		{
		
		case 0: case 1:
			memcpy( pRecvMail, pThisMail, sizeof( MailEntry ) );
			g_ifMng->m_mailWin->m_mailListCount[MAIL_INTERFACE_TYPE_RECVMAIL] ++;
			pRecvMail	++;
			break;
		
		case 2: case 3:
			memcpy( pStoredMail, pThisMail, sizeof( MailEntry ) );
			g_ifMng->m_mailWin->m_mailListCount[MAIL_INTERFACE_TYPE_STOREMAIL] ++;
			pStoredMail	++;
			break;
		}
	}
	
	
	g_ifMng->m_mailWin->SetDisplayMode( true );
	


}

void GTH_ProcessMessage_MailSystem_State()
{
	int			notReadNumber, readNumber, notReadSaveNumber, readSaveNumber;

	notReadNumber		=	MSG_ReadByte();
	readNumber			=	MSG_ReadByte();
	notReadSaveNumber	=	MSG_ReadByte();
	readSaveNumber		=	MSG_ReadByte();

	
	if( notReadNumber > 0 )
	{
		g_ifMng->m_menuWin->m_communityBtn->SetFlickerMode( true );
		g_ifMng->m_communityWin->m_mailBtn->SetFlickerMode( true );
	}
	else
	{
		g_ifMng->m_menuWin->m_communityBtn->SetFlickerMode( false );
		g_ifMng->m_communityWin->m_mailBtn->SetFlickerMode( false );
	}

}

void GTH_ProcessMessage_MailSystem_View()
{
	MailEntry	mail;

	memset( &mail, 0, sizeof( MailEntry ) );

	mail.mailIdx		=	MSG_ReadByte();
	
	sstrncpy( mail.fromName, MSG_ReadString(), NAMESTRING - 1 );
	CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(mail.fromName);

	sstrncpy( mail.mailTitle, MSG_ReadString(), MAIL_TITLESIZE );
	CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(mail.mailTitle);

	strncpy( mail.mailTime, MSG_ReadString(), 19 );
	sstrncpy( mail.mailStr, MSG_ReadString(), MAIL_STRSIZE );
	CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(mail.mailStr);

	mail.mailState		=	MSG_ReadByte();

	
	g_ifMng->m_mailEditWin->SetMailContent( mail );
	g_ifMng->m_mailEditWin->SetDisplayMode( true );

	g_ifMng->m_menuWin->m_communityBtn->SetFlickerMode( false );
	g_ifMng->m_communityWin->m_mailBtn->SetFlickerMode( false );
	








}

void GTH_ProcessMessage_MailSystem_Arrive()
{
	char		fromName[NAMESTRING+1];
	char		szTemp[256];

	sstrncpy( fromName, MSG_ReadString(), NAMESTRING - 1 );
	CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(fromName);

	
	g_ifMng->m_menuWin->m_communityBtn->SetFlickerMode( true );
	g_ifMng->m_communityWin->m_mailBtn->SetFlickerMode( true );

	sprintf( szTemp, g_LPACK.GetMessage(0,997), fromName );
	g_ifMng->AddSysMessage( szTemp );
}

#include "..\CTools.h"
void GTH_SendMessage_MailSystem_Send( char *toName, char *mailTitle, char *mailStr )
{	
	

	CTools::Replace_singleQUOTATIONmark_by_doubleQUOTATIONmark(toName);
	CTools::Replace_singleQUOTATIONmark_by_doubleQUOTATIONmark(mailTitle);
	CTools::Replace_singleQUOTATIONmark_by_doubleQUOTATIONmark(mailStr);

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_EXTEND );
		MSG_WriteByte( &netMessage, CC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_SEND );
		
		MSG_WriteString( &netMessage, toName );
		MSG_WriteString( &netMessage, mailTitle );
		MSG_WriteString( &netMessage, mailStr );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );

}

void GTH_SendMessage_MailSystem_Receive()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_EXTEND );
		MSG_WriteByte( &netMessage, CC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_RECEIVE );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );

}

void GTH_SendMessage_MailSystem_Delete( int mailIdx[], int count )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_EXTEND );
		MSG_WriteByte( &netMessage, CC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_DELETE );
		
		MSG_WriteByte( &netMessage, count );
		
		for( int i = 0; i < count; i ++ )
		{
			MSG_WriteByte( &netMessage, mailIdx[i] );
		}
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_MailSystem_View( int mailIdx )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_EXTEND );
		MSG_WriteByte( &netMessage, CC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_VIEW );
		
		MSG_WriteByte( &netMessage, mailIdx );
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_MailSystem_Save( int mailIdx[], int count )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_EXTEND );
		MSG_WriteByte( &netMessage, CC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_SAVE );
		
		MSG_WriteByte( &netMessage, count );
		
		for( int i = 0; i < count; i ++ )
		{
			MSG_WriteByte( &netMessage, mailIdx[i] );
		}
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );
}







void GTH_ProcessMessage_WorldChatting()
{
	int reply, error;

	reply		=	MSG_ReadByte();
	error		=	MSG_ReadByte();
	
	
	if( reply )
	{
		g_cgv.myCharacterInfo->curChargeSE	=	MSG_ReadLong();
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMessage(0,998) );

		if(gGameConfigFlag.m_bWorld_Chat_Limit)
		{
			g_cgv.myCharacterInfo->WorldChatting.iDecreaseCount		=	MSG_ReadLong();

			
			if(g_cgv.myCharacterInfo->WorldChatting.iDecreaseCount <= 0)
			{
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 249 ), GTHCOLOR_ERRORMESSAGE );
			}
		}
	}
	
	else
	{
		switch(error) {
		case WORLD_CHATTING_ERROR_SE:
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMessage(0,999) );
			break;

		case WORLD_CHATTING_ERROR_COUNT:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 234 ));
			break;  
		}
	}

	return;
}



void GTH_SendMessage_WorldChatting_Count( )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_EXTEND );
		MSG_WriteByte( &netMessage, CC_EXTEND_WORLDCHATTING_COUNT );
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );
}


void GTH_ProcessMessage_WorldChatting_Count()
{
	g_cgv.myCharacterInfo->WorldChatting.lDate				=	MSG_ReadLong();
	g_cgv.myCharacterInfo->WorldChatting.iMaxCount			=	MSG_ReadLong();
	g_cgv.myCharacterInfo->WorldChatting.iDecreaseCount		=	MSG_ReadLong();
	return;
}



void GTH_SendMessage_WorldChatting( char* msg,const BOOL bUseSpeaker)
{
	
	if((FALSE == bUseSpeaker) &&		
	   (g_cgv.myCharacterInfo->m_nMemberShipType <= 0 )){
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2,275 ), GTHCOLOR_ERRORMESSAGE );
		return;
	}


	
	if( g_LPACK.CheckAbuseWord( msg ) ){
		g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 21 ), GTHCOLOR_ERRORMESSAGE );
		return;
	}
	

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_EXTEND );
		MSG_WriteByte( &netMessage, CC_WORLDCHATTING );
		MSG_WriteByte( &netMessage,bUseSpeaker);
		MSG_WriteByte( &netMessage,g_cgv.saveInventoryIdx);
		MSG_WriteString( &netMessage, msg );
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );


	if(bUseSpeaker == TRUE){
		GTH_UseItem_Delete(g_cgv.saveInventoryIdx);	
		g_cgv.saveInventoryIdx = -1;
	}
}