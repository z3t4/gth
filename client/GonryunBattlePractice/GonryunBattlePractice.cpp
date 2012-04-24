



#include "../global_def.h"	





CGonryunBattlePractice::CGonryunBattlePractice()
{
	m_ibattleMode = PCB_NONE;
	m_ieffectMode = PCB_EFF_NONE;
	m_ieffectIndex = 0;
	m_dwTimes = 0;
	m_dwFPS = 0.5f;
	m_arrEffIdx = 0;
	m_bshow = FALSE;
	m_ibattleinfoMode = PCB_NONE;
	m_fAngle = 0.0f;
	m_fAngle=timeGetTime();
	m_iLevelDiffer = 0;
	m_leader.pcidx = -1;
	m_leader.Name[0] = NULL;
	m_iroyalty = 0;
}



CGonryunBattlePractice::~CGonryunBattlePractice()
{

}
		
												

void CGonryunBattlePractice::SendCancelRequest()													
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_GET_CANCEL);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}


void CGonryunBattlePractice::SendBanBatting(void)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_BAN_BATTING);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CGonryunBattlePractice::SendCancelBatting(void)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_CANCEL_BATTING);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CGonryunBattlePractice::RecvDanBattle()			
{
	BYTE bCommand;

	bCommand = MSG_ReadByte();	
	switch (bCommand)
	{
	case BATTLE_PRACTICE_GET_OTHERLISTINFO	:		RecvRequesterList();			  break;
	case BATTLE_PRACTICE_END	:					RecvGonryunEnd();				  break;
	case BATTLE_PRACTICE_SEND_OTHERMYINFO:			RecvOtherMyinfo();				  break;
	case BATTLE_PRACTICE_REQUEST_DELETE_USER :		RecvGonryunDeleteUser();		  break;
	case BATTLE_PRACTICE_GET_MYINFO:				RecvMyinfo();				 	  break;
	case BATTEL_PRACTICE_REQUEST_BATTELBATTING:		RecvBattleBatting();			  break; 
	case BATTLE_PRACTICE_REQUEST_BATTELGENERATE:	RecvBattleGenerate();			  break; 
	case BATTLE_PRACTICE_BATTING_RESULT:			RecvRequestResultBatting();		  break; 
	case BATTLE_PRACTICE_GENERATE_RESULT:			RecvRequestResultGenerate();	  break; 
	case BATTLE_PRACTICE_STATE:						RecvGonryunState();				  break; 
	case BATTLE_PRACTICE_RWORDFIELD:				RecvGonryunRewordField();		  break; 
	case BATTLE_PRACTICE_BATTINGTRADE_REQUEST:			GTH_ProcessMessage_PCTrade_RequestBatting();	break;
	case BATTLE_PRACTICE_BATTINGTRADE_RESULT:			GTH_ProcessMessage_PCTrade_AdmitBatting();		break;
	case BATTLE_PRACTICE_BATTINGTRADE_REJECT:			GTH_ProcessMessage_PCTrade_RejectBatting();		break;
	case BATTLE_PRACTICE_BATTINGTRADE_OK:				GTH_ProcessMessage_PCTrade_OkBatting();			break; 
	case BATTLE_PRACTICE_BATTINGTRADE_CANCEL:			GTH_ProcessMessage_PCTrade_CancelBatting();		break; 
	case BATTLE_PRACTICE_BATTINGTRADE_EXIT :				GTH_ProcessMessage_PCTrade_ExitBatting();		break;
	case BATTLE_PRACTICE_BATTINGTRADE_ITEM :				GTH_ProcessMessage_PCTrade_ItemBatting();		break;
	case BATTLE_PRACTICE_BATTINGTRADE_CONFIRM :			GTH_ProcessMessage_PCTrade_ConfirmBatting();	break;
	case BATTLE_PRACTICE_BATTINGTRADE_SE :				GTH_ProcessMessage_PCTrade_SEBatting();			break;	
	case BATTLE_PRACIICE_EFFECT_MODE:				RecvProcessEffect();							break;	
	case BATTEL_PRACTICE_INFO:						RecvGonryunBattleInfo();						break;
	case BATTLE_PRACTICE_FIELDOUT:					RecvGonryunFieldOut();							break;
	case BATTLE_PRACTICE_BATTING_REJECT:			RecvGonryunBattigReject();						break;
    case BATTLE_PRACTICE_GENERATE_REJECT:			RecvGonryunGenerateReject();					break;
	case BATTLE_PRACTICE_MSG:						RecvBattleMsg();								break;		
	case BATTEL_PRACTICE_NAKE_RESULT :				RecvBattleNake();								break;		

	case BATTLE_PRACTICE_MYINFO_TOUSER:				RecvBattleAddUser();			break;
	case BATTLE_PRACTICE_DELETE_USER:				RecvDeleteUser();				break;
	case BATTEL_PRACTICE_RESULT:					RecvRequestGonryunResult();		  break; 
	case BATTLE_PRACTICE_OPEN:						RecvBattleOpen();	break;
	
	}
}


void	CGonryunBattlePractice::RecvBattleOpen(void)
{
	m_iroyalty = MSG_ReadLong();
	m_ibattingRoyalty = MSG_ReadLong();
}


void CGonryunBattlePractice::RecvDeleteUser(void)
{
	char czUserName[NAMESTRING+1];
	strncpy(czUserName,MSG_ReadString(),NAMESTRING);
	czUserName[NAMESTRING] = NULL;

	for(int i=0; i < MAX_STANDBYCOUNT; i++)
	{
		int compare = strncmp(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i],czUserName,NAMESTRING);
		if(0 == compare)
		{
			memset(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i], NULL , sizeof (char) * NAMESTRING );
			g_ifMng->m_pgonryunBattlePractice->m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+i]->SetText("");		
			g_ifMng->m_pgonryunBattlePractice->m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+i]->SetColor(0xff808080);
			g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].win  = 0;
			g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].draw = 0;
			g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].lose = 0;
			g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].level= 0;
			g_ifMng->m_pgonryunBattlePractice->m_currentUserCount--;
			break;
		}
	}	
}

void CGonryunBattlePractice::RecvBattleAddUser(void)
{
	char czUserName[NAMESTRING+1];
	strncpy(czUserName,MSG_ReadString(),NAMESTRING);
	czUserName[NAMESTRING]=NULL;

	for(int i=0; i < MAX_STANDBYCOUNT; i++)
	{
		int compare = strcmp(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i],"");
		if(0 == compare)
		{
				strcpy(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i],czUserName);
				g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].win=MSG_ReadByte();
				g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].draw=MSG_ReadByte();
				g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].lose=MSG_ReadByte();
				g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].level=MSG_ReadByte();
				g_ifMng->m_pgonryunBattlePractice->m_currentUserCount++;
			break;
		}
	}
}


void CGonryunBattlePractice::RecvBattleNake()
{
	g_cgv.myCharacterInfo->curChargeSE=MSG_ReadLong();
}



void CGonryunBattlePractice::RecvRequestGonryunResult()
{
	char cz_temp[256];
	char cz_name[NAMESTRING+1];
	cz_name[0]=NULL;
	int result=MSG_ReadByte();
	
	strncpy(cz_name,MSG_ReadString(),NAMESTRING);
	cz_name[NAMESTRING]=NULL;
	
	switch(result) {
	case 0: 
	
	
		break;
	case 1: 
		sprintf( cz_temp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,118), cz_name );
		g_ifMng->AddSysMessage( cz_temp, D3DCOLOR_ARGB(255, 255, 125, 100) );
		break;
	case 2: 
		
	
	
		break;

	}
	

}


void CGonryunBattlePractice::RecvGonryunBattigReject()
{
	if( g_ifMng->m_msgType == IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING )		
			g_ifMng->ExitMessage();	
}

void CGonryunBattlePractice::RecvGonryunGenerateReject()
{
	if( g_ifMng->m_msgType == IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_GENERATE )		
			g_ifMng->ExitMessage();	
}

void CGonryunBattlePractice::RecvGonryunFieldOut()
{
	g_ifMng->AddSysMessage( g_LPACK.GetMessage(0, 914), GTHCOLOR_QUESTMESSAGE );
}

void CGonryunBattlePractice::RecvErrorCode()
{
	BYTE eCode	= MSG_ReadByte();
	BYTE eType	= MSG_ReadByte();
}

void CGonryunBattlePractice::RecvRequesterList()
{
	g_ifMng->m_pgonryunBattlePractice->ClearStandbyList();

	BYTE	bSize=MSG_ReadByte();
	m_iLevelDiffer = MSG_ReadByte();
	g_ifMng->m_pgonryunBattlePractice->m_currentUserCount = bSize;

	char czUserName[NAMESTRING+1];
	strncpy(czUserName,MSG_ReadString(),NAMESTRING);
	czUserName[NAMESTRING]=NULL;
	int bisLeader=MSG_ReadShort();

	g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[0].win=MSG_ReadByte();
	g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[0].draw=MSG_ReadByte();
	g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[0].lose=MSG_ReadByte();
	g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[0].level=MSG_ReadByte();
  
	g_ifMng->m_chatWin->Enable( FALSE );
 	g_ifMng->m_pgonryunBattlePractice->Enable();	
	g_ifMng->SetFocus( g_ifMng->m_pgonryunBattlePractice);		

	strncpy(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[0],czUserName,NAMESTRING);
	g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[0][NAMESTRING] = NULL;

		strncpy(g_ifMng->m_pgonryunBattlePractice->m_szATeamList,czUserName,NAMESTRING);
	g_ifMng->m_pgonryunBattlePractice->m_szATeamList[NAMESTRING] = NULL;
		g_ifMng->m_pgonryunBattlePractice->m_RegAteamIdx=0;

	
	char UserName[NAMESTRING+1];
	for (int i=1; i < bSize; i++)
	{
		strncpy(UserName,MSG_ReadString(),NAMESTRING);
		UserName[NAMESTRING] = NULL;
		strncpy(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i], UserName,NAMESTRING);
		g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i][NAMESTRING] = NULL;
		short int bisleader=MSG_ReadShort();
		if( 0 == strcmp(g_cgv.syncCharacter[0].name,UserName))
		{
			
			g_ifMng->m_pgonryunBattlePractice->m_MyTextIdxinStandby=i;
		}
		g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].win=MSG_ReadByte();
		g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].draw=MSG_ReadByte();
		g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].lose=MSG_ReadByte();
		g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].level=MSG_ReadByte();
	}

	if ( bisLeader ==  1)
	{
	    if(0 == strncmp(g_cgv.syncCharacter[0].name,czUserName,NAMESTRING))
		{
				g_ifMng->m_pgonryunBattlePractice->m_isLeader = TRUE;
				g_ifMng->m_pgonryunBattlePractice->m_MyTextIdxinStandby=0;
		}
	}


}


void CGonryunBattlePractice::RecvGonryunEnd()
{
	if(0==g_ifMng->m_pgonryunBattlePractice->State()) return;
	g_ifMng->m_pgonryunBattlePractice->Enable(FALSE);
	g_ifMng->m_pgonryunBattlePractice->ClearStandbyList();
	m_iLevelDiffer = 0;
	

}


void CGonryunBattlePractice::RecvGonryunDeleteUser()
{
	
 	char czUser[NAMESTRING];
	strncpy(czUser,MSG_ReadString(),NAMESTRING);
	czUser[NAMESTRING-1]=NULL;
	for (int i = 0; i < MAX_STANDBYCOUNT; i ++)
	{
		int compare=strcmp(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i],czUser);
		if(0==compare)
		{
			memset(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i], NULL , sizeof (char) * NAMESTRING );
			g_ifMng->m_pgonryunBattlePractice->m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+i]->SetText("");		
			g_ifMng->m_pgonryunBattlePractice->m_textCtl[IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT+i]->SetColor(0xff808080);
			g_ifMng->m_pgonryunBattlePractice->m_currentUserCount--;
			break;
		}
	}
}

void CGonryunBattlePractice::RecvOtherMyinfo()
{
	char czUserName[NAMESTRING];
	strncpy(czUserName,MSG_ReadString(),NAMESTRING);
	czUserName[NAMESTRING-1]=NULL;
	int bisLeader=MSG_ReadShort();

	for(int i=0; i < MAX_STANDBYCOUNT; i++)
	{
		int compare=strcmp(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i],"");
		if(0==compare)
		{
				strcpy(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[i],czUserName);
				g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].win=MSG_ReadByte();
				g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].draw=MSG_ReadByte();
				g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].lose=MSG_ReadByte();
				g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[i].level=MSG_ReadByte();
				g_ifMng->m_pgonryunBattlePractice->m_currentUserCount++;
			break;
		}
	}

}

void CGonryunBattlePractice::RecvMyinfo()
{
	char czUserName[NAMESTRING];
	strncpy(czUserName,MSG_ReadString(),NAMESTRING);
	czUserName[NAMESTRING-1]=NULL;
	g_ifMng->m_pgonryunBattlePractice->ClearStandbyList();
	int bisLeader=MSG_ReadShort();
	g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[0].win=MSG_ReadByte();
	g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[0].draw=MSG_ReadByte();
	g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[0].lose=MSG_ReadByte();
	g_ifMng->m_pgonryunBattlePractice->m_arrayUserInfo[0].level=MSG_ReadByte();
  
	g_ifMng->m_chatWin->Enable( false );
 	g_ifMng->m_pgonryunBattlePractice->Enable();	
	g_ifMng->SetFocus( g_ifMng->m_pgonryunBattlePractice);		


	strncpy(g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[0],czUserName,NAMESTRING);

	
	g_ifMng->m_pgonryunBattlePractice->m_MyTextIdxinStandby=0;
	g_ifMng->m_pgonryunBattlePractice->m_currentUserCount++;
	g_ifMng->m_pgonryunBattlePractice->m_isLeader=(BOOL)bisLeader;
	if(bisLeader==TRUE)
	{
		strncpy(g_ifMng->m_pgonryunBattlePractice->m_szATeamList,czUserName,NAMESTRING);
		g_ifMng->m_pgonryunBattlePractice->m_RegAteamIdx=0;
	}
	
	
}

void CGonryunBattlePractice::RequestBattleBatting(char *pUser)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_REQUEST_BATTELBATTING);
		MSG_WriteString(&netMessage, pUser);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CGonryunBattlePractice::RequestBattleGenerate(char *pUser) 
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_REQUEST_BATTELGENERATE );
		MSG_WriteString(&netMessage, pUser);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}


void CGonryunBattlePractice::RecvBattleBatting()
{
	char cz_string[256];
	sprintf(cz_string,g_LPACK.GetMessage(9,112),g_LPACK.GetMessage(9,128));
	g_ifMng->SetMessage( g_LPACK.GetMessage(0,322),cz_string, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING, 10000.0f );
}


void CGonryunBattlePractice::RecvBattleGenerate()
{	
	char cz_string[256];
	m_leader.pcidx = MSG_ReadLong();
	m_leader.Name[0]=NULL;
	strncpy(m_leader.Name,MSG_ReadString(),NAMESTRING);
	m_leader.Name[NAMESTRING-1] = NULL;

	sprintf(cz_string,g_LPACK.GetMessage(9,197),m_leader.Name);	
	g_ifMng->SetMessage( g_LPACK.GetMessage(0,322),cz_string, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_GENERATE, 10000.0f );
}
	




void CGonryunBattlePractice::SendSuggestResultGenerate(BYTE bresult)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_GENERATE_RESULT);
		if(g_ifMng->m_pgonryunBattlePractice->m_isLeader!=TRUE){
			MSG_WriteLong(&netMessage, m_leader.pcidx);
			MSG_WriteString(&netMessage,m_leader.Name);
		}
		else{
			MSG_WriteString(&netMessage,g_ifMng->m_pgonryunBattlePractice->m_szStandbyList[g_ifMng->m_pgonryunBattlePractice->m_selectStandbyIdx]);
		}
		MSG_WriteByte(&netMessage, bresult);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CGonryunBattlePractice::RecvRequestResultBatting()
{
	BYTE type 	= MSG_ReadByte();		

	if( g_ifMng->m_msgType == IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING )		
			g_ifMng->ExitMessage();	
	

	
	switch ( type )
	{
		
	case 0:
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(9,134), D3DCOLOR_ARGB(255, 255, 125, 100) );		
	
		break;

		
	case 1:
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(9,135), D3DCOLOR_ARGB(255, 255, 125, 100) );
		g_ifMng->m_pgonryunBattlePractice->ClearStandbyList();
		g_ifMng->m_pgonryunBattlePractice->Enable(false);
		g_ifMng->m_pgonryunBattlePractice->ClearRegTeams();
		break;

		
	case 2:		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0, 36), D3DCOLOR_ARGB(255, 255, 125, 100) );		
		g_ifMng->m_pgonryunBattlePractice->Enable(FALSE);

		
		g_ifMng->m_pgonryunBattlePractice->ClearRegTeams();
	
		
		break;	
	}	
}

void CGonryunBattlePractice::RecvRequestResultGenerate()	
{
	BYTE type 	= MSG_ReadByte();		

	if( g_ifMng->m_msgType == IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_GENERATE )		
			g_ifMng->ExitMessage();	
	

	
	switch ( type )
	{
	
	case 0:
	
		break;

	
	case 1:
		g_cgv.myCharacterInfo->m_bbattleMode=TRUE;
		g_ifMng->m_pgonryunBattlePractice->m_bbattleMode=TRUE;
	
					
		break;

		
	case 2:		
	
		g_ifMng->m_pgonryunBattlePractice->Enable(FALSE);
		g_ifMng->m_pgonryunBattlePractice->ClearRegTeams();

		
		break;	
	}	
}

void CGonryunBattlePractice::RecvGonryunState()
{
	
	m_ibattleMode=MSG_ReadShort();
	
	switch(m_ibattleMode) {
	case PCB_BATTING: 
		g_input->MouseEnable(true);
		g_input->KeyEnable(true);
		g_ifMng->m_pGonryunBattleProgressInfo->Enable(true);
	    g_ifMng->m_pgonryunBattleBattingwindow->Enable(false);
		g_ifMng->m_itemWin->Enable(false);
		g_ifMng->m_miniMapWin->Enable(false);
		g_ifMng->SetFocus( g_ifMng->m_pGonryunBattleProgressInfo );
		break;
	case PCB_START:
		g_input->MouseEnable(true);
		g_input->KeyEnable(true);
		g_ifMng->m_pGonryunBattleProgressInfo->Enable(true);
	    g_ifMng->m_pgonryunBattleBattingwindow->Enable(false);
		g_ifMng->m_itemWin->Enable(false);
		g_ifMng->m_miniMapWin->Enable(false);
		g_ifMng->SetFocus( g_ifMng->m_pGonryunBattleProgressInfo );
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(9, 115), D3DCOLOR_ARGB(255, 255, 125, 100) );	
		break;
	case PCB_END: 
		
		
		
		g_input->MouseEnable(true);
		g_input->KeyEnable(true);
		g_ifMng->m_pGonryunBattleProgressInfo->m_RemainTime=0;
		g_ifMng->m_pGonryunBattleProgressInfo->Enable(false);	
		g_ifMng->m_miniMapWin->Enable(true);
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(9, 116), D3DCOLOR_ARGB(255, 255, 125, 100) );
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0, 914), GTHCOLOR_QUESTMESSAGE );	
		break;

	case PCB_NONE: 	
		g_input->MouseEnable(true);
		g_input->KeyEnable(true);
		g_ifMng->m_pGonryunBattleProgressInfo->m_RemainTime=0;
		g_ifMng->m_pGonryunBattleProgressInfo->Enable(false);	
		g_ifMng->m_miniMapWin->Enable(true);
	
		break;
	case PCB_READY: 
		g_input->MouseEnable(false);
		g_input->KeyEnable(false);
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0, 913), GTHCOLOR_QUESTMESSAGE );
		g_ifMng->m_pgonryunBattlePractice->Enable(false);
		g_ifMng->m_pgonryunBattleBattingwindow->Enable(false);
		g_ifMng->m_itemWin->Enable(false);
		break;
	default	:
		g_ifMng->m_pgonryunBattleBattingwindow->Enable(false);
		g_ifMng->m_itemWin->Enable(false);
			break;
	}

}


void CGonryunBattlePractice::RecvGonryunRewordField()
{
	short int rewordtype=MSG_ReadShort();

	

}


void CGonryunBattlePractice::ProcessGonryunEffect()
{
	
	if(m_ieffectMode == PCB_EFF_NONE) return;
	switch(m_ieffectMode) {

	case PCB_EFF_READY:
		{
			if(timeGetTime() - m_dwTimes>900/1)
			{
				m_bshow=TRUE;
				m_ieffectIndex++;
				if(m_ieffectIndex>64) 
				{
					m_ieffectMode=PCB_EFF_NONE;
					g_input->MouseEnable(true);
					g_input->KeyEnable(true);
					m_bshow=FALSE;
					break;
				}
				
				m_dwTimes=timeGetTime();
			}
			
			else m_bshow=FALSE;
			
			float fTime=(timeGetTime() - m_fAngle)*0.03f;
			m_fAngle=timeGetTime();
			
			Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
			
			if( g_camera.m_angles[ YAW ] <  0.0f )     g_camera.m_angles[ YAW ] += 360.0f;
			if( g_camera.m_angles[ YAW ] >  360.0f )   g_camera.m_angles[ YAW ] -= 360.0f;
			
			g_camera.m_angles[ YAW ] += fTime;	
			if(m_bshow==TRUE)
			{
				
				
				if(m_ieffectIndex<=63)
				{
					g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_COUNT ), GTH_SOUND_PLAY_ONCE );
				}
				
				else 
				{
					g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_READYFIGHT ), GTH_SOUND_PLAY_ONCE );
				}
				
				g_input->MouseEnable(false);
				g_input->KeyEnable(false);
				m_arrEffIdx = g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
					m_ieffectIndex, 
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
				g_input->MouseEnable(true);
				g_input->KeyEnable(true);
				m_bshow=FALSE;
			}
			
		}
		
		break;
		
		
		
	case PCB_EFF_WIN:
		if(m_bshow==TRUE)
		{
			Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
			VectorCopy( g_camera.m_angles, myChar->angles );
			g_camera.m_angles[YAW] = myChar->angles[YAW] + 180.0f;
			
			g_musicMng->PlaySample3D( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_COUNT ), GTH_SOUND_PLAY_ONCE , myChar->position );
			if(m_ieffectIndex!=66) break;
			m_arrEffIdx = g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
				m_ieffectIndex, 
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

			m_ieffectMode=PCB_EFF_NONE;
			m_bshow=FALSE;
		}
		
		break;
	case PCB_EFF_DEFEAT:
		if(m_bshow==TRUE)
		{
			Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
			VectorCopy( g_camera.m_angles, myChar->angles );
			g_camera.m_angles[YAW] = myChar->angles[YAW] + 180.0f;
		
			
			g_musicMng->PlaySample3D( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_FAIL ), GTH_SOUND_PLAY_ONCE , myChar->position );

			m_arrEffIdx = g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
				m_ieffectIndex, 
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
			m_ieffectMode=PCB_EFF_NONE;

			m_bshow=FALSE;
		}
		break;
   }

}

void CGonryunBattlePractice::RecvProcessEffect()
{
	m_ieffectMode=MSG_ReadShort();
	switch(m_ieffectMode)
	{
	case PCB_EFF_READY: 
		{
			m_ieffectIndex=53;
			m_bshow=TRUE;
		}
	
		break;
	case PCB_EFF_WIN: 
		{
			g_ifMng->m_pGonryunBattleProgressInfo->m_RemainTime=0;
			g_ifMng->m_pGonryunBattleProgressInfo->Enable(false);	
			g_ifMng->m_miniMapWin->Enable(true);
		

			m_ieffectIndex=66;
			m_bshow=TRUE;
		}
		
		break;
	case PCB_EFF_DEFEAT: 
		{
			g_ifMng->m_pGonryunBattleProgressInfo->m_RemainTime=0;
			g_ifMng->m_pGonryunBattleProgressInfo->Enable(false);	
			g_ifMng->m_miniMapWin->Enable(true);
	
			m_ieffectIndex=67;
			m_bshow=TRUE;
		}
		
		break;
	default:
			m_ieffectIndex=0;
			m_bshow=TRUE;
		break;

	}
}

void CGonryunBattlePractice::RecvGonryunBattleInfo()
{
	g_ifMng->m_pGonryunBattleProgressInfo->m_RemainTime=MSG_ReadLong();
}

void CGonryunBattlePractice::GTH_SendMessage_CheatGonryunCode(short int message)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
	MSG_WriteByte(&netMessage, CC_EXTEND );
	MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
	MSG_WriteLong(&netMessage, message);
	NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CGonryunBattlePractice::SendGonryunBan()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BAN);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );	
}

void CGonryunBattlePractice::SendGonryunStart(int minLevel,int maxLevel)							
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_START);
		MSG_WriteLong(&netMessage, minLevel);
		MSG_WriteLong(&netMessage, maxLevel);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGonryunBattlePractice::SendMessagePosition(float pos_x,float pos_y, float pos_z)	
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_POSITION);
	    MSG_WriteFloat(&netMessage,pos_x);
		MSG_WriteFloat(&netMessage,pos_y);
		MSG_WriteFloat(&netMessage,pos_z);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGonryunBattlePractice::SendFailItemAdd(void)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_FAILADDITEM);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGonryunBattlePractice::RecvBattleMsg()									
{
	int		reply;
	int		readValue[3];
	char	msgBuffer[MAX_PATH];

	memset(msgBuffer,0,MAX_PATH);

	reply=MSG_ReadLong();
	readValue[0]=MSG_ReadLong();
	readValue[1]=MSG_ReadLong();
	readValue[2]=MSG_ReadLong();

	switch (reply)
	{
	case overflow_oneday_use_count :
		sprintf(msgBuffer, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 130 ), readValue[0]);
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), msgBuffer );
		break;
	case missmatch_user_level :
		sprintf(msgBuffer, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 109 ), readValue[0], readValue[1]);
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), msgBuffer );
		break;
	case not_enoughf_nak :
		
		sprintf(msgBuffer, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 128 ), readValue[0]);
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), msgBuffer );
		break;
	case busy_GonryunBattlePractice :
		sprintf(msgBuffer, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 119 ));
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), msgBuffer );
		break;
	case overflow_user_number :
		sprintf(msgBuffer, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 110 ));
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), msgBuffer );
		break;
	case gonryun_NotUse :
		sprintf(msgBuffer, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 119 ));
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), msgBuffer );
		break;
	case differLevel : 
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,111), 1, IF_MESSAGE_NONE );
		break;
	case busy_Suggest:
	
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,534), 1, IF_MESSAGE_NONE );
		break;
	case unknown:
	
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,535), 1, IF_MESSAGE_NONE );
		break;
	case Weight_Over:
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,583), 1, IF_MESSAGE_NONE );
		break;
	
	case NotEnough_InventorySpace:
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,584), 1, IF_MESSAGE_NONE );
		break;

	case gonryun_Lest_UseCount:
		sprintf(msgBuffer, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,585), readValue[0]);
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), msgBuffer,1, IF_MESSAGE_NONE);
		break;
	}
}





