 #include "../global_def.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 

int GTH_ProcessMessage_PCTrade()
{
	int command;
	
	command = MSG_ReadByte();

	switch( command )
	{
	case PCTRADE_REQUEST :
		GTH_ProcessMessage_PCTrade_Request();
		break;
	case PCTRADE_ADMIT :
		GTH_ProcessMessage_PCTrade_Admit();
		break;
	case PCTRADE_REJECT :
		GTH_ProcessMessage_PCTrade_Reject();
		break;
	case PCTRADE_EXIT :
		GTH_ProcessMessage_PCTrade_Exit();
		break;
	case PCTRADE_ITEM :
		GTH_ProcessMessage_PCTrade_Item();
		break;
	case PCTRADE_OK :
		GTH_ProcessMessage_PCTrade_Ok();
		break;
	case PCTRADE_CANCEL :
		GTH_ProcessMessage_PCTrade_Cancel();
		break;
	case PCTRADE_CONFIRM :
		GTH_ProcessMessage_PCTrade_Confirm();
		break;

	case PCTRADE_SE :
		GTH_ProcessMessage_PCTrade_SE();
		break;
	}

	return 1;
}


int GTH_SendMessage_PCTrade_Request( int idxOnServer )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		
		MSG_WriteByte(&netMessage, PCTRADE_REQUEST );
		MSG_WriteShort(&netMessage, idxOnServer);
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}


int GTH_ProcessMessage_PCTrade_Request()
{
	int idxOnServer, type;
	int reply = 1, error = 0;
	char name[128], szTemp[256];

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		
		idxOnServer = MSG_ReadShort();
		type = MSG_ReadByte();
		strcpy( name, MSG_ReadString() );
		
		if( !type )
		{
			
			sprintf( szTemp, "%s%s", name, g_LPACK.GetMassage(0,55) );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, 0, IF_MESSAGE_PCTRADE_REQUEST );
			g_ifMng->m_tradeWin->SetTradeInfo( idxOnServer, name );
		}
		else
		{
			


#if defined _GTH_CHINESE_VERSION || _GTH_TAIWAN_VERSION
			sprintf( szTemp, g_LPACK.GetMassage(0,58), name );
#else
			sprintf( szTemp, "%s%s", name, g_LPACK.GetMassage(0,58) );
#endif		
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_CANCEL, IF_MESSAGE_PCTRADE_WAITREPLY, 10000.0f );
			g_ifMng->m_tradeWin->SetTradeInfo( idxOnServer, name );
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{ 
		case ERROR_ENTITYMENU_1 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,56), D3DCOLOR_ARGB(255, 255, 55, 55) );
			break;
		case ERROR_ENTITYMENU_2 : case ERROR_ENTITYMENU_3 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,57), D3DCOLOR_ARGB(255, 255, 55, 55)  );
			break;
		}
	}
	return 1;
}

int GTH_SendMessage_PCTrade_Admit()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		
		MSG_WriteByte(&netMessage, PCTRADE_ADMIT );
		MSG_WriteShort(&netMessage, g_ifMng->m_tradeWin->m_otherIdxOnServer );
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_Admit()
{
	int reply = 1, error = 0;

	if( g_ifMng->m_msgType == IF_MESSAGE_PCTRADE_REQUEST || 
		g_ifMng->m_msgType == IF_MESSAGE_PCTRADE_WAITREPLY )
	{
		g_ifMng->ExitMessage();
	}

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		g_ifMng->m_tradeWin->InitTrade();
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{ 
		case ERROR_ENTITYMENU_1 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,56), D3DCOLOR_ARGB(255, 255, 55, 55) );
			break;
		case ERROR_ENTITYMENU_2 : 
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,57), D3DCOLOR_ARGB(255, 255, 55, 55)  );
			break;
		case ERROR_ENTITYMENU_3 :
			break;
		}
	}

	return 1;
}

int GTH_SendMessage_PCTrade_Reject( int type )
{
	
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		
		MSG_WriteByte(&netMessage, PCTRADE_REJECT );
		MSG_WriteByte(&netMessage, type);
		MSG_WriteShort(&netMessage, g_ifMng->m_tradeWin->m_otherIdxOnServer );
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_Reject()
{
	int reply = 1, error = 0;
	int type;
	char szTemp[256];
	
	
	

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		type = MSG_ReadByte();
		if( type )
		{
			
			g_cgv.saveMenuTime = 5000.0f;
		
			
			sprintf( szTemp, g_LPACK.GetMassage(0,74), g_ifMng->m_tradeWin->m_otherName );
			g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );
		}
		else
		{
			
			sprintf( szTemp, g_LPACK.GetMassage(0,74), g_ifMng->m_tradeWin->m_otherName );
			g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );
		}
	}
	else
	{
		error = MSG_ReadByte();
	}

	if( g_ifMng->m_msgType == IF_MESSAGE_PCTRADE_REQUEST || 
		g_ifMng->m_msgType == IF_MESSAGE_PCTRADE_WAITREPLY )
	{
		g_ifMng->ExitMessage();
	}

	return 1;
}

int GTH_SendMessage_PCTrade_Item( int idx, int pos )
{
	if( !GTH_LockSending() ) return 1;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		MSG_WriteByte(&netMessage, PCTRADE_ITEM );
		
		MSG_WriteShort(&netMessage, idx );
		MSG_WriteByte(&netMessage, pos );
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_Item()
{
	GTH_UnlockSending();

	int reply = 1, error = 0;
	int type, pos, isOwner, itemIdx;
	item_t item;

	reply = MSG_ReadByte();
	
	if( reply > 0 )
	{
		isOwner = MSG_ReadByte();
		type = MSG_ReadByte();
		pos = MSG_ReadByte();

		if( isOwner )
		{			
			switch( type )
			{
				
			case 0 :
				g_cgv.myCharacterInfo->myTrade[pos] = g_cgv.myCharacterInfo->mouseInventory;
				g_cgv.myCharacterInfo->mouseInventory = -1;
				break;
				
			case 1 :
				g_cgv.myCharacterInfo->mouseInventory = g_cgv.myCharacterInfo->myTrade[pos];
				g_cgv.myCharacterInfo->myTrade[pos] = -1;
				break;
				
			case 2 :
				itemIdx = g_cgv.myCharacterInfo->mouseInventory;
				g_cgv.myCharacterInfo->mouseInventory = g_cgv.myCharacterInfo->myTrade[pos];
				g_cgv.myCharacterInfo->myTrade[pos] = itemIdx;
				break;
			}
		}
		else
		{
			switch( type )
			{
				
			case 0 :
				MSG_ReadItem( &item );
				memcpy( &g_cgv.myCharacterInfo->tradeItem[pos], &item, sizeof( item_t ) );
				g_cgv.myCharacterInfo->otherTrade[pos] = pos;
				break;
				
			case 1 :
				memset( &g_cgv.myCharacterInfo->tradeItem[pos], 0, sizeof( item_t ) );
				g_cgv.myCharacterInfo->tradeItem[pos].itemTableIdx = -1;
				g_cgv.myCharacterInfo->otherTrade[pos] = -1;
				break;
				
			case 2 :
				MSG_ReadItem( &item );
				memcpy( &g_cgv.myCharacterInfo->tradeItem[pos], &item, sizeof( item_t ) );
				g_cgv.myCharacterInfo->otherTrade[pos] = pos;
				break;
			}
		}

	}
	else
	{
		error = MSG_ReadByte();
	}
	return 1;
}

int GTH_SendMessage_PCTrade_Ok( int idx )
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		
		MSG_WriteByte(&netMessage, PCTRADE_OK );
		MSG_WriteShort(&netMessage, idx );
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_Ok()
{
	int reply = 1, error = 0, type;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		type = MSG_ReadByte();
		g_ifMng->m_tradeWin->SetReady( type, true );
	}
	else
	{
		error = MSG_ReadByte();
	}
	return 1;
}

int GTH_SendMessage_PCTrade_Cancel( int idx )
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		
		MSG_WriteByte(&netMessage, PCTRADE_CANCEL );
		MSG_WriteShort(&netMessage, idx );
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_Cancel()
{
	int reply = 1, error = 0, type;




	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		type = MSG_ReadByte();
		if( type ) 
		{
			g_ifMng->m_tradeWin->SetReady( 0, false );
			g_ifMng->m_tradeWin->SetReady( 1, false );
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,75), D3DCOLOR_ARGB(255, 255, 125, 100) );
		}
		else 
		{
			g_ifMng->m_tradeWin->SetReady( 0, false );
			g_ifMng->m_tradeWin->SetReady( 1, false );
		}
	}
	else
	{
		error = MSG_ReadByte();
	}
	return 1;
}

int GTH_SendMessage_PCTrade_Exit()
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		
		MSG_WriteByte(&netMessage, PCTRADE_EXIT );
		MSG_WriteShort(&netMessage, g_ifMng->m_tradeWin->m_otherIdxOnServer );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}



int GTH_ProcessMessage_PCTrade_Exit()
{
	int reply = 1, error = 0;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
	}
	else
	{
		error = MSG_ReadByte();
	}

	
	g_ifMng->m_tradeWin->CancelTrade();

	return 1;
}

int GTH_SendMessage_PCTrade_Confirm( int idx, int type )
{


	
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		
		MSG_WriteByte(&netMessage, PCTRADE_CONFIRM );
		MSG_WriteByte(&netMessage, type );
		
		MSG_WriteShort(&netMessage, idx );
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_Confirm()
{
	int reply = 1, error = 0, type;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		type = MSG_ReadByte();
		switch( type )
		{
			
		case 0 :

			
			

			break;
			
		case 1 :
			g_ifMng->m_tradeWin->SetConfirm( true );
			break;
			
		case 2 :
			g_ifMng->m_tradeWin->ConfirmTrade();
			break;
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_PCTRADE_1 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,61) );
			break;
		case ERROR_PCTRADE_2 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,49) );
			break;
		case ERROR_PCTRADE_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,62) );
			break;
		case ERROR_PCTRADE_4 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(0,63));
			break;
		case ERROR_PCTRADE_5 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,36));
			break;
		}
		g_ifMng->m_tradeWin->CancelTrade();
	}
	return 1;
}

int GTH_SendMessage_PCTrade_SE( int idx, int se )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCTRADE );
		MSG_WriteByte(&netMessage, PCTRADE_SE );
		MSG_WriteShort(&netMessage, idx );
		MSG_WriteLong(&netMessage, se );
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_SE()
{
	int type, se;
	int reply, error;

	reply = MSG_ReadByte();
	if( reply )
	{
		type = MSG_ReadByte();
		se = MSG_ReadLong();
		
		if( type ) g_ifMng->m_tradeWin->m_myTradeSE = se;
		else g_ifMng->m_tradeWin->m_otherTradeSE = se;
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_PCTRADE_5 :
			
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,36), D3DCOLOR_ARGB(255, 255, 125, 100) );
			break;
		}
	}
	
	return 1;
}