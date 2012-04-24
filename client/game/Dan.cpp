#include "../global_def.h"																



void CDanBattle::SendSetStatus(BYTE bStatus)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_SETSTATUS);
		MSG_WriteByte(&netMessage, bStatus);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CDanBattle::SendRequestBattle()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_REQUEST_BATTLE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CDanBattle::SendSuggestBattle(int danIdx,LPSTR pDanName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_SUGGEST_BATTLE );
		MSG_WriteLong(&netMessage, danIdx);
		MSG_WriteString(&netMessage, pDanName);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CDanBattle::SendSuggestResult(BYTE bResult)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_SUGGEST_RESULT);
		MSG_WriteByte(&netMessage, bResult);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CDanBattle::SendChangeBattleMap()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_CHANGE_BATTLEMAP);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CDanBattle::SendCancelRequest()													
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_REQUEST_CANCEL);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CDanBattle::SendInitChao()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_INIT_CHAO);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}


void CDanBattle::SendDanTestMode(int mode)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_BATTLE_TEST_MODE);		
		MSG_WriteByte(&netMessage, mode);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}



void CDanBattle::RecvDanBattle()			
{
	BYTE bCommand;

	bCommand = MSG_ReadByte();	
	switch (bCommand)
	{
	case DANB_ERROR				: RecvErrorCode();			break;
	case DANB_REQUESTER_LIST	: RecvRequesterList();		break;
	case DANB_SUGGEST_BATTLE	: RecvSuggest();			break;
	case DANB_BATTLE_INFO		: RecvBattleInfo();			break;
	case DANB_BATTLE_RESULT		: RecvBattleResult();		break;
	case DANB_BATTLE_READY		: RecvBattleReady();		break;	
	case DANB_BATTLE_START		: RecvBattleStart();		break;
	case DANB_SUGGEST_RESULT	: RecvSuggestResult();		break;	
	case DANB_BATTLE_DAN		: RecvBattleDan();			break;
	case DANB_BATTLE_REWARD		: RecvBattleReward();		break;		
	case DANB_BATTLE_REFRESH_PERSONDATA								
								: RecvRefreshData();		break;
	}
}

void CDanBattle::RecvErrorCode()
{
	BYTE eCode	= MSG_ReadByte();
	BYTE eType	= MSG_ReadByte();





}

void CDanBattle::RecvRequesterList()
{
	BYTE	bSize=MSG_ReadByte();

	char tempMessage[MAX_PATH];
	char DanName[GUILDNAMESTRING];
	char UserName[NAMESTRING];
	int	 danIdx;
	




	
	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_guildArenaWin->Enable();	

	
	g_ifMng->SetFocus( g_ifMng->m_guildArenaWin );		
	g_ifMng->m_guildArenaWin->ClearStandbyList();
	g_ifMng->m_guildArenaWin->m_StandByCount = bSize;

	


	for (int i=0;i<bSize;i++)
	{
		danIdx=MSG_ReadLong();
		sstrncpy(DanName,MSG_ReadString(),GUILDNAMESTRING-1);
		sstrncpy(UserName,MSG_ReadString(),NAMESTRING-1);



		
		
		if ( i == 0 )		
			strcpy(g_ifMng->m_guildArenaWin->m_szATeamList, DanName);
		
		g_ifMng->m_guildArenaWin->m_szStandby_danIdx_List[i] = danIdx;
		
		
		if ( danIdx == g_cgv.myCharacterInfo->guildIdx )
			g_ifMng->m_guildArenaWin->m_MyTextIdxinStandby = i;
		

		strcpy(g_ifMng->m_guildArenaWin->m_szStandbyList[i], DanName);		
		
	}

	
	if ( bSize >= 2 )
	{
		if ( g_ifMng->m_guildArenaWin->m_szStandby_danIdx_List[0] ==
			g_cgv.myCharacterInfo->guildIdx )		
			g_ifMng->m_guildArenaWin->m_ReadyBattleFlag = g_ifMng->m_guildArenaWin->STATE_REQUEST;
		else 
			g_ifMng->m_guildArenaWin->m_ReadyBattleFlag = g_ifMng->m_guildArenaWin->STATE_NONE;

	}
	

}

void CDanBattle::RecvBattleInfo()
{
	int Point[2];
	DWORD remainTime;
	char tempMessage[MAX_PATH];

	Point[0]=MSG_ReadLong();
	Point[1]=MSG_ReadLong();
	remainTime=MSG_ReadLong();

	
	


	
	

	int totalUser=MSG_ReadLong();
	int aliveUser=MSG_ReadLong();

	
	g_ifMng->m_guildArenaInfoWin->m_AteamPoint	= Point[0];	
	g_ifMng->m_guildArenaInfoWin->m_AJoinTotal	= aliveUser;

	



	totalUser=MSG_ReadLong();
	aliveUser=MSG_ReadLong();

	g_ifMng->m_guildArenaInfoWin->m_BteamPoint	= Point[1];;		
	g_ifMng->m_guildArenaInfoWin->m_BJoinTotal	= aliveUser;	


	



	Point[0]=MSG_ReadLong();
	Point[1]=MSG_ReadLong();
	
	g_ifMng->m_guildArenaInfoWin->m_AGuildTotal	= Point[0];
	g_ifMng->m_guildArenaInfoWin->m_BGuildTotal	= Point[1];
	

	
	g_ifMng->m_guildArenaInfoWin->SetBattleRemainTime(remainTime);


	
	char DanName[2][GUILDNAMESTRING];
	sstrncpy(DanName[0],MSG_ReadString(),GUILDNAMESTRING-1);
	sstrncpy(DanName[1],MSG_ReadString(),GUILDNAMESTRING-1);

	strcpy(g_ifMng->m_guildArenaInfoWin->m_AteamName, DanName[0]);
	strcpy(g_ifMng->m_guildArenaInfoWin->m_BteamName, DanName[1]);
	
	
	
	
}


void CDanBattle::RecvBattleResult()
{
	char szTemp[256];

	BYTE bResult=MSG_ReadByte();
	switch (bResult)
	{
	case  0 :
		sprintf( szTemp, g_LPACK.GetMassage( 9, 57),  g_ifMng->m_guildArenaInfoWin->m_AteamName);
		g_ifMng->AddChatMessage( szTemp, GTHCOLOR_SYSMESSAGE, g_LPACK.GetMassage( 9, 45));
		break;
	case 1 :
		sprintf( szTemp, g_LPACK.GetMassage( 9, 57),  g_ifMng->m_guildArenaInfoWin->m_BteamName);
		g_ifMng->AddChatMessage( szTemp, GTHCOLOR_SYSMESSAGE, g_LPACK.GetMassage( 9,45));
		break;
	case 2 :
		g_ifMng->AddChatMessage( g_LPACK.GetMassage( 9,58), GTHCOLOR_SYSMESSAGE, g_LPACK.GetMassage( 9,45));
		break;
	}

	
	g_ifMng->m_guildArenaInfoWin->Enable(false);
	g_ifMng->m_guildArenaWin->ClearRegTeams();
	g_ifMng->m_miniMapWin->Enable();

	g_ifMng->m_guildArenaWin->m_ReadyBattleFlag = g_ifMng->m_guildArenaWin->STATE_NONE;
	
	
}

void CDanBattle::RecvBattleStart()
{
	
	g_ifMng->AddChatMessage( g_LPACK.GetMassage(9, 62), GTHCOLOR_SYSMESSAGE, g_LPACK.GetMassage(9, 67));


	
	g_ifMng->m_guildArenaWin->Enable(false);
	g_ifMng->m_guildArenaWin->ClearRegTeams();
	
	

	g_ifMng->m_guildArenaInfoWin->Enable();	
	g_ifMng->m_guildArenaWin->m_ReadyBattleFlag = g_ifMng->m_guildArenaWin->STATE_BATTLE;
	g_ifMng->m_miniMapWin->Enable(false);
	
	g_ifMng->SetFocus( g_ifMng->m_guildArenaInfoWin );
	

}



void CDanBattle::RecvSuggest()
{
	
	
	
	g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), g_LPACK.GetMessage(9,56), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_DAN_BATTLE_SUGGEST_ACCEPT, 10000.0f );
	
}







void CDanBattle::RecvSuggestResult()
{	
	BYTE type 	= MSG_ReadByte();		

	if( g_ifMng->m_msgType == IF_MESSAGE_DAN_BATTLE_SUGGEST_ACCEPT )		
			g_ifMng->ExitMessage();	
	

	
	switch ( type )
	{
		
	case 0:
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(9,54), D3DCOLOR_ARGB(255, 255, 125, 100) );		
		g_ifMng->m_guildArenaWin->Enable(FALSE);		

		
		g_ifMng->m_guildArenaWin->ClearRegTeams();
		
		
		break;

		
	case 1:
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(9,55), D3DCOLOR_ARGB(255, 255, 125, 100) );
		g_ifMng->m_guildArenaWin->m_ReadyBattleFlag = g_ifMng->m_guildArenaWin->STATE_READY;				
		break;

		
	case 2:		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0, 36), D3DCOLOR_ARGB(255, 255, 125, 100) );		
		g_ifMng->m_guildArenaWin->Enable(FALSE);

		
		g_ifMng->m_guildArenaWin->ClearRegTeams();
		
		
		break;	
	}	

	
}

void CDanBattle::RecvBattleDan()
{
	char tempString[MAX_PATH];
	char DanName[2][GUILDNAMESTRING];

	
	
	MSG_ReadLong();
	MSG_ReadLong();

	sstrncpy(DanName[0],MSG_ReadString(),GUILDNAMESTRING-1);
	sstrncpy(DanName[1],MSG_ReadString(),GUILDNAMESTRING-1);


	
	sprintf(tempString, g_LPACK.GetMassage(9, 63) ,DanName[0],DanName[1]);
	

	g_ifMng->AddSysMessage( tempString, D3DCOLOR_ARGB(255, 255, 125, 100) );

}


void CDanBattle::RecvBattleReady()
{
	g_ifMng->m_guildArenaWin->Enable(FALSE);
	g_ifMng->m_guildArenaWin->ClearRegTeams();

	
	g_ifMng->AddChatMessage( g_LPACK.GetMassage(9, 66), GTHCOLOR_SYSMESSAGE, g_LPACK.GetMassage(9, 67));	

	g_ifMng->m_guildArenaInfoWin->SetBattleRemainTime(MSG_ReadLong());
}

void CDanBattle::SendBattleReady()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_DAN_BATTLE );
		MSG_WriteByte(&netMessage, DANB_BATTLE_READY);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}





void CDanBattle::RecvBattleReward()
{
	int type=MSG_ReadLong();
	int danFame=MSG_ReadLong();
	int myFame=MSG_ReadLong();
	
	int rankpoint=MSG_ReadLong();
	

	char tempString[MAX_PATH];

	if (type)																			
	{
		sprintf(tempString,
				
				g_LPACK.GetMassage(9, 64),
				danFame,myFame);


		



		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
								66, 
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

	}
	else																				
	{
		sprintf(tempString,
			
			g_LPACK.GetMassage(9, 65),
			danFame,myFame);
		
		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
								67, 
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
	}
	
	
	g_ifMng->AddSysMessage( tempString, D3DCOLOR_ARGB(255, 255, 125, 100) );	

	
	g_ifMng->m_guildArenaWin->m_ReadyBattleFlag = g_ifMng->m_guildArenaWin->STATE_NONE;
	g_ifMng->m_guildArenaInfoWin->SetBattleRemainTime(0); 
	
	
	
}


void CDanBattle::RecvRefreshData()
{
	int type=MSG_ReadLong();
	
	
	switch (type)
	{
	case 0:
		{		
			int Nak=MSG_ReadLong();
			g_cgv.myCharacterInfo->curChargeSE = Nak;
		}
		break;		

	case 1:
		
		
		{		
			int rankpoint=MSG_ReadLong();
			int famepoint=MSG_ReadLong();
			
			g_cgv.myCharacterInfo->rankPoint = rankpoint;
			g_cgv.myCharacterInfo->guild.famePoint = famepoint;
		}
		break;

		
	case 2:
		{
			DWORD remaintime=MSG_ReadLong();
			g_ifMng->m_guildArenaInfoWin->SetBattleRemainTime(remaintime);
		}
		break;

		
	case 3:
		int	Danstate=MSG_ReadLong();
		g_ifMng->m_guildArenaWin->m_ReadyBattleFlag = Danstate;
		break;
		
		
	}
}