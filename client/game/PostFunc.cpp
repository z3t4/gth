
#include "../global_def.h"

CPostFunc::CPostFunc(){ }

CPostFunc::~CPostFunc(){ }

#include "..\CTools.h"


void CPostFunc::GTH_ProcessMessage_RecvPostSystem()
{
	
	int command = MSG_ReadByte();	


	switch( command )
	{
	case POSTSYSTEM_OPEN:			
		GTH_ProcessMessage_PostSystem_OpenWindow();
		break;

	case POSTSYSTEM_EXIT_WIN:		
		GTH_ProcessMessage_PostSystem_ExitWindow();
		break;

	case POSTSYSTEM_SEND :				
		GTH_ProcessMessage_PostSystem_Send();
		break;

	case POSTSYSTEM_DELETE:
		GTH_ProcessMessage_PostSystem_DeletePost();
		break;


	case POSTSYSTEM_STATE:				
		GTH_ProcessMessage_PostSystem_State();
		break;

	case POSTSYSTEM_WRITEOPEN:			
		GTH_ProcessMessage_PostSystem_WriteOpen();
		break;


	case POSTSYSTEM_SENDCOMPLETE:		
		GTH_ProcessMessage_PostSystem_SendComplete();
		break;
		
	case POSTSYSTEM_NOTIFY :			
		GTH_ProcessMessage_PostSystem_Arrive();
		break;				
	}
}




void CPostFunc::GTH_ProcessMessage_PostSystem_SendComplete(void)
{
	int reply = MSG_ReadLong();
	int error = MSG_ReadByte();

	if(reply)
		g_ifMng->m_mailBoxWin->SendItemDelete();

}

bool CPostFunc::Safe_ItemBox(int index)
{
	if(index < 0 || index >= MAX_NUMBER_OF_OWNITEM) return false;
	return true;
}

int CPostFunc::GetItemCount()
{	
	int total=0;
	for(int pos = 0; pos < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; pos++)
	{
		int index = g_cgv.myCharacterInfo->m_ItemsInPost[pos];
		
		if(!Safe_ItemBox(index)) continue;		
		
		if(&g_cgv.myCharacterInfo->item[index] != NULL)
			total++;
	}
	return total;
}







void CPostFunc::GTH_SendMessage_PostSystem_Send()
{

	
	char    name[NAMESTRING+1];
	char	title[CPostManager::POST_TITLESIZE+1];
	char	message[CPostManager::POST_STRSIZE+1];



	strncpy(name,g_ifMng->m_mailBoxWin->m_name,NAMESTRING);
	name[NAMESTRING]=NULL;
	CTools::Replace_singleQUOTATIONmark_by_doubleQUOTATIONmark(name);


	strncpy(title,g_ifMng->m_mailBoxWin->m_title,CPostManager::POST_TITLESIZE);
	title[CPostManager::POST_TITLESIZE]=NULL;
	CTools::Replace_singleQUOTATIONmark_by_doubleQUOTATIONmark(title);

	strncpy(message,g_ifMng->m_mailBoxWin->m_message,CPostManager::POST_STRSIZE);
	message[CPostManager::POST_STRSIZE]=NULL;
	CTools::Replace_singleQUOTATIONmark_by_doubleQUOTATIONmark(message);







	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND);
		MSG_WriteByte(&netMessage, CC_POSTSYSTEM);
		MSG_WriteByte(&netMessage, POSTSYSTEM_SEND);
		
		MSG_WriteString(&netMessage, name);
		MSG_WriteString(&netMessage, title);
		MSG_WriteString(&netMessage, message);
		MSG_WriteLong(&netMessage, g_ifMng->m_mailBoxWin->m_nak);
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );	
}






int CPostFunc::GTH_SendMessage_PostSystem_ReceiveItems()
{	
	CPostManager::PostPackage_t* pPostPackage = NULL;
	pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();	
	int	OpenIdx = g_cgv.myCharacterInfo->m_PostMng.m_OpenPostIdx;

	
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_MAILBOX );
		MSG_WriteByte(&netMessage, MAILBOX_RECEIVE_ITEMS );		
		MSG_WriteLong(&netMessage, OpenIdx );				
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
	
	return 1;

	

}






inline void CPostFunc::GTH_ProcessMessage_PostSystem_ReceiveItems_Success(void)
{
	CPostManager::PostPackage_t* pPostPackage = NULL;
	pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();		
	if(NULL == pPostPackage) return;
	
	
	int inventoryPos	= -1;
	int itemIdx			= -1;
	for ( int i = 0; i < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; i ++)						
	{
		
		item_t *pitem = NULL;
		pitem = &pPostPackage->Items[i];
		if ( pitem == NULL || pitem->itemTableIdx < 0)
			continue;
		
		int stackItemIdx = GTH_StackMyItem( pitem );
		if( stackItemIdx >= 0 )
		{
			g_cgv.myCharacterInfo->item[stackItemIdx].durability += pitem->durability + 1;
			if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
				g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
		}
		else
		{
			inventoryPos = GTH_FindEmptyInventoryPos();
			itemIdx = GTH_AddMyItem( pitem );
			g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
		}
		
		GTH_InitItem(pitem);
		pPostPackage->ItemNumber--;
		
	}
	
	g_cgv.myCharacterInfo->curChargeSE +=pPostPackage->Nak;
	pPostPackage->Nak = 0;

	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_EQUIP_ITEM ), GTH_SOUND_PLAY_ONCE );

	
	
	GTH_SendMessageRequestItemSyncItem();



}
void CPostFunc::GTH_ProcessMessage_PostSystem_ReceiveItems()
{
	int reply = MSG_ReadLong();	
	int error = 1;

	if ( reply == 1)	
		GTH_ProcessMessage_PostSystem_ReceiveItems_Success();

	else	
	{
		error = MSG_ReadLong();

		switch( error )
		{		
			
		case 3:
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 217) );
			break;
			
		
		case 4:
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 262) );
			break;
			
		default:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 227) );
			break;
		}
		
	}			

	g_chatManager->SetInputState( INPUT_INDEX_POST_NAME, -1 );
	g_chatManager->SetInputState( INPUT_INDEX_POST_MESSAGE, -1 );
	g_chatManager->SetInputState( INPUT_INDEX_POST_TITLE, -1 );
	g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

}

void CPostFunc::GTH_SendMessage_PostSystem_State()
{

	int state = g_cgv.myCharacterInfo->m_PostMng.GetState();
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_POSTSYSTEM );
		MSG_WriteByte(&netMessage, POSTSYSTEM_STATE );	
		MSG_WriteByte(&netMessage, state);
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );	

}

void CPostFunc::GTH_SendMessage_PostSystem_View()
{

}




void CPostFunc::GTH_ProcessMessage_PostSystem_OpenWindow()
{

	
	int	PostIdx = 0;
	int PostCount = -1;
	

	enum tagGCPacket_POSTSYSTEM_OPEN::enumCode code =
		tagGCPacket_POSTSYSTEM_OPEN::enumCode::fail;
	code = static_cast<enum tagGCPacket_POSTSYSTEM_OPEN::enumCode>(MSG_ReadByte());	
	
	
	switch( code ) 
	{
	case tagGCPacket_POSTSYSTEM_OPEN::enumCode::success:
		{
			PostCount = MSG_ReadLong();	

			g_cgv.myCharacterInfo->m_PostMng.InitPostSystem();		
			
			for ( PostIdx = 0;  PostIdx < PostCount; PostIdx++)
			{
				CPostManager::PostPackage_t PostPackage;
				memset(&PostPackage, 0, sizeof(CPostManager::PostPackage_t) );
				
				PostPackage.iPostIdx		= MSG_ReadLong();
				sstrncpy(PostPackage.szFromName, MSG_ReadString(), NAMESTRING);
				PostPackage.szFromName[NAMESTRING] = NULL;
				CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(PostPackage.szFromName);
				
				sstrncpy(PostPackage.szPostTitle,  MSG_ReadString(), CPostManager::POST_TITLESIZE);
				PostPackage.szPostTitle[CPostManager::POST_TITLESIZE] = NULL;
				CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(PostPackage.szPostTitle);

				
				sstrncpy(PostPackage.szSendPostTime	, MSG_ReadString(), CPostManager::POST_SENDTIME_INFO_STR_LEN);
				PostPackage.szSendPostTime[CPostManager::POST_SENDTIME_INFO_STR_LEN] = NULL;
				


				sstrncpy(PostPackage.szMailStr, MSG_ReadString(), CPostManager::POST_STRSIZE);
				PostPackage.szMailStr[CPostManager::POST_STRSIZE] = NULL;
				CTools::Replace_doubleQUOTATIONmark_by_singleQUOTATIONmark(PostPackage.szMailStr);
				
				int state = MSG_ReadLong();
				
				if ( state == 0)		
					PostPackage.MailState  =  CPostManager::enumPostPackageState::POSTPACKAGE_UNCHECK;
				else
					PostPackage.MailState  =  CPostManager::enumPostPackageState::POSTPACKAGE_CHECK;
				
				
				
				int nak = MSG_ReadLong();
				PostPackage.Nak = nak;

				int SendType = MSG_ReadLong();				
				
				
				PostPackage.iRemainDays = MSG_ReadLong();	


				if ( SendType == 0)
					PostPackage.PostSendType = CPostManager::enumPostSendType::POST_SENDTYPE_WEB;
				else
					PostPackage.PostSendType = CPostManager::enumPostSendType::POST_SENDTYPE_CHAR;
				
				
				
				g_cgv.myCharacterInfo->m_PostMng.AddPostPackage(PostPackage);				
				
			}					
			
			
				g_ifMng->m_PostWin->InitIFPost();
				g_ifMng->m_PostWin->SetDisplayMode(true);				
				g_ifMng->m_chatWin->Enable( false );

				g_ifMng->m_PostWin->Enable(1);
				g_ifMng->SetFocus( g_ifMng->m_PostWin );				

		}
		break;		

	case tagGCPacket_POSTSYSTEM_OPEN::enumCode::fail:
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 226) );		
		break;

	default:
		break;		
	}	
	

}





void CPostFunc::GTH_ProcessMessage_PostSystem_Send()
{

	int reply = -1, error = -1;

	reply = MSG_ReadByte();
	if(reply > 0)
	{
		g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 263));
		
	}
	else
	{
		error = MSG_ReadByte();
		switch(error)
		{
		case 0:
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 264));
			
			break;
		case 1:
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 265));
			
			break;
		case 2:
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 266));
			
			break;
		case 3:
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 270));
			
			break;
		default:
			break;
		}
	}

}


void CPostFunc::GTH_ProcessMessage_PostSystem_State()
{

	int reply = -1;
	
	reply = MSG_ReadByte();
	
	if(reply)
		g_cgv.myCharacterInfo->m_PostMng.InverseState();

}

void CPostFunc::GTH_SendMessage_PostSystem_WriteOpen(void)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND);
		MSG_WriteByte(&netMessage, CC_POSTSYSTEM);
		MSG_WriteByte(&netMessage, POSTSYSTEM_WRITEOPEN);		
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
}

void CPostFunc::GTH_ProcessMessage_PostSystem_WriteOpen(void)
{
	int reply = -1;
	int error = -1;
	reply	=	MSG_ReadByte();

	g_ifMng->m_mailBoxWin->InitMailBoxWin();

	if ( reply == TRUE )
	{			
		g_chatManager->InitInputStr(INPUT_INDEX_POST_NAME);
		g_chatManager->InitInputStr(INPUT_INDEX_POST_TITLE);
		g_chatManager->InitInputStr(INPUT_INDEX_POST_MESSAGE);
		
		g_ifMng->m_PostWin->Enable(false);		
		g_ifMng->m_PostWin->m_bDisplay = false;
		
		g_ifMng->m_charWin->Enable(false);
		g_ifMng->m_itemWin->Enable(true);
		g_ifMng->m_mailBoxWin->Enable(true);
		g_ifMng->m_mailBoxWin->m_bDisplay = true;
		g_ifMng->SetFocus(g_ifMng->m_mailBoxWin);


		
		CPostManager::PostPackage_t postpackage;		
		g_cgv.myCharacterInfo->m_PostMng.InitPostSystem();
		g_cgv.myCharacterInfo->m_PostMng.SetPostSystem(CPostManager::enumPostPackageMode::POSTSYSTEM_WRITEMODE);		
		g_cgv.myCharacterInfo->m_PostMng.InitPackage(&postpackage);
		g_cgv.myCharacterInfo->m_PostMng.AddPostPackage(postpackage);
		g_cgv.myCharacterInfo->m_PostMng.SetOpenPost(g_cgv.myCharacterInfo->m_PostMng.GetPostCount() - 1);		
	}
	else
	{

		error = MSG_ReadByte();
		if ( error == 0)
		{
			
		}
	}
}
void CPostFunc::GTH_ProcessMessage_PostSystem_View()
{

}

void CPostFunc::GTH_ProcessMessage_PostSystem_Arrive()
{
	int iUnReadedCnt = MSG_ReadLong();
	int iReadCnt = MSG_ReadLong();

	if(iUnReadedCnt >= 1){
		char message[_MAX_PATH];		
		wsprintf(message,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,267),iUnReadedCnt);
		g_ifMng->AddSysMessage(message, GTHCOLOR_NOTICEMESSAGE );
		
		g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,268), GTHCOLOR_NOTICEMESSAGE );
	}
}


void CPostFunc::GTH_ProcessMessage_MailBox_Open()
{

	int reply = -1;
	int itemNumber;
	int PostIdx = -1;	

	CPostManager::PostPackage_t* pPostPackage = NULL;
	reply = MSG_ReadByte();		

	g_ifMng->m_mailBoxWin->InitMailBoxWin();

	if ( reply )
	{
		PostIdx		= MSG_ReadLong();
		if ( PostIdx <0 ) 
			return;				

		pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();
		
		itemNumber = MSG_ReadLong();
		
		if ( pPostPackage == NULL)
			return;
		
		pPostPackage->ItemNumber = itemNumber;

		for (int i = 0; i < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; i++)
		{
			GTH_InitItem(&pPostPackage->Items[i]);

		
			MSG_ReadItem(&pPostPackage->Items[i]);
		}
		

		g_ifMng->m_PostWin->Enable(false);		
		g_ifMng->m_PostWin->m_bDisplay = false;
		g_ifMng->m_charWin->Enable(false);
		g_ifMng->m_itemWin->Enable(true);
		g_ifMng->m_mailBoxWin->Enable(true);
		g_ifMng->m_mailBoxWin->m_bDisplay = true;		
		g_ifMng->SetFocus(g_ifMng->m_mailBoxWin);
	}
	else
	{  
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 227) );
	}

	
}









void CPostFunc::GTH_ProcessMessage_RecvMailBox()
{
	int command;
	
	command = MSG_ReadByte();

	switch( command )
	{

	case MAILBOX_OPEN :
		GTH_ProcessMessage_MailBox_Open();
		break;
	
	case MAILBOX_EXIT :
		GTH_ProcessMessage_MailBox_Exit();
		break;
	
	case MAILBOX_ITEM :
		GTH_ProcessMessage_MailBox_Item();
		break;
		
	case MAILBOX_RECEIVE_ITEMS :
		GTH_ProcessMessage_PostSystem_ReceiveItems();
		break;

	case MAILBOX_UNDOINVENITEM:
		GTH_ProcessMessage_MailBox_UndoInvenItem();
		break;
	}
}
	
void CPostFunc::GTH_ProcessMessage_MailBox_UndoInvenItem(void)
{
	GTH_UnlockSending();

	int reply = 1, error = -1;
	
	reply = MSG_ReadLong();
	if(reply)
		g_ifMng->m_mailBoxWin->UndoMailBoxInventory();		
	else
		error = MSG_ReadByte();
}

bool CPostFunc::GTH_SendMessage_MailBox_UndoInvenItem(void)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage, CC_EXTEND);
		MSG_WriteByte(&netMessage, CC_MAILBOX);
		MSG_WriteByte(&netMessage, MAILBOX_UNDOINVENITEM);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	return true;
}







int CPostFunc::GTH_SendMessage_MailBox_Open(int PostType, int PostIdx)
{
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_MAILBOX );
		MSG_WriteByte(&netMessage, MAILBOX_OPEN );		
		MSG_WriteLong(&netMessage, PostType );		
		MSG_WriteLong(&netMessage, PostIdx );		
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}







int CPostFunc::GTH_SendMessage_MailBox_Item( int idx, int pos )
{
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_MAILBOX );
		MSG_WriteByte(&netMessage, MAILBOX_ITEM );
		
		
		MSG_WriteByte(&netMessage, pos );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int CPostFunc::GTH_ProcessMessage_MailBox_Item()
{
	GTH_UnlockSending();
	
	int reply = 1, error = 0;
	int pos, isOwner, ItemTableIdx;
	
	
	CPostManager::PostPackage_t* pPostPackage = NULL;
	pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();
	
	reply = MSG_ReadLong();

	if( reply > 0 )
	{
		isOwner = MSG_ReadByte();
		pos = MSG_ReadByte();
		g_cgv.myCharacterInfo->mouseInventory = MSG_ReadShort();
		g_cgv.myCharacterInfo->m_ItemsInPost[pos] = MSG_ReadShort();
		ItemTableIdx = MSG_ReadShort();

	}
	else
	{
		error = MSG_ReadByte();

		
		g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 269));
		
	}
	return 1;
}













void CPostFunc::GTH_ProcessMessage_MailBox_Exit()
{

	
	CPostManager::PostPackage_t* pPostPackage = NULL;
	pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetOpenPost();

	for (int i = 0; i < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; i ++)
	{	
		if ( g_cgv.myCharacterInfo->m_ItemsInPost[i] < 0)
			continue;
		int pos = g_cgv.myCharacterInfo->m_ItemsInPost[i];
	}
	g_ifMng->m_mailBoxWin->Enable(false);
	g_ifMng->SetFocus(g_ifMng->m_PostWin);
}






int CPostFunc::GTH_SendMessage_MailBox_Exit()
{
	g_ifMng->m_mailBoxWin->UndoMailBoxInventory();

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_MAILBOX );
		MSG_WriteByte(&netMessage, MAILBOX_EXIT );	
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;

}






void CPostFunc::GTH_ProcessMessage_PostSystem_ExitWindow()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_POSTSYSTEM );
		MSG_WriteByte(&netMessage, POSTSYSTEM_EXIT_WIN );	
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );	

}

void CPostFunc::GTH_SendMessage_PostSystem_DeletePost()
{		
	int DeleteCount = g_cgv.myCharacterInfo->m_PostMng.GetCheckCount();	

	if ( DeleteCount < 0 )
		DeleteCount = 0;

	int MaxPostCount = g_cgv.myCharacterInfo->m_PostMng.GetPostCount();

	if ( DeleteCount > MaxPostCount)
		DeleteCount = MaxPostCount;


	CPostManager::PostPackage_t* pPostPackage = NULL;	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_POSTSYSTEM );
		MSG_WriteByte(&netMessage, POSTSYSTEM_DELETE );	
		MSG_WriteLong(&netMessage, DeleteCount);

		for (int PostIdx = 0; PostIdx < MaxPostCount; PostIdx ++)	
		{
			pPostPackage = g_cgv.myCharacterInfo->m_PostMng.GetPostPackage(PostIdx);			

			if ( pPostPackage == NULL || pPostPackage->bCheck == false)
				continue;

				MSG_WriteLong(&netMessage, PostIdx);				
		}
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );	


	

}

void CPostFunc::GTH_ProcessMessage_PostSystem_DeletePost(void)
{
	int reply = -1, error = -1;

	reply = MSG_ReadLong();	

	if(reply <= 0)
	{
		error = MSG_ReadLong();
		switch(error)
		{
		case 0:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),  
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 281));			
			break;

		default:
			break;
		}
	}
}