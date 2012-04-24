




#include "../global_def.h"






void GTH_WorldChangeSystem_Process()
{
	int command = MSG_ReadByte();	

	switch(command) 
	{
	case GTH_REPLY_USE_ITEM:
		GTH_ProcessMessage_Reply_WorldChangeUseItem();
		break;

	case GTH_REPLY_WORLD_CHANGE:
		GTH_ProcessMessage_Reply_WorldChange();
		break;	

		
	case GTH_CLEAR_WORLD_CHANGE_MODE:
		GTH_ProcessMessage_ClearWorldChageMode();
		break;		
	}

}






void GTH_SendMessage_Request_WorldChangeUseItem(WorldChangeSystem *pWorldChangeSystem)
{
	MSG_BeginWriting( &netMessage);	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte	( &netMessage, CC_EXTEND);
		MSG_WriteByte	( &netMessage, CC_WORLD_CHANGE);	
		MSG_WriteByte	( &netMessage, GTH_REQUEST_USE_ITEM);			
		MSG_WriteByte	( &netMessage, pWorldChangeSystem->m_SaveInvenPos);
		NET_SendMessage	( &gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}







void GTH_ProcessMessage_Reply_WorldChangeUseItem()
{
	int reply = 0;
	int erroridx = 0;

	reply = MSG_ReadByte();

	if ( reply == 0 )
	{
		erroridx = MSG_ReadLong();
		switch(erroridx) {
		case 1:
			

			break;
		case 2:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 283) );
			break;	


		}
		
		
	}	
	else
	{
		
		g_ifMng->m_WorldChangeWin->Enable();
		g_ifMng->SetFocus( g_ifMng->m_WorldChangeWin);
	}
}








void GTH_SendMessage_Request_WorldChange(WorldChangeSystem *pWorldChangeSystem)
{
	MSG_BeginWriting( &netMessage);	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte	( &netMessage, CC_EXTEND);
		MSG_WriteByte	( &netMessage, CC_WORLD_CHANGE);
		MSG_WriteByte	( &netMessage, GTH_REQUEST_WORLD_CHANGE);
		MSG_WriteLong	( &netMessage, pWorldChangeSystem->m_SaveInvenPos);				
		MSG_WriteLong	( &netMessage, pWorldChangeSystem->m_SelectWorldIdx);
		NET_SendMessage	( &gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

}






void GTH_ProcessMessage_Reply_WorldChange()
{
	int  reply			= 0;	
	int  saveInvenPos	= 0;
	int  errorIdx		= 0;
	char strbuffer[MAX_PATH];

	reply = MSG_ReadByte();

	if ( reply == 0 )
	{
		errorIdx = MSG_ReadByte();

		
		switch(errorIdx) 
		{
		case 0:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 284) );
			break;

		case 1:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 285) );				
			break;

		case 2:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 322),
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 286) );
			break;		

			
		case 3:
			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,588), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_NONE);			
			break;		

		}
		
	}
	else
	{			
		saveInvenPos = MSG_ReadLong();				
		int itemIdx	 = g_cgv.myCharacterInfo->inventory[saveInvenPos];			
		item_t *pItem = &g_cgv.myCharacterInfo->item[itemIdx];

		
		if ( g_itemTable[pItem->itemTableIdx].stackFlag == 1)
			GTH_UseItem_Delete(saveInvenPos);
		
		
		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
			WorldChangeSystem::WORLD_CHANGE_EFFECT_IDX, 								
								g_pApp->m_myCharacter->position,
								NULL, 
								true, 
								g_pApp->m_myCharacter->entityType, 
								g_pApp->m_myCharacter->idxOnServer,
								0.0f, 
								0.0f, 
								0.0f, 
								-1.0f,
								false );


	

		g_ifMng->m_WorldChangeWin->Enable(false);		
		g_ifMng->m_itemWin->Enable(false);
		
		int SelectWorldIdx = g_cgv.myCharacterInfo->m_WorldChageSystem.m_SelectWorldIdx;
		_snprintf(strbuffer, MAX_PATH, "%s %s", g_cgv.worldTable[SelectWorldIdx].hanName,  
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 287));
		strbuffer[MAX_PATH-1] = NULL;

		g_ifMng->AddSysMessage( strbuffer, D3DCOLOR_ARGB(255, 255, 125, 100) );	

		SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_EFFECT_MODE);

	}
}



BOOL IsAllowWorldChange_ForPcEvent()
{
	
	if ( g_pApp->m_myCharacter->isAlive = 0)
		return 2;

	switch(g_pApp->m_myCharacter->event)
	{
	case GTH_EV_CHAR_ATTACK:
	case GTH_EV_CHAR_SKILL:
	case GTH_EV_CHAR_SKILLCASTING:
	case GTH_EV_CHAR_BEFORE_TELEPORT:
	case GTH_EV_CHAR_TELEPORT:
	case GTH_EV_CHAR_FREEZE:
	case GTH_EV_CHAR_PUSHED:
	case GTH_EV_CHAR_HIDE:
		return 3;
	}

	return 1;
}


void SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode mode)
{	
	static int setTime=0;

	
	if(mode ==  WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_CHECK)
	{
		if ( g_cgv.myCharacterInfo->m_WorldChageSystem.m_SystemMode == WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE) return;

		
		if( timeGetTime() - setTime > 20000)
		{
			g_cgv.myCharacterInfo->m_WorldChageSystem.m_SystemMode = WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE;	

			g_input->MouseEnable(true);
			g_input->KeyEnable(true);
		}

		return;
	}

	g_cgv.myCharacterInfo->m_WorldChageSystem.m_SystemMode = mode;	

	if ( mode == WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE)
	{		
		g_input->MouseEnable(true);
		g_input->KeyEnable(true);
	}

	else
	{
		setTime = timeGetTime();

		
		g_input->MouseEnable(false);
		g_input->KeyEnable(false);
	}

}


void GTH_ProcessMessage_ClearWorldChageMode()
{
	g_cgv.myCharacterInfo->m_WorldChageSystem.m_SystemMode = 
		WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE;

	

	g_input->MouseEnable(true);
	g_input->KeyEnable(true);

}