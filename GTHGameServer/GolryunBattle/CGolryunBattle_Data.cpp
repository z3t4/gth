




#include "..\global.h"
#include "CGolryunBattle_Data.h"
#include "CGolryunBattle_Common.h"
#include "CGolryunBattle_Process.h"




CGolryunBattle_Data::CGolryunBattle_Data()
{
	m_bOK=FALSE;


	m_iBattleGUID = 1;
	m_EntranceTicket.iMaxAllocGUID = 1;
	
	memset(&m_EntranceTicket.LotteryGUIDList, 0, sizeof(int)*tagGolryunBattle::Max_Num_Prize_List_Per_Battle);

	InitBattle();

	m_bOK=TRUE;

}




CGolryunBattle_Data::~CGolryunBattle_Data()
{
	
}






BOOL CGolryunBattle_Data::isOK(void) const
{
	return m_bOK;
}


BOOL CGolryunBattle_Data::InitBattle()
{
	

	struct tagTournament* pTournament=NULL;
	for(int TournamentIdx=0; TournamentIdx < tagGolryunBattle::Max_Num_Tournament_Per_Battle; TournamentIdx++){

		pTournament = &m_Tournament[TournamentIdx];
		memset(pTournament, 0x00, sizeof(struct tagTournament));


		struct tagMatch* pMatch=NULL;
		for(int MatchIdx=0; MatchIdx < tagGolryunBattle::Max_Num_Match_Per_Tournament; MatchIdx++){
			pMatch = &pTournament->Match[MatchIdx];

			pMatch->RedCornerFighter.pcIdx=-1;
			pMatch->RedCornerFighter.FightNum=-1;

			pMatch->BlueCornerFighter.pcIdx=-1;
			pMatch->BlueCornerFighter.FightNum=-1;				
			pMatch->WinnerCorner = tagMatch::enumCorner::None;
		}			
			
		pTournament->FirstWinner.pcIdx=-1;
		pTournament->FirstWinner.FightNum=-1;

		pTournament->SecondWinner.pcIdx=-1;
		pTournament->SecondWinner.FightNum=-1;

		
		pTournament->TwoGyun.iTotalNak=0;
		pTournament->TwoGyun.fShareRate=0.0f;
	}




	return TRUE;
}



struct CGolryunBattle_Data::tagTournament* 
CGolryunBattle_Data::GetTournamentPointer(const int Idx)
{
	if(Idx < 0) return NULL;
	if(Idx >= tagGolryunBattle::Max_Num_Tournament_Per_Battle) return NULL;

	return &m_Tournament[Idx];
}



