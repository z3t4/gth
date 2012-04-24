





#include "..\global.h"
#include "tagGolryunBattle.h"



#include "CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;

#include "CGolryunBattle_Data.h"
#include "CGolryunBattle_Process.h"
#include "CGolryunBattle_Encoder.h"
#include "CGolryunBattle_Common.h"


#include "..\Tools\CTools.h"
extern CTools* gcpTools;





CGolryunBattle_Encoder::CGolryunBattle_Encoder(CGolryunBattle_Data* cpData)
{
	m_bOK=FALSE;
	m_cpData = cpData;
	m_bOK=TRUE;

}




CGolryunBattle_Encoder::~CGolryunBattle_Encoder()
{

}





BOOL CGolryunBattle_Encoder::isOK(void) const
{
	return m_bOK;
}



void CGolryunBattle_Encoder::
SendErrorCode(i3socket_t* sock,
			  const BYTE eCode,
			  enum tagGolryunBattlePacket_GB_ERROR_CODE::enumCode eType) const										   
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_ERROR_CODE);
	MSG_WriteByte(&netMessage, eCode);
	MSG_WriteByte(&netMessage, eType);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendTrasportGolryun(i3socket_t* sock)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_TRANS_GOLRYUN);
	MSG_WriteShort(&netMessage, (gcpGolryunBattle->GetProcessClassPointer())->m_iTournamentIdx);
	MSG_WriteShort(&netMessage, m_cpData->m_iBattleGUID);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendTrasportArayan(i3socket_t* sock)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_TRANS_ARAYAN);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendSuggestBattle(i3socket_t* sock, BYTE eType)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_SUGGEST_BATTLE);
	MSG_WriteByte(&netMessage, eType);
	MSG_WriteShort(&netMessage, m_cpData->m_iBattleGUID);
	MSG_WriteShort(&netMessage, m_cpProcess->m_iTournamentIdx);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendOpenBattleTicket(i3socket_t* sock, BYTE eType)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_OPEN_BATTLE_TICKET);

	MSG_WriteByte(&netMessage, eType);
	
	if(eType)
	{
		MSG_WriteShort(&netMessage, m_cpProcess->m_iTournamentGUID);
		MSG_WriteShort(&netMessage, m_cpData->m_iBattleGUID);
		MSG_WriteShort(&netMessage, m_cpProcess->m_iTournamentIdx);

		
		if(m_cpProcess->m_Ticket.iFighterNum>tagGolryunBattle::Max_Num_Fighter_Per_Tournament)
		{
			m_cpProcess->m_Ticket.iFighterNum = tagGolryunBattle::Max_Num_Fighter_Per_Tournament;
		}

		MSG_WriteShort(&netMessage, m_cpProcess->m_Ticket.iFighterNum);

		struct CGolryunBattle_Process::tagFightPlayerInfo* pTempFighter=NULL;
		for(int idx=0; idx < m_cpProcess->m_Ticket.iFighterNum; idx++){
			pTempFighter = m_cpProcess->GetFighterPointer(idx);
			if(NULL == pTempFighter) continue;
			
			MSG_WriteShort(&netMessage, idx+1);
			MSG_WriteByte(&netMessage,  pTempFighter->Job);			
			MSG_WriteShort(&netMessage, pTempFighter->Record.win);
			MSG_WriteShort(&netMessage, pTempFighter->Record.lose); 
			if(m_cpProcess->m_Ticket.Fighter[idx].fShareRate < 1.1f)	
				m_cpProcess->m_Ticket.Fighter[idx].fShareRate = 1.1f;			
			MSG_WriteFloat(&netMessage, m_cpProcess->m_Ticket.Fighter[idx].fShareRate);			
		}		
	}
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendOpenShareWin(i3socket_t* sock)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_OPEN_SHARE_WIN);

	MSG_WriteShort(&netMessage, m_cpData->m_iBattleGUID);

	
	for(int idx=0; idx<tagGolryunBattle::Max_Num_Tournament_Per_Battle; idx++)
	{
		MSG_WriteByte(&netMessage, m_cpData->m_Tournament[idx].bComplete);
		MSG_WriteShort(&netMessage, m_cpData->m_Tournament[idx].iTournamentGUID);
		MSG_WriteShort(&netMessage, m_cpData->m_Tournament[idx].FirstWinner.FightNum);
		MSG_WriteFloat(&netMessage, m_cpData->m_Tournament[idx].TwoGyun.fShareRate);
	}

	int record_strat_idx = m_cpData->m_iBattleGUID - tagGolryunBattle::Max_Num_Battle_Record;
	if(record_strat_idx<=0) record_strat_idx = 0;
	

	CGolryunBattle_DBV2* cpDBV2=gcpGolryunBattle->GetDBV2ClassPointer();
	struct CGolryunBattle_DBV2::tagBattle* pBattleRecord=NULL;

	for(int recordIdx = record_strat_idx; recordIdx<m_cpData->m_iBattleGUID; recordIdx++)
	{
		
		
		pBattleRecord=cpDBV2->GetBattleRecordPointer(recordIdx);
		if(NULL == pBattleRecord) continue;

		for(int idx=0; idx<tagGolryunBattle::Max_Num_Tournament_Per_Battle; idx++)
		{
			MSG_WriteShort(&netMessage, pBattleRecord->nGUID);
			MSG_WriteByte(&netMessage,  pBattleRecord->Tournament[idx].bComplete);
			MSG_WriteShort(&netMessage, pBattleRecord->Tournament[idx].nGUID);
			MSG_WriteShort(&netMessage, pBattleRecord->Tournament[idx].firstWinner.Num);
			MSG_WriteFloat(&netMessage, pBattleRecord->Tournament[idx].TwoGyun.Rate);
		}
	}

	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendBuyBattleTicket(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_BUY_BATTLE_TICKET);

	MSG_WriteLong(&netMessage, pPlayer->curChargeSE);
		
	NET_SendMessage( &pPlayer->sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendTwoGyunTotalNak(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_TWOGYUN_TOTAL_NAK);

	MSG_WriteLong(&netMessage, m_cpProcess->m_Ticket.iTotalNak);
		
	NET_SendMessage( &pPlayer->sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendPacketMatchTimePerSecond(playerCharacter_t* pPlayer, const int time)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_MATCH_PROGRESS_TIME);

	MSG_WriteLong(&netMessage, time);
		
	NET_SendMessage( &pPlayer->sock, &netMessage);

	MSG_EndWriting(&netMessage);
}



void CGolryunBattle_Encoder::SendShareRate(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_SHARE_RATE);
	
	for(int i=0; i< tagGolryunBattle::Max_Num_Fighter_Per_Tournament; i++)
	{
		if(m_cpProcess->m_Ticket.Fighter[i].fShareRate < 1.1f)
			m_cpProcess->m_Ticket.Fighter[i].fShareRate = 1.1f;
		
		MSG_WriteFloat(&netMessage, m_cpProcess->m_Ticket.Fighter[i].fShareRate);
	}
		
	NET_SendMessage( &pPlayer->sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendReNewShareRate(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_RENEW_BATTLE_TICKET);
		
	NET_SendMessage( &pPlayer->sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendDisplayTournament(i3socket_t* sock)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_DISPLAY_TOURNAMENT);

	MSG_WriteShort(&netMessage, (gcpGolryunBattle->GetProcessClassPointer())->m_iTournamentIdx);

	
	
	for(int TournamentIdx=0; TournamentIdx < tagGolryunBattle::Max_Num_Tournament_Per_Battle; TournamentIdx++)
	{
		MSG_WriteByte(&netMessage, m_cpData->m_Tournament[TournamentIdx].bComplete);
		MSG_WriteShort(&netMessage, m_cpData->m_Tournament[TournamentIdx].FirstWinner.FightNum);
	}
	

	struct CGolryunBattle_Data::tagMatch* pMatch=NULL;
	for(int MatchIdx=0; MatchIdx < tagGolryunBattle::Max_Num_Match_Per_Tournament; MatchIdx++)
	{		
		pMatch = m_cpProcess->GetMatchPointer(MatchIdx);
		MSG_WriteByte(&netMessage,pMatch->bComplete);
		MSG_WriteShort(&netMessage, pMatch->RedCornerFighter.FightNum);
		if(pMatch->WinnerCorner == CGolryunBattle_Data::tagMatch::enumCorner::Red) MSG_WriteByte(&netMessage,1);
		else MSG_WriteByte(&netMessage,0);

		MSG_WriteShort(&netMessage, pMatch->BlueCornerFighter.FightNum);
		if(pMatch->WinnerCorner == CGolryunBattle_Data::tagMatch::enumCorner::Blue) MSG_WriteByte(&netMessage,1);
		else MSG_WriteByte(&netMessage,0);
	}

	
	pMatch=m_cpProcess->GetMatchPointer(14);
	switch(pMatch->WinnerCorner){
		case CGolryunBattle_Data::tagMatch::enumCorner::Red:
			MSG_WriteShort(&netMessage, pMatch->RedCornerFighter.FightNum);	
			break;
		case CGolryunBattle_Data::tagMatch::enumCorner::Blue:
			MSG_WriteShort(&netMessage, pMatch->BlueCornerFighter.FightNum);	
			break;
		case CGolryunBattle_Data::tagMatch::enumCorner::None:
			MSG_WriteShort(&netMessage, -1);	
			break;
	}	

	NET_SendMessage( sock, &netMessage);
	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendCheckPrize(i3socket_t* sock)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_CHECK_PRIZE);

	for(int i=0; i<tagGolryunBattle::Max_Num_Prize_List_Per_Battle; i++)
	{
		MSG_WriteShort(&netMessage, m_cpData->m_EntranceTicket.LotteryGUIDList[i]);
	}
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}


void CGolryunBattle_Encoder::SendObserveStart(playerCharacter_t* pToPlayer,const BOOL bSuccess)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_OBSERVE_START);
	MSG_WriteByte(&netMessage, bSuccess);		

	NET_SendMessage( &(pToPlayer->sock), &netMessage);
	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendObserveMatchTime(playerCharacter_t* pToPlayer,const int time)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_OBSERVE_MATCH_TIME);
	MSG_WriteLong(&netMessage, time);

	NET_SendMessage( &(pToPlayer->sock), &netMessage);
	MSG_EndWriting(&netMessage);
}



void CGolryunBattle_Encoder::SendEffectCount(i3socket_t* sock, int angle)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_REQUEST_START_COUNT);
	MSG_WriteShort(&netMessage, angle);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendEffectResult(i3socket_t* sock, int index)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_EFFECT_RESULT);
	MSG_WriteShort(&netMessage, index);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendStartMatch(i3socket_t* sock, const int time)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_REQUEST_PROGRESS_MATCH);
	MSG_WriteLong(&netMessage, time);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::Send_GB_BATTLE_RASA_Sync(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_BATTLE_RASA_Sync);

	MSG_WriteLong(&netMessage,  (int)pPlayer->curRA);
	MSG_WriteShort(&netMessage, (int)pPlayer->curSA);
	MSG_WriteLong(&netMessage,  (int)pPlayer->calMaxRA);
	MSG_WriteShort(&netMessage, (int)pPlayer->calMaxSA);

	NET_SendMessage(&pPlayer->sock, &netMessage);

	MSG_EndWriting(&netMessage);
}



void CGolryunBattle_Encoder::SendEndMatchNotice(void)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_END_MATCH_NOTICE);

	playerCharacter_t* pTempPlayer=NULL;
	for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++ ){
		pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
		if(NULL == pTempPlayer) continue;
		if((pTempPlayer->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index) &&	(pTempPlayer->worldIdx != 0)) continue;




		NET_SendMessage(&pTempPlayer->sock, &netMessage);		
	}	

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendStartMatchNotice(playerCharacter_t* pPlayer, int redNum, int blueNum)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_START_MATCH_NOTICE);
	MSG_WriteShort(&netMessage, redNum);
	MSG_WriteShort(&netMessage, blueNum);

	NET_SendMessage(&pPlayer->sock, &netMessage);		
	
	MSG_EndWriting(&netMessage);
}



void CGolryunBattle_Encoder::
SendEndMatch(i3socket_t* sock,enum tagGolryunBattlePacket_GB_REQUEST_END_MATCH::enumCode code)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_REQUEST_END_MATCH);

		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendCancelTournament(i3socket_t* sock)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_REQUEST_CANCEL_TOURNAMENT);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendJoinNumber(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_REQUEST_START_BETTING);
	MSG_WriteShort(&netMessage, pPlayer->GonryunBattle.m_iJoinNum);
	
	NET_SendMessage( &pPlayer->sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendCheckJoinerNum(i3socket_t* sock, int num)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_CHECK_JOINER_NUM);
	MSG_WriteShort(&netMessage, num);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendStatusMatchNotice(i3socket_t* sock, int noticeIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_STATUS_MATCH_NOTICE);
	MSG_WriteShort(&netMessage, noticeIdx);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendNoticeBattle(i3socket_t* sock, char* notice)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_NOTICE_BATTLE);
	MSG_WriteString(&netMessage, notice);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}


void CGolryunBattle_Encoder::
SendObserveEnd(playerCharacter_t* pToPlayer,const BOOL bSuccess)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_OBSERVE_END);		
	MSG_WriteByte(&netMessage, bSuccess);
	NET_SendMessage( &pToPlayer->sock, &netMessage);
	MSG_EndWriting(&netMessage);
}


void CGolryunBattle_Encoder::SendLocalUserNotice(i3socket_t* sock, int lpackIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_LOCAL_USER_NOTICE);
	MSG_WriteShort(&netMessage, lpackIdx);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::SendAllUserData(i3socket_t* sock)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_ALL_USER_DATA);
	MSG_WriteShort(&netMessage, (gcpGolryunBattle->GetProcessClassPointer())->m_iTournamentIdx);
		
	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CGolryunBattle_Encoder::ManagerNoticeBattle(char* notice)
{
	playerCharacter_t* pTempPlayer=NULL;
	for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++)
	{		
		pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
		if(NULL == pTempPlayer) continue;
		if((pTempPlayer->worldIdx != 21) &&	(pTempPlayer->worldIdx != 0)) continue;
		SendNoticeBattle(&pTempPlayer->sock, notice);
	}
	ShowLogInfo("SendNoticeBattle()");
}


void CGolryunBattle_Encoder::ManagerLocalUserNotice(int lpackIdx)
{
	playerCharacter_t* pTempPlayer=NULL;
	for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++)
	{		
		pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
		if(NULL == pTempPlayer) continue;
		if((pTempPlayer->worldIdx != 21) &&	(pTempPlayer->worldIdx != 0)) continue;
		SendLocalUserNotice(&pTempPlayer->sock, lpackIdx);
	}
	ShowLogInfo("SendLocalUserNotice()");
}



void CGolryunBattle_Encoder::ManagerAllUserData()
{
	playerCharacter_t* pTempPlayer=NULL;
	for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++)
	{
		pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
		if(NULL == pTempPlayer) continue;		
		SendAllUserData(&pTempPlayer->sock);
	}

	ShowLogInfo("SendAllUserData()");
}


void CGolryunBattle_Encoder::SetProcessClassPointer(CGolryunBattle_Process* cpProcess)
{
	m_cpProcess = cpProcess;
}


void CGolryunBattle_Encoder::SendMessageToGM(const int LpackIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_GB_BATTLE);	
	MSG_WriteByte(&netMessage, tagGolryunBattlePacket::GB_GM_NOTICE);
	MSG_WriteLong(&netMessage, LpackIdx);

	playerCharacter_t* pTempPlayer=NULL;
	for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++ ){
		pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
		if(NULL == pTempPlayer) continue;
		if((pTempPlayer->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index)) continue;
		if(!pTempPlayer->gameMaster) continue;

		NET_SendMessage( &pTempPlayer->sock, &netMessage);	
	}

	MSG_EndWriting(&netMessage);
}
