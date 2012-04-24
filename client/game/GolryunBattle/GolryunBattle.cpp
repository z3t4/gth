




#include "../global_def.h"
#include "GolryunBattle.h"
#include "GolryunBattle_Encoder.h"

CGolryunBattle g_GolryunBattle;														

CGolryunBattle::CGolryunBattle()
{
	m_iUpdateTime=0;
	m_iStartTime=0;
	m_iStartCnt=0;

	m_iTournamentIdx = 0;
	m_iBattleIdx=1;

	memset(m_OldPosition, 0, sizeof(vec3_t));
	memset(m_ObserverPos, 0, sizeof(vec3_t));

	m_Match         = new tagMatch[MAX_NUM_MATCH_PER_TOURNAMENT];
	m_Tournament    = new tagTournament;
	m_BattleCurrent = new tagBattle;
	m_BattleRecord  = new tagBattle[MAX_NUM_BATTLE_RECORD];

	InitMatch();
	InitTournament(m_Tournament);
	InitBattle(m_BattleCurrent);
	InitBattleRecord();

	SetStatus(tagGolryun_Packet_Status::GB_REQUEST_INIT_BATTLE);
	SetObserverMode(eObserverMode::general);
}

CGolryunBattle::~CGolryunBattle()
{
	delete[] m_Match;
	delete m_Tournament;
	delete m_BattleCurrent;
	delete[] m_BattleRecord;
}

void CGolryunBattle::InitMatch()
{
	int i,j;
	for (i=0;i<MAX_NUM_MATCH_PER_TOURNAMENT;i++)
	{
		m_Match[i].bComplete = false;
		for (j=0;j<2;j++)
		{
			m_Match[i].Fighter[j].iFightNum = -1;
			m_Match[i].Fighter[j].bWin = false;
		}
	}
}

int	CGolryunBattle::InitTournament(tagTournament* pTournament)
{

	pTournament->bComplete = false;
	pTournament->m_iTournamentGUID = 0;
	pTournament->m_ShareRate = 0.0f;

	pTournament->FirstWinner.iFightNum = -1;
	pTournament->FirstWinner.bWin = false;

	pTournament->SecondWinner.iFightNum = -1;
	pTournament->SecondWinner.bWin = false;
	
	return TRUE;
}

void CGolryunBattle::InitBattle(tagBattle* pBattle)
{
	pBattle->bComplete = false;
	pBattle->m_iBattleGUID = 0;
	memset(pBattle->m_PrizeNumber, 0, sizeof(int)*MAX_NUM_PRIZE_LIST_PER_BATTLE);

	int tournamentIdx;
	for(tournamentIdx=0; tournamentIdx<MAX_NUM_TOURNAMENT_PER_BATTLE; tournamentIdx++)
	{
		InitTournament(&pBattle->Tournament[tournamentIdx]);
	}
	
}

void CGolryunBattle::InitBattleRecord()
{
	int battIdx;
	for(battIdx=0; battIdx<MAX_NUM_BATTLE_RECORD; battIdx++)
	{
		InitBattle(&m_BattleRecord[battIdx]);
	}
}

void CGolryunBattle::SetStatus(int iStatus)
{
	m_iStatus = iStatus;
	m_iStartTime = m_iUpdateTime;
}

int CGolryunBattle::GetStatus()
{
	return m_iStatus;	
}

void CGolryunBattle::SendRequestStartBattle(int iManorIdx)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_START_BATTLE);
		MSG_WriteShort(&netMessage, iManorIdx);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendRequestStartJoin()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_START_JOIN);
		MSG_WriteShort(&netMessage, m_iTournamentIdx);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendRequestStartReCallFighter()
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_START_RECALL_FIGHTER);
		MSG_WriteShort(&netMessage, m_iTournamentIdx);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendRequestCloseJoin()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_CLOSE_JOIN);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendRequestStartBetting()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_START_BETTING);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}
  

void CGolryunBattle::SendRequestReadyTournament()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_READY_TOURNAMENT);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendRequestStartCount()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_START_COUNT);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendRequestPauseMatch()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_PAUSE_MATCH);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendRequestReStartMatch()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_RESTART_MATCH);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendRequestEndMatch()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_END_MATCH);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



void CGolryunBattle::SendRequestWaitTournament()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_WAIT_TOURNAMENT);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendRequestCloseBattle()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_CLOSE_BATTLE);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendRequestStartPrize()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_START_PRIZE);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendTransWatchingRoom()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_TRANS_WATCHINGROOM);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendTransWaitingRoom()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_TRANS_WAITINGROOM);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendTransBattleStadium()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_TRANS_BATTLESTADIUM);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendNoticeBattle(char* notice)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_NOTICE_BATTLE);
		MSG_WriteString(&netMessage, notice);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendWalkOverMatch(int joinNum)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_WALKOVER_MATCH);
		MSG_WriteShort(&netMessage, joinNum);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendCheckJoinerNum()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_CHECK_JOINER_NUM);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendRequestCancelTournament()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_CANCEL_TOURNAMENT);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendTwoGyunTotalNak()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_TWOGYUN_TOTAL_NAK);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendAcceptTransGolryun()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_ACCEPT_TRANS_GOLRYUN );

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendAcceptToBattle()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_ACCEPT_BATTLE );

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendBuyBattleTicket(int itemTableIdx, int number)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_BUY_BATTLE_TICKET );
		MSG_WriteShort(&netMessage, itemTableIdx );
		MSG_WriteShort(&netMessage, number );

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendReNewBattleTicket()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_RENEW_BATTLE_TICKET );

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendReNewDisplayTournament()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_RENEW_DISPLAY_TOURNAMENT );

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendCheckStartMatch()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_CHECK_START_MATCH);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::SendObserveEnd()
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_OBSERVE_END);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendOpenBattleTicket()
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_OPEN_BATTLE_TICKET);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendOpenShareWin()
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_OPEN_SHARE_WIN);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendDisplayTournament()
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_DISPLAY_TOURNAMENT);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



void CGolryunBattle::SendInsertJoinList(char* name)
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_INSERT_JOIN_LIST);
		MSG_WriteString(&netMessage, name);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle::SendAssignFighterNumber(char* name, int number)
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_ASSIGN_FIGHTER_NUMBER);
		MSG_WriteString(&netMessage, name );
		MSG_WriteLong(&netMessage, number );

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle::RecvGolryunBattle()			
{
	BYTE bCommand;

	bCommand = MSG_ReadByte();	
	switch (bCommand)
	{	

		case tagGolryun_Packet_Status::GB_REQUEST_START_COUNT:			RecvEffectCount();			break;			
		case tagGolryun_Packet_Status::GB_REQUEST_PROGRESS_MATCH:		RecvStartMatch();			break;		
		case tagGolryun_Packet_Status::GB_REQUEST_END_MATCH:			RecvEndMatch();				break;	
		case tagGolryun_Packet_Status::GB_REQUEST_CANCEL_TOURNAMENT:	RecvInitTournament();		break;		
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
		case tagGolryun_Packet_Status::GB_GM_NOTICE:					RecvPauseMatchNotice();		break;
	}
}

void CGolryunBattle::RecvPauseMatchNotice()
{

	int LpackIdx = MSG_ReadLong();
	if( !GTH_IsGameMaster( g_pApp->m_myCharacter ) ) return;
	
	g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
		g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GOLRYUN_BATTLE, LpackIdx ));

}

void CGolryunBattle::RecvObserveMatchTime()
{
	int remainTime = MSG_ReadLong();
	if(remainTime<0)	remainTime = 0;
	g_ifMng->m_observerInfoWin->m_RemainTime = remainTime;
}

void CGolryunBattle::RecvObserveEnd()
{	
	GTH_UnlockSending();	
	
	BOOL bSuccess = MSG_ReadByte();
	if(FALSE == bSuccess){
		g_pApp->m_bAppExit = TRUE;
		PostMessage(g_pApp->m_hWnd,WM_CLOSE,0,0);
		return;
	}

	SetObserverMode(eObserverMode::general);
	
	memcpy(g_pApp->m_myCharacter->position, m_OldPosition, sizeof(vec3_t));
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

void CGolryunBattle::RecvObserveStart()
{

	BOOL bSuccess = MSG_ReadByte();
	if(FALSE == bSuccess && FALSE == g_pApp->m_myCharacter->m_bWaitingRoom){
		g_pApp->m_bAppExit = TRUE;
		PostMessage(g_pApp->m_hWnd,WM_CLOSE,0,0);
		return;
	}
	
	SetObserverMode(eObserverMode::observe);
	g_camera.m_maxCamDist = -700.0f;

	
	memcpy(m_OldPosition, g_pApp->m_myCharacter->position, sizeof(vec3_t));
	g_pApp->m_myCharacter->position[0] = 2108;
	g_pApp->m_myCharacter->position[1] = -11771;
	g_pApp->m_myCharacter->position[2] = -300;
	

	
	m_ObserverPos[0] = 2108;
	m_ObserverPos[1] = -11771;
	m_ObserverPos[2] = 180;
	
	
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

void CGolryunBattle::RecvTwoGyunTotalNak(void)
{	
	int totalNak = MSG_ReadLong();
	char szTemp[256];
	sprintf( szTemp, "Total Income Nak: %s", g_ifMng->GetNumberString( totalNak ) );
	g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), szTemp);

}

void CGolryunBattle::RecvNoticeBattle(void)
{	
	char notice[256];

	strcpy(notice, MSG_ReadString());
	g_ifMng->AddNoticeMessage( notice, D3DCOLOR_ARGB(255, 55, 255, 55) );

}


void CGolryunBattle::RecvStatusMatchNotice(void)
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

void CGolryunBattle::RecvSyncRASAMatch(void)
{	
	g_pApp->m_myCharacter->curRA = MSG_ReadLong();
	g_pApp->m_myCharacter->curSA = MSG_ReadShort();
	g_pApp->m_myCharacter->calMaxRA = MSG_ReadLong();
	g_pApp->m_myCharacter->calMaxSA = MSG_ReadShort();

}

void CGolryunBattle::RecvCheckJoinerNum(void)
{	
	int joinNum = MSG_ReadShort();
	char szTemp[256];
	
	sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,364), joinNum);
	g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), szTemp);

}

void CGolryunBattle::RecvStartMatchNotice(void)
{
	g_ifMng->m_observerInfoWin->m_redFighterNumber = MSG_ReadShort();
	g_ifMng->m_observerInfoWin->m_blueFighterNumber = MSG_ReadShort();
}

void CGolryunBattle::RecvEndMatchNotice(void)
{
	if(g_ifMng->m_tournamentWin->State() || g_ifMng->m_tournamentMiniWin->State()){
		SendReNewDisplayTournament();		
	}

	if(g_ifMng->m_observerInfoWin->State()){
		g_ifMng->m_observerInfoWin->m_RemainTime = 0;
		g_ifMng->m_observerInfoWin->m_redFighterNumber = 0;
		g_ifMng->m_observerInfoWin->m_blueFighterNumber = 0;
	}
}

void CGolryunBattle::RecvReNewBattleTicket(void)
{
	if(g_ifMng->m_battleTicketWin->State()){
		SendReNewBattleTicket();		
	}
}

void CGolryunBattle::RecvErrorCode()
{	
	BYTE eCode	= MSG_ReadByte();
	BYTE eType	= MSG_ReadByte();

	char szTemp[256];
	int level;

	switch(eType)
	{
			
		case tagGolryun_Packet_Error_Code::GB_ERROR_CODE_1:
			level = g_golryunManagerTable[m_iTournamentIdx].levelMin;
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

void CGolryunBattle::RecvLocalUserNotice()
{

	int lpackIdx = MSG_ReadShort();

	char szTemp[255];

	switch(lpackIdx)
	{
		
		case 151:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,151), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 152:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,152), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 158:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,158), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;
		
		case 159:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,159), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 160:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,160), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 161:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,161), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 162:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,162), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 209:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,209), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 210:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,210), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 163:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,163), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 164:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,164), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 166:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,166), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 167:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,167));
			break;

		
		case 169:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,169), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;

		
		case 370:		
			sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,370), g_golryunManagerTable[m_iTournamentIdx].levelMin );
			break;
	}

	g_ifMng->AddNoticeMessage( szTemp, D3DCOLOR_ARGB(255, 55, 255, 55) );
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 55, 255, 55) );

}

void CGolryunBattle::RecvAllUserData()
{

	m_iTournamentIdx = MSG_ReadShort();

}

void CGolryunBattle::RecvTransGolryun()
{
	m_iTournamentIdx = MSG_ReadShort();
	m_iBattleIdx = MSG_ReadShort();
	
	




	

	ProcessTransGolryunBattle();
}

void CGolryunBattle::RecvTransArayan()
{
	
	g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,373), 
		MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_TRANSPORT_ARAYAN_ACCEPT );
}

void CGolryunBattle::RecvSuggestBattle()
{
	char szTemp[256];
	int level, deposit;

	BYTE eType	= MSG_ReadByte();
	m_iBattleIdx		= MSG_ReadShort();
	m_iTournamentIdx	= MSG_ReadShort();

	switch(eType)
	{
	case 0:
		
		g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,374));		
		break;
	case 1:
		
		level = g_golryunManagerTable[m_iTournamentIdx].levelMin;
		deposit = g_golryunManagerTable[m_iTournamentIdx].deposit;
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,149), level, deposit ); 
		g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), szTemp, 
			MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GB_BATTLE_SUGGEST_ACCEPT );
		break;
	case 2:
		
		level = g_golryunManagerTable[m_iTournamentIdx].levelMin;
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,147), level );
		g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), szTemp );
		break;
	}	

	g_ifMng->m_chatWin->Enable( false );
}

void CGolryunBattle::RecvOpenBattleTicket()
{
	GTH_UnlockSending();
	
	BYTE eType	= MSG_ReadByte();

	if(eType)
	{
		
		m_Tournament->m_iTournamentGUID = MSG_ReadShort();
		m_iBattleIdx					= MSG_ReadShort();
		m_iTournamentIdx				= MSG_ReadShort();
		
		g_ifMng->m_battleTicketWin->m_JoinerCnt = MSG_ReadShort();

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

void CGolryunBattle::RecvBuyBattleTicket()
{
	g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong(); 	
}

void CGolryunBattle::RecvShareRate()
{
	for(int i=0; i<g_ifMng->m_battleTicketWin->m_JoinerCnt; i++)
	{
		g_ifMng->m_battleTicketWin->m_ticketList[i].shareRate = MSG_ReadFloat(); 
	}	
}

void CGolryunBattle::RecvOpenShareWin()
{
	GTH_UnlockSending();

	m_iBattleIdx = MSG_ReadShort();

	for(int i=0; i<MAX_NUM_TOURNAMENT_PER_BATTLE; i++)
	{
		m_BattleCurrent->Tournament[i].bComplete = MSG_ReadByte();
		m_BattleCurrent->Tournament[i].m_iTournamentGUID = MSG_ReadShort(); 
		m_BattleCurrent->Tournament[i].FirstWinner.iFightNum = MSG_ReadShort();
		m_BattleCurrent->Tournament[i].m_ShareRate = MSG_ReadFloat();
	}

	int recordIdx=0;
	if(m_iBattleIdx>MAX_NUM_BATTLE_RECORD) recordIdx = MAX_NUM_BATTLE_RECORD;
	else recordIdx = m_iBattleIdx-1;

	for(int idx=0; idx<recordIdx; idx++)
	{
		for(int i=0; i<MAX_NUM_TOURNAMENT_PER_BATTLE; i++)
		{
			m_BattleRecord[idx].m_iBattleGUID = MSG_ReadShort(); 
			m_BattleRecord[idx].Tournament[i].bComplete = MSG_ReadByte(); 
			m_BattleRecord[idx].Tournament[i].m_iTournamentGUID = MSG_ReadShort(); 
			m_BattleRecord[idx].Tournament[i].FirstWinner.iFightNum = MSG_ReadShort();
			m_BattleRecord[idx].Tournament[i].m_ShareRate = MSG_ReadFloat();
		}
	}

	g_ifMng->m_shareWin->m_iBattleCnt = m_iBattleIdx;
	g_ifMng->m_shareWin->InitShareInventory();

	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_shareWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_shareWin );
	
}

void CGolryunBattle::RecvDisplayTournament()
{
	GTH_UnlockSending();
	
	int i,j;

	m_iTournamentIdx = MSG_ReadShort();
	for(i=0; i<MAX_NUM_TOURNAMENT_PER_BATTLE; i++)
	{
		m_BattleCurrent->Tournament[i].bComplete = MSG_ReadByte();
		m_BattleCurrent->Tournament[i].FirstWinner.iFightNum = MSG_ReadShort();
	}

	for (i=0;i<MAX_NUM_MATCH_PER_TOURNAMENT;i++)
	{
		m_Match[i].bComplete          = MSG_ReadByte();
		for (j=0;j<2;j++)
		{
			
			m_Match[i].Fighter[j].iFightNum = MSG_ReadShort();
			m_Match[i].Fighter[j].bWin   = MSG_ReadByte();			
		}
	}

	m_Tournament->FirstWinner.iFightNum = MSG_ReadShort();
	
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

void CGolryunBattle::RecvCheckPrize()
{
	for(int i=0; i<MAX_NUM_PRIZE_LIST_PER_BATTLE; i++)
	{
		m_BattleCurrent->m_PrizeNumber[i] = MSG_ReadShort();

	}
	
	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_prizeWin->Enable( true );
	g_ifMng->SetFocus( g_ifMng->m_prizeWin );
	
}

void CGolryunBattle::RecvEffectCount()
{
	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
	myChar->angles[YAW] = MSG_ReadShort();
	SetStatus(tagGolryun_Packet_Status::GB_REQUEST_START_COUNT);

	g_input->MouseEnable(false);
	g_input->KeyEnable(false);
}

void CGolryunBattle::RecvMatchProgressTime()
{
	g_ifMng->m_golryunInfoWin->m_RemainTime = MSG_ReadLong();
}

void CGolryunBattle::RecvStartMatch()
{

	g_ifMng->m_golryunInfoWin->m_RemainTime = MSG_ReadLong();
	g_ifMng->m_golryunInfoWin->Enable(true);

	g_ifMng->m_miniMapWin->Enable(false);
	g_ifMng->SetFocus( g_ifMng->m_golryunInfoWin );


	SetStatus(tagGolryun_Packet_Status::GB_REQUEST_PROGRESS_MATCH);

}

void CGolryunBattle::RecvEndMatch()
{

	g_musicMng->ChangeBGMusic( g_pApp->m_mapInfos[ 0 ].BGMusic );
	
	g_ifMng->m_golryunInfoWin->m_RemainTime=0;
	g_ifMng->m_golryunInfoWin->Enable(false);
	g_ifMng->m_miniMapWin->Enable(true);
	SetStatus(tagGolryun_Packet_Status::GB_REQUEST_END_MATCH);

}

void CGolryunBattle::RecvInitTournament()
{
	g_ifMng->m_golryunInfoWin->m_RemainTime=0;
	g_ifMng->m_golryunInfoWin->Enable(false);
	g_ifMng->m_miniMapWin->Enable(true);

	g_pApp->m_myCharacter->m_bWaitingRoom = FALSE;

	SetStatus(tagGolryun_Packet_Status::GB_REQUEST_CANCEL_TOURNAMENT);

}

void CGolryunBattle::RecvJoinNumber()
{
	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
	myChar->m_JoinNum = MSG_ReadShort();
	
	SetStatus(tagGolryun_Packet_Status::GB_REQUEST_START_BETTING);
}

void CGolryunBattle::RecvEffectResult()
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


void CGolryunBattle::ProcessEffectCount()
{
	if(GetStatus() != tagGolryun_Packet_Status::GB_REQUEST_START_COUNT)	return;

	if(g_camera.m_angles[YAW]++>360) g_camera.m_angles[YAW] = 0;
	g_camera.m_distance = -300;
	g_camera.m_angles[PITCH] = 30;

	m_iUpdateTime = (unsigned int)g_timer.GetAppMilliTime();	
	DWORD remainTime = m_iUpdateTime-m_iStartTime;
	if(remainTime < 1000)
		return;

	m_iStartTime = m_iUpdateTime;

	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;
	
	int effIdx  = g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
							54+m_iStartCnt, 
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


	if(m_iStartCnt++>=10)
	{
		myChar->pvpMode = true;
		VectorCopy( g_camera.m_angles, myChar->angles );
		g_camera.m_angles[PITCH] = 30;

		m_iStartCnt=0;
		SetStatus(tagGolryun_Packet_Status::GB_REQUEST_PROGRESS_MATCH);

		g_input->MouseEnable(true);
		g_input->KeyEnable(true);

		
		
		if(!g_golryunManagerTable[m_iTournamentIdx].bcontrol)
		{


			g_ifMng->m_golryunInfoWin->Enable(true);
			g_ifMng->m_miniMapWin->Enable(false);
			g_ifMng->SetFocus( g_ifMng->m_golryunInfoWin );


		}		
		

		g_musicMng->ChangeBGMusic( "../BGMusic/thefight.mp3" );


		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_READYFIGHT ), GTH_SOUND_PLAY_ONCE );
	}
	else
	{

		g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_COUNT ), GTH_SOUND_PLAY_ONCE );
	}

}

void CGolryunBattle::ProcessTransGolryunBattle()
{

	


	if(TRUE == CheckInventoryMaxSize())
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL,49));
		return;
	}
	
	
	for( int i=0; i<MAX_INVENTORY_SIZE; i++ )
	{
		if( g_cgv.myCharacterInfo->inventory[i] < 0 ) continue;
		int itemIdx = g_cgv.myCharacterInfo->inventory[i];

		if(g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx == ENTERANCE_TICKET_ITEM_INDEX &&
		   g_cgv.myCharacterInfo->item[itemIdx].optionIdx[0] == m_iBattleIdx )
		{
			GTH_SendMessage_TransportWorld( CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX, 0 );	
			g_ifMng->m_chatWin->Enable( false );
			return;
		}
	}
	
	g_GolryunBattle_Encoder.SendAcceptTransGolryun();
	GTH_SendMessage_TransportWorld( CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX, 0 );	
	g_ifMng->m_chatWin->Enable( false );	
}

void CGolryunBattle::ProcessSuggestBattle()
{
	char szTemp[256];

	int level = g_cgv.myCharacterInfo->level;
	if(g_cgv.myCharacterInfo->curChargeSE < g_golryunManagerTable[m_iTournamentIdx].deposit)
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,150));
		return;
	}
	
	if(level<g_golryunManagerTable[m_iTournamentIdx].levelMin || 
	   level>g_golryunManagerTable[m_iTournamentIdx].levelMax)
	{
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,145), g_golryunManagerTable[m_iTournamentIdx].levelMin );
		g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), szTemp);
		return;
	}

	g_pApp->m_myCharacter->m_bWaitingRoom = TRUE;
	g_pApp->m_myCharacter->m_JoinNum=0;
	
	g_cgv.myCharacterInfo->curChargeSE -= g_golryunManagerTable[m_iTournamentIdx].deposit;
	
	g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,148), D3DCOLOR_ARGB(255, 55, 255, 55) );
	g_GolryunBattle_Encoder.SendAcceptToBattle();		
	g_ifMng->m_chatWin->Enable( false );	

	if(g_pApp->m_myCharacter->worldIdx != CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX)
		GTH_SendMessage_TransportWorld( CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX, 1 );

}


int CGolryunBattle::EncodeSerial(int time, int level, int winNum)
{
	TicketSerialInfo ticket;
	ticket.TIME=time;
	ticket.LEVEL=level;
	ticket.WINNUM=winNum;

	int Serial;
	memcpy(&Serial,&ticket,sizeof(TicketSerialInfo));
	return Serial;
}

TicketSerialInfo CGolryunBattle::DecodeSerial(int Serial)
{
	TicketSerialInfo ticket;
	memcpy(&ticket,&Serial,sizeof(TicketSerialInfo));
	return ticket;
}

struct CGolryunBattle::tagMatch* 
CGolryunBattle::GetMatchPointer(const int matchIdx)
{ 
	if(matchIdx<0)	return NULL;
	if(matchIdx>=MAX_NUM_MATCH_PER_TOURNAMENT) return NULL;
	return &m_Match[matchIdx]; 
}

struct CGolryunBattle::tagBattle* 
CGolryunBattle::FindBattleRecordPointer(const int battleIdx)
{ 
	struct tagBattle* pRecorder = NULL;
	for(int recordIdx=0; recordIdx<MAX_NUM_BATTLE_RECORD; recordIdx++)
	{
		pRecorder = &m_BattleRecord[recordIdx];
		if(pRecorder->m_iBattleGUID == battleIdx) return pRecorder;
	}

	return NULL; 
}

struct CGolryunBattle::tagBattle* 
CGolryunBattle::GetBattleRecordPointer(const int battleIdx)
{ 
	if(battleIdx<0)	return NULL;
	if(battleIdx>=MAX_NUM_BATTLE_RECORD) return NULL;
	return &m_BattleRecord[battleIdx];
}

void CGolryunBattle::SetManorGrade(Fx_CHARACTER_t *character )
{ 
	DWORD manorColor[] =
	{
		D3DCOLOR_ARGB( 255, 255, 255, 255 ),
		D3DCOLOR_ARGB( 255, 74, 239, 0 ),
		D3DCOLOR_ARGB( 255, 0, 218, 252 ),
		D3DCOLOR_ARGB( 255, 2, 84, 254 ),
		D3DCOLOR_ARGB( 255, 0, 0, 0 ),
	};

	int nameColorIdx = character->m_NameColorIdx;

	if(character->m_bWaitingRoom)
		g_chatManager->SetColor( manorColor[0] );
	else
		g_chatManager->SetColor( manorColor[nameColorIdx] );
}

BOOL CGolryunBattle::CheckInventoryMaxSize()
{
	for( int i=0; i<MAX_INVENTORY_SIZE; i++ )
	{
		if( g_cgv.myCharacterInfo->inventory[i] < 0 )
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CGolryunBattle::SetupObserver(void)
{
	vec3_t angle;

	memset(angle, 0, sizeof(vec3_t));

	g_camera.AdjustCameraAngle( m_ObserverPos , angle , g_timer.GetElapsedTime() );  
	g_camera.AdjustCamera( m_ObserverPos , angle );

	if	(	g_charManager ->m_tbDo	)
	{
		vec3_t	trembleDelta;
		trembleDelta [0]	=	sin ( g_timer .GetAppTime () * 70.0f ) * 5.0f * g_charManager ->m_tbPower;
		trembleDelta [1]	=	cos ( g_timer .GetAppTime () * 68.0f ) * 5.0f * g_charManager ->m_tbPower;
		trembleDelta [2]	=	sin ( g_timer .GetAppTime () * 66.0f ) * cos ( g_timer .GetAppTime () * 64.0f ) * 5.0f * g_charManager ->m_tbPower;

		VectorAdd (	g_camera.m_position,	g_camera.m_position,	trembleDelta	);
	}

	g_camera.SetupMatrices( true );
}

void CGolryunBattle::SetupReStartGame()
{
	SetObserverMode(eObserverMode::general);
	g_pApp->m_myCharacter->m_bObserveMode = FALSE;
	g_pApp->m_myCharacter->m_bWaitingRoom = FALSE;
	g_pApp->m_myCharacter->m_JoinNum = 0;
	g_camera.m_maxCamDist = -350.0f;
	g_camera.m_distance = -350.0f;
}






