



#include "../global_def.h"








void GTH_DecodeDanBattleSystem()
{
	enumNewDanBattlePacket Packet = static_cast<enum enumNewDanBattlePacket>(MSG_ReadShort());	
	switch(Packet)
	{		
		
	case CC_ACTIVE_SYSTEM:				
		GTH_ProcessMessage_DanBattleActiveSystem();
		break;

		
	case GSC_DAN_BATTLE_WIN_OPEN:
		GTH_ProcessMessage_NewDanBattleWinOpen();
		break;
 
		
	case GSC_REQUEST_DANBATTLE:		
		GTH_ProcessMessage_RequestDanBattle();
		break;

		
	case GSC_SUGGEST_DANBATTLE:
		GTH_ProcessMessage_SuggestDanBattle();
		break;

		
	case GSC_ACCEPT_DANBATTLE:
		GTH_ProcessMessage_AcceptDanBattle();
		break;		

		
	case GSC_DAN_BATTLE_READY:
		GTH_ProcessMessage_NewDanBattleReady();
		break;
		
		
	case GSC_DAN_BATTLE_START:
		GTH_ProcessMessage_NewDanBattleStart();
		break;

		
	case GSC_DAN_BATTLE_END:
		GTH_ProcessMessage_NewDanBattleEnd();
		break;

		
	case GSC_DAN_BATTLE_RESULT:
		GTH_ProcessMessage_DanBattleResult();
		break;

		
	case GSC_DAN_BATTLE_PLUNDER:
		GTH_ProcessMessage_NewDanBattlePlunder();
		break;

		
	case GSC_DAN_BATTLE_INFO:
		GTH_ProcessMessage_DanBattleInfo();
		break;

	case GSC_DAN_BATTLE_NOTICE_MSG:
		GTH_ProcessMessage_DanBattleNoticeMsg();
		break;

	}
}






void GTH_ProcessMessage_DanBattleActiveSystem()
{

}






void GTH_ProcessMessage_RequestDanBattle()
{
	BOOL Reply = FALSE;
	tagGCPacket_NewDanBattle::enumCode code = tagGCPacket_NewDanBattle::enumCode::fail;
	code = (tagGCPacket_NewDanBattle::enumCode)MSG_ReadLong();

	if ( code == tagGCPacket_NewDanBattle::enumCode::success)
	{
		int Result = MSG_ReadByte();
		if ( Result == 0)
		{		




			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 652), 
				MSG_BUTTONTYPE_NONE, IF_MESSAGE_REQUEST_NEW_DANBATTLE, 1000.0f );

		}
		else		
			g_ifMng->ExitMessage();		
		return;
	}

	
	tagGCPacket_NewDanBattle::enumError erroridx = tagGCPacket_NewDanBattle::enumError::none;	
	erroridx = (tagGCPacket_NewDanBattle::enumError)MSG_ReadByte();
	switch(erroridx) 
	{
	case tagGCPacket_NewDanBattle::enumError::not_join_guild:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 599));
		break;
		
	case tagGCPacket_NewDanBattle::enumError::not_capacity:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 600));
		break;		
		
	case tagGCPacket_NewDanBattle::enumError::not_enough_my_member:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 601));
		break;			
		
	case tagGCPacket_NewDanBattle::enumError::nonexstent_guild:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 602));
		break;	

	case tagGCPacket_NewDanBattle::enumError::myself_guild:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 603));
		break;			

	case tagGCPacket_NewDanBattle::enumError::not_enough_target_member:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 604));
		break;

	case tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 605));
		break;

	case tagGCPacket_NewDanBattle::enumError::battleInsert_Error:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 606));
		break;		

	case tagGCPacket_NewDanBattle::enumError::not_Accept_DanBattle:
		{
			g_ifMng->ExitMessage();
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 607));
		}
		break;
		
	case tagGCPacket_NewDanBattle::enumError::Error_Full_danbattleCount:
		{
			g_ifMng->ExitMessage();			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 665));
		}break;
	}	
}






void GTH_ProcessMessage_AcceptDanBattle()
{	
	
	
	g_NewDanBattleSystem.InitDanBattleInfo();
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo();

	if ( pDanBattleInfo == NULL)
		return;

	
	pDanBattleInfo->BattleMode = (NewDanBattleSystem::DanBattleInfo::enumBattleMode)MSG_ReadByte();
	
	
	pDanBattleInfo->fRemainSec	 = MSG_ReadFloat();	

	
	pDanBattleInfo->nBattleMapIdx = MSG_ReadByte();		

	
	pDanBattleInfo->fWaitTImeForReady = MSG_ReadFloat();
	pDanBattleInfo->fWaitTImeForEnd = MSG_ReadFloat();



	
	pDanBattleInfo->RequestDanInfo.nDanIdx = MSG_ReadLong();
	pDanBattleInfo->RequestDanInfo.nRemainPoint = MSG_ReadByte();




	
	pDanBattleInfo->AcceptDanInfo.nDanIdx = MSG_ReadLong();
	pDanBattleInfo->AcceptDanInfo.nRemainPoint = MSG_ReadByte();




	pDanBattleInfo->fWaitTImeForReady = MSG_ReadFloat();

	pDanBattleInfo->nDanBattleIdx	= MSG_ReadLong();

	g_NewDanBattleSystem.SetDanBattleStatus(
		NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_READY);	

	g_NewDanBattleSystem.m_EffectFlag = 0x00;

	g_musicMng->ChangeBGMusic( "../BGMusic/danbattle.mp3" );
}







void GTH_SendMessage_DanBattleInfo()
{

}







void GTH_ProcessMessage_DanBattleResult()
{
	char mSG[256];
	char tempString[MAX_PATH];
	char EnemyGuildName[GUILDNAMESTRING];
	char NOTICE_String[MAX_PATH];
	char* LoseGuildName;

	
	CIFDanWarRequestWin* RequestWIn = g_ifMng->m_DanWarRequestWin;
	tagGCPacket_NewDanBattle::enumwincode code = tagGCPacket_NewDanBattle::enumwincode::loss;
	code = (tagGCPacket_NewDanBattle::enumwincode)MSG_ReadByte();

	
	NewDanBattleSystem::DanBattleInfo::enumEndType type= (NewDanBattleSystem::DanBattleInfo::enumEndType)MSG_ReadLong();
	

	
	_snprintf(EnemyGuildName, GUILDNAMESTRING, MSG_ReadString());
	EnemyGuildName[GUILDNAMESTRING-1] = NULL;


	
	switch (code )																			
	{
	case tagGCPacket_NewDanBattle::enumwincode::win:
		{							
		
			sprintf(tempString, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 608));	
		
		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 156, g_pApp->m_myCharacter->position,NULL, 
								true, g_pApp->m_myCharacter->entityType, g_pApp->m_myCharacter->idxOnServer,
								0.0f, 0.0f, 0.0f, -1.0f, false );			
		
		RequestWIn->m_textCtlDanName->SetText(g_cgv.syncCharacter[0].guildName);	
		
		RequestWIn->m_textCtlWorldName->SetText(EnemyGuildName);	
		LoseGuildName = EnemyGuildName;

		}
		break;

	case tagGCPacket_NewDanBattle::enumwincode::loss:
		{	
			
			
			sprintf(tempString, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 609));		



			
			
			
			RequestWIn->m_textCtlWorldName->SetText(g_cgv.syncCharacter[0].guildName);
			LoseGuildName = g_cgv.syncCharacter[0].guildName;

			
			RequestWIn->m_textCtlDanName->SetText(EnemyGuildName);
	
			
			
		}break;


	case tagGCPacket_NewDanBattle::enumwincode::draw:
		{	
			
			
			sprintf(tempString, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 610));		
			g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 156, g_pApp->m_myCharacter->position,NULL, 	
				true, g_pApp->m_myCharacter->entityType, g_pApp->m_myCharacter->idxOnServer,
				0.0f, 0.0f, 0.0f, -1.0f, false );
			
			
			
			RequestWIn->m_textCtlWorldName->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 611));
			
			
			RequestWIn->m_textCtlDanName->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 611));
		}break; 
	}
		
	strncpy(mSG, MSG_ReadString(), 256);
	mSG[256-1] = NULL;

	RequestWIn->m_ComboCtlDanWarType->ClearString();
	RequestWIn->m_ComboCtlDanWarType->AddString(mSG);
	RequestWIn->m_ComboCtlDanWarType->SetCurSel(0);

	int WinCount	= MSG_ReadLong();
	int LoseCount	= MSG_ReadLong();	
	
	
	_snprintf(mSG, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 612), WinCount, LoseCount );
	mSG[256-1] = NULL;

	RequestWIn->m_ComboCtlDanWarTime->ClearString();	
	RequestWIn->m_ComboCtlDanWarTime->AddString(mSG);	
	RequestWIn->m_ComboCtlDanWarTime->SetCurSel(0);	

	int MyPoint = MSG_ReadLong();
	
	_snprintf(mSG, 256,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 613),  MyPoint);
	mSG[256-1] = NULL;
	RequestWIn->m_textCtlDanMemberNum->SetText(mSG);	

	g_ifMng->AddSysMessage( tempString, GTHCOLOR_ITEMNOTICE_MESSAGE);		

	

	
	switch(type)
	{
	case NewDanBattleSystem::DanBattleInfo::enumEndType::REMAIN_MEMBER_COUNT:
		
		_snprintf(NOTICE_String, MAX_PATH,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 614), LoseGuildName);
		break;
	case NewDanBattleSystem::DanBattleInfo::enumEndType::TIME_OUT:
		
		_snprintf(NOTICE_String, MAX_PATH, tempString);
		break;

	case NewDanBattleSystem::DanBattleInfo::enumEndType::SYSTEM:
		
		_snprintf(NOTICE_String, MAX_PATH,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 616));
		break;
	}

	NOTICE_String[MAX_PATH-1] = NULL;
	g_NewDanBattleSystem.AddDanBattleNoticeMsg(NOTICE_String);


	
	g_musicMng->ChangeBGMusic( g_pApp->m_mapInfos[ 0 ].BGMusic );	

	
	
	g_ifMng->m_DanWarBattleWin->Enable(FALSE);

	
	g_ifMng->m_DanWarRequestWin->InitDanWarWin(CIFDanWarRequestWin::DanWarRequestWinType::RESULT,
			&g_NewDanBattleSystem);
	g_ifMng->m_DanWarRequestWin->Enable(TRUE);


	g_ifMng->SetFocus(g_ifMng->m_DanWarRequestWin);

	
	g_NewDanBattleSystem.InitDanBattleInfo();

}






void GTH_SendMessage_DanBattleResult()
{
	

}







void GTH_SendMessage_Request_NewDanBattleWinOpen()
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DAN_BATTLE);
		MSG_WriteShort(&netMessage,	CC_DAN_BATTLE_WIN_OPEN);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
	
}








void GTH_ProcessMessage_NewDanBattleWinOpen()
{
	tagGCPacket_NewDanBattle::enumCode code = tagGCPacket_NewDanBattle::enumCode::fail;
	code = (tagGCPacket_NewDanBattle::enumCode)MSG_ReadLong();	

	if ( code == tagGCPacket_NewDanBattle::enumCode::fail)
	{
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 617), 1, IF_MESSAGE_NONE );
			

		return;
	}
	else
	{
		g_NewDanBattleSystem.InitDanBattleInfo();
		g_NewDanBattleSystem.m_nMinPcCount = MSG_ReadByte();
		g_NewDanBattleSystem.m_nBattleTimeForMin1 = MSG_ReadLong();
		g_NewDanBattleSystem.m_nBattleTimeForMin2 = MSG_ReadLong();	
		NewDanBattleSystem::DanBattleInfo* pBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo();

		if ( pBattleInfo != NULL)
			pBattleInfo->nBattleMapIdx= MSG_ReadByte();

		g_ifMng->m_DanWarRequestWin->InitDanWarWin(CIFDanWarRequestWin::DanWarRequestWinType::REQUEST,
			&g_NewDanBattleSystem);
		

		g_ifMng->m_DanWarRequestWin->Enable(TRUE);
		g_ifMng->SetFocus(g_ifMng->m_DanWarRequestWin);	
	}

}






void GTH_ProcessMessage_NewDanBattleReady()
{
	
	g_NewDanBattleSystem.InitDanBattleInfo();
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo();


	
	int EnemyguildIdx	= MSG_ReadLong();	

	
	_snprintf(pDanBattleInfo->RequestDanInfo.szGuildName, GUILDNAMESTRING,  MSG_ReadString());
	pDanBattleInfo->RequestDanInfo.szGuildName[GUILDNAMESTRING-1] = NULL;

	
	int nMapIdx			= MSG_ReadByte();

	
	int nRemainTime		= MSG_ReadLong();
	
	
	pDanBattleInfo->fWaitTImeForReady	= nRemainTime;
	pDanBattleInfo->nBattleMapIdx		= nMapIdx;	
	pDanBattleInfo->BattleMode = (NewDanBattleSystem::DanBattleInfo::enumBattleMode)MSG_ReadByte();

	
	
	char worldname[WORLDNAMESTRING];
	if ( nMapIdx >= 0 && nMapIdx < MAX_WORLD_NUMBER)		
		_snprintf(worldname,WORLDNAMESTRING,  g_cgv.worldTable[nMapIdx].hanName);
		worldname[WORLDNAMESTRING -1] = NULL;

	
	char szDanBattleMode[50];	
	if ( pDanBattleInfo->BattleMode == NewDanBattleSystem::DanBattleInfo::enumBattleMode::BATTLEMODE_GENERAL)	
	{		
		
		strncpy(szDanBattleMode,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 618), 50);	
	}
	else
		strncpy(szDanBattleMode,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 619), 50);	
		
		

	szDanBattleMode[50-1] = NULL;

	char msg[256];				
	
	_snprintf(msg, 256,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 620),
		worldname, 
		pDanBattleInfo->RequestDanInfo.szGuildName,
		nRemainTime/1000, 
		szDanBattleMode);		
	msg[256-1] = NULL;




	
	g_ifMng->m_GmNotice = FALSE;
	g_ifMng->AddSysMessage(msg, GTHCOLOR_ITEMNOTICE_MESSAGE);
	g_NewDanBattleSystem.AddDanBattleNoticeMsg(msg, GTHCOLOR_ITEMNOTICE_MESSAGE);

	
	

	
	g_NewDanBattleSystem.m_EffectFlag = 0x00;
	g_NewDanBattleSystem.SetDanBattleStatus(
		NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_READY);	

	g_ifMng->m_GmNotice = FALSE;
	
	g_NewDanBattleSystem.AddDanBattleNoticeMsg( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 621));

	
 	g_musicMng->ChangeBGMusic( "../BGMusic/danbattle.mp3" );
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( 43 ), GTH_SOUND_PLAY_ONCE );
	
}






void GTH_ProcessMessage_NewDanBattleStart()
{

	int EnemyguildIdx	= MSG_ReadLong();
	int nMapIdx			= MSG_ReadLong();
	int nRemainTime		= MSG_ReadLong();



	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo();

	if ( pDanBattleInfo != NULL)
		pDanBattleInfo->fRemainSec = nRemainTime;
	
	g_NewDanBattleSystem.m_EffectFlag = 0x00;

	
	g_NewDanBattleSystem.SetDanBattleStatus(
		NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE);
	
	














	g_ifMng->m_GmNotice = FALSE;
	
	g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 622), GTHCOLOR_ITEMNOTICE_MESSAGE);
	g_NewDanBattleSystem.AddDanBattleNoticeMsg(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 622), GTHCOLOR_ITEMNOTICE_MESSAGE);

	g_ifMng->m_DanWarBattleWin->Enable(TRUE);
	g_ifMng->SetFocus(g_ifMng->m_DanWarBattleWin);

}






void GTH_ProcessMessage_NewDanBattleEnd()
{
	int EnemyguildIdx	= MSG_ReadLong();
	int nMapIdx			= MSG_ReadLong();
	int nRemainTime		= MSG_ReadLong();
	
	
	g_NewDanBattleSystem.SetDanBattleStatus(
		NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_END);

	g_NewDanBattleSystem.m_EffectFlag = 0x00;
	g_ifMng->m_GmNotice = FALSE;
	
	g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 623), GTHCOLOR_ITEMNOTICE_MESSAGE);
	
	
	
	
	

	
	g_ifMng->m_DanWarBattleWin->Enable(FALSE);
	
}






void GTH_ProcessMessage_NewDanBattlePlunder()
{
	char Msg[tagGCPacket_NewDanBattle::msg_max_len+1];
	tagGCPacket_NewDanBattle::PlunderPcCode PcCode 
		= (tagGCPacket_NewDanBattle::PlunderPcCode)MSG_ReadByte();	

	tagGCPacket_NewDanBattle::PlunderItemCode ItemCode 
		= (tagGCPacket_NewDanBattle::PlunderItemCode)MSG_ReadByte();

	g_ifMng->m_GmNotice = FALSE;	
	
	if ( PcCode == tagGCPacket_NewDanBattle::PlunderPcCode::Attacker)
	{
		switch(ItemCode)
		{		
		case tagGCPacket_NewDanBattle::PlunderItemCode::itemplunderFail:
			{


				g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 624), GTHCOLOR_ITEMNOTICE_MESSAGE);
			g_NewDanBattleSystem.AddDanBattleNoticeMsg( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 624), GTHCOLOR_ITEMNOTICE_MESSAGE );

			break;
			}

		case tagGCPacket_NewDanBattle::PlunderItemCode::itemplunderSuccecs:			
			{							
				item_t Plunderitem;
				MSG_ReadItem(&Plunderitem);

				int stackItemIdx = GTH_StackMyItem( &Plunderitem );
				
				if( stackItemIdx >= 0 )
				{							
					g_cgv.myCharacterInfo->item[stackItemIdx].durability += Plunderitem.durability + 1;
					if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
						g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;					
				}
				else
				{								
					int itemIdx = GTH_AddMyItem( &Plunderitem );
					int inventoryPos = GTH_FindEmptyInventoryPos();
					
					if ( inventoryPos != -1)
						g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;					
				}

				_snprintf(Msg, 	tagGCPacket_NewDanBattle::msg_max_len, 
					g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 625), Plunderitem.name);
					

				Msg[tagGCPacket_NewDanBattle::msg_max_len] = NULL;

				g_ifMng->m_GmNotice = FALSE;
				g_ifMng->AddSysMessage(Msg, GTHCOLOR_ITEMNOTICE_MESSAGE);
				g_NewDanBattleSystem.AddDanBattleNoticeMsg( Msg, GTHCOLOR_ITEMNOTICE_MESSAGE );
				g_musicMng->PlaySample(g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_SUCCESS ), GTH_SOUND_PLAY_ONCE);
	
			}
			break;

		case tagGCPacket_NewDanBattle::PlunderItemCode::NomoreitemPlunder:			
			
			g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 626), GTHCOLOR_ITEMNOTICE_MESSAGE);
			break;
		}
	}
	
	else
	{
		if ( ItemCode == tagGCPacket_NewDanBattle::PlunderItemCode::itemDeleteSuccecs )
		{				
			int DeleteInvenPos = MSG_ReadLong();
			if ( DeleteInvenPos >= 0 && DeleteInvenPos < MAX_INVENTORY_SIZE)
			{			
				int itemIdx = g_cgv.myCharacterInfo->inventory[DeleteInvenPos];
				if ( itemIdx >= 0 && itemIdx < MAX_NUMBER_OF_OWNITEM )
				{			
					item_t *pItem = NULL;
					pItem = &g_cgv.myCharacterInfo->item[itemIdx];
					
					_snprintf(Msg, 	tagGCPacket_NewDanBattle::msg_max_len, 
						g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 627), pItem->name);
						

					
					Msg[tagGCPacket_NewDanBattle::msg_max_len] = NULL;
				}
				DeleteItemFromMyInventoryByInvenPos(DeleteInvenPos);
			}

			g_ifMng->AddSysMessage(Msg, GTHCOLOR_ITEMNOTICE_MESSAGE);



			g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_FAIL ), GTH_SOUND_PLAY_ONCE );
		}		
	}
}







void GTH_SendMessage_RequestDanBattle()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DAN_BATTLE);
		MSG_WriteShort(&netMessage,	CC_REQUEST_DANBATTLE);
		MSG_WriteString(&netMessage, g_chatManager->GetInputStr( INPUT_INDEX_DAN_NAME ));		
		MSG_WriteByte(&netMessage,	 g_ifMng->m_DanWarRequestWin->m_ComboCtlDanWarType->GetCurSel());
		MSG_WriteByte(&netMessage,	 g_ifMng->m_DanWarRequestWin->m_ComboCtlDanWarTime->GetCurSel());
		
		MSG_WriteLong(&netMessage,	 g_ifMng->m_DanWarRequestWin->m_PrimiumItemCheckBox->GetCheck());		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}







void GTH_ProcessMessage_SuggestDanBattle()
{	
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo();
	if ( pDanBattleInfo == NULL)
		return;

	
	pDanBattleInfo->BattleMode = (NewDanBattleSystem::DanBattleInfo::enumBattleMode)MSG_ReadByte();

	
	pDanBattleInfo->fRemainSec	 = MSG_ReadFloat();
	pDanBattleInfo->RequestDanInfo.nDanIdx = MSG_ReadLong();
	pDanBattleInfo->RequestDanInfo.nRemainPoint = MSG_ReadByte();
	pDanBattleInfo->nBattleMapIdx = MSG_ReadByte();	
	_snprintf(pDanBattleInfo->RequestDanInfo.szGuildName, GUILDNAMESTRING, MSG_ReadString());
	pDanBattleInfo->RequestDanInfo.szGuildName[GUILDNAMESTRING-1] =  NULL;
	
	pDanBattleInfo->bPlunderPremiumItem = MSG_ReadLong();
	

	g_ifMng->m_DanWarRequestWin->InitDanWarWin(CIFDanWarRequestWin::DanWarRequestWinType::EXAMINATION,
		&g_NewDanBattleSystem);	
	
	g_ifMng->m_DanWarRequestWin->Enable(TRUE);
	g_ifMng->SetFocus(g_ifMng->m_DanWarRequestWin);	
}







void GTH_SendMessage_ReplySuggest(tagGCPacket_NewDanBattle::enumCode code)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	 EXTEND_SECOND);
		MSG_WriteShort(&netMessage,  NEW_DAN_BATTLE);
		MSG_WriteShort(&netMessage,	 CC_SUGGEST_DANBATTLE);		
		MSG_WriteByte(&netMessage,	 code);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_ProcessMessage_DanBattleInfo()
{	

	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo();
	pDanBattleInfo->BattleMode		= (NewDanBattleSystem::DanBattleInfo::enumBattleMode)MSG_ReadByte();
	pDanBattleInfo->BattleStatus	= (NewDanBattleSystem::DanBattleInfo::enumBattleStatus)MSG_ReadByte();


	
	pDanBattleInfo->nDanBattleIdx	= MSG_ReadLong();
	
	
	pDanBattleInfo->fRemainSec		= MSG_ReadFloat();
	
	pDanBattleInfo->nBattleMapIdx	= MSG_ReadByte();

	
	
	pDanBattleInfo->AcceptDanInfo.nDanIdx		= MSG_ReadLong();
	
	pDanBattleInfo->AcceptDanInfo.nRemainPoint	= MSG_ReadByte();
	
	pDanBattleInfo->AcceptDanInfo.nKillPoint	= MSG_ReadLong();

	
	pDanBattleInfo->AcceptDanInfo.naccumulationKillpoint = MSG_ReadLong();
	
	
	g_cgv.myCharacterInfo->Killpoint = MSG_ReadLong();

	
	g_cgv.myCharacterInfo->naccumulationKillpoint = MSG_ReadLong();	

	
	
	
	pDanBattleInfo->RequestDanInfo.nKillPoint = MSG_ReadLong();	
	
	pDanBattleInfo->RequestDanInfo.nRemainPoint = MSG_ReadByte();
	
	pDanBattleInfo->RequestDanInfo.naccumulationKillpoint = MSG_ReadLong();



	if ( pDanBattleInfo->BattleStatus != NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE)
		return;

	g_NewDanBattleSystem.SetDanBattleStatus(
		NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE);
	












	
	g_musicMng->ChangeBGMusic( "../BGMusic/danbattle.mp3" );
	
	
	g_ifMng->m_DanWarBattleWin->Enable(TRUE);	

}


void	GTH_SendMessage_ChangeStatus(NewDanBattleSystem::DanBattleInfo::enumBattleStatus status, char* guildname)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	 EXTEND_SECOND);
		MSG_WriteShort(&netMessage,  NEW_DAN_BATTLE);
		MSG_WriteShort(&netMessage,	 CS_DAN_BATTLE_CHANGE_STATUS);
		MSG_WriteLong(&netMessage,	 status);		
		MSG_WriteString(&netMessage, guildname);
		NET_SendMessage(&gsSocket,   &netMessage);
	}
	MSG_EndWriting(&netMessage);
}








void GTH_ProcessMessage_DanBattleNoticeMsg()
{
	NewDanBattleSystem::noticetype type = (NewDanBattleSystem::noticetype)MSG_ReadLong();
	char Msg[256];
	strncpy(Msg, MSG_ReadString(), 256);
	Msg[255] = NULL;

	DWORD color = GTHCOLOR_NOTICEMESSAGE;
	switch(type)
	{
	case NewDanBattleSystem::noticetype::notice:		
		break;

	case NewDanBattleSystem::noticetype::killpoint_notice:		
		color = GTHCOLOR_QUESTMESSAGE;
		break;

	case NewDanBattleSystem::noticetype::plunder_notice:		
		color = GTHCOLOR_LOOTMESSAGE;
		break;
	}
	g_NewDanBattleSystem.AddDanBattleNoticeMsg(Msg, color, type);	
	
}