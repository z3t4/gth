 #include "../global_def.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 

int GTH_ProcessMessage_Party()
{
	int command;
	
	command = MSG_ReadByte();

	switch( command )
	{
	case PARTY_ERROR :
		GTH_ProcessMessage_Party_Error();
		break;
	case PARTY_REQUEST_ORGANIZE :
		GTH_ProcessMessage_Party_Request_Organize();
		break;
	case PARTY_ADMIT_ORGANIZE :
		GTH_ProcessMessage_Party_Admit_Organize();
		break;
	case PARTY_REJECT_ORGANIZE :
		GTH_ProcessMessage_Party_Reject_Organize();
		break;

	case PARTY_REQUEST_JOIN :
		GTH_ProcessMessage_Party_Request_Join();
		break;
	case PARTY_ADMIT_JOIN :
		GTH_ProcessMessage_Party_Admit_Join();
		break;
	case PARTY_REJECT_JOIN :
		GTH_ProcessMessage_Party_Reject_Join();
		break;

	case PARTY_OUT :
		GTH_ProcessMessage_Party_Out();
		break;

	case PARTY_FORCEDOUT :
		GTH_ProcessMessage_Party_ForcedOut();
		break;

	case PARTY_DISORGANIZE :
		GTH_ProcessMessage_Party_Disorganize();
		break;

		
	case PARTY_SENDALL :
		GTH_ProcessMessage_Party_Request_SendAll();
		break;

	case PARTY_UPDATE :

		GTH_ProcessMessage_Party_Update();
		break;

	case PARTY_CANJOIN :
		GTH_ProcessMessage_Party_Request_CanJoin();
		break;

	case PARTY_ITEMDIVIDETYPE :

		break;

	case PARTY_CHATMESSAGE :
		GTH_ProcessMessage_Party_ChatMessage();
		break;
	}
	return 1;
}






int GTH_ProcessMessage_Party_Error()
{
	int error;
	error = MSG_ReadByte();
	switch( error )
	{
	case ERROR_PARTY_1 :
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,68), D3DCOLOR_ARGB(255, 255, 55, 55) );
		
		break;
	case ERROR_PARTY_2 :
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,69), D3DCOLOR_ARGB(255, 255, 55, 55) );
		
		break;
	}
	
	return 1;
}

int GTH_SendMessage_Party_Request_Organize( int idxOnServer )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		
		MSG_WriteByte(&netMessage, PARTY_REQUEST_ORGANIZE );
		MSG_WriteShort(&netMessage, idxOnServer);
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_Request_Organize()
{
	int reply = 1, error = 0, type, idx;
	char name[256];

#if defined _GTH_CHINESE_VERSION || _GTH_TAIWAN_VERSION		
	char temp[256];	
#endif

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		idx = MSG_ReadShort();
		type = MSG_ReadByte();
		strcpy( name, MSG_ReadString() );

		g_ifMng->m_partyWin->SetPartyMember( idx, name );
		
		if( type )
		{	
			
#if defined _GTH_CHINESE_VERSION || _GTH_TAIWAN_VERSION		
			sprintf( temp, g_LPACK.GetMassage(0,64), name );
			strcpy( name, temp );
#else
			strcat( name, g_LPACK.GetMassage(0,64) );
#endif

			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), name, MSG_BUTTONTYPE_CANCEL, IF_MESSAGE_PARTY_REQUEST_ORGANIZE, 10000.0f );
		}
		
		else
		{
			strcat( name, g_LPACK.GetMassage(0,66) );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), name, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_PARTY_REPLY_ORGANIZE, 10000.0f );
		}
	}
	else
	{
		error = MSG_ReadByte();		
		switch( error )
		{
		case ERROR_PARTY_1 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,70) , D3DCOLOR_ARGB(255, 255, 55, 55));
			
			break;
		case ERROR_PARTY_2 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,68), D3DCOLOR_ARGB(255, 255, 55, 55) );
			
			break;
		case ERROR_PARTY_3 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,71), D3DCOLOR_ARGB(255, 255, 55, 55));
			
			break;
		case ERROR_PARTY_4 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,72), D3DCOLOR_ARGB(255, 255, 55, 55) );
			
			break;
		}
	}
	return 1;
}


int GTH_SendMessage_Party_Admit_Organize()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_ADMIT_ORGANIZE );
		
		MSG_WriteShort(&netMessage, g_ifMng->m_partyWin->GetPartyMemberIdx() );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}


int GTH_ProcessMessage_Party_Admit_Organize()
{
	int reply, error, type;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		type = MSG_ReadByte();

		strcpy( g_cgv.myCharacterInfo->organizerName, MSG_ReadString() );
		g_cgv.myCharacterInfo->organizeServer = MSG_ReadByte();
		g_cgv.myCharacterInfo->organizeTime = MSG_ReadLong();
		g_cgv.myCharacterInfo->memberNumber = MSG_ReadByte();
		g_cgv.myCharacterInfo->itemDivideType = MSG_ReadByte();
		g_cgv.myCharacterInfo->canJoin = MSG_ReadByte();
		g_cgv.myCharacterInfo->partyIndex = MSG_ReadShort();
		g_cgv.myCharacterInfo->isLeader = MSG_ReadByte();
		MSG_ReadMember( &g_cgv.myCharacterInfo->member[0] );
		MSG_ReadMember( &g_cgv.myCharacterInfo->member[1] );
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,73), D3DCOLOR_ARGB(255, 255, 125, 100) );
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_PARTY_1 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,70), D3DCOLOR_ARGB(255, 255, 55, 55) );
			
			break;
		case ERROR_PARTY_2 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,68), D3DCOLOR_ARGB(255, 255, 55, 55) );
			
			break;
		case ERROR_PARTY_3 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,71), D3DCOLOR_ARGB(255, 255, 55, 55) );
			
			break;
		case ERROR_PARTY_4 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,72), D3DCOLOR_ARGB(255, 255, 55, 55) );
			
			break;
		}
	}
	if( g_ifMng->m_msgType == IF_MESSAGE_PARTY_REQUEST_ORGANIZE || 
		g_ifMng->m_msgType == IF_MESSAGE_PARTY_REPLY_ORGANIZE )
	{
		g_ifMng->ExitMessage();
	}
	return 1;
}

int GTH_SendMessage_Party_Reject_Organize( int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_REJECT_ORGANIZE );
		
		MSG_WriteByte(&netMessage, type );
		
		MSG_WriteShort(&netMessage, g_ifMng->m_partyWin->GetPartyMemberIdx() );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_Reject_Organize()
{
	int reply = 1, error = 0;
	int type;
	char tempStr[256];
	
	
	
	type = MSG_ReadByte();
	if( type )
	{
		
		sprintf( tempStr, g_LPACK.GetMassage(0,76), g_ifMng->m_partyWin->GetPartyMemberName() );
		g_ifMng->AddSysMessage( tempStr, D3DCOLOR_ARGB(255, 255, 125, 100) );

		
		g_cgv.saveMenuTime = 5000.0f;
	}
	else
	{
		
		sprintf( tempStr, g_LPACK.GetMassage(0,76), g_ifMng->m_partyWin->GetPartyMemberName() );
		g_ifMng->AddSysMessage( tempStr, D3DCOLOR_ARGB(255, 255, 125, 100) );
	}

	if( g_ifMng->m_msgType == IF_MESSAGE_PARTY_REQUEST_ORGANIZE || 
		g_ifMng->m_msgType == IF_MESSAGE_PARTY_REPLY_ORGANIZE )
	{
		g_ifMng->ExitMessage();
	}

	return 1;
}





int GTH_SendMessage_Party_Request_Join( int idxOnServer )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		
		MSG_WriteByte(&netMessage, PARTY_REQUEST_JOIN );
		MSG_WriteShort(&netMessage, idxOnServer);
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_Request_Join()
{
	int reply = 1, error = 0, type, idx;
	char name[256];
#if defined _GTH_CHINESE_VERSION || _GTH_TAIWAN_VERSION		
	char temp[256];
#endif

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		idx = MSG_ReadShort();
		type = MSG_ReadByte();
		strcpy( name, MSG_ReadString() );

		g_ifMng->m_partyWin->SetPartyMember( idx, name );

		
		if( type )
		{
			
#if defined _GTH_CHINESE_VERSION || _GTH_TAIWAN_VERSION		
			sprintf( temp, g_LPACK.GetMassage(0,77), name );
			strcpy( name, temp );
#else
			strcat( name, g_LPACK.GetMassage(0,77) );
#endif
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), name, MSG_BUTTONTYPE_CANCEL, IF_MESSAGE_PARTY_REQUEST_JOIN, 10000.0f );
		}
		
		else
		{
			
			strcat( name, g_LPACK.GetMassage(0,78));
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), name, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_PARTY_REPLY_JOIN, 10000.0f );
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_PARTY_1 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,70), D3DCOLOR_ARGB(255, 255, 55, 55) );
			break;
		case ERROR_PARTY_2 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,68), D3DCOLOR_ARGB(255, 255, 55, 55) );
			break;
		case ERROR_PARTY_3 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,71), D3DCOLOR_ARGB(255, 255, 55, 100) );
			break;
		case ERROR_PARTY_4 :
			
			g_ifMng->AddSysMessage(  g_LPACK.GetMassage(0,72), D3DCOLOR_ARGB(255, 255, 55, 55));		
			break;
		case ERROR_PARTY_6 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,79), D3DCOLOR_ARGB(255, 255, 55, 55) );
		}
	}
	return 1;
}


int GTH_SendMessage_Party_Admit_Join()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_ADMIT_JOIN );
		
		MSG_WriteShort(&netMessage, g_ifMng->m_partyWin->GetPartyMemberIdx() );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}


int GTH_ProcessMessage_Party_Admit_Join()
{
	partyMember_t m;
	char tempMsg[256];
	int reply, error, isNewMember, i;

	if( g_ifMng->m_msgType == IF_MESSAGE_PARTY_REQUEST_JOIN || 
		g_ifMng->m_msgType == IF_MESSAGE_PARTY_REPLY_JOIN )
	{
		g_ifMng->ExitMessage();
	}

	reply = MSG_ReadByte();

	if( reply > 0 )
	{
		isNewMember = MSG_ReadByte();
		
		strcpy( g_cgv.myCharacterInfo->organizerName, MSG_ReadString() );
		g_cgv.myCharacterInfo->organizeServer = MSG_ReadByte();
		g_cgv.myCharacterInfo->organizeTime = MSG_ReadLong();
		g_cgv.myCharacterInfo->memberNumber = MSG_ReadByte();
		g_cgv.myCharacterInfo->itemDivideType = MSG_ReadByte();
		g_cgv.myCharacterInfo->canJoin = MSG_ReadByte();
		g_cgv.myCharacterInfo->partyIndex = MSG_ReadShort();
		g_cgv.myCharacterInfo->isLeader = MSG_ReadByte();
		
		for( i = 0; i < g_cgv.myCharacterInfo->memberNumber; i ++ )
		{
			MSG_ReadMember( &m );
			memcpy( &g_cgv.myCharacterInfo->member[m.idx], &m, sizeof( partyMember_t ) );
		}

		
		if( !isNewMember )
		{
			
			sprintf( tempMsg, g_LPACK.GetMassage(0,80), m.memberName );			
			g_ifMng->AddSysMessage( tempMsg, D3DCOLOR_ARGB(255, 255, 125, 100));

		}
		
		else
		{
			
			sprintf( tempMsg, g_LPACK.GetMassage(0,81) );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), tempMsg );
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_PARTY_1 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,70), D3DCOLOR_ARGB(255, 255, 55, 55) );
			break;
		case ERROR_PARTY_2 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,68), D3DCOLOR_ARGB(255, 255, 55, 55) );
			break;
		case ERROR_PARTY_3 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,71), D3DCOLOR_ARGB(255, 255, 55, 55));
			break;
		case ERROR_PARTY_4 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,72), D3DCOLOR_ARGB(255, 255, 55, 55) );
			break;
		case ERROR_PARTY_6 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,79), D3DCOLOR_ARGB(255, 255, 55, 55) );
			break;

		}
	}

	return 1;
}

int GTH_SendMessage_Party_Reject_Join( int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_REJECT_JOIN );
		
		MSG_WriteByte(&netMessage, type );
		
		MSG_WriteShort(&netMessage, g_ifMng->m_partyWin->GetPartyMemberIdx() );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_Reject_Join()
{
	int reply = 1, error = 0;
	int type;
	char tempStr[256];
	
	
	
	type = MSG_ReadByte();
	if( type )
	{
		
		sprintf( tempStr,g_LPACK.GetMassage(0,82) , g_ifMng->m_partyWin->GetPartyMemberName() );
		g_ifMng->AddSysMessage( tempStr, D3DCOLOR_ARGB(255, 255, 125, 100) );

		
		g_cgv.saveMenuTime = 5000.0f;
	}
	else
	{
		
		sprintf( tempStr, g_LPACK.GetMassage(0,83), g_ifMng->m_partyWin->GetPartyMemberName() );
		g_ifMng->AddSysMessage( tempStr, D3DCOLOR_ARGB(255, 255, 125, 100) );

	}

	if( g_ifMng->m_msgType == IF_MESSAGE_PARTY_REQUEST_JOIN || 
		g_ifMng->m_msgType == IF_MESSAGE_PARTY_REPLY_JOIN )
	{
		g_ifMng->ExitMessage();
	}
	
	return 1;
}






int GTH_SendMessage_Party_Out()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_OUT );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_Out()
{
	int reqFlag;
	int outMemberIdx, newLeaderIdx;
	char tempStr[256];

	reqFlag = MSG_ReadByte();

	if( !reqFlag )
	{
		outMemberIdx = MSG_ReadByte();
		newLeaderIdx = MSG_ReadByte();

		
		sprintf( tempStr, g_LPACK.GetMassage(0,84), g_cgv.myCharacterInfo->member[outMemberIdx].memberName );

		memset( &g_cgv.myCharacterInfo->member[outMemberIdx], 0, sizeof( partyMember_t ) );
		g_cgv.myCharacterInfo->member[outMemberIdx].serverIndex = -1;
		g_cgv.myCharacterInfo->member[newLeaderIdx].isLeader = true;

		g_cgv.myCharacterInfo->memberNumber --;

		if( !stricmp( g_cgv.myCharacterInfo->name, g_cgv.myCharacterInfo->member[newLeaderIdx].memberName ) )
			g_cgv.myCharacterInfo->isLeader = true;
		else
			g_cgv.myCharacterInfo->isLeader = false;

		g_ifMng->AddSysMessage( tempStr );
	}
	else
	{
		GTH_InitPartyInfo();
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,85), D3DCOLOR_ARGB(255, 255, 125, 100) );
	}

	return 1;
}


int GTH_SendMessage_Party_ForcedOut( char* memberName )
{
	if( !g_cgv.myCharacterInfo->isLeader ) return 1;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_FORCEDOUT );
		
		MSG_WriteString(&netMessage, memberName );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_ForcedOut()
{
	int reqFlag;
	int outMemberIdx, newLeaderIdx;
	char tempStr[256];

	reqFlag = MSG_ReadByte();

	if( !reqFlag )
	{
		outMemberIdx = MSG_ReadByte();
		newLeaderIdx = MSG_ReadByte();

		
		sprintf( tempStr, g_LPACK.GetMassage(0,86), g_cgv.myCharacterInfo->member[outMemberIdx].memberName );


		memset( &g_cgv.myCharacterInfo->member[outMemberIdx], 0, sizeof( partyMember_t ) );
		g_cgv.myCharacterInfo->member[outMemberIdx].serverIndex = -1;
		g_cgv.myCharacterInfo->member[newLeaderIdx].isLeader = true;
		g_cgv.myCharacterInfo->memberNumber --;

		if( !stricmp( g_cgv.myCharacterInfo->name, g_cgv.myCharacterInfo->member[newLeaderIdx].memberName ) )
			g_cgv.myCharacterInfo->isLeader = true;
		else
			g_cgv.myCharacterInfo->isLeader = false;

		g_ifMng->AddSysMessage( tempStr );
	}
	else
	{
		GTH_InitPartyInfo();
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,87), D3DCOLOR_ARGB(255, 255, 125, 100) );

	}

	return 1;
}


int GTH_SendMessage_Party_Disorganize()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_DISORGANIZE );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_Disorganize()
{
	GTH_InitPartyInfo();
	
	g_ifMng->AddSysMessage(g_LPACK.GetMassage(0,88), D3DCOLOR_ARGB(255, 255, 125, 100)  );

	return 1;
}

int GTH_ProcessMessage_Party_Update()
{
	int memberIdx, newLeaderIdx, active, level;

	memberIdx = MSG_ReadByte();
	newLeaderIdx = MSG_ReadByte();

	active = MSG_ReadByte();
	level = MSG_ReadByte();

	g_cgv.myCharacterInfo->member[memberIdx].isActive = active;
	g_cgv.myCharacterInfo->member[memberIdx].level = level;
	g_cgv.myCharacterInfo->member[memberIdx].worldIdx = MSG_ReadShort();

	if( !active )
	{
		g_cgv.myCharacterInfo->member[memberIdx].isLeader = false;
		g_cgv.myCharacterInfo->member[newLeaderIdx].isLeader = true;
		if( !stricmp( g_cgv.myCharacterInfo->member[newLeaderIdx].memberName, g_cgv.myCharacterInfo->name ) )
		{
			g_cgv.myCharacterInfo->isLeader = true;
		}
	}
		
	return 1;
}





int GTH_ProcessMessage_Party_Request_SendAll()
{
	partyMember_t m;
	int i, reply;

	reply = MSG_ReadByte();

	if( reply > 0 )
	{
		strcpy( g_cgv.myCharacterInfo->organizerName, MSG_ReadString() );
		g_cgv.myCharacterInfo->organizeServer = MSG_ReadByte();
		g_cgv.myCharacterInfo->organizeTime = MSG_ReadLong();
		g_cgv.myCharacterInfo->memberNumber = MSG_ReadByte();
		g_cgv.myCharacterInfo->itemDivideType = MSG_ReadByte();
		g_cgv.myCharacterInfo->canJoin = MSG_ReadByte();
		g_cgv.myCharacterInfo->partyIndex = MSG_ReadShort();
		g_cgv.myCharacterInfo->isLeader = MSG_ReadByte();
		
		for( i = 0; i < g_cgv.myCharacterInfo->memberNumber; i ++ )
		{
			MSG_ReadMember( &m );
			memcpy( &g_cgv.myCharacterInfo->member[m.idx], &m, sizeof( partyMember_t ) );
		}
	}
	else
	{
		GTH_InitPartyInfo();
	}

	return 1;
}

int GTH_SendMessage_Party_Request_CanJoin( int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_CANJOIN );
		MSG_WriteByte(&netMessage, type);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_Request_CanJoin()
{
	int type;
	type = MSG_ReadByte();
	if( type ) 
	{
		g_cgv.myCharacterInfo->canJoin = true;
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,89), D3DCOLOR_ARGB(255, 255, 125, 100) );
	}
	else 
	{
		g_cgv.myCharacterInfo->canJoin = false;
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,90), D3DCOLOR_ARGB(255, 255, 125, 100) );
	}
	
	return 1;
}


int GTH_SendMessage_Party_Request_ItemDivideType( int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_ITEMDIVIDETYPE );
		MSG_WriteByte(&netMessage, type);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}


int GTH_ProcessMessage_Party_Request_ItemDivideType()
{
	int type;
	char szTemp[512];

	type = MSG_ReadByte();
	g_cgv.myCharacterInfo->m_bischange= MSG_ReadShort();
	g_cgv.myCharacterInfo->itemDivideType = type;
	
	if( type == 0 )
	{
		
			sprintf( szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 86 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 99 ));
	}
	else if( type == 1)
	{
		
			sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 86 ),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 100 ));
	}
	else if(type == 2)
	{
			sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 86 ),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 101 ));
	}



	g_ifMng->AddSysMessage(szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );

	return 1;
}

int GTH_SendMessage_Party_ChatMessage( char *str )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PARTY );
		MSG_WriteByte(&netMessage, PARTY_CHATMESSAGE );
		MSG_WriteString(&netMessage, str);
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Party_ChatMessage()
{
	char chatMessage[CHATSTRING], fromName[CHATSTRING];
	char szTemp[256];

	strcpy(fromName, MSG_ReadString());
	strcpy(chatMessage, MSG_ReadString());

	sprintf( szTemp, "%s(from Party)", fromName );

	g_ifMng->AddChatMessage( chatMessage, GTHCOLOR_PARTYMESSAGE, szTemp, IF_CHATMSG_TYPE_PARTY );
	
	return 1;
}

