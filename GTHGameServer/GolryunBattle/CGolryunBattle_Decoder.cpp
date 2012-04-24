



#include "..\global.h"

#include <assert.h>
#include "CGolryunBattle_Decoder.h"
#include "CGolryunBattle_Common.h"

#include "..\Tools\CTools.h"
extern CTools* gcpTools;


#include "CGolryunBattle_Log.h"
extern CGolryunBattle_Log* gcpGolryunBattleLog;

#include "..\GolryunBattle\CGolryunBattle_Observer.h"
extern CGolryunBattle_Observer* gcpGolryunBattle_Observer;

#include "..\GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;




CGolryunBattle_Decoder::CGolryunBattle_Decoder(
	CGolryunBattle_DBV2*    cpDBV2,
	CGolryunBattle_Encoder* cpEncoder,
	CGolryunBattle_Process* cpProcess)
{
	m_bOK=FALSE;	

	m_cpDBV2 = cpDBV2;
	assert(NULL != m_cpDBV2);

	m_cpEncoder=cpEncoder;
	assert(NULL != m_cpEncoder);

	m_cpProcess = cpProcess;
	assert(NULL != cpProcess);	

	ShowLogInfo("GB Status : GB_REQUEST_INIT_BATTLE");
	
	m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::TournamentStep_INIT_BATTLE); 

	m_bOK=TRUE;

}




CGolryunBattle_Decoder::~CGolryunBattle_Decoder()
{

}




BOOL CGolryunBattle_Decoder::isOK(void) const
{
	return m_bOK;
}





void CGolryunBattle_Decoder::RecvWalkOverMatch()
{
	int iJoinNum = MSG_ReadShort();
	m_cpProcess->CheckMatchResult(iJoinNum);	
}

void CGolryunBattle_Decoder::RecvNoticeBattle()
{
	char notice[256];
	strcpy(notice, MSG_ReadString());

	m_cpEncoder->ManagerNoticeBattle(notice);
}

void CGolryunBattle_Decoder::RecvBattleManorIdx()
{
	int iManorIdx = MSG_ReadShort(); 
	if(iManorIdx < 0) iManorIdx=0;
	if(iManorIdx > 4) iManorIdx=4;

	switch(iManorIdx)
	{
	case 0:
		m_cpProcess->SetManorStatus(CGolryunBattle_Process::enumManor::normal);
		break;
	case 1:
		m_cpProcess->SetManorStatus(CGolryunBattle_Process::enumManor::week);
		break;
	case 2:
		m_cpProcess->SetManorStatus(CGolryunBattle_Process::enumManor::month);
		break;
	case 3:
		m_cpProcess->SetManorStatus(CGolryunBattle_Process::enumManor::ramification);
		break;
	case 4:
		m_cpProcess->SetManorStatus(CGolryunBattle_Process::enumManor::year);
		break;
	default:
		m_cpProcess->SetManorStatus(CGolryunBattle_Process::enumManor::normal);
		break;
	}
}


void CGolryunBattle_Decoder::RecvBattleTournamentIdx(const int in_nTournamentLevelIdx)
{
	

	m_cpProcess->m_iTournamentIdx = in_nTournamentLevelIdx;
	m_cpProcess->m_iTournamentGUID++;

	
	playerCharacter_t* pTempPlayer=NULL;
	for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++ )
	{
		pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
		if(NULL == pTempPlayer) continue;		
		m_cpEncoder->SendAllUserData(&pTempPlayer->sock);
	}
}




void CGolryunBattle_Decoder::RecvObserveEnd(playerCharacter_t* pPlayer)
{
	if(FALSE == pPlayer->GonryunBattle.Observer.bActive){
		m_cpEncoder->SendObserveEnd(pPlayer,FALSE);
		return;
	}

	if(FALSE == gcpGolryunBattle_Observer->Delete(pPlayer)){
		m_cpEncoder->SendObserveEnd(pPlayer,FALSE);
		return;
	}

	pPlayer->GonryunBattle.Observer.bActive=FALSE;
	m_cpEncoder->SendObserveEnd(pPlayer,TRUE);
}

void CGolryunBattle_Decoder::RecvOpenBattleTicket(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer) return;

	m_cpEncoder->SendOpenBattleTicket(&pPlayer->sock, 1);
	GTH_DeleteNoticePC(pPlayer);
}

void CGolryunBattle_Decoder::RecvOpenShareWin(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer) return;

	pPlayer->GonryunBattle.ShareWin.i64TotalNak=0;
	pPlayer->GonryunBattle.ShareWin.iFortuneStone=0;
	for(int idx = 0; idx < playerCharacter_t::tagGonryunBattle::tagShareWin::MAX_INVENTORY_NUMBER; idx ++ )
	{
		pPlayer->GonryunBattle.ShareWin.Inventory[idx] = -1;
	}

	
	GTH_AddNoticePC( pPlayer, -1, -1, BUSY_STATE_NPC_INVENTORY );
	m_cpEncoder->SendOpenShareWin(&pPlayer->sock);
	GTH_DeleteNoticePC(pPlayer);
}

void CGolryunBattle_Decoder::RecvDisplayTournament(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer) return;

	m_cpEncoder->SendDisplayTournament(&pPlayer->sock);
	GTH_DeleteNoticePC(pPlayer);

}


void CGolryunBattle_Decoder::RecvForceEject(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_FORCE_EJECT);
		return;
	}

	char TargetName[NAMESTRING+1]="";
	strncpy(TargetName,MSG_ReadString(),NAMESTRING);
	TargetName[NAMESTRING-1]=NULL;

	playerCharacter_t* pTargetPlayer= gcpTools->GetPlayerRecordPointer(TargetName);
	if((NULL == pTargetPlayer) ||
	   (tagGolryunBattle::Golryun_Battle_Map_Index != pTargetPlayer->worldIdx)){
		
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_FORCE_EJECT);
		return;
	}

	
	if(FALSE == gcpTools->isSummonAllow(pTargetPlayer)){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_FORCE_EJECT);
		return;
	}

	if(FALSE == m_cpProcess->ForceEjectToFighter(pTargetPlayer)){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_FORCE_EJECT);
	}else{	
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::SUCCESS_FORCE_EJECT);
	}
}


void CGolryunBattle_Decoder::RecvCheckStatusPvPMode(playerCharacter_t* pPlayer)
{
	struct CGolryunBattle_Data::tagMatch* pMatch=m_cpProcess->GetCurMatch();
	assert(NULL != pMatch);

	playerCharacter_t* pRedCornerPlayer=m_cpProcess->GetFighterPlayerPointer(&pMatch->RedCornerFighter);
	playerCharacter_t* pBlueCornerPlayer=m_cpProcess->GetFighterPlayerPointer(&pMatch->BlueCornerFighter);
	
	if(pRedCornerPlayer == NULL)	return;
	if(pBlueCornerPlayer == NULL)	return;

	if(pRedCornerPlayer->pvpMode && pBlueCornerPlayer->pvpMode)
	{
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::SUCCESS_STATUS_PvPMODE);
	}
	else
	{
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_STATUS_PvPMODE);

		gcpGolryunBattleLog->Write("공격상태확인 Red:%s PvPMode=%d, Blue:%s PvPMode=%d", 
									pRedCornerPlayer->name, 
									pRedCornerPlayer->pvpMode,
									pBlueCornerPlayer->name, 
									pBlueCornerPlayer->pvpMode);

		pRedCornerPlayer->pvpMode = true;
		pBlueCornerPlayer->pvpMode = true;
	}

}





inline void CGolryunBattle_Decoder::Decoder_GB_INSERT_JOIN_LIST(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_INSERT_JOINMAP);
		return;
	}

	char TargetName[NAMESTRING+1]="";
	strncpy(TargetName,MSG_ReadString(),NAMESTRING);
	TargetName[NAMESTRING-1]=NULL;

	playerCharacter_t* pTargetPlayer= gcpTools->GetPlayerRecordPointer(TargetName);
	if((NULL == pTargetPlayer) ||
	   (tagGolryunBattle::Golryun_Battle_Map_Index != pTargetPlayer->worldIdx)){
		
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_INSERT_JOINMAP);
		return;
	}

	if(FALSE == gcpTools->isSummonAllow(pTargetPlayer)){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_INSERT_JOINMAP);
		return;
	}
	
	if(FALSE == m_cpProcess->InsertJoinMap(pTargetPlayer)){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_INSERT_JOINMAP);
	}else{	
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::SUCCESS_INSERT_JOINMAP);
	}

}



inline void CGolryunBattle_Decoder::Decoder_GB_ASSIGN_FIGHTER_NUMBER(void)
{
	char TargetName[NAMESTRING+1]="";
	strncpy(TargetName,MSG_ReadString(),NAMESTRING);
	TargetName[NAMESTRING-1]=NULL;
	int FightNum = MSG_ReadLong();



	if(FALSE == m_cpProcess->AssignFightNum(TargetName,FightNum)){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_ASSIGN_FIGHTER_NUMBER);
		return;
	}


	m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::SUCCESS_ASSIGN_FIGHTER_NUMBER);
}





inline void CGolryunBattle_Decoder::Decoder_ProgressCommand_GB_REQUEST_START_JOIN(void)
{
	int nTournamentLevelIdx = MSG_ReadShort(); 
	if((nTournamentLevelIdx < 0) ||
	   (nTournamentLevelIdx >= tagGolryunBattle::Max_Num_Tournament_Per_Battle)){
		
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_OVERFLOW_TOURNAMNETIDX);
		return;
	}

	CGolryunBattle_Data* cpData=gcpGolryunBattle->GetDataClassPointer();
	if(NULL == cpData){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_UNKNOWN);
		return;
	}

	if(TRUE == cpData->m_Tournament[nTournamentLevelIdx].bComplete){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_PREVIOUS_PLAY_TOURNAMENT);
		return;
	}

	


	ShowLogInfo("GB Status : GB_REQUEST_START_JOIN");
	gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_START_JOIN");		
	m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_START_JOIN);
	m_cpProcess->InitTournament();
	RecvBattleTournamentIdx(nTournamentLevelIdx);

	GTH_SystemAdmin_Notice();
}



inline void CGolryunBattle_Decoder::Decoder_ProgressCommand_GB_START_RECALL_FIGHTER(void)
{
	int nTournamentLevelIdx = MSG_ReadShort(); 
	if((nTournamentLevelIdx < 0) ||
	   (nTournamentLevelIdx >= tagGolryunBattle::Max_Num_Tournament_Per_Battle)){
		
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_OVERFLOW_TOURNAMNETIDX);
		return;
	}

	CGolryunBattle_Data* cpData=gcpGolryunBattle->GetDataClassPointer();
	if(NULL == cpData){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_UNKNOWN);
		return;
	}

	if(TRUE == cpData->m_Tournament[nTournamentLevelIdx].bComplete){
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_PREVIOUS_PLAY_TOURNAMENT);
		return;
	}

	gcpGolryunBattleLog->Write("GB Status : GB_START_RECALL_FIGHTER");	 
	m_cpProcess->InitTournament();
	RecvBattleTournamentIdx(nTournamentLevelIdx);
	GTH_SystemAdmin_Notice();
}






inline void CGolryunBattle_Decoder::Decoder_ProgressCommand(const int in_Command)
{

	if(NULL != g_curPC){	
		if(0 == GTH_CheckValidation_GameMaster(g_curPC)) return;
	}

	if(FALSE == m_cpProcess->isReady()){		
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::NOTREADY);
		if(CGolryunBattle_DBV2::ERROR_NONE != m_cpDBV2->GetLastError()){
			gcpGolryunBattleLog->Write("can't play GolryunBattle becasue DBerror [%s]",m_cpDBV2->GetLastErrorStr());
			m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::ERROR_SQL);
			return;
		}
		return;
	}

	switch(in_Command){

	
	case tagGolryunBattlePacket::GB_INSERT_JOIN_LIST:
		Decoder_GB_INSERT_JOIN_LIST(g_curPC);
		break;

	
	case tagGolryunBattlePacket::GB_ASSIGN_FIGHTER_NUMBER:
		Decoder_GB_ASSIGN_FIGHTER_NUMBER();
		break;

	
	case tagGolryunBattlePacket::GB_PAUSE_MATCH:
		m_cpProcess->SetProgressMatchStatus(TRUE);
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::PAUSE_MATCH_RESERVED);
		break;

	
	case tagGolryunBattlePacket::GB_RESTART_MATCH:
	case tagGolryunBattlePacket::GB_REQUEST_START_COUNT:
		m_cpProcess->SetProgressMatchStatus(FALSE);
		m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::TournamentStep_START_COUNT); 
		m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::PAUSE_MATCH_REPLAY);
		break;

	
	case tagGolryunBattlePacket::GB_REQUEST_START_BATTLE :	
		ShowLogInfo("GB Status : GB_REQUEST_START_BATTLE");
		gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_START_BATTLE");

		
		m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::TournamentStep_START_BATTLE); 
		
		
		RecvBattleManorIdx();
		break;	

	
	case tagGolryunBattlePacket::GB_REQUEST_START_JOIN :	
		Decoder_ProgressCommand_GB_REQUEST_START_JOIN();
		break;	

	
	case tagGolryunBattlePacket::GB_START_RECALL_FIGHTER :	
		Decoder_ProgressCommand_GB_START_RECALL_FIGHTER();
		break;
	 
		

	
	case tagGolryunBattlePacket::GB_REQUEST_CLOSE_JOIN :	
		ShowLogInfo("GB Status : GB_REQUEST_CLOSE_JOIN");
		gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_CLOSE_JOIN");
		
		m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_CLOSE_JOIN);
	
		break;

	
	case tagGolryunBattlePacket::GB_REQUEST_START_BETTING :	
		ShowLogInfo("GB Status : GB_REQUEST_START_BETTING");
		gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_START_BETTING");
		
		
		m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_START_BETTING);
		m_cpProcess->ExtractJoinNumber();

		break;	

	
	case tagGolryunBattlePacket::GB_REQUEST_READY_TOURNAMENT :	
		ShowLogInfo("GB Status : GB_REQUEST_READY_TOURNAMENT");
		gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_READY_TOURNAMENT");

		
		m_cpEncoder->ManagerLocalUserNotice(152);	
		m_cpEncoder->ManagerLocalUserNotice(158);	
		m_cpProcess->ExtractMatchNumber();


		
		m_cpProcess->SetProgressMatchStatus(TRUE);
		m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_Match_Pause);

		
		
		break;

	
	case tagGolryunBattlePacket::GB_REQUEST_CLOSE_BATTLE : 	
		ShowLogInfo("GB Status : GB_REQUEST_CLOSE_BATTLE");
		gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_CLOSE_BATTLE");
		
		m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_CLOSE_BATTLE);
		m_cpProcess->SaveBattleRecoder();
		break;

	
	case tagGolryunBattlePacket::GB_REQUEST_START_PRIZE :
		ShowLogInfo("GB Status : GB_REQUEST_START_PRIZE");
		gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_START_PRIZE");
		
		m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_START_PRIZE);
		m_cpProcess->ExtractPrizeNumber();			
		break;

	
	case tagGolryunBattlePacket::GB_REQUEST_CANCEL_TOURNAMENT :
		ShowLogInfo("GB Status : GB_REQUEST_CANCEL_TOURNAMENT");
		gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_CANCEL_TOURNAMENT");

		
		m_cpProcess->SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_CANCEL_TOURNAMENT);
		m_cpProcess->InitTournament();
		m_cpEncoder->ManagerLocalUserNotice(370);	
		

		break;

	
	case tagGolryunBattlePacket::GB_WALKOVER_MATCH :
		ShowLogInfo("GB Status : GB_WALKOVER_MATCH");
		gcpGolryunBattleLog->Write("GB Status : GB_WALKOVER_MATCH");
		RecvWalkOverMatch();
		break;

	
	case tagGolryunBattlePacket::GB_TWOGYUN_TOTAL_NAK :
		ShowLogInfo("GB Status : GB_TWOGYUN_TOTAL_NAK");
		gcpGolryunBattleLog->Write("GB Status : GB_TWOGYUN_TOTAL_NAK");
		m_cpEncoder->SendTwoGyunTotalNak(g_curPC);
		break;

	
	case tagGolryunBattlePacket::GB_FORCE_EJECT :
		RecvForceEject(g_curPC);
		break;

	
	case tagGolryunBattlePacket::GB_CHECK_STATUS_PvPMODE :
		RecvCheckStatusPvPMode(g_curPC);
		break;

	
	case tagGolryunBattlePacket::GB_CHECK_JOINER_NUM :


		m_cpProcess->CheckJoinerNumGM();
		break;

	
	case tagGolryunBattlePacket::GB_TRANS_WATCHINGROOM :
		ShowLogInfo("GB Status : GB_TRANS_WATCHINGROOM");
		gcpGolryunBattleLog->Write("GB Status : GB_TRANS_WATCHINGROOM");
		m_cpProcess->TransWatchingRoomGM(g_curPC);
		break;

	
	case tagGolryunBattlePacket::GB_TRANS_WAITINGROOM :
		ShowLogInfo("GB Status : GB_TRANS_WAITINGROOM");
		gcpGolryunBattleLog->Write("GB Status : GB_TRANS_WAITINGROOM");
		m_cpProcess->TransWaitingRoomGM(g_curPC);
		break;

	
	case tagGolryunBattlePacket::GB_TRANS_BATTLESTADIUM :
		ShowLogInfo("GB Status : GB_TRANS_BATTLESTADIUM");
		gcpGolryunBattleLog->Write("GB Status : GB_TRANS_BATTLESTADIUM");
		m_cpProcess->TransBattleStadiumGM(g_curPC);
		break;

	
	case tagGolryunBattlePacket::GB_NOTICE_BATTLE :
		ShowLogInfo("GB Status : GB_NOTICE_BATTLE");
		gcpGolryunBattleLog->Write("GB Status : GB_NOTICE_BATTLE");
		RecvNoticeBattle();
		break;

	}

	
}







void CGolryunBattle_Decoder::Decoder(void)
{	

	BYTE Command = MSG_ReadByte();
	switch (Command)
	{

	case tagGolryunBattlePacket::GB_INSERT_JOIN_LIST:	
	case tagGolryunBattlePacket::GB_ASSIGN_FIGHTER_NUMBER:		
	case tagGolryunBattlePacket::GB_PAUSE_MATCH:		
	case tagGolryunBattlePacket::GB_RESTART_MATCH:
	case tagGolryunBattlePacket::GB_REQUEST_START_COUNT:		
	case tagGolryunBattlePacket::GB_REQUEST_START_BATTLE :
	case tagGolryunBattlePacket::GB_REQUEST_START_JOIN :			
	case tagGolryunBattlePacket::GB_START_RECALL_FIGHTER :			
	case tagGolryunBattlePacket::GB_REQUEST_CLOSE_JOIN :			
	case tagGolryunBattlePacket::GB_REQUEST_START_BETTING :			
	case tagGolryunBattlePacket::GB_REQUEST_READY_TOURNAMENT :			
	case tagGolryunBattlePacket::GB_REQUEST_CLOSE_BATTLE : 			
	case tagGolryunBattlePacket::GB_REQUEST_START_PRIZE :		
	case tagGolryunBattlePacket::GB_REQUEST_CANCEL_TOURNAMENT :	
	case tagGolryunBattlePacket::GB_WALKOVER_MATCH :		
	case tagGolryunBattlePacket::GB_TWOGYUN_TOTAL_NAK :		
	case tagGolryunBattlePacket::GB_FORCE_EJECT :		
	case tagGolryunBattlePacket::GB_CHECK_STATUS_PvPMODE :		
	case tagGolryunBattlePacket::GB_CHECK_JOINER_NUM :
	case tagGolryunBattlePacket::GB_TRANS_WATCHINGROOM :		
	case tagGolryunBattlePacket::GB_TRANS_WAITINGROOM :		
	case tagGolryunBattlePacket::GB_TRANS_BATTLESTADIUM :		
	case tagGolryunBattlePacket::GB_NOTICE_BATTLE :
		Decoder_ProgressCommand(Command);
		return;	
	

	
	case tagGolryunBattlePacket::GB_OPEN_BATTLE_TICKET :
		RecvOpenBattleTicket(g_curPC);
		break;
	
	
	case tagGolryunBattlePacket::GB_OPEN_SHARE_WIN :
		RecvOpenShareWin(g_curPC);
		break;

	
	case tagGolryunBattlePacket::GB_DISPLAY_TOURNAMENT :
		RecvDisplayTournament(g_curPC);
		break;	

	


	
	case tagGolryunBattlePacket::GB_ACCEPT_TRANS_GOLRYUN :
		ShowLogInfo("GB Status : GB_ACCEPT_TRANS_GOLRYUN");
		gcpGolryunBattleLog->Write("GB Status : GB_ACCEPT_TRANS_GOLRYUN");


		break;

	case tagGolryunBattlePacket::GB_BUY_BATTLE_TICKET :	

		m_cpProcess->BuyBattleTicket();							
		break;	
	
	case tagGolryunBattlePacket::GB_RENEW_BATTLE_TICKET : 

		m_cpProcess->ReNewBattleTicket();			
		break;
		
	case tagGolryunBattlePacket::GB_RENEW_DISPLAY_TOURNAMENT : 

		m_cpEncoder->SendDisplayTournament(&g_curPC->sock);			
		break;
	case tagGolryunBattlePacket::GB_ACCEPT_BATTLE :	

		m_cpProcess->Enter_By_Tournament(g_curPC);
		break;	
		
	case tagGolryunBattlePacket::GB_TIME_OVER :
		ShowLogInfo("GB Status : GB_TIME_OVER");
		gcpGolryunBattleLog->Write("GB Status : GB_TIME_OVER");
		m_cpProcess->CheckMatchResult();
		break;

	

	
	case tagGolryunBattlePacket::GB_OBSERVE_END :
		RecvObserveEnd(g_curPC);
		break;

	}



	
}