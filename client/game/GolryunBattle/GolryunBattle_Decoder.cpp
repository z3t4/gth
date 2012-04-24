




#include "../global_def.h"
#include "../game/GolryunBattle/GolryunBattle_Decoder.h"
								
CGolryunBattle_Decoder g_GolryunBattle_Decoder;							

CGolryunBattle_Decoder::CGolryunBattle_Decoder()
{
	
}

CGolryunBattle_Decoder::~CGolryunBattle_Decoder()
{
	
}


void CGolryunBattle_Decoder::RecvGolryunBattle()			
{
	BYTE bCommand;

	bCommand = MSG_ReadByte();	
	switch (bCommand)
	{	

		case tagGolryun_Packet_Status::GB_REQUEST_START_COUNT:			RecvEffectCount();			break;			
		case tagGolryun_Packet_Status::GB_REQUEST_PROGRESS_MATCH:		RecvStartMatch();			break;		
		case tagGolryun_Packet_Status::GB_REQUEST_END_MATCH:			RecvEndMatch();				break;	
		case tagGolryun_Packet_Status::GB_REQUEST_CANCEL_TOURNAMENT:	RecvCancelTournament();		break;		
		case tagGolryun_Packet_Status::GB_ERROR_CODE:					RecvErrorCode();			break;	
		case tagGolryun_Packet_Status::GB_TRANS_GOLRYUN:				RecvTransGolryun();			break;	
		case tagGolryun_Packet_Status::GB_TRANS_ARAYAN:					RecvTransArayan();			break;	
		case tagGolryun_Packet_Status::GB_SUGGEST_BATTLE:				RecvSuggestBattle();		break;			
		case tagGolryun_Packet_Status::GB_EFFECT_RESULT:				RecvEffectResult();			break;			
		case tagGolryun_Packet_Status::GB_OPEN_BATTLE_TICKET:			RecvOpenBattleTicket();		break;	
		case tagGolryun_Packet_Status::GB_SHARE_RATE:					RecvShareRate();			break;
		case tagGolryun_Packet_Status::GB_RENEW_BATTLE_TICKET:			RecvReNewBattleTicket();	break;
		case tagGolryun_Packet_Status::GB_OPEN_SHARE_WIN:				RecvOpenShareWin();			break;
		case tagGolryun_Packet_Status::GB_DISPLAY_TOURNAMENT:			RecvDisplayTournament();	break;
		case tagGolryun_Packet_Status::GB_CHECK_PRIZE:					RecvCheckPrize();			break;
		case tagGolryun_Packet_Status::GB_LOCAL_USER_NOTICE:			RecvLocalUserNotice();		break;
		case tagGolryun_Packet_Status::GB_ALL_USER_DATA:				RecvAllUserData();			break;
		case tagGolryun_Packet_Status::GB_START_MATCH_NOTICE:			RecvStartMatchNotice();		break;
		case tagGolryun_Packet_Status::GB_END_MATCH_NOTICE:				RecvEndMatchNotice();		break;
		case tagGolryun_Packet_Status::GB_BATTLE_RASA_Sync:				RecvSyncRASAMatch();		break;
		case tagGolryun_Packet_Status::GB_CHECK_JOINER_NUM:				RecvCheckJoinerNum();		break;
		case tagGolryun_Packet_Status::GB_STATUS_MATCH_NOTICE:			RecvStatusMatchNotice();	break;
		case tagGolryun_Packet_Status::GB_NOTICE_BATTLE:				RecvNoticeBattle();			break;
		case tagGolryun_Packet_Status::GB_BUY_BATTLE_TICKET:			RecvBuyBattleTicket();		break;
		case tagGolryun_Packet_Status::GB_TWOGYUN_TOTAL_NAK:			RecvTwoGyunTotalNak();		break;
		case tagGolryun_Packet_Status::GB_OBSERVE_START:				RecvObserveStart();			break;
		case tagGolryun_Packet_Status::GB_OBSERVE_END:					RecvObserveEnd();			break;
		case tagGolryun_Packet_Status::GB_OBSERVE_MATCH_TIME:			RecvObserveMatchTime();		break;
		case tagGolryun_Packet_Status::GB_MATCH_PROGRESS_TIME:			RecvMatchProgressTime();	break;
		case tagGolryun_Packet_Status::GB_GM_NOTICE:					RecvMessageToGM();			break;
	}
}

void CGolryunBattle_Decoder::RecvMessageToGM()
{
	int LpackIdx = MSG_ReadLong();

	if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;

	g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
		g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GOLRYUN_BATTLE, LpackIdx ));
}

void CGolryunBattle_Decoder::RecvObserveMatchTime()
{
	int remainTime = MSG_ReadLong();
	if(remainTime<0)	remainTime = 0;
	g_ifMng->m_observerInfoWin->m_RemainTime = remainTime;
}

void CGolryunBattle_Decoder::RecvObserveEnd()
{	
	GTH_UnlockSending();	
	
	BOOL bSuccess = MSG_ReadByte();
	if(FALSE == bSuccess){
		g_pApp->m_bAppExit = TRUE;
		PostMessage(g_pApp->m_hWnd,WM_CLOSE,0,0);
		return;
	}

	g_GolryunBattle.SetObserverMode(CGolryunBattle::eObserverMode::general);
	
	memcpy(g_pApp->m_myCharacter->position, g_GolryunBattle.m_OldPosition, sizeof(vec3_t));
	g_pApp->m_myCharacter->m_bObserveMode = FALSE;
	
	g_camera.m_maxCamDist = -350.0f;
	g_camera.m_distance = -350.0f;

	
	g_ifMng->m_observerInfoWin->m_RemainTime = 0;
	g_ifMng->m_observerInfoWin->m_redFighterNumber = 0;
	g_ifMng->m_observerInfoWin->m_blueFighterNumber = 0;
	

	
	for( int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if(g_ifMng->m_winControl[i]->BeforeState())
		{
			g_ifMng->m_winControl[i]->SetBeforeState(false);
			g_ifMng->m_winControl[i]->Enable(true);
		}		
	}

	g_ifMng->m_observerMenuWin->Enable(false);
	g_ifMng->m_observerInfoWin->Enable(false);
	
	g_ifMng->m_tournamentMiniWin->Enable(false);
	g_ifMng->m_battleTicketWin->Enable(false);
	g_ifMng->m_shareWin->Enable(false);
		
}

void CGolryunBattle_Decoder::RecvObserveStart()
{

	BOOL bSuccess = MSG_ReadByte();
	if(FALSE == bSuccess && FALSE == g_pApp->m_myCharacter->m_bWaitingRoom){
		g_pApp->m_bAppExit = TRUE;
		PostMessage(g_pApp->m_hWnd,WM_CLOSE,0,0);
		return;
	}
	
	g_GolryunBattle.SetObserverMode(CGolryunBattle::eObserverMode::observe);
	g_camera.m_maxCamDist = -700.0f;

	
	memcpy(g_GolryunBattle.m_OldPosition, g_pApp->m_myCharacter->position, sizeof(vec3_t));
	g_pApp->m_myCharacter->position[0] = 2108;
	g_pApp->m_myCharacter->position[1] = -11771;
	g_pApp->m_myCharacter->position[2] = -300;
	

	
	g_GolryunBattle.m_ObserverPos[0] = 2108;
	g_GolryunBattle.m_ObserverPos[1] = -11771;
	g_GolryunBattle.m_ObserverPos[2] = 180;
	
	
	g_pApp->m_myCharacter->m_bObserveMode = TRUE;
	g_ifMng->m_chatWin->Enable( false );

	
	for( int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if(g_ifMng->m_winControl[i]->State())
		{
			g_ifMng->m_winControl[i]->SetBeforeState(true);
			g_ifMng->m_winControl[i]->Enable(false);
		}		
	}

	
	g_ifMng->m_dlgWin->Enable(true);
	g_ifMng->m_observerMenuWin->Enable(true);
	g_ifMng->m_observerInfoWin->Enable(true);
	
}

void CGolryunBattle_Decoder::RecvTwoGyunTotalNak(void)
{	
	int totalNak = MSG_ReadLong();
	char szTemp[256];
	sprintf( szTemp, "Total Income Nak: %s", g_ifMng->GetNumberString( totalNak ) );
	g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), szTemp);

}

void CGolryunBattle_Decoder::RecvNoticeBattle(void)
{	
	char notice[256];

	strcpy(notice, MSG_ReadString());
	g_ifMng->AddNoticeMessage( notice, D3DCOLOR_ARGB(255, 55, 255, 55) );

}


void CGolryunBattle_Decoder::RecvStatusMatchNotice(void)
{	
	int noticeIdx = MSG_ReadShort();

	char szTemp[255];

	switch(noticeIdx)
	{
		case 1:	
			
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,361) );

			break;

		case 4:
		case 2:	
			
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,362) );

			break;
			
		case 3:	
			
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,363) );

			break;

	}

	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 55, 255, 55) );

}

void CGolryunBattle_Decoder::RecvSyncRASAMatch(void)
{	
	g_pApp->m_myCharacter->curRA = MSG_ReadLong();
	g_pApp->m_myCharacter->curSA = MSG_ReadShort();
	g_pApp->m_myCharacter->calMaxRA = MSG_ReadLong();
	g_pApp->m_myCharacter->calMaxSA = MSG_ReadShort();

}

void CGolryunBattle_Decoder::RecvCheckJoinerNum(void)
{	
	int joinNum = MSG_ReadShort();
	char szTemp[256];
	
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,364), joinNum);
	g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), szTemp);

}

void CGolryunBattle_Decoder::RecvStartMatchNotice(void)
{
	g_ifMng->m_observerInfoWin->m_redFighterNumber = MSG_ReadShort();
	g_ifMng->m_observerInfoWin->m_blueFighterNumber = MSG_ReadShort();
}

void CGolryunBattle_Decoder::RecvEndMatchNotice(void)
{
	if(g_ifMng->m_tournamentWin->State() || g_ifMng->m_tournamentMiniWin->State()){
		g_GolryunBattle_Encoder.SendReNewDisplayTournament();		
	}

	if(g_ifMng->m_observerInfoWin->State()){
		g_ifMng->m_observerInfoWin->m_RemainTime = 0;
		g_ifMng->m_observerInfoWin->m_redFighterNumber = 0;
		g_ifMng->m_observerInfoWin->m_blueFighterNumber = 0;
	}
}

void CGolryunBattle_Decoder::RecvReNewBattleTicket(void)
{
	if(g_ifMng->m_battleTicketWin->State()){
		g_GolryunBattle_Encoder.SendReNewBattleTicket();		
	}
}

void CGolryunBattle_Decoder::RecvErrorCode()
{	
	BYTE eCode	= MSG_ReadByte();
	BYTE eType	= MSG_ReadByte();

	char szTemp[256];
	int level;

	int iTournamentIdx = g_GolryunBattle.GetTournamentIdx();

	switch(eType)
	{
			
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_1:
			level = g_golryunManagerTable[iTournamentIdx].levelMin;
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,147), level );
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), szTemp );
			break;
			
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_2:
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,36));
			break;
			
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_3:
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,366) );
			break;
			
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_4:
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,367) );
			break;
			
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_5:
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,368) );
			break;
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_6:
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,369) );
			break;
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_7:
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,345) );
			break;
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_8:
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,49));
			break;
	}

}

void CGolryunBattle_Decoder::RecvLocalUserNotice()
{

	int lpackIdx = MSG_ReadShort();

	char szTemp[255];

	int iTournamentIdx = g_GolryunBattle.GetTournamentIdx();

	switch(lpackIdx)
	{
		
		case 151:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,151), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 152:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,152), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 158:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,158), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;
		
		case 159:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,159), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 160:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,160), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 161:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,161), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 162:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,162), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 209:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,209), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 210:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,210), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 163:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,163), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 164:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,164), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 166:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,166), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 167:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,167));
			break;

		
		case 169:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,169), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;

		
		case 370:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,370), g_golryunManagerTable[iTournamentIdx].levelMin );
			break;
	}

	g_ifMng->AddNoticeMessage( szTemp, D3DCOLOR_ARGB(255, 55, 255, 55) );
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 55, 255, 55) );

}

void CGolryunBattle_Decoder::RecvAllUserData()
{

	int iTournamentIdx = MSG_ReadShort();
	g_GolryunBattle.SetTournamentIdx(iTournamentIdx);

}

void CGolryunBattle_Decoder::RecvTransGolryun()
{
	int iTournamentIdx = MSG_ReadShort();
	g_GolryunBattle.SetTournamentIdx(iTournamentIdx);
	
	int iBattleIdx = MSG_ReadShort();
	g_GolryunBattle.SetBattleIdx(iBattleIdx);
	
	




	

	g_GolryunBattle.ProcessTransGolryunBattle();
}

void CGolryunBattle_Decoder::RecvTransArayan()
{
	
	g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,373), 
		MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_TRANSPORT_ARAYAN_ACCEPT );
}

void CGolryunBattle_Decoder::RecvSuggestBattle()
{
	char szTemp[256];
	int level, deposit;

	BYTE eType	= MSG_ReadByte();

	int iBattleIdx = MSG_ReadShort();
	g_GolryunBattle.SetBattleIdx(iBattleIdx);

	int iTournamentIdx = MSG_ReadShort();
	g_GolryunBattle.SetTournamentIdx(iTournamentIdx);

	switch(eType)
	{
	case 0:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,374));		
		break;
	case 1:
		
		level = g_golryunManagerTable[iTournamentIdx].levelMin;
		deposit = g_golryunManagerTable[iTournamentIdx].deposit;
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,149), level, deposit ); 
		g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), szTemp, 
			MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GB_BATTLE_SUGGEST_ACCEPT );
		break;
	case 2:
		
		level = g_golryunManagerTable[iTournamentIdx].levelMin;
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,147), level );
		g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), szTemp );
		break;
	}	

	g_ifMng->m_chatWin->Enable( false );
}

void CGolryunBattle_Decoder::RecvOpenBattleTicket()
{
	GTH_UnlockSending();
	
	BYTE eType	= MSG_ReadByte();

	if(eType)
	{
		
		CGolryunBattle::tagTournament* pTournament = g_GolryunBattle.GetTournamentPointer();
		pTournament->m_iTournamentGUID	= MSG_ReadShort();
		
		int iBattleIdx = MSG_ReadShort();
		g_GolryunBattle.SetBattleIdx(iBattleIdx);
		
		int iTournamentIdx = MSG_ReadShort();
		g_GolryunBattle.SetTournamentIdx(iTournamentIdx);
		
		g_ifMng->m_battleTicketWin->m_JoinerCnt = MSG_ReadShort();
		if(g_ifMng->m_battleTicketWin->m_JoinerCnt > CGolryunBattle::MAX_NUM_FIGHTER_PER_TOURNAMENT) 
			g_ifMng->m_battleTicketWin->m_JoinerCnt = CGolryunBattle::MAX_NUM_FIGHTER_PER_TOURNAMENT;
		
		for(int i=0; i<g_ifMng->m_battleTicketWin->m_JoinerCnt; i++)
		{
			g_ifMng->m_battleTicketWin->m_ticketList[i].num = MSG_ReadShort();
			g_ifMng->m_battleTicketWin->m_ticketList[i].job = MSG_ReadByte();
			g_ifMng->m_battleTicketWin->m_ticketList[i].win = MSG_ReadShort();
			g_ifMng->m_battleTicketWin->m_ticketList[i].lose = MSG_ReadShort();
			g_ifMng->m_battleTicketWin->m_ticketList[i].shareRate = MSG_ReadFloat(); 

		}

		g_ifMng->m_battleTicketWin->m_winNumber = 0;
		g_ifMng->m_battleTicketWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_battleTicketWin );
	}
	else
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,375));	
	}

	g_ifMng->m_chatWin->Enable( false );
	
}

void CGolryunBattle_Decoder::RecvBuyBattleTicket()
{
	g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong(); 	
}

void CGolryunBattle_Decoder::RecvShareRate()
{
	for(int i=0; i<g_ifMng->m_battleTicketWin->m_JoinerCnt; i++)
	{
		g_ifMng->m_battleTicketWin->m_ticketList[i].shareRate = MSG_ReadFloat(); 
	}	
}

void CGolryunBattle_Decoder::RecvOpenShareWin()
{
	GTH_UnlockSending();

	int iBattleIdx = MSG_ReadShort();
	g_GolryunBattle.SetBattleIdx(iBattleIdx);

	CGolryunBattle::tagBattle* pBattle = g_GolryunBattle.GetBattleCurrentPointer();
	for(int i=0; i<CGolryunBattle::MAX_NUM_TOURNAMENT_PER_BATTLE; i++)
	{	
		pBattle->Tournament[i].bComplete = MSG_ReadByte();
		pBattle->Tournament[i].m_iTournamentGUID = MSG_ReadShort(); 
		pBattle->Tournament[i].FirstWinner.iFightNum = MSG_ReadShort();
		pBattle->Tournament[i].m_ShareRate = MSG_ReadFloat();
	}

	int recordIdx=0;
	if(iBattleIdx>CGolryunBattle::MAX_NUM_BATTLE_RECORD) recordIdx = CGolryunBattle::MAX_NUM_BATTLE_RECORD;
	else recordIdx = iBattleIdx-1;

	for(int idx=0; idx<recordIdx; idx++)
	{
		CGolryunBattle::tagBattle* pBattleRecord = g_GolryunBattle.GetBattleRecordPointer(idx);
		for(int i=0; i<CGolryunBattle::MAX_NUM_TOURNAMENT_PER_BATTLE; i++)
		{		
			pBattleRecord->m_iBattleGUID = MSG_ReadShort(); 
			pBattleRecord->Tournament[i].bComplete = MSG_ReadByte(); 
			pBattleRecord->Tournament[i].m_iTournamentGUID = MSG_ReadShort(); 
			pBattleRecord->Tournament[i].FirstWinner.iFightNum = MSG_ReadShort();
			pBattleRecord->Tournament[i].m_ShareRate = MSG_ReadFloat();
		}
	}

	g_ifMng->m_shareWin->m_iBattleCnt = iBattleIdx;
	g_ifMng->m_shareWin->InitShareInventory();

	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_shareWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_shareWin );
	
}

void CGolryunBattle_Decoder::RecvDisplayTournament()
{
	GTH_UnlockSending();
	
	int i,j;

	int iTournamentIdx = MSG_ReadShort();
	g_GolryunBattle.SetTournamentIdx(iTournamentIdx);
	
	CGolryunBattle::tagBattle* pBattle=NULL; 
	pBattle = g_GolryunBattle.GetBattleCurrentPointer();
	for(i=0; i<CGolryunBattle::MAX_NUM_TOURNAMENT_PER_BATTLE; i++)
	{
		pBattle->Tournament[i].bComplete = MSG_ReadByte();
		pBattle->Tournament[i].FirstWinner.iFightNum = MSG_ReadShort();
	}

	CGolryunBattle::tagMatch* pMatch=NULL;
	for (i=0;i<CGolryunBattle::MAX_NUM_MATCH_PER_TOURNAMENT;i++)
	{
		pMatch = g_GolryunBattle.GetMatchPointer(i);
		pMatch->bComplete          = MSG_ReadByte();
		for (j=0;j<2;j++)
		{
			
			pMatch->Fighter[j].iFightNum = MSG_ReadShort();
			pMatch->Fighter[j].bWin   = MSG_ReadByte();			
		}
	}

	CGolryunBattle::tagTournament* pTournament=NULL;
	pTournament = g_GolryunBattle.GetTournamentPointer();
	pTournament->FirstWinner.iFightNum = MSG_ReadShort();
	
	g_ifMng->m_chatWin->Enable( false );

	if(g_pApp->m_myCharacter->m_bObserveMode)
	{
		g_ifMng->m_tournamentMiniWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_tournamentMiniWin );
	}
	else
	{
		g_ifMng->m_tournamentWin->Enable( true );
		g_ifMng->SetFocus( g_ifMng->m_tournamentWin );
	}
}

void CGolryunBattle_Decoder::RecvCheckPrize()
{
	CGolryunBattle::tagBattle* pBattle=NULL; 
	pBattle = g_GolryunBattle.GetBattleCurrentPointer();
	for(int i=0; i<CGolryunBattle::MAX_NUM_PRIZE_LIST_PER_BATTLE; i++)
	{
		pBattle->m_PrizeNumber[i] = MSG_ReadShort();

	}
	
	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_prizeWin->Enable( true );
	g_ifMng->SetFocus( g_ifMng->m_prizeWin );
	
}

void CGolryunBattle_Decoder::RecvEffectCount()
{
	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
	myChar->angles[YAW] = MSG_ReadShort();
	g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_START_COUNT);

	g_input->MouseEnable(false);
	g_input->KeyEnable(false);

	g_ifMng->m_tournamentWin->Enable(false);
	g_ifMng->m_battleTicketWin->Enable(false);
	g_ifMng->m_shareWin->Enable(false);
}

void CGolryunBattle_Decoder::RecvMatchProgressTime()
{
	g_ifMng->m_golryunInfoWin->m_RemainTime = MSG_ReadLong();
}


void CGolryunBattle_Decoder::RecvStartMatch()
{

	g_ifMng->m_golryunInfoWin->m_RemainTime = MSG_ReadLong();
	g_ifMng->m_golryunInfoWin->Enable(true);

	g_ifMng->m_miniMapWin->Enable(false);
	g_ifMng->SetFocus( g_ifMng->m_golryunInfoWin );


	g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_PROGRESS_MATCH);

}

void CGolryunBattle_Decoder::RecvEndMatch()
{
	g_pApp->m_myCharacter->pvpMode = false;
	g_musicMng->ChangeBGMusic( g_pApp->m_mapInfos[ 0 ].BGMusic );
	
	g_ifMng->m_golryunInfoWin->m_RemainTime=0;
	g_ifMng->m_golryunInfoWin->Enable(false);
	g_ifMng->m_miniMapWin->Enable(true);
	g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_END_MATCH);

}

void CGolryunBattle_Decoder::RecvCancelTournament()
{
	g_ifMng->m_golryunInfoWin->m_RemainTime=0;
	g_ifMng->m_golryunInfoWin->Enable(false);
	g_ifMng->m_miniMapWin->Enable(true);

	g_pApp->m_myCharacter->m_bWaitingRoom = FALSE;
	g_pApp->m_myCharacter->pvpMode = false;

	g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_CANCEL_TOURNAMENT);

}

void CGolryunBattle_Decoder::RecvJoinNumber()
{
	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
	myChar->m_JoinNum = MSG_ReadShort();
	
	g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_START_BETTING);
}


void CGolryunBattle_Decoder::RecvEffectResult()
{
	int index = MSG_ReadShort();

	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
	myChar->angles[YAW] = 90;
	VectorCopy( g_camera.m_angles, myChar->angles );
	g_camera.m_angles[YAW] = myChar->angles[YAW] + 180.0f;


	int effIdx  = g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
							index, 
							myChar->position,
							NULL, 
							true, 
							myChar->entityType, 
							myChar->idxOnServer,
							0.0f, 
							0.0f, 
							0.0f, 
							-1.0f,
							false );

	switch(index)
	{
	case 66:	
		g_musicMng->PlaySample3D( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_SUCCESS ), GTH_SOUND_PLAY_ONCE , myChar->position );

		break;
	case 67:	
		g_musicMng->PlaySample3D( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_FAIL ), GTH_SOUND_PLAY_ONCE , myChar->position );

		break;
	case 68:	
		g_musicMng->PlaySample3D( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_CONG ), GTH_SOUND_PLAY_ONCE , myChar->position );

		break;
	}
}









