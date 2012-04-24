




#include "../Global.h"

#include "../Tools/CTools.h"
extern CTools* gcpTools;









void GTH_WorldChangeSystem_Process()
{
	int command = MSG_ReadByte();	

	switch(command) 
	{
	case GTH_REQUEST_USE_ITEM:
		GTH_ProcessMessage_Request_WorldChangeUseItem(g_curPC);
		break;

	case GTH_REQUEST_WORLD_CHANGE:
		GTH_ProcessMessage_ReQuest_WorldChange();
		break;	
	}

}






void GTH_SendMessage_Reply_WorldChangeUseItem(playerCharacter_t* pPc, int reply, int erroridx, int InvenPos)
{
	MSG_BeginWriting( &netMessage);	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte	( &netMessage, GSC_EXTEND);
		MSG_WriteByte	( &netMessage, GSC_WORLD_CHANGE);
		MSG_WriteByte	( &netMessage, GTH_REPLY_USE_ITEM);
		MSG_WriteByte	( &netMessage, reply);

		if ( reply == 0)
			MSG_WriteLong( &netMessage, erroridx);

		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}










void GTH_ProcessMessage_ReQuest_WorldChange()
{

	int  reply			= 0;
	int  erroridx		= 0;

	int	 worldIdx		= 0;
	int  saveInvenPos	= 0;

	saveInvenPos	= MSG_ReadLong();		
	worldIdx		= MSG_ReadLong();

	
	if ( worldIdx == g_curPC->worldIdx)	
	{
		GTH_SendMessage_Reply_WorldChange(g_curPC, 0, 0, saveInvenPos);
		return;
	}

	
	if ( GTH_IsLocalServiceWorld(worldIdx) == false )
	{
		int memberIdx = GTH_FindServerWithinGroup( worldIdx );
		if (memberIdx < 0) 		
		{
			
			GTH_SendMessage_Reply_WorldChange(g_curPC, 0, 2, saveInvenPos);		
			return;
		}

	}


	
	if ( g_curPC->busyState != BUSY_STATE_NONE)
	{
		GTH_SendMessage_Reply_WorldChange(g_curPC, 0, 3, saveInvenPos);
		return;
	}

	if((saveInvenPos < 0) || (saveInvenPos >= MAX_INVENTORY_SIZE)){
		GTH_SendMessage_Reply_WorldChange(g_curPC, 0, 1, saveInvenPos);
		return;
	}

	
	int	itemArrayIdx = g_curPC->inventory[saveInvenPos];
	if(itemArrayIdx < 0) 	
	{		
		GTH_SendMessage_Reply_WorldChange(g_curPC, 0, 1, saveInvenPos);
		return;
	}
	
	
	item_t* pItem = gcpTools->GetMyItem(g_curPC, itemArrayIdx);
	if(NULL == pItem){
		GTH_SendMessage_Reply_WorldChange(g_curPC, 0, 1, saveInvenPos);
		return;
	}

	

	if(TRUE == gcpTools->IsStackItem(pItem)){
	
	

		item_t DeleteItem = *pItem;
		
		if(FALSE == GTH_UseItem_Delete(saveInvenPos)){
			GTH_SendMessage_Reply_WorldChange(g_curPC, 0, 1, saveInvenPos);
			return;
		}
		g_logSystem->WriteToLog(g_curPC,LOG_CLASS_ITEM,"%d;%d;%d;%d;%d;%s;",LOG_CLASS_ITEM_USE,DeleteItem.itemTableIdx,worldIdx,g_curPC->worldIdx,DeleteItem.durability-1,DeleteItem.name);
	}

	
	g_curPC->m_WorldChangeSystem.m_SystemMode = 
		WorldChangeSystem::enumWorldChangeMode::WORLD_CHANGESYSTEM_EFFECT_MODE;
		
	
	
	
	g_curPC->busyState = BUSY_STATE_WORLDCHAGNE;
	
	
	GTH_SendMessage_CompleteEffect_To_AllPC(g_curPC, WorldChangeSystem::WORLD_CHANGE_EFFECT_IDX);
	
	GTH_SendMessage_Reply_WorldChange(g_curPC, 1, 0, saveInvenPos);

	
	g_curPC->fWorldChangeStartTime = g_timer->GetAppMilliTime();
	g_curPC->iChangeWorldIdx = worldIdx;

	
}







void GTH_SendMessage_Reply_WorldChange(playerCharacter_t* pPc, int reply, int erroridx, int InvenPos)
{
	MSG_BeginWriting( &netMessage);	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte	( &netMessage, GSC_EXTEND);
		MSG_WriteByte	( &netMessage, GSC_WORLD_CHANGE);
		MSG_WriteByte	( &netMessage, GTH_REPLY_WORLD_CHANGE);						
		MSG_WriteByte	( &netMessage, reply);

		if ( reply == 0)
			MSG_WriteLong( &netMessage, erroridx);
		else
			MSG_WriteLong( &netMessage, InvenPos);

		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );

}








void GTH_ProcessMessage_Request_WorldChangeUseItem(playerCharacter_t* in_pPlayer)
{

	if(NULL == in_pPlayer) return;
	int nInvenPosIdx = MSG_ReadByte();
	if((nInvenPosIdx < 0) || (nInvenPosIdx >= MAX_INVENTORY_SIZE)){		
		GTH_SendMessage_Reply_WorldChangeUseItem(in_pPlayer, 0, 1 , nInvenPosIdx);
		return;
	}


	item_t*	pItemRecord = gcpTools->GetMyItem(in_pPlayer,in_pPlayer->inventory[nInvenPosIdx]);
	if(NULL == pItemRecord){		
		
		GTH_SendMessage_Reply_WorldChangeUseItem(in_pPlayer, 0, 1 , nInvenPosIdx);
		return;
	}


	

	
	if (in_pPlayer->event == GTH_EV_CHAR_IDLE) 	
		GTH_SendMessage_Reply_WorldChangeUseItem(in_pPlayer, 1, 0, nInvenPosIdx);
	

}



void GTH_SendMessage_ClearWorldChageMode(playerCharacter_t* pPc)
{
	MSG_BeginWriting( &netMessage);	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte	( &netMessage, GSC_EXTEND);
		MSG_WriteByte	( &netMessage, GSC_WORLD_CHANGE);
		MSG_WriteByte	( &netMessage, GTH_CLEAR_WORLD_CHANGE_MODE);		
		NET_SendMessage(&pPc->sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}
