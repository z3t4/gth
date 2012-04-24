



#include "../Global.h"
#include <VECTOR>


#include "..\Tools\CTools.h"
#include "../Tools/CTempPlayerInventoryX.h"
extern CTools* gcpTools;





CPostFunc::CPostFunc()
{

}

CPostFunc::~CPostFunc()
{

}

void CPostFunc::GTH_ProcessMessage_PostSystem()
{

	byte command = MSG_ReadByte();

	switch ( command )
	{		
		
	case POSTSYSTEM_OPEN:
		GTH_ProcessMessage_RequestPostList();
		break;

	case POSTSYSTEM_EXIT_WIN:
		GTH_ProcessMessage_PostWinExit();
		break;

		
	case POSTSYSTEM_SEND:
		GTH_ProcessMessage_PostSend();
		break;			

		
	case POSTSYSTEM_DELETE:
		GTH_ProcessMessage_PostDelete();
		break;		
	
	case POSTSYSTEM_STATE:
	
		GTH_ProcessMessage_PostState();
	
		break;
	case POSTSYSTEM_WRITEOPEN:
		GTH_ProcessMessage_PostWriteOpen();
		break;
	case POSTSYSTEM_SENDCOMPLETE:

		break;

	case POSTSYSTEM_DELETENAK:
		break;

	case POSTSYSTEM_NOTIFY:
		break;
	}
}


	




void CPostFunc::
GTH_SendMessage_PostSendComplete(playerCharacter_t* pc,int reply,  int error)
{	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_POSTSYSTEM);
		MSG_WriteByte(&netMessage, POSTSYSTEM_SENDCOMPLETE);
		
		
		MSG_WriteLong(&netMessage, reply);
		MSG_WriteByte(&netMessage, error);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}





void CPostFunc::GTH_ProcessMessage_PostWriteOpen(void)
{	
	
	
	CPostManager::PostPackage_t PostPackage;
	

	g_curPC->m_PostMng.InitPostSystem();
	g_curPC->m_PostMng.SetPostSystem(CPostManager::enumPostSystemMode::POSTSYSTEM_WRITEMODE);	
	g_curPC->m_PostMng.InitPost(&PostPackage);	
	g_curPC->m_PostMng.AddPostPackage(&PostPackage);	
	g_curPC->m_PostMng.SetOpenPost(g_curPC->m_PostMng.GetPostCount()-1);
	GTH_SendMessage_PostWriteOpen(g_curPC, 1,-1);
}




void CPostFunc::GTH_SendMessage_PostWriteOpen(playerCharacter_t* pc, int reply, int error)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_POSTSYSTEM);
		MSG_WriteByte(&netMessage, POSTSYSTEM_WRITEOPEN);
		MSG_WriteByte(&netMessage, reply);
		MSG_WriteByte(&netMessage, error);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}






void CPostFunc::GTH_ProcessMessage_RequestPostList()
{
	
	if ( g_curPC == NULL)
		return;

	
	g_DBGameServer->LoadPackageList(g_curPC);

}







void CPostFunc::
GTH_SendMessage_PostList(
	playerCharacter_t* pc,
	const enum tagGCPacket_POSTSYSTEM_OPEN::enumCode code)
{
	CPostManager::PostPackage_t* pPostPackage = NULL;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND );
		MSG_WriteByte(&netMessage, GSC_POSTSYSTEM );
		MSG_WriteByte(&netMessage, POSTSYSTEM_OPEN );
		MSG_WriteByte(&netMessage, code);

		
			
			int PostCount = pc->m_PostMng.GetPostCount();
			MSG_WriteLong(&netMessage,  PostCount);				
			
			for (int i =0; i < PostCount; i ++)
			{
				pPostPackage = pc->m_PostMng.GetPostPackage(i);	
				MSG_WriteLong( &netMessage, pPostPackage->iPostIdx );
				MSG_WriteString( &netMessage, pPostPackage->szFromName );
				MSG_WriteString( &netMessage, pPostPackage->szPostTitle );
				MSG_WriteString( &netMessage, pPostPackage->szSendPostTime );
				MSG_WriteString( &netMessage, pPostPackage->szMailStr );
				MSG_WriteLong( &netMessage, (int)pPostPackage->MailState );			
				
				MSG_WriteLong( &netMessage, (int)pPostPackage->Nak );			
				MSG_WriteLong( &netMessage, (int)pPostPackage->PostSendType );	

				
				MSG_WriteLong( &netMessage, (int)pPostPackage->iRemainDays );
				
			}	
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}







void CPostFunc::GTH_ProcessMessage_PostSend()
{
	CPostManager::PostPackage_t* pPostPackage = NULL;
	pPostPackage = g_curPC->m_PostMng.GetOpenPost();
	if(NULL == pPostPackage){
		GTH_SendMessage_PostSend(g_curPC, -1, 2);
		return;
	}

	g_curPC->busyState = BUSY_STATE_NONE;
	sstrncpy(pPostPackage->szFromName, MSG_ReadString(), NAMESTRING);
	pPostPackage->szFromName[NAMESTRING] = NULL;
	
	
	sstrncpy(pPostPackage->szPostTitle, MSG_ReadString(), CPostManager::POST_TITLESIZE);
	pPostPackage->szPostTitle[CPostManager::POST_TITLESIZE]=NULL;

	sstrncpy(pPostPackage->szMailStr, MSG_ReadString(), CPostManager::POST_STRSIZE);
	pPostPackage->szMailStr[CPostManager::POST_STRSIZE] = NULL;
	pPostPackage->Nak = MSG_ReadLong();
	
	if(pPostPackage->Nak < 0){
		g_PostFunc.GTH_ProcessMessage_MailBox_UndoInvenItem(g_curPC);
		g_logSystem->Write("SavePackage Error!! : (ID: %s)", g_curPC->userID);				
		g_PostFunc.GTH_SendMessage_PostSend(g_curPC, FALSE,0);
		return;
	}

	
	if ( g_curPC->curChargeSE - pPostPackage->Nak < 0 )
	{
		
		g_PostFunc.GTH_ProcessMessage_MailBox_UndoInvenItem(g_curPC);
		g_logSystem->Write("SavePackage Error!! : (ID: %s)", g_curPC->userID);				
		g_PostFunc.GTH_SendMessage_PostSend(g_curPC, FALSE,0);
		return;
	}
	
	g_DBGameServer->SavePackage(
		g_curPC, 
		pPostPackage->szPostTitle, 
		pPostPackage->szMailStr, 
		pPostPackage->szFromName,
		pPostPackage->Nak);										
	
	return;
}






void CPostFunc::GTH_SendMessage_PostSend(playerCharacter_t* pc,  int reply, int errorIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_POSTSYSTEM);
		MSG_WriteByte(&netMessage, POSTSYSTEM_SEND);
		
		MSG_WriteByte(&netMessage, reply);
		MSG_WriteByte(&netMessage, errorIdx);

		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}






void CPostFunc::GTH_ProcessMessage_MailBox()
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

	case MAILBOX_RECEIVE_ITEMS:
		GTH_ProcessMessage_MailBox_ReceiveItems(g_curPC);
		break;

	case MAILBOX_UNDOINVENITEM:
		GTH_ProcessMessage_MailBox_UndoInvenItem(g_curPC);
		break;
	}
}






void CPostFunc::GTH_ProcessMessage_MailBox_UndoInvenItem(playerCharacter_t* pc)
{	
	CPostManager::PostPackage_t* pPostPackage = NULL;
	pPostPackage = pc->m_PostMng.GetOpenPost();
	if(NULL == pPostPackage) 
	{
		GTH_SendMessage_MailBox_UndoInvenItem(pc, 1, -1);
		return;
	}

	for(int idx=0; idx < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; idx++)
	{
		if(pPostPackage->ItemBox[idx] < 0) continue;
		GTH_AddMyInventory_ForRegsitedItem(pc,pPostPackage->ItemBox[idx]);
	}
	
	
	GTH_SendMessage_MailBox_UndoInvenItem(pc, 1, -1);
}















void CPostFunc::GTH_SendMessage_MailBox_UndoInvenItem(playerCharacter_t* pc, int reply, int errorIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_MAILBOX);
		MSG_WriteByte(&netMessage, MAILBOX_UNDOINVENITEM);
		
		MSG_WriteLong(&netMessage, reply);
		MSG_WriteByte(&netMessage, errorIdx);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void CPostFunc::GTH_ProcessMessage_MailBox_Open()
{

	int	PostIdx = -1;
	int PostType= -1;

	PostType	= MSG_ReadLong();
	PostIdx		= MSG_ReadLong();	

	g_curPC->busyState = BUSY_STATE_NONE;
	if(g_curPC->m_PostMng.GetPostState() == CPostManager::POSTSYSTEM_READMODE)
	{		
		
		g_curPC->m_PostMng.SetOpenPost(PostIdx);
		if(NULL == g_curPC->m_PostMng.GetOpenPost()){
			GTH_SendMessage_MailBox_Open(g_curPC, 1, -1, PostIdx);
			return;
		}
		
		g_DBGameServer->OpenPost(g_curPC, PostType, g_curPC->m_PostMng.GetOpenPost()->iPostIdx);		
		return;
	}
	
	GTH_SendMessage_MailBox_Open(g_curPC, 1, -1, PostIdx);

}






void CPostFunc::GTH_SendMessage_MailBox_Open(playerCharacter_t* pc, int reply, int error, int PostIdx)
{
	CPostManager::PostPackage_t *pPost = NULL;
	pPost = pc->m_PostMng.GetPostPackage(PostIdx);	
	if(NULL == pPost){
		reply = 0;
		error = 1;		
	}


	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_MAILBOX);
		MSG_WriteByte(&netMessage, MAILBOX_OPEN);		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{	
			MSG_WriteLong(&netMessage, PostIdx);			
			MSG_WriteLong(&netMessage, pPost->ItemNumber);			
			
			for(int i = 0; i < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; i++)
				MSG_WriteItem(&netMessage, &pPost->Items[i]);			
		}else{
			MSG_WriteByte(&netMessage, error);
		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}









void CPostFunc::GTH_ProcessMessage_MailBox_Item()
{
	int pos			= MSG_ReadByte();	
	if(g_curPC->m_PostMng.GetPostState() == CPostManager::POSTSYSTEM_READMODE )	{		
		GTH_SendMessage_MailBox_Item(g_curPC,NULL, -1, 1, TRUE, pos, 0);
		return;
	}

	CPostManager::PostPackage_t* pPostPack = NULL;
	pPostPack = g_curPC->m_PostMng.GetOpenPost();	
	if(NULL == pPostPack){
		GTH_SendMessage_MailBox_Item(g_curPC,NULL, -1, 1, TRUE, pos, 0);
		return;
	}



	if( g_curPC->mouseInventory >= 0 )
	{		
		if(pPostPack->ItemBox[pos] >= 0)
		{
			int tempInven = g_curPC->mouseInventory;
			g_curPC->mouseInventory = pPostPack->ItemBox[pos];
			pPostPack->ItemBox[pos] = tempInven;
		}
		else
		{
			pPostPack->ItemBox[pos] = g_curPC->mouseInventory;
			g_curPC->mouseInventory = -1;			
		}
	}
	else
	{		
		g_curPC->mouseInventory = pPostPack->ItemBox[pos];
		pPostPack->ItemBox[pos]=-1;
	}

	int itemIdx = pPostPack->ItemBox[pos];
	int itemTableIdx = g_curPC->item[itemIdx].itemTableIdx;
	GTH_SendMessage_MailBox_Item( g_curPC,pPostPack, 1, -1, FALSE, pos, itemTableIdx);
}








void CPostFunc::
GTH_SendMessage_MailBox_Item( 
	playerCharacter_t* pc, 
	const CPostManager::PostPackage_t* pPostPack,
	int reply, 
	int error, 
	int isOwner, 
	int pos, 
	int itemTableIdx )
{
	if((reply > 0) && (NULL == pPostPack)){
		reply=-1;
		error=1;
	}

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_MAILBOX);
		MSG_WriteByte(&netMessage, MAILBOX_ITEM);
		
		MSG_WriteLong(&netMessage, reply);
	
		if((reply > 0) && (NULL != pPostPack))
		{
			MSG_WriteByte(&netMessage, isOwner);
			MSG_WriteByte(&netMessage, pos);
			MSG_WriteShort(&netMessage, pc->mouseInventory );
			MSG_WriteShort( &netMessage, pPostPack->ItemBox[pos] );
			MSG_WriteShort(&netMessage, itemTableIdx);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

}






void CPostFunc::GTH_ProcessMessage_MailBox_Exit()
{		
	
	
	
	GTH_SendMessage_MailBox_Exit(g_curPC);
	g_DBGameServer->LoadPackageList(g_curPC);
}






void CPostFunc::GTH_SendMessage_MailBox_Exit(playerCharacter_t* pc)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_MAILBOX);
		MSG_WriteByte(&netMessage, MAILBOX_EXIT);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}










void CPostFunc::GTH_SendMessage_MailBox_ReceiveItems(playerCharacter_t* pc,  int reply, int errorIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_MAILBOX);
		MSG_WriteByte(&netMessage, MAILBOX_RECEIVE_ITEMS);
		MSG_WriteLong(&netMessage, reply);

		if ( reply == 0)
			MSG_WriteLong(&netMessage, errorIdx);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}









void CPostFunc::GTH_ProcessMessage_PostWinExit()
{

	g_curPC->busyState = BUSY_STATE_NONE;

	
	g_curPC->m_PostMng.SetPostSystem(CPostManager::enumPostSystemMode::POSTSYSTEM_NONE);
	NPC_DeleteNoticePC(&g_monster[g_curPC->npcIdx], g_curPC);
}


void CPostFunc::GTH_ProcessMessage_PostDelete()
{
	int MaxCount = 0;	
	int PostIdx = -1;
	int PostCount = 0;


	
	std::vector<int> vPostType;
	std::vector<int> vPostIdx;
	

	MaxCount = MSG_ReadLong();	
	int HavePostCount = g_curPC->m_PostMng.GetPostCount();
	if ( (MaxCount <= 0) || (MaxCount > HavePostCount)){
		GTH_SendMessage_PostDelete(g_curPC, 0,1);
		return;
	}

	
	
	
	
	for (PostCount = 0; PostCount < MaxCount; PostCount++){
		PostIdx = MSG_ReadLong();
		
		if((PostIdx < 0) || (PostIdx > HavePostCount)){
			GTH_SendMessage_PostDelete(g_curPC, 0,1);
			return;		
		}

		CPostManager::PostPackage_t* pPostPack = NULL;	
		pPostPack = g_curPC->m_PostMng.GetPostPackage(PostIdx);
		if(NULL == pPostPack){
			GTH_SendMessage_PostDelete(g_curPC, 0,1);
			return;
		}	

	

		vPostType.push_back((int)pPostPack->PostSendType);
		vPostIdx.push_back(pPostPack->iPostIdx);
	}
	
	for (PostCount = 0; PostCount < MaxCount; PostCount++){				
		g_DBGameServer->DeletePost(g_curPC,vPostType[PostCount],vPostIdx[PostCount]);
	}

	GTH_SendMessage_PostDelete(g_curPC, 1,-1);

}

void CPostFunc::GTH_SendMessage_PostDelete(playerCharacter_t* pc, int reply, int error)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_POSTSYSTEM);
		MSG_WriteByte(&netMessage, POSTSYSTEM_DELETE);
		MSG_WriteLong(&netMessage, reply);

		if ( reply == 0)
			MSG_WriteLong(&netMessage, error);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CPostFunc::GTH_ProcessMessage_PostState()
{
	int reply = 1;
	int	error = -1;
	int State = 0;
	
	CPostManager::enumPostSystemMode inState;
	
	State = MSG_ReadByte();
	
	if(State == 0)
		inState = CPostManager::POSTSYSTEM_WRITEMODE;
	else
		inState = CPostManager::POSTSYSTEM_READMODE;
	
	g_curPC->m_PostMng.SetPostSystem(inState);
	
	GTH_SendMessage_PostState(g_curPC, reply, error);
}

void CPostFunc::GTH_SendMessage_PostState(playerCharacter_t* pc, int reply, int error)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_POSTSYSTEM);
		MSG_WriteByte(&netMessage, POSTSYSTEM_STATE);
		MSG_WriteLong(&netMessage, reply);
		
		if ( reply == 0)
			MSG_WriteLong(&netMessage, error);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CPostFunc::
GTH_SendMessage_MailBox_Cnt(playerCharacter_t* pc,const int iUnReadedNum,const int iReadedNum)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND );
		MSG_WriteByte(&netMessage, GSC_POSTSYSTEM );
		MSG_WriteByte(&netMessage, POSTSYSTEM_NOTIFY );
		MSG_WriteLong( &netMessage, iUnReadedNum);
		MSG_WriteLong( &netMessage, iReadedNum);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}






class CTempPlayerInventory{
	private:
		struct tagTempInventory{
			int TableIdx;
			int numberIdx;
		};
		struct tagTempInventory m_TempInventory[MAX_INVENTORY_SIZE];
	public:
		BOOL Insert(const item_t* pItem);

	public:
		CTempPlayerInventory(const playerCharacter_t* pPlayer);
		~CTempPlayerInventory();
};



CTempPlayerInventory::CTempPlayerInventory(const playerCharacter_t* pPlayer)
{
	int ItemArrayidx=-1;	
	int InventoryIdx=-1;		
	item_t* pItem=NULL;
	itemTable_t* pItemTable=NULL;

	for(InventoryIdx=0; InventoryIdx < MAX_INVENTORY_SIZE; InventoryIdx++){
		m_TempInventory[InventoryIdx].TableIdx=-1;
		m_TempInventory[InventoryIdx].numberIdx=-1;

		pItem = gcpTools->GetMyItem((playerCharacter_t*)pPlayer,pPlayer->inventory[InventoryIdx]);
		if(NULL == pItem) continue;	
		pItemTable = gcpTools->GetItemTablePointer(pItem->itemTableIdx);
		if(NULL == pItemTable) continue;

		m_TempInventory[InventoryIdx].TableIdx = pItem->itemTableIdx;
		if(pItemTable->stackFlag) m_TempInventory[InventoryIdx].numberIdx= pItem->durability;
		else m_TempInventory[InventoryIdx].numberIdx=-1;			
	}
}

CTempPlayerInventory::~CTempPlayerInventory()
{

}


BOOL CTempPlayerInventory::Insert(const item_t* pItem)
{	
	itemTable_t* pItemTable=NULL;
	pItemTable = gcpTools->GetItemTablePointer(pItem->itemTableIdx);
	if(NULL == pItemTable){
		
		
		return TRUE;
	}

	int InventoryIdx=-1;		
	if(pItemTable->stackFlag){
		for(InventoryIdx=0; InventoryIdx < MAX_INVENTORY_SIZE; InventoryIdx++){			
			if(m_TempInventory[InventoryIdx].TableIdx != pItem->itemTableIdx) continue;

			
			int numberIdx = m_TempInventory[InventoryIdx].numberIdx + (pItem->durability + 1);
			if(numberIdx >= MAX_COUNT_STACK_ITEM) return FALSE;
			m_TempInventory[InventoryIdx].numberIdx = numberIdx;
			return TRUE;
		}
	}

	for(InventoryIdx=0; InventoryIdx < MAX_INVENTORY_SIZE; InventoryIdx++){			
		if(m_TempInventory[InventoryIdx].TableIdx >= 0) continue;

		m_TempInventory[InventoryIdx].TableIdx = pItem->itemTableIdx;
		m_TempInventory[InventoryIdx].numberIdx = pItem->durability;
		return TRUE;
	}

	return FALSE;
}




BOOL CPostFunc::InsertCheck_atINVENTORY(
	const playerCharacter_t*           pPlayer,
	const CPostManager::PostPackage_t* pPostPack)
{
	class CTempPlayerInventory TempInventory(pPlayer);
	const item_t*      pPostItem=NULL;	
	for(int PostPackIdx=0; PostPackIdx < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; PostPackIdx++){
		pPostItem=&pPostPack->Items[PostPackIdx]; 
		if(NULL == pPostItem) continue;
		if(FALSE == TempInventory.Insert(pPostItem)) return FALSE;
	}
	return TRUE;

}






void CPostFunc::GTH_ProcessMessage_MailBox_ReceiveItems(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer) return;
	int PostPackageIdx=-1;	

	PostPackageIdx = MSG_ReadShort();			

	CPostManager::PostPackage_t* pPostPack = NULL;	
	pPostPack = pPlayer->m_PostMng.GetPostPackage(PostPackageIdx);	
	if(NULL == pPostPack){ 		
		GTH_SendMessage_MailBox_ReceiveItems(pPlayer,0,1);
		return;
	}

	
	if( pPlayer->m_PostMng.GetPostState() != CPostManager::POSTSYSTEM_READMODE ){		
		GTH_SendMessage_MailBox_ReceiveItems(pPlayer,0,2);
		return;	
	}

	
	if ( (pPostPack->Nak <= 0) && (pPostPack->ItemNumber == 0)){
		GTH_SendMessage_MailBox_ReceiveItems(pPlayer,0,4);
		return;		
	}

	
	if(FALSE == InsertCheck_atINVENTORY(pPlayer,pPostPack)){
		GTH_SendMessage_MailBox_ReceiveItems(pPlayer,0,3);
		return;
	}

	
	int Nak			=	pPlayer->curChargeSE + pPostPack->Nak;		
	if( Nak > MAX_OWN_NAK ){
		GTH_SendMessage_MailBox_ReceiveItems(pPlayer,0,3);
		return;		
	}			

	
	
	g_DBGameServer->DeletePostItems(pPlayer,(int)pPostPack->PostSendType, pPostPack->iPostIdx);			

	
	

}














void CPostFunc::
GTH_ProcessMessage_MailBox_AddItemToPlayer(
	playerCharacter_t* pPlayer,
	const int DeletePostPackageIdx,
	const BOOL bDelete)
{
	CPostManager::PostPackage_t* pPostPack = NULL;
	pPostPack = pPlayer->m_PostMng.GetOpenPost();
	if((NULL == pPostPack) || (pPostPack->iPostIdx != DeletePostPackageIdx)){
		g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;%d;;;",LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR,2,0,LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR_UNKNOWN_PACKAGE);
		GTH_SendMessage_MailBox_ReceiveItems(pPlayer,0,5);
		return;
	}
	
	if(FALSE == bDelete){
		g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;%d;;;",LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR,2,0,LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR_NOT_DELETE_POST);
		GTH_SendMessage_MailBox_ReceiveItems(pPlayer,0,5);
		return;
	}



	
	
	
	
	
	
	
	
	
	int PostItemIdx=-1;
	item_t* pPostItem=NULL;
	itemTable_t* pPostItemTable=NULL;


	
	
	for(PostItemIdx = 0; PostItemIdx < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; PostItemIdx ++)
	{		
		pPostItem=&pPostPack->Items[PostItemIdx];		
		if(pPostItem->itemTableIdx < 0 ) continue;

		pPostItemTable = gcpTools->GetItemTablePointer(pPostItem->itemTableIdx);
		if(NULL == pPostItemTable) continue;


		if(TRUE == gcpTools->IsStackItem(pPostItem)){
			int ItemArrayIdx = GTH_StackMyItem(pPlayer,pPostItem);
			if( ItemArrayIdx >= 0 ){

				item_t* pStackItem= gcpTools->GetMyItem(pPlayer,ItemArrayIdx);
				if(NULL != pStackItem){

					int numberIdx = pStackItem->durability + (pPostItem->durability + 1);
					if(numberIdx >= MAX_COUNT_STACK_ITEM){
						g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;%d;;%s;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR,1,pPostItem->durability,LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR_NOT_INSERT_INVENTORY,pPostItem->name,pPostPack->szFromName);
						continue;
					}					
					pStackItem->durability = numberIdx;					
					g_DBGameServer->SaveItem(pPlayer, pStackItem, ItemArrayIdx );

					if(pPostPack->PostSendType == CPostManager::enumPostSendType::POST_SENDTYPE_WEB){
						g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;;;%s;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_WEB,1,pPostItem->durability,pPostItem->name, "FROM_WEB");
					}else{
						g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;;;%s;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_CHA,1,pPostItem->durability,pPostItem->name, pPostPack->szFromName);
					}
					continue;
				}
			}
		}


		int inventoryPos = -1;		
		
		inventoryPos = GTH_FindEmptyInventoryPos(pPlayer);		
		if(inventoryPos < 0){
			g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;%d;;%s;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR,1,pPostItem->durability,LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR_NOT_INSERT_INVENTORY,pPostItem->name,pPostPack->szFromName);
			continue;
		}

		int itemArrayIdx = -1;
		
		itemArrayIdx = GTH_AddMyItem( pPlayer,pPostItem);
		if(itemArrayIdx < 0){
			g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;%d;;%s;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR,1,pPostItem->durability,LOG_CLASS_POSTSYSTEM_RECEIVE_ERROR_NOT_INSERT_INVENTORY,pPostItem->name,pPostPack->szFromName);
			continue;
		}
		
		
		pPlayer->inventory[inventoryPos] = itemArrayIdx;		
		
		
		g_DBGameServer->AddItem(pPlayer, pPostItem, itemArrayIdx,"PostSystem");
		if(pPostPack->PostSendType == CPostManager::enumPostSendType::POST_SENDTYPE_WEB){
			g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;;;%s;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_WEB,1,pPostItem->durability,pPostItem->name, "FROM_WEB");
		}else{
			g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;;;%s;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_CHA,1,pPostItem->durability,pPostItem->name, pPostPack->szFromName);
		}
	}		

	
	for(PostItemIdx = 0; PostItemIdx < CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; PostItemIdx ++){		
		InitItem(&pPostPack->Items[PostItemIdx]);		
		pPostPack->ItemNumber --;					
	}
	
	if(pPostPack->Nak > 0){
		
		if(pPostPack->PostSendType == CPostManager::enumPostSendType::POST_SENDTYPE_WEB){
			g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;;;;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_WEB,0,pPostPack->Nak, "FROM_WEB");
		}else{
			g_logSystem->WriteToLog(pPlayer,LOG_CLASS_POSTSYSTEM,"%d;%d;%d;;;;%s",LOG_CLASS_POSTSYSTEM_RECEIVE_CHA,0,pPostPack->Nak, pPostPack->szFromName);			
		}

		pPlayer->curChargeSE += pPostPack->Nak;
		pPostPack->Nak = 0;	
		g_DBGameServer->SaveChaPackageNak(pPlayer, pPostPack);
	}

	
	GTH_SendMessage_MailBox_ReceiveItems(pPlayer,1,0);
}




