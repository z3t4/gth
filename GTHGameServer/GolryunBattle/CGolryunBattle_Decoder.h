



#if !defined(AFX_CGOLRYUNBATTLE_DECODER_H__5203D48E_E804_43E3_931B_92A46DA43E4C__INCLUDED_)
#define AFX_CGOLRYUNBATTLE_DECODER_H__5203D48E_E804_43E3_931B_92A46DA43E4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "CGolryunBattle_Encoder.h"
#include "CGolryunBattle_Process.h"
#include "CGolryunBattle_DBV2.h"


class CGolryunBattle_Decoder  
{
	private:
		CGolryunBattle_Encoder* m_cpEncoder;
		CGolryunBattle_Process* m_cpProcess;
		CGolryunBattle_DBV2*    m_cpDBV2;
		BOOL m_bOK;

	private:
		inline void Decoder_ProgressCommand(const int in_Command);
		inline void Decoder_ProgressCommand_GB_REQUEST_START_JOIN(void);
		inline void Decoder_ProgressCommand_GB_START_RECALL_FIGHTER(void);

	public:
		BOOL	    isOK(void) const;
		
		void		RecvNoticeBattle();
		void		RecvBattleManorIdx();
		
		void		RecvBattleTournamentIdx(const int in_nTournamentLevelIdx);
		void		RecvWalkOverMatch();
		void        RecvObserveEnd(playerCharacter_t* pPlayer);
		void		RecvOpenBattleTicket(playerCharacter_t* pPlayer);
		void		RecvOpenShareWin(playerCharacter_t* pPlayer);
		void		RecvDisplayTournament(playerCharacter_t* pPlayer);
		void		RecvForceEject(playerCharacter_t* pPlayer);
		void		RecvCheckStatusPvPMode(playerCharacter_t* pPlayer);	
			
		void		Decoder(void);

		inline void Decoder_GB_INSERT_JOIN_LIST(playerCharacter_t* pPlayer);
		inline void Decoder_GB_ASSIGN_FIGHTER_NUMBER(void);

		
		

	public:
		CGolryunBattle_Decoder(
			CGolryunBattle_DBV2* cpDBV2,
			CGolryunBattle_Encoder* cpEncoder,
			CGolryunBattle_Process* cpProcess);
		virtual ~CGolryunBattle_Decoder();

};

#endif 
