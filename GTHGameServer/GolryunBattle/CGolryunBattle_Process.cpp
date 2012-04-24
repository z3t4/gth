



#pragma warning(disable:4786)
#include "..\global.h"
#include "CGolryunBattle_Process.h"
#include "CGolryunBattle_Common.h"
#include "CGolryunBattle_Data.h"
#include <assert.h>


#include "CLandList.h"
#include "CServerLpackCtrl.h"
extern CServerLpackCtrl* gcpServerLpackCtrl;

#include "..\Tools\CTools.h"
extern CTools* gcpTools;

#include "CGolryunBattle_Log.h"
extern CGolryunBattle_Log* gcpGolryunBattleLog;

#include "tagGolryunBattle.h"
extern struct tagGolryunBattleVar gGolryunBattleVar;

#include "..\GolryunBattle\CGolryunBattle_Observer.h"
extern CGolryunBattle_Observer* gcpGolryunBattle_Observer;
















CGolryunBattle_Process::CGolryunBattle_Process(
	
	CGolryunBattle_DBV2*	 cpDBV2,	
	CGolryunBattle_Data*     cpData,
	CGolryunBattle_Encoder*  cpEncoder)
{
	m_bOK=FALSE;
	m_cpData = cpData;
	m_cpEncoder = cpEncoder;
	
	m_cpDBV2    = cpDBV2;
	m_iTournamentGUID=0;
	m_iTournamentIdx=0;
	m_eTournamentStep = TournamentStep_None;
	m_eManor = normal;
	m_bPause	= FALSE;



	InitTournament();
	

	m_bOK=TRUE;

}

CGolryunBattle_Process::~CGolryunBattle_Process()
{
	if(TRUE == GTH_IsLocalServiceWorld(tagGolryunBattle::Golryun_Battle_Map_Index)){
		if(CGolryunBattle_DBV2::ERROR_NONE == m_cpDBV2->GetLastError()){		
		struct CGolryunBattle_DBV2::tagBattle  Battle;				
		Copy_toDB_fromDATA(Battle);
		m_cpDBV2->Update(Battle);
	}
	}
	
	

	m_JoinMap.clear();
	m_AllJoinList.clear();
}


BOOL CGolryunBattle_Process::isOK(void) const
{
	return m_bOK;
}	




BOOL	CGolryunBattle_Process::isReady(void)
{
	if(FALSE == m_cpDBV2->isReady()) return FALSE;

	static BOOL bDATAloadingComplete=FALSE;
	if(FALSE == bDATAloadingComplete){
		
		struct CGolryunBattle_DBV2::tagBattle* pBattle=NULL;	
		pBattle = m_cpDBV2->GetTemporaryPlayingBattle();
		if(NULL == pBattle){
			gcpGolryunBattleLog->Write("BOOL CGolryunBattle_Process::isReady(void) const");
			gcpGolryunBattleLog->Write("if(NULL == pBattle)");
			return FALSE;
		}		
		Copy_toDATA_fromDB(pBattle);
		m_iTournamentGUID =	pBattle->nMaxTournamentGUID;
		m_iTournamentGUIDminmum = m_cpDBV2->GetMinimumTournamentGUID();
		bDATAloadingComplete=TRUE;
	}


	return TRUE;
}















void CGolryunBattle_Process::InitMatchArray(void)
{
	m_iMatchIdx		= 0;
	m_iNextRound_MatchIdx	= 0;
	m_iOffSetMatchIdx = 0;

	struct CGolryunBattle_Data::tagMatch* pMatch=NULL;
	for(int iMatchIdx=0; iMatchIdx < tagGolryunBattle::Max_Num_Match_Per_Tournament; iMatchIdx++){

		pMatch=	GetMatchPointer(iMatchIdx);
		if(NULL == pMatch) continue;

		memset(pMatch,0x00,sizeof(struct CGolryunBattle_Data::tagMatch));
		pMatch->RedCornerFighter.pcIdx=-1;
		pMatch->RedCornerFighter.FightNum=-1;

		pMatch->BlueCornerFighter.pcIdx=-1;
		pMatch->BlueCornerFighter.FightNum=-1;
	}

}

BOOL CGolryunBattle_Process::InitBattle(void)
{
	m_iTournamentIdx = 0;
	m_cpData->InitBattle();
	InitTournament();
	return TRUE;
}

BOOL CGolryunBattle_Process::InitTournament()
{
	MoveToWatchingRoomAll_CancelBattlePlayer();

	InitMatchArray();
	InitFighterArray();

	memset(&m_Ticket, 0, sizeof(struct tagTicket));
	m_bPause=FALSE;

	return TRUE;
}


void CGolryunBattle_Process::SetResultStatus(eMatchResult eResultStatus)
{
	m_eResultStatus		= eResultStatus;
}



void CGolryunBattle_Process::
SetTournamentStatus(const enum eTournamentStep eTournamentStep)
{
	m_eTournamentStep = eTournamentStep;
	m_iStartTime	= g_globalTime;
}








void CGolryunBattle_Process::SetManorStatus(enumManor eManor)
{
	m_eManor		= eManor;
}







void CGolryunBattle_Process::SaveBattleRecoder(void)
{
	
	struct CGolryunBattle_DBV2::tagBattle Battle;
	Copy_toDB_fromDATA(Battle);
	m_cpDBV2->Save(Battle);



	
	m_cpData->m_iBattleGUID++;	
	InitBattle();


	struct CGolryunBattle_DBV2::tagBattle newBattle;
	Copy_toDB_fromDATA(newBattle);
	m_cpDBV2->Add(newBattle);
	
	
	m_iTournamentGUIDminmum = m_cpDBV2->GetMinimumTournamentGUID();
}







inline void CGolryunBattle_Process::Copy_toDB_fromDATA(struct CGolryunBattle_DBV2::tagBattle& Battle)
{
	
	Battle.bActive=TRUE;
	Battle.nGUID = m_cpData->m_iBattleGUID;	
	Battle.nMaxTournamentGUID = m_iTournamentGUID;

	
	Battle.nMaxEntranceTicketGUID = m_cpData->m_EntranceTicket.iMaxAllocGUID;
	for(int idx=0; idx < tagGolryunBattle::Max_Num_Prize_List_Per_Battle; idx++){
		Battle.nLotteryGUIDList[idx] = m_cpData->m_EntranceTicket.LotteryGUIDList[idx];
	}

	
	struct CGolryunBattle_DBV2::tagTournament* pToTournament=NULL;
	const struct CGolryunBattle_Data::tagTournament* pFromTournament=NULL;

	for(idx= 0; idx < tagGolryunBattle::Max_Num_Tournament_Per_Battle; idx++){
		pToTournament = &(Battle.Tournament[idx]);
		pFromTournament   = &(m_cpData->m_Tournament[idx]);


		
		pToTournament->bComplete = pFromTournament->bComplete;
		
		pToTournament->nGUID = pFromTournament->iTournamentGUID;
		pToTournament->nLevelGroupIdx = idx;
			
		
		strncpy(pToTournament->firstWinner.Name,pFromTournament->FirstWinner.name,NAMESTRING);
		pToTournament->firstWinner.Name[NAMESTRING]=NULL;			
		pToTournament->firstWinner.Num =pFromTournament->FirstWinner.FightNum;

		
		strncpy(pToTournament->secondWinner.Name,pFromTournament->SecondWinner.name,NAMESTRING);
		pToTournament->secondWinner.Name[NAMESTRING]=NULL;			
        pToTournament->secondWinner.Num = pFromTournament->SecondWinner.FightNum;


		
		pToTournament->TwoGyun.nTotalNak = pFromTournament->TwoGyun.iTotalNak;
		pToTournament->TwoGyun.Rate= pFromTournament->TwoGyun.fShareRate;
	}
}



























inline void CGolryunBattle_Process::Copy_toDATA_fromDB(const struct CGolryunBattle_DBV2::tagBattle* pBattle)
{
	
	m_cpData->m_iBattleGUID = pBattle->nGUID;

	
	m_cpData->m_EntranceTicket.iMaxAllocGUID = pBattle->nMaxEntranceTicketGUID;
	for(int idx=0; idx < tagGolryunBattle::Max_Num_Prize_List_Per_Battle; idx++){
		m_cpData->m_EntranceTicket.LotteryGUIDList[idx] = pBattle->nLotteryGUIDList[idx];
	}


	
	struct CGolryunBattle_Data::tagTournament* pToTournament=NULL;
	const struct CGolryunBattle_DBV2::tagTournament* pFromTournament=NULL;

	for(idx= 0; idx < tagGolryunBattle::Max_Num_Tournament_Per_Battle; idx++){
		pFromTournament = &(pBattle->Tournament[idx]);
		pToTournament   = &(m_cpData->m_Tournament[idx]);


		
		pToTournament->bComplete = pFromTournament->bComplete;
		
		pToTournament->iTournamentGUID = pFromTournament->nGUID;
			
		
		pToTournament->FirstWinner.pcIdx=-1;
		pToTournament->FirstWinner.id[0]=NULL;		
		strncpy(pToTournament->FirstWinner.name,pFromTournament->firstWinner.Name,NAMESTRING);
		pToTournament->FirstWinner.name[NAMESTRING]=NULL;			
		pToTournament->FirstWinner.FightNum =pFromTournament->firstWinner.Num;







		
		pToTournament->SecondWinner.pcIdx=-1;
		pToTournament->SecondWinner.id[0]=NULL;		
		strncpy(pToTournament->SecondWinner.name,pFromTournament->secondWinner.Name,NAMESTRING);
        pToTournament->SecondWinner.name[NAMESTRING]=NULL;			
		pToTournament->SecondWinner.FightNum =pFromTournament->secondWinner.Num;


		
		pToTournament->TwoGyun.iTotalNak = pFromTournament->TwoGyun.nTotalNak;
		pToTournament->TwoGyun.fShareRate= pFromTournament->TwoGyun.Rate;


		
		struct CGolryunBattle_Data::tagMatch* pToMatch = NULL;
		for(int MatchIdx=0; MatchIdx < tagGolryunBattle::Max_Num_Match_Per_Tournament; MatchIdx++){
			pToMatch   = &(pToTournament->Match[MatchIdx]);

			
			pToMatch->RedCornerFighter.pcIdx=-1;
			pToMatch->RedCornerFighter.id[0]=NULL;
			pToMatch->RedCornerFighter.name[0]=NULL;			
			pToMatch->RedCornerFighter.FightNum=-1;

				
			
			pToMatch->BlueCornerFighter.pcIdx=-1;
			pToMatch->BlueCornerFighter.id[0]=NULL;
			pToMatch->BlueCornerFighter.name[0]=NULL;			
			pToMatch->BlueCornerFighter.FightNum=-1;

			pToMatch->WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::None;


			
			pToMatch->bComplete = FALSE;
		}		
	}
}









void CGolryunBattle_Process::InitFighterArray(void)
{
	struct tagFightPlayerInfo* pTempFighter=NULL;
	for(int idx=0; idx < tagGolryunBattle::Max_Num_Fighter_Per_Tournament; idx++){
		pTempFighter = &m_Fighter[idx];
		memset(pTempFighter,0x00,sizeof(struct tagFightPlayerInfo));
		pTempFighter->Idx=-1;
	}
}

struct CGolryunBattle_Process::tagFightPlayerInfo* 
CGolryunBattle_Process::GetFighterPointer(const int fighterIdx)
{
	if(fighterIdx < 0) return NULL;
	if(fighterIdx >= tagGolryunBattle::Max_Num_Fighter_Per_Tournament) return NULL;
	return &m_Fighter[fighterIdx];
}

struct CGolryunBattle_Data::tagMatch* 
CGolryunBattle_Process::GetMatchPointer(const int matchIdx)
{
	if(matchIdx < 0) return NULL;
	if(matchIdx > 14) return NULL;
	return &m_Match[matchIdx];
}


BOOL CGolryunBattle_Process::isFighterPlayer_and_LogOffMark(const playerCharacter_t *pc)
{
	struct CGolryunBattle_Data::tagMatch* pMatch=GetCurMatch();
	if(NULL == pMatch) return FALSE;

	if(pMatch->RedCornerFighter.pcIdx == pc->idx){
		pMatch->RedCornerFighter.pcIdx = -1;
		return TRUE;
	}
	if(pMatch->BlueCornerFighter.pcIdx == pc->idx){
		pMatch->BlueCornerFighter.pcIdx = -1;
		return TRUE;
	}
	return FALSE;
}



BOOL CGolryunBattle_Process::isJoinPlayer(const playerCharacter_t *pc)
{
	JOINTER_MAP_ITOR itor;	
	playerCharacter_t* ptempPlayer=NULL;	
	for(itor = m_JoinMap.begin(); itor != m_JoinMap.end();)
	{
		ptempPlayer=CGolryunBattle_Common::GetArenaPlayer(itor->second);
		if(NULL == ptempPlayer){
			itor = m_JoinMap.erase(itor);
			continue;		
		}

		if(ptempPlayer == pc) return TRUE;
		itor++;
	}		

	return FALSE;
}

BOOL CGolryunBattle_Process::RemoveFighterArrayV2(playerCharacter_t* pc)
{
	if(pc->GonryunBattle.m_iJoinNum < 1) return FALSE;
	if(pc->GonryunBattle.m_iJoinNum > tagGolryunBattle::Max_Num_Fighter_Per_Tournament) return FALSE;
	if(pc->idx != m_Fighter[pc->GonryunBattle.m_iJoinNum-1].Idx) return FALSE;
	m_Fighter[pc->GonryunBattle.m_iJoinNum-1].Idx=-1;
	pc->GonryunBattle.m_bWaitingRoom = FALSE;
	pc->GonryunBattle.m_iJoinNum=0;
	return TRUE;
}



void CGolryunBattle_Process::MoveToWatchingRoomAll_CancelBattlePlayer(void)
{

	playerCharacter_t* pTempPlayer=NULL;
	ALL_JOINER_ITOR iAllJoinItor;

	struct CGolryunBattle_Data::tagFighter Fighter;
	for(iAllJoinItor=m_AllJoinList.begin(); iAllJoinItor != m_AllJoinList.end(); iAllJoinItor++){

		Fighter = *iAllJoinItor;
		
		pTempPlayer = gcpTools->GetPlayerRecordPointer(Fighter.pcIdx);
		if(NULL == pTempPlayer) continue;
		if(0 != strcmp(pTempPlayer->name,Fighter.name)) continue;
		if(0 != strcmp(pTempPlayer->userID,Fighter.id)) continue;

		
		
		
		g_DBGameServer->
			SaveChaPackageListBySystem(
				gcpServerLpackCtrl->GetLpack(0),
				Fighter.name,
				gcpServerLpackCtrl->GetLpack(1),
				gcpServerLpackCtrl->GetLpack(2),
				g_golryunManagerTable[m_iTournamentIdx].deposit);


		g_logSystem->Write("경기취소 보상m_iTournamentIdx=[%d] name[%s]",
			m_iTournamentIdx,
			Fighter.name);

			
	}
	
	m_AllJoinList.clear();

	JOINTER_MAP_ITOR	itor;
	for(itor=m_JoinMap.begin(); itor != m_JoinMap.end(); itor++){

		pTempPlayer = CGolryunBattle_Common::GetPlayer(itor->second);
		if(NULL == pTempPlayer) continue;
		
		inner_MoveToWatchingRoom(pTempPlayer);		
		m_cpEncoder->SendCancelTournament(&pTempPlayer->sock);

		if(FALSE == RemoveFighterArrayV2(pTempPlayer)){		
			pTempPlayer->GonryunBattle.m_bWaitingRoom = FALSE;
			pTempPlayer->GonryunBattle.m_iJoinNum=0;
	}

		
		g_logSystem->WriteToLog( 
			pTempPlayer, 
			LOG_CLASS_GOLRYUN, 
			"%d;%d;%d;%d;;;", 
			CLogSystem::enumGolryunSubLog::TournamentCancel,
			m_iTournamentGUID,
			m_cpData->m_iBattleGUID,
			m_iTournamentIdx);	

		gcpGolryunBattleLog->Write("토너먼트가 취소 되었습니다. 참가자를 대전장으로 이동 %s m_bWaitingRoom=%d m_iJoinNum=%d", 
			pTempPlayer->name,
			pTempPlayer->GonryunBattle.m_bWaitingRoom,
			pTempPlayer->GonryunBattle.m_iJoinNum);			

	}
	m_JoinMap.clear();


}


	








inline BOOL CGolryunBattle_Process::CheckQuitUser_Waiter(playerCharacter_t *pc)
{
	JOINTER_MAP_ITOR iLoop;	
	playerCharacter_t* ptempPlayer=NULL;

	
	for(iLoop=m_JoinMap.begin() ; iLoop != m_JoinMap.end();)
	{
		ptempPlayer=CGolryunBattle_Common::GetArenaPlayer(iLoop->second);
		if(NULL == ptempPlayer){
			iLoop = m_JoinMap.erase(iLoop);
			
			continue;		
		}

		if(ptempPlayer == pc){
			iLoop = m_JoinMap.erase(iLoop);
			

			gcpGolryunBattleLog->Write("게임중에 나간경우 UserName: %s JoinNumber: %d", 
				pc->name, pc->GonryunBattle.m_iJoinNum);

			pc->GonryunBattle.m_bWaitingRoom = FALSE;
			pc->GonryunBattle.m_iJoinNum = 0;

			gcpGolryunBattleLog->Write("JoinCount : %d", m_JoinMap.size());
			ShowLogInfo("JoinCount : %d", m_JoinMap.size());

			ShowLogInfo("CheckQuitUser_Waiter()!!!");
			return TRUE;
		}

		iLoop++;
	}		

	return FALSE;
}




void CGolryunBattle_Process::CheckQuitUser(playerCharacter_t *pc)
{
	if(FALSE == isJoinPlayer(pc)) return;

	if(TRUE == isFighterPlayer_and_LogOffMark(pc)){
		CheckMatchResult();
		return;
	}

	if(TRUE == CheckQuitUser_Waiter(pc)) return;	

	
	
	
	return;
}


BOOL CGolryunBattle_Process::CheckExpelUser(playerCharacter_t *pc)
{
	if( 30 == IsPvPEnablePosition( pc->worldIdx, pc->position )){
		
		if( TRUE == pc->GonryunBattle.m_bWaitingRoom ) return FALSE;
		if( pc->GonryunBattle.m_iJoinNum > 0 ) return FALSE;
		if( TRUE == pc->pvpMode ) return FALSE;
		if( TRUE == pc->gameMaster ) return FALSE;

		if( TRUE == inner_MoveToWatchingRoom(pc) ) return TRUE;		
	}

	return FALSE;
}


void CGolryunBattle_Process::RewardBattleResult(playerCharacter_t *pc, int type)
{
	
	switch(type)
	{
		case REWARD_WINNER:
			RewardForWinner(pc);
			break;

		case REWARD_LOSER:
			RewardForLoser(pc);
			break;

		case REWARD_WAITER:
			RewardForWaiter(pc);
			break;
	}

}

void CGolryunBattle_Process::RewardForLoser(playerCharacter_t *pc)
{
	int exp = 0;
	float value = 0.0f;
	int genExp = 0;
	int nak =0; 
	int genCap = 0;

	exp=g_expTable[pc->level].exp;
	value=g_golryunRewardManagerTable[m_iTournamentIdx].defeatExp/100.0f;
	exp=exp*value;

	genExp=g_expTable[pc->genLevel].genExp;
	value=g_golryunRewardManagerTable[m_iTournamentIdx].defeatExp/100.0f;
	genExp=genExp*value/2;

	
	GTH_PC_GetLoot( pc->idx, exp, genExp, nak, genCap, 0, TRUE);

		
		g_logSystem->WriteToLog( 
			pc, 
			LOG_CLASS_GOLRYUN, 
			"%d;%d;%d;;;;", 
			CLogSystem::enumGolryunSubLog::RewardLoser,
			exp,
			genExp);
		
}

void CGolryunBattle_Process::RewardForWinner(playerCharacter_t *pc)
{
	
	int groupIdx=0;

	if(m_iMatchIdx <= eMatchIndexRound::End_MatchIdx_Round16)
	{
		groupIdx = g_golryunRewardManagerTable[m_iTournamentIdx].rewardGroupIdx[0];	
	}

	else if(m_iMatchIdx <= eMatchIndexRound::End_MatchIdx_Round8)
	{
		groupIdx = g_golryunRewardManagerTable[m_iTournamentIdx].rewardGroupIdx[1];	
	}
	
	else if(m_iMatchIdx <= eMatchIndexRound::End_MatchIdx_Round4)
	{
		groupIdx = g_golryunRewardManagerTable[m_iTournamentIdx].rewardGroupIdx[2];	
	}

	else if(m_iMatchIdx == eMatchIndexRound::End_MatchIdx_RoundFinal)
	{
		
		return;
	}

	ShowLogInfo("Reward Group Index : %d", groupIdx);
	gcpGolryunBattleLog->Write("Reward Group Index : %d", groupIdx);

	int exp=0, genExp=0, nak=0, genCap=0;
	float value = 0.0f;
	int share=0,standardvalue=0,countuse=0,mynusvalue=0;
	int i, j;

	
	for(i=0; i<3; i++)
	{
		share = g_golryunItemGroupTable[groupIdx].rewardField[i].expRate/10.f;
		mynusvalue=10-share;
		for(j=0; j<10; j++)
		{
			standardvalue=GTH_RandomizePower(1,10);
			if(0<=standardvalue && standardvalue<=share)
			{
				countuse++;
			}
		}

		if(countuse>=mynusvalue)
		{
			exp = g_expTable[pc->level].exp;
			value=g_golryunItemGroupTable[groupIdx].rewardField[i].rewardExp/100.0f;
			exp=exp*value;

			genExp=g_expTable[pc->genLevel].genExp;
			value=g_golryunItemGroupTable[groupIdx].rewardField[i].rewardExp/100.0f;
			genExp=genExp*value/2;

			
			GTH_PC_GetLoot( pc->idx, exp, genExp, nak, genCap, 0, TRUE);

			ShowLogInfo("Exp Reward Field Index : %d", i);
			gcpGolryunBattleLog->Write("Exp Reward Field Index : %d", i);

			
			g_logSystem->WriteToLog( 
				pc, 
				LOG_CLASS_GOLRYUN, 
				"%d;%d;%d;;;;", 
				CLogSystem::enumGolryunSubLog::RewardWinner,
				exp,
				genExp);
			
			break;
		}	
	}

	share=0,standardvalue=0,countuse=0,mynusvalue=0, countuse=0;

	
	for(i=0; i<3; i++)
	{
		share = g_golryunItemGroupTable[groupIdx].rewardField[i].chooseRate/10.f;
		mynusvalue=10-share;
		for(j=0; j<10; j++)
		{
			standardvalue=GTH_RandomizePower(1,10);
			if(0<=standardvalue && standardvalue<=share)
			{
				countuse++;
			}
		}

		if(countuse>=mynusvalue)
		{
			int itemIdx = g_golryunItemGroupTable[groupIdx].rewardField[i].rewardItem;
			int itemNumber = g_golryunItemGroupTable[groupIdx].rewardField[i].provisionNum;

			int addItemData[6];

			addItemData[0] = itemIdx;		
			addItemData[1] = -1;						
			addItemData[2] = itemNumber;							
			addItemData[3] = 1;							
			addItemData[4] = -1;					
			addItemData[5] = -1;					

			AI_NPC_AddItem( pc, addItemData );
			ShowLogInfo("Item Reward Field Index : %d", i);
			gcpGolryunBattleLog->Write("Item Reward Field Index : %d", i);

			
			g_logSystem->WriteToLog( 
				pc, 
				LOG_CLASS_GOLRYUN, 
				"%d;%d;%d;;;;", 
				CLogSystem::enumGolryunSubLog::RewardWinner_Item,
				itemIdx,
				itemNumber);
			
			break;
		}	
	}
}

void CGolryunBattle_Process::RewardForWaiter(playerCharacter_t *pc)
{
	int exp = 0;
	float value = 0.0f;
	int genExp = 0;
	int nak =0; 
	int genCap = 0;

	exp=g_expTable[pc->level].exp;
	value=g_golryunRewardManagerTable[m_iTournamentIdx].waitingExp/100.0f;
	exp=exp*value;

	genExp=g_expTable[pc->genLevel].genExp;
	value=g_golryunRewardManagerTable[m_iTournamentIdx].waitingExp/100.0f;
	genExp=genExp*value/2;

	
	GTH_PC_GetLoot( pc->idx, exp, genExp, nak, genCap, 0, TRUE);

	
	g_logSystem->WriteToLog( 
		pc, 
		LOG_CLASS_GOLRYUN, 
		"%d;%d;%d;;;;", 
		CLogSystem::enumGolryunSubLog::RewardWaiter,
		exp,
		genExp);
	
}



struct CGolryunBattle_Data::tagMatch* 
CGolryunBattle_Process::GetCurMatch(void)
{
	return &m_Match[m_iMatchIdx];
}























inline void CGolryunBattle_Process::
SetCheckNormalMatchParam(struct tagCheckNormalMatchParam& Var)
{
	Var.pMatch		  = GetCurMatch();
	Var.Red.pFighter  = &(Var.pMatch->RedCornerFighter);
	Var.Blue.pFighter = &(Var.pMatch->BlueCornerFighter);
	Var.Red.pPlayer   = GetFighterPlayerPointer(Var.Red.pFighter);
	Var.Blue.pPlayer  = GetFighterPlayerPointer(Var.Blue.pFighter);


	switch(Var.pMatch->WinnerCorner){
		case CGolryunBattle_Data::tagMatch::enumCorner::Red:
			Var.Winner = Var.Red;
			Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::Red;
			Var.Loser  = Var.Blue;			
			break;

		case CGolryunBattle_Data::tagMatch::enumCorner::Blue:
			Var.Winner = Var.Blue;
			Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::Blue;
			Var.Loser  = Var.Red;			
			break;

		case CGolryunBattle_Data::tagMatch::enumCorner::None:
			Var.Winner.pPlayer= NULL;
			Var.Winner.pFighter=NULL;

			Var.Loser.pPlayer = NULL;
			Var.Loser.pFighter=NULL;
			Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::None;
			break;
	}
};



void CGolryunBattle_Process::MoveToEndMatch_Result(void)
{
	struct tagCheckNormalMatchParam Var;
	SetCheckNormalMatchParam(Var);

	switch(m_eResultStatus)
	{
		case eMatchResult::RESULT_NORMAL_MATCH: 
		case eMatchResult::RESULT_TIMEOVER_MATCH:
			{
				

				if(NULL != Var.Winner.pPlayer)
				{
					if(m_iMatchIdx == eMatchIndexRound::End_MatchIdx_RoundFinal){					
					MoveToWatchingRoom(Var.Winner.pPlayer);
						gcpGolryunBattleLog->Write("결승전에서 대전장으로 이동 %s", Var.Winner.pPlayer->name);
				}else{
					MoveToWaitingRoom(Var.Winner.pPlayer);
						gcpGolryunBattleLog->Write("승리 시 대기장으로 이동 %s", Var.Winner.pPlayer->name);
				}

				RewardBattleResult(Var.Winner.pPlayer, REWARD_WINNER);
				Var.Winner.pPlayer->GonryunBattlePractice.Record.iwin++;
				g_DBGameServer->SaveGonryunBattle(Var.Winner.pPlayer);
				}
				

				if(NULL != Var.Loser.pPlayer)
				{
				
				MoveToWatchingRoom(Var.Loser.pPlayer);
				RewardBattleResult(Var.Loser.pPlayer, REWARD_LOSER);
				Var.Loser.pPlayer->GonryunBattlePractice.Record.idefeat++;
				g_DBGameServer->SaveGonryunBattle(Var.Loser.pPlayer);
					gcpGolryunBattleLog->Write("패배 시 대전장으로 이동 %s", Var.Loser.pPlayer->name);
				}
				
			}
			break;

		case eMatchResult::RESULT_DRAWN_MATCH:
			{	
				if(NULL != Var.Red.pPlayer){
					MoveToWatchingRoom(Var.Red.pPlayer);


					RewardBattleResult(Var.Red.pPlayer, REWARD_LOSER);
					Var.Red.pPlayer->GonryunBattlePractice.Record.idraw++;
					g_DBGameServer->SaveGonryunBattle(Var.Red.pPlayer);
					gcpGolryunBattleLog->Write("무승부 처리 RED:%s ", Var.Red.pPlayer->name);

				}
				if(NULL != Var.Blue.pPlayer){
					MoveToWatchingRoom(Var.Blue.pPlayer);				


					RewardBattleResult(Var.Blue.pPlayer, REWARD_LOSER);
					Var.Blue.pPlayer->GonryunBattlePractice.Record.idraw++;
					g_DBGameServer->SaveGonryunBattle(Var.Blue.pPlayer);
					gcpGolryunBattleLog->Write("무승부 처리 BLUE:%s ", Var.Blue.pPlayer->name);
				}
				m_cpEncoder->ManagerLocalUserNotice(167);	

			}
			break;

		case eMatchResult::RESULT_OFFLINE_MATCH: 
			{
				if(NULL != Var.Winner.pPlayer){
					if(m_iMatchIdx == eMatchIndexRound::End_MatchIdx_RoundFinal){						
						MoveToWatchingRoom(Var.Winner.pPlayer);
					}else{
						MoveToWaitingRoom(Var.Winner.pPlayer);
					}

					RewardBattleResult(Var.Winner.pPlayer, REWARD_WINNER);
					Var.Winner.pPlayer->GonryunBattlePractice.Record.iwin++;
					g_DBGameServer->SaveGonryunBattle(Var.Winner.pPlayer);

				}

				if(NULL != Var.Loser.pPlayer){
					RewardBattleResult(Var.Loser.pPlayer, REWARD_LOSER);
					Var.Loser.pPlayer->GonryunBattlePractice.Record.idefeat++;
					g_DBGameServer->SaveGonryunBattle(Var.Loser.pPlayer);

				}
				m_cpEncoder->ManagerLocalUserNotice(167);	
			}
			break;


		case eMatchResult::RESULT_DEFAULT_MATCH:	
			{
				m_cpEncoder->ManagerLocalUserNotice(167);	
			}
			break;

		case eMatchResult::RESULT_ERROR_MATCH:
		default:
			{
				if(NULL != Var.Red.pPlayer){
					MoveToWatchingRoom(Var.Red.pPlayer);
				}
				if(NULL != Var.Blue.pPlayer){
					MoveToWatchingRoom(Var.Blue.pPlayer);				
				}
				m_cpEncoder->ManagerLocalUserNotice(167);	
			}
			break;			
	}




	if(NULL != Var.Red.pPlayer){
		Var.Red.pPlayer->alive = true;
		Var.Red.pPlayer->curRA = Var.Red.pPlayer->calMaxRA;
		Var.Red.pPlayer->curSA = Var.Red.pPlayer->calMaxSA;
		m_cpEncoder->Send_GB_BATTLE_RASA_Sync(Var.Red.pPlayer);	
		
	}

	if(NULL != Var.Blue.pPlayer){
		Var.Blue.pPlayer->alive = true;
		Var.Blue.pPlayer->curRA = Var.Blue.pPlayer->calMaxRA;
		Var.Blue.pPlayer->curSA = Var.Blue.pPlayer->calMaxSA;
		m_cpEncoder->Send_GB_BATTLE_RASA_Sync(Var.Blue.pPlayer);		
	}

	
	
	
	if(m_iMatchIdx < eMatchIndexRound::End_MatchIdx_RoundFinal){
		struct CGolryunBattle_Data::tagMatch*	pNextRoundMatch = GetMatchPointer(m_iOffSetMatchIdx + m_iNextRound_MatchIdx);

		if(NULL != pNextRoundMatch){

			if(0 == (m_iMatchIdx%2)){
				
				pNextRoundMatch->RedCornerFighter.pcIdx=-1;
				pNextRoundMatch->RedCornerFighter.FightNum=-1;
				if(NULL != Var.Winner.pFighter) pNextRoundMatch->RedCornerFighter =  *(Var.Winner.pFighter);

			}else{
				
				pNextRoundMatch->BlueCornerFighter.pcIdx=-1;
				pNextRoundMatch->BlueCornerFighter.FightNum=-1;
				if(NULL != Var.Winner.pFighter) pNextRoundMatch->BlueCornerFighter = *(Var.Winner.pFighter);
			}

			if(1 == (m_iMatchIdx%2)){
				m_iNextRound_MatchIdx++;		
			}
		}
	}
}

void CGolryunBattle_Process::CheckFighterWaitingRoomAndJoinNumber(void)
{

	playerCharacter_t* pTempPlayer=NULL;
	ALL_JOINER_ITOR iAllJoinItor;

	struct CGolryunBattle_Data::tagFighter Fighter;
	for(iAllJoinItor=m_AllJoinList.begin(); iAllJoinItor != m_AllJoinList.end(); iAllJoinItor++){

		Fighter = *iAllJoinItor;
		
		pTempPlayer = gcpTools->GetPlayerRecordPointer(Fighter.pcIdx);
		if(NULL == pTempPlayer) continue;

		pTempPlayer->GonryunBattle.m_bWaitingRoom = FALSE;
		pTempPlayer->GonryunBattle.m_iJoinNum = 0;
		
	}
}



inline void CGolryunBattle_Process::MoveToEndMatch_NextMatchProgress_RoundEnd(void)
{

	
	
	SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_None);
	ShowLogInfo("GB Status : GB_REQUEST_WAIT_TOURNAMENT");
	gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_WAIT_TOURNAMENT");

	m_cpEncoder->ManagerLocalUserNotice(164);	
	m_cpEncoder->ManagerLocalUserNotice(165);	
	m_cpEncoder->ManagerLocalUserNotice(166);	

	
	CheckFighterWaitingRoomAndJoinNumber();
	

	struct CGolryunBattle_Data::tagTournament* pTournament = 
		m_cpData->GetTournamentPointer(m_iTournamentIdx);
	if(NULL != pTournament){
		


		
		pTournament->bComplete = TRUE;
		pTournament->iTournamentGUID = m_iTournamentGUID;
		m_iTournamentGUID = m_iTournamentGUID+1;
		pTournament->iPlayLevelGroupIdx = m_iTournamentIdx;

		for(int MatchIdx=0; MatchIdx < tagGolryunBattle::Max_Num_Match_Per_Tournament; MatchIdx++){				
			pTournament->Match[MatchIdx] = m_Match[MatchIdx];
		}


		pTournament->FirstWinner = m_FirstWinner;
		pTournament->SecondWinner = m_SecondWinner;

		playerCharacter_t* pWinner=gcpTools->GetPlayerRecordPointer(m_FirstWinner.pcIdx);
		if(NULL != pWinner) SettingManorColor(pWinner);
		
		
		
		pTournament->TwoGyun.iTotalNak  = m_Ticket.iTotalNak;
		pTournament->TwoGyun.fShareRate = 1.0f;
		if((m_FirstWinner.FightNum >= 1) && (m_FirstWinner.FightNum <= tagGolryunBattle::Max_Num_Fighter_Per_Tournament)){
			pTournament->TwoGyun.fShareRate = m_Ticket.Fighter[m_FirstWinner.FightNum-1].fShareRate;
		}		

		char strShareRate[_MAX_PATH];
		_snprintf(strShareRate,_MAX_PATH,"Share Rate = %7.2f",pTournament->TwoGyun.fShareRate);
		char strTotalNak[_MAX_PATH];
		_snprintf(strShareRate,_MAX_PATH,"Total Nak = %d",pTournament->TwoGyun.iTotalNak);

		
		if(NULL != g_curPC){
			g_logSystem->WriteToLog( 
				g_curPC, 
				LOG_CLASS_GOLRYUN, 
				"%d;%d;%d;%d;%d;%s;%s", 
				CLogSystem::enumGolryunSubLog::TournamentResult,
				pTournament->iTournamentGUID,
				pTournament->iPlayLevelGroupIdx,
				pTournament->FirstWinner.FightNum,
				pTournament->SecondWinner.FightNum,
				strShareRate,
				strTotalNak);
		}
	}
		

	playerCharacter_t* pTempPlayer=NULL;
	ALL_JOINER_ITOR iAllJoinItor;

	struct CGolryunBattle_Data::tagFighter Fighter;
	for(iAllJoinItor=m_AllJoinList.begin(); iAllJoinItor != m_AllJoinList.end(); iAllJoinItor++){

		Fighter = *iAllJoinItor;
		
		pTempPlayer = gcpTools->GetPlayerRecordPointer(Fighter.pcIdx);
		if(NULL == pTempPlayer) continue;
		if(0 != strcmp(pTempPlayer->name,Fighter.name)) continue;
		if(0 != strcmp(pTempPlayer->userID,Fighter.id)) continue;

		
		
		
		g_DBGameServer->
			SaveChaPackageListBySystem(
				gcpServerLpackCtrl->GetLpack(0),
				Fighter.name,
				gcpServerLpackCtrl->GetLpack(1),
				gcpServerLpackCtrl->GetLpack(5),
				g_golryunManagerTable[m_iTournamentIdx].deposit);


		g_logSystem->Write("경기종료 보상m_iTournamentIdx=[%d] name[%s]",
			m_iTournamentIdx,
			Fighter.name);

			
	}
	
	m_AllJoinList.clear();



	MoveToWatchingRoomAll_NotBattlePlayer();
	
		
	

}

inline void CGolryunBattle_Process::MoveToEndMatch_NextMatchProgress(void)
{
	switch(m_iMatchIdx){		
		case eMatchIndexRound::End_MatchIdx_Round16:
			{
				m_cpEncoder->ManagerLocalUserNotice(160);	
				SetTournamentStatus(TournamentStep_START_COUNT);
				gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_START_COUNT");
			}
			break;

		case eMatchIndexRound::End_MatchIdx_Round8:
			{
				m_cpEncoder->ManagerLocalUserNotice(162);	
				SetTournamentStatus(TournamentStep_START_COUNT);

				gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_START_COUNT");	
			}
			break;

		case eMatchIndexRound::End_MatchIdx_Round4:
			{
				m_cpEncoder->ManagerLocalUserNotice(210);	


				SetProgressMatchStatus(TRUE);
				SetTournamentStatus(TournamentStep_Match_Pause);
				m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::AUTO_PAUSE_PRE_FINAL_MATCH);
				
				gcpGolryunBattleLog->Write("결승전에서 일시 중지");			
			}
			break;


		case eMatchIndexRound::End_MatchIdx_RoundFinal:
			MoveToEndMatch_NextMatchProgress_RoundEnd();
			
			

			
			{
				struct CGolryunBattle_DBV2::tagBattle  Battle;				
				Copy_toDB_fromDATA(Battle);
				m_cpDBV2->Update(m_iTournamentIdx,Battle);

				playerCharacter_t* pWinner=gcpTools->GetPlayerRecordPointer(m_FirstWinner.pcIdx);
				if(NULL != pWinner){
					g_DBGameServer->GolryunBattle_SaveBattleInfo(
						pWinner, 
						m_cpData->m_iBattleGUID, 
						m_iTournamentGUID,
						pWinner->enumNameColor);
				}
			}
			break;

		default:
			SetTournamentStatus(TournamentStep_START_COUNT);
			gcpGolryunBattleLog->Write("m_iMatchIdx = %d", m_iMatchIdx);
			break;


	}
	m_iMatchIdx++;
	if(m_iMatchIdx > eMatchIndexRound::End_MatchIdx_RoundFinal) m_iMatchIdx = 0;
}










	




void CGolryunBattle_Process::CheckJoinerNumGM()
{
	int joinNum = m_JoinMap.size();
	m_cpEncoder->SendCheckJoinerNum(&g_curPC->sock, joinNum);
}

int CGolryunBattle_Process::BuyEntranceTicket()
{

	
	int reqNak = g_itemTable[tagGolryunBattle::Entrance_Ticket_Item_Index].sellCost;
	
	
	g_curPC->curChargeSE -= reqNak;

	int addItemData[6];

	addItemData[0] = tagGolryunBattle::Entrance_Ticket_Item_Index;		
	addItemData[1] = -1;						
	addItemData[2] = 1;							
	addItemData[3] = 1;							
	addItemData[4] = -1;					
	addItemData[5] = -1;					

	AI_NPC_AddItem( g_curPC, addItemData );

	return 0;
}

int CGolryunBattle_Process::BuyBattleTicket()
{

	
	if(TournamentStep_START_BETTING != GetTournamentStatus()){
		
		m_cpEncoder->SendErrorCode(&g_curPC->sock, 0, tagGolryunBattlePacket_GB_ERROR_CODE::not_time_buy_Twogyun);
		return 0;
	}

	int itemTableIdx  = MSG_ReadShort();

	switch(itemTableIdx){
		case tagGolryunBattle::TwoGyun_100000_Item_Index:
		case tagGolryunBattle::TwoGyun_1000000_Item_Index:
			break;
		default:
			return 0;
	}	
	int	iSelectFighterNum = MSG_ReadShort(); 
	if(iSelectFighterNum > tagGolryunBattle::Max_Num_Fighter_Per_Tournament) return 0;
	if(iSelectFighterNum < 1)
	{
		
		m_cpEncoder->SendErrorCode(&g_curPC->sock, 0, tagGolryunBattlePacket_GB_ERROR_CODE::not_selected_twogryun_figtherNum);
		return 0;
	}

	
	int reqNak = g_itemTable[itemTableIdx].sellCost;
	if(g_curPC->curChargeSE < reqNak){
		
		m_cpEncoder->SendErrorCode(&g_curPC->sock, 0, tagGolryunBattlePacket_GB_ERROR_CODE::not_enough_nak);
		return 0;
	}

	int inventoryPos = GTH_FindEmptyInventoryPos( g_curPC );
	if( inventoryPos < 0 )	
	{
		g_logSystem->Write("Obtain Ticket Item Failure!! : Inventory Space not Enough. %s(%s) )", g_curPC->name, g_curPC->userID );
		m_cpEncoder->SendErrorCode(&g_curPC->sock, 0, tagGolryunBattlePacket_GB_ERROR_CODE::not_enough_inventory);
		return 0;
	}

	g_curPC->curChargeSE -= reqNak;
	m_Ticket.iTotalNak += reqNak;
	m_Ticket.Fighter[iSelectFighterNum-1].iTotalNak += reqNak;

	for(int idx=0; idx < tagGolryunBattle::Max_Num_Fighter_Per_Tournament; idx++)
	{
		if(m_Ticket.Fighter[idx].iTotalNak <= 0){
			m_Ticket.Fighter[idx].fShareRate=0.0f;
			continue;
		}		

		m_Ticket.Fighter[idx].fShareRate = 			
				((float)m_Ticket.iTotalNak * 0.75f) / (float)m_Ticket.Fighter[idx].iTotalNak;
	}

	m_cpEncoder->SendBuyBattleTicket(g_curPC);
	m_cpEncoder->SendShareRate(g_curPC);
	gcpGolryunBattleLog->Write("투권구매 : %s %d",g_curPC->name, reqNak);
	gcpGolryunBattleLog->Write("Total : %d",m_Ticket.iTotalNak);

	int addItemData[6];
	addItemData[0] = itemTableIdx;		
	addItemData[1] = -1;				
	addItemData[2] =  1;				
	addItemData[3] =  1;				
	addItemData[4] = -1;				
	addItemData[5] = iSelectFighterNum;	

	
	AI_NPC_AddItem( g_curPC, addItemData );
	GTH_DeleteNoticePC(g_curPC);
	return 0;
}









void CGolryunBattle_Process::ReNewBattleTicket()
{
	
	m_cpEncoder->SendShareRate(g_curPC);
}









void CGolryunBattle_Process::ExtractPrizeNumber()
{
	CRandList PrizeList;
	PrizeList.Initialize();
	
	for(int iEnterTicketIdx=1; iEnterTicketIdx < m_cpData->m_EntranceTicket.iMaxAllocGUID; iEnterTicketIdx++)	
		PrizeList.PushData(iEnterTicketIdx);


	int iSelectLotteryIdx;
	for(int LotteryIdx=0; LotteryIdx < tagGolryunBattle::Max_Num_Prize_List_Per_Battle; LotteryIdx++){
		m_cpData->m_EntranceTicket.LotteryGUIDList[LotteryIdx]=-1;
		if(FALSE == PrizeList.GetRandomData(iSelectLotteryIdx)) continue;
		m_cpData->m_EntranceTicket.LotteryGUIDList[LotteryIdx]=iSelectLotteryIdx;
	}	


	
	
}





void CGolryunBattle_Process::MoveToWatchingRoomAll_NotBattlePlayer(void)
{
	JOINTER_MAP_ITOR	joinerItor;
	playerCharacter_t* pTempPlayer=NULL;

	for(joinerItor = m_JoinMap.begin(); joinerItor != m_JoinMap.end();){
		
		pTempPlayer = CGolryunBattle_Common::GetPlayer(joinerItor->second);			
		if(NULL == pTempPlayer){
			joinerItor = m_JoinMap.erase(joinerItor);
			continue;
		}

		



		RewardBattleResult(pTempPlayer, REWARD_WAITER);
		inner_MoveToWatchingRoom(pTempPlayer);		
		m_cpEncoder->SendCancelTournament(&pTempPlayer->sock);		
		if(FALSE == RemoveFighterArrayV2(pTempPlayer)){		
			pTempPlayer->GonryunBattle.m_bWaitingRoom = FALSE;
			pTempPlayer->GonryunBattle.m_iJoinNum=0;	
		}

		gcpGolryunBattleLog->Write("토너먼트가 종료 되었습니다. 번호 부여 않된 유저 대전장으로 이동 %s m_bWaitingRoom=%d m_iJoinNum=%d", 
			pTempPlayer->name,
			pTempPlayer->GonryunBattle.m_bWaitingRoom,
			pTempPlayer->GonryunBattle.m_iJoinNum);			

		joinerItor = m_JoinMap.erase(joinerItor);
	}		
}



void CGolryunBattle_Process::MoveToWatchingRoom(playerCharacter_t* pc)
{
	if(FALSE == inner_MoveToWatchingRoom(pc)) return; 
	if(FALSE == RemoveFighterArrayV2(pc)){

		gcpGolryunBattleLog->Write("%s(%d) 대전장으로 이동 시키려 했으나 실패 했습니다.%s m_bWaitingRoom=%d m_iJoinNum=%d", 
			__FILE__,
			__LINE__,
			pc->name,
			pc->GonryunBattle.m_bWaitingRoom,
			pc->GonryunBattle.m_iJoinNum);		
	};



	JOINTER_MAP_ITOR	joinerItor;
	playerCharacter_t* pTempPlayer=NULL;
	for(joinerItor = m_JoinMap.begin(); joinerItor != m_JoinMap.end();){
		
		pTempPlayer = CGolryunBattle_Common::GetArenaPlayer(joinerItor->second);
		if(NULL == pTempPlayer){
			joinerItor = m_JoinMap.erase(joinerItor);
			continue;
		}

		if(pTempPlayer == pc){
			joinerItor = m_JoinMap.erase(joinerItor);
			break;
		}

		joinerItor++;

	}	

}



void CGolryunBattle_Process::MoveToRedCorner(playerCharacter_t* pc)
{
	int zoneIdx;
	vec3_t pos;
	pos[0] = 14709;
	pos[1] = 3680;
	pos[2] = 3252;

	if ( pc->active && pc->worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index)
	{
		vec_copy( pos, pc->position );
		zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
		pc->zoneIdx = zoneIdx;
		pc->pvpMode = true;
		
		GTH_SendPCEventMessage_Respawn( pc );
		GTH_SendMessage_SyncItemObject( pc );	
		

	}
}

void CGolryunBattle_Process::MoveToBlueCorner(playerCharacter_t* pc)
{
	int zoneIdx;
	vec3_t pos;
	pos[0] = 14056;
	pos[1] = 3694;
	pos[2] = 3252;

	if ( pc->active && pc->worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index)
	{
		vec_copy( pos, pc->position );
		zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
		pc->zoneIdx = zoneIdx;
		pc->pvpMode = true;
		
		GTH_SendPCEventMessage_Respawn( pc );
		GTH_SendMessage_SyncItemObject( pc );	
		

	}
}



void CGolryunBattle_Process::MoveToStartMatch()
{
	struct CGolryunBattle_Data::tagMatch* pMatch=GetCurMatch();
	assert(NULL != pMatch);

	playerCharacter_t* pRedCornerPlayer=GetFighterPlayerPointer(&pMatch->RedCornerFighter);
	playerCharacter_t* pBlueCornerPlayer=GetFighterPlayerPointer(&pMatch->BlueCornerFighter);
	

	if(NULL != pRedCornerPlayer){
		if(TRUE == pRedCornerPlayer->GonryunBattle.Observer.bActive){
			if(TRUE == gcpGolryunBattle_Observer->Delete(pRedCornerPlayer)){
				pRedCornerPlayer->GonryunBattle.Observer.bActive=FALSE;
				m_cpEncoder->SendObserveEnd(pRedCornerPlayer,TRUE);
			}else{
				
				m_cpEncoder->SendObserveEnd(pRedCornerPlayer,FALSE);
			}
		}
		MoveToRedCorner(pRedCornerPlayer);
	}
	if(NULL != pBlueCornerPlayer){
		if(TRUE == pBlueCornerPlayer->GonryunBattle.Observer.bActive){
			if(TRUE == gcpGolryunBattle_Observer->Delete(pBlueCornerPlayer)){
				pBlueCornerPlayer->GonryunBattle.Observer.bActive=FALSE;
				m_cpEncoder->SendObserveEnd(pBlueCornerPlayer,TRUE);
			}else{
				
				m_cpEncoder->SendObserveEnd(pBlueCornerPlayer,FALSE);
			}
	}
		MoveToBlueCorner(pBlueCornerPlayer);	
	}

	if(m_iMatchIdx == eMatchIndexRound::Offset_MatchIdx_Round16)
		m_cpEncoder->ManagerLocalUserNotice(159);	

	if(m_iMatchIdx == eMatchIndexRound::Offset_MatchIdx_Round8)
		m_cpEncoder->ManagerLocalUserNotice(161);	

	if(m_iMatchIdx == eMatchIndexRound::Offset_MatchIdx_Round4)
		m_cpEncoder->ManagerLocalUserNotice(209);	

	if(m_iMatchIdx == eMatchIndexRound::Offset_MatchIdx_RoundFinal)
		m_cpEncoder->ManagerLocalUserNotice(163);	
	

	CheckGameStartMatch();


	char RedName[NAMESTRING+1]="";
	char BlueName[NAMESTRING+1]="";
	if(NULL != pRedCornerPlayer){
		strncpy(RedName,pRedCornerPlayer->name,NAMESTRING);
		RedName[NAMESTRING]=NULL;
	}
	if(NULL != pBlueCornerPlayer){
		strncpy(BlueName,pBlueCornerPlayer->name,NAMESTRING);
		BlueName[NAMESTRING]=NULL;
	}

	ShowLogInfo("MatchCnt : %d",m_iMatchIdx);
	gcpGolryunBattleLog->Write("MatchCnt : %d",m_iMatchIdx);

	ShowLogInfo("대전자: %s vs %s",RedName, BlueName);
	gcpGolryunBattleLog->Write("대전자: %s vs %s",RedName, BlueName);

	if(NULL != pRedCornerPlayer)
	{
		g_logSystem->WriteToLog( 
			pRedCornerPlayer, 
			LOG_CLASS_GOLRYUN, 
			"%d;%d;;;;%s;%s", 
			CLogSystem::enumGolryunSubLog::StartMatch,
			m_iMatchIdx,
			RedName,
			BlueName);
	}

}

void CGolryunBattle_Process::FindOutFighterNumber()
{

	struct CGolryunBattle_Data::tagMatch* pMatch=GetCurMatch();
	assert(NULL != pMatch);

	playerCharacter_t* pRedCornerPlayer=GetFighterPlayerPointer(&pMatch->RedCornerFighter);
	playerCharacter_t* pBlueCornerPlayer=GetFighterPlayerPointer(&pMatch->BlueCornerFighter);
	
	int redFighterNum=0;
	int blueFighterNum=0;

	if(NULL == pRedCornerPlayer || NULL == pBlueCornerPlayer)
	{
		redFighterNum=0;
		blueFighterNum=0;
	}
	else
	{
		redFighterNum = pRedCornerPlayer->GonryunBattle.m_iJoinNum;
		blueFighterNum = pBlueCornerPlayer->GonryunBattle.m_iJoinNum;
	}
	
	playerCharacter_t* pTempPlayer=NULL;
	for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++ ){
		pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
		if(NULL == pTempPlayer) continue;
		if((pTempPlayer->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index) &&	(pTempPlayer->worldIdx != 0)) continue;
		if(FALSE == pTempPlayer->GonryunBattle.Observer.bActive)	continue;

		m_cpEncoder->SendStartMatchNotice(pTempPlayer, redFighterNum, blueFighterNum);
	
	}	

}










inline void CGolryunBattle_Process::
CheckMatchResult_when_BothLive(struct tagCheckNormalMatchParam& Var)
{
	float RedRa = (Var.Red.pPlayer->curRA / Var.Red.pPlayer->calMaxRA)*100;
	float BlueRa = (Var.Blue.pPlayer->curRA/ Var.Blue.pPlayer->calMaxRA)*100;

	if(RedRa > BlueRa)
	{
		Var.Winner.pPlayer= Var.Red.pPlayer;
		Var.Winner.pFighter = Var.Red.pFighter;
		Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::Red;

		Var.Loser.pPlayer = Var.Blue.pPlayer;
		Var.Loser.pFighter = Var.Blue.pFighter;
		
		SetResultStatus(RESULT_TIMEOVER_MATCH);		
		return;
	}

	if(RedRa < BlueRa)
	{
		Var.Winner.pPlayer  = Var.Blue.pPlayer;
		Var.Winner.pFighter = Var.Blue.pFighter;
		Var.WinnerCorner   = CGolryunBattle_Data::tagMatch::enumCorner::Blue;

		Var.Loser.pPlayer   = Var.Red.pPlayer;
		Var.Loser.pFighter  = Var.Red.pFighter;		
		SetResultStatus(RESULT_TIMEOVER_MATCH);		
		return;		
	}

	Var.Winner.pPlayer  = NULL;
	Var.Winner.pFighter = NULL;
	Var.WinnerCorner   = CGolryunBattle_Data::tagMatch::enumCorner::None;

	Var.Loser.pPlayer  = NULL;
	Var.Loser.pFighter = NULL;	
	SetResultStatus(RESULT_DRAWN_MATCH);	
}

inline void CGolryunBattle_Process::
CheckMatchResult_SetWinnerAndLoser(struct tagCheckNormalMatchParam& Var)
{
	if((NULL == Var.Red.pPlayer) && (NULL == Var.Blue.pPlayer)){
		SetResultStatus(RESULT_DEFAULT_MATCH);
		return;
	}


	if((NULL != Var.Red.pPlayer) && (NULL == Var.Blue.pPlayer)){
		Var.Winner.pPlayer  = Var.Red.pPlayer;
		Var.Winner.pFighter = Var.Red.pFighter;
		Var.WinnerCorner   = CGolryunBattle_Data::tagMatch::enumCorner::Red;

		Var.Loser.pPlayer  = Var.Blue.pPlayer;
		Var.Loser.pFighter = Var.Blue.pFighter;		
		SetResultStatus(RESULT_OFFLINE_MATCH);	
		return;
	}

	if((NULL == Var.Red.pPlayer) && (NULL != Var.Blue.pPlayer)){
		Var.Winner.pPlayer  = Var.Blue.pPlayer;
		Var.Winner.pFighter = Var.Blue.pFighter;
		Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::Blue;

		Var.Loser.pPlayer = Var.Red.pPlayer;
		Var.Loser.pFighter = Var.Red.pFighter;
		
		SetResultStatus(RESULT_OFFLINE_MATCH);
		return;
		
	}

	if((NULL != Var.Red.pPlayer) && (NULL != Var.Blue.pPlayer)){
		if(Var.Red.pPlayer->alive && !Var.Blue.pPlayer->alive){
			Var.Winner.pPlayer= Var.Red.pPlayer;
			Var.Winner.pFighter = Var.Red.pFighter;
			Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::Red;

			Var.Loser.pPlayer = Var.Blue.pPlayer;
			Var.Loser.pFighter = Var.Blue.pFighter;
			
			SetResultStatus(RESULT_NORMAL_MATCH);
			return;
		}


		if(Var.Blue.pPlayer->alive && !Var.Red.pPlayer->alive){
			Var.Winner.pPlayer= Var.Blue.pPlayer;
			Var.Winner.pFighter = Var.Blue.pFighter;
			Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::Blue;

			Var.Loser.pPlayer = Var.Red.pPlayer;
			Var.Loser.pFighter = Var.Red.pFighter;
			
			SetResultStatus(RESULT_NORMAL_MATCH);	
			return;
		}
		
		if(Var.Red.pPlayer->alive && Var.Blue.pPlayer->alive){
			CheckMatchResult_when_BothLive(Var);
			return;
		}		
	}
}


inline void CGolryunBattle_Process::
CheckMatchResult_Effect(struct tagCheckNormalMatchParam& Var)
{

	if(NULL != Var.Winner.pPlayer){
		if(m_iMatchIdx == eMatchIndexRound::End_MatchIdx_RoundFinal) 
		{
			GTH_SendMessage_CompleteEffect_To_AllPC( Var.Winner.pPlayer, 68 );
			m_cpEncoder->SendEffectResult(&(Var.Winner.pPlayer->sock), 68);
		}
		else
		{
			GTH_SendMessage_CompleteEffect_To_AllPC(Var.Winner.pPlayer, 66 );	
			m_cpEncoder->SendEffectResult(&(Var.Winner.pPlayer->sock), 66);
		}		
		ShowLogInfo("승리 : %s",Var.Winner.pPlayer->name);
		gcpGolryunBattleLog->Write("승리 : %s",Var.Winner.pPlayer->name);
		m_cpEncoder->SendEndMatch(&(Var.Winner.pPlayer)->sock, tagGolryunBattlePacket_GB_REQUEST_END_MATCH::enumCode::winner);
	}

	if(NULL != Var.Loser.pPlayer){
		GTH_SendMessage_CompleteEffect_To_AllPC( Var.Loser.pPlayer, 67 );
		m_cpEncoder->SendEffectResult(&(Var.Loser.pPlayer->sock), 67);

		gcpGolryunBattleLog->Write("패배 : %s",Var.Loser.pPlayer->name);	
		ShowLogInfo("패배 : %s",Var.Loser.pPlayer->name);	
		m_cpEncoder->SendEndMatch(&(Var.Loser.pPlayer)->sock, tagGolryunBattlePacket_GB_REQUEST_END_MATCH::enumCode::loser);
	}


	
	if((NULL == Var.Winner.pPlayer) && (NULL == Var.Loser.pPlayer)){
		if(NULL != Var.Red.pPlayer){
			GTH_SendMessage_CompleteEffect_To_AllPC( Var.Red.pPlayer, 67 );
			m_cpEncoder->SendEffectResult(&(Var.Red.pPlayer->sock), 67);
			m_cpEncoder->SendEndMatch(&(Var.Red.pPlayer)->sock, tagGolryunBattlePacket_GB_REQUEST_END_MATCH::enumCode::loser);
		}

		if(NULL != Var.Blue.pPlayer){
			GTH_SendMessage_CompleteEffect_To_AllPC( Var.Blue.pPlayer, 67 );
			m_cpEncoder->SendEffectResult(&(Var.Blue.pPlayer->sock), 67);
			m_cpEncoder->SendEndMatch(&(Var.Blue.pPlayer)->sock, tagGolryunBattlePacket_GB_REQUEST_END_MATCH::enumCode::loser);
		}
		gcpGolryunBattleLog->Write("둘다 패배");
		ShowLogInfo("둘다 패배");	
		return;
	}

}


enum CGolryunBattle_Process::eTournamentStep 
CGolryunBattle_Process::GetTournamentStatus(void)
{
	return m_eTournamentStep;
}





void CGolryunBattle_Process::CheckMatchResult(const int WinnerFightNum)
{
	
	if(TournamentStep_PROGRESS_MATCH != GetTournamentStatus()) return;



	struct tagCheckNormalMatchParam Var;
	InitCheckNormalMatchParam(Var);
	CheckMatchResult_SetWinnerAndLoser(Var);
	

	if(0 == WinnerFightNum){
		Var.Winner.pPlayer  = NULL;
		Var.Winner.pFighter = NULL;
		Var.WinnerCorner   = CGolryunBattle_Data::tagMatch::enumCorner::None;

		Var.Loser.pPlayer  = NULL;
		Var.Loser.pFighter = NULL;	
		SetResultStatus(RESULT_DRAWN_MATCH);	
	}

	if((Var.Blue.pFighter)->FightNum == WinnerFightNum){
		
		Var.Winner.pPlayer  = Var.Blue.pPlayer;
		Var.Winner.pFighter = Var.Blue.pFighter;
		Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::Blue;
		Var.Loser.pPlayer = Var.Red.pPlayer;
		Var.Loser.pFighter = Var.Red.pFighter;			
		SetResultStatus(RESULT_NORMAL_MATCH);
	}

	if((Var.Red.pFighter)->FightNum == WinnerFightNum){		
		Var.Winner.pPlayer  = Var.Red.pPlayer;
		Var.Winner.pFighter = Var.Red.pFighter;
		Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::Red;
		Var.Loser.pPlayer = Var.Blue.pPlayer;
		Var.Loser.pFighter = Var.Blue.pFighter;			
		SetResultStatus(RESULT_NORMAL_MATCH);
	}

	CheckMatchResult_Effect(Var);



	if(m_iMatchIdx == eMatchIndexRound::End_MatchIdx_RoundFinal){
		memset(&m_FirstWinner,0x00,sizeof(struct CGolryunBattle_Data::tagFighter));
		m_FirstWinner.pcIdx=-1;

		memset(&m_SecondWinner,0x00,sizeof(struct CGolryunBattle_Data::tagFighter));
		m_SecondWinner.pcIdx=-1;


		if(NULL != Var.Winner.pFighter) m_FirstWinner = *(Var.Winner.pFighter);
		if(NULL != Var.Loser.pFighter)  m_SecondWinner = *(Var.Loser.pFighter);

		
	}	
	
	Var.pMatch->WinnerCorner = Var.WinnerCorner;	
	Var.pMatch->bComplete	 = TRUE;

	
	
	
	SetTournamentStatus(TournamentStep_END_MATCH);
	gcpGolryunBattleLog->Write("GB Status : GB_REQUEST_END_MATCH");
	return;	
}



void CGolryunBattle_Process::CheckGameStartMatch()
{
	struct CGolryunBattle_Data::tagMatch* pMatch = GetCurMatch();
	assert(NULL != pMatch);


	
	playerCharacter_t* pRedPlayer  = GetFighterPlayerPointer(&pMatch->RedCornerFighter);
	
	playerCharacter_t* pBluePlayer  = GetFighterPlayerPointer(&pMatch->BlueCornerFighter);
	

	if((NULL != pRedPlayer) && (NULL != pBluePlayer))	
	{		
		m_cpEncoder->SendEffectCount(&pRedPlayer->sock, 180);
		m_cpEncoder->SendEffectCount(&pBluePlayer->sock, 0);
		return;
	}


	CheckMatchResult();

	return;	
}

void CGolryunBattle_Process::TransWaitingRoomGM(playerCharacter_t* pc)
{
	if ( pc->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index ) return;

	int zoneIdx;
	vec3_t pos;

	pos[0] = 19591;
	pos[1] = 1966;
	pos[2] = 3072;

	vec_copy( pos, pc->position );
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	pc->zoneIdx = zoneIdx;
	
	GTH_SendPCEventMessage_Respawn( pc );
	GTH_SendMessage_SyncItemObject( pc );	

}

void CGolryunBattle_Process::TransWatchingRoomGM(playerCharacter_t* pc)
{
	if ( pc->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index ) return;

	int zoneIdx;
	vec3_t pos;

	pos[0] = 14390;
	pos[1] = 5787;
	pos[2] = 3456;

	vec_copy( pos, pc->position );
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	pc->zoneIdx = zoneIdx;
	
	GTH_SendPCEventMessage_Respawn( pc );
	GTH_SendMessage_SyncItemObject( pc );	

}

void CGolryunBattle_Process::TransBattleStadiumGM(playerCharacter_t* pc)
{
	if ( pc->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index ) return;

	int zoneIdx;
	vec3_t pos;

	pos[0] = 14709;
	pos[1] = 3680;
	pos[2] = 3252;

	vec_copy( pos, pc->position );
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	pc->zoneIdx = zoneIdx;
	
	GTH_SendPCEventMessage_Respawn( pc );
	GTH_SendMessage_SyncItemObject( pc );	

}

void CGolryunBattle_Process::SettingManorColor(playerCharacter_t* pc)
{
	if(pc->enumNameColor > m_eManor) return;
		
	switch(m_eManor){			
		case enumManor::week:
			pc->enumNameColor = playerCharacter_t::enumNameColor::week;
			break;

		case enumManor::month:
			pc->enumNameColor = playerCharacter_t::enumNameColor::month;
			break;

		case enumManor::ramification:
			pc->enumNameColor = playerCharacter_t::enumNameColor::ramification;
			break;

		case enumManor::year:
			pc->enumNameColor = playerCharacter_t::enumNameColor::year;
			break;
		default:
			pc->enumNameColor = playerCharacter_t::enumNameColor::normal;
			break;
	}	

}









	





inline BOOL CGolryunBattle_Process::inner_MoveToWatchingRoom(playerCharacter_t* pc)
{
	int zoneIdx;
	
	vec3_t pos;

	pos[0] = 14390;
	pos[1] = 5787;
	pos[2] = 3456;

	if ( pc->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index ) return FALSE;
	
	vec_copy( pos, pc->position );
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
	pc->zoneIdx = zoneIdx;	
	pc->pvpMode = false;
	GTH_SendPCEventMessage_Respawn( pc );
	GTH_SendMessage_SyncItemObject( pc );


	

	return TRUE;
}



BOOL CGolryunBattle_Process::MoveToWaitingRoom(playerCharacter_t* pPlayer)
{
	if( pPlayer->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index ) return FALSE;

	int zoneIdx=-1;
	int winner=-1;
	vec3_t pos;

	pos[0] = 19591;
	pos[1] = 1966;
	pos[2] = 3072;

	
	vec_copy( pos, pPlayer->position );
	zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pPlayer->idx, pPlayer->worldIdx, pPlayer->zoneIdx, pPlayer->position);
	pPlayer->zoneIdx = zoneIdx;
	pPlayer->pvpMode = false;
	GTH_SendPCEventMessage_Respawn( pPlayer);
	GTH_SendMessage_SyncItemObject( pPlayer);		

	
	pPlayer->GonryunBattle.m_bWaitingRoom = TRUE;

	

	return TRUE;

	
}





void CGolryunBattle_Process::ExtractJoinNumber(void)
{
	

	m_Ticket.iFighterNum=0;
	
	CRandList JoinerRandomList;
	JoinerRandomList.Initialize();	
	for(JOINTER_MAP_ITOR Itor = m_JoinMap.begin(); Itor != m_JoinMap.end(); Itor++)
		JoinerRandomList.PushData((Itor->second).pcIdx);


	InitFighterArray();
	



	int fighterIdx=-1;
	int pcIdx=-1;
	struct tagFightPlayerInfo* pTempFighter=NULL;
	playerCharacter_t* pTempPlayer=NULL;			

	for(fighterIdx=0; fighterIdx < tagGolryunBattle::Max_Num_Fighter_Per_Tournament; fighterIdx++){
		if(FALSE == JoinerRandomList.GetRandomData(pcIdx)) continue;	

		pTempFighter = &m_Fighter[fighterIdx];
		pTempPlayer=CGolryunBattle_Common::GetArenaPlayer(pcIdx);
		if(NULL == pTempPlayer) continue;

		pTempFighter->Idx		  = pTempPlayer->idx;		
		pTempFighter->Job		  = pTempPlayer->pcJob;
		pTempFighter->Record.win  = pTempPlayer->GonryunBattlePractice.Record.iwin;
		pTempFighter->Record.lose = pTempPlayer->GonryunBattlePractice.Record.idefeat;
		pTempFighter->Record.draw = pTempPlayer->GonryunBattlePractice.Record.idraw;			
		pTempPlayer->GonryunBattle.m_iJoinNum = fighterIdx+1;

		strncpy(pTempFighter->Name,pTempPlayer->name,NAMESTRING);
		pTempFighter->Name[NAMESTRING]=NULL;
		strncpy(pTempFighter->ID,pTempPlayer->userID,IDSTRING);
		pTempFighter->ID[IDSTRING]=NULL;
		m_Ticket.iFighterNum+=1;


		ShowLogInfo("UserPc %d %s ; JoinNumber %d", pTempPlayer->idx, pTempPlayer->name ,pTempPlayer->GonryunBattle.m_iJoinNum);
		gcpGolryunBattleLog->Write("UserPc %d %s ; JoinNumber %d", pTempPlayer->idx, pTempPlayer->name ,pTempPlayer->GonryunBattle.m_iJoinNum);

	}	

	m_cpEncoder->ManagerLocalUserNotice(151);	
}





void CGolryunBattle_Process::ExtractMatchNumber(void)
{
	int iStartMatchIdx=0, iEndMatchIdx=0;
	int iRoundIdx = MSG_ReadShort();

	switch(iRoundIdx)
	{
	case eRound::Round16:
		m_iMatchIdx=eMatchIndexRound::Offset_MatchIdx_Round16;
		iEndMatchIdx=eMatchIndexRound::Offset_MatchIdx_Round8;
		m_iOffSetMatchIdx=eMatchIndexRound::Offset_MatchIdx_Round8;		
		break;
	case eRound::Round8:
		m_iMatchIdx=eMatchIndexRound::Offset_MatchIdx_Round8;
		iEndMatchIdx=eMatchIndexRound::Offset_MatchIdx_Round4;
		m_iOffSetMatchIdx=eMatchIndexRound::Offset_MatchIdx_Round4;
		break;
	case eRound::Round4:
		m_iMatchIdx=eMatchIndexRound::Offset_MatchIdx_Round4;
		iEndMatchIdx=eMatchIndexRound::Offset_MatchIdx_RoundFinal;
		m_iOffSetMatchIdx=eMatchIndexRound::Offset_MatchIdx_RoundFinal;
		break;
	case eRound::Round2:
		m_iMatchIdx=eMatchIndexRound::Offset_MatchIdx_RoundFinal;
		iEndMatchIdx=tagGolryunBattle::Max_Num_Match_Per_Tournament;
		m_iOffSetMatchIdx=tagGolryunBattle::Max_Num_Match_Per_Tournament;	
		break;
	default:
		return;

	}

	CRandList MatchList;
	MatchList.Initialize();
	int iFighterNum;
	gcpGolryunBattleLog->Write("배팅총액 : %d",m_Ticket.iTotalNak);

	for(iFighterNum=1; iFighterNum <= iRoundIdx; iFighterNum++)
	{
		MatchList.PushData(iFighterNum);

		gcpGolryunBattleLog->Write("%d번 총액: %d 배당률: %f",
			iFighterNum, m_Ticket.Fighter[iFighterNum-1].iTotalNak, m_Ticket.Fighter[iFighterNum-1].fShareRate);
	}

	playerCharacter_t* pTempPlayer=NULL;
	struct CGolryunBattle_Data::tagMatch* pMatch=NULL;

	for(iStartMatchIdx=m_iMatchIdx; iStartMatchIdx < iEndMatchIdx; iStartMatchIdx++){

		pMatch=	GetMatchPointer(iStartMatchIdx);
		if(NULL == pMatch) continue;

		memset(pMatch,0x00,sizeof(struct CGolryunBattle_Data::tagMatch));
		pMatch->RedCornerFighter.pcIdx=-1;
		pMatch->RedCornerFighter.FightNum=-1;

		pMatch->BlueCornerFighter.pcIdx=-1;
		pMatch->BlueCornerFighter.FightNum=-1;


		if(FALSE == MatchList.GetRandomData(iFighterNum)) continue;		

		
		pMatch->RedCornerFighter.FightNum=iFighterNum;
		
		pTempPlayer = CGolryunBattle_Common::GetArenaPlayer(m_Fighter[iFighterNum-1].Idx);
		if(NULL != pTempPlayer){

			pMatch->RedCornerFighter.pcIdx= pTempPlayer->idx;			
			strncpy(pMatch->RedCornerFighter.id,pTempPlayer->userID,IDSTRING);
			pMatch->RedCornerFighter.id[IDSTRING]=NULL;
			strncpy(pMatch->RedCornerFighter.name,pTempPlayer->name,NAMESTRING);
			pMatch->RedCornerFighter.name[NAMESTRING]=NULL;		

			ShowLogInfo("UserName : %s, MatchNumber : %d",
				pMatch->RedCornerFighter.name, pMatch->RedCornerFighter.FightNum);

			gcpGolryunBattleLog->Write("UserName : %s, MatchNumber : %d",
				pMatch->RedCornerFighter.name, pMatch->RedCornerFighter.FightNum);

		}
		


		
		if(FALSE == MatchList.GetRandomData(iFighterNum)) continue;		
		
		pMatch->BlueCornerFighter.FightNum=iFighterNum;
		
		pTempPlayer = CGolryunBattle_Common::GetArenaPlayer(m_Fighter[iFighterNum-1].Idx);
		if(NULL != pTempPlayer){

			pMatch->BlueCornerFighter.pcIdx= pTempPlayer->idx;			
			strncpy(pMatch->BlueCornerFighter.id,pTempPlayer->userID,IDSTRING);
			pMatch->BlueCornerFighter.id[IDSTRING]=NULL;
			strncpy(pMatch->BlueCornerFighter.name,pTempPlayer->name,NAMESTRING);
			pMatch->BlueCornerFighter.name[NAMESTRING]=NULL;

			gcpGolryunBattleLog->Write("UserName : %s, MatchNumber : %d",
				pMatch->BlueCornerFighter.name, pMatch->BlueCornerFighter.FightNum);

			ShowLogInfo("UserName : %s, MatchNumber : %d",
				pMatch->BlueCornerFighter.name, pMatch->BlueCornerFighter.FightNum);
		}
	}

	m_cpEncoder->SendEndMatchNotice();
}


inline void CGolryunBattle_Process::UpdateHandleStatus_TournamentStep_PROGRESS_MATCH(void)
{
	int elasp_time=g_globalTime - m_BattleTimer.old_timeSync_transmite_Time;
	if(elasp_time >= tagBattleTimer::timeSyncPacket_Transmite_Time){				
		
		
		m_BattleTimer.old_timeSync_transmite_Time += tagBattleTimer::timeSyncPacket_Transmite_Time;
	
		
		if(g_globalTime - m_BattleTimer.start_time>=tagBattleTimer::match_start_count_time)
		{
			int matchTime = g_globalTime - m_BattleTimer.start_time - tagBattleTimer::match_start_count_time;
			int remainTime = tagBattleTimer::end_check_time - matchTime;
			if(remainTime<0) remainTime = 0;

			
			playerCharacter_t* pTempPlayer=NULL;
			for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++ ){
				pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
				if(NULL == pTempPlayer) continue;
				if((pTempPlayer->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index)) continue;
				if(FALSE == pTempPlayer->GonryunBattle.Observer.bActive)	continue;

				m_cpEncoder->SendObserveMatchTime(pTempPlayer, remainTime);
			
			}	

			
			struct CGolryunBattle_Data::tagMatch* pMatch=GetCurMatch();
			assert(NULL != pMatch);

			playerCharacter_t* pRedCornerPlayer=GetFighterPlayerPointer(&pMatch->RedCornerFighter);
			playerCharacter_t* pBlueCornerPlayer=GetFighterPlayerPointer(&pMatch->BlueCornerFighter);

			if(NULL == pRedCornerPlayer || NULL == pBlueCornerPlayer)	return;

			m_cpEncoder->SendPacketMatchTimePerSecond(pRedCornerPlayer, remainTime);
			m_cpEncoder->SendPacketMatchTimePerSecond(pBlueCornerPlayer, remainTime);
		
		}

		
		if(g_globalTime - m_BattleTimer.start_time >= tagBattleTimer::end_check_time + tagBattleTimer::match_start_count_time){	 
			
			
			CheckMatchResult();
			return;
		}					
	}



}

void CGolryunBattle_Process::UpdateHandleStatus(void)
{
	if(FALSE == isReady()) return;
	

	
	switch(GetTournamentStatus())
	{
		case TournamentStep_Match_Pause:
			break;
		

		
		case TournamentStep_START_BETTING:
			{	
				int elasp_time=g_globalTime - m_iStartTime;
					
				if(elasp_time >= tagBattleTimer::timeSyncPacket_Transmite_Time){
					
					m_iStartTime = g_globalTime;

					
					playerCharacter_t* pTempPlayer=NULL;
					for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++ ){
						pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
						if(NULL == pTempPlayer) continue;
						if((pTempPlayer->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index)) continue;
						m_cpEncoder->SendReNewShareRate(pTempPlayer);
					
					}	
				}
			}
			return;

		case TournamentStep_END_MATCH:
			{
				
				DWORD remainTime=g_globalTime - m_iStartTime;
				if ( remainTime < gGolryunBattleVar.GOLRYUNBATTLE_CEREMONY_WAIT_TIME)
					return;

				m_iStartTime	= g_globalTime; 			
				MoveToEndMatch();
			}
			return;


		
		case TournamentStep_START_COUNT:
			{			
				
				DWORD remainTime=g_globalTime-m_iStartTime;
				if ( remainTime < gGolryunBattleVar.GOLRYUNBATTLE_BEFORE_MATCH_WAIT_TIME) return;

				m_iStartTime	= g_globalTime; 
				m_BattleTimer.start_time=g_globalTime;
				m_BattleTimer.old_timeSync_transmite_Time=g_globalTime;

				
				SetTournamentStatus(TournamentStep_PROGRESS_MATCH);
				ShowLogInfo("Now Current GB Status : GB_REQUEST_PROGRESS_MATCH");

				gcpGolryunBattleLog->Write("Now Current GB Status : GB_REQUEST_PROGRESS_MATCH");

				MoveToStartMatch();
				FindOutFighterNumber();
			}
			return;

		
		case TournamentStep_PROGRESS_MATCH:
			UpdateHandleStatus_TournamentStep_PROGRESS_MATCH();
			return;
	}
}






inline void CGolryunBattle_Process::
InitCheckNormalMatchParam(struct tagCheckNormalMatchParam& Var)
{
	Var.pMatch		 = GetCurMatch();
	Var.Red.pFighter  = &(Var.pMatch->RedCornerFighter);
	Var.Blue.pFighter = &(Var.pMatch->BlueCornerFighter);
	Var.Red.pPlayer   = GetFighterPlayerPointer(Var.Red.pFighter);
	Var.Blue.pPlayer  = GetFighterPlayerPointer(Var.Blue.pFighter);

	Var.Winner.pPlayer= NULL;
	Var.Winner.pFighter=NULL;

	Var.Loser.pPlayer = NULL;
	Var.Loser.pFighter=NULL;
	Var.WinnerCorner = CGolryunBattle_Data::tagMatch::enumCorner::None;
};






inline void CGolryunBattle_Process::MoveToEndMatch(void)
{	
	MoveToEndMatch_Result();
	m_cpEncoder->SendEndMatchNotice();
	MoveToEndMatch_NextMatchProgress();

	if(FALSE == m_bPause) return;

	m_cpEncoder->SendMessageToGM(tagGolryunBattleMessage::PAUSE_MATCH);
	SetTournamentStatus(TournamentStep_Match_Pause);
	
}







playerCharacter_t* CGolryunBattle_Process::GetFighterPlayerPointer(const struct tagFightPlayerInfo* pFighter)
{	
	if(NULL == pFighter) return NULL;

	playerCharacter_t* pTempPlayer = CGolryunBattle_Common::GetArenaPlayer(pFighter->Idx);
	if(NULL == pTempPlayer) return NULL;

	if(0 != strcmp(pTempPlayer->name,pFighter->Name)) return NULL;
	if(0 != strcmp(pTempPlayer->userID,pFighter->ID)) return NULL;

	
	
	if(FALSE == pTempPlayer->GonryunBattle.m_bWaitingRoom) return NULL;

	return pTempPlayer;
}



playerCharacter_t* CGolryunBattle_Process::
GetFighterPlayerPointer(const struct CGolryunBattle_Data::tagFighter* pFighter)
{
	if(NULL == pFighter) return NULL;

	playerCharacter_t* pTempPlayer = CGolryunBattle_Common::GetArenaPlayer(pFighter->pcIdx);
	if(NULL == pTempPlayer) return NULL;

	if(0 != strcmp(pTempPlayer->name,pFighter->name)) return NULL;
	if(0 != strcmp(pTempPlayer->userID,pFighter->id)) return NULL;
	if(pTempPlayer->GonryunBattle.m_iJoinNum != pFighter->FightNum) return NULL;
	if(FALSE == pTempPlayer->GonryunBattle.m_bWaitingRoom) return NULL;

	return pTempPlayer;
}








void CGolryunBattle_Process::Enter_By_Tournament(playerCharacter_t* pPlayer)
{
	
	
	if(NULL == pPlayer) return;
	if(FALSE != pPlayer->GonryunBattle.m_bWaitingRoom ||
		0 != pPlayer->GonryunBattle.m_iJoinNum){
		gcpGolryunBattleLog->Write("참가중복오류 %s m_bWaitingRoom=%d m_iJoinNum=%d",pPlayer->name,pPlayer->GonryunBattle.m_bWaitingRoom,pPlayer->GonryunBattle.m_iJoinNum);	
		return;
	}
	
	if ( m_JoinMap.size() >= tagGolryunBattle::Max_Num_Fighter_Per_Tournament)
	{
		
		gcpGolryunBattleLog->Write("참가 인원 초과 %s",pPlayer->name);	
		return;
	}
	
	if(FALSE == MoveToWaitingRoom(pPlayer)){	
		gcpGolryunBattleLog->Write("참가신청을 했으나 알수없는 오류로 실패 %s",pPlayer->name);	
		return;		
	};

	
	

	struct CGolryunBattle_Data::tagFighter Fighter;
	strncpy(Fighter.id, pPlayer->userID,IDSTRING);   
	Fighter.id[IDSTRING]=NULL;
	strncpy(Fighter.name, pPlayer->name,NAMESTRING); 
	Fighter.name[NAMESTRING]=NULL;
	Fighter.pcIdx	=pPlayer->idx;
	Fighter.FightNum=0;


	pair<JOINTER_MAP_ITOR,BOOL> ret;
	ret=m_JoinMap.insert(JOINER_MAP::value_type(pPlayer->idx,Fighter));	
	if(FALSE == ret.second){
		gcpGolryunBattleLog->Write("m_JoinMap에 Insert오류. %s",pPlayer->name);	
		return;		
	}	
	
	m_AllJoinList.push_back(Fighter);		




													
	pPlayer->GonryunBattle.m_bWaitingRoom = TRUE;
	pPlayer->GonryunBattle.m_iJoinNum = 0;
	CGolryunBattle_Common::Change_Inventory_Nak((-1) * g_golryunManagerTable[m_iTournamentIdx].deposit,pPlayer);
	
	gcpGolryunBattleLog->Write("참가여부 Name: %s Level: %d m_bWaitingRoom=%d m_iJoinNum=%d",
		pPlayer->name,
		pPlayer->level,
		pPlayer->GonryunBattle.m_bWaitingRoom,
		pPlayer->GonryunBattle.m_iJoinNum);
	
	g_logSystem->WriteToLog( 
		pPlayer, 
		LOG_CLASS_GOLRYUN, 
		"%d;%d;%d;%d;%d;;", 
		CLogSystem::enumGolryunSubLog::JoinBattle,
		m_iTournamentGUID,
		m_cpData->m_iBattleGUID,
		m_iTournamentIdx,		
		g_golryunManagerTable[m_iTournamentIdx].deposit);
	
	
	if ( m_JoinMap.size() >= tagGolryunBattle::Max_Num_Fighter_Per_Tournament)
	{
		
		SetTournamentStatus(CGolryunBattle_Process::eTournamentStep::TournamentStep_CLOSE_JOIN);
		gcpGolryunBattleLog->Write("인원이 찼으므로 신청을 끝낸다. TournamentStep_CLOSE_JOIN");	
	}

	gcpGolryunBattleLog->Write("JoinCount : %d",m_JoinMap.size());
	return;
}






BOOL CGolryunBattle_Process::InsertJoinMap(playerCharacter_t* pPlayer)
{	
	if((FALSE != pPlayer->GonryunBattle.m_bWaitingRoom) ||
	   (0 != pPlayer->GonryunBattle.m_iJoinNum)){
		gcpGolryunBattleLog->Write("참가중복오류 %s m_bWaitingRoom=%d m_iJoinNum=%d",pPlayer->name,pPlayer->GonryunBattle.m_bWaitingRoom,pPlayer->GonryunBattle.m_iJoinNum);	
		return FALSE;
	}


	if(FALSE == MoveToWaitingRoom(pPlayer)){	
		gcpGolryunBattleLog->Write("참가신청을 했으나 알수없는 오류로 실패 %s",pPlayer->name);	
		return FALSE;		
	};
	

	struct CGolryunBattle_Data::tagFighter Fighter;
	strncpy(Fighter.id, pPlayer->userID,IDSTRING);   
	Fighter.id[IDSTRING-1]=NULL;
	strncpy(Fighter.name, pPlayer->name,NAMESTRING); 
	Fighter.name[NAMESTRING-1]=NULL;
	Fighter.pcIdx	=pPlayer->idx;
	Fighter.FightNum=0;


	pair<JOINTER_MAP_ITOR,BOOL> ret;
	ret=m_JoinMap.insert(JOINER_MAP::value_type(pPlayer->idx,Fighter));	
	if(FALSE == ret.second){
		gcpGolryunBattleLog->Write("m_JoinMap에 Insert오류. %s",pPlayer->name);	
		return FALSE;		
	}	
													
	pPlayer->GonryunBattle.m_bWaitingRoom = TRUE;
	pPlayer->GonryunBattle.m_iJoinNum = 0;
	

	gcpGolryunBattleLog->Write("참가여부 Name: %s Level: %d m_bWaitingRoom=%d m_iJoinNum=%d",
		pPlayer->name,
		pPlayer->level,
		pPlayer->GonryunBattle.m_bWaitingRoom,
		pPlayer->GonryunBattle.m_iJoinNum);
	
	g_logSystem->WriteToLog( 
		pPlayer, 
		LOG_CLASS_GOLRYUN, 
		"%d;%d;%d;%d;%d;;", 
		CLogSystem::enumGolryunSubLog::JoinBattle,
		m_iTournamentGUID,
		m_cpData->m_iBattleGUID,
		m_iTournamentIdx,		
		g_golryunManagerTable[m_iTournamentIdx].deposit);
	
	gcpGolryunBattleLog->Write("JoinCount : %d",m_JoinMap.size());
	return TRUE;
}


struct CGolryunBattle_Process::tagFightPlayerInfo* 
CGolryunBattle_Process::GetMatchFighter(playerCharacter_t* pPlayer)
{
	
	struct tagFightPlayerInfo* pTempFighter=NULL;

	for(int idx=0; idx < tagGolryunBattle::Max_Num_Fighter_Per_Tournament; idx++){
		pTempFighter = &m_Fighter[idx];
		if(pTempFighter->Idx < 0) continue;
		if(pPlayer->idx != pTempFighter->Idx) continue;

		if(0 != strcmp(pPlayer->name,pTempFighter->Name)){
			pTempFighter->Idx=-1;
			continue;
		}

		if(0 != strcmp(pPlayer->userID,pTempFighter->ID)){
			pTempFighter->Idx=-1;
			continue;
		}

		return pTempFighter;
	}

	return NULL;
};


struct CGolryunBattle_Data::tagMatch* 
CGolryunBattle_Process::GetMatch(const int FightNum)
{
	struct CGolryunBattle_Data::tagMatch* pMatch=NULL;
	for(int iMatchIdx=tagGolryunBattle::Max_Num_Match_Per_Tournament-1; iMatchIdx >=0 ; iMatchIdx--){
		pMatch=	GetMatchPointer(iMatchIdx);
		if(NULL == pMatch) continue;

		if(pMatch->RedCornerFighter.FightNum == FightNum) return pMatch;		
		if(pMatch->BlueCornerFighter.FightNum == FightNum) return pMatch;
	}

	return NULL;

};





BOOL CGolryunBattle_Process::AssignFightNum(const char* szpJoinerName,const int FighterNum)
{
	
	if(eTournamentStep::TournamentStep_Match_Pause != GetTournamentStatus())  return FALSE;

	if(FighterNum < 1) return FALSE;
	if(FighterNum > tagGolryunBattle::Max_Num_Fighter_Per_Tournament) return FALSE;


	struct CGolryunBattle_Data::tagMatch* pMatch=GetMatch(FighterNum);
	if(NULL == pMatch) return FALSE;

	playerCharacter_t* pTargetPlayer= gcpTools->GetPlayerRecordPointer(szpJoinerName);
	if(NULL == pTargetPlayer) return FALSE;


	JOINTER_MAP_ITOR itor;
	itor = m_JoinMap.find( pTargetPlayer->idx );
	if(itor == m_JoinMap.end()) return FALSE;
	


	struct tagFightPlayerInfo* pOldFighter=GetMatchFighter(pTargetPlayer);
	struct tagFightPlayerInfo* pNewFigther=&m_Fighter[FighterNum-1];	
	if(pOldFighter == pNewFigther) return FALSE;	
	if(pNewFigther->Idx <= 0) return FALSE;


	pNewFigther->Idx		  = pTargetPlayer->idx;		
	pNewFigther->Job		  = pTargetPlayer->pcJob;
	pNewFigther->Record.win  = pTargetPlayer->GonryunBattlePractice.Record.iwin;
	pNewFigther->Record.lose = pTargetPlayer->GonryunBattlePractice.Record.idefeat;
	pNewFigther->Record.draw = pTargetPlayer->GonryunBattlePractice.Record.idraw;			
	pTargetPlayer->GonryunBattle.m_iJoinNum = FighterNum;
	strncpy(pNewFigther->Name,pTargetPlayer->name,NAMESTRING);	pNewFigther->Name[NAMESTRING-1]=NULL;
	strncpy(pNewFigther->ID,pTargetPlayer->userID,IDSTRING);	pNewFigther->ID[IDSTRING-1]=NULL;


	if(NULL != pOldFighter){
		pOldFighter->Idx=-1;
	}

	if(pMatch->RedCornerFighter.FightNum == FighterNum){
		
		pMatch->RedCornerFighter.pcIdx= pTargetPlayer->idx;			
		strncpy(pMatch->RedCornerFighter.id,pTargetPlayer->userID,IDSTRING);
		pMatch->RedCornerFighter.id[IDSTRING-1]=NULL;
		strncpy(pMatch->RedCornerFighter.name,pTargetPlayer->name,NAMESTRING);
		pMatch->RedCornerFighter.name[NAMESTRING-1]=NULL;
	}

	if(pMatch->BlueCornerFighter.FightNum == FighterNum){
		
		pMatch->BlueCornerFighter.pcIdx= pTargetPlayer->idx;			
		strncpy(pMatch->BlueCornerFighter.id,pTargetPlayer->userID,IDSTRING);
		pMatch->BlueCornerFighter.id[IDSTRING-1]=NULL;
		strncpy(pMatch->BlueCornerFighter.name,pTargetPlayer->name,NAMESTRING);
		pMatch->BlueCornerFighter.name[NAMESTRING-1]=NULL;	
	}


	return TRUE;
}

BOOL CGolryunBattle_Process::ForceEjectToFighter(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer) return FALSE;

	int FighterNum = pPlayer->GonryunBattle.m_iJoinNum;
	if(FighterNum < 0) return FALSE;
	if(FighterNum > tagGolryunBattle::Max_Num_Fighter_Per_Tournament) return FALSE;

	JOINTER_MAP_ITOR joinerItor;
	joinerItor = m_JoinMap.find( pPlayer->idx );
	if(joinerItor == m_JoinMap.end()) return FALSE;
	

	playerCharacter_t* pTempPlayer=NULL;

	pTempPlayer = CGolryunBattle_Common::GetPlayer(joinerItor->second);
	if(NULL == pTempPlayer) return FALSE;
	if(pTempPlayer != pPlayer) return FALSE;

	inner_MoveToWatchingRoom(pTempPlayer);		

	if(FALSE == RemoveFighterArrayV2(pTempPlayer)){		
		pTempPlayer->GonryunBattle.m_bWaitingRoom = FALSE;
		pTempPlayer->GonryunBattle.m_iJoinNum=0;	
	}		

	joinerItor = m_JoinMap.erase(joinerItor);

	gcpGolryunBattleLog->Write("강제로 %s 를 퇴장했습니다.  m_bWaitingRoom=%d m_iJoinNum=%d", 
		pTempPlayer->name,
		pTempPlayer->GonryunBattle.m_bWaitingRoom,
		pTempPlayer->GonryunBattle.m_iJoinNum);	

	return TRUE;
}
