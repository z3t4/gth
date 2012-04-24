




#include "../global_def.h"
#include "../game/GolryunBattle/GolryunBattle.h"
#include "../game/GolryunBattle/GolryunBattle_Encoder.h"

CGolryunBattle_Encoder g_GolryunBattle_Encoder;														

CGolryunBattle_Encoder::CGolryunBattle_Encoder()
{
	
}

CGolryunBattle_Encoder::~CGolryunBattle_Encoder()
{
	
}


void CGolryunBattle_Encoder::SendRequestStartBattle(int iManorIdx)
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

void CGolryunBattle_Encoder::SendRequestStartJoin(char* message)
{
	int iTournamentIdx = g_GolryunBattle.GetTournamentIdx();

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_START_JOIN);
		MSG_WriteShort(&netMessage, iTournamentIdx);
		MSG_WriteString(&netMessage, message);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle_Encoder::SendRequestCloseJoin()
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

void CGolryunBattle_Encoder::SendRequestStartBetting()
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
 


void CGolryunBattle_Encoder::SendRequestReadyTournament(int iRound)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_REQUEST_READY_TOURNAMENT);
		MSG_WriteShort(&netMessage, iRound );

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



void CGolryunBattle_Encoder::SendRequestStartCount()
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


void CGolryunBattle_Encoder::SendRequestPauseMatch()
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


void CGolryunBattle_Encoder::SendRequestReStartMatch()
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


void CGolryunBattle_Encoder::SendRequestEndMatch()
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



void CGolryunBattle_Encoder::SendRequestWaitTournament()
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

void CGolryunBattle_Encoder::SendRequestCloseBattle()
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


void CGolryunBattle_Encoder::SendRequestStartPrize()
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


void CGolryunBattle_Encoder::SendTransWatchingRoom()
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


void CGolryunBattle_Encoder::SendTransWaitingRoom()
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


void CGolryunBattle_Encoder::SendTransBattleStadium()
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


void CGolryunBattle_Encoder::SendNoticeBattle(char* notice)
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


void CGolryunBattle_Encoder::SendWalkOverMatch(int joinNum)
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


void CGolryunBattle_Encoder::SendCheckJoinerNum()
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


void CGolryunBattle_Encoder::SendRequestCancelTournament()
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


void CGolryunBattle_Encoder::SendTwoGyunTotalNak()
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

void CGolryunBattle_Encoder::SendAcceptTransGolryun()
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


void CGolryunBattle_Encoder::SendAcceptToBattle()
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


void CGolryunBattle_Encoder::SendBuyBattleTicket(int itemTableIdx, int number)
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

void CGolryunBattle_Encoder::SendReNewBattleTicket()
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

void CGolryunBattle_Encoder::SendReNewDisplayTournament()
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

void CGolryunBattle_Encoder::SendCheckStartMatch()
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

void CGolryunBattle_Encoder::SendObserveEnd()
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


void CGolryunBattle_Encoder::SendOpenBattleTicket()
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


void CGolryunBattle_Encoder::SendOpenShareWin()
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


void CGolryunBattle_Encoder::SendDisplayTournament()
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


void CGolryunBattle_Encoder::SendInsertJoinList(char* name)
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


void CGolryunBattle_Encoder::SendAssignFighterNumber(char* name, int number)
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


void CGolryunBattle_Encoder::SendRequestStartReCallFighter(char* message)
{
	if( !GTH_LockSending() ) return;
	
	int iTournamentIdx = g_GolryunBattle.GetTournamentIdx();

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_START_RECALL_FIGHTER);
		MSG_WriteShort(&netMessage, iTournamentIdx);
		MSG_WriteString(&netMessage, message);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGolryunBattle_Encoder::SendForceEject(char* name)
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_FORCE_EJECT);
		MSG_WriteString(&netMessage, name);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGolryunBattle_Encoder::SendCheckStatusPvPMode()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_GB_BATTLE );
		MSG_WriteByte(&netMessage, tagGolryun_Packet_Status::GB_CHECK_STATUS_PvPMODE);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}








